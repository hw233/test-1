#include "ClanCopy.h"
#include "Clan.h"
#include "MsgID.h"
#include "GData/ClanCopyTable.h"
#include "Player.h"
#include "Common/Stream.h"
#include "Country.h"
#include "Script/GameActionLua.h"
#include "Server/SysMsg.h"


namespace GObject
{




const static UInt32 COPY_ROUTE_COUNT = 3;  // 帮派副本的路径数目
const static UInt32 COPY_MIN_PLAYER = 5;
const static UInt32 COPY_EXTRA_PLAYER = 10;     // 怪物冲阵时玩家多拦截的数目（例如据点有人可以多拦怪的数量）
const static UInt32 MAX_TICK_COUNT = 100;

#ifdef DEBUG_CLAN_COPY
UInt32 clanCopyFileIndex = 1;
#endif

ClanCopy::ClanCopy(Clan *c, UInt32 copyId, Player * player) : _clan(c), _copyId  (copyId), _launchPlayer(player)
{
#ifdef DEBUG_CLAN_COPY
    char fileBuf[32];
    snprintf (fileBuf, 32, "clan_copy_%u.txt", clanCopyFileIndex++);
    fileSt = new std::fstream;
    fileSt->open(fileBuf, std::fstream::out);
#endif
    _copyLevel = c->getCopyLevel() + 1;
    _spotMap.clear();
    _spotMonster.clear();
    _spotPlayer.clear();
    _spotDeadPlayer.clear();
    _deadMonster.clear();
    _waitForWinPlayer.clear();
    _status = CLAN_COPY_READY;

    _readyTime = TimeUtil::Now();
    _startTime = 0;
    _endTime = 0;
    _tickTime = 0;

    _npcIndex = 1;

    _tickTimeInterval = 5;
    _startTick = 1;
    _monsterRefreshTick = 3;
    _tickCount = 0;

    _maxMonsterWave = 20;
    _curMonsterWave = 0;

    _observerPlayer.clear();
#ifdef DEBUG_CLAN_COPY
    *fileSt << "clan_copy_log:" << std::endl;
#endif
    initCopyData();
}

ClanCopy::~ClanCopy()
{
    _clan->clearCopySnap();
    for (std::map<Player *, UInt8>::iterator it = _playerIndex.begin(); it != _playerIndex.end(); ++ it)
    {
        it->first->regenAll();
        _clan->insertIntoCopySnap(it->first, it->second);
    }
    for (SpotMonster::iterator it = _spotMonster.begin(); it != _spotMonster.end(); ++it)
    {
        for (SpotMonsterList::iterator it2 = it->second.begin(); it2 != it->second.end(); ++ it2)
        {
            _deadMonster.insert(*it2);
        }
    }
#ifdef DEBUG_CLAN_COPY
    UInt32 count = 0;
#endif
    for (std::set<ClanCopyMonster *>::iterator it = _deadMonster.begin(); it != _deadMonster.end(); ++ it)
    {
#ifdef DEBUG_CLAN_COPY
        ++ count;
#endif
        delete *it;
    }

    _spotMap.clear();
    _spotMonster.clear();
    _spotPlayer.clear();
    _spotDeadPlayer.clear();
    _observerPlayer.clear();
#ifdef DEBUG_CLAN_COPY
    * fileSt << count <<  " monsters destoryed." << std::endl;
    * fileSt << "clan copy destoryed." << std::endl << std::endl << std::endl;
    fileSt->close();
    delete fileSt;
#endif
}

UInt8 ClanCopy::getStatus()
{
    // 返回副本状态
    return _status;
}

UInt16 ClanCopy::getTotalPlayer()
{
    return _playerIndex.size();
}

void ClanCopy::addPlayer(Player * player, bool needNotify /* = true */)
{
    // 增加帮派副本玩家人数 （仅在准备状态有效）
    if (_status != CLAN_COPY_READY)
        return;
    std::set<Player*>::iterator obIt = _observerPlayer.find(player);
    if (obIt!= _observerPlayer.end())
    {
        // 从围观群众中移除
#ifdef DEBUG_CLAN_COPY
        *fileSt << "observer \"" << player->getName() << "\" change to player." << std::endl;
#endif
        _observerPlayer.erase(obIt);
    }

    // 先寻找上次副本结束时快照，来快速将玩家放入历史据点

    if (UInt8 spotId = _clan->getCopyPlayerSnap(player))
    {
        if (_spotPlayer[spotId].size() < _spotMap[spotId].maxPlayerCount)
        {
            // 加入某一据点的玩家列表中
            _spotPlayer[spotId].push_back(ClanCopyPlayer(player));
            _playerIndex.insert(std::make_pair(player, spotId));
            updateSpotBufferValue(spotId);
#ifdef DEBUG_CLAN_COPY
            *fileSt << "\"" << player->getName() << "\" change to spot(" << (UInt32) spotId << ")." << std::endl;
#endif
            if (needNotify)
            {
                notifySpotPlayerInfo();
            }
            return;
        }
    }

    for (SpotMap::iterator mapIt = _spotMap.begin(); mapIt != _spotMap.end(); ++mapIt)
    {
        //一个一个据点的找空位置
        if (mapIt->first == Enemy_Base)
            continue;
        SpotPlayer::iterator playerIt = _spotPlayer.find(mapIt->first);
        if (playerIt == _spotPlayer.end())
            return;
        if ((playerIt->second).size() >= (mapIt->second).maxPlayerCount)
            continue;
        else
        {
            // 加入某一据点的玩家列表中
            (playerIt->second).push_back(ClanCopyPlayer(player));
            _playerIndex.insert(std::make_pair(player, mapIt->first));
            updateSpotBufferValue(mapIt->first);
#ifdef DEBUG_CLAN_COPY
            *fileSt << "\"" << player->getName() << "\" change to spot(" << (UInt32) mapIt->first << ")." << std::endl;
#endif
            if (needNotify)
            {
                notifySpotPlayerInfo();
            }
            return;
        }
    }
    player->sendMsgCode(0, 1351);
}

void ClanCopy::addPlayerFromSpot(Clan* c)
{
    // 将据点内所在的玩家拉近副本中
    class autoJoinVisitor : public Visitor<ClanMember>
    {
        public:
            autoJoinVisitor()
            {
            }

            bool operator()(ClanMember* member)
            {
                ClanCopyMgr::Instance().playerEnter(member->player, false);
                return true;
            }
    };
    autoJoinVisitor visitor;
    _clan->VisitMembers(visitor);
    notifySpotPlayerInfo();
}

void ClanCopy::addObserver(Player * player)
{
    // 增加帮派副本围观人员（不明真相的围观群众）
    if (_observerPlayer.find(player) != _observerPlayer.end())
    {
        TRACE_LOG("ClanCopy: addObserver error1.");
        return;    // 已经是围观人员，不需要再次加入（这里也是异常流程）
    }
    if (_playerIndex.find(player) != _playerIndex.end())
    {
        TRACE_LOG("ClanCopy: addObserver error2.");
        return;    // 已经是副本战斗人员（这里也是异常流程）
    }
    _observerPlayer.insert(player);
#ifdef DEBUG_CLAN_COPY
    *fileSt << "observer \"" << player->getName() << "\" add." << std::endl;
#endif
    notifySpotPlayerInfo(player);
    return;
}

void ClanCopy::playerEscape(Player *player)
{
    // 玩家逃跑
    std::set<Player *>::iterator obIt = _observerPlayer.find(player);
    if (obIt != _observerPlayer.end())
    {
        // 围观群众散去
        _observerPlayer.erase(obIt);
#ifdef DEBUG_CLAN_COPY
        *fileSt << "observer \"" << player->getName() << "\" leave." << std::endl;
#endif
        return;
    }

    if (_status == CLAN_COPY_READY)
    {
        if (player == _launchPlayer)
        {
            // 发起者逃跑，副本作废
            _status = CLAN_COPY_OVER;
#ifdef DEBUG_CLAN_COPY
            *fileSt << "Launcher \"" << player->getName() << "\" leave." << std::endl;
#endif
            notifyLauncherEscape();
            Stream st2;
            SYSMSGVP(st2, 807, _launchPlayer->getName().c_str());
            _clan->broadcast(st2);
            return;
        }
        else
        {
            // 其他玩家逃跑
            std::map<Player *, UInt8>::iterator playerIndexIt = _playerIndex.find(player);
            if (playerIndexIt != _playerIndex.end())
            {
                UInt8 spotId = playerIndexIt->second;
                for (SpotPlayerList::iterator spotPlayerListIt = _spotPlayer[spotId].begin(); spotPlayerListIt != _spotPlayer[spotId].end(); ++ spotPlayerListIt)
                {
                    if (spotPlayerListIt->player == player)
                    {
                        // 该玩家在该据点
                        spotPlayerListIt->deadType = 2;  // 该玩家已逃跑
                        spotPlayerListIt->player->regenAll();    // 生命值回满
                        _spotPlayer[spotId].erase(spotPlayerListIt);
#ifdef DEBUG_CLAN_COPY
                        *fileSt << "\"" << player->getName() << "\" escape from spot " << (UInt32)spotId << " when ready." << std::endl;
#endif
                        _playerIndex.erase(playerIndexIt);
                        updateSpotBufferValue(spotId);
                        notifySpotPlayerInfo();
                        if (_playerIndex.empty())
                        {
                            _status = CLAN_COPY_OVER;
                        }
                        return;
                    }
                }
            }
            else
            {
#ifdef DEBUG_CLAN_COPY
                *fileSt << "\"" << player->getName() << "\" is not in playerIndex." << std::endl;
#endif
                TRACE_LOG("ClanCopy: playerEscape error.");

            }

        }
    }
    else
    {

        std::map<Player *, UInt8>::iterator playerIndexIt = _playerIndex.find(player);
        if (playerIndexIt != _playerIndex.end())
        {
            // 游戏中玩家逃跑
            UInt8 spotId = playerIndexIt->second;
            for (SpotPlayerList::iterator spotPlayerListIt = _spotPlayer[spotId].begin(); spotPlayerListIt != _spotPlayer[spotId].end(); ++ spotPlayerListIt)
            {
                // 该玩家在该据点（活的）
                if (spotPlayerListIt->player == player)
                {
                    spotPlayerListIt->deadType = 2;  // 该玩家已逃跑
                    _spotDeadPlayer[spotId].push_back (*spotPlayerListIt); // 加入死亡名单
                    spotPlayerListIt->player->regenAll();    // 生命值回满
                    _spotPlayer[spotId].erase(spotPlayerListIt);
#ifdef DEBUG_CLAN_COPY
                    *fileSt << "\"" << player->getName() << "\" escape from spot " << (UInt32)spotId << "." << std::endl;
#endif
                    _playerIndex.erase(playerIndexIt);
                    updateSpotBufferValue(spotId);
                    notifySpotPlayerInfo();
                    if (_playerIndex.empty())
                    {
                        _status = CLAN_COPY_LOSE;
                        notifyCopyLose();
                        _status = CLAN_COPY_OVER;
                    }
                    return;
                }
            }

            for (SpotPlayerList::iterator spotPlayerListIt = _spotDeadPlayer[spotId].begin(); spotPlayerListIt != _spotDeadPlayer[spotId].end(); ++ spotPlayerListIt)
            {
                // 该玩家在该据点（战死）
                if (spotPlayerListIt->player == player)
                {
                    spotPlayerListIt->deadType = 2;  // 该玩家已逃跑
                    spotPlayerListIt->player->regenAll();    // 生命值回满
                    _waitForWinPlayer.push_back(spotPlayerListIt->player);
                    _spotDeadPlayer[spotId].erase(spotPlayerListIt);
#ifdef DEBUG_CLAN_COPY
                    *fileSt << "Dead player \"" << player->getName() << "\" escape from spot " << (UInt32)spotId << "." << std::endl;
#endif
                    _playerIndex.erase(playerIndexIt);
                    notifySpotPlayerInfo();
                    if (_playerIndex.empty())
                    {
                        _status = CLAN_COPY_OVER;
                    }
                    return;
                }
            }
#ifdef DEBUG_CLAN_COPY
            *fileSt << "\"" << player->getName() << "\" is not in spot " << (UInt32)spotId << "." << std::endl;
#endif
        }
    }
}

void ClanCopy::initCopyData()
{
    // 读取副本配置数据，初始化副本状态
#ifdef DEBUG_CLAN_COPY
    *fileSt << "initCopyData..." << std::endl;
#endif
    _homeMaxHp = _homeHp = GData::clanCopyTable[_copyLevel].homeHp;
    _maxReward = GData::clanCopyTable[_copyLevel].maxReward;
    _maxMonsterWave = GData::clanCopyTable[_copyLevel].maxWaveCount;

    ClanCopySpot clanCopySpot;
    SpotPlayerList spotPlayerList;
    SpotMonsterList spotMonsterList;
    for (UInt8 i = 0; i < SPOT_COUNT; ++ i)
    {
#ifdef DEBUG_CLAN_COPY
        *fileSt << "init spot id(" << (UInt32) SpotId[i] << ")." << std::endl;
#endif
        clanCopySpot.spotId = SpotId[i];
        clanCopySpot.spotBufferType = SpotBufferId[i];
        clanCopySpot.spotBufferValue = 0;
        clanCopySpot.nextSpotId.clear();
        clanCopySpot.nextMoveTick = NextMoveTick[i];
        for (UInt8 j = 0; j < NextSpotNum[i]; ++ j)
        {
            clanCopySpot.nextSpotId.push_back(NextSpotId[i][j]);
#ifdef DEBUG_CLAN_COPY
            *fileSt << "nextSpot id = " << (UInt32) NextSpotId[i][j] << "." << std::endl;
#endif
        }
        _spotMap.insert (std::make_pair(SpotId[i], clanCopySpot));
        _spotMonster.insert (std::make_pair(SpotId[i], spotMonsterList));
        _spotPlayer.insert (std::make_pair(SpotId[i], spotPlayerList));
        _spotDeadPlayer.insert (std::make_pair(SpotId[i], spotPlayerList));
    }
#ifdef DEBUG_CLAN_COPY
    *fileSt << "initCopyData complete." << std::endl << std::endl;
#endif
}

void ClanCopy::setInterval(UInt32 interval)
{
    // 设置游戏节奏时间
    if (!interval)
        return;
    _tickTimeInterval = interval;
}

void ClanCopy::setStartTick(UInt32 tickCount)
{
    //  设置副本第一轮刷怪时间 （节奏数目）
    _startTick = tickCount;
}

void ClanCopy::setStartTimeInterval(UInt32 startTimeInterval)
{
    // 设置副本第一轮刷怪的时间（秒数）
    _startTick = startTimeInterval / _tickTimeInterval;
}

void ClanCopy::updateBufferAttr(UInt8 spotId)
{
    // 根据据点人数更新属性附加值
    switch(_spotMap[spotId].spotBufferType)
    {
        case 0x01:
            _spotAttrs[spotId].defendP = _spotMap[spotId].spotBufferValue;
            break;
        case 0x11:
            _spotAttrs[spotId].actionP = _spotMap[spotId].spotBufferValue;
            break;
        case 0x21:
            _spotAttrs[spotId].hpP = _spotMap[spotId].spotBufferValue;
            break;
        case 0x31:
            _spotAttrs[spotId].attackP = _spotMap[spotId].spotBufferValue;
            _spotAttrs[spotId].magatkP = _spotMap[spotId].spotBufferValue;
            break;
        case 0xff:
            break;
    }
}

void ClanCopy::updateSpotBufferValue(UInt8 spotId)
{
    // 根据人数更新据点buffer加成
    UInt8 count = 0;
    if (_status != CLAN_COPY_PROCESS)
        count = _spotPlayer[spotId].size();
    else
    {
        for (SpotPlayerList::iterator it = _spotPlayer[spotId].begin(); it != _spotPlayer[spotId].end(); ++ it)
        {
            if (!it->deadType)
                ++ count;
        }
    }
    _spotMap[spotId].spotBufferValue = GData::clanCopySpotMap[spotId].bufferValue[count];
    updateBufferAttr(spotId);
}

void ClanCopy::adjustPlayerPosition(Player * opPlayer, Player* player, UInt8 oldSpotId, UInt8 oldPosition, UInt8 newSpotId, UInt8 newPosition)
{
    // 调整玩家位置
    if (_status != CLAN_COPY_READY)
        return;
#ifdef DEBUG_CLAN_COPY
    *fileSt << "Operater : " << opPlayer->getName() << "."  << std::endl;
    *fileSt << "Move player \"" << player->getName() << "\" from (" << (UInt32) oldSpotId << ", " << (UInt32) oldPosition << ") \
        to (" << (UInt32) newSpotId << ", " << (UInt32) newPosition << ")." << std::endl;
#endif

    std::map<Player *, UInt8>::iterator playerIndexIt = _playerIndex.find(opPlayer);
    if (playerIndexIt == _playerIndex.end())
    {
        opPlayer->sendMsgCode(0, 1346);
#ifdef DEBUG_CLAN_COPY
        *fileSt << "Error1." << std::endl;
#endif
        return;
    }

    playerIndexIt = _playerIndex.find(player);

    if (playerIndexIt == _playerIndex.end())
    {
#ifdef DEBUG_CLAN_COPY
        *fileSt << "Error2." << std::endl;
#endif
        return;
    }

    if (oldSpotId != playerIndexIt->second)
    {
        notifySpotPlayerInfo(opPlayer);
#ifdef DEBUG_CLAN_COPY
        *fileSt << "Error3." << std::endl;
#endif
        return;
    }

    UInt8 index = 0;
    for (SpotPlayerList::iterator spotPlayerListIt = _spotPlayer[oldSpotId].begin(); spotPlayerListIt != _spotPlayer[oldSpotId].end(); ++ spotPlayerListIt)
    {
        ++ index;
        if (index == oldPosition)
        {
            if (spotPlayerListIt->player == player)
            {
                // 玩家在原位置，调整至新位置
                if (newSpotId != oldSpotId)
                {
                    // 调整至新据点（默认放在末尾）
                    if (_spotPlayer[newSpotId].size() >= _spotMap[newSpotId].maxPlayerCount)
                    {
                        opPlayer->sendMsgCode(0, 1348);  // 新据点位置玩家数已满
#ifdef DEBUG_CLAN_COPY
                        *fileSt << "Error4." << std::endl;
#endif
                        return;
                    }
                    _spotPlayer[newSpotId].push_back(*spotPlayerListIt);
                    _spotPlayer[oldSpotId].erase(spotPlayerListIt);
                    _playerIndex[player] = newSpotId;
                    updateSpotBufferValue(oldSpotId);
                    updateSpotBufferValue(newSpotId);
                    notifySpotPlayerInfo();
                    return;
                }
                else
                {
                    // 在本据点调整
                    if (!newPosition)
                    {
                        opPlayer->sendMsgCode(0, 1349);
                        notifySpotPlayerInfo(opPlayer);
#ifdef DEBUG_CLAN_COPY
                        *fileSt << "Error5." << std::endl;
#endif
                        return;
                    }
                    else
                    {
                        ClanCopyPlayer clanCopyPlayer = *spotPlayerListIt;
                        _spotPlayer[oldSpotId].erase(spotPlayerListIt);
                        index = 0;
                        for (spotPlayerListIt = _spotPlayer[oldSpotId].begin(); spotPlayerListIt != _spotPlayer[oldSpotId].end(); ++ spotPlayerListIt)
                        {
                            ++ index;
                            if (index == newPosition)
                            {
                                _spotPlayer[newSpotId].insert(spotPlayerListIt, clanCopyPlayer);
                                notifySpotPlayerInfo();
                                return;
                            }
                        }
                        _spotPlayer[newSpotId].push_back(clanCopyPlayer);
                        notifySpotPlayerInfo();
                        return;
                    }
                }
            }
            else
            {
                // 该玩家已不再这个位置，刷新据点玩家列表
                opPlayer->sendMsgCode(0, 1347);
                notifySpotPlayerInfo(opPlayer);
                return;
            }
        }
    }
}

void ClanCopy::requestStart(Player * player)
{
    // 玩家请求开始副本战斗
    if (_playerIndex.find(player) == _playerIndex.end())
        return;
    if (_status != CLAN_COPY_READY)
    {
#ifdef DEBUG_CLAN_COPY
        *fileSt << "_status = " << (UInt32) _status << " when requestStart." << std::endl;
#endif
        player->sendMsgCode(0, 1345); // 请求阶段错误
        return;
    }
    if (_playerIndex.size() < COPY_MIN_PLAYER)
    {
#ifdef DEBUG_CLAN_COPY
        *fileSt << "\"" << player->getName() << "\" start failed (not enough people)." << std::endl;
#endif
        player->sendMsgCode(0, 1356); // 人数不足
        return;
    }
    _status = CLAN_COPY_PROCESS;
    _tickTime = _startTime = TimeUtil::Now();
#ifdef DEBUG_CLAN_COPY
    *fileSt << "\"" << player->getName() << "\" start clan copy." << std::endl;
#endif

    for (std::map<Player*, UInt8>::iterator it = _playerIndex.begin(); it != _playerIndex.end(); ++ it)
    {
        it->first->clanCopyUdpLog(1131);
    }
    UInt32 sday = TimeUtil::SharpDay();
    UInt32 hour = (_startTime - sday) / 3600;
    _launchPlayer->clanCopyUdpLog(1137, static_cast<UInt8>(hour));
    notifySpotBattleInfo();
}

void ClanCopy::process(UInt32 now)
{
    // 游戏状态进行
#ifdef DEBUG_CLAN_COPY
    static UInt32 count = 0;
#endif

    if (_status != CLAN_COPY_PROCESS)
    {
        if (_status == CLAN_COPY_LOSE || _status == CLAN_COPY_WIN)
        {
            // 这个是怎么进来的？
            WARN_LOG("ClanCopy: _status = %d.", (UInt32) _status);
            _status = CLAN_COPY_OVER;
        }
#ifdef DEBUG_CLAN_COPY
        count = 0;
#endif
        return;
    }

    if (_tickCount > MAX_TICK_COUNT)
    {
            // 这个又是怎么进来的？
            WARN_LOG("ClanCopy: _tickCount = %d.", (UInt32) _tickCount);
            _status = CLAN_COPY_OVER;
            return;
    }

    if ((now - _tickTime) < _tickTimeInterval)
        return;    // 未到达触发下一状态的时间
    _tickTime = now;

#ifdef DEBUG_CLAN_COPY
    *fileSt << "process (" << ++ count << ") start: " << std::endl;
    *fileSt << "status = " << (UInt32)_status << "." << std::endl;
#endif

    for (SpotMap::reverse_iterator rit = _spotMap.rbegin(); rit != _spotMap.rend(); ++rit)
    {
        _spotBattleInfo[rit->first].clear();
        if (rit->first == Enemy_Base)
            enemyBaseAct();
        else
            monsterAssault(rit->first);
    }

    if (_status == CLAN_COPY_LOSE)
    {
        notifySpotBattleInfo();
        notifyCopyLose();
        _status = CLAN_COPY_OVER;
        return;
    }

    //　检查是否已经胜利
    if (_tickCount >= _maxMonsterWave * _monsterRefreshTick + _startTick)
    {
        if (checkWin())
        {
            UInt32 awardValue = static_cast<UInt32> (static_cast<float>(_homeHp) / _homeMaxHp * _maxReward);
            notifySpotBattleInfo();
            addWinReward(awardValue);
            _clan->addCopyLevel();
            _clan->addCopyWinLog(_launchPlayer);
            _status = CLAN_COPY_OVER;
            return;
        }
    }
    notifySpotBattleInfo();
    ++_tickCount;
#ifdef DEBUG_CLAN_COPY
    *fileSt << "tickCount = " << _tickCount << std::endl;
    *fileSt << "process (" << count << ") end: " << std::endl;
    *fileSt << "status = " << (UInt32)_status << "." << std::endl << std::endl;
#endif
}

void ClanCopy::enemyBaseAct()
{
    // 敌人老巢的行动
    monsterMove(Enemy_Base); // 上一轮产生的怪物先行移动
    if (_curMonsterWave * _monsterRefreshTick + _startTick > _tickCount)
    {
        // 还未到新一轮刷怪时间
#ifdef DEBUG_CLAN_COPY
        *fileSt << "enemyBase bye." << std::endl;
#endif
        return;   // 未到达刷怪时间
    }
    if (_curMonsterWave < _maxMonsterWave)
    {
        // 刷新一轮怪
        ++ _curMonsterWave;
#ifdef DEBUG_CLAN_COPY
        *fileSt << "enemyBase create " << (UInt32) _curMonsterWave << " wave monster." << std::endl;
#endif
        createEnemy();
    }
    else
    {
        // 最后一波怪刷完后需要的冲阵
    }
}

void ClanCopy::createEnemy()
{
    // 敌人老巢产生敌人
    UInt32 key = (((static_cast<UInt32>(_copyLevel)) << 16) | _curMonsterWave) << 8;
    UInt8 routeCount = _spotMap[Enemy_Base].nextSpotId.size();
    UInt8 routeBit = 0;  // 路径bit图
    if (routeCount >= 8)
    {
#ifdef DEBUG_CLAN_COPY
        *fileSt << "routeCount error." << std::endl;
#endif
        TRACE_LOG("ClanCopy: RouteCount error. routeCount = %u", (UInt32) routeCount);
    }

    for (UInt8 i = 0; i < routeCount; ++ i)
    {
        // 首先产生固定路径的怪物
        GData::ClanCopyMonsterMap::iterator it = GData::clanCopyMonsterMap.find(key | (i + 1));
        if (it == GData::clanCopyMonsterMap.end())
        {
            // 该路径没有固定怪物
        }
        else
        {
            // 固定路径产生怪物
            createEnemyToSpotId(_spotMap[Enemy_Base].nextSpotId[i], it->second);
            routeBit |= 0x01 << i;
        }
    }

    GData::ClanCopyMonsterMap::iterator it = GData::clanCopyMonsterMap.lower_bound(key);
    GData::ClanCopyMonsterMap::iterator endIt = GData::clanCopyMonsterMap.upper_bound(key);

    // 如果这里配置表出错，怪物大于剩余的空余路数，那就不知道会出现什么错误
    UInt8 count = GData::clanCopyMonsterMap.count(key);

    while (it != endIt)
    {
        // 随机在一路产生怪物
        UInt8 i = _rnd(count) + 1;
        UInt8 index = 0;
        UInt8 tempBit = routeBit;
        do{
            if (!(tempBit & 0x01))
            {
                if (i == 1)
                {
                    createEnemyToSpotId(_spotMap[Enemy_Base].nextSpotId[index], it->second);
                    routeBit |= 0x01 << index;
                    break;
                }
                else
                {
                    --i;
                }
            }
            tempBit >>= 1;
            ++index;

        }while (i);
        -- count;
        ++it;
    }
}

void ClanCopy::createEnemyToSpotId(UInt8 spotId, const GData::ClanCopyMonsterData& clanCopyMonsterData)
{
     // 设置该轮怪物参数
#ifdef DEBUG_CLAN_COPY
    *fileSt << "New monster created. npcId = " << clanCopyMonsterData.npcId << "." << std::endl;
#endif
    // 老巢中放入怪物
    for (UInt16 count = 0; count < clanCopyMonsterData.npcCount; ++ count)
    {
        ClanCopyMonster* clanCopyMonster = new ClanCopyMonster(_npcIndex ++, clanCopyMonsterData.npcId, clanCopyMonsterData.monsterType, _curMonsterWave, clanCopyMonsterData.npcValue, 0, _tickCount + 1, _copyLevel);
        clanCopyMonster->nextSpotId = spotId;
        clanCopyMonster->npcIndex = _npcIndex ++;
        _spotMonster[Enemy_Base].push_back(clanCopyMonster);
    }
#ifdef DEBUG_CLAN_COPY
    *fileSt << "Next spot Id: " << (UInt32)spotId << "." << std::endl;
    *fileSt << (UInt32)clanCopyMonsterData.npcCount << std::endl;
#endif
}

void ClanCopy::monsterAssault(UInt8 spotId)
{
    // 据点战斗或者怪物移动
#ifdef DEBUG_CLAN_COPY
    *fileSt << "Monster assault." << std::endl;
#endif

    if (_spotMap[spotId].nextMoveTick > _tickCount )
    {
        // 仍然在战斗阶段
        spotCombat(spotId);
    }
    else
    {
        // 怪物冲锋阶段
        monsterMove(spotId);
        spotCombat(spotId);
        _spotMap[spotId].nextMoveTick = _tickCount + _monsterRefreshTick;
    }
#ifdef DEBUG_CLAN_COPY
    *fileSt << "Monster assault end." << std::endl;
#endif
}

void ClanCopy::spotCombat(UInt8 spotId)
{
    // 开始一轮战斗
#ifdef DEBUG_CLAN_COPY
    *fileSt << "SpotCombat (" << (UInt32)spotId <<") ." << std::endl;
#endif
    SpotPlayerList::iterator playerIt = _spotPlayer[spotId].begin();
    SpotMonsterList::iterator monsterIt = _spotMonster[spotId].begin();
    UInt8 count = 0;

    bool flag = false;// 是否有新怪物达到，需要冲阵的标志位

    while(monsterIt != _spotMonster[spotId].end())
    {
        if ((*monsterIt)->justMoved)
        {
            (*monsterIt)->justMoved = false;
            (*monsterIt)->deadType = 0;
            flag = true;
        }
        else if((*monsterIt)->deadType)
        {
            _deadMonster.insert(*monsterIt);
        }
        ++ monsterIt;
    }

    if (_spotMap[spotId].nextMoveTick == (_tickCount + 1) )
        flag = true;
    else
        flag = false;

    if (flag)
    {
        // 需要重新冲阵
#ifdef DEBUG_CLAN_COPY
        *fileSt << "Start monsters match players." << std::endl;
#endif
        _spotMonster[spotId].remove_if(isMonsterDead());

        for (playerIt = _spotPlayer[spotId].begin(); playerIt != _spotPlayer[spotId].end(); ++ playerIt)
        {
            if (playerIt->deadType)
                _spotDeadPlayer[spotId].push_back(*playerIt);
        }
        _spotPlayer[spotId].remove_if(isPlayerDead());
    }

    playerIt = _spotPlayer[spotId].begin();
    monsterIt = _spotMonster[spotId].begin();


    while (monsterIt != _spotMonster[spotId].end() && count < BATTLER_COUNT)
    {

        // 死怪，重新查找下一个怪物
        if ((*monsterIt)->deadType == 2)
        {
            (*monsterIt)->deadType = 1;
            ++ monsterIt;
            continue;
        }
        if ((*monsterIt)->deadType == 1)
        {
            ++ monsterIt;
            continue;
        }

        // 怪物一一与玩家匹配直到没有怪物
        while (playerIt != _spotPlayer[spotId].end())
        {
            if (playerIt->deadType)
            {
                ++playerIt;
                continue;
            }
            else
            {
                // 找到可以匹配的玩家与怪物战斗
#ifdef DEBUG_CLAN_COPY
                *fileSt << "Find player to fight to monster." << std::endl;
#endif
                (*monsterIt)->nextSpotId = spotId;
                Battle::BattleSimulator bsim(Battle::BS_COPY5, playerIt->player, (*monsterIt)->name, (*monsterIt)->level, false);
                playerIt->player->PutFighters( bsim, 0 );

                size_t c = (*monsterIt)->npcList.size();
                if(c <= 0)
                {
#ifdef DEBUG_CLAN_COPY
                    *fileSt << "Battle error1." << std::endl;
#endif
                    TRACE_LOG("ClanCopy: Battle error. npcList.size() == 0.");
                    return;
                }
                bsim.setFormation(1, (*monsterIt)->formation);
                if((*monsterIt)->npcList[0].fighter->getId() <= GREAT_FIGHTER_MAX)
                    bsim.setPortrait(1, (*monsterIt)->npcList[0].fighter->getId());
                else
                    bsim.setPortrait(1, (*monsterIt)->npcList[0].fighter->favor);
                for(size_t i = 0; i < c; ++ i)
                {
                    if ((*monsterIt)->npcList[i].fighter->getCurrentHP())
                    {
                        Battle::BattleFighter * bf = bsim.newFighter(1, (*monsterIt)->npcList[i].pos, (*monsterIt)->npcList[i].fighter);
                        bf->setHP((*monsterIt)->npcList[i].fighter->getCurrentHP());
                    }
                }

                playerIt->player->addHIAttr(_spotAttrs[spotId]);
                bsim.start();
                playerIt->player->clearHIAttr();
                Stream& packet = bsim.getPacket();
                if (packet.size() <= 8)
                {
#ifdef DEBUG_CLAN_COPY
                    *fileSt << "Battle error2." << std::endl;
#endif
                    TRACE_LOG("ClanCopy: Battle error.");
                    return;
                }

                bool res = bsim.getWinner() == 1;
                if (res)
                {
                    // 玩家防守成功
                    (*monsterIt)->deadType = 1 + (flag?1:0);
#ifdef DEBUG_CLAN_COPY
                    *fileSt << "Player win." << std::endl;
                    *fileSt << "Monster deadType = " << (UInt32) (*monsterIt)->deadType << "." << std::endl;
#endif
                    _deadMonster.insert(*monsterIt);
                }
                else
                {
                    // 玩家防守失败
#ifdef DEBUG_CLAN_COPY
                    *fileSt << "Player lose." << std::endl;
#endif
                    playerIt->deadType = 1; // 标记玩家已死
                    // 加入死亡名单
                    playerIt->player->regenAll();    // 生命值回满
                    updateSpotBufferValue(spotId);

                    for(size_t i = 0; i < c; ++ i)
                    {
                        Battle::BattleObject * obj = bsim(1, (*monsterIt)->npcList[i].pos);
                        if(obj == NULL || !obj->isChar())
                            continue;
                        Battle::BattleFighter * bfgt = static_cast<Battle::BattleFighter *>(obj);
                        UInt32 nHP = bfgt->getHP();
                        (*monsterIt)->npcList[i].fighter->setCurrentHP(nHP, false);
                    }
                }
                _spotBattleInfo[spotId].push_back(
                        ClanCopyBattleInfo(playerIt->player->getId(), (*monsterIt)->npcIndex, res,
                            res?playerIt->player->allHpP():(*monsterIt)->allHpP()));
#ifdef DEBUG_CLAN_COPY
                if (!res)
                    *fileSt << "Monster remain hp = " << (UInt32) (*monsterIt)->allHpP() << "." << std::endl;
#endif
                Stream st(REP::ATTACK_NPC);
                st << static_cast<UInt8>(res) << static_cast<UInt8>(1) << static_cast<UInt32> (0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
                st.append(&packet[8], packet.size() - 8);
                st << Stream::eos;
                playerIt->player->send(st);
                bsim.applyFighterHP(0, playerIt->player, false);
                ++ count;
                break;
            }
        }
        if (playerIt == _spotPlayer[spotId].end())
        {
            // 没有玩家与怪物匹配
#ifdef DEBUG_CLAN_COPY
            *fileSt << "No player to fight to monster." << std::endl;
#endif
            break;
        }
        else
        {
            ++ monsterIt;
            ++playerIt;
        }
    }

    while (monsterIt != _spotMonster[spotId].end())
    {
        if (playerIt == _spotPlayer[spotId].end())
        {
            // 没有玩家与怪物匹配
#ifdef DEBUG_CLAN_COPY
            *fileSt << "No player to fight to monster." << std::endl;
#endif
            break;
        }
        else
        {
            ++ monsterIt;
            ++playerIt;
        }

    }

    if (flag)
    {
        if (_spotPlayer[spotId].size())
        {
            count = 0;
            while (monsterIt != _spotMonster[spotId].end() && count < COPY_EXTRA_PLAYER)
            {
                ++ monsterIt;
                ++ count;
            }
        }

        while (monsterIt != _spotMonster[spotId].end())
        {
            (*monsterIt)->nextSpotId = _spotMap[spotId].nextSpotId[0]; // FIXME
            ++ monsterIt;
        }
    }

#ifdef DEBUG_CLAN_COPY
    *fileSt << "Spot combat end." << std::endl;
#endif
}

void ClanCopy::monsterMove(UInt8 spotId)
{
    // 该据点的怪物向下一个据点移动
#ifdef DEBUG_CLAN_COPY
    *fileSt << "Monster move in spot (" << (UInt32) spotId << ")." << std::endl;
#endif
    if (spotId == Enemy_Base)
    {
        // 老巢怪物直接移动
        for (SpotMonsterList::iterator monsterIt = _spotMonster[spotId].begin(); monsterIt != _spotMonster[spotId].end(); ++ monsterIt)
        {
            UInt8 nextSpotId = (*monsterIt)->nextSpotId; // 获取目标据点Id
            (*monsterIt)->justMoved = true;
            (*monsterIt)->nextMoveTick = _tickCount + _monsterRefreshTick;
            (*monsterIt)->nextSpotId = nextSpotId;
            _spotMonster[nextSpotId].push_back(*monsterIt);
#ifdef DEBUG_CLAN_COPY
            *fileSt << "Enemy move from Enemy_Base to " << (UInt32) nextSpotId << "." << std::endl;
#endif
        }
        _spotMonster[spotId].clear(); // 怪物老巢怪物列表
    }
    else
    {
        // 存在玩家防守的据点
        SpotPlayerList::iterator playerIt = _spotPlayer[spotId].begin();
        SpotMonsterList::iterator monsterIt = _spotMonster[spotId].begin();

        // 冲阵据点只要有人就要多留住一定数量的怪
        if (_spotPlayer[spotId].size())
        {
            for (UInt8 i = 0; i < COPY_EXTRA_PLAYER; ++i)
            {
                if (monsterIt == _spotMonster[spotId].end())
                    break;
                (*monsterIt)->nextSpotId = spotId;
                ++ monsterIt;
            }
        }

        while (monsterIt != _spotMonster[spotId].end())
        {
            // 怪物与玩家一一匹配
           if (playerIt == _spotPlayer[spotId].end())
                break;
           (*monsterIt)->nextSpotId = spotId;
           ++ monsterIt;
           ++ playerIt;
#ifdef DEBUG_CLAN_COPY
            *fileSt << "Enemy battle in (" << (UInt32)spotId << ")." << std::endl;
#endif
        }

        // 怪物比玩家多的人向下一据点移动
        for (; monsterIt != _spotMonster[spotId].end(); ++ monsterIt)
        {
            (*monsterIt)->deadType = 2;
            if (spotId == Home)
            {
                // 已经在主基地，直接攻击主基地
                attackHome(*monsterIt);
                if (!_homeHp)
                {
                    // 主基地被摧毁，游戏结束
                    _status = CLAN_COPY_LOSE;
                    return;
                }
            }
            else
            {
                //  移动至下一据点
                UInt8 nextSpotId = _spotMap[spotId].nextSpotId[0]; // 获取目标据点Id FIXME:

                (*monsterIt)->nextMoveTick = _tickCount + _monsterRefreshTick;
                _spotMonster[nextSpotId].push_back(*monsterIt);
                (*monsterIt)->nextSpotId = nextSpotId;
                (*monsterIt)->deadType = 1;
                (*monsterIt)->justMoved = true;
#ifdef DEBUG_CLAN_COPY
            *fileSt << "Enemy move from " << (UInt32)spotId << " to " << (UInt32) nextSpotId << "." << std::endl;
#endif
            }
        }
        _spotMonster[spotId].remove_if(isMonsterDead());
    }
}

void ClanCopy::attackHome(ClanCopyMonster* clanCopyMonster)
{
    // 怪物攻击主基地
    _deadMonster.insert(clanCopyMonster);
    _spotBattleInfo[Home].push_back(ClanCopyBattleInfo(clanCopyMonster->npcId, clanCopyMonster->npcIndex, 0xFF, 0));
    clanCopyMonster->deadType = 0xff;
    if (_homeHp <= clanCopyMonster->npcValue)
    {
        // 主基地被摧毁
        _homeHp = 0;
#ifdef DEBUG_CLAN_COPY
        *fileSt << "monsterValue = " << clanCopyMonster->npcValue << "." << std::endl;
        *fileSt << "Home has been destroyed." << std::endl;
#endif
        return;
    }
    else
    {
        _homeHp -= clanCopyMonster->npcValue;
#ifdef DEBUG_CLAN_COPY
        *fileSt << "monsterValue = " << clanCopyMonster->npcValue << "." << std::endl;
        *fileSt << "Home hp = " << _homeHp << "." << std::endl;
#endif
    }
}

bool ClanCopy::checkWin()
{
    // 检查是否帮派副本挑战成功
    bool win = true;
    for (SpotMonster::iterator it = _spotMonster.begin(); it != _spotMonster.end(); ++ it)
    {
        if (!(it->second).empty())
        {
            win = false;
            return win;
        }
    }
    _status = CLAN_COPY_WIN;
#ifdef DEBUG_CLAN_COPY
        *fileSt << "Clan copy win in tick (" << _tickCount << ")." << std::endl;
#endif
    return win;
}

void ClanCopy::addWinReward(UInt32 awardValue)
{
    // 发送胜利奖励
    _clan->addStatueExp(awardValue);
    Table rewards = GameAction()->GetClanCopyRewards(_copyLevel);
    UInt32 types = rewards.size();
    MailPackage::MailItem *mitem = new MailPackage::MailItem [types];

    for (UInt8 i = 1; i <= types; ++i)
    {
        Table item = rewards.get<Table>(i);
        if (item.size() < 2) return;
        mitem[i - 1].id = item.get<UInt16>(1);
        mitem[i - 1].count = item.get<UInt32>(2);
    }

    Stream st (REP::CLAN_COPY);
    st << static_cast<UInt8>(0x02);
    st << static_cast<UInt8>(0x06);
    st << static_cast<UInt32>(awardValue);
    st << static_cast<UInt32>(GData::clanCopyTable[_copyLevel + 1].expOutput);
    st << Stream::eos;

    for (std::map<Player *, UInt8>::iterator playerIt = _playerIndex.begin();
            playerIt != _playerIndex.end(); ++ playerIt)
    {
        // 副本战斗玩家
        if (playerIt->first->GetVar(VAR_MAX_CLAN_COPY_LEVEL) < _copyLevel)
        {
            playerIt->first->sendMailItem(800, 801, mitem, types, true);
            playerIt->first->SetVar(VAR_MAX_CLAN_COPY_LEVEL, _copyLevel);
        }
        else
        {
            playerIt->first->send(st);
        }
    }
    for (std::vector<Player* >::iterator playerIt = _waitForWinPlayer.begin();
            playerIt != _waitForWinPlayer.end(); ++ playerIt)
    {
        // 战死后离开的玩家
        if ((*playerIt)->GetVar(VAR_MAX_CLAN_COPY_LEVEL) < _copyLevel)
        {
            (*playerIt)->sendMailItem(800, 801, mitem, types, true);
            (*playerIt)->SetVar(VAR_MAX_CLAN_COPY_LEVEL, _copyLevel);
        }
        else
        {
            (*playerIt)->send(st);
        }
    }
    notifyCopyWin(awardValue, types, mitem);
    delete[] mitem;

}


void ClanCopy::notifyAll(Stream st)
{
    // 通知该据点内所有帮派成员
    for (std::map<Player *, UInt8>::iterator playerIt = _playerIndex.begin();
            playerIt != _playerIndex.end(); ++ playerIt)
    {
        // 通知副本战斗玩家
        playerIt->first->send(st);
    }
    for (std::set<Player *>::iterator obIt = _observerPlayer.begin();
            obIt != _observerPlayer.end(); ++ obIt)
    {
        // 通知围观群众
        (*obIt)->send(st);
    }
}

void ClanCopy::notifyWaitForWin(Stream st)
{
    // 通知战死后逃跑的玩家
    for (std::vector<Player* >::iterator playerIt = _waitForWinPlayer.begin();
            playerIt != _waitForWinPlayer.end(); ++ playerIt)
    {
        // 战死后离开的玩家
        (*playerIt)->send(st);
    }
}

void ClanCopy::notifySpotPlayerInfo(Player * player /* = NULL */)
{
    // 通知玩家该据点状态
    if (_status == CLAN_COPY_PROCESS)
    {
        // 已经是战斗状态，直接发送战斗情报
        notifySpotBattleInfo(player);
        return;
    }

    Stream st(REP::CLAN_COPY);
    st << static_cast<UInt8> (CLAN_COPY_MEMBER_LIST_OP);
    st << static_cast<UInt16> (_copyLevel);
    UInt8 count = _spotMap.size();
    st << static_cast<UInt8> (count);
    for (SpotMap::iterator mapIt = _spotMap.begin(); mapIt != _spotMap.end(); ++ mapIt)
    {
        // 每个据点的信息
        UInt8 spotId = mapIt->first;
        st << static_cast<UInt8> (spotId);
        ClanCopySpot &clanCopySpot = mapIt->second;
        st << static_cast<UInt8> (clanCopySpot.maxPlayerCount);
        st << static_cast<UInt8> (clanCopySpot.spotBufferType);
        st << static_cast<UInt16> (clanCopySpot.spotBufferValue * 1000);


        st << static_cast<UInt8> (_spotPlayer[spotId].size() + _spotDeadPlayer[spotId].size());
        UInt8 i = 0;
        for (SpotPlayerList::iterator playerListIt = _spotPlayer[spotId].begin();
                playerListIt != _spotPlayer[spotId].end();++ i, ++ playerListIt)
        {
            // 据点内每个活玩家的信息
            st << static_cast<UInt8> (i + 1);
            st << static_cast<UInt64> (playerListIt->player->getId());
            st << playerListIt->player->getName();
            st << static_cast<UInt8> (playerListIt->player->GetLev());
        }
        for (SpotPlayerList::iterator playerDeadListIt =  _spotDeadPlayer[spotId].begin();
                playerDeadListIt != _spotDeadPlayer[spotId].end();++ i, ++ playerDeadListIt)
        {
            // 据点内每个死/逃跑玩家的信息
            st << static_cast<UInt8> (i + 1);
            st << static_cast<UInt64> (playerDeadListIt->player->getId());
            st << playerDeadListIt->player->getName();
            st << static_cast<UInt8> (playerDeadListIt->player->GetLev());
        }
    }
    st << Stream::eos;

    if (player)
        player->send(st);
    else
        notifyAll(st);
}

void ClanCopy::notifySpotBattleInfo(Player * player /* = NULL */)
{
    // 通知玩家每个据点的战斗状态
    if (_status == CLAN_COPY_READY)
    {
        // 已经是战斗状态，直接发送战斗情报
        notifySpotPlayerInfo(player);
        return;
    }
    Stream st(REP::CLAN_COPY);
    st << static_cast<UInt8> (CLAN_COPY_BATTLE);
    st << static_cast<UInt32> (_copyId);
    st << static_cast<UInt16> (_copyLevel);
    st << static_cast<UInt16> (_maxMonsterWave);
    st << static_cast<UInt16> (_curMonsterWave);
    st << static_cast<UInt32> (_homeMaxHp);
    st << static_cast<UInt32> (_homeHp);
    st << static_cast<UInt16> (_tickCount);
    UInt8 count = _spotMap.size();
    st << static_cast<UInt8> (count);
    for (SpotMap::iterator mapIt = _spotMap.begin(); mapIt != _spotMap.end(); ++ mapIt)
    {
        // 每个据点的信息
        UInt8 spotId = mapIt->first;
        st << static_cast<UInt8> (spotId);
        ClanCopySpot &clanCopySpot = mapIt->second;
        st << static_cast<UInt8> (clanCopySpot.maxPlayerCount);
        st << static_cast<UInt8> (clanCopySpot.spotBufferType);
        st << static_cast<UInt16> (clanCopySpot.spotBufferValue * 1000);

        UInt8 i = 0;
        st << static_cast<UInt8> (_spotPlayer[spotId].size() + _spotDeadPlayer[spotId].size());
        for (SpotPlayerList::iterator playerListIt =  _spotPlayer[spotId].begin();
                playerListIt != _spotPlayer[spotId].end();++ i, ++ playerListIt)
        {
            // 据点内每个活玩家的信息
            st << static_cast<UInt8> (i + 1);
            st << static_cast<UInt64> (playerListIt->player->getId());
            st << static_cast<UInt8> (playerListIt->deadType);
        }

        for (SpotPlayerList::iterator playerDeadListIt = _spotDeadPlayer[spotId].begin();
                playerDeadListIt != _spotDeadPlayer[spotId].end();++ i, ++ playerDeadListIt)
        {
            // 据点内每个死/逃跑玩家的信息
            st << static_cast<UInt8> (i + 1);
            st << static_cast<UInt64> (playerDeadListIt->player->getId());
            st << static_cast<UInt8> (playerDeadListIt->deadType);
        }
        st << static_cast<UInt8> (_spotMonster[spotId].size());
        for (SpotMonsterList::iterator monsterListIt = _spotMonster[spotId].begin(); monsterListIt != _spotMonster[spotId].end(); ++ monsterListIt)
        {
            st << static_cast<UInt32> ((*monsterListIt)->npcIndex);
            st << static_cast<UInt32> ((*monsterListIt)->npcId);
            st << static_cast<UInt8> ((*monsterListIt)->monsterType);
            st << static_cast<UInt16> ((*monsterListIt)->npcValue);
            st << static_cast<UInt8> ((*monsterListIt)->nextSpotId);
            st << static_cast<UInt8> ((*monsterListIt)->deadType);
        }
        st << static_cast<UInt8> (_spotBattleInfo[spotId].size());
        for (BattleInfo::iterator battleInfoIt = _spotBattleInfo[spotId].begin(); battleInfoIt != _spotBattleInfo[spotId].end(); ++ battleInfoIt)
        {
            st << static_cast<UInt64> (battleInfoIt->playerId);
            st << static_cast<UInt32> (battleInfoIt->monsterIndex);
            st << static_cast<UInt8>  (battleInfoIt->battleResult);
            st << static_cast<UInt8>  (battleInfoIt->remainHpP);
        }
    }
    st << Stream::eos;

    if (player)
        player->send(st);
    else
        notifyAll(st);
}

void ClanCopy::notifyCopyLose()
{
    // 通知玩家副本失败

    _launchPlayer->clanCopyUdpLog(1134, static_cast<UInt8>(_copyLevel));
    Stream st (REP::CLAN_COPY);
    st << static_cast<UInt8>(0x02);
    st << static_cast<UInt8>(0x04);
    st << Stream::eos;

    notifyAll(st);
    notifyWaitForWin(st);
}

void ClanCopy::notifyCopyWin(UInt32 awardValue, UInt8 itemTypes, MailPackage::MailItem *mitem)
{
    // 通知玩家副本成功

    _launchPlayer->clanCopyUdpLog(1133, static_cast<UInt8>(_copyLevel));
    Stream st (REP::CLAN_COPY);
    st << static_cast<UInt8>(0x02);
    st << static_cast<UInt8>(0x03);
    st << static_cast<UInt32>(awardValue);
    st << static_cast<UInt32>(GData::clanCopyTable[_copyLevel + 1].expOutput);
    st << static_cast<UInt8>(itemTypes);
    for (UInt8 i = 0; i < itemTypes; ++i)
    {
        st << static_cast<UInt32> (mitem[i].id);
        st << static_cast<UInt16> (mitem[i].count);
    }
    st << Stream::eos;

    notifyAll(st);
    notifyWaitForWin(st);
}

void ClanCopy::notifyLauncherEscape()
{
    // 通知玩家发起者逃跑，副本作废
    class CopyOverVisitor : public Visitor<ClanMember>
    {
        public:
            CopyOverVisitor()
            {
            }

            bool operator() (ClanMember * member)
            {
                Stream st (REP::CLAN_COPY);
                st << static_cast<UInt8>(0x02);
                st << static_cast<UInt8>(0x02);
                st << Stream::eos;
                member->player->send(st);
                return true;
            }

    };

    CopyOverVisitor visitor;
    _clan->VisitMembers(visitor);
    _clan->broadcastCopyInfo();
}

void ClanCopy::notifyCopyCreate()
{
    // 通知所有玩家成员帮派副本建立
    class SendDataVisitor : public Visitor<ClanMember>
    {
        public:
            SendDataVisitor()
            {}

            bool operator() (ClanMember* member)
            {
                if (!member->player->isOnline()) return true;

                return true;
            }
    };
    SendDataVisitor vistor;
    _clan->VisitMembers(vistor);
    _clan->broadcastCopyInfo();
}

void ClanCopy::forceEnd(UInt8 type)
{
    // 强制结束帮派副本
    switch (type)
    {
        case FORCE_END_BY_RESET:
            {
                // 弹窗通知玩家到达副本重置时间，副本作废
                class CopyOverVisitor : public Visitor<ClanMember>
                {
                    public:
                        CopyOverVisitor()
                        {
                        }

                        bool operator() (ClanMember * member)
                        {
                            Stream st (REP::CLAN_COPY);
                            st << static_cast<UInt8>(0x02);
                            st << static_cast<UInt8>(0x05);
                            st << Stream::eos;
                            member->player->send(st);
                            return true;
                        }

                };

                CopyOverVisitor visitor;
                _clan->VisitMembers(visitor);
                _clan->broadcastCopyInfo();
            }
            break;
        case FORCE_END_BY_ERROR:
            // 半夜重置时仍然有副本残留，异常状态
            break;
        case FORCE_END_BY_GM:
            // FIXME: GM强制要求结束副本（暂时没有这个需求，随便写写的）
            break;
        default:
            break;
    }
    _status = CLAN_COPY_OVER;
}

/////////////////////////////////////////////////////////////
// ClanCopyMgr

ClanCopyMgr::ClanCopyMgr()
{
    UInt32 now = TimeUtil::Now();
    UInt32 sweek = TimeUtil::SharpWeek(1, now);
    _interval = 5;
    if ((sweek - now) <= 1800 || (now + 3600 * 24 * 7 - sweek) <= 1800)
    {
        _reset = true;
    }
    else
    {
        _reset = false;
    }
}

ClanCopyMgr::~ClanCopyMgr()
{

}

void ClanCopyMgr::setInterval(UInt32 time)
{
    _interval = time;
}

void ClanCopyMgr::ResetBegin()
{
    // 开始重置副本，不能开始创建新帮派副本
    for (ClanCopyMap::iterator it = _clanCopyMap.begin(); it != _clanCopyMap.end();)
    {
        if (it->second->getStatus() == CLAN_COPY_READY)
        {
            // 强制结束还在准备状态，没有开始的帮派副本
            forceEndClanCopy(it->second);
            delete (it->second);
            _clanCopyMap.erase(it ++);
        }
        else
            ++ it;
    }
    _reset = true;
    INFO_LOG("Start reset clanCopy.");
}

void ClanCopyMgr::Reset()
{
    // 每个帮派的副本等级下降五级
    class ResetClanCopyVisitor : public Visitor<Clan>
    {
        public:
            bool operator() (Clan* clan)
            {
                clan->resetCopyLevel();
                return true;
            }
    };
    ResetClanCopyVisitor visitor;
    globalClans.enumerate(visitor);
    INFO_LOG("Reset clanCopy.");
}

void ClanCopyMgr::ResetEnd()
{
    _reset = false;
    INFO_LOG("End reset clanCopy.");
}

void ClanCopyMgr::process(UInt32 now)
{
    // 定时器处理所有帮派副本
    for (ClanCopyMap::iterator it = _clanCopyMap.begin(); it != _clanCopyMap.end();)
    {
        if (it->second->getStatus() == CLAN_COPY_OVER)
        {
            // 该帮派副本数据已经无效，清除有关数据
            deleteClanCopy(it->second);
            delete (it->second);
            _clanCopyMap.erase(it ++);
        }
        else
        {
            // 处理帮派副本游戏进程
            it->second->process(now);
            ++ it;
        }
    }
}

void ClanCopyMgr::forceEndAllClanCopy()
{
    for (ClanCopyMap::iterator it = _clanCopyMap.begin(); it != _clanCopyMap.end();)
    {
        forceEndClanCopy(it->second);
        delete (it->second);
        _clanCopyMap.erase(it ++);
    }
    _reset = true;

}

UInt8 ClanCopyMgr::getCopyStatus(UInt32 clanId)
{
    // 查询某一帮派的副本状态
    ClanCopyMap::iterator it = _clanCopyMap.find(clanId);
    if (it == _clanCopyMap.end())
    {
        return CLAN_COPY_NONE;
    }
    else
    {
        return it->second->getStatus();
    }
}

ClanCopy * ClanCopyMgr::getClanCopyByClan(Clan *c)
{
    ClanCopyMap::iterator it = _clanCopyMap.find(c->getId());

    if (it != _clanCopyMap.end())
    {
        return it->second;
    }
    else
        return NULL;
}

bool ClanCopyMgr::createClanCopy(Player* player, Clan *c)
{
    // 创建一个新帮派副本
    static UInt32 clanId = 1;
    if (c == NULL)
        return false;
    if (_clanCopyMap.find(c->getId()) != _clanCopyMap.end())
    {
        if ((_clanCopyMap.find(c->getId()))->second->getStatus() == CLAN_COPY_OVER)
            player->sendMsgCode(0, 1350);
        return false;
    }

    if (!c->copyLevelAvailable())
    {
        player->sendMsgCode(0, 1355);
        return false;
    }

    if (_reset)
    {
        player->sendMsgCode(0, 1354);
        return false;
    }

    ClanCopy  * copy = new ClanCopy(c, clanId ++, player);
    copy->setInterval(_interval);
    if (!copy)
    {
        // new都失败，那也差不多了
        return false;
    }
    _clanCopyMap.insert(std::make_pair(c->getId(), copy));
    copy->addPlayerFromSpot(c);
    c->broadcastCopyInfo();
    c->notifyCopyCreated(player);

    Stream st;
    SYSMSGVP(st, 806, player->getName().c_str());
    c->broadcast(st);

    return true;
}

void ClanCopyMgr::forceEndClanCopy(ClanCopy* clanCopy, UInt8 type /* = FORCE_END_BY_RESET */)
{
    // 强制结束副本
    clanCopy->forceEnd(type);
}

void ClanCopyMgr::deleteClanCopy(ClanCopy *clanCopy)
{
    // 清除已经结束的帮派副本(Nothing done)
}

void ClanCopyMgr::playerEnter(Player * player, bool needNotify /* = true */)
{
    // 有玩家进入帮派副本据点
    if(player->getLocation() != CLAN_COPY_LOCATION) return;

    Clan* clan = player->getClan();
    if(clan == NULL) return;  // 没有帮派

    ClanCopyMap::iterator it = _clanCopyMap.find(clan->getId());
    if (it == _clanCopyMap.end())
        return;  // 没有开启副本

    if (player->GetVar(VAR_CLAN_LEAVE_TIME) + 24 * 60 * 60 > TimeUtil::Now())
    {
        player->sendMsgCode(1, 1343);
        it->second->addObserver(player);
    }
    else
    {
        if (it->second->getStatus() == CLAN_COPY_READY)
        {
            // 副本准备阶段，加入玩家列表
            it->second->addPlayer(player, needNotify);
        }
        else
        {
            // 副本已经开始，加入围观群众
            it->second->addObserver(player);
        }
    }
}

void ClanCopyMgr::playerLeave(Player * player)
{
    // 有玩家离开帮派副本据点
    if(player->getLocation() != CLAN_COPY_LOCATION) return;

    Clan* clan = player->getClan();
    if(clan == NULL) return;  // 没有帮派

    ClanCopyMap::iterator it = _clanCopyMap.find(clan->getId());
    if (it == _clanCopyMap.end())
        return;  // 没有开启副本

    it->second->playerEscape(player); //副本已经开始，玩家算逃跑
}

void ClanCopyMgr::playerRequestStart(Player * player)
{
    // 玩家请求开始副本
    if(player->getLocation() != CLAN_COPY_LOCATION) return;

    Clan* clan = player->getClan();
    if(clan == NULL) return;  // 没有帮派

    ClanCopyMap::iterator it = _clanCopyMap.find(clan->getId());
    if (it == _clanCopyMap.end())
        return;  // 没有开启副本

    it->second->requestStart(player);
}

UInt16 ClanCopyMgr::getTotalPlayer(Clan* c)
{
    // 查询某一帮派副本的参与人数
    ClanCopyMap::iterator it = _clanCopyMap.find(c->getId());
    if (it == _clanCopyMap.end())
    {
        return 0;
    }
    else
    {
        return it->second->getTotalPlayer();
    }
}

// ClanCopyMgr
/////////////////////////////////////////////////////////////

}

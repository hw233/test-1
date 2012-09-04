#include "ClanCopy.h"
#include "Clan.h"
#include "MsgID.h"
#include "GData/ClanCopyTable.h"
#include "Player.h"
#include "Common/Stream.h"


namespace GObject
{




const static UInt32 COPY_ROUTE_COUNT = 3;  // 帮派副本的路径数目

#ifdef DEBUG_CLAN_COPY
UInt32 clanCopyFileIndex = 1;
#endif

ClanCopy::ClanCopy(Clan *c, Player * player) : _clan(c), _launchPlayer(player)
{
#ifdef DEBUG_CLAN_COPY
    char fileBuf[32];
    snprintf (fileBuf, 32, "clan_copy_%d.txt", clanCopyFileIndex++);
    fileSt.open(fileBuf, std::fstream::in);
#endif
    _copyLevel = c->getCopyLevel();
    _spotMap.clear();
    _spotMonster.clear();
    _spotPlayer.clear();
    _spotDeadPlayer.clear();
    _spotDeadPlayer.clear();
    _status = CLAN_COPY_READY;

    _readyTime = 0;
    _startTime = 0;
    _endTime = 0;
    _tickTime = 0;

    _tickTimeInterval = 5;
    _startTick = 1;
    _monsterRefreshTick = 9;
    _tickCount = 0;

    _maxMonsterWave = 20;
    _curMonsterWave = 0;

    _totalPlayer = 0;  
    _deadPlayer = 0;   
    _escapePlayer = 0; 
    _observerPlayer.clear();
#ifdef DEBUG_CLAN_COPY
    fileSt << "clan_copy_log:" << std::endl;
#endif
    
    _launchPlayer = NULL;
}


ClanCopy::~ClanCopy()
{
#ifdef DEBUG_CLAN_COPY
    fileSt.close();
#endif
    _spotMap.clear();
    _spotMonster.clear();
    _spotPlayer.clear();
    _spotDeadPlayer.clear();
    _observerPlayer.clear();
}

UInt8 ClanCopy::getStatus()
{
    // 返回副本状态
    return _status;
}

UInt16 ClanCopy::getTotalPlayer()
{
    return _totalPlayer;
}

void ClanCopy::addPlayer(Player * player)
{
    // 增加帮派副本玩家人数 （仅在准备状态有效）
    if (_status != CLAN_COPY_READY)
        return;
    std::set<Player*>::iterator obIt = _observerPlayer.find(player);
    if (obIt!= _observerPlayer.end())
        _observerPlayer.erase(obIt);
    for (SpotMap::iterator mapIt = _spotMap.begin(); mapIt != _spotMap.end(); ++mapIt)
    {
        if (mapIt->first == Home)
            continue;
        SpotPlayer::iterator playerIt = _spotPlayer.find(mapIt->first);
        if (playerIt == _spotPlayer.end())
            return;
        if ((playerIt->second).size() >= (mapIt->second).maxPlayerCount)
            continue;
        else
        {
            (playerIt->second).push_back(ClanCopyPlayer(player));
            notifySpotPlayerInfo(mapIt->first);
            return;
        }
    }
    player->sendMsgCode(0, 1351);
}

void ClanCopy::addObserver(Player * player)
{
    // 增加帮派副本围观人员（不明真相的围观群众）
    if (_observerPlayer.find(player) != _observerPlayer.end())
        return;    // 已经是围观人员，不需要再次加入（这里也是异常流程）
    if (_playerIndex.find(player) != _playerIndex.end())
        return;    // 已经是副本战斗人员（这里也是异常流程）
    _observerPlayer.insert(player);
}

void ClanCopy::playerEscape(Player *player)
{
    // TODO: 玩家逃跑
    std::set<Player *>::iterator obIt = _observerPlayer.find(player);
    if (obIt != _observerPlayer.end())
    {
        // 围观群众散去
        _observerPlayer.erase(obIt);
        return;
    }

    if (player == _launchPlayer)
    {
        // 发起者逃跑，副本作废
        _status = CLAN_COPY_OVER;
        notifyLauncherEscape();
    }

    std::map<Player *, UInt8>::iterator playerIndexIt = _playerIndex.find(player);
    if (playerIndexIt != _playerIndex.end())
    {
        // TODO: 游戏中玩家逃跑
    }
}

void ClanCopy::initCopyData()
{
    // TODO: 读取副本配置数据，初始化副本状态
}


void ClanCopy::process(UInt32 now)
{
    // 游戏状态进行
    if (_status == CLAN_COPY_READY)
        return;  // 还在准备状态，游戏未开始

    if (_status == CLAN_COPY_WIN)
        return;
    if (_status == CLAN_COPY_LOSE)
        return;

    if ((now - _tickTime) < _tickTimeInterval)
        return;    // 未到达触发下一状态的时间
    _tickTime = now;

    for (SpotMap::iterator it = _spotMap.begin(); it != _spotMap.end(); ++it)
    {
        if (it->first == Enemy_Base)
            enemyBaseAct();
        else
            monsterAssault(it->first);
    }

    if (_tickCount >= _maxMonsterWave * _monsterRefreshTick + _startTick)
    {
        checkWin();
    }
    ++_tickCount;
}

void ClanCopy::monsterAssault(const UInt8& spotId)
{
    // 据点战斗或者怪物移动
    SpotMap::iterator spotIt = _spotMap.find(spotId);
    if (spotIt == _spotMap.end())
        return; // 正常情况下不会进入这里
    ClanCopySpot &spot = spotIt->second;

    SpotPlayer::iterator playerIt = _spotPlayer.find(spotId);
    if (playerIt == _spotPlayer.end())
        return; // 正常情况下不会进入这里
    SpotPlayerList& playerList = playerIt->second;

    SpotMonster::iterator monsterIt = _spotMonster.find(spotId);
    if (monsterIt == _spotMonster.end())
    {
        return; // 正常情况下不会进入这里
    }
    SpotMonsterList& monsterList = monsterIt->second;

    if (spot.lastBattleTick + _monsterRefreshTick > _tickCount )
    {
        // 仍然在战斗阶段
        spotCombat(monsterList, playerList, spotId);
    }
    else
    {
        // 怪物冲锋阶段
        monsterMove(spotId);
        spot.lastBattleTick = _tickCount;
    }
}

void ClanCopy::spotCombat(SpotMonsterList& monsterList, SpotPlayerList& playerList, UInt8 spotId)
{
    // 开始一轮战斗
    SpotPlayerList::iterator playerIt = playerList.begin();
    SpotMonsterList::iterator monsterIt = monsterList.begin(); 
    while (monsterIt != monsterList.end())
    {
        // 怪物一一与玩家匹配直到没有怪物
        if (playerIt == playerList.end())
        {
            // 没有玩家与怪物匹配
            break;
        }
        else
        {
            // 找到可以匹配的玩家与怪物战斗
            GData::NpcGroups::iterator npcGroupsIt = GData::npcGroups.find(monsterIt->npcId);
            if(npcGroupsIt == GData::npcGroups.end())
                return;

            GData::NpcGroup * ng = npcGroupsIt->second;
            Battle::BattleSimulator bsim(Battle::BS_COPY5, playerIt->player, ng->getName(), ng->getLevel(), false);
            playerIt->player->PutFighters( bsim, 0 );
            ng->putFighters( bsim );
            bsim.start();
            Stream& packet = bsim.getPacket();
            if (packet.size() <= 8)
                return;

            bool res = bsim.getWinner() == 1;
            if (res)
            {
                // 玩家防守成功
                monsterIt->isDead = true;
            }
            else
            {
                // 玩家防守失败
                playerIt->isDead = true;
                _spotDeadPlayer.insert (std::make_pair(spotId, *playerIt)); // 加入死亡名单
                playerIt->player->regenAll();    // 生命值回满
            }
            Stream st(REP::ATTACK_NPC);
            st << static_cast<UInt8>(res) << static_cast<UInt8>(1) << static_cast<UInt8> (32) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
            st.append(&packet[8], packet.size() - 8);
            st << Stream::eos;
            playerIt->player->send(st);
            bsim.applyFighterHP(0, playerIt->player, false);
        }
        ++ playerIt;
        ++ monsterIt;
    }

    // 移除已经死亡的怪物或者玩家
    playerList.remove_if(isPlayerDead());
    monsterList.remove_if(isMonsterDead());
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

void ClanCopy::enemyBaseAct()
{
    // 敌人老巢的行动
    monsterMove(Enemy_Base); // 上一轮产生的怪物先行移动
    if (_curMonsterWave * _monsterRefreshTick + _startTick < _tickCount)
        return;   // 未到达刷怪时间
    if (_curMonsterWave < _maxMonsterWave)
    {
        // 刷新一轮怪
        ++ _curMonsterWave;
        createEnemy();
    }
}

void ClanCopy::createEnemy()
{
    // 敌人老巢产生敌人

    UInt32 key = ((static_cast<UInt32>(_copyLevel)) << 16) | _curMonsterWave;
    GData::ClanCopyMonsterMap::iterator it = GData::clanCopyMonsterMap.find(key);
    if (it == GData::clanCopyMonsterMap.end())
    {
        // 该轮没有怪物（暂时不会配置出该情况）
        return;
    }
    const GData::ClanCopyMonsterData &clanCopyMonsterData = it->second;

    {
        // 没有老巢据点或者老巢据点道路数小于怪物配置数目
        SpotMap::iterator it = _spotMap.find(Enemy_Base);
        if (it == _spotMap.end() || (it->second).nextSpotId.size() < clanCopyMonsterData.npcRouteCount)
            return;  
    }

    // 设置该轮怪物参数
    ClanCopyMonster clanCopyMonster (clanCopyMonsterData.npcId, _curMonsterWave, clanCopyMonsterData.npcValue, 0, _tickCount + 1);
    SpotMonster::iterator spotMonsterIt = _spotMonster.find(Enemy_Base);
    if (spotMonsterIt == _spotMonster.end())
        return; // 正常情况不会进入这里

    // 老巢中放入怪物
    SpotMonsterList &spotMonsterList = spotMonsterIt->second;
    for (UInt8 i = 0; i < clanCopyMonsterData.npcRouteCount; ++ i)
    {
        // 设置一路的怪
        clanCopyMonster.nextSpotId = _spotMap[Enemy_Base].nextSpotId[i];
        for (UInt16 count = 0; count < clanCopyMonsterData.npcCount; ++ count)
        {
            spotMonsterList.push_back(clanCopyMonster);
        }
    }
}

void ClanCopy::monsterMove(UInt8 spotId)
{
    // 该据点的怪物向下一个据点移动
    SpotMonster::iterator it = _spotMonster.find(spotId);
    if (it == _spotMonster.end())
        return;
    SpotMonsterList &monsterList = it->second;
    if (spotId == Enemy_Base)
    {
        // 老巢怪物直接移动
        for (SpotMonsterList::iterator monsterIt = monsterList.begin(); monsterIt != monsterList.end(); ++ monsterIt)
        {
            UInt8 nextSpotId = monsterIt->nextSpotId; // 获取目标据点Id
            SpotMonster::iterator spotMonsterIt = _spotMonster.find(nextSpotId); // 目标据点的怪物列表
            if (spotMonsterIt == _spotMonster.end())
                return;
            monsterIt->justMoved = true;
            monsterIt->nextMoveTick = _tickCount + _monsterRefreshTick;
            (spotMonsterIt->second).push_back(*monsterIt);
        }
        monsterList.clear(); // 怪物老巢怪物列表
    }
    else
    {
        // 存在玩家防守的据点
        SpotPlayer::iterator spotPlayerIt = _spotPlayer.find(spotId);
        if (spotPlayerIt == _spotPlayer.end())
            return;
        SpotPlayerList &playerList = spotPlayerIt->second; // 获取该据点的玩家列表

        SpotPlayerList::iterator playerIt = playerList.begin();
        SpotMonsterList::iterator monsterIt = monsterList.begin();

        while (monsterIt != monsterList.end())
        {
            // 怪物与玩家一一匹配
            if (playerIt != playerList.end())
                continue;
            ++ monsterIt;
            ++ playerIt;
        }

        // 怪物比玩家多的人向下一据点移动
        for (; monsterIt != monsterList.end(); ++ monsterIt)
        {
            if (spotId == Home)
            {
                // 已经在主基地，直接攻击主基地
                attackHome(*monsterIt);
                if (!_homeHp)
                {
                    // 主基地被摧毁，游戏结束
                    _status = CLAN_COPY_LOSE;
                    notifyCopyLose();
                    return;
                }
            }
            else
            {
                //  移动至下一据点
                UInt8 nextSpotId = monsterIt->nextSpotId; // 获取目标据点Id
                SpotMonster::iterator spotMonsterIt = _spotMonster.find(nextSpotId); // 目标据点的怪物列表
                if (spotMonsterIt == _spotMonster.end())
                    return;

                monsterIt->justMoved = true;
                monsterIt->nextMoveTick = _tickCount + _monsterRefreshTick;
                (spotMonsterIt->second).push_back(*monsterIt);
            }
            monsterIt->isDead = true;
        }
        monsterList.remove_if(isMonsterDead());
    }
}

void ClanCopy::attackHome(ClanCopyMonster& clanCopyMonster)
{
    // 怪物攻击主基地
    clanCopyMonster.isDead = true;
    if (_homeHp <= clanCopyMonster.npcValue)
    {
        // 主基地被摧毁
        _homeHp = 0;
        return;
    }
    else
    {
        _homeHp -= clanCopyMonster.npcValue;
    }
}

void ClanCopy::lose()
{
    // TODO: 帮派副本挑战失败

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
    _clan->addCopyLevel();
    _clan->addCopyWinLog(_launchPlayer);
    notifyCopyWin();
    return win;
}

void ClanCopy::notifySpotPlayerInfo(UInt8 spotId)
{
    // 通知玩家该据点状态

    Stream st(REP::CLAN_COPY);
    st << static_cast<UInt8> (CLAN_COPY_MEMBER_LIST_OP);
    st << static_cast<UInt16> (_copyLevel);
    UInt8 count = _spotMap.size();
    st << static_cast<UInt8> (count);
    for (SpotMap::iterator mapIt = _spotMap.begin(); mapIt != _spotMap.end(); ++ mapIt)
    {
        // 每个据点的信息
        st << static_cast<UInt8> (mapIt->first);
        ClanCopySpot &clanCopySpot = mapIt->second;
        st << static_cast<UInt8> (clanCopySpot.maxPlayerCount);
        st << static_cast<UInt8> (clanCopySpot.spotBufferType);
        st << static_cast<UInt16> (clanCopySpot.spotBufferValue);

        SpotPlayer::iterator spotPlayerIt = _spotPlayer.find(mapIt->first);
        if (spotPlayerIt == _spotPlayer.end())
            return;

        st << static_cast<UInt8> ((spotPlayerIt->second).size());
        UInt8 i = 0;
        for (SpotPlayerList::iterator playerListIt =  (spotPlayerIt->second).begin(); 
                playerListIt != spotPlayerIt->second.end();++ i, ++ playerListIt)
        {
            // 据点内每个玩家的信息
            st << static_cast<UInt8> (i + 1);
            st << static_cast<UInt64> (playerListIt->player->getId());
            st << playerListIt->player->getName();
            st << static_cast<UInt8> (playerListIt->player->GetLev());
        }
    }
    st << Stream::eos;
 
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

void ClanCopy::notifyCopyLose()
{
    // TODO: 通知玩家副本失败
}

void ClanCopy::notifyCopyWin()
{
    // TODO: 通知玩家副本成功
}

void ClanCopy::notifyLauncherEscape()
{
    // TODO: 通知玩家发起者逃跑，副本作废
}

/////////////////////////////////////////////////////////////
// ClanCopyMgr

ClanCopyMgr::ClanCopyMgr()
{
}

ClanCopyMgr::~ClanCopyMgr()
{
}

void ClanCopyMgr::process(UInt32 now)
{
    // 定时器处理所有帮派副本
    for (ClanCopyMap::iterator it = _clanCopyMap.begin(); it != _clanCopyMap.end();)
    {
        if ((it->second).getStatus() == CLAN_COPY_OVER)
        {
            // 该帮派副本数据已经无效，清除有关数据
            deleteClanCopy(&(it->second));
            _clanCopyMap.erase(it ++);
        }
        else
        {
            // 处理帮派副本游戏进程
            (it->second).process(now);
            ++ it;
        }
    }
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
        return (it->second).getStatus();
    }
}

bool ClanCopyMgr::createClanCopy(Player* player, Clan *c)
{
    // 创建一个新帮派副本
    if (c == NULL)
        return false;
    if (_clanCopyMap.find(c->getId()) != _clanCopyMap.end())
        return false;
    _clanCopyMap.insert(std::make_pair(c->getId(), ClanCopy(c, player)));
    return true;
}

void ClanCopyMgr::deleteClanCopy(ClanCopy *clanCopy)
{
    // 清除已经结束的帮派副本(Nothing done)
}

void ClanCopyMgr::clanLose(ClanCopy* clanCopy)
{
    // TODO: 该副本已经失败
}

void ClanCopyMgr::playerEnter(Player * player)
{
    // TODO: 有玩家进入帮派副本据点
    if(player->getLocation() != CLAN_COPY_LOCATION) return;

    Clan* clan = player->getClan();
    if(clan == NULL) return;  // 没有帮派

    ClanCopyMap::iterator it = _clanCopyMap.find(clan->getId());
    if (it == _clanCopyMap.end())
        return;  // 没有开启副本

    if (player->GetVar(VAR_CLAN_LEAVE_TIME) + 24 * 60 * 60 > TimeUtil::Now())
    {
        player->sendMsgCode(1, 1343);
        (it->second).addObserver(player);
    }
    else
    {

        if ((it->second).getStatus() == CLAN_COPY_READY)
        {
            (it->second).addPlayer(player);
        }
        else
        {
            (it->second).addObserver(player);
        }
    }

    /*
    //通知状态变化
    Stream st(REP::CLAN_COPY);
    st << static_cast<UInt8> (CLAN_COPY_MEMBER_LIST_OP);
    st << static_cast<UInt16> (_copyLevel);
    UInt8 count = _spotMap.size();
    st << static_cast<UInt8> (count);
    for (SpotMap::iterator mapIt = _spotMap.begin(); mapIt != _spotMap.end(); ++ mapIt)
    {
        st << static_cast<UInt8> (mapIt->first);
        ClanCopySpot &clanCopySpot = mapIt->second;
        st << static_cast<UInt8> (clanCopySpot.maxPlayerCount);
        st << static_cast<UInt8> (clanCopySpot.spotBufferType);
        st << static_cast<UInt16> (clanCopySpot.spotBufferValue);

        SpotPlayer::iterator spotPlayerIt = _spotPlayer.find(mapIt->first);
        if (spotPlayerIt == _spotPlayer.end())
            return;

        st << static_cast<UInt8> ((spotPlayerIt->second).size());
        for (SpotPlayerList::iterator playerListIt =  (spotPlayerIt->second).begin(); 
                playerListIt != spotPlayerIt->second.end(); ++ playerListIt)
        {
            st << static_cast<UInt8> (playerListIt - (spotPlayerIt->second).begin() + 1);
            st << static_cast<UInt64> playerListIt->player->getId();
            st << playerListIt->player->getName();
            st << static_cast<UInt8> playerListIt->player->getLevel();
        }
    }
    st << Stream::eos;
    */
}

void ClanCopyMgr::playerLeave(Player * player)
{
    // TODO: 有玩家离开帮派副本据点
    if(player->getLocation() != CLAN_COPY_LOCATION) return;

    Clan* clan = player->getClan();
    if(clan == NULL) return;  // 没有帮派

    ClanCopyMap::iterator it = _clanCopyMap.find(clan->getId());
    if (it == _clanCopyMap.end())
        return;  // 没有开启副本

    (it->second).playerEscape(player);

    //通知状态变化
    /*
       Stream stream(REP::CLAN_RANKBATTLE_REP);
       stream << UInt8(9);
       stream << player->getId();
       stream << UInt8(1);
       stream << Stream::eos;
       info->Broadcast(stream);
       */

    // 如果是围观群众，先清除围观群众
    /*
    std::set<Player*>::iterator it = _observerPlayer.find(player);
    if (it != _observerPlayer.end())
        _observerPlayer.erase(it);

    // 如果是正在进行的玩家
    std::map<Player *, UInt8>::iterator indexIt = _playerIndex.find(player);
    if (indexIt != _playerIndex.end())
    {
        // TODO: 战斗玩家离开副本
    }
    */
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
        return (it->second).getTotalPlayer();
    }
}

// ClanCopyMgr
/////////////////////////////////////////////////////////////

}

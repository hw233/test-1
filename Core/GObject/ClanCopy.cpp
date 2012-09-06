#include "ClanCopy.h"
#include "Clan.h"
#include "MsgID.h"
#include "GData/ClanCopyTable.h"
#include "Player.h"
#include "Common/Stream.h"


namespace GObject
{




const static UInt32 COPY_ROUTE_COUNT = 3;  // 帮派副本的路径数目
const static UInt32 COPY_MIN_PLAYER = 1;

#ifdef DEBUG_CLAN_COPY
UInt32 clanCopyFileIndex = 1;
#endif

ClanCopy::ClanCopy(Clan *c, Player * player) : _clan(c), _launchPlayer(player)
{
#ifdef DEBUG_CLAN_COPY
    char fileBuf[32];
    snprintf (fileBuf, 32, "clan_copy_%d.txt", clanCopyFileIndex++);
    fileSt = new std::fstream;
    fileSt->open(fileBuf, std::fstream::out);
#endif
    _copyLevel = c->getCopyLevel();
    _spotMap.clear();
    _spotMonster.clear();
    _spotPlayer.clear();
    _spotDeadPlayer.clear();
    _status = CLAN_COPY_READY;

    _readyTime = TimeUtil::Now();
    _startTime = 0;
    _endTime = 0;
    _tickTime = 0;

    _tickTimeInterval = 5;
    _startTick = 1;
    _monsterRefreshTick = 9;
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
#ifdef DEBUG_CLAN_COPY
    * fileSt << "clan copy destoryed." << std::endl;
    fileSt->close();
    delete fileSt;
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
    return _playerIndex.size();
}

void ClanCopy::addPlayer(Player * player)
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
#ifdef DEBUG_CLAN_COPY
            *fileSt << "\"" << player->getName() << "\" change to spot(" << (UInt32) mapIt->first << ")." << std::endl;
#endif
            notifySpotPlayerInfo();
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
                ClanCopyMgr::Instance().playerEnter(member->player);
                return true;
            }
    };
    autoJoinVisitor visitor;
    _clan->VisitMembers(visitor);
}

void ClanCopy::addObserver(Player * player)
{
    // 增加帮派副本围观人员（不明真相的围观群众）
    if (_observerPlayer.find(player) != _observerPlayer.end())
        return;    // 已经是围观人员，不需要再次加入（这里也是异常流程）
    if (_playerIndex.find(player) != _playerIndex.end())
        return;    // 已经是副本战斗人员（这里也是异常流程）
    _observerPlayer.insert(player);
#ifdef DEBUG_CLAN_COPY
    *fileSt << "observer \"" << player->getName() << "\" add." << std::endl;
#endif
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

    if (player == _launchPlayer)
    {
        if (_status == CLAN_COPY_READY)
        {
            // 发起者逃跑，副本作废
            _status = CLAN_COPY_OVER;
#ifdef DEBUG_CLAN_COPY
            *fileSt << "Launcher \"" << player->getName() << "\" leave." << std::endl;
#endif
            notifyLauncherEscape();
            return;
        }
    }

    std::map<Player *, UInt8>::iterator playerIndexIt = _playerIndex.find(player);
    if (playerIndexIt != _playerIndex.end())
    {
        UInt8 spotId = playerIndexIt->second;
        // 游戏中玩家逃跑
        SpotPlayer::iterator spotPlayerIt = _spotPlayer.find(spotId);
        if (spotPlayerIt == _spotPlayer.end())
            return;
        SpotPlayerList& spotPlayerList = spotPlayerIt->second;
        for (SpotPlayerList::iterator spotPlayerListIt = spotPlayerList.begin(); spotPlayerListIt != spotPlayerList.end(); ++ spotPlayerListIt)
        {
            if (spotPlayerListIt->player == player)
            {
                spotPlayerListIt->deadType = 2;
                SpotDeadPlayer::iterator spotDeadPlayerIt = _spotDeadPlayer.find(spotId);
                if (spotDeadPlayerIt != _spotDeadPlayer.end())
                    return;
                (spotDeadPlayerIt->second).push_back (*spotPlayerListIt); // 加入死亡名单
                spotPlayerListIt->player->regenAll();    // 生命值回满
                spotPlayerList.remove_if(isPlayerDead());
#ifdef DEBUG_CLAN_COPY
                *fileSt << "\"" << player->getName() << "\" escape from spot " << (UInt32)spotId << "." << std::endl;
#endif
                _playerIndex.erase(playerIndexIt);
                return;
            }
        }

    }
}

void ClanCopy::initCopyData()
{
    // 读取副本配置数据，初始化副本状态
#ifdef DEBUG_CLAN_COPY
    *fileSt << "initCopyData..." << std::endl;
#endif
    _homeHp = GData::clanCopyTable[_copyLevel].homeHp;

    ClanCopySpot clanCopySpot;
    SpotPlayerList spotPlayerList;
    SpotMonsterList spotMonsterList;
    for (UInt8 i = 0; i < SPOT_COUNT; ++ i)
    {
#ifdef DEBUG_CLAN_COPY
    *fileSt << "init spot id(" << (UInt32) SpotId[i] << ")." << std::endl;
#endif
        clanCopySpot.spotId = SpotId[i];
        clanCopySpot.spotBufferType = SpotId[i];
        clanCopySpot.nextSpotId.clear();
        for (UInt8 j = 0; j < NextSpotNum[i]; ++ j)
        {
            clanCopySpot.nextSpotId.push_back(NextSpotId[i][j]);
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


void ClanCopy::process(UInt32 now)
{
    // 游戏状态进行
#ifdef DEBUG_CLAN_COPY
    static UInt32 count = 0;
#endif

    if (_status != CLAN_COPY_PROCESS)
        return;

    if ((now - _tickTime) < _tickTimeInterval)
        return;    // 未到达触发下一状态的时间
    _tickTime = now;

#ifdef DEBUG_CLAN_COPY
    *fileSt << "process (" << ++ count << ") start: " << std::endl; 
    *fileSt << "status = " << (UInt32)_status << "." << std::endl; 
#endif

    for (SpotMap::iterator it = _spotMap.begin(); it != _spotMap.end(); ++it)
    {
        if (it->first == Enemy_Base)
            enemyBaseAct();
        else
            monsterAssault(it->first);
    }

    if (_tickCount >= _maxMonsterWave * _monsterRefreshTick + _startTick)
    {
        if (checkWin())
        {
            //　已经胜利
            _status = CLAN_COPY_OVER;
        }
    }
    ++_tickCount;
#ifdef DEBUG_CLAN_COPY
    *fileSt << "tickCount = " << _tickCount << std::endl; 
    *fileSt << "process (" << count << ") end: " << std::endl; 
    *fileSt << "status = " << (UInt32)_status << "." << std::endl << std::endl; 
#endif
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
        player->sendMsgCode(0, 1346); // 人数不足
        return;
    }
    _status = CLAN_COPY_PROCESS;
    _tickTime = _startTime = TimeUtil::Now();
#ifdef DEBUG_CLAN_COPY
    *fileSt << "\"" << player->getName() << "\" start clan copy." << std::endl;
#endif
}

void ClanCopy::adjustPlayerPosition(Player * opPlayer, Player* player, UInt8 oldSpotId, UInt8 oldPosition, UInt8 newSpotId, UInt8 newPositon)
{
    // 调整玩家位置
    std::map<Player *, UInt8>::iterator playerIndexIt = _playerIndex.find(opPlayer);
    if (playerIndexIt == _playerIndex.end())
    {
        opPlayer->sendMsgCode(0, 1346);
        return;
    }

    playerIndexIt = _playerIndex.find(player);

    if (playerIndexIt == _playerIndex.end())
        return;
    if (oldSpotId != playerIndexIt->second)
    {
        notifySpotPlayerInfo(opPlayer);
        return;
    }

    SpotPlayerList& spotPlayerList = _spotPlayer[oldSpotId];
    UInt8 index = 0;
    for (SpotPlayerList::iterator spotPlayerListIt = spotPlayerList.begin(); spotPlayerListIt != spotPlayerList.end(); ++ spotPlayerListIt)
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
                        return;
                    }
                    _spotPlayer[newSpotId].push_back(*spotPlayerListIt);
                    _spotPlayer[oldSpotId].erase(spotPlayerListIt);
                    notifySpotPlayerInfo();
                    return;
                }
                else
                {
                    // 在本据点调整
                    if (!newPositon)
                    {
                        opPlayer->sendMsgCode(0, 1394);
                        notifySpotPlayerInfo(opPlayer);
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
                            if (index == newPositon)
                            {
                                _spotPlayer[newSpotId].insert(spotPlayerListIt, clanCopyPlayer);
                                notifySpotPlayerInfo();
                            }
                        }
                        notifySpotPlayerInfo(opPlayer);
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
            Battle::BattleSimulator bsim(Battle::BS_COPY5, playerIt->player, monsterIt->name, monsterIt->level, false);
            playerIt->player->PutFighters( bsim, 0 );

            size_t c = monsterIt->npcList.size();
            if(c <= 0)
                return;
            bsim.setFormation(1, monsterIt->formation);
            if(monsterIt->npcList[0].fighter->getId() <= GREAT_FIGHTER_MAX)
                bsim.setPortrait(1, monsterIt->npcList[0].fighter->getId());
            else
                bsim.setPortrait(1, monsterIt->npcList[0].fighter->favor);
            for(size_t i = 0; i < c; ++ i)
            {
                bsim.newFighter(1, monsterIt->npcList[i].pos, monsterIt->npcList[i].fighter);
            }

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
                playerIt->deadType = 1; // 标记玩家已死
                // 加入死亡名单
                SpotDeadPlayer::iterator spotDeadPlayerIt = _spotDeadPlayer.find(spotId);
                if (spotDeadPlayerIt != _spotDeadPlayer.end())
                    return;
                (spotDeadPlayerIt->second).push_back (*playerIt); 
                playerIt->player->regenAll();    // 生命值回满
            }
            Stream st(REP::ATTACK_NPC);
            st << static_cast<UInt8>(res) << static_cast<UInt8>(1) << static_cast<UInt32> (0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
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

/*
   void ClanCopy::putSpotMonster(BattleSimulator& bsim, ClanCopyMonster& clanCopyMonster)
   {
   for (std::vector<NpcFData>::iterator it = clanCopyMonster.npcList.begin(); it != clanCopyMonster.npcList.end(); ++ it)
   {

   GObject::Fighter* fighter = m_fighter[idx];
   fighter->attack = m_tlzNpcBaseAttra[idx].attack * (1 + s_rate3NpcBaseModulus[monsterType][0]*tlzLevel);
   fighter->magatk = m_tlzNpcBaseAttra[idx].magatk * (1 + s_rate3NpcBaseModulus[monsterType][1]*tlzLevel);
   fighter->defend = m_tlzNpcBaseAttra[idx].defend * (1 + s_rate3NpcBaseModulus[monsterType][2]*tlzLevel);
   fighter->magdef = m_tlzNpcBaseAttra[idx].magdef * (1 + s_rate3NpcBaseModulus[monsterType][3]*tlzLevel);
   fighter->maxhp = m_tlzNpcBaseAttra[idx].hp * (1 + s_rate3NpcBaseModulus[monsterType][4]*tlzLevel);
   fighter->action = m_tlzNpcBaseAttra[idx].action * (1 + s_rate3NpcBaseModulus[monsterType][5]*tlzLevel);
   fighter->hitrate = m_tlzNpcBaseAttra[idx].hitrate * (1 + s_rate3NpcAdvanceModulus[monsterType][0]*tlzLevel);
   fighter->evade = m_tlzNpcBaseAttra[idx].evade * (1 + s_rate3NpcAdvanceModulus[monsterType][1]*tlzLevel);
   fighter->counter = m_tlzNpcBaseAttra[idx].counter * (1 + s_rate3NpcAdvanceModulus[monsterType][2]*tlzLevel);
   fighter->magres = m_tlzNpcBaseAttra[idx].magres * (1 + s_rate3NpcAdvanceModulus[monsterType][3]*tlzLevel);

   }
//增强NPC的属性
bsim.newFighter(1, form->getPos(i), fighter);
}
*/

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
    ClanCopyMonster clanCopyMonster (clanCopyMonsterData.npcId, clanCopyMonsterData.monsterType, _curMonsterWave, clanCopyMonsterData.npcValue, 0, _tickCount + 1, _copyLevel);

#ifdef DEBUG_CLAN_COPY
    *fileSt << "New monster created. npcId = " << clanCopyMonsterData.npcId << "." << std::endl;
#endif
    // 老巢中放入怪物
    if (clanCopyMonsterData.npcRouteCount == 1)
    {
        // BOSS随机选择一路（很丑陋，但就这么用吧）
        UInt8 rndNum = _rnd(_spotMap[Enemy_Base].nextSpotId.size());
        clanCopyMonster.nextSpotId = _spotMap[Enemy_Base].nextSpotId[rndNum];
        for (UInt16 count = 0; count < clanCopyMonsterData.npcCount; ++ count)
        {
            _spotMonster[Enemy_Base].push_back(clanCopyMonster);
        }
#ifdef DEBUG_CLAN_COPY
        *fileSt << "Next spot Id: " << (UInt32)clanCopyMonster.nextSpotId << "." << std::endl;
        *fileSt << (UInt32)clanCopyMonsterData.npcCount << std::endl;
#endif
        return;
    }
    for (UInt8 i = 0; i < clanCopyMonsterData.npcRouteCount; ++ i)
    {
        // 三路中设置每一路的怪
        clanCopyMonster.nextSpotId = _spotMap[Enemy_Base].nextSpotId[i];
        for (UInt16 count = 0; count < clanCopyMonsterData.npcCount; ++ count)
        {
            _spotMonster[Enemy_Base].push_back(clanCopyMonster);
        }
#ifdef DEBUG_CLAN_COPY
        *fileSt << "Next spot Id: " << (UInt32)clanCopyMonster.nextSpotId << "." << std::endl;
        *fileSt << (UInt32)clanCopyMonsterData.npcCount << std::endl;
#endif
    }
}

/*
   void ClanCopy::start3()
   {
   for (int i = 0; i < 2; ++i)
   {
   GData::NpcGroups::iterator it = GData::npcGroups.find(s_tlzNpcId[m_tjTypeId][i]);
   if(it == GData::npcGroups.end())
   break;
   GData::NpcGroup* _ng = it->second;
   m_tlzNpcName[i] = _ng->getName(); 
   }
   for(int i = 0; i < 2; ++i)
   {
   int npcId = s_tlzNpcId[m_tjTypeId][i];
   GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
   if(it == GData::npcGroups.end())
   break;
   GData::NpcGroup* _ng = it->second;
   std::vector<GData::NpcFData>& _npcList = _ng->getList();
   if (_npcList.size() == 0)
   break;
   GObject::Fighter* fighter = _npcList[0].fighter;
   m_fighter[i] = fighter;

   m_tlzNpcBaseAttra[i].attack = fighter->getBaseAttack(); 
   m_tlzNpcBaseAttra[i].magatk = fighter->getBaseMagAttack();
   m_tlzNpcBaseAttra[i].defend = fighter->getBaseDefend();
   m_tlzNpcBaseAttra[i].magdef = fighter->getBaseMagDefend();
   m_tlzNpcBaseAttra[i].hp     = fighter->getBaseHP();
   m_tlzNpcBaseAttra[i].action = fighter->getBaseAction();
   m_tlzNpcBaseAttra[i].hitrate =fighter->getBaseHitrate();
   m_tlzNpcBaseAttra[i].evade = fighter->getBaseEvade();
   m_tlzNpcBaseAttra[i].counter = fighter->getBaseCounter();
   m_tlzNpcBaseAttra[i].magres = fighter->getBaseMagRes();
   }
   }
   */

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
                    _status = CLAN_COPY_OVER;
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
#ifdef DEBUG_CLAN_COPY
        *fileSt << "monsterValue = " << clanCopyMonster.npcValue << "." << std::endl;
        *fileSt << "Home has been destroyed." << std::endl;
#endif
        return;
    }
    else
    {
        _homeHp -= clanCopyMonster.npcValue;
#ifdef DEBUG_CLAN_COPY
        *fileSt << "monsterValue = " << clanCopyMonster.npcValue << "." << std::endl;
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
    _clan->addCopyLevel();
    _clan->addCopyWinLog(_launchPlayer);
    notifyCopyWin();
    return win;
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

void ClanCopy::notifySpotPlayerInfo(Player * player /* = NULL */)
{
    // 通知玩家该据点状态

    Stream st(REP::CLAN_COPY);
    st << static_cast<UInt8> (CLAN_COPY_MEMBER_LIST_OP);
    st << static_cast<UInt16> (_copyLevel);
    UInt8 count = _spotMap.size();
    st << static_cast<UInt8> (count);
    for (SpotMap::iterator mapIt = _spotMap.begin(); mapIt != _spotMap.end(); ++ mapIt)
    {
        UInt8 spotId = mapIt->first;
        // 每个据点的信息
        st << static_cast<UInt8> (spotId);
        ClanCopySpot &clanCopySpot = mapIt->second;
        st << static_cast<UInt8> (clanCopySpot.maxPlayerCount);
        st << static_cast<UInt8> (clanCopySpot.spotBufferType);
        st << static_cast<UInt16> (clanCopySpot.spotBufferValue);

        SpotPlayer::iterator spotPlayerIt = _spotPlayer.find(spotId);
        if (spotPlayerIt == _spotPlayer.end())
            return;
        SpotDeadPlayer::iterator spotDeadPlayerIt = _spotDeadPlayer.find(spotId);
        if (spotDeadPlayerIt == _spotDeadPlayer.end())
            return;

        st << static_cast<UInt8> ((spotPlayerIt->second).size() + (spotDeadPlayerIt->second).size());
        UInt8 i = 0;
        for (SpotPlayerList::iterator playerListIt =  (spotPlayerIt->second).begin(); 
                playerListIt != spotPlayerIt->second.end();++ i, ++ playerListIt)
        {
            // 据点内每个活玩家的信息
            st << static_cast<UInt8> (i + 1);
            st << static_cast<UInt64> (playerListIt->player->getId());
            st << playerListIt->player->getName();
            st << static_cast<UInt8> (playerListIt->player->GetLev());
            st << static_cast<UInt8> (playerListIt->deadType);
        }
        for (SpotPlayerList::iterator playerDeadListIt =  (spotDeadPlayerIt->second).begin(); 
                playerDeadListIt != spotDeadPlayerIt->second.end();++ i, ++ playerDeadListIt)
        {
            // 据点内每个死/逃跑玩家的信息
            st << static_cast<UInt8> (i + 1);
            st << static_cast<UInt64> (playerDeadListIt->player->getId());
            st << playerDeadListIt->player->getName();
            st << static_cast<UInt8> (playerDeadListIt->player->GetLev());
            st << static_cast<UInt8> (playerDeadListIt->deadType);
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
    class CopyLoseVisitor : public Visitor<ClanMember>
    {
        public:
            CopyLoseVisitor()
            {
            }

            bool operator() (ClanMember * member)
            {
                Stream st (REP::CLAN_COPY);
                st << static_cast<UInt8>(0x02);
                st << static_cast<UInt8>(0x04);
                st << Stream::eos;
                member->player->send(st);
                return true;
            }

    };

    CopyLoseVisitor visitor;
    _clan->VisitMembers(visitor);
}

void ClanCopy::notifyCopyWin()
{
    // 通知玩家副本成功
    class CopyWinVisitor : public Visitor<ClanMember>
    {
        public:
            CopyWinVisitor()
            {
            }

            bool operator() (ClanMember * member)
            {
                Stream st (REP::CLAN_COPY);
                st << static_cast<UInt8>(0x02);
                st << static_cast<UInt8>(0x03);
                st << Stream::eos;
                member->player->send(st);
                return true;
            }

    };

    CopyWinVisitor visitor;
    _clan->VisitMembers(visitor);
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
#ifdef DEBUG_CLAN_COPY
    /*
    static UInt32 count = 0;
    if (!count)
    {
        createClanCopy(GObject::globalPlayers.begin()->second,globalClans[1]);
        std::cout << "ClanCopy create." << std::endl;
        std::cout << "Launcher : " << GObject::globalPlayers.begin()->second->getName() << std::endl;
    }
    ++ count;
    */
#endif
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

bool ClanCopyMgr::createClanCopy(Player* player, Clan *c)
{
    // 创建一个新帮派副本
    if (c == NULL)
        return false;
    if (_clanCopyMap.find(c->getId()) != _clanCopyMap.end())
    {
        if ((_clanCopyMap.find(c->getId()))->second->getStatus() == CLAN_COPY_OVER)
            player->sendMsgCode(0, 1350);
        return false;
    }
    ClanCopy *copy = new ClanCopy(c, player);
    if (!copy)
    {
        // new都失败，那也差不多了
        return false;
    }
    copy->addPlayerFromSpot(c);
    _clanCopyMap.insert(std::make_pair(c->getId(), copy));
    c->notifyCopyCreated(player);
    return true;
}

void ClanCopyMgr::deleteClanCopy(ClanCopy *clanCopy)
{
    // 清除已经结束的帮派副本(Nothing done)
}

void ClanCopyMgr::playerEnter(Player * player)
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
            it->second->addPlayer(player);
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

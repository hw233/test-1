#include "ClanCopy.h"
#include "MsgID.h"
#include "GData/ClanCopyTable.h"
#include "Player.h"
#include "Common/Stream.h"

namespace GObject
{

const static UInt32 COPY_ROUTE_COUNT = 3;  // 帮派副本的路径数目


ClanCopy::ClanCopy(Clan *c) : _clan(c), _copyLevel(0)
{
    _spotMap.clear();
    _spotMonster.clear();
    _spotPlayer.clear();
    _spotDeadPlayer.clear();
    _spotDeadPlayer.clear();

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
}


ClanCopy::~ClanCopy()
{
    _spotMap.clear();
    _spotMonster.clear();
    _spotPlayer.clear();
    _spotDeadPlayer.clear();
}

void ClanCopy::initCopyData()
{
    // TODO: 读取副本配置数据，初始化副本状态
}


void ClanCopy::process(UInt32 now)
{
    // TODO: 游戏状态进行
    if (_startTime == 0)
        return;  // 还在准备状态，游戏未开始

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
    ++_tickCount;
}

void ClanCopy::monsterAssault(const UInt8& spotId)
{
    // TODO: 怪物对据点发起冲锋
    SpotMap::iterator spotIt = _spotMap.find(spotId);
    if (spotIt == _spotMap.end())
    {
        return; // 正常情况下不会进入这里
    }
    ClanCopySpot &spot = spotIt->second;

    SpotPlayer::iterator playerIt = _spotPlayer.find(spotId);
    if (playerIt == _spotPlayer.end())
    {
        return; // 正常情况下不会进入这里
    }
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
    // 开始一轮战斗（FIXME: 战斗状态更新通知客户端）
    SpotPlayerList::iterator playerIt = playerList.begin();
    SpotMonsterList::iterator monsterIt = monsterList.begin(); 
    while (monsterIt != monsterList.end())
    {
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
                    // TODO: 主基地被摧毁，游戏结束
                    ClanCopyMgr::Instance().clanLose(this);
                    return;
                }
            }
            else
            {
                UInt8 nextSpotId = monsterIt->nextSpotId; // 获取目标据点Id
                SpotMonster::iterator spotMonsterIt = _spotMonster.find(nextSpotId); // 目标据点的怪物列表
                if (spotMonsterIt == _spotMonster.end())
                    return;

                monsterIt->justMoved = true;
                (spotMonsterIt->second).push_back(*monsterIt);
            }
            monsterIt->isDead = true;
        }
        monsterList.remove_if(isMonsterDead());
    }
}

void ClanCopy::attackHome(const ClanCopyMonster& clanCopyMonster)
{
    // 怪物攻击主基地
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

void ClanCopy::win()
{
    // TODO: 帮派副本挑战成功
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
    // TODO; 定时器处理所有帮派副本
    for (ClanCopyMap::iterator it = _clanCopyMap.begin(); it != _clanCopyMap.end(); ++ it)
    {
        (it->second).process(now);
    }
}

bool ClanCopyMgr::createClanCopy(Clan *c)
{
    // TODO: 创建一个新帮派副本
    return false;
}

void ClanCopyMgr::deleteClanCopy(Clan *c)
{
    // TODO: 清除已经结束的帮派副本
}

void ClanCopyMgr::clanLose(ClanCopy* clanCopy)
{
    // TODO: 该副本已经失败
}

// ClanCopyMgr
/////////////////////////////////////////////////////////////

}

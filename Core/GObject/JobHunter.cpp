#include "JobHunter.h"

#include "Common/Stream.h"
#include "Common/StringTokenizer.h"
#include "Player.h"
#include "Fighter.h"
#include "MsgID.h"
#include "Battle/BattleSimulator.h"
#include "GData/NpcGroup.h"
#include "Script/GameActionLua.h"
#include "Country.h"

//************************************************
// 用于第四职业招募的——寻墨
//************************************************

namespace GObject
{

JobHunter::JobHunter(Player * player)
    : _owner(player), _slot1(0), _slot2(0), _slot3(0), _strengthPoint(0),
      _gameProgress(PROGRESS_NONE), _posX(0), _posY(0), _stepCount(0)
{
}

JobHunter::~JobHunter()
{
}

bool JobHunter::list2string(std::string& str)
{
    char buf[1024] = {0};
    char* pbuf = buf;
    char* pend = &buf[sizeof(buf)-1];
    for (std::set<UInt16>::iterator it = _fighterList.begin(); it != _fighterList.end(); ++it)
    {
        pbuf += snprintf(pbuf, pend - pbuf, "%u", *it);
        pbuf += snprintf(pbuf, pend - pbuf, ",");
    }

    if (pbuf != buf)
        str = buf;
    return true;
}

void JobHunter::LoadFighterList(std::string& list)
{
    // 加载玩家的待招列表
    StringTokenizer tokenizer(list, ",");
    for (UInt32 i = 0; i < tokenizer.count(); ++ i)
    {
        UInt16 id = atoi(tokenizer[i].c_str());
        if (id)
            _fighterList.insert(id);
    }
}

void JobHunter::LoadGameFromDB(std::string& data)
{
    // TODO: 从数据库加载上次进度
}

void JobHunter::AddToFighterList(UInt16 id)
{
    // 增加至待招列表
    if (_owner->GetLev() < 70)
        return;
    Fighter * fighter = globalFighters[id];
    if (!fighter)
        return;
    if (fighter->getClass() != 4)
        return;

    if (_fighterList.find(id) != _fighterList.end())
        return;
    _fighterList.insert(id);

    std::string list;
    list2string(list);

    DB2().PushUpdateData("REPLACE INTO `ExJob` (`playeyId`, `list`) VALUES (%"I64_FMT"u, '%s')", 
            _owner->getId(), list.c_str());

    SendFighterList();

}

void JobHunter::SendFighterList()
{
    // 发送待招散仙列表
    if (!_owner)
        return;
    Stream st(REP::EXJOB);
    st << static_cast<UInt8>(0x01);
    st << static_cast<UInt8>(_fighterList.size());
    for (std::set<UInt16>::iterator it = _fighterList.begin(); it != _fighterList.end(); ++ it)
    {
        st << static_cast<UInt16>(*it);
    }
    st << Stream::eos;
    _owner->send(st);
}

void JobHunter::OnHireFighter(UInt16 id)
{
    // 雇佣墨家散仙
    if (_fighterList.find(id) == _fighterList.end())
        return;
    _fighterList.erase(id);
    Fighter * fighter = globalFighters[id];
    if (!fighter)
        return;
    _owner->addFighter(fighter, true);
    SendFighterList();
}

void JobHunter::OnRequestStart(UInt8 index)
{
    // 玩家请求开始共鸣
    if ( index >= PROGRESS_MAX)
        return;
    OnUpdateSlot(true);
    SendGameInfo(2);
    InitMap();
    _gameProgress = index;
}

void JobHunter::OnUpdateSlot(bool isAuto /* = false */)
{
    // 老虎机转动
    if (!_gameProgress)
        return;

#if 0
    _equipProb = 0;
    _cittaProb = 0;
    _trumpProb = 0;
#endif

    _strengthPoint = 0;
    UInt8 res[SLOT_MAX] = {0};
    _slot1 = _rnd(SLOT_MAX);
    _slot2 = _rnd(SLOT_MAX);
    _slot3 = _rnd(SLOT_MAX);
    ++(res[_slot1]);
    ++(res[_slot2]);
    ++(res[_slot3]);

#if 0
    // 每个增加的概率
    _equipProb = res[SLOT_DRAGON] * 10 + res[SLOT_TIGER]   * 20;
    _cittaProb = res[SLOT_DRAGON] * 10 + res[SLOT_PHOENIX] * 20;
    _trumpProb = res[SLOT_DRAGON] * 10 + res[SLOT_TIGER]   * 20;

    // 三个相同的额外增加的概率
    _equipProb += res[SLOT_DRAGON] / 3 * 20 + res[SLOT_TIGER]   / 3 * 40;
    _cittaProb += res[SLOT_DRAGON] / 3 * 20 + res[SLOT_PHOENIX] / 3 * 40;
    _trumpProb += res[SLOT_DRAGON] / 3 * 20 + res[SLOT_TURTLE]  / 3 * 40;
#endif

    // 每个增加的概率
    _strengthPoint += res[SLOT_GOLD]  * 25;
    _strengthPoint += res[SLOT_WOOD]  * 20;
    _strengthPoint += res[SLOT_WATAR] * 15;
    _strengthPoint += res[SLOT_FIRE]  * 10;
    _strengthPoint += res[SLOT_MUD]   *  5;
    _strengthPoint += res[SLOT_NONE]   *  0;

    for (UInt8 i = 1; i < SLOT_MAX; ++i)
    {
        if (res[i] == 3)
            _strengthPoint += 25;
        if (res[i] == 2)
            _strengthPoint += 10;
    }
    
    if (!isAuto)
        SendGameInfo(2);
}

void JobHunter::SendGameInfo(UInt8 type)
{
    // 发送寻墨游戏的具体内容
    if (type == 2)
    {
        Stream st (REP::EXJOB);
        st << static_cast<UInt8> (2);

        st << static_cast<UInt8> (_gameProgress);           // 共鸣地点
        st << static_cast<UInt8> (_slot1);                  // 1号摇奖位 
        st << static_cast<UInt8> (_slot2);                  // 2号摇奖位 
        st << static_cast<UInt8> (_slot3);                  // 3号摇奖位 
        st << static_cast<UInt8> (_gameProgress ? 0 : 1);   // 是否已经进入寻墨地图
        st <<Stream::eos;
        _owner->send(st);
    }
}

void JobHunter::SendMapInfo()
{
    // 发送地图信息
    Stream st(REP::JOBHUNTER);
    st << Stream::eos;
    st << static_cast<UInt8>(0);

    for (UInt8 y = 0; y < MAX_POS_Y; ++ y)
    {
        for (UInt8 x = 0; x < MAX_POS_X; ++ x)
        {
            MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(x,y));
            if (it != _mapInfo.end())
            {
                if (!(it->second).gridType | KNOWN_FLAG)
                {
                    st << static_cast<UInt8>(1); // 未探索
                }
                else
                {
                    if ((it->second).gridType == GRID_NORMAL)
                        st << static_cast<UInt8>(3); // 已探索且完成的
                    else if ((it->second).gridType == GRID_BORN)
                        st << static_cast<UInt8>(0x12); // 玩家出生点
                    else
                        st << static_cast<UInt8>(2); // 已探索但有事件未完成的
                }
            }
            else
            {
                st << static_cast<UInt8>(0); // 不可到达的格子
            }
        }
    }
    _owner->send(st);
}

void JobHunter::SendGridInfo(UInt16 pos)
{
    // 发送某一格子的具体信息
    MapInfo::iterator it = _mapInfo.find(pos);
    if (it == _mapInfo.end())
        return;
    Stream st(REP::JOBHUNTER);
    st << static_cast<UInt8>(1);

    st << static_cast<UInt8>(POS_TO_CLIENT_POS(pos));
    if (!(it->second).gridType | KNOWN_FLAG)
    {
        st << static_cast<UInt8>(1); // 未探索
    }
    else
    {
        if ((it->second).gridType == GRID_NORMAL)
            st << static_cast<UInt8>(3); // 已探索且完成的
        else
            st << static_cast<UInt8>(2); // 已探索但有事件未完成的
    }

    st << static_cast<UInt8>(POS_TO_CLIENT_POS(POS_TO_INDEX(_posX, _posY)));

    st << Stream::eos;
    _owner->send(st);
}

void JobHunter::OnCommand(UInt8 command, UInt8 val, UInt8 val2)
{
    // 收到客户端发来请求处理游戏操作
    switch (command)
    {
        case 0:
            // 刷新地图信息
            SendMapInfo();
            break;
        case 1:
            // 移动至某一格
            OnMove(CLIENT_POS_TO_POS(val));
            break;
        case 2:
            // 针对某一格子的具体操作
            switch (val)
            {
                case GRID_MONSTER:
                    if (!val2)
                        OnAttackMonster(POS_TO_INDEX(_posX, _posY));
                    else
                        OnSkipMonster();
                    break;
                case GRID_BOSS:
                    if (!val2)
                        OnAttackMonster(POS_TO_INDEX(_posX, _posY));
                    break;
                case GRID_TREASURE:
                    if (!val2)
                        OnGetTreasure();
                    break;
                case GRID_TRAP:
                    if (!val2)
                        OnSolveTrap();
                    else
                        OnBreakthroughTrap();
                    break;
                case GRID_LENGEND:
                    if (!val2)
                        OnAttackMonster(POS_TO_INDEX(_posX, _posY));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

bool JobHunter::InitMap()
{
    // 初始化地图信息
    std::vector<UInt16> validGrid; // 可选格子
    std::set<UInt16> invalidGrid;  // 暂时无法放入地图的格子
    std::map<UInt16, UInt8> neighbourCount; // 相邻的已经有的格子数

    for (UInt8 x = 0; x < MAX_POS_X; ++ x)
    {
        for (UInt8 y = 0; y < MAX_POS_Y; ++ y)
        {
            invalidGrid.insert(POS_TO_INDEX(x,y));
        }
    }


    // 选取一个初始点作为地图目标点（墨家秘洞）
    UInt16 curPos = POS_TO_INDEX(_rnd(MAX_POS_X), _rnd(MAX_POS_Y));
    UInt8  type = GRID_CAVE;
    validGrid.push_back(curPos);
    UInt16 index = 0;
    invalidGrid.erase(curPos);

    for (UInt8 i = 0; i < MAX_GRID; ++ i)
    {
        // 选择12个点作为地图中可行进点
        _mapInfo.insert(std::make_pair(curPos, GridInfo(curPos, type, neighbourCount[curPos])));
        validGrid.erase(validGrid.begin() + index);
        UInt8 x = 0;
        UInt8 y = 0;
        INDEX_TO_POS(curPos, x, y);

        ////////////////////////////////////////
        // 将新地点四周点放入可选格子
        if (x)
        {
            // 左边
            UInt16 pos = POS_TO_INDEX(x-1,y);
            MapInfo::iterator it = _mapInfo.find(pos);
            if (invalidGrid.find(pos) != invalidGrid.end())
            {
                validGrid.push_back(pos);
                invalidGrid.erase(pos);
            }
            else if (it != _mapInfo.end())
            {
                ++((it->second).neighbCount);
            }
            ++neighbourCount[pos];
        }
        if (y)
        {
            // 上边
            UInt16 pos = POS_TO_INDEX(x,y-1);
            MapInfo::iterator it = _mapInfo.find(pos);
            if (invalidGrid.find(pos) != invalidGrid.end())
            {
                validGrid.push_back(pos);
                invalidGrid.erase(pos);
            }
            else if (it != _mapInfo.end())
            {
                ++((it->second).neighbCount);
            }
            ++neighbourCount[pos];
        }
        if ((x + 1) < MAX_POS_X)
        {
            // 右边
            UInt16 pos = POS_TO_INDEX(x+1,y);
            MapInfo::iterator it = _mapInfo.find(pos);
            if (invalidGrid.find(pos) != invalidGrid.end())
            {
                validGrid.push_back(pos);
                invalidGrid.erase(pos);
            }
            else if (it != _mapInfo.end())
            {
                ++((it->second).neighbCount);
            }
            ++neighbourCount[pos];
        }
        if ((y + 1) < MAX_POS_Y)
        {
            // 下边
            UInt16 pos = POS_TO_INDEX(x,y+1);
            MapInfo::iterator it = _mapInfo.find(pos);
            if (invalidGrid.find(pos) != invalidGrid.end())
            {
                validGrid.push_back(pos);
                invalidGrid.erase(pos);
            }
            else if (it != _mapInfo.end())
            {
                ++((it->second).neighbCount);
            }
            ++neighbourCount[pos];
        }
        // 将新地点四周点放入可选格子
        ////////////////////////////////////////

        if (!validGrid.size())
        {
            DumpMapData();
            return false;
        }
        index = _rnd(validGrid.size());
        curPos = validGrid[index];
        UInt8 prob = _rnd(100) + 1;
        type = GameAction()->calcGridType(prob);
        if (type == GRID_BOSS)
            type = GRID_MONSTER;
    }

    AddBossGrid();
    AddLengendGrid();
    SelectBornGrid();
    DumpMapData();
    return true;
}

void JobHunter::AddBossGrid()
{
    // 选择符合条件的点概率出现boss
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if ((it->second).neighbCount == 1 && (it->second).gridType != GRID_CAVE)
        {
            if (_rnd(2))
            {
                // 该点设置为BOSS点
                (it->second).gridType = GRID_BOSS;
                break;
            }
        }
    }
}

void JobHunter::AddLengendGrid()
{
    // 根据转出的概率选择出现神兽的点
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if ((it->second).gridType != GRID_CAVE)
        {
            if (_rnd(2))
            {
                // 该点设置为神兽点
                (it->second).gridType = GRID_LENGEND;
                break;
            }
        }
    }
}

void JobHunter::SelectBornGrid()
{
    // 选取一个合适的出生点
    UInt8 rndIndex = _rnd(_mapInfo.size());
    UInt8 index = 0;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if ((index == rndIndex) && (it->second).gridType != GRID_CAVE && (it->second).gridType != GRID_LENGEND )
        {
            // 就选择这个作为出生点了
            (it->second).gridType = GRID_BORN;
            return;
        }
        ++ index;
    }

    // 目标点和出生点重复重新选取一个
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if ((it->second).gridType != GRID_CAVE && (it->second).gridType != GRID_LENGEND)
        {
            // 就选择这个作为出生点了
            (it->second).gridType = GRID_BORN;
            return;
        }
    }
}

void JobHunter::OnMove(UInt16 pos)
{
    // 玩家在寻墨游戏中移动
    UInt8 x = 0;
    UInt8 y = 0;
    INDEX_TO_POS(pos, x, y);
    UInt8 dx = (_posX > x) ? (_posX - x) : (x - _posX);
    UInt8 dy = (_posY > y) ? (_posY - y) : (y - _posY);
    if ((dx + dy) != 1)
    {
        _owner->sendMsgCode(0, 2201, x * 100 + y); // 请移动至相邻的坐标
        return;
    }
    MapInfo::iterator it = _mapInfo.find(pos);
    if (it == _mapInfo.end())
    {
        _owner->sendMsgCode(0, 2202, x * 100 + y); // 该地点无法到达
    }

    switch ((it->second).gridType & ~KNOWN_FLAG)
    {
        case GRID_NORMAL:
            break;
        case GRID_MONSTER:
            // 普通怪物，可能被偷袭
            {
                UInt8 prob = _rnd(100);
                if (prob < 50)
                {
                    // 被偷袭直接开打
                    bool res = OnAttackMonster(pos);
                    if (!res)
                    {
                        // 失败，停留在原位
                        (it->second).gridType |= KNOWN_FLAG;
                        return;
                    }
                    else
                    {
                        // 打赢，格子类型变为普通类型
                        (it->second).gridType = GRID_NORMAL;
                    }
                }
            }
            break;
        case GRID_BOSS:
            // Boss，也可能被偷袭
            {
                UInt8 prob = _rnd(100);
                if (prob < 50)
                {
                    // 被偷袭直接开打
                    bool res = OnAttackMonster(pos);
                    if (!res)
                    {
                        // 失败，停留在原位
                    }
                    else
                    {
                        // 打赢，格子类型变为普通类型
                        (it->second).gridType = GRID_NORMAL;
                    }
                }
            }
            break;
        case GRID_TREASURE:
            break;
        case GRID_TRAP:
            break;
        case GRID_LENGEND:
            break;
        case GRID_CAVE:
            break;
        default:
            break;
    }

    //  更新该格子的信息
    (it->second).gridType |= KNOWN_FLAG;
    _posX = x;
    _posY = y;
    SendGridInfo(pos);
}

void JobHunter::OnSkipMonster()
{
    // TODO: 玩家选择避开怪物
    if (! (CheckGridType(GRID_MONSTER) || CheckGridType(GRID_BOSS)))
        return;
}

bool JobHunter::OnAttackMonster(UInt16 pos)
{
    // 攻击怪物
    UInt32 npcId = 0;
    MapInfo::iterator it = _mapInfo.find(pos);
    if (it == _mapInfo.end())
        return false;
    switch((it->second).gridType & ~KNOWN_FLAG)
    {
        case GRID_MONSTER:
            npcId = GameAction()->getRandomNormalMonster(_gameProgress);
            break;
        case GRID_LENGEND:
            npcId = GameAction()->getBossMonster(_gameProgress);
            break;
        case GRID_BOSS:
            npcId = GameAction()->getLengendMonster(_gameProgress, _strengthPoint);
            break;
    }
    if (!npcId)
        return false;
    GData::NpcGroups::iterator npcIt = GData::npcGroups.find(npcId);
    if(npcIt == GData::npcGroups.end())
        return false;

    GData::NpcGroup * ng = npcIt->second;
    Battle::BattleSimulator bsim(Battle::BS_COPY5, _owner, ng->getName(), ng->getLevel(), false);
    bsim.start();

    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;

    UInt16 ret = 0x0100;
    bool res = bsim.getWinner() == 1;
    if (res)
    {
        // 消灭怪物
        ret = 0x0101;
        _owner->_lastNg = ng;
        _owner->pendExp(ng->getExp());
        ng->getLoots(_owner, _owner->_lastLoot, 0, NULL);
    }
    else
    {
        // 被怪物打败
    }

    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(_owner, lastExp) << static_cast<UInt8>(0);
    UInt8 sz = _owner->_lastLoot.size();
    st << sz;
    for(UInt8 i = 0; i < sz; ++ i)
    {
        st << _owner->_lastLoot[i].id << _owner->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << Stream::eos;
    _owner->send(st);
    return res;
}

void JobHunter::OnSolveTrap()
{
    // TODO: 解锁机关
    if (!CheckGridType(GRID_TRAP))
        return;
}

void JobHunter::OnBreakthroughTrap()
{
    // TODO: 前行突破机关
    if (!CheckGridType(GRID_TRAP))
        return;
    UInt8 prob = _rnd(100);
    if (prob < 50)
    {
        // 成功
    }
    else
    {
        // 失败
    }
}

void JobHunter::OnGetTreasure()
{
}

void JobHunter::OnFoundCave()
{
    // TODO: 找到洞穴
    if (!CheckGridType(GRID_CAVE))
        return;
}

void JobHunter::OnAutoExplore()
{
    // TODO: 自动探索
}

void JobHunter::OnAbort()
{
    // TODO: 主动放弃
}


void JobHunter::DumpMapData()
{
#ifdef JOB_HUNTER_DEBUG
    char buffer[1024] = "\nMapInfo:\n    0 1 2 3 4\n\n"; // 5 6 7 8 9 A B C D E F\n";
    char buffer2[1024] = "\nNeighbourInfo:\n    0 1 2 3 4\n\n"; // 5 6 7 8 9 A B C D E F\n";
    for (UInt8 y = 0; y < MAX_POS_Y; ++ y)
    {
        char buf[16];
        snprintf(buf, 16, "%X: ", y);
        strcat (buffer, buf);
        strcat (buffer2, buf);
        for (UInt8 x = 0; x < MAX_POS_X; ++x)
        {
            UInt16 index = POS_TO_INDEX(x,y);
            MapInfo::iterator it = _mapInfo.find(index);
            if (it != _mapInfo.end())
            {
                snprintf(buf, 16, "%2X", (UInt32)((it->second).gridType));
                strcat (buffer, buf);
                snprintf(buf, 16, "%2X", (UInt32)((it->second).neighbCount));
                strcat (buffer2, buf);
            }
            else
            {
                strcat (buffer, " 0");
                strcat (buffer2, " 0");
            }
        }
        strcat(buffer, "\n");
        strcat(buffer2, "\n");
    }
    puts(buffer);
    puts(buffer2);
#else
#endif
}

} 


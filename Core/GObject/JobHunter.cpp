#include "JobHunter.h"

#include "Common/Stream.h"
#include "Common/StringTokenizer.h"
#include "Player.h"
#include "Fighter.h"
#include "Package.h"
#include "MsgID.h"
#include "Battle/BattleSimulator.h"
#include "GData/NpcGroup.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "Map.h"
#include "GObjectDBExecHelper.h"

//************************************************
// 用于第四职业招募的——寻墨
//************************************************

namespace GObject
{

JobHunter::JobHunter(Player * player)
    : _owner(player), _slot1(0), _slot2(0), _slot3(0), _strengthPoint(0),
      _isInGame(false), _gameProgress(PROGRESS_NONE), 
      _posX(0), _posY(0), _earlyPosX(0), _earlyPosY(0), _stepCount(0)
{
    _nextMoveTime = TimeUtil::Now();
    DB2().PushUpdateData("INSERT IGNORE INTO `job_hunter` (`playerId`) VALUES (%"I64_FMT"u)", player->getId()); 
}

JobHunter::JobHunter(Player * player, std::string& fighterList, std::string& mapInfo, UInt8 progress,
        UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt32 stepCount)
    : _owner(player), _gameProgress(progress), _posX(posX), _posY(posY), _earlyPosX(earlyPosX), _earlyPosY(earlyPosY), _stepCount(stepCount)
{
    // 从数据库加载时调用到的构造函数
    LoadFighterList(fighterList);
    LoadMapInfo(mapInfo);
}

JobHunter::~JobHunter()
{
}

bool JobHunter::FighterList2String(std::string& str)
{
    // 获得需要保存的散仙列表字符串
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

bool JobHunter::MapInfo2String(std::string& str)
{
    // 获得需要保存的地图信息字符串
    char buf[1024] = {0};
    char* pbuf = buf;
    char * pend = &buf[sizeof(buf)-1];
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        pbuf += snprintf(pbuf, pend - pbuf, "%u", POS_TO_INDEX((it->second).posX,(it->second).posY));
        pbuf += snprintf(pbuf, pend - pbuf, ",");
        pbuf += snprintf(pbuf, pend - pbuf, "%u", (it->second).neighbCount);
        pbuf += snprintf(pbuf, pend - pbuf, ",");
        pbuf += snprintf(pbuf, pend - pbuf, "%u", (it->second).gridType);
        pbuf += snprintf(pbuf, pend - pbuf, "|");
    }
    if (pbuf != buf)
        str = buf;
    return true;
}

void JobHunter::LoadFighterList(const std::string& list)
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

bool JobHunter::LoadMapInfo(const std::string& list)
{
    // 加载地图数据
    if (!_gameProgress)
        return false;
    StringTokenizer tokenizer(list, "|");
    for (UInt32 i = 0; i < tokenizer.count(); ++ i)
    {
        StringTokenizer tokenizer2(tokenizer[i], ",");
        if (tokenizer.count() < 3)
            return false;
        UInt16 pos = atoi(tokenizer2[0].c_str());
        UInt8 neighbCount = atoi(tokenizer2[1].c_str());
        UInt8 gridType = atoi(tokenizer2[2].c_str());
        _mapInfo.insert(std::make_pair(pos, GridInfo(pos, gridType, neighbCount)));
    }
    return true;
}

void JobHunter::SaveMapInfo()
{
    // 保存地图信息
    std::string mapString;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++it)
    {
        mapString += it->first;
        mapString += ",";
        mapString += (it->second).neighbCount;
        mapString += ",";
        mapString += (it->second).gridType;
        mapString += "|";
    }

    DB2().PushUpdateData("UPDATE `job_hunter` SET `mapInfo` = '%s', `posX` = %u, `posY` = %u, `earlyPosX` = %u, `earlyPosY` = %u WHERE `playerId` = %"I64_FMT"u", 
            mapString.c_str(), _posX, _posY, _earlyPosX, _earlyPosY, _owner->getId());
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
    FighterList2String(list);

    DB2().PushUpdateData("UPDATE `job_hunter` SET `fighterList` = '%s') WHERE `playerId` = %"I64_FMT"u", list.c_str(), _owner->getId());

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

    std::string list;
    FighterList2String(list);

    DB2().PushUpdateData("UPDATE `job_hunter` SET `fighterList` = '%s') WHERE `playerId` = %"I64_FMT"u", list.c_str(), _owner->getId());

    SendFighterList();
}

void JobHunter::OnRequestStart(UInt8 index)
{
    // 玩家请求开始共鸣
    if ( index >= PROGRESS_MAX)
        return;
    if (!_owner->GetPackage()->GetItemAnyNum(EX_JOB_ITEM_ID))
    {
        _owner->sendMsgCode(0, 2204, 0); // 墨家徽记不足
        return;
    }
    _owner->GetPackage()->DelItemAny(EX_JOB_ITEM_ID, 1);
    _gameProgress = index;
    //OnUpdateSlot(true);
    SendGameInfo(2);
    InitMap();
    UInt16 spot = GetSpotIdFromGameId(index);
    _owner->moveTo(spot, true);
    DB2().PushUpdateData("UPDATE `job_hunter` SET `progress` = '%u') WHERE `playerId` = %"I64_FMT"u", _gameProgress, _owner->getId());
    std::cout << "Move to " << (UInt32) spot << "."  << std::endl;
}

void JobHunter::OnUpdateSlot(bool isAuto /* = false */)
{
    // 老虎机转动
    //if (!_gameProgress)
        //return;

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
    //_strengthPoint += res[SLOT_FIRE]  * 10;
    //_strengthPoint += res[SLOT_MUD]   *  5;
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
        st << static_cast<UInt8> (_isInGame ? 1 : 0);   // 是否已经进入寻墨地图
        st <<Stream::eos;
        _owner->send(st);
    }
}

void JobHunter::SendMapInfo()
{
    // 发送地图信息
    Stream st(REP::JOBHUNTER);
    st << static_cast<UInt8>(0);

    for (UInt8 y = 0; y < MAX_POS_Y; ++ y)
    {
        for (UInt8 x = 0; x < MAX_POS_X; ++ x)
        {
            MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(x,y));
            if (it != _mapInfo.end())
            {
#if 0
                if ((it->second).gridType & UNKNOWN_FLAG)
                {
                    st << static_cast<UInt8>(UNKNOWN_FLAG); // 未探索
                }
                else
                {
                    st << static_cast<UInt8>((it->second).gridType);
                }
#else
                st << static_cast<UInt8>((it->second).gridType);
#endif
            }
            else
            {
                st << static_cast<UInt8>(0); // 不可到达的格子
            }
        }
    }
    st << static_cast<UInt8>(POS_TO_CLIENT_POS(POS_TO_INDEX(_posX, _posY)));
    st << Stream::eos;
    _owner->send(st);
}

void JobHunter::SendAutoInfo()
{
    // TODO: 发送自动探索的信息
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
#if 0
    if (!((it->second).gridType & UNKNOWN_FLAG))
    {
        st << static_cast<UInt8>(UNKNOWN_FLAG); // 未探索
    }
    else
    {
            st << static_cast<UInt8>((it->second).gridType); 
    }
#else
    st << static_cast<UInt8>((it->second).gridType); 
#endif

    st << static_cast<UInt8>(POS_TO_CLIENT_POS(POS_TO_INDEX(_posX, _posY)));

    st << Stream::eos;
    _owner->send(st);
    DumpMapData();
}

void JobHunter::OnCommand(UInt8 command, UInt8 val, UInt8 val2)
{
    // 收到客户端发来请求处理游戏操作
    UInt32 now = TimeUtil::Now();
    if (val && now < _nextMoveTime)
    {
        _owner->sendMsgCode(0, 2203, _nextMoveTime - now);
        return;
    }
            
    switch (command)
    {
        case 0:
            // 刷新地图信息
            if (!_isInGame)
                SendGameInfo(2);
            _isInGame = true;
            DumpMapData();
            SendMapInfo();
            return;
            break;
        case 1:
            // 移动至某一格
            OnMove(CLIENT_POS_TO_POS(val));
            break;
        case 2:
            // 针对某一格子的具体操作
            {
                MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
                if (it == _mapInfo.end())
                    return;
                if ((it->second).gridType & CLEAR_FLAG)
                {
                    SendGridInfo(POS_TO_INDEX(_posX, _posY));
                    return;
                }
                switch ((it->second).gridType & 0x0F)
                {
                    case GRID_MONSTER:
                    case GRID_BOSS:
                        if (!val)
                            OnAttackMonster(POS_TO_INDEX(_posX, _posY));
                        else
                            OnSkipMonster();
                        break;
                    case GRID_TREASURE:
                        if (!val)
                            OnGetTreasure();
                        break;
                    case GRID_CAVE:
                        if(!val)
                            OnFoundCave();
                        break;
                    case GRID_LENGEND:  // 功能已去除
                        /*
                           if (!val)
                           OnAttackMonster(POS_TO_INDEX(_posX, _posY));
                           break;
                           */
                    case GRID_TRAP: // 功能已去除
                        /*
                           if (!val)
                           OnSolveTrap();
                           else
                           OnBreakthroughTrap();
                           break;
                           */
                    default:
                        break;
                }
            }
            break;
        case 3:
            _owner->checkLastExJobAward();
            return;
            break;
        default:
            break;
    }
    SaveMapInfo();
    DumpMapData();
}

UInt16 JobHunter::GetSpotIdFromGameId(UInt8 id)
{
    // 根据游戏id获取对应的据点id
    Map *map = Map::FromID(MAP_ID_INDEX[id]);
    if (!map)
        return 0;
    else
        return map->GetRandomSpot(9);
}

bool JobHunter::InitMap()
{
    // 初始化地图信息
    UInt8 gridCount[GRID_NORMAL_MAX] = {0};
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
    UInt8  type = GRID_MONSTER;
    validGrid.push_back(curPos);
    UInt16 index = 0;
    invalidGrid.erase(curPos);

    for (UInt8 i = 0; i < MAX_GRID; ++ i)
    {
        // 选择12个点作为地图中可行进点
        UInt8 prob = _rnd(100);
        if (prob < 70)
        {
            type = GRID_MONSTER;
        }
        else
        {
            type = GRID_TREASURE;
        }

        if ( ++(gridCount[type]) >= MAX_GRID_COUNT[type])
            type = GRID_MONSTER;

        type |= UNKNOWN_FLAG;

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


#if 0
        type = GameAction()->calcGridType(prob);
        if (type == GRID_BOSS)
            type = GRID_MONSTER;
#endif
    }

    SelectBossGrid();
    SelectCaveGrid();
    SelectBornGrid();
    AddMinTreasureGrid();
    SaveMapInfo();
    DumpMapData();
    return true;
}

void JobHunter::SelectBossGrid()
{
    // 选择符合条件的点概率出现boss
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if ((it->second).neighbCount == 1)
        {
            // 该点设置为BOSS点
            (it->second).gridType = GRID_BOSS | UNKNOWN_FLAG;
            break;
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

void JobHunter::SelectCaveGrid()
{
    // 选取一个地点作为目标点
    UInt8 rndIndex = _rnd(_mapInfo.size());
    UInt8 index = 0;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if ((index == rndIndex) && ((it->second).gridType & 0x0F) != GRID_BOSS)
        {
            // 就选择这个作为目标点
            (it->second).gridType = GRID_CAVE | UNKNOWN_FLAG;
            return;
        }
        ++ index;
    }

    // 洞穴点和boss或者出生点重复重新选取一个
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if (((it->second).gridType & 0x0F) != GRID_BOSS)
        {
            // 就选择这个作为目标点
            (it->second).gridType = GRID_CAVE | UNKNOWN_FLAG;
            return;
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
        if ((index == rndIndex) && ((it->second).gridType & 0x0F) != GRID_BOSS && ((it->second).gridType & 0x0F) != GRID_CAVE )
        {
            // 就选择这个作为出生点了
            (it->second).gridType = GRID_NORMAL;
            _earlyPosX = _posX = (it->second).posX;
            _earlyPosY = _posY = (it->second).posY;
            return;
        }
        ++ index;
    }

    // BOSS点和出生点重复重新，选取一个
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if (((it->second).gridType & 0x0F) != GRID_BOSS && ((it->second).gridType & 0x0F) != GRID_CAVE )
        {
            // 就选择这个作为出生点了
            (it->second).gridType = GRID_NORMAL;
            _earlyPosX = _posX = (it->second).posX;
            _earlyPosY = _posY = (it->second).posY;
            return;
        }
    }
}

void JobHunter::AddMinTreasureGrid()
{
    // 至少一个宝箱点
    MapInfo::iterator it;
    for (it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if (((it->second).gridType & 0x0F) == GRID_TREASURE)
            return;
    }
    for (it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if (((it->second).gridType & 0x0F) == GRID_MONSTER)
            (it->second).gridType = GRID_TREASURE | UNKNOWN_FLAG;
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
        return;
    }

    if (((it->second).gridType & CLEAR_FLAG))
    {
        // 该格子事件已经完成
    }
    else
    {
        // 该格子事件未完成
        switch ((it->second).gridType & 0x0F)
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
                        //bool res = OnAttackMonster(pos);
                        OnAttackMonster(pos);
                    }
                }
                break;
            case GRID_BOSS:
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
    }

    //  更新该格子的信息
    (it->second).gridType &= ~UNKNOWN_FLAG;

    _earlyPosX = _posX;
    _earlyPosY = _posY;

    _posX = x;
    _posY = y;
    ++ _stepCount;

    SendGridInfo(pos);
}

void JobHunter::OnSkipMonster()
{
    // 玩家选择避开怪物
    if (! (CheckGridType(GRID_MONSTER) || CheckGridType(GRID_BOSS)))
        return;
    _posX = _earlyPosX;
    _posY = _earlyPosY;
    SendGridInfo(POS_TO_INDEX(_posX, _posY));
    
}

bool JobHunter::OnAttackMonster(UInt16 pos)
{
    // 攻击怪物
    UInt32 npcId = 0;
    MapInfo::iterator it = _mapInfo.find(pos);
    if (it == _mapInfo.end())
        return false;
    switch((it->second).gridType & 0x0F)
    {
        case GRID_MONSTER:
            npcId = GameAction()->getRandomNormalMonster(_gameProgress);
            break;
        //case GRID_LENGEND:
            //npcId = GameAction()->getLengendMonster(_gameProgress);
            //break;
        case GRID_BOSS:
            npcId = GameAction()->getBossMonster(_gameProgress);
            break;
    }
    if (!npcId)
        return false;
    GData::NpcGroups::iterator npcIt = GData::npcGroups.find(npcId);
    if(npcIt == GData::npcGroups.end())
        return false;

    GData::NpcGroup * ng = npcIt->second;
    Battle::BattleSimulator bsim(Battle::BS_COPY5, _owner, ng->getName(), ng->getLevel(), false);
    _owner->PutFighters(bsim, 0);
    ng->putFighters(bsim);
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
        (it->second).gridType |= CLEAR_FLAG;
    }
    else
    {
        // 被怪物打败
        _posX = _earlyPosX;
        _posY = _earlyPosY;
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
    // 解锁机关 (已删除功能)
    if (!CheckGridType(GRID_TRAP))
        return;
    _nextMoveTime = TimeUtil::Now() + 30;
    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
    (it->second).gridType = GRID_NORMAL;
    SendGridInfo(POS_TO_INDEX(_posX, _posY));
}

void JobHunter::OnBreakthroughTrap()
{
    // 前行突破机关 (已删除功能)
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
        _nextMoveTime = TimeUtil::Now() + 60;
    }
    SendGridInfo(POS_TO_INDEX(_posX, _posY));
}

void JobHunter::OnGetTreasure(bool isAuto /* = false */)
{
    // 打开宝箱
    if (!CheckGridType(GRID_TREASURE))
        return;
    Table rewards = GameAction()->getTreasure(_gameProgress);
    Stream st(REP::JOBHUNTER);
    st << static_cast<UInt8>(3);
    UInt8 count = rewards.size();
    st << static_cast<UInt8>(count);
    for (UInt8 i = 0; i < count; ++i)
    {
        Table item = rewards.get<Table>(i + 1);
        if (item.size() < 3) return;
        UInt32 itemId = item.get<UInt32>(1);
        UInt32 itemCount = item.get<UInt32>(2);
        bool   bind = item.get<bool>(3);
        _owner->GetPackage()->AddItem2(itemId, itemCount, true, bind, FromJobHunter);
        st << static_cast<UInt32>(itemId);
        st << static_cast<UInt32>(itemCount);
    }
    st << Stream::eos;
    _owner->send(st);

    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
    (it->second).gridType |= CLEAR_FLAG;
}

bool JobHunter::OnFoundCave(bool isAuto /* = false */)
{
    // 找到洞穴
    if (!CheckGridType(GRID_CAVE))
        return false;
    UInt32 npcId = GameAction()->foundCave(_gameProgress);
    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));

    if (!npcId)
        return false;
    GData::NpcGroups::iterator npcIt = GData::npcGroups.find(npcId);
    if(npcIt == GData::npcGroups.end())
        return false;

    GData::NpcGroup * ng = npcIt->second;
    Battle::BattleSimulator bsim(Battle::BS_COPY5, _owner, ng->getName(), ng->getLevel(), false);
    _owner->PutFighters(bsim, 0);
    ng->putFighters(bsim);
    bsim.start();

    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;

    UInt16 ret = 0x0100;
    bool res = bsim.getWinner() == 1;
    if (res)
    {
        // 消灭怪物
        (it->second).gridType |= CLEAR_FLAG;
        ret = 0x0101;
        _owner->_lastNg = ng;
        _owner->pendExp(ng->getExp());
        ng->getLoots(_owner, _owner->_lastExJobAward, 0, NULL);

    }
    else
    {
        // 被怪物打败
        _posX = _earlyPosX;
        _posY = _earlyPosY;
    }

    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(_owner, lastExp) << static_cast<UInt8>(0);
    st << static_cast<UInt8>(0);
    st.append(&packet[8], packet.size() - 8);
    st << Stream::eos;
    _owner->send(st);

    UInt16 fighterId = (_rnd(100) + 10);
    AddToFighterList(static_cast<UInt16>(fighterId));

    Stream st2(REP::JOBHUNTER);
    st2 << static_cast<UInt8>(2);
    st2 << static_cast<UInt16>(fighterId);

    UInt8 sz = _owner->_lastExJobAward.size();
    st2 << sz;
    for(UInt8 i = 0; i < sz; ++ i)
    {
        st2 << static_cast<UInt16>(_owner->_lastExJobAward[i].id) << static_cast<UInt16>(_owner->_lastExJobAward[i].count);
    }

    // 步数奖励配置
    Table rewards = GameAction()->getStepAward(_stepCount);
    UInt8 count = rewards.size();
    st2 << static_cast<UInt16>(count);
    std::cout << "Count = " << (UInt32) count << "." << std::endl;
    for (UInt8 i = 0; i < count; ++i)
    {
        Table item = rewards.get<Table>(i + 1);
        if (item.size() < 3) break;
        UInt32 itemId = item.get<UInt32>(1);
        UInt32 itemCount = item.get<UInt32>(2);
        //bool   bind = item.get<bool>(3);
        st2 << static_cast<UInt16>(itemId) << static_cast<UInt16>(itemCount);
        _owner->lastExJobAwardPush(itemId, itemCount);
    }
    st2 << Stream::eos;
    _owner->send(st2);

    SendFighterList();
    return res;
}

void JobHunter::OnAutoExplore()
{
    // TODO: 自动探索
}

void JobHunter::OnAbort()
{
    // 主动放弃
    _gameProgress = PROGRESS_NONE;
    _stepCount = 0;
    _posX = _posY = 0;
    _earlyPosX = _earlyPosY = 0;
    _mapInfo.clear();
    _isInGame = false;
    DB2().PushUpdateData("UPDATE `job_hunter` SET `progress` = '%s') WHERE `playerId` = %"I64_FMT"u", _gameProgress, _owner->getId());
    SendGameInfo(2);

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
    //puts(buffer2);
    snprintf (buffer, 1024, "Player pos: x = 0X%X, y = 0X%X. Step count = %d.\n", _posX, _posY, _stepCount);
    puts(buffer);
#else
#endif
}

} 


//************************************************
// 用于水晶梦境
//************************************************
#include "Dreamer.h"
#include "Player.h"
#include "Package.h"
#include "MsgID.h"
#include "Country.h"
#include "Script/GameActionLua.h"

namespace GObject
{

Dreamer::Dreamer(Player * player)
    :_owner(player), _maxX(0), _maxY(0), _maxGrid(0), 
    _isInGame(false), _type(0), _gameProgress(0),
    _posX(0), _posY(0), _earlyPosX(0), _earlyPosY(0),
    _remainTime(0)
{
    for (UInt8 i = 1; i < MAX_LEVEL + 1; ++i)
    {
        InitMap (i);
        _mapInfo.clear();
    }
}

Dreamer::~Dreamer()
{
}

void Dreamer::OnCommand(UInt8 command, UInt8 val)
{
    // 收到客户端发来请求处理游戏操作
    switch (command)
    {
        case 1:
            // 移动至某一格
            OnMove((val));
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
                    case GRID_NORMAL:
                        break;
                    case GRID_ARROW:
                        break;
                    case GRID_KEY:
                        break;
                    case GRID_TREASURE:
                        break;
                    case GRID_ITEM:
                        break;
                    case GRID_TRANSPORT:
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }

    DumpMapData();
}

bool Dreamer::InitMap(UInt8 level)
{
    // TODO: 初始化地图信息
    if (level > MAX_LEVEL || level == 0)
    {
        return false;
    }
    _maxX = MAX_X[level];
    _maxY = MAX_Y[level];
    _maxGrid = DREAMER_MAX_GRID[level];
    std::vector<UInt16> validGrid; // 可选格子
    std::set<UInt16> invalidGrid;  // 暂时无法放入地图的格子
    std::map<UInt16, UInt8> neighbourCount; // 相邻的已经有的格子数
    std::map<UInt16, UInt8> typeMap;        // 可用的地图类型

    for (UInt8 x = 0; x < _maxX; ++ x)
    {
        for (UInt8 y = 0; y < _maxY; ++ y)
        {
            invalidGrid.insert(POS_TO_INDEX(x,y));
        }
    }


    // 选取一个初始点作为地图目标点（传送点）
    UInt16 curPos = POS_TO_INDEX(_rnd(_maxX), _rnd(_maxY));
    UInt8  type = GRID_TRANSPORT;
    validGrid.push_back(curPos);
    UInt16 index = 0;
    invalidGrid.erase(curPos);

    for (UInt8 i = 0; i < _maxGrid; ++ i)
    {
        // 选择N个点作为地图中可行进点
        type = GRID_NORMAL;
        UInt8 tmpIndex = _rnd(typeMap.size());
        UInt8 tmp = 0;
        for (std::map<UInt16, UInt8>::iterator tmpIt = typeMap.begin(); tmpIt != typeMap.end(); ++ tmpIt)
        {
            if (tmpIndex == tmp)
            {
                type = tmpIt->first;
                if (!(--(tmpIt->second)))
                {
                    typeMap.erase(tmpIt);
                }
                break;
            }
            ++tmp;
        }



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
        if ((x + 1) < _maxX)
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
        if ((y + 1) < _maxY)
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

    }

    /*
    AddBossGrid(validGrid, neighbourCount);
    SelectCaveGrid();
    SelectBornGrid();
    AddMinTreasureGrid();
    SaveMapInfo();
    */
    DumpMapData();
    return true;
}

bool Dreamer::InitArrow()
{
    // TODO: 初始化箭头方向和类型（一定很坑爹玩家）
    return true;
}

void Dreamer::OnMove(UInt16 pos)
{
    // 玩家在梦境游戏中移动
    UInt8 x = 0;
    UInt8 y = 0;
    INDEX_TO_POS(pos, x, y);
    UInt8 dx = (_posX > x) ? (_posX - x) : (x - _posX);
    UInt8 dy = (_posY > y) ? (_posY - y) : (y - _posY);
    if ((dx + dy) != 1)
    {
        _owner->sendMsgCode(0, 2201, x * 100 + y); // 请移动至相邻的坐标
        SendMapInfo();
        return;
    }
    MapInfo::iterator it = _mapInfo.find(pos);
    if (it == _mapInfo.end())
    {
        _owner->sendMsgCode(0, 2202, x * 100 + y); // 该地点无法到达
        SendMapInfo();
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
            case GRID_ARROW:
                break;
            case GRID_KEY:
                break;
            case GRID_ITEM:
                break;
            case GRID_TRANSPORT:
                break;
            case GRID_TREASURE:
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

    SendGridInfo(pos);
}

void Dreamer::OnGetTreasure()
{
    // TODO: 打开宝箱
    UInt8 index = CheckGridType(GRID_TREASURE);
    if (!index)
        return;
    Table rewards = GameAction()->getDreamerTreasure(_gameProgress, index);
    Stream st;
    st.init(REP::AUTOJOBHUNTER);
    st << static_cast<UInt8>(14);
    UInt8 count = rewards.size();
    st << static_cast<UInt8>(count);
    for (UInt8 i = 0; i < count; ++i)
    {
        Table item = rewards.get<Table>(i + 1);
        if (item.size() < 3) return;
        UInt32 itemId = item.get<UInt32>(1);
        UInt32 itemCount = item.get<UInt32>(2);
        bool   bind = item.get<bool>(3);
        _owner->GetPackage()->AddItem2(itemId, itemCount, true, bind, FromDreamer);
        st << static_cast<UInt16>(itemId);
        st << static_cast<UInt16>(itemCount);
    }
    st << Stream::eos;
    //_owner->send(st);

    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
    (it->second).gridType |= CLEAR_FLAG;

}

void Dreamer::SendGridInfo(UInt16 pos)
{
    // TODO: 发送某一格子的具体信息
    MapInfo::iterator it = _mapInfo.find(pos);
    if (it == _mapInfo.end())
        return;
    /*
    Stream st(REP::JOBHUNTER);
    st << static_cast<UInt8>(1);

    st << static_cast<UInt8>(POS_TO_CLIENT_POS(pos));
    st << static_cast<UInt8>((it->second).gridType); 

    st << static_cast<UInt8>(POS_TO_CLIENT_POS(POS_TO_INDEX(_posX, _posY)));

    st << Stream::eos;
    _owner->send(st);
    */
    DumpMapData();
}

void Dreamer::SendMapInfo()
{
    // 发送地图信息
    /*
    Stream st(REP::JOBHUNTER);
    st << static_cast<UInt8>(0);

    for (UInt8 y = 0; y < _maxY; ++ y)
    {
        for (UInt8 x = 0; x < _maxX; ++ x)
        {
            MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(x,y));
            if (it != _mapInfo.end())
            {
                st << static_cast<UInt8>((it->second).gridType);
            }
            else
            {
                st << static_cast<UInt8>(0); // 不可到达的格子
            }
        }
    }
    st << static_cast<UInt8>(POS_TO_INDEX(_posX, _posY));
    st << Stream::eos;
    _owner->send(st);
    */
}

void Dreamer::DumpMapData()
{
#ifdef DREAMER_DEBUG
    char buffer[1024] = "\nMapInfo:\n    0 1 2 3 4 5 6 7 8 9 A B C D E F\n\n";
    char buffer2[1024] = "\nNeighbourInfo:\n    0 1 2 3 4 5 6 7 8 9 A B C D E F\n\n";
    for (UInt8 y = 0; y < 0x0F; ++ y)
    {
        char buf[16];
        snprintf(buf, 16, "%X: ", y);
        strcat (buffer, buf);
        strcat (buffer2, buf);
        for (UInt8 x = 0; x < 0x0F; ++x)
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
    snprintf (buffer, 1024, "Player pos: x = 0X%X, y = 0X%X. Step count = %d.\n", _posX, _posY, _remainTime);
    puts(buffer);
#else
#endif
}

#if 0
Dreamer::Dreamer(Player * player)
    : _owner(player), _spotId(0), 
    _slot1(0), _slot2(0), _slot3(0), _isInGame(false), _gameProgress(PROGRESS_NONE), 
    _posX(0), _posY(0), _earlyPosX(0), _earlyPosY(0), _remainTime(0), _isInAuto(false), _isAutoLose(false)
{
    _nextMoveTime = TimeUtil::Now();
    DB2().PushUpdateData("INSERT IGNORE INTO `job_hunter` (`playerId`) VALUES (%"I64_FMT"u)", player->getId()); 
    memset(_spItemRate, 0, sizeof(_spItemRate));
}

Dreamer::Dreamer(Player * player, std::string& fighterList, std::string& mapInfo, UInt8 progress,
        UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt32 stepCount, UInt8 slotVal1, UInt8 slotVal2, UInt8 slotVal3)
    : _owner(player), _spotId(0), _slot1(slotVal1), _slot2(slotVal2), _slot3(slotVal3), 
    _gameProgress(progress), _posX(posX), _posY(posY), _earlyPosX(earlyPosX), _earlyPosY(earlyPosY), _remainTime(stepCount),
      _isInAuto(false), _isAutoLose(false)
{
    // 从数据库加载时调用到的构造函数
    LoadFighterList(fighterList);
    LoadMapInfo(mapInfo);
    _nextMoveTime = TimeUtil::Now();
    if (_gameProgress != PROGRESS_NONE)
        _spotId = _owner->GetVar(VAR_JOB_HUNTER_SPOT_ID);

    UInt8 res[SLOT_MAX] = {0};
    ++(res[_slot1]);
    ++(res[_slot2]);
    ++(res[_slot3]);

    for (UInt8 i = 1; i < SLOT_MAX; ++i)
    {
        _spItemRate[i] = ITEM_RATE[res[i]];
    }

}

Dreamer::~Dreamer()
{
}

bool Dreamer::FighterList2String(std::string& str)
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

bool Dreamer::MapInfo2String(std::string& str)
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

void Dreamer::LoadFighterList(const std::string& list)
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

bool Dreamer::LoadMapInfo(const std::string& list)
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
        if (!(gridType & UNKNOWN_FLAG) && pos != POS_TO_INDEX(_posX,_posY))
            _isInGame = true;
    }
    return true;
}

void Dreamer::SaveMapInfo()
{
    // 保存地图信息
    char buf[1024] = {0};
    char* pbuf = buf;
    char* pend = &buf[sizeof(buf)-1];
    std::string mapString;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++it)
    {
        pbuf += snprintf(pbuf, pend - pbuf, "%u", (UInt32)it->first);
        pbuf += snprintf(pbuf, pend - pbuf, ",");
        pbuf += snprintf(pbuf, pend - pbuf, "%u", (UInt32)(it->second).neighbCount);
        pbuf += snprintf(pbuf, pend - pbuf, ",");
        pbuf += snprintf(pbuf, pend - pbuf, "%u", (UInt32)(it->second).gridType);
        pbuf += snprintf(pbuf, pend - pbuf, "|");
    }


    if (pbuf != buf)
        mapString = buf;

    DB2().PushUpdateData("UPDATE `job_hunter` SET `mapInfo` = '%s', `posX` = %u, `posY` = %u, `earlyPosX` = %u, `earlyPosY` = %u, `stepCount` = %u WHERE `playerId` = %"I64_FMT"u", 
            mapString.c_str(), _posX, _posY, _earlyPosX, _earlyPosY, _remainTime, _owner->getId());
}

void Dreamer::AddToFighterList(UInt16 id)
{
    // 增加至待招列表
    if (_owner->GetLev() < 70)
        return;
    Fighter * fighter = globalFighters[id];
    if (!fighter)
        return;
    if (fighter->getClass() != 4)
        return;
    if (_owner->hasFighter(id))
        return;

    if (_fighterList.find(id) != _fighterList.end())
        return;
    _fighterList.insert(id);

    std::string list;
    FighterList2String(list);

    DB2().PushUpdateData("UPDATE `job_hunter` SET `fighterList` = '%s' WHERE `playerId` = %"I64_FMT"u", list.c_str(), _owner->getId());

    SendFighterList();
}

void Dreamer::SendFighterList()
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

void Dreamer::OnHireFighter(UInt16 id)
{
    // 雇佣墨家散仙
    if (_fighterList.find(id) == _fighterList.end())
        return;
    if (_owner->isFighterFull())
    {
        _owner->sendMsgCode(0, 1200);
        return;
    }
    if (_owner->takeFighter(id, true) == NULL)
        return;
    if (!_owner->GetShuoShuo()->getShuoShuo(SS_MO_HIRE))
        _owner->OnShuoShuo(SS_MO_HIRE);
    if (!_owner->GetVar(VAR_EX_JOB_HAS_HAD))
    {
        _owner->SetVar(VAR_EX_JOB_HAS_HAD, 1);
        Stream st(REP::EXJOB);
        st << static_cast<UInt8>(0xFF);
        st << Stream::eos;
        _owner->send(st);

                Mail *pmail = NULL;
                MailPackage::MailItem mitem[9] = {{2872, 1}, {2873, 1}, {2874, 1}, {2875, 1}, {2876, 1}, {2877, 1}, {2878, 1}, {2879, 1}, {1656, 1}};
                MailItemsInfo itemsInfo(mitem, NEWJOBHIRE, 9);

                SYSMSG(title, 4070);
                SYSMSG(content, 4071);
                pmail = _owner->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);

                if(pmail != NULL)
                {
                    mailPackageManager.push(pmail->id, mitem, 9, true);
                }
    }
    _fighterList.erase(id);

    std::string list;
    FighterList2String(list);

    DB2().PushUpdateData("UPDATE `job_hunter` SET `fighterList` = '%s' WHERE `playerId` = %"I64_FMT"u", list.c_str(), _owner->getId());

    SendFighterList();
}

void Dreamer::OnRequestStart(UInt8 index)
{
    // 玩家请求开始共鸣
    if ( index >= PROGRESS_MAX)
        return;
    if ( _gameProgress != PROGRESS_NONE)
    {
        _owner->sendMsgCode(0, 2206, 0);
        return;
    }

    TeamCopyPlayerInfo* tcp = _owner->getTeamCopyPlayerInfo();
    if (!tcp || !tcp->getPass(index + 3))
    {
        _owner->sendMsgCode(0, 2207, 0);
        return;
    }
    
    if (!_owner->GetPackage()->GetItemAnyNum(EX_JOB_ITEM_ID))
    {
        _owner->sendMsgCode(0, 2204, 0); // 墨家徽记不足
        return;
    }
    _owner->GetPackage()->DelItemAny(EX_JOB_ITEM_ID, 1);
    _gameProgress = index;
    OnUpdateSlot(true);
    InitMap();
    _spotId = GetSpotIdFromGameId(index);
    _owner->SetVar(VAR_JOB_HUNTER_SPOT_ID, _spotId);
    SendGameInfo(2, true);
    DB2().PushUpdateData("UPDATE `job_hunter` SET `progress` = '%u' WHERE `playerId` = %"I64_FMT"u", _gameProgress, _owner->getId());
    _owner->udpLog("jobHunter", "F_1161", "", "", "", "", "act");
}

bool Dreamer::IsInGame()
{
    return _isInGame;
}

bool Dreamer::IsInAuto()
{
    return false;
}

void Dreamer::SendGameInfo(UInt8 type, bool isUpdeated /* = false */)
{
    // 发送寻墨游戏的具体内容
    UInt8 flag = isUpdeated ? 0x10:0;
    if (type == 2)
    {
        Stream st (REP::EXJOB);
        st << static_cast<UInt8> (2);

        st << static_cast<UInt8> (_gameProgress);       // 共鸣地点
        st << static_cast<UInt8> (_slot1 | flag);       // 1号摇奖位 
        st << static_cast<UInt8> (_slot2 | flag);       // 2号摇奖位 
        st << static_cast<UInt8> (_slot3 | flag);       // 3号摇奖位 
        st << static_cast<UInt8> (_isInGame ? 1 : 0);   // 是否已经进入寻墨地图
        st << static_cast<UInt16>(_spotId);             // 寻墨需要所在的据点
        UInt8 passBit = 0;
        TeamCopyPlayerInfo* tcp = _owner->getTeamCopyPlayerInfo();
        if (tcp)
        {
            for (UInt8 i = 0; i < 4; ++i)
            {
                if (tcp->getPass(4 + i))
                {
                    passBit |= 0x01 << i;
                }
            }
        }
        st << static_cast<UInt8>(passBit);
        st <<Stream::eos;
        _owner->send(st);
    }
}


void Dreamer::SendAutoInfo()
{
    // 发送自动探索的信息
    if (_isInAuto)
    {
        Stream st(REP::AUTOJOBHUNTER);
        st << static_cast<UInt8>(0);
        st << Stream::eos;
        _owner->send(st);
    }
}



void Dreamer::OnAutoCommand(UInt8 type)
{
    // 自动战斗
    switch(type)
    {
        case 0:
            // 开始自动战斗
            OnAutoStart();
            break;
        case 1:
            // 取消自动战斗
            OnAutoStop();
            break;
        case 2:
            // 立即完成自动战斗
            OnAutoFinish();
            break;
        case 0x10:
            // 自动战斗一步
            OnAutoStep();
            break;
        default:
            break;
    }
}

UInt16 Dreamer::GetSpotIdFromGameId(UInt8 id)
{
    // 根据游戏id获取对应的据点id
    std::vector<UInt16> spots;
    Map::GetAllSpot(spots);
    if (!spots.size()) return 0;

    return spots[uRand(spots.size())];
   
}


void Dreamer::AddBossGrid( std::vector<UInt16>& validGrid, std::map<UInt16, UInt8>& neighbourCount)
{
    // 增加一个死角用于出现boss
    for (std::vector<UInt16>::iterator it = validGrid.begin(); it != validGrid.end(); ++ it)
    {
        if (_mapInfo.find(*it) != _mapInfo.end())
            continue;

        if (neighbourCount[*it] == 1)
        {
            _mapInfo.insert(std::make_pair(*it, GridInfo(*it, GRID_BOSS | UNKNOWN_FLAG, neighbourCount[*it])));
            return;
        }
    }
    TRACE_LOG("Dreamer: addBoss error.");
}

void Dreamer::AddLengendGrid()
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

void Dreamer::SelectCaveGrid()
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

void Dreamer::SelectBornGrid()
{
    // 选取一个合适的出生点
    UInt8 rndIndex = _rnd(_mapInfo.size());
    UInt8 index = 0;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if ((index == rndIndex) && ((it->second).gridType & 0x0F) != GRID_BOSS && ((it->second).gridType & 0x0F) != GRID_CAVE )
        {
            // 就选择这个作为出生点了
            (it->second).gridType = GRID_NORMAL | CLEAR_FLAG;
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
            (it->second).gridType = GRID_NORMAL | CLEAR_FLAG;
            _earlyPosX = _posX = (it->second).posX;
            _earlyPosY = _posY = (it->second).posY;
            return;
        }
    }
}

void Dreamer::AddMinTreasureGrid()
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


void Dreamer::OnJumpWhenAuto(UInt16 pos, UInt32 stepCount)
{
    // 自动战斗直接跳到目标格子
    MapInfo::iterator it = _mapInfo.find(pos);
    if (it == _mapInfo.end())
        return;
    (it->second).gridType &= ~UNKNOWN_FLAG;

    _earlyPosX = _posX;
    _earlyPosY = _posY;

    UInt8 x = 0;
    UInt8 y = 0;
    INDEX_TO_POS(pos, x, y);
    _posX = x;
    _posY = y;
    _remainTime += stepCount;

    // 该格子事件未完成
    switch ((it->second).gridType & 0x0F)
    {
        case GRID_NORMAL:
            break;
        case GRID_MONSTER:
            // 普通怪物，可能被偷袭
            {
                OnAttackMonster(pos, true);
            }
#ifdef JOB_HUNTER_DEBUG
            std::cout << "tick monster" << std::endl;
#endif
            break;
        case GRID_BOSS:
            OnAttackMonster(pos, true);
#ifdef JOB_HUNTER_DEBUG
            std::cout << "tick boss" << std::endl;
#endif
            break;
        case GRID_TREASURE:
            OnGetTreasure(true);
#ifdef JOB_HUNTER_DEBUG
            std::cout << "tick treasure" << std::endl;
#endif
            break;
        case GRID_TRAP:
            break;
        case GRID_LENGEND:
            break;
        case GRID_CAVE:
            OnFoundCave(true);
#ifdef JOB_HUNTER_DEBUG
            std::cout << "tick cave" << std::endl;
#endif
            break;
        default:
            break;
    }
#ifdef JOB_HUNTER_DEBUG
    std::cout << "tick gridType = " << (UInt32) (it->second).gridType <<std::endl;
#endif
}

void Dreamer::OnSkipMonster(bool isAuto)
{
    // 玩家选择避开怪物
    if (! (CheckGridType(GRID_MONSTER) || CheckGridType(GRID_BOSS)))
        return;
    _posX = _earlyPosX;
    _posY = _earlyPosY;
    SendGridInfo(POS_TO_INDEX(_posX, _posY));
}

bool Dreamer::OnAttackMonster(UInt16 pos, bool isAuto)
{
    // 攻击怪物
    UInt32 npcId = 0;
    MapInfo::iterator it = _mapInfo.find(pos);
    if (it == _mapInfo.end())
        return false;
    UInt8 type = 0;
    switch((it->second).gridType & 0x0F)
    {
        case GRID_MONSTER:
            npcId = GameAction()->getRandomNormalMonster(_gameProgress);
            type = 10;
            break;
        //case GRID_LENGEND:
            //npcId = GameAction()->getLengendMonster(_gameProgress);
            //break;
        case GRID_BOSS:
            npcId = GameAction()->getBossMonster(_gameProgress);
            type = 12;
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

        if (type == 12)
        {
            for (UInt8 i = 1; i < SLOT_MAX; ++ i)
            {
                struct GData::LootResult lr = {0, 1};
                lr.id = GameAction()->getSpecialItem(_gameProgress, i);
                UInt8 prob = _rnd(100);
                if (prob < _spItemRate[i])
                {
                    if (_owner->GetPackage()->Add(lr.id, lr.count, true, true, FromNpc))
                    {
                        _owner->_lastLoot.push_back(lr);
                    }
                }
            }
        }
    }
    else
    {
        // 被怪物打败
        _posX = _earlyPosX;
        _posY = _earlyPosY;
        ++ type;
        _isInAuto = false;
        _isAutoLose = true;
        GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(_owner, EVENT_JOBHUNTER, _owner->getId());
        if (ev)
            ev->release();
        SendMapInfo();
    }

    if (!isAuto)
    {
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
        SendGridInfo(POS_TO_INDEX(_posX, _posY));
    }
    else
    {
        Stream st(REP::AUTOJOBHUNTER);
        st << type;
        st << static_cast<UInt32>(npcId);
        UInt8 sz = _owner->_lastLoot.size();
        st << sz;
        for(UInt8 i = 0; i < sz; ++ i)
        {
            st << _owner->_lastLoot[i].id << _owner->_lastLoot[i].count;
        }
        st << Stream::eos;
        _owner->send(st);
        _owner->checkLastBattled();
    }
    return res;
}
bool Dreamer::OnFoundCave(bool isAuto)
{
    // 找到洞穴
    if (!CheckGridType(GRID_CAVE))
        return false;
    UInt16 fighterId = GameAction()->foundCave(_gameProgress);
    Fighter * fighter = globalFighters[fighterId];
    UInt32 npcId = 0;
    if (!fighter)
        return false;
    if (fighter->getSex())
    {
        npcId = 9601;
    }
    else
    {
        npcId = 9600;
    }
    if (_owner->hasFighter(fighterId))
        fighterId = 0;

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

        npcId = GameAction()->getBossMonster(_gameProgress);
        npcIt = GData::npcGroups.find(npcId);
        if(npcIt == GData::npcGroups.end())
            return false;

        ng = npcIt->second;
        ng->getLoots(_owner, _owner->_lastExJobAward, 0, NULL, true);
        _owner->udpLog("jobHunter", "F_1162", "", "", "", "", "act");

    }
    else
    {
        // 被怪物打败
        _posX = _earlyPosX;
        _posY = _earlyPosY;
        _isInAuto = false;
        _isAutoLose = true;
        GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(_owner, EVENT_JOBHUNTER, _owner->getId());
        if (ev)
            ev->release();
        SendMapInfo();

    }

    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(_owner, lastExp) << static_cast<UInt8>(0);
    st << static_cast<UInt8>(0);
    st.append(&packet[8], packet.size() - 8);
    st << Stream::eos;

    if (!isAuto)
        _owner->send(st);


    Stream st2;
    if (!isAuto)
    {
        st2.init(REP::JOBHUNTER);
        st2 << static_cast<UInt8>(2);
    }
    else
    {
        st2.init(REP::AUTOJOBHUNTER);
        st2 << static_cast<UInt8>(15);
        st2 << static_cast<UInt8>(res?1:0);
        if (!res)
        {
            st2 << Stream::eos;
            _owner->send(st2);
            return true;
        }
    }
    st2 << static_cast<UInt16>(fighterId);

    UInt8 sz = _owner->_lastExJobAward.size();
    st2 << sz;
    for(UInt8 i = 0; i < sz; ++ i)
    {
        st2 << static_cast<UInt16>(_owner->_lastExJobAward[i].id) << static_cast<UInt16>(_owner->_lastExJobAward[i].count);
    }

    // 步数奖励配置
    if (res)
    {
        Table rewards = GameAction()->getStepAward(_remainTime);
        UInt8 count = rewards.size();
        st2 << static_cast<UInt8>(count);
        for (UInt8 i = 0; i < count; ++i)
        {
            Table item = rewards.get<Table>(i + 1);
            if (item.size() < 3) break;
            UInt32 itemId = item.get<UInt32>(1);
            UInt32 itemCount = item.get<UInt32>(2);
            //bool   bind = item.get<bool>(3);
            st2 << static_cast<UInt16>(itemId) << static_cast<UInt16>(itemCount);
            if (!isAuto)
                _owner->lastExJobAwardPush(itemId, itemCount);
            _owner->GetPackage()->Add(itemId, itemCount, true, isAuto? false:true, FromNpc);
        }
    }
    else
        st2 << static_cast<UInt8>(0);
    st2 << Stream::eos;
    if (res)
        _owner->send(st2);

    if (res)
        AddToFighterList(static_cast<UInt16>(fighterId));

    if (!isAuto)
        SendGridInfo(POS_TO_INDEX(_posX, _posY));
    return res;
}

void Dreamer::OnAbort(bool isAuto /* = false */)
{
    // 主动放弃
    _gameProgress = PROGRESS_NONE;
    _remainTime = 0;
    _posX = _posY = 0;
    _earlyPosX = _earlyPosY = 0;
    _mapInfo.clear();
    _isInGame = false;
    _spotId = 0;
    _isInAuto = false;
    _owner->SetVar(VAR_JOB_HUNTER_SPOT_ID, _spotId);
    _nextMoveTime = TimeUtil::Now();
    DB2().PushUpdateData("UPDATE `job_hunter` SET `progress` = '%u', `mapInfo` = '', `posX` = %u, `posY` = %u, `earlyPosX` = %u, `earlyPosY` = %u, `stepCount` = %u  WHERE `playerId` = %"I64_FMT"u", 
            _gameProgress, _posX, _posY, _earlyPosX, _earlyPosY, _remainTime, _owner->getId());
    SendGameInfo(2);
}

void Dreamer::OnLeaveGame(UInt16 spotId)
{
    // 判断是否离开据点
    if (_isInGame && spotId == _spotId)
    {
        SendGameInfo(2);
    }
    OnAutoStop();
}

void Dreamer::OnAutoStart()
{
    // 开始自动战斗
    if (_owner->getVipLevel() < 4)
    {
        return;
    }
    if (1000 > _owner->getTael())
    {
        _owner->sendMsgCode(0, 1100);
        return;
    }

    ConsumeInfo ci(AutoDreamer,0,0);
    _owner->useTael(1000, &ci);

    EventAutoDreamer* event = new (std::nothrow) EventAutoDreamer(_owner, 20, DREAMER_MAX_GRID, _gameProgress);
    if (!event) 
    {
        return;
    }
    PushTimerEvent(event);
    Stream st(REP::AUTOJOBHUNTER);
    st << static_cast<UInt8>(0);
    st << Stream::eos;
    _owner->send(st);
    _isInAuto = true;
    _isAutoLose = false;
}

void Dreamer::OnAutoStep()
{
    // 自动战斗中单步操作
    if (_isInAuto == false)
        return;
    if (_isAutoLose == true)
        return;
    if (_owner->GetPackage()->GetRestPackageSize() < 3)
    {
        Stream st(REP::AUTOJOBHUNTER);
        st << static_cast<UInt8>(5);
        st << Stream::eos;
        _owner->send(st);
        _isInAuto = false;
        SendMapInfo();
        return;
    }
    UInt16 nextPos = GetPossibleGrid();
    if (nextPos == 0xffff)
    {
#ifdef JOB_HUNTER_DEBUG
        std::cout << "tick0" << std::endl;
#endif
        return;
    }
    UInt8 x = 0;
    UInt8 y = 0;
    INDEX_TO_POS(nextPos, x, y);
    UInt32 steps = 0;
    if (x > _posX)
        steps += x - _posX;
    else
        steps += _posX - x;
    if (y > _posY)
        steps += y - _posY;
    else
        steps += _posY - y;
    OnJumpWhenAuto(nextPos, steps);
    if (CheckEnd())
    {
        OnAbort(true);
        GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(_owner, EVENT_JOBHUNTER, _owner->getId());
        if (ev)
            ev->release();
        Stream st(REP::AUTOJOBHUNTER);
        st << static_cast<UInt8>(4);
        st << Stream::eos;
        _owner->send(st);
        _owner->checkLastExJobAward();
    }
    _owner->checkLastExJobAward();
    DumpMapData();
}

void Dreamer::OnAutoStop()
{
    // 停止自动战斗
	GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(_owner, EVENT_JOBHUNTER, _owner->getId());
	if(ev != NULL)
        ev->release();
    if (_isInAuto)
    {
        Stream st(REP::AUTOJOBHUNTER);
        st << static_cast<UInt8>(1);
        st << Stream::eos;
        _owner->send(st);
        _isInAuto = false;
        SendMapInfo();
    }
}

void Dreamer::OnAutoFinish()
{
    // 立即完成自动战斗
    if (10 > _owner->getGoldOrCoupon())
    {
        _owner->sendMsgCode(0, 1101);
        return;
    }

    ConsumeInfo ci(AutoDreamerComplete,0,0);
    _owner->useGoldOrCoupon(10, &ci);

    for (UInt8 i = 0; i < DREAMER_MAX_GRID; ++i)
    {
        OnAutoStep();
        if (!_isInAuto)
            break;
    }
    GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(_owner, EVENT_JOBHUNTER, _owner->getId());
    if (ev)
        ev->release();
}

UInt16 Dreamer::GetPossibleGrid()
{
    // 寻找可能的格子
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if ((it->second).gridType & CLEAR_FLAG)
            continue;
        UInt8 x = 0;
        UInt8 y = 0;
        INDEX_TO_POS(it->first, x, y);
        if (x)
        {
            UInt16 pos = POS_TO_INDEX(x - 1, y);
            MapInfo::iterator it2 = _mapInfo.find(pos);
            if (it2 != _mapInfo.end() && ((it2->second).gridType & CLEAR_FLAG))
            {
#ifdef JOB_HUNTER_DEBUG
                std::cout << "x = " << (UInt32) (x) << ". y = " << (UInt32) (y) << "." << std::endl;
                std::cout << "Grid type = " << (UInt32) (it->second).gridType << "." << std::endl;
#endif
                return it->first;
            }
        }
        if (y)
        {
            UInt16 pos = POS_TO_INDEX(x, y - 1);
            MapInfo::iterator it2 = _mapInfo.find(pos);
            if (it2 != _mapInfo.end() && ((it2->second).gridType & CLEAR_FLAG))
            {
#ifdef JOB_HUNTER_DEBUG
                std::cout << "x = " << (UInt32) (x) << ". y = " << (UInt32) (y) << "." << std::endl;
                std::cout << "Grid type = " << (UInt32) (it->second).gridType << "." << std::endl;
#endif
                return it->first;
            }
        }
        if ((x + 1) < _maxX)
        {
            UInt16 pos = POS_TO_INDEX(x + 1, y);
            MapInfo::iterator it2 = _mapInfo.find(pos);
            if (it2 != _mapInfo.end() && ((it2->second).gridType & CLEAR_FLAG))
            {
#ifdef JOB_HUNTER_DEBUG
                std::cout << "x = " << (UInt32) (x) << ". y = " << (UInt32) (y) << "." << std::endl;
                std::cout << "Grid type = " << (UInt32) (it->second).gridType << "." << std::endl;
#endif
                return it->first;
            }
        }
        if ((y + 1) < _maxY)
        {
            UInt16 pos = POS_TO_INDEX(x, y + 1);
            MapInfo::iterator it2 = _mapInfo.find(pos);
            if (it2 != _mapInfo.end() && ((it2->second).gridType & CLEAR_FLAG))
            {
#ifdef JOB_HUNTER_DEBUG
                std::cout << "x = " << (UInt32) (x) << ". y = " << (UInt32) (y) << "." << std::endl;
                std::cout << "Grid type = " << (UInt32) (it->second).gridType << "." << std::endl;
#endif
                return it->first;
            }
        }
    }
    return 0xffff;
}

bool Dreamer::CheckEnd()
{
    // 检查是否探索全部完成
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++it)
    {
        if (!((it->second).gridType & CLEAR_FLAG))
            return false;
    }
    _owner->OnShuoShuo(SS_MO_COPY);
    _owner->udpLog("jobHunter", "F_1163", "", "", "", "", "act");
    if (!_owner->GetShuoShuo()->getShuoShuo(SS_MO_COPY))
        _owner->OnShuoShuo(SS_MO_COPY);
    return true;
}
#endif
} 


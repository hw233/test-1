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
#include "TeamCopy.h"
#include "ShuoShuo.h"
#include "Server/SysMsg.h"

//************************************************
// 用于第四职业招募的——寻墨
//************************************************

namespace GObject
{

JobHunter::JobHunter(Player * player)
    : _owner(player), _spotId(0), _slot1(0), _slot2(0), _slot3(0), _strengthPoint(0),
      _isInGame(false), _gameProgress(PROGRESS_NONE), 
      _posX(0), _posY(0), _earlyPosX(0), _earlyPosY(0), _stepCount(0), _isInAuto(false), _isAutoLose(false)
{
    _nextMoveTime = TimeUtil::Now();
    DB2().PushUpdateData("INSERT IGNORE INTO `job_hunter` (`playerId`) VALUES (%"I64_FMT"u)", player->getId()); 
}

JobHunter::JobHunter(Player * player, std::string& fighterList, std::string& mapInfo, UInt8 progress,
        UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt32 stepCount)
    : _owner(player), _spotId(0), _gameProgress(progress), _posX(posX), _posY(posY), _earlyPosX(earlyPosX), _earlyPosY(earlyPosY), _stepCount(stepCount),
      _isInAuto(false), _isAutoLose(false)
{
    // 从数据库加载时调用到的构造函数
    LoadFighterList(fighterList);
    LoadMapInfo(mapInfo);
    _nextMoveTime = TimeUtil::Now();
    if (_gameProgress != PROGRESS_NONE)
        _spotId = _owner->GetVar(VAR_JOB_HUNTER_SPOT_ID);
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
            mapString.c_str(), _posX, _posY, _earlyPosX, _earlyPosY, _stepCount, _owner->getId());
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

void JobHunter::OnRequestStart(UInt8 index)
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
    //OnUpdateSlot(true);
    InitMap();
    _spotId = GetSpotIdFromGameId(index);
    _owner->SetVar(VAR_JOB_HUNTER_SPOT_ID, _spotId);
    SendGameInfo(2);
    //_owner->moveTo(_spotId, true);
    DB2().PushUpdateData("UPDATE `job_hunter` SET `progress` = '%u' WHERE `playerId` = %"I64_FMT"u", _gameProgress, _owner->getId());
    _owner->udpLog("jobHunter", "F_1161", "", "", "", "", "act");
}

void JobHunter::OnUpdateSlot(bool isAuto)
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

bool JobHunter::IsInGame()
{
    return _isInGame;
}

bool JobHunter::IsInAuto()
{
    return false;
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
        st << static_cast<UInt16>(_spotId);              // 寻墨需要所在的据点
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
    // 发送自动探索的信息
    if (_isInAuto)
    {
        Stream st(REP::AUTOJOBHUNTER);
        st << static_cast<UInt8>(0);
        st << Stream::eos;
        _owner->send(st);
    }
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

void JobHunter::OnCommand(UInt8 command, UInt8 val, UInt8 val2, bool isAuto /* = false */)
{
    // 收到客户端发来请求处理游戏操作
    /*
    UInt32 now = TimeUtil::Now();
    if (val && now < _nextMoveTime)
    {
        _owner->sendMsgCode(0, 2203, _nextMoveTime - now);
        return;
    }
    */
    if (command == 0)
    {
        // 刷新地图信息
        if (!_isInGame && PLAYER_DATA(_owner, location) == _spotId)
        {
            _isInGame = true;
            SendGameInfo(2);
        }
        DumpMapData();
        SendMapInfo();
        return;
    }
    if (PLAYER_DATA(_owner, location) != _spotId)
    {
        _owner->sendMsgCode(0, 2208);
        return;
    }
            
    switch (command)
    {
        case 1:
            // 移动至某一格
            OnMove(CLIENT_POS_TO_POS(val), isAuto);
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
                            OnAttackMonster(POS_TO_INDEX(_posX, _posY), isAuto);
                        else
                            OnSkipMonster(isAuto);
                        break;
                    case GRID_TREASURE:
                        if (!val)
                            OnGetTreasure(isAuto);
                        break;
                    case GRID_CAVE:
                        if(!val)
                            OnFoundCave(isAuto);
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
            // 发送通关道具进入背包信息
            _owner->checkLastExJobAward();
            return;
            break;
        default:
            break;
    }

    DumpMapData();

    if (CheckEnd())
    {
        OnAbort(isAuto);
    }
    else
        SaveMapInfo();
}

void JobHunter::OnAutoCommand(UInt8 type)
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

UInt16 JobHunter::GetSpotIdFromGameId(UInt8 id)
{
    // 根据游戏id获取对应的据点id
    std::vector<UInt16> spots;
    Map::GetAllSpot(spots);
    if (!spots.size()) return 0;

    return spots[uRand(spots.size())];
   
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

void JobHunter::OnMove(UInt16 pos, bool isAuto)
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
                    if (prob < 50 && (it->second).gridType & UNKNOWN_FLAG)
                    {
                        // 被偷袭直接开打
                        //bool res = OnAttackMonster(pos);
                        _owner->sendMsgCode(0, 2205, 0);
                        OnAttackMonster(pos, isAuto);
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

void JobHunter::OnJumpWhenAuto(UInt16 pos, UInt32 stepCount)
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
    _stepCount += stepCount;

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
    if (CheckEnd())
    {
        Stream st(REP::AUTOJOBHUNTER);
        st << static_cast<UInt8>(4);
        st << Stream::eos;
        _owner->send(st);
    }
}

void JobHunter::OnSkipMonster(bool isAuto)
{
    // 玩家选择避开怪物
    if (! (CheckGridType(GRID_MONSTER) || CheckGridType(GRID_BOSS)))
        return;
    _posX = _earlyPosX;
    _posY = _earlyPosY;
    SendGridInfo(POS_TO_INDEX(_posX, _posY));
}

bool JobHunter::OnAttackMonster(UInt16 pos, bool isAuto)
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
    }
    else
    {
        // 被怪物打败
        _posX = _earlyPosX;
        _posY = _earlyPosY;
        -- type;
        _isInAuto = false;
        _isAutoLose = true;
        GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(_owner, EVENT_JOBHUNTER, _owner->getId());
        if (ev)
            ev->release();
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

void JobHunter::OnSolveTrap(bool isAuto)
{
    // 解锁机关 (已删除功能)
    if (!CheckGridType(GRID_TRAP))
        return;
    _nextMoveTime = TimeUtil::Now() + 30;
    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
    (it->second).gridType = GRID_NORMAL;
    SendGridInfo(POS_TO_INDEX(_posX, _posY));
}

void JobHunter::OnBreakthroughTrap(bool isAuto)
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

void JobHunter::OnGetTreasure(bool isAuto)
{
    // 打开宝箱
    if (!CheckGridType(GRID_TREASURE))
        return;
    Table rewards = GameAction()->getTreasure(_gameProgress);
    Stream st;
    if (!isAuto)
    {
        st.init(REP::JOBHUNTER);
        st << static_cast<UInt8>(3);
    }
    else
    {
        st.init(REP::AUTOJOBHUNTER);
        st << static_cast<UInt8>(14);
    }
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

    if (!isAuto)
        SendGridInfo(POS_TO_INDEX(_posX, _posY));
}

bool JobHunter::OnFoundCave(bool isAuto)
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
        npcId = 9600;
    }
    else
    {
        npcId = 9601;
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
    Table rewards = GameAction()->getStepAward(_stepCount);
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
        _owner->lastExJobAwardPush(itemId, itemCount);
        _owner->GetPackage()->Add(itemId, itemCount, true, true, FromNpc);
    }
    st2 << Stream::eos;
    _owner->send(st2);

    AddToFighterList(static_cast<UInt16>(fighterId));
    if (!isAuto)
        SendGridInfo(POS_TO_INDEX(_posX, _posY));
    return res;
}

void JobHunter::OnAbort(bool isAuto /* = false */)
{
    // 主动放弃
    _gameProgress = PROGRESS_NONE;
    _stepCount = 0;
    _posX = _posY = 0;
    _earlyPosX = _earlyPosY = 0;
    _mapInfo.clear();
    _isInGame = false;
    _spotId = 0;
    _isInAuto = false;
    _owner->SetVar(VAR_JOB_HUNTER_SPOT_ID, _spotId);
    _nextMoveTime = TimeUtil::Now();
    DB2().PushUpdateData("UPDATE `job_hunter` SET `progress` = '%u', `mapInfo` = '', `posX` = %u, `posY` = %u, `earlyPosX` = %u, `earlyPosY` = %u, `stepCount` = %u  WHERE `playerId` = %"I64_FMT"u", 
            _gameProgress, _posX, _posY, _earlyPosX, _earlyPosY, _stepCount, _owner->getId());
    SendGameInfo(2);
}

void JobHunter::OnLeaveGame(UInt16 spotId)
{
    // 判断是否离开据点
    if (_isInGame && spotId == _spotId)
    {
        //_isInGame = false;
        SendGameInfo(2);
    }
    OnAutoStop();
}

void JobHunter::OnAutoStart()
{
    // 开始自动战斗
    if (10 > _owner->getGoldOrCoupon())
    {
        _owner->sendMsgCode(0, 1101);
        return;
    }

    ConsumeInfo ci(AutoJobHunter,0,0);
    _owner->useGoldOrCoupon(10, &ci);

    EventAutoJobHunter* event = new (std::nothrow) EventAutoJobHunter(_owner, 3, MAX_GRID, _gameProgress);
    if (!event) 
    {
#ifdef JOB_HUNTER_DEBUG
        std::cout << "create error." << std::endl;
#endif
        return;
    }
    PushTimerEvent(event);
#ifdef JOB_HUNTER_DEBUG
    std::cout << "create success." << std::endl;
#endif
    Stream st(REP::AUTOJOBHUNTER);
    st << static_cast<UInt8>(0);
    st << Stream::eos;
    _owner->send(st);
    _isInAuto = true;
    _isAutoLose = false;
}

void JobHunter::OnAutoStep()
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
    }
    DumpMapData();
}

void JobHunter::OnAutoStop()
{
    // 停止自动战斗
	GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(_owner, EVENT_JOBHUNTER, _owner->getId());
	if(ev == NULL)
		return;
	ev->release();
    Stream st(REP::AUTOJOBHUNTER);
    st << static_cast<UInt8>(1);
    st << Stream::eos;
    _owner->send(st);
    _isInAuto = false;
    SendMapInfo();
}

void JobHunter::OnAutoFinish()
{
    // TODO: 立即完成自动战斗
    if (10 > _owner->getGoldOrCoupon())
    {
        _owner->sendMsgCode(0, 1101);
        return;
    }

    ConsumeInfo ci(AutoJobHunterComplete,0,0);
    _owner->useGoldOrCoupon(20, &ci);

    for (UInt8 i = 0; i < MAX_GRID; ++i)
    {
        OnAutoStep();
        if (_isInAuto)
            break;
    }
    GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(_owner, EVENT_JOBHUNTER, _owner->getId());
    if (ev)
        ev->release();
}

UInt16 JobHunter::GetPossibleGrid()
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
        if ((x + 1) < MAX_POS_X)
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
        if ((y + 1) < MAX_POS_Y)
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

bool JobHunter::CheckEnd()
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


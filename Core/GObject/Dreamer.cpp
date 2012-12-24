//************************************************
// 用于水晶梦境
//************************************************
#include "Dreamer.h"
#include "Player.h"
#include "Package.h"
#include "MsgID.h"
#include "Country.h"
#include "Script/GameActionLua.h"
#include "GData/DreamerTable.h"

#include <math.h>

namespace GObject
{

Dreamer::Dreamer(Player * player)
    :_owner(player), _gameProgress(0), _gameLevel(0), _isInGame(false),
    _maxX(0), _maxY(0), _maxGrid(0), 
    _posX(0), _posY(0), _earlyPosX(0), _earlyPosY(0),
    _timeConsume(0), _remainTime(50), _keysCount(0)
{
#ifdef DREAMER_DEBUG
    _gameProgress = PROGRESS_70;
    for (UInt8 i = 1; i < MAX_LEVEL + 1; ++i)
    {
        _gameLevel = i;
        if(!InitMap (i))
        {
            printf ("InitMap error.\n");
        }
        _mapInfo.clear();
    }
#endif
    DB2().PushUpdateData("INSERT IGNORE INTO `dreamer` (`playerId`) VALUES (%"I64_FMT"u)", player->getId()); 
}

Dreamer::Dreamer(Player * player, UInt8 progress, UInt8 level, UInt8 maxX, UInt8 maxY, UInt8 maxGrid, std::string& mapInfo, 
        UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt8 timeConsume, UInt32 remainTime, UInt8 keysCount)
    : _owner(player), _gameProgress(progress), _gameLevel(level), _isInGame(false), _maxX(maxX), _maxY(maxY), _maxGrid(maxGrid), 
    _posX(posX), _posY(posY), _earlyPosX(earlyPosX), _earlyPosY(earlyPosY), _timeConsume(timeConsume), _remainTime(remainTime), _keysCount(keysCount)
{
    // 从数据库加载时调用到的构造函数
    LoadMapInfo(mapInfo);
}



Dreamer::~Dreamer()
{
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

    DB2().PushUpdateData("UPDATE `dreamer` SET `maxX` = '%u', `maxY` = '%u', `maxGrid` = '%u', `mapInfo` = '%s', "\
            "`posX` = '%u', `posY` = '%u', `earlyPosX` = '%u', `earlyPosY` = '%u', `remainTime` = '%u', `keys` = '%u' WHERE `playerId` = %"I64_FMT"u", 
            _maxX, _maxY, _maxGrid, mapString.c_str(), _posX, _posY, _earlyPosX, _earlyPosY, _remainTime, _keysCount, _owner->getId());
}

void Dreamer::OnCommand(UInt8 command, UInt8 val /* = 0 */)
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
                    SendGridInfo(_posX, _posY);
                    return;
                }
                switch ((it->second).gridType & 0x0F)
                {
                    case GRID_WAVE      :
                        OnStepIntoWave();
                        (it->second).gridType |= CLEAR_FLAG;
                        break;
                    case GRID_KEY       :
                        OnGetKey();
                        (it->second).gridType |= CLEAR_FLAG;
                        break;
                    case GRID_TREASURE  :
                        OnGetTreasure();
                        break;
                    case GRID_EYE       :
                        break;
                    case GRID_ITEM      :
                        OnGetItem();
                        (it->second).gridType |= CLEAR_FLAG;
                        break;
                    case GRID_WHIRLWIND :
                        OnSufferWhirlwind();
                        (it->second).gridType |= CLEAR_FLAG;
                        break;
                    case GRID_TIME      :
                        OnGetTime();
                        (it->second).gridType |= CLEAR_FLAG;
                        break;
                    case GRID_ARROW     :
                        break;
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
    SaveMapInfo();
    SendMapInfo();
    DumpMapData();
}

bool Dreamer::InitMap(UInt8 level)
{
    // 初始化地图信息
    if (level > MAX_LEVEL || level == 0)
    {
        return false;
    }

    const GData::DreamerLevelData& dreamerLevelData = GData::dreamerDataTable[_gameProgress][level];
    _maxX = dreamerLevelData.maxX;
    _maxY = dreamerLevelData.maxY;
    _maxGrid = dreamerLevelData.maxGrid;
    std::map<UInt16, UInt8> typeMap = dreamerLevelData.gridTypeCount;        // 可用的地图类型
    _timeConsume = dreamerLevelData.timeConsume;        // 该层每步消耗的时间

    std::vector<UInt16> validGrid; // 可选格子
    std::set<UInt16> invalidGrid;  // 暂时无法放入地图的格子
    std::map<UInt16, UInt8> neighbourCount; // 相邻的已经有的格子数
    for (UInt8 x = 0; x < _maxX; ++ x)
    {
        for (UInt8 y = 0; y < _maxY; ++ y)
        {
            invalidGrid.insert(POS_TO_INDEX(x,y));
        }
    }


    UInt16 curPos = POS_TO_INDEX(_rnd(_maxX), _rnd(_maxY));
    
    validGrid.push_back(curPos);
    UInt16 index = 0;
    invalidGrid.erase(curPos);

    for (UInt8 i = 0; i < _maxGrid; ++ i)
    {
        // 选择N个点作为地图中可行进点
        UInt8  type = GRID_ARROW;
        type = GRID_ARROW;
        UInt8 tmpIndex = _rnd(typeMap.size());
        UInt8 tmp = 0;
        for (std::map<UInt16, UInt8>::iterator tmpIt = typeMap.begin(); tmpIt != typeMap.end(); ++ tmpIt)
        {
            if (tmpIndex == tmp)
            {
                if (!tmpIt->second)
                {
                    typeMap.erase(tmpIt);
                    break;
                }
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
            break;
        }
        index = _rnd(validGrid.size());
        curPos = validGrid[index];

    }

    if(!SelectBornGrid())
        return false;
    if (!InitArrow())
        return false;
    if (!InitItem())
        return false;
    SaveMapInfo();
    DumpMapData();
    return true;
}

bool Dreamer::InitArrow()
{
    // 初始化箭头方向和类型（对玩家来说一定很坑爹）
    std::vector<MapInfo::iterator> arrow;
    std::vector<MapInfo::iterator> notArrow;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++it)
    {
        if (((it->second.gridType)&0x000F) == GRID_ARROW)
        {
            arrow.push_back(it);
        }
        else
        {
            notArrow.push_back(it);
        }
    }
    for (std::vector<MapInfo::iterator>::iterator it = arrow.begin(); it != arrow.end(); ++it)
    {
        UInt16 srcPos = (*it)->first;
        MapInfo::iterator it2 = notArrow[_rnd(notArrow.size())];
        UInt16 dstPos = it2->first;
        (*it)->second.gridType = ((CalcArrowType(srcPos, dstPos) | ((it2->second.gridType) & 0x000F)) << 8) 
            | CLEAR_FLAG | GRID_ARROW;
    }
    return true;
}

bool Dreamer::InitItem()
{
    // 初始化宝箱种类
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if (((it->second).gridType & 0x000F) == GRID_ITEM)
        {
            UInt8 prob = _rnd(100);
            UInt8 type = 0;
            if (prob < 40)
                type = 1;
            else if (prob < 80)
                type = 2;
            else
                type = 3;

            (it->second).gridType = GRID_ITEM | UNKNOWN_FLAG | ((UInt16)type << 8);
        }
    }

    return true;
}

bool Dreamer::InitEye()
{
    // TODO: 初始化梦境之眼的指示类型
    return true;
}

bool Dreamer::SelectBornGrid()
{
    // 选取一个合适的出生点
    UInt8 count = 0;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if (((it->second).gridType & 0x000F) == GRID_ARROW)
        {
            ++count;
        }
    }

    if (!count)
    {
        TRACE_LOG("Dreamer: select born error1.");
        return false;
    }

    // 重复重新，选取一个
    UInt8 rndIndex = _rnd(count);
    UInt8 index = 0;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++ it)
    {
        if (((it->second).gridType & 0x000F) == GRID_ARROW && index ++ == rndIndex)
        {
            // 就选择这个作为出生点了
            (it->second).gridType |= CLEAR_FLAG;
            _earlyPosX = _posX = (it->second).posX;
            _earlyPosY = _posY = (it->second).posY;
            return true;
        }
    }
    TRACE_LOG("Dreamer: select born error2.");
    return false;
}

void Dreamer::OnRequestStart(UInt8 progress)
{
    // 玩家请求开始梦境
    if (!progress || progress >= PROGRESS_MAX)
    {
        _owner->sendMsgCode(0, 2221, 0);
        return;
    }

    if ( _gameProgress != PROGRESS_NONE)
    {
        _owner->sendMsgCode(0, 2226, 0);
        return;
    }
    
    if (!_owner->GetPackage()->GetItemAnyNum(DREAMER_ITEM[progress]))
    {
        _owner->sendMsgCode(0, 2224, progress); // 墨家徽记不足
        return;
    }
    _gameProgress = progress;
    _gameLevel = 1;
    _keysCount = 0;
    DB2().PushUpdateData("UPDATE `dreamer` SET `progress` = '%u' `level` = '%u', `keys` = '%u' WHERE `playerId` = %"I64_FMT"u", _gameProgress, _gameLevel, _owner->getId(), _keysCount);
    if (!InitMap(_gameLevel))
    {
        OnAbort();
    }
    _owner->GetPackage()->DelItemAny(DREAMER_ITEM[progress], 1);
    SendGameInfo();
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
        switch ((it->second).gridType & 0x000F)
        {
            case GRID_WAVE      :
                break;
            case GRID_KEY       :
                break;
            case GRID_TREASURE  :
                break;
            case GRID_EYE       :
                break;
            case GRID_ITEM      :
                break;
            case GRID_WHIRLWIND :
                break;
            case GRID_TIME      :
                break;
            case GRID_ARROW     :
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

    SendGridInfo(x, y);
}

void Dreamer::OnAbort()
{
    // 主动放弃
    _gameProgress = PROGRESS_NONE;
    _gameLevel = LEVEL_NONE;
    _maxX = _maxY = 0;
    _maxGrid = 0;
    _remainTime = 0;
    _posX = _posY = 0;
    _earlyPosX = _earlyPosY = 0;
    _mapInfo.clear();
    _isInGame = false;
    DB2().PushUpdateData("UPDATE `dreamer` SET `progress` = '%u', `level` = '%u', `maxX` = '%u',  `maxY` = '%u', `maxGrid` = '%u', `mapInfo` = '',"\
            "`posX` = %u, `posY` = %u, `earlyPosX` = %u, `earlyPosY` = %u, `remainTime` = %u  WHERE `playerId` = %"I64_FMT"u", 
            _gameProgress, _gameLevel, _maxX, _maxY, _maxGrid, _posX, _posY, _earlyPosX, _earlyPosY, _remainTime, _owner->getId());
    SendGameInfo();
}

void Dreamer::OnStepIntoWave()
{
    // 进入漩涡，开始下一层
    if (CheckGridType(GRID_WAVE))
    {
        TRACE_LOG("Dream: wave error.");
        return;
    }
    if (_gameLevel < GData::dreamerDataTable[_gameProgress].size() - 1)
    {
        // 还有下一层
        InitMap(++_gameLevel);
        SendMapInfo();
    }
    else
    {
        // 已经最后一层结束了，Nothing
    }
}

void Dreamer::OnGetTreasure()
{
    // 打开宝箱
    if (!CheckGridType(GRID_TREASURE))
    {
        TRACE_LOG("Dream: treasure error.");
        return;
    }
    if (!_keysCount)
    {
        _owner->sendMsgCode(0, 2222, 0);    // 钥匙不足，无法打开宝箱
        return;
    }
    Table rewards = GameAction()->getDreamerTreasure(_gameProgress);
    Stream st;
    st.init(REP::DREAMER);
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

    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
    (it->second).gridType |= CLEAR_FLAG;
}

void Dreamer::OnGetKey()
{
    // 得到钥匙
    if (CheckGridType(GRID_KEY))
    {
        TRACE_LOG("Dream: key error.");
        return;
    }
    ++_keysCount;
    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
    (it->second).gridType |= CLEAR_FLAG;
}

void Dreamer::OnGetItem()
{
    // 获得道具
    UInt8 index = CheckGridType(GRID_ITEM);
    if (!index)
    {
        TRACE_LOG("Dream: item error.");
        return;
    }
    Table rewards = GameAction()->getDreamerItem(_gameProgress, index);
    Stream st;
    st.init(REP::DREAMER);
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

    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
    (it->second).gridType |= CLEAR_FLAG;
}

void Dreamer::OnSufferWhirlwind()
{
    // 遇上飓风，随机到达任何地点
    UInt8 rndIndex = _rnd(_mapInfo.size());
    UInt8 index = 0;
    for (MapInfo::iterator it = _mapInfo.begin(); it != _mapInfo.end(); ++it)
    {
        if (index == rndIndex)
        {
            INDEX_TO_POS(it->first, _posX, _posY);
            (it->second).gridType &= ~UNKNOWN_FLAG;
            return;
        }
        ++index;
    }
    TRACE_LOG("Dreamer: suffer whirlwind error.");
}

void Dreamer::OnGetTime()
{
    // 增加时间
    _remainTime += _timeConsume;
}

void Dreamer::SendGameInfo()
{
    // TODO: 发送梦境游戏的具体内容
    /*
    UInt8 flag = isUpdeated ? 0x10:0;
    if (type == 2)
    {
        Stream st (REP::DREAMER);
        st << static_cast<UInt8> (2);

        st << static_cast<UInt8> (_gameProgress);       // 共鸣地点
        st << static_cast<UInt8> (_isInGame ? 1 : 0);   // 是否已经进入寻墨地图
        st <<Stream::eos;
        _owner->send(st);
    }
    */
}

void Dreamer::SendGridInfo(UInt8 posX, UInt8 posY)
{
    // 发送某一格子的具体信息
    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(posX, posY));
    if (it == _mapInfo.end())
        return;
    Stream st(REP::DREAMER);
    st << static_cast<UInt8>(1);

    st << static_cast<UInt8>(posX);
    st << static_cast<UInt8>(posY);
    st << static_cast<UInt16>((it->second).gridType); 

    st << static_cast<UInt8>(_posX);
    st << static_cast<UInt8>(_posY);

    st << Stream::eos;
    _owner->send(st);
}

void Dreamer::SendMapInfo()
{
    // 发送地图信息
    Stream st(REP::DREAMER);
    st << static_cast<UInt8>(0);

    for (UInt8 y = 0; y < _maxY; ++ y)
    {
        for (UInt8 x = 0; x < _maxX; ++ x)
        {
            MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(x,y));
            if (it != _mapInfo.end())
            {
                st << static_cast<UInt8>(x);
                st << static_cast<UInt8>(y);
                st << static_cast<UInt8>((it->second).gridType);
            }
        }
    }
    st << static_cast<UInt8>(_posX);
    st << static_cast<UInt8>(_posY);
    st << Stream::eos;
    _owner->send(st);
}

UInt8 Dreamer::CheckGridType(UInt8 type)
{
    // 检查该坐标是否和需要的类型匹配
    MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
    if (it != _mapInfo.end())
    {
        if ((((it->second).gridType)&0x00FF) != type)
            return 0;
        else
        {
            switch ((it->second).gridType & 0x000F)
            {
                case GRID_ARROW     :
                case GRID_TIME      :
                case GRID_EYE       :
                    return ((it->second).gridType & 0xFF00) >> 8;
                case GRID_TREASURE  :
                case GRID_WAVE      :
                case GRID_KEY       :
                case GRID_ITEM      :
                case GRID_WHIRLWIND :
                default:
                    return 1;
            }
        }
    }
    return 0;
}

UInt8 Dreamer::CalcArrowType(UInt16 srcPos, UInt16 dstPos)
{
    // 返回箭头类型
    UInt8 srcX = 0;
    UInt8 srcY = 0;
    UInt8 dstX = 0;
    UInt8 dstY = 0;
    INDEX_TO_POS(srcPos, srcX, srcY);
    INDEX_TO_POS(srcPos, srcX, srcY);
    Int16 dX = (Int16)dstX - (Int16)srcX;
    Int16 dY = (Int16)dstY - (Int16)srcY;
    if (!dX)
    {
        return dY > 0 ? UP:DOWN;
    }
    if (!dY)
    {
        return dX > 0 ? RIGHT:LEFT;
    }

    float res = atan2(dY, dX);
    res = res * 180 / M_PI;
    if (res <= -157.5 || res >= 157.5)
        return DOWN;
    if (res >= -157.5 && res <= -112.5)
        return LEFT_DOWN;
    if (res >= -112.5 && res <= -67.5)
        return LEFT;
    if (res >= -67.5 && res <= -22.5)
        return LEFT_UP;
    if (res >= -22.5 && res <= 22.5)
        return UP;
    if (res >= 22.5 && res <= 67.5)
        return RIGHT_UP;
    if (res >= 67.5 && res <= 112.5)
        return RIGHT;
    if (res >= 112.5 && res <= 157.5)
        return RIGHT_DOWN;

    return 0;
}

void Dreamer::DumpMapData()
{
#ifdef DREAMER_DEBUG
    // DEBUG版本输出地图信息
    char buffer[1024] = "\nMapInfo:\n    0 1 2 3 4 5 6 7 8 9 A B C D E F\n\n";
    char buffer2[1024] = "\nNeighbourInfo:\n    0 1 2 3 4 5 6 7 8 9 A B C D E F\n\n";
    for (UInt8 y = 0; y < _maxY; ++ y)
    {
        char buf[16];
        snprintf(buf, 16, "%X: ", y);
        strcat (buffer, buf);
        strcat (buffer2, buf);
        for (UInt8 x = 0; x < _maxX; ++x)
        {
            UInt16 index = POS_TO_INDEX(x,y);
            MapInfo::iterator it = _mapInfo.find(index);
            if (it != _mapInfo.end())
            {
                if (x == _posX && y == _posY)
                {
                    snprintf(buf, 16, "*%1X", (UInt32)(0x0F & (it->second).gridType));
                }
                else
                {
                    snprintf(buf, 16, "%2X", (UInt32)(0x0F & (it->second).gridType));
                }
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

} 


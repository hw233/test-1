#ifndef _DREAMER_H_
#define _DREAMER_H_

//************************************************
// 用于水晶梦境
//************************************************

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/URandom.h"

namespace GObject
{

#ifdef _DEBUG
#define DREAMER_DEBUG
#endif

class Player;

#ifndef POS_TO_INDEX
#define POS_TO_INDEX(x,y)  (UInt16)(((x)+((UInt16)(y)<<8)))
#endif

#ifndef INDEX_TO_POS
#define INDEX_TO_POS(index,x,y) \
    x = (index) & 0xff;\
    y = ((index) >> 8) & 0xff;
#endif


#if 0
static UInt8 spotMapType[] =
{
            /*0123456789ABCDEF */
    /*0x00*/ "XXXXXXXXXXXXXXXX"\
    /*0x10*/ "XXXXXXXXXXXXXXXX"\
    /*0x20*/ "XXXXXXXXXXXXXXXX"\
    /*0x30*/ "XXXXXXXXXXXXXXXX"\
    /*0x40*/ "XXXXXXXXXXXXXXXX"\
    /*0x50*/ "XXXXXXXXXXXXXXXX"\
    /*0x60*/ "XXXXXXXXXXXXXXXX"\
    /*0x70*/ "XXXXXXXXXXXXXXXX"\
    /*0x80*/ "XXXXXXXXXXXXXXXX"\
    /*0x90*/ "XXXXXXXXXXXXXXXX"\
    /*0xA0*/ "XXXXXXXXXXXXXXXX"\
    /*0xB0*/ "XXXXXXXXXXXXXXXX"\
    /*0xC0*/ "XXXXXXXXXXXXXXXX"\
    /*0xD0*/ "XXXXXXXXXXXXXXXX"\
    /*0xE0*/ "XXXXXXXXXXXXXXXX"\
    /*0xF0*/ "XXXXXXXXXXXXXXXX"\
            /*0123456789ABCDEF */
};
#endif

static const UInt8 MAX_LEVEL = 6;
static const UInt8 MAX_X[MAX_LEVEL + 1] = 
{
    0, 2, 4, 6, 8, 10, 12
};

static const UInt8 MAX_Y[MAX_LEVEL + 1] = 
{
    0, 2, 4, 6, 8, 10, 12
};

static const UInt8 DREAMER_MAX_GRID[MAX_LEVEL + 1] = 
{
    0, 4, 8, 16, 32, 64, 128
};

class Dreamer
{

#ifndef UNKNOWN_FLAG
#define UNKNOWN_FLAG 0x80
#endif
#ifndef CLEAR_FLAG
#define CLEAR_FLAG 0x40
#endif

    enum GridType
    {
        GRID_NORMAL     = 0x1,    // 普通格子
        GRID_ARROR      = 0x2,    // 箭头方向格子
        GRID_KEY        = 0x3,    // 钥匙格子
        GRID_TREASURE   = 0x4,    // 宝箱格子
        GRID_ITEM       = 0x5,    // 道具格子
        GRID_TRANSPORT  = 0x6,    // 传送点格子
        GRID_NORMAL_MAX,
    };

    enum Progress
    {
        PROGRESS_NONE = 0,  // 没有开始梦境
        PROGRESS_LEVEL1,    // 水晶梦境第一层
        PROGRESS_LEVEL2,    // 水晶梦境第二层
        PROGRESS_LEVEL3,    // 水晶梦境第三层
        PROGRESS_LEVEL4,    // 水晶梦境第四层
        PROGRESS_LEVEL5,    // 水晶梦境第五层
        PROGRESS_LEVEL6,    // 水晶梦境第六层
        PROGRESS_MAX,
    };

    struct GridInfo
    {
        UInt8 neighbCount;  // 已经和该点相邻的格子的个数
        UInt8 posX;         // 在大地图上的X坐标
        UInt8 posY;         // 在大地图上的Y坐标
        UInt8 gridType;     // 格子类型（GridType类型）

        GridInfo(UInt16 index, UInt8 type, UInt8 neighbourCount)
            : neighbCount(neighbourCount)
        {
            INDEX_TO_POS(index, posX, posY);
            gridType = type;
        }
    };


    public:
        Dreamer(Player *player);
        ~Dreamer();

        void OnCommand(UInt8 command, UInt8 val);

        void SendGridInfo(UInt16 pos);
        void SendMapInfo();

    private:
        bool InitMap(UInt8 level);
        void OnMove(UInt16 pos);

    private:
        inline bool CheckGridType(UInt8 type)
        {
            // 检查该坐标是否和需要的类型匹配
            MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
            if (it != _mapInfo.end())
            {
                return ((it->second).gridType == type);
            }
            return false;
        }

    private:
        typedef std::map<UInt16, GridInfo> MapInfo;

        Player * _owner;

        UInt8 _maxX;
        UInt8 _maxY;
        UInt8 _maxGrid;
        

        MapInfo _mapInfo;               // 梦境游戏地图信息

        bool  _isInGame;                // 玩家是否已经进入梦境游戏
        UInt8 _gameProgress;            // 梦境游戏状态

        UInt8 _posX;                    // 玩家在梦境游戏中的X坐标
        UInt8 _posY;                    // 玩家在梦境游戏中的Y坐标
        UInt8 _earlyPosX;
        UInt8 _earlyPosY;

        UInt32 _stepCount;              // 梦境游戏中花费的行动步数

        UInt32 _nextMoveTime;           // 下一次移动的时间

        URandom _rnd;                   // 用于产生随机数
        

    private:
        void DumpMapData();

};
}

#endif


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

static const UInt8 MAX_LEVEL = 5;
static const UInt8 MAX_X[MAX_LEVEL + 1] = 
{
    0, 2, 3, 4, 5, 6
};

static const UInt8 MAX_Y[MAX_LEVEL + 1] = 
{
    0, 2, 3, 4, 5, 6
};

static const UInt8 DREAMER_MAX_GRID[MAX_LEVEL + 1] = 
{
    0, 4, 7, 10, 12, 15
};

static const UInt8 CONSUME_TIME[MAX_LEVEL + 1] = 
{
    0, 6, 5, 4, 3, 2//, 1,
};

class Dreamer
{

#ifndef UNKNOWN_FLAG
#define UNKNOWN_FLAG 0x80
#endif

#define LEFT_UP    0x00  //↖
#define UP         0x10  //↑
#define RIGHT_UP   0x20  //↗
#define RIGHT      0x30  //→
#define RIGHT_DOWN 0x40  //↘
#define DOWN       0x50  //↓
#define LEFT_DOWN  0x60  //↙
#define LEFT       0x70  //←

    enum GridType
    {
        GRID_NORMAL     = 0x1,    // 普通
        GRID_ARROW      = 0x2,    // 箭头方向
        GRID_KEY        = 0x3,    // 钥匙
        GRID_TREASURE   = 0x4,    // 宝箱
        GRID_ITEM       = 0x5,    // 道具
        GRID_TIME       = 0x6,    // 增加剩余时间
        GRID_TRANSPORT  = 0x7,    // 传送点
        GRID_NORMAL_MAX,
    };

    enum DreamType
    {
        TYPE_NULL   = 0,
        TYPE_1      = 1,
        TYPE_2      = 2,
        TYPE_3      = 3,
        TYPE_4      = 4,
        TYPE_MAX,
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
        UInt16 gridType;     // 格子类型（GridType类型）

        GridInfo(UInt16 index, UInt16 type, UInt8 neighbourCount)
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
        bool InitArrow();

        void OnMove(UInt16 pos);
        void OnGetTreasure();

    private:
        UInt8 CheckGridType(UInt8 type)
        {
            // 检查该坐标是否和需要的类型匹配
            MapInfo::iterator it = _mapInfo.find(POS_TO_INDEX(_posX, _posY));
            if (it != _mapInfo.end())
            {
                return ((it->second).gridType == type)? 1:0;
            }
            return 0;
        }

    private:
        typedef std::map<UInt16, GridInfo> MapInfo;

        Player * _owner;

        UInt8 _maxX;
        UInt8 _maxY;
        UInt8 _maxGrid;
        

        MapInfo _mapInfo;               // 梦境游戏地图信息

        bool  _isInGame;                // 玩家是否已经进入梦境游戏
        UInt8 _type;                    // 梦境种类
        UInt8 _gameProgress;            // 梦境游戏状态

        UInt8 _posX;                    // 玩家在梦境游戏中的X坐标
        UInt8 _posY;                    // 玩家在梦境游戏中的Y坐标
        UInt8 _earlyPosX;
        UInt8 _earlyPosY;

        UInt32 _remainTime;             // 梦境游戏中剩余的体力

        URandom _rnd;                   // 用于产生随机数
        

    private:
        void DumpMapData();

};
}

#endif


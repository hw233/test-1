#ifndef _DREAMER_H_
#define _DREAMER_H_

//************************************************
// 用于水晶梦境
//************************************************

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
//#include "Pos.h"
//#include "AStar.h"

namespace GObject
{

#ifdef _DEBUG
//#define DREAMER_DEBUG
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

static const UInt8 MAX_PROGRESS = 4;
static const UInt8 MAX_LEVEL = 5;

static const UInt16 DREAMER_ITEM[MAX_PROGRESS + 1] =
{
    0,
    9290,
    9295,
    9300,
    9305,
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
        GRID_WAVE       = 0x1,    // 漩涡，通往下一层
        GRID_KEY        = 0x2,    // 钥匙
        GRID_TREASURE   = 0x3,    // 宝箱（需要钥匙）
        GRID_EYE        = 0x4,    // 眼位（前往漩涡，钥匙，宝箱三者之一）
        GRID_ITEM       = 0x5,    // 道具（不需要钥匙的宝箱）
        GRID_WHIRLWIND  = 0x6,    // 旋风（随机从送至容易地点）
        GRID_TIME       = 0x7,    // 增加剩余时间
        GRID_ARROW      = 0x8,    // 箭头
        GRID_NORMAL_MAX,
    };

    enum Progress
    {
        PROGRESS_NONE    = 0,
        PROGRESS_70      = 1,    // 70级副本梦境
        PROGRESS_80      = 2,    // 80级副本梦境
        PROGRESS_90      = 3,    // 90级副本梦境
        PROGRESS_100     = 4,    // 100级副本梦境
        PROGRESS_MAX,
    };

    enum Level
    {
        LEVEL_NONE = 0,     // 没有开始梦境
        LEVEL_1,            // 水晶梦境第一层
        LEVEL_2,            // 水晶梦境第二层
        LEVEL_3,            // 水晶梦境第三层
        LEVEL_4,            // 水晶梦境第四层
        LEVEL_5,            // 水晶梦境第五层
        LEVEL_6,            // 水晶梦境第六层
    };

    struct GridInfo
    {
        UInt8 neighbCount;  // 已经和该点相邻的格子的个数
        UInt8 posX;         // 在大地图上的X坐标
        UInt8 posY;         // 在大地图上的Y坐标
        UInt16 gridType;    // 格子类型（GridType类型）

        GridInfo(UInt16 index, UInt16 type, UInt8 neighbourCount)
            : neighbCount(neighbourCount)
        {
            INDEX_TO_POS(index, posX, posY);
            gridType = type;
        }
    };

    friend class DreamerWalker;


    public:
        Dreamer(Player *player);
        Dreamer(Player * player, UInt8 progress, UInt8 level, UInt8 maxX, UInt8 maxY, UInt8 maxGrid, const std::string& mapInfo, 
                UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt8 timeConsume, UInt32 remainTime, UInt8 keysCount, 
                UInt8 eyesCount, UInt8 eyeTime, UInt8 eyeX, UInt8 eyeY);
        ~Dreamer();

        void OnCommand(UInt8 command, UInt8 val, UInt8 val2);

        void SendGameInfo();
        void SendGridInfo(UInt8 posX, UInt8 posY);
        void SendMapInfo(bool isNext = false);
        void SendEyeInfo(UInt8 type);
        void SendTimeOver();
        void SendExploreOver();

        void SetTime(UInt8 count);
        void SetEye(UInt8 count);
        void SetKey(UInt8 count);

    private:
        bool InitMap(UInt8 level);
        bool InitArrow();
        bool InitItem();
        bool InitEye();
        bool SelectBornGrid();
        bool LoadMapInfo(const std::string& list);
        void SaveMapInfo();

        void OnRequestStart(UInt8 progress);
        bool OnMove(UInt8 x, UInt8 y);
        bool OnStepIntoWave();
        bool OnGetTreasure();
        bool OnGetKey();
        bool OnGetItem();
        bool OnSufferWhirlwind();
        bool OnGetTime();
        bool OnGetEye();
        bool OnUseEye(UInt8 type);
        bool OnBuyEye(UInt8 count = 1);
        void OnAbort();

        UInt8 CheckGridType(UInt8 type);
        bool CheckEnd();
        UInt8 CalcArrowType(UInt16 srcPos, UInt16 dstPos);

    private:
        typedef std::map<UInt16, GridInfo> MapInfo;

        Player * _owner;

        MapInfo _mapInfo;               // 梦境游戏地图信息

        UInt8 _gameProgress;            // 梦境游戏状态
        UInt8 _gameLevel;               // 梦境层数
        bool  _isInGame;                // 玩家是否已经进入梦境游戏

        UInt8 _maxX;
        UInt8 _maxY;
        UInt8 _maxGrid;
        
        UInt8 _posX;                    // 玩家在梦境游戏中的X坐标
        UInt8 _posY;                    // 玩家在梦境游戏中的Y坐标
        UInt8 _earlyPosX;
        UInt8 _earlyPosY;

        UInt8  _timeConsume;            // 该层每步消耗的体力
        UInt8  _remainTime;             // 梦境游戏中剩余的体力
        UInt8  _keysCount;              // 手头钥匙的数量

        UInt8  _eyesCount;              // 手头拥有梦境之眼的数量
        UInt8  _eyesTime;               // 梦境之眼的有效时间
        UInt8  _eyeX;
        UInt8  _eyeY;

        URandom _rnd;                   // 用于产生随机数
        

    private:
        void DumpMapData();

};


/*
class DreamerWalker : public AStar <1, 3>
{
    public:
        DreamerWalker(Dreamer::MapInfo mapInfo)
            :_mapInfo(mapInfo);
    public:
        bool moveable(const UInt16 src, const UInt16 dst, UInt8 radius);
    private:
        const Dreamer::MapInfo& _mapInfo;
};

bool DreamerWalker::moveable(const Pos& tempPos, const Pos& dst, int radius) 
{
    UInt16 srcIndex = POS_TO_INDEX(src.x, src.y);
    UInt16 dstIndex = POS_TO_INDEX(dst.x, dst.y);
    return (_mapInfo.find(dstIndex) != _mapInfo.end());
}

bool DreamerWalker::move(int direct, int step = 1)
{
    return true;
}
*/

}

#endif


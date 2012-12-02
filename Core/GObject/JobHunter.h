#ifndef _JOBHUNTER_H_
#define _JOBHUNTER_H_

//************************************************
// 用于第四职业招募的——寻墨
//************************************************

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "GData/NpcGroup.h"

namespace GObject
{
#ifdef  _DEBUG
#define JOB_HUNTER_DEBUG
#endif

class Player;

#define POS_TO_INDEX(x,y)  (UInt16)(((x)+((UInt16)(y)<<8)))

#define INDEX_TO_POS(index,x,y) \
    x = (index) & 0xff;\
    y = ((index) >> 8) & 0xff;

#define CLIENT_POS_TO_POS(x) (UInt16)(((UInt16)((x) / MAX_POS_X) << 8) + ((x) % MAX_POS_X))
#define POS_TO_CLIENT_POS(x) ((((UInt16)(x) >> 8) * MAX_POS_X) + ((x) & 0xff))

#define  EX_JOB_ITEM_ID 9229

static const UInt8 MAX_POS_X = 5;
static const UInt8 MAX_POS_Y = 5;
static const UInt8 MAX_GRID  = 12;

static const UInt8 MAX_GRID_COUNT[256]
{
    0, // INVALID
    0, // NORMAL
    MAX_GRID, // MONSTER
    1, // BOSS
    MAX_GRID, // TREASURE
    2, // TRAP
    0, // NORMAL_MAX

};

static const UInt32 SPOT_ID[5] = {
    0, 122, 23232, 32332, 111
};

static const UInt8 MAP_ID_INDEX[] = {
    0,
    32,
    50,
    41,
    20,
    0
};


class JobHunter
{

#define UNKNOWN_FLAG 0x80
#define CLEAR_FLAG 0x40

    enum GridType
    {
        GRID_INVALID    = 0x0,    // 无法到达的格子
        GRID_NORMAL     = 0x1,    // 普通已探索格子
        GRID_MONSTER    = 0x2,    // 怪物格子
        GRID_BOSS       = 0x3,    // boss格子
        GRID_TREASURE   = 0x4,    // 宝箱格子
        GRID_TRAP       = 0x5,    // 陷阱格子
        GRID_NORMAL_MAX,
        GRID_LENGEND    = 0xA,
        GRID_CAVE       = 0xF, // 目标格子，墨家秘洞存在宝物和散仙（概率存在）
    };

    enum Progress
    {
        PROGRESS_NONE = 0,  // 没有开始寻墨
        PROGRESS_70,        //  70级副本寻墨
        PROGRESS_80,        //  80级副本寻墨
        PROGRESS_90,        //  90级副本寻墨
        PROGRESS_100,       // 100级副本寻墨
        PROGRESS_MAX,
    };

    struct GridInfo
    {
        UInt8 neighbCount;  // 已经和该点相邻的格子的个数
        UInt8 posX;         // 在大地图上的X坐标
        UInt8 posY;         // 在大地图上的Y坐标
        UInt8 gridType;     // 格子类型（GridType类型）

        typedef std::list<UInt16> Route;
        typedef std::map<UInt16, Route> GridRoute;
        GridRoute route;    // 该点与其他连通点的路径

        /*
        GridInfo(UInt8 x, UInt8 y, UInt8 type)
            : neighbCount(0), posX(x), posY(y), gridType(type)
        {
        }
        */
        GridInfo(UInt16 index, UInt8 type, UInt8 neighbourCount)
            : neighbCount(neighbourCount)
        {
            INDEX_TO_POS(index, posX, posY);
            gridType = type;
        }
    };

#if 0
    enum SlotType
    {                           //                         每个  三个相同额外增加
        SLOT_DRAGON     = 1,    // 青龙 （法宝、心法、装备  10%，   20%）
        SLOT_TIGER      = 2,    // 白虎 （法宝              20%，   40%）
        SLOT_PHOENIX    = 3,    // 朱雀 （心法              20%，   40%）
        SLOT_TURTLE     = 4,    // 玄武 （装备              20%，   40%）
        SLOT_MAX,
    };
#endif
    enum SlotType
    {
        SLOT_NONE   = 0,
        SLOT_GOLD   = 1,    // 每点25强度
        SLOT_WOOD   = 2,    // 每点20强度
        SLOT_WATAR  = 3,    // 每点15强度
        //SLOT_FIRE   = 4,    // 每点10强度
        //SLOT_MUD    = 5,    // 每点 5强度
        SLOT_MAX,
    };

    public:
        JobHunter(Player* player);
        JobHunter(Player * player, std::string& fighterList, std::string& mapInfo, UInt8 progress,
                UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt32 stepCount);
        ~JobHunter();

        void LoadFighterList(const std::string& str);
        bool LoadMapInfo (const std::string& str);
        void SaveMapInfo ();


        void AddToFighterList(UInt16 id);
        void SendFighterList();
        void OnHireFighter(UInt16 id);

        void OnRequestStart(UInt8 index);
        void OnUpdateSlot(bool isAuto = false);

        bool IsInGame();
        bool IsInAuto();
        void SendGameInfo(UInt8 type);

        void SendMapInfo();

        void SendAutoInfo();

        void OnCommand(UInt8 command, UInt8 val, UInt8 val2, bool isAuto = false);
        void OnAutoCommand(UInt8 type);

        void SendGridInfo(UInt16 pos);
        void OnAbort(bool isAuto);
        void OnLeaveGame(UInt16 spotId);
        bool CheckEnd();

    private:

        UInt16 GetSpotIdFromGameId(UInt8 id);

        bool InitMap();
        void SelectBossGrid();
        void AddLengendGrid();
        void SelectCaveGrid();
        void SelectBornGrid();
        void AddMinTreasureGrid();

        void OnMove(UInt16 pos, bool isAuto);
        void OnJumpWhenAuto(UInt16 pos, UInt32 stepCount);
        void OnSkipMonster(bool isAuto);
        bool OnAttackMonster(UInt16 pos, bool isAuto);
        void OnSolveTrap(bool isAuto);
        void OnBreakthroughTrap(bool isAuto);
        void OnGetTreasure(bool isAuto);
        bool OnFoundCave(bool isAuto);
        void OnAutoStart();
        void OnAutoStep();
        void OnAutoStop();
        void OnAutoFinish();

        UInt16 GetPossibleGrid();


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
        bool FighterList2String(std::string& str);
        bool MapInfo2String(std::string& str);

    private:

#if 0
        static UInt8 spotMapType[] =
        {
                    /*0123456789ABCDEF */
            /*0x00*/ "OOOOOXXXXXXXXXXX"\
            /*0x10*/ "OOOOOXXXXXXXXXXX"\
            /*0x20*/ "OOOOOXXXXXXXXXXX"\
            /*0x30*/ "OOOOOXXXXXXXXXXX"\
            /*0x40*/ "OOOOOXXXXXXXXXXX"\
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


        typedef std::map<UInt16, GridInfo> MapInfo;
        Player *_owner;
        std::set<UInt16> _fighterList;  // 可招募散仙列表

        MapInfo _mapInfo;               // 寻墨游戏地图信息
        UInt16 _spotId;                 // 寻墨的据点ID

        UInt8 _slot1;                   // 一号神兽感应位
        UInt8 _slot2;                   // 二号神兽感应位
        UInt8 _slot3;                   // 三号神兽感应位

#if 0
        UInt8 _equipProb;               // 装备寻得率
        UInt8 _cittaProb;               // 心法寻得率
        UInt8 _trumpProb;               // 法宝寻得率
#endif
        UInt8 _strengthPoint;           // 神兽强度（为了刷地图上的神兽boss的）

        bool  _isInGame;                // 玩家是否已经进入寻墨游戏
        UInt8 _gameProgress;            // 寻墨游戏状态

        UInt8 _posX;                    // 玩家在寻墨游戏中的X坐标
        UInt8 _posY;                    // 玩家在寻墨游戏中的Y坐标
        UInt8 _earlyPosX;
        UInt8 _earlyPosY;

        UInt32 _stepCount;              // 寻墨游戏中花费的行动步数

        UInt32 _nextMoveTime;           // 下一次移动的时间

        URandom _rnd;                   // 用于产生随机数
        
        bool _isInAuto;
        bool _isAutoLose;               // 自动探索是否失败

    private:
        void DumpMapData();

};

}

#endif


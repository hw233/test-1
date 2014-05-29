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
//#define JOB_HUNTER_DEBUG
#endif

class Player;

#define POS_TO_INDEX(x,y)  (UInt16)(((x)+((UInt16)(y)<<8)))

#define INDEX_TO_POS(index,x,y) \
    x = (index) & 0xff;\
    y = ((index) >> 8) & 0xff;

#define CLIENT_POS_TO_POS(x) (UInt16)(((UInt16)((x) / MAX_POS_X) << 8) + ((x) % MAX_POS_X))
#define POS_TO_CLIENT_POS(x) ((((UInt16)(x) >> 8) * MAX_POS_X) + ((x) & 0xff))

#define  EX_JOB_ITEM_ID 9229
#define  EX_JOB_SLOT_ID 9285

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

static const UInt8 ITEM_RATE[10] = 
{
    0,
    10,
    30,
    100,
    100,
    100,
    100,
    100,
    100,
    100
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
        PROGRESS_110,       // 110级副本寻墨
        PROGRESS_120,       // 120级副本寻墨
        PROGRESS_130,       // 130级副本寻墨
        PROGRESS_140,       // 140级副本寻墨
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

        GridInfo(UInt16 index, UInt8 type, UInt8 neighbourCount)
            : neighbCount(neighbourCount)
        {
            INDEX_TO_POS(index, posX, posY);
            gridType = type;
        }
    };

    enum SlotType
    {                           //     
        SLOT_DRAGON     = 1,    // 青龙
        SLOT_TIGER      = 2,    // 白虎
        SLOT_PHOENIX    = 3,    // 朱雀
        SLOT_TURTLE     = 4,    // 玄武
        SLOT_MAX,
    };

    public:
        JobHunter(Player* player);
        JobHunter(Player * player, std::string& fighterList, std::string& mapInfo, UInt8 progress,
                UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt32 stepCount, UInt8 slotVal1, UInt8 slotVal2, UInt8 slotVal3);
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
        void SendGameInfo(UInt8 type, bool isUpdeated = false);

        void SendMapInfo();

        void SendAutoInfo();

        void OnCommand(UInt8 command, UInt8 val, UInt8 val2, bool isAuto = false);
        void OnAutoCommand(UInt8 type);

        void SendGridInfo(UInt16 pos);
        void OnAbort(bool isAuto);
        void OnLeaveGame(UInt16 spotId);
        bool CheckEnd();

        void OnAutoStop();

    private:

        UInt16 GetSpotIdFromGameId(UInt8 id);

        bool InitMap();
        void AddBossGrid( std::vector<UInt16>& validGrid, std::map<UInt16, UInt8>& neighbourCount);
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

        UInt8  _spItemRate[SLOT_MAX];
                                        // 青龙梦引掉率
                                        // 白虎梦引掉率
                                        // 朱雀梦引掉率
                                        // 玄武梦引掉率

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


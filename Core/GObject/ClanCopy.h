#ifndef _CLAN_COPY_H_
#define _CLAN_COPY_H_

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "GData/NpcGroup.h"
#include "GData/ClanCopyTable.h"
#include "Fighter.h"
#include "Mail.h"

//#define DEBUG_CLAN_COPY

#ifdef DEBUG_CLAN_COPY
#include <fstream>
#include <iostream>
#endif


namespace Battle
{
    class BattleSimulator;
}

namespace GData
{
    class NpcGroup;
}

namespace GObject
{

#define MONSTER_NORMAL 0   // 小怪
#define MONSTER_ELITE  1   // 精英
#define MONSTER_BOSS   2   // BOSS
//物攻  法攻  物防  法防  生命  身法  命中   闪避   反击   法术抵抗
static const float  s_rate3NpcBaseModulus[][6] = {
{0.005f, 0.005f, 0.01f, 0.01f, 0.05f, 0.001f},
{0.002f, 0.002f, 0.01f, 0.01f, 0.02f, 0.001f},
{0.002f, 0.002f, 0.01f, 0.01f, 0.02f, 0.001f},
};
static const float  s_rate3NpcAdvanceModulus[][4] = {
{0.0002f, 0.005f, 0.004f, 0.01f},
{0.0002f, 0.005f, 0.004f, 0.01f},
{0.0002f, 0.005f, 0.004f, 0.01f},
};

static const float  s_rate3NpcAdvanceModMax[] =  {200, 100, 100, 200};


#define  CLAN_COPY_LOCATION  0x3202

class Player;
class Clan;
class BattleSimulator;
class ClanCopyMonsterData;

static const UInt8 BATTLER_COUNT = 5;
static const UInt8 SPOT_COUNT = 5;
static const UInt8 SpotId[SPOT_COUNT] =
{
    0x01, 0x11, 0x21, 0x31, 0xff
};

static const UInt8 SpotBufferId[SPOT_COUNT] =
{
    0x01, 0x11, 0x21, 0x31, 0x01
};

static const UInt8 NextSpotNum[SPOT_COUNT] =
{
    1, 1, 1, 1, 3
};
static const UInt8 NextMoveTick[SPOT_COUNT] =
{
    4, 3, 3, 3, 2
};

static const UInt8 NextSpotId[SPOT_COUNT][16] =
{
    {0x01},
    {0x01},
    {0x01},
    {0x01},
    {0x11, 0x21, 0x31},
};

enum Copy_Spot
{
    Home = 0x01,
    Route1_Spot1 = 0x11,
    Route2_Spot1 = 0x21,
    Route3_Spot1 = 0x31,
    Enemy_Base = 0xff,
};

enum Copy_Spot_Buffer
{
    Spot_Buffer_Action = 0x11,
    Spot_Buffer_Hp = 0x21,
    Spot_Buffer_Attack = 0x31,
    Spot_Buffer_Defend = 0x01
};

enum CLAN_COPY_STATUS
{
    // 帮派副本状态列表
    CLAN_COPY_NONE    = 0x00,
    CLAN_COPY_READY   = 0x01,
    CLAN_COPY_PROCESS = 0x02,
    CLAN_COPY_WIN     = 0x03,
    CLAN_COPY_LOSE    = 0x04,
    CLAN_COPY_OVER    = 0xFF,  // 帮派副本数据待清除状态
};

enum CLAN_COPY_FORCE_END_TYPE
{
    FORCE_END_BY_RESET = 0x01,
    FORCE_END_BY_GM = 0x02,
    FORCE_END_BY_ERROR = 0xff
};

struct ClanCopyPlayer
{
    // 据点玩家信息
	ClanCopyPlayer(Player * player, UInt8 spSkillType = 0)
		: player(player), spSkillType(spSkillType)
	{
        deadType = 0;
	}
	Player * player;
    UInt8 spSkillType;
    UInt8 deadType;
};


struct isPlayerDead
{
    bool operator() (const ClanCopyPlayer& clanCopyPlayer) const
    {
        return (clanCopyPlayer.deadType?true:false);
    }
};

struct ClanCopyMonster
{
    // 帮派副本怪物信息结构
    UInt32 npcIndex;        // 该副本中怪物的序号(即副本中的身份证号码)
    UInt32 npcId;           // 怪物类型Id
    UInt8  monsterType;     // 怪物种类（普通怪，精英怪，boss）
    UInt16 waveCount;       // 第几波产生的怪物
    UInt16 npcValue;        // 怪物对主基地的破坏值
    UInt8  nextSpotId;      // 怪物下一个目标据点
    UInt32 nextMoveTick;    // 下一次移动的tick时间
    UInt8  deadType;        // 该怪物是否已经死亡
    bool   justMoved;       // 是否新进来的
    UInt16 formation;       // 怪物阵型
    std::string name;
    UInt8 level;
    std::vector<GData::NpcFData> npcList;  // 具体怪物列表



    ClanCopyMonster(UInt32 npcIndex, UInt32 npcId, UInt8 monsterType, UInt16 waveCount, UInt16 npcValue, UInt8 nextSpotId, UInt32 nextMoveTick, UInt16 copyLevel)
        : npcIndex(npcIndex), npcId(npcId), monsterType(monsterType), waveCount(waveCount), npcValue(npcValue), nextSpotId(nextSpotId), nextMoveTick(nextMoveTick)
    {
        deadType = true;;
        justMoved = false;
        npcList.clear();
        GData::NpcGroups::iterator ngIt = GData::npcGroups.find(npcId);
        if(ngIt == GData::npcGroups.end())
            return;
        GData::NpcGroup* ng = ngIt->second;
        name = ng->getName();
        level = ng->getLevel();
        formation = ng->getFormation();
        std::vector<GData::NpcFData>& nl = ng->getList();
        for (std::vector<GData::NpcFData>::iterator it = nl.begin(); it != nl.end(); ++ it)
        {
            Fighter * fighter = it->fighter->clone(NULL);
            GData::NpcFData fdata = {fighter, it->pos};

            npcList.push_back(fdata);

            fighter->maxhp = fighter->getBaseHP();
            fighter->setCurrentHP(fighter->maxhp, false);
            /*
            fighter->attack  = fighter->getBaseAttack()    * (1 + s_rate3NpcBaseModulus[monsterType][0]*copyLevel);
            fighter->magatk  = fighter->getBaseMagAttack() * (1 + s_rate3NpcBaseModulus[monsterType][1]*copyLevel);
            fighter->defend  = fighter->getBaseDefend()    * (1 + s_rate3NpcBaseModulus[monsterType][2]*copyLevel);
            fighter->magdef  = fighter->getBaseMagDefend() * (1 + s_rate3NpcBaseModulus[monsterType][3]*copyLevel);
            fighter->maxhp   = fighter->getBaseHP()        * (1 + s_rate3NpcBaseModulus[monsterType][4]*copyLevel);
            fighter->action  = fighter->getBaseAction()    * (1 + s_rate3NpcBaseModulus[monsterType][5]*copyLevel);
            fighter->hitrate = fighter->getBaseHitrate()   * (1 + s_rate3NpcAdvanceModulus[monsterType][0]*copyLevel);
            fighter->evade   = fighter->getBaseEvade()     * (1 + s_rate3NpcAdvanceModulus[monsterType][1]*copyLevel);
            fighter->counter = fighter->getBaseCounter()   * (1 + s_rate3NpcAdvanceModulus[monsterType][2]*copyLevel);
            fighter->magres  = fighter->getBaseMagRes()    * (1 + s_rate3NpcAdvanceModulus[monsterType][3]*copyLevel);

            fighter->hitrate = fighter->hitrate < s_rate3NpcAdvanceModMax[0] ? fighter->hitrate : s_rate3NpcAdvanceModMax[0];
            fighter->evade = fighter->evade < s_rate3NpcAdvanceModMax[0] ? fighter->evade : s_rate3NpcAdvanceModMax[1];
            fighter->counter = fighter->counter < s_rate3NpcAdvanceModMax[0] ? fighter->counter : s_rate3NpcAdvanceModMax[2];
            fighter->magres = fighter->magres < s_rate3NpcAdvanceModMax[0] ? fighter->magres : s_rate3NpcAdvanceModMax[3];
            */
            fighter->setDirty(false);

        }
    }

    ~ClanCopyMonster()
    {
        for (std::vector<GData::NpcFData>::iterator it = npcList.begin(); it != npcList.end(); ++ it)
        {
            delete it->fighter;
        }
    }

    UInt8 allHpP()
    {
        UInt32 total = 0;
        UInt32 totalmax = 0;
        for(UInt8 i = 0; i < npcList.size(); ++ i)
        {
            Fighter * fighter = npcList[i].fighter;
            if (fighter->getCurrentHP())
                total += fighter->getCurrentHP();
            else
                total += fighter->getMaxHP();
            totalmax += fighter->getBaseHP();
        }
        if (!totalmax)
            return 0;
        if (!total)
            return 100;
        UInt8 p = (((float)total)/totalmax) * 100;
        if (p > 100)
            p = 100;
        return p;
    }

};

struct isMonsterDead
{
    bool operator() (const ClanCopyMonster* clanCopyMonster) const
    {
        return clanCopyMonster->deadType? true : false;
    }
};

struct ClanCopySpot
{
    // 帮派副本的据点信息结构
    UInt8 spotId;
    UInt8 spotBufferType;          // 该据点加成类型
    float spotBufferValue;        // 该据点加成数据
    UInt16 maxPlayerCount;         // 该据点最大的玩家数
    UInt32 nextMoveTick;         // 上一场战斗发生的tick时间
    std::vector<UInt8> nextSpotId; // 通往下一个据点的id

    ClanCopySpot(UInt8 spotId, UInt8 spotBufferType, float spotBufferValue,
            UInt16 maxPlayerCount = 20, UInt32 nextMoveTick = 0)
        :spotId(spotId), spotBufferType(spotBufferType), spotBufferValue(spotBufferValue),
        maxPlayerCount(maxPlayerCount), nextMoveTick(nextMoveTick)
    {
    }

    ClanCopySpot(): spotId(0), spotBufferType(0), spotBufferValue(0), maxPlayerCount(20), nextMoveTick(0)
    {
    }
};

struct ClanCopyBattleInfo
{
    // 帮派副本战斗信息
    UInt64 playerId;        // 玩家Id
    UInt32 monsterIndex;    // 怪物唯一索引
    UInt8  battleResult;    // 战斗结果 1.玩家胜利 2.怪物胜利
    UInt8  remainHpP;       // 胜利方剩余的hp
    ClanCopyBattleInfo (UInt64 playerId, UInt32 monsterIndex, UInt8 battleResult, UInt8 remainHpP)
        : playerId(playerId), monsterIndex(monsterIndex), battleResult(battleResult), remainHpP(remainHpP)
    {
    }
};

class ClanCopy
{
    public:
        typedef std::vector<ClanCopyBattleInfo> BattleInfo;
        typedef std::map<UInt8, ClanCopySpot> SpotMap;
        typedef std::list<ClanCopyMonster* > SpotMonsterList;
        typedef std::list<ClanCopyPlayer> SpotPlayerList;
        typedef std::map<UInt8, SpotMonsterList> SpotMonster; // 该据点的怪物
        typedef std::map<UInt8, SpotPlayerList> SpotPlayer;   // 该据点的玩家
        typedef std::map<UInt8, SpotPlayerList> SpotDeadPlayer;   // 该据点死亡的玩家（逃跑也算）
        typedef std::map<UInt8, BattleInfo> SpotBattleInfo;
        typedef std::set<ClanCopyMonster* > DeadMonster;

    public:
        ClanCopy(Clan *c, UInt32 copyId, Player *player);
        ~ClanCopy();

        UInt8 getStatus();
        UInt16 getTotalPlayer();

        void addPlayer(Player* player, bool needNotify = true);
        void addPlayerFromSpot(Clan* c);
        void addObserver(Player* player);
        void playerEscape(Player* player);

        void initCopyData();
        void updateBufferAttr(UInt8 spotId);
        void process(UInt32 now);
        void requestStart(Player* player);

        void forceEnd(UInt8 type);

        void adjustPlayerPosition(Player * opPlayer, Player* player, UInt8 oldSpotId, UInt8 oldPosition, UInt8 newSpotId, UInt8 newPositon);

        void updateSpotBufferValue(UInt8 spotId);

        void monsterAssault(UInt8 spotId);
        void enemyBaseAct();
        void createEnemy();
        void createEnemyToSpotId(UInt8 spotId, const GData::ClanCopyMonsterData& clanCopyMonsterData);
        void monsterMove(UInt8 spotId);
        void spotCombat(UInt8 spotId);
        void attackHome(ClanCopyMonster* clanCopyMonster);
        bool checkWin();
        void addWinReward(UInt32 awardValue);

        void setInterval(UInt32 interval);
        void setStartTick(UInt32 tickCount);
        void setStartTimeInterval(UInt32 startTimeInterval);

        void putSpotMonster(BattleSimulator& bsim, ClanCopyMonster& clanCopyMonster);

        void notifyAll(Stream st);
        void notifyWaitForWin(Stream st);
        void notifySpotPlayerInfo(Player * player = NULL);
        void notifySpotBattleInfo(Player * player = NULL);
        void notifyCopyLose();
        void notifyCopyWin(UInt32 awardValue, UInt8 itemTypes, MailPackage::MailItem * mitem);
        void notifyLauncherEscape();

        void notifyCopyCreate();

    private:
        Clan   *_clan;
        UInt16 _copyLevel;
        UInt32 _copyId;
        UInt8  _status;


        SpotMap        _spotMap;
        SpotMonster    _spotMonster;
        SpotPlayer     _spotPlayer;
        SpotDeadPlayer _spotDeadPlayer;
        SpotBattleInfo _spotBattleInfo;
        DeadMonster    _deadMonster;
        std::vector<Player *> _waitForWinPlayer; // 失败后逃跑的玩家列表（胜利后需要获得邮件）

        UInt32 _homeMaxHp;
        UInt32 _homeHp;
        UInt32 _maxReward;

        UInt32 _readyTime;         // 副本报名的时间戳
        UInt32 _startTime;         // 副本开始的时间戳
        UInt32 _endTime;           // 副本结束的时间戳
        UInt32 _tickTime;

        UInt32 _tickTimeInterval;      // 状态改变的最小时间间隔（秒）
        UInt32 _startTick;             // 开始第一轮刷怪的tick
        UInt32 _monsterRefreshTick;    // 刷新怪物的间隔tick
        UInt32 _tickCount;             // 游戏已经开始的tick数

        UInt32 _maxMonsterWave;        // 该等级副本刷怪的波数（包括轮空的）
        UInt32 _curMonsterWave;        // 当前已经刷新的怪的波数（包括轮空的）

        UInt32 _npcIndex;

        std::set<Player *> _observerPlayer; // 围观人员
        std::map<Player *, UInt8> _playerIndex; //快速查找玩家对应据点的索引

        Player* _launchPlayer;
        URandom _rnd;
        std::map<UInt8, GData::AttrExtra>  _spotAttrs;

#ifdef DEBUG_CLAN_COPY
        // 谁能告诉我为什么一定需要通过指针new一个才能编译通过
        // 测试不用指针，居然又好了，真的是人品驱动编程啊
        std::fstream * fileSt;
#endif
};


class ClanCopyMgr : public Singleton <ClanCopyMgr>
{
    typedef std::map<UInt32, ClanCopy* > ClanCopyMap;

    public:
    ClanCopyMgr();
    ~ClanCopyMgr();

    void setInterval(UInt32 time);

    void ResetBegin();
    void Reset();
    void ResetEnd();

    void process(UInt32 now);

    UInt8 getCopyStatus(UInt32 clanId);

    // 创建一个新帮派副本
    bool createClanCopy(Player* player, Clan *c);

    void forceEndClanCopy(ClanCopy *clanCopy, UInt8 type = FORCE_END_BY_RESET);
    void forceEndAllClanCopy();
    // 清除已经结束的副本
    void deleteClanCopy(ClanCopy *clanCopy);

    void playerEnter(Player * player, bool needNotify = true);
    void playerLeave(Player * player);
    void playerRequestStart(Player * player);

    UInt16 getTotalPlayer(Clan *c);
    ClanCopy * getClanCopyByClan(Clan *c);

    private:
    UInt16 _maxCopyLevel;  // 全服最高通关的副本等级
    ClanCopyMap _clanCopyMap;
    bool _reset;           // 是否在副本重置时间段 （每周日23:30~周一00:30）
    UInt32 _interval;
};

struct ClanCopyLog
{
    UInt32 clanId;
    UInt32 logTime;
    UInt8  logType;
    UInt32 logVal;
    std::string playerName;
    ClanCopyLog (UInt32 clanId, UInt32 logTime,
            UInt8 logType, UInt32 logVal, std::string playerName)
        : clanId(clanId), logTime(logTime), logType(logType), logVal(logVal), playerName(playerName)
    {
    }
};

}



#endif

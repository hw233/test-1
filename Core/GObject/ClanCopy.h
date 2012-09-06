#ifndef _CLAN_COPY_H_
#define _CLAN_COPY_H_

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "GData/NpcGroup.h"
#include "Fighter.h"

#define DEBUG_CLAN_COPY

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
{0.005, 0.005, 0.01, 0.01, 0.05, 0.001},
{0.002, 0.002, 0.01, 0.01, 0.02, 0.001},
{0.002, 0.002, 0.01, 0.01, 0.02, 0.001},
};
static const float  s_rate3NpcAdvanceModulus[][4] = {
{0.0002, 0.005, 0.004, 0.01},
{0.0002, 0.005, 0.004, 0.01},
{0.0002, 0.005, 0.004, 0.01},
};

#define  CLAN_COPY_LOCATION  0x3202

class Player;
class Clan;
class BattleSimulator;

static const UInt8 SPOT_COUNT = 5;
static const UInt8 SpotId[SPOT_COUNT] =
{
    0x01, 0x11, 0x21, 0x31, 0xff
};
static const UInt8 NextSpotNum[SPOT_COUNT] = 
{
    1, 1, 1, 1, 3
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
    CLAN_COPY_OVER    = 0xFF,
};

struct ClanCopyPlayer
{
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
    UInt32 npcId;
    UInt8  monsterType;     // 怪物种类（普通怪，精英怪，boss）
    UInt16 waveCount;       // 第几波产生的怪物
    UInt16 npcValue;        // 怪物对主基地的破坏值
    UInt8  nextSpotId;      // 怪物下一个目标据点
    UInt32 nextMoveTick;    // 下一次移动的tick时间
    bool   isDead;          // 该怪物是否已经死亡
    bool   justMoved;       // 是否新进来的
    std::vector<GData::NpcFData> npcList;  // 具体怪物列表 
    std::vector<Fighter> npcFighter;
    UInt16 formation;       // 怪物阵型
    std::string name;
    UInt8 level;
    

    ClanCopyMonster(UInt32 npcId, UInt8 monsterType, UInt16 waveCount, UInt16 npcValue, UInt8 nextSpotId, UInt32 nextMoveTick, UInt16 copyLevel)
        : npcId(npcId), monsterType(monsterType), waveCount(waveCount), npcValue(npcValue), nextSpotId(nextSpotId), nextMoveTick(nextMoveTick)
    {
        isDead = false;
        justMoved = false;
        npcList.clear();
        npcFighter.clear();
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
            npcFighter.push_back(*(it->fighter));
            UInt8 index = npcFighter.size() - 1;
            GData::NpcFData fdata = {&npcFighter[index], it->pos};
            npcList.push_back(fdata);

            Fighter * fighter = &npcFighter[index];
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
        }
    }
};

struct isMonsterDead
{
    bool operator() (const ClanCopyMonster& clanCopyMonster) const
    {
        return clanCopyMonster.isDead;
    }
};

struct ClanCopySpot
{
    // 帮派副本的据点信息结构
    UInt8 spotId;
    UInt8 spotBufferType;          // 该据点加成类型
    UInt16 spotBufferValue;        // 该据点加成数据
    UInt16 maxPlayerCount;         // 该据点最大的玩家数
    UInt32 lastBattleTick;         // 上一场战斗发生的tick时间
    std::vector<UInt8> nextSpotId; // 通往下一个据点的id

    ClanCopySpot(UInt8 spotId, UInt8 spotBufferType, UInt16 spotBufferValue,
            UInt16 maxPlayerCount = 20, UInt32 lastBattleTick = 0)
        :spotId(spotId), spotBufferType(spotBufferType), spotBufferValue(spotBufferValue),
        maxPlayerCount(maxPlayerCount), lastBattleTick(lastBattleTick)
    {
    }

    ClanCopySpot(): spotId(0), spotBufferType(0), spotBufferValue(0), maxPlayerCount(20), lastBattleTick(0)
    {
    }


};

class ClanCopy
{
    public:
        typedef std::map<UInt8, ClanCopySpot> SpotMap;
        typedef std::list<ClanCopyMonster> SpotMonsterList;
        typedef std::list<ClanCopyPlayer> SpotPlayerList;
        typedef std::map<UInt8, SpotMonsterList> SpotMonster; // 该据点的怪物
        typedef std::map<UInt8, SpotPlayerList> SpotPlayer;   // 该据点的玩家
        typedef std::map<UInt8, SpotPlayerList> SpotDeadPlayer;   // 该据点死亡的玩家（逃跑也算）

    public:
        ClanCopy(Clan *c, Player *player); 
        ~ClanCopy();

        UInt8 getStatus();
        UInt16 getTotalPlayer();

        void addPlayer(Player* player);
        void addPlayerFromSpot(Clan* c);
        void addObserver(Player* player);
        void playerEscape(Player* player);

        void initCopyData();
        void process(UInt32 now);
        void requestStart(Player* player);

        void adjustPlayerPosition(Player * opPlayer, Player* player, UInt8 oldSpotId, UInt8 oldPosition, UInt8 newSpotId, UInt8 newPositon);

        void monsterAssault(const UInt8& spotId);
        void enemyBaseAct();
        void createEnemy();
        void monsterMove(UInt8 spotId);
        void spotCombat(SpotMonsterList& monsterList, SpotPlayerList& playerList, UInt8 spotId);
        void attackHome(ClanCopyMonster& clanCopyMonster);
        bool checkWin();
        void setInterval(UInt32 interval);
        void setStartTick(UInt32 tickCount);
        void setStartTimeInterval(UInt32 startTimeInterval);

        void putSpotMonster(BattleSimulator& bsim, ClanCopyMonster& clanCopyMonster);

        void notifyAll(Stream st);
        void notifySpotPlayerInfo(Player * player = NULL);
        void notifyCopyLose();
        void notifyCopyWin();
        void notifyLauncherEscape();

        void notifyCopyCreate();

    private:
        Clan   *_clan;
        UInt16 _copyLevel;
        UInt8  _status;


        SpotMap _spotMap;
        SpotMonster _spotMonster;
        SpotPlayer _spotPlayer;
        SpotDeadPlayer _spotDeadPlayer;
        UInt32 _homeHp;

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

        std::set<Player *> _observerPlayer; // 围观人员
        std::map<Player *, UInt8> _playerIndex; //快速查找玩家对应据点的索引

        Player* _launchPlayer;
        URandom _rnd;

#ifdef DEBUG_CLAN_COPY
        // 谁能告诉我为什么一定需要通过指针new一个才能编译通过
        std::fstream* fileSt;           
#endif
};


class ClanCopyMgr : public Singleton <ClanCopyMgr>
{
    typedef std::map<UInt32, ClanCopy* > ClanCopyMap;

    public:
    ClanCopyMgr();
    ~ClanCopyMgr();

    void process(UInt32 now);

    UInt8 getCopyStatus(UInt32 clanId);

    // 创建一个新帮派副本
    bool createClanCopy(Player* player, Clan *c);

    // 清除已经结束的副本
    void deleteClanCopy(ClanCopy *clanCopy);

    void playerEnter(Player * player);
    void playerLeave(Player * player);
    void playerRequestStart(Player * player);

    UInt16 getTotalPlayer(Clan *c);

    private:
    UInt16 _maxCopyLevel;  // 全服最高通关的副本等级
    ClanCopyMap _clanCopyMap;

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

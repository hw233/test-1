#ifndef _CLAN_COPY_H_
#define _CLAN_COPY_H_

#include "Config.h"
#include "Common/Singleton.h"

//#define DEBUG_CLAN_COPY   // 这条件宏编译搞不定啊

#ifdef DEBUG_CLAN_COPY
#include <fstream>
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


#define  CLAN_COPY_LOCATION  0x3202

class Player;
class Clan;

enum Copy_Spot
{
    Home = 0x01,
    Route1_Spot1 = 0x11,
    Route2_Spot1 = 0x21,
    Route3_Spot1 = 0x31,
    Enemy_Base = 0xff,
};

enum CLAN_COPY_STATUS
{
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
        isDead = false;
	}
	Player * player;
    UInt8 spSkillType;
    bool isDead;
};

struct isPlayerDead
{
    bool operator() (const ClanCopyPlayer& clanCopyPlayer) const
    {
        return clanCopyPlayer.isDead;
    }
};

struct ClanCopyMonster
{
    // 帮派副本怪物信息结构
    UInt32 npcId;
    UInt16 waveCount;       // 第几波产生的怪物
    UInt16 npcValue;        // 怪物对主基地的破坏值
    UInt8  nextSpotId;      // 怪物下一个目标据点
    UInt32 nextMoveTick;    // 下一次移动的tick时间
    bool   isDead;          // 该怪物是否已经死亡
    bool   justMoved;       // 是否新进来的

    ClanCopyMonster(UInt32 npcId, UInt16 waveCount, UInt16 npcValue, UInt8 nextSpotId, UInt32 nextMoveTick)
        : npcId(npcId), waveCount(waveCount), npcValue(npcValue), nextSpotId(nextSpotId), nextMoveTick(nextMoveTick)
    {
        isDead = false;
        justMoved = false;
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
    std::vector<UInt8> nextSpotId; // 通往下一个据点的id
    UInt16 maxPlayerCount;         // 该据点最大的玩家数
    UInt32 lastBattleTick;         // 上一场战斗发生的tick时间

};

class ClanCopy
{
    public:
        typedef std::map<UInt8, ClanCopySpot> SpotMap;
        typedef std::list<ClanCopyMonster> SpotMonsterList;
        typedef std::list<ClanCopyPlayer> SpotPlayerList;
        typedef std::map<UInt8, SpotMonsterList> SpotMonster; // 该据点的怪物
        typedef std::map<UInt8, SpotPlayerList> SpotPlayer;   // 该据点的玩家
        typedef std::map<UInt8, ClanCopyPlayer> SpotDeadPlayer;   // 该据点死亡的玩家（逃跑也算）

    public:
        ClanCopy(Clan *c, Player *player); 
        ~ClanCopy();

        UInt8 getStatus();
        UInt16 getTotalPlayer();

        void addPlayer(Player* player);
        void addObserver(Player* player);
        void playerEscape(Player* player);

        void initCopyData();
        void process(UInt32 now);

        void monsterAssault(const UInt8& spotId);
        void enemyBaseAct();
        void createEnemy();
        void monsterMove(UInt8 spotId);
        void spotCombat(SpotMonsterList& monsterList, SpotPlayerList& playerList, UInt8 spotId);
        void attackHome(ClanCopyMonster& clanCopyMonster);
        void lose();
        bool checkWin();
        void setInterval(UInt32 interval);
        void setStartTick(UInt32 tickCount);
        void setStartTimeInterval(UInt32 startTimeInterval);

        void notifySpotPlayerInfo(UInt8 spotId);
        void notifyCopyLose();
        void notifyCopyWin();
        void notifyLauncherEscape();

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

        UInt32 _totalPlayer;           // 总共参与人数（报名结束阶段截止）
        UInt32 _deadPlayer;            // 已经死亡人数
        UInt32 _escapePlayer;          // 中途逃跑人数

        std::set<Player *> _observerPlayer; // 围观人员
        std::map<Player *, UInt8> _playerIndex; //快速查找玩家对应据点的索引

        Player* _launchPlayer;

#ifdef DEBUG_CLAN_COPY
        std::fstream fileSt;
#endif
};


class ClanCopyMgr : public Singleton <ClanCopyMgr>
{
    typedef std::map<UInt32, ClanCopy> ClanCopyMap;

    public:
    ClanCopyMgr();
    ~ClanCopyMgr();

    void process(UInt32 now);

    UInt8 getCopyStatus(UInt32 clanId);

    // 创建一个新帮派副本
    bool createClanCopy(Player* player, Clan *c);

    // 清除已经结束的副本
    void deleteClanCopy(ClanCopy *clanCopy);

    void clanLose(ClanCopy* clanCopy);

    void playerEnter(Player * player);
    void playerLeave(Player * player);

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

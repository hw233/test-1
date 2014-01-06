#ifndef _ARENASERVERWAR_H_
#define _ARENASERVERWAR_H_

#include "Common/Stream.h"
#include "GObjectManager.h"
#include "Common/BinaryReader.h"
#include "Common/TimeUtil.h"
#include "GObject/Player.h"
#include "GObject/World.h"

namespace GObject
{

#define SERVERWAR_MAX_MEMBER 10

struct DBArenaServerWar;
struct DBArenaBet;

enum SERVERWAR_STAGE
{
    SWAR_ENTERED_NONE = 0,
    SWAR_ENTERED_STAGE_1 = 1,    // 跨区赛甲组
    SWAR_ENTERED_STAGE_2 = 2,    // 跨区赛乙组
    SWAR_ENTERED_MAX,
};

enum SERVERWAR_STAGE_PROGRESS
{
    e_war_sign = 0,
    e_war_challenge = 1,
    e_war_group = 2,
    e_war_16 = 3,
    e_war_8 = 4,
    e_war_4 = 5,
    e_war_2 = 6,
    e_war_1 = 7,
    e_war_sign_end = 8,
    e_war_challenge_end = 9,
    e_war_group_end = 10,
    e_war_16_end = 11,
    e_war_end = 12,
    e_war_nextbegin = 13,
};

struct EnumMailStruct
{
    EnumMailStruct(UInt8 g = 0, UInt8 r = 0, UInt16 s = 0, bool iw = false, UInt8 tw = 0, UInt8 tl = 0, UInt32 lh = 0) :
        group(g), round(r), session(s), isWon(iw), twon(tw), tloss(tl), longhun(lh) {}
    UInt8 group;
    UInt8 round;
    UInt16 session;
    bool isWon;
    UInt8 twon;
    UInt8 tloss;
    UInt32 longhun;
};

struct SWPlayerData
{
    SWPlayerData(Player * pl = NULL, UInt32 bp = 0): player(pl), battlePoint(bp) {}
	Player * player;
	UInt32 battlePoint;
};

struct sw_battlePoint
{
    bool operator()(const SWPlayerData& pd1, const SWPlayerData& pd2) const
    {
        if(pd1.battlePoint == pd2.battlePoint)
        {
            return (pd1.player)->getId() < (pd2.player)->getId();
        }

        return pd1.battlePoint > pd2.battlePoint;
    }
};
typedef std::set<SWPlayerData, sw_battlePoint> SWPDSort;


struct PlayerInfo
{
    PlayerInfo() : playerId(0), level(0), mainFid(0), battlePoint(0) {}
    UInt64 playerId;
    std::string name;
    UInt8 level;
    UInt8 mainFid;
    UInt32 battlePoint;
    inline void readInfo(BinaryReader& brd)
    {
        brd >> playerId >> name >> level;
        brd >> mainFid >> battlePoint;
    }
    inline void appendInfo(Stream& st) const
    {
        st << name << level << battlePoint << mainFid;
    }
};
struct pd_battlePoint
{
    bool operator()(const PlayerInfo& pd1, const PlayerInfo& pd2) const
    {
        if(pd1.battlePoint == pd2.battlePoint)
        {
            return pd1.playerId < pd2.playerId;
        }

        return pd1.battlePoint > pd2.battlePoint;
    }
};
typedef std::set<PlayerInfo, pd_battlePoint> PInfoSort;


//交战的两个玩家信息
struct PairPlayerReport
{
    PairPlayerReport() : won(0), battleId(0) {}
    //PlayerInfo pInfo1;
    //PlayerInfo pInfo2;
    UInt8 won;
    UInt32 battleId;
    std::string name1;
    std::string name2;

    inline void readReport(BinaryReader& brd)
    {
        //pInfo1.readInfo(brd);
        //pInfo2.readInfo(brd);
        brd >> won >> battleId;
        brd >> name1 >> name2;
    }
    inline void appendReport(Stream& st)
    {
        st << won << battleId;
        st << name1 << name2;
        //st << pInfo1.name << pInfo1.mainFid << pInfo1.battlePoint;
        //st << pInfo2.name << pInfo2.mainFid << pInfo2.battlePoint;
    }
};

//晋级赛两个服务战斗战报数据
struct PairServerReport
{
    PairServerReport() : channelId(0), serverId(0), otherChannelId(0), otherServerId(0), won(0xFF) {}
    int channelId;
    int serverId;
    int otherChannelId;
    int otherServerId;
    std::string serverName;
    std::string otherServerName;
    UInt8 won;  //1:serverId胜 0:otherServerId胜
    std::vector<PairPlayerReport> ppReportVec;

    inline void readServerReport(BinaryReader& brd)
    {
        UInt8 tmpWin = 0;
        brd >> tmpWin;
        if(tmpWin == 0)
            won = 1;
        else
            won = 0;
        brd >> channelId >> serverId >> otherChannelId >> otherServerId;
        brd >> serverName >> otherServerName;
        UInt8 size = 0;
        brd >> size;
        for(UInt8 i = 0; i < size; ++ i)
        {
            PairPlayerReport ppr;
            ppr.readReport(brd);
            ppReportVec.push_back(ppr);
        }
    }
    inline void appendServerReport(Stream& st)
    {
        st << serverId << serverName << otherServerId << otherServerName;
        UInt8 size = ppReportVec.size();
        st << won << size;
        for(UInt8 i = 0; i < size; ++ i)
        {
            ppReportVec[i].appendReport(st);
        }
    }
};

//淘汰赛战报
struct ServerPreliminaryBattle
{
    ServerPreliminaryBattle(): won(0), type(0), otherChannelId(0), otherServerId(0), otherAttrRatio(0), battleTime(0) {}
	UInt8 won;
	UInt8 type;
	int otherChannelId;
	int otherServerId;
    UInt8 otherAttrRatio;
    std::string serverName;
    std::string otherServerName;
	UInt32 battleTime;
    std::vector<PairPlayerReport> battles;
    std::vector<PlayerInfo> otherPInfoVec;

    inline void appendPreReport(Stream& st)
    {
        //append ReportData
        st << otherServerId << otherServerName;
        const static UInt8 won_mod[] = {1, 0, 1, 0, 1};
        UInt8 size = battles.size();
        st << won_mod[won] << size;
        for(UInt8 i = 0; i < size; ++ i)
        {
            battles[i].appendReport(st);
        }
        //append ServerData
        st << otherServerId << otherServerName;
        st << static_cast<UInt32>(0) << static_cast<UInt32>(0);
        size = otherPInfoVec.size();
        st << static_cast<UInt8>(otherAttrRatio/3) << size;
        for(UInt8 i = 0; i < size; ++ i)
        {
            otherPInfoVec[i].appendInfo(st);
        }
    }
    inline void readPreReport(BinaryReader& brd)
    {
        brd >> won >> type >> battleTime;
        brd >> otherChannelId >> otherServerId >> otherServerName >> otherAttrRatio;
        UInt8 size = 0;
        brd >> size;
        for(UInt8 i = 0; i < size; ++ i)    //对阵战报数据
        {
            PairPlayerReport ppr;
            ppr.readReport(brd);
            battles.push_back(ppr);
        }
        brd >> size;
        for(UInt8 i = 0; i < size; ++ i)    //对手玩家信息
        {
            PlayerInfo pInfo;
            pInfo.readInfo(brd);
            otherPInfoVec.push_back(pInfo);
        }
    }
};

//队伍
struct ServerEliminationPlayer
{
	ServerEliminationPlayer(): channelId(0), serverId(0), battlePoint(0), support(0), attrRatio(0) {}
    int channelId;
    int serverId;
    std::string serverName;
    UInt32 battlePoint;
    UInt32 support;
    UInt8 attrRatio;
    PInfoSort pInfoSet;
	std::vector<ServerPreliminaryBattle> battlesVec;   //本服淘汰赛战报
	std::map<Player *, UInt8> betMap;

    ~ServerEliminationPlayer()
    {
        betMap.clear();
    }
	inline void resetBet()
    {
        betMap.clear();
    }
    inline void readPlayerInfo(BinaryReader& brd)
    {
        pInfoSet.clear();
        UInt8 size = 0;
        brd >> size;
        for(UInt8 i = 0; i < size; ++ i)
        {
            PlayerInfo pInfo;
            pInfo.readInfo(brd);
            pInfoSet.insert(pInfo);
        }
    }
    inline void appendServerData(Stream& st)
    {
        st << serverId << serverName << battlePoint << support;
        st << static_cast<UInt8>(attrRatio/3) << static_cast<UInt8>(pInfoSet.size());

        for(PInfoSort::iterator it = pInfoSet.begin(); it != pInfoSet.end(); ++ it)
        {
            (*it).appendInfo(st);
        }
    }
};
typedef ServerEliminationPlayer ServerPreliminaryPlayer;
typedef std::list<ServerPreliminaryPlayer> ServerPreliminaryPlayerList;
typedef ServerPreliminaryPlayerList::iterator ServerPreliminaryPlayerListIterator;
typedef std::map<int, ServerPreliminaryPlayerListIterator> ServerPreliminaryPlayerListMap;
struct ServerPlayerLess
{
     bool operator()(const ServerPreliminaryPlayerListIterator sp1, const ServerPreliminaryPlayerListIterator sp2) const
     {
         return sp1->battlePoint > sp2->battlePoint;
     }
};
typedef std::multiset<ServerPreliminaryPlayerListIterator, ServerPlayerLess> ServerPreliminaryPlayersSet;


//晋级赛战报
struct ServerEliminationBattle
{
	ServerEliminationBattle() : wonFlag(0) {}
    std::vector<PairServerReport> battles;
    UInt8 wonFlag;

	inline void reset()
	{
		battles.clear();
	}
    inline UInt8 winner(UInt8 idx)
    {
        UInt8 w = 0;
        const UInt8 round[4] = { 3, 5, 5, 7 };
        const UInt8 oflag[7] = {1, 2, 4, 8, 16, 32, 64};
        for(int i = 0; i < round[idx]; ++ i)
            if(wonFlag & oflag[i])
                ++ w;
        if((w << 1) > round[idx])
            return 1;
        return 0;
    }
    inline UInt8 winCount(UInt8 idx)
    {
        UInt8 w = 0;
        const UInt8 round[4] = { 3, 5, 5, 7 };
        const UInt8 oflag[7] = {1, 2, 4, 8, 16, 32, 64};
        for(int i = 0; i < round[idx]; ++ i)
            if((wonFlag & oflag[i]) == 0)
                ++ w;
        return w;
    }
};

//玩家押注信息
struct PlayerBetData
{
    PlayerBetData() {}

	std::vector<BetInfo> betList[SWAR_ENTERED_MAX-1][5];
};

//历届冠军结构信息
struct LeaderServer
{
    LeaderServer() : serverId(0) {}
    int serverId;
    std::string serverName;
    PInfoSort members;
};


class ServerWarMgr
{
    public:
        ServerWarMgr(): _loaded(false), _notified(0), _session(0), _progress(e_war_nextbegin), _status(0),
            _round(0), _nextTime(0)
        {
            memset(_finalIdx, 0, sizeof(_finalIdx));
        }
        ~ServerWarMgr() {}

	    inline bool active() { return _loaded && (_session != 0x8001 || (_progress != 0)) && World::getArenaState() == ARENA_XIANJIE_CHUANQI; }
        inline bool isOpen() { return World::getArenaState() == ARENA_XIANJIE_CHUANQI; }
        inline UInt16 getSession() { return _session; }
        inline bool findSWPlayerData(Player * pl)
        {
            if (pl == NULL) return false;
            for (SWPDSort::iterator it = _signSort.begin(); it != _signSort.end(); ++ it)
            {
                if ((*it).player == pl)
                    return true;
            }
            return false;
        }
        inline void insertSWPlayerData(Player * pl, bool needFind = true)
        {
            if (pl == NULL)
                return;
            if (needFind)
            {
                for (SWPDSort::iterator it = _signSort.begin(); it != _signSort.end(); ++ it)
                {
                    if ((*it).player == pl)
                    {
                        _signSort.erase(it);
                        break;
                    }
                }
            }
            _signSort.insert(SWPlayerData(pl, pl->getBattlePoint()));
        }
        inline void clearPreliminaryBattels()
        {
            for(UInt8 i = 0; i < SWAR_ENTERED_MAX-1; ++ i)
            {
                if(!_preliminaryServers[i].empty())
                {
                    _preliminaryServers[i].clear();
                    _preliminaryServers_list[i].clear();
                    _preliminaryServers_list_set[i].clear();
                }
            }
        }

        void loadFromDB(DBArenaServerWar&);
        void pushBetFromDB(Player *, DBArenaBet&);
        void updateSignupToDB(Player *);
        void clearWarSort();
        void setWarSort();
        void signup(Player *);
        void challenge(Player *, std::string&);
        void notifyChallengeResult(Player* atker, Player* defer, bool win);
        void attackPlayer(Player* atker, Player* defer);
        void beAttackByPlayer(Player* defer, Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup);
        void sendWarSortInfo(Player *);
        void commitLineup(Player *);
        void jiJianTai_operate(Player *);
        void jiJianTai_complete(Player *, UInt8);
        void jiJianTai_convert(Player *);
        void jiJianTai_openBox(Player *, UInt8);
        void sendjiJianTaiInfo(Player *);
        void enterArena();
        void enterArena_neice();
        void readFrom(BinaryReader&);
        void readTeams(BinaryReader&);
        void readHistories(BinaryReader&);
        void readElimination(BinaryReader&);
        void pushPreliminary(BinaryReader&);
        void addLonghun(UInt32);
        void calcFinalBet(int);
        void calcBet(ServerPreliminaryPlayer&, UInt16, UInt8, UInt8, bool, const char *);
        void sendTeamMail(const std::string&, const std::string&);
        void giveTeamLastAward(UInt8, UInt8, UInt8);
        void sendTeamMail_neice(ServerEliminationPlayer&, const std::string&, const std::string&);
        void giveTeamLastAward_neice(ServerEliminationPlayer&, UInt8, UInt8, UInt8);
        void sendTeamMail_neiceJiJian(ServerEliminationPlayer&, const std::string&, const std::string&);
        void addLonghun_neice(ServerEliminationPlayer&, UInt32);
        void sendStatus(Player*);
        void sendActive(Player*);
        void sendSignupInfo(Player *);
        void bet(BinaryReader&, Player *);
        UInt8 bet1(Player * player, UInt8 state, UInt8 group, int sid, UInt8 type);
        UInt8 bet2(Player * player, UInt8 state, UInt8 group, UInt16 pos, UInt8 type);
        void updateBattlePoint(BinaryReader&);
        void updateSuport(UInt8, UInt8, UInt16);
        void sendPreliminary(Player*, UInt8, UInt8, UInt16, UInt8);
        void sendElimination(Player *, UInt8, UInt8);
        bool hasLastLeaderBoard();
        void updateLeaderBoard(BinaryReader&);
        void sendLeaderBoard(Player *);
        void sendLastLeaderBoard(Player *);
    public:
        Stream _readbuf;

    private:
        SWPDSort _signSort;
        std::map<Player *, UInt8> _warSort;
        bool _loaded;
        UInt8 _notified;
        UInt16 _session;
        UInt8 _progress;
        UInt8 _status;
        UInt8 _round;
        UInt32 _nextTime;

        ServerEliminationPlayer _finals[SWAR_ENTERED_MAX-1][16];   //晋级赛队伍
        UInt64 _finalIdx[SWAR_ENTERED_MAX-1][5][16];
        ServerEliminationBattle _finalBattles[SWAR_ENTERED_MAX-1][15]; //晋级赛战报
        ServerPreliminaryPlayerListMap _preliminaryServers[SWAR_ENTERED_MAX-1];  //进16强战报
        ServerPreliminaryPlayerList _preliminaryServers_list[SWAR_ENTERED_MAX-1];
        ServerPreliminaryPlayersSet _preliminaryServers_list_set[SWAR_ENTERED_MAX-1];
        std::map<Player *, PlayerBetData> _playerBet;

        std::map<UInt16, LeaderServer> _leaderBoard;
};

class ServerWarBoss
{
public:
    ServerWarBoss();
    ~ServerWarBoss() {}

    inline void setHP(UInt32 hp)
    {
        if (!_final)
            return;
        if (!hp)
            hp = 100;
        _hp[0] = hp;
    }
    bool isServerWarBoss(UInt32);
    void process(UInt32 now);
    void calcNext(UInt32 now);
    void broadcastTV(UInt32 now);
    void appear(UInt32 npcId);
    void startBoss();
    bool attack(Player* pl, UInt16 loc, UInt32 npcId);
    void deleteBoss();
    void disappear();
    bool attackBoss(Player* pl, UInt32 npcId, UInt8 expfactor, bool final);
    void reward(Player* player);
    void getRandList(UInt32 sz, UInt32 num, std::set<UInt32>& ret);

    void sendHpMax(Player* = NULL);
    void sendHp(Player* = NULL);
    void sendDmg(UInt32);
    void sendCount(Player* = NULL);
    void sendId(Player* = NULL);
    void sendLoc(Player* = NULL);
    void sendDaily(Player* = NULL);
    void sendBossInfo(UInt8 state, Player* player = NULL);
private:
    UInt32 _prepareTime;
    UInt32 _appearTime;
    UInt32 _disappearTime;
    UInt8 _prepareStep;

    UInt32 _npcid;
    UInt16 _loc;
    UInt8 _count;
    bool _disappeared;
    bool _final;
    UInt8 _percent;
    GData::NpcGroup* _ng;
    std::vector<UInt32> _hp;
    AtkInfoType _atkinfo;
};

extern ServerWarMgr serverWarMgr;
extern ServerWarBoss serverWarBoss;
extern bool server_addBuffData(Player * player, void * data);

}

#endif // _ARENASERVERWAR_H_

#ifndef _ARENATEAM_H_
#define _ARENATEAM_H_

#include "Common/Stream.h"
#include "GObjectManager.h"
#include "Common/BinaryReader.h"
#include "Common/TimeUtil.h"
#include "Arena.h"
#include "GObject/World.h"

namespace GObject
{

class Player;
struct DBTeamArenaData;
struct DBTeamArenaSkill;

/********** 组队跨服战 **********/
#define TEAMARENA_MAXMEMCNT 3

enum TEAMARENA_STAMP_TYPE   //仙界跨服战 成员印记
{
    TEAMARENA_NONE    = 0,   //无印记
    TEAMARENA_SHITOU  = 1,   //石头
    TEAMARENA_JIANDAO = 2,   //剪刀
    TEAMARENA_BU      = 3,   //布
    TEAMARENA_STAMP_MAX,
};

enum TEAMARENA_ENTER_STATE       //仙界跨服战 战队参加状态
{
    TEAMARENA_UNENTER  = 0,     //未参加
    TEAMARENA_ENTER    = 1,     //已参加
    TEAMARENA_OBSOLETE = 2,     //被淘汰
    TEAMARENA_ENTER_MAX,
};

enum TEAM_STAGE_PROGRESS
{
    e_team_sign = 0,
    e_team_32 = 1,
    e_team_16 = 2,
    e_team_8 = 3,
    e_team_4 = 4,
    e_team_2 = 5,
    e_team_1 = 6,
    e_team_sign_end = 7,
    e_team_32_end = 8,
    e_team_end = 9,
    e_team_nextbegin = 10,
};

//战队申请
struct TeamPendingMember
{
	TeamPendingMember(Player * pl = NULL, UInt32 ot = 0): player(pl), opTime(ot) {}
	Player * player;
	UInt32 opTime;
};

struct TeamSkillData
{
	TeamSkillData(UInt8 s = 0, UInt8 l = 0, UInt32 e = 0)
		: skillId(s), level(l), extra(e)
	{
	}

	UInt8  skillId;
	UInt8  level;	//如果 = 0， 表示此技能尚未被激活
	UInt32 extra;
};
typedef std::map<UInt8, TeamSkillData> TeamSkills;

struct TeamArenaData
{
    TeamArenaData()
        : id(0), score(0), level(1), count(0), inArena(0), maxRank(0), lastRank(0), inspireLvl(0), overTime(0), leader(NULL)
    {
        overTime = TimeUtil::SharpWeek(1);
        memset( members, 0, sizeof(members) );
        memset( stamps, TEAMARENA_NONE, sizeof(stamps) );
        memset( scores, 0, sizeof(scores) );
    }
    UInt64 id;
    std::string name;
    UInt32 score;       //战队总积分
    UInt8 level;        //战队等级
    UInt8 count;        //战队成员人数
    UInt8 inArena;      //是否参加组队跨服战 0无 1参加 2淘汰
    UInt16 maxRank;      //历史最高名次
    UInt16 lastRank;     //上周名次
    UInt8 inspireLvl;   //每周鼓舞等级
    UInt32 overTime;    //每周结束时间
    Player* leader;     //战队长
    Player* members[TEAMARENA_MAXMEMCNT]; //战队成员
    UInt8 stamps[TEAMARENA_MAXMEMCNT];   //成员印记
    UInt32 scores[TEAMARENA_MAXMEMCNT];  //成员个人积分
	TeamSkills teamskills;      //战队技能
	std::map<UInt64, TeamPendingMember> pendingMap;

	void loadSkillFromDB(DBTeamArenaSkill&);
    void buildTeamSkill();
    void appendListInfo(Stream&);
    void updateSkillToDB(TeamSkillData*);
    bool checkTimeOver(UInt32 now = TimeUtil::Now());
    void updateToDB();
    UInt8 getLastRank();
    void broadcastTeam(Stream& st);
    void purgePending();
    void broadcastPendingMemberInfo(TeamPendingMember& tpm);

    inline UInt64 getId() { return id; }
    inline std::string& getName() { return name; }
    inline UInt8 GetLev() { return level; }
    inline bool isFull() { return count >= TEAMARENA_MAXMEMCNT; }
    inline void setArenaState(UInt8 s)
    {
        if(s >= TEAMARENA_ENTER_MAX)
            return;
        inArena = s;
    }
    inline bool isInArena() { return inArena == TEAMARENA_ENTER; }
    inline TeamSkillData * getOneSkill(UInt8 skillId)
    {
        TeamSkills::iterator it = teamskills.find(skillId);
        if(it != teamskills.end())
            return &it->second;
        return NULL;
    }
    inline UInt8 getMemberLvl(Player * pl)
    {
        if(pl == NULL) return 0;
        for(UInt8 i = 0; i < count; ++ i)
        {
            if(members[i] == pl)
                return countLevel(scores[i]);
        }
        return 0;
    }
    inline void addScore(UInt32 sco)
    {
        score += sco;
        level = countLevel(score);
    }
    inline UInt8 countLevel(UInt32 sco)
    {
        UInt8 lvl = 0;
        if(sco < 10)
            lvl = 1;
        else if(sco < 35)
            lvl = 2;
        else if(sco < 75)
            lvl = 3;
        else if(sco < 130)
            lvl = 4;
        else if(sco < 200)
            lvl = 5;
        else if(sco < 285)
            lvl = 6;
        else if(sco < 385)
            lvl = 7;
        else if(sco < 500)
            lvl = 8;
        else if(sco < 630)
            lvl = 9;
        else
            lvl = 10;
        return lvl;
    }
};

//战报返回：战队成员数据
struct TeamPlayerReportData
{
    TeamPlayerReportData()
    {
        memset( playerId, 0, sizeof(playerId) );
        memset( heroId, 0, sizeof(heroId) );
        memset( stamps, 0, sizeof(stamps) );
        memset( battlePoint, 0, sizeof(battlePoint) );
    }
    UInt64 playerId[TEAMARENA_MAXMEMCNT];
    std::string name[TEAMARENA_MAXMEMCNT];
    UInt8 heroId[TEAMARENA_MAXMEMCNT];
    UInt8 stamps[TEAMARENA_MAXMEMCNT];
    UInt32 battlePoint[TEAMARENA_MAXMEMCNT];

    inline void sortByBattlePoint(TeamPlayerReportData& tprd)
    {
        int i, j, k;
        int sort[TEAMARENA_MAXMEMCNT] = {0};
        for(i = k = 0; i < TEAMARENA_MAXMEMCNT; ++ i)
        {
            for(j = 0; j < TEAMARENA_MAXMEMCNT; ++ j)
            {
                if(battlePoint[j] < battlePoint[k])
                    k = j;
            }
            battlePoint[k] += (1<<31);
            sort[i] = k;
        }
        for(i = 0; i < TEAMARENA_MAXMEMCNT; ++ i)
            battlePoint[i] -= (1<<31);
        j = TEAMARENA_MAXMEMCNT - 1;
        for(i = 0; i < TEAMARENA_MAXMEMCNT; ++ i, -- j)
        {
            tprd.playerId[i] = playerId[sort[j]];
            tprd.name[i] = name[sort[j]];
            tprd.heroId[i] = heroId[sort[j]];
            tprd.stamps[i] = stamps[sort[j]];
            tprd.battlePoint[i] = battlePoint[sort[j]];
        }
    }
};

struct TeamPlayerBattleReport
{
    TeamPlayerBattleReport()
    {
        memset( won, 0, sizeof(won) );
        memset( battleId, 0, sizeof(battleId) );
    }
    TeamPlayerReportData tprd1;
    TeamPlayerReportData tprd2;
	UInt8 won[TEAMARENA_MAXMEMCNT];
    UInt32 battleId[TEAMARENA_MAXMEMCNT];

    inline void readReport(BinaryReader& brd)
    {
        UInt8 memCnt = 0;
        brd >> memCnt;
        for(UInt8 i = 0; i < memCnt && i < TEAMARENA_MAXMEMCNT; ++ i)
        {
            brd >> tprd1.playerId[i] >> tprd1.name[i] >> tprd1.heroId[i];
            brd >> tprd1.battlePoint[i] >> tprd1.stamps[i];

            brd >> tprd2.playerId[i] >> tprd2.name[i] >> tprd2.heroId[i];
            brd >> tprd2.battlePoint[i] >> tprd2.stamps[i];
            brd >> battleId[i] >> won[i];
        }
    }
    inline void appendReport(Stream& st)
    {
        UInt8 win = 0;  //己方对阵胜利次数
        for(UInt8 j = 0; j < TEAMARENA_MAXMEMCNT; ++ j)
        {
            st << tprd1.name[j];
            st << tprd1.stamps[j];
            st << tprd1.heroId[j];
            st << tprd1.battlePoint[j];
            if(won[j] >= 1)
                win |= (1 << j);
        }
        for(UInt8 j = 0; j < TEAMARENA_MAXMEMCNT; ++ j)
        {
            st << tprd2.name[j];
            st << tprd2.stamps[j];
            st << tprd2.heroId[j];
            st << tprd2.battlePoint[j];
        }
        st << win;
        for(UInt8 j = 0; j < TEAMARENA_MAXMEMCNT; ++ j)
            st << battleId[j];
    }
};

struct TeamPriliminaryBattle
{
    TeamPriliminaryBattle(): won(0), type(0), otherHeroId(0), battleTime(0) {}
	UInt8 won;
	UInt8 type;
	UInt8 otherHeroId;      //战队长主将id
	std::string otherName;
	UInt32 battleTime;
    TeamPlayerBattleReport tpbr;

    inline void append(Stream& st)
    {
        const static UInt8 won_mod[] = {1, 0, 1, 0, 1};
        st << won_mod[won];
        st << otherName;
        tpbr.appendReport(st);
    }
};

struct TeamArenaPlayer
{
	TeamArenaPlayer(): group(0xFF) {}
	UInt8 group;
	std::string realName;
	std::vector<TeamPriliminaryBattle> battles;
};

//玩家押注信息
struct PlayerBetInfo
{
    PlayerBetInfo() {}

	std::vector<BetInfo> betList[7][2];
};

struct TeamEliminationPlayer
{
	TeamEliminationPlayer(): id(0), level(0), inspireLvl(0), battlePoint(0), leaderFid(0), support(0) {}
	UInt64 id;
	UInt8 level;
	UInt8 inspireLvl;
    UInt32 battlePoint;
	UInt8 leaderFid;    //战队长主将id
    UInt32 support;
	std::string name;
    TeamPlayerReportData tprd;
	std::map<Player *, UInt8> betMap;

    ~TeamEliminationPlayer()
    {
        betMap.clear();
    }
	void calcBet(bool, const char *);
	void resetBet();
};
typedef TeamEliminationPlayer TeamPreliminaryPlayer;

struct TeamEliminationBattle
{
    UInt8 wonFlag;
    std::vector<TeamPlayerBattleReport> battleId;
	UInt8 winner(UInt8 idx);
	UInt8 winCount(UInt8 idx);

	TeamEliminationBattle() {}
	inline void reset()
	{
		battleId.clear();
	}
};

typedef std::list<TeamPreliminaryPlayer> TeamPreliminaryPlayerList;
typedef TeamPreliminaryPlayerList::iterator TeamPreliminaryPlayerListIterator;
typedef std::map<UInt64, TeamPreliminaryPlayerListIterator> TeamPreliminaryPlayerListMap;
typedef std::map<TeamArenaData *, TeamArenaPlayer> TeamArenaPlayerMap;

struct TeamPlayerLess
{
     bool operator()(const TeamPreliminaryPlayerListIterator p1, const TeamPreliminaryPlayerListIterator p2) const
     {
         return p1->battlePoint > p2->battlePoint;
     }
};
typedef std::multiset<TeamPreliminaryPlayerListIterator, TeamPlayerLess> TeamPreliminaryPlayersSet;

//历届冠军结构信息
struct LeaderTeam
{
    LeaderTeam()
    {
        memset( memberLvl, 0, sizeof(memberLvl) );
        memset( memberBp, 0, sizeof(memberBp) );
        memset( heroId, 0, sizeof(heroId) );
    }
    std::string teamName;
    std::string leaderName;
    std::string memberName[TEAMARENA_MAXMEMCNT];
    UInt8 memberLvl[TEAMARENA_MAXMEMCNT];
    UInt32 memberBp[TEAMARENA_MAXMEMCNT];
    UInt8 heroId[TEAMARENA_MAXMEMCNT];
};


class TeamArenaMgr
{
    public:
        TeamArenaMgr():
            _loaded(false), _notified(0), _session(0), _progress(0), _status(0), _round(0), _nextTime(0)
        {
            memset(_teamsCount, 0, sizeof(_teamsCount));
            memset(_winnerColor, 0, sizeof(_winnerColor));
            memset(_finalIdx, 0, sizeof(_finalIdx));
        }
        ~TeamArenaMgr() {}

	    inline bool active()
        { return _loaded && (_session != 0x8001 || (_progress != 0)) && World::getArenaState() == ARENA_XIANJIE_ZHIZUN; }
        inline bool isOpen() { return World::getArenaState() == ARENA_XIANJIE_ZHIZUN; }
        inline void setSession(UInt16 s) { _session = s; }
        void loadFromDB(DBTeamArenaData&);
        void pushBetFromDB( Player * player, UInt8 round, UInt8 state, UInt8 group, UInt8 recieved, UInt16 pos, UInt8 type );
        bool createTeam(Player *, std::string&);
        void dismissTeam(Player *);
        bool addTeamMember(Player *, Player *);
        void fireTeamMember(Player *, UInt64);
        void handoverLeader(Player *, UInt64);
        void setMemberPosition(Player *, UInt64, UInt64, UInt64, std::string&);
        void leaveTeamArena(Player *);
        void upgradeTeamSkill(Player *, UInt8, UInt32);
        void inspireTeam(Player *);
        void enterArena(Player *);
        void commitLineup1(Player *);
        void commitLineup(Player *);
        void championWorship(Player*, UInt8);
        void resetTeamState(TeamArenaData *);
        void addTeamScore(TeamArenaData *, UInt8, bool);
        void getTeamInfo(Player *);
        void sendTeamInfo(TeamArenaData *, Player * = NULL);
        void sendReqInfo(Player *, UInt8);

        void teamArenaEntered(TeamArenaData *, UInt8, const std::string&);
        void pushPreliminary(BinaryReader& br);
        void readFrom( BinaryReader& brd );
        void readTeams(BinaryReader& brd);
        void readPreTeams(BinaryReader& brd);
        void readHistories(BinaryReader& brd);
        void readElimination(BinaryReader& brd);
        void calcFinalBet(int i);
        void calcBet(TeamPreliminaryPlayer& pp, UInt16 pos, UInt8 state, UInt8 group, bool won, const char * t);
        void sendTeamMail(TeamArenaData *, const std::string&, const std::string&);
        void giveTeamLastAward(TeamArenaData * tad, UInt8 group, UInt8 idx, UInt8 type);
        UInt8 bet1( Player * player, UInt8 state, UInt8 group, UInt64 tid, UInt8 type );
        UInt8 bet2( Player * player, UInt8 state, UInt8 group, UInt16 pos, UInt8 type );
        void updateBattlePoint(BinaryReader& brd);
        void updateInspireLevel(BinaryReader& brd);
        void updateLeaderBoard(BinaryReader& brd);
        void updateLastRank(BinaryReader& brd);
        void setTeamMaxRank(TeamArenaData *, UInt8, UInt8);
        void resetTeamLastRank(TeamArenaData * tad);

        void sendLeaderBoard(Player*);
        void sendLastLeaderBoard(Player*);
        void sendActive(Player* pl);
        void sendStatus(Player* pl);
        void sendEnter(Player* pl);
        void sendPreliminary(Player* player, UInt8 flag, UInt16 start, UInt8 len);
        void sendElimination( Player * player, UInt8 type, UInt8 group );
        void updateSuport(UInt8 type, UInt8 flag, UInt16 pos);

        void applyTeam(Player *, std::string&);
        void listTeamPending(Player *);
        void acceptApply(Player *, UInt64);
        void declineApply(Player *, UInt64);
        void listAllTeam(Player *, UInt16, UInt8);
        void searchTeam(Player *, std::string&, UInt8);
    private:
        bool _loaded;
        UInt8 _notified;
        UInt16 _session;
        UInt8 _winnerColor[3];
        std::string _winner[3];
        UInt8 _progress;
        UInt8 _status;
        UInt8 _round;
        UInt32 _nextTime;
        UInt32 _teamsCount[3];
	    TeamArenaPlayerMap _teams;
        TeamEliminationPlayer _finals[2][32];   //晋级赛队伍
        UInt64 _finalIdx[2][6][32];
        TeamEliminationBattle _finalBattles[2][31]; //晋级赛战报
        TeamPreliminaryPlayerListMap _preliminaryPlayers;  //进32强战报
        TeamPreliminaryPlayerList _preliminaryPlayers_list;
        TeamPreliminaryPlayersSet _preliminaryPlayers_list_set;
        std::map<UInt16, LeaderTeam> _leaderBoard;
        std::vector<UInt64> _lastRankTeam;

        std::map<Player *, PlayerBetInfo> _playerBet;

public:
    Stream _readbuf;
};


typedef GGlobalObjectManagerT<TeamArenaData, UInt64> GlobalTeamArenaData;
extern GlobalTeamArenaData globalTeamArena;
typedef GGlobalObjectManagerIStringT<TeamArenaData> GlobalNamedTeamArenaData;
extern GlobalNamedTeamArenaData globalNamedTeamArena;
extern TeamArenaMgr teamArenaMgr;

}

#endif // _ARENATEAM_H_

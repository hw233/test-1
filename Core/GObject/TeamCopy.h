#ifndef _TEAM_COPY_H_
#define _TEAM_COPY_H_

#include "Config.h"

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
class Player;

#define TEAMCOPY_MAXTYPECNT 2 
#define TEAMCOPY_MAXCOPYCNT 6 
#define TEAMCOPY_MAXMEMCNT  3 
#define TEAMCOPY_MAXPAGECNT 6 
#define TEAMCOPY_MAXPWDCNT  6 
#define TEAMCOPY_MAXTEAMCNT 600 

static const UInt32 TEAMCOPY_EXTRAREWARDTM1 = 7 * 3600;
static const UInt32 TEAMCOPY_EXTRAREWARDTM2 = 11 * 3600;

struct TeamData
{
    TeamData()
    {
        id = 0;
        leader = NULL;
        memset( members, 0, sizeof(members) );
        count = 0;
        t = 0;
        upLevel = 0;
        dnLevel = 0;
    }

    //CopyId + (CopyTeamsIdx << 8)
    UInt32 id;
    Player* leader;
    Player* members[TEAMCOPY_MAXMEMCNT];
    UInt8 count;
    UInt8 t;
    UInt8 upLevel;
    UInt8 dnLevel;
    std::string pwd;
};

struct TeamCopyAwards
{
    TeamCopyAwards(UInt32 id, UInt32 num) : id(id), num(num) {}
    UInt32 id;
    UInt32 num;
};

class TeamCopyPlayerInfo
{
    public:
        static const UInt8 _needRoll;
        static const UInt8 _hasRoll;
        static std::vector<TeamCopyAwards> _awards[TEAMCOPY_MAXCOPYCNT];
        static void addTeamCopyAwardCfg(UInt8 rollId, UInt32 awardId, UInt32 awardCnt);
        static void clearTeamCopyAwardCfg();

    private:
        bool m_pass[TEAMCOPY_MAXTYPECNT][TEAMCOPY_MAXCOPYCNT];
        UInt8 m_passTimes[TEAMCOPY_MAXTYPECNT][TEAMCOPY_MAXCOPYCNT];
        UInt32 m_vTime[TEAMCOPY_MAXTYPECNT][TEAMCOPY_MAXCOPYCNT];
        Player* m_owner;
        UInt8 m_maxPass;
        UInt8 m_roll;
        UInt8 m_rollId;
        UInt32 m_awardId;
        UInt32 m_awardCnt;

    public:
        TeamCopyPlayerInfo(Player* owner);

        void setPassFromDB(UInt8 copyId, UInt8 t, bool pass);
        void setPassTimesFromDB(UInt8 copyId, UInt8 t, UInt8 passTimes, UInt32 vTime);
        void rollAward(UInt8 type);
        bool getAward();
        void sendAwardInfo();
        void loadAwardInfoFromDB(UInt8 copyId, UInt8 roll, UInt32 awardId, UInt32 awardCnt);
        void setAwardRoll(UInt8 copyId);

        bool getPass(UInt8 copyId, UInt8 t);
        UInt8 getPassTimes(UInt8 copyId, UInt8 t);
        void setPass(UInt8 copyId, UInt8 t, bool pass, bool notify = false);
        void setPassTimes(UInt8 copyId, UInt8 t, UInt8 passTimes, UInt32 vTime);
        void incPass(UInt8 copyId, UInt8 t);
        bool checkTeamCopyPlayer(UInt8 copyId, UInt8 t);
        void checkCopyPass(UInt32 taskId);
        void reqTeamCopyInfo();
        void sendUpdateTeamCopyInfo(UInt8 copyId);
        void resetTCPlayer();
};


class TeamCopy
{
    public:
        static const UInt8 lvls[TEAMCOPY_MAXCOPYCNT];

    public:
        TeamCopy();
        ~TeamCopy();

        bool enterTeamCopy(Player* pl, UInt8 copyId, UInt8 t);
        bool leaveTeamCopy(Player* pl);
        void reqTeamList(Player* pl, UInt32 start, UInt8 count, UInt8 type);
        void reqTeamInfo(Player* pl);
        void updateTeamInfo(Player* pl);
        UInt32 createTeam(Player* pl, std::string pwd, UInt8 upLevel, UInt8 dnLevel);
        UInt32 joinTeam(Player* pl, UInt32 teamId, std::string pwd);
        void leaveTeam(Player* pl);
        void teamKick(Player* pl, UInt64 playerId);
        void reQueueTeam(Player* pl, UInt8 idx0, UInt8 idx1);
        void handoverLeader(Player* pl, UInt64 playerId);
        void teamBattleStart(Player* pl);
        void sendTeamCopyPageUpdate(UInt8 copyId, UInt8 t, UInt32 startIdx, UInt32 endIdx);
        bool quikJoinTeam(Player* pl);

        bool checkTeamCopy(Player* pl, UInt8 copyId, UInt8 t);
        void incLevelTeamCnt(UInt8 copyId, UInt8 t, UInt8 upLevel, UInt8 dnLevel);
        void decLevelTeamCnt(UInt8 copyId, UInt8 t, UInt8 upLevel, UInt8 dnLevel);
        UInt16 getIdleTeamNumber(UInt8 copyId, UInt8 t);
        void recycleTeamNumber(UInt8 copyId, UInt8 t, UInt16 number);
        void addTeamCopyNpc(UInt8 copyId, UInt8 t, UInt16 location, UInt32 npcId);
        void sendBattleReport(TeamData* td, GData::NpcGroup* ng, Battle::BattleSimulator& bsim, UInt32& rptid);

        void process(UInt32 now);
        void sendDaily(Player* player);

    private:
        typedef std::vector<TeamData*> CopyTeams;
        typedef std::map<UInt32, TeamData*> AllCopyTeams;
        typedef std::map<UInt32, TeamData*>::iterator AllCopyTeamsIterator;

        typedef std::map<UInt8, UInt32> LevelTeamCnt;
        typedef std::map<UInt8, UInt32>::iterator LevelTeamCntIterator;
        typedef std::vector<UInt16> TeamNumber;
        typedef std::vector<UInt32> TeamCopyNpcId;

        struct TeamCopyNpc
        {
            UInt16 location;
            TeamCopyNpcId npcId;
        };

        typedef std::vector<Player*> TeamCopyPlayer;

        // [t难度][副本idx]
        TeamNumber m_tnIdle[TEAMCOPY_MAXTYPECNT][TEAMCOPY_MAXCOPYCNT];
        TeamCopyPlayer m_playerIdle[TEAMCOPY_MAXTYPECNT][TEAMCOPY_MAXCOPYCNT];
        CopyTeams* m_copysTeam[TEAMCOPY_MAXTYPECNT][TEAMCOPY_MAXCOPYCNT];
        LevelTeamCnt m_clvTeamCnt[TEAMCOPY_MAXTYPECNT][TEAMCOPY_MAXCOPYCNT];
        AllCopyTeams m_allCopyTeams;
        TeamCopyNpc m_tcNpcId[TEAMCOPY_MAXTYPECNT][TEAMCOPY_MAXCOPYCNT];
        bool _notify1;
        bool _notify2;
        bool _isDoubleExp;
};

extern TeamCopy* teamCopyManager;

}


#endif


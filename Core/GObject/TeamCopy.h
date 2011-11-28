#ifndef _TEAM_COPY_H_
#define _TEAM_COPY_H_

#include "Config.h"

namespace Battle
{
    class BattleSimulator;
}

namespace GObject
{
class Player;

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
        pwd = "";
    }

    //CopyId + (CopyTeamsIdx << 8)
    UInt32 id;
    Player* leader;
    Player* members[3];
    UInt8 count;
    UInt8 t;
    UInt8 upLevel;
    UInt8 dnLevel;
    std::string pwd;
};

class TeamCopy
{
    static const UInt8 maxCopyCnt = 6;
    static const UInt8 maxMemCnt = 3;
    static const UInt8 maxPageLen = 6;
    static const UInt8 maxPWDLen = 8;
    static const UInt8 lvls[6];
    static const UInt32 maxTeamCnt = 600;

    public:
        TeamCopy();
        ~TeamCopy();

        bool enterTeamCopy(Player* pl, UInt8 copyId, UInt8 t);
        bool leaveTeamCopy(Player* pl);
        void reqTeamList(Player* pl, UInt32 start, UInt32 count, UInt8 type);
        void reqTeamInfo(Player* pl);
        UInt32 createTeam(Player* pl, std::string pwd, UInt8 upLevel, UInt8 dnLevel);
        UInt32 joinTeam(Player* pl, UInt32 teamId, std::string pwd);
        void leaveTeam(Player* pl);
        void teamKick(Player* pl, UInt64 playerId);
        void reQueueTeam(Player* pl, UInt8 idx0, UInt8 idx1, UInt8 idx2);
        void handoverLeader(Player* pl, UInt64 playerId);
        void teamBattleStart(Player* pl);
        void sendTeamCopyPageUpdate(UInt8 copyId, UInt8 t, UInt32 tdIdx);

        bool checkTeamCopy(Player* pl, UInt8 copyId, UInt8 t);
        void incLevelTeamCnt(UInt8 copyId, UInt8 t, UInt8 upLevel, UInt8 dnLevel);
        void decLevelTeamCnt(UInt8 copyId, UInt8 t, UInt8 upLevel, UInt8 dnLevel);
        UInt16 getIdleTeamNumber(UInt8 copyId, UInt8 t);
        void recycleTeamNumber(UInt8 copyId, UInt8 t, UInt16 number);

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
        TeamNumber m_tnIdle[2][6];
        TeamCopyPlayer m_playerIdle[2][6];
        CopyTeams* m_copysTeam[2][6];
        LevelTeamCnt m_clvTeamCnt[2][6];
        AllCopyTeams m_allCopyTeams;
        TeamCopyNpc m_tcNpcId[2][6];
};

extern TeamCopy* teamCopyManager;

}


#endif


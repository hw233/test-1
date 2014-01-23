#ifndef _PETTEAMCOPY_H_
#define _PETTEAMCOPY_H_

#include "Config.h"
#include "Common/Stream.h"

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

#define PETTEAMCOPY_MAXTYPECNT 2
#define PETTEAMCOPY_MAXCOPYCNT 5
#define PETTEAMCOPY_MAXMEMCNT  3
#define PETTEAMCOPY_MAXPAGECNT 6
#define PETTEAMCOPY_MAXQUALITYCNT 4
#define PETTEAMCOPY_MAXTEAMCNT 600
enum
{
    ORDINARYCOPY = 0,
    DIFFICULTYCOPY
};

struct KillMonsterLog
{
    std::string playerName;
    std::string monsterName;
    std::vector<UInt32> items;
};

struct PetTeamData
{
    PetTeamData()
    {
        id = 0;
        index = 0;
        type = 0;
        quality = 0;
        leader = NULL;
        count = 0;
        memset(members, 0, sizeof(members));
        memset(formation, 0, sizeof(formation));
        NPCId = 0;
        mark = false;
        start = false;
        useMoney = false;
    }

    UInt32 id;                                  // 宠物组队副本 队伍id
    UInt8 index;                                // 当前宠物组队副本index
    UInt8 type;                                 // 普通珍兽 or 狂化珍兽
    UInt8 quality;                              // 队伍品阶
    Player* leader;                             // 队长
    UInt8 count;                                // 成员人数
    Player* members[PETTEAMCOPY_MAXMEMCNT];     // 成员
    UInt8 formation[PETTEAMCOPY_MAXMEMCNT];     // 阵形状态
    UInt32 NPCId;                               // npcGroupId
    bool mark;                                  // 是否设置阵形标志
    bool start;                                 // 是否倒计时开始标志
    bool useMoney;                              // 创建房间是否是否花费仙石标志
};

class PetTeamCopyPlayerInfo
{
    public:
        PetTeamCopyPlayerInfo(Player* owner);

        typedef std::vector<UInt32> PlayerNpcId;
        PlayerNpcId m_playerNpcId[PETTEAMCOPY_MAXTYPECNT][PETTEAMCOPY_MAXCOPYCNT];

        void setNpcGroupIdFromDB(UInt8 copyId, UInt8 type, UInt32 npcGroup1Id, UInt32 npcGroup2Id, UInt32 npcGroup3Id);
    private:
        Player* m_owner;
};

class PetTeamCopy
{
    public:
        static const UInt8 lvls[PETTEAMCOPY_MAXCOPYCNT];

    public:
        PetTeamCopy();
        ~PetTeamCopy();

        void reqTeamList(Player* pl);
        void reqMonsterInfo(Player* pl);
        void reqTeamInfo(Player* pl);
        void updateTeamInfo(Player* pl, UInt32 teamId);
        void delTeamInfo(Player* pl, UInt32 teamId);
        void sendPetTeamCopyPageUpdate(UInt8 opt, UInt32 teamId);
        void teamInfo(Player* pl, Stream& st);

        void enter(Player* pl);
        void quit(Player* pl);
        bool enterTeamCopy(Player* pl, UInt8 copyId, UInt8 t);
        void refreshMonster(Player* pl);
        void addMonster(Player* pl, UInt8 copyId, UInt8 type, UInt32 oldNPCId);
        UInt32 createTeam(Player* pl, UInt32 NPCId, UInt32 npcGroupId);
        UInt32 joinTeam(Player* pl, UInt32 teamId);
        void teamKick(Player* pl, UInt64 playerId, UInt8 mark=0);
        void leaveTeam(Player* pl, UInt8 mark=0/*0:被踢退出队伍；1:主动退出队伍；2:退出游戏; 3:顶号退出游戏*/);
        void dismissTeam(Player* pl, UInt8 mark=0);
        void inviteFriend(Player* pl, UInt64 friendId);
        void refuseJoin(Player* pl, UInt64 leaderId);
        void reqStart(Player* pl, UInt8 opt);
        void teamBattleStart(Player* pl);
        void sendBattleReport(PetTeamData* td, GData::NpcGroup* ng, Battle::BattleSimulator& bsim, UInt32& rptid);

        bool checkPetTeamCopy(Player* pl, UInt8 copyId, UInt8 t);
        UInt16 getIdleTeamNumber(UInt8 copyId, UInt8 t);
        void recycleTeamNumber(UInt8 copyId, UInt8 t, UInt16 number);
        void setFormation(Player* pl, UInt8 pos1=0, UInt8 pos2=0, UInt8 pos3=0);
        void addPetTeamCopyNpc(UInt8 copyId, UInt8 t, UInt8 quality, UInt32 npcId);

        void pushLog(UInt8 color1, const std::string& playerName, UInt8 color2, const std::string& monsterName, const std::vector<UInt32>& i);
        void pushLog(const std::string& playerName, const std::string& monsterName, const std::string& its);

    private:
        typedef std::map<UInt32, PetTeamData*> AllTeams;                                    // map<队伍Id， 队伍信息>
        typedef std::map<UInt32, PetTeamData*>::iterator AllTeamsIterator;

        typedef std::vector<UInt16> PetTeamNumber;
        typedef std::vector<Player*> PetTeamCopyPlayer;

        std::list<KillMonsterLog> _logs;
        // 配置
        typedef std::map<UInt8, std::vector<UInt32>> PetTeamCopyNpcId;                       // map<品质，怪物> 用于刷怪
        typedef std::map<UInt8, std::vector<UInt32>>::iterator PetTeamCopyNpcIdIterator;
        typedef std::map<UInt32, UInt8> PetTeamCopyNpc;                                      // map<怪物Id，品质>
        typedef std::map<UInt32, UInt8>::iterator PetTeamCopyNpcIterator;               

        // [t难度][副本idx]
        PetTeamNumber m_ptnIdle[PETTEAMCOPY_MAXTYPECNT][PETTEAMCOPY_MAXCOPYCNT];
        AllTeams m_allTeams;
        PetTeamCopyPlayer m_playerIdle;

        PetTeamCopyNpcId m_ptcNpcId[PETTEAMCOPY_MAXTYPECNT][PETTEAMCOPY_MAXCOPYCNT];
        PetTeamCopyNpc m_petTeamCopyNpc;
};

extern PetTeamCopy* petTeamCopyManager;
}

#endif


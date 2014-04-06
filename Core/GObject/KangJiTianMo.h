#ifndef _KANGJITIANMO_H_
#define _KANGJITIANMO_H_

#include "Config.h"
#include "Common/Stream.h"
#include "Common/Mutex.h"

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

#define WEEK_TIME       7*86400
#define TEAM_MAXMEMCNT  3

class Player;

struct TeamMemberData
{
    UInt8 refCount;
    UInt32 teamId;                              // 队伍id
    UInt8 memCnt;                               // 成员个数
    Player* members[TEAM_MAXMEMCNT];            // 成员
    UInt8 index;                                // 怪物索引
    UInt32 NPCId;                               // 怪物Id
    UInt8 start;                                // 是否战斗中（0：非战斗，1：战斗中）

    TeamMemberData() : refCount(0), teamId(0), memCnt(0), index(0), NPCId(0), start(0) { memset(members, 0, sizeof(members)); }
};

class KangJiTianMo 
{
    public:
        KangJiTianMo();
        ~KangJiTianMo();

    private:
        
        struct InactiveSort
        {
            GObject::Player* player;
            UInt8 level;
            UInt32 power;

            InactiveSort() : player(NULL), level(0), power(0){}
        };

        struct lt_sortA
        {
            bool operator()(const InactiveSort& a, const InactiveSort& b) const { return a.power > b.power || (a.power == b.power && a.level > b.level); }
        };

        typedef std::multiset<InactiveSort, lt_sortA> InactiveSortType;

        typedef std::vector<UInt64> InactiveMember;

        typedef std::vector<UInt32> InactiveMemberMark;

    public:
        InactiveMember _InactiveMember;   // 普通回流玩家
        InactiveMemberMark _Mark;

    private:
        InactiveSortType _CommonSort;     // 普通回流玩家排序

        Mutex _mutex;
    
    private:
        void TeamMemberInfo(Player* pl, Stream& st);
        void UpdateTeamMember(Player* pl);
        void DelBattleRoomInfo(Player* pl);
        void GetBattleRoomInfo(Player* pl, Stream& st);
        void SendBattleReport(TeamMemberData* tmd, GData::NpcGroup* ng, Battle::BattleSimulator& bsim, UInt32& rptid);

    public:
        void AddInactiveMemberFromDB(UInt64 playerId);
        void AddInactiveMember();
        void ClearInactiveMember();
        void RandInactiveMember(Player* pl, UInt8 type);
        void GetKJTMStatus(Player* pl);
        void GetKJTMData(Player* pl);
        void SetInactiveSort(Player* member);
        void SendInactiveSort(Player* pl, UInt8 type);
        void ClearInactiveSort();
        void AddTeamMember(UInt32 teamId, Player* p1, Player* p2, Player* p3);
        void CreateTeamMember(Player* pl);
        void JoinTeamMember(UInt64 playerId, UInt64 applicantId);
        void LeaveTeamMember(Player* pl);
        void TeamMemberKick(Player* leader, Player* pl);
        void GetKillNPCStatus(Player* pl);
        void CreateBattleRoom(Player* pl, UInt8 index);
        void InviteTeamMember(Player* pl);
        void JoinBattleRoom(Player* pl, UInt64 leaderId);
        void LeaveRoom(Player* pl, UInt8 opt=0/*0:正常离开；1:非正常离开*/);
        void LeaveBattleRoom(Player* pl, UInt8 opt=0/*0:正常离开；1:非正常离开*/);
        void DismissBattleRoom(Player* pl);
        void NoticeBattle(Player* pl);
        void StartBattle(Player* pl);
};

extern KangJiTianMo* KJTMManager;
}

#endif

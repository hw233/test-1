#ifndef _CLANCITYBATTLE_H_
#define _CLANCITYBATTLE_H_

#include "Common/Platform.h"
#include <vector>
#include <map>

namespace GObject
{
    class Player;
    class Stream;
    class Clan;
    struct CCBSkill;
    class ClanCityBattle;

    enum CCBPlayerType
    {
        e_player = 0,
        e_npc = 1,
        e_skill = 2
    };

    // 玩家
    struct CCBPlayer
    {
        CCBPlayerType type;
        union
        {
            Player* player;
            UInt32 npcId;
            UInt8 skillid;
        } fgt;
        UInt8 side;
        UInt8 pos;
        UInt32 score;
        UInt32 cd;
        UInt16 weary;

        CCBPlayer(CCBPlayerType t) : type(t), fgt(0), side(0), pos(0), score(0), cd(0), weary(0) {}
        void sendInfo();
        bool challenge(CCBPlayer* other);
    };

    typedef std::vector<CCBPlayer*> CCBPlayerList;
    typedef std::map<Player*, CCBPlayer*> CCBPlayerMap;
    // 据点
    struct CCBSpot
    {
        UInt8 id;
        bool  canAtk;
        UInt16 hp;
        CCBPlayerList waiters[2];
        CCBPlayerList alive[2];
        CCBPlayerList dead[2];
        CCBSkill skills[2][3];
        CCBPlayerList battler[2];
        ClanCityBattle* clancity;

        CCBSpot(ClanCityBattle* cc) : id(0), canAtk(false), hp(0), clancity(cc) {}
        void playerEnter(CCBPlayer* pl);
        void playerLeave(CCBPlayer* pl);
        void prepare();
        void handleBattle();
        void end();
        void erasePl(CCBPlayerList& list, CCBPlayer* pl);
        void moveAll(CCBPlayerList& list, UInt16 spot);
    };

    struct CCBClan
    {
        Clan* clan;
        UInt32 score;
        CCBSkill skills[3];

        CCBClan() : clan(NULL), score(0) {}
    };
    struct bpGreater
    {
        bool operator() (const CCBClan* first, const CCBClan* second)
        {
            return first->score > second->score;
        }
    };

    struct CCBSkill
    {
        UInt8 id;
        UInt8 lvl;
        UInt8 times;

        CCBSkill() : id(0), lvl(0), times(0) {}
        void init() { times = 0; }
        void doSkillEffect(CCBSpot& spot);
        void doSkillEffect(CCBClan& clan);
        void doSkillEffect(CCBPlayer* pl);
    };
    typedef std::map<Clan*, CCBClan*> CCBClanMap;
    typedef std::set<CCBClan*, bpGreater> CCBClanSort;

    class ClanCityBattle
    {
    private:
        CCBPlayerMap m_players;
        CCBClanMap   m_clans;
        CCBClanSort  m_clanSort;
        CCBSpot      m_spots[7];
        UInt16       m_loc;
        UInt32       m_nextTime;
    public:
        ClanCityBattle(UInt16 loc);

        static void Init();
        inline UInt16 getLocation() { return m_loc; }
        void process(UInt32);
        void prepare(UInt16);
        void start(UInt16);
        void end();
        bool playerEnter(Player *);
        void playerLeave(Player *);

        void move(Player* player, UInt8 spot);
        void move(CCBPlayer* pl, UInt8 spot);
        void openCanAtkSpot();
    };

} // namespace GObject

#endif

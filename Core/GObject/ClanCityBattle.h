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
    class ClanCity;

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
            CCBSkill* skill;
        } fgt;
        UInt8 side;
        UInt8 pos;
        UInt8 win;
        UInt32 score;
        UInt32 cd;
        UInt32 weary;

        CCBPlayer(CCBPlayerType t) : type(t), fgt.player(0), side(0), pos(0), win(0), score(0), cd(0), weary(0) {}
        void sendInfo();
        bool challenge(CCBPlayer* other);
        bool attackNpc(CCBPlayer* npc);
        bool attackPlayer(CCBPlayer* other);
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
        CCBSkill skill_dmg[2];
        CCBSkill skill_action[2];
        CCBSkill skill_bomb[2];
        CCBPlayerList battler[2];
        ClanCity* clancity;
        CCBPlayer   bomb[2];
        CCBPlayerList npc;

        void playerEnter(CCBPlayer* pl);
        void playerLeave(CCBPlayer* pl);
        void prepare();
        void handleBattle();
        void end();
        void erasePl(CCBPlayerList& list, CCBPlayer* pl);
        void moveAll(CCBPlayerList& list, UInt16 spot);
        void fillTo100(CCBPlayerList& dst, CCBPlayerList& src, CCBPlayer* bomb);
        void makeDamage(UInt16 dmg);
    };

    struct CCBClan
    {
        Clan* clan;
        UInt32 score;
        CCBSkill skill_hp;
        CCBSkill skill_atk;
        CCBSkill skill_action;

        void doSkill(CCBPlayer* pl);
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
        void doSkillEffect(CCBPlayer* pl);
        UInt16 doSkillEffect(UInt16 dmg);
    };
    typedef std::map<Clan*, CCBClan*> CCBClanMap;
    typedef std::set<CCBClan*, bpGreater> CCBClanSort;

    class ClanCity
    {
    private:
        CCBPlayerMap m_players;
        CCBClanMap   m_clans;
        CCBClanSort  m_clanSort;
        CCBSpot      m_spots[7];
        UInt16       m_loc;
        UInt32       m_nextTime;
        UInt32       m_expTime;
        UInt8        m_type;
        UInt8        m_round;
        UInt8        m_open;
    public:
        ClanCity(UInt16 loc);

        inline UInt16 getLocation() { return m_loc; }
        void process(UInt32);
        void prepare(UInt16);
        void start(UInt16);
        void end();
        bool playerEnter(Player *);
        void playerLeave(Player *);

        void move(Player* player, UInt8 spot);
        void move(CCBPlayer* pl, UInt8 spot);
        void openNextSpot(UInt8 id);
        void handleBattle();
        void doClanSkill(CCBPlayer* pl);
        inline void setType(UInt8 type) { m_type = type; }
        void endOneRound();
        void loadFromDB();
    };

} // namespace GObject

#endif

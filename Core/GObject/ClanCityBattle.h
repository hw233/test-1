#ifndef _CLANCITYBATTLE_H_
#define _CLANCITYBATTLE_H_

#include "Common/Platform.h"
#include <vector>
#include <map>

#define CCB_OPEN_LEVEL  70

class Stream;

namespace GObject
{
    class Player;
    class Clan;
    struct CCBSkill;
    struct CCBSpot;
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
        UInt8 dead;
        UInt8 realive;
        UInt32 score;
        UInt32 realivecd;
        UInt32 movecd;
        UInt32 weary;
        UInt8 entered;

        CCBPlayer() : type(e_player), side(0), pos(0), win(0), dead(0), realive(0), score(0), realivecd(0), movecd(0), weary(0), entered(0) {}
        CCBPlayer(CCBPlayerType t) : type(t), side(0), pos(0), win(0), dead(0), realive(0), score(0), realivecd(0), movecd(0), weary(0), entered(0) 
        {
            fgt.player = NULL;
        }
        void sendInfo();
        bool challenge(CCBPlayer* other, UInt32& bid);
        bool attackNpc(CCBPlayer* npc, UInt32& bid);
        bool attackPlayer(CCBPlayer* other, UInt32& bid);
        void addReAliveCD();
        bool inReAliveCD();
        inline UInt32 getWinScore()
        {
            if(win > 1)
                return (2 + (win > 7 ? 2 : 1));
            else
                return 2;
        }
        inline UInt32 getLostScore() { return 1; }
        void writeToDB();
    };

    struct CCBSkill
    {
        UInt8 id;
        UInt8 lvl;
        UInt8 times;

        CCBSkill() : id(0), lvl(0), times(0) {}
        void init() { times = 0; }
        void clear() { lvl = 0; times = 0; }
        void doSkillEffect(CCBSpot& spot);
        bool doSkillEffect(CCBPlayer* pl);
        UInt16 doSkillEffect(UInt16 dmg);
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
        CCBPlayerList battler[2];
        CCBSkill skill_dmg[2];
        CCBSkill skill_action[2];
        CCBSkill skill_bomb[2];
        ClanCity* clancity;
        CCBPlayer   bomb[2];
        CCBPlayerList npcs;

        CCBSpot();
        void Init();
        void playerEnter(CCBPlayer* pl);
        bool playerLeave(CCBPlayer* pl);
        void prepare();
        void handleBattle();
        void end();
        bool erasePl(CCBPlayerList& list, CCBPlayer* pl);
        void moveAllToHome(CCBPlayerList& list, UInt8 spot);
        void fillTo100(CCBPlayerList& dst, CCBPlayerList& src, CCBPlayer* bomb);
        void fillBoss(CCBPlayerList& dst, CCBPlayerList& src);
        void fillNpc(CCBPlayerList& dst, CCBPlayerList& src);
        void flushNpc();
        void makeDamage(UInt16 dmg);
        UInt32 getDefCount();
        UInt32 getAtkCount();
        CCBPlayer* popAlive(CCBPlayerList& list);
        void makeAliveInfo(Stream& st);
        CCBPlayer* popOnePlayer(CCBPlayerList& list);
        void handleAllDead();
        bool canMoveTo();
        void clear();
        void reAlive(CCBPlayer* pl, bool force);

        void send(CCBPlayerList& list, Stream& st);
        void broadcast(Stream& st);
        void sendAliveInfo(CCBPlayer* pl);

        void writeToDB();
    };

    struct CCBClan
    {
        Clan* clan;
        UInt32 score;
        CCBSkill skill_hp;
        CCBSkill skill_atk;
        CCBSkill skill_action;

        CCBClan(Clan* cl);
        void doSkill(CCBPlayer* pl);
        void sendInfo(CCBPlayer* player);
        void writeToDB();
    };

    struct bpGreater
    {
        bool operator() (const CCBClan* first, const CCBClan* second);
    };

    typedef std::map<Clan*, CCBClan*> CCBClanMap;
    typedef std::multimap<Clan*, CCBPlayer*> CCBClanPlayerMap;
    typedef std::set<CCBClan*, bpGreater> CCBClanSort;

    class ClanCity
    {
    private:
        CCBPlayerMap m_players;
        CCBPlayerMap m_players_leave;
        CCBClanPlayerMap m_clanPlayers[2];
        CCBClanMap   m_clans;
        CCBClanSort  m_clanSort;
        CCBSpot      m_spots[7];
        UInt16       m_loc;
        UInt32       m_openTime;
        UInt32       m_nextTime;
        UInt32       m_expTime;
        UInt8        m_type;
        UInt8        m_recycle_round;
        UInt8        m_round;
        UInt8        m_status;
        UInt32       m_defClanId;
        UInt32       m_startTime;
        UInt32       m_endTime;
        bool         m_openFlag;
    public:
        ClanCity(UInt16 loc);

        bool isRunning();
        bool isOpen();
        void setOpenTime();
        void setOpenFlag() { m_openFlag = true; }
        inline UInt8 getStatus() { return m_status; }
        inline bool hasValidateOpenTime() { return m_openTime != 0xFFFFFFFF; }
        inline void setType(UInt8 type) { m_type = type; }
        inline UInt8 getType() { return m_type; }
        inline UInt8 getRound() { return m_round; }
        inline UInt8 getRecycleRound() { return m_recycle_round; }
        inline UInt16 getLocation() { return m_loc; }
        void process(UInt32);
        void prepare();
        void start();
        void end();
        bool playerEnter(Player *);
        bool playerLeave(Player *);
        void forceEnter(CCBPlayer* pl, UInt8 spot);

        bool isClanCityDefender(Clan* cl);
        UInt8 move(Player* player, UInt8 spot);
        void move(CCBPlayer* pl, UInt8 spot);
        void openNextSpot(UInt8 id);
        void handleBattle();
        void doClanSkill(CCBPlayer* pl);
        void endOneRound();
        void prepareOneRound();
        void checkAddExp(UInt32 curtime);
        void loadFromDB();

        void makeLeaderBoardInfo(Stream& st);
        void makeOpenStatusInfo(Stream& st);
        void sendPlayerInfo(Player* player);
        void sendClanInfo(Player* player);
        void sendAllSpotInfo(Player* player);
        void sendLeaderBoard(Player* player);
        void sendSelfSpotInfo(Player* player);
        void upClanSkill(Player* player, UInt8 sidx);
        void reAlive(Player* player, UInt8 force);
        void upSpotSkill(Player* player, UInt8 spot, UInt8 sidx);
        void sendOpenStatus(Player* pl);
        void broadcast(Stream& st);
        void makeAllSpotInfo(Stream& st);
        void giveScore(CCBPlayer*pl, UInt32 score);
        void giveAllScore(UInt8 side, UInt32 score);
        void writeToDB();
    };

    extern ClanCity* gClanCity;

} // namespace GObject

#endif

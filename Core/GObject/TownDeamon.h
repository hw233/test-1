#ifndef _TOWN_DEAMON_H_
#define _TOWN_DEAMON_H_

#include "Config.h"

namespace GData
{
    class NpcGroup;
}

namespace GObject
{

static const UInt32 TOWNDEAMONENDTM = 19 * 3600;
static const UInt32 TD_VITALITY_TIMEUNIT = 600;
static const UInt32 TD_AWARD_TIMEUNIT = 900;
//static const UInt32 TD_AWARD_TIMEUNIT = 300;
static const UInt32 TD_CHALLENGE_TIMEUNIT = 300;
//static const UInt32 TD_CHALLENGE_TIMEUNIT = 20;

class Player;
struct Lineup;

struct DeamonMonster
{
    Player* player;
    UInt32 npcId;
    UInt32 itemId;
    UInt32 itemNum;
    bool inChallenge;

    DeamonMonster()
    {
        player = NULL;
        npcId = 0;
        itemId = 0;
        itemNum = 0;
        inChallenge = false;
    }
};

struct DeamonPlayerData
{
    UInt32 challengeTime;
    UInt32 startTime;
    UInt16 deamonLevel;
    UInt16 curLevel;
    UInt16 maxLevel;
    UInt32 accTime;
    UInt32 accLen;
    UInt32 accAwards;
    UInt32 vitalityTime;
    UInt32 vitality;
    UInt32 spirit;
    UInt32 itemId;
    UInt32 itemNum;
    UInt16 quitLevel;
    Player* attacker;

    DeamonPlayerData()
    {
        challengeTime = 0;
        startTime = 0;
        deamonLevel = 0;
        curLevel = 0;
        maxLevel = 0;
        accTime = 0;
        accLen = 0;
        accAwards = 0;
        vitalityTime = 0;
        vitality = 0;
        spirit = 100;
        itemId = 0;
        itemNum = 0;
        quitLevel = 0;
        attacker = NULL;
    }

    UInt32 calcAccLeft();
    UInt32 calcAwards();
    UInt32 calcVitality();
    UInt32 calcSpirit();
};

static const UInt32 TOWNDEAMON_LEVEL_UP = 40;
static const UInt32 TD_MAXACCTIME = 172800;
static const UInt32 TD_MAXVITALITY = 1000;

class TownDeamon
{
    public:
        TownDeamon();
        virtual ~TownDeamon();

        void loadDeamonMonstersFromDB(UInt16 level, UInt32 npcId, UInt32 itemId, UInt32 itemNum);
        void loadDeamonPlayersFromDB(UInt16 level, UInt16 maxLevel, Player* pl);
        void listDeamons(Player*, UInt16 start, UInt16 count);
        void showLevelTown(Player*, UInt16 level);
        void showTown(Player*);
        void useAccItem(Player*, UInt8 count);
        void useVitalityItem(Player*, UInt8 count);
        void cancelDeamon(Player*);
        void challenge(Player*, UInt16 level, UInt8 type);
        void notifyChallengeResult(Player*, Player* defer, bool win);
        void autoCompleteQuite(Player*, UInt16 levels);
        void process();

        void checkStartTime(Player* pl);
        bool checkTownDeamon(Player* pl);
        bool attackNpc(Player* pl, UInt32 npcId);
        void attackPlayer(Player* pl, Player* defer);
        void beAttackByPlayer(Player* defer, Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup);

    private:
        void quitDeamon(Player* pl, Player* attacker = NULL);
        void occupyDeamon(Player* pl, UInt16 level);

    private:
        std::vector<DeamonMonster> m_Monsters;
        UInt16 m_location;
        UInt16 m_maxDeamonLevel;
        UInt16 m_maxLevel;
};

extern TownDeamon* townDeamonManager;

}


#endif

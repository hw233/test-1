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

class Player;

struct DeamonMonster
{
    Player* player;
    UInt32 npcId;

    DeamonMonster()
    {
        player = NULL;
        npcId = 0;
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
    UInt32 awards;
    UInt32 vitalityTime;
    Int32  vitality;

    DeamonPlayerData()
    {
        challengeTime = 0;
        startTime = 0;
        deamonLevel = 0;
        curLevel = 0;
        maxLevel = 0;
        accTime = 0;
        awards = 0;
        vitalityTime = 0;
        vitality = 0;
    }
};

static const UInt8 TOWNDEAMON_LEVEL_UP = 40;
static const UInt8 TD_MAXACCTIME = 86400;
static const UInt8 TD_MAXVITALITY = 1000;

class TownDeamon
{
    public:
        TownDeamon();
        virtual ~TownDeamon();

        void loadDeamonMonstersFromDB(UInt16 level, UInt32 npcId);
        void loadDeamonPlayersFromDB(UInt16 level, Player* pl);
        void listDeamons(Player*, UInt16 start, UInt16 count);
        void showLevelTown(Player*, UInt16 level);
        void showTown(Player*);
        void useAccItem(Player*, UInt8 count);
        void useVitalityItem(Player*, UInt8 count);
        void cancelDeamon(Player*);
        void challenge(Player*, UInt16 level, UInt8 type);
        void notifyChallengeResult(Player*, UInt16 level, UInt8 win);
        void autoCompleteQuite(Player*, UInt16 levels);
        void process();

        bool checkTownDeamon(Player* pl);
        bool attackNpc(Player* pl, UInt32 npcId);
        bool attackPlayer(Player* pl, Player* defer);
        bool beAttackByPlayer();

    private:
        std::vector<DeamonMonster> m_Monsters;
        UInt16 m_location;
};

extern TownDeamon* townDeamonManager;

}


#endif

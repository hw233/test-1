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
static const UInt32 TD_CHALLENGE_TIMEUNIT = 300;

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
    UInt32 accLen;
    UInt32 accAwards;
    UInt32 vitalityTime;
    UInt32  vitality;
    UInt32  spirit;

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
    }

    UInt32 calcAccLeft()
    {
        UInt32 accLeft = 0;
        if(accLen > TimeUtil::Now() - accTime)
            accLeft = accLen - TimeUtil::Now() + accTime;
        return accLen;
    }

    UInt32 calcAwards()
    {
        UInt32 awards = 0;
        UInt32 accLeft = 0;
        if(accLen > TimeUtil::Now() - accTime)
            accLeft = accLen - TimeUtil::Now() + accTime;

        if(startTime != 0)
            awards = (TimeUtil::Now() - startTime + accLen - accLeft)/TD_AWARD_TIMEUNIT + accAwards;
        return awards;
    }

    UInt32 calcVitality()
    {
        UInt32 vitalityLeft = 0;
        if(vitality * TD_VITALITY_TIMEUNIT > TimeUtil::Now() - vitalityTime)
            vitalityLeft = vitality - (TimeUtil::Now() + vitalityTime) / TD_VITALITY_TIMEUNIT;

        return vitalityLeft;
    }

    UInt32 calcSpirit()
    {
        UInt32 spiritLeft = 0;
        if(vitality * TD_VITALITY_TIMEUNIT > TimeUtil::Now() - vitalityTime)
            spiritLeft = spirit;
        else if((spirit + vitality) * TD_VITALITY_TIMEUNIT > (TimeUtil::Now() + vitalityTime))
            spiritLeft = spirit + vitality - (TimeUtil::Now() + vitalityTime) / TD_VITALITY_TIMEUNIT;

        return spiritLeft;
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
        void notifyChallengeResult(Player*, Player* defer, UInt8 win);
        void autoCompleteQuite(Player*, UInt16 levels);
        void process();

        bool checkTownDeamon(Player* pl);
        void attackNpc(Player* pl, UInt32 npcId);
        void attackPlayer(Player* pl, Player* defer);
        void beAttackByPlayer(Player* defer, Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup);

    private:
        std::vector<DeamonMonster> m_Monsters;
        UInt16 m_location;
};

extern TownDeamon* townDeamonManager;

}


#endif

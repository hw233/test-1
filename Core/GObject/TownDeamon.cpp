#include "TownDeamon.h"
#include "Player.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "Package.h"
#include "Server/Cfg.h"
#include "GData/Money.h"
#include "Country.h"
#include "Script/GameActionLua.h"


namespace GObject
{

TownDeamon* townDeamonManager = new TownDeamon();

TownDeamon::TownDeamon()
{
    m_Monsters.clear();
}

TownDeamon::~TownDeamon()
{
    m_Monsters.clear();
}

void TownDeamon::loadDeamonMonstersFromDB(UInt16 level, UInt32 npcId)
{
    DeamonMonster dm;
    dm.npcId = npcId;
    m_Monsters.push_back(dm);
    if(level != m_Monsters.size())
        printf("TownDeamon: loadDeamonMonsterFromDB Error\n");
}

void TownDeamon::loadDeamonPlayersFromDB(UInt16 level, Player* pl)
{
    if(level > m_Monsters.size() || level == 0)
        printf("TownDeamon: loadDeamonPlayerFromDB Error\n");
    DeamonMonster& dm = m_Monsters[level];
    dm.player = pl;
}

void TownDeamon::listDeamons(Player* pl, UInt16 start, UInt16 count)
{
    if(start > m_Monsters.size())
        return;

    UInt16 total = m_Monsters.size();
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x02);
    st << total << start << count;
    UInt16 end = start + count;
    if(end > m_Monsters.size())
        end = m_Monsters.size();

    UInt8 pos = st.size() - 1;
    UInt8 cnt = 0;
    for(Int16 idx = start - 1; idx < end; ++idx)
    {
        ++ cnt;
        Player* pl2 = m_Monsters[idx].player;
        if(pl2)
        {
            DeamonPlayerData* dpd = pl2->getDeamonPlayerData();
            UInt32 spirit = dpd->vitality >= 0 ? 100 : (100 + dpd->vitality);
            if(spirit < 0)
                spirit = 0;
            st << idx << pl2->getName() << pl2->GetLev() << spirit;
        }
        else
            st << idx << "" << static_cast<UInt16>(0) << static_cast<UInt8>(0) << static_cast<UInt32>(0);
    }

    st.data<UInt8>(pos) = cnt;
    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::showTown(Player* pl)
{
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x00);

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    st << dpd->curLevel << dpd->maxLevel << dpd->deamonLevel;
    st << TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();
    st << dpd->awards << dpd->accTime;
    UInt32 spirit = dpd->vitality >= 0 ? 100 : (100 + dpd->vitality);
    UInt32 vitality = dpd->vitality > 0 ? dpd->vitality : 0;
    if(spirit < 0)
        spirit = 0;
    st << vitality << spirit;

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::showLevelTown(Player* pl, UInt16 level)
{
    if(level > m_Monsters.size() || level == 0)
    {
        return;
    }

    Int16 idx = level - 1;

    Player* pl2 = m_Monsters[idx].player;
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x01);
    if(!pl2)
    {
        st << level << "" << static_cast<UInt8>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0);
    }
    else
    {
        DeamonPlayerData* dpd = pl2->getDeamonPlayerData();

        st << dpd->curLevel << pl2->getName() << pl2->GetLev();
        st << TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();
        st << dpd->awards;
        UInt32 spirit = dpd->vitality >= 0 ? 100 : (100 + dpd->vitality);
        if(spirit < 0)
            spirit = 0;
        st << spirit;
    }

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::useAccItem(Player*, UInt8 count)
{
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x03);

    UInt8 res = 0;
    UInt16 leftTime = TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    if(dpd->deamonLevel == 0)
        res = 2;
    else
    {
    }

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::useVitalityItem(Player*, UInt8 count)
{
}

void TownDeamon::cancelDeamon(Player* pl)
{
    Stream st(REP::TOWN_DEAMON);

    pl->send(st);
}

void TownDeamon::challenge(Player* pl, UInt16 level, UInt8 type)
{
}

void TownDeamon::notifyChallengeResult(Player* pl, UInt16 level, UInt8 win)
{
    Stream st(REP::TOWN_DEAMON);

    pl->send(st);
}

void TownDeamon::autoCompleteQuite(Player* pl, UInt16 levels)
{
}

void TownDeamon::process()
{
}

}



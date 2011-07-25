
#include "Config.h"
#include "Tripod.h"
#include "Player.h"
#include "Package.h"
#include "GData/ItemType.h"
//#include "Common/Stream.h"
#include "GData/GDataManager.h"
#include "GData/LootTable.h"

namespace GObject
{

static TripodData nulltd;

void Tripod::getTripodInfo(Player* pl)
{
    if (!pl)
        return;
    TripodData& td = getTripodData(pl);
    Stream st(0x39);
    st << static_cast<UInt8>(0);
    st << td.fire;
    genAward(td, st);

    st << static_cast<UInt32>(MAX_TRIPOD_SOUL) << td.soul << Stream::eos;
    pl->send(st);
}

bool Tripod::genAward(const TripodData& td, UInt32& id, UInt8& num)
{
    UInt32 loot = GData::GDataManager::GetTripodAward(td.fire, td.quality);
    const GData::LootItem* li = GData::lootTable[loot];
    if (li)
    {
        GData::LootResult lr = li->roll();
        if (lr.id)
        {
            id = lr.id;
            num = lr.count;
            return true;
        }
    }
    return false;
}

void Tripod::genAward(const TripodData& td, Stream& st)
{
    UInt32 id;
    UInt8 num;

    if (genAward(td, id, num)) {
        st << num;
        st << id;
    } else {
        st << static_cast<UInt8>(0);
        st << static_cast<UInt32>(0);
    }

    return;
}

static UInt8 tripod_factor[4][4] =
{
#if 0
    {30,    0,      0,      0},
    {50,    20,     0,      0},
    {30,    50,     20,     0},
    {0,     20,     70,     10}
#else
    {30,    0,      0,      0},
    {50,    70,     0,      0},
    {30,    80,     100,    0},
    {0,     20,     90,     100}
#endif
};

void Tripod::addItem(Player* pl, UInt32 itemid, int num, UInt8 bind)
{
    if (!pl || !itemid || !num)
        return;

    TripodData& td = getTripodData(pl);
    Package* pk = pl->GetPackage();
    if (!pk)
        return;
    ItemBase* ib = NULL;
    ib = pk->FindItem(itemid, bind);
    if (ib)
    {
        td.soul += ib->getEnergy();

        UInt8 quality = ib->getQuality() - 2;
        int rnd = uRand(100);
        for (int i = 0; i < 4; ++i)
        {
            if (tripod_factor[quality][i] && rnd <= tripod_factor[quality][i])
            {
                if (td.quality < i+2) {
                    td.quality = i+2;
                    break;
                }
            }
        }

        if (IsEquipId(itemid))
        {    
            pk->DelEquip(itemid, ToDesdroy);
        }    
        else 
        {    
            pk->DelItem(itemid, num, bind);
        }    
    }

    if (td.soul >= MAX_TRIPOD_SOUL)
        PopTimerEvent(pl, EVENT_PLAYERPRTRIPOD, pl->getId());

    DB().PushUpdateData("UPDATE `tripod` SET `quality` = %u WHERE `id` = %"I64_FMT"u", td.quality, pl->getId());
}

static UInt8 fire_com[] = {24,20,18,17,12,10,9,6,5,3};
static UInt8 fire_factor[][6] = 
{
#if 0
    {0,     0,  0,  0,  80,     20},
    {0,     0,  0,  90, 0,      10},
    {0,     0,  100,0,  0,      0},
    {100,   0,  0,  0,  0,      0},
    {0,     5,  0,  15, 80,     0},
    {0,     0,  90, 10, 0,      0},
    {100,   0,  0,  0,  0,      0},
    {90,    0,  0,  0,  10,     0},
    {0,     90, 0,  9,  0,      1},
    {90,    0,  10, 0,  0,      0}
#else
    {0,     0,  0,  0,  80,     100},
    {0,     0,  0,  90, 0,      100},
    {0,     0,  100,0,  0,      0},
    {100,   0,  0,  0,  0,      0},
    {0,     5,  0,  20, 100,    0},
    {0,     0,  90, 100 ,0,     0},
    {100,   0,  0,  0,  0,      0},
    {90,    0,  0,  0,  100,    0},
    {0,     90, 0,  99, 0,      100},
    {90,    0,  100,0,  0,      0}
#endif
};

void Tripod::makeFire(Player* pl, UInt32 id1, UInt32 id2)
{
    if (!pl)
        return;
    TripodData& td = getTripodData(pl);
    Stream st(0x39);

    UInt32 id = id1 | id2;
    int i = 0;
    while (i < 10)
    {
        if (id & fire_com[i])
            break;
        ++i;
    }
    UInt8 rnd = uRand(100);
    int j = 0;
    while (j < 6)
    {
        if (fire_factor[i][j] && rnd <= fire_factor[i][j])
            break;
        ++j;
    }
    if (j < 6)
        td.fire = j+1;

    st << static_cast<UInt8>(0);
    st << td.fire;
    genAward(td, st);
    pl->send(st);
    DB().PushUpdateData("UPDATE `tripod` SET `fire` = %u WHERE `id` = %"I64_FMT"u", td.fire, pl->getId());
}

void Tripod::getAward(Player* pl)
{
    if (!pl)
        return;
    TripodData& td = getTripodData(pl);
    if (td.awdst != 1)
        return;

    UInt32 id;
    UInt8 num;

    if (!genAward(td, id, num))
        return;
    pl->GetPackage()->AddItem(id, num, true, false, FromTripod);

    td.awdst = 0;
    td.soul = 0;
    DB().PushUpdateData("UPDATE `tripod` SET `soul` = 0,`awdst` = 0 WHERE `id` = %"I64_FMT"u", pl->getId());
    addTripodData(pl->getId(), td);
}

TripodData& Tripod::addTripodData(UInt64 id, const TripodData& data)
{
    Player* pl = globalPlayers[id];
    if (!pl)
        return nulltd;
    TripodData& td = m_tds[id];
    if (&data != &td)
        td = data;
    EventPlayerTripod* event = new (std::nothrow) EventPlayerTripod(pl, 60, MAX_TRIPOD_SOUL/POINT_PERMIN);
    if (!event) return nulltd;
    PushTimerEvent(event);
    return td;
}

TripodData& Tripod::newTripodData(Player* pl)
{
    if (!pl)
        return nulltd;
    TripodData td;
    DB().PushUpdateData("REPLACE INTO `tripod`(`id`, `soul`, `fire`, `quality`, `awdst`) VALUES(%"I64_FMT"u, %u, %u, %u, %u)", pl->getId(), td.soul, td.fire, td.quality, td.awdst);
    return addTripodData(pl->getId(), td);
}

TripodData& Tripod::getTripodData(Player* pl)
{
    if (!pl)
        return nulltd;
    if (m_tds.find(pl->getId()) == m_tds.end())
        return newTripodData(pl);
    return m_tds[pl->getId()];
}

TripodData& Tripod::getTripodData(UInt64 id)
{
    Player* pl = globalPlayers[id];
    return getTripodData(pl);
}

} // namespace GObject


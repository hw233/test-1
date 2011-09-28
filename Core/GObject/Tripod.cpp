
#include "Config.h"
#include "Tripod.h"
#include "Player.h"
#include "Package.h"
#include "GData/ItemType.h"
//#include "Common/Stream.h"
#include "GData/GDataManager.h"
#include "GData/LootTable.h"
#include "Server/SysMsg.h"
#include "MsgID.h"

namespace GObject
{

static TripodData nulltd;

void Tripod::getTripodInfo(Player* pl)
{
    if (!pl)
        return;
	FastMutex::ScopedLock lk(_mutex);
    TripodData& td = getTripodData(pl);
    sendTripodInfo(pl, td);
}

void Tripod::sendTripodInfo(Player* pl, TripodData& td)
{
    Stream st(REP::TRIPOD_INFO);
    st << static_cast<UInt8>(0);
    st << td.fire;
    st << td.quality;

    genAward(pl, td, st);
    DB6().PushUpdateData("UPDATE `tripod` SET `regen` = %u, `itemId` = %u, `num` = %u WHERE `id` = %"I64_FMT"u",
            td.needgen, td.itemId, td.num, pl->getId());

    st << static_cast<UInt32>(MAX_TRIPOD_SOUL) << td.soul << Stream::eos;
    pl->send(st);
}

bool Tripod::genAward(Player* pl, TripodData& td)
{
    if (td.needgen) {
        UInt32 loot = GData::GDataManager::GetTripodAward(td.fire, 5-td.quality); // 0-橙,1-紫,2-蓝,3-绿
        const GData::LootItem* li = GData::lootTable[loot];
        if (li)
        {
            GData::LootResult lr = li->roll();
            if (lr.id)
            {
                td.itemId = lr.id;
                td.num = lr.count;
                td.needgen = 0;
                return true;
            }
        }
        return false;
    }
    return true;
}

void Tripod::genAward(Player* pl, TripodData& td, Stream& st)
{
    if (genAward(pl, td)) {
        st << td.num;
        st << td.itemId;
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

	FastMutex::ScopedLock lk(_mutex);
    TripodData& td = getTripodData(pl);
    Package* pk = pl->GetPackage();
    if (!pk)
        return;
    ItemBase* ib = NULL;
    ib = pk->FindItem(itemid, bind);
    if (ib)
    {
        td.soul += ib->getEnergy();

        UInt8 quality = ib->getQuality() > 1 ? ib->getQuality() - 2 : 0;
        int rnd = uRand(100);
        for (int i = 0; i < 4; ++i)
        {
            if (tripod_factor[quality][i] && rnd <= tripod_factor[quality][i])
            {
                if (td.quality < i+2)
                {
                    if (pl->getVipLevel() >= 3)
                    {
                        td.quality = i+3; // 2-绿,3-蓝,4-紫,5-橙
                    }
                    else
                        td.quality = i+2; // 2-绿,3-蓝,4-紫,5-橙
                    if (td.quality > 5)
                        td.quality = 5;
                    td.needgen = 1;
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
    {
        PopTimerEvent(pl, EVENT_PLAYERPRTRIPOD, pl->getId());
        td.awdst = 1;
        td.soul = MAX_TRIPOD_SOUL;
    }

    DB6().PushUpdateData("UPDATE `tripod` SET `soul` = %u, `quality` = %u, `regen` = %u WHERE `id` = %"I64_FMT"u",
            td.soul, td.quality, td.needgen, pl->getId());
}

static UInt16 fire_begin = 47;
static UInt16 fire_end = 51;
static UInt8 fire_id2bit[] = {16/*47*/, 8/*48*/, 4/*49*/, 2/*50*/, 1/*51*/};
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
    {0,     0,  100,0,  0,      0},
    {0,     0,  0,  0,  100,    0},
    {0,     0,  0,  0,  0,      100},
    {0,     0,  0,  100,0,      0},
    {0,     80, 100,0,  0,      0},
    {0,     50, 0,  100,0,      0},
    {40,    100,0,  0,  0,      0},
    {0,     50, 0,  0,  100,    0},
    {80,    0,  0,  100,0,      0},
    {50,    0,  100,0,  0,      0},
#endif
};

void Tripod::makeFire(Player* pl, UInt32 id1, UInt32 id2)
{
    if (!pl)
        return;
	FastMutex::ScopedLock lk(_mutex);
    TripodData& td = getTripodData(pl);
    Stream st(REP::TRIPOD_INFO);

    if (id1 < fire_begin)
        id1 = fire_begin;
    if (id2 < fire_begin)
        id2 = fire_begin;
    if (id1 > fire_end)
        id1 = fire_end;
    if (id2 > fire_end)
        id2 = fire_end;

    ItemBase* ib1 = pl->GetPackage()->GetItem(id1, true);
    if (!ib1)
        ib1 = pl->GetPackage()->GetItem(id1);
    if (!ib1) {
        SYSMSG_SEND(2003, pl);
        return;
    }
    ItemBase* ib2 = pl->GetPackage()->GetItem(id2, true);
    if (!ib2)
        ib2 = pl->GetPackage()->GetItem(id2);
    if (!ib2) {
        SYSMSG_SEND(2003, pl);
        return;
    }

    UInt32 id = fire_id2bit[id1-fire_begin] | fire_id2bit[id2-fire_begin];
    int i = 0;
    while (i < 10)
    {
        if (id == fire_com[i])
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

    td.needgen = 1;

    st << static_cast<UInt8>(1);
    st << td.fire;
    st << td.quality;
    genAward(pl, td, st);
    st << Stream::eos;
    pl->send(st);
    DB6().PushUpdateData("UPDATE `tripod` SET `fire` = %u, `regen` = %u, `itemId` = %u, `num` = %u WHERE `id` = %"I64_FMT"u",
            td.fire, td.needgen, td.itemId, td.num, pl->getId());

    pl->GetPackage()->DelItem2(ib1, 1);
    pl->GetPackage()->DelItem2(ib2, 1);
    SYSMSG_SEND(2002, pl);
}

void Tripod::getAward(Player* pl)
{
    if (!pl)
        return;

    if (pl->GetPackage()->IsFull())
    {
        pl->sendMsgCode(0, 1011);
        return;
    }

    FastMutex::ScopedLock lk(_mutex);
    TripodData& td = getTripodData(pl);
    if (td.awdst != 1)
        return;

    if (!genAward(pl, td))
        return;

    if (IsEquipTypeId(td.itemId))
        pl->GetPackage()->AddEquip(td.itemId, true, false, FromTripod);
    else
        pl->GetPackage()->AddItem(td.itemId, td.num, true, false, FromTripod);

    td.fire = 0;
    td.quality = 2;
    td.needgen = 1;
    td.awdst = 0;
    td.soul = 0;
    td.itemId = 0;
    td.num = 0;
    DB6().PushUpdateData("UPDATE `tripod` SET `soul` = 0,`awdst` = 0, `itemId` = 0, `num` = 0, `regen` = 1 WHERE `id` = %"I64_FMT"u", pl->getId());
    addTripodData(pl->getId(), td);
    sendTripodInfo(pl, td);
}

TripodData& Tripod::addTripodData(UInt64 id, const TripodData& data, bool init)
{
    Player* pl = globalPlayers[id];
    if (!pl)
        return nulltd;
    TripodData& td = m_tds[id];

    if(pl->getVipLevel() > 2)
        td.quality = 3;

    if (&data != &td)
        td = data;
    EventPlayerTripod* event = new (std::nothrow) EventPlayerTripod(pl, 60, MAX_TRIPOD_SOUL/POINT_PERMIN);
    if (!event) return nulltd;
    PushTimerEvent(event);

    if (init)
        td.needgen = 0;
    DB6().PushUpdateData("UPDATE `tripod` SET `regen` = 0 WHERE `id` = %"I64_FMT"u", pl->getId());
    return td;
}

TripodData& Tripod::newTripodData(Player* pl)
{
    if (!pl)
        return nulltd;
    TripodData td;
    if(pl->getVipLevel() > 2)
        td.quality = 3;
    DB6().PushUpdateData("REPLACE INTO `tripod`(`id`, `soul`, `fire`, `quality`, `awdst`, `regen`, `itemId`, `num`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u, %u,%u)", pl->getId(), td.soul, td.fire, td.quality, td.awdst, td.needgen, td.itemId, td.num);
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


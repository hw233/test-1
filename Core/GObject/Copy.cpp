
#include "Copy.h"
#include "Player.h"
#include "GData/CopyTable.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "Common/Stream.h"

namespace GObject
{

void PlayerCopy::sendAllInfo(Player* pl)
{
}

void PlayerCopy::sendInfo(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

    CopyData& cd = getCopyData(pl, id, true);
    Stream st(0x67);
    st << id;
    st << cd.floor;
    st << cd.spot;
    UInt8 count = 1-cd.freeCount;
    count <<= 4;
    count |= 2-cd.goldCount;
    st << count;
    st << Stream::eos;
    pl->send(st);
}

void PlayerCopy::enter(Player* pl, UInt8 id, UInt8 type)
{
    if (!pl)
        return;

    Stream st(0x67);
    CopyData& tcd = getCopyData(pl, id);
    if (type == 0) {
        if (tcd.freeCount > 1) {
            st << id << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            return;
        }
        ++tcd.freeCount;
    } else if (type == 1) {
        if (tcd.goldCount > 2 || pl->getGold() < 20) {
            st << id << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            return;
        }
        ++tcd.goldCount;

        ConsumeInfo ci(EnterCopy,0,0);
        pl->useGold(20);
    }

    DB().PushUpdateData("UPDATE `player_copy` SET `freeCount` = %u, `goldCount` = %u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.freeCount, tcd.goldCount, pl->getId(), id);

    st << id << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);
}

void PlayerCopy::fight(Player* pl, UInt8 id)
{
    if (!pl)
        return;

    CopyData& tcd = getCopyData(pl, id);
    if (tcd.freeCount > 1 || tcd.goldCount > 2) {
        return;
    }

    UInt32 fgtid = GData::copyManager[id<<8|tcd.floor][tcd.spot];
    if (fgtid) {
        GData::NpcGroups::iterator it = GData::npcGroups.find(fgtid);
        if(it == GData::npcGroups.end())
            return;

        GData::NpcGroup* ng = it->second;
        if (!ng) {
            return;
        }

        Battle::BattleSimulator bsim(pl->getLocation(), pl, ng->getName(), ng->getLevel(), false, 0);
        pl->PutFighters(bsim, 0);
        ng->putFighters(bsim);

        bsim.start();
        Stream& st = bsim.getPacket();
        if(st.size() <= 8)
            return;

        // TODO: loot
        pl->send(st);

        if (bsim.getWinner() == 1) {
            bool nextfloor = false;
            if (tcd.spot >= (GData::copyManager[id<<8|tcd.floor].size() - 1))
                nextfloor = true;

            if (nextfloor) {
                ++tcd.floor;
                tcd.spot = 1;
            }

            if (tcd.floor > GData::copyMaxManager[id]) {
                Stream st(0x67);
                st << id;
                st << Stream::eos;
                pl->send(st);
            } else {
                Stream st(0x67);
                st << id << tcd.floor << tcd.spot;
                st << Stream::eos;
                pl->send(st);
            }
        }
    }
}

void PlayerCopy::reset(Player* pl, UInt8 id)
{
    if (!pl)
        return;

    Stream st(0x67);
    CopyData& tcd = getCopyData(pl, id);
    if (!tcd.floor)
    {
        st << id << static_cast<UInt8>(1) << Stream::eos;
        pl->send(st);
        return;
    }

    tcd.floor = 1;
    tcd.spot = 1;
    ++tcd.freeCount;
    DB().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u",
            pl->getId(), tcd.floor, tcd.spot, id);

    st << id << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);
}

void PlayerCopy::addPlayer(UInt64 playerId, UInt8 id, UInt8 floor, UInt8 spot, UInt8 free, UInt8 gold)
{
    CopyData& cd = getCopyData(playerId, id);
    cd.floor = floor;
    cd.spot = spot;
    cd.freeCount = free;
    cd.goldCount = gold;
}

CopyData& PlayerCopy::getCopyData(Player* pl, UInt8 id, bool update)
{
    static CopyData nulldata;
    if (!pl || !id)
        return nulldata;
    return getCopyData(pl->getId(), id, update);
}

CopyData& PlayerCopy::getCopyData(UInt64 playerId, UInt8 id, bool update)
{
    static CopyData nulldata;
    CopyData& cd = m_copys[playerId][id];
    if (!cd.floor) {
        cd.floor = 1;
        cd.spot = 1;
        if (update)
            DB().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`, `freeCount`, `goldCount`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u)", playerId, id, cd.floor, cd.spot, cd.freeCount, cd.goldCount);
    }
    return cd;
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


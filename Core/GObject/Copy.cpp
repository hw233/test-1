
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
    st << static_cast<UInt8>(0);
    st << id;
    st << cd.floor;
    st << cd.spot;
    UInt8 count = 2-cd.goldCount;
    count <<= 4;
    count |= 3-cd.freeCount;
    st << count;
    st << Stream::eos;
    pl->send(st);
}

void PlayerCopy::enter(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

    Stream st(0x67);
    CopyData& tcd = getCopyData(pl, id, true);

    UInt8 ret = 1;
    if (!tcd.floor) {
        tcd.floor = 1;
        tcd.spot = 1;
    }
    if (tcd.freeCount < 3) {
        ++tcd.freeCount;
        ret = 0;
    } else if (tcd.goldCount < 2) {
        if (tcd.goldCount > 2 || pl->getGold() < (UInt32)20*(tcd.goldCount+1)) {
            st << static_cast<UInt8>(1) << id << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);

            if (pl->getGold() < (UInt32)20*(tcd.goldCount+1))
                pl->sendMsgCode(0, 1007);
            return;
        }
        ++tcd.goldCount;

        ConsumeInfo ci(EnterCopy,0,0);
        pl->useGold(20*tcd.goldCount); // 第1次20,第2次40
        ret = 0;
    }

    if (!ret)
        DB().PushUpdateData("UPDATE `player_copy` SET `freeCount` = %u, `goldCount` = %u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.freeCount, tcd.goldCount, pl->getId(), id);

    st << static_cast<UInt8>(1) << id << ret << Stream::eos;
    pl->send(st);
}

void PlayerCopy::fight(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

    CopyData& tcd = getCopyData(pl, id);
    if (tcd.freeCount > 3 || tcd.goldCount > 2) {
        return;
    }

    UInt32 fgtid = GData::copyManager[id<<8|tcd.floor][tcd.spot];
    if (fgtid) {
        if (pl->attackCopyNpc(fgtid)) {
            bool nextfloor = false;
            if (tcd.spot >= (GData::copyManager[id<<8|tcd.floor].size() - 1))
                nextfloor = true;

            if (nextfloor) {
                ++tcd.floor;
                tcd.spot = 1;
            } else
                ++tcd.spot;

            if (tcd.floor > GData::copyMaxManager[id]) {
                Stream st(0x67);
                st << static_cast<UInt8>(5);
                st << id;
                st << Stream::eos;
                pl->send(st);
            } else {
                Stream st(0x67);
                st << static_cast<UInt8>(6);
                st << id << tcd.floor << tcd.spot;
                st << Stream::eos;
                pl->send(st);
            }

            DB().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.floor, tcd.spot, pl->getId(), id);
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
        st << static_cast<UInt8>(2) << id << static_cast<UInt8>(1) << Stream::eos;
        pl->send(st);
        return;
    }

    tcd.floor = 1;
    tcd.spot = 1;
    tcd.freeCount = 1;
    DB().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u,`freeCount`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u",
            tcd.floor, tcd.spot, tcd.freeCount, pl->getId(), id);

    st << static_cast<UInt8>(2) << id << static_cast<UInt8>(0) << Stream::eos;
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
        cd.updatetime = TimeUtil::Now();
        if (update)
            DB().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`, `freeCount`, `goldCount`, `updatetime`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u, %u)", playerId, id, cd.floor, cd.spot, cd.freeCount, cd.goldCount, TimeUtil::Now());
    } else {
        if (TimeUtil::Day(TimeUtil::Now()) != TimeUtil::Day(cd.updatetime)) {
            cd.updatetime = TimeUtil::Now();
            cd.freeCount = 0;
            cd.goldCount = 0;
        }
    }
    return cd;
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */



#include "Copy.h"
#include "Player.h"
#include "GData/CopyTable.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "GData/Money.h"

namespace GObject
{


void PlayerCopy::sendAllInfo(Player* pl)
{
}

void PlayerCopy::sendInfo(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

	FastMutex::ScopedLock lk(_mutex);
    CopyData& cd = getCopyData(pl, id, true);
    Stream st(REP::COPY_INFO);
    st << static_cast<UInt8>(0);
    st << id;
    st << cd.floor;
    st << cd.spot;
    UInt8 count = GOLDCNT-PLAYER_DATA(pl, copyGoldCnt);
    count <<= 4;
    
    count |= FREECNT-PLAYER_DATA(pl, copyFreeCnt);
    st << count;
    st << Stream::eos;
    pl->send(st);
}

UInt8 PlayerCopy::getCopySize(Player* pl)
{
    if(!pl)
        return 0;

    return static_cast<UInt8>(m_copys[pl->getId()].size());
}

void PlayerCopy::buildInfo(Player* pl, Stream& st)
{
    if(!pl)
        return;

    std::map<UInt8, CopyData>& pl_copy = m_copys[pl->getId()];
    for(std::map<UInt8, CopyData>::iterator it = pl_copy.begin(); it != pl_copy.end(); ++ it )
    {
        st << static_cast<UInt8>(it->first);
    }
}

void PlayerCopy::enter(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

	FastMutex::ScopedLock lk(_mutex);
    Stream st(REP::COPY_INFO);
    CopyData& tcd = getCopyData(pl, id, true);

    UInt8 ret = 1;
    if (PLAYER_DATA(pl, copyFreeCnt) < FREECNT) {
        ++PLAYER_DATA(pl, copyFreeCnt);
        ret = 0;
    } else if (PLAYER_DATA(pl, copyGoldCnt) < GOLDCNT) {
        //if (pl->getGold() < (UInt32)20*(PLAYER_DATA(pl, copyGoldCnt)+1)) {
        if (pl->getGold() < GData::moneyNeed[GData::COPY_ENTER1+PLAYER_DATA(pl, copyGoldCnt)].gold) {
            st << static_cast<UInt8>(1) << id << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            pl->sendMsgCode(0, 1101);
            return;
        }
        ++PLAYER_DATA(pl, copyGoldCnt);

        ConsumeInfo ci(EnterCopy,0,0);
        pl->useGold(20*PLAYER_DATA(pl, copyGoldCnt)); // 第1次20,第2次40
        ret = 0;
    }

    if (!tcd.floor) {
        tcd.floor = 1;
        tcd.spot = 1;
    }

    if (!ret) {
        DB().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = %u, `copyGoldCnt` = %u WHERE `id` = %"I64_FMT"u", PLAYER_DATA(pl, copyFreeCnt), PLAYER_DATA(pl, copyGoldCnt), pl->getId());
        DB().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.floor, tcd.spot, pl->getId(), id);
    }


    st << static_cast<UInt8>(1) << id << ret << Stream::eos;
    pl->send(st);
}

void PlayerCopy::fight(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

	FastMutex::ScopedLock lk(_mutex);
    CopyData& tcd = getCopyData(pl, id);
    if (!tcd.floor) {
        return;
    }

    if (PLAYER_DATA(pl, copyFreeCnt) > FREECNT && PLAYER_DATA(pl, copyGoldCnt) > GOLDCNT)
        return;

    if (!GData::copyManager[id<<8|tcd.floor].size())
        return;

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
                Stream st(REP::COPY_INFO);
                st << static_cast<UInt8>(5);
                st << id;
                st << Stream::eos;
                pl->send(st);

                tcd.floor = 0;
                tcd.spot = 0;
                DB().PushUpdateData("DELETE FROM `player_copy` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.spot, pl->getId(), id);
                return;
            } else {
                Stream st(REP::COPY_INFO);
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

	FastMutex::ScopedLock lk(_mutex);
    Stream st(REP::COPY_INFO);
    CopyData& tcd = getCopyData(pl, id);
    if (!tcd.floor)
    {
        st << static_cast<UInt8>(2) << id << static_cast<UInt8>(1) << Stream::eos;
        pl->send(st);
        return;
    }

    tcd.floor = 0;
    tcd.spot = 0;
    //DB().PushUpdateData("UPDATE `player_copy` SET `floor`=%u,`spot`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.floor, tcd.spot, pl->getId(), id);
    DB().PushUpdateData("DELETE FROM `player_copy` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tcd.spot, pl->getId(), id);

    st << static_cast<UInt8>(2) << id << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);
}

void PlayerCopy::addPlayer(UInt64 playerId, UInt8 id, UInt8 floor, UInt8 spot)
{
    CopyData& cd = m_copys[playerId][id];
    cd.floor = floor;
    cd.spot = spot;
}

CopyData& PlayerCopy::getCopyData(Player* pl, UInt8 id, bool update)
{
    static CopyData nulldata;
    if (!pl || !id)
        return nulldata;
    return getCopyData(pl, pl->getId(), id, update);
}

CopyData& PlayerCopy::getCopyData(Player* pl, UInt64 playerId, UInt8 id, bool update)
{
    static CopyData nulldata;
    CopyData& cd = m_copys[playerId][id];
    if (!cd.floor) {
        PLAYER_DATA(pl, copyUpdate) = TimeUtil::Now();
        if (update) {
            DB().PushUpdateData("UPDATE `player` SET `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), playerId);
            DB().PushUpdateData("REPLACE INTO `player_copy`(`playerId`, `id`, `floor`, `spot`) VALUES(%"I64_FMT"u, %u, %u, %u)", playerId, id, cd.floor, cd.spot);
        }
    } else {
        if (pl && TimeUtil::Day(TimeUtil::Now()) != TimeUtil::Day(PLAYER_DATA(pl, copyUpdate))) {
            if (pl) {
                PLAYER_DATA(pl, copyUpdate) = TimeUtil::Now();
                PLAYER_DATA(pl, copyFreeCnt) = 0;
                PLAYER_DATA(pl, copyGoldCnt) = 0;
            }

            DB().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = 0, `copyGoldCnt` = 0, `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), playerId);
        }
    }
    return cd;
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


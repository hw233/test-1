
#include "FrontMap.h"
#include "Player.h"
#include "GData/FrontMapTable.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "MsgID.h"

namespace GObject
{

static const UInt8 FREECNT = 1;
static const UInt8 GOLDCNT = 2;

void FrontMap::sendAllInfo(Player* pl)
{
}

void FrontMap::sendInfo(Player* pl, UInt8 id, bool needspot)
{
    Stream st(REP::FORMATTON_INFO);
    FastMutex::ScopedLock lk(_mutex);
    UInt8 count = getCount(pl);
    st << static_cast<UInt8>(0);
    st << id;
    st << count;

    if (needspot) {
        sendFrontMap(st, pl, id);
    }

    st << Stream::eos;
    pl->send(st);
}

void FrontMap::sendFrontMap(Stream& st, Player* pl, UInt8 id)
{
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];

    size_t off = st.size();
    st << static_cast<UInt8>(0);

    bool first = true;
    UInt8 size = 0;
    UInt8 max = GData::frontMapMaxManager[id];
    for (UInt8 i = 1; i <= max; ++i) {
        std::vector<GData::FrontMapFighter>& fmf = GData::frontMapManager[id];
        if (i >= fmf.size())
            continue;
        if (!fmf[i].count)
            continue;

        st << i;
        if (i < tmp.size()) {
            if (!tmp[i].count)
                st << static_cast<UInt8>(0xff);
            else
                st << static_cast<UInt8>(GData::frontMapManager[id][i].count - tmp[i].count);
        } else {
            if (first) {
                first = false;
                st << fmf[i].count;
            } else
                st << static_cast<UInt8>(0xff);
        }
        ++size;
    }

    if (size)
        st.data<UInt8>(off) = size;
}

void FrontMap::enter(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

    FastMutex::ScopedLock lk(_mutex);
    UInt8 ret = 1;
    if (PLAYER_DATA(pl, frontFreeCnt) < FREECNT) {
        ++PLAYER_DATA(pl, frontFreeCnt);
        ret = 0;
    } else if (PLAYER_DATA(pl, frontGoldCnt) < GOLDCNT) {
        if (pl->getGold() < (UInt32)20*(PLAYER_DATA(pl, frontGoldCnt)+1)) {
            Stream st(REP::FORMATTON_INFO);
            st << static_cast<UInt8>(1) << id << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            pl->sendMsgCode(0, 1101);
            return;
        }  

        ++PLAYER_DATA(pl, frontGoldCnt);
        ret = 0;

        ConsumeInfo ci(EnterFrontMap,0,0);
        pl->useGold(20*PLAYER_DATA(pl, frontGoldCnt));
    }

    if (!ret) {
        DB().PushUpdateData("UPDATE `player` SET `frontFreeCnt` = %u, `frontGoldCnt` = %u, `frontUpdate` = %u WHERE `id` = %"I64_FMT"u", PLAYER_DATA(pl, frontFreeCnt), PLAYER_DATA(pl,frontGoldCnt), TimeUtil::Now(), pl->getId());

        UInt8 count = getCount(pl);
        Stream st(REP::FORMATTON_INFO);
        st << static_cast<UInt8>(3) << count << Stream::eos;
        pl->send(st);
    }

    Stream st(REP::FORMATTON_INFO);
    st << static_cast<UInt8>(1) << id << ret << Stream::eos;
    sendFrontMap(st, pl, id);
    pl->send(st);
}

UInt8 FrontMap::getCount(Player* pl)
{
    if (pl && TimeUtil::Day(TimeUtil::Now()) != TimeUtil::Day(PLAYER_DATA(pl, frontUpdate))) {
        if (pl) {
            PLAYER_DATA(pl, frontUpdate) = TimeUtil::Now();
            PLAYER_DATA(pl, frontFreeCnt) = 0;
            PLAYER_DATA(pl, frontGoldCnt) = 0;
            DB().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = 0, `copyGoldCnt` = 0, `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), pl->getId());
        }
    }

    UInt8 count = GOLDCNT-PLAYER_DATA(pl, frontGoldCnt);
    count <<= 4;
    count |= FREECNT-PLAYER_DATA(pl, frontFreeCnt);
    return count;
}

void FrontMap::fight(Player* pl, UInt8 id, UInt8 spot)
{
    if (!pl || !id || !spot)
        return;

    FastMutex::ScopedLock lk(_mutex);
    if (PLAYER_DATA(pl, frontFreeCnt) > FREECNT && PLAYER_DATA(pl, frontGoldCnt) > GOLDCNT)
        return;

    Stream st(REP::FORMATTON_INFO);
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (spot > GData::frontMapMaxManager[id]) {
        // TODO: 
        return;
    }

    if (spot >= tmp.size()) {
        tmp.resize(spot+1);
        tmp[spot].count = 0;
        tmp[spot].status = 0;
        DB().PushUpdateData("REPLACE INTO `player_frontmap` SET `playerId`=%"I64_FMT"u,`id`=%u,`spot`=%u,`count`=0,`status`=0", pl->getId(), id, spot); 
    }

    UInt8 count = tmp[spot].count;
    if (count >= GData::frontMapManager[id][spot].count) {
        st << static_cast<UInt8>(5) << id << spot << static_cast<UInt8>(0) << Stream::eos;
        pl->send(st);
        return;
    }

    bool ret = false;
    UInt32 fgtid = GData::frontMapManager[id][spot].fighterId;
    if (fgtid) {
        if (pl->attackCopyNpc(fgtid)) {
            ret = true;
        }

        if (ret) {
            ++tmp[spot].count;
            tmp[spot].status = 1;

            if (spot >= GData::frontMapMaxManager[id]) {
                Stream st(REP::FORMATTON_INFO);
                st << static_cast<UInt8>(4) << id << Stream::eos;
                pl->send(st);
            } else {
                UInt8 nspot = spot+1;
                while (!GData::frontMapManager[id][nspot].count && nspot <= GData::frontMapMaxManager[id])
                    ++nspot;
                Stream st(REP::FORMATTON_INFO);
                st << static_cast<UInt8>(5) << id << nspot;
                if (nspot < tmp.size()) {
                    st << static_cast<UInt8>(GData::frontMapManager[id][nspot].count - tmp[nspot].count);
                } else {
                    st << GData::frontMapManager[id][nspot].count;
                }
                st << Stream::eos;
                pl->send(st);
            }

            DB().PushUpdateData("UPDATE `player_frontmap` SET `count`=%u,`status`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u AND `spot`=%u", tmp[spot].count, tmp[spot].status, pl->getId(), id, spot);
        } else
            DB().PushUpdateData("UPDATE `player_frontmap` SET `count`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u", tmp[spot].count, pl->getId(), id);

        st << static_cast<UInt8>(5) << id << spot << static_cast<UInt8>(GData::frontMapManager[id][spot].count - tmp[spot].count) << Stream::eos;
        pl->send(st);
    }
    return;
}

void FrontMap::reset(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;
    FastMutex::ScopedLock lk(_mutex);
    Stream st(REP::FORMATTON_INFO);
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (tmp.size() < 1) {
        st << static_cast<UInt8>(2) << id << static_cast<UInt8>(1) << Stream::eos;
        pl->send(st);
        return;
    }

    tmp.resize(0);
    DB().PushUpdateData("DELETE FROM `player_frontmap` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", pl->getId(), id);

    st << static_cast<UInt8>(2) << id << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);
}

void FrontMap::addPlayer(UInt64 playerId, UInt8 id, UInt8 spot, UInt8 count, UInt8 status)
{
    if (!playerId || !id || !spot)
        return;
    std::vector<FrontMapData>& tmp = m_frts[playerId][id];
    if (tmp.size() <= spot)
        tmp.resize(spot+1);
    tmp[spot].count = count;
    tmp[spot].status = status?1:0;
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


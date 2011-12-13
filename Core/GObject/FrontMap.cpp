
#include "FrontMap.h"
#include "Player.h"
#include "GData/FrontMapTable.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "MsgID.h"
#include "GData/Money.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Script/GameActionLua.h"
#include "Country.h"

namespace GObject
{

UInt8 FrontMap::_activeCount = 0;

void autoClear(Player* pl, bool complete = false, UInt8 id = 0, UInt8 spot = 0)
{
    if (!pl)
        return;

    if (complete)
    {
    }

    PopTimerEvent(pl, EVENT_AUTOFRONTMAP, pl->getId());
    pl->delFlag(Player::AutoFrontMap);
    pl->SetVar(VAR_ATOFM, 0);
    DB3().PushUpdateData("DELETE FROM `auto_frontmap` WHERE playerId = %"I64_FMT"u", pl->getId());
}

void FrontMap::setFrontMapActiveCount(UInt8 c)
{
    _activeCount = c;
}

void FrontMap::sendAllInfo(Player* pl)
{
}

UInt8 FrontMap::getFreeCount()
{
    if (World::_wday == 7)
        return _activeCount + FREECNT * 2;
    return _activeCount+ FREECNT;
}

UInt8 FrontMap::getGoldCount(UInt8 vipl)
{
    if (vipl == 1)
        return 1;
    if (vipl == 2)
        return 2;
    if (vipl >= 3)
        return 3;
    return 0;
}

void FrontMap::sendInfo(Player* pl, UInt8 id, bool needspot, bool force)
{
    FastMutex::ScopedLock lk(_mutex);
    Stream st(REP::FORMATTON_INFO);
    UInt8 count = getCount(pl);
    st << static_cast<UInt8>(0);
    st << id;
    st << count;

    if (needspot)
        sendFrontMap(st, pl, id, needspot?true:force);

    st << Stream::eos;
    pl->send(st);
}

void FrontMap::sendInfo2(Player* pl, UInt8 id, bool succ)
{
    Stream st(REP::FORMATTON_INFO);
    st << static_cast<UInt8>(1);
    st << id;

    if (succ)
        sendFrontMap(st, pl, id, true);
    else 
        st << static_cast<UInt8>(0);

    st << Stream::eos;
    pl->send(st);
}

UInt8 FrontMap::getFrontMapSize(Player* pl)
{
    if(!pl)
        return 0;

    return m_frts[pl->getId()].size();
}

void FrontMap::buildInfo(Player* pl, Stream& st)
{
    if(!pl)
        return;

    std::map<UInt8, std::vector<FrontMapData>>& pl_frts = m_frts[pl->getId()];
    for (std::map<UInt8, std::vector<FrontMapData>>::iterator
            it = pl_frts.begin(), end = pl_frts.end(); it != end; ++it)
        st << static_cast<UInt8>(it->first);
}

void FrontMap::sendFrontMap(Stream& st, Player* pl, UInt8 id, bool force)
{
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];

    if (force && !tmp.size())
        return;

    if (!tmp.size()) {
        tmp.resize(1);
        DB3().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`) VALUES(%"I64_FMT"u, %u, 0, 0, 0)",
                pl->getId(), id);
    }

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
                first = false;
            st << static_cast<UInt8>(fmf[i].count - tmp[i].count);
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

bool FrontMap::checkLevel(Player* pl, UInt8 id)
{
    UInt8 lvls[] = {35, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95};
    if (pl->GetLev() < lvls[id-1] || id > sizeof(lvls)/sizeof(UInt8)) {
        SYSMSG_SENDV(2109, pl, pl->GetLev(), lvls[id-1]);
        return false;
    }
    return true;
}

void FrontMap::enter(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

    if (!checkLevel(pl, id))
        return;

    FastMutex::ScopedLock lk(_mutex); // XXX: ???
    UInt8 ret = 1;
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (!tmp.size()) {
        if (PLAYER_DATA(pl, frontFreeCnt) < getFreeCount()) {
            ++PLAYER_DATA(pl, frontFreeCnt);
            tmp.resize(1);
            DB3().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`) VALUES(%"I64_FMT"u, %u, 0, 0, 0)",
                    pl->getId(), id);
            ret = 0;
        } else if (PLAYER_DATA(pl, frontGoldCnt) < getGoldCount(pl->getVipLevel())) {
            if (pl->getGold() < GData::moneyNeed[GData::FRONTMAP_ENTER1+PLAYER_DATA(pl, frontGoldCnt)].gold) {
                Stream st(REP::FORMATTON_INFO);
                st << static_cast<UInt8>(1) << id << static_cast<UInt8>(1) << Stream::eos;
                pl->send(st);
                pl->sendMsgCode(0, 1104);
                return;
            }  

            ++PLAYER_DATA(pl, frontGoldCnt);
            tmp.resize(1);
            tmp[0].lootlvl = PLAYER_DATA(pl, frontGoldCnt);
            DB3().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`, `lootlvl`) VALUES(%"I64_FMT"u, %u, 0, 0, 0, %u)", 
                    pl->getId(), id, PLAYER_DATA(pl, frontGoldCnt));
            ret = 0;

            ConsumeInfo ci(EnterFrontMap,0,0);
            pl->useGold(20*PLAYER_DATA(pl, frontGoldCnt));
        } else {
            // XXX:
            return;
        }
    } else {
        ret = 0;
    }

    if (!ret) {
        sendInfo2(pl, id, true);

        PLAYER_DATA(pl, frontUpdate) = TimeUtil::Now();
        DB1().PushUpdateData("UPDATE `player` SET `frontFreeCnt` = %u, `frontGoldCnt` = %u, `frontUpdate` = %u WHERE `id` = %"I64_FMT"u",
                PLAYER_DATA(pl, frontFreeCnt), PLAYER_DATA(pl,frontGoldCnt), TimeUtil::Now(), pl->getId());

        UInt8 count = getCount(pl);
        Stream st(REP::FORMATTON_INFO);
        st << static_cast<UInt8>(3) << count << Stream::eos;
        pl->send(st);
        GameAction()->doAty(pl, AtyFormation, 0 ,0 );
    } else {
        sendInfo2(pl, id, false);
    }
}

UInt8 FrontMap::getCount(Player* pl)
{
    if (!pl)
        return 0;

    if (!TimeUtil::SameDay(TimeUtil::Now(), PLAYER_DATA(pl, frontUpdate)) ||
            getGoldCount(pl->getVipLevel()) < PLAYER_DATA(pl, frontGoldCnt) ||
            getFreeCount() < PLAYER_DATA(pl, frontFreeCnt)) {
        PLAYER_DATA(pl, frontUpdate) = TimeUtil::Now();
        PLAYER_DATA(pl, frontFreeCnt) = 0;
        PLAYER_DATA(pl, frontGoldCnt) = 0;
        DB1().PushUpdateData("UPDATE `player` SET `frontFreeCnt` = 0, `frontGoldCnt` = 0, `frontUpdate` = %u WHERE `id` = %"I64_FMT"u",
                PLAYER_DATA(pl, frontUpdate), pl->getId());
    }

    UInt8 count = getGoldCount(pl->getVipLevel())-PLAYER_DATA(pl, frontGoldCnt);
    count <<= 4;
    count |= getFreeCount()-PLAYER_DATA(pl, frontFreeCnt);
    return count;
}

UInt8 FrontMap::fight(Player* pl, UInt8 id, UInt8 spot, bool ato, bool complate)
{
    if (!pl || !id || !spot)
        return 0;

    FastMutex::ScopedLock lk(_mutex);
    if (PLAYER_DATA(pl, frontFreeCnt) > getFreeCount() &&
            PLAYER_DATA(pl, frontGoldCnt) > getGoldCount(pl->getVipLevel()))
        return 0;

    Stream st(REP::FORMATTON_INFO);
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (spot > GData::frontMapMaxManager[id]) {
        // TODO: 
        return 0;
    }

    if (spot > 1) {
        if (spot > tmp.size())
            return 0;
    }

    if (spot >= tmp.size()) {
        tmp.resize(spot+1);
        tmp[spot].lootlvl = tmp[spot-1].lootlvl;
        DB3().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`, `lootlvl`) VALUES(%"I64_FMT"u, %u, %u, 0, 0, %u)",
                pl->getId(), id, spot, tmp[spot].lootlvl);
    }

    UInt8 count = tmp[spot].count;
    if (count >= GData::frontMapManager[id][spot].count) {
        st << static_cast<UInt8>(5) << id << spot << static_cast<UInt8>(0) << Stream::eos;
        pl->send(st);
        return 0;
    }

    bool ret = false;
    UInt32 fgtid = GData::frontMapManager[id][spot].fighterId;
    if (fgtid) {
        std::vector<UInt16> loot;
        if (pl->attackCopyNpc(fgtid, 0, id, World::_wday==7?2:1, tmp[spot].lootlvl, ato, &loot)) {
            ret = true;
            if (ato)
                pl->checkLastBattled();
        }

        if (ret) {
            ++tmp[spot].count;
            tmp[spot].status = 1;

            if (spot >= GData::frontMapMaxManager[id]) {
                if (ato)
                {
                    Stream st(REP::AUTO_FRONTMAP);
                    UInt8 size = loot.size();
                    if (size >= 1) {
                        UInt8 rsize = loot[0];
                        if (rsize != size/2)
                            st << static_cast<UInt8>(5);
                        else
                            st << static_cast<UInt8>(4);

                        st << id << spot;

                        st << static_cast<UInt8>(rsize);

                        for (UInt8 i = 1, c = 0; i < size && c < rsize; i += 2, ++c)
                            st << loot[i] << static_cast<UInt8>(loot[i+1]);
                    } else {
                        st << static_cast<UInt8>(4) << id << spot << static_cast<UInt8>(0);
                    }

                    st << Stream::eos;
                    pl->send(st);
                }
                else
                {
                    Stream st(REP::FORMATTON_INFO);
                    st << static_cast<UInt8>(4) << id << Stream::eos;
                    pl->send(st);
                }
                tmp.resize(0);

                GameAction()->onFrontMapWin(pl, id, spot, tmp[spot].lootlvl);
                DB3().PushUpdateData("DELETE FROM `player_frontmap` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", pl->getId(), id);
                if (ato)
                    autoClear(pl, complate);
                return 2;
            } else {
                UInt8 nspot = spot+1;
                while (!GData::frontMapManager[id][nspot].count && nspot <= GData::frontMapMaxManager[id])
                    ++nspot;

                if (ato)
                {
                    Stream st(REP::AUTO_FRONTMAP);
                    UInt8 size = loot.size();
                    if (size) {
                        UInt8 rsize = loot[0];
                        if (rsize != size/2) {
                            st << static_cast<UInt8>(5);
                        } else {
                            st << static_cast<UInt8>(3);
                        }
                        st << id << spot;

                        st << static_cast<UInt8>(size/2);
                        for (UInt8 i = 1; i < rsize*2+1; i += 2)
                            st << loot[i] << static_cast<UInt8>(loot[i+1]);

                        for (UInt8 i = rsize*2+1; i < size; i += 2) {
                        }
                    } else {
                        st << static_cast<UInt8>(3) << id << spot << static_cast<UInt8>(0);
                    }

                    st << Stream::eos;
                    pl->send(st);
                }
                else
                {
                    Stream st(REP::FORMATTON_INFO);
                    st << static_cast<UInt8>(5) << id << nspot;

                    if (nspot < tmp.size())
                        st << static_cast<UInt8>(GData::frontMapManager[id][nspot].count - tmp[nspot].count);
                    else
                        st << GData::frontMapManager[id][nspot].count;

                    st << Stream::eos;
                    pl->send(st);
                }
                GameAction()->onFrontMapFloorWin(pl, id, spot, tmp[spot].lootlvl);
            }

            DB3().PushUpdateData("UPDATE `player_frontmap` SET `count`=%u,`status`=%u WHERE `playerId` = %"I64_FMT"u AND `id` = %u AND `spot`=%u",
                    tmp[spot].count, tmp[spot].status, pl->getId(), id, spot);
        }
        else
        {
            if (ato)
            {
                Stream st(REP::AUTO_FRONTMAP);
                st << static_cast<UInt8>(2) << id << spot << Stream::eos;
                pl->send(st);

                autoClear(pl, complate);
                return 0;
            }
        }

        st << static_cast<UInt8>(5) << id << spot << static_cast<UInt8>(GData::frontMapManager[id][spot].count - tmp[spot].count) << Stream::eos;
        pl->send(st);

        return ret?1:0;
    }
    return 0;
}

void FrontMap::reset(Player* pl, UInt8 id)
{
    if (!pl || !id)
        return;

    FastMutex::ScopedLock lk(_mutex);

    Stream st(REP::FORMATTON_INFO);
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (!tmp.size()) {
        st << static_cast<UInt8>(2) << id << static_cast<UInt8>(1) << Stream::eos;
        pl->send(st);
        return;
    }

    tmp.resize(0);
    st << static_cast<UInt8>(2) << id << static_cast<UInt8>(0) << Stream::eos;
    pl->send(st);

    DB3().PushUpdateData("DELETE FROM `player_frontmap` WHERE `playerId` = %"I64_FMT"u AND `id` = %u", pl->getId(), id);
}

void FrontMap::addPlayer(UInt64 playerId, UInt8 id, UInt8 spot, UInt8 count, UInt8 status, UInt8 lootlvl)
{
    if (!playerId || !id)
        return;

    std::vector<FrontMapData>& tmp = m_frts[playerId][id];
    if (tmp.size() <= spot)
        tmp.resize(spot+1);

    tmp[spot].count = count;
    tmp[spot].status = status?1:0;
    tmp[spot].lootlvl = lootlvl;
}

void FrontMap::autoBattle(Player* pl, UInt8 id, UInt8 type, UInt8 mtype, bool init)
{
    if (!pl || !id)
        return;

    switch (type)
    {
        case 0:
            {
                if (!init) {
                    if (pl->hasFlag(Player::AutoFrontMap)) {
                        pl->sendMsgCode(0, 1414);
                        return;
                    }

                    if (pl->getVipLevel() < 5)
                        return;

                    if (!checkLevel(pl, id))
                        return;

                    if (mtype == 1)
                    {
                        if (GData::moneyNeed[GData::FRONTMAP_AUTO].gold > pl->getGoldOrCoupon()) {
                            pl->sendMsgCode(0, 1104);
                            return;
                        } else {
                            ConsumeInfo ci(EnterFrontMap,0,0);
                            pl->useGoldOrCoupon(GData::moneyNeed[GData::FRONTMAP_AUTO].gold, &ci);
                        }
                    }
                    else
                    {
                        if (GData::moneyNeed[GData::FRONTMAP_AUTO1+id-1].tael > pl->getTael()) {
                            pl->sendMsgCode(0, 1100);
                            return;
                        } else {
                            ConsumeInfo ci(EnterFrontMap,0,0);
                            pl->useTael(GData::moneyNeed[GData::FRONTMAP_AUTO1+id-1].tael, &ci);
                        }
                    }
                }

                std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
                if (!tmp.size())
                    return;

                UInt8 max = GData::frontMapMaxManager[id];
                UInt8 count = max - tmp.size() + 1;
                UInt8 nspot = tmp.size();
                if (nspot != 1 && !tmp[nspot-1].status)
                {
                    --nspot;
                    ++count;
                }
                if (!nspot)
                    nspot = 1;

                UInt8 secs = 0;
                if (cfg.GMCheck)
                    secs = 60; 
                else
                    secs = 20; 

                EventAutoFrontMap* event = new (std::nothrow) EventAutoFrontMap(pl, secs, count, id, nspot);
                if (!event) return;
                PushTimerEvent(event);

                pl->addFlag(Player::AutoFrontMap);
                pl->SetVar(VAR_ATOFM, id);
                DB3().PushUpdateData("REPLACE INTO `auto_frontmap` (`playerId`, `id`) VALUES (%"I64_FMT"u, %u)", pl->getId(), id);

                Stream st(REP::AUTO_FRONTMAP);
                st << static_cast<UInt8>(0) << id << nspot << Stream::eos; 
                pl->send(st);
            }
            break;

        case 1:
            {
                if (!pl->hasFlag(Player::AutoFrontMap)) {
                    pl->sendMsgCode(0, 1415);
                    return;
                }

                autoClear(pl);

                Stream st(REP::AUTO_FRONTMAP);
                st << static_cast<UInt8>(1) << id << Stream::eos; 
                pl->send(st);
            }
            break;

        case 2:
            {
                if (!pl->hasFlag(Player::AutoFrontMap)) {
                    pl->sendMsgCode(0, 1415);
                    return;
                }

                if (pl->getVipLevel() < 7)
                    return;

                if (GData::moneyNeed[GData::FRONTMAP_IM].gold > pl->getGoldOrCoupon())
                {
                    pl->sendMsgCode(0, 1104);
                    return;
                }

                ConsumeInfo ci(AutoFrontMapComplete,0,0);
                pl->useGoldOrCoupon(GData::moneyNeed[GData::FRONTMAP_IM].gold, &ci);
                pl->addCopyCompleteGold(GData::moneyNeed[GData::FRONTMAP_IM].gold);

                std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
                if (!tmp.size())
                    return;

                UInt8 max = GData::frontMapMaxManager[id];
                UInt8 nspot = tmp.size();
                if (nspot != 1 && !tmp[nspot-1].status)
                    --nspot;
                if (!nspot)
                    nspot = 1;

                for (UInt8 i = nspot; i <= max; ++i)
                {
                    if (!fight(pl, id, i, true, true))
                        break;
                }
            }
            break;

        default:
            break;
    }
}

void FrontMap::sendAutoFrontMap(Player* pl)
{
    if (!pl)
        return;

    UInt8 id = pl->GetVar(VAR_ATOFM);
    if (!id)
        return;

    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (!tmp.size())
        return;

    UInt8 nspot = tmp.size();
    if (nspot != 1 && !tmp[nspot-1].status)
        --nspot;
    if (!nspot)
        nspot = 1;

    Stream st(REP::AUTO_FRONTMAP);
    st << static_cast<UInt8>(0) << id << nspot << Stream::eos;
    pl->send(st);
}

} // namespace GObject

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


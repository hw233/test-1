﻿
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
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include "Package.h"
#include "GObject/Tianjie.h"
#include "GObject/Clan.h"

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
    DB3().PushUpdateData("DELETE FROM `auto_frontmap` WHERE playerId = %" I64_FMT "u", pl->getId());
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
    return 3;
    if (vipl == 1)
        return 1;
    if (vipl == 2)
        return 2;
    if (vipl >= 3)
        return 3;
    return 0;
}

UInt32 FrontMap::getEnterGold(Player* pl)
{
    UInt8 vipl = pl->getVipLevel();
     UInt32 VipType = pl->GetVar(VAR_VIP_PRIVILEGE_DATA_TYPE);
    if(vipl > 3  ||( pl->inVipPrivilegeTime()&& VipType % 2 ==0 ))
        vipl = 3;
    UInt32 extraVipGold[4][3] = {
        {20, 20, 20},
        { 0, 20, 20},
        { 0,  0, 20},
        { 0,  0,  0},
    };

    return (GData::moneyNeed[GData::FRONTMAP_ENTER1+PLAYER_DATA(pl, frontGoldCnt)].gold + extraVipGold[vipl][PLAYER_DATA(pl, frontGoldCnt)]);
}

void FrontMap::sendInfo(Player* pl, UInt8 id, bool needspot, bool force)
{

    UInt32 mark = pl->GetVar(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK);
    UInt8 pos = id - 1;
    pos = static_cast<UInt8>(GET_BIT_MARK(mark, pos));

    FastMutex::ScopedLock lk(_mutex);
    Stream st(REP::FORMATTON_INFO);
    UInt8 count = getCount(pl);
    st << static_cast<UInt8>(0);
    st << id;
    st << count;
    st << pos;

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
        DB3().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`) VALUES(%" I64_FMT "u, %u, 0, 0, 0)",
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
    static UInt8 lvls[] = {35, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95};
    static UInt16 spots[] = {1284, 2053, 4360, 4611, 5893, 5637, 8195, 6153, 9222, 9481, 10244, 5129};

    if (pl->getLocation() != spots[id-1])
    {
        SYSMSG_SENDV(2244, pl);
        return false;
    }

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

    if (!pl->hasChecked())
        return;

    if (!checkLevel(pl, id))
        return;

    FastMutex::ScopedLock lk(_mutex);

    UInt8 ret = 1;
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (!tmp.size()) {
        if (PLAYER_DATA(pl, frontFreeCnt) < getFreeCount()) {
            ++PLAYER_DATA(pl, frontFreeCnt);
            tmp.resize(1);
            DB3().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`) VALUES(%" I64_FMT "u, %u, 0, 0, 0)",
                    pl->getId(), id);
            ret = 0;
            pl->frontMapUdpLog(id, 1);
        } else if (PLAYER_DATA(pl, frontGoldCnt) < getGoldCount(pl->getVipLevel())) {
            UInt32 gold = getEnterGold(pl);
            if (pl->getGold() < gold) {
                Stream st(REP::FORMATTON_INFO);
                st << static_cast<UInt8>(1) << id << static_cast<UInt8>(1) << Stream::eos;
                pl->send(st);
                pl->sendMsgCode(0, 1104);
                return;
            }

            ++PLAYER_DATA(pl, frontGoldCnt);
            if(PLAYER_DATA(pl, frontGoldCnt) == getGoldCount(pl->getVipLevel()) && World::get11Time() && pl->getClan()!=NULL)
                SYSMSG_BROADCASTV(4958, pl->getClan()->getName().c_str(),pl->getCountry(), pl->getPName());

            tmp.resize(1);
            tmp[0].lootlvl = PLAYER_DATA(pl, frontGoldCnt);
            DB3().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`, `lootlvl`) VALUES(%" I64_FMT "u, %u, 0, 0, 0, %u)",
                    pl->getId(), id, PLAYER_DATA(pl, frontGoldCnt));
            ret = 0;

            ConsumeInfo ci(EnterFrontMap,0,0);
            pl->useGold(gold, &ci);
            pl->frontMapUdpLog(id, 3);
        } else {
            // XXX:
            return;
        }
    } else {
        ret = 0;
    }

    if (!ret) {
        sendInfo2(pl, id, true);

        UInt8 count = getCount(pl);

        PLAYER_DATA(pl, frontUpdate) = TimeUtil::Now();
        DB1().PushUpdateData("UPDATE `player` SET `frontFreeCnt` = %u, `frontGoldCnt` = %u, `frontUpdate` = %u WHERE `id` = %" I64_FMT "u",
                PLAYER_DATA(pl, frontFreeCnt), PLAYER_DATA(pl,frontGoldCnt), TimeUtil::Now(), pl->getId());

        Stream st(REP::FORMATTON_INFO);
        st << static_cast<UInt8>(3) << count << Stream::eos;
        pl->send(st);
        GameAction()->doStrong(pl, SthFormation, 0 ,0 );
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
        DB1().PushUpdateData("UPDATE `player` SET `frontFreeCnt` = 0, `frontGoldCnt` = 0, `frontUpdate` = %u WHERE `id` = %" I64_FMT "u",
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
    // 是否在次数范围内
    if (PLAYER_DATA(pl, frontFreeCnt) > getFreeCount() &&
            PLAYER_DATA(pl, frontGoldCnt) > getGoldCount(pl->getVipLevel()))
        return 0;

    // 是否进入过阵图
    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (!tmp.size())
        return 0;

    // 阵图据点是否合法
    if (spot > GData::frontMapMaxManager[id])
        return 0;

    // 是否跳点打
    if (spot > 1 && spot > tmp.size())
        return 0;

    if (pl->hasFlag(Player::AutoFrontMap) && !ato) {
        // pl->sendMsgCode(0, 1414);
        return 0;
    }

    if (spot >= tmp.size()) {
        tmp.resize(spot+1);
        tmp[spot].lootlvl = tmp[spot-1].lootlvl;
        DB3().PushUpdateData("REPLACE INTO `player_frontmap`(`playerId`, `id`, `spot`, `count`, `status`, `lootlvl`) VALUES(%" I64_FMT "u, %u, %u, 0, 0, %u)",
                pl->getId(), id, spot, tmp[spot].lootlvl);
    }

    Stream st(REP::FORMATTON_INFO);
    UInt8 count = tmp[spot].count;
    if (count >= GData::frontMapManager[id][spot].count) {
        st << static_cast<UInt8>(5) << id << spot << static_cast<UInt8>(0) << Stream::eos;
        pl->send(st);
        return 0;
    }

    bool ret = false;
    UInt32 fgtid = GData::frontMapManager[id][spot].fighterId;
    if (!fgtid)
        return 0;

    pl->OnHeroMemo(MC_SLAYER, MD_MASTER, 1, 0);

    std::vector<UInt16> loot;
    bool isFull = false;
    if (pl->attackCopyNpc(fgtid, 0, id, World::_wday==7?2:1, isFull, tmp[spot].lootlvl, ato, &loot)) {
        ret = true;
        if (ato)
            pl->checkLastBattled();
    }

    if (ret) {
        ++tmp[spot].count;
        tmp[spot].status = 1;

        if (spot >= GData::frontMapMaxManager[id]) { // 通关
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
            if (!PLAYER_DATA(pl, frontGoldCnt))
                pl->frontMapUdpLog(id,2);
            else
                pl->frontMapUdpLog(id,4);
            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA(pl, created));
            if(id == 2 && thisDay <= endDay)
            {
                UInt32 targetVal = pl->GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_FRONT_MAP))
                {
                    targetVal |=TARGET_FRONT_MAP;
                    pl->AddVar(VAR_CTS_TARGET_COUNT, 1);
                    pl->SetVar(VAR_CLAWARD2, targetVal);
                    pl->sendNewRC7DayTarget();
                    pl->newRC7DayUdpLog(1152, 8);
                }
            }


            if(World::getFourCopAct())
            {
                UInt32 randNum;
                if(PLAYER_DATA(pl, frontFreeCnt) == getFreeCount() && PLAYER_DATA(pl, frontGoldCnt) > 0)
                {
                    if(3 <= PLAYER_DATA(pl, frontGoldCnt))
                        randNum = 15;
                    else if(2 == PLAYER_DATA(pl, frontGoldCnt))
                        randNum = 12 + uRand(3);
                    else
                        randNum = 11 + uRand(2);
                }
                else
                    randNum = 10;
                pl->GetPackage()->AddItem2(9209, randNum, true, true);
            }
            if(World::getGoldSnakeAct())
            {
                UInt32 num = 0;
                if(PLAYER_DATA(pl, frontFreeCnt) == getFreeCount() && PLAYER_DATA(pl, frontGoldCnt) > 0)
                {
                    if(3 <= PLAYER_DATA(pl, frontGoldCnt))
                        num = 3;
                    else if(2 == PLAYER_DATA(pl, frontGoldCnt))
                        num = 2;
                    else
                        num = 1;
                }
                else
                    num = 1;
                pl->GetPackage()->Add(9314, num, true, false);
            }
            if (GObject::Tianjie::instance().isTjOpened())
            { 
                pl->GetPackage()->AddItem(9138, 1, false, false);
            }

            UInt32 mark = pl->GetVar(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK);
            UInt8 pos = id - 1;
            mark = CLR_BIT(mark, pos);
            pl->SetVar(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, mark);

            GameAction()->onFrontMapWin(pl, id, spot, tmp[spot].lootlvl);
            pl->copyFrontWinAward(2, tmp[spot].lootlvl > 0);
            DB3().PushUpdateData("DELETE FROM `player_frontmap` WHERE `playerId` = %" I64_FMT "u AND `id` = %u", pl->getId(), id);
            if (ato)
                autoClear(pl, complate);

            pl->OnHeroMemo(MC_SLAYER, MD_MASTER, 1, 2);
            if (!pl->GetShuoShuo()->getShuoShuo(id-1 + SS_FM1))
                pl->OnShuoShuo(id-1 + SS_FM1);
            pl->setContinuousRFAward(5);
            return 2;
        } else { // 打过某一点
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
                        autoClear(pl, complate);
                        st << static_cast<UInt8>(5);
                    } else {
                        st << static_cast<UInt8>(3);
                    }
                    st << id << spot;

                    st << static_cast<UInt8>(size/2);
                    for (UInt8 i = 1; i < rsize*2+1; i += 2)
                        st << loot[i] << static_cast<UInt8>(loot[i+1]);
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

        DB3().PushUpdateData("UPDATE `player_frontmap` SET `count`=%u,`status`=%u WHERE `playerId` = %" I64_FMT "u AND `id` = %u AND `spot`=%u",
                tmp[spot].count, tmp[spot].status, pl->getId(), id, spot);
    }
    else
    {
        if (isFull)
        {
            Stream st(REP::AUTO_FRONTMAP);
            st << static_cast<UInt8>(5) << Stream::eos;
            pl->send(st);
            autoClear(pl, complate);
            return 0;
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
    }

    st << static_cast<UInt8>(5) << id << spot << static_cast<UInt8>(GData::frontMapManager[id][spot].count - tmp[spot].count) << Stream::eos;
    pl->send(st);

    return ret?1:0;
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

    UInt32 mark = pl->GetVar(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK);
    UInt8 pos = id - 1;
    mark = CLR_BIT(mark, pos);
    pl->SetVar(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, mark);

    TRACE_LOG("%s: %" I64_FMT "u, %u", __PRETTY_FUNCTION__, pl->getId(), id);
    DB3().PushUpdateData("DELETE FROM `player_frontmap` WHERE `playerId` = %" I64_FMT "u AND `id` = %u", pl->getId(), id);
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
    //阵图广播
    if (!pl || !id)
        return;

    std::vector<FrontMapData>& tmp = m_frts[pl->getId()][id];
    if (!tmp.size())
        return;

    UInt32 mark = pl->GetVar(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK); 
    UInt8 pos = id - 1;
    switch (type)
    {
        case 0:
            {
                if (!init) {
                    if (pl->hasFlag(Player::AutoFrontMap)) {
                        pl->sendMsgCode(0, 1414);
                        return;
                    }

                    if (!checkLevel(pl, id))
                        return;

                    pl->OnHeroMemo(MC_SLAYER, MD_MASTER, 1, 1);

                    bool girl = (World::getGirlDay() && !pl->IsMale());
                    if (!World::getNewYear() &&
                        !girl &&
                        !World::getNetValentineDay() &&
                        0 == GET_BIT_MARK(mark, pos))
                    {
                        UInt32 pref = 0;
                        UInt8 div = 1;
                        UInt32 VipType = pl->GetVar(VAR_VIP_PRIVILEGE_DATA_TYPE);
                        if (pl->getVipLevel() >= 5 ||( pl->inVipPrivilegeTime()&& VipType%2== 0 ) )
                            pref = 1000;
                        if (World::_wday == 7)
                            div = 2;
                        if (mtype == 1)
                        {
                            if (GData::moneyNeed[GData::FRONTMAP_AUTO].gold/div > pl->getGoldOrCoupon()) {
                                pl->sendMsgCode(0, 1104);
                                return;
                            } else {
                                ConsumeInfo ci(EnterAutoFrontMap,0,0);
                                pl->useGoldOrCoupon(GData::moneyNeed[GData::FRONTMAP_AUTO].gold/div, &ci);
                            }
                        }
                        else
                        {
                            if ((GData::moneyNeed[GData::FRONTMAP_AUTO1+id-1].tael - pref)/div > pl->getTael()) {
                                pl->sendMsgCode(0, 1100);
                                return;
                            } else {
                                ConsumeInfo ci(EnterAutoFrontMap,0,0);
                                pl->useTael((GData::moneyNeed[GData::FRONTMAP_AUTO1+id-1].tael - pref)/div, &ci);
                            }
                        }

                        mark = SET_BIT(mark, pos);
                        pl->SetVar(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, mark);
                    }
                }

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
                DB3().PushUpdateData("REPLACE INTO `auto_frontmap` (`playerId`, `id`) VALUES (%" I64_FMT "u, %u)", pl->getId(), id);

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

                if (!World::getAutoBattleAct() && !World::getNewYear() && !pl->isYD() && !pl->isBD() && !pl->isQQVIP())
                {
                    // 限时vip特权
                    UInt32 VipType = pl->GetVar(VAR_VIP_PRIVILEGE_DATA_TYPE);
                    if (pl->getVipLevel() < 7 && !(pl->inVipPrivilegeTime() &&( VipType %2 == 0)))
                        return;
                }

                if (!pl->hasChecked())
                    return;

                if (GData::moneyNeed[GData::FRONTMAP_IM].gold > pl->getGoldOrCoupon())
                {
                    pl->sendMsgCode(0, 1104);
                    return;
                }

                ConsumeInfo ci(AutoFrontMapComplete,0,0);
                pl->useGoldOrCoupon(GData::moneyNeed[GData::FRONTMAP_IM].gold, &ci);
                pl->addCopyCompleteGold(GData::moneyNeed[GData::FRONTMAP_IM].gold);

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
                pl->setBuffData(PLAYER_BUFF_ATTACKING, 0);

                mark = CLR_BIT(mark, pos);
                pl->SetVar(VAR_FRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, mark);
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



#include "XJFrontMap.h"
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
#include "Package.h"

namespace GObject
{

    void autoClear1(Player* pl, bool complete = false, UInt8 id = 0, UInt8 spot = 0)
    {
        if (!pl)
            return;

        if (complete)
        {
        }

        PopTimerEvent(pl, EVENT_AUTOXJFRONTMAP, pl->getId());
        pl->delFlag(Player::AutoXJFrontMap);
        pl->SetVar(VAR_ATOXJFM, 0);
        DB3().PushUpdateData("DELETE FROM `auto_xjfrontmap` WHERE playerId = %" I64_FMT "u", pl->getId());
    }

    void XJFrontMap::sendInfo(Player* pl, UInt8 id ,bool needspot, bool force)
    {
        UInt32 mark = pl->GetVar(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK);
        UInt8 pos = id - 1;
        pos = static_cast<UInt8>(GET_BIT_MARK(mark,pos));

        FastMutex::ScopedLock lk(_mutex);
        Stream st(REP::XJFRONTMAP);
        UInt8 count = getCount(pl);
        st << static_cast<UInt8>(1);//璇玑阵图信息
        st << static_cast<UInt8>(0);
        st << id;
        st << count;
        st << pos;

        if (needspot)
            sendFrontMap(st, pl, id, needspot?true:force);
        st << Stream::eos;
        pl->send(st);

    }
    
    void XJFrontMap::sendInfo2(Player* pl, UInt8 id, bool succ)
    {
        Stream st(REP::XJFRONTMAP);
        st << static_cast<UInt8>(1);//璇玑阵图信息
        st << static_cast<UInt8>(1);
        st << id;

        if (succ)
            sendFrontMap(st, pl, id, true);
        else
            st << static_cast<UInt8>(0);

        st << Stream::eos;
        pl->send(st);
    }
   
    UInt8 XJFrontMap::getFreeCount()
    {
        return FREECNT;
    }

    UInt8 XJFrontMap::getGoldCount(UInt8 xjfrontGoldCnt)
    {
        UInt8 ret_cnt = GOLDCNT - xjfrontGoldCnt;
        return ret_cnt;
    }

    UInt32 XJFrontMap::getEnterGold(Player* pl)
    {
        //TODO 额外副本仙石  20 40 60 
        return (GData::moneyNeed[GData::FRONTMAP_ENTER1+PLAYER_DATA(pl, xjfrontGoldCnt)].gold) ;
    }

    UInt8 XJFrontMap::getCount(Player* pl)
    {
        if (!pl)
            return 0;

        if (!TimeUtil::SameDay(TimeUtil::Now(), PLAYER_DATA(pl, xjfrontUpdate)) ||
                getGoldCount() < PLAYER_DATA(pl, xjfrontGoldCnt) ||
                getFreeCount() < PLAYER_DATA(pl, xjfrontFreeCnt)) {
            PLAYER_DATA(pl, xjfrontUpdate) = TimeUtil::Now();
            PLAYER_DATA(pl, xjfrontFreeCnt) = 0;
            PLAYER_DATA(pl, xjfrontGoldCnt) = 0;
            DB1().PushUpdateData("UPDATE `player` SET `xjfrontFreeCnt` = 0, `xjfrontGoldCnt` = 0, `xjfrontUpdate` = %u WHERE `id` = %" I64_FMT "u",
                    PLAYER_DATA(pl, xjfrontUpdate), pl->getId());
        }

        UInt8 count = getGoldCount(PLAYER_DATA(pl, xjfrontGoldCnt));
        count <<= 4;
        count |= getFreeCount()-PLAYER_DATA(pl, xjfrontFreeCnt);
        return count;
    }

    void XJFrontMap::sendFrontMap(Stream& st, Player* pl, UInt8 id, bool force)
    {
        std::vector<XJFrontMapData>& tmp = m_frts[pl->getId()][id];

        if (force && !tmp.size())
            return;

        if (!tmp.size()) {
            tmp.resize(1);
            DB3().PushUpdateData("REPLACE INTO `player_xjfrontmap`(`playerId`, `id`, `spot`, `count`, `status`) VALUES(%" I64_FMT "u, %u, 0, 0, 0)",
                    pl->getId(), id);
        }

        size_t off = st.size();
        st << static_cast<UInt8>(0);

        bool first = true;
        UInt8 size = 0;
        UInt8 max = GData::xjfrontMapMaxManager[id];
        for (UInt8 i = 1; i <= max; ++i) {
            std::vector<GData::FrontMapFighter>& fmf = GData::xjfrontMapManager[id];
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

    bool XJFrontMap::checkLevel(Player* pl, UInt8 id)
    {
        UInt8 ID = id - 100;
        static UInt8 lvls[] = {75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130};
        static UInt16 spots[] = {1284, 2053, 4360, 4611, 5893, 5637, 8195, 6153, 9222, 9481, 10244, 5129};

        if (pl->getLocation() != spots[ID-1])
        {
            SYSMSG_SENDV(2244, pl);
            return false;
        }

        if (pl->GetLev() < lvls[ID-1] || ID > sizeof(lvls)/sizeof(UInt8)) {
            SYSMSG_SENDV(2109, pl, pl->GetLev(), lvls[ID-1]);
            return false;
        }
        return true;
    }

    void XJFrontMap::enter(Player* pl, UInt8 id)
    {
        if (!pl || !id)
            return;

        if (!pl->hasChecked())
            return;

        if (!checkLevel(pl, id))
            return;

        FastMutex::ScopedLock lk(_mutex);

        UInt8 ret = 1;
        std::vector<XJFrontMapData>& tmp = m_frts[pl->getId()][id];
        if (!tmp.size()) {
            if (PLAYER_DATA(pl, xjfrontFreeCnt) < getFreeCount()) {
                ++PLAYER_DATA(pl, xjfrontFreeCnt);
                tmp.resize(1);
                DB3().PushUpdateData("REPLACE INTO `player_xjfrontmap`(`playerId`, `id`, `spot`, `count`, `status`) VALUES(%" I64_FMT "u, %u, 0, 0, 0)",
                        pl->getId(), id);
                ret = 0;
                //TODO 璇玑阵图udplog
                //pl->frontMapUdpLog(id, 1);
            } else if (PLAYER_DATA(pl, xjfrontGoldCnt) < getGoldCount()) {
                UInt32 gold = getEnterGold(pl);
                if (pl->getGold() < gold) {
                    Stream st(REP::XJFRONTMAP);
                    st << static_cast<UInt8>(1);//璇玑阵图信息
                    st << static_cast<UInt8>(1) << id << static_cast<UInt8>(1) << Stream::eos;
                    pl->send(st);
                    pl->sendMsgCode(0, 1104);
                    return;
                }

                ++PLAYER_DATA(pl, xjfrontGoldCnt);

                tmp.resize(1);
                tmp[0].lootlvl = PLAYER_DATA(pl, xjfrontGoldCnt);
                DB3().PushUpdateData("REPLACE INTO `player_xjfrontmap`(`playerId`, `id`, `spot`, `count`, `status`, `lootlvl`) VALUES(%" I64_FMT "u, %u, 0, 0, 0, %u)",
                        pl->getId(), id, PLAYER_DATA(pl, xjfrontGoldCnt));
                ret = 0;

                ConsumeInfo ci(EnterXJFrontMap,0,0);
                pl->useGold(gold, &ci);
                //TODO 璇玑阵图udplog
                //pl->frontMapUdpLog(id, 3);
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

            PLAYER_DATA(pl, xjfrontUpdate) = TimeUtil::Now();
            DB1().PushUpdateData("UPDATE `player` SET `xjfrontFreeCnt` = %u, `xjfrontGoldCnt` = %u, `xjfrontUpdate` = %u WHERE `id` = %" I64_FMT "u",
                    PLAYER_DATA(pl, xjfrontFreeCnt), PLAYER_DATA(pl,xjfrontGoldCnt), TimeUtil::Now(), pl->getId());

            Stream st(REP::XJFRONTMAP);
            st << static_cast<UInt8>(1);//璇玑阵图信息
            st << static_cast<UInt8>(3) << count << Stream::eos;
            pl->send(st);
        } else {
            sendInfo2(pl, id, false);
        }
    }

    UInt8 XJFrontMap::fight(Player* pl, UInt8 id, UInt8 spot, bool ato, bool complate)
    {
        if (!pl || !id || !spot)
            return 0;

        FastMutex::ScopedLock lk(_mutex);
        // 是否在次数范围内
        if (PLAYER_DATA(pl, xjfrontFreeCnt) > getFreeCount() &&
                PLAYER_DATA(pl, xjfrontGoldCnt) > getGoldCount())
            return 0;

        // 是否进入过阵图
        std::vector<XJFrontMapData>& tmp = m_frts[pl->getId()][id];
        if (!tmp.size())
            return 0;

        // 阵图据点是否合法
        if (spot > GData::xjfrontMapMaxManager[id])
            return 0;

        // 是否跳点打
        if (spot > 1 && spot > tmp.size())
            return 0;

        if (pl->hasFlag(Player::AutoXJFrontMap) && !ato) {
            // pl->sendMsgCode(0, 1414);
            return 0;
        }
        if (pl->hasFlag(Player::AutoFrontMap) && !ato) {
            pl->sendMsgCode(0, 1414);
            return 0;
        }

        if (spot >= tmp.size()) {
            tmp.resize(spot+1);
            tmp[spot].lootlvl = tmp[spot-1].lootlvl;
            DB3().PushUpdateData("REPLACE INTO `player_xjfrontmap`(`playerId`, `id`, `spot`, `count`, `status`, `lootlvl`) VALUES(%" I64_FMT "u, %u, %u, 0, 0, %u)",
                    pl->getId(), id, spot, tmp[spot].lootlvl);
        }

        Stream st(REP::XJFRONTMAP);
        st << static_cast<UInt8>(1);//璇玑阵图信息
        UInt8 count = tmp[spot].count;
        if (count >= GData::xjfrontMapManager[id][spot].count) {
            st << static_cast<UInt8>(5) << id << spot << static_cast<UInt8>(0) << Stream::eos;
            pl->send(st);
            return 0;
        }

        bool ret = false;
        UInt32 fgtid = GData::xjfrontMapManager[id][spot].fighterId;
        if (!fgtid)
            return 0;

        std::vector<UInt16> loot;
        bool isFull = false;
        UInt64 exp = 0;
        if (pl->attackCopyNpc(fgtid, 0, id, 1, isFull,exp ,tmp[spot].lootlvl, ato, &loot)) {
            ret = true;
            if (ato)
                pl->checkLastBattled();
        }

        if (ret) {
            ++tmp[spot].count;
            tmp[spot].status = 1;

            if (spot >= GData::xjfrontMapMaxManager[id]) { // 通关
                if (ato)
                {
                    Stream st(REP::XJFRONTMAP);
                    st << static_cast<UInt8>(2);//璇玑阵图自动战斗
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

                    st<<static_cast<UInt64>(exp);
                    st << Stream::eos;
                    pl->send(st);
                }
                else
                {
                    Stream st(REP::XJFRONTMAP);
                    st << static_cast<UInt8>(1);//璇玑阵图信息
                    st << static_cast<UInt8>(4) << id << Stream::eos;
                    pl->send(st);
                }
                tmp.resize(0);
              /*  if (!PLAYER_DATA(pl, xjfrontGoldCnt))
                    pl->frontMapUdpLog(id,2);
                else
                    pl->frontMapUdpLog(id,4);*/

                UInt32 mark = pl->GetVar(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK);
                UInt8 pos = id - 1;
                mark = CLR_BIT(mark, pos);
                pl->SetVar(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, mark);

                DB3().PushUpdateData("DELETE FROM `player_xjfrontmap` WHERE `playerId` = %" I64_FMT "u AND `id` = %u", pl->getId(), id);
                if (ato)
                    autoClear1(pl, complate);

                return 2;
            } else { // 打过某一点
                UInt8 nspot = spot+1;
                while (!GData::xjfrontMapManager[id][nspot].count && nspot <= GData::xjfrontMapMaxManager[id])
                    ++nspot;

                if (ato)
                {
                    Stream st(REP::XJFRONTMAP);
                    st << static_cast<UInt8>(2);//璇玑阵图信息
                    UInt8 size = loot.size();
                    if (size) {
                        UInt8 rsize = loot[0];
                        if (rsize != size/2) {
                            autoClear1(pl, complate);
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

                    st << static_cast<UInt64>(exp);
                    st << Stream::eos;
                    pl->send(st);
                }
                else
                {
                    Stream st(REP::XJFRONTMAP);
                    st << static_cast<UInt8>(1);//璇玑阵图信息
                    st << static_cast<UInt8>(5) << id << nspot;

                    if (nspot < tmp.size())
                        st << static_cast<UInt8>(GData::xjfrontMapManager[id][nspot].count - tmp[nspot].count);
                    else
                        st << GData::xjfrontMapManager[id][nspot].count;

                    st << Stream::eos;
                    pl->send(st);
                }
            }

            DB3().PushUpdateData("UPDATE `player_xjfrontmap` SET `count`=%u,`status`=%u WHERE `playerId` = %" I64_FMT "u AND `id` = %u AND `spot`=%u",
                    tmp[spot].count, tmp[spot].status, pl->getId(), id, spot);
        }
        else
        {
            if (isFull)
            {
                Stream st(REP::XJFRONTMAP);
                st << static_cast<UInt8>(2);//璇玑阵图自动战斗
                st << static_cast<UInt8>(5) << Stream::eos;
                pl->send(st);
                autoClear1(pl, complate);
                return 0;
            }
            else
            {

                if (ato)
                {
                    Stream st(REP::XJFRONTMAP);
                    st << static_cast<UInt8>(2);//璇玑阵图自动战斗
                    st << static_cast<UInt8>(2) << id << spot<<static_cast<UInt64>(exp) << Stream::eos;
                    pl->send(st);

                    autoClear1(pl, complate);
                    return 0;
                }
            }
        }

        st << static_cast<UInt8>(5) << id << spot << static_cast<UInt8>(GData::xjfrontMapManager[id][spot].count - tmp[spot].count) << Stream::eos;
        pl->send(st);

        return ret?1:0;
    }

    void XJFrontMap::reset(Player* pl, UInt8 id)
    {
        if (!pl || !id)
            return;

        FastMutex::ScopedLock lk(_mutex);

        Stream st(REP::XJFRONTMAP);
        st << static_cast<UInt8>(1);//璇玑阵图信息
        std::vector<XJFrontMapData>& tmp = m_frts[pl->getId()][id];
        if (!tmp.size()) {
            st << static_cast<UInt8>(2) << id << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);

            return;
        }

        tmp.resize(0);
        st << static_cast<UInt8>(2) << id << static_cast<UInt8>(0) << Stream::eos;
        pl->send(st);

        UInt32 mark = pl->GetVar(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK);
        UInt8 pos = id - 1;
        mark = CLR_BIT(mark, pos);
        pl->SetVar(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, mark);

        TRACE_LOG("%s: %" I64_FMT "u, %u", __PRETTY_FUNCTION__, pl->getId(), id);
        DB3().PushUpdateData("DELETE FROM `player_xjfrontmap` WHERE `playerId` = %" I64_FMT "u AND `id` = %u", pl->getId(), id);
    }

    void XJFrontMap::addPlayer(UInt64 playerId, UInt8 id, UInt8 spot, UInt8 count, UInt8 status, UInt8 lootlvl)
    {
        if (!playerId || !id)
            return;

        std::vector<XJFrontMapData>& tmp = m_frts[playerId][id];
        if (tmp.size() <= spot)
            tmp.resize(spot+1);

        tmp[spot].count = count;
        tmp[spot].status = status?1:0;
        tmp[spot].lootlvl = lootlvl;
    }

    void XJFrontMap::autoBattle(Player* pl, UInt8 id, UInt8 type, UInt8 mtype, bool init)
    {
        //阵图广播
        if (!pl || !id)
            return;

        std::vector<XJFrontMapData>& tmp = m_frts[pl->getId()][id];
        if (!tmp.size())
            return;

        UInt32 mark = pl->GetVar(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK); 
        UInt8 pos = id - 1;
        switch (type)
        {
            case 0:
                {
                    if (!init) {
                        if (pl->hasFlag(Player::AutoXJFrontMap)) {
                            pl->sendMsgCode(0, 1414);
                            return;
                        }
                        if (pl->hasFlag(Player::AutoFrontMap)) {
                            pl->sendMsgCode(0, 1414);
                            return;
                        }

                        if (!checkLevel(pl, id))
                            return;

                        if (0 == GET_BIT_MARK(mark, pos))
                        {
                            if (mtype == 1)
                            {
                                if (GData::moneyNeed[GData::FRONTMAP_AUTO].gold > pl->getGoldOrCoupon()) {
                                    pl->sendMsgCode(0, 1104);
                                    return;
                                } else {
                                    ConsumeInfo ci(EnterAutoXJFrontMap,0,0);
                                    pl->useGoldOrCoupon(GData::moneyNeed[GData::FRONTMAP_AUTO].gold, &ci);
                                }
                            }
                            else
                            {
                                if ((GData::moneyNeed[GData::FRONTMAP_AUTO2].tael) > pl->getTael()) {
                                    pl->sendMsgCode(0, 1100);
                                    return;
                                } else {
                                    ConsumeInfo ci(EnterAutoXJFrontMap,0,0);
                                    pl->useTael((GData::moneyNeed[GData::FRONTMAP_AUTO2].tael), &ci);
                                }
                            }

                            mark = SET_BIT(mark, pos);
                            pl->SetVar(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, mark);
                        }
                    }

                    UInt8 max = GData::xjfrontMapMaxManager[id];
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

                    EventAutoXJFrontMap* event = new (std::nothrow) EventAutoXJFrontMap(pl, secs, count, id, nspot);
                    if (!event) return;
                    PushTimerEvent(event);

                    pl->addFlag(Player::AutoXJFrontMap);
                    pl->SetVar(VAR_ATOXJFM, id);
                    DB3().PushUpdateData("REPLACE INTO `auto_xjfrontmap` (`playerId`, `id`) VALUES (%" I64_FMT "u, %u)", pl->getId(), id);

                    Stream st(REP::XJFRONTMAP);
                    st << static_cast<UInt8>(2);//璇玑阵图自动战斗
                    st << static_cast<UInt8>(0) << id << nspot << Stream::eos;
                    pl->send(st);
                }
                break;

            case 1:
                {
                    if (!pl->hasFlag(Player::AutoXJFrontMap)) {
                        pl->sendMsgCode(0, 1415);
                        return;
                    }

                    autoClear1(pl);

                    Stream st(REP::XJFRONTMAP);
                    st << static_cast<UInt8>(2);//璇玑阵图自动战斗
                    st << static_cast<UInt8>(1) << id << Stream::eos;
                    pl->send(st);
                }
                break;

            case 2:
                {
                    if (!pl->hasFlag(Player::AutoXJFrontMap)) {
                        pl->sendMsgCode(0, 1415);
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

                    UInt8 max = GData::xjfrontMapMaxManager[id];
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
                    pl->SetVar(VAR_XJFRONTMAP_AUTO_FIGHT_USE_MONEY_MARK, mark);
                }
                break;

            default:
                break;
        }
    }

    void XJFrontMap::sendAutoFrontMap(Player* pl)
    {
        if (!pl)
            return;

        UInt8 id = pl->GetVar(VAR_ATOXJFM);
        if (!id)
            return;

        std::vector<XJFrontMapData>& tmp = m_frts[pl->getId()][id];
        if (!tmp.size())
            return;

        UInt8 nspot = tmp.size();
        if (nspot != 1 && !tmp[nspot-1].status)
            --nspot;
        if (!nspot)
            nspot = 1;

        Stream st(REP::XJFRONTMAP);
        st << static_cast<UInt8>(2);//璇玑阵图自动战斗
        st << static_cast<UInt8>(0) << id << nspot << Stream::eos;
        pl->send(st);
    }

    void XJFrontMap::buildInfo(Player* pl, Stream& st)
    {
        if(!pl)
            return;

        std::map<UInt8, std::vector<XJFrontMapData>>& pl_frts = m_frts[pl->getId()];
        for (std::map<UInt8, std::vector<XJFrontMapData>>::iterator
                it = pl_frts.begin(), end = pl_frts.end(); it != end; ++it)
            st << static_cast<UInt8>(it->first);
    }

    UInt8 XJFrontMap::getFrontMapSize(Player* pl)
    {
        if(!pl)
            return 0;

        return m_frts[pl->getId()].size();
    }

}

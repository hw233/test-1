
#include "Config.h"
#include "Player.h"
#include "Package.h"
#include "Clan.h"
#include "Log/Log.h"
#include "PracticePlace.h"
#include "Common/TimeUtil.h"
#include "GData/GDataManager.h"
#include "GObject/GObjectDBExecHelper.h"
#include "Server/WorldServer.h"
#include "Battle/BattleSimulator.h"
#include <sstream>
#include <mysql.h>
#include "Server/SysMsg.h"
#include "Mail.h"
#include "MsgID.h"

namespace GObject
{

UInt8 PracticePlace::_maxCount = 5;
UInt8 PracticePlace::_extraCount[11] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
UInt8 PracticePlace::_picCnt[11] = {0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2};


#define ITEM_PRACTICE_PROT 54
    // XXX: place must greater than 0
    bool PracticePlace::pay(Player* pl, UInt8 place, UInt16 slot,
            UInt8 type, UInt8 priceType, UInt8 time, UInt8 prot)
    {
        if (!pl || !time || !place || place > PPLACE_MAX || ((place != PPLACE_MAX) && slot > m_places[place-1].place.maxslot))
            return false;

        Stream st(REP::PRACTICE_OCCUPY);

        if (pl->isPracticing())
        {
            st << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            return false;
        }

        if (place != PPLACE_MAX)
        {
            if (!m_places[place-1].data.size()) { // XXX: no configuration
                st << static_cast<UInt8>(1) << Stream::eos;
                pl->send(st);
                return false;
            }
        }

        PracticeData* p = getPracticeData(pl);
        if (place != PPLACE_MAX && p && p->cdend > TimeUtil::Now())
        {
            st << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            return false;
        }

        PPlace& data = m_places[place-1].place;
        Player* owner = globalPlayers[data.ownerid];

        if(place != PPLACE_MAX && NULL == owner)
        {
            st << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            return false;
        }

        Clan* clan = NULL;
        if(NULL != owner)
            clan = owner->getClan();

        bool clanflag = (clan == pl->getClan());

        UInt32 slotprice = 0;
        UInt32 protprice = 0;
        ConsumeInfo ci(Practice,0,0);
        if(!clanflag)
        {
            if (place != PPLACE_MAX) {
                PracticeData*& pd = m_places[place-1].data[slot];
                if (pd)
                {
                    if (pd->winnerid != pl->getId())
                    {
                        st << static_cast<UInt8>(1) << Stream::eos;
                        pl->send(st);
                        return false;
                    }

                    Player* def = globalPlayers[pd->getId()];
                    if (!def)
                    {
                        st << static_cast<UInt8>(1) << Stream::eos;
                        pl->send(st);
                        return false;
                    }

                    UInt16 money =  ((float)pd->checktime / pd->traintime) * pd->slotprice;
                    UInt16 money2 =  ((float)pd->checktime / pd->traintime) * pd->protprice;
                    def->getTael(money + money2);
                    data.slotincoming += pd->slotprice - money;
                    data.protincoming += pd->protprice - money2;
                    DB1().PushUpdateData("UPDATE `practice_place` SET `protincoming` = %u, `slotincoming` = %u WHERE `id` = %u", data.protincoming, data.slotincoming, place);

                    pd->winnerid = 0;
                    m_places[PPLACE_MAX-1].data.push_back(pd);
                    def->setPracticingPlaceSlot(PPLACE_MAX << 16);
                    --m_places[place-1].used;
                    ++m_places[PPLACE_MAX-1].used;

                    if(clan)
                    {
                        clan->addClanFunds(pd->slotprice + pd->protprice - money - money2);
                        Stream st(REP::CLAN_BUILD);
                        UInt8 cnt = 2;
                        st << cnt;
                        UInt8 offset = st.size();
                        st << static_cast<UInt8>(2) << static_cast<UInt8>(m_places[place - 1].used);
                        st << static_cast<UInt8>(3) << data.slotincoming;
                        if(money2)
                        {
                            ++ cnt;
                            st << static_cast<UInt8>(4) << data.protincoming;
                        }
                        st.data<UInt8>(offset) = cnt;
                        st << Stream::eos;
                        clan->broadcast(st);
                    }

                    pd = 0;
                    // DB1().PushUpdateData("DELETE FROM `practice_data` WHERE `id` = %"I64_FMT"u)", def->getId());
                    DB1().PushUpdateData("UPDATE `practice_data` SET place = %u where `id`= %"I64_FMT"u", PPLACE_MAX, def->getId());
                    SYSMSG(title, 1000);
                    SYSMSGV(content, 1001, pl->getName().c_str());
                    def->GetMailBox()->newMail(NULL, 1, title, content);
                    //PopTimerEvent(def, EVENT_PLAYERPRACTICING, def->getId());
                }
            }

#if 0
            if (place == PPLACE_MAX || !data.slotmoney)
            {
                if (priceType == 0)
                {
                    const std::vector<UInt32>& golds = GData::GDataManager::GetGoldPractice();
                    if (!golds.size() || type >= golds.size())
                        return false;
                    price = time * golds[type];
                    if (pl->getGold() < price)
                    {
                        pl->sendMsgCode(0, 1101);
                        st << static_cast<UInt8>(1) << Stream::eos;
                        return false;
                    }
                    pl->useGold(price, &ci);
                }
                else
                {
                    const std::vector<UInt32>& taels = GData::GDataManager::GetTaelPractice();
                    if (!taels.size() || type >= taels.size()) {
                        st << static_cast<UInt8>(1) << Stream::eos;
                        return false;
                    }
                    price = time * taels[type];
                    if (pl->getTael() < price)
                    {
                        st << static_cast<UInt8>(1) << Stream::eos;
                        pl->sendMsgCode(0, 1100);
                        return false;
                    }
                    pl->useTael(price, &ci);
                    if(clan)
                        clan->addClanFunds(price);
                }
            }
            else
#endif
            if (place != PPLACE_MAX)
            {
                slotprice = time * data.slotmoney;
                if (slotprice) {
                    if (pl->getTael() < slotprice)
                    {
                        pl->sendMsgCode(0, 1100);
                        st << static_cast<UInt8>(1) << Stream::eos;
                        pl->send(st);
                        return false;
                    }
                    pl->useTael(slotprice, &ci);
                }
            }

            switch(prot)
            {
            case 1:
                if(pl->GetPackage()->DelItemAny(ITEM_PRACTICE_PROT, 1) == false)
                {
                    pl->sendMsgCode(0, 1055);
                    st << static_cast<UInt8>(1) << Stream::eos;
                    pl->send(st);
                    return false;
                }
                break;
            case 2:
                {
                    protprice = time * data.protmoney;
                    if (protprice) {
                        if (pl->getTael() < protprice)
                        {
                            pl->sendMsgCode(0, 1100);
                            st << static_cast<UInt8>(1) << Stream::eos;
                            pl->send(st);
                            return false;
                        }
                        pl->useTael(protprice, &ci);
                    }
                }
                break;
            }
        }

        PracticeData* pp = new (std::nothrow) PracticeData(pl->getId());
        if (!pp) {
            st << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            return false;
        }

		UInt32 now = TimeUtil::Now();

        pp->type = type;
        pp->pricetype = priceType;
        pp->slotprice = slotprice;
        pp->protprice = protprice;
        pp->traintime = 60 * time * 8;
        pp->checktime = pp->traintime;
        pp->trainend = now + pp->traintime;
        pp->prot = prot;
        pp->cdend = now + pp->traintime * 60 + 60 * 60;
        pp->winnerid = 0;

        DB1().PushUpdateData("REPLACE INTO `practice_data`(`id`, `place`, `slot`, `type`, `pricetype`, `slotprice`, `protprice`, `traintime`, `checktime`, `prot`, `cdend`, `winnerid`, `fighters`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %"I64_FMT"u, '')", pl->getId(), place, slot, type, priceType, slotprice, protprice, pp->traintime, pp->checktime, prot, pp->cdend, pp->winnerid);

        pl->setPracticingPlaceSlot(place << 16 | slot);
        addPractice(pl, pp, place, slot); // XXX: must be here after setPracticingPlaceSlot

        st << static_cast<UInt8>(0) << pp->traintime * 60 << prot << pl->getPIcCount() << static_cast<UInt8>(place - 1) << static_cast<UInt8>(0) << Stream::eos;
        pl->send(st);
        return true;
    }

    bool PracticePlace::stop(Player* pl)
    {
        if (!pl)
            return false;

        Stream st(REP::PRACTICE_STOP);

        if (!pl->isPracticing()) {
            st << static_cast<UInt32>(0) << static_cast<UInt16>(0) << static_cast<UInt16>(0) << Stream::eos;
            pl->send(st);
            return false;
        }

        UInt8 place = pl->getPracticePlace();
        UInt16 slot = pl->getPracticeSlot();

        PPlace& data = m_places[place-1].place;
        Player* owner = globalPlayers[data.ownerid];
        Clan* clan = NULL;
        if(NULL != owner)
            clan = owner->getClan();

        PracticeData* pd = getPracticeData(pl);
        if (pd)
        {
            pd->winnerid = 0;
            pd->fighters.clear();
            pd->cdend = TimeUtil::Now() + 10 * 60;

            //DB1().PushUpdateData("DELETE FROM `practice_data` WHERE `id` = %"I64_FMT"u)", pl->getId());
            DB1().PushUpdateData("UPDATE `practice_data` SET place = 7, slot = 0, checktime = 0, cdend = %u where `id`= %"I64_FMT"u", pd->cdend, pl->getId());
            PopTimerEvent(pl, EVENT_PLAYERPRACTICING, pl->getId());

            if(place == PPLACE_MAX || clan != pl->getClan())
                --m_places[place-1].used;

            UInt32 trained = pd->traintime - pd->checktime;
            UInt16 money = ((float)(trained)/pd->traintime)*pd->slotprice;
            UInt16 money2 = ((float)(trained)/pd->traintime)*pd->protprice;
            UInt16 remain = pd->slotprice + pd->protprice - money - money2;
            pl->getTael(remain);
            data.slotincoming += money;
            data.protincoming += money2;
            DB1().PushUpdateData("UPDATE `practice_place` SET `protincoming` = %u, `slotincoming` = %u WHERE `id` = %u", data.protincoming, data.slotincoming, place);

            pd->checktime = 0; // XXX:

            if(clan)
            {
                clan->addClanFunds(money + money2);
                Stream st(REP::CLAN_BUILD);
                UInt8 cnt = 2;
                st << cnt;
                UInt8 offset = st.size();
                st << static_cast<UInt8>(2) << static_cast<UInt8>(m_places[place - 1].used);
                st << static_cast<UInt8>(3) << data.slotincoming;
                if(money2)
                {
                    ++ cnt;
                    st << static_cast<UInt8>(4) << data.protincoming;
                }
                st.data<UInt8>(offset) = cnt;
                st << Stream::eos;
                clan->broadcast(st);
            }

            st << trained * 60 << money << remain << Stream::eos;
            //delPracticeData(pl);
        } else
        {
            st << static_cast<UInt32>(0) << static_cast<UInt16>(0) << static_cast<UInt16>(0) << Stream::eos;
        }
        pl->setPracticingPlaceSlot(0);
        m_places[place-1].data[slot] = 0;

        pl->send(st);
        return true;
    }

    bool PracticePlace::sitdown(Player* pl, UInt32* fgtid, size_t size)
    {
        if (!pl || !fgtid || !size)
            return false;

        Stream st(REP::PRACTICE_START);

        if (!pl->isPracticing())
        {
            st << static_cast<UInt8>(0) << Stream::eos;
            pl->send(st);
            return false;
        }

        PracticeData* data = getPracticeData(pl);
        if (!data)
        {
            st << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            return false;
        }

        int nVipLevel = pl->getVipLevel();
        UInt32 nCount = _maxCount + _extraCount[nVipLevel];

        if (data->fighters.size() > nCount)
        {
            st << static_cast<UInt8>(1) << Stream::eos;
            pl->send(st);
            return false;
        }

        //data->lock.lock();
        for (size_t i = 0; i < size && i < nCount; ++i)
        {
            if (!isSitdownYet(data, fgtid[i]))
            {
                if (pl->findFighter(fgtid[i]))
                {
                    data->fighters.push_back(fgtid[i]);
                    updateFighters(data->fighters, pl->getId());
                }
            }
        }
        //data->lock.unlock();

        st << static_cast<UInt8>(0) << Stream::eos;
        //pl->send(st);
        return true;
    }

    bool PracticePlace::standup(Player* pl, UInt32* fgtid, size_t size)
    {
        if (!pl || !fgtid || !size)
            return false;

        Stream st(REP::PRACTICE_STOP);

        PracticeData* data = getPracticeData(pl);
        if (!data)
        {
            st << static_cast<UInt32>(0) << static_cast<UInt16>(0) << static_cast<UInt16>(0) << Stream::eos;
            pl->send(st);
            return false;
        }

        //data->lock.lock();
        for (size_t i = 0; i < size; ++i)
        {
            if (isSitdownYet(data, fgtid[i]))
            {
                data->fighters.remove(fgtid[i]);
                updateFighters(data->fighters, pl->getId());
            }
        }
        //data->lock.unlock();

        // TODO: notify client
        return true;
    }

    void PracticePlace::updateFighters(std::list<UInt32>& fgts, UInt64 id)
    {
        size_t cnt = 0;
        size_t nfgt = fgts.size();
        std::ostringstream fighters;
        for (auto i = fgts.begin(), e = fgts.end(); i != e; ++i)
        {
            fighters << *i;
            if (cnt++ < nfgt - 1)
                fighters << ",";
        }

        DB1().PushUpdateData("UPDATE `practice_data` SET `fighters` = '%s' WHERE `id` = %"I64_FMT"u",
                fighters.str().c_str(), id);
    }

    void PracticePlace::getAllPlaceInfo(Player* pl)
    {
        Stream st(REP::PRACTICE_PLACE_IFNO);
        const std::vector<UInt32>& addons = GData::GDataManager::GetPlaceAddons();
        st << static_cast<UInt8>(PPLACE_MAX);
        for (int i = 0; i < PPLACE_MAX; ++i) {
            st << static_cast<UInt8>(i);
            if (i >= static_cast<int>(addons.size()))
                st << static_cast<UInt16>(100);
            else
                st << static_cast<UInt16>(addons[i]);

            PPlace& data = m_places[i].place;
            st << data.slotmoney;

            UInt64 ownerid = getPlaceOwnerId(i+1);
            if (ownerid)
            {
                Player* owner = globalPlayers[ownerid];
                if (owner)
                {
                    st << owner->getClanName();
                }
                else
                    st << "";
            } else {
                st << "";
            }

            st << static_cast<UInt8>(data.maxslot);
            st << static_cast<UInt8>(m_places[i].used);

            if (data.protid)
            {
                Player* prot = globalPlayers[data.protid];
                if (prot)
                {
                    UInt8 cls_sex = prot->GetClassAndSex();
                    st << prot->getName();
                    st << prot->GetLev();
                    st << static_cast<UInt8>((cls_sex >> 4) & 0x0F);
                    st << static_cast<UInt8>(cls_sex & 0x0F);
                }
                else
                {
                    st << "";
                    st << static_cast<UInt8>(0);
                    st << static_cast<UInt8>(0);
                    st << static_cast<UInt8>(0);
                }
            }
            else
            {
                st << "";
                st << static_cast<UInt8>(0);
                st << static_cast<UInt8>(0);
                st << static_cast<UInt8>(0);
            }
            st << data.enemyCount;
            st << data.winCount;
        }

        st << Stream::eos;
        pl->send(st);

        if(!pl->isPracticing())
            return;

        PracticeData* p = getPracticeData(pl);
        if (p)
        {
            Stream st1(REP::PRACTICE_OCCUPY);
            st1 << static_cast<UInt8>(0);
            st1 << p->checktime * 60;
            st1 << p->prot;
            st1 << pl->getPIcCount();
            st1 << static_cast<UInt8>(pl->getPracticePlace()-1);

            UInt8 size = p->fighters.size();
            st1 << size;

            for (std::list<UInt32>::iterator i = p->fighters.begin(), e = p->fighters.end(); i != e; ++i)
                st1 << *i;

            st1 << Stream::eos;
            pl->send(st1);
        }
    }

    void PracticePlace::getPlaceInfo(Player* pl, UInt8 place)
    {
    }

    void PracticePlace::getList(Player* pl, UInt8 place, UInt16 pageno, UInt16 pagenum)
    {
        if (!pl || !place || !pagenum || place > PPLACE_MAX)
            return;

        PlaceData& pd = m_places[place-1];
        if(pageno > pd.data.size()/pagenum)
            return;

        auto i = pd.data.begin();
        std::advance(i, pageno*pagenum);
        if (i == pd.data.end())
            return;

        Stream st(REP::PRACTICE_PAGE);
        st << static_cast<UInt8>(pageno);
        st << static_cast<UInt8>(0);
        PracticeData* ppd = NULL;
        int n = 0;
        int c = 0;
        for (auto e = pd.data.end(); i != e && n < pagenum; ++i, ++n)
        {
            ppd = *i;
            if (ppd) {
                Player* pl = globalPlayers[ppd->getId()];
                if (!pl)
                    continue;

                st << static_cast<UInt8>(n);
                st << pl->getName();
                st << pl->getCountry();

                Fighter* fgt = pl->getMainFighter();
                if (fgt)
                {
                    st << fgt->getClass();
                    st << fgt->getSex();
                    st << fgt->getLevel();
                }
                else
                {
                    st << static_cast<UInt8>(0);
                    st << static_cast<UInt8>(0);
                    st << static_cast<UInt8>(0);
                }
                st << ppd->prot;
                st << ppd->checktime*60;
                ++c;
            }
        }

        st.data<UInt8>(5) = c;
        st << Stream::eos;
        pl->send(st);
    }

    bool PracticePlace::replaceProtecter(Player* pl, UInt8 place, UInt64 protid)
    {
        if (!pl || !place || !protid)
            return false;
        PPlace& data = m_places[place-1].place;
        if (data.ownerid != pl->getId())
            return false;
        if (data.protid == protid)
            return false;
        Player* proter = globalPlayers[protid];
        if (!proter)
            return false;
        data.protid = protid;
        DB1().PushUpdateData("UPDATE `practice_place` SET `protid` = %"I64_FMT"u WHERE `id` = %u", protid, place);

        // TODO: notify client
        return true;
    }

    bool PracticePlace::replaceProtecter(Player* pl, UInt64 protid)
    {
        if (!pl || !protid)
            return false;

        bool flag = false;
        for(UInt8 idx = 0; idx < PPLACE_MAX; ++ idx)
        {
            flag = replaceProtecter(pl, idx + 1, protid);
            if(flag)
                break;
        }

        return flag;
    }

    bool PracticePlace::setCharges(Player* pl, UInt16 money)
    {
        if (!pl)
            return false;

        bool flag = false;
        for(UInt8 idx = 0; idx < PPLACE_MAX; ++ idx)
        {
            flag = setCharges(pl, idx + 1, money);
            if(flag)
                break;
        }

        return flag;
    }

    bool PracticePlace::setCharges(Player* pl, UInt8 place, UInt16 money)
    {
        if (!pl || !place || !money)
            return false;

        PPlace& data = m_places[place-1].place;

        if (data.ownerid != pl->getId())
            return false;

        data.slotmoney = money;
        DB1().PushUpdateData("UPDATE `practice_place` SET `slotmoney` = %u WHERE `id` = %u", money, place);

        // TODO: notify client
        Stream st(REP::CLAN_BUILD);
        st << static_cast<UInt8>(1) << static_cast<UInt8>(7) << data.slotmoney;
        st << Stream::eos;
        pl->send(st);

        return true;
    }

    bool PracticePlace::setProtCharges(Player* pl, UInt16 money)
    {
        if (!pl)
            return false;

        bool flag = false;
        for(UInt8 idx = 0; idx < PPLACE_MAX; ++ idx)
        {
            flag = setProtCharges(pl, idx + 1, money);
            if(flag)
                break;
        }

        return flag;
    }

    bool PracticePlace::setProtCharges(Player* pl, UInt8 place, UInt16 money)
    {
        if (!pl || !place || !money)
            return false;

        PPlace& data = m_places[place-1].place;

        if (data.ownerid != pl->getId())
            return false;

        data.protmoney = money;
        DB1().PushUpdateData("UPDATE `practice_place` SET `protmoney` = %u WHERE `id` = %u", money, place);

        // TODO: notify client
        Stream st(REP::CLAN_BUILD);
        st << static_cast<UInt8>(1) << static_cast<UInt8>(8) << data.protmoney;
        st << Stream::eos;
        pl->send(st);

        return true;
    }

    bool PracticePlace::doChallenge(Player* pl, UInt8 place, UInt16 idx, const std::string& name)
    {
        if (!pl || !place)
            return false;
        if (place == PPLACE_MAX) // XXX: 
            return false;
        
        if (idx > m_places[place-1].data.size())
            return false;

        Player* owner = globalPlayers[m_places[place-1].place.ownerid];
        if(NULL == owner)
            return false;

        PracticeData* p = getPracticeData(pl);
        if (p && p->cdend > TimeUtil::Now())
        {
            pl->sendMsgCode(0, 3100);
            return false;
        }


        Stream st(REP::PRACTICE_ROB);
        Player* def = 0;
        bool sumfalg = false;
        PracticeData* pd = m_places[place-1].data[idx];
        if (pd)
        {
            Player* opl = globalPlayers[pd->getId()];
            if (!opl || opl->getName() != name)
            {
                st << static_cast<UInt8>(2);
                st << Stream::eos;
                return false;
            }

            if (!pd->prot)
            {
                def = globalPlayers[pd->getId()];
            }
            else if (pd->prot == 1)
            {
                if (m_places[place-1].place.protid)
                {
                    def = globalPlayers[m_places[place-1].place.protid];
                    sumfalg = true;
                }
                else
                    def = globalPlayers[pd->getId()];
            }
            else if (pd->prot == 2)
            {
                st << static_cast<UInt8>(1);
                st << Stream::eos;
                pl->send(st);
                return false;
            }
        }

        if (!def) {
            st << static_cast<UInt8>(0);
            st << static_cast<UInt8>(place-1);
            st << static_cast<UInt8>(idx);
            st << Stream::eos;
            pl->send(st);
            return true;
        }

        Battle::BattleSimulator bsim(pl->getLocation(), pl, def, false);
        pl->PutFighters(bsim, 0, true);
        def->PutFighters(bsim, 1, true);
        bsim.start();
        
        Stream& packet = bsim.getPacket();
        if(packet.size() <= 8) {
            st << static_cast<UInt8>(1);
            st << Stream::eos;
            pl->send(st);
            return false;
        }

        bool res = bsim.getWinner() == 1;
        {
            Stream st(REP::ATTACK_NPC);
            if(res)
                st << static_cast<UInt16>(0x0101);
            else
                st << static_cast<UInt16>(0x0100);
            st << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
            st.append(&packet[8], packet.size() - 8);
            st << Stream::eos;
            pl->send(st);
        }

        // TODO: package
        if(sumfalg)
            ++m_places[place-1].place.enemyCount;

        if (res)
        {
            st << static_cast<UInt8>(0);
            st << static_cast<UInt8>(place-1);
            st << static_cast<UInt8>(idx);
            pd->winnerid = pl->getId();

            if(sumfalg)
            {
                ++m_places[place-1].place.winCount;
                Player* owner = GObject::globalPlayers[m_places[place -1].place.ownerid];
                if (owner)
                {
                    Stream st(REP::CLAN_BUILD);
                    st << static_cast<UInt8>(1) << static_cast<UInt8>(5) << m_places[place-1].place.winCount;
                    st << Stream::eos;
                    owner->send(st);
                }
            }
        } 
        else
        {
            st << static_cast<UInt8>(1);

            if(sumfalg)
            {
                Player* owner = GObject::globalPlayers[m_places[place -1].place.ownerid];
                if (owner)
                {
                    Stream st(REP::CLAN_BUILD);
                    st << static_cast<UInt8>(1) << static_cast<UInt8>(6) << m_places[place-1].place.enemyCount - m_places[place-1].place.winCount;
                    st << Stream::eos;
                    owner->send(st);
                }
            }
        }

        st << Stream::eos;
        pl->send(st);

        return true;
    }

    bool PracticePlace::addSlot(Player* pl)
    {
        if(!pl)
            return false;

        bool flag = false;
        for(UInt8 idx = 0; idx < PPLACE_MAX; ++ idx)
        {
            addSlot(pl, idx + 1);
            if(flag)
                break;
        }

        return flag;
   }

    bool PracticePlace::addSlot(Player* pl, UInt8 place)
    {
        if (!pl || !place)
            return false;
        if (place > PPLACE_MAX)
            return false;
        PlaceData& pd = m_places[place-1];
        if (pd.place.ownerid != pl->getId())
        {
            Player* owner = GObject::globalPlayers[pd.place.ownerid];
            if(!owner)
                return false;

            Clan* clan = owner->getClan();
            if(clan != pl->getClan())
                return false;
        }

        const std::vector<UInt32>& golds = GData::GDataManager::GetGoldOpenSlot();
        if (!golds.size() || pd.place.openslot >= golds.size())
            return false;
        UInt32 price = golds[pd.place.openslot]; 
        if (pl->getGold() < price)
        {
            pl->sendMsgCode(0, 1101);
            return false;
        }
        ConsumeInfo ci(AddPracticeSlot,0,0);
        pl->useGold(price, &ci);

        ++pd.place.maxslot;
        ++pd.place.openslot;
        pd.data.resize(pd.place.maxslot);

        DB1().PushUpdateData("UPDATE `practice_place` SET `maxslot` = %u, `openslot` = %u WHERE ownerid = %"I64_FMT"u", pd.place.maxslot, pd.place.openslot, pd.place.ownerid);

        // TODO: notify client
        Clan* clan = pl->getClan();
        if(clan)
        {
            UInt32 openPrice = 0;
            if (golds.size() && pd.place.openslot < golds.size())
                openPrice = golds[pd.place.openslot];
            Stream st(REP::CLAN_BUILD);
            st << static_cast<UInt8>(1);
            st << static_cast<UInt8>(1) << static_cast<UInt8>(pd.place.maxslot) << static_cast<UInt16>(openPrice);
            st << Stream::eos;
            clan->broadcast(st);
        }

        return true;
    }

    bool PracticePlace::addSlotFromTech(Player* pl, UInt8 place)
    {
        UInt8 idx = 0;
        if (!pl || place > PPLACE_MAX)
            return false;

        GObject::Clan* clan = pl->getClan();
        if(clan == NULL)
            return false;

        if(0 != place)
        {
            idx = place - 1;
        }
        else
        {
            UInt64 ownerid = clan->getOwner()->getId();
            for(; idx < PPLACE_MAX; idx ++)
            {
                if(ownerid == m_places[idx].place.ownerid)
                    break;
            }

            if(idx == PPLACE_MAX)
                return false;
        }

        PlaceData& pd = m_places[idx];
        if (pd.place.ownerid != pl->getId())
            return false;

        UInt8 techslot = clan->getPracticeSlot();
        UInt8 slotadd = techslot - pd.place.techslot;

        pd.place.maxslot += slotadd;
        pd.place.techslot += techslot;
        pd.data.resize(pd.place.maxslot);

        DB1().PushUpdateData("UPDATE `practice_place` SET `maxslot` = %u WHERE ownerid = %"I64_FMT"u", pd.place.maxslot, pd.place.ownerid);

        UInt32 openPrice = 0;
        const std::vector<UInt32>& golds = GData::GDataManager::GetGoldOpenSlot();
        if (golds.size() && pd.place.openslot < golds.size())
            openPrice = golds[pd.place.openslot];

        Stream st(REP::CLAN_BUILD);
        st << static_cast<UInt8>(1) << static_cast<UInt8>(1);
        st << static_cast<UInt8>(pd.place.maxslot) << static_cast<UInt16>(openPrice);
        st << Stream::eos;
        clan->broadcast(st);

        return true;
    }

    bool PracticePlace::addPlace(PPlace& place, UInt8 idx)
    {
        if (idx > PPLACE_MAX)
            return false;
        PlaceData& pd = m_places[idx];
        pd.place = place;
        pd.data.resize(place.maxslot);
        Player* pl = globalPlayers[pd.place.ownerid];
        if(!pl)
        {
            addSlotFromTech(pl, idx+1);
        }

        return true;
    }

    bool PracticePlace::addPractice(Player* pl, PracticeData* pd, UInt8 place, UInt8 slot)
    {
        if (!pl || !pd)
            return false;
        PracticeData*& oldpd = m_pradata[pd->getId()];
        if (oldpd) delete oldpd;
        oldpd = pd;

        if (pd->checktime)
        {
            if (!place)
            {
                DB1().PushUpdateData("DELETE FROM `practice_data` WHERE `id` = %"I64_FMT"u)", pl->getId());
                return false;
            }

            EventPlayerPractice* event = new (std::nothrow) EventPlayerPractice(pl, 60*10, pd->checktime/10, pd->trainend);
            if (event == NULL) return false;
            PushTimerEvent(event);

            Player* owner = globalPlayers[m_places[place-1].place.ownerid];
            GObject::Clan* clan = NULL;
            if(NULL != owner)
            {
                clan = owner->getClan();
            }

            if(place == PPLACE_MAX || clan != pl->getClan())
            {
                if(place == PPLACE_MAX)
                    m_places[place-1].data.push_back(pd);
                else
                    m_places[place-1].data[slot] = pd;
                ++m_places[place-1].used;
            }

            if(clan && clan != pl->getClan())
            {
                Stream st(REP::CLAN_BUILD);
                st << static_cast<UInt8>(1) << static_cast<UInt8>(2) << static_cast<UInt8>(m_places[place - 1].used);
                st << Stream::eos;
                clan->broadcast(st);
            }

        }
        return true;
    }

    PracticeData* PracticePlace::getPracticeData(UInt64 playerid)
    {
        return getPracticeData(globalPlayers[playerid]);
    }

    PracticeData* PracticePlace::getPracticeData(Player* pl)
    {
        if (!pl)
            return 0;
        auto it = m_pradata.find(pl->getId());
        if (it != m_pradata.end())
            return it->second;
        return 0;
    }

    bool PracticePlace::delPracticeData(Player* pl)
    {
        if (!pl)
            return false;
        if (!pl->isPracticing())
            return false;
        auto it = m_pradata.find(pl->getId());
        if (it != m_pradata.end()) {
            delete it->second;
            m_pradata.erase(it);
            return true;
        }
        return false;
    }

    bool PracticePlace::isSitdownYet(PracticeData* pd, UInt32 id)
    {
        if (!pd || !id)
            return false;

        for (auto i = pd->fighters.begin(); i != pd->fighters.end(); ++i)
        {
            if ((*i) == id)
                return true;
        }
        return false;
    }

    void PracticePlace::moveAllToMax(UInt8 place)
    {
        if (!place || place > PPLACE_MAX)
            return;

        //ScopedLocker<Mutex> lock1(m_lock[place-1]);
        //ScopedLocker<Mutex> lock2(m_lock[PPLACE_MAX-1]);

        PlaceData& pd = m_places[PPLACE_MAX-1];
        for (auto i = m_places[place-1].data.begin(), e = m_places[place-1].data.end(); i != e; ++i)
        {
            pd.data.push_back(*i);
            *i = 0;

            // TODO: notify client
        }
        return;
    }


    UInt64 PracticePlace::getPlaceOwnerId(UInt8 place)
    {
        PlaceData& pd = m_places[place-1];
        return pd.place.ownerid;
    }

    bool PracticePlace::replaceOwner(Player* oldpl, Player* newpl)
    {
        UInt8 idx = 0;
        if (!oldpl || !newpl)
            return false;

        for(; idx < PPLACE_MAX; idx ++)
        {
            if(oldpl->getId() == m_places[idx].place.ownerid)
                break;
        }

        if(idx == PPLACE_MAX)
            return false;

        PlaceData& pd = m_places[idx];

        GObject::Clan* clan = newpl->getClan();
        if(clan == NULL)
            return false;

        GObject::Clan* oldclan = oldpl->getClan();
        if(oldclan != clan)
        {
            pd.place.openslot = 0;
            UInt8 techslot = clan->getPracticeSlot();
            UInt8 slotadd = techslot - pd.place.techslot;

            pd.place.maxslot += slotadd;
            pd.place.techslot += techslot;
            pd.data.resize(pd.place.maxslot);
        }

        pd.place.ownerid = newpl->getId();

        DB1().PushUpdateData("UPDATE `practice_place` SET `ownerid` = %"I64_FMT"u, `maxslot` = %u WHERE id = %u", pd.place.ownerid, pd.place.maxslot, idx + 1);

        clan->broadcastPracticePlaceInfo();
        oldclan->broadcastPracticePlaceInfo();
        return true;
    }

    bool PracticePlace::ocupyPlace(Player* pl, UInt8 place)
    {
        UInt8 idx = 0;
        if (!pl)
            return false;

        for(; idx < PPLACE_MAX; idx ++)
        {
            if(pl->getId() == m_places[idx].place.ownerid)
                break;
        }

        if(place - 1 == idx)
            return false;

        GObject::Clan* clan = pl->getClan();
        if(clan == NULL)
            return false;

        if(clan->getOwner() != pl)
            return false;

        PlaceData& oldpd = m_places[idx];
        if(idx != PPLACE_MAX)
        {
            oldpd.place.maxslot -= oldpd.place.openslot + oldpd.place.techslot;
            oldpd.place.openslot = 0;
            oldpd.place.techslot = 0;
            oldpd.data.resize(oldpd.place.maxslot);

            oldpd.place.ownerid = 0;
            DB1().PushUpdateData("UPDATE `practice_place` SET `ownerid` = %"I64_FMT"u, `maxslot` = %u WHERE id = %u", oldpd.place.ownerid, oldpd.place.maxslot, idx + 1);
        }

        if(place >= PPLACE_MAX)
            return true;

        PlaceData& pd = m_places[place - 1];
        pd.place.openslot = 0;
        UInt8 techslot = clan->getPracticeSlot();
        UInt8 slotadd = techslot - pd.place.techslot;

        pd.place.maxslot += slotadd;
        pd.place.techslot += techslot;
        pd.data.resize(pd.place.maxslot);

        pd.place.ownerid = pl->getId();

        DB1().PushUpdateData("UPDATE `practice_place` SET `ownerid` = %"I64_FMT"u, `maxslot` = %u WHERE id = %u", pd.place.ownerid, pd.place.maxslot, place);

        clan->broadcastPracticePlaceInfo();

        return true;
    }

    PlaceData* PracticePlace::getPlaceData(Player* pl, UInt8& place)
    {
        if (!pl)
            return NULL;

        size_t idx = 0;
        place = PPLACE_MAX;
        for(; idx < PPLACE_MAX; idx ++)
        {
            if(pl->getId() == m_places[idx].place.ownerid)
            {
                place = idx + 1;
                break;
            }
        }

        if(idx == PPLACE_MAX)
            return NULL;

        return &(m_places[idx]);
    }

    PlaceData* PracticePlace::getPlaceData(UInt64 playerId, UInt8& place)
    {
        size_t idx = 0;
        place = PPLACE_MAX;
        for(; idx < PPLACE_MAX; idx ++)
        {
            if(playerId == m_places[idx].place.ownerid)
            {
                place = idx + 1;
                break;
            }
        }

        if(idx == PPLACE_MAX)
            return NULL;

        return &(m_places[idx]);
    }

    void PracticePlace::resetPracticePlaceIncoming()
    {
        size_t idx = 0;
        for(; idx < PPLACE_MAX; idx ++)
        {
            m_places[idx].place.slotincoming = 0;
            m_places[idx].place.protincoming = 0;
        }
    }


} // namespace GObject


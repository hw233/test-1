
#include "Config.h"
#include "Player.h"
#include "Log/Log.h"
#include "PracticePlace.h"
#include "Common/TimeUtil.h"
#include "GData/GDataManager.h"
#include "GObject/GObjectDBExecHelper.h"
#include "Server/WorldServer.h"
#include "Battle/BattleSimulator.h"
#include <sstream>
#include <mysql.h>

namespace GObject
{

    // XXX: place must greater than 0
    bool PracticePlace::pay(Player* pl, UInt8 place, UInt16 slot,
            UInt8 type, UInt8 priceType, UInt8 time, UInt8 prot)
    {
        if (!pl || !time || !place || place > PPLACE_MAX || slot > m_places[place-1].place.maxslot)
            return false;

        if (pl->isPracticing())
        {
            // TODO: msg
            return false;
        }

        if (place != PPLACE_MAX)
        {
            if (!m_places[place-1].data.size()) // XXX: no configuration
                return false;
        }

        PracticeData* p = getPracticeData(pl);
        if (p && p->cdend > TimeUtil::Now())
        {
            return false;
        }

        PracticeData*& pd = m_places[place-1].data[slot];
        if (pd)
        {
            if (pd->winnerid != pl->getId())
            {
                return false;
            }

            Player* def = globalPlayers[pd->getId()];
            if (!def)
            {
                return false;
            }

            UInt16 money =  ((float)pd->checktime / pd->traintime) * pd->price;
            if (pd->pricetype == 0)
                def->getGold(money);
            else
                def->getTael(money);

            pd->winnerid = 0;
            m_places[PPLACE_MAX-1].data.push_back(pd);
            pd = 0;
            --m_places[place-1].used;
            ++m_places[PPLACE_MAX-1].used;
        }
        ++m_places[place-1].used;

        UInt32 price = 0;
        ConsumeInfo ci(Practice,0,0);
        if (priceType == 0)
        {
            const std::vector<UInt32>& golds = GData::GDataManager::GetGoldPractice();
            if (!golds.size() || type >= golds.size())
                return false;
            price = time * golds[type]; 
            if (pl->getGold() < price)
            {
                pl->sendMsgCode(0, 2008);
                return false;
            }
            pl->useGold(price, &ci);
        }
        else
        {
            const std::vector<UInt32>& taels = GData::GDataManager::GetTaelPractice();
            if (!taels.size() || type >= taels.size())
                return false;
            price = time * taels[type];
            if (pl->getTael() < price)
            {
                pl->sendMsgCode(0, 2007);
                return false;
            }
            pl->useTael(price, &ci);
        }

        PracticeData* pp = new (std::nothrow) PracticeData(pl->getId());
        if (!pp)
            return false;
        pp->type = type;
        pp->pricetype = priceType;
        pp->price = price;
        pp->traintime = 60 * time;
        pp->checktime = pp->traintime;
        pp->trainend = TimeUtil::Now() + pp->traintime;
        pp->prot = prot;
        pp->cdend = TimeUtil::Now() + pp->traintime * 60 + 60 * 60;
        pp->winnerid = 0;
        m_places[place-1].data[slot] = pp;

        DB().PushUpdateData("REPLACE INTO `practice_data`(`id`, `place`, `slot`, `type`, `pricetype`, `price`, `traintime`, `checktime`, `prot`, `cdend`, `winnerid`, `fighters`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %"I64_FMT"u, '')", pl->getId(), place, slot, type, priceType, price, pp->traintime, pp->checktime, prot, pp->cdend, pp->winnerid);

        // TODO: 
#if 0
		Stream st(0x2E);
		UInt32 remain = event->GetEnd() - TimeUtil::Now();
		st << id << data->priceType << remain << Stream::eos;
		send(st);
#endif
        pl->setPracticingPlaceSlot(place << 16 | slot);
        addPractice(pl, pp); // XXX: must be here after setPracticingPlaceSlot
        return false;
    }

    bool PracticePlace::stop(Player* pl)
    {
        if (!pl)
            return false;

        if (!pl->isPracticing())
            return false;

        UInt8 place = pl->getPracticePlace();
        UInt16 slot = pl->getPracticeSlot();

        if (slot >= m_places[place-1].data.size())
            return false;
        PracticeData*& pd = m_places[place-1].data[slot];
        if (pd)
        {
            pd->winnerid = 0;
            pd->fighters.clear();

            --m_places[place-1].used;
            pd = 0;
        }
        if (place != PPLACE_MAX)
            pl->setPracticingPlaceSlot(PPLACE_MAX<<16);
        return true;
    }

    bool PracticePlace::sitdown(Player* pl, UInt32* fgtid, size_t size)
    {
        if (!pl || !fgtid || !size)
            return false;

        if (!pl->isPracticing())
            return false;

        PracticeData* data = getPracticeData(pl);
        if (!data)
            return false;

        if (data->fighters.size() > 5)
            return false;

        //data->lock.lock();
        for (size_t i = 0; i < size && i < 5; ++i)
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

        // TODO: notify client

        return true;
    }

    bool PracticePlace::standup(Player* pl, UInt32* fgtid, size_t size)
    {
        if (!pl || !fgtid || !size)
            return false;

        PracticeData* data = getPracticeData(pl);
        if (!data)
            return false;

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

        DB().PushUpdateData("UPDATE `practice_data` SET `fighters` = '%s' WHERE `id` = %"I64_FMT"u",
                fighters.str().c_str(), id);
    }

    void PracticePlace::getPlaceInfo(Player* pl, UInt8 place)
    {
    }

    void PracticePlace::getList(Player* pl, UInt8 place, UInt16 pageno, UInt16 pagenum)
    {
        if (!pl || !place || !pageno || !pagenum || place > PPLACE_MAX)
            return;

        PlaceData& pd = m_places[place-1];
        auto i = pd.data.begin();
        std::advance(i, pageno*pagenum);

        for (auto e = pd.data.end(); i != e; ++i)
        {
            // TODO: send *i to client
        }
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
        DB().PushUpdateData("UPDATE `practice_place` SET `protid` = %"I64_FMT"u WHERE `id` = %u", protid, place);

        // TODO: notify client
        return true;
    }

    bool PracticePlace::setCharges(Player* pl, UInt8 place, UInt16 money)
    {
        if (!pl || !place || !money)
            return false;

        PPlace& data = m_places[place-1].place;

        if (data.ownerid != pl->getId())
            return false;

        data.slotmoney = money;
        DB().PushUpdateData("UPDATE `practice_place` SET `slotmoney` = %u WHERE `id` = %u", money, place);

        // TODO: notify client

        return true;
    }

    bool PracticePlace::setProtCharges(Player* pl, UInt8 place, UInt16 money)
    {
        if (!pl || !place || !money)
            return false;

        PPlace& data = m_places[place-1].place;

        if (data.ownerid != pl->getId())
            return false;

        data.protmoney = money;
        DB().PushUpdateData("UPDATE `practice_place` SET `protmoney` = %u WHERE `id` = %u", money, place);

        // TODO: notify client

        return true;
    }

    bool PracticePlace::doChallenge(Player* pl, UInt8 place, UInt16 idx)
    {
        if (!pl || !place)
            return false;
        if (place == PPLACE_MAX) // XXX: 
            return false;
        
        if (idx > m_places[place-1].data.size())
            return false;

        Player* def = 0;
        PracticeData* pd = m_places[place-1].data[idx];
        if (pd)
        {
            if (!pd->prot)
            {
                def = globalPlayers[pd->getId()];
            }
            else if (pd->prot == 1)
            {
                if (m_places[place-1].place.protid)
                    def = globalPlayers[m_places[place-1].place.protid];
                else
                    def = globalPlayers[pd->getId()];
            }
            else if (pd->prot == 2)
            {
                // TODO: 
                return false;
            }
        }

        if (!def)
            return false;

        Battle::BattleSimulator bsim(pl->getLocation(), pl, def);
        pl->PutFighters(bsim, 0, true);
        def->PutFighters(bsim, 1, true);
        bsim.start();
        
        Stream& packet = bsim.getPacket();
        if(packet.size() <= 8)
            return false;

        // TODO: package

        if (bsim.getWinner() == 1)
        {
            pd->winnerid = pl->getId();
        }

        return true;
    }

    bool PracticePlace::addSlot(Player* pl, UInt8 place)
    {
        if (!pl || !place)
            return false;
        if (place > PPLACE_MAX)
            return false;
        PlaceData& pd = m_places[place-1];
        if (pd.place.ownerid != pl->getId())
            return false;

        const std::vector<UInt32>& golds = GData::GDataManager::GetGoldOpenSlot();
        if (!golds.size() || pd.place.openslot >= golds.size())
            return false;
        UInt32 price = golds[pd.place.openslot]; 
        if (pl->getGold() < price)
        {
            pl->sendMsgCode(0, 2008);
            return false;
        }
        ConsumeInfo ci(AddPracticeSlot,0,0);
        pl->useGold(price, &ci);

        if (pd.data.size() >= pd.place.maxslot ||
                pd.data.size() + 1 > pd.place.maxslot)
            return false;
        ++pd.place.maxslot;
        ++pd.place.openslot;
        pd.data.resize(pd.place.maxslot);

        // TODO: notify client

        return true;
    }

    bool PracticePlace::addPlace(PPlace& place, UInt8 idx)
    {
        if (idx > PPLACE_MAX)
            return false;
        PlaceData& pd = m_places[idx];
        pd.place = place;
        pd.data.resize(place.maxslot);
        return true;
    }

    bool PracticePlace::addPractice(Player* pl, PracticeData* pd)
    {
        if (!pl || !pd)
            return false;
        PracticeData*& oldpd = m_pradata[pd->getId()];
        if (oldpd) delete oldpd;
        oldpd = pd;

        if (pd->checktime)
        {
            EventPlayerPractice* event = new (std::nothrow) EventPlayerPractice(pl, 60*10, pd->checktime, pd->trainend);
            if (event == NULL) return false;
            PushTimerEvent(event);
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
        if (!pl->isPracticing())
            return 0;
        auto it = m_pradata.find(pl->getId());
        if (it != m_pradata.end())
            return it->second;
        return 0;
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

} // namespace GObject


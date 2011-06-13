
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

        if (place != PPLACE_MAX)
        {
            if (!m_places[place-1].data.size()) // XXX: no configuration
                return false;

            if (pl->isPracticing())
            {
                // TODO: msg
                return false;
            }
        }

        PracticeData* p = getPracticeData(pl);
        if (p && p->cdend > TimeUtil::Now())
        {
            return false;
        }

        if (m_places[place-1].data[slot] && m_places[place-1].data[slot]->winnerid != pl->getId())
        {
            return false;
        }

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

        data->lock.lock();
        for (size_t i = 0; i < size && i < 5; ++i)
        {
            if (!isSitdownYet(data, fgtid[i]))
            {
                if (pl->findFighter(fgtid[i]))
                    data->fighters.push_back(fgtid[i]);
            }
        }

        size_t cnt = 0;
        size_t nfgt = data->fighters.size();
        std::ostringstream fighters;
        for (auto i = data->fighters.begin(), e = data->fighters.end(); i != e; ++i)
        {
            fighters << *i;
            if (cnt++ < nfgt - 1)
                fighters << ",";
        }
        data->lock.unlock();

        DB().PushUpdateData("UPDATE `practice_data` SET `fighters` = '%s' WHERE `id` = %"I64_FMT"u",
                fighters.str().c_str(), pl->getId());

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

        data->lock.lock();
        for (size_t i = 0; i < size; ++i)
        {
            data->fighters.remove(fgtid[i]);
            // TODO: notify client
        }
        data->lock.unlock();

        return true;
    }

    void PracticePlace::getPlaceInfo(Player* pl, UInt8 place)
    {
    }

    void PracticePlace::getList(Player* pl, UInt8 place, UInt16 pageno)
    {
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
            {
                ScopedLocker<Mutex> lock1(m_lock[PPLACE_MAX-1]);
                ScopedLocker<Mutex> lock2(m_lock[place-1]);

                PracticeData*& pd = m_places[place-1].data[idx];
                UInt16 money =  ((float)pd->checktime / pd->traintime) * pd->price;
                if (pd->pricetype == 0)
                    def->getGold(money);
                else
                    def->useTael(money);

                m_places[PPLACE_MAX-1].data.push_back(pd);
                pd = 0;
            }
        }

        return true;
    }

    bool PracticePlace::addSlot(Player* pl, UInt8 place, UInt8 num)
    {
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
        ScopedLocker<Mutex> lock(m_lock[pl->getPracticePlace()-1]);
        PracticeData*& oldpd = m_pradata[pd->getId()];
        if (oldpd) delete oldpd;
        oldpd = pd;

        if (pd->checktime)
        {
            EventPlayerPractice* event = new (std::nothrow) EventPlayerPractice(pl, 60, pd->checktime, pd->trainend);
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
        ScopedLocker<Mutex> lock(m_lock[pl->getPracticePlace()-1]);
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

} // namespace GObject


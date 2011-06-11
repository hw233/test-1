
#include "Config.h"
#include "Player.h"
#include "Log/Log.h"
#include "PracticePlace.h"
#include "Common/TimeUtil.h"

namespace GObject
{

    bool PracticePlace::pay(Player* pl, UInt8 place, UInt16 slot, UInt8 type, UInt8 time, UInt8 prot)
    {
        if (!pl || !time || place > PPLACE_MAX)
            return false;

        if (pl->isPracticing())
        {
            // TODO: msg
            return false;
        }

        std::map<Player*, PlaceCD>::iterator i = m_places[place].players.find(pl);
        if (i != m_places[place].players.end())
        {
            if (i->second.cdend >= TimeUtil::Now())
            {
                // cding TODO: msg
                return false;
            }
        }

#if 0
        // pay
        ConsumeInfo ci(Practice,0,0);
        if (priceType == 0)
        {
            const std::vector<UInt32>& golds = GData::GDataManager::GetGoldTrainList();
            price = time * golds[fgt->getLevel()]; 
            if (getGold() < price)
            {
                sendMsgCode(0, 2008);
                return false;
            }
            useGold(price, &ci);
        }
        else
        {
            const std::vector<UInt32>& taels = GData::GDataManager::GetTaelTrainList();
            price = time * taels[fgt->getLevel()];
            if (getTael() < price)
            {
                sendMsgCode(0, 2007);
                return false;
            }
            useTael(price, &ci);
        }
#endif

        PracticeData pd;
        pd.type = type;
        pd.traintime = 60 * time;
        pd.checktime = pd.traintime;
        pd.trainend = TimeUtil::Now() + pd.traintime;
        pd.prot = prot;

        // TODO: timer
        
        pl->setPracticing(true);
        return false;
    }

    bool PracticePlace::addPlace(PPlace& place, UInt8 idx)
    {
        if (idx > PPLACE_MAX)
            return false;
        PlaceData& pd = m_places[idx];
        pd.place = place;
        pd.playerdata.resize(place.maxslot);
        return true;
    }
} // namespace GObject


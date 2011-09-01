
#include "WorldBoss.h"
#include "Map.h"
#include "Common/URandom.h"
#include "Server/SysMsg.h"
#include "Fighter.h"
#include "GData/NpcGroup.h"
#include "Server/Cfg.h"

namespace GObject
{
    static UInt32 worldboss[] = {5162, 5162, 5103, 5168, 5127, 5197, 5164};
    static UInt32 worldboss1[] = {5162, 5162, 5103, 5168, 5127, 5197, 5164};
    //static UInt32 worldboss1[] = {5000, 5001, 5000, 5001, 5000, 5001, 5001};

    bool WorldBoss::isWorldBoss(UInt32 npcid)
    {
        for (UInt8 i = 0; i < 6; ++i)
        {
            if (cfg.GMCheck)
            {
                if (worldboss[i] == npcid)
                    return true;
            } else
            {
                if (worldboss1[i] == npcid)
                    return true;
            }
        }
        return false;
    }

    void WorldBoss::refresh(UInt32 now)
    {
        time_t now2 = static_cast<time_t>(now);
        struct tm * t = localtime(&now2);

        if (t->tm_hour >= 20)
        {
            reset();
            return;
        }

        UInt8 level = 0;
        if (t->tm_hour >= 19 && m_max >= 100)
        {
            level = 7;
        }
        else if (t->tm_hour >= 18 && m_max >= 90)
        {
            level = 6;
        }
        else if (t->tm_hour >= 17 && m_max >= 80)
        {
            level = 5;
        }
        else if (t->tm_hour >= 16 && m_max >= 70)
        {
            level = 4;
        }
        else if (t->tm_hour >= 15 && m_max >= 60)
        {
            level = 3;
        }
        else if (t->tm_hour >= 14 && m_max >= 50)
        {
            level = 2;
        }
        else if (t->tm_hour >= 13 && m_max >= 40)
        {
            level = 1;
        }

        //if (!cfg.GMCheck)
        //    level = uRand(7)+1;

        if (!level)
            return;

        reset();

        std::vector<UInt16> spots;
        Map::GetAllSpot(spots);
        if (!spots.size())
            return;

        UInt16 spot = spots[uRand(spots.size())];
        if (!spot)
            return;

        UInt32 npcid = 0;
        if (cfg.GMCheck)
        {
            npcid = worldboss[level-1];
        }
        else
        {
            npcid = worldboss1[level-1];
        }
        Fighter* fgt = globalFighters[npcid];
        if (fgt)
        {
            GData::NpcGroup* ng = GData::npcGroups[npcid];
            if (ng)
            {
                add(spot, npcid, level, 0, true);
            }
        }
    }

    void WorldBoss::prepare(UInt32 now)
    {
        time_t now2 = static_cast<time_t>(now);
        struct tm * t = localtime(&now2);
        if ((t->tm_hour >= 12 && t->tm_min >= 45 && t->tm_hour < 13) ||
            (t->tm_hour >= 13 && t->tm_min >= 45 && t->tm_hour < 14) ||
            (t->tm_hour >= 14 && t->tm_min >= 45 && t->tm_hour < 15) ||
            (t->tm_hour >= 15 && t->tm_min >= 45 && t->tm_hour < 16) ||
            (t->tm_hour >= 16 && t->tm_min >= 45 && t->tm_hour < 17) ||
            (t->tm_hour >= 17 && t->tm_min >= 45 && t->tm_hour < 18) ||
            (t->tm_hour >= 18 && t->tm_min >= 45 && t->tm_hour < 19))
        {
            if (60 - t->tm_min - 1 >= 4)
                SYSMSG_BROADCASTV(547, 60 - t->tm_min);
            return;
        }
    }

    void WorldBoss::attack(Player* pl, UInt16 loc, UInt32 npcid)
    {
        std::map<UInt16, WBoss>::iterator i = m_boss.find(loc);
        if (i != m_boss.end())
        {
            if (i->second.npcId == npcid)
            {
                UInt16 count = 9;
                if (i->second.count >= count)
                {
                    if (pl->getVipLevel() >= 2)
                    {
                        if (!pl->getBuffData(PLAYER_BUFF_WBOSS))
                        {
                            pl->setBuffData(PLAYER_BUFF_WBOSS, 1, true);
                            pl->setBuffData(PLAYER_BUFF_WBOSSID, npcid, true);
                        }
                        else if (pl->getBuffData(PLAYER_BUFF_WBOSSID) != npcid)
                        {
                            pl->setBuffData(PLAYER_BUFF_WBOSSID, npcid, true);
                        }
                        else
                        {
                            SYSMSG_SEND(551, pl);
                            return;
                        }
                    }
                    else
                    {
                        SYSMSG_SEND(550, pl);
                        return;
                    }
                }

                if (pl->attackNpc(npcid))
                {
                    ++i->second.count;
                    DB().PushUpdateData("UPDATE `worldboss` SET `count` = %u WHERE `npcId` = %u", i->second.count, npcid);

                    Map* map = pl->GetMap();
                    if (map)
                    {
                        Fighter* fgt = globalFighters[npcid];
                        if (fgt)
                        {
                            SYSMSG_BROADCASTV(552, pl->getName().c_str(), map->GetName().c_str(),
                                    map->GetSpot(pl->getLocation())->m_Name.c_str(), fgt->getName().c_str());
                        }
                    }
                }
            }
        }
    }

    void WorldBoss::reset()
    {
        for (std::map<UInt16, WBoss>::iterator i = m_boss.begin(), e = m_boss.end(); i != e; ++i)
        {
            Map * map = Map::FromSpot(i->first);
            if (map)
            {
                map->Hide(i->second.npcId);
                map->DelObject(i->second.npcId);
            }
        }

        m_boss.clear();
        DB().PushUpdateData("DELETE FROM `worldboss`");
    }

    void WorldBoss::add(UInt16 loc, UInt32 npcId, UInt8 level, UInt8 count, bool show)
    {
        Map * map = Map::FromSpot(loc);
        if (map)
        {
            MOData mo;
            mo.m_ID = npcId;
            mo.m_Hide = false;
            mo.m_Spot = loc;
            mo.m_Type = 6;
            mo.m_ActionType = 0;
            map->AddObject(mo);

            if (show) 
            {
                map->Show(npcId);
                if (cfg.GMCheck)
                {
                    SYSMSG_BROADCAST(548);
                }
                else
                {
                    SpotData* sd = Map::Spot(loc);
                    if (sd)
                    {
                        SYSMSG_BROADCAST(548);
                        SYSMSG_BROADCASTV(549, map->GetName().c_str(), sd->m_Name.c_str());
                    }
                }
                DB().PushUpdateData("REPLACE INTO `worldboss` (`npcId`, `level`, `location`, `count`) VALUES (%u,%u,%u,%u)", npcId, level, loc, 0);
            }

            WBoss wb;
            wb.npcId = npcId;
            wb.level = level;
            wb.count = count;
            m_boss[loc] = wb;

        }
    }

    WorldBoss worldBoss;
}


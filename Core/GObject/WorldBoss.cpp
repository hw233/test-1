
#include "WorldBoss.h"
#include "Map.h"
#include "Common/URandom.h"
#include "Server/SysMsg.h"
#include "Fighter.h"
#include "GData/NpcGroup.h"
#include "Server/Cfg.h"

UInt8 _bossLvl = 0;

namespace GObject
{
    static UInt32 worldboss[] = {
        5466, 5467, 5468, 5469,
        5162, 5473, 5474, 5475,
        5103, 5470, 5471, 5472,
    };

    bool WorldBoss::isWorldBoss(UInt32 npcid)
    {
        for (UInt8 i = 0; i < sizeof(worldboss)/sizeof(UInt32); ++i)
        {
            if (cfg.GMCheck || true)
            {
                if (worldboss[i] == npcid)
                    return true;
            }
            else
            {
                if (worldboss[i] == npcid)
                    return true;
            }
        }
        return false;
    }

    UInt8 WorldBoss::getLevel(UInt32 now)
    {
        if (!cfg.GMCheck)
            return _bossLvl;

        time_t now2 = static_cast<time_t>(now);
        struct tm * t = localtime(&now2);
        switch (t->tm_hour)
        {
            case 20:
                {
                    if (m_level)
                        reset();
                }
                break;
            case 19:
                if (m_max >= 100)
                {
                    return 7;
                }
                break;
            case 18:
                if (m_max >= 90)
                {
                    return 6;
                }
                break;
            case 17:
                if (m_max >= 80)
                {
                    return 5;
                }
                break;
            case 16:
                if (m_max >= 70)
                {
                    return 4;
                }
                break;
            case 15:
                if (m_max >= 60)
                {
                    return 3;
                }
                break;
            case 14:
                if (m_max >= 50)
                {
                    return 2;
                }
                break;
            case 13:
                if (m_max >= 40)
                {
                    return 1;
                }
                break;
            default:
                break;
        }
        return 0;
    }

    void WorldBoss::refresh(UInt32 now)
    {
        UInt8 level = getLevel(now);
        if (!level)
        {
            if (m_boss.size())
                reset();
            return;
        }

        if (level == m_level)
            return;

        reset();

        UInt32 npcid = 0;
        UInt8 idx = (level-1)*4;
        if (cfg.GMCheck)
        {
            if (idx >= sizeof(worldboss)/sizeof(UInt32))
                return;
            npcid = worldboss[idx];
        }
        else
        {
            if (idx >= sizeof(worldboss)/sizeof(UInt32))
                return;
            npcid = worldboss[idx];
        }

        std::vector<UInt16> spots;
        Map::GetAllSpot(spots);
        if (!spots.size())
            return;

        UInt16 spot = spots[uRand(spots.size())];
        if (!spot)
            return;

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
        if ((t->tm_hour == 12 && t->tm_min >= 45 && m_max >= 40) ||
            (t->tm_hour == 13 && t->tm_min >= 45 && m_max >= 50) ||
            (t->tm_hour == 14 && t->tm_min >= 45 && m_max >= 60) ||
            (t->tm_hour == 15 && t->tm_min >= 45 && m_max >= 70) ||
            (t->tm_hour == 16 && t->tm_min >= 45 && m_max >= 80) ||
            (t->tm_hour == 17 && t->tm_min >= 45 && m_max >= 90) ||
            (t->tm_hour == 18 && t->tm_min >= 45 && m_max >= 100))
        {
            if (60 - t->tm_min >= 5)
                SYSMSG_BROADCASTV(547, ((60 - t->tm_min)/10)*10+5);
            return;
        }
    }

    void WorldBoss::attack(Player* pl, UInt16 loc, UInt32 npcid)
    {
        if (!m_level)
            return;

        bool in = false;
        for (int i = 0; i < 4; ++i)
        {
            if (worldboss[i+(m_level-1)*4] == npcid)
            {
                in = true;
                break;
            }
        }

        if (!in)
            return;

        FastMutex::ScopedLock lk(m_lck);
        std::map<UInt16, WBoss>::iterator i = m_boss.find(loc);
        if (i != m_boss.end())
        {
            //if (i->second.npcId == npcid)
            //{ // XXX:
                bool vip = false;
                UInt16 count = 10;
                if (i->second.count >= count)
                {
                    //if (pl->getVipLevel() >= 1)
                    //{
                        if (!pl->getBuffData(PLAYER_BUFF_WBOSS))
                        {
                            pl->setBuffData(PLAYER_BUFF_WBOSS, 1, true);
                            vip = true;
                        }
                        else
                        {
                            SYSMSG_SEND(551, pl);
                            return;
                        }
                    //}
                    //else
                    //{
                    //    SYSMSG_SEND(550, pl);
                    //    return;
                    //}
                }

                if (pl->attackCopyNpc(i->second.npcId, 2, 0, World::_wday==4?2:1))
                {
                    if (!vip)
                    {
                        ++i->second.count;
                        DB5().PushUpdateData("DELETE FROM `worldboss` WHERE location = %u", loc);

                        if (i->second.count < count)
                        {
                            UInt32 npcID = i->second.npcId;
                            UInt8 idx = (i->second.level-1)*4 + (i->second.count-1)/3 + 1;
                            if (cfg.GMCheck)
                            {
                                if (idx < sizeof(worldboss)/sizeof(UInt32))
                                    npcID = worldboss[idx];
                            }
                            else
                            {
                                if (idx < sizeof(worldboss)/sizeof(UInt32))
                                    npcID = worldboss[idx];
                            }
                            add(i->first, npcID, i->second.level, i->second.count, true, false);
                        }

                        Map* map = pl->GetMap();
                        if (map)
                        {
                            if (i->second.count >= count)
                            {
                                SYSMSG_BROADCASTV(553, i->second.npcId);
                            }
                            else
                            {
                                SYSMSG_BROADCASTV(552, pl->getCountry(), pl->getName().c_str(), loc, i->second.npcId);
                            }
                        }
                    }
                }
            //}
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

        m_level = 0;
        m_boss.clear();
        DB5().PushUpdateData("DELETE FROM `worldboss`");
    }

    void WorldBoss::add(UInt16 loc, UInt32 npcId, UInt8 level, UInt8 count, bool show, bool msg)
    {
        if (getLevel(time(NULL)) != level)
        {
            DB5().PushUpdateData("DELETE FROM `worldboss` WHERE location = %u", loc);
            return;
        }
        Map * map = Map::FromSpot(loc);
        if (map)
        {
            std::map<UInt16, WBoss>::iterator i = m_boss.find(loc);
            if (i != m_boss.end())
            {
                map->Hide(i->second.npcId);
                map->DelObject(i->second.npcId);
            }

            MOData mo;
            mo.m_ID = npcId;
            mo.m_Hide = false;
            mo.m_Spot = loc;
            mo.m_Type = 6;
            mo.m_ActionType = 0;
            map->AddObject(mo);

            if (show)
            {
                map->Show(npcId, true, mo.m_Type);
                Fighter* fgt = globalFighters[npcId];
                if (!fgt)
                    return;

                if (msg)
                {
                    if (cfg.GMCheck)
                    {
                        SYSMSG_BROADCASTV(554, fgt->getId(), loc, fgt->getId());
                    }
                    else
                    {
                        SYSMSG_BROADCASTV(548, fgt->getId());
                        SYSMSG_BROADCASTV(549, loc);
                    }
                }
                DB5().PushUpdateData("REPLACE INTO `worldboss` (`npcId`, `level`, `location`, `count`) VALUES (%u,%u,%u,%u)", npcId, level, loc, count);
            }

            WBoss wb;
            wb.npcId = npcId;
            wb.level = level;
            wb.count = count;
            m_boss[loc] = wb;
            m_level = level;
        }
    }

    void WorldBoss::setBossLevel(UInt8 lvl)
    {
        _bossLvl = lvl;
    }

    WorldBoss worldBoss;
}


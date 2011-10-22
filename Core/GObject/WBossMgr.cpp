
#include "WBossMgr.h"
#include "Server/Cfg.h"
#include "Common/TimeUtil.h"
#include "Server/SysMsg.h"
#include "Fighter.h"
#include "Map.h"
#include "GData/NpcGroup.h"

namespace GObject
{

const UInt8 WBOSS_NUM = 7;

static UInt32 worldboss[] = {
    5466, 5467, 5468, 5469, 5469,
    5162, 5473, 5474, 5475, 5475,
    5103, 5470, 5471, 5472, 5472,
};

bool WBoss::attack(Player* pl, UInt32 id)
{
    bool in = false;
    for (int i = 0; i < 5; ++i)
    {
        if (worldboss[i+(m_lvl-1)*5] == id)
        {
            in = true;
            break;
        }
    }
    if (!in) return false;

    bool res = pl->attackCopyNpc(id, 2, 0, World::_wday==4?2:1);
    if (res)
        ++m_count;
    if (m_count >= m_maxcnt)
        m_final = true;
    return res;
}

void WBoss::appear(UInt32 npcid, UInt32 oldid)
{
    if (!npcid) return;

    Fighter* fgt = globalFighters[npcid];
    if (!fgt) return;

    GData::NpcGroup* ng = GData::npcGroups[npcid];
    if (!ng) return;

    Map * map = Map::FromSpot(m_loc);
    if (!map) return;

    if (oldid)
    {
        map->Hide(oldid);
        map->DelObject(oldid);
    }

    MOData mo;
    mo.m_ID = npcid;
    mo.m_Hide = false;
    mo.m_Spot = m_loc;
    mo.m_Type = 6;
    mo.m_ActionType = 0;
    map->AddObject(mo);
    map->Show(npcid, true, mo.m_Type);
}

void WBoss::disapper(UInt32 npcid)
{
    if (!npcid) return;
    Map * map = Map::FromSpot(m_loc);
    if (!map) return;
    map->Hide(npcid);
    map->DelObject(npcid);
}

bool WBossMgr::isWorldBoss(UInt32 npcid)
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

void WBossMgr::calcNext(UInt32 now)
{
    UInt32 appears[] = {
        TimeUtil::SharpDayT(0,now) + 20 * 60 * 60,
        TimeUtil::SharpDayT(0,now) + 18 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 17 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 16 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 15 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 14 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 13 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 12 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now),
    };

    for (UInt8 i = 0; i < WBOSS_NUM+2; ++i)
    {
        if (now >= appears[i] && i == 0)
        {
            m_level = 0;
            _prepareTime = 0;
            _appearTime = 0;
            _disapperTime = 0;
            return;
        }

        if (now >= appears[i])
        {
            _prepareTime = appears[i];
            _appearTime = _prepareTime + 15 * 60;
            _disapperTime = _appearTime + 60 * 60;
            m_level = WBOSS_NUM - i;
            break;
        }

        if (i == WBOSS_NUM+1)
        {
            _prepareTime = appears[i-1];
            _appearTime = _prepareTime + 15 * 60;
            _disapperTime = _appearTime + 60 * 60;
        }
    }
}

void WBossMgr::process(UInt32 now)
{
    if (!_prepareTime)
        calcNext(now);

    broadcastTV(now);

    if (_appearTime && now >= _appearTime)
        appear(m_level, now);

    if (_disapperTime && now >= _disapperTime)
        disapper(m_level, now);
}

void WBossMgr::broadcastTV(UInt32 now)
{
    if (now >= _prepareTime)
        _prepareStep = 1; 

    switch (_prepareStep)
    {    
        case 1:
            SYSMSG_BROADCASTV(547, 15); 
            _prepareStep = 2; 
            break;

        case 2:
            if (now < _appearTime - 10 * 60)
                return;
            SYSMSG_BROADCASTV(547, 10); 
            _prepareStep = 3; 
            break;

        case 3:
            if (now < _appearTime - 5 * 60)
                return;
            SYSMSG_BROADCASTV(547, 5);
            _prepareStep = 4; 
            break;

        case 4:
            if (now < _appearTime)
                return;
            _prepareStep = 5;
            break;

        default:
            break;
    }
}

void WBossMgr::appear(UInt8 level, UInt32 now)
{
    if (!level)
        return;

    UInt32 npcid = 0;
    UInt8 idx = (level-1)*5;
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

#if 0
    Fighter* fgt = globalFighters[npcid];
    if (!fgt)
        return;

    GData::NpcGroup* ng = GData::npcGroups[npcid];
    if (!ng)
        return;

    Map * map = Map::FromSpot(loc);
    if (!map)
        return;

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

    map->Show(npcId, true, mo.m_Type);

    WBoss wb;
    wb.npcId = npcId;
    wb.level = level;
    wb.count = count;
    m_boss[loc] = wb;
    m_level = level;
#endif
}

void WBossMgr::disapper(UInt8 level, UInt32 now)
{
    calcNext(now);
}

} // namespace GObject


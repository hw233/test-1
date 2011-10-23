
#include "WBossMgr.h"
#include "Server/Cfg.h"
#include "Common/TimeUtil.h"
#include "Server/SysMsg.h"
#include "Fighter.h"
#include "Map.h"
#include "GData/NpcGroup.h"
#include "MsgID.h"

namespace GObject
{

const UInt8 WBOSS_NUM = 7;
const UInt8 WBOSS_ATTKMAX = 10;

static UInt8 getAttackMax()
{
    return WBOSS_ATTKMAX;
}

static UInt32 worldboss[] = {
    5466, 5467, 5468, 5469, 5469,
    5162, 5473, 5474, 5475, 5475,
    5103, 5470, 5471, 5472, 5472,
};

bool WBoss::attackWorldBoss(Player* pl, UInt32 npcId, UInt8 expfactor, bool final)
{
    if (!pl) return false;
    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now) 
    {    
        pl->sendMsgCode(0, 1407, buffLeft - now);
        return false;
    }    
    pl->checkLastBattled();
    GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
    if(it == GData::npcGroups.end())
        return false;

    GData::NpcGroup * ng = it->second;
    std::vector<GData::NpcFData>& nflist = ng->getList();
    size_t sz = nflist.size();

    Battle::BattleSimulator bsim(0, pl, ng->getName(), ng->getLevel(), false);
    pl->PutFighters( bsim, 0 ); 
    ng->putFighters( bsim );
    bsim.start();
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;

    UInt16 ret = 0x0100;
    bool res = bsim.getWinner() == 1;
    if(res)
    {    
        ret = 0x0101;
        if (final)
        {
        }
        else
        {
            pl->_lastNg = ng;
            pl->pendExp(ng->getExp()*expfactor);
            ng->getLoots(pl, pl->_lastLoot, 1, NULL);
        }
    }
    else
    {
    }

    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(pl, lastExp) << static_cast<UInt8>(0);
    UInt8 size = pl->_lastLoot.size();
    for (UInt8 i = 0; i < size; ++i)
    {
        st << pl->_lastLoot[i].id << pl->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << Stream::eos;
    pl->send(st);

    if (final)
        pl->setBuffData(PLAYER_BUFF_ATTACKING, now + 30);
    else
        pl->setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns());
    return true;
}

bool WBoss::attack(Player* pl, UInt16 loc, UInt32 id)
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

    if (pl->getLocation() != loc)
        return false;

    if (loc != m_loc)
        return false;

    if (!m_id)
        return false;

    bool res = attackWorldBoss(pl, m_id, World::_wday==4?2:1, m_final);
    if (res && !m_final)
    {
        if (m_count >= m_maxcnt)
            m_final = true;

        ++m_count;

        if (m_count <= m_maxcnt)
        {
            SYSMSG_BROADCASTV(552, pl->getCountry(), pl->getName().c_str(), loc, m_id);
            UInt8 idx = (m_lvl-1)*5 + (m_count-1)/3 + 1;
            UInt32 id = 0;
            if (idx < sizeof(worldboss)/sizeof(UInt32))
                id = worldboss[idx];
            appear(id, m_id);
        }
    }
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

    if (oldid && oldid != npcid)
    {
        map->Hide(oldid);
        map->DelObject(oldid);
    }

    if (oldid != npcid)
    {
        MOData mo;
        mo.m_ID = npcid;
        mo.m_Hide = false;
        mo.m_Spot = m_loc;
        mo.m_Type = 6;
        mo.m_ActionType = 0;
        map->AddObject(mo);
        map->Show(npcid, true, mo.m_Type);
        m_id = npcid;
    }

    if (m_count == 10)
        { SYSMSG_BROADCASTV(553, npcid); }
    if (!m_count)
        { SYSMSG_BROADCASTV(554, fgt->getId(), m_loc, fgt->getId()); }
}

void WBoss::disapper()
{
    Map * map = Map::FromSpot(m_loc);
    if (!map) return;
    map->Hide(m_id);
    map->DelObject(m_id);
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
#if 0
        TimeUtil::SharpDayT(0,now) + 20 * 60 * 60,
        TimeUtil::SharpDayT(0,now) + 18 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 17 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 16 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 15 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 14 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 13 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now) + 12 * 60 * 60 + 45 * 60,
        TimeUtil::SharpDayT(0,now),
#else
        TimeUtil::SharpDayT(0,now) + 20*60*60+60 * 60,
        TimeUtil::SharpDayT(0,now) + 20*60*60+50 * 60,
        TimeUtil::SharpDayT(0,now) + 20*60*60+40 * 60,
        TimeUtil::SharpDayT(0,now) + 20*60*60+30 * 60,
        TimeUtil::SharpDayT(0,now) + 20*60*60+20 * 60,
        TimeUtil::SharpDayT(0,now) + 20*60*60+10 * 60,
        TimeUtil::SharpDayT(0,now) + 20*60*60+5 * 60,
        TimeUtil::SharpDayT(0,now) + 20*60*60+10,
        TimeUtil::SharpDayT(0,now),
#endif
    };

    for (UInt8 i = 0; i < WBOSS_NUM+2; ++i)
    {
        if (now >= appears[i] && i == 0)
        {
            m_level = 0;
            _prepareTime = TimeUtil::SharpDayT(1,now) + 12 * 60 * 60 + 45 * 60;
            _appearTime = _prepareTime + 15 * 60;;
            _disapperTime = _appearTime + 60 * 60;
            m_boss->disapper();
            return;
        }

        if (i <= WBOSS_NUM && now >= appears[i] && m_maxlvl >= (30+i*10))
        {
            _prepareTime = appears[i];
            if (cfg.GMCheck)
            {
                _appearTime = _prepareTime + 15 * 60;
                _disapperTime = _appearTime + 60 * 60;
            }
            else
            {
                _appearTime = _prepareTime + 20;
                _disapperTime = _appearTime + 5 * 60;
            }
            m_level = WBOSS_NUM - i + 1;
            break;
        }

        if (i == WBOSS_NUM+1 && m_maxlvl >= 40)
        {
            _prepareTime = appears[i-1];
            if (cfg.GMCheck)
            {
                _appearTime = _prepareTime + 15 * 60;
                _disapperTime = _appearTime + 60 * 60;
            }
            else
            {
                _appearTime = _prepareTime + 30;
                _disapperTime = _appearTime + 5 * 60;
            }
            m_level = 1;
        }
    }
}

void WBossMgr::process(UInt32 now)
{
    if (!_prepareTime)
        calcNext(now);
    broadcastTV(now);
}

void WBossMgr::broadcastTV(UInt32 now)
{
    if (now >= _prepareTime && !_prepareStep)
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
            appear(m_level, now);
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
    if (idx >= sizeof(worldboss)/sizeof(UInt32))
        return;
    npcid = worldboss[idx];

    std::vector<UInt16> spots;
    Map::GetAllSpot(spots);
    if (!spots.size()) return;

    UInt16 spot = spots[uRand(spots.size())];
    if (!spot) return;

    if (!m_boss)
    {
        m_boss = new WBoss(npcid, 0, getAttackMax(), spot, level);
        m_boss->appear(npcid, 0);
    }
    else
        m_boss->appear(npcid, m_boss->getId());
    calcNext(now);
}

void WBossMgr::attack(Player* pl, UInt16 loc, UInt32 npcid)
{
    if (!pl || !loc || !npcid)
        return;
    if (!m_boss)
        return;
    bool res = m_boss->attack(pl, loc, npcid);
    if (res && m_boss->isFinal())
    {
        disapper();
    }
    else
    {
    }
}

void WBossMgr::disapper()
{
    if (m_boss)
    {
        m_boss->disapper();
        delete m_boss;
        m_boss = NULL;
    }
    calcNext(TimeUtil::Now());
    _prepareStep = 0;
}

WBossMgr worldBoss;

} // namespace GObject


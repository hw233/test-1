
#include "WBossMgr.h"
#include "Server/Cfg.h"
#include "Common/TimeUtil.h"
#include "Server/SysMsg.h"
#include "Fighter.h"
#include "Map.h"
#include "MsgID.h"
#include "Package.h"

namespace GObject
{

const UInt8 WBOSS_NUM = 7;
const UInt8 WBOSS_ATTKMAX = 10;

static UInt8 getAttackMax()
{
    return WBOSS_ATTKMAX;
}

static UInt32 worldboss[] = {
    5466, 5467, 5468, 5469, 5509,
    5162, 5473, 5474, 5475, 5510,
    5103, 5470, 5471, 5472, 5511,
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

    Battle::BattleSimulator bsim(0, pl, _ng->getName(), _ng->getLevel(), false);
    pl->PutFighters( bsim, 0 ); 
    _ng->putFighters( bsim );

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    size_t sz = nflist.size();

    if (final)
    {
        for(size_t i = 0; i < sz; ++ i)
        {
            if(_hp[i] == 0xFFFFFFFF)
                continue;
            GData::NpcFData& nfdata = nflist[i];
            Battle::BattleFighter * bf = bsim.newFighter(1, nfdata.pos, nfdata.fighter);
            bf->setHP(_hp[i]);
        }
    }

    bsim.start();
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;

    UInt16 ret = 0x0100;
    bool res = bsim.getWinner() == 1;
    if(res)
    {    
        ret = 0x0101;
        if (!final)
        {
            pl->_lastNg = _ng;
            pl->pendExp(_ng->getExp()*expfactor);
            _ng->getLoots(pl, pl->_lastLoot, 1, NULL);
        }
    }

    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(pl, lastExp) << static_cast<UInt8>(0);
    UInt8 size = pl->_lastLoot.size();
    st << size;
    for (UInt8 i = 0; i < size; ++i)
    {
        st << pl->_lastLoot[i].id << pl->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << Stream::eos;
    pl->send(st);
    bsim.applyFighterHP(0, pl);

    if (sz && final)
    {
        UInt32 oldHP = _hp[0];
        for(size_t i = 0; i < sz; ++ i)
        {
            GData::NpcFData& nfdata = nflist[i];
            Battle::BattleObject * obj = bsim(1, nfdata.pos);
            if(obj == NULL || !obj->isChar())
                continue;
            Battle::BattleFighter * bfgt = static_cast<Battle::BattleFighter *>(obj);
            UInt32 nHP = bfgt->getHP();
            if(nHP == 0)
                nHP = 0xFFFFFFFF;
            if(_hp[i] != 0xFFFFFFFF && _hp[i] != nHP)
                _hp[i] = nHP;
        }
        pl->setBuffData(PLAYER_BUFF_ATTACKING, now + 30);

        if(oldHP == 0xFFFFFFFF)
            return res;

        if(oldHP == 0)
            oldHP = nflist[0].fighter->getMaxHP();
        UInt32 newHP = (_hp[0] == 0xFFFFFFFF) ? 0 : _hp[0];
        if(oldHP > newHP)
        {
            UInt32 damage = oldHP - newHP;
            UInt32 exp = (float(damage) / nflist[0].fighter->getMaxHP()) * _ng->getExp() * expfactor;
            pl->pendExp(exp);

            AttackInfo info(pl, damage);
            AtkInfoType::iterator i = m_atkinfo.begin(), e = m_atkinfo.end();
            for ( ; i != e; ++i)
            {
                if ((*i).player == pl)
                {
                    info += *i;
                    m_atkinfo.erase(i);
                    break;
                }
            }
            m_atkinfo.insert(info);

            UInt8 newPercent = float(newHP * 100) / nflist[0].fighter->getMaxHP();
            if (newPercent > 100)
                newPercent = 100;
            if (_percent < newPercent)
                _percent = newPercent;
            if (!newPercent)
            {
                SYSMSG_BROADCASTV(550, nflist[0].fighter->getId());
                _percent = 0;
                reward(pl);
            }
            else if (newPercent <= 5 && _percent - newPercent >= 5)
            {
                SYSMSG_BROADCASTV(548, pl->getCountry(), pl->getName().c_str(), nflist[0].fighter->getId(), newPercent);
                _percent = newPercent;
            }
            else if (_percent - newPercent >= 10)
            {
                SYSMSG_BROADCASTV(548, pl->getCountry(), pl->getName().c_str(), nflist[0].fighter->getId(), newPercent);
                _percent = newPercent;
            }
        }
    }
    else
        pl->setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns());

    return res;
}

void WBoss::reward(Player* player)
{
    static UInt16 trumps[] = {90, 222, 221, 223, };

    size_t sz = m_atkinfo.size();
    UInt32 idx = uRand(sz);
    UInt32 j = 0;
    for (AtkInfoType::reverse_iterator i = m_atkinfo.rbegin(), e = m_atkinfo.rend() ; i != e; ++i)
    {
        if (j == 0)
        {
            UInt16 equip = GObject::getRandOEquip(_ng->getLevel());
            (*i).player->GetPackage()->Add(equip, true);
            (*i).player->GetPackage()->Add(514, 3, true);

            UInt8 idx = _ng->getLevel();
            if (idx < 40)
                idx = 40;
            if (idx > 100)
                idx = 100;
            idx -= 40;
            idx /= 10;

            if (idx > sizeof(trumps)/sizeof(UInt16))
                continue;
            (*i).player->GetPackage()->Add(trumps[idx], 1, true);
            SYSMSG_BROADCASTV(557, j+1, (*i).player->getCountry(), (*i).player->getName().c_str(), equip, 514, 3, trumps[idx], 1);
        }
        if (j == 1)
        {
            UInt16 equip = GObject::getRandOEquip(_ng->getLevel());
            (*i).player->GetPackage()->Add(equip, true);
            (*i).player->GetPackage()->Add(514, 2, true);
            SYSMSG_BROADCASTV(558, j+1, (*i).player->getCountry(), (*i).player->getName().c_str(), equip, 514, 2);
        }
        if (j == 2)
        {
            UInt16 equip = GObject::getRandOEquip(_ng->getLevel());
            (*i).player->GetPackage()->Add(equip, true);
            (*i).player->GetPackage()->Add(514, 1, true);
            SYSMSG_BROADCASTV(558, j+1, (*i).player->getCountry(), (*i).player->getName().c_str(), equip, 514, 1);

            if (idx <= j)
                break;
        }

        if (j == idx)
        {
            UInt16 equip = GObject::getRandOEquip(_ng->getLevel());
            (*i).player->GetPackage()->Add(equip, true);
            (*i).player->GetPackage()->Add(514, 1, true);
            SYSMSG_BROADCASTV(558, j+1, (*i).player->getCountry(), (*i).player->getName().c_str(), equip, 514, 1);

            if (idx >= 3)
                break;
        }

        ++j;
    }

    if (player)
    {
        player->GetPackage()->Add(56, 5, true);
        player->getTael(10000);
        SYSMSG_BROADCASTV(559, player->getCountry(), player->getName().c_str(), 56, 5);
    }
}

bool WBoss::attack(Player* pl, UInt16 loc, UInt32 id)
{
    bool in = false;
    for (int i = 0; i < 5; ++i)
    {
        if (worldboss[i+(m_lvl-1)*5] == id)
        {
            in = true;
            if (i == 4)
                m_final = true;
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

    if (!_ng)
        return false;

    if (m_disappered)
    {
        SYSMSG_BROADCASTV(551);
        return false;
    }

    if (!_percent)
        return false;

    bool res = attackWorldBoss(pl, m_id, World::_wday==4?2:1, m_final);
    if (res && !m_final)
    {
        ++m_count;
        if (m_count <= m_maxcnt)
        {
            SYSMSG_BROADCASTV(552, pl->getCountry(), pl->getName().c_str(), loc, m_count, m_id);
            UInt8 idx = (m_lvl-1)*5 + (m_count-1)/3 + 1;
            UInt32 id = 0;
            if (idx < sizeof(worldboss)/sizeof(UInt32))
                id = worldboss[idx];
            appear(id, m_id);
        }
    }
    else if (res && m_final)
    {
        disapper();
    }
    return res;
}

void WBoss::appear(UInt32 npcid, UInt32 oldid)
{
    if (!npcid) return;

    GData::NpcGroups::iterator it = GData::npcGroups.find(npcid);
    if(it == GData::npcGroups.end())
        return;

    _ng = it->second;
    if (!_ng) return;

    _hp.clear();
    std::vector<GData::NpcFData>& nflist = _ng->getList();
    size_t sz = nflist.size();
    if (!sz) return;
    _hp.resize(sz);
    for(size_t i = 0; i < sz; ++ i)
    {
        if(_hp[i] == 0 || _hp[i] >= 0xFFFFFFF0)
            _hp[i] = nflist[i].fighter->getMaxHP();
    }

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

    fprintf(stderr, "appear: %u, lvl: %u, loc: %u\n", m_id, m_lvl, m_loc);

    m_disappered = false;
    if (m_count == 10)
        { SYSMSG_BROADCASTV(553, npcid); }
    if (!m_count)
        { SYSMSG_BROADCASTV(554, nflist[0].fighter->getId(), m_loc, nflist[0].fighter->getId()); }
}

void WBoss::disapper()
{
    Map * map = Map::FromSpot(m_loc);
    if (!map) return;
    map->Hide(m_id);
    map->DelObject(m_id);

    m_count = 0;
    m_disappered = true;
    m_final = false;
    _percent = 100;
    _ng = NULL;
    _hp.clear();
    m_atkinfo.clear();
    fprintf(stderr, "disapper: %u, lvl: %u, loc: %u\n", m_id, m_lvl, m_loc);
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

void WBossMgr::nextDay(UInt32 now)
{
    m_level = 0;
    _prepareTime = TimeUtil::SharpDayT(1,now) + 12 * 60 * 60 + 45 * 60;
    _appearTime = _prepareTime + 15 * 60;;
    _disapperTime = _appearTime + 60 * 60 - 10;
    if (m_boss)
    {
        m_boss->disapper();
        delete m_boss;
        m_boss = NULL;
    }
    fprintf(stderr, "out of time. next day\n");
    return;
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
        TimeUtil::SharpDayT(0,now) + 15*60*60+24*60+21*60,
        TimeUtil::SharpDayT(0,now) + 15*60*60+24*60+18*60,
        TimeUtil::SharpDayT(0,now) + 15*60*60+24*60+15*60,
        TimeUtil::SharpDayT(0,now) + 15*60*60+24*60+12*60,
        TimeUtil::SharpDayT(0,now) + 15*60*60+24*60+9*60,
        TimeUtil::SharpDayT(0,now) + 15*60*60+24*60+6*60,
        TimeUtil::SharpDayT(0,now) + 15*60*60+24*60+3*60,
        TimeUtil::SharpDayT(0,now) + 15*60*60+24*60+10,
        TimeUtil::SharpDayT(0,now),
#endif
    };

    for (UInt8 i = 0; i < WBOSS_NUM+2; ++i)
    {
        if (now >= appears[i] && i == 0)
        {
            nextDay(now);
            return;
        }

        if (i <= WBOSS_NUM && now >= appears[i] && m_maxlvl >= (40+(WBOSS_NUM-i)*10))
        {
            if (m_boss && m_boss->isDisappered())
            {
                if (i == 1)
                {
                    nextDay(now);
                    return;
                }
                else
                {
                    m_level = WBOSS_NUM - i + 1;
                    if (m_level != m_boss->getLevel())
                    {
                        _prepareTime = appears[i];
                        m_level = WBOSS_NUM - i + 1;
                    }
                    else
                    {
                        _prepareTime = appears[i-1];
                        m_level = WBOSS_NUM - i + 2;
                    }
                }
            }
            else
            {
                _prepareTime = appears[i];
                m_level = WBOSS_NUM - i + 1;
            }

            if (cfg.GMCheck)
            {
                _appearTime = _prepareTime + 15 * 60;
                _disapperTime = _appearTime + 60 * 60 - 60;
            }
            else
            {
                _appearTime = _prepareTime + 20;
                _disapperTime = _appearTime + 2 * 60 - 10;
            }
            break;
        }

        if (i == WBOSS_NUM+1 && m_maxlvl >= 40)
        {
            _prepareTime = appears[i-1];
            if (cfg.GMCheck)
            {
                _appearTime = _prepareTime + 15 * 60;
                _disapperTime = _appearTime + 60 * 60 - 60;
            }
            else
            {
                _appearTime = _prepareTime + 30;
                _disapperTime = _appearTime + 2 * 60 - 10;
            }
            m_level = 1;
        }
    }

    if (!cfg.GMCheck)
        fprintf(stderr, "lvl: %u\n", m_level);
}

void WBossMgr::process(UInt32 now)
{
    if (!_prepareTime)
        calcNext(now);
    broadcastTV(now);
    if (now >= _disapperTime && m_boss && !m_boss->isDisappered())
        disapper(now);
}

void WBossMgr::broadcastTV(UInt32 now)
{
    if (now >= _prepareTime && !_prepareStep)
        _prepareStep = 1; 

    switch (_prepareStep)
    {    
        case 1:
            SYSMSG_BROADCASTV(547, 15); 
            fprintf(stderr, "15 mins\n");
            _prepareStep = 2; 
            break;

        case 2:
            if (now < _appearTime - 10 * 60)
                return;
            fprintf(stderr, "10 mins\n");
            SYSMSG_BROADCASTV(547, 10); 
            _prepareStep = 3; 
            break;

        case 3:
            if (now < _appearTime - 5 * 60)
                return;
            fprintf(stderr, "5 mins\n");
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
    {
        nextDay(now);
        return;
    }
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
    {
        if (level != m_boss->getLevel())
        {
            m_boss->setLevel(level);
            m_boss->setLoc(spot);
        }
        m_boss->appear(npcid, m_boss->getId());
    }
}

void WBossMgr::attack(Player* pl, UInt16 loc, UInt32 npcid)
{
    if (!pl || !loc || !npcid)
        return;
    if (!m_boss)
        return;
    m_boss->attack(pl, loc, npcid);
}

void WBossMgr::disapper(UInt32 now)
{
    if (m_boss && !m_boss->isDisappered())
        m_boss->disapper();
    _prepareStep = 0;
    calcNext(now);
}

WBossMgr worldBoss;

} // namespace GObject


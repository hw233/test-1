
#include "WBossMgr.h"
#include "Server/Cfg.h"
#include "Common/TimeUtil.h"
#include "Server/SysMsg.h"
#include "Fighter.h"
#include "Map.h"
#include "MsgID.h"
#include "Package.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Country.h"
#include "Script/GameActionLua.h"
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
    5168, 5476, 5477, 5478, 5512,
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

    Battle::BattleSimulator bsim(Battle::BS_WBOSS, pl, _ng->getName(), _ng->getLevel(), false);
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

        if(oldHP != 0xFFFFFFFF)
        {
            if(oldHP == 0)
                oldHP = nflist[0].fighter->getMaxHP();
            UInt32 newHP = (_hp[0] == 0xFFFFFFFF) ? 0 : _hp[0];
            if(oldHP > newHP)
            {
                UInt32 damage = oldHP - newHP;
                UInt32 exp = (float(damage) / nflist[0].fighter->getMaxHP()) * _ng->getExp() * expfactor;
                pl->pendExp(exp);
                sendDmg(damage);

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

                UInt8 newPercent = (float(newHP) / nflist[0].fighter->getMaxHP()) * 100;
                
                if (newPercent > 100)
                    newPercent = 100;
                if (_percent < newPercent)
                    _percent = newPercent;
                if (!newPercent)
                {
                    SYSMSG_BROADCASTV(550, nflist[0].fighter->getId());
                    _percent = 0;
                    _hp[0] = 0;
                    reward(pl);
                    res = true;
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
                sendHp();
            }
        }
    }

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

    pl->setBuffData(PLAYER_BUFF_ATTACKING, now + 30);
    return res;
}

void WBoss::getRandList(UInt32 sz, UInt32 num, std::set<UInt32>& ret)
{
    if (sz >= 3)
    {
        if (sz <= 10)
        {
            for (UInt8 i = 0; i < 7; ++i)
                ret.insert(i+3);
        }
        else
        {
            UInt32 j = uRand(sz-3);
            for (UInt32 i = 0; i < num; ++i)
            {
                while (ret.find(j+3) != ret.end())
                    j = uRand(sz-3);
                ret.insert(j+3);
            }
        }
    }
}

void WBoss::flee()
{
    SYSMSG_BROADCASTV(570, m_id);
}

void WBoss::reward(Player* player)
{
    static UInt16 trumps[] = {226,90,225,227,};
    static UInt8 trumpnum[] = {3,2,1};
    static UInt16 gems[] = {5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142};

    size_t sz = m_atkinfo.size();
    if (!sz) return;

    UInt8 lvl = _ng->getLevel();
    UInt8 tlvl = lvl;

    if (tlvl < 40)
        tlvl = 40;
    if (tlvl > 100)
        tlvl = 100;

    tlvl -= 40;
    tlvl /= 10;

    UInt32 lucky1 = uRand(sz);
    if (lucky1 < 10)
        lucky1 = 10;
    UInt32 lucky2 = uRand(sz);
    if (lucky2 < 10)
        lucky2 = 10;

    if (lucky1 == lucky2)
        lucky2 += uRand(10);

    if (lucky2 >= sz)
        lucky2 = sz-1;

    if (lucky1 >= sz)
        lucky1 = 0;
    if (lucky2 >= sz)
        lucky2 = 0;

    std::set<UInt32> equip_lucky;
    std::set<UInt32> breath;
    std::set<UInt32> gem;

    UInt32 luckynum = 0;
    if (lvl == 40)
        luckynum = (float)10 * sz / 100;
    else
        luckynum = (float)5 * sz / 100;
    getRandList(sz, luckynum, equip_lucky);

    luckynum = (float)5 * sz / 100;
    getRandList(sz, luckynum, breath);
    getRandList(sz, luckynum, gem);

    UInt32 j = 0;
    for (AtkInfoType::reverse_iterator i = m_atkinfo.rbegin(), e = m_atkinfo.rend(); i != e; ++i, ++j)
    {
        if (j < 3)
        {
            UInt16 equip = 0;
            if (lvl == 40)
                equip = GObject::getRandPEquip(lvl);
            else
                equip = GObject::getRandOEquip(lvl);

            if (tlvl > sizeof(trumps)/sizeof(UInt16))
                continue;

            MailPackage::MailItem item[] = {{equip,1},{514,trumpnum[j]},};
            (*i).player->sendMailItem(564, 565, item, 2);
            MailPackage::MailItem item1[] = {{trumps[tlvl],1},};
            (*i).player->sendMailItem(564, 565, item1, 1, false);
            SYSMSG_BROADCASTV(557, j+1, (*i).player->getCountry(), (*i).player->getName().c_str(), equip, 514, trumpnum[j], trumps[tlvl], 1);
        }

        if ((j >= 3 && j <= 9))
        {
            MailPackage::MailItem item[] = {{trumps[tlvl],1},};
            (*i).player->sendMailItem(564, 565, item, 1, false);
            SYSMSG_BROADCASTV(558, j+1, (*i).player->getCountry(), (*i).player->getName().c_str(), trumps[tlvl], 1);
        }

        if (j == lucky1 || j == lucky2)
        {
            MailPackage::MailItem item[] = {{trumps[tlvl],1},};
            (*i).player->sendMailItem(562, 563, item, 1, false);
            SYSMSG_BROADCASTV(560, (*i).player->getCountry(), (*i).player->getName().c_str(), trumps[tlvl], 1);
        }

        if (j >= 3)
        {
            if (equip_lucky.find(j) != equip_lucky.end())
            {
                UInt16 equip = 0;
                if (lvl == 40)
                    equip = GObject::getRandPEquip(lvl);
                else
                    equip = GObject::getRandOEquip(lvl);
                if (equip)
                {
                    MailPackage::MailItem item[] = {{equip,1},};
                    (*i).player->sendMailItem(562, 563, item, 1);
                }
            }

            if (breath.find(j) != breath.end())
            {
                MailPackage::MailItem item[] = {{508,1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }

            if (gem.find(j) != gem.end())
            {
                UInt8 idx = uRand(sizeof(gems)/sizeof(UInt16));
                MailPackage::MailItem item[] = {{gems[idx],1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }
        }

        MailPackage::MailItem item[] = {{55,1},{MailPackage::Tael,500}};
        (*i).player->sendMailItem(568, 569, item, 2);
        GameAction()->doAty((*i).player, AtyBoss, 0 , 0 );
    }

    if (player)
    {
        MailPackage::MailItem item[] = {{56,5},{MailPackage::Tael,10000},};
        player->sendMailItem(566, 567, item, 2);
        SYSMSG_BROADCASTV(559, player->getCountry(), player->getName().c_str(), 56, 5, 10000);
    }
    m_atkinfo.clear();
}

bool WBoss::attack(WBossMgr* mgr, Player* pl, UInt16 loc, UInt32 id)
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
            if (!((idx+1) % 5))
                m_final = true;
            appear(id, m_id);
        }
    }
    else if (res && m_final)
    {
        mgr->disapper(TimeUtil::Now());
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
        _hp[i] = nflist[i].fighter->getMaxHP();

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

    TRACE_LOG("appear: %u(%u:%u), lvl: %u, loc: %u, count: %u", m_id, npcid, oldid, m_lvl, m_loc, m_count);
    fprintf(stderr, "appear: %u, lvl: %u, loc: %u\n", m_id, m_lvl, m_loc);

    m_disappered = false;
    if (m_count == 10)
        { SYSMSG_BROADCASTV(553, npcid); }
    if (!m_count)
        { SYSMSG_BROADCASTV(554, nflist[0].fighter->getId(), m_loc, nflist[0].fighter->getId()); }

    if (!m_final || m_count == 10)
        sendCount();
    if (m_final)
    {
        sendHpMax();
        sendHp();
    }

    sendId();
    sendLoc();
}

void WBoss::disapper()
{
    if (m_disappered)
        return;

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
    
    TRACE_LOG("disapper: %u, lvl: %u, loc: %u", m_id, m_lvl, m_loc);
}

void WBoss::sendHpMax(Player* player)
{
    if (!_ng)
        return;
    if (!_hp.size())
        return;

    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(0);

    std::vector<GData::NpcFData>& nflist = _ng->getList();
    size_t sz = nflist.size();
    if (!sz) return;

    st << nflist[0].fighter->getMaxHP();
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void WBoss::sendHp(Player* player)
{
    if (!_hp.size())
        return;

    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(1);
    st << _hp[0];
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void WBoss::sendDmg(UInt32 damage)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(2);
    st << damage;
    st << Stream::eos;
    NETWORK()->Broadcast(st);
}

void WBoss::sendLoc(Player* player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(5);
    st << static_cast<UInt32>(m_loc);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void WBoss::sendId(Player* player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(4);
    st << static_cast<UInt32>(m_id);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void WBoss::sendCount(Player* player)
{
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(3);
    st << static_cast<UInt32>(m_count);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
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
    m_level = 1;
#if 1
    _prepareTime = TimeUtil::SharpDayT(1,now) + 12 * 60 * 60 + 45 * 60;
    _appearTime = _prepareTime + 15 * 60;
    _disapperTime = _appearTime + 60 * 60 - 10;
#else
    _prepareTime = TimeUtil::SharpDayT(1,now) + 15 * 60 * 60 + 45 * 60;
    _appearTime = _prepareTime + 15 * 60;
    _disapperTime = _appearTime + 60 * 60 - 10;
#endif
    if (m_boss)
    {
        m_boss->disapper();
        delete m_boss;
        m_boss = NULL;
    }
    TRACE_LOG("out of time. next day: %u, %u", _prepareTime, _prepareStep);
    fprintf(stderr, "out of time. next day: %u\n", _prepareTime);
    return;
}

void WBossMgr::calcNext(UInt32 now)
{
    UInt32 appears[] = {
#if 1
        TimeUtil::SharpDayT(0,now) + 20 * 60 * 60,
        TimeUtil::SharpDayT(0,now) + 18 * 60 * 60 + 45 * 60, /*100*/
        TimeUtil::SharpDayT(0,now) + 17 * 60 * 60 + 45 * 60, /*90*/
        TimeUtil::SharpDayT(0,now) + 16 * 60 * 60 + 45 * 60, /*80*/
        TimeUtil::SharpDayT(0,now) + 15 * 60 * 60 + 45 * 60, /*70*/
        TimeUtil::SharpDayT(0,now) + 14 * 60 * 60 + 45 * 60, /*60*/
        TimeUtil::SharpDayT(0,now) + 13 * 60 * 60 + 45 * 60, /*50*/
        TimeUtil::SharpDayT(0,now) + 12 * 60 * 60 + 45 * 60, /*40*/
        TimeUtil::SharpDayT(0,now),
#else
        TimeUtil::SharpDayT(0,now) + 13*60*60+41*60+21*60,
        TimeUtil::SharpDayT(0,now) + 13*60*60+41*60+18*60,
        TimeUtil::SharpDayT(0,now) + 13*60*60+41*60+15*60,
        TimeUtil::SharpDayT(0,now) + 13*60*60+41*60+12*60,
        TimeUtil::SharpDayT(0,now) + 13*60*60+41*60+9*60,
        TimeUtil::SharpDayT(0,now) + 13*60*60+41*60+6*60,
        TimeUtil::SharpDayT(0,now) + 13*60*60+41*60+3*60,
        TimeUtil::SharpDayT(0,now) + 13*60*60+41*60+10,
        TimeUtil::SharpDayT(0,now),
#endif
    };

    if ((m_level+1) * 5 > sizeof(worldboss)/sizeof(UInt32))
    {
        nextDay(now);
        return;
    }

    for (UInt8 i = 0; i < WBOSS_NUM+2; ++i)
    {
        if (now >= appears[i] && i == 0)
        {
            nextDay(now);
            return;
        }

        if (i <= WBOSS_NUM && now >= appears[i])
        {
            if (m_boss && m_boss->isDisappered())
            {
                UInt8 dstlvl = WBOSS_NUM-i+1;
                if (i == 1 && dstlvl == m_level)
                {
                    nextDay(now);
                    return;
                }

                if (dstlvl == m_level)
                {
                    _prepareTime = appears[i-1];
                    m_level = dstlvl + 1;
                }
                else
                {
                    _prepareTime = appears[i];
                    m_level = dstlvl;
                }

                if (m_maxlvl < (40+(m_level-1)*10))
                {
                    nextDay(now);
                    return;
                }
            }
            else
            {
                if (m_maxlvl < (40+(WBOSS_NUM-i)*10))
                {
                    nextDay(now);
                    return;
                }

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
                _appearTime = _prepareTime + 15 * 60;
                _disapperTime = _appearTime + 60 * 60 - 60;
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
                _appearTime = _prepareTime + 15 * 60;
                _disapperTime = _appearTime + 60 * 60 - 60;
            }
            m_level = 1;
        }
    }

    if ((m_level) * 5 > sizeof(worldboss)/sizeof(UInt32))
    {
        nextDay(now);
        return;
    }

    if (!cfg.GMCheck)
    {
        fprintf(stderr, "lvl: %u, time: %u\n", m_level, _prepareTime);
    }
    TRACE_LOG("lvl: %u, time: %u", m_level, _prepareTime);
}

void WBossMgr::process(UInt32 now)
{
    if (!_prepareTime)
        calcNext(now);
    broadcastTV(now);
    if (now >= _disapperTime && m_boss && !m_boss->isDisappered())
    {
        disapper(now);
        if (m_boss)
            m_boss->flee();
    }
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
    m_boss->attack(this, pl, loc, npcid);
}

void WBossMgr::disapper(UInt32 now)
{
    if (m_boss && !m_boss->isDisappered())
    {
        m_boss->disapper();

        Stream st(REP::DAILY_DATA);
        st << static_cast<UInt8>(6);
        st << static_cast<UInt8>(5);
        st << static_cast<UInt32>(0);
        st << Stream::eos;
        NETWORK()->Broadcast(st);
    }
    _prepareStep = 0;
    calcNext(now);
}

void WBossMgr::sendDaily(Player* player)
{
    if (!player)
        return;
    if (m_boss && !m_boss->isDisappered())
    {
        m_boss->sendHpMax(player);
        m_boss->sendHp(player);
        m_boss->sendCount(player);
        m_boss->sendId(player);
        m_boss->sendLoc(player);
    }
}

void WBossMgr::bossAppear(UInt8 lvl, bool force)
{
    if (lvl > 7)
        return;

    if (m_boss && (!m_boss->isDisappered() || !lvl))
        m_boss->disapper();

    UInt32 now = TimeUtil::Now();
    if (!force)
    {
        if (lvl)
        {
            if (cfg.GMCheck)
            {
                _prepareTime = now - 14 * 60;
                _appearTime = _prepareTime + 20;
                _disapperTime = _appearTime + 60 * 60 - 10;
                _prepareStep = 0;
            }
            else
            {
                _prepareTime = now;
                _appearTime = _prepareTime;
                _disapperTime = _appearTime + 2 * 60 - 10;
                _prepareStep = 0;
            }
        }
    }
    else
    {
        appear(lvl, now);
    }
}

WBossMgr worldBoss;

} // namespace GObject


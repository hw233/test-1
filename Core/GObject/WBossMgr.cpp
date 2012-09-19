
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
#include "MapCollection.h"
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include "AthleticsRank.h"
#include "Script/BattleFormula.h"

namespace GObject
{

const UInt8 WBOSS_NUM = 7;
const UInt8 WBOSS_ATTKMAX = 10;
const float WBOSS_BASE_TIME = 300.f;
const float WBOSS_HP_FACTOR = 1.f;
const float WBOSS_ATK_FACTOR = .5f;
const UInt8 WBOSS_BASE_LVL = 20;
const UInt32 WBOSS_MIN_HP = 20000000;
const UInt32 WBOSS_MAX_HP = 350000000;
const float WBOSS_MAX_ASC_HP_FACTOR = 1.40f;

#if 1
#define AWARD_AREA1 3
#define AWARD_AREA2 10
#define AWARD_MAN 20
#else
#define AWARD_AREA1 3
#define AWARD_AREA2 10
#define AWARD_MAN 20
#endif

static unsigned int attackPre = 0; // XXX: 攻击BOSS的先后顺序因子

static UInt8 getAttackMax()
{
    return WBOSS_ATTKMAX;
}

#if 1
#define DEMONS_1 6235
#define DEMONS_2 6236
#define DEMONS_3 6237
#define DEMONS_4 6238
#define DEMONS_5 6239
#define DEMONS_6 6240
#define DEMONS_7 6241
#else
#define DEMONS_1 5197
#define DEMONS_2 5514
#define DEMONS_3 5514
#define DEMONS_4 5514
#define DEMONS_5 5514
#define DEMONS_6 5514
#define DEMONS_7 5514
#endif

static UInt32 worldboss[] = {
    5466, 5509,
    5162, 5510,
    5103, 5511,
    5168, 5512,
    5127, 5513,
    5197, 5514,
    DEMONS_1, DEMONS_2, DEMONS_3, DEMONS_4, DEMONS_5, DEMONS_6, DEMONS_7,
};

static UInt32 worldboss1[] = {
    5466, 7000,
    5162, 7001,
    5103, 7002,
    5168, 7003,
    5127, 7004,
    5197, 7005,
    DEMONS_1, DEMONS_2, DEMONS_3, DEMONS_4, DEMONS_5, DEMONS_6, DEMONS_7,
};

static UInt32 demons[] = {DEMONS_1, DEMONS_2, DEMONS_3, DEMONS_4, DEMONS_5, DEMONS_6, DEMONS_7};

static UInt8 bossidx[][2] = {
    {0,3},
    {1,4},
    {2,5},
    {3,0},
    {4,1},
    {5,2},
    {6,6},
};

static UInt8 getColor(UInt8 lvl)
{
    if (lvl >= 90)
        return 4;
    if (lvl >= 80)
        return 3;
    if (lvl >= 70)
        return 2;
    if (lvl >= 60)
        return 1;
    if (lvl >= 50)
        return 0;
    if (lvl >= 40)
        return 10;
    return 10;
}

static UInt32 getExp(UInt8 lvl)
{
    if (lvl >= 90)
        return 27187500;
    if (lvl >= 80)
        return 21187500;
    if (lvl >= 70)
        return 15937500;
    if (lvl >= 60)
        return 11437500;
    if (lvl >= 50)
        return 7687500;
    if (lvl >= 40)
        return 4687500;
    return 4687500;
}

bool WBoss::attackWorldBoss(Player* pl, UInt32 npcId, UInt8 expfactor, bool final)
{
    static UInt32 sendflag = 7;

    ++sendflag;

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
    pl->PutFighters(bsim, 0);
    _ng->putFighters(bsim);

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

    UInt32 thisDay = TimeUtil::SharpDay();
    UInt32 sixthDay = TimeUtil::SharpDay(5, PLAYER_DATA(pl, created));
    if(thisDay == sixthDay && !pl->GetVar(VAR_CLAWARD2))
    {
        pl->SetVar(VAR_CLAWARD2, 1);
        pl->sendRC7DayInfo(TimeUtil::Now());
    }

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
                UInt32 exp = ((float)damage / nflist[0].fighter->getMaxHP()) * _ng->getExp() * expfactor;
                if (exp < 1000)
                    exp = 1000;
                pl->pendExp(exp);
                if (!(sendflag % 8))
                    sendDmg(damage);

                AttackInfo info(pl, damage);
                AtkInfoType::iterator i = m_atkinfo.begin();
                while (i != m_atkinfo.end())
                {
                    if ((*i).player == pl)
                    {
                        info += *i;
                        m_atkinfo.erase(i);
                        break;
                    }
                    ++i;
                }
                bool ret = m_atkinfo.insert(info).second;
                TRACE_LOG("WBOSS INSERT ret: %u (pid: %"I64_FMT"u, dmg: %u)", ret, pl->getId(), damage);

                UInt8 newPercent = ((float)newHP / nflist[0].fighter->getMaxHP()) * 100;

                if (newPercent > 100)
                    newPercent = 100;
                if (_percent < newPercent)
                    _percent = newPercent;
                if (!newPercent)
                {
                    SYSMSG_BROADCASTV(550, nflist[0].fighter->getId());
                    _percent = 0;
                    _hp[0] = 0;
                    attackPre = 0;
                    reward(pl);
                    res = true;
                    if (sendflag % 8)
                        sendHp();

                    //updateLastDB(TimeUtil::Now());

                    nflist[0].fighter->setExtraAttack(0);
                    nflist[0].fighter->setExtraMagAttack(0);
                    nflist[0].fighter->setWBoss(false);
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
                if (!(sendflag % 8))
                    sendHp();
            }
            else
            {
                TRACE_LOG("WBOSS OLDHP(%u) < NEWHP(%u)", oldHP, newHP);
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

void WBoss::updateLastDB(UInt32 end)
{
    UInt8 idx = bossidx[World::_wday-1][m_idx];
    if (!m_appearTime)
        return;

    if (m_appearTime > end)
        return;

    UInt32 last = end - m_appearTime;
    Last l;
    l.time = last;
    l.hp = m_lastHP;
    l.atk = m_lastAtk;
    l.matk = m_lastMAtk;
    _mgr->setLast(idx, l);
    DB3().PushUpdateData("REPLACE INTO `wboss` (`idx`, `last`, `hp`, `atk`, `matk`) VALUES (%u, %u, %u, %d, %d)",
            idx, last, m_lastHP, m_lastAtk, m_lastMAtk);
}

void WBoss::getRandList(UInt32 sz, UInt32 num, std::set<UInt32>& ret)
{
    if (sz > AWARD_MAN)
    {
        UInt32 j = uRand(sz-AWARD_MAN);
        for (UInt32 i = 0; i < num; ++i)
        {
            while (ret.find(j+AWARD_MAN) != ret.end())
                j = uRand(sz-AWARD_MAN);
            ret.insert(j+AWARD_MAN);
        }
    }
}

void WBoss::flee()
{
    SYSMSG_BROADCASTV(570, m_id);
}

void WBoss::reward(Player* player)
{
    static UInt16 trumpFrag[] = {226,90,225,227,270,298};
    static UInt16 gems[] = {5003,5013,5023,5033,5043,5053,5063,5073,5083,5093,5103,5113,5123,5133,5143,};

    size_t sz = m_atkinfo.size();
    if (!sz) return;

    UInt8 tlvl = 0;
    if (World::getOpenTest())
    {
        if (m_id >= 7000 && m_id <= 7005)
            tlvl = m_id-7000;
    }
    else
    {
        if (m_id >= 5509 && m_id <= 5514)
            tlvl = m_id-5509;
    }

    UInt32 lucky1 = uRand(sz);
    UInt32 lucky2 = uRand(sz);
    if (lucky1 == lucky2)
        lucky2 = uRand(sz);

    std::set<UInt32> comp;
    std::set<UInt32> breath;
    std::set<UInt32> gem;
    std::set<UInt32> _503;
    std::set<UInt32> _515;
    std::set<UInt32> _507;
    std::set<UInt32> _509;

    UInt32 luckynum = (float)10 * sz / 100;
    getRandList(sz, luckynum, gem); // 宝石

    luckynum = (float)5 * sz / 100;
    getRandList(sz, luckynum, breath); // 凝神丹
    getRandList(sz, luckynum, comp); // 补髓丹

    if (World::_wday == 7)
        tlvl = uRand(sizeof(trumpFrag)/sizeof(UInt16));

    bool notify4_10 = false;
    bool notify11_20 = false;
    UInt32 j = 0;
    for (AtkInfoType::reverse_iterator i = m_atkinfo.rbegin(); i != m_atkinfo.rend(); ++i, ++j)
    {
        (*i).player->wBossUdpLog(1096 + m_idx);
        if (j < AWARD_AREA1)
        {
            MailPackage::MailItem item1[] = {{trumpFrag[tlvl],4},};
            (*i).player->sendMailItem(564, 565, item1, 1, false);
            SYSMSG_BROADCASTV(557, j+1, (*i).player->getCountry(), (*i).player->getName().c_str(), trumpFrag[tlvl], 4);
        }

        if (j >= AWARD_AREA1 && j < AWARD_AREA2)
        {
            MailPackage::MailItem item1[] = {{trumpFrag[tlvl],2},};
            (*i).player->sendMailItem(564, 565, item1, 1, false);
            if (!notify4_10)
            {
                SYSMSG_BROADCASTV(558, trumpFrag[tlvl], 2);
                notify4_10 = true;
            }
        }

        if (j >= AWARD_AREA2 && j < AWARD_MAN)
        {
            MailPackage::MailItem item1[] = {{trumpFrag[tlvl],1},};
            (*i).player->sendMailItem(564, 565, item1, 1, false);
            if (!notify11_20)
            {
                SYSMSG_BROADCASTV(571, trumpFrag[tlvl], 1);
                notify11_20 = true;
            }
        }

        if (j == lucky1 || j == lucky2) // 法宝碎片
        {
            MailPackage::MailItem item[] = {{trumpFrag[tlvl],1},};
            (*i).player->sendMailItem(562, 563, item, 1, false);
            SYSMSG_BROADCASTV(560, (*i).player->getCountry(), (*i).player->getName().c_str(), trumpFrag[tlvl], 1);
        }

        if (j >= AWARD_MAN)
        {
            if (comp.find(j) != comp.end())
            {
                MailPackage::MailItem item[] = {{506,1},};
                (*i).player->sendMailItem(562, 563, item, 1);
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

            if (_503.find(j) != _503.end())
            {
                MailPackage::MailItem item[] = {{503,1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }

            if (_515.find(j) != _515.end())
            {
                MailPackage::MailItem item[] = {{515,1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }

            if (_507.find(j) != _507.end())
            {
                MailPackage::MailItem item[] = {{507,1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }

            if (_509.find(j) != _509.end())
            {
                MailPackage::MailItem item[] = {{509,1},};
                (*i).player->sendMailItem(562, 563, item, 1);
            }
        }

        if (World::getOpenTest() && World::_wday != 7)
        {
            MailPackage::MailItem item[] = {{1526,1},{MailPackage::Tael,1000},{9091,1}};
            (*i).player->sendMailItem(568, 569, item, 3);
        }
        else
        {
            MailPackage::MailItem item[] = {{1526,1},{MailPackage::Tael,1000}};
            (*i).player->sendMailItem(568, 569, item, 2);
        }

        GameAction()->doAty((*i).player, AtyBoss, 0, 0);
        (*i).player->setContinuousRFAward(6);
    }

    if (player)
    {
        MailPackage::MailItem item[] = {{56,5},{MailPackage::Tael,20000},};
        player->sendMailItem(566, 567, item, 2);
        SYSMSG_BROADCASTV(559, player->getCountry(), player->getName().c_str(), 56, 5, 20000);
    }
    m_atkinfo.clear();
}

bool WBoss::attack(Player* pl, UInt16 loc, UInt32 id)
{
    bool in = false;
    for (int i = 0; i < 2; ++i)
    {
        if (World::getOpenTest())
        {
            if (worldboss1[2*bossidx[World::_wday-1][m_idx]+i] == id)
            {
                in = true;
                break;
            }
        }
        else
        {
            if (worldboss[2*bossidx[World::_wday-1][m_idx]+i] == id)
            {
                in = true;
                break;
            }
        }
    }

    if (!in && World::_wday == 7 && (id == DEMONS_3 || id == DEMONS_4 || id == DEMONS_5 || id == DEMONS_6 || id == DEMONS_7))
        in = true;

    if (!in) return false;

    if (pl->getLocation() != loc)
        return false;

    if (loc != m_loc)
        return false;

    if (pl->getThreadId() !=  mapCollection.getCountryFromSpot(loc))
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

    if (m_final)
        pl->OnHeroMemo(MC_SLAYER, MD_LEGEND, 0, 0);
    if (World::_wday == 7)
        pl->OnHeroMemo(MC_SLAYER, MD_LEGEND, 0, 1);
    if (!m_final)
        pl->OnHeroMemo(MC_SLAYER, MD_LEGEND, 0, 2);
    pl->OnShuoShuo(SS_WBOSS);

    if (!m_final)
    {
        // XXX: 打元神也参与抽奖
        AttackInfo info(pl, attackPre);
        AtkInfoType::iterator i = m_atkinfo.begin();
        while (i != m_atkinfo.end())
        {
            if ((*i).player == pl)
            {
                m_atkinfo.erase(i);
                break;
            }
            ++i;
        }
        m_atkinfo.insert(info);
        ++attackPre;
    }

    bool res = attackWorldBoss(pl, m_id, World::_wday==4?2:1, m_final);
    if (res && !m_final)
    {
        ++m_count;
        if (m_count <= m_maxcnt)
        {
            SYSMSG_BROADCASTV(552, pl->getCountry(), pl->getName().c_str(), loc, m_count, m_id);
            UInt8 idx = 2*bossidx[World::_wday-1][m_idx] + (m_count-1)/9;
            UInt32 id = 0;
            if (World::getOpenTest())
            {
                if (idx < sizeof(worldboss1)/sizeof(UInt32))
                    id = worldboss1[idx];
            }
            else
            {
                if (idx < sizeof(worldboss)/sizeof(UInt32))
                    id = worldboss[idx];
            }
            if (!((idx+1) % 2))
                m_final = true;
            id = _mgr->fixBossId(id, m_idx);
            appear(id, m_id);
        }
    }
    else if (res && m_final)
    {
        _mgr->disapper(TimeUtil::Now());
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

    std::vector<GData::NpcFData>& nflist = _ng->getList();

    if (m_final)
    {
        setLast(_mgr->getLastTime(bossidx[World::_wday-1][m_idx]));
        setAppearTime(TimeUtil::Now());
        _mgr->fixBossName(npcid, nflist[0].fighter, m_idx);
    }

    _mgr->setBossName(m_idx, nflist[0].fighter->getName());

    _hp.clear();
    size_t sz = nflist.size();
    if (!sz) return;
    _hp.resize(sz);

    UInt32 ohp = 0;
    if (m_final)
        ohp = _mgr->getLastHP(bossidx[World::_wday-1][m_idx]);
    if (m_final && !ohp)
        ohp = nflist[0].fighter->getMaxHP();
    _hp[0] = ohp;

    Int32 extatk = 0;
    Int32 extmagatk = 0;
    Int32 atk = 0;
    Int32 matk = 0;
    if (m_final && nflist[0].fighter && !m_extra && m_last)
    {
        //UInt32 exthp = 0;

        atk = _mgr->getLastAtk(bossidx[World::_wday-1][m_idx]);
        matk = _mgr->getLastMAtk(bossidx[World::_wday-1][m_idx]);

        Int32 baseatk = Script::BattleFormula::getCurrent()->calcAttack(nflist[0].fighter);
        Int32 basematk = Script::BattleFormula::getCurrent()->calcMagAttack(nflist[0].fighter);

        float hp_factor = (float)WBOSS_BASE_TIME / (float)m_last;
        if(hp_factor > WBOSS_MAX_ASC_HP_FACTOR)
            hp_factor = WBOSS_MAX_ASC_HP_FACTOR;
        ohp = ohp * hp_factor;
        if (ohp < WBOSS_MIN_HP)
            ohp = WBOSS_MIN_HP;
        if (ohp > WBOSS_MAX_HP)
            ohp = WBOSS_MAX_HP;

        setHP(ohp);
        nflist[0].fighter->setBaseHP(ohp);
        nflist[0].fighter->setWBoss(true);

        extatk = (WBOSS_BASE_TIME/(float)m_last - 1.f) * WBOSS_ATK_FACTOR * (atk + baseatk);
        nflist[0].fighter->setExtraAttack(extatk + atk);

        extmagatk = (WBOSS_BASE_TIME/(float)m_last - 1.f) * WBOSS_ATK_FACTOR * (matk + basematk);
        nflist[0].fighter->setExtraMagAttack(extmagatk + matk);

        TRACE_LOG("BOSS: hp: %u, base attack: %u, extra attack: %d, magattack: %u, extra magattack: %d",
                nflist[0].fighter->getBaseHP(), nflist[0].fighter->getBaseAttack(),
                nflist[0].fighter->getExtraAttack(), nflist[0].fighter->getBaseMagAttack(), nflist[0].fighter->getExtraMagAttack());
#ifdef _DEBUG
        fprintf(stderr, "BOSS: hp: %u, base attack: %u, extra attack: %d, magattack: %u, extra magattack: %d",
                nflist[0].fighter->getBaseHP(), nflist[0].fighter->getBaseAttack(),
                nflist[0].fighter->getExtraAttack(), nflist[0].fighter->getBaseMagAttack(), nflist[0].fighter->getExtraMagAttack());
#endif
        m_extra = true;
    }

    if (m_final)
    {
        m_lastHP = ohp;
        m_lastAtk = extatk + atk;
        m_lastMAtk = extmagatk + matk;

        UInt8 level = WBOSS_BASE_LVL + _hp[0]/1500000;
        nflist[0].fighter->setLevel(level);
        nflist[0].fighter->setColor(getColor(_mgr->getLevel()));
        _ng->setExp(getExp(_mgr->getLevel()));
        TRACE_LOG("BOSS level: %u, color: %u, exp: %u", nflist[0].fighter->getLevel(), nflist[0].fighter->getColor(), _ng->getExp());
#ifdef DEBUG
        fprintf(stderr, "BOSS level: %u, color: %u, exp: %u", nflist[0].fighter->getLevel(), nflist[0].fighter->getColor(), _ng->getExp());
#endif
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

    TRACE_LOG("appear: %u(%u:%u), idx: %u, loc: %u, count: %u", m_id, npcid, oldid, m_idx, m_loc, m_count);
    fprintf(stderr, "appear: %u, idx: %u, loc: %u\n", m_id, m_idx, m_loc);

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

    _mgr->setBossSt(m_idx, 1);
}

void WBoss::disapper()
{
    if (m_disappered)
        return;

    Map * map = Map::FromSpot(m_loc);
    if (!map) return;
    map->Hide(m_id);
    map->DelObject(m_id);

    _mgr->setBossName(m_idx, ""); // XXX: must before setBossSt
    _mgr->setBossSt(m_idx, 2);

    if(m_final)
        updateLastDB(TimeUtil::Now());
    m_extra = false;
    m_last = 0;
    m_appearTime = 0;
    m_count = 0;
    m_disappered = true;
    m_final = false;
    _percent = 100;
    _ng = NULL;
    _hp.clear();

    TRACE_LOG("disapper: %u, idx: %u, loc: %u", m_id, m_idx, m_loc);
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

WBossMgr::WBossMgr()
    : _prepareTime(0), _prepareStep(0), _appearTime(0),
    _disapperTime(0), m_idx(0), m_maxlvl(0), m_boss(NULL)
{
    memset(m_lasts, 0x00, sizeof(m_lasts));
    m_bossID[0] = m_bossID[1] = 0;
    m_bossSt[0] = m_bossSt[1] = 0;

    UInt32 now = TimeUtil::Now();
    if (now > TimeUtil::SharpDayT(0,now) + 16 * 60 * 60 + 15 * 60)
        m_bossSt[0] = m_bossSt[1] = 2;
}

bool WBossMgr::isWorldBoss(UInt32 npcid)
{
    if (World::getOpenTest())
    {
        for (UInt8 i = 0; i < sizeof(worldboss1)/sizeof(UInt32); ++i)
        {
            if (cfg.GMCheck || true)
            {
                if (worldboss1[i] == npcid)
                    return true;
            }
            else
            {
                if (worldboss1[i] == npcid)
                    return true;
            }
        }
    }
    else
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
    }
    return false;
}

void WBossMgr::nextDay(UInt32 now)
{
    _prepareTime = TimeUtil::SharpDayT(1,now) + 10 * 60 * 60 + 15 * 60;
    _appearTime = _prepareTime + 15 * 60;
    _disapperTime = _appearTime + 60 * 60 - 10;

    if (m_boss)
    {
        m_boss->disapper();
        m_boss->setIdx(0);
        m_idx = 0;
        //delete m_boss;
        //m_boss = NULL;
    }
    TRACE_LOG("out of time. next day: %u, %u", _prepareTime, _prepareStep);
    fprintf(stderr, "out of time. next day: %u\n", _prepareTime);
    return;
}

void WBossMgr::calcNext(UInt32 now)
{
    UInt32 appears[] = {
        TimeUtil::SharpDayT(0,now) + 16 * 60 * 60 + 15 * 60,
        TimeUtil::SharpDayT(0,now) + 15 * 60 * 60 + 15 * 60,
        TimeUtil::SharpDayT(0,now) + 10 * 60 * 60 + 15 * 60,
    };

    if (m_idx == 1)
    {
        nextDay(now);
        return;
    }

    _prepareTime = 0;
    for (UInt8 i = 0; i < sizeof(appears)/sizeof(UInt32); ++i)
    {
        if (now > appears[i])
        {
            if (i == 0)
            {
                m_idx = 0;
                nextDay(now);
                return;
            }

            UInt8 idx = i;
            if (m_boss && m_boss->isDisappered() && m_boss->getIdx() == sizeof(appears)/sizeof(UInt32)-i-1)
                idx = idx - 1;

            _prepareTime = appears[idx];
            m_idx = sizeof(appears)/sizeof(UInt32)-idx-1;
            break;
        }
    }

    if (!_prepareTime)
    {
        m_idx = 0;
        _prepareTime = appears[sizeof(appears)/sizeof(UInt32)-1];
    }

    _appearTime = _prepareTime + 15 * 60;
    _disapperTime = _appearTime + 60 * 60 - 60;

    if (!cfg.GMCheck)
    {
        fprintf(stderr, "boss time: %u\n", _prepareTime);
    }
    TRACE_LOG("boss time: %u", _prepareTime);
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
        {
            m_boss->flee();
            m_boss->setId(0); // XXX: 修正心魔逃走后无法显示BOSS问题
        }
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
            appear(now);
            break;

        default:
            break;
    }
}

void WBossMgr::resetBossSt()
{
    m_bossID[0] = m_bossID[1] = 0;
    m_bossSt[0] = m_bossSt[1] = 0;
    m_bossName[0] = m_bossName[1] = "";
    sendBossInfo(NULL);
}

UInt16 WBossMgr::fixBossId(UInt16 id, UInt8 idx)
{
    if (id != DEMONS_2)
        return id;
    AthleticsRank::Rank it = gAthleticsRank.getRankBegin(1);
    AthleticsRank::Rank end = gAthleticsRank.getRankEnd(1);
    if (it == end)
        return id;
    if (idx == 1)
        ++it;
    if (it == end)
        return id;
    Player* first = (*it)->ranker;
    if (!first)
        return id;
    UInt8 cls = first->GetClass();
    UInt8 sex = first->IsMale()?0:3;
    return demons[cls+sex];
}

void WBossMgr::fixBossName(UInt16 id, Fighter* fighter, UInt8 idx)
{
    if (id == DEMONS_2 || id == DEMONS_3 || id == DEMONS_4 || id == DEMONS_5 || id == DEMONS_6 || id == DEMONS_7)
    {
        AthleticsRank::Rank it = gAthleticsRank.getRankBegin(1);
        AthleticsRank::Rank end = gAthleticsRank.getRankEnd(1);
        if (it == end)
            return;
        if (idx == 1)
            ++it;
        if (it == end)
            return;
        Player* first = (*it)->ranker;
        if (!first)
            return;
        SYSMSG(pre, 2353);
        std::string name = first->getName() + pre;
        fighter->setName(name);
    }
}

void WBossMgr::appear(UInt32 now)
{
    UInt32 npcid = 0;
    UInt8 idx = 2*bossidx[World::_wday-1][m_idx];
    if (World::getOpenTest())
    {
        if (idx >= sizeof(worldboss1)/sizeof(UInt32))
        {
            nextDay(now);
            return;
        }
        npcid = worldboss1[idx];
    }
    else
    {
        if (idx >= sizeof(worldboss)/sizeof(UInt32))
        {
            nextDay(now);
            return;
        }
        npcid = worldboss[idx];
    }
    npcid = fixBossId(npcid, m_idx);

    std::vector<UInt16> spots;
    Map::GetAllSpot(spots);
    if (!spots.size()) return;

    UInt16 spot = spots[uRand(spots.size())];
    if (!spot) return;

    UInt32 oldnpcid = 0;
    if (!m_boss)
    {
        m_boss = new WBoss(this, npcid, 0, getAttackMax(), spot, m_idx);
    }
    else
    {
        m_boss->setIdx(m_idx);
        m_boss->setLoc(spot);
        oldnpcid = m_boss->getId();
    }
    m_boss->appear(npcid, oldnpcid);
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
        appear(now);
    }
}

void WBossMgr::setHP(UInt32 hp)
{
    if (m_boss)
        m_boss->setHP(hp);
}

void WBossMgr::setBossSt(UInt8 idx, UInt8 st)
{
    m_bossSt[idx] = st;
    sendBossInfo(NULL);
}

void WBossMgr::setBossName(UInt8 idx, std::string name)
{
    m_bossName[idx] = name;
}

void WBossMgr::sendBossInfo(Player* pl)
{
    if (World::getOpenTest())
    {
        m_bossID[0] = fixBossId(worldboss1[2*bossidx[World::_wday-1][0]+1], 0);
        m_bossID[1] = fixBossId(worldboss1[2*bossidx[World::_wday-1][1]+1], 1);
    }
    else
    {
        m_bossID[0] = fixBossId(worldboss[2*bossidx[World::_wday-1][0]+1], 0);
        m_bossID[1] = fixBossId(worldboss[2*bossidx[World::_wday-1][1]+1], 1);
    }

    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(6);
    st << static_cast<UInt8>(0);
    st << static_cast<UInt8>(m_bossSt[0]);
    st << static_cast<UInt16>(m_bossID[0]);
    st << m_bossName[0];
    st << Stream::eos;
    if (pl)
        pl->send(st);
    else
        NETWORK()->Broadcast(st);

    Stream st1(REP::DAILY_DATA);
    st1 << static_cast<UInt8>(6);
    st1 << static_cast<UInt8>(6);
    st1 << static_cast<UInt8>(1);
    st1 << static_cast<UInt8>(m_bossSt[1]);
    st1 << static_cast<UInt16>(m_bossID[1]);
    st1 << m_bossName[1];
    st1 << Stream::eos;
    if (pl)
        pl->send(st1);
    else
        NETWORK()->Broadcast(st1);
}

WBossMgr worldBoss;

} // namespace GObject


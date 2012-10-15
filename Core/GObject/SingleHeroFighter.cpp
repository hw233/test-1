#include "Config.h"
#include "MsgID.h"
#include "SingleHeroFighter.h"
#include "Fighter.h"
#include "Player.h"
#include "SHSYTmpl.h"

#include "DB/DBConnectionMgr.h"
#include "DB/DBExecutor.h"
#include "DB/DBExecHelper.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"



namespace GObject
{
#define CITTA_BOOK_ID 1199

    SingleHeroFighter::SingleHeroFighter(Player* pl, UInt8 type) : _player(pl), _fgt(NULL), _type(type), _level(0), _soulMax(0), _potential(0), _capacity(0), _soulExtraAura(0), _soulAuraLeft(0), _tLevel(0), _tTurns(0), _tLastTurns(0)
    {
        memset(_citta, 0, sizeof(_citta));
        memset(_trump, 0, sizeof(_trump));
    }

    SingleHeroFighter::SingleHeroFighter(Player* pl, Fighter* fgt, UInt8 type) : _player(pl), _type(type), _tLevel(0), _tTurns(0), _tLastTurns(0)
    {
        _fgt = fgt;
        _level = fgt->getLevel();
        _soulMax = fgt->getMaxSoul();
        _potential = fgt->getPotential();
        _capacity = fgt->getCapacity();
        UInt16 peerless = fgt->getPeerlessLevel();

        std::vector<UInt16> skills;
        fgt->getUpSkillAndLevel(skills);
        UInt8 cnt = skills.size();

        UInt8 idx = 0;
        for(idx = 0; idx < CITTA_UPMAX; ++ idx)
        {
            _citta[idx] = fgt->getUpCittaAndLevel(idx);
        }

        for (idx = 0; idx < cnt; ++idx)
        {
            if (skills[idx])
            {
                SStrengthen* ss = fgt->SSGetInfo(skills[idx]);
                if (ss)
                {
                    UInt16 skill_id = SKILL_ID(skills[idx]);
                    _ss[skill_id] = static_cast<UInt16>(SKILLANDLEVEL(skill_id, ss->lvl));
                }
            }
        }
        if (peerless != 0)
        {
            SStrengthen* ss = fgt->SSGetInfo(peerless);
            if (ss)
            {
                UInt16 skill_id = SKILL_ID(peerless);
                _ss[skill_id] = static_cast<UInt16>(SKILLANDLEVEL(skill_id, ss->lvl));
            }
        }

        for(int i = 0; i < TRUMP_UPMAX; ++ i)
            _trump[i] = fgt->getTrumpSkill(i);

        _attrExtraEquip = *(fgt->getAttrExtraEquip());
        _elixirattr = fgt->getElixirAttr();
        _soulExtraAura = fgt->getSoulExtraAura();
        _soulAuraLeft = fgt->getSoulAuraLeft();
        _fgt->setName(pl->getName());
        writeDB();
    }

    SingleHeroFighter::SingleHeroFighter(Player* pl, UInt8 cls, UInt8 type) : _player(pl), _fgt(NULL), _type(type), _level(0), _soulMax(0), _potential(0), _capacity(0), _soulExtraAura(0), _soulAuraLeft(0), _tLevel(0), _tTurns(0), _tLastTurns(0)
    {
        memset(_citta, 0, sizeof(_citta));
        memset(_trump, 0, sizeof(_trump));

        const UInt8 fId[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        UInt8 sex = pl->IsMale() ? 0 : 1;
        Fighter * fgt = globalFighters[fId[2*(cls-1)+sex]];
        if(fgt == NULL)
            return;
        _fgt = fgt->clone(NULL);
        if(_fgt == NULL)
            return;

        setLevel(shsyTmpl._level);
        setPotential(shsyTmpl._potential);
        setSoulMax(shsyTmpl._soulMax);
        setAttrExtraEquip(shsyTmpl._attrExtraEquip[cls]);
        _fgt->setName(pl->getName());
        _fgt->setUpCittasMax();
        writeDB();
    }

    SingleHeroFighter::~SingleHeroFighter()
    {
        if(_fgt)
            delete _fgt;
    }

    void SingleHeroFighter::setSoulMax(Int32 soulMax)
    {
        _soulMax = soulMax;
        _fgt->setSoulMax(soulMax);
    }

    void SingleHeroFighter::setAttrExtraEquip(GData::AttrExtra& attr)
    {
        _attrExtraEquip = attr;
        _fgt->resetAttrExtraEquip2();
        _fgt->setAttrExtraEquip(attr);
    }

    void SingleHeroFighter::setAttr2(UInt8 soulExtraAura, UInt8 soulAuraLeft, ElixirAttr& attr)
    {
        _soulExtraAura = soulExtraAura;
        _soulAuraLeft = soulAuraLeft;
        _elixirattr = attr;
        _fgt->setElixirAttr(attr);
        _fgt->setSoulAuraLeft(soulAuraLeft);
        _fgt->setSoulExtraAura(soulExtraAura);
    }

    void SingleHeroFighter::setPotential(float potential)
    {
        _potential = potential;
        _fgt->setPotential(potential, false);
    }

    void SingleHeroFighter::setCapacity(float capacity)
    {
        _capacity = capacity;
        _fgt->setCapacity(capacity, false);
    }

    void SingleHeroFighter::setLevel(UInt8 level)
    {
        _level = level;
        _fgt->setLevel(level);
    }

    void SingleHeroFighter::setUpCittas(std::string& citta)
    {
        if (!citta.length())
            return;

        StringTokenizer tk(citta, ",");
        for (size_t i = 0; i < tk.count() || i < CITTA_UPMAX; ++i)
        {
            _citta[i] = atoi(tk[i].c_str());
            _fgt->upCittaWithOutCheck(_citta[i], i);
        }
    }

    void SingleHeroFighter::setUpTrumps(std::string& trump)
    {
        if (!trump.length())
            return;

        StringTokenizer tk(trump, ",");
        std::vector<const GData::SkillBase*> trumps_skill;
        for (size_t i = 0; i < tk.count() || i < TRUMP_UPMAX; ++i)
        {
            _trump[i] = atoi(tk[i].c_str());
            trumps_skill.push_back(GData::skillManager[_trump[i]]);
        }
        upTrump(trumps_skill);
    }

    void SingleHeroFighter::setUpSS(std::string& skillstrengthen)
    {
        StringTokenizer skills(skillstrengthen, ",");
        for (UInt8 i = 0; i < skills.count(); ++i)
        {
            UInt16 skillId = atoi(skills[i].c_str());
            UInt16 sid = SKILL_ID(skillId);
            _ss[sid] = skillId;
        }
        _fgt->setUpSS(skillstrengthen);
    }

    bool SingleHeroFighter::upCitta( UInt16 citta, int idx )
    {
        if(!_fgt)
            return false;
        _fgt->upCittaWithOutCheck(citta, idx);

        return true;
    }

    bool SingleHeroFighter::upTrump(const std::vector<const GData::SkillBase*>& trumps)
    {
        if(!_fgt)
            return false;
        _fgt->addSkillsFromCT(trumps, false, true);

        return true;
    }

    bool SingleHeroFighter::offTrump(const std::vector<const GData::SkillBase*>& trumps)
    {
        if(!_fgt)
            return false;
        _fgt->delSkillsFromCT(trumps, false);

        return true;
    }

    void SingleHeroFighter::writeDB()
    {
        {
			UInt32 p = static_cast<UInt32>(_fgt->getPotential() * 100 + 0.5);
            UInt32 c = static_cast<UInt32>((_fgt->getCapacity()+0.05) * 100);

            std::string strcitta;
            std::string strtrump;
            std::string strss;
            strCitta(strcitta);
            strTrump(strtrump);
            strSS(strss);

            DB1().PushUpdateData("REPLACE INTO `sh_fighter`(`id`, `playerId`, `potential`, `capacity`, `type`, `level`, `soulMax`, `citta`, `trump`, `skillstrengthen`) VALUES(%u, %"I64_FMT"u, %u.%02u, %u.%02u, %u, %u, %u, \'%s\', \'%s\', \'%s\')", getId(), _player->getId(), p / 100, p % 100, c / 100, c % 100, _type, _fgt->getLevel(), _soulMax,  strcitta.c_str(), strtrump.c_str(), strss.c_str());
        }

        {
            GData::AttrExtra& attr = _attrExtraEquip;
            DB1().PushUpdateData("REPLACE INTO `sh_fighter_attr_extra`(`fighterId`, `playerId`, `type`,"
                "`strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`,"
                "`auraMax`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `tough`, `action`,"
                "`hitrate`, `evade`, `critical`, `criticaldmg`, `pierce`, `counter`, `magres`,"
                "`strengthP`, `physiqueP`, `agilityP`, `intelligenceP`, `willP`, `soulP`, `auraP`,"
                "`auraMaxP`, `attackP`, `magatkP`, `defendP`, `magdefP`, `hpP`, `toughP`, `actionP`,"
                "`hitrateP`, `evadeP`, `criticalP`, `criticaldmgP`, `pierceP`, `counterP`, `magresP`,"
                "`hitrlvl`, `evdlvl`, `crilvl`, `pirlvl`, `counterlvl`, `mreslvl`, `toughlvl`"
                ") VALUES(%u, %"I64_FMT"u, %u, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,"
                    "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f,"
                    "%f, %f, %f, %f, %f, %f, %f)", getId(), _player->getId(), _type,
                    attr.strength, attr.physique, attr.agility, attr.intelligence, attr.will, attr.soul, attr.aura,
                    attr.auraMax, attr.attack, attr.magatk, attr.defend, attr.magdef, attr.hp, attr.tough, attr.action,
                    attr.hitrate, attr.evade, attr.critical, attr.criticaldmg, attr.pierce, attr.counter, attr.magres,
                    attr.strengthP, attr.physiqueP, attr.agilityP, attr.intelligenceP, attr.willP, attr.soulP, attr.auraP,
                    attr.auraMaxP, attr.attackP, attr.magatkP, attr.defendP, attr.magdefP, attr.hpP, attr.toughP, attr.actionP,
                    attr.hitrateP, attr.evadeP, attr.criticalP, attr.criticaldmgP, attr.pierceP, attr.counterP, attr.magresP,
                    attr.hitrlvl, attr.evdlvl, attr.crilvl, attr.pirlvl, attr.counterlvl, attr.mreslvl, attr.toughlvl);
        }

        {
            ElixirAttr& attr = _elixirattr;
            DB1().PushUpdateData("REPLACE INTO `sh_fighter_attr2`(`fighterId`, `playerId`, `type`, `soulExtraAura`, `soulAuraLeft`, `elixir_strength`, `elixir_physique`, `elixir_agility`,"
                    " `elixir_intelligence`, `elixir_will`, `elixir_soul`, `elixir_attack`, `elixir_defend`, `elixir_critical`, `elixir_pierce`, `elixir_evade`, `elixir_counter`,"
                    " `elixir_tough`, `elixir_action`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", getId(), _player->getId(), _type,
                    _soulExtraAura, _soulAuraLeft, attr.strength, attr.physique, attr.agility, attr.intelligence, attr.will, attr.soul, attr.attack, attr.defend, attr.critical,
                    attr.pierce, attr.evade, attr.counter, attr.tough, attr.action);
        }
    }

    void SingleHeroFighter::strSS(std::string& str)
    {
        std::map<UInt16, UInt16>::iterator it = _ss.begin();
        if(it != _ss.end())
        {
            str += Itoa(it->second);
            ++ it;
        }
        for (; it != _ss.end(); ++it)
        {
            str += ",";
            str += Itoa(it->second);
        }
    }

    void SingleHeroFighter::strTrump(std::string& str)
    {
        value2string(_trump, TRUMP_UPMAX, str);
    }

    void SingleHeroFighter::strCitta(std::string& str)
    {
        value2string(_citta, CITTA_UPMAX, str);
    }

    bool SingleHeroFighter::checkCittas(std::vector<UInt16>& cittas)
    {
        size_t cnt = cittas.size();
        for(size_t i = 0; i < cnt; ++ i)
        {
            UInt16 cittaItemId = CITTA_ID(cittas[i]) + CITTA_BOOK_ID;
            const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[cittaItemId];
            if(!itemType)
                return false;
            if(itemType->career != 0 && itemType->career != _fgt->getClass())
                return false;
        }

        return true;
    }

    bool SingleHeroFighter::checkTrumps(std::vector<UInt16>& trumps)
    {
        size_t cnt = trumps.size();
        for(size_t i = 0; i < cnt; ++ i)
        {
            const GData::ItemBaseType * itemType = GData::itemBaseTypeManager[trumps[i]];
            if(!itemType)
                return false;
            if((itemType->career != 0 && itemType->career != _fgt->getClass()) || itemType->subClass != Item_Trump)
                return false;

            const GData::AttrExtra* attr = (static_cast<const GData::ItemTrumpType*>(itemType))->attrExtra;
            if(!attr)
                return false;
            if(attr->skills.size() > 0)
            {
                if(attr->skills[0])
                    trumps[i] = SKILLANDLEVEL(SKILL_ID(attr->skills[0]->getId()), SH_SY_SKILL_LEVEL);
            }
        }

        return true;
    }

    void SingleHeroFighter::saveStrategy(std::vector<UInt16>& trumps, std::vector<UInt16>& cittas)
    {
        if(!checkCittas(cittas))
            return;
        if(!checkTrumps(trumps))
            return;
        if(!_fgt)
            return;
        UInt32 now = TimeUtil::Now();
        if(now - _cd < SH_FIGHTER_STRATEGY_CD)
            return;

        _cd = now;

        size_t trump_cnt = trumps.size();
        if(trump_cnt > TRUMP_UPMAX)
            trump_cnt = TRUMP_UPMAX;
        size_t tidx = 0;
        int i = 0;
        std::vector<const GData::SkillBase*> trumps_skill;

        for(i = 0; i < TRUMP_UPMAX; ++ i)
        {
            _trump[i] = 0;
            trumps_skill.push_back(GData::skillManager[_trump[i]]);
        }
        offTrump( trumps_skill );
        trumps_skill.resize(0);
        for(tidx = 0; tidx < trump_cnt; ++ tidx)
        {
            trumps_skill.push_back(GData::skillManager[trumps[tidx]]);
            _trump[tidx] = trumps[tidx];
        }
        upTrump( trumps_skill );

        for(i = 0; i < CITTA_UPMAX; ++ i)
        {
            if(_citta[i])
                _fgt->offCitta(_citta[i], false, true, false);
            _citta[i] = 0;
        }
        size_t citta_cnt = cittas.size();
        if(citta_cnt > CITTA_UPMAX)
            citta_cnt = CITTA_UPMAX;
        size_t cidx = 0;
        for(cidx = 0; cidx < citta_cnt; ++ cidx)
        {
            upCitta( cittas[cidx], cidx );
            _citta[cidx] = _fgt->getUpCittaAndLevel(cidx);
        }

        UInt8 cls = _fgt->getClass();
        GData::AttrExtra attrExtraEquip = shsyTmpl._attrExtraEquip[cls];
        attrExtraEquip += *(_fgt->getAttrExtraEquip());

        setAttrExtraEquip(attrExtraEquip);

        showStrategy();
        writeDB();
    }

    void SingleHeroFighter::showStrategy()
    {
        if(!_player || !_fgt)
            return;

        UInt32 now = TimeUtil::Now();
		Stream st(REP::SINGLE_HERO);

        UInt32 leftTime = now - _cd;
        if(leftTime > SH_FIGHTER_STRATEGY_CD)
            leftTime = 0;
        else
            leftTime = SH_FIGHTER_STRATEGY_CD - leftTime;
        st << static_cast<UInt8>(0x04);
        st << static_cast<UInt32>(leftTime);

        UInt8 tCnt = getTrumpsNum();
        st << tCnt;
        UInt8 idx = 0;
        for(idx = 0; idx < tCnt; ++ idx)
        {
            st << _trump[idx];
        }

        UInt8 cCnt = getCittasNum();
        st << cCnt;
        for(idx = 0; idx < cCnt; ++ idx)
        {
            st << _citta[idx];
        }

        st << Stream::eos;

        _player->send(st);
    }

    UInt8 SingleHeroFighter::getCittasNum()
    {
        UInt8 c = 0;
        for (size_t i = 0; i < CITTA_UPMAX; ++i)
            if (_citta[i]) ++c;
        return c;
    }

    UInt8 SingleHeroFighter::getTrumpsNum()
    {
        UInt8 c = 0;
        for (size_t i = 0; i < TRUMP_UPMAX; ++i)
            if (_trump[i]) ++c;
        return c;
    }

    void SingleHeroFighter::saveTowerProgress()
    {
        DB1().PushUpdateData("UPDATE `sh_player` SET `tLevel`=%u, `tTurns`=%u, `tLastTurns`=%u WHERE `playerId`=%"I64_FMT"u", _tLevel, _tTurns, _tLastTurns, _player->getId());
    }
}


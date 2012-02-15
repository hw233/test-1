#include "Config.h"
#include "BattleFighter.h"
#include "GObject/Item.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "Common/URandom.h"
#include "Script/BattleFormula.h"
#include "GObject/Clan.h"
#include "GObject/GObjectManager.h"
#include "GObject/HeroMemo.h"

namespace Battle
{

BattleFighter::BattleFighter(Script::BattleFormula * bf, GObject::Fighter * f, UInt8 side, UInt8 pos):
	BattleObject(BattleObject::Char, side, pos),
	_strength(0), _agility(0), _physique(0), _intelligence(0), _will(0), _soul(0), _aura(0), _auraMax(0), _tough(0),
	_attack(0), _magatk(0), _defend(0), _magdef(0), _hitrate(0), _evade(0), _critical(0), _criticaldmg(0), _pierce(0), _counter(0), _magres(0),
    _atkreduce(0), _magatkreduce(0),
	_maxhp(f->getMaxHP()), _maxAction(0),
	_attackAdd(0), _magAtkAdd(0), _defAdd(0), _magDefAdd(0), _hitrateAdd(0), _evadeAdd(0), _criticalAdd(0), _criticalDmgAdd(0),
    _pierceAdd(0), _counterAdd(0), _magResAdd(0), _toughAdd(0),
	_maxhpAdd(0), _maxActionAdd(0),
    _atkAdd_last(0), _magAtkAdd_last(0), _defAdd_last(0), _magDefAdd_last(0), _hitrateAdd_last(0), _evadeAdd_last(0),
    _criticalAdd_last(0), _criticalDmgAdd_last(0), _pierceAdd_last(0), _counterAdd_last(0), _magResAdd_last(0), _toughAdd_last(0),
    _maxhpAdd_last(0), _maxActionAdd_last(0), _atkreduce_last(0), _magatkreduce_last(0), _formEffect(NULL), _formula(bf),
    _forgetLevel(0), _forgetRound(0), _flag(0), _poisonRound(0), _poisonLevel(0), _stunRound(0), _stunLevel(0),
     _confuseRound(0), _confuseLevel(0), _weakRound(0), _weakLevel(0), _immune(0),
    _evade100(0), _evade100_cd(0), _evade100_last(0), _evade100_base_cd(0), _evade100_base_last(0),
    _defend100(0), _defend100_cd(0), _defend100_last(0), _defend100_base_cd(0), _defend100_base_last(0),
	_attackAdd2(0), _magAtkAdd2(0), _defAdd2(0), _magDefAdd2(0), _hitrateAdd2(0), _evadeAdd2(0),
    _criticalAdd2(0), _criticalDmgAdd2(0), _pierceAdd2(0), _counterAdd2(0), _magResAdd2(0), _toughAdd2(0),
    _atkreduce2(0), _magatkreduce2(0),
	_maxhpAdd2(0), _maxActionAdd2(0)
{
    memset(_immuneLevel, 0, sizeof(_immuneLevel));
    memset(_immuneRound, 0, sizeof(_immuneRound));
	setFighter(f);
}

void BattleFighter::setFighter( GObject::Fighter * f )
{
	_fighter = f;

    _peerlessSkill.base = GData::skillManager[_fighter->getPeerlessAndLevel()];

    std::vector<UInt16> activeSkill;
    _fighter->getUpSkillAndLevel(activeSkill);

    size_t idx;
    size_t cnt = activeSkill.size();

    _activeSkill.clear();
    _activeSkillIdx = 0;
    _revival = false;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[activeSkill[idx]];
        skillItem.cd = 0;
        _activeSkill.insert(_activeSkill.end(), skillItem);
    }

    std::vector<UInt16>& passiveSkillPrvAtk100Id = _fighter->getPassiveSkillPreAtk100();
    std::vector<UInt16>& passiveSkillAftAtk100Id = _fighter->getPassiveSkillAftAtk100();
    std::vector<UInt16>& passiveSkillBeAtk100Id = _fighter->getPassiveSkillBeAtk100();
    std::vector<UInt16>& passiveSkillAftEvd100Id = _fighter->getPassiveSkillAftEvd100();
    std::vector<UInt16>& passiveSkillAftRes100Id = _fighter->getPassiveSkillAftRes100();
    std::vector<UInt16>& passiveSkillEnter100Id = _fighter->getPassiveSkillEnter100();
    std::vector<UInt16>& passiveSkillDead100Id = _fighter->getPassiveSkillDead100();
    std::vector<UInt16>& passiveSkillAftNAtk100Id = _fighter->getPassiveSkillAftNAtk100();

    std::vector<UInt16>& passiveSkillPreAtkId = _fighter->getPassiveSkillPreAtk();
    std::vector<UInt16>& passiveSkillAftAtkId = _fighter->getPassiveSkillAftAtk();
    std::vector<UInt16>& passiveSkillBeAtkId = _fighter->getPassiveSkillBeAtk();
    std::vector<UInt16>& passiveSkillAftEvdId = _fighter->getPassiveSkillAftEvd();
    std::vector<UInt16>& passiveSkillAftResId = _fighter->getPassiveSkillAftRes();
    std::vector<UInt16>& passiveSkillEnterId = _fighter->getPassiveSkillEnter();
    std::vector<UInt16>& passiveSkillDeadId = _fighter->getPassiveSkillDead();
    std::vector<UInt16>& passiveSkillAftNAtk = _fighter->getPassiveSkillAftNAtk();


    cnt = passiveSkillPrvAtk100Id.size();
    _passiveSkillPrvAtk100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillPrvAtk100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillPrvAtk100.insert(_passiveSkillPrvAtk100.end(), skillItem);
    }

    cnt = passiveSkillAftAtk100Id.size();
    _passiveSkillAftAtk100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillAftAtk100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillAftAtk100.insert(_passiveSkillAftAtk100.end(), skillItem);
    }

    cnt = passiveSkillBeAtk100Id.size();
    _passiveSkillBeAtk100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillBeAtk100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillBeAtk100.insert(_passiveSkillBeAtk100.end(), skillItem);
    }

    cnt = passiveSkillAftEvd100Id.size();
    _passiveSkillAftEvd100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillAftEvd100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillAftEvd100.insert(_passiveSkillAftEvd100.end(), skillItem);
    }

    cnt = passiveSkillAftRes100Id.size();
    _passiveSkillAftRes100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillAftRes100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillAftRes100.insert(_passiveSkillAftRes100.end(), skillItem);
    }

    cnt = passiveSkillEnter100Id.size();
    _passiveSkillEnter100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillEnter100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillEnter100.insert(_passiveSkillEnter100.end(), skillItem);
    }

    cnt = passiveSkillDead100Id.size();
    _passiveSkillDead100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillDead100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillDead100.insert(_passiveSkillDead100.end(), skillItem);
    }

    cnt = passiveSkillAftNAtk100Id.size();
    _passiveSkillAftNAtk100.clear();
    for(idx = 0; idx < cnt; ++ idx)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillAftNAtk100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillAftNAtk100.insert(_passiveSkillAftNAtk100.end(), skillItem);
    }

    cnt = passiveSkillPreAtkId.size();
    _passiveSkillPreAtk.clear();
    float rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillPreAtkId[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillPreAtk.insert(_passiveSkillPreAtk.end(), skillItem);
    }

    cnt = passiveSkillAftAtkId.size();
    _passiveSkillAftAtk.clear();
    rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillAftAtkId[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillAftAtk.insert(_passiveSkillAftAtk.end(), skillItem);
    }

    cnt = passiveSkillBeAtkId.size();
    _passiveSkillBeAtk.clear();
    rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillBeAtkId[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillBeAtk.insert(_passiveSkillBeAtk.end(), skillItem);
    }

    cnt = passiveSkillAftEvdId.size();
    _passiveSkillAftEvd.clear();
    rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillAftEvdId[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillAftEvd.insert(_passiveSkillAftEvd.end(), skillItem);
    }

    cnt = passiveSkillAftResId.size();
    _passiveSkillAftRes.clear();
    rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillAftResId[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillAftRes.insert(_passiveSkillAftRes.end(), skillItem);
    }

    cnt = passiveSkillEnterId.size();
    _passiveSkillEnter.clear();
    rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillEnterId[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillEnter.insert(_passiveSkillEnter.end(), skillItem);
    }

    cnt = passiveSkillDeadId.size();
    _passiveSkillDead.clear();
    rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillDeadId[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillDead.insert(_passiveSkillDead.end(), skillItem);
    }

    cnt = passiveSkillAftNAtk.size();
    _passiveSkillAftNAtk.clear();
    rateExtent = 0;
    for(idx = 0; idx < cnt; ++ idx)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillAftNAtk[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillAftNAtk.insert(_passiveSkillAftNAtk.end(), skillItem);
    }
}

void BattleFighter::updateAllAttr()
{
	updateBuffExtras();

	_strength = _formula->calcStrength(this);
	_agility = _formula->calcAgility(this);
	_physique = _formula->calcPhysique(this);
	_intelligence = _formula->calcIntelligence(this);
	_attack = _formula->calcAttack(this);
	_defend = _formula->calcDefend(this);
	_hitrate = _formula->calcHitrate(this, NULL);
	_evade = _formula->calcEvade(this, NULL);
	_critical = _formula->calcCritical(this, NULL);
	_pierce = _formula->calcPierce(this, NULL);
	_counter = _formula->calcCounter(this, NULL);
	_maxAction = _formula->calcAction(this);
	UInt32 oldhp = getMaxHP();
	_maxhp = _formula->calcHP(this);
    _criticaldmg = _formula->calcCriticalDmg(this);

    _aura = _formula->calcAura(this);
    _auraMax = _formula->calcAuraMax(this);
    _will = _formula->calcWill(this);
    _soul = _formula->calcSoul(this);
    _tough = _formula->calcTough(this, NULL);
    _magatk = _formula->calcMagAttack(this);
    _magdef = _formula->calcMagDefend(this);
    _magres = _formula->calcMagRes(this, NULL);

    if(_maxhp == 0)
        _maxhp = 1;

	if((_flag & BlockBoss) == 0 && oldhp > 0 && _hp > 0 && oldhp < getMaxHP())
	{
		_hp = _hp + getMaxHP() - oldhp;
	}
	if(_hp > getMaxHP())
	{
		_hp = getMaxHP();
	}
}

void BattleFighter::setAttrExtra(UInt8 klass, UInt8 career, UInt8 level)
{
	//float strength = 2.5f;
	//float physique = 6.5f; 
	//float agility = 2.5f;
	//float intelligence = 1.5f;
	//UInt32 attack = 25;
	//UInt32 defend = 30;
	//UInt32 maxhp = 150;
	//float action = 0.2f;
	//float hitrate = 0.15;
	//float evade = 0.25;
	//float critical = 0.15;
	//float pierce = 0.1;
	//float counter = 0.3;
	
	UInt8 i = 1;
	lua_tinker::table factors = _formula->getFactor(klass, career, level);
	_attrbylevel.strength = static_cast<UInt16>(factors.get<float>(i++));
	_attrbylevel.physique = static_cast<UInt16>(factors.get<float>(i ++));
	_attrbylevel.intelligence = static_cast<UInt16>(factors.get<float>(i ++));
	_attrbylevel.agility = static_cast<UInt16>(factors.get<float>(i ++));
	_attrbylevel.hitrate = factors.get<float>(i ++);
	_attrbylevel.evade = factors.get<float>(i ++);
	_attrbylevel.critical = factors.get<float>(i ++);
	_attrbylevel.pierce = factors.get<float>(i ++);
	_attrbylevel.counter = factors.get<float>(i ++);
	_attrbylevel.action = factors.get<float>(i ++);
	_attrbylevel.hp = factors.get<UInt16>(i ++);
	_attrbylevel.attack = factors.get<UInt16>(i ++);
	_attrbylevel.defend = factors.get<UInt16>(i ++);
}

inline void addAttrExtra( GData::AttrExtra& ae, const GData::AttrExtra * ext )
{
	if(ext == NULL)
		return;
	ae += *ext;
}

void BattleFighter::updateBuffExtras()
{
	_attrExtra.reset();
	if(_formEffect != NULL)
	{
		addAttrExtra(_attrExtra, _formEffect->attrExtra);
	}
	addAttrExtra(_attrExtra, _fighter->getAttrExtraEquip());
	UInt32 ext = (_flag & 0x03) + ((_flag >> 2) & 0x03);
	if(_flag & Enh3)
		ext += 2;
	if(_flag & BlockBoss)
	{
		addAttrExtra(_attrExtra, &_attrbylevel);
	}
	if(ext > 0)
	{
		float extAttr = 0.05f * ext;
		_attrExtra.strengthP += extAttr;
		_attrExtra.physiqueP += extAttr;
		_attrExtra.agilityP += extAttr;
		_attrExtra.intelligenceP += extAttr;
		_attrExtra.willP += extAttr;
	}
}

float BattleFighter::calcAttack( bool& isCritical, BattleFighter* defender, float* pCf)
{
	// 计算暴击率
    float rate = getCritical(defender);
    isCritical = uRand(10000) < (rate > 0 ? rate : 0) * 100;

	float atk = getAttack();
	/* TODO: random
	int extra = (uRand(9)) - 4;
	*/

    float factor = getCriticalDmg() - defender->getTough(this);
    if(factor < 1)
        factor = 1;

	// 如果暴击
	if(isCritical)
	{
		atk = atk * factor;
	}

    if(pCf)
        *pCf = factor;
	return atk;
}

float BattleFighter::calcMagAttack(bool& isCritical, BattleFighter* defender , float* pCf)
{
    float rate = getCritical(defender);
    isCritical = uRand(10000) < (rate > 0 ? rate : 0) * 100;
    float magatk = getMagAttack();

    float factor = getCriticalDmg() - defender->getTough(this);
    if(factor < 1)
        factor = 1;

    if(isCritical)
    {
        magatk = magatk * factor;
    }

    if(pCf)
        *pCf = factor;
    return magatk;
}

void BattleFighter::calcSkillAttack(bool& isCritical, BattleFighter* defender, float& atk, float& magatk, float* pCf)
{
    float rate = getCritical(defender);
    isCritical = uRand(10000) < (rate > 0 ? rate : 0) * 100;

    magatk = getMagAttack();
	atk = getAttack();

    float factor = getCriticalDmg() - defender->getTough(this);
    if(factor < 1)
        factor = 1;

    if(isCritical)
    {
        magatk = magatk * factor;
		atk = atk * factor;
    }

    if(pCf)
        *pCf = factor;
}

float BattleFighter::calcTherapy(const GData::SkillBase* skill)
{
    if(!skill)
        return 0;

    if(skill->effect == NULL)
        return 0;

    float aura_factor = 1;
    if(skill->cond == GData::SKILL_PEERLESS)
    {
        aura_factor = _aura / 100;
        _aura = 0;
    }

    return aura_factor * ((_magatk + _magAtkAdd + _magAtkAdd2) * skill->effect->hpP + skill->effect->addhp + skill->effect->hp);
}

float BattleFighter::calcPoison(const GData::SkillBase* skill, BattleFighter* defender, bool cs)
{
    if(!skill)
        return 0;

    if(skill->effect == NULL)
        return 0;

    float magatk = getMagAttack();
    float atk = getAttack();

    float factor = getCriticalDmg() - defender->getTough(this);
    if(factor < 1)
        factor = 1;

    if(!cs)
    {
        factor = 1;
    }

    // 道
    if(getClass() == 3)
    {
        return (atk * skill->effect->hpP + skill->effect->hp + skill->effect->addhp) * factor * (950 + uRand(100)) / 1000;
    }

    // 儒释
    return (magatk * skill->effect->hpP + skill->effect->hp + skill->effect->addhp) * factor *  (950 + uRand(100)) / 1000;
}

bool BattleFighter::calcHit( BattleFighter * defender )
{
	if(defender->getStunRound() > 0)
		return true;

	// 计算命中值
	float hitrate = getHitrate(defender) - defender->getEvade(this);

	// 必中
	if(hitrate >= 100)
		return true;

	// 计算命中率
	return uRand(10000) < hitrate * 100;
}

bool BattleFighter::calcCounter(BattleFighter* attacker, bool ranged)
{
	return uRand(ranged ? 20000 : 10000) < getCounter(attacker) * 100;
}

bool BattleFighter::canBeCounter()
{
	GObject::ItemWeapon * wp = _fighter->getWeapon();
	// TODO: no weapon_def return wp == NULL || wp->getWeaponDef().canCounter();
	return wp == NULL;
}

bool BattleFighter::calcPierce(BattleFighter* defender)
{
	return uRand(10000) < getPierce(defender) * 100;
}

void BattleFighter::initStats(bool checkEnh)
{
	if(_fighter != NULL)
	{
		if(checkEnh && (_fighter->getId() <= GREAT_FIGHTER_MAX || _fighter->getId() > NPC_FIGHTER_MAX) && _fighter->getOwner() != NULL)
		{
			UInt32 now = TimeUtil::Now();
			if(_fighter->getBuffData(FIGHTER_BUFF_ATTR3, now))
				_flag |= 3;
			else if(_fighter->getBuffData(FIGHTER_BUFF_ATTR2, now))
				_flag |= 2;
			else if(_fighter->getBuffData(FIGHTER_BUFF_ATTR1, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_CRMASGIRL, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_DRESS, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_WEDDING, now))
				_flag |= 1;
			_flag |= (_fighter->getOwner()->getBuffData(PLAYER_BUFF_HOLY, 0)) << 28;
		}
		if(_fighter->getId() > 4800 && _fighter->getId() < 5000)
		{
			_flag |= BlockBoss;
		}
		updateAllAttr();
		if(_hp == 0)
			_hp = _maxhp;
	}
}

UInt32 BattleFighter::regenHP( UInt32 u )
{
    if(_weakRound > 0)
        u /= 2;

	UInt32 oldhp = _hp;
	if(oldhp >= getMaxHP())
	{
		_hp = getMaxHP();
		return 0;
	}
	_hp += u;
	if(_hp > getMaxHP())
	{
		_hp = getMaxHP();
		return getMaxHP() - oldhp;
	}
	return u;
}

UInt16 BattleFighter::getPortrait()
{
//	if(_fighter->isNpc())
//		return _fighter->favor;
//	else
		return _fighter->getId();
}

void BattleFighter::postInit()
{
	UInt32 holy_level = _flag >> 28;
	if(holy_level)
	{
		switch(holy_level)
		{
		case 2:
			_attack += 100;
			_maxAction = _maxAction * 10 / 11;
			_hitrate += 10.0f;
			_evade += 12.0f;
			_counter += 10.0f;
			_critical += 5.0f;
			_pierce += 5.0f;
			break;
		case 4:
			_attack += 200;
			_maxAction = _maxAction * 10 / 11;
			_hitrate += 50.0f;
			_evade += 12.0f;
			_counter += 10.0f;
			_critical += 5.0f;
			_pierce += 5.0f;
			break;
		case 6:
			_attack += 300;
			_maxAction = _maxAction * 20 / 23;
			_hitrate += 100.0f;
			_evade += 20.0f;
			_counter += 10.0f;
			_critical += 10.0f;
			_pierce += 10.0f;
			break;
		}
	}
}

const GData::SkillBase* BattleFighter::getActiveSkill(bool need_therapy)
{
    GData::SkillItem* resSkillItem = NULL;
    if(NULL != _peerlessSkill.base)
    {
        if(_aura >= 100 && _peerlessSkill.base->effect != NULL)
        {
            // peerless skill first
            if (_fighter->getOwner())
                _fighter->getOwner()->OnHeroMemo(GObject::MC_SKILL, GObject::MD_ADVANCED, 0, 2);
            return _peerlessSkill.base;
        }
    }

    size_t cnt = _activeSkill.size();
    if(cnt == 0)
        return NULL;

    bool has_therapy = false;
    size_t idx0 = _activeSkillIdx % cnt;
    size_t idx = 0;
    for(size_t i = 0; i < cnt; ++i)
    {
        idx = (idx0 + i) % cnt;
        if(NULL != _activeSkill[idx].base && _activeSkill[idx].cd == 0)
        {
            if(_activeSkill[idx].base->effect == NULL)
                continue;
            bool isTherapy = (_activeSkill[idx].base->effect->hp > 0 || _activeSkill[idx].base->effect->hpP > 0.001) && _activeSkill[idx].base->target == 0;
            // therapy skill second while need therapy
            if(need_therapy && isTherapy)
            {
                if(resSkillItem)
                {
                    resSkillItem->cd = 0;
                }
                has_therapy = true;
                resSkillItem = &(_activeSkill[idx]);
                _activeSkill[idx].cd = resSkillItem->base->cd + 1;
                break;
            }

            if(!resSkillItem)
            {
                if(!need_therapy && isTherapy)
                {
                    continue;
                }
                resSkillItem = &(_activeSkill[idx]);
                _activeSkill[idx].cd = resSkillItem->base->cd + 1;

                if(!need_therapy)
                    break;
            }
        }
    }

    if(resSkillItem)
    {
        if(!has_therapy)
            _activeSkillIdx  = idx + 1;

        return resSkillItem->base;
    }

    return NULL;
}

const GData::SkillBase* BattleFighter::getPassiveSkill100(std::vector<GData::SkillItem>& passiveSkill100, size_t& idx)
{
    size_t cnt = passiveSkill100.size();
    for(; idx < cnt; idx++)
    {
        if(passiveSkill100[idx].cd == 0)
        {
            if(passiveSkill100[idx].base == NULL)
                continue;
            if(passiveSkill100[idx].base->effect == NULL)
                continue;
            passiveSkill100[idx].cd = passiveSkill100[idx].base->cd + 1;
            if (_fighter->getOwner())
                _fighter->getOwner()->OnHeroMemo(GObject::MC_SKILL, GObject::MD_MASTER, 0, 1);
            return passiveSkill100[idx++].base;
        }
    }

    return NULL;
}

const GData::SkillBase* BattleFighter::getPassiveSkillPrvAtk100(size_t& idx)
{
    return getPassiveSkill100(_passiveSkillPrvAtk100, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftAtk100(size_t& idx)
{
    return getPassiveSkill100(_passiveSkillAftAtk100, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillBeAtk100(size_t& idx)
{
    return getPassiveSkill100(_passiveSkillBeAtk100, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftEvd100(size_t& idx)
{
    return getPassiveSkill100(_passiveSkillAftEvd100, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftRes100(size_t& idx)
{
    return getPassiveSkill100(_passiveSkillAftRes100, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillEnter100(size_t& idx)
{
    return getPassiveSkill100(_passiveSkillEnter100, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillDead100(size_t& idx)
{
    return getPassiveSkill100(_passiveSkillDead100, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftNAtk100(size_t& idx)
{
    return getPassiveSkill100(_passiveSkillAftNAtk100, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkill(std::vector<GData::SkillItem>& passiveSkill)
{
    size_t cnt = passiveSkill.size();
    if(cnt < 1)
        return NULL;
    URandom rand;
    UInt32 rnd = rand(cnt*100*100);
    const GData::SkillBase* resSkillBase = NULL;

    for(size_t idx = 0; idx < cnt; ++idx)
    {
        if(rnd < passiveSkill[idx].rateExtent && passiveSkill[idx].cd == 0)
        {
            if(passiveSkill[idx].base == NULL)
                continue;
            if(passiveSkill[idx].base->effect == NULL)
                continue;
            passiveSkill[idx].cd = passiveSkill[idx].base->cd + 1;
            resSkillBase = passiveSkill[idx].base;
            if (_fighter->getOwner())
                _fighter->getOwner()->OnHeroMemo(GObject::MC_SKILL, GObject::MD_MASTER, 0, 1);
            break;
        }
    }
 
    return resSkillBase;
}

const GData::SkillBase* BattleFighter::getPassiveSkillPreAtk()
{
    return getPassiveSkill(_passiveSkillPreAtk);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftAtk()
{
    return getPassiveSkill(_passiveSkillAftAtk);
}

const GData::SkillBase* BattleFighter::getPassiveSkillBeAtk()
{
    return getPassiveSkill(_passiveSkillBeAtk);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftEvd()
{
    return getPassiveSkill(_passiveSkillAftEvd);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftRes()
{
    return getPassiveSkill(_passiveSkillAftRes);
}

const GData::SkillBase* BattleFighter::getPassiveSkillEnter()
{
    return getPassiveSkill(_passiveSkillEnter);
}

const GData::SkillBase* BattleFighter::getPassiveSkillDead()
{
    return getPassiveSkill(_passiveSkillDead);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftNAtk()
{
    return getPassiveSkill(_passiveSkillAftNAtk);
}

void BattleFighter::releaseSkillCD(std::vector<GData::SkillItem>& skill, int cd)
{
    size_t cnt = skill.size();
    size_t idx;
    for(idx = 0; idx < cnt; idx++)
    {
        if(skill[idx].cd > 0)
        {
            skill[idx].cd -= cd;
        }
    }
}

void BattleFighter::releaseSkillCD(int cd)
{
    releaseSkillCD(_activeSkill, cd);

    releaseSkillCD(_passiveSkillPrvAtk100, cd);
    releaseSkillCD(_passiveSkillAftAtk100, cd);
    releaseSkillCD(_passiveSkillBeAtk100, cd);
    releaseSkillCD(_passiveSkillAftEvd100, cd);
    releaseSkillCD(_passiveSkillAftRes100, cd);
    releaseSkillCD(_passiveSkillEnter100, cd);
    releaseSkillCD(_passiveSkillDead100, cd);
    releaseSkillCD(_passiveSkillAftNAtk100, cd);

    releaseSkillCD(_passiveSkillPreAtk, cd);
    releaseSkillCD(_passiveSkillAftAtk, cd);
    releaseSkillCD(_passiveSkillBeAtk, cd);
    releaseSkillCD(_passiveSkillAftEvd, cd);
    releaseSkillCD(_passiveSkillAftRes, cd);
    releaseSkillCD(_passiveSkillEnter, cd);
    releaseSkillCD(_passiveSkillDead, cd);
    releaseSkillCD(_passiveSkillAftNAtk, cd);
}

float BattleFighter::getHitrate(BattleFighter* defgt)
{
    float hiterate = 0;
    if(defgt == NULL)
        hiterate = _hitrate + _hitrateAdd + _hitrateAdd2;
    else
        hiterate = _formula->calcHitrate(this, defgt) + _hitrateAdd + _hitrateAdd2;

    if(hiterate > GObject::GObjectManager::getHiterateMax() && !isNpc())
        hiterate = GObject::GObjectManager::getHiterateMax();

    if(hiterate < 0)
        return 0;

    return hiterate;
}

float BattleFighter::getEvade(BattleFighter* defgt)
{
    float evade = 0;
    if(defgt == NULL)
        evade = _evade + _evadeAdd + _evadeAdd2;
    else
        evade = _formula->calcEvade(this, defgt) + _evadeAdd + _evadeAdd2;

    if(evade > GObject::GObjectManager::getEvadeMax() && !isNpc())
        evade = GObject::GObjectManager::getEvadeMax();

    if(evade < 0)
        return 0;

    return evade;
}

float BattleFighter::getCritical(BattleFighter* defgt)
{
    float critical = 0;
    if(defgt == NULL)
        critical = _critical + _criticalAdd + _criticalAdd2;
    else
        critical = _formula->calcCritical(this, defgt) + _criticalAdd + _criticalAdd2;

    if(critical > GObject::GObjectManager::getCriticalMax() && !isNpc())
        critical = GObject::GObjectManager::getCriticalMax();

    if(critical < 0)
        return 0;

    return critical;
}

float BattleFighter::getPierce(BattleFighter* defgt)
{
    float pierce = 0;
    if(defgt == NULL)
        pierce = _pierce + _pierceAdd + _pierceAdd2;
    else
        pierce = _formula->calcPierce(this, defgt) + _pierceAdd + _pierceAdd2;

    if(pierce > GObject::GObjectManager::getPierceMax() && !isNpc())
        pierce = GObject::GObjectManager::getPierceMax();

    if(pierce < 0)
        return 0;

    return pierce;
}

float BattleFighter::getCounter(BattleFighter* defgt)
{
    float counter = 0;
    if(defgt == NULL)
        counter = _counter + _counterAdd + _counterAdd2;
    else
        counter = _formula->calcCounter(this, defgt) + _counterAdd + _counterAdd2;

    if(counter > GObject::GObjectManager::getCounterMax() && !isNpc())
        counter = GObject::GObjectManager::getCounterMax();

    if(counter < 0)
        return 0;

    return counter;
}

float BattleFighter::getMagRes(BattleFighter* defgt)
{
    float magres = 0;
    if(defgt == NULL)
        magres = _magres + _magResAdd + _magResAdd2;
    else
        magres = _formula->calcMagRes(this, defgt) + _magResAdd + _magResAdd2;

    if(magres > GObject::GObjectManager::getMagResMax() && !isNpc())
        magres = GObject::GObjectManager::getMagResMax();

    if(magres < 0)
        return 0;

    return magres;
}

float BattleFighter::calcCriticalDmg(BattleFighter* defender)
{
    float factor = getCriticalDmg() - defender->getTough(this);
    if(factor < 1)
        factor = 1;

    return factor;
}

float BattleFighter::getTough(BattleFighter* defgt)
{
    float tough = 0;
    if(defgt == NULL)
        tough = _tough + _toughAdd + _toughAdd2;
    else
        tough = _formula->calcTough(this, defgt) + _toughAdd + _toughAdd2;

    if(tough > GObject::GObjectManager::getToughMax() && !isNpc())
        tough = GObject::GObjectManager::getToughMax();

    if(tough < 0)
        return 0;

    return tough/100.0f;
}

void BattleFighter::setImmuneLevel(UInt8 state, UInt8 f)
{
    if(state == 0)
        return;

    for(Int8 idx = 0; idx < 8;++idx)
    {
        if(state & (1 << idx))
        {
            _immuneLevel[idx] = f;
        }
    }
}

UInt8 BattleFighter::getImmuneLevel(UInt8 state)
{
    if(state == 0)
        return 0;

    Int8 idx = 0;
    for(;;++idx)
    {
        if(state & (1 << idx))
        {
            break;
        }
    }

    if(idx > 7)
        return 0;

    return _immuneLevel[idx];
}

void BattleFighter::setImmuneRound(UInt8 state, UInt8 f)
{
    if(state == 0)
        return;

    for(Int8 idx = 0; idx < 8; ++idx)
    {
        if(state & (1 << idx))
        {
            _immuneRound[idx] = f;
        }
    }
}

UInt8 BattleFighter::getImmuneRound(UInt8 state)
{
    if(state == 0)
        return 0;

    Int8 idx = 0;
    for(;;++idx)
    {
        if(state & (1 << idx))
        {
            break;
        }
    }

    if(idx > 7)
        return 0;

    return _immuneRound[idx];
}

}

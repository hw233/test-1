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
    _forgetLevel(0), _forgetRound(0), _flag(0),_flag2(0),  _poisonRound(0), _poisonLevel(0), _stunRound(0), _stunLevel(0),
     _confuseRound(0), _confuseLevel(0), _weakRound(0), _weakLevel(0), _immune(0),
    _evade100(0), _evade100_cd(0), _evade100_last(0), _evade100_base_cd(0), _evade100_base_last(0),
    _defend100(0), _defend100_cd(0), _defend100_last(0), _defend100_base_cd(0), _defend100_base_last(0),
	_attackAdd2(0), _magAtkAdd2(0), _defAdd2(0), _magDefAdd2(0), _hitrateAdd2(0), _evadeAdd2(0),
    _criticalAdd2(0), _criticalDmgAdd2(0), _pierceAdd2(0), _counterAdd2(0), _magResAdd2(0), _toughAdd2(0),
    _atkreduce2(0), _magatkreduce2(0),
	_maxhpAdd2(0), _maxActionAdd2(0), _fakeDeadTimes(0),
    _atkreduce3(0), _magatkreduce3(0), _pudu_debuf(0),
    _atkreduce3_last(0), _magatkreduce3_last(0), _pudu_debuf_last(0),
    _deep_forget_dmg_extra(0), _deep_forget_last(0), _deep_stun_dmg_extra(0), _deep_stun_last(0),
    _therapy_dec(0), _therapy_dec_last(0),_therapy_add(0),_therapy_add_last(0),
    _aura_dec(0), _aura_dec_last(0),
    _bleed1(0), _bleed2(0), _bleed3(0),
    _bleed1_last(0), _bleed2_last(0), _bleed3_last(0),
    _immune2(0), _def_dec(0), _def_dec_last(0), _def_dec_times(0),

    _aura_bleed(0), _aura_dec_cd(0), _aura_bleed_last(0),
    _stun_bleed(0), _stun_cd(0), _stun_bleed_last(0),
    _confuse_bleed(0), _confuse_cd(0), _confuse_bleed_last(0),
    _colorStock(0), _colorStockTimes(0), _colorStockLast(0),
    _atkAddSpecial(0), _atkSpecialLast(0), _magAtkAddSpecial(0), _magAtkSpecialLast(0), _hitChangeByPeerless(0),
    _bleedRandom(0), _bleedRandomLast(0), _bleedAttackClass(1), _counterChangeByPeerless(0),_bSingleAttackFlag(false)
{
    memset(_immuneLevel, 0, sizeof(_immuneLevel));
    memset(_immuneRound, 0, sizeof(_immuneRound));
	setFighter(f);
}

void BattleFighter::setFighter( GObject::Fighter * f )
{
	_fighter = f;

    _peerlessSkill.base = GData::skillManager[_fighter->getPeerlessAndLevel()];
    // reg skillstrenghten
    updateSkillStrengthen(_fighter->getPeerlessAndLevel());

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

        updateSkillStrengthen(activeSkill[idx]);
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

    std::vector<UInt16>& passiveSkillOnTherapy = _fighter->getPassiveSkillOnTherapy();
    std::vector<UInt16>& passiveSkillOnSkillDmg = _fighter->getPassiveSkillOnSkillDmg();
    std::vector<UInt16>& passiveSkillOnOtherDead = _fighter->getPassiveSkillOnOtherDead();

    cnt = passiveSkillPrvAtk100Id.size();
    _passiveSkillPrvAtk100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillPrvAtk100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillPrvAtk100.insert(_passiveSkillPrvAtk100.end(), skillItem);

        updateSkillStrengthen(passiveSkillPrvAtk100Id[idx]);
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

        updateSkillStrengthen(passiveSkillAftAtk100Id[idx]);
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

        updateSkillStrengthen(passiveSkillBeAtk100Id[idx]);
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

        updateSkillStrengthen(passiveSkillAftEvd100Id[idx]);
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

        updateSkillStrengthen(passiveSkillAftRes100Id[idx]);
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

        updateSkillStrengthen(passiveSkillEnter100Id[idx]);
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

        updateSkillStrengthen(passiveSkillDead100Id[idx]);
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

        updateSkillStrengthen(passiveSkillAftNAtk100Id[idx]);
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

        updateSkillStrengthen(passiveSkillPreAtkId[idx]);
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

        updateSkillStrengthen(passiveSkillAftAtkId[idx]);
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

        updateSkillStrengthen(passiveSkillBeAtkId[idx]);
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

        updateSkillStrengthen(passiveSkillAftEvdId[idx]);
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

        updateSkillStrengthen(passiveSkillAftResId[idx]);
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

        updateSkillStrengthen(passiveSkillEnterId[idx]);
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

        updateSkillStrengthen(passiveSkillDeadId[idx]);
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

        updateSkillStrengthen(passiveSkillAftNAtk[idx]);
    }

    cnt = passiveSkillOnTherapy.size();
    _passiveSkillOnTherapy.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnTherapy[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnTherapy.insert(_passiveSkillOnTherapy.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnTherapy[idx]);
    }

    cnt = passiveSkillOnSkillDmg.size();
    _passiveSkillOnSkillDmg.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnSkillDmg[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnSkillDmg.insert(_passiveSkillOnSkillDmg.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnSkillDmg[idx]);
    }

    cnt = passiveSkillOnOtherDead.size();
    _passiveSkillOnOtherDead.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnOtherDead[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnOtherDead.insert(_passiveSkillOnOtherDead.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnOtherDead[idx]);
    }
}

void BattleFighter::updateAllAttr()
{
	updateBuffExtras();
    float factor = 1.0f;
    if(_fighter && _fighter->getOwner())
        factor = _fighter->getOwner()->getSpiritFactor();

	_strength = _formula->calcStrength(this);
	_agility = _formula->calcAgility(this);
	_physique = _formula->calcPhysique(this);
	_intelligence = _formula->calcIntelligence(this);
	_attack = _formula->calcAttack(this) * factor;
	_defend = _formula->calcDefend(this) * factor;
	_hitrate = _formula->calcHitrate(this, NULL) + factor - 1.0f;
	_evade = _formula->calcEvade(this, NULL) + factor - 1.0f;
	_critical = _formula->calcCritical(this, NULL) + factor - 1.0f;
	_pierce = _formula->calcPierce(this, NULL) + factor - 1.0f;
	_counter = _formula->calcCounter(this, NULL) + factor - 1.0f;
	_maxAction = _formula->calcAction(this) * factor;
	UInt32 oldhp = getMaxHP();
	_maxhp = _formula->calcHP(this) * factor;
    _criticaldmg = _formula->calcCriticalDmg(this);

    _aura = _formula->calcAura(this);
    _auraMax = _formula->calcAuraMax(this);
    _will = _formula->calcWill(this);
    _soul = _formula->calcSoul(this);
    _tough = _formula->calcTough(this, NULL) + factor - 1.0f;
    _magatk = _formula->calcMagAttack(this) * factor;
    _magdef = _formula->calcMagDefend(this) * factor;
    _magres = _formula->calcMagRes(this, NULL) + factor - 1.0f;

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

    if(_fighter && _fighter->getOwner())
    {
        const GData::AttrExtra* ae = _fighter->getOwner()->getHIAttr();
        if (ae)
            addAttrExtra(_attrExtra, ae);
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
    if (_flag & Enh4)
    {
        _attrExtra.attack += 50;
        _attrExtra.magatk += 50;
        _attrExtra.defend += 100;
        _attrExtra.magdef += 100;
    }
    if (_flag & Enh5)
    {
        _attrExtra.attack += 50;
        _attrExtra.magatk += 50;
        _attrExtra.defend += 100;
        _attrExtra.magdef += 100;
    }

    if (_flag & Enh6)
    {
        _attrExtra.attack += 50;
        _attrExtra.magatk += 50;
        _attrExtra.defend += 100;
        _attrExtra.magdef += 100;
    }

    if (_flag2 & AthlEnh1)
    {
        _attrExtra.attack *= 1.05f;
        _attrExtra.magatk *= 1.05f;
    }
    if (_flag2 & AthlEnh2)
    {
        _attrExtra.defend *= 1.05f;
        _attrExtra.magdef += 1.05f;
    }
    if (_flag2 & AthlEnh3)
    {
        _attrExtra.pierce *= 1.05f;
    }
    if (_flag2 & AthlEnh4)
    {
        _attrExtra.critical *= 1.05f;
    }
    if (_flag2 & AthlEnh5)
    {
        _attrExtra.evade *= 1.05f;
    }
    if (_flag2 & AthlEnh6)
    {
        _attrExtra.counter *= 1.05f;
    }
    if (_flag2 & AthlEnh7)
    {
        _attrExtra.physique *= 1.05f;
    }
    if (_flag2 & AthlEnh8)
    {
        _attrExtra.magres *= 1.05f;
    }
    if (_flag2 & AthlEnh9)
    {
        _attrExtra.hitrate *= 1.05f;
    }

    if (_flag2 & AthlEnh31)
    {
        _attrExtra.pierce *= 1.1f;
        _attrExtra.attack *= 1.1f;
        _attrExtra.magatk *= 1.1f;
    }
    if (_flag2 & AthlEnh32)
    {
        _attrExtra.criticaldmg *= 1.25f;
        _attrExtra.critical += 1.1f;
    }
    if (_flag2 & AthlEnh33)
    {
        _attrExtra.magres *= 1.2f;
    }
    if (_flag2 & AthlEnh34)
    {
        _attrExtra.counter *= 1.1f;
        _attrExtra.hitrate *= 1.1f;
    }
    if (_flag2 & AthlEnh35)
    {
        _attrExtra.evade *= 1.1f;
        _attrExtra.action *= 1.1f;
    }
    if (_flag2 & AthlEnh36)
    {
        _attrExtra.physique *= 1.1f;
        _attrExtra.defend *= 1.1f;
        _attrExtra.magdef *= 1.1f;
    }
    if (_flag2 & AthlEnh37)
    {
        _attrExtra.aura += 100;
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

float BattleFighter::calcTherapy(bool& isCritical, bool& first, const GData::SkillBase* skill)
{
    if(!skill)
        return 0;

    if(skill->effect == NULL)
        return 0;

    float aura_factor = 1;
    if(skill->cond == GData::SKILL_PEERLESS)
    {
        aura_factor = _aura / 100;
       // _aura = 0;  //  set函数统一处理，不在这里修改灵气了 
    }

    GData::SkillStrengthenBase* ss = getSkillStrengthen(SKILL_ID(skill->getId()));
    const GData::SkillStrengthenEffect* ef = NULL;
    if(ss)
        ef = ss->getEffect(GData::ON_THERAPY, GData::TYPE_CRITICAL);
    if(ef)
    {
        if(uRand(10000) < getCritical(NULL)* 100)
        {
            float f = (getCriticalDmg()-1)*ef->value/100 + 1;
            aura_factor *= f;
            if(first)
            {
                isCritical = true;
                first = false;
            }
        }
    }

    return aura_factor * (getMagAttack() * skill->effect->hpP + skill->effect->addhp + skill->effect->hp);
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
                    _fighter->getBuffData(FIGHTER_BUFF_WEDDING, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_RMAN, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_RWMAN, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_DMAN, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_DWMAN, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_SMAN, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_SWMAN, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_RDIAMOND, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_QQVIP, now))
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

UInt32 BattleFighter::regenHP( UInt32 u, bool weak )
{
    if(_weakRound > 0 && weak)
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

const GData::SkillBase* BattleFighter::getPassiveSkillOnTherapy()
{
    size_t idx = 0;
    return getPassiveSkill100(_passiveSkillOnTherapy, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnSkillDmg()
{
    size_t idx = 0;
    return getPassiveSkill100(_passiveSkillOnSkillDmg, idx);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnOtherDead()
{
    size_t idx = 0;
    return getPassiveSkill100(_passiveSkillOnOtherDead, idx);
}

float BattleFighter::getHitrate(BattleFighter* defgt)
{
    float hiterate = 0;
    if(defgt == NULL)
        hiterate = _hitrate + _hitrateAdd + _hitrateAdd2 + _hitChangeByPeerless;
    else
        hiterate = _formula->calcHitrate(this, defgt) + _hitrateAdd + _hitrateAdd2 + _hitChangeByPeerless;

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
        counter = _counter + _counterAdd + _counterAdd2 + _counterChangeByPeerless;
    else
        counter = _formula->calcCounter(this, defgt) + _counterAdd + _counterAdd2 + _counterChangeByPeerless;

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

float BattleFighter::getCriticalDmg()
{
    float cridmg = _criticaldmg + _criticalDmgAdd + _criticalDmgAdd2;
    if(cridmg > GObject::GObjectManager::getCriticalDmgMax() && !isNpc())
        cridmg = GObject::GObjectManager::getCriticalDmgMax();

    if(cridmg < 0)
        cridmg = 0;

    return cridmg;
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

void BattleFighter::fakeDead()
{
    ++_fakeDeadTimes;
    _hp = 1;
}

GData::SkillStrengthenBase* BattleFighter::getSkillStrengthen(UInt16 skillId)
{
    std::map<UInt16, GData::SkillStrengthenBase*>::iterator it = _skillStrengthen.find(skillId);
    if(it != _skillStrengthen.end())
        return it->second;

    return NULL;
}

void BattleFighter::updateSkillStrengthen(UInt16 skillId)
{
    UInt8 lvl = _fighter->SSGetLvl(skillId);
    if(lvl)
    {
        UInt16 skill_id = SKILL_ID(skillId);
        UInt16 ssId = SKILLANDLEVEL(skill_id, lvl);
        _skillStrengthen[skill_id] = (GData::SkillStrengthenBase*)(GData::skillStrengthenManager[ssId]);
    }
}

void BattleFighter::setAttackAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_attackAdd < 0.001f)
            || ( _attackAdd > 0 && v < 0)
            || ( _attackAdd < 0 && v > 0)
            || ((_attackAdd > 0) && (v > 0) && (_attackAdd < v))
            || ((_attackAdd < 0) && (v < 0) && (_attackAdd > v)))
    {
        _attackAdd = v;
        _atkAdd_last = last;
    }
}

void BattleFighter::setMagAttackAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_magAtkAdd < 0.001f)
            || ( _magAtkAdd > 0 && v < 0)
            || ( _magAtkAdd < 0 && v > 0)
            || ((_magAtkAdd > 0) && (v > 0) && (_magAtkAdd < v))
            || ((_magAtkAdd < 0) && (v < 0) && (_magAtkAdd > v)))
    {
        _magAtkAdd = v;
        _magAtkAdd_last = last;
    }
}

void BattleFighter::setDefendAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_defAdd < 0.001f)
            || ( _defAdd > 0 && v < 0)
            || ( _defAdd < 0 && v > 0)
            || ((_defAdd > 0) && (v > 0) && (_defAdd < v))
            || ((_defAdd < 0) && (v < 0) && (_defAdd > v)))
    {
        _defAdd = v;
        _defAdd_last = last;
    }
}

void BattleFighter::setMagDefendAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_magDefAdd < 0.001f)
            || ( _magDefAdd > 0 && v < 0)
            || ( _magDefAdd < 0 && v > 0)
            || ((_magDefAdd > 0) && (v > 0) && (_magDefAdd < v))
            || ((_magDefAdd < 0) && (v < 0) && (_magDefAdd > v)))
    {
        _magDefAdd = v;
        _magDefAdd_last = last;
    }
}

void BattleFighter::setHitrateAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_hitrateAdd < 0.001f)
            || ( _hitrateAdd > 0 && v < 0)
            || ( _hitrateAdd < 0 && v > 0)
            || ((_hitrateAdd > 0) && (v > 0) && (_hitrateAdd < v))
            || ((_hitrateAdd < 0) && (v < 0) && (_hitrateAdd > v)))
    {
        _hitrateAdd = v;
        _hitrateAdd_last = last;
    }
}

void BattleFighter::setEvadeAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_evadeAdd < 0.001f)
            || ( _evadeAdd > 0 && v < 0)
            || ( _evadeAdd < 0 && v > 0)
            || ((_evadeAdd > 0) && (v > 0) && (_evadeAdd < v))
            || ((_evadeAdd < 0) && (v < 0) && (_evadeAdd > v)))
    {
        _evadeAdd = v;
        _evadeAdd_last = last;
    }
}

void BattleFighter::setCriticalAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_criticalAdd < 0.001f)
            || ( _criticalAdd > 0 && v < 0)
            || ( _criticalAdd < 0 && v > 0)
            || ((_criticalAdd > 0) && (v > 0) && (_criticalAdd < v))
            || ((_criticalAdd < 0) && (v < 0) && (_criticalAdd > v)))
    {
        _criticalAdd = v;
        _criticalAdd_last = last;
    }
}

void BattleFighter::setCriticalDmgAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_criticalDmgAdd < 0.001f)
            || ( _criticalDmgAdd > 0 && v < 0)
            || ( _criticalDmgAdd < 0 && v > 0)
            || ((_criticalDmgAdd > 0) && (v > 0) && (_criticalDmgAdd < v))
            || ((_criticalDmgAdd < 0) && (v < 0) && (_criticalDmgAdd > v)))
    {
        _criticalDmgAdd = v;
        _criticalDmgAdd_last = last;
    }
}

void BattleFighter::setPierceAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_pierceAdd < 0.001f)
            || ( _pierceAdd > 0 && v < 0)
            || ( _pierceAdd < 0 && v > 0)
            || ((_pierceAdd > 0) && (v > 0) && (_pierceAdd < v))
            || ((_pierceAdd < 0) && (v < 0) && (_pierceAdd > v)))
    {
        _pierceAdd = v;
        _pierceAdd_last = last;
    }
}

void BattleFighter::setCounterAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_counterAdd < 0.001f)
            || ( _counterAdd > 0 && v < 0)
            || ( _counterAdd < 0 && v > 0)
            || ((_counterAdd > 0) && (v > 0) && (_counterAdd < v))
            || ((_counterAdd < 0) && (v < 0) && (_counterAdd > v)))
    {
        _counterAdd = v;
        _counterAdd_last = last;
    }
}

void BattleFighter::setMagResAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_magResAdd < 0.001f)
            || ( _magResAdd > 0 && v < 0)
            || ( _magResAdd < 0 && v > 0)
            || ((_magResAdd > 0) && (v > 0) && (_magResAdd < v))
            || ((_magResAdd < 0) && (v < 0) && (_magResAdd > v)))
    {
        _magResAdd = v;
        _magResAdd_last = last;
    }
}

void BattleFighter::setMaxHPAdd(Int32 v, UInt16 last)
{
    if((v == 0) || (_maxhpAdd == 0)
            || ( _maxhpAdd > 0 && v < 0)
            || ( _maxhpAdd < 0 && v > 0)
            || ((_maxhpAdd > 0) && (v > 0) && (_maxhpAdd < v))
            || ((_maxhpAdd < 0) && (v < 0) && (_maxhpAdd > v)))
    {
        _maxhpAdd = v;
        _maxhpAdd_last = last;
    }
}

void BattleFighter::setActionAdd(Int32 v, UInt16 last)
{
    if((v == 0) || (_maxActionAdd == 0)
            || ( _maxActionAdd > 0 && v < 0)
            || ( _maxActionAdd < 0 && v > 0)
            || ((_maxActionAdd > 0) && (v > 0) && (_maxActionAdd < v))
            || ((_maxActionAdd < 0) && (v < 0) && (_maxActionAdd > v)))
    {
        _maxActionAdd = v;
        _maxActionAdd_last = last;
    }
}

void BattleFighter::setToughAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_toughAdd < 0.001f)
            || ( _toughAdd > 0 && v < 0)
            || ( _toughAdd < 0 && v > 0)
            || ((_toughAdd > 0) && (v > 0) && (_toughAdd < v))
            || ((_toughAdd < 0) && (v < 0) && (_toughAdd > v)))
    {
        _toughAdd = v;
        _toughAdd_last = last;
    }
}

void BattleFighter::setAtkReduce(float v, UInt16 last)
{
    if((v < 0.001f) || (_atkreduce < 0.001f)
            || ( _atkreduce > 0 && v < 0)
            || ( _atkreduce < 0 && v > 0)
            || ((_atkreduce > 0) && (v > 0) && (_atkreduce < v))
            || ((_atkreduce < 0) && (v < 0) && (_atkreduce > v)))
    {
        _atkreduce = v;
        _atkreduce_last = last;
    }
}

void BattleFighter::setMagAtkReduce(float v, UInt16 last)
{
    if((v < 0.001f) || (_magatkreduce < 0.001f)
            || ( _magatkreduce > 0 && v < 0)
            || ( _magatkreduce < 0 && v > 0)
            || ((_magatkreduce > 0) && (v > 0) && (_magatkreduce < v))
            || ((_magatkreduce < 0) && (v < 0) && (_magatkreduce > v)))
    {
        _magatkreduce = v;
        _magatkreduce_last = last;
    }
}

void BattleFighter::setAtkReduce3(float v, UInt16 last)
{
    if((v < 0.001f) || (_atkreduce3 < 0.001f)
            || ( _atkreduce3 > 0 && v < 0)
            || ( _atkreduce3 < 0 && v > 0)
            || ((_atkreduce3 > 0) && (v > 0) && (_atkreduce3 < v))
            || ((_atkreduce3 < 0) && (v < 0) && (_atkreduce3 > v)))
    {
        _atkreduce3 = v;
        _atkreduce3_last = last;
    }
}

void BattleFighter::setMagAtkReduce3(float v, UInt16 last)
{
    if((v < 0.001f) || (_magatkreduce3 < 0.001f)
            || ( _magatkreduce3 > 0 && v < 0)
            || ( _magatkreduce3 < 0 && v > 0)
            || ((_magatkreduce3 > 0) && (v > 0) && (_magatkreduce3 < v))
            || ((_magatkreduce3 < 0) && (v < 0) && (_magatkreduce3 > v)))
    {
        _magatkreduce3 = v;
        _magatkreduce3_last = last;
    }
}

void BattleFighter::setPuduDebuf(float v, UInt16 last)
{
    if((v < 0.001f) || (_pudu_debuf < 0.001f)
            || ( _pudu_debuf > 0 && v < 0)
            || ( _pudu_debuf < 0 && v > 0)
            || ((_pudu_debuf > 0) && (v > 0) && (_pudu_debuf < v))
            || ((_pudu_debuf < 0) && (v < 0) && (_pudu_debuf > v)))
    {
        _pudu_debuf = v;
        _pudu_debuf_last = last;
    }
}
 

void BattleFighter::makeDamage( UInt32& u )
{
    if(_colorStockTimes > 0)
    {
        u = 0;
        -- _colorStockTimes;
        return;
    }

	if(_hp < u)
		_hp = 0;
	else
		_hp -= u;
}


}

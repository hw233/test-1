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
	_attackAdd(0), _magAtkAdd(0), _defAdd(0), _magDefAdd(0), _hitrateAdd(0), _evadeAdd(0), 
    _attackRoundAdd(0), _attackRoundSub(0), _magAtkRoundAdd(0), _defRoundAdd(0), _magDefRoundAdd(0),
    _defendChangeSS(0), _magDefendChangeSS(0),
    _criticalAdd(0), _criticalDmgAdd(0),
    _pierceAdd(0), _counterAdd(0), _magResAdd(0), _toughAdd(0),
    _pierceRoundAdd(0),
	_maxhpAdd(0), _maxActionAdd(0),
    _atkAdd_last(0), _magAtkAdd_last(0), _defAdd_last(0), _magDefAdd_last(0), _hitrateAdd_last(0), _evadeAdd_last(0),
    _attackRoundAddLast(0), _attackRoundSubLast(0), _magAtkRoundAddLast(0), _defRoundAddLast(0), _magDefRoundAddLast(0),
    _defendChangeSSLast(0), _magDefendChangeSSLast(0),
    _criticalAdd_last(0), _criticalDmgAdd_last(0), _pierceAdd_last(0), _counterAdd_last(0), _magResAdd_last(0), _toughAdd_last(0),
    _pierceRoundAdd_last(0),
    _maxhpAdd_last(0), _maxActionAdd_last(0), _atkreduce_last(0), _magatkreduce_last(0), 
    _attackRoundAddId(0), _attackRoundSubId(0), _magAtkRoundAddId(0), _defRoundAddId(0), _magDefRoundAddId(0),
    _pierceRoundAddId(0),
    _formEffect(NULL), _formula(bf),
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
    _deep_confuse_dmg_extra(0), _deep_confuse_last(0),
    _deep_forget_dmg_extra(0), _deep_forget_last(0), _deep_stun_dmg_extra(0), _deep_stun_last(0),
    //_therapy_dec(0), _therapy_dec_last(0),_therapy_add(0),_therapy_add_last(0),
    _therapy_buff(0), _therapy_buff_last(0),
    _aura_dec(0), _aura_dec_last(0),
    _bleedFlag(0),
    _bleed1(0), _bleed2(0), _bleed3(0), _self_bleed(0),
    _bleed1_last(0), _bleed2_last(0), _bleed3_last(0), _self_bleed_last(0),
    _immune2(0), _def_dec(0), _def_dec_last(0), _def_dec_times(0),
    _aura_bleed(0), _aura_present(0), _aura_present_cd(0), _aura_dec_cd(0), _aura_bleed_last(0),
    _stun_bleed(0), _stun_present(0), _stun_present_cd(0), _stun_cd(0), _stun_bleed_last(0),
    _confuse_bleed(0), _confuse_present(0), _confuse_present_cd(0), _confuse_cd(0), _confuse_bleed_last(0),
    _colorStock(0), _colorStockTimes(0), _colorStockLast(0),
    _summon(false), _summonLast(0), _moAuraBuf(0), _moAuraBufLast(0), _moEvade100(false), _moEvade100Last(0),
    _hideBuf(false), _hideBufLast(0), _markMo(false), _markMoLast(0),
    _blind(0), _blind_last(0), _deep_blind_dmg_extra(0), _deep_blind_last(0),
    
	_moAttackAdd(0), _moMagAtkAdd(0), _moAtkReduce(0), _moMagAtkReduce(0),
	_moAttackAddCD(0), _moMagAtkAddCD(0), _moAtkReduceCD(0), _moMagAtkReduceCD(0),
	_petAttackAdd(0), _petMagAtkAdd(0), _petAtkReduce(0), _petMagAtkReduce(0),
	_petAttackAddCD(0), _petMagAtkAddCD(0), _petAtkReduceCD(0), _petMagAtkReduceCD(0),
    _petExAtk(0), _petExAtkEnable(false), _petExAtkId(0),
    _lastHPLostP(0), 
    _hpAtkAdd(0), _hpMagAtkAdd(0), _hpAtkAddCount(0),
    _hpAtkReduce(0), _hpMagAtkReduce(0), _hpAtkReduceCount(0), _hpRecoverCount(0),
    _bleedMo(0), _bleedMoLast(0), _blind_bleed(0), _blind_present(0), _blind_present_cd(0),
    _blind_cd(0), _blind_bleed_last(0), _summoner(NULL), _unSummonAura(0), 
    _bleedLingYan(0), _bleedLingYanLast(0), _bleedLingYanAuraDec(0), _bleedLingYanAuraDecProb(0),
    _bleedFieldGape(0), _bleedFieldGapeLast(0), _bleedFieldGapeStunProb(0), _bleedFieldGapeType(0),
    _shieldHP(0), _shieldHPLast(0), _petShieldHP(0), 
    _petProtect100(false), _petProtect100Last(0), _petProtect100Skill(NULL), _petAtk100(0), _petAtk100Last(0), _petMark(false),
    _flawCount(0), _flawDamageAdd(0), _flawLast(0), _withstandFactor(0), _withstandCount(0),
    _chaosWorldId(0), _chaosWorldLast(0),
    _atkAddSpecial(0), _atkSpecialLast(0), _magAtkAddSpecial(0), _magAtkSpecialLast(0), 
    _atkDecSpecial(0), _atkDecSpecialLast(0), _magAtkDecSpecial(0), _magAtkDecSpecialLast(0),
    _skillUsedChangeAttrValue(0), _skillUsedChangeAttrLast(0), _skillUsedChangeAttr(0),
    _bleedRandom(0), _bleedRandomLast(0), _bleedAttackClass(1),_bleedBySkill(0), _bleedBySkillLast(0), _bleedBySkillClass(1),
    _hitChangeByPeerless(0),_counterChangeByPeerless(0),_bSingleAttackFlag(false),_bMainTargetDead(false),_nCurrentAttackIndex(0),
    _darkVigor(0), _dvFactor(0), _darkVigorLast(0), _hpShieldSelf(0), _hpShieldSelf_last(0),
    _counter_spirit_atk_add(0), _counter_spirit_magatk_add(0), _counter_spirit_def_add(0), _counter_spirit_magdef_add(0), _counter_spirit_times(0), _counter_spirit_last(0), _counter_spirit_efv(0), _counter_spirit_skillid(0), _counter_spirit_skill_cd(0), _pet_coatk(0), _fire_defend(0), _fire_defend_last(0), _fire_fake_dead_rate(0), _fire_fake_dead_rate_last(0), _sneak_atk(0), _sneak_atk_status(0), _sneak_atk_last(0), _sneak_atk_recover_rate(0),
    _selfSummon(NULL), _dec_wave_dmg(0), _lingqu_last(0), _lingqu_times(0), _lingqu(false), _soulout_last(0), _soulout(false),  _lingshi_bleed(0), _lingshi_bleed_last(0),
    _lingyou_atk(0), _lingyou_magatk(0), _lingyou_def(0), _lingyou_magdef(0), _lingHpShield(false), _criticaldmgreduce(0), _abnormalTypeCnt(0), _bleedTypeCnt(0),_evadeCnt(0), _peerlessDisableLast(0), _soulProtectLast(0), _soulProtectCount(0), _xinMoCount(0), _2ndRateCoAtk(0), _2ndCoAtkSkill(NULL), _2ndRateProtect(0), _2ndProtectSkill(NULL), _dmg_deep(0), _dmg_deep_last(0), _dmg_ningshi(0), _dmg_ningshi_last(0), _ningshizhe(NULL)
   ,_ruRedCarpetLast(0), _shiFlowerLast(0), _shiFlowerAura(0), _daoRoseLast(0), _moKnotLast(0)
   ,_bActCnt(0), _immune3(0), _revivalCnt(0), _prudentLast(0),_prudentHitrate(0), _prudentHitrateLastOtherside(0), _silkwormCnt(0)
   ,_yehuoLevel(0), _yehuo_ss_dmgRate(0), _yehuo_ss_upRate(0), _jiuziDmgCnt(0), _changeStatus(0), _newModeLast(0), _counterCnt(0), _criticalCnt(0), _preAtk(false), _friendDeadCnt(0), _enemyDeadCnt(0), _mojianCnt(0xFF)
   ,_tyslSSCnt(0), _tyslSSFactor(0),_tyslSSAddCnt(true), _tyslSSCnt2(0)
   ,_controlBallCnt(0)
{
    memset(_immuneLevel, 0, sizeof(_immuneLevel));
    memset(_immuneRound, 0, sizeof(_immuneRound));
    auralRate = 0;
    auraValue = 0;
    auralLast = 0;
    launcher = NULL;
    memset(_peerlessDisableSSLast, 0, sizeof(_peerlessDisableSSLast));
	setFighter(f);
}

BattleFighter::~BattleFighter()
{
}

void BattleFighter::setFighter( GObject::Fighter * f )
{
	_fighter = f;

    _peerlessSkill.base = GData::skillManager[_fighter->getPeerlessAndLevel()];  //无双技能
    // reg skillstrenghten
    updateSkillStrengthen(_fighter->getPeerlessAndLevel());

    std::vector<UInt16> activeSkill;
    _fighter->getUpSkillAndLevel(activeSkill);    //获得设置的技能

    size_t idx;
    size_t cnt = activeSkill.size();

    _activeSkill.clear();
    _activeSkillIdx = 0;
    _therapySkillIdx = 0;
    _revival = false;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[activeSkill[idx]];
        if(!skillItem.base || !skillItem.base->effect)
            continue;
        skillItem.cd = 0;
        bool isTherapy = (skillItem.base->effect->hp > 0 || skillItem.base->effect->hpP > 0.001) && skillItem.base->target == GData::e_battle_target_selfside;
        if(isTherapy)
            _therapySkill.insert(_therapySkill.end(), skillItem);  //治疗性质技能
        else
            _activeSkill.insert(_activeSkill.end(), skillItem);    //活动技能

        updateSkillStrengthen(activeSkill[idx]);
    }

    _abnormalTypeSkill = NULL;
    _bleedTypeSkill = NULL;

    updatePassiveSkill100(_fighter->getPassiveSkillPreAtk100(), _passiveSkillPreAtk100);
    updatePassiveSkill100(_fighter->getPassiveSkillAftAtk100(), _passiveSkillAftAtk100);
    updatePassiveSkill100(_fighter->getPassiveSkillAftAction100(), _passiveSkillAftAction100);
    updatePassiveSkill100(_fighter->getPassiveSkillBeAtk100(), _passiveSkillBeAtk100);
    updatePassiveSkill100(_fighter->getPassiveSkillAftEvd100(), _passiveSkillAftEvd100);
    updatePassiveSkill100(_fighter->getPassiveSkillAftRes100(), _passiveSkillAftRes100);
    updatePassiveSkill100(_fighter->getPassiveSkillEnter100(), _passiveSkillEnter100);
    updatePassiveSkill100(_fighter->getPassiveSkillDead100(), _passiveSkillDead100);
    updatePassiveSkill100(_fighter->getPassiveSkillAftNAtk100(), _passiveSkillAftNAtk100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnTherapy(), _passiveSkillOnTherapy);
    updatePassiveSkill100(_fighter->getPassiveSkillOnSkillDmg(), _passiveSkillOnSkillDmg);
    updatePassiveSkill100(_fighter->getPassiveSkillOnOtherDead(), _passiveSkillOnOtherDead);
    updatePassiveSkill100(_fighter->getPassiveSkillOnCounter100(), _passiveSkillOnCounter100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnAttackBleed100(), _passiveSkillOnAttackBleed100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnAttackConfuseForget100(), _passiveSkillOnAttackConfuseForget100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnAttackStun100(), _passiveSkillOnAttackStun100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnAttackBlind100(), _passiveSkillOnAttackBlind100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnAtkDmg100(), _passiveSkillOnAtkDmg100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnPetProtect100(), _passiveSkillOnPetProtect100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnGetDmg100(), _passiveSkillOnGetDmg100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnBeDmg100(), _passiveSkillOnBeDmg100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnBePHYDmg100(), _passiveSkillOnBePHYDmg100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnBeMagDmg100(), _passiveSkillOnBeMagDmg100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnHP10P100(), _passiveSkillOnHP10P100);
    updatePassiveSkill100(_fighter->getPassiveSkillDeadFake100(), _passiveSkillDeadFake100);
    updatePassiveSkill100(_fighter->getPassiveSkillAbnormalTypeDmg100(), _passiveSkillAbnormalTypeDmg100);
    updatePassiveSkill100(_fighter->getPassiveSkillBleedTypeDmg100(), _passiveSkillBleedTypeDmg100);
    updatePassiveSkill100(_fighter->getPassiveSkillXMCZ100(), _passiveSkillXMCZ100);
    updatePassiveSkill100(_fighter->getPassiveSkillBLTY100(), _passiveSkillBLTY100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnHPChange100(), _passiveSkillOnHPChange100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnWithstand100(), _passiveSkillOnWithstand100);
    updatePassiveSkill100(_fighter->getPassiveSkillOnOtherConfuseForget100(), _passiveSkillOnOtherConfuseForget100);

    updatePassiveSkill(_fighter->getPassiveSkillPreAtk(), _passiveSkillPreAtk);
    updatePassiveSkill(_fighter->getPassiveSkillAftAtk(), _passiveSkillAftAtk);
    updatePassiveSkill(_fighter->getPassiveSkillAftAction(), _passiveSkillAftAction);
    updatePassiveSkill(_fighter->getPassiveSkillBeAtk(), _passiveSkillBeAtk);
    updatePassiveSkill(_fighter->getPassiveSkillAftEvd(), _passiveSkillAftEvd);
    updatePassiveSkill(_fighter->getPassiveSkillAftRes(), _passiveSkillAftRes);
    updatePassiveSkill(_fighter->getPassiveSkillEnter(), _passiveSkillEnter);
    updatePassiveSkill(_fighter->getPassiveSkillDead(), _passiveSkillDead);
    updatePassiveSkill(_fighter->getPassiveSkillAftNAtk(), _passiveSkillAftNAtk);
    updatePassiveSkill(_fighter->getPassiveSkillOnPetProtect(), _passiveSkillOnPetProtect);
    updatePassiveSkill(_fighter->getPassiveSkillOnCounter(), _passiveSkillOnCounter);
    updatePassiveSkill(_fighter->getPassiveSkillOnAttackConfuseForget(), _passiveSkillOnAttackConfuseForget);
    updatePassiveSkill(_fighter->getPassiveSkillOnAttackStun(), _passiveSkillOnAttackStun);
    updatePassiveSkill(_fighter->getPassiveSkillOnAttackBlind(), _passiveSkillOnAttackBlind);
    updatePassiveSkill(_fighter->getPassiveSkillOnAtkDmg(), _passiveSkillOnAtkDmg);
    updatePassiveSkill(_fighter->getPassiveSkillOnGetDmg(), _passiveSkillOnGetDmg);
    updatePassiveSkill(_fighter->getPassiveSkillOnBeDmg(), _passiveSkillOnBeDmg);
    updatePassiveSkill(_fighter->getPassiveSkillOnBePHYDmg(), _passiveSkillOnBePHYDmg);
    updatePassiveSkill(_fighter->getPassiveSkillOnBeMagDmg(), _passiveSkillOnBeMagDmg);
    updatePassiveSkill(_fighter->getPassiveSkillOnHP10P(), _passiveSkillOnHP10P);
    updatePassiveSkill(_fighter->getPassiveSkillDeadFake(), _passiveSkillDeadFake);
    updatePassiveSkill(_fighter->getPassiveSkillOnHPChange(), _passiveSkillOnHPChange);
    updatePassiveSkill(_fighter->getPassiveSkillOnWithstand(), _passiveSkillOnWithstand);
    updatePassiveSkill(_fighter->getPassiveSkillOnOtherConfuseForget(), _passiveSkillOnOtherConfuseForget);

    updateSoulSkillDead(_fighter->getSoulSkillSoulOut());
    updateSoulSkillProtect(_fighter->getSoulSkillProtect());
    updatePassiveSkill(_fighter->getPassiveSkillBleedTypeDmg(), _passiveSkillBleedTypeDmg);
    updatePassiveSkillPrvAtk100Status();
    updatePassiveSkillBLTY100Status();
    updatePassiveSkill100(_fighter->getPassiveSkillViolent100(), _passiveSkillViolent100);
    updatePassiveSkill100(_fighter->getPassiveSkillRevival100(), _passiveSkillRevival100);

    updatePassiveSkill100(_fighter->getPassiveSkillByLingshi100(GData::SKILL_ENTER_LINGSHI), _passiveSkillEnterLingshi100);
    initPassiveSkillByLingshi();

    std::vector<GObject::LBSkill>& lbSkills =  _fighter->getLBSkill();
    cnt = lbSkills.size();
    for(idx = 0; idx < cnt; ++ idx)
    {
        const GData::LBSkillBase* base = GData::lbSkillManager[lbSkills[idx].skillid];
        if(!base)
            continue;
        GData::LBSkillItem item;
        item.base = base;
        item.cd = 0;
        item.prob = base->prob * ((float)lbSkills[idx].factor/10000);
        item.ef_value = base->ef_value * ((float)lbSkills[idx].factor/10000);
        switch(base->cond)
        {
        case GData::e_lb_cond_skill:
            _onSkillCond.push_back(item);
            break;
        case GData::e_lb_cond_action1:
            _onActionCond1.push_back(item);
            break;
        case GData::e_lb_cond_dead:
            _onDeadCond.push_back(item);
            break;
        case GData::e_lb_cond_bleed:
            _onBleedCond.push_back(item);
            break;
        case GData::e_lb_cond_state:
            _onStateCond.push_back(item);
            break;
        case GData::e_lb_cond_action2:
            _onActionCond2.push_back(item);
            break;
        }
    }

    _sg_v.clear();
    _fighter->getAllSGInfo(_sg_v);

}

void BattleFighter::initPassiveSkillByLingshi()
{
    if (_allPassiveSkillLingshi.size() < GData::SKILL_PASSIVES)
        _allPassiveSkillLingshi.resize(GData::SKILL_PASSIVES);
    if (_allPassiveSkillLingshi100.size() < GData::SKILL_PASSIVES)
        _allPassiveSkillLingshi100.resize(GData::SKILL_PASSIVES);
    for (UInt8 type = GData::SKILL_PASSSTART; type < GData::SKILL_PASSIVES; ++type)
    {
        std::vector<UInt16>& passiveSkillId = _fighter->getPassiveSkillByLingshi(type);
        _allPassiveSkillLingshi[type] = passiveSkillId;
        std::vector<UInt16>& passiveSkillId100 = _fighter->getPassiveSkillByLingshi100(type);
        _allPassiveSkillLingshi100[type] = passiveSkillId100;
    }
}

void BattleFighter::updateAllAttr()
{
	updateBuffExtras();
    float factor = 1.0f;
    if(_fighter && _fighter->getOwner())
        factor = _fighter->getOwner()->getSpiritFactor() * _fighter->getOwner()->getKJTMFactor();

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
inline void factorAttrExtra( GData::AttrExtra& ae, const GObject::AttrFactor * ef )
{
	if(ef == NULL)
		return;
	ae *= (*ef);
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

    if(_fighter && _fighter->getOwner() && _fighter->getOwner()->hasHiAttrFlag())
    {
        const GData::AttrExtra* ae = _fighter->getOwner()->getHIAttr();
        if (ae)
            addAttrExtra(_attrExtra, ae);
    }
    if(_fighter && _fighter->getOwner() && _fighter->getOwner()->hasClanRankBuffFlag())
    {
     if(_fighter->getOwner()->getBuffData(PLAYER_BUFF_CLANTREE2))
        {
            _attrExtra.attack *= 1.1f;
            _attrExtra.magatk *= 1.1f ;
            _attrExtra.defend *= 1.1f;
            _attrExtra.magdef *= 1.1f;
            _attrExtra.action *= 1.1f;
            _attrExtra.tough  *= 1.1f;
            _attrExtra.evade  *= 1.1f;
            _attrExtra.hitrate  *= 1.1f;
            _attrExtra.hp  *= 1.1f;
        }
    }       
/*    if(_fighter && _fighter->getOwner() && _fighter->getOwner()->hasHiAfFlag())
    {
        const GObject::AttrExtra* af = _fighter->getOwner()->getHIAf();
        if (af)
        {
            _attrExtra.attackP += af->attackP;
            _attrExtra.magatkP += af->magatkP;
            if (af->hp > 0)
                _attrExtra.hpP += af->hpP;
            _attrExtra.actionP += af->actionP;
        }
    }
*/
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

    if (_flag & Enh7)
    {
        _attrExtra.attack += 50;
        _attrExtra.magatk += 50;
        _attrExtra.defend += 100;
        _attrExtra.magdef += 100;
        _attrExtra.action += 20;
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
        /*_attrExtra.pierce *= 1.1f;
        _attrExtra.attack *= 1.1f;
        _attrExtra.magatk *= 1.1f;*/
        _attrExtra.attack *= 1.05f;
        _attrExtra.magatk *= 1.05f;
    }
    if (_flag2 & AthlEnh32)
    {
        /*_attrExtra.criticaldmg *= 1.25f;
        _attrExtra.critical += 1.1f;*/
        _attrExtra.physique *= 1.05f;

    }
    if (_flag2 & AthlEnh33)
    {
        //_attrExtra.magres *= 1.2f;
        _attrExtra.evade *= 1.05f;
    }
    if (_flag2 & AthlEnh34)
    {
        /*_attrExtra.counter *= 1.1f;
        _attrExtra.hitrate *= 1.1f;*/
        _attrExtra.pierce *= 1.05f;
    }
    if (_flag2 & AthlEnh35)
    {
        /*_attrExtra.evade *= 1.1f;
        _attrExtra.action *= 1.1f;*/
        _attrExtra.counter *= 1.05f;
    }
    if (_flag2 & AthlEnh36)
    {
        /*_attrExtra.physique *= 1.1f;
        _attrExtra.defend *= 1.1f;
        _attrExtra.magdef *= 1.1f;*/
        _attrExtra.critical *= 1.05f;
    }
    if (_flag2 & AthlEnh37)
    {
        /*_attrExtra.aura += 100;*/
        _attrExtra.defend *= 1.05f;
        _attrExtra.magdef *= 1.05f;
    }

    float clanFactor = 0;
    if(_fighter && _fighter->getOwner())
    {
        if(_fighter->getOwner()->getBuffData(PLAYER_BUFF_CLAN1) > 0)
            clanFactor = 1.05;
        else if(_fighter->getOwner()->getBuffData(PLAYER_BUFF_CLAN2) > 0)
            clanFactor = 1.03;
        else if(_fighter->getOwner()->getBuffData(PLAYER_BUFF_CLAN3) > 0)
            clanFactor = 1.02;
        
        if(_fighter->getOwner()->getBuffData(PLAYER_BUFF_TYSS) > 0)
        {
            if(clanFactor == 0)
                clanFactor = 1 + 0.01;
            else
                clanFactor += 0.01;
        }

    }
    if(clanFactor > 0.001f)
    {
        _attrExtra.attack *= clanFactor;
        _attrExtra.magatk *= clanFactor;
        _attrExtra.defend *= clanFactor;
        _attrExtra.magdef *= clanFactor;
        _attrExtra.action *= clanFactor;
        _attrExtra.tough  *= clanFactor;
        _attrExtra.evade  *= clanFactor;
        _attrExtra.hitrate *= clanFactor;
        _attrExtra.hp *= clanFactor;
    }

    if(_fighter && _fighter->getOwner())
    {
        UInt8 bufId = _fighter->getOwner()->getRBBufId();
        float factor = _fighter->getOwner()->getRBBufValue();
        switch(bufId)
        {
            case HP_ID:
                _attrExtra.hp *= (1 + factor);
                break;
            case ACTION_ID:
                _attrExtra.action *= (1 + factor);
                break;
            case HIT_ID:
                _attrExtra.hitrate *= (1 + factor);
                break;
            case EVADE_ID:
                _attrExtra.evade *= (1 + factor);
                break;
            case CRITICAL_ID:
                _attrExtra.critical *= (1 + factor);
                break;
            case PIERCE_ID:
                _attrExtra.pierce *= (1 + factor);
                break;
            case COUNTER_ID:
                _attrExtra.counter *= (1 + factor);
                break;
            case TOUGH_ID:
                _attrExtra.tough *= (1 + factor);
                break;
            case MAGRES_ID:
                _attrExtra.magres *= (1 + factor);
                break;
            case STRENGTH_ID:
                _attrExtra.strength *= (1 + factor);
                break;
            case PHYSIQUE_ID:
                _attrExtra.physique *= (1 + factor);
                break;
            case AGILITY_ID:
                _attrExtra.agility *= (1 + factor);
                break;
            case INTELLIGENCE_ID:
                _attrExtra.intelligence *= (1 + factor);
                break;
            case WILL_ID:
                _attrExtra.will *= (1 + factor);
                break;
            case ATTACK_ID:
                _attrExtra.attack *= (1 + factor);
                break;
            case MAGATK_ID:
                _attrExtra.magatk *= (1 + factor);
                break;
            case DEFEND_ID:
                _attrExtra.defend *= (1 + factor);
                break;
            case MAGDEF_ID:
                _attrExtra.magdef *= (1 + factor);
                break;
            case CRIDMG_ID:
                _attrExtra.criticaldmg *= (1 + factor);
                break;
            case CRIIMMUNE_ID:
                _attrExtra.criticaldmgimmune *= (1 + factor);
                break;
            case AURA_ID:
                _attrExtra.aura += factor;
                break;
            default:
                break;
        }
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

    float factor = getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(this);
    if(factor < 1.25)
        factor = 1.25;

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

    float factor = getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(this);
    if(factor < 1.25)
        factor = 1.25;

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

    float factor = getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(this);
    if(factor < 1.25)
        factor = 1.25;

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

    Int32 sg_v = getSkillGradeExtraValue(SKILL_ID(skill->getId()));
    GData::LBSkillItem* item = getSkillCondItem(SKILL_ID(skill->getId()));
    if(NULL != item)
        return aura_factor * (getMagAttack() * skill->effect->hpP + skill->effect->addhp + skill->effect->hp + item->ef_value + sg_v);

    return aura_factor * (getMagAttack() * skill->effect->hpP + skill->effect->addhp + skill->effect->hp + sg_v);
}

float BattleFighter::calcMaxTherapy(const GData::SkillBase* skill)
{
    // 回血量的最高值，和施放者的攻击力有关
    if(!skill)
        return 0;

    if(skill->effect == NULL)
        return 0;

    return getMagAttack() * skill->effect->maxhpdampec;
}

float BattleFighter::calcPoison(const GData::SkillBase* skill, BattleFighter* defender, bool cs)
{
    if(!skill)
        return 0;

    if(skill->effect == NULL)
        return 0;

    float magatk = getMagAttack();
    float atk = getAttack();

    float factor = getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(this);
    if(factor < 1.25)
        factor = 1.25;

    if(!cs)
    {
        factor = 1.25;
    }

    // 道
    if(getClass() == GObject::e_cls_dao || getClass() == GObject::e_cls_mo)
    {
        return (atk * skill->effect->hpP + skill->effect->hp + skill->effect->addhp) * factor * (950 + uRand(100)) / 1000;
    }

    // 儒释
    return (magatk * skill->effect->hpP + skill->effect->hp + skill->effect->addhp) * factor *  (950 + uRand(100)) / 1000;
}

bool BattleFighter::calcHit( BattleFighter * defender, const GData::SkillBase* skill)
{
	if(defender->getStunRound() > 0)
		return true;

	// 计算命中值
	float hitrate = getHitrate(defender) - defender->getEvade(this);

    if(skill)
    {
        GData::SkillStrengthenBase* ss = getSkillStrengthen(SKILL_ID(skill->getId()));
        const GData::SkillStrengthenEffect* ef = NULL;
        if(ss)
            ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_HITRATE_COUNTER);
        if(ef)
        {
            hitrate += ef->value;
        }
    }

    hitrate = hitrate * (1 - getBlind());
	// 必中
	if(hitrate >= 100)
		return true;

	// 计算命中率
	return uRand(10000) < hitrate * 100;
}

bool BattleFighter::calcCounter(BattleFighter* attacker, const GData::SkillBase* skill)
{
	return uRand(10000) < getCounter(attacker, skill) * 100;
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
                    _fighter->getBuffData(FIGHTER_BUFF_QQVIP, now) ||
                    _fighter->getBuffData(FIGHTER_BUFF_SANTA, now))
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
 
        if(_fighter && _fighter->getOwner() && _fighter->getOwner()->getCBHPFlag())
        {
            _hp *= 0.1;
        }
	}
}

UInt32 BattleFighter::regenHP( UInt32 u, bool weak /* = false */, float hppec /* = 0 */, float maxRhp /* = 0 */)
{
    if(isSoulOut())
        return 0;
    if(_weakRound > 0 && weak)
        u /= 2;
    UInt32 tmp = getMaxHP() * hppec;

    if (maxRhp > 0)
        tmp = tmp > maxRhp? maxRhp:tmp;

    u += tmp;
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
    UInt16 portrait = 0;
    if(!isNpc())
    {
        UInt32 now = TimeUtil::Now();
        if(getBuffData(FIGHTER_BUFF_CRMASGIRL, now))
            portrait = 1058;
        else if(getBuffData(FIGHTER_BUFF_DRESS, now))
            portrait = 1063;
        else if(getBuffData(FIGHTER_BUFF_WEDDING, now))
            portrait = 1064;
        else if(getBuffData(FIGHTER_BUFF_RMAN, now))
            portrait = 1;
        else if(getBuffData(FIGHTER_BUFF_RWMAN, now))
            portrait = 2;
        else if(getBuffData(FIGHTER_BUFF_SMAN, now))
            portrait = 3;
        else if(getBuffData(FIGHTER_BUFF_SWMAN, now))
            portrait = 4;
        else if(getBuffData(FIGHTER_BUFF_DMAN, now))
            portrait = 5;
        else if(getBuffData(FIGHTER_BUFF_DWMAN, now))
            portrait = 6;
        else if(getBuffData(FIGHTER_BUFF_RDIAMOND, now))
            portrait = 1089;
        else if(getBuffData(FIGHTER_BUFF_BLUE, now))
            portrait = 1090;
        else if(getBuffData(FIGHTER_BUFF_QQVIP, now))
            portrait = 1091;
        else if(getBuffData(FIGHTER_BUFF_SANTA, now))
            portrait = 1092;
        else
            portrait = getFighter()->getPortrait();
    }
    else
    {
        portrait = getFighter()->getPortrait();
    }

    return portrait;
}

UInt8 BattleFighter::getClassAndSex()
{
    UInt8 clsnsex = 0;
    if(!isNpc())
    {
        UInt32 now = TimeUtil::Now();
        if(getBuffData(FIGHTER_BUFF_RMAN, now))
            clsnsex = GObject::e_cls_ru<<4;
        else if(getBuffData(FIGHTER_BUFF_RWMAN, now))
            clsnsex = GObject::e_cls_ru<<4|1;
        else if(getBuffData(FIGHTER_BUFF_SMAN, now))
            clsnsex = GObject::e_cls_shi<<4;
        else if(getBuffData(FIGHTER_BUFF_SWMAN, now))
            clsnsex = GObject::e_cls_shi<<4|1;
        else if(getBuffData(FIGHTER_BUFF_DMAN, now))
            clsnsex = GObject::e_cls_dao<<4;
        else if(getBuffData(FIGHTER_BUFF_DWMAN, now))
            clsnsex = GObject::e_cls_dao<<4|1;
        else
            clsnsex = getFighter()->getClassAndSex();
    }
    else
    {
        clsnsex = static_cast<UInt8>(getFighter()->reqFriendliness);
    }

    // 仙宠在战报中的职业ID为5
    if ((clsnsex >> 4) >= GObject::e_cls_qinglong) 
        clsnsex = (clsnsex & 0x0f) | (GObject::e_cls_qinglong << 4);

    return clsnsex;
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

const GData::SkillBase* BattleFighter::getActiveSkill(bool need_therapy, bool noPossibleTarget)
{
    GData::SkillItem* resSkillItem = NULL;
    if(NULL != _peerlessSkill.base)
    {
        bool isValid = false;
        if(getBuddhaLightLast() > 0 && getBlind() < 0.001f)
        {
            if(uRand(10000) < getBuddhaLightRate() * 100)
                isValid = true;
            else
                setBuddhaLight(0, 0xFF);
        }
        if(_peerlessDisableLast > 0)
            --_peerlessDisableLast;
        else
        {
            if(((_aura >= 100 || isValid == true) && _peerlessSkill.base->effect != NULL) && (!noPossibleTarget || _peerlessSkill.base->target != GData::e_battle_target_otherside))
            {
                // peerless skill first
                if (_fighter->getOwner())
                    _fighter->getOwner()->OnHeroMemo(GObject::MC_SKILL, GObject::MD_ADVANCED, 0, 2);
                return _peerlessSkill.base;
            }
        }
    }
    else
    {
        if(getBuddhaLightLast() > 0 && getBlind() < 0.001f)
            setBuddhaLight(0, 0xFF);
    }

    if(need_therapy)
    {
        const GData::SkillBase* skill = getTherapySkill();
        if(skill)
            return skill;
    }

    size_t cnt = _activeSkill.size();
    if(cnt == 0)
        return NULL;

    // therapy skill second while need therapy
    size_t idx0 = _activeSkillIdx % cnt;
    size_t idx = 0;
    for(size_t i = 0; i < cnt; ++i)
    {
        idx = (idx0 + i) % cnt;
        if(NULL != _activeSkill[idx].base && _activeSkill[idx].cd == 0)
        {
            if(_activeSkill[idx].base->effect == NULL)
                continue;
            if(!resSkillItem && (!noPossibleTarget || _activeSkill[idx].base->target != GData::e_battle_target_otherside))
            {
                resSkillItem = &(_activeSkill[idx]);
                _activeSkill[idx].cd = resSkillItem->base->cd + 1;
                break;
            }
        }
    }

    if(resSkillItem)
    {
        _activeSkillIdx  = idx + 1;

        return resSkillItem->base;
    }

    return NULL;
}

const GData::SkillBase* BattleFighter::getTherapySkill()
{
    GData::SkillItem* resSkillItem = NULL;
    size_t cnt = _therapySkill.size();
    if(cnt == 0)
        return NULL;

    size_t idx0 = _therapySkillIdx % cnt;
    size_t idx = 0;
    for(size_t i = 0; i < cnt; ++i)
    {
        idx = (idx0 + i) % cnt;
        if(NULL != _therapySkill[idx].base && _therapySkill[idx].cd == 0)
        {
            if(_therapySkill[idx].base->effect == NULL)
                continue;
            if(!resSkillItem)
            {
                resSkillItem = &(_therapySkill[idx]);
                _therapySkill[idx].cd = resSkillItem->base->cd + 1;
                break;
            }
        }
    }

    if(resSkillItem)
    {
        _therapySkillIdx  = idx + 1;

        return resSkillItem->base;
    }

    return NULL;
}

const GData::SkillBase* BattleFighter::getPassiveSkill100(std::vector<GData::SkillItem>& passiveSkill100, size_t& idx, bool noPossibleTarget)
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
            if(noPossibleTarget && passiveSkill100[idx].base->target == GData::e_battle_target_otherside)
                continue;
            passiveSkill100[idx].cd = passiveSkill100[idx].base->cd + 1;

            if (_fighter->getOwner())
                _fighter->getOwner()->OnHeroMemo(GObject::MC_SKILL, GObject::MD_MASTER, 0, 1);

            return passiveSkill100[idx++].base;
        }
    }

    return NULL;
}

const GData::SkillBase* BattleFighter::getPassiveSkillPreAtk100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillPreAtk100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftAtk100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillAftAtk100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftAction100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillAftAction100, idx, noPossibleTarget);
}


const GData::SkillBase* BattleFighter::getPassiveSkillBeAtk100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillBeAtk100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftEvd100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillAftEvd100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftRes100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillAftRes100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillEnter100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillEnter100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillDead100(size_t& idx, bool noPossibleTarget)
{
    const GData::SkillBase* skill = getPassiveSkill100(_passiveSkillDead100, idx, noPossibleTarget);

    if(skill != NULL && SKILL_ID(skill->getId()) == 215)
        _passiveSkillDead100.erase(_passiveSkillDead100.begin() + idx - 1);

    return skill;
}

const GData::SkillBase* BattleFighter::getPassiveSkillDeadFake100(size_t& idx, bool noPossibleTarget)
{
    const GData::SkillBase* skill = getPassiveSkill100(_passiveSkillDeadFake100, idx, noPossibleTarget);

    if(skill != NULL && SKILL_ID(skill->getId()) == 215)
        _passiveSkillDeadFake100.erase(_passiveSkillDeadFake100.begin() + idx - 1);

    return skill;
}


const GData::SkillBase* BattleFighter::getPassiveSkillAftNAtk100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillAftNAtk100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnCounter100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnCounter100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAttackBleed100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnAttackBleed100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAttackConfuseForget100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnAttackConfuseForget100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAttackStun100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnAttackStun100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAttackBlind100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnAttackBlind100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAtkDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnAtkDmg100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnGetDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnGetDmg100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnHP10P100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100( _passiveSkillOnHP10P100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnBeMagDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100( _passiveSkillOnBeMagDmg100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnBePHYDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100( _passiveSkillOnBePHYDmg100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnBeDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100( _passiveSkillOnBeDmg100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAbnormalTypeDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillAbnormalTypeDmg100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillBleedTypeDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillBleedTypeDmg100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillXMCZ100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillXMCZ100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillBLTY100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillBLTY100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillViolent100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillViolent100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillRevival100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillRevival100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillLingshi100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillLingshi100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillEnterLingshi100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillEnterLingshi100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnHPChange100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnHPChange100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnWithstand100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnWithstand100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnOtherConfuseAndForget100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnOtherConfuseForget100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkill(std::vector<GData::SkillItem>& passiveSkill, bool noPossibleTarget)
{
    size_t cnt = passiveSkill.size();
    if(cnt < 1)
        return NULL;
    URandom rand;
    UInt32 rnd = rand(cnt*100*100);
    const GData::SkillBase* resSkillBase = NULL;

    for(size_t idx = 0; idx < cnt; ++idx)
    {
        float rate = passiveSkill[idx].rateExtent; 

        // 技能符文增加触发概率
        GData::SkillStrengthenBase* ss = getSkillStrengthen(SKILL_ID(passiveSkill[idx].base->getId()));
        const GData::SkillStrengthenEffect* ef = NULL;
        if (ss)
            ef = ss->getEffect(GData::ON_GET_SKILL_PROB, GData::TYPE_PROB_ADD);
        if (ef)
            rate = passiveSkill[idx].rateExtent + ef->value * 100;

        if(rnd < rate && passiveSkill[idx].cd == 0)
        {
            if(passiveSkill[idx].base == NULL)
                continue;
            if(passiveSkill[idx].base->effect == NULL)
                continue;
            if(noPossibleTarget && passiveSkill[idx].base->target == GData::e_battle_target_otherside)
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

const GData::SkillBase* BattleFighter::getPassiveSkillForce(std::vector<GData::SkillItem>& passiveSkill, bool noPossibleTarget)
{
    size_t cnt = passiveSkill.size();
    if(cnt < 1)
        return NULL;
    const GData::SkillBase* resSkillBase = NULL;

    for(size_t idx = 0; idx < cnt; ++idx)
    {
        if(passiveSkill[idx].base == NULL)
            continue;
        if(passiveSkill[idx].base->effect == NULL)
            continue;
        if(noPossibleTarget && passiveSkill[idx].base->target == GData::e_battle_target_otherside)
            continue;
        resSkillBase = passiveSkill[idx].base;
        break;
    }

    return resSkillBase;
}

const GData::SkillBase* BattleFighter::getPassiveSkillPreAtk(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillPreAtk, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftAtk(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillAftAtk, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftAction(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillAftAction, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillBeAtk(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillBeAtk, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftEvd(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillAftEvd, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftRes(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillAftRes, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillEnter(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillEnter, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillDead(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillDead, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillDeadFake(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillDeadFake, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillBleedTypeDmg(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillBleedTypeDmg, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftNAtk(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillAftNAtk, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnCounter(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillOnCounter, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAttackBleed(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnAttackBleed, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAttackConfuseForget(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnAttackConfuseForget, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAttackStun(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnAttackStun, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAttackBlind(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnAttackBlind, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAtkDmgForce(bool noPossibleTarget)
{
    return getPassiveSkillForce(_passiveSkillOnAtkDmg, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAtkDmg(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillOnAtkDmg, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnPetProtect(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillOnPetProtect, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnPetProtectForce(bool noPossibleTarget)
{
    return getPassiveSkillForce(_passiveSkillOnPetProtect, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnGetDmg(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnGetDmg, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnHP10P(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnHP10P, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnBeMagDmg(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnBeMagDmg, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnBePHYDmg(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnBePHYDmg, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnBeDmg(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnBeDmg, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnHPChange(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillOnHPChange, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnWithstand(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillOnWithstand, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnOtherConfuseAndForget(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillOnOtherConfuseForget, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getSkillSoulProtect()
{
    const GData::SkillBase* skillBase = NULL;
    if(_passiveSkillSoulProtect.size() > 0)
        skillBase = _passiveSkillSoulProtect[0].base;
    return skillBase;
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
    releaseSkillCD(_therapySkill, cd);

    releaseSkillCD(_passiveSkillPreAtk100, cd);
    releaseSkillCD(_passiveSkillAftAtk100, cd);
    releaseSkillCD(_passiveSkillAftAction100, cd);
    releaseSkillCD(_passiveSkillBeAtk100, cd);
    releaseSkillCD(_passiveSkillAftEvd100, cd);
    releaseSkillCD(_passiveSkillAftRes100, cd);
    releaseSkillCD(_passiveSkillEnter100, cd);
    releaseSkillCD(_passiveSkillDead100, cd);
    releaseSkillCD(_passiveSkillAftNAtk100, cd);
    releaseSkillCD(_passiveSkillOnCounter100, cd);
    releaseSkillCD(_passiveSkillOnAttackBleed100, cd);
    releaseSkillCD(_passiveSkillOnAtkDmg100, cd);
    releaseSkillCD(_passiveSkillOnPetProtect100, cd);
    releaseSkillCD(_passiveSkillOnGetDmg100, cd);
    releaseSkillCD(_passiveSkillOnBeDmg100, cd);
    releaseSkillCD(_passiveSkillOnBePHYDmg100, cd);
    releaseSkillCD(_passiveSkillOnBeMagDmg100, cd);
    releaseSkillCD(_passiveSkillOnHP10P100, cd);
    releaseSkillCD(_passiveSkillDeadFake100, cd);
    releaseSkillCD(_passiveSkillOnHPChange100, cd);
    releaseSkillCD(_passiveSkillOnWithstand100, cd);

    releaseSkillCD(_passiveSkillPreAtk, cd);
    releaseSkillCD(_passiveSkillAftAtk, cd);
    releaseSkillCD(_passiveSkillAftAction, cd);
    releaseSkillCD(_passiveSkillBeAtk, cd);
    releaseSkillCD(_passiveSkillAftEvd, cd);
    releaseSkillCD(_passiveSkillAftRes, cd);
    releaseSkillCD(_passiveSkillEnter, cd);
    releaseSkillCD(_passiveSkillDead, cd);
    releaseSkillCD(_passiveSkillAftNAtk, cd);
    releaseSkillCD(_passiveSkillOnTherapy, cd);
    releaseSkillCD(_passiveSkillOnSkillDmg, cd);
    releaseSkillCD(_passiveSkillOnCounter, cd);
    releaseSkillCD(_passiveSkillOnAtkDmg, cd);
    releaseSkillCD(_passiveSkillOnPetProtect, cd);
    releaseSkillCD(_passiveSkillOnGetDmg, cd);
    releaseSkillCD(_passiveSkillOnBeDmg, cd);
    releaseSkillCD(_passiveSkillOnBePHYDmg, cd);
    releaseSkillCD(_passiveSkillOnBeMagDmg, cd);
    releaseSkillCD(_passiveSkillOnHP10P, cd);
    releaseSkillCD(_passiveSkillDeadFake, cd);
    releaseSkillCD(_passiveSkillSoulProtect, cd);
    releaseSkillCD(_passiveSkillOnHPChange, cd);
    releaseSkillCD(_passiveSkillOnWithstand, cd);

    releaseLBSkillCD();
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

const GData::SkillBase* BattleFighter::getPassiveSkillOnPetProtect100()
{
    size_t idx = 0;
    
    return getPassiveSkill100( _passiveSkillOnPetProtect100, idx);
}

float BattleFighter::getHitrate(BattleFighter* defgt)
{
    float hiterate = 0;
    if(defgt == NULL)
        hiterate = _hitrate + _hitrateAdd + _hitrateAdd2 + _hitChangeByPeerless;
    else
        hiterate = _formula->calcHitrate(this, defgt) + _hitrateAdd + _hitrateAdd2 + _hitChangeByPeerless;

    if(getPrudentLast() > 0)
        hiterate -= getPrudentHitrate();
    if(getPrudentHitrateLastOtherside() > 0)
        hiterate -= 90;

    if(getImmune3() > 0)
        hiterate += 100;

// #ifdef _DEBUG
//     if (_hitChangeByPeerless != 0)
//     {
//         fprintf(stderr, "old hitrate = %f, new hitrate = %f\n", hiterate - _hitChangeByPeerless, hiterate);
//     }
// #endif
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

float BattleFighter::getCounter(BattleFighter* defgt, const GData::SkillBase* skill)
{
    float counter = 0;
    if(defgt == NULL)
        counter = _counter + _counterAdd + _counterAdd2 + _counterChangeByPeerless;
    else
        counter = _formula->calcCounter(this, defgt) + _counterAdd + _counterAdd2 + _counterChangeByPeerless;

    if(getImmune3() > 0)
        counter += 100.0f;

// #ifdef _DEBUG
//     if (_counterChangeByPeerless != 0)
//     {
//         fprintf(stderr, "old counter = %f, new counter = %f\n", counter - _counterChangeByPeerless, counter);
//     }
// #endif

    if(counter > GObject::GObjectManager::getCounterMax() && !isNpc())
        counter = GObject::GObjectManager::getCounterMax();

    if(skill)
    {
        GData::SkillStrengthenBase* ss = defgt->getSkillStrengthen(SKILL_ID(skill->getId()));
        const GData::SkillStrengthenEffect* ef = NULL;
        if(ss)
            ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_HITRATE_COUNTER);
        if(!ef && ss)
            ef = ss->getEffect(GData::ON_ATTACK, GData::TYPE_DEC_COUNTER);
        if(ef)
        {
            counter -= ef->valueExt1;
        }
    }

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
    float factor = getCriticalDmg() - defender->getCriticalDmgImmune() - defender->getTough(this);
    if(factor < 1.25)
        factor = 1.25;

    if(factor > GObject::GObjectManager::getCriticalDmgMax() && !isNpc())
        factor = GObject::GObjectManager::getCriticalDmgMax();

    return factor;
}

float BattleFighter::getCriticalDmg()
{
    float cridmg = _criticaldmg + _criticalDmgAdd + _criticalDmgAdd2 + _criticaldmgreduce;
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

void BattleFighter::setImmuneLevel(UInt16 state, UInt8 f)
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

UInt8 BattleFighter::getImmuneLevel(UInt16 state)
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

void BattleFighter::setImmuneRound(UInt16 state, UInt8 f)
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

UInt8 BattleFighter::getImmuneRound(UInt16 state)
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
            || ((_attackAdd > 0) && (v > 0) && ((_attackAdd - v) < 0.001f))
            || ((_attackAdd < 0) && (v < 0) && ((_attackAdd - v) > 0.001f)))
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
            || ((_magAtkAdd > 0) && (v > 0) && ((_magAtkAdd - v) < 0.001f))
            || ((_magAtkAdd < 0) && (v < 0) && ((_magAtkAdd - v) > 0.001f)))
    {
        _magAtkAdd = v;
        _magAtkAdd_last = last;
    }
}

void BattleFighter::setAttackRoundAdd(float v, UInt16 last /* = 0 */, UInt16 skillId /* = 0 */)
{
    _attackRoundAdd = v;
    _attackRoundAddLast = last;
    _attackRoundAddId   = skillId;
}

void BattleFighter::setAttackRoundSub(float v, UInt16 last /* = 0 */, UInt16 skillId /* = 0 */)
{
    _attackRoundSub = v;
    _attackRoundSubLast = last;
    _attackRoundSubId   = skillId;
}

void BattleFighter::setMagAttackRoundAdd(float v, UInt16 last /* = 0 */, UInt16 skillId /* = 0 */)
{
    _magAtkRoundAdd = v;
    _magAtkRoundAddLast = last;
    _magAtkRoundAddId   = skillId;
}

void BattleFighter::setDefRoundAdd(float v, UInt16 last /* = 0 */, UInt16 skillId /* = 0 */)
{
    _defRoundAdd = v;
    _defRoundAddLast = last;
    _defRoundAddId   = skillId;
}

void BattleFighter::setPierceRoundAdd(float v, UInt16 last /* = 0 */, UInt16 skillId /* = 0 */)
{
    _pierceRoundAdd = v;
    _pierceRoundAdd_last = last;
    _pierceRoundAddId   = skillId;
}

void BattleFighter::setMagDefRoundAdd(float v, UInt16 last /* = 0 */, UInt16 skillId /* = 0 */)
{
    _magDefRoundAdd = v;
    _magDefRoundAddLast = last;
    _magDefRoundAddId   = skillId;
}

bool BattleFighter::addRoundAttack()
{
    if (_attackRoundAdd > 0.1f)
    {
        _attackAdd += _attackRoundAdd;
        return true;
    }
    else
        return false;
}

bool BattleFighter::subRoundAttack()
{
    if (_attackRoundSub > 0.1f)
    {
        _attackAdd -= _attackRoundSub;
        return true;
    }
    else
        return false;
}

bool BattleFighter::addRoundDef()
{
    if (_defRoundAdd > 0.1f)
    {
        _defAdd += _defRoundAdd;
        return true;
    }
    else
        return false;
}

bool BattleFighter::addRoundMagAtk()
{
    if (_magAtkRoundAdd > 0.1f)
    {
        _magAtkAdd += _magAtkRoundAdd;
        return true;
    }
    else
        return false;
}

bool BattleFighter::addRoundMagDef()
{
    if (_magDefRoundAdd > 0.1f)
    {
        _magDefAdd += _magDefRoundAdd;
        return true;
    }
    else
        return false;
}

bool BattleFighter::addRoundPierce()
{
    if (_pierceRoundAdd > 0.1f)
    {
        _pierceAdd += _pierceRoundAdd;
        return true;
    }
    else
        return false;
}

void BattleFighter::setDefendAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_defAdd < 0.001f)
            || ( _defAdd > 0 && v < 0)
            || ( _defAdd < 0 && v > 0)
            || ((_defAdd > 0) && (v > 0) && ((_defAdd - v) < 0.001f))
            || ((_defAdd < 0) && (v < 0) && ((_defAdd - v) > 0.001f)))
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
            || ((_magDefAdd > 0) && (v > 0) && ((_magDefAdd - v) < 0.001f))
            || ((_magDefAdd < 0) && (v < 0) && ((_magDefAdd - v) > 0.001f)))
    {
        _magDefAdd = v;
        _magDefAdd_last = last;
    }
}

void BattleFighter::setDefendChangeSS(float v, UInt16 last)
{
    if((v < 0.001f) || (_defendChangeSS < 0.001f)
            || ( _defendChangeSS > 0 && v < 0)
            || ( _defendChangeSS < 0 && v > 0)
            || ((_defendChangeSS > 0) && (v > 0) && ((_defendChangeSS - v) < 0.001f))
            || ((_defendChangeSS < 0) && (v < 0) && ((_defendChangeSS - v) > 0.001f)))
    {
        _defendChangeSS = v;
        _defendChangeSSLast = last;
    }
}

void BattleFighter::setMagDefendChangeSS(float v, UInt16 last)
{
    if((v < 0.001f) || (_magDefendChangeSS < 0.001f)
            || ( _magDefendChangeSS > 0 && v < 0)
            || ( _magDefendChangeSS < 0 && v > 0)
            || ((_magDefendChangeSS > 0) && (v > 0) && ((_magDefendChangeSS - v) < 0.001f))
            || ((_magDefendChangeSS < 0) && (v < 0) && ((_magDefendChangeSS - v) > 0.001f)))
    {
        _magDefendChangeSS = v;
        _magDefendChangeSSLast = last;
    }
}

void BattleFighter::setHitrateAdd(float v, UInt16 last)
{
    if((v < 0.001f) || (_hitrateAdd < 0.001f)
            || ( _hitrateAdd > 0 && v < 0)
            || ( _hitrateAdd < 0 && v > 0)
            || ((_hitrateAdd > 0) && (v > 0) && ((_hitrateAdd - v) < 0.001f))
            || ((_hitrateAdd < 0) && (v < 0) && ((_hitrateAdd - v) > 0.001f)))
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
            || ((_evadeAdd > 0) && (v > 0) && ((_evadeAdd - v) < 0.001f))
            || ((_evadeAdd < 0) && (v < 0) && ((_evadeAdd - v) > 0.001f)))
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
            || ((_criticalAdd > 0) && (v > 0) && ((_criticalAdd - v) < 0.001f))
            || ((_criticalAdd < 0) && (v < 0) && ((_criticalAdd - v) > 0.001f)))
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
            || ((_criticalDmgAdd > 0) && (v > 0) && ((_criticalDmgAdd - v) < 0.001f))
            || ((_criticalDmgAdd < 0) && (v < 0) && ((_criticalDmgAdd - v) > 0.001f)))
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
            || ((_pierceAdd > 0) && (v > 0) && ((_pierceAdd - v) < 0.001f))
            || ((_pierceAdd < 0) && (v < 0) && ((_pierceAdd - v) > 0.001f)))
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
            || ((_counterAdd > 0) && (v > 0) && ((_counterAdd - v) < 0.001f))
            || ((_counterAdd < 0) && (v < 0) && ((_counterAdd - v) > 0.001f)))
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
            || ((_magResAdd > 0) && (v > 0) && ((_magResAdd - v) < 0.001f))
            || ((_magResAdd < 0) && (v < 0) && ((_magResAdd - v) > 0.001f)))
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
            || ((_maxhpAdd > 0) && (v > 0) && (_maxhpAdd <= v))
            || ((_maxhpAdd < 0) && (v < 0) && (_maxhpAdd >= v)))
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
            || ((_maxActionAdd > 0) && (v > 0) && (_maxActionAdd <= v))
            || ((_maxActionAdd < 0) && (v < 0) && (_maxActionAdd >= v)))
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
            || ((_toughAdd > 0) && (v > 0) && ((_toughAdd - v) < 0.001f))
            || ((_toughAdd < 0) && (v < 0) && ((_toughAdd - v) > 0.001f)))
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
            || ((_atkreduce > 0) && (v > 0) && ((_atkreduce - v) < 0.001f))
            || ((_atkreduce < 0) && (v < 0) && ((_atkreduce - v) > 0.001f)))
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
            || ((_magatkreduce > 0) && (v > 0) && ((_magatkreduce - v) < 0.001f))
            || ((_magatkreduce < 0) && (v < 0) && ((_magatkreduce - v) > 0.001f)))
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
            || ((_atkreduce3 > 0) && (v > 0) && ((_atkreduce3 - v) < 0.001f))
            || ((_atkreduce3 < 0) && (v < 0) && ((_atkreduce3 - v) > 0.001f)))
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
            || ((_magatkreduce3 > 0) && (v > 0) && ((_magatkreduce3 - v) < 0.001f))
            || ((_magatkreduce3 < 0) && (v < 0) && ((_magatkreduce3 - v) > 0.001f)))
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
            || ((_pudu_debuf > 0) && (v > 0) && ((_pudu_debuf - v) < 0.001f))
            || ((_pudu_debuf < 0) && (v < 0) && ((_pudu_debuf - v) > 0.001f)))
    {
        _pudu_debuf = v;
        _pudu_debuf_last = last;
    }
}

bool BattleFighter::updateHPPAttackAdd(float addP, float hpLostp, float maxCount)
{
#ifdef _BATTLE_DEBUG
    std::cout << "updateHPPAttackAdd: " << addP << "," << hpLostp << "," << maxCount << "." << std::endl;
#endif
    if (maxCount >= 1.0f) // 是否存在触发上限次数
    {
        UInt32 count = (UInt32) maxCount;
        if (count > 0 && _hpAtkAddCount >= count) // 是否已经超过触发上限次数
        {
#ifdef _BATTLE_DEBUG
            std::cout << "count: " << count << "," << _hpAtkAddCount << "." << std::endl << std::endl;
#endif
            return false;
        }
    }

    UInt32 lostHP = 0;
    if (_hp < _maxhp)
        lostHP = _maxhp - _hp;
    float lostP = (float)lostHP / (float)_maxhp;
    UInt32 lostCount2 = (UInt32)(_lastHPLostP / hpLostp);
    UInt32 lostCount = (UInt32)(lostP / hpLostp);
    if (lostCount == lostCount2)
    {
#ifdef _BATTLE_DEBUG
        std::cout << "lostCount: " << lostCount << ", _lastHPLostP = " << _lastHPLostP << "." << std::endl << std::endl;
#endif
        return false;
    }
    _hpMagAtkAdd = _attack * hpLostp * lostCount;
    _hpAtkAdd = _magatk * hpLostp * lostCount;
    ++_hpAtkAddCount;
#ifdef _BATTLE_DEBUG
    std::cout << "updateHPPAttackAdd2: " << _hpAtkAdd << "," << _hpAtkAddCount << "," << lostCount << "." << std::endl << std::endl;
#endif
    return true;
}

bool BattleFighter::updateHPPAttackReduce(float reduceP, float hpLostp, float maxCount)
{
#ifdef _BATTLE_DEBUG
    std::cout << "updateHPPAttackReduce: " << reduceP << "," << hpLostp << "," << maxCount << "." << std::endl;
#endif
    if (maxCount >= 1.0f) // 是否存在触发上限次数
    {
        UInt32 count = (UInt32) maxCount;
        if (count > 0 && _hpAtkReduceCount >= count) // 是否已经超过触发上限次数
        {
#ifdef _BATTLE_DEBUG
            std::cout << "count: " << count << "," << _hpAtkReduceCount << "." << std::endl << std::endl;
#endif
            return false;
        }
    }
    UInt32 lostHP = 0;
    if (_hp < _maxhp)
        lostHP = _maxhp - _hp;
    float lostP = (float)lostHP / (float)_maxhp;
    UInt32 lostCount2 = (UInt32)(_lastHPLostP / hpLostp);
    UInt32 lostCount = (UInt32)(lostP / hpLostp);
    if (lostCount == lostCount2)
    {
#ifdef _BATTLE_DEBUG
        std::cout << "lostCount: " << lostCount << ", _lastHPLostP = " << _lastHPLostP << "." << std::endl << std::endl;
#endif
        return false;
    }
    _hpMagAtkReduce = reduceP * lostCount;
    _hpAtkReduce = reduceP * lostCount;
    ++_hpAtkReduceCount;
#ifdef _BATTLE_DEBUG
    std::cout << "updateHPPAttackReduce2: " << _hpAtkReduce << "," << _hpAtkReduceCount << "," << lostCount << "." << std::endl << std::endl;
#endif
    return true;
}
 
UInt32 BattleFighter::updateHPPRecover(float recoverP, float hpLostp, float maxCount)
{
    // 回复HP
#ifdef _BATTLE_DEBUG
    std::cout << "updateHPPRecover: " << recoverP << "," << hpLostp << "," << maxCount << "." << std::endl;
#endif
    if (maxCount >= 1.0f) // 是否存在触发上限次数
    {
        UInt32 count = (UInt32) maxCount;
        if (count > 0 && _hpRecoverCount >= count) // 是否已经超过触发上限次数
        {
#ifdef _BATTLE_DEBUG
            std::cout << "count: " << count << "," << _hpRecoverCount << "." << std::endl << std::endl;
#endif
            return 0;
        }
    }
    float rhp = 1.0f;
    UInt32 lostHP = 0;
    if (_hp < _maxhp)
        lostHP = _maxhp - _hp;
    float lostP = (float)lostHP / (float)_maxhp;
    UInt32 lostCount2 = (UInt32)(_lastHPLostP / hpLostp);
    UInt32 lostCount = (UInt32)(lostP / hpLostp);
    if (lostCount == lostCount2)
    {
#ifdef _BATTLE_DEBUG
        std::cout << "lostCount: " << lostCount << ", _lastHPLostP = " << _lastHPLostP << "." << std::endl << std::endl;
#endif
        return 0;
    }

    rhp = ((float)_maxhp) * recoverP;
    if (_hp + rhp >= _maxhp)
        rhp = _maxhp - _hp;
    regenHP(rhp);
    ++_hpRecoverCount;
#ifdef _BATTLE_DEBUG
    std::cout << "updateHPPRecover2: " << rhp << "," << _hpRecoverCount << "," << lostCount << "." << std::endl << std::endl;
#endif
    return static_cast<UInt32>(rhp);
}

UInt32 BattleFighter::updateHPPRecover2Fake(float recoverP, float hpLostp, float maxCount)
{
    // 回复HP
#ifdef _BATTLE_DEBUG
    std::cout << "updateHPPRecover: " << recoverP << "," << hpLostp << "," << maxCount << "." << std::endl;
#endif
    if (maxCount >= 1.0f) // 是否存在触发上限次数
    {
        UInt32 count = (UInt32) maxCount;
        if (count > 0 && _hpRecoverCount >= count) // 是否已经超过触发上限次数
        {
#ifdef _BATTLE_DEBUG
            std::cout << "count: " << count << "," << _hpRecoverCount << "." << std::endl << std::endl;
#endif
            return 0;
        }
    }
    float rhp = 1.0f;
    UInt32 lostHP = 0;
    if (_hp < _maxhp)
        lostHP = _maxhp - _hp;
    float lostP = (float)lostHP / (float)_maxhp;
    UInt32 lostCount2 = (UInt32)(_lastHPLostP / hpLostp);
    UInt32 lostCount = (UInt32)(lostP / hpLostp);
    if (lostCount == lostCount2)
    {
#ifdef _BATTLE_DEBUG
        std::cout << "lostCount: " << lostCount << ", _lastHPLostP = " << _lastHPLostP << "." << std::endl << std::endl;
#endif
        return 0;
    }

    rhp = ((float)_maxhp) * recoverP;
    if (_hp + rhp >= _maxhp)
        rhp = _maxhp - _hp;
    ++_hpRecoverCount;
#ifdef _BATTLE_DEBUG
    std::cout << "updateHPPRecover2: " << rhp << "," << _hpRecoverCount << "," << lostCount << "." << std::endl << std::endl;
#endif
    return static_cast<UInt32>(rhp);
}

void    BattleFighter::updateLastHPLostP()
{
    UInt32 lostHP = 0;
    if (_hp < _maxhp)
        lostHP = _maxhp - _hp;
    _lastHPLostP = (float)lostHP / (float)_maxhp;
}
 

void BattleFighter::makeDamage( UInt32& u )
{
    if(_colorStockTimes > 0)
    {
        u = 0;
        -- _colorStockTimes;
        return;
    }
    if(isSoulOut())
    {
        u = 0;
        return;
    }
    if(_lingqu_times > 0)
    {
        -- _lingqu_times;
        if(_lingqu_times > 0)
            u = 0;
        else
            _hp = 0;
        return;
    }

    addDarkVigor(u);
	if(_hp < u)
		_hp = 0;
	else
		_hp -= u;
}

bool BattleFighter::makeShieldDamage(UInt32& u)
{
    BattleFighter* shieldObj = static_cast<BattleFighter*>(getShieldObj());
    if(!shieldObj)
        return false;

    float& shieldHP = shieldObj->getShieldHPBuf();
    if(shieldHP < 0.001f)
        return false;
    if(shieldHP < u)
    {
        u -= shieldHP;
        shieldObj->setShieldHPBuf(0, 0);
        setShieldObj(NULL);
    }
    else
    {
        shieldHP -= u;
        u = 0;
    }

    return true;
}

BattleFighter* BattleFighter::summonSelf(float factor, UInt8 last)
{
    if(last == 0)
        return NULL;

    BattleFighter * bf = new(std::nothrow) BattleFighter(_formula, _fighter);
    //BattleFighter* bf = new BattleFighter(*this);
    if(!bf)
        return NULL;

    bf->initStats(false);
    bf->clearSkill();

    UInt32 aura = 0;
    if(isHide())
        aura = 100;

    bf->setSneakAtk(_sneak_atk, _sneak_atk_status, _sneak_atk_last);
    bf->setRecoverSnakeAtk(_sneak_atk_recover_rate);

    bf->setSummonFactor(aura, factor, last);
    setSelfSummon(bf);
    bf->setXiangMoChanZhangSkill(NULL);
    bf->setBiLanTianYiSkill(NULL);
    return bf;
}

void BattleFighter::clearSkill()
{
    memset(&_peerlessSkill, 0, sizeof(_peerlessSkill));
    _activeSkill.clear();
    _therapySkill.clear();
    _passiveSkillPreAtk100.clear();
    _passiveSkillAftAtk100.clear();
    _passiveSkillAftAction100.clear();
    _passiveSkillBeAtk100.clear();
    _passiveSkillAftEvd100.clear();
    _passiveSkillAftRes100.clear();
    _passiveSkillEnter100.clear();
    _passiveSkillDead100.clear();
    _passiveSkillAftNAtk100.clear();
    _passiveSkillOnCounter100.clear();
    _passiveSkillOnAttackBleed100.clear();
    _passiveSkillOnAtkDmg100.clear();
    _passiveSkillOnGetDmg100.clear();
    _passiveSkillOnBeDmg100.clear();
    _passiveSkillOnBePHYDmg100.clear();
    _passiveSkillOnBeMagDmg100.clear();
    _passiveSkillOnHP10P100.clear();
    _passiveSkillDeadFake100.clear();
    _passiveSkillAbnormalTypeDmg100.clear();
    _passiveSkillBleedTypeDmg100.clear();
    _passiveSkillXMCZ100.clear();
    _passiveSkillBLTY100.clear();
    _passiveSkillViolent100.clear();
    _passiveSkillRevival100.clear();
    _passiveSkillLingshi100.clear();
    _passiveSkillEnterLingshi100.clear();
    _passiveSkillOnHPChange100.clear();
    _passiveSkillOnWithstand100.clear();

    _passiveSkillPreAtk.clear();
    _passiveSkillAftAtk.clear();
    _passiveSkillAftAction.clear();
    _passiveSkillBeAtk.clear();
    _passiveSkillAftEvd.clear();
    _passiveSkillAftRes.clear();
    _passiveSkillEnter.clear();
    _passiveSkillDead.clear();
    _passiveSkillSoulProtect.clear();
    _passiveSkillAftNAtk.clear();
    _passiveSkillOnCounter.clear();
    _passiveSkillOnGetDmg.clear();
    _passiveSkillOnBeDmg.clear();
    _passiveSkillOnBePHYDmg.clear();
    _passiveSkillOnBeMagDmg.clear();
    _passiveSkillOnHP10P.clear();
    _passiveSkillDeadFake.clear();
    _passiveSkillBleedTypeDmg.clear();
    _passiveSkillOnHPChange.clear();
    _passiveSkillOnWithstand.clear();

    _passiveSkillOnTherapy.clear();
    _passiveSkillOnSkillDmg.clear();
    _passiveSkillOnOtherDead.clear();

    _skillStrengthen.clear();

    _onSkillCond.clear();
    _onActionCond1.clear();
    _onActionCond2.clear();
    _onDeadCond.clear();
    _onBleedCond.clear();
    _onStateCond.clear();
    _sg_v.clear();
}

void BattleFighter::setSummonFactor(UInt32 aura, float factor, UInt8 last)
{
    if(last == 0)
        return; 
    _attack = _attack * factor;
    _magatk = _magatk * factor;
    _maxhp = _maxhp * factor;
    _hp = _maxhp;

    _aura = aura;

    _peerlessSkill.base = GData::skillManager[49509];

    _summon = true;
    _summonLast = last;
}

bool BattleFighter::releaseSummon()
{
    if(_summonLast == 0 || !_summon)
        return false;

    -- _summonLast;
    if(_summonLast == 0)
    {
        _hp = 0;
        return true;
    }

    return false;
}

bool BattleFighter::releaseMoAuraBuf()
{
    if(_moAuraBufLast == 0 || _moAuraBuf == 0)
        return false;
    -- _moAuraBufLast;
    if(_moAuraBufLast == 0)
    {
        _moAuraBuf = 0;
        return true;
    }

    return false;
}

bool BattleFighter::getMoEvade100()
{
    bool ret = _moEvade100;
    releaseMoEvade100();
    return ret;
}

bool BattleFighter::releaseMoEvade100()
{
    if(_moEvade100Last == 0 || !_moEvade100)
        return false;
    -- _moEvade100Last;
    if(_moEvade100Last == 0)
    {
        _moEvade100 = false;
        return true;
    }

    return false;
}

void BattleFighter::setHideBuf(UInt8 value, UInt8 last)
{
    if(last == 0)
        return;

    _hideBuf = value;
    _hideBufLast = last;
    setHide(_hideBuf);
}
bool BattleFighter::releaseHideBuf()
{
    if(_hideBufLast == 0 || !_hideBuf)
        return false;
    -- _hideBufLast;
    if(_hideBufLast == 0)
    {
        _hideBuf = false;
        setHide(false);
        _shieldHP = 0;
        _shieldHPLast = 0;
        return true;
    }
    return false;
}

bool BattleFighter::releaseMarkMo()
{
    if(_markMoLast == 0 || !_markMo)
        return false;
    -- _markMoLast;
    if(_markMoLast == 0)
    {
        _markMo = false;
        return true;
    }
    return false;
}

bool BattleFighter::releaseBlind()
{
    if(_blind_last == 0 || _blind < 0.001f)
        return false;
    -- _blind_last;
    if(_blind_last == 0)
    {
        _blind = 0;
        _deep_blind_last = 0;
        _deep_blind_dmg_extra = 0;
        return true;
    }
    return false;
}

bool BattleFighter::releaseDeepBlind()
{
    if(_deep_blind_last == 0 || _deep_blind_dmg_extra < 0.001f)
        return false;
    -- _deep_blind_last;
    if(_deep_blind_last == 0)
    {
        _deep_blind_dmg_extra = 0;
        return true;
    }
    return false;
}

bool BattleFighter::releaseMoAttackAdd()
{
    if(_moAttackAddCD == 0 || _moAttackAdd < 0.001)
        return false;
    -- _moAttackAddCD;
    if(_moAttackAddCD == 0)
    {
        _moAttackAdd = 0.0f;
        return true;
    }
    return false;
}

bool BattleFighter::releaseMoMagAtkAdd()
{
    if(_moMagAtkAddCD == 0 || _moMagAtkAdd < 0.001)
        return false;
    -- _moMagAtkAddCD;
    if(_moMagAtkAddCD == 0)
    {
        _moMagAtkAdd = 0.0f;
        return true;
    }
    return false;
}

bool BattleFighter::releaseMoAtkReduce()
{
    if(_moAtkReduceCD == 0 || _moAtkReduce < 0.001)
        return false;
    -- _moAtkReduceCD;
    if(_moAtkReduceCD == 0)
    {
        _moAtkReduce = 0.0f;
        return true;
    }
    return false;
}

bool BattleFighter::releaseMoMagAtkReduce()
{
    if(_moMagAtkReduceCD == 0 || _moMagAtkReduce < 0.001)
        return false;
    -- _moMagAtkReduceCD;
    if(_moMagAtkReduceCD == 0)
    {
        _moMagAtkReduce = 0.0f;
        return true;
    }
    return false;
}

bool BattleFighter::releasePetAttackAdd()
{
    if(_petAttackAddCD == 0 || _petAttackAdd < 0.001)
        return false;
    -- _petAttackAddCD;
    if(_petAttackAddCD == 0)
    {
        _petAttackAdd = 0.0f;
        return true;
    }
    return false;
}

bool BattleFighter::releasePetMagAtkAdd()
{
    if(_petMagAtkAddCD == 0 || _petMagAtkAdd < 0.001)
        return false;
    -- _petMagAtkAddCD;
    if(_petMagAtkAddCD == 0)
    {
        _petMagAtkAdd = 0.0f;
        return true;
    }
    return false;
}

bool BattleFighter::releasePetAtkReduce()
{
    if(_petAtkReduceCD == 0 || _petAtkReduce < 0.001)
        return false;
    -- _petAtkReduceCD;
    if(_petAtkReduceCD == 0)
    {
        _petAtkReduce = 0.0f;
        return true;
    }
    return false;
}

bool BattleFighter::releasePetMagAtkReduce()
{
    if(_petMagAtkReduceCD == 0 || _petMagAtkReduce < 0.001)
        return false;
    -- _petMagAtkReduceCD;
    if(_petMagAtkReduceCD == 0)
    {
        _petMagAtkReduce = 0.0f;
        return true;
    }
    return false;
}

bool BattleFighter::releaseBleedMo()
{
    if(_bleedMoLast == 0 || _bleedMo < 0.001)
        return false;
    -- _bleedMoLast;
    if(_bleedMoLast == 0)
    {
        _bleedMo = 0;
        return true;
    }
    return false;
}

void BattleFighter::setShieldHPBuf(float value, UInt8 last)
{
    _shieldHP = value;
    _shieldHPLast = last;
}

bool BattleFighter::releaseShieldHPBuf()
{
    if(_shieldHPLast == 0 || _shieldHP < 0.001)
        return false;
    -- _shieldHPLast;
    if(_shieldHPLast== 0)
    {
        _shieldHP = 0;
        return true;
    }
    return false;
}

void BattleFighter::setDarkVigor(float value, UInt8 last)
{
    if(_darkVigorLast != 0)
        return;

    _dvFactor = value;
    _darkVigorLast = last;
    _darkVigor = 0;
}

void BattleFighter::addDarkVigor(float value)
{
    if(_darkVigorLast == 0)
        return;

    _darkVigor += value * _dvFactor;
}

bool BattleFighter::releaseDarkVigor()
{
    if(_darkVigorLast == 0 || _darkVigor < 0.001)
        return false;
    -- _darkVigorLast;
    if(_darkVigorLast == 0)
    {
        _darkVigor = 0;
        _dvFactor = 0;
        return true;
    }

    return false;
}

GData::LBSkillItem* BattleFighter::getSkillCondItem(UInt16 skillid)
{
    GData::LBSkillItem* item = NULL;
    if(skillid == 0)
        return item;

    UInt8 cnt = _onSkillCond.size();
    for(int i = 0; i < cnt; ++ i)
    {
        if(_onSkillCond[i].base->cond2 == skillid)
        {
            item = &(_onSkillCond[i]);
            break;
        }
    }

    if(item && item->prob > 0.01f)
    {
        if(item->prob > uRand(10000))
            item->cd = item->base->cd;
        else
            item = NULL;
    }
    return item;
}

GData::LBSkillItem* BattleFighter::getActionCondItem1()
{
    GData::LBSkillItem* item = NULL;
    UInt8 cnt = _onActionCond1.size();
    for(int i = 0; i < cnt; ++ i)
    {
        item = &(_onActionCond1[i]);
        break;
    }

    if(item && item->prob > 0.01f)
    {
        if(item->prob > uRand(10000))
            item->cd = item->base->cd;
        else
            item = NULL;
    }
    return item;
}

GData::LBSkillItem* BattleFighter::getActionCondItem2()
{
    GData::LBSkillItem* item = NULL;
    UInt8 cnt = _onActionCond2.size();
    for(int i = 0; i < cnt; ++ i)
    {
        item = &(_onActionCond2[i]);
        break;
    }

    if(item && item->prob > 0.01f)
    {
        if(item->prob > uRand(10000))
            item->cd = item->base->cd;
        else
            item = NULL;
    }
    return item;
}

GData::LBSkillItem* BattleFighter::getDeadCondItem()
{
    GData::LBSkillItem* item = NULL;

    UInt8 cnt = _onDeadCond.size();
    for(int i = 0; i < cnt; ++ i)
    {
        item = &(_onDeadCond[i]);
        break;
    }

    if(item && item->prob > 0.01f)
    {
        if(item->prob > uRand(10000))
            item->cd = item->base->cd;
        else
            item = NULL;
    }
    return item;
}

GData::LBSkillItem* BattleFighter::getBleedCondItem()
{
    GData::LBSkillItem* item = NULL;

    UInt8 cnt = _onBleedCond.size();
    for(int i = 0; i < cnt; ++ i)
    {
        if(_onBleedCond[i].cd > 0)
            continue;

        item = &(_onBleedCond[i]);
        break;
    }

    if(item && item->prob > 0.01f)
    {
        if(item->prob > uRand(10000))
            item->cd = item->base->cd;
        else
            item = NULL;
    }
    return item;
}

GData::LBSkillItem* BattleFighter::getStateCondItem(UInt16 state)
{
    GData::LBSkillItem* item = NULL;

    UInt8 cnt = _onStateCond.size();
    for(int i = 0; i < cnt; ++ i)
    {
        if(_onStateCond[i].cd > 0)
            continue;
        if(!(_onStateCond[i].base->cond2 & state))
            continue;

        item = &(_onStateCond[i]);
        break;
    }

    if(item && item->prob > 0.01f)
    {
        if(item->prob > uRand(10000))
            item->cd = item->base->cd;
        else
            item = NULL;
    }
    return item;
}

void BattleFighter::releaseLBSkillCD()
{
    UInt8 cnt = _onBleedCond.size();
    UInt8 idx = 0;
    for(idx = 0; idx < cnt; ++ idx)
    {
        GData::LBSkillItem& item = _onBleedCond[idx];
        if(item.cd > 0)
            -- item.cd;
    }

    cnt = _onStateCond.size();
    for(idx = 0; idx < cnt; ++ idx)
    {
        GData::LBSkillItem& item = _onStateCond[idx];
        if(item.cd > 0)
            -- item.cd;
    }
}

bool BattleFighter::releaseHpSieldSelf()
{
    if(_hpShieldSelf_last == 0)
        return false;

    -- _hpShieldSelf_last;
    if(_hpShieldSelf_last == 0)
    {
        _hpShieldSelf = 0;
        return true;
    }

    return false;
}

bool BattleFighter::releasePetAtk100()
{
    if(_petAtk100Last == 0)
        return false;

    -- _petAtk100Last;
    if(_petAtk100Last == 0)
    {
        _hpShieldSelf = 0;
        return true;
    }

    return false;
}

void BattleFighter::addCounterSpiritBuf(float atk, float magatk, float def, float magdef, UInt8 last)
{
    if(_counter_spirit_times < 3)
    {
        ++ _counter_spirit_times;
        _counter_spirit_atk_add += atk;
        _counter_spirit_magatk_add += magatk;
        _counter_spirit_def_add += def;
        _counter_spirit_magdef_add += magdef;
    }
    _counter_spirit_last = last;

    return;
}

bool BattleFighter::releaseCounterSpirit()
{
    if(_counter_spirit_skill_cd != 0)
        --_counter_spirit_skill_cd;

    if(_counter_spirit_last == 0)
        return false;

    -- _counter_spirit_last;
    if(_counter_spirit_last != 0)
        return false;

    _counter_spirit_times = 0;
    _counter_spirit_atk_add = 0;
    _counter_spirit_magatk_add = 0;
    _counter_spirit_def_add = 0;
    _counter_spirit_magdef_add = 0;
    return true;
}

void BattleFighter::setCounterSpiritSkill(UInt16 skillid, float efv, const std::vector<float>& factor)
{
    if(_counter_spirit_skill_cd == 0)
    {
        _counter_spirit_efv = efv;
        _counter_spirit_factor = factor;
        _counter_spirit_skillid = skillid;
        ++ _counter_spirit_skill_cd;
    }
}

void BattleFighter::clearCounterSpiritSkill()
{
    _counter_spirit_efv = 0.0f;
    _counter_spirit_factor.clear();
    _counter_spirit_skillid = 0;
}

float BattleFighter::getCounterSpiritAtk()
{
    float lostHP = getLostHP();
    if(lostHP * 10 < getMaxHP())
        lostHP = getMaxHP() / 10;

    return (_counter_spirit_efv * lostHP);
}

bool BattleFighter::releaseFireDefend()
{
    if(_fire_defend_last == 0)
        return false;

    -- _fire_defend_last;
    if(_fire_defend_last != 0)
        return false;

    _fire_defend = 0;
    return true;
}

bool BattleFighter::doFireFakeDead()
{
    if(_fire_fake_dead_rate_last == 0)
        return false;

    if(uRand(10000) < _fire_fake_dead_rate * 100)
    {
        _fire_fake_dead_rate = 0;
        _fire_fake_dead_rate_last = 0;
        _hp = 1;
        return true;
    }

    return false;
}

bool BattleFighter::releaseFireFakeDead()
{
    if(_fire_fake_dead_rate_last == 0)
        return false;

    -- _fire_fake_dead_rate_last;
    if(_fire_fake_dead_rate_last != 0)
        return false;

    _fire_fake_dead_rate = 0;
    return true;
}

void BattleFighter::nextSneakStatus()
{
    switch(_sneak_atk_status)
    {
    case e_sneak_none:
        break;
    case e_sneak_on:
        _sneak_atk_status = e_sneak_atk;
        break;
    case e_sneak_atk:
        _sneak_atk_status = e_sneak_none;
        break;
    }

    return;
}

bool BattleFighter::releaseSneakAtk()
{
    if(_sneak_atk_last == 0)
        return false;

     -- _sneak_atk_last;
     if(_sneak_atk_last != 0)
         return false;

     _sneak_atk_status = e_sneak_none;
     _sneak_atk_recover_rate = 0;
     _sneak_atk = 0;

     return true;
}

bool BattleFighter::recoverSneakAtk()
{
    if(_sneak_atk_status != e_sneak_none || _sneak_atk_last == 0)
        return false;

    if(_sneak_atk_recover_rate > uRand(10000))
    {
        _sneak_atk_status = e_sneak_on;
        _sneak_atk_recover_rate *= 0.5f;
        return true;
    }

    return false;
}

void BattleFighter::updateSoulSkillDead(UInt16 skillId)
{
    if(skillId == 0)
        return;

    GData::SkillItem skillItem;
    skillItem.base = GData::skillManager[skillId];
    if(skillItem.base == NULL)
        return;

    skillItem.cd = 0;
    if(skillItem.base->prob > 99.999f)
    {
        skillItem.rateExtent = 0;
        _passiveSkillDead100.insert(_passiveSkillDead100.end(), skillItem);
    }
    else
    {
        size_t cnt = _passiveSkillDead.size();
        if(cnt > 0)
            skillItem.rateExtent = _passiveSkillDead[cnt-1].rateExtent + skillItem.base->prob * 100;
        else
            skillItem.rateExtent = skillItem.base->prob * 100;
        _passiveSkillDead.insert(_passiveSkillDead.end(), skillItem);
    }
}

void BattleFighter::updateSoulSkillProtect(UInt16 skillId)
{
    if(skillId == 0)
        return;
    GData::SkillItem skillItem;
    skillItem.base = GData::skillManager[skillId];
    if(skillItem.base == NULL)
        return;
    if(_passiveSkillSoulProtect.size() > 0)
        return;
    _passiveSkillSoulProtect.insert(_passiveSkillSoulProtect.end(), skillItem);
    setSoulProtectLast(1);
}

void BattleFighter::updatePassiveSkillPrvAtk100Status()
{
    _xiangMoChanZhangSkill = NULL;
    const GData::SkillBase* passiveSkill = NULL;
    size_t skillIdx = 0;
    while(NULL != (passiveSkill = getPassiveSkillXMCZ100(skillIdx)))
    {
        _xiangMoChanZhangSkill = passiveSkill;
        break;
    }
}

void BattleFighter::updatePassiveSkillBLTY100Status()
{
    _biLanTianYiSkill = NULL;
    const GData::SkillBase* passiveSkill = NULL;
    size_t skillIdx = 0;
    while(NULL != (passiveSkill = getPassiveSkillBLTY100(skillIdx)))
    {
        _biLanTianYiSkill = passiveSkill;
        break;
    }
}

void BattleFighter::addPassiveSkill100(std::vector<UInt16>& passiveSkill100Id, std::vector<GData::SkillItem>& passiveSkill100)
{
    for (std::vector<UInt16>::iterator it0 = passiveSkill100Id.begin(); it0!= passiveSkill100Id.end(); ++it0)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[*it0];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        passiveSkill100.push_back(skillItem);

        updateSkillStrengthen(*it0);
    }
}

void BattleFighter::addPassiveSkill(std::vector<UInt16>& passiveSkillId, std::vector<GData::SkillItem>& passiveSkill)
{
    for (std::vector<UInt16>::iterator it0 = passiveSkillId.begin(); it0!= passiveSkillId.end(); ++it0)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[*it0];
        skillItem.cd = 0;
        float rateExtent = skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        passiveSkill.push_back(skillItem);

        updateSkillStrengthen(*it0);
    }
}

void BattleFighter::removePassiveSkill100(std::vector<UInt16>& passiveSkill100Id, std::vector<GData::SkillItem>& passiveSkill100)
{
    for (std::vector<UInt16>::iterator it0 = passiveSkill100Id.begin(); it0!= passiveSkill100Id.end(); ++it0)
    {
        for(std::vector<GData::SkillItem>::iterator it = passiveSkill100.begin(); it != passiveSkill100.end(); ++it)
        {
            if ((*it).base && ((*it).base->getId() == *it0))
            {
                passiveSkill100.erase(it);
                break;
            }
        }
    }
}

void BattleFighter::removePassiveSkill(std::vector<UInt16>& passiveSkillId, std::vector<GData::SkillItem>& passiveSkill)
{
    for (std::vector<UInt16>::iterator it0 = passiveSkillId.begin(); it0!= passiveSkillId.end(); ++it0)
    {
        for(std::vector<GData::SkillItem>::iterator it = passiveSkill.begin(); it != passiveSkill.end(); ++it)
        {
            if ((*it).base && ((*it).base->getId() == *it0))
            {
                passiveSkill.erase(it);
                break;
            }
        }
    }
}

void BattleFighter::updatePassiveSkill100(std::vector<UInt16>& passiveSkill100Id, std::vector<GData::SkillItem>& passiveSkill100)
{
    size_t idx;
    size_t cnt = passiveSkill100Id.size();
    passiveSkill100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkill100Id[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        passiveSkill100.insert(passiveSkill100.end(), skillItem);

        updateSkillStrengthen(passiveSkill100Id[idx]);
    }
}

void BattleFighter::updatePassiveSkill(std::vector<UInt16>& passiveSkillId, std::vector<GData::SkillItem>& passiveSkill)
{
    size_t idx;
    size_t cnt = passiveSkillId.size();
    passiveSkill.clear();
    float rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillId[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        passiveSkill.insert(passiveSkill.end(), skillItem);

        updateSkillStrengthen(passiveSkillId[idx]);
    }
}

void BattleFighter::updateAllPassiveSkillLingshi()
{
    for (UInt8 type = GData::SKILL_PASSSTART; type < GData::SKILL_PASSIVES; ++ type)
    {
        updatePassiveSkillLingshi100(type);
        updatePassiveSkillLingshi(type);
    }
}

void BattleFighter::updateAllPassiveSkillLingshiExceptEnter()
{
    for (UInt8 type = GData::SKILL_PASSSTART; type < GData::SKILL_PASSIVES; ++ type)
    {
        if(type == GData::SKILL_ENTER_LINGSHI)
            continue;
        updatePassiveSkillLingshi100(type);
        updatePassiveSkillLingshi(type);
    }
}

void BattleFighter::updatePassiveSkillLingshi100(UInt8 type)
{
    switch (type)
    {
        case GData::SKILL_PREATK:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillPreAtk100);
            break;
        case GData::SKILL_AFTATK:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftAtk100);
            break;
        case GData::SKILL_BEATKED:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillBeAtk100);
            break;
        case GData::SKILL_AFTEVD:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftEvd100);
            break;
        case GData::SKILL_AFTRES:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftRes100);
            break;
        case GData::SKILL_ENTER:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillEnter100);
            break;
        case GData::SKILL_DEAD:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnOtherDead);
            break;
        case GData::SKILL_AFTNATK:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftNAtk100);
            break;
        case GData::SKILL_ONTHERAPY:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnTherapy);
            break;
        case GData::SKILL_ONSKILLDMG:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnSkillDmg);
            break;
        case GData::SKILL_ONOTHERDEAD:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnOtherDead);
            break;
        case GData::SKILL_ONCOUNTER:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnCounter100);
            break;
        case GData::SKILL_ONATKBLEED:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnAttackBleed100);
            break;
        case GData::SKILL_ONATKDMG:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnAtkDmg100);
            break;
        case GData::SKILL_ONPETPROTECT:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnPetProtect100);
            break;
        case GData::SKILL_ONGETDMG:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnGetDmg100);
            break;
        case GData::SKILL_ONBEDMG:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnBeDmg100);
            break;
        case GData::SKILL_ONBEPHYDMG:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnBePHYDmg100);
            break;
        case GData::SKILL_ONBEMAGDMG:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnBeMagDmg100);
            break;
        case GData::SKILL_ONHP10P:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnHP10P100);
            break;
        case GData::SKILL_DEAD_FAKE:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillDeadFake100);
            break;
        case GData::SKILL_ABNORMAL_TYPE_DMG:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAbnormalTypeDmg100);
            break;
        case GData::SKILL_BLEED_TYPE_DMG:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillBleedTypeDmg100);
            break;
        case GData::SKILL_XMCZ:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillXMCZ100);
            break;
        case GData::SKILL_BLTY:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillBLTY100);
            break;
        case GData::SKILL_AFTACTION:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftAction100);
            break;
        case GData::SKILL_ONHPCHANGE:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnHPChange100);
            break;
        case GData::SKILL_ONWITHSTAND:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnWithstand100);
            break;
        case GData::SKILL_VIOLENT:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillViolent100);
            break;
        case GData::SKILL_REVIVAL:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillRevival100);
            break;
        case GData::SKILL_LINGSHI:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillLingshi100);
            break;
        case GData::SKILL_ONOTHERCONFUSEFORGET:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnOtherConfuseForget100);
            break;
        case GData::SKILL_ENTER_LINGSHI:
            addPassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillEnterLingshi100);
            break;
        case GData::SKILL_ONATKSTUN:
            addPassiveSkill(_allPassiveSkillLingshi100[type], _passiveSkillOnAttackStun100);
            break;
        case GData::SKILL_ONATKCONFUSEFORGET:
            addPassiveSkill(_allPassiveSkillLingshi100[type], _passiveSkillOnAttackConfuseForget100);
            break;
        case GData::SKILL_ONATKBLIND:
            addPassiveSkill(_allPassiveSkillLingshi100[type], _passiveSkillOnAttackBlind100);
            break;
    }
}

void BattleFighter::updatePassiveSkillLingshi(UInt8 type)
{
    switch (type)
    {
        case GData::SKILL_PREATK:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillPreAtk);
            break;
        case GData::SKILL_AFTATK:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftAtk);
            break;
        case GData::SKILL_BEATKED:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillBeAtk);
            break;
        case GData::SKILL_AFTEVD:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftEvd);
            break;
        case GData::SKILL_AFTRES:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftRes);
            break;
        case GData::SKILL_ENTER:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillEnter);
            break;
        case GData::SKILL_DEAD:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnOtherDead);
            break;
        case GData::SKILL_AFTNATK:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftNAtk);
            break;
        case GData::SKILL_ONTHERAPY:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnTherapy);
            break;
        case GData::SKILL_ONSKILLDMG:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnSkillDmg);
            break;
        case GData::SKILL_ONOTHERDEAD:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnOtherDead);
            break;
        case GData::SKILL_ONCOUNTER:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnCounter);
            break;
        case GData::SKILL_ONATKBLEED:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnAttackBleed);
            break;
        case GData::SKILL_ONATKDMG:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnAtkDmg);
            break;
        case GData::SKILL_ONPETPROTECT:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnPetProtect);
            break;
        case GData::SKILL_ONGETDMG:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnGetDmg);
            break;
        case GData::SKILL_ONBEDMG:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnBeDmg);
            break;
        case GData::SKILL_ONBEPHYDMG:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnBePHYDmg);
            break;
        case GData::SKILL_ONBEMAGDMG:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnBeMagDmg);
            break;
        case GData::SKILL_ONHP10P:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnHP10P);
            break;
        case GData::SKILL_DEAD_FAKE:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillDeadFake);
            break;
        case GData::SKILL_ABNORMAL_TYPE_DMG:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAbnormalTypeDmg);
            break;
        case GData::SKILL_BLEED_TYPE_DMG:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillBleedTypeDmg);
            break;
        case GData::SKILL_XMCZ:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillXMCZ);
            break;
        case GData::SKILL_BLTY:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillBLTY);
            break;
        case GData::SKILL_AFTACTION:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftAction);
            break;
        case GData::SKILL_ONHPCHANGE:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnHPChange);
            break;
        case GData::SKILL_ONWITHSTAND:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnWithstand);
            break;
        case GData::SKILL_VIOLENT:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillViolent);
            break;
        case GData::SKILL_REVIVAL:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillRevival);
            break;
        case GData::SKILL_LINGSHI:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillLingshi);
            break;
        case GData::SKILL_ONOTHERCONFUSEFORGET:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnOtherConfuseForget);
            break;
        case GData::SKILL_ENTER_LINGSHI:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillEnterLingshi);
            break;
        case GData::SKILL_ONATKSTUN:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnAttackStun);
            break;
        case GData::SKILL_ONATKCONFUSEFORGET:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnAttackConfuseForget);
            break;
        case GData::SKILL_ONATKBLIND:
            addPassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnAttackBlind);
            break;
    }
}

void BattleFighter::unUpdateAllPassiveSkillLingshi()
{
    for (UInt8 type = GData::SKILL_PASSSTART; type < GData::SKILL_PASSIVES; ++ type)
    {
        unUpdatePassiveSkillLingshi100(type);
        unUpdatePassiveSkillLingshi(type);
    }
}

void BattleFighter::unUpdatePassiveSkillLingshi(UInt8 type)
{
    switch (type)
    {
        case GData::SKILL_PREATK:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillPreAtk);
            break;
        case GData::SKILL_AFTATK:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftAtk);
            break;
        case GData::SKILL_BEATKED:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillBeAtk);
            break;
        case GData::SKILL_AFTEVD:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftEvd);
            break;
        case GData::SKILL_AFTRES:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftRes);
            break;
        case GData::SKILL_ENTER:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillEnter);
            break;
        case GData::SKILL_DEAD:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnOtherDead);
            break;
        case GData::SKILL_AFTNATK:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftNAtk);
            break;
        case GData::SKILL_ONTHERAPY:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnTherapy);
            break;
        case GData::SKILL_ONSKILLDMG:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnSkillDmg);
            break;
        case GData::SKILL_ONOTHERDEAD:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnOtherDead);
            break;
        case GData::SKILL_ONCOUNTER:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnCounter);
            break;
        case GData::SKILL_ONATKBLEED:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnAttackBleed);
            break;
        case GData::SKILL_ONATKDMG:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnAtkDmg);
            break;
        case GData::SKILL_ONPETPROTECT:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnPetProtect);
            break;
        case GData::SKILL_ONGETDMG:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnGetDmg);
            break;
        case GData::SKILL_ONBEDMG:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnBeDmg);
            break;
        case GData::SKILL_ONBEPHYDMG:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnBePHYDmg);
            break;
        case GData::SKILL_ONBEMAGDMG:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnBeMagDmg);
            break;
        case GData::SKILL_ONHP10P:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnHP10P);
            break;
        case GData::SKILL_DEAD_FAKE:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillDeadFake);
            break;
        case GData::SKILL_ABNORMAL_TYPE_DMG:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAbnormalTypeDmg);
            break;
        case GData::SKILL_BLEED_TYPE_DMG:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillBleedTypeDmg);
            break;
        case GData::SKILL_XMCZ:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillXMCZ);
            break;
        case GData::SKILL_BLTY:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillBLTY);
            break;
        case GData::SKILL_AFTACTION:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillAftAction);
            break;
        case GData::SKILL_ONHPCHANGE:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnHPChange);
            break;
        case GData::SKILL_ONWITHSTAND:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnWithstand);
            break;
        case GData::SKILL_VIOLENT:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillViolent);
            break;
        case GData::SKILL_REVIVAL:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillRevival);
            break;
        case GData::SKILL_LINGSHI:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillLingshi);
            break;
        case GData::SKILL_ENTER_LINGSHI:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillEnterLingshi);
            break;
        case GData::SKILL_ONOTHERCONFUSEFORGET:
            removePassiveSkill(_allPassiveSkillLingshi[type], _passiveSkillOnOtherConfuseForget);
            break;
    }
}

void BattleFighter::unUpdatePassiveSkillLingshi100(UInt8 type)
{
    switch (type)
    {
        case GData::SKILL_PREATK:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillPreAtk100);
            break;
        case GData::SKILL_AFTATK:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftAtk100);
            break;
        case GData::SKILL_BEATKED:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillBeAtk100);
            break;
        case GData::SKILL_AFTEVD:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftEvd100);
            break;
        case GData::SKILL_AFTRES:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftRes100);
            break;
        case GData::SKILL_ENTER:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillEnter100);
            break;
        case GData::SKILL_DEAD:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnOtherDead);
            break;
        case GData::SKILL_AFTNATK:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftNAtk100);
            break;
        case GData::SKILL_ONTHERAPY:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnTherapy);
            break;
        case GData::SKILL_ONSKILLDMG:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnSkillDmg);
            break;
        case GData::SKILL_ONOTHERDEAD:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnOtherDead);
            break;
        case GData::SKILL_ONCOUNTER:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnCounter100);
            break;
        case GData::SKILL_ONATKBLEED:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnAttackBleed100);
            break;
        case GData::SKILL_ONATKDMG:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnAtkDmg100);
            break;
        case GData::SKILL_ONPETPROTECT:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnPetProtect100);
            break;
        case GData::SKILL_ONGETDMG:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnGetDmg100);
            break;
        case GData::SKILL_ONBEDMG:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnBeDmg100);
            break;
        case GData::SKILL_ONBEPHYDMG:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnBePHYDmg100);
            break;
        case GData::SKILL_ONBEMAGDMG:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnBeMagDmg100);
            break;
        case GData::SKILL_ONHP10P:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnHP10P100);
            break;
        case GData::SKILL_DEAD_FAKE:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillDeadFake100);
            break;
        case GData::SKILL_ABNORMAL_TYPE_DMG:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAbnormalTypeDmg100);
            break;
        case GData::SKILL_BLEED_TYPE_DMG:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillBleedTypeDmg100);
            break;
        case GData::SKILL_XMCZ:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillXMCZ100);
            break;
        case GData::SKILL_BLTY:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillBLTY100);
            break;
        case GData::SKILL_AFTACTION:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillAftAction100);
            break;
        case GData::SKILL_ONHPCHANGE:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnHPChange100);
            break;
        case GData::SKILL_ONWITHSTAND:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnWithstand100);
            break;
        case GData::SKILL_VIOLENT:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillViolent100);
            break;
        case GData::SKILL_REVIVAL:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillRevival100);
            break;
        case GData::SKILL_LINGSHI:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillLingshi100);
            break;
        case GData::SKILL_ENTER_LINGSHI:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillEnterLingshi100);
            break;
        case GData::SKILL_ONOTHERCONFUSEFORGET:
            removePassiveSkill100(_allPassiveSkillLingshi100[type], _passiveSkillOnOtherConfuseForget100);
            break;
    }
}

bool BattleFighter::releaseLingQu()
{
    if(_lingqu_last == 0)
        return false;

    -- _lingqu_last;
    if(_lingqu_last == 0)
    {
        _lingqu_times = 0;
		_hp = 0;
        return true;
    }

    return false;
}

bool BattleFighter::releaseLingShiBleed()
{
    if(_lingshi_bleed_last == 0)
        return false;
    -- _lingshi_bleed_last;
    if(_lingshi_bleed_last == 0)
    {
        _lingshi_bleed = 0;
        return true;
    }
    return false;
}

bool BattleFighter::releaseSoulOut()
{
    if(_soulout_last == 0)
        return false;
    -- _soulout_last;
    if(_soulout_last == 0)
    {
        _hp = 0;
        return true;
    }

    return false;
}

const GData::SkillBase* BattleFighter::get2ndCoAtkSkill()
{
    if(!_2ndCoAtkSkill)
        return NULL;

    if(_2ndRateCoAtk * 100 > uRand(10000))
        return _2ndCoAtkSkill;
    return NULL;
}

void BattleFighter::set2ndCoAtkSkill(float rate, const GData::SkillBase* pskill)
{
    _2ndRateCoAtk = rate;
    _2ndCoAtkSkill = pskill;
}

const GData::SkillBase* BattleFighter::get2ndProtectSkill()
{
    if(!_2ndProtectSkill)
        return NULL;

    if(_2ndRateProtect * 100 > uRand(10000))
        return _2ndProtectSkill;
    return NULL;
}

void BattleFighter::set2ndProtectSkill(float rate, const GData::SkillBase* pskill)
{
    _2ndRateProtect = rate;
    _2ndProtectSkill = pskill;
}

bool BattleFighter::releaseDmgDeep()
{
    if(_dmg_deep_last == 0)
        return false;
    -- _dmg_deep_last;
    if(_dmg_deep_last == 0)
    {
        _dmg_deep = 0;
        return true;
    }

    return false;
}

void BattleFighter::pushBeiNingShiZhe(BattleFighter* bo)
{
    if(!bo)
        return;

    if(std::find(_beiningshizhe.begin(),_beiningshizhe.end(),bo) == _beiningshizhe.end())
        _beiningshizhe.push_back(bo);
}

void BattleFighter::popBeiNingShiZhe(BattleFighter* bo)
{
    if(!bo)
        return;

    std::vector<BattleFighter*>::iterator it = std::find(_beiningshizhe.begin(),_beiningshizhe.end(),bo);
    if(it != _beiningshizhe.end())
        _beiningshizhe.erase(it);
}

bool BattleFighter::setDmgNingShi(BattleFighter* bf, float value, UInt8 last)
{
    if(!_ningshizhe)
    {
        _ningshizhe = bf;
        _ningshizhe->pushBeiNingShiZhe(this);
        _dmg_ningshi = value;
        _dmg_ningshi_last = last;
        return true;
    }

    return false;
}

bool BattleFighter::releaseDmgNingShi()
{
    if(_dmg_ningshi_last == 0)
        return false;
    -- _dmg_ningshi_last;
    if(_dmg_ningshi_last == 0)
    {
        _ningshizhe->popBeiNingShiZhe(this);
        _ningshizhe = NULL;
        _dmg_ningshi= 0;
        return true;
    }

    return false;
}

bool BattleFighter::clearDmgNingShi()
{
    if(_dmg_ningshi_last > 0)
    {
        _dmg_ningshi_last = 0;
        _ningshizhe->popBeiNingShiZhe(this);
        _ningshizhe = NULL;
        _dmg_ningshi= 0;
        return true;
    }

    return false;
}

Int32 BattleFighter::getSkillGradeExtraValue(UInt16 skillId)
{
    std::map<UInt16, Int32>::iterator it = _sg_v.find(skillId);
    if(it == _sg_v.end())
        return 0;
    return it->second;
}

}


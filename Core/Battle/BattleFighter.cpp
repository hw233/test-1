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
    _deep_confuse_dmg_extra(0), _deep_confuse_last(0),
    _deep_forget_dmg_extra(0), _deep_forget_last(0), _deep_stun_dmg_extra(0), _deep_stun_last(0),
    //_therapy_dec(0), _therapy_dec_last(0),_therapy_add(0),_therapy_add_last(0),
    _therapy_buff(0), _therapy_buff_last(0),
    _aura_dec(0), _aura_dec_last(0),
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
    _bleedMo(0), _bleedMoLast(0), _summoner(NULL), _unSummonAura(0), 
    _shieldHP(0), _shieldHPLast(0), _petShieldHP(0), 
    _petProtect100(false), _petProtect100Last(0), _petAtk100(0), _petAtk100Last(0), _petMark(false),
    _atkAddSpecial(0), _atkSpecialLast(0), _magAtkAddSpecial(0), _magAtkSpecialLast(0), 
    _atkDecSpecial(0), _atkDecSpecialLast(0), _magAtkDecSpecial(0), _magAtkDecSpecialLast(0),
    _skillUsedChangeAttrValue(0), _skillUsedChangeAttrLast(0), _skillUsedChangeAttr(0),
    _bleedRandom(0), _bleedRandomLast(0), _bleedAttackClass(1),_bleedBySkill(0), _bleedBySkillLast(0), _bleedBySkillClass(1),
    _hitChangeByPeerless(0),_counterChangeByPeerless(0),_bSingleAttackFlag(false),_bMainTargetDead(false),_nCurrentAttackIndex(0),
    _darkVigor(0), _dvFactor(0), _darkVigorLast(0), _hpShieldSelf(0), _hpShieldSelf_last(0)
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
    _therapySkillIdx = 0;
    _revival = false;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[activeSkill[idx]];
        skillItem.cd = 0;
        bool isTherapy = (skillItem.base->effect->hp > 0 || skillItem.base->effect->hpP > 0.001) && skillItem.base->target == GData::e_battle_target_selfside;
        if(isTherapy)
            _therapySkill.insert(_therapySkill.end(), skillItem);
        else
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
    std::vector<UInt16>& passiveSkillOnPetProtect = _fighter->getPassiveSkillOnPetProtect();

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
        if(SKILL_ID(skillItem.base->getId()) == 215)
            _passiveSkillDead100.insert(_passiveSkillDead100.begin(), skillItem);
        else
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

    cnt = passiveSkillOnPetProtect.size();
    _passiveSkillOnPetProtect.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnPetProtect[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnOtherDead.insert(_passiveSkillOnPetProtect.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnPetProtect[idx]);
    }

    std::vector<UInt16>& passiveSkillOnCounter = _fighter->getPassiveSkillOnCounter();
    std::vector<UInt16>& passiveSkillOnCounter100 = _fighter->getPassiveSkillOnCounter100();
    std::vector<UInt16>& passiveSkillOnAttackBleed100 = _fighter->getPassiveSkillOnAttackBleed100();
    std::vector<UInt16>& passiveSkillOnAtkDmg = _fighter->getPassiveSkillOnAtkDmg();
    std::vector<UInt16>& passiveSkillOnAtkDmg100 = _fighter->getPassiveSkillOnAtkDmg100();
    std::vector<UInt16>& passiveSkillOnGetDmg = _fighter->getPassiveSkillOnGetDmg();
    std::vector<UInt16>& passiveSkillOnGetDmg100 = _fighter->getPassiveSkillOnGetDmg100();

    cnt = passiveSkillOnCounter.size();
    _passiveSkillOnCounter.clear();
    rateExtent = 0;
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnCounter[idx]];
        skillItem.cd = 0;
        rateExtent += skillItem.base->prob * 100;
        skillItem.rateExtent = rateExtent;
        _passiveSkillOnCounter.insert(_passiveSkillOnCounter.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnCounter[idx]);
    }

    cnt = passiveSkillOnCounter100.size();
    _passiveSkillOnCounter100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnCounter100[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnCounter100.insert(_passiveSkillOnCounter100.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnCounter100[idx]);
    }

    cnt = passiveSkillOnAttackBleed100.size();
    _passiveSkillOnAttackBleed100.clear();
    for(idx = 0; idx < cnt; idx++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnAttackBleed100[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnAttackBleed100.insert(_passiveSkillOnAttackBleed100.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnAttackBleed100[idx]);
    }

    cnt = passiveSkillOnAtkDmg.size();
    _passiveSkillOnAtkDmg.clear();
    for (idx = 0; idx < cnt; idx ++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnAtkDmg[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnAtkDmg.insert(_passiveSkillOnAtkDmg.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnAtkDmg[idx]);
    }

    cnt = passiveSkillOnAtkDmg100.size();
    _passiveSkillOnAtkDmg100.clear();
    for (idx = 0; idx < cnt; idx ++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnAtkDmg100[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnAtkDmg100.insert(_passiveSkillOnAtkDmg100.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnAtkDmg100[idx]);
    }

    cnt = passiveSkillOnGetDmg100.size();
    _passiveSkillOnGetDmg100.clear();
    for (idx = 0; idx < cnt; idx ++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnGetDmg100[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnGetDmg100.insert(_passiveSkillOnGetDmg100.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnGetDmg100[idx]);
    }

    cnt = passiveSkillOnGetDmg.size();
    _passiveSkillOnGetDmg.clear();
    for (idx = 0; idx < cnt; idx ++)
    {
        GData::SkillItem skillItem;
        skillItem.base = GData::skillManager[passiveSkillOnGetDmg[idx]];
        skillItem.cd = 0;
        skillItem.rateExtent = 0;
        _passiveSkillOnGetDmg.insert(_passiveSkillOnGetDmg.end(), skillItem);

        updateSkillStrengthen(passiveSkillOnGetDmg[idx]);
    }

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

    if(_fighter && _fighter->getOwner() && _fighter->getOwner()->hasHiAttrFlag())
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

    GData::LBSkillItem* item = getSkillCondItem(SKILL_ID(skill->getId()));
    if(NULL != item)
        return aura_factor * (getMagAttack() * skill->effect->hpP + skill->effect->addhp + skill->effect->hp + item->ef_value);

    return aura_factor * (getMagAttack() * skill->effect->hpP + skill->effect->addhp + skill->effect->hp);
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

    float factor = getCriticalDmg() - defender->getTough(this);
    if(factor < 1)
        factor = 1;

    if(!cs)
    {
        factor = 1;
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
	}
}

UInt32 BattleFighter::regenHP( UInt32 u, bool weak /* = false */, float hppec /* = 0 */, float maxRhp /* = 0 */)
{
    if(_weakRound > 0 && weak)
        u /= 2;
    u += getMaxHP() * hppec;

    if (maxRhp > 0)
        u = u > maxRhp? maxRhp:u;

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
        clsnsex = (clsnsex & 0x0f) | GObject::e_cls_qinglong;

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
        if((_aura >= 100 && _peerlessSkill.base->effect != NULL) && (!noPossibleTarget || _peerlessSkill.base->target != GData::e_battle_target_otherside))
        {
            // peerless skill first
            if (_fighter->getOwner())
                _fighter->getOwner()->OnHeroMemo(GObject::MC_SKILL, GObject::MD_ADVANCED, 0, 2);
            return _peerlessSkill.base;
        }
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

const GData::SkillBase* BattleFighter::getPassiveSkillPrvAtk100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillPrvAtk100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftAtk100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillAftAtk100, idx, noPossibleTarget);
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

const GData::SkillBase* BattleFighter::getPassiveSkillOnAtkDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnAtkDmg100, idx, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnGetDmg100(size_t& idx, bool noPossibleTarget)
{
    return getPassiveSkill100(_passiveSkillOnGetDmg100, idx, noPossibleTarget);
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
        if(rnd < passiveSkill[idx].rateExtent && passiveSkill[idx].cd == 0)
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

const GData::SkillBase* BattleFighter::getPassiveSkillPreAtk(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillPreAtk, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillAftAtk(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillAftAtk, noPossibleTarget);
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
    //return getPassiveSkill( _passiveSkillOnAttackBleed, noPossibleTarget);
    return NULL;
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnAtkDmg(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillOnAtkDmg, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnPetProtect(bool noPossibleTarget)
{
    return getPassiveSkill(_passiveSkillOnPetProtect, noPossibleTarget);
}

const GData::SkillBase* BattleFighter::getPassiveSkillOnGetDmg(bool noPossibleTarget)
{
    return getPassiveSkill( _passiveSkillOnGetDmg, noPossibleTarget);
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
 

void BattleFighter::makeDamage( UInt32& u )
{
    if(_colorStockTimes > 0)
    {
        u = 0;
        -- _colorStockTimes;
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

    bf->setSummonFactor(aura, factor, last);

    return bf;
}

void BattleFighter::clearSkill()
{
    memset(&_peerlessSkill, 0, sizeof(_peerlessSkill));
    _activeSkill.clear();
    _therapySkill.clear();
    _passiveSkillPrvAtk100.clear();
    _passiveSkillAftAtk100.clear();
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

    _passiveSkillPreAtk.clear();
    _passiveSkillAftAtk.clear();
    _passiveSkillBeAtk.clear();
    _passiveSkillAftEvd.clear();
    _passiveSkillAftRes.clear();
    _passiveSkillEnter.clear();
    _passiveSkillDead.clear();
    _passiveSkillAftNAtk.clear();
    _passiveSkillOnCounter.clear();
    _passiveSkillOnGetDmg.clear();

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
        return true;

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

bool BattleFighter::releasePetProtect100()
{
    if(_petProtect100Last == 0)
        return false;

    -- _petProtect100Last;
    if(_petProtect100Last == 0)
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


}

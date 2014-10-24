
#ifndef CITTATABLE_H_
#define CITTATABLE_H_

#include "Config.h"
#include "ObjectManager.h"
#include "SkillTable.h"

namespace GData
{

struct CittaEffect : public ObjectBaseNT<UInt16>
{
    CittaEffect(UInt16 id)
        : ObjectBaseNT<UInt16>(id), strength(0), physique(0), agility(0), intelligence(0), will(0), soul(0),
    aura(0), auraMax(0), attack(0), magatk(0), defend(0), magdef(0),
    hp(0), tough(0), action(0), hitrate(0), evade(0), critical(0),
    criticaldmg(0), pierce(0), counter(0), magres(0), strengthP(0), physiqueP(0),
    agilityP(0), intelligenceP(0), willP(0), soulP(0), auraP(0), auraMaxP(0),
    attackP(0), magatkP(0), defendP(0), magdefP(0), hpP(0), toughP(0),
    actionP(0), hitrateP(0), evadeP(0), criticalP(0), criticaldmgP(0), pierceP(0), counterP(0), magresP(0),
    fairyAck(0),fairyDef(0),criticaldef(0),piercedef(0),counterdef(0),attackpierce(0)
    {}
    ~CittaEffect() { skill.clear(); }

	float strength;
	float physique;
	float agility;
	float intelligence;
	float will;
	float soul;
	float aura;
	float auraMax;
	float attack;
	float magatk;
	float defend;
	float magdef;
	float hp;
	float tough;
	float action;
	float hitrate;
	float evade;
	float critical;
	float criticaldmg;
	float pierce;
	float counter;
	float magres;

	float strengthP;
	float physiqueP;
	float agilityP;
	float intelligenceP;
	float willP;
	float soulP;
	float auraP;
	float auraMaxP;
	float attackP;
	float magatkP;
	float defendP;
	float magdefP;

	float hpP;
    float toughP;
	float actionP;
	float hitrateP;
	float evadeP;
	float criticalP;
	float criticaldmgP;
	float pierceP;
	float counterP;
	float magresP;

    float fairyAck;
    float fairyDef;
    
    UInt32 criticaldef;
    UInt32 piercedef;
    UInt32 counterdef;
    UInt32 attackpierce;

    float hitrlvl;
    float evdlvl;
    float crilvl;
    float pirlvl;
    float counterlvl;
    float mreslvl;
    float toughlvl;
    float criticaldmgimmune;
	CittaEffect getIncenseUp(const float rate) const
	{
        CittaEffect aet = *this;
		aet.strength *= rate;
		aet.physique *= rate;
		aet.agility *= rate;
		aet.intelligence *= rate;
		aet.will *= rate;
		aet.soul *= rate;
		aet.aura *= rate;
		aet.auraMax *= rate;
		aet.attack *= rate;
        aet.magatk *= rate;
		aet.defend *= rate;
        aet.magdef *= rate;
		aet.hp *= rate;
		aet.tough *= rate;
		aet.action *= rate;
		aet.hitrate *= rate;
		aet.evade *= rate;
		aet.critical *= rate;
        //std::cout << "暴击伤害不加成:" << rate << std::endl;
		//aet.criticaldmg *= rate;
		aet.pierce *= rate;
		aet.counter *= rate;
        aet.magres *= rate;

		aet.strengthP *= rate;
		aet.physiqueP *= rate;
		aet.agilityP *= rate;
		aet.intelligenceP *= rate;
		aet.willP *= rate;
		aet.soulP *= rate;
		aet.attackP *= rate;
        aet.magatkP *= rate;
		aet.defendP *= rate;
        aet.magdefP *= rate;

		aet.hpP *= rate;
		aet.actionP *= rate;
        //std::cout << "暴击伤害不加成:" << rate << std::endl;
		//aet.criticaldmgP *= rate;   //暴击伤害不加成
		aet.counterP *= rate;
        aet.magresP *= rate;

        aet.hitrlvl *= rate;
        aet.evdlvl *= rate;
        aet.crilvl *= rate;
        aet.pirlvl *= rate;
        aet.counterlvl *= rate;
        aet.mreslvl *= rate;
        aet.toughlvl *= rate;
        aet.criticaldmgimmune *= rate;
        return aet;
	}

    std::vector<const SkillBase*> skill;  // 带出技能
};

struct CittaBase : public ObjectBaseT<UInt16>
{
    CittaBase(UInt16 id, std::string& name)
        : ObjectBaseT<UInt16>(id, name) {}

    ~CittaBase() { if (effect) delete effect; }

    UInt16 type;
    UInt32 pexp;
    UInt16 needsoul;
    const CittaEffect* effect;
};

typedef ObjectMapT<CittaBase, UInt16> CittaManager;
#define cittaManager CittaManager::Instance()

typedef ObjectMapT<CittaEffect, UInt16> CittaEffectManager;
#define cittaEffectManager CittaEffectManager::Instance()

} // namespace GData

#endif // CITTATABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


#ifndef _ATTREXTRA_H_
#define _ATTREXTRA_H_

#include "ObjectManager.h"
#include "CittaTable.h"
#include "GObject/AttrFactor.h"

namespace GData
{

struct SkillBase;

struct AttrExtra
{
	AttrExtra() : strength(0), physique(0), agility(0), intelligence(0), will(0), soul(0),
    aura(0), auraMax(0), attack(0), magatk(0), defend(0), magdef(0),
    hp(0), tough(0), action(0), hitrate(0), evade(0), critical(0),
    criticaldmg(0), pierce(0), counter(0), magres(0), strengthP(0), physiqueP(0),
    agilityP(0), intelligenceP(0), willP(0), soulP(0), auraP(0), auraMaxP(0),
    attackP(0), magatkP(0), defendP(0), magdefP(0), hpP(0), toughP(0),
    actionP(0), hitrateP(0), evadeP(0), criticalP(0), criticaldmgP(0), pierceP(0), counterP(0), magresP(0),
    hitrlvl(0), evdlvl(0), crilvl(0), pirlvl(0), counterlvl(0), mreslvl(0), toughlvl(0)
	{}
	AttrExtra& operator +=(const AttrExtra& other)
	{
		strength += other.strength;
		physique += other.physique;
		agility += other.agility;
		intelligence += other.intelligence;
		will += other.will;
		soul += other.soul;
		aura += other.aura;
		auraMax += other.auraMax;
		attack += other.attack;
        magatk += other.magatk;
		defend += other.defend;
        magdef += other.magdef;
		hp += other.hp;
		tough += other.tough;
		action += other.action;
		hitrate += other.hitrate;
		evade += other.evade;
		critical += other.critical;
		criticaldmg += other.criticaldmg;
		pierce += other.pierce;
		counter += other.counter;
        magres += other.magres;

		strengthP += other.strengthP;
		physiqueP += other.physiqueP;
		agilityP += other.agilityP;
		intelligenceP += other.intelligenceP;
		willP += other.willP;
		soulP += other.soulP;
		auraP += other.auraP;
		auraMaxP += other.auraMaxP;
		attackP += other.attackP;
        magatkP += other.magatkP;
		defendP += other.defendP;
        magdefP += other.magdefP;

		hpP += other.hpP;
		actionP += other.actionP;
		criticaldmgP += other.criticaldmgP;
		counterP += other.counterP;
        magresP += other.magresP;
#if 0
		toughP += other.toughP;
		hitrateP += other.hitrateP;
		evadeP += other.evadeP;
		criticalP += other.criticalP;
		pierceP += other.pierceP;
#endif

        hitrlvl += other.hitrlvl;
        evdlvl += other.evdlvl;
        crilvl += other.crilvl;
        pirlvl += other.pirlvl;
        counterlvl += other.counterlvl;
        mreslvl += other.mreslvl;
        toughlvl += other.toughlvl;

		return *this;
	}

	AttrExtra& operator *=(const GObject::AttrFactor& af)
	{
		strength *= af.strength;
		physique *= af.physique;
		agility *= af.agility;
		intelligence *= af.intelligence;
		will *= af.will;
		soul *= af.soul;
		aura = af.aura;
		auraMax = af.auraMax;
		attack *= af.attack;
        magatk *= af.magatk;
		defend *= af.defend;
        magdef *= af.magdef;
		hp *= af.hp;
		tough *= af.tough;
		action *= af.action;
		hitrate *= af.hitrate;
		evade *= af.evade;
		critical *= af.critical;
		criticaldmg *= af.criticaldmg;
		pierce *= af.pierce;
		counter *= af.counter;
        magres *= af.magres;

		strengthP *= af.strength;
		physiqueP *= af.physique;
		agilityP *= af.agility;
		intelligenceP *= af.intelligence;
		willP *= af.will;
		soulP *= af.soul;
		attackP *= af.attack;
        magatkP *= af.magatk;
		defendP *= af.defend;
        magdefP *= af.magdef;

		hpP *= af.hp;
		actionP *= af.action;
		criticaldmgP *= af.criticaldmg;
		counterP *= af.counter;
        magresP *= af.magres;

        hitrlvl *= af.hitrate;
        evdlvl *= af.evade;
        crilvl *= af.critical;
        pirlvl *= af.pierce;
        counterlvl *= af.counter;
        mreslvl *= af.magres;
        toughlvl *= af.tough;

		return *this;
	}


	AttrExtra& operator +=(const CittaEffect& other)
	{
		strength += other.strength;
		physique += other.physique;
		agility += other.agility;
		intelligence += other.intelligence;
		will += other.will;
		soul += other.soul;
		aura += other.aura;
		auraMax += other.auraMax;
		attack += other.attack;
        magatk += other.magatk;
		defend += other.defend;
        magdef += other.magdef;
		hp += other.hp;
		tough += other.tough;
		action += other.action;
		hitrate += other.hitrate;
		evade += other.evade;
		critical += other.critical;
		criticaldmg += other.criticaldmg;
		pierce += other.pierce;
		counter += other.counter;
        magres += other.magres;

		strengthP += other.strengthP;
		physiqueP += other.physiqueP;
		agilityP += other.agilityP;
		intelligenceP += other.intelligenceP;
		willP += other.willP;
		soulP += other.soulP;
		auraP += other.auraP;
		auraMaxP += other.auraMaxP;
		attackP += other.attackP;
        magatkP += other.magatkP;
		defendP += other.defendP;
        magdefP += other.magdefP;

		hpP += other.hpP;
		actionP += other.actionP;
		criticaldmgP += other.criticaldmgP;
		counterP += other.counterP;
        magresP += other.magresP;
#if 0
		toughP += other.toughP;
		hitrateP += other.hitrateP;
		evadeP += other.evadeP;
		criticalP += other.criticalP;
		pierceP += other.pierceP;
#endif

        hitrlvl += other.hitrlvl;
        evdlvl += other.evdlvl;
        crilvl += other.crilvl;
        pirlvl += other.pirlvl;
        counterlvl += other.counterlvl;
        mreslvl += other.mreslvl;
        toughlvl += other.toughlvl;

		return *this;
	}

	inline void reset()
	{
        strength = 0;
        physique = 0;
        agility = 0;
        intelligence = 0;
        will = 0;
        soul = 0;
        aura = 0;
        auraMax = 0;
        attack = 0;
        magatk = 0;
        defend = 0;
        magdef = 0;
        hp = 0;
        tough = 0;
        action = 0;
        hitrate = 0;
        evade = 0;
        critical = 0;
        criticaldmg = 0;
        pierce = 0;
        counter = 0;
        magres = 0;

        strengthP = 0;
        physiqueP = 0;
        agilityP = 0;
        intelligenceP = 0;
        willP = 0;
        soulP = 0;
        auraP = 0;
        auraMaxP = 0;
        attackP = 0;
        magatkP = 0;
        defendP = 0;
        magdefP = 0;
        hpP = 0;
        toughP = 0;
        actionP = 0;
        hitrateP = 0;
        evadeP = 0;
        criticalP = 0;
        criticaldmgP = 0;
        pierceP = 0;
        counterP = 0;
        magresP = 0;

        hitrlvl = 0;
        evdlvl = 0;
        crilvl = 0;
        pirlvl = 0;
        counterlvl = 0;
        mreslvl = 0;
        toughlvl = 0;
    }

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

    float hitrlvl;
    float evdlvl;
    float crilvl;
    float pirlvl;
    float counterlvl;
    float mreslvl;
    float toughlvl;

    std::vector<const SkillBase*> skills;
};

struct AttrExtraItem:
	public ObjectBaseNT<>
{
	AttrExtraItem(UInt32 idx): ObjectBaseNT<>(idx) { }
	AttrExtra _extra;
	inline AttrExtra* operator ->() { return &_extra; }
	inline operator const AttrExtra*() const { return &_extra; }
};

typedef ObjectListT<AttrExtraItem> AttrExtraManager;

#define attrExtraManager AttrExtraManager::Instance()

}

#endif // _ATTREXTRA_H_

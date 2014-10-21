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
    hitrlvl(0), evdlvl(0), crilvl(0), pirlvl(0), counterlvl(0), mreslvl(0), toughlvl(0), criticaldmgimmune(0),
    fairyAck(0),fairyDef(0),criticaldef(0),piercedef(0),counterdef(0),attackpierce(0)
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
        criticaldmgimmune += other.criticaldmgimmune;
        fairyAck += other.fairyAck;
        fairyDef += other.fairyDef;
        criticaldef += other.criticaldef;
        piercedef += other.piercedef;
        counterdef += other.counterdef;
        attackpierce += other.attackpierce;


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
        criticaldmgimmune += other.criticaldmgimmune;

        fairyAck += other.fairyAck;
        fairyDef += other.fairyDef;
        criticaldef += other.criticaldef;
        piercedef += other.piercedef;
        counterdef += other.counterdef;
        attackpierce += other.attackpierce;

		return *this;
	}
    AttrExtra operator *(const float rate) const
	{
        AttrExtra aet = *this;
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
		aet.criticaldmg *= rate;
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
		aet.criticaldmgP *= rate;
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
        aet.fairyAck *= rate;
        aet.fairyDef *= rate;
        aet.criticaldef *= rate;
        aet.piercedef *= rate;
        aet.counterdef *= rate;
        aet.attackpierce *= rate;
        return aet;
	}

    AttrExtra operator ^(const float rate) const
	{
        AttrExtra aet = *this;
		aet.strength *= rate;
		aet.physique *= rate;
		aet.agility *= rate;
		aet.intelligence *= rate;
		aet.will *= rate;
		aet.soul *= rate;
		aet.aura *= rate;
		aet.auraMax *= rate;
		aet.attack = static_cast<UInt32>(aet.attack * rate + 0.99);
        aet.magatk = static_cast<UInt32>(aet.magatk * rate + 0.99);
		aet.defend = static_cast<UInt32>(aet.defend * rate + 0.99);
        aet.magdef = static_cast<UInt32>(aet.magdef * rate + 0.99);
		aet.hp = static_cast<UInt32>(aet.hp * rate + 0.99);
		aet.tough *= rate;
		aet.action = static_cast<UInt32>(aet.action * rate + 0.99);
		aet.hitrate *= rate;
		aet.evade *= rate;
		aet.critical *= rate;
		aet.criticaldmg *= rate;
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
		aet.criticaldmgP *= rate;
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
        aet.fairyAck *= rate;
        aet.fairyDef *= rate;
        aet.criticaldef *= rate;
        aet.piercedef *= rate;
        aet.counterdef *= rate;
        aet.attackpierce *= rate;
        return aet;
	}
    inline AttrExtra getFairyEquipInfo(const AttrExtra& other)
    {
		strength += other.strength;
		physique += other.physique;
		agility += other.agility;
		intelligence += other.intelligence;
		will += other.will;
		soul += other.soul;
		aura += other.aura;
		auraMax += other.auraMax;
        //仙术攻击防御，对物攻、法攻、物防、法防加成为0.3  (仙术攻击防御值 写在 法攻，法防之上)
		attack += other.magatk * 0.3;
        magatk += other.magatk * 0.3;
		defend += other.magdef * 0.3;
        magdef += other.magdef * 0.3;
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
        criticaldmgimmune += other.criticaldmgimmune;

        fairyAck += other.magatk;
        fairyDef += other.magdef;
        criticaldef += other.criticaldef;
        piercedef += other.piercedef;
        counterdef += other.counterdef;
        attackpierce += other.attackpierce;

		return *this;
    }
    inline AttrExtra getFairyEquipInfo(const CittaEffect& other)
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
        criticaldmgimmune += other.criticaldmgimmune;

        fairyAck += other.magatk;
        fairyDef += other.magdef;

        criticaldef += other.criticaldef;
        piercedef += other.piercedef;
        counterdef += other.counterdef;
        attackpierce += other.attackpierce;

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
        criticaldmgimmune = 0;
        fairyAck = 0;
        fairyDef = 0;
        criticaldef = 0;
        piercedef = 0;
        counterdef = 0;
        attackpierce = 0;
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
    float criticaldmgimmune;

    float fairyAck;
    float fairyDef;

    UInt32 criticaldef;
    UInt32 piercedef;
    UInt32 counterdef;
    UInt32 attackpierce;
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

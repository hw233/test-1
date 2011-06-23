#ifndef _ATTREXTRA_H_
#define _ATTREXTRA_H_

#include "ObjectManager.h"

namespace GData
{

struct SkillBase;

struct AttrExtra
{
	AttrExtra():
		strength(0), physique(0), agility(0), intelligence(0),
        will(0), soul(0), aura(0), auraMax(0), tough(0),
		strengthP(0), physiqueP(0), agilityP(0), intelligenceP(0),
        willP(0), soulP(0), auraP(0), auraMaxP(0), 
		attack(0), attackP(0), magatk(0), magatkP(0),
        defend(0), defendP(0), magdef(0), magdefP(0),
		hp(0), hpP(0), action(0), hitrate(0), evade(0),
        critical(0), critical_dmg(0), pierce(0), counter(0), magres(0)
	{ }
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
		tough += other.tough;
		strengthP += other.strengthP;
		physiqueP += other.physiqueP;
		agilityP += other.agilityP;
		intelligenceP += other.intelligenceP;
		willP += other.willP;
		soulP += other.soulP;
		auraP += other.auraP;
		auraMaxP += other.auraMaxP;
		attack += other.attack;
		attackP += other.attackP;
		defend += other.defend;
		defendP += other.defendP;
        magatk += other.magatk;
        magatkP += other.magatkP;
        magdef += other.magdef;
        magdefP += other.magdefP;
		hp += other.hp;
		hpP += other.hpP;
		action += other.action;
		hitrate += other.hitrate;
		evade += other.evade;
		critical += other.critical;
		critical_dmg += other.critical_dmg;
		pierce += other.pierce;
		counter += other.counter;
        magres += other.magres;
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
        tough = 0;
        strengthP = 0;
        physiqueP = 0;
        agilityP = 0;
        intelligenceP = 0;
        willP = 0;
        soulP = 0;
        auraP = 0;
        auraMaxP = 0;
        attack = 0;
        attackP = 0;
        magatk = 0;
        magatkP = 0;
        defend = 0;
        defendP = 0;
        magdef = 0;
        magdefP = 0;
        hp = 0;
        hpP = 0;
        action = 0;
        hitrate = 0;
        evade = 0;
        critical = 0;
        critical_dmg = 0;
        pierce = 0;
        counter = 0;
        magres = 0;
	}

	UInt16 strength;
	UInt16 physique;
	UInt16 agility;
	UInt16 intelligence;
	UInt16 will;
	UInt16 soul;
	UInt16 aura;
	UInt16 auraMax;
	UInt16 tough;
	float strengthP;
	float physiqueP;
	float agilityP;
	float intelligenceP;
	float willP;
	float soulP;
	float auraP;
	float auraMaxP;
	UInt16 attack;
	float attackP;
	UInt16 magatk;
	float magatkP;
	UInt16 defend;
	float defendP;
	UInt16 magdef;
	float magdefP;
	UInt16 hp;
	float hpP;
	float action;
	float hitrate;
	float evade;
	float critical;
	float critical_dmg;
	float pierce;
	float counter;
	float magres;
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

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
        willP(0), soulP(0), auraP(0), auraMaxP(0), toughP(0),
		attack(0), attackP(0), mag_attack(0), mag_attackP(0),
        defend(0), defendP(0), mag_defend(0), mag_defendP(0),
		hp(0), hpP(0), action(0), hitrate(0), evade(0),
        critical(0), critical_dmg(0), pierce(0), counter(0), mag_res(0)
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
		toughP += other.toughP;
		attack += other.attack;
		attackP += other.attackP;
		defend += other.defend;
		defendP += other.defendP;
        mag_attack += other.mag_attack;
        mag_attackP += other.mag_attackP;
        mag_defend += other.mag_defend;
        mag_defendP += other.mag_defendP;
		hp += other.hp;
		hpP += other.hpP;
		action += other.action;
		hitrate += other.hitrate;
		evade += other.evade;
		critical += other.critical;
		critical_dmg += other.critical_dmg;
		pierce += other.pierce;
		counter += other.counter;
        mag_res += other.mag_res;
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
        toughP = 0;
        attack = 0;
        attackP = 0;
        mag_attack = 0;
        mag_attackP = 0;
        defend = 0;
        defendP = 0;
        mag_defend = 0;
        mag_defendP = 0;
        hp = 0;
        hpP = 0;
        action = 0;
        hitrate = 0;
        evade = 0;
        critical = 0;
        critical_dmg = 0;
        pierce = 0;
        counter = 0;
        mag_res = 0;
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
	float toughP;
	UInt16 attack;
	float attackP;
	UInt16 mag_attack;
	float mag_attackP;
	UInt16 defend;
	float defendP;
	UInt16 mag_defend;
	float mag_defendP;
	UInt16 hp;
	float hpP;
	float action;
	float hitrate;
	float evade;
	float critical;
	float critical_dmg;
	float pierce;
	float counter;
	float mag_res;
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

#ifndef _ATTREXTRA_H_
#define _ATTREXTRA_H_

#include "ObjectManager.h"

namespace GData
{

struct Attr1Extra
{
	Attr1Extra():
		strength(0), physique(0), agility(0), intelligence(0),
		strengthP(0), physiqueP(0), agilityP(0), intelligenceP(0)
	{ }
	Attr1Extra& operator +=(const Attr1Extra& other)
	{
		strength += other.strength;
		physique += other.physique;
		agility += other.agility;
		intelligence += other.intelligence;
		strengthP += other.strengthP;
		physiqueP += other.physiqueP;
		agilityP += other.agilityP;
		intelligenceP += other.intelligenceP;
		return *this;
	}
	inline void reset()
	{
		strength = 0;
		physique = 0;
		agility = 0;
		intelligence = 0;
		strengthP = 0;
		physiqueP = 0;
		agilityP = 0;
		intelligenceP = 0;
	}
	UInt16 strength;
	UInt16 physique;
	UInt16 agility;
	UInt16 intelligence;
	float strengthP;
	float physiqueP;
	float agilityP;
	float intelligenceP;
};

struct Attr2Extra
{
	Attr2Extra():
		attack(0), attackP(0), defend(0), defendP(0),
		hp(0), hpP(0), action(0), hitrate(0), evade(0), critical(0), pierce(0), counter(0)
	{}
	Attr2Extra& operator +=(const Attr2Extra& other)
	{
		attack += other.attack;
		attackP += other.attackP;
		defend += other.defend;
		defendP += other.defendP;
		hp += other.hp;
		hpP += other.hpP;
		action += other.action;
		hitrate += other.hitrate;
		evade += other.evade;
		critical += other.critical;
		pierce += other.pierce;
		counter += other.counter;
		return *this;
	}
	inline void reset()
	{
		attack = 0;
		attackP = 0;
		defend = 0;
		defendP = 0;
		hp = 0;
		hpP = 0;
		action = 0;
		hitrate = 0;
		evade = 0;
		critical = 0;
		pierce = 0;
		counter = 0;
	}
	UInt16 attack;
	float attackP;
	UInt16 defend;
	float defendP;
	UInt16 hp;
	float hpP;
	float action;
	float hitrate;
	float evade;
	float critical;
	float pierce;
	float counter;
};

struct Attr1ExtraItem:
	public ObjectBaseNT<>
{
	Attr1ExtraItem(UInt32 idx): ObjectBaseNT<>(idx) { }
	Attr1Extra _extra;
	inline Attr1Extra* operator ->() { return &_extra; }
	inline operator const Attr1Extra*() const { return &_extra; }
};

struct Attr2ExtraItem:
	public ObjectBaseNT<>
{
	Attr2ExtraItem(UInt32 idx): ObjectBaseNT<>(idx) { }
	Attr2Extra _extra;
	inline Attr2Extra* operator ->() { return &_extra; }
	inline operator const Attr2Extra*() const { return &_extra; }
};

typedef ObjectListT<Attr1ExtraItem> Attr1ExtraManager;
typedef ObjectListT<Attr2ExtraItem> Attr2ExtraManager;

#define attr1ExtraManager Attr1ExtraManager::Instance()
#define attr2ExtraManager Attr2ExtraManager::Instance()

}

#endif // _ATTREXTRA_H_

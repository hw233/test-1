#ifndef _ATTREXTRA_H_
#define _ATTREXTRA_H_

#include "ObjectManager.h"

namespace GData
{

struct Attr1Extra
{
	Attr1Extra():
		strength(0), physique(0), agility(0), intelligence(0),
        will(0), soul(0), aura(0), tough(0),
		strengthP(0), physiqueP(0), agilityP(0), intelligenceP(0),
        willP(0), soulP(0), auraP(0), toughP(0)
	{ }
	Attr1Extra& operator +=(const Attr1Extra& other)
	{
		strength += other.strength;
		physique += other.physique;
		agility += other.agility;
		intelligence += other.intelligence;
		will += other.will;
		soul += other.soul;
		aura += other.aura;
		tough += other.tough;
		strengthP += other.strengthP;
		physiqueP += other.physiqueP;
		agilityP += other.agilityP;
		intelligenceP += other.intelligenceP;
		willP += other.willP;
		soulP += other.soulP;
		auraP += other.auraP;
		toughP += other.toughP;
		return *this;
	}
	inline void reset()
	{
        bzero(this, sizeof(*this));
	}

	UInt16 strength;
	UInt16 physique;
	UInt16 agility;
	UInt16 intelligence;
	UInt16 will;
	UInt16 soul;
	UInt16 aura;
	UInt16 tough;
	float strengthP;
	float physiqueP;
	float agilityP;
	float intelligenceP;
	float willP;
	float soulP;
	float auraP;
	float toughP;
};

struct Attr2Extra
{
	Attr2Extra():
		attack(0), attackP(0), img_attack(0), img_attackP(0),
        defend(0), defendP(0), img_defend(0), img_defendP(0),
		hp(0), hpP(0), action(0), hitrate(0), evade(0),
        critical(0), critical_dmg(0), pierce(0), counter(0), img_res(0)
	{}
	Attr2Extra& operator +=(const Attr2Extra& other)
	{
		attack += other.attack;
		attackP += other.attackP;
		defend += other.defend;
		defendP += other.defendP;
        img_attack += other.img_attack;
        img_attackP += other.img_attackP;
        img_defend += other.img_defend;
        img_defendP += other.img_defendP;
		hp += other.hp;
		hpP += other.hpP;
		action += other.action;
		hitrate += other.hitrate;
		evade += other.evade;
		critical += other.critical;
		critical_dmg += other.critical_dmg;
		pierce += other.pierce;
		counter += other.counter;
        img_res += other.img_res;
		return *this;
	}
	inline void reset()
	{
        bzero(this, sizeof(*this));
	}
	UInt16 attack;
	float attackP;
	UInt16 img_attack;
	float img_attackP;
	UInt16 defend;
	float defendP;
	UInt16 img_defend;
	float img_defendP;
	UInt16 hp;
	float hpP;
	float action;
	float hitrate;
	float evade;
	float critical;
	float critical_dmg;
	float pierce;
	float counter;
	float img_res;
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

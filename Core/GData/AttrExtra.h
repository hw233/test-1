#ifndef _ATTREXTRA_H_
#define _ATTREXTRA_H_

#include "ObjectManager.h"

namespace GData
{

struct AttrExtra
{
	AttrExtra():
		strength(0), physique(0), agility(0), intelligence(0),
        will(0), soul(0), aura(0), tough(0),
		strengthP(0), physiqueP(0), agilityP(0), intelligenceP(0),
        willP(0), soulP(0), auraP(0), toughP(0),
		attack(0), attackP(0), img_attack(0), img_attackP(0),
        defend(0), defendP(0), img_defend(0), img_defendP(0),
		hp(0), hpP(0), action(0), hitrate(0), evade(0),
        critical(0), critical_dmg(0), pierce(0), counter(0), img_res(0)
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
		tough += other.tough;
		strengthP += other.strengthP;
		physiqueP += other.physiqueP;
		agilityP += other.agilityP;
		intelligenceP += other.intelligenceP;
		willP += other.willP;
		soulP += other.soulP;
		auraP += other.auraP;
		toughP += other.toughP;
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

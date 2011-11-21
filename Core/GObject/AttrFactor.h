#ifndef _ATTRFACTOR_H_
#define _ATTRFACTOR_H_

namespace GObject
{

struct AttrFactor
{
	AttrFactor() : strength(0), physique(0), agility(0), intelligence(0), will(0), soul(0),
    aura(0), auraMax(0), attack(0), magatk(0), defend(0), magdef(0),
    hp(0), tough(0), action(0), hitrate(0), evade(0), critical(0),
    criticaldmg(0), pierce(0), counter(0), magres(0)
	{}

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
};

}

#endif


#ifndef _BATTLEFORMULA_H_
#define _BATTLEFORMULA_H_

#include "Script.h"
#include "lua_tinker.h"

namespace GObject
{
	class Fighter;
    class SecondSoul;
}

namespace Battle
{
	class BattleFighter;
}

namespace Script
{

class BattleFormula:
	public Script
{
public:
	struct SkillData
	{
		SkillData(): rate(0), value1(0.0f), value2(0.0f), value3(0), value4(0) {}
		UInt32 rate;
		float value1;
		float value2;
		UInt32 value3;
		UInt32 value4;
	};
public:
	BattleFormula(const char * file);
	void init();
	void postInit();

    float calcSoulStrenght(GObject::SecondSoul*);
    float calcSoulAgility(GObject::SecondSoul*);
    float calcSoulPhysique(GObject::SecondSoul*);
    float calcSoulIntelligence(GObject::SecondSoul*);
    float calcSoulWill(GObject::SecondSoul*);

    float calcSoulXinxiuAttack(GObject::SecondSoul*);
    float calcSoulXinxiuAction(GObject::SecondSoul*);
    float calcSoulXinxiuDefend(GObject::SecondSoul*);
    float calcSoulXinxiuHp(GObject::SecondSoul*);

	float calcStrength(GObject::Fighter *);
	float calcAgility(GObject::Fighter *);
	float calcIntelligence(GObject::Fighter *);
	float calcWill(GObject::Fighter *);
	float calcSoul(GObject::Fighter *);
	float calcAura(GObject::Fighter *);
	float calcAuraMax(GObject::Fighter *);
	float calcTough(GObject::Fighter *, GObject::Fighter *);
	float calcPhysique(GObject::Fighter *);
	float calcAttack(GObject::Fighter *);
	float calcMagAttack(GObject::Fighter *);
	float calcDefend(GObject::Fighter *);
	float calcMagDefend(GObject::Fighter *);
	float calcHitrate(GObject::Fighter *, GObject::Fighter *);
	float calcEvade(GObject::Fighter *, GObject::Fighter *);
	float calcCritical(GObject::Fighter *, GObject::Fighter *);
	float calcCriticalDmg(GObject::Fighter *);
	float calcPierce(GObject::Fighter *, GObject::Fighter *);
	float calcCounter(GObject::Fighter *, GObject::Fighter *);
	float calcMagRes(GObject::Fighter *, GObject::Fighter *);
	float calcHitRateLevel(GObject::Fighter *);
	float calcEvadeLevel(GObject::Fighter *);
	float calcCriticalLevel(GObject::Fighter *);
	float calcPierceLevel(GObject::Fighter *);
	float calcCounterLevel(GObject::Fighter *);
	float calcToughLevel(GObject::Fighter *);
	float calcMagResLevel(GObject::Fighter *);

	float calcStrength(Battle::BattleFighter *);
	float calcAgility(Battle::BattleFighter *);
	float calcIntelligence(Battle::BattleFighter *);
	float calcWill(Battle::BattleFighter *);
	float calcSoul(Battle::BattleFighter *);
	float calcAura(Battle::BattleFighter *);
	float calcAuraMax(Battle::BattleFighter *);
	float calcTough(Battle::BattleFighter *, Battle::BattleFighter *);
	float calcPhysique(Battle::BattleFighter *);
	float calcAttack(Battle::BattleFighter *);
	float calcMagAttack(Battle::BattleFighter *);
	float calcDefend(Battle::BattleFighter *);
	float calcMagDefend(Battle::BattleFighter *);
	float calcHitrate(Battle::BattleFighter *, Battle::BattleFighter *);
	float calcEvade(Battle::BattleFighter *, Battle::BattleFighter *);
	float calcCritical(Battle::BattleFighter *, Battle::BattleFighter *);
	float calcCriticalDmg(Battle::BattleFighter *);
	float calcPierce(Battle::BattleFighter *, Battle::BattleFighter *);
	float calcCounter(Battle::BattleFighter *, Battle::BattleFighter *);
	float calcMagRes(Battle::BattleFighter *, Battle::BattleFighter *);
	float calcHitRateLevel(Battle::BattleFighter *);
	float calcEvadeLevel(Battle::BattleFighter *);
	float calcCriticalLevel(Battle::BattleFighter *);
	float calcPierceLevel(Battle::BattleFighter *);
	float calcCounterLevel(Battle::BattleFighter *);
	float calcToughLevel(Battle::BattleFighter *);
	float calcMagResLevel(Battle::BattleFighter *);

	float calcHP(GObject::Fighter *);
	float calcAction(GObject::Fighter *);
	float calcBattlePoint(GObject::Fighter *);
	float calcAutoBattle(float, float);
	float calcSkillBattlePoint(UInt8 color, UInt8 level, UInt8 type, UInt8 ssLevel);

	float calcHP(Battle::BattleFighter *);
	float calcAction(Battle::BattleFighter *);

	float calcDamage(float, float, float, float, float);
    float calcPracticeInc(GObject::Fighter *);
    float calcBasePExp(GObject::Fighter *);
    float calcPExpNoBuf(GObject::Fighter *);
    float calcClanTechAddon(UInt16 id, UInt8 lvl);
	lua_tinker::table getFactor(UInt8, UInt8, UInt8);
    UInt32 calcTaskAward(UInt8, UInt8, UInt8);

	inline SkillData& skillData(UInt16 c, UInt16 s, UInt8 level)
	{
		if(level > 39)
			level = 39;
		return _skillData[c][s][level];
	}

	static BattleFormula * getCurrent();

private:
	SkillData _skillData[3][10][40];
};

}

#endif // _BATTLEFORMULA_H_

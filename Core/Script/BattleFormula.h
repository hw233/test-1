#ifndef _BATTLEFORMULA_H_
#define _BATTLEFORMULA_H_

#include "Script.h"
#include "lua_tinker.h"

namespace GObject
{
	class Fighter;
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

	float calcStrength(GObject::Fighter *);
	float calcAgility(GObject::Fighter *);
	float calcIntelligence(GObject::Fighter *);
	float calcPhysique(GObject::Fighter *);
	float calcAttack(GObject::Fighter *);
	float calcDefend(GObject::Fighter *);
	float calcHitrate(GObject::Fighter *);
	float calcEvade(GObject::Fighter *);
	float calcCritical(GObject::Fighter *);
	float calcPierce(GObject::Fighter *);
	float calcCounter(GObject::Fighter *);

	float calcStrength(Battle::BattleFighter *);
	float calcAgility(Battle::BattleFighter *);
	float calcIntelligence(Battle::BattleFighter *);
	float calcPhysique(Battle::BattleFighter *);
	float calcAttack(Battle::BattleFighter *);
	float calcDefend(Battle::BattleFighter *);
	float calcHitrate(Battle::BattleFighter *);
	float calcEvade(Battle::BattleFighter *);
	float calcCritical(Battle::BattleFighter *);
	float calcPierce(Battle::BattleFighter *);
	float calcCounter(Battle::BattleFighter *);

	UInt32 calcHP(GObject::Fighter *);
	UInt32 calcAction(GObject::Fighter *);
	float calcBattlePoint(GObject::Fighter *);
	float calcAutoBattle(float, float);

	UInt32 calcHP(Battle::BattleFighter *);
	UInt32 calcAction(Battle::BattleFighter *);

	UInt32 calcDamage(float, float);
	lua_tinker::table getFactor(UInt8, UInt8, UInt8);

	void reloadSkillData();

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

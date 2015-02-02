#include "Config.h"
#include "BattleFormula.h"
#include "Battle/BattleFighter.h"
//#include "GObject/Fighter.h"
#include "GObject/Country.h"
#include "GObject/World.h"
#include "Server/OidGenerator.h"
#include "GObject/ItemData.h"
#include "Server/WorkerRunner.h"
namespace Script
{

BattleFormula::BattleFormula(const char * filename)
{
	doFile(filename);
}

void BattleFormula::init()
{
	set("Warrior1", 1);
	set("Warrior2", 2);
	set("Assassin1", 3);
	set("Assassin2", 4);
	set("Warlock1", 5);
	set("Warlock2", 6);
	class_add<BattleFormula>("BattleFormula");
	set("battle", this);

	//class_add<GObject::Fighter>("Fighter");
	//class_def<GObject::Fighter>("getId", &GObject::Fighter::getId);

	class_add<Battle::BattleFighter>("BattleFighter");

    //class_add<GObject::ItemLingbaoAttr>("ItemLingbaoAttr");
    //class_def<GObject::ItemLingbaoAttr>("getType", &GObject::ItemLingbaoAttr::getType);
    //class_def<GObject::ItemLingbaoAttr>("getValue", &GObject::ItemLingbaoAttr::getValue);
}

void BattleFormula::postInit()
{
    call<void>("initSeed", IDGenerator::gSeedOidGenerator.ID());
}

float BattleFormula::calcSoulStrenght(GObject::SecondSoul* ss)
{
    return call<float>("calcSoulStrength", ss);
}

float BattleFormula::calcSoulAgility(GObject::SecondSoul* ss)
{
    return call<float>("calcSoulAgility", ss);
}

float BattleFormula::calcSoulPhysique(GObject::SecondSoul* ss)
{
    return call<float>("calcSoulPhysique", ss);
}

float BattleFormula::calcSoulIntelligence(GObject::SecondSoul* ss)
{
    return call<float>("calcSoulIntelligence", ss);
}

float BattleFormula::calcSoulWill(GObject::SecondSoul* ss)
{
    return call<float>("calcSoulWill", ss);
}

float BattleFormula::calcSoulXinxiuAttack(GObject::SecondSoul* ss)
{
    return call<float>("calcSoulXinxiuAttack", ss);
}

float BattleFormula::calcSoulXinxiuAction(GObject::SecondSoul* ss)
{
    return call<float>("calcSoulXinxiuAction", ss);
}

float BattleFormula::calcSoulXinxiuDefend(GObject::SecondSoul* ss)
{
	return call<float>("calcSoulXinxiuDefend", ss);
}

float BattleFormula::calcSoulXinxiuHp(GObject::SecondSoul* ss)
{
	return call<float>("calcSoulXinxiuHp", ss);
}

float BattleFormula::calcStrength( GObject::Fighter * fgt )
{
	return call<float>("calcStrength", fgt);
}

float BattleFormula::calcAgility( GObject::Fighter * fgt )
{
	return call<float>("calcAgility", fgt);
}

float BattleFormula::calcIntelligence( GObject::Fighter * fgt )
{
	return call<float>("calcIntelligence", fgt);
}

float BattleFormula::calcWill( GObject::Fighter * fgt )
{
	return call<float>("calcWill", fgt);
}

float BattleFormula::calcSoul( GObject::Fighter * fgt )
{
	return call<float>("calcSoul", fgt);
}

float BattleFormula::calcAura( GObject::Fighter * fgt )
{
	return call<float>("calcAura", fgt);
}

float BattleFormula::calcAuraMax( GObject::Fighter * fgt )
{
    return call<float>("calcAuraMax", fgt);
}

float BattleFormula::calcTough( GObject::Fighter * fgt, GObject::Fighter * defgt )
{
	return call<float>("calcTough", fgt, defgt);
}

float BattleFormula::calcPhysique( GObject::Fighter * fgt )
{
	return call<float>("calcPhysique", fgt);
}

float BattleFormula::calcAttack( GObject::Fighter * fgt )
{
	return call<float>("calcAttack", fgt);
}

float BattleFormula::calcMagAttack( GObject::Fighter * fgt )
{
	return call<float>("calcMagAttack", fgt);
}

float BattleFormula::calcDefend( GObject::Fighter * fgt )
{
	return call<float>("calcDefend", fgt);
}

float BattleFormula::calcMagDefend( GObject::Fighter * fgt )
{
	return call<float>("calcMagDefend", fgt);
}

float BattleFormula::calcHitrate( GObject::Fighter * fgt, GObject::Fighter * defgt )
{
	return call<float>("calcHitrate", fgt, defgt);
}

float BattleFormula::calcEvade( GObject::Fighter * fgt, GObject::Fighter * defgt )
{
	return call<float>("calcEvade", fgt, defgt);
}

float BattleFormula::calcCritical( GObject::Fighter * fgt, GObject::Fighter * defgt )
{
	return call<float>("calcCritical", fgt, defgt);
}

float BattleFormula::calcCriticalDmg( GObject::Fighter * fgt )
{
	return call<float>("calcCriticalDmg", fgt);
}

float BattleFormula::calcPierce( GObject::Fighter * fgt, GObject::Fighter * defgt )
{
	return call<float>("calcPierce", fgt, defgt);
}

float BattleFormula::calcCounter( GObject::Fighter * fgt, GObject::Fighter * defgt )
{
	return call<float>("calcCounter", fgt, defgt);
}

float BattleFormula::calcMagRes( GObject::Fighter * fgt, GObject::Fighter * defgt )
{
	return call<float>("calcMagRes", fgt, defgt);
}

float BattleFormula::calcHP( GObject::Fighter * fgt )
{
	return call<UInt32>("calcHP", fgt);
}

float BattleFormula::calcAction( GObject::Fighter * fgt )
{
	return call<UInt32>("calcAction", fgt);
}

float BattleFormula::calcBattlePoint(GObject::Fighter * fgt)
{
	return call<float>("calcBattlePoint", fgt);
}

float BattleFormula::calcSkillBattlePoint(UInt8 color, UInt8 level, UInt8 type, UInt8 ssLevel)
{
	return call<float>("calcSkillBattlePoint", color, level, type, ssLevel);
}

float BattleFormula::calcHitRateLevel(GObject::Fighter * fgt)
{
	return call<float>("calcHitRateLevel", fgt);
}

float BattleFormula::calcEvadeLevel(GObject::Fighter * fgt)
{
	return call<float>("calcEvadeLevel", fgt);
}

float BattleFormula::calcCriticalLevel(GObject::Fighter * fgt)
{
	return call<float>("calcCriticalLevel", fgt);
}

float BattleFormula::calcPierceLevel(GObject::Fighter * fgt)
{
	return call<float>("calcPierceLevel", fgt);
}

float BattleFormula::calcCounterLevel(GObject::Fighter * fgt)
{
	return call<float>("calcCounterLevel", fgt);
}

float BattleFormula::calcToughLevel(GObject::Fighter * fgt)
{
	return call<float>("calcToughLevel", fgt);
}

float BattleFormula::calcMagResLevel(GObject::Fighter * fgt)
{
	return call<float>("calcMagResLevel", fgt);
}


float BattleFormula::calcStrength(Battle::BattleFighter * fgt)
{
	return call<float>("calcStrength", fgt);
}

float BattleFormula::calcAgility(Battle::BattleFighter * fgt)
{
	return call<float>("calcAgility", fgt);
}

float BattleFormula::calcIntelligence(Battle::BattleFighter * fgt)
{
	return call<float>("calcIntelligence", fgt);
}

float BattleFormula::calcWill( Battle::BattleFighter* fgt )
{
	return call<float>("calcWill", fgt);
}

float BattleFormula::calcSoul( Battle::BattleFighter * fgt )
{
	return call<float>("calcSoul", fgt);
}

float BattleFormula::calcAura( Battle::BattleFighter * fgt )
{
	return call<float>("calcAura", fgt);
}

float BattleFormula::calcAuraMax( Battle::BattleFighter * fgt )
{
	return call<float>("calcAuraMax", fgt);
}

float BattleFormula::calcTough( Battle::BattleFighter * fgt, Battle::BattleFighter * defgt )
{
	return call<float>("calcTough", fgt, defgt);
}

float BattleFormula::calcPhysique(Battle::BattleFighter * fgt)
{
	return call<float>("calcPhysique", fgt);
}

float BattleFormula::calcAttack(Battle::BattleFighter * fgt)
{
	return call<float>("calcAttack", fgt);
}

float BattleFormula::calcMagAttack(Battle::BattleFighter * fgt)
{
	return call<float>("calcMagAttack", fgt);
}

float BattleFormula::calcDefend(Battle::BattleFighter * fgt)
{
	return call<float>("calcDefend", fgt);
}

float BattleFormula::calcMagDefend(Battle::BattleFighter * fgt)
{
	return call<float>("calcMagDefend", fgt);
}

float BattleFormula::calcHitrate(Battle::BattleFighter * fgt, Battle::BattleFighter * defgt)
{
	return call<float>("calcHitrate", fgt, defgt);
}

float BattleFormula::calcEvade(Battle::BattleFighter * fgt, Battle::BattleFighter * defgt)
{
	return call<float>("calcEvade", fgt, defgt);
}

float BattleFormula::calcCritical(Battle::BattleFighter * fgt, Battle::BattleFighter * defgt)
{
	return call<float>("calcCritical", fgt, defgt);
}

float BattleFormula::calcCriticalDmg(Battle::BattleFighter * fgt)
{
	return call<float>("calcCriticalDmg", fgt);
}

float BattleFormula::calcHitRateLevel(Battle::BattleFighter * fgt)
{
	return call<float>("calcHitRateLevel", fgt);
}

float BattleFormula::calcEvadeLevel(Battle::BattleFighter * fgt)
{
	return call<float>("calcEvadeLevel", fgt);
}

float BattleFormula::calcCriticalLevel(Battle::BattleFighter * fgt)
{
	return call<float>("calcCriticalLevel", fgt);
}

float BattleFormula::calcPierceLevel(Battle::BattleFighter * fgt)
{
	return call<float>("calcPierceLevel", fgt);
}

float BattleFormula::calcCounterLevel(Battle::BattleFighter * fgt)
{
	return call<float>("calcCounterLevel", fgt);
}

float BattleFormula::calcToughLevel(Battle::BattleFighter * fgt)
{
	return call<float>("calcToughLevel", fgt);
}

float BattleFormula::calcMagResLevel(Battle::BattleFighter * fgt)
{
	return call<float>("calcMagResLevel", fgt);
}


lua_tinker::table BattleFormula::getFactor(UInt8 klass, UInt8 career, UInt8 level)
{
	return call<lua_tinker::table>("getFactor", klass, career, level);
}
float BattleFormula::calcPierce(Battle::BattleFighter * fgt, Battle::BattleFighter * defgt)
{
	return call<float>("calcPierce", fgt, defgt);
}

float BattleFormula::calcCounter(Battle::BattleFighter * fgt, Battle::BattleFighter * defgt)
{
	return call<float>("calcCounter", fgt, defgt);
}

float BattleFormula::calcMagRes(Battle::BattleFighter * fgt, Battle::BattleFighter * defgt)
{
	return call<float>("calcMagRes", fgt, defgt);
}

float BattleFormula::calcHP(Battle::BattleFighter * fgt)
{
	return call<UInt32>("calcHP", fgt);
}

float BattleFormula::calcAction(Battle::BattleFighter * fgt)
{
	return call<UInt32>("calcAction", fgt);
}

float BattleFormula::calcDamage( float atk, float def, float atklvl, float toughFactor, float dmgreduce, float attackpierce )
{
	return call<float>("calcDamage", atk, def, atklvl, toughFactor, dmgreduce, attackpierce);
}

float BattleFormula::calcAutoBattle( float mybp, float theirbp )
{
	return call<float>("calcAutoBattle", mybp, theirbp);
}

float BattleFormula::calcPracticeInc( GObject::Fighter * fgt ,bool merged)
{
	return call<float>("calcPracticeInc", fgt, merged);
}

float BattleFormula::calcBasePExp( GObject::Fighter * fgt )
{
	return call<float>("calcBasePExp", fgt);
}

float BattleFormula::calcPExpNoBuf( GObject::Fighter * fgt )
{
	return call<float>("calcPExpNoBuf", fgt);
}

float BattleFormula::calcClanTechAddon(UInt16 id, UInt8 lvl)
{
    return call<float>("calcClanTechAddon", id, lvl);
}

UInt32 BattleFormula::calcTaskAward( UInt8 type, UInt8 color, UInt8 lvl )
{
	return call<UInt32>("GetTaskAwardFactor", type, color, lvl);
}

UInt32 BattleFormula::calcLingbaoBattlePoint(const GObject::ItemLingbaoAttr *lbatr)
{
    return call<UInt32>("calcLingbaoBattlePoint", lbatr);
}

BattleFormula * BattleFormula::getCurrent()
{
	WorkerRunner<>& worker = WorkerThread<WorkerRunner<> >::LocalWorker();
	switch(worker.GetThreadID())
	{
	case WORKER_THREAD_WORLD:
		return reinterpret_cast<GObject::World &>(worker).getBattleFormula();
	case WORKER_THREAD_COUNTRY_1:
	case WORKER_BATTLE:
	case WORKER_THREAD_NEUTRAL:
		return reinterpret_cast<GObject::Country &>(worker).GetBattleFormula();
    //case WORKER_THREAD_SORT:
    //    return reinterpret_cast<GObject::SortWorker &>(worker).getBattleFormula();
	}
	return NULL;
}

float BattleFormula::calcCriticalDef(Battle::BattleFighter* defgt)
{
	return call<float>("calcCriticalDef", defgt);
}

float BattleFormula::calcPierceDef(Battle::BattleFighter* defgt)
{
	return call<float>("calcPierceDef", defgt);
}

float BattleFormula::calcCounterDef(Battle::BattleFighter* defgt)
{
	return call<float>("calcCounterDef", defgt);
}

float BattleFormula::calcAttackPierce(Battle::BattleFighter* fgt)
{
	return call<float>("calcAttackPierce", fgt);
}

}


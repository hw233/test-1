#include "Config.h"
#include "BattleFormula.h"
#include "Battle/BattleFighter.h"
#include "GObject/Fighter.h"
#include "GObject/SecondSoul.h"
#include "GObject/Country.h"
#include "GObject/World.h"
#include "Server/OidGenerator.h"

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
	class_add<GObject::Fighter>("Fighter");
	class_def<GObject::Fighter>("getId", &GObject::Fighter::getId);
	class_def<GObject::Fighter>("getClass", &GObject::Fighter::getClass);
	class_def<GObject::Fighter>("getLevel", &GObject::Fighter::getLevel);
	class_def<GObject::Fighter>("getColor", &GObject::Fighter::getColor);
	class_def<GObject::Fighter>("getPotential", &GObject::Fighter::getPotential);
	class_def<GObject::Fighter>("getCapacity", &GObject::Fighter::getCapacity);
	class_def<GObject::Fighter>("getExtraStrength", &GObject::Fighter::getExtraStrength);
	class_def<GObject::Fighter>("getExtraPhysique", &GObject::Fighter::getExtraPhysique);
	class_def<GObject::Fighter>("getExtraAgility", &GObject::Fighter::getExtraAgility);
	class_def<GObject::Fighter>("getExtraIntelligence", &GObject::Fighter::getExtraIntelligence);
	class_def<GObject::Fighter>("getExtraWill", &GObject::Fighter::getExtraWill);
	class_def<GObject::Fighter>("getExtraSoul", &GObject::Fighter::getExtraSoul);
	class_def<GObject::Fighter>("getExtraAura", &GObject::Fighter::getExtraAura);
	class_def<GObject::Fighter>("getExtraAuraMax", &GObject::Fighter::getExtraAuraMax);
	class_def<GObject::Fighter>("getExtraTough", &GObject::Fighter::getExtraTough);
	class_def<GObject::Fighter>("getExtraStrengthPercent", &GObject::Fighter::getExtraStrengthP);
	class_def<GObject::Fighter>("getExtraPhysiquePercent", &GObject::Fighter::getExtraPhysiqueP);
	class_def<GObject::Fighter>("getExtraAgilityPercent", &GObject::Fighter::getExtraAgilityP);
	class_def<GObject::Fighter>("getExtraIntelligencePercent", &GObject::Fighter::getExtraIntelligenceP);
	class_def<GObject::Fighter>("getExtraWillPercent", &GObject::Fighter::getExtraWillP);
	class_def<GObject::Fighter>("getExtraSoulPercent", &GObject::Fighter::getExtraSoulP);
	class_def<GObject::Fighter>("getExtraAuraPercent", &GObject::Fighter::getExtraAuraP);
	class_def<GObject::Fighter>("getExtraAuraMaxPercent", &GObject::Fighter::getExtraAuraMaxP);
	class_def<GObject::Fighter>("getExtraAttack", &GObject::Fighter::getExtraAttack);
	class_def<GObject::Fighter>("getExtraAttackPercent", &GObject::Fighter::getExtraAttackP);
	class_def<GObject::Fighter>("getExtraMagAttack", &GObject::Fighter::getExtraMagAttack);
	class_def<GObject::Fighter>("getExtraMagAttackPercent", &GObject::Fighter::getExtraMagAttackP);
	class_def<GObject::Fighter>("getExtraDefend", &GObject::Fighter::getExtraDefend);
	class_def<GObject::Fighter>("getExtraDefendPercent", &GObject::Fighter::getExtraDefendP);
	class_def<GObject::Fighter>("getExtraMagDefend", &GObject::Fighter::getExtraMagDefend);
	class_def<GObject::Fighter>("getExtraMagDefendPercent", &GObject::Fighter::getExtraMagDefendP);
	class_def<GObject::Fighter>("getExtraHP", &GObject::Fighter::getExtraHP);
	class_def<GObject::Fighter>("getExtraHPPercent", &GObject::Fighter::getExtraHPP);
	class_def<GObject::Fighter>("getExtraAction", &GObject::Fighter::getExtraAction);
	class_def<GObject::Fighter>("getExtraActionPercent", &GObject::Fighter::getExtraActionP);
	class_def<GObject::Fighter>("getExtraHitrate", &GObject::Fighter::getExtraHitrate);
	class_def<GObject::Fighter>("getExtraEvade", &GObject::Fighter::getExtraEvade);
	class_def<GObject::Fighter>("getExtraCritical", &GObject::Fighter::getExtraCritical);
	class_def<GObject::Fighter>("getExtraCriticalDmg", &GObject::Fighter::getExtraCriticalDmg);
	class_def<GObject::Fighter>("getExtraPierce", &GObject::Fighter::getExtraPierce);
	class_def<GObject::Fighter>("getExtraCounter", &GObject::Fighter::getExtraCounter);
	class_def<GObject::Fighter>("getExtraMagRes", &GObject::Fighter::getExtraMagRes);
	class_def<GObject::Fighter>("isNpc", &GObject::Fighter::isNpc);
	class_def<GObject::Fighter>("isMale", &GObject::Fighter::isMale);
	class_def<GObject::Fighter>("isBoy", &GObject::Fighter::isBoy);
	class_def<GObject::Fighter>("getSex", &GObject::Fighter::getSex);
	class_def<GObject::Fighter>("getBaseStrength", &GObject::Fighter::getBaseStrength);
	class_def<GObject::Fighter>("getBasePhysique", &GObject::Fighter::getBasePhysique);
	class_def<GObject::Fighter>("getBaseAgility", &GObject::Fighter::getBaseAgility);
	class_def<GObject::Fighter>("getBaseIntelligence", &GObject::Fighter::getBaseIntelligence);
	class_def<GObject::Fighter>("getBaseWill", &GObject::Fighter::getBaseWill);
	class_def<GObject::Fighter>("getBaseSoul", &GObject::Fighter::getBaseSoul);
	class_def<GObject::Fighter>("getBaseAura", &GObject::Fighter::getBaseAura);
	class_def<GObject::Fighter>("getBaseAuraMax", &GObject::Fighter::getBaseAuraMax);
	class_def<GObject::Fighter>("getBaseTough", &GObject::Fighter::getBaseTough);
	class_def<GObject::Fighter>("getBaseAttack", &GObject::Fighter::getBaseAttack);
	class_def<GObject::Fighter>("getBaseMagAttack", &GObject::Fighter::getBaseMagAttack);
	class_def<GObject::Fighter>("getBaseDefend", &GObject::Fighter::getBaseDefend);
	class_def<GObject::Fighter>("getBaseMagDefend", &GObject::Fighter::getBaseMagDefend);
	class_def<GObject::Fighter>("getBaseHP", &GObject::Fighter::getBaseHP);
	class_def<GObject::Fighter>("getBaseAction", &GObject::Fighter::getBaseAction);
	class_def<GObject::Fighter>("getBaseHitrate", &GObject::Fighter::getBaseHitrate);
	class_def<GObject::Fighter>("getBaseEvade", &GObject::Fighter::getBaseEvade);
	class_def<GObject::Fighter>("getBaseCritical", &GObject::Fighter::getBaseCritical);
	class_def<GObject::Fighter>("getBaseCriticalDmg", &GObject::Fighter::getBaseCriticalDmg);
	class_def<GObject::Fighter>("getBasePierce", &GObject::Fighter::getBasePierce);
	class_def<GObject::Fighter>("getBaseCounter", &GObject::Fighter::getBaseCounter);
	class_def<GObject::Fighter>("getBaseMagRes", &GObject::Fighter::getBaseMagRes);
	class_def<GObject::Fighter>("getPracticePlace", &GObject::Fighter::getPracticePlace);
	class_def<GObject::Fighter>("isGoldPractice", &GObject::Fighter::isGoldPractice);
	class_def<GObject::Fighter>("getClanTechAddon", &GObject::Fighter::getClanTechAddon);
	class_def<GObject::Fighter>("getAcuPraAdd", &GObject::Fighter::getAcuPraAdd);
	class_def<GObject::Fighter>("getPracticeBufFactor", &GObject::Fighter::getPracticeBufFactor);
	class_def<GObject::Fighter>("getPracticeIncByDiamond", &GObject::Fighter::getPracticeIncByDiamond);
	class_def<GObject::Fighter>("getPracticeIncByQQVip", &GObject::Fighter::getPracticeIncByQQVip);
	class_def<GObject::Fighter>("getExtraHitrateLevel", &GObject::Fighter::getExtraHitrateLevel);
	class_def<GObject::Fighter>("getExtraEvadeLevel", &GObject::Fighter::getExtraEvadeLevel);
	class_def<GObject::Fighter>("getExtraCriticalLevel", &GObject::Fighter::getExtraCriticalLevel);
	class_def<GObject::Fighter>("getExtraPierceLevel", &GObject::Fighter::getExtraPierceLevel);
	class_def<GObject::Fighter>("getExtraCounterLevel", &GObject::Fighter::getExtraCounterLevel);
	class_def<GObject::Fighter>("getExtraToughLevel", &GObject::Fighter::getExtraToughLevel);
	class_def<GObject::Fighter>("getExtraMagResLevel", &GObject::Fighter::getExtraMagResLevel);

	class_def<GObject::Fighter>("getSoulPracticeAddOn", &GObject::Fighter::getSoulPracticeAddOn);
	class_def<GObject::Fighter>("getSoulPracticeFactor", &GObject::Fighter::getSoulPracticeFactor);

	class_add<Battle::BattleFighter>("BattleFighter");
	class_def<Battle::BattleFighter>("getId", &Battle::BattleFighter::getId);
	class_def<Battle::BattleFighter>("getClass", &Battle::BattleFighter::getClass);
	class_def<Battle::BattleFighter>("getLevel", &Battle::BattleFighter::getLevel);
	class_def<Battle::BattleFighter>("getColor", &Battle::BattleFighter::getColor);
	class_def<Battle::BattleFighter>("getPotential", &Battle::BattleFighter::getPotential);
	class_def<Battle::BattleFighter>("getExtraStrength", &Battle::BattleFighter::getExtraStrength);
	class_def<Battle::BattleFighter>("getExtraPhysique", &Battle::BattleFighter::getExtraPhysique);
	class_def<Battle::BattleFighter>("getExtraAgility", &Battle::BattleFighter::getExtraAgility);
	class_def<Battle::BattleFighter>("getExtraIntelligence", &Battle::BattleFighter::getExtraIntelligence);
	class_def<Battle::BattleFighter>("getExtraWill", &Battle::BattleFighter::getExtraWill);
	class_def<Battle::BattleFighter>("getExtraSoul", &Battle::BattleFighter::getExtraSoul);
	class_def<Battle::BattleFighter>("getExtraAura", &Battle::BattleFighter::getExtraAura);
	class_def<Battle::BattleFighter>("getExtraAuraMax", &Battle::BattleFighter::getExtraAuraMax);
	class_def<Battle::BattleFighter>("getExtraTough", &Battle::BattleFighter::getExtraTough);
	class_def<Battle::BattleFighter>("getExtraStrengthPercent", &Battle::BattleFighter::getExtraStrengthP);
	class_def<Battle::BattleFighter>("getExtraPhysiquePercent", &Battle::BattleFighter::getExtraPhysiqueP);
	class_def<Battle::BattleFighter>("getExtraAgilityPercent", &Battle::BattleFighter::getExtraAgilityP);
	class_def<Battle::BattleFighter>("getExtraIntelligencePercent", &Battle::BattleFighter::getExtraIntelligenceP);
	class_def<Battle::BattleFighter>("getExtraWillPercent", &Battle::BattleFighter::getExtraWillP);
	class_def<Battle::BattleFighter>("getExtraSoulPercent", &Battle::BattleFighter::getExtraSoulP);
	class_def<Battle::BattleFighter>("getExtraAuraPercent", &Battle::BattleFighter::getExtraAuraP);
	class_def<Battle::BattleFighter>("getExtraAuraMaxPercent", &Battle::BattleFighter::getExtraAuraMaxP);
	class_def<Battle::BattleFighter>("getExtraAttack", &Battle::BattleFighter::getExtraAttack);
	class_def<Battle::BattleFighter>("getExtraAttackPercent", &Battle::BattleFighter::getExtraAttackP);
	class_def<Battle::BattleFighter>("getExtraMagAttack", &Battle::BattleFighter::getExtraMagAttack);
	class_def<Battle::BattleFighter>("getExtraMagAttackPercent", &Battle::BattleFighter::getExtraMagAttackP);
	class_def<Battle::BattleFighter>("getExtraDefend", &Battle::BattleFighter::getExtraDefend);
	class_def<Battle::BattleFighter>("getExtraDefendPercent", &Battle::BattleFighter::getExtraDefendP);
	class_def<Battle::BattleFighter>("getExtraMagDefend", &Battle::BattleFighter::getExtraMagDefend);
	class_def<Battle::BattleFighter>("getExtraMagDefendPercent", &Battle::BattleFighter::getExtraMagDefendP);
	class_def<Battle::BattleFighter>("getExtraHP", &Battle::BattleFighter::getExtraHP);
	class_def<Battle::BattleFighter>("getExtraHPPercent", &Battle::BattleFighter::getExtraHPP);
	class_def<Battle::BattleFighter>("getExtraAction", &Battle::BattleFighter::getExtraAction);
	class_def<Battle::BattleFighter>("getExtraActionPercent", &Battle::BattleFighter::getExtraActionP);
	class_def<Battle::BattleFighter>("getExtraHitrate", &Battle::BattleFighter::getExtraHitrate);
	class_def<Battle::BattleFighter>("getExtraEvade", &Battle::BattleFighter::getExtraEvade);
	class_def<Battle::BattleFighter>("getExtraCritical", &Battle::BattleFighter::getExtraCritical);
	class_def<Battle::BattleFighter>("getExtraCriticalDmg", &Battle::BattleFighter::getExtraCriticalDmg);
	class_def<Battle::BattleFighter>("getExtraPierce", &Battle::BattleFighter::getExtraPierce);
	class_def<Battle::BattleFighter>("getExtraCounter", &Battle::BattleFighter::getExtraCounter);
	class_def<Battle::BattleFighter>("getExtraMagRes", &Battle::BattleFighter::getExtraMagRes);
	class_def<Battle::BattleFighter>("isNpc", &Battle::BattleFighter::isNpc);
	class_def<Battle::BattleFighter>("isMale", &Battle::BattleFighter::isMale);
	class_def<Battle::BattleFighter>("isBoy", &Battle::BattleFighter::isBoy);
	class_def<Battle::BattleFighter>("getSex", &Battle::BattleFighter::getSex);
	class_def<Battle::BattleFighter>("getBaseStrength", &Battle::BattleFighter::getBaseStrength);
	class_def<Battle::BattleFighter>("getBasePhysique", &Battle::BattleFighter::getBasePhysique);
	class_def<Battle::BattleFighter>("getBaseAgility", &Battle::BattleFighter::getBaseAgility);
	class_def<Battle::BattleFighter>("getBaseIntelligence", &Battle::BattleFighter::getBaseIntelligence);
	class_def<Battle::BattleFighter>("getBaseWill", &Battle::BattleFighter::getBaseWill);
	class_def<Battle::BattleFighter>("getBaseSoul", &Battle::BattleFighter::getBaseSoul);
	class_def<Battle::BattleFighter>("getBaseAura", &Battle::BattleFighter::getBaseAura);
	class_def<Battle::BattleFighter>("getBaseAuraMax", &Battle::BattleFighter::getBaseAuraMax);
	class_def<Battle::BattleFighter>("getBaseTough", &Battle::BattleFighter::getBaseTough);
	class_def<Battle::BattleFighter>("getBaseAttack", &Battle::BattleFighter::getBaseAttack);
	class_def<Battle::BattleFighter>("getBaseMagAttack", &Battle::BattleFighter::getBaseMagAttack);
	class_def<Battle::BattleFighter>("getBaseDefend", &Battle::BattleFighter::getBaseDefend);
	class_def<Battle::BattleFighter>("getBaseMagDefend", &Battle::BattleFighter::getBaseMagDefend);
	class_def<Battle::BattleFighter>("getBaseHP", &Battle::BattleFighter::getBaseHP);
	class_def<Battle::BattleFighter>("getBaseAction", &Battle::BattleFighter::getBaseAction);
	class_def<Battle::BattleFighter>("getBaseHitrate", &Battle::BattleFighter::getBaseHitrate);
	class_def<Battle::BattleFighter>("getBaseEvade", &Battle::BattleFighter::getBaseEvade);
	class_def<Battle::BattleFighter>("getBaseCritical", &Battle::BattleFighter::getBaseCritical);
	class_def<Battle::BattleFighter>("getBaseCriticalDmg", &Battle::BattleFighter::getBaseCriticalDmg);
	class_def<Battle::BattleFighter>("getBasePierce", &Battle::BattleFighter::getBasePierce);
	class_def<Battle::BattleFighter>("getBaseCounter", &Battle::BattleFighter::getBaseCounter);
	class_def<Battle::BattleFighter>("getBaseMagRes", &Battle::BattleFighter::getBaseMagRes);
	class_def<Battle::BattleFighter>("getExtraHitrateLevel", &Battle::BattleFighter::getExtraHitrateLevel);
	class_def<Battle::BattleFighter>("getExtraEvadeLevel", &Battle::BattleFighter::getExtraEvadeLevel);
	class_def<Battle::BattleFighter>("getExtraCriticalLevel", &Battle::BattleFighter::getExtraCriticalLevel);
	class_def<Battle::BattleFighter>("getExtraPierceLevel", &Battle::BattleFighter::getExtraPierceLevel);
	class_def<Battle::BattleFighter>("getExtraCounterLevel", &Battle::BattleFighter::getExtraCounterLevel);
	class_def<Battle::BattleFighter>("getExtraToughLevel", &Battle::BattleFighter::getExtraToughLevel);
	class_def<Battle::BattleFighter>("getExtraMagResLevel", &Battle::BattleFighter::getExtraMagResLevel);

	class_add<GObject::SecondSoul>("SecondSoul");
	class_def<GObject::SecondSoul>("getClass", &GObject::SecondSoul::getClass);
	class_def<GObject::SecondSoul>("getPracticeLevel", &GObject::SecondSoul::getPracticeLevel);
	class_def<GObject::SecondSoul>("getStateLevel", &GObject::SecondSoul::getStateLevel);
	class_def<GObject::SecondSoul>("getXinxiu", &GObject::SecondSoul::getXinxiu);
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

float BattleFormula::calcDamage( float atk, float def, float atklvl, float toughFactor, float dmgreduce )
{
	return call<float>("calcDamage", atk, def, atklvl, toughFactor, dmgreduce);
}

float BattleFormula::calcAutoBattle( float mybp, float theirbp )
{
	return call<float>("calcAutoBattle", mybp, theirbp);
}

float BattleFormula::calcPracticeInc( GObject::Fighter * fgt )
{
	return call<float>("calcPracticeInc", fgt);
}

float BattleFormula::calcClanTechAddon(UInt16 id, UInt8 lvl)
{
    return call<float>("calcClanTechAddon", id, lvl);
}

UInt32 BattleFormula::calcTaskAward( UInt8 type, UInt8 color, UInt8 lvl )
{
	return call<UInt32>("GetTaskAwardFactor", type, color, lvl);
}

BattleFormula * BattleFormula::getCurrent()
{
	WorkerRunner<>& worker = WorkerThread<WorkerRunner<> >::LocalWorker();
	switch(worker.GetThreadID())
	{
	case WORKER_THREAD_WORLD:
		return reinterpret_cast<GObject::World &>(worker).getBattleFormula();
	case WORKER_THREAD_COUNTRY_1:
	case WORKER_THREAD_COUNTRY_2:
	case WORKER_THREAD_NEUTRAL:
		return reinterpret_cast<GObject::Country &>(worker).GetBattleFormula();
	}
	return NULL;
}

}


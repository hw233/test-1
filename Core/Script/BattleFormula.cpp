#include "Config.h"
#include "BattleFormula.h"
#include "Battle/BattleFighter.h"
#include "GObject/Fighter.h"
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
	class_def<BattleFormula>("reloadSkillData", &BattleFormula::reloadSkillData);
	set("battle", this);
	class_add<GObject::Fighter>("Fighter");
	class_def<GObject::Fighter>("getId", &GObject::Fighter::getId);
	class_def<GObject::Fighter>("getClass", &GObject::Fighter::getClass);
	class_def<GObject::Fighter>("getLevel", &GObject::Fighter::getLevel);
	class_def<GObject::Fighter>("getColor", &GObject::Fighter::getColor);
	class_def<GObject::Fighter>("getPotential", &GObject::Fighter::getPotential);
	class_def<GObject::Fighter>("getExtraStrength", &GObject::Fighter::getExtraStrength);
	class_def<GObject::Fighter>("getExtraPhysique", &GObject::Fighter::getExtraPhysique);
	class_def<GObject::Fighter>("getExtraAgility", &GObject::Fighter::getExtraAgility);
	class_def<GObject::Fighter>("getExtraIntelligence", &GObject::Fighter::getExtraIntelligence);
	class_def<GObject::Fighter>("getExtraStrengthPercent", &GObject::Fighter::getExtraStrengthP);
	class_def<GObject::Fighter>("getExtraPhysiquePercent", &GObject::Fighter::getExtraPhysiqueP);
	class_def<GObject::Fighter>("getExtraAgilityPercent", &GObject::Fighter::getExtraAgilityP);
	class_def<GObject::Fighter>("getExtraIntelligencePercent", &GObject::Fighter::getExtraIntelligenceP);
	class_def<GObject::Fighter>("getExtraAttack", &GObject::Fighter::getExtraAttack);
	class_def<GObject::Fighter>("getExtraAttackPercent", &GObject::Fighter::getExtraAttackP);
	class_def<GObject::Fighter>("getExtraDefend", &GObject::Fighter::getExtraDefend);
	class_def<GObject::Fighter>("getExtraDefendPercent", &GObject::Fighter::getExtraDefendP);
	class_def<GObject::Fighter>("getExtraHP", &GObject::Fighter::getExtraHP);
	class_def<GObject::Fighter>("getExtraHPPercent", &GObject::Fighter::getExtraHPP);
	class_def<GObject::Fighter>("getExtraAction", &GObject::Fighter::getExtraAction);
	class_def<GObject::Fighter>("getExtraHitrate", &GObject::Fighter::getExtraHitrate);
	class_def<GObject::Fighter>("getExtraEvade", &GObject::Fighter::getExtraEvade);
	class_def<GObject::Fighter>("getExtraCritical", &GObject::Fighter::getExtraCritical);
	class_def<GObject::Fighter>("getExtraPierce", &GObject::Fighter::getExtraPierce);
	class_def<GObject::Fighter>("getExtraCounter", &GObject::Fighter::getExtraCounter);
	class_def<GObject::Fighter>("isNpc", &GObject::Fighter::isNpc);
	class_def<GObject::Fighter>("getBaseStrength", &GObject::Fighter::getBaseStrength);
	class_def<GObject::Fighter>("getBasePhysique", &GObject::Fighter::getBasePhysique);
	class_def<GObject::Fighter>("getBaseAgility", &GObject::Fighter::getBaseAgility);
	class_def<GObject::Fighter>("getBaseIntelligence", &GObject::Fighter::getBaseIntelligence);
	class_def<GObject::Fighter>("getBaseAttack", &GObject::Fighter::getBaseAttack);
	class_def<GObject::Fighter>("getBaseDefend", &GObject::Fighter::getBaseDefend);
	class_def<GObject::Fighter>("getBaseHP", &GObject::Fighter::getBaseHP);
	class_def<GObject::Fighter>("getBaseAction", &GObject::Fighter::getBaseAction);
	class_def<GObject::Fighter>("getBaseHitrate", &GObject::Fighter::getBaseHitrate);
	class_def<GObject::Fighter>("getBaseEvade", &GObject::Fighter::getBaseEvade);
	class_def<GObject::Fighter>("getBaseCritical", &GObject::Fighter::getBaseCritical);
	class_def<GObject::Fighter>("getBasePierce", &GObject::Fighter::getBasePierce);
	class_def<GObject::Fighter>("getBaseCounter", &GObject::Fighter::getBaseCounter);

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
	class_def<Battle::BattleFighter>("getExtraStrengthPercent", &Battle::BattleFighter::getExtraStrengthP);
	class_def<Battle::BattleFighter>("getExtraPhysiquePercent", &Battle::BattleFighter::getExtraPhysiqueP);
	class_def<Battle::BattleFighter>("getExtraAgilityPercent", &Battle::BattleFighter::getExtraAgilityP);
	class_def<Battle::BattleFighter>("getExtraIntelligencePercent", &Battle::BattleFighter::getExtraIntelligenceP);
	class_def<Battle::BattleFighter>("getExtraAttack", &Battle::BattleFighter::getExtraAttack);
	class_def<Battle::BattleFighter>("getExtraAttackPercent", &Battle::BattleFighter::getExtraAttackP);
	class_def<Battle::BattleFighter>("getExtraDefend", &Battle::BattleFighter::getExtraDefend);
	class_def<Battle::BattleFighter>("getExtraDefendPercent", &Battle::BattleFighter::getExtraDefendP);
	class_def<Battle::BattleFighter>("getExtraHP", &Battle::BattleFighter::getExtraHP);
	class_def<Battle::BattleFighter>("getExtraHPPercent", &Battle::BattleFighter::getExtraHPP);
	class_def<Battle::BattleFighter>("getExtraAction", &Battle::BattleFighter::getExtraAction);
	class_def<Battle::BattleFighter>("getExtraHitrate", &Battle::BattleFighter::getExtraHitrate);
	class_def<Battle::BattleFighter>("getExtraEvade", &Battle::BattleFighter::getExtraEvade);
	class_def<Battle::BattleFighter>("getExtraCritical", &Battle::BattleFighter::getExtraCritical);
	class_def<Battle::BattleFighter>("getExtraPierce", &Battle::BattleFighter::getExtraPierce);
	class_def<Battle::BattleFighter>("getExtraCounter", &Battle::BattleFighter::getExtraCounter);
	class_def<Battle::BattleFighter>("isNpc", &Battle::BattleFighter::isNpc);
	class_def<Battle::BattleFighter>("getBaseStrength", &Battle::BattleFighter::getBaseStrength);
	class_def<Battle::BattleFighter>("getBasePhysique", &Battle::BattleFighter::getBasePhysique);
	class_def<Battle::BattleFighter>("getBaseAgility", &Battle::BattleFighter::getBaseAgility);
	class_def<Battle::BattleFighter>("getBaseIntelligence", &Battle::BattleFighter::getBaseIntelligence);
	class_def<Battle::BattleFighter>("getBaseAttack", &Battle::BattleFighter::getBaseAttack);
	class_def<Battle::BattleFighter>("getBaseDefend", &Battle::BattleFighter::getBaseDefend);
	class_def<Battle::BattleFighter>("getBaseHP", &Battle::BattleFighter::getBaseHP);
	class_def<Battle::BattleFighter>("getBaseAction", &Battle::BattleFighter::getBaseAction);
	class_def<Battle::BattleFighter>("getBaseHitrate", &Battle::BattleFighter::getBaseHitrate);
	class_def<Battle::BattleFighter>("getBaseEvade", &Battle::BattleFighter::getBaseEvade);
	class_def<Battle::BattleFighter>("getBaseCritical", &Battle::BattleFighter::getBaseCritical);
	class_def<Battle::BattleFighter>("getBasePierce", &Battle::BattleFighter::getBasePierce);
	class_def<Battle::BattleFighter>("getBaseCounter", &Battle::BattleFighter::getBaseCounter);
}

void BattleFormula::postInit()
{
	call<void>("initSeed", IDGenerator::gSeedOidGenerator.ID());
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

float BattleFormula::calcPhysique( GObject::Fighter * fgt )
{
	return call<float>("calcPhysique", fgt);
}

float BattleFormula::calcAttack( GObject::Fighter * fgt )
{
	return call<float>("calcAttack", fgt);
}

float BattleFormula::calcDefend( GObject::Fighter * fgt )
{
	return call<float>("calcDefend", fgt);
}

float BattleFormula::calcHitrate( GObject::Fighter * fgt )
{
	return call<float>("calcHitrate", fgt);
}

float BattleFormula::calcEvade( GObject::Fighter * fgt )
{
	return call<float>("calcEvade", fgt);
}

float BattleFormula::calcCritical( GObject::Fighter * fgt )
{
	return call<float>("calcCritical", fgt);
}

float BattleFormula::calcPierce( GObject::Fighter * fgt )
{
	return call<float>("calcPierce", fgt);
}

float BattleFormula::calcCounter( GObject::Fighter * fgt )
{
	return call<float>("calcCounter", fgt);
}

UInt32 BattleFormula::calcHP( GObject::Fighter * fgt )
{
	return call<UInt32>("calcHP", fgt);
}

UInt32 BattleFormula::calcAction( GObject::Fighter * fgt )
{
	return call<UInt32>("calcAction", fgt);
}

float BattleFormula::calcBattlePoint(GObject::Fighter * fgt)
{
	return call<float>("calcBattlePoint", fgt);
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

float BattleFormula::calcPhysique(Battle::BattleFighter * fgt)
{
	return call<float>("calcPhysique", fgt);
}

float BattleFormula::calcAttack(Battle::BattleFighter * fgt)
{
	return call<float>("calcAttack", fgt);
}

float BattleFormula::calcDefend(Battle::BattleFighter * fgt)
{
	return call<float>("calcDefend", fgt);
}

float BattleFormula::calcHitrate(Battle::BattleFighter * fgt)
{
	return call<float>("calcHitrate", fgt);
}

float BattleFormula::calcEvade(Battle::BattleFighter * fgt)
{
	return call<float>("calcEvade", fgt);
}

float BattleFormula::calcCritical(Battle::BattleFighter * fgt)
{
	return call<float>("calcCritical", fgt);
}
lua_tinker::table BattleFormula::getFactor(UInt8 klass, UInt8 career, UInt8 level)
{
	return call<lua_tinker::table>("getFactor", klass, career, level);
}
float BattleFormula::calcPierce(Battle::BattleFighter * fgt)
{
	return call<float>("calcPierce", fgt);
}

float BattleFormula::calcCounter(Battle::BattleFighter * fgt)
{
	return call<float>("calcCounter", fgt);
}

UInt32 BattleFormula::calcHP(Battle::BattleFighter * fgt)
{
	return call<UInt32>("calcHP", fgt);
}

UInt32 BattleFormula::calcAction(Battle::BattleFighter * fgt)
{
	return call<UInt32>("calcAction", fgt);
}

UInt32 BattleFormula::calcDamage( float atk, float def )
{
	return call<UInt32>("calcDamage", atk, def);
}

float BattleFormula::calcAutoBattle( float mybp, float theirbp )
{
	return call<float>("calcAutoBattle", mybp, theirbp);
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

void BattleFormula::reloadSkillData()
{
	Table t = get<Table>("skillData");
	for(int i = 0; i < 3; ++ i)
	{
		for(int j = 0; j < 10; ++ j)
		{
			UInt32 skillId = (i + 1) * 100 + j + 1;
			Table skillT = t.get<Table>(skillId);
			int size = skillT.size();
			if(size > 40)
				size = 40;
			for(int k = 0; k < size; ++ k)
			{
				Table skillS = skillT.get<Table>(k + 1);
				SkillData& sd = _skillData[i][j][k];
				sd.rate = skillS.get<UInt32>("rate");
				switch(skillId)
				{
				case 101:
				case 102:
					sd.value1 = skillS.get<float>("damage");
					break;
				case 103:
				case 104:
				case 105:
				case 109:
				case 110:
				case 206:
				case 207:
				case 210:
					sd.value1 = skillS.get<float>("factor");
					break;
				case 107:
				case 108:
				case 301:
				case 302:
				case 310:
					sd.value1 = skillS.get<float>("percent") / 100.0f;
					break;
				case 305:
				case 306:
				case 307:
				case 309:
					sd.value1 = skillS.get<float>("percent");
					break;
				case 106:
					sd.value1 = skillS.get<float>("base");
					sd.value2 = skillS.get<float>("factor");
					break;
				case 201:
					sd.value1 = skillS.get<float>("percent_low");
					sd.value2 = skillS.get<float>("percent_high");
					sd.value3 = skillS.get<UInt32>("count");
					break;
				case 202:
				case 203:
				case 204:
				case 209:
					sd.value1 = skillS.get<float>("factor");
					sd.value3 = skillS.get<UInt32>("round");
					break;
				case 303:
					sd.value3 = skillS.get<UInt32>("regen");
					sd.value4 = skillS.get<UInt32>("count");
					sd.value1 = skillS.get<float>("factor");
					break;
				case 304:
				case 308:
					sd.value1 = skillS.get<float>("damage");
					sd.value3 = skillS.get<UInt32>("count");
					sd.value4 = skillS.get<UInt32>("stun");
					break;
				case 205:
				case 208:
					sd.value1 = skillS.get<float>("attack_percent") / 100;
					sd.value2 = skillS.get<float>("defense");
					break;
				}
			}
		}
	}
}

}

#ifndef _BATTLEFIGHTER_H_
#define _BATTLEFIGHTER_H_

#include "GObject/Fighter.h"
#include "GData/Formation.h"
#include "GData/AttrExtra.h"
#include "BattleObject.h"

namespace Script
{
class BattleFormula;
}

namespace GObject
{
	class ItemEquip;
}

namespace Battle
{

class BattleFighter:
	public BattleObject
{
	friend class BattleSimulator;
public:
	BattleFighter(Script::BattleFormula *, GObject::Fighter * = NULL, UInt8 side = 0, UInt8 pos = 0);

	void setFighter(GObject::Fighter * f);
	inline void setFormationEffect(const GData::Formation::GridEffect * fe) {_formEffect = fe;}

	inline UInt32 getId() { return _fighter->getId(); }
	inline UInt8 getClass() { return _fighter->getClass(); }
	inline UInt8 getLevel() { return _fighter->getLevel(); }
	inline UInt8 getColor() { return _fighter->getColor(); }
	inline float getPotential() { return _fighter->getPotential(); }

	inline UInt16 getExtraStrength() { return _attr1Extra.strength; }
	inline UInt16 getExtraPhysique() { return _attr1Extra.physique; }
	inline UInt16 getExtraAgility() { return _attr1Extra.agility; }
	inline UInt16 getExtraIntelligence() { return _attr1Extra.intelligence; }
	inline float getExtraStrengthP() { return _attr1Extra.strengthP; }
	inline float getExtraPhysiqueP() { return _attr1Extra.physiqueP; }
	inline float getExtraAgilityP() { return _attr1Extra.agilityP; }
	inline float getExtraIntelligenceP() { return _attr1Extra.intelligenceP; }
	inline UInt16 getExtraAttack() { return _attr2Extra.attack; }
	inline float getExtraAttackP() { return _attr2Extra.attackP; }
	inline UInt16 getExtraDefend() { return _attr2Extra.defend; }
	inline float getExtraDefendP() { return _attr2Extra.defendP; }
	inline UInt16 getExtraHP() { return _attr2Extra.hp; }
	inline float getExtraHPP() { return _attr2Extra.hpP; }
	inline float getExtraAction() { return _attr2Extra.action; }
	inline float getExtraHitrate() { return _attr2Extra.hitrate; }
	inline float getExtraEvade() { return _attr2Extra.evade; }
	inline float getExtraCritical() { return _attr2Extra.critical; }
	inline float getExtraPierce() { return _attr2Extra.pierce; }
	inline float getExtraCounter() { return _attr2Extra.counter; }

	inline GObject::Fighter * getFighter() {return _fighter;}

	inline bool isNpc() { return _fighter->isNpc(); }
	inline Int16 getBaseStrength() { return _fighter->getBaseStrength(); }
	inline Int16 getBasePhysique() { return _fighter->getBasePhysique(); }
	inline Int16 getBaseAgility() { return _fighter->getBaseAgility(); }
	inline Int16 getBaseIntelligence() { return _fighter->getBaseIntelligence(); }
	inline Int16 getBaseAttack() {return _fighter->getBaseAttack();}
	inline Int16 getBaseDefend() {return _fighter->getBaseDefend();}
	inline float getBaseHitrate() {return _fighter->getBaseHitrate();}
	inline float getBaseEvade() {return _fighter->getBaseEvade();}
	inline float getBaseCritical() {return _fighter->getBaseCritical();}
	inline float getBasePierce() {return _fighter->getBasePierce();}
	inline float getBaseCounter() {return _fighter->getBaseCounter();}
	inline Int32 getBaseHP() {return _fighter->getBaseHP();}
	inline UInt32 getBaseAction() {return _fighter->getBaseAction();}

	inline float getStrength() { return _strength; }
	inline float getAgility() { return _agility; }
	inline float getPhysique() { return _physique; }
	inline float getIntelligence() { return _intelligence; }
	inline float getAttack() {return _attack + _attackAdd;}
	inline float getDefend() {return _defend + _defendAdd;}
	inline float getHitrate() {return _hitrate + _hitrateAdd;}
	inline float getEvade() {return _evade + _evadeAdd;}
	inline float getCritical() {return _critical + _criticalAdd;}
	inline float getPierce() {return _pierce + _pierceAdd;}
	inline float getCounter() {return _counter + _counterAdd;}
	inline UInt32 getMaxHP() {return _maxhp + _maxhpAdd;}
	inline UInt32 getAction() {return _maxAction;}
	inline const GData::Formation::GridEffect * getFormationEffect() const {return _formEffect;}

	inline float getAttackAdd() {return _attackAdd;}
	inline float getDefendAdd() {return _defendAdd;}
	inline float getHitrateAdd() {return _hitrateAdd;}
	inline float getEvadeAdd() {return _evadeAdd;}
	inline float getCriticalAdd() {return _criticalAdd;}
	inline float getPierceAdd() {return _pierceAdd;}
	inline float getCounterAdd() {return _counterAdd;}
	inline UInt32 getMaxHPAdd() {return _maxhpAdd;}
	inline UInt32 getActionAdd() {return _maxActionAdd;}
	inline void setAttackAdd(float v) {_attackAdd = v;}
	inline void setDefendAdd(float v) {_defendAdd = v;}
	inline void setHitrateAdd(float v) {_hitrateAdd = v;}
	inline void setEvadeAdd(float v) {_evadeAdd = v;}
	inline void setCriticalAdd(float v) {_criticalAdd = v;}
	inline void setPierceAdd(float v) {_pierceAdd = v;}
	inline void setCounterAdd(float v) {_counterAdd = v;}
	inline void setMaxHPAdd(UInt32 v) {_maxhpAdd = v;}
	inline void setActionAdd(UInt32 v) {_maxActionAdd = v;}

	inline UInt32 getLostHP() { UInt32 mhp = _maxhp + _maxhpAdd; if(mhp > _hp) return mhp - _hp; return 0; }

	UInt32 regenHP(UInt32 u);

	void updateAllAttr();
	void initStats(bool);
	void postInit();
	float calcAttack(bool& isCritical);
	bool calcHit(BattleFighter * defender);
	bool calcCounter(bool ranged = false);
	bool canBeCounter();
	bool calcPierce();

	inline void addAction(UInt32 p);

	inline GData::Attr1Extra * getTotalAttr1Extra() { return &_attr1Extra; }
	inline GData::Attr2Extra * getTotalAttr2Extra() { return &_attr2Extra; }

	UInt16 getPortrait();
	inline UInt32 getFlag() { return _flag; }
	inline void addFlag(UInt32 f) { _flag |= f; }
	inline void delFlag(UInt32 f) { _flag &= ~f; }
	inline bool hasFlag(UInt32 f) { return (_flag & f) > 0;}
	inline UInt32 getPoisonRound() { return (_flag & 0x1C00000) >> 22; }
	inline UInt32 getStunRound() { return (_flag & 0xE000000) >> 25; }
	inline void setPoisonRound(UInt32 r) { _flag = (_flag & 0xFE3FFFFF) + (r << 22); }
	inline void setStunRound(UInt32 r) { _flag = (_flag & 0xF1FFFFFF) + (r << 25); }
	inline UInt32 getThornLevel() { return (_flag & Thorn) >> 5; }
	inline UInt32 getPoisonLevel() { return (_flag & Poison) >> 8; }
	inline UInt32 getConfuseLevel() { return (_flag & Confuse) >> 11; }
	inline void setThornLevel(UInt32 l) { _flag = (_flag & ~Thorn) + (l << 5); }
	inline void setPoisonLevel(UInt32 l) { _flag = (_flag & ~Poison) + (l << 8); }
	inline void setConfuseLevel(UInt32 l) { _flag = (_flag & ~Confuse) + (l << 11); }
	void setAttrExtra(UInt8, UInt8, UInt8);

private:
	void updateBuffExtras();

private:
	GObject::Fighter * _fighter;
	float _strength, _agility, _physique, _intelligence;
	float _attack, _defend, _hitrate, _evade, _critical, _pierce, _counter;
	UInt32 _maxhp, _maxAction;
	float _attackAdd, _defendAdd, _hitrateAdd, _evadeAdd, _criticalAdd, _pierceAdd, _counterAdd;
	UInt32 _maxhpAdd, _maxActionAdd;
	const GData::Formation::GridEffect * _formEffect;
	Script::BattleFormula * _formula;
	/* 武将状态 0-1bit:增强符
	   2-3bit:群体增强符
	   b4:愈战愈勇
	   28-31bit: holy level */
	UInt32 _flag;
	GData::Attr1Extra _attr1ExtraEquip; // 装备附加一级属性和
	GData::Attr2Extra _attr2ExtraEquip; // 装备附加二级属性和
	GData::Attr1Extra _attr1Extra; // 全部附加一级属性和
	GData::Attr2Extra _attr2Extra; // 全部附加二级属性和
	GData::Attr1Extra _attr1bylevel;
	GData::Attr2Extra _attr2bylevel;

public:
	enum StatusFlag
	{
		Enh1 = 0x03,
		Enh2 = 0x0C,
		Enh3 = 0x10,
		Thorn = 0xE0,
		Poison = 0x700,
		Confuse = 0x3800,
		Mirrored = 0x4000,
		MaxHPRef = 0x8000,
		IsMirror = 0x10000,
		BlockBoss = 0x200000,
	};
};

}

#endif // _BATTLEFIGHTER_H_

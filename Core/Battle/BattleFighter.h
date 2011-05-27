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

	inline UInt16 getExtraStrength() { return _attrExtra.strength; }
	inline UInt16 getExtraPhysique() { return _attrExtra.physique; }
	inline UInt16 getExtraAgility() { return _attrExtra.agility; }
	inline UInt16 getExtraIntelligence() { return _attrExtra.intelligence; }
	inline UInt16 getExtraWill() { return _attrExtra.will; }
	inline UInt16 getExtraSoul() { return _attrExtra.soul; }
	inline UInt16 getExtraAura() { return _attrExtra.aura; }
	inline UInt16 getExtraTough() { return _attrExtra.tough; }
	inline float getExtraStrengthP() { return _attrExtra.strengthP; }
	inline float getExtraPhysiqueP() { return _attrExtra.physiqueP; }
	inline float getExtraAgilityP() { return _attrExtra.agilityP; }
	inline float getExtraIntelligenceP() { return _attrExtra.intelligenceP; }
	inline float getExtraWillP() { return _attrExtra.willP; }
	inline float getExtraSoulP() { return _attrExtra.soulP; }
	inline float getExtraAuraP() { return _attrExtra.auraP; }
	inline float getExtraToughP() { return _attrExtra.toughP; }
	inline UInt16 getExtraAttack() { return _attrExtra.attack; }
	inline float getExtraAttackP() { return _attrExtra.attackP; }
	inline UInt16 getExtraMagAttack() { return _attrExtra.mag_attack; }
	inline float getExtraMagAttackP() { return _attrExtra.mag_attackP; }
	inline UInt16 getExtraDefend() { return _attrExtra.defend; }
	inline float getExtraDefendP() { return _attrExtra.defendP; }
	inline UInt16 getExtraMagDefend() { return _attrExtra.mag_defend; }
	inline float getExtraMagDefendP() { return _attrExtra.mag_defendP; }
	inline UInt16 getExtraHP() { return _attrExtra.hp; }
	inline float getExtraHPP() { return _attrExtra.hpP; }
	inline float getExtraAction() { return _attrExtra.action; }
	inline float getExtraHitrate() { return _attrExtra.hitrate; }
	inline float getExtraEvade() { return _attrExtra.evade; }
	inline float getExtraCritical() { return _attrExtra.critical; }
	inline float getExtraCriticalDmg() { return _attrExtra.critical_dmg; }
	inline float getExtraPierce() { return _attrExtra.pierce; }
	inline float getExtraCounter() { return _attrExtra.counter; }
	inline float getExtraMagRes() { return _attrExtra.mag_res; }

	inline GObject::Fighter * getFighter() {return _fighter;}

	inline bool isNpc() { return _fighter->isNpc(); }
	inline bool isBoy() { return _fighter->isBoy(); }
	inline bool isMale() { return _fighter->isMale(); }
	inline UInt8 getSex() { return _fighter->getSex(); }
	inline Int16 getBaseStrength() { return _fighter->getBaseStrength(); }
	inline Int16 getBasePhysique() { return _fighter->getBasePhysique(); }
	inline Int16 getBaseAgility() { return _fighter->getBaseAgility(); }
	inline Int16 getBaseIntelligence() { return _fighter->getBaseIntelligence(); }
	inline Int16 getBaseWill() { return _fighter->getBaseWill(); }
	inline Int16 getBaseSoul() { return _fighter->getBaseSoul(); }
	inline Int16 getBaseAura() { return _fighter->getBaseAura(); }
	inline Int16 getBaseTough() { return _fighter->getBaseTough(); }
	inline Int16 getBaseAttack() {return _fighter->getBaseAttack();}
	inline Int16 getBaseMagAttack() {return _fighter->getBaseMagAttack();}
	inline Int16 getBaseDefend() {return _fighter->getBaseDefend();}
	inline Int16 getBaseMagDefend() {return _fighter->getBaseMagDefend();}
	inline float getBaseHitrate() {return _fighter->getBaseHitrate();}
	inline float getBaseEvade() {return _fighter->getBaseEvade();}
	inline float getBaseCritical() {return _fighter->getBaseCritical();}
	inline float getBaseCriticalDmg() {return _fighter->getBaseCriticalDmg();}
	inline float getBasePierce() {return _fighter->getBasePierce();}
	inline float getBaseCounter() {return _fighter->getBaseCounter();}
	inline float getBaseMagRes() {return _fighter->getBaseMagRes();}
	inline Int32 getBaseHP() {return _fighter->getBaseHP();}
	inline UInt32 getBaseAction() {return _fighter->getBaseAction();}

	inline float getStrength() { return _strength; }
	inline float getAgility() { return _agility; }
	inline float getPhysique() { return _physique; }
	inline float getIntelligence() { return _intelligence; }
	inline float getWill() { return _will; }
	inline float getSoul() { return _soul; }
	inline float getAura() { return _aura; }
	inline float getTough() { return _tough; }
	inline float getAttack() {return _attack + _attackAdd;}
	inline float getMagAttack() {return _mag_attack + _magAttackAdd;}
	inline float getDefend() {return _defend + _defendAdd;}
	inline float getMagDefend() {return _mag_defend + _magDefendAdd;}
	inline float getHitrate() {return _hitrate + _hitrateAdd;}
	inline float getEvade() {return _evade + _evadeAdd;}
	inline float getCritical() {return _critical + _criticalAdd;}
	inline float getCriticalDmg() {return _critical_dmg + _criticalDmgAdd;}
	inline float getPierce() {return _pierce + _pierceAdd;}
	inline float getCounter() {return _counter + _counterAdd;}
	inline float getMagRes() {return _mag_res+ _magResAdd;}
	inline UInt32 getMaxHP() {return _maxhp + _maxhpAdd;}
	inline UInt32 getAction() {return _maxAction;}
	inline const GData::Formation::GridEffect * getFormationEffect() const {return _formEffect;}

	inline float getAttackAdd() {return _attackAdd;}
    inline float getMagAttackAdd() {return _magAttackAdd;}
	inline float getDefendAdd() {return _defendAdd;}
	inline float getMagDefendAdd() {return _magDefendAdd;}
	inline float getHitrateAdd() {return _hitrateAdd;}
	inline float getEvadeAdd() {return _evadeAdd;}
	inline float getCriticalAdd() {return _criticalAdd;}
	inline float getCriticalDmgAdd() {return _criticalDmgAdd;}
	inline float getPierceAdd() {return _pierceAdd;}
	inline float getCounterAdd() {return _counterAdd;}
	inline float getMagResAdd() {return _magResAdd;}
	inline UInt32 getMaxHPAdd() {return _maxhpAdd;}
	inline UInt32 getActionAdd() {return _maxActionAdd;}
	inline void setAttackAdd(float v) {_attackAdd = v;}
	inline void setMagAttackAdd(float v) {_magAttackAdd = v;}
	inline void setDefendAdd(float v) {_defendAdd = v;}
	inline void setMagDefendAdd(float v) {_magDefendAdd = v;}
	inline void setHitrateAdd(float v) {_hitrateAdd = v;}
	inline void setEvadeAdd(float v) {_evadeAdd = v;}
	inline void setCriticalAdd(float v) {_criticalAdd = v;}
	inline void setCriticalDmgAdd(float v) {_criticalDmgAdd = v;}
	inline void setPierceAdd(float v) {_pierceAdd = v;}
	inline void setCounterAdd(float v) {_counterAdd = v;}
	inline void setMagResAdd(float v) {_magResAdd = v;}
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

	inline GData::AttrExtra * getTotalAttrExtra() { return &_attrExtra; }

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
	float _strength, _agility, _physique, _intelligence, _will, _soul, _aura, _tough;
	float _attack, _mag_attack, _defend, _mag_defend, _hitrate, _evade;
    float _critical, _critical_dmg, _pierce, _counter, _mag_res;
	UInt32 _maxhp, _maxAction;
	float _attackAdd, _magAttackAdd, _defendAdd, _magDefendAdd, _hitrateAdd, _evadeAdd;
    float _criticalAdd, _criticalDmgAdd, _pierceAdd, _counterAdd, _magResAdd;
	UInt32 _maxhpAdd, _maxActionAdd;
	const GData::Formation::GridEffect * _formEffect;
	Script::BattleFormula * _formula;
	/* 武将状态 0-1bit:增强符
	   2-3bit:群体增强符
	   b4:愈战愈勇
	   28-31bit: holy level */
	UInt32 _flag;
	GData::AttrExtra _attrExtraEquip; // 装备附加一级属性和
	GData::AttrExtra _attrExtra; // 全部附加一级属性和
	GData::AttrExtra _attrbylevel;

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

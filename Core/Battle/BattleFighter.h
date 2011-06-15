#ifndef _BATTLEFIGHTER_H_
#define _BATTLEFIGHTER_H_

#include "GObject/Fighter.h"
#include "GData/Formation.h"
#include "GData/AttrExtra.h"
#include "BattleObject.h"
#include "GData/SkillTable.h"

namespace Script
{
class BattleFormula;
}

namespace GObject
{
	class ItemEquip;
}

namespace GData
{
    class SkillBase;
    class SkillItem;
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
	inline UInt16 getExtraMagAttack() { return _attrExtra.magatk; }
	inline float getExtraMagAttackP() { return _attrExtra.magatkP; }
	inline UInt16 getExtraDefend() { return _attrExtra.defend; }
	inline float getExtraDefendP() { return _attrExtra.defendP; }
	inline UInt16 getExtraMagDefend() { return _attrExtra.magdef; }
	inline float getExtraMagDefendP() { return _attrExtra.magdefP; }
	inline UInt16 getExtraHP() { return _attrExtra.hp; }
	inline float getExtraHPP() { return _attrExtra.hpP; }
	inline float getExtraAction() { return _attrExtra.action; }
	inline float getExtraHitrate() { return _attrExtra.hitrate; }
	inline float getExtraEvade() { return _attrExtra.evade; }
	inline float getExtraCritical() { return _attrExtra.critical; }
	inline float getExtraCriticalDmg() { return _attrExtra.critical_dmg; }
	inline float getExtraPierce() { return _attrExtra.pierce; }
	inline float getExtraCounter() { return _attrExtra.counter; }
	inline float getExtraMagRes() { return _attrExtra.magres; }

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
	inline float getAuraMax() { return _auraMax; }
	inline float getTough() { return _tough; }
	inline float getAttack() {return _attack + _attackAdd;}
	inline float getMagAttack() {return _magatk + _magAttackAdd;}
	inline float getDefend() {return _defend + _defendAdd;}
	inline float getMagDefend() {return _magdef + _magDefendAdd;}
	inline float getHitrate() {return _hitrate + _hitrateAdd;}
	inline float getEvade() {return _evade + _evadeAdd;}
	inline float getCritical() {return _critical + _criticalAdd;}
	inline float getCriticalDmg() {return _critical_dmg + _criticalDmgAdd;}
	inline float getPierce() {return _pierce + _pierceAdd;}
	inline float getCounter() {return _counter + _counterAdd;}
	inline float getMagRes() {return _magres+ _magResAdd;}
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
	inline void setAttackAdd(float v, UInt16 last = 0) {_attackAdd = v; _atkAdd_last = last;}
	inline void setMagAttackAdd(float v, UInt16 last = 0) {_magAttackAdd = v; _magAtkAdd_last = last;}
	inline void setDefendAdd(float v, UInt16 last = 0) {_defendAdd = v; _defAdd_last = last;}
	inline void setMagDefendAdd(float v, UInt16 last = 0) {_magDefendAdd = v; _magDefAdd_last = last;}
	inline void setHitrateAdd(float v, UInt16 last = 0) {_hitrateAdd = v; _hitrateAdd_last = last;}
	inline void setEvadeAdd(float v, UInt16 last = 0) {_evadeAdd = v; _evadeAdd_last = last;}
	inline void setCriticalAdd(float v, UInt16 last = 0) {_criticalAdd = v; _criticalAdd_last = last;}
	inline void setCriticalDmgAdd(float v) {_criticalDmgAdd = v;}
	inline void setPierceAdd(float v, UInt16 last = 0) {_pierceAdd = v; _pierceAdd_last = last;}
	inline void setCounterAdd(float v, UInt16 last = 0) {_counterAdd = v; _counterAdd_last = last;}
	inline void setMagResAdd(float v, UInt16 last = 0) {_magResAdd = v; _magResAdd_last = last;}
	inline void setMaxHPAdd(UInt32 v) {_maxhpAdd = v;}
	inline void setActionAdd(UInt32 v, UInt16 last = 0) {_maxActionAdd = v; _maxActionAdd_last = last;}
    inline void AddAura(UInt32 v) {_aura += v;}
    inline void setToughAdd(float v, UInt16 last) {_toughAdd = v; _toughAdd_last = last;}

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
    float calcTherapy(const GData::SkillBase* skill);
    float calcMagAttack(bool& isCritical);

	inline void addAction(UInt32 p);

	inline GData::AttrExtra * getTotalAttrExtra() { return &_attrExtra; }

	UInt16 getPortrait();
	inline UInt32 getFlag() { return _flag; }
	inline void addFlag(UInt32 f) { _flag |= f; }
	inline void delFlag(UInt32 f) { _flag &= ~f; }
	inline bool hasFlag(UInt32 f) { return (_flag & f) > 0;}
	inline UInt32 getPoisonRound() { return (_flag & PoisonRound) >> 22; }
	inline UInt32 getStunRound() { return (_flag & StunRound) >> 25; }
	inline void setPoisonRound(UInt32 r) { _flag = (_flag & ~PoisonRound) + (r << 22); }
	inline void setStunRound(UInt32 r) { _flag = (_flag & ~StunRound) + (r << 25); }
	inline UInt32 getStunLevel() { return (_flag & Stun) >> 5; }
	inline UInt32 getPoisonLevel() { return (_flag & Poison) >> 8; }
	inline UInt32 getConfuseLevel() { return (_flag & Confuse) >> 11; }
	inline void setStunLevel(UInt32 l) { _flag = (_flag & ~Stun) + (l << 5); }
	inline void setPoisonLevel(UInt32 l) { _flag = (_flag & ~Poison) + (l << 8); }
	inline void setConfuseLevel(UInt32 l) { _flag = (_flag & ~Confuse) + (l << 11); }
    inline UInt32 getConfuseRound() { return (_flag & ConfuseRound) >> 28; }
    inline void setConfuseRound(UInt32 l) { _flag = (_flag & ~ConfuseRound) + (l << 28); }
	void setAttrExtra(UInt8, UInt8, UInt8);
    inline void setImmuneLevel(UInt8 f) { _immuneLevel = f; }
    inline void setImmuneRound(UInt8 f) { _immuneRound = f; }
    inline UInt8 getImmuneLevel() { return _immuneLevel; }
    inline UInt8 getImmuneRound() { return _immuneRound; }
    inline void setForgetLevel(UInt8 f) { _forgetLevel = f; }
    inline void setForgetRound(UInt8 f) { _forgetRound = f; }
    inline UInt8 getForgetLevel() { return _forgetLevel; }
    inline UInt8 getForgetRound() { return _forgetRound; }


    const GData::SkillBase* getActiveSkill(bool need_therapy = false);
    const GData::SkillBase* getPassiveSkillPrvAtk100(size_t& idx);
    const GData::SkillBase* getPassiveSkillAftAtk100(size_t& idx);
    const GData::SkillBase* getPassiveSkillBeAtk100(size_t& idx);
    const GData::SkillBase* getPassiveSkillAftEvd100(size_t& idx);
    const GData::SkillBase* getPassiveSkillAftRes100(size_t& idx);
    const GData::SkillBase* getPassiveSkillEnter100(size_t& idx);
    const GData::SkillBase* getPassiveSkillDead100(size_t& idx);

    const GData::SkillBase* getPassiveSkillPreAtk();
    const GData::SkillBase* getPassiveSkillAftAtk();
    const GData::SkillBase* getPassiveSkillBeAtk();
    const GData::SkillBase* getPassiveSkillAftEvd();
    const GData::SkillBase* getPassiveSkillAftRes();
    const GData::SkillBase* getPassiveSkillEnter();
    const GData::SkillBase* getPassiveSkillDead();
    void releaseSkillCD(int cd);

    inline bool isReAlive() { return _reAlive; }
    inline void setReAlive() { _reAlive = true; _hp = _maxhp; }

private:
	void updateBuffExtras();

private:
	GObject::Fighter * _fighter;
	float _strength, _agility, _physique, _intelligence, _will, _soul, _aura, _auraMax, _tough;
	float _attack, _magatk, _defend, _magdef, _hitrate, _evade;
    float _critical, _critical_dmg, _pierce, _counter, _magres;
	UInt32 _maxhp, _maxAction;
	float _attackAdd, _magAttackAdd, _defendAdd, _magDefendAdd, _hitrateAdd, _evadeAdd;
    float _criticalAdd, _criticalDmgAdd, _pierceAdd, _counterAdd, _magResAdd, _toughAdd;
	UInt32 _maxhpAdd, _maxActionAdd;
    UInt8 _atkAdd_last, _magAtkAdd_last, _defAdd_last, _magDefAdd_last, _hitrateAdd_last, _evadeAdd_last;
    UInt8 _criticalAdd_last, criticalDmgAdd_last, _pierceAdd_last, _counterAdd_last, _magResAdd_last, _toughAdd_last;
    UInt8 _maxhpAdd_last, _maxActionAdd_last;
	const GData::Formation::GridEffect * _formEffect;
	Script::BattleFormula * _formula;
    UInt8 _immuneLevel, _immuneRound;
    UInt8 _forgetLevel, _forgetRound;
	/* 武将状态 0-1bit:增强符
	   2-3bit:群体增强符
	   b4:愈战愈勇
	   28-31bit: holy level */
	UInt32 _flag;
	GData::AttrExtra _attrExtraEquip; // 装备附加一级属性和
	GData::AttrExtra _attrExtra; // 全部附加一级属性和
	GData::AttrExtra _attrbylevel;


    GData::SkillItem _peerlessSkill;
    std::vector<GData::SkillItem> _activeSkill;
    std::vector<GData::SkillItem> _passiveSkillPrvAtk100;
    std::vector<GData::SkillItem> _passiveSkillAftAtk100;
    std::vector<GData::SkillItem> _passiveSkillBeAtk100;
    std::vector<GData::SkillItem> _passiveSkillAftEvd100;
    std::vector<GData::SkillItem> _passiveSkillAftRes100;
    std::vector<GData::SkillItem> _passiveSkillEnter100;
    std::vector<GData::SkillItem> _passiveSkillDead100;

    std::vector<GData::SkillItem> _passiveSkillPreAtk;
    std::vector<GData::SkillItem> _passiveSkillAftAtk;
    std::vector<GData::SkillItem> _passiveSkillBeAtk;
    std::vector<GData::SkillItem> _passiveSkillAftEvd;
    std::vector<GData::SkillItem> _passiveSkillAftRes;
    std::vector<GData::SkillItem> _passiveSkillEnter;
    std::vector<GData::SkillItem> _passiveSkillDead;

    bool _reAlive;

public:
	enum StatusFlag
	{
		Enh1 = 0x03,
		Enh2 = 0x0C,
		Enh3 = 0x10,
		Stun = 0xE0,
		Poison = 0x700,
		Confuse = 0x3800,
		Mirrored = 0x4000,
		MaxHPRef = 0x8000,
		IsMirror = 0x10000,
		BlockBoss = 0x200000,
        PoisonRound = 0x1C00000,
        StunRound = 0xE000000,
        ConfuseRound = 0x70000000,
	};
};

}

#endif // _BATTLEFIGHTER_H_

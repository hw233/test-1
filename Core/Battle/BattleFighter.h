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
    inline UInt32 getBuffData( UInt8 idx, UInt32 now ) { return _fighter->getBuffData(idx, now); }

	inline UInt16 getExtraStrength() { return _attrExtra.strength; }
	inline UInt16 getExtraPhysique() { return _attrExtra.physique; }
	inline UInt16 getExtraAgility() { return _attrExtra.agility; }
	inline UInt16 getExtraIntelligence() { return _attrExtra.intelligence; }
	inline UInt16 getExtraWill() { return _attrExtra.will; }
	inline UInt16 getExtraSoul() { return _attrExtra.soul; }
	inline UInt16 getExtraAura() { return _attrExtra.aura; }
	inline UInt16 getExtraAuraMax() { return _attrExtra.auraMax; }
	inline float getExtraTough() { return _attrExtra.tough; }
	inline float getExtraStrengthP() { return _attrExtra.strengthP; }
	inline float getExtraPhysiqueP() { return _attrExtra.physiqueP; }
	inline float getExtraAgilityP() { return _attrExtra.agilityP; }
	inline float getExtraIntelligenceP() { return _attrExtra.intelligenceP; }
	inline float getExtraWillP() { return _attrExtra.willP; }
	inline float getExtraSoulP() { return _attrExtra.soulP; }
	inline float getExtraAuraP() { return _attrExtra.auraP; }
	inline float getExtraAuraMaxP() { return _attrExtra.auraMaxP; }
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
	inline float getExtraActionP() { return _attrExtra.actionP; }
	inline float getExtraHitrate() { return _attrExtra.hitrate; }
	inline float getExtraEvade() { return _attrExtra.evade; }
	inline float getExtraCritical() { return _attrExtra.critical; }
	inline float getExtraCriticalDmg() { return _attrExtra.criticaldmg; }
	inline float getExtraPierce() { return _attrExtra.pierce; }
	inline float getExtraCounter() { return _attrExtra.counter; }
	inline float getExtraMagRes() { return _attrExtra.magres; }

    inline float getExtraHitrateLevel() { return _attrExtra.hitrlvl; }
    inline float getExtraEvadeLevel() { return _attrExtra.evdlvl; }
    inline float getExtraCriticalLevel() { return _attrExtra.crilvl; }
    inline float getExtraPierceLevel() { return _attrExtra.pirlvl; }
    inline float getExtraCounterLevel() { return _attrExtra.counterlvl; }
    inline float getExtraToughLevel() { return _attrExtra.toughlvl; }
    inline float getExtraMagResLevel() { return _attrExtra.mreslvl; }

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
	inline Int16 getBaseAuraMax() { return _fighter->getBaseAuraMax(); }
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
	inline Int32 getBaseAction() {return _fighter->getBaseAction();}

	inline float getStrength() { return _strength; }
	inline float getAgility() { return _agility; }
	inline float getPhysique() { return _physique; }
	inline float getIntelligence() { return _intelligence; }
	inline float getWill() { return _will; }
	inline float getSoul() { return _soul; }
	inline float getAura() { return (_aura > 0 ? _aura : 0); }
	inline float getAuraMax() { return (_auraMax > 0 ? _auraMax : 0); }
	inline float getAttack() {float ret = _attack + _attackAdd + _attackAdd2; return (ret > 0 ? ret : 0);}
	inline float getMagAttack() {float ret = _magatk + _magAtkAdd + _magAtkAdd2; return (ret > 0 ? ret : 0);}
	inline float getDefend() {float ret = _defend + _defAdd + _defAdd2; return (ret > 0 ? ret : 0);}
	inline float getMagDefend() {float ret = _magdef + _magDefAdd + _magDefAdd2; return (ret > 0 ? ret : 0);}
	float getHitrate(BattleFighter* defgt);
	float getEvade(BattleFighter* defgt);
	float getCritical(BattleFighter* defgt);
	inline float getCriticalDmg() {float ret = _criticaldmg + _criticalDmgAdd + _criticalDmgAdd2; return (ret > 0 ? ret : 0);}
    float calcCriticalDmg(BattleFighter* defender);
	float getPierce(BattleFighter* defgt);
	float getCounter(BattleFighter* defgt);
	float getMagRes(BattleFighter* defgt);
	float getTough(BattleFighter* defgt);
	inline UInt32 getMaxHP() {Int64 ret = _maxhp + _maxhpAdd + _maxhpAdd2; return (ret > 0 ? ret : 0);}
	inline Int32 getAction() {Int32 ret = _maxAction + _maxActionAdd + _maxActionAdd2; return (ret > 0 ? ret : 0);}
	inline const GData::Formation::GridEffect * getFormationEffect() const {return _formEffect;}

	inline float getAttackAdd() {return _attackAdd;}
    inline float getMagAttackAdd() {return _magAtkAdd;}
	inline float getDefendAdd() {return _defAdd;}
	inline float getMagDefendAdd() {return _magDefAdd;}
	inline float getHitrateAdd() {return _hitrateAdd;}
	inline float getEvadeAdd() {return _evadeAdd;}
	inline float getCriticalAdd() {return _criticalAdd;}
	inline float getCriticalDmgAdd() {return _criticalDmgAdd;}
	inline float getPierceAdd() {return _pierceAdd;}
	inline float getCounterAdd() {return _counterAdd;}
	inline float getMagResAdd() {return _magResAdd;}
	inline Int32 getMaxHPAdd() {return _maxhpAdd;}
	inline Int32 getActionAdd() {return _maxActionAdd;}
    inline float getToughAdd() { return _toughAdd;}
    inline float getAtkReduce() { float ret = _atkreduce + _atkreduce2; return (ret > 0 ? ret : 0); }
    inline float getMagAtkReduce() { float ret = _magatkreduce + _magatkreduce2; return (ret > 0 ? ret : 0); }

	inline void setAttackAdd(float v, UInt16 last = 0) {_attackAdd = v; _atkAdd_last = last;}
	inline void setMagAttackAdd(float v, UInt16 last = 0) {_magAtkAdd = v; _magAtkAdd_last = last;}
	inline void setDefendAdd(float v, UInt16 last = 0) {_defAdd = v; _defAdd_last = last;}
	inline void setMagDefendAdd(float v, UInt16 last = 0) {_magDefAdd = v; _magDefAdd_last = last;}
	inline void setHitrateAdd(float v, UInt16 last = 0) {_hitrateAdd = v; _hitrateAdd_last = last;}
	inline void setEvadeAdd(float v, UInt16 last = 0) {_evadeAdd = v; _evadeAdd_last = last;}
	inline void setCriticalAdd(float v, UInt16 last = 0) {_criticalAdd = v; _criticalAdd_last = last;}
	inline void setCriticalDmgAdd(float v, UInt16 last = 0) {_criticalDmgAdd = v; _criticalDmgAdd_last = last;}
	inline void setPierceAdd(float v, UInt16 last = 0) {_pierceAdd = v; _pierceAdd_last = last;}
	inline void setCounterAdd(float v, UInt16 last = 0) {_counterAdd = v; _counterAdd_last = last;}
	inline void setMagResAdd(float v, UInt16 last = 0) {_magResAdd = v; _magResAdd_last = last;}
	inline void setMaxHPAdd(UInt32 v) {_maxhpAdd = v;}
	inline void setActionAdd(UInt32 v, UInt16 last = 0) {_maxActionAdd = v; _maxActionAdd_last = last;}
    inline void AddAura(Int32 v) {_aura += v; if(_aura > _auraMax) _aura = _auraMax; else if(_aura < 0) _aura = 0;}
    inline void setAura(Int32 v) {_aura = v > _auraMax ? _auraMax : v;}
    inline void setToughAdd(float v, UInt16 last) {_toughAdd = v; _toughAdd_last = last;}
    inline void setAtkReduce(float v, UInt16 last) { _atkreduce = v; _atkreduce_last = last; }
    inline void setMagAtkReduce(float v, UInt16 last) { _magatkreduce = v; _magatkreduce_last = last; }

	inline UInt8& getAttackAddLast() {return _atkAdd_last;}
    inline UInt8& getMagAttackAddLast() {return _magAtkAdd_last;}
	inline UInt8& getDefendAddLast() {return _defAdd_last;}
	inline UInt8& getMagDefendAddLast() {return _magDefAdd_last;}
	inline UInt8& getHitrateAddLast() {return _hitrateAdd_last;}
	inline UInt8& getEvadeAddLast() {return _evadeAdd_last;}
	inline UInt8& getCriticalAddLast() {return _criticalAdd_last;}
	inline UInt8& getCriticalDmgAddLast() {return _criticalDmgAdd_last;}
	inline UInt8& getPierceAddLast() {return _pierceAdd_last;}
	inline UInt8& getCounterAddLast() {return _counterAdd_last;}
	inline UInt8& getMagResAddLast() {return _magResAdd_last;}
	inline UInt8& getMaxHPAddLast() {return _maxhpAdd_last;}
	inline UInt8& getActionAddLast() {return _maxActionAdd_last;}
    inline UInt8& getToughAddLast() { return _toughAdd_last;}
    inline UInt8& getAtkReduceLast() { return _atkreduce_last;}
    inline UInt8& getMagAtkReduceLast() { return _magatkreduce_last;}

	inline UInt32 getLostHP() { Int64 tmp = _maxhp + _maxhpAdd + _maxhpAdd2; UInt32 mhp = (tmp > 0 ? tmp : 0); if(mhp > _hp) return mhp - _hp; return 0; }

	UInt32 regenHP(UInt32 u, bool weak = false);

	void updateAllAttr();
	void initStats(bool);
	void postInit();
	float calcAttack(bool& isCritical, BattleFighter* defender , float* pCf);
	bool calcHit(BattleFighter * defender);
	bool calcCounter(BattleFighter* attacker, bool ranged = false);
	bool canBeCounter();
	bool calcPierce(BattleFighter* defender);
    float calcTherapy(const GData::SkillBase* skill);
    float calcMagAttack(bool& isCritical, BattleFighter* defender, float* pCf);
    float calcPoison(const GData::SkillBase* skill, BattleFighter* defender, bool cs);
    void calcSkillAttack(bool& isCritical, BattleFighter* defender, float& atk, float& magatk, float* pCf);

	inline void addAction(UInt32 p);

	inline GData::AttrExtra * getTotalAttrExtra() { return &_attrExtra; }

	UInt16 getPortrait();
	inline UInt32 getFlag() { return _flag; }
	inline void addFlag(UInt32 f) { _flag |= f; }
	inline void delFlag(UInt32 f) { _flag &= ~f; }
	inline bool hasFlag(UInt32 f) { return (_flag & f) > 0;}
	void setAttrExtra(UInt8, UInt8, UInt8);
    void setImmuneLevel(UInt8 state, UInt8 f);
    void setImmune(UInt8 f) { _immune |= f; }
    void setImmuneRound(UInt8 state, UInt8 f);
    UInt8 getImmuneLevel(UInt8 state);
    inline UInt8 getImmune() { return _immune; }
    UInt8 getImmuneRound(UInt8 state);
    inline void setForgetLevel(UInt8 f) { _forgetLevel = f; }
    inline void setForgetRound(UInt8 f) { _forgetRound = f; }
    inline UInt8 getForgetLevel() { return _forgetLevel; }
    inline UInt8 getForgetRound() { return _forgetRound; }

	inline UInt8 getPoisonRound() { return _poisonRound; }
	inline void setPoisonRound(UInt8 r) { _poisonRound = r; }
	inline void setPoisonLevel(UInt8 l) { _poisonLevel = l; }
	inline UInt8 getPoisonLevel() { return _poisonLevel; }

	inline UInt8 getStunRound() { return _stunRound; }
	inline void setStunRound(UInt8 r) { _stunRound = r; }
	inline UInt8 getStunLevel() { return _stunLevel; }
	inline void setStunLevel(UInt8 l) { _stunLevel = l; }

	inline UInt32 getConfuseLevel() { return _confuseLevel; }
	inline void setConfuseLevel(UInt32 l) { _confuseLevel = l; }
    inline UInt32 getConfuseRound() { return _confuseRound; }
    inline void setConfuseRound(UInt32 r) { _confuseRound = r; }

	inline UInt32 getWeakLevel() { return _weakLevel; }
	inline void setWeakLevel(UInt32 l) { _weakLevel = l; }
    inline UInt32 getWeakRound() { return _weakRound; }
    inline void setWeakRound(UInt32 r) { _weakRound = r; }

    const GData::SkillBase* getActiveSkill(bool need_therapy = false);
    const GData::SkillBase* getPassiveSkillPrvAtk100(size_t& idx);
    const GData::SkillBase* getPassiveSkillAftAtk100(size_t& idx);
    const GData::SkillBase* getPassiveSkillBeAtk100(size_t& idx);
    const GData::SkillBase* getPassiveSkillAftEvd100(size_t& idx);
    const GData::SkillBase* getPassiveSkillAftRes100(size_t& idx);
    const GData::SkillBase* getPassiveSkillEnter100(size_t& idx);
    const GData::SkillBase* getPassiveSkillDead100(size_t& idx);
    const GData::SkillBase* getPassiveSkillAftNAtk100(size_t& idx);

    const GData::SkillBase* getPassiveSkillPreAtk();
    const GData::SkillBase* getPassiveSkillAftAtk();
    const GData::SkillBase* getPassiveSkillBeAtk();
    const GData::SkillBase* getPassiveSkillAftEvd();
    const GData::SkillBase* getPassiveSkillAftRes();
    const GData::SkillBase* getPassiveSkillEnter();
    const GData::SkillBase* getPassiveSkillDead();
    const GData::SkillBase* getPassiveSkillAftNAtk();
    void releaseSkillCD(int cd);
    void releaseSkillCD(std::vector<GData::SkillItem>& skill, int cd);

    const GData::SkillBase* getPassiveSkill(std::vector<GData::SkillItem>& passiveSkill);
    const GData::SkillBase* getPassiveSkill100(std::vector<GData::SkillItem>& passiveSkill100, size_t& idx);

    inline bool isRevival() { return _revival; }
    inline void setRevival()
    {
        _revival = true;
        _hp = _maxhp;
        _attackAdd = 0;
        _magAtkAdd = 0;
        _defAdd = 0;
        _magDefAdd = 0;
        _hitrateAdd = 0;
        _evadeAdd = 0;
        _criticalAdd = 0;
        _criticalDmgAdd = 0;
        _pierceAdd = 0;
        _counterAdd = 0;
        _magResAdd = 0;
        _toughAdd = 0;
        _maxhpAdd = 0;
        _maxActionAdd = 0;
        _atkAdd_last = 0;
        _magAtkAdd_last = 0;
        _defAdd_last = 0;
        _magDefAdd_last = 0;
        _hitrateAdd_last = 0;
        _evadeAdd_last = 0;
        _criticalAdd_last = 0;
        _criticalDmgAdd_last = 0;
        _pierceAdd_last = 0;
        _counterAdd_last = 0;
        _magResAdd_last = 0;
        _toughAdd_last = 0;
        _maxhpAdd_last = 0;
        _maxActionAdd_last = 0;
    }
    inline void setEvad100( bool evd100 ) { _evade100 = evd100; }
    inline bool getEvad100() { return _evade100; }
    inline void setEvad100BaseCD( UInt8 evd100_cd ) { _evade100_base_cd = evd100_cd; }
    inline UInt8 getEvad100BaseCD() { return _evade100_base_cd; }
    inline void setEvad100CD( UInt8 evd100_cd ) { _evade100_cd = evd100_cd; }
    inline UInt8& getEvad100CD() { return _evade100_cd; }
    inline void setEvad100BaseLast( UInt8 evd100_last ) { _evade100_base_last = evd100_last; }
    inline UInt8 getEvad100BaseLast() { return _evade100_base_last; }
    inline void setEvad100Last( UInt8 evd100_last ) { _evade100_last = evd100_last; }
    inline UInt8& getEvad100Last() { return _evade100_last; }
    inline void setDefend100( bool def100 ) { _defend100 = def100; }
    inline bool getDefend100() { return _defend100; }
    inline void setDefend100BaseCD( UInt8 def100_cd ) { _defend100_base_cd = def100_cd; }
    inline UInt8 getDefend100BaseCD() { return _defend100_base_cd; }
    inline void setDefend100CD( UInt8 def100_cd ) { _defend100_cd = def100_cd; }
    inline UInt8& getDefend100CD() { return _defend100_cd; }
    inline void setDefend100BaseLast( UInt8 def100_last ) { _defend100_base_last = def100_last; }
    inline UInt8 getDefend100BaseLast() { return _defend100_base_last; }
    inline void setDefend100Last( UInt8 def100_last ) { _defend100_last = def100_last; }
    inline UInt8& getDefend100Last() { return _defend100_last; }

	inline float getAttackAdd2() {return _attackAdd2;}
    inline float getMagAttackAdd2() {return _magAtkAdd2;}
	inline float getDefendAdd2() {return _defAdd2;}
	inline float getMagDefendAdd2() {return _magDefAdd2;}
	inline float getHitrateAdd2() {return _hitrateAdd2;}
	inline float getEvadeAdd2() {return _evadeAdd2;}
	inline float getCriticalAdd2() {return _criticalAdd2;}
	inline float getCriticalDmgAdd2() {return _criticalDmgAdd2;}
	inline float getPierceAdd2() {return _pierceAdd2;}
	inline float getCounterAdd2() {return _counterAdd2;}
	inline float getMagResAdd2() {return _magResAdd2;}
	inline Int32 getMaxHPAdd2() {return _maxhpAdd2;}
	inline Int32 getActionAdd2() {return _maxActionAdd2;}
    inline float getToughAdd2() { return _toughAdd2;}
    inline float getAtkReduce2() { return _atkreduce2; }
    inline float getMagAtkReduce2() { return _magatkreduce2; }

	inline void setAttackAdd2(float v) {_attackAdd2 = v;}
	inline void setMagAttackAdd2(float v) {_magAtkAdd2 = v;}
	inline void setDefendAdd2(float v) {_defAdd2 = v;}
	inline void setMagDefendAdd2(float v) {_magDefAdd2 = v;}
	inline void setHitrateAdd2(float v) {_hitrateAdd2 = v;}
	inline void setEvadeAdd2(float v) {_evadeAdd2 = v;}
	inline void setCriticalAdd2(float v) {_criticalAdd2 = v;}
	inline void setCriticalDmgAdd2(float v) {_criticalDmgAdd2 = v;}
	inline void setPierceAdd2(float v) {_pierceAdd2 = v;}
	inline void setCounterAdd2(float v) {_counterAdd2 = v;}
	inline void setMagResAdd2(float v) {_magResAdd2 = v;}
	inline void setMaxHPAdd2(Int32 v) {_maxhpAdd = v;}
	inline void setActionAdd2(Int32 v) {_maxActionAdd2 = v;}
    inline void setToughAdd2(float v) {_toughAdd2 = v;}
    inline void setAtkReduce2(float v) { _atkreduce2 = v; }
    inline void setMagAtkReduce2(float v) { _magatkreduce2 = v; }

    inline UInt8 getSoulExtraAura() { return _fighter->getSoulExtraAura(); }

private:
	void updateBuffExtras();

private:
	GObject::Fighter * _fighter;
	float _strength, _agility, _physique, _intelligence, _will, _soul, _aura, _auraMax, _tough;
	float _attack, _magatk, _defend, _magdef, _hitrate, _evade;
    float _critical, _criticaldmg, _pierce, _counter, _magres;
    float _atkreduce, _magatkreduce;
	UInt32 _maxhp;
    Int32 _maxAction;
	float _attackAdd, _magAtkAdd, _defAdd, _magDefAdd, _hitrateAdd, _evadeAdd;
    float _criticalAdd, _criticalDmgAdd, _pierceAdd, _counterAdd, _magResAdd, _toughAdd;
	Int32 _maxhpAdd, _maxActionAdd;
    UInt8 _atkAdd_last, _magAtkAdd_last, _defAdd_last, _magDefAdd_last, _hitrateAdd_last, _evadeAdd_last;
    UInt8 _criticalAdd_last, _criticalDmgAdd_last, _pierceAdd_last, _counterAdd_last, _magResAdd_last, _toughAdd_last;
    UInt8 _maxhpAdd_last, _maxActionAdd_last;
    UInt8 _atkreduce_last, _magatkreduce_last;
	const GData::Formation::GridEffect * _formEffect;
	Script::BattleFormula * _formula;
    UInt8 _forgetLevel, _forgetRound;
	/* 武将状态 0-1bit:增强符
	   2-3bit:群体增强符
	   b4:愈战愈勇
	   28-31bit: holy level */
	UInt32 _flag;

    UInt8 _poisonRound;
    UInt8 _poisonLevel;
    UInt8 _stunRound;
    UInt8 _stunLevel;
    UInt8 _confuseRound;
    UInt8 _confuseLevel;
    UInt8 _weakRound;
    UInt8 _weakLevel;

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
    std::vector<GData::SkillItem> _passiveSkillAftNAtk100;

    std::vector<GData::SkillItem> _passiveSkillPreAtk;
    std::vector<GData::SkillItem> _passiveSkillAftAtk;
    std::vector<GData::SkillItem> _passiveSkillBeAtk;
    std::vector<GData::SkillItem> _passiveSkillAftEvd;
    std::vector<GData::SkillItem> _passiveSkillAftRes;
    std::vector<GData::SkillItem> _passiveSkillEnter;
    std::vector<GData::SkillItem> _passiveSkillDead;
    std::vector<GData::SkillItem> _passiveSkillAftNAtk;

    bool _revival;
    UInt8 _activeSkillIdx;
    // 入场时技能效果
    UInt8 _immuneLevel[8];
    UInt8 _immune;
    UInt8 _immuneRound[8];
    bool _evade100;
    UInt8 _evade100_cd;
    UInt8 _evade100_last;
    UInt8 _evade100_base_cd;
    UInt8 _evade100_base_last;
    bool _defend100;
    UInt8 _defend100_cd;
    UInt8 _defend100_last;
    UInt8 _defend100_base_cd;
    UInt8 _defend100_base_last;
	float _attackAdd2, _magAtkAdd2, _defAdd2, _magDefAdd2, _hitrateAdd2, _evadeAdd2;
    float _criticalAdd2, _criticalDmgAdd2, _pierceAdd2, _counterAdd2, _magResAdd2, _toughAdd2;
    float _atkreduce2, _magatkreduce2;
	Int32 _maxhpAdd2, _maxActionAdd2;

    UInt16 _fakeDeadTimes;
public:
    void fakeDead();
    inline UInt16 getFakeDeadTimes() { return _fakeDeadTimes; }

public:
	enum StatusFlag
	{
		Enh1 = 0x03,
		Enh2 = 0x0C,
		Enh3 = 0x10,
        Enh4 = 0x20,
        Enh5 = 0x40,
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

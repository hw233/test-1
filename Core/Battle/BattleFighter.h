#ifndef _BATTLEFIGHTER_H_
#define _BATTLEFIGHTER_H_

#include "GObject/Fighter.h"
#include "GData/Formation.h"
#include "GData/AttrExtra.h"
#include "BattleObject.h"
#include "GData/SkillTable.h"
#include "GData/SkillStrengthen.h"

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
    struct SkillBase;
    struct SkillItem;
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
	inline Int32 getExtraAttack() { return _attrExtra.attack; }
	inline float getExtraAttackP() { return _attrExtra.attackP; }
	inline Int32 getExtraMagAttack() { return _attrExtra.magatk; }
	inline float getExtraMagAttackP() { return _attrExtra.magatkP; }
	inline Int32 getExtraDefend() { return _attrExtra.defend; }
	inline float getExtraDefendP() { return _attrExtra.defendP; }
	inline Int32 getExtraMagDefend() { return _attrExtra.magdef; }
	inline float getExtraMagDefendP() { return _attrExtra.magdefP; }
	inline Int32 getExtraHP() { return _attrExtra.hp; }
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
	inline Int32 getBaseAttack() {return _fighter->getBaseAttack();}
	inline Int32 getBaseMagAttack() {return _fighter->getBaseMagAttack();}
	inline Int32 getBaseDefend() {return _fighter->getBaseDefend();}
	inline Int32 getBaseMagDefend() {return _fighter->getBaseMagDefend();}
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
	inline float getAttack() {float ret = _attack + _attackAdd + _attackAdd2 + _atkAddSpecial + _atkDecSpecial + _moAttackAdd; return (ret > 0 ? ret : 0);}
	inline float getMagAttack() {float ret = _magatk + _magAtkAdd + _magAtkAdd2 + _magAtkAddSpecial + _magAtkDecSpecial + _moMagAtkAdd; return (ret > 0 ? ret : 0);}
	inline float getDefend() {float ret = _defend + _defAdd + _defAdd2; return (ret > 0 ? ret : 0);}
	inline float getMagDefend() {float ret = _magdef + _magDefAdd + _magDefAdd2; return (ret > 0 ? ret : 0);}
	float getHitrate(BattleFighter* defgt);
	float getEvade(BattleFighter* defgt);
	float getCritical(BattleFighter* defgt);
	float getCriticalDmg();
    float calcCriticalDmg(BattleFighter* defender);
	float getPierce(BattleFighter* defgt);
	float getCounter(BattleFighter* defgt, const GData::SkillBase* skil = NULL);
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
    inline float getAtkReduce() { return _atkreduce + _atkreduce2 + _atkreduce3 + _moAtkReduce; }
    inline float getMagAtkReduce() { return _magatkreduce + _magatkreduce2 + _magatkreduce3 + _moMagAtkReduce; }

	void setAttackAdd(float v, UInt16 last = 0);
	void setMagAttackAdd(float v, UInt16 last = 0);
	void setDefendAdd(float v, UInt16 last = 0);
	void setMagDefendAdd(float v, UInt16 last = 0);
	void setHitrateAdd(float v, UInt16 last = 0);
	void setEvadeAdd(float v, UInt16 last = 0);
	void setCriticalAdd(float v, UInt16 last = 0);
	void setCriticalDmgAdd(float v, UInt16 last = 0);
	void setPierceAdd(float v, UInt16 last = 0);
	void setCounterAdd(float v, UInt16 last = 0);
	void setMagResAdd(float v, UInt16 last = 0);
	void setMaxHPAdd(Int32 v, UInt16 last = 0);
	void setActionAdd(Int32 v, UInt16 last = 0);
    void setToughAdd(float v, UInt16 last = 0);
    void setAtkReduce(float v, UInt16 last = 0);
    void setMagAtkReduce(float v, UInt16 last = 0);
    inline void AddAura(Int32 v) {_aura += v; if(_aura > _auraMax) _aura = _auraMax; else if(_aura < 0) _aura = 0;}
    inline void setAura(Int32 v) {_aura = v > _auraMax ? _auraMax : v;}

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
    inline UInt8& getAtkReduce3Last() { return _atkreduce3_last;}
    inline UInt8& getMagAtkReduce3Last() { return _magatkreduce3_last;}

    void setAtkReduce3(float v, UInt16 last);
    void setMagAtkReduce3(float v, UInt16 last);
    void setPuduDebuf(float v, UInt16 last);

	inline UInt32 getLostHP() { Int64 tmp = _maxhp + _maxhpAdd + _maxhpAdd2; UInt32 mhp = (tmp > 0 ? tmp : 0); if(mhp > _hp) return mhp - _hp; return 0; }

	UInt32 regenHP(UInt32 u, bool weak = false);

	void updateAllAttr();
	void initStats(bool);
	void postInit();
	float calcAttack(bool& isCritical, BattleFighter* defender , float* pCf);
	bool calcHit(BattleFighter * defender, const GData::SkillBase* skill);
	bool calcCounter(BattleFighter* attacker, bool ranged = false, const GData::SkillBase* skill = NULL);
	bool canBeCounter();
	bool calcPierce(BattleFighter* defender);
    float calcTherapy(bool& isCritical, bool& first, const GData::SkillBase* skill);
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
	inline UInt32 getFlag2() { return _flag2; }
	inline void addFlag2(UInt32 f) { _flag2 |= f; }
	inline void delFlag2(UInt32 f) { _flag2 &= ~f; }
	inline bool hasFlag2(UInt32 f) { return (_flag2 & f) > 0;}
	void setAttrExtra(UInt8, UInt8, UInt8);
    void setImmuneLevel(UInt16 state, UInt8 f);
    void setImmune(UInt16 f) { _immune |= f; }
    void setImmuneRound(UInt16 state, UInt8 f);
    UInt8 getImmuneLevel(UInt16 state);
    inline UInt16 getImmune() { return _immune; }
    UInt8 getImmuneRound(UInt16 state);
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

//    inline UInt32 getWeakLevel() { return _weakLevel; }
//    inline void setWeakLevel(UInt32 l) { _weakLevel = l; }
//    inline UInt32 getWeakRound() { return _weakRound; }
//    inline void setWeakRound(UInt32 r) { _weakRound = r; }

    const GData::SkillBase* getTherapySkill();
    const GData::SkillBase* getActiveSkill(bool need_therapy = false, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillPrvAtk100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftAtk100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillBeAtk100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftEvd100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftRes100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillEnter100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillDead100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftNAtk100(size_t& idx, bool noPossibleTarget = false);

    const GData::SkillBase* getPassiveSkillPreAtk(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftAtk(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillBeAtk(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftEvd(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftRes(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillEnter(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillDead(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftNAtk(bool noPossibleTarget = false);
    void releaseSkillCD(int cd);
    void releaseSkillCD(std::vector<GData::SkillItem>& skill, int cd);


    const GData::SkillBase* getPassiveSkillOnTherapy();
    const GData::SkillBase* getPassiveSkillOnSkillDmg();
    const GData::SkillBase* getPassiveSkillOnOtherDead();

    const GData::SkillBase* getPassiveSkill(std::vector<GData::SkillItem>& passiveSkill, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkill100(std::vector<GData::SkillItem>& passiveSkill100, size_t& idx, bool noPossibleTarget = false);

    GData::SkillStrengthenBase* getSkillStrengthen(UInt16 skillId);
    void updateSkillStrengthen(UInt16 skillId);

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
	inline void setMaxHPAdd2(Int32 v) {_maxhpAdd2 = v;}
	inline void setActionAdd2(Int32 v) {_maxActionAdd2 = v;}
    inline void setToughAdd2(float v) {_toughAdd2 = v;}
    inline void setAtkReduce2(float v) { _atkreduce2 = v; }
    inline void setMagAtkReduce2(float v) { _magatkreduce2 = v; }

    inline UInt8 getSoulExtraAura() { return _fighter->getSoulExtraAura(); }
    // 通过天赋术额外保留部分灵力
    inline UInt8 getAuraLeft(){ return _fighter->getSoulAuraLeft(); } 
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
	UInt32 _flag2;

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
    std::vector<GData::SkillItem> _therapySkill;
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

    std::vector<GData::SkillItem> _passiveSkillOnTherapy;
    std::vector<GData::SkillItem> _passiveSkillOnSkillDmg;
    std::vector<GData::SkillItem> _passiveSkillOnOtherDead;

    std::map<UInt16, GData::SkillStrengthenBase*> _skillStrengthen;

    bool _revival;
    UInt8 _activeSkillIdx;
    UInt8 _therapySkillIdx;
    // 入场时技能效果
    UInt8 _immuneLevel[8];
    UInt16 _immune;
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

    float _atkreduce3, _magatkreduce3, _pudu_debuf;
    UInt8 _atkreduce3_last, _magatkreduce3_last, _pudu_debuf_last;

    float _deep_forget_dmg_extra;
    UInt8 _deep_forget_last;
    float _deep_stun_dmg_extra;
    UInt8 _deep_stun_last;

//    float _therapy_dec;
//    UInt8 _therapy_dec_last;

//    float _therapy_add;
//    UInt8 _therapy_add_last;

    float _therapy_buff;
    UInt8 _therapy_buff_last;

    float _aura_dec;
    UInt8 _aura_dec_last;

    float _bleed1, _bleed2, _bleed3;
    UInt8 _bleed1_last, _bleed2_last, _bleed3_last;
    UInt16 _immune2;

    float _def_dec;
    UInt8 _def_dec_last;
    UInt8 _def_dec_times;


    UInt32 _aura_bleed;
    UInt8 _aura_dec_cd, _aura_bleed_last;
    UInt32 _stun_bleed;
    UInt8 _stun_cd, _stun_bleed_last;
    UInt32 _confuse_bleed;
    UInt8 _confuse_cd, _confuse_bleed_last;

    UInt16 _colorStock;
    UInt8 _colorStockTimes, _colorStockLast;

    bool _summon;
    UInt8 _summonLast;
    UInt8 _moAuraBuf, _moAuraBufLast;
    bool _moEvade100;
    UInt8 _moEvade100Last;
    bool _hideBuf;
    UInt8 _hideBufLast;
    bool _markMo;
    UInt8 _markMoLast;
    float _blind;
    UInt8 _blind_last;
    float _deep_blind_dmg_extra;
    UInt8 _deep_blind_last;
	float _moAttackAdd, _moMagAtkAdd, _moAtkReduce, _moMagAtkReduce;
	UInt8 _moAttackAddCD, _moMagAtkAddCD, _moAtkReduceCD, _moMagAtkReduceCD;
    float _bleedMo;
    UInt8 _bleedMoLast;
    BattleFighter* _summoner;
    UInt8 _unSummonAura;

    // cotton add for skillstrengthen
public:
    inline float getAtkAddSpecial(){ return _atkAddSpecial; }
    inline Int16 getAtkSpecialLast(){return _atkSpecialLast;}
    inline void setAtkSpecialLast(Int16 nLast) { _atkSpecialLast = nLast; }
    inline void setAtkAddSpecial(float v, Int16 nlast){ _atkAddSpecial = v; _atkSpecialLast = nlast; }

    inline float getMagAtkAddSpecial(){ return _magAtkAddSpecial; }
    inline Int16 getMagAtkSpecialLast(){return _magAtkSpecialLast;}
    inline void setMagAtkSpecialLast(Int16 nLast) { _magAtkSpecialLast = nLast; }
    inline void setMagAtkAddSpecial(float v, Int16 nlast){ _magAtkAddSpecial = v; _magAtkSpecialLast = nlast; }

    inline float getAtkDecSpecial(){ return _atkDecSpecial; }
    inline Int16 getAtkDecSpecialLast(){return _atkDecSpecialLast;}
    inline void setAtkDecSpecialLast(Int16 nLast) { _atkDecSpecialLast = nLast; }
    inline void setAtkDecSpecial(float v, Int16 nlast){ _atkDecSpecial = v; _atkDecSpecialLast = nlast; }

    inline float getMagAtkDecSpecial(){ return _magAtkDecSpecial; }
    inline Int16 getMagAtkDecSpecialLast(){return _magAtkDecSpecialLast;}
    inline void setMagAtkDecSpecialLast(Int16 nLast) { _magAtkDecSpecialLast = nLast; }
    inline void setMagAtkDecSpecial(float v, Int16 nlast){ _magAtkDecSpecial = v; _magAtkDecSpecialLast = nlast; }

    inline void setSkillUsedChangeAttr(float fvalue, Int16 nlast, int nattr)
    {
        _skillUsedChangeAttrValue = fvalue;
        _skillUsedChangeAttrLast = nlast;
        _skillUsedChangeAttr = nattr;
    }
    inline void getSkillUsedChangeAttr(float& fvalue, Int16& nlast, int& nattr)
    {
       fvalue = _skillUsedChangeAttrValue;
       nlast = _skillUsedChangeAttrLast;
       nattr = _skillUsedChangeAttr;
    }

    inline bool getSingleAttackFlag(){ return _bSingleAttackFlag; }
    inline void setSingleAttackFlag(bool b){ _bSingleAttackFlag =  b; }

    inline bool getMainTargetDeadFlag(){ return _bMainTargetDead; }
    inline void setMainTargetDeadFlag(bool b){ _bMainTargetDead = b; }

    inline float getHitChangeByPeerless(){ return _hitChangeByPeerless; }
    inline void setHitChangeByPeerless(float v){ _hitChangeByPeerless = v; }
    inline float getCounterChangeByPeerless(){ return _counterChangeByPeerless; }
    inline void setCounterChangeByPeerless(float v){ _counterChangeByPeerless = v; }

    inline float getBleedRandom(){ return _bleedRandom; }
    inline void setBleedRandom(float v, Int16 last){ _bleedRandom = v; _bleedRandomLast = last; }
    inline Int16& getBleedRandomLast(){ return _bleedRandomLast; }
    inline void setBleedRandomLast(Int16 v){ _bleedRandomLast = v; }
    inline UInt8 getBleedAttackClass(){ return _bleedAttackClass; }
    inline void setBleedAttackClass(UInt8 v){ _bleedAttackClass = v; }

    inline float getBleedBySkill(){ return _bleedBySkill; }
    inline void setBleedBySkill(float v, Int16 last){ _bleedBySkill = v; _bleedBySkillLast = last; }
    inline Int16& getBleedBySkillLast(){ return _bleedBySkillLast; }
    inline void setBleedBySkillLast(Int16 v){ _bleedBySkillLast = v; }
    inline UInt8 getBleedBySkillClass(){ return _bleedBySkillClass; }
    inline void setBleedBySkillClass(UInt8 v){ _bleedBySkillClass = v; }

    inline int getAttackIndex(){return _nCurrentAttackIndex; }
    inline void setAttackIndex(int nindex){ _nCurrentAttackIndex = nindex; }

private:
    float _atkAddSpecial;  // 技能符文吸收对方伤害额外增加的物攻
    Int16 _atkSpecialLast;
    float _magAtkAddSpecial;
    Int16 _magAtkSpecialLast;
    float _atkDecSpecial;  // 技能符文:自己被对方吸收的攻击
    Int16 _atkDecSpecialLast;
    float _magAtkDecSpecial;
    Int16 _magAtkDecSpecialLast;

    // 自己对自己上buf的时候，部分属性在技能使用的过程中计算，但是加成放在技能使用之后，所以加了以下三个变态的变量暂存，策划强烈要求的，后来人掉进这个坑里，或者看不明白什么意思，请无视
    float _skillUsedChangeAttrValue;
    Int16 _skillUsedChangeAttrLast;
    int   _skillUsedChangeAttr;
    // end

    float _bleedRandom;
    Int16 _bleedRandomLast;
    UInt8 _bleedAttackClass;  // 流血的时候，前端要根据攻击者的类型表现特效

    float _bleedBySkill;      // 无量七色符文流血
    Int16 _bleedBySkillLast;
    UInt8 _bleedBySkillClass;  // 流血的时候，前端要根据攻击者的类型表现特效

    // 由于无双技能导致的额外命中增加、反击减少，只能作用在本次，所以每次攻击完毕，直接清除，也不用通知前端
    float _hitChangeByPeerless;
    float _counterChangeByPeerless;
    
    bool  _bSingleAttackFlag;  // 群体攻击时候，只攻击到一人的标记
    bool  _bMainTargetDead;    // 主目标被打死（目前只在area为0的时候做了标记，以后需要就慢慢加好了）
    int   _nCurrentAttackIndex;  // 当前攻击对象索引，只在我自己需要的时候赋值过，后面的人要用的时候，请在攻击之前自己设置好

public:
    void fakeDead();
    inline UInt16 getFakeDeadTimes() { return _fakeDeadTimes; }

    inline UInt8& getDeepForgetLast() { return _deep_forget_last; }
    inline float getDeepForgetDmgExtra() { return _deep_forget_dmg_extra; }
    inline void setDeepForgetDmgExtra(float v, UInt8 l) { _deep_forget_dmg_extra = v; _deep_forget_last = l; }
    inline UInt8& getDeepStunLast() { return _deep_stun_last; }
    inline float getDeepStunDmgExtra() { return _deep_stun_dmg_extra; }
    inline void setDeepStunDmgExtra(float v, UInt8 l) { _deep_stun_dmg_extra = v; _deep_stun_last = l; }

//    inline UInt8& getTherapyDecLast() { return _therapy_dec_last; }
//    inline float getTherapyDec() { return _therapy_dec; }
//    inline void setTherapyDec(float value, UInt8 last) { _therapy_dec = value; _therapy_dec_last = last; }

    // 加治疗效果
//    inline UInt8& getTherapyAddLast() { return _therapy_add_last; }
//    inline float getTherapyAdd() { return _therapy_add; }
//    inline void setTherapyAdd(float value, UInt8 last) { _therapy_add = value; _therapy_add_last = last; }
    // 治疗效果加成（可能正可能负）
    inline UInt8& getTherapyBuffLast() { return _therapy_buff_last; }
    inline float getTherapyBuff() { return _therapy_buff; }
    inline void setTherapyBuff(float value, UInt8 last) { _therapy_buff = value; _therapy_buff_last = last; }

    inline UInt8& getAuraDecLast() { return _aura_dec_last; }
    inline float getAuraDec() { return _aura_dec; }
    inline void setAuraDec(float value, UInt8 last) { _aura_dec = value; _aura_dec_last = last; }

    inline UInt8& getBleed1Last() { return _bleed1_last; }
    inline float getBleed1() { return _bleed1; }
    inline void setBleed1(float value, UInt8 last) { _bleed1 = value; _bleed1_last = last; }
    inline UInt8& getBleed2Last() { return _bleed2_last; }
    inline float getBleed2() { return _bleed2; }
    inline void setBleed2(float value, UInt8 last) { _bleed2 = value; _bleed2_last = last; }
    inline UInt8& getBleed3Last() { return _bleed3_last; }
    inline float getBleed3() { return _bleed3; }
    inline void setBleed3(float value, UInt8 last) { _bleed3 = value; _bleed3_last = last; }


    inline UInt8& getAuraDecCD() { return _aura_dec_cd; }
    inline UInt8& getAuraBleedLast() { return _aura_bleed_last; }
    inline float getAuraBleed() { return _aura_bleed; }
    inline void setAuraBleed(float value, UInt8 last, UInt8 cd) { _aura_bleed = value; _aura_bleed_last = last; _aura_dec_cd = cd; }

    inline UInt8& getStunCD() { return _stun_cd; }
    inline UInt8& getStunBleedLast() { return _stun_bleed_last; }
    inline float getStunBleed() { return _stun_bleed; }
    inline void setStunBleed(float value, UInt8 last, UInt8 cd) { _stun_bleed = value; _stun_bleed_last = last; _stun_cd = cd; }

    inline UInt8& getConfuceCD() { return _confuse_cd; }
    inline UInt8& getConfuceBleedLast() { return _confuse_bleed_last; }
    inline float getConfuceBleed() { return _confuse_bleed; }
    inline void setConfuceBleed(float value, UInt8 last, UInt8 cd) { _confuse_bleed = value; _confuse_bleed_last = last; _confuse_cd = cd; }


    inline UInt16 getImmune2() { return _immune2; }
    inline void setImmune2(UInt16 v) { _immune2 = v; }

    inline UInt8& getDefDecLast() { return _def_dec_last; }
    inline float getDefDec() { return _def_dec; }
    inline void setDefDec(float value, UInt8 last) { _def_dec = value; _def_dec_last = last; }
    inline UInt8 getDefDecTimes() { return _def_dec_times; }
    inline void setDefDecTimes(UInt8 v) { _def_dec_times = v; }

    inline UInt16 getColorStock() { return _colorStock; }
    inline UInt8& getColorStockTimes() { return _colorStockTimes; }
    inline UInt8& getColorStockLast() { return _colorStockLast; }
    inline void setColorStock(UInt16 v, UInt8 ts, UInt8 ls) { _colorStock = v; _colorStockTimes = ts; _colorStockLast = ls; }


	virtual void makeDamage(UInt32& u);

    BattleFighter* summonSelf(float factor, UInt8 last);
    void clearSkill();
    void setSummonFactor(float factor, UInt8 last);
    bool releaseSummon();
    inline void setMoAuraBuf(UInt8 value, UInt8 last) { _moAuraBuf = value; _moAuraBufLast = last; }
    inline UInt8 getMoAuraBuf() { return _moAuraBuf; }
    bool releaseMoAuraBuf();
    inline void setMoEvade100(UInt8 value, UInt8 last) { _moEvade100 = value; _moEvade100Last = last; }
    bool getMoEvade100();
    bool releaseMoEvade100();
    void setHideBuf(UInt8 value, UInt8 last);
    bool releaseHideBuf();
    inline void setMarkMo(bool value, UInt8 last) { _markMo = value; _markMoLast= last; }
    inline bool isMarkMo() { return _markMo; }
    bool releaseMarkMo();
    inline void setDeepBlind(bool value, UInt8 last) { _deep_blind_dmg_extra = value; _deep_blind_last = last; }
    inline float getDeepBlindDmgExtra() { return _deep_blind_dmg_extra; }
    bool releaseDeepBlind();
    inline void setBlind(bool value, UInt8 last) { _blind = value; _blind_last = last; }
    inline float getBlind() { return _blind; }
    bool releaseBlind();

    inline void setMoAttackAdd(float value, UInt8 last) { _moAttackAdd = value; _moAttackAddCD = last; }
    inline void setMoMagAtkAdd(float value, UInt8 last) { _moMagAtkAdd = value; _moMagAtkAddCD = last; }
    inline void setMoAtkReduce(float value, UInt8 last) { _moAtkReduce = value; _moAtkReduceCD = last; }
    inline void setMoMagAtkReduce(float value, UInt8 last) { _moMagAtkReduce = value; _moMagAtkReduceCD = last; }

    inline float getMoAttackAdd() { return _moAttackAdd; }
    inline float getMoMagAtkAdd() { return _moMagAtkAdd; }
    inline float getMoAtkReduce() { return _moAtkReduce; }
    inline float getMoMagAtkReduce() { return _moMagAtkReduce; }

    bool releaseMoAttackAdd();
    bool releaseMoMagAtkAdd();
    bool releaseMoAtkReduce();
    bool releaseMoMagAtkReduce();

    inline float getBleedMo() { return _bleedMo; }
    inline void setBleedMo(float value, UInt8 last) { _bleedMo = value; _bleedMoLast = last; }
    bool releaseBleedMo();

    void setUnSummonAura(BattleFighter* bf, UInt32 aura) { _summoner = bf, _unSummonAura = aura; }
    bool isSummon() { return _summon; }
    BattleFighter* getSummoner() { return _summoner; }
    UInt8 getUnSummonAura() { return _unSummonAura; }

public:
	enum StatusFlag
	{
		Enh1 = 0x03,
		Enh2 = 0x0C,
		Enh3 = 0x10,
        Enh4 = 0x20,
        Enh5 = 0x40,
        Enh6 = 0x80000000,
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
	enum StatusFlag2
	{
		AthlEnh1 = 0x01,
		AthlEnh2 = 0x02,
		AthlEnh3 = 0x04,
        AthlEnh4 = 0x08,
        AthlEnh5 = 0x10,
		AthlEnh6 = 0x20,
		AthlEnh7 = 0x40,
		AthlEnh8 = 0x80,
		AthlEnh9 = 0x100,
		AthlEnh31 = 0x10000,
		AthlEnh32 = 0x20000,
		AthlEnh33 = 0x40000,
        AthlEnh34 = 0x80000,
        AthlEnh35 = 0x100000,
		AthlEnh36 = 0x200000,
		AthlEnh37 = 0x400000,
    };
};

}

#endif // _BATTLEFIGHTER_H_

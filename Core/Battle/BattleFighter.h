#ifndef _BATTLEFIGHTER_H_
#define _BATTLEFIGHTER_H_

#include "GObject/Fighter.h"
#include "GData/Formation.h"
#include "GData/AttrExtra.h"
#include "BattleObject.h"
#include "GData/SkillTable.h"
#include "GData/SkillStrengthen.h"
#include "GData/LBSkillTable.h"

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
    struct LBSkillItem;
}

namespace Battle
{

    enum SneakStatus
    {
        e_sneak_none = 0,
        e_sneak_on = 1,
        e_sneak_atk = 2
    };



class BattleFighter:
	public BattleObject
{
	friend class BattleSimulator;

#define BLEED_TYPE_FLAG_NONE            0x00000000      // û��Ѫ
#define BLEED_TYPE_FLAG_1               0x00000001      // ����Ѫ
#define BLEED_TYPE_FLAG_2               0x00000002      // ����Ѫ
#define BLEED_TYPE_FLAG_3               0x00000004      // ����Ѫ
#define BLEED_TYPE_FLAG_MO              0x00000008      // ī��Ѫ
#define BLEED_TYPE_FLAG_PET             0x00000010      // ������Ѫ�����ͣ�
#define BLEED_TYPE_FLAG_POISON          0x00000020      // ����Ѫ
#define BLEED_TYPE_FLAG_SELF            0x00000040      // �Լ���ɵ���Ѫ�������Է٣�
#define BLEED_TYPE_FLAG_AURA            0x00000080      // ��˫��ɵ���Ѫ���������롢�Ž����ȡ��̹���, ���������
#define BLEED_TYPE_FLAG_CONFUSE         0x00000100      // ������Ѫ (��ٷ���)
#define BLEED_TYPE_FLAG_STUN            0x00000200      // ѣ����Ѫ (��ٷ���)
#define BLEED_TYPE_FLAG_BLIND           0x00000400      // ��ä��Ѫ
#define BLEED_TYPE_FLAG_LINGSHI         0x00000800      // ��ʴ��Ѫ
#define BLEED_TYPE_FLAG_LINGYAN         0x00001000      // ������Ѫ
#define BLEED_TYPE_FLAG_FIELD_GAPE      0x00002000      // ����Ч����Ѫ

#define HP_ID           1
#define ACTION_ID       2
#define HIT_ID          3
#define EVADE_ID        4
#define CRITICAL_ID     5
#define PIERCE_ID       6
#define COUNTER_ID      7
#define TOUGH_ID        8
#define MAGRES_ID       9
#define STRENGTH_ID     10
#define PHYSIQUE_ID     11
#define AGILITY_ID      12
#define INTELLIGENCE_ID 13
#define WILL_ID         14
#define ATTACK_ID       15
#define MAGATK_ID       16
#define DEFEND_ID       17
#define MAGDEF_ID       18
#define CRIDMG_ID       19
#define CRIIMMUNE_ID    20
#define AURA_ID         21

public:
	BattleFighter(Script::BattleFormula *, GObject::Fighter * = NULL, UInt8 side = 0, UInt8 pos = 0);

	void setFighter(GObject::Fighter * f);
	inline void setFormationEffect(const GData::Formation::GridEffect * fe) {_formEffect = fe;}

	inline UInt32 getId() { return _fighter->getId(); }
	inline UInt8 getClass() { return _fighter->getClass(); }
	inline UInt8 getLevel() { return _fighter->getLevel(); }
	inline UInt8 getLevelInLua() { return _fighter->getLevelInLua(); }
	inline UInt8 getColor() { return _fighter->getColor(); }
	inline float getPotential() { return _fighter->getPotential(); }
    inline UInt32 getBuffData( UInt8 idx, UInt32 now ) { return _fighter->getBuffData(idx, now); }
    UInt8 getClassAndSex();

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
    inline float getExtraCriticalDmgImmune() { return _attrExtra.criticaldmgimmune+_fighter->getAcupointsGoldAttr(1); }

	inline GObject::Fighter * getFighter() {return _fighter;}

	inline bool isNpc() { return _fighter->isNpc(); }
	inline bool isBoy() { return _fighter->isBoy(); }
	inline bool isMale() { return _fighter->isMale(); }
    inline bool isPet()     { return _fighter->isPet(); }
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
	inline float getAcupointsGoldAttr(UInt8 attrId) { return _fighter->getAcupointsGoldAttr(attrId); }
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

	inline float getAttack() 
    {
        float ret = _attack + _attackAdd + _attackAdd2 + _atkAddSpecial + _atkDecSpecial 
            + _moAttackAdd + _petAttackAdd + (_petExAtkEnable?_petExAtk:0) + _counter_spirit_atk_add + _pet_coatk + _hpAtkAdd; 
        return  ret;
    }
	inline float getMagAttack() 
    {
        float ret = _magatk + _magAtkAdd + _magAtkAdd2 + _magAtkAddSpecial + _magAtkDecSpecial 
            + _moMagAtkAdd + _petMagAtkAdd + _counter_spirit_magatk_add + _pet_coatk + _hpMagAtkAdd; 
        return ret;
    }

	inline float getDefend() 
    {
        float ret = _defend + _defAdd + _defAdd2 + _counter_spirit_def_add + _defendChangeSS; 
        return (ret > 0 ? ret : 0);
    }
	inline float getMagDefend() 
    {
        float ret = _magdef + _magDefAdd + _magDefAdd2 + _counter_spirit_magdef_add + _fire_defend + _magDefendChangeSS;
        return (ret > 0 ? ret : 0);
    }

    void setDefend(float percent)
    {
        _defend *= percent;
        _defAdd *= percent;
        _defAdd2 *= percent;
        _counter_spirit_def_add *= percent;
        _defendChangeSS *= percent;

        _magdef *= percent;
        _magDefAdd *= percent;
        _magDefAdd2 *= percent;
        _counter_spirit_magdef_add *= percent;
        _fire_defend *= percent;
        _magDefendChangeSS *= percent;
    }

	float getHitrate(BattleFighter* defgt);
	float getEvade(BattleFighter* defgt);
	float getCritical(BattleFighter* defgt);
	float getCriticalDmg();
    float calcCriticalDmg(BattleFighter* defender);
	float getPierce(BattleFighter* defgt);
	float getCounter(BattleFighter* defgt, const GData::SkillBase* skil = NULL);
	float getMagRes(BattleFighter* defgt);
	float getTough(BattleFighter* defgt);
    float getCriticalDmgImmune() { return _attrExtra.criticaldmgimmune+_fighter->getAcupointsGoldAttr(1); }
	inline UInt32 getMaxHP() {Int64 ret = _maxhp + _maxhpAdd + _maxhpAdd2; return (ret > 0 ? ret : 0);}
    float getHPP() { float ret = static_cast<float>(getHP())/ static_cast<float>(getMaxHP()); return ret; }
	inline Int32 getAction() {Int32 ret = _maxAction + _maxActionAdd + _maxActionAdd2; return (ret > 0 ? ret : 0);}
	inline const GData::Formation::GridEffect * getFormationEffect() const {return _formEffect;}

	inline float getAttackAdd() {return _attackAdd;}
    inline float getMagAttackAdd() {return _magAtkAdd;}
	inline float getDefendAdd() {return _defAdd;}
	inline float getMagDefendAdd() {return _magDefAdd;}
    inline float getDefendChangeSS() {return _defendChangeSS;}
    inline float getMagDefendChangeSS() {return _magDefendChangeSS;}
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
    inline float getAtkReduce() { return _atkreduce + _atkreduce2 + _atkreduce3 + _moAtkReduce + _hpAtkReduce - (_flawDamageAdd * _flawCount); }
    inline float getMagAtkReduce() { return _magatkreduce + _magatkreduce2 + _magatkreduce3 + _moMagAtkReduce + _hpMagAtkReduce - (_flawDamageAdd * _flawCount); }

	void setAttackAdd(float v, UInt16 last = 0);
	void setMagAttackAdd(float v, UInt16 last = 0);
	void setDefendAdd(float v, UInt16 last = 0);
	void setMagDefendAdd(float v, UInt16 last = 0);
    void setDefendChangeSS(float v, UInt16 last = 0);
    void setMagDefendChangeSS(float v, UInt16 last = 0);
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
    inline UInt8& getDefendChangeSSLast() { return _defendChangeSSLast;}
    inline UInt8& getMagDefendChangeSSLast() { return _magDefendChangeSSLast;}
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

	UInt32 regenHP(UInt32 u, bool weak = false, float hppec = 0, float maxRhp = 0);

	void updateAllAttr();
	void initStats(bool);
	void postInit();
	float calcAttack(bool& isCritical, BattleFighter* defender , float* pCf);
	bool calcHit(BattleFighter * defender, const GData::SkillBase* skill);
	bool calcCounter(BattleFighter* attacker, const GData::SkillBase* skill = NULL);
	bool canBeCounter();
	bool calcPierce(BattleFighter* defender);
    float calcTherapy(bool& isCritical, bool& first, const GData::SkillBase* skill);
    float calcMaxTherapy(const GData::SkillBase* skill);
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
    const GData::SkillBase* getPassiveSkillAftAction100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillBeAtk100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftEvd100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftRes100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillEnter100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillDead100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftNAtk100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnAtkDmg100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnGetDmg100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnBeDmg100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnBePHYDmg100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnBeMagDmg100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnHP10P100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnHPChange100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnWithstand100(size_t& idx, bool noPossibleTarget = false);

    const GData::SkillBase* getPassiveSkillPreAtk(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftAtk(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftAction(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillBeAtk(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftEvd(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftRes(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillEnter(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillDead(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAftNAtk(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnAtkDmg(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnAtkDmgForce(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnPetProtect(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnPetProtectForce(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnPetAtk(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnGetDmg(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnBeDmg(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnBePHYDmg(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnBeMagDmg(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnHP10P(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnHPChange(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnWithstand(bool noPossibleTarget = false);

    const GData::SkillBase* getSkillSoulProtect();

    void releaseSkillCD(int cd);
    void releaseSkillCD(std::vector<GData::SkillItem>& skill, int cd);

    void updatePassiveSkill(std::vector<UInt16>& passiveSkillId, std::vector<GData::SkillItem>& passiveSkill);
    void updatePassiveSkill100(std::vector<UInt16>& passiveSkill100Id, std::vector<GData::SkillItem>& passiveSkill100);

    void updateSoulSkillDead(UInt16 skillId);
    void updateSoulSkillProtect(UInt16 skillId);
    void updatePassiveSkillPrvAtk100Status();
    void updatePassiveSkillBLTY100Status();

    const GData::SkillBase* getPassiveSkillOnTherapy();
    const GData::SkillBase* getPassiveSkillOnSkillDmg();
    const GData::SkillBase* getPassiveSkillOnOtherDead();
    const GData::SkillBase* getPassiveSkillOnPetProtect100();

    const GData::SkillBase* getPassiveSkill(std::vector<GData::SkillItem>& passiveSkill, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillForce(std::vector<GData::SkillItem>& passiveSkill, bool noPossibleTarget = false);
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
        _defendChangeSS = 0;
        _magDefendChangeSS = 0;
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
        _defendChangeSSLast = 0;
        _magDefendChangeSSLast = 0;
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
    // ͨ���츳�����Ᵽ����������
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
    float _defendChangeSS, _magDefendChangeSS;
    float _criticalAdd, _criticalDmgAdd, _pierceAdd, _counterAdd, _magResAdd, _toughAdd;
	Int32 _maxhpAdd, _maxActionAdd;
    UInt8 _atkAdd_last, _magAtkAdd_last, _defAdd_last, _magDefAdd_last, _hitrateAdd_last, _evadeAdd_last;
    UInt8 _defendChangeSSLast, _magDefendChangeSSLast;
    UInt8 _criticalAdd_last, _criticalDmgAdd_last, _pierceAdd_last, _counterAdd_last, _magResAdd_last, _toughAdd_last;
    UInt8 _maxhpAdd_last, _maxActionAdd_last;
    UInt8 _atkreduce_last, _magatkreduce_last;
	const GData::Formation::GridEffect * _formEffect;
	Script::BattleFormula * _formula;
    UInt8 _forgetLevel, _forgetRound;
	/* �佫״̬ 0-1bit:��ǿ��
	   2-3bit:Ⱥ����ǿ��
	   b4:��ս����
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

	GData::AttrExtra _attrExtraEquip; // װ������һ�����Ժ�
	GData::AttrExtra _attrExtra; // ȫ������һ�����Ժ�
	GData::AttrExtra _attrbylevel;


    GData::SkillItem _peerlessSkill;
    std::vector<GData::SkillItem> _activeSkill;
    std::vector<GData::SkillItem> _therapySkill;
    std::vector<GData::SkillItem> _passiveSkillPrvAtk100;
    std::vector<GData::SkillItem> _passiveSkillAftAtk100;
    std::vector<GData::SkillItem> _passiveSkillAftAction100;
    std::vector<GData::SkillItem> _passiveSkillBeAtk100;
    std::vector<GData::SkillItem> _passiveSkillAftEvd100;
    std::vector<GData::SkillItem> _passiveSkillAftRes100;
    std::vector<GData::SkillItem> _passiveSkillEnter100;
    std::vector<GData::SkillItem> _passiveSkillDead100;
    std::vector<GData::SkillItem> _passiveSkillAftNAtk100;
    std::vector<GData::SkillItem> _passiveSkillOnPetProtect100;
    std::vector<GData::SkillItem> _passiveSkillOnHPChange100;
    std::vector<GData::SkillItem> _passiveSkillOnWithstand100;

    std::vector<GData::SkillItem> _passiveSkillPreAtk;
    std::vector<GData::SkillItem> _passiveSkillAftAtk;
    std::vector<GData::SkillItem> _passiveSkillAftAction;
    std::vector<GData::SkillItem> _passiveSkillBeAtk;
    std::vector<GData::SkillItem> _passiveSkillAftEvd;
    std::vector<GData::SkillItem> _passiveSkillAftRes;
    std::vector<GData::SkillItem> _passiveSkillEnter;
    std::vector<GData::SkillItem> _passiveSkillDead;
    std::vector<GData::SkillItem> _passiveSkillAftNAtk;
    std::vector<GData::SkillItem> _passiveSkillOnPetProtect;
    std::vector<GData::SkillItem> _passiveSkillOnHPChange;
    std::vector<GData::SkillItem> _passiveSkillOnWithstand;

    std::vector<GData::SkillItem> _passiveSkillOnTherapy;
    std::vector<GData::SkillItem> _passiveSkillOnSkillDmg;
    std::vector<GData::SkillItem> _passiveSkillOnOtherDead;

    std::map<UInt16, GData::SkillStrengthenBase*> _skillStrengthen;

    bool _revival;
    UInt8 _activeSkillIdx;
    UInt8 _therapySkillIdx;
    // �볡ʱ����Ч��
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

    float _deep_confuse_dmg_extra;
    UInt8 _deep_confuse_last;
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

    UInt32 _bleedFlag;  // ����������Ѫ����
    
    float _bleed1, _bleed2, _bleed3, _self_bleed;
    UInt8 _bleed1_last, _bleed2_last, _bleed3_last, _self_bleed_last;
    UInt16 _immune2;

    float _def_dec;
    UInt8 _def_dec_last;
    UInt8 _def_dec_times;


    UInt32 _aura_bleed;
    float _aura_present;
    UInt8 _aura_present_cd;
    UInt8 _aura_dec_cd, _aura_bleed_last;
    UInt32 _stun_bleed;
    float _stun_present;
    UInt8 _stun_present_cd;
    UInt8 _stun_cd, _stun_bleed_last;
    UInt32 _confuse_bleed;
    float _confuse_present;
    UInt8 _confuse_present_cd;
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
	float _petAttackAdd, _petMagAtkAdd, _petAtkReduce, _petMagAtkReduce;
	UInt8 _petAttackAddCD, _petMagAtkAddCD, _petAtkReduceCD, _petMagAtkReduceCD;
    float _petExAtk;
    bool  _petExAtkEnable;
    UInt16 _petExAtkId;

    float _lastHPLostP;

    // ������ʧHP���ӵ��˺�����
    float _hpAtkAdd, _hpMagAtkAdd; 
    UInt32 _hpAtkAddCount;

    // ������ʧHP���ӵ��˺�����
    float _hpAtkReduce, _hpMagAtkReduce; 
    UInt32 _hpAtkReduceCount;      
    float _hpRecoverCount;

    float _bleedMo;
    UInt8 _bleedMoLast;
    UInt32 _blind_bleed;
    float _blind_present;
    UInt8 _blind_present_cd;
    UInt8 _blind_cd, _blind_bleed_last;
    BattleFighter* _summoner;
    UInt8 _unSummonAura;

    float _bleedLingYan;            // ������Ѫ�˺�
    UInt8 _bleedLingYanLast;        // ���ͳ���ʱ��
    float _bleedLingYanAuraDec;     // ������Ч��������ֵ
    float _bleedLingYanAuraDecProb; // ������Ч��������ֵ��������

    float _bleedFieldGape;               // ����Ч��ÿ�غϳ����˺�
    UInt8 _bleedFieldGapeLast;           // ����Ч������ʱ��
    float _bleedFieldGapeStunProb;       // ����Ч��ʱ����ѣ�εĸ���
    UInt8 _bleedFieldGapeType;           // ����Ч������ 0��1��2 

    float _shieldHP;
    UInt8 _shieldHPLast;

    float _petShieldHP;         // �ɳ��ɢ���ϵĻ���HP

    bool  _petProtect100;
    UInt8 _petProtect100Last;
    const GData::SkillBase* _petProtect100Skill;

    float  _petAtk100;
    UInt8 _petAtk100Last;

    bool  _petMark;

    UInt8 _flawCount;
    float _flawDamageAdd;
    UInt8 _flawLast;

    float _withstandFactor;
    UInt8 _withstandCount;

    bool  _chaosWorld;


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
    float _atkAddSpecial;  // ���ܷ������նԷ��˺��������ӵ��﹥
    Int16 _atkSpecialLast;
    float _magAtkAddSpecial;
    Int16 _magAtkSpecialLast;
    float _atkDecSpecial;  // ���ܷ���:�Լ����Է����յĹ���
    Int16 _atkDecSpecialLast;
    float _magAtkDecSpecial;
    Int16 _magAtkDecSpecialLast;

    // �Լ����Լ���buf��ʱ�򣬲��������ڼ���ʹ�õĹ����м��㣬���Ǽӳɷ��ڼ���ʹ��֮�����Լ�������������̬�ı����ݴ棬�߻�ǿ��Ҫ��ģ������˵������������߿�������ʲô��˼��������
    float _skillUsedChangeAttrValue;
    Int16 _skillUsedChangeAttrLast;
    int   _skillUsedChangeAttr;
    // end

    float _bleedRandom;
    Int16 _bleedRandomLast;
    UInt8 _bleedAttackClass;  // ��Ѫ��ʱ��ǰ��Ҫ���ݹ����ߵ����ͱ�����Ч

    float _bleedBySkill;      // ������ɫ������Ѫ
    Int16 _bleedBySkillLast;
    UInt8 _bleedBySkillClass;  // ��Ѫ��ʱ��ǰ��Ҫ���ݹ����ߵ����ͱ�����Ч

    // ������˫���ܵ��µĶ����������ӡ��������٣�ֻ�������ڱ��Σ�����ÿ�ι�����ϣ�ֱ�������Ҳ����֪ͨǰ��
    float _hitChangeByPeerless;
    float _counterChangeByPeerless;
    
    bool  _bSingleAttackFlag;  // Ⱥ�幥��ʱ��ֻ������һ�˵ı��
    bool  _bMainTargetDead;    // ��Ŀ�걻������Ŀǰֻ��areaΪ0��ʱ�����˱�ǣ��Ժ���Ҫ�������Ӻ��ˣ�
    int   _nCurrentAttackIndex;  // ��ǰ��������������ֻ�����Լ���Ҫ��ʱ��ֵ�����������Ҫ�õ�ʱ�����ڹ���֮ǰ�Լ����ú�

public:
    void fakeDead();
    inline UInt16 getFakeDeadTimes() { return _fakeDeadTimes; }

    inline UInt8& getDeepForgetLast() { return _deep_forget_last; }
    inline float getDeepForgetDmgExtra() { return _deep_forget_dmg_extra; }
    inline void setDeepForgetDmgExtra(float v, UInt8 l) { _deep_forget_dmg_extra = v; _deep_forget_last = l; }
    inline UInt8& getDeepStunLast() { return _deep_stun_last; }
    inline float getDeepStunDmgExtra() { return _deep_stun_dmg_extra; }
    inline void setDeepStunDmgExtra(float v, UInt8 l) { _deep_stun_dmg_extra = v; _deep_stun_last = l; }
    inline UInt8& getDeepConfuseLast() { return _deep_confuse_last; }
    inline float getDeepConfuseDmgExtra() { return _deep_confuse_dmg_extra; }
    inline void setDeepConfuseDmgExtra(float v, UInt8 l) { _deep_confuse_dmg_extra = v; _deep_confuse_last = l; }

//    inline UInt8& getTherapyDecLast() { return _therapy_dec_last; }
//    inline float getTherapyDec() { return _therapy_dec; }
//    inline void setTherapyDec(float value, UInt8 last) { _therapy_dec = value; _therapy_dec_last = last; }

    // ������Ч��
//    inline UInt8& getTherapyAddLast() { return _therapy_add_last; }
//    inline float getTherapyAdd() { return _therapy_add; }
//    inline void setTherapyAdd(float value, UInt8 last) { _therapy_add = value; _therapy_add_last = last; }
    // ����Ч���ӳɣ����������ܸ���
    inline UInt8& getTherapyBuffLast() { return _therapy_buff_last; }
    inline float getTherapyBuff() { return _therapy_buff; }
    inline void setTherapyBuff(float value, UInt8 last) { _therapy_buff = value; _therapy_buff_last = last; }

    inline UInt8& getAuraDecLast() { return _aura_dec_last; }
    inline float getAuraDec() { return _aura_dec; }
    inline void setAuraDec(float value, UInt8 last) { _aura_dec = value; _aura_dec_last = last; }

    inline bool isBleeding() { return _bleedFlag?true:false; }

    inline UInt8& getBleed1Last() { return _bleed1_last; }
    inline float getBleed1() { return _bleed1; }
    inline void setBleed1(float value, UInt8 last) 
    { 
        _bleed1 = value; 
        _bleed1_last = last; 
        if (_bleed1 && _bleed1_last)
            _bleedFlag |= BLEED_TYPE_FLAG_1;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_1;
    }

    inline UInt8& getBleed2Last() { return _bleed2_last; }
    inline float getBleed2() { return _bleed2; }
    inline void setBleed2(float value, UInt8 last)
    { 
        _bleed2 = value; 
        _bleed2_last = last; 
        if (_bleed2 && _bleed2_last)
            _bleedFlag |= BLEED_TYPE_FLAG_2;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_2;
    }
    inline UInt8& getBleed3Last() { return _bleed3_last; }
    inline float getBleed3() { return _bleed3; }
    inline void setBleed3(float value, UInt8 last)
    { 
        _bleed3 = value; 
        _bleed3_last = last; 
        if (_bleed3 && _bleed3_last)
            _bleedFlag |= BLEED_TYPE_FLAG_3;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_3;
    }

    inline UInt8& getSelfBleedLast() { return _self_bleed_last; }
    inline float getSelfBleed() { return _self_bleed; }
    inline void setSelfBleed(float value, UInt8 last)
    { 
        _self_bleed = value; 
        _self_bleed_last = last; 
        if (_self_bleed && _self_bleed_last)
            _bleedFlag |= BLEED_TYPE_FLAG_SELF;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_SELF;
    }

    inline UInt8& getAuraDecCD() { return _aura_dec_cd; }
    inline float getAuraPrecent() { return _aura_present; }
    inline UInt8& getAuraBleedLast() { return _aura_bleed_last; }
    inline float getAuraBleed() { return _aura_bleed; }
    inline void setAuraBleed(float value, UInt8 last, UInt8 cd) 
    { 
        _aura_bleed = value; 
        _aura_bleed_last = last; 
        _aura_dec_cd = cd; 
        if (_aura_bleed && _aura_bleed_last)
            _bleedFlag |= BLEED_TYPE_FLAG_AURA;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_AURA;

    }
    inline void setAuraPresent(float v, UInt8 cd) { _aura_present = v; _aura_present_cd = cd; }
    inline UInt8 getAruaPresentCD() { return _aura_present_cd; }

    inline UInt8& getStunCD() { return _stun_cd; }
    inline float getStunPresent() { return _stun_present; }
    inline UInt8& getStunBleedLast() { return _stun_bleed_last; }
    inline float getStunBleed() { return _stun_bleed; }
    inline void setStunBleed(float value, UInt8 last, UInt8 cd) 
    { 
        _stun_bleed = value; 
        _stun_bleed_last = last; 
        _stun_cd = cd; 
        if (_stun_bleed && _stun_bleed_last)
            _bleedFlag |= BLEED_TYPE_FLAG_STUN;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_STUN;
    }
    inline void setStunPresent(float v, UInt8 cd) { _stun_present = v; _stun_present_cd = cd; }
    inline UInt8 getStunPresentCD() { return _stun_present_cd; }

    inline UInt8& getConfuceCD() { return _confuse_cd; }
    inline float getConfucePresent() { return _confuse_present; }
    inline UInt8& getConfuceBleedLast() { return _confuse_bleed_last; }
    inline float getConfuceBleed() { return _confuse_bleed; }
    inline void setConfuceBleed(float value, UInt8 last, UInt8 cd) 
    { 
        _confuse_bleed = value; 
        _confuse_bleed_last = last; 
        _confuse_cd = cd; 
        if (_confuse_bleed && _confuse_bleed_last)
            _bleedFlag |= BLEED_TYPE_FLAG_CONFUSE;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_CONFUSE;
    }
    inline void setConfusePresent(float v, UInt8 cd) { _confuse_present = v; _confuse_present_cd = cd; }
    inline UInt8 getConfucePresentCD() { return _confuse_present_cd; }


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
    bool makeShieldDamage(UInt32& u);

    BattleFighter* summonSelf(float factor, UInt8 last);
    void clearSkill();
    void setSummonFactor(UInt32 aura, float factor, UInt8 last);
    bool releaseSummon();
    inline void setMoAuraBuf(UInt8 value, UInt8 last) { if(last == 0) return; _moAuraBuf = value; _moAuraBufLast = last; }
    inline void resetMoAuraBuf() { _moAuraBuf = 0; _moAuraBufLast = 0; }
    inline UInt8 getMoAuraBuf() { return _moAuraBuf; }
    bool releaseMoAuraBuf();
    inline void setMoEvade100(UInt8 value, UInt8 last) { if(last == 0) return; _moEvade100 = value; _moEvade100Last = last; }
    inline void resetMoEvade100() { _moEvade100 = 0; _moEvade100Last = 0; }
    bool getMoEvade100();
    bool releaseMoEvade100();
    void setHideBuf(UInt8 value, UInt8 last);
    bool releaseHideBuf();
    inline void setMarkMo(bool value, UInt8 last) { if(last == 0) return; _markMo = value; _markMoLast= last; }
    inline void resetMarkMo() { _markMo = 0; _markMoLast = 0; }
    inline bool isMarkMo() { return _markMo; }
    bool releaseMarkMo();
    inline void setDeepBlind(float value, UInt8 last) { if(last == 0) return; _deep_blind_dmg_extra = value; _deep_blind_last = last; }
    inline void resetDeepBlind() { _deep_blind_dmg_extra = 0; _deep_blind_last = 0; }
    inline float getDeepBlindDmgExtra() { return _deep_blind_dmg_extra; }
    bool releaseDeepBlind();
    inline void setBlind(float value, UInt8 last) { if(last == 0) return; _blind = value; _blind_last = last; }
    inline void resetBlind() { _blind = 0; _blind_last = 0; }
    inline float getBlind() { return _blind; }
    bool releaseBlind();

    inline void setMoAttackAdd(float value, UInt8 last) { if(last == 0) return; _moAttackAdd = value; _moAttackAddCD = last; }
    inline void setMoMagAtkAdd(float value, UInt8 last) { if(last == 0) return; _moMagAtkAdd = value; _moMagAtkAddCD = last; }
    inline void setMoAtkReduce(float value, UInt8 last) { if(last == 0) return; _moAtkReduce = value; _moAtkReduceCD = last; }
    inline void setMoMagAtkReduce(float value, UInt8 last) { if(last == 0) return; _moMagAtkReduce = value; _moMagAtkReduceCD = last; }

    inline void resetMoAttackAdd() { _moAttackAdd = 0; _moAttackAddCD = 0; }
    inline void resetMoMagAtkAdd() { _moMagAtkAdd = 0; _moMagAtkAddCD = 0; }
    inline void resetMoAtkReduce() { _moAtkReduce = 0; _moAtkReduceCD = 0; }
    inline void resetMoMagAtkReduce() { _moMagAtkReduce = 0; _moMagAtkReduceCD = 0; }

    inline float getMoAttackAdd() { return _moAttackAdd; }
    inline float getMoMagAtkAdd() { return _moMagAtkAdd; }
    inline float getMoAtkReduce() { return _moAtkReduce; }
    inline float getMoMagAtkReduce() { return _moMagAtkReduce; }

    bool releaseMoAttackAdd();
    bool releaseMoMagAtkAdd();
    bool releaseMoAtkReduce();
    bool releaseMoMagAtkReduce();

    inline void setPetAttackAdd(float value, UInt8 last) { if(last == 0) return; _petAttackAdd = value; _petAttackAddCD = last; }
    inline void setPetMagAtkAdd(float value, UInt8 last) { if(last == 0) return; _petMagAtkAdd = value; _petMagAtkAddCD = last; }
    inline void setPetAtkReduce(float value, UInt8 last) { if(last == 0) return; _petAtkReduce = value; _petAtkReduceCD = last; }
    inline void setPetMagAtkReduce(float value, UInt8 last) { if(last == 0) return; _petMagAtkReduce = value; _petMagAtkReduceCD = last; }

    inline void setHPAtkReduce(float value) { _hpAtkReduce = value;}
    inline void setHPMagAtkReduce(float value) { _hpMagAtkReduce = value;}
    inline void setHPAtkAdd(float value) { _hpAtkAdd = value;}
    inline void setHPMagAtkAdd(float value) { _hpMagAtkAdd = value;}

    bool updateHPPAttackAdd(float addP, float hpLostp, float maxCount);
    bool updateHPPAttackReduce(float reduceP, float hpLostp, float maxCount);
    UInt32  updateHPPRecover(float recoverP, float hpLostp, float maxCount);
    UInt32  updateHPPRecover2Fake(float recoverP, float hpLostp, float maxCount);
    void    updateLastHPLostP();

    inline void resetPetAttackAdd() { _petAttackAdd = 0; _petAttackAddCD = 0; }
    inline void resetPetMagAtkAdd() { _petMagAtkAdd = 0; _petMagAtkAddCD = 0; }
    inline void resetPetAtkReduce() { _petAtkReduce = 0; _petAtkReduceCD = 0; }
    inline void resetPetMagAtkReduce() { _petMagAtkReduce = 0; _petMagAtkReduceCD = 0; }

    inline void resetHPAtkReduce() { _hpAtkReduce = 0;}
    inline void resetHPMagAtkReduce() { _hpMagAtkReduce = 0;}
    inline void resetHPAtkAdd() { _hpAtkAdd = 0;}
    inline void resetHPMagAtkAdd() { _hpMagAtkAdd = 0;}

    inline float getPetAttackAdd() { return _petAttackAdd; }
    inline float getPetMagAtkAdd() { return _petMagAtkAdd; }
    inline float getPetAtkReduce() { return _petAtkReduce; }
    inline float getPetMagAtkReduce() { return _petMagAtkReduce; }

    inline float getHPAtkReduce() { return _hpAtkReduce; }
    inline float getHPMagAtkReduce() { return _hpMagAtkReduce; }
    inline float getHPAtkAdd() { return _hpAtkAdd; }
    inline float getHPMagAtkAdd() { return _hpMagAtkAdd; }

    inline void setPetExAtk(float v, UInt16 skillId) { _petExAtk = v; _petExAtkId = skillId;}
    inline float getPetExAtk() { return _petExAtk; }
    inline void setPetExAtkEnable(bool v) { _petExAtkEnable = v;}
    inline bool getPetExAtkEnable() { return _petExAtkEnable; }
    inline UInt16 getPetExAtkId() { return _petExAtkId; }

    bool releasePetAttackAdd();
    bool releasePetMagAtkAdd();
    bool releasePetAtkReduce();
    bool releasePetMagAtkReduce();

    inline float getBleedMo() { return _bleedMo; }
    inline UInt16 getBleedMoLast() { return _bleedMoLast; }
    inline void setBleedMo(float value, UInt8 last) 
    { 
        if(last == 0) 
            return; 
        _bleedMo = value; 
        _bleedMoLast = last; 
        if (_bleedMo && _bleedMoLast)
            _bleedFlag |= BLEED_TYPE_FLAG_MO;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_MO;
    }
    inline void resetBleedMo() { _bleedMo = 0; _bleedMoLast = 0; }
    bool releaseBleedMo();

    //XXX
    inline UInt8& getBlindCD() { return _blind_cd; }
    inline float getBlindPresent() { return _blind_present; }
    inline UInt8& getBlindBleedLast() { return _blind_bleed_last; }
    inline float getBlindBleed() { return _blind_bleed; }
    inline void setBlindBleed(float value, UInt8 last, UInt8 cd) 
    { 
        _blind_bleed = value; 
        _blind_bleed_last = last; 
        _blind_cd = cd; 
        if (_blind_bleed && _blind_bleed_last)
            _bleedFlag |= BLEED_TYPE_FLAG_BLIND;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_BLIND;
    }
    inline void setBlindPresent(float v, UInt8 cd) { _blind_present = v; _blind_present_cd = cd; }
    inline UInt8 getBlindPresentCD() { return _blind_present_cd; }

    inline UInt8& getBleedLingYanLast() { return _bleedLingYanLast; }
    inline float getBleedLingYan() { return _bleedLingYan; }
    inline void setBleedLingYan(float value, UInt8 last, float auraDec, float auraDecProb) 
    { 
        _bleedLingYan = value;
        _bleedLingYanLast = last;
        _bleedLingYanAuraDec = auraDec;
        _bleedLingYanAuraDecProb = auraDecProb;
        if (_bleedLingYan && _bleedLingYanLast)
            _bleedFlag |= BLEED_TYPE_FLAG_LINGYAN;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_LINGYAN;
    }
    inline float getBleedLingYanAuraDescProb() { return _bleedLingYanAuraDecProb; }
    inline float getBleedLingYanAuraDesc() { return _bleedLingYanAuraDec; }

    inline UInt8& getBleedFieldGapeLast() { return _bleedFieldGapeLast; }
    inline float getBleedFieldGape() { return _bleedFieldGape; }
    inline float getBleedFieldGapeStunProb() { return _bleedFieldGapeStunProb; }
    inline UInt8& getBleedFieldGapeType() { return _bleedFieldGapeType;}
    inline void setBleedFieldGape(float value, UInt8 last, float stunProb, UInt8 type = 0) 
    { 
        _bleedFieldGape = value;
        _bleedFieldGapeLast = last;
        _bleedFieldGapeStunProb = stunProb;
        _bleedFieldGapeType = type;
        if (_bleedFieldGape && _bleedFieldGapeLast)
            _bleedFlag |= BLEED_TYPE_FLAG_FIELD_GAPE;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_FIELD_GAPE;
    }



    void setUnSummonAura(BattleFighter* bf, UInt32 aura) { _summoner = bf, _unSummonAura = aura; }
    bool isSummon() { return _summon; }
    BattleFighter* getSummoner() { return _summoner; }
    UInt8 getUnSummonAura() { return _unSummonAura; }

    inline float& getShieldHPBuf() { return _shieldHP; }
    void setShieldHPBuf(float value, UInt8 last);
    bool releaseShieldHPBuf();

private:
    std::vector<GData::SkillItem> _passiveSkillOnCounter;
    std::vector<GData::SkillItem> _passiveSkillOnCounter100;
    std::vector<GData::SkillItem> _passiveSkillOnAttackBleed100;
    std::vector<GData::SkillItem> _passiveSkillOnAtkDmg;
    std::vector<GData::SkillItem> _passiveSkillOnAtkDmg100;
    std::vector<GData::SkillItem> _passiveSkillOnGetDmg100;
    std::vector<GData::SkillItem> _passiveSkillOnGetDmg;

    std::vector<GData::SkillItem> _passiveSkillOnBeDmg100;
    std::vector<GData::SkillItem> _passiveSkillOnBePHYDmg100;
    std::vector<GData::SkillItem> _passiveSkillOnBeMagDmg100;
    std::vector<GData::SkillItem> _passiveSkillOnHP10P100;

    std::vector<GData::SkillItem> _passiveSkillOnBeDmg;
    std::vector<GData::SkillItem> _passiveSkillOnBePHYDmg;
    std::vector<GData::SkillItem> _passiveSkillOnBeMagDmg;
    std::vector<GData::SkillItem> _passiveSkillOnHP10P;

    float _darkVigor, _dvFactor;
    UInt8 _darkVigorLast;
public:
    inline float getDarkVigor() { return _darkVigor; }
    inline float getDarkVigorFactor() { return (_darkVigorLast == 0 ? 0 : 0.15); }
    void setDarkVigor(float value, UInt8 last);
    void addDarkVigor(float value);
    bool releaseDarkVigor();
    const GData::SkillBase* getPassiveSkillOnCounter100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnAttackBleed100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnCounter(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillOnAttackBleed(bool noPossibleTarget = false);

private:
    std::vector<GData::LBSkillItem> _onSkillCond;
    std::vector<GData::LBSkillItem> _onActionCond1;
    std::vector<GData::LBSkillItem> _onActionCond2;
    std::vector<GData::LBSkillItem> _onDeadCond;
    std::vector<GData::LBSkillItem> _onBleedCond;
    std::vector<GData::LBSkillItem> _onStateCond;
    float _hpShieldSelf;
    UInt8 _hpShieldSelf_last;
public:
    GData::LBSkillItem* getSkillCondItem(UInt16 skillid);
    GData::LBSkillItem* getActionCondItem1();
    GData::LBSkillItem* getActionCondItem2();
    GData::LBSkillItem* getDeadCondItem();
    GData::LBSkillItem* getBleedCondItem();
    GData::LBSkillItem* getStateCondItem(UInt16 state);
    Int32 getSkillGradeExtraValue(UInt16 skillId);
    void releaseLBSkillCD();

    float& getHpShieldSelf() { return _hpShieldSelf; }
    void setHpShieldSelf(float v, UInt8 l) { _hpShieldSelf = v; _hpShieldSelf_last = l; }
    void addHpShieldSelf(float v, UInt8 l) { _hpShieldSelf += v; _hpShieldSelf_last = l; }
    bool releaseHpSieldSelf();
    const GData::SkillBase* _xiangMoChanZhangSkill;
    const GData::SkillBase* getXiangMoChanZhangSkill(){ return _xiangMoChanZhangSkill; }
    const void setXiangMoChanZhangSkill(GData::SkillBase* skill){ _xiangMoChanZhangSkill = skill; }

    const GData::SkillBase* _biLanTianYiSkill;
    const GData::SkillBase* getBiLanTianYiSkill(){ return _biLanTianYiSkill; }
    const void setBiLanTianYiSkill(GData::SkillBase* skill){ _biLanTianYiSkill = skill; }

    inline float& getPetShieldHP() { return _petShieldHP; }
    inline void setPetShieldHP(float value) { _petShieldHP = value; }

    inline bool getPetProtect100() { return _petProtect100; }
    inline void setPetProtect100(bool v, UInt8 l, const GData::SkillBase* skill) { _petProtect100 = v; _petProtect100Last = l; _petProtect100Skill = skill; }
    const GData::SkillBase* getPetProtect100Skill() { return _petProtect100Skill; }

    inline void setPetMark(bool v) { _petMark = v; }
    inline bool isPetMark() { return _petMark; }

    inline float getPetAtk100() { return _petAtk100; }
    inline bool getPetAtk100Last() { return _petAtk100Last;}
    inline void setPetAtk100(float v, UInt8 l) { _petAtk100 = v; _petAtk100Last = l; }
    bool releasePetAtk100();

    UInt8 setFlaw(float damageAdd, UInt8 last, UInt8 count) 
    { 
        _flawDamageAdd = damageAdd; _flawLast = last; _flawCount = count;
        return _flawCount;
    }
    UInt8 addFlaw(float damageAdd, UInt8 last, UInt8 count = 1)
    {
        _flawDamageAdd = damageAdd; _flawLast = last; _flawCount += count;
        if (_flawCount > 3) _flawCount = 3;
        return _flawCount;
    }

    UInt8 getFlawCount() { return _flawCount; }
    float getFlawDamageAdd() { return _flawDamageAdd; }
    UInt8& getFlawLast()     { return _flawLast; }

    void  setWithstand(float withstand) { _withstandFactor = withstand; }
    void  addWithstandCount()   
    { 
        ++_withstandCount; 
        if (_withstandCount > 3)
            _withstandCount = 3; 
    }
    float getWithstandFactor() { return _withstandFactor; }
    UInt8 getWithstandCount() { return _withstandCount; }
    void  resetWithstandCount() { _withstandCount = 0; }

    bool  getChaosWorld () { return _chaosWorld; }
    void  setChaosWorld(bool chaosWorld) { _chaosWorld = chaosWorld; }

public:
    float _counter_spirit_atk_add;
    float _counter_spirit_magatk_add;
    float _counter_spirit_def_add;
    float _counter_spirit_magdef_add;
    UInt8 _counter_spirit_times;
    UInt8 _counter_spirit_last;
    float _counter_spirit_efv;
    std::vector<float> _counter_spirit_factor;
    UInt16 _counter_spirit_skillid;
    UInt8 _counter_spirit_skill_cd;

    inline UInt8 getCounterSpiritTimes() { return _counter_spirit_times; }
    void addCounterSpiritBuf(float atk, float magatk, float def, float magdef, UInt8 last);
    void setCounterSpiritSkill(UInt16 skillid, float efv, const std::vector<float>& factor);
    void clearCounterSpiritSkill();
    float getCounterSpiritAtk();
    UInt16 getCounterSpiritSkillId() { return _counter_spirit_skillid; }
    std::vector<float>& getCounterSpiritFactor() { return _counter_spirit_factor; }
    bool releaseCounterSpirit();

private:
    float _pet_coatk;
    void setPetCoAtk(float v) { _pet_coatk = v; }
    void releasePetCoAtk() { if(_pet_coatk > 0.001f) _pet_coatk = 0; }

    float _fire_defend;
    UInt8 _fire_defend_last;
    float _fire_fake_dead_rate;
    UInt8 _fire_fake_dead_rate_last;
    void setFireDefend(float v, UInt8 l) { _fire_defend = v; _fire_defend_last = l; }
    bool releaseFireDefend();
    void setFireFakeDead(float v, UInt8 l) { _fire_fake_dead_rate = v; _fire_fake_dead_rate_last = l; }
    bool doFireFakeDead();
    bool releaseFireFakeDead();

    float _sneak_atk;
    UInt8 _sneak_atk_status;
    UInt8 _sneak_atk_last;
    float _sneak_atk_recover_rate;
    float getSneakAtk() { return _sneak_atk; }
    UInt8 getSneakStatus() { return _sneak_atk_status; }
    void nextSneakStatus();
    void setSneakAtk(float v, UInt8 s, UInt8 l) { _sneak_atk = v, _sneak_atk_status = s, _sneak_atk_last = l; }
    bool releaseSneakAtk();
    void setRecoverSnakeAtk(float v) { _sneak_atk_recover_rate = v; }
    bool recoverSneakAtk();

    BattleFighter* _selfSummon;
    inline void setSelfSummon(BattleFighter* bf) { _selfSummon = bf; }
    inline BattleFighter* getSelfSummon() { return _selfSummon; }

    float _dec_wave_dmg;
    float getDecWaveDmg() { return _dec_wave_dmg; }
    void setDecWaveDmg(float v) { _dec_wave_dmg = v; }

    UInt8 _lingqu_last;
    UInt8 _lingqu_times;
    bool _lingqu;
    inline bool getLingQu() { return _lingqu; }
    inline void setLingQu(bool v, UInt8 l) { _hp = 1; _lingqu = v; _lingqu_last = l; _lingqu_times = 9; }
    inline bool isLingQu() { return _lingqu_last != 0; }
    inline UInt8& getLingQuTimes() { return _lingqu_times; }
    bool releaseLingQu();

    UInt8 _soulout_last;
    bool _soulout;
    inline bool isSoulOut() { return _soulout; }
    inline void setSoulOut(float v, UInt8 l)
    {
        _hp = 1;
        _attack = _attack * v;
        _magatk = _magatk * v;

        _attackAdd = 0;
        _attackAdd2 = 0;
        _atkAddSpecial = 0;
        _atkDecSpecial = 0;
        _moAttackAdd = 0;
        _petAttackAdd = 0;
        _petExAtk = 0;
        _counter_spirit_atk_add = 0;
        _pet_coatk = 0;

        _magAtkAdd = 0;
        _magAtkAdd2 = 0;
        _magAtkAddSpecial = 0;
        _magAtkDecSpecial = 0;
        _moMagAtkAdd = 0;
        _petMagAtkAdd = 0;
        _counter_spirit_magatk_add = 0;

        _soulout = true;
        _soulout_last = l;

        _hideBufLast = 0;
        _hideBuf = false;
        _shieldHP = 0;
        _shieldHPLast = 0;

        setHide(true);
    }
    bool releaseSoulOut();

    float _lingshi_bleed;
    UInt8 _lingshi_bleed_last;
    float getLingShiBleed() { return _lingshi_bleed; }
    void setLingShiBleed(float v, UInt8 l) 
    { 
        _lingshi_bleed = v; 
        _lingshi_bleed_last = l; 
        if (_lingshi_bleed && _lingshi_bleed_last)
            _bleedFlag |= BLEED_TYPE_FLAG_LINGSHI;
        else
            _bleedFlag &= ~BLEED_TYPE_FLAG_LINGSHI;
    }
    bool releaseLingShiBleed();

    float _lingyou_atk;
    float _lingyou_magatk;
    float _lingyou_def;
    float _lingyou_magdef;
    float getLingYouAtk() { return _lingyou_atk; }
    float getLingYouMagAtk() { return _lingyou_magatk; }
    float getLingYouDef() { return _lingyou_def; }
    float getLingYouMagDef() { return _lingyou_magdef; }
    void setLingYouAtk(float v) { _lingyou_atk = v; }
    void setLingYouMagAtk(float v) { _lingyou_magatk = v; }
    void setLingYouDef(float v) { _lingyou_def = v; }
    void setLingYouMagDef(float v) { _lingyou_magdef = v; }

    bool _lingHpShield;
    bool getLingHpShield() { return _lingHpShield; }
    void setLingHpShield(bool v) { _lingHpShield = v; }

    float _criticaldmgreduce;
    float getCriticalDmgReduce() { return _criticaldmgreduce; }
    void setCriticalDmgReduce(float v) { _criticaldmgreduce = v; }

    UInt8 _abnormalTypeCnt;
    bool addAbnormalTypeCnt()
    {
        if(_abnormalTypeCnt < 5)
        {
            ++_abnormalTypeCnt;
            return true;
        }
        return false;
    }
    UInt8 getAbnormalTypeCnt() { return _abnormalTypeCnt; }
    void useAbnormalTypeCnt() { if(_abnormalTypeCnt >= 3) _abnormalTypeCnt -= 3; }
    const GData::SkillBase* _abnormalTypeSkill;
    void setAbnormalTypeSkill(const GData::SkillBase* skill) { _abnormalTypeSkill = skill; }
    const GData::SkillBase* getAbnormalTypeSkill() { return _abnormalTypeSkill; }

    UInt8 _bleedTypeCnt;
    bool addBleedTypeCnt()
    {
        if(_bleedTypeCnt < 5)
        {
            ++_bleedTypeCnt;
            return true;
        }
        return false;
    }
    UInt8 getBleedTypeCnt() { return _bleedTypeCnt; }
    void useBleedTypeCnt() { if(_bleedTypeCnt >= 3) _bleedTypeCnt -= 3; }
    const GData::SkillBase* _bleedTypeSkill;
    void setBleedTypeSkill(const GData::SkillBase* skill) { _bleedTypeSkill = skill; }
    const GData::SkillBase* getBleedTypeSkill() { return _bleedTypeSkill; }

    float auralRate;
    float auraValue;
    UInt8 auralLast;
    BattleFighter* launcher;
    inline void setBuddhaLight(float v, UInt8 l) { auralRate = v; auralLast = l; }
    inline UInt8 getBuddhaLightLast() { return auralLast; }
    inline float getBuddhaLightRate() { return auralRate; }
    inline void setBuddhaLightValue(float value) { auraValue = value; }
    inline float getBuddhaLightValue() { return auraValue; }
    inline void setBuddhaLightLauncher(BattleFighter* f) { launcher = f; }
    inline BattleFighter* getBuddhaLightLauncher() { return launcher; }

    UInt8 _evadeCnt;
    UInt8 getEvadeCnt() { return _evadeCnt; }
    void addEvadeCnt(UInt8 cnt) { if(_evadeCnt + cnt <= 5) _evadeCnt += cnt; else _evadeCnt = 5; }
    void minusEvadeCnt(UInt8 cnt) { if(_evadeCnt >= cnt) _evadeCnt -= cnt; else _evadeCnt = 0; }

    UInt8 _peerlessDisableLast;
    inline void setPeerLessDisableLast(UInt8 l) { _peerlessDisableLast = l; }
    inline UInt8 getPeerLessDisableLast() { return _peerlessDisableLast; }

    UInt8 _peerlessDisableSSLast[25];
    inline void setPeerLessDisableSSLast(UInt8 i, UInt8 l) { if(i < 25) _peerlessDisableSSLast[i] = l; }
    inline UInt8 getPeerLessDisableSSLast(UInt8 i) { if(i < 25) return _peerlessDisableSSLast[i]; else return 0; }
    UInt32 _peerlessDisableSSHP[25];
    inline void setPeerLessDisableSSHP(UInt8 i, UInt32 hp) { if(i < 25) _peerlessDisableSSHP[i] = hp; }
    inline UInt32 getPeerLessDisableSSHP(UInt8 i) { if(i < 25) return _peerlessDisableSSHP[i]; else return 0; }

    UInt8 _soulProtectLast;
    inline void setSoulProtectLast(UInt8 l) { _soulProtectLast = l; }
    inline UInt8 getSoulProtectLast() { return _soulProtectLast; }
    UInt8 _soulProtectCount;
    inline void setSoulProtectCount(UInt8 count) { _soulProtectCount = count; }
    inline UInt8 getSoulProtectCount() { return _soulProtectCount; }

    UInt8 _xinMoCount;
    inline void setXinMoCount(UInt8 l) { _xinMoCount = l; }
    inline UInt8 getXinMoCount() { return _xinMoCount; }

    std::vector<GData::SkillItem> _passiveSkillDeadFake100;
    std::vector<GData::SkillItem> _passiveSkillDeadFake;
    std::vector<GData::SkillItem> _passiveSkillSoulProtect;
    std::vector<GData::SkillItem> _passiveSkillAbnormalTypeDmg100;
    std::vector<GData::SkillItem> _passiveSkillBleedTypeDmg100;
    std::vector<GData::SkillItem> _passiveSkillBleedTypeDmg;
    std::vector<GData::SkillItem> _passiveSkillXMCZ100;
    std::vector<GData::SkillItem> _passiveSkillBLTY100;
    std::vector<GData::SkillItem> _passiveSkillViolent100;
    std::vector<GData::SkillItem> _passiveSkillRevival100;
    std::vector<GData::SkillItem> _passiveSkillLingshi100;

    const GData::SkillBase* getPassiveSkillDeadFake100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillDeadFake(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillAbnormalTypeDmg100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillBleedTypeDmg100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillBleedTypeDmg(bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillXMCZ100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillBLTY100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillViolent100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillRevival100(size_t& idx, bool noPossibleTarget = false);
    const GData::SkillBase* getPassiveSkillLingshi100(size_t& idx, bool noPossibleTarget = false);

private:
    float _2ndRateCoAtk;
    const GData::SkillBase* _2ndCoAtkSkill;

    const GData::SkillBase* get2ndCoAtkSkill();
    void set2ndCoAtkSkill(float rate, const GData::SkillBase* pskill);


    float _2ndRateProtect;
    const GData::SkillBase* _2ndProtectSkill;

    const GData::SkillBase* get2ndProtectSkill();
    void set2ndProtectSkill(float rate, const GData::SkillBase* pskill);

    float _dmg_deep;
    UInt8 _dmg_deep_last;

    bool isDmgDeep() { return _dmg_deep_last > 0; }
    float getDmgDeep() { return _dmg_deep; }
    void setDmgDeep(float value, UInt8 last) { _dmg_deep = value; _dmg_deep_last = last; }
    bool releaseDmgDeep();

    float _dmg_ningshi;
    UInt8 _dmg_ningshi_last;
    BattleFighter* _ningshizhe;
    std::vector<BattleFighter*> _beiningshizhe;

    void pushBeiNingShiZhe(BattleFighter* bo);
    void popBeiNingShiZhe(BattleFighter* bo);

    bool isDmgNingShi() { return _dmg_ningshi_last > 0; }
    float getDmgNingShi() { return _dmg_ningshi; }
    bool setDmgNingShi(BattleFighter* bf, float value, UInt8 last);
    bool releaseDmgNingShi();
    bool clearDmgNingShi();
    std::vector<BattleFighter*> getBeiNingShiZhe() { return _beiningshizhe; }

    UInt8 _ruRedCarpetLast;
    void setRuRedCarpet(UInt8 last) { _ruRedCarpetLast = last; }
    UInt8 getRuRedCarpet() { return _ruRedCarpetLast; }

    UInt8 _shiFlowerLast;
    UInt8 _shiFlowerAura;
    void setShiFlower(UInt8 last, UInt8 aura) { _shiFlowerLast = last; _shiFlowerAura = aura; }
    UInt8 getShiFlower() { return _shiFlowerLast; }
    UInt8 getShiFlowerAura() { return _shiFlowerAura; }

    UInt8 _daoRoseLast;
    void setDaoRose(UInt8 last) { _daoRoseLast = last; }
    UInt8 getDaoRose() { return _daoRoseLast; }

    UInt8 _moKnotLast;
    void setMoKnot(UInt8 last) { _moKnotLast = last; }
    UInt8 getMoKnot() { return _moKnotLast; }

    UInt16 _bActCnt;
    UInt16 getActCnt() { return _bActCnt; }
    void addActCnt(UInt16 count) { _bActCnt += count; }
    UInt16 _immune3;
    void setImmune3(UInt16 v) { _immune3 = v; }
    UInt16 getImmune3() { return _immune3; }

    UInt16 _revivalCnt;
    void setRevivalCnt(UInt16 count) { _revivalCnt = count; }
    UInt16 getRevivalCnt() { return _revivalCnt;}

    UInt8 _prudentLast;
    void setPrudentLast(UInt8 last) { _prudentLast = last; }
    UInt8 getPrudentLast() { return _prudentLast; }
    float _prudentHitrate;
    void setPrudentHitrate(float value) { _prudentHitrate = value; }
    float getPrudentHitrate() { return _prudentHitrate;}
    UInt8 _prudentHitrateLastOtherside;
    void setPrudentHitrateLastOtherside(UInt8 last) { _prudentHitrateLastOtherside = last; }
    UInt8 getPrudentHitrateLastOtherside() { return _prudentHitrateLastOtherside; }

    UInt16 _silkwormCnt;
    void setSilkwormCnt(UInt16 count) { _silkwormCnt = count; }
    UInt16 getSilkwormCnt() { return _silkwormCnt;}
    void addSilkwormCnt(UInt16 count) { _silkwormCnt += count; }

    UInt8 _yehuoLevel;
    void setYehuoLevel(UInt8 level) { _yehuoLevel = level; }
    UInt8 getYehuoLevel() { return _yehuoLevel; }
    float _yehuo_ss_dmgRate;
    void setYehuoSSDmgRate(float f) { _yehuo_ss_dmgRate = f; }
    float getYehuoSSDmgRate() { return _yehuo_ss_dmgRate; }
    float _yehuo_ss_upRate;
    void setYehuoSSUpRate(float f) { _yehuo_ss_upRate = f; }
    float getYehuoSSUpRate() { return _yehuo_ss_upRate; }

    std::map<UInt16, Int32> _sg_v;

    UInt8 _jiuziDmgCnt;
    void setJiuziDmgCnt(UInt8 count) { _jiuziDmgCnt = count; }
    UInt8 getJiuziDmgCnt() { return _jiuziDmgCnt; }

public:
	enum StatusFlag
	{
		Enh1 = 0x03,
		Enh2 = 0x0C,
		Enh3 = 0x10,
        Enh4 = 0x20,
        Enh5 = 0x40,
        Enh6 = 0x80000000,
        Enh7 = 0x100,
        Enh8 = 0x200,
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

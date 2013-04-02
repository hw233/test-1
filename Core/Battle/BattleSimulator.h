#ifndef _BATTLESIMULATOR_H_
#define _BATTLESIMULATOR_H_

#include "BattleField.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "GData/Area.h"
#include "GData/SkillStrengthen.h"

namespace Script
{
class BattleFormula;
}

namespace Battle
{

    enum BattleScene
    {
        // 副本
        BS_COPY1 = 0xFF01,
        BS_COPY2 = 0xFF02,
        BS_COPY3 = 0xFF03,
        BS_COPY4 = 0xFF04,
        BS_COPY5 = 0xFF05,
        BS_COPY6 = 0xFF06,

        // 阵图
        BS_FRONTMAP1 = 0xFE01,

        // 斗剑场
        BS_ATHLETICS1 = 0xFD01,
        // 新阵营(蜀山论剑)
        BS_NEWCOUNTRYBATTLE = 0xFD02,
        // 世界BOSS
        BS_WBOSS = 0xFC01
    };


class BattleSimulator:
	public BattleField
{
public:
	BattleSimulator(UInt32, GObject::Player *, const std::string&, UInt8, bool report = true, UInt32 fake_turns = 500);
	BattleSimulator(UInt32, GObject::Player *, GObject::Player *, bool report = true, UInt32 fake_turns = 500);
	inline int getId() {return _id;}
	inline int getTurns() {return _turns;}
	void start(UInt8 prevWin = 0xFF, bool checkEnh = true);
	inline Stream& getPacket() {return _packet;}
	inline Script::BattleFormula * getFormula() { return _formula; }
	inline int getWinner() { return _winner; } // returns 1 for attacker winning, 2 for defender winning, or 0 for that game's still in progress
	bool applyFighterHP(UInt8, GObject::Player *, bool = true, UInt32 = 0);
	void getFighterHP(UInt8, GObject::Fighter **, UInt8 *, UInt32 * hp, UInt32 = 0);
	inline void setPortrait(UInt8 side, UInt16 por) { _portrait[side] = por; }
	BattleFighter * newFighter(UInt8 side, UInt8 pos, GObject::Fighter *);
    BattleFighter * newPet(UInt8 side, UInt8 pos, GObject::Fighter *);
	inline void setFormula(Script::BattleFormula * formula) { _formula = formula; }

    void putTeams(const std::string& name, UInt8 level, UInt16 portrait, UInt8 side);
    void switchPlayer(GObject::Player* player, UInt8 side);
    void switchPlayer(const std::string& name, UInt8 level);
    UInt32 clearLastBattle(UInt8 side, bool isLast);

private:
	struct FighterStatus
	{
		BattleFighter * bfgt;
		UInt32 action;
		//UInt32 poisonAction;
		UInt32 poisonDamage;

		inline FighterStatus() {}
		inline FighterStatus(BattleFighter * f, UInt32 pa = 0, UInt32 pd = 0)
		{
			bfgt = f;
			//poisonAction = pa;
			poisonDamage = pd;
			if(pa)
				action = pa;
			else
				action = bfgt->getAction();
		}

		inline bool operator ==(const FighterStatus& other) const
		{
			return bfgt == other.bfgt;
		}

		inline void addAction( UInt32 p )
		{
			if(action <= p)
				action = 0;
			else
				action -= p;
		}

		inline void resetAction()
		{
		//	if(poisonAction)
		//		action = poisonAction;
		//	else
				action = bfgt->getAction();
		}
	};

	struct AttackPoint
	{
		UInt8 pos;
		UInt8 type;
		float factor;
	};

    struct AtkFactor
    {
        BattleFighter* bf;
        float factor;
    };
    typedef std::vector<AtkFactor> AtkList;

	struct DefStatus
	{
        DefStatus()
        {
            pos = 0;
            damType = 0;
            damType2 = 0;
            damage = 0;
            damageType = 0;
            leftHP = 0;
            rhp = 0;
            rLeftHP = 0;
            counterDmg = 0;
            counterLeft = 0;
        }

		UInt8 pos;
		UInt8 damType;
		UInt8 damType2;
        UInt8 damageType;  // 如果是伤害技能，则用户客户端表示其伤害类型
		UInt32 damage;
		UInt32 leftHP;
        UInt32 rhp;
        UInt32 rLeftHP;
		UInt32 counterDmg;
		UInt32 counterLeft;
	};
	struct StatusChange
	{
        StatusChange()
        {
            pos = 0;
            statusId = 0;
            type = 0;
            data = 0;
        }

		UInt8 pos;
		UInt16 statusId;
		UInt8 type;
		UInt32 data;
	};
    struct AttackAct
    {
		BattleFighter* bf;
        UInt8 target_side;
        UInt8 target_pos;
        const GData::SkillBase* skill;
        UInt32 param;
        UInt32 param1; // 参数不够用，加一个
    };

    enum StatusType
    {
        MIN_STATUS = 0,
        e_stAura = 0,
        e_stAtk,
        e_stDef,
        e_stMagAtk,
        e_stMagDef,
        e_stTough,
        e_stAction,
        e_stEvade,
        e_stCritical,
        e_stPierce,
        e_stCounter,
        e_stMagRes,
        e_stCriticalDmg,
        e_stHitRate,
        e_stAtkReduce,
        e_stMagAtkReduce,
        e_stAtkInit = 51,
        e_stDefInit,
        e_stMagAtkInit,
        e_stMagDefInit,
        e_stToughInit,
        e_stActionInit,
        e_stEvadeInit,
        e_stCriticalInit,
        e_stPierceInit,
        e_stCounterInit,
        e_stMagResInit,
        e_stCriticalDmgInit,
        e_stHitRateInit,
        e_stAtkReduceInit,
        e_stMagAtkReduceInit,
        e_stMaxReiastu = 100,
        e_stReiastu = 101,
        MAX_STATUS,
    };

    enum StateType
    {
        e_damNormal = 0,
        e_damEvade = 2,
        e_damHpAdd = 3,
        e_damRevival = 4,
        e_damBack = 5,
        e_Poison = 6,
        e_UnPoison = 7,
        e_damAbsorb = 8,
        e_Stun = 9,
        e_UnStun = 10,
        e_Confuse = 11,
        e_UnConfuse = 12,
        e_Forget = 13,
        e_UnForget = 14,
        e_Immune = 15,
        e_damInj = 16,
        e_damOut = 17,
        e_Res = 19,
        e_ResR = 20,
        e_Disperse = 21,
        e_Summon = 22, // 召唤，残影
        e_Weak = 23,
        e_UnWeak = 24,
        e_skill = 25,
        e_deepStun = 26,
        e_unDeepStun = 27,
        e_deepForget = 28,
        e_unDeepForget = 29,
        e_xinmo = 30,
        e_unxinmo = 31,
        e_neishan = 32,
        e_unneishan = 33,
        e_Immune2 = 34,
        e_unImmune2 = 35,
        e_defDec = 36,
        e_undefDec = 37,
        e_Bleed1 = 38,    // 儒,电击
        e_unBleed1 = 39,  // 儒
        e_Bleed2 = 40,    // 释,心剑
        e_unBleed2 = 41,  // 释
        e_Bleed3 = 42,    // 道,剑气
        e_unBleed3 = 43,  // 道
        e_TherapyBuff = 44,   // 加治疗效果
        e_unTherapyBuff = 45,
        e_Bleed4 = 46,    // 燃烧
        e_unBleed4 = 47,  // 
        e_Immune3 = 48,   // 五彩石
        e_unImmune3 = 49, // 

        e_blind = 50, // 至盲
        e_unBlind = 51, // 解除至盲
        e_hide = 52, // 潜行
        e_unHide = 53, // 解除潜行
        e_moAuraBuf = 54, // 龙舞
        e_unMoAuraBuf = 55, // 解除龙舞
        e_unSummon = 56, // 残影消失
        e_deepBlind = 57, // 深度至盲
        e_unDeepBlind = 58, // 解除深度至盲
        e_BleedMo = 59,    // 墨之殇(墨流血)
        e_unBleedMo = 60,  // 解除墨之殇
        e_markMo = 61, // 墨之印记
        e_unMarkMo = 62, // 解除墨之印记
        e_moAuraBuf1 = 63, // 凤舞
        e_damHPShield = 64, // 生命护盾
        e_darkVigor = 65, // 暗劲
        e_unDarkVigor = 66, // 解除暗劲
        e_ling = 67, // 灵
        e_wu = 68, // 悟
        e_xin = 69, // 信
        e_hpShieldSelf = 70, // 护盾
        e_unHpShieldSelf = 71, // 解除护盾
        e_deepConfuse = 72,    // 深度混乱
        e_unDeepConfuse = 73,  // 解除深度混乱
        e_petShield = 74,       // 仙宠的护盾 
        e_unPetShield = 75,     // 解除仙宠的护盾
        e_selfBleed = 76,       // 自己流血 （自焚）
        e_unSelfBleed = 77,     // 自己流血结束 （自焚烧完了）
        e_petAppear = 78,
        e_petMark = 79,         // 神兽印记
        e_unPetMark = 80,         // 神兽印记
        e_petProtect100 = 81,   // 100%援护标志
        e_unPetProtect100 = 82,   // 100%援护标志
        e_petAtk100 = 83,       // 100%合击标志
        e_unPetAtk100 = 84,       // 100%合击标志
        e_counterSpirit = 85,     // 反击士气
        e_unCounterSpirit = 86,   // 解除反击士气

        e_MAX_STATE,
    };

    enum DamageType
    {
        e_damageNone    = 0,
        e_damagePhysic  = 1,    // 物理伤害
        e_damageMagic   = 2,    // 法术伤害
        e_damageTrue    = 3,    // 真实伤害
        e_damagePoison  = 4,    // 毒伤害
    };

    enum SpecialStatus
    {
        MIN_SPECIAL_STATUS = 0,
        e_ss_Atk,
        e_ss_DecAtk,
        e_ss_MagAtk,
        e_ss_DecMagAtk,
        MAX_SPECIAL_STATUS,
    };

private:
    int getSideStartPos(int side);
    int getSidePos(BattleObject* bf);
	int findFirstAttacker();
	UInt32 doAttack(int);
    UInt32 FightersEnter(UInt8 prevWin = 0xFF);
    UInt32 doSkillAttackAftEnter(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt);
    void reQueueFighterStatus(BattleFighter* bf);
	void insertFighterStatus(BattleFighter* bf);
	void insertFighterStatus2Current(BattleFighter* bf);
	void removeFighterStatus(BattleFighter* bf);
	UInt32 attackOnce(BattleFighter * bf, bool& first, bool& cs, bool& pr, const GData::SkillBase* skill, BattleObject * bo, float factor, int counter_deny = -1, AttackPoint * counter_deny_list = NULL, std::vector<AttackAct>* atkAct = NULL, bool canProtect = false);

    UInt32 doXinmoAttack(BattleFighter * bf, BattleObject* bo);

	float testRescue(BattleFighter *& bf, int counter_deny, AttackPoint * counter_deny_list);
	float testLink(BattleFighter *& bf, UInt16& skillId);
    bool onDead(bool activeFlag, BattleObject * bo);
	int testWinner();
	int testWinner2();
	void appendToPacket(UInt8 from_side, UInt8 from_pos, UInt8 target_pos, UInt8 atk_type, UInt16 add_id, bool cs, bool pr);
	UInt32 tryPreUseSkill(BattleFighter * bf, BattleObject * target_object);
	UInt32 tryDelayUseSkill(BattleFighter * bf, BattleObject * target_object);
	void setStatusChange(UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, UInt32 value, bool active);
	void setStatusChange(BattleFighter * bf, UInt8 side, UInt8 pos, int cnt, const GData::SkillBase* skill, UInt8 type, float value, UInt16 last, bool active);
	void setStatusChange2(BattleFighter* bf, UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, UInt16 last, bool active);
	void onDamage(BattleObject * bo, bool active, std::vector<AttackAct>* atkAct = NULL);
	BattleFighter * getRandomFighter(UInt8 side, UInt8 * excepts, size_t exceptCount);
	BattleFighter * getMaxHpFighter(UInt8 side, UInt8 * excepts, size_t exceptCount);
	BattleFighter * getMinHpFighter(UInt8 side, UInt8 * excepts, size_t exceptCount);
	BattleFighter * getMaxAtkFighter(UInt8 side, UInt8 * excepts, size_t exceptCount);
	BattleFighter * getMinAtkFighter(UInt8 side, UInt8 * excepts, size_t exceptCount);
    bool doNormalAttack(BattleFighter* bf, int otherside, int target_pos, std::vector<AttackAct>* atkAct = NULL, float factor = 1, bool canProtect = false);
    bool doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, std::vector<AttackAct>* atkAct = NULL, UInt32 skillParam = 0, UInt8* launchPeerLess = NULL, bool canProtect = false);
    BattleFighter* getTherapyTarget(BattleFighter* bf);
    BattleFighter* getTherapyTarget2(BattleFighter* bf, UInt8 * excepts, size_t exceptCount, bool isFirst = false);
    BattleFighter* getTherapyTarget3(BattleFighter* bf, UInt8 * excepts, size_t exceptCount);

    bool doSkillStatus(bool activeFlag, BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, bool& self, bool ifDecAura);
    bool doSkillStatus2(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt);
    bool doSkillState(BattleFighter* bf, const GData::SkillBase* skill, BattleObject* bo, float factor, UInt8 poisonTimes, UInt32& dmg, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct);
    bool doStateMagRes(BattleFighter* bf, BattleFighter* target_bo, UInt16 state, const GData::SkillBase* skill, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct);
    bool doStateMagRes2(BattleFighter* bf, BattleFighter* target_bo, UInt16 state);

    void getSkillTarget(BattleFighter* bf, const GData::SkillBase* skill, int& target_side, int& target_pos, int& cnt);

    UInt32 doPoisonAttack(BattleFighter* bf, bool cs, const GData::SkillBase* skill, BattleFighter* area_target, float factor, std::vector<AttackAct>* atkAct);

    void doPassiveSkillBeAtk(BattleFighter* bf, BattleFighter* bo, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct, UInt32 dmg);
	void doSkillAtk2(bool activeFlag, std::vector<AttackAct>* atkAct);

    UInt32 releaseCD(BattleFighter* bf);
    void releaseWeak(BattleFighter* bo);

    bool isImmuneDecAura(const GData::SkillBase* skill, int target_side, int target_pos);

    void  InitAttainRecord();
    void  CheckAttain();
    void  SendAttainMsgToPlayer( GObject::Player* player, UInt32 id, UInt32 param);


    typedef void (Battle::BattleSimulator::*setStatusFunc)(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    typedef void (Battle::BattleSimulator::*setStatusFunc2)(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);

    // 不可被抵抗
    void setStatusChange_Aura2(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    // 可被抵抗
    void setStatusChange_Aura(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_Atk(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_Def(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_MagAtk(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_MagDef(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_Tough(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_Action(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_Evade(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_Critical(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_Pierce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_Counter(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_MagRes(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_CriDmg(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_HitR(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_AtkReduce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);
    void setStatusChange_MagAtkReduce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, bool active);

    void setStatusChange2_Def(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_Aura(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_Atk(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_MagAtk(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_MagDef(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_Tough(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_Action(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_Evade(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_Critical(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_Pierce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_Counter(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_MagRes(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_CriDmg(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_HitR(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_AtkReduce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);
    void setStatusChange2_MagAtkReduce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, bool active);


    bool doSkillStrengthenAttack(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);

    typedef bool (Battle::BattleSimulator::*doSkillStrengthenFunc)(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);

    bool doSkillStrengthen_disperse(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_reduce(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_week(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_debuf_defend(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_atkadd(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_absorbAtk(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_addMagicAtk(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_absorbMagAtk(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_bufTherapy(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_DebufAura( BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active );
    bool doSkillStrengthen_AttackFriend(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    bool doSkillStrengthen_BleedBySkill(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);
    void SetSpecialAttrChange(BattleFighter* bf, const GData::SkillBase* skill, SpecialStatus eType, Int16 nLast, float value, bool bOffset);
    void ReduceSpecialAttrLast(BattleFighter* bf, SpecialStatus eType, Int16 nReduce);
    // 元磁神雷使用后调用的接口
    bool AddYuanCiState_SkillStrengthen(BattleFighter* pFighter, BattleFighter* pTarget, const GData::SkillBase* skill, const int nAttackCount);
    // 群攻的时候只中一个目标产生加强
    void ModifySingleAttackValue_SkillStrengthen(BattleFighter* bf,const GData::SkillBase* skill, float& fvalue, bool isAdd);
    // 上状态，只跟作用双方和状态有关的函数接口，主要用在技能符文加强的上状态
    bool AddSkillStrengthenState(BattleFighter* pFighter, BattleFighter* pTarget,const UInt16 nSkillId, const UInt8 nState, const Int16 nLast);
    bool AddStateAfterResist_SkillStrengthen(BattleFighter* pFighter, BattleFighter* pTarget, const GData::SkillBase* skill);
    // 技能符文随机流血
    bool BleedRandom_SkillStrengthen(BattleFighter* bf, BattleFighter* bo, const GData::SkillStrengthenEffect* ef);
    // 获取流血值
    UInt32 GetBleedDmg(BattleFighter* bf, BattleFighter* bo, float nfactor);
    // 上状态被抵抗也流血
    bool AddExtraDamageAfterResist_SkillStrengthen(BattleFighter* pFighter, BattleFighter* pTarget, const GData::SkillBase* skill, int nDamage);
    // 毒被抵抗时上状态
    bool AddStateAfterPoisonResist_SkillStrengthen(BattleFighter* pFighter, BattleFighter* pTarget, const GData::SkillBase* skill, int nfactor);

    bool doDeBufAttack(BattleFighter* bf);

    // 计算普通攻击产生的伤害值
    UInt32 CalcNormalAttackDamage(BattleFighter * bf, BattleObject* bo, StateType& eStateType);
    // 计算治疗因子
    float calcTherapyFactor(BattleFighter* bo);
    float calcTherapyBuff(BattleFighter* bo);  // 这个是加治疗、减治疗和虚弱三种效果相互覆盖的结果，这个治疗状态不可驱散
    //float calcTherapyDebuf(BattleFighter* bo, DefStatus* defList, size_t& defCount);
    //float calcTherapyAddBuff(BattleFighter* bo, DefStatus* defList, size_t& defCount);
    float calcAuraDebuf(BattleFighter* bo);
    void calcAuraAdd(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill, float& bfAuraAdd, float& boAuraAdd);
    float getSkillEffectExtraHideAura(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill);
    void doShieldHPAttack(BattleFighter* bo, UInt32& dmg);
    bool doSkillStrengthen_ShieldHP(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, bool active);



    UInt32 doSkillStrenghtenCriticalPierceDmgB(BattleFighter* bf, const GData::SkillBase* skill, bool cs, bool pr, GData::SkillStrengthenBase*  ss, int target_side, int target_pos);

    void getSkillEffectExtraHitrate(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill, float& hitrate, UInt8& last);
    UInt32 getSkillEffectExtraHitCnt(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill);

    void doSkillEffectExtraAttack(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill);

    typedef void (Battle::BattleSimulator::*doSkillEffectExtraFunc)(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);


    void doSkillEffectExtra_Hide(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_RndFgtBufAura(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_Evade100(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_HideSummon(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_MarkHideWeek(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_HideAttack(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_SelfSideDaoDmgReduce(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_SelfSideRuShiMagAtk(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_SelfSideBufAura(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_HpShield(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_SelfBleed(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_RandomShield(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_SelfAttack(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_RandomTargetAttack(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_MarkPet(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_AtkPetMarkAura(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_AtkPetMarkDmg(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_ProtectPet100(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);
    void doSkillEffectExtra_PetAtk100(BattleFighter* bf, int target_side, int target_pos, const GData::SkillBase* skill, size_t eftIdx);


    void doSkillEffectExtraAbsorb(BattleFighter* bf, UInt32 dmg, const GData::SkillBase* skill);

    void doSkillStrenghtenTherapyAnotherMore(BattleFighter* bf, UInt32 dmg, const GData::SkillBase* skill, GData::SkillStrengthenBase*  ss);

    int getNearOnePos(int side, int pos);
    UInt32 doSkillStrenghtenCriticalDamageAroundOne(BattleFighter* bf, const GData::SkillBase* skill, bool cs, GData::SkillStrengthenBase*  ss, int target_side, int target_pos);
    void getSkillEffectExtraBlind(BattleFighter* bf, BattleFighter* target_bo, const GData::SkillBase* skill, UInt16& effect_state);

    bool doSkillStrengthenDeepBlind(BattleFighter* bf, BattleFighter* bo, GData::SkillStrengthenBase* ss);



    bool doDarkVigorAttack(BattleFighter* bf, float darkVigor);
    void doSkillEffectExtraCounter(BattleFighter* bf, BattleFighter* bo, const GData::SkillBase* skill);
    void doPassiveSkillOnCounter(BattleFighter* bf, BattleFighter* bo);

private:
	int _id, _winner, _turns;
	UInt8 _position;
	bool _report;
	UInt32 _fake_turns;
	//std::vector<FighterStatus> _fgtlist[2];
	std::vector<BattleFighter*> _fgtlist[2];
    Int8 _cur_fgtlist_idx;
	Stream _packet;
	Script::BattleFormula * _formula;
	UInt16 _portrait[2];
	GObject::Player * _player[2];
	std::string _other_name;
	UInt8 _other_level;

	BattleFighter* _activeFgt;
	std::vector<BattleFighter*> _onTherapy;
	std::vector<BattleFighter*> _onSkillDmg;
	std::vector<BattleFighter*> _onOtherDead;
    std::vector<BattleFighter*> _onPetProtect;
    std::vector<BattleFighter*> _onPetAtk;

    UInt8 _cur_round_except[25];
    UInt8 _except_count;

    UInt8 _teams[2];
    std::vector<std::string> _team_name[2];
    std::vector<UInt8> _team_level[2];
    std::vector<UInt16> _team_portrait[2];

	URandom _rnd;

    setStatusFunc statusFuncTable[MAX_STATUS];
    setStatusFunc2 statusFuncTable2[MAX_STATUS];

    doSkillStrengthenFunc skillStrengthenTable[GData::TYPE_MAX];
    doSkillEffectExtraFunc skillEffectExtraTable[GData::e_eft_max];

    //成就记录
    UInt32   _evadeNum[2]; //连续闪避次数
    UInt32  _csNum[2]; //连续暴击次数
    UInt32  _prNum[2]; //连续破击次数
    UInt32  _fjNum[2]; //反击次数

    UInt32  _maxEvade[2]; //最高闪避
    UInt32  _maxCS[2];    //最高连续暴击次数
    UInt32  _maxPR[2];    //最高破击次数
    UInt32  _maxFJ[2];    //最高反击次数
    UInt32  _maxSkillDmg[2]; //最高技能伤害
    UInt32  _maxPeerLessDmg[2]; //最高无双伤害
    UInt32  _maxAura[2]; //最大灵气上限
    float   _maxCSFactor[2]; //暴击比例
    UInt32  _attackRound; //攻击回合
    bool  _firstPLDmg[2]; //第一回合打出无双技能
    UInt8   _getDamageSkillCount[2]; // 每回合受伤触发技能的最大数目

    /*
    bool    _evade3OK[2]; //达成次数
    bool    _evade9ok[2]; //达成次数
    bool    _cs3ok[2];
    bool    _cs9ok[2];
    bool    _pr3ok[2];
    bool    _pr9ok[2];
    bool    _fj3ok[2];
    bool    _fj9ok[2];

    bool    _skillDmg300[2];
    bool    _skillDmg1k[2];
    bool    _skillDmg5k[2];
    bool    _peerlessDmg1k[2];
    bool    _peerlessDmg5k[2];
    bool    _peerlessDmg1w[2];*/

    std::vector<DefStatus> _defList;
    std::vector<StatusChange> _scList;
    void appendDefStatus(StateType type, UInt32 value, BattleFighter* bf, DamageType damageType = e_damageNone);
    void appendStatusChange(StatusType type, UInt32 value, UInt16 skillId, BattleFighter* bf);
    void appendReiatsuChange(int side);
    void appendStatusChangeForReiastu(StatusType type, UInt32 value, UInt16 skillId, UInt8 side);
    void appendMaxReiatsu();

private:
    GData::LBSkillItem* GetActionCondSkillItem2(BattleFighter* bf, BattleFighter* bo);
    GData::LBSkillItem* GetActionCondSkillItem1(BattleFighter* bf, BattleFighter* bo);

    void doItemLingSkillAttack(BattleFighter* bf, BattleFighter* bo);
    void doItemWuSkillAttack(BattleFighter* bf);

    void doItemLing_DefAdd(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last);
    void doItemLing_DefDec(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last);
    void doItemLing_AtkAdd(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last);
    void doItemLing_AtkDec(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last);
    void doItemLingWu_State(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last);
    void doItemLing_Aura(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last);
    void doItemWu_Dmg(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last);
    void doItemWu_HPShield(BattleFighter* bf, BattleFighter* bo, float value, UInt8 last);

    float getItemXin_BleedDec(BattleFighter* bf, GData::LBSkillItem* item);
    bool  getItemXin_MagRes(BattleFighter* bf, UInt16 state);
    bool  getItemXin_BleedOut(BattleFighter* bf, GData::LBSkillItem* item);

    bool getItemLing_cs(BattleFighter* bf, GData::LBSkillItem* item);
    bool getItemLing_pr(BattleFighter* bf, GData::LBSkillItem* item);

    void getAtkList(BattleFighter* bf, const GData::LBSkillBase* lbskill, AtkList& atkList);
    void getAtkList(BattleFighter* bf, const GData::SkillBase* skill, AtkList& atkList, Int8 offset = 0);
    void makeDamage(BattleFighter* bo, UInt32& u);

    bool doAuraPresent(BattleFighter* bf);
    bool doConfusePresent(BattleFighter* bf);
    bool doStunPresent(BattleFighter* bf);
    void appendInitDefStatus(BattleFighter* bf);

private:
    // 和仙宠有关的
    UInt32  tryPetEnter(UInt8 side, UInt8 reiatsuType, bool slience = false); // 仙宠尝试上场（根据增加后的灵压判定是否上场）
    UInt32  doPetEnter(UInt8 side);                     // 仙宠闪亮上场

    bool    tryProtectDamage(BattleFighter* bf, float& phyAtk, float& magatk, float factor);
    bool    do100ProtectDamage(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor);
    bool    doProtectDamage(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor);
    bool    protectDamage(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor);

    bool    tryAttackWithPet(BattleFighter* bf, float& phyAtk, float& magatk, float factor);
    bool    do100AttackWithPet(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor);
    bool    doAttackWithPet(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor);
    bool    attackWithPet(BattleFighter* bf, BattleFighter* pet, float& phyAtk, float& magAtk, float factor);
private:
    int     getPossibleTarget(int, int, BattleFighter* bf = NULL); // return -1 for no found target

    static  bool hasPetMarked(BattleObject* bo);
    static  bool isPet(BattleObject* bo);
    UInt32 upPetObject(UInt8, bool = true);
    UInt32 doSpiritAttack(BattleFighter * bf, BattleFighter* bo, float atk, bool& pr, bool& cs, bool& first);
};

}

#endif // _BATTLESIMULATOR_H_

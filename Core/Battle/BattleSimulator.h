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
	void start(UInt8 prevWin = 0xFF);
	inline Stream& getPacket() {return _packet;}
	inline Script::BattleFormula * getFormula() { return _formula; }
	inline int getWinner() { return _winner; } // returns 1 for attacker winning, 2 for defender winning, or 0 for that game's still in progress
	bool applyFighterHP(UInt8, GObject::Player *, bool = true, UInt32 = 0);
	void getFighterHP(UInt8, GObject::Fighter **, UInt8 *, UInt32 * hp, UInt32 = 0);
	inline void setPortrait(UInt8 side, UInt16 por) { _portrait[side] = por; }
	BattleFighter * newFighter(UInt8 side, UInt8 pos, GObject::Fighter *);
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
	struct DefStatus
	{
        DefStatus()
        {
            pos = 0;
            damType = 0;
            damType2 = 0;
            damage = 0;
            leftHP = 0;
            rhp = 0;
            rLeftHP = 0;
            counterDmg = 0;
            counterLeft = 0;
        }

		UInt8 pos;
		UInt8 damType;
		UInt8 damType2;
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
    };

    enum StatusType
    {
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
        e_Summon = 22,
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
        e_Bleed4 = 46,    // 燃烧
        e_unBleed4 = 47,  // 
        e_Immune3 = 48,   // 五彩石
        e_unImmune3 = 49, // 
    };


private:
	int findFirstAttacker();
	UInt32 doAttack(int);
    UInt32 FightersEnter(UInt8 prevWin = 0xFF);
    UInt32 doSkillAttackAftEnter(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount);
    void reQueueFighterStatus(BattleFighter* bf);
	void insertFighterStatus(BattleFighter* bf);
	void removeFighterStatus(BattleFighter* bf);
	UInt32 attackOnce(BattleFighter * bf, bool& first, bool& cs, bool& pr, const GData::SkillBase* skill, BattleObject * bo, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, int counter_deny = -1, AttackPoint * counter_deny_list = NULL, std::vector<AttackAct>* atkAct = NULL);

    UInt32 doXinmoAttack(BattleFighter * bf, BattleObject* bo, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount);

	float testRescue(BattleFighter *& bf, int counter_deny, AttackPoint * counter_deny_list);
	float testLink(BattleFighter *& bf, UInt16& skillId);
    bool onDead(bool activeFlag, BattleObject * bo, DefStatus* defList, size_t& defCount, StatusChange * scList, size_t& scCount);
	int testWinner();
	int testWinner2();
	void appendToPacket(UInt8 from_side, UInt8 from_pos, UInt8 target_pos, UInt8 atk_type, UInt16 add_id, bool cs, bool pr, DefStatus* defList, size_t defCount, StatusChange * scList, size_t scCount);
	UInt32 tryPreUseSkill(BattleFighter * bf, BattleObject * target_object);
	UInt32 tryDelayUseSkill(BattleFighter * bf, BattleObject * target_object);
	void setStatusChange(UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, UInt32 value, StatusChange * scList, size_t& scCount, bool active);
	void setStatusChange(BattleFighter * bf, UInt8 side, UInt8 pos, int cnt, const GData::SkillBase* skill, UInt8 type, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
	void setStatusChange2(BattleFighter* bf, UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
	void onDamage(BattleObject * bo, DefStatus* defList, size_t& defCount, StatusChange * scList, size_t& scCount, bool active, std::vector<AttackAct>* atkAct = NULL);
	BattleFighter * getRandomFighter(UInt8 side, UInt8 * excepts, size_t exceptCount);
    bool doNormalAttack(BattleFighter* bf, int otherside, int target_pos, std::vector<AttackAct>* atkAct = NULL);
    bool doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, std::vector<AttackAct>* atkAct = NULL, UInt32 skillParam = 0);
    BattleFighter* getTherapyTarget(BattleFighter* bf);
    BattleFighter* getTherapyTarget2(BattleFighter* bf, UInt8 * excepts, size_t exceptCount);
    bool doSkillStatus(bool activeFlag, BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, StatusChange* scList, size_t& scCount, bool& self, bool ifDecAura);
    bool doSkillStatus2(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, StatusChange* scList, size_t& scCount);
    void doSkillState(BattleFighter* bf, const GData::SkillBase* skill, BattleObject* bo, DefStatus* defList, size_t& defCount, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct = NULL);
    void getSkillTarget(BattleFighter* bf, const GData::SkillBase* skill, int& target_side, int& target_pos, int& cnt);

    UInt32 doPoisonAttack(BattleFighter* bf, bool cs, const GData::SkillBase* skill, BattleFighter* area_target, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, std::vector<AttackAct>* atkAct);

    void doPassiveSkillBeAtk(BattleFighter* bf, BattleFighter* bo, std::vector<AttackAct>* atkAct2, std::vector<AttackAct>* atkAct, UInt32 dmg);
	void doSkillAtk2(bool activeFlag, std::vector<AttackAct>* atkAct, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount);

    UInt32 releaseCD(BattleFighter* bf);
    void releaseWeak(BattleFighter* bo, DefStatus* defList, size_t& defCount);

    bool isImmuneDecAura(const GData::SkillBase* skill, int target_side, int target_pos, DefStatus* defList, size_t& defCount);

    void  InitAttainRecord();
    void  CheckAttain();
    void  SendAttainMsgToPlayer( GObject::Player* player, UInt32 id, UInt32 param);

    typedef void (Battle::BattleSimulator::*setStatusFunc)(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    typedef void (Battle::BattleSimulator::*setStatusFunc2)(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);

    void setStatusChange_Aura(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_Atk(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_Def(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_MagAtk(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_MagDef(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_Tough(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_Action(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_Evade(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_Critical(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_Pierce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_Counter(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_MagRes(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_CriDmg(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_HitR(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_AtkReduce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange_MagAtkReduce(BattleFighter * bf, UInt8 side, UInt8 pos, const GData::SkillBase* skill, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);

    void setStatusChange2_Def(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_Aura(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_Atk(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_MagAtk(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_MagDef(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_Tough(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_Action(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_Evade(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_Critical(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_Pierce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_Counter(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_MagRes(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_CriDmg(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_HitR(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_AtkReduce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
    void setStatusChange2_MagAtkReduce(BattleFighter* bf, UInt8 side, UInt8 pos, UInt16 skillId, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);


    bool doSkillStrengthenAttack(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active);

    typedef bool (Battle::BattleSimulator::*doSkillStrengthenFunc)(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active);

    bool doSkillStrengthen_disperse(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active);
    bool doSkillStrengthen_reduce(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active);
    bool doSkillStrengthen_week(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active);
    bool doSkillStrengthen_debuf_defend(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active);
    bool doSkillStrengthen_atkadd(BattleFighter* bf, const GData::SkillBase* skill, const GData::SkillStrengthenEffect* ef, int target_side, int target_pos, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, bool active);

    bool doDeBufAttack(BattleFighter* bf);
    float calcTherapyDebuf(BattleFighter* bo, DefStatus* defList, size_t& defCount);
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

    UInt8 _teams[2];
    std::vector<std::string> _team_name[2];
    std::vector<UInt8> _team_level[2];
    std::vector<UInt16> _team_portrait[2];

	URandom _rnd;

    setStatusFunc statusFuncTable[MAX_STATUS];
    setStatusFunc2 statusFuncTable2[MAX_STATUS];

    doSkillStrengthenFunc skillStrengthenTable[GData::TYPE_MAX];

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
};

}

#endif // _BATTLESIMULATOR_H_

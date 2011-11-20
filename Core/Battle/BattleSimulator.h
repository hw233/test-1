#ifndef _BATTLESIMULATOR_H_
#define _BATTLESIMULATOR_H_

#include "BattleField.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "GData/Area.h"

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
    void clearLastBattle(UInt8 winner);
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
		UInt8 pos;
		UInt8 damType;
		UInt8 damType2;
		UInt16 damage;
		UInt32 leftHP;
        UInt16 rhp;
        UInt32 rLeftHP;
		UInt16 counterDmg;
		UInt32 counterLeft;
	};
	struct StatusChange
	{
		UInt8 pos;
		UInt16 statusId;
		UInt8 type;
		UInt16 data;
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
        e_stAura,
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
    };


private:
	int findFirstAttacker();
	UInt32 doAttack(int);
    UInt32 FightersEnter();
    UInt32 doSkillAttackAftEnter(BattleFighter* bf);
    void reQueueFighterStatus(BattleFighter* bf);
	void insertFighterStatus(BattleFighter* bf);
	void removeFighterStatus(BattleFighter* bf);
	UInt32 attackOnce(BattleFighter * bf, bool& cs, bool& pr, const GData::SkillBase* skill, BattleObject * bo, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, int counter_deny = -1, AttackPoint * counter_deny_list = NULL, std::vector<AttackAct>* atkAct = NULL);
	float testRescue(BattleFighter *& bf, int counter_deny, AttackPoint * counter_deny_list);
	float testLink(BattleFighter *& bf, UInt16& skillId);
	void onDead(BattleObject * bo, std::vector<AttackAct>* atkAct = NULL);
	int testWinner();
	void appendToPacket(UInt8 from_side, UInt8 from_pos, UInt8 target_pos, UInt8 atk_type, UInt16 add_id, bool cs, bool pr, DefStatus* defList, size_t defCount, StatusChange * scList, size_t scCount);
	UInt32 tryPreUseSkill(BattleFighter * bf, BattleObject * target_object);
	UInt32 tryDelayUseSkill(BattleFighter * bf, BattleObject * target_object);
	void setStatusChange(UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, UInt32 value, StatusChange * scList, size_t& scCount, bool active);
	void setStatusChange(UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
	void setStatusChange2(UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, UInt16 last, StatusChange * scList, size_t& scCount, bool active);
	void onDamage(BattleObject * bo, StatusChange * scList, size_t& scCount, bool active, std::vector<AttackAct>* atkAct = NULL);
	BattleFighter * getRandomFighter(UInt8 side, UInt8 * excepts, size_t exceptCount);
    UInt32 doNormalAttack(BattleFighter* bf, int otherside, int target_pos, std::vector<AttackAct>* atkAct = NULL);
    UInt32 doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, std::vector<AttackAct>* atkAct = NULL, UInt32 skillParam = 0);
    BattleFighter* getTherapyTarget(BattleFighter* bf);
    void doSkillStatus(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, StatusChange* scList, size_t& scCount);
    void doSkillStatus2(BattleFighter* bf, const GData::SkillBase* skill, int target_side, int target_pos, int cnt, StatusChange* scList, size_t& scCount);
    void doSkillState(BattleFighter* bf, const GData::SkillBase* skill, BattleObject* bo, DefStatus* defList, size_t& defCount, std::vector<AttackAct>* atkAct = NULL);
    void getSkillTarget(BattleFighter* bf, const GData::SkillBase* skill, int& target_side, int& target_pos, int& cnt);

    UInt32 doPoisonAttack(BattleFighter* bf, bool cs, const GData::SkillBase* skill, BattleFighter* area_target, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, std::vector<AttackAct>* atkAct);

    void doPassiveSkillBeAtk(BattleFighter* bf, BattleFighter* bo, std::vector<AttackAct>* atkAct, UInt32 dmg);

    UInt32 releaseCD(BattleFighter* bf);

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

    UInt8 _teams[2];
	std::string _team_name[2][2];
	UInt8 _team_level[2][2];
	UInt16 _team_portrait[2][2];

	URandom _rnd;
};

}

#endif // _BATTLESIMULATOR_H_

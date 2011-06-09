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

class BattleSimulator:
	public BattleField
{
public:
	BattleSimulator(UInt32, GObject::Player *, const std::string&, UInt8, bool report = true, UInt32 fake_turns = 0xFFFFFFFF);
	BattleSimulator(UInt32, GObject::Player *, GObject::Player *, bool report = true, UInt32 fake_turns = 0xFFFFFFFF);
	inline int getId() {return _id;}
	inline int getTurns() {return _turns;}
	void start();
	inline Stream& getPacket() {return _packet;}
	inline Script::BattleFormula * getFormula() { return _formula; }
	inline int getWinner() { return _winner; } // returns 1 for attacker winning, 2 for defender winning, or 0 for that game's still in progress
	bool applyFighterHP(UInt8, GObject::Player *, bool = true, UInt32 = 0);
	void getFighterHP(UInt8, GObject::Fighter **, UInt8 *, UInt32 * hp, UInt32 = 0);
	inline void setPortrait(UInt8 side, UInt16 por) { _portrait[side] = por; }
	BattleFighter * newFighter(UInt8 side, UInt8 pos, GObject::Fighter *);

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
		UInt16 damage;
		UInt32 leftHP;
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

private:
	int findFirstAttacker();
	UInt32 doAttack(int);
	void insertFighterStatus(FighterStatus& fs);
	void removeFighterStatus(FighterStatus& fs);
	UInt32 attackOnce(BattleFighter * bf, bool& cs, bool& pr, BattleObject * bo, float factor, DefStatus* defList, size_t& defCount, StatusChange* scList, size_t& scCount, int counter_deny = -1, AttackPoint * counter_deny_list = NULL);
	float testRescue(BattleFighter *& bf, int counter_deny, AttackPoint * counter_deny_list);
	float testLink(BattleFighter *& bf, UInt16& skillId);
	void onDead(BattleObject * bo);
	int testWinner();
	void appendToPacket(UInt8 from_side, UInt8 from_pos, UInt8 target_pos, UInt8 atk_type, UInt16 add_id, bool cs, bool pr, DefStatus* defList, size_t defCount, StatusChange * scList, size_t scCount);
	UInt32 tryPreUseSkill(BattleFighter * bf, BattleObject * target_object);
	UInt32 tryDelayUseSkill(BattleFighter * bf, BattleObject * target_object);
	void setStatusChange(UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, UInt32 value, StatusChange * scList, size_t& scCount, bool active);
	void setStatusChange(UInt8 side, UInt8 pos, int cnt, UInt16 skillId, UInt8 type, float value, StatusChange * scList, size_t& scCount, bool active);
	void onDamage(BattleObject * bo, StatusChange * scList, size_t& scCount, bool active);
	BattleFighter * getRandomFighter(UInt8 side, UInt8 * excepts, size_t exceptCount);
    UInt32 doNormalAttack(BattleFighter* bf, int otherside, int target_pos);
    UInt32 doSkillAttack(BattleFighter* bf, const GData::SkillBase* skill, BattleFighter* therapy_bf = NULL);
    BattleFighter* getTherapyTarget(BattleFighter* bf);

private:
	int _id, _winner, _turns;
	UInt8 _position;
	bool _report;
	UInt32 _fake_turns;
	std::vector<FighterStatus> _fgtlist[2];
    Int8 _cur_fgtlist_idx;
	Stream _packet;
	Script::BattleFormula * _formula;
	UInt16 _portrait[2];
	GObject::Player * _player[2];
	std::string _other_name;
	UInt8 _other_level;
	URandom _rnd;
};

}

#endif // _BATTLESIMULATOR_H_

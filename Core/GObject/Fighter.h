#ifndef _FIGHTER_H_
#define _FIGHTER_H_

#include "GObjectManager.h"

#define ARMORS_MAX 7
#define FIGHTER_BUFF_COUNT 4

#include "Item.h"
#include "GData/AttrExtra.h"
#include "Common/TimeUtil.h"
#include "Server/ServerTypes.h"

namespace GObject
{

#define GREAT_FIGHTER_MAX 800
#define NPC_FIGHTER_MAX 16384

#define FIGHTER_BUFF_ATTR1		0x01
#define FIGHTER_BUFF_ATTR2		0x02
#define FIGHTER_BUFF_ATTR3		0x03

class Player;
class Fighter
{
public:
	Fighter(UInt32, Player *);
	~Fighter();

	Fighter * clone(Player * owner);
	Fighter * cloneWithEquip(Player * owner);

	inline UInt32 getId() { return _id; }
	inline Player * getOwner() { return _owner; }
	inline void setOwner(Player * p) { _owner = p; }

	inline void setName(const std::string& s) {_name = s;}
	inline void setClass(UInt8 c) {_class = c;}
	inline void setMale(bool m) {_isMale = m;}
	inline void setLevel(UInt8 l) {_level = l;}
	inline void setExp(UInt64 e) {_exp = e;}
	void setLevelAndExp(UInt8 l, UInt64 e);
	void setPotential(float p, bool = true);
	void setCurrentHP(UInt16 hp, bool = true);
	void setSkill(UInt16 skill, bool = true);
	void setSkillLevel(UInt8 level, bool = true);
	void setSkillAndLevel(UInt16 data);
	void setSkillAndLevel(UInt16 skill, UInt8 level, bool = true);
	bool learnSkill(UInt16 skill);
	bool skillLevelUp(UInt8 lv);

	const std::string& getName();
	const std::string& getBattleName();
	inline UInt8 getClass() {return _class;}
	inline UInt8 getClassAndSex() {return _isMale ? (_class - 1) : _class;}
	inline bool isMale() {return _isMale;}
	inline bool isNpc() { return _id > GREAT_FIGHTER_MAX; }
	inline UInt8 getLevel() {return _level;}
	inline UInt64 getExp() {return _exp;}
	inline float getPotential() {return _potential;}
	inline UInt16 getCurrentHP() {return _hp;}
	inline UInt16 getSkill() { return _skill; }
	inline UInt8 getSkillLevel() { return _skillLevel; }
	inline UInt16 getSkillAndLevel() { return static_cast<UInt16>(_skill * 100 + _skillLevel); }
	inline ItemWeapon * getWeapon() { return _weapon; }
	inline ItemArmor * getArmor(int idx) { return (idx >= 0 && idx < 5) ? _armor[idx] : NULL; }
	inline ItemEquip * getAmulet() { return _amulet; }
	inline ItemEquip * getRing() { return _ring; }
	UInt32 getMaxHP();

	UInt32 regenHP(UInt32);
	bool addExp(UInt64);
	void sendModification(UInt8 t, UInt64 v);
	void sendModification(UInt8 n, UInt8 * t, UInt64 * v);
	void sendModification(UInt8 t, ItemEquip * v, bool = true);
	void sendModification(UInt8 n, UInt8 * t, ItemEquip ** v, bool = true);
	void updateToDB(UInt8 t, UInt64 v);

	UInt32 getBuffData(UInt8 idx, UInt32 now = TimeUtil::Now());
	void setBuffData(UInt8, UInt32, bool = true);

	UInt32 getWeaponId();
	UInt32 getArmorId(int idx);
	UInt32 getAmuletId();
	UInt32 getRingId();

	UInt16 getWeaponAttack();
	void getArmorDefendAndHP(UInt16& def, UInt16& hp);
	inline UInt8 getColor() { return _color; }
	static UInt8 getColor2(float);

	ItemWeapon * setWeapon(ItemWeapon * w, bool = true);
	ItemArmor * setArmor(int idx, ItemArmor * a, bool = true);
	ItemEquip * setAmulet(ItemEquip * a, bool = true);
	ItemEquip * setRing(ItemEquip * r, bool = true);
	ItemEquip * findEquip(UInt32 id, UInt8& pos);
	void removeEquip(UInt8 pos, ItemEquip * equip, UInt8 toWhere = 0);
	inline void setDirty(bool d = true) { _attrDirty = d; _bPDirty = d; }

public:
	inline const GData::Attr1Extra * getAttr1ExtraEquip() { checkDirty(); return &_attr1ExtraEquip; }
	inline const GData::Attr2Extra * getAttr2ExtraEquip() { checkDirty(); return &_attr2ExtraEquip; }
	inline UInt16 getExtraStrength() { checkDirty(); return _attr1ExtraEquip.strength; }
	inline UInt16 getExtraPhysique() { checkDirty(); return _attr1ExtraEquip.physique; }
	inline UInt16 getExtraAgility() { checkDirty(); return _attr1ExtraEquip.agility; }
	inline UInt16 getExtraIntelligence() { checkDirty(); return _attr1ExtraEquip.intelligence; }
	inline float getExtraStrengthP() { checkDirty(); return _attr1ExtraEquip.strengthP; }
	inline float getExtraPhysiqueP() { checkDirty(); return _attr1ExtraEquip.physiqueP; }
	inline float getExtraAgilityP() { checkDirty(); return _attr1ExtraEquip.agilityP; }
	inline float getExtraIntelligenceP() { checkDirty(); return _attr1ExtraEquip.intelligenceP; }
	inline UInt16 getExtraAttack() { checkDirty(); return _attr2ExtraEquip.attack; }
	inline float getExtraAttackP() { checkDirty(); return _attr2ExtraEquip.attackP; }
	inline UInt16 getExtraDefend() { checkDirty(); return _attr2ExtraEquip.defend; }
	inline float getExtraDefendP() { checkDirty(); return _attr2ExtraEquip.defendP; }
	inline UInt16 getExtraHP() { checkDirty(); return _attr2ExtraEquip.hp; }
	inline float getExtraHPP() { checkDirty(); return _attr2ExtraEquip.hpP; }
	inline float getExtraAction() { checkDirty(); return _attr2ExtraEquip.action; }
	inline float getExtraHitrate() { checkDirty(); return _attr2ExtraEquip.hitrate; }
	inline float getExtraEvade() { checkDirty(); return _attr2ExtraEquip.evade; }
	inline float getExtraCritical() { checkDirty(); return _attr2ExtraEquip.critical; }
	inline float getExtraPierce() { checkDirty(); return _attr2ExtraEquip.pierce; }
	inline float getExtraCounter() { checkDirty(); return _attr2ExtraEquip.counter; }
	inline float getBattlePoint() { checkBPDirty(); return _battlePoint; }

	inline Int16 getBaseStrength() { if(_id > 6) return strength; const Int16 add[6][6] = {{0}, {0}, {0}, {15, 15, 15, 15, 40, 40}, {35, 35, 35, 35, 90, 90}, {55, 55, 55, 55, 140, 140}}; return strength + add[_color][_class]; }
	inline Int16 getBasePhysique() { if(_id > 6) return physique; const Int16 add[6][6] = {{0}, {0}, {0}, {40, 40, 15, 15, 15, 15}, {90, 90, 30, 30, 35, 35}, {140, 140, 45, 45, 55, 55}}; return physique + add[_color][_class]; }
	inline Int16 getBaseAgility() { if(_id > 6) return agility; const Int16 add[6][6] = {{0}, {0}, {0}, {15, 15, 40, 40, 15, 15}, {30, 30, 90, 90, 30, 30}, {45, 45, 140, 140, 45, 45}}; return agility + add[_color][_class]; }
	inline Int16 getBaseIntelligence() { if(_id > 6) return intelligence; const Int16 add[6][6] = {{0}, {0}, {0}, {10, 10, 10, 10, 10, 10}, {25, 25, 25, 25, 25, 25}, {40, 40, 40, 40, 40, 40}}; return intelligence + add[_color][_class]; }
	inline Int16 getBaseAttack() { return attack; }
	inline Int16 getBaseDefend() { return defend; }
	inline float getBaseHitrate() { return hitrate; }
	inline float getBaseEvade() { if(_id > 6) return evade; const float add[6][6] = {{0}, {0}, {0}, {0, 0, 2, 2, 0, 0}, {0, 0, 4, 4, 0, 0}, {0, 0, 4, 4, 0, 0}}; return evade + add[_color][_class]; }
	inline float getBaseCritical() { if(_id > 6) return critical; const float add[6][6] = {{0}, {0}, {0}, {4, 4, 0, 0, 0, 0}, {8, 8, 0, 0, 0, 0}, {8, 8, 8, 8, 0, 0}}; return critical + add[_color][_class]; }
	inline float getBasePierce() { if(_id > 6) return pierce; const float add[6][6] = {{0}, {0}, {0}, {0, 0, 0, 0, 3, 3}, {0, 0, 0, 0, 6, 6}, {0, 0, 0, 0, 10, 10}}; return pierce + add[_color][_class]; }
	inline float getBaseCounter() { if(_id > 6) return counter; const float add[6][6] = {{0}, {0}, {0}, {0}, {0}, {4, 4, 0, 0, 0, 0}}; return counter + add[_color][_class];  }
	inline Int32 getBaseHP() { return maxhp; }
	inline UInt32 getBaseAction() { return action; }

protected:
	void addAttr( ItemEquip * );
	void rebuildEquipAttr();
	void rebuildBattlePoint();
	inline void checkDirty()
	{
		if(_attrDirty)
		{
			_attrDirty = false;
			rebuildEquipAttr();
		}
	}
	inline void checkBPDirty()
	{
		if(_bPDirty)
		{
			_bPDirty = false;
			rebuildBattlePoint();
		}
	}

protected:
	UInt32 _id;

	Player * _owner;

	std::string _name;
	bool _isMale;		// ÊÇ·ñÄÐÐÔ
	UInt8 _class;
	UInt8 _level;
	UInt64 _exp;
	float _potential;
	UInt8 _color;
	UInt16 _hp;

	UInt16 _skill;
	UInt8 _skillLevel;

	ItemWeapon * _weapon;
	ItemArmor * _armor[5];
	ItemEquip * _ring;
	ItemEquip * _amulet;

	bool _attrDirty;
	UInt32 _maxHP;
	bool _bPDirty;
	float _battlePoint;
	GData::Attr1Extra _attr1ExtraEquip;
	GData::Attr2Extra _attr2ExtraEquip;

	UInt32 _buffData[FIGHTER_BUFF_COUNT];

public:
	UInt32 favor;
	UInt32 reqFriendliness;
	Int32 strength;
	Int32 physique;
	Int32 agility;
	Int32 intelligence;
	Int32 attack;
	Int32 defend;
	Int32 maxhp;
	UInt32 action;
	float hitrate;
	float evade;
	float critical;
	float pierce;
	float counter;

	struct Offset { Int8 x, y; };
	std::vector<Offset> extraPos;
};

class GlobalFighters
{
public:
	struct FighterData
	{
		FighterData(Fighter * fgt = NULL): fighter(fgt), spot(0) { }
		Fighter * fighter;
		UInt16 spot;
	};

public:
	GlobalFighters(): _fighters(65536), _maxFgtId(0) {}
	void setAllDirty();
	void buildSummonSet();
	Fighter *& operator[] (const UInt32& k) { return _fighters[k].fighter; }
	Fighter * getRandomOut();
	Fighter * getRandomOut(Player * pl);
	Fighter * getRandomOut(Player * pl, std::set<UInt32>& excepts, std::set<UInt32>& excepts2, UInt8, UInt32 = 100);
	void setSpot(UInt32 id, UInt16 spot);
	UInt16 getSpot(UInt32 id);
	inline void setMaxFgtId(UInt32 n) { _maxFgtId = n; }

private:
	FastMutex _mutex;
	std::vector<FighterData> _fighters;
	UInt32 _maxFgtId;
	std::map<UInt32,UInt32> _summonSet[3];
};

extern GlobalFighters globalFighters;

extern Fighter& getGreatFighter(UInt32 id);
extern bool existGreatFighter(UInt32 id);

}

#endif // _FIGHTER_H_

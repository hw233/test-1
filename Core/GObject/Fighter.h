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

#define SKILL_LEVEL_MAX 100
#define SKILL_UPMAX 3
#define CITTA_LEVEL_MAX 100
#define CITTA_UPMAX 6
#define TRUMP_UPMAX 3
#define BLOODBIT_MAX 15

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
    inline void setPExp(UInt64 e) { _pexp = e; }
	void setLevelAndExp(UInt8 l, UInt64 e);
	void setPotential(float p, bool = true);
	void setCurrentHP(UInt16 hp, bool = true);

	const std::string& getName();
	const std::string& getBattleName();
	inline UInt8 getClass() {return _class;}
	inline UInt8 getClassAndSex() {return _isMale ? (_class - 1) : _class;}
	inline bool isMale() {return _isMale;}
	inline bool isNpc() { return _id > GREAT_FIGHTER_MAX; }
	inline UInt8 getLevel() {return _level;}
	inline UInt64 getExp() {return _exp;}
	inline UInt64 getPExp() {return _pexp;}
	inline float getPotential() {return _potential;}
	inline UInt16 getCurrentHP() {return _hp;}

    void setCapacity(float c, bool = true);
	inline float getCapacity() {return _capacity;}

    void setPeerless(UInt16 peerless, bool = true);
    inline UInt16 getPeerless() { return _peerless / SKILL_LEVEL_MAX; }
    inline UInt16 getPeerlessLevel() { return _peerless % SKILL_LEVEL_MAX; }
    inline UInt16 getPeerlessAndLevel() { return _peerless; }

    inline UInt8 getBloodCntMax() { return 3; }
    bool setBloodBit(int idx, UInt8 v, bool = true);
    bool incBloodBit(int idx, bool = true);

    inline UInt8 getBloodBit(int idx) { return (idx >= 0 && idx < BLOODBIT_MAX) ? _bloodbit[idx] : static_cast<UInt8>(-1); }
    void getAllBloodBits(Stream& st);
    void setBloodBits(std::string& bloodbit, bool = true);

    // TODO:
#if 0
	void setSkillLevel(UInt8 level, bool = true);
	void setSkillAndLevel(UInt16 data);
	void setSkillAndLevel(UInt16 skill, UInt8 level, bool = true);
#endif

    // 学习技能
	bool learnSkill(UInt16 skill);
    // 装备技能
    void upSkill(UInt16 skill, bool = true);
    // 卸下技能
    void offSkill(UInt16 skill);
    // 交换两个技能顺序
    void swapSkill(UInt8 idx1, UInt8 idx2, bool = true);
    // 通过索引装备技能
    void upSkillByIdx(UInt16 skill, bool = true);
    // 升级技能
	bool skillLevelUp(UInt16 skill, UInt8 lv);
    // 通过索引升级技能
	bool skillLevelUpByIdx(UInt8 , UInt8 lv);
    // 是否学会此技能
    UInt8 hasSkill(UInt16 skill);
    // 是否装备此技能
    UInt8 isKillUp(UInt16 skill);
    // 通过索引卸下技能
    void offSkillByIdx(UInt8 idx);

    inline UInt8 getUpSkills() { return _skillup; }
    inline UInt8 getUpMaxSkills() { return SKILL_UPMAX; }
	inline UInt16 getUpSkill(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] / SKILL_LEVEL_MAX : 0; }
	inline UInt8 getUpSkillLevel(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] % SKILL_LEVEL_MAX : 0; }
	inline UInt16 getUpSkillAndLevel(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] : 0; }

    inline UInt8 getSkills() { return _skills.size(); }

    // 取得所有装备的技能和等级
    void getAllUpSkillAndLevel(Stream& st);
    // 取得所有学习的技能和等级
    void getAllSkillsAndLevel(Stream& st);
    // 取得装备了的和学习了的技能和等级
    void getAllSkillAndLevel(Stream& st);

    void setUpSkills(std::string& skill, bool = true);
    void setSkills(std::string& skills, bool = true);
    void setUpCittas(std::string& citta, bool = true);
    void setCittas(std::string& cittas, bool = true);

    UInt8 hasCitta(UInt16 citta);
    UInt8 isCittaUp(UInt16 citta);
    inline UInt8 getUpCittas() { return _cittaup; }
    inline UInt8 getUpMaxCittas() { return CITTA_UPMAX; }
	inline UInt16 getUpCitta(int idx = 0) { return (idx >= 0 && idx < CITTA_UPMAX) ? _citta[idx] / CITTA_LEVEL_MAX : 0; }
	inline UInt8 getUpCittaLevel(int idx = 0) { return (idx >= 0 && idx < CITTA_UPMAX) ? _citta[idx] % CITTA_LEVEL_MAX : 0; }
	inline UInt16 getUpCittaAndLevel(int idx = 0) { return (idx >= 0 && idx < CITTA_UPMAX) ? _citta[idx] : 0; }
    inline UInt8 getCittas() { return _cittas.size(); }
    // 取得所有装备的心法和等级
    void getAllUpCittaAndLevel(Stream& st);
    // 取得所有学习的心法和等级
    void getAllCittasAndLevel(Stream& st);
    // 取得装备了的和学习了的心法和等级
    void getAllCittaAndLevel(Stream& st);

	inline ItemWeapon * getWeapon() { return _weapon; }
	inline ItemArmor * getArmor(int idx) { return (idx >= 0 && idx < 5) ? _armor[idx] : NULL; }
	inline ItemEquip * getAmulet() { return _amulet; }
	inline ItemEquip * getRing() { return _ring; }

	inline ItemEquip * getTrump(int idx) { return (idx >= 0 && idx < TRUMP_UPMAX) ? _trump[idx] : 0; }
    inline int getMaxTrumps () { return TRUMP_UPMAX; }
    UInt32 getTrumpId(int idx);
    int getAllTrumpId(UInt32* trumps, int size = TRUMP_UPMAX);
    void getAllTrumps(Stream& st);

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
	ItemEquip ** setTrump(std::string& trumps, bool = true);
    ItemEquip* setTrump( UInt32 trump, int idx, bool = true);
    ItemEquip* setTrump( ItemEquip* trump, int idx, bool = true);
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

    template <typename T>
    bool value2string(T* values, int size, std::string& str)
    {
        if (!values || size)
            return false;

        char buf[256] = {0};
        char* pbuf = buf;
        char* pend = &buf[sizeof(buf)-1];
        for (int i = 0; i < size; ++i)
        {
            pbuf += snprintf(pbuf, pend - pbuf, "%u", values[i]);
            if (i < size - 1)
                pbuf += snprintf(pbuf, pend - pbuf, "|");
        }
        return true;
    }

protected:
	UInt32 _id;

	Player * _owner;

	std::string _name;
	bool _isMale;		// 是否男性
	UInt8 _class;
	UInt8 _level;
	UInt64 _exp;        // 经验
    UInt64 _pexp;       // 修炼经验
	float _potential;   // 资质
	float _capacity;    // 潜力
	UInt8 _color;
	UInt16 _hp;

    UInt16 _peerless;       // 无双技能
    UInt8 _bloodbit[BLOODBIT_MAX];    // 血道

    UInt16 _skill[SKILL_UPMAX];     // 装备的技能 _skill[i] % SKILL_LEVEL_MAX => skilllevel, _skill[i]/SKILL_LEVEL_MAX=> skillid 
    UInt8 _skillup;                 // 装备的技能个数
    std::vector<UInt16> _skills;    // 学会的技能

    UInt16 _citta[CITTA_UPMAX];  // 装备的心法
    UInt8 _cittaup;              // 装备的心法个数
    std::vector<UInt16> _cittas; // 学会的心法

	ItemWeapon * _weapon;
	ItemArmor * _armor[5];
	ItemEquip * _ring;
	ItemEquip * _amulet;
	ItemEquip * _trump[TRUMP_UPMAX];    // 法宝

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

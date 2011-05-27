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
#define ACUPOINTS_MAX 15

#define SKILL_LEVEL(x)  ((x)%SKILL_LEVEL_MAX)
#define SKILL_ID(x) ((x)/SKILL_LEVEL_MAX)
#define SKILLANDLEVEL(s,l) ((s)*SKILL_LEVEL_MAX | (l))

#define CITTA_LEVEL(x) ((x)%CITTA_LEVEL_MAX)
#define CITTA_ID(x) ((x)/CITTA_LEVEL_MAX)
#define CITTAANDLEVEL(c,l) ((c)*CITTA_LEVEL_MAX | (l))

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
    inline void setSex(UInt8 s) {_sex = s;}
	inline void setLevel(UInt8 l) {_level = l;}
	inline void setExp(UInt64 e) {_exp = e;}
    inline void setPExp(UInt64 e) { _pexp = e; }
	void setLevelAndExp(UInt8 l, UInt64 e);
	void setPotential(float p, bool = true);
	void setCurrentHP(UInt16 hp, bool = true);

	const std::string& getName();
	const std::string& getBattleName();
	inline UInt8 getClass() {return _class;}
	inline UInt8 getClassAndSex() {return _class<<4|_sex;}
	inline bool isMale() {return _sex == 0;}
	inline bool isBoy() {return _sex == 2;}
    inline UInt8 getSex() {return _sex;}
	inline bool isNpc() { return _id > GREAT_FIGHTER_MAX; }
	inline UInt8 getLevel() {return _level;}
	inline UInt64 getExp() {return _exp;}
	inline UInt64 getPExp() {return _pexp;}
	inline float getPotential() {return _potential;}
	inline UInt16 getCurrentHP() {return _hp;}

    void setCapacity(float c, bool = true);
	inline float getCapacity() {return _capacity;}

    void setPeerless(UInt16 pl, bool = true);
    inline UInt16 getPeerless() { return peerless / SKILL_LEVEL_MAX; }
    inline UInt16 getPeerlessLevel() { return peerless % SKILL_LEVEL_MAX; }
    inline UInt16 getPeerlessAndLevel() { return peerless; }

    inline UInt8 getAcupointsCntMax() { return 3; }
    bool setAcupointsBit(int idx, UInt8 v, bool = true);
    bool incAcupointsBit(int idx, bool = true);

    inline UInt8 getAcupointsBit(int idx) { return (idx >= 0 && idx < ACUPOINTS_MAX) ? _acupoints[idx] : static_cast<UInt8>(-1); }
    void getAllAcupointsBits(Stream& st);
    void setAcupointsBits(std::string& acupoints, bool = true);

    // XXX: 由心法和法宝带出技能，且技能不需要升级
#if 0
    // 学习技能
	bool learnSkill(UInt16 skill);
#endif
    // 装备技能
    bool upSkill(UInt16 skill, int idx, bool = true);
    // 卸下技能
    bool offSkill(UInt16 skill, bool = true);
    // 升级技能
	bool skillLevelUp(UInt16 skill, UInt8 lv);
    // 技能是否可装备
    int hasSkill(UInt16 skill);
    // 是否装备此技能
    int isSkillUp(UInt16 skill);
    // 通过索引卸下技能
    bool offSkillByIdx(UInt8 idx);
    // 取得装备的技能个数
    int getUpSkillsNum();
    
    bool addNewSkill(UInt16 skill, bool = true);

    inline UInt8 getUpSkillsMax() { return SKILL_UPMAX; }
	inline UInt16 getUpSkill(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] / SKILL_LEVEL_MAX : 0; }
	inline UInt8 getUpSkillLevel(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] % SKILL_LEVEL_MAX : 0; }
	inline UInt16 getUpSkillAndLevel(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] : 0; }

    inline UInt8 getSkillsNum() { return _skills.size(); }

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

    // 装备心法
    bool upCitta(UInt16 citta, int idx, bool = true);
    // 卸下心法
    bool offCitta(UInt16 citta, bool = true);
    // 增加一个心法
    bool addNewCitta(UInt16 citta, bool = true);
    // 删除一个心法
    bool delCitta(UInt16 citta, bool = true);

    int hasCitta(UInt16 citta);
    int isCittaUp(UInt16 citta);

    inline UInt8 getUpCittasNum();
    inline UInt8 getUpCittasMax() { return CITTA_UPMAX; }
	inline UInt16 getUpCitta(int idx = 0) { return (idx >= 0 && idx < CITTA_UPMAX) ? CITTA_ID(_citta[idx]) : 0; }
	inline UInt8 getUpCittaLevel(int idx = 0) { return (idx >= 0 && idx < CITTA_UPMAX) ? CITTA_LEVEL(_citta[idx]) : 0; }
	inline UInt16 getUpCittaAndLevel(int idx = 0) { return (idx >= 0 && idx < CITTA_UPMAX) ? _citta[idx] : 0; }
    inline UInt8 getCittasNum() { return _cittas.size(); }
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

    void sendModificationAcupointsBit(UInt8 t, int idx, bool = true);

#if 1
    void sendModification(UInt8 t, UInt16 skill, int idx, bool = true);
#else
    void sendModificationUpSkill(UInt8 t, UInt16 skill, int idx, bool = true);
    void sendModificationUpCitta(UInt8 t, UInt16 citta, int idx, bool = true);
    void sendModificationSkills(UInt8 t, UInt16 skill, int idx, bool = true);
    void sendModificationCittas(UInt8 t, UInt16 citta, int idx, bool = true);
#endif

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
	inline const GData::AttrExtra * getAttrExtraEquip() { checkDirty(); return &_attrExtraEquip; }
	inline UInt16 getExtraStrength() { checkDirty(); return _attrExtraEquip.strength; }
	inline UInt16 getExtraPhysique() { checkDirty(); return _attrExtraEquip.physique; }
	inline UInt16 getExtraAgility() { checkDirty(); return _attrExtraEquip.agility; }
	inline UInt16 getExtraIntelligence() { checkDirty(); return _attrExtraEquip.intelligence; }
	inline UInt16 getExtraWill() { checkDirty(); return _attrExtraEquip.will; }
	inline UInt16 getExtraSoul() { checkDirty(); return _attrExtraEquip.soul; }
	inline UInt16 getExtraAura() { checkDirty(); return _attrExtraEquip.aura; }
	inline UInt16 getExtraTough() { checkDirty(); return _attrExtraEquip.tough; }
	inline float getExtraStrengthP() { checkDirty(); return _attrExtraEquip.strengthP; }
	inline float getExtraPhysiqueP() { checkDirty(); return _attrExtraEquip.physiqueP; }
	inline float getExtraAgilityP() { checkDirty(); return _attrExtraEquip.agilityP; }
	inline float getExtraIntelligenceP() { checkDirty(); return _attrExtraEquip.intelligenceP; }
	inline float getExtraWillP() { checkDirty(); return _attrExtraEquip.willP; }
	inline float getExtraSoulP() { checkDirty(); return _attrExtraEquip.soulP; }
	inline float getExtraAuraP() { checkDirty(); return _attrExtraEquip.auraP; }
	inline float getExtraToughP() { checkDirty(); return _attrExtraEquip.toughP; }
	inline UInt16 getExtraAttack() { checkDirty(); return _attrExtraEquip.attack; }
	inline float getExtraAttackP() { checkDirty(); return _attrExtraEquip.attackP; }
	inline UInt16 getExtraMagAttack() { checkDirty(); return _attrExtraEquip.mag_attack; }
	inline float getExtraMagAttackP() { checkDirty(); return _attrExtraEquip.mag_attackP; }
	inline UInt16 getExtraDefend() { checkDirty(); return _attrExtraEquip.defend; }
	inline float getExtraDefendP() { checkDirty(); return _attrExtraEquip.defendP; }
	inline UInt16 getExtraMagDefend() { checkDirty(); return _attrExtraEquip.mag_defend; }
	inline float getExtraMagDefendP() { checkDirty(); return _attrExtraEquip.mag_defendP; }
	inline UInt16 getExtraHP() { checkDirty(); return _attrExtraEquip.hp; }
	inline float getExtraHPP() { checkDirty(); return _attrExtraEquip.hpP; }
	inline float getExtraAction() { checkDirty(); return _attrExtraEquip.action; }
	inline float getExtraHitrate() { checkDirty(); return _attrExtraEquip.hitrate; }
	inline float getExtraEvade() { checkDirty(); return _attrExtraEquip.evade; }
	inline float getExtraCritical() { checkDirty(); return _attrExtraEquip.critical; }
	inline float getExtraCriticalDmg() { checkDirty(); return _attrExtraEquip.critical_dmg; }
	inline float getExtraPierce() { checkDirty(); return _attrExtraEquip.pierce; }
	inline float getExtraCounter() { checkDirty(); return _attrExtraEquip.counter; }
	inline float getExtraMagRes() { checkDirty(); return _attrExtraEquip.mag_res; }
	inline float getBattlePoint() { checkBPDirty(); return _battlePoint; }

	inline Int16 getBaseStrength()
    {
        return strength;
        // XXX: 暂时不启用
        if (_id > 9)
            return strength;
        static const Int16 add[9][9] = {
            {0}, {0}, {0},
            {15, 15, 15, 15, 40, 40},
            {35, 35, 35, 35, 90, 90},
            {55, 55, 55, 55, 140, 140}
        };
        return strength + add[_color][_class];
    }

	inline Int16 getBasePhysique()
    {
        return physique;
        if (_id > 9)
            return physique;
        static const Int16 add[9][9] = {{0}, {0}, {0}, {40, 40, 15, 15, 15, 15}, {90, 90, 30, 30, 35, 35}, {140, 140, 45, 45, 55, 55}};
        return physique + add[_color][_class];
    }

	inline Int16 getBaseAgility()
    {
        return agility;
        if (_id > 9)
            return agility;
        static const Int16 add[9][9] = {{0}, {0}, {0}, {15, 15, 40, 40, 15, 15}, {30, 30, 90, 90, 30, 30}, {45, 45, 140, 140, 45, 45}};
        return agility + add[_color][_class];
    }

	inline Int16 getBaseIntelligence()
    {
        return intelligence;
        if (_id > 9)
            return intelligence;
        static const Int16 add[9][9] = {{0}, {0}, {0}, {10, 10, 10, 10, 10, 10}, {25, 25, 25, 25, 25, 25}, {40, 40, 40, 40, 40, 40}};
        return intelligence + add[_color][_class];
    }

	inline Int16 getBaseWill() { return will; }
	inline Int16 getBaseSoul() { return soul; }
	inline Int16 getBaseAura() { return aura; }
	inline Int16 getBaseTough() { return tough; }

	inline Int16 getBaseAttack() { return attack; }
	inline Int16 getBaseMagAttack() { return mag_attack; }
	inline Int16 getBaseDefend() { return defend; }
	inline Int16 getBaseMagDefend() { return mag_defend; }
	inline float getBaseHitrate() { return hitrate; }

	inline float getBaseEvade()
    {
        return evade;
        if(_id > 9)
            return evade;
        static const float add[9][9] = {{0}, {0}, {0}, {0, 0, 2, 2, 0, 0}, {0, 0, 4, 4, 0, 0}, {0, 0, 4, 4, 0, 0}};
        return evade + add[_color][_class];
    }

	inline float getBaseCritical()
    {
        return critical;
        if(_id > 9)
            return critical;
        const float add[9][9] = {{0}, {0}, {0}, {4, 4, 0, 0, 0, 0}, {8, 8, 0, 0, 0, 0}, {8, 8, 8, 8, 0, 0}};
        return critical + add[_color][_class];
    }

    inline float getBaseCriticalDmg()
    {
        return critical_dmg;
    }

	inline float getBasePierce()
    {
        return pierce;
        if (_id > 9)
            return pierce;
        static const float add[9][9] = {{0}, {0}, {0}, {0, 0, 0, 0, 3, 3}, {0, 0, 0, 0, 6, 6}, {0, 0, 0, 0, 10, 10}};
        return pierce + add[_color][_class];
    }

	inline float getBaseCounter()
    {
        return counter;
        if (_id > 9)
            return counter;
        static const float add[9][9] = {{0}, {0}, {0}, {0}, {0}, {4, 4, 0, 0, 0, 0}};
        return counter + add[_color][_class];
    }

    inline float getBaseMagRes() { return mag_res; }
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

        if (pbuf != buf)
            str = buf;
        return true;
    }

protected:
	UInt32 _id;

	Player * _owner;

	std::string _name;
    UInt8 _sex;         // 性别
	UInt8 _class;
	UInt8 _level;
	UInt64 _exp;        // 经验
    UInt64 _pexp;       // 修炼经验
	float _potential;   // 资质
	float _capacity;    // 潜力
	UInt8 _color;
	UInt16 _hp;

    UInt8 _acupoints[ACUPOINTS_MAX];    // 穴道

    UInt16 _skill[SKILL_UPMAX];     // 装备的技能 _skill[i] % SKILL_LEVEL_MAX => skilllevel, _skill[i]/SKILL_LEVEL_MAX=> skillid 
    std::vector<UInt16> _skills;    // 可装备的技能

    UInt16 _citta[CITTA_UPMAX];  // 装备的心法
    std::vector<UInt16> _cittas; // 可装备的心法

	ItemWeapon * _weapon;
	ItemArmor * _armor[5];
	ItemEquip * _ring;
	ItemEquip * _amulet;
	ItemEquip * _trump[TRUMP_UPMAX];    // 法宝

	bool _attrDirty;
	UInt32 _maxHP;
	bool _bPDirty;
	float _battlePoint;
	GData::AttrExtra _attrExtraEquip;

	UInt32 _buffData[FIGHTER_BUFF_COUNT];

public:
	UInt32 favor;
	UInt32 reqFriendliness;
	Int32 strength;
	Int32 physique;
	Int32 agility;
	Int32 intelligence;
    Int32 will;
    Int32 soul;
    Int32 aura;
    Int32 tough;
	Int32 attack;
	Int32 mag_attack;
	Int32 defend;
	Int32 mag_defend;
	Int32 maxhp;
	UInt32 action;
    UInt16 peerless;       // 无双技能
	float hitrate;
	float evade;
	float critical;
	float critical_dmg;
	float pierce;
	float counter;
	float mag_res;

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

﻿#ifndef _FIGHTER_H_
#define _FIGHTER_H_

#include "GObjectManager.h"

#define ARMORS_MAX 7

#include "Item.h"
#include "GData/AttrExtra.h"
#include "Common/TimeUtil.h"
#include "Server/ServerTypes.h"
#include "GData/SkillTable.h"
#include "GData/CittaTable.h"
#include "GObject/WBossMgr.h"
#include "GObject/Mail.h"
#include "SecondSoul.h"
#include "ShuoShuo.h"

namespace GObject
{

#define GREAT_FIGHTER_MAX 800
#define NPC_FIGHTER_MAX 16384

#define FIGHTER_BUFF_ATTR1		0x01
#define FIGHTER_BUFF_ATTR2		0x02
#define FIGHTER_BUFF_ATTR3		0x03
#define FIGHTER_BUFF_XTHTYT     0x04
#define FIGHTER_BUFF_CRMASGIRL  0x05
#define FIGHTER_BUFF_DRESS      0x07
#define FIGHTER_BUFF_WEDDING    0x08

#define FIGHTER_BUFF_RMAN       0x09 //儒男
#define FIGHTER_BUFF_RWMAN      0x0A //儒女
#define FIGHTER_BUFF_SMAN       0x0B //释男
#define FIGHTER_BUFF_SWMAN      0x0C //释女
#define FIGHTER_BUFF_DMAN       0x0D //道男
#define FIGHTER_BUFF_DWMAN      0x0E //道女

#define FIGHTER_BUFF_RDIAMOND   0x0F //红钻变身卡
#define FIGHTER_BUFF_BLUE       0x10 //蓝钻变身卡
#define FIGHTER_BUFF_QQVIP      0x11 //QQ会员变身卡
#define FIGHTER_BUFF_SANTA      0x12 //圣诞老人变身卡

#define FIGHTER_BUFF_COUNT 0x20
#define FIGHTER_BUFF_START 0x80

#define SKILL_UPMAX 3 // 技能最初就能装备3个
#define CITTA_UPMAX 9
#define CITTA_INIT 5 // 心法最初只能装2个,由穴道控制装备个数
#define TRUMP_UPMAX 3
#define TRUMP_INIT 1 // 法宝最初只能装1个,由VIP等级控制装备个数
#define ACUPOINTS_MAX 15
#define ACUPOINTSGOLD_MAX 9    //本命金丹最大值
#define LINGBAO_UPMAX 3
#define LINGSHI_UPMAX 3

#define PEERLESS_UPMAX 1

enum
{
    e_lb_ling = 0,
    e_lb_wu = 1,
    e_lb_xin = 2,

    e_lb_max
};

enum
{
    e_cls_ru = 1,
    e_cls_shi = 2,
    e_cls_dao = 3,
    e_cls_mo = 4,
    e_cls_qinglong = 5,
    e_cls_baihu = 6,
    e_cls_zhuque = 7,
    e_cls_xuanwu = 8,

    e_cls_max
};

struct DBXingchen;
struct Xingchenzhen
{
    Xingchenzhen() : lvl(0), curVal(0), xctCurVal(0), xctMaxVal(0)  { memset(gems, 0, sizeof(gems)); }

    UInt8 lvl;          // 星辰等级
    UInt32 curVal;      // 当前星辰值
    UInt16 gems[7];     // 镶嵌的宝石id
    UInt16 xctCurVal;   // 星辰图当前值
    UInt16 xctMaxVal;   // 星辰图最大值
};
struct XinMoData
{
    XinMoData() : lvl(0),val(0){}
    UInt8 lvl;
    UInt32 val;
};

struct SStrengthen
{
    SStrengthen() : father(0), maxVal(0), curVal(0), lvl(0) {}

    UInt16 father; // 带出此技能的心法或法宝ID
    UInt32 maxVal; // 熔炼最大值
    UInt32 curVal; // 熔炼当前值
    UInt8 lvl; // 熔炼等级
    UInt8 maxLvl; // 解封等级
};

struct SGrade
{
    SGrade() : lvl(0) {}

    UInt8 lvl; // 当前等级
};

struct SoulItemExp
{
    UInt16 itemId;
    UInt8 res;
    Int16 exp;
};

struct LBSkill
{
    UInt32 lbid;
    UInt16 skillid;
    UInt16 factor;
};

struct ElixirAttr
{
    ElixirAttr() : strength(0), physique(0), agility(0), intelligence(0), will(0), soul(0),
    attack(0), defend(0), critical(0), pierce(0), evade(0), counter(0), tough(0), action(0) {}

    ElixirAttr& operator+=(ElixirAttr& attr)
    {
        strength += attr.strength;
        physique += attr.physique;
        agility += attr.agility;
        intelligence += attr.intelligence;
        will += attr.will;
        soul += attr.soul;
        attack += attr.attack;
        defend += attr.defend;
        critical += attr.critical;
        pierce += attr.pierce;
        evade += attr.evade;
        counter += attr.counter;
        tough += attr.tough;
        action += attr.action;
        return *this;
    }

    Int32 strength;
    Int32 physique;
    Int32 agility;
    Int32 intelligence;
    Int32 will;
    Int32 soul; // max soul
    Int32 attack; // atk,magatk
    Int32 defend; // def,magdef
    Int32 critical;
    Int32 pierce;
    Int32 evade;
    Int32 counter;
    Int32 tough;
    Int32 action;
};

class Player;
class Evolution;
class Fighter
{
public:
	Fighter(UInt32, Player *);
    //Fighter(const Fighter& fighter);
	~Fighter();

	virtual Fighter * clone(Player * owner);
	virtual Fighter * cloneWithEquip(Player * owner);
    virtual Fighter * cloneWithOutDirty(Player * player);

	inline UInt32 getId() { return _id; }
	inline Player * getOwner() { return _owner; }
	inline void setOwner(Player * p) { _owner = p; }

	inline void setName(const std::string& s) {_name = s;}

	inline void setClass(UInt8 c) { _class = c; }

    inline void setSex(UInt8 s) {_sex = s;}
	inline void setLevel(UInt8 l, bool boss = false) { _level = l; if (boss) worldBoss.setLevel(l); }
	inline void setExp(UInt64 e) {_exp = e;}
    inline void setPExp(UInt32 e) { _pexp = e; }
    inline void setPExpMax(UInt32 e) { _pexpMax = e; }
	void setLevelAndExp(UInt8 l, UInt64 e);
	void setPotential(float p, bool = true);
	void setCurrentHP(UInt32 hp, bool = true);
	void addHPPercent(UInt8 p, bool = true);

	const std::string& getName();
	const std::string& getBattleName();
	inline UInt8 getClass() {return _class;}
	inline UInt8 getClassAndSex() {return _class<<4|_sex;}
	inline bool isMale() {return _sex == 0;}
	inline bool isBoy() {return _sex == 2;}
    inline UInt8 getSex() {return _sex;}
	inline bool isNpc() { return _id > GREAT_FIGHTER_MAX; }
    inline bool isWBoss() { return _iswboss; }
    inline void setWBoss(bool v) { _iswboss = v; }
    inline bool isWBossInspire() { return _iswbossinspire; }
    inline void setWBossInspire(bool v) { _iswbossinspire = v; }
    inline bool isClanBigBossBuf() { return _iscbbbuf; }
    inline void setClanBigBossBuf(bool v) { _iscbbbuf = v; }
    inline bool isDarkDargonBuf() { return _isddbuf; }
    inline void setDarkDargonBuf(bool v) { _isddbuf= v; }
	inline UInt8 getLevel() {return _level;}
    inline UInt8 getLevelInLua() { if (isPet() && _level >= 50) return _level - 49;  return _level;}
	inline UInt64 getExp() {return _exp;}
	inline UInt32 getPExp() {return _pexp;}
	inline UInt32 getPExpMax() {return _pexpMax;}
    inline bool isPExpFull() { return _pexpMax <= _pexp; }
	inline float getPotential() {return _potential;}
	inline UInt32 getCurrentHP() {return _hp;}
    bool isExpFull();
    UInt64 getExpMax();

    void setCapacity(float c, bool = true);
	inline float getCapacity() {return _capacity;}
    
    //获取先天技能
    UInt16 getInnateSkill();

    // 装备无双技能
    void setPeerless(UInt16 pl, bool = true);
    inline void upPeerless(UInt16 pl, bool writedb = true) { setPeerless(pl, writedb); }
    // 卸下无双技能
    void offPeerless(bool = true);
    // 增加一个可装备的无双技能
    bool addNewPeerless(UInt16 pl, bool = true, bool = false);
    // 删除一个可装备的无双技能
    bool delPeerless(UInt16 pl, bool = true);
    inline UInt16 getPeerless() { return peerless / SKILL_LEVEL_MAX; }
    inline UInt16 getPeerlessLevel() { return peerless % SKILL_LEVEL_MAX; }
    inline UInt16 getPeerlessAndLevel() { return peerless; }
    // 是否装备了pl这个无双技能
    inline bool isPeerlessUp(UInt16 pl) { return SKILL_ID(peerless) == SKILL_ID(pl); }
    // 是否有pl这个可装备的无双技能
    int hasPeerless(UInt16 pl);
    
  
    inline void SetPowerUpByP(float up) { _powerUp = up; }
    inline float GetPowerUp(){ return _powerUp; }
    inline void ClearPowerUp(){ _powerUp = 0; }

    inline UInt8 getAcupointsCntMax() { return 3; }
    UInt8 getAcupointCnt();
    bool setAcupoints(int idx, UInt8 v, bool = true, bool = false);
    bool setAcupointsGold(int idx, UInt8 v, bool = true, bool = false);
    float getAcupointsGoldAttr(UInt8 attrId);
    bool incAcupointsBit(int idx, bool = true);

    inline UInt8 getAcupointsBit(int idx) { return (idx >= 0 && idx < ACUPOINTS_MAX) ? _acupoints[idx] : static_cast<UInt8>(-1); }
    void getAllAcupointsBits(Stream& st);
    void getAllAcupointsGoldBits(Stream& st);
    void setAcupoints(std::string& acupoints, bool = true);
    void setAcupointsGold(std::string& acupoints, bool = true);
    void getAllLingbaoLevelAndFall(Stream& st);
    bool setToAcupoints(int idx, bool writedb);

    // XXX: 由心法和法宝带出技能，且技能不需要升级
#if 0
    // 学习技能
	bool learnSkill(UInt16 skill);
#endif
    bool testMutual(UInt16 skill);
    // 装备技能
    bool upSkill(UInt16 skill, int idx, bool = true, bool = true);
    // 卸下技能
    bool offSkill(UInt16 skill, bool = true);
    // 更新技能
    bool updateSkill(UInt16 skill, UInt16 nskill, bool sync = true, bool = true);
    // 升级技能
	bool skillLevelUp(UInt16 skill, UInt8 lv);
    // 技能是否可装备
    int hasSkill(UInt16 skill);
    // 是否装备此技能
    int isSkillUp(UInt16 skill);
    // 通过索引卸下技能
    bool offSkillByIdx(UInt8 idx);
    // 取得装备的技能个数
    UInt16 getUpSkillsNum();
    // 取得所有被动技能的个数
    UInt16 getPSkillsNum();
    // 取得100%被动技能的个数
    UInt16 getP100SkillsNum();
    // 取得概率被动技能的个数
    UInt16 getPnSkillsNum();
    // 增加一个新技能,包括技能升级
    bool addNewSkill(UInt16 skill, bool = true, bool = false, bool = false);
    // 删除一个可装备的技能
    bool delSkill(UInt16 skill, bool = true, bool = true, bool = true);
    // 取得装备技能的最大数
    inline UInt8 getUpSkillsMax() { return SKILL_UPMAX; }
    // 取得技能装备位置idx处的技能ID
	inline UInt16 getUpSkill(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] / SKILL_LEVEL_MAX : 0; }
    // 取得技能装备位置idx处的技能等级
	inline UInt8 getUpSkillLevel(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] % SKILL_LEVEL_MAX : 0; }
    // 取得技能装备位置idx处的技能ID和等级
	inline UInt16 getUpSkillAndLevel(int idx = 0) { return (idx >= 0 && idx < SKILL_UPMAX) ? _skill[idx] : 0; }
    // 取得默认技能
    inline UInt16 getDefaultSkillAndLevel() { return _skills.size()?_skills[0]:0; }
    // 取得技能的容器
    inline const std::vector<UInt16>& getSkillsVec() { return _skills;}
    // 取得可装备的技能数
    UInt8 getSkillsNum();
    // 取得所有装备的技能和等级
    void getAllUpSkillAndLevel(Stream& st);
    // 取得所有被动技能
    void getAllPSkillAndLevel(Stream& st);
    void getAllPSkillAndLevel4Arena(Stream& st);
    void getAllLingshiSkillAndLevel2Arena(Stream& st);
    // 取得所有学习的技能和等级
    void getAllSkillsAndLevel(Stream& st);
    // 取得装备了的和学习了的技能和等级
    void getAllSkillAndLevel(Stream& st);
    // 初始化装备的技能
    void setUpSkills(std::string& skill, bool = true);
    // 初始化可装备的技能
    void setSkills(std::string& skills, bool = true);
    // 删除已装备的技能
    void delSkills(std::string&, bool = true);
    // 更新被动技能表
    bool upPassiveSkill(UInt16 skill, UInt16 type, bool, bool = true);
    // 更新灵侍技能表
    bool upPassiveSkillLingshi();
    // 装备被动技能
    //bool upPassiveSkill(UInt16* skill, UInt8 size, bool = true);
    // 更新被动技能
    bool offPassiveSkill(UInt16 skill, UInt16 type, bool = false, bool = true);

    // 取得所有主动技能
    void getUpSkillAndLevel(std::vector<UInt16>& skills);
    // 根据索引返回被动技能容器
    inline const std::vector<UInt16>& getPassiveSkillByIndex(UInt16 index) { return _passkl[index];}
    // 取得攻击前被动100%触发技能
    inline std::vector<UInt16>& getPassiveSkillPreAtk100() { return _passkl[GData::SKILL_PREATK-GData::SKILL_PASSSTART]; }
    // 取得攻击后被动100%触发技能
    inline std::vector<UInt16>& getPassiveSkillAftAtk100() { return _passkl[GData::SKILL_AFTATK-GData::SKILL_PASSSTART]; }
    // 取得行动后被动100%触发技能（包括被眩晕）
    inline std::vector<UInt16>& getPassiveSkillAftAction100() { return _passkl[GData::SKILL_AFTACTION-GData::SKILL_PASSSTART]; }
    // 取得普通攻击后被动100%触发技能
    inline std::vector<UInt16>& getPassiveSkillAftNAtk100() { return _passkl[GData::SKILL_AFTNATK-GData::SKILL_PASSSTART]; }
    // 取得被攻击后被动100%触发技能
    inline std::vector<UInt16>& getPassiveSkillBeAtk100() { return _passkl[GData::SKILL_BEATKED-GData::SKILL_PASSSTART]; }
    // 取得闪避后被动100%触发技能
    inline std::vector<UInt16>& getPassiveSkillAftEvd100() { return _passkl[GData::SKILL_AFTEVD-GData::SKILL_PASSSTART]; }
    // 取得法术抵抗后被动100%触发技能
    inline std::vector<UInt16>& getPassiveSkillAftRes100() { return _passkl[GData::SKILL_AFTRES-GData::SKILL_PASSSTART]; }
    // 取得入场时概率100%触发技能
    inline std::vector<UInt16>& getPassiveSkillEnter100() { return _passkl[GData::SKILL_ENTER-GData::SKILL_PASSSTART]; }
    // 取得死亡后概率100%触发技能
    inline std::vector<UInt16>& getPassiveSkillDead100() { return _passkl[GData::SKILL_DEAD-GData::SKILL_PASSSTART]; }
    // 取得队友被攻击时100%触发技能
    inline std::vector<UInt16>& getPassiveSkillOnPetProtect100() { return _passkl[GData::SKILL_ONPETPROTECT-GData::SKILL_PASSSTART]; }
    // 取得收到任意伤害时100%触发技能
    inline std::vector<UInt16>& getPassiveSkillOnGetDmg100() { return _passkl[GData::SKILL_ONGETDMG-GData::SKILL_PASSSTART]; }
    // 取得HP变化时100%触发技能
    inline std::vector<UInt16>& getPassiveSkillOnHPChange100() { return _passkl[GData::SKILL_ONHPCHANGE - GData::SKILL_PASSSTART]; }
    // 取得招架时100%触发技能
    inline std::vector<UInt16>& getPassiveSkillOnWithstand100() { return _passkl[GData::SKILL_ONWITHSTAND - GData::SKILL_PASSSTART]; }

    // 根据索引返回被动技能容器(为什么要拆成两部分？跪了……）
    inline const std::vector<UInt16>& getPassiveSkillByIndex2(UInt16 index) { return _rpasskl[index];}
    // 取得攻击前被动概率触发技能
    inline std::vector<UInt16>& getPassiveSkillPreAtk() { return _rpasskl[GData::SKILL_PREATK-GData::SKILL_PASSSTART]; }
    // 取得攻击后被动概率触发技能
    inline std::vector<UInt16>& getPassiveSkillAftAtk() { return _rpasskl[GData::SKILL_AFTATK-GData::SKILL_PASSSTART]; }
    // 取得行动后被动概率触发技能（包括被眩晕）
    inline std::vector<UInt16>& getPassiveSkillAftAction() { return _rpasskl[GData::SKILL_AFTACTION-GData::SKILL_PASSSTART]; }
    // 取得普通攻击后被动概率触发技能
    inline std::vector<UInt16>& getPassiveSkillAftNAtk() { return _rpasskl[GData::SKILL_AFTNATK-GData::SKILL_PASSSTART]; }
    // 取得被攻击后被动概率触发技能
    inline std::vector<UInt16>& getPassiveSkillBeAtk() { return _rpasskl[GData::SKILL_BEATKED-GData::SKILL_PASSSTART]; }
    // 取得闪避后被动概率触发技能
    inline std::vector<UInt16>& getPassiveSkillAftEvd() { return _rpasskl[GData::SKILL_AFTEVD-GData::SKILL_PASSSTART]; }
    // 取得法术抵抗后被动概率触发技能
    inline std::vector<UInt16>& getPassiveSkillAftRes() { return _rpasskl[GData::SKILL_AFTRES-GData::SKILL_PASSSTART]; }
    // 取得入场时概率触发技能
    inline std::vector<UInt16>& getPassiveSkillEnter() { return _rpasskl[GData::SKILL_ENTER-GData::SKILL_PASSSTART]; }
    // 取得死亡后概率触发技能
    inline std::vector<UInt16>& getPassiveSkillDead() { return _rpasskl[GData::SKILL_DEAD-GData::SKILL_PASSSTART]; }
    // 取得队友被攻击时概率触发技能
    inline std::vector<UInt16>& getPassiveSkillOnPetProtect() { return _rpasskl[GData::SKILL_ONPETPROTECT-GData::SKILL_PASSSTART]; }
    // 取得收到任意伤害时概率触发技能
    inline std::vector<UInt16>& getPassiveSkillOnGetDmg() { return _rpasskl[GData::SKILL_ONGETDMG-GData::SKILL_PASSSTART]; }
    // 取得HP变化时概率触发技能
    inline std::vector<UInt16>& getPassiveSkillOnHPChange() { return _rpasskl[GData::SKILL_ONHPCHANGE - GData::SKILL_PASSSTART]; }
    // 取得招架时100%触发技能
    inline std::vector<UInt16>& getPassiveSkillOnWithstand() { return _rpasskl[GData::SKILL_ONWITHSTAND - GData::SKILL_PASSSTART]; }
    // 取得对方获得封印沉默触发的技能
    inline std::vector<UInt16>& getPassiveSkillOnOtherConfuseForget() { return _rpasskl[GData::SKILL_ONOTHERCONFUSEFORGET - GData::SKILL_PASSSTART]; }

    // 神农宝鼎
    inline std::vector<UInt16>& getPassiveSkillOnTherapy() { return _passkl[GData::SKILL_ONTHERAPY-GData::SKILL_PASSSTART]; }
    // 轩辕神剑
    inline std::vector<UInt16>& getPassiveSkillOnSkillDmg() { return _passkl[GData::SKILL_ONSKILLDMG-GData::SKILL_PASSSTART]; }
    // 五彩元石
    inline std::vector<UInt16>& getPassiveSkillOnOtherDead() { return _passkl[GData::SKILL_ONOTHERDEAD-GData::SKILL_PASSSTART]; }

    // 反击后触发的技能
    inline std::vector<UInt16>& getPassiveSkillOnCounter() { return _rpasskl[GData::SKILL_ONCOUNTER-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnCounter100() { return _passkl[GData::SKILL_ONCOUNTER-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnAttackBleed100() { return _passkl[GData::SKILL_ONATKBLEED-GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillOnAtkDmg() {return _rpasskl[GData::SKILL_ONATKDMG-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnAtkDmg100() { return _passkl[GData::SKILL_ONATKDMG - GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillOnBeDmg100() { return _passkl[GData::SKILL_ONBEDMG - GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnBeDmg() { return _rpasskl[GData::SKILL_ONBEDMG - GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnBePHYDmg100() { return _passkl[GData::SKILL_ONBEPHYDMG - GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnBePHYDmg() { return _rpasskl[GData::SKILL_ONBEPHYDMG - GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnBeMagDmg100() { return _passkl[GData::SKILL_ONBEMAGDMG - GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnBeMagDmg() { return _rpasskl[GData::SKILL_ONBEMAGDMG - GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnHP10P100() { return _passkl[GData::SKILL_ONHP10P - GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnHP10P() { return _rpasskl[GData::SKILL_ONHP10P - GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillDeadFake() { return _rpasskl[GData::SKILL_DEAD_FAKE-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillDeadFake100() { return _passkl[GData::SKILL_DEAD_FAKE-GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillAbnormalTypeDmg100() { return _passkl[GData::SKILL_ABNORMAL_TYPE_DMG-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillBleedTypeDmg100() { return _passkl[GData::SKILL_BLEED_TYPE_DMG-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillBleedTypeDmg() { return _rpasskl[GData::SKILL_BLEED_TYPE_DMG-GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillXMCZ100() { return _passkl[GData::SKILL_XMCZ-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillBLTY100() { return _passkl[GData::SKILL_BLTY-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillViolent100() { return _passkl[GData::SKILL_VIOLENT-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillRevival100() { return _passkl[GData::SKILL_REVIVAL-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnOtherConfuseForget100() { return _passkl[GData::SKILL_ONOTHERCONFUSEFORGET-GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillByLingshi(UInt8 type) { return _rpassklLingshi[type-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillByLingshi100(UInt8 type) { return _passklLingshi[type-GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillOnAttackConfuseForget() { return _rpasskl[GData::SKILL_ONATKCONFUSEFORGET-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnAttackConfuseForget100() { return _passkl[GData::SKILL_ONATKCONFUSEFORGET-GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillOnAttackStun() { return _rpasskl[GData::SKILL_ONATKSTUN-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnAttackStun100() { return _passkl[GData::SKILL_ONATKSTUN-GData::SKILL_PASSSTART]; }

    inline std::vector<UInt16>& getPassiveSkillOnAttackBlind() { return _rpasskl[GData::SKILL_ONATKBLIND-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillOnAttackBlind100() { return _passkl[GData::SKILL_ONATKBLIND-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillCondition100() { return _passkl[GData::SKILL_CONDITION-GData::SKILL_PASSSTART]; }
    inline std::vector<UInt16>& getPassiveSkillEvolution100() { return _passkl[GData::SKILL_EVOLUTION-GData::SKILL_PASSSTART]; }
    // 取得心法带出技能的ID表
    const std::vector<const GData::SkillBase*>& skillFromCitta(UInt16 citta);

    // 是否可散功心法
    bool canCittaBeDel(UInt16 citta);
    // 初始化装备的心法
    void setUpCittas(std::string& citta, bool = false);
    // 初始化可装备的心法
    void setCittas(std::string& cittas, bool = true);
    // 装备心法
    bool upCitta(UInt16 citta, int idx, bool = true, bool = false, bool = true);
    // 卸下心法
    bool offCitta(UInt16 citta, bool = false, bool = false, bool = true);
    // 卸下所有心法
    void offAllCitta();
    // 增加一个心法
    bool addNewCitta(UInt16 citta, bool = true, bool = false, bool = false);
    // 删除所有心法
    void delAllCitta( bool = true);
    // 删除一个心法
    bool delCitta(UInt16 citta, bool = true, bool = false);
    //是否可以删除某个心法
    bool CanDelCitta(UInt16 citta);
    // 是否学会了此心法
    int hasCitta(UInt16 citta);
    // 是否装备了此心法
    int isCittaUp(UInt16 citta);
    // 升级心法
    bool lvlUpCitta(UInt16 citta, bool = true);
    // 取得装备的心法数
    UInt8 getUpCittasNum();
    // 取得最大装备心法数
    inline UInt8 getUpCittasMax() { return _cittaslot <= CITTA_UPMAX ? _cittaslot : CITTA_UPMAX; }
    // 取得装备位置idx处所装备的心法的ID
	inline UInt16 getUpCitta(int idx = 0) { return (idx >= 0 && idx < getUpCittasMax() ) ? CITTA_ID(_citta[idx]) : 0; }
    // 取得装备位置idx处所装备的心法等级
	inline UInt8 getUpCittaLevel(int idx = 0) { return (idx >= 0 && idx < getUpCittasMax() ) ? CITTA_LEVEL(_citta[idx]) : 0; }
    // 取得装备位置idx处所装备的心法的ID和等级
	inline UInt16 getUpCittaAndLevel(int idx = 0) { return (idx >= 0 && idx < getUpCittasMax()) ? _citta[idx] : 0; }
    // 取得默认心法
	inline UInt16 getDefaultCittaAndLevel() { return getUpCittaAndLevel(0); }
    // 取得可装备的心法数
    UInt8 getCittasNum();
    // 取得所有装备的心法和等级
    void getAllUpCittaAndLevel(Stream& st);
    // 取得所有学习的心法和等级
    void getAllCittasAndLevel(Stream& st);
    // 取得装备了的和学习了的心法和等级
    void getAllCittaAndLevel(Stream& st);
    // 取得所有学会的无双技能
    void getAllPeerlessAndLevel(Stream& st);
    const std::vector<UInt16>& getCittas() const { return _cittas; }

    // 取得可装备的无双技能
    void getAllPeerless(Stream& st);
    // 取得可装备的无双技能的个数
    inline UInt8 getPeerlessNum() { return _peerless.size(); }

    void addSkillsFromCT(const std::vector<const GData::SkillBase*>& skills, bool = true, bool = false, bool = true);
    void delSkillsFromCT(const std::vector<const GData::SkillBase*>& skills, bool = true);

	inline ItemInnateTrump* getInnateTrump() { return _innateTrump; }
	inline ItemHalo* getHalo() { return _halo; }
	inline ItemFashion* getFashion() { return _fashion; }
	inline ItemWeapon * getWeapon() { return _weapon; }
	inline ItemArmor * getArmor(int idx) { return (idx >= 0 && idx < 5) ? _armor[idx] : NULL; }
    inline void * getArmorAddr() { return _armor;}
	inline ItemEquip * getAmulet() { return _amulet; }
	inline ItemEquip * getRing() { return _ring; }

	inline ItemEquip * getTrump(int idx) { return (idx >= 0 && idx < getMaxTrumps()) ? _trump[idx] : 0; }
    inline void * getTrumpAddr() { return _trump;}
    inline UInt8 getMaxTrumps () { return TRUMP_UPMAX; }
    UInt32 getTrumpId(int idx);
    int getAllTrumpId(UInt32* trumps, int size = TRUMP_UPMAX);
    int getAllTrumpTypeId(UInt32* ids, int size = TRUMP_UPMAX);
    int getAllTrumpEnchant(UInt8* trumps, int size = TRUMP_UPMAX);
    void getAllTrumps(Stream& st);
    UInt32 getTrumpNum();

	inline ItemEquip * getLingbao(int idx) { return (idx >= 0 && idx < getMaxLingbaos()) ? _lingbao[idx] : 0; }
    inline void * getLingbaoAddr() { return _lingbao;}
    inline UInt8 getMaxLingbaos() { return e_lb_max; }
    UInt32 getLingbaoId(int idx);
    int getAllLingbaoId(UInt32* lingbaos, int size = e_lb_max);
    int getAllLingbaoTypeId(UInt32* ids, int size = e_lb_max);
    void getAllLingbaos(Stream& st);
    UInt32 getLingbaoNum();

    inline std::vector<LBSkill>& getLBSkill() { return _lbSkill; }
    ItemEquip* setLingbao( UInt8 idx , ItemEquip* lingbao, bool = true);
    void loadLingbao(std::string& lb);
    void loadEvolutionEquip(std::string& ee);
    void loadLBSkill(std::string& lbSkill);
    bool addLBSkill(UInt32 lbid, UInt16 skillid, UInt16 factor);
    bool delLBSkill(UInt32 lbid);
    void getAllLbSkills(Stream& st);



	UInt32 getMaxHP();

	UInt32 regenHP(UInt32);
	bool addExp(UInt64, UInt32 = 0, bool = true);
	bool addPExp(Int32, bool = true, bool = false, UInt32 = 0);
    void flushExp();

	void sendModification(UInt8 t, UInt64 v);
	void sendModification2(UInt8 t, UInt64 v, bool = true);
	void sendMaxSoul();
	void sendModification(UInt8 n, UInt8 * t, UInt64 * v, bool = true);
	void sendModification(UInt8 t, ItemEquip * v, bool = true);
	void sendModification(UInt8 n, UInt8 * t, ItemEquip ** v, bool = true);

    void sendModificationAcupoints(UInt8 t, int idx, bool = true);
    void sendModificationAcupointsGold(UInt8 t, int idx, bool = true);

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

	UInt32 getInnateTrumpId();
	UInt32 getHaloId();
	UInt32 getFashionId();
	UInt32 getWeaponId();
	UInt32 getArmorId(int idx);
	UInt32 getAmuletId();
	UInt32 getRingId();

	UInt32 getInnateTrumpTypeId();
	UInt32 getHaloTypeId();
	UInt32 getFashionTypeId();
    UInt16 getPortrait();

	UInt16 getWeaponAttack();
	void getArmorDefendAndHP(UInt16& def, UInt16& hp);
    inline void setColor(UInt8 color) { _color = color; }
	inline UInt8 getColor() { return _color; }
	static UInt8 getColor2(float);

    void setSummoned(UInt32 summoned) { _summoned = summoned; }
    UInt32 getSummoned() { return _summoned; }
	ItemEquip * setInnateTrump(ItemInnateTrump* r, bool = true);
	ItemEquip * setHalo(ItemHalo* r, bool = true);
	ItemEquip * setFashion(ItemFashion* r, bool = true);
	ItemWeapon * setWeapon(ItemWeapon * w, bool = true);
	ItemArmor * setArmor(int idx, ItemArmor * a, bool = true);
	ItemEquip * setAmulet(ItemEquip * a, bool = true);
	ItemEquip * setRing(ItemEquip * r, bool = true);
	ItemEquip ** setTrump(std::string& trumps, bool = true);
    ItemEquip* setTrump( UInt32 trump, int idx, bool = true);
    ItemEquip* setTrump( ItemEquip* trump, int idx, bool = true);
    UInt8 getLingshiNum();
    int getAllLingshiId( UInt32* lingshis, int size = LINGSHI_UPMAX);
	inline ItemEquip * getLingshi(int idx) { return (idx >= 0 && idx < LINGSHI_UPMAX) ? _lingshi[idx] : 0; }
    ItemEquip * setLingshi(ItemEquip *);
	ItemEquip ** setLingshi(std::string&, bool = true);
    ItemEquip * setLingshi(ItemEquip *, int idx, bool = true);
    void updateLingshiSkillId(ItemEquip *, UInt8);
	ItemEquip * findEquip(UInt32 id, UInt8& pos);
	ItemEquip * findfashion(UInt32 id);
	void findTrumpByTypeId(std::vector<ItemEquip*>& ret, UInt32 id);
	void removeEquip(UInt8 pos, ItemEquip * equip, UInt8 toWhere = 0);
	inline void setDirty(bool d = true) { _attrDirty = d; _bPDirty = d; }
    bool hasTrumpType(UInt32 trumpid);
    bool canSetTrump(UInt8 idx, UInt32 trumpid);


    void setAttrType1(UInt8 t);
    void setAttrValue1(UInt16 v);
    void setAttrType2(UInt8 t, bool force=false);
    void setAttrValue2(UInt16 v, bool force=false);
    void setAttrType3(UInt8 t, bool force=false);
    void setAttrValue3(UInt16 v, bool force=false);
    UInt8 getAttrType1(bool = false);
    UInt16 getAttrValue1(bool = false);
    UInt8 getAttrType2(bool = false);
    UInt16 getAttrValue2(bool = false);
    UInt8 getAttrType3(bool = false);
    UInt16 getAttrValue3(bool = false);

    UInt8 forge(UInt8 which, UInt8 lock = 0, bool = false);
    void updateForgeAttr(bool = true);
    void broadcastForge(UInt8);

    static inline bool isMainFighter(UInt32 id) { return id > 0 && id < 10; }
    inline bool isMainFighter() { return isMainFighter(_id); }

    void getAllSSAndLevel(Stream& st);
    void getAllSSAndLevelOfPet(Stream& st);

	inline void setId(UInt32 id) { _id = id; } //改性卡使用

public:
	inline const GData::AttrExtra * getAttrExtraEquip() { checkDirty(); return &_attrExtraEquip; }
	inline UInt16 getExtraStrength() { checkDirty(); return _attrExtraEquip.strength; }
	inline UInt16 getExtraPhysique() { checkDirty(); return _attrExtraEquip.physique; }
	inline UInt16 getExtraAgility() { checkDirty(); return _attrExtraEquip.agility; }
	inline UInt16 getExtraIntelligence() { checkDirty(); return _attrExtraEquip.intelligence; }
	inline UInt16 getExtraWill() { checkDirty(); return _attrExtraEquip.will; }
	inline UInt16 getExtraSoul() { checkDirty(); return _attrExtraEquip.soul; }
	inline UInt16 getExtraAura() { checkDirty(); return _attrExtraEquip.aura; }
	inline UInt16 getExtraAuraMax() { checkDirty(); return _attrExtraEquip.auraMax; }
	inline float getExtraTough() { checkDirty(); return _attrExtraEquip.tough; }
	inline float getExtraStrengthP() { checkDirty(); return _attrExtraEquip.strengthP; }
	inline float getExtraPhysiqueP() { checkDirty(); return _attrExtraEquip.physiqueP; }
	inline float getExtraAgilityP() { checkDirty(); return _attrExtraEquip.agilityP; }
	inline float getExtraIntelligenceP() { checkDirty(); return _attrExtraEquip.intelligenceP; }
	inline float getExtraWillP() { checkDirty(); return _attrExtraEquip.willP; }
	inline float getExtraSoulP() { checkDirty(); return _attrExtraEquip.soulP; }
	inline float getExtraAuraP() { checkDirty(); return _attrExtraEquip.auraP; }
	inline float getExtraAuraMaxP() { checkDirty(); return _attrExtraEquip.auraMaxP; }
	inline Int32 getExtraAttack() { checkDirty(); return _attrExtraEquip.attack; }
	inline float getExtraAttackP() { checkDirty(); return _attrExtraEquip.attackP; }
	inline Int32 getExtraMagAttack() { checkDirty(); return _attrExtraEquip.magatk; }
	inline float getExtraMagAttackP() { checkDirty(); return _attrExtraEquip.magatkP; }
	inline Int32 getExtraDefend() { checkDirty(); return _attrExtraEquip.defend; }
	inline float getExtraDefendP() { checkDirty(); return _attrExtraEquip.defendP; }
	inline Int32 getExtraMagDefend() { checkDirty(); return _attrExtraEquip.magdef; }
	inline float getExtraMagDefendP() { checkDirty(); return _attrExtraEquip.magdefP; }
	inline UInt32 getExtraHP() { checkDirty(); return _attrExtraEquip.hp; }
	inline float getExtraHPP() { checkDirty(); return _attrExtraEquip.hpP; }
	inline float getExtraAction() { checkDirty(); return _attrExtraEquip.action; }
	inline float getExtraActionP() { checkDirty(); return _attrExtraEquip.actionP; }
	inline float getExtraHitrate() { checkDirty(); return _attrExtraEquip.hitrate; }
	inline float getExtraEvade() { checkDirty(); return _attrExtraEquip.evade; }
	inline float getExtraCritical() { checkDirty(); return _attrExtraEquip.critical; }
	inline float getExtraCriticalDmg() { checkDirty(); return _attrExtraEquip.criticaldmg; }
	inline float getExtraPierce() { checkDirty(); return _attrExtraEquip.pierce; }
	inline float getExtraCounter() { checkDirty(); return _attrExtraEquip.counter; }
	inline float getExtraMagRes() { checkDirty(); return _attrExtraEquip.magres; }
	inline float getBattlePoint() { 
        checkBPDirty();
        //std::cout << "id:" <<static_cast<UInt32>(getId());
        //std::cout << " battlePoint:" << _battlePoint << " skillPoint:" << _skillBP << std::endl;
        return _battlePoint + _skillBP; 
    }

	inline float getBattlePoint_Dirty() { return _battlePoint + _skillBP; }

    inline float getExtraHitrateLevel() { checkDirty(); return _attrExtraEquip.hitrlvl; }
    inline float getExtraEvadeLevel() { checkDirty(); return _attrExtraEquip.evdlvl; }
    inline float getExtraCriticalLevel() { checkDirty(); return _attrExtraEquip.crilvl; }
    inline float getExtraPierceLevel() { checkDirty(); return _attrExtraEquip.pirlvl; }
    inline float getExtraCounterLevel() { checkDirty(); return _attrExtraEquip.counterlvl; }
    inline float getExtraToughLevel() { checkDirty(); return _attrExtraEquip.toughlvl; }
    inline float getExtraMagResLevel() { checkDirty(); return _attrExtraEquip.mreslvl; }
    inline float getExtraCriticalDmgImmune() { checkDirty(); return _attrExtraEquip.criticaldmgimmune; }

    inline float getExtraFairyAtk(){ checkDirty(); return _attrExtraEquip.fairyAck;}
    inline float getExtraFairyDef(){ checkDirty(); return _attrExtraEquip.fairyDef;}

public:
    inline void setExtraAttack(Int32 atk) { setDirty(true); _wbextatk = atk; }
	inline void setExtraMagAttack(Int32 atk) { setDirty(true); _wbextmagatk = atk; }
    inline void setAttrExtraEquip(const GData::AttrExtra& other){ _attrExtraEquip += other; }
    inline void resetAttrExtraEquip(){setDirty(true); _attrExtraEquip.reset();}
    inline void resetAttrExtraEquip2(){setDirty(false); _attrExtraEquip.reset();}
    
    inline void setPlExtraAttack(Int32 atk) { _wbplextatk = atk; }
	inline void setPlExtraMagAttack(Int32 atk) { _wbplextmagatk = atk; }
    inline void setPlCBBExtraAttack(Int32 atk) { _cbbplextatk = atk; }
	inline void setPlCBBExtraMagAttack(Int32 atk) { _cbbplextmagatk = atk; }
    
    inline void setPlDDExtraAttack(Int32 atk) { _ddplextatk = atk; }
	inline void setPlDDExtraMagAttack(Int32 atk) { _ddplextmagatk = atk; }
	inline void setPlDDExtraDef(Int32 def) { _ddplextdef = def; }
	inline void setPlDDExtraAction(Int32 action) { _ddplextaction = action; }
	inline void setPlDDExtraCritical(Int32 critical) { _ddplextcritical = critical; }
	inline void setPlDDExtraHp(Int32 hp) { _ddplexthp = hp; }

    UInt8 getToggleReiatsu();        // 返回出场所需灵压
    UInt8 getTargetPos();            // 返回备胎该出场的目标位置

public:
	inline Int16 getBaseStrength()
    {
        return strength + _elixirattr.strength;
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
        return physique + _elixirattr.physique;
        if (_id > 9)
            return physique;
        static const Int16 add[9][9] = {{0}, {0}, {0}, {40, 40, 15, 15, 15, 15}, {90, 90, 30, 30, 35, 35}, {140, 140, 45, 45, 55, 55}};
        return physique + add[_color][_class];
    }

	inline Int16 getBaseAgility()
    {
        return agility + _elixirattr.agility;
        if (_id > 9)
            return agility;
        static const Int16 add[9][9] = {{0}, {0}, {0}, {15, 15, 40, 40, 15, 15}, {30, 30, 90, 90, 30, 30}, {45, 45, 140, 140, 45, 45}};
        return agility + add[_color][_class];
    }

	inline Int16 getBaseIntelligence()
    {
        return intelligence + _elixirattr.intelligence;
        if (_id > 9)
            return intelligence;
        static const Int16 add[9][9] = {{0}, {0}, {0}, {10, 10, 10, 10, 10, 10}, {25, 25, 25, 25, 25, 25}, {40, 40, 40, 40, 40, 40}};
        return intelligence + add[_color][_class];
    }

	inline Int16 getBaseWill() { return will + _elixirattr.will; }
	inline Int16 getBaseAura() { return aura; }
	inline Int16 getBaseAuraMax() { return auraMax; }
	inline float getBaseTough() { return tough + _elixirattr.tough * 0.001f * 100; }

	inline Int32 getBaseAttack() { return attack + _elixirattr.attack; }
	inline Int32 getBaseMagAttack() { return magatk + _elixirattr.attack; }
	inline Int32 getBaseDefend() { return defend + _elixirattr.defend * 10; }
	inline Int32 getBaseMagDefend() { return magdef + _elixirattr.defend * 10; }
	inline float getBaseHitrate() { return hitrate; }

	inline Int16 getBaseSoul() { return baseSoul; }
    inline Int16 getSoul() { return soul; }
    Int16 getMaxSoul();
    Int16 get2ndSounSoulMax();

	inline float getBaseEvade()
    {
        return evade + _elixirattr.evade * 0.001f * 100;
    }

	inline float getBaseCritical()
    {
        return critical + _elixirattr.critical * 0.001f * 100;
    }

    inline float getBaseCriticalDmg()
    {
        return criticaldmg;
    }

	inline float getBasePierce()
    {
        return pierce + _elixirattr.pierce * 0.001f * 100;
    }

	inline float getBaseCounter()
    {
        return counter + _elixirattr.counter * 0.001f * 100;
    }

    inline float getBaseMagRes() { return magres; }
    inline void setBaseHP(UInt32 hp) { maxhp = hp; }
	inline UInt32 getBaseHP() { return maxhp; }
	inline UInt32 getBaseAction() { return action + _elixirattr.action; }

    Int32 getAcuPraAdd() { return _praadd; }
    float getPracticeBufFactor();
    float getPracticeIncByDiamond();
    float getPracticeIncByQQVip();
    float getClanTechAddon(int place);
    float getPracticeInc();
    float getBasePExpEach();
    float getPExpNoBuf();
    // TODO:
    UInt16 getPracticePlace();
    bool isGoldPractice() { return false; }

	void addAttr( ItemEquip *,UInt8 flag = 0 );
	void addTrumpAttr( ItemEquip* );
	void addLingbaoAttr( ItemEquip* );
    void addAttr( const GData::CittaEffect* ce );
    void    CheckEquipEnchantAttainment(UInt8 e);
    bool  IsEquipEnchantLev(UInt8 e);

    void getAttrExtraEquip(Stream& st);
protected:
    void addAttrExtra( GData::AttrExtra& ae, const GData::AttrExtra * ext ,UInt8 flag = 0 );
    void addAttrExtra( GData::AttrExtra& ae, const GData::CittaEffect* ce ,UInt8 flag = 0);
    void addAttrExtraGem( GData::AttrExtra& ae, GData::ItemGemType * igt );
    void addAttrExtraXCGem( GData::AttrExtra& ae, GData::ItemGemType * igt );
    void addLingshiAttr( ItemEquip* );
	virtual void rebuildEquipAttr();
	void rebuildBattlePoint();
	void rebuildSkillBattlePoint();
	inline void checkDirty()
	{
		if(_attrDirty)
		{
			_attrDirty = false;
			rebuildEquipAttr();
		}
	}
	void checkBPDirty();

    template <typename T>
    bool value2string(T* values, int size, std::string& str)
    {
        if (!values || !size)
            return true; // XXX: will be set to ''

        char buf[1024] = {0};
        char* pbuf = buf;
        char* pend = &buf[sizeof(buf)-1];
        for (int i = 0; i < size; ++i)
        {
            pbuf += snprintf(pbuf, pend - pbuf, "%u", values[i]);
            if (i < size - 1)
                pbuf += snprintf(pbuf, pend - pbuf, ",");
        }

        if (pbuf != buf)
            str = buf;
        return true;
    }

    void isCanStrengthenSuit(UInt32 * setId, UInt32 * setNum, Fighter * fgt);
protected:
	UInt32 _id;

	Player * _owner;

	std::string _name;
    UInt8 _sex;         // 性别
	UInt8 _class;
	UInt8 _level;
	UInt64 _exp;        // 经验
    UInt32 _pexp;       // 修炼经验
    UInt32 _pexpAddTmp; // for Attainment
    UInt32 _pexpMax;    // 修炼最大经验
	float _potential;   // 潜力
	float _capacity;    // 资质
	UInt8 _color;
    UInt32 _summoned;
	UInt32 _hp;

    UInt8 _acupoints[ACUPOINTS_MAX];    // 穴道
    UInt8 _acupointsGold[ACUPOINTSGOLD_MAX];    // 本命金丹

    UInt16 _skill[SKILL_UPMAX];     // 装备的技能 _skill[i] % SKILL_LEVEL_MAX => skilllevel, _skill[i]/SKILL_LEVEL_MAX=> skillid
    std::vector<UInt16> _skills;    // 可装备的技能 TODO: 如果所有技能都将是由心法带出,则数据表里不需要存这个字段

    UInt8 _cittaslot;               // 可装备心法最大数
    UInt16 _citta[CITTA_UPMAX];     // 装备的心法
    std::vector<UInt16> _cittas;    // 可装备的心法

    std::vector<UInt16> _peerless;  // 可装备的无双技能

    // 被动触发技能, 分摊概率触发, XXX: 注意装备和删除心法或法宝时需更新
    std::vector<UInt16> _rpasskl[GData::SKILL_PASSIVES-GData::SKILL_PASSSTART]; // 概率触发被动技能
    std::vector<UInt16> _passkl[GData::SKILL_PASSIVES-GData::SKILL_PASSSTART]; // 100%触发技能

    std::vector<UInt16> _rpassklLingshi[GData::SKILL_PASSIVES-GData::SKILL_PASSSTART];  // 概率触发被动技能(灵侍变身状态下才会触发)
    std::vector<UInt16> _passklLingshi[GData::SKILL_PASSIVES-GData::SKILL_PASSSTART]; // 100%触发技能(灵侍变身状态下才会触发)

    std::vector<LBSkill> _lbSkill;

	ItemHalo* _halo;
	ItemFashion* _fashion;
	ItemWeapon * _weapon;
	ItemArmor * _armor[5];
	ItemEquip * _ring;
	ItemEquip * _amulet;
	ItemEquip * _trump[TRUMP_UPMAX];    // 法宝
	ItemEquip * _lingbao[e_lb_max];// 灵宝
    ItemInnateTrump * _innateTrump; // 先天法宝
	ItemEquip * _lingshi[LINGSHI_UPMAX];    // 灵侍

	bool _attrDirty;
	UInt32 _maxHP;
	bool _bPDirty;
	bool _skillBPDirty;
    UInt16 _expMods;
    UInt32 _expEnd;
    UInt16 _pexpMods;
    bool _forceWrite;
	float _battlePoint;
	float _skillBP;
	GData::AttrExtra _attrExtraEquip;

	UInt32 _buffData[FIGHTER_BUFF_COUNT];

    Int32 _praadd;  // 穴道对修为增长的加成
    float _powerUp;

    // 天赋属性
    UInt8 _attrType1;
    UInt16 _attrValue1;
    UInt8 _attrType2;
    UInt16 _attrValue2;
    UInt8 _attrType3;
    UInt16 _attrValue3;

    SecondSoul* m_2ndSoul;

    //是否隐藏时装
    bool _hideFashion;

public:
	float getSoulPracticeAddOn();
	float getSoulPracticeFactor();
    bool openSecondSoul(UInt8 cls);
    void setSecondSoul(SecondSoul* sedondSoul);
    UInt8 getSoulExtraAura();
    UInt8 getSoulAuraLeft();
    UInt16 getSoulSkillSoulOut();
    UInt16 getSoulSkillProtect();

    bool practiceLevelUp();
    bool changeSecondSoulClass(UInt8 cls);
    bool changeSecondSoulXinxiu(UInt8 xinxiu);

    bool enchantSoul(UInt32 itemId, bool bind, std::vector<SoulItemExp>& soulItemExpOut);
    bool equipSoulSkill(UInt8 idx, UInt32 itemId, bool bind=true);

    void send2ndSoulInfo();

    UInt8 getSoulSkillIdx(UInt16 itemId);
    void reload2ndSoul();
    void setSoulLevel(UInt32 level);
    void resetLevelAndExp(UInt8 maxLevel);

    void setHideFashion(bool v, bool writedb = true) 
    {
        _hideFashion = v;
        UInt8 cmd = 0x33;
        UInt64 value = v;
        sendModification(1, &cmd, &value, writedb);
    };
    SecondSoul* getSecondSoul() {return m_2ndSoul;}
    bool getHideFashion() {return _hideFashion;};
public:
    inline void setElixirAttr(ElixirAttr& attr) { _elixirattr = attr; }
    bool addElixirAttrByOffset(UInt8 off, Int32 v);
    Int32 getElixirAttrByOffset(UInt8 off);
    void appendElixirAttr(Stream& st);
    void appendElixirAttr2(Stream& st);
    ElixirAttr& getElixirAttr() { return _elixirattr; }


private:
    ElixirAttr _elixirattr;

private:
    bool _iswboss;
    bool _iswbossinspire;
    bool _iscbbbuf;
    bool _isddbuf;
    Int32 _wbextatk;
    Int32 _wbextmagatk;
    Int32 _wbplextatk;
    Int32 _wbplextmagatk;
    Int32 _cbbplextatk;
    Int32 _cbbplextmagatk;
    Int32 _ddplextatk;
    Int32 _ddplextmagatk;
    Int32 _ddplextdef;
    Int32 _ddplextaction;
    Int32 _ddplextcritical;
    Int32 _ddplexthp;

public:
    // 仅仅用于内存拷贝出来的Fighter, 切勿她用
    void setUpSS(std::string& skillstrengthen);
    inline void setSoulMax(Int32 v) { _soulMax = v; }
    inline void setSoulExtraAura(Int32 v) { _soulExtraAura = v; }
    inline void setSoulAuraLeft(Int32 v) { _soulAuraLeft = v; }
    inline void setSoulSkillSoulOut(Int32 v) { _soulSkillSoulOut = v; }
    inline void setUpCittasMax() { _cittaslot = CITTA_UPMAX; }
    bool upCittaWithOutCheck( UInt16 citta, int idx );
    inline void setSoulSkillProtect(Int32 v) { _soulSkillProtect = v; }
    UInt16 getTrumpSkill(int i) { if(i >= TRUMP_UPMAX) return 0; else return _trumpSkill[i]; }
    std::vector<UInt16>& getLingshiSkill(int i) { if(i >= LINGSHI_UPMAX) i = 0; return _lingshiSkill[i]; }
    Int32 _soulMax;
    UInt8 _soulExtraAura;
    UInt8 _soulAuraLeft;
    UInt16 _soulSkillSoulOut;
    UInt16 _trumpSkill[TRUMP_UPMAX];
    UInt16 _soulSkillProtect;
    std::vector<UInt16> _lingshiSkill[LINGSHI_UPMAX];

    // 内丹系统
public:
    UInt8 SSGetLvl(UInt16 skillid);
    SStrengthen* SSGetInfo(UInt16 skillid);
    void SSOpen(UInt16 id);
    UInt8 SSUpgrade(UInt16 id, UInt32 itemId, UInt16 itemNum, bool bind = false);
    void SSDismiss(UInt16, bool = false, Mail * = NULL);
    void SSDismissAll(bool = false);
    void SSErase(UInt16 id);
    void SSUpdate2DB(UInt16 id, SStrengthen& ss);
    void SSDeleteDB(UInt16 id);
    void SSFromDB(UInt16 id, SStrengthen& ss);
    void SSSendSSInfo(UInt16 skill);
    void SSNotify(UInt16 id, SStrengthen& ss);
    void makeFighterSSInfo(Stream& st);
    void makeFighterSSInfoWithNoSkill(Stream& st);
    bool appendFighterSSInfo(Stream& st, UInt16 skillid);
    bool appendFighterSSInfo(Stream& st, UInt16 skillid, SStrengthen* ss);
    void PeerlessSSNotify(UInt16 id, bool writedb);

    UInt16 calcSkillBattlePoint(UInt16 skillId, UInt8 type);

    UInt32 calcLingbaoBattlePoint();
    UInt32 recalcLingbao();
    void eraseLingbaoInfo(ItemEquip* eqiup);
    void pushLingbaoInfo(ItemEquip* equip);
private:
    std::map<UInt16, SStrengthen> m_ss;
    std::map<UInt16, SGrade> m_sg; //技能升阶

public:
	UInt32 favor;
	UInt32 reqFriendliness;
	Int32 strength;
	Int32 physique;
	Int32 agility;
	Int32 intelligence;
    Int32 will;
    Int32 soulMax;  // 最大元神力,穴道会增加元神力上限
    Int32 soul;     // 已使用元神力,装备心法会消耗元神力
    Int32 baseSoul; // 初始元神力
    Int32 aura;     // 当前灵气
    Int32 auraMax;  // 最大灵气
    float tough;
	Int32 attack;
	Int32 magatk;
	Int32 defend;
	Int32 magdef;
	Int32 maxhp;
	UInt32 action;
    UInt16 peerless;// 装备的无双技能
    UInt16 talent;  // 天赋
	float hitrate;
	float evade;
	float critical;
	float criticaldmg;
	float pierce;
	float counter;
	float magres;

	struct Offset { Int8 x, y; };
	std::vector<Offset> extraPos;

    //分别计算散仙的战斗力
public:
	inline GData::AttrExtra& getAttrExtraEquip1() { return _attrExtraEquip; }
    UInt32 calcBaseBattlePoint();   //基础
    UInt32 calcEquipBattlePoint();  //装备与法宝
    UInt32 calcSkillBattlePoint();  //技能
    UInt32 calcCittaBattlePoint();  //心法
    UInt32 calc2ndSoulBattlePoint();  //第二元神
    UInt32 calcClanBattlePoint();  //帮派
    UInt32 calcLingbaoBattlePoint1();  //宝具
    UInt32 calcFormBattlePoint();  //阵法
    // 仙宠
public:
    inline bool isPet() { return getClass() >= e_cls_qinglong && getClass() <= e_cls_xuanwu; }
    UInt8 getPassklNum();
    UInt8 getRpassklNum();
    void updateToDBPetSkill();

    //镇封星辰图
private:
    Xingchenzhen m_xingchen;
    XinMoData m_xinmo;
    UInt8 lingbaoLevel[3];
    UInt32 lingbaoFall[3];  //祝福值
    Evolution * _evl;
    UInt32 _incense;
public:
    inline Xingchenzhen& getXingchen() { return m_xingchen; }
    inline UInt8 getXingchenLvl()  {return m_xingchen.lvl;}

    Evolution * getEvolution();
    inline UInt8 getLingbaoLevel(UInt8 type) { if (type > 2) return 0; return lingbaoLevel[type];}
    inline void setLingbaoLevel(UInt8 type ,UInt8 val){ if(type > 2) return; lingbaoLevel[type] = val; }

    inline UInt32 getLingbaoFall(UInt8 type) { if(type > 2) return 0; return lingbaoFall[type];}
    inline void setLingbaoFall(UInt8 type , UInt32 val){ if(type > 2) return; lingbaoFall[type] = val; }

    inline UInt32 getIncense(){ return _incense;}
    inline void setIncense(UInt32 incense){ _incense = incense;}
    void addIncense(UInt32 count) { _incense+=count; }
    void UpdateIncenseToDB();

    inline XinMoData getXinMo() { return m_xinmo; }
    void setXinMo(UInt8 lev , UInt32 val);

    void updateLingbaoLevelToDB(UInt8 type) ;
    void updateLingbaoFallToDB(UInt8 type) ;

    void setXingchenFromDB(DBXingchen&);
    bool upgradeXingchen(UInt8 type);
    void updateDBxingchen();
    void sendXingchenInfo(UInt8 type);
    void setGem(UInt16 gemId,UInt8 bind, UInt8 pos, UInt8 type);
    void dismantleGem(UInt8 pos, UInt8 type);
    UInt32 exchangeXingchenValue(UInt16 zqId, UInt32 zqCount, UInt8 bind);
    bool IsCanSetGem(ItemBase *item, UInt8 pos);
    void dismissXingchen();
    bool quickUpGrade(UInt8 type);
    void xingchenInfo(Stream & st);
    void tunShiXingKong();
    void tuPoJieXian();
    void GMSetXZLvl(UInt8 lvl);
    void GMSetXCTCurVal(UInt16 value);
    void GMSetXCTMaxVal(UInt16 value);

    void pushPetInfo2Leaderboard();
    void petSSAdd(UInt16 id);
    void petSSErase(UInt16 id);

    void updateDBxinmo();
    bool upgradeXinMo();
    bool quickUpGradeXinMo();
    void dismissXinMo();

    void SGradeManual(UInt16 skillId);
    void SGradeAuto(UInt16 skillId);
    void makeFighterSGInfo(Stream& st);
    SGrade* SGGetInfo(UInt16 skillid);
    bool appendFighterSGInfo(Stream& st, UInt16 skillid);
    bool appendFighterSGInfo(Stream& st, UInt16 skillid, SGrade* sg);
    void SGFromDB(UInt16 id, SGrade& sg);
    void makeFighterSGInfoWithNoSkill(Stream& st);
    void getAllSGAndValue(Stream& st);
    void SGDismiss(UInt16, bool = false, Mail * = NULL);
    void SGDismissAll(bool isDel);
    void SGDeleteDB(UInt16 id);
    void getAllSGInfo(std::map<UInt16, Int32>& sg_info);

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
	Fighter * getRandomOut(Player * pl, std::set<UInt32>& excepts, std::set<UInt32>& excepts2, UInt8 type, UInt32 rib, UInt32 rip, UInt32 rio);
	void setSpot(UInt32 id, UInt16 spot);
	UInt16 getSpot(UInt32 id);
	inline void setMaxFgtId(UInt32 n) { _maxFgtId = n; }
    UInt32 getColorFighterNum(UInt8 free_gold, UInt8 color ) { return _summonSet[free_gold][color].size(); }

private:
	FastMutex _mutex;
	std::vector<FighterData> _fighters;
	UInt32 _maxFgtId;
	std::map<UInt32,UInt32> _summonSet[2][5];
};

extern GlobalFighters globalFighters;
extern Fighter* fighter_5509;

extern Fighter& getGreatFighter(UInt32 id);
extern bool existGreatFighter(UInt32 id);

}

#endif // _FIGHTER_H_

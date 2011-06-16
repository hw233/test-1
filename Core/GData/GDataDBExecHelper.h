#ifndef _GDATADBEXECHELPER_H_
#define _GDATADBEXECHELPER_H_

#include "DB/DBExecHelper.h"

namespace GData
{

struct DBAreaDef
{
	UInt32 id;
	UInt8  side;
	std::string area;
};

//Weapon class definitions read from DB
struct DBWeaponDef
{
	UInt32 id;
	std::string name;
	UInt32 class_req;
	UInt32 area_effect;
	UInt8 can_counter;
};

struct DBItemType
{
	UInt32		typeId;         // 物品编号
	std::string	name;           // 名字
	UInt8		subClass;       // 物品类型 - 
    UInt8       career;         // 职业
	UInt16		reqLev;         // 物品(可装备)等级
	UInt32		coin;           // 价格(铜)
	UInt8		quality;        // 品质
	UInt16		maxQuantity;    // 最大重叠数
	UInt8		bindType;       // 是否绑定
	UInt16		data;           // 可使用道具: 作用数值
	UInt32		attrExtra;      // 属性附加
};

struct DBEquipSetType
{
	UInt32 id;
	std::string name;
	UInt16 attrId[4];
};

struct DBAttrExtra
{
	UInt32 id;
	std::string strength;
	std::string physique;
	std::string agility;
	std::string intelligence;
    std::string will;
    std::string soul;
    std::string aura;
    std::string auraMax;
    std::string tough;
	std::string attack;
	std::string magatk;
	std::string defend;
	std::string magdef;
	std::string hp;
	std::string skills;
	float action;
	float hitrate;
	float evade;
	float critical;
	float critical_dmg;
	float pierce;
	float counter;
	float magres;
};

struct DBFormation
{
	UInt32 id;
	std::string name;
	std::string eyes;
	std::string grids;
	UInt8 leastNum;
	UInt16 skillId;
};

struct DBSkill
{
    UInt16 id;
    std::string name;
    UInt16 cond;        // 触发条件: SKILL_ACTIVE   - 主动
                        //           SKILL_PEERLESS - 无双技能,当灵气>=100释放
                        //           SKILL_PREATK   - 攻击前被动触发(回血技能,无概率)
                        //           SKILL_AFTATK   - 攻击后被动触发(有概率)
                        //           SKILL_BEATKED  - 被攻击后触发(有概率)
                        //           SKILL_AFTEVD   - 闪避后触发
                        //           SKILL_AFTRES   - 抵抗后触发
                        //           SKILL_ENTER    - 入场时触发
                        //           SKILL_DEAD     - 死亡后触发
    float prob;         // 主动状态触发概率 或 被动触发概率
    UInt8 target;       // 作用对象: 0-友方,1-敌方,2-自己(对友方和自己加,对敌方减)
    UInt8 area;         // 伤害范围: 0-单体,1-全体,2-横排,3-竖列,4-十字,5-品字 (4,5可选支持)
    std::string factor; // 伤害倍率: 如, 横排伤害 1,0.3,0.5,1,0 距离攻击目标为0的伤害系数是1,距离为2的伤害系数为0.5
    Int16 last;         // 持续时间: -1-一直有效,0-非持续,N-持续次数
    UInt16 cd;          // 冷却回合
    UInt16 effectid;    // 效果索引
};

struct DBSkillEffect
{
    UInt16 id;
    UInt8 state;        // 状态: 0-无状态 1-中毒，2-混乱，4-晕眩(无法攻击)，8-无法使用技能, 16-反击 有等级之分
    UInt8 immune;       // 对状态技能的免疫,只能免疫比自己技能低的技能
    std::string damage; // 物理伤害 [+/-]num/num% (目前物理伤害和法术伤害互斥)
    float adddam;       // 物理伤害附加(具体值)
    std::string magdam; // 法术伤害 [+/-]num/num%
    float addmag;       // 法术伤害附加(具体值)
    std::string hp;     // HP改变 [+/-]num/num%
    float addhp;        // HP改变附加(具体值)
    std::string absorb; // 伤害吸血 [+/-]num/num%
    std::string thorn;  // 反弹 [+/-]num/num%
    std::string inj2hp; // 受伤回扣 [+/-]num/num%
    std::string aura;   // 作用士气 [+/-]num/num%
    std::string atk;    // 物理攻击 [+/-]num/num%
    std::string def;    // 物理防御 [+/-]num/num%
    std::string magatk; // 法术攻击 [+/-]num/num%
    std::string magdef; // 法术防御 [+/-]num/num%
    float tough;        // 坚韧
    float action;       // 身法
    float hitrate;      // 命中
    float evade;        // 闪避
    float critical;     // 暴击
    float pierce;       // 击破/护甲穿透
    float counter;      // 反击
    float magres;       // 法术抵抗
};

struct DBClanSkillType
{
	UInt16 id;
	UInt8  level;
	UInt32 needs;
	UInt8  clanLev;
	UInt32 effect1;
	UInt32 effect2;
};

struct DBCitta
{
    UInt16 id;          // 类型(阶)及等级 id=id/100 lvl=id%100
    std::string name;   // 名称
    UInt32 pexp;        // 修为消耗
    UInt16 needsoul;    // 元神需求(负重)
    UInt16 effectid;    // 效果索引
};

struct DBCittaEffect
{
	UInt16 id;
    std::string skill;          // 带出技能(包括技能和无双技能)
	std::string strength;       // 力量 [+/-]num/num%
	std::string physique;       // 耐力 [+/-]num/num%
	std::string agility;        // 敏捷 [+/-]num/num%
	std::string intelligence;   // 智力 [+/-]num/num%
    std::string will;           // 意志 [+/-]num/num%
    std::string soul;           // 元神 [+/-]num/num%
    std::string aura;           // 灵气 [+/-]num/num%
    std::string auraMax;        // 最大灵气 [+/-]num/num%
	std::string attack;         // 物攻 [+/-]num/num%
	std::string magatk;     // 法攻 [+/-]num/num%
	std::string defend;         // 物防 [+/-]num/num%
	std::string magdef;     // 法防 [+/-]num/num%
	std::string hp;             // HP [+/-]num/num%
    float tough;                // 坚韧
	float action;               // 身法
	float hitrate;              // 命中
	float evade;                // 闪避
	float critical;             // 暴击
	float critical_dmg;         // 暴击伤害
	float pierce;               // 击破/护甲穿透
	float counter;              // 反击
	float magres;               // 法术抵抗
};

// 穴道及修为
struct DBAcuPra
{
    UInt8 id;       // 穴道
    UInt8 lvl;      // 层级(1-3)
    UInt8 needlvl;  // 所需等级
    UInt32 pra;     // 所需修为
    UInt32 soulmax; // 元神力上限
    UInt32 pramax;  // 修为上限
    UInt8 citslot;  // 心法装备加1
    UInt8 aura;     // 出场灵气加成
    UInt8 auraInc;  // 灵气增长加成
};

struct DBExp
{
	UInt8 lvl;
	UInt64 exp;
};

struct DBLootItem
{
	UInt32 id;
	std::string table;
	UInt8 isPack;
};

struct DBStoreItem
{
	UInt8 type;
	UInt16 itemId;
	UInt16 price;
};

}


namespace DB {

SPECIALBEGIN(GData::DBAreaDef)
SPECIALDEF(3)
	(
	UInt32,		id,
	UInt8,		side,
	std::string,area
	)
SPECIALEND()

SPECIALBEGIN(GData::DBWeaponDef)
SPECIALDEF(5)
	(
	UInt32,		id,
	std::string,name,
	UInt32,		class_req,
	UInt32,		area_effect,
	UInt8,		can_counter
	)
SPECIALEND()

SPECIALBEGIN(GData::DBItemType)
SPECIALDEF(11)
	(
	UInt32,		typeId,
	std::string,name,
	UInt8,		subClass,
    UInt8,      career,
	UInt16,		reqLev,
	UInt32,		coin,
	UInt8,		quality,
	UInt16,		maxQuantity,
	UInt8,		bindType,
	UInt16,		data,
	UInt32,		attrExtra
	)
SPECIALEND()

SPECIALBEGIN(GData::DBEquipSetType)
SPECIALDEF(6)
	(
	UInt32,	id,
	std::string,name,
	UInt16,	attrId[0],
	UInt16,	attrId[1],
	UInt16,	attrId[2],
	UInt16,	attrId[3]
	)
SPECIALEND()

SPECIALBEGIN(GData::DBAttrExtra)
SPECIALDEF(24)
	(
	UInt32, id,
	std::string, strength,
	std::string, physique,
	std::string, agility,
	std::string, intelligence,
    std::string, will,
    std::string, soul,
    std::string, aura,
    std::string, auraMax,
    std::string, tough,
	std::string, attack,
	std::string, magatk,
	std::string, defend,
	std::string, magdef,
	std::string, hp,
	std::string, skills,
	float, action,
	float, hitrate,
	float, evade,
	float, critical,
	float, critical_dmg,
	float, pierce,
	float, counter,
	float, magres
	)
SPECIALEND()

SPECIALBEGIN(GData::DBFormation)
SPECIALDEF(6)
	(
	UInt32, id,
	std::string, name,
	std::string, eyes,
	std::string, grids,
	UInt8, leastNum,
	UInt16, skillId
	)
SPECIALEND()

SPECIALBEGIN(GData::DBExp)
SPECIALDEF(2)
	(
	UInt8,			lvl,
	UInt64,			exp
	)
SPECIALEND()

SPECIALBEGIN(GData::DBLootItem)
SPECIALDEF(3)
	(
	UInt32, id,
	std::string, table,
	UInt8, isPack
	)
SPECIALEND()

SPECIALBEGIN(GData::DBStoreItem)
SPECIALDEF(3)
	(
	UInt8, type,
	UInt16, itemId,
	UInt16, price
	)
SPECIALEND()


SPECIALBEGIN(GData::DBClanSkillType)
SPECIALDEF(6)
	(
	UInt16, id,
	UInt8,  level,
	UInt32, needs,
	UInt8, clanLev,
	UInt32, effect1,
	UInt32, effect2
	)
SPECIALEND()

SPECIALBEGIN(GData::DBSkill)
SPECIALDEF(10)
    (
        UInt16, id,
        std::string, name,
        UInt8, target,
        UInt16, cond,
        float, prob,
        UInt8, area,
        std::string, factor,
        Int16, last,
        UInt16, cd,
        UInt16, effectid
    )
SPECIALEND()

SPECIALBEGIN(GData::DBSkillEffect)
SPECIALDEF(25)
    (
        UInt16, id,
        UInt8, state,
        UInt8, immune,
        std::string, damage,
        float, adddam,
        std::string, magdam,
        float, addmag,
        std::string, absorb,
        std::string, thorn,
        std::string, inj2hp,
        std::string, aura,
        std::string, atk,
        std::string, def,
        std::string, magatk,
        std::string, magdef,
        std::string, hp,
        float, addhp,
        float, tough,
        float, action,
        float, hitrate,
        float, evade,
        float, pierce,
        float, critical,
        float, counter,
        float, magres
    )
SPECIALEND()

SPECIALBEGIN(GData::DBAcuPra)
SPECIALDEF(9)
    (
        UInt8, id,
        UInt8, lvl,
        UInt8, needlvl,
        UInt32, pra,
        UInt32, soulmax,
        UInt32, pramax,
        UInt8, citslot,
        UInt8, aura,
        UInt8, auraInc
    )
SPECIALEND()

SPECIALBEGIN(GData::DBCitta)
SPECIALDEF(4)
    (
        UInt16, id,
        std::string, name,
        UInt16, needsoul,
        UInt16, effectid
    )
SPECIALEND()

SPECIALBEGIN(GData::DBCittaEffect)
SPECIALDEF(24)
    (
        UInt16, id,
        std::string, skill,
        std::string, strength,
        std::string, physique,
        std::string, agility,
        std::string, intelligence,
        std::string, will,
        std::string, soul,
        std::string, aura,
        std::string, auraMax,
        std::string, attack,
        std::string, magatk,
        std::string, defend,
        std::string, magdef,
        std::string, hp,
        float, tough,
        float, action,
        float, hitrate,
        float, evade,
        float, critical,
        float, critical_dmg,
        float, pierce,
        float, counter,
        float, magres
    )
SPECIALEND()

}

#endif // _GDATADBEXECHELPER_H_

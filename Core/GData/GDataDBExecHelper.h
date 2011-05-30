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
	std::string rate;
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
	UInt32		coin;           // 价格(铜)
	UInt16		reqLev;         // 物品(可装备)等级
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
	std::string mag_attack;
	std::string defend;
	std::string mag_defend;
	std::string hp;
	float action;
	float hitrate;
	float evade;
	float critical;
	float critical_dmg;
	float pierce;
	float counter;
	float mag_res;
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
    UInt8 target;       // 作用对象: 0-友方,1-敌方
    UInt16 cond;        // 触发条件: 0-主动,1-攻击前被动触发(回血技能,无概率),2-攻击后被动触发(有概率),3-被攻击后触发(有概率) N-灵气值(>=)
    float prob;         // 触发概率
    UInt8 area;         // 伤害范围: 0-单体,1-横排,2-竖列,3-全体
    std::string factor; // 伤害倍率: 如, 横排伤害 1,0.3,0.5,1,0 对第一个位置是100%,对第二个位置是30%,对第五个位置没有伤害
    UInt16 last;        // 持续时间: -1-一直有效,0-非持续,N-持续次数
    UInt16 cd;          // 冷却回合
    UInt16 effectId;    // 效果索引
};

struct DBSkillEffect
{
    UInt16 id;
    UInt8 state;        // 状态: 0-无状态 1中毒，2混乱，4晕眩(无法攻击)，8无法使用技能, 有等级之分
    float stateprob;    // 状态触发概率
    UInt8 immune;       // 对状态技能的免疫,只能免疫比自己技能低的技能
    std::string damage; // 物理伤害 [+-]num/num%
    std::string magdam; // 法术伤害 [+-]num/num%
    std::string hp;     // HP改变
    std::string aura;   // 作用士气 [+/-]num
    UInt8 hitCount;     // 连击次数
    std::string def;    // 物理防御 [+/-]num/num%
    std::string magdef; // 法术防御 [+-]num/num%
    float evade;        // 闪避
    float pierce;       // 破击
    float adddam;       // 人物属性的伤害加成(人物属性的adddam倍)
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
    UInt16 id;          // id及阶数(等级)
    std::string name;   // 名称
    UInt16 needsoul;    // 元神需求(负重)
    UInt32 effectId;    // 效果索引
};

struct DBCittaEffect
{
	UInt32 id;
	std::string strength;       // 力量 [+/-]num/num%
	std::string physique;       // 耐力 [+/-]num/num%
	std::string agility;        // 敏捷 [+/-]num/num%
	std::string intelligence;   // 智力 [+/-]num/num%
    std::string will;           // 意志 [+/-]num/num%
    std::string soul;           // 元神 [+/-]num/num%
    std::string aura;           // 灵气 [+/-]num/num%
    std::string auraMax;        // 最大灵气 [+/-]num/num%
	std::string attack;         // 物攻 [+/-]num/num%
	std::string mag_attack;     // 法攻 [+/-]num/num%
	std::string defend;         // 物防 [+/-]num/num%
	std::string mag_defend;     // 法防 [+/-]num/num%
	std::string hp;             // HP [+/-]num/num%
    std::string skill;          // 带出技能(包括技能和无双技能)
	float action;
	float hitrate;
	float evade;
	float critical;
	float critical_dmg;
	float pierce;
	float counter;
	float mag_res;
    float practice;             // 修炼速度加成
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
SPECIALDEF(4)
	(
	UInt32,		id,
	UInt8,		side,
	std::string,area,
	std::string,rate
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
SPECIALDEF(10)
	(
	UInt32,		typeId,
	std::string,name,
	UInt8,		subClass,
	UInt32,		coin,
	UInt16,		reqLev,
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
SPECIALDEF(23)
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
	std::string, mag_attack,
	std::string, defend,
	std::string, mag_defend,
	std::string, hp,
	float, action,
	float, hitrate,
	float, evade,
	float, critical,
	float, critical_dmg,
	float, pierce,
	float, counter,
	float, mag_res
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
        UInt16, last,
        UInt16, cd,
        UInt16, effectId
    )
SPECIALEND()

SPECIALBEGIN(GData::DBSkillEffect)
SPECIALDEF(14)
    (
        UInt16, id,
        UInt8, state,
        float, stateprob,
        UInt8, immune,
        std::string, damage,
        std::string, magdam,
        std::string, hp,
        std::string, aura,
        UInt8, hitCount,
        std::string, def,
        std::string, magdef,
        float, evade,
        float, pierce,
        float, adddam
    )
SPECIALEND()

SPECIALBEGIN(GData::DBCitta)
SPECIALDEF(4)
    (
        UInt16, id,
        std::string, name,
        UInt16, needsoul,
        UInt32, effectId
    )
SPECIALEND()

SPECIALBEGIN(GData::DBCittaEffect)
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
        std::string, attack,
        std::string, mag_attack,
        std::string, defend,
        std::string, mag_defend,
        std::string, hp,
        std::string, skill,
        float, action,
        float, hitrate,
        float, evade,
        float, critical,
        float, critical_dmg,
        float, pierce,
        float, counter,
        float, mag_res,
        float, practice
    )
SPECIALEND()

}

#endif // _GDATADBEXECHELPER_H_

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
    UInt16 cond;        // 触发条件: 0-主动
                        //           1-攻击前被动触发(回血技能,无概率)
                        //           2-攻击后被动触发(有概率)
                        //           3-被攻击后触发(有概率)
                        //           4-无双技能,当灵气>=100释放
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
    UInt8 state;        // 状态: 0-无状态 1-中毒，2-混乱，4-晕眩(无法攻击)，8-无法使用技能, 有等级之分
    UInt8 immune;       // 对状态技能的免疫,只能免疫比自己技能低的技能
    std::string damage; // 物理伤害 num/num% (目前物理伤害和法术伤害互斥)
    float adddam;       // 物理伤害附加(具体值)
    std::string magdam; // 法术伤害 num/num%
    float addmag;       // 法术伤害附加(具体值)
    std::string hp;     // HP改变 num/num%
    float addhp;        // HP改变附加(具体值)
    float absorb;       // 伤害吸血系数
    std::string aura;   // 作用士气 num/num%
    std::string atk;    // 物理攻击 num/num%
    std::string def;    // 物理防御 num/num%
    std::string magatk; // 法术攻击 num/num%
    std::string magdef; // 法术防御 num/num%
    float tough;        // 坚韧
    float action;       // 身法
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
    UInt16 id;          // id及阶数(等级)
    std::string name;   // 名称
    UInt16 needsoul;    // 元神需求(负重)
    UInt16 effectid;    // 效果索引
};

struct DBCittaEffect
{
	UInt16 id;
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
	float magres;
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
	std::string, mag_attack,
	std::string, defend,
	std::string, mag_defend,
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
SPECIALDEF(22)
    (
        UInt16, id,
        UInt8, state,
        UInt8, immune,
        std::string, damage,
        float, adddam,
        std::string, magdam,
        float, addmag,
        float, absorb,
        std::string, aura,
        std::string, atk,
        std::string, def,
        std::string, magatk,
        std::string, magdef,
        std::string, hp,
        float, addhp,
        float, tough,
        float, action,
        float, evade,
        float, pierce,
        float, critical,
        float, counter,
        float, magres
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
        float, magres,
        float, practice
    )
SPECIALEND()

}

#endif // _GDATADBEXECHELPER_H_

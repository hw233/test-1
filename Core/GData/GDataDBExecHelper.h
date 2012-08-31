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
	UInt16		vLev;           // 物品(价值)等级
	UInt32		coin;           // 价格(铜)
	UInt8		quality;        // 品质 1-白 2-绿 3-蓝 4-紫 5-橙
	UInt16		maxQuantity;    // 最大重叠数
	UInt8		bindType;       // 是否绑定
	UInt16		energy;         // 九仪鼎值
    UInt16      trumpExp;       // 法宝经验
	UInt16		data;           // 可使用道具: 作用数值
    UInt16      enchant;        // 附魔类型
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
	std::string skill;          // 带出技能
	std::string strength;       // 力量 [+/-]num/num%
	std::string physique;       // 耐力 [+/-]num/num%
	std::string agility;        // 敏捷 [+/-]num/num%
	std::string intelligence;   // 智力 [+/-]num/num%
    std::string will;           // 意志 [+/-]num/num%
    std::string soul;           // 元神 [+/-]num/num%
    std::string aura;           // 作用士气 [+/-]num/num%
    std::string auraMax;        // 最大灵气 [+/-]num/num%
	std::string attack;         // 物攻 [+/-]num/num%
	std::string magatk;         // 法术攻击 [+/-]num/num%
	std::string defend;         // 物防 [+/-]num/num%
	std::string magdef;         // 法术防御 [+/-]num/num%
	std::string hp;             // HP [+/-]num/num%
    std::string tough;          // 坚韧 [+/-]num/num%
	std::string action;         // 身法 [+/-]num/num%
	std::string hitrate;        // 命中 [+/-]num/num%
	std::string evade;          // 闪避 [+/-]num/num%
	std::string critical;       // 暴击 [+/-]num/num%
	std::string criticaldmg;    // 暴击伤害 [+/-]num/num%
	std::string pierce;         // 击破/护甲穿透 [+/-]num/num%
	std::string counter;        // 反击 [+/-]num/num%
	std::string magres;         // 法术抵抗 [+/-]num/num%
};

struct DBFormation
{
	UInt16 id;
	std::string name;
    UInt8 grid1;
    UInt32 prop1;
    UInt8 grid2;
    UInt32 prop2;
    UInt8 grid3;
    UInt32 prop3;
    UInt8 grid4;
    UInt32 prop4;
    UInt8 grid5;
    UInt32 prop5;
	std::string levelup_item;
};

struct DBSkillStrengthen
{
    UInt16 id;
    std::string name;
    std::string effect;
};

struct DBSkillStrengthenEffect
{
    UInt16 id;
    UInt16 cond;
    UInt8 target;
    float prob;
    UInt8 area;
    std::string factor;
    Int16 last;
    Int16 type;
    float value;
    float valueExt1;
    float valueExt2;
};

struct DBSkill
{
    UInt16 id;
    std::string name;
    UInt16 cond;        // 触发条件: SKILL_ACTIVE   - 主动
                        //           SKILL_PEERLESS - 无双技能,当灵气>=100释放
                        //           SKILL_PREATK   - 攻击前被动触发(回血技能,无概率)
                        //           SKILL_AFTATK   - 攻击后被动触发(有概率)
                        //           SKILL_AFTNATK  - 普通攻击后被动触发(有概率)
                        //           SKILL_BEATKED  - 被攻击后触发(有概率)
                        //           SKILL_AFTEVD   - 闪避后触发
                        //           SKILL_AFTRES   - 抵抗后触发
                        //           SKILL_ENTER    - 入场时触发
                        //           SKILL_DEAD     - 死亡后触发
    float prob;         // 主动状态触发概率 或 被动触发概率
    UInt8 target;       // 作用对象: 0-友方,1-敌方,2-自己(对友方和自己加,对敌方减)
    UInt8 area;         // 伤害范围: 0-单体,1-全体,2-横排,3-竖列,4-十字,5-V字,6-T字
    std::string factor; // 伤害倍率: 如, 横排伤害 1,0.3,0.5,1,0 距离攻击目标为0的伤害系数是1,距离为2的伤害系数为0.5
    Int16 last;         // 持续时间: -1-一直有效,0-非持续,N-持续次数
    UInt16 cd;          // 冷却回合
    UInt16 effectid;    // 效果索引
};

struct DBSkillEffect
{
    UInt16 id;
    UInt8 state;        // 状态: 0-无状态 1-中毒，2-混乱，4-晕眩(无法攻击)，8-无法使用技能, 16-反伤, 32-虚弱, 64-降灵气 有等级之分
    UInt8 immune;       // 对状态技能的免疫,只能免疫比自己技能低的技能
    UInt8 disperse;     // 驱散状态,只对友方使用,除自己外,是状态的值的和
    std::string damage; // 物理伤害 num/num% (目前物理伤害和法术伤害互斥)
    float adddam;       // 物理伤害附加(具体值)
    std::string magdam; // 法术伤害 num/num%
    float addmag;       // 法术伤害附加(具体值)
    std::string crrdam; // 职业伤害 num/num%
    float addcrr;       // 职业伤害附加(具体值)
    std::string hp;     // HP改变 [+/-]num/num%
    float addhp;        // HP改变附加(具体值)[+/-]
    std::string absorb; // 伤害吸血 num/num%
    std::string thorn;  // 反弹 num/num%
    std::string inj2hp; // 受伤回扣 num/num%
    std::string aura;   // 作用士气 [+/-]num/num%
    std::string atk;    // 物理攻击 [+/-]num/num%
    std::string def;    // 物理防御 [+/-]num/num%
    std::string magatk; // 法术攻击 [+/-]num/num%
    std::string magdef; // 法术防御 [+/-]num/num%
    float tough;        // 坚韧[+/-]
    std::string action; // 身法[+/-]num/num%
    float hitrate;      // 命中[+/-]
    float evade;        // 闪避[+/-]
    float critical;     // 暴击[+/-]
    float pierce;       // 击破/护甲穿透[+/-]
    float counter;      // 反击[+/-]
    float magres;       // 法术抵抗[+/-]
    float atkreduce;    // 物理伤害减免
    float magatkreduce; // 法术伤害减免
};

struct DBTalent
{
    UInt16 id;                  // 天赋ID
    std::string name;           // 天赋名
    UInt8 cls;                  // 职业
    UInt8 quality;              // 天赋品质
    float prob;                 // 洗概率(同一职业的概率之和为100)
    float potential;            // 潜力*
    float capacity;             // 资质*
	std::string strength;       // 力量 [+/-]num/num%
	std::string physique;       // 耐力 [+/-]num/num%
	std::string agility;        // 敏捷 [+/-]num/num%
	std::string intelligence;   // 智力 [+/-]num/num%
    std::string will;           // 意志 [+/-]num/num%
    std::string soul;           // 元神 [+/-]num/num%
    std::string aura;           // 作用士气 [+/-]num/num%
    std::string auraMax;        // 最大灵气 [+/-]num/num%
	std::string attack;         // 物攻 [+/-]num/num%
	std::string magatk;         // 法术攻击 [+/-]num/num%
	std::string defend;         // 物防 [+/-]num/num%
	std::string magdef;         // 法术防御 [+/-]num/num%
	std::string hp;             // HP [+/-]num/num%
    float tough;                // 坚韧
	float action;               // 身法
	float hitrate;              // 命中
	float evade;                // 闪避
	float critical;             // 暴击
	float criticaldmg;         // 暴击伤害
	float pierce;               // 击破/护甲穿透
	float counter;              // 反击
	float magres;               // 法术抵抗
};

struct DBClanLvl
{
	UInt8 lvl;          // 帮派等级
	UInt64 exp;         // 建设度值
    UInt32 pkgsize;     // 仓库大小
    UInt32 weal;        // 福利
};

struct DBClanTechType
{
	UInt16 id;          // 科技ID
	UInt8  level;       // 科技等级
	UInt32 needs;       // 所需捐献
	UInt8  clanLev;     // 所需帮派等级
	UInt32 effect1;     // 效果一
	UInt32 effect2;     // 效果二
};

struct DBClanSkillType
{
	UInt16 id;          // 技能ID
    std::string name;   // 技能名
	UInt8  level;       // 技能等级
	UInt32 needs;       // 升级所需
    float value;        // 技能效果值
};

struct DBClanCopy
{
    UInt16 level;               // 帮派副本等级
    UInt32 expOutput;           // 该等级每小时给予神像的经验值
};

struct DBClanCopyMonster
{
    UInt16 level;              // 副本等级
    UInt16 appearRound;        // 生产该怪的波数
    UInt32 npcId;              // 怪物npcGroupId
    UInt8  npcCount;           // 每一路怪物的数量
    UInt8  npcRouteCount;      // 出现的路数 
    UInt16 npcValue;           // 怪物对主基地的破坏值
};

struct DBClanStatue
{
    // 帮派神像模板参数
    UInt16 level;       // 神像等级
    UInt32 needExp;     // 升级所需经验值
    UInt32 consumeExp;  // 每小时减少的经验值
    UInt32 exHp;        // 神像增加的hp
    UInt32 exAttack;    // 神像增加的物攻值
    UInt32 exDefend;    // 神像增加的物防值
    UInt32 exMagAtk;    // 神像增加的法攻值
    UInt32 exMagDef;    // 神像增加的法防值
    UInt32 exAction;    // 神像增加的身法值
    UInt32 exHitRate;   // 神像增加的命中值
};

struct DBSoulSkillType
{
	UInt16 id;          // 技能ID
    std::string name;   // 技能名
	UInt8  level;       // 技能等级
	UInt32 itemId;      // 技能书ID
    float value;        // 技能效果值
};

struct DBCitta
{
    UInt16 id;          // ID及等级 id=id/100 lvl=id%100
    UInt16 type;        // 阶
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
	std::string magatk;         // 法攻 [+/-]num/num%
	std::string defend;         // 物防 [+/-]num/num%
	std::string magdef;         // 法防 [+/-]num/num%
	std::string hp;             // HP [+/-]num/num%
    std::string tough;          // 坚韧 [+/-]num/num%
	std::string action;         // 身法 [+/-]num/num%
	std::string hitrate;        // 命中 [+/-]num/num%
	std::string evade;          // 闪避 [+/-]num/num%
	std::string critical;       // 暴击 [+/-]num/num%
	std::string criticaldmg;    // 暴击伤害 [+/-]num/num%
	std::string pierce;         // 击破/护甲穿透 [+/-]num/num%
	std::string counter;        // 反击 [+/-]num/num%
	std::string magres;         // 法术抵抗 [+/-]num/num%
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
};

struct DBExp
{
	UInt8 lvl;
	UInt64 exp;
};

struct DBSoulExp
{
	UInt8 lvl;
	UInt32 exp;
	UInt32 pexp;
};

struct DBLootItem
{
	UInt32 id;
	std::string table;
	UInt8 isPack;
};
struct DBEUpgrade
{
    UInt32 id;
    std::string stfs;
    UInt32 toId;
};
struct DBStoreItem
{
	UInt8 type;
	UInt16 itemId;
	UInt16 price;
};

struct DBFighterProb
{
    UInt32 id;
    Int32 free;
    Int32 gold;
};

struct DBSpiritAttr
{
    UInt16 level;
    float attack;
    float defend;
    float critical;
    float pierce;
    float action;
    float tough;
    float criticaldmg;
    float hp;
};

struct DBSoulItemExp
{
    UInt32 itemId;
    UInt32 exp;
};

struct DBSSExp
{
    UInt16 id;
    UInt32 exp1;
    UInt32 exp2;
    UInt32 exp3;
    UInt32 exp4;
    UInt32 exp5;
    UInt32 exp6;
    UInt32 exp7;
    UInt32 exp8;
    UInt32 exp9;
};

struct DBSSProb
{
    UInt16 id;
    UInt32 prob1;
    UInt32 prob2;
    UInt32 prob3;
    UInt32 prob4;
    UInt32 prob5;
    UInt32 prob6;
    UInt32 prob7;
    UInt32 prob8;
    UInt32 prob9;
};

struct DBCopy
{
    UInt8 id;
    UInt8 floor;
    UInt8 spot;
    UInt32 fighterId;
};

struct DBFrontMap
{
    UInt8 id;
    UInt8 spot;
    UInt8 count;
    UInt32 fighterId;
};

struct DBTianjie
{
	int id;
	bool is_opened;
	bool is_execute;
	bool is_finish;
	bool is_ok;
	int level;
	int rate;
	int opentime;
    std::string r1_killed;
    std::string r2_donated;
    int r3_copyid;
    UInt8 r4_day;
    UInt8 open_next;
    UInt8 is_wait;
};
struct DBPlayerMaxLevel
{
    int level;
};
struct DBTianleizhen
{
    int type;
    int level;
    int npcid;
    int score;
};
struct DBReportId
{
    int id;
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
SPECIALDEF(15)
	(
	UInt32,		typeId,
	std::string,name,
	UInt8,		subClass,
    UInt8,      career,
	UInt16,		reqLev,
	UInt16,		vLev,
	UInt32,		coin,
	UInt8,		quality,
	UInt16,		maxQuantity,
	UInt8,		bindType,
	UInt16,		energy,
    UInt16,     trumpExp,
	UInt16,		data,
	UInt16,		enchant,
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
    std::string, tough,
	std::string, action,
	std::string, hitrate,
	std::string, evade,
	std::string, critical,
	std::string, criticaldmg,
	std::string, pierce,
	std::string, counter,
	std::string, magres
	)
SPECIALEND()

SPECIALBEGIN(GData::DBFormation)
SPECIALDEF(13)
	(
	UInt16, id,
	std::string, name,
    UInt8, grid1,
    UInt32, prop1,
    UInt8, grid2,
    UInt32, prop2,
    UInt8, grid3,
    UInt32, prop3,
    UInt8, grid4,
    UInt32, prop4,
    UInt8, grid5,
    UInt32, prop5,
	std::string, levelup_item
	)
SPECIALEND()

SPECIALBEGIN(GData::DBExp)
SPECIALDEF(2)
	(
	UInt8,			lvl,
	UInt64,			exp
	)
SPECIALEND()

SPECIALBEGIN(GData::DBSoulExp)
SPECIALDEF(3)
	(
	UInt8, lvl,
	UInt32, exp,
	UInt32, pexp
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

SPECIALBEGIN(GData::DBEUpgrade)
SPECIALDEF(3)
	(
	UInt32, id,
	std::string, stfs,
    UInt32, toId
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

SPECIALBEGIN(GData::DBClanLvl)
SPECIALDEF(4)
	(
	UInt8,			lvl,
	UInt64,			exp,
    UInt32,         pkgsize,
    UInt32,         weal
	)
SPECIALEND()

SPECIALBEGIN(GData::DBClanTechType)
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

SPECIALBEGIN(GData::DBClanSkillType)
SPECIALDEF(5)
	(
	UInt16, id,
    std::string, name,
	UInt8,  level,
	UInt32, needs,
    float, value
	)
SPECIALEND()

SPECIALBEGIN(GData::DBClanCopy)
SPECIALDEF(2)
    (
    UInt16, level,       
    UInt32, expOutput
    )
SPECIALEND()

SPECIALBEGIN(GData::DBClanCopyMonster)
SPECIALDEF(6)
    (
    UInt16, level,
    UInt16, appearRound,
    UInt32, npcId,
    UInt8,  npcCount,
    UInt8,  npcRouteCount,
    UInt16, npcValue
    )
SPECIALEND()


SPECIALBEGIN(GData::DBClanStatue)
SPECIALDEF(10)
    (
    UInt16, level,       
    UInt32, needExp,         
    UInt32, consumeExp,   
    UInt32, exHp,        
    UInt32, exAttack,    
    UInt32, exDefend,    
    UInt32, exMagAtk,    
    UInt32, exMagDef,    
    UInt32, exAction,    
    UInt32, exHitRate
    )
SPECIALEND()

SPECIALBEGIN(GData::DBSoulSkillType)
SPECIALDEF(5)
	(
	UInt16, id,
    std::string, name,
	UInt8,  level,
	UInt32, itemId,
    float, value
	)
SPECIALEND()

SPECIALBEGIN(GData::DBSkillStrengthen)
SPECIALDEF(3)
    (
    UInt16, id,
    std::string, name,
    std::string, effect
    )
SPECIALEND()


SPECIALBEGIN(GData::DBSkillStrengthenEffect)
SPECIALDEF(11)
    (
    UInt16, id,
    UInt16, cond,
    UInt8, target,
    float, prob,
    UInt8, area,
    std::string, factor,
    Int16, last,
    Int16, type,
    float, value,
    float, valueExt1,
    float, valueExt2
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
SPECIALDEF(30)
    (
        UInt16, id,
        UInt8, state,
        UInt8, immune,
        UInt8, disperse,
        std::string, damage,
        float, adddam,
        std::string, magdam,
        float, addmag,
        std::string, crrdam,
        float, addcrr,
        std::string, hp,
        float, addhp,
        std::string, absorb,
        std::string, thorn,
        std::string, inj2hp,
        std::string, aura,
        std::string, atk,
        std::string, def,
        std::string, magatk,
        std::string, magdef,
        float, tough,
        std::string, action,
        float, hitrate,
        float, evade,
        float, critical,
        float, pierce,
        float, counter,
        float, magres,
        float, atkreduce,
        float, magatkreduce
    )
SPECIALEND()

SPECIALBEGIN(GData::DBTalent)
SPECIALDEF(29)
    (
        UInt16, id,
        std::string, name,
        UInt8, cls,
        UInt8, quality,
        float, prob,
        float, potential,
        float, capacity,
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
        float, criticaldmg,
        float, pierce,
        float, counter,
        float, magres
    )
SPECIALEND()

SPECIALBEGIN(GData::DBAcuPra)
SPECIALDEF(7)
    (
        UInt8, id,
        UInt8, lvl,
        UInt8, needlvl,
        UInt32, pra,
        UInt32, soulmax,
        UInt32, pramax,
        UInt8, citslot
    )
SPECIALEND()

SPECIALBEGIN(GData::DBCitta)
SPECIALDEF(6)
    (
        UInt16, id,
        UInt16, type,
        std::string, name,
        UInt32, pexp,
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
        std::string, tough,
        std::string, action,
        std::string, hitrate,
        std::string, evade,
        std::string, critical,
        std::string, criticaldmg,
        std::string, pierce,
        std::string, counter,
        std::string, magres
    )
SPECIALEND()

SPECIALBEGIN(GData::DBFighterProb)
SPECIALDEF(3)
    (
        UInt32, id,
        Int32, free,
        Int32, gold
    )
SPECIALEND()

SPECIALBEGIN(GData::DBSpiritAttr)
SPECIALDEF(9)
    (
        UInt16, level,
        float, attack,
        float, defend,
        float, critical,
        float, pierce,
        float, action,
        float, tough,
        float, criticaldmg,
        float, hp
    )
SPECIALEND()

SPECIALBEGIN(GData::DBSoulItemExp)
SPECIALDEF(2)
    (
    UInt32, itemId,
    UInt32, exp
    )
SPECIALEND()

SPECIALBEGIN(GData::DBSSExp)
SPECIALDEF(10)
    (
    UInt16,id,
    UInt32,exp1,
    UInt32,exp2,
    UInt32,exp3,
    UInt32,exp4,
    UInt32,exp5,
    UInt32,exp6,
    UInt32,exp7,
    UInt32,exp8,
    UInt32,exp9
    )
SPECIALEND()

SPECIALBEGIN(GData::DBSSProb)
SPECIALDEF(10)
    (
    UInt16,id,
    UInt32,prob1,
    UInt32,prob2,
    UInt32,prob3,
    UInt32,prob4,
    UInt32,prob5,
    UInt32,prob6,
    UInt32,prob7,
    UInt32,prob8,
    UInt32,prob9
    )
SPECIALEND()

SPECIALBEGIN(GData::DBCopy)
SPECIALDEF(4)
    (
        UInt8, id,
        UInt8, floor,
        UInt8, spot,
        UInt32, fighterId
    )
SPECIALEND()

SPECIALBEGIN(GData::DBFrontMap)
SPECIALDEF(4)
    (
        UInt8, id,
        UInt8, spot,
        UInt8, count,
        UInt32, fighterId
    )
SPECIALEND()

SPECIALBEGIN(GData::DBTianjie)
SPECIALDEF(14)
    (
    int,id,
    bool,is_opened,
    bool,is_execute,
    bool,is_finish,
    bool,is_ok,
    int,level,
    int,rate,
    int,opentime,
    std::string, r1_killed,
    std::string, r2_donated,
    int, r3_copyid,
    UInt8, r4_day,
    UInt8, open_next,
    UInt8, is_wait
    )
SPECIALEND()

SPECIALBEGIN(GData::DBPlayerMaxLevel)
SPECIALDEF(1)
	(
	int,level
	)
SPECIALEND()

SPECIALBEGIN(GData::DBTianleizhen)
SPECIALDEF(4)
    (
    int,type,
    int,level,
    int,npcid,
    int,score
    )
SPECIALEND()

SPECIALBEGIN(GData::DBReportId)
SPECIALDEF(1)
	(
	int,id
	)
SPECIALEND()


}

#endif // _GDATADBEXECHELPER_H_

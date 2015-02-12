#ifndef _GDATADBEXECHELPER_H_
#define _GDATADBEXECHELPER_H_
#include "DB/DBExecHelper.h"


namespace GData
{
    struct DBItemType
    {
        UInt32      typeId;         // 物品编号
        std::string name;           // 名字
        UInt8       subClass;       // 物品类型 -
        UInt8       career;         // 职业
        UInt16      reqLev;         // 物品(可装备)等级
        UInt16      vLev;           // 物品(价值)等级
        UInt32      coin;           // 价格(铜)
        UInt8       quality;        // 品质 1-白 2-绿 3-蓝 4-紫 5-橙
        UInt16      maxQuantity;    // 最大重叠数
        UInt8       bindType;       // 是否绑定
        UInt16      energy;         // 九仪鼎值
        UInt16      trumpExp;       // 法宝经验
        UInt16      data;           // 可使用道具: 作用数值
        UInt16      enchant;        // 附魔类型
        UInt32      attrExtra;      // 属性附加
        float       salePriceUp;    // 交易价格上限 
    };
    struct DBExp  
    {
        UInt8 lvl; 
        UInt64 exp; 
    };
    struct DBReportId
    {
        int id;
    };

    struct DBSkillCondition  
    {
        UInt16 id; 
        std::string name; 
        UInt16 cond;
        UInt8 prob;
        UInt16 distance;
        UInt8 priority;
    };

    struct DBSkillScope  
    {
        UInt16 id; 
        std::string name; 
        UInt8 area;
        UInt32 x;
        UInt32 y;
        UInt32 radx;
        UInt32 rady;
    };

    struct DBSkillEffect  
    {
        UInt16 id; 
        std::string name; 
        UInt8 skillType;

        UInt16 damage;
        float damageP;

        UInt16 trerapy;
        float trerapyP;
    };

    struct DBFighterBase  
    {
        UInt32 id;
        std::string name;
        UInt8 color;
        UInt8 typeId;
        UInt8 childType;
        UInt16 speed;
        UInt16 bodySize;
        UInt32 hp;
        UInt32 attack;
        UInt32 defend;
        UInt32 magatk;
        UInt32 magdef;
        UInt32 critical;
        UInt32 criticalDef;
        UInt32 hit;
        UInt32 evade;
    //    std::string skill;
    };

    struct DBFighterBaseSkill  
    {
        UInt32 id;
        UInt8  levelLimit;
        UInt32 skillId;
    };

    struct DBSkill  
    {
        UInt32 id;
        std::string name;
        UInt32 skillCondId; 
        UInt32 skillScopeId;
        UInt32 skillEffectId;
        UInt16 cd;
        UInt16 actionCd;
        UInt16 actionBackCd;
    };
}
namespace DB
{
    SPECIALBEGIN(GData::DBItemType)
        SPECIALDEF(16)
        (
         UInt32,     typeId,
         std::string,name,
         UInt8,      subClass,
         UInt8,      career,
         UInt16,     reqLev,
         UInt16,     vLev,
         UInt32,     coin,
         UInt8,      quality,
         UInt16,     maxQuantity,
         UInt8,      bindType,
         UInt16,     energy,
         UInt16,     trumpExp,
         UInt16,     data,
         UInt16,     enchant,
         UInt32,     attrExtra,
         float,      salePriceUp
        )
        SPECIALEND()

        SPECIALBEGIN(GData::DBExp)   
        SPECIALDEF(2)
        (
         UInt8,          lvl,
         UInt64,         exp
        )
        SPECIALEND()

        SPECIALBEGIN(GData::DBReportId)
        SPECIALDEF(1)
        (
         int,id
        )
        SPECIALEND()

        SPECIALBEGIN(GData::DBSkillCondition)
        SPECIALDEF(6)
        (
         UInt16, id,
         std::string, name,
         UInt16, cond,
         UInt8, prob,
         UInt16, distance,
         UInt8, priority
        )
        SPECIALEND()

        SPECIALBEGIN(GData::DBSkillScope)
        SPECIALDEF(7)
        (
         UInt16, id,
         std::string, name,
         UInt8, area,
         UInt32, x,
         UInt32, y,
         UInt32, radx,
         UInt32, rady
        )
        SPECIALEND()

        SPECIALBEGIN(GData::DBSkillEffect)
        SPECIALDEF(7)
        (
         UInt16, id,
         std::string, name,
         UInt8, skillType,

         UInt16, damage,
         float, damageP,

         UInt16, trerapy,
         float, trerapyP
        )
        SPECIALEND()

        SPECIALBEGIN(GData::DBFighterBase)
        SPECIALDEF(12)
        (
         UInt32, id,
         std::string, name,
         UInt8, color,
         UInt8, typeId,
         UInt8, childType,
         UInt16, speed,
         UInt16, bodySize,
         UInt32, hp,
         UInt32, attack,
         UInt32, defend,
         UInt32, magatk,
         UInt32, magdef,
         UInt32, critical,
         UInt32, criticalDef,
         UInt32, hp,
         UInt32, evade
         //std::string, skill
        )
        SPECIALEND()

        SPECIALBEGIN(GData::DBFighterBaseSkill)
        SPECIALDEF(3)
        (
         UInt32, id,
         UInt8 , levelLimit,
         UInt32, skillId
        )
        SPECIALEND()

        SPECIALBEGIN(GData::DBSkill)
        SPECIALDEF(8)
        (
         UInt32, id,
         std::string , name,
         UInt32, skillCondId,
         UInt32, skillScopeId,
         UInt32, skillEffectId,
         UInt16, cd,
         UInt16, actionCd,
         UInt16, actionBackCd
        )
        SPECIALEND()

}
#endif


#ifndef SKILLSTRENGTHEN_H_
#define SKILLSTRENGTHEN_H_

#include "Config.h"
#include "ObjectManager.h"

namespace GData
{

#define SKILLSTRENGTHEN_LEVEL_MAX 100
#define SKILLSTRENGTHEN_LEVEL(x)  (((UInt16)(x))%SKILLSTRENGTHEN_LEVEL_MAX)
#define SKILLSTRENGTHEN_ID(x) (((UInt16)(x))/SKILLSTRENGTHEN_LEVEL_MAX)
#define SKILLSTRENGTHENANDLEVEL(s,l) (((UInt16)(s))*SKILLSTRENGTHEN_LEVEL_MAX + ((UInt16)(l)))

enum
{
    ON_PIERCE = 1,   // 破击时
    ON_CRITICAL,     // 暴击时
    ON_FORGET,       // 封印时
    ON_STUN,         // 晕眩时
    ON_SPREAD,       // 波及时
    ON_AURA,         // 回灵时
    ON_DEBUF,        // 减益时
    ON_THERAPY,      // 治疗时
    ON_DAMAGE,       // 伤害时
    ON_BEDAMAGE,     // 受伤害时
    ON_WEAK,         // 虚弱时
};

enum
{
    TYPE_AREA = 1,      // 技能范围扩展
    TYPE_DAMAG_B,       // 攻击目标后面的位置，针对单体技能
    TYPE_BLEED,         // 流血 儒
    TYPE_DAMAG_A,       // 受到攻击时伤害增加
    TYPE_XINMO,         // 心魔效果
    TYPE_DAMAG_REDUCE,  // 伤害减免
    TYPE_DEBUF_DEFEND,  // 受到攻击时防御减少
    TYPE_DISPERSE,      // 驱散
    TYPE_CRITICAL,      // 暴击效果
    TYPE_ABSORB,        // 吸血效果
    TYPE_IMMUNE,        // 免疫所有控制状态
    TYPE_DEBUF_THERAPY, // 减治疗效果
    TYPE_ATKADD,        // 增加攻击效果
    TYPE_MAX,
};

struct SkillStrengthenEffect : public ObjectBaseNT<UInt16>
{
    SkillStrengthenEffect(UInt16 id)
        : ObjectBaseNT<UInt16>(id), cond(0), target(0), prob(0), area(0), last(0), type(0), value(0) {}
    ~SkillStrengthenEffect() {}

    UInt16 cond;
    UInt8 target;
    float prob;
    UInt8 area;
    std::vector<float> factor;
    Int16 last;
    Int16 type;
    float value;
};

struct SkillStrengthenBase : public ObjectBaseT<UInt16>
{
    SkillStrengthenBase(UInt16 id, const std::string& name)
        : ObjectBaseT<UInt16>(id, name) {}
    ~SkillStrengthenBase() { effect.clear(); }

    std::vector<const SkillStrengthenEffect*> effect;
    const SkillStrengthenEffect* getEffect(UInt16 cond, Int16 type);
};


typedef ObjectMapT<SkillStrengthenBase, UInt16> SkillStrengthenManager;
#define skillStrengthenManager SkillStrengthenManager::Instance()

typedef ObjectMapT<SkillStrengthenEffect, UInt16> SkillStrengthenEffectManager;
#define skillStrengthenEffectManager SkillStrengthenEffectManager::Instance()

} // namespace GData

#endif // SKILLSTRENGTHEN_H_



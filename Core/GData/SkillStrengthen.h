
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
    ON_CRITICAL = 2,     // 暴击时
    ON_FORGET = 3,       // 封印时
    ON_STUN = 4,         // 晕眩时
    ON_SPREAD = 5,       // 波及时
    ON_AURA = 6,         // 回灵时
    ON_DEBUF = 7,        // 减益时
    ON_THERAPY = 8,      // 治疗时
    ON_DAMAGE = 9,       // 伤害时
    ON_BEDAMAGE = 10,     // 受伤害时
    ON_WEAK = 11,         // 虚弱时
    ON_ATTACKSINGLE = 12, // 群攻只攻击到一个目标时
    ON_SKILLUSED = 13,   // 技能使用之后
    ON_RESIST = 14,    // 状态被抵抗时
    ON_USEHAOTIAN = 15,  // 使用昊天镜的时候。。。
    ON_TARGET_DEAD = 16, // 目标受攻击死亡
    ON_BLIND = 17,      // 至盲时
    ON_ATTACK = 18 ,     // 攻击时
    ON_SUMMON = 19,     // 召唤时
    ON_HIDE = 20,       // 隐身时
    ON_BLEED = 21,     // 流血时
    ON_CONFUSE = 22,   // 混乱时
    ON_HITE = 23,      // 命中时
};

enum
{
    TYPE_AREA = 1,      // 技能范围扩展
    TYPE_DAMAG_B = 2,       // 攻击目标后面的位置，针对单体技能
    TYPE_BLEED = 3,         // 流血 儒
    TYPE_DAMAG_A = 4,       // 受到攻击时伤害增加
    TYPE_XINMO = 5,         // 心魔效果
    TYPE_DAMAG_REDUCE = 6,  // 伤害减免
    TYPE_DEBUF_DEFEND = 7,  // 受到攻击时防御减少
    TYPE_DISPERSE = 8,      // 驱散
    TYPE_CRITICAL = 9,      // 暴击效果
    TYPE_ABSORB = 10,        // 吸血效果
    TYPE_IMMUNE = 11,        // 免疫所有控制状态
    TYPE_DEBUF_THERAPY = 12, // 减治疗效果
    TYPE_ATKADD = 13,        // 增加攻击效果
    TYPE_ABSORB_ATTACK = 14, // 吸收目标物理攻击，持续到一次有效攻击
    TYPE_YUANCISHENLEI = 15, // 元磁神雷攻击特殊效果
    TYPE_INTENSIFYSTATE = 16, // 强化技能效果
    TYPE_ADDMAGICATK = 17,   // 增加法伤(取自己的法伤当因子，楼下的是取对方的当因子)
    TYPE_ABSORB_MAGATK = 18,
    TYPE_ADDSTATE = 19,     // 上状态
    TYPE_HAOTIANJING = 20,  // 借用一下昊天镜的名字，这个状态其实就是升自己命中，降对方反击
    TYPE_RANDOM_BLEED = 21, // 随机一人流血
    TYPE_AURA_RETURN = 22, //减少的灵气X%平均附加给同伴（除了自己）
    TYPE_ATK_RETURN = 23, //减少的攻击，转换X%给己方攻击最高的同伴，持续一次（last）攻击（主动出手的攻击）
    TYPE_BUF_THERAPY = 24, // 增治疗效果
    TYPE_DEBUF_AURA = 25, //被攻击的目标下2（last）次获得的灵气时，每次减少X点
    TYPE_DAMAGE_EXTRA = 26, // 被抵抗也流血
    TYPE_ATTACK_FRIEND = 27,// 攻击本方血最少的人
    TYPE_BLEED_BYSKILL = 28, // 技能导致流血，取技能的攻击力，hp改变和hp改变附加
    TYPE_AURA_GET = 29,   // 获取灵气
    TYPE_RESIST_ADDSTATE = 30, // 被抵抗上状态
    TYPE_THERAPY_ANOTHER_MORE = 31, // 多治疗一个目标
    TYPE_DAMAGE_AROUND_ONE = 32, // 浅射附近的一个人
    TYPE_HITRATE_EXTRA = 33, // 增加当前额外命中率
    TYPE_UNSUMMON_AURA = 34, // 残影消失时召唤者增加灵气
    TYPE_DEC_COUNTER = 35, // 减敌方当前反击率
    TYPE_HITRATE_COUNTER = 36, // 增加当前命中减敌方当前反击率
    TYPE_SHIELD_HP = 37,   // 生命护盾
    TYPE_FAKE_DEAD = 38,   // 概率不死
    TYPE_MAX,
};

struct SkillStrengthenEffect : public ObjectBaseNT<UInt16>
{
    SkillStrengthenEffect(UInt16 id)
        : ObjectBaseNT<UInt16>(id), cond(0), target(0), prob(0), area(0), last(0), type(0), value(0), valueExt1(0), valueExt2(0) {}
    ~SkillStrengthenEffect() {}

    UInt16 cond;
    UInt8 target;
    float prob;
    UInt8 area;
    std::vector<float> factor;
    Int16 last;
    Int16 type;
    float value;
    float valueExt1;
    float valueExt2;
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




#ifndef SKILLTABLE_H_
#define SKILLTABLE_H_

#include "Config.h"
#include "ObjectManager.h"

namespace GData
{

#define SKILL_LEVEL_MAX 100
#define SKILL_LEVEL(x)  (((UInt16)(x))%SKILL_LEVEL_MAX)
#define SKILL_ID(x) (((UInt16)(x))/SKILL_LEVEL_MAX)
#define SKILLANDLEVEL(s,l) (((UInt16)(s))*SKILL_LEVEL_MAX + ((UInt16)(l)))

#define CITTA_LEVEL(x) (((UInt16)(x))%CITTA_LEVEL_MAX)
#define CITTA_ID(x) (((UInt16)(x))/CITTA_LEVEL_MAX)
#define CITTAANDLEVEL(c,l) (((UInt16)(c))*CITTA_LEVEL_MAX + ((UInt16)(l)))
#define CITTA_ITEMID(x) ((UInt16)(x) / CITTA_LEVEL_MAX + LCITTA_ID - 1)

enum
{
    /*0*/ SKILL_ACTIVE = 0,
    /*1*/ SKILL_PEERLESS,
    SKILL_PASSSTART,
    /*2*/ SKILL_PREATK = SKILL_PASSSTART,
    /*3*/ SKILL_AFTATK,
    /*4*/ SKILL_BEATKED,
    /*5*/ SKILL_AFTEVD,
    /*6*/ SKILL_AFTRES,
    /*7*/ SKILL_ENTER,
    /*8*/ SKILL_DEAD,
    /*9*/ SKILL_AFTNATK,
    /*10*/ SKILL_ONTHERAPY,
    /*11*/ SKILL_ONSKILLDMG,
    /*12*/ SKILL_ONOTHERDEAD,
    /*13*/ SKILL_ONCOUNTER,
    /*14*/ SKILL_ONATKBLEED,
    /*15*/ SKILL_ONATKDMG,
    /*16*/ SKILL_ONPETPROTECT,
    SKILL_PASSIVES
};

enum
{
    SKILL_EFFECT_FALG_VALUE = 30000
};

enum
{
    e_battle_target_selfside  = 0,
    e_battle_target_otherside = 1,
    e_battle_target_self      = 2,
    e_battle_target_otherside_max = 3,
    e_battle_target_otherside_min = 4,
    e_battle_target_selfside_max = 5,
    e_battle_target_selfside_min = 6,
    e_battle_target_selfside_atk_max = 7,
};


// 技能附加特效类型:
enum
{
    e_eft_hide = 1, // 潜行 
    e_eft_double_hit = 2, // 连击 
    e_eft_mark_hide_dhit = 3, // (墨印或潜行时)加连击
    e_eft_mark_hide_blind = 4, // (墨印或潜行时)至盲
    e_eft_selfside_ru_shi_magatk = 5, // (友方全体,队友为释儒)加法术攻击
    e_eft_selfside_dao_dmgreduce = 6, // (友方全体,队友为道)加伤害减免
    e_eft_hide_attack = 7, // (潜行时)加攻击
    e_eft_mark_hide_week = 8, // (墨印或潜行时)加虚弱
    e_eft_hide_summon = 9, // (潜行时)召唤潜行的残影
    e_eft_rnd_fgt_buf_aura = 10, // 随机队友获得心动后涨灵气的buf
    e_eft_evade100 = 11, // 百分百闪避一次主动攻击
    e_eft_selfside_buf_aura = 12, // 全体队友获得心动后涨灵气的buf
    e_eft_selfside_absorb = 13, // 给队友吸血
    e_eft_hide_aura = 14, // (墨印或潜行时)敌方被攻击时不增加灵气
    e_eft_counter_hate = 15, // 反击后的仇恨值
    e_eft_hp_shield = 16,       // 释放自己生命值百分比的护盾
    e_eft_self_bleed = 17,           // 给自己加点燃效果（自焚？）
    e_eft_random_shield = 18,        // 随机释放护盾
    e_eft_self_attack = 19,        // 附加自己百分比攻击力
    e_eft_random_target_attack = 20,    // 随机选择人加攻击力
    e_eft_mark_pet  = 21,               // 神兽印记
    e_eft_atk_pet_mark_aura = 22,       // 攻击带神兽印记的涨自身灵气
    e_eft_atk_pet_mark_extra_dmg = 23,  // 攻击带神兽印记的造成额外攻击
    e_eft_protect_pet_100 = 24,         // 宠物100%保护目标

    e_eft_max
};

enum
{
    e_state_poison = 0x1, // 中毒
    e_state_confuse = 0x2, // 混乱
    e_state_stun = 0x4, // 晕眩
    e_state_forget = 0x8, // 封印
    e_state_dmgback = 0x10, // 伤害反弹
    e_state_weak = 0x20, // 虚弱
    e_state_dec_aura = 0x40, // 减灵气
    e_state_mark_mo = 0x80, // 墨印
    e_state_blind = 0x100, // 至盲

    e_state_c_s_f_w = 0x2e, // 混乱，晕眩，封印，虚弱
    e_state_c_s_f_m_b = 0x18e, // 混乱，晕眩，封印，墨印，至盲
    e_state_c_s_f_b = 0x10e, // 混乱，晕眩，封印，至盲
    e_state_c_s_f_w_m_b = 0x1ae, // 混乱，晕眩，封印，虚弱，墨印，至盲
};


struct SkillEffect : public ObjectBaseNT<UInt16>
{
    SkillEffect(UInt16 id)
        : ObjectBaseNT<UInt16>(id), state(0), immune(0), disperse(0),
        damage(0), damageP(0), adddam(0), magdam(0), magdamP(0), addmag(0), crrdam(0), crrdamP(0), addcrr(0),
        hp(0), hpP(0), addhp(0), absorb(0), absorbP(0), thorn(0), thornP(0),inj2hp(0), inj2hpP(0),
        aura(0), auraP(0), atk(0), atkP(0), def(0), defP(0), magatk(0),
        magatkP(0), magdef(0), magdefP(0), tough(0), action(0), actionP(0), hitrate(0), evade(0),
        critical(0), pierce(0), counter(0), magres(0), atkreduce(0), magatkreduce(0), hppec(0), maxhpdampec(0) {}
    ~SkillEffect() {}

    UInt16 state; // 状态: 0-无状态 1-中毒，2-混乱，4-晕眩(无法攻击)，8-无法使用技能, 16-反伤, 32-虚弱, 64-降灵气 有等级之分
    UInt16 immune; // 对状态技能的免疫,只能免疫比自己技能低的技能
    UInt16 disperse; // 驱散状态,只对友方使用,除自己外,是状态的值的和
    Int16 damage; // 物理伤害 num/num% (目前物理伤害和法术伤害互斥)
    float damageP;
    float adddam; // 物理伤害附加(具体值)
    Int16 magdam; // 法术伤害 num/num%
    float magdamP;
    float addmag; // 法术伤害附加(具体值)
    Int16 crrdam; // 职业伤害 num/num%
    float crrdamP;
    float addcrr; // 职业伤害附加(具体值)
    Int16 hp; // HP改变 [+/-]num/num%
    float hpP;
    float addhp; // HP改变附加(具体值)[+/-]
    Int16 absorb; // 伤害吸血 num/num%
    float absorbP;
    Int16 thorn; // 反弹 num/num%
    float thornP;
    Int16 inj2hp; // 受伤回扣 num/num%
    float inj2hpP;
    Int16 aura; // 作用士气 [+/-]num/num%
    float auraP;
    Int16 atk; // 物理攻击 [+/-]num/num%
    float atkP;
    Int16 def; // 物理防御 [+/-]num/num%
    float defP;
    Int16 magatk; // 法术攻击 [+/-]num/num%
    float magatkP;
    Int16 magdef; // 法术防御 [+/-]num/num%
    float magdefP;
    float tough; // 坚韧[+/-]
    float action; // 身法[+/-]num/num%
    float actionP;
    float hitrate; // 命中[+/-]
    float evade; // 闪避[+/-]
    float critical; // 暴击[+/-]
    float pierce; // 击破/护甲穿透[+/-]
    float counter; // 反击[+/-]
    float magres; // 法术抵抗[+/-]
    float atkreduce; // 物理伤害减免
    float magatkreduce; // 法术伤害减免
    float hppec;        // 最大生命值伤害百分比
    float maxhpdampec;  // 最大生命值伤害百分比最大值（最高攻击力的百分比）

    // 技能附加特效类型:
    std::vector<UInt16> eft;
    std::vector<UInt8> efl; // 技能附加特效持续回合
    std::vector<float> efv; // 技能附加特效值
};

struct SkillBase : public ObjectBaseT<UInt16>
{
    SkillBase(UInt16 id, const std::string& name)
        : ObjectBaseT<UInt16>(id, name), target(0), cond(0),
        prob(0), area(0), last(0), cd(0), effect(0) {}
    ~SkillBase() { if (effect) delete effect; }

    UInt8 color;               // 技能颜色 1-白色 2-绿色 3-蓝色 4-紫色 5-橙色
    UInt8 target;              // 作用对象: 0-友方,1-敌方,2-自己(对友方和自己加,对敌方减)
                               //           3-敌方血量最高 4-敌方血量最低
                               //           5-我方血量最高 6-我方血量最低
    UInt16 cond;               // 触发条件: SKILL_ACTIVE     - 主动
                               //           SKILL_PEERLESS   - 无双技能,当灵气>=100释放
                               //           SKILL_PREATK     - 攻击前被动触发(回血技能,无概率)
                               //           SKILL_AFTATK     - 攻击后被动触发(有概率)
                               //           SKILL_AFTNATK    - 普通攻击后被动触发(有概率)
                               //           SKILL_BEATKED    - 被攻击后触发(有概率)
                               //           SKILL_AFTEVD     - 闪避后触发
                               //           SKILL_AFTRES     - 抵抗后触发
                               //           SKILL_ENTER      - 入场时触发
                               //           SKILL_DEAD       - 死亡后触发
                               //           SKILL_ONCOUNTER  - 反击后触发
                               //           SKILL_ONATKBLEED - 攻击流血对象触发
    float prob;                // 主动状态触发概率 或 被动触发概率
    UInt8 area;                // 伤害范围: 0-单体,1-全体,2-横排,3-竖列,4-十字,5-V字,6-T字
    std::vector<float> factor; // 伤害倍率: 如, 横排伤害 1,0.3,0.5,1,0 距离攻击目标为0的伤害系数是1,距离为2的伤害系数为0.5
    Int16 last;                // 持续时间: -1-一直有效,0-非持续,N-持续次数
    UInt16 cd;                 // 冷却回合
    const SkillEffect* effect;
};

struct SkillItem
{
    const SkillBase* base;
    float rateExtent;
    UInt16 cd;
};

typedef ObjectMapT<SkillBase, UInt16> SkillManager;
#define skillManager SkillManager::Instance()

typedef ObjectMapT<SkillEffect, UInt16> SkillEffectManager;
#define skillEffectManager SkillEffectManager::Instance()

} // namespace GData

#endif // CITTA_H

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


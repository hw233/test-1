#ifndef LBSKILLTABLE_H_
#define LBSKILLTABLE_H_

#include "Config.h"
#include "ObjectManager.h"

namespace GData
{

enum
{
    // 触发条件
    e_lb_cond_skill = 1,      // 针对特定技能
    e_lb_cond_action1 = 2,    // 首回合先于目标行动（攻击前）
    e_lb_cond_dead = 3,       // 死亡后触发
    e_lb_cond_bleed = 4,      // 流血时
    e_lb_cond_state = 5,      // 中状态时(致盲，沉默，减灵气，混乱)
    e_lb_cond_action2 = 6,    // 首回合先于目标行动（攻击时）
};

// 子条件：
// 当触发条件为 针对特定技能，子条件填技能id
// 当触发条件为中状态时 子条件天相应状态

enum
{
    // 技能效果:
    e_lbef_dmg = 1, // 伤害(概率）
    e_lbef_def_add = 2, // 加防御(概率）
    e_lbef_def_dec = 3, // 减防御(概率）
    e_lbef_cri = 4, // 额外暴击(概率）
    e_lbef_pri = 5, // 额外破击(概率）
    e_lbef_atk_add = 6, // 加攻击(概率）
    e_lbef_atk_dec = 7, // 减攻击(概率）
    e_lbef_state = 8, // 状态(晕眩，混乱，减灵气， 致盲)(概率）
    e_lbef_aura = 9, // 加灵气(概率）
    e_lbef_hpshield = 10, // 护盾(概率）
    e_lbef_bleed_dec = 11, // 流血减免
    e_lbef_magres = 12, // 状态抵抗(概率）
    e_lbef_bleed_out = 13, // 解除流血(概率）
};



struct LBSkillBase: public ObjectBaseT<UInt16>
{
    LBSkillBase(UInt16 id, const std::string& name)
        : ObjectBaseT<UInt16>(id, name)
        {}

    ~LBSkillBase() {}

    UInt8 lbtype;              // 灵宝类型(灵,悟,性)
    UInt8 level;               // 灵宝等级
    UInt8 target;              // 作用对象: 0-友方,1-敌方,2-自己(对友方和自己加,对敌方减)
    float  prob;               // 触发概率
    UInt16 cond;               // 触发条件:
    UInt16 cond2;              // 子条件
    UInt8 area;                // 伤害范围: 0-单体,1-全体,2-横排,3-竖列,4-十字,5-V字,6-T字, 9-随机2人, 10-随机3人, 11-随机4人
    std::vector<float> factor; // 伤害倍率: 如, 横排伤害 1,0.3,0.5,1,0 距离攻击目标为0的伤害系数是1,距离为2的伤害系数为0.5
    Int16 last;                // 持续时间: -1-一直有效,0-非持续,N-持续次数
    UInt16 cd;                 // 冷却回合

    UInt16 ef_type;            // 技能效果类型
    UInt16 minFactor;          // 最低系数
    UInt16 ef_value;           // 技能效果值1
    UInt16 battlepoint;        // 战力
};

struct LBSkillItem
{
    const LBSkillBase* base;
    UInt16 cd;
    float  prob;
    UInt16 ef_value;
};

typedef ObjectMapT<LBSkillBase, UInt16> LBSkillManager;
#define lbSkillManager LBSkillManager::Instance()

}

#endif

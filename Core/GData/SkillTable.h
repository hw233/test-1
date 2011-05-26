
#ifndef SKILL_H_
#define SKILL_H_

#include "Config.h"
#include "ObjectManager.h"

namespace GData
{

struct SkillEffect : public ObjectBaseNT<UInt16>
{
    SkillEffect(UInt16 id) : ObjectBaseNT<UInt16>(id),
    state(0), stateprob(0), immune(0), damage(0), damageP(0), 
    magdam(0), magdamP(0), hp(0), hpP(0), aura(0), auraP(0),
    hitCount(0), def(0), defP(0), magdef(0), magdefP(0), evade(0),
    pierce(0), adddam(0) {}
    UInt8 state;
    float stateprob;
    UInt8 immune;
    UInt16 damage;
    float damageP;
    UInt16 magdam;
    float magdamP;
    UInt16 hp;
    float hpP;
    UInt16 aura;
    float auraP;
    UInt8 hitCount;
    UInt16 def;
    float defP;
    UInt16 magdef;
    float magdefP;
    float evade;
    float pierce;
    float adddam;
};

struct Skill : public ObjectBaseT<UInt16>
{
    Skill(UInt16 id, std::string& name) : ObjectBaseT<UInt16>(id, name),
    target(0), cond(0), prob(0), area(0), last(0), cd(0), effect(0) {}
    UInt8 target;
    UInt16 cond;
    float prob;
    UInt8 area;
    std::vector<float> factor;
    UInt16 last;
    UInt16 cd;
    const SkillEffect* effect;
};

typedef ObjectMapT<Skill, UInt16> SkillManager;
#define skillManager SkillManager::Instance()

typedef ObjectMapT<SkillEffect, UInt16> SkillEffectManager;
#define skillEffectManager SkillEffectManager::Instance()

} // namespace GData

#endif // SKILL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


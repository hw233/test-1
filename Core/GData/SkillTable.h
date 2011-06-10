
#ifndef SKILLTABLE_H_
#define SKILLTABLE_H_

#include "Config.h"
#include "ObjectManager.h"

namespace GData
{

struct SkillEffect : public ObjectBaseNT<UInt16>
{
    SkillEffect(UInt16 id)
        : ObjectBaseNT<UInt16>(id), state(0), immune(0), 
        damage(0), damageP(0), adddam(0), magdam(0), magdamP(0), addmag(0),
        hp(0), hpP(0), addhp(0), absorb(0), absorbP(0), aura(0), auraP(0), atk(0), atkP(0),
        def(0), defP(0), magatk(0), magatkP(0), magdef(0), magdefP(0), 
        tough(0), action(0), evade(0), critical(0), pierce(0), counter(0), magres(0) {}
    ~SkillEffect() {}

    UInt8 state;
    UInt8 immune;
    Int16 damage;
    float damageP;
    float adddam;
    Int16 magdam;
    float magdamP;
    float addmag;
    Int16 hp;
    float hpP;
    float addhp;
    Int16 absorb;
    float absorbP;
    Int16 aura;
    float auraP;
    Int16 atk;
    float atkP;
    Int16 def;
    float defP;
    Int16 magatk;
    float magatkP;
    Int16 magdef;
    float magdefP;
    float tough;
    float action;
    float evade;
    float critical;
    float pierce;
    float counter;
    float magres;
};

struct SkillBase : public ObjectBaseT<UInt16>
{
    SkillBase(UInt16 id, const std::string& name)
        : ObjectBaseT<UInt16>(id, name), target(0), cond(0),
        prob(0), area(0), last(0), cd(0), effect(0) {}
    ~SkillBase() { if (effect) delete effect; }

    UInt8 target;
    UInt16 cond;
    float prob;
    UInt8 area;
    std::vector<float> factor;
    Int16 last;
    UInt16 cd;
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


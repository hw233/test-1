
#ifndef CITTATABLE_H_
#define CITTATABLE_H_

#include "Config.h"
#include "ObjectManager.h"
#include "SkillTable.h"

namespace GData
{

struct CittaEffect : public ObjectBaseNT<UInt16>
{
    CittaEffect(UInt16 id)
        : ObjectBaseNT<UInt16>(id), strength(0), strengthP(0), physique(0), physiqueP(0),
    agility(0), agilityP(0), intelligence(0), intelligenceP(0), will(0), willP(0),
    soul(0), soulP(0), aura(0), auraP(0), auraMax(0), auraMaxP(0), attack(0), attackP(0),
    defend(0), defendP(0), mag_defend(0), mag_defendP(0), hp(0), hpP(0), 
    action(0), hitrate(0), evade(0), critical(0), critical_dmg(0), pierce(0),
    counter(0), magres(0), practice(0) {}
    ~CittaEffect() { skill.clear(); }

    UInt16 strength;
    float strengthP;
    UInt16 physique;
    float physiqueP;
    UInt16 agility;
    float agilityP;
    UInt16 intelligence;
    float intelligenceP;
    UInt16 will;
    float willP;
    UInt16 soul;
    float soulP;
    UInt16 aura;
    float auraP;
    UInt16 auraMax;
    float auraMaxP;
    // UInt16 tough;
    // UInt16 toughP;
    UInt16 attack;
    float attackP;
    UInt16 mag_attack;
    float mag_attackP;
    UInt16 defend;
    float defendP;
    UInt16 mag_defend;
    float mag_defendP;
    UInt16 hp;
    float hpP;
    float action;
    float hitrate;
    float evade;
    float critical;
    float critical_dmg;
    float pierce;
    float counter;
    float magres;
    float practice;                 // 修炼速度加成
    std::vector<const SkillBase*> skill;  // 带出技能
};

struct CittaBase : public ObjectBaseT<UInt16>
{
    CittaBase(UInt16 id, std::string& name)
        : ObjectBaseT<UInt16>(id, name) {}

    ~CittaBase() { if (effect) delete effect; }

    UInt16 needsoul;
    const CittaEffect* effect;
};

typedef ObjectMapT<CittaBase, UInt16> CittaManager;
#define cittaManager CittaManager::Instance()

typedef ObjectMapT<CittaEffect, UInt16> CittaEffectManager;
#define cittaEffectManager CittaEffectManager::Instance()

} // namespace GData

#endif // CITTATABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


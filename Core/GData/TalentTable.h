
#ifndef TALENTTABLE_H_
#define TALENTTABLE_H_

#include "Config.h"
#include "ObjectManager.h"

namespace GData
{

struct Talent : public ObjectBaseT<UInt16>
{
    Talent(UInt16 id, const std::string& name)
        : ObjectBaseT<UInt16>(id, name), cls(0), quality(0), prob(0), potential(0), capacity(0),
        strength(0), strengthP(0), physique(0), physiqueP(0), agility(0), agilityP(0),
        intelligence(0), intelligenceP(0), will(0), willP(0), soul(0), soulP(0), aura(0), auraP(0),
        auraMax(0), auraMaxP(0), attack(0), attackP(0), magatk(0), magatkP(0), defend(0), defendP(0),
        magdef(0), magdefP(0), hp(0), hpP(0), tough(0), action(0), hitrate(0), evade(0), critical(0),
        criticaldmg(0), pierce(0), counter(0), magres(0) {}
    ~Talent() {}

    UInt8 cls;
    UInt8 quality;
    float prob;
    float potential;
    float capacity;
    Int16 strength;
    float strengthP;
    Int16 physique;
    float physiqueP;
    Int16 agility;
    float agilityP;
    Int16 intelligence;
    float intelligenceP;
    Int16 will;
    float willP;
    Int16 soul;
    float soulP;
    Int16 aura;
    float auraP;
    Int16 auraMax;
    float auraMaxP;
    Int16 attack;
    float attackP;
    Int16 magatk;
    float magatkP;
    Int16 defend;
    float defendP;
    Int16 magdef;
    float magdefP;
    Int16 hp;
    float hpP;
    float tough;
    float action;
    float hitrate;
    float evade;
    float critical;
    float criticaldmg;
    float pierce;
    float counter;
    float magres;
};

typedef ObjectMapT<Talent, UInt16> TalentManager;
#define talentManager TalentManager::Instance()

} // namespace GData

#endif // TALENTTABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


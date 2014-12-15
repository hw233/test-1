#ifndef _ATTREXTRA_H_
#define _ATTREXTRA_H_

#include "Config.h"
#include "ObjectManager.h"
//#include "CittaTable.h"
//#include "GObject/AttrFactor.h"

namespace GData
{

struct SkillBase;

struct AttrExtra
{
    AttrExtra():hp(0),attack(0),defend(0),magic_attack(0),magic_defend(0),critical(0),evade(0)
    { } 
    AttrExtra& operator +=(const AttrExtra& other)
    { 
         hp += other.hp;
         attack += other.attack;
         defend += other.defend;
         magic_attack += other.magic_attack;
         magic_defend += other.magic_defend;
         critical += other.critical;
         evade += other.evade;
         return *this;
    } 

    AttrExtra& operator *=(const float rate)
    { 
         hp *=rate;
         attack *= rate;
         defend *= rate;
         magic_attack *= rate;
         magic_defend *= rate;
         critical *= rate;
         evade *= rate;
         return *this;
    } 

    UInt32 hp;
    UInt32 attack;
    UInt32 defend;
    UInt32 magic_attack;
    UInt32 magic_defend;
    UInt32 critical;
    UInt32 evade;
};

struct AttrExtraItem:
    public ObjectBaseNT<>
{
    AttrExtraItem(UInt32 idx): ObjectBaseNT<>(idx) { }
    AttrExtra _extra;
    inline AttrExtra* operator ->() { return &_extra; }
    inline operator const AttrExtra*() const { return &_extra; }
};

typedef ObjectListT<AttrExtraItem> AttrExtraManager;

#define attrExtraManager AttrExtraManager::Instance()

}

#endif // _ATTREXTRA_H_

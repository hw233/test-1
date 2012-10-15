#ifndef SHSY_TEMPLATE
#define SHSY_TEMPLATE

#include "Script/Script.h"
#include "GData/AttrExtra.h"

namespace GObject
{
    class SHSYTmpl
    {
    public:
        SHSYTmpl();
        void load(const char * = NULL);
        inline void setFilename(const char * fn) {_filename = fn;}

        inline void setStrength(UInt8 cls, float strength) { _attrExtraEquip[cls].strength = strength; }
        inline void setPhysique(UInt8 cls, float physique) { _attrExtraEquip[cls].physique = physique; }
        inline void setAgility(UInt8 cls, float agility) { _attrExtraEquip[cls].agility = agility; }
        inline void setIntelligence(UInt8 cls, float intelligence) { _attrExtraEquip[cls].intelligence = intelligence; }
        inline void setWill(UInt8 cls, float will) { _attrExtraEquip[cls].will = will; }
        inline void setSoul(UInt8 cls, float soul) { _attrExtraEquip[cls].soul = soul; }
        inline void setAura(UInt8 cls, float aura) { _attrExtraEquip[cls].aura = aura; }
        inline void setAuraMax(UInt8 cls, float auraMax) { _attrExtraEquip[cls].auraMax = auraMax; }
        inline void setAttack(UInt8 cls, float attack) { _attrExtraEquip[cls].attack = attack; }
        inline void setMagAtk(UInt8 cls, float magatk) { _attrExtraEquip[cls].magatk = magatk; }
        inline void setDefend(UInt8 cls, float defend) { _attrExtraEquip[cls].defend = defend; }
        inline void setMagdef(UInt8 cls, float magdef) { _attrExtraEquip[cls].magdef = magdef; }
        inline void setHP(UInt8 cls, float hp) { _attrExtraEquip[cls].hp = hp; }
        inline void setTough(UInt8 cls, float tough) { _attrExtraEquip[cls].tough = tough; }
        inline void setAction(UInt8 cls, float action) { _attrExtraEquip[cls].action = action; }
        inline void setHitrate(UInt8 cls, float hitrate) { _attrExtraEquip[cls].hitrate = hitrate; }
        inline void setEvade(UInt8 cls, float evade) { _attrExtraEquip[cls].evade = evade; }
        inline void setCritical(UInt8 cls, float critical) { _attrExtraEquip[cls].critical= critical; }
        inline void setCriticalDmg(UInt8 cls, float criticaldmg) { _attrExtraEquip[cls].criticaldmg = criticaldmg; }
        inline void setPierce(UInt8 cls, float pierce) { _attrExtraEquip[cls].pierce = pierce; }
        inline void setCounter(UInt8 cls, float counter) { _attrExtraEquip[cls].counter = counter; }
        inline void setMagRes(UInt8 cls, float magres) { _attrExtraEquip[cls].magres = magres; }
        inline void setLevel(UInt8 level) { _level = level; }
        inline void setPotential(float potential) { _potential = potential; }
        inline void setSoulMax(Int32 soulmax) { _soulMax = soulmax; }

        GData::AttrExtra _attrExtraEquip[4];
        UInt8 _level;
        float _potential;
        Int32 _soulMax;

    private:
        std::string _filename;
    };

    extern SHSYTmpl shsyTmpl;
}


namespace Script
{
    class SHSYTmplScript:
        public Script
    {
    public:
        SHSYTmplScript(GObject::SHSYTmpl * shsytmpl);
    };

}

#endif  //SHSY_TEMPLATE

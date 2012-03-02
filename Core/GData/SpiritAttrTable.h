
#ifndef SPIRIT_ATTR_TYPE_H_
#define SPIRIT_ATTR_TYPE_H_

#include "Config.h"

namespace GData
{

struct SpiritAttr
{
    SpiritAttr() : attack(0), defend(0), critical_lvl(0), pierce_lvl(0), action(0), tough_lvl(0), critical_dmg(0), hp(0) {}

    float attack;
    float defend;
    float critical_lvl;
    float pierce_lvl;
    float action;
    float tough_lvl;
    float critical_dmg;
    float hp;
};

typedef std::vector<SpiritAttr> SpiritAttrTable;

extern SpiritAttrTable spiritAttrTable;

} // namespace GData

#endif // CITTA_H 

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */



#ifndef ACUPRA_H_
#define ACUPRA_H_

#include "Config.h"
#include "ObjectManager.h"

namespace GData
{

struct AcuPra : public ObjectBaseNT<UInt16>
{
    AcuPra(const UInt16& id) : ObjectBaseNT<UInt16>(id) {}

    UInt8 needlvl;
    UInt32 pra;
    UInt32 soulmax;
    UInt32 pramax;
    UInt32 citslot;
};

typedef ObjectMapT<AcuPra, UInt16> AcuPraManager;
#define acupraManager AcuPraManager::Instance()

struct AcuPraGold : public ObjectBaseNT<UInt16>
{
    AcuPraGold(const UInt16& id) : ObjectBaseNT<UInt16>(id) {}

    UInt32 useReal;
//    UInt32 praadd;
    UInt32 soulmax;
    UInt8 attrNum;
    float attrValue;
};

typedef ObjectMapT<AcuPraGold, UInt16> AcuPraGoldManager;
#define acupraGoldManager AcuPraGoldManager::Instance()
}

#endif // ACUPRA_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


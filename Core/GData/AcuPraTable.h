
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

}

#endif // ACUPRA_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


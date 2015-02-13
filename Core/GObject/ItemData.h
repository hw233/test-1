#ifndef _ITEMDATA_H_
#define _ITEMDATA_H_

#include "Common/Stream.h"

#define ENCHANT_LEVEL_MAX 10
#define TRUMP_ENCHANT_LEVEL_MAX 8
#define SOCKETS_MAX 6

namespace GObject
{
    struct ItemData
    {
        UInt32	id;
        UInt64	ownerId;
        UInt16	itemNum;
        UInt8   bindType;

        bool operator < (const ItemData& item) const
        {
            return ownerId < item.ownerId;
        }
    };
}

#endif

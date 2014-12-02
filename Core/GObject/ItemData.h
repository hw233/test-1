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

    struct ItemEquipData
    {
        UInt8   sockets;            //孔数                                                                                        
        UInt8   enchant;            //强化级别
        UInt8   tRank;              //法宝当前阶
        UInt8   maxTRank;           //法宝最大阶
        UInt32  trumpExp;           //法宝经验
        UInt16  gems[SOCKETS_MAX];          //宝石镶嵌增加附加属性
        //ItemEquipAttr2 extraAttr2;
        //ItemEquipSpiritAttr spiritAttr;

        ItemEquipData(): sockets(0), enchant(0), tRank(0), maxTRank(0), trumpExp(0) { memset(gems, 0, sizeof(gems)); }
    };
}

#endif

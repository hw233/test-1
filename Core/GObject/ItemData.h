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

	struct ItemEquipAttr2
	{
		ItemEquipAttr2(): type1(0), type2(0), type3(0), value1(0), value2(0), value3(0)  { }
		UInt8 type1;
		UInt8 type2;
		UInt8 type3;
		Int16 value1;
		Int16 value2;
		Int16 value3;
		inline UInt8 getCount()
		{
			if(type1 != 0)
			{
				if(type2 != 0)
				{
					if(type3 != 0)
						return 3;
					return 2;
				}
				return 1;
			}
			return 0;
		}
		inline void appendAttrToStream(Stream& st)
		{
			if(type1 != 0)
			{
				if(type2 != 0)
				{
					if(type3 != 0)
						st << static_cast<UInt8>(3) << type1 << value1 << type2 << value2 << type3 << value3;
					else
						st << static_cast<UInt8>(2) << type1 << value1 << type2 << value2;
				}
				else
					st << static_cast<UInt8>(1) << type1 << value1;
			}
			else
				st << static_cast<UInt8>(0);
		}
	};
    // 装备注灵属性
    struct ItemEquipSpiritAttr
    {
        UInt16  spLev[4];             // 注灵等级
        UInt8   spForm[3];            // 注灵公式

        ItemEquipSpiritAttr() { memset(spLev, 0, sizeof(spLev)); memset(spForm, 0, sizeof(spForm)); }

		inline void appendAttrToStream(Stream& st)
        {
            st << spLev[0] << spLev[1] << spLev[2] << spLev[3];
            st << spForm[0] << spForm[1] << spForm[2];
        }
    };
	//装备额外属性
	struct ItemEquipData
	{
		UInt8	sockets;			//孔数
		UInt8	enchant;			//强化级别
		UInt8	tRank;			    //法宝当前阶
		UInt8	maxTRank;			//法宝最大阶
		UInt32	trumpExp;			//法宝经验
		UInt16	gems[SOCKETS_MAX];			//宝石镶嵌增加附加属性
		ItemEquipAttr2 extraAttr2;
        ItemEquipSpiritAttr spiritAttr;

		ItemEquipData(): sockets(0), enchant(0), tRank(0), maxTRank(0), trumpExp(0) { memset(gems, 0, sizeof(gems)); }
	};
}

#endif

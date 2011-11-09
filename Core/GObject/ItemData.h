#ifndef _ITEMDATA_H_
#define _ITEMDATA_H_

#include "Common/Stream.h"

#define ENCHANT_LEVEL_MAX 12
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
	//װ����������
	struct ItemEquipData
	{
		UInt8	sockets;			//����
		UInt8	enchant;			//ǿ������
		UInt8	tRank;			    //������ǰ��
		UInt8	maxTRank;			//��������
		UInt32	trumpExp;			//��������
		UInt16	gems[SOCKETS_MAX];			//��ʯ��Ƕ���Ӹ�������
		ItemEquipAttr2 extraAttr2;

		ItemEquipData(): sockets(0), enchant(0) { memset(gems, 0, sizeof(gems)); }
	};
}

#endif

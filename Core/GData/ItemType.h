#ifndef _ITEMTYPE_H_
#define _ITEMTYPE_H_

#include "AttrExtra.h" 
//#include "WeaponDef.h"
#include "GObject/GObjectBase.h"

#define LNORMAL_ID      0
#define RNORMAL_ID      499
#define LENHANCE_ID     500
#define RENHANCE_ID     799
#define LTASK_ID        800
#define RTASK_ID        999
#define LFORMULA_ID     1000
#define RFORMULA_ID     1199
#define LCITTA_ID       1200
#define RCITTA_ID       1499
#define LARM_ID         1500
#define RARM_ID         4999
#define LGEM_ID         5000
#define XIAN_ID         18000
#define LXIANGEM_ID     20000
#define LXIANGEM_ID_END     21000
#define RGEM_ID         5999
#define LCITTA1_ID      6000
#define RCITTA1_ID      7000
#define LSOUL_ID        7001
#define RSOUL_ID        7999
#define LSL_ID          8000
#define RSL_ID          8999
#define LNORMAL1_ID     9000
#define LMOUNT_ID       9500
#define RMOUNT_ID       9899
#define RNORMAL1_ID     10999
#define LBMIN_ID        11500
#define LBMAX_ID        11999
#define LPETARM_ID      12000
#define RPETARM_ID      12999
#define LPETGEM_ID      13000
#define RPETGEM_ID      13999
#define LLINGSHI_ID     14000
#define RLINGSHI_ID     15000
#define LZHENYAUN_ID    15001
#define RZHENYUAN_ID    15499
#define LOTHER_ID       15500
#define LNORMAL2_ID     16000
#define RNORMAL2_ID     17999
#define HORCRUX_ID      19000
#define HORCRUX_ID_END  19015
#define ROTHER_ID       65535

#define COUPON_ID       499

enum ItemQuality
{
    Item_Green  = 2,    //绿色
    Item_Blue   = 3,    //蓝色
    Item_Purple = 4,    //紫色
    Item_Yellow = 5,    //橙色
};

enum ItemClass
{
    // [00~19]  装备 [1500~4990]   1武器，2头，3胸，4肩，5腰，6腿，7项链，8戒指，10法宝，11时装
    // [20~29]  普通 [0~499]
    // [30~39]  阵法 [1000~1199]
    // [40~49]  心法 [1200~1499]
    // [50~59]  强化 [500~799]
    // [60~79]  宝石 [5001~5999]  60力量，61敏捷，62智力，63体魄，64生命，65攻击，66防御，67命中，68反击，69闪避，70暴击，71破击，72身法
    // [80~89]  任务 [800~999]
    // [90~100] 魂   [7000~8000]
	Item_None = 0,
	Item_Weapon = 1,	//武器  武器 1
	Item_Armor1,		//防具1 头盔 2
	Item_Armor2,		//防具2 胸甲 3
	Item_Armor3,		//防具3 肩甲 4
	Item_Armor4,		//防具4 腰带 5
	Item_Armor5,		//防具5 腿甲 6

	Item_Normal = 20,	//普通物品 20~29
   };

inline bool IsEquipId(UInt32 id) // XXX: 这个是流水号
{
    return id > 25000; // XXX: 只有武器需要流水号
	// return id >= LARM_ID && id <= RARM_ID;
}

inline bool IsEquipTypeId(UInt32 id)
{
	return (id >= LARM_ID && id <= RARM_ID) || (id >=LBMIN_ID && id <= LBMAX_ID) || (id > XIAN_ID && id < LXIANGEM_ID);
}


inline bool IsEquip(UInt8 subClass)
{
	//return (subClass >= static_cast<UInt8>(Item_Weapon) && subClass <= static_cast<UInt8>(Item_InnateTrump)) || (subClass >= static_cast<UInt8>(Item_Evolution) && subClass <= static_cast<UInt8>(Item_Evolution3)) || (subClass == static_cast<UInt8>(Item_Horcrux) );
    return false;
}

inline bool IsFighterTypeId(UInt32 id)
{
    if (id == 73 || id == 74 || id == 75 || id == 76 || id == 75 || id == 77)
        return true;
    return false;
}

namespace GData
{

	struct ItemBaseType:
		public ObjectBaseT<>
	{
		ItemClass	subClass;
		UInt32		price;
		UInt16		reqLev;
		UInt16		vLev;
		UInt8		quality;
		UInt16		maxQuantity;
		UInt8		bindType;
		UInt16		energy;
		UInt32		trumpExp;
        UInt16      data;
        UInt8       career;
        float       salePriceUp;

		ItemBaseType(UInt32 id = 0, const std::string& name = "") : ObjectBaseT<>(id, name)	{}
		virtual ~ItemBaseType() { }	//RTTI
		bool operator<(const ItemBaseType& other) const { return getId() < other.getId(); }
		bool operator==(const ItemBaseType& other) const { return getId() == other.getId(); }
		inline UInt16 Size(UInt16 num) const
		{
		//	if(subClass == Item_Task)
		//		return 0;
			if(IsEquip(subClass))
				return num;
			return (num + maxQuantity - 1) / maxQuantity;
		}
	};

	//普通物品
	struct ItemNormalType : public ItemBaseType
	{
		ItemNormalType(UInt32 id = 0, const std::string& name = "") : ItemBaseType(id, name) { }
		virtual ~ItemNormalType() { }
	};

	//宝石
	
	//装备
	struct ItemEquipType : public ItemBaseType
	{
		const AttrExtra*	attrExtra;
        bool fix;
		ItemEquipType(UInt32 id = 0, const std::string& name = "", UInt32 attrId = 0) : ItemBaseType(id, name), fix(false)
		{
			const AttrExtraItem * attr = attrExtraManager[attrId];
			if(attr != NULL)
				attrExtra = *attr;
			else
				attrExtra = NULL;
		}
        void setAttr(const AttrExtra* attr, bool fix = false)
        {
            if (this->fix)
                delete attrExtra;
            attrExtra = attr;
            this->fix = fix;
        }
		virtual ~ItemEquipType()
        {
            if (fix && attrExtra)
                delete attrExtra;
        }
	};

	struct ItemWeaponType : public ItemEquipType
	{
		ItemWeaponType(UInt32 id = 0, const std::string& name = "", UInt32 attrId = 0) : ItemEquipType(id, name, attrId) {}
	};

	struct ItemTrumpType : public ItemEquipType
	{
		ItemTrumpType(UInt32 id = 0, const std::string& name = "", UInt32 attrId = 0) : ItemEquipType(id, name, attrId) {}
	};

	struct ItemEquipSetType:
		public ObjectBaseT<>
	{
		const AttrExtra* attrExtra[4];

		ItemEquipSetType(UInt32 id = 0, const std::string& name = "") : ObjectBaseT<>(id, name)	{}
	};
	typedef ObjectListT<ItemBaseType> ItemBaseTypeManager;
	typedef ObjectMapT<ItemBaseType, std::string> ItemBaseTypeNameManager;

	extern ItemBaseTypeManager itemBaseTypeManager;
	extern ItemBaseTypeNameManager itemBaseTypeNameManager;

	typedef ObjectListT<ItemEquipSetType> ItemEquipSetTypeManager;
	extern ItemEquipSetTypeManager itemEquipSetTypeManager;

#define ITEM_BIND_CHECK(bindType, bind)	\
	if(!bind && bindType == 1) bind = true;
}

#endif

#ifndef _ITEMTYPE_H_
#define _ITEMTYPE_H_

#include "AttrExtra.h"
#include "WeaponDef.h"

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
#define RGEM_ID         5200
#define LOTHER_ID       5201
#define ROTHER_ID       65535

enum ItemClass
{
    // [00~19] 装备 [1500~4990]   1武器，2头，3胸，4肩，5腰，6腿，7项链，8戒指，10法宝，11坐骑
    // [20~29] 普通 [0~499]
    // [30~39] 阵法 [1000~1199]
    // [40~49] 心法 [1200~1499]
    // [50~59] 强化 [500~799]
    // [60~79] 宝石 [5001~5200]  60力量，61敏捷，62智力，63体魄，64生命，65攻击，66防御，67命中，68反击，69闪避，70暴击，71破击，72身法
    // [80~89] 任务 [800~999]
	Item_None = 0,
	Item_Weapon = 1,	//武器  武器 1
	Item_Armor1,		//防具1 头盔 2
	Item_Armor2,		//防具2 胸甲 3
	Item_Armor3,		//防具3 肩甲 4
	Item_Armor4,		//防具4 腰带 5
	Item_Armor5,		//防具5 腿甲 6
	Item_Amulet,		//项链 7
	Item_Ring,			//戒指 8
	Item_Reserved,		//保留 9
	Item_Trump,			//法宝 10
	Item_Mounts,		//坐骑 11
	Item_Normal = 20,	//普通物品 20~29
    Item_Normal29 = 29,
	Item_Formula = 30,	//阵法 30~39
	Item_Formula1, Item_Formula2, Item_Formula3,
	Item_Formula4, Item_Formula5, Item_Formula6,
	Item_Formula7, Item_Formula8, Item_Formula9,
	Item_Citta = 40,	//心法 40~49
	Item_Citta1, Item_Citta2, Item_Citta3,
	Item_Citta4, Item_Citta5, Item_Citta6,
	Item_Citta7, Item_Citta8, Item_Citta9,
	Item_Enhance = 50,	//强化 50~59
	Item_Enhance1, Item_Enhance2, Item_Enhance3,
	Item_Enhance4, Item_Enhance5, Item_Enhance6,
	Item_Enhance7, Item_Enhance8, Item_Enhance9,
	Item_Gem = 60,		//宝石 60~79                       力量宝石
    Item_Gem1, Item_Gem2, Item_Gem3, Item_Gem4,         // 敏捷宝石， 智力宝石， 耐力宝石， 意志宝石
    Item_Gem5, Item_Gem6, Item_Gem8, Item_Gem9,         // 生命宝石， 攻击宝石， 防御宝石， 命中宝石
    Item_Gem10, Item_Gem11, Item_Gem12, Item_Gem13,     // 反击宝石， 闪避宝石， 暴击宝石， 破击宝石
    Item_Gem14, Item_Gem15, Item_Gem16, Item_Gem17,     // 身法宝石， 坚韧宝石， 法抗宝石
    Item_Gem18, Item_Gem19,
	Item_Task = 80,		//任务 80~89
	Item_Other = 90,	//其他 90~
};

inline bool IsEquipId(UInt32 id) // XXX: 这个是流水号
{
    return id > 25000; // XXX: 只有武器需要流水号
	// return id >= LARM_ID && id <= RARM_ID;
}

inline bool IsEquipTypeId(UInt32 id)
{
	return id >= LARM_ID && id <= RARM_ID;
}

inline bool IsGemId(UInt32 id)
{
	return id > LGEM_ID && id <= RGEM_ID;
}

inline bool IsEquip(UInt8 subClass)
{
	return subClass >= static_cast<UInt8>(Item_Weapon) && subClass <= static_cast<UInt8>(Item_Trump);
}

inline UInt8 getGemLev(UInt32 id)
{
    UInt8 lev = id%10;
    if(IsGemId)
    {
        if(lev == 0)
            return 10;
        else
            return lev;
    }
    else
        return 0;
}

inline bool IsFighterTypeId(UInt32 id)
{
    if (id == 73 || id == 74 || id == 75 || id == 76 || id == 75 || id == 77)
        return true;
    return false;
}

inline ItemClass GetItemSubClass(UInt32 id)
{
#if 1
    if (id >= LNORMAL_ID && id <= RNORMAL_ID)
        return Item_Normal;
    if (id >= LENHANCE_ID && id <= RENHANCE_ID)
        return Item_Enhance;
    if (id >= LTASK_ID && id <= RTASK_ID)
        return Item_Task;
    if (id >= LFORMULA_ID && id <= RFORMULA_ID)
        return Item_Formula;
    if (id >= LCITTA_ID && id <= RCITTA_ID)
        return Item_Citta;
    //if (id >= LARM_ID && id <= RARM_ID)
    //    return Item_Weapon; // XXX: 
    if (id >= LGEM_ID && id <= RGEM_ID)
		return Item_Gem;
	return Item_Other;
#else
    if(id <= 5000)
        return Item_Other;
    if(id <= 5800)
        return Item_Gem;
    //if(id <= 7000)
    //    return Item_Favor;
    if(id <= 10000)
        return Item_Normal;
    if(id <= 15000)
        return Item_Other;
    if(id <= 25000)
        return Item_Task;
    return Item_Other;
#endif
}

namespace GData
{

	struct ItemBaseType:
		public ObjectBaseT<>
	{
		ItemClass	subClass;
		UInt32		price;
		UInt16		reqLev;
		UInt8		quality;
		UInt16		maxQuantity;
		UInt8		bindType;
		UInt16		energy;
		UInt32		trumpExp;
        UInt16      data;
        UInt8       career;

		ItemBaseType(UInt32 id = 0, const std::string& name = "") : ObjectBaseT<>(id, name)	{}
		virtual ~ItemBaseType() { }	//RTTI
		bool operator<(const ItemBaseType& other) const { return getId() < other.getId(); }
		bool operator==(const ItemBaseType& other) const { return getId() == other.getId(); }
	};

	//普通物品
	struct ItemNormalType : public ItemBaseType
	{
		ItemNormalType(UInt32 id = 0, const std::string& name = "") : ItemBaseType(id, name) { }
		virtual ~ItemNormalType() { }
	};

	//宝石
	struct ItemGemType : public ItemBaseType
	{
		const AttrExtra* attrExtra;	//附加属性
		ItemGemType(UInt32 id = 0, const std::string& name = "", UInt32 attrId = 0) : ItemBaseType(id, name)
		{
			const AttrExtraItem * attr = attrExtraManager[attrId];
			if(attr != NULL)
				attrExtra = *attr;
			else
				attrExtra = NULL;
		}
		virtual ~ItemGemType() { }
	};

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
	extern std::vector<ItemGemType *> gemTypes;
	typedef ObjectListT<ItemEquipSetType> ItemEquipSetTypeManager;
	extern ItemEquipSetTypeManager itemEquipSetTypeManager;

#define ITEM_BIND_CHECK(bindType, bind)	\
	if(!bind && bindType == 1) bind = true;

}

#endif

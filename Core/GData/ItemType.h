#ifndef _ITEMTYPE_H_
#define _ITEMTYPE_H_

#include "AttrExtra.h"
#include "WeaponDef.h"

enum ItemClass
{
	Item_None = 0,
	Item_Normal,		//普通物品
	Item_Gem,			//宝石
	Item_Task,			//任务
	Item_Weapon = 4,	//武器  武器
	Item_Armor1,		//防具1 头盔
	Item_Armor2,		//防具2 胸甲
	Item_Armor3,		//防具3 肩甲
	Item_Armor4,		//防具4 腰带
	Item_Armor5,		//防具5 腿甲
	Item_Amulet,		//项链  项链
	Item_Ring,			//戒指  戒指
	Item_Favor,			//喜好品
    Item_Trump,         //法宝
	Item_Other,			//其他
};
inline bool IsEquipId(UInt32 id)
{
	return id > 25000;
}

inline bool IsEquipTypeId(UInt32 id)
{
	return id >= 1 && id <= 5000;
}

inline bool IsGemId(UInt32 id)
{
	return id > 5000 && id <= 5800;
}

inline bool IsEquip(UInt8 subClass)
{
	return subClass >= static_cast<UInt8>(Item_Weapon) && subClass <= static_cast<UInt8>(Item_Ring);
}

inline ItemClass GetItemSubClass(UInt32 id)
{
	if(id <= 5000)
		return Item_Other;
	if(id <= 5800)
		return Item_Gem;
	if(id <= 7000)
		return Item_Favor;
	if(id <= 10000)
		return Item_Normal;
	if(id <= 15000)
		return Item_Other;
	if(id <= 25000)
		return Item_Task;
	return Item_Other;
}

namespace GData
{

	struct ItemBaseType:
		public ObjectBaseT<>
	{
		ItemClass	subClass;
		UInt32		coin;
		UInt16		reqLev;
		UInt8		quality;
		UInt16		maxQuantity;
		UInt8		bindType;

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
		const Attr1Extra* attr1Extra;	//附加一级属性
		const Attr2Extra* attr2Extra;	//附加二级属性
		ItemGemType(UInt32 id = 0, const std::string& name = "", UInt32 attr1Id = 0, UInt32 attr2Id = 0) : ItemBaseType(id, name)
		{
			const Attr1ExtraItem * item1 = attr1ExtraManager[attr1Id];
			if(item1 != NULL)
				attr1Extra = *item1;
			else
				attr1Extra = NULL;
			const Attr2ExtraItem * item2 = attr2ExtraManager[attr2Id];
			if(item2 != NULL)
				attr2Extra = *item2;
			else
				attr2Extra = NULL;
		}
		virtual ~ItemGemType() { }
	};

	//装备
	struct ItemEquipType : public ItemBaseType
	{
		const Attr1Extra*	attr1Extra;
		const Attr2Extra*	attr2Extra;
		ItemEquipType(UInt32 id = 0, const std::string& name = "", UInt32 attr1Id = 0, UInt32 attr2Id = 0) : ItemBaseType(id, name)
		{
			const Attr1ExtraItem * item1 = attr1ExtraManager[attr1Id];
			if(item1 != NULL)
				attr1Extra = *item1;
			else
				attr1Extra = NULL;
			const Attr2ExtraItem * item2 = attr2ExtraManager[attr2Id];
			if(item2 != NULL)
				attr2Extra = *item2;
			else
				attr2Extra = NULL;
		}
		virtual ~ItemEquipType() { }
	};

	struct ItemWeaponType : public ItemEquipType
	{
		ItemWeaponType(UInt32 id = 0, const std::string& name = "", UInt32 attr1Id = 0, UInt32 attr2Id = 0) : ItemEquipType(id, name, attr1Id, attr2Id) {}
	};

	struct ItemEquipSetType:
		public ObjectBaseT<>
	{
		const Attr1Extra* attr1Extra[4];
		const Attr2Extra* attr2Extra[4];

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
	if(!bind && bindType == 2) bind = true;

}

#endif

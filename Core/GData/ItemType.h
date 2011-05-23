#ifndef _ITEMTYPE_H_
#define _ITEMTYPE_H_

#include "AttrExtra.h"
#include "WeaponDef.h"

enum ItemClass
{
	Item_None = 0,
	Item_Normal,		//��ͨ��Ʒ
	Item_Gem,			//��ʯ
	Item_Task,			//����
	Item_Weapon = 4,	//����  ����
	Item_Armor1,		//����1 ͷ��
	Item_Armor2,		//����2 �ؼ�
	Item_Armor3,		//����3 ���
	Item_Armor4,		//����4 ����
	Item_Armor5,		//����5 �ȼ�
	Item_Amulet,		//����  ����
	Item_Ring,			//��ָ  ��ָ
	Item_Favor,			//ϲ��Ʒ
    Item_Trump,         //����
	Item_Other,			//����
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

	//��ͨ��Ʒ
	struct ItemNormalType : public ItemBaseType
	{
		ItemNormalType(UInt32 id = 0, const std::string& name = "") : ItemBaseType(id, name) { }
		virtual ~ItemNormalType() { }
	};

	//��ʯ
	struct ItemGemType : public ItemBaseType
	{
		const Attr1Extra* attr1Extra;	//����һ������
		const Attr2Extra* attr2Extra;	//���Ӷ�������
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

	//װ��
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

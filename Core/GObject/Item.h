#ifndef _ITEM_H_
#define _ITEM_H_

#include "GData/ItemType.h"
#include "ItemData.h"
#include "GObjectManager.h"

namespace GObject
{
	class ItemBase :
		public GObjectBaseT<ItemBase>
	{
	public:
		ItemBase(UInt32 id, const GData::ItemBaseType& itemType)
			: GObjectBaseT<ItemBase>(id), _itemBaseType(itemType), m_BindStatus(false), m_TotalCount(0)
		{
		}
		virtual ~ItemBase() {};
	
	public:
		const GData::ItemBaseType& GetItemType() const { return _itemBaseType; }

		inline ItemClass	getClass() const { return _itemBaseType.subClass; }
		inline const std::string& getName() const   { return _itemBaseType.getName();	 }
		inline UInt8		getQuality() const { return _itemBaseType.quality; }
		inline UInt16		getReqLev() const { return _itemBaseType.reqLev; }
		inline UInt32		getPrice() const { return _itemBaseType.coin; }

		inline UInt16		getMaxQuantity() const	{ return _itemBaseType.maxQuantity; }
		inline UInt8		getBindType() const	{ return _itemBaseType.bindType; }

		bool GetBindStatus() const { return m_BindStatus; }
		inline bool  SetBindStatus(bool bindType)
		{
			if (bindType == m_BindStatus)
				return false;
			m_BindStatus = bindType;
			return true;
		}

	public:
		inline bool SetItem(UInt16 num) { m_TotalCount = num; return true; } //FIXME: Write to DB !!!
		inline bool IncItem(UInt16 num) { m_TotalCount += num; return true; }
		inline bool DecItem(UInt16 num) { if(num > m_TotalCount) return false; m_TotalCount -= num; return true; }
		inline UInt16 Size() const
		{
			if(_itemBaseType.subClass == Item_Task)
				return 0;
			if(IsEquip(_itemBaseType.subClass))
				return 1;
			return (m_TotalCount + _itemBaseType.maxQuantity - 1) / _itemBaseType.maxQuantity;
		}
		inline UInt16 Size(UInt16 num) const
		{
			if(_itemBaseType.subClass == Item_Task)
				return 0;
			if(IsEquip(_itemBaseType.subClass))
				return 1;
			return (num + _itemBaseType.maxQuantity - 1) / _itemBaseType.maxQuantity;
		}
		inline UInt16 Count() const		{ return IsEquip(_itemBaseType.subClass) ? 1 : m_TotalCount; }
		inline UInt16 GetLeftNum() const { return m_TotalCount % _itemBaseType.maxQuantity; }
		inline UInt16 GetOverlayNum() const { return _itemBaseType.maxQuantity; }

	protected:
		const GData::ItemBaseType& _itemBaseType;

		bool	m_BindStatus;
		UInt16	m_TotalCount;
	};

	//×°±¸
	class ItemEquip : public ItemBase
	{
	public:
		ItemEquip(UInt32 id, const GData::ItemEquipType& itemEquipType, ItemEquipData& itemEquipData) 
			: ItemBase(id, itemEquipType), _itemEquipData(itemEquipData)
		{
		}

		virtual ~ItemEquip() {}

	public:
		inline ItemEquipData& getItemEquipData() { return _itemEquipData; }

		inline const GData::Attr1Extra * getAttr1Extra() { return static_cast<const GData::ItemEquipType&>(_itemBaseType).attr1Extra; }
		inline ItemEquipAttr2& getEquipAttr2() { return _itemEquipData.extraAttr2; }

		void DoEquipBind(bool = false);

	public:
		ItemEquipData _itemEquipData;
	};

	class ItemWeapon : public ItemEquip
	{
	public:
		ItemWeapon(UInt32 id, const GData::ItemWeaponType& itemArmorType, ItemEquipData& itemEquipData)
			: ItemEquip(id, itemArmorType, itemEquipData) 
		{ }
		inline UInt16 getAttack() { return static_cast<const GData::ItemWeaponType&>(_itemBaseType).value; } 
		inline UInt16 getAttack1() { return static_cast<const GData::ItemWeaponType&>(_itemBaseType).value1; } 
	};

	class ItemArmor : public ItemEquip
	{
	public:
		ItemArmor(UInt32 id, const GData::ItemEquipType& itemEquipType, ItemEquipData& itemEquipData)
		    : ItemEquip(id, itemEquipType, itemEquipData) 
		{ }
		inline UInt16 getDefend() { return static_cast<const GData::ItemEquipType&>(_itemBaseType).value; } 
		inline UInt16 getDefend1() { return static_cast<const GData::ItemEquipType&>(_itemBaseType).value1; } 
	};
}

#endif

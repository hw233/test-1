#ifndef _ITEM_H_
#define _ITEM_H_

#include "GData/ItemType.h"
#include "ItemData.h"
#include "GObjectManager.h"
//#include "GData/SkillTable.h"

struct SkillBase;

namespace GObject
{
    class ItemBase :
        public GObjectBaseT<ItemBase>
    {
        public:
            ItemBase(UInt32 id, const GData::ItemBaseType* itemType)
                : GObjectBaseT<ItemBase>(id), _itemBaseType(itemType), m_BindStatus(false), m_TotalCount(0), m_SellTime(0)
            {
            }
            virtual ~ItemBase() {};

        public:
            const GData::ItemBaseType& GetItemType() const { return *_itemBaseType; }

            inline ItemClass	getClass() const { return _itemBaseType->subClass; }
            inline UInt16 GetTypeId() const { return _itemBaseType->getId(); }
            inline const std::string& getName() const   { return _itemBaseType->getName();	 }
            inline UInt8		getQuality() const { return _itemBaseType->quality; }
            inline UInt16		getReqLev() const { return _itemBaseType->reqLev; }
            inline UInt32		getPrice() const { return _itemBaseType->price; }
            inline UInt16		getValueLev() const { return _itemBaseType->vLev; }

            inline UInt16		getMaxQuantity() const	{ return _itemBaseType->maxQuantity; }
            inline UInt8		getBindType() const	{ return _itemBaseType->bindType; }
            inline UInt16       getEnergy() const { return _itemBaseType->energy; }
            inline UInt8        GetCareer() const { return _itemBaseType->career; }

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
            inline UInt32 SetSellTime(UInt32 sellTime)  { m_SellTime = sellTime; return true; }
            inline UInt16 Size() const
            {
                if(IsEquip(_itemBaseType->subClass))
                    return 1;
                return (m_TotalCount + _itemBaseType->maxQuantity - 1) / _itemBaseType->maxQuantity;
            }
            inline UInt16 Size(UInt16 num) const
            {
                if(IsEquip(_itemBaseType->subClass))
                    return 1;
                return (num + _itemBaseType->maxQuantity - 1) / _itemBaseType->maxQuantity;
            }
            inline UInt16 Count() const
            {
                bool result = IsEquip(_itemBaseType->subClass);
                return result ? 1 : m_TotalCount;
            }
            inline UInt16 GetLeftNum() const { return m_TotalCount % _itemBaseType->maxQuantity; }
            inline UInt16 GetOverlayNum() const { return _itemBaseType->maxQuantity; }
            inline UInt32 GetSellTime() const { return m_SellTime; }
        protected:
            const GData::ItemBaseType* _itemBaseType;

            bool	m_BindStatus;
            UInt16	m_TotalCount;
            UInt32  m_SellTime;  //出售物品时间
    };

    //装备
    class ItemEquip : public ItemBase
    {
        public:
            ItemEquip(UInt32 id, const GData::ItemBaseType* itemEquipType, ItemEquipData& itemEquipData)
                : ItemBase(id, itemEquipType), _itemEquipData(itemEquipData)
            {
            }

            virtual ~ItemEquip() {}

        public:
            inline ItemEquipData& getItemEquipData() { return _itemEquipData; }

            inline const GData::AttrExtra * getAttrExtra() { return static_cast<const GData::ItemEquipType*>(_itemBaseType)->attrExtra; }

            inline UInt16 getAttack()
            {
                const GData::AttrExtra* attrExtra = getAttrExtra();
                if (attrExtra)
                    return attrExtra->attack;
                return 0;
            }
            inline UInt16 getImgAttack()
            {
                const GData::AttrExtra* attrExtra = getAttrExtra();
                if (attrExtra)
                    return attrExtra->magic_attack;
                return 0;
            }
            inline UInt16 getDefend()
            {
                const GData::AttrExtra* attrExtra = getAttrExtra();
                if (attrExtra)
                    return attrExtra->defend;
                return 0;
            }
            inline UInt16 getImgDefend()
            {
                const GData::AttrExtra* attrExtra = getAttrExtra();
                if (attrExtra)
                    return attrExtra->magic_defend;
                return 0;
            }

            void DoEquipBind(bool = false);

        public:
            ItemEquipData _itemEquipData;
    };

    class ItemWeapon : public ItemEquip  
    {
        public:
            ItemWeapon(UInt32 id, const GData::ItemBaseType* itemArmorType, ItemEquipData& itemEquipData)
                : ItemEquip(id, itemArmorType, itemEquipData)
            { }
    };

    class ItemArmor : public ItemEquip  
    {
        public:
            ItemArmor(UInt32 id, const GData::ItemBaseType* itemArmorType, ItemEquipData& itemEquipData)
                : ItemEquip(id, itemArmorType, itemEquipData)
            { }
    };
}

#endif

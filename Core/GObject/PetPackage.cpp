#include "Config.h"
#include "GData/Store.h"
#include "Server/OidGenerator.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "MsgID.h"
#include "Player.h"
#include "Fighter.h"
#include "Package.h"
#include "PetPackage.h"

namespace GObject
{

    extern URandom GRND;

	PetPackage::PetPackage(Player* player) : Package(player),
        m_ItemSize(0), m_EquipSize(0)
	{
	}

	PetPackage::~PetPackage()
	{
	}

	void PetPackage::UnInit()
	{
		item_elem_iter iter = m_PetItems.begin();
		for (; iter != m_PetItems.end(); ++iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_PetItems.clear();
		iter = m_PetEquips.begin();
		for (; iter != m_PetEquips.end(); ++iter)
		{
			SAFE_DELETE(iter->second);
		}
		m_PetEquips.clear();
	}

	ItemBase* PetPackage::AddItemFromDB(UInt32 id, UInt32 num, bool bind)
	{
		assert(!IsPetItem(id));
		const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[id];
		if(itemType == NULL) return NULL;
		ItemBase * item = new(std::nothrow) ItemBase(id, itemType);
		if (item == NULL) return NULL;
		ITEM_BIND_CHECK(itemType->bindType, bind);
		item->SetBindStatus(bind);
        UInt16 oldq = item->Size(), newq = item->Size(item->Count() + num);
        m_ItemSize = m_ItemSize + newq - oldq;
        item->IncItem(num);
        m_PetItems[ItemKey(id, bind)] = item;
		return item;
	}

	ItemBase * PetPackage::AddExistEquip(ItemPetEq * equip, bool fromDB)
	{
		ItemBase *& e = m_PetEquips[ItemKey(equip->getId())];
		if(e == NULL)
			++ m_EquipSize;
		e = equip;
		//SendSingleEquipData(equip);
		return equip;
	}

	ItemPetEq * PetPackage::FindPetEquip( FairyPet *& pet, UInt8& pos, UInt16 petId, UInt32 id )
	{
		if(!IsPetEquipTypeId(id))
			 return NULL;
		if(petId == 0)
			return FindPetEquip(id);
		pet = m_Owner->findFairyPet(petId);
		if(pet == NULL)
			return NULL;
		return static_cast<ItemPetEq *>(pet->findEquip(id, pos));
	}

	ItemBase* PetPackage::AddPetEquipN( UInt32 typeId, UInt32 num, bool bind, bool silence, UInt8 FromWhere )
	{
		if(GetPetEqPgRestSize() < num)
			return NULL;
		ItemBase * item = NULL;
		for(UInt32 i = 0; i < num; ++ i)
		{
			item = AddPetEquip(typeId, bind, !silence, FromWhere);
		}
		return item;
	}

	ItemBase* PetPackage::AddPetEquip(UInt32 typeId, bool bind, bool notify, UInt8 FromWhere)
	{
		if (!IsPetEquipTypeId(typeId)) return NULL;
		if(GetPetEqPgRestSize() < 1)
			return NULL;
		//Add New Equip
		const GData::ItemBaseType * itype = GData::itemBaseTypeManager[typeId];
		if(itype == NULL) return NULL;
		switch(itype->subClass)
		{
		case Item_PetEquip1:
		case Item_PetEquip2:
		case Item_PetEquip3:
			{
				ItemEquip * equip;
				ItemEquipData edata;

			    edata.extraAttr2.type1 = 0; edata.extraAttr2.value1 = 0;
			    edata.extraAttr2.type2 = 0; edata.extraAttr2.value2 = 0;
			    edata.extraAttr2.type3 = 0; edata.extraAttr2.value3 = 0;

				UInt32 id = IDGenerator::gItemOidGenerator.ID();
			    equip = new ItemEquip(id, itype, edata);
				if(equip == NULL)
					return NULL;
				ITEM_BIND_CHECK(itype->bindType,bind);

                /*
                if (itype->subClass == Item_Trump || itype->subClass == Item_Halo || itype->subClass == Item_Fashion)
                    m_Owner->OnShuoShuo(SS_TRUMP);
                */

				ItemBase *& e = m_PetEquips[id];
				if(e == NULL)
					++ m_EquipSize;
				e = equip;
				DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, 1, %"I64_FMT"u, %u)", id, m_Owner->getId(), bind ? 1 : 0);
				DB4().PushUpdateData("INSERT INTO `equipment`(`id`, `itemId`, `maxTRank`, `trumpExp`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`) VALUES(%u, %u, %u, %u, %u, %d, %u, %d, %u, %d)", id, typeId, edata.maxTRank, edata.trumpExp, edata.extraAttr2.type1, edata.extraAttr2.value1, edata.extraAttr2.type2, edata.extraAttr2.value2, edata.extraAttr2.type3, edata.extraAttr2.value3);

				//SendSingleEquipData(equip);
				//equip->SetBindStatus(bind);
				if(notify)
					ItemNotifyEquip(equip);
				if((FromWhere != 0 && itype->quality >= 4))
					DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), typeId, id, FromWhere, TimeUtil::Now());

                //OnAddEquipAndCheckAttainment(itype, FromWhere);
				return equip;
			}
		default:
			break;
		}
		return NULL;
	}

	bool PetPackage::TryAddPetItem( ItemBase * item, UInt16 num )
	{
		UInt16 cur = m_ItemSize;
		UInt16 oldq = item->Size(), newq = item->Size(item->Count() + num);
		cur = cur - oldq + newq;
		if(cur > INIT_PETGEM_PACK_SIZE)
			return false;
		if(!item->IncItem(num))
			return false;
		m_ItemSize = cur;
		return true;
	}

	bool PetPackage::TryDelPetItem( ItemBase * item, UInt16 num )
	{
		if(item->Count() < num)
			return false;
		UInt16 cur = m_ItemSize;
		UInt16 oldq = item->Size(), newq = item->Size(item->Count() - num);
		cur = cur - oldq + newq;
		if(!item->DecItem(num))
			return false;
		m_ItemSize = cur;

		return true;
	}

	bool PetPackage::DelPetItem(UInt32 id, UInt16 num, bool bind, UInt8 toWhere)
	{
		if (num == 0 || !IsPetItem(id))
			return false;
        if (!toWhere)
            toWhere = ToDelete;
		ItemBase * item = FindPetItem(id, bind);
		if (item == NULL) return false;
		bool ret = TryDelPetItem(item, num);
		if (ret)
		{
            std::string tbn("item_courses");
            DBLOG().GetMultiDBName(tbn);
            DBLOG().PushUpdateData("insert into  `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)",tbn.c_str(), cfg.serverLogId, m_Owner->getId(), item->GetItemType().getId(), num, toWhere, TimeUtil::Now());

            UInt32 price = GData::store.getPrice(id);
            if (price || GData::GDataManager::isInUdpItem(id))
            {
                udpLog(item->getClass(), id, num, price, "sub");
            }

            //SendItemData(item);
			UInt16 cnt = item->Count();
			if (cnt == 0)
			{
				SAFE_DELETE(item);
				m_PetItems.erase(ItemKey(id, bind));
				DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", id, bind, m_Owner->getId());
			}
			else
				DB4().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND`ownerId` = %"I64_FMT"u", cnt, id, bind, m_Owner->getId());
		}
		return ret;
	}

	ItemBase* PetPackage::AddPetItem(UInt32 typeId, UInt32 num, bool bind, bool notify, UInt8 FromWhere)
	{
        if (!typeId || !num) return NULL;
		if (!IsPetItem(typeId)) return NULL;
		const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[typeId];
		if(itemType == NULL) return NULL;
		ITEM_BIND_CHECK(itemType->bindType, bind);
		ItemBase * item = FindPetItem(typeId, bind);

		if (item != NULL)
		{
			bool ret = TryAddPetItem(item, num);
			if (ret == false)
			    return NULL;
            DB4().PushUpdateData("UPDATE `item` SET `itemNum` = %u WHERE `id` = %u AND `bindType` = %u AND `ownerId` = %"I64_FMT"u", item->Count(), typeId, bind, m_Owner->getId());
		}
		else
		{
			//Add New Item
			item = new(std::nothrow) ItemBase(typeId, itemType);
			if (item == NULL) return NULL;
			bool ret = TryAddPetItem(item, num);
			if (ret == false)
			{
                SAFE_DELETE(item);
                return NULL;
            }
            m_PetItems[ItemKey(typeId, bind)] = item;
            DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, %u, %"I64_FMT"u, %u)", typeId, num, m_Owner->getId(), bind ? 1 : 0);
			item->SetBindStatus(bind);
		}
        if(notify)
            ItemNotify(typeId, num);
        AddItemCoursesLog(typeId, num, FromWhere);
        if (FromWhere != FromNpcBuy && (GData::store.getPrice(typeId) || GData::GDataManager::isInUdpItem(typeId)))
            udpLog(item->getClass(), typeId, num, 0, "add");
        return item;
	}

	UInt8 PetPackage::MergePetGem(UInt32 gemId1, UInt32 gemId2, UInt32& ogid)
	{   //仙宠宝石合成
		if (GetItemSubClass(gemId1) != Item_PetGem || GetItemSubClass(gemId2) != Item_PetGem)
            return 1;
		if(GetPetGemPgRestSize() < 1)
		{
			m_Owner->sendMsgCode(0, 1011);
			return 1;
		}

		ItemBase * item1 = FindPetItem(gemId1, true);
		ItemBase * item2 = FindPetItem(gemId2, true);
        if(!item1 || !item2)
            return 1;
        if(item1->getReqLev() != item2->getReqLev())
            return 1;
        if(item1->getQuality() != item2->getQuality())
            return 1;
        if(item1->getReqLev() >= 20 && item1->getQuality() >= 5)
            return 1;
        if(gemId1 == gemId2)
            ogid = gemId1 + 1;
        else
        {
            UInt32 rnd = uRand(10000);
            if(rnd < 4000)
                ogid = gemId1 + 1;
            else if(rnd < 8000)
                ogid = gemId2 + 1;
            else
                ogid = GameAction()->getpetGemIdByMerge(item1->getReqLev() + 1);
        }
        if(GetItemSubClass(ogid) != Item_PetGem)
            return 1;
	    DelPetItem(gemId1, 1, true);
	    DelPetItem(gemId2, 1, true);
	    AddPetItem(ogid, 1, true, true, FromMerge);
		return 0;
	}

	UInt8 PetPackage::AttachPetGem( UInt32 petId, UInt32 itemId, UInt32 gemId )
	{   //仙宠宝石镶嵌
		if (GetItemSubClass(gemId) != Item_PetGem) return 1;
		FairyPet * pet = NULL;
		UInt8 pos = 0;
		ItemPetEq * equip = FindPetEquip(pet, pos, petId, itemId);
		if(equip == NULL)
			return 1;
        GData::ItemGemType * igt = GData::petGemTypes[gemId - LPETGEM_ID];
        if(!igt)
            return 1;

        ItemPetEqAttr& peAttr = equip->getPetEqAttr();
		UInt8 pempty = 0xFF;
		for(int i = 0; i < (int)(equip->getQuality()) - 2 && i < (int)(sizeof(peAttr.gems)/sizeof(peAttr.gems[0])); ++ i)
		{
			if(peAttr.gems[i] == 0)
			{
				if(pempty == 0xFF)
					pempty = i;
			}
			else
            {
                const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[peAttr.gems[i]];
                if(!itemType || itemType->subClass == equip->getClass())
				    return 1;
            }
		}
		if(pempty == 0xFF)
            return 1;
		if(!DelPetItem(gemId, 1, true, ToGemAttach))
			return 1;
		peAttr.gems[pempty] = gemId;
		DB4().PushUpdateData("UPDATE `petEquipAttr` SET `socket%u` = %u WHERE `id` = %u", pempty + 1, peAttr.gems[pempty], equip->getId());

        /*
	    equip->DoEquipBind();
		if(pet != NULL)
		{
			pet->setDirty();
			pet->sendModification(0x20 + pos, equip, false);
		}
		else
			//SendSingleEquipData(equip);
        */
		return 0;
	}

}


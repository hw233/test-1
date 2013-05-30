#include "Config.h"
#include "Server/OidGenerator.h"
#include "Server/SysMsg.h"
#include "Script/GameActionLua.h"
#include "GData/FairyPetTable.h"
#include "GData/SkillTable.h"
#include "GData/GDataManager.h"
#include "GData/Store.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "Country.h"
#include "MsgID.h"
#include "Player.h"
#include "Fighter.h"
#include "Package.h"
#include "PetPackage.h"

namespace GObject
{

    static int getRandomPetEqGemColorIdx(UInt32 score, UInt8 type)
    {   //type 0:内丹 1:精魄
        if(score < 400)
            return -1;
        int cidx = 0;
        UInt32 chance = uRand(10000);
        if(score <= 600)
        {
            if(chance < (15000 - 25 * score))
                cidx = type ? 0 : 0;
            else
                cidx = type ? 2 : 1;
        }
        else if(score < 1301)
        {
            if(chance < (18200 - 14 * score))
                cidx = type ? 2 : 1;
            else
                cidx = type ? 4 : 2;
        }
        else if(score < 2001)
        {
            if(chance < (20000 - 10 * score))
                cidx = type ? 4 : 2;
            else
                cidx = type ? 6 : 3;
        }
        else
        {
            return -1;
        }

        return cidx;
    }

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
		assert(IsPetItem(id));
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
        if(equip == NULL)
            return NULL;
		ItemBase *& e = m_PetEquips[ItemKey(equip->getId())];
		if(e == NULL)
			++ m_EquipSize;
		e = equip;
        if(!fromDB)
		    SendSingleEquipData(equip, 1);
		return equip;
	}

	ItemPetEq * PetPackage::FindPetEquip( FairyPet *& pet, UInt16 petId, UInt8& pos, UInt32 id )
	{
		if(petId == 0)
			return FindPetEquip(id);
		pet = m_Owner->findFairyPet(petId);
		if(pet == NULL)
			return NULL;
        pet->checkTimeOver();
		return static_cast<ItemPetEq *>(pet->findEquip(id, pos));
	}

	ItemBase* PetPackage::AddPetEquipN( UInt32 typeId, UInt32 num, bool bind, bool silence, UInt8 FromWhere )
	{
		if(GetPetEqPgRestSize() < num)
			return NULL;
		ItemBase * item = NULL;
		for(UInt32 i = 0; i < num; ++ i)
		{
			item = AddRandomPetEq(0, typeId, -1, FromWhere);
		}
		return item;
	}

	ItemBase* PetPackage::AddPetEquip(ItemPetEq * equip, bool notify, UInt8 FromWhere)
	{
		if(equip == NULL || GetPetEqPgRestSize() < 1)
			return NULL;
        ItemBase *& e = m_PetEquips[equip->getId()];
        if(e == NULL)
            ++ m_EquipSize;
        e = equip;
		ItemEquipData& edata = equip->getItemEquipData();
        ItemPetEqAttr& peqAttr = equip->getPetEqAttr();
        DB4().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(%u, 1, %"I64_FMT"u, %u)", equip->getId(), m_Owner->getId(), equip->GetBindStatus() ? 1 : 0);
        DB4().PushUpdateData("INSERT INTO `equipment`(`id`, `itemId`, `maxTRank`, `trumpExp`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`) VALUES(%u, %u, %u, %u, %u, %d, %u, %d, %u, %d)", equip->getId(), equip->GetTypeId(), edata.maxTRank, edata.trumpExp, edata.extraAttr2.type1, edata.extraAttr2.value1, edata.extraAttr2.type2, edata.extraAttr2.value2, edata.extraAttr2.type3, edata.extraAttr2.value3);
        DB4().PushUpdateData("INSERT INTO `petEquipattr`(`id`, `level`, `exp`, `skillId`, `socket1`, `socket2`, `socket3`, `socket4`) VALUES(%u, %u, %u, %u, %u, %u, %u, %u)", equip->getId(), peqAttr.lv, peqAttr.exp, peqAttr.skill, peqAttr.gems[0], peqAttr.gems[1], peqAttr.gems[2], peqAttr.gems[3]);
        if(FromWhere != 0 && equip->getQuality() >= Item_Purple)
            DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), equip->GetTypeId(), equip->getId(), FromWhere, TimeUtil::Now());

        SendSingleEquipData(static_cast<ItemPetEq *>(equip), 1);
        if(notify)
            ItemNotifyEquip(equip);

        if(equip->getQuality() >= Item_Yellow && FromWhere == FromBBFT)
            SYSMSG_BROADCASTV(4156, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->getQuality(), equip->getName().c_str(), 1);
        if(FromWhere == FromBBFT)
            m_Owner->fairyPetUdpLog(10000, equip->getQuality() + 20);
        return equip;
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

            SendItemData(item);
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

    bool PetPackage::DelPetEquip(UInt32 id, UInt8 toWhere)
    {
        item_elem_iter iter = m_PetEquips.find(ItemKey(id));
        if(iter == m_PetEquips.end())
            return false;
        ItemPetEq * equip = static_cast<ItemPetEq *>(iter->second);
        if(equip == NULL)
            return false;
        m_PetEquips.erase(iter);
        -- m_EquipSize;
        DB4().PushUpdateData("DELETE FROM `item` WHERE `id` = %u", id);
        DB4().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", id);
        DB4().PushUpdateData("DELETE FROM `petEquipattr` WHERE `id` = %u", id);
        if(toWhere != 0 && equip->getQuality() >= Item_Purple)
        {
            DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, m_Owner->getId(), equip->GetItemType().getId(), equip->getId(), toWhere, TimeUtil::Now());
        }
        SendSingleEquipData(equip, 0);
		SAFE_DELETE(equip);
        return true;
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
        SendItemData(item);
        if(item->getQuality() >= Item_Yellow && FromWhere == FromPetGemMgerge)
            SYSMSG_BROADCASTV(4155, m_Owner->getCountry(), m_Owner->getName().c_str(), item->getQuality(), item->getName().c_str(), num);
        AddItemCoursesLog(typeId, num, FromWhere);
        if (FromWhere != FromNpcBuy && (GData::store.getPrice(typeId) || GData::GDataManager::isInUdpItem(typeId)))
            udpLog(item->getClass(), typeId, num, 0, "add");
        if(FromWhere == FromBBFT)
        {
            if(item->getReqLev() == 1)
                m_Owner->fairyPetUdpLog(10000, 26);
            else if(item->getReqLev() == 3)
                m_Owner->fairyPetUdpLog(10000, 27);
            else if(item->getReqLev() == 5)
                m_Owner->fairyPetUdpLog(10000, 28);
            else if(item->getReqLev() == 7)
                m_Owner->fairyPetUdpLog(10000, 29);
        }
        return item;
	}

	UInt8 PetPackage::MergePetGem(UInt16 gemId1, UInt16 gemId2, UInt16& ogid)
	{   //仙宠宝石合成
		if (GetItemSubClass(gemId1) != Item_PetGem || GetItemSubClass(gemId2) != Item_PetGem)
            return 0;
		if(GetPetGemPgRestSize() < 1)
		{
			m_Owner->sendMsgCode(0, 1094);
			return 0;
		}

		ItemBase * item1 = FindPetItem(gemId1, true);
		ItemBase * item2 = FindPetItem(gemId2, true);
        if(!item1 || !item2)
            return 0;
        if(gemId1 == gemId2 && item1->Count() < 2)
            return 0;
        if(item1->getReqLev() != item2->getReqLev())
            return 0;
        if(item1->getQuality() != item2->getQuality())
            return 0;
        if(item1->getReqLev() >= 20 && item1->getQuality() >= 5)
            return 0;
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
                ogid = GData::GDataManager::GetPetGemTypeIdByLev(item1->getReqLev());
        }
        if(GetItemSubClass(ogid) != Item_PetGem)
            return 0;
        if(gemId1 == gemId2)
            DelPetItem(gemId1, 2, true, ToPetGemMgerge);
        else
        {
            DelPetItem(gemId1, 1, true, ToPetGemMgerge);
            DelPetItem(gemId2, 1, true, ToPetGemMgerge);
        }
	    AddPetItem(ogid, 1, true, true, FromPetGemMgerge);
		return 1;
	}

	UInt8 PetPackage::AttachPetGem(UInt32 petId, UInt32 equipId, UInt16 gemId)
	{   //仙宠宝石镶嵌
		if (GetItemSubClass(gemId) != Item_PetGem) return 0;
		FairyPet * pet = NULL;
        UInt8 pos = 0;
		ItemPetEq * equip = FindPetEquip(pet, petId, pos, equipId);
		if(equip == NULL)
			return 0;
        GData::ItemGemType * igt = GData::petGemTypes[gemId - LPETGEM_ID];
        if(!igt)
            return 0;

        ItemPetEqAttr& peAttr = equip->getPetEqAttr();
		UInt8 pempty = 0xFF;
		for(int i = 0; i <= (int)(equip->getQuality()) - 2 && i < (int)(sizeof(peAttr.gems)/sizeof(peAttr.gems[0])); ++ i)
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
				    return 0;
            }
		}
		if(pempty == 0xFF)
            return 0;
		if(!DelPetItem(gemId, 1, true, ToPetGemAttach))
			return 0;
		peAttr.gems[pempty] = gemId;
	    equip->DoEquipBind(true);
		DB4().PushUpdateData("UPDATE `petEquipattr` SET `socket%u` = %u WHERE `id` = %u", pempty + 1, peAttr.gems[pempty], equip->getId());

		if(pet != NULL)
		{
			pet->setDirty();
			pet->sendModification(equip, pos);
		}
		else
			SendSingleEquipData(equip, 2);
		return 1;
	}

	UInt8 PetPackage::DetachPetGem(UInt32 petId, UInt32 equipId, UInt8 gemPos)
	{   //仙宠宝石拆卸
        if(gemPos >= 4)
            return 0;
		if(GetPetGemPgRestSize() < 1)
		{
			m_Owner->sendMsgCode(0, 1094);
			return 0;
		}
		FairyPet * pet = NULL;
        UInt8 pos= 0;
		ItemPetEq * equip = FindPetEquip(pet, petId, pos, equipId);
		if(equip == NULL)
			return 0;
        ItemPetEqAttr& peAttr = equip->getPetEqAttr();
        UInt16 oldGem = peAttr.gems[gemPos];
        if(oldGem == 0)
            return 0;
		peAttr.gems[gemPos] = 0;
        AddPetItem(oldGem, 1, true, true, FromPetDetachGem);
	    equip->DoEquipBind(true);
		DB4().PushUpdateData("UPDATE `petEquipattr` SET `socket%u` = %u WHERE `id` = %u", gemPos + 1, 0, equip->getId());

		if(pet != NULL)
		{
			pet->setDirty();
			pet->sendModification(equip, pos);
		}
		else
			SendSingleEquipData(equip, 2);
		return 1;
	}

	bool PetPackage::EquipTo(UInt32 id, FairyPet * pet, UInt8& pos)
	{   //仙宠穿卸装备
        if(pet == NULL || pos >= 3)
            return false;
		if(id == 0 && GetPetEqPgRestSize() < 1)
		{
			m_Owner->sendMsgCode(0, 1094);
			return false;
		}
		ItemPetEq * old = NULL;
		ItemPetEq * equip = NULL;
		if(id == 0)
		{   //卸下
            old = pet->setEquip(NULL, pos);
		    if(old == NULL)
                return false;
        }
        else
        {   //穿上
			item_elem_iter iter = m_PetEquips.find(ItemKey(id));
			if(iter == m_PetEquips.end())
				return false;
			equip = static_cast<ItemPetEq *>(iter->second);
            if(equip == NULL)
                return false;
            old = pet->setEquip(equip, pos);
            if(pos == 0xFF)
                return false;
            SendSingleEquipData(equip, 0);
            m_PetEquips.erase(iter);
            -- m_EquipSize;
        }
		if(old != NULL)
			AddExistEquip(old);
		return true;
    }

	UInt8 PetPackage::equipUpgrade(UInt32 petId, UInt32 equipId, std::string& idStr)
	{   //仙宠装备升级
        FairyPet * pet = NULL;
        UInt8 pos = 0;
		ItemPetEq * equip = FindPetEquip(pet, petId, pos, equipId);
        ItemPetEq * eatEq = NULL;
		if(equip == NULL)
			return 0;
        StringTokenizer tk(idStr, ",");
        ItemPetEqAttr & peAttr = equip->getPetEqAttr();
        if(peAttr.lv >= GData::pet.getEquipMaxLev())
            return 0;
        for(UInt8 i = 0; i < tk.count(); ++i)
        {
            UInt32 id = atoi(tk[i].c_str());
            eatEq = FindPetEquip(id);
            if(eatEq == NULL)
                continue;
            bool hasGem = false;
            for(UInt8 i = 0; i < sizeof(peAttr.gems)/sizeof(peAttr.gems[0]); ++ i)
            {
		        if(GetItemSubClass(eatEq->getPetEqAttr().gems[i]) == Item_PetGem)
                {
                    hasGem = true;
                    break;
                }
            }
            if(hasGem) continue;
            UInt32 res = equipUpgrade(equip, eatEq);
            if(res > 0)
            {
                DelPetEquip(id, ToPetEquipUpgrade);
                if(res == 1)
                    break;
            }
        }
        UInt8 skillLev = GData::pet.getEquipSkillLev(peAttr.lv);
        if(skillLev && SKILLANDLEVEL(SKILL_ID(peAttr.skill), skillLev) != peAttr.skill)
        {
            peAttr.skill = SKILLANDLEVEL(SKILL_ID(peAttr.skill), skillLev);
            if (pet)
            {
                pet->upgradeEquipSkill(equip);
            }
        }
		DB4().PushUpdateData("UPDATE `petEquipattr` SET `exp` = %u, `level` = %u, `skillId` = %u WHERE `id` = %u", peAttr.exp, peAttr.lv, peAttr.skill, equipId);
		if(pet != NULL)
		{
			pet->setDirty();
			pet->sendModification(equip, pos);
		}
		else
			SendSingleEquipData(equip, 2);
		return 1;
	}

    UInt32 PetPackage::equipUpgrade(ItemPetEq * equip, ItemPetEq * eatEq)
    {
        ItemPetEqAttr & peAttr = equip->getPetEqAttr();
        UInt32 upExp = GData::pet.getEquipExpData(peAttr.lv, equip->getQuality() - 2);
        if(upExp == 0) return 0;
        peAttr.exp += (eatEq->GetItemType().trumpExp + eatEq->getPetEqAttr().exp);
        if(peAttr.exp >= upExp)
        {
            UInt8 maxLev = GData::pet.getEquipMaxLev();
            UInt8 tmp = peAttr.lv;
            for(UInt8 i = tmp; i <= maxLev; ++ i)
            {
                if(peAttr.exp < GData::pet.getEquipExpData(i, equip->getQuality() - 2))
                    break;
                ++ peAttr.lv;
            }
            if(peAttr.lv >= maxLev)
            {
                peAttr.lv = maxLev;
                SYSMSG_BROADCASTV(4157, m_Owner->getCountry(), m_Owner->getName().c_str(), equip->getQuality(), equip->getName().c_str(), maxLev);
                return 1;
            }
        }
        return eatEq->getId();
    }

	void PetPackage::SendPackageItemInfor()
	{
		ItemCont::iterator cit = m_PetItems.begin();
		Stream st(REP::FAIRY_PET);
		st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x01);
		st << static_cast<UInt16>(m_PetItems.size());
		for (; cit != m_PetItems.end(); ++ cit)
		{
            AppendItemData(st, cit->second);
		}
		st << static_cast<UInt16>(m_PetEquips.size());
		for (cit = m_PetEquips.begin(); cit != m_PetEquips.end(); ++ cit)
		{
            AppendEquipData(st, static_cast<ItemPetEq *>(cit->second));
		}
		st << Stream::eos;
		m_Owner->send(st);
	}

	void PetPackage::AppendEquipData(Stream& st, ItemPetEq * equip)
	{
		st << equip->getId() << static_cast<UInt8>(equip->GetBindStatus() ? 1 : 0);
		st << equip->GetTypeId();
        equip->getPetEqAttr().appendAttrToStream(st);
	}

	void PetPackage::SendSingleEquipData(ItemPetEq * equip, UInt8 type)
	{   //type: 0删除 1新增 2更新
		Stream st(REP::FAIRY_PET);
		st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x02);
		st << type;
        if(type)
        {
		    AppendEquipData(st, equip);
        }
        else
            st << equip->getId();
		st << Stream::eos;
		m_Owner->send(st);
	}

	void PetPackage::SendItemData(ItemBase * item)
	{
		Stream st(REP::FAIRY_PET);
		st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x03);
		AppendItemData(st, item);
		st << Stream::eos;
		m_Owner->send(st);
	}

    ItemBase* PetPackage::AddRandomPetEq(UInt32 score, UInt32 typeId, int colorIdx, UInt8 FromWhere)
    {
		if(GetPetEqPgRestSize() < 1)
		{
			m_Owner->sendMsgCode(0, 1094);
			return NULL;
		}
		const GData::ItemBaseType * itype = NULL;
        if(score != 0)
            colorIdx = getRandomPetEqGemColorIdx(score, 0);
        else
        {
            if(typeId != 0)
            {
		        itype = GData::itemBaseTypeManager[typeId];
                if(itype == NULL)
                    return NULL;
                colorIdx = itype->quality - 2;
            }
        }
        if(colorIdx < 0)
            return NULL;
        if(typeId == 0)
            typeId = GData::GDataManager::GetPetEqTypeIdByColor(colorIdx);
        UInt16 skillId = GData::GDataManager::GetPetEqSkill();
        if(skillId == 0 || typeId == 0)
            return NULL;
		itype = GData::itemBaseTypeManager[typeId];
		if(itype == NULL)
            return NULL;

        UInt32 id = IDGenerator::gItemOidGenerator.ID();
        ItemPetEqAttr ipeqAttr;
        ItemEquipData edata;
        ipeqAttr.skill = skillId;
        ItemPetEq * equip = new ItemPetEq(id, itype, edata, ipeqAttr);

        if(equip == NULL)
            return NULL;
        equip->SetBindStatus(true);

        return AddPetEquip(equip, true, FromWhere);
    }

    ItemBase* PetPackage::AddRandomPetGem(UInt32 score, int lvIdx)
    {
		if(GetPetGemPgRestSize() < 1)
		{
			m_Owner->sendMsgCode(0, 1094);
			return NULL;
		}
        if(score != 0)
            lvIdx = getRandomPetEqGemColorIdx(score, 1);
        if(lvIdx < 0 || lvIdx > 6)
            return NULL;

        UInt32 itemId = GData::GDataManager::GetPetGemTypeIdByLev(lvIdx);
        return AddPetItem(itemId, 1, true, true, FromBBFT);
    }

}


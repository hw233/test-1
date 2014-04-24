#include "Config.h"
#include "ChatItem.h"
#include "Item.h"
#include "Country.h"
#include "Player.h"
#include "MsgID.h"
#include "Fighter.h"
#include "Package.h"
#include "PetPackage.h"
#include "Script/BattleFormula.h"
#include "Server/WorldServer.h"
#include "Server/OidGenerator.h"
#include "Common/TimeUtil.h"
#include "Married.h"

namespace GObject
{

ChatItem chatItem;

void ChatItem::addItem( Player * player, UInt32 id )
{
	ItemEquip * equip = player->GetPackage()->GetEquip(id);
	if(equip == NULL)
		return;

	ChatItemData& cid = _itemData[id];
	cid.st.init(REP::FLAUNT_GOOD);
	cid.st << static_cast<UInt8>(0x01) << static_cast<UInt8>(player->IsMale() ? 0 : 1) << player->getCountry()
		<< player->getName();
	player->GetPackage()->AppendEquipData(cid.st, equip, false);
	cid.st << Stream::eos;
	cid.lastAccess = TimeUtil::Now();
}

UInt32 ChatItem::addFighter( Player * player, UInt32 id )
{
	Fighter * fgt = player->findFighter(id);
	if(fgt == NULL)
		return 0;

	FIndex fi = {player->getId(), id};
	ChatItemData& cid = _fighterData[fi];
	cid.st.init(REP::FLAUNT_GOOD);
    // XXX:
	cid.st << static_cast<UInt8>(0x02) << static_cast<UInt8>(player->IsMale() ? 0 : 1) << player->getCountry()
		<< player->getName() << static_cast<UInt16>(fgt->getId()) << fgt->getLevel() << fgt->getPotential() << fgt->getPeerlessAndLevel();

	Script::BattleFormula * bformula = Script::BattleFormula::getCurrent();
	UInt32 str = static_cast<UInt32>(bformula->calcStrength(fgt));
	UInt32 agi = static_cast<UInt32>(bformula->calcAgility(fgt));
	UInt32 inte = static_cast<UInt32>(bformula->calcIntelligence(fgt));
	UInt32 will = static_cast<UInt32>(bformula->calcWill(fgt));
	UInt32 phy = static_cast<UInt32>(bformula->calcPhysique(fgt));
	UInt32 atk = static_cast<UInt32>(bformula->calcAttack(fgt));
	UInt32 magatk = static_cast<UInt32>(bformula->calcMagAttack(fgt));
	UInt32 def = static_cast<UInt32>(bformula->calcDefend(fgt));
	UInt32 magdef = static_cast<UInt32>(bformula->calcMagDefend(fgt));
	UInt32 hp = static_cast<UInt32>(bformula->calcHP(fgt));

	UInt16 hit = static_cast<float>(bformula->calcHitrate(fgt, NULL)) * 100;
	UInt16 evd = static_cast<float>(bformula->calcEvade(fgt, NULL)) * 100;
	UInt16 magres = static_cast<float>(bformula->calcMagRes(fgt, NULL)) * 100;
	UInt16 cri = static_cast<float>(bformula->calcCritical(fgt, NULL)) * 100;
	UInt16 prc = static_cast<float>(bformula->calcPierce(fgt, NULL)) * 100;
	UInt16 cnt = static_cast<float>(bformula->calcCounter(fgt, NULL)) * 100;
	UInt16 tough = static_cast<float>(bformula->calcTough(fgt, NULL)) * 100;

	cid.st << str << agi << inte << will << phy << atk << magatk << def << magdef << hp << hit << evd << magres << cri << prc << cnt << tough << Stream::eos;
	cid.lastAccess = TimeUtil::Now();

	return id;
}

void ChatItem::addZhenyuan( Player * player, UInt32 id )
{
	ItemEquip * zhenyuan = player->GetPackage()->GetEquip(id);
	if(zhenyuan == NULL || !IsZhenYuan(zhenyuan->getClass()))
		return;

	ChatItemData& cid = _zhenyuanData[id];
	cid.st.init(REP::FLAUNT_GOOD);
	cid.st << static_cast<UInt8>(0x1B) << static_cast<UInt8>(player->IsMale() ? 0 : 1) << player->getCountry()
		<< player->getName();
    player->GetPackage()->AppendZhenyuanData(cid.st, static_cast<ItemZhenyuan *>(zhenyuan));
	cid.st << Stream::eos;
	cid.lastAccess = TimeUtil::Now();
}

void ChatItem::post( UInt8 type, UInt64 pid, UInt32 id, Player * player )
{
	switch(type)
	{
	case 0x01:
		{
			FastMutex::ScopedLock lk(_itemMutex);
			addItem(player, id);
			return;
		}
		break;
	case 0x02:
		{
			FastMutex::ScopedLock lk(_fighterMutex);
			addFighter(player, id);
			return;
		}
		break;
	case 0x06:
		{
			FastMutex::ScopedLock lk(_petItemMutex);
			addPetItem(player, id, pid);
			return;
		}
		break;
	case 0x07:
		{
			FastMutex::ScopedLock lk(_petMutex);
			addFairyPet(player, id);
			return;
		}
		break;
	case 0x0B:
		{
			FastMutex::ScopedLock lk(_zhenyuanMutex);
			addZhenyuan(player, id);
			return;
		}
		break;
	case 0x1B:
		{
			FastMutex::ScopedLock lk(_zhenyuanMutex);
			std::map<UInt32, ChatItemData>::iterator it = _zhenyuanData.find(id);
			if(it == _zhenyuanData.end())
			{
				return;
			}
			it->second.lastAccess = TimeUtil::Now();
			player->send(it->second.st);
			return;
		}
		break;
    case 0x11:
		{
			FastMutex::ScopedLock lk(_itemMutex);
			std::map<UInt32, ChatItemData>::iterator it = _itemData.find(id);
			if(it == _itemData.end())
			{
				return;
			}
			it->second.lastAccess = TimeUtil::Now();
			player->send(it->second.st);
		}
		break;
	case 0x12:
		{
			FastMutex::ScopedLock lk(_fighterMutex);
			FIndex fi = {pid, id};
			std::map<FIndex, ChatItemData>::iterator it = _fighterData.find(fi);
			if(it == _fighterData.end())
			{
				return;
			}
			it->second.lastAccess = TimeUtil::Now();
			player->send(it->second.st);
			return;
		}
		break;
	case 0x16:
		{
			FastMutex::ScopedLock lk(_petItemMutex);
			std::map<UInt32, ChatItemData>::iterator it = _petItemData.find(id);
			if(it == _petItemData.end())
			{
				return;
			}
			it->second.lastAccess = TimeUtil::Now();
			player->send(it->second.st);
			return;
		}
		break;
	case 0x17:
		{
			FastMutex::ScopedLock lk(_petMutex);
			FIndex fi = {pid, id};
			std::map<FIndex, ChatItemData>::iterator it = _fairyPetData.find(fi);
			if(it == _fairyPetData.end())
			{
				return;
			}
			it->second.lastAccess = TimeUtil::Now();
			player->send(it->second.st);
			return;
		}
		break;
    case 0x19:
		{
			FastMutex::ScopedLock lk(_coupleMutex);
			addCouplePet(player);
			return;
		}
		break;

    case 0x20:
		{
			FastMutex::ScopedLock lk(_coupleMutex);
			std::map<UInt64, ChatItemData>::iterator it = _couplePetData.find(pid);
			if(it == _couplePetData.end())
			{
				return;
			}
			it->second.lastAccess = TimeUtil::Now();
			player->send(it->second.st);
			return;
		}
		break;
	}
}

void ChatItem::purge( UInt32 curtime )
{
	{
		FastMutex::ScopedLock lk(_itemMutex);
		std::map<UInt32, ChatItemData>::iterator it = _itemData.begin();
		while(it != _itemData.end())
		{
			if(curtime > it->second.lastAccess + 3600 * 3)
				_itemData.erase(it ++);
			else
				++ it;
		}
	}

	{
		FastMutex::ScopedLock lk(_fighterMutex);
		std::map<FIndex, ChatItemData>::iterator it = _fighterData.begin();
		while(it != _fighterData.end())
		{
			if(curtime > it->second.lastAccess + 3600 * 3)
				_fighterData.erase(it ++);
			else
				++ it;
		}
	}

	{
		FastMutex::ScopedLock lk(_petItemMutex);
		std::map<UInt32, ChatItemData>::iterator it = _petItemData.begin();
		while(it != _petItemData.end())
		{
			if(curtime > it->second.lastAccess + 3600 * 3)
				_petItemData.erase(it ++);
			else
				++ it;
		}
	}

	{
		FastMutex::ScopedLock lk(_petMutex);
		std::map<FIndex, ChatItemData>::iterator it = _fairyPetData.begin();
		while(it != _fairyPetData.end())
		{
			if(curtime > it->second.lastAccess + 3600 * 3)
				_fairyPetData.erase(it ++);
			else
				++ it;
		}
	}

    {
		FastMutex::ScopedLock lk(_coupleMutex);
		std::map<UInt64, ChatItemData>::iterator it = _couplePetData.begin();
		while(it != _couplePetData.end())
		{
			if(curtime > it->second.lastAccess + 3600 * 3)
				_couplePetData.erase(it ++);
			else
				++ it;
		}
	}

    {
		FastMutex::ScopedLock lk(_zhenyuanMutex);
		std::map<UInt32, ChatItemData>::iterator it = _zhenyuanData.begin();
		while(it != _zhenyuanData.end())
		{
			if(curtime > it->second.lastAccess + 3600 * 3)
				_zhenyuanData.erase(it ++);
			else
				++ it;
		}
	}
}

void ChatItem::addPetItem( Player * player, UInt32 id, UInt32 petId )
{
    FairyPet * pet = NULL;
    UInt8 pos = 0;
    ItemPetEq * equip = player->GetPetPackage()->FindPetEquip(pet, petId, pos, id);
    if(equip == NULL)
        return;

	ChatItemData& cid = _petItemData[id];
	cid.st.init(REP::FLAUNT_GOOD);
	cid.st << static_cast<UInt8>(0x06) << static_cast<UInt8>(player->IsMale() ? 0 : 1) << player->getCountry()
		<< player->getName();
	player->GetPetPackage()->AppendEquipData(cid.st, equip);
	cid.st << Stream::eos;
	cid.lastAccess = TimeUtil::Now();
}

UInt32 ChatItem::addFairyPet( Player * player, UInt32 id )
{
	FairyPet * pet = player->findFairyPet(id);
	if(pet == NULL)
		return 0;

	FIndex fi = {player->getId(), id};
	ChatItemData& cid = _fairyPetData[fi];
	cid.st.init(REP::FLAUNT_GOOD);
	cid.st << static_cast<UInt8>(0x07) << static_cast<UInt8>(player->IsMale() ? 0 : 1)
        << player->getCountry() << player->getName();
    cid.st << id << pet->getPetLev() << pet->getPetBone();
    pet->AppendEquipData(cid.st);
    pet->sendHunPoInfo(cid.st);
    pet->sendSevenSoulInfo(cid.st);
	cid.st << Stream::eos;
	cid.lastAccess = TimeUtil::Now();

	return id;
}

void ChatItem::addCouplePet( Player * player)
{
    if(gMarriedMgr.PreCheckingStatus(player) != 0)
        return;
    
    GObject::Player * man_player = NULL;
    GObject::Player * woman_player = NULL;
    
    GObject::Player * obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
    if(!obj_player || !player) 
        return;
    if(!player->getMainFighter()->getSex())//男的
    {
        man_player = player;
        woman_player = obj_player;
    }
    else
    {
        man_player = obj_player;
        woman_player = player;
    }
    
    ChatItemData& cid = _couplePetData[player->getId()];
	cid.st.init(REP::FLAUNT_GOOD);
    cid.st << static_cast<UInt8>(0x19) << static_cast<UInt8>(player->IsMale() ? 0 : 1) << player->getCountry() << player->getName() << man_player->getName() << man_player->getCountry() << woman_player->getName() << woman_player->getCountry();
    gMarriedMgr.AppendPetData(player,cid.st); 
    cid.st << Stream::eos;
	cid.lastAccess = TimeUtil::Now();

    return;
}


}

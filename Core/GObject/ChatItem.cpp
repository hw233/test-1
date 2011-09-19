#include "Config.h"
#include "ChatItem.h"
#include "Item.h"
#include "Country.h"
#include "Player.h"
#include "MsgID.h"
#include "Fighter.h"
#include "Package.h"
#include "Script/BattleFormula.h"
#include "Server/WorldServer.h"
#include "Server/OidGenerator.h"
#include "Common/TimeUtil.h"

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
	UInt16 str = static_cast<UInt16>(bformula->calcStrength(fgt));
	UInt16 agi = static_cast<UInt16>(bformula->calcAgility(fgt));
	UInt16 inte = static_cast<UInt16>(bformula->calcIntelligence(fgt));
	UInt16 will = static_cast<UInt16>(bformula->calcWill(fgt));
	UInt16 phy = static_cast<UInt16>(bformula->calcPhysique(fgt));
	UInt16 atk = static_cast<UInt16>(bformula->calcAttack(fgt));
	UInt16 magatk = static_cast<UInt16>(bformula->calcMagAttack(fgt));
	UInt16 def = static_cast<UInt16>(bformula->calcDefend(fgt));
	UInt16 magdef = static_cast<UInt16>(bformula->calcMagDefend(fgt));
	UInt16 hp = static_cast<UInt16>(bformula->calcHP(fgt));

	UInt16 hit = static_cast<UInt16>(bformula->calcHitrate(fgt, NULL));
	UInt16 evd = static_cast<UInt16>(bformula->calcEvade(fgt, NULL));
	UInt16 magres = static_cast<UInt16>(bformula->calcMagRes(fgt, NULL));
	UInt16 cri = static_cast<UInt16>(bformula->calcCritical(fgt, NULL));
	UInt16 prc = static_cast<UInt16>(bformula->calcPierce(fgt, NULL));
	UInt16 cnt = static_cast<UInt16>(bformula->calcCounter(fgt, NULL));
	UInt16 tough = static_cast<UInt16>(bformula->calcTough(fgt, NULL));

	cid.st << str << agi << inte << will << phy << atk << magatk << def << magdef << hp << hit << evd << magres << cri << prc << cnt << tough << Stream::eos;
	cid.lastAccess = TimeUtil::Now();

	return id;
}

void ChatItem::post( UInt8 type, UInt64 pid, UInt32 id, Player * player )
{
	switch(type)
	{
	case 0x01:
		{
			addItem(player, id);
			return;
		}
		break;
	case 0x02:
		{
			addFighter(player, id);
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
}

}

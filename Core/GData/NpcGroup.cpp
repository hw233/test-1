#include "Config.h"
#include "NpcGroup.h"
#include "Formation.h"
#include "LootTable.h"
#include "GObject/Fighter.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "GObject/Package.h"
#include "Script/GameActionLua.h"
#include "Script/BattleFormula.h"
#include "Battle/BattleSimulator.h"

namespace GData
{

NpcGroups npcGroups;

void NpcGroup::addFighter( UInt32 id, UInt8 pos )
{
	if(pos > 24)
		return;
	GObject::Fighter * fgt = globalFighters[id];
	if(fgt == NULL)
		return;
	NpcFData fdata = {fgt, pos};
	_npcList.push_back(fdata);
	++ _npcCount[fgt->getId()];
}

void NpcGroup::putFighters( Battle::BattleSimulator& bsim)
{
	size_t c = _npcList.size();
	if(c <= 0)
		return;
	bsim.setFormation(1, _formation);
	if(_npcList[0].fighter->getId() <= GREAT_FIGHTER_MAX)
		bsim.setPortrait(1, _npcList[0].fighter->getId());
	else
		bsim.setPortrait(1, _npcList[0].fighter->favor);
	for(size_t i = 0; i < c; ++ i)
	{
		bsim.newFighter(1, _npcList[i].pos, _npcList[i].fighter);
	}
}

void NpcGroup::monsterKilled( GObject::Player * player, UInt32 count )
{
	std::map<UInt32, UInt16>::iterator it;
	for(it = _npcCount.begin(); it != _npcCount.end(); ++ it)
		GameAction()->MonsterKilled(player, it->first, it->second * count);
}

const std::string& NpcGroup::getName() const
{
	static std::string _empty; return (_npcList.empty() ? _empty : _npcList[0].fighter->getName());
}

void NpcGroup::setLoots( std::vector<const LootItem *>& loots )
{
	_loots = loots;
}

void NpcGroup::getLoots( GObject::Player * player )
{
	std::vector<const LootItem *>::iterator it;
	for(it = _loots.begin(); it != _loots.end(); ++ it)
	{
		(*it)->roll(player);
	}
}

void NpcGroup::getLoots( GObject::Player * player, std::vector<LootResult>& il )
{
	std::vector<const LootItem *>::iterator it;
	for(it = _loots.begin(); it != _loots.end(); ++ it)
	{
		LootResult lr = (*it)->roll();
		if(lr.id == 0)
			continue;
		player->GetPackage()->Add(lr.id, lr.count, GetItemSubClass(lr.id) == Item_Favor, true, FromNpc);
		il.push_back(lr);
	}
}

void NpcGroup::calcBattlePoints( Script::BattleFormula * bformula )
{
	float t = 0.0f;
	size_t c = _npcList.size();
	for(size_t i = 0; i < c; ++ i)
	{
		t += bformula->calcBattlePoint(_npcList[i].fighter);
	}
	_bp = t;
}

UInt8 NpcGroup::getLevel()
{
	return (_npcList.empty() ? 0 : _npcList[0].fighter->getLevel());
}

UInt8 NpcGroup::getClass()
{
	return (_npcList.empty() ? 0 : _npcList[0].fighter->getClass());
}

}

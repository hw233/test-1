#include "Config.h"
#include "WorldScript.h"
#include "GData/Store.h"
#include "GData/Title.h"
#include "GObject/Dungeon.h"
#include "GObject/Player.h"
#include "GObject/Fighter.h"
#include "GObject/World.h"
#include "Server/OidGenerator.h"
#include "GObject/SpecialAward.h"
#include "GameActionLua.h"
#include "Server/Cfg.h"

namespace Script
{

WorldScript::WorldScript( const char * fn )
{
	doFile(fn);
}

void WorldScript::onActivityCheck(UInt32 tm)
{
	call<void>("onActivityCheck", tm);
}


void WorldScript::init()
{
	def("setDungeonCount", GObject::Dungeon::setMaxCount);
	def("setDungeonPrice", GObject::Dungeon::setPrice);
	def("setDungeonExtraCount", GObject::Dungeon::setExtraCount);
	def("setTavernInterval", GObject::Player::setTavernInterval);
	def("setTavernRate", GObject::Player::setTavernRate);
    def("setRecruitCost", GObject::Player::setRecruitCost);
    def("setTavernBlueCount", GObject::Player::setTavernBlueCount);
    def("setTavernPurpleCount", GObject::Player::setTavernPurpleCount);
	def("setActivityStage", GObject::World::setActivityStage);
	def("setActAvailable", GObject::World::setActAvailable);
	def("setActAvailable1", GObject::World::setActAvailable1);
	def("getActivityStage", GObject::World::getActivityStage);
	def("setIsNewServer", GObject::World::setIsNewServer);
	def("Broadcast", SysBroadcast);
	def("GetStore", GetStore);
	def("GetTitleList", GetTitleList);

	class_add<GData::Store>("Store");
	class_def<GData::Store>("clear", &GData::Store::clear);
	class_def<GData::Store>("add", &GData::Store::add);
	class_def<GData::Store>("update", &GData::Store::makePacket);
	class_add<GData::TitleList>("TitleList");
	class_def<GData::TitleList>("set", &GData::TitleList::set);
}

void WorldScript::postInit()
{
	call<void>("initSeed", IDGenerator::gSeedOidGenerator.ID());
	call<void>("setServer", cfg.channelName.c_str(), cfg.serverNum);
	call<void>("initActTime", cfg.openYear, cfg.openMonth, cfg.openDay);
}

UInt32 WorldScript::onAthleticsNewBox( UInt8 type, UInt32 count )
{
	return call<UInt32>("onAthleticsNewBox", type, count);
}

GData::Store * WorldScript::GetStore()
{
	return &GData::store;
}

GData::TitleList * WorldScript::GetTitleList()
{
	return &GData::titleList;
}

}

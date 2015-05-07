#include "Config.h"
#include "WorldScript.h"
#include "Server/OidGenerator.h"
#include "GameActionLua.h"
#include "Server/Cfg.h"
#include "GData/Store.h"

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
}

void WorldScript::postInit()
{
	call<void>("initSeed", IDGenerator::gSeedOidGenerator.ID());
	//call<void>("setServer", cfg.channelName.c_str(), cfg.serverNum);
	//call<void>("initActTime", cfg.openYear, cfg.openMonth, cfg.openDay);
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
	return NULL;//&GData::titleList;
}

void WorldScript::forceCommitArena()
{
	call<void>("forceCommitArena");
}

}

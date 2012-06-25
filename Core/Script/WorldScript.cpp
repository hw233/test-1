#include "Config.h"
#include "WorldScript.h"
#include "GData/Store.h"
#include "GData/Title.h"
#include "GObject/Dungeon.h"
#include "GObject/Player.h"
#include "GObject/Copy.h"
#include "GObject/FrontMap.h"
#include "GObject/Fighter.h"
#include "GObject/World.h"
#include "Server/OidGenerator.h"
#include "GObject/SpecialAward.h"
#include "GameActionLua.h"
#include "Server/Cfg.h"
#include "GObject/HeroIsland.h"
#include "GObject/TeamCopy.h"

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
    def("setTavernOrangeCount", GObject::Player::setTavernOrangeCount);
	def("isFBVersion", GObject::World::isFBVersion);
	def("isDebug", GObject::World::isDebug);
	def("setActivityStage", GObject::World::setActivityStage);
	def("setActAvailable", GObject::World::setActAvailable);
	def("setActAvailable1", GObject::World::setActAvailable1);
	def("getActivityStage", GObject::World::getActivityStage);
	def("setIsNewServer", GObject::World::setIsNewServer);
	def("setNationalDay", GObject::World::setNationalDay);
	def("setHalloween", GObject::World::setHalloween);
	def("setAutoHeal", GObject::World::setAutoHeal);
	def("getAutoHeal", GObject::World::getAutoHeal);
	def("setSingleDay", GObject::World::setSingleDay);
	def("getSingleDay", GObject::World::getSingleDay);
	def("setThanksgiving", GObject::World::setThanksgiving);
	def("getThanksgiving", GObject::World::getThanksgiving);
	def("getChristmas", GObject::World::getChristmas);
	def("setChristmas", GObject::World::setChristmas);
	def("getNewYear", GObject::World::getNewYear);
	def("setNewYear", GObject::World::setNewYear);
	def("getBlueactiveday", GObject::World::getBlueactiveday);
	def("setBlueactiveday", GObject::World::setBlueactiveday);
	def("getRechargeActive", GObject::World::getRechargeActive);
	def("setRechargeActive", GObject::World::setRechargeActive);
	def("getValentineDay", GObject::World::getValentineDay);
	def("setValentineDay", GObject::World::setValentineDay);
	def("getNetValentineDay", GObject::World::getNetValentineDay);
	def("setNetValentineDay", GObject::World::setNetValentineDay);
	def("getGirlDay", GObject::World::getGirlDay);
	def("setGirlDay", GObject::World::setGirlDay);
	def("getWhiteLoveDay", GObject::World::getWhiteLoveDay);
	def("setWhiteLoveDay", GObject::World::setWhiteLoveDay);
	def("getTrumpEnchRet", GObject::World::getTrumpEnchRet);
	def("setTrumpEnchRet", GObject::World::setTrumpEnchRet);
	def("getFoolsDay", GObject::World::getFoolsDay);
	def("setFoolsDay", GObject::World::setFoolsDay);
	def("getChingMing", GObject::World::getChingMing);
	def("setChingMing", GObject::World::setChingMing);
	def("getCarnival", GObject::World::getCarnival);
	def("setCarnival", GObject::World::setCarnival);
	def("getFighter1368", GObject::World::getFighter1368);
	def("setFighter1368", GObject::World::setFighter1368);
	def("getEnchantAct", GObject::World::getEnchantAct);
	def("setEnchantAct", GObject::World::setEnchantAct);
	def("getTrainFighter", GObject::World::getTrainFighter);
	def("setTrainFighter", GObject::World::setTrainFighter);
	def("getRC7Day", GObject::World::getRC7Day);
	def("setRC7Day", GObject::World::setRC7Day);
	def("getShuoShuo", GObject::World::getShuoShuo);
	def("setShuoShuo", GObject::World::setShuoShuo);
	def("getCFriend", GObject::World::getCFriend);
	def("setCFriend", GObject::World::setCFriend);
	def("getMayDay", GObject::World::getMayDay);
	def("setMayDay", GObject::World::setMayDay);
	def("getMayDay1", GObject::World::getMayDay1);
	def("setMayDay1", GObject::World::setMayDay1);
	def("getYDMDAct", GObject::World::getYDMDAct);
	def("setYDMDAct", GObject::World::setYDMDAct);
	def("getSSDTAct", GObject::World::getSSDTAct);
	def("setSSDTAct", GObject::World::setSSDTAct);
	def("getGemMergeAct", GObject::World::getGemMergeAct);
	def("setGemMergeAct", GObject::World::setGemMergeAct);
	def("getEnchantGt11", GObject::World::getEnchantGt11);
	def("setEnchantGt11", GObject::World::setEnchantGt11);
    def("setBlueDiamondAct", GObject::World::setBlueDiamondAct);
    def("setYellowDiamondAct", GObject::World::setYellowDiamondAct);
    def("setQQGameAct", GObject::World::setQQGameAct);
    def("getBlueDiamondAct", GObject::World::getBlueDiamondAct);
    def("getYellowDiamondAct", GObject::World::getYellowDiamondAct);
    def("getQQGameAct", GObject::World::getQQGameAct);
	def("setJune", GObject::World::setJune);
	def("getJune", GObject::World::getJune);
	def("setJune1", GObject::World::setJune1);
	def("getJune1", GObject::World::getJune1);
	def("setRechargeNextRet", GObject::World::setRechargeNextRet);
	def("getRechargeNextRet", GObject::World::getRechargeNextRet);
	def("setRechargeNextRetStart", GObject::World::setRechargeNextRetStart);
	def("Broadcast", SysBroadcast);
	def("GetStore", GetStore);
	def("GetTitleList", GetTitleList);
    def("setShiMenActiveCount", GObject::Player::setShiMenActiveCount);
    def("setYaMenActiveCount", GObject::Player::setYaMenActiveCount);
    def("setCopyActiveCount", GObject::PlayerCopy::setCopyActiveCount);
    def("setFrontMapActiveCount", GObject::FrontMap::setFrontMapActiveCount);
    def("setRareAnimals", GObject::HeroIsland::setRareAnimals);
    def("addHIAwardsCfg", GObject::HeroIsland::addHIAwardsCfg);
    def("addRankAwards", GObject::HeroIsland::addRankAwards);
    def("clearAllHICfg", GObject::HeroIsland::clearAllHICfg);
    def("addTeamCopyAwardCfg", GObject::TeamCopyPlayerInfo::addTeamCopyAwardCfg);
    def("clearTeamCopyAwardCfg", GObject::TeamCopyPlayerInfo::clearTeamCopyAwardCfg);
    def("addGM", GObject::GObjectManager::addGM);
    def("delGM", GObject::GObjectManager::delGM);
    def("clearUdpItem", GData::GDataManager::clearUdpItem);
    def("addUdpItem", GData::GDataManager::addUdpItem);
    def("setFFTypeChance", GObject::GObjectManager::setFFTypeChance);
    def("setFFAttrChance", GObject::GObjectManager::setFFAttrChance);
    def("setFFAttrMaxValProp", GObject::GObjectManager::setFFAttrMaxValProp);
    def("setFFAttrMaxVal", GObject::GObjectManager::setFFAttrMaxVal);

	class_add<GData::Store>("Store");
	class_def<GData::Store>("clear", &GData::Store::clear);
	class_def<GData::Store>("add", &GData::Store::add);
	class_def<GData::Store>("addExchange", &GData::Store::addExchange);
	class_def<GData::Store>("needResetDiscount", &GData::Store::needResetDiscount);
	class_def<GData::Store>("storeDiscount", &GData::Store::storeDiscount);
	class_def<GData::Store>("discountLimit", &GData::Store::discountLimit);
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

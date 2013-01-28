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
#include "GObject/ClanCopy.h"

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
	def("isVTVersion", GObject::World::isVTVersion);
	def("isDebug", GObject::World::isDebug);
	def("setActivityStage", GObject::World::setActivityStage);
	def("setActAvailable", GObject::World::setActAvailable);
	def("setActAvailable1", GObject::World::setActAvailable1);
	def("getActivityStage", GObject::World::getActivityStage);
	def("setIsNewServer", GObject::World::setIsNewServer);
	def("isNewServer", GObject::World::IsNewServer);
	def("setNationalDay", GObject::World::setNationalDay);
	def("setHalloween", GObject::World::setHalloween);
	def("setAutoHeal", GObject::World::setAutoHeal);
	def("getAutoHeal", GObject::World::getAutoHeal);
	def("setCopyFrontWinSwitch", GObject::World::setCopyFrontWinSwitch);
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
	def("getYearActive", GObject::World::getYearActive);
	def("setYearActive", GObject::World::setYearActive);
	def("getQgameGiftAct", GObject::World::getQgameGiftAct);
	def("setQgameGiftAct", GObject::World::setQgameGiftAct);
	def("getRechargeActive3366", GObject::World::getRechargeActive3366);
	def("setRechargeActive3366", GObject::World::setRechargeActive3366);
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
    def("setQQVipAct", GObject::World::setQQVipAct);
    def("setFallAct", GObject::World::setFallAct);
    def("setQQGameAct", GObject::World::setQQGameAct);
    def("set3366PrivilegeAct", GObject::World::set3366PrivilegeAct);
    def("setQzonePYPrivilegeAct", GObject::World::setQzonePYPrivilegeAct);
    def("setGoodVoiceAct", GObject::World::setGoodVoiceAct);
    def("set3366GiftAct", GObject::World::set3366GiftAct);
    def("getBlueDiamondAct", GObject::World::getBlueDiamondAct);
    def("getYellowDiamondAct", GObject::World::getYellowDiamondAct);
    def("getQQVipAct", GObject::World::getQQVipAct);
    def("getFallAct", GObject::World::getFallAct);
    def("getQQGameAct", GObject::World::getQQGameAct);
    def("get3366PrivilegeAct", GObject::World::get3366PrivilegeAct);
    def("getQzonePYPrivilegeAct", GObject::World::getQzonePYPrivilegeAct);
	def("setJune", GObject::World::setJune);
	def("getJune", GObject::World::getJune);
	def("setJune1", GObject::World::setJune1);
	def("getJune1", GObject::World::getJune1);
	def("setJuly", GObject::World::setJuly);
	def("getJuly", GObject::World::getJuly);
    def("setQixi", GObject::World::setQixi);
    def("getQixi", GObject::World::getQixi);
    def("setWansheng", GObject::World::setWansheng);
    def("getWansheng", GObject::World::getWansheng);
    def("set11Act", GObject::World::set11Act);
    def("get11Act", GObject::World::get11Act);
    def("setSSToolbarAct", GObject::World::setSSToolbarAct);
    def("setFeastLoginAct", GObject::World::setFeastLoginAct);
    def("setTowerLoginAct", GObject::World::setTowerLoginAct);
    def("getSSToolbarAct", GObject::World::getSSToolbarAct);
    def("setSnowAct", GObject::World::setSnowAct);
    def("getSnowAct", GObject::World::getSnowAct);
    def("setGoldSnakeAct", GObject::World::setGoldSnakeAct);
    def("getGoldSnakeAct", GObject::World::getGoldSnakeAct);
    def("setHeroIslandAct", GObject::World::setHeroIslandAct);
    def("setDragonKingAct", GObject::World::setDragonKingAct);
    def("getDragonKingAct", GObject::World::getDragonKingAct);
    
    def("setGuoqing", GObject::World::setGuoqing);
    def("getGuoqing", GObject::World::getGuoqing);
    def("set9215Act", GObject::World::set9215Act);
    def("get9215Act", GObject::World::get9215Act);
	def("setRechargeNextRet", GObject::World::setRechargeNextRet);
	def("getRechargeNextRet", GObject::World::getRechargeNextRet);
	def("setRechargeNextRetStart", GObject::World::setRechargeNextRetStart);
	def("setLevelAwardEnd", GObject::World::setLevelAwardEnd);
	def("setDuanWu", GObject::World::setDuanWu);
	def("getDuanWu", GObject::World::getDuanWu);
	def("setICAct", GObject::World::setICAct);
	def("getICAct", GObject::World::getICAct);
	def("setPExpItems", GObject::World::setPExpItems);
	def("getPExpItems", GObject::World::getPExpItems);
	def("setSoSoMapBegin", GObject::World::setSoSoMapBegin);
	def("setOpenTest", GObject::World::setOpenTest);
	def("getOpenTest", GObject::World::getOpenTest);
	def("setConsumeActive", GObject::World::setConsumeActive);
	def("setConsume918", GObject::World::setConsume918);
	def("getConsume918", GObject::World::getConsume918);
	def("setConsumeAwardAct", GObject::World::setConsumeAwardAct);
    def("getConsumeActive", GObject::World::getConsumeActive);
    def("setKillMonsterAct", GObject::World::setKillMonsterAct);
    def("getKillMonsterAct", GObject::World::getKillMonsterAct);
	def("setNeedRechargeRank", GObject::World::setNeedRechargeRank);
	def("getNeedRechargeRank", GObject::World::getNeedRechargeRank);
	def("setNeedConsumeRank", GObject::World::setNeedConsumeRank);
	def("getNeedConsumeRank", GObject::World::getNeedConsumeRank);
	def("setRechargeTime", GObject::World::setRechargeTime);
	def("setConsumeTime", GObject::World::setConsumeTime);
	def("setTgcEvent", GObject::World::setTgcEvent);
	def("getTgcEvent", GObject::World::getTgcEvent);
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
	def("setTownReward_10_15", GObject::World::setTownReward_10_15);
	def("setCompassAct", GObject::World::setCompassAct);
	def("getCompassAct", GObject::World::getCompassAct);
	def("setCallSnakeEggAct", GObject::World::setCallSnakeEggAct);
	def("setSnakeEggAwardAct", GObject::World::setSnakeEggAwardAct);
	def("setItem9344Act", GObject::World::setItem9344Act);
	def("getItem9344Act", GObject::World::getItem9344Act);
	def("setItem9343Act", GObject::World::setItem9343Act);
	def("getItem9343Act", GObject::World::getItem9343Act);

    def("setMergeAthAct", GObject::World::setMergeAthAct);
    def("getMergeAthAct", GObject::World::getMergeAthAct);
    def("setFourCopAct", GObject::World::setFourCopAct);
    def("getFourCopAct", GObject::World::getFourCopAct);
    def("commitArenaForceOnce", GObject::World::commitArenaForceOnce);

	class_add<GData::Store>("Store");
	class_def<GData::Store>("clear", &GData::Store::clear);
    class_def<GData::Store>("clearNormalDiscount", &GData::Store::clearNormalDiscount);
    class_def<GData::Store>("clearSpecialDiscount", &GData::Store::clearSpecialDiscount);
	class_def<GData::Store>("add", &GData::Store::add);
	class_def<GData::Store>("addExchange", &GData::Store::addExchange);
	class_def<GData::Store>("needResetDiscount", &GData::Store::needResetDiscount);
	class_def<GData::Store>("storeDiscount", &GData::Store::storeDiscount);
    class_def<GData::Store>("addSpecialDiscount", &GData::Store::addSpecialDiscount);
    class_def<GData::Store>("addNormalDiscount", &GData::Store::addNormalDiscount);
	class_def<GData::Store>("update", &GData::Store::makePacket);
    class_add<GData::Discount>("Discount");
	class_add<GData::TitleList>("TitleList");
	class_def<GData::TitleList>("set", &GData::TitleList::set);

    def("setLoginAward", GObject::World::setLoginAward);
    def("getLoginAward", GObject::World::getLoginAward);
    def("setBDSuperman", GObject::World::setBDSuperman);
    //def("setInterval", GObject::ClanCopy::setInterval);
    //def("setStartTick", GObject::ClanCopy::setStartTick);
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

void WorldScript::forceCommitArena()
{
	call<void>("forceCommitArena");
}

}

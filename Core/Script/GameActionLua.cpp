#include "Config.h"
#include "GameActionLua.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "GObject/Fighter.h"
#include "GObject/Mail.h"
#include "GObject/TaskMgr.h"
#include "GObject/Package.h"
#include "GObject/MOAction.h"
#include "GObject/AttainMgr.h"
#include "GObject/Country.h"
#include "GObject/RealItemAward.h"
#include "Log/Log.h"
#include "MsgID.h"
#include "GObject/QixiTmpl.h"

#include "MsgHandler/CountryMsgStruct.h"
#include "Network/TcpServerWrapper.h"
#include "Server/OidGenerator.h"

using namespace GObject;

namespace Script
{
	GameActionLua::GameActionLua(UInt8 tid, const char * path) : _player1(NULL), _player2(NULL), _tid(tid)
	{
		doFile(path);
	}

	GameActionLua::~GameActionLua()
	{

	}

	void GameActionLua::init()
	{
		RegisterActionInterface();
	}

	void GameActionLua::postInit()
	{
		lua_tinker::call<void>(_L, "initSeed", IDGenerator::gSeedOidGenerator.ID());
		TRACE_LOG("Script loaded...");
	}

	void SysBroadcast(UInt8 type, const char * msg)
	{
		Stream st(REP::SYSTEM_INFO);
		st << type << msg << Stream::eos;
		NETWORK()->Broadcast(st);
	}

	void SysSendMsg(GObject::Player * player, UInt8 type, const char * msg)
	{
		Stream st(REP::SYSTEM_INFO);
		st << type << msg << Stream::eos;
		player->send(st);
	}

	void GameActionLua::RegisterActionInterface()
	{
		#define CLASS_DEF(klass,member)	 \
			lua_tinker::class_def<klass>(_L, #member, &klass::member)
		#define CLASS_ADD(klass)	\
			lua_tinker::class_add<klass>(_L, #klass);
		#define CLASS_STATIC_DEF(klass,member)	\
			lua_tinker::class_def<klass>(_L, #klass "_" #member, &klass::member)

		lua_tinker::class_add<GameActionLua>(_L, "GameActionLua");
		lua_tinker::set(_L, "_GameActionLua", this);

		//static function register or free function
		lua_tinker::def(_L, "GetTaskReqStep", &TaskMgr::GetTaskReqStep);
		lua_tinker::def(_L, "IsEquipId",	IsEquipId);
		lua_tinker::def(_L, "IsEquipTypeId",IsEquipTypeId);
		lua_tinker::def(_L, "IsEquip",		IsEquip);
		lua_tinker::def(_L, "Broadcast",	SysBroadcast);
		lua_tinker::def(_L, "SendMsg",		SysSendMsg);
		lua_tinker::def(_L, "TaskAction",	&MOAction::TaskAction);
        lua_tinker::def(_L, "isFBVersion", GObject::World::isFBVersion);
		lua_tinker::def(_L, "isVTVersion", GObject::World::isVTVersion);
		lua_tinker::def(_L, "isNewServer", GObject::World::IsNewServer);
		lua_tinker::def(_L, "isDebug", GObject::World::isDebug);
		lua_tinker::def(_L, "getActivityStage",	GObject::World::getActivityStage);
		lua_tinker::def(_L, "getAutoHeal",	GObject::World::getAutoHeal);
		lua_tinker::def(_L, "getSingleDay",	GObject::World::getSingleDay);
		lua_tinker::def(_L, "getChristmas",	GObject::World::getChristmas);
		lua_tinker::def(_L, "getNewYear",	GObject::World::getNewYear);
		lua_tinker::def(_L, "getBlueactiveday",	GObject::World::getBlueactiveday);
		lua_tinker::def(_L, "getRechargeActive", GObject::World::getRechargeActive);
		lua_tinker::def(_L, "getRechargeActive3366", GObject::World::getRechargeActive3366);
		lua_tinker::def(_L, "getYearActive", GObject::World::getYearActive);
		lua_tinker::def(_L, "getQgameGiftAct", GObject::World::getQgameGiftAct);
		lua_tinker::def(_L, "getValentineDay", GObject::World::getValentineDay);
		lua_tinker::def(_L, "getNetValentineDay", GObject::World::getNetValentineDay);
		lua_tinker::def(_L, "getGirlDay", GObject::World::getGirlDay);
		lua_tinker::def(_L, "getWhiteLoveDay", GObject::World::getWhiteLoveDay);
		lua_tinker::def(_L, "getTrumpEnchRet", GObject::World::getTrumpEnchRet);
		lua_tinker::def(_L, "getFoolsDay", GObject::World::getFoolsDay);
		lua_tinker::def(_L, "getChingMing", GObject::World::getChingMing);
		lua_tinker::def(_L, "getCarnival", GObject::World::getCarnival);
		lua_tinker::def(_L, "getFighter1368", GObject::World::getFighter1368);
		lua_tinker::def(_L, "getEnchantAct", GObject::World::getEnchantAct);
		lua_tinker::def(_L, "getTrainFighter", GObject::World::getTrainFighter);
		lua_tinker::def(_L, "getRC7Day", GObject::World::getRC7Day);
		lua_tinker::def(_L, "getShuoShuo", GObject::World::getShuoShuo);
		lua_tinker::def(_L, "getCFriend", GObject::World::getCFriend);
		lua_tinker::def(_L, "getMayDay", GObject::World::getMayDay);
		lua_tinker::def(_L, "getMayDay1", GObject::World::getMayDay1);
		lua_tinker::def(_L, "getJune", GObject::World::getJune);
		lua_tinker::def(_L, "getJune1", GObject::World::getJune1);
		lua_tinker::def(_L, "getJuly", GObject::World::getJuly);
        lua_tinker::def(_L, "getQixi", GObject::World::getQixi);
        lua_tinker::def(_L, "getWansheng", GObject::World::getWansheng);
        lua_tinker::def(_L, "getQingren", GObject::World::getQingren);
        lua_tinker::def(_L, "getGuoqing", GObject::World::getGuoqing);
		lua_tinker::def(_L, "getYDMDAct", GObject::World::getYDMDAct);
		lua_tinker::def(_L, "getWeekDay",	GObject::World::getWeekDay);
		lua_tinker::def(_L, "getThanksgiving",	GObject::World::getThanksgiving);
		lua_tinker::def(_L, "getRandOEquip",	GObject::getRandOEquip);
		lua_tinker::def(_L, "getRandPEquip",	GObject::getRandPEquip);
        lua_tinker::def(_L, "getRandGem" ,      GObject::getRandGem);
		lua_tinker::def(_L, "getGemMergeAct", GObject::World::getGemMergeAct);
		lua_tinker::def(_L, "getEnchantGt11", GObject::World::getEnchantGt11);
        lua_tinker::def(_L, "getBlueDiamondAct", GObject::World::getBlueDiamondAct);
        lua_tinker::def(_L, "getYellowDiamondAct", GObject::World::getYellowDiamondAct);
        lua_tinker::def(_L, "getQQVipAct", GObject::World::getQQVipAct);
        lua_tinker::def(_L, "getFallAct", GObject::World::getFallAct);
        lua_tinker::def(_L, "getQQGameAct", GObject::World::getQQGameAct);
        lua_tinker::def(_L, "get3366PrivilegeAct", GObject::World::get3366PrivilegeAct);
        lua_tinker::def(_L, "getQzonePYPrivilegeAct", GObject::World::getQzonePYPrivilegeAct);
        lua_tinker::def(_L, "getRechargeNextRet", GObject::World::getRechargeNextRet);
        lua_tinker::def(_L, "setRechargeNextRetStart", GObject::World::setRechargeNextRetStart);
        lua_tinker::def(_L, "setMergeAthAct", GObject::World::setMergeAthAct);
        lua_tinker::def(_L, "getMergeAthAct", GObject::World::getMergeAthAct);
        lua_tinker::def(_L, "setFourCopAct", GObject::World::setFourCopAct);
        lua_tinker::def(_L, "getFourCopAct", GObject::World::getFourCopAct);
        lua_tinker::def(_L, "getPExpItems", GObject::World::getPExpItems);
        lua_tinker::def(_L, "getOpenTest", GObject::World::getOpenTest);
        lua_tinker::def(_L, "getConsumeActive", GObject::World::getConsumeActive);
        lua_tinker::def(_L, "getNeedRechargeRank", GObject::World::getNeedRechargeRank);
        lua_tinker::def(_L, "getNeedConsumeRank", GObject::World::getNeedConsumeRank);
        lua_tinker::def(_L, "getKillMonsterAct", GObject::World::getKillMonsterAct);
        lua_tinker::def(_L, "getTgcEvent", GObject::World::getTgcEvent);
        lua_tinker::def(_L, "get9215Act", GObject::World::get9215Act);
        lua_tinker::def(_L, "getSnowAct", GObject::World::getSnowAct);
		lua_tinker::def(_L, "getCompassAct", GObject::World::getCompassAct);
		lua_tinker::def(_L, "getItem9344Act", GObject::World::getItem9344Act);
		lua_tinker::def(_L, "getItem9343Act", GObject::World::getItem9343Act);
		lua_tinker::def(_L, "getAutoBattleAct", GObject::World::getAutoBattleAct);
		lua_tinker::def(_L, "getSnakeSpringEquipAct", GObject::World::setSnakeSpringEquipAct);
		lua_tinker::def(_L, "getFoolBao", GObject::World::getFoolBao);
		lua_tinker::def(_L, "getOpenTime", GObject::World::getOpenTime);
		lua_tinker::def(_L, "isRPServer", GObject::World::isRPServer);

        CLASS_DEF(GameActionLua, Print);
        lua_tinker::def(_L, "getDuanWu", GObject::World::getDuanWu);
        lua_tinker::def(_L, "getICAct", GObject::World::getICAct);
        lua_tinker::def(_L, "setLevelAwardEnd", GObject::World::setLevelAwardEnd);
		CLASS_DEF(GameActionLua, Print);
		CLASS_DEF(GameActionLua, GetPlayer1);
		CLASS_DEF(GameActionLua, GetPlayer2);
		CLASS_DEF(GameActionLua, GetNpcRelationTask);
		CLASS_DEF(GameActionLua, GetTaskData);
		CLASS_DEF(GameActionLua, SetPlayerData);
		CLASS_DEF(GameActionLua, GetPlayerData);
		CLASS_DEF(GameActionLua, RunConveyAction);
		CLASS_DEF(GameActionLua, GetPlayerName);
		CLASS_DEF(GameActionLua, GetPlayerStateName);
		CLASS_DEF(GameActionLua, RunTask);
		CLASS_DEF(GameActionLua, RunDayTaskAccept);
		CLASS_DEF(GameActionLua, RunAutoBattleAction);
		CLASS_DEF(GameActionLua, GetDayTaskCompletedCount);
		CLASS_DEF(GameActionLua, GetDayTaskFlushColor);
		CLASS_DEF(GameActionLua, GetItemName);
		CLASS_DEF(GameActionLua, GetGreatFighterName);
		CLASS_DEF(GameActionLua, RunItemTaskAction);
        CLASS_DEF(GameActionLua, GetSharpDay);

		CLASS_ADD(Player);
		CLASS_DEF(Player, getPName);
		CLASS_DEF(Player, IsMale);
		CLASS_DEF(Player, isBD);
		CLASS_DEF(Player, GetLev);
		CLASS_DEF(Player, getTotalRecharge);
		CLASS_DEF(Player, GetExp);
		CLASS_DEF(Player, AddExp);
		CLASS_DEF(Player, AddPExp);
		CLASS_DEF(Player, AddPExpBy);
		CLASS_DEF(Player, AddItemBy);
		CLASS_DEF(Player, getGold);
		CLASS_DEF(Player, useGold);
		CLASS_DEF(Player, useGold4LuckDraw);
		CLASS_DEF(Player, getGold4LuckDraw);
		CLASS_DEF(Player, useGoldInLua);
		CLASS_DEF(Player, getGoldInLua);
		CLASS_DEF(Player, getCoupon);
		CLASS_DEF(Player, useCoupon);
		CLASS_DEF(Player, pendCoupon);
		CLASS_DEF(Player, getTael);
		CLASS_DEF(Player, useTael);
		CLASS_DEF(Player, getCoin);
		CLASS_DEF(Player, useCoin);
		CLASS_DEF(Player, addStatus);
		CLASS_DEF(Player, removeStatus);
		CLASS_DEF(Player, hasStatus);
		CLASS_DEF(Player, hasStatusAny);
		CLASS_DEF(Player, addStatusBit);
		CLASS_DEF(Player, setStatusBit);
		CLASS_DEF(Player, removeStatusBit);
		CLASS_DEF(Player, getStatusBit);
		CLASS_DEF(Player, hasStatusBit);
		CLASS_DEF(Player, setTitle);
		CLASS_DEF(Player, getTitle);
		CLASS_DEF(Player, getPrestige);
		CLASS_DEF(Player, getAchievement);
		CLASS_DEF(Player, useAchievement);
		CLASS_DEF(Player, getLocation);
		CLASS_DEF(Player, setLocation);
		CLASS_DEF(Player, IsInTeam);
		CLASS_DEF(Player, getLocation);
		CLASS_DEF(Player, getCountry);
		CLASS_DEF(Player, GetClass);
		CLASS_DEF(Player, GetTaskMgr);
		CLASS_DEF(Player, GetPackage);
		CLASS_DEF(Player, GetMailBox);
		CLASS_DEF(Player, GetFreePackageSize);
		CLASS_DEF(Player, addFightCurrentHp);
		CLASS_DEF(Player, addFightCurrentHpAll);
		CLASS_DEF(Player, getFighterCount);
		CLASS_DEF(Player, getMainFighter);
		CLASS_DEF(Player, regenHP);
		CLASS_DEF(Player, findFighter);
		CLASS_DEF(Player, getBuffData);
		CLASS_DEF(Player, setBuffData);
		CLASS_DEF(Player, sendExpHook);
		CLASS_DEF(Player, moveTo);
		CLASS_DEF(Player, moveToHome);
		CLASS_DEF(Player, moveToNeutralHome);
		CLASS_DEF(Player, autoRegenAll);
		CLASS_DEF(Player, sendMsgCode);
		CLASS_DEF(Player, isMainFighter);
		CLASS_DEF(Player, getGoldOrCoupon);
		CLASS_DEF(Player, useGoldOrCoupon);
		CLASS_DEF(Player, hasFlag);
		CLASS_DEF(Player, takeFighter);
		CLASS_DEF(Player, isFighterFull);
		CLASS_DEF(Player, getNextExtraReward);
		CLASS_DEF(Player, setNextExtraReward);
		CLASS_DEF(Player, isDungeonPassed);
		CLASS_DEF(Player, getVipLevel);
		CLASS_DEF(Player, getLastOnline);
		CLASS_DEF(Player, sendTopupMail);
		CLASS_DEF(Player, setTicketCount);
		CLASS_DEF(Player, getTicketCount);
		CLASS_DEF(Player, getClan);
		CLASS_DEF(Player, getCreated);
		CLASS_DEF(Player, getBlockBossLevel);
        CLASS_DEF(Player, getClanTaskId);
        CLASS_DEF(Player, isClanTaskFull);
        CLASS_DEF(Player, isClanTask);
        CLASS_DEF(Player, AddClanBuilding);
        CLASS_DEF(Player, AddClanContrib);
		CLASS_DEF(Player, GetAttainMgr);
        CLASS_DEF(Player, GetActivityMgr);
        CLASS_DEF(Player, GetStrengthenMgr);
		CLASS_DEF(Player, addNewFormation);
		CLASS_DEF(Player, openLevelBox);
		CLASS_DEF(Player, sendMailPack);
		CLASS_DEF(Player, setVipAwardFlag);
        CLASS_DEF(Player, GetVar);
        CLASS_DEF(Player, GetVarS);
        CLASS_DEF(Player, SetVar);
        CLASS_DEF(Player, AddVar);
        CLASS_DEF(Player, AddVarS);
		CLASS_DEF(Player, getAttainment);
		CLASS_DEF(Player, isOffical);
		CLASS_DEF(Player, getPlatform);
		CLASS_DEF(Player, OnShuoShuo);
        CLASS_DEF(Player, sendMDSoul);
        CLASS_DEF(Player, sendHappyInfo);
        CLASS_DEF(Player, lastLootPush);
        CLASS_DEF(Player, RegisterAward);
        CLASS_DEF(Player, BirthdayAward);
        CLASS_DEF(Player, hasRealItemAward);
        CLASS_DEF(Player, getRealItemAward);
        CLASS_DEF(Player, getMoneyArenaLua);
		CLASS_DEF(Player, getQQVipPrivilege);
		CLASS_DEF(Player, setQQVipPrivilege);
		CLASS_DEF(Player, postRoamResult);
		CLASS_DEF(Player, postKillMonsterRoamResult);
        CLASS_DEF(Player, lastQueqiaoAwardPush);
        CLASS_DEF(Player, lastKillMonsterAwardPush);
        CLASS_DEF(Player, lastNew7DayTargetAwardPush);
		CLASS_DEF(Player, lastCFTicketsAward);
        CLASS_DEF(Player, luaUdpLog);
        CLASS_DEF(Player, addFighterFromItem);
        CLASS_DEF(Player, hasFighter);
        CLASS_DEF(Player, fighterFromItem);
        CLASS_DEF(Player, appendCompassItem);
        CLASS_DEF(Player, getXianyuanLua);
        CLASS_DEF(Player, getFengsuiLua);
        CLASS_DEF(Player, getLongyuanLua);
        CLASS_DEF(Player, getPetByPetEgg);
        CLASS_DEF(Player, SetVipPrivilege);
        CLASS_DEF(Player, sendVipPrivilege);

        CLASS_ADD(Fighter);
		CLASS_DEF(Fighter, regenHP);
		CLASS_DEF(Fighter, getCurrentHP);
		CLASS_DEF(Fighter, getMaxHP);
		CLASS_DEF(Fighter, addExp);
		CLASS_DEF(Fighter, getExp);
		CLASS_DEF(Fighter, addPExp);
		CLASS_DEF(Fighter, getPExp);
		CLASS_DEF(Fighter, getPExpMax);
		CLASS_DEF(Fighter, isPExpFull);
		CLASS_DEF(Fighter, getExpMax);
		CLASS_DEF(Fighter, isExpFull);
		CLASS_DEF(Fighter, getAcupointCnt);
		CLASS_DEF(Fighter, getBuffData);
		CLASS_DEF(Fighter, setBuffData);
		CLASS_DEF(Fighter, getClass);
		CLASS_DEF(Fighter, getSex);
		// TODO: CLASS_DEF(Fighter, getSkill);
		// CLASS_DEF(Fighter, setSkill);
		// CLASS_DEF(Fighter, setSkillLevel);
		// CLASS_DEF(Fighter, learnSkill);
		CLASS_DEF(Fighter, skillLevelUp);
		CLASS_DEF(Fighter, addNewCitta);
		CLASS_DEF(Fighter, changeSecondSoulClass);
		CLASS_DEF(Fighter, addElixirAttrByOffset);
		CLASS_DEF(Fighter, getElixirAttrByOffset);
		CLASS_DEF(Fighter, changeSecondSoulXinxiu);
		CLASS_DEF(Fighter, get2ndSounSoulMax);
		CLASS_DEF(Fighter, isPet);

		//????
		CLASS_ADD(TaskMgr);
		CLASS_DEF(TaskMgr, AddTask);
		CLASS_DEF(TaskMgr, TaskCanAccept);
		CLASS_DEF(TaskMgr, GetDayTaskCompletedCnt);
		CLASS_DEF(TaskMgr, GetDayTaskCurrTaskId);
		CLASS_DEF(TaskMgr, CanDayTaskSubmit);
		CLASS_DEF(TaskMgr, DayTaskSubmit);
		CLASS_DEF(TaskMgr, AutoTaskAbandon);
		CLASS_DEF(TaskMgr, GetTaskAcceptNpc);
		CLASS_DEF(TaskMgr, GetTaskSubmitNpc);
		CLASS_DEF(TaskMgr, GetTaskStep);
		CLASS_DEF(TaskMgr, HasAcceptedTask);
		CLASS_DEF(TaskMgr, HasCompletedTask);
		CLASS_DEF(TaskMgr, HasSubmitedTask);
		CLASS_DEF(TaskMgr, DelAcceptedTask);
		CLASS_DEF(TaskMgr, AddTaskStep);
		CLASS_DEF(TaskMgr, AddTaskStep2);
		CLASS_DEF(TaskMgr, AddDummyTaskStep);
		CLASS_DEF(TaskMgr, AddDummyTaskStep2);
		CLASS_DEF(TaskMgr, AcceptTask);
		CLASS_DEF(TaskMgr, SubmitTask);
		CLASS_DEF(TaskMgr, AbandonTask);
		CLASS_DEF(TaskMgr, ResetTaskStep);
		CLASS_DEF(TaskMgr, CheckPreTaskStep);
		CLASS_DEF(TaskMgr, GetTaskSubType);
		CLASS_DEF(TaskMgr, IsTaskCompleted);
		CLASS_DEF(TaskMgr, DayTaskAward);
		CLASS_DEF(TaskMgr, AddTaskStep4);
        CLASS_DEF(TaskMgr, GetTaskBeginTime);
        CLASS_DEF(TaskMgr, GetTaskEndTime);
		CLASS_DEF(TaskMgr, DelSubmitedTask);
		CLASS_DEF(TaskMgr, AddCanAcceptTask2);
		CLASS_DEF(TaskMgr, DayTaskSubmit2);
		CLASS_DEF(TaskMgr, TaskExist);
		CLASS_DEF(TaskMgr, DelCanAcceptTask2);
		CLASS_DEF(TaskMgr, SendCanAcceptTaskInfor);

        // ?ɾʹ???
		CLASS_ADD(AttainMgr);
        CLASS_DEF(AttainMgr, HasFinishAttainment);
        CLASS_DEF(AttainMgr, HasAttained);
        CLASS_DEF(AttainMgr, UpdateAttainment);
        CLASS_DEF(AttainMgr, getAttainStatus);
        CLASS_DEF(AttainMgr, MidnightCheckAttain);
        CLASS_DEF(AttainMgr, CanAttain);

		//????
        CLASS_ADD(ActivityMgr);
        //CLASS_DEF(ActivityMgr, GetFlag);
        //CLASS_DEF(ActivityMgr, UpdateFlag);
        CLASS_DEF(ActivityMgr, CheckTimeOver);
        //CLASS_DEF(ActivityMgr, AddPoint);
        CLASS_DEF(ActivityMgr, UpdateToDB);
        //CLASS_DEF(ActivityMgr, GetPoint);
        //CLASS_DEF(ActivityMgr, AddRewardFlag);
        CLASS_DEF(ActivityMgr, AddScores);

		//变强之路
        CLASS_ADD(StrengthenMgr);
        CLASS_DEF(StrengthenMgr, GetFlag);
        CLASS_DEF(StrengthenMgr, UpdateFlag);
        CLASS_DEF(StrengthenMgr, CheckTimeOver);
        CLASS_DEF(StrengthenMgr, AddSouls);
        CLASS_DEF(StrengthenMgr, UpdateToDB);
        CLASS_DEF(StrengthenMgr, GetSouls);

        //????
		CLASS_ADD(Package);
		CLASS_DEF(Package, Add);
		CLASS_DEF(Package, AddItem);
		CLASS_DEF(Package, AddEquip);
        CLASS_DEF(Package, AddEquipN);
		CLASS_DEF(Package, SetItem);
		CLASS_DEF(Package, DelItemAll);
		CLASS_DEF(Package, DelItem);
		CLASS_DEF(Package, DelItemAny);
		CLASS_DEF(Package, DelEquip);
		CLASS_DEF(Package, SellItem);
		CLASS_DEF(Package, SellEquip);
		CLASS_DEF(Package, UseItem);
		CLASS_DEF(Package, UseItemOther);
		CLASS_DEF(Package, UseTaskItem);
		CLASS_DEF(Package, GetItem);
		CLASS_DEF(Package, GetEquip);
		CLASS_DEF(Package, GetItemUsedGrids);
		CLASS_DEF(Package, ExistItem);
		CLASS_DEF(Package, GetItemNum);
		CLASS_DEF(Package, GetItemAnyNum);
		CLASS_DEF(Package, GetMaxPackageSize);
		CLASS_DEF(Package, GetUsedPackageSize);
		CLASS_DEF(Package, GetRestPackageSize);
		CLASS_DEF(Package, IsFull);
		CLASS_DEF(Package, DelItemSendMsg);
		CLASS_DEF(Package, GetItemCareer);

		//??Ʒ
		CLASS_ADD(ItemBase);
		CLASS_DEF(ItemBase, Count);

		CLASS_ADD(MailBox);
		CLASS_DEF(MailBox, newItemMail);
		CLASS_DEF(MailBox, newItemPackageMail);
	}

	const char* GameActionLua::GetItemName(UInt32 itemId)
	{
		const GData::ItemBaseType * data = GData::GDataManager::GetItemTypeData(itemId);
		return data == NULL ? "" : data->getName().c_str();
	}

	const char* GameActionLua::GetGreatFighterName(UInt32 fgtId)
	{
		GObject::Fighter& fighter = GObject::getGreatFighter(fgtId);
		return fighter.getId() == 0 ? "" : fighter.getName().c_str();
	}

	Table GameActionLua::GetTaskData(Player* player, UInt32 id)
	{
		Table task(_L);
		task.set("m_TaskId", 0);

		if (player == NULL) return task;
		const TaskData data = player->GetTaskMgr()->GetTaskData(id);

		task.set("m_TaskId", data.m_TaskId);
		task.set("m_OwnerId", data.m_OwnerId);
		task.set("m_AcceptTime", data.m_AcceptTime);
		task.set("m_Step", data.m_Step);
		task.set("m_TimeBegin", data.m_TimeBegin);
		task.set("m_TimeEnd", data.m_TimeEnd);
		task.set("m_Completed", data.m_Completed);
		task.set("m_Submit", data.m_Submit);

		return task;
	}

	UInt32 GameActionLua::GetRandLoopTask(Player * player, UInt32 dayTaskId)
	{
		return Run<UInt32>(player, "GetRandLoopTask", dayTaskId);
	}

	UInt8 GameActionLua::GetRandLoopTaskQuality()
	{
		return Run<UInt8>(NULL, "GetRandLoopTaskQuality");
	}

	UInt16 GameActionLua::GetLoopTaskMaxCount(UInt32 dayTaskId)
	{
		return Run<UInt16>(NULL, "GetLoopTaskMaxCount", dayTaskId);
	}

	UInt16 GameActionLua::GetLoopTaskMaxQualityCount(UInt32 dayTaskId)
	{
		return Run<UInt16>(NULL, "GetLoopTaskMaxQualityCount", dayTaskId);
	}

	Table GameActionLua::GetLoopTaskTasks(Player * player, UInt32 dayTaskId)
	{
		return Run<Table>(player, "GetLoopTaskTasks", dayTaskId);
	}

	UInt32 GameActionLua::GetLoopTaskIdByNpc(Player * player, UInt32 npcId)
	{
		return Run<UInt32>(player, "GetLoopTaskIdByNpc", npcId);
	}

	UInt32 GameActionLua::GetLoopTaskFlushGold(UInt32 dayTaskId)
	{
		return Run<UInt32>(NULL, "GetLoopTaskFlushGold", dayTaskId);
	}

	UInt8 GameActionLua::GetRandLoopTaskManualQuality(UInt8 color, bool useGold, UInt32 count)
	{
		return Run<UInt8>(NULL, "GetRandLoopTaskManualQuality", color, useGold, count);
	}

	Table GameActionLua::FlushLoopTask(UInt8 preColor, UInt8 color, UInt32 count, UInt32 preCount)
	{
		return Run<Table>(NULL, "FlushLoopTask", preColor, color, count, preCount);
	}

	UInt32 GameActionLua::GetAutoCompletedTask(Player * player, UInt32 dayTaskId)
	{
		return Run<UInt32>(player, "GetAutoCompletedTask", dayTaskId);
	}

	UInt32 GameActionLua::GetAutoCompletedConsume()
	{
		return Run<UInt32>(NULL, "GetAutoCompletedConsume");
	}

	bool GameActionLua::DayTaskAward(Player * player, UInt32 dayTaskId, UInt16 count, UInt8 color)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "DayTaskAward_%08d", dayTaskId);
		return Run<bool>(player, buffer, count, color);
	}

	bool GameActionLua::IsFlushQualityDayTask(UInt32 dayTaskId)
	{
		return Run<bool>(NULL, "IsFlushQualityDayTask", dayTaskId);
	}

	bool GameActionLua::IsAutoCompletedTask(UInt32 taskId)
	{
		return Run<bool>(NULL, "IsAutoTask", taskId);
	}

	Table GameActionLua::GetDayTaskCompletedCount(Player * player, UInt32 dayTaskId)
	{
		Table task(_L);

		UInt16 count = 0;
		UInt8 token = player->GetTaskMgr()->GetDayTaskCompletedCount(dayTaskId, count) ? 1 : 0;
		task.set("dayTask", token);
		task.set("count", count);

		return task;
	}

	Table GameActionLua::GetDayTaskFlushColor(Player * player, UInt32 dayTaskId)
	{
		Table task(_L);

		UInt8 color = 0;
		UInt8 token = player->GetTaskMgr()->GetDayTaskFlushColor(dayTaskId, color) ? 1 : 0;
		task.set("dayTask", token);
		task.set("color", color);

		return task;
	}

	Table GameActionLua::RunNpcDefaultAction(Player * player, UInt32 npcId)
	{
		return Run<Table>(player, "RunNpcDefaultAction", npcId);
	}

	void GameActionLua::RunItemTaskAction(Player* player, UInt32 taskId, UInt32 dummyNpcId)
	{
		MOAction::ItemTaskAction(player, taskId, dummyNpcId);
	}

	UInt32 GameActionLua::GetSharpDay(UInt32 now)
	{
		return TimeUtil::SharpDay(0, now);
	}

	bool GameActionLua::RunOperationTaskAction0(Player * player, UInt8 op)
	{
		return Run<bool>(player, "RunOperationTaskAction0", op);
	}

	bool GameActionLua::RunOperationTaskAction1(Player * player, UInt8 op, UInt32 param1)
	{
		return Run<bool>(player, "RunOperationTaskAction1", op, param1);
	}

	bool GameActionLua::RunOperationTaskAction2(Player * player, UInt8 op, UInt32 param1, UInt32 param2)
	{
		return Run<bool>(player, "RunOperationTaskAction2", op, param1, param2);
	}

	bool GameActionLua::RunOperationTaskAction3(Player * player, UInt8 op, UInt32 param1, UInt32 param2, UInt32 param3)
	{
		return Run<bool>(player, "RunOperationTaskAction3", op, param1, param2, param3);
	}


	bool GameActionLua::SetPlayerData(Player* player, UInt8 field, UInt32 value)
	{
		switch (field)
		{
		case 1:	//gold
			PLAYER_DATA(player, gold) = value;
			break;
		case 2: //coupon
			PLAYER_DATA(player, coupon) = value;
			break;
		case 3:	//tael
			PLAYER_DATA(player, tael) = value;
			break;
		case 4: //coin
			PLAYER_DATA(player, coin) = value;
			break;
		case 5: //status
			PLAYER_DATA(player, status) = value;
			break;
		case 6: //country
			PLAYER_DATA(player, country) = value;
			break;
		case 7: //location
			PLAYER_DATA(player, location) = value;
			break;
		case 8: //inCity
			PLAYER_DATA(player, inCity) = value;
			break;
		case 9:	//achievement
			PLAYER_DATA(player, achievement) = value;
			break;
		default:
			return false;
		}

		return true;
	}

	UInt32 GameActionLua::GetPlayerData(Player* player, UInt8 field)
	{
		switch (field)
		{
		case 1:	//gold
			return PLAYER_DATA(player, gold);
		case 2: //coupon
			return PLAYER_DATA(player, coupon);
		case 3:	//tael
			return PLAYER_DATA(player, tael);
		case 4: //coin
			return PLAYER_DATA(player, coin);
		case 5: //status
			return PLAYER_DATA(player, status);
		case 6: //country
			return PLAYER_DATA(player, country);
		case 7: //location
			return PLAYER_DATA(player, location);
		case 8: //inCity
			return PLAYER_DATA(player, inCity);
		case 9: //achievement
			return PLAYER_DATA(player, achievement);
		default:
			return 0;
		}
	}

	bool GameActionLua::CheckTaskAcceptCondition(Player* player, UInt32 taskId)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "Task_Can_Accept_%08d", taskId);
		return Run<bool>(player, buffer);
	}

	Table GameActionLua::RunTask(Player* player, UInt32 taskId, UInt32 npcId)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "Task_%08d", taskId);
		return Run<Table>(player, buffer, npcId);
	}

	Table GameActionLua::RunTaskStep(Player* player, UInt32 taskId, UInt8 step)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "Task_%08d_step", taskId);
		return Run<Table>(player, buffer, step);
	}

	Table GameActionLua::RunDayCopyTask(Player * player, UInt32 npcId)
	{
		return Run<Table>(player, "RunDayCopyTask", npcId);
	}

	Table GameActionLua::RunDayCopyTaskStep(Player * player, UInt32 npcId, UInt32 actionId)
	{
		return Run<Table>(player, "RunDayCopyTaskStep", npcId, actionId);
	}

	Table GameActionLua::RunActiveTask(Player * player, UInt32 npcId)
	{
		return Run<Table>(player, "RunActiveTask", player, npcId);
	}

	Table GameActionLua::RunActiveTaskStep(Player * player, UInt32 npcId, UInt32 actionId)
	{
		return Run<Table>(player, "RunActiveTaskStep", player, npcId, actionId);
	}

	Table GameActionLua::RunDayTask(Player* player, UInt32 npcId)
	{
		return Run<Table>(player, "RunDayTask", npcId);
	}

	Table GameActionLua::RunDayTaskAccept(Player* player, UInt32 taskId, UInt32 npcId)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "DayTask_%08d", taskId);
		return Run<Table>(player, buffer, npcId);
	}

    Table GameActionLua::GetNewCountryBattleBuffs()
    {
        return Run<Table>(NULL, "getNewCountryBattleBuffs");
    }

    Table GameActionLua::GetNCBEffortIdValue()
    {
        return Run<Table>(NULL, "getNCBEffortIdValue");
    }

    Table GameActionLua::GetNewCountryBattleAward(UInt8 lvl)
    {
        return Call<Table>("getNewCountryBattleAward", lvl);
    }

    Table GameActionLua::GetClanBattleBuffs()
    {
        return Run<Table>(NULL, "getClanBattleBuffs");
    }

    Table GameActionLua::GetClanBattleDailyRewards()
    {
        return Run<Table>(NULL, "getClanBattleDailyRewards");
    }

    Table GameActionLua::GetClanBattleWeekSortRewards()
    {
        return Run<Table>(NULL, "getClanBattleWeekSortRewards");
    }

    Table GameActionLua::GetClanBattleWeekScoreRewards()
    {
        return Run<Table>(NULL, "getClanBattleWeekScoreRewards");
    }

    Table GameActionLua::GetClanCopyRewards(UInt32 level)
    {
        return Run<Table>(NULL, "getClanCopyRewards", level);
    }


	bool GameActionLua::MonsterKilled(Player* player, UInt32 monsterId, UInt16 monsterNum)
	{
		return Run<bool>(player, "RunMonsterKilled", monsterId, monsterNum);
	}

	bool GameActionLua::AcceptTask(Player* player, UInt32 taskId)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "Task_%08d_accept", taskId);
		return Run<bool>(player, buffer, taskId);
	}

	bool GameActionLua::SubmitTask(Player* player, UInt32 taskId, UInt32 itemId, UInt16 itemNum)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "Task_%08d_submit", taskId);
		return Run<bool>(player, buffer, itemId, itemNum);
	}

	bool GameActionLua::AbandonTask(Player* player, UInt32 taskId)
	{
		char buffer[64];
		snprintf(buffer, sizeof(buffer), "Task_%08d_abandon", taskId);
		return Run<bool>(player, buffer);
	}

	bool GameActionLua::RunConveyAction(Player* player, UInt32 taskId)
	{
		assert(player != NULL);
		Table conveyPath = Run<Table>(player, "RunConvey", taskId);
		int size = conveyPath.size();
		if (size < 3) return false;

		UInt32 npcId = conveyPath.fetch<UInt32>(1);

		Stream st(REP::CONVEYBATTLE);
		st << npcId << taskId << static_cast<UInt16>(size-1);
		for (int i = 1; i <= (size-1)/2; i++)
		{
			st << conveyPath.fetch<UInt16>(2*i);
			st << static_cast<UInt8>(conveyPath.fetch<UInt16>(2*i+1) != 0);
		}
		st << Stream::eos;
		TaskMgr* taskMgr = player->GetTaskMgr();
		taskMgr->SetConvey();			//?û???״̬
		taskMgr->SetConveyTask(taskId);
		taskMgr->SetConveyNpc(npcId);
		player->send(&st[0], st.size());

		return true;
	}

	//?ɼ???NPC?ߵĽӿ?
	Table GameActionLua::RunCollectTask(Player* player, UInt32 npcId)
	{
		return Run<Table>(player, "RunCollectTask", npcId);
	}

	bool GameActionLua::RunCollectTaskItem(Player* player, UInt32 npcId)
	{
		return Run<bool>(player, "RunCollectTaskItem", npcId);
	}

	bool GameActionLua::RunExploreTask(Player* player, UInt32 npcId)
	{
		return Run<bool>(player, "RunExplore", npcId);
	}

	bool GameActionLua::RunAutoBattleAction(Player* player, UInt32 npcId, UInt32 turns)
	{
		return player->attackNpc(npcId, turns, true);
	}

	UInt32 GameActionLua::GetConveyMonster(Player* player, UInt32 taskId, UInt16 spot)
	{
		return Run<UInt32>(player, "GetConveyMonster", taskId, spot);
	}

	//???»????????Ľ???
	bool GameActionLua::UpdateConveyStep(Player* player, UInt32 taskId, UInt16 spot, bool succ)
	{
		TaskMgr* taskMgr = player->GetTaskMgr();
		bool ret =  Run<bool>(player, "UpdateConveyStep", taskId, spot, succ);
		if (!ret || taskMgr->HasCompletedTask(taskId))
		{
			taskMgr->SetConvey(false);
		}
		return ret;
	}

	bool GameActionLua::IsItemTaskItem(UInt32 itemId)
	{
		return Run<bool>(NULL, "IsItemTaskItem", itemId);
	}

	bool GameActionLua::IsTaskItemItem(UInt32 itemId)
	{
		return Run<bool>(NULL, "IsTaskItemItem", itemId);
	}

	bool GameActionLua::RunTaskItemUse(Player* player, UInt32 itemId)
	{
		assert(player != NULL);
		return Run<bool>(player, "RunTaskItemUse", itemId);
	}

	bool GameActionLua::RunItemTaskUse(Player* player, UInt32 itemId)
	{
		assert(player != NULL);
		return Run<bool>(player, "RunItemTaskUse", itemId);
	}

	UInt16 GameActionLua::RunItemNormalUse(Player* player, UInt32 itemId, UInt32 param, UInt16 num, bool bind)
	{
		assert(player != NULL);
        return Run<UInt32>(player, "RunItemNormalUse", itemId, num, bind, param);
	}

    UInt16 GameActionLua::getRandTrump(UInt8 lvl)
    {
        return Run<UInt32>(NULL, "getRandTrump", lvl);
    }

	UInt16 GameActionLua::RunItemNormalUseOther(Player* player, UInt32 itemId, Player* other, UInt16 num, bool bind)
	{
		assert(player != NULL);
		assert(other != NULL);
		return Run<UInt32>(player, "RunItemNormalUseOther", itemId, num, bind, other);
	}

	Table GameActionLua::GetNpcRelationTask(UInt32 npcId)
	{
		Table task(_L);
		const std::set<UInt32>& data = GData::GDataManager::GetTaskNpcRelationData(npcId);
		std::set<UInt32>::const_iterator cit = data.begin();
		for (; cit != data.end(); ++cit)
		{
			task.put(*cit);
		}
		return task;
	}

	const char* GameActionLua::GetPlayerName(Player* player)
	{
		if (player == NULL) return "";
		const std::string& name = player->getName();
        if(cfg.merged)
        {
            return player->getNameNoSuffix(name);
        }
        return name.c_str();
	}

	const char* GameActionLua::GetPlayerStateName(Player* player)
	{
		if (player == NULL) return "";
		if (player->getCountry() == 0)
		{
			return "??ü";
		}
		if (player->getCountry() == 1)
		{
			return "%??";
        }
		else{
			return "??b";
		}
	}

	void GameActionLua::DoLuaTest(Player * player, const std::string& buffer)
	{
		Player* savePlayer1 = _player1;
		_player1 = player;

		lua_tinker::dostring(_L, buffer.c_str());

		_player1 = savePlayer1;
	}

	void GameActionLua::RunAutoRegen( Player* player, Fighter* fighter )
	{
		Run<bool>(player, "DoAutoRegen", fighter);
	}

	void GameActionLua::onLogin( Player* player )
	{
		Call<void>("onLogin", player);
	}

	void GameActionLua::onDungeonWin( Player* player, UInt8 id, UInt8 count, bool free )
	{
		Call<void>("onDungeonWin", player, id, count, free);
	}

	void GameActionLua::onClanBattleAttend( Player* player )
	{
		Call<void>("onClanBattleAttend", player);
	}

	void GameActionLua::onCountryBattleAttend( Player* player )
	{
		Call<void>("onCountryBattleAttend", player);
	}

	void GameActionLua::onCountryBattleWinStreak( Player* player, UInt32 count )
	{
		Call<void>("onCountryBattleWinStreak", player, count);
	}

	void GameActionLua::onAthleticWin( Player* player )
	{
		Call<void>("onAthleticWin", player);
	}

	void GameActionLua::onDayTaskComplete( Player* player, UInt32 count )
	{
		Call<void>("onDayTaskComplete", player, count);
	}

	void GameActionLua::onExchange( Player* player )
	{
		Call<void>("onExchange", player);
	}

	bool GameActionLua::onPurchase( Player* player, UInt32 id, UInt8 count )
	{
		return Call<bool>("onPurchase", player, id, count);
	}

	void GameActionLua::onAttackBoss( Player* player )
	{
		Call<void>("onAttackBoss", player);
	}

	void GameActionLua::exchangeExtraReward( Player* player, UInt32 id )
	{
		Call<void>("exchangeExtraReward", player, id);
	}

	void GameActionLua::onLevelup( Player* player, UInt8 olev, UInt8 nlev )
	{
		Call<void>("onLevelup", player, olev, nlev);
        doAttainment(player, 10001, nlev);
	}

	void GameActionLua::onInvitedBy( Player* player )
	{
		Call<void>("onInvitedBy", player);
	}

	bool GameActionLua::onGetCFriendAward( Player* player, UInt8 idx )
	{
		return Call<bool>("onGetCFriendAward", player, idx);
	}

	UInt8 GameActionLua::onUseTickets( Player* player)
	{
		return Call<UInt8>("onUseTickets", player);
	}

	bool GameActionLua::testTakePack( Player* player, UInt8 type, UInt8 freq )
	{
		return Call<bool>("testTakePack", player, type, freq);
	}

	bool GameActionLua::testTakePackSize( Player* player, UInt8 type )
	{
		return Call<bool>("testTakePackSize", player, type);
	}

	void GameActionLua::onGetVipPack( Player* player, UInt8 type )
	{
		Call<void>("onGetVipPack", player, type);
	}

	void GameActionLua::onEnchant( Player* player, UInt8 level )
	{
		Call<void>("onEnchant", player, level);
	}

	void GameActionLua::onEnchantAct( Player* player, UInt8 level, UInt8 quality, UInt8 type )
	{
		Call<void>("onEnchantAct", player, level, quality, type);
	}

	void GameActionLua::onEnchantGt11( Player* player, UInt16 id, UInt8 level, UInt8 type)
	{
		Call<void>("onEnchantGt11", player, id, level, type);
	}

	void GameActionLua::onSoulEnchantMaxSoul( Player* player, UInt32 oms, UInt32 yams )
	{
		Call<void>("onSoulEnchantMaxSoul", player, oms, yams);
    }

	void GameActionLua::onEquipForge( Player* player, UInt32 itemId, UInt32 onums)
	{
		Call<void>("onEquipForge", player, itemId, onums);
	}

	void GameActionLua::onTrainFighterAct( Player* player, Fighter* fgt )
	{
		Call<void>("onTrainFighterAct", player, fgt);
	}

	void GameActionLua::onRechargeAct( Player* player, UInt32 total )
	{
		Call<void>("onRechargeAct", player, total);
	}

	void GameActionLua::onCopyFloorWin( Player* player, UInt8 id, UInt8 floor, UInt8 spot, UInt8 lootlvl )
	{
		Call<void>("onCopyFloorWin", player, id, floor, spot, lootlvl);
	}

	void GameActionLua::onCopyWin( Player* player, UInt8 id, UInt8 floor, UInt8 spot, UInt8 lootlvl )
	{
		Call<void>("onCopyWin", player, id, floor, spot, lootlvl);
	}

	void GameActionLua::onFrontMapFloorWin( Player* player, UInt8 id, UInt8 spot, UInt8 lootlvl )
	{
		Call<void>("onFrontMapFloorWin", player, id, spot, lootlvl);
    }

	void GameActionLua::onFrontMapWin( Player* player, UInt8 id, UInt8 spot, UInt8 lootlvl )
	{
		Call<void>("onFrontMapWin", player, id, spot, lootlvl);
    }

	bool GameActionLua::onOnlineAward(Player* player, UInt32 itemId, UInt8 count)
	{
		return Call<bool>("onOnlineAward", player, itemId, count);
	}

	bool GameActionLua::onThanksgivingDay(Player* player)
	{
		return Call<bool>("onThanksgivingDay", player);
	}

	bool GameActionLua::onBlueactiveday(Player* player)
	{
		return Call<bool>("onBlueactiveday", player);
	}

	bool GameActionLua::onChristmas(Player* player)
	{
		return Call<bool>("onChristmas", player);
	}

	bool GameActionLua::onNewYear(Player* player)
	{
		return Call<bool>("onNewYear", player);
	}

	bool GameActionLua::onValentineDay(Player* player)
	{
		return Call<bool>("onValentineDay", player);
	}

	bool GameActionLua::onFoolsDay(Player* player)
	{
		return Call<bool>("onFoolsDay", player);
	}

	bool GameActionLua::onMayDay(Player* player)
	{
		return Call<bool>("onMayDay", player);
	}

	bool GameActionLua::onMayDay1(Player* player)
	{
		return Call<bool>("onMayDay1", player);
	}

	bool GameActionLua::getHeroMemoAward(Player* player, UInt8 idx, UInt32 soul)
	{
		return Call<bool>("getHeroMemoAward", player, idx, soul);
	}

    void GameActionLua::flushHeroMemoAward(Player* player, UInt8 idx)
    {
        return Call<void>("flushHeroMemoAward", player, idx);
    }

	bool GameActionLua::getShuoShuoAward(Player* player, UInt8 idx)
	{
		return Call<bool>("getShuoShuoAward", player, idx);
	}

	bool GameActionLua::getActivateAttrResult(UInt8 lastActivateCount, UInt8 quality)
	{
		return Call<bool>("getActivateAttrResult", lastActivateCount, quality);
	}

	UInt32 GameActionLua::onAttakerAddexp(Player* attacker, Player *defender, UInt32 award)
	{
		return Call<UInt32>("onAttakerAward", attacker, defender, award);
	}

	void GameActionLua::onTopup(Player* player, UInt32 ogold, UInt32 ngold)
	{
		Call<void>("onTopup", player, ogold, ngold);
	}

	UInt32 GameActionLua::onTavernFlush( UInt8 color )
	{
		return Call<UInt32>("onTavernFlush", color);
	}

	bool GameActionLua::onTakeMailPackage( Player* player, UInt32 pkgId )
	{
		return Call<bool>("onTakeMailPackage", player, pkgId);
	}
	lua_tinker::table GameActionLua::onGetMailItems(UInt32 pkgId)
	{
		return Call<lua_tinker::table>("onGetMailItems", pkgId);
	}

    lua_tinker::table GameActionLua::GetOnlineReward(UInt8 cnt)
    {
        return Call<lua_tinker::table>("GetOnlineReward", cnt);
    }

    UInt8 GameActionLua::onGetKillMonsterReward(Player* player)
    {
        return Call<UInt8>("onGetKillMonsterReward", player);
    }

    void   GameActionLua::GetAtyReward(Player* p, UInt32 flag)
    {
        Call<void>("GetAtyReward", p , flag);
    }
	UInt32 GameActionLua::onLuckyDrawItemRoll( UInt8 type )
	{
		return Call<UInt32>("onLuckyDrawItemRoll", type);
	}

	UInt32 GameActionLua::onDungeonLootItemRoll( Player * player, UInt8 id, UInt8 level, bool isBoss )
	{
		return Call<UInt32>("onDungeonLootItemRoll", player, id, level, isBoss);
	}
	void GameActionLua::onMergeGem(Player * player, UInt8 lev, UInt8 num)
	{
		Call<void>("onMergeGem", player, lev, num);
	}
    void GameActionLua::doAttainment(Player* pl, UInt16 attainId, UInt32 param)
    {
#ifdef NO_ATTAINMENT
        return;
#endif
        Call<void>("doAttainment", pl, attainId, param);
    }
    void GameActionLua::finishAttainment(Player* pl, UInt16 attainId)
    {
        Call<void>("finishAttainment", pl, attainId);
    }

    void GameActionLua::onRecruitAward(Player* player)
    {
        Call<void>("onRecruitAward", player);
    }
    void GameActionLua::doAty( Player* pl, UInt32 id,  UInt32 param1, UInt32 param2)
    {
        Call<void>("doAty", pl, id, param1, param2);
    }
    UInt32  GameActionLua::GetAtyCheckFlag(UInt32 idx)
    {
        return Call<UInt32>("GetAtyCheckFlag", idx);
    }
    UInt32  GameActionLua::GetMaxActivity(UInt32 vip)
    {
        return Call<UInt32>(  "GetMaxActivity", vip);
    }
    void GameActionLua::onCLLoginReward(Player* pl, UInt8 cts)
    {
        return Call<void>(  "onCLLoginReward", pl, cts);
    }
    UInt16 GameActionLua::onCLLoginRewardRF(Player* pl, UInt8 cts, UInt8 type)
    {
        return Call<UInt16>(  "onCLLoginRewardRF", pl, cts, type);
    }
    void GameActionLua::onCL3DayReward(Player* pl)
    {
        return Call<void>(  "onCL3DayReward", pl);
    }
    bool GameActionLua::onRC7DayWill(Player* pl, UInt8 idx)
    {
        return Call<bool>(  "onRC7DayWill", pl, idx);
    }
    UInt32 GameActionLua::onUseMDSoul(Player* pl, UInt8 type, UInt8 v )
    {
        return Call<UInt32>(  "onUseMDSoul", pl, type, v);
    }
    bool GameActionLua::onTurnOnRC7Day(Player* pl, UInt32 total, UInt32 offset)
    {
        return Call<bool>(  "onTurnOnRC7Day", pl, total, offset);
    }
    bool GameActionLua::onTurnOnRF7Day(Player* pl, UInt32 total, UInt32 offset)
    {
        return Call<bool>(  "onTurnOnRF7Day", pl, total, offset);
    }
    lua_tinker::table GameActionLua::luckyDraw(Player* player, UInt8 id, UInt8 num, bool bind)
    {
        return Call<lua_tinker::table>("luckyDraw", player, id, num, bind);
    }
    bool GameActionLua::RunSSDTAward(Player* player, UInt8 opt)
	{
		assert(player != NULL);
		return Call<bool>("RunSSDTAward", player, opt);
	}
    bool GameActionLua::Run11ActAward(Player* player, UInt8 opt)
	{
		assert(player != NULL);
		return Call<bool>("Run11ActAward", player, opt);
	}

    UInt16 GameActionLua::RunHappyAward(Player* player, UInt8 opt)
    {
		assert(player != NULL);
		return Call<UInt16>("RunHappyAward", player, opt);
    }

    UInt16 GameActionLua::RunTargetAward(Player* player)
    {
		assert(player != NULL);
		return Call<UInt16>("RunTargetAward", player);
    }
    UInt16 GameActionLua::RunTargetAwardRF(Player* player)
    {
		assert(player != NULL);
		return Call<UInt16>("RunTargetAwardRF", player);
    }

    UInt8 GameActionLua::RunNewRegisterAward(Player* player)
    {
		assert(player != NULL);
		return Call<UInt8>("RunNewRegisterAward", player);
    }

    UInt8 GameActionLua::RunNewRegisterAwardAD_RF(Player* player, UInt8 idx)
    {
		assert(player != NULL);
		return Call<UInt8>("RunNewRegisterAwardAD_RF", player, idx);
    }

    UInt8 GameActionLua::RunBirthdayAward(Player* player)
    {
		assert(player != NULL);
		return Call<UInt8>("RunBirthdayAward", player);
    }

    bool GameActionLua::RunNewRC7DayLoginAward(Player* player, UInt8 val)
    {
        assert(player != NULL);
        return Call<bool>("RunNewRC7DayLoginAward", player, val);
    }

    bool GameActionLua::RunNewRC7DayLoginAward2(Player* player, UInt8 val)
    {
        assert(player != NULL);
        return Call<bool>("RunNewRC7DayLoginAward2", player, val);
    }

    bool GameActionLua::RunNewRC7DayRechargeAward(Player* player, UInt8 val, UInt32 totalRecharge)
    {
        assert(player != NULL);
        return Call<bool>("RunNewRC7DayRechargeAward", player, val, totalRecharge);
    }

    UInt8 GameActionLua::RunNewRC7DayTargetAward(Player* player)
    {
        assert(player != NULL);
        return Call<UInt8>("RunNewRC7DayTargetAward", player);
    }

    UInt8 GameActionLua::RunBlueDiamondAward(Player* player, UInt8 opt)
    {
		assert(player != NULL);
		return Call<UInt8>("RunBlueDiamondAward", player, opt);
    }
    UInt8 GameActionLua::RunConsumeAward(Player* player, UInt8 opt)
    {
		assert(player != NULL);
		return Call<UInt8>("RunConsumeAward", player, opt);
    }
    void GameActionLua::sendRNR(Player* player, UInt32 now, UInt32 date, UInt32 total)
    {
		return Call<void>("sendRNR", player, now, date, total);
    }
    void GameActionLua::sendRechargeMails(Player* player, UInt32 ototal, UInt32 ntotal)
    {
		return Call<void>("sendRechargeMails", player, ototal, ntotal);
    }
    void GameActionLua::sendRechargeRankAward(Player* player, Int32 pos)
    {
		return Call<void>("sendRechargeRankAward", player, pos);
    }
    void GameActionLua::sendConsumeRankAward(Player* player, Int32 pos)
    {
		return Call<void>("sendConsumeRankAward", player, pos);
    }

    UInt16 GameActionLua::doAtySignIn(Player* player)
    {
        return Call<UInt16>("doAtySignIn", player);
    }

    lua_tinker::table GameActionLua::GetExchangeProps(UInt32 id)
    {
        return Call<lua_tinker::table>("GetExchangeProps", id);
    }

    UInt32 GameActionLua::GetExchangePropsID()
    {
        return Call<UInt32>("GetExchangePropsID");
    }

    lua_tinker::table GameActionLua::GetdayExtraAward(UInt32 month, UInt32 day)
    {
        return Call<lua_tinker::table>("GetdayExtraAward", month, day);
    }

    void GameActionLua::sendConsumeMails(Player* player, UInt32 ototal, UInt32 ntotal)
    {
		return Call<void>("sendConsumeMails", player, ototal, ntotal);
    }

	bool GameActionLua::onSoSoMapAward( Player* player, UInt8 off)
	{
		return Call<bool>("onSoSoMapAward", player, off);
	}

	bool GameActionLua::onGetYearActAward( Player* player, UInt8 type)
	{
		return Call<bool>("onGetYearActAward", player, type);
	}

    UInt8 GameActionLua::onGetAthlRandomMaxValue(UInt8 diffculty)
	{
		return Call<UInt8>("onGetAthlRandomMaxValue", diffculty);
	}
	UInt32 GameActionLua::onGetAthlRandomAward(UInt8 diffculty, UInt8 opt)
	{
		return Call<UInt32>("onGetAthlRandomAward", diffculty, opt);
	}
	UInt8 GameActionLua::onGetAthlRandomDiffculty()
	{
		return Call<UInt8>("onGetAthlRandomDiffculty");
	}

    UInt8 GameActionLua::onRoamingQueqiao(Player* pl, UInt8 pos)
    {
		return Call<UInt8 >(qixiTmpl._onRoamingFunc, pl, pos);
    }
    
    UInt32 GameActionLua::GetBDSupermanPrice(Player* player, UInt32 itemId, bool isUsed)
    {
		assert(player != NULL);
		return Call<UInt32>("GetBDSupermanPrice", player, itemId, isUsed);
    }

    bool GameActionLua::onFirstRecharge(Player* player, UInt8 index)
    {
		assert(player != NULL);
		return Call<bool>("onFirstRecharge", player, index);
    }

    void GameActionLua::doStrong(Player* pl, UInt8 id, UInt32 param1, UInt32 param2)
    {
		assert(pl != NULL);
        Call<void>("doStrong", pl, id, param1, param2);
    }
    
    UInt8 GameActionLua::GetSthCheckFlag(UInt8 idx)
    {
        return Call<UInt8>("GetSthCheckFlag", idx);
    }
    
    UInt8 GameActionLua::EveryDayRoarSoul()
    {
        return Call<UInt8>("EveryDayRoarSoul");
    }
    
    lua_tinker::table GameActionLua::openGreenBoxStrong()
    {
        return Call<lua_tinker::table>("openGreenBoxStrong");
    }
    
    lua_tinker::table GameActionLua::openBlueBoxStrong()
    {
        return Call<lua_tinker::table>("openBlueBoxStrong");
    }
    
    lua_tinker::table GameActionLua::openPurpleBoxStrong()
    {
        return Call<lua_tinker::table>("openPurpleBoxStrong");
    }
    
    lua_tinker::table GameActionLua::openOrangeBoxStrong()
    {
        return Call<lua_tinker::table>("openOrangeBoxStrong");
    }
    
    UInt8 GameActionLua::RunThanksGivingDayAward(Player* player, UInt8 opt)
	{
		assert(player != NULL);
		return Call<UInt8>("RunThanksGivingDayAward", player, opt);
	}

    bool GameActionLua::onGetFeastGiftAward(Player* player, UInt8 type)
	{
		assert(player != NULL);
		return Call<bool>("onGetFeastGiftAward", player, type);
	}

    UInt32 GameActionLua::getRandomNormalMonster(UInt8 id)
    {
        return Call<UInt32>("getRandomNormalMonster", id);
    }

    UInt32 GameActionLua::getBossMonster(UInt8 id)
    {
        return Call<UInt32>("getBossMonster", id);
    }

    UInt32 GameActionLua::getLengendMonster(UInt8 id, UInt8 val)
    {
        return Call<UInt32>("getLengendMonster", id, val);
    }

    UInt32 GameActionLua::calcGridType(UInt8 prob)
    {
        return Call<UInt8>("calcGridType", prob);
    }

    lua_tinker::table GameActionLua::getTreasure(UInt8 id)
    {
        return Call<lua_tinker::table>("getTreasure", id);
    }
    
    UInt32 GameActionLua::foundCave(UInt8 id)
    {
        return Call<UInt32>("foundCave", id);
    }

    UInt32 GameActionLua::getStepAward(UInt8 progress, UInt32 step)
    {
        return Call<UInt32>("getStepAward", progress, step);
    }
    
    UInt16 GameActionLua::getSpecialItem(UInt8 id, UInt8 index)
    {
        return Call<UInt16>("getSpecialItem", id, index);
    }

    lua_tinker::table GameActionLua::getCopyFrontmapAward(UInt8 step, UInt16 localtion)
    {
        return Call<lua_tinker::table>("getCopyFrontmapAward", step, localtion);
    }

    UInt32 GameActionLua::getDreamerTreasure(UInt8 id)
    {
        return Call<UInt32>("getDreamerTreasure", id);
    }
 
    UInt32 GameActionLua::getDreamerItem(UInt8 id, UInt8 index)
    {
        return Call<UInt32>("getDreamerItem", id, index);
    }

    Table GameActionLua::getDragonKingAward(UInt8 step, UInt8 flag)
	{
		return Call<Table>("getDragonKingAward", step, flag);
	}

    bool GameActionLua::checkDragonKingCanSucceed(Player * player, UInt8 step, UInt8 flag)
	{
		assert(player != NULL);
		return Call<bool>("checkDragonKingCanSucceed", player, step, flag);
	}

    Table GameActionLua::getSaveGoldActAward(UInt32 gold)
	{
		return Call<Table>("getSaveGoldActAward", gold);
	}

    Table GameActionLua::getSaveGoldActExtraAward(UInt32 gold)
	{
		return Call<Table>("getSaveGoldActExtraAward", gold);
	}

    bool GameActionLua::onGetNewYearGiveGiftAward(Player* player, UInt8 dayOrder, UInt8 times)
    {
		assert(player != NULL);
		return Call<bool>("onGetNewYearGiveGiftAward", player, dayOrder, times);
    }

	bool GameActionLua::onGetNewYearQQGameAward( Player* player, UInt8 type)
	{
		return Call<bool>("onGetNewYearQQGameAward", player, type);
    }

	bool GameActionLua::onGetQZoneQQGameAward( Player* player, UInt8 type)
	{
		return Call<bool>("onGetQZoneQQGameAward", player, type);
    }

	bool GameActionLua::onGetNewYearQzoneContinueAward( Player* player, UInt8 type)
	{
		return Call<bool>("onGetNewYearQzoneContinueAward", player, type);
    }

	Table GameActionLua::onSeekFairypetAwardAndSucceed(UInt8 step, UInt8 isConvert)
	{
		return Call<Table>("onSeekFairypetAwardAndSucceed", step, isConvert);
    }

	UInt8 GameActionLua::getPetColorFromId(UInt32 petId)
	{
		return Call<UInt8>("getPetColorFromId", petId);
    }

	Table GameActionLua::getConvertPetValue(UInt8 color)
	{
		return Call<Table>("getConvertPetValue", color);
    }

	UInt32 GameActionLua::exchangPurplePet( Player* player )
	{
		return Call<UInt32>("exchangPurplePet", player);
    }

    bool GameActionLua::RunVipPrivilegeAward(Player* player, UInt8 idx, UInt8 dayth)
    {
		return Call<bool>("RunVipPrivilegeAward", player, idx, dayth);
    }

    bool GameActionLua::checkAnswerInFoolsDay(UInt8 qid, char answer)
    {
		return Call<bool>("checkAnswerInFoolsDay", qid, answer);
    }

    void GameActionLua::getAwardInFoolsDay(Player* player, UInt8 idx)
    {
		assert(player != NULL);
		Call<void>("getAwardInFoolsDay", player, idx);
    }

    bool GameActionLua::getLuckyStarAward(Player* player, UInt8 idx)
    {
		assert(player != NULL);
		return Call<bool>("getLuckyStarAward", player, idx);
    }

}


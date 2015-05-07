#include "Config.h"
#include "GameActionLua.h"
#include "Server/WorldServer.h"
#include "Log/Log.h"
#include "MsgID.h"
#include "Common/TimeUtil.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Network/TcpServerWrapper.h"
#include "Server/OidGenerator.h"
#include "GObject/Player.h"

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
        //lua_tinker::def(_L, "GetTaskReqStep", &TaskMgr::GetTaskReqStep);
        //lua_tinker::def(_L, "IsEquipId",	IsEquipId);
        //lua_tinker::def(_L, "IsEquipTypeId",IsEquipTypeId);
        //lua_tinker::def(_L, "IsEquip",		IsEquip);
        //lua_tinker::def(_L, "Broadcast",	SysBroadcast);
        //lua_tinker::def(_L, "SendMsg",		SysSendMsg);
        //lua_tinker::def(_L, "TaskAction",	&MOAction::TaskAction);
        //CLASS_DEF(GameActionLua, Print);

        //lua_tinker::def(_L, "getDuanWu", GObject::World::getDuanWu);
        //lua_tinker::def(_L, "getICAct", GObject::World::getICAct);
        //lua_tinker::def(_L, "setLevelAwardEnd", GObject::World::setLevelAwardEnd);
        //lua_tinker::def(_L, "getFlyRoadActivity", GObject::World::getFlyRoadActivity);  //飞升之路

        //CLASS_DEF(GameActionLua, GetItemName);
        //CLASS_DEF(GameActionLua, GetGreatFighterName);

        //CLASS_DEF(GameActionLua, GetPlayer1);
        //CLASS_DEF(GameActionLua, GetPlayer2);
        //CLASS_DEF(GameActionLua, GetNpcRelationTask);
        //CLASS_DEF(GameActionLua, GetTaskData);
        //CLASS_DEF(GameActionLua, SetPlayerData);
        //CLASS_DEF(GameActionLua, GetPlayerData);
        //CLASS_DEF(GameActionLua, RunConveyAction);
        //CLASS_DEF(GameActionLua, GetPlayerName);
        //CLASS_DEF(GameActionLua, GetPlayerStateName);
        //CLASS_DEF(GameActionLua, RunTask);
        //CLASS_DEF(GameActionLua, RunDayTaskAccept);
        //CLASS_DEF(GameActionLua, RunAutoBattleAction);
        //CLASS_DEF(GameActionLua, GetDayTaskCompletedCount);
        //CLASS_DEF(GameActionLua, GetDayTaskFlushColor);
        //CLASS_DEF(GameActionLua, RunItemTaskAction);
        //CLASS_DEF(GameActionLua, GetSharpDay);
        //CLASS_DEF(GameActionLua, GetPlayerPtr);
        CLASS_ADD(Player);
        CLASS_DEF(Player, GetVar);
        CLASS_DEF(Player, GetVarS);
        CLASS_DEF(Player, SetVar);
        CLASS_DEF(Player, AddVar);
        CLASS_DEF(Player, AddVarS);

    }
    /*
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

    */

    /*
    Table GameActionLua::GetDayTaskCompletedCount(Player * player, UInt32 dayTaskId)
    {
        Table task(_L);

        UInt16 count = 0;
        UInt8 token = player->GetTaskMgr()->GetDayTaskCompletedCount(dayTaskId, count) ? 1 : 0;
        task.set("dayTask", token);
        task.set("count", count);

        return task;
    }
    */

    /*
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
    */
    UInt32 GameActionLua::GetSharpDay(UInt32 now)
    {
        return TimeUtil::SharpDay(0, now);
    }


    UInt16 GameActionLua::getGovernDropMoney(UInt8 res)
    {
        return Run<UInt16>(NULL, "getGovenDropMoney", res);
    }


    UInt16 GameActionLua::getGovernDropItem(UInt8 res)
    {
        return Run<UInt16>(NULL, "getGovenDropItem", res);
    }

    UInt16 GameActionLua::GetGovernDropMoney(UInt8 res)
    {
        return Run<UInt16>(NULL,"GetGovernDropMoney", res);
    }


    UInt16 GameActionLua::GetGovernDropItem(UInt8 res)
    {
        return Run<UInt16>(NULL,"GetGovernDropItem", res);
    }
    
    UInt16 GameActionLua::GetRandFighter()
    {
        return Run<UInt16>(NULL, "GetRandFighter");
    }

    Table GameActionLua::GetMapInfo(UInt8 mapId)
    {
        return Run<Table>(NULL, "GetMapInfo",mapId);
    }

    Table GameActionLua::GetCampInfo(UInt8 mapId)
    {
        return Run<Table>(NULL,"GetCampInfo",mapId);
    }

    UInt8 GameActionLua::GetRide(UInt8 landform)
    {
        return Run<UInt8>(NULL,"GetRide",landform);
    }

    UInt8 GameActionLua::GetInfo(UInt8 mapId,UInt8 x , UInt8 y )  //获得地图上的地形信息
    {
        return Run<UInt8>(NULL,"GetInfo",mapId,x,y);
    }

    UInt32 GameActionLua::RandMonster(UInt8 group)
    {
        return Run<UInt32>(NULL,"RandMonster",group);
    }


    //Player * GameActionLua::GetPlayerPtr(IDTYPE playerId)
    //{
    //    return globalPlayers[playerId];
    //}
}


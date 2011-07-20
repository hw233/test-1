#include "Config.h"
#include "MOAction.h"
#include "Script/GameActionLua.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Server/WorldServer.h"
#include "Network/TcpServerWrapper.h"
#include "GData/GDataManager.h"
#include "GObject/Player.h"
#include "GObject/Fighter.h"
#include "GObject/TaskMgr.h"
#include "Common/TimeUtil.h"

namespace GObject
{
	void MOAction::Action(Player* player, UInt32 npcId, UInt32 actionType)
	{
		if (npcId == 4716)
		{
			RunDayCopyTask(player, npcId);
			return ;
		}
		if ((actionType & MOT_Task) == MOT_Task)
		{
			TaskAction(player, npcId);
			return ;
		}
		if ((actionType & MOT_Collect) == MOT_Collect)
		{
			CollectAction(player, npcId);
			return ;
		}
		if ((actionType & MOT_LoopTask) == MOT_LoopTask)
		{
			LoopTaskAction(player, npcId);
			return ;
		}
		if ((actionType & MOT_GreatFighter) == MOT_GreatFighter)
		{
			GreatFighterAction(player, npcId);
			return ;
		}
		//if ((actionType & MOT_EquipStrength) == MOT_EquipStrength ||
		//	(actionType & MOT_EquipForge) == MOT_EquipForge ||
		//	(actionType & MOT_EquipReplace) == MOT_EquipReplace)
		{
			TaskAction(player, npcId);
			return ;
		}
	}

	void MOAction::TaskAction(Player* player, UInt32 npcId)
	{
		std::size_t size = 0;

		Stream st(0x80);
		st << npcId << static_cast<UInt16>(0);
		st << static_cast<UInt16>(0);
		const std::set<UInt32>& relation = GData::GDataManager::GetTaskNpcRelationData(npcId);
		std::set<UInt32>::const_iterator cit = relation.begin();
        // XXX
		const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(*cit);
        PlayerData& pldd = player->getPlayerData();
		for (; cit != relation.end(); ++cit)
		{
            if(taskType.m_Class == 6)
            {
                if(player->getClan() == NULL || *cit != pldd.clanTaskId || pldd.ctFinishCount > CLAN_TASK_MAXCOUNT - 1)
                    continue;
            }

			Table TaskMsgTable = GameAction()->RunTask(player, *cit, npcId);
			UInt16 actionType = TaskMsgTable.get<UInt16>("m_ActionType");
			if (actionType == 0)
			{
				if (npcId == 138 || npcId == 139)
					return;
				continue;
			}
			++size;
			st << actionType << TaskMsgTable.get<UInt8>("m_ActionToken") << *cit << TaskMsgTable.get<UInt8>("m_ActionStep") << TaskMsgTable.get<const char*>("m_ActionMsg");
		}

		Table defaultMsgTable = GameAction()->RunNpcDefaultAction(player, npcId);
		int sz = defaultMsgTable.size();
		if (sz > 0)
		{
			for (int i = 1; i <= sz; i++)
			{
				Table elem = defaultMsgTable.get<Table>(i);
				st << elem.get<UInt16>("m_ActionType") << elem.get<UInt8>("m_ActionToken") << elem.get<UInt32>("m_ActionID") << elem.get<UInt8>("m_ActionStep") << elem.get<const char*>("m_ActionMsg");
				++size;
			}
		}

		st.data<UInt16>(4+4+2) = size;
		st << Stream::eos;
		player->send(st);
	}

	void MOAction::TaskActionStep(Player* player, UInt32 npcId, UInt32 taskId, UInt8 step)
	{
        // XXX
		const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(taskId);
        PlayerData& pldd = player->getPlayerData();
        if(taskType.m_Class == 6)
        {
            if(player->getClan() == NULL || taskId != pldd.clanTaskId || pldd.ctFinishCount > CLAN_TASK_MAXCOUNT - 1)
                return;
        }

		Table TaskMsgTable = GameAction()->RunTaskStep(player, taskId, step);
		UInt8 actionType = TaskMsgTable.get<UInt8>("m_ActionType");
		if (actionType != 0)
		{
			Stream st(0x81);
			st << npcId;
			st << TaskMsgTable.get<const char*>("m_NpcMsg") << actionType << TaskMsgTable.get<UInt8>("m_ActionToken");
			st << TaskMsgTable.get<const char*>("m_ActionMsg") << taskId << TaskMsgTable.get<UInt8>("m_ActionStep") << Stream::eos;
			player->send(st);
		}
	}

	void MOAction::ItemTaskAction(Player* player, UInt32 taskId, UInt32 dummyNpcId)
	{
		Table TaskMsgTable = GameAction()->RunTask(player, taskId, dummyNpcId);
		UInt16 actionType = TaskMsgTable.get<UInt16>("m_ActionType");
		if (actionType != 0)
		{
			Stream st(0x80);
			st << dummyNpcId << static_cast<UInt16>(0);
			st << static_cast<UInt16>(1);
			st << actionType << TaskMsgTable.get<UInt8>("m_ActionToken") << taskId << TaskMsgTable.get<UInt8>("m_ActionStep") << TaskMsgTable.get<const char*>("m_ActionMsg") << Stream::eos;
			player->send(st);
		}
	}

	void MOAction::CollectAction(Player* player, UInt32 npcId)
	{
		Table collect = GameAction()->RunCollectTask(player, npcId);
		Stream st(0x80);
		UInt16 actionType = collect.get<UInt16>("m_ActionType");
		st << npcId << static_cast<UInt16>(0);
		st << static_cast<UInt16>(1);
		st << actionType << (actionType == 0 ? static_cast<UInt8>(254) : collect.get<UInt8>("m_ActionToken")) << static_cast<UInt32>(0) << collect.get<UInt8>("m_ActionStep") << collect.get<const char*>("m_ActionMsg") << Stream::eos;
		player->send(st);
	}

	void MOAction::CollectActionStep(Player* player, UInt32 npcId)
	{
		bool ret = GameAction()->RunCollectTaskItem(player, npcId);
		if (ret)
		{
			Stream st(0x89);
			st << npcId << Stream::eos;
			player->send(st);
		}
	}

	void MOAction::LoopTaskAction(Player * player, UInt32 npcId)
	{
		UInt32 viplvl = player->getVipLevel();
		Table TaskMsgTable = GameAction()->RunDayTask(player, npcId);
		UInt16 actionType = TaskMsgTable.get<UInt16>("m_ActionType");
		UInt32 dayTaskId = GameAction()->GetLoopTaskIdByNpc(player, npcId);
		TaskMgr* taskMgr = player->GetTaskMgr();
		DayTaskData* data = taskMgr->GetDayTaskData(dayTaskId);
		if (data == NULL)
		{
			Stream st(0x8A);
			st << npcId << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt16>(0) << static_cast<UInt16>(0) << static_cast<UInt32>(0);
			st << static_cast<UInt16>(0) << static_cast<UInt8>(254) << static_cast<UInt32>(0) << static_cast<UInt8>(0) << "";
			st << Stream::eos;
			player->send(st);
			return ;
		} 
		UInt32 now = TimeUtil::Now();
		UInt32 leftFlushTime = 0;
		if(viplvl >= 4)
		{
			if (data->m_PreFlushTime != 0 && TimeUtil::SharpFourtyMin(0, data->m_PreFlushTime) == TimeUtil::SharpFourtyMin(0, now))
				leftFlushTime = TimeUtil::SharpFourtyMin(1, now) - now;			
		}
		else
		{
			if (data->m_PreFlushTime != 0 && TimeUtil::SharpHour(0, data->m_PreFlushTime) == TimeUtil::SharpHour(0, now))
				leftFlushTime = TimeUtil::SharpHour(1, now) - now;
		}
		Stream st(0x8A);
		st << npcId << dayTaskId;

		if(viplvl >= 1)
		{
			if(data->m_PreTaskQuality<=3)
                   data->m_PreTaskQuality=3;
        }

		st << data->m_PreTaskQuality << data->m_MaxCount << data->m_Count << data->m_MaxFlushQualityCount << leftFlushTime;
		if (actionType != 0)
		{
			st << actionType << TaskMsgTable.get<UInt8>("m_ActionToken") << TaskMsgTable.get<UInt32>("m_ActionID") << TaskMsgTable.get<UInt8>("m_ActionStep") << TaskMsgTable.get<const char*>("m_ActionMsg");
		}
		else
		{
			UInt8 actionToken = 254;
			if (taskMgr->HasAcceptedTask(data->m_PreTaskId) || taskMgr->HasCompletedTask(data->m_PreTaskId))
				actionToken = 255;
			st << static_cast<UInt16>(0) << actionToken << (data->m_AutoCompletedTaskId != 0 ? data->m_AutoCompletedTaskId : data->m_PreTaskId) << static_cast<UInt8>(0) << "";
		}
		st << Stream::eos;
		player->send(st);
	}

	void MOAction::DefaultNpcAction(Player * player, UInt32 npcId)
	{
		Table msgTable = GameAction()->RunNpcDefaultAction(player, npcId);
		UInt16 size = msgTable.size();
		if (size == 0) return ;

		Stream st(0x80);
		st << npcId << static_cast<UInt16>(0);
		st << size;
		for (UInt16 i = 1; i <= size; i++)
		{
			Table elem = msgTable.get<Table>(i);
			st << elem.get<UInt16>("m_ActionType") << elem.get<UInt8>("m_ActionToken") << elem.get<UInt32>("m_ActionID") << elem.get<UInt8>("m_ActionStep") << elem.get<const char*>("m_ActionMsg");
		}
		st << Stream::eos;
		player->send(st);
	}

	void MOAction::GreatFighterAction(Player * player, UInt32 npcId)
	{
		player->checkGreatFighterFriendliness(npcId);

		UInt16 size = 0;
		Stream st(0x80);
		st << npcId << static_cast<UInt16>(0);
		st << static_cast<UInt16>(0);
		Table msg = GameAction()->RunGreatFighterAction(player, npcId);
		for (UInt16 i = 1; i <= msg.size(); ++i)
		{
			Table elem = msg.get<Table>(i);
			UInt16 actionType = elem.get<UInt16>("m_ActionType");
			if (actionType == 0) continue;
			size++;
			st << actionType << elem.get<UInt8>("m_ActionToken") << elem.get<UInt32>("m_ActionID") << elem.get<UInt8>("m_ActionStep") << elem.get<const char*>("m_ActionMsg");
		}
		st.data<UInt16>(4+4+2) = size;
		st << Stream::eos;
		player->send(st);
	}

	void MOAction::GreatFighterActionStep(Player * player, UInt32 npcId)
	{
		Table msg = GameAction()->RunGreatCollectActionStep(player, npcId);
		UInt8 actionType = msg.get<UInt8>("m_ActionType");
		if (actionType != 0)
		{
			Stream st(0x81);
			st << static_cast<UInt32>(npcId);
			st << msg.get<const char*>("m_NpcMsg") << actionType << msg.get<UInt8>("m_ActionToken");
			st << msg.get<const char*>("m_ActionMsg") << msg.get<UInt32>("m_ActionID") << msg.get<UInt8>("m_ActionStep") << Stream::eos;
			player->send(st);
		}
	}

	void MOAction::RunGreatCtrlActionStep(Player * player, UInt32 npcId)
	{
		Table msg = GameAction()->RunGreatCtrlActionStep(player, npcId);
		UInt8 actionType = msg.get<UInt8>("m_ActionType");
		if (actionType != 0)
		{
			Stream st(0x81);
			st << static_cast<UInt32>(npcId);
			st << msg.get<const char*>("m_NpcMsg") << actionType << msg.get<UInt8>("m_ActionToken");
			st << msg.get<const char*>("m_ActionMsg") << msg.get<UInt32>("m_ActionID") << msg.get<UInt8>("m_ActionStep") << Stream::eos;
			player->send(st);
		}
	}


	void MOAction::RunDayCopyTask(Player * player, UInt32 npcId)
	{
		Table msg = GameAction()->RunDayCopyTask(player, npcId);
		UInt16 sz = static_cast<UInt16>(msg.size());
		
		Stream st(0x80);
		st << npcId << static_cast<UInt16>(0) << sz;
		for (UInt16 i = 1; i <= sz; ++i)
		{
			Table m = msg.get<Table>(i);
			st << m.get<UInt16>("m_ActionType") << m.get<UInt8>("m_ActionToken") << m.get<UInt32>("m_ActionID") << m.get<UInt8>("m_ActionStep") << m.get<const char*>("m_ActionMsg");
		}
		st << Stream::eos;
		player->send(st);
	}

	void MOAction::RunDayCopyTaskStep(Player * player, UInt32 npcId, UInt32 actionId)
	{
		Table msg = GameAction()->RunDayCopyTaskStep(player, npcId, actionId);
		UInt16 sz = static_cast<UInt16>(msg.size());

		Stream st(0x80);
		st << npcId << static_cast<UInt16>(0) << sz;
		for (UInt16 i = 1; i <= sz; ++i)
		{
			Table m = msg.get<Table>(i);
			st << m.get<UInt16>("m_ActionType") << m.get<UInt8>("m_ActionToken") << m.get<UInt32>("m_ActionID") << m.get<UInt8>("m_ActionStep") << m.get<const char*>("m_ActionMsg");
		}
		st << Stream::eos;
		player->send(st);
	}

}

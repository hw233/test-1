#include "Config.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "TaskMgr.h"
#include "EventBase.h"
#include "DB/DBExecutor.h"
#include "DB/DBConnectionMgr.h"
#include "GObjectDBExecHelper.h"
#include "Server/OidGenerator.h"
#include "Server/Cfg.h"
#include "GData/GDataManager.h"
#include "Server/OidGenerator.h"
#include "DB/DBWorker.h"
#include "Log/Log.h"
#include "Script/GameActionLua.h"
#include "Mail.h"
#include "Country.h"
#include "Fighter.h"
#include "Common/StringTokenizer.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Common/TimeUtil.h"
#include "Server/SysMsg.h"
#include "SpecialAward.h"

namespace GObject
{
	class EventAutoTask : public EventBase
	{
	public:
		EventAutoTask(Player * player, UInt32 interval, UInt32 taskId):
			EventBase(player, interval), m_TaskId(taskId)
		{}
		~EventAutoTask()
		{}

	public:
		virtual UInt32 GetID() const { return EVENT_TASKAUTOCOMPLETE; }
		void Process(UInt32)
		{
			m_Player->GetTaskMgr()->SetAutoTaskCompleted(m_TaskId);
		}
		bool Equal(UInt32 id, size_t data) const
		{
			if(!EventBase::Equal(id, data)) return false;
			return m_TaskId == data;
		}

	protected:
		UInt32		m_TaskId;
	};

	using namespace GData;

	void TaskMgr::LoadTask(TaskData * task)
	{
		SetTaskStep(task, task->m_StepStr);
		if (task->m_Completed == 0)
			m_TaskAcceptedList[task->m_TaskId] = task;
		else
			m_TaskCompletedList[task->m_TaskId] = task;
	}

	void TaskMgr::LoadSubmitedTask(UInt32 id, UInt32 tm)
	{
		m_TaskSubmitedList[id] = tm;
	}

	void TaskMgr::LoadDayTask(DayTaskData * task)
	{
		m_DayTaskList[task->m_LoopTaskId] = task;
	}

	bool TaskMgr::Init()
	{
		ConvertOldGuildTask();
		UInt32 now = TimeUtil::Now();
		UInt32 thisDay = TimeUtil::SharpDay(0, now);
		if (m_ThisDay != thisDay)
		{
			CheckDayTask(thisDay, false);
			CheckCanAcceptTaskByLev(m_PlayerOwner->GetLev(), false);
			m_ThisDay = thisDay;
		}
		DayTaskList::iterator it = m_DayTaskList.begin();
		for (; it != m_DayTaskList.end(); ++ it)
		{
			DayTaskData* data = it->second;
			if (data->m_AutoCompletedTaskId != 0)
			{
				UInt32 expire = data->m_AutoCompletedTaskAcceptTime + (data->m_MaxFlushQualityCount - data->m_Count) * 60;
				if (now >= expire)
					SetAutoTaskCompleted(data->m_AutoCompletedTaskId);
				else
				{
					EventBase* event = new(std::nothrow)EventAutoTask(m_PlayerOwner, expire - now, data->m_AutoCompletedTaskId);
					if (event == NULL) return false;
					PushTimerEvent(event);
				}
			}
		}

		return true;
	}

	void TaskMgr::UnInit()
	{
		task_iterator it;

		for (it = m_TaskAcceptedList.begin(); it != m_TaskAcceptedList.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
		m_TaskAcceptedList.clear();
		for (it = m_TaskCompletedList.begin(); it != m_TaskCompletedList.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
		m_TaskCompletedList.clear();
		m_TaskSubmitedList.clear();
	}

	
	void TaskMgr::ConvertOldGuildTask()
	{
		static const UInt64 initVal = (0x3FFFFFFFull << NEWGUILDSTEP_MAX);
		static const UInt64 fullVal = 0xFFFFFFFFFFFFFFFFull;
		static const UInt32 tasks[] = { 111111, 111112, 111113, 111114, 111115, 111116,111117,111118, 111119, 111120, 111121, 111122, 111123, 111124, 111125, 111126, 111127, 111128, 111129, 111130, 111131, 111132, 111133 };
		if (PLAYER_DATA(m_PlayerOwner, newGuild) != 0)
			return;
		if (m_PlayerOwner->GetLev() > 10)
		{
			PLAYER_DATA(m_PlayerOwner, newGuild) = fullVal;
		}
		else
		{
			PLAYER_DATA(m_PlayerOwner, newGuild) = initVal;
			for (UInt8 i = 0; i < sizeof(tasks) / sizeof(UInt32); ++ i)
			{
				if (HasSubmitedTask(tasks[i]))
					PLAYER_DATA(m_PlayerOwner, newGuild) |= (0x1ull << i);
			}
		}
		DB().PushUpdateData("UPDATE `player` SET `newGuild` = %"I64_FMT"u WHERE `id` = %"I64_FMT"u", PLAYER_DATA(m_PlayerOwner, newGuild), m_PlayerOwner->getId());
		DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` >= 111111 AND `taskId` <= 111133 AND `ownerId` = %"I64_FMT"u", m_PlayerOwner->getId());
	}


#define GET_TASK_DATA(id, td)					\
	TaskData* td = NULL;						\
	task_iterator it_;							\
	it_ = m_TaskAcceptedList.find(id);			\
	if (it_ == m_TaskAcceptedList.end())		\
	{											\
		it_ = m_TaskCompletedList.find(id);		\
		if (it_ != m_TaskCompletedList.end())	\
			td = it_->second;					\
	}											\
	else										\
		td = it_->second;


#define GET_TASK_DATA_ITER(tlist, id, it)	\
	it = tlist.find(id);

	UInt32 TaskMgr::GetTaskAcceptTime(UInt32 taskId)
	{
		GET_TASK_DATA(taskId, td);
		return td == NULL ? 0 : td->m_AcceptTime;
	}

	UInt32 TaskMgr::GetTaskStep(UInt32 taskId, UInt16 stepType)
	{
		GET_TASK_DATA(taskId, td);
		return td == NULL ? 0 : GetTaskStep2(td, stepType);
	}

	UInt32 TaskMgr::GetTaskBeginTime(UInt32 taskId)
	{
		GET_TASK_DATA(taskId, td);
		return td == NULL ? 0 : td->m_TimeBegin;
	}

	UInt32 TaskMgr::GetTaskEndTime(UInt32 taskId)
	{
		std::map<UInt32, UInt32>::iterator found = m_TaskSubmitedList.find(taskId);
		if (found != m_TaskSubmitedList.end())
		{
			return found->second;
		}
		return static_cast<UInt32>(-1);
	}

	bool TaskMgr::IsTaskCompleted(UInt32 taskId)
	{
		task_cit_iterator cit;

		GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, cit);
		return cit != m_TaskCompletedList.end();
	}

	bool TaskMgr::TaskExist(UInt32 taskId)
	{
		task_cit_iterator cit;

		if (m_TaskSubmitedList.find(taskId) != m_TaskSubmitedList.end())
			return true;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, taskId, cit);
		if (cit != m_TaskAcceptedList.end()) return true;
		GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, cit);
		if (cit != m_TaskCompletedList.end()) return true;
		return false;
	}

	bool TaskMgr::HasAcceptedTask(UInt32 taskId)
	{
		task_cit_iterator cit;

		GET_TASK_DATA_ITER(m_TaskAcceptedList, taskId, cit);
		return cit != m_TaskAcceptedList.end();
	}

	bool TaskMgr::HasCompletedTask(UInt32 taskId)
	{
		task_cit_iterator cit;

		GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, cit);
		return cit != m_TaskCompletedList.end();
	}

	bool TaskMgr::HasSubmitedTask(UInt32 taskId)
	{

		if (m_TaskSubmitedList.find(taskId) != m_TaskSubmitedList.end())
			return true;
		return false;

	}

	bool TaskMgr::DelCompletedTask(UInt32 taskId)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, it);
		if (it == m_TaskCompletedList.end()) return false;
		DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", taskId, it->second->m_OwnerId);
		SAFE_DELETE(it->second);
		m_TaskCompletedList.erase(it);
		return true;
	}

	bool TaskMgr::DelAcceptedTask(UInt32 taskId)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, taskId, it);
		if (it == m_TaskAcceptedList.end()) return false;
		DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", taskId, it->second->m_OwnerId);
		SAFE_DELETE(it->second);
		m_TaskAcceptedList.erase(it);
		return true;
	}

	bool TaskMgr::DelSubmitedTask(UInt32 taskId)
	{
		std::map<UInt32, UInt32>::iterator found = m_TaskSubmitedList.find(taskId);
		if (found == m_TaskSubmitedList.end())
			return false;
		DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", taskId, m_PlayerOwner->getId());
		m_TaskSubmitedList.erase(found);
		return true;
	}

	UInt8 TaskMgr::GetTaskSubType(UInt32 taskId)
	{
		const TaskType& taskType = GDataManager::GetTaskTypeData(taskId);
		return taskType.m_TypeId == 0 ? 0 : taskType.m_SubType;
	}

	UInt32 TaskMgr::GetTaskAcceptNpc(UInt32 taskId)
	{
		const TaskType& taskType = GDataManager::GetTaskTypeData(taskId);
		return taskType.m_TypeId == 0 ? 0 : taskType.m_AcceptNpc;
	}

	UInt32 TaskMgr::GetTaskSubmitNpc(UInt32 taskId)
	{
		const TaskType& taskType = GDataManager::GetTaskTypeData(taskId);
		return taskType.m_TypeId == 0 ? 0 : taskType.m_SubmitNpc;
	}

	UInt32 TaskMgr::AcceptTask(UInt32 taskId)
	{
        // XXX
		const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(taskId);
        if(taskType.m_Class == 6)
        {
            PlayerData& pldd = m_PlayerOwner->getPlayerData();
            if(m_PlayerOwner->getClan() == NULL || taskId != pldd.clanTaskId || pldd.ctFinishCount > CLAN_TASK_MAXCOUNT - 1)
                return 0;
        }

		TaskData* task = AddTask(taskId);
		if (task != NULL)
		{
			DelCanAcceptTask(task->m_TaskId);
			SendNewTaskInfor(*task);
			if(taskId == 10101 || taskId == 20101)
				m_PlayerOwner->setNewGuildTaskStep(8);
			return task->m_TaskId;
		}
		return 0;
	}

	bool TaskMgr::AbandonTask(UInt32 taskId)
	{
		if (DelTask(taskId))
		{
			if (TaskCanAccept(taskId))
				AddCanAcceptTask(taskId);
			return true;
		}
		return false;
	}

	bool TaskMgr::SubmitTask(UInt32 taskId)
	{
  		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, it);
		if (it == m_TaskCompletedList.end() || it->second->m_Completed != 1)
			return false;

        std::map<UInt32, UInt32>::iterator found = m_TaskSubmitedList.find(taskId);
		if (found != m_TaskSubmitedList.end())
		{
			WARN_LOG("Find the same submit task ! taskId=%u ownerId=%"I64_FMT"u", taskId, m_PlayerOwner->getId());
		}
		const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(taskId);
        if(taskType.m_Class == 4 || taskType.m_Class == 5 || taskType.m_Class == 6)
        {
            DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", taskId, m_PlayerOwner->getId());
        }
        else
        {
            m_TaskSubmitedList[taskId] = TimeUtil::Now();
            DB().PushUpdateData("UPDATE `task_instance`  SET `submit` = 1,`timeEnd` = %u WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", m_TaskSubmitedList[taskId], it->second->m_TaskId, it->second->m_OwnerId);
        }
		SAFE_DELETE(it->second);
		m_TaskCompletedList.erase(it);
		CheckCanAcceptTaskByTask(taskId);
		if(taskId < 111111 || taskId > 111200)
		{
			SYSMSG_SENDV(109, m_PlayerOwner, taskId);
			SYSMSG_SENDV(1009, m_PlayerOwner, taskId);
		}

		return true; }

	TaskData* TaskMgr::AddTask(UInt32 taskId)
	{
		const TaskType& taskType= GDataManager::GetTaskTypeData(taskId);
		if (taskType.m_TypeId == 0) return NULL;

        // XXX: 师门，衙门任务限制
        if (taskType.m_Class == 4 || taskType.m_Class == 5)
        {
            if (m_PlayerOwner->ColorTaskOutOfAccept(taskType.m_Class))
                return NULL;
            m_PlayerOwner->ColorTaskAccept(taskType.m_Class, taskId);
        }

		TaskData* task = new(std::nothrow) TaskData();
		if(task == NULL) return NULL;
		task->m_TaskId = taskId;
		task->m_Class = taskType.m_Class;
		task->m_SubType = taskType.m_SubType;
		task->m_AcceptNpc = taskType.m_AcceptNpc;
		task->m_SubmitNpc = taskType.m_SubmitNpc;
		task->m_LoopTask = taskType.m_LoopTask;
		task->m_AcceptMaxNum = taskType.m_AcceptMaxNum;
		task->m_OwnerId = m_PlayerOwner->getId();
		task->m_AcceptTime = TimeUtil::Now();
		task->m_Completed = 0;
		task->m_Submit = 0;
		InitTaskStep(task, taskType.m_ReqStep.size());
		if (taskType.m_Class != 3 && taskType.m_Class != 4 && taskType.m_Class != 5)
		{
			task->m_TimeBegin = task->m_AcceptTime;
			task->m_TimeEnd = (taskType.m_ReqTime != 0) ? (task->m_TimeBegin + taskType.m_ReqTime) : (static_cast<UInt32>(-1));
		}
		else
		{
			task->m_TimeBegin = TimeUtil::SharpDay(0);
			task->m_TimeEnd = TimeUtil::SharpDay(1) - 1;
		}
		TaskData *& td = m_TaskAcceptedList[taskId];
		if (td != NULL)
		{
			WARN_LOG("Add the same accepted task : taskId=%u, ownerId=%"I64_FMT"u", taskId, m_PlayerOwner->getId());
			SAFE_DELETE(td);
		}
		td = task;

		DB().PushUpdateData("REPLACE INTO `task_instance` VALUES (%d, %"I64_FMT"u, %d, '%s',%d,%d,%d,%d)", task->m_TaskId, task->m_OwnerId, \
		task->m_AcceptTime, task->m_StepStr.c_str(), task->m_TimeBegin, task->m_TimeEnd, task->m_Completed, task->m_Submit);
//		TRACE_LOG("player[%"I64_FMT"u:%s] accept task [%d:%d]", m_PlayerOwner->getId(), m_PlayerOwner->getName().c_str(), task->m_TaskId, task->m_Id);

		return task;
	}

	void TaskMgr::CheckCanAcceptTaskByTask(UInt32 taskId)
	{
		const TaskTypeRelation& taskTypeRelation = GDataManager::GetTaskTypeRelationData(taskId);
		bool notify = false;
		for (std::set<UInt32>::const_iterator cit = taskTypeRelation.m_PostTask.begin(); cit != taskTypeRelation.m_PostTask.end(); ++cit)
		{
			if (TaskCanAccept(*cit))
			{
				m_CanAcceptTaskList.insert(*cit);
				notify = true;
			}
		}
		if(notify)
			SendCanAcceptTaskInfor();
	}

	bool TaskMgr::TaskCanAccept(UInt32 taskId)
	{
		return GameAction()->CheckTaskAcceptCondition(m_PlayerOwner, taskId) && !isShiMenTask(taskId) && !isYaMenTask(taskId);
	}

	TaskData TaskMgr::GetTaskData(UInt32 taskId)
	{
		TaskData* data = GetTask(taskId);
		static TaskData null;
		return data != NULL ? *data : null;
	}

	bool TaskMgr::isShiMenTask(UInt32 taskid)
    {
#if 0
        const std::vector<UInt32>& ids = GData::GDataManager::GetShiMenTask(m_PlayerOwner->getCountry());
        UInt32* pid = (UInt32*)&ids[0];
        size_t size = ids.size();
        for (size_t i = 0; i < size; ++i)
        {
            if (*pid++ == taskid)
                return true;
        }
        return false;
#else
        const TaskType& taskType= GDataManager::GetTaskTypeData(taskid);
        if (taskType.m_Class == 4)
            return true;
        return false;
#endif
    }

	bool TaskMgr::isYaMenTask(UInt32 taskid)
    {
#if 0
        const std::vector<UInt32>& ids = GData::GDataManager::GetYaMenTask(m_PlayerOwner->getCountry());
        UInt32* pid = (UInt32*)&ids[0];
        size_t size = ids.size();
        for (size_t i = 0; i < size; ++i)
        {
            if (*pid++ == taskid)
                return true;
        }
        return false;
#else
        const TaskType& taskType= GDataManager::GetTaskTypeData(taskid);
        if (taskType.m_Class == 5)
            return true;
        return false;
#endif
    }

	void TaskMgr::CheckCanAcceptTaskByLev(UInt16 lev, bool notify)
	{
		m_CanAcceptTaskList.clear();
		const std::set<UInt32>& taskLevRelation = GDataManager::GetTaskLevRelation(lev);
		for (std::set<UInt32>::const_iterator cit = taskLevRelation.begin(); cit != taskLevRelation.end(); ++cit)
		{
			if (TaskCanAccept(*cit))
                m_CanAcceptTaskList.insert(*cit);

		}
		if (notify)
			SendCanAcceptTaskInfor();
	}

	void TaskMgr::SendNewTaskInfor(const TaskData& task)
	{
		Stream st(0x87);
		st << static_cast<UInt32>(0) << task.m_Class << task.m_TaskId << task.m_AcceptTime << task.m_TimeBegin << task.m_TimeEnd << Stream::eos;
		m_PlayerOwner->send(st);
	}

	bool TaskMgr::CanDayTaskSubmit(UInt32 taskId)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, it);
		if (it == m_TaskCompletedList.end() || it->second->m_Completed != 1)
			return false;
		day_task_iterator dayIt;
		GET_TASK_DATA_ITER(m_DayTaskList, it->second->m_LoopTask, dayIt);
		if (dayIt == m_DayTaskList.end())
			return false;
		if (dayIt->second == NULL)
			return false;
		if (dayIt->second->m_AutoCompletedTaskId != taskId && dayIt->second->m_PreTaskId != taskId)
			return false;
		UInt32 todayBegin = TimeUtil::SharpDay(0);
		if (it->second->m_AcceptTime < todayBegin)
		{
			DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", it->second->m_TaskId, it->second->m_OwnerId);
			SAFE_DELETE(it->second);
			m_TaskCompletedList.erase(it);
			return false;
		}
		return true;
	}


	bool TaskMgr::DayTaskSubmit2(UInt32 dayTaskId)
	{
		day_task_iterator dayIt;
		GET_TASK_DATA_ITER(m_DayTaskList, dayTaskId, dayIt);
		DayTaskData* dayTaskData = NULL;
		if (dayIt != m_DayTaskList.end())
		{
			dayTaskData = dayIt->second;
			++(dayTaskData->m_Count);
			DB().PushUpdateData("UPDATE `day_task_instance` SET `count` = %u WHERE `loopTask` = %u AND `ownerId` = %"I64_FMT"u", dayTaskData->m_Count, dayTaskData->m_LoopTaskId, m_PlayerOwner->getId());
			return true;
		}
		return false;
	}

	bool TaskMgr::DayTaskSubmit(UInt32 taskId)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, it);
		if (it == m_TaskCompletedList.end()) return false;
		day_task_iterator dayIt;
		GET_TASK_DATA_ITER(m_DayTaskList, it->second->m_LoopTask, dayIt);
		DayTaskData* dayTaskData = NULL;
		if (dayIt != m_DayTaskList.end())
		{
			dayTaskData = dayIt->second;
			if (dayTaskData->m_Count < dayTaskData->m_MaxCount)
			{
				if (taskId == dayTaskData->m_AutoCompletedTaskId)
				{
					if(World::_activityStage > 0)
						GameAction()->onDayTaskComplete(m_PlayerOwner, dayTaskData->m_MaxFlushQualityCount - dayTaskData->m_Count);
					dayTaskData->m_Count = dayTaskData->m_MaxFlushQualityCount;
					dayTaskData->m_AutoCompletedTaskId = 0;
					if (dayTaskData->m_PreTaskId == 0)
					{
						dayTaskData->m_PreTaskId = GameAction()->GetRandLoopTask(m_PlayerOwner, dayTaskData->m_LoopTaskId);
					}
				}
				else if (taskId == dayTaskData->m_PreTaskId)
				{
					if(World::_activityStage > 0)
						GameAction()->onDayTaskComplete(m_PlayerOwner, 1);
					++(dayTaskData->m_Count);
					if (dayTaskData->m_Count < dayTaskData->m_MaxCount)
					{
						dayTaskData->m_PreTaskId = GameAction()->GetRandLoopTask(m_PlayerOwner, dayTaskData->m_LoopTaskId);
					}
				}
				DB().PushUpdateData("UPDATE `day_task_instance` SET `count` = %u, `preTaskId` = %u, `autoCompletedTaskId` = 0 WHERE `loopTask` = %u AND `ownerId` = %"I64_FMT"u", dayTaskData->m_Count, dayTaskData->m_PreTaskId, dayTaskData->m_LoopTaskId, m_PlayerOwner->getId());
			}
		}

		DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", it->second->m_TaskId, m_PlayerOwner->getId());
		if(dayTaskData->m_Count == 10 && dayTaskData->m_PreTaskQuality >= 4)
			DBLOG().PushUpdateData("insert into `task_color_count`(`server_id`, `player_id`, `task_color`, `this_day`) values(%u, %"I64_FMT"u, %u, %u)", cfg.serverLogId, m_PlayerOwner->getId(), dayTaskData->m_PreTaskQuality, TimeUtil::SharpDay());
		SAFE_DELETE(it->second);
		m_TaskCompletedList.erase(it);
		DelCanAcceptTask(taskId);
		if (dayIt != m_DayTaskList.end())
		{
			if (TaskCanAccept(dayTaskData->m_PreTaskId))
				AddCanAcceptTask(dayTaskData->m_PreTaskId);
		}
		return true;
	}

	bool TaskMgr::AutoTaskAbandon(UInt32 taskId)
	{
		return DelAutoTask(taskId);
	}

	bool TaskMgr::DayTaskAward(UInt32 dayTaskId)
	{
		day_task_iterator it;
		GET_TASK_DATA_ITER(m_DayTaskList, dayTaskId, it);
		if (it != m_DayTaskList.end())
		{
			DayTaskData* dayTaskData = it->second;
			UInt16 count = dayTaskData->m_AutoCompletedTaskId != 0 ? dayTaskData->m_MaxFlushQualityCount : dayTaskData->m_Count+1;
			return GameAction()->DayTaskAward(m_PlayerOwner, dayTaskId, count, dayTaskData->m_PreTaskQuality);
		}
		return false;
	}

	TaskData* TaskMgr::GetTask(UInt32 taskId)
	{
		GET_TASK_DATA(taskId, td);
		return td;
	}

	bool TaskMgr::AddTaskStep(UInt32 taskId)
	{
		return AddTaskStep3(taskId, 1, 1, true);
	}

	bool TaskMgr::AddTaskStep2(UInt32 taskId, UInt16 stepType,  UInt32 step)
	{
		return AddTaskStep3(taskId, stepType, step, true);
	}

	bool TaskMgr::AddDummyTaskStep(UInt32 taskId)
	{
		return AddTaskStep3(taskId, 1, 1, false);
	}

	bool TaskMgr::AddDummyTaskStep2(UInt32 taskId, UInt16 stepType, UInt32 step)
	{
		return AddTaskStep3(taskId, stepType, step, false);
	}

	bool TaskMgr::AddTaskStep4(UInt32 taskId, UInt16 stepType, UInt32 step)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, taskId, it);
		if (it == m_TaskAcceptedList.end()) return false;
		const TaskType& taskType = GDataManager::GetTaskTypeData(taskId);
		if (taskType.m_TypeId == 0) return false;
		TaskData* td = it->second;
		if (stepType > td->m_Step.size())
			return false;
		if (stepType > taskType.m_ReqStep.size())
			return false;
		UInt32 oldStep = td->m_Step[stepType-1];
		if (oldStep >= step)
			return false;
		return AddTaskStep3(taskId, stepType, step-oldStep, true);
	}

	bool TaskMgr::AddTaskStep3(UInt32 taskId, UInt16 stepType, UInt32 step, bool notify)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, taskId, it);
		if (it == m_TaskAcceptedList.end()) return false;
		const TaskType& taskType = GDataManager::GetTaskTypeData(taskId);
		if (taskType.m_TypeId == 0) return false;
		TaskData* td = it->second;
		if (stepType > td->m_Step.size())
		{
			ERROR_LOG("Error Task Step !!! task[%d]id[0]stepType[%d]playerid[%llu]", taskId, stepType, m_PlayerOwner->getId());
			return false;
		}
		if (stepType > taskType.m_ReqStep.size()) return false;
		UInt32 oldStep = td->m_Step[stepType-1];
		UInt32 newStep = oldStep + step;
		td->m_Step[stepType-1] = newStep > taskType.m_ReqStep[stepType-1] ? taskType.m_ReqStep[stepType-1] : newStep;
		if (td->m_Step[stepType-1] != oldStep)
		{
			bool completedToken = false;
			if (td->m_Step == taskType.m_ReqStep)
			{
				td->m_Completed = 1;
				completedToken = true;
				m_TaskCompletedList[it->first] = it->second;
				m_TaskAcceptedList.erase(it);
			}
			if (notify)
			{
				Stream st(0x86);
				st << td->m_TaskId << static_cast<UInt8>(0) << static_cast<UInt32>(td->m_Step[stepType-1]) << static_cast<UInt32>(stepType) << Stream::eos;
				m_PlayerOwner->send(st);
			}
			if (completedToken && notify)
			{
				Stream st(0x86);
				st << td->m_TaskId << static_cast<UInt8>(1) << static_cast<UInt32>(1) << static_cast<UInt32>(0) << Stream::eos;
				m_PlayerOwner->send(st);
			}

			UpdateTaskStep(td, stepType, td->m_Step[stepType-1]);
			DB().PushUpdateData("UPDATE `task_instance` SET `step` = '%s', `completed` = %d WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", td->m_StepStr.c_str(), td->m_Completed, td->m_TaskId, td->m_OwnerId);
			return true;
		}
		return false;
	}


	void TaskMgr::SendCurrTaskInfor()
	{
		Stream st(0x82);
		UInt32 thisDay = TimeUtil::SharpDay(0);
		st << static_cast<UInt16>(m_TaskAcceptedList.size()+m_TaskCompletedList.size());
		TaskMgr::TaskList::iterator it;
		TaskData* td = NULL;
		for (it = m_TaskAcceptedList.begin(); it != m_TaskAcceptedList.end();)
		{
			td = it->second;
			if (td->m_Class == 3)
			{
				DayTaskData* dayTaskData = GetDayTaskData(td->m_LoopTask);
				if (dayTaskData != NULL && dayTaskData->m_CurrDay != thisDay)
				{
					DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", td->m_TaskId, td->m_OwnerId);
					SAFE_DELETE(td);
					m_TaskAcceptedList.erase(it++);
					continue;
				}
			}
			st << static_cast<UInt32>(0) << td->m_Class << td->m_TaskId << td->m_AcceptTime;
			st << static_cast<UInt16>(td->m_Step.size());
			for (UInt16 i = 0; i < td->m_Step.size(); ++i)
			{
				st << td->m_Step[i];
			}
			st << td->m_TimeBegin << td->m_TimeEnd << td->m_Completed << td->m_Submit;
			++it;
		}

		for (it = m_TaskCompletedList.begin(); it != m_TaskCompletedList.end();)
		{
			td = it->second;
			if (td->m_Class == 3)
			{
				DayTaskData* dayTaskData = GetDayTaskData(td->m_LoopTask);
				if (dayTaskData != NULL && dayTaskData->m_CurrDay != thisDay)
				{
					DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", td->m_TaskId, td->m_OwnerId);
					SAFE_DELETE(it->second);
					m_TaskCompletedList.erase(it++);
					continue;
				}
			}
			st << static_cast<UInt32>(0) << td->m_Class << td->m_TaskId << td->m_AcceptTime;
			st << static_cast<UInt16>(td->m_Step.size());
			for (UInt16 i = 0; i < td->m_Step.size(); ++i)
			{
				st << td->m_Step[i];
			}
			st << td->m_TimeBegin << td->m_TimeEnd << td->m_Completed << td->m_Submit;
			++it;
		}
		st << Stream::eos;
		m_PlayerOwner->send(st);
	}

	void TaskMgr::SendCanAcceptTaskInfor()
	{
		if (!m_PlayerOwner->isOnline())
			return;
		Stream st(0x83);
		st << static_cast<UInt16>(m_CanAcceptTaskList.size());
		CanAcceptTaskList::const_iterator cit = m_CanAcceptTaskList.begin();
		for (; cit != m_CanAcceptTaskList.end(); ++cit)
		{
			st << *cit;
		}
		st << Stream::eos;
		m_PlayerOwner->send(st);
	}

	void TaskMgr::DelCanAcceptTask(UInt32 taskId)
	{
		m_CanAcceptTaskList.erase(taskId);
		SendCanAcceptTaskInfor();
	}


	void TaskMgr::DelCanAcceptTask2(UInt32 taskId, bool notify)
	{
		m_CanAcceptTaskList.erase(taskId);
		if (notify)
			SendCanAcceptTaskInfor();
	}

	void TaskMgr::DelCanAcceptTask(const std::vector<UInt32>& taskIds)
	{
		UInt16 size = static_cast<UInt16>(taskIds.size());
		if (size == 0) return;
		for (UInt16 i = 0; i < size; ++i)
			m_CanAcceptTaskList.erase(taskIds[i]);
		SendCanAcceptTaskInfor();
	}

	void TaskMgr::AddCanAcceptTask(UInt32 taskId)
	{
		m_CanAcceptTaskList.insert(taskId);
		SendCanAcceptTaskInfor();
	}

	void TaskMgr::AddCanAcceptTask2(UInt32 taskId, bool notify)
	{
		m_CanAcceptTaskList.insert(taskId);
		if (notify)
			SendCanAcceptTaskInfor();
	}

	void TaskMgr::AddCanAcceptTask(const std::vector<UInt32>& taskIds)
	{
		UInt16 size = static_cast<UInt16>(taskIds.size());
		if (size == 0) return;
		for (UInt16 i = 0; i < size; ++i)
        {
			m_CanAcceptTaskList.insert(taskIds[i]);
        }
		SendCanAcceptTaskInfor();
	}

	bool TaskMgr::DoAcceptedTask(UInt32 taskId)
	{
		if (HasAcceptedTask(taskId))
			return AddTaskStep(taskId);
		return false;
	}

	bool TaskMgr::ResetTaskStep(UInt32 taskId)
	{
		if(taskId < 111111 || taskId > 111200)
		{
			SYSMSG_SENDV(108, m_PlayerOwner);
			SYSMSG_SENDV(1008, m_PlayerOwner, taskId);
		}
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, taskId, it);
		if (it != m_TaskAcceptedList.end())
		{
			ResetTaskStep2(it->second);
			return true;
		}

		GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, it);
		if (it != m_TaskCompletedList.end())
		{
			ResetTaskStep2(it->second);
			it->second->m_Completed = 0;
			m_TaskAcceptedList[it->first] = it->second;
			m_TaskCompletedList.erase(it);
			return true;
		}

		return false;
	}

	bool TaskMgr::CheckPreTaskStep(UInt32 taskId, UInt16 stepType)
	{
		const TaskType& taskType = GDataManager::GetTaskTypeData(taskId);
		if (taskType.m_TypeId == 0)
			return false;

		GET_TASK_DATA(taskId, td);
		if (td == NULL)
			return false;

		if (stepType == 1)
			return true;

		if (stepType > taskType.m_ReqStep.size())
			return false;
		assert(stepType <= td->m_Step.size());
		for (std::size_t i = 1; i < stepType; ++i)
		{
			if (td->m_Step[i-1] < taskType.m_ReqStep[i-1])
				return false;
		}

		return true;
	}

	bool TaskMgr::InitTaskStep(TaskData* task, UInt16 size)
	{
        if (!size)
            return false;
		task->m_Step.resize(size);
		task->m_StepStr.resize(size*5-1);
		memset(&task->m_StepStr[0], '0', size*5-1);
		for (std::size_t i = 0; i < static_cast<std::size_t>(size) - 1; ++ i)
			task->m_StepStr[i*5+4] = ',';
		return true;
	}

	bool TaskMgr::ResetTaskStep2(TaskData* task)
	{
		std::size_t size = task->m_Step.size();
		for (std::size_t i = 0; i < size; i++)
		{
			task->m_Step[i] = 0;
			memset(&task->m_StepStr[i*5], '0', 4);
		}
		return true;
	}

	bool TaskMgr::UpdateTaskStep(TaskData* task, UInt16 taskType, UInt16 taskStep)
	{
		if (taskType > task->m_Step.size())
			return false;
		//FIXME
		//if (task->m_StepStr.size()+1 > static_cast<size_t>(taskType*5))
		//	return false;
		task->m_Step[taskType-1] = taskStep;
		char buffer[5];
		snprintf(buffer, sizeof(buffer), "%04d", taskStep);
		memcpy(&task->m_StepStr[(taskType-1)*5], buffer, 4);
		return true;
	}

	void TaskMgr::SetTaskStep(TaskData* task, const std::string& stepStr)
	{
		StringTokenizer tokenizer(stepStr, ",");
		StringTokenizer::Iterator it;
		for (it = tokenizer.begin(); it != tokenizer.end(); ++it)
		{
			task->m_Step.push_back(atoi((*it).c_str()));
		}
		task->m_Step.resize(tokenizer.count()); //must
	}

	UInt16 TaskMgr::GetTaskStep2(const TaskData* task, UInt16 stepType)
	{
		if (stepType > task->m_Step.size())
			return 0;
		return task->m_Step[stepType-1];
	}


	UInt16 TaskMgr::GetTaskReqStep(UInt32 taskType, UInt16 stepType)
	{
		if (stepType == 0) stepType = 1;
		const TaskType& taskTypeData = GDataManager::GetTaskTypeData(taskType);
		if (taskTypeData.m_TypeId == 0)
			return static_cast<UInt16>(-1);
		if (stepType > taskTypeData.m_ReqStep.size())
			return static_cast<UInt16>(-1);
		return  taskTypeData.m_ReqStep[stepType-1];
	}

	UInt16 TaskMgr::GetDayTaskCompletedCnt(UInt32 dayTaskId)
	{
		day_task_iterator it;
		GET_TASK_DATA_ITER(m_DayTaskList, dayTaskId, it);
		if (it != m_DayTaskList.end())
			return it->second->m_Count;

		DayTaskData* dayTaskData = new(std::nothrow) DayTaskData();
		if (dayTaskData == NULL) return 0;
		dayTaskData->m_LoopTaskId = dayTaskId;
		dayTaskData->m_Count = 0;
		dayTaskData->m_MaxCount = GameAction()->GetLoopTaskMaxCount(dayTaskId);
		dayTaskData->m_MaxFlushQualityCount = GameAction()->GetLoopTaskMaxQualityCount(dayTaskId);
		dayTaskData->m_PreTaskId = GameAction()->GetRandLoopTask(m_PlayerOwner, dayTaskId);
		dayTaskData->m_PreFlushTime = 0;
		dayTaskData->m_PreTaskQuality = GameAction()->GetRandLoopTaskQuality();
		dayTaskData->m_AutoCompletedTaskId = 0;
		dayTaskData->m_AutoCompletedTaskAcceptTime = 0;
		dayTaskData->m_Owner = m_PlayerOwner->getId();
		dayTaskData->m_CurrDay = TimeUtil::SharpDay(0);
		dayTaskData->m_FlushCount = 0;
		m_DayTaskList.insert(std::make_pair(dayTaskId, dayTaskData));
		DB().PushUpdateData("REPLACE INTO `day_task_instance` (`loopTask`, `count`, `maxCount`, `maxFlushQualityCount`, `preTaskId`, `preFlushTime`, `preTaskQuality`, `ownerId`, `currDay`, `flushCount`) VALUES(%u, %u, %u, %u, %u, %u, %u, %"I64_FMT"u, %u, %u)",		\
			dayTaskData->m_LoopTaskId, dayTaskData->m_Count, dayTaskData->m_MaxCount, dayTaskData->m_MaxFlushQualityCount, dayTaskData->m_PreTaskId, dayTaskData->m_PreFlushTime, dayTaskData->m_PreTaskQuality, m_PlayerOwner->getId(), dayTaskData->m_CurrDay, dayTaskData->m_FlushCount);
		return dayTaskData->m_Count;
	}

	UInt32 TaskMgr::GetDayTaskCurrTaskId(UInt32 dayTaskId)
	{
		day_task_iterator it;
		GET_TASK_DATA_ITER(m_DayTaskList, dayTaskId, it);
		if (it != m_DayTaskList.end())
		{
			DayTaskData* dayTaskData = it->second;
			return dayTaskData->m_AutoCompletedTaskId != 0 ? dayTaskData->m_AutoCompletedTaskId : dayTaskData->m_PreTaskId;
		}
		return 0;
	}

	DayTaskData* TaskMgr::GetDayTaskData(UInt32 dayTaskId)
	{
		day_task_iterator it;
		GET_TASK_DATA_ITER(m_DayTaskList, dayTaskId, it);
		return it != m_DayTaskList.end() ? it->second : NULL;
	}

	bool TaskMgr::FlushTaskColor(UInt32 dayTaskId, UInt8 &color, UInt32 &nextFlushTime)
	{	
		UInt32 viplvl = m_PlayerOwner->getVipLevel();

		color = nextFlushTime = 0;
		DayTaskData* dayTaskData = GetDayTaskData(dayTaskId);
		if (dayTaskData == NULL) return false;
		if (dayTaskData->m_Count >= dayTaskData->m_MaxFlushQualityCount) return false;
		if (dayTaskData->m_AutoCompletedTaskId != 0) return false;
		color = dayTaskData->m_PreTaskQuality;
		if (color >= 5) return false;
		UInt32 now = TimeUtil::Now();
		bool useGold = false;
		if(viplvl >= 4)
		{
			if (dayTaskData->m_PreFlushTime != 0 && TimeUtil::SharpFourtyMin(0, dayTaskData->m_PreFlushTime) == TimeUtil::SharpFourtyMin(0, now))
			{
				UInt32 price = GameAction()->GetLoopTaskFlushGold(dayTaskId);
				if (m_PlayerOwner->getGold() < price)
					return false;
				//for old data
				useGold = true;
				ConsumeInfo ci(DayTaskFlushColor,0,0);
				m_PlayerOwner->useGold(price,&ci);
			}	
			nextFlushTime = TimeUtil::SharpFourtyMin(1, now) - now;
		}
		else
		{
			if (dayTaskData->m_PreFlushTime != 0 && TimeUtil::SharpHour(0, dayTaskData->m_PreFlushTime) == TimeUtil::SharpHour(0, now))
			{
				UInt32 price = GameAction()->GetLoopTaskFlushGold(dayTaskId);
				if (m_PlayerOwner->getGold() < price)
					return false;
				//for old data
				useGold = true;
				ConsumeInfo ci(DayTaskFlushColor,0,0);
				m_PlayerOwner->useGold(price,&ci);
			}
			nextFlushTime = TimeUtil::SharpHour(1, now) - now;
		}
		UInt8 newColor = GameAction()->GetRandLoopTaskManualQuality(color, useGold, dayTaskData->m_FlushCount);
		if(newColor == dayTaskData->m_PreTaskQuality)
			++ dayTaskData->m_FlushCount;
		else
		{
			dayTaskData->m_PreTaskQuality = newColor;
			dayTaskData->m_FlushCount = 0;
		}
		dayTaskData->m_PreFlushTime = now;
		DB().PushUpdateData("UPDATE `day_task_instance` SET `preTaskQuality` = %u, `preFlushTime` = %u, `flushCount` = %u WHERE `loopTask` = %u AND `ownerId` = %"I64_FMT"u", dayTaskData->m_PreTaskQuality, dayTaskData->m_PreFlushTime, dayTaskData->m_FlushCount, dayTaskId, m_PlayerOwner->getId());
		color = dayTaskData->m_PreTaskQuality;

		return true;
	}

	bool TaskMgr::FlushTaskColor(UInt32 dayTaskId, UInt8 taskColor, UInt32 goldTotal, UInt8 &color, UInt32 &nextFlushTime)
	{
		UInt32 viplvl = m_PlayerOwner->getVipLevel();

		color = nextFlushTime = 0;
		DayTaskData* dayTaskData = GetDayTaskData(dayTaskId);
		if (dayTaskData == NULL) return false;
		if (dayTaskData->m_Count >= dayTaskData->m_MaxFlushQualityCount) return false;
		if (dayTaskData->m_AutoCompletedTaskId != 0) return false;
		color = dayTaskData->m_PreTaskQuality;
		if (color >= taskColor) return false;
		if (m_PlayerOwner->getGold() < goldTotal)
			return false;
		UInt32 price = GameAction()->GetLoopTaskFlushGold(dayTaskId);
		int count = static_cast<int>(goldTotal / price);
		if (count <= 0) return false;
		//是否有免费刷新的机会
		UInt32 now = TimeUtil::Now();
		if(viplvl >= 4)
		{
			if (dayTaskData->m_PreFlushTime == 0 || TimeUtil::SharpFourtyMin(0, dayTaskData->m_PreFlushTime) != TimeUtil::SharpFourtyMin(0, now))
			{
				nextFlushTime = TimeUtil::SharpFourtyMin(1, now) - now;
				UInt8 newColor = GameAction()->GetRandLoopTaskManualQuality(color, false, dayTaskData->m_FlushCount);
				if(newColor != color)
				{
					dayTaskData->m_FlushCount = 0;
					color = newColor;
				}
				else
				{
					++ dayTaskData->m_FlushCount;
				}
			}		
		}
		else
		{
			if (dayTaskData->m_PreFlushTime == 0 || TimeUtil::SharpHour(0, dayTaskData->m_PreFlushTime) != TimeUtil::SharpHour(0, now))
			{
				nextFlushTime = TimeUtil::SharpHour(1, now) - now;
				UInt8 newColor = GameAction()->GetRandLoopTaskManualQuality(color, false, dayTaskData->m_FlushCount);
				if(newColor != color)
				{
					dayTaskData->m_FlushCount = 0;
					color = newColor;
				}
				else
				{
					++ dayTaskData->m_FlushCount;
				}
			}
		}
		if(color < taskColor)
		{
			Table result = GameAction()->FlushLoopTask(dayTaskData->m_PreTaskQuality, taskColor, count, dayTaskData->m_FlushCount);
			color = result.get<UInt8>("flush_color");
			UInt32 totalCount = result.get<UInt32>("total_count");
			dayTaskData->m_FlushCount = result.get<UInt32>("flush_count");

			if (totalCount > 0)
			{
				price *= totalCount;
				ConsumeInfo ci(DayTaskFlushColor,0,0);
				m_PlayerOwner->useGold(price,&ci);
			}
		}
		dayTaskData->m_PreFlushTime = now;
		dayTaskData->m_PreTaskQuality = color;
		DB().PushUpdateData("UPDATE `day_task_instance` SET `preTaskQuality` = %u, `flushCount` = %u WHERE `loopTask` = %u AND `ownerId` = %"I64_FMT"u", dayTaskData->m_PreTaskQuality, dayTaskData->m_FlushCount, dayTaskId, m_PlayerOwner->getId());
	
		return true;
	}


	bool TaskMgr::AddAutoTask(UInt32 dayTaskId)
	{
		UInt32 autoTask = GameAction()->GetAutoCompletedTask(m_PlayerOwner, dayTaskId);
		if (autoTask == 0) return false;
		UInt32 alreadyCount = GetDayTaskCompletedCnt(dayTaskId);
		DayTaskData* dayTask = GetDayTaskData(dayTaskId);
		if (dayTask == NULL) return false;
		if (dayTask->m_AutoCompletedTaskId != 0 || alreadyCount >= dayTask->m_MaxFlushQualityCount)
			return false;
		UInt32 consume = GameAction()->GetAutoCompletedConsume();
		if (PLAYER_DATA(m_PlayerOwner, gold) + PLAYER_DATA(m_PlayerOwner, coupon) < consume)
			return false;
		if (!GameAction()->AcceptTask(m_PlayerOwner, autoTask))
			return false;
		if (dayTask->m_PreTaskId != 0)
		{
			DelTask(dayTask->m_PreTaskId);
			DelCanAcceptTask(dayTask->m_PreTaskId);
		}
		UInt16 coupon = m_PlayerOwner->getCoupon();
		ConsumeInfo ci(AutoProcessTask,0,0);
		if (coupon >= consume)
			m_PlayerOwner->useCoupon(consume,&ci);
		else
		{
			m_PlayerOwner->useCoupon(coupon,&ci);
            m_PlayerOwner->useGold(consume - coupon,&ci);
		}
		dayTask->m_AutoCompletedTaskId = autoTask;
		dayTask->m_AutoCompletedTaskAcceptTime = TimeUtil::Now();
		DB().PushUpdateData("UPDATE `day_task_instance` SET `autoCompletedTaskId` = %u,`autoCompletedTaskAcceptTime` = %u WHERE `loopTask` = %u AND `ownerId` = %"I64_FMT"u", autoTask, dayTask->m_AutoCompletedTaskAcceptTime, dayTaskId, m_PlayerOwner->getId());
		{
			//Add AutoTask completed check
			UInt32 expire = (dayTask->m_MaxFlushQualityCount - dayTask->m_Count) * 60;
			
			UInt32 viplvl = m_PlayerOwner->getVipLevel();
            if(viplvl >= 5) 
				expire=0;
			else if(viplvl == 4)
                expire=(dayTask->m_MaxFlushQualityCount - dayTask->m_Count) * 30;

			EventBase* event = new(std::nothrow)EventAutoTask(m_PlayerOwner, expire, autoTask);
			if (event == NULL) return false;
			PushTimerEvent(event);
		}
		return true;
	}


	bool TaskMgr::DelAutoTask(UInt32 autoTask)
	{
		TaskData* task = NULL;
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, autoTask, it);
		if (it != m_TaskAcceptedList.end())
		{
			task = it->second;
			m_TaskAcceptedList.erase(it);
		}
		else
		{
			GET_TASK_DATA_ITER(m_TaskCompletedList, autoTask, it);
			if (it != m_TaskCompletedList.end())
			{
				task = it->second;
				m_TaskCompletedList.erase(it);
			}
		}
		if (task == NULL) return false;
		DayTaskData* dayTask = GetDayTaskData(task->m_LoopTask);
		if (dayTask == NULL) return false;
		dayTask->m_AutoCompletedTaskId = 0;
		if (TaskCanAccept(dayTask->m_PreTaskId))
			AddCanAcceptTask(dayTask->m_PreTaskId);
		DB().PushUpdateData("UPDATE `day_task_instance` SET `autoCompletedTaskId` = 0 WHERE `loopTask` = %u AND `ownerId` = %"I64_FMT"u", dayTask->m_LoopTaskId, m_PlayerOwner->getId());
		DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", task->m_TaskId, task->m_OwnerId);
		SAFE_DELETE(task);
		m_PlayerOwner->getCoupon(GameAction()->GetAutoCompletedConsume());
		{
			PopTimerEvent(m_PlayerOwner, EVENT_TASKAUTOCOMPLETE, autoTask);
		}
		return true;
	}

	bool TaskMgr::DelTask(UInt32 taskId)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, taskId, it);
		if (it != m_TaskAcceptedList.end())
		{
			DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", it->second->m_TaskId, it->second->m_OwnerId);
			SAFE_DELETE(it->second);
			m_TaskAcceptedList.erase(it);
			return true;
		}
		else
		{
			GET_TASK_DATA_ITER(m_TaskCompletedList, taskId, it);
			if (it != m_TaskCompletedList.end())
			{
				DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", it->second->m_TaskId, it->second->m_OwnerId);
				SAFE_DELETE(it->second);
				m_TaskCompletedList.erase(it);
				return true;
			}
		}
		return false;
	}

	bool TaskMgr::SetAutoTaskCompleted(UInt32 autoTask)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, autoTask, it);
		if (it == m_TaskAcceptedList.end()) return false;
		DayTaskData* dayTask = GetDayTaskData(it->second->m_LoopTask);
		if (dayTask == NULL) return false;
		if (autoTask != dayTask->m_AutoCompletedTaskId) return false;
		if (dayTask->m_Count >= dayTask->m_MaxFlushQualityCount) return false;
		it->second->m_Completed = 1;
		DB().PushUpdateData("UPDATE `task_instance` SET `completed` = %u WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", 1, it->second->m_TaskId, it->second->m_OwnerId);
		m_TaskCompletedList[it->first] = it->second;
		m_TaskAcceptedList.erase(it);


		Stream st(0x86);
		st << autoTask << static_cast<UInt8>(1) << static_cast<UInt32>(1) << static_cast<UInt32>(0) << Stream::eos;
		m_PlayerOwner->send(st);

		struct AutoDayTaskName
		{
			UInt32	m_TaskId;
			UInt32	m_TaskNameId;
		};

		static const AutoDayTaskName names[] =
		{
			{70100, 210},
			{70200, 210},
			{70300, 210},
			{70400, 211}
		};
		UInt16 i = 0;
		for (i = 0; i < sizeof(names)/sizeof(AutoDayTaskName); ++i)
		{
			if (names[i].m_TaskId == autoTask)
				break;
		}
		if (i >= sizeof(names)/sizeof(AutoDayTaskName))
			return true;
		SYSMSG(title, names[i].m_TaskNameId);
		SYSMSGV(content, 204, title);
		m_PlayerOwner->GetMailBox()->newMail(NULL, 0x01, title, content);

		return true;
	}

	UInt32 TaskMgr::GetAutoTaskLeftTime(UInt32 taskId)
	{
		task_iterator it;
		GET_TASK_DATA_ITER(m_TaskAcceptedList, taskId, it);
		if (it == m_TaskAcceptedList.end()) return 0;
		UInt32 dayTaskId = it->second->m_LoopTask;
		day_task_iterator dayIt;
		GET_TASK_DATA_ITER(m_DayTaskList, dayTaskId, dayIt);
		if (dayIt != m_DayTaskList.end())
		{
			DayTaskData* dayTaskData = dayIt->second;
			if (dayTaskData->m_AutoCompletedTaskId != taskId) return 0;
			if (dayTaskData->m_Count >= dayTaskData->m_MaxFlushQualityCount) return 0;
			UInt32 completedTime = (dayTaskData->m_MaxFlushQualityCount - dayTaskData->m_Count) * 60 + dayTaskData->m_AutoCompletedTaskAcceptTime;
			
			UInt32 viplvl = m_PlayerOwner->getVipLevel();
			if(viplvl >= 5)
				completedTime = dayTaskData->m_AutoCompletedTaskAcceptTime;
			else if(viplvl == 4)
                    completedTime = (dayTaskData->m_MaxFlushQualityCount - dayTaskData->m_Count) * 30 + dayTaskData->m_AutoCompletedTaskAcceptTime;

			UInt32 now = TimeUtil::Now();
			if (now >= completedTime ) return 0;
			return completedTime - now;
		}
		return 0;
	}

	bool TaskMgr::GetDayTaskCompletedCount(UInt32 dayTaskId, UInt16& count)
	{
		DayTaskData* dayTaskData = GetDayTaskData(dayTaskId);
		if (dayTaskData == NULL) return false;
		count = dayTaskData->m_Count;
		return true;
	}

	bool TaskMgr::GetDayTaskFlushColor(UInt32 dayTaskId, UInt8& color)
	{
		DayTaskData* dayTaskData = GetDayTaskData(dayTaskId);
		if (dayTaskData != NULL && dayTaskData->m_Count <= dayTaskData->m_MaxCount)
		{
			color = dayTaskData->m_PreTaskQuality;
			return true;
		}
		return false;
	}

	bool TaskMgr::DelDayTask(UInt32 dayTaskId, bool notify)
	{
		task_iterator it;

		for (it = m_TaskAcceptedList.begin(); it != m_TaskAcceptedList.end();)
		{
			if (it->second->m_LoopTask == dayTaskId)
			{
				if (m_PlayerOwner->isOnline() && notify)
				{
					Stream st(0x85);
					st << it->second->m_TaskId << static_cast<UInt8>(2) << Stream::eos;
					m_PlayerOwner->send(st);
				}
				DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", it->second->m_TaskId, it->second->m_OwnerId);
				SAFE_DELETE(it->second);
				m_TaskAcceptedList.erase(it++);
			}
			else
				++it;
		}
		for (it = m_TaskCompletedList.begin(); it != m_TaskCompletedList.end();)
		{
			if (it->second->m_LoopTask == dayTaskId)
			{
				if (m_PlayerOwner->isOnline() && notify)
				{
					Stream st(0x85);
					st << it->second->m_TaskId << static_cast<UInt8>(2) << Stream::eos;
					m_PlayerOwner->send(st);
				}
				DB().PushUpdateData("DELETE FROM `task_instance` WHERE `taskId` = %d AND `ownerId` = %"I64_FMT"u", it->second->m_TaskId, it->second->m_OwnerId);
				SAFE_DELETE(it->second);
				m_TaskCompletedList.erase(it++);
			}
			else
				++it;
		}
		return true;
	}


	void TaskMgr::CheckDayTask(UInt32 curr, bool notify)
	{
		day_task_iterator it;
		for (it = m_DayTaskList.begin(); it != m_DayTaskList.end();)
		{
			if (it->second->m_CurrDay != curr)
			{
				DelDayTask (it->second->m_LoopTaskId, notify);
				DB().PushUpdateData("DELETE FROM `day_task_instance` WHERE `ownerId` = %"I64_FMT"u AND `loopTask` = %u", m_PlayerOwner->getId(), it->second->m_LoopTaskId);
				SAFE_DELETE(it->second);
				m_DayTaskList.erase(it++);
			}
			else
				++it;
		}
		if(notify)
			CheckCanAcceptTaskByLev(m_PlayerOwner->GetLev(), true);
	}
}

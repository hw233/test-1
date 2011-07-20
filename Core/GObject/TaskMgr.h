#ifndef _TASKMGR_INC_
#define _TASKMGR_INC_

#include <list>
#include <algorithm>
#include "TaskData.h"

namespace GObject
{
	class Player;
}

namespace GObject
{
	struct TaskComp
	{
		TaskComp(UInt32 taskId = 0 ) : m_TaskId(taskId) {};
		bool operator()(const TaskData* task) const
		{
			assert(task != NULL);
			//if (task == NULL) return false;
			return task->m_TaskId == m_TaskId;
		}

		UInt32 m_TaskId;
	};


	class TaskMgr
	{
		typedef std::map<UInt32, TaskData*> TaskList;
		typedef TaskList::const_iterator task_cit_iterator;
		typedef TaskList::iterator task_iterator;

		typedef std::map<UInt32, DayTaskData*> DayTaskList;
		typedef DayTaskList::const_iterator day_task_cit_iterator;
		typedef DayTaskList::iterator day_task_iterator;

	public:
		TaskMgr(Player* player) : m_ThisDay(0), m_PlayerOwner(player), m_IsConvey(false), m_ConveyTask(0), m_ConveyNpc(0) {};
		~TaskMgr() {};

	public:
		void LoadTask(TaskData *);
		void LoadSubmitedTask(UInt32, UInt32);
		void LoadDayTask(DayTaskData *);

	public:
		bool Init();
		void UnInit();

		void ConvertOldGuildTask();

	public:
		UInt32 GetTaskAcceptTime(UInt32 taskId);
		UInt32 GetTaskStep(UInt32 taskId, UInt16 stepType);
		UInt32 GetTaskBeginTime(UInt32 taskId);
		UInt32 GetTaskEndTime(UInt32 taskId);
		UInt8  GetTaskSubType(UInt32 taskId);
		UInt32 GetTaskAcceptNpc(UInt32 taskId);
		UInt32 GetTaskSubmitNpc(UInt32 taskId);
		

		bool   HasAcceptedTask(UInt32 taskId);
		bool   HasCompletedTask(UInt32 taskId);
		bool   HasSubmitedTask(UInt32 taskId);
		bool   DelCompletedTask(UInt32 taskId);
		bool   DelAcceptedTask(UInt32 taskId);
		bool   DelSubmitedTask(UInt32 taskId);

		bool IsTaskCompleted(UInt32 taskId);
		bool TaskExist(UInt32 taskId);

	public:
		UInt32 AcceptTask(UInt32 taskId);	
		bool   AbandonTask(UInt32 taskId);
		bool   SubmitTask(UInt32 taskId);

		TaskData* AddTask(UInt32 taskId);
		bool DelTask(UInt32 taskId);
		void DelCanAcceptTask(UInt32);
		void DelCanAcceptTask2(UInt32, bool = true);
		void DelCanAcceptTask(const std::vector<UInt32>&);
		void AddCanAcceptTask(UInt32);	//for script
		void AddCanAcceptTask2(UInt32, bool = true);
		void AddCanAcceptTask(const std::vector<UInt32>&);

		bool DoAcceptedTask(UInt32);

		bool ResetTaskStep(UInt32 taskId);
		bool CheckPreTaskStep(UInt32 taskId, UInt16 stepType);

		TaskData* GetTask(UInt32 taskId);
		TaskData  GetTaskData(UInt32 taskId);

        bool isShiMenTask(UInt32 taskid);
        bool isYaMenTask(UInt32 taskid);
		void CheckCanAcceptTaskByTask(UInt32 taskId);
		void CheckCanAcceptTaskByLev(UInt16 lev, bool = true);
		bool TaskCanAccept(UInt32 taskId);

		bool AddTaskStep(UInt32 taskId);
		bool AddTaskStep2(UInt32 taskId, UInt16 stepType,  UInt32 step);
		bool AddDummyTaskStep(UInt32 taskId);
		bool AddDummyTaskStep2(UInt32 taskId, UInt16 stepType, UInt32 step);
		bool AddTaskStep3(UInt32 taskId, UInt16 stepType, UInt32 step, bool notify);
		bool AddTaskStep4(UInt32 taskId, UInt16 stepType, UInt32 step);

		static UInt16 GetTaskReqStep(UInt32 taskId, UInt16 stepType = 1);

	public:
		UInt16 GetDayTaskCompletedCnt(UInt32 dayTaskId);
		UInt32 GetDayTaskCurrTaskId(UInt32 dayTaskId);
		DayTaskData* GetDayTaskData(UInt32 dayTaskId);
		bool FlushTaskColor(UInt32, UInt8 &, UInt32 &);
		bool FlushTaskColor(UInt32, UInt8, UInt32, UInt8 &, UInt32 &);
		bool AddAutoTask(UInt32 dayTaskId);
		bool DelAutoTask(UInt32 dayTaskId);
		bool SetAutoTaskCompleted(UInt32 dayTaskId);
		UInt32 GetAutoTaskLeftTime(UInt32 taskId);
		bool   GetDayTaskCompletedCount(UInt32 dayTaskId, UInt16& count);
		bool   GetDayTaskFlushColor(UInt32 dayTaskId, UInt8& color);
		bool CanDayTaskSubmit(UInt32 taskId);
		bool DayTaskSubmit(UInt32 taskId);
		bool DayTaskSubmit2(UInt32 taskId); //FIXME ALL--------
		bool AutoTaskAbandon(UInt32 taskId);
		bool DayTaskAward(UInt32 dayTaskId);
		bool DelDayTask(UInt32, bool);
		void CheckDayTask(UInt32, bool = true);

	public:
		void SendNewTaskInfor(const TaskData& task);
		void SendCurrTaskInfor();
		void SendCanAcceptTaskInfor();

	protected:
		bool InitTaskStep(TaskData*, UInt16);
		bool ResetTaskStep2(TaskData*);
		bool UpdateTaskStep(TaskData*, UInt16, UInt16);
		void SetTaskStep(TaskData* task, const std::string& stepStr);
		UInt16 GetTaskStep2(const TaskData* task, UInt16 stepType);

	public:
		bool IsInConvey() const				{ return m_IsConvey;	}
		void SetConvey(bool token = true)	{ m_IsConvey = token;	}
		void SetConveyTask(UInt32 taskId)   { m_ConveyTask = taskId;}
		void SetConveyNpc(UInt32 npcId)		{ m_ConveyNpc = npcId;	}
		UInt32 GetConveyTask() const		{ return m_ConveyTask;	}
		UInt32 GetConveyNpc() const			{ return m_ConveyNpc;	}

	private:
		UInt32 m_ThisDay;

		Player* m_PlayerOwner;

		TaskList m_TaskAcceptedList;		
		TaskList m_TaskCompletedList;
        std::map<UInt32, UInt32> m_TaskSubmitedList; //FIXME

		typedef std::set<UInt32> CanAcceptTaskList;
		CanAcceptTaskList m_CanAcceptTaskList;	

		DayTaskList m_DayTaskList;			

		bool m_IsConvey;	
		UInt32 m_ConveyTask;
		UInt32 m_ConveyNpc;
	};
}

#endif

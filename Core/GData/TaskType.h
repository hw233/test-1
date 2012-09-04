#ifndef _TASKTYPE_INC_
#define _TASKTYPE_INC_

#include <string>
#include <vector>
#include <map>
#include <set>

namespace GData
{
	struct TaskType
	{
		UInt32				m_TypeId;
		std::string			m_Name;
		UInt8				m_Class;		//1:主线 2：支线 3：日常（循环） 4: 师门 5:衙门 6：帮派
		UInt8				m_SubType;
		UInt32				m_AcceptNpc;	//任务接受NPC；如果为0， 则表示此任务是通过物品触发的
		UInt32				m_SubmitNpc;
		UInt32				m_ReqTime;
		UInt16				m_AcceptMaxNum;	//循环任务
		std::vector<UInt16>	m_ReqStep;
		std::set<UInt32>	m_PreTask;
		UInt16				m_ReqLev;
		UInt16				m_LoopTask;
        UInt8               m_Country;

		TaskType(UInt32 id = 0) : m_TypeId(id) {};
		bool operator<(const TaskType& other) const { return m_TypeId < other.m_TypeId; }
		bool operator==(const TaskType& other) const { return m_TypeId == other.m_TypeId; }
	};
	typedef std::map<UInt32, TaskType> TaskTypeList;

	//任务关系的依赖链表
	struct TaskTypeRelation
	{
		std::set<UInt32>	m_PreTask;	//前序依赖任务
		std::set<UInt32>	m_PostTask;	//后序关联任务
	};
	typedef std::map<UInt32, TaskTypeRelation> TaskTypeRelationList;

	//NPC关联任务表
	typedef std::map<UInt32, std::set<UInt32> > TaskNpcRelationList;
	typedef std::map<UInt32, std::set<UInt32> > DayTaskNpcRelationList;

	//等级关联任务表
	typedef std::map<UInt16, std::set<UInt32> > TaskLevRelationList;

	//每日任务表
	typedef std::map<UInt32, std::set<UInt32> > TaskDayList;
}


#endif

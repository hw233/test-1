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
		UInt8				m_Class;		//1:���� 2��֧�� 3���ճ���ѭ���� 4: ʦ�� 5:���� 6������
		UInt8				m_SubType;
		UInt32				m_AcceptNpc;	//�������NPC�����Ϊ0�� ���ʾ��������ͨ����Ʒ������
		UInt32				m_SubmitNpc;
		UInt32				m_ReqTime;
		UInt16				m_AcceptMaxNum;	//ѭ������
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

	//�����ϵ����������
	struct TaskTypeRelation
	{
		std::set<UInt32>	m_PreTask;	//ǰ����������
		std::set<UInt32>	m_PostTask;	//�����������
	};
	typedef std::map<UInt32, TaskTypeRelation> TaskTypeRelationList;

	//NPC���������	
	typedef std::map<UInt32, std::set<UInt32> > TaskNpcRelationList;
	typedef std::map<UInt32, std::set<UInt32> > DayTaskNpcRelationList;

	//�ȼ����������
	typedef std::map<UInt16, std::set<UInt32> > TaskLevRelationList;

	//ÿ�������
	typedef std::map<UInt32, std::set<UInt32> > TaskDayList;
}


#endif

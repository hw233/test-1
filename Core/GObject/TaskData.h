#ifndef _TASKDATA_INC_
#define _TASKDATA_INC_

#include <string>
#include <vector>

namespace GObject
{
	//TODO: ���һ�ֶΣ� ���ڱ�ʾ������ʱ����ʲô���� �����ύ����ʱʹ��
	struct TaskData
	{
		UInt32				m_TaskId;
		UInt8				m_Class;
		UInt8				m_SubType;
		UInt32				m_AcceptNpc;
		UInt32				m_SubmitNpc;
		UInt16				m_LoopTask;			//valid if m_Class == 3
		UInt16				m_AcceptMaxNum;	

		UInt64				m_OwnerId;
		UInt32				m_AcceptTime;
		std::vector<UInt16>	m_Step;				//��ǰ���������Ϣ0000,0000,0000,0000,...
		UInt32				m_TimeBegin;		//�������Ч����ʱ��
		UInt32				m_TimeEnd;			//�������Ч����ʱ��
		UInt8				m_Completed;		//�����Ƿ������
		UInt8				m_Submit;			//�����Ƿ����ύ

		std::string			m_StepStr;			//Only for DB

		bool operator<(const TaskData& other) const { return m_TaskId < other.m_TaskId; }
		bool operator==(const TaskData& other) const { return m_TaskId == other.m_TaskId; }
	};

	struct DayTaskData
	{
		UInt32 m_LoopTaskId;
		UInt16 m_Count;
		UInt16 m_MaxCount;
		UInt16 m_MaxFlushQualityCount;
		UInt32 m_PreTaskId;
		UInt32 m_PreFlushTime;
		UInt8  m_PreTaskQuality;
		UInt32 m_AutoCompletedTaskId;
		UInt32 m_AutoCompletedTaskAcceptTime;
		UInt64 m_Owner;
		UInt32 m_CurrDay;
		UInt16 m_FlushCount;

		DayTaskData(UInt32 id = 0) : m_LoopTaskId(id)
		{
			m_Count = 0;
			m_MaxCount = 0;
			m_MaxFlushQualityCount = 0;
			m_PreTaskId = 0;
			m_PreFlushTime = 0;
			m_PreTaskQuality = 0;
			m_AutoCompletedTaskId = 0;
			m_AutoCompletedTaskAcceptTime = 0;
			m_Owner = 0;
			m_CurrDay = 0;
			m_FlushCount = 0;
		}

		bool operator<(const DayTaskData& other) const { return m_LoopTaskId < other.m_LoopTaskId; }
		bool operator==(const DayTaskData& other) const { return m_LoopTaskId == other.m_LoopTaskId; }
	};
	
}

#endif

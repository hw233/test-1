#ifndef _COUNTRYMSGSTRUCT_H_
#define _COUNTRYMSGSTRUCT_H_

#include "Common/Serialize.h"
#include "MsgID.h"

struct CountryEnterStruct
{
	CountryEnterStruct(bool nt, UInt8 ic, UInt16 nl):
		notify(nt), inCity(ic), newLoc(nl) { }
	bool notify;
	UInt8 inCity;
	UInt16 newLoc;
};

struct NpcActionResp
{
	UInt32		m_NpcId;
	std::string m_NpcMsg;
	UInt8		m_ActionType;
	UInt8		m_ActionToken;
	std::string m_ActionMsg;
	UInt32		m_ActionID;
	UInt8		m_ActionStep;

	NpcActionResp() : m_NpcId(0), m_ActionType(0), m_ActionToken(0), m_ActionID(0), m_ActionStep(0) {};

	MESSAGE_DEF7(REP::DIALOG_INTERACTION, UInt32, m_NpcId, std::string, m_NpcMsg, UInt8, m_ActionType, UInt8, m_ActionToken, std::string, m_ActionMsg, UInt32, m_ActionID, UInt8, m_ActionStep);
};


struct TaskActionResp
{
	UInt32	m_TaskId;	//
	UInt8	m_Action;	//0：接受 1：提交 2：放弃

	TaskActionResp() : m_TaskId(0), m_Action(0) {};

	MESSAGE_DEF2(REP::PLAYER_ABANDON_TASK, UInt32, m_TaskId, UInt8, m_Action);
};


struct ActionTable
{
	UInt16		m_ActionType;
	UInt8		m_ActionToken;
	UInt32		m_ActionID;
	UInt8		m_ActionStep;	//交互的步骤
	std::string m_ActionMsg;
	ActionTable() : m_ActionType(0), m_ActionToken(0), m_ActionID(0), m_ActionStep(0) {};
};
S11N_TRAITS_5(ActionTable, UInt16, m_ActionType, UInt8, m_ActionToken, UInt32, m_ActionID, UInt8, m_ActionStep, std::string, m_ActionMsg);

struct NpcTriggerResp
{
	UInt32						m_NpcId;
	UInt16						m_ActionType;
	std::vector<ActionTable>	m_ActionCont;
	NpcTriggerResp() : m_NpcId(0) {};

	MESSAGE_DEF3(REP::DIALOG_START, UInt32, m_NpcId, UInt16, m_ActionType, std::vector<ActionTable>, m_ActionCont);
};

struct AcceptTaskResp
{
	UInt32		m_Id;
	UInt8		m_Class;	//任务类型：1主线任务、2支线任务、3每日任务
	UInt32		m_TaskId;
	UInt32		m_AcceptTime;
	UInt32		m_TimeBegin;
	UInt32		m_TimeEnd;

	MESSAGE_DEF6(REP::NEW_TASK, UInt32, m_Id, UInt8, m_Class, UInt32, m_TaskId, UInt32, m_AcceptTime, UInt32, m_TimeBegin, UInt32, m_TimeEnd);
};

struct TaskInfor
{
	UInt32				m_Id;
	UInt8				m_Class;	//任务类型：1主线任务、2支线任务、3每日任务
	UInt32				m_TaskId;
	UInt32				m_AcceptTime;
	std::vector<UInt16>	m_Step;
	UInt32				m_TimeBegin;
	UInt32				m_TimeEnd;
	UInt8				m_Completed;
	UInt8				m_Submit;	//不用给CLIENT
	TaskInfor() : m_Id(0), m_Class(0), m_TaskId(0), m_AcceptTime(0), m_Step(0), m_TimeBegin(0), m_TimeEnd(0), m_Completed(0), m_Submit(0) {}
};
S11N_TRAITS_9(TaskInfor, UInt32, m_Id, UInt8, m_Class, UInt32, m_TaskId, UInt32, m_AcceptTime, std::vector<UInt16>, m_Step, UInt32, m_TimeBegin, UInt32, m_TimeEnd, UInt8, m_Completed, UInt8, m_Submit);

struct CurrTaskResp
{	
	std::vector<TaskInfor> m_TaskInforVec;

	MESSAGE_DEF1(REP::GET_TASK_LIST, std::vector<TaskInfor>, m_TaskInforVec);
};

struct CollectNpcActinoResp
{
	UInt32 m_NpcId;

	CollectNpcActinoResp() : m_NpcId(0) {};

	MESSAGE_DEF1(REP::COLLECTNPCACTION, UInt32, m_NpcId);
};
//send attainMsg bettwen contries
struct stAttainMsg
{
    UInt32  attainID;
    UInt32  param;
    stAttainMsg() : attainID(0), param(0)
    {

    }
};
#endif // _COUNTRYMSGSTRUCT_H_

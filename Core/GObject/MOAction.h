#ifndef MOACTION_INC
#define MOACTION_INC

#include <memory>
#include "MsgHandler/CountryMsgStruct.h"
#include "Script/GameActionLua.h"
#include "Country.h"

namespace GObject
{
	enum MOActionType
	{

		MOT_None			= 0x0000,
		MOT_Task			= 0x0001,	//任务会话相关
		MOT_Collect			= 0x0002,	//采集相关
		MOT_LoopTask		= 0x0004,	//循环任务
		MOT_EquipStrength	= 0x0008,	//装备强化
		MOT_EquipForge		= 0x0010,	//装备锻造
		MOT_GemMaster		= 0x0020,	//宝石大师
		MOT_EquipReplace	= 0x0040,	//装备置换
		MOT_Professor		= 0x0800,	//潜力洗炼
		MOT_GreatFighter	= 0x1000,	//名将

	};

	class MOAction
	{
	public:
		static void TaskAction(Player* player, UInt32 npcId);
		static void TaskActionStep(Player* player, UInt32 npcId, UInt32 taskId, UInt8 step);
		
		static void CollectAction(Player* player, UInt32 npcId);
		static void CollectActionStep(Player* player, UInt32 npcId);
		static void LoopTaskAction(Player* player, UInt32 npcId);

		static void DefaultNpcAction(Player* player, UInt32 npcId);

		static void GreatFighterAction(Player* player, UInt32 npcId);
		static void GreatFighterActionStep(Player* player, UInt32 npcId);
		static void RunGreatCtrlActionStep(Player* player, UInt32 npcId);

		static void ItemTaskAction(Player* player, UInt32 taskId, UInt32 npcId);

		static void RunDayCopyTask(Player *, UInt32);
		static void RunActiveTask(Player *, UInt32);
		static void RunDayCopyTaskStep(Player *, UInt32, UInt32);
        static void RunActiveTaskStep(Player *, UInt32, UInt32);

		static void Action(Player* player, UInt32 npcId, UInt32 actionType);
	};

}



#endif

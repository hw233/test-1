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
		MOT_Task			= 0x0001,	//����Ự���
		MOT_Collect			= 0x0002,	//�ɼ����
		MOT_LoopTask		= 0x0004,	//ѭ������
		MOT_EquipStrength	= 0x0008,	//װ��ǿ��
		MOT_EquipForge		= 0x0010,	//װ������
		MOT_GemMaster		= 0x0020,	//��ʯ��ʦ
		MOT_EquipReplace	= 0x0040,	//װ���û�
		MOT_Professor		= 0x0800,	//Ǳ��ϴ��
		MOT_GreatFighter	= 0x1000,	//����

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

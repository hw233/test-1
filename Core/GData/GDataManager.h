#ifndef _GDATAMANAGER_H_
#define _GDATAMANAGER_H_

#include "GData/ItemType.h"
#include "GData/TaskType.h"

namespace GObject
{
	class  ItemWeapon;
}

namespace GData
{
	class GDataManager
	{
	public:
		static bool LoadAllData();

	public:
		static bool LoadExpData();
        static bool LoadAcuPraData();
		static bool LoadAreaData();
		static bool LoadWeaponDefData();
		static bool LoadItemTypeData();
		static bool LoadTaskTypeData();
		static bool LoadNpcData();
		static bool LoadFormationData();
		static bool LoadAttrExtraData();
		static bool LoadLootData();
		static bool LoadQueRule();
		static bool LoadFighterTrainData();
		static bool LoadPracticeData();
		static bool LoadTalent();
		static bool LoadSkillEffect();
		static bool LoadSkills();
		static bool LoadCittaEffect();
		static bool LoadCittas();
        static bool LoadClanLvlData();
		static bool LoadClanTechTable();
		static bool LoadClanSkillTable();
        static bool LoadFighterProb();

	public:
		static const TaskType& GetTaskTypeData(UInt32 typeId);
		static const TaskTypeRelation& GetTaskTypeRelationData(UInt32 typeId);
		static const std::set<UInt32>& GetTaskNpcRelationData(UInt32 npcId);
		static const std::set<UInt32>& GetTaskDayData(UInt32 taskId);
		static const std::set<UInt32>& GetTaskLevRelation(UInt16	lev);
		static const ItemBaseType* GetItemTypeData(UInt32 itemId);
		static GObject::ItemWeapon* GetNpcWeapon(UInt32 wpId);
		static const std::vector<UInt32>& GetSaleItemSortRule(UInt8);
		static const std::vector<UInt32>& GetTaelTrainList();
		static const std::vector<UInt32>& GetGoldTrainList();
		static const std::vector<UInt32>& GetLevelTrainExp();
		static const std::vector<UInt32>& GetTaelPractice();
		static const std::vector<UInt32>& GetGoldPractice();
		static const std::vector<UInt32>& GetGoldOpenSlot();
		

	public:
		static TaskTypeList				 m_TaskTypeList;
		static TaskTypeRelationList		 m_TaskTypeRelationList;
		static TaskNpcRelationList		 m_TaskNpcRelationList;
		static TaskDayList				 m_TaskDayList;
		static TaskLevRelationList		 m_TaskLevRelationList;
		static std::vector<UInt32>		 m_SaleItemsSortRule[13];
		static std::vector<UInt32>		 m_TaelTrainList;
		static std::vector<UInt32>		 m_GoldTrainList;
		static std::vector<UInt32>		 m_LevelTrainExp;
		static std::vector<UInt32>		 m_TaelPractice;
		static std::vector<UInt32>		 m_GoldPractice;
		static std::vector<UInt32>		 m_GoldOpenSlot;
	};

	
}

#endif // _GDATAMANAGER_H_

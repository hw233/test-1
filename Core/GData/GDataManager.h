﻿#ifndef _GDATAMANAGER_H_
#define _GDATAMANAGER_H_

#include "GData/ItemType.h"
#include "GData/TaskType.h"
#include "Server/ServerTypes.h"
#include "GData/SpiritAttrTable.h"

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
        static bool LoadSoulExpData();
        static bool LoadAcuPraData();
        static bool LoadAcuPraGoldData();
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
		static bool LoadTripodAward();
		static bool LoadFlushTaskFactor();
		static bool LoadFlushBookFactor();
		static bool LoadTalent();
		static bool LoadSkillEffect();
		static bool LoadSkills();
		static bool LoadCittaEffect();
		static bool LoadCittas();
        static bool LoadClanLvlData();
        static bool LoadClanCopy();
        static bool LoadClanCopyMonster();
        static bool LoadClanStatue();
        static bool LoadClanBuilding();
		static bool LoadClanTechTable();
		static bool LoadClanSkillTable();
        static bool LoadSoulSkillTable();
        static bool LoadFighterProb();
        static bool LoadCopyData();
        static bool LoadFrontMapData();
        static bool LoadOnlineAwardData();
        static bool LoadMoney();
        static bool LoadEUpgradeData();
        static bool LoadHeroMemoMaxSoul();
        static bool LoadSpiritAttrTable();
        static bool LoadSoulItemExp();
		static bool LoadSkillStrengthenEffect();
		static bool LoadSkillStrengthens();
        static bool LoadSkillStrengthenExp();
        static bool LoadSkillStrengthenProb();
        static bool LoadDreamer();
        static bool LoadPetPinJie();
        static bool LoadPetGenGu();
        static bool LoadPetLingYa();
        static bool LoadPetEquipExp();
        static bool LoadPetEqAttreffect();
        static bool LoadXingchenConfig();
        static bool LoadXinMoConfig();
        static bool LoadJiguanshuConfig();
        static bool LoadJiguanyuConfig();
        static bool LoadTuzhiConfig();
        static bool LoadKeyinConfig();
        static bool LoadZhenweiConfig();
        static bool LoadSanHunConfig();
        static bool LoadTeamArenaSkillConfig();
        static bool LoadTeamArenaInspireConfig();
        static bool LoadPetSevenSoulLevel();
        static bool LoadPetSevenSoulUpgrade();
        static bool LoadRideConfig();
        static bool LoadRideUpgradeConfig();
        static bool LoadCangjianConfig();
        static bool LoadCoupleInfo();
        static bool LoadCoupleCopy();

        static void clearUdpItem();
        static void addUdpItem(UInt32 id);
        static bool isInUdpItem(UInt32 id);

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
		static const std::vector<UInt32>& GetPlaceAddons();
		static const std::vector<UInt32>& GetShiMenTask(int country);
		static const std::vector<UInt32>& GetYaMenTask(int country);
		static const std::vector<UInt32>& GetShiYaMenTask(int country, int type);
		static const std::vector<UInt8>& GetFlushTaskFactor(int ttype, int ftype);
		static const std::vector<UInt32>& GetFlushBookFactor(int type/*0- 免费 1-银币 2-金币*/);
		static const std::vector<UInt32>& GetFlushBookPrice();
        static UInt32 GetTaskAwardFactor(int ttype, int color);
        static UInt32 GetTripodAward(int fire, int quality);
        static const std::vector<UInt32>& GetClanTask();
        static const std::vector<UInt16>& GetOnlineAward(UInt8 cls, UInt8 i);
        static UInt16 GetOnlineAwardTime(UInt8 i);
        static UInt8 GetOnlineAwardCount();
        static UInt32 getMaxStrengthenVal(UInt16 id, UInt8 clvl);
        static UInt32 getSkillStrengthenProb(UInt16 id, UInt8 clvl);
        static inline UInt32 getNeedBindLevel30CFD(UInt32 id) /** 30级副本、阵图、决战之地掉落的7种物品绑定 **/
        {
            if((id >= 1600 && id <= 1602) || (id >= 1213 && id <= 1214) || (id >= 1220 && id <= 1221))
                return true;
            else
                return false;
        };
        //仙宠相关
        static UInt16 GetPetEqTypeIdByColor(int);
        static UInt16 GetPetGemTypeIdByLev(int);
        static UInt16 GetPetEqSkill();
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
		static std::vector<UInt32>		 m_PlaceAddons;
        static std::vector<UInt32>		 m_ShiMenTask[COUNTRY_MAX];
        static std::vector<UInt32>		 m_YaMenTask[COUNTRY_MAX];
        static std::vector<UInt8>		 m_FlushTaskFactor[2][2];
        static std::vector<UInt32>		 m_TaskAwardFactor[2];
        static std::vector<UInt32>		 m_TripodAward[7];
        static std::vector<UInt32>       m_ClanTask;
        static std::vector<UInt32>       m_BookFactor[3];
        static std::vector<UInt32>       m_BookPrice;
        static std::vector<UInt16>       m_OnlineAwardTime;
        static std::vector<std::vector<UInt16> > m_OnlineAward[3];
        static std::map<UInt32, UInt32>  m_soulItemExp;
        static std::vector<UInt32>      m_udpLogItems;
        static std::map<UInt16, std::vector<UInt32> > m_skillstrengthexp;
        static std::map<UInt16, std::vector<UInt32> > m_skillstrengthprob;
        static std::vector<UInt16>       m_petEqs[4];
        static std::vector<UInt16>       m_petGems[20];
        static std::vector<UInt16>       m_petEqSkills;
	};

    extern std::map<UInt16, UInt16> skill2item;
}

#endif // _GDATAMANAGER_H_

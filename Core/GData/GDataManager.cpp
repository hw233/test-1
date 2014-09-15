#include "Config.h"
#include "GDataManager.h"
#include "Area.h"
#include "WeaponDef.h"
#include "Formation.h"
#include "ExpTable.h"
#include "ClanLvlTable.h"
#include "LootTable.h"
#include "ClanTechTable.h"
#include "ClanSkillTable.h"
#include "ClanCopyTable.h"
#include "ClanStatueTable.h"
#include "ClanBuildingTable.h"
#include "GObject/Item.h"
#include "DB/DBConnectionMgr.h"
#include "GDataDBExecHelper.h"
#include "DB/DBExecutor.h"
#include "Server/ServerTypes.h"
#include "Server/Cfg.h"
#include "SkillTable.h"
#include "TalentTable.h"
#include "CittaTable.h"
#include "CopyTable.h"
#include "FrontMapTable.h"
#include "AcuPraTable.h"
#include "FighterProb.h"
#include "Money.h"
#include "Common/StringTokenizer.h"
#include "Common/URandom.h"
#include "EUpgradeTable.h"
#include "GObject/HeroMemo.h"
#include "Script/lua_tinker.h"
#include "SoulExpTable.h"
#include "SoulSkillTable.h"
#include "SkillStrengthen.h"
#include "DreamerTable.h"
#include "FairyPetTable.h"
#include "XingchenData.h"
#include "DrinkAttr.h"
#include "IncenseTable.h"
#include "PictureAttr.h"
#include "JiguanData.h"
#include "HunPoData.h"
#include "ErlkingTable.h"
#include "TeamArenaSkill.h"
#include "SevenSoul.h"
#include "RideConfig.h"
#include "GObject/Married.h"
#include "CoupleUpgrade.h"
#include "CoupleCopy.h"
#include "LingShiTable.h"
#include "CardSystem.h"
#include "NewQuestionsTable.h"
#include "ClanShop.h"
#include "KettleNpc.h"
#include "lingbaoLevel.h"
#include "GObject/QuestionPaper.h"

namespace GData
{
    KettleNpc kettleAttrData;
	ItemBaseTypeManager		itemBaseTypeManager;
	ItemBaseTypeNameManager itemBaseTypeNameManager;
	ObjectMapT<GObject::ItemWeapon> npcWeapons;
	std::vector<ItemGemType *> gemTypes(1000);
	std::vector<ItemGemType *> petGemTypes(1000);
	std::vector<ItemGemType *> mountTypes(400);
	std::vector<ItemGemType *> lingshiTypes(1000);
	ItemEquipSetTypeManager	itemEquipSetTypeManager;
    std::map<UInt16, UInt16> skill2item;

	TaskTypeList			GDataManager::m_TaskTypeList;
	TaskTypeRelationList	GDataManager::m_TaskTypeRelationList;
	TaskNpcRelationList		GDataManager::m_TaskNpcRelationList;
	TaskDayList				GDataManager::m_TaskDayList;
	TaskLevRelationList		GDataManager::m_TaskLevRelationList;
	std::vector<UInt32>		GDataManager::m_SaleItemsSortRule[13];
	std::vector<UInt32>		GDataManager::m_TaelTrainList;
	std::vector<UInt32>		GDataManager::m_GoldTrainList;
	std::vector<UInt32>		GDataManager::m_LevelTrainExp;
	std::vector<UInt32>		GDataManager::m_TaelPractice;
	std::vector<UInt32>		GDataManager::m_GoldPractice;
	std::vector<UInt32>		GDataManager::m_GoldOpenSlot;
	std::vector<UInt32>		GDataManager::m_PlaceAddons;
	std::vector<UInt32>		GDataManager::m_ShiMenTask[COUNTRY_MAX];
	std::vector<UInt32>		GDataManager::m_YaMenTask[COUNTRY_MAX];
    std::vector<UInt8>		GDataManager::m_FlushTaskFactor[2][2];
    std::vector<UInt32>		GDataManager::m_TaskAwardFactor[2];
    std::vector<UInt32>		GDataManager::m_TripodAward[7];
    std::vector<UInt32>     GDataManager::m_ClanTask;
    std::vector<UInt32>		GDataManager::m_BookFactor[3];
    std::vector<UInt32>		GDataManager::m_BookPrice;
    std::vector<UInt16>     GDataManager::m_OnlineAwardTime;
    std::vector<std::vector<UInt16> > GDataManager::m_OnlineAward[3];
    std::map<UInt32, UInt32>  GDataManager::m_soulItemExp;
    std::vector<UInt32> GDataManager::m_udpLogItems;
    std::map<UInt16, std::vector<UInt32> > GDataManager::m_skillstrengthexp;
    std::map<UInt16, std::vector<UInt32> > GDataManager::m_skillstrengthprob;
    std::vector<UInt16>     GDataManager::m_petEqs[4];
    std::vector<UInt16>     GDataManager::m_petGems[20];
    std::vector<UInt16>     GDataManager::m_petEqSkills;
    std::vector<UInt16>     GDataManager::m_zhenyuanItem[20];

	bool GDataManager::LoadAllData()
	{
		if (!LoadExpData())
		{
			fprintf(stderr, "Load ExpData Error !\n");
            std::abort();
		}
        if(!LoadSoulExpData())
        {
			fprintf(stderr, "Load SoulExpData Error !\n");
            std::abort();
        }
		if (!LoadAcuPraData())
		{
			fprintf(stderr, "Load AcuPra Error !\n");
            std::abort();
		}
		if (!LoadAreaData())
		{
			fprintf(stderr, "Load AreaData Error !\n");
            std::abort();
		}
		if (!LoadAcuPraGoldData())
		{
			fprintf(stderr, "Load AcuPraGoldData  Error !\n");
            std::abort();
		}
		if (!LoadWeaponDefData())
		{
			fprintf(stderr, "Load WeaponDefData Error !\n");
            std::abort();
		}
        if (!LoadSkillEffect())
        {
			fprintf(stderr, "Load skill effect Error !\n");
            std::abort();
        }
        if (!LoadSkills())
        {
			fprintf(stderr, "Load skills template Error !\n");
            std::abort();
        }
		if (!LoadAttrExtraData())
		{
			fprintf(stderr, "Load AttrExtraData Error !\n");
            std::abort();
		}
		if (!LoadItemTypeData())
		{
			fprintf(stderr, "Load ItemTypeData Error !\n");
            std::abort();
		}
		if (!LoadFormationData())
		{
			fprintf(stderr, "Load FormationData Error !\n");
            std::abort();
		}

        if(!LoadEUpgradeData())
        {
            fprintf(stderr, "Load EUpgradeData Error !\n");
            std::abort();
        }
		if (!LoadTaskTypeData())
		{
			fprintf(stderr, "Load TaskTypeData Error !\n");
            std::abort();
		}
		if (!LoadLootData())
		{
			fprintf(stderr, "Load LootData Error !\n");
            std::abort();
		}
		if(!LoadQueRule())
		{
			fprintf(stderr, "Load sale que rule Error !\n");
            std::abort();
		}
		if (!LoadFighterTrainData())
		{
			fprintf(stderr, "Load fighter train data Error !\n");
            std::abort();
		}
		if (!LoadPracticeData())
		{
			fprintf(stderr, "Load practice data Error !\n");
            std::abort();
		}
		if (!LoadFlushTaskFactor())
		{
			fprintf(stderr, "Load flush task factor Error !\n");
            std::abort();
		}
		if (!LoadFlushBookFactor())
		{
			fprintf(stderr, "Load flush book factor Error !\n");
            std::abort();
		}
		if (!LoadTripodAward())
		{
			fprintf(stderr, "Load flush task factor Error !\n");
            std::abort();
		}
        if (!LoadTalent())
        {
			fprintf(stderr, "Load talent template Error !\n");
            std::abort();
        }
        if (!LoadCittaEffect())
        {
			fprintf(stderr, "Load citta effect Error !\n");
            std::abort();
        }
        if (!LoadCittas())
        {
			fprintf(stderr, "Load cittas template Error !\n");
            std::abort();
        }
		if (!LoadClanLvlData())
		{
			fprintf(stderr, "Load clan level template Error !\n");
            std::abort();
		}
		if (!LoadClanTechTable())
		{
			fprintf(stderr, "Load clan tech template Error !\n");
            std::abort();
		}
		if (!LoadClanSkillTable())
		{
			fprintf(stderr, "Load clan skill template Error !\n");
            std::abort();
		}
        if(!LoadSoulSkillTable())
        {
			fprintf(stderr, "Load soul skill template Error !\n");
            std::abort();
        }
		if (!LoadFighterProb())
		{
			fprintf(stderr, "Load fighter prob template Error !\n");
            std::abort();
		}
		if (!LoadCopyData())
		{
			fprintf(stderr, "Load copy data template Error !\n");
            std::abort();
		}
		if (!LoadFrontMapData())
		{
			fprintf(stderr, "Load front map data template Error !\n");
            std::abort();
		}
		if (!LoadXJFrontMapData())
		{
			fprintf(stderr, "Load xjfront map data template Error !\n");
            std::abort();
		}
		if (!LoadOnlineAwardData())
		{
			fprintf(stderr, "Load online award template Error !\n");
            std::abort();
		}
		if (!LoadMoney())
		{
			fprintf(stderr, "Load money Error !\n");
            std::abort();
		}
        if (!LoadHeroMemoMaxSoul())
        {
			fprintf(stderr, "Load Hero Memo Max Soul Error !\n");
            std::abort();
        }
        if (!LoadSpiritAttrTable())
        {
			fprintf(stderr, "Load Spirit Attr Type Error !\n");
            std::abort();
        }
        if(!LoadSoulItemExp())
        {
			fprintf(stderr, "Load Soul Item Exp Table Error !\n");
            std::abort();
        }
        if(!LoadSkillStrengthenExp())
        {
			fprintf(stderr, "Load Skill Strengthen Exp Table Error !\n");
            std::abort();
        }
        if(!LoadSkillStrengthenProb())
        {
			fprintf(stderr, "Load Skill Strengthen Prob Table Error !\n");
            std::abort();
        }
        if(!LoadSkillStrengthenEffect())
        {
			fprintf(stderr, "Load Skill Strengthen effect Table Error !\n");
            std::abort();
        }
        if(!LoadSkillStrengthens())
        {
			fprintf(stderr, "Load Skill Strengthen Table Error !\n");
            std::abort();
        }
        
        if (!LoadClanCopy())
        {
            fprintf (stderr, "Load Clan Copy Table Error !\n");
            std::abort();
        }

        if (!LoadClanCopyMonster())
        {
            fprintf (stderr, "Load Clan Copy Monster Table Error !\n");
            std::abort();
        }

        if (!LoadClanStatue())
        {
            fprintf (stderr, "Load Clan Statue Error !\n");
            std::abort();
        }

        if (!LoadClanBuilding())
        {
            fprintf (stderr, "Load Clan Building Error !\n");
            std::abort();
        }
        
        if (!LoadDreamer())
        {
            fprintf (stderr, "Load Dreamer Table Error !\n");
            std::abort();
        }

        if (!LoadPetPinJie())
        {
            fprintf (stderr, "Load PetPinJie Table Error !\n");
            std::abort();
        }

        if (!LoadPetGenGu())
        {
            fprintf (stderr, "Load PetGenGu Table Error !\n");
            std::abort();
        }

        if (!LoadPetLingYa())
        {
            fprintf (stderr, "Load PetLingYa Table Error !\n");
            std::abort();
        }

        if (!LoadPetEquipExp())
        {
            fprintf (stderr, "Load PetEquipLevelUpExp Table Error !\n");
            std::abort();
        }

        if (!LoadPetEqAttreffect())
        {
            fprintf (stderr, "Load PetEqAttreffect Table Error !\n");
            std::abort();
        }

        if (!LoadXingchenConfig())
        {
            fprintf (stderr, "Load LoadXingchenConfig Error !\n");
            std::abort();
        }
        if (!LoadXinMoConfig())
        {
            fprintf (stderr, "Load LoadXinMoConfig Error !\n");
            std::abort();
        }
        if (!LoadDrinkAttrConfig())
        {
            fprintf (stderr, "Load LoadDrinkAttrConfig Error !\n");
            std::abort();
        }

        if (!LoadJiguanshuConfig())
        {
            fprintf (stderr, "Load LoadJiguanshuConfig Error !\n");
            std::abort();
        }

        if (!LoadJiguanyuConfig())
        {
            fprintf (stderr, "Load LoadJiguanyuConfig Error !\n");
            std::abort();
        }

        if (!LoadTuzhiConfig())
        {
            fprintf (stderr, "Load LoadTuzhiConfig Error !\n");
            std::abort();
        }

        if (!LoadKeyinConfig())
        {
            fprintf (stderr, "Load LoadKeyinConfig Error !\n");
            std::abort();
        }

        if (!LoadZhenweiConfig())
        {
            fprintf (stderr, "Load LoadZhenweiConfig Error !\n");
            std::abort();
        }

        if (!LoadGearConfig())
        {
            fprintf (stderr, "Load LoadGearConfig Error !\n");
            std::abort();
        }

        if (!LoadGearTreeConfig())
        {
            fprintf (stderr, "Load LoadGearTreeConfig Error !\n");
            std::abort();
        }

        if (!LoadSanHunConfig())
        {
            fprintf (stderr, "Load LoadSanHunConfig Error !\n");
            std::abort();
        }

        if (!LoadErlkingConfig())
        {
            fprintf (stderr, "Load LoadErlkingConfig Error !\n");
            std::abort();
        }

        if (!LoadNewQuestionsConfig())
        {
            fprintf (stderr, "Load LoadNewQuestionsConfig Error !\n");
            std::abort();
        }

        if (!LoadTeamArenaSkillConfig())
        {
            fprintf (stderr, "Load LoadTeamArenaSkillConfig Error !\n");
            std::abort();
        }

        if (!LoadTeamArenaInspireConfig())
        {
            fprintf (stderr, "Load LoadTeamArenaInspireConfig Error !\n");
            std::abort();
        }

        if (!LoadPetSevenSoulLevel())
        {
            fprintf (stderr, "LoadPetSevenSoulLevel Error !\n");
            std::abort();
        }

        if (!LoadPetSevenSoulUpgrade())
        {
            fprintf (stderr, "LoadPetSevenSoulUpgrade Error !\n");
            std::abort();
        }

        if (!LoadRideConfig())
        {
            fprintf (stderr, "Load LoadRideConfig Error !\n");
            std::abort();
        }
        if (!LoadRideUpgradeConfig())
        {
            fprintf (stderr, "Load LoadRideUpgradeConfig Error !\n");
            std::abort();
        }
        if (!LoadCangjianConfig())
        {
            fprintf (stderr, "Load LoadCangjianConfig Error !\n");
            std::abort();
        }
        if (!LoadCoupleInfo())
        {
            fprintf (stderr, "Load LoadCoupleInfoConfig Error !\n");
            std::abort();
        }
        if (!LoadCoupleCopy())
        {
            fprintf (stderr, "Load LoadCoupleCopyConfig Error !\n");
            std::abort();
        }
        if (!LoadSkillEvConfig())
        {
            fprintf (stderr, "Load LoadSkillEvConfig Error !\n");
            std::abort();
        }

        if (!LoadRandBattleConfig())
        {
            fprintf (stderr, "Load LoadRandBattleConfig Error !\n");
            std::abort();
        }

        if (!LoadLingShiConfig())
        {
            fprintf (stderr, "Load LoadLingShiConfig Error !\n");
            std::abort();
        }
        if (!LoadCardUpgrade())
        {
            fprintf (stderr, "Load LoadCardUpgrade Error !\n");
            std::abort();
        }
        if (!LoadClanShopInfo())
        {
            fprintf (stderr, "Load LoadClanShopInfo Error !\n");
            std::abort();
        }
        if (!LoadCardInfo())
        {
            fprintf (stderr, "Load LoadCardInfo Error !\n");
            std::abort();
        }
        if (!LoadCubeAttr())
        {
            fprintf (stderr, "Load LoadCubeAttr Error !\n");
            std::abort();
        }
        if (!LoadFloorAttr())
        {
            fprintf (stderr, "Load LoadFloorAttr Error !\n");
            std::abort();
        }
        if (!LoadPicInfo())
        {
            fprintf (stderr, "Load LoadPicInfo Error !\n");
            std::abort();
        }
        if (!LoadCubeCount())
        {
            fprintf (stderr, "Load LoadCubeCount Error !\n");
            std::abort();
        }
        if (!LoadKettleNpc())
        {
            fprintf (stderr, "Load LoadKettleNpc Error !\n");
            std::abort();
        }
        if (!LoadLingbaoLevel())
        {
            fprintf (stderr, "Load LoadLingbaoLevel Error !\n");
            std::abort();
        }

        if (!LoadIncenseAttrConfig())
        {
            fprintf (stderr, "Load LoadIncenseAttrConfig Error !\n");
            std::abort();
        }
        
        if (!LoadTitlePaper())
        {
            fprintf (stderr, "Load LoadTitlePaper Error !\n");
            std::abort();
        }

		return true;
	}

    bool GDataManager::LoadSoulExpData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBSoulExp dbexp;
		if(execu->Prepare("SELECT `lvl`, `exp`, `pexp` FROM `soul_lvl_exp`", dbexp) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            if(dbexp.exp != 0)
                soulExpTable.setTable(dbexp.lvl, dbexp.exp);
            if(dbexp.pexp != 0)
                soulExpTable.setPracticeTable(dbexp.lvl, dbexp.pexp);
		}
		return true;
	}


	bool GDataManager::LoadExpData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBExp dbexp;
		if(execu->Prepare("SELECT `lvl`, `exp` FROM `lvl_exp`", dbexp) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			expTable.setTable(dbexp.lvl, dbexp.exp);
		}
#ifdef _DEBUG
        //fprintf(stderr, "lvl: %u\n", expTable.exp2level(22159159243-1 ));
        //fprintf(stderr, "lvl: %u\n", expTable.exp2level(21501576053-1 ));
        //fprintf(stderr, "lvl: %u\n", expTable.exp2level(20856473348-1 ));
        //fprintf(stderr, "lvl: %u\n", expTable.exp2level(14551108403-1 ));
#endif
		return true;
	}

	bool GDataManager::LoadClanLvlData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBClanLvl dbexp;
		if(execu->Prepare("SELECT `lvl`, `exp`, `pkgsize`, `weal` FROM `clan_lvl`", dbexp) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			clanLvlTable.setTable(dbexp.lvl, dbexp.exp, dbexp.pkgsize, dbexp.weal);
		}
		return true;
	}

	bool GDataManager::LoadAcuPraData()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBAcuPra ap;
		if(execu->Prepare("SELECT `id`, `lvl`, `needlvl`, `pra`, `soulmax`, `pramax`, `citslot` FROM `acupra`", ap) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            GData::AcuPra* pap = new GData::AcuPra(ap.id << 8 | ap.lvl);
            if (!pap)
                return false;
            pap->needlvl = ap.needlvl;
            pap->pra = ap.pra;
            pap->soulmax = ap.soulmax;
            pap->pramax = ap.pramax;
            pap->citslot = ap.citslot;
            GData::acupraManager.add(pap);
		}
        return true;
    }
	bool GDataManager::LoadAcuPraGoldData()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBAcuPraGold ap;
		if(execu->Prepare("SELECT `id`,`lvl`,`usereal`,`soulmax`,`attrnum`,`attrvalue` FROM `acupragold`", ap) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            GData::AcuPraGold* pap = new GData::AcuPraGold(ap.id << 8 | ap.lvl);
            if (!pap)
                return false;
            pap->useReal = ap.useReal;
          //  pap->praadd = ap.praadd;
            pap->soulmax = ap.soulmax;
            pap->attrNum = ap.attrNum;
            pap->attrValue = ap.attrValue;
            GData::acupraGoldManager.add(pap);
		}
        return true;
    }

	bool GDataManager::LoadAreaData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBAreaDef adef;
		if(execu->Prepare("SELECT `id`, `side`, `area` FROM `area`", adef) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			std::vector<Area::Data> ad;
			{
				const std::string area = adef.area;
				StringTokenizer tokenizer(area, ";");
				ad.resize(tokenizer.count());
				StringTokenizer::Iterator it;
				int z = 0;
				for(it = tokenizer.begin(); it != tokenizer.end(); ++ it)
				{
					const std::string& s = *it;
					size_t p = s.find(',');
					if(p == std::string::npos)
					{
						ad[z].x = 0;
						ad[z].y = 0;
					}
					else
					{
						std::string v = std::string(s.begin(), s.begin() + p);
						ad[z].x = atoi(v.c_str());
						ad[z].y = atoi(s.c_str() + p + 1);
					}
					++ z;
				}
			}
			Area& ar = areaList[adef.id];
			ar.setSide(adef.side);
			for(size_t i = 0; i < ad.size(); ++ i)
			{
				ar.add(ad[i]);
			}
		}
		return true;
	}

	bool GDataManager::LoadWeaponDefData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBWeaponDef wdef;
		if(execu->Prepare("SELECT `id`, `name`, `classReq`, `areaEffect`, `canCounter` FROM `weapon_def`", wdef) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			WeaponDefManager::Instance().add(new WeaponDef(wdef.id, wdef.name, wdef.class_req, wdef.can_counter > 0, areaList[wdef.area_effect]));
		}

		return true;
	}

	void SetValOrPercent(UInt16& val, float& perc, const std::string& str)
	{
		if(str[str.length() - 1] == '%')
		{
			std::string astr(str.begin(), str.end() - 1);
			perc = static_cast<float>(atof(astr.c_str())) / 100;
			val = 0;
		}
		else
		{
			val = atoi(str.c_str());
			perc = 0;
		}
	}

	void SetValOrPercent(Int16& val, float& perc, const std::string& str)
	{
		if(str[str.length() - 1] == '%')
		{
			std::string astr(str.begin(), str.end() - 1);
			perc = static_cast<float>(atof(astr.c_str())) / 100;
			val = 0;
		}
		else
		{
			val = atoi(str.c_str());
			perc = 0;
		}
	}

	void SetValOrPercent(float& val, float& perc, const std::string& str)
	{
		if(str[str.length() - 1] == '%')
		{
			std::string astr(str.begin(), str.end() - 1);
			perc = static_cast<float>(atof(astr.c_str())) / 100;
			val = 0;
		}
		else
		{
			val = atof(str.c_str());
			perc = 0;
		}
	}

	bool GDataManager::LoadAttrExtraData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		GData::DBAttrExtra ae;
		if(execu->Prepare("SELECT `id`, `skill`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`, `auraMax`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `tough`, `action`, `hitrate`, `evade`, `critical`, `criticaldmg`, `pierce`, `counter`, `magres` FROM `attr_extra`", ae) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			AttrExtraItem * aextra = new AttrExtraItem(ae.id);
			SetValOrPercent(aextra->_extra.strength, aextra->_extra.strengthP, ae.strength);
			SetValOrPercent(aextra->_extra.physique, aextra->_extra.physiqueP, ae.physique);
			SetValOrPercent(aextra->_extra.agility, aextra->_extra.agilityP, ae.agility);
			SetValOrPercent(aextra->_extra.intelligence, aextra->_extra.intelligenceP, ae.intelligence);
			SetValOrPercent(aextra->_extra.will, aextra->_extra.willP, ae.will);
			SetValOrPercent(aextra->_extra.soul, aextra->_extra.soulP, ae.soul);
			SetValOrPercent(aextra->_extra.aura, aextra->_extra.auraP, ae.aura);
			SetValOrPercent(aextra->_extra.auraMax, aextra->_extra.auraMaxP, ae.auraMax);
			SetValOrPercent(aextra->_extra.attack, aextra->_extra.attackP, ae.attack);
			SetValOrPercent(aextra->_extra.magatk, aextra->_extra.magatkP, ae.magatk);
			SetValOrPercent(aextra->_extra.defend, aextra->_extra.defendP, ae.defend);
			SetValOrPercent(aextra->_extra.magdef, aextra->_extra.magdefP, ae.magdef);
			SetValOrPercent(aextra->_extra.hp, aextra->_extra.hpP, ae.hp);
			SetValOrPercent(aextra->_extra.toughlvl, aextra->_extra.tough, ae.tough);
            aextra->_extra.tough *= 100;
			SetValOrPercent(aextra->_extra.action, aextra->_extra.actionP, ae.action);
			SetValOrPercent(aextra->_extra.hitrlvl, aextra->_extra.hitrate, ae.hitrate);
            aextra->_extra.hitrate *= 100;
			SetValOrPercent(aextra->_extra.evdlvl, aextra->_extra.evade, ae.evade);
            aextra->_extra.evade *= 100;
			SetValOrPercent(aextra->_extra.crilvl, aextra->_extra.critical, ae.critical);
            aextra->_extra.critical *= 100;
			SetValOrPercent(aextra->_extra.criticaldmg, aextra->_extra.criticaldmgP, ae.criticaldmg);
            aextra->_extra.criticaldmg /= 100;
			SetValOrPercent(aextra->_extra.pirlvl, aextra->_extra.pierce, ae.pierce);
            aextra->_extra.pierce *= 100;
			SetValOrPercent(aextra->_extra.counterlvl, aextra->_extra.counter, ae.counter);
            aextra->_extra.counter *= 100;
			SetValOrPercent(aextra->_extra.mreslvl, aextra->_extra.magres, ae.magres);
            aextra->_extra.magres *= 100;
            aextra->_extra.criticaldmgimmune = 0;

            StringTokenizer tk(ae.skill, "|");
            if (tk.count())
            {
                for (size_t i=0; i<tk.count(); ++i)
                {
                    const SkillBase* skill = skillManager[::atoi(tk[i].c_str())];
                    if (skill)
                        (*aextra)->skills.push_back(skill);
                }
            }

			attrExtraManager.add(aextra);
		}
		return true;
	}

    void makeSkill2Item(const std::vector<const SkillBase*>& sks, UInt16 itemid)
    {
        if (!sks.size())
            return;

        UInt16 skillid = 0;
        for (UInt32 i = 0; i < sks.size(); ++i)
        {
            if (sks[i])
            {
                skillid = sks[i]->getId();
                skill2item[skillid/100] = itemid;
            }
        }
    }

	bool GDataManager::LoadItemTypeData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBItemType idt;
		if(execu->Prepare("SELECT `id`, `name`, `subClass`, `career`, `reqLev`, `vLev`, `coin`, `quality`, `maxQuantity`, `bindType`, `energy`, `trumpExp`, `data`, `enchant`, `attrId`, `salePriceUp` FROM `item_template`", idt) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            if(idt.typeId == 0)
                continue;

			ItemBaseType * wt;
			switch(idt.subClass)
			{
			case Item_Weapon:
				{
					wt = new ItemWeaponType(idt.typeId, idt.name, idt.attrExtra);
                    if (IsEquipId(idt.typeId))
                    {
                        GObject::ItemEquipData ied;
                        npcWeapons.add(new GObject::ItemWeapon(idt.typeId, wt, ied));
                    }
				}
				break;
			case Item_Armor1:
			case Item_Armor2:
			case Item_Armor3:
			case Item_Armor4:
			case Item_Armor5:
			case Item_Ring:
			case Item_Amulet:
				{
					wt = new ItemEquipType(idt.typeId, idt.name, idt.attrExtra);
				}
				break;
            case Item_Halo:
            case Item_InnateTrump:
			case Item_Fashion: // XXX:
			case Item_Trump:
				{
					wt = new ItemTrumpType(idt.typeId, idt.name, idt.attrExtra);
                    if (((ItemTrumpType*)wt)->attrExtra)
                        makeSkill2Item(((ItemTrumpType*)wt)->attrExtra->skills, idt.typeId);
				}
                break;
            case Item_Gem:
            case Item_Gem1:
            case Item_Gem2:
            case Item_Gem3:
            case Item_Gem4:
            case Item_Gem5:
            case Item_Gem6:
            case Item_Gem8:
            case Item_Gem9:
            case Item_Gem10:
            case Item_Gem11:
            case Item_Gem12:
            case Item_Gem13:
            case Item_Gem14:
            case Item_Gem15:
            case Item_Gem16:
            case Item_Gem17:
            case Item_Gem18:
            case Item_Gem19:
				{
					ItemGemType * igt = new ItemGemType(idt.typeId, idt.name, idt.attrExtra);
					wt = igt;
					gemTypes[wt->getId() - LGEM_ID] = igt;
				}
				break;
            case Item_PetGem:
            case Item_PetGem1:
            case Item_PetGem2:
            case Item_PetGem3:
            case Item_PetGem4:
            case Item_PetGem5:
            case Item_PetGem6:
            case Item_PetGem7:
            case Item_PetGem8:
            case Item_PetGem9:
            case Item_PetGem10:
            case Item_PetGem11:
				{
					ItemGemType * igt = new ItemGemType(idt.typeId, idt.name, idt.attrExtra);
					wt = igt;
					petGemTypes[wt->getId() - LPETGEM_ID] = igt;
                    if(idt.reqLev > 0 && idt.reqLev <= 20)
                        m_petGems[idt.reqLev - 1].push_back(idt.typeId);
				}
				break;
            case Item_Mount:
            case Item_MountChip:
				{
					ItemGemType * igt = new ItemGemType(idt.typeId, idt.name, idt.attrExtra);
					wt = igt;
					mountTypes[wt->getId() - LMOUNT_ID] = igt;
                }
				break;
            case Item_LingShi:
            case Item_LingShi1:
            case Item_LingShi2:
            case Item_LingShi3:
				{
					ItemGemType * igt = new ItemGemType(idt.typeId, idt.name, idt.attrExtra);
					wt = igt;
					lingshiTypes[wt->getId() - LLINGSHI_ID] = igt;
                }
				break;
            case Item_PetEquip:
            case Item_PetEquip1:
            case Item_PetEquip2:
            case Item_PetEquip3:
            case Item_PetEquip4:
            case Item_PetEquip5:
            case Item_PetEquip6:
            case Item_PetEquip7:
            case Item_PetEquip8:
            case Item_PetEquip9:
            case Item_PetEquip10:
                {
                    if(idt.quality > 2 && idt.quality < 6)
                        m_petEqs[idt.quality - 2].push_back(idt.typeId);
                }
            case Item_Formula6:
            case Item_Formula7:
            case Item_Formula8:
            case Item_Formula9:
                {
                    if(idt.reqLev >= 75 && (idt.reqLev % 5) == 0)
                        m_zhenyuanItem[(idt.reqLev-75)/5].push_back(idt.typeId);
                }
			default:
				{
					wt = new ItemNormalType(idt.typeId, idt.name);
				}
				break;
			}
			wt->subClass = static_cast<ItemClass>(idt.subClass);
			wt->price = idt.coin;
			wt->reqLev = idt.reqLev;
			wt->vLev = idt.vLev;
			wt->quality = idt.quality;
			wt->maxQuantity = idt.maxQuantity;
			wt->bindType = idt.bindType;
			wt->energy = idt.energy;
			wt->trumpExp = idt.trumpExp;
			wt->data = idt.data;
            wt->career = idt.career;
            wt->salePriceUp = idt.salePriceUp;
			itemBaseTypeManager.add(wt);
			itemBaseTypeNameManager.add(wt->getName(), wt);
		}
		DBEquipSetType est;
		if(execu->Prepare("SELECT `itemId`, `name`, `attrId2`, `attrId4`, `attrId6`, `attrId8` FROM `equipment_set`", est) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            if(est.id == 0)
                continue;
            ItemEquipSetType * iest = NULL;
            if(est.id < 2000)
                iest = new ItemEquipSetType(1 + (est.id - 1750)/8, est.name);
            else
                iest = new ItemEquipSetType(33 + (est.id - 2000)/8, est.name);
            /*
            if(est.id - 1750 < 8)
            {
                iest = new ItemEquipSetType(1, est.name);
            }
            else
            {
                if (est.id < 2000)
                    continue;
                iest = new ItemEquipSetType((est.id-2000)/8, est.name);
            }
            */
			for(int j = 0; j < 4; ++ j)
			{
                if(est.attrId[j] != 0)
				    iest->attrExtra[j] = *attrExtraManager[est.attrId[j]];
			}
			itemEquipSetTypeManager.add(iest);
		}

		return true;
	}

	bool GDataManager::LoadTaskTypeData()
	{
		{
			lua_State* L = lua_open();
			luaopen_base(L);
			luaopen_string(L);
			luaopen_table(L);
			{
				std::string path = cfg.scriptPath + "DayTask/DayTaskTable.lua";
				lua_tinker::dofile(L, path.c_str());
				lua_tinker::table dayTasks = lua_tinker::call<lua_tinker::table>(L, "LoadAllLoopTasks");
				UInt32 size = dayTasks.size();
				for (UInt32 i = 1; i <= size; i++)
				{
					UInt32 dayTaskId = dayTasks.get<UInt32>(i);
					lua_tinker::table elem = lua_tinker::call<lua_tinker::table>(L, "LoadAllLoopTaskTasks", dayTaskId);
					for (UInt32 j = 1; j <= 2; j++)
					{
						lua_tinker::table subElem = elem.get<lua_tinker::table>(j);
						UInt32 sz = subElem.size();
						for (UInt32 k = 1; k <= sz; k++)
						{
							m_TaskDayList[dayTaskId].insert(subElem.get<UInt32>(k));
						}
					}
				}
				//fixme
				m_TaskDayList[1003].insert(70300);
				m_TaskDayList[1004].insert(70400);
			}
			lua_close(L);
		}
		{
			lua_State* L = lua_open();
            luaL_openlibs(L);
			//luaopen_base(L);
			//luaopen_string(L);
			//luaopen_table(L);
			{
#ifdef _VT
				std::string path = cfg.scriptPath + "ServerTaskConfVt.lua";
#else
				std::string path = cfg.scriptPath + "ServerTaskConf.lua";
#endif
				lua_tinker::dofile(L, path.c_str());
				lua_tinker::table conf = lua_tinker::call<lua_tinker::table>(L, "GetTaskConfTable");
				UInt32 size = conf.size();
				for (UInt32 i = 1; i <= size; i++)
				{
					lua_tinker::table elem = conf.get<lua_tinker::table>(i);
					int pos = 1;
					TaskType task;
					task.m_TypeId		= elem.get<UInt32>(pos++);
					task.m_Name			= elem.get<const char*>(pos++);
					task.m_Class		= elem.get<UInt8>(pos++);
					task.m_SubType		= elem.get<UInt8>(pos++);
					task.m_AcceptNpc	= elem.get<UInt32>(pos++);
					task.m_SubmitNpc	= elem.get<UInt32>(pos++);
					task.m_ReqTime		= elem.get<UInt32>(pos++);
					task.m_AcceptMaxNum = elem.get<UInt32>(pos++);
					const std::string reqStep = elem.get<const char*>(pos++);
					StringTokenizer tokenizer1(reqStep, ",");
					StringTokenizer::Iterator it1;
					for (it1 = tokenizer1.begin(); it1 != tokenizer1.end(); ++it1)
					{
						task.m_ReqStep.push_back(atoi((*it1).c_str()));
					}

					const std::string preTask = elem.get<const char*>(pos++);
					StringTokenizer tokenizer2(preTask, ",");
					StringTokenizer::Iterator it2;
					for(it2 = tokenizer2.begin(); it2 != tokenizer2.end(); ++ it2)
					{
						task.m_PreTask.insert(static_cast<UInt32>(atoi((*it2).c_str())));
					}
					task.m_LoopTask = 0;
					if (task.m_Class == 3)
					{
						TaskDayList::const_iterator cit = m_TaskDayList.begin();
						for (; cit != m_TaskDayList.end(); ++cit)
						{
							if (cit->second.find(task.m_TypeId) != cit->second.end())
							{
								task.m_LoopTask = cit->first;
								break;
							}
						}
					}
					task.m_ReqLev = elem.get<UInt16>(pos++);
					task.m_Country = elem.get<UInt32>(pos++);

                    if (task.m_Class == 4)
                        m_ShiMenTask[task.m_Country].push_back(task.m_TypeId);
                    else if (task.m_Class == 5)
                        m_YaMenTask[task.m_Country].push_back(task.m_TypeId);
                    if (task.m_Class == 6)
                        m_ClanTask.push_back(task.m_TypeId);

                    m_TaskTypeList.insert(std::make_pair(task.m_TypeId, task));
				}
			}
			lua_close(L);
		}

		TaskTypeList::const_iterator cit = m_TaskTypeList.begin();
		for (; cit != m_TaskTypeList.end(); ++cit)
		{
			//load task relation table
			m_TaskTypeRelationList[cit->second.m_TypeId].m_PreTask = cit->second.m_PreTask;
			std::set<UInt32>::const_iterator i = cit->second.m_PreTask.begin();
			for (; i != cit->second.m_PreTask.end(); ++i)
			{
				m_TaskTypeRelationList[*i].m_PostTask.insert(cit->first);
			}

			//load npc task relation table
			if (cit->second.m_AcceptNpc != 0 && cit->second.m_Class != 3)
			{
				m_TaskNpcRelationList[cit->second.m_AcceptNpc].insert(cit->first);
			}
			if (cit->second.m_SubmitNpc != 0/* && cit->second.m_Class != 3*/)
			{
				m_TaskNpcRelationList[cit->second.m_SubmitNpc].insert(cit->first);
			}

			//load task level relation table
			for (UInt16 lev = cit->second.m_ReqLev; lev <= LEVEL_MAX; lev++)
			{
				m_TaskLevRelationList[lev].insert(cit->first);
			}
		}

		return true;
	}

	bool GDataManager::LoadQueRule()
	{
		lua_State* L = lua_open();
		luaopen_base(L);
		luaopen_string(L);
		luaopen_table(L);
		{
			std::string path = cfg.scriptPath + "items/saleItem.lua";
			lua_tinker::dofile(L, path.c_str());
			for(UInt32 i = 0; i < 13; i ++)
			{
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "GetSaleRule", i + 1);
				UInt32 size = table_temp.size();
				UInt32 itemid;
				for(UInt32 j = 0; j < size; j ++)
				{
					itemid =  table_temp.get<UInt32>(j + 1);
					m_SaleItemsSortRule[i].push_back(itemid);
				}
			}
		}
		lua_close(L);
		return true;
	}

	bool GDataManager::LoadFighterTrainData()
	{
		lua_State * L = lua_open();
		luaopen_base(L);
		luaopen_string(L);
		luaopen_table(L);
		{
			std::string path = cfg.scriptPath + "Other/FighterTrain.lua";
			lua_tinker::dofile(L, path.c_str());

			//TaelTrain
			lua_tinker::table tael_train = lua_tinker::call<lua_tinker::table>(L, "GetTaelTrain");
			UInt32 sz1 = tael_train.size();
			for (UInt32 i = 0; i < sz1; ++ i)
			{
				lua_tinker::table tael_train_elem = tael_train.get<lua_tinker::table>(i+1);
				UInt32 elem_sz = tael_train_elem.size();
				m_TaelTrainList.resize(LEVEL_MAX + 1);
				for (UInt32 j = 0; j < elem_sz; ++ j)
				{
					UInt8 lowLev, highLev;
					UInt32 tael;
					lowLev = tael_train_elem.get<UInt8>(1);
					highLev = tael_train_elem.get<UInt8>(2);
					tael = tael_train_elem.get<UInt32>(3);
					for (UInt8 lev = lowLev; lev <= highLev; ++ lev)
						m_TaelTrainList[lev] = tael;
				}
			}

			//GoldTrain
			lua_tinker::table gold_train = lua_tinker::call<lua_tinker::table>(L, "GetGoldTrain");
			UInt32 sz2 = gold_train.size();
			m_GoldTrainList.resize(LEVEL_MAX + 1);
			for (UInt32 i = 0; i < sz2; ++ i)
			{
				lua_tinker::table gold_train_elem = gold_train.get<lua_tinker::table>(i+1);
				UInt32 elem_sz = gold_train_elem.size();
				for (UInt32 j = 0; j < elem_sz; ++ j)
				{
					UInt32 lowLev, highLev;
					UInt32 gold;
					lowLev = gold_train_elem.get<UInt8>(1);
					highLev = gold_train_elem.get<UInt8>(2);
					gold = gold_train_elem.get<UInt32>(3);
					for (UInt8 lev = lowLev; lev <= highLev; ++ lev)
						m_GoldTrainList[lev] = gold;
				}
			}

			//LevelTrainExp
			lua_tinker::table exp_train = lua_tinker::call<lua_tinker::table>(L, "GetLevelTrainExp");
			UInt32 sz3 = exp_train.size();
			m_LevelTrainExp.resize(LEVEL_MAX + 1);
			for (UInt32 i = 0; i < sz3; ++ i)
			{
				lua_tinker::table exp_train_elem = exp_train.get<lua_tinker::table>(i+1);
				UInt32 elem_sz = exp_train_elem.size();
				for (UInt32 j = 0; j < elem_sz; ++ j)
				{
					UInt8 lev = exp_train_elem.get<UInt8>(1);
					UInt32 exp = exp_train_elem.get<UInt32>(2);
					m_LevelTrainExp[lev] = exp;
				}
			}
		}
		return true;
	}

	bool GDataManager::LoadPracticeData()
	{
		lua_State * L = lua_open();
		luaopen_base(L);
		luaopen_string(L);
		luaopen_table(L);
		{
			std::string path = cfg.scriptPath + "formula/practice.lua";
			lua_tinker::dofile(L, path.c_str());

            {
                lua_tinker::table addons = lua_tinker::call<lua_tinker::table>(L, "GetPlaceAddons");
                UInt32 size = addons.size();
                for (UInt32 i = 0; i < size; ++ i)
                {
                    m_PlaceAddons.push_back(addons.get<float>(i+1)*100+100);
                }
            }

            {
                lua_tinker::table tael_pra = lua_tinker::call<lua_tinker::table>(L, "GetTaelPractice");
                UInt32 size = tael_pra.size();
                for (UInt32 i = 0; i < size; ++ i)
                {
                    m_TaelPractice.push_back(tael_pra.get<UInt32>(i+1));
                }
            }

            {
                lua_tinker::table gold_pra = lua_tinker::call<lua_tinker::table>(L, "GetGoldPractice");
                UInt32 size = gold_pra.size();
                for (UInt32 i = 0; i < size; ++ i)
                {
                    m_GoldPractice.push_back(gold_pra.get<UInt32>(i+1));
                }
            }

            {
                lua_tinker::table gold_openslot = lua_tinker::call<lua_tinker::table>(L, "GetGoldOpenSlot");
                UInt32 size = gold_openslot.size();
                for (UInt32 i = 0; i < size; ++ i)
                {
                    m_GoldOpenSlot.push_back(gold_openslot.get<UInt32>(i+1));
                }
            }

        }
        return true;
    }

	bool GDataManager::LoadTripodAward()
	{
		lua_State * L = lua_open();
		luaopen_base(L);
		luaopen_string(L);
		luaopen_table(L);
		{
			std::string path = cfg.scriptPath + "items/TripodLoot.lua";
			lua_tinker::dofile(L, path.c_str());

            {
                for (int i = 0; i < 7; ++i) {
                    lua_tinker::table award = lua_tinker::call<lua_tinker::table>(L, "GetTripodAward", i+1);
                    UInt32 size = award.size();
                    for (UInt32 n = 0; n < size; ++n)
                    {
                        m_TripodAward[i].push_back(award.get<UInt32>(n+1));
                    }
                }
            }
        }
        return true;
    }

	bool GDataManager::LoadFlushTaskFactor()
	{
		lua_State * L = lua_open();
		luaopen_base(L);
		luaopen_string(L);
		luaopen_table(L);
		{
			std::string path = cfg.scriptPath + "formula/flushtask.lua";
			lua_tinker::dofile(L, path.c_str());

            {
                for (int i = 0; i < 2; ++i) {
                    for (int j = 0; j < 2; ++j) {
                        lua_tinker::table factor = lua_tinker::call<lua_tinker::table>(L, "GetFlushTaskFactor", i+1, j+1);
                        UInt32 size = factor.size();
                        for (UInt32 n = 0; n < size; ++n)
                        {
                            m_FlushTaskFactor[i][j].push_back(factor.get<UInt8>(n+1));
                        }
                    }
                }

#if 0
                for (int i = 0; i < 2; ++i) {
                    lua_tinker::table factor = lua_tinker::call<lua_tinker::table>(L, "GetTaskAwardFactor", i+1);
                    UInt32 size = factor.size();
                    for (UInt32 n = 0; n < size; ++n)
                    {
                        m_TaskAwardFactor[i].push_back(factor.get<UInt8>(n+1));
                    }
                }
#endif
            }
        }
        return true;
    }

    struct idfact
    {
        UInt32 id;
        UInt32 factor;
        UInt32 price;
    };

    class Sort
    {
        public:
            bool operator()(idfact a, idfact b)
            {
                return a.factor < b.factor;
            }
    };

	bool GDataManager::LoadFlushBookFactor()
	{
		lua_State * L = lua_open();
		luaopen_base(L);
		luaopen_string(L);
		luaopen_table(L);
		{
			std::string path = cfg.scriptPath + "formula/flushbook.lua";
			lua_tinker::dofile(L, path.c_str());

            std::vector<idfact> ids;
            for (int i = 0; i < 3; ++i)
            {
                lua_tinker::table factor = lua_tinker::call<lua_tinker::table>(L, "GetFlushBookFactor", i+1);
                if (!factor.size())
                    return false;
                for (int j = 0; j < factor.size(); ++j)
                {
                    lua_tinker::table idnf = factor.get<lua_tinker::table>(j+1);
                    idfact t;
                    t.id = idnf.get<UInt32>(1);
                    t.factor = idnf.get<UInt32>(2);
                    t.price = idnf.get<UInt32>(3);
                    ids.push_back(t);
                }

                // totalfactor,factor1,id1,factor2,id2...factorN,idN
                m_BookFactor[i].resize(2*factor.size()+1);
                m_BookPrice.resize(2*factor.size());
                std::sort(ids.begin(), ids.end(), Sort());

                UInt32 totalfactor = 0;
                for (UInt32 m = 1, n = 0, l = 0; n < ids.size(); m+=2, ++n, l+=2)
                {
                    totalfactor += ids[n].factor;
                    m_BookFactor[i][m] = totalfactor;
                    m_BookFactor[i][m+1] = ids[n].id;

                    // id1,price1,id2,price2,...idN,priceN
                    m_BookPrice[l] = ids[n].id;
                    m_BookPrice[l+1] = ids[n].price;
                }
                m_BookFactor[i][0] = totalfactor;

                ids.clear();
            }
        }
        return true;
    }

    bool GDataManager::LoadTalent()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBTalent tal;
		if(execu->Prepare("SELECT `id`, `name`, `cls`, `quality`, `prob`, `potential`, `capacity`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`, `auraMax`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `tough`, `action`, `hitrate`, `evade`, `critical`, `criticaldmg`, `pierce`, `counter`, `magres` FROM `talent`", tal) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            Talent* ptal = new Talent(tal.id, tal.name);
            ptal->cls = tal.cls;
            ptal->quality = tal.quality;
            ptal->prob = tal.prob;
            ptal->potential = tal.potential;
            ptal->capacity = tal.capacity;
            SetValOrPercent(ptal->strength, ptal->strengthP, tal.strength);
            SetValOrPercent(ptal->physique, ptal->physiqueP, tal.physique);
            SetValOrPercent(ptal->agility, ptal->agilityP, tal.agility);
            SetValOrPercent(ptal->intelligence, ptal->intelligenceP, tal.intelligence);
            SetValOrPercent(ptal->will, ptal->willP, tal.will);
            SetValOrPercent(ptal->soul, ptal->soulP, tal.soul);
            SetValOrPercent(ptal->aura, ptal->auraP, tal.aura);
            SetValOrPercent(ptal->auraMax, ptal->auraMaxP, tal.auraMax);
            SetValOrPercent(ptal->attack, ptal->attackP, tal.attack);
            SetValOrPercent(ptal->magatk, ptal->magatkP, tal.magatk);
            SetValOrPercent(ptal->defend, ptal->defendP, tal.defend);
            SetValOrPercent(ptal->magdef, ptal->magdefP, tal.magdef);
            SetValOrPercent(ptal->hp, ptal->hpP, tal.hp);
            ptal->tough = tal.tough;
            ptal->action = tal.action;
            ptal->evade = tal.evade;
            ptal->critical = tal.critical;
            ptal->criticaldmg = tal.criticaldmg / 100;
            ptal->pierce = tal.pierce;
            ptal->counter = tal.counter;
            ptal->magres = tal.magres;
            talentManager.add(ptal);
        }
        return true;
    }


    bool GDataManager::LoadSkillStrengthenEffect()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSkillStrengthenEffect dbne;
        if(execu->Prepare("SELECT `id`, `cond`, `target`, `prob`, `area`, `factor`, `last`, `type`, `value`, `valueExtend1`, `valueExtend2` FROM  `skillstrengthen_effect`", dbne) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            SkillStrengthenEffect* ef = new SkillStrengthenEffect(dbne.id);
            if(!ef)
                return false;
            ef->cond = dbne.cond;
            ef->target = dbne.target;
            ef->prob = dbne.prob;
            ef->area = dbne.area;
            ef->last = dbne.last;
            ef->type = dbne.type;
            ef->value = dbne.value;
            ef->valueExt1 = dbne.valueExt1;
            ef->valueExt2 = dbne.valueExt2;

            StringTokenizer tk(dbne.factor, ",");
            if (tk.count())
            {
                for (size_t i = 0; i < tk.count(); ++i)
                    ef->factor.push_back(::atof(tk[i].c_str()));
            }

            skillStrengthenEffectManager.add(ef);
        }

        return true;
    }

    bool GDataManager::LoadSkillStrengthens()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSkillStrengthen dbnd;
        if(execu->Prepare("SELECT `id`, `name`, `effect` FROM  `skillstrengthen_skill`", dbnd) != DB::DB_OK)
            return false;
        while(execu->Next() == DB::DB_OK)
        {
            SkillStrengthenBase* skillstrengthen = new SkillStrengthenBase(dbnd.id, dbnd.name);
            if(!skillstrengthen)
                return false;

            StringTokenizer tk(dbnd.effect, ",");
            if (tk.count())
            {
                for (size_t i = 0; i < tk.count(); ++i)
                {
                    skillstrengthen->effect.push_back(skillStrengthenEffectManager[::atoi(tk[i].c_str())]);
                }
            }

            skillStrengthenManager.add(skillstrengthen);
        }

        return true;
    }

    bool GDataManager::LoadSkillEffect()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBSkillEffect effs;
		if(execu->Prepare("SELECT `id`, `state`, `immune`, `disperse`, `damage`, `adddam`, `magdam`, `addmag`, `crrdam`, `addcrr`, `hp`, `addhp`, `absorb`, `thorn`, `inj2hp`, `aura`, `atk`, `def`, `magatk`, `magdef`, `tough`, `action`, `hitrate`, `evade`, `critical`, `pierce`, `counter`, `magres`, `atkreduce`, `magatkreduce`, `eft`, `efl`, `efv`, `hppec`, `maxhpdampec` FROM `skill_effect`", effs) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            SkillEffect* ef = new SkillEffect(effs.id);
            if (!ef)
                return false;
            ef->state = effs.state;
            ef->immune = effs.immune;
            ef->disperse = effs.disperse;
            SetValOrPercent(ef->damage, ef->damageP, effs.damage);
            ef->adddam = effs.adddam;
            SetValOrPercent(ef->magdam, ef->magdamP, effs.magdam);
            ef->addmag = effs.addmag;
            SetValOrPercent(ef->crrdam, ef->crrdamP, effs.crrdam);
            ef->addcrr = effs.addcrr;
            SetValOrPercent(ef->hp, ef->hpP, effs.hp);
            ef->addhp = effs.addhp;
            SetValOrPercent(ef->absorb, ef->absorbP, effs.absorb);
            SetValOrPercent(ef->thorn, ef->thornP, effs.thorn);
            SetValOrPercent(ef->inj2hp, ef->inj2hpP, effs.inj2hp);
            SetValOrPercent(ef->aura, ef->auraP, effs.aura);
            SetValOrPercent(ef->atk, ef->atkP, effs.atk);
            SetValOrPercent(ef->def, ef->defP, effs.def);
            SetValOrPercent(ef->magatk, ef->magatkP, effs.magatk);
            SetValOrPercent(ef->magdef, ef->magdefP, effs.magdef);
            ef->tough = effs.tough;
            SetValOrPercent(ef->action, ef->actionP, effs.action);
            ef->hitrate = effs.hitrate;
            ef->evade = effs.evade;
            ef->critical = effs.critical;
            ef->pierce = effs.pierce;
            ef->counter = effs.counter;
            ef->magres = effs.magres;
            ef->atkreduce = effs.atkreduce;
            ef->magatkreduce = effs.magatkreduce;
            {
                StringTokenizer tk(effs.eft, ",");
                if (tk.count())
                {
                    for (size_t i = 0; i < tk.count(); ++i)
                        ef->eft.push_back(::atoi(tk[i].c_str()));
                }
            }
            {
                StringTokenizer tk(effs.efl, ",");
                if (tk.count())
                {
                    for (size_t i = 0; i < tk.count(); ++i)
                        ef->efl.push_back(::atoi(tk[i].c_str()));
                }
            }
            {
                StringTokenizer tk(effs.efv, ",");
                if (tk.count())
                {
                    for (size_t i = 0; i < tk.count(); ++i)
                        ef->efv.push_back(::atof(tk[i].c_str()));
                }
            }
            ef->hppec = effs.hppec;
            ef->maxhpdampec = effs.maxhpdampec;
            skillEffectManager.add(ef);
        }
        return true;
    }

    bool GDataManager::LoadSkills()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBSkill skills;
		if(execu->Prepare("SELECT `id`, `name`, `color`, `target`, `cond`, `prob`, `area`, `factor`, `last`, `cd`, `effectid` FROM `skills`", skills) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            SkillBase* skill = new SkillBase(skills.id, skills.name);
            if (!skill)
                return false;
            skill->color = skills.color;
            skill->target = skills.target;
            skill->cond = skills.cond;
            skill->prob = skills.prob;
            skill->area = skills.area;
            StringTokenizer tk(skills.factor, ",");
            if (tk.count())
            {
                for (size_t i = 0; i < tk.count(); ++i)
                    skill->factor.push_back(::atof(tk[i].c_str()));
            }
            skill->last = skills.last;
            skill->cd = skills.cd;
            skill->effect = skillEffectManager[skills.effectid];
            skillManager.add(skill);
            if(skills.id > 60000)
                m_petEqSkills.push_back(skills.id);
        }
        return true;
    }

    bool GDataManager::LoadCittaEffect()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCittaEffect cf;
		if(execu->Prepare("SELECT `id`, `skills`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`, `auraMax`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `tough`, `action`, `hitrate`, `evade`, `critical`, `criticaldmg`, `pierce`, `counter`, `magres` FROM `citta_effect`", cf) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            CittaEffect* cft = new CittaEffect(cf.id);
            SetValOrPercent(cft->strength, cft->strengthP, cf.strength);
            SetValOrPercent(cft->physique, cft->physiqueP, cf.physique);
            SetValOrPercent(cft->agility, cft->agilityP, cf.agility);
            SetValOrPercent(cft->intelligence, cft->intelligenceP, cf.intelligence);
            SetValOrPercent(cft->will, cft->willP, cf.will);
            SetValOrPercent(cft->soul, cft->soulP, cf.soul);
            SetValOrPercent(cft->aura, cft->auraP, cf.aura);
            SetValOrPercent(cft->auraMax, cft->auraMaxP, cf.auraMax);
            SetValOrPercent(cft->attack, cft->attackP, cf.attack);
            SetValOrPercent(cft->magatk, cft->magatkP, cf.magatk);
            SetValOrPercent(cft->defend, cft->defendP, cf.defend);
            SetValOrPercent(cft->magdef, cft->magdefP, cf.magdef);
            SetValOrPercent(cft->hp, cft->hpP, cf.hp);
			SetValOrPercent(cft->toughlvl, cft->tough, cf.tough);
            cft->tough *= 100;
			SetValOrPercent(cft->action, cft->actionP, cf.action);
			SetValOrPercent(cft->hitrlvl, cft->hitrate, cf.hitrate);
            cft->hitrate *= 100;
			SetValOrPercent(cft->evdlvl, cft->evade, cf.evade);
            cft->evade *= 100;
			SetValOrPercent(cft->crilvl, cft->critical, cf.critical);
            cft->critical *= 100;
			SetValOrPercent(cft->criticaldmg, cft->criticaldmgP, cf.criticaldmg);
            cft->criticaldmg /= 100;
			SetValOrPercent(cft->pirlvl, cft->pierce, cf.pierce);
            cft->pierce *= 100;
			SetValOrPercent(cft->counterlvl, cft->counter, cf.counter);
            cft->counter *= 100;
			SetValOrPercent(cft->mreslvl, cft->magres, cf.magres);
            cft->magres *= 100;
            cft->criticaldmgimmune = 0;

            StringTokenizer tk(cf.skill, ",");
            if (tk.count())
            {
                for (size_t i=0; i<tk.count(); ++i)
                {
                    const SkillBase* skill = skillManager[::atoi(tk[i].c_str())];
                    if (skill)
                        cft->skill.push_back(skill);
                }
            }

            cittaEffectManager.add(cft);
        }
        return true;
    }

    bool GDataManager::LoadCittas()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCitta ct;
		if(execu->Prepare("SELECT `id`, `type`, `name`, `pexp`, `needsoul`, `effectid` FROM `cittas`", ct) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            CittaBase* citta = new CittaBase(ct.id, ct.name);
            citta->type = ct.type;
            citta->pexp = ct.pexp;
            citta->needsoul = ct.needsoul;
            citta->effect = cittaEffectManager[ct.effectid];
            cittaManager.add(citta);
            if (citta->effect)
                makeSkill2Item(citta->effect->skill, CITTA_TO_ITEMID(ct.id));
        }
        return true;
    }

	bool GDataManager::LoadClanTechTable()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		UInt32 accNeeds = 0;
		DBClanTechType cst;
		if (execu->Prepare("SELECT `id`, `level`, `needs`, `clanLev`, `effect1`, `effect2` FROM `clan_tech_template` ORDER BY `id` ASC, `level` ASC", cst) != DB::DB_OK)
			return false;
		while (execu->Next() == DB::DB_OK)
		{
			if (cst.id >= clanTechTable.size())
			{
				accNeeds = 0;
				clanTechTable.resize(cst.id + 1);
			}
			SingleClanTechTable & single = clanTechTable[cst.id];
			if (cst.level >= single.size())
				single.resize(cst.level + 1);
			accNeeds += cst.needs;
			single[cst.level] = ClanTechTableData(cst.id, cst.level, cst.needs, accNeeds, cst.clanLev, cst.effect1, cst.effect2);
		}

		return true;
	}

	bool GDataManager::LoadSoulSkillTable()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBSoulSkillType sst;
		if (execu->Prepare("SELECT `id`, `name`, `level`, `itemId`, `value` FROM `soul_skill_template` ORDER BY `id` ASC, `level` ASC", sst) != DB::DB_OK)
			return false;
		while (execu->Next() == DB::DB_OK)
		{
			if (sst.id >= soulSkillTable.size())
			{
				soulSkillTable.resize(sst.id + 1);
			}
			SingleSoulSkillTable & single = soulSkillTable[sst.id];
			if (sst.level >= single.size())
				single.resize(sst.level + 1);
			single[sst.level] = SoulSkillTableData(sst.id, sst.name, sst.level, sst.itemId, sst.value);
		}

		return true;
	}

	bool GDataManager::LoadClanSkillTable()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBClanSkillType cst;
		if (execu->Prepare("SELECT `id`, `name`, `level`, `needs`, `value` FROM `clan_skill_template` ORDER BY `id` ASC, `level` ASC", cst) != DB::DB_OK)
			return false;
		while (execu->Next() == DB::DB_OK)
		{
			if (cst.id >= clanSkillTable.size())
			{
				clanSkillTable.resize(cst.id + 1);
			}
			SingleClanSkillTable & single = clanSkillTable[cst.id];
			if (cst.level >= single.size())
				single.resize(cst.level + 1);
			single[cst.level] = ClanSkillTableData(cst.id, cst.name, cst.level, cst.needs, cst.value);
		}

		return true;
	}

    bool GDataManager::LoadClanCopy()
    {
        // 读取和帮派副本有关的配置参数
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBClanCopy cc;
		if (execu->Prepare("SELECT `level`, `expOutput`, `monsterWaveCount`, `minPlayer`, `maxPlayer`, `spotMaxPlayer`, `homeHp`, `maxReward` FROM `clan_copy_template` ORDER BY `level` ASC", cc) != DB::DB_OK)
			return false;
		while (execu->Next() == DB::DB_OK)
		{
            if (cc.level >= clanCopyTable.size())
            {
                clanCopyTable.resize(static_cast<size_t>(cc.level + 1));
            }
            clanCopyTable[cc.level] = ClanCopyData(cc.level, cc.expOutput, cc.monsterWaveCount,
                    cc.minPlayer, cc.maxPlayer, cc.spotMaxPlayer, cc.homeHp, cc.maxReward);
		}
        clanCopyTable.push_back(ClanCopyData());

        lua_State* L = lua_open();
        luaopen_base(L);
        luaopen_string(L);
        luaopen_table(L);
        {
            std::string path = cfg.scriptPath + "formula/clancopy.lua";
            lua_tinker::dofile(L, path.c_str());
            lua_tinker::table bufferTypes = lua_tinker::call<lua_tinker::table>(L, "LoadClanCopySpotBufferType");
            UInt32 size = bufferTypes.size();
            for (UInt32 i = 1; i <= size; i++)
            {
                UInt8 bufferType = bufferTypes.get<UInt8>(i);
                lua_tinker::table bufferValues = lua_tinker::call<lua_tinker::table>(L, "LoadClanCopySpotBufferValue", bufferType);
                UInt32 bufferValueSize = bufferValues.size();
                ClanCopySpotData clanCopySpotData(bufferType);
                for (UInt32 j = 1; j <= bufferValueSize; j++)
                {
                    clanCopySpotData.bufferValue.push_back(bufferValues.get<float>(j));
                }
                clanCopySpotMap.insert(std::make_pair(bufferType, clanCopySpotData));
            }
        }
        lua_close(L);

		return true;
    }

    bool GDataManager::LoadClanCopyMonster()
    {
        // 读取和帮派副本怪物配置
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBClanCopyMonster ccs;
		if (execu->Prepare("SELECT `level`, `appearRound`, `npcId`, `npcCount`, `npcRouteIndex`, `npcValue`, `monsterType` FROM `clan_copy_monster_template` ORDER BY `level` ASC, `appearRound` ASC", ccs) != DB::DB_OK) return false;
		while (execu->Next() == DB::DB_OK)
		{
            
            UInt32 key = ((static_cast<UInt32>(ccs.level)) << 16 | ccs.appearRound) << 8 | ccs.npcRouteIndex;
            clanCopyMonsterMap.insert(std::make_pair(key, 
                        ClanCopyMonsterData(ccs.level, ccs.appearRound, ccs.npcId, 
                            ccs.npcCount, ccs.npcRouteIndex, ccs.npcValue, ccs.monsterType)));
		}

		return true;
    }


    bool GDataManager::LoadClanStatue()
    {
        // 读取帮派神像数据
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBClanStatue cs;
		if (execu->Prepare("SELECT `level`, `needExp`, `consumeExp`, \
                    `exHp`, `exAttack`, `exDefend`, `exMagAtk`, `exMagDef`, `exAction`, `exHitRate`\
                    FROM `clan_statue_template` ORDER BY `level` ASC", cs) != DB::DB_OK)
			return false;
		while (execu->Next() == DB::DB_OK)
		{
			if (cs.level >= clanStatueTable.size())
			{
				clanStatueTable.resize(cs.level + 1);
			}
            clanStatueTable[cs.level] = ClanStatueTableData(
                                     cs.level, cs.needExp, cs.consumeExp, 
                                     cs.exHp, cs.exAttack, cs.exDefend, 
                                     cs.exMagAtk, cs.exMagDef, cs.exAction, cs.exHitRate);

		}
        clanStatueTable[0] = (ClanStatueTableData());
        return true;
    }

    bool GDataManager::LoadClanBuilding()
    {
        // 读取帮派建筑数据
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBClanBuilding cb;
		if (execu->Prepare("SELECT `level`, `needExp`, "
                    "`phyAtkValue`, `magAtkValue`, `actionValue`, `hpValue` , `oracleValue`"
                    "FROM `clan_building_template` ORDER BY `level` ASC", cb) != DB::DB_OK)
			return false;
        clanBuildingList.clear();
        clanBuildingList.resize(GData::buildingTypeMax);
		while (execu->Next() == DB::DB_OK)
		{
            {
                // 物攻加成
                ClanBuildingTable& clanBuildingTable = clanBuildingList[GData::buildingTypePhyAtk];
                if (cb.level >= clanBuildingTable.size())
                    clanBuildingTable.resize(cb.level + 1);
                clanBuildingTable[cb.level] = ClanBuildingTableData(GData::buildingTypePhyAtk, cb.level, cb.needExp, cb.phyAtkValue);
            }

            {
                // 法攻加成
                ClanBuildingTable& clanBuildingTable = clanBuildingList[GData::buildingTypeMagAtk];
                if (cb.level >= clanBuildingTable.size())
                    clanBuildingTable.resize(cb.level + 1);
                clanBuildingTable[cb.level] = ClanBuildingTableData(GData::buildingTypeMagAtk, cb.level, cb.needExp, cb.magAtkValue);
            }

            {
                // 身法加成
                ClanBuildingTable& clanBuildingTable = clanBuildingList[GData::buildingTypeAction];
                if (cb.level >= clanBuildingTable.size())
                    clanBuildingTable.resize(cb.level + 1);
                clanBuildingTable[cb.level] = ClanBuildingTableData(GData::buildingTypeAction, cb.level, cb.needExp, cb.actionValue);
            }

            {
                // 生命加成
                ClanBuildingTable& clanBuildingTable = clanBuildingList[GData::buildingTypeHP];
                if (cb.level >= clanBuildingTable.size())
                    clanBuildingTable.resize(cb.level + 1);
                clanBuildingTable[cb.level] = ClanBuildingTableData(GData::buildingTypeHP, cb.level, cb.needExp, cb.hpValue);
            }
            {
                // 神域塔
                ClanBuildingTable& clanBuildingTable = clanBuildingList[GData::buildingTypeOracle];
                if (cb.level >= clanBuildingTable.size())
                    clanBuildingTable.resize(cb.level + 1);
                clanBuildingTable[cb.level] = ClanBuildingTableData(GData::buildingTypeOracle, cb.level, cb.needExp, cb.oracleValue);
            }
		}
        return true;
    }

	bool GDataManager::LoadFighterProb()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBFighterProb dbfp;
		if(execu->Prepare("SELECT `id`, `free`, `gold` FROM `fighter_prob`", dbfp) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            FighterProb& fp = fighterProb[dbfp.id];
            fp.free = dbfp.free;
            fp.gold = dbfp.gold;
        }
        return true;
    }

    bool GDataManager::LoadCopyData()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBCopy dbc;
		if(execu->Prepare("SELECT `id`, `floor`, `spot`, `fighterId` FROM `copy` ORDER BY `id`,`floor`,`spot`", dbc) != DB::DB_OK)
			return false;

        bool nextcopy = false;
        bool first = true;
        int id = 0;
        int floor = 0;
		while(execu->Next() == DB::DB_OK)
		{
            if (!first && id != dbc.id)
                nextcopy = true;

            if (nextcopy) {
                copyMaxManager[id] = floor;
                nextcopy = false;
            }

            std::vector<UInt32>& cpv = copyManager[dbc.id<<8|dbc.floor];
            if (cpv.size() <= dbc.spot)
                cpv.resize(dbc.spot+1, 0);
            cpv[dbc.spot] = dbc.fighterId;

            id = dbc.id;
            floor = dbc.floor;
            first = false;
        }
        copyMaxManager[id] = floor;
        return true;
    }

    bool GDataManager::LoadFrontMapData()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBFrontMap dbc;
		if(execu->Prepare("SELECT `id`, `spot`, `count`, `fighterId` FROM `frontmap` WHERE `id` < 100 ORDER BY `id`,`spot`", dbc) != DB::DB_OK)
			return false;

        bool nextfrontmap = false;
        bool first = true;
        int id = 0;
        int spot = 0;
		while(execu->Next() == DB::DB_OK)
		{
            if(dbc.id > 100)
                break;
            
            if (!first && id != dbc.id)
                nextfrontmap = true;

            if (nextfrontmap) {
                frontMapMaxManager[id] = spot;
                nextfrontmap = false;
            }

            std::vector<FrontMapFighter>& cpv = frontMapManager[dbc.id];
            if (cpv.size() <= dbc.spot)
                cpv.resize(dbc.spot+1);
            cpv[dbc.spot].count = dbc.count;
            cpv[dbc.spot].fighterId = dbc.fighterId;

            id = dbc.id;
            spot = dbc.spot;
            first = false;
        }
        frontMapMaxManager[id] = spot;
        return true;
    }

    bool GDataManager::LoadXJFrontMapData()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBFrontMap dbc;
		if(execu->Prepare("SELECT `id`, `spot`, `count`, `fighterId` FROM `frontmap` WHERE `id` > 100 ORDER BY `id`,`spot`", dbc) != DB::DB_OK)
			return false;

        bool nextfrontmap = false;
        bool first = true;
        int id = 0;
        int spot = 0;
		while(execu->Next() == DB::DB_OK)
		{
            if(dbc.id < 100)
                break;
            if (!first && id != dbc.id)
                nextfrontmap = true;

            if (nextfrontmap) {
                xjfrontMapMaxManager[id] = spot;
                nextfrontmap = false;
            }

            std::vector<FrontMapFighter>& cpv = xjfrontMapManager[dbc.id];
            if (cpv.size() <= dbc.spot)
                cpv.resize(dbc.spot+1);
            cpv[dbc.spot].count = dbc.count;
            cpv[dbc.spot].fighterId = dbc.fighterId;

            id = dbc.id;
            spot = dbc.spot;
            first = false;
        }
        xjfrontMapMaxManager[id] = spot;
        return true;
    }

    bool GDataManager::LoadOnlineAwardData()
    {
        lua_State* L = lua_open();
        luaopen_base(L);
        luaopen_string(L);
        luaopen_table(L);
        {
            std::string path = cfg.scriptPath + "items/onlineAward.lua";
            lua_tinker::dofile(L, path.c_str());

            for (int i = 1; i <= 3; ++i)
            {
                lua_tinker::table award = lua_tinker::call<lua_tinker::table>(L, "GetOnlineAward", i);
                std::vector<std::vector<UInt16> >& vecs = m_OnlineAward[i-1];
                UInt8 size = award.size();
                vecs.resize(size);
                for (int j = 1; j <= size; ++j) {
                    lua_tinker::table items = award.get<lua_tinker::table>(j);
                    for (int k = 1; k <= items.size(); ++k) {
                        vecs[j-1].push_back(items.get<UInt16>(k));
                    }
                }
            }

            lua_tinker::table oltime = lua_tinker::call<lua_tinker::table>(L, "GetOnlineAwardTime");
            for (int i = 1; i <= oltime.size(); ++i) {
                m_OnlineAwardTime.push_back(oltime.get<UInt16>(i));
            }
        }
        lua_close(L);
        return true;
    }

	bool GDataManager::LoadFormationData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBFormation dbf;
		if(execu->Prepare("SELECT `id`, `name`, `battlePoint`, `grid1`, `prop1`, `grid2`, `prop2`, `grid3`, `prop3`, `grid4`, `prop4`, `grid5`, `prop5`, `levelup_item` FROM `formation`", dbf) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			Formation * form = new Formation(dbf.id, dbf.name);
            Formation::GridEffect effect;
            const AttrExtraItem* ae = NULL;

            form->setBattlePoint(dbf.battlePoint);
            ae = attrExtraManager[dbf.prop1];
            effect.pos = dbf.grid1;
            if (ae)
                effect.attrExtra = *ae;
            form->addGrid(effect);

            ae = attrExtraManager[dbf.prop2];
            effect.pos = dbf.grid2;
            if (ae)
                effect.attrExtra = *ae;
            form->addGrid(effect);

            ae = attrExtraManager[dbf.prop3];
            effect.pos = dbf.grid3;
            if (ae)
                effect.attrExtra = *ae;
            form->addGrid(effect);

            ae = attrExtraManager[dbf.prop4];
            effect.pos = dbf.grid4;
            if (ae)
                effect.attrExtra = *ae;
            form->addGrid(effect);

            ae = attrExtraManager[dbf.prop5];
            effect.pos = dbf.grid5;
            if (ae)
                effect.attrExtra = *ae;
            form->addGrid(effect);

            if (dbf.levelup_item.length())
            {
				StringTokenizer tk(dbf.levelup_item, ",");
				size_t count = tk.count();
                for(size_t idx = 0; idx < count; ++ idx)
                {
                    form->addLevUpItem(static_cast<UInt32>(atoi(tk[idx].c_str())));
                }
            }

			formationManager.add(form);
		}
		return true;
	}

    bool GDataManager::LoadEUpgradeData()
    {
         std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
          if (execu.get() == NULL || !execu->isConnected()) return false;

          DBEUpgrade dbeu;
          if(execu->Prepare("SELECT `eqid`, `stuff`,`targetid`  FROM `eupgrade`", dbeu) != DB::DB_OK)
            return false;

        while(execu->Next() == DB::DB_OK)
        {
            StringTokenizer tk(dbeu.stfs, "|");
            size_t cnt = tk.count();
            if(cnt == 0)
                continue;

           stEUpgradeItem& item = eUpgradeTable[dbeu.id];
           item.toId = dbeu.toId;

            for(size_t j = 0; j < cnt; ++ j)
            {
                StringTokenizer tk2(tk[j], ",");
                size_t tcnt = tk2.count();
                if(tcnt < 2)
                    continue;
                stUseItem itm;
                itm.id = atoi(tk2[0].c_str());
                itm.num = atoi(tk2[1].c_str());
                item.stfs.push_back(itm);
            }

       }
        return true;
    }
	bool GDataManager::LoadLootData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBLootItem dbli;
		if(execu->Prepare("SELECT `id`, `itemId`, `isPack`, `bind` FROM `loot`", dbli) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			LootItem li;
			li.isPack = dbli.isPack;
			li.m_bind = dbli.bind > 0;
			if(li.isPack == 1)
			{
				StringTokenizer tk(dbli.table, "|");
				size_t cnt = tk.count();
				if(cnt == 0)
					continue;
				for(size_t j = 0; j < cnt; ++ j)
				{
					StringTokenizer tk2(tk[j], ",");
					size_t tcnt = tk2.count();
					if(tcnt < 2)
						continue;
					LootItem::LootData ld;
					ld.id = atoi(tk2[0].c_str());
					if(ld.id == 0)
						continue;
					ld.chance = atoi(tk2[1].c_str());
					if(ld.chance == 0)
						continue;
					if(tcnt > 2)
					{
						for(size_t k = 2; k < tcnt; ++ k)
						{
							LootItem::LootCount lcnt;
							lcnt.count = atoi(tk2[k].c_str());
							++ k;
							if(lcnt.count == 0)
								continue;
							if(k < tcnt)
								lcnt.chance = atoi(tk2[k].c_str());
							else
								lcnt.chance = 10000;
							ld.counts.push_back(lcnt);
						}
					}
					li.items.push_back(ld);
				}
			}
            else if (li.isPack == 2)
            {
				StringTokenizer tk(dbli.table, "|");
				size_t cnt = tk.count();
				if(cnt == 0)
					continue;
				for(size_t j = 0; j < cnt; ++ j)
                {
					StringTokenizer tk2(tk[j], ",");
					size_t tcnt = tk2.count();
                    if (tcnt < 2)
                        continue;
                    LootItem::LootData ld; // ld.chance for chance, ld.counts.count for id
                    for (size_t k = 0; k < tcnt; ++k)
                    {
                        if (k == tcnt - 1)
                            ld.chance = atoi(tk2[k].c_str());
                        else
                        {
                            LootItem::LootCount lcnt;
                            lcnt.count = atoi(tk2[k].c_str());
                            lcnt.chance = 0;
                            ld.counts.push_back(lcnt);
                        }
                    }
                    li.items.push_back(ld);
                }
            }
			else
			{
				StringTokenizer tk(dbli.table, ",");
				size_t cnt = tk.count();
				if(cnt == 0)
					continue;
				for(size_t j = 0; j < cnt; ++ j)
				{
					LootItem::LootData ld;
					ld.id = atoi(tk[j].c_str());
					if(ld.id > 0)
						li.items.push_back(ld);
				}
			}
			if(!li.items.empty())
				lootTable.add(dbli.id, li);
		}

		return true;
	}

    bool GDataManager::LoadDreamer()
    {
        // 读取水晶梦境配置
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBDreamer dbd;
		if(execu->Prepare("SELECT `level`, `floor`, `maxX`, `maxY`, `gridCount`, `timeConsume`, `typeCount` FROM `dreamer_template` ORDER BY `level`,`floor`", dbd) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            DreamerLevelData dreamerLevelData(dbd.maxX, dbd.maxY, dbd.gridCount, dbd.timeConsume, dbd.typeCount);
			if (dbd.level >= dreamerDataTable.size())
			{
				dreamerDataTable.resize(dbd.level + 1);
			}
            if (dbd.floor >= dreamerDataTable[dbd.level].size())
            {
                dreamerDataTable[dbd.level].resize(dbd.floor + 1);
                dreamerDataTable[dbd.level].resize(dbd.floor + 1);
            }
            dreamerDataTable[dbd.level][dbd.floor] = dreamerLevelData;
        }
        return true;
    }

    bool GDataManager::LoadPetPinJie()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBPinJie dbpj;
		if(execu->Prepare("SELECT `id`, `name`, `consume`, `prob`, `skillLev`, `bless` FROM `pet_pinjie`", dbpj) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            pet.setLevTable(dbpj);
        }
        return true;
    }

    bool GDataManager::LoadPetGenGu()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBGenGu dbgg;
		if(execu->Prepare("SELECT `id`, `name`, `limit`, `baseProb`, `failBack`, `consume1`, `consume2`, `growRate` FROM `pet_gengu`", dbgg) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            Pet::GenguData ggd;
            ggd.id = dbgg.id;
            ggd.name = dbgg.name;
            ggd.limit = dbgg.limit;
            ggd.baseProb = dbgg.baseProb;
            ggd.failBack = dbgg.failBack;
            ggd.consume1 = dbgg.consume1;
            ggd.consume2 = dbgg.consume2;
            ggd.growRate = dbgg.growRate;
            pet.setBoneTable(ggd);
        }
        return true;
    }

    bool GDataManager::LoadPetLingYa()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBLingYa dbly;
		if(execu->Prepare("SELECT `id`, `color`, `lingya`, `initBone`, `finalBone` FROM `pet_pressure`", dbly) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            Pet::LingyaData lyd;
            lyd.petId = dbly.id;
            lyd.color = dbly.color;
            lyd.lingya = dbly.lingya;
            lyd.initBone = dbly.initBone;
            lyd.finalBone = dbly.finalBone;
            pet.setLingyaTable(lyd);
        }
        return true;
    }

    bool GDataManager::LoadPetEquipExp()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBPetEqExp dbeq;
		if(execu->Prepare("SELECT `id`, `green`, `blue`, `purple`, `yellow`, `skillLev` FROM `pet_levelup`", dbeq) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            if(dbeq.id == 0)
                continue;
            Pet::EquipExpData eqd;
            eqd.level = dbeq.id;
            eqd.levExp[0] = dbeq.green;
            eqd.levExp[1] = dbeq.blue;
            eqd.levExp[2] = dbeq.purple;
            eqd.levExp[3] = dbeq.yellow;
            eqd.skillLev = dbeq.skillLev;
            pet.setEqExpTable(eqd);
        }
        return true;
    }

    bool GDataManager::LoadPetEqAttreffect()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBPetEqAttreffect dbeqAttr;
		if(execu->Prepare("SELECT `id`, `hp`, `atk`, `magatk`, `def`, `magdef`, `critical`, `pierce`, `hitrate`, `evade`, `counter`, `tough` FROM `pet_neidan`", dbeqAttr) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            if(dbeqAttr.id == 0)
                continue;
            Pet::EquipAttreffect eqAttr;
            eqAttr.level = dbeqAttr.id;
            eqAttr.effect[0] = dbeqAttr.hp;
            eqAttr.effect[1] = dbeqAttr.atk;
            eqAttr.effect[2] = dbeqAttr.magatk;
            eqAttr.effect[3] = dbeqAttr.def;
            eqAttr.effect[4] = dbeqAttr.magdef;
            eqAttr.effect[5] = dbeqAttr.critical;
            eqAttr.effect[6] = dbeqAttr.pierce;
            eqAttr.effect[7] = dbeqAttr.hitrate;
            eqAttr.effect[8] = dbeqAttr.evade;
            eqAttr.effect[9] = dbeqAttr.counter;
            eqAttr.effect[10] = dbeqAttr.tough;
            pet.setEqAttreffect(eqAttr);
        }
        return true;
    }

    bool GDataManager::LoadXingchenConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBXingchenConfig dbxcc;
		if(execu->Prepare("SELECT `id`, `limitLev`, `name`, `consume`, `maxVal`, `rate1`, `rate2`, `rate3`, `payBack` FROM `xingchen`", dbxcc) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            if(dbxcc.id == 0)
                continue;
            XingchenData::stXingchen stxc;
            stxc.level = dbxcc.id;
            stxc.limitLev = dbxcc.limitLev;
            stxc.name  = dbxcc.name;
            stxc.consume = dbxcc.consume;
            stxc.maxVal = dbxcc.maxVal;
            stxc.rate1 = dbxcc.rate1;
            stxc.rate2 = dbxcc.rate2;
            stxc.rate3 = dbxcc.rate3;
            stxc.payBack = dbxcc.payBack;
            xingchenData.setXingchenTable(stxc);
        }
        return true;
    }
    
    bool GDataManager::LoadJiguanshuConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBJiguanshuConfig dbjgs;
		if(execu->Prepare("SELECT `jgshuLvl`, `totalNeedExp`, `needExp`, `attrValue1`, `attrValue2`, `attrValue3`, `attrValue4`  FROM `jiguanshu`", dbjgs) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            jiguanData.setJiguanshuInfo(dbjgs);
        }

        return true;
    }

    bool GDataManager::LoadJiguanyuConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBJiguanyuConfig dbjgy;
		if(execu->Prepare("SELECT `jiguanyuId`, `mark`, `name`, `quality`, `attrType`, `element`, `attrValue`, `molding`, `skillId` FROM `jiguanyu`", dbjgy) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            jiguanData.setJiguanyuInfo(dbjgy);
        }

        return true;
    }

    bool GDataManager::LoadTuzhiConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBTuzhiConfig dbtz;
		if(execu->Prepare("SELECT `tuzhiId`, `name`, `quality`, `needjgsLvl`, `jiguanyuId` FROM `tuzhi`", dbtz) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            jiguanData.settuzhiInfo(dbtz);
        }

        return true;
    }

    bool GDataManager::LoadKeyinConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBKeyinConfig dbky;

		if(execu->Prepare("SELECT `keyinId`, `lvl`, `name`, `quality`, `attrType`, `attrValue`, `materialA`, `materialB`, `maxValue` FROM `signet`", dbky) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            jiguanData.setkeyinInfo(dbky);
        }

        return true;
    }

    bool GDataManager::LoadZhenweiConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBZhenweiConfig dbzw;

		if(execu->Prepare("SELECT `keyId`, `name`, `collectType`, `collect1`, `collect2`, `collect3`, `collect4`, `collect5`, `collect6`, `collect7`, `collect8`, `award` FROM `zhenwei`", dbzw) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            jiguanData.setZhenweiInfo(dbzw);
        }

        return true;
    }

    bool GDataManager::LoadGearConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBGearConfig dbg;

		if(execu->Prepare("SELECT `partId`, `name`, `needMoneyA`, `needMoneyB`, `needMoneyC`, `needMoneyD`, `needMoneyE`, `attrValueA`, `attrValueB`, `attrValueC`, `attrValueD`, `attrValueE`, `attrValueF`, `attrValueG` FROM `gear`", dbg) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            jiguanData.setGearInfo(dbg);
        }

        return true;
    }

    bool GDataManager::LoadGearTreeConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBGearTreeConfig dbgt;

		if(execu->Prepare("SELECT `suiteId`, `lastSuiteId`, `partAId`, `componentAId`, `componentBId`, `partBId`, `componentCId`, `componentDId`, `partCId`, `componentEId`, `componentFId` FROM `geartree`", dbgt) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            jiguanData.setGearTreeInfo(dbgt);
        }

        return true;
    }

    bool GDataManager::LoadSanHunConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBSanHunConfig dbsh;
		if(execu->Prepare("SELECT `lvl`, `attr1`, `attr2`, `attr3`, `attr4`, `attr5`, `attr6`, `attr7`, `attr8`, `attr9`, `money1`, `money2` FROM `pet_sanhun`", dbsh) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            hunpoData.setSanHunInfo(dbsh);
        }

        return true;
    }

    bool GDataManager::LoadErlkingConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBErlkingConfig db;

		if(execu->Prepare("SELECT `copyId`, `conditionA`, `conditionB`, `npcgroup` FROM `erlking`", db) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            erlkingData.setErlkingInfo(db);
        }

        return true;
    }

    bool GDataManager::LoadNewQuestionsConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBNewQuestionsConfig db;

		if(execu->Prepare("SELECT `id`, `lvl`, `answer` FROM `newquestions`", db) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            newquestionsData.setNewQuestionsInfo(db);
        }

        return true;
    }

	bool GDataManager::LoadMoney()
	{
			lua_State* L = lua_open();
			luaopen_base(L);
			luaopen_string(L);
			luaopen_table(L);
			{
				std::string path = cfg.scriptPath + "formula/money.lua";
                lua_tinker::dofile(L, path.c_str());

                lua_tinker::table money = lua_tinker::call<lua_tinker::table>(L, "GetMoney");
                size_t size = money.size();
                moneyNeed.resize(GData::MONEY_MAX);
                for (int i = 1; i < GData::MONEY_MAX && i <= (int)size; ++i)
                {
                    lua_tinker::table needmoney = money.get<lua_tinker::table>(i);
                    Money m;
                    m.gold = needmoney.get<int>(1);
                    m.tael = needmoney.get<int>(2);
                    m.ticket = needmoney.get<int>(3);
                    moneyNeed[i] = m;
                }
            }
            lua_close(L);
            return true;
    }

    bool GDataManager::LoadHeroMemoMaxSoul()
    {
			lua_State* L = lua_open();
			luaopen_base(L);
			luaopen_string(L);
			luaopen_table(L);
			{
				std::string path = cfg.scriptPath + "World/HeroMemo.lua";
                lua_tinker::dofile(L, path.c_str());
                lua_tinker::table soul = lua_tinker::call<lua_tinker::table>(L, "getHeroMemoMaxSoul");
                for (int i = 1; i <= soul.size(); ++i)
                    GObject::HeroMemo::addMaxSoul(soul.get<UInt16>(i));
            }
            lua_close(L);
            return true;
    }

	const TaskType& GDataManager::GetTaskTypeData(UInt32 typeId)
	{
		static TaskType null;

		TaskTypeList::const_iterator cit = m_TaskTypeList.find(typeId);
		return cit != m_TaskTypeList.end() ? cit->second : null;
	}

	const TaskTypeRelation& GDataManager::GetTaskTypeRelationData(UInt32 typeId)
	{
		static TaskTypeRelation null;

		TaskTypeRelationList::const_iterator cit = m_TaskTypeRelationList.find(typeId);
		return cit != m_TaskTypeRelationList.end() ? cit->second : null;
	}

	const std::set<UInt32>& GDataManager::GetTaskNpcRelationData(UInt32 npcId)
	{
		static std::set<UInt32> null;

		TaskNpcRelationList::const_iterator cit = m_TaskNpcRelationList.find(npcId);
		return cit != m_TaskNpcRelationList.end() ? cit->second : null;
	}

	//?ճ?????
	const std::set<UInt32>& GDataManager::GetTaskDayData(UInt32 taskId)
	{
		static std::set<UInt32> null;

		TaskDayList::const_iterator cit = m_TaskDayList.find(taskId);
		return cit != m_TaskDayList.end() ? cit->second : null;
	}

	const std::set<UInt32>& GDataManager::GetTaskLevRelation(UInt16 lev)
	{
		static std::set<UInt32> null;

		TaskLevRelationList::const_iterator cit = m_TaskLevRelationList.find(lev);
		return cit != m_TaskLevRelationList.end() ? cit->second : null;
	}

	const ItemBaseType* GDataManager::GetItemTypeData(UInt32 itemId)
	{
		return itemBaseTypeManager[itemId];
	}

	GObject::ItemWeapon* GDataManager::GetNpcWeapon( UInt32 wpId )
	{
		return const_cast<GObject::ItemWeapon *>(npcWeapons[wpId]);
	}

	const std::vector<UInt32>& GDataManager::GetSaleItemSortRule(UInt8 type)
	{
		static std::vector<UInt32> null;
		return type >= 13 ? null : m_SaleItemsSortRule[type];
	}

	const std::vector<UInt32>& GDataManager::GetTaelTrainList()
	{
		return m_TaelTrainList;
	}

	const std::vector<UInt32>& GDataManager::GetGoldTrainList()
	{
		return m_GoldTrainList;
	}

	const std::vector<UInt32>& GDataManager::GetLevelTrainExp()
	{
		return m_LevelTrainExp;
	}

	const std::vector<UInt32>& GDataManager::GetTaelPractice()
	{
		return m_TaelPractice;
	}

	const std::vector<UInt32>& GDataManager::GetGoldPractice()
	{
		return m_GoldPractice;
	}

	const std::vector<UInt32>& GDataManager::GetGoldOpenSlot()
	{
		return m_GoldOpenSlot;
	}

	const std::vector<UInt32>& GDataManager::GetPlaceAddons()
	{
		return m_PlaceAddons;
	}

	const std::vector<UInt32>& GDataManager::GetShiMenTask(int country)
	{
		return m_ShiMenTask[country];
	}

	const std::vector<UInt32>& GDataManager::GetYaMenTask(int country)
	{
		return m_YaMenTask[country];
	}

    const std::vector<UInt32>& GDataManager::GetShiYaMenTask(int country, int type)
    {
        if (type >= 1)
            return m_YaMenTask[2];
        return m_ShiMenTask[country];
    }

    const std::vector<UInt32>& GDataManager::GetClanTask()
    {
        return m_ClanTask;
    }

    const std::vector<UInt8>& GDataManager::GetFlushTaskFactor(int ttype, int ftype)
    {
        return m_FlushTaskFactor[ttype][ftype];
    }

    const std::vector<UInt32>& GDataManager::GetFlushBookFactor(int type)
    {
        return m_BookFactor[type];
    }

    const std::vector<UInt32>& GDataManager::GetFlushBookPrice()
    {
        return m_BookPrice;
    }

    UInt32 GDataManager::GetTaskAwardFactor(int ttype, int color)
    {
        return m_TaskAwardFactor[ttype][color];
    }

    UInt32 GDataManager::GetTripodAward(int fire, int quality)
    {
        return m_TripodAward[fire][quality];
    }

    const std::vector<UInt16>& GDataManager::GetOnlineAward(UInt8 cls, UInt8 i)
    {
        static std::vector<UInt16> null;
        std::vector<std::vector<UInt16> >& vecs = m_OnlineAward[cls-1];
        if (i >= m_OnlineAward[cls-1].size())
            return null;
        return vecs[i];
    }

    UInt16 GDataManager::GetOnlineAwardTime(UInt8 i)
    {
        if (i > m_OnlineAwardTime.size())
            return 0;
        return m_OnlineAwardTime[i];
    }

    UInt8 GDataManager::GetOnlineAwardCount()
    {
        return m_OnlineAwardTime.size();
    }

    //仙宠相关
    UInt16 GDataManager::GetPetEqTypeIdByColor(int colorIdx)
    {
        if(colorIdx < 0 || colorIdx >= 4)
            return 0;
        size_t eqCnt = m_petEqs[colorIdx].size();
        if(eqCnt == 0)
            return 0;
        return m_petEqs[colorIdx][uRand(eqCnt)];
    }

    UInt16 GDataManager::GetPetGemTypeIdByLev(int lvIdx)
    {
        if(lvIdx < 0 || lvIdx >= 20)
            return 0;
        size_t gemCnt = m_petGems[lvIdx].size();
        if(gemCnt == 0)
            return 0;
        return m_petGems[lvIdx][uRand(gemCnt)];
    }

    UInt16 GDataManager::GetZhenyuanTypeIdByLev(int lvIdx)
    {
        if(lvIdx < 0 || lvIdx >= 20)
            return 0;
        size_t cnt = m_zhenyuanItem[lvIdx].size();
        if(cnt == 0)
            return 0;
        return m_zhenyuanItem[lvIdx][uRand(cnt)];
    }

    UInt16 GDataManager::GetPetEqSkill()
    {
        /*
        size_t cnt = m_petEqSkills.size();
        if(cnt == 0)
            return 0;
        return m_petEqSkills[uRand(cnt)];
        */
        static UInt16 petEqSkills[] = { 60001, 60101, 60201, 60301, 60401, 60501, 60601, 60701, 60801, 60901, 61001, 61101, 61201, 61301, 61401, 61501 };
        static UInt16 chance[] = { 100, 200, 300, 1200, 2100, 3000, 3900, 4800, 5600, 6400, 7200, 8000, 8500, 9000, 9500, 10000 };
        UInt16 rnd = uRand(10000);
        for(UInt8 i = 0; i < sizeof(chance)/sizeof(UInt16); ++ i)
        {
            if(rnd < chance[i])
                return petEqSkills[i];
        }
        return 0;
    }

    bool GDataManager::LoadSpiritAttrTable()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSpiritAttr dbsa;
		if(execu->Prepare("SELECT `level`, `attack`, `defend`, `critical`, `pierce`, `action`, `tough`, `criticaldmg`, `hp` FROM `spirit_attr` order by `level` asc", dbsa) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            SpiritAttr sa;
            sa.attack = dbsa.attack;
            sa.defend = dbsa.defend;
            sa.critical_lvl = dbsa.critical;
            sa.pierce_lvl = dbsa.pierce;
            sa.action = dbsa.action;
            sa.tough_lvl = dbsa.tough;
            sa.critical_dmg = dbsa.criticaldmg;
            sa.hp = dbsa.hp;

            spiritAttrTable.push_back(sa);
        }
        return true;
    }

    bool GDataManager::LoadSoulItemExp()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSoulItemExp dbsie;
		if(execu->Prepare("SELECT `itemId`, `exp` FROM `soul_item_exp`", dbsie) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            m_soulItemExp[dbsie.itemId] = dbsie.exp;
        }
        return true;
    }

    void GDataManager::clearUdpItem()
    {
        m_udpLogItems.clear();
    }

    void GDataManager::addUdpItem(UInt32 id)
    {
        m_udpLogItems.push_back(id);
    }

    bool GDataManager::isInUdpItem(UInt32 id)
    {
        UInt32 size = m_udpLogItems.size();
        for (UInt32 i = 0; i < size; ++i)
        {
            if (m_udpLogItems[i] == id)
                return true;
        }
        return false;
    }

    bool GDataManager::LoadSkillStrengthenExp()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSSExp ssexp;
		if(execu->Prepare("SELECT `id`, `exp1`, `exp2`, `exp3`, `exp4`, `exp5`, `exp6`, `exp7`, `exp8`, `exp9` FROM `skillstrengthen`", ssexp) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            m_skillstrengthexp[ssexp.id].resize(9);
            UInt32* pv = &ssexp.exp1;
            for (UInt8 i = 0; i < 9; ++i)
                m_skillstrengthexp[ssexp.id][i] = *pv++;;
        }
        return true;
    }
    bool GDataManager::LoadSkillStrengthenProb()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
        DBSSProb ssprob;
		if(execu->Prepare("SELECT `id`, `prob1`, `prob2`, `prob3`, `prob4`, `prob5`, `prob6`, `prob7`, `prob8`, `prob9` FROM `skillstrengthenprob`", ssprob) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            m_skillstrengthprob[ssprob.id].resize(9);
            UInt32* pv = &ssprob.prob1;
            for (UInt8 i = 0; i < 9; ++i)
                m_skillstrengthprob[ssprob.id][i] = *pv++;;
        }
        return true;
    }

    bool GDataManager::LoadTeamArenaSkillConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBTeamArenaSkillConfig dbtasc;
		if(execu->Prepare("SELECT `id`, `level`, `needs`, `teamLev`, `attack`, `hp`, `defend`, `magdef`, `action`, `magresP` FROM `team_arena_skill`", dbtasc) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            if(dbtasc.id > 0)
                TeamArenaConfigMgr::LoadSkillFromDB(dbtasc);
        }
        return true;
    }

    bool GDataManager::LoadTeamArenaInspireConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBTeamArenaInspireConfig dbtaic;
		if(execu->Prepare("SELECT `level`, `consume`, `rate`, `attackP`, `defendP`,`hpP`, `actionP` FROM `team_arena_inspire`", dbtaic) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            if(dbtaic.level > 0)
                TeamArenaConfigMgr::LoadInspireFromDB(dbtaic);
        }
        return true;
    }

    bool GDataManager::LoadRideConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBRideConfig dbrc;
		if(execu->Prepare("SELECT `id`, `name`, `itemId`, `chips`, `propId`, `show`, `useMore` FROM `ride`", dbrc) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            if(dbrc.id > 0)
                ride.setRideTable(dbrc);
        }
        return true;
    }

    bool GDataManager::LoadRideUpgradeConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBRideUpgradeCfg dbruc;
		if(execu->Prepare("SELECT `level`, `name`, `lvLimit`, `singleCost`, `lvExp`, `rate` FROM `ride_upgrade`", dbruc) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            ride.setRideUpgradeTable(dbruc);
        }
        return true;
    }

    bool GDataManager::LoadCangjianConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBCangjianCfg dbcjc;
		if(execu->Prepare("SELECT `floor`, `name`, `prob`, `otherNum`, `bless` FROM `ride_cangjian`", dbcjc) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            ride.setCangjianTable(dbcjc);
        }
        return true;
    }

    UInt32 GDataManager::getMaxStrengthenVal(UInt16 id, UInt8 clvl)
    {
        std::map<UInt16, std::vector<UInt32> >::iterator i = m_skillstrengthexp.find(id);
        if (i != m_skillstrengthexp.end())
        {
            if (i->second.size() > clvl)
                return i->second[clvl];
        }
        return 0;
    }
    UInt32 GDataManager::getSkillStrengthenProb(UInt16 id, UInt8 clvl)
    {
        std::map<UInt16, std::vector<UInt32> >::iterator i = m_skillstrengthprob.find(id);
        if (i != m_skillstrengthprob.end())
        {
            if (i->second.size() > clvl)
                return i->second[clvl];
        }
        return 0;
    }

    bool GDataManager::LoadPetSevenSoulLevel()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBPetSevenSoulLevel dbvalue;
		if(execu->Prepare("SELECT `soullevel`, `needsoulnum`, `skilllevel` FROM `pet_sevensoullevel`", dbvalue) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
            sevenSoul.loadSevenSoulLevel(dbvalue.soullevel, dbvalue.needsoulnum, dbvalue.skilllevel);

		return true;
	}

    bool GDataManager::LoadPetSevenSoulUpgrade()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBPetSevenSoulUpgrade dbvalue;
		if(execu->Prepare("SELECT `pettype`, `soulid`, `skillstr`, `condionvalue` FROM `pet_sevensoulupgrade`", dbvalue) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
            sevenSoul.loadSevenSoulUpgrade(dbvalue.pettype, dbvalue.soulid, dbvalue.skillstr, dbvalue.condionvalue);

		return true;
    }

    bool GDataManager::LoadCoupleInfo()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCoupleInfo dbpn;
		if(execu->Prepare("SELECT `level` ,`levelExp`, `hp`, `attak`, `magic_attak`, `df_critical`, `action` FROM `coupleinfo` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
            cu.loadUpgradeTable(dbpn); 
		return true;
    }

    bool GDataManager::LoadCoupleCopy()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCoupleCopy dbcc;
		if(execu->Prepare("SELECT `id` ,`name`, `location`, `monster` FROM `couplecopy` ", dbcc) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
            cc.loadCopyTable(dbcc); 
		return true;
    }

    bool GDataManager::LoadXinMoConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBXinMoConfig dbxcc;
		if(execu->Prepare("SELECT `id`, `limitLev`, `name`, `consume`, `maxVal`, `attack`, `hp`, `action`,`cridec` ,`skilllev`,`payBack` FROM `xinmo`", dbxcc) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            XinMoData::stXinMo stxc;
            stxc.level = dbxcc.id;
            stxc.limitLev = dbxcc.limitLev;
            stxc.name  = dbxcc.name;
            stxc.consume = dbxcc.consume;
            stxc.maxVal = dbxcc.maxVal;
            stxc.attack = dbxcc.attack;
            stxc.hp = dbxcc.hp;
            stxc.action = dbxcc.action;
            stxc.cridec = dbxcc.cridec;
            stxc.skilllev = dbxcc.skilllev;
            stxc.payBack = dbxcc.payBack;
            xinmoData.setXinMoTable(stxc);
        }
        return true;
    }
    bool GDataManager::LoadDrinkAttrConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBDrinkAttrConfig dbda;
		if(execu->Prepare("SELECT `value`, `hp`  FROM `drinkAttr`", dbda) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            DrinkAttr::stDrinkAttr da;
            da.hp = dbda.hp;
            drinkAttrData.setDrinkAttrTable(dbda.value , da);
        }
        return true;
    }
    bool GDataManager::LoadIncenseAttrConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBIncenseAttrConfig  dbda;
		if(execu->Prepare("SELECT `val`, `up`  FROM `incenseAttr`", dbda) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            if(dbda.val)
                incenseData.setIncenseAttrTable(dbda.val , dbda.up);
        }
        return true;
    }

    bool GDataManager::LoadSkillEvConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBSkillEv dbskillev;
		if(execu->Prepare("SELECT `lev`, `effect`, `consume`, `needLev` FROM `skill_ev`", dbskillev) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            SkillEvData::stSkillEv skillEv;
            skillEv.effect = dbskillev.effect;
            skillEv.consume = dbskillev.consume;
            skillEv.needLev = dbskillev.needLev;
            GData::skillEvData.setSkillEvData(dbskillev.lev, skillEv);
        }
        return true;
    }

    bool GDataManager::LoadRandBattleConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBRandBattleAttr dbattr;
		if(execu->Prepare("SELECT `lev`, `id`, `value`, `next` FROM `randbattle_attr`", dbattr) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            RandBattleData::stRandBattle randBattle;
            randBattle.id = dbattr.id;
            randBattle.value = dbattr.value;
            randBattle.next = dbattr.next;
            GData::randBattleData.setRandBattleData(dbattr.lev, randBattle);
        }
        return true;
    }

    bool GDataManager::LoadLingShiConfig()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        DBLingShi dbls;
		if(execu->Prepare("SELECT `level`, `consume`, `exp`, `isBreak`, `useItem`, `useGold`, `attack`, `magatk`, `hp`, `action` FROM `lingshi_level`", dbls) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
            if(dbls.level > 0)
                lingshiCls.setLingshiTable(dbls);
        }
        return true;
    }

    bool GDataManager::LoadCardUpgrade()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCardUpgrade dbpn;
		if(execu->Prepare("SELECT `level`, `gexp`, `bexp`, `pexp`, `yexp`  , `hgexp`, `hbexp`, `hpexp`, `hyexp`, `skillLevel`, `attrIndex` FROM `cardupgrade` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
            csys.loadCardUpgradeTable(dbpn); 
		return true;
    }
    
    bool GDataManager::LoadCardInfo()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCardInfo dbpn;
		if(execu->Prepare("SELECT `id`, `type`, `color`, `lvLimit`, `skillId`,`name` FROM `cardInfo` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
            csys.loadInitCardInfo(dbpn); 
		return true;
    }

    bool GDataManager::LoadClanShopInfo()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBClanShopInfo dbpn;
		if(execu->Prepare("SELECT `id`, `itemid`, `name`, `price`, `prob`, `color`,`lvl` FROM `clan_contribution_shop` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
            ClanShopInfo::ClanShopItems its;
            its.id = dbpn.id;
            its.itemid = dbpn.itemid;
            its.name = dbpn.name;
            its.price = dbpn.price;
            its.prob = dbpn.prob;
            its.color = dbpn.color;
            its.lvl = dbpn.lvl;
            clanShopInfo.setClanShopInfo(dbpn.id, its);
        }
		return true;
    }

    bool GDataManager::LoadCubeAttr()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCubeAttr dbpn;
		if(execu->Prepare("SELECT `id`, `hp`, `attack`, `action` FROM `cubeAttr` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
             pictureAttrData.setVecInfoAttr( dbpn.hp , dbpn.attack , dbpn.action);
        }
		return true;
    }
    bool GDataManager::LoadFloorAttr()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBFloorAttr dbpn;
		if(execu->Prepare("SELECT `id`, `hp`, `attack`, `action` FROM `floorAttr` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
          // if(dbpn.id != 0)
               pictureAttrData.setVecInfoAttr( dbpn.hp , dbpn.attack , dbpn.action , 1);
        }
		return true;
    }

    bool GDataManager::LoadPicInfo()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBPicInfo dbpn;
		if(execu->Prepare("SELECT `id`, `index`, `cost`, `buff` FROM `picInfo` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
            pictureAttrData.setFloorInfo( dbpn.id , dbpn.index , dbpn.cost , dbpn.buffId);
        }
		return true;
    }
    bool GDataManager::LoadCubeCount()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCubeCount dbpn;
		if(execu->Prepare("SELECT `id`, `index`, `cnt` FROM `cubeCount` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
            pictureAttrData.setCubeCount( dbpn.id , dbpn.index , dbpn.cnt);
        }
		return true;
    }
    bool GDataManager::LoadKettleNpc()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBKettleNpc dbk;
		if(execu->Prepare("SELECT `monsterId`, `attack`, `magatk` , `hp`,`action`,`exp` FROM `kettleNpc` ", dbk) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
            KettleAttr ka(dbk.attack , dbk.magatk , dbk.hp ,dbk.action ,dbk.exp);
            kettleAttrData.LoadKettleMonsterAttr( (dbk.monsterId-1)/7 , (dbk.monsterId-1)%7 ,ka);
        }
		return true;
    }
    bool GDataManager::LoadLingbaoLevel()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBLingbaoLevel dblbl;
		if(execu->Prepare("SELECT `id`, `itemNum`, `itemCount` , `percent`,`probability`,`HoneyFall`,`HoneyFallP` FROM `lingbaoLevel` ", dblbl) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
        {
            lingbaoLevelData.SetLingbaoLevelData( dblbl.id/100-1 , dblbl.id%100-1,dblbl.itemNum ,dblbl.itemCount , dblbl.percent , dblbl.probability , dblbl.honeyFall , dblbl.honeyFallP);
        }
		return true;
    }

    bool GDataManager::LoadTitlePaper()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBTitlePaper dbpn;
		if(execu->Prepare("SELECT `titleid`, `award`, `type`, `option_num` FROM `titlepaper` ", dbpn) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
            GObject::QuestionPaper::Instance().loadTitlePaper(dbpn); 
		return true;
    }
}


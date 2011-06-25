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
#include "GObject/Item.h"
#include "DB/DBConnectionMgr.h"
#include "GDataDBExecHelper.h"
#include "DB/DBExecutor.h"
#include "Server/ServerTypes.h"
#include "Server/Cfg.h"
#include "SkillTable.h"
#include "TalentTable.h"
#include "CittaTable.h"
#include "AcuPraTable.h"
#include "Common/StringTokenizer.h"

#include "Script/lua_tinker.h"

namespace GData
{
	ItemBaseTypeManager		itemBaseTypeManager;
	ItemBaseTypeNameManager itemBaseTypeNameManager;
	ObjectMapT<GObject::ItemWeapon> npcWeapons;
	std::vector<ItemGemType *> gemTypes(200);
	ItemEquipSetTypeManager	itemEquipSetTypeManager;

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

	bool GDataManager::LoadAllData()
	{
		if (!LoadExpData())
		{
			fprintf(stderr, "Load ExpData Error !\n");
			return false;
		}
		if (!LoadAcuPraData())
		{
			fprintf(stderr, "Load AcuPra Error !\n");
			return false;
		}
		if (!LoadAreaData())
		{
			fprintf(stderr, "Load AreaData Error !\n");
			return false;
		}
		if (!LoadWeaponDefData())
		{
			fprintf(stderr, "Load WeaponDefData Error !\n");
			return false;
		}
		if (!LoadAttrExtraData())
		{
			fprintf(stderr, "Load AttrExtraData Error !\n");
			return false;
		}
		if (!LoadItemTypeData())
		{
			fprintf(stderr, "Load ItemTypeData Error !\n");
			return false;
		}
		if (!LoadFormationData())
		{
			fprintf(stderr, "Load FormationData Error !\n");
			return false;
		}
		if (!LoadTaskTypeData())
		{
			fprintf(stderr, "Load TaskTypeData Error !\n");
			return false;
		}
		if (!LoadLootData())
		{
			fprintf(stderr, "Load LootData Error !\n");
			return false;
		}
		if(!LoadQueRule())
		{
			fprintf(stderr, "Load sale que rule Error !\n");
			return false;
		}
		if (!LoadFighterTrainData())
		{
			fprintf(stderr, "Load fighter train daata Error !\n");
			return false;
		}
		if (!LoadPracticeData())
		{
			fprintf(stderr, "Load practice daata Error !\n");
			return false;
		}
        if (!LoadTalent())
        {
			fprintf(stderr, "Load talent template Error !\n");
			return false;
        }
        if (!LoadSkillEffect())
        {
			fprintf(stderr, "Load skill effect Error !\n");
			return false;
        }
        if (!LoadSkills())
        {
			fprintf(stderr, "Load skills template Error !\n");
			return false;
        }
        if (!LoadCittaEffect())
        {
			fprintf(stderr, "Load citta effect Error !\n");
			return false;
        }
        if (!LoadCittas())
        {
			fprintf(stderr, "Load cittas template Error !\n");
			return false;
        }
		if (!LoadClanLvlData())
		{
			fprintf(stderr, "Load clan level template Error !\n");
			return false;
		}
		if (!LoadClanTechTable())
		{
			fprintf(stderr, "Load clan tech template Error !\n");
			return false;
		}
		if (!LoadClanSkillTable())
		{
			fprintf(stderr, "Load clan skill template Error !\n");
			return false;
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
		return true;
	}

	bool GDataManager::LoadClanLvlData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBClanLvl dbexp;
		if(execu->Prepare("SELECT `lvl`, `exp` FROM `clan_lvl`", dbexp) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			clanLvlTable.setTable(dbexp.lvl, dbexp.exp);
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

	bool GDataManager::LoadAttrExtraData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		GData::DBAttrExtra ae;
		if(execu->Prepare("SELECT `id`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`, `auraMax`, `tough`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `action`, `hitrate`, `evade`, `critical`, `critical_dmg`, `pierce`, `counter`, `magres`, `skills` FROM `attr_extra`", ae) != DB::DB_OK)
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
			aextra->_extra.tough = ae.tough;
			aextra->_extra.action = ae.action;
			aextra->_extra.hitrate = ae.hitrate;
			aextra->_extra.evade = ae.evade;
			aextra->_extra.critical = ae.critical;
			aextra->_extra.critical_dmg = ae.critical_dmg;
			aextra->_extra.pierce = ae.pierce;
			aextra->_extra.counter = ae.counter;
			aextra->_extra.magres = ae.magres;

            StringTokenizer tk(ae.skills, ",");
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

	bool GDataManager::LoadItemTypeData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBItemType idt;
		if(execu->Prepare("SELECT `id`, `name`, `subClass`, `career`, `reqLev`, `coin`, `quality`, `maxQuantity`, `bindType`, `energy`, `data`, `enchant`, `attrId` FROM `item_template`", idt) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			ItemBaseType * wt;
			switch(idt.subClass)
			{
			case Item_Weapon:
				{
					wt = new ItemWeaponType(idt.typeId, idt.name, idt.attrExtra);
                    if (IsEquipId(idt.typeId))
                    {
                        GObject::ItemEquipData ied;
                        npcWeapons.add(new GObject::ItemWeapon(idt.typeId, *static_cast<const GData::ItemWeaponType *>(wt), ied));
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
			case Item_Trump:
			// case Item_Mounts: // XXX:
				{
					wt = new ItemEquipType(idt.typeId, idt.name, idt.attrExtra);
				}
				break;
			case Item_Gem:
				{
					ItemGemType * igt = new ItemGemType(idt.typeId, idt.name, idt.attrExtra);
					wt = igt;
					gemTypes[wt->getId() - LGEM_ID] = igt;
				}
				break;
			default:
				{
					wt = new ItemNormalType(idt.typeId, idt.name);
				}
				break;
			}
			wt->subClass = static_cast<ItemClass>(idt.subClass);
			wt->coin = idt.coin;
			wt->reqLev = idt.reqLev;
			wt->quality = idt.quality;
			wt->maxQuantity = idt.maxQuantity;
			wt->bindType = idt.bindType;
			wt->energy = idt.energy;
			wt->data = idt.data;
			itemBaseTypeManager.add(wt);
			itemBaseTypeNameManager.add(wt->getName(), wt);
		}
		DBEquipSetType est;
		if(execu->Prepare("SELECT `itemId`, `name`, `attrId2`, `attrId4`, `attrId6`, `attrId8` FROM `equipment_set`", est) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			ItemEquipSetType * iest = new ItemEquipSetType(est.id / 20, est.name);
			for(int j = 0; j < 4; ++ j)
			{
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
			luaopen_base(L);
			luaopen_string(L);
			luaopen_table(L);
			{
				std::string path = cfg.scriptPath + "ServerTaskConf.lua";
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
				m_TaelTrainList.resize(101);
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
			m_GoldTrainList.resize(101);
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
			m_LevelTrainExp.resize(121);
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
			std::string path = cfg.scriptPath + "Other/Practice.lua";
			lua_tinker::dofile(L, path.c_str());

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

    bool GDataManager::LoadTalent()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBTalent tal;
		if(execu->Prepare("SELECT `id`, `name`, `cls`, `quality`, `prob`, `potential`, `capacity`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`, `auraMax`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `tough`, `action`, `hitrate`, `evade`, `critical`, `critical_dmg`, `pierce`, `counter`, `magres` FROM `talent`", tal) != DB::DB_OK)
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
            ptal->critical_dmg = tal.critical_dmg;
            ptal->pierce = tal.pierce;
            ptal->counter = tal.counter;
            ptal->magres = tal.magres;
            talentManager.add(ptal);
        }
        return true;
    }


    bool GDataManager::LoadSkillEffect()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBSkillEffect effs;
		if(execu->Prepare("SELECT `id`, `state`, `immune`, `damage`, `adddam`, `magdam`, `addmag`, `hp`, `addhp`, `absorb`, `thorn`, `inj2hp`, `aura`, `atk`, `def`, `magatk`, `magdef`, `tough`, `action`, `hitrate`, `evade`, `critical`, `pierce`, `counter`, `magres` FROM `skill_effect`", effs) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            SkillEffect* ef = new SkillEffect(effs.id);
            if (!ef)
                return false;
            ef->state = effs.state;
            ef->immune = effs.immune;
            SetValOrPercent(ef->damage, ef->damageP, effs.damage);
            ef->adddam = effs.adddam;
            SetValOrPercent(ef->magdam, ef->magdamP, effs.magdam);
            ef->addmag = effs.addmag;
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
            ef->action = effs.action;
            ef->evade = effs.evade;
            ef->critical = effs.critical;
            ef->pierce = effs.pierce;
            ef->counter = effs.counter;
            ef->magres = effs.magres;
            skillEffectManager.add(ef);
        }
        return true;
    }

    bool GDataManager::LoadSkills()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBSkill skills;
		if(execu->Prepare("SELECT `id`, `name`, `target`, `cond`, `prob`, `area`, `factor`, `last`, `cd`, `effectid` FROM `skills`", skills) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            SkillBase* skill = new SkillBase(skills.id, skills.name);
            if (!skill)
                return false;
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
        }
        return true;
    }

    bool GDataManager::LoadCittaEffect()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBCittaEffect cf;
		if(execu->Prepare("SELECT `id`, `skills`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`, `auraMax`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `tough`, `action`, `hitrate`, `evade`, `critical`, `critical_dmg`, `pierce`, `counter`, `magres` FROM `citta_effect`", cf) != DB::DB_OK)
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
            cft->tough = cf.tough;
            cft->action = cf.action;
            cft->hitrate = cf.hitrate;
            cft->evade = cf.evade;
            cft->critical = cf.critical;
            cft->critical_dmg = cf.critical_dmg;
            cft->pierce = cf.pierce;
            cft->counter = cf.counter;
            cft->magres = cf.magres;

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
		if(execu->Prepare("SELECT `id`, `type`, `name`, `needsoul`, `effectid` FROM `cittas`", ct) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
            CittaBase* citta = new CittaBase(ct.id, ct.name);
            citta->type = ct.type;
            citta->needsoul = ct.needsoul;
            citta->effect = cittaEffectManager[ct.effectid];
            cittaManager.add(citta);
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
				clanTechTable.resize(cst.id+1);
			}
			SingleClanTechTable & single = clanTechTable[cst.id];
			if (cst.level >= single.size())
				single.resize(cst.level+1);
			accNeeds += cst.needs;
			single[cst.level] = ClanTechTableData(cst.id, cst.level, cst.needs, accNeeds, cst.clanLev, cst.effect1, cst.effect2);
		}

		return true;
	}

	bool GDataManager::LoadClanSkillTable()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		UInt32 accNeeds = 0;
		DBClanSkillType cst;
		if (execu->Prepare("SELECT `id`, `name`, `level`, `needs`, `hp`, `attack`, `defend`, `magatk`, `magdef` FROM `clan_skill_template` ORDER BY `id` ASC, `level` ASC", cst) != DB::DB_OK)
			return false;
		while (execu->Next() == DB::DB_OK)
		{
			if (cst.id >= clanSkillTable.size())
			{
				accNeeds = 0;
				clanSkillTable.resize(cst.id+1);
			}
			SingleClanSkillTable & single = clanSkillTable[cst.id];
			if (cst.level >= single.size())
				single.resize(cst.level+1);
			accNeeds += cst.needs;
			single[cst.level] = ClanSkillTableData(cst.id, cst.name, cst.level, accNeeds, cst.hp, cst.attack, cst.defend, cst.magatk, cst.magdef);
		}

		return true;
	}

	bool GDataManager::LoadFormationData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBFormation dbf;
		if(execu->Prepare("SELECT `id`, `name`, `eyes`, `grids`, `leastNum`, `skillId` FROM `formation`", dbf) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			StringTokenizer tk1(dbf.eyes, ",");
			std::vector<UInt8> eyes;
			for(StringTokenizer::Iterator it = tk1.begin(); it != tk1.end(); ++ it)
			{
				eyes.push_back(atoi((*it).c_str()));
			}
			Formation * form = new Formation(dbf.id, dbf.name, dbf.leastNum, dbf.skillId, eyes.size(), (eyes.size() > 0 ? &eyes[0] : NULL));
			StringTokenizer tk2(dbf.grids, "|");
			for(StringTokenizer::Iterator it = tk2.begin(); it != tk2.end(); ++ it)
			{
				Formation::GridEffect effect;
				StringTokenizer tk3(*it, ",");
				if(tk3.count() < 6)
					continue;
				effect.pos = atoi(tk3[0].c_str());
				if(effect.pos > 24)
					continue;
				effect.link = atoi(tk3[1].c_str());
				if(effect.link > 24)
					continue;
				effect.link_ratio = atoi(tk3[2].c_str());
				if(effect.link_ratio > 100)
					effect.link_ratio = 100;
				effect.rescue = atoi(tk3[3].c_str());
				if(effect.rescue > 24)
					continue;
				effect.rescue_ratio = atoi(tk3[4].c_str());
				if(effect.link_ratio > 100)
					effect.link_ratio = 100;
				UInt32 attrid = atoi(tk3[5].c_str());
				const AttrExtraItem * ae = attrExtraManager[attrid];
				if(ae == NULL)
					effect.attrExtra = NULL;
				else
					effect.attrExtra = *ae;
				form->addGrid(effect);
			}
			formationManager.add(form);
		}
		return true;
	}

	bool GDataManager::LoadLootData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBLootItem dbli;
		if(execu->Prepare("SELECT `id`, `itemId`, `isPack` FROM `loot`", dbli) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			LootItem li;
			li.isPack = dbli.isPack > 0;
			if(li.isPack)
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

	//日常任务
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
}

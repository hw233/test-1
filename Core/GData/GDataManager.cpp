#include "Config.h"
#include "GDataManager.h"
#include "Area.h"
#include "WeaponDef.h"
#include "Formation.h"
#include "ExpTable.h"
#include "LootTable.h"
#include "ClanSkillTable.h"
#include "GObject/Item.h"
#include "DB/DBConnectionMgr.h"
#include "GDataDBExecHelper.h"
#include "DB/DBExecutor.h"
#include "Server/ServerTypes.h"
#include "Server/Cfg.h"
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

	bool GDataManager::LoadAllData()
	{
		if (!LoadExpData())
		{
			fprintf(stderr, "Load ExpData Error !\n");
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

	bool GDataManager::LoadAreaData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBAreaDef adef;
		if(execu->Prepare("SELECT `id`, `side`, `area`, `rate` FROM `area`", adef) != DB::DB_OK)
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
			{
				const std::string area = adef.rate;
				StringTokenizer tokenizer(area, ",");
				if(tokenizer.count() < ad.size())
					continue;
				StringTokenizer::Iterator it;
				int z = 0;
				for(it = tokenizer.begin(); it != tokenizer.end(); ++ it)
				{
					if((*it).empty())
						continue;
					if((*it)[0] == '/')
					{
						ad[z].type = 1;
						ad[z ++].factor = static_cast<float>(atof((*it).c_str() + 1));
					}
					else if((*it)[0] == '*')
					{
						ad[z].type = 2;
						ad[z ++].factor = static_cast<float>(atof((*it).c_str() + 1));
					}
					else
					{
						ad[z].type = 0;
						ad[z ++].factor = static_cast<float>(atof((*it).c_str()));
					}
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

	bool GDataManager::LoadAttrExtraData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		GData::DBAttr1Extra a1e;
		if(execu->Prepare("SELECT `id`, `strength`, `physique`, `agility`, `intelligence` FROM `attr1_extra`", a1e) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			Attr1ExtraItem * a1extra = new Attr1ExtraItem(a1e.id);
			SetValOrPercent(a1extra->_extra.strength, a1extra->_extra.strengthP, a1e.strength);
			SetValOrPercent(a1extra->_extra.physique, a1extra->_extra.physiqueP, a1e.physique);
			SetValOrPercent(a1extra->_extra.agility, a1extra->_extra.agilityP, a1e.agility);
			SetValOrPercent(a1extra->_extra.intelligence, a1extra->_extra.intelligenceP, a1e.intelligence);
			attr1ExtraManager.add(a1extra);
		}
		GData::DBAttr2Extra a2e;
		if(execu->Prepare("SELECT `id`, `attack`, `defend`, `hp`, `action`, `hitrate`, `evade`, `critical`, `pierce`, `counter` FROM `attr2_extra`", a2e) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			Attr2ExtraItem * a2extra = new Attr2ExtraItem(a2e.id);
			SetValOrPercent(a2extra->_extra.attack, a2extra->_extra.attackP, a2e.attack);
			SetValOrPercent(a2extra->_extra.defend, a2extra->_extra.defendP, a2e.defend);
			SetValOrPercent(a2extra->_extra.hp, a2extra->_extra.hpP, a2e.hp);
			a2extra->_extra.action = a2e.action;
			a2extra->_extra.hitrate = a2e.hitrate;
			a2extra->_extra.evade = a2e.evade;
			a2extra->_extra.critical = a2e.critical;
			a2extra->_extra.pierce = a2e.pierce;
			a2extra->_extra.counter = a2e.counter;
			attr2ExtraManager.add(a2extra);
		}
		return true;
	}

	bool GDataManager::LoadItemTypeData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBItemType idt;
		if(execu->Prepare("SELECT `id`, `name`, `subClass`, `coin`, `reqLev`, `quality`, `maxQuantity`, `bindType`, `canDestroy`, `value`, `data`, `attr1Id`, `attr2Id` FROM `item_template`", idt) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			ItemBaseType * wt;
			switch(idt.subClass)
			{
			case Item_Weapon:
				{
					wt = new ItemWeaponType(idt.typeId, idt.name, idt.value, idt.attr1Extra, static_cast<UInt8>(idt.data));
					if(idt.typeId > 25000)
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
				{
					wt = new ItemEquipType(idt.typeId, idt.name, idt.value, idt.attr1Extra);
				}
				break;
			case Item_Gem:
				{
					ItemGemType * igt = new ItemGemType(idt.typeId, idt.name, idt.attr1Extra, idt.attr2Extra);
					wt = igt;
					gemTypes[wt->getId() - 5000] = igt;
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
			wt->canDestroy = idt.canDestroy;
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
				iest->attr1Extra[j] = *attr1ExtraManager[est.attrId[j]];
				iest->attr2Extra[j] = *attr2ExtraManager[est.attrId[j]];
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

	bool GDataManager::LoadClanSkillTable()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		UInt32 accNeeds = 0;
		DBClanSkillType cst;
		if (execu->Prepare("SELECT `id`, `level`, `needs`, `clanLev`, `effect1`, `effect2` FROM `clan_skill_template` ORDER BY `id` ASC, `level` ASC", cst) != DB::DB_OK)
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
			single[cst.level] = ClanSkillTableData(cst.id, cst.level, cst.needs, accNeeds, cst.clanLev, cst.effect1, cst.effect2);
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
				UInt32 attr1id = atoi(tk3[5].c_str());
				UInt32 attr2id;
				if(tk3.count() == 6)
					attr2id = attr1id;
				else
					attr2id = atoi(tk3[6].c_str());
				const Attr1ExtraItem * a1e = attr1ExtraManager[attr1id];
				if(a1e == NULL)
					effect.attr1Extra = NULL;
				else
					effect.attr1Extra = *a1e;
				const Attr2ExtraItem * a2e = attr2ExtraManager[attr2id];
				if(a2e == NULL)
					effect.attr2Extra = NULL;
				else
					effect.attr2Extra = *a2e;
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
		return const_cast<GObject::ItemWeapon *>(npcWeapons[wpId + 25000]);
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

}

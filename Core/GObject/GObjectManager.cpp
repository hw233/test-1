#include "Config.h"
#include "GObjectManager.h"
#include "DB/DBConnectionMgr.h"
#include "GObjectDBExecHelper.h"
#include "DB/DBExecutor.h"
#include "Server/OidGenerator.h"

#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Fighter.h"
#include "Map.h"
#include "Clan.h"
#include "ClanBattle.h"
#include "ClanManager.h"
#include "Item.h"
#include "Mail.h"
#include "Package.h"
#include "TaskMgr.h"
#include "Trade.h"
#include "SaleMgr.h"
#include "AthleticsRank.h"
#include "Athletics.h"
#include "TradeCheck.h"
#include "Boss.h"
#include "Dungeon.h"
#include "SpecialAward.h"
#include "Prepaid.h"
#include "ClanSkill.h"
#include "ClanBattle.h"
#include "ClanManager.h"
#include "LuckyDraw.h"
#include "BlockBossMgr.h"
#include "MapCollection.h"
#include "CountryBattle.h"
#include "Arena.h"
#include "GData/NpcGroup.h"
#include "GData/LootTable.h"
#include "GData/ExpTable.h"
#include "GData/GDataManager.h"
#include "GData/DungeonData.h"
#include "GData/ClanAssistantData.h"
#include "Script/BattleFormula.h"
#include "Common/LoadingCounter.h"
#include "Common/StringTokenizer.h"
#include "Common/DirectoryIterator.h"

#include <fcntl.h>

namespace GObject
{
	std::map<UInt32, ItemEquip *> GObjectManager::equips;

	bool GObjectManager::InitIDGen()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		//task
		UInt32 maxId;
		execu->Extract("SELECT max(`mailId`) FROM `mail`", maxId);
		IDGenerator::gMailOidGenerator.Init(maxId);
		execu->Extract("SELECT max(`id` & 0xFFFFFF) FROM `clan`", maxId);
		IDGenerator::gClanOidGenerator.Init(maxId);
		execu->Extract("SELECT max(`id`) FROM `equipment`", maxId);
		IDGenerator::gItemOidGenerator.Init(maxId);
		execu->Extract("SELECT max(`tradeId`) FROM `trade`", maxId);
		IDGenerator::gTradeOidGenerator.Init(maxId);
		execu->Extract("SELECT max(`saleId`) FROM `sale`", maxId);
		IDGenerator::gSaleOidGenerator.Init(maxId);
		execu->Extract("SELECT max(`id`) FROM `athletics_record`", maxId);
		IDGenerator::gAthleticsRecordOidGenerator.Init(maxId);
		execu->Extract("SELECT max(`id`) FROM `clan_battler`", maxId);
		IDGenerator::gClanBatterRecordIDGenerator.Init(maxId);

		return true;
	}

	void GObjectManager::loadAllData()
	{
		loadMapData();
		loadEquipments();
		loadFighters();
		loadClanAssist();
		loadClanRobMonster();
		loadAllPlayers();
		loadAllAthletics();
		unloadEquipments();
		loadAllFriends();
		LoadDungeon();
		loadAllClans();
		LoadSpecialAward();
		LoadLuckyDraw();
		LoadArena();
		DB::gDataDBConnectionMgr->UnInit();
	}

	bool GObjectManager::InitGlobalObject()
	{
        DB::DBExecutor* dbexer = DB::gObjectDBConnectionMgr->GetExecutor();
        if (!dbexer)
            return false;
		std::unique_ptr<DB::DBExecutor> execu(dbexer);
		DBVersion dbver;
		if(execu->Extract("SELECT `version` FROM `database_version`", dbver) != DB::DB_OK)
			return false;
		std::set<std::string> updates;
		std::string basePath = cfg.scriptPath + "../sql/updates/";
		DirectoryIterator dirit(basePath);
		DirectoryIterator end;
		while(dirit != end)
		{
			std::string filename = dirit.path().getBaseName();
			if(strcasecmp(filename.c_str(), dbver.version.c_str()) > 0)
			{
				updates.insert(dirit.path().getBaseName());
			}
			++ dirit;
		}
		if(updates.empty())
			return true;
		std::set<std::string>::iterator iter;
		for(iter = updates.begin(); iter != updates.end(); ++ iter)
		{
			int f = open((basePath + (*iter) + ".sql").c_str(), O_RDONLY);
			if(f < 0)
				continue;
			int off = static_cast<int>(lseek(f, 0, SEEK_END));
			if(off < 0 || lseek(f, 0, SEEK_SET) < 0)
				continue;
			char * updatestr = new(std::nothrow) char[off + 1];
			if(updatestr == NULL)
			{
				fprintf(stderr, "Out of memory! Cannot update game object database!\n");
				exit(1);
				return false;
			}
			if((off = read(f, updatestr, off)) > 0)
			{
				updatestr[off] = 0;
				execu->Execute(updatestr);
			}
			delete[] updatestr;
		}
		iter = updates.end();
		-- iter;
		char qstr[256];
		sprintf(qstr, "UPDATE `database_version` SET `version` = '%s'", iter->c_str());
		execu->Execute(qstr);
		return true;
	}

	bool GObjectManager::resetClanData(UInt32 now)
	{
		UInt32 thisDay = TimeUtil::SharpDay(0, now);
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		
		execu->Execute2("UPDATE `clan` SET `grabAchieve` = 0 WHERE `battleThisDay` <> %u", thisDay);
		execu->Execute2("UPDATE `clan` AS `update_clan` SET `battleStatus` = IF(%u < `battleThisDay` + `battleTime` * 1800, 0, IF(%u < `battleThisDay`+ `battleTime` * 1800 + 3600, 1, 2)) WHERE `battleThisDay` = %u AND `update_clan`.`id` = `id` AND (`battleStatus` = 0 OR `battleStatus` = 1)", now, now, thisDay);
		execu->Execute2("UPDATE `clan` AS `update_clan` SET `battleThisDay` = %u, `battleTime` = `update_clan`.`nextBattleTime`, `battleStatus` = IF(%u < %u + `nextBattleTime` * 1800, 0, IF(%u < %u + `nextBattleTime` * 1800 + 3600, 1, 2)) WHERE `battleThisDay` <> %u AND `update_clan`.`id` = `id` AND `battleStatus` <> 256", thisDay, now, thisDay, now, thisDay, thisDay);
		execu->Execute2("UPDATE `clan_player` AS `update_clan_player` SET `enterCount` = 0, `achieveCount` = 0 WHERE `playerId` = `update_clan_player`.`playerId` AND `thisDay` <> %u", thisDay);
		
		return true;
	}

	bool GObjectManager::loadMapData()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		// load maps
		MapData mdata;
		if(execu->Prepare("SELECT `id`, `name`, `level`, `acquirable`, `country` FROM `map`", mdata) != DB::DB_OK)
			return false;

		mapList.resize(256);
		while(execu->Next() == DB::DB_OK)
		{
			UInt8 id = mdata.m_ID;
			if(mapList[id] != NULL)
				continue;
			Map * map = new Map(id, mdata.m_Name);
			mapList[id] = map;
			map->SetMapData(mdata);

			mapCollection.setCountry(id, mdata.m_Country);
		}


		// load map spots
		DBMapSpot spot;
		if(execu->Prepare("SELECT `id`, `name`, `type`, `countryBattle`, `x`, `y` FROM `map_spot`", spot) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
			Map::AddSpot(spot.id, spot.name, spot.type, spot.countryBattle, spot.x, spot.y);
		}

		// load map objects
		MOData mos;
		if(execu->Prepare("SELECT `id`, `name`, `spot`, `type`, `actionType` FROM `map_object`", mos) != DB::DB_OK)
			return false;

		while(execu->Next() == DB::DB_OK)
		{
			if(mos.m_ID <= GREAT_FIGHTER_MAX)
				globalFighters.setSpot(mos.m_ID, mos.m_Spot);
			Map * map = Map::FromSpot(mos.m_Spot);
			mos.m_Hide = false;
			if(map == NULL)
			{
				//ERROR_LOG...
				continue;
			}
			map->AddObject(mos);
		}

		return true;
	}

	bool GObjectManager::loadFighters()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading fighter templates:");
		DBFighter dbfgt;
		if(execu->Prepare("SELECT `id`, `name`, `class`, `level`, `sex`, `potential`, `capacity`, `skill`, `npc_weapon`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`, `tough`, `attack`, `img_attack`, `defend`, `img_defend`, `hp`, `action`, `peerless`, `hitrate`, `evade`, `critical`, `critical_dmg`, `pierce`, `counter`, `img_res`, `extraPos` FROM `special_fighter_template`", dbfgt) != DB::DB_OK)
			return false;

		UInt32 maxGF = 0;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			UInt32 id = dbfgt.id;
			GObject::Fighter * fgt = new GObject::Fighter(id, NULL);
			fgt->setName(dbfgt.name);
			fgt->setClass(dbfgt.cls);
			fgt->setLevel(dbfgt.lvl);
			fgt->setExp(GData::expTable.getLevelMin(fgt->getLevel()));
            fgt->setSex(dbfgt.sex);
			ItemWeapon * nwp = GData::GDataManager::GetNpcWeapon(dbfgt.npc_weapon);
			fgt->setWeapon(nwp, false);
            fgt->setSkills(dbfgt.skill);
			fgt->setPotential(dbfgt.potential, false);
			fgt->strength = dbfgt.strength;
			fgt->physique = dbfgt.physique;
			fgt->agility = dbfgt.agility;
			fgt->intelligence = dbfgt.intelligence;
			fgt->attack = dbfgt.attack;
			fgt->defend = dbfgt.defend;
			fgt->maxhp = dbfgt.hp;
			fgt->action = dbfgt.action;
			fgt->hitrate = dbfgt.hitrate;
			fgt->evade = dbfgt.evade;
			fgt->critical = dbfgt.critical;
			fgt->pierce = dbfgt.pierce;
			fgt->counter = dbfgt.counter;

			StringTokenizer tokenizer(dbfgt.extraPos, "|");
			for(size_t j = 0; j < tokenizer.count(); ++ j)
			{
				StringTokenizer tokenizer2(tokenizer[j], ",");
				if(tokenizer2.count() < 2)
					continue;
				Fighter::Offset off;
				off.x = atoi(tokenizer2[0].c_str());
				off.y = atoi(tokenizer2[1].c_str());
				fgt->extraPos.push_back(off);
			}
			if(fgt->getId() <= GREAT_FIGHTER_MAX)
			{
				if(fgt->getId() > maxGF)
					maxGF = fgt->getId();
			}
			globalFighters[fgt->getId()] = fgt;
			float potential = fgt->getPotential();
			UInt8 level = fgt->getLevel();
			switch (id)
			{
			case 4478:
				{
					for (UInt8 inc = 1; inc <= 50; ++ inc)
					{
						Fighter * fgt_clone = fgt->clone(NULL);
						fgt_clone->setWeapon(nwp, false);
						fgt_clone->setPotential(potential + inc * 0.02f, false);
						fgt_clone->setLevel(level + inc);
						globalFighters[id + inc] = fgt_clone;
					}
				}
				break;
			case 4529:
			case 4551:
				{
					for (UInt8 inc = 1; inc <= 10; ++ inc)
					{
						Fighter * fgt_clone = fgt->clone(NULL);
						fgt_clone->setWeapon(nwp, false);
						fgt_clone->setPotential(potential + inc * 0.05f, false);
						fgt_clone->setLevel(level + inc * 5);
						globalFighters[id + inc] = fgt_clone;
					}
				}
				break;
			case 4540:
			case 4562:
				{
					for (UInt8 inc = 1; inc <= 10; ++ inc)
					{
						Fighter * fgt_clone = fgt->clone(NULL);
						fgt_clone->setWeapon(nwp, false);
						fgt_clone->setPotential(potential + inc * 0.1f, false);
						fgt_clone->setLevel(level + inc * 5);
						globalFighters[id + inc] = fgt_clone;
					}
				}
				break;
			default:
				{
					float pt = 0.0f;
					if(id >= 4373 && id <= 4391)
						pt = 0.8f;
					else if(id >= 4392 && id <= 4413)
						pt = 0.8f;
					else if(id >= 4419 && id <= 4460)
						pt = 0.8f;
					else if(id >= 4697 && id <= 4710)
						pt = 1.0f;
					else if(id >= 4723 && id <= 4740)
						pt = 1.2f;
					if(pt != 0.0f)
					{
						Fighter * fgt_clone = fgt->clone(NULL);
						fgt_clone->setWeapon(nwp, false);
						fgt_clone->setPotential(fgt->getPotential() + pt, false);
						globalFighters[fgt_clone->getId() + 4096] = fgt_clone;
						fgt_clone = fgt->clone(NULL);
						fgt_clone->setWeapon(nwp, false);
						fgt_clone->setPotential(fgt->getPotential() + pt * 2, false);
						globalFighters[fgt_clone->getId() + 8192] = fgt_clone;
					}
				}
				break;
			}
		}
		lc.finalize();
		globalFighters.setMaxFgtId(maxGF);
		globalFighters.buildSummonSet();

		lc.prepare("Loading NPC groups:");
		GData::DBNpcGroup dbng;
		if(execu->Prepare("SELECT `id`, `fighterId`, `formationId`, `experience`, `lootId` FROM `npc_group`", dbng) != DB::DB_OK)
			return false;
		std::string path = cfg.scriptPath + "formula/main.lua";
		Script::BattleFormula bform(path.c_str());
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			GData::NpcGroup * ngroup = new GData::NpcGroup(dbng.id);
			StringTokenizer tokenizer(dbng.fighterId, "|");
			for(size_t j = 0; j < tokenizer.count(); ++ j)
			{
				StringTokenizer tokenizer2(tokenizer[j], ",");
				for(size_t k = 1; k < tokenizer2.count(); ++ k)
					ngroup->addFighter(atoi(tokenizer2[0].c_str()), atoi(tokenizer2[k].c_str()));
			}
			ngroup->setFormation(dbng.formationId);
			ngroup->setExp(dbng.experience);
			StringTokenizer tk(dbng.lootId, ",");
			std::vector<const GData::LootItem *> lootItem;
			for(size_t j = 0; j < tk.count(); ++ j)
			{
				const GData::LootItem * li = GData::lootTable[atoi(tk[j].c_str())];
				if(li == NULL)
					continue;
				lootItem.push_back(li);
			}
			ngroup->setLoots(lootItem);
			ngroup->calcBattlePoints(&bform);
			GData::npcGroups[dbng.id] = ngroup;
		}
		lc.finalize();

		lc.prepare("Loading boss template:");
		DBBoss dbboss;
		if(execu->Prepare("SELECT `id`, `location`, `spawnTime`, `fleeTime`, `appearLevel` FROM `boss`", dbboss) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Map * map = Map::FromSpot(dbboss.location);
			if(map == NULL)
			{
				continue;
			}
			MOData mo;
			mo.m_ID = dbboss.id;
			mo.m_Hide = true;
			mo.m_Spot = dbboss.location;
			mo.m_Type = 6;
			mo.m_ActionType = 0;
			map->AddObject(mo);
			bossManager.addBoss(dbboss.id, dbboss.location, dbboss.spawnTime, dbboss.fleeTime, dbboss.appearLevel);
		}
		lc.finalize();

		return true;
	}

	bool GObjectManager::loadClanAssist()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		DBClanAssistant dcat;
		if (execu->Prepare("SELECT `id`, `formated`, `monsters` FROM `clan_hold_monster`", dcat) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			GData::ClanAssistant * cat = new GData::ClanAssistant(dcat.id);
			cat->id = dcat.id;
			cat->formated = dcat.formated;
			if(cat->formated)
			{
				//BOSS
				StringTokenizer tk(dcat.assistants, "|");
				{
					StringTokenizer tk1(tk[0], ",");
					GData::AssistantFData afd;
					afd.fighter = GObject::globalFighters[atoi(tk1[0].c_str())];
					if (afd.fighter == NULL)
						continue;
					afd.fighterNum = 1;
					UInt8 line = atoi(tk1[1].c_str());
					for (UInt8 i = line * 5; i <= line * 5 + 4; ++ i)
						afd.pos.push_back(i);
					cat->_assistList.push_back(afd);
				}
				//monster
				{
					StringTokenizer tk2(tk[1], ",");
					GObject::Fighter * fgt = GObject::globalFighters[atoi(tk2[0].c_str())];
					if (fgt == NULL)
						continue;
					GData::AssistantFData afd;
					afd.fighter = fgt;
					afd.fighterNum = atoi(tk2[1].c_str());
					UInt8 lineBegin = atoi(tk2[2].c_str());
					UInt8 lineEnd = atoi(tk2[3].c_str());
					for (UInt8 i = lineBegin * 5; i <= lineEnd * 5 + 4; ++ i)
						afd.pos.push_back(i);
					cat->_assistList.push_back(afd);
				}
			}
			else
			{
				StringTokenizer tk(dcat.assistants, ",");
				if (tk.count() < 2)
					continue;
				GObject::Fighter * fgt = GObject::globalFighters[atoi(tk[0].c_str())];
				if (fgt == NULL)
					continue;
				GData::AssistantFData afd;
				afd.fighter = fgt;
				afd.fighterNum = atoi(tk[1].c_str());
				for (UInt8 i = 0; i <= 24; ++ i)
					afd.pos.push_back(i);
				cat->_assistList.push_back(afd);
			}
			GData::clanAssistants.add(cat);
		}

		return true;
	}

	bool GObjectManager::loadClanRobMonster()
	{
		lua_State * L = lua_open();
		luaopen_base(L);
		luaopen_string(L);
		luaopen_table(L);
		{
			std::string path = cfg.scriptPath + "formula/blockboss.lua";
			lua_tinker::dofile(L, path.c_str());

			//totemAssistBoss
			{
				lua_tinker::table tb = lua_tinker::call<lua_tinker::table>(L, "getClanTotemAssistBoss");
				UInt32 sz = static_cast<UInt32>(tb.size());
				for (UInt32 i = 1; i <= sz; ++ i)
				{
					UInt32 id = tb.get<UInt32>(i);
					GData::NpcGroups::iterator found = GData::npcGroups.find(id);
					if (found != GData::npcGroups.end())
						GData::clanRobMonster._clanTotemAssistBoss.push_back(found->second);
				}
			}

			//clanSouthBoss
			{
				lua_tinker::table tb = lua_tinker::call<lua_tinker::table>(L, "getClanSouthBoss");
				UInt32 sz = static_cast<UInt32>(tb.size());
				for (UInt32 i = 1; i <= sz; ++ i)
				{
					UInt32 id = tb.get<UInt32>(i);
					GData::NpcGroups::iterator found = GData::npcGroups.find(id);
					if (found != GData::npcGroups.end())
						GData::clanRobMonster._clanSouthBoss.push_back(found->second);
				}
			}
			//clanNorthBoss
			{
				lua_tinker::table tb = lua_tinker::call<lua_tinker::table>(L, "getClanNorthBoss");
				UInt32 sz = static_cast<UInt32>(tb.size());
				for (UInt32 i = 1;i <= sz; ++ i)
				{
					UInt32 id = tb.get<UInt32>(i);
					GData::NpcGroups::iterator found = GData::npcGroups.find(id);
					if (found != GData::npcGroups.end())
						GData::clanRobMonster._clanNorthBoss.push_back(found->second);
				}
			}
			//
			{
				UInt32 id = lua_tinker::call<UInt32>(L, "getClanSouthGuarderBoss");
				GData::NpcGroups::iterator found = GData::npcGroups.find(id);
				if (found != GData::npcGroups.end())
					GData::clanRobMonster._clanSouthGuarderBoss = found->second;
			}
			//
			{
				UInt32 id = lua_tinker::call<UInt32>(L, "getClanNorthGuarderBoss");
				GData::NpcGroups::iterator found = GData::npcGroups.find(id);
				if (found != GData::npcGroups.end())
					GData::clanRobMonster._clanNorthGuarderBoss = found->second;
			}
			//
			{
				UInt32 id = lua_tinker::call<UInt32>(L, "getClalTotemGuarderBoss");
				GData::NpcGroups::iterator found = GData::npcGroups.find(id);
				if (found != GData::npcGroups.end())
					GData::clanRobMonster._clanTotemGuarderBoss = found->second;
			}
		}

		return true;
	}

	bool find_buff(DBFighterBuffData& buff, UInt32 id)
	{
		return buff.fighterId == id;
	}

	inline bool player_load(Player * p, int)
	{
		p->Load();
		gBlockbossmgr.addPlayerRank(p, p->getBlockBossLevel(), p->GetLev());
		return true;
	}

	bool GObjectManager::loadAllPlayers()
	{
		UInt64 last_id;
		Player * pl;

		using namespace std::placeholders;
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading players:");
		// load players
		DBPlayerData dbpd;
		if(execu->Prepare("SELECT `player`.`id`, `name`, `gold`, `coupon`, `tael`, `coin`, `status`, `country`, `title`, `archievement`, `location`, `inCity`, `lastOnline`, `newGuild`, `packSize`, `mounts`, `icCount`, `formation`, `lineup`, `bossLevel`, `totalRecharge`, `nextReward`, `nextExtraReward`, `lastExp`, `lastResource`, `tavernId`, `bookStore`, `gmLevel`, `wallow`, UNIX_TIMESTAMP(`created`), `locked_player`.`lockExpireTime` FROM `player` LEFT JOIN `locked_player` ON `player`.`id` = `locked_player`.`player_id`", dbpd) != DB::DB_OK)
            return false;

		lc.reset(200);
		UInt32 tlimit = TimeUtil::SharpDay(-1);
		UInt32 nextDay = TimeUtil::SharpDay(1);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
#define LOAD_LINEUP(lus, lud) \
			{ \
				StringTokenizer tk(lus, "|"); \
				for(size_t z = 0; z < tk.count(); ++ z) \
				{ \
					if(z > 4) break; \
					StringTokenizer tk2(tk[z], ","); \
					if(tk2.count() < 2) \
						continue; \
					lud[z].fid = atoi(tk2[0].c_str()); \
					lud[z].pos = atoi(tk2[1].c_str()); \
				} \
			}
			LOAD_LINEUP(dbpd.lineup, dbpd.pdata.lineup);
			UInt64 id = dbpd.id;
			Player * pl = new Player(id);

			if(dbpd.pdata.gold > 0x7FFFFFFF)
			{
				dbpd.pdata.gold = 10;
				DB().PushUpdateData("UPDATE `player` SET `gold` = 10 WHERE `id` = %"I64_FMT"u", id);
			}

			{
				StringTokenizer tk(dbpd.icCount, "|");
				size_t count = tk.count();
				if(count > 0)
				{
					dbpd.pdata.icCount = atoi(tk[0].c_str());
					if(count > 1)
					{
						char * endptr;
						dbpd.pdata.nextIcReset = strtoul(tk[1].c_str(), &endptr, 10);
					}
					else
						dbpd.pdata.nextIcReset = nextDay;
				}
			}

			{
				StringTokenizer ntk(dbpd.nextreward, "|");
				if(ntk.count() > 3)
				{
					dbpd.pdata.rewardStep = atoi(ntk[0].c_str());
					dbpd.pdata.nextRewardItem = atoi(ntk[1].c_str());
					dbpd.pdata.nextRewardCount = atoi(ntk[2].c_str());
					dbpd.pdata.nextRewardTime = atoi(ntk[3].c_str());
					pl->setPlayerData(dbpd.pdata);
				}
				else
				{
					char * endptr;
					UInt32 rwd = strtoul(dbpd.nextreward.c_str(), &endptr, 10);
					dbpd.pdata.rewardStep = rwd >> 28;
					dbpd.pdata.nextRewardTime = rwd & 0xFFFFFF;
					UInt32 seed = ((rwd & 0x0F000000) >> 24) + static_cast<UInt32>(id);
					pl->setPlayerData(dbpd.pdata);
					pl->recalcVipLevel();
					pl->genOnlineRewardItems(seed);
				}
			}

			{
				StringTokenizer tk(dbpd.tavernId, "|");
				size_t count = tk.count();
				if(count > 0)
				{
					do {
						size_t tcount = count;
						if(tcount > 6)
							tcount = 6;
						for(size_t j = 0; j < tcount; ++ j)
						{
							PLAYER_DATA(pl, tavernId)[j] = atoi(tk[j].c_str());
						}
						if(count > 6)
						{
							PLAYER_DATA(pl, tavernBlueCount) = atoi(tk[6].c_str());
							if(count > 7)
							{
								PLAYER_DATA(pl, tavernPurpleCount) = atoi(tk[7].c_str());
								if(count > 8)
								{
									char * endptr;
									pl->setNextTavernUpdate(strtoul(tk[8].c_str(), &endptr, 10));
									break;
								}
							}
						}
						pl->setNextTavernUpdate(1);
					} while(0);
				}
				else
					pl->setNextTavernUpdate(0);
			}

			{
				StringTokenizer tk(dbpd.bookStore, "|");
				size_t count = tk.count();
				if(count > 0)
				{
					do {
						size_t tcount = count;
						if(tcount > 6)
							tcount = 6;
						for(size_t j = 0; j < tcount; ++ j)
						{
							PLAYER_DATA(pl, bookStore)[j] = atoi(tk[j].c_str());
						}
						if(count > 6)
						{
							char * endptr;
							pl->setNextBookStoreUpdate(strtoul(tk[6].c_str(), &endptr, 10));
							break;
						}
						pl->setNextBookStoreUpdate(1);
					} while(0);
				}
				else
					pl->setNextBookStoreUpdate(0);
			}

			pl->setBossLevel(dbpd.bossLevel, false);

			pl->patchMergedName();
			globalPlayers.add(id, pl);
			if(dbpd.pdata.created >= tlimit)
			{
				newPlayers.add(id, pl);
			}
			globalNamedPlayers.add(pl->getName(), pl);
		}
		lc.finalize();

		lc.prepare("Loading prepaid id list:");
		DBPrepaid prepaidData;
		if(execu->Prepare("SELECT `id`, `gold` FROM `prepaid`", prepaidData) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			prepaid.push(prepaidData.id, prepaidData.gold);
		}
		lc.finalize();

		lc.prepare("Loading player buffs:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBPlayerBuffData buffData;
		if(execu->Prepare("SELECT `id`, `buffId`, `data` FROM `player_buff` ORDER BY `id`", buffData) != DB::DB_OK)
			return false;
		lc.reset(100);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(buffData.id != last_id)
			{
				last_id = buffData.id;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			pl->setBuffData(buffData.buffId, buffData.data, false);
		}
		lc.finalize();

		lc.prepare("Loading friendliness:");
		DBFightersFriendliness ffdata;
		if(execu->Prepare("SELECT `playerId`, `fighterId`, `friendliness`, `favorSubmitCount`, `favorSubmitDay` FROM `friendliness` ORDER BY `playerId`", ffdata) != DB::DB_OK)
			return false;
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		lc.reset(2000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(ffdata.playerId != last_id)
			{
				last_id = ffdata.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			pl->setGreatFighterTaskVal(ffdata.fighterId, ffdata.friendliness, ffdata.submitFavorCount, ffdata.submitFavorDay);
		}
		lc.finalize();

		lc.prepare("Loading fighters:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBFighterObj specfgtobj;
		if(execu->Prepare("SELECT `id`, `playerId`, `potential`, `capacity`, `level`, `experience`, `practiceExp`, `hp`, `weapon`, `armor1`, `armor2`, `armor3`, `armor4`, `armor5`, `ring`, `amulet`, `peerless`, `trump`, `acupoints`, `skill`, `citta`, `skills`, `cittas` FROM `fighter` ORDER BY `playerId`", specfgtobj) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(specfgtobj.playerId != last_id)
			{
				last_id = specfgtobj.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			if(specfgtobj.id > GREAT_FIGHTER_MAX)
				continue;
			Fighter * fgt = globalFighters[specfgtobj.id];
			if(fgt == NULL)
				continue;
			Fighter * fgt2 = fgt->clone(pl);
			if(fgt2 == NULL)
				continue;
			pl->addFighter(fgt2, false);
			fgt2->setPotential(specfgtobj.potential, false);
            fgt2->setCapacity(specfgtobj.capacity, false);
			fgt2->setLevel(specfgtobj.level);
			fgt2->setExp(specfgtobj.experience);
			fgt2->setPExp(specfgtobj.practiceExp);
			fgt2->setCurrentHP(specfgtobj.hp, false);
            fgt2->setPeerless(specfgtobj.peerless, false);
            fgt2->setAcupointsBits(specfgtobj.acupoints, false);
			fgt2->setWeapon(fetchWeapon(specfgtobj.weapon), false);
			fgt2->setArmor(0, fetchArmor(specfgtobj.armor1), false);
			fgt2->setArmor(1, fetchArmor(specfgtobj.armor2), false);
			fgt2->setArmor(2, fetchArmor(specfgtobj.armor3), false);
			fgt2->setArmor(3, fetchArmor(specfgtobj.armor4), false);
			fgt2->setArmor(4, fetchArmor(specfgtobj.armor5), false);
			fgt2->setRing(fetchEquipment(specfgtobj.ring), false);
			fgt2->setAmulet(fetchEquipment(specfgtobj.amulet), false);
            fgt2->setTrump(specfgtobj.trump, false);
            fgt2->setUpSkills(specfgtobj.skill, false);
            fgt2->setSkills(specfgtobj.skills, false);
            fgt2->setUpCittas(specfgtobj.citta, false);
            fgt2->setCittas(specfgtobj.cittas, false);
            // TODO:
#if 0
			if(specfgtobj.skill != 0)
			{
				fgt2->setSkillAndLevel(specfgtobj.skill);
			}
#endif
		}
		lc.finalize();

		lc.prepare("Loading fighter buffs:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBFighterBuffData fbuff;
		if(execu->Prepare("SELECT `playerId`, `id`, `buffId`, `data` FROM `fighter_buff` ORDER BY `playerId`", fbuff) != DB::DB_OK)
			return false;
		lc.reset(200);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(fbuff.playerId != last_id)
			{
				last_id = fbuff.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			Fighter * fgt = pl->findFighter(fbuff.fighterId);
			if(fgt != NULL)
				fgt->setBuffData(fbuff.buffId, fbuff.data, false);
		}
		lc.finalize();

		lc.prepare("Loading password:");
		last_id = 0xFFFFFFFFFFFFFFFFull; 
		pl = NULL;
		DBSecondPWD spd;
		if(execu->Prepare("SELECT `playerId`, `password`, `questionForPWD`, `answerForPWD` FROM `pass_word`", spd) != DB::DB_OK)
			return false;
		lc.reset(100);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(spd.playerId != last_id)
			{
				last_id = spd.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			pl->setSecondPWDDB(spd.secondPWD, spd.questionForPWD, spd.answerForPWD);
		}
		lc.finalize();

		//load all items
		lc.prepare("Loading items:");
		last_id = 0xFFFFFFFFFFFFFFFFull; 
		pl = NULL;
		ItemData idata;
		if(execu->Prepare("SELECT `id`, `ownerId`, `itemNum`, `bindType` FROM `item` ORDER BY `ownerId`", idata) != DB::DB_OK)
			return false;
		lc.reset(2000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(idata.ownerId != last_id)
			{
				last_id = idata.ownerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			if (!IsEquipId(idata.id))
				pl->GetPackage()->AddItemFromDB(idata.id, idata.itemNum, idata.bindType != 0);
			else
				pl->GetPackage()->AddEquipFromDB(idata.id, idata.bindType != 0);
		}
		lc.finalize();

		//load all trades
		lc.prepare("Loading trades:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBTradeData tdata;
		if(execu->Prepare("SELECT `tradeId`, `ownerId`, `tradeSide`, `tradeStatus`, `tradeTo`, `tradeTitle`, `tradeTime`,  `coin`, `gold`, `items` FROM `trade` ORDER BY `ownerId`", tdata) != DB::DB_OK)
			return false;
		lc.reset(100);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (tdata.ownerId != last_id)
			{
				last_id = tdata.ownerId;
				pl = globalPlayers[last_id];
			}
			if (pl == NULL)
				continue;
			Player * tradePlayer = globalPlayers[tdata.tradeTo];
			if (tradePlayer == NULL)
				continue;
			TradeData * trade = new(std::nothrow) TradeData(tdata.tradeId);
			trade->_tradeSide = tdata.tradeSide;
			trade->_tradeStatus = tdata.tradeStatus;
			trade->_tradeTitle = tdata.tradeTitle;
			trade->_tradeTime = tdata.tradeTime;
			trade->_tradePlayer = tradePlayer;
			trade->_coin = tdata.coin;
			trade->_gold = tdata.gold;
			if (Trade::needBuildTradeItems(trade->_tradeStatus))
			{
				trade->_strItems = tdata.items;
				StringTokenizer tokenizer(tdata.items, "|");
				for(size_t j = 0; j < tokenizer.count(); ++ j)
				{
					StringTokenizer tokenizer2(tokenizer[j], ",");
					std::size_t count = tokenizer2.count();
					ItemBase * item;
					if (count == 1)
					{
						item = Package::BuildEquipFromDB(atoi(tokenizer2[0].c_str()), false);
						if (item != NULL) 
							trade->_items.push_back(static_cast<ItemEquip *>(item));
					}
					else if(count == 2)
					{
						item = Package::BuildItem(atoi(tokenizer2[0].c_str()), atoi(tokenizer2[1].c_str()), false);
						if (item != NULL) 
							trade->_items.push_back(item);
					}
				}
			}
			pl->GetTrade()->addTradeFromDB(trade);
			gTradeCheck.addTradeCheck(trade->_id, trade->_tradeTime, pl);
		}
		lc.finalize();

		lc.prepare("Loading mail package:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBMailPackageData mpdata;
		if(execu->Prepare("SELECT `id`, `itemId`, `itemCount` FROM `mail_package` ORDER BY `id`", mpdata) != DB::DB_OK)
			return false;
		lc.reset(50);
		UInt32 last_pid = 0xFFFFFFFF;
		MailPackage * mp = NULL;
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(mpdata.id != last_pid)
			{
				last_pid = mpdata.id;
				mp = mailPackageManager.add(last_pid);
			}
			mp->push(mpdata.itemId, mpdata.itemCount);
		}
		lc.finalize();

		lc.prepare("Loading mails:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBMailData mdata;
		if(execu->Prepare("SELECT `mailId`, `playerId`, `sender`, `recvTime`, `flag`, `title`, `content`, `additionalId` FROM `mail` ORDER BY `playerId`, `mailId`", mdata) != DB::DB_OK)
			return false;
		lc.reset(500);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(mdata.playerId != last_id)
			{
				last_id = mdata.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			pl->GetMailBox()->newMail(mdata.id, mdata.sender, mdata.recvTime, mdata.flag, mdata.title, mdata.content, mdata.additionalId);
		}
		lc.finalize();

		lc.prepare("Loading boss data:");
		DBBossHP bosshp;
		if(execu->Prepare("SELECT `id`, `level`, `pos`, `hp` FROM `boss`", bosshp) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Boss * boss = bossManager.findBoss(bosshp.id);
			if(boss == NULL)
				continue;
			if(bosshp.pos == 0)
				boss->setLevel(bosshp.level, false);
			boss->setHP(bosshp.pos, bosshp.hp);
		}
		lc.finalize();

		lc.prepare("Loading boss damage data:");
		DBBossDamage bossdam;
		if(execu->Prepare("SELECT `id`, `playerId`, `damage`, `exp` FROM `boss_damage`", bossdam) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Boss * boss = bossManager.findBoss(bossdam.id);
			if(boss == NULL)
				continue;
			Player * player_ = globalPlayers[bossdam.playerId];
			if(player_ == NULL)
				continue;
			boss->setDamage(player_, bossdam.damage, bossdam.exp);
		}
		lc.finalize();

		lc.prepare("Loading player pending tasks:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBTaskData dtdata;
		if(execu->Prepare("SELECT `ownerId`, `taskId`, `acceptTime`, `step`, `timeBegin`, `timeEnd`, `completed`, `submit` FROM `task_instance` WHERE `submit` = 0 ORDER BY `ownerId`", dtdata) != DB::DB_OK)
			return false;
		lc.reset(2000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(dtdata.m_ownerId != last_id)
			{
				last_id = dtdata.m_ownerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(dtdata.m_TaskId);
			if (taskType.m_TypeId == 0) continue;
			TaskData* task = new TaskData();
			task->m_TaskId = dtdata.m_TaskId;
			task->m_Class = taskType.m_Class;
			task->m_SubType = taskType.m_SubType;
			task->m_AcceptNpc = taskType.m_AcceptNpc;
			task->m_SubmitNpc = taskType.m_SubmitNpc;
			task->m_LoopTask = taskType.m_LoopTask;
			task->m_AcceptMaxNum = taskType.m_AcceptMaxNum;
			task->m_OwnerId = dtdata.m_ownerId;
			task->m_AcceptTime = dtdata.m_AcceptTime;
			task->m_TimeBegin = dtdata.m_TimeBegin;
			task->m_TimeEnd = dtdata.m_TimeEnd;
			task->m_Completed = dtdata.m_Completed;
			task->m_Submit = dtdata.m_Submit;
			task->m_StepStr = dtdata.m_TaskStep;
			pl->GetTaskMgr()->LoadTask(task);
		}
		lc.finalize();

		lc.prepare("Loading player completed tasks:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBTaskCompleteData dtdata2;
		if(execu->Prepare("SELECT `ownerId`, `taskId`, `timeEnd` FROM `task_instance` WHERE `submit` = 1 ORDER BY `ownerId`", dtdata2) != DB::DB_OK)
			return false;
		lc.reset(4000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(dtdata2.m_ownerId != last_id)
			{
				last_id = dtdata2.m_ownerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(dtdata2.m_TaskId);
			if (taskType.m_TypeId == 0) continue;
			pl->GetTaskMgr()->LoadSubmitedTask(dtdata2.m_TaskId, dtdata2.m_TimeEnd);
		}
		lc.finalize();

		lc.prepare("Loading player day tasks:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBDayTaskData dtddata;
		if(execu->Prepare("SELECT `loopTask`, `ownerId`, `count`, `maxCount`, `maxFlushQualityCount`, `preTaskId`, `preFlushTime`, `preTaskQuality`, `autoCompletedTaskId`, `autoCompletedTaskAcceptTime`, `currDay`, `flushCount` FROM `day_task_instance` ORDER BY `ownerId`", dtddata) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(dtddata.m_ownerId != last_id)
			{
				last_id = dtddata.m_ownerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			DayTaskData* task = new DayTaskData();
			task->m_LoopTaskId = dtddata.m_LoopTaskId;
			task->m_Count = dtddata.m_Count;
			task->m_MaxCount = dtddata.m_MaxCount;
			task->m_MaxFlushQualityCount = dtddata.m_MaxFlushQualityCount;
			task->m_PreTaskId = dtddata.m_PreTaskId;
			task->m_PreFlushTime = dtddata.m_PreFlushTime;
			task->m_PreTaskQuality = dtddata.m_PreTaskQuality;
			task->m_AutoCompletedTaskId = dtddata.m_AutoCompletedTaksId;
			task->m_AutoCompletedTaskAcceptTime = dtddata.m_AutoCompletedTaskAcceptTime;
			task->m_Owner = dtddata.m_ownerId;
			task->m_CurrDay = dtddata.m_CurrDay;
			task->m_FlushCount = dtddata.m_FlushCount;
			pl->GetTaskMgr()->LoadDayTask(task);
		}
		lc.finalize();

		//load all sales put last
		lc.prepare("Loading sales:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		UInt32 now = TimeUtil::Now();
		DBSaleData dbsdata;
		if(execu->Prepare("SELECT `saleId`, `ownerId`, `status`, `saleTime`, `priceType`, `price`, `itemId`, `itemNum` FROM `sale` WHERE `status` < 4 ORDER BY `ownerId`", dbsdata) != DB::DB_OK)
			return false;
		lc.reset(100);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (dbsdata.ownerId != last_id)
			{
				last_id = dbsdata.ownerId;
				pl = globalPlayers[last_id];
			}
			if (pl == NULL)
				continue;
			ItemBase * item = NULL;
			if (IsEquipId(dbsdata.itemId))
				item = Package::BuildEquipFromDB(dbsdata.itemId);
			else
				item = Package::BuildItem(dbsdata.itemId, dbsdata.itemNum);
			if (item == NULL) continue;
			SALE_STATUS status = static_cast<SALE_STATUS>(dbsdata.status);
			if (status == SALE_SELL)
			{
				if (SaleMgr::isTimeoutSale(dbsdata.saleTime, now))
				{
					pl->GetSale()->addSaleMailFromDB(dbsdata.saleId, item);
					continue;
				}
				SaleData * sale = new SaleData();
				sale->_id = dbsdata.saleId;
				sale->_status = dbsdata.status;
				sale->_time = dbsdata.saleTime;
				sale->_priceType = dbsdata.priceType;
				sale->_price = dbsdata.price;
				sale->_item = item;
				sale->_owner = pl;
				gSaleMgr.addSaleFromDB(sale);

				SaleSellRespData * respData = new SaleSellRespData();
				respData->id = sale->_id;
				respData->price = sale->_price;
				respData->priceType = sale->_priceType;
				if(sale->_item->getName().length() <= 21)
					strcpy(respData->itemName, sale->_item->getName().c_str());
				else
				{
					memcpy(respData->itemName, sale->_item->getName().c_str(), 21);
					respData->itemName[21] = 0;
				}
				pl->GetSale()->addSaleFromDB(respData);
			}
			else if (status == SALE_BUY || status == SALE_CANCEL || status == SALE_TIMEOUT)
			{
				pl->GetSale()->addSaleMailFromDB(dbsdata.saleId, item, true);
			}
		}
		lc.finalize();

		//load all athletics record
		lc.prepare("Loading athletics records:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		Player * atker, *defer;
		atker = defer = NULL;
		DBAthleticsRecordData drd;
		if(execu->Prepare("SELECT `id`, `atker`, `defer`, `repid`, `time`, `winSide`, `awardType`, `awardAtkerCount` FROM `athletics_record` ORDER BY `time` desc", drd) != DB::DB_OK)
			return false;
		lc.reset(2000);
		bool ret1, ret2;
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			atker = globalPlayers[drd.atker];
			defer = globalPlayers[drd.defer];
			if (atker == NULL || defer == NULL)
				continue;
			ret1 = atker->GetAthletics()->addAthleticsDataFromDB(drd.id, 0, defer, drd.winSide, drd.awardType, drd.awardAtkerCount, drd.repid, drd.time);
			ret2 = defer->GetAthletics()->addAthleticsDataFromDB(drd.id, 1, atker, drd.winSide, drd.awardType, drd.awardAtkerCount, drd.repid, drd.time);
			if (!ret1 && !ret2)
			{
				DB().PushUpdateData("DELETE FROM `athletics_record` WHERE `id` = %u", drd.id);
			}
		}
		lc.finalize();
		
		//use for new year activity fix???????????????????
		lc.prepare("Loading exchange plane ticket:");
		DBExchangeTicket det;
		if(execu->Prepare("SELECT `playerId`, `exchange_num` FROM `exchange_ticket`", det) != DB::DB_OK)
			return false;
		lc.reset(100);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player *player = globalPlayers[det.playerId];
			if(player == NULL)
				continue;
			player->setTicketCount(det.count, false);
		}
		lc.finalize();
		/////////////////////////////////

		globalPlayers.enumerate(player_load, 0);

		return true;
	}

	bool GObjectManager::loadAllAthletics()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading athletics_rank:");
		DBAthleticsData dbd;
		if(execu->Prepare("SELECT `row`, `rank`, `ranker`, `maxRank`, `challengeNum`, `challengeTime`, `boxColor`, `boxType`, `boxCount`, `boxFlushTime`, `winStreak` FROM `athletics_rank` ORDER BY `rank`", dbd) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl = globalPlayers[dbd.ranker];
			if (pl == NULL)
				continue;
			pl->GetAthletics()->setEnterAthletics();
			AthleticsRankData * data = new AthleticsRankData();
			data->row = dbd.row;
			data->ranker = pl;
			data->rank = dbd.rank;
			data->maxrank = dbd.maxRank;
			data->challengenum = dbd.challengeNum;
			data->challengetime = dbd.challengeTime;
			//data->boxid = dbd.boxId;
			data->awardType = dbd.boxtype;
			data->awardCount = dbd.boxcount;
			data->boxcolor = dbd.boxcolor;
			data->boxflushtime = dbd.boxFlushTime;
			//AthleticsRank::buildBoxAward(data->boxid, data->awardType, data->awardCount, data->awardName);
			data->winstreak = dbd.winStreak;
			gAthleticsRank.addAthleticsFromDB(dbd.row, data);
		}
		lc.finalize();

		return true;
	}

	bool GObjectManager::delayLoad()
	{
		globalFighters.setAllDirty();
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading auto battle data:");
		DBAutoBattle dab;
		if(execu->Prepare("SELECT `playerId`, `npcId`, `count`, `interval` FROM `auto_battle`", dab) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl = globalPlayers[dab.playerId];
			if(pl == NULL)
				continue;
			pl->pushAutoBattle(dab.npcId, dab.count, dab.interval);
		}
		lc.finalize();

		lc.prepare("Loading auto dungeon challenge data:");
		DBDungeonAuto dda;
		if(execu->Prepare("SELECT `playerId`, `dungeonId`, `totalExp`, `won` FROM `dungeon_auto`", dda) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl = globalPlayers[dda.playerId];
			if(pl == NULL)
				continue;
			pl->pushAutoDungeon(dda.dungeonId, dda.totalExp, dda.won);
		}
		lc.finalize();

		//load training fighters
		lc.prepare("Loading fighter train table:");
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		Player * pl = NULL;
		DBFighterTrain dbft;
		if(execu->Prepare("SELECT `fgtId`, `ownerId`, `priceType`, `price`, `trainTime`, `trainRemain`, `checkTime`, `accExp` FROM `fighter_train` ORDER BY `ownerId`", dbft) != DB::DB_OK)
			return false;
		lc.reset(100);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (dbft.ownerId != last_id)
			{
				last_id = dbft.ownerId;
				pl = globalPlayers[last_id];
			}
			if (pl == NULL)
				continue;
			if(dbft.trainTime > 0x10000)
			{
				UInt32 trainTime = dbft.trainRemain / 60;
				UInt32 endTime = dbft.trainTime + dbft.trainRemain;
				if(endTime > dbft.checkTime)
				{
					UInt32 checkTime = (endTime - dbft.checkTime) / 60;
					if(trainTime > 1440)
						trainTime = 1440;
					if(checkTime > trainTime)
						checkTime = trainTime;
					pl->addTrainFighterFromDB(dbft.fighterId, dbft.priceType, dbft.price, trainTime, checkTime, dbft.accExp);
					DB().PushUpdateData("REPLACE INTO `fighter_train`(`fgtId`, `ownerId`, `priceType`, `price`, `trainTime`, `checkTime`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u)", dbft.fighterId, dbft.ownerId, dbft.priceType, dbft.price, trainTime, checkTime);
				}
				else
					DB().PushUpdateData("DELETE FROM `fighter_train` WHERE `fgtId` = %u AND `ownerId` = %"I64_FMT"u", dbft.fighterId, dbft.ownerId);
			}
			else
				pl->addTrainFighterFromDB(dbft.fighterId, dbft.priceType, dbft.price, dbft.trainTime, dbft.checkTime, dbft.accExp);
		}
		lc.finalize();

		return true;
	}

	bool GObjectManager::loadAllFriends()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading friends:");
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		Player * pl = NULL;
		DBFriend dbfr;
		if(execu->Prepare("SELECT `id`, `type`, `friendId` FROM `friend` WHERE `id` < `friendId` ORDER BY `id`", dbfr) != DB::DB_OK)
			return false;
		lc.reset(500);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(dbfr.id != last_id)
			{
				last_id = dbfr.id;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			Player * toadd = globalPlayers[dbfr.friendId];
			if(toadd == NULL)
				continue;
			switch(dbfr.type)
			{
			case 0:
				pl->addFriendFromDB(toadd);
				break;
			case 1:
				pl->addBlockFromDB(toadd);
				break;
			case 2:
				pl->addFoeFromDB(toadd);
				break;
			}
		}
		lc.finalize();
		return true;
	}

	static bool cacheClan(Clan * clan, int)
	{
		UInt8 cny = clan->getCountry();
		globalClansByCountry[cny].add(clan->getId(), clan);
		globalNamedClans[cny].add(clan->getName(), clan);
		globalGlobalNamedClans.add(clan->getName(), clan);
		clanCache.push(clan);
		return true;
	}

	inline bool dungeon_enum(const GData::DungeonData * dd, void *)
	{
		dungeonManager.add(new Dungeon(dd->getId(), dd));
		return true;
	}

	bool GObjectManager::LoadDungeon()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		
		LoadingCounter lc("Loading dungeon monster templates:");
		GData::DBDungeonMonster dmon;
		if(execu->Prepare("SELECT `id`, `formated`, `monsters`, `experience` FROM `dungeon_monster`", dmon) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			GData::DungeonMonster * dm = new GData::DungeonMonster(dmon.id);
			dm->formated = dmon.formated > 0;
			dm->exp = dmon.experience;
			if(dm->formated)
			{
				StringTokenizer tk(dmon.monsters, "|");
				for(size_t i = 0; i < tk.count(); ++ i)
				{
					StringTokenizer tk2(tk[i], ",");
					if(tk2.count() < 2)
						continue;
					Fighter * fgt = globalFighters[atoi(tk2[0].c_str())];
					std::vector<UInt8>& vl = dm->monsterPos[fgt];
					for(size_t j = 1; j < tk2.count(); ++ j)
						vl.push_back(atoi(tk2[j].c_str()));
					if(dm->fighter == NULL)
						dm->fighter = fgt;
				}
			}
			else
			{
				StringTokenizer tk(dmon.monsters, "|");
				if(tk.count() < 2)
					continue;
				{
					StringTokenizer tk2(tk[0], ",");
					if(tk2.count() == 0)
						continue;
					for(size_t i = 0; i < tk2.count(); ++ i)
					{
						Fighter * fgt = globalFighters[atoi(tk2[i].c_str())];
						std::vector<UInt8>& vl = dm->monsterPos[fgt];
						vl.push_back(1);
						if(dm->fighter == NULL)
							dm->fighter = fgt;
					}
				}
				{
					StringTokenizer tk2(tk[1], ",");
					if(tk2.count() == 0)
						continue;
					dm->minNum = atoi(tk2[0].c_str());
					if(tk2.count() > 1)
						dm->maxNum = atoi(tk2[1].c_str());
					else
						dm->maxNum = dm->minNum;
				}
			}
			GData::dungeonMonsters.add(dm);
		}
		lc.finalize();

		lc.prepare("Loading dungeon templates:");
		GData::DBDungeon dd;
		if(execu->Prepare("SELECT `id`, `name`, `location`, `type`, `lvlReq` FROM `dungeon`", dd) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			GData::DungeonData * dg = new GData::DungeonData(dd.id);
			dg->setName(dd.name);
			dg->location = dd.location;
			dg->type = dd.type;
			dg->levelReq = dd.lvlReq;
			GData::dungeons.add(dg);
		}
		lc.finalize();

		lc.prepare("Loading dungeon level templates:");
		GData::DBDungeonLevel dlvl;
		if(execu->Prepare("SELECT `id`, `level`, `monsterSet`, `lootSet` FROM `dungeon_level`", dlvl) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			GData::DungeonData * dd = const_cast<GData::DungeonData *>(GData::dungeons[dlvl.id / 10]);
			GData::DungeonLevel * dl = new GData::DungeonLevel;
			dl->monsterSet = GData::dungeonMonsters[dlvl.monsterSet];
			StringTokenizer tk(dlvl.lootSet, ",");
			for(size_t i = 0; i < tk.count(); ++ i)
			{
				dl->loots.push_back(GData::lootTable[atoi(tk[i].c_str())]);
			}
			std::vector<const GData::DungeonLevel *>& ddl = dd->monsters[(dlvl.id % 10) - 1];
			if(ddl.size() <= static_cast<size_t>(dlvl.level - 1))
				ddl.resize(dlvl.level);
			ddl[dlvl.level - 1] = dl;
		}
		lc.finalize();
		
		GData::dungeons.enumerate(dungeon_enum, NULL);

		lc.prepare("Loading dungeon player data:");
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		Player * pl = NULL;
		execu.reset(DB::gObjectDBConnectionMgr->GetExecutor());
		DBDungeonPlayer dp;
		if(execu->Prepare("SELECT `id`, `playerId`, `difficulty`, `level`, `count`, `totalCount`, `firstPass`, `counterEnd` FROM `dungeon_player` ORDER BY `playerId`", dp) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(dp.playerId != last_id)
			{
				last_id = dp.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			Dungeon * dg = dungeonManager[dp.id];
			if(dg == NULL)
				continue;
			StringTokenizer tk(dp.firstPass, ",");
			UInt32 fp[DUNGEON_DIFFICULTY_MAX] = {0, 0};
			size_t tcnt = tk.count();
			if(tcnt > DUNGEON_DIFFICULTY_MAX)
				tcnt = DUNGEON_DIFFICULTY_MAX;
			for(size_t j = 0; j < tcnt; ++ j)
				fp[j] = atoi(tk[j].c_str());
			dg->pushPlayer(pl, dp.difficulty, dp.level, dp.count, dp.totalCount, fp, dp.counterEnd);
		}
		lc.finalize();
		return true;
	}

	static bool configLoadedClanData(Clan * clan, void * data)
	{
		UInt32 allyClanId = clan->getAllyClanFromDB();
		if (allyClanId != 0)
		{
			Clan * allyClan = globalClans[allyClanId];
			if (allyClan == NULL)
				clan->addAllyClanFromDB(static_cast<UInt32>(0));
			else
			{
				clan->addAllyClanFromDB(allyClan);
				allyClan->addAllyClanFromDB(clan);
				allyClan->addAllyClanFromDB(clan->getId());
			}
		}

		std::vector<UInt32> enemyClanIds = clan->getEnemyClanFromDB();
		std::vector<UInt32>::iterator iter = enemyClanIds.begin();
		for (; iter < enemyClanIds.end();)
		{
			if (clan->hasAllyClan(*iter))
				iter = enemyClanIds.erase(iter);
			else
			{
				Clan * enemyClan = globalClans[*iter];
				if (enemyClan == NULL)
					iter = enemyClanIds.erase(iter);
				else
				{
					clan->addEnemyClanFromDB(enemyClan);
					++ iter;
				}
			}
		}
		clan->setEnemyClanFromDB(enemyClanIds);

		UInt32 now = *reinterpret_cast<UInt32 *>(data);
		clan->getClanBattle()->resumeClanBattleData(now);

		return true;
	}

	bool GObjectManager::loadAllClans()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		DBClanAssistant dcat;
		if (execu->Prepare("SELECT `id`, `formated`, `monsters` FROM `clan_hold_monster`", dcat) != DB::DB_OK)
			return false;
		while(execu->Next() == DB::DB_OK)
		{
			GData::ClanAssistant * cat = new GData::ClanAssistant(dcat.id);
			cat->id = dcat.id;
			cat->formated = dcat.formated;
			if(cat->formated)
			{
				//BOSS
				StringTokenizer tk(dcat.assistants, "|");
				{
					StringTokenizer tk1(tk[0], ",");
					GData::AssistantFData afd;
					afd.fighter = globalFighters[atoi(tk1[0].c_str())];
					if (afd.fighter == NULL)
						continue;
					afd.fighterNum = 1;
					UInt8 line = atoi(tk1[1].c_str());
					for (UInt8 i = line * 5; i <= line * 5 + 4; ++ i)
						afd.pos.push_back(i);
					cat->_assistList.push_back(afd);
				}
				//monster
				{
					StringTokenizer tk2(tk[1], ",");
					Fighter * fgt = globalFighters[atoi(tk2[0].c_str())];
					if (fgt == NULL)
						continue;
					GData::AssistantFData afd;
					afd.fighter = fgt;
					afd.fighterNum = atoi(tk2[1].c_str());
					UInt8 lineBegin = atoi(tk2[2].c_str());
					UInt8 lineEnd = atoi(tk2[3].c_str());
					for (UInt8 i = lineBegin * 5; i <= lineEnd * 5 + 4; ++ i)
						afd.pos.push_back(i);
					cat->_assistList.push_back(afd);
				}
			}
			else
			{
				StringTokenizer tk(dcat.assistants, ",");
				if (tk.count() < 2)
					continue;
				Fighter * fgt = globalFighters[atoi(tk[0].c_str())];
				if (fgt == NULL)
					continue;
				GData::AssistantFData afd;
				afd.fighter = fgt;
				afd.fighterNum = atoi(tk[1].c_str());
				for (UInt8 i = 0; i <= 24; ++ i)
					afd.pos.push_back(i);
				cat->_assistList.push_back(afd);
			}
			GData::clanAssistants.add(cat);
		}


		execu.reset(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) 
			return false;

		LoadingCounter lc("Loading clans:");
		DBClan cl;
		if (execu->Prepare("SELECT `id`, `name`, `rank`, `foundTime`, `founder`, `leader`, `contact`, `announce`, `purpose`, `proffer`, `grabAchieve`, `battleTime`, `nextBattleTime`, `allyClan`, `enemyClan1`, `enemyClan2`, `battleThisDay`, `battleStatus`, `southEdurance`, `northEdurance`, `hallEdurance`, `hasBattle` FROM `clan`", cl) != DB::DB_OK)
			return false;
		lc.reset(1000);
		Clan * clan = NULL;
		ClanBattle * clanBattle = NULL;
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (cl.id != 0)
			{
				clan = new Clan(cl.id, cl.name, cl.foundTime);
				clanBattle = clan->getClanBattle();
				clan->setContact(cl.contact, false);
				clan->setAnnounce(cl.announce, false);
				clan->setPurpose(cl.purpose, false);
				clan->setProffer(cl.proffer, false);
				clan->addAllyClanFromDB(cl.allyClan);
				clan->addEnemyClanFromDB(cl.enemyClan1);
				clan->addEnemyClanFromDB(cl.enemyClan2);
				clan->patchMergedName();
				clan->setFounder(cl.founder);
				clan->setLeaderId(cl.leader, false);
				clanBattle->setOwnerClanId(cl.id);
				if (!clanManager.validClanBattleTime(cl.battleTime))
				{
					cl.battleTime = 25;
					DB().PushUpdateData("UPDATE `clan` SET `battleTime` = 25 WHERE `id` = %u", cl.id);
				}
				if (!clanManager.validClanBattleTime(cl.nextBattleTime))
				{
					cl.battleTime = 25;
					DB().PushUpdateData("UPDATE `clan` SET `nextBattleTime` = 25 WHERE `id` = %u", cl.id);
				}
				clanBattle->setBattleTime(cl.battleTime);
				clanBattle->setNextBattleTime(cl.nextBattleTime);
				globalClans.add(cl.id, clan);
			}
			else
			{
				clanBattle = clanManager.getRobBattleClan();
				clanBattle->setOwnerClanId(cl.allyClan);
				clanBattle->setOwnerClanLev(cl.rank);
				clanBattle->setOwnerClanAtkCount(static_cast<UInt16>(cl.foundTime));
				clanBattle->setOwnerClanRank(cl.rank);
				clanBattle->setBattleTime(39);
				clanBattle->setNextBattleTime(39);
			}
			clanBattle->setGrabAcheive(cl.grabAchieve);
			clanBattle->setBattleThisDay(cl.battleThisDay);
			clanBattle->setBattleStatus(cl.battleStatus);
			clanBattle->setOldBattleStatus(cl.battleStatus);
			clanBattle->setSouthEdurance(cl.southEdurance);
			clanBattle->setNorthEdurance(cl.northEdurance);
			clanBattle->setHallEdurance(cl.hallEdurance);
		}
		lc.finalize();

		UInt32 now = TimeUtil::Now();
		UInt32 thisDay = TimeUtil::SharpDay(0, now);
		resetClanData(now);

		lc.prepare("Loading clan players:");
		DBClanPlayer cp;
		if (execu->Prepare("SELECT `id`, `playerId`, `joinTime`, `proffer`, `enterCount`, `achieveCount`, `thisDay`, `petFriendness1`, `petFriendness2`, `petFriendness3`, `petFriendness4`, `favorCount1`, `favorCount2`, `favorCount3`, `favorCount4`, `lastFavorTime1`, `lastFavorTime2`, `lastFavorTime3`, `lastFavorTime4` FROM `clan_player` ORDER BY `id`, `proffer` DESC, `joinTime` ASC", cp) != DB::DB_OK)
			return false;
		UInt32 lastId = 0xFFFFFFFF;
		lc.reset(1000);
		UInt32 rank = 0;
		bool hasLeader = false;
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(cp.id != lastId)
			{
				if (clan != NULL && !hasLeader)
					clan->fixLeaderId();
				clan = globalClans[cp.id];
				lastId = cp.id;
				rank = 0;
				hasLeader = false;
			}
			if(clan == NULL)
				continue;
			Player * pl = globalPlayers[cp.playerId];
			if(pl == NULL)
				continue;
			ClanMember * cm = new(std::nothrow) ClanMember();
			if (cm == NULL) return false;
			cm->player = pl;
			if (pl->getId() == clan->getLeaderId())
			{
				hasLeader = true;
				cm->cls = 4;
			}
			else
				cm->cls = Clan::buildRank(rank);
			cm->joinTime = cp.joinTime;
			cm->proffer = cp.proffer;
			if (thisDay != cp.thisDay)
			{
				DB().PushUpdateData("UPDATE `clan_player` SET `enterCount` = 0, `achieveCount` = 0, `thisDay` = %u WHERE `playerId` = %"I64_FMT"u", thisDay, pl->getId());
				cm->enterCount = 0;
				cm->achieveCount = 0;
			}
			else
			{
				cm->enterCount = cp.enterCount;
				cm->achieveCount = cp.achieveCount;
			}
			for(UInt32 i = 0; i < 4; i ++)
			{
				if(cp.petFriendness[i] != 0)
				{
					cm->clanPet.insert(std::make_pair(i + 7, GObject::ClanPlayerPet(cp.favorCount[i], cp.lastFavorTime[i], cp.petFriendness[i])));
				}
			}
			if (clan->join(cm))
			{
				if (cm->cls == 4)
					globalOwnedClans[pl->getCountry()].add(pl->getName(), clan);
				else
					++ rank;
			}
			else
				delete cm;
		}
		lc.finalize();
		globalClans.enumerate(cacheClan, 0);

		//load all clan skills
		lc.prepare("Loading clan skills:");
		DBClanSkill cs;
		if(execu->Prepare("SELECT `clanId`, `skillId`, `level`, `extra` FROM `clan_skill` ORDER BY `clanId`", cs) != DB::DB_OK)
			return false;
		lastId = 0xFFFFFFFF;
		clan = NULL;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (cs.clanId != lastId)
				clan = globalClans[cs.clanId];
			if (clan == NULL)
				continue;
			clan->getClanSkill()->addSkillFromDB(cs.skillId, cs.level, cs.extra);
		}
		lc.finalize();

		lc.prepare("Loading clan pending players:");
		DBClanPendingPlayer cpp;
		if(execu->Prepare("SELECT `id`, `playerId`, `class`, `opTime` FROM `clan_pending_player` ORDER BY `id`", cpp) != DB::DB_OK)
			return false;
		lastId = 0xFFFFFFFF;
		clan = NULL;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(cpp.id != lastId)
			{
				clan = globalClans[cpp.id];
			}
			if(clan == NULL)
				continue;
			Player * pl = globalPlayers[cpp.playerId];
			if(pl == NULL)
				continue;
			if (cpp.cls == 16)
				clan->apply(pl, cpp.opTime, false);
			else
				clan->invite(pl, cpp.opTime, false);
		}
		lc.finalize();


		lc.prepare("Loading clan donate record:");
		clan = NULL;
		lastId = 0xFFFFFFFF;
		DBClanDonateRecord ddr;
		if (execu->Prepare("SELECT `clanId`, `donateName`, `skillId`, `donateCount`, `donateTime` FROM `clan_donate_record` ORDER BY `clanId`, `donateTime`", ddr) != DB::DB_OK)
			return false;
		lc.reset(200);
		while (execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (ddr.clanId != lastId)
				clan = globalClans[ddr.clanId];
			if (clan == NULL)
			{
				DB().PushUpdateData("DELETE FROM `clan_donate_record` WHERE `clanId` = %u", ddr.clanId);
				continue;
			}
			clan->addClanDonateRecordFromDB(ddr.doanteName, ddr.skillId, ddr.donateCount, ddr.donateTime);
		}
		lc.finalize();

		globalClans.enumerate(configLoadedClanData, (void *)(&now));
		clanManager.resumeRobClanBattleData();
		//clanManager.updateAllocated();

		lc.prepare("Loading clan battle result record:");
		clan = NULL;
		lastId = 0xFFFFFFFF;
		clanBattle = NULL;
		DBClanBattleResult dbr;
		if (execu->Prepare("SELECT `id`, `battleTime`, `result` FROM `clan_battle_result` ORDER BY `id`, `battleTime`", dbr) != DB::DB_OK)
			return false;
		lc.reset(200);
		while (execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (dbr.id == 0)
				clanBattle = clanManager.getRobBattleClan();
			else if (dbr.id != lastId)
			{
				clan = globalClans[dbr.id];
				if (clan == NULL)
				{
					DB().PushUpdateData("DELETE FROM `clan_battle_result` WHERE `id` = %u", dbr.id);
					continue;
				}
				clanBattle = clan->getClanBattle();
			}
			clanBattle->getClanDynamicMsg()->addCBResultFromDB(dbr.id, dbr.battleTime, dbr.result);
		}
		lc.finalize();


		//加载宗族战场玩家数据
		lc.prepare("Loading clan battler:");
		clan = NULL;
		lastId = 0xFFFFFFFF;
		DBClanBattler cb;
		if (execu->Prepare("SELECT `id`, `battler`, `battlerLev`, `battleClanId`, `battleClanTime`, `battleHold`, `battleStatus`, `reliveNum`, `wins`, `serialWins`, `maxSerialWins`, `southEdurance`, `northEdurance`, `grabAchieve`, `hasEnter`, `hasAttack` FROM `clan_battler` ORDER BY `battleClanId`", cb) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (cb.battlerClanId == 0)
			{
				clan = NULL;
				clanBattle = clanManager.getRobBattleClan();				
			}
			else if (cb.battlerClanId != lastId)
			{
				clanBattle = NULL;
				clan = globalClans[cb.battlerClanId];
			}
			if (clan == NULL && clanBattle == NULL)
			{
				DB().PushUpdateData("DELETE FROM `clan_battler` WHERE `id` = %u", cb.id);
				continue;
			}
			if (clan != NULL)
				clanBattle = clan->getClanBattle();
			Player * pl = globalPlayers[cb.battler];
			if (pl == NULL || pl->getClan() == NULL)
			{
				DB().PushUpdateData("DELETE FROM `clan_battler` WHERE `id` = %u", cb.id);
				continue;
			}
			if (clan != NULL)
			{
				ClanBattleReportElem& elem = clan->getClanDynamicMsg()->getCBReport(cb.battleClanTime);
				elem.second.push_back(ClanBattleReport(pl->getName(), pl->getClan()->getName(), cb.battlerLev, cb.reliveNum, cb.serialWins, cb.maxSerialWins, clanBattle->getClanRelation(pl->getClan()), cb.northEdurance, cb.southEdurance, cb.grabAchieve));
			}
			if (cb.battleClanTime != thisDay || clanBattle->getOldBattleStatus() != 1)
				continue;
			ClanBattlePlayer * cbp = new(std::nothrow) ClanBattlePlayer(pl, cb.battleHold, 1);
			if (cbp == NULL)
				continue;
			cbp->id = cb.id;
			cbp->side = clanBattle->getClanRelation(pl->getClan());
			cbp->reliveNum = cb.reliveNum;
			cbp->wins = cb.wins;
			cbp->serailWins = cb.serialWins;
			cbp->maxSerailWins = cb.maxSerialWins;
			cbp->southEdurance = cb.southEdurance;
			cbp->northEdurance = cb.northEdurance;
			cbp->grabAchieve = cb.grabAchieve;
			cbp->offTime = now;
			cbp->status = cb.battleStatus;
			cbp->hasEnter = cb.hasEnter;
			cbp->hasAttack = cb.hasAttack;
			if (!clanBattle->resumePlayerToClanBattle(cbp, now))
			{
				delete cbp;
				DB().PushUpdateData("DELETE FROM `clan_battler` WHERE `id` = %u", cb.id);
				continue;	
			}
		}
		lc.finalize();
		clanManager.resumeRobClanBattleScore();

		lc.prepare("Loading clan repositories:");
		DBClanRepo crepo;
		if(execu->Prepare("SELECT `id`, `itemId`, `itemNum` FROM `clan_repo` ORDER BY `id`", crepo) != DB::DB_OK)
			return false;
		UInt32 last_cid = 0xFFFFFFFF;
		clan = NULL;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(last_cid != crepo.id)
			{
				last_cid = crepo.id;
				clan = globalClans[last_cid];
			}
			if(clan == NULL)
				continue;
			clan->setRepoNum(crepo.itemId, crepo.itemNum, false);
		}
		lc.finalize();

		lc.prepare("Loading clan pending rewards:");
		DBClanPendingReward cpr;
		if(execu->Prepare("SELECT `id`, `timeAlloc`, `playerId`, `itemId`, `itemNum` FROM `clan_pending_reward` ORDER BY `id`", cpr) != DB::DB_OK)
			return false;

		last_cid = 0xFFFFFFFF;
		clan = NULL;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(last_cid != cpr.id)
			{
				last_cid = cpr.id;
				clan = globalClans[last_cid];
			}
			if(clan == NULL)
				continue;
			Player * player = globalPlayers[cpr.playerId];
			if(player != NULL && player->getClan() == clan)
				clan->setPendingReward(player, cpr.timeAlloc, cpr.itemId, cpr.itemNum);
		}
		lc.finalize();

		lc.prepare("Loading clan reward allocation records:");
		DBClanRewardRecord crr;
		if(execu->Prepare("SELECT `id`, `type`, `timeAlloc`, `playerName`, `items` FROM `clan_reward_record` ORDER BY `id`", crr) != DB::DB_OK)
			return false;

		last_cid = 0xFFFFFFFF;
		clan = NULL;
		lc.reset(50);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(last_cid != crr.id)
			{
				last_cid = crr.id;
				clan = globalClans[last_cid];
			}
			if(clan == NULL)
				continue;
			GObject::AllocRecord ar;
			ar.allocType = crr.type;
			ar.playerName = crr.playerName;
			StringTokenizer tk(crr.items, "|");
			for(StringTokenizer::Iterator it = tk.begin(); it != tk.end(); ++ it)
			{
				StringTokenizer tk2(*it, ",");
				if(tk2.count() < 2)
					continue;
				AllocItem ai;
				ai.itemId = atoi(tk2[0].c_str());
				ai.itemNum = atoi(tk2[1].c_str());
				ar.allocItems.push_back(ai);
			}
			clan->addAllocRecord(crr.timeAlloc, ar, false);
		}
		lc.finalize();

		return true;
	}
	
	
	bool GObjectManager::loadEquipments()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading equipments:");
		DBEquipment dbe;
		if(execu->Prepare("SELECT `equipment`.`id`, `itemId`, `enchant`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`, `sockets`, `socket1`, `socket2`, `socket3`, `socket4`, `socket5`, `socket6`, `bindType`  FROM `equipment` LEFT JOIN `item` ON `equipment`.`id` = `item`.`id` OR `item`.`id` = NULL", dbe) != DB::DB_OK)
			return false;

		lc.reset(2000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			const GData::ItemBaseType * itype = GData::itemBaseTypeManager[dbe.itemId];
			if(itype == NULL)
				continue;
			switch(itype->subClass)
			{
			case Item_Weapon:
			case Item_Armor1:
			case Item_Armor2:
			case Item_Armor3:
			case Item_Armor4:
			case Item_Armor5:
			case Item_Ring:
			case Item_Amulet:
				{
					ItemEquipData ied;
					ied.enchant = dbe.enchant;
					ied.sockets = dbe.sockets;
					ied.gems[0] = dbe.socket1;
					ied.gems[1] = dbe.socket2;
					ied.gems[2] = dbe.socket3;
					ied.gems[3] = dbe.socket4;
					ied.gems[4] = dbe.socket5;
					ied.gems[5] = dbe.socket6;
					ied.enchant = dbe.enchant;
					ItemEquip * equip;
					switch(itype->subClass)
					{
					case Item_Weapon:
						equip = new ItemWeapon(dbe.id, *static_cast<const GData::ItemWeaponType *>(itype), ied);
						break;
					case Item_Armor1:
					case Item_Armor2:
					case Item_Armor3:
					case Item_Armor4:
					case Item_Armor5:
						equip = new ItemArmor(dbe.id, *static_cast<const GData::ItemEquipType *>(itype), ied);
						break;
					default:
						equip = new ItemEquip(dbe.id, *static_cast<const GData::ItemEquipType *>(itype), ied);
						break;
					}
					ItemEquipAttr2& ea2 = equip->getEquipAttr2();
					ea2.type1 = dbe.attrType1;
					ea2.value1 = dbe.attrValue1;
					ea2.type2 = dbe.attrType2;
					ea2.value2 = dbe.attrValue2;
					ea2.type3 = dbe.attrType3;
					ea2.value3 = dbe.attrValue3;
					equip->SetBindStatus(dbe.bindType > 0);
					equips[dbe.id] = equip;
				}
				break;
			default:
				break;
			}
		}
		lc.finalize();

		char fn[256];
		time_t curtime = time(NULL);
		strftime(fn, 256, "log/lossing_equip_%y%m%d_%H%M%S.log", localtime(&curtime));

		return true;
	}
	bool GObjectManager::LoadSpecialAward()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading SpecialAward:");
		DBSpecialAward sa;
		if(execu->Prepare("SELECT `rank`, `type`, `playerId` FROM SpecialAward ORDER BY `rank`", sa) != DB::DB_OK)
			return false;

		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			UInt8 type = sa.type;
			Player *pl = globalPlayers[sa.playerid];
			if(type < 4)
				gSpecialAward.AddAwardListFromDB(type, pl);
		}
		lc.finalize();

		return true;	
	}
	bool GObjectManager::LoadLuckyDraw()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading LuckyDrawCost");
		DBLuckyDraw ld;
		if(execu->Prepare("SELECT `playerId`, `cost` FROM `luckydrawgold`", ld)!= DB::DB_OK)
			return false;	
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * player = globalPlayers[ld.playerid];
			if(player == NULL) continue;
			luckyDraw.pushCostFromDB(player, ld.cost);
		}
		lc.finalize();

		return true;	
	}

	bool GObjectManager::LoadArena()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Arena Bets");
		DBArenaBet ab;
		if(execu->Prepare("SELECT `id`, `round`, `group`, `pos`, `tael` FROM `arena_bet` ORDER BY `id`", ab)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		Player * pl = NULL;
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(ab.id != last_id)
			{
				last_id = ab.id;
				pl = globalPlayers[last_id];
			}
			if (pl == NULL)
				continue;
			arena.pushBetFromDB(pl, ab.round, ab.group, ab.pos, ab.tael);
		}
		lc.finalize();

		return true;	
	}

	bool GObjectManager::unloadEquipments()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		std::map<UInt32, ItemEquip *>::iterator it = equips.begin();
		for(; it != equips.end(); ++ it)
		{
			if(it->second != NULL)
			{
				delete it->second;
			}
		}
		equips.clear();
		return true;
	}

	ItemEquip * GObjectManager::fetchEquipment( UInt32 id, bool record )
	{
		if(id == 0)
			return NULL;
		std::map<UInt32, ItemEquip *>::iterator it = equips.find(id);
		if(it == equips.end())
		{
			return NULL;
		}
		ItemEquip * base = it->second;
		equips.erase(it);
		return base;
	}

	ItemWeapon * GObjectManager::fetchWeapon( UInt32 id )
	{
		ItemEquip * equip = fetchEquipment(id);
		if(equip == NULL)
			return NULL;
		if(equip->GetItemType().subClass != static_cast<UInt8>(Item_Weapon))
		{
			delete equip;
			return NULL;
		}
		return static_cast<ItemWeapon *>(equip);
	}

	ItemArmor * GObjectManager::fetchArmor( UInt32 id )
	{
		ItemEquip * equip = fetchEquipment(id);
		if(equip == NULL)
			return NULL;
		if(equip->GetItemType().subClass < static_cast<UInt8>(Item_Armor1) || equip->GetItemType().subClass > static_cast<UInt8>(Item_Armor5))
		{
			delete equip;
			return NULL;
		}
		return static_cast<ItemArmor *>(equip);
	}
}

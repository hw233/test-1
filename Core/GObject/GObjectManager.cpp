#include "Config.h"
#include "GObjectManager.h"
#include "DB/DBConnectionMgr.h"
#include "GObjectDBExecHelper.h"
#include "DB/DBExecutor.h"
#include "Server/OidGenerator.h"
#include "Common/Itoa.h"

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
#include "AttainMgr.h"
#include "Trade.h"
#include "SaleMgr.h"
#include "AthleticsRank.h"
#include "Athletics.h"
#include "TradeCheck.h"
#include "Boss.h"
#include "Dungeon.h"
#include "SpecialAward.h"
#include "Prepaid.h"
#include "ClanTech.h"
#include "ClanBattle.h"
#include "ClanManager.h"
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
#include "GObject/PracticePlace.h"
#include "GObject/Copy.h"
#include "GObject/FrontMap.h"
#include "GObject/WBossMgr.h"
#include "GObject/TeamCopy.h"
#include "ActivityMgr.h"
#include "HoneyFall.h"
#include "TownDeamon.h"
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include "CFriend.h"
#include "ArenaBattle.h"
#include "GData/Store.h"
#include "LuckyDraw.h"
#include <fcntl.h>
#include "RealItemAward.h"
#include "NewRelation.h"
#include "GVar.h"
#include "Server/SysMsg.h"
#include "SingleHeroStage.h" 

namespace GObject
{
    URandom GRND(time(0)); // XXX: 加载和运行时都需要使用

	std::map<UInt32, ItemEquip *> GObjectManager::equips;

    UInt32 GObjectManager::_enchant_cost;
    UInt32 GObjectManager::_merge_cost;
    UInt32 GObjectManager::_detach_cost;
    UInt32 GObjectManager::_split_cost;
    UInt32 GObjectManager::_forge_cost;
    UInt32 GObjectManager::_split_chance[4][2];
    UInt32 GObjectManager::_merge_chance[11];
    UInt32 GObjectManager::_enchant_chance[2][6][12];
    UInt8  GObjectManager::_enchant_max[11];

    UInt32 GObjectManager::_trump_lorder_chance[6][12];
    UInt32 GObjectManager::_trump_exp_rank[6][12];
    AttrFactor GObjectManager::_trump_rank_factor[6][12];
    std::vector<UInt16> GObjectManager::_trump_maxrank_chance;

    std::vector<float> GObjectManager::_trump_smelt[4];

    UInt16 GObjectManager::_attrTypeChances[3][9];
    UInt16 GObjectManager::_attrChances[3][9];
    std::map<UInt8, stAttrMax*> GObjectManager::_attrMax;
    UInt16 GObjectManager::_attrDics[3][9];
    std::map<UInt8, stAttrMax*> GObjectManager::_attrTrumpMax;

    UInt32 GObjectManager::_socket_chance[6];
    UInt32 GObjectManager::_min_potential;
    UInt32 GObjectManager::_max_potential;
    UInt32 GObjectManager::_min_capacity;
    UInt32 GObjectManager::_max_capacity;
    std::vector<UInt32> GObjectManager::_potential_chance;
    std::vector<UInt32> GObjectManager::_capacity_chance;

    std::vector<UInt32> GObjectManager::_FFTypeChance;
    std::vector<UInt32> GObjectManager::_FFAttrChance;
    std::vector<UInt32> GObjectManager::_FFAttrMaxValProb;
    std::vector<UInt32> GObjectManager::_FFAttrMaxVal;

    UInt32 GObjectManager::_team_m_chance[3];
    UInt32 GObjectManager::_team_m_item[3];
    std::map<UInt32, UInt32> GObjectManager::_team_om_chance[3];
    std::map<UInt32, UInt32> GObjectManager::_team_om_item;

    std::vector<stHftChance> GObjectManager::_hft_chance[6][12];

    std::map<UInt8, stRingHpBase*> GObjectManager::_ringHpBase;
    float GObjectManager::_ringHpFactor[12];

    UInt8 GObjectManager::_evade_factor;
    UInt8 GObjectManager::_hitrate_factor;
    UInt8 GObjectManager::_critcal_factor;
    UInt8 GObjectManager::_pierce_factor;
    UInt8 GObjectManager::_tough_factor;

    float GObjectManager::_hiterate_max;
    float GObjectManager::_evade_max;
    float GObjectManager::_critical_max;
    float GObjectManager::_pierce_max;
    float GObjectManager::_tough_max;
    float GObjectManager::_counter_max;
    float GObjectManager::_mres_max;
    float GObjectManager::_cridmg_max;

    std::vector<std::vector<UInt32>> GObjectManager::_color_chance_gold;
    std::vector<std::vector<UInt32>> GObjectManager::_color_chance_free;

    std::map<UInt16, UInt16> GObjectManager::_battle_scene;

    std::vector<std::vector<YDItem>> GObjectManager::_yellow_diamond_award;
    std::vector<std::vector<YDItem>> GObjectManager::_d3d6_diamond_award;
    std::vector<std::vector<YDItem>> GObjectManager::_qplus_diamond_award;
    std::vector<YDItem>              GObjectManager::_year_yellow_diamond_award;
    std::vector<UInt32>              GObjectManager::_yellow_diamond_gem;

    std::vector<std::vector<UInt32>> GObjectManager::_soulEnchantChance;
    std::vector<UInt32> GObjectManager::_decSoulStateExp;

    GObjectManager:: vMergeStfs    GObjectManager:: _vMergeStfs;
    std::map <UInt32,  std::vector<UInt32> >   GObjectManager:: _mMergeStfsIndex;
    //std::map <UInt32, UInt32>  GObjectManager::_EUpgradeIdMap;
	bool GObjectManager::InitIDGen()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		//task
		UInt32 maxId;
		execu->Extract("SELECT max(`mailId`) FROM `mail`", maxId);
		IDGenerator::gMailOidGenerator.Init(maxId);
		execu->Extract("SELECT max(`id`) FROM `clan`", maxId);
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
		execu->Extract("SELECT max(`id`) FROM `athletics_event`", maxId);
		IDGenerator::gAthleticsEventOidGenerator.Init(maxId);

		return true;
	}

	void GObjectManager::loadAllData()
	{
        if(!loadGVar())
        {
            fprintf(stderr, "loadGVar error!\n");
            std::abort();
        }
        if(!loadRealItemAward())
        {
            fprintf(stderr, "loadRealItemAward error!\n");
            std::abort();
        }
        if(!loadEquipForge())
        {
            fprintf(stderr, "loadEquipForge error!\n");
            std::abort();
        }
		if(!loadMapData())
        {
            fprintf(stderr, "loadMapData error!\n");
            std::abort();
        }
        if(!loadAttrFactor())
        {
            fprintf(stderr, "loadMapData error!\n");
            std::abort();
        }
        if(!loadCopy())
        {
            fprintf(stderr, "loadCopy error!\n");
            std::abort();
        }
        if(!loadFrontMap())
        {
            fprintf(stderr, "loadFrontMap error!\n");
            std::abort();
        }
		if(!loadEquipments())
        {
            fprintf(stderr, "loadEquipments error!\n");
            std::abort();
        }
		if(!loadEquipmentsSpirit())
        {
            fprintf(stderr, "loadEquipmentsSpirit error!\n");
            std::abort();
        }
        if(!loadFightersPCChance())
        {
            fprintf(stderr, "loadFightersPCChance error!\n");
            std::abort();
        }
		if(!loadFighters())
        {
            fprintf(stderr, "loadFighters error!\n");
            std::abort();
        }
		if(!loadClanAssist())
        {
            fprintf(stderr, "loadClanAssist error!\n");
            std::abort();
        }
		if(!loadClanRobMonster())
        {
            fprintf(stderr, "loadClanRobMonster error!\n");
            std::abort();
        }
        if(!loadQQVipAward())
        {
            fprintf(stderr, "loadQQVipAward error!\n");
            std::abort();
        }
		if(!loadAllPlayers())
        {
            fprintf(stderr, "loadAllPlayers error!\n");
            std::abort();
        }
        //loadSecondSoul();
		if(!loadAllAthletics())
        {
            fprintf(stderr, "loadAllAthletics error!\n");
            std::abort();
        }
		if(!loadAllAthleticsEvent())
        {
            fprintf(stderr, "loadAllAthleticsEvent error!\n");
            std::abort();
        }
		if(!unloadEquipments())
        {
            fprintf(stderr, "unloadEquipments error!\n");
            std::abort();
        }
		if(!loadAllFriends())
        {
            fprintf(stderr, "loadAllFriends error!\n");
            std::abort();
        }
		if(!LoadDungeon())
        {
            fprintf(stderr, "LoadDungeon error!\n");
            std::abort();
        }
        if(!loadTeamCopy())
        {
            fprintf(stderr, "loadTeamCopy error!\n");
            std::abort();
        }
		if(!loadAllClans())
        {
            fprintf(stderr, "loadAllClans error!\n");
            std::abort();
        }
		if(!LoadSpecialAward())
        {
            fprintf(stderr, "LoadSpecialAward error!\n");
            std::abort();
        }
		if(!LoadArena())
        {
            fprintf(stderr, "LoadArena error!\n");
            std::abort();
        }
        if(!LoadPracticePlace())
        {
            fprintf(stderr, "LoadPracticePlace error!\n");
            std::abort();
        }
        if(!LoadWorldBoss())
        {
            fprintf(stderr, "LoadWorldBoss error!\n");
            std::abort();
        }
        if(!LoadTownDeamon())
        {
            fprintf(stderr, "LoadTownDeamon error!\n");
            std::abort();
        }
        if(!InitMoneyLog())
        {
            fprintf(stderr, "InitMoneyLog error!\n");
            std::abort();
        }
        if(!LoadShuoShuo()) // XXX: must before LoadHeroMemo
        {
            fprintf(stderr, "LoadShuoShuo error!\n");
            std::abort();
        }
        if(!LoadHeroMemo())
        {
            fprintf(stderr, "LoadHeroMemo error!\n");
            std::abort();
        }
        if(!LoadCFriendAwards())
        {
            fprintf(stderr, "LoadCFriendAwards error!\n");
            std::abort();
        }
        if(!LoadWBoss())
        {
            fprintf(stderr, "LoadWBoss error!\n");
            std::abort();
        }
        if(!LoadDiscount())
        {
            fprintf(stderr, "LoadDiscount error!\n");
            std::abort();
        }
        if(!LoadSoulItemChance())
        {
            fprintf(stderr, "LoadSoulItemChance error!\n");
            std::abort();
        }
        if(!LoadLuckyLog())
        {
            fprintf(stderr, "LoadLuckyLog error!\n");
            std::abort();
        }
        if(!loadRNR())
        {
            fprintf(stderr, "loadRNR error!\n");
            std::abort();
        }
        if(!loadNewRelation())
        {
            fprintf(stderr, "loadNewRelation error!\n");
            std::abort();
        }
        if(!loadSkillStrengthen())
        {
            fprintf(stderr, "loadSkillStrengthen error!\n");
            std::abort();
        }
        if(!loadQixi())
        {
            fprintf(stderr, "loadQixi error!\n");
            std::abort();
        }
        if(!loadArenaExtraBoard())
        {
            fprintf(stderr, "loadArenaExtraBoard error!\n");
            std::abort();
        }
        if(!loadJobHunter())
        {
            fprintf(stderr, "loadJobHunter error!\n");
            std::abort();
        }
        if(!loadCopyFrontWin())
        {
            fprintf(stderr, "loadCopyFrontWin error!\n");
            std::abort();
        }

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
		execu->Execute2("UPDATE `clan_player` AS `update_clan_player` SET `enterCount` = 0 WHERE `playerId` = `update_clan_player`.`playerId` AND `thisDay` <> %u", thisDay);

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
            if (mos.m_ID == 5021 ||
                mos.m_ID == 5273 ||
                mos.m_ID == 5092 ||
                mos.m_ID == 5088 ||
                mos.m_ID == 5091 ||
                mos.m_ID == 5006 ||
                mos.m_ID == 5272 ||
                mos.m_ID == 5097
                )
                mos.m_Hide = true;
            else
                mos.m_Hide = false;
			if(map == NULL)
			{
				//ERROR_LOG...
				continue;
			}
			map->AddObject(mos);
		}

        lua_State* L = lua_open();
        luaopen_base(L);
        luaopen_string(L);
        {
            std::string path = cfg.scriptPath + "Other/BattleScene.lua";
            lua_tinker::dofile(L, path.c_str());

            lua_tinker::table bs_table = lua_tinker::call<lua_tinker::table>(L, "getBattleScene");
            size_t bs_size = bs_table.size();
            for(UInt32 i = 0; i < bs_size; i ++)
            {
                lua_tinker::table tempTable = bs_table.get<lua_tinker::table>(i + 1);
                size_t tempSize = tempTable.size();
                UInt16 first = 0;
                UInt16 second = 0;
                if(tempSize > 1)
                {
                    first = tempTable.get<UInt16>(1);
                    second = tempTable.get<UInt16>(2);
                }
                else if(tempSize == 1)
                    first = tempTable.get<UInt16>(1);

                _battle_scene[first] = second;
            }
        }
        lua_close(L);

		return true;
	}

    bool GObjectManager::loadAttrFactor()
    {
        lua_State* L = lua_open();
        luaopen_base(L);
        luaopen_string(L);
        {
            std::string path = cfg.scriptPath + "formula/attribute.lua";
            lua_tinker::dofile(L, path.c_str());

            _evade_factor = lua_tinker::call<UInt8>(L, "getEvadeFactor");
            _hitrate_factor = lua_tinker::call<UInt8>(L, "getHitrateFactor");
            _critcal_factor = lua_tinker::call<UInt8>(L, "getCriticalFactor");
            _pierce_factor = lua_tinker::call<UInt8>(L, "getPierceFactor");
            _tough_factor = lua_tinker::call<UInt8>(L, "getToughFactor");

            _hiterate_max = lua_tinker::call<float>(L, "getHitrateMax");
            _evade_max = lua_tinker::call<float>(L, "getEvadeMax");
            _critical_max = lua_tinker::call<float>(L, "getCriticalMax");
            _pierce_max = lua_tinker::call<float>(L, "getPierceMax");
            _tough_max = lua_tinker::call<float>(L, "getToughMax");
            _counter_max = lua_tinker::call<float>(L, "getCounterMax");
            _mres_max = lua_tinker::call<float>(L, "getMagResMax");
            _cridmg_max = lua_tinker::call<float>(L, "getCriticalDmgMax");
        }
        lua_close(L);

        return true;
    }

    bool GObjectManager::loadCopy()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;

        LoadingCounter lc("Loading copy:");
		DBCopyData dbcd;
        lc.reset(2000);
		if(execu->Prepare("SELECT `playerId`, `id`, `floor`, `spot`, `lootlvl` FROM `player_copy` ORDER BY `playerId`,`id`", dbcd) != DB::DB_OK)
            return false;
		while(execu->Next() == DB::DB_OK)
		{
            playerCopy.addPlayer(dbcd.playerId, dbcd.id, dbcd.floor, dbcd.spot, dbcd.lootlvl);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadFrontMap()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;

        LoadingCounter lc("Loading frontmap:");
		DBFrontMapData dbcd;
        lc.reset(2000);
		if(execu->Prepare("SELECT `playerId`, `id`, `spot`, `count`, `status`, `lootlvl` FROM `player_frontmap` ORDER BY `playerId`,`id`", dbcd) != DB::DB_OK)
            return false;
		while(execu->Next() == DB::DB_OK)
		{
            frontMap.addPlayer(dbcd.playerId, dbcd.id, dbcd.spot, dbcd.count, dbcd.status, dbcd.lootlvl);
        }
        lc.finalize();
        return true;
    }

	bool GObjectManager::loadFighters()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading fighter templates:");
		DBFighter dbfgt;
		if(execu->Prepare("SELECT `id`, `name`, `class`, `level`, `sex`, `potential`, `capacity`, `skill`, `npc_weapon`, `strength`, `physique`, `agility`, `intelligence`, `will`, `soul`, `aura`, `auraMax`, `tough`, `attack`, `magatk`, `defend`, `magdef`, `hp`, `action`, `talent`, `hitrate`, `evade`, `critical`, `criticaldmg`, `pierce`, `counter`, `magres`, `extraPos` FROM `special_fighter_template`", dbfgt) != DB::DB_OK)
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
            fgt->setSkills(dbfgt.skill, false);
			fgt->setPotential(dbfgt.potential, false);
            fgt->setCapacity(dbfgt.capacity, false);
			fgt->strength = dbfgt.strength;
			fgt->physique = dbfgt.physique;
			fgt->agility = dbfgt.agility;
			fgt->intelligence = dbfgt.intelligence;
			fgt->will = dbfgt.will;
			fgt->soulMax = dbfgt.soul;
			fgt->baseSoul = dbfgt.soul;
			fgt->aura = dbfgt.aura;
			fgt->auraMax = dbfgt.auraMax;
			fgt->tough = dbfgt.tough;
			fgt->attack = dbfgt.attack;
			fgt->magatk = dbfgt.magatk;
			fgt->defend = dbfgt.defend;
			fgt->magdef = dbfgt.magdef;
			fgt->maxhp = dbfgt.hp;
			fgt->action = dbfgt.action;
			fgt->talent = dbfgt.talent;
			fgt->hitrate = dbfgt.hitrate;
			fgt->evade = dbfgt.evade;
			fgt->critical = dbfgt.critical;
			fgt->criticaldmg = dbfgt.criticaldmg / 100;
			fgt->pierce = dbfgt.pierce;
			fgt->counter = dbfgt.counter;
			fgt->magres = dbfgt.magres;

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
		}
		lc.finalize();
		globalFighters.setMaxFgtId(maxGF);
		globalFighters.buildSummonSet();

		lc.prepare("Loading NPC groups:");
		GData::DBNpcGroup dbng;
		if(execu->Prepare("SELECT `id`, `fighterId`, `formationId`, `type`, `experience`, `lootId`, `lootId1`, `lootId2`, `lootId3` FROM `npc_group`", dbng) != DB::DB_OK)
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

            for (size_t i = 0; i < 4; ++i)
            {
                StringTokenizer tk(dbng.lootId[i], ",");
                std::vector<const GData::LootItem *> lootItem;
                for(size_t j = 0; j < tk.count(); ++ j)
                {
                    const GData::LootItem * li = GData::lootTable[atoi(tk[j].c_str())];
                    if(li == NULL)
                        continue;
                    lootItem.push_back(li);
                }

                if (lootItem.size())
                    ngroup->addLoots(lootItem);
            }

			ngroup->calcBattlePoints(&bform);
            ngroup->setType(dbng.type);
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

        lua_close(L);

		return true;
	}

	bool find_buff(DBFighterBuffData& buff, UInt32 id)
	{
		return buff.fighterId == id;
	}

    void mergeCittaPages(Player* p)
    {
        if (!p)
            return;
        struct MergeFromToInfo
        {
            UInt16 from;
            UInt16 to;
        };

        static MergeFromToInfo mfti[] = {
            {6054, 6053},
            {6055, 6053},
            {6057, 6056},
            {6058, 6056},
            {6059, 6056},
            {6060, 6056},
            {6062, 6061},
            {6063, 6061},
            {6064, 6061},
            {6065, 6061},
            {6066, 6061},
            {6068, 6067},
            {6069, 6067},
            {6070, 6067},
            {6071, 6067},
            {6072, 6067},
            {6074, 6073},
            {6075, 6073},
            {6076, 6073},
            {6077, 6073},
            {6078, 6073},
            {6079, 6073},
            {6081, 6080},
            {6082, 6080},
            {6083, 6080},
            {6084, 6080},
            {6085, 6080},
            {6086, 6080},
            {6088, 6087},
            {6089, 6087},
            {6090, 6087},
            {6091, 6087},
            {6092, 6087},
            {6093, 6087},
            {6095, 6094},
            {6096, 6094},
            {6097, 6094},
            {6098, 6094},
            {6099, 6094},
            {6100, 6094},
            {6101, 6094},
            {6102, 6094},
            {6104, 6103},
            {6105, 6103},
            {6106, 6103},
            {6107, 6103},
            {6108, 6103},
            {6109, 6103},
            {6110, 6103},
            {6111, 6103},
            {6113, 6112},
            {6114, 6112},
            {6115, 6112},
            {6116, 6112},
            {6117, 6112},
            {6118, 6112},
            {6119, 6112},
            {6120, 6112},
        };

        Package* pkg = p->GetPackage();
        for (size_t i = 0; i < sizeof(mfti)/sizeof(MergeFromToInfo); ++i)
        {
            UInt16 num = pkg->GetItemNum(mfti[i].from, true);
            if (num)
            {
                pkg->DelItem(mfti[i].from, num, true);
                pkg->AddItem(mfti[i].to, num, true);
                num = 0;
            }

            num = pkg->GetItemNum(mfti[i].from, false);
            if (num)
            {
                pkg->DelItem(mfti[i].from, num, false);
                pkg->AddItem(mfti[i].to, num, false);
                num = 0;
            }
        }
    }

    void buchang1530_1(Player* p, ItemEquip* item)
    {
        if (!p || !item)
            return;

        UInt8 enchant = item->getItemEquipData().enchant;
        UInt8 count = 0;
        if (enchant == 1)
            count = 1;
        else if (enchant == 2)
            count = 2;
        else if (enchant == 3)
            count = 3;
        else if (enchant == 4)
            count = 4;
        else if (enchant == 5)
            count = 6;
        else if (enchant == 6)
            count = 12;
        else if (enchant == 7)
            count = 20;
        else if (enchant == 8)
            count = 30;

        if (!count && !item->getItemEquipData().trumpExp)
            return;

        SYSMSG(title, 4029);
        SYSMSGV(content, 4030, enchant, item->getItemEquipData().trumpExp);

        MailPackage::MailItem mitem[2] = {{515,count},{134,UInt32(0.3f*item->getItemEquipData().trumpExp/1000)}};
        Mail * mail = p->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFD0000/*free*/);
        if (mail)
            mailPackageManager.push(mail->id, mitem, 2, true);

        std::string strItems;
        for (int i = 0; i < 1; ++i)
        {
            strItems += Itoa(mitem[i].id);
            strItems += ",";
            strItems += Itoa(mitem[i].count);
            strItems += "|";
        }

        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, p->getId(), mail->id, BuChang1530, title, content, strItems.c_str(), mail->recvTime);
    }

    bool buchang1530_2(std::pair<const unsigned int, GObject::Fighter*>& f)
    {
        Fighter* fgt = f.second;
        if (!fgt)
            return true;
        Player* pl = fgt->getOwner();
        if (!pl)
            return true;

        std::vector<ItemEquip*> ret;
        fgt->findTrumpByTypeId(ret, 1530);
        for (size_t i = 0; i < ret.size(); ++i)
            buchang1530_1(pl, ret[i]);
        return true;
    }
    
    void buchang1530(Player* p)
    {
        return; // XXX: 不需要了
        if (!p)
            return;
        Package* pkg = p->GetPackage();
        std::vector<ItemEquip*> ret;
        pkg->FindEquipByTypeId(ret, 1530, true);
        for (size_t i = 0; i < ret.size(); ++i)
            buchang1530_1(p, ret[i]);
        p->foreachFighter(buchang1530_2);
    }

    void buchangMo(Player *p)
    {
        if (!p)
            return;
        if (p->hasFighterWithClass(4))
        {


            SYSMSG(title, 4072);
            SYSMSGV(content, 4073);

            MailPackage::MailItem mitem[1] = {{30,20}};
            Mail * mail = p->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFD0000/*free*/);
            if (mail)
                mailPackageManager.push(mail->id, mitem, 2, true);

            std::string strItems;
            strItems += Itoa(mitem[0].id);
            strItems += ",";
            strItems += Itoa(mitem[0].count);
            strItems += "|";

            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, p->getId(), mail->id, BuChangMo, title, content, strItems.c_str(), mail->recvTime);
        }
    }

    inline bool player_load(Player * p, int)
    {
        p->Load();
        gBlockbossmgr.addPlayerRank(p, p->getBlockBossLevel(), p->GetLev());
        //p->verifyFighter();
        //if (!GVAR.GetVar(GVAR_CITTASMERGE))
        //    mergeCittaPages(p);
        //if (!GVAR.GetVar(GVAR_1530BUCHANG))
        //    buchang1530(p);
        if (!GVAR.GetVar(GVAR_OLDRC7DAYBUCHANG))
            p->sendOldRC7DayAward();
        if (!GVAR.GetVar( GVAR_JOB_MO_PEXP))
            buchangMo(p);
		return true;
	}

	inline bool heromemo_loaded(Player * p, int)
	{
        p->initHeroMemo();
		return true;
	}

    inline bool shuoshuo_loaded(Player* p, int)
    {
        p->initShuoShuo();
        return true;
    }

    bool GObjectManager::loadQQVipAward()
    {
        lua_State* L = lua_open();
        luaopen_base(L);
        luaopen_string(L);
        luaopen_table(L);
        {
            std::string path = cfg.scriptPath + "Other/QQVipAward.lua";
            lua_tinker::dofile(L, path.c_str());

            lua_tinker::table ydTable = lua_tinker::call<lua_tinker::table>(L, "getYellowDiamondAward");
            size_t yd_size = ydTable.size();
            for(UInt32 i = 0; i < yd_size; i ++)
            {
                std::vector<YDItem> itemVt;
                itemVt.clear();
                lua_tinker::table itemTable = ydTable.get<lua_tinker::table>(i + 1);
                size_t itemSize = itemTable.size();
                for(UInt8 j = 0; j < itemSize; ++j)
                {
                    lua_tinker::table tempTable = itemTable.get<lua_tinker::table>(j + 1);
                    YDItem item = {0};

                    item.itemId = tempTable.get<UInt32>(1);
                    item.itemNum = tempTable.get<UInt8>(2);


                    itemVt.push_back(item);
                }
                _yellow_diamond_award.push_back(itemVt);
            }

            lua_tinker::table d3Table = lua_tinker::call<lua_tinker::table>(L, "getDouble3Double6Award");
            size_t d3_size = d3Table.size();
            for(UInt32 i = 0; i < d3_size; i ++)
            {
                std::vector<YDItem> itemVt;
                itemVt.clear();
                lua_tinker::table itemTable = d3Table.get<lua_tinker::table>(i + 1);
                size_t itemSize = itemTable.size();
                for(UInt8 j = 0; j < itemSize; ++j)
                {
                    lua_tinker::table tempTable = itemTable.get<lua_tinker::table>(j + 1);
                    YDItem item = {0};

                    item.itemId = tempTable.get<UInt32>(1);
                    item.itemNum = tempTable.get<UInt8>(2);

                    itemVt.push_back(item);
                }
                _d3d6_diamond_award.push_back(itemVt);
            }

            lua_tinker::table qplusTable = lua_tinker::call<lua_tinker::table>(L, "getQPlusAward");
            size_t qplus_size = qplusTable.size();
            for(UInt32 i = 0; i < qplus_size; i ++)
            {
                std::vector<YDItem> itemVt;
                itemVt.clear();
                lua_tinker::table itemTable = qplusTable.get<lua_tinker::table>(i + 1);
                size_t itemSize = itemTable.size();
                for(UInt8 j = 0; j < itemSize; ++j)
                {
                    lua_tinker::table tempTable = itemTable.get<lua_tinker::table>(j + 1);
                    YDItem item = {0};

                    item.itemId = tempTable.get<UInt32>(1);
                    item.itemNum = tempTable.get<UInt8>(2);

                    itemVt.push_back(item);
                }
                _qplus_diamond_award.push_back(itemVt);
            }

            lua_tinker::table yydTable = lua_tinker::call<lua_tinker::table>(L, "getYearYellowDiamondAward");
            size_t yyd_size = yydTable.size();
            for(UInt32 j = 0; j < yyd_size; j ++)
            {
                lua_tinker::table tempTable = yydTable.get<lua_tinker::table>(j + 1);
                YDItem item = {0};

                item.itemId = tempTable.get<UInt32>(1);
                item.itemNum = tempTable.get<UInt8>(2);

                _year_yellow_diamond_award.push_back(item);
            }

            lua_tinker::table ydGemTable = lua_tinker::call<lua_tinker::table>(L, "getYellowDiamondGem");
            size_t ydGem_size = ydGemTable.size();
            for(UInt32 j = 0; j < ydGem_size; j ++)
            {
                _yellow_diamond_gem.push_back(ydGemTable.get<UInt32>(j + 1));
            }

        }
        lua_close(L);

        return true;

    }

    UInt16 getCittaItemId(UInt16 citta)
    {
        UInt16 c2item[] = {
            27, 1306,
            39, 1311,
            47, 1315,
            53, 1318,
            57, 1319,
            63, 1322,
            32, 1308,
            41, 1313,
            33, 1309,
            40, 1312,
            59, 1321,
            80, 1324,
            36, 1310,
            31, 1307,
            52, 1317,
            45, 1314,
            58, 1320,
            66, 1323,
        };

        for (UInt16 i = 0; i < sizeof(c2item)/sizeof(UInt16); i += 2)
        {
            if (c2item[i] == citta)
                return c2item[i+1];
        }
        return 0;
    }

    bool cittaSplit2(std::pair<const unsigned int, GObject::Fighter*>& f)
    {
        Fighter* fgt = f.second;
        if (!fgt)
            return true;
        Player* pl = fgt->getOwner();
        if (!pl)
            return true;

        const std::vector<UInt16>& cittas = fgt->getCittas();
        if (!cittas.size())
            return true;

        size_t size = cittas.size();
        for (size_t i = 0; i < size; ++i)
        {
            UInt16 citta = cittas[i];
            if (citta)
            {
                UInt8 lvl = CITTA_LEVEL(citta);
                UInt16 id = CITTA_ID(citta);

                UInt16 itemid = getCittaItemId(id);
                if (!itemid)
                    continue;

                if (fgt->getCittasNum() < 40)
                {
                    UInt16 ncitta = CITTAANDLEVEL(itemid - 1200 + 1, lvl);
                    int idx = fgt->isCittaUp(citta);
                    if (fgt->addNewCitta(ncitta, true, true, idx>=0?true:false))
                        continue;
                }

                UInt32 exp = 0;
                for (UInt8 l = 1; l <= lvl; ++l)
                {
                    const GData::CittaBase* cb = GData::cittaManager[CITTAANDLEVEL(id,l)];
                    if (!cb)
                        continue;
                    exp += cb->pexp;
                }

                if (exp) {
                    UInt16 rCount1 = static_cast<UInt16>(exp / 1000000);
                    exp = exp % 1000000;
                    UInt16 rCount2 = static_cast<UInt16>(exp / 10000);
                    exp = exp % 10000;
                    UInt16 rCount3 = static_cast<UInt16>(exp / 100);

                    SYSMSG(title, 3003);
                    SYSMSGV(content, 3004, fgt->getName().c_str());
                    MailPackage::MailItem mitem[4] = {{31, rCount1}, {30, rCount2}, {29, rCount3}, {itemid, 1}};
                    MailItemsInfo itemsInfo(mitem, SplitCitta, 4);
                    GObject::Mail * pmail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                    if (pmail != NULL)
                    {
                        GObject::mailPackageManager.push(pmail->id, mitem, 4, true);
                    }
                }
            }
        }

        return true;
    }

    void cittaSplit(Player* pl)
    {
        pl->foreachFighter(cittaSplit2);
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
		if(execu->Prepare("SELECT `player`.`id`, `name`, `gold`, `coupon`, `tael`, `coin`, `prestige`, `status`, `country`, `title`, `titleAll`, `archievement`, `attainment`, `qqvipl`, `qqvipyear`, `qqawardgot`, `qqawardEnd`, `ydGemId`, `location`, `inCity`, `lastOnline`, `newGuild`, `packSize`, `mounts`, `icCount`, `piccount`, `nextpicreset`, `formation`, `lineup`, `bossLevel`, `totalRecharge`, `nextReward`, `nextExtraReward`, `lastExp`, `lastResource`, `tavernId`, `bookStore`, `shimen`, `fshimen`, `yamen`, `fyamen`, `clantask`, `copyFreeCnt`, `copyGoldCnt`, `copyUpdate`, `frontFreeCnt`, `frontGoldCnt`, `frontUpdate`, `formations`, `atohicfg`, `gmLevel`, `wallow`, `dungeonCnt`, `dungeonEnd`, UNIX_TIMESTAMP(`created`), `locked_player`.`lockExpireTime`, `openid` FROM `player` LEFT JOIN `locked_player` ON `player`.`id` = `locked_player`.`player_id`", dbpd) != DB::DB_OK)
            return false;

		lc.reset(200);
		UInt32 tlimit = TimeUtil::SharpDay(-1);
		UInt32 nextDay = TimeUtil::SharpDay(1);
		while(execu->Next() == DB::DB_OK)
		{
            if (!dbpd.id)
                continue;

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
				DB1().PushUpdateData("UPDATE `player` SET `gold` = 10 WHERE `id` = %"I64_FMT"u", id);
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

            if (dbpd.pdata.packSize < PlayerData::INIT_PACK_SIZE)
            {
                dbpd.pdata.packSize = PlayerData::INIT_PACK_SIZE;
				DB1().PushUpdateData("UPDATE `player` SET `packSize` = %u WHERE `id` = %"I64_FMT"u", dbpd.pdata.packSize, id);
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
					//UInt32 seed = ((rwd & 0x0F000000) >> 24) + static_cast<UInt32>(id);
					pl->setPlayerData(dbpd.pdata);
					pl->genOnlineRewardItems();
				}
                pl->recalcVipLevel();
			}

			{
                bool fault = false;
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
                                if(count >8)
                                {
                                    PLAYER_DATA(pl, tavernOrangeCount) = atoi(tk[8].c_str());
                                    if (PLAYER_DATA(pl, tavernOrangeCount) > 10000)
                                    {
                                        PLAYER_DATA(pl, tavernOrangeCount) = 0;
                                        fault = true;
                                    }
                                    if(count > 9)
                                    {
                                        char * endptr;
                                        pl->setNextTavernUpdate(strtoul(tk[9].c_str(), &endptr, 10));
                                        break;
                                    }
                                }
							}
						}
						pl->setNextTavernUpdate(1);
					} while(0);
				}
				else
					pl->setNextTavernUpdate(0);
                if (fault)
                    pl->writeTavernIds();
			}

            if (dbpd.shimen.length())
            {
				StringTokenizer tk(dbpd.shimen, "|");
				size_t count = tk.count();
				if(count > 0)
				{
					do {
						size_t tcount = count;
						if(count > 3)
						{
							PLAYER_DATA(pl, smFreeCount) = atoi(tk[count - 3].c_str());
                            PLAYER_DATA(pl, smFinishCount) = atoi(tk[count - 2].c_str());
                            PLAYER_DATA(pl, smAcceptCount) = atoi(tk[count - 1].c_str());
						}

						if(tcount > 32)
							tcount = 29;
                        if(tcount > PLAYER_DATA(pl, smAcceptCount))
                            tcount = PLAYER_DATA(pl, smAcceptCount);

						for(size_t j = 0; j < tcount; ++ j)
						{
                            StringTokenizer tk1(tk[j].c_str(), ",");
                            if(tk1.count() > 1)
                            {
                                UInt32 taskId = atoi(tk1[0].c_str());
                                UInt8 color = atoi(tk1[1].c_str());
                                if( taskId != 0 )
                                {
                                    PLAYER_DATA(pl, shimen).push_back(taskId);
                                    PLAYER_DATA(pl, smcolor).push_back(color);
                                }
                            }
						}
					} while(0);
				}
			}

            if (dbpd.fshimen.length())
            {
				StringTokenizer tk(dbpd.fshimen, "|");
				size_t count = tk.count();
				if(count > 0)
				{
					do {
						size_t tcount = count;
						if(tcount > 6)
							tcount = 6;
						for(size_t j = 0; j < tcount; ++ j)
						{
                            StringTokenizer tk1(tk[j].c_str(), ",");
                            if(tk1.count() > 1)
                            {
                                PLAYER_DATA(pl, fshimen)[j] = atoi(tk1[0].c_str());
                                PLAYER_DATA(pl, fsmcolor)[j] = atoi(tk1[1].c_str());
                            }
						}
					} while(0);
				}
			}

            if (dbpd.yamen.length())
            {
				StringTokenizer tk(dbpd.yamen, "|");
				size_t count = tk.count();
				if(count > 0)
				{
					do {
						size_t tcount = count;
						if(count > 3)
						{
							PLAYER_DATA(pl, ymFreeCount) = atoi(tk[count - 3].c_str());
                            PLAYER_DATA(pl, ymFinishCount) = atoi(tk[count - 2].c_str());
                            PLAYER_DATA(pl, ymAcceptCount) = atoi(tk[count - 1].c_str());
						}

						if(tcount > 32)
							tcount = 29;
                        if(tcount > PLAYER_DATA(pl, ymAcceptCount))
                            tcount = PLAYER_DATA(pl, ymAcceptCount);

						for(size_t j = 0; j < tcount; ++ j)
						{
                            StringTokenizer tk1(tk[j].c_str(), ",");
                            if(tk1.count() > 1)
                            {
                                UInt32 taskId = atoi(tk1[0].c_str());
                                UInt8 color = atoi(tk1[1].c_str());
                                if(taskId != 0)
                                {
                                    PLAYER_DATA(pl, yamen).push_back(taskId);
                                    PLAYER_DATA(pl, ymcolor).push_back(color);
                                }
                            }
						}
					} while(0);
				}
			}

            if (dbpd.fyamen.length())
            {
				StringTokenizer tk(dbpd.fyamen, "|");
				size_t count = tk.count();
				if(count > 0)
				{
					do {
						size_t tcount = count;
						if(tcount > 6)
							tcount = 6;
						for(size_t j = 0; j < tcount; ++ j)
						{
                            StringTokenizer tk1(tk[j].c_str(), ",");
                            if(tk1.count() > 1)
                            {
                                PLAYER_DATA(pl, fyamen)[j] = atoi(tk1[0].c_str());
                                PLAYER_DATA(pl, fymcolor)[j] = atoi(tk1[1].c_str());
                            }
						}
					} while(0);
				}
			}

            if (dbpd.clantask.length())
            {
				StringTokenizer tk(dbpd.clantask, ",");
                if(tk.count() > 1)
                {
                    PLAYER_DATA(pl, clanTaskId) = atoi(tk[0].c_str());
                    PLAYER_DATA(pl, ctFinishCount) = atoi(tk[1].c_str());
                }
            }

            if (dbpd.formations.length())
            {
				StringTokenizer tk(dbpd.formations, ",");
				size_t count = tk.count();
                for(size_t idx = 0; idx < count; ++ idx)
                {
                    pl->addNewFormation(atoi(tk[idx].c_str()));
                }
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
            pl->setAtoHICfg(dbpd.atohicfg);

			pl->patchMergedName();
			globalPlayers.add(id, pl);
			if(dbpd.pdata.created >= tlimit)
			{
				newPlayers.add(id, pl);
			}
			globalNamedPlayers.add(pl->getName(), pl);
            if (dbpd.titleAll.length())
            {
				StringTokenizer tk(dbpd.titleAll, "|");
				size_t count = tk.count();
                for(size_t idx = 0; idx < count; ++ idx)
			    {
                    StringTokenizer tk1(tk[idx].c_str(), ",");
                    if(tk1.count() > 1)
                        pl->loadTitleAll(atoi(tk1[0].c_str()), atoi(tk1[1].c_str()));
                    else
                        pl->loadTitleAll(atoi(tk1[0].c_str()), 0);
                }
            }
            else
                pl->loadTitleAll(0, 0);

            if(!pl->hasTitle(0))
                pl->fixOldVertionTitle(0);
            if(!pl->hasTitle(dbpd.pdata.title))
                pl->fixOldVertionTitle(dbpd.pdata.title);
            pl->setOpenId(dbpd.openid, true);
		}
		lc.finalize();

		lc.prepare("Loading Player HoneyFall:");
        pl = NULL;
		DBHoneyFall hfData;
		if(execu->Prepare("SELECT `playerId`, `type`, `value` FROM `player_honeyfall`", hfData) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            pl = globalPlayers[hfData.playerId];
            if(pl == NULL)
                continue;

            HoneyFall* hf = pl->getHoneyFall();
            hf->setHftValue(hfData.type, hfData.value);
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

        lc.prepare("Loading player vars:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
        pl = NULL;
        DBPlayerVar playerVar;
        if(execu->Prepare("SELECT `playerId`, `id`, `data`, `over` FROM `var` ORDER BY `playerId`", playerVar) != DB::DB_OK)
            return false;
        lc.reset(100);
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if(playerVar.playerId != last_id)
            {
                last_id = playerVar.playerId;
                pl = globalPlayers[last_id];
            }
            if(pl == NULL) continue;
            pl->LoadVar(playerVar.id, playerVar.data, playerVar.overTime);
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
		}
		lc.finalize();

        // ???ɼ???
        lc.prepare("Loading clan skill:");
        DBClanSkill cs;
		if(execu->Prepare("SELECT `playerId`, `skillId`, `level` FROM `clan_skill` ORDER BY `skillId`, `playerId`", cs) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl = globalPlayers[cs.playerId];
			if(pl == NULL)
				continue;
            pl->addClanSkillFromDB(cs.skillId, cs.level);
		}
		lc.finalize();

		lc.prepare("Loading fighters:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
        UInt8 lvl_max = 0;
		DBFighter2 specfgtobj;
        //if(execu->Prepare("SELECT `fighter`.`id`, `fighter`.`playerId`, `potential`, `capacity`, `level`, `relvl`, `experience`, `practiceExp`, `hp`, `fashion`, `weapon`, `armor1`, `armor2`, `armor3`, `armor4`, `armor5`, `ring`, `amulet`, `peerless`, `talent`, `trump`, `acupoints`, `skill`, `citta`, `fighter`.`skills`, `cittas`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`, `fighterId`, `cls`, `xinxiu`, `practiceLevel`, `stateLevel`, `stateExp`, `second_soul`.`skills`, `elixir`.`strength`, `elixir`.`physique`, `elixir`.`agility`, `elixir`.`intelligence`, `elixir`.`will`, `elixir`.`soul`, `elixir`.`attack`,`elixir`.`defend`, `elixir`.`critical`, `elixir`.`pierce`, `elixir`.`evade`, `elixir`.`counter`, `elixir`.`tough`, `elixir`.`action`, `fighter`.`hideFashion` FROM `fighter` LEFT JOIN `second_soul` ON `fighter`.`id`=`second_soul`.`fighterId` AND `fighter`.`playerId`=`second_soul`.`playerId` LEFT JOIN `elixir` ON `fighter`.`id`=`elixir`.`id` AND `fighter`.`playerId`=`elixir`.`playerId` ORDER BY `fighter`.`playerId`", specfgtobj) != DB::DB_OK)
		if(execu->Prepare("SELECT `fighter`.`id`, `fighter`.`playerId`, `potential`, `capacity`, `level`, `relvl`, `experience`, `practiceExp`, `hp`, `halo`, `fashion`, `weapon`, `armor1`, `armor2`, `armor3`, `armor4`, `armor5`, `ring`, `amulet`, `peerless`, `talent`, `trump`, `acupoints`, `skill`, `citta`, `fighter`.`skills`, `cittas`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`, `fighterId`, `cls`, `xinxiu`, `practiceLevel`, `stateLevel`, `stateExp`, `second_soul`.`skills`, `elixir`.`strength`, `elixir`.`physique`, `elixir`.`agility`, `elixir`.`intelligence`, `elixir`.`will`, `elixir`.`soul`, `elixir`.`attack`,`elixir`.`defend`, `elixir`.`critical`, `elixir`.`pierce`, `elixir`.`evade`, `elixir`.`counter`, `elixir`.`tough`, `elixir`.`action`,`fighter`.`hideFashion` FROM `fighter` LEFT JOIN `second_soul` ON `fighter`.`id`=`second_soul`.`fighterId` AND `fighter`.`playerId`=`second_soul`.`playerId` LEFT JOIN `elixir` ON `fighter`.`id`=`elixir`.`id` AND `fighter`.`playerId`=`elixir`.`playerId` ORDER BY `fighter`.`playerId`", specfgtobj) != DB::DB_OK)
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
            if(pl->isMainFighter(specfgtobj.id) && specfgtobj.level > 28)
            {
                GObject::LevelPlayers* lvPlayer = NULL;
                GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(specfgtobj.level);
                if(it != GObject::globalLevelsPlayers.end())
                     lvPlayer = it->second;

                if(lvPlayer == NULL)
                {
                    lvPlayer = new GObject::LevelPlayers();
                    GObject::globalLevelsPlayers[specfgtobj.level] = lvPlayer;
                }

                lvPlayer->push_back(pl->getId());

                if(!GObject::shStageMgr.getActive())
                {
                    if(specfgtobj.level >= SINGLE_HERO_OPEN_LEVEL)
                        GObject::shStageMgr.incActive(1);
                }
            }

            if(specfgtobj.fighterId != 0 && specfgtobj.level >= 60)
            {
                SecondSoul* secondSoul = new SecondSoul(fgt2, specfgtobj.cls, specfgtobj.practiceLevel, specfgtobj.stateExp, specfgtobj.stateLevel, specfgtobj.xinxiu);
                StringTokenizer tokenizer(specfgtobj.skills_2nd, ",");
                int idx = 0;
                for(size_t j = 0; j < tokenizer.count(); ++ j)
                {
                    UInt16 skillId = atoi(tokenizer[j].c_str());
                    secondSoul->setSoulSkill(idx, skillId);
                    ++ idx;
                }
                fgt2->setSecondSoul(secondSoul);
            }

#define MAXVAL(x,y) { if (x > y) x = y; }
#define MV 200
#define MV1 10
            ElixirAttr attr;
            attr.strength = specfgtobj.strength;
            MAXVAL(attr.strength, MV);
            attr.physique = specfgtobj.physique;
            MAXVAL(attr.physique, MV);
            attr.agility = specfgtobj.agility;
            MAXVAL(attr.agility, MV);
            attr.intelligence = specfgtobj.intelligence;
            MAXVAL(attr.intelligence, MV);
            attr.will = specfgtobj.will;
            MAXVAL(attr.will, MV);
            attr.soul = specfgtobj.soul;
            MAXVAL(attr.soul, MV);
            attr.attack = specfgtobj.attack;
            MAXVAL(attr.attack, MV);
            attr.defend = specfgtobj.defend;
            MAXVAL(attr.defend, MV);
            attr.critical = specfgtobj.critical;
            MAXVAL(attr.critical, MV1);
            attr.pierce = specfgtobj.pierce;
            MAXVAL(attr.pierce, MV1);
            attr.evade = specfgtobj.evade;
            MAXVAL(attr.evade, MV1);
            attr.counter = specfgtobj.counter;
            MAXVAL(attr.counter, MV1);
            attr.tough = specfgtobj.tough;
            MAXVAL(attr.tough, MV1);
            attr.action = specfgtobj.action;
            MAXVAL(attr.action, MV);
            fgt2->setElixirAttr(attr);
#undef MV
#undef MV1
#undef MAXVAL

			fgt2->setPotential(specfgtobj.potential, false);
            fgt2->setCapacity(specfgtobj.capacity, false);
			fgt2->setLevel(specfgtobj.level, true);
			fgt2->setExp(specfgtobj.experience);
			fgt2->setPExp(specfgtobj.practiceExp);
			fgt2->setCurrentHP(specfgtobj.hp, false);
            fgt2->setAcupoints(specfgtobj.acupoints, false);
			fgt2->setHalo(fetchHalo(specfgtobj.halo), false);
			fgt2->setFashion(fetchFashion(specfgtobj.fashion), false);
			fgt2->setWeapon(fetchWeapon(specfgtobj.weapon), false);
			fgt2->setArmor(0, fetchArmor(specfgtobj.armor1), false);
			fgt2->setArmor(1, fetchArmor(specfgtobj.armor2), false);
			fgt2->setArmor(2, fetchArmor(specfgtobj.armor3), false);
			fgt2->setArmor(3, fetchArmor(specfgtobj.armor4), false);
			fgt2->setArmor(4, fetchArmor(specfgtobj.armor5), false);
			fgt2->setRing(fetchEquipment(specfgtobj.ring), false);
			fgt2->setAmulet(fetchEquipment(specfgtobj.amulet), false);
            fgt2->setTrump(specfgtobj.trump, false);
            fgt2->setPeerless(specfgtobj.peerless, false); // XXX: must after setTrump
            fgt2->setCittas(specfgtobj.cittas, false);
            fgt2->setUpCittas(specfgtobj.citta, false);
            //fgt2->setSkills(specfgtobj.skills, false);
            fgt2->setUpSkills(specfgtobj.skill, false);
            fgt2->setAttrType1(specfgtobj.attrType1);
            fgt2->setAttrValue1(specfgtobj.attrValue1);
            fgt2->setAttrType2(specfgtobj.attrType2);
            fgt2->setAttrValue2(specfgtobj.attrValue2);
            fgt2->setAttrType3(specfgtobj.attrType3);
            fgt2->setAttrValue3(specfgtobj.attrValue3);
            fgt2->setHideFashion(specfgtobj.hideFashion,false);
			pl->addFighter(fgt2, false, true);
            if (specfgtobj.level > lvl_max)
                lvl_max = specfgtobj.level;

            if(pl->isMainFighter(specfgtobj.id) && specfgtobj.level >= 70)
                ArenaBattle::Instance().addLevelMan(true);
		}
		lc.finalize();
        worldBoss.setLevel(lvl_max);

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
#if 1
		if(execu->Prepare("SELECT `id`, `ownerId`, `itemNum`, `bindType` FROM `item` ORDER BY `ownerId`", idata) != DB::DB_OK)
#else
		if(execu->Prepare("SELECT `id`, `ownerId`, `itemNum`, `bindType` FROM `item` ORDER BY `ownerId`, `bindType` DESC", idata) != DB::DB_OK)
#endif
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
#if 1
			if (!IsEquipId(idata.id))
				pl->GetPackage()->AddItemFromDB(idata.id, idata.itemNum, idata.bindType != 0);
#else
			if (!IsEquipId(idata.id))
            {
                if (idata.id >= 1200 && idata.id <= 1499)
                    pl->GetPackage()->AddItemFromDB(idata.id, idata.itemNum, idata.bindType != 0);
                else
                {
                    if (!idata.bindType)
                    {
                        UInt16 num = pl->GetPackage()->GetItemNum(idata.id, true);
                        if (num)
                        {
                            DB1().PushUpdateData("DELETE FROM `item` where `id` = %u and `ownerId` = %"I64_FMT"u and `bindType` = 0", idata.id, pl->getId());
                            DB1().PushUpdateData("UPDATE `item` set `itemNum` = `itemNum` + %u where `id` = %u and `ownerId` = %"I64_FMT"u",
                                    idata.itemNum, idata.id, pl->getId());
                        }
                        else
                        {
                            DB1().PushUpdateData("UPDATE `item` set `bindType` = 1 where `id` = %u and `ownerId` = %"I64_FMT"u",
                                    idata.id, pl->getId());
                        }
                        pl->GetPackage()->AddItemFromDB(idata.id, idata.itemNum+num, true);
                    }
                    else
                        pl->GetPackage()->AddItemFromDB(idata.id, idata.itemNum, true);
                }
            }
#endif
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

        lc.prepare("Loading team copy player:");
        last_id = 0xFFFFFFFFFFFFFFFFull;
        DBTeamCopyPlayer dbtcp;
        if(execu->Prepare("SELECT `playerId`, `copyId`, `type`, `pass`, `passTimes`, `vTime` FROM `teamcopy_player` ORDER BY `playerId`, `copyId`, `type`", dbtcp) != DB::DB_OK)
            return false;
        lc.reset(500);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl = globalPlayers[dbtcp.playerId];
            if(!pl)
                continue;
            TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
            if(!tcpInfo)
                continue;

            tcpInfo->setPassFromDB(dbtcp.copyId, dbtcp.type, dbtcp.pass);
            tcpInfo->setPassTimesFromDB(dbtcp.copyId, dbtcp.type, dbtcp.passTimes, dbtcp.vTime);
        }
		lc.finalize();

        lc.prepare("Loading team copy player award:");
        last_id = 0xFFFFFFFFFFFFFFFFull;
        DBTeamCopyPlayerAward dbtcpa;
        if(execu->Prepare("SELECT `playerId`, `rollId`, `roll`, `awardId`, `awardCnt` FROM `teamcopy_player_award` ORDER BY `playerId`", dbtcpa) != DB::DB_OK)
            return false;
        lc.reset(500);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl = globalPlayers[dbtcpa.playerId];
            if(!pl)
                continue;
            TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
            if(!tcpInfo)
                continue;

            tcpInfo->loadAwardInfoFromDB(dbtcpa.rollId, dbtcpa.roll, dbtcpa.awardId, dbtcpa.awardCnt);
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

            TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
            if(tcpInfo && dtdata.m_Completed)
                tcpInfo->checkCopyPass(dtdata.m_TaskId);
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

            TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
            if(tcpInfo)
                tcpInfo->checkCopyPass(dtdata2.m_TaskId);
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
		if(execu->Prepare("SELECT `id`, `atker`, `defer`, `repid`, `time`, `winSide` FROM `athletics_record` ORDER BY `time` desc", drd) != DB::DB_OK)
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
			ret1 = atker->GetAthletics()->addAthleticsDataFromDB(drd.id, 0, defer, drd.winSide, drd.repid, drd.time);
			ret2 = defer->GetAthletics()->addAthleticsDataFromDB(drd.id, 1, atker, drd.winSide, drd.repid, drd.time);
			if (!ret1 && !ret2)
			{
				DB1().PushUpdateData("DELETE FROM `athletics_record` WHERE `id` = %u", drd.id);
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

        // Load player attainment
		lc.prepare("Loading player attainment:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBAttainData dadata;
		if(execu->Prepare("SELECT `ownerId`, `attainId`, `status`, `updatetime` FROM `attainment` ORDER BY `ownerId`, `attainId`", dadata) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(dadata.ownerId != last_id)
			{
				last_id = dadata.ownerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			AttainData* attain = new AttainData();
			attain->ownerId = dadata.ownerId;
			attain->attainId = dadata.attainId;
			attain->status = dadata.status;
			attain->updatetime = dadata.updatetime;
			pl->GetAttainMgr()->LoadAttain(attain);
		}
		lc.finalize();

        // Load player activity
        lc.prepare("Loading player activityData:");
    	last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBActivityData atydata;
		//if(execu->Prepare("SELECT `playerId`, `overTime`, `awardId`, `point`, `award`, `flags`, `scores`, `propsID`, `signRecord` FROM `activityData` ORDER BY  `playerId`", atydata) != DB::DB_OK)
		if(execu->Prepare("SELECT `playerId`, `overTime`, `scores`, `propsID`, `signRecord` FROM `activityData` ORDER BY  `playerId`", atydata) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(atydata.playerId != last_id)
			{
				last_id = atydata.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			pl->GetActivityMgr()->LoadFromDB(atydata);
		}
		lc.finalize();

        // Load player Strengthen souls
        lc.prepare("Loading player StrengthenData:");
    	last_id = 0xFFFFFFFFFFFFFFFFull;
		pl = NULL;
		DBStrengthenData sthdata;
		if(execu->Prepare("SELECT `playerId`, `overTime`, `souls`, `soulId`, `flags`, `greenBox`, `blueBox`, `purpleBox`, `orangeBox` FROM `strengthenData` ORDER BY  `playerId`", sthdata) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(sthdata.playerId != last_id)
			{
				last_id = sthdata.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			pl->GetStrengthenMgr()->LoadFromDB(sthdata);
		}

        lc.finalize();
		lc.prepare("Loading mail package:");
		last_id = 0xFFFFFFFFFFFFFFFFull;
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

		/////////////////////////////////

		globalPlayers.enumerate(player_load, 0);
        //GVAR.SetVar(GVAR_CITTASMERGE, 1);
        //GVAR.SetVar(GVAR_1530BUCHANG, 1);
        GVAR.SetVar(GVAR_OLDRC7DAYBUCHANG, 1);
        GVAR.SetVar(GVAR_JOB_MO_PEXP, 1);

		return true;
	}

	bool GObjectManager::loadAllAthletics()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading athletics_rank:");
		DBAthleticsData dbd;
        UInt32 rank[2] = {0,0};
		if(execu->Prepare("SELECT `row`, `rank`, `ranker`, `maxRank`, `challengeNum`, `challengeTime`, `prestige`, `tael`, `winStreak`, `beWinStreak`, `failStreak`, `beFailStreak`, `oldRank`, `first4Rank`, `extrachallenge`, `pageNum`, `eChallengeTime`, `ePhysical`, `eSelectIndex`, `eCombine1`, `eCombine2`, `eCombine3`, `eCombine4`, `eCombine5`, `eRival1`, `eRival2`, `eRival3`, `eRival4`, `eRival5`, `eCanAttack1`, `eCanAttack2`, `eCanAttack3`, `eCanAttack4`, `eCanAttack5`, `eRivalType1`, `eRivalType2`, `eRivalType3`, `eRivalType4`, `eRivalType5` FROM `athletics_rank` ORDER BY `rank`, `maxRank`", dbd) != DB::DB_OK)
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

            ++ rank[dbd.row];
            //if(rank[dbd.row] > ATHLETICS_RANK_MAX_CNT)
            //    rank[dbd.row] = ATHLETICS_RANK_MAX_CNT + 1;
            data->rank = rank[dbd.row];
#if 0
            if(rank[dbd.row] != dbd.rank/* && rank[dbd.row] <= ATHLETICS_RANK_MAX_CNT*/)
            {
                DB6().PushUpdateData("UPDATE `athletics_rank` SET `rank` = %u WHERE `ranker` = %"I64_FMT"u", data->rank, dbd.ranker);
            }
#endif
			data->maxrank = dbd.maxRank;
			data->challengenum = dbd.challengeNum;
			data->challengetime = dbd.challengeTime;
			//data->boxid = dbd.boxId;
			//data->awardType = dbd.boxtype;
			//data->awardCount = dbd.boxcount;
			//data->boxcolor = dbd.boxcolor;
			//data->boxflushtime = dbd.boxFlushTime;
			//AthleticsRank::buildBoxAward(data->boxid, data->awardType, data->awardCount, data->awardName);
            data->prestige = dbd.prestige;
            data->tael = dbd.tael;
			data->winstreak = dbd.winstreak;
            data->bewinstreak = dbd.bewinstreak;
            data->failstreak = dbd.failstreak;
            data->befailstreak = dbd.befailstreak;
            data->oldrank = dbd.oldrank;
            data->first4rank = dbd.first4rank;
            data->extrachallenge = dbd.extrachallenge;
            data->pageNum = dbd.pageNum;
            AthleticsPInfo *PlayerPInfo = pl->GetAthletics()->getPlayerPInfo();
            PlayerPInfo->eChallengeTime = dbd.eChallengeTime;
            PlayerPInfo->ePhysical = dbd.ePhysical;
            if(PlayerPInfo->ePhysical > gAthleticsRank.GetMaxPhysical(pl->getVipLevel()))
                PlayerPInfo->ePhysical = gAthleticsRank.GetMaxPhysical(pl->getVipLevel());
            PlayerPInfo->eSelectIndex = dbd.eSelectIndex;
            for(UInt8 index = 0; index < 5; index++)
            {
                PlayerPInfo->eCombine[index] = dbd.eCombine[index];
                PlayerPInfo->eRival[index] = dbd.eRival[index];
                PlayerPInfo->eCanAttack[index] = dbd.eCanAttack[index];
                PlayerPInfo->eRivalType[index] = dbd.eRivalType[index];
            }
			gAthleticsRank.addAthleticsFromDB(dbd.row, data);
		}
        gAthleticsRank.checkRankL();
		lc.finalize();

		return true;
	}

	bool GObjectManager::loadAllAthleticsEvent()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading athletics_event:");
		DBAthleticsEventData dbed;
        if(execu->Prepare("SELECT `id`, `row`, `player1`, `player2`, `cond`, `color`, `value`, `itemcount`, `itemid`, `time` FROM `athletics_event` ORDER BY `time` desc", dbed) != DB::DB_OK)
            return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl1 = globalPlayers[dbed.player1];
			Player * pl2 = globalPlayers[dbed.player2];
			if (pl1 == NULL && pl2 == NULL)
				continue;
            gAthleticsRank.addAthleticsEventDataFromDB(dbed.row, dbed.id, pl1, pl2, dbed.cond, dbed.color, dbed.value, dbed.itemCount, dbed.itemId, dbed.time);
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

		lc.prepare("Loading auto copy challenge data:");
		DBAutoCopy dac;
		if(execu->Prepare("SELECT `playerId`, `id` FROM `autocopy`", dac) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl = globalPlayers[dac.playerId];
			if(pl == NULL)
				continue;
			playerCopy.autoBattle(pl, dac.id, 0, 0, true);
		}
		lc.finalize();

		lc.prepare("Loading auto frontmat challenge data:");
		DBAutoFrontMap afm;
		if(execu->Prepare("SELECT `playerId`, `id` FROM `auto_frontmap`", afm) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			Player * pl = globalPlayers[afm.playerId];
			if(pl == NULL)
				continue;
			frontMap.autoBattle(pl, afm.id, 0, 0, true);
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
				UInt32 trainTime = dbft.trainRemain / 3600;
				UInt32 endTime = dbft.trainTime + dbft.trainRemain;
				if(endTime > dbft.checkTime)
				{
					UInt32 checkTime = (endTime - dbft.checkTime) / 3600;
					if(trainTime > 24)
						trainTime = 24;
					if(checkTime > trainTime)
						checkTime = trainTime;
					pl->addTrainFighterFromDB(dbft.fighterId, dbft.priceType, dbft.price, trainTime, checkTime, dbft.accExp);
					DB1().PushUpdateData("REPLACE INTO `fighter_train`(`fgtId`, `ownerId`, `priceType`, `price`, `trainTime`, `checkTime`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u)", dbft.fighterId, dbft.ownerId, dbft.priceType, dbft.price, trainTime, checkTime);
				}
				else
					DB1().PushUpdateData("DELETE FROM `fighter_train` WHERE `fgtId` = %u AND `ownerId` = %"I64_FMT"u", dbft.fighterId, dbft.ownerId);
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
		if(execu->Prepare("SELECT `id`, `type`, `friendId` FROM `friend` ORDER BY `id`", dbfr) != DB::DB_OK)
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
            case 3:
                pl->addCFriendFromDB(toadd);
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

        // ͨ??????????
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

        // ͨ????????
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

        // ͨ??????????
		lc.prepare("Loading dungeon level templates:");
		GData::DBDungeonLevel dlvl;
		if(execu->Prepare("SELECT `id`, `level`, `monsterSet`, `lootSet` FROM `dungeon_level`", dlvl) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			GData::DungeonData * dd = const_cast<GData::DungeonData *>(GData::dungeons[dlvl.id]);
			GData::DungeonLevel * dl = new GData::DungeonLevel;
			dl->monsterSet = GData::dungeonMonsters[dlvl.monsterSet];
			StringTokenizer tk(dlvl.lootSet, ",");
			for(size_t i = 0; i < tk.count(); ++ i)
			{
				dl->loots.push_back(GData::lootTable[atoi(tk[i].c_str())]);
			}
            std::vector<const GData::DungeonLevel *>& ddl = dd->monsters;
            if(ddl.size() <= static_cast<size_t>(dlvl.level))
                ddl.resize(dlvl.level + 1);
            ddl[dlvl.level] = dl;
		}
		lc.finalize();

		GData::dungeons.enumerate(dungeon_enum, NULL);

        // ͨ?????û?????
		lc.prepare("Loading dungeon player data:");
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		Player * pl = NULL;
		execu.reset(DB::gObjectDBConnectionMgr->GetExecutor());
		DBDungeonPlayer dp;
		if(execu->Prepare("SELECT `id`, `playerId`, `level`, `count`, `totalCount`, `firstPass`, `counterEnd`, `justice`, `justice_roar` FROM `dungeon_player` ORDER BY `playerId`", dp) != DB::DB_OK)
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
			dg->pushPlayer(pl, dp.level, dp.count, dp.totalCount, dp.firstPass, dp.counterEnd, dp.justice, dp.justice_roar);
		}
		lc.finalize();
		return true;
	}

	bool GObjectManager::loadTeamCopy()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        // ???Ӹ???????
		LoadingCounter lc("Loading team copy templates:");
		GData::DBTeamCopy dbtc;
		if(execu->Prepare("SELECT `id`, `type`, `location`, `npcgroups` FROM `team_copy`", dbtc) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			StringTokenizer tk(dbtc.npcgroups, ",");
			for(size_t i = 0; i < tk.count(); ++ i)
			{
                teamCopyManager->addTeamCopyNpc(dbtc.id, dbtc.type, dbtc.location, atoi(tk[i].c_str()));
            }
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

        // ??????Ϣ
		LoadingCounter lc("Loading clans:");
		DBClan cl;
		if (execu->Prepare("SELECT `id`, `name`, `rank`, `level`, `funds`, `foundTime`, `founder`, `leader`, `watchman`, `construction`, `contact`, `announce`, `purpose`, `proffer`, `grabAchieve`, `battleTime`, `nextBattleTime`, `allyClan`, `enemyClan1`, `enemyClan2`, `battleThisDay`, `battleStatus`, `southEdurance`, `northEdurance`, `hallEdurance`, `hasBattle`, `battleScore`, `dailyBattleScore` `battleRanking` FROM `clan`", cl) != DB::DB_OK)
			return false;
		lc.reset(1000);
		Clan * clan = NULL;
		ClanBattle * clanBattle = NULL;
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (cl.id != 0)
			{
				clan = new Clan(cl.id, cl.name, cl.foundTime, cl.level );
				clanBattle = clan->getClanBattle();
				clan->setContact(cl.contact, false);
				clan->setAnnounce(cl.announce, false);
				clan->setPurpose(cl.purpose, false);
				clan->setProffer(cl.proffer, false);
				clan->addAllyClanFromDB(cl.allyClan);
				clan->addEnemyClanFromDB(cl.enemyClan1);
				clan->addEnemyClanFromDB(cl.enemyClan2);
				//clan->patchMergedName();
                clan->setClanFunds(cl.funds);
				clan->setFounder(cl.founder);
				clan->setLeaderId(cl.leader, false);
                clan->patchMergedName();
				clan->setWatchmanId(cl.watchman, false);
				clan->setConstruction(cl.construction, false);
                clan->LoadBattleScore(cl.battleScore);
                clan->LoadDailyBattleScore(cl.dailyBattleScore);
                clan->LoadLastBattleRanking(cl.battleRanking);
				clanBattle->setOwnerClanId(cl.id);
				if (!clanManager.validClanBattleTime(cl.battleTime))
				{
					cl.battleTime = 25;
					DB5().PushUpdateData("UPDATE `clan` SET `battleTime` = 25 WHERE `id` = %u", cl.id);
				}
				if (!clanManager.validClanBattleTime(cl.nextBattleTime))
				{
					cl.battleTime = 25;
					DB5().PushUpdateData("UPDATE `clan` SET `nextBattleTime` = 25 WHERE `id` = %u", cl.id);
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

        // ??????Ա
		lc.prepare("Loading clan players:");
		DBClanPlayer cp;
		if (execu->Prepare("SELECT `id`, `playerId`, `joinTime`, `proffer`, `cls`, `enterCount`, `thisDay`, `petFriendness1`, `petFriendness2`, `petFriendness3`, `petFriendness4`, `favorCount1`, `favorCount2`, `favorCount3`, `favorCount4`, `lastFavorTime1`, `lastFavorTime2`, `lastFavorTime3`, `lastFavorTime4`, `signupRankBattleTime`, `rankBattleField` FROM `clan_player` ORDER BY `id`, `proffer` DESC, `joinTime` ASC", cp) != DB::DB_OK)
			return false;
		UInt32 lastId = 0xFFFFFFFF;
		lc.reset(1000);
		UInt32 rank = 0;
		bool hasLeader = false;
        clan = NULL;
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
            {
                DB5().PushUpdateData("DELETE FROM `clan_player` WHERE `playerId` = %"I64_FMT"u", cp.playerId);
				continue;
            }
			Player * pl = globalPlayers[cp.playerId];
			if(pl == NULL)
				continue;
			ClanMember * cm = new(std::nothrow) ClanMember();
			if (cm == NULL) return false;
			cm->player = pl;
			cm->cls = cp.cls;
			if (pl->getId() == clan->getLeaderId())
			{
				hasLeader = true;
                if(cp.cls != 4)
                {
                    cm->cls = 4;
                    DB5().PushUpdateData("UPDATE `clan_player` SET `cls` = 4 WHERE `playerId` = %"I64_FMT"u", pl->getId());
                }
			}
			cm->joinTime = cp.joinTime;
            cm->proffer = cp.proffer;
			if (thisDay != cp.thisDay)
			{
				DB5().PushUpdateData("UPDATE `clan_player` SET `enterCount` = 0, `thisDay` = %u WHERE `playerId` = %"I64_FMT"u", thisDay, pl->getId());
				cm->enterCount = 0;
			}
			else
			{
				cm->enterCount = cp.enterCount;
			}
			for(UInt32 i = 0; i < 4; i ++)
			{
				if(cp.petFriendness[i] != 0)
				{
					cm->clanPet.insert(std::make_pair(i + 7, GObject::ClanPlayerPet(cp.favorCount[i], cp.lastFavorTime[i], cp.petFriendness[i])));
				}
			}

            cm->signupRankBattleTime = cp.signupRankBattleTime;
            cm->rankBattleField = cp.rankBattleField;

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

        lc.prepare("Loading clan item:");
        DBClanItem ci;
        if(execu->Prepare("SELECT `clanid`, `playerid`, `itemid`, `itemnum` FROM `clan_item` ORDER BY `clanid`", ci) != DB::DB_OK)
            return false;
        lastId = 0xFFFFFFFF;
        clan = NULL;
        lc.reset(1000);
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if(ci.clanid != lastId)
            {
                lastId = ci.clanid;
                clan = globalClans[ci.clanid];
            }
            if(clan == NULL) continue;

            clan->LoadItem(ci.playerid, ci.itemid, ci.itemnum);
        }
        lc.finalize();

        lc.prepare("Loading clan item history:");
        DBClanItemHistory cih;
        if(execu->Prepare("SELECT `id`, `clanid`, `type`, `time`, `playerid`, `itemstr` FROM `clan_item_history` ORDER BY `clanid`,`time`", cih) != DB::DB_OK)
            return false;
        lastId = 0xFFFFFFFF;
        clan = NULL;
        lc.reset(1000);
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if(cih.clanid != lastId)
            {
                lastId = cih.clanid;
                clan = globalClans[cih.clanid];
            }
            if(clan == NULL) continue;

            clan->LoadItemHistory(cih.type, cih.time, cih.playerid, cih.itemstr);
        }
        lc.finalize();

        //???ɿƼ?
		lc.prepare("Loading clan tech:");
		DBClanTech ct;
		if(execu->Prepare("SELECT `clanId`, `techId`, `level`, `extra` FROM `clan_tech` ORDER BY `clanId`", ct) != DB::DB_OK)
			return false;
		lastId = 0xFFFFFFFF;
		clan = NULL;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (ct.clanId != lastId)
				clan = globalClans[ct.clanId];
			if (clan == NULL)
				continue;
			clan->getClanTech()->addTechFromDB(ct.techId, ct.level, ct.extra);
		}
		lc.finalize();

        //????????????
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

        //????????
		lc.prepare("Loading clan donate record:");
		clan = NULL;
		lastId = 0xFFFFFFFF;
		DBClanDonateRecord ddr;
		if (execu->Prepare("SELECT `clanId`, `donateName`, `techId`, `donateCount`, `donateTime` FROM `clan_donate_record` ORDER BY `clanId`, `donateTime`", ddr) != DB::DB_OK)
			return false;
		lc.reset(200);
		while (execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if (ddr.clanId != lastId)
				clan = globalClans[ddr.clanId];
			if (clan == NULL)
			{
				DB5().PushUpdateData("DELETE FROM `clan_donate_record` WHERE `clanId` = %u", ddr.clanId);
				continue;
			}
			clan->addClanDonateRecordFromDB(ddr.doanteName, ddr.techId, ddr.donateCount, ddr.donateTime);
		}
		lc.finalize();

		globalClans.enumerate(configLoadedClanData, (void *)(&now));
		clanManager.resumeRobClanBattleData();
		//clanManager.updateAllocated();

        // ????ս??
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
					DB5().PushUpdateData("DELETE FROM `clan_battle_result` WHERE `id` = %u", dbr.id);
					continue;
				}
				clanBattle = clan->getClanBattle();
			}
			clanBattle->getClanDynamicMsg()->addCBResultFromDB(dbr.id, dbr.battleTime, dbr.result);
		}
		lc.finalize();


		//???ذ???ս??????????
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
				DB5().PushUpdateData("DELETE FROM `clan_battler` WHERE `id` = %u", cb.id);
				continue;
			}
			if (clan != NULL)
				clanBattle = clan->getClanBattle();
			Player * pl = globalPlayers[cb.battler];
			if (pl == NULL || pl->getClan() == NULL)
			{
				DB5().PushUpdateData("DELETE FROM `clan_battler` WHERE `id` = %u", cb.id);
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
				DB5().PushUpdateData("DELETE FROM `clan_battler` WHERE `id` = %u", cb.id);
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

        //???ά??
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

        // 读取帮派神像
        lc.prepare("Loading clan statue:");
        DBClanStatue cs;
        if (execu->Prepare("SELECT `clanid`, `level`, `exp`, `expUpdateTime` FROM `clan_statue` ORDER BY `clanid`", cs) != DB::DB_OK)
            return false;
        clan = NULL;
        lastId = 0xFFFFFFFF;
        lc.reset(1000);
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if (cs.clanId != lastId)
            {
                lastId = cs.clanId;
                clan = globalClans[cs.clanId];
            }
            if(clan == NULL) continue;
            clan->LoadStatue(cs.level, cs.exp, cs.expUpdateTime);
        }
        lc.finalize();

        // 读取帮派副本
        lc.prepare("Loading clan copy:");
        DBClanCopy cc;
        if (execu->Prepare("SELECT `clanid`, `level`, `levelUpdateTime`, `maxCopyLevel`, `maxCopyTime` FROM `clan_copy` ORDER BY `clanid`", cc) != DB::DB_OK)
            return false;
        clan = NULL;
        lastId = 0xFFFFFFFF;
        lc.reset(1000);
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if (cc.clanId != lastId)
            {
                lastId = cc.clanId;
                clan = globalClans[cc.clanId];
            }
            if(clan == NULL) continue;
            clan->LoadCopy(cc.level, cc.levelUpdateTime, cc.maxLevel, cc.maxLevelTime);
        }
        lc.finalize();
        
        // 读取帮派副本历史日志
        lc.prepare("Loading clan copy log:");
        DBClanCopyLog ccl;
        if (execu->Prepare("SELECT `clanid`, `logTime`, `logType`, `playerName`, `logVal` FROM `clan_copy_log` ORDER BY `clanid` ASC, `logTime` ASC", ccl) != DB::DB_OK)
            return false;
        clan = NULL;
        lc.reset(1000);
        lastId = 0xFFFFFFFF;
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if (ccl.clanId != lastId)
            {
                lastId = ccl.clanId;
                clan = globalClans[ccl.clanId];
            }
            if (clan == NULL) continue;
            clan->LoadCopyLog(ccl.logTime, ccl.logType, ccl.playerName, ccl.logVal);
        }
        lc.finalize();

		return true;
	}

    bool GObjectManager::loadEquipForge()
    {
		lua_State* L = lua_open();
		luaopen_base(L);
		luaopen_string(L);
		luaopen_table(L);
		{
			std::string path = cfg.scriptPath + "items/EquipForge.lua";
			lua_tinker::dofile(L, path.c_str());

            _enchant_cost = lua_tinker::call<UInt32>(L, "getEnchantCost");
            _merge_cost = lua_tinker::call<UInt32>(L, "getMergeCost");
            _detach_cost = lua_tinker::call<UInt32>(L, "getDetachCost");
            _split_cost = lua_tinker::call<UInt32>(L, "getSplitCost");
            _forge_cost = lua_tinker::call<UInt32>(L, "getForgeCost");

            UInt8 q = 0;
			for(; q < 4; q ++)
			{
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getSplitChance", q + 1);
				UInt32 size = std::min(2, table_temp.size());
				for(UInt32 j = 0; j < size; j ++)
				{
					_split_chance[q][j] =  table_temp.get<UInt32>(j + 1);
				}
			}

            {
                lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getTeamMatieralSplit");
				UInt32 size = table_temp.size();
				for(UInt32 j = 0; j < size; j ++)
				{
                    lua_tinker::table table_temp2 = table_temp.get<lua_tinker::table>(j + 1);
					UInt8 q = table_temp2.get<UInt8>(1) - 2;
                    if(q > 2)
                        continue;
                    _team_m_chance[q] = table_temp2.get<UInt32>(2);
                    _team_m_item[q] = table_temp2.get<UInt32>(3);
                }
            }

            {
                lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getOrangeTeamMatieralSplit");
				UInt32 size = table_temp.size();
				for(UInt32 j = 0; j < size; j ++)
				{
                    lua_tinker::table table_temp2 = table_temp.get<lua_tinker::table>(j + 1);
					UInt32 itemId = table_temp2.get<UInt32>(1);
                    _team_om_chance[0][itemId] = table_temp2.get<UInt32>(2);
                    _team_om_chance[1][itemId] = table_temp2.get<UInt32>(3);
                    _team_om_chance[2][itemId] = table_temp2.get<UInt32>(4);
                    _team_om_item[itemId] = table_temp2.get<UInt32>(5);
                }
            }

            {
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getMergeChance");
				UInt32 size = std::min(11, table_temp.size());
				for(UInt32 j = 0; j < size; j ++)
				{
					_merge_chance[j] =  table_temp.get<UInt32>(j + 1);
				}
            }

            {
                 lua_tinker::table table_tmp = lua_tinker::call<lua_tinker::table>(L, "getMergeStuff");
                 UInt32 size  = table_tmp.size();
                 for (UInt32 j = 0; j < size; j++)
                 {
                     stMergeStf  s;

                     lua_tinker::table t1 = table_tmp.get<lua_tinker::table>(j + 1);
                     UInt32  tSize = t1.size();
                       // for (UInt32 i = 0 ; i < t; i++)
                        for (UInt32 i = 0 ; i< tSize; i++ )
                        {
                            if(i == tSize - 1)
                            {
                               s.m_to = t1.get<UInt32>(i + 1);
                            }
                            else
                            {

                                lua_tinker::table c = t1.get<lua_tinker::table>(i + 1);
                                UInt32  cSize = c.size();
                                if(cSize == 2)
                                {
                                    stMergeS  ms;
                                    ms.id = c.get<UInt32>(1);
                                    std::vector<UInt32>& v = _mMergeStfsIndex[ms.id];
                                    v.push_back(j);

                                    ms.num = c.get<UInt32>(2);
                                    s.m_stfs.push_back(ms);
                                }
                                if(cSize == 3)
                                {
                                    UInt32  id1 = c.get<UInt32>(1);
                                    UInt32  id2 = c.get<UInt32>(2);
                                    UInt32  num = c.get<UInt32>(3);
                                    if(id1 < id2)
                                    {
                                        for(;id1<= id2; id1++)
                                        {
                                            stMergeS ms;
                                            std::vector<UInt32>& v = _mMergeStfsIndex[id1];
                                            v.push_back(j);
                                            ms.id = id1;
                                            ms.num = num;
                                            s.m_stfs.push_back(ms);
                                        }
                                    }
                                }
                            }

                        }
                      _vMergeStfs.push_back(s);
                 }
            }

            {
                /*lua_tinker::table t = lua_tinker::call<lua_tinker::table>(L, "getEUpgradeIdMap");
                UInt32  tSize = t.size();
                for (UInt32 i = 0; i< tSize; i++)
                {
                    lua_tinker::table t_idmap = t.get< lua_tinker::table >(i + 1);
                    if(t_idmap.size() == 2)
                    {
                        lua_tinker::table  t_from = t_idmap.get< lua_tinker::table >(1);

                        lua_tinker::table  t_to = t_idmap.get< lua_tinker::table >(2);

                        UInt32 sFrom = t_from.size();
                        UInt32 sTo = t_to.size();
                        if(sFrom  == sTo)
                        {
                            if(sFrom == 1)
                                _EUpgradeIdMap[t_from.get<UInt32>(1)] = t_to.get<UInt32>(1);
                            else if(sFrom == 2)
                            {
                                UInt32 fromId1 = t_from.get<UInt32>(1) ;
                                UInt32 fromId2 = t_from.get<UInt32>(2);
                                UInt32 toId1 = t_to.get<UInt32>(1);
                                UInt32 toId2 = t_to.get<UInt32>(2);
                                if(fromId1 < fromId2 && toId1 < toId2 &&  fromId2 - fromId1 == toId2 - toId1 )
                                {
                                   do
                                   {
                                       _EUpgradeIdMap[fromId1 ++ ] = toId1 ++ ;
                                   }
                                    while(fromId1 != fromId2);
                                }
                            }
                        }

                    }
                }*/
            }
			for(UInt8 t = 0; t < 2; ++t)
            {
                for(q = 0; q < 6; q ++)
                {
                    lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getEnchantChance", t + 1, q + 1);
                    UInt32 size = std::min(12, table_temp.size());
                    for(UInt32 j = 0; j < size; j ++)
                    {
                        _enchant_chance[t][q][j] =  table_temp.get<UInt32>(j + 1);
                    }

                }
            }

            {
                for(UInt8 q = 0; q < 6; q ++)
                {
                    lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getEnchantChanceAdv", q + 1);
                    UInt32 size = table_temp.size();
                    for(UInt32 j = 0; j < size; j ++)
                    {
                        stHftChance hftc;
                        lua_tinker::table table_temp2 = table_temp.get<lua_tinker::table>(j+1);
                        UInt8 lv = table_temp2.get<UInt8>(1) - 1;
                        if(lv > 12)
                            continue;
                        hftc.times = table_temp2.get<UInt16>(2);
                        hftc.chance = table_temp2.get<UInt32>(3);
                        _hft_chance[q][lv].push_back(hftc);
                    }
                }
            }

			for(q = 0; q < 6; q ++)
            {
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getTrumpLOrderChance", q + 1);
				UInt32 size = std::min(12, table_temp.size());
				for(UInt32 j = 0; j < size; j ++)
				{
					_trump_lorder_chance[q][j] =  table_temp.get<UInt32>(j + 1);
				}
            }

			for(q = 0; q < 6; q ++)
            {
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getTrumpExpRank", q + 1);
				UInt32 size = std::min(12, table_temp.size());
				for(UInt32 j = 0; j < size; j ++)
				{
					_trump_exp_rank[q][j] =  table_temp.get<UInt32>(j + 1);
				}
            }

            for(UInt32 j = 0; j < 9; j ++)
            {
                for(q = 0; q < 6; q ++)
				{
                    lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getTrumpRankFactor", j + 1, q + 1);
                    UInt32 size = table_temp.size();
                    if(size > 21)
                    {
                        _trump_rank_factor[q][j].strength =  table_temp.get<float>(1);
                        _trump_rank_factor[q][j].physique =  table_temp.get<float>(2);
                        _trump_rank_factor[q][j].agility =  table_temp.get<float>(3);
                        _trump_rank_factor[q][j].intelligence =  table_temp.get<float>(4);
                        _trump_rank_factor[q][j].will =  table_temp.get<float>(5);
                        _trump_rank_factor[q][j].soul =  table_temp.get<float>(6);
                        _trump_rank_factor[q][j].aura =  table_temp.get<float>(7);
                        _trump_rank_factor[q][j].auraMax =  table_temp.get<float>(8);
                        _trump_rank_factor[q][j].attack =  table_temp.get<float>(9);
                        _trump_rank_factor[q][j].magatk =  table_temp.get<float>(10);
                        _trump_rank_factor[q][j].defend =  table_temp.get<float>(11);
                        _trump_rank_factor[q][j].magdef =  table_temp.get<float>(12);
                        _trump_rank_factor[q][j].hp =  table_temp.get<float>(13);
                        _trump_rank_factor[q][j].tough =  table_temp.get<float>(14);
                        _trump_rank_factor[q][j].action =  table_temp.get<float>(15);
                        _trump_rank_factor[q][j].hitrate =  table_temp.get<float>(16);
                        _trump_rank_factor[q][j].evade =  table_temp.get<float>(17);
                        _trump_rank_factor[q][j].critical =  table_temp.get<float>(18);
                        _trump_rank_factor[q][j].criticaldmg =  table_temp.get<float>(19);
                        _trump_rank_factor[q][j].pierce =  table_temp.get<float>(20);
                        _trump_rank_factor[q][j].counter =  table_temp.get<float>(21);
                        _trump_rank_factor[q][j].magres =  table_temp.get<float>(22);
                    }
				}
            }

            {
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getTrumpMaxRankChance");
				UInt32 size = std::min(5, table_temp.size());
				for(UInt32 j = 0; j < size; j ++)
				{
                    _trump_maxrank_chance.push_back(table_temp.get<UInt16>(j + 1));
				}
            }


            for(q = 0; q < 4; ++ q)
            {
                lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getTrumpSmelt", q + 1);
                UInt32 size = std::min(8, table_temp.size());
                for(UInt32 j = 0; j < size; j ++)
                {
                    _trump_smelt[q].push_back(table_temp.get<float>(j + 1));
                }
            }

            {
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getEnchantMax");
				UInt32 size = std::min(11, table_temp.size());
				for(UInt32 j = 0; j < size; j ++)
				{
					_enchant_max[j] =  table_temp.get<UInt32>(j + 1);
				}
            }

            for(UInt8 q = 0; q < 3; q ++)
            {
                {
                    lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getAttrTypeChance", q + 1);
                    UInt32 size = std::min(9, table_temp.size());
                    for(UInt32 j = 0; j < size; j ++)
                    {
                        _attrTypeChances[q][j] =  table_temp.get<UInt16>(j + 1);
                    }
                }
                {
                    lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getAttrChance", q + 1);
                    UInt32 size = std::min(9, table_temp.size());
                    for(UInt32 j = 0; j < size; j ++)
                    {
                        _attrChances[q][j] =  table_temp.get<UInt16>(j + 1);
                    }
                }
                {
                    lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getAttrDics", q + 1);
                    UInt32 size = std::min(9, table_temp.size());
                    for(UInt32 j = 0; j < size; j ++)
                    {
                        _attrDics[q][j] =  table_temp.get<UInt16>(j + 1);
                    }
                }
            }

            {
                lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getAttrMax");
                UInt32 size = table_temp.size();
                for(UInt8 i = 0; i < size; ++i)
                {
                    lua_tinker::table table_temp2 = table_temp.get<lua_tinker::table>(i+1);
                    UInt8 q = table_temp2.get<float>(1) - 3;
                    UInt8 lvl = table_temp2.get<float>(2);
                    UInt8 crr = table_temp2.get<float>(3);
                    UInt32 size2 = table_temp2.size();

                    if(q > 2 || crr >= e_cls_max)
                        continue;

                    for(UInt8 t = 3; t < size2; ++t)
                    {
                        std::map<UInt8, stAttrMax*>::iterator it = _attrMax.find(lvl);
                        stAttrMax* attr = NULL;
                        if(it != _attrMax.end())
                            attr = it->second;
                        else
                        {
                            attr = new stAttrMax();
                            _attrMax[lvl] = attr;
                        }

                        attr->attrMax[q][crr][t-3] = table_temp2.get<float>(t+1);
                    }
                }
            }

            {
                lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getAttrTrumpMax");
                UInt32 size = table_temp.size();
                for(UInt8 i = 0; i < size; ++i)
                {
                    lua_tinker::table table_temp2 = table_temp.get<lua_tinker::table>(i+1);
                    UInt8 q = table_temp2.get<float>(1) - 3;
                    UInt8 lvl = table_temp2.get<float>(2);
                    UInt8 crr = table_temp2.get<float>(3);
                    UInt32 size2 = table_temp2.size();

                    if(q > 2 || crr >= e_cls_max)
                        continue;

                    for(UInt8 t = 3; t < size2; ++t)
                    {
                        std::map<UInt8, stAttrMax*>::iterator it = _attrTrumpMax.find(lvl);
                        stAttrMax* attr = NULL;
                        if(it != _attrTrumpMax.end())
                            attr = it->second;
                        else
                        {
                            attr = new stAttrMax();
                            _attrTrumpMax[lvl] = attr;
                        }

                        attr->attrMax[q][crr][t-3] = table_temp2.get<float>(t+1);
                    }
                }
            }

            {
                lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getEnchantRing");
                UInt32 size = table_temp.size();
                for(UInt8 i = 0; i < size; ++i)
                {
                    lua_tinker::table table_temp2 = table_temp.get<lua_tinker::table>(i+1);
                    UInt8 lvl = table_temp2.get<float>(1);

                    std::map<UInt8, stRingHpBase*>::iterator it = _ringHpBase.find(lvl);
                    stRingHpBase* ringHp = NULL;
                    if(it != _ringHpBase.end())
                        ringHp = it->second;
                    else
                    {
                        ringHp = new stRingHpBase();
                        _ringHpBase[lvl] = ringHp;
                    }

                    ringHp->hpBase[0] = table_temp2.get<float>(2);
                    ringHp->hpBase[1] = table_temp2.get<float>(3);
                    ringHp->hpBase[2] = table_temp2.get<float>(4);
                    ringHp->hpBase[3] = table_temp2.get<float>(5);
                    ringHp->hpBase[4] = table_temp2.get<float>(6);
                }
            }

            {
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getEnchantRingRate");
				UInt32 size = std::min(12, table_temp.size());
				for(UInt32 j = 0; j < size; j ++)
				{
					_ringHpFactor[j] =  table_temp.get<float>(j + 1);
				}
            }

            {
				lua_tinker::table table_temp = lua_tinker::call<lua_tinker::table>(L, "getSocketChance");
				UInt32 size = std::min(6, table_temp.size());
				for(UInt32 j = 0; j < size; j ++)
				{
					_socket_chance[j] =  table_temp.get<UInt32>(j + 1);
				}
            }
        }
        lua_close(L);
        return true;
    }

    bool GObjectManager::loadFightersPCChance()
    {
        lua_State* L = lua_open();
        luaopen_base(L);
        luaopen_string(L);
        luaopen_table(L);
        {
            std::string path = cfg.scriptPath + "Other/FighterTrain.lua";
            lua_tinker::dofile(L, path.c_str());

            _min_potential = lua_tinker::call<UInt32>(L, "getMinPotential");
            _max_potential = lua_tinker::call<UInt32>(L, "getMaxPotential");
            _min_capacity = lua_tinker::call<UInt32>(L, "getMinCapacity");
            _max_capacity = lua_tinker::call<UInt32>(L, "getMaxCapacity");

            lua_tinker::table potentialTable = lua_tinker::call<lua_tinker::table>(L, "getPotentialChance");
            size_t potential_size = potentialTable.size();
            for(UInt32 i = 0; i < potential_size; i ++)
            {
                lua_tinker::table tempTable = potentialTable.get<lua_tinker::table>(i + 1);
                size_t tempSize = tempTable.size();
                UInt32 chance = 0;
                UInt32 a = 0;
                UInt32 b = 0;
                if(tempSize >= 2)
                {
                    a = tempTable.get<UInt32>(1);
                    b = tempTable.get<UInt32>(2);
                }
                else if(tempSize == 1)
                    a = tempTable.get<UInt32>(1);

                chance = MAKECHANCE(a, b);
                _potential_chance.push_back(chance);
            }

            lua_tinker::table capacityTable = lua_tinker::call<lua_tinker::table>(L, "getCapacityChance");
            size_t capacity_size = capacityTable.size();
            for(UInt32 j = 0; j < capacity_size; j ++)
            {
                lua_tinker::table tempTable = capacityTable.get<lua_tinker::table>(j + 1);
                size_t tempSize = tempTable.size();
                UInt32 chance = 0;
                UInt32 a = 0;
                UInt32 b = 0;
                if(tempSize >= 2)
                {
                    a = tempTable.get<UInt32>(1);
                    b = tempTable.get<UInt32>(2);
                }
                else if(tempSize == 1)
                    a = tempTable.get<UInt32>(1);

                chance = MAKECHANCE(a, b);
                _capacity_chance.push_back(chance);
            }

            {
                // ?ƹ?????ˢ???佫????
                lua_tinker::table ct = lua_tinker::call<lua_tinker::table>(L, "getColorFighterChance_Free");
                size_t ct_size =  ct.size();
                _color_chance_free.resize(ct_size);
                for(UInt8 i = 0; i < ct_size; ++ i)
                {
                    lua_tinker::table tempTable = ct.get<lua_tinker::table>(i + 1);
                    size_t tempSize = 4 < tempTable.size() ? 4 : tempTable.size();
                    _color_chance_free[i].resize(tempSize);
                    for(UInt8 j = 0; j < tempSize; ++ j)
                    {
                        _color_chance_free[i][j] = tempTable.get<UInt32>(j+1);
                    }
                }
            }

            {
                // ?ƹݽ???ˢ???佫????
                lua_tinker::table ct = lua_tinker::call<lua_tinker::table>(L, "getColorFighterChance_Gold");
                size_t ct_size =  ct.size();
                _color_chance_gold.resize(ct_size);
                for(UInt8 i = 0; i < ct_size; ++ i)
                {
                    lua_tinker::table tempTable = ct.get<lua_tinker::table>(i + 1);
                    size_t tempSize = 4 < tempTable.size() ? 4 : tempTable.size();
                    _color_chance_gold[i].resize(tempSize);
                    for(UInt8 j = 0; j < tempSize; ++ j)
                    {
                        _color_chance_gold[i][j] = tempTable.get<UInt32>(j+1);
                    }
                }
            }

        }
        lua_close(L);

        return true;
    }

        bool GObjectManager::loadEquipments()
        {
            std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
            if (execu.get() == NULL || !execu->isConnected()) return false;

            LoadingCounter lc("Loading equipments:");
            DBEquipment dbe;
            if(execu->Prepare("SELECT `equipment`.`id`, `itemId`, `enchant`, `tRank`, `maxTRank`, `trumpExp`, `attrType1`, `attrValue1`, `attrType2`, `attrValue2`, `attrType3`, `attrValue3`, `sockets`, `socket1`, `socket2`, `socket3`, `socket4`, `socket5`, `socket6`, `bindType`  FROM `equipment` LEFT JOIN `item` ON `equipment`.`id` = `item`.`id` OR `item`.`id` = NULL", dbe) != DB::DB_OK)
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
                case Item_Halo:
                case Item_Fashion:
                case Item_Trump:
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
                    ied.tRank = dbe.tRank;
                    ied.maxTRank = dbe.maxTRank == 0 ? 2 : dbe.maxTRank;
                    ied.trumpExp = dbe.trumpExp;
                    ItemEquip * equip;
                    switch(itype->subClass)
                    {
                    case Item_Weapon:
                        equip = new ItemWeapon(dbe.id, itype, ied);
                        break;
                    case Item_Armor1:
                    case Item_Armor2:
                    case Item_Armor3:
					case Item_Armor4:
					case Item_Armor5:
						equip = new ItemArmor(dbe.id, itype, ied);
                        break;
                    case Item_Halo:
                    case Item_Fashion:
                    case Item_Trump:
                        if (itype->subClass == Item_Halo)
                        {
                            equip = new ItemHalo(dbe.id, itype, ied);
                            if (equip && ied.enchant)
                                ((ItemTrump*)equip)->fixSkills();
                        }
                        else if (itype->subClass == Item_Fashion)
                        {
                            equip = new ItemFashion(dbe.id, itype, ied);
                        }
                        else
                        {
                            equip = new ItemTrump(dbe.id, itype, ied);
                            if (equip && ied.enchant)
                                ((ItemTrump*)equip)->fixSkills();
                        }
                        break;
                    default:
                        equip = new ItemEquip(dbe.id, itype, ied);
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

	bool GObjectManager::loadEquipmentsSpirit()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;

        LoadingCounter lc("Loading equipments spirit:");
        DBEquipmentSpirit dbes;
        if(execu->Prepare("SELECT `equipment`.`id`, `splev1`, `splev2`, `splev3`, `splev4`, `spform1`, `spform2`, `spform3` FROM `equipment` LEFT JOIN `equipment_spirit` ON `equipment`.`id` = `equipment_spirit`.`id`", dbes) != DB::DB_OK)
            return false;

        lc.reset(2000);
        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            std::map<UInt32, ItemEquip *>::iterator it = equips.find(dbes.id);
            if(it == equips.end())
                continue;

            ItemEquip * equip = it->second;
            if(equip == NULL)
                continue;
            switch(equip->getClass())
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
                    ItemEquipSpiritAttr& esa = equip->getEquipSpiritAttr();
                    esa.spLev[0] = dbes.splev1;
                    esa.spLev[1] = dbes.splev2;
                    esa.spLev[2] = dbes.splev3;
                    esa.spLev[3] = dbes.splev4;
                    esa.spForm[0] = dbes.spform1;
                    esa.spForm[1] = dbes.spform2;
                    esa.spForm[2] = dbes.spform3;

                    if(equip->getQuality() == 5 && esa.spForm[0] == 0)
                    {
                        Package::GenSpirit(equip);
                    }
				}
				break;
			default:
				break;
			}
		}
		lc.finalize();

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

	bool GObjectManager::LoadArena()
	{
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Arena Bets");
		DBArenaBet ab;
		if(execu->Prepare("SELECT `id`, `round`, `state`, `group`, `recieved`, `pos`, `tael` FROM `arena_bet` ORDER BY `id`", ab)!= DB::DB_OK)
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
			arena.pushBetFromDB(pl, ab.round, ab.state, ab.group, ab.recieved, ab.pos, ab.tael);
		}
		lc.finalize();

		return true;
	}

    bool GObjectManager::LoadPracticePlace()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Practice Place");
		DBPracticePlace pp;
		if(execu->Prepare("SELECT `id`, `ownerid`, `protid`, `maxslot`, `openslot`, `protmoney`, `slotmoney`, `open`, `enemyCount`, `winCount`, `slotincoming`, `protincoming` FROM `practice_place` ORDER BY `id`", pp)!= DB::DB_OK)
			return false;
		lc.reset(1000);
        UInt8 i = 0;
		while(execu->Next() == DB::DB_OK)
		{
            if (!pp.id)
                continue;
            GObject::PPlace place;
            place.ownerid = pp.ownerid;
            place.protid = pp.protid;
            place.maxslot = pp.maxslot;
            place.openslot = pp.openslot;
            place.protmoney = pp.protmoney;
            place.slotmoney = pp.slotmoney;
            place.open = pp.open;
            place.enemyCount = pp.enemyCount;
            place.winCount = pp.winCount;
            place.slotincoming = pp.slotincoming;
            place.protincoming = pp.protincoming;
            practicePlace.addPlace(place, i++);
        }
		lc.finalize();
        return true;
    }

    void getMoneyLog(int today, int type, DBMoneyLog& r)
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return ;

        memset(&r, 0x00, sizeof(r));
        char buf[1024] = {0};
        snprintf(buf, sizeof(buf), "SELECT `time`, `type`, `gold`, `coupon`, `tael`, `achievement`, `prestige` FROM `money` WHERE `time` = %d AND `type` = %d", today, type);
		execu->Extract(buf, r);
    }

    void assignMoneyLog(DBMoneyLog& r, MoneyIn& mi)
    {
        mi.gold = r.gold;
    }

    bool GObjectManager::InitMoneyLog()
    {
        UInt32 now = TimeUtil::Now();

        int today[7] = {
            TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-6, now)),
            TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-5, now)),
            TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-4, now)),
            TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-3, now)),
            TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-2, now)),
            TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-1, now)),
            TimeUtil::GetYYMMDD(now),
        };

        DB8().PushUpdateData("INSERT INTO `money` (`time`, `type`, `gold`, `coupon`, `tael`, `achievement`, `prestige`) VALUES (%d,%d,0,0,0,0,0)", today[6], 1);
        DB8().PushUpdateData("INSERT INTO `money` (`time`, `type`, `gold`, `coupon`, `tael`, `achievement`, `prestige`) VALUES (%d,%d,0,0,0,0,0)", today[6], 2);

		DBMoneyLog t;
        for (int i = 0; i < 7; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                getMoneyLog(today[i], j+1, t);
                assignMoneyLog(t, World::_moneyIn[i][j]);
            }
        }
        World::_moneyLogged = today[6];

        return true;
    }

    bool GObjectManager::LoadHeroMemo()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Hero Memo Data");
        Player* pl = 0;
		DBHeroMemo hm;
		if(execu->Prepare("SELECT `playerId`, `awards`, `memos` FROM `heromemo`", hm)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			pl = globalPlayers[hm.playerId];
            if (!pl)
                continue;
            pl->GetHeroMemo()->loadFromDB(hm.awards.c_str(), hm.memos.c_str());
            pl = 0;
        }
		lc.finalize();

		globalPlayers.enumerate(heromemo_loaded, 0);
        return true;
    }

    bool GObjectManager::LoadShuoShuo()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Shuo Shuo Data");
        Player* pl = 0;
		DBShuoShuo ss;
		if(execu->Prepare("SELECT `playerId`, `updateTime`, `shuoshuo` FROM `shuoshuo`", ss)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			pl = globalPlayers[ss.playerId];
            if (!pl)
                continue;
            pl->GetShuoShuo()->loadFromDB(ss.update, ss.ss.c_str());
            pl = 0;
        }
		lc.finalize();

		globalPlayers.enumerate(shuoshuo_loaded, 0);
        return true;
    }

    bool GObjectManager::LoadCFriendAwards()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading CFriend Awards Data");
        Player* pl = 0;
		DBCFriendAwards cfa;
		if(execu->Prepare("SELECT `playerId`, `invitedId`, `awards` FROM `cfriend_awards`", cfa)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			pl = globalPlayers[cfa.playerId];
            if (!pl)
                continue;
            if (cfa.invitedId)
                pl->setInvitedBy(cfa.invitedId, false);
            pl->GetCFriend()->loadFromDB(cfa.awards.c_str());
            pl->setCFriends();
            pl = 0;
        }
		lc.finalize();
        return true;
    }

    bool GObjectManager::addGM(UInt64 id, UInt8 lvl)
    {
        if (!id)
            return false;
        Player* player = globalPlayers[id];
        if (player)
            player->setGMLevel(lvl);
        return true;
    }

    bool GObjectManager::delGM(UInt64 id)
    {
        return addGM(id, 0);
    }

    void GObjectManager::setFFTypeChance(UInt32 v)
    {
        _FFTypeChance.push_back(v);
    }

    void GObjectManager::setFFAttrChance(UInt32 v)
    {
        _FFAttrChance.push_back(v);
    }

    void GObjectManager::setFFAttrMaxValProp(UInt32 v)
    {
        _FFAttrMaxValProb.push_back(v);
    }

    void GObjectManager::setFFAttrMaxVal(UInt32 v)
    {
        _FFAttrMaxVal.push_back(v);
    }

    UInt8 GObjectManager::getFFType()
    {
        UInt32 size = _FFTypeChance.size();
        UInt32 v = GRND(10000);
        if (!size)
            return 0;
        for (UInt32 i = 0; i < size; ++i)
        {
            if (v <= _FFTypeChance[i])
                return i+1;
        }
        return 0;
    }

    UInt16 GObjectManager::getFFValue(UInt8 type, bool initmain)
    {
        if (!type)
            return 0;

        if (type > _FFTypeChance.size())
            return 0;
        if (type > _FFAttrMaxVal.size())
            return 0;

        if (initmain)
            return GRND(20, 40) * _FFAttrMaxVal[type-1];

        UInt32 size = _FFAttrChance.size();
        if (!size)
            return 0;

        UInt32 v = GRND(10000);
        UInt8 prob = 0;
        for (UInt32 i = 0; i < size; ++i)
        {
            if (v <= _FFAttrChance[i])
            {
                prob = i;
                break;
            }
        }

        if (prob >= _FFAttrMaxValProb.size())
            return 0;
        if ((UInt8)(prob+1) >= _FFAttrMaxValProb.size())
            return 0;

        UInt32 v1 = GRND(_FFAttrMaxValProb[prob], _FFAttrMaxValProb[prob+1]);
        return v1 * _FFAttrMaxVal[type-1];
    }

    UInt8 GObjectManager::getFFMaxVal(UInt8 type)
    {
        if (type > _FFAttrMaxVal.size())
            return 100;
        return _FFAttrMaxVal[type-1];
    }

    bool GObjectManager::LoadTripodData() // XXX: ??Ҫ?ӳټ???,??World::Init??????
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Tripod Data");
		DBTripod t;
		if(execu->Prepare("SELECT `id`, `soul`, `fire`, `quality`, `awdst`, `itemId`, `num` FROM `tripod`", t)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            TripodData td;
            td.soul = t.soul;
            td.fire = t.fire;
            td.quality = t.quality;
            td.awdst = t.awdst;
            td.itemId = t.itemId;
            td.num = t.num;

            Player* player = globalPlayers[t.id];
            if (player)
                player->runTripodData(td, true);
        }
		lc.finalize();
        return true;
    }

    bool GObjectManager::LoadWorldBoss() // XXX: ??Ҫ?ӳټ???,??World::Init??????
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading WorldBoss Data");
		DBWorldBoss t;
		if(execu->Prepare("SELECT `npcId`, `level`, `location`, `count` FROM `worldboss`", t)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
            //worldBoss.add(t.loc, t.npcId, t.level, t.count);
        }
		lc.finalize();
        return true;
    }

    bool GObjectManager::LoadPracticeData() // XXX: ??Ҫ?ӳټ???,??World::Init??????
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Practice Data");
        Player* pl = 0;
		DBPracticeData pd;
		if(execu->Prepare("SELECT `id`, `place`, `slot`, `type`, `pricetype`, `slotprice`, `protprice`, `traintime`, `checktime`, `prot`, `cdend`, `winnerid`, `fighters` FROM `practice_data`", pd)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            GObject::PracticeData* ppd = new (std::nothrow) GObject::PracticeData(pd.id);
            if (!ppd)
                return false;
            ppd->type = pd.type;
            ppd->pricetype = pd.pricetype;
            ppd->slotprice = pd.slotprice;
            ppd->protprice = pd.protprice;
            ppd->traintime = pd.traintime;
            ppd->checktime = pd.checktime;
            ppd->trainend = TimeUtil::Now() + 60 * pd.checktime;
            ppd->prot = pd.prot;
            ppd->cdend = pd.cdend;
            ppd->winnerid = pd.winnerid;
            StringTokenizer tk(pd.fighters, ",");
            for (size_t i = 0; i < tk.count(); ++i)
            {
                ppd->fighters.push_back(::atoi(tk[i].c_str()));
            }

            pl = globalPlayers[pd.id];
            if (!pl)
                continue;

            if (ppd->checktime)
            {
                pl->setPracticingPlaceSlot(pd.place << 16 | pd.slot);
                practicePlace.addPractice(pl, ppd, pd.place, pd.slot);
            }
            else
            {
                pl->setPracticingPlaceSlot(0 << 16);
                practicePlace.addPractice(pl, ppd, PPLACE_MAX, pd.slot);
            }
        }
		lc.finalize();
        return true;
    }

    UInt8 GObjectManager::reRecharge(const std::string& no, UInt16 id, UInt32 num, std::string& err)
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("reRecharge");
		DBReRecharge rrdb;
        char buf[1024] = {0};
        snprintf(buf, sizeof(buf), "SELECT `playerId`, `id`, `num`, `status` FROM `recharge` WHERE `no` = '%s'", no.c_str());
		if(execu->Prepare(buf, rrdb)!= DB::DB_OK)
        {
            err += "db error.";
			return 1;
        }
		lc.reset(1000);

        bool found = false;
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            if (rrdb.id == id && rrdb.num == num)
            {
                found = true;
                break;
            }
        }
		lc.finalize();

        if (!found)
        {
            err += "not found.";
            return 1;
        }

        return 0;
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

    void GObjectManager::pushEquipment(ItemEquip* e)
    {
        if (!e)
            return;
        equips[e->getId()] = e;
    }

	ItemFashion * GObjectManager::fetchFashion( UInt32 id )
	{
		ItemEquip * equip = fetchEquipment(id);
		if(equip == NULL)
			return NULL;
		if(equip->GetItemType().subClass != static_cast<UInt8>(Item_Fashion))
		{
			delete equip;
			return NULL;
		}
		return static_cast<ItemFashion*>(equip);
	}

	ItemHalo * GObjectManager::fetchHalo( UInt32 id )
	{
		ItemEquip * equip = fetchEquipment(id);
		if(equip == NULL)
			return NULL;
		if(equip->GetItemType().subClass != static_cast<UInt8>(Item_Halo))
		{
			delete equip;
			return NULL;
		}
		return static_cast<ItemHalo*>(equip);
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

    bool GObjectManager::LoadTownDeamon() //
    {
        if(!townDeamonManager)
            return false;

		std::unique_ptr<DB::DBExecutor> execu(DB::gDataDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

		LoadingCounter lc("Loading TownDeamon Monsters:");
		GData::DBTownDeamonMonster dbtdm;
		if(execu->Prepare("SELECT `level`, `npcId`, `itemId`, `itemNum` FROM `towndeamon_monster` order by level asc", dbtdm) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            townDeamonManager->loadDeamonMonstersFromDB(dbtdm.level, dbtdm.npcId, dbtdm.itemId, dbtdm.itemNum);
        }
		lc.finalize();

		std::unique_ptr<DB::DBExecutor> execu2(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu2.get() == NULL || !execu2->isConnected()) return false;
		lc.prepare("Loading TownDeamon Player:");
		DBTownDeamonPlayer dbtdp;
        Player* pl = NULL;
		if(execu2->Prepare("SELECT `deamonLevel`, `curLevel`, `maxLevel`, `playerId`, `startTime`, `accTime`, `accLen`, `accAwards`, `vitalityTime`, `vitality`, `spirit`, `challengeTime`, `itemId`, `itemNum`, `quitLevel`, `attacker` FROM `towndeamon_player` order by deamonLevel asc", dbtdp) != DB::DB_OK)
			return false;
		lc.reset(20);
		while(execu2->Next() == DB::DB_OK)
		{
			lc.advance();
            pl = globalPlayers[dbtdp.playerId];
            if(pl == NULL)
                continue;
            DeamonPlayerData* dpData = pl->getDeamonPlayerData();
            if(!dpData)
                continue;
            dpData->challengeTime = dbtdp.challengeTime;
            dpData->startTime = dbtdp.startTime;
            dpData->deamonLevel = dbtdp.deamonLevel;
            dpData->curLevel = dbtdp.curLevel;
            dpData->maxLevel = dbtdp.maxLevel;
            dpData->accTime = dbtdp.accTime;
            dpData->accLen = dbtdp.accLen;
            dpData->accAwards = dbtdp.accAwards;
            dpData->vitalityTime = dbtdp.vitalityTime;
            dpData->vitality = dbtdp.vitality;
            dpData->spirit = dbtdp.spirit;
            dpData->itemId = dbtdp.itemId;
            dpData->itemNum = dbtdp.itemNum;
            dpData->quitLevel = dbtdp.quitLevel;
            if(dbtdp.attacker != 0)
                dpData->attacker = globalPlayers[dbtdp.attacker];
            else
                dpData->attacker = NULL;

            if(dbtdp.deamonLevel != 0)
                townDeamonManager->loadDeamonPlayersFromDB(dbtdp.deamonLevel, dpData->maxLevel, pl);
        }
		lc.finalize();

        return true;
    }

    bool GObjectManager::LoadWBoss()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading WorldBoss Data");
		DBWBoss t;
		if(execu->Prepare("SELECT `idx`, `last`, `hp`, `atk`, `matk` FROM `wboss`", t)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            Last l;
            l.time = t.last;
            l.hp = t.hp;
            l.atk = t.atk;
            l.matk = t.matk;
            worldBoss.setLast(t.idx, l);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::LoadDiscount()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Discount Data");
		DBDiscount t;
		if(execu->Prepare("SELECT `itemid`, `timeBegin`, `timeEnd`, `priceOriginal`, `priceDiscount`,\
                    `type`, `count` FROM `discount`", t)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            GData::store.addDiscountFromDB(t.itemID, t.type, t.count, t.beginTime, t.endTime, t.priceOriginal, t.priceDiscount);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadSecondSoul()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;

        LoadingCounter lc("Loading Fighter Second Soul:");
		DBSecondSoul dbss;
        Player* pl = NULL;
		if(execu->Prepare("SELECT `fighterId`, `playerId`, `cls`, `xinxiu`, `practiceLevel`, `stateLevel`, `stateExp`, `skills` FROM `second_soul`", dbss) != DB::DB_OK)
			return false;
		lc.reset(20);
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
			if(dbss.playerId != last_id)
			{
				last_id = dbss.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			Fighter * fgt = pl->findFighter(dbss.fighterId);
			if(fgt == NULL || fgt->getLevel() < 60)
            {
                continue;
            }

            SecondSoul* secondSoul = new SecondSoul(fgt, dbss.cls, dbss.practiceLevel, dbss.stateExp, dbss.stateLevel, dbss.xinxiu);
            StringTokenizer tokenizer(dbss.skills, ",");
            int idx = 0;
            for(size_t j = 0; j < tokenizer.count(); ++ j)
            {
                UInt16 skillId = atoi(tokenizer[j].c_str());
                secondSoul->setSoulSkill(idx, skillId);
                ++ idx;
            }
            fgt->setSecondSoul(secondSoul);
		}
		lc.finalize();

        return true;
    }

    bool GObjectManager::LoadSoulItemChance()
    {
        lua_State* L = lua_open();
        luaopen_base(L);
        luaopen_string(L);
        {
            std::string path = cfg.scriptPath + "items/SoulItem.lua";
            lua_tinker::dofile(L, path.c_str());

            lua_tinker::table si_table = lua_tinker::call<lua_tinker::table>(L, "getSoulItemChance");
            size_t si_size = si_table.size();
            for(UInt32 i = 0; i < si_size; i ++)
            {
                lua_tinker::table tempTable = si_table.get<lua_tinker::table>(i + 1);
                int tempSize = tempTable.size();

                std::vector<UInt32> tmpChance;
                for(int j = 1; j < tempSize - 1; ++ j)
                {
                    tmpChance.push_back(tempTable.get<UInt32>(j + 1));
                }

                _soulEnchantChance.push_back(tmpChance);
                _decSoulStateExp.push_back(tempTable.get<UInt32>(tempSize));
            }
        }
        lua_close(L);

        return true;
    }

    bool GObjectManager::LoadLuckyLog()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Lucky Log");
		DBLuckyLog t;
		if(execu->Prepare("SELECT `name`, `items` FROM `luckylog` ORDER BY `id` DESC LIMIT 10", t)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            luckyDraw.pushLog(t.name, t.items);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadRealItemAward()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Real Item Award");
		DBRealItemAward t;
		if(execu->Prepare("SELECT `id`, `cd`, `card_no`, `card_psw` FROM `real_item_award` ORDER BY `id`", t)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            realItemAwardMgr.load(t.id, t.cd, t.card_no, t.card_psw);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadNewRelation()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading New Relation");
		DBNewRelation t;
		if(execu->Prepare("SELECT `playerId`, `mood`, `sign` FROM `new_relation` ORDER BY `playerId`", t)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            Player* pl = globalPlayers[t.playerId];
            if(!pl)
                continue;
            pl->GetNewRelation()->LoadFromDB(t.mood, t.sign);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadRNR()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading RNR");
		DBRNR t;
		if(execu->Prepare("SELECT `id`, `record` FROM `rechargenextret` ORDER BY `id`", t)!= DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
		{
			lc.advance();
            Player* pl = globalPlayers[t.id];
            if (!pl)
                continue;
            pl->loadRNRFromDB(t.record);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadGVar()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading RNR");
        DBGVar gvar;
        if(execu->Prepare("SELECT `id`, `data`, `over` FROM `gvar` ORDER BY `id`", gvar) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
        {
			lc.advance();
            GVarSystem::Instance().LoadVar(gvar.id, gvar.data, gvar.overTime);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadSkillStrengthen()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading skill strengthen");
        DBSS ss;
        if(execu->Prepare("SELECT `id`, `playerId`, `skillid`, `father`, `maxVal`, `curVal`, `lvl`, `maxLvl` FROM `skill_strengthen` ORDER BY `playerId`", ss) != DB::DB_OK)
			return false;
		lc.reset(1000);
        Player* pl = NULL;
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		while(execu->Next() == DB::DB_OK)
        {
			lc.advance();
			if(ss.playerId != last_id)
			{
				last_id = ss.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;
			Fighter * fgt = pl->findFighter(ss.id);
			if(fgt == NULL)
				continue;

            SStrengthen s;
            s.father = ss.father;
            s.maxVal = ss.maxVal;
            s.curVal = ss.curVal;
            s.lvl = ss.lvl;
            s.maxLvl = ss.maxLvl;
            fgt->SSFromDB(ss.skillid, s);
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadQixi()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading Qixi");
        DBQixi qixi;
        if(execu->Prepare("SELECT `playerId`, `lover`, `bind`, `pos`, `event`, `score` FROM `qixi` ORDER BY `playerId`", qixi) != DB::DB_OK)
			return false;
		lc.reset(1000);
        Player* pl = NULL;
        Player* lover = NULL;
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		while(execu->Next() == DB::DB_OK)
        {
			lc.advance();
            lover = NULL;
			if(qixi.playerId != last_id)
			{
				last_id = qixi.playerId;
				pl = globalPlayers[last_id];
				lover = globalPlayers[qixi.lover];
			}
			if(pl == NULL)
				continue;

            pl->loadQixiInfoFromDB(lover, qixi.bind, qixi.pos, qixi.event, qixi.score);
        }
        lc.finalize();
        return true;

    }

    bool GObjectManager::loadJobHunter()
    {
        // 读取寻墨有关数据
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading ExJob");
        DBJobHunter dbjh;
        if(execu->Prepare("SELECT `playerId`, `fighterList`, `mapInfo`, `progress`, `posX`, `posY`, `earlyPosX`, `earlyPosY`, `stepCount` FROM `job_hunter` ORDER BY `playerId`", dbjh) != DB::DB_OK)
			return false;
		lc.reset(1000);
        Player * player = NULL;
		while(execu->Next() == DB::DB_OK)
        {
            player = globalPlayers[dbjh.playerId];
            if (player)
            {
                player->setJobHunter(dbjh.fighterList, dbjh.mapInfo, dbjh.progress, dbjh.posX, dbjh.posY, dbjh.earlyPosX, dbjh.earlyPosY, dbjh.stepCount);
            }
			lc.advance();
        }
        lc.finalize();
        return true;
    }

    bool GObjectManager::loadCopyFrontWin()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading copy_front_win");
        DBCopyFrontWin dbcf;
        if(execu->Prepare("SELECT `playerId`, `posOrig`, `posPut`, `itemId`, `ratio` FROM `copy_front_win` ORDER BY `playerId`, `posOrig`", dbcf) != DB::DB_OK)
			return false;
		lc.reset(1000);
        Player* pl = NULL;
		UInt64 last_id = 0xFFFFFFFFFFFFFFFFull;
		while(execu->Next() == DB::DB_OK)
        {
			lc.advance();
			if(dbcf.playerId != last_id)
			{
				last_id = dbcf.playerId;
				pl = globalPlayers[last_id];
			}
			if(pl == NULL)
				continue;

            pl->loadCopyFrontWinFromDB(dbcf.posOrig, dbcf.posPut, dbcf.itemId, dbcf.ratio);
        }
        lc.finalize();
        return true;
    }

    float  GObjectManager::getAttrMax( UInt8 lvl, UInt8 t, UInt8 q, UInt8 crr )
    {
        if(q > 2)
            q = 2;
        if(t > 8)
            t = 8;
        if(crr >= e_cls_max)
            crr = e_cls_max - 1;
        std::map<UInt8, stAttrMax*>::iterator it = _attrMax.find(lvl);
        stAttrMax* attr = NULL;
        if(it != _attrMax.end())
            attr = it->second;
        else
            attr = _attrMax[10];

        return attr->attrMax[q][crr][t];
    }

    float  GObjectManager::getAttrTrumpMax( UInt8 lvl, UInt8 t, UInt8 q, UInt8 crr )
    {
        if(q > 2)
            q = 2;
        if(t > 8)
            t = 8;
        if(crr >= e_cls_max)
            crr = e_cls_max - 1;
        std::map<UInt8, stAttrMax*>::iterator it = _attrTrumpMax.find(lvl);
        stAttrMax* attr = NULL;
        if(it != _attrTrumpMax.end())
            attr = it->second;
        else
            attr = _attrTrumpMax[0];

        return attr->attrMax[q][crr][t];
    }

    float GObjectManager::getRingHpFromEnchant(UInt8 lvl, UInt8 crr, UInt8 enchant)
    {
        if(enchant == 0)
            return 0;

        if(crr >= e_cls_max)
            crr = 0;
        if(enchant > 12)
            enchant = 0;
        -- enchant;

        std::map<UInt8, stRingHpBase*>::iterator it = _ringHpBase.find(lvl);
        if(it == _ringHpBase.end())
        {
            UInt8 lvl2 = lvl - lvl%10;
            it = _ringHpBase.find(lvl2);
        }

        stRingHpBase* ringHp = NULL;
        if(it == _ringHpBase.end())
            ringHp = _ringHpBase[10];
        else
            ringHp = it->second;

        return ringHp->hpBase[crr] * _ringHpFactor[enchant];
    }

    bool GObjectManager::loadArenaExtraBoard()
    {
		std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
		if (execu.get() == NULL || !execu->isConnected()) return false;
		LoadingCounter lc("Loading arena_extra_board");
        DBArenaExtraBoard extraboard;
        if(execu->Prepare("SELECT `week`, `name1`, `name2`, `name3`, `name4`, `name5`, `heroId1`, `heroId2`, `heroId3`, `heroId4`, `heroId5`, `sufferTotal`, `sufferCnt1`, `sufferCnt2`, `sufferCnt3`, `sufferCnt4`, `sufferCnt5`, `lasttime1`, `lasttime2`, `lasttime3`, `lasttime4`, `lasttime5` FROM `arena_extra_board` ORDER BY `week`", extraboard) != DB::DB_OK)
			return false;
		lc.reset(1000);
		while(execu->Next() == DB::DB_OK)
        {
			lc.advance();
            UInt8 week = extraboard.week;
			if(week == ARENA_WEEK_START || week == ARENA_WEEK_END)
            {
                UInt8 type = week - ARENA_WEEK_START;
                GObject::World::stArenaOld[type].week = extraboard.week;
                GObject::World::stArenaOld[type].sufferTotal = extraboard.sufferTotal;
                for(UInt8 i = 0; i < 5; i++)
                {
                    GObject::World::stArenaOld[type].name[i] = extraboard.name[i];
                    GObject::World::stArenaOld[type].heroId[i] = extraboard.heroId[i];
                    GObject::World::stArenaOld[type].sufferCnt[i] = extraboard.sufferCnt[i];
                    GObject::World::stArenaOld[type].lasttime[i] = extraboard.lasttime[i];
                }

                ValueSort cur;
                ValueSortType resultRank;
                for(UInt8 i = 0; i < 5; i++)
                {
                    cur.sufferCnt = World::stArenaOld[type].sufferCnt[i];
                    cur.lastTime = World::stArenaOld[type].lasttime[i];
                    cur.name = World::stArenaOld[type].name[i];
                    //std::cout << (UInt32)i << "::: " << cur.name << std::endl;
                    resultRank.insert(cur);
                }
                for(UInt8 i = 0; i < 5; i++)
                {
                    UInt8 j = 0;
                    for(ValueSortType::iterator iter = resultRank.begin(), e = resultRank.end(); iter != e && j < 5; ++iter, ++j)
                    {
                        //std::cout << (UInt32)j << ": " << (*iter).name << std::endl;
                        if((*iter).name == World::stArenaOld[type].name[i])
                            break;
                    }
                    //std::cout << "result: " << (UInt32)(j+1) << std::endl;
                    World::stArenaOld[type].rank[i] = j + 1;
                }
            }
        }
        lc.finalize();
        return true;
    }
}


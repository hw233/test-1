#include "Config.h"
#include "GMHandler.h"
#include "MsgID.h"
#include "Server/WorldServer.h"
#include "Server/OidGenerator.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "GData/DungeonData.h"
#include "GData/ExpTable.h"
#include "GData/NpcGroup.h"
#include "GObject/Player.h"
#include "GObject/Package.h"
#include "GObject/Fighter.h"
#include "GObject/Clan.h"
#include "GObject/Country.h"
#include "GObject/ClanManager.h"
#include "GObject/ClanBattle.h"
#include "GObject/ClanManager.h"
#include "GData/DungeonData.h"
#include "GData/ExpTable.h"
#include "GData/NpcGroup.h"
#include "GObject/Dungeon.h"
#include "GObject/Arena.h"
#include "Script/GameActionLua.h"
#include "Script/ConfigScript.h"
#include "Battle/BattleSimulator.h"
#include "Common/StringTokenizer.h"
#include "CountryMsgStruct.h"
#include "GObject/PracticePlace.h"
#include "GObject/Copy.h"

GMHandler gmHandler;

GMHandler::GMHandler()
{

	Reg(3, "donate", &GMHandler::OnClanDonate);
	Reg(3, "banchat", &GMHandler::OnBanChat);
	Reg(3, "addexp", &GMHandler::OnAddExp);
	Reg(3, "exp", &GMHandler::OnAddExp);
	Reg(3, "pexp", &GMHandler::OnAddPExp);
	Reg(3, "addpexp", &GMHandler::OnAddPExp);
	Reg(3, "additem", &GMHandler::OnAddItem);
	Reg(3, "addclanbox", &GMHandler::OnClanBox);
	Reg(3, "item", &GMHandler::OnAddItem);
	Reg(3, "itemb", &GMHandler::OnAddItemB);
	Reg(3, "itemset", &GMHandler::OnAddItemSet);
	Reg(3, "equipall", &GMHandler::OnEquipAll);
	Reg(3, "addmoney", &GMHandler::OnAddMoney);
	Reg(3, "money", &GMHandler::OnAddMoney);
	Reg(3, "topup", &GMHandler::OnTopup);
	Reg(3, "killmoney", &GMHandler::OnKillMoney);
	Reg(3, "addfighter", &GMHandler::OnAddFighter);
	Reg(3, "fighter", &GMHandler::OnAddFighter);
	Reg(3, "setpotential", &GMHandler::OnSetPotential);
	Reg(3, "setp", &GMHandler::OnSetPotential);
	Reg(3, "setf", &GMHandler::OnSetFriendliness);
	Reg(3, "rename", &GMHandler::OnRename);
	Reg(3, "testlua", &GMHandler::OnTestLua);
	Reg(3, "setdl", &GMHandler::OnSetDL);
	Reg(3, "setvip", &GMHandler::OnSetVip);
	Reg(3, "super", &GMHandler::OnSuper);
	Reg(3, "luckydraw", &GMHandler::OnLuckyDraw);
	Reg(3, "spawn", &GMHandler::OnSpawn);
	Reg(3, "unspawn", &GMHandler::OnUnspawn);

	Reg(3, "playerwallow", &GMHandler::OnPlayerWallow);
	Reg(3, "wallow", &GMHandler::OnWallow);
	Reg(3, "setgl", &GMHandler::OnSetGM);
	Reg(3, "reloadLua", &GMHandler::OnReLoadLua);
	Reg(3, "limitluckydraw", &GMHandler::OnLimitLuckyDraw);
	Reg(3, "run", &GMHandler::OnRunScript);
	Reg(3, "reconn", &GMHandler::OnReconn);
	Reg(3, "settitle", &GMHandler::OnSetTitle);
	Reg(3, "title", &GMHandler::OnSetTitle);
	Reg(3, "setbl", &GMHandler::OnSetBosslevel);
	Reg(3, "attack", &GMHandler::OnAttack);
	Reg(3, "challenge", &GMHandler::OnChallenge);
	Reg(3, "setwd", &GMHandler::OnsetWeekDay);
	Reg(3, "upskill", &GMHandler::OnUpSkill);
	Reg(3, "us", &GMHandler::OnUpSkill);
	Reg(3, "offskill", &GMHandler::OnOffSkill);
	Reg(3, "uppsskill", &GMHandler::OnUpPasSkill);
	Reg(3, "ups", &GMHandler::OnUpPasSkill);
	Reg(3, "offpsskill", &GMHandler::OnOffPasSkill);
	Reg(3, "uppeerless", &GMHandler::OnUpPeerless);
	Reg(3, "offpeerless", &GMHandler::OnOffPeerless);
	Reg(3, "op", &GMHandler::OnOffPeerless);
	Reg(3, "learncitta", &GMHandler::OnLearnCitta);
	Reg(3, "lc", &GMHandler::OnLearnCitta);
	Reg(3, "upcitta", &GMHandler::OnUpCitta);
	Reg(3, "uc", &GMHandler::OnUpCitta);
	Reg(3, "offcitta", &GMHandler::OnOffCitta);
	Reg(3, "level", &GMHandler::OnSetLevel);
	Reg(3, "setlevel", &GMHandler::OnSetLevel);
	Reg(3, "forge", &GMHandler::OnForge);
	Reg(3, "gem", &GMHandler::OnGem);
	Reg(3, "enchant", &GMHandler::OnEnchant);
	Reg(3, "resetic", &GMHandler::OnResetIc);
	Reg(3, "autocb", &GMHandler::OnAutoCB);

	Reg(2, "holy", &GMHandler::OnHoly);
	Reg(2, "setspeed", &GMHandler::OnSetSpeed);
	Reg(2, "spd", &GMHandler::OnSetSpeed);
	Reg(2, "move", &GMHandler::OnMove);
	Reg(2, "regen", &GMHandler::OnRegen);

	Reg(1, "pinfo", &GMHandler::OnPlayerInfo);
	Reg(1, "cinfo", &GMHandler::OnCharInfo);
	Reg(1, "enterarena", &GMHandler::OnEnterArena);
	Reg(3, "nextarena", &GMHandler::OnNextArena);

	Reg(1, "enterClan", &GMHandler::OnEnterClan);

	Reg(3, "pay4pra", &GMHandler::OnPay4Pra);
	Reg(3, "sitpra", &GMHandler::OnSitPra);
	Reg(3, "flushtask", &GMHandler::OnFlushTask);
	Reg(3, "setcountry", &GMHandler::OnSetCountry);
	Reg(3, "setacu", &GMHandler::OnSetAcu);
	Reg(3, "useitem", &GMHandler::OnUseItem);
    Reg(3, "ocupyplace", &GMHandler::OnOcupyPlace);
    Reg(3, "ec", &GMHandler::OnEnterCopy);
}

void GMHandler::Reg( int gmlevel, const std::string& code, GMHandler::GMHPROC proc )
{
	std::string code_ = code;
	char * lwr_ = strlwr(&code_[0]);
	for(int i = gmlevel; i < 4; ++ i)
	{
		GMHP& gmhp = _procMap[i][lwr_];
		gmhp.np = false;
		gmhp.proc = proc;
		gmhp.procnp = NULL;
	}
}

void GMHandler::Reg( int gmlevel, const std::string& code, GMHandler::GMHPROCNP proc )
{
	std::string code_ = code;
	char * lwr_ = strlwr(&code_[0]);
	for(int i = gmlevel; i < 4; ++ i)
	{
		GMHP& gmhp = _procMap[i][lwr_];
		gmhp.np = true;
		gmhp.proc = NULL;
		gmhp.procnp = proc;
	}
}

bool GMHandler::Handle( const std::string& txt, GObject::Player * player, bool isFromBackstage)
{
	UInt8 gml = 3;
	if(cfg.GMCheck && !isFromBackstage)
	{
		if(player == NULL)
			return false;
		gml = player->getGMLevel();
		if(gml > 3)
			return false;
	}
	if(txt.empty())
		return false;
	std::string ptxt = txt;
	if(ptxt[0] == '#')
	{
		if(ptxt.length() < 9)
			return false;
		ptxt.erase(ptxt.begin(), ptxt.begin() + 9);
	}
	if(ptxt[0] != '/')
		return false;
	ptxt.erase(ptxt.begin());
	StringTokenizer tk(ptxt, " ", StringTokenizer::TOK_IGNORE_EMPTY || StringTokenizer::TOK_TRIM);
	if(tk.count() == 0)
		return true;
	std::string cmd = tk[0];
	strlwr(&cmd[0]);
	std::map<std::string, GMHP>::iterator it = _procMap[gml].find(cmd);
	if(it == _procMap[gml].end())
		return true;
	std::vector<std::string> sl(tk.begin() + 1, tk.end());
	if(it->second.np)
	{
		GMHPROCNP proc = it->second.procnp;
		(this->*proc)(sl);
	}
	else
	{
		if(player == NULL)
			return false;
		GMHPROC proc = it->second.proc;
		(this->*proc)(player, sl);
	}
	return true;
}

#ifdef _WIN32
#define strtoull _strtoui64
#endif

void GMHandler::OnsetWeekDay(std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
	UInt8 wday = atoi(args[0].c_str());
	if(wday < 1 || wday > 7)
		return;
	WORLD().setWeekDay(wday);
}

void GMHandler::OnClanBox(GObject::Player * player, std::vector<std::string>& args)
{
	struct AddClanBox
	{
		GObject::Clan *cl;
		UInt8 count;
	};
	AddClanBox clb;
	if(args.size() < 2 )
		return;
	if(args[0].empty())
		return;
	clb.cl = globalGlobalNamedClans[player->fixName(args[0])];
	clb.count = atoi(args[1].c_str());
	if(clb.cl == NULL || clb.count == 0) 
		return;
	GameMsgHdr hdr(0x17B, WORKER_THREAD_WORLD, player, sizeof(AddClanBox));
	GLOBAL().PushMsg(hdr, &clb);
}

void GMHandler::OnSetBosslevel(GObject::Player * player, std::vector<std::string>& args)
{
	if(args.empty())
		return;
	UInt8 level = static_cast<UInt8>(atoi(args[0].c_str()));
	if(level < 21 || level > 999)
		return;
	player->setBossLevel(level);
}
void GMHandler::OnClanDonate(GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;

	UInt32 count = atoi(args[0].c_str());
	if(count == 0)
		return;
	struct DonateItem
	{
		UInt8  skillId;
		UInt16 type;
		UInt32 count;
	};
	GObject::Clan *pclan = player->getClan();
	if(pclan == NULL)
		return;
	
	DonateItem items = {1, 1, count};
	GameMsgHdr hdr(0x17A, WORKER_THREAD_WORLD, player, sizeof(DonateItem));
	GLOBAL().PushMsg(hdr, &items);
}
void GMHandler::OnBanChat( std::vector<std::string>& args )
{
	if(args.empty())
		return;
	char * endptr;
	UInt64 playerId = strtoull(args[0].c_str(), &endptr, 10);
	if(playerId == 0)
		return;
	GObject::Player * pl = GObject::globalPlayers[playerId];
	if(pl == NULL)
		return;
	if(args.size() == 1)
	{
		UInt32 exp = atoi(args[0].c_str());
		if(exp == 0)
			return;
		pl->addBuffData(PLAYER_BUFF_BANCHAT, TimeUtil::Now() + 3 * 86400);
	}
	else
	{
		UInt32 days = atoi(args[1].c_str());
		pl->addBuffData(PLAYER_BUFF_BANCHAT, TimeUtil::Now() + days * 86400);
	}
}

void GMHandler::OnAddExp( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() == 1)
	{
		UInt32 exp = atoi(args[0].c_str());
		if(exp == 0)
			return;
		player->AddExp(exp);
	}
	else
	{
		UInt32 exp = atoi(args[1].c_str());
		if(exp == 0)
			return;
		char * endptr;
		UInt64 playerId = strtoull(args[0].c_str(), &endptr, 10);
		if(playerId == 0)
			return;
		GObject::Player * pl = GObject::globalPlayers[playerId];
		if(pl == NULL)
			return;
		pl->AddExp(exp);
	}
}

void GMHandler::OnAddPExp( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() == 1)
	{
		UInt32 exp = atoi(args[0].c_str());
		if(exp == 0)
			return;
		player->AddPExp(exp);
	}
	else
	{
		UInt32 exp = atoi(args[1].c_str());
		if(exp == 0)
			return;
		char * endptr;
		UInt64 playerId = strtoull(args[0].c_str(), &endptr, 10);
		if(playerId == 0)
			return;
		GObject::Player * pl = GObject::globalPlayers[playerId];
		if(pl == NULL)
			return;
		pl->AddPExp(exp);
	}
}

void GMHandler::OnAddItem(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return ;
	for(size_t i = 0; i < args.size(); ++ i)
	{
		UInt32 itemId = atoi(args[i].c_str());
		if(IsEquipTypeId(itemId))
		{
			player->GetPackage()->AddEquip(itemId);
		}
		else
		{
			++ i;
			if(i >= args.size())
				return;
			UInt32 itemNum = atoi(args[i].c_str());
			player->GetPackage()->AddItem(itemId, itemNum);
		}
	}
}

void GMHandler::OnAddItemB(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return ;
	for(size_t i = 0; i < args.size(); ++ i)
	{
		UInt32 itemId = atoi(args[i].c_str());
		if(IsEquipTypeId(itemId))
		{
			player->GetPackage()->AddEquip(itemId, true);
		}
		else
		{
			++ i;
			if(i >= args.size())
				return;
			UInt32 itemNum = atoi(args[i].c_str());
			player->GetPackage()->AddItem(itemId, itemNum, true);
		}
	}
}

void GMHandler::OnAddItemSet(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 2)
		return;
	int set = atoi(args[0].c_str());
	int lvl = atoi(args[1].c_str());
	if(set > 1)
		return;
	int idx = -1;
	if(lvl == 50)
		idx = 0;
	else if(lvl == 70)
		idx = 1;
	else if(lvl == 80)
		idx = 2;
	else if(lvl == 90)
		idx = 3;
	else if(lvl == 100)
		idx = 4;
	else
		return;
	const UInt32 itemIdStart[2][5] = {{1521, 1561, 1601, 1681, 1721}, {1541, 1581, 1621, 1701, 1741}};
	for(UInt32 n = 0; n < 14; ++ n)
	{
		UInt32 itemId = itemIdStart[set][idx] + n;
		player->GetPackage()->AddEquip(itemId);
	}
}

void GMHandler::OnAddMoney( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() < 1)
		return;
	if(args.size() == 1)
	{
		UInt32 val = atoi(args[0].c_str());
		if(val == 0)
			return;
		player->getCoin(val);
	}
	else
	{
		switch(atoi(args[0].c_str()))
		{
		case 1:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->getTael(val);
			}
			break;
		case 2:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->getGold(val);
			}
			break;
		case 3:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->getCoupon(val);
			}
			break;
		case 4:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->getAchievement(val);
			}
		}
	}
}

void GMHandler::OnKillMoney( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() < 1)
		return;
	if(args.size() == 1)
	{
		UInt32 val = atoi(args[0].c_str());
		if(val == 0)
			return;
		player->useCoin(val);
	}
	else
	{
		switch(atoi(args[0].c_str()))
		{
		case 1:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->useTael(val);
			}
			break;
		case 2:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->useGold(val);
			}
			break;
		case 3:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->useCoupon(val);
			}
			break;
		case 4:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->useAchievement(val);
			}
		}
	}
}

void GMHandler::OnSetVip( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	UInt32 level = atoi(args[0].c_str());
	if(level > 10)
		return;
	const static UInt32 gold[] = {0, 200, 1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 300000};
	player->setTotalRecharge(gold[level]);
}

void GMHandler::OnTopup( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	UInt32 val = atoi(args[0].c_str());
	player->getGold(val);
	player->addTotalRecharge(val);
}

void GMHandler::OnHoly( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() == 0)
	{
		if(player->getBuffData(PLAYER_BUFF_HOLY) == 0)
			player->setBuffData(PLAYER_BUFF_HOLY, player->getGMLevel() == 2 ? 2 : 4);
		else
			player->setBuffData(PLAYER_BUFF_HOLY, 0);
	}
	else
	{
		if(args[0] == "on")
			player->setBuffData(PLAYER_BUFF_HOLY, player->getGMLevel() == 2 ? 2 : 4);
		else if(args[0] == "off")
			player->setBuffData(PLAYER_BUFF_HOLY, 0);
		else
		{
			UInt32 val = atoi(args[0].c_str());
			if(val < 7)
				player->setBuffData(PLAYER_BUFF_HOLY, val);
		}
	}
}

void GMHandler::OnRegen( GObject::Player * player, std::vector<std::string>& )
{
	player->regenAll();
}

void GMHandler::OnSetGM( std::vector<std::string>& args )
{
	if(args.size() < 2)
		return;
	char * endptr;
	UInt64 id = strtoull(args[0].c_str(), &endptr, 10);
	if(id == 0)
		return;
	GObject::Player * player2 = globalPlayers[id];
	if(player2 == NULL)
		return;
	player2->setGMLevel(atoi(args[1].c_str()));
}

void GMHandler::OnTestLua( GObject::Player * player, std::vector<std::string>& args )
{
	if (args.size() == 0) return ;
	GameAction()->DoLuaTest(player, args[0]);
}

void GMHandler::OnReLoadLua( std::vector<std::string>& args )
{
	/*
	  mask: 0x00F - Universe  0x0F0 - world  0xF00 - country
	  0x001  - battle
	  0x002  - activity
	  0x010  - data
	  0x100  - task/item
	*/
	UInt16 reloadFlag = 0;
	bool world = false;
	bool country = false;
	if(args.empty())
	{
		world = country = true;
		reloadFlag = 0xFFFF;
	}
	else
	{
		for(size_t i = 0; i < args.size(); ++ i)
		{
			if(args[i] == "world")
			{
				world = true;
				reloadFlag |= 0x00FF;
			}
			else if(args[i] == "country")
			{
				country = true;
				reloadFlag |= 0x0F0F;
			}
			else if(args[i] == "battle")
			{
				world = country = true;
				reloadFlag |= 0x1;
			}
			else if(args[i] == "data")
			{
				world = true;
				reloadFlag |= 0x10;
			}
			else if(args[i] == "task")
			{
				country = true;
				reloadFlag |= 0x100;
			}
			else if(args[i] == "act")
			{
				world = country = true;
				reloadFlag |= 0x02;
			}
		}
	}
	if(world)
	{
		GameMsgHdr hdr4(0x1EE, WORKER_THREAD_WORLD, NULL, sizeof(UInt32));
		GLOBAL().PushMsg(hdr4, &reloadFlag);
	}
	else if(country)
	{
		GameMsgHdr hdr1(0x1EE, WORKER_THREAD_COUNTRY_1, NULL, sizeof(UInt16));
		GLOBAL().PushMsg(hdr1, &reloadFlag);
		GameMsgHdr hdr2(0x1EE, WORKER_THREAD_COUNTRY_2, NULL, sizeof(UInt16));
		GLOBAL().PushMsg(hdr2, &reloadFlag);
		GameMsgHdr hdr3(0x1EE, WORKER_THREAD_NEUTRAL, NULL, sizeof(UInt16));
		GLOBAL().PushMsg(hdr3, &reloadFlag);
	}
}

void GMHandler::OnAddFighter( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	UInt32 id = 0;
	if(args.size() > 1)
	{
		char * endptr;
		player = GObject::globalPlayers[strtoull(args[0].c_str(), &endptr, 10)];
		if(player == NULL)
			return;
		id = atoi(args[1].c_str());
	}
	else
		id = atoi(args[0].c_str());
	if(id == 0 || id > GREAT_FIGHTER_MAX)
		return;
	GObject::Fighter * fgt = GObject::globalFighters[id];
	if(fgt == NULL)
		return;
	player->takeFighter(id, true);
}

void GMHandler::OnSetPotential( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() < 2)
		return;
	UInt32 id = atoi(args[0].c_str());
	GObject::Fighter * fgt = player->findFighter(id);
	if(fgt == NULL)
		return;
	float p = static_cast<float>(atof(args[1].c_str()));
	if(p < 0.60f || p > 2.00f)
		return;
	fgt->setPotential(p);
}

void GMHandler::OnSetSpeed( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	UInt32 spd = 0;
	if(args.size() > 1)
	{
		char * endptr;
		player = GObject::globalPlayers[strtoull(args[0].c_str(), &endptr, 10)];
		if(player == NULL)
			return;
		spd = atoi(args[1].c_str());
	}
	else
		spd = atoi(args[0].c_str());
	if(spd <= 0)
		return;
	if(spd > 16)
		spd = 16;
	player->sendModification(9, spd, false);
}

void GMHandler::OnRename( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args[0].empty() || args[0].size() > 15)
		return;
	GObject::Fighter * fgt = player->getMainFighter();
	if(fgt == NULL)
		return;
	std::string nameTmp(args[0]);
	GObject::Player *playertmp = globalNamedPlayers[player->fixName(args[0])];
	if(playertmp != NULL)
		return ;
	GObject::globalNamedPlayers.remove(PLAYER_DATA(player, name));
	PLAYER_DATA(player, name) = args[0];
	//if(cfg.merged && player->getId() >= 0x1000000000000ull)
	//{
	//	PLAYER_DATA(player, name).push_back(static_cast<char>(player->getId() >> 48));
	//}
	//player->setClan(player->getClan());
	player->rebuildBattleName();
	fgt->setName(args[0]);
	GObject::globalNamedPlayers.add(args[0], player);
	DB().PushUpdateData("UPDATE `player` SET `name` = '%s' WHERE id = %"I64_FMT"u", nameTmp.c_str(), player->getId());
	DBLOG().PushUpdateData("update `register_states` set `player_name` = '%s' where `server_id` = %u and `player_id` = %"I64_FMT"u", nameTmp.c_str(), cfg.serverLogId, player->getId());
}

void GMHandler::OnPlayerInfo( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(args[0])];
	if(pl == NULL)
		return;
	UInt16 loc = pl->getLocation();
	GObject::Map * map = GObject::Map::FromSpot(loc);
	if(map == NULL)
		return;
	GObject::SpotData * sd = NULL;
	bool inCity = PLAYER_DATA(pl, inCity) > 0;
	if(inCity)
	{
		sd = map->GetSpot(loc);
	}
	SYSMSG_SENDV(600, player, pl->getId(), pl->getName().c_str(), pl->GetLev());
	SYSMSG_SENDV(601, player, map->GetName().c_str(), ((sd != NULL) ? (sd->m_Name.c_str()) : ""));
	SYSMSG_SENDV(602, player, PLAYER_DATA(pl, gold), PLAYER_DATA(pl, coupon));
	SYSMSG_SENDV(603, player, PLAYER_DATA(pl, tael), PLAYER_DATA(pl, coin));
}

void GMHandler::OnCharInfo( GObject::Player * player, std::vector<std::string>& args )
{
	player->sendAllFightersInfoToGM();
}

void GMHandler::OnMove( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	char * endptr;
	UInt16 loc;
	if(args[0].size() > 2 && args[0][0] == '0' && (args[0][1] == 'x' || args[0][1] == 'X'))
		loc = static_cast<UInt16>(strtoul(args[0].c_str() + 2, &endptr, 16));
	else
		loc = static_cast<UInt16>(strtoul(args[0].c_str(), &endptr, 10));
	GObject::SpotData * sd = Map::Spot(loc);
	if(sd == NULL)
		return;
	player->moveTo(loc, true);
}

void GMHandler::OnSetFriendliness( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() < 2)
		return;
	UInt32 fgtId;
	UInt16 friendliness = 0;
	if(args.size() > 2)
	{
		char * endptr;
		player = GObject::globalPlayers[strtoull(args[0].c_str(), &endptr, 10)];
		if(player == NULL)
			return;
		fgtId = atoi(args[1].c_str());
		friendliness = static_cast<UInt16>(atoi(args[2].c_str()));
	}
	else
	{
		fgtId = atoi(args[0].c_str());
		friendliness = static_cast<UInt16>(atoi(args[1].c_str()));
	}
	player->setGreatFighterFriendliness(fgtId, friendliness);
}

void GMHandler::OnSetDL( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() < 3)
		return;
	UInt32 id = atoi(args[0].c_str());
	GObject::Dungeon * dg = GObject::dungeonManager[id];
	if(dg == NULL)
		return;
	UInt32 level = atoi(args[1].c_str()) - 1;
	dg->playerJump(player, level);
}

void makeItemSuper( GObject::Package * package, GObject::ItemEquip * equip, UInt8 type, UInt8 enchant = 10, UInt8 level = 10, bool flushAttr = true )
{
	if(equip == NULL || level > 10)
		return;
	ItemEquipData& ied = equip->getItemEquipData();
	ItemClass ic = equip->getClass();
	ied.sockets = 6;
	if(enchant > 0 && ic != Item_Amulet && ic != Item_Ring)
	{
		ied.enchant = enchant;
	}
	if(level > 0 && level <= 10)
	{
		switch(type)
		{
		case 0:
			if(flushAttr)
			{
				ied.extraAttr2.type1 = 4;
				ied.extraAttr2.value1 = 150;
				ied.extraAttr2.type2 = 7;
				ied.extraAttr2.value2 = 150;
				ied.extraAttr2.type3 = 1;
				ied.extraAttr2.value3 = 450;
			}
			ied.gems[0] = 5010;
			ied.gems[1] = 5040;
			ied.gems[2] = 5050;
			ied.gems[3] = 5060;
			ied.gems[4] = 5080;
			ied.gems[5] = 5090;
			switch(ic)
			{
			case Item_Armor1:
			case Item_Armor2:
				ied.gems[3] = 5070;
				break;
			case Item_Armor3:
			case Item_Armor4:
				ied.gems[3] = 5020;
				break;
			case Item_Armor5:
				ied.gems[3] = 5130;
				break;
			case Item_Ring:
			case Item_Amulet:
				ied.gems[3] = 5100;
				ied.gems[2] = 5110;
				ied.gems[1] = 5120;
				break;
			default:
				break;
			}
			break;
		case 1:
			if(flushAttr)
			{
				ied.extraAttr2.type1 = 2;
				ied.extraAttr2.value1 = 150;
				ied.extraAttr2.type2 = 6;
				ied.extraAttr2.value2 = 280;
				ied.extraAttr2.type3 = 5;
				ied.extraAttr2.value3 = 300;
			}
			ied.gems[0] = 5010;
			ied.gems[1] = 5020;
			ied.gems[2] = 5040;
			ied.gems[3] = 5060;
			ied.gems[4] = 5080;
			ied.gems[5] = 5090;
			switch(ic)
			{
			case Item_Armor1:
			case Item_Armor2:
				ied.gems[3] = 5070;
				break;
			case Item_Armor3:
			case Item_Armor4:
				ied.gems[3] = 5050;
				break;
			case Item_Armor5:
				ied.gems[3] = 5130;
				break;
			case Item_Ring:
			case Item_Amulet:
				ied.gems[2] = 5110;
				ied.gems[3] = 5120;
				break;
			default:
				break;
			}
			break;
		}
		for(int i = 0; i < 6; ++ i)
		{
			ied.gems[i] -= 10 - level;
		}
	}
	else
	{
		for(int i = 0; i < 6; ++ i)
		{
			ied.gems[i] = 0;
		}
	}
	DB().PushUpdateData("UPDATE `equipment` SET `enchant` = %u, `sockets` = %u, `socket1` = %u, `socket2` = %u, `socket3` = %u, `socket4` = %u, `socket5` = %u, `socket6` = %u, `attrType1` = %u, `attrValue1` = %d, `attrType2` = %u, `attrValue2` = %d, `attrType3` = %u, `attrValue3` = %d WHERE `id` = %u",
		ied.enchant, ied.sockets, ied.gems[0], ied.gems[1], ied.gems[2], ied.gems[3], ied.gems[4], ied.gems[5], ied.extraAttr2.type1, ied.extraAttr2.value1, ied.extraAttr2.type2, ied.extraAttr2.value2, ied.extraAttr2.type3, ied.extraAttr2.value3, equip->getId());
}

void makeSuper( GObject::Fighter * fgt, UInt8 equipLvl = 100, UInt8 enchant = 10, UInt8 gemlevel = 10, bool flushAttr = true )
{
	if(fgt == NULL)
		return;
	GObject::Player * player = fgt->getOwner();
	if(player == NULL)
		return;
	const UInt32 itemIdStart[3][5] = {
        {2088, 2112, 2136, 2160, 2184},
        {2080, 2104, 2128, 2152, 2176},
        {2072, 2096, 2120, 2144, 2168}
    };
	int idx = -1;
	if(equipLvl == 60)
		idx = 0;
	else if(equipLvl == 70)
		idx = 1;
	else if(equipLvl == 80)
		idx = 2;
	else if(equipLvl == 90)
		idx = 3;
	else if(equipLvl == 100)
		idx = 4;
	else
		return;
	if(flushAttr)
		fgt->setPotential(1.79f);
	GObject::Package * package = player->GetPackage();
	GObject::ItemEquip * o = NULL;
	GObject::ItemWeapon * weapon;
	GObject::ItemArmor * armor;
	GObject::ItemEquip * equip;
	switch(fgt->getClass())
	{
	case 1:
		weapon = static_cast<GObject::ItemWeapon *>(package->AddEquip(itemIdStart[0][idx], false, true));
        if(weapon)
        { 
            makeItemSuper(package, weapon, 0, enchant, gemlevel, flushAttr);
            package->EquipTo(weapon->getId(), fgt, 0x21, o);
        }
        armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[0][idx] + 1, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 0, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x22, o);
        }
        armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[0][idx] + 3, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 0, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x23, o);
        }
        armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[0][idx] + 2, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 0, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x24, o);
        }
        armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[0][idx] + 4, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 0, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x25, o);
        }
        armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[0][idx] + 5, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 0, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x26, o);
        }
        equip = static_cast<GObject::ItemEquip *>(package->AddEquip(itemIdStart[0][idx] + 6, false, true));
        if(equip)
        { 
            makeItemSuper(package, equip, 0, enchant, gemlevel, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x27, o);
        }
        equip = static_cast<GObject::ItemEquip *>(package->AddEquip(itemIdStart[0][idx] + 7, false, true));
        if(equip)
        { 
            makeItemSuper(package, equip, 0, enchant, gemlevel, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x28, o);
        }
		break;
	case 2:
		weapon = static_cast<GObject::ItemWeapon *>(package->AddEquip(itemIdStart[1][idx], false, true));
        if(weapon)
        { 
            makeItemSuper(package, weapon, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(weapon->getId(), fgt, 0x21, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[1][idx] + 1, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x22, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[1][idx] + 3, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x23, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[1][idx] + 2, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x24, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[1][idx] + 4, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x25, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[1][idx] + 5, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x26, o);
        }
		equip = static_cast<GObject::ItemEquip *>(package->AddEquip(itemIdStart[1][idx] + 6, false, true));
        if(equip)
        { 
            makeItemSuper(package, equip, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x27, o);
        }
		equip = static_cast<GObject::ItemEquip *>(package->AddEquip(itemIdStart[1][idx] + 7, false, true));
        if(equip)
        { 
            makeItemSuper(package, equip, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x28, o);
        }
		break;
	case 3:
		weapon = static_cast<GObject::ItemWeapon *>(package->AddEquip(itemIdStart[2][idx], false, true));
	    if(weapon)
        { 
            makeItemSuper(package, weapon, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(weapon->getId(), fgt, 0x21, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[2][idx] + 1, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x22, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[2][idx] + 3, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x23, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[2][idx] + 2, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x24, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[2][idx] + 4, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x25, o);
        }
		armor = static_cast<GObject::ItemArmor *>(package->AddEquip(itemIdStart[2][idx] + 5, false, true));
        if(armor)
        { 
            makeItemSuper(package, armor, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(armor->getId(), fgt, 0x26, o);
        }
		equip = static_cast<GObject::ItemEquip *>(package->AddEquip(itemIdStart[2][idx] + 6, false, true));
        if(equip)
        { 
            makeItemSuper(package, equip, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x27, o);
        }
		equip = static_cast<GObject::ItemEquip *>(package->AddEquip(itemIdStart[2][idx] + 7, false, true));
        if(equip)
        { 
            makeItemSuper(package, equip, 1, enchant, gemlevel, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x28, o);
        }
		break;
	default:
		break;
	}
}

void addSuperClass( GObject::Player * player, UInt32 id )
{
	GObject::Fighter * fgt = player->takeFighter(id);
    if (!fgt)
    {
        WARN_LOG("get fighter by id");
        return;
    }
	UInt64 exp = GData::expTable.getLevelMin(100);
	fgt->addExp(exp);
	makeSuper(fgt);
}
void GMHandler::OnPlayerWallow( GObject::Player * player, std::vector<std::string>& args )
{
	UInt8 type = 0;
	if(args.empty())
		return;
	if(args.size() >= 2)
	{
		char * endptr;
		UInt64 playerId = strtoull(args[0].c_str(), &endptr, 10);
		if(playerId == 0)
			return;
		player = globalPlayers[playerId];
		if(player == NULL)
			return;
		type = atoi(args[1].c_str());
	}
	else
	{
		type = atoi(args[0].c_str());
	}
	if(type > 0)
		type = 1;
	if(PLAYER_DATA(player, wallow) != type)
	{
		PLAYER_DATA(player, wallow) = type;
		DB().PushUpdateData("UPDATE `player` SET `wallow`=%u WHERE `id`=%"I64_FMT"u", type, player->getId());
		player->sendWallow();
	}
}
void GMHandler::OnSuper( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() >= 1)
	{
		char * endptr;
		UInt64 playerId = strtoull(args[0].c_str(), &endptr, 10);
		if(playerId == 0)
			return;
		player = globalPlayers[playerId];
		if(player == NULL)
			return;
	}
	if(player->getFighterCount() > 1 || player->GetPackage()->GetRestPackageSize() < 40)
		return;
	player->AddExp(GData::expTable.getLevelMin(100));
    player->AddPExp(100000);
	makeSuper(player->getMainFighter());
	addSuperClass(player, 10);
	addSuperClass(player, 16);
    addSuperClass(player, 13);
    addSuperClass(player, 15);
	switch(player->GetClass())
	{
#if 0
	case 1:
		addSuperClass(player, 316);
		addSuperClass(player, 321);
		break;
	case 3:
		addSuperClass(player, 311);
		addSuperClass(player, 321);
		break;
	case 5:
		addSuperClass(player, 311);
		addSuperClass(player, 316);
		break;
#else
#endif
	default:
		break;
	}
}

static void doEquipFighter( const std::pair<UInt32, GObject::Fighter *>& p, int level )
{
	makeSuper(p.second, level, 0, 100, false);
}

void GMHandler::OnEquipAll(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return;
	if (args.size() > 1)
	{
		int fgtId = atoi(args[0].c_str());
		int lvl = atoi(args[1].c_str());
		GObject::Fighter * fgt = player->findFighter(fgtId);
		if(fgt == NULL)
			return;
		makeSuper(fgt, lvl, 0, 0, false);
	}
	else
	{
		int lvl = atoi(args[0].c_str());
		using namespace std::placeholders;
		player->foreachFighter(std::bind(doEquipFighter, _1, lvl));
	}
}

inline static bool enumWOff(Network::GameClient * client, Stream * st)
{
	client->send(&(*st)[0], st->size());
	return true;
}

inline static bool enumWOn(Network::GameClient * client, bool data)
{
	GObject::Player * player = client->GetPlayer();
	if(player == NULL)
		return true;
	Stream st;
	player->makeWallow(st);
	client->send(&st[0], st.size());
	return true;
}

inline static bool enumLD(Network::GameClient * client, int data)
{
	GObject::Player * player = client->GetPlayer();
	if(player == NULL)
		return true;
	UInt8 status = static_cast<UInt8>(PLAYER_DATA(player, status));
	if(data == 2 || (data == 1 && player->getVipLevel() == 0))
		status |= 0x80;
	player->sendModification(5, status, false);
	return true;
}

void GMHandler::OnWallow( std::vector<std::string>& arglist )
{
	if(arglist.empty())
		return;
	bool n = atoi(arglist[0].c_str()) != 0;
	if(cfg.enableWallow == n)
		return;
	cfg.enableWallow = n;
	if(n)
		NETWORK()->Enumerate<Network::GameClient>(enumWOn, n);
	else
	{
		Stream st(REP::WALLOW_VERIFY);
		st << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt32>(0) << Stream::eos;
		NETWORK()->Enumerate<Network::GameClient>(enumWOff, &st);
	}
}

void GMHandler::OnLimitLuckyDraw( std::vector<std::string>& arglist )
{
	if(arglist.empty())
		return;
	int n = atoi(arglist[0].c_str());
	if(cfg.limitLuckyDraw == n)
		return;
	cfg.limitLuckyDraw = n;
	NETWORK()->Enumerate<Network::GameClient>(enumLD, n);
}

void GMHandler::OnLuckyDraw( GObject::Player * player, std::vector<std::string>& arglist )
{
	bool n = true;
	if(!arglist.empty())
		n = atoi(arglist[0].c_str()) > 0;

	if(n)
		player->removeStatus(Player::NoLuckyDraw);
	else
		player->addStatus(Player::NoLuckyDraw);
}

void GMHandler::OnRunScript( std::vector<std::string>& arglist )
{
	if(arglist.size() < 2)
		return;
	if(arglist[0] == "config")
	{
		Script::ConfigScript script(&cfg);
		script.runScript(arglist[1].c_str());
	}
	else if(arglist[0] == "country")
	{
		size_t len = 1 + arglist[1].length();
		GameMsgHdr hdr1(0x1ED, WORKER_THREAD_COUNTRY_1, NULL, len);
		GLOBAL().PushMsg(hdr1, (void *)arglist[1].c_str());
		GameMsgHdr hdr2(0x1ED, WORKER_THREAD_COUNTRY_2, NULL, len);
		GLOBAL().PushMsg(hdr2, (void *)arglist[1].c_str());
		GameMsgHdr hdr3(0x1ED, WORKER_THREAD_NEUTRAL, NULL, len);
		GLOBAL().PushMsg(hdr3, (void *)arglist[1].c_str());
	}
	else if(arglist[0] == "world")
	{
		size_t len = 1 + arglist[1].length();
		GameMsgHdr hdr(0x1ED, WORKER_THREAD_WORLD, NULL, len);
		GLOBAL().PushMsg(hdr, (void *)arglist[1].c_str());
	}
	else if(arglist[0] == "battle")
	{
		size_t len = 1 + arglist[1].length();
		GameMsgHdr hdr1(0x1EF, WORKER_THREAD_COUNTRY_1, NULL, len);
		GLOBAL().PushMsg(hdr1, (void *)arglist[1].c_str());
		GameMsgHdr hdr2(0x1EF, WORKER_THREAD_COUNTRY_2, NULL, len);
		GLOBAL().PushMsg(hdr2, (void *)arglist[1].c_str());
		GameMsgHdr hdr3(0x1EF, WORKER_THREAD_NEUTRAL, NULL, len);
		GLOBAL().PushMsg(hdr3, (void *)arglist[1].c_str());
		GameMsgHdr hdr4(0x1EF, WORKER_THREAD_WORLD, NULL, len);
		GLOBAL().PushMsg(hdr4, (void *)arglist[1].c_str());
	}
}

void GMHandler::OnReconn( std::vector<std::string>& arglist )
{
	DB().Pause();
	DBLOG().Pause();
}

void GMHandler::OnSpawn( std::vector<std::string>& arglist )
{
	if(arglist.size() < 2)
		return;
	UInt16 location = atoi(arglist[0].c_str());
	Map * map = Map::FromSpot(location);
	if(map != NULL)
	{
		UInt16 id = atoi(arglist[1].c_str());
		map->Show(id);
	}
}

void GMHandler::OnUnspawn( std::vector<std::string>& arglist )
{
	if(arglist.size() < 2)
		return;
	UInt16 location = atoi(arglist[0].c_str());
	Map * map = Map::FromSpot(location);
	if(map != NULL)
	{
		UInt16 id = atoi(arglist[1].c_str());
		map->Hide(id);
	}
}

void GMHandler::OnSetTitle( std::vector<std::string>& args )
{
	if(args.size() < 2)
		return;
	char * endptr;
	UInt64 id = strtoull(args[0].c_str(), &endptr, 10);
	if(id == 0)
		return;
	GObject::Player * player2 = globalPlayers[id];
	if(player2 == NULL)
		return;
	player2->setTitle(atoi(args[1].c_str()));
}

void GMHandler::OnAttack( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() < 1)
		return;
	UInt32 npcId = atoi(args[0].c_str());
	int npcCount = 1;
	if(args.size() > 1)
		npcCount = atoi(args[1].c_str());
	player->regenAll();
	int win = 0;
	if(npcId < 4096)
	{
		UInt8 dungeonId = (npcId % 1000) / 100;
		UInt8 dungeonLevel = (npcId % 100) - 1;
		GObject::Dungeon * dg = GObject::dungeonManager[dungeonId];
		const GData::DungeonData * dd = GData::dungeons[dungeonId];
		if(dg == NULL || dd == NULL)
			return;
		for(int i = 0; i < npcCount; ++ i)
		{
			if(dg->doAttack(player, dungeonLevel))
				++ win;
		}
        // TODO:
		SYSMSG_SENDV(610, player, dd->getName().c_str(), 0, dungeonLevel + 1, npcCount, win, static_cast<float>(win * 10000 / npcCount) / 100);
	}
	else
	{
		GData::NpcGroup * ng = GData::npcGroups[npcId];
		if(ng == NULL)
			return;
		for(int i = 0; i < npcCount; ++ i)
		{
			if(player->attackNpc(npcId, 0xFFFFFFFF, true))
				++ win;
		}
		SYSMSG_SENDV(611, player, ng->getName().c_str(), npcCount, win, static_cast<float>(win * 10000 / npcCount) / 100);
	}
}

void GMHandler::OnChallenge( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.size() < 2)
		return;
	char * endptr;
	UInt64 playerId1 = strtoull(args[0].c_str(), &endptr, 10);
	UInt64 playerId2 = strtoull(args[1].c_str(), &endptr, 10);
	if(playerId1 == 0 || playerId2 == 0)
		return;
	GObject::Player * pl1 = GObject::globalPlayers[playerId1];
	if(pl1 == NULL)
		return;
	GObject::Player * pl2 = GObject::globalPlayers[playerId2];
	if(pl2 == NULL)
		return;
	if(args.size() > 2)
	{
		int count = atoi(args[2].c_str());
		if(count > 0)
		{
			int win = 0;
			for(int i = 0; i < count; ++ i)
			{
				Battle::BattleSimulator bsim(0x8FFF, pl1, pl2);
				pl1->PutFighters( bsim, 0, true );
				pl2->PutFighters( bsim, 1, true );
				bsim.start();
				if(bsim.getWinner() == 1) ++ win;
			}
			SYSMSG_SENDV(612, player, pl1->getName().c_str(), pl2->getName().c_str(), count, win, static_cast<float>(win * 10000 / count) / 100);
			return;
		}
	}
	Battle::BattleSimulator bsim(0x8FFF, pl1, pl2);
	pl1->PutFighters( bsim, 0, true );
	pl2->PutFighters( bsim, 1, true );
	bsim.start();
	bool res = bsim.getWinner() == 1;

	Stream st(REP::ATTACK_NPC);
	st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
	player->send(st);
}

void GMHandler::OnUpSkill( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 2)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt32 skillId = atoi(args[1].c_str());(void)skillId;
		UInt32 skillLevel = atoi(args[2].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;
		if(skillLevel == 0 || skillLevel > 40)
			return;

        UInt16 num = fgt->getUpSkillsNum();
        fgt->upSkill(SKILLANDLEVEL(skillId, skillLevel), num);
	}
}

void GMHandler::OnLearnCitta( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 2)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt32 cittaId = atoi(args[1].c_str());(void)cittaId;
		UInt32 cittaLevel = atoi(args[2].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;

        fgt->addNewCitta(CITTAANDLEVEL(cittaId, cittaLevel));
	}
}

void GMHandler::OnUpCitta( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 2)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt32 cittaId = atoi(args[1].c_str());(void)cittaId;
		UInt32 cittaLevel = atoi(args[2].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;

        UInt16 num = fgt->getUpCittasNum();
        fgt->upCitta(CITTAANDLEVEL(cittaId, cittaLevel), num);
	}
}

void GMHandler::OnOffSkill( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 1)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt32 skillId = atoi(args[1].c_str());(void)skillId;
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;
        fgt->offSkill(SKILLANDLEVEL(skillId, 0));
    }
}

void GMHandler::OnOffCitta( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 1)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt32 cittaId = atoi(args[1].c_str());(void)cittaId;
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;
        fgt->offCitta(CITTAANDLEVEL(cittaId, 0));
    }
}

void GMHandler::OnUpPasSkill( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 2)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt16 skillId = atoi(args[1].c_str());(void)skillId;
		UInt16 skillLevel = atoi(args[2].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;
		if(skillLevel == 0 || skillLevel > 40)
			return;

        UInt16 skill = SKILLANDLEVEL(skillId, skillLevel);
        UInt16 skills[1] = {skill};
        fgt->upPassiveSkill(skills, 1);
	}
}

void GMHandler::OnOffPasSkill( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() >= 2)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt16 skillId = atoi(args[1].c_str());(void)skillId;
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;

        UInt16 skill = SKILLANDLEVEL(skillId, 0);
        fgt->offSkill(skill);
    }
}

void GMHandler::OnUpPeerless( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 2)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt16 skillId = atoi(args[1].c_str());(void)skillId;
		UInt16 skillLevel = atoi(args[2].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;

        UInt16 skill = SKILLANDLEVEL(skillId, skillLevel);
        fgt->setPeerless(skill);
    }
}

void GMHandler::OnOffPeerless( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() >= 1)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;

        fgt->offPeerless(0);
    }
}

void GMHandler::OnSetLevel( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 1)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;
		UInt8 lvl = atoi(args[1].c_str());
		UInt64 exp = GData::expTable.getLevelMin(lvl);
		fgt->setLevelAndExp(lvl, exp);
	}
	else
	{
		UInt8 lvl = atoi(args[0].c_str());
		UInt64 exp = GData::expTable.getLevelMin(lvl);
		player->setLevelAndExp(lvl, exp);
	}
}

static void makeExtraAttr( GObject::Fighter * fgt, UInt8 pos, GObject::ItemEquip * equip, UInt8 t1, Int16 v1, UInt8 t2, Int16 v2, UInt8 t3, Int16 v3 )
{
	if(equip == NULL)
		return;
	GObject::ItemEquipData& ied = equip->getItemEquipData();
	GObject::ItemEquipAttr2& ea2 = ied.extraAttr2;
	if(ea2.type1 == t1 && ea2.type2 == t2 && ea2.type3 == t3 && ea2.value1 == v1 && ea2.value2 == v2 && ea2.value3 == v3)
		return;
	ea2.type1 = t1;
	ea2.value1 = v1;
	ea2.type2 = t2;
	ea2.value2 = v2;
	ea2.type3 = t3;
	ea2.value3 = v3;
	DB().PushUpdateData("UPDATE `equipment` SET `attrType1` = %u, `attrValue1` = %d, `attrType2` = %u, `attrValue2` = %d, `attrType3` = %u, `attrValue3` = %d WHERE `id` = %u",
		ea2.type1, ea2.value1, ea2.type2, ea2.value2, ea2.type3, ea2.value3, equip->getId());
	fgt->sendModification(0x21 + pos, equip, false);
}

static void makeFighterExtraAttr( GObject::Fighter * fgt, UInt8 t1, Int16 v1, UInt8 t2, Int16 v2, UInt8 t3, Int16 v3 )
{
	makeExtraAttr(fgt, 0, fgt->getWeapon(), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 1, fgt->getArmor(0), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 2, fgt->getArmor(1), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 3, fgt->getArmor(2), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 4, fgt->getArmor(3), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 5, fgt->getArmor(4), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 6, fgt->getAmulet(), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 7, fgt->getRing(), t1, v1, t2, v2, t3, v3);
}

void GMHandler::OnForge( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	size_t sz = args.size();
	if(sz == 4 || sz == 6 || sz == 8)
	{
		UInt32 fgtId = atoi(args[0].c_str());
		GObject::Fighter * fgt = player->findFighter(fgtId);
		if(fgt == NULL)
			return;
		UInt32 pos = atoi(args[1].c_str());
		GObject::ItemEquip * equip = NULL;
		switch(pos)
		{
		case 1:
			equip = fgt->getWeapon();
			break;
		case 2:
			equip = fgt->getArmor(0);
			break;
		case 3:
			equip = fgt->getArmor(1);
			break;
		case 4:
			equip = fgt->getArmor(2);
			break;
		case 5:
			equip = fgt->getArmor(3);
			break;
		case 6:
			equip = fgt->getArmor(4);
			break;
		case 7:
			equip = fgt->getAmulet();
			break;
		case 8:
			equip = fgt->getRing();
			break;
		}
		if(equip == NULL)
			return;
		UInt8 t[3] = {0, 0, 0};
		Int16 v[3] = {0, 0, 0};
		size_t cnt = (sz - 2) >> 1;
		for(size_t i = 0; i < cnt; ++ i)
		{
			t[i] = atoi(args[2 + i * 2].c_str());
			v[i] = atoi(args[3 + i * 2].c_str());
		}
		makeExtraAttr(fgt, pos - 1, equip, t[0], v[0], t[1], v[1], t[2], v[2]);
	}
	else if(sz == 3 || sz == 5 || sz == 7)
	{
		UInt32 fgtId = atoi(args[0].c_str());
		GObject::Fighter * fgt = player->findFighter(fgtId);
		if(fgt == NULL)
			return;
		UInt8 t[3] = {0, 0, 0};
		Int16 v[3] = {0, 0, 0};
		size_t cnt = (sz - 1) >> 1;
		for(size_t i = 0; i < cnt; ++ i)
		{
			t[i] = atoi(args[1 + i * 2].c_str());
			v[i] = atoi(args[2 + i * 2].c_str());
		}
		makeFighterExtraAttr(fgt, t[0], v[0], t[1], v[1], t[2], v[2]);
	}
}

static void makeGem( const std::pair<UInt32, GObject::Fighter *>& p, UInt8 glvl, GObject::Package * package )
{
	GObject::Fighter * fgt = p.second;
	makeItemSuper(package, fgt->getWeapon(), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
	makeItemSuper(package, fgt->getArmor(0), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
	makeItemSuper(package, fgt->getArmor(1), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
	makeItemSuper(package, fgt->getArmor(2), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
	makeItemSuper(package, fgt->getArmor(3), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
	makeItemSuper(package, fgt->getArmor(4), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
	makeItemSuper(package, fgt->getAmulet(), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
	makeItemSuper(package, fgt->getRing(), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
	UInt8 pos[8] = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28};
	GObject::ItemEquip * equips[8] = {fgt->getWeapon(), fgt->getArmor(0), fgt->getArmor(1), fgt->getArmor(2), fgt->getArmor(3), fgt->getArmor(4), fgt->getAmulet(), fgt->getRing()};
	fgt->setDirty();
	fgt->sendModification(8, pos, equips, false);
}

void GMHandler::OnGem( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 1)
	{
		UInt8 glvl = atoi(args[1].c_str());
		if(glvl > 10)
			return;
		UInt32 fgtId = atoi(args[0].c_str());
		GObject::Fighter * fgt = player->findFighter(fgtId);
		if(fgt == NULL)
			return;
		GObject::Package * package = player->GetPackage();
		makeItemSuper(package, fgt->getWeapon(), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
		makeItemSuper(package, fgt->getArmor(0), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
		makeItemSuper(package, fgt->getArmor(1), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
		makeItemSuper(package, fgt->getArmor(2), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
		makeItemSuper(package, fgt->getArmor(3), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
		makeItemSuper(package, fgt->getArmor(4), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
		makeItemSuper(package, fgt->getAmulet(), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
		makeItemSuper(package, fgt->getRing(), (fgt->getClass() == 5 ? 0 : 1), 0, glvl, false);
		UInt8 pos[8] = {0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28};
		GObject::ItemEquip * equips[8] = {fgt->getWeapon(), fgt->getArmor(0), fgt->getArmor(1), fgt->getArmor(2), fgt->getArmor(3), fgt->getArmor(4), fgt->getAmulet(), fgt->getRing()};
		fgt->setDirty();
		fgt->sendModification(8, pos, equips, false);
	}
	else
	{
		UInt8 glvl = atoi(args[0].c_str());
		if(glvl > 10)
			return;
		using namespace std::placeholders;
		player->foreachFighter(std::bind(makeGem, _1, glvl, player->GetPackage()));
	}
}

static void makeEnchant( GObject::Fighter * fgt, UInt8 pos, GObject::ItemEquip * equip, UInt8 level )
{
	if(equip == NULL)
		return;
	GObject::ItemEquipData& ied = equip->getItemEquipData();
	if(ied.enchant == level)
		return;
	ied.enchant = level;
	DB().PushUpdateData("UPDATE `equipment` SET `enchant` = %u WHERE `id` = %u", ied.enchant, equip->getId());
	fgt->setDirty();
	fgt->sendModification(0x21 + pos, equip, false);
}

static void makeFighterEnchant( const std::pair<UInt32, GObject::Fighter *>& p, UInt8 level )
{
	GObject::Fighter * fgt = p.second;
	makeEnchant(fgt, 0, fgt->getWeapon(), level);
	makeEnchant(fgt, 1, fgt->getArmor(0), level);
	makeEnchant(fgt, 2, fgt->getArmor(1), level);
	makeEnchant(fgt, 3, fgt->getArmor(2), level);
	makeEnchant(fgt, 4, fgt->getArmor(3), level);
	makeEnchant(fgt, 5, fgt->getArmor(4), level);
}

void GMHandler::OnEnchant( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	if(args.size() > 2)
	{
		UInt32 enc = atoi(args[2].c_str());
		if(enc > 10)
			return;
		UInt32 fgtId = atoi(args[0].c_str());
		GObject::Fighter * fgt = player->findFighter(fgtId);
		if(fgt == NULL)
			return;
		UInt32 pos = atoi(args[1].c_str());
		GObject::ItemEquip * equip = NULL;
		switch(pos)
		{
		case 1:
			equip = fgt->getWeapon();
			break;
		case 2:
			equip = fgt->getArmor(0);
			break;
		case 3:
			equip = fgt->getArmor(1);
			break;
		case 4:
			equip = fgt->getArmor(2);
			break;
		case 5:
			equip = fgt->getArmor(3);
			break;
		case 6:
			equip = fgt->getArmor(4);
			break;
		}
		makeEnchant(fgt, pos - 1, equip, enc);
	}
	else if(args.size() > 1)
	{
		UInt8 enc = atoi(args[1].c_str());
		if(enc > 10)
			return;
		UInt32 fgtId = atoi(args[0].c_str());
		GObject::Fighter * fgt = player->findFighter(fgtId);
		if(fgt == NULL)
			return;
		makeEnchant(fgt, 0, fgt->getWeapon(), enc);
		makeEnchant(fgt, 1, fgt->getArmor(0), enc);
		makeEnchant(fgt, 2, fgt->getArmor(1), enc);
		makeEnchant(fgt, 3, fgt->getArmor(2), enc);
		makeEnchant(fgt, 4, fgt->getArmor(3), enc);
		makeEnchant(fgt, 5, fgt->getArmor(4), enc);
	}
	else
	{
		UInt8 enc = atoi(args[0].c_str());
		if(enc > 10)
			return;
		using namespace std::placeholders;
		player->foreachFighter(std::bind(makeFighterEnchant, _1, enc));
	}
}

void GMHandler::OnResetIc( GObject::Player * player, std::vector<std::string>& )
{
	player->resetIcCount();
}

void GMHandler::OnAutoCB( GObject::Player * player, std::vector<std::string>& args )
{
	UInt8 toggle = 1;
	if(!args.empty())
	{
		toggle = atoi(args[0].c_str()) != 0;
	}
	if(toggle)
		player->autoCB();
	else
		player->autoCB(false);
}

void GMHandler::OnEnterArena( GObject::Player * player, std::vector<std::string>& )
{
	GObject::Arena::enterArena(player);
}

void GMHandler::OnNextArena( GObject::Player * player, std::vector<std::string>& )
{
	Stream st(REP::NEXT_ARENA, 0x01);
	char p[128];
	sprintf(p, "/next %u", WORLD().getWeekDay());
	st << "" << p << Stream::eos;
	NETWORK()->SendToArena(st);
}

void GMHandler::OnEnterClan( GObject::Player * player, std::vector<std::string>& )
{
	(GObject::clanManager.getRobBattleClan())->enterClanCity(player);
}

void GMHandler::OnPay4Pra( GObject::Player * player, std::vector<std::string>& args)
{
    if (args.size() < 6)
        return;
    UInt8 place = atoi(args[0].c_str());
    UInt16 slot = atoi(args[1].c_str());
    UInt8 type = atoi(args[2].c_str());
    UInt8 pricetype = atoi(args[3].c_str());
    UInt8 time = atoi(args[4].c_str());
    UInt8 prot = atoi(args[5].c_str());
    player->payPractice(place, slot, type, pricetype, time, prot);
}

void GMHandler::OnSitPra( GObject::Player * player, std::vector<std::string>& args)
{
    if (!player || args.size() < 1)
        return;

    size_t cnt = 0;
    UInt32 fighters[5] = {0};
    for (size_t i = 0; i < args.size() && i < 5; ++i)
        fighters[cnt++] = atoi(args[i].c_str());

    player->addPracticeFighter(fighters, cnt);
}

void GMHandler::OnFlushTask( GObject::Player * player, std::vector<std::string>& args)
{
    if (!player || args.size() < 1)
        return;

    UInt8 ttype = atoi(args[0].c_str());
    UInt8 ftype = 0;
    UInt8 color = 0;
    UInt8 count = 1;
    if (args.size() > 1) {
        ftype = atoi(args[1].c_str());
        if (args.size() > 2) {
            color = atoi(args[2].c_str());
            if (args.size() > 3)
                count = atoi(args[3].c_str());
        }
    }
    player->flushTaskColor(ttype, ftype, color, count);
}

void GMHandler::OnSetCountry( GObject::Player * player, std::vector<std::string>& args)
{
    if (!player || args.size() < 1)
        return;
    UInt8 country = atoi(args[0].c_str());
    if (country > 2)
        return;
    if (player->getCountry() != country)
    {
        player->setCountry(country);
        PlayerData& pd = player->getPlayerData();

        GObject::Country& cny = CURRENT_COUNTRY();
        cny.PlayerLeave(player);

        CountryEnterStruct ces(true, pd.inCity ? 1 : 0, pd.location);
        GameMsgHdr hdr(0x1F0, country, player, sizeof(CountryEnterStruct));
        GLOBAL().PushMsg( hdr, &ces );
    }
}

void GMHandler::OnSetAcu( GObject::Player * player, std::vector<std::string>& args)
{
    if (!player || args.size() < 2)
        return;
    UInt32 id = atoi(args[0].c_str());
	GObject::Fighter * fgt = player->findFighter(id);
    if (!fgt)
        return;
    UInt8 idx = atoi(args[1].c_str());
    UInt8 lvl = atoi(args[2].c_str());
    fgt->setAcupoints(idx, lvl);
}

void GMHandler::OnUseItem( GObject::Player * player, std::vector<std::string>& args)
{
    if (!player || args.size() < 1)
        return;
    UInt32 itemid = atoi(args[0].c_str());
    UInt8 num = 1;
    UInt32 fgtid = 0;
    if (args.size() >= 2)
        num = atoi(args[1].c_str());
    if (args.size() >= 3)
        fgtid = atoi(args[2].c_str());
    player->GetPackage()->UseItem(itemid, num, fgtid);
}

void GMHandler::OnOcupyPlace(GObject::Player * player, std::vector<std::string>& args)
{
    if(!player || args.size() < 1)
        return;

	char * endptr;
	UInt64 playerId = strtoull(args[0].c_str(), &endptr, 10);
	if(playerId == 0)
		return;
	GObject::Player * pl = GObject::globalPlayers[playerId];
	if(pl == NULL)
		return;

    UInt8 place = atoi(args[1].c_str());

    practicePlace.ocupyPlace(pl, place);
}

void GMHandler::OnEnterCopy(GObject::Player *player, std::vector<std::string>& args)
{
    if(!player || args.size() < 1)
        return;

    UInt8 id = atoi(args[0].c_str());
    playerCopy.enter(player, id);
}


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
#include "GObject/CFriend.h"
#include "GObject/Package.h"
#include "GObject/PetPackage.h"
#include "GObject/Fighter.h"
#include "GObject/Clan.h"
#include "GObject/Country.h"
#include "GObject/ClanManager.h"
#include "GObject/ClanBattle.h"
#include "GObject/ClanManager.h"
#include "GData/DungeonData.h"
#include "GData/ExpTable.h"
#include "GData/NpcGroup.h"
#include "GData/SkillTable.h"
#include "GData/FairyPetTable.h"
#include "GObject/Dungeon.h"
#include "GObject/Arena.h"
#include "Script/GameActionLua.h"
#include "Script/ConfigScript.h"
#include "Battle/BattleSimulator.h"
#include "Common/StringTokenizer.h"
#include "CountryMsgStruct.h"
#include "GObject/PracticePlace.h"
#include "GObject/Copy.h"
#include "GObject/FrontMap.h"
#include "GObject/HeroIsland.h"
#include "GObject/TeamCopy.h"
#include "GObject/AthleticsRank.h"
#include "GObject/HeroMemo.h"
#include "MsgHandler/JsonParser.h"
#include "GObject/LuckyDraw.h"
#include "GObject/ClanCopy.h"
#include "GObject/SingleHeroStage.h"
#include "GObject/NewCountryBattle.h"
#include "GObject/Tianjie.h"
#include "GObject/ClanCopy.h"
#include "GObject/ClanBoss.h"
#include "Common/Itoa.h"
#include "GObject/TownDeamon.h"
#include "Script/BattleFormula.h"
#include "GObject/FairyPet.h"

#include "GObject/Tianjie.h"
#include "Memcached.h"
#include "Version.h"
#include "GObject/FairySpar.h"
GMHandler gmHandler;

GMHandler::GMHandler()
{

	Reg(1, "pinfo", &GMHandler::OnPlayerInfo);
	Reg(1, "cinfo", &GMHandler::OnCharInfo);
//	Reg(1, "enterarena", &GMHandler::OnEnterArena);
	Reg(1, "enterClan", &GMHandler::OnEnterClan);

	Reg(2, "holy", &GMHandler::OnHoly);
	Reg(2, "setspeed", &GMHandler::OnSetSpeed);
	Reg(2, "spd", &GMHandler::OnSetSpeed);
	Reg(2, "move", &GMHandler::OnMove);
	Reg(2, "regen", &GMHandler::OnRegen);
    Reg(2, "wa", &GMHandler::OnWorldAnnounce);

	Reg(3, "donate", &GMHandler::OnClanDonate);
	Reg(3, "banchat", &GMHandler::OnBanChat);
	Reg(3, "addexp", &GMHandler::OnAddExp);
	Reg(2, "exp", &GMHandler::OnAddExp);
	Reg(2, "pexp", &GMHandler::OnAddPExp);
	Reg(3, "addpexp", &GMHandler::OnAddPExp);
	Reg(3, "additem", &GMHandler::OnAddItem);
	Reg(3, "addclanbox", &GMHandler::OnClanBox);
	Reg(3, "item", &GMHandler::OnAddItem);
	Reg(2, "itemb", &GMHandler::OnAddItemB);
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
	Reg(2, "setvip", &GMHandler::OnSetVip);
	Reg(3, "super", &GMHandler::OnSuper);
	Reg(3, "spawn", &GMHandler::OnSpawn);
	Reg(3, "unspawn", &GMHandler::OnUnspawn);
    Reg(3, "lb", &GMHandler::OnLingbao);
    Reg(3, "lbskill", &GMHandler::OnLingbaoSkill);
    Reg(3, "lbs", &GMHandler::OnLingbaos);
    Reg(3, "testlb", &GMHandler::testLingbao);
    Reg(3, "peteq", &GMHandler::OnPetEq);



	Reg(3, "playerwallow", &GMHandler::OnPlayerWallow);
	Reg(3, "wallow", &GMHandler::OnWallow);
	Reg(3, "setgl", &GMHandler::OnSetGM);
	Reg(3, "reloadLua", &GMHandler::OnReLoadLua);
	Reg(3, "reload", &GMHandler::OnReLoadLua);
	Reg(3, "limitluckydraw", &GMHandler::OnLimitLuckyDraw);
    UInt8 getDisVarOffset(UInt8 type);
	Reg(3, "run", &GMHandler::OnRunScript);
	Reg(3, "reconn", &GMHandler::OnReconn);
	Reg(3, "settitle", &GMHandler::OnSetTitle);
	Reg(3, "title", &GMHandler::OnSetTitle);
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
	Reg(3, "newcb", &GMHandler::OnNewCountryBattle);
	Reg(3, "autofb", &GMHandler::OnAutoFB);

	Reg(3, "nextarena", &GMHandler::OnNextArena);
	Reg(3, "pay4pra", &GMHandler::OnPay4Pra);
	Reg(3, "sitpra", &GMHandler::OnSitPra);
	Reg(3, "flushtask", &GMHandler::OnFlushTask);
	Reg(3, "setcountry", &GMHandler::OnSetCountry);
	Reg(3, "setacu", &GMHandler::OnSetAcu);
	Reg(3, "useitem", &GMHandler::OnUseItem);
	Reg(3, "uitem", &GMHandler::OnUseItem);
    Reg(3, "ocupyplace", &GMHandler::OnOcupyPlace);
    Reg(3, "ec", &GMHandler::OnEnterCopy);
    Reg(3, "gmc", &GMHandler::OnGmCheck);
    Reg(3, "m2a", &GMHandler::OnMoney2All);
    Reg(3, "kick", &GMHandler::OnKick);
    Reg(3, "count", &GMHandler::OnCount);
    Reg(2, "wc", &GMHandler::OnCount);
    Reg(3, "tid", &GMHandler::OnThreadId);
    Reg(3, "ac", &GMHandler::OnAutoCopy);
    Reg(3, "lock", &GMHandler::OnLock);
    Reg(3, "unlock", &GMHandler::OnUnLock);
    Reg(3, "setbl", &GMHandler::OnSetBosslevel);
    Reg(3, "qqkey", &GMHandler::OnQQVipKey);

    Reg(3, "clanbuild", &GMHandler::OnClanBuild);
    Reg(3, "clanitem", &GMHandler::OnClanItem);
    Reg(3, "clanbattlescore", &GMHandler::OnClanBattleScore);

    Reg(3, "hiinfo", &GMHandler::OnInfoHI);
    Reg(3, "hienter", &GMHandler::OnEnterHI);
    Reg(3, "hileave", &GMHandler::OnLeaveHI);
    Reg(3, "hiattack", &GMHandler::OnAttackHI);
    Reg(3, "himove", &GMHandler::OnMoveHI);
    Reg(3, "hiid", &GMHandler::OnGetIDHI);
    Reg(3, "hitask", &GMHandler::OnTaskHI);
    Reg(3, "hici", &GMHandler::OnCiTaskHI);
    Reg(3, "hirestart", &GMHandler::OnRestartHI);
    Reg(3, "hiaward", &GMHandler::OnAwardHI);
    Reg(3, "hiuseskill", &GMHandler::OnUseSkillHI);
    Reg(3, "appearboss", &GMHandler::OnAppearBoss);
    Reg(3, "resettcplayer", &GMHandler::OnResetTeamCopyPlayer);
    Reg(3, "moneyin", &GMHandler::OnMoneyIn);
    Reg(3, "newyear", &GMHandler::OnNewYear);
    Reg(3, "ff", &GMHandler::OnFgtForge);
    Reg(3, "ghmaward", &GMHandler::OnGetHeroMemoAward);
    Reg(3, "sysdlg", &GMHandler::OnSysDailog);
    Reg(3, "regenall", &GMHandler::OnRegenAll);
    Reg(2, "bosshp", &GMHandler::OnSetBossHp);
    Reg(3, "systm", &GMHandler::OnTime);
    Reg(2, "tm", &GMHandler::OnTime);
    Reg(3, "token", &GMHandler::OnToken);
	Reg(3, "recharge", &GMHandler::OnRecharge);
	Reg(2, "boss", &GMHandler::OnBossHP);
	Reg(3, "json", &GMHandler::OnJson);
	Reg(3, "rc7awd", &GMHandler::OnRC7Awd);
	Reg(3, "rc7ton", &GMHandler::OnRC7TurnOn);
	Reg(3, "vars", &GMHandler::OnAddVarS);
	Reg(3, "ld", &GMHandler::OnLuckyDraw);

    Reg(3, "sign", &GMHandler::OnHandleSignIn);

    Reg(3, "newr", &GMHandler::OnNewRelation);
    Reg(3, "ssopen", &GMHandler::OnSSOpen);
    Reg(3, "ssup", &GMHandler::OnSSUp);
    Reg(3, "sserase", &GMHandler::OnSSErase);
    Reg(3, "sosog", &GMHandler::OnSoSoGet);

    Reg(3, "tj1", &GMHandler::OnTj1);
    Reg(3, "tj2", &GMHandler::OnTj2);
    Reg(3, "tj3", &GMHandler::OnTj3);
    Reg(3, "tj4", &GMHandler::OnTj4);
    Reg(3, "opentj", &GMHandler::OnOpenTj);
    Reg(2, "idip", &GMHandler::OnAddIdip);
    Reg(2, "clear", &GMHandler::OnClearTask);
    Reg(2, "reset", &GMHandler::OnClearCFT);

    Reg(3, "statue", &GMHandler::OnStatueExp);
    Reg(3, "setcopy", &GMHandler::OnSetClanCopyLevel);
    Reg(3, "setcopytime", &GMHandler::OnSetClanCopyTime);

    Reg(3, "entershs", &GMHandler::OnEnterSingleHeroStage);
    Reg(3, "nextshs", &GMHandler::OnSingleHeroStageNextTime);
    Reg(3, "resetshs", &GMHandler::OnSingleHeroStageReset);
    Reg(3, "gotoshs", &GMHandler::OnSingleHeroStageGoTo);
    Reg(3, "autonp", &GMHandler::OnNewPlayerAuto);
    Reg(3, "autosupper", &GMHandler::OnNewPlayerAutoSuper);

    Reg(3, "bp", &GMHandler::OnShowBattlePoint);
    Reg(3, "enterarena", &GMHandler::OnEnterArena);
    Reg(3, "idipbuy", &GMHandler::OnIdipBuy);

    Reg(3, "biglock", &GMHandler::OnBigLock);
    Reg(3, "bigunlock", &GMHandler::OnBigUnLock);
    Reg(2, "strong", &GMHandler::OnStrengthen);
   
    Reg(3, "fsale", &GMHandler::OnForbidSale);
    Reg(3, "unfsale", &GMHandler::OnUnForbidSale);
    Reg(3, "loginlimit", &GMHandler::OnSetLoginLimit);

    Reg(3, "sysup", &GMHandler::OnSysUpdate);
    Reg(2, "cfriend", &GMHandler::OnCFriend);
    Reg(2, "shuo", &GMHandler::OnShuoShuo);
    Reg(2, "gold", &GMHandler::OnSaveGoldAct);

    Reg(3, "drtm", &GMHandler::OnDreamerTimeSet);
    Reg(3, "drkey", &GMHandler::OnDreamerKeySet);
    Reg(3, "dreye", &GMHandler::OnDreamerEyeSet);

    Reg(3, "act", &GMHandler::OnSomeAct);
    Reg(2, "king", &GMHandler::OnDragonKingAct);
    Reg(2, "pet", &GMHandler::OnFairyPetGM);
    Reg(2, "fool", &GMHandler::OnFoolsDayGM);
    Reg(2, "star", &GMHandler::OnLuckyStarGM);
    Reg(2, "acttm", &GMHandler::OnSurnameleg);

    Reg(3, "opencb", &GMHandler::OnClanBossOpen);
    Reg(3, "cb", &GMHandler::OnClanBoss);
    Reg(3, "pick", &GMHandler::OnClanBossPick);
    Reg(3, "empower", &GMHandler::OnClanBossEmpower);
    Reg(3, "setem", &GMHandler::OnClanBossSetEm);

    Reg(3, "settdlvl", &GMHandler::OnSetTownDeamonMaxLevel);
    Reg(3, "spar", &GMHandler::OnFairySpar);
    
    Reg(3, "setxzlvl", &GMHandler::OnSetXZLvl);
    Reg(3, "setxzvalue", &GMHandler::OnSetXCValue);

    Reg(2, "eqexp", &GMHandler::OnAddPetEquipExp);
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

void GMHandler::OnSetXZLvl(GObject::Player * player, std::vector<std::string>& args)
{
    
	if(args.empty())
		return;
	if(args.size() == 2)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt32 xzLevel = atoi(args[1].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;
        fgt->getXingchen().lvl = xzLevel;

        fgt->updateDBxingchen();
        fgt->sendXingchenInfo(0);
	}
}

void GMHandler::OnSetXCValue(GObject::Player * player, std::vector<std::string>& args)
{

	if(args.empty())
		return;
	if(args.size() == 2)
	{
		UInt32 fighterId = atoi(args[0].c_str());
		UInt32 xcValue  = atoi(args[1].c_str());
		GObject::Fighter * fgt = player->findFighter(fighterId);
		if(fgt == NULL)
			return;
        player->SetVar(VAR_XINGCHENZHEN_VALUE, xcValue);
        
        fgt->sendXingchenInfo(0);
	}
}

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
#if 0
    ActivityMgr* mgr = player->GetActivityMgr();
    if(args.size() == 1)
    {
        int i = atoi(args[0].c_str());

        if( i == 1)
            mgr->ActivityList(7);
        else if( i == 2)
            mgr->ChangeOnlineReward();

    }
    else if(args.size() == 2)
    {
        int i = atoi(args[0].c_str());
         mgr->GetReward(i);
    }
    return;
    //only for test liuhuiting
    //
    for (int i = 10001; i<= 11001; i ++)
    {
        for (int j = 1; j < 10 ; j ++)
        {
         player->OnDoAttainment(i , j);
        }

        for (int j = 10; j<= 80; j= j + 10)
        {
             player->OnDoAttainment(i ,j);
        }
         player->OnDoAttainment(i , 500);
    }
#endif
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
        else if(IsPetEquipTypeId(itemId))
		{
			player->GetPetPackage()->AddRandomPetEq(0, itemId);
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
        else if(IsPetEquipTypeId(itemId))
		{
			player->GetPetPackage()->AddRandomPetEq(0, itemId);
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

                if (args.size() > 2) {
                    Player* player2 = globalPlayers[atoi(args[2].c_str())];
                    if (player2)
                        player = player2;
                }
				player->getTael(val);
			}
			break;
		case 2:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;

                if (args.size() > 2) {
                    Player* player2 = globalPlayers[atoi(args[2].c_str())];
                    if (player2)
                        player = player2;
                }
                IncommingInfo ii(InFromAddMoney, 0, 0);
                player->getGold(val, &ii);

                {
                    char gold[32] = {0};
                    snprintf(gold, 32, "%u", val);
                    player->udpLog("free", gold, "", "", "", "", "currency");
                }
			}
			break;
		case 3:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;

                if (args.size() > 2) {
                    Player* player2 = globalPlayers[atoi(args[2].c_str())];
                    if (player2)
                        player = player2;
                }
				player->getCoupon(val);
			}
			break;
		case 4:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;

                if (args.size() > 2) {
                    Player* player2 = globalPlayers[atoi(args[2].c_str())];
                    if (player2)
                        player = player2;
                }
				player->getAchievement(val);
			}
            break;

        case 5:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;

                if (args.size() > 2) {
                    Player* player2 = globalPlayers[atoi(args[2].c_str())];
                    if (player2)
                        player = player2;
                }
				player->getPrestige(val);
            }
            break;

        case 11:
			{
				UInt32 val = atoi(args[1].c_str());
				if(val == 0)
					return;
				player->getMoneyArena(val);
            }
            break;

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
	if(level > 15)
		return;
	const static UInt32 gold[] = {88,888,1888,5888,18888,58888,88888,188888,288888,588888,988888,1888888,2888888,5888888,8888888};
    if (args.size() > 1)
        player = globalPlayers[atoll(args[1].c_str())];
    if (!player)
        return;
	player->setTotalRecharge(gold[level]);
}

void GMHandler::OnTopup( GObject::Player * player, std::vector<std::string>& args )
{
	if(args.empty())
		return;
	UInt32 val = atoi(args[0].c_str());
    IncommingInfo ii(InFromTopUp, 0, 0);
	player->getGold(val, &ii);
    {
        char gold[32] = {0};
        snprintf(gold, 32, "%u", val);
        player->udpLog("free", gold, "", "", "", "", "currency");
    }
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
		GameMsgHdr hdr4(0x1EE, WORKER_THREAD_WORLD, NULL, sizeof(UInt16));
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
	DB1().PushUpdateData("UPDATE `player` SET `name` = '%s' WHERE id = %"I64_FMT"u", nameTmp.c_str(), player->getId());
	DBLOG1().PushUpdateData("update `register_states` set `player_name` = '%s' where `server_id` = %u and `player_id` = %"I64_FMT"u", nameTmp.c_str(), cfg.serverLogId, player->getId());
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
	SYSMSG_SENDV(609, player, PLAYER_DATA(pl, country));
	SYSMSG_SENDV(612, player, pl->isOnline()?"YES":"NO");
	SYSMSG_SENDV(613, player, PLAYER_DATA(pl, copyFreeCnt), PLAYER_DATA(pl, copyGoldCnt));
	SYSMSG_SENDV(614, player, PLAYER_DATA(pl, frontFreeCnt), PLAYER_DATA(pl, frontGoldCnt));
	SYSMSG_SENDV(622, player, pl->getThreadId());
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

void makeItemSuper( GObject::Package * package, GObject::ItemEquip * equip, UInt8 type, UInt8 enchant = 8, UInt8 level = 10, bool flushAttr = true )
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
	DB4().PushUpdateData("UPDATE `equipment` SET `enchant` = %u, `sockets` = %u, `socket1` = %u, `socket2` = %u, `socket3` = %u, `socket4` = %u, `socket5` = %u, `socket6` = %u, `attrType1` = %u, `attrValue1` = %d, `attrType2` = %u, `attrValue2` = %d, `attrType3` = %u, `attrValue3` = %d WHERE `id` = %u",
		ied.enchant, ied.sockets, ied.gems[0], ied.gems[1], ied.gems[2], ied.gems[3], ied.gems[4], ied.gems[5], ied.extraAttr2.type1, ied.extraAttr2.value1, ied.extraAttr2.type2, ied.extraAttr2.value2, ied.extraAttr2.type3, ied.extraAttr2.value3, equip->getId());
}

void makeSuper( GObject::Fighter * fgt, UInt8 equipLvl = 100, UInt8 enchant = 8, UInt8 gemlevel = 10, bool flushAttr = true )
{
	if(fgt == NULL)
		return;
	GObject::Player * player = fgt->getOwner();
	if(player == NULL)
		return;
	const UInt32 itemIdStart[3][5] = {
        {2568, 2592, 2784, 2784, 2784},
        {2576, 2600, 2792, 2792, 2792},
        {2584, 2608, 2800, 2800, 2800}
    };
    const UInt16 trump[] = {1608,1609,1610};
	int idx = -1;
	if(equipLvl <= 60)
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
	if(flushAttr) {
		fgt->setPotential(1.79f);
        fgt->setCapacity(8);
    }
	GObject::Package * package = player->GetPackage();
	GObject::ItemEquip * o = NULL;
	GObject::ItemWeapon * weapon;
	GObject::ItemArmor * armor;
	GObject::ItemEquip * equip;
	switch(fgt->getClass())
	{
	case e_cls_ru:
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
        equip = static_cast<GObject::ItemEquip *>(package->AddEquip(trump[0], false, true));
        if(equip)
        {
            makeItemSuper(package, equip, 0, 9, 0, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x0a, o);
        }
		break;
	case e_cls_shi:
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
        equip = static_cast<GObject::ItemEquip *>(package->AddEquip(trump[1], false, true));
        if(equip)
        {
            makeItemSuper(package, equip, 0, 9, 0, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x0a, o);
        }
		break;
	case e_cls_dao:
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
        equip = static_cast<GObject::ItemEquip *>(package->AddEquip(trump[2], false, true));
        if(equip)
        {
            makeItemSuper(package, equip, 0, 9, 0, flushAttr);
            package->EquipTo(equip->getId(), fgt, 0x0a, o);
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
	makeSuper(fgt, fgt->getLevel());
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
		DB1().PushUpdateData("UPDATE `player` SET `wallow`=%u WHERE `id`=%"I64_FMT"u", type, player->getId());
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
    IncommingInfo ii(InFromSuper, 0, 0);
    player->getGold(10000000, &ii);
    {
        char gold[32] = {0};
        snprintf(gold, 32, "%u", 10000000);
        player->udpLog("free", gold, "", "", "", "", "currency");
    }
    player->getTael(10000000);
	makeSuper(player->getMainFighter(), player->GetLev());
	addSuperClass(player, 10);
	addSuperClass(player, 16);
    addSuperClass(player, 13);
    addSuperClass(player, 15);
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
	DB1().Pause();
	DB2().Pause();
	DB3().Pause();
	DB4().Pause();
	DB5().Pause();
	DB6().Pause();
	DBLOG().Pause();
	DBLOG1().Pause();
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
		//SYSMSG_SENDV(610, player, dd->getName().c_str(), 0, dungeonLevel + 1, npcCount, win, static_cast<float>(win * 10000 / npcCount) / 100);
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
		//SYSMSG_SENDV(611, player, ng->getName().c_str(), npcCount, win, static_cast<float>(win * 10000 / npcCount) / 100);
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
				Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, pl1, pl2);
				pl1->PutFighters( bsim, 0, true );
				pl2->PutFighters( bsim, 1, true );
				bsim.start();
				if(bsim.getWinner() == 1) ++ win;
			}
			SYSMSG_SENDV(612, player, pl1->getName().c_str(), pl2->getName().c_str(), count, win, static_cast<float>(win * 10000 / count) / 100);
			return;
		}
	}
	Battle::BattleSimulator bsim(Battle::BS_ATHLETICS1, pl1, pl2);
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
        if (args.size() >= 3)
        {
            GObject::Player * pl = GObject::globalPlayers[atoi(args[2].c_str())];
            if (!pl)
                return;
            UInt32 fighterId = atoi(args[0].c_str());
            if (!fighterId)
            {
                UInt8 lvl = atoi(args[1].c_str());
                UInt64 exp = GData::expTable.getLevelMin(lvl);
                pl->setLevelAndExp(lvl, exp);
                pl->SetVar(VAR_LEVEL_AWARD, (lvl == 0 ? 1 : lvl)-1);
            }
            else
            {
                GObject::Fighter * fgt = pl->findFighter(fighterId);
                if(fgt == NULL)
                    return;
                UInt8 lvl = atoi(args[1].c_str());
                UInt64 exp = GData::expTable.getLevelMin(lvl);
                fgt->setLevelAndExp(lvl, exp);
            }
            return;
        }

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
        player->SetVar(VAR_LEVEL_AWARD, (lvl == 0 ? 1 : lvl)-1);
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
	DB3().PushUpdateData("UPDATE `equipment` SET `attrType1` = %u, `attrValue1` = %d, `attrType2` = %u, `attrValue2` = %d, `attrType3` = %u, `attrValue3` = %d WHERE `id` = %u",
		ea2.type1, ea2.value1, ea2.type2, ea2.value2, ea2.type3, ea2.value3, equip->getId());
	fgt->sendModification(0x20 + pos, equip, false);
}

static void makeFighterExtraAttr( GObject::Fighter * fgt, UInt8 t1, Int16 v1, UInt8 t2, Int16 v2, UInt8 t3, Int16 v3 )
{
	makeExtraAttr(fgt, 0, fgt->getFashion(), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 1, fgt->getWeapon(), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 2, fgt->getArmor(0), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 3, fgt->getArmor(1), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 4, fgt->getArmor(2), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 5, fgt->getArmor(3), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 6, fgt->getArmor(4), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 7, fgt->getAmulet(), t1, v1, t2, v2, t3, v3);
	makeExtraAttr(fgt, 8, fgt->getRing(), t1, v1, t2, v2, t3, v3);
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
		case 0:
			equip = fgt->getFashion();
            break;
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
		makeExtraAttr(fgt, pos, equip, t[0], v[0], t[1], v[1], t[2], v[2]);
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
	DB3().PushUpdateData("UPDATE `equipment` SET `enchant` = %u WHERE `id` = %u", ied.enchant, equip->getId());
	fgt->setDirty();
	fgt->sendModification(0x20 + pos, equip, false);
}

static void makeFighterEnchant( const std::pair<UInt32, GObject::Fighter *>& p, UInt8 level )
{
	GObject::Fighter * fgt = p.second;
	makeEnchant(fgt, 1, fgt->getWeapon(), level);
	makeEnchant(fgt, 2, fgt->getArmor(0), level);
	makeEnchant(fgt, 3, fgt->getArmor(1), level);
	makeEnchant(fgt, 4, fgt->getArmor(2), level);
	makeEnchant(fgt, 5, fgt->getArmor(3), level);
	makeEnchant(fgt, 6, fgt->getArmor(4), level);
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
		makeEnchant(fgt, pos, equip, enc);
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
		makeEnchant(fgt, 1, fgt->getWeapon(), enc);
		makeEnchant(fgt, 2, fgt->getArmor(0), enc);
		makeEnchant(fgt, 3, fgt->getArmor(1), enc);
		makeEnchant(fgt, 4, fgt->getArmor(2), enc);
		makeEnchant(fgt, 5, fgt->getArmor(3), enc);
		makeEnchant(fgt, 6, fgt->getArmor(4), enc);
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

void GMHandler::OnAutoFB( GObject::Player * player, std::vector<std::string>& args )
{
    UInt8 id = 0;
    UInt8 type = 0;
    id = atoi(args[0].c_str());
    type = atoi(args[1].c_str());
    frontMap.autoBattle(player, id, type, false);
}

#if 0
void GMHandler::OnEnterArena( GObject::Player * player, std::vector<std::string>& )
{
	GObject::arena.enterArena(player);
}
#endif

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

void GMHandler::OnUseItemOther( GObject::Player * player, std::vector<std::string>& args)
{
    if (!player || args.size() < 2)
        return;
    UInt32 itemid = atoi(args[0].c_str());
    UInt8 num = 1;
    if (args.size() >= 3)
        num = atoi(args[1].c_str());
    player->GetPackage()->UseItemOther(itemid, num, args[1]);
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
    practicePlace.moveAllToMax(place);
    practicePlace.replaceOwner(pl, place);
}

void GMHandler::OnEnterCopy(GObject::Player *player, std::vector<std::string>& args)
{
    if(!player || args.size() < 1)
        return;

    UInt8 id = atoi(args[0].c_str());
    playerCopy.enter(player, id);
}

void GMHandler::OnWorldAnnounce(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player || args.size() < 1)
        return;

	Stream st(REP::SYSTEM_INFO);
	st << static_cast<UInt8>(0x16) << args[0] << Stream::eos;
	NETWORK()->Broadcast(st);
}

void GMHandler::OnGmCheck(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;

    cfg.GMCheck = true;
    if (args.size() > 1)
        cfg.GMCheck = atoi(args[0].c_str());
}

inline bool give_money(Player * p, UInt32* money)
{
    UInt32 lvl = *(UInt32*)(money);
    UInt32* moneys = (UInt32*)(money+1);

    if (p && p->GetLev() >= lvl)
    {
        if (moneys[0])
        {
            IncommingInfo ii(InFromMoney2All, 0, 0);
            p->getGold(moneys[0], &ii);
            {
                char gold[32] = {0};
                snprintf(gold, 32, "%u", moneys[0]);
                p->udpLog("free", gold, "", "", "", "", "currency");
            }
        }
        if (moneys[1])
            p->getTael(moneys[1]);
        if (moneys[2])
            p->getCoupon(moneys[2]);
    }
    return true;
}

void GMHandler::OnMoney2All(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;

    if (args.size() <= 1)
        return;

    UInt32 gold = 1000;
    UInt32 tael = 0;
    UInt32 ticket = 0;

    UInt8 lvl = atoi(args[0].c_str());
    //if (lvl < 30)
    //    lvl = 60;

    if (args.size() >= 2)
        gold = atoi(args[1].c_str());
    if (args.size() >= 3)
        tael = atoi(args[2].c_str());
    if (args.size() >= 4)
        ticket = atoi(args[3].c_str());

    UInt32 moneys[] = {lvl, gold, tael, ticket};
    globalPlayers.enumerate(give_money, (UInt32*)moneys);
}

void GMHandler::OnKick(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player || args.size() < 1)
        return;

    UInt64 playerId = atol(args[0].c_str());
	GObject::Player * pl= GObject::globalPlayers[playerId];
    if (pl)
    {
        TcpConnection conn = NETWORK()->GetConn(pl->GetSessionID());
        if (conn)
        {
            Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
            if (cl)
            {
                pl->SetSessionID(-1);
                pl->testBattlePunish();
                pl->setOnline(false);
                static UInt8 kick_pkt[4] = {0x00, 0x00, 0xFF, REP::RECONNECT};
                cl->send(kick_pkt, 4);
                cl->SetPlayer(NULL);
                cl->pendClose();
            }
        }
    }
}

void GMHandler::OnCount(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;
	SYSMSG_SENDV(620, player, SERVER().GetTcpService()->getOnlineNum());
}

void GMHandler::OnThreadId(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;

    if (args.size() >= 2)
    {
        UInt64 playerId = atol(args[0].c_str());
        GObject::Player * pl= GObject::globalPlayers[playerId];

        UInt8 threadId = atoi(args[1].c_str());
        if (threadId == 0xFF)
            threadId = 2;
        CURRENT_COUNTRY().PlayerLeave(pl);
        pl->setThreadId(threadId);

        PlayerData& pd = pl->getPlayerData();
        CountryEnterStruct ces(true, pd.inCity ? 1 : 0, pd.location);
        GameMsgHdr hdr(0x1F0, threadId, player, sizeof(CountryEnterStruct));
        GLOBAL().PushMsg(hdr, &ces);
    }
}

void GMHandler::OnAutoCopy(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;

    if (args.size() >= 2)
    {
        UInt8 id = atoi(args[0].c_str());
        UInt8 type = atoi(args[1].c_str());
        playerCopy.autoBattle(player, id, type);
    }
}

void GMHandler::OnLock(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;

    if (args.size() < 1)
        return;

    UInt64 playerId = atoll(args[0].c_str());
    UInt64 expireTime = 0;
    if (args.size() >= 2)
        expireTime = atoi(args[1].c_str()) * 60 * 60 + TimeUtil::Now();
    GObject::Player * pl= GObject::globalPlayers[playerId];
    if(pl==NULL)
        return;

    if(pl->getLockExpireTime() == static_cast<UInt32>(0))
    {
        pl->setLockExpireTime(static_cast<UInt32>(expireTime));
        DB1().PushUpdateData("REPLACE INTO `locked_player`(`player_id`, `lockExpireTime`) VALUES(%"I64_FMT"u, %u)", playerId, expireTime);
        if(pl->isOnline())
        {
            TcpConnection conn = NETWORK()->GetConn(pl->GetSessionID());
            if(conn.get() == NULL)
                return;
            Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
            cl->closeConn();
        }
    }
}

void GMHandler::OnUnLock(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;

    if (args.size() < 1)
        return;

    UInt64 playerId = atoll(args[0].c_str());
    GObject::Player * pl= GObject::globalPlayers[playerId];
    if(pl==NULL)
        return;

    if(pl->getLockExpireTime() != static_cast<UInt32>(0))
    {
        pl->setLockExpireTime(0);
        DB1().PushUpdateData("DELETE FROM `locked_player` WHERE `player_id` = %"I64_FMT"u", pl->getId());
    }
}

void GMHandler::OnSetBosslevel(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;
    if (args.size() < 1)
        return;
    //GObject::worldBoss.setBossLevel(atoi(args[0].c_str()));
}

void GMHandler::OnQQVipKey(GObject::Player *player, std::vector<std::string>& args)
{
    if (!player)
        return;

    struct Key
    {
        UInt16 len;
        char key[128];
    } key;

    if (args.size() >= 1)
        key.len = snprintf(key.key, 128, args[0].c_str());
    else
        key.len = snprintf(key.key, 128, "1-hello");
	GameMsgHdr hdr(0x2D, player->getThreadId(), player, key.len+sizeof(UInt16));
	GLOBAL().PushMsg(hdr, &key);
}

void GMHandler::OnClanBuild(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
	if(args.size() == 1)
	{
		UInt32 val = atoi(args[0].c_str());
		if(val == 0)
			return;

		player->AddClanContrib(val);
		player->AddClanBuilding(val);
	}
}

void GMHandler::OnClanItem(GObject::Player *player, std::vector<std::string>& args)
{
    if(args.size() < 1) return;

    UInt16 itemId = 0;
    UInt32 itemNum = 0;

    itemId = (UInt16)atoi(args[0].c_str());
    if(args.size() > 1)
    {
        itemNum = (UInt32)atoi(args[1].c_str());
    }
    if(player->getClan() != NULL)
    {
        player->getClan()->AddItem(itemId, itemNum);
    }
}

void GMHandler::OnClanBattleScore(GObject::Player *player, std::vector<std::string>& args)
{
    if(args.size() < 1) return;

    UInt32 score = (UInt32)atoi(args[0].c_str());
    if(player->getClan() != NULL)
    {
        player->getClan()->SetBattleScore(score);
    }
}

void GMHandler::OnInfoHI(GObject::Player *player, std::vector<std::string>& args)
{
    heroIsland.playerInfo(player);
}

void GMHandler::OnEnterHI(GObject::Player *player, std::vector<std::string>& args)
{
    heroIsland.playerEnter(player);
}

void GMHandler::OnLeaveHI(GObject::Player *player, std::vector<std::string>&)
{
    heroIsland.playerLeave(player);
}

void GMHandler::OnAttackHI(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 2)
		return;
    heroIsland.attack(player, atoi(args[0].c_str()), atoi(args[1].c_str()));
}

void GMHandler::OnMoveHI(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    heroIsland.moveTo(player, atoi(args[0].c_str()));
}

void GMHandler::OnGetIDHI(GObject::Player *player, std::vector<std::string>& args)
{
    heroIsland.getIdentity(player);
}

void GMHandler::OnTaskHI(GObject::Player *player, std::vector<std::string>& args)
{
    heroIsland.startCompass(player);
}

void GMHandler::OnCiTaskHI(GObject::Player *player, std::vector<std::string>& args)
{
    heroIsland.commitCompass(player);
}

void GMHandler::OnRestartHI(GObject::Player *player, std::vector<std::string>& args)
{
    heroIsland.restart(TimeUtil::Now());
}

void GMHandler::OnAwardHI(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    heroIsland.getAward(player, atoi(args[0].c_str()), 1);
}

void GMHandler::OnUseSkillHI(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    heroIsland.useSkill(player, atoi(args[0].c_str()), 1);
}


void GMHandler::OnAppearBoss(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    if (args.size() >= 2)
        worldBoss.bossAppear(atoi(args[0].c_str()), atoi(args[1].c_str()));
    else
        worldBoss.bossAppear(atoi(args[0].c_str()));
}

void GMHandler::OnResetTeamCopyPlayer(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 1)
    {
        TeamCopyPlayerInfo* tcpInfo = player->getTeamCopyPlayerInfo();
        if(tcpInfo)
            tcpInfo->resetTCPlayer();
        return;
    }
    else
    {
        UInt64 playerId = atoll(args[0].c_str());
        GObject::Player * pl = GObject::globalPlayers[playerId];
        if(pl == NULL)
            return;

        TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
        if(tcpInfo)
            tcpInfo->resetTCPlayer();
    }
}

void GMHandler::OnMoneyIn(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        player->moneyLog(1, 1);
    else
        player->moneyLog(atoi(args[0].c_str()), 1);
}

void GMHandler::OnNewYear(GObject::Player* player, std::vector<std::string>& args)
{
    World::_newyear = World::_newyear?false:true;
    World::_rechargeactive = World::_rechargeactive?false:true;
	Stream st(REP::DAILY_DATA);
    World::makeActivityInfo(st);
	NETWORK()->Broadcast(st);
}

void GMHandler::OnFgtForge(GObject::Player* player, std::vector<std::string>& args)
{
    if (!args.size())
        return;
    UInt8 lock = 0x7;
    if (args.size() < 2)
        lock = 0;
    else
        lock = atoi(args[1].c_str());
	UInt32 id = atoi(args[0].c_str());
	GObject::Fighter * fgt = player->findFighter(id);
    if (!fgt)
        return;
    fgt->forge(0, lock);
}

void GMHandler::OnGetHeroMemoAward(GObject::Player* player, std::vector<std::string>& args)
{
    if (!args.size())
        return;
    UInt8 idx = atoi(args[0].c_str());
    player->GetHeroMemo()->getAward(idx);
}

inline bool player_enum(GObject::Player* p, int)
{
    if (!p->isOnline())
    {
        p->setSysDailog(true);
    }
    else
    {
        if(World::getSysDailogPlatform() == SYS_DIALOG_ALL_PLATFORM || World::getSysDailogPlatform() == atoi(p->getDomain()))
        {
            Stream st(REP::SYSDAILOG);
            st << Stream::eos;
            p->send(st);
        }
        else
        {
            p->setSysDailog(true);
        }
    }
    return true;
}

void GMHandler::OnSysDailog(GObject::Player* player, std::vector<std::string>& args)
{
    if (!args.size())
        return;
    UInt8 platform = atoi(args[0].c_str());
    World::setSysDailogPlatform(platform);
    GObject::globalPlayers.enumerate(player_enum, 0);
}
void GMHandler::OnRegenAll(GObject::Player* player, std::vector<std::string>& args)
{
    player->regenAll(true);
}
void GMHandler::OnSetBossHp(GObject::Player* player, std::vector<std::string>& args)
{
    if (!args.size())
        return;
    GObject::worldBoss.setHP(atoi(args[0].c_str()));
}
void GMHandler::OnTime(GObject::Player* player, std::vector<std::string>& args)
{
    time_t curtime1 = time(NULL) + 30;
    struct tm *local = localtime(&curtime1);
    SYSMSG_SENDV(2350, player, 1900+local->tm_year, 1+local->tm_mon, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec, World::_wday);
}
void GMHandler::OnToken(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() > 0)
        player->AddVar(VAR_GOLD_TOKEN, atoi(args[0].c_str()));
    if (args.size() > 1)
        player->AddVar(VAR_TAEL_TOKEN, atoi(args[1].c_str()));
    if (args.size() > 2)
        player->AddVar(VAR_COIN_TOKEN, atoi(args[2].c_str()));
    player->sendTokenInfo();
}
void GMHandler::OnRecharge(GObject::Player* player, std::vector<std::string>& args)
{
    UInt32 money = 0;
	LoginMsgHdr hdr(0x105, WORKER_THREAD_LOGIN, 0, player->GetSessionID(), sizeof(money));
	GLOBAL().PushMsg(hdr, &money);
}
void GMHandler::OnBossHP(GObject::Player* player, std::vector<std::string>& args)
{
	SYSMSG_SENDV(735, player, GObject::worldBoss.getHP());
}
void GMHandler::OnJson(GObject::Player* player, std::vector<std::string>& args)
{
#if !defined(_FB) && !defined(_VT)
    UInt64 begin = TimeUtil::GetTick();
    //std::string json = "{\"head\": {\"uiPacketLen\":100,\"uiCmdid\":\"1\",\"uiSeqid\":1,\"szServiceName\":\"IDIP\",\"uiSendTime\": 20110820,\"uiVersion\":1001,\"ucAuthenticate\":\"\",\"iResult\":0,\" szRetErrMsg\":\"\"},\"body\":{\"szOpenId\":\"100001\",\" uiAreaId\":0,\"playerId\":1111}}";
    //const char* json = "                                       {\"head\": {\"uiPacketLen\":100,\"uiCmdid\":\"5\",\"uiSeqid\":1,\"szServiceName\":\"IDIP\",\"uiSendTime\": 20110820,\"uiVersion\":1001,\"ucAuthenticate\":\"\",\"iResult\":0,\" szRetErrMsg\":\"\"},\"body\":{\"szOpenId\":\"100001\",\" uiAreaId\":0,\"playerId\":1111,\"iNum\":1,\"uiItemId\":507}}";
    //const char* json = "                                       {\"head\": {\"uiPacketLen\":100,\"uiCmdid\":27,\"uiSeqid\":1,\"szServiceName\":\"IDIP\",\"uiSendTime\": 20121008,\"uiVersion\":1001,\"ucAuthenticate\":\"\",\"iResult\":0,\" szRetErrMsg\":\"\"},\"body\":{\"uiAreaId\":0,\"ucType\":3}}";
    //const char* json = "                                       {\"head\": {\"uiPacketLen\":100,\"uiCmdid\":63,\"uiSeqid\":1,\"szServiceName\":\"IDIP\",\"uiSendTime\": 20121008,\"uiVersion\":1001,\"ucAuthenticate\":\"\",\"iResult\":0,\" szRetErrMsg\":\"\"},\"body\":{\"uiAreaId\":0}}";
    const char* json = "                                       {\"head\": {\"uiPacketLen\":100,\"uiCmdid\":109,\"uiSeqid\":1,\"szServiceName\":\"IDIP\",\"uiSendTime\": 20121114,\"uiVersion\":1001,\"ucAuthenticate\":\"\",\"iResult\":0,\" szRetErrMsg\":\"\"},\"body\":{\"szOpenId\":\"1\",\"playerId\":\"1\",\"uiAreaId\":0}}";
    Stream st(0);
    for (UInt16 i = 0; i < 1; ++i)
        jsonParser2((void*)json, strlen(json), st);
    UInt64 end = TimeUtil::GetTick();
    fprintf(stderr, "total secs: %.2f\n", (float)(end-begin)/1000000);
#endif
}
void GMHandler::OnRC7Awd(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt8 idx = 0;
    if (args.size() == 2)
        idx = atoi(args[1].c_str());
    player->getContinuousReward(atoi(args[0].c_str()), idx);
}
void GMHandler::OnRC7TurnOn(GObject::Player* player, std::vector<std::string>& args)
{
    player->turnOnRC7Day();
}
void GMHandler::OnAddVarS(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    int v = atoi(args[0].c_str());
    player->AddVarS(VAR_LDPOINT, v);
	SYSMSG_SENDV(9999, player, player->GetVarS(VAR_LDPOINT));
}
void GMHandler::OnLuckyDraw(GObject::Player* player, std::vector<std::string>& args)
{
    UInt8 id = 1;
    UInt8 num = 1;
    bool bind = false;
    if (args.size() >= 1)
        id = atoi(args[0].c_str());
    if (args.size() >= 2)
        num = atoi(args[1].c_str());
    if (args.size() >= 3)
        bind = atoi(args[2].c_str());

    luckyDraw.draw(player, id, num, bind);
}
void GMHandler::OnNewRelation(GObject::Player* player, std::vector<std::string>& args)
{
    UInt8 type = 0;
    UInt8 start = 0;
    UInt8 cnt = 1;
    if(args.size() >= 1)
        type = atoi(args[0].c_str());

    if(type == 5)
    {
        /*
        std::string responderName("70603");
        if(args.size() >= 2)
            responderName = args[1];
        player->GetNewRelation()->challengeRequest(player, responderName);
        */
        player->copyFrontWinAward(1);
    }
    else if(type == 6)
    {
        /*
        std::string senderName("70603");
        if(args.size() >= 2)
            senderName = args[1];
        UInt8 accept = 1;
        if(args.size() >= 3)
            accept = atoi(args[2].c_str());
        player->GetNewRelation()->challengeRespond(player, senderName, accept);
        */
        player->copyFrontWinAward(2);
    }
    else if(type == 7)
    {
        GObject::gAthleticsRank.requestAthleticsList(player, 0x40);
        return;
    }
    else if(type == 8)
    {
        GObject::gAthleticsRank.requestAthleticsList(player, 0x80);
        return;
    }

    if (args.size() >= 2)
        start = atoi(args[1].c_str());
    if (args.size() >= 3)
        cnt = atoi(args[2].c_str());
    if(type > 4)
        return;
    if(type == 4)
    {
        GObject::Clan *clan = player->getClan();
        if(clan != NULL)
            clan->sendClanList(player, type, start, cnt);
    }
    else
        player->sendFriendList(type, start, cnt);
}

void GMHandler::OnSSOpen(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 3)
        return;

    UInt32 fgtid = atoi(args[0].c_str());
    UInt32 skillid = atoi(args[1].c_str());

    Fighter* fgt = player->findFighter(fgtid);
    if (!fgt)
        return;
    fgt->SSOpen(skillid);
}

void GMHandler::OnSSUp(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 3)
        return;

    UInt32 fgtid = atoi(args[0].c_str());
    UInt32 skillid = atoi(args[1].c_str());
    UInt32 itemid = atoi(args[2].c_str());

    bool bind = false;
    if (args.size() >= 4)
        bind = atoi(args[3].c_str());

    Fighter* fgt = player->findFighter(fgtid);
    if (!fgt)
        return;
    fgt->SSUpgrade(skillid, itemid, bind);
}

void GMHandler::OnSSErase(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 2)
        return;

    UInt32 fgtid = atoi(args[0].c_str());
    UInt32 skillid = atoi(args[1].c_str());

    Fighter* fgt = player->findFighter(fgtid);
    if (!fgt)
        return;
    fgt->SSErase(skillid);
}
void GMHandler::OnSoSoGet(GObject::Player* player, std::vector<std::string>& args)
{
    player->getSoSoMapAward();
}


void GMHandler::OnHandleSignIn(GObject::Player* player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
	if(args.size() == 1)
        return;
	else
	{
		ActivityMgr* mgr = player->GetActivityMgr();
        switch(atoi(args[0].c_str()))
		{
		case 1:
			{
				UInt32 val = atoi(args[1].c_str());
                if(val == 0)
                    return;
                mgr->AddScores(val);
                mgr->UpdateToDB();
            }
            break;
        case 2:
            {
				UInt16 val = atoi(args[1].c_str());
                if(val < 0)
                    return;
                UInt8 day = TimeUtil::MonthDay();
                mgr->SetOneDayRecord(day, val);
                mgr->UpdateToDB();
            }
            break;
        }
    }
}

void GMHandler::OnTj1(GObject::Player* player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    int npcCount = atoi(args[0].c_str());
    GObject::Tianjie::instance().setTj1Count(npcCount);
}
void GMHandler::OnTj2(GObject::Player* player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    int scoreCount = atoi(args[0].c_str());
    GObject::Tianjie::instance().setTj2Count(scoreCount);
}
void GMHandler::OnAddIdip(GObject::Player * player, std::vector<std::string>& args)
{
    player->IDIPAddItem(503, 1, true);
}

void GMHandler::OnClearTask(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt8 type = atoi(args[0].c_str());
    GameMsgHdr msg(0x325, player->getThreadId(), player, sizeof(type));
    GLOBAL().PushMsg(msg, &type);
}

void GMHandler::OnClearCFT(GObject::Player* player, std::vector<std::string>& args)
{
    PLAYER_DATA(player, frontUpdate) = TimeUtil::Now();
    PLAYER_DATA(player, copyUpdate) = TimeUtil::Now();
    PLAYER_DATA(player, copyFreeCnt) = 0;
    PLAYER_DATA(player, copyGoldCnt) = 0;
    PLAYER_DATA(player, frontFreeCnt) = 0;
    PLAYER_DATA(player, frontGoldCnt) = 0;
    PLAYER_DATA(player, dungeonCnt) = 0;
    DB1().PushUpdateData("UPDATE `player` SET `frontFreeCnt` = 0, `frontGoldCnt` = 0, `frontUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), player->getId());
    DB1().PushUpdateData("UPDATE `player` SET `copyFreeCnt` = 0, `copyGoldCnt` = 0, `copyUpdate` = %u WHERE `id` = %"I64_FMT"u", TimeUtil::Now(), player->getId());
	DB1().PushUpdateData("UPDATE `player` SET `dungeonCnt` = 0 where `id` = %"I64_FMT"u", player->getId());
    player->sendDailyInfo();
}

void GMHandler::OnStatueExp(GObject::Player* player, std::vector<std::string>& args)
{
    if(args.empty())
        return;
    if(args.size() == 1)
    {
        Int32 exp = atoi(args[0].c_str());
        if(exp >= 0)
            player->AddStatueExp(exp);
        else
            player->SubStatueExp(-exp);

    }
}

void GMHandler::OnSetClanCopyLevel(GObject::Player* player, std::vector<std::string>& args)
{
    if(args.empty())
        return;
    if(args.size() == 1)
    {
        UInt32 level = atoi(args[0].c_str());

        player->setClanCopyLevel(level);

    }
}

void GMHandler::OnSetClanCopyTime(GObject::Player* player, std::vector<std::string>& args)
{
    if(args.empty())
        return;
    if (args.size() == 1)
    {
        Int32 copyTime = atoi(args[0].c_str());
        if(copyTime < 0)
            return;
        else
            player->setClanCopyTime(static_cast<UInt32>(copyTime));
    }
}

void GMHandler::OnTj3(GObject::Player* player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    int tlzLevelCount = atoi(args[0].c_str());
    GObject::Tianjie::instance().setTj3Count(tlzLevelCount);
}

void GMHandler::OnTj4(GObject::Player* player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    int bossMaxHp = atoi(args[0].c_str());
    GObject::Tianjie::instance().setTj4BossHp(bossMaxHp);
}
void GMHandler::OnOpenTj(GObject::Player* player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    int level = atoi(args[0].c_str());
    bool force = 0;
    if (args.size() >= 2)
        force = atoi(args[1].c_str());

    GObject::Tianjie::instance().manualOpenTj(level, force);
}

void GMHandler::OnEnterSingleHeroStage(GObject::Player* player, std::vector<std::string>& arge)
{
    UInt32 cnt = 0;
	if(arge.size() >= 1)
        cnt = atoi(arge[0].c_str());

    GameMsgHdr imh(0x1AA, WORKER_THREAD_WORLD, NULL, sizeof(cnt));
    GLOBAL().PushMsg(imh, &cnt);
}

void GMHandler::OnSingleHeroStageNextTime(GObject::Player* player, std::vector<std::string>& arge)
{
    GObject::shStageMgr.setNextTime();
}

void GMHandler::OnSingleHeroStageReset(GObject::Player* player, std::vector<std::string>& arge)
{
    GameMsgHdr imh(0x1A9, WORKER_THREAD_WORLD, NULL, NULL);
    GLOBAL().PushMsg(imh, NULL);
}

void GMHandler::OnSingleHeroStageGoTo(GObject::Player* player, std::vector<std::string>& arge)
{
	if(arge.size() < 1)
		return;
    UInt8 dst = atoi(arge[0].c_str());
    GObject::shStageMgr.setDstProgress(dst);
}

void GMHandler::OnNewPlayerAuto(GObject::Player* player, std::vector<std::string>& arge)
{
	if(arge.size() < 1)
		return;

    int cnt = atoi(arge[0].c_str()) + 1000;
    std::string newname;
    newname.resize(10);

    for(int playerID = 1000; playerID < cnt; ++ playerID)
    {
        newname.clear();
        char buf[10] = {0};
        sprintf(buf, "%d", playerID);
        newname = buf;

        GObject::Player::patchMergedName(playerID, newname);
        GObject::Player * pl = GObject::globalNamedPlayers[newname];
        if(pl == NULL)
        {
            pl = GObject::globalPlayers[playerID];
        }

        if(pl != NULL)
            continue;

        pl = new(std::nothrow) GObject::Player(playerID);
        if(pl == NULL)
            continue;

        UInt16 loc = 0x0002;
        UInt8 country = COUNTRY_NEUTRAL; // XXX: 低级玩家暂时规为中立

        PLAYER_DATA(pl, name) = newname;
        PLAYER_DATA(pl, country) = country;
        PLAYER_DATA(pl, formation) = FORMATION_1;
        PLAYER_DATA(pl, created) = TimeUtil::Now();
        pl->addNewFormation(FORMATION_1);
        pl->addNewFormation(FORMATION_2);

        UInt32 fgtId = playerID%6 + 1;
        PLAYER_DATA(pl, newGuild) = 0;

        GObject::Fighter * fgt2 = GObject::globalFighters[fgtId];
        GObject::Fighter * fgt = NULL;
        if(fgt2 != NULL)
        {
            fgt = fgt2->clone(pl);
            if(fgt != NULL)
                pl->addFighter(fgt, true);
        }
        if(fgt == NULL)
        {
            delete pl;
        }
        else
        {
            GObject::Lineup& lup = PLAYER_DATA(pl, lineup)[0];
            lup.pos = 12;
            lup.fighter = fgt;
            lup.updateId();

            DB1().PushUpdateData("INSERT INTO `player` (`id`, `name`, `country`, `location`, `lineup`, `wallow`, `formation`, `formations`) VALUES (%" I64_FMT "u, '%s', %u, %u, '%u,12', %u, %u, '%u,%u')", pl->getId(), newname.c_str(), country, loc, fgtId, PLAYER_DATA(pl, wallow), FORMATION_1, FORMATION_1, FORMATION_2);

            GObject::globalPlayers.add(pl);
            GObject::newPlayers.add(pl);
            GObject::globalNamedPlayers.add(newname, pl);
            pl->SetVar(GObject::VAR_VIPFIRST, 1); // XXX: fix old servers

            DBLOG1().PushUpdateData("insert into register_states(server_id,player_id,player_name,platform,reg_time) values(%u,%"I64_FMT"u, '%s', %u, %u)", cfg.serverLogId, pl->getId(), pl->getName().c_str(), 0, TimeUtil::Now());

            CountryEnterStruct ces(false, 1, loc);
            GameMsgHdr imh(0x1F0, country, pl, sizeof(CountryEnterStruct));
            GLOBAL().PushMsg(imh, &ces);
            UInt8 flag = 0;
            GameMsgHdr imh2(0x201, country, pl, 1);
            GLOBAL().PushMsg(imh2, &flag);
        }
    }
}

void GMHandler::OnNewPlayerAutoSuper(GObject::Player* player, std::vector<std::string>& arge)
{
	if(arge.size() < 1)
		return;

    int cnt = atoi(arge[0].c_str()) + 1000;
    for(int playerID = 1000; playerID < cnt; ++ playerID)
    {
        Player* pl = GObject::globalPlayers[playerID];
        if(!pl)
            continue;

        std::vector<std::string> cmd;
        OnSuper(pl, cmd);
    }
}

void GMHandler::OnShowBattlePoint(GObject::Player* player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt8 type = atoi(args[0].c_str());
    if(type == 1)
    {
        SYSMSG_SENDV(623, player, static_cast<UInt32>(player->getBattlePoint()));

        for(int i = 0; i < 5; ++ i)
        {
            GObject::Lineup& lup = PLAYER_DATA(player, lineup)[i];
            Fighter* fighter = lup.fighter;
            if(fighter)
                SYSMSG_SENDV(624, player, fighter->getName().c_str(), static_cast<UInt32>(fighter->getBattlePoint()));
        }
        std::map<UInt32, FairyPet *>& fairyPet = player->getFairyPet();
        std::map<UInt32, FairyPet *>::iterator it = fairyPet.begin();
        for(; it != fairyPet.end(); ++ it)
        {
            FairyPet * pet = it->second;
            if(pet)
                SYSMSG_SENDV(625, player, pet->getName().c_str(), static_cast<UInt32>(pet->getBattlePoint()));
        }
    }
    else if(type == 2)
    {
        UInt32 petId = atoi(args[1].c_str());
        FairyPet * pet = player->findFairyPet(petId);
        if(pet == NULL) return;
        Script::BattleFormula * bformula = Script::BattleFormula::getCurrent();
        UInt32 hp = static_cast<UInt32>(bformula->calcHP(pet));
        UInt16 atk = static_cast<UInt16>(bformula->calcAttack(pet));
        UInt16 magatk = static_cast<UInt16>(bformula->calcMagAttack(pet));
        UInt16 def = static_cast<UInt16>(bformula->calcDefend(pet));
        UInt16 magdef = static_cast<UInt16>(bformula->calcMagDefend(pet));
        UInt16 action = static_cast<UInt16>(bformula->calcAction(pet));
        UInt8 lingya = pet->getPetLingya();
        UInt16 cri = static_cast<float>(bformula->calcCritical(pet, NULL)) * 100;
        UInt16 cridmg = static_cast<float>(bformula->calcCriticalDmg(pet)) * 100;
        UInt16 prc = static_cast<float>(bformula->calcPierce(pet, NULL)) * 100;
        UInt16 magres = static_cast<float>(bformula->calcMagRes(pet, NULL)) * 100;
        UInt16 hit = static_cast<float>(bformula->calcHitrate(pet, NULL)) * 100;
        UInt16 evd = static_cast<float>(bformula->calcEvade(pet, NULL)) * 100;
        UInt16 cnt = static_cast<float>(bformula->calcCounter(pet, NULL)) * 100;
        UInt16 tough = static_cast<float>(bformula->calcTough(pet, NULL)) * 100;

        SYSMSG_SENDV(626, player, pet->getName().c_str(), static_cast<UInt32>(pet->getBattlePoint()),
                hp, atk, magatk, def, magdef, action, lingya, cri, cridmg, prc, magres, hit, evd, cnt, tough);
    }
    else if(type == 3)
    {
        for(int i = 0; i < 5; ++ i)
        {
            GObject::Lineup& lup = PLAYER_DATA(player, lineup)[i];
            Fighter* fighter = lup.fighter;
            if(fighter)
            {
                UInt32 basePoint = fighter->calcBaseBattlePoint();
                UInt32 eqPoint = fighter->calcEquipBattlePoint();
                UInt32 skillPoint = fighter->calcSkillBattlePoint();
                UInt32 cittaPoint = fighter->calcCittaBattlePoint();
                UInt32 soulPoint = fighter->calc2ndSoulBattlePoint();
                UInt32 clanPoint = fighter->calcClanBattlePoint();
                UInt32 lingbaoPoint = fighter->calcLingbaoBattlePoint1();
                UInt32 formPoint = fighter->calcFormBattlePoint();
                UInt32 petPoint = 0;
                FairyPet * pet = player->getBattlePet();
                if(pet)
                    petPoint = pet->getBattlePoint();
                SYSMSG_SENDV(627, player, fighter->getName().c_str(), basePoint, eqPoint, skillPoint, cittaPoint, soulPoint, clanPoint, petPoint, lingbaoPoint, formPoint);
            }
        }
    }
}

void GMHandler::OnEnterArena(GObject::Player* player, std::vector<std::string>& arge)
{
    GameMsgHdr imh(0x1AB, WORKER_THREAD_WORLD, NULL, 0);
    GLOBAL().PushMsg(imh, NULL);
}

void GMHandler::OnIdipBuy(GObject::Player* player, std::vector<std::string>& arge)
{
    std::string err;
    player->IDIPBuy(505, 2, 100, err);
}
void GMHandler::OnBigLock(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;

    UInt64 playerId = atoll(args[0].c_str());
    UInt32 expireTime = 0;
    if (args.size() >= 2)
        expireTime = atoi(args[1].c_str()) * 60 * 60 + TimeUtil::Now();
    memLockUser(playerId, expireTime);

/*    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
    if (execu.get() != NULL && execu->isConnected())
    {
        execu->Execute2("REPLACE INTO `locked_player`(`player_id`, `lockExpireTime`) VALUES(%"I64_FMT"u, %u)", playerId, expireTime);
    }
    */
}

void GMHandler::OnBigUnLock(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;

    UInt64 playerId = atoll(args[0].c_str());
    memUnLockUser(playerId);
/*
    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
    if (execu.get() != NULL && execu->isConnected())
    {
        execu->Execute2("DELETE FROM `locked_player` WHERE `player_id` = %"I64_FMT"u", playerId);
    }
    */
}

void GMHandler::OnStrengthen(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() <= 1)
		return;
	else
	{
		StrengthenMgr* mgr = player->GetStrengthenMgr();
        switch(atoi(args[0].c_str()))
		{
		case 1:
			{
				UInt32 val = atoi(args[1].c_str());
                if(val <= 0 || val > 256)
                    return;
                mgr->AddSouls(val);
                mgr->UpdateToDB();
            }
            break;
        case 2:
			{
                UInt8 id = atoi(args[1].c_str());
                GameAction()->doStrong(player, id, 0, 0);
            }
            break;
        case 3:
			{
                mgr->SetSoulId(0);
                mgr->UpdateToDB();
            }
            break;
        case 4:
			{
                mgr->GMClearSthAll();
                mgr->AddSouls(100);
                mgr->UpdateToDB();
            }
            break;
        default:
            return;
            break;
        }
    }
}

void GMHandler::OnForbidSale(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt64 playerId = atoll(args[0].c_str());
    setForbidSaleValue(playerId, true);
//    UInt32 fTime = atol(args[1].c_str());
//    setForbidSaleValue(playerId, true,fTime);

    GObject::Player * pl = GObject::globalPlayers[playerId];
    if (NULL != pl)
    {
        pl->setForbidSale(true);
        GameMsgHdr hdr(0x352, pl->getThreadId(), pl, NULL);
        GLOBAL().PushMsg(hdr, NULL);
    }

    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
    if (execu.get() != NULL && execu->isConnected())
    {
        execu->Execute2("REPLACE into `fsale_player` values(%"I64_FMT"u,%d,1)", playerId, TimeUtil::Now());
    }
}

void GMHandler::OnUnForbidSale(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt64 playerId = atoll(args[0].c_str());
    setForbidSaleValue(playerId, false);

    GObject::Player * pl = GObject::globalPlayers[playerId];
    if (NULL != pl)
        pl->setForbidSale(false);
    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
    if (execu.get() != NULL && execu->isConnected())
    {
        execu->Execute2("REPLACE into `fsale_player` values(%"I64_FMT"u,%d,0)", playerId, TimeUtil::Now());
    }
}

void GMHandler::OnSetLoginLimit(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 2)
        return;
    UInt8 pf = atoi(args[0].c_str());
    UInt32 v = atoi(args[1].c_str());
    setPlatformLogin(pf, v);
}

void GMHandler::OnNewCountryBattle(GObject::Player * player, std::vector<std::string>& args)
{
	if(args.size() <= 1)
		return;
    UInt16 loc = player->getLocation();
    GObject::Map * map = GObject::Map::FromSpot(loc);
    if(map == NULL)
        return;
    GObject::SpotData * sd = map->GetSpot(loc);
    if(!sd || !sd->m_NewCountryBattle)
        return;
    NewCountryBattle * ncb = sd->m_NewCountryBattle;
    NewCBPlayerData * ncbpData = ncb->findNCBData(player);
    if(!ncbpData)
        return;
    switch(atoi(args[0].c_str()))
    {
    case 1:
        {
            UInt32 val = atoi(args[1].c_str());
            ncbpData->setAchievementLevel(val);
            ncbpData->addAngerDomineer(0, val);
            ncbpData->addAngerDomineer(1, val);
            ncb->sendSelfInfo(player, ncbpData);
        }
        break;
    case 2:
        {
            UInt32 val = atoi(args[1].c_str());
            ncbpData->totalAchievement -= val;
            if(ncbpData->totalAchievement < 0)
                ncbpData->totalAchievement = 0;
            ncbpData->setAchievementLevel();
            ncb->sendSelfInfo(player, ncbpData);
        }
        break;
    default:
        return;
        break;
    }
}

void GMHandler::OnSysUpdate(GObject::Player *player, std::vector<std::string>& args)
{
    UInt8 t = 0;
    UInt8 show = 0;
    std::string v = VERSION;
    if (args.size() >= 1)
        t = atoll(args[0].c_str());
    if (args.size() >= 2)
        show = atoll(args[1].c_str());
    if (args.size() >= 3)
       v = args[2]; 
   //版本更新公告
   Stream st(REP::SYSDAILOG);
   st << static_cast<UInt8>(1);
   st << static_cast<UInt8>(t);
   st << static_cast<UInt8>(show);
   st << v;
   st << Stream::eos;
   player->send(st);

//    player->sendSysUpdate();
}

void GMHandler::OnCFriend(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    CFriend * cFriend = player->GetCFriend();
    if(!cFriend) return;
    switch(atoi(args[0].c_str()))
    {
    case 1:
        {
            UInt8 idx = atoi(args[1].c_str());
            cFriend->setCFriendSafe(idx);
            if(idx > 3 && idx < 7)
                cFriend->setCFriendNum(3);
            if(idx >= 39)
                cFriend->setCFriendSuccess(3);
        }
        break;
    case 2:
        cFriend->clearAllForGM();
        break;
    case 3:
        player->AddVar(VAR_CFRIENDTICKETS, atoi(args[1].c_str()));
        break;
    default:
        break;
     }
    cFriend->sendCFriend();
}

void GMHandler::OnShuoShuo(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    ShuoShuo * sShuo = player->GetShuoShuo();
    if(!sShuo) return;
    switch(atoi(args[0].c_str()))
    {
    case 1:
        sShuo->setShuoSafe(atoi(args[1].c_str()));
        break;
    case 2:
        sShuo->clearAllForGM();
        break;
    default:
        break;
     }
    sShuo->sendShuoShuo();
}

void GMHandler::OnSaveGoldAct(GObject::Player *player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return ;
    switch(atoi(args[0].c_str()))
    {
    case 1:
        player->SetVar(VAR_SAVEGOLD_ISGET, 0);
        break;
    case 2:
        {
            player->SetVarNow(VAR_SAVEGOLD_ISGET, 0, 0);
            player->SetVarNow(VAR_SAVEGOLD_GET_STATUS, 0, 0);
        }
        break;
    case 3:
        {
            UInt32 gold = atoi(args[1].c_str());
            player->SetVar(VAR_SAVEGOLD_COUNT, gold);
            player->SetVar(VAR_SAVEGOLD_SET_TIME, TimeUtil::Now() - 7*86400-10);
        }
        break;
    }
    player->sendSaveGoldAct();
}

void GMHandler::OnLingbao(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return ;
	for(size_t i = 0; i < args.size(); ++ i)
	{
		UInt32 itemId = atoi(args[i].c_str());
		if(IsEquipTypeId(itemId))
		{
            GObject::Package * package = player->GetPackage();
			ItemEquip* equip = static_cast<ItemEquip*>(package->AddEquip(itemId));
            ItemClass ic = equip->getClass();
            switch(ic)
            {
            case Item_LBling:
            case Item_LBwu:
            case Item_LBxin:
                {
                    UInt8 attrNum = 4;
                    ItemLingbaoAttr& lbattr = (static_cast<ItemLingbao*>(equip))->getLingbaoAttr();
                    UInt8 lv = equip->getReqLev();
                    stLBAttrConf& lbAttrConf = GObjectManager::getLBAttrConf();
                    std::vector<UInt8> allAttrType = lbAttrConf.attrType;
                    UInt8 itemTypeIdx = ic - Item_LBling;
                    for(int i = 0; i < attrNum; ++ i)
                    {
                        UInt8 size = allAttrType.size();
                        UInt8 idx = uRand(size);
                        lbattr.type[i] = allAttrType[idx];
                        lbattr.value[i] = lbAttrConf.getAttrMax(lv, itemTypeIdx, lbattr.type[i]-1) * lbAttrConf.getDisFactor(9999);
                        allAttrType.erase(allAttrType.begin() + idx);
                    }
                    lbattr.lbColor = 2 + lbAttrConf.getColor(lv, itemTypeIdx, lbattr.type, lbattr.value, attrNum);
                    if(lbattr.lbColor == 5)
                    {
                        for(int i = 0; i < 2; ++ i)
                        {
                            UInt16 lbIdx = 0;
                            if(i > 0)
                                lbIdx = ic - Item_LBling + 1;
                            UInt8 maxCnt = lbAttrConf.getSkillsMaxCnt(lbIdx, lv);
                            UInt16 skillId = lbAttrConf.getSkill(lbIdx, lv, uRand(maxCnt));
                            lbattr.skill[i] = skillId;
                            UInt16 factor = GData::lbSkillManager[skillId]->minFactor;
                            lbattr.factor[i] = factor + uRand(10000-factor);
                        }
                    }

                    std::string strType;
                    std::string strValue;
                    std::string strSkill;
                    std::string strFactor;
                    for(int i = 0; i < 4; ++ i)
                    {
                        strType += Itoa(lbattr.type[i], 10);
                        strValue += Itoa(lbattr.value[i], 10);

                        if(i < 3)
                        {
                            strType += ',';
                            strValue += ',';
                        }
                        if(i < 2)
                        {
                            strSkill += Itoa(lbattr.skill[i], 10);
                            strFactor += Itoa(lbattr.factor[i], 10);

                            if(i < 1)
                            {
                                strSkill += ',';
                                strFactor += ',';
                            }
                        }
                    }

                    DB4().PushUpdateData("REPLACE INTO `lingbaoattr`(`id`, `tongling`, `lbcolor`, `types`, `values`, `skills`, `factors`, `battlepoint`) VALUES(%u, %d, %d, '%s', '%s', '%s', '%s', '%u')", equip->getId(), lbattr.tongling, lbattr.lbColor, strType.c_str(), strValue.c_str(), strSkill.c_str(), strFactor.c_str(), lbattr.battlePoint);
                }
                break;
            default:
                break;
            }
		}
	}
}


void GMHandler::OnLingbaoSkill(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return ;
	for(size_t i = 0; i < args.size(); ++ i)
	{
		UInt32 itemId = atoi(args[i].c_str());
        ++ i;
        UInt16 skill1 = atoi(args[i].c_str()); 
        ++ i;
        UInt16 skill2 = atoi(args[i].c_str()); 
		if(IsEquipTypeId(itemId))
		{
            GObject::Package * package = player->GetPackage();
			ItemEquip* equip = static_cast<ItemEquip*>(package->AddEquip(itemId));
            ItemClass ic = equip->getClass();
            switch(ic)
            {
            case Item_LBling:
            case Item_LBwu:
            case Item_LBxin:
                {
                    UInt8 attrNum = 4;
                    ItemLingbaoAttr& lbattr = (static_cast<ItemLingbao*>(equip))->getLingbaoAttr();
                    UInt8 lv = equip->getReqLev();
                    stLBAttrConf& lbAttrConf = GObjectManager::getLBAttrConf();
                    std::vector<UInt8> allAttrType = lbAttrConf.attrType;
                    UInt8 itemTypeIdx = ic - Item_LBling;
                    for(int i = 0; i < attrNum; ++ i)
                    {
                        UInt8 size = allAttrType.size();
                        UInt8 idx = uRand(size);
                        lbattr.type[i] = allAttrType[idx];
                        lbattr.value[i] = lbAttrConf.getAttrMax(lv, itemTypeIdx, lbattr.type[i]-1) * lbAttrConf.getDisFactor(9999);
                        allAttrType.erase(allAttrType.begin() + idx);
                    }
                    lbattr.lbColor = 2 + lbAttrConf.getColor(lv, itemTypeIdx, lbattr.type, lbattr.value, attrNum);
                    if(lbattr.lbColor == 5)
                    {
                        for(int i = 0; i < 2; ++ i)
                        {
                            UInt16 lbIdx = 0;
                            if(i > 0)
                                lbIdx = ic - Item_LBling + 1;
                            if(i == 0)
                                lbattr.skill[i] = skill1;
                            else
                                lbattr.skill[i] = skill2;
                            const GData::LBSkillBase* skillBase = GData::lbSkillManager[lbattr.skill[i]];
                            if(!skillBase)
                            {
                                lbattr.skill[i] = 0;
                                continue;
                            }
                            UInt16 factor = skillBase->minFactor;
                            lbattr.factor[i] = factor + uRand(10000-factor);
                        }
                    }

                    std::string strType;
                    std::string strValue;
                    std::string strSkill;
                    std::string strFactor;
                    for(int i = 0; i < 4; ++ i)
                    {
                        strType += Itoa(lbattr.type[i], 10);
                        strValue += Itoa(lbattr.value[i], 10);

                        if(i < 3)
                        {
                            strType += ',';
                            strValue += ',';
                        }
                        if(i < 2)
                        {
                            strSkill += Itoa(lbattr.skill[i], 10);
                            strFactor += Itoa(lbattr.factor[i], 10);

                            if(i < 1)
                            {
                                strSkill += ',';
                                strFactor += ',';
                            }
                        }
                    }

                    DB4().PushUpdateData("REPLACE INTO `lingbaoattr`(`id`, `tongling`, `lbcolor`, `types`, `values`, `skills`, `factors`, `battlepoint`) VALUES(%u, %d, %d, '%s', '%s', '%s', '%s', '%u')", equip->getId(), lbattr.tongling, lbattr.lbColor, strType.c_str(), strValue.c_str(), strSkill.c_str(), strFactor.c_str(), lbattr.battlePoint);
                }
                break;
            default:
                break;
            }
		}
	}
}

void GMHandler::OnLingbaos(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return ;
    UInt32 itemId = atoi(args[0].c_str());
    UInt32 cnt = atoi(args[1].c_str());
	for(size_t i = 0; i < cnt; ++ i)
	{
		if(IsEquipTypeId(itemId))
		{
            GObject::Package * package = player->GetPackage();
			package->AddEquip(itemId);
        }
	}
}

void GMHandler::testLingbao(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return ;
    UInt32 itemId = atoi(args[0].c_str());
    UInt32 cnt = atoi(args[1].c_str());
    UInt32 colors[4] = {0};
    UInt32 skills[2] = {0};
	for(size_t i = 0; i < cnt; ++ i)
	{
		if(IsEquipTypeId(itemId))
		{
            GObject::Package * package = player->GetPackage();
			package->testLingbao(itemId, colors, 4, skills, 2);
        }
	}

	SYSMSG_SENDV(4116, player, itemId, cnt, colors[0], colors[1], colors[2], colors[3], skills[0], skills[1]);
}



void GMHandler::OnDreamerTimeSet(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt8 count = atoi(args[0].c_str());
    player->setDreamerTime(count);
}

void GMHandler::OnDreamerKeySet(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt8 count = atoi(args[0].c_str());
    player->setDreamerKey(count);
}

void GMHandler::OnDreamerEyeSet(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt8 count = atoi(args[0].c_str());
    player->setDreamerEye(count);
}

void GMHandler::OnSomeAct(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt8 type = atoi(args[0].c_str());
    if(type == 1)
    {
        if(args.size() < 2)
            return;
        UInt32 day = atoi(args[1].c_str());
        UInt32 now = TimeUtil::MkTime(2013, 1, 31) + 86400 * day;
        player->calcNewYearQzoneContinueDay(now);
        player->sendNewYearQzoneContinueAct();
    }
    else if(type == 2)
    {
        if(args.size() < 3)
            return;
        UInt32 npcId = atoi(args[1].c_str());
        UInt32 type = atoi(args[2].c_str());
        player->autoBattle(npcId, type);
    }
    else if(type == 3)
        player->instantAutoBattle();
}

void GMHandler::OnDragonKingAct(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 1)
        return;
    UInt32 type = atoi(args[0].c_str());
    if(type >= GObject::DRAGONKING_MAX)
        type = 1;
    GObject::GVAR.SetVar(GObject::GVAR_DRAGONKING_ACTION, type);
    GObject::GVAR.SetVar(GObject::GVAR_DRAGONKING_BEGIN, TimeUtil::Now());
    GObject::GVAR.SetVar(GObject::GVAR_DRAGONKING_END, TimeUtil::Now() + 3600);
}

void GMHandler::OnFairyPetGM(GObject::Player *player, std::vector<std::string>& args)
{
    if (args.size() < 2)
        return;
    UInt8 type = atoi(args[0].c_str());
    UInt32 val = atoi(args[1].c_str());
    switch(type)
    {
        case 0:
            {
                FairyPet * pet = player->findFairyPet(val);
                if(pet)
                {
                    pet->setPetBone(EVOLVE_BONE_LIMIT);
                    pet->sendGenguInfo();
                }
            }
            break;
        case 1:
            player->setCanHirePet(val);
            player->hireFairyPet(val);
            player->sendFairyPetList();
            break;
        case 2:
            player->AddVar(VAR_FAIRYPET_LONGYUAN, val);
            player->AddVar(VAR_FAIRYPET_FENGSUI, val);
            player->AddVar(VAR_FAIRYPET_XIANYUAN, val);
            player->AddVar(VAR_FAIRYPET_LIKEABILITY, 1);
            player->sendFairyPetResource();
            break;
        case 3:
            {
                FairyPet * pet = player->findFairyPet(val);
                if(pet)
                {
                    pet->setPetEvolve(atoi(args[2].c_str()));
                    pet->sendPetEvolve();
                }
            }
            break;
        case 4:
            player->AddVar(VAR_FAIRYPET_LIKEABILITY, val);
            player->sendFairyPetResource();
            break;
    }
}

void GMHandler::OnSurnameleg(GObject::Player *player, std::vector<std::string>& args)
{
    UInt8 type = atoi(args[0].c_str());
     UInt16 reloadFlag = 0x00FF;
     GameMsgHdr hdr4(0x1EE, WORKER_THREAD_WORLD, NULL, sizeof(UInt16));
#pragma pack(1)
            struct mas
            {
                UInt8 hash[36];
                UInt8 type;
                UInt32 begin;
                UInt32 end;
            }_msg;
#pragma pack()
            _msg.type = 2;
            _msg.begin = TimeUtil::Now();
            _msg.end = TimeUtil::Now() + 300;
            LoginMsgHdr hdr1(SPEQ::ACTIVITYONOFF, WORKER_THREAD_LOGIN, 0,0, sizeof(mas));
    switch(type)
    {
        case 1:
            GVAR.SetVar(GVAR_SURNAMELEGEND_BEGIN, TimeUtil::Now());
            GVAR.SetVar(GVAR_SURNAMELEGEND_END, TimeUtil::Now() + 300);
		    GLOBAL().PushMsg(hdr4, &reloadFlag);
            player->LuckyBagRank();
            GLOBAL().PushMsg(hdr1, &_msg);
            break;
        case 2:
            GVAR.SetVar(GVAR_SURNAMELEGEND_BEGIN, 0);
            GVAR.SetVar(GVAR_SURNAMELEGEND_END, 0);
		    GLOBAL().PushMsg(hdr4, &reloadFlag);
            break;
    }
}

void GMHandler::OnFoolsDayGM(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    UInt8 type = atoi(args[0].c_str());
    if(type == 1)
    {
        player->SetVar(VAR_FOOLS_DAY, 0);
        player->SetVar(VAR_FOOLS_DAY_INFO, 0);
    }
    else if(type == 2)
    {
        player->SetVar(VAR_NUWA_SIGNET, 0);
        player->SetVar(VAR_NUWA_OPENTIME, 0);
    }
}

void GMHandler::OnLuckyStarGM(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    UInt8 type = atoi(args[0].c_str());
    switch(type)
    {
        case 1:
            GVAR.SetVar(GVAR_LUCKYSTAR_BEGIN, TimeUtil::Now());
            GVAR.SetVar(GVAR_LUCKYSTAR_END, TimeUtil::Now()+86400*2);
            break;
        case 2:
            player->SetVar(VAR_LUCKYSTAR_GET_STATUS, 0);
            player->sendLuckyStarInfo(2);
            break;
        case 3:
            GVAR.SetVar(GVAR_LUCKYSTAR_BEGIN, 0);
            GVAR.SetVar(GVAR_LUCKYSTAR_END, 0);
            break;
    }
}
void GMHandler::OnClanBossOpen(GObject::Player *player, std::vector<std::string>& args)
{
    GObject::ClanBoss::instance().setCanOpened(true);
    GObject::ClanBoss::instance().start();
}

void GMHandler::OnClanBoss(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 1)
		return;
    int bossMaxHp = atoi(args[0].c_str());
    GObject::ClanBoss::instance().setBossHp(bossMaxHp);
}
void GMHandler::OnClanBossPick(GObject::Player *player, std::vector<std::string>& args)
{
}
void GMHandler::OnClanBossEmpower(GObject::Player *player, std::vector<std::string>& args)
{
	if(args.size() < 2)
		return;
    UInt8 t = atoi(args[0].c_str());
    UInt32 em = atoi(args[1].c_str());
    GObject::ClanBoss::instance().addEmpower(t, em);
}
void GMHandler::OnClanBossSetEm(GObject::Player *player, std::vector<std::string>& args)
{
    if(args.size() < 1)
        return;
    UInt8 t = atoi(args[0].c_str()); 
    GObject::ClanBoss::instance().setPowerType(t);
}

void GMHandler::OnSetTownDeamonMaxLevel(GObject::Player *player, std::vector<std::string>& args)
{
    if(args.size() < 1)
        return;
    UInt16 lv = atoi(args[0].c_str()); 
    player->getDeamonPlayerData()->maxLevel = lv;
}

void GMHandler::OnFairySpar(GObject::Player *player, std::vector<std::string>& args)
{
    if(args.size() < 1)
        return;
    UInt8 type = atoi(args[0].c_str());
    if(type == 0)
    {
        if(args.size() < 2)
            return;
        UInt8 complexP = atoi(args[1].c_str());
        if(complexP > 100)
            complexP = 100;
        player->GetFairySpar()->gmSetComplexPercent(complexP);
    }
    else if(type == 1)
    {
        if(args.size() < 6)
            return;
        UInt8 elem1 = atoi(args[1].c_str());
        if(elem1 > 24)
            elem1 = 24;
        UInt8 elem2 = atoi(args[2].c_str());
        if(elem2 > 24)
            elem2 = 24;
        UInt8 elem3 = atoi(args[3].c_str());
        if(elem3 > 24)
            elem3 = 24;
        UInt8 elem4 = atoi(args[4].c_str());
        if(elem4 > 24)
            elem4 = 24;
        UInt8 elem5 = atoi(args[5].c_str());
        if(elem5 > 24)
            elem5 = 24;
        player->GetFairySpar()->gmSetElement(elem1, elem2, elem3, elem4, elem5);
    }
}

void GMHandler::OnAddPetEquipExp(GObject::Player *player, std::vector<std::string>& args)
{
    if(args.size() < 3)
        return;
    UInt32 petId = atoi(args[0].c_str());
    UInt8 pos = atoi(args[1].c_str());
    FairyPet * pet = player->findFairyPet(petId);
    if(!pet) return;
    ItemPetEq * equip = pet->findEquip(pos);
    if(!equip) return;
    ItemPetEqAttr & peAttr = equip->getPetEqAttr();
    if(peAttr.lv >= GData::pet.getEquipMaxLev())
        return;
    UInt32 upExp = GData::pet.getEquipExpData(peAttr.lv, equip->getQuality() - 2);
    if(upExp == 0) return;
    peAttr.exp += atoi(args[2].c_str());
    if(peAttr.exp >= upExp)
    {
        UInt8 maxLev = GData::pet.getEquipMaxLev();
        UInt8 tmp = peAttr.lv;
        for(UInt8 i = tmp; i <= maxLev; ++ i)
        {
            if(peAttr.exp < GData::pet.getEquipExpData(i, equip->getQuality() - 2))
                break;
            ++ peAttr.lv;
        }
        if(peAttr.lv >= maxLev)
            peAttr.lv = maxLev;
    }
    UInt8 skillLev = GData::pet.getEquipSkillLev(peAttr.lv);
    if(skillLev && SKILLANDLEVEL(SKILL_ID(peAttr.skill), skillLev) != peAttr.skill)
    {
        peAttr.skill = SKILLANDLEVEL(SKILL_ID(peAttr.skill), skillLev);
        if (pet)
        {
            std::string skills = Itoa(peAttr.skill);
            pet->setSkills(skills, false);
            pet->updateToDBPetSkill();
        }
    }
    DB4().PushUpdateData("UPDATE `petEquipattr` SET `exp` = %u, `level` = %u, `skillId` = %u WHERE `id` = %u", peAttr.exp, peAttr.lv, peAttr.skill, equip->getId());
    pet->setDirty();
    pet->sendModification(equip, pos);
}

void GMHandler::OnPetEq(GObject::Player * player, std::vector<std::string>& args)
{
	if (args.size() < 1)
		return ;
	for(size_t i = 0; i < args.size(); ++ i)
	{
		UInt32 itemId = atoi(args[i].c_str());
        ++ i;
        UInt16 skill = atoi(args[i].c_str());
		const GData::ItemBaseType * itype = GData::itemBaseTypeManager[itemId];
		if(itype == NULL)
            break;

        GObject::PetPackage * package = player->GetPetPackage();
        UInt32 id = IDGenerator::gItemOidGenerator.ID();
        ItemPetEqAttr ipeqAttr;
        ItemEquipData edata;
        ipeqAttr.skill = skill;
        ItemPetEq * equip = new ItemPetEq(id, itype, edata, ipeqAttr);
        if(equip == NULL)
            break;

        equip->SetBindStatus(true);
        package->AddPetEquip(equip, true);
    }

}


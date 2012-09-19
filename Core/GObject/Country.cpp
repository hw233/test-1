#include "Config.h"
#include "Country.h"
#include "GData/GDataManager.h"
#include "Script/GameActionLua.h"
#include "Script/BattleFormula.h"
#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Boss.h"
#include "CountryBattle.h"
#include "HeroIsland.h"
#include "Log/Log.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"
#include "ClanRankBattle.h"

namespace GObject
{

Country::Country(UInt8 tid): m_ThreadID(tid), m_GameActionLua(NULL), m_BattleFormula(NULL)
{
}

Country::~Country()
{

}

void Country::Country_Boss_Check(void *)
{
	bossManager.process(TimeUtil::Now());
}

void Country::Country_Battle_Check(void *)
{
	globalCountryBattle.process(TimeUtil::Now());
}

void Country::Hero_Island_Check(void *)
{
    heroIsland.process(TimeUtil::Now());
}

void Country::ClanRankBattleCheck(void *)
{
    ClanRankBattleMgr::Instance().Process(TimeUtil::Now());
}

bool Country::Init()
{
	//GameActionLua
	std::string path = cfg.scriptPath + "main.lua";
	m_GameActionLua = new Script::GameActionLua(TID(), path.c_str());
	path = cfg.scriptPath + "formula/main.lua";
	m_BattleFormula = new Script::BattleFormula(path.c_str());
	if(TID() == WORKER_THREAD_NEUTRAL)
	{
		UInt32 now = TimeUtil::Now();
		bossManager.getNextBoss();
		bossManager.process(now);
        ClanRankBattleMgr::Instance().Init();
		AddTimer(30000, Country_Boss_Check);
		AddTimer(5000, Country_Battle_Check, static_cast<void *>(NULL), (5 - (now % 5)) * 1000);
		AddTimer(5000, Hero_Island_Check, static_cast<void *>(NULL), (5 - (now % 5)) * 1000);
        AddTimer(1000, ClanRankBattleCheck);
        //townDeamonManager->process();
	}

	return true;
}

bool shutdown_enum(Player * pl, UInt8 tid)
{
	if (pl != NULL && pl->getThreadId() == tid && pl->isOnline())
	{
		pl->Logout(true);
	}
	return true;
}

void Country::UnInit()
{
    if(TID() == WORKER_THREAD_NEUTRAL)
    {
        ClanRankBattleMgr::Instance().UnInit();
    }

	//???͵ǳ???Ϣ??????????????
	globalPlayers.enumerate(shutdown_enum, m_ThreadID);

	SAFE_DELETE(m_GameActionLua);
}

std::string Country::GetLogName()
{
	return "log/Country" + Itoa(TID()) + "/";
}

void Country::PlayerEnter(Player * pl, bool notify)
{
	if(pl == NULL)
		return;

	pl->setThreadId(GetThreadID());
	m_Players.add(pl);

	if(notify)
	{
		if(PLAYER_DATA(pl, inCity))
		{
			Map * map = pl->GetMap();
			if(map != NULL)
            {
				map->PlayerEnter(pl, true);
                ClanRankBattleMgr::Instance().PlayerEnter(pl);
            }
		}
	}
}

void Country::PlayerLeave(Player * pl)
{
	if(pl == NULL)
		return ;
	Map * map = pl->GetMap();
	if(map != NULL)
		map->PlayerLeave(pl);
	m_Players.remove(pl);
}

Player * Country::FindPlayer( UInt32 slot )
{
	return m_Players[slot];
}

void Country::Broadcast( UInt16 loc, Stream& st )
{
	Map * map = Map::FromSpot(loc);
	if(map == NULL)
		return;
	map->Broadcast(loc, st);
}

}

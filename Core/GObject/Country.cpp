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
#include "ClanCopy.h"
#include "NewCountryBattle.h"
#include "TownDeamon.h"
#include "Athletics.h"
#include "HeroMemo.h"

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

void Country::ClanCopyCheck(void *)
{
    ClanCopyMgr::Instance().process(TimeUtil::Now());
}

void Country::ClanCopyResetBegin(void *)
{
    ClanCopyMgr::Instance().ResetBegin();
}

void Country::ClanCopyReset(void *)
{
    ClanCopyMgr::Instance().Reset();
}

bool PhysicalCheckEnumerate(Player * pl, UInt8 para)
{
    if(!pl || !(pl->GetAthletics()))
        return false;
    pl->GetAthletics()->PhysicalCheck();
    return true;
}

void Country::PhysicalCheckTimer(void *para)
{
	globalPlayers.enumerate(PhysicalCheckEnumerate, static_cast<UInt8>(0));
}

void Country::ClanCopyResetEnd(void *)
{
    ClanCopyMgr::Instance().ResetEnd();
}

inline bool heromemo_enum(Player * p, int)
{
    if (!GVAR.GetVar(GVAR_HEROMEM_CUT))
    {
        if (!p)
            return true;
        p->GetHeroMemo()->flushAward();
    }
    return true;
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
        UInt32 now = TimeUtil::Now(), sweek = TimeUtil::SharpWeek(1);
        bossManager.getNextBoss();
		bossManager.process(now);
        ClanRankBattleMgr::Instance().Init();
        NewCountryBattle::Init();
		AddTimer(30000, Country_Boss_Check);
        AddTimer(5000, Country_Battle_Check, static_cast<void *>(NULL), (5 - (now % 5)) * 1000);
		AddTimer(5000, Hero_Island_Check, static_cast<void *>(NULL), (5 - (now % 5)) * 1000);
        AddTimer(1000, ClanRankBattleCheck);
        //townDeamonManager->process();
        //UInt32 tdChkPoint = TimeUtil::SharpDayT(0, now) + TOWNDEAMONENDTM;
        /** 提前2分钟 **/
	    AddTimer(3600 * 1000, PhysicalCheckTimer, static_cast<void *>(NULL), (((3600 - now % 3600) + 58 * 60) % 3600) * 1000);

        AddTimer(1000, ClanCopyCheck);

        AddTimer(3600 * 24 * 7 * 1000, ClanCopyResetBegin, static_cast<void * >(NULL), 
                (sweek - now) > 1800 ? (sweek - now - 1800) * 1000 : ((sweek - now) + 3600 * 24 * 7 - 1800) * 1000 );
        AddTimer(3600 * 24 * 7 * 1000, ClanCopyReset, static_cast<void * >(NULL), (sweek - now + 240) * 1000);
        AddTimer(3600 * 24 * 7 * 1000, ClanCopyResetEnd, static_cast<void * >(NULL), (sweek - now + 1800) * 1000);
        //AddTimer(4 * 60 * 1000, ClanCopyResetBegin, static_cast<void * >(NULL), 60 * 1000);
        //AddTimer(4 * 60 * 1000, ClanCopyReset, static_cast<void * >(NULL), 120 * 1000);
        //AddTimer(4 * 60 * 1000, ClanCopyResetEnd, static_cast<void * >(NULL), 180 * 1000);
        globalPlayers.enumerate(heromemo_enum, m_ThreadID);
        GVAR.SetVar(GVAR_HEROMEM_CUT, 1);
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
                ClanCopyMgr::Instance().playerEnter(pl);
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

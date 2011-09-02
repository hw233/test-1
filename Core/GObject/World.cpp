#include "Config.h"
#include "World.h"
#include "Leaderboard.h"
#include "ClanManager.h"
#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Map.h"
#include "Fighter.h"
#include "TaskMgr.h"
#include "EventBase.h"
#include "LuckyDraw.h"
#include "ChatItem.h"
#include "Announce.h"
#include "Dungeon.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecHelper.h"
#include "Server/SysMsg.h"
#include "Common/TimeUtil.h"
#include "GObject/TradeCheck.h"
#include "GObject/SaleMgr.h"
#include "GObject/AthleticsRank.h"
#include "GObject/ClanBattle.h"
#include "GObject/ClanManager.h"
#include "GObject/Clan.h"
#include "Script/WorldScript.h"
#include "Script/BattleFormula.h"
#include "SpecialAward.h"
#include "PracticePlace.h"
#include "WorldBoss.h"
#include "MsgID.h"

namespace GObject
{

struct DBDataLatest
{
	UInt32 announce;
};

}

namespace DB {

SPECIALBEGIN(GObject::DBDataLatest)
SPECIALDEF(1)
(
	UInt32, announce
)
SPECIALEND()

}

namespace GObject
{
UInt8 World::_wday = 0;
int World::_activityStage = 0;
bool World::_actAvailable = false;
bool World::_actAvailable1 = false;
bool World::_isNewServer = false;

#define LEADERBOARD_UPDATE_INTERVAL 3600

World::World(): WorkerRunner<WorldMsgHandler>(1000), _worldScript(NULL), _battleFormula(NULL), _now(TimeUtil::Now()), _today(TimeUtil::SharpDay(0, _now + 30)), _announceLast(0)
{
}

World::~World()
{
}

void World::World_testUpdate( World * world )
{
	world->testUpdate();
}

void World::World_Leaderboard_Update( void * )
{
	leaderboard.update();
}

void World::World_ChatItem_Purge( void * )
{
	chatItem.purge(TimeUtil::Now());
}

void World::World_Multi_Check( World * world )
{
	UInt32 now = world->_now;
	announce.process(now);
	gTradeCheck.update(now);
	gSaleMgr.update(now);
	clanManager.process(now, world->_today);
}

bool enum_midnight(void * ptr, void *)
{
	Player * pl = static_cast<Player *>(ptr);
	if(pl == NULL)
		return true;
	if (pl->isOnline())
	{
		GameMsgHdr hdr(0x269, pl->getThreadId(), pl, 0);
		GLOBAL().PushMsg(hdr, NULL);
	}
	if(pl->getGold() > 0)
	{
		DBLOG().PushUpdateData("insert into `gold_accounts`(`server_id`, `player_id`, `gold`, `time`) values(%u, %"I64_FMT"u, %u, %u)", cfg.serverLogId, pl->getId(), pl->getGold(), TimeUtil::SharpDay(1));
	}

    pl->buildClanTask();
    pl->clearFinishCount();
	return true;
}

bool enum_clan_midnight(void * ptr, void * data)
{
	Clan * clan = static_cast<Clan *>(ptr);
	if (clan == NULL)
		return true;
	clan->getClanDynamicMsg()->cleanClanDynamicMsg();
	Player * leader = clan->getLeader();
	if (leader == NULL)
		return true;
#if 0
	UInt32 now = *reinterpret_cast<UInt32 *>(data);
	UInt32 lastOnline = leader->getLastOnline();
	if (now > lastOnline && now - lastOnline >= 10 * 86400)
		clan->alterLeader();
#endif

	return true;
}
void World::makeActivityInfo(Stream &st)
{
	st.init(REP::DAILY_DATA);
	st << static_cast<UInt8>(5) << _wday << static_cast<UInt8>(_activityStage) << Stream::eos;
}
void World::calWeekDay()
{
	time_t curtime1 = time(NULL) + 30;
	struct tm *local = localtime(&curtime1);
	_wday = static_cast<UInt8>(local->tm_wday);
	if(_wday == 0)
		_wday = static_cast<UInt8>(7);
	if(_wday == 4)
		ClanCityBattle::setMaxEnterCount(3 * 2);
	else
		ClanCityBattle::setMaxEnterCount(3);
	if(_wday == 3)
	{
		size_t sz;
		UInt16 * prices = Dungeon::getPrice(sz);
		for(size_t i = 0; i < sz; ++ i)
			prices[i] /= 2;	
	}
    if(_wday == 1)
    {
        practicePlace.resetPracticePlaceIncoming();
    }
}

void World::setWeekDay(UInt8 wday)
{
	_wday = wday;
	if(_wday == 4)
		ClanCityBattle::setMaxEnterCount(3 * 2);
	else
		ClanCityBattle::setMaxEnterCount(3);
	if(_wday == 3)
	{
		size_t sz;
		_worldScript->onActivityCheck(TimeUtil::SharpDay(1) - 10);
		UInt16 * prices = Dungeon::getPrice(sz);
		for(size_t i = 0; i < sz; ++ i)
			prices[i] /= 2;	
	}
	Stream st(REP::DAILY_DATA);
	makeActivityInfo(st);
	NETWORK()->Broadcast(st);
}


bool enum_dungeon_midnight(void * ptr, void * data)
{
	Dungeon *dg = static_cast<Dungeon *>(ptr); 
	if(dg == NULL)
		return false;
	UInt32 now = *reinterpret_cast<UInt32 *>(data);
	dg->pushEnterCountBS(now);

	return true;
}
void World::World_Midnight_Check( World * world )
{
	UInt32 curtime = TimeUtil::Now();
	world->_worldScript->onActivityCheck(curtime+30);

	world->_today = TimeUtil::SharpDay(0, curtime+30);	
	DB().PushUpdateData("UPDATE `player` SET `icCount` = 0;");
	luckyDraw.setLuckyDrawCost();
	luckyDraw.checkCleanup();

	globalPlayers.enumerate(enum_midnight, static_cast<void *>(NULL));
	
	dungeonManager.enumerate(enum_dungeon_midnight, &curtime);
	globalClans.enumerate(enum_clan_midnight, &curtime);
	clanManager.reConfigClanBattle();
	challengeCheck.clear();
	
	calWeekDay();
	Stream st(REP::DAILY_DATA);
	makeActivityInfo(st);
	NETWORK()->Broadcast(st);
}

void World::World_Online_Log( void * )
{
	UInt32 onlineNums=NETWORK()->getOnlineNum();
	DBLOG().PushUpdateData("insert into online_situations (server_id,divtime,num) values(%u,%u,%u)", cfg.serverLogId, TimeUtil::Now(), onlineNums);
}

void World::World_Athletics_Check( void * )
{
	gAthleticsRank.TmExtraAward();
}

void World::World_Boss_Refresh(void*)
{
    worldBoss.refresh(TimeUtil::Now());
}

void World::World_Boss_Prepare(void*)
{
    worldBoss.prepare(TimeUtil::Now());
}

bool World::Init()
{
	GObjectManager::delayLoad();
	GObjectManager::LoadPracticeData();
	GObjectManager::LoadTripodData();
	
	std::string path = cfg.scriptPath + "World/main.lua";
	_worldScript = new Script::WorldScript(path.c_str());
	path = cfg.scriptPath + "formula/main.lua";
	_battleFormula = new Script::BattleFormula(path.c_str());

	calWeekDay();

	AddTimer(60 * 1000, World_testUpdate, this);
	AddTimer(LEADERBOARD_UPDATE_INTERVAL * 1000, World_Leaderboard_Update);
	AddTimer(3600 * 4 * 1000, World_ChatItem_Purge);
	AddTimer(5000, World_Multi_Check, this);

	UInt32 now = TimeUtil::Now(), sday = TimeUtil::SharpDay(1) - 10;
	if(sday < now) sday += 86400;
	AddTimer(86400 * 1000, World_Midnight_Check, this, (sday - now) * 1000);
	AddTimer(600 * 1000, World_Online_Log, static_cast<void *>(NULL), ((now + 600) / 600 * 600 - now) * 1000);
	UInt32 athChkPoint = TimeUtil::SharpDay(0, now) + EXTRAREWARDTM;
	
    if (cfg.GMCheck || true)
    {
        AddTimer(5 * 60 * 1000, World_Boss_Prepare, static_cast<void*>(NULL));
        UInt32 worldBossChk = TimeUtil::SharpDay(0, now) + 13 * 3600;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), (worldBossChk >= now ? worldBossChk - now : 86400 + athChkPoint - now) * 1000);
        worldBossChk = TimeUtil::SharpDay(0, now) + 14 * 3600;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), (worldBossChk >= now ? worldBossChk - now : 86400 + athChkPoint - now) * 1000);
        worldBossChk = TimeUtil::SharpDay(0, now) + 15 * 3600;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), (worldBossChk >= now ? worldBossChk - now : 86400 + athChkPoint - now) * 1000);
        worldBossChk = TimeUtil::SharpDay(0, now) + 16 * 3600;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), (worldBossChk >= now ? worldBossChk - now : 86400 + athChkPoint - now) * 1000);
        worldBossChk = TimeUtil::SharpDay(0, now) + 17 * 3600;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), (worldBossChk >= now ? worldBossChk - now : 86400 + athChkPoint - now) * 1000);
        worldBossChk = TimeUtil::SharpDay(0, now) + 18 * 3600;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), (worldBossChk >= now ? worldBossChk - now : 86400 + athChkPoint - now) * 1000);
        worldBossChk = TimeUtil::SharpDay(0, now) + 19 * 3600;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), (worldBossChk >= now ? worldBossChk - now : 86400 + athChkPoint - now) * 1000);
        worldBossChk = TimeUtil::SharpDay(0, now) + 20 * 3600;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), (worldBossChk >= now ? worldBossChk - now : 86400 + athChkPoint - now) * 1000);
        //AddTimer(86400 * 1000, World_Athletics_Check, static_cast<void *>(NULL), (athChkPoint >= now ? athChkPoint - now : 86400 + athChkPoint - now) * 1000);
    }
    else
    {
        AddTimer(5 * 60 * 1000, World_Boss_Prepare, static_cast<void*>(NULL), 30000);
        UInt32 worldBossChk = TimeUtil::SharpDay(0, now) + 9 * 60 * 60 + 20 * 60;
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), 30000);
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), 10*60*1000);
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), 20*60*1000);
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), 30*60*1000);
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), 40*60*1000);
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), 50*60*1000);
        AddTimer(86400 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), 60*60*1000);
        //AddTimer(86400 * 1000, World_Athletics_Check, static_cast<void *>(NULL), 60 * 1000);
    }

	return true;
}

void World::UnInit()
{
	delete _worldScript;
}

void World::OnTimer()
{
	_now = TimeUtil::Now();
}

std::string World::GetLogName()
{
	return "log/World/";
}

void World::testUpdate( )
{
	std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
	if (execu.get() == NULL || !execu->isConnected()) return;
	DBDataLatest latestlist;
	execu->Extract("SELECT `announce` FROM `data_latest`", latestlist);
	if(latestlist.announce != _announceLast)
	{
		announce.reload();
		_announceLast = latestlist.announce;
	}
}

}

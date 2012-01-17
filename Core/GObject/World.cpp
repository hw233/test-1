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
#include "WBossMgr.h"
#include "HeroIsland.h"
#include "MsgID.h"
#include "GObject/DCLogger.h"
#include "GObject/TeamCopy.h"

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
UInt32 World::_moneyLogged = 0;
MoneyIn World::_moneyIn[7][2] = {{{0,},{0,}},};
int World::_activityStage = 0;
bool World::_actAvailable = false;
bool World::_actAvailable1 = false;
bool World::_isNewServer = false;
bool World::_nationalDay = false;
bool World::_halloween = false;
bool World::_singleday = false;
bool World::_thanksgiving = false;
bool World::_christmas = false;
bool World::_newyear = false;
bool World::_blueactiveday = false;
bool World::_rechargeactive = false;

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
	//leaderboard.update(); // XXX: 移到单独线程去处理
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




typedef std::multimap<UInt32, Player*> ChopStickSortMap;
ChopStickSortMap chopStickSortMap;

bool bSingleDayEnd = false;

bool enum_midnight(void * ptr, void *)
{
	Player * pl = static_cast<Player *>(ptr);
	if(pl == NULL)
		return true;
	if (pl->isOnline())
	{
		GameMsgHdr hdr(0x269, pl->getThreadId(), pl, 0);
		GLOBAL().PushMsg(hdr, NULL);

        if (World::_blueactiveday)
            pl->onBlueactiveday();
	}
    else
    {
        pl->buildClanTask(true);
        pl->clearFinishCount();
        if (World::_thanksgiving)
            pl->resetThanksgiving();
    }
#if 0
	if(pl->getGold() > 0)
	{
		DBLOG1().PushUpdateData("insert into `gold_accounts`(`server_id`, `player_id`, `gold`, `time`) values(%u, %"I64_FMT"u, %u, %u)", cfg.serverLogId, pl->getId(), pl->getGold(), TimeUtil::SharpDay(1));
	}
#endif

    if(bSingleDayEnd)
    {
        UInt32 num = pl->GetVar(VAR_SINGLE_CHOPSTICKS);
        if(num > 0)
        {
            chopStickSortMap.insert(std::make_pair(num, pl));
        }
    }

    if (World::_halloween && pl->isOnline())
        pl->sendHalloweenOnlineAward(TimeUtil::Now(), true);

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

    clan->ClearDueItemHistory();
    clan->SetDailyBattleScore(0);
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
	st << static_cast<UInt8>(5) << _wday;
    st << static_cast<UInt8>(_newyear?1:0) << Stream::eos;
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
#if 0
		size_t sz;
		UInt16 * prices = Dungeon::getPrice(sz);
		for(size_t i = 0; i < sz; ++ i)
			prices[i] /= 2;	
#endif
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
    if (!cfg.GMCheck)
        _worldScript->onActivityCheck(TimeUtil::Now()+30);
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

    bool bSingleDay = getSingleDay();
	world->_worldScript->onActivityCheck(curtime+30);

	world->_today = TimeUtil::SharpDay(0, curtime+30);	
	DB1().PushUpdateData("UPDATE `player` SET `icCount` = 0;");
	luckyDraw.setLuckyDrawCost();
	luckyDraw.checkCleanup();

    chopStickSortMap.clear();
    //判断是不是光棍节结束
    bSingleDayEnd = bSingleDay && !getSingleDay();
	globalPlayers.enumerate(enum_midnight, static_cast<void *>(NULL));

    //给筷子
    if(bSingleDayEnd)
    {
        int pos = 0;
        UInt32 chopStickNum = 0xFFFFFFFF;
        for(ChopStickSortMap::reverse_iterator iter = chopStickSortMap.rbegin();
                iter != chopStickSortMap.rend(); ++iter)
        {
            if(iter->first != chopStickNum)
            {
                ++pos;
                chopStickNum = iter->first;
            }
            if(pos > 3) break;

            UInt32 titles[] = {0, 1, 2, 3};
           
            Player* player = iter->second;
            player->setTitle(titles[pos]);
            SYSMSG_BROADCASTV(2142, player->getCountry(), player->getPName(), titles[pos]); 
        }
    }
	
	dungeonManager.enumerate(enum_dungeon_midnight, &curtime);
	globalClans.enumerate(enum_clan_midnight, &curtime);
	clanManager.reConfigClanBattle();
	challengeCheck.clear();
	
	calWeekDay();
	Stream st(REP::DAILY_DATA);
	makeActivityInfo(st);
	NETWORK()->Broadcast(st);
    World_CreateNewDB_Check();
}
void World::World_CreateNewDB_Check()
{
    UInt32 day = 1;
    UInt32 mon = 1;
    UInt32 year = 2011;
    TimeUtil::GetDMY(&day, &mon, &year);
    if(day == 28)//28日开始建数据库
    {
        TimeUtil::GetNextMY(mon, &year);
        CreateNewDB(mon, year);
    }
}
void CreateNewDB(UInt32 mon , UInt32 year)
{
   //TO DO 配置
   //if(0 == mon)
   //{
   //    UInt32 day = 1;
   //    TimeUtil::GetDMY(&day, &mon, &year);
   //
#if 0
   DBLOG1().PushUpdateData("CREATE TABLE IF NOT EXISTS `consume_tael_%u_%u`\
           ( `server_id` int(10) unsigned NOT NULL,\
             `player_id` bigint(20) unsigned NOT NULL,\
             `item_id` int(10) unsigned NOT NULL,\
             `consume_type` int(10) unsigned NOT NULL,\
             `item_num` int(10) unsigned NOT NULL,\
             `expenditure` int(10) unsigned NOT NULL,\
             `consume_time` int(10) unsigned NOT NULL,\
             INDEX server_player (`server_id`, `player_id`),\
             INDEX server_player_item (`server_id`, `player_id`, `item_id`),\
             INDEX server_player_type (`server_id`, `player_id`, `consume_type`)\
           ) ENGINE=MyISAM DEFAULT CHARSET=utf8;",year, mon);
#endif

    DBLOG1().PushUpdateData(cfg.sql_consume_tael.c_str(), year, mon);
    DBLOG1().PushUpdateData(cfg.sql_item_courses.c_str(), year, mon);
    DBLOG1().PushUpdateData(cfg.sql_item_histories.c_str(), year, mon);
}
void World::World_Online_Log( void * )
{
	UInt32 onlineNums=NETWORK()->getOnlineNum();
	DBLOG1().PushUpdateData("insert into online_situations (server_id,divtime,num) values(%u,%u,%u)", cfg.serverLogId, TimeUtil::Now(), onlineNums);
    dclogger.online();
}

void World::World_Athletics_Check( void * )
{
	gAthleticsRank.TmExtraAward();
}

void World::World_Boss_Refresh(void*)
{
    worldBoss.process(TimeUtil::Now());
}

void World::Team_Copy_Process(void*)
{
    teamCopyManager->process(TimeUtil::Now());
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
    UInt32 day = 1;
    UInt32 mon = 1;
    UInt32 year = 2011;
    TimeUtil::GetDMY(&day, &mon, &year);
    CreateNewDB(mon, year);
    TimeUtil::GetNextMY(mon, &year);
    CreateNewDB(mon, year);
	AddTimer(60 * 1000, World_testUpdate, this);
	//AddTimer(3600 * 1000, World_Leaderboard_Update);
	AddTimer(3600 * 4 * 1000, World_ChatItem_Purge);
	AddTimer(5000, World_Multi_Check, this);

	UInt32 now = TimeUtil::Now(), sday = TimeUtil::SharpDay(1) - 10;
	if(sday < now) sday += 86400;
	AddTimer(86400 * 1000, World_Midnight_Check, this, (sday - now) * 1000);
    AddTimer(5 * 60 * 1000, World_Online_Log, static_cast<void *>(NULL), ((now + 300) / 300 * 300 - now) * 1000);
    AddTimer(5 * 1000, World_Boss_Refresh, static_cast<void*>(NULL));

    UInt32 athChkPoint = TimeUtil::SharpDayT(0, now) + EXTRAREWARDTM;
    AddTimer(86400 * 1000, World_Athletics_Check, static_cast<void *>(NULL), (athChkPoint >= now ? athChkPoint - now : 86400 + athChkPoint - now) * 1000);

    AddTimer(5 * 1000, Team_Copy_Process, static_cast<void*>(NULL));
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

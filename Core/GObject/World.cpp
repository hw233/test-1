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
#include "ChatItem.h"
#include "Announce.h"
#include "Dungeon.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecHelper.h"
#include "Server/SysMsg.h"
#include "Common/TimeUtil.h"
#include "TradeCheck.h"
#include "SaleMgr.h"
#include "AthleticsRank.h"
#include "ClanBattle.h"
#include "ClanManager.h"
#include "Clan.h"
#include "Script/WorldScript.h"
#include "Script/BattleFormula.h"
#include "SpecialAward.h"
#include "PracticePlace.h"
#include "WBossMgr.h"
#include "HeroIsland.h"
#include "MsgID.h"
#include "DCLogger.h"
#include "TeamCopy.h"
#include "ArenaBattle.h"
#include "GData/Store.h"
#include "CountryBattle.h"
#include "ClanRankBattle.h"
#include "ShuoShuo.h"
#include "CFriend.h"
#include "Common/Itoa.h"
#include "Server/SysMsg.h"
#include "Arena.h"

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
bool World::_autoHeal = false;
bool World::_nationalDay = false;
bool World::_halloween = false;
bool World::_singleday = false;
bool World::_thanksgiving = false;
bool World::_christmas = false;
bool World::_newyear = false;
bool World::_blueactiveday = false;
bool World::_rechargeactive = false;
UInt8 World::_rechargeactiveno = 0;
bool World::_valentineday = false;
bool World::_netvalentineday = false;
bool World::_girlday = false;
bool World::_whiteloveday = false;
bool World::_trumpenchret = false;
bool World::_foolsday = false;
bool World::_chingming = false;
bool World::_carnival = false;
bool World::_rc7day = false;
bool World::_shuoshuo = false;
bool World::_cfriend = false;
bool World::_mayday = false;
bool World::_mayday1 = false;
bool World::_ydmdact = false;
bool World::_ssdtact = false;
bool World::_fighter1368 = false;
bool World::_enchantact = false;
bool World::_trainfighter = false;
bool World::_gemmergeact = false;
bool World::_bluediamondact = false;
bool World::_yellowdiamondact = false;
bool World::_qqgameact = false;
void* World::_recalcwd = NULL;
bool World::_june = false;
bool World::_june1 = false;
bool World::_july = false;
bool World::_enchant_gt11 = false;
bool World::_rechargenextret;
UInt32 World::_rechargenextretstart;
UInt32 World::_rechargenextretend;
bool World::_mergeathact = false;
bool World::_fourcopact = false;
bool World::_duanwu;
bool World::_icact;
UInt32 World::_levelawardend;
bool World::_pexpitems;
UInt32 World::_sosomapbegin = 0;
bool World::_opentest;
bool World::_consumeactive;

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

void World::ReCalcWeekDay( World * world )
{
    if(!world)
        return;
    calWeekDay(world);
}

typedef std::multimap<UInt32, Player*> ChopStickSortMap;
ChopStickSortMap chopStickSortMap;

typedef std::multimap<UInt32, Player*> ShusanLoveSortMap;
ShusanLoveSortMap shusanLoveSortMap;

typedef std::multimap<UInt32, Player*> MayDaySortMap;
MayDaySortMap mayDaySortMap1;
MayDaySortMap mayDaySortMap2;
MayDaySortMap mayDaySortMap3;

typedef std::multimap<UInt32, Player*> JuneSortMap;
JuneSortMap juneSortMap;

typedef std::multimap<UInt32, Player*> PExpItemsMap;
PExpItemsMap pexpItemsMap;

bool bSingleDayEnd = false;
bool bValentineDayEnd = false;
bool bMayDayEnd = false;
bool bJuneEnd = false;
bool bPExpItemsEnd = false;

bool enum_midnight(void * ptr, void* next)
{
    UInt32 nextday = *static_cast<UInt32*>(next);
	Player * pl = static_cast<Player *>(ptr);
	if(pl == NULL)
		return true;
	if (pl->isOnline())
	{
		GameMsgHdr hdr(0x269, pl->getThreadId(), pl, sizeof(nextday));
		GLOBAL().PushMsg(hdr, &nextday);
	}
    else
    {
        pl->buildClanTask(true);
        pl->clearFinishCount();
        if (World::_thanksgiving)
            pl->resetThanksgiving();
    }

    if(bSingleDayEnd)
    {
        UInt32 num = pl->GetVar(VAR_SINGLE_CHOPSTICKS);
        if(num > 0)
        {
            chopStickSortMap.insert(std::make_pair(num, pl));
        }
    }

    if (bValentineDayEnd)
    {
        UInt32 num = pl->GetVar(VAR_SHUSAN_LOVE);
        if(num > 0)
            shusanLoveSortMap.insert(std::make_pair(num, pl));
    }

    if (bMayDayEnd)
    {
        UInt32 num = pl->GetVar(VAR_MDSOUL_CNT1);
        if (num > 0)
            mayDaySortMap1.insert(std::make_pair(num, pl));

        num = pl->GetVar(VAR_MDSOUL_CNT2);
        if (num > 0)
            mayDaySortMap2.insert(std::make_pair(num, pl));

        num = pl->GetVar(VAR_MDSOUL_CNT3);
        if (num > 0)
            mayDaySortMap3.insert(std::make_pair(num, pl));
    }

    if(bJuneEnd)
    {
        UInt32 num = pl->GetVar(VAR_JUNE_HAPPY_ITEM_CNT);
        if (num > 0)
            juneSortMap.insert(std::make_pair(num, pl));
    }

    if(bPExpItemsEnd)
    {
        UInt32 num = pl->GetVar(VAR_PEXPITEMS);
        if (num > 0)
            pexpItemsMap.insert(std::make_pair(num, pl));
    }


    if (World::_halloween && pl->isOnline())
        pl->sendHalloweenOnlineAward(TimeUtil::Now(), true);

    if (pl->isOnline() && World::_wday == 7)
        GData::store.sendList(1, pl);

    if (!pl->isOnline())
    {
        pl->GetShuoShuo()->reset(false);
        pl->GetCFriend()->reset(false);
    }

    if (TimeUtil::SharpDay(0, nextday) >= TimeUtil::SharpDay(0, World::_rechargenextretstart)+13*24*60*60 &&
            TimeUtil::SharpDay(0, nextday) < TimeUtil::SharpDay(0, World::_rechargenextretend)+13*24*60*60+2*24*60*60)
        pl->sendRNR(nextday);

    if (pl->GetVar(VAR_RECHARGE_TOTAL) &&
            (TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 7, 1) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 7, 4) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 7, 9) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 7, 17) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 7, 25)))
    {
        if (pl->isOnline())
        {
            GameMsgHdr hdr(0x282, pl->getThreadId(), pl, 0);
            GLOBAL().PushMsg(hdr, NULL);
        }
        else
        {
            pl->SetVar(VAR_RECHARGE_TOTAL, 0);
        }
    }
#ifdef _FB
    if (TimeUtil::SharpDay(0, nextday) == TimeUtil::SharpDay(0, World::_levelawardend))
        pl->sendLevelAward();
#endif

	return true;
}

bool enum_clan_midnight(void * ptr, void * data)
{
	Clan * clan = static_cast<Clan *>(ptr);
	if (clan == NULL)
		return true;
	Player * leader = clan->getLeader();
	if (leader == NULL)
		return true;

    clan->ClearDueItemHistory();
    clan->SetDailyBattleScore(0);
	return true;
}

bool enum_lucky_draw_rank_list(void * ptr, void * data )
{
    Player* player = static_cast<Player*>(ptr);
    if(player == NULL)
        return true;

    WORLD().RankLuckyDraw(player, false);
    return true;
}

void World::makeActivityInfo(Stream &st)
{
	st.init(REP::DAILY_DATA);
	st << static_cast<UInt8>(5) << _wday;

    UInt32 active = _newyear?1:0;
    active |= _rechargeactiveno&1?2:0;
    active |= _girlday?4:0;
    //active |= _trumpenchret?8:0;
    active |= _rechargeactiveno&2?16:0;
    active |= _foolsday?32:0;
    active |= _chingming?64:0;
    active |= _ssdtact?128:0;
    active |= _june?256:0;
    st << active << Stream::eos;
}
void World::calWeekDay( World * world )
{
	time_t curtime1 = time(NULL) + 30;
	struct tm *local = localtime(&curtime1);
	UInt8 wday = static_cast<UInt8>(local->tm_wday);
	if(wday == 0)
		wday = static_cast<UInt8>(7);
    if(_wday == wday)
    {
        if(!_recalcwd)
            _recalcwd = world->AddTimer(10000, ReCalcWeekDay, world, 10000);
        return;
    }
    else
    {
        if(_recalcwd)
        {
            world->RemoveTimer(_recalcwd);
            _recalcwd = NULL;
        }
        _wday = wday;
    }
	if(_wday == 4)
		ClanCityBattle::setMaxEnterCount(3 * 2);
	else
		ClanCityBattle::setMaxEnterCount(3);
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

void SendShusanLoveTitleCard()
{
    if(bValentineDayEnd)
    {
        int pos = 0;
        UInt32 shunsanLoveNum = 0xFFFFFFFF;
        for(ShusanLoveSortMap::reverse_iterator iter = shusanLoveSortMap.rbegin();
                iter != shusanLoveSortMap.rend(); ++iter)
        {
            if(iter->first != shunsanLoveNum)
            {
                ++pos;
                shunsanLoveNum = iter->first;
            }
            if(pos > 3) break;

            Player* player = iter->second;
            if (!player)
                continue;
            if (player->isOnline())
            {
                GameMsgHdr hdr(0x237, player->getThreadId(), player, sizeof(pos));
                GLOBAL().PushMsg(hdr, &pos);
            }
            else
            {
                player->sendShusanLoveTitleCard(pos);
            }
        }
    }
}

void SendMDSoulCnt()
{
    if(bMayDayEnd)
    {
        int pos = 0;
        UInt32 mayDayNum = 0xFFFFFFFF;
        for(MayDaySortMap::reverse_iterator iter = mayDaySortMap1.rbegin();
                iter != mayDaySortMap1.rend(); ++iter)
        {
            if(iter->first != mayDayNum)
            {
                ++pos;
                mayDayNum = iter->first;
            }
            if(pos > 1) break;

            Player* player = iter->second;
            if (!player)
                continue;
            if (player->isOnline())
            {
                int type = 1;
                GameMsgHdr hdr(0x246, player->getThreadId(), player, sizeof(type));
                GLOBAL().PushMsg(hdr, &type);
            }
            else
            {
                player->sendMayDayTitleCard(1);
            }
        }

        pos = 0;
        mayDayNum = 0xFFFFFFFF;
        for(MayDaySortMap::reverse_iterator iter = mayDaySortMap2.rbegin();
                iter != mayDaySortMap2.rend(); ++iter)
        {
            if(iter->first != mayDayNum)
            {
                ++pos;
                mayDayNum = iter->first;
            }
            if(pos > 1) break;

            Player* player = iter->second;
            if (!player)
                continue;
            if (player->isOnline())
            {
                int type = 2;
                GameMsgHdr hdr(0x246, player->getThreadId(), player, sizeof(type));
                GLOBAL().PushMsg(hdr, &type);
            }
            else
            {
                player->sendMayDayTitleCard(2);
            }
        }

        pos = 0;
        mayDayNum = 0xFFFFFFFF;
        for(MayDaySortMap::reverse_iterator iter = mayDaySortMap3.rbegin();
                iter != mayDaySortMap3.rend(); ++iter)
        {
            if(iter->first != mayDayNum)
            {
                ++pos;
                mayDayNum = iter->first;
            }
            if(pos > 1) break;

            Player* player = iter->second;
            if (!player)
                continue;
            if (player->isOnline())
            {
                int type = 3;
                GameMsgHdr hdr(0x246, player->getThreadId(), player, sizeof(type));
                GLOBAL().PushMsg(hdr, &type);
            }
            else
            {
                player->sendMayDayTitleCard(3);
            }
        }
    }
}

void SendHappyItemCnt()
{
    if(bJuneEnd)
    {
        int pos = 0;
        UInt32 happyItemNum = 0xFFFFFFFF;
        for(JuneSortMap::reverse_iterator iter = juneSortMap.rbegin();
                iter != juneSortMap.rend(); ++iter)
        {
            if(iter->first != happyItemNum)
            {
                ++pos;
                happyItemNum = iter->first;
            }
            if(pos > 2) break;

            Player* player = iter->second;
            if (!player)
                continue;
            if (player->isOnline())
            {
                GameMsgHdr hdr(0x247, player->getThreadId(), player, sizeof(pos));
                GLOBAL().PushMsg(hdr, &pos);
            }
            else
            {
                player->sendJuneHappyTitleCard(pos);
            }
        }
    }

}

void SendPExpCard()
{
    if(bPExpItemsEnd)
    {
        int pos = 0;
        UInt32 happyItemNum = 0xFFFFFFFF;
        for(PExpItemsMap::reverse_iterator iter = pexpItemsMap.rbegin();
                iter != pexpItemsMap.rend(); ++iter)
        {
            if(iter->first != happyItemNum)
            {
                ++pos;
                happyItemNum = iter->first;
            }
            if(pos > 1) break;

            Player* player = iter->second;
            if (!player)
                continue;
            if (player->isOnline())
            {
                GameMsgHdr hdr(0x253, player->getThreadId(), player, sizeof(pos));
                GLOBAL().PushMsg(hdr, &pos);
            }
            else
            {
                player->sendPExpCard(pos);
            }
        }
    }

}

void World::World_Midnight_Check( World * world )
{
	UInt32 curtime = TimeUtil::Now();

    if (World::_wday == 7)
        GData::store.resetDistcount();

    bool bSingleDay = getSingleDay();
    bool bValentineDay = getValentineDay();
    bool bMayDay = getMayDay();
    bool bJune = getJune();
    bool bPExpItems = getPExpItems();
	world->_worldScript->onActivityCheck(curtime+30);

	world->_today = TimeUtil::SharpDay(0, curtime+30);	
	DB1().PushUpdateData("UPDATE `player` SET `icCount` = 0;");

    chopStickSortMap.clear();
    shusanLoveSortMap.clear();
    mayDaySortMap1.clear();
    mayDaySortMap2.clear();
    mayDaySortMap3.clear();

    //判断是不是光棍节结束
    bSingleDayEnd = bSingleDay && !getSingleDay();
    //判断是不是情人节结束
    bValentineDayEnd = bValentineDay && !getValentineDay();
    //五一使用风雷石活动是否结束
    bMayDayEnd = bMayDay && !getMayDay();
    // 六一活动是否结束
    bJuneEnd = bJune && !getJune();
    bPExpItemsEnd = bPExpItems && !getPExpItems();

    UInt32 nextday = curtime + 30;
	globalPlayers.enumerate(enum_midnight, static_cast<void *>(&nextday));
    leaderboard.newDrawingGame(nextday);
    //给筷子使用称号
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

    //给巧克力使用称号卡
    SendShusanLoveTitleCard();
    SendMDSoulCnt();
    SendHappyItemCnt();
    SendPExpCard();
#ifdef _FB
    if(bJuneEnd)
        world->SendLuckyDrawAward();
#endif
	
	dungeonManager.enumerate(enum_dungeon_midnight, &curtime);
	globalClans.enumerate(enum_clan_midnight, &curtime);
	clanManager.reConfigClanBattle();
	challengeCheck.clear();
	
	calWeekDay(world);
	Stream st(REP::DAILY_DATA);
	makeActivityInfo(st);
	NETWORK()->Broadcast(st);
    World_CreateNewDB_Check();
    worldBoss.resetBossSt();
    globalCountryBattle.setStatus(0);
    ClanRankBattleMgr::Instance().setStatus(0);
    teamCopyManager->resetStatus();
    heroIsland.setStatus(0);
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
    DBLOG1().PushUpdateData(cfg.sql_consume_tael.c_str(), year, mon);
    DBLOG1().PushUpdateData(cfg.sql_item_courses.c_str(), year, mon);
    DBLOG1().PushUpdateData(cfg.sql_item_histories.c_str(), year, mon);
}
void World::World_Online_Log( void * )
{
	UInt32 onlineNums=NETWORK()->getOnlineNum();
	DBLOG1().PushUpdateData("insert into online_situations (server_id,divtime,num) values(%u,%u,%u)", cfg.serverLogId, TimeUtil::Now(), onlineNums);
#ifdef _FB
#else
    dclogger.online();
#endif
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

void World::AthleticsPhysicalCheck(void *)
{
    gAthleticsRank.process();
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

	calWeekDay(this);
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
	AddTimer(60 * 1000, World_One_Min, this);

#ifdef _FB
    if(getJune())
        globalPlayers.enumerate(enum_lucky_draw_rank_list, static_cast<void *>(NULL));
#endif

	UInt32 now = TimeUtil::Now(), sday = TimeUtil::SharpDay(1) - 10;
	if(sday < now) sday += 86400;
	AddTimer(86400 * 1000, World_Midnight_Check, this, (sday - now) * 1000);
    AddTimer(5 * 60 * 1000, World_Online_Log, static_cast<void *>(NULL), ((now + 300) / 300 * 300 - now) * 1000);
    AddTimer(5 * 1000, World_Boss_Refresh, static_cast<void*>(NULL));

    UInt32 athChkPoint = TimeUtil::SharpDayT(0, now) + EXTRAREWARDTM;
    AddTimer(86400 * 1000, World_Athletics_Check, static_cast<void *>(NULL), (athChkPoint >= now ? athChkPoint - now : 86400 + athChkPoint - now) * 1000);

    AddTimer(5 * 1000, Team_Copy_Process, static_cast<void*>(NULL));
    AddTimer(3600 * 1000, AthleticsPhysicalCheck);
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

void World::RankLuckyDraw(Player* player, bool notify)
{
    UInt32 luckyDrawCnt = player->GetVar(VAR_LUCKYDRAW_CNT);
    if(luckyDrawCnt == 0)
        return;
    LuckyDrawRankList::iterator it = _luckyDrawRankList.find(player);
    RLuckyDrawRank r_rank = _luckyDrawList.rbegin();
    bool change = false;
    if(it != _luckyDrawRankList.end())
    {
        RLuckyDrawRank r_rank2(it->second);
        r_rank = r_rank2;
    }
    else
        change = true;

    for(; r_rank != _luckyDrawList.rend(); ++ r_rank)
    {
        if(luckyDrawCnt <= (*r_rank)->GetVar(VAR_LUCKYDRAW_CNT))
            break;

        if(!change)
        {
            change = true;
        }

        if(notify)
        {
            SYSMSG(title, 2362);
            UInt32 pos = std::distance(_luckyDrawList.begin(), r_rank.base()) + 1;
            if(pos == 2)
            {
                SYSMSGV(content, 2363, player->getName().c_str(), pos);
                (*r_rank)->GetMailBox()->newMail(NULL, 0x12, title, content, 0xFFFE0000);
            }
            else if(pos == 3)
            {
                SYSMSGV(content, 2363, player->getName().c_str(), pos);
                (*r_rank)->GetMailBox()->newMail(NULL, 0x12, title, content, 0xFFFE0000);
            }
            else if(pos == 4)
            {
                SYSMSGV(content, 2363, player->getName().c_str(), pos);
                (*r_rank)->GetMailBox()->newMail(NULL, 0x12, title, content, 0xFFFE0000);
            }
            else if(pos == 11)
            {
                SYSMSGV(content, 2363, player->getName().c_str(), pos);
                (*r_rank)->GetMailBox()->newMail(NULL, 0x12, title, content, 0xFFFE0000);
            }
        }
    }

    if(change)
    {
        if(it != _luckyDrawRankList.end())
            _luckyDrawList.erase(it->second);
        _luckyDrawRankList[player] = _luckyDrawList.insert(r_rank.base(), player);
        if(notify)
        {
            UInt32 pos = std::distance(_luckyDrawList.begin(), _luckyDrawRankList[player]) + 1;
            if(pos <= 10)
            {
                SYSMSGV(title, 2358, pos);
                SYSMSGV(content, 2359, pos);
                player->GetMailBox()->newMail(NULL, 0x12, title, content, 0xFFFE0000);
            }
        }
    }
}

void World::SendLuckyDrawList(Player* player)
{
    Stream st(REP::LUCKY_RANK);
    LuckyDrawRankList::iterator it = _luckyDrawRankList.find(player);
    UInt32 pos = 0;
    if(it != _luckyDrawRankList.end())
        pos = std::distance(_luckyDrawList.begin(), it->second) + 1;

    st << player->GetVar(VAR_LUCKYDRAW_CNT) << pos;
    UInt8 cnt = 10;
    if(10 > _luckyDrawList.size())
        cnt = static_cast<UInt8>(_luckyDrawList.size());
    st << cnt;
    LuckyDrawRank rank = _luckyDrawList.begin();
    for(int i = 0; rank != _luckyDrawList.end() && i < cnt; ++i, ++rank)
    {
        st << (*rank)->getName() << (*rank)->getCountry() << (*rank)->GetLev() << (*rank)->GetVar(VAR_LUCKYDRAW_CNT);
    }

    st << Stream::eos;
    player->send(st);
}

void World::SendLuckyDrawAward()
{
    int pos = 0;
 
    SYSMSG(title, 2360);
    for(LuckyDrawRank rank = _luckyDrawList.begin(); rank != _luckyDrawList.end() && pos < 10; ++ rank, ++ pos)
    {
        SYSMSGV(content, 2361, (*rank)->getName().c_str(), pos+1);
        if(pos == 0)
        {
            Mail * mail = (*rank)->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[3] = {{9022,5}, {30,6}, {9034, 1}};
                mailPackageManager.push(mail->id, mitem, 3, true);

                std::string strItems;
                for(int i = 0; i < 3; ++ i)
                {
                    strItems += Itoa(mitem[i].id);
                    strItems += ",";
                    strItems += Itoa(mitem[i].count);
                    strItems += "|";
                }
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, (*rank)->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
            }
        }
        else if(pos == 1)
        {
            Mail * mail = (*rank)->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[2] = {{9019,4}, {30,4}};
                mailPackageManager.push(mail->id, mitem, 2, true);

                std::string strItems;
                for(int i = 0; i < 2; ++ i)
                {
                    strItems += Itoa(mitem[i].id);
                    strItems += ",";
                    strItems += Itoa(mitem[i].count);
                    strItems += "|";
                }
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, (*rank)->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
            }
        }
        else if(pos == 2)
        {
            Mail * mail = (*rank)->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[2] = {{9017,3}, {30,2}};
                mailPackageManager.push(mail->id, mitem, 2, true);

                std::string strItems;
                for(int i = 0; i < 2; ++ i)
                {
                    strItems += Itoa(mitem[i].id);
                    strItems += ",";
                    strItems += Itoa(mitem[i].count);
                    strItems += "|";
                }
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, (*rank)->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
            }
        }
        else
        {
            Mail * mail = (*rank)->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[2] = {{9021,2}, {30,1}};
                mailPackageManager.push(mail->id, mitem, 2, true);

                std::string strItems;
                for(int i = 0; i < 2; ++ i)
                {
                    strItems += Itoa(mitem[i].id);
                    strItems += ",";
                    strItems += Itoa(mitem[i].count);
                    strItems += "|";
                }
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, (*rank)->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
            }
        }
    }
}

bool enum_openact(void * ptr, void * v)
{
	Player * pl = static_cast<Player *>(ptr);
	if(pl == NULL)
		return true;
    UInt32 day = *(UInt32*)v;
    pl->sendOpenAct(day);
    return true;
}

void World::World_One_Min( World * world )
{
#ifdef _FB
	UInt32 now = world->_now;
    struct tm t;
    time_t tt = now;
    localtime_r(&tt, &t);

    UInt32 day = 0;

    if (t.tm_year + 1900 == cfg.openYear &&
            t.tm_mon == cfg.openMonth - 1 &&
            t.tm_mday == cfg.openDay + 6 - 1 &&
            t.tm_hour == 19 &&
            t.tm_min == 10)
        day = 2;
    if (t.tm_year + 1900 == cfg.openYear &&
            t.tm_mon == cfg.openMonth - 1 &&
            t.tm_mday == cfg.openDay + 7 - 1 &&
            t.tm_hour == 15 &&
            t.tm_min == 0 )
        day = 1;
    if (t.tm_year + 1900 == cfg.openYear &&
            t.tm_mon == cfg.openMonth - 1 &&
            t.tm_mday == cfg.openDay + 7 - 1 &&
            t.tm_hour == 19 &&
            t.tm_min == 10)
        day = 1;

    if (day)
        globalPlayers.enumerate(enum_openact, (void*)&day);
#endif
}

void World::commitArenaForceOnce()
{
    GObject::arena.commitArenaForceOnce();
}

}

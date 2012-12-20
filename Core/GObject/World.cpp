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
#ifndef _FB
#ifndef _VT
#ifndef _WIN32
#include "DCLogger.h"
#endif
#endif
#endif
#include "TeamCopy.h"
#include "ArenaBattle.h"
#include "GData/Store.h"
#include "CountryBattle.h"
#include "ClanRankBattle.h"
#include "ClanCopy.h"
#include "ShuoShuo.h"
#include "CFriend.h"
#include "Common/Itoa.h"
#include "Server/SysMsg.h"
#include "Arena.h"
#include "Tianjie.h"
#include "DaysRank.h"
#include "TownDeamon.h"
#include "SingleHeroStage.h"
#include "SHSYTmpl.h"
#include "QixiTmpl.h"
#include "MsgHandler/Memcached.h"
#include "RechargeTmpl.h"
#include "GVar.h"

static const UInt32 DAYSRANKTM = 23 * 3600+50*60;

namespace GObject
{

bool lt_valuesort::operator()(const ValueSort& a, const ValueSort& b) const { return a.sufferCnt < b.sufferCnt || (a.sufferCnt == b.sufferCnt && a.lastTime > b.lastTime); }

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
bool World::_copyfrontwin = false;
bool World::_nationalDay = false;
bool World::_halloween = false;
bool World::_singleday = false;
bool World::_thanksgiving = false;
bool World::_christmas = false;
bool World::_newyear = false;
bool World::_blueactiveday = false;
bool World::_rechargeactive = false;
bool World::_rechargeactive3366 = false;
bool World::_yearact = false;
bool World::_qgamegiftact = false;
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
bool World::_qqvipact = false;
bool World::_fallact = false;
bool World::_qqgameact = false;
bool World::_3366privilegeact = false;
bool World::_qzonepyprivilegeact = false;
bool World::_goodvoiceact = false;
bool World::_3366giftact = false;
void* World::_recalcwd = NULL;
bool World::_june = false;
bool World::_june1 = false;
bool World::_july = false;
bool World::_qixi= false;
bool World::_wansheng= false;
bool World::_11Act= false;
bool World::_ssToolbarAct= false;
bool World::_guoqing= false;
bool World::_9215Act= false;
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
bool World::_consume918 = false;
RCSortType World::rechargeSort;
RCSortType World::consumeSort;
bool World::_needrechargerank = false;
bool World::_needconsumerank = false;
bool World::_killMonsteract = 0;
#ifndef _WIN32
CUserLogger* World::ulog = NULL;
#endif
UInt32 World::_rechargebegin = 0;
UInt32 World::_rechargeend = 0;
UInt32 World::_consumebegin = 0;
UInt32 World::_consumeend = 0;
bool   World::_townReward_10_15 = false;
bool World::_loginAward = false;
bool World::_bluediamonSuperman = false;
bool World::_tgcevent = false;
/** 场外活动 **/
stArenaExtra World::stArenaOld[2];
stArenaExtra World::stArena;
/** 0：侠骨；1：柔情；2财富；3传奇 **/
RCSortType World::killMonsterSort[4];

World::World(): WorkerRunner<WorldMsgHandler>(1000), _worldScript(NULL), _battleFormula(NULL), _now(TimeUtil::Now()), _today(TimeUtil::SharpDay(0, _now + 30)), _announceLast(0)
{
}

World::~World()
{
}

void World::World_testUpdate( World * world )
{
	//world->testUpdate();
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
	//announce.process(now);
	gTradeCheck.update(now);
	gSaleMgr.update(now);
	clanManager.process(now, world->_today);

	shStageMgr.process();
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
bool bQixiEnd = false;
bool bWanshengEnd = false;
bool bGuoqingEnd = false;
bool bRechargeEnd = false;
bool bConsumeEnd = false;
bool bXiaoyaoEnd = false;

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
        /*
        if (World::_thanksgiving)
            pl->resetThanksgiving();
        */
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
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 7, 25) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 5) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 17) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 18) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 19) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 27) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 28) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 29) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 3) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 4) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 5) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 6) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 10) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 11) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 12) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 13) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 16) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 17) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 18) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 19) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 20) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 21) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 22) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 23) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 27) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 28) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 29) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 30) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 6) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 8) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 9) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 10) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 11) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 12) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 13) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 14) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 15) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 16) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 17) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 18) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 19) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 20) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 21) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 22) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 23) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 24) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 25) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 26) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 27) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 28)
            ))
    {
        if (pl->isOnline())
        {
            GameMsgHdr hdr(0x282, pl->getThreadId(), pl, 0);
            GLOBAL().PushMsg(hdr, NULL);
        }
        else
        {
            if (pl->GetVar(VAR_RECHARGE_TOTAL))
                pl->SetVar(VAR_RECHARGE_TOTAL, 0);
            if (pl->GetVar(VAR_RECHARGE_SCORE))
                pl->SetVar(VAR_RECHARGE_SCORE, 0);
        }
        GObject::RechargeTmpl::instance().clear();
    }

    if (pl->GetVar(VAR_CONSUME) &&
            (TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 23) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 24) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 25) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 26)
            ))
    {
        if (pl->isOnline())
        {
            GameMsgHdr hdr(0x283, pl->getThreadId(), pl, 0);
            GLOBAL().PushMsg(hdr, NULL);
        }
        else
        {
            if (pl->GetVar(VAR_CONSUME))
                pl->SetVar(VAR_CONSUME, 0);
        }
    }

    if (TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 24) ||
        TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 24))
    {
        int couponCount = 0;
        int total = pl->GetVar(VAR_CONSUME_918);
        if (total >= 1000 && total < 5000)
            couponCount = 300;
        else if (total >= 5000 && total < 10000)
            couponCount = 800;
        else if (total >= 10000 && total < 50000)
            couponCount = 1500;
        else if (total >= 50000)
            couponCount = 5000;
        if (couponCount > 0)
        {
            MailPackage::MailItem mitem[1] = {{0xA000, couponCount}};
            MailItemsInfo itemsInfo(mitem, Activity, 1);
            SYSMSGV(title, 5100);
            SYSMSGV(content, 5102, total, couponCount);
            Mail* mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content,0xFFFE0000, true, &itemsInfo);
            GObject::mailPackageManager.push(mail->id, mitem, 1, true);
        }
    }

#ifdef _FB
    if (TimeUtil::SharpDay(0, nextday) == TimeUtil::SharpDay(0, World::_levelawardend))
        pl->sendLevelAward();
#endif

	return true;
}

bool enum_qixi(void * ptr, void * data)
{
	Player * pl = static_cast<Player *>(ptr);
	if(pl == NULL)
		return true;

    WORLD().sendQixiPlayers(pl);

    return true;
}

bool enum_qixi_score(void * ptr, void * data)
{
	Player * pl = static_cast<Player *>(ptr);
	if(pl == NULL)
		return true;

    WORLD().sendQixiScoreAward(pl);

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

bool enum_qixi_rank_list(void * ptr, void * data )
{
    Player* player = static_cast<Player*>(ptr);
    if(player == NULL || !player->queQiaoCheck())
        return true;

    Player* lover = player->getLover();
    WORLD().LoadQixiScore(player, lover);
    return true;
}

bool enum_extra_act_calc_total(Player* player, void* data)
{
    if(player == NULL)
        return true;
    UInt8 supportid = player->GetVar(GObject::VAR_ARENA_SUPPORT);
    if(supportid >= 1 && supportid <= 5)
        World::setArenaTotalCnt(World::getArenaTotalCnt() + 1);
    return true;
}

bool enum_extra_act_update_status(Player* player, void* data)
{
    if(player == NULL)
        return true;

    UInt8 updatetype = *reinterpret_cast<UInt8 *>(data);
    if(updatetype == 1)
    {
        player->SetVar(VAR_ARENA_SUPPORT, 0);
    }

    if(player->isOnline())
    {
        player->ArenaExtraAct(0, 0);
    }
    return true;
}

bool enum_extra_act_award(Player* player, void* data)
{
    if(player == NULL)
        return true;

    if(player->isOnline())
        player->ArenaExtraAct(3, 0);

    UInt8 supportId = player->GetVar(VAR_ARENA_SUPPORT);
    if(supportId == 0 || supportId > 5)
        return true;

    UInt8 curRank = World::stArena.rank[supportId-1];
    UInt32 moneyArena = 0;
    switch(curRank)
    {
        case 1:
            moneyArena = 800;
            break;
        case 2:
            moneyArena = 600;
            break;
        case 3:
            moneyArena = 500;
            break;
        case 4:
            moneyArena = 400;
            break;
        case 5:
            moneyArena = 300;
            break;
        default:
            break;
    }
    if(moneyArena > 0)
    {
        SYSMSGV(title, 736);
        SYSMSGV(content, 737, curRank, moneyArena);
        player->GetMailBox()->newMail(NULL, 0x01, title, content);

        GameMsgHdr hdr(0x251, player->getThreadId(), player, sizeof(moneyArena));
        GLOBAL().PushMsg(hdr, &moneyArena);
    }
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
	time_t curtime1 = time(NULL) + 300;
	struct tm *local = localtime(&curtime1);
	_wday = static_cast<UInt8>(local->tm_wday);
	if(_wday == 0)
		_wday = static_cast<UInt8>(7);
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

void SendRechargeRankAward()
{
    if(bRechargeEnd)
    {
        World::initRCRank();
        int pos = 0;
        for (RCSortType::iterator i = World::rechargeSort.begin(), e = World::rechargeSort.end(); i != e; ++i)
        {
            ++pos;

            if(pos > 7) break;

            Player* player = i->player;
            if (!player)
                continue;

            GameMsgHdr hdr(0x257, player->getThreadId(), player, sizeof(pos));
            GLOBAL().PushMsg(hdr, &pos);

            SYSMSG_BROADCASTV(4033, pos, player->getCountry(), player->getPName(), i->total);

            char id[1024] = {0};
            char ctx[1024] = {0};
            snprintf(id, sizeof(id), "F_10000_1213_%u_%d", cfg.serverNum, pos);
            snprintf(ctx, sizeof(ctx), "%"I64_FMT"u_%s_%u", player->getId(), player->getPName(), i->total);
            World::udpLog(id, ctx, "", "", "", "", "act");
        }
        World::rechargeSort.clear();
    }
}

void SendConsumeRankAward()
{
    if(bConsumeEnd)
    {
        World::initRCRank();
        int pos = 0;
        for (RCSortType::iterator i = World::consumeSort.begin(), e = World::consumeSort.end(); i != e; ++i)
        {
            ++pos;

            if(pos > 7) break;

            Player* player = i->player;
            if (!player)
                continue;

            GameMsgHdr hdr(0x258, player->getThreadId(), player, sizeof(pos));
            GLOBAL().PushMsg(hdr, &pos);

            SYSMSG_BROADCASTV(4034, pos, player->getCountry(), player->getPName(), i->total);
            //TRACE_LOG("CONSUME RANK: %s\t\t\t%"I64_FMT"u\t\t\t%s\t\t\t%u", pos, player->getId(), player->getPName(), i->total);
        }
        World::consumeSort.clear();
    }
}

void SendKillMonsterRankAward()
{
    for(UInt8 index = 0; index < 4; index++)
    {
        Int32 pos = 0;
        for (RCSortType::iterator i = World::killMonsterSort[index].begin(), e = World::killMonsterSort[index].end(); i != e; ++i)
        {
            ++pos;
            if(pos > 1) break;

            Player* player = i->player;
            if (!player)
                continue;
            //if (player->isOnline())
            //{
            //    GameMsgHdr hdr(0x258, player->getThreadId(), player, sizeof(pos));
            //    GLOBAL().PushMsg(hdr, &pos);
            //}
            //else
            //{
                player->sendKillMonsterRankAward(index, pos);
            //}
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
    bool bQixi = getQixi();
    bool bWansheng = getWansheng();
    bool bGuoqing = getGuoqing();
    bool bXiaoyao = get9215Act();
    bool bRecharge = (getRechargeActive() || getRechargeActive3366()) && getNeedRechargeRank();
    bool bConsume = getConsumeActive() && getNeedConsumeRank();
    bool bPExpItems = getPExpItems();
    bool bMonsterAct = getKillMonsterAct();
	world->_worldScript->onActivityCheck(curtime+300);

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
    bQixiEnd = bQixi && !getQixi();
    bWanshengEnd = bWansheng && !getWansheng();
    bGuoqingEnd = bGuoqing && !getGuoqing();
    bXiaoyaoEnd = bXiaoyao && !get9215Act();
    bRechargeEnd = bRecharge && !(getRechargeActive()||getRechargeActive3366());
    bConsumeEnd = bConsume && !getConsumeActive();
    bool bMonsterActEnd = bMonsterAct && !getKillMonsterAct();
    UInt32 nextday = curtime + 30;

    if (TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 17) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 18) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 19) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 27) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 28) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 10, 29) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 4) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 5) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 6) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 11) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 12) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 13) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 17) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 18) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 19) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 20) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 21) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 22) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 23) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 28) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 29) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 30) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 9) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 10) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 11) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 12) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 13) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 14) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 15) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 16) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 17) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 18) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 19) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 20) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 21) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 22) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 23) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 24) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 25) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 26) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 27) ||
            TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 12, 28)
            )
        bRechargeEnd = true;

    if (TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 24) ||
        TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 25) ||
        TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 26) ||
        TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2012, 11, 27)
            )
        bConsumeEnd = true;

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
    if(bQixiEnd || bWanshengEnd)
        world->SendQixiAward();
    if(bGuoqingEnd)
        world->SendGuoqingAward();
    if (bRechargeEnd)
        SendRechargeRankAward();
    if (bConsumeEnd)
        SendConsumeRankAward();
    if(bMonsterActEnd)
    {
        world->killMonsterInit();
	    SendKillMonsterRankAward();
    }
    if (bXiaoyaoEnd)
        world->SendXiaoyaoAward();

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
#ifndef _WIN32
#ifdef _FB
#else
    dclogger.online();

    UInt32 onlines[MAX_DOMAIN] = {0};
    dclogger.getOnline(onlines);

    char sz_online[32] = {0};
    char sz_time[32] = {0};
    char sz_server[32] = {0};

    snprintf(sz_online, sizeof(sz_online), "%u", onlineNums);
    snprintf(sz_time, sizeof(sz_time), "%u", (UInt32)time(NULL));
    snprintf(sz_server, sizeof(sz_server), "%d", cfg.serverNum);
    udpLog("pcu", sz_server, sz_time, sz_online, "", "", "ser");

    snprintf(sz_online, sizeof(sz_online), "%u", onlines[1]);
    snprintf(sz_time, sizeof(sz_time), "%u", (UInt32)time(NULL));
    snprintf(sz_server, sizeof(sz_server), "%d", cfg.serverNum);
    udpLog("vippcu", sz_server, sz_time, sz_online, "", "", "ser");
#endif
#endif // _WIN32
}

void World::World_Store_Check(void *)
{
    GData::store.process(TimeUtil::Now());
}

void World::World_Athletics_Check( void* data)
{
    UInt8 type = *reinterpret_cast<UInt8 *>(data);
	gAthleticsRank.TmExtraAward(type);
}

void World::World_Boss_Refresh(void*)
{
    worldBoss.process(TimeUtil::Now());
}
void World::Tianjie_Refresh(void*)
{
	GObject::Tianjie::instance().process(TimeUtil::Now());
}
void World::DaysRank_Refresh(void*)
{
	GObject::DaysRank::instance().process();
}
void World::SendQQGameGift(void*)
{
    UInt32 now = TimeUtil::Now();
    if (now < TimeUtil::MkTime(2012, 12, 18) || now > TimeUtil::MkTime(2012, 12, 21))
        return;
    std::unordered_map<UInt64, Player*>& pm = GObject::globalPlayers.getMap();
    std::unordered_map<UInt64, Player*>::iterator iter;
    for (iter = pm.begin(); iter != pm.end(); ++iter)
    {
        Player* pl = iter->second;
        if (NULL != pl && pl->isOnline() && atoi(pl->getDomain()) == 10 && pl->GetVar(VAR_QQGAME_GIFT_1218)==0)
        {
            SYSMSGV(title, 4100, TimeUtil::MonthDay());
            SYSMSGV(content, 4101, TimeUtil::MonthDay());
            Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem = {15,5};
                mailPackageManager.push(mail->id, &mitem, 1, true);
                pl->SetVar(VAR_QQGAME_GIFT_1218, 1);
            }
        }
    }
}
void World::Team_Copy_Process(void*)
{
    teamCopyManager->process(TimeUtil::Now());
}
#if 0
void World::AthleticsPhysicalCheck(void *)
{
    gAthleticsRank.process();
}
#endif
void World::TownDeamonTmAward(void *)
{
    townDeamonManager->process();
}

void World::setArenaInfo(UInt8 type)
{
    GObject::arena.setArenaInfo(type);
}

void World::setArenaTotalCntEnum()
{
    globalPlayers.enumerate(enum_extra_act_calc_total, static_cast<void *>(NULL));
    if(getArenaTotalCnt() < 20)
        setArenaTotalCnt(20);
}

void World::ArenaExtraActTimer(void *)
{
    UInt32 now = TimeUtil::Now();
    UInt32 week = TimeUtil::GetWeekDay(now);

    if(week < ARENA_WEEK_START || week > ARENA_WEEK_END)
    {
        return;
    }

    UInt32 t1 = TimeUtil::SharpDayT(0, now) + ARENA_SINGUP_START;
    UInt32 t2 = TimeUtil::SharpDayT(0, now) + ARENA_SINGUP_END;
    UInt32 t3 = TimeUtil::SharpDayT(0, now) + ARENA_SUFFER_END;
    UInt8 type;
    UInt32 startTime[] = {t1, t2, t3};
    UInt32 curProcess = GVAR.GetVar(GVAR_ARENA_EXT_PROCESS);
    //printf("curProcess = %u\n", curProcess);
    if(week == ARENA_WEEK_START)
        type = 0;
    else
        type = 1;
    static UInt8 updatetype = 1;
    static UInt8 updatetype2 = 2;

    if(now < t1)
    {
        if(stArena.heroId[0] != 0)
            stArena.heroId[0] = 0;
        if(World::getArenaTotalCnt() != 0)
            World::setArenaTotalCnt(0);
        for(UInt8 i = 0; i < 5; i++)
        {
            if(stArena.sufferCnt[i] != 0)
                stArena.sufferCnt[i] = 0;
            if(stArena.lasttime[i] != 0)
                stArena.lasttime[i] = 0;
            if(stArena.rank[i] != 0)
                stArena.rank[i] = 0;
        }
        //GVAR.SetVar(GVAR_ARENA_EXT_PROCESS, 0);
    }
    else
    {
        if(stArena.heroId[0] == 0)
            setArenaInfo(type);
        if(stArena.heroId[0] == 0)
            return;
        if(now >= t2 && World::getArenaTotalCnt() == 0)
        {
            WORLD().setArenaTotalCntEnum();
            //DB1().PushUpdateData("UPDATE `arena_extra_board` SET `sufferTotal` = %u WHERE `week` = %u", stArena.sufferTotal, stArena.week);
        }
    }

    if(curProcess > 2 || now < startTime[curProcess])
        return;
    switch(curProcess)
    {
        case 0:
            printf("t1\n");
            globalPlayers.enumerate(enum_extra_act_update_status, static_cast<void *>(&updatetype));
            GVAR.AddVar(GVAR_ARENA_EXT_PROCESS, 1);
            break;
        case 1:
            printf("t2\n");
            globalPlayers.enumerate(enum_extra_act_update_status, static_cast<void *>(&updatetype2));
            GVAR.AddVar(GVAR_ARENA_EXT_PROCESS, 1);
            break;
        case 2:
            {
            printf("t3\n");

            ValueSort cur;
            ValueSortType resultRank;

            SYSMSGV(title, 739);
            SYSMSGV(content, 740);
            for(UInt8 i = 0; i < 5; i++)
            {

                Player* pl = globalPlayers[World::stArena.playerId[i]];
                if(pl == NULL)
                    continue;
                Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                if(mail)
                {
                    MailPackage::MailItem mitem[1] = {{9076,1}};
                    mailPackageManager.push(mail->id, mitem, 1, true);

                    std::string strItems;
                    for(int index = 0; index < 1; ++ index)
                    {
                        strItems += Itoa(mitem[index].id);
                        strItems += ",";
                        strItems += Itoa(mitem[index].count);
                        strItems += "|";
                    }
                    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, LogArenaExtraAct, title, content, strItems.c_str(), mail->recvTime);
                }
            }

            for(UInt8 i = 0; i < 5; i++)
            {
                cur.sufferCnt = World::stArena.sufferCnt[i];
                cur.lastTime = World::stArena.lasttime[i];
                cur.name = World::stArena.name[i];
                resultRank.insert(cur);
            }
            for(UInt8 i = 0; i < 5; i++)
            {
                UInt8 j = 0;
                for(ValueSortType::iterator iter = resultRank.begin(), e = resultRank.end(); iter != e && j < 5; ++iter, ++j)
                {
                    if((*iter).name == World::stArena.name[i])
                        break;
                }
                stArena.rank[i] = j + 1;
            }

            globalPlayers.enumerate(enum_extra_act_award, static_cast<void *>(NULL));

            stArena.week = week;
            DB1().PushUpdateData("REPLACE INTO `arena_extra_board` (`week`, `name1`, `name2`, `name3`, `name4`, `name5`, `heroId1`, `heroId2`, `heroId3`, `heroId4`, `heroId5`, `sufferTotal`, `sufferCnt1`, `sufferCnt2`, `sufferCnt3`, `sufferCnt4`, `sufferCnt5`, `lasttime1`, `lasttime2`, `lasttime3`, `lasttime4`, `lasttime5`) VALUES(%u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", World::stArena.week, World::stArena.name[0].c_str(), World::stArena.name[1].c_str(), World::stArena.name[2].c_str(), World::stArena.name[3].c_str(), World::stArena.name[4].c_str(), World::stArena.heroId[0], World::stArena.heroId[1], World::stArena.heroId[2], World::stArena.heroId[3], World::stArena.heroId[4], World::stArena.sufferTotal, World::stArena.sufferCnt[0], World::stArena.sufferCnt[1], World::stArena.sufferCnt[2], World::stArena.sufferCnt[3], World::stArena.sufferCnt[4], World::stArena.lasttime[0], World::stArena.lasttime[1], World::stArena.lasttime[2], World::stArena.lasttime[3], World::stArena.lasttime[4]);

            GObject::World::stArenaOld[type].week = GObject::World::stArena.week;
            GObject::World::stArenaOld[type].sufferTotal = GObject::World::stArena.sufferTotal;
            for(UInt8 i = 0; i < 5; i++)
            {
                GObject::World::stArenaOld[type].name[i] = GObject::World::stArena.name[i];
                GObject::World::stArenaOld[type].heroId[i] = GObject::World::stArena.heroId[i];
                GObject::World::stArenaOld[type].sufferCnt[i] = GObject::World::stArena.sufferCnt[i];
                GObject::World::stArenaOld[type].lasttime[i] = GObject::World::stArena.lasttime[i];
                GObject::World::stArenaOld[type].rank[i] = GObject::World::stArena.rank[i];
            }
            GVAR.AddVar(GVAR_ARENA_EXT_PROCESS, 1);
            }
            break;
        default:
            break;

    }
#if 0
    if(now >= t1 && now < t1 + 5)
    {
        printf("t1\n");
        globalPlayers.enumerate(enum_extra_act_update_status, static_cast<void *>(&updatetype));
    }
    else if(now >= t2 && now < t2 + 5)
    {
        printf("t2\n");
        globalPlayers.enumerate(enum_extra_act_update_status, static_cast<void *>(&updatetype2));
    }
    else if(now >= t3 && now < t3 + 5)
    {
        printf("t3\n");

        ValueSort cur;
        ValueSortType resultRank;

        for(UInt8 i = 0; i < 5; i++)
        {
            SYSMSGV(title, 739);
            SYSMSGV(content, 740);

            Player* pl = globalPlayers[World::stArena.playerId[i]];
            if(pl == NULL)
                continue;
            Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[1] = {{9076,1}};
                mailPackageManager.push(mail->id, mitem, 1, true);

                std::string strItems;
                for(int index = 0; index < 1; ++ index)
                {
                    strItems += Itoa(mitem[index].id);
                    strItems += ",";
                    strItems += Itoa(mitem[index].count);
                    strItems += "|";
                }
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, LogArenaExtraAct, title, content, strItems.c_str(), mail->recvTime);
            }
        }

        for(UInt8 i = 0; i < 5; i++)
        {
            cur.sufferCnt = World::stArena.sufferCnt[i];
            cur.lastTime = World::stArena.lasttime[i];
            cur.name = World::stArena.name[i];
            resultRank.insert(cur);
        }
        for(UInt8 i = 0; i < 5; i++)
        {
            UInt8 j = 0;
            for(ValueSortType::iterator iter = resultRank.begin(), e = resultRank.end(); iter != e && j < 5; ++iter, ++j)
            {
                if((*iter).name == World::stArena.name[i])
                    break;
            }
            stArena.rank[i] = j + 1;
        }

        globalPlayers.enumerate(enum_extra_act_award, static_cast<void *>(NULL));

        stArena.week = week;
        DB1().PushUpdateData("REPLACE INTO `arena_extra_board` (`week`, `name1`, `name2`, `name3`, `name4`, `name5`, `heroId1`, `heroId2`, `heroId3`, `heroId4`, `heroId5`, `sufferTotal`, `sufferCnt1`, `sufferCnt2`, `sufferCnt3`, `sufferCnt4`, `sufferCnt5`, `lasttime1`, `lasttime2`, `lasttime3`, `lasttime4`, `lasttime5`) VALUES(%u, '%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)", World::stArena.week, World::stArena.name[0].c_str(), World::stArena.name[1].c_str(), World::stArena.name[2].c_str(), World::stArena.name[3].c_str(), World::stArena.name[4].c_str(), World::stArena.heroId[0], World::stArena.heroId[1], World::stArena.heroId[2], World::stArena.heroId[3], World::stArena.heroId[4], World::stArena.sufferTotal, World::stArena.sufferCnt[0], World::stArena.sufferCnt[1], World::stArena.sufferCnt[2], World::stArena.sufferCnt[3], World::stArena.sufferCnt[4], World::stArena.lasttime[0], World::stArena.lasttime[1], World::stArena.lasttime[2], World::stArena.lasttime[3], World::stArena.lasttime[4]);

        GObject::World::stArenaOld[type].week = GObject::World::stArena.week;
        GObject::World::stArenaOld[type].sufferTotal = GObject::World::stArena.sufferTotal;
        for(UInt8 i = 0; i < 5; i++)
        {
            GObject::World::stArenaOld[type].name[i] = GObject::World::stArena.name[i];
            GObject::World::stArenaOld[type].heroId[i] = GObject::World::stArena.heroId[i];
            GObject::World::stArenaOld[type].sufferCnt[i] = GObject::World::stArena.sufferCnt[i];
            GObject::World::stArenaOld[type].lasttime[i] = GObject::World::stArena.lasttime[i];
            GObject::World::stArenaOld[type].rank[i] = GObject::World::stArena.rank[i];
        }
    }
    else
    {
    }
#endif
}

void World::ClanStatueCheck(void *)
{
    class UpdateStatueVisitor : public Visitor<Clan>
    {
        public:
            UpdateStatueVisitor()
            {
            }

            bool operator()(Clan* clan)
            {
                clan->updateStatueExp();
                return true;
            }

    };
    UpdateStatueVisitor visitor;
    globalClans.enumerate(visitor);
}

#if 0
bool advancedHookEnumerate(Player * pl, UInt8 para)
{
    if(!pl)
        return false;
    GameMsgHdr hdr1(0x256, pl->getThreadId(), pl, 0);
    GLOBAL().PushMsg(hdr1, NULL);
    return true;
}

void World::advancedHookTimer(void *para)
{
	globalPlayers.enumerate(advancedHookEnumerate, static_cast<UInt8>(0));
}
#endif
bool World::Init()
{
    static UInt8 type = 0;
    static UInt8 type2 = 1;
	GObject::Tianjie::instance().Init();
	AddTimer(5 * 1000, Tianjie_Refresh, static_cast<void*>(NULL));
	GObject::DaysRank::instance().Init(); 
	//AddTimer(60 * 1000, DaysRank_Refresh, static_cast<void*>(NULL)); //调试用

	GObjectManager::delayLoad();
	GObjectManager::LoadPracticeData();
	GObjectManager::LoadTripodData();

	std::string path = cfg.scriptPath + "World/main.lua";
	_worldScript = new Script::WorldScript(path.c_str());
	path = cfg.scriptPath + "formula/main.lua";
	_battleFormula = new Script::BattleFormula(path.c_str());
    path = cfg.scriptPath + "shsytmpl.lua";
    shsyTmpl.setFilename(path.c_str());
    shsyTmpl.load();
    path = cfg.scriptPath + "qixitmpl.lua";
    qixiTmpl.setFilename(path.c_str());
    qixiTmpl.load();
    path = cfg.scriptPath + "RechargeTmpl.lua";
    GObject::RechargeTmpl::instance().setFilename(path.c_str());
    GObject::RechargeTmpl::instance().load();


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

	if(!shStageMgr.loadFromDB())
		shStageMgr.init(_now, _wday);

	AddTimer(5000, World_Multi_Check, this);
	AddTimer(60 * 1000, World_One_Min, this);
    AddTimer(60 * 1000 * 5, World_Store_Check);

#ifdef _FB
    if(getJune())
        globalPlayers.enumerate(enum_lucky_draw_rank_list, static_cast<void *>(NULL));
#endif
    if(getQixi() || getWansheng())
        globalPlayers.enumerate(enum_qixi_rank_list, static_cast<void *>(NULL));

	UInt32 now = TimeUtil::Now(), sday = TimeUtil::SharpDay(1) - 10;
	if(sday < now) sday += 86400;
	AddTimer(86400 * 1000, World_Midnight_Check, this, (sday - now) * 1000);
    AddTimer(5 * 60 * 1000, World_Online_Log, static_cast<void *>(NULL), ((now + 300) / 300 * 300 - now) * 1000);
    AddTimer(5 * 1000, World_Boss_Refresh, static_cast<void*>(NULL), 5 * 1000);

    UInt32 athChkPoint = TimeUtil::SharpDayT(0, now) + EXTRAREWARDTM;
    AddTimer(86400 * 1000, World_Athletics_Check, static_cast<void *>(&type), (athChkPoint >= now ? athChkPoint - now : 86400 + athChkPoint - now) * 1000);
    if(cfg.merged)
    {
        athChkPoint += 30 * 60;
        AddTimer(86400 * 1000, World_Athletics_Check, static_cast<void *>(&type2), (athChkPoint >= now ? athChkPoint - now : 86400 + athChkPoint - now) * 1000);
    }
    //AddTimer(5 * 1000, Team_Copy_Process, static_cast<void*>(NULL));
    //AddTimer(3600 * 1000, AthleticsPhysicalCheck, static_cast<void *>(NULL), (3600 - now % 3600) * 1000);
    AddTimer(3600 * 1000, ClanStatueCheck, static_cast<void *>(NULL), (3600 - now % 3600) * 1000);

    UInt32 tdChkPoint = TimeUtil::SharpDayT(0, now) + TOWNDEAMONENDTM;
    AddTimer(86400 * 1000, TownDeamonTmAward, static_cast<void *>(NULL), (tdChkPoint >= now ? tdChkPoint - now : 86400 + tdChkPoint - now) * 1000);

    UInt32 drChkPoint = TimeUtil::SharpDayT(0, now) + DAYSRANKTM;
    AddTimer(86400 * 1000, DaysRank_Refresh, static_cast<void *>(NULL), (drChkPoint >= now ? drChkPoint - now : 86400 + drChkPoint - now) * 1000);

    //AddTimer(60 * 1000, advancedHookTimer, static_cast<void *>(NULL), (60 - now % 60) * 1000);
    AddTimer(5 * 1000, ArenaExtraActTimer, static_cast<void *>(NULL), (5 - now % 5) * 1000);

    UInt32 QQGameGiftPoint = TimeUtil::SharpDayT(0, now) + 20*3600;
    AddTimer(86400 * 1000, SendQQGameGift, static_cast<void *>(NULL), (QQGameGiftPoint >= now ? QQGameGiftPoint - now : 86400 + QQGameGiftPoint - now) * 1000);
    
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
#else
#endif
}

void World::commitArenaForceOnce()
{
    GObject::arena.commitArenaForceOnce();
}

void World::LoadQixiScore(Player* pl, Player* lover)
{
    if(!pl->queQiaoCheck())
        return;
    if(lover->getLover() != pl)
    {
        pl->resetQixi();
        return;
    }

    QixiScoreMap::iterator it = _qixiScoreMap.find(pl);
    if(it == _qixiScoreMap.end())
    {
        QixiPair* qp = new QixiPair();
        qp->p1.lover = lover;
        qp->p1.score = pl->getScore();
        qp->p2.lover = pl;
        qp->p2.score = lover->getScore();
        QixiPlayersIt qpIt = _qixiPlayerSet.insert(qp);
        _qixiScoreMap[pl] = qpIt;
        _qixiScoreMap[lover] = qpIt;
    }
}


void World::UpdateQixiScore(Player* pl, Player* lover)
{
    if(!pl->queQiaoCheck())
        return;
    QixiScoreMap::iterator it = _qixiScoreMap.find(pl);
    UInt32 myPlace = 0;
    if(it == _qixiScoreMap.end())
    {
        QixiPair* qp = new QixiPair();
        qp->p1.lover = lover;
        qp->p1.score = pl->getScore();
        qp->p2.lover = pl;
        qp->p2.score = lover->getScore();
        QixiPlayersIt qpIt = _qixiPlayerSet.insert(qp);
        _qixiScoreMap[pl] = qpIt;
        _qixiScoreMap[lover] = qpIt;
        myPlace = std::distance(_qixiPlayerSet.begin(), qpIt) + 1;
    }
    else
    {
        QixiPlayersIt qpIt = it->second;
        QixiPair* qp = *(qpIt);

        _qixiPlayerSet.erase(qpIt);
        if(qp->p1.lover == lover)
            qp->p1.score = pl->getScore();
        else if(qp->p2.lover == lover)
            qp->p2.score = pl->getScore();

        QixiPlayersIt qpIt2 = _qixiPlayerSet.insert(qp);
        _qixiScoreMap[pl] = qpIt2;
        _qixiScoreMap[lover] = qpIt2;
        myPlace = std::distance(_qixiPlayerSet.begin(), qpIt2) + 1;
    }

    if(0 != myPlace && myPlace < 4)
    {
        globalPlayers.enumerate(enum_qixi, static_cast<void *>(NULL));
    }
    else
    {
        sendQixiPlayers(pl);
        sendQixiPlayers(lover);
    }
}

void World::sendQixiPlayers(Player* pl)
{
    UInt8 i = 0;
    UInt32 myPlace = 0;
    UInt32 myScore = 0;
    QixiScoreMap::iterator it = _qixiScoreMap.find(pl);
    if(it != _qixiScoreMap.end())
    {
        QixiPlayersIt qpIt = it->second;
        myPlace = std::distance(_qixiPlayerSet.begin(), qpIt) + 1;
        QixiPair* qp = *qpIt;
        myScore = qp->p1.score + qp->p2.score;
    }
    else
    {
        myScore = pl->getScore();
    }

    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x02);
    st << myPlace << myScore;
    size_t offset = st.size();

    st << i;
    for(QixiPlayersIt qpIt = _qixiPlayerSet.begin(); i < 3 && qpIt != _qixiPlayerSet.end(); ++ qpIt, ++ i)
    {
        QixiPair* qp = *qpIt;
        st << qp->p1.lover->getName() << qp->p2.lover->getName() << static_cast<UInt32>(qp->p1.score + qp->p2.score);
    }
    st.data<UInt8>(offset) = i;
    st << Stream::eos;

    pl->send(st);
}

void World::DivorceQixiPair(Player* pl)
{
    QixiScoreMap::iterator it = _qixiScoreMap.find(pl);
    Player* lover1 = NULL;
    Player* lover2 = NULL;
    UInt32 myPlace = 0;
    if(it == _qixiScoreMap.end())
    {
        return;
    }
    else
    {
        QixiPlayersIt qpIt = it->second;
        QixiPair* qp = *(qpIt);
        myPlace = std::distance(_qixiPlayerSet.begin(), qpIt) + 1;

        _qixiPlayerSet.erase(qpIt);
        _qixiScoreMap.erase(qp->p1.lover);
        _qixiScoreMap.erase(qp->p2.lover);
        lover1 = qp->p1.lover;
        lover2 = qp->p2.lover;
        delete qp;
    }

    if(0 != myPlace && myPlace < 4)
    {
        globalPlayers.enumerate(enum_qixi, static_cast<void *>(NULL));
    }
    else
    {
        sendQixiPlayers(lover1);
        sendQixiPlayers(lover2);
    }

}

void World::SendQixiAward()
{
    UInt32 pos = 0;

    globalPlayers.enumerate(enum_qixi_score, static_cast<void *>(NULL));
    std::vector<MailPackage::MailItem> mitems;
    SYSMSG(title, qixiTmpl._lbTitle);
    for(QixiPlayersIt qpIt = _qixiPlayerSet.begin(); qpIt != _qixiPlayerSet.end() && pos < 99; ++ qpIt, ++ pos)
    {
        mitems.clear();
        for(size_t idx = 0; idx < qixiTmpl._lbAwards.size(); ++ idx)
        {
            if(pos < qixiTmpl._lbAwards[idx].lbPos)
            {
                mitems = qixiTmpl._lbAwards[idx].items;
                break;
            }
        }

        UInt16 mitemNum = mitems.size();
        if(mitemNum > 0)
        {
            QixiPair* qp = *(qpIt);
            Player* player[2];
            player[0] = qp->p1.lover;
            player[1] = qp->p2.lover;
            for(int idx = 0; idx < 2; ++ idx)
            {
                Player* pl = player[idx];
                Player* lover = pl->getLover();
                SYSMSGV(content, qixiTmpl._lbContent, lover->getName().c_str(), pos+1, pos+1);
                Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                if(mail)
                {
                    std::string strItems;
                    for(int i = 0; i < mitemNum; ++ i)
                    {
                        MailPackage::MailItem mitem;
                        bool bind = true;
                        if(mitems[i].id == qixiTmpl._titleItem)
                        {
                            mitem.id = mitems[i].id + (pl->GetClassAndSex() & 0x0F);
                            bind = false;
                        }
                        else
                            mitem.id = mitems[i].id;
                        mitem.count = mitems[i].count;
                        mailPackageManager.push(mail->id, &mitem, 1, bind);

                        strItems += Itoa(mitem.id);
                        strItems += ",";
                        strItems += Itoa(mitem.count);
                        strItems += "|";
                    }

                    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
                }
            }
        }
    }
}

void World::sendQixiScoreAward(Player* pl)
{
    if(pl->queQiaoCheck())
    {
        SYSMSG(title, qixiTmpl._partnerTitle);
        Player* lover = pl->getLover();
        SYSMSGV(content, qixiTmpl._partnerContent, lover->getName().c_str());

        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        std::vector<MailPackage::MailItem>& mitem = qixiTmpl._partnerAward;
        mailPackageManager.push(mail->id, &mitem[0], mitem.size(), true);

        std::string strItems;
        for(size_t i = 0; i < mitem.size(); ++ i)
        {
            strItems += Itoa(mitem[i].id);
            strItems += ",";
            strItems += Itoa(mitem[i].count);
            strItems += "|";
        }
        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
    }

    UInt32 score = pl->getScore();
    if(score < qixiTmpl._minAwardScore)
        return;

    SYSMSG(title, qixiTmpl._scoreTitle);
    SYSMSGV(content, qixiTmpl._scoreContent, score);

    std::string strItems;
    Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);

    for(size_t idx = 0; idx < qixiTmpl._scoreAwards.size(); ++ idx)
    {
        if(score >= qixiTmpl._scoreAwards[idx].score)
        {
            std::vector<MailPackage::MailItem>& mitem = qixiTmpl._scoreAwards[idx].items;
            mailPackageManager.push(mail->id, &mitem[0], mitem.size(), true);

            for(size_t i = 0; i < mitem.size(); ++ i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            break;
        }
    }

    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
}

struct SSelectYuebingUsedMost : public Visitor<Player>
{
    Player* _player;
    UInt32 _used;
    SSelectYuebingUsedMost() : _player(NULL), _used(0) {}
    bool operator()(Player* player)
    {
        UInt32 used = player->GetVar(VAR_YUEBING_USED);
        if(_player == NULL || used > _used)
        {
            _player = player;
            _used = used;
        }
        return true;
    }
};
void World::SendGuoqingAward()
{
    SSelectYuebingUsedMost selector;
    globalPlayers.enumerate(selector);
    if(selector._player == NULL)
        return;
    MailPackage::MailItem items[] =
    {
        {9180, 1}
    };
    selector._player->sendMailItem(4031, 4032, items, sizeof(items)/sizeof(items[0]), false);


}
struct SSelectXiaoyaoUsedMost : public Visitor<Player>
{
    Player* _player;
    UInt32 _used;
    SSelectXiaoyaoUsedMost() : _player(NULL), _used(0) {}
    bool operator()(Player* player)
    {
        UInt32 used = player->GetVar(VAR_9215_USED);
        if(_player == NULL || used > _used)
        {
            _player = player;
            _used = used;
        }
        return true;
    }
};
void World::SendXiaoyaoAward()
{
    SSelectXiaoyaoUsedMost selector;
    globalPlayers.enumerate(selector);
    if(selector._player == NULL)
        return;
    MailPackage::MailItem items[] =
    {
        {9216, 1}
    };
    selector._player->sendMailItem(4062, 4063, items, sizeof(items)/sizeof(items[0]), false);
}


#ifndef _WIN32
void World::udpLog(const char* str1, const char* str2, const char* str3, const char* str4,
    const char* str5, const char* str6, const char* type)
{
    if (!ulog && cfg.udplog)
        ulog = _analyzer.GetInstance("world");
    if (ulog && cfg.udplog)
    {
        ulog->LogMsg(str1, str2, str3, str4, str5, str6, type);
        TRACE_LOG("(%s,%s,%s,%s,%s,%s,%s)", str1, str2, str3, str4, str5, str6, type);
    }
}
#endif

inline bool player_enum_rc(GObject::Player * p, int)
{
    //using namespace GObject;
    if (World::getRechargeActive() || World::getRechargeActive3366())
    {
        UInt32 total;
        if(World::getRechargeActive())
            total = p->GetVar(VAR_RECHARGE_TOTAL);
        else
            total = p->GetVar(VAR_RECHARGE_TOTAL3366);
        if (total)
        {
            RCSort s;
            s.player = p;
            s.total = total;
            World::rechargeSort.insert(s);
        }
    }
    if (World::getConsumeActive())
    {
        UInt32 total = p->GetVar(VAR_CONSUME);
        if (total)
        {
            RCSort s;
            s.player = p;
            s.total = total;
            World::consumeSort.insert(s);
        }
    }
    return true;
}

static bool init = false;
void World::initRCRank()
{
    if (init)
        return;
    GObject::globalPlayers.enumerate(player_enum_rc, 0);
    init = true;
}

inline bool player_enum_killmonster(GObject::Player * p, int)
{
    using namespace GObject;
    if (World::getKillMonsterAct())
    {
        UInt32 total;
        RCSort s;
        total = p->GetVar(VAR_XIAGU_CNT);
        if (total)
        {
            s.player = p;
            s.total = total;
            World::killMonsterSort[0].insert(s);
        }
        total = p->GetVar(VAR_ROUQING_CNT);
        if (total)
        {
            s.player = p;
            s.total = total;
            World::killMonsterSort[1].insert(s);
        }
        total = p->GetVar(VAR_CAIFU_CNT);
        if (total)
        {
            s.player = p;
            s.total = total;
            World::killMonsterSort[2].insert(s);
        }
        total = p->GetVar(VAR_CHUANQI_CNT);
        if (total)
        {
            s.player = p;
            s.total = total;
            World::killMonsterSort[3].insert(s);
        }
    }
    return true;
}

#define RANK_CNT 1
void World::killMonsterAppend(Stream& st, UInt8 index)
{
    using namespace GObject;
    if(index > 3)
        return;

    UInt8 cnt = killMonsterSort[index].size();
    if (cnt > RANK_CNT)
        cnt = RANK_CNT;
    UInt32 c = 0;
    for (RCSortType::iterator i = killMonsterSort[index].begin(), e = killMonsterSort[index].end(); i != e; ++i)
    {
        st << i->player->getName();
        st << i->total;
        ++c;
        if (c >= RANK_CNT)
            break;
    }
    if(c == 0)
    {
        std::string nullName;
        st << nullName;
        st << c;
    }
}

void World::killMonsterInit()
{
    static bool sortInit = false;
    if(!sortInit)
    {
        sortInit = true;
        GObject::globalPlayers.enumerate(player_enum_killmonster, 0);
    }
}

void World::UpdateKillMonsterRank(Player* pl, UInt8 type, UInt8 count)
{
    Stream st(REP::COUNTRY_ACT);
    UInt8 subType = 0x02;
    st << subType;
    UInt8 subType2 = 0x01;
    st << subType2;
    st << pl->GetVar(VAR_ZYCM_POS);
    st << static_cast<UInt8>(pl->GetVar(VAR_ZYCM_TIPS));
    st << type;
    UInt32 curCnt;
    if(type == 1)
    {
        curCnt = pl->GetVar(VAR_XIAGU_CNT);
    }
    else if(type == 2)
    {
        curCnt = pl->GetVar(VAR_ROUQING_CNT);
    }
    else if(type == 3)
    {
        curCnt = pl->GetVar(VAR_CAIFU_CNT);
    }
    else if(type == 4)
    {
        curCnt = pl->GetVar(VAR_CHUANQI_CNT);
    }
    else
        curCnt = 0;
    st << curCnt;
    st << Stream::eos;
    pl->send(st);

    if(type < 1 || type > 4)
        return;

    UInt8 index = type -1;
    for (RCSortType::iterator i = World::killMonsterSort[index].begin(), e = World::killMonsterSort[index].end(); i != e; ++i)
    {
        if (i->player == pl)
        {
            World::killMonsterSort[index].erase(i);
            break;
        }
    }
    RCSort s;
    s.player = pl;
    s.total = curCnt;
    World::killMonsterSort[index].insert(s);

    UInt32 newrank = 0;
    for (RCSortType::iterator i = World::killMonsterSort[index].begin(), e = World::killMonsterSort[index].end(); i != e; ++i)
    {
        ++newrank;
        if (i->player == pl)
            break;
    }

    if (newrank <= RANK_CNT)
    {
        Stream st(REP::COUNTRY_ACT);
        UInt8 subType = 0x02;
        st << subType;
        UInt8 subType2 = 2;
        st << subType2;
        st << static_cast<UInt8>(index + 1);
        killMonsterAppend(st, index);
        st << Stream::eos;
        NETWORK()->Broadcast(st);
    }
}

}


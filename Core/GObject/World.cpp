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
#include "NewHeroIsland.h"
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
#include "ArenaTeam.h"
#include "ArenaServerWar.h"
#include "Tianjie.h"
#include "DaysRank.h"
#include "TownDeamon.h"
#include "SingleHeroStage.h"
#include "SHSYTmpl.h"
#include "QixiTmpl.h"
#include "LBNameTmpl.h"
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
bool World::_accrecharge = false;
bool World::_newyear = false;
bool World::_blueactiveday = false;
bool World::_rechargeactive = false;
bool World::_rechargeactive3366 = false;
bool World::_yearact = false;
bool World::_qgamegiftact = false;
bool World::_spreadAct = false;
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
bool World::_qzongpygiftact = false;
void* World::_recalcwd = NULL;
void* World::_swBosstimer = NULL;
bool World::_june = false;
bool World::_june1 = false;
bool World::_july = false;
bool World::_qixi= false;
bool World::_foolbao = false;
bool World::_summerFlow3 = false;
bool World::_halfgold = false;
bool World::_qqBoardLogin = false;
bool World::_surnamelegend = false;
bool World::_11time = false;
bool World::_ggtime = false;
bool World::_qzoneRechargetime = false;
bool World::_ryhbActivity = false;
bool World::_zcjbActivity = false;
bool World::_wansheng= false;
bool World::_qingren= false;
bool World::_specialbook= false;
bool World::_11Act= false;
bool World::_ssToolbarAct= false;
bool World::_snowAct= false;
bool World::_goldSnakeAct= false;
bool World::_collectCardAct= false;
bool World::_halloweenAct= false;
bool World::_heroIslandAct= false;
bool World::_dragonKingAct= false;
bool World::_saveGoldAct= false;
bool World::_feastloginAct= false;
bool World::_newYearGiveGiftAct= false;
bool World::_newYearQQGameAct= false;
bool World::_QZoneQQGameAct= false;
bool World::_newYearQzoneContinueAct= false;
bool World::_maydayloginAct= false;
UInt8 World::_towerloginAct= 0;
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
bool World::_consumeawardact = false;
bool World::_summerFlow = false;
bool World::_summerMeet = false;
bool World::_qishiban = false;
RCSortType World::qishibanScoreSort;
RCSortType World::rechargeSort;
RCSortType World::consumeSort;
RCSortType World::popularitySort;
RCSortType World::LuckyBagSort;
RCSortType World::PlayerGradeSort;
RCSortType World::guangGunSort;
ClanGradeSort World::clanGradeSort;
bool World::_needrechargerank = false;
bool World::_needconsumerank = false;
bool World::_killMonsteract = 0;
RCSortType World::rechargeRP7Sort;
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
bool World::_weiboact = false;
bool World::_qqgameonlineawardact = false;
bool World::_peteggact = false;
bool World::_longyuanact = false;
bool World::_compassact = false;
UInt8 World::_callsnakeeggact = 0;
UInt8 World::_snakeeggawardact = 0;
bool World::_item9344act = false;
bool World::_item9343act = false;
bool World::_autobattleact = false;
UInt8 World::_snakespringequipact = 0;
/** 场外活动 **/
stArenaExtra World::stArenaOld[2];
stArenaExtra World::stArena;
/** 0：侠骨；1：柔情；2财富；3传奇 **/
RCSortType World::killMonsterSort[4];
UInt8 World::m_sysDailogPlatform = SYS_DIALOG_ALL_PLATFORM;
Player* World::spreadKeeper = NULL;
UInt32 World::spreadBuff = 0;
UInt8 World::_arenaState = 0;      //0:无 1:仙界第一 2:仙界至尊
bool World::_memcinited = false;
bool World::_miluzhijiao = false;

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
void World::ServerWarBoss_Refresh( World * world )
{
    serverWarBoss.process(TimeUtil::Now());
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
bool bQingrenEnd = false;
bool bGuoqingEnd = false;
bool bRechargeEnd = false;
bool bConsumeEnd = false;
bool bXiaoyaoEnd = false;
bool bFoolBaoEnd =  false;
bool bHalfGoldEnd = false;
bool bSurnameLegendEnd = false;
bool b11TimeEnd = false;
bool bGGTimeEnd = false;
bool bSnowEnd = false;
bool bGoldSnakeEnd = false;
bool bItem9344End = false;
bool bItem9343End = false;
bool bQiShiBanEnd = false;

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

    if (pl->GetVar(VAR_RECHARGE_TOTAL) && (TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 10, 5)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 10, 6)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 1)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 2)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 3)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 4)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 5)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 6)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 7)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 8)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 9)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 10)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 11)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 12)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 13)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 14)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 15)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 16)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 17)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 18)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 19)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 20)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 21)


         || (cfg.rpServer && (TimeUtil::SharpDay(0, nextday) <= World::getOpenTime()+7*86400))
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

    if (pl->GetVar(VAR_RECHARGE_CONDCNT) &&
        (TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 5, 25)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 6, 1)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 6, 8)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 6, 15)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 6, 22)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 6, 29)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 7, 6)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 7, 13)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 7, 20)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 7, 27)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 8, 3)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 8, 10)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 8, 17)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 8, 24)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 8, 31)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 9, 7)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 9, 14)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 9, 21)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 9, 28)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 10, 5)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 10, 12)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 10, 19)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 10, 26)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 11, 2)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 11, 9)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 11, 16)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 11, 23)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 11, 30)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 7)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 14)
        || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 21)
        ))
    {
#if 0
        if (pl->isOnline())
        {
            GameMsgHdr hdr(0x284, pl->getThreadId(), pl, 0);
            GLOBAL().PushMsg(hdr, NULL);
        }
        else
        {
            if (pl->GetVar(VAR_RECHARGE_CONDCNT))
                pl->SetVar(VAR_RECHARGE_CONDCNT, 0);
        }
#else
        if (pl->GetVar(VAR_RECHARGE_CONDCNT))
        {
            pl->SetVar(VAR_RECHARGE_CONDCNT_LAST, pl->GetVar(VAR_RECHARGE_CONDCNT));
            pl->SetVar(VAR_RECHARGE_CONDCNT, 0);
        }
        else
        {
            if (pl->GetVar(VAR_RECHARGE_CONDCNT_LAST))
                pl->SetVar(VAR_RECHARGE_CONDCNT_LAST, 0);
        }
#endif
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

    /*
    if (nextday >= TimeUtil::MkTime(2013, 2, 9))
    {   //金蛇献瑞 聚福兆祥活动
        struct goldData
        {
            UInt8 opt;
            UInt32 param;
        }gData;
        gData.opt = 0;
        gData.param = 0;
        GameMsgHdr h(0x343,  pl->getThreadId(), pl, sizeof(gData));
        GLOBAL().PushMsg(h, &gData);
    }
    */

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
bool enum_snow(void * ptr, void * data)
{
	Player * pl = static_cast<Player *>(ptr);
	if(pl == NULL)
		return true;
    WORLD().sendSnowPlayers(pl);
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

bool enum_teamArena_midnight(void * ptr, void * data)
{
	TeamArenaData * tad = static_cast<TeamArenaData *>(ptr);
	if (tad == NULL)
		return true;
	UInt32 now = *reinterpret_cast<UInt32 *>(data);
    tad->checkTimeOver(now);
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
bool enum_snow_rank_list(void * ptr, void * data )
{
    Player* player = static_cast<Player*>(ptr);
    if(player == NULL || !player->getSnowBind())
        return true;

    Player* lover = player->getSnowLover();
    WORLD().LoadSnowScore(player, lover);
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
#if 0
bool enum_spread_count(Player* pl, void *data)
{
	if(pl == NULL)
		return true;
    if(pl->GetVar(VAR_SPREAD_FLAG) & SPREAD_ALREADY_USE)
        ++World::spreadCount;
    return true;
}
#endif
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
        int yapos = 0;

        struct RCTotal
        {
            Player* player;
            int total;
        };
        struct RCRank
        {
            int pos;
            int total;
            RCTotal f7[7];
        } rcrank;

        memset(&rcrank, 0x00, sizeof(rcrank));

        for (RCSortType::iterator i = World::rechargeSort.begin(), e = World::rechargeSort.end(); i != e; ++i)
        {
            ++yapos;

            if (yapos > 7)
                break;

            Player* player = i->player;
            if (!player)
                continue;

            rcrank.f7[yapos-1].player = player;
            rcrank.f7[yapos-1].total = i->total;
        }

        int pos = 0;
        for (RCSortType::iterator i = World::rechargeSort.begin(), e = World::rechargeSort.end(); i != e; ++i)
        {
            ++pos;

            Player* player = i->player;
            if (!player)
                continue;

            rcrank.pos = pos;
            rcrank.total = i->total;

            GameMsgHdr hdr(0x257, player->getThreadId(), player, sizeof(rcrank));
            GLOBAL().PushMsg(hdr, &rcrank);

            if (pos <= 7)
                SYSMSG_BROADCASTV(4033, pos, player->getCountry(), player->getPName(), i->total);

            char id[1024] = {0};
            char ctx[1024] = {0};
            snprintf(id, sizeof(id), "F_10000_1213_%u_%d", cfg.serverNum, pos);
            snprintf(ctx, sizeof(ctx), "%" I64_FMT "u_%s_%u", player->getId(), player->getRealName().c_str(), i->total);
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
            //TRACE_LOG("CONSUME RANK: %s\t\t\t%" I64_FMT "u\t\t\t%s\t\t\t%u", pos, player->getId(), player->getPName(), i->total);
        }
        World::consumeSort.clear();
    }
}

void World::SendSurnameLegendAward()
{
    if(bSurnameLegendEnd)
    {
        World::initRCRank();
        int pos = 0;
        for (RCSortType::iterator i = World::LuckyBagSort.begin(), e = World::LuckyBagSort.end(); i != e; ++i)
        {
            ++pos;

            if(pos > 1) break;

            Player* player = i->player;
            if (!player)
                continue;
            MailPackage::MailItem items[] =
            {
                //{9907, 1}
                //{9911, 1}
                //{9913, 1}
                //{9921, 1}
                {9926, 1}
            };
            player->sendMailItem(4175, 4176, items, sizeof(items)/sizeof(items[0]), false);
        }
        World::LuckyBagSort.clear();
    }
}
void World::SendPopulatorRankAward(void*)
{
    World::initRCRank();
    RCSortType::iterator i = World::popularitySort.begin();
    if (i == World::popularitySort.end())
        return;

    Player* player = i->player;
    if (!player)
        return;


    char id[1024] = {0};
    char ctx[1024] = {0};
    snprintf(id, sizeof(id), "F_10000_pop_%u_%d", cfg.serverNum, i->total);
    snprintf(ctx, sizeof(ctx), "%" I64_FMT "u_%s_%u", player->getId(), player->getPName(), i->total);
    player->setTitle(201, 7 * 3600 * 24);
    SYSMSGV(title, 4145, TimeUtil::Month(), TimeUtil::MonthDay());
    SYSMSGV(content, 4146, TimeUtil::Month(), TimeUtil::MonthDay(), i->total);
    player->GetMailBox()->newMail(NULL, 0x01, title, content);
    SYSMSG_BROADCASTV(4147, player->getCountry(), player->getPName());
    World::udpLog(id, ctx, "", "", "", "", "act");
    World::popularitySort.clear();
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

inline bool player_enum_3(GObject::Player* pl, int)
{
    pl->CleanQiShiBan(1);

    return true;
}

void World::World_Midnight_Check( World * world )
{
	UInt32 curtime = TimeUtil::Now();

    if (World::_wday == 7)
        GData::store.resetDistcount();

    bool bSingleDay = getSingleDay();
    bool bValentineDay = getValentineDay();
    bool bMayDay = getMayDay();
    bool bfoolbao = getFoolBao();
    bool bsurnamelegend = getSurnameLegend();
    bool b11time = get11Time();
    bool bGGtime = getGGTime();
    bool bhalfgold = getHalfGold();
    bool bJune = getJune();
    bool bQixi = getQixi();
    bool bWansheng = getWansheng();
    bool bQingren = getQingren();
    bool bGuoqing = getGuoqing();
    bool bXiaoyao = get9215Act();
    bool bSnowAct = getSnowAct();
    bool bGoldSnakAct = getGoldSnakeAct();
    bool bRecharge = (getRechargeActive() || getRechargeActive3366()) && getNeedRechargeRank();
    bool bConsume = getConsumeActive() && getNeedConsumeRank();
    bool bPExpItems = getPExpItems();
    bool bMonsterAct = getKillMonsterAct();
    bool bItem9344 = getItem9344Act();
    bool bItem9343 = getItem9343Act();
    bool bQiShiBanTime = getQiShiBanTime();

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
    //愚公宝箱是否结束
    bFoolBaoEnd =  bfoolbao && !getFoolBao(); 
   // 
    bHalfGoldEnd = bhalfgold && !getHalfGold();
    //蜀山传奇掉落活动是否结束
    bSurnameLegendEnd = bsurnamelegend && !getSurnameLegend(300);
    b11TimeEnd = b11time && !get11Time();
    //七石斗法活动结束
    bQiShiBanEnd = bQiShiBanTime && !getQiShiBanTime(300);
    bGGTimeEnd = bGGtime && !getGGTime();

    bPExpItemsEnd = bPExpItems && !getPExpItems();
    bQixiEnd = bQixi && !getQixi();
    bWanshengEnd = bWansheng && !getWansheng();
    bQingrenEnd = bQingren && !getQingren();
    bGuoqingEnd = bGuoqing && !getGuoqing();
    bXiaoyaoEnd = bXiaoyao && !get9215Act();
    bSnowEnd = bSnowAct && !getSnowAct(300);
    bGoldSnakeEnd = bGoldSnakAct && !getGoldSnakeAct();
    bRechargeEnd = bRecharge && !(getRechargeActive()||getRechargeActive3366());
    bConsumeEnd = bConsume && !getConsumeActive();
    bItem9344End = bItem9344 && !getItem9344Act();
    bItem9343End = bItem9343 && !getItem9343Act();

    bool bMonsterActEnd = bMonsterAct && !getKillMonsterAct();
    UInt32 nextday = curtime + 30;

    if (TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 10, 5)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 1)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 2)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 3)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 4)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 5)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 6)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 7)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 8)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 9)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 10)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 11)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 12)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 13)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 14)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 15)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 16)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 17)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 18)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 19)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 20)
         || TimeUtil::SharpDay(0, nextday) == TimeUtil::MkTime(2013, 12, 21)
         )
        bRechargeEnd = true;
    if (cfg.rpServer)
    {
        if (TimeUtil::SharpDay(0, nextday) <= getOpenTime()+7*86400)
            bRechargeEnd = true;
    }

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
    if(bQixiEnd || bWanshengEnd || bQingrenEnd)
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
    if(bFoolBaoEnd)
        world->SendFoolBaoAward();
    if(bSurnameLegendEnd)
        world->SendSurnameLegendAward();
    if(b11TimeEnd)
        world->Send11AirBookAward();
    if(bGGTimeEnd)
        world->SendGuangGunAward();
    if (bSnowEnd)
        world->SendSnowAward();
    if (bGoldSnakeEnd)
        world->SendGoldSnakeAward();
    if (bItem9344End)
        world->SendItem9344Award();
     if (bItem9343End)
        world->SendItem9343Award();
     if (bQiShiBanEnd)
        world->SendQiShiBanAward();
 
	dungeonManager.enumerate(enum_dungeon_midnight, &curtime);
	globalClans.enumerate(enum_clan_midnight, &curtime);
	clanManager.reConfigClanBattle();
	challengeCheck.clear();
	globalTeamArena.enumerate(enum_teamArena_midnight, &curtime);
    
    if(World::getQiShiBanTime())
        GObject::globalPlayers.enumerate(player_enum_3, 0);

	calWeekDay(world);
	Stream st(REP::DAILY_DATA);
	makeActivityInfo(st);
	NETWORK()->Broadcast(st);
    World_CreateNewDB_Check();
    worldBoss.resetBossSt();
    globalCountryBattle.setStatus(0);
    ClanRankBattleMgr::Instance().setStatus(0);
    //heroIsland.setStatus(0);
    newHeroIsland.setStatus(0);
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

    if(!GObject::arena.active())
        return;
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
                    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, LogArenaExtraAct, title, content, strItems.c_str(), mail->recvTime);
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
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, LogArenaExtraAct, title, content, strItems.c_str(), mail->recvTime);
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

inline static bool enum_spread_send(Player* player, void* data)
{
    if(player == NULL || !player->isOnline())
        return true;
    player->sendSpreadBasicInfo();
    return true;
}

void SpreadCheck(void* data)
{
    if (!World::getSpreadAct())
        return;
	UInt32 now = TimeUtil::Now();
    UInt8 week = TimeUtil::GetWeekDay(now);
    if(week < SPREAD_START_WEEK || week > SPREAD_END_WEEK)
        return;
    UInt32 startTime = TimeUtil::SharpDayT(0, now) + SPREAD_START_TIME;
    UInt32 flag;
    if(now >= startTime && ((flag = GVAR.GetVar(GVAR_SPREAD_CONDITION)) & 0xFF) == 0)
    {
        flag += 1;
        GVAR.SetVar(GVAR_SPREAD_CONDITION, flag);
        globalPlayers.enumerate(enum_spread_send, static_cast<void *>(NULL));
    }
    UInt32 endTime = TimeUtil::SharpDayT(0, now) + SPREAD_END_TIME;
    if(now >= endTime && ((flag = GVAR.GetVar(GVAR_SPREAD_CONDITION)) &0xFF) == 1)
    {
        flag += 1;
        GVAR.SetVar(GVAR_SPREAD_CONDITION, flag);
        globalPlayers.enumerate(enum_spread_send, static_cast<void *>(NULL));
    }
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
    MemCachInit();
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

    path = cfg.scriptPath + "lbnametmpl.lua";
    lbnameTmpl.setFilename(path.c_str());
    lbnameTmpl.load();

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
    if(getQixi() || getWansheng() || getQingren())
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
    UInt32 sweek = TimeUtil::SharpWeek(1);
    AddTimer(3600 * 24 * 7 * 1000, SendPopulatorRankAward, static_cast<void * >(NULL), (sweek - now - 10) * 1000);
	AddTimer(5 * 1000, SpreadCheck, static_cast<void *>(NULL), (5 - now % 5) * 1000);
    
    //开服战世界boss
    UInt32 value = GVAR.GetVar(GVAR_SERVERWAR_XIUWEI);
    UInt32 overTime = GVAR.GetOverTime(GVAR_SERVERWAR_XIUWEI);
    if(value == SERVERWAR_VALUE_XIUWEI5 && (overTime - TimeUtil::SharpDayT(0, now)) > 7*86400)
        WORLD()._swBosstimer = WORLD().AddTimer(5000, WORLD().ServerWarBoss_Refresh, &(WORLD()), 10000);
    
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
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, (*rank)->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, (*rank)->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, (*rank)->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, (*rank)->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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

                    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
                }
            }
        }
    }
}

void World::UpdateGuangGunScore(Player* pl)//GG
{
    Player* player=pl;
    if(pl->getGGStatus()==2)
        player = pl->getGGTimeCaptain();
    UInt32 score = player->getGGTimeScore();
    for (RCSortType::iterator i = guangGunSort.begin(), e = guangGunSort.end(); i != e; ++i)
    {
        if (i->player == player)
        {
            guangGunSort.erase(i);
            break;
        }
    }
    RCSort s;
    s.player = player;
    s.total = score;
    guangGunSort.insert(s);
    sendGuangGunPlayers(pl); 
}
void World::sendGuangGunPlayers(Player* pl)
{
    Player* player=pl;     //player表示队长   pl是请求人
    if(pl->getGGStatus()==2)
        player = pl->getGGTimeCaptain();
    UInt32 myPlace = 0;
    UInt32 myScore = 0;
    UInt8 rank = 0;
    for (RCSortType::iterator i = World::guangGunSort.begin(), e = World::guangGunSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->player == player)
        {
            Stream st(REP::ACT);//GG
            st << static_cast<UInt8>(0x20) << static_cast<UInt8>(1) << static_cast<UInt8>(2);
            st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
            pl->send(st);
            break;
        }
    }

    Stream st(REP::ACTIVE);   //GG
    st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x02);
    st << myPlace << myScore;
    size_t offset = st.size();
    UInt8 i =0;
    st << i;
    for(RCSortType::iterator it = World::guangGunSort.begin(), e = World::guangGunSort.end(); it != e&&i<3; ++it,++i)
    {
        Player *p = it->player;
        st << p ->getGGTimeScore();
        st << p->getName();
        if(!p->getGGPlayer1())
            st <<p->getGGPlayer1()->getName();
        else
            st<<"";
        if(!p->getGGPlayer2())
            st <<p->getGGPlayer2()->getName();
        else 
            st<<"";
    }
    st.data<UInt8>(offset) = i;
    st << Stream::eos;
    pl->send(st);
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
        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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

    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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
struct SSelectFoolBaoUsedMost : public Visitor<Player>
{
    Player* _player;
    UInt32 _used;
    SSelectFoolBaoUsedMost() : _player(NULL), _used(0) {}
    bool operator()(Player* player)
    {
        UInt32 used = player->GetVar(VAR_FOOLBAO_USED);
        if(_player == NULL || used > _used)
        {
            _player = player;
            _used = used;
        }
        return true;
    }
};
void World::SendFoolBaoAward()
{
    SSelectFoolBaoUsedMost selector;
    globalPlayers.enumerate(selector);
    if(selector._player == NULL)
        return;
    MailPackage::MailItem items[] =
    {
        {9902, 1}
    };
    selector._player->sendMailItem(4142, 4143, items, sizeof(items)/sizeof(items[0]), false);
}

/*struct SSelectSurnameLegendUsedMost : public Visitor<Player>
{
    Player* _player;
    UInt32 _used;
    SSelectSurnameLegendUsedMost(): _player(NULL), _used(0) {}
    bool operator()(Player* player)
    {
        UInt32 used = player->GetVar(VAR_SURNAMELEGEND_USED);
        if(_player == NULL || used > _used)
        {
            _player = player;
            _used = used;
        }
        return true;
    }
};
void World::SendSurnameLegendAward()
{
     SSelectSurnameLegendUsedMost  selector;
    globalPlayers.enumerate(selector);
    if(selector._player == NULL)
        return;
    MailPackage::MailItem items[] =
    {
        {9904, 1}
    };
    selector._player->sendMailItem(4151, 4152, items, sizeof(items)/sizeof(items[0]), false);
}
*/
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

struct SSelectItem9344UsedMost : public Visitor<Player>
{
    Player* _player;
    UInt32 _used;
    SSelectItem9344UsedMost() : _player(NULL), _used(0) {}
    bool operator()(Player* player)
    {
        UInt32 used = player->GetVar(VAR_9344_USED);
        if(_player == NULL || used > _used)
        {
            _player = player;
            _used = used;
        }
        return true;
    }
};
void World::SendItem9344Award()
{
    SSelectItem9344UsedMost selector;
    globalPlayers.enumerate(selector);
    if(selector._player == NULL)
        return;
    MailPackage::MailItem items[] =
    {
        {9345, 1}
    };
    selector._player->sendMailItem(4120, 4121, items, sizeof(items)/sizeof(items[0]), false);
}
struct SSelectItem9343UsedMost : public Visitor<Player>
{
    Player* _player;
    UInt32 _used;
    SSelectItem9343UsedMost() : _player(NULL), _used(0) {}
    bool operator()(Player* player)
    {
        UInt32 used = player->GetVar(VAR_9343_USED);
        if(_player == NULL || used > _used)
        {
            _player = player;
            _used = used;
        }
        return true;
    }
};
void World::SendItem9343Award()
{
    SSelectItem9343UsedMost selector;
    globalPlayers.enumerate(selector);
    if(selector._player == NULL)
        return;
    MailPackage::MailItem items[] =
    {
        {9346, 1}
    };
    selector._player->sendMailItem(4122, 4123, items, sizeof(items)/sizeof(items[0]), false);
}

struct SSelectGoldSnakeUsedMost : public Visitor<Player>
{
    Player* _player;
    UInt32 _used;
    SSelectGoldSnakeUsedMost() : _player(NULL), _used(0) {}
    bool operator()(Player* player)
    {
        UInt32 used = player->GetVar(VAR_9215_USED); //使用与逍遥仙相同的var
        if(_player == NULL || used > _used)
        {
            _player = player;
            _used = used;
        }
        return true;
    }
};
void World::SendGoldSnakeAward()
{
    SSelectGoldSnakeUsedMost selector;
    globalPlayers.enumerate(selector);
    if(selector._player == NULL)
        return;
    MailPackage::MailItem items[] = { {9315, 1} };
    selector._player->sendMailItem(4064, 4065, items, sizeof(items)/sizeof(items[0]), false);
}


#ifndef _WIN32
void World::udpLog(const char* str1, const char* str2, const char* str3, const char* str4,
    const char* str5, const char* str6, const char* type)
{
    if (!ulog && cfg.udplog)
        ulog = _analyzer.GetInstance("1");
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
            s.time = p->GetVar(VAR_RECHARGE_TIME);
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

    UInt32 popularity = p->GetVar(VAR_POPULARITY);
    if (popularity)
    {
        RCSort s;
        s.player = p;
        s.total = popularity;
        World::popularitySort.insert(s);
    }
    if (World::getSurnameLegend())
    {
        UInt32 used = p->GetVar(VAR_SURNAMELEGEND_USED);
        if (used)
        {
            RCSort s;
            s.player = p;
            s.total = used;
            World::LuckyBagSort.insert(s);
        }
    }
    if (World::get11Time())
    {
        UInt32 used = p->GetVar(VAR_11AIRBOOK_GRADE);
        if (used)
        {
            RCSort s;
            s.player = p;
            s.total = used;
            World::PlayerGradeSort.insert(s);
        }
    }
    if (World::getQiShiBanTime())
    {
        //UInt32 score = p->GetQiShiBanScore();
        UInt32 score = p->GetVar(VAR_QISHIDOUFA_CYCLE_HIGHESTSCORE);
        if (score)
        {
            RCSort s;
            s.player = p;
            s.total = score;
            World::qishibanScoreSort.insert(s);
        }
    }
    if (World::getGGTime())
    {
        if(p->getGGStatus()!=2)
        {
            UInt32 used = p->getGGTimeScore();
            if (used)
            {
                RCSort s;
                s.player = p;
                s.total = used;
                World::guangGunSort.insert(s);
            }
        }
    }
    return true;
}
inline bool clan_enum_grade(GObject::Clan *clan,int)
{
    if (World::get11Time())
    {
        clan->updataClanGradeInAirBook();
        UInt32 grade = clan->getGradeInAirBook();
        if (grade)
        {
            ClanSort s;
            s.clan = clan;
            s.total = grade;
            World::clanGradeSort.insert(s);
        }
    }

    return true;
}
inline bool player_enum_rp7rc(GObject::Player * p, int)
{
    UInt32 opTime = TimeUtil::MkTime(cfg.openYear, cfg.openMonth, cfg.openDay);
    UInt32 total = 0;
    if(TimeUtil::SharpDay(0) < opTime + 7 * 86400 )
        total = p->getTotalRecharge();
    else if (TimeUtil::SharpDay(0) < opTime + 30 * 86400)
        total = p->GetVar(VAR_RP7_RECHARGE);
    else
        return false;
    if (total)
    {
        RCSort s;
        s.player = p;
        s.total = total;
        World::rechargeRP7Sort.insert(s);
    }
    return true;
}


static bool init = false;
static bool initRP7 = false;
void World::initRCRank()
{
    if (init)
        return;
    GObject::globalPlayers.enumerate(player_enum_rc, 0);
    GObject::globalClans.enumerate(clan_enum_grade, 0);
    init = true;
}

void World::initRP7RCRank()
{
    if (initRP7)
        return;
 
    GObject::globalPlayers.enumerate(player_enum_rp7rc, 0);
    initRP7 = true;
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
/////////////////snow
void World::LoadSnowScore(Player* pl, Player* lover)
{
    if(!pl->getSnowBind())
        return;
    if(lover->getSnowLover() != pl)
    {
        pl->resetSnow();
        return;
    }

    SnowScoreMap::iterator it = _snowScoreMap.find(pl);
    if(it == _snowScoreMap.end())
    {
        SnowPair* qp = new SnowPair();
        qp->p1.lover = lover;
        qp->p1.score = pl->getSnowScore();
        qp->p2.lover = pl;
        qp->p2.score = lover->getSnowScore();
        SnowPlayersIt qpIt = _snowPlayerSet.insert(qp);
        _snowScoreMap[pl] = qpIt;
        _snowScoreMap[lover] = qpIt;
    }
}


void World::UpdateSnowScore(Player* pl, Player* lover)
{
    if(!pl->getSnowBind())
        return;
    SnowScoreMap::iterator it = _snowScoreMap.find(pl);
    UInt32 myPlace = 0;
    if(it == _snowScoreMap.end())
    {
        SnowPair* qp = new SnowPair();
        qp->p1.lover = lover;
        qp->p1.score = pl->getSnowScore();
        qp->p2.lover = pl;
        qp->p2.score = lover->getSnowScore();
        SnowPlayersIt qpIt = _snowPlayerSet.insert(qp);
        _snowScoreMap[pl] = qpIt;
        _snowScoreMap[lover] = qpIt;
        myPlace = std::distance(_snowPlayerSet.begin(), qpIt) + 1;
    }
    else
    {
        SnowPlayersIt qpIt = it->second;
        SnowPair* qp = *(qpIt);

        _snowPlayerSet.erase(qpIt);
        if(qp->p1.lover == lover)
            qp->p1.score = pl->getSnowScore();
        else if(qp->p2.lover == lover)
            qp->p2.score = pl->getSnowScore();

        SnowPlayersIt qpIt2 = _snowPlayerSet.insert(qp);
        _snowScoreMap[pl] = qpIt2;
        _snowScoreMap[lover] = qpIt2;
        myPlace = std::distance(_snowPlayerSet.begin(), qpIt2) + 1;
    }

    if(0 != myPlace && myPlace < 4)
    {
        globalPlayers.enumerate(enum_snow, static_cast<void *>(NULL));
    }
    else
    {
        sendSnowPlayers(pl);
        sendSnowPlayers(lover);
    }
}

void World::sendSnowPlayers(Player* pl)
{
    UInt8 i = 0;
    UInt32 myPlace = 0;
    UInt32 myScore = 0;
    SnowScoreMap::iterator it = _snowScoreMap.find(pl);
    if(it != _snowScoreMap.end())
    {
        SnowPlayersIt qpIt = it->second;
        myPlace = std::distance(_snowPlayerSet.begin(), qpIt) + 1;
        SnowPair* qp = *qpIt;
        myScore = qp->p1.score + qp->p2.score;
    }
    else
    {
        myScore = pl->getSnowScore();
    }

    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x02);
    st << myPlace << myScore;
    size_t offset = st.size();

    st << i;
    for(SnowPlayersIt qpIt = _snowPlayerSet.begin(); i < 3 && qpIt != _snowPlayerSet.end(); ++ qpIt, ++ i)
    {
        SnowPair* qp = *qpIt;
        st << qp->p1.lover->getName() << qp->p2.lover->getName() << static_cast<UInt32>(qp->p1.score + qp->p2.score);
    }
    st.data<UInt8>(offset) = i;
    st << Stream::eos;

    pl->send(st);
}

void World::DivorceSnowPair(Player* pl)
{
    SnowScoreMap::iterator it = _snowScoreMap.find(pl);
    Player* lover1 = NULL;
    Player* lover2 = NULL;
    UInt32 myPlace = 0;
    if(it == _snowScoreMap.end())
    {
        return;
    }
    else
    {
        SnowPlayersIt qpIt = it->second;
        SnowPair* qp = *(qpIt);
        myPlace = std::distance(_snowPlayerSet.begin(), qpIt) + 1;

        _snowPlayerSet.erase(qpIt);
        _snowScoreMap.erase(qp->p1.lover);
        _snowScoreMap.erase(qp->p2.lover);
        lover1 = qp->p1.lover;
        lover2 = qp->p2.lover;
        delete qp;
    }

    if(0 != myPlace && myPlace < 4)
    {
        globalPlayers.enumerate(enum_snow, static_cast<void *>(NULL));
    }
    else
    {
        sendSnowPlayers(lover1);
        sendSnowPlayers(lover2);
    }

}

void World::SendSnowAward()
{
    static MailPackage::MailItem s_item[][3] = {
        {{515,30},{1325,30},{134,30}},
        {{515,20},{1325,20},{134,20}},
        {{515,15},{1325,15},{134,15}},
        {{515,10},{1325,10},{134,10}},
        {{514,15},{1325,5},{134,5}},
        {{500,10},{503,5}}
    };
    static MailPackage::MailItem s_card[2] = {{9927,1},{9928,1}};

  //  globalPlayers.enumerate(enum_snow_score, static_cast<void *>(NULL));
    
    UInt8 mark = 0;
    std::string str = ""; 
    SYSMSG(title1, 5110);
    for(SnowPlayersIt iter = _snowPlayerSet.begin(); iter != _snowPlayerSet.end() && mark < 3; ++iter, ++mark)
    {   
        UInt8 sex1 = 0;
        UInt8 sex2 = 0;
        UInt32 totalScore = 0;
        SnowPair* sp = *(iter);
        if(sp->p1.lover->IsMale())
            sex1 = 0;
        else
            sex1 = 1;

        if(sp->p2.lover->IsMale())
            sex2 = 0;
        else
            sex2 = 1;

        totalScore = sp->p1.score + sp->p2.score;
        SYSMSGV(buf, 5112, mark+1, sex1, sp->p1.lover->getName().c_str(), sex2, sp->p2.lover->getName().c_str(), totalScore);
        str += buf;

        if(2 == mark || mark == _snowPlayerSet.size() - 1)
        {
            SYSMSGV(buf, 5113, str.c_str());
            str = buf;
            break;
        }
    }

    UInt32 pos = 0;
    SYSMSG(title, 4114);
    for(SnowPlayersIt qpIt = _snowPlayerSet.begin(); qpIt != _snowPlayerSet.end() /*&& pos < 99*/; ++ qpIt, ++ pos)
    {
        /*if (pos >= 50)
            break;*/
        SnowPair* qp = *(qpIt);
        Player* player[2];
        player[0] = qp->p1.lover;
        player[1] = qp->p2.lover;
        UInt32 score = qp->p1.score + qp->p2.score;
        for(int idx = 0; idx < 2; ++idx)
        {
            Player* pl = player[idx];
            if(pl == NULL)
                continue;
            
            Player* lover = pl->getSnowLover();
            if(lover == NULL)
                continue;

            SYSMSGV(content1, 5111, score, pos+1, str.c_str());
            pl->GetMailBox()->newMail(NULL, 0x01, title1, content1);

            if(pos < 50)
            {
                SYSMSGV(content, 4115, lover->getName().c_str(), pos+1);
                Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                if(mail)
                {
                    int i = pos;
                    int count = 3;
                    if (i >= 3 && i <= 9) i = 3;
                    if (i >= 10 && i <= 19) i = 4;
                    if (i >= 20 && i <= 49)
                    {
                        i = 5;
                        count = 2;
                    }
                    mailPackageManager.push(mail->id, s_item[i], count, true);
                    if (i == 0)
                    {
                        if (pl->IsMale())
                            mailPackageManager.push(mail->id, &s_card[0], 1, false);
                        else
                            mailPackageManager.push(mail->id, &s_card[1], 1, false);
                    }
                }
            }
        }
    }
}
void World::SendRechargeRP7RankAward()
{
    static UInt32 s_couponCount[] = {2000,1000,500,200,200,200,200,200,200,200};
 
    World::initRP7RCRank();
    int pos = 0;
    for (RCSortType::iterator i = World::rechargeRP7Sort.begin(), e = World::rechargeRP7Sort.end(); i != e; ++i)
    {
        ++pos;

        if(pos > 10) break;

        Player* player = i->player;
        if (!player)
            continue;
        SYSMSG(title, 4910);
        SYSMSGV(content, 4911, pos);

        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem item = {0xA000,s_couponCount[pos-1]};
            mailPackageManager.push(mail->id, &item, 1, true);
            if (1 == pos)
            {
                MailPackage::MailItem card = {9901,1};
                mailPackageManager.push(mail->id, &card, 1, true);
            }
        }
          
/*        char id[1024] = {0};
        char ctx[1024] = {0};
        snprintf(id, sizeof(id), "F_10000_1213_%u_%d", cfg.serverNum, pos);
        snprintf(ctx, sizeof(ctx), "%" I64_FMT "u_%s_%u", player->getId(), player->getPName(), i->total);
        World::udpLog(id, ctx, "", "", "", "", "act");
  */
    }
//    World::rechargeRP7Sort.clear();
}

Player* World::getSpreadKeeper()
{
    if(!World::spreadKeeper)
    {
        UInt64 playerId = (static_cast<UInt64>(GVAR.GetVar(GVAR_SPREAD_KEEPER1)) << 32) + GVAR.GetVar(GVAR_SPREAD_KEEPER2);
        Player* pl = globalPlayers[playerId];
        if(pl)
            World::spreadKeeper = pl;
    }
    return World::spreadKeeper;
}

UInt32 World::getSpreadCount()
{
    return (GVAR.GetVar(GVAR_SPREAD_CONDITION) >> 8);
}
void World::Send11AirBookAward()    //lib待定
{
        
    Send11PlayerRankAward();
    Send11ClanRankAward();
    Send11CountryRankAward();
    World::PlayerGradeSort.clear();
    World::clanGradeSort.clear();
}
void World::Send11PlayerRankAward()
{
    World::initRCRank();
    int pos = 0;
    static MailPackage::MailItem s_item[][5] = {
        {{509,30},{515,30},{503,60},{134,30},{9022,50}},
        {{509,25},{515,25},{503,50},{134,25},{9022,30}},
        {{509,20},{515,20},{503,40},{134,20},{9022,20}},
    };
   // static MailPackage::MailItem card = {9922,1};
    SYSMSG(title, 4950);
    for (RCSortType::iterator i = World::PlayerGradeSort.begin(), e = World::PlayerGradeSort.end(); i != e; ++i)
    {
        Player* player = i->player;
        if (!player)
            continue;
        ++pos;
        if(pos > 3) break;
        SYSMSGV(content, 4951, pos);
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        //player->sendMailItem(4153, 4154, items, sizeof(items)/sizeof(items[0]), false);
        if(mail)
        {
            mailPackageManager.push(mail->id, s_item[pos-1], 5, true);
//            if(pos ==1)
  //              mailPackageManager.push(mail->id, &card, 1, true);
        }
        std::string strItems;
        for(int index = 0; index < 5; ++ index)
        {
            strItems += Itoa(s_item[pos-1][index].id);
            strItems += ",";
            strItems += Itoa(s_item[pos-1][index].count);
            strItems += "|";
        }
        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
    }
}

void World::SendQiShiBanAward()
{
    World::initRCRank();
    int pos = 0;

    static MailPackage::MailItem s_item[][3] = {
        {{1325,10},{5057,1}},
        {{1325,9},{5056,1}},
        {{1325,8},{5055,1}},
        {{1325,5},{5054,3}},
        {{1325,5},{5054,2}},
        {{1325,5},{5054,1}},
        {{1325,4},{5053,3}},
        {{1325,4},{5053,2}},
        {{1325,4},{5053,1}},
        {{1325,3},{5053,1}},
    };

    SYSMSG(title, 4972);
    for (RCSortType::iterator i = World::qishibanScoreSort.begin(), e = World::qishibanScoreSort.end(); i != e; ++i)
    {
        Player* player = i->player;
        if (!player)
            continue;
        ++pos;
        if(pos > 10) break;
        SYSMSGV(content, 4973, pos);
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            mailPackageManager.push(mail->id, s_item[pos-1], 2, true);
        }
    }
    World::qishibanScoreSort.clear();
}

void World::Send11ClanRankAward()
{
    World::initRCRank();
    int pos = 0;
    UInt32 ClanAwardID[5] = {134,1325,503,1126,9389};
    UInt32 ClanAwardNum[][5]={
        {180,150,180,150,180},
        {150,120,150,120,150},
        {100,100,100,75,100},
        {50,50,50,40,50},
        {20,20,20,20,30}
    };
    ClanGradeSort::iterator i = World::clanGradeSort.begin();
    for ( ;i != World::clanGradeSort.end(); ++i)
    {
        Clan* clan = i->clan;
        if (!clan)
            continue;
        ++pos;
        UInt32 type = pos ; 
        if(pos > 3 && pos <8)
            type = 4;
        if(pos > 7 )
        {
            UInt32 ClanGrade = clan->getGradeInAirBook();
            if(ClanGrade < 24000)
                break;
            else 
                type = 5;
            char str[16] = {0};
            sprintf(str, "F_130926_4");
            udpLog("tianshuqiyuan", str, "", "", "", "", "act");
        }
    //    SYSMSGV(content, 4947, pos+1);
      //  Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        //player->sendMailItem(4153, 4154, items, sizeof(items)/sizeof(items[0]), false);
        //if(mail)
        //{//
          //  mailPackageManager.push(mail->id, s_item[pos], 4, true);
           // if(pos ==0)
            //    mailPackageManager.push(mail->id, &card, 1, true);
       // }
        if(type > 5 ||type<1)
            continue;
        for(UInt32 index =0 ; index < 5 ;++index)
        {
            clan->AddItem(ClanAwardID[index],ClanAwardNum[type-1][index]);       
        }
    }
    
}

void World::Send11CountryRankAward()
{
    World::initRCRank();
    static MailPackage::MailItem s_item[][5] ={
         {{503,5},{515,3},{509,3},{134,3},{1325,3}},
    };
    ClanGradeSort::iterator i = World::clanGradeSort.begin();
    UInt32 EM=0,KL=0;
    UInt8 em=2,kl=2;
    UInt8 win = 0;
    std::string emei[2];
    std::string kunlun[2];
    for ( ;i != World::clanGradeSort.end()&&( em || kl ); ++i)
    {
        Clan* clan = i->clan;
        if (!clan)
            continue;
       if(clan->getCountry() ==0 && em != 0)
       {
            UInt32 ClanGrade = clan->getGradeInAirBook();
            emei[2-em]=clan->getName();
            em--;
            EM += ClanGrade;
       }
       else if(clan->getCountry() ==1 && kl!=0)
       {
           UInt32 ClanGrade = clan->getGradeInAirBook();  
           kunlun[2-kl]=clan->getName();
           kl--;
           KL += ClanGrade;
       }
    } 
    if(EM < KL )
        win = 1 ;
    if(win == 0)
    {
        SYSMSG_BROADCASTV(4962,emei[0].c_str(),emei[1].c_str() );
    }
    else if(win == 1)
    {
        SYSMSG_BROADCASTV(4963,kunlun[0].c_str(),kunlun[1].c_str());
    }
    SYSMSG(title, 4952);
    for (RCSortType::iterator i = World::PlayerGradeSort.begin(), e = World::PlayerGradeSort.end(); i != e; ++i)
    {
        Player* player = i->player;
        if (!player)
            continue;
        if(player->getCountry() != win)
            continue;
        SYSMSGV(content, 4953);
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
             mailPackageManager.push(mail->id, s_item[0], 5, true); 
        }
        std::string strItems;
        for(int index = 0; index <5; ++ index)
        {
            strItems += Itoa(s_item[0][index].id);
            strItems += ",";
            strItems += Itoa(s_item[0][index].count);
            strItems += "|";
        }
        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
    }
}

bool World::MemCachInit()
{
    size_t sz = cfg.IDQueryMemcached.size();
    for (size_t i = 0; i < sz; ++i)
    {
        char buf[128];
        snprintf(buf, 128, "%s:%d", cfg.IDQueryMemcached[i].ip.c_str(), cfg.IDQueryMemcached[i].port); 
        m_MCached.pushHost(buf);
        _memcinited = true;
    }
    return true;
}

void World::SetMemCach_qishiban(UInt32 score, const char * openId)
{
    if (_memcinited)
    {
        char value[32] = {0};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "qishiban_%s", openId);
        size_t vlen = snprintf(value, sizeof(value), "%d", score);

        bool res = m_MCached.set(key, len, value, vlen, 0);
        TRACE_LOG("setKey: %s, setScore: %u, res:%u", key, score, res);
    }
}

UInt32 World::GetMemCach_qishiban(const char * openId)
{
    char value[32]={0};
    char key[MEMCACHED_MAX_KEY] = {0};
    snprintf(key, MEMCACHED_MAX_KEY, "qishiban_%s", openId);

    UInt32 score = 0;
    if (_memcinited)
    {
        const char* res = m_MCached.get(key, value, sizeof(value));
        score = atoi(value);
        TRACE_LOG("getKey: %s, getScore: %u, res:%u", key, score, res);
    }

    return score;
}

void World::SendGuangGunAward()    //待定
{
    World::initRCRank();
    int pos = 0;
    static MailPackage::MailItem s_item[][5] = {
        {{515,30},{503,40},{134,30},{1325,30},{9435,20}},
        {{515,25},{503,30},{134,25},{1325,25},{9435,10}},
        {{515,20},{503,25},{134,20},{1325,20},{9435,8}},
    };
    static MailPackage::MailItem card = {9924,1};
    SYSMSG(title, 4964);
    for (RCSortType::iterator i = World::guangGunSort.begin(), e = World::guangGunSort.end(); i != e; ++i)
    {
        Player* play = i->player;
        if (!play)
            continue;
        Player* player = play->getGGTimeCaptain();
        ++pos;
        if(pos > 3) break;
        SYSMSGV(content, 4965, pos);
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        //player->sendMailItem(4153, 4154, items, sizeof(items)/sizeof(items[0]), false);
        if(mail)
        {
            mailPackageManager.push(mail->id, s_item[pos-1], 5, true);
            if(pos ==1)
                mailPackageManager.push(mail->id, &card, 1, true);
        }

        Player* player1 = player->getGGPlayer1();
        if (player1)
        {
            SYSMSGV(content, 4965, pos);
            Mail * mail1 = player1->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            //player->sendMailItem(4153, 4154, items, sizeof(items)/sizeof(items[0]), false);
            if(mail1)
            {
                mailPackageManager.push(mail1->id, s_item[pos-1], 5, true);
                if(pos ==1)
                    mailPackageManager.push(mail1->id, &card, 1, true);
            }
        }
        Player* player2 = player->getGGPlayer2();
        if (player2)
        {
            SYSMSGV(content, 4965, pos);
            Mail * mail2 = player2->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            //player->sendMailItem(4153, 4154, items, sizeof(items)/sizeof(items[0]), false);
            if(mail2)
            {
                mailPackageManager.push(mail2->id, s_item[pos-1], 5, true);
                if(pos ==1)
                    mailPackageManager.push(mail2->id, &card, 1, true);
            }
        }
    }
}

}


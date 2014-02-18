#ifndef WORLD_INC
#define WORLD_INC

#include "MsgHandler/WorldMsgHandler.h"
#include "Server/WorkerThread.h"
#include "Common/Stream.h"
#include "Server/Cfg.h"
#include "Common/TimeUtil.h"
#include "Common/MCached.h"
#include "GObject/GVar.h"
#ifndef _WIN32
#include "kingnet_analyzer.h"
#endif

namespace Network
{
	class TcpConduit;
}

namespace Script
{
	class WorldScript;
	class BattleFormula;
}

namespace GObject
{

//仙界相关
enum ARENA_STATE
{
    ARENA_XIANJIE_NONE    = 0,
    ARENA_XIANJIE_DIYI    = 1,
    ARENA_XIANJIE_ZHIZUN  = 2,
    ARENA_XIANJIE_CHUANQI = 3,
    ARENA_XIANJIE_MAX,
};

class Clan;
struct MoneyIn
{
    int gold;
    int coupon;
    int tael;
    int achievement;
    int prestige;
};

struct stOldMan
{
    UInt8 _loc;
    UInt16 _spot;
    UInt32 _time ;
    std::set<UInt64> _players;
    stOldMan():_loc(0),_spot(0),_time(0){}
};
struct stArenaExtra
{
    UInt8 week;
    std::string name[5];
    UInt8 heroId[5];
    UInt32 sufferTotal;
    UInt32 sufferCnt[5];
    UInt32 lasttime[5];
    UInt8 rank[5];
    UInt64 playerId[5];
};

typedef std::list<Player*> LuckyDrawList;
typedef LuckyDrawList::iterator LuckyDrawRank;
typedef LuckyDrawList::reverse_iterator RLuckyDrawRank;
typedef std::map<Player *, LuckyDrawRank> LuckyDrawRankList;

struct RCSort
{
    GObject::Player* player;
    UInt32 total;
    UInt32 time;
    RCSort():player(NULL),total(0),time(0){}
};


struct ClanSort   //帮派积分（10· 1活动）
{
    GObject::Clan* clan;
    UInt32 total;
};   
struct lt_rcsort
{
    bool operator()(const RCSort& a, const RCSort& b) const { return a.total > b.total || (a.total==b.total && a.time < b.time); }
};
struct clan_sort
{
    bool operator()(const ClanSort& a, const ClanSort& b) const { return a.total >= b.total; }
};

typedef std::multiset<RCSort, lt_rcsort> RCSortType;
typedef std::set<ClanSort,clan_sort> ClanGradeSort;

struct supportSort
{
    UInt32 support;
    UInt8 heroId;
    std::string name;
    UInt64 playerId;
};
struct lt_ssort
{
    bool operator()(const supportSort& a, const supportSort& b) const { return a.support >= b.support; }
};
typedef std::set<supportSort, lt_ssort> SupportSortType;

struct QixiScore
{
    Player* lover;
    UInt32 score;
};

struct QixiPair
{
    QixiScore p1;
    QixiScore p2;
};

struct ScoreGreater
{
     bool operator()(const QixiPair* qp1, const QixiPair* qp2) const
     {
         return ((qp1->p1.score + qp1->p2.score) > (qp2->p1.score + qp2->p2.score));
     }
};

typedef std::multiset<QixiPair*, ScoreGreater> QixiPlayerSet;
typedef QixiPlayerSet::iterator QixiPlayersIt;
typedef std::map<Player*, QixiPlayersIt> QixiScoreMap;

typedef QixiScore SnowScore;
typedef QixiPair  SnowPair;
typedef QixiPlayerSet SnowPlayerSet;
typedef QixiPlayersIt SnowPlayersIt;
typedef QixiScoreMap  SnowScoreMap;

struct ValueSort
{
    UInt32 sufferCnt;
    UInt32 lastTime;
    std::string name;
};
struct lt_valuesort
{
    bool operator()(const ValueSort& a, const ValueSort& b) const;
};
typedef std::multiset<ValueSort, lt_valuesort> ValueSortType;

class World:
	public WorkerRunner<WorldMsgHandler>
{
public:
	World();
	~World();

public:
    inline static UInt8 getArenaState() { return _arenaState; }
    inline static void setArenaState(UInt8 as) { if(as >= ARENA_XIANJIE_MAX) as = ARENA_XIANJIE_MAX-1; _arenaState = as; }

	inline UInt32 ThisDay() { return _today; }
	inline UInt32 Now() { return _now; }
	inline bool isNewCountryBattle() { return !(_wday % 2); }

    inline static bool isFBVersion() { return cfg.fbVersion; }
    inline static bool isVTVersion() { return cfg.vtVersion; }
    inline static bool isDebug() { return cfg.debug; }

	inline static void setActivityStage(int stage)
	{ _activityStage = stage; }
	inline static int getActivityStage()
	{ return _activityStage; }
	inline static void setActAvailable(bool aa)
	{ _actAvailable = aa; }
	inline static void setActAvailable1(bool aa)
	{ _actAvailable1 = aa; }
	inline static void setAutoHeal(bool v)
	{ _autoHeal = v; }
	inline static bool getAutoHeal()
	{ return _autoHeal;}
	inline static void setCopyFrontWinSwitch(bool v)
	{ _copyfrontwin = v; }
	inline static bool getCopyFrontWinSwitch()
	{ return _copyfrontwin;}
	inline static void setIsNewServer(bool aa)
	{ _isNewServer = aa; }
	inline static bool IsNewServer()
	{ return _isNewServer;}
	inline static void setNationalDay(bool aa)
	{ _nationalDay = aa; }
    inline static void setHalloween(bool v)
    { _halloween = v; }
    inline static void setSingleDay(bool v)
    { _singleday = v; }
    inline static bool getSingleDay()
    { return _singleday; }
    inline static void setThanksgiving(bool v)
    { _thanksgiving = v; }
    inline static bool getThanksgiving()
    { return _thanksgiving; }
    inline static void setChristmas(bool v)
    { _christmas = v; }
    inline static bool getChristmas()
    { return _christmas; }
    inline static void setNewYear(bool v)
    { _newyear = v; }
    inline static bool getNewYear()
    { return _newyear; }
    inline static void setBlueactiveday(bool v)
    { _blueactiveday = v; }
    inline static bool getBlueactiveday()
    { return _blueactiveday; }
    inline static void setRechargeActive(bool v, UInt8 no)
    { _rechargeactive = v; if (v) _rechargeactiveno |= no; else _rechargeactiveno &= ~no; }
    inline static bool getRechargeActive()
    { return _rechargeactive; }
    inline static void setRechargeActive3366(bool v, UInt8 no)
    { _rechargeactive3366 = v; if (v) _rechargeactiveno |= no; else _rechargeactiveno &= ~no; }
    inline static bool getRechargeActive3366()
    { return _rechargeactive3366; }
    inline static void setYearActive(bool v)
    { _yearact = v; }
    inline static bool getYearActive()
    { return _yearact; }
    inline static void setQgameGiftAct(bool v)
    { _qgamegiftact = v; }
    inline static bool getQgameGiftAct()
    { return _qgamegiftact; }
    inline static void setSpreadAct(bool v)
    { _spreadAct = v; }
    inline static bool getSpreadAct()
    { return _spreadAct; }
    inline static void setValentineDay(bool v)
    { _valentineday = v; }
    inline static bool getValentineDay()
    { return _valentineday; }
    inline static void setNetValentineDay(bool v)
    { _netvalentineday = v; }
    inline static bool getNetValentineDay()
    { return _netvalentineday; }
    inline static void setGirlDay(bool v)
    { _girlday = v; }
    inline static bool getGirlDay()
    { return _girlday; }
    inline static void setWhiteLoveDay(bool v)
    { _whiteloveday = v; }
    inline static bool getWhiteLoveDay()
    { return _whiteloveday; }
    inline static void setTrumpEnchRet(bool v)
    { _trumpenchret = v; }
    inline static bool getTrumpEnchRet()
    { return _trumpenchret; }
    inline static void setFoolsDay(bool v)
    { _foolsday = v; }
    inline static bool getFoolsDay()
    { return _foolsday; }
    inline static void setChingMing(bool v)
    { _chingming = v; }
    inline static bool getChingMing()
    { return _chingming; }
    inline static void setCarnival(bool v)
    { _carnival = v; }
    inline static bool getCarnival()
    { return _carnival; }
    inline static void setRC7Day(bool v)
    { _rc7day = v; }
    inline static bool getRC7Day()
    { return _rc7day; }
    inline static void setShuoShuo(bool v)
    { _shuoshuo = v; }
    inline static bool getShuoShuo()
    { return _shuoshuo || true; }
    inline static void setCFriend(bool v)
    { _cfriend = v; }
    inline static bool getCFriend()
    { return _cfriend; }
    inline static void setCFriendAct(bool v)
    { _cfriendAct = v; }
    inline static bool getCFriendAct()
    { return _cfriendAct; }
    inline static void setMayDay(bool v)
    { _mayday = v; }
    inline static bool getMayDay()
    { return _mayday; }
    inline static void setMayDay1(bool v)
    { _mayday1 = v; }
    inline static bool getMayDay1()
    { return _mayday1; }
    inline static void setYDMDAct(bool v)
    { _ydmdact = v; }
    inline static bool getYDMDAct()
    { return _ydmdact; }
    inline static void setSSDTAct(bool v)
    { _ssdtact = v; }
    inline static bool getSSDTAct()
    { return _ssdtact; }
    inline static void setFighter1368(bool v)
    { _fighter1368 = v; }
    inline static bool getFighter1368()
    { return _fighter1368; }
    inline static void setEnchantAct(bool v)
    { _enchantact = v; }
    inline static bool getEnchantAct()
    { return _enchantact; }
    inline static void setBlueDiamondAct(bool v)
    { _bluediamondact = v; }
    inline static bool getBlueDiamondAct()
    { return _bluediamondact; }
    inline static void setYellowDiamondAct(bool v)
    { _yellowdiamondact = v; }
    inline static bool getYellowDiamondAct()
    { return _yellowdiamondact; }
    inline static void setQQVipAct(bool v)
    { _qqvipact = v; }
    inline static bool getQQVipAct()
    { return _qqvipact; }
    inline static void setFallAct(bool v)
    { _fallact = v; }
    inline static bool getFallAct()
    { return _fallact; }
     inline static void setQQGameAct(bool v)
    { _qqgameact = v; }
    inline static bool getQQGameAct()
    { return _qqgameact; }
    inline static void set3366PrivilegeAct(bool v)
    { _3366privilegeact = v; }
    inline static bool get3366PrivilegeAct()
    { return _3366privilegeact; }
    inline static void setQzonePYPrivilegeAct(bool v)
    { _qzonepyprivilegeact = v; }
    inline static bool getQzonePYPrivilegeAct()
    { return _qzonepyprivilegeact; }
    inline static void setGoodVoiceAct(bool v)
    { _goodvoiceact = v; }
    inline static bool getGoodVoiceAct()
    { return _goodvoiceact; }
    inline static void set3366GiftAct(bool v)
    { _3366giftact = v; }
    inline static bool get3366GiftAct()
    { return _3366giftact; }
    inline static void setQzongPYGiftAct(bool v)
    { _qzongpygiftact = v; }
    inline static bool getQzongPYGiftAct()
    { return _qzongpygiftact; }

    inline static void setTrainFighter(bool v)
    { _trainfighter = v; }
    inline static bool getTrainFighter()
    { return _trainfighter; }
    inline static void setGemMergeAct(bool v)
    { _gemmergeact = v; }
    inline static bool getGemMergeAct()
    { return _gemmergeact; }
    inline static void setEnchantGt11(bool v)
    { _enchant_gt11 = v; }
    inline static bool getEnchantGt11()
    { return _enchant_gt11; }


    inline static void setJune(bool v)
    { _june = v; }
    inline static bool getJune()
    { return _june; }
    inline static void setJune1(bool v)
    { _june1 = v; }
    inline static bool getJune1()
    { return _june1; }
    inline static void setJuly(bool v)
    { _july = v; }
    inline static bool getJuly()
    { return _july; }

    inline static void setQixi(bool v)
    { _qixi = v; }
    inline static bool getQixi()
    { return _qixi; }
    inline static void setWansheng(bool v)
    { _wansheng= v; }
    inline static bool getWansheng()
    { return _wansheng; }
    inline static void setQingren(bool v)
    { _qingren= v; }
    inline static bool getQingren()
    { return _qingren; }
    inline static void setSpecialBookAct(bool v)
    { _specialbook= v; }
    inline static bool getSpecialBookAct()
    { return _specialbook; }


    inline static void set11Act(bool v)
    { _11Act= v; }
    inline static bool get11Act()
    { return _11Act; }

    inline static void setSSToolbarAct(bool v)
    { _ssToolbarAct= v; }
    inline static bool getSSToolbarAct()
    { return _ssToolbarAct; }

    /*inline static void setSnowAct(bool v)
    { _snowAct= v; }
    inline static bool getSnowAct()
    { return _snowAct; }*/
    inline static void setGoldSnakeAct(bool v)
    { _goldSnakeAct= v; }
    inline static bool getGoldSnakeAct()
    { return _goldSnakeAct; }
    inline static void setCollectCardAct(bool v)
    { _collectCardAct= v; }
    inline static bool getCollectCardAct()
    { return _collectCardAct; }
    inline static void setHalloweenAct(bool v)
    { _halloweenAct= v; }
    inline static bool getHalloweenAct()
    { return _halloweenAct; }
    inline static void setHeroIslandAct(bool v)
    { _heroIslandAct= v; }
    inline static bool getHeroIslandAct()
    { return _heroIslandAct; }
    inline static void setDragonKingAct(bool v)
    { _dragonKingAct= v; }
    inline static bool getDragonKingAct()
    { return _dragonKingAct; }
    inline static void setSaveGoldAct(bool v)
    { _saveGoldAct= v; }
    inline static bool getSaveGoldAct()
    { return _saveGoldAct; }
    inline static void setFeastLoginAct(bool v)
    { _feastloginAct= v; }
    inline static bool getFeastLoginAct()
    { return _feastloginAct; }
    inline static void setNewYearGiveGiftAct(bool v)
    { _newYearGiveGiftAct= v; }
    inline static bool getNewYearGiveGiftAct()
    { return _newYearGiveGiftAct; }
    inline static void setNewYearQQGameAct(bool v)
    { _newYearQQGameAct= v; }
    inline static bool getNewYearQQGameAct()
    { return _newYearQQGameAct; }
    inline static void setQZoneQQGameAct(bool v)
    { _QZoneQQGameAct= v; }
    inline static bool getQZoneQQGameActY()
    {
        UInt32 begin = GVAR.GetVar(GVAR_QZONEQQGAMEY_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_QZONEQQGAMEY_END);
        UInt32 now = TimeUtil::Now();
        bool QZoneQQGameAct = false ;
        if( now >= begin && now <= end)
            QZoneQQGameAct = true;
        else
            QZoneQQGameAct = false;
        return QZoneQQGameAct; 
    }
    inline static bool getQZoneQQGameAct()
    {
        UInt32 begin = GVAR.GetVar(GVAR_QZONEQQGAME_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_QZONEQQGAME_END);
        UInt32 now = TimeUtil::Now();
        if(begin == 0 && end == 0)
        {
            return _QZoneQQGameAct;
        }
        else if( now >= begin && now <= end)
            _QZoneQQGameAct = true;
        else
            _QZoneQQGameAct = false;
        return _QZoneQQGameAct; 
    }
    inline static void setNewYearQzoneContinueAct(bool v)
    { _newYearQzoneContinueAct= v; }
    inline static bool getNewYearQzoneContinueAct()
    { return _newYearQzoneContinueAct; }
    inline static void setMayDayLoginAct(bool v)
    { _maydayloginAct= v; }
    inline static bool getMayDayLoginAct()
    { return _maydayloginAct; }

    inline static void setTowerLoginAct(UInt8 v)
    { _towerloginAct= v; }
    inline static UInt8 getTowerLoginAct()
    { return _towerloginAct; }

    inline static void setGuoqing(bool v)
    { _guoqing = v; }
    inline static bool getGuoqing()
    { return _guoqing; }

    inline static void set9215Act(bool v)
    { _9215Act= v; }
    inline static bool get9215Act()
    { return _9215Act; }

    inline static void setRechargeNextRet(bool v)
    { _rechargenextret = v; }
    inline static bool getRechargeNextRet()
    { return _rechargenextret; }
    inline static void setRechargeNextRetStart(UInt32 start, UInt32 end)
    { _rechargenextretstart = start; _rechargenextretend = end; }

    inline static void setMergeAthAct(bool v)
    { _mergeathact = v; }
    inline static bool getMergeAthAct()
    { return _mergeathact; }
    inline static void setFourCopAct(bool v)
    { _fourcopact = v; }
    inline static bool getFourCopAct()
    { return _fourcopact; }
    inline static void setDuanWu(bool v)
    { _duanwu = v; }
    inline static bool getDuanWu()
    { return _duanwu; }

    inline static void setICAct(bool v)
    { _icact = v; }
    inline static bool getICAct()
    { return _icact; }

    inline static void setLevelAwardEnd(UInt32 end)
    { _levelawardend = end; }

    inline static void setPExpItems(bool v)
    { _pexpitems = v; }
    inline static bool getPExpItems()
    { return _pexpitems; }
    inline static void setSoSoMapBegin(UInt32 v)
    { _sosomapbegin = v; }

    inline static void setOpenTest(bool v)
    { _opentest = v; }
    inline static bool getOpenTest()
    { return _opentest; }

    inline static void setConsumeActive(bool v)
    { _consumeactive = v; }
    inline static bool getConsumeActive()
    { return _consumeactive; }
    inline static void setConsume918(bool v)
    { _consume918 = v; }
    inline static bool getConsume918()
    { return _consume918; }
    inline static void setConsumeAwardAct(bool v)
    { _consumeawardact = v; }
    inline static bool getConsumeAwardAct()
    { return _consumeawardact; }
    inline static void  setSummerFlow(bool v)
    {   _summerFlow=v; } 
    inline static void  setSummerMeet(bool v)
    {   _summerMeet=v; } 
    inline static void  set11Time(bool v)
    {   _11time=v; } 
    inline static bool  get11Time()
    {return _11time; } 
    inline static void  setGGTime(bool v)
    {   _ggtime=v; } 
    inline static bool  getGGTime(UInt32 time =0 )
    {
        UInt32 now = TimeUtil::Now() + time;
        if ((now > getOpenTime() + 7 * 86400) && (now  < getOpenTime() + 14 * 86400) )
            return true;
        return false ;
        //return _ggtime; 
    } 
    inline static void  setQZoneRechargeTime(bool v)
    {   _qzoneRechargetime=v; } 
    inline static bool  getQZoneRechargeTime()
    {
        UInt32 begin = GVAR.GetVar(GVAR_QZONE_RECHARGE_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_QZONE_RECHARGE_END);
        UInt32 now = TimeUtil::Now() ;
        if( now >= begin && now <= end)
            return true;
        else
            return false;
    } 
    inline static bool  get3366RechargeTime()
    {
        UInt32 begin = GVAR.GetVar(GVAR_3366_RECHARGE_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_3366_RECHARGE_END);
        UInt32 now = TimeUtil::Now() ;
        if( now >= begin && now <= end)
            return true;
        else
            return false;
    }
    
    inline static bool  get3366BuyTime()
    {
        UInt32 begin = GVAR.GetVar(GVAR_3366_BUY_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_3366_BUY_END);
        UInt32 now = TimeUtil::Now() ;
        if( now >= begin && now <= end)
            return true;
        else
            return false;
    }

    inline static bool  getOldManTime()
    {
        //UInt32 begin = GVAR.GetVar(GVAR_OLDMAN_BEGIN);
        UInt32 begin = TimeUtil::MkTime(2014, 1, 31);
        //UInt32 end = GVAR.GetVar(GVAR_OLDMAN_END);
        UInt32 end = TimeUtil::MkTime(2014, 2, 7);
        UInt32 now = TimeUtil::Now() ;
        if( now >= begin && now <= end)
            return true;
        else
            return false;
    } 
   
    inline static UInt32 get11TimeAirNum(UInt32 time = 0)
    {
        UInt32 _11timeBegin = TimeUtil::MkTime(2014, 1, 25);
        UInt32 _11timeEnd = TimeUtil::MkTime(2014, 1, 30);
//        UInt32 _11timeBegin = TimeUtil::MkTime(2013, 9, 28);
//      UInt32 _11timeEnd = TimeUtil::MkTime(2013, 10, 12);
        UInt32 now = TimeUtil::Now() ;
        if(time !=0)
            now = time;
        if(now < (_11timeBegin) || now >= _11timeEnd )
            return -1;
       return (TimeUtil::SharpDay(0, now) - _11timeBegin )/86400+1; 
    }
    inline static UInt32 get11TimeNum(UInt32 time = 0)
    {
        return -1;
        UInt32 _11timeBegin = TimeUtil::MkTime(2013, 9, 28);
        UInt32 _11timeEnd = TimeUtil::MkTime(2013, 10, 12);
        UInt32 now = TimeUtil::Now() ;
        if(time !=0)
            now = time;
        if(now < (_11timeBegin) || now >= _11timeEnd )
            return -1;
       return (TimeUtil::SharpDay(0, now) - _11timeBegin )/86400+1; 
    }
    inline static bool getSummerFlow()
    { 
        UInt32 begin = GVAR.GetVar(GVAR_SUMMER_FLOW_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_SUMMER_FLOW_END);
        UInt32 now = TimeUtil::Now() ;
        if(begin == 0 && end == 0)
            return _summerFlow;
        if( now >= begin && now <= end)
            return true;
        else
            return false;
    } 
    inline static void setNeedRechargeRank(bool v)
    { _needrechargerank = v; }
    inline static bool getNeedRechargeRank()
    { return _needrechargerank; }
    inline static void setRechargeTime(UInt32 begin, UInt32 end)
    {
        _rechargebegin = begin;
        _rechargeend = end;
    }
    inline static UInt32 getRechargeBegin() { return _rechargebegin; }
    inline static UInt32 getRechargeEnd() { return _rechargeend; }
    inline static void setConsumeTime(UInt32 begin, UInt32 end)
    {
        _consumebegin = begin;
        _consumeend = end;
    }
    inline static bool  getNewYearSpeedTime(UInt32 now = 1)
    {
        if( now == 1 )
              now = TimeUtil::Now();
        UInt32 _speedBegin = TimeUtil::MkTime(2014, 1, 18);
        UInt32 _speedEnd = TimeUtil::MkTime(2014, 2,16);
        if(now >_speedBegin && now <= _speedEnd)
            return true;
        else 
            return false;
    } 

    inline static UInt32 getConsumeBegin() { return _consumebegin; }
    inline static UInt32 getConsumeEnd() { return _consumeend; }
    inline static void setNeedConsumeRank(bool v)
    { _needconsumerank = v; }
    inline static bool getNeedConsumeRank()
    { return _needconsumerank; }
    inline static void setKillMonsterAct(bool v)
    { _killMonsteract = v; }
    inline static bool getKillMonsterAct()
    { return _killMonsteract; }


	inline Script::WorldScript * getWorldScript() { return _worldScript; }
	inline Script::BattleFormula * getBattleFormula() { return _battleFormula; }
    static void commitArenaForceOnce();

    inline static void setTownReward_10_15(bool v)
    { _townReward_10_15 = v; }
    inline static bool getTownReward_10_15()
    { return _townReward_10_15; }

    inline static void setLoginAward(bool v)
    { _loginAward = v; }
    inline static bool getLoginAward()
    { return _loginAward; }
    inline static void setBDSuperman(bool v)
    { _bluediamonSuperman = v; }
    inline static bool getBDSuperman()
    { return _bluediamonSuperman; }

    inline static void setTgcEvent(bool v)
    { _tgcevent = v; }
    inline static bool getTgcEvent()
    { return _tgcevent; }

    inline static void setWeiboAct(bool v)
    { _weiboact = v; }
    inline static bool getWeiboAct()
    { return _weiboact; }
    inline static void setPetEggAct(bool v)
    { _peteggact = v; }
    inline static bool getPetEggAct()
    { return _peteggact; }
    inline static void setLongyuanAct(bool v)
    { _longyuanact = v; }
    inline static bool getLongyuanAct()
    { return _longyuanact; }

    inline static void setQQGameOnlineAwardAct(bool v)
    { _qqgameonlineawardact = v; }
    inline static bool getQQGameOnlineAwardAct()
    { return _qqgameonlineawardact; }

    inline static void setAccRecharge(bool v)
    { _accrecharge = v; }
    inline static bool getAccRecharge()
    { return _accrecharge; }

    inline static void setMiLuZhiJiaoAct(bool v)
    { _miluzhijiao = v; }
    inline static bool getMiLuZhiJiaoAct()
    { return _miluzhijiao; }

    inline static void setBuyFundAct(bool v)
    { _buyfund = v; }
    inline static bool getBuyFundAct()
    { return _buyfund; }

    inline static bool getLuckyStarAct()
    {
        UInt32 now = TimeUtil::Now();
        UInt32 btime = GVAR.GetVar(GVAR_LUCKYSTAR_BEGIN);
        UInt32 etime = GVAR.GetVar(GVAR_LUCKYSTAR_END);
        return btime <= now && now <= etime;
    }
    inline static void setArenaHeroId(UInt8 pos, UInt8 heroId)
    {
        if(pos < 5 && stArena.heroId[pos] != heroId)
        {
            stArena.heroId[pos] = heroId;
        }
    }
    inline static UInt8 getArenaHeroId(UInt8 pos)
    {
        if(pos < 5)
        {
            return stArena.heroId[pos];
        }
        return 0;
    }

    inline static void setArenaName(UInt8 pos, std::string name)
    {
        if(pos < 5/* && stArena.name[pos].compare(name) != 0*/)
        {
            if(stArena.name[pos] == name)
                return;
            stArena.name[pos] = name;
        }
    }
    inline static std::string getArenaName(UInt8 pos)
    {
        static std::string nullName;
        if(pos < 5)
        {
            return stArena.name[pos];
        }
        return nullName;
    }

    inline static void setArenaTotalCnt(UInt32 total)
    {
        if(stArena.sufferTotal != total)
        {
            stArena.sufferTotal = total;
        }
    }
    inline static UInt32 getArenaTotalCnt()
    {
        return stArena.sufferTotal;
    }
    static void setArenaInfo(UInt8 type);
    void setArenaTotalCntEnum();
    inline void resetArenaInfo()
    {
        static std::string nullname;
        GObject::World::stArena.week = 0;
        GObject::World::stArena.sufferTotal = 0;
        for(UInt8 i = 0; i < 5; i++)
        {
            GObject::World::stArena.name[i] = nullname;
            GObject::World::stArena.heroId[i] = 0;
            GObject::World::stArena.sufferCnt[i] = 0;
            GObject::World::stArena.lasttime[i] = 0;
            GObject::World::stArena.rank[i] = 0;
            GObject::World::stArena.playerId[i] = 0;
        }
    }
    inline static void setArenaPlayerId(UInt8 pos, UInt64 playerId)
    {
        if(pos < 5 && stArena.playerId[pos] != playerId)
        {
            stArena.playerId[pos] = playerId;
        }
    }
    inline static UInt64 getArenaPlayerId(UInt8 pos)
    {
        if(pos < 5)
        {
            return stArena.playerId[pos];
        }
        return 0;
    }

    inline static bool canDestory(UInt32 itemid)
    {
        if(itemid == 9273)
        {
            if(World::getGoodVoiceAct())
                return false;
            else
                return true;
        }

        static UInt32 items[] =
        {
            9184,9185,9186,9187,9188,
        };
        if (World::getTgcEvent())
        {
            for (size_t i = 0; i < sizeof(items)/sizeof(items[0]); ++i)
            {
                if (items[i] == itemid)
                    return false;
            }
        }
        return true;
    }
    inline static void setCompassAct(bool v)
    { _compassact = v; }
    inline static bool getCompassAct()
    { return _compassact; }

    inline static void setFoolBao(bool v)
    { _foolbao = v; }
    inline static bool getFoolBao()
    { return _foolbao; } 
    
    inline static void setSummerFlow3Time(bool v)
    { _summerFlow3 = v; }
    inline static bool getSummerFlow3Time()
    {
        return _summerFlow3;
    } 

    inline static void setSurnameLegend(bool v)
    {
        /* 春节期间注释掉
        UInt32 begin = GVAR.GetVar(GVAR_SURNAMELEGEND_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_SURNAMELEGEND_END);
        UInt32 now = TimeUtil::Now();
        if( now >= begin && now <= end)
            return;
        */
        _surnamelegend = v;
    }
   
    inline static bool getSurnameLegend(UInt32 time = 0)
    {
        /* 春节期间注释掉
        UInt32 begin = GVAR.GetVar(GVAR_SURNAMELEGEND_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_SURNAMELEGEND_END);
        UInt32 now = TimeUtil::Now() + time;
        if(begin == 0 && end == 0)
        {
            return _surnamelegend;
        }
        else if( now >= begin && now <= end)
            _surnamelegend = true;
        else
            _surnamelegend = false;
        */
        return _surnamelegend;
    }

    inline static void setSnowAct(bool v)
    {
        UInt32 begin = GVAR.GetVar(GVAR_CHRISTMAS_PILESNOW_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_CHRISTMAS_PILESNOW_END);
        UInt32 now = TimeUtil::Now();
        if( now >= begin && now <= end)
            return;
        _snowAct = v;
    }

    inline static bool getSnowAct(UInt32 time = 0)
    {
        UInt32 begin = GVAR.GetVar(GVAR_CHRISTMAS_PILESNOW_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_CHRISTMAS_PILESNOW_END);
        UInt32 now = TimeUtil::Now() + time;
        if(begin == 0 && end == 0)
            return _snowAct;

        if(now >= begin && now <= end)
            _snowAct = true;
        else
            _snowAct = false;

        return _snowAct;
    }

    inline static void setHappyFireTime(bool v)
    {
        /* 春节期间注释掉
        UInt32 begin = GVAR.GetVar(GVAR_YEARHAPPY_RANK_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_YEARHAPPY_RANK_END);
        UInt32 now = TimeUtil::Now();
        if( now >= begin && now <= end)
            return;
        */
        _happyFire = v;
    }
    inline static bool getHappyFireTime(UInt32 time = 0)
    {
        /* 春节期间注释掉
        UInt32 begin = GVAR.GetVar(GVAR_YEARHAPPY_RANK_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_YEARHAPPY_RANK_END);
        UInt32 now = TimeUtil::Now()+time;
        if( now >= begin && now <= end)
            _happyFire = true;
        else
            _happyFire = false;
        */
        return _happyFire;
    } 
    inline static bool getLuckyMeet(UInt32 time = 0)
    {
        UInt32 begin = GVAR.GetVar(GVAR_LUCKYMEET_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_LUCKYMEET_END);
        UInt32 now = TimeUtil::Now() + time;
         if( now >= begin && now <= end)
            return true;
        else
            return false;
    } 
    inline static bool getSummerMeetTime(UInt32 time = 0)
    {
        UInt32 begin = GVAR.GetVar(GVAR_SUMMER_MEET_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_SUMMER_MEET_END);
        UInt32 now = TimeUtil::Now() + time;
        if(begin == 0 && end == 0)
            return _summerMeet;
        if( now >= begin && now <= end)
            return true;
        else
            return false;
    }

    inline static bool getQiShiBanTime(UInt32 time = 0)
    {
        UInt32 begin = GVAR.GetVar(GVAR_QISHIBANGAME_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_QISHIBANGAME_END);
        UInt32 now = TimeUtil::Now() + time;
        if(begin == 0 && end == 0)
            return _qishiban;
        if( now >= begin && now <= end)
            return true;
        else
            return false;
    } 

    inline static void setRYHBActivity(bool v)
    {
        UInt32 opTime = TimeUtil::MkTime(cfg.openYear, cfg.openMonth, cfg.openDay);
        UInt32 time20130531 = TimeUtil::MkTime(2013, 5, 31);
        if(opTime >= time20130531)
            return;

        UInt32 begin = GVAR.GetVar(GVAR_RYHB_ACTIVITY_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_RYHB_ACTIVITY_END);
        UInt32 now = TimeUtil::Now();
        if( now >= begin && now <= end)
            return;
        _ryhbActivity = v;
    }

    inline static bool getRYHBActivity()
    {
        UInt32 begin = GVAR.GetVar(GVAR_RYHB_ACTIVITY_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_RYHB_ACTIVITY_END);
        UInt32 now = TimeUtil::Now();
        if(begin != 0 && end != 0)
        {
            if(_ryhbActivity)
            {
                if(now < begin || now > end)
                    _ryhbActivity = false;
            }
            else
            {
                if(now >= begin && now <= end)
                    _ryhbActivity = true;
            }
        }

        return _ryhbActivity;
    }

    inline static bool getZCJBActivity()
    {
        UInt32 begin = GVAR.GetVar(GVAR_ZCJB_ACTIVITY_BEGIN);
        UInt32 end = GVAR.GetVar(GVAR_ZCJB_ACTIVITY_END);
        UInt32 now = TimeUtil::Now();
        if(begin != 0 && end != 0)
        {
            if(_zcjbActivity)
            {
                if(now < begin || now > end)
                    _zcjbActivity = false;
            }
            else
            {
                if(now >= begin && now <= end)
                    _zcjbActivity = true;
            }
        }

        return _zcjbActivity;
    }

    inline static void setHalfGold(bool v)
    { _halfgold = v; }
    inline static bool getHalfGold()
    { return _halfgold; } 

    inline static void setQQBoardLoginTime(bool v)
    { _qqBoardLogin = v; }
    inline static bool getQQBoardLoginTime()
    { return _qqBoardLogin; }

    inline static void setJiqirenAct(bool v)
    { _jiqirenAct = v; }
    inline static bool getJiqirenAct()
    { return _jiqirenAct; }

    inline static void setCallSnakeEggAct(UInt8 v)
    { _callsnakeeggact = v; }
    inline static UInt8 getCallSnakeEggAct()
    { return _callsnakeeggact; }
    inline static void setSnakeEggAwardAct(UInt8 v)
    { _snakeeggawardact = v; }
    inline static UInt8 getSnakeEggAwardAct()
    { return _snakeeggawardact; }
     inline static void setItem9344Act(bool v)
    { _item9344act = v; }
    inline static bool getItem9344Act()
    { return _item9344act; }
    inline static void setItem9343Act(bool v)
    { _item9343act = v; }
    inline static bool getItem9343Act()
    { return _item9343act; }
    inline static void setAutoBattleAct(bool v)
    { _autobattleact = v; }
    inline static bool getAutoBattleAct()
    { return _autobattleact; }
     inline static void setSnakeSpringEquipAct(UInt8 v)
    { _snakespringequipact = v; }
    inline static UInt8 getSnakeSpringEquipAct()
    { return _snakespringequipact; }
    inline static UInt32 getOpenTime()
    {
        UInt32 opTime = TimeUtil::MkTime(cfg.openYear, cfg.openMonth, cfg.openDay);
        return opTime;
    }
    inline static bool inActive_opTime_20130531(UInt32 now = TimeUtil::Now())
    {
        UInt32 opTime = TimeUtil::MkTime(cfg.openYear, cfg.openMonth, cfg.openDay);
        UInt32 time20130531 = TimeUtil::MkTime(2013, 5, 31);

        return ((opTime >= time20130531) && (now < (opTime + 7 * 86400)));
    }
    inline static bool isRPServer()
    {return cfg.rpServer;}
    inline static bool getGMCheck()
    {return cfg.GMCheck;}
    inline static bool inActive_new()
    {
        UInt32 opTime = TimeUtil::MkTime(cfg.openYear, cfg.openMonth, cfg.openDay);
        UInt32 actTime_new1 = TimeUtil::MkTime(2013, 9, 8);
        UInt32 actTime_new2 = TimeUtil::MkTime(2013, 9, 13);

        return !(opTime >= actTime_new1 && opTime <= actTime_new2);
    }
 
public:
	inline static UInt8 getWeekDay()
	{ return _wday; }
	static void makeActivityInfo(Stream &st);
	void setWeekDay(UInt8 wday);

    void RankLuckyDraw(Player* player, bool notify = true);
    void SendLuckyDrawList(Player* player);
    void SendLuckyDrawAward();
public:
	static void calWeekDay( World * world );
    inline static void setSysDailogPlatform(UInt8 v) { m_sysDailogPlatform = v; }
    inline static UInt8 getSysDailogPlatform() { return m_sysDailogPlatform; }
    static Player* getSpreadKeeper();
    static UInt32 getSpreadCount();
    static UInt32 FindTheOldMan(Player* pl); // 找到圣诞老人
public:
    static UInt32 _moneyLogged;
    static MoneyIn _moneyIn[7][2];
    static stOldMan _oldMan;

	static int _activityStage;
	static bool _actAvailable;//??????+6??
	static bool _actAvailable1;//??????+7??
	static UInt8 _wday;
    static bool _autoHeal;//disable autoheal
    static bool _copyfrontwin;//disable copyfrontwin
	static bool _isNewServer;
    static bool _nationalDay;
    static bool _halloween;
    static bool _singleday;
    static bool _thanksgiving;
    static bool _christmas;
    static bool _accrecharge;
    static bool _newyear;
    static bool _blueactiveday;
    static bool _rechargeactive;
    static bool _rechargeactive3366;
    static bool _yearact;
    static bool _qgamegiftact;
    static bool _spreadAct;
    static UInt8 _rechargeactiveno;
    static bool _valentineday;
    static bool _netvalentineday;
    static bool _girlday;
    static bool _whiteloveday;
    static bool _trumpenchret;
    static bool _foolsday;
    static bool _chingming;
    static bool _carnival;
    static bool _rc7day;
    static bool _shuoshuo;
    static bool _cfriend;
    static bool _cfriendAct;
    static bool _mayday;
    static bool _mayday1;
    static bool _ydmdact;
    static bool _ssdtact;
    static bool _fighter1368;
    static bool _enchantact;
    static bool _trainfighter;
    static bool _gemmergeact;
    static bool _bluediamondact;
    static bool _yellowdiamondact;
    static bool _qqvipact;
    static bool _fallact;
    static bool _qqgameact;
    static bool _3366privilegeact;
    static bool _qzonepyprivilegeact;
    static bool _goodvoiceact;
    static bool _3366giftact;
    static bool _qzongpygiftact;
    static void* _recalcwd;
    static void* _swBosstimer;
    static bool _june;
    static bool _june1;
    static bool _july;
    static bool _qixi;
    static bool _wansheng;
    static bool _qingren;
    static bool _specialbook;
    static bool _11Act;
    static bool _ssToolbarAct;
    static bool _snowAct;
    static bool _goldSnakeAct;
    static bool _collectCardAct;
    static bool _halloweenAct;
    static bool _heroIslandAct;
    static bool _dragonKingAct;
    static bool _saveGoldAct;
    static bool _feastloginAct;
    static bool _newYearGiveGiftAct;
    static bool _newYearQQGameAct;
    static bool _QZoneQQGameAct;
    static bool _newYearQzoneContinueAct;
    static bool _maydayloginAct;
    static UInt8 _towerloginAct;
    static bool _guoqing;
    static bool _9215Act;
    static bool _enchant_gt11;
    static bool _rechargenextret;
    static UInt32 _rechargenextretstart;
    static UInt32 _rechargenextretend;
    static bool _mergeathact;
    static bool _fourcopact;
    static bool _duanwu;
    static bool _icact;
    static UInt32 _levelawardend;
    static bool _pexpitems;
    static UInt32 _sosomapbegin;
    static bool _opentest;
    static bool _consumeactive;
    static bool _consume918;
    static bool _consumeawardact;
    static bool _summerFlow;
    static bool _summerMeet;
    static bool _needrechargerank;
    static bool _needconsumerank;
    static bool _killMonsteract;
    static UInt32 _rechargebegin;
    static UInt32 _rechargeend;
    static UInt32 _consumebegin;
    static UInt32 _consumeend;
    static bool  _townReward_10_15;
    static bool _loginAward;
    static bool _bluediamonSuperman;
    static bool _tgcevent;
    static bool _weiboact;
    static bool _qqgameonlineawardact;
    static bool _peteggact;
    static bool _longyuanact;
    static stArenaExtra stArenaOld[2];
    static stArenaExtra stArena;
    static bool _compassact;
    static bool _foolbao;
    static bool _summerFlow3;
    static bool _surnamelegend;
    static bool _speedTime;
    static bool _happyFire;
    static bool _11time;
    static bool _qishiban;
    static bool _ggtime;
    static bool _qzoneRechargetime;
    static bool _ryhbActivity;
    static bool _zcjbActivity;
    static bool _halfgold;
    static bool _qqBoardLogin;
    static bool _jiqirenAct;
    static UInt8 _callsnakeeggact;
    static UInt8 _snakeeggawardact;
    static bool _item9344act;
    static bool _item9343act;
    static bool _autobattleact;
    static UInt8 _snakespringequipact;
    static UInt8 m_sysDailogPlatform;
    static Player* spreadKeeper;
    static UInt32 spreadBuff;
    static UInt8 _arenaState;      //0:无 1:仙界第一 2:仙界至尊 3:仙界传奇
    static bool _memcinited;
    static bool _miluzhijiao;
    static bool _buyfund;
public:
    static RCSortType qishibanScoreSort;     //七石板积分排名
    static RCSortType rechargeSort;
    static RCSortType consumeSort;
    static RCSortType popularitySort;
    static RCSortType LuckyBagSort;
    static RCSortType PlayerGradeSort; //十一活动
    static ClanGradeSort clanGradeSort; // 十一活动
    static RCSortType guangGunSort; //十一活动
    static RCSortType happyFireSort;     //七石板积分排名
    static void initRCRank();
    static void initRP7RCRank();

    static RCSortType killMonsterSort[4];
    static RCSortType rechargeRP7Sort;
protected:
	inline UInt8 TID() const { return WORKER_THREAD_WORLD; }
	void OnTimer();

	bool Init();
	void UnInit();

	std::string GetLogName();

private:
	static void World_testUpdate(World *);
	static void World_Leaderboard_Update( void * );
	static void World_ChatItem_Purge( void * );
    static void World_Store_Check(void *);
	static void World_Multi_Check( World * );
	static void World_Midnight_Check( World * );
    static void World_CreateNewDB_Check();
	static void World_Online_Log( void * );
	static void World_Athletics_Check( void * );
    static void World_Boss_Refresh(void*);
    static void World_Boss_Prepare(void*);
    static void World_OldMan_Refresh(void*);   //圣诞老人刷新
    static void World_MarryBoard_Refresh(void*);   //圣诞老人刷新
    static void World_Marry_Process(void*);   //婚礼执行
    static void Hero_Island_Process(void*);
    static void Team_Copy_Process(void*);
	static void World_One_Min( World * );
    static void AthleticsPhysicalCheck(void *);
	static void Tianjie_Refresh(void*);
	static void DaysRank_Refresh(void*);
    static void SendQQGameGift(void*);
    static void TownDeamonTmAward(void *);
    static void ArenaExtraActTimer(void *);
    static void ClanCopyCheck(void *);
    static void ClanStatueCheck(void *);
    static void SendPopulatorRankAward(void*);
    //static void advancedHookTimer(void *para);
public:
	static void ReCalcWeekDay( World * );
	static void ServerWarBoss_Refresh( World * );

#ifndef _WIN32
public:
    static CUserLogger* ulog;
    static void udpLog(const char* str1, const char* str2, const char* str3, const char* str4,
            const char* str5, const char* str6, const char* type);
#endif

public:
    bool MemCachInit();
    void SetMemCach_qishiban(UInt32 score, const char * openId, const char * name, UInt8 fighterId);
    void SetMemCach_qishibanInfo(const char * openId, const char * name, UInt8 fighterId);
    UInt32 GetMemCach_qishibanScore(const char * openId);
    bool GetMemCach_qishibanInfo(const char * openId, std::string& info);
    void SetMemCach_CFriend_Invited(UInt64);
    UInt16 GetMemCach_CFriend_Invited(UInt64);
    void DelMemCach_CFriend_Invited(UInt64);
    void DelMemCach_CFriend_InvitedAct(UInt64);
    void SetMemCach_CFriend_InvitedAct(UInt64);
    UInt16 GetMemCach_CFriend_InvitedAct(UInt64);
    void UpdateQixiScore(Player* pl, Player* lover);
    void sendGuangGunPlayers(Player* pl);
    void sendQixiPlayers(Player* pl);
    void DivorceQixiPair(Player* pl);
    void LoadQixiScore(Player* pl, Player* lover);
    void SendQixiAward();
    void SendGuoqingAward();
    void sendQixiScoreAward(Player* pl);
    void SendXiaoyaoAward();
    void SendGoldSnakeAward();
    void SendItem9344Award();
    void SendItem9343Award();
    void SendFoolBaoAward();
    void SendSurnameLegendAward();
    void Send11AirBookAward();
    void Send11CountryRankAward();
    void Send11PlayerRankAward();
    void Send11ClanRankAward();
    void UpdateSnowScore(Player* pl, Player* lover);
    void sendSnowPlayers(Player* pl);
    void DivorceSnowPair(Player* pl);
    void LoadSnowScore(Player* pl, Player* lover);
    void SendSnowAward();
    void SnowClear();
    void SendQiShiBanAward();
    void SendGuangGunAward();
    static UInt16 GetRandomSpot();
    void SendHappyFireAward();

    void killMonsterAppend(Stream& st, UInt8 index);
    void killMonsterInit();
    void UpdateKillMonsterRank(Player* pl, UInt8 Type, UInt8 count);

    void UpdateGuangGunScore(Player* pl);
    void CreateMarryBoard(UInt64 man , UInt64 woman ,UInt8 type, UInt32 time);
    static void SendRechargeRP7RankAward();
private:
	void testUpdate();
	Script::WorldScript * _worldScript;
	Script::BattleFormula * _battleFormula;
	UInt32 _now;
	UInt32 _today;
	UInt32 _announceLast;
    std::vector<UInt32> _domain_nums;
    LuckyDrawList _luckyDrawList;
    LuckyDrawRankList _luckyDrawRankList;

    QixiScoreMap _qixiScoreMap;
    QixiPlayerSet _qixiPlayerSet;
    SnowScoreMap _snowScoreMap;
    SnowPlayerSet _snowPlayerSet;
    MCached m_MCached; // 注意：该m_MCached只用于世界线程；
};

    void CreateNewDB(UInt32 mon = 0, UInt32 year = 2011);

}

#endif

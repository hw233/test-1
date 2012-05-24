#ifndef WORLD_INC
#define WORLD_INC

#include "MsgHandler/WorldMsgHandler.h"
#include "Server/WorkerThread.h"
#include "Common/Stream.h"
#include "Server/Cfg.h"

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

struct MoneyIn
{
    int gold;
    int coupon;
    int tael;
    int achievement;
    int prestige;
};

typedef std::list<Player*> LuckyDrawList;
typedef LuckyDrawList::iterator LuckyDrawRank;
typedef LuckyDrawList::reverse_iterator RLuckyDrawRank;
typedef std::map<Player *, LuckyDrawRank> LuckyDrawRankList;

class World:
	public WorkerRunner<WorldMsgHandler>
{
public:
	World();
	~World();

public:
	inline UInt32 ThisDay() { return _today; }
	inline UInt32 Now() { return _now; }

    inline static bool isFBVersion() { return cfg.fbVersion; }
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
    inline static void setQQGameAct(bool v)
    { _qqgameact = v; }
    inline static bool getQQGameAct()
    { return _qqgameact; }

    inline static void setTrainFighter(bool v)
    { _trainfighter = v; }
    inline static bool getTrainFighter()
    { return _trainfighter; }
    inline static void setGemMergeAct(bool v)
    { _gemmergeact = v; }
    inline static bool getGemMergeAct()
    { return _gemmergeact; }


    inline static void setJune(bool v)
    { _june = v; }
    inline static bool getJune()
    { return _june; }
    inline static void setJune1(bool v)
    { _june1 = v; }
    inline static bool getJune1()
    { return _june1; }

	inline Script::WorldScript * getWorldScript() { return _worldScript; }
	inline Script::BattleFormula * getBattleFormula() { return _battleFormula; }

public:
	inline static UInt8 getWeekDay()
	{ return _wday; }
	static void makeActivityInfo(Stream &st);
	void setWeekDay(UInt8 wday);

    void RankLuckyDraw(Player* player);
    void SendLuckyDrawList(Player* player);
    void SendLuckyDrawAward();

public:
	static void calWeekDay( World * world );

public:
    static UInt32 _moneyLogged;
    static MoneyIn _moneyIn[7][2];

	static int _activityStage;
	static bool _actAvailable;//开服到+6日
	static bool _actAvailable1;//开服到+7日
	static UInt8 _wday;
    static bool _autoHeal;//disable autoheal
	static bool _isNewServer;
    static bool _nationalDay;
    static bool _halloween;
    static bool _singleday;
    static bool _thanksgiving;
    static bool _christmas;
    static bool _newyear;
    static bool _blueactiveday;
    static bool _rechargeactive;
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
    static bool _qqgameact;
    static void* _recalcwd;
    static bool _june;
    static bool _june1;

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
	static void World_Multi_Check( World * );
	static void World_Midnight_Check( World * );
    static void World_CreateNewDB_Check();
	static void World_Online_Log( void * );
	static void World_Athletics_Check( void * );
    static void World_Boss_Refresh(void*);
    static void World_Boss_Prepare(void*);
    static void Hero_Island_Process(void*);
    static void Team_Copy_Process(void*);
	static void ReCalcWeekDay( World * );

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
};

    void CreateNewDB(UInt32 mon = 0, UInt32 year = 2011);

}

#endif

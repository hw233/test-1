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

	inline Script::WorldScript * getWorldScript() { return _worldScript; }
	inline Script::BattleFormula * getBattleFormula() { return _battleFormula; }

public:
	inline static UInt8 getWeekDay()
	{ return _wday; }
	static void makeActivityInfo(Stream &st);
	void setWeekDay(UInt8 wday);

public:
	static void calWeekDay();

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
    static bool _girlday;
    static bool _whiteloveday;
    static bool _trumpenchret;
    static bool _foolsday;
    static bool _chingming;
    static bool _carnival;

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

private:
	void testUpdate();
	Script::WorldScript * _worldScript;
	Script::BattleFormula * _battleFormula;
	UInt32 _now;
	UInt32 _today;
	UInt32 _announceLast;
    std::vector<UInt32> _domain_nums;
};

    void CreateNewDB(UInt32 mon = 0, UInt32 year = 2011);

}

#endif

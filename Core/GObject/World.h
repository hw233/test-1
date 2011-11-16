#ifndef WORLD_INC
#define WORLD_INC

#include "MsgHandler/WorldMsgHandler.h"
#include "Server/WorkerThread.h"
#include "Common/Stream.h"

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

class World:
	public WorkerRunner<WorldMsgHandler>
{
public:
	World();
	~World();

public:
	inline UInt32 ThisDay() { return _today; }
	inline UInt32 Now() { return _now; }
	inline static void setActivityStage(int stage)
	{ _activityStage = stage; }
	inline static int getActivityStage()
	{ return _activityStage; }
	
	inline static void setActAvailable(bool aa)
	{ _actAvailable = aa; }
	inline static void setActAvailable1(bool aa)
	{ _actAvailable1 = aa; }

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
	static int _activityStage;
	static bool _actAvailable;//开服到+6日
	static bool _actAvailable1;//开服到+7日
	static UInt8 _wday;
	static bool _isNewServer;
    static bool _nationalDay;
    static bool _halloween;
    static bool _singleday;
    static bool _thanksgiving;

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

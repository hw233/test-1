#ifndef COUNTRY_INC
#define COUNTRY_INC

#include "Server/WorkerRunner.h"
#include "Map.h"
#include "GObjectManager.h"
#include "MsgHandler/CountryMsgHandler.h"


namespace Script
{
	class GameActionLua;
	class BattleFormula;
}

class Log;
namespace GObject
{

class Player;

class Country:
	public WorkerRunner<CountryMsgHandler>
{
public:
	Country(UInt8 id);
	virtual ~Country();

public:
	inline UInt8 GetThreadID() const
	{
		return m_ThreadID;
	}

	inline Script::GameActionLua* GetGameActionLua()
	{
		return m_GameActionLua;
	}

	inline Script::BattleFormula* GetBattleFormula()
	{
		return m_BattleFormula;
	}

public:
	bool PlayerMapSwitch(UInt8 index, UInt32 playerId);		//???ҿ??????л?????
	void MergePendingPlayerList();
	void PlayerEnter(Player * pl, bool = true);
	void PlayerLeave(Player * pl);

	Player * FindPlayer(UInt32 slot);

	void Broadcast(UInt16, Stream&);

protected:
	bool Init();
	void UnInit();
	UInt8 TID() const { return m_ThreadID; }
	std::string GetLogName();

private:
	static void Country_Boss_Check(void *);
	static void Country_Battle_Check(void *);
    static void Hero_Island_Check(void *);
    static void ClanRankBattleCheck(void *);
    static void TownDeamonTmAward(void *);

    static void ClanCopyCheck(void *);
    static void ClanCopyResetBegin(void *);
    static void ClanCopyReset(void *);
    static void ClanCopyResetEnd(void *);

private:
	UInt8		m_ThreadID;					//?????߳?

	std::vector<Map *>	m_MapManager;

	Script::GameActionLua*	m_GameActionLua;
	Script::BattleFormula*	m_BattleFormula;

	typedef GLocalObjectManagerT<Player> LocalPlayers;

	LocalPlayers m_Players;
};

}

#define CURRENT_COUNTRY() WorkerThread<GObject::Country>::LocalWorker()
#define GameAction() CURRENT_COUNTRY().GetGameActionLua()

#endif

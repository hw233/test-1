﻿#ifndef COUNTRY_INC
#define COUNTRY_INC

#include "Server/WorkerRunner.h"
#include "GObjectManager.h"
#include "MsgHandler/CountryMsgHandler.h"
#include "Script/GameActionLua.h"
namespace Script
{
	class GameActionLua;
	class BattleFormula;
}

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
	void MergePendingPlayerList();
	void PlayerEnter(Player * pl, bool = true);
	void PlayerLeave(Player * pl);

	Player * FindPlayer(UInt32 slot);
protected:
	bool Init();
	void UnInit();
	UInt8 TID() const { return m_ThreadID; }
	std::string GetLogName();

    
private:
	UInt8		m_ThreadID;					//?????߳?

	Script::GameActionLua*	m_GameActionLua;
	Script::BattleFormula*	m_BattleFormula;

	typedef GLocalObjectManagerT<Player> LocalPlayers;

	LocalPlayers m_Players;
};

}

#define CURRENT_COUNTRY() WorkerThread<GObject::Country>::LocalWorker()
#define GameAction() CURRENT_COUNTRY().GetGameActionLua()

#endif

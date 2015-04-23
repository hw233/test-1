#include "Config.h"
#include "Country.h"
#include "GData/GDataManager.h"
#include "Script/GameActionLua.h"
#include "Script/BattleFormula.h"
#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Log/Log.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"

namespace GObject
{

Country::Country(UInt8 tid): m_ThreadID(tid), m_GameActionLua(NULL), m_BattleFormula(NULL)
{
}

Country::~Country()
{

}

bool Country::Init()
{
    //GameActionLua
    std::string path = cfg.scriptPath + "main.lua";
    m_GameActionLua = new Script::GameActionLua(TID(), path.c_str());
    path = cfg.scriptPath + "formula/main.lua";
    m_BattleFormula = new Script::BattleFormula(path.c_str());

	return true;
}


void Country::UnInit()
{
	SAFE_DELETE(m_GameActionLua);
}

std::string Country::GetLogName()
{
	return "log/Country" + Itoa(TID()) + "/";
}


Player * Country::FindPlayer( UInt32 slot )
{
	return m_Players[slot];
}

}

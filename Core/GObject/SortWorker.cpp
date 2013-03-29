
#include "Config.h"
#include "SortWorker.h"
#include "GObject/Leaderboard.h"
#include "Script/GameActionLua.h"
#include "Script/BattleFormula.h"
#include "Server/Cfg.h"

namespace GObject
{
    SortWorker::SortWorker(UInt8 type, UInt8 worker) :
#ifndef _DEBUG
        WorkerRunner<>(60*60*1000), m_Type(type), m_Worker(worker)
#else
        WorkerRunner<>(5*60*1000), m_Type(type), m_Worker(worker)
#endif
    {}

    SortWorker::~SortWorker()
    {}

    bool SortWorker::Init()
    {
       //GameActionLua
	   std::string path = cfg.scriptPath + "main.lua";
	   m_GameActionLua = new Script::GameActionLua(TID(), path.c_str());
	   path = cfg.scriptPath + "formula/main.lua";
	   m_BattleFormula = new Script::BattleFormula(path.c_str());
	
       return true;
    }

    void SortWorker::UnInit()
    {
    }

    void SortWorker::OnTimer()
    {
        leaderboard.begin();
        TRACE_LOG("Sorting Begin...");
        leaderboard.update();
        TRACE_LOG("Sorting End...");
        leaderboard.end();
    }

    void SortWorker::OnPause()
    {
    }

    std::string SortWorker::GetLogName()
    {
        return "log/Sort/";
    }

}


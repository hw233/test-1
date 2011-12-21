 
#include "Config.h"
#include "SortWorker.h"
#include "GObject/Leaderboard.h"

namespace GObject
{
    SortWorker::SortWorker(UInt8 type, UInt8 worker) :
        WorkerRunner<>(60*60*1000), m_Type(type), m_Worker(worker)
    {}

    SortWorker::~SortWorker()
    {}

    bool SortWorker::Init()
    {
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


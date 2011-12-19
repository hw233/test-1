 
#include "Config.h"
#include "SortWorker.h"
#include "GObject/Leaderboard.h"

namespace GObject
{
    SortWorker::SortWorker(UInt8 type, UInt8 worker) :
        //WorkerRunner<>(60*60*1000), m_Type(type), m_Worker(worker)
        WorkerRunner<>(60*1000), m_Type(type), m_Worker(worker), m_inited(false)
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
        leaderboard.update();
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


#include "Config.h"
#include "LoadWorker.h"
#include "Player.h"

#include "Server/Cfg.h"
#include "Common/SHA1Engine.h"
#include "Common/LoadingCounter.h"
#include "GObjectDBExecHelper.h" 
#include <sstream>

namespace GObject
{
    LoadWorker::LoadWorker(UInt8 type, UInt8 worker) :
        WorkerRunner<>(25), m_Type(type), m_Worker(worker)
    {

    }

    LoadWorker::~LoadWorker()
    {

    }

    bool LoadWorker::Init()
    {
        return true;
    }

    std::string LoadWorker::GetLogName()
    {
        return "log/LoadWorker/";
    }

    void LoadWorker::UnInit()
    {
        OnTimer();
    }

    void LoadWorker::OnTimer()
    {
        std::vector<UrlParam> list;
        {
            FastMutex::ScopedLock lk(m_Mutex);
            list.swap(m_QueryList);
        }

        for (std::vector<UrlParam>::iterator it = list.begin(); it != list.end(); ++ it)
        {
            GObject::Player * pl = GObject::globalPlayers[it->playerId];
            if(!pl)
            {
                continue;
            }

            if(0 == it->type)
            {
                //GetMailInfo(it->playerId);
            }
        }
    }

    void LoadWorker::OnPause()
    {

    }

    void LoadWorker::Push(UInt64 playerId, UInt16 type)
    {
        UrlParam urlParam;
        urlParam.playerId = playerId;
        urlParam.type = type;

        FastMutex::ScopedLock lk(m_Mutex);
        m_QueryList.push_back(urlParam);
    }

    bool LoadWorker::GetMailInfo(UInt64 playerId)
    {
        return true;
    }
}


#include "Config.h"
#include "DCWorker.h"

namespace GObject
{
    DCWorker::DCWorker(UInt8 type, UInt8 worker) :
        WorkerRunner<>(200), m_Type(type), m_Worker(worker),
        m_Limit(0), m_logger(NULL), m_inited(false)
    {}

    DCWorker::~DCWorker()
    {
        if (m_inited) delete m_logger;
    }

    bool DCWorker::Init()
    {
#ifndef _DEBUG
        m_logger = new (std::nothrow) CLogger();
        if (!m_logger)
            return false;

        std::string appname = "appoperlog";
        if (m_logger->init(appname))
        {
            ERROR_LOG("Init DCLogger FAILED");
            return true;
        }
        else
        {
            TRACE_LOG("Init DCLogger SUCCESS");
        }
        m_inited = true;
#endif
        return true;
    }

    void DCWorker::UnInit()
    {
        OnTimer();
    }

    void DCWorker::OnTimer()
    {
        std::vector<const char *> l;
        {
            FastMutex::ScopedLock lk(m_Mutex);
            if(m_DCLog.empty())
                return;

            l = m_DCLog;
            m_DCLog.clear();
            m_Limit = 0;
        }

        if (!l.empty())
        {
            size_t size = l.size();
            size_t sz = size;
            const char** msg = &l[0];
            while (size)
            {
                --size;
                bool r = false;
#ifndef _DEBUG
                if (*msg)
                {
                    std::string data = *msg;
                    if (m_inited && m_logger && !m_logger->write_baselog(LT_BASE, data, true))
                        r = true;
                }
#endif
                if (*msg)
                    TRACE_LOG("[%u]%u:%u-[%s] -> %d", m_Worker, sz, size, *msg, r ? 1 : 0);

                delete[] *msg;
                ++msg;
            }
        }
    }

    void DCWorker::OnPause()
    {
    }

    std::string DCWorker::GetLogName()
    {
        return "log/DC/";
    }

    void DCWorker::Push(const char* msg, size_t len)
    {
        if (!msg)
            return;

        char *p = new(std::nothrow) char[len+1];
        if (p == NULL)
            return;
        memcpy(p, msg, len);
        p[len] = '\0';

        ++m_Limit;

        FastMutex::ScopedLock lk(m_Mutex);
        m_DCLog.push_back(p);
    }
}


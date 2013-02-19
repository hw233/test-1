
#include "Config.h"
#include "DCWorker.h"
#include "Server/Cfg.h"

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
        std::vector<LogMsg> log;
        {
            FastMutex::ScopedLock lk(m_Mutex);
            log.swap(m_DCLog);
        }
        if (!log.empty())
        {
            size_t size = log.size();
            size_t i = 0;
            while (i < size)
            {
                const char* msg = log[i].logString;
                const char logType = log[i].logType;
                bool r = false;
#ifndef _DEBUG
                if (msg)
                {
                    std::string data = msg;
                    if (m_inited && m_logger && !m_logger->write_baselog(logType, data, (logType == LT_SECDATA)? false:true))
                        r = true;
                }
#endif
                if (msg && logType != LT_SECDATA)
                {
                    TRACE_LOG("[%u]%u:%u-[%s] -> %d", m_Worker, i, size, msg, r ? 1 : 0);
                    TRACE_LOG("logType = %u", (UInt32)logType);
                }
                if (logType == LT_SECDATA && cfg.secdclogTest && cfg.isTestPlatform)
                {
                    TRACE_LOG("[%u]%u:%u-[%s] -> %d", m_Worker, i, size, msg, r ? 1 : 0);
                }

                delete[] msg;
                ++i;
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

    void DCWorker::Push(const char* msg, size_t len, const char logType /* =  LT_NORMAL */)
    {
        if (!msg)
            return;

        char *p = new(std::nothrow) char[len+1];
        if (p == NULL)
            return;
        memcpy(p, msg, len);
        p[len] = '\0';

        LogMsg logMsg;
        logMsg.logString = p;
        logMsg.logType = logType;

        ++m_Limit;

        FastMutex::ScopedLock lk(m_Mutex);
        m_DCLog.push_back(logMsg);
    }
}


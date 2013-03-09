
#include "Config.h"
#include "DCWorker.h"
#include "Server/Cfg.h"


extern "C" {
#include "../../tools/json-1.4/json.h"
}

namespace GObject
{
    static int recvret(char* data, size_t size, size_t nmemb, char* buf)
    {
#ifndef MAX_RET_LEN
#define MAX_RET_LEN 1024
#endif
        size_t nsize = size * nmemb;
        if (nsize > MAX_RET_LEN) {
            memcpy(buf, data, MAX_RET_LEN);
            return MAX_RET_LEN;
        }

        memcpy(buf, data, nsize);
        return nsize;
#undef MAX_RET_LEN
    }

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
        curl = curl_easy_init();
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
        std::vector<LogMsg> unionLog;
        {
            FastMutex::ScopedLock lk(m_Mutex);
            log.swap(m_DCLog);
            unionLog.swap(m_UnionLog);
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
        if (!unionLog.empty())
        {
            // 游戏联盟的上报
#ifndef MAX_RET_LEN
#define MAX_RET_LEN 1024
#endif
            size_t size = unionLog.size();
            size_t i = 0;
            while (i < size)
            {
                const char* msg = unionLog[i].logString;
                const static char host[] = "http://union.tencentlog.com/cgi-bin/Register.cgi?";
                Int32 ret = -9999;
                char res[MAX_RET_LEN] = "";
#ifndef _DEBUG
                if (msg)
                {
                    std::string data = msg;
                    snprintf (res, MAX_RET_LEN, "%s%s", host, msg);
                    // curl调用上报
                    char buffer[MAX_RET_LEN] = {0};
                    curl_easy_setopt(curl, CURLOPT_URL, res);
                    puts(res);
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recvret);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
                    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1);
                    CURLcode curlRes = curl_easy_perform(curl);
                    if (CURLE_OK == curlRes)
                    {
                        char msg[MAX_RET_LEN] = "";
                        ret = UnionLoggerResultParse(buffer, msg);
                    }
                }
#endif
                TRACE_LOG("[%u]%u:%u-[%s] -> %d", m_Worker, i, size, msg, ret);

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
        if (logType == UNION_DC_TYPE)
        {
            m_UnionLog.push_back(logMsg);
        }
        else
        {
            m_DCLog.push_back(logMsg);
        }
    }

    UInt32 DCWorker::UnionLoggerResultParse(char* result, char* msg)
    {
        // 上报结果解析
        #define JSON_ERR_CUSTOM -9527
        #define JSON_ERR_CUSTOM2 -9528
        #define JSON_ERR_CUSTOM3 -9529
        #define JSON_ERR_CUSTOM4 -9530

        TRACE_LOG("JSON RESULT: %s", result);
        json_t* obj = NULL;


        enum json_error jerr;
        if ((jerr = json_parse_document(&obj, result)) != JSON_OK)
            return JSON_ERR_CUSTOM;

        json_t* hdr = json_find_first_label(obj, "ret");
        if (!hdr || !hdr->child || !hdr->text)
            return JSON_ERR_CUSTOM2;

        Int32 ret = atoi(hdr->child->text);
        if (ret)
            return ret;

        hdr =  json_find_first_label(obj, "msg");
        if (!hdr || !hdr->child || !hdr->text)
            return JSON_ERR_CUSTOM3;
        ret = atoi(hdr->child->text);
        UInt32 len = strlen(hdr->child->text);
        strncpy (msg, hdr->child->text, len + 1);
        msg[len] = '\0';
        return 0;
        #undef JSON_ERR_CUSTOM
        #undef JSON_ERR_CUSTOM2
        #undef JSON_ERR_CUSTOM3
        #undef JSON_ERR_CUSTOM4
    }
}


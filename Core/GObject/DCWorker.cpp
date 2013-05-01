
#include "Config.h"
#include "DCWorker.h"
#include "Server/Cfg.h"
#include "Player.h"


extern "C" {
#include "../../tools/json-1.4/json.h"
}

namespace GObject
{
#ifndef _DEBUG
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
#endif

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
        size_t sz = cfg.IDQueryMemcached.size();
        for (size_t i = 0; i < sz; ++i)
        {
            char buf[128];
            snprintf(buf, 128, "%s:%d", cfg.IDQueryMemcached[i].ip.c_str(), cfg.IDQueryMemcached[i].port); 
            m_MCached.pushHost(buf);
        }
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
        curl = curl_easy_init();
#endif
        m_inited = true;
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
        //std::vector<OpenIdMsg> openIdLog;
        {
            FastMutex::ScopedLock lk(m_Mutex);
            log.swap(m_DCLog);

            unionLog.swap(m_UnionLog);

            //openIdLog.swap(m_Openid);
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
                Int32 ret = -9999;
#ifndef _DEBUG
                const static char host[] = "http://union.tencentlog.com/cgi-bin/Register.cgi?";
                char res[MAX_RET_LEN] = "";
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
        /*
        if (!openIdLog.empty())
        {
            size_t size = openIdLog.size();
            size_t i = 0;
            while (i < size)
            {
                char* msg = openIdLog[i].openId;
                UInt64 playerId = openIdLog[i].playerId;
                bool r = false;
                GObject::Player * pl = GObject::globalPlayers[playerId];
                if (!pl)
                {
                    delete[] msg;
                    ++i;
                    continue;
                }
//#ifndef _DEBUG
                if (msg)
                {
                    std::string data = msg;
                    if (CheckOpenId(playerId, msg))
                    {
                        r = true;
                    }
                    else
                    {
                        // 非法OpenId
                        r = false;
                        GameMsgHdr imh(0x332, pl->getThreadId(), pl, strlen(msg) + 1);
                        GLOBAL().PushMsg(imh, msg);
                    }
                }
//#endif
                TRACE_LOG("[%u]%u:%u:PlayerId: %"I64_FMT"u, OpenId: %s] -> %d", m_Worker, i, size, playerId, msg, r ? 1 : 0);

                delete[] msg;
                ++i;
            }
        }
        */
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

    void DCWorker::PushCheckOpenId(UInt64 playerId, const char * openId, UInt32 len)
    {
        /*
        if (!openId)
            return;

        char *p = new(std::nothrow) char[len+1];
        if (p == NULL)
            return;
        memcpy(p, openId, len);
        p[len] = '\0';

        OpenIdMsg openMsg;
        openMsg.openId = p;
        openMsg.playerId = playerId;

        ++m_Limit;

        {
            FastMutex::ScopedLock lk(m_Mutex);
            m_Openid.push_back(openMsg);
        }
        */
    }

    /*
    bool DCWorker::CheckOpenId(UInt64 playerId, char * openId)
    {
        // Memcached 校验playerID和openID
        if (!m_inited)
            return true;
        playerId = playerId & 0xFFFFFFFF;
        char buf[128] = {0};
        snprintf(buf, 128, "oid_%"I64_FMT"u", playerId);
        char openId2[256] = {0};
        m_MCached.get(buf, strlen(buf), openId2, 255);
        openId2[255] = '\0';
        if (strncmp(openId, openId2, strlen(openId)) == 0)
        {
            return true;
        }
        else
        {
            strcpy(openId, openId2);
            return false;
        }
    }
    */

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
    UInt8 DCWorker::CheckRPOpenid(char* openid)
    {
        if (!cfg.rpServer)
            return true;
        if (!m_inited || NULL == openid)
            return false;
        char value[32] = {0};
        char key[64] = {0};
        size_t len = snprintf(key, sizeof(key), "uid_asss_rp_%s", openid); 
        m_MCached.get(key, len, value, sizeof(value));
        value[1] = 0;
        return atoi(value);
    }
}


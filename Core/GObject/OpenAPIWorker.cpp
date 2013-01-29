#include "Config.h"

#include "OpenAPIWorker.h"

#include "Server/Cfg.h"
#include "Common/SHA1Engine.h"
#include "Player.h"
#include <openssl/hmac.h>
#include <sstream>


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
    OpenAPIWorker::OpenAPIWorker(UInt8 type, UInt8 worker) :
        WorkerRunner<>(200), m_Type(type), m_Worker(worker),
        m_Limit(0), m_inited(false)
    {
#define _APPKEY "3e3edffcc7824018920291ccfd2cfaf4"
        version = _VERSION;
        appid = _APPID;
        appkey = _APPKEY"&";
        curl = curl_easy_init();
    }

    OpenAPIWorker::~OpenAPIWorker()
    {
    }

    bool OpenAPIWorker::Init()
    {
        size_t sz = cfg.IDQueryMemcached.size();
        for (size_t i = 0; i < sz; ++i)
        {
            char buf[128];
            snprintf(buf, 128, "%s:%d", cfg.IDQueryMemcached[i].ip.c_str(), cfg.IDQueryMemcached[i].port); 
            m_MCached.pushHost(buf);
            m_inited = true;
        }
        return true;
    }

    void OpenAPIWorker::UnInit()
    {
        OnTimer();
    }

    void OpenAPIWorker::OnTimer()
    {
        std::vector<UrlParam> list;
        {
            FastMutex::ScopedLock lk(m_Mutex);
            list.swap(m_QueryList);
        }
        size_t size = list.size();
        m_Limit -= size;
        size_t i = 0;
        for (std::vector<UrlParam>::iterator it = list.begin(); it != list.end(); ++ it)
        {
#ifndef MAX_RET_LEN
#define MAX_RET_LEN 1024
#endif
            ++ i;
            GObject::Player * pl = GObject::globalPlayers[it->playerId];
            if (!pl)
            {
                delete[] it->openId;
                delete[] it->openKey;
                delete[] it->pf;
                delete[] it->userIp;
                continue;
            }

            const static char host[] = "http://openapi.tencentyun.com";
            char buffer[MAX_RET_LEN] = {0};
            char url[MAX_RET_LEN] = "";

            if (it->type == 1002)
                strncpy(url, "/v3/csec/punish_query", 22);
            else if (it->type == 0)
                strncpy(url, "/v3/user/is_login", 18);

            char res[MAX_RET_LEN] = "";

            if (!CheckOpenId(it->playerId, it->openId))
            {
                GameMsgHdr imh(0x332, pl->getThreadId(), pl, strlen(it->openId) + 1);
                GLOBAL().PushMsg(imh, it->openId);
            }


            SetUrlString (url, it->playerId, it->type, it->openId, it->openKey, it->pf, it->userIp);
            sprintf (res, "%s%s", host, url);

            int timeout = 1;

            curl_easy_setopt(curl, CURLOPT_URL, res);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recvret);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);


            CURLcode curlRes = curl_easy_perform(curl);
            Int32 ret = 0;
            if (CURLE_OK == curlRes)
            {
                bool needForbid = false;
                char msg[MAX_RET_LEN] = "";
                if (it->type == 1002)
                    ret = PunishResultParse(buffer, &needForbid, msg);
                else if (it->type == 0)
                    ret = IsLoginResultParse(buffer, msg);

                if (ret == 0)
                {
                    if (it->type == 1002)
                    {
                        // 交易punish查询结果返回
                        if(needForbid)
                        {
                            // 需要封杀交易功能
                            GameMsgHdr imh(0x330, pl->getThreadId(), pl, 0);
                            GLOBAL().PushMsg(imh, NULL);
                        }
                        else
                        {
                            // 不需要，简单记录一下
                            GameMsgHdr imh(0x331, pl->getThreadId(), pl, sizeof(ret));
                            GLOBAL().PushMsg(imh, &ret);
                        }
                    }
                    else if(it->type == 0)
                    {
                        // is_login续期openkey正确,nothing to do.
                    }
                }
                else
                {
#define MAX_MSG_LEN 1024
                    struct OpenAPIFailedInfo
                    {
                        UInt32 type;
                        Int32  ret;
                        char   msg[MAX_MSG_LEN];
                    };
                    OpenAPIFailedInfo faildInfo;
                    faildInfo.type = it->type;
                    faildInfo.ret  = ret;
                    strncpy (faildInfo.msg, msg, strlen(msg));
                    faildInfo.msg[MAX_MSG_LEN - 1] = '\0';
                    GameMsgHdr imh(0x333, pl->getThreadId(), pl, sizeof(faildInfo));
                    GLOBAL().PushMsg(imh, &faildInfo);
                }
            }
            TRACE_LOG("[%u]%u:%u[%s] -> %s, result=%d.", m_Worker, i, size, res, curlRes == CURLE_OK?"OK":"FAIL", ret);
            delete[] it->openId;
            delete[] it->openKey;
            delete[] it->pf;
            delete[] it->userIp;
#undef MAX_RET_LEN
        }
    }

    void OpenAPIWorker::OnPause()
    {
    }

    std::string OpenAPIWorker::GetLogName()
    {
        return "log/OpenAPI/";
    }

    void OpenAPIWorker::Push(UInt64 playerId, UInt16 type, const char * openId, const char * openKey, const char * pf, const char * userIp)
    {
        const static int OPEN_ID_LEN  = 64;
        const static int OPEN_KEY_LEN = 128;
        const static int PF_LEN = 64;
        const static int IP_LEN = 20;
        int len = 0;
        if (!openId || !openKey || !pf)
            return;

        UrlParam urlParam;
        urlParam.playerId = playerId;
        urlParam.type = type;

        char *p = new(std::nothrow) char[OPEN_ID_LEN+1];
        if (p == NULL)
            return;
        len = strlen(openId);
        len = len > OPEN_ID_LEN ? OPEN_ID_LEN : len;
        memcpy(p, openId, len);
        p[len] = '\0';
        urlParam.openId = p;

        p = new(std::nothrow) char[OPEN_KEY_LEN+1];
        if (p == NULL)
        {
            delete []urlParam.openId;
            return;
        }
        len = strlen(openKey);
        len = len > OPEN_KEY_LEN ? OPEN_KEY_LEN : len;
        memcpy(p, openKey, len);
        p[len] = '\0';
        urlParam.openKey = p;

        p = new(std::nothrow) char[PF_LEN+1];
        if (p == NULL)
        {
            delete []urlParam.openId;
            delete []urlParam.openKey;
            return;
        }
        len = strlen(pf);
        len = len > PF_LEN ? PF_LEN : len;
        memcpy(p, pf, len);
        p[len] = '\0';
        urlParam.pf = p;

        p = new(std::nothrow) char[IP_LEN+1];
        if (p == NULL)
        {
            delete []urlParam.openId;
            delete []urlParam.openKey;
            delete []urlParam.pf;
            return;
        }
        len = strlen(userIp);
        len = len > IP_LEN ? IP_LEN : len;
        memcpy(p, userIp, len);
        p[len] = '\0';
        urlParam.userIp = p;

        ++m_Limit;

        FastMutex::ScopedLock lk(m_Mutex);
        m_QueryList.push_back(urlParam);
    }

    void OpenAPIWorker::SetUrlString(char* url, UInt64 playerId, UInt16 type, const char * openId, const char * openKey, const char * pf, const char * userIp)
    {
        std::ostringstream strGet;
        strGet << "GET&";
        strGet << UrlEncode(url);
        strGet << "&";

        std::ostringstream sigKey;
        sigKey << "appid=";
        sigKey << (UInt32)appid;
        sigKey << "&format=json"; // XXX: 返回格式先写死为json
        sigKey << "&openid=";
        sigKey << openId;
        sigKey << "&openkey=";
        sigKey << openKey;
        sigKey << "&pf=";
        sigKey << pf;
        if (type)
        {
            sigKey << "&type=";
            sigKey << (UInt32) type;
        }
        sigKey << "&userip=";
        sigKey << userIp;
        std::string sigValue;
        sigValue += strGet.str(); 
        sigValue += UrlEncode(sigKey.str().c_str());


        unsigned int size = 0;
        unsigned char* res;
        res = HMAC(EVP_sha1(), appkey.c_str(), appkey.size(), (unsigned char *)sigValue.c_str(), sigValue.size(), NULL, &size);
        sigValue = Base64Encode(res, size);
        sigValue = UrlEncode(sigValue.c_str());
        sigKey << "&sig=";
        sigKey << sigValue;
        strcat (url, "?");
        strncat(url, sigKey.str().c_str(), sigKey.str().size() + 1);
    }

    Int32 OpenAPIWorker::PunishResultParse(char* result, bool* needForbid, char* msg)
    {
        // 解析json的结果 (封交易)
        #define JSON_ERR_CUSTOM -9527
        #define JSON_ERR_CUSTOM2 -9528
        #define JSON_ERR_CUSTOM3 -9529
        #define JSON_ERR_CUSTOM4 -9530

        json_t* obj = NULL;


        enum json_error jerr;
        if ((jerr = json_parse_document(&obj, result)) != JSON_OK)
            return JSON_ERR_CUSTOM;

        json_t* hdr = json_find_first_label(obj, "ret");
        if (!hdr && !hdr->child && !hdr->text)
            return JSON_ERR_CUSTOM2;

        Int32 ret = atoi(hdr->child->text);
        if (ret)
        {

            hdr =  json_find_first_label(obj, "msg");
            if (!hdr && !hdr->child && !hdr->text)
                return JSON_ERR_CUSTOM3;
            UInt32 len = strlen(hdr->child->text);
            strncpy (msg, hdr->child->text, len);
            msg[len] = '\0';
            return ret;
        }

        hdr =  json_find_first_label(obj, "punish");
        if (!hdr && !hdr->child && !hdr->text)
            return JSON_ERR_CUSTOM4;
        ret = atoi(hdr->child->text);
        *needForbid = ret==1? true:false;
        return 0;
        #undef JSON_ERR_CUSTOM
        #undef JSON_ERR_CUSTOM2
        #undef JSON_ERR_CUSTOM3
        #undef JSON_ERR_CUSTOM4
    }

    Int32 OpenAPIWorker::IsLoginResultParse(char * result, char* msg)
    {
        // 解析json的结果 (openkey续期)
        #define JSON_ERR_CUSTOM -9527
        #define JSON_ERR_CUSTOM2 -9528
        #define JSON_ERR_CUSTOM3 -9529
        #define JSON_ERR_CUSTOM4 -9530

        json_t* obj = NULL;


        enum json_error jerr;
        if ((jerr = json_parse_document(&obj, result)) != JSON_OK)
            return JSON_ERR_CUSTOM;

        json_t* hdr = json_find_first_label(obj, "ret");
        if (!hdr && !hdr->child && !hdr->text)
            return JSON_ERR_CUSTOM2;

        Int32 ret = atoi(hdr->child->text);
        if (ret)
            return ret;

        hdr =  json_find_first_label(obj, "msg");
        if (!hdr && !hdr->child && !hdr->text)
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

    bool OpenAPIWorker::CheckOpenId(UInt64 playerId, char * openId)
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

    std::string OpenAPIWorker::UrlEncode(const char *in_str)
    {
        // URL 编码
        int in_str_len = strlen(in_str);
        int out_str_len = 0;
        std::string out_str;
        register unsigned char c;
        unsigned char *to, *start;
        unsigned char const *from, *end;
        unsigned char hexchars[] = "0123456789ABCDEF";

        from = (unsigned char * ) in_str;
        end = (unsigned char * ) in_str + in_str_len;
        start = to = (unsigned char * ) malloc (3*in_str_len+1);

        while (from < end)
        {
            c = *from++;
            if (c==' ')
            {
                to[0] = '%';
                to[1] = '2';
                to[2] = '0';
                to += 3;
            }
            else if((c < '0' && c != '-' && c != '.')
                    || (c < 'A' && c > '9')
                    || (c > 'Z' && c < 'a' && c != '_')
                    || (c > 'z'))
            {
                to[0] = '%';
                to[1] = hexchars[c >> 4];
                to[2] = hexchars[c & 15];
                to += 3;
            }
            else
            {
                *to++ = c;
            }
        }
        *to = 0;

        out_str_len = to - start;
        out_str = (char * ) start;
        free (start);
        return out_str;
    }

    std::string OpenAPIWorker::Base64Encode(unsigned char const * byte_to_encode, unsigned int in_len)
    {
        // Base64编码
        static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];
        std::string ret;

        while(in_len --)
        {
            char_array_3[i++] = *(byte_to_encode++);
            if (i == 3)
            {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for(i = 0; i < 4; ++i)
                {
                    ret += base64_chars [char_array_4[i]];
                }
                i = 0;
            }
        }
        if (i)
        {
            for (j = i; j < 3; ++j)
            {
                char_array_3[j] = '\0';
            }

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(j = 0; j < i + 1; ++j)
            {
                ret += base64_chars [char_array_4[j]];
            }
            if(i < 3)
                ret += '=';
        }
        //std::cout << ret << std::endl; // XXX: 为什么GDB在这里我无法输出ret的数据
        return ret;
    }
}


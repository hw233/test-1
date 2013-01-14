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
            const static char host[] = "http://openapi.tencentyun.com";
            ++ i;
            char buffer[MAX_RET_LEN] = {0};
            char url[MAX_RET_LEN] = "/v3/csec/punish_query";
            char res[MAX_RET_LEN] = "";

            // XXX:测试用数据

            SetUrlString (url, it->playerId, it->type, it->openId, it->openKey, it->pf, it->userIp);
            sprintf (res, "%s%s", host, url);

            int timeout = 1;

            curl_easy_setopt(curl, CURLOPT_URL, res);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, recvret);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);


            CURLcode curlRes = curl_easy_perform(curl);
            if (CURLE_OK == curlRes)
            {
                Int32 ret = ResultParse(buffer);
                TRACE_LOG("[%u]%u:%u[%s] -> %d, result = %d", m_Worker, i, size, res, curlRes == CURLE_OK, ret);
                if (ret == 1)
                {
                    // 需要封杀交易功能
                    GObject::Player * pl = GObject::globalPlayers[it->playerId];
                    if (NULL != pl)
                    {
                        GameMsgHdr imh(0x330, pl->getThreadId(), pl, 0);
                        GLOBAL().PushMsg(imh, NULL);
                    }
                }
                else
                {
                    GObject::Player * pl = GObject::globalPlayers[it->playerId];
                    if (NULL != pl)
                    {
                        GameMsgHdr imh(0x331, pl->getThreadId(), pl, sizeof(ret));
                        GLOBAL().PushMsg(imh, &ret);
                    }
                }
            }
            else
            {
                TRACE_LOG("[%u]%u:%u[%s] -> %d", m_Worker, i, size, res, curlRes == CURLE_OK);
                continue;
            }
            delete[] it->openId;
            delete[] it->openKey;
            delete[] it->pf;
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
        const static int OPEN_ID_LEN  = 20;
        const static int OPEN_KEY_LEN = 60;
        const static int PF_LEN = 20;
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
        sigKey << "&type=";
        sigKey << (UInt32) type;
        sigKey << "&userip=";
        sigKey << userIp;
        std::string sigValue;
        sigValue += strGet.str(); 
        sigValue += UrlEncode(sigKey.str().c_str());


        unsigned int size = 0;
        unsigned char* res;
        res = HMAC(EVP_sha1(), appkey.c_str(), appkey.size(), (unsigned char *)sigValue.c_str(), sigValue.size(), NULL, &size);
        sigValue = Base64Encode(res, size);
        sigKey << "&sig=";
        sigKey << sigValue;
        strcat (url, "?");
        strncat(url, sigKey.str().c_str(), sigKey.str().size() + 1);

    }

    void OpenAPIWorker::SetUrlString2(char* url, UInt64 playerId, UInt16 type, const char * openId, const char * openKey, const char * pf, const char * userIp)
    {
        // 测试用的函数，现在不需要使用了
        SHA1Engine sha1;

        std::ostringstream sigKey2;
        sigKey2 << "/v3/user/get_info";
        std::string val1 = "GET&";
        val1 += UrlEncode ( sigKey2.str().c_str());
        val1 += "&";

        std::ostringstream sigKey;
        sigKey << "appid=";
        sigKey << (UInt32)123456;
        sigKey << "&format=json"; // XXX: 返回格式先写死为json
        sigKey << "&openid=";
        sigKey << openId;
        sigKey << "&openkey=";
        sigKey << openKey;
        sigKey << "&pf=";
        sigKey << pf;
        sigKey << "&userip=";
        sigKey << userIp;
        std::string sigValue = UrlEncode(sigKey.str().c_str());

        std::string sigValue2;
        sigValue2 += val1;
        sigValue2 += sigValue;

        unsigned int size = 0;
        unsigned char* res;
        unsigned char tmp[1024];
        res = HMAC(EVP_sha1(), "228bf094169a40a3bd188ba37ebe8723&", strlen("228bf094169a40a3bd188ba37ebe8723&"), (unsigned char *)sigValue2.c_str(), sigValue2.size(), tmp, &size);
        sigValue = Base64Encode(res, size);
        sigKey << "&sig=";
        sigKey << sigValue;
        strncpy(url, sigKey.str().c_str(), sigKey.str().size());
    }

    std::string OpenAPIWorker::UrlEncode(const char *in_str)
    {
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
        //std::cout << ret << std::endl; // XXX: 为什么GDB在这里我输出ret的数据
        return ret;
    }

    Int32 OpenAPIWorker::ResultParse(char* result)
    {
        // 解析json的结果
#define JSON_ERR_CUSTOM -9527
#define JSON_ERR_CUSTOM2 -9528
#define JSON_ERR_CUSTOM3 -9529

        json_t* obj = NULL;

        enum json_error jerr;
        if ((jerr = json_parse_document(&obj, result)) != JSON_OK)
            return JSON_ERR_CUSTOM;

        json_t* hdr = json_find_first_label(obj, "ret");
        if (!hdr)
            return JSON_ERR_CUSTOM2;

        Int32 ret = atoi(hdr->text);
        if (!ret)
            return ret;

        hdr =  json_find_first_label(obj, "punish");
        if (!hdr)
            return JSON_ERR_CUSTOM3;
        ret = atoi(hdr->text);
        return ret;
#undef JSON_ERR_CUSTOM
#undef JSON_ERR_CUSTOM2
#undef JSON_ERR_CUSTOM3
    }

}


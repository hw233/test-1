
#ifndef _OPEN_API_WORKER_H_
#define _OPEN_API_WORKER_H_

#include "Server/WorkerThread.h"
#include "Common/Mutex.h"
#include "Common/AtomicVal.h"
#include <curl/curl.h>

namespace GObject
{


    class OpenAPIWorker
        : public WorkerRunner<>
    {
        public:
            OpenAPIWorker(UInt8, UInt8);
            ~OpenAPIWorker();

        public:
            UInt8 TID() const { return m_Worker; }
            void Push(UInt64 playerId, UInt16 type, const char * openId, const char * openKey, const char * pf, const char * userIp);

        protected:
            bool Init();
            void UnInit();

            void OnTimer();
            void OnPause();
            std::string GetLogName();


        private:

            void SetUrlString(char* url, UInt64 playerId, UInt16 type, const char * openId, const char * openKey, const char * pf, const char * userIp);
            void SetUrlString2(char* url, UInt64 playerId, UInt16 type, const char * openId, const char * openKey, const char * pf, const char * userIp);

            std::string UrlEncode(const char *in_str);
            std::string Base64Encode(unsigned char const * byte_to_encode, unsigned int in_len);
            Int32 ResultParse(char* result);

            struct UrlParam
            {
                UInt64 playerId;
                UInt16  type;
                const char * openId;
                const char * openKey;
                const char * pf;
                const char * userIp;
            };

            UInt32 version;
            UInt32 appid;
            std::string appkey;
            UInt8 m_Type;
            UInt8 m_Worker;
            AtomicVal<UInt32> m_Limit;
            bool m_inited;

            std::vector<UrlParam> m_QueryList;


            CURL* curl;
            FastMutex m_Mutex;
    };

}


#endif // _OPEN_API_WORKER_H_

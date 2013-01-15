
#ifndef _OPEN_API_WORKER_H_
#define _OPEN_API_WORKER_H_

#include "Server/WorkerThread.h"
#include "Common/Mutex.h"
#include "Common/AtomicVal.h"
#include "Common/MCached.h"
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

            std::string UrlEncode(const char *in_str);
            std::string Base64Encode(unsigned char const * byte_to_encode, unsigned int in_len);
            Int32 ResultParse(char* result, bool* needForbid);
            bool CheckOpenId(UInt64 playerId, char * openId);

            struct UrlParam
            {
                UInt64 playerId;
                UInt16  type;
                char * openId;
                char * openKey;
                char * pf;
                char * userIp;
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
            MCached m_MCached;
    };

}


#endif // _OPEN_API_WORKER_H_

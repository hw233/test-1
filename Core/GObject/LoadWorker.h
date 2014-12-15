
#ifndef _LOAD_WORKER_H_
#define _LOAD_WORKER_H_

#include "Server/WorkerThread.h"
#include "Common/Mutex.h"
#include "Common/AtomicVal.h"
#include "Common/MCached.h"
#include <curl/curl.h>
#ifndef IDTYPE
#define IDTYPE UInt32
#endif

namespace GObject
{
    class LoadWorker
        : public WorkerRunner<>
    {
        public:
            LoadWorker(UInt8, UInt8);
            ~LoadWorker();

        public:
            inline UInt8 TID() const { return m_Worker; }
            void Push(UInt64 playerId, UInt16 type);

        protected:
            bool Init();
            void UnInit();
            void OnTimer();
            void OnPause();
            bool GetMailInfo(UInt64 playerId);
            std::string GetLogName();

        private:

            struct UrlParam
            {
                IDTYPE playerId;
                UInt16 type;
            };

            UInt8 m_Type;
            UInt8 m_Worker;

            std::vector<UrlParam> m_QueryList;

            FastMutex m_Mutex;
    };

}


#endif // _LOAD_WORKER_H_

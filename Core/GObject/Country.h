
#ifndef COUNTRY_H_
#define COUNTRY_H_
#include "Server/WorkerRunner.h"
#include "MsgHandler/CountryMsgHandler.h"
namespace GObject
{
    class Country
        :public WorkerRunner<CountryMsgHandler>
    {
        public:
            Country(UInt8 id);
            inline UInt8 TID() const { return m_ThreadID; }
            bool Init();
            void UnInit();
            std::string GetLogName();
        
        private:
            UInt8       m_ThreadID;
    };
}
#endif // COUNTRY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


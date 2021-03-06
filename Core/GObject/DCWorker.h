
#ifndef DCWORKER_H_
#define DCWORKER_H_

#include "Server/WorkerThread.h"
#include "Common/Mutex.h"
#include "Common/AtomicVal.h"
#include "Common/MCached.h"
#include "dcapi_cpp.h"
#include <curl/curl.h>

using namespace DataCollector;

namespace GObject
{

const char UNION_DC_TYPE = 100;

class DCWorker
    : public WorkerRunner<>
{
public:
	DCWorker(UInt8, UInt8);
	~DCWorker();

public:
	UInt8 TID() const { return m_Worker; }
    void Push(const char* msg, size_t len, const char logType = LT_NORMAL);
    void PushCheckOpenId(UInt64 playerId, const char* openId, UInt32 len);
    UInt8 CheckRPOpenid(char* openid);
    UInt8 CheckGRPOpenid(char* openid);
    UInt8 CheckActiveOpenid(char * key1,char * openid);
    UInt8 CheckYBLevel(UInt64 playerId ,char * message);
protected:
	bool Init();
	void UnInit();

	void OnTimer();
	void OnPause();
	std::string GetLogName();

private:
    UInt32 UnionLoggerResultParse(char* result, char* msg, size_t size);
    UInt32 CheckYBResultParse(char* result, char* msg, size_t size);
    //bool CheckOpenId(UInt64 playerId, char * openId);

private:
    struct LogMsg
    {
        const char * logString;
        char logType;
    };
    struct OpenIdMsg
    {
        UInt64 playerId;
        char * openId;
    };
	FastMutex m_Mutex;
    MCached m_MCached;
	UInt8 m_Type;
	UInt8 m_Worker;
    AtomicVal<UInt32> m_Limit;
	std::vector<LogMsg> m_DCLog;
    std::vector<LogMsg> m_UnionLog;
    //std::vector<OpenIdMsg> m_Openid;
    CLogger* m_logger;
    bool m_inited;

    CURL* curl;
};

}

#endif // DCWORKER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


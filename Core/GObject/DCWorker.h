
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

protected:
	bool Init();
	void UnInit();

	void OnTimer();
	void OnPause();
	std::string GetLogName();

private:
    UInt32 UnionLoggerResultParse(char* result, char* msg);
    bool CheckOpenId(UInt64 playerId, char * openId);
    void PushCheckOpenId();

private:
    struct LogMsg
    {
        const char * logString;
        char logType;
    };
	FastMutex m_Mutex;
    MCached m_MCached;
	UInt8 m_Type;
	UInt8 m_Worker;
    AtomicVal<UInt32> m_Limit;
	std::vector<LogMsg> m_DCLog;
    std::vector<LogMsg> m_UnionLog;
    CLogger* m_logger;
    bool m_inited;

    CURL* curl;
};

}

#endif // DCWORKER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */



#ifndef DCWORKER_H_
#define DCWORKER_H_

#include "Server/WorkerThread.h"
#include "Common/Mutex.h"
#include "Common/AtomicVal.h"
#include "dcapi_cpp.h"

using namespace DataCollector;

namespace GObject
{

class DCWorker
    : public WorkerRunner<>
{
public:
	DCWorker(UInt8, UInt8);
	~DCWorker();

public:
	UInt8 TID() const { return m_Worker; }
    void Push(const char* msg, size_t len);

protected:
	bool Init();
	void UnInit();

	void OnTimer();
	void OnPause();
	std::string GetLogName();

private:
	FastMutex m_Mutex;
	UInt8 m_Type;
	UInt8 m_Worker;
    AtomicVal<UInt32> m_Limit;
	std::vector<const char *> m_DCLog;
    CLogger* m_logger;
    bool m_inited;
};

}

#endif // DCWORKER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


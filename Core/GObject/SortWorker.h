
#ifndef SORTWORKER_H_
#define SORTWORKER_H_

#include "Server/WorkerThread.h"
#include "Common/Mutex.h"

namespace GObject
{

class SortWorker 
    : public WorkerRunner<>
{
public:
	SortWorker(UInt8, UInt8);
	~SortWorker();

public:
	UInt8 TID() const { return m_Worker; }

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
    bool m_inited;
};

}

#endif // SORTWORKER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


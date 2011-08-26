#ifndef LOGINWORKER_INC
#define LOGINWORKER_INC

#include "Server/WorkerRunner.h"
#include "DB/DBExecutor.h"
#include "MsgHandler/LoginMsgHandler.h"
#include "Common/AtomicCounter.h"

namespace Login
{

class LoginWorker:
	public WorkerRunner<LoginMsgHandler>
{
public:
	LoginWorker();
	virtual ~LoginWorker();

    UInt32 Count()
    {
        ++m_current;
        return ++m_count;
    }

    int Current() const
    {
        return m_current;
    }

    void Logout()
    {
        if (m_current)
            --m_current;
    }

protected:
	UInt8 TID() const { return WORKER_THREAD_LOGIN; }
	bool Init();
	void UnInit();

	std::string GetLogName();

protected:
    AtomicCounter m_count;
    AtomicCounter m_current;
};

}

#endif

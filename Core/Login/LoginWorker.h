#ifndef LOGINWORKER_INC
#define LOGINWORKER_INC

#include "Server/WorkerRunner.h"
#include "DB/DBExecutor.h"
#include "MsgHandler/LoginMsgHandler.h"

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

    UInt32 Current() const
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
    UInt32 m_count;
    UInt32 m_current;
};

}

#endif

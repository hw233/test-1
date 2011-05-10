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

protected:
	UInt8 TID() const { return WORKER_THREAD_LOGIN; }
	bool Init();
	void UnInit();

	std::string GetLogName();
};

}

#endif

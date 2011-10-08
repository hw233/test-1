#ifndef DBWORKER_INC
#define DBWORKER_INC

#include "Server/WorkerThread.h"
#include "Common/Mutex.h"
#include "Common/AtomicVal.h"

namespace DB
{

class DBExecutor;

class DBWorker;

class DBWorker:
	public WorkerRunner<>
{
public:
	DBWorker(UInt8, UInt8);
	~DBWorker();

public:
	UInt8 TID() const { return m_Worker; }

protected:
	bool Init();
	void UnInit();

	void OnTimer();
	void OnPause();
	std::string GetLogName();

public:
	void PushUpdateData(const char *, ...);
    void PushUpdateDataF(const char * fmt, ...); // force
    void PushUpdateDataL(const char * fmt, ...); // lazy

private:
	bool DoDBQuery(const char* query);
	static void CalcUserLost(DBWorker *);
    void InfoLog(const char* query);

private:
	std::unique_ptr<DBExecutor> m_DBExecutor;      //用于数据库查询数据进行初始化

	FastMutex m_Mutex;
	UInt8 m_Type;
	UInt8 m_Worker;
    AtomicVal<UInt32> m_Limit;
	std::vector<const char *> m_UpdateItems;
};

}

#endif

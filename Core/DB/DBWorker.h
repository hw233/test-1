#ifndef DBWORKER_INC
#define DBWORKER_INC

#include "Server/WorkerThread.h"
#include "Common/Mutex.h"
#include "Common/AtomicVal.h"
#include "Common/MemBlockPool.h"

namespace DB
{

class DBExecutor;

class DBWorker;

class DBWorker:
	public WorkerRunner<>
{
    const static size_t MIN_MEMBLOCK_SIZE = 256;
    const static size_t MEMPOOL_NUM = 5;

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
    void GetMultiDBName(std::string& oriName);
private:
	bool DoDBQuery(const char* query);
	static void CalcUserLost(DBWorker *);
    void InfoLog(const char* query);

    void* AllocMemBlock(size_t size);
    void  FreeMemBlock(void* ptr);

private:
	std::unique_ptr<DBExecutor> m_DBExecutor;      //用于数据库查询数据进行初始化

	FastMutex m_Mutex;
	UInt8 m_Type;
	UInt8 m_Worker;
    AtomicVal<UInt32> m_Limit;
	std::vector<char *> m_UpdateItems;

    MemBlockPool*  m_Pools[MEMPOOL_NUM];
    size_t         m_MaxPoolSize;
};

}

#endif

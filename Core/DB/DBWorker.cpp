#include "Config.h"
#include "DBWorker.h"
#include "DB/DBConnectionMgr.h"
#include "Log/Log.h"
#include "Common/TimeUtil.h"
#include "GObject/Player.h"
#include "Server/Cfg.h"
#include <errmsg.h>
#include <cstdarg>

namespace DB
{

DBWorker::DBWorker(UInt8 type, UInt8 worker) :
    WorkerRunner<>((type == 1 && cfg.serverLogId == 0) ? 0 : 500),
    m_Type(type), m_Worker(worker), m_Limit(0)
{
}

DBWorker::~DBWorker()
{
}

void DBWorker::CalcUserLost(DBWorker * worker)
{
	UInt32 tstart = TimeUtil::SharpDay(-1);
	UInt32 tend = tstart + 86400;
	{
		Mutex::ScopedLock lk(GObject::newPlayers.getMutex());
		std::unordered_map<UInt64, GObject::Player *>& newplayers = GObject::newPlayers.getMap();
		std::unordered_map<UInt64, GObject::Player *>::iterator it = newplayers.begin();
		while(it != newplayers.end())
		{
			GObject::Player * pl = it->second;
			UInt32 created = PLAYER_DATA(pl, created);
			if(created < tend)
			{
				if(created >= tstart)
					worker->PushUpdateData("update register_states set level=%u where server_id=%u and player_id=%"I64_FMT"u", pl->GetLev(), cfg.serverLogId, pl->getId());
				newplayers.erase(it ++);
			}
			else
				++ it;
		}
	}
	worker->PushUpdateData("call update_register(%u,%u)", cfg.serverLogId, tend);
}

bool DBWorker::Init()
{
	switch(m_Type)
	{
	case 0:
		m_DBExecutor.reset(gObjectDBConnectionMgr->GetExecutor());
		break;
	case 1:
		{
			m_DBExecutor.reset(gLogDBConnectionMgr->GetExecutor());
#if 0
			UInt32 now = TimeUtil::Now(), sday = TimeUtil::SharpDay(1) - 10;
			if(sday < now) sday += 86400;
			AddTimer(86400 * 1000, CalcUserLost, this, (sday - now) * 1000);
#endif
		}
		break;
	}

	return true;
}

void DBWorker::UnInit()
{
	OnTimer();
	m_DBExecutor.reset();
}

void DBWorker::OnTimer()
{
	std::vector<const char *> l;
	{
		FastMutex::ScopedLock lk(m_Mutex);
		if(m_UpdateItems.empty())
			return;
		l = m_UpdateItems;
		m_UpdateItems.clear();
        m_Limit = 0;
	}
	while(m_DBExecutor.get() == NULL || !m_DBExecutor->isConnected())
	{
		Thread::sleep(500);
		if(m_Type == 0)
			m_DBExecutor.reset(gObjectDBConnectionMgr->GetExecutor());
		else
			m_DBExecutor.reset(gLogDBConnectionMgr->GetExecutor());
	}

#if 0
	std::vector<const char *>::iterator it;
	for(it = l.begin(); it != l.end(); ++ it)
	{
		bool r = DoDBQuery(*it);
		TRACE_LOG("[%s] -> %d", *it, r ? 1 : 0);
		delete[] *it;
	}
#else
    if (!l.empty())
    {
        size_t size = l.size();
        size_t sz = size;
        const char** query = &l[0];
        while (size)
        {
            --size;
            bool r = DoDBQuery(*query);
            TRACE_LOG("[%u]%u:%u-[%s] -> %d", m_Worker, sz, size, *query, r ? 1 : 0);
            delete[] *query;
            ++query;
            if (!*query)
                continue;
        }
    }
#endif
}

void DBWorker::OnPause()
{
	m_DBExecutor->setMgr(NULL);
	m_DBExecutor.reset();
	if(m_Type == 0)
		DB::gObjectDBConnectionMgr->Init( cfg.dbObjectHost.c_str(), cfg.dbObjectUser.c_str(), cfg.dbObjectPassword.c_str(), cfg.dbObjectSource.c_str(), 4, 32, cfg.dbObjectPort );
	else
		DB::gLogDBConnectionMgr->Init( cfg.dbLogHost.c_str(), cfg.dbLogUser.c_str(), cfg.dbLogPassword.c_str(), cfg.dbLogSource.c_str(), 4, 32, cfg.dbLogPort );
	Resume();
}

void DBWorker::InfoLog(const char* p)
{
    if (!p)
        return;

    if (m_Worker == WORKER_THREAD_DB) // player
    {
        DB().GetLog()->OutInfo("[%u]Push [%s]\n", m_Worker, p);
    }
    else if (m_Worker == WORKER_THREAD_DB1) // player
    {
        DB1().GetLog()->OutInfo("[%u]Push [%s]\n", m_Worker, p);
    }
    else if (m_Worker == WORKER_THREAD_DB2) // fighter
    {
        DB2().GetLog()->OutInfo("[%u]Push [%s]\n", m_Worker, p);
    }
    else if (m_Worker == WORKER_THREAD_DB3) // dungeon_player,player_copy,player_front
    {
        DB3().GetLog()->OutInfo("[%u]Push [%s]\n", m_Worker, p);
    }
    else if (m_Worker == WORKER_THREAD_DB4) // item,equipment
    {
        DB4().GetLog()->OutInfo("[%u]Push [%s]\n", m_Worker, p);
    }
    else if (m_Worker == WORKER_THREAD_DB5) // task_instance
    {
        DB5().GetLog()->OutInfo("[%u]Push [%s]\n", m_Worker, p);
    }
    else if (m_Worker == WORKER_THREAD_DB6) // 
    {
        DB6().GetLog()->OutInfo("[%u]Push [%s]\n", m_Worker, p);
    }
}
void DBWorker::PushUpdateData(const char * fmt, ...)
{
	if(m_Type == 1 && cfg.serverLogId == 0)
		return;

    if (m_Worker == WORKER_THREAD_DB) // normal
    {
        if (m_Limit > 5000)
            return;
    }

    if (m_Worker == WORKER_THREAD_DB1) // player
    {
    }

    if (m_Worker == WORKER_THREAD_DB2) // fighter
    {
    }

    if (m_Worker == WORKER_THREAD_DB3) // dungeon_player,player_copy,player_front
    {
    }

    if (m_Worker == WORKER_THREAD_DB4) // item,equipment
    {
    }

    if (m_Worker == WORKER_THREAD_DB5) // task_instance
    {
    }

    if (m_Worker == WORKER_THREAD_DB6) // 
    {
    }

    ++m_Limit;

	/* Guess we need no more than 256 bytes. */
	int size = 256;

	char *p = new(std::nothrow) char[size];
	if (p == NULL)
		return;

	while (1) {
		va_list ap;
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
		int n = vsnprintf(p, size, fmt, ap);
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			break;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		delete[] p;
		if ((p = new(std::nothrow) char[size]) == NULL) {
			return;
		}
	}

	FastMutex::ScopedLock lk(m_Mutex);
	m_UpdateItems.push_back(p);

    if(m_Type == 0)
        InfoLog(p);
}

void DBWorker::PushUpdateDataL(const char * fmt, ...)
{
    if (m_Limit > 500)
        return;
    ++m_Limit;

	if(m_Type == 1 && cfg.serverLogId == 0)
		return;
	/* Guess we need no more than 256 bytes. */
	int size = 256;

	char *p = new(std::nothrow) char[size];
	if (p == NULL)
		return;

	while (1) {
		va_list ap;
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
		int n = vsnprintf(p, size, fmt, ap);
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			break;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		delete[] p;
		if ((p = new(std::nothrow) char[size]) == NULL) {
			return;
		}
	}

	FastMutex::ScopedLock lk(m_Mutex);
	m_UpdateItems.push_back(p);

	if(m_Type == 0)
        InfoLog(p);
}

void DBWorker::PushUpdateDataF(const char * fmt, ...)
{
	if(m_Type == 1 && cfg.serverLogId == 0)
		return;
	/* Guess we need no more than 256 bytes. */
	int size = 256;

	char *p = new(std::nothrow) char[size];
	if (p == NULL)
		return;

	while (1) {
		va_list ap;
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
		int n = vsnprintf(p, size, fmt, ap);
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			break;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		delete[] p;
		if ((p = new(std::nothrow) char[size]) == NULL) {
			return;
		}
	}

    ++m_Limit;
	FastMutex::ScopedLock lk(m_Mutex);
	m_UpdateItems.push_back(p);

	if(m_Type == 0)
        InfoLog(p);
}

bool DBWorker::DoDBQuery(const char* query)
{
	if (query == NULL)
		return false;
	DBResult r;
	while((r = m_DBExecutor->Execute(query)) == DB_ConnLost)
	{
		TRACE_LOG("Lost connection to Database, retrying...");
		Thread::sleep(500);
	}
	return r == DB_OK;
}

std::string DBWorker::GetLogName()
{
	switch(m_Type)
	{
	case 0:
		return "log/DB/";
		break;
	case 1:
		return "log/DBLog/";
		break;
	}
	return "";
}
void  DBWorker::GetMultiDBName(std::string& oriName)
{
    char app[64] = {0};
    UInt32 m = 0;
    UInt32 y = 0;
    TimeUtil::GetDMY(NULL, &m, &y);
    snprintf(app, 64, "_%u_%u", y, m);
    oriName  += app;
}
}

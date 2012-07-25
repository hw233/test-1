#include "Config.h"
#include "DBExecutor.h"
#include "DBConnectionMgr.h"

namespace DB
{

DBConnectionMgr* gDataDBConnectionMgr;
DBConnectionMgr* gObjectDBConnectionMgr;
DBConnectionMgr* gLogDBConnectionMgr;

DBExecutor * DBConnectionMgr::GetExecutor()
{
	FastMutex::ScopedLock lk(_mutex);
	if(_connPool.empty())
	{
		DBConn * conn = new DBConn(_host, _user, _passwd, _db, _port);
		if(!conn->connect())
		{
			delete conn;
			return NULL;
		}
		return new DBExecutor(conn, this);
	}
	std::set<DBConn *>::iterator iter = _connPool.begin();
	DBExecutor * res = new DBExecutor(*iter, this);
	_connPool.erase(iter);
	return res;
}

void DBConnectionMgr::closeAll()
{
	std::set<DBConn *>::iterator it;
	for(it = _connPool.begin(); it != _connPool.end(); ++ it)
	{
		delete *it;
	}
	_connPool.clear();
}

void DBConnectionMgr::AddBack( DBConn * conn )
{
	FastMutex::ScopedLock lk(_mutex);
	if(_connPool.size() >= _maxConn || !conn->isConnected())
	{
		delete conn;
		return;
	}
	_connPool.insert(conn);
}

bool DBConnectionMgr::Init(const std::string& host, const std::string& user, const std::string& pwd, const std::string& dbname, UInt16 minConn, UInt16 maxConn, UInt16 port, UInt16 idleTime)
{
    FastMutex::ScopedLock lk(_mutex);
	closeAll();
	_host = host;
	_port = port;
	_user = user;
	_passwd = pwd;
	_db = dbname;
	_minConn = minConn;
	_maxConn = maxConn;
	_idleTime = idleTime;
	fillConnections();
	return true;
}

void DBConnectionMgr::UnInit()
{
    FastMutex::ScopedLock lk(_mutex);
	closeAll();
}

void DBConnectionMgr::fillConnections()
{
	UInt16 sz = static_cast<UInt16>(_connPool.size());
	for(; sz < _minConn; ++ sz)
	{
		DBConn * conn = new DBConn(_host, _user, _passwd, _db, _port);
		if(!conn->connect())
		{
			delete conn;
			return;
		}
		_connPool.insert(conn);
	}
}

}

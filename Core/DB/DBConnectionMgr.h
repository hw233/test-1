#ifndef DBCONNECTIONMGR_INC
#define DBCONNECTIONMGR_INC

#include "Common/Mutex.h"

namespace DB
{
	class DBConnectionMgr;
	class DBExecutor;
	class DBConn;
	extern DBConnectionMgr* gDataDBConnectionMgr;
	extern DBConnectionMgr* gObjectDBConnectionMgr;
	extern DBConnectionMgr* gLogDBConnectionMgr;
	class DBConnectionMgr
	{
	public:
		DBConnectionMgr() {}
		virtual ~DBConnectionMgr() {}

	public:
		DBExecutor * GetExecutor();

		inline bool ConnFull() const
		{
			//TODO--
			return false;
		}

	public:
		bool Init(const std::string& host, const std::string& user, const std::string& pwd, const std::string& dbname, UInt16 minConn = 1, UInt16 maxConn = 32, UInt16 port = 3306, UInt16 idleTime = 60);
		void UnInit();

		void AddBack(DBConn *);

	private:
		void fillConnections();
		void closeAll();

	private:
		std::set<DBConn *> _connPool;

		std::string _host;
		UInt16		_port;
		std::string _user;
		std::string _passwd;
		std::string _db;
		UInt16		_minConn;
		UInt16		_maxConn;
		UInt16		_idleTime;

		FastMutex	_mutex;
	};
}

#endif

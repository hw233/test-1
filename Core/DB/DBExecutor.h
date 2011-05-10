#ifndef DBEXECUTOR_INC
#define DBEXECUTOR_INC

#include "DBConn.h"

namespace DB
{

class DBConnectionMgr;

template <class T>
class TypeHandler
{
public:
	static void bind(std::size_t pos, T& obj, DBConn* conn)
	{
		assert(conn != 0);
		conn->bind(pos, obj);
	}

private:
	TypeHandler();
	~TypeHandler();
	TypeHandler(const TypeHandler&);
	TypeHandler& operator = (const TypeHandler&);
};

//SQL statement executor 
class DBExecutor
{
public:
	DBExecutor(DBConn * conn, DBConnectionMgr * mgr);
	~DBExecutor();

public:
	inline DBResult Begin() { return _conn->begin(); }
	inline DBResult Commit() { return _conn->commit(); }
	inline DBResult Rollback() { return _conn->rollback(); }
	DBResult Execute(const char * sql);
	DBResult Execute2(const char * sql, ...);  
	inline bool isConnected() { return _conn->isConnected(); }

public:
	inline DBExecutor& operator << (const char * q)
	{
		_conn->execute(q);
		return *this;
	}

public:
	template <typename T>
	inline DBResult Extract(const char * sql, T& t)
	{
		DBResult r = Prepare(sql, t);
		if(r == DB_OK)
		{
			r = Next();
			Finish();
		}
		return r;
	}
	template <typename T>
	inline bool ExtractData(const char * sql, std::vector<T>& tlist)
	{
		T t;
		DBResult r = Prepare(sql, t);
		if(r == DB_OK)
		{
			while((r = Next()) == DB_OK)
			{
				tlist.push_back(t);
			}
			if(r == DB_NoMore)
				return true;
		}
		Finish();
		return r == DB_OK;
	}
	template <typename T>
	DBResult Prepare(const char * sql, T& t);
	inline DBResult Next() { return _conn->next(); }
	inline void Finish() { _conn->closestmt(); }
	inline void setMgr(DBConnectionMgr * mgr) { _mgr = mgr; }

private:
	DBConn * _conn;
	DBConnectionMgr * _mgr;
};

template <typename T>
inline DBResult DBExecutor::Prepare(const char * sql, T& t)
{
	DBResult res = _conn->prepare(sql);
	if(res != DB_OK)
		return res;
	TypeHandler<T>::bind(0, t, _conn);
	return DB_OK;
}

}

#endif

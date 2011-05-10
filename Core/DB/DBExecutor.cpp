#include "Config.h"
#include "DBExecutor.h"
#include "DBConnectionMgr.h"
#include <cstdarg>

namespace DB
{

DBExecutor::DBExecutor( DBConn * conn, DBConnectionMgr * mgr ) : _conn(conn), _mgr(mgr)
{

}

DBExecutor::~DBExecutor()
{
	if(_mgr != NULL)
		_mgr->AddBack(_conn);
}

DBResult DBExecutor::Execute(const char * sql)
{
	return _conn->execute(sql);
}

DBResult DBExecutor::Execute2(const char * sql, ...)
{
	/* Guess we need no more than 256 bytes. */
	int size = 256;

	char *p = new(std::nothrow) char[size];
	if (p == NULL)
		return DB_Fail;

	while (1) {
		va_list ap;
		/* Try to print in the allocated space. */
		va_start(ap, sql);
		int n = vsnprintf(p, size, sql, ap);
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
			return DB_Fail;
		}
	}
	DBResult ret = _conn->execute(p);
	delete[] p;
	return ret;
}

}

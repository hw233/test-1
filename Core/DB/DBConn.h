#ifndef _DBCONN_H_
#define _DBCONN_H_

#include "DBConnectionMgr.h"
#include <mysql.h>
#include <errmsg.h>
#undef min
#undef max
#undef sleep

namespace DB
{

class ResultMetadata
{
public:
	void reset();

	void init(MYSQL_STMT* stmt);

	inline UInt32 columnsReturned() const { return static_cast<UInt32>(_columnLength.size()); }

	inline MYSQL_BIND* row() { return &_row[0]; }

	inline std::size_t length(std::size_t pos) const { return _lengths[pos]; }

	inline const char* rawData(std::size_t pos) const { return reinterpret_cast<const char*>(_row[pos].buffer); }

	inline bool isNull(std::size_t pos) const { return (_isNull[pos] != 0); }

private:
	std::vector<size_t>        _columnLength;
	std::vector<MYSQL_BIND>    _row;
	std::vector<char>          _buffer;
	std::vector<unsigned long> _lengths;
    std::vector<my_bool>       _isNull;
};

class DBConnectionMgr;

UInt8 __fieldType(enum_field_types ft);

class Binder
{
public:
	virtual void extract(MYSQL_BIND& b, void * data) = 0;
};

struct BinderStruct
{
	BinderStruct(MYSQL_BIND& b, Binder * bnd, void * d, enum_field_types t, size_t l): bind(b), binder(bnd), data(d), type(t), length(l) {}
	~BinderStruct() { delete binder; }
	void extract() { binder->extract(bind, data); }
	MYSQL_BIND& bind;
	Binder * binder;
	void * data;
	enum_field_types type;
	size_t length;
};

template<typename T>
class BinderMem: public Binder
{
public:
	template<typename RT>
	inline void extract2(void * src, void * data)
	{
		*reinterpret_cast<T*>(data) = static_cast<T>(*reinterpret_cast<RT*>(src));
	}
	template<typename RT>
	inline void clear(void * data)
	{
		*reinterpret_cast<T*>(data) = 0;
	}
	inline void extract(MYSQL_BIND& b, void * data)
	{
		switch(b.buffer_type)
		{
		case MYSQL_TYPE_TINY:
			if(*b.is_null)
				clear<Int8>(data);
			else
				extract2<Int8>(b.buffer, data);
			break;
		case MYSQL_TYPE_SHORT:
			if(*b.is_null)
				clear<Int16>(data);
			else
				extract2<Int16>(b.buffer, data);
			break;
		case MYSQL_TYPE_INT24:
		case MYSQL_TYPE_LONG:
			if(*b.is_null)
				clear<Int32>(data);
			else
				extract2<Int32>(b.buffer, data);
			break;
		case MYSQL_TYPE_LONGLONG:
			if(*b.is_null)
				clear<Int64>(data);
			else
				extract2<Int64>(b.buffer, data);
			break;
		case MYSQL_TYPE_FLOAT:
			if(*b.is_null)
				clear<float>(data);
			else
				extract2<float>(b.buffer, data);
			break;
		case MYSQL_TYPE_DOUBLE:
			if(*b.is_null)
				clear<double>(data);
			else
				extract2<double>(b.buffer, data);
			break;
		default:
			break;
		}
	}
};

class BinderString: public Binder
{
public:
	void extract(MYSQL_BIND& b, void * data)
	{
		std::string * str = reinterpret_cast<std::string *>(data);
		if(*b.is_null)
		{
			str->clear();
		}
		else
		{
			str->assign(reinterpret_cast<const char *>(b.buffer), *b.length);
		}
	}
};

class BinderVector: public Binder
{
public:
	void extract(MYSQL_BIND& b, void * data)
	{
		std::vector<UInt8> * vec = reinterpret_cast<std::vector<UInt8> *>(data);
		if(*b.is_null)
		{
			vec->clear();
		}
		else
		{
			vec->assign(reinterpret_cast<UInt8 *>(b.buffer), reinterpret_cast<UInt8 *>(b.buffer) + *b.length);
		}
	}
};

template <typename T> struct BindTypeTraits { typedef Binder BindType; static const enum_field_types FieldType = MYSQL_TYPE_BLOB; };
template <> struct BindTypeTraits<bool> { typedef BinderMem<Int8> BindType; static const enum_field_types FieldType = MYSQL_TYPE_TINY; };
template <> struct BindTypeTraits<wchar_t> { typedef BinderMem<Int16> BindType; static const enum_field_types FieldType = MYSQL_TYPE_SHORT; };
template <> struct BindTypeTraits<Int8> { typedef BinderMem<Int8> BindType; static const enum_field_types FieldType = MYSQL_TYPE_TINY; };
template <> struct BindTypeTraits<Int16> { typedef BinderMem<Int16> BindType; static const enum_field_types FieldType = MYSQL_TYPE_SHORT; };
template <> struct BindTypeTraits<Int32> { typedef BinderMem<Int32> BindType; static const enum_field_types FieldType = MYSQL_TYPE_LONG; };
template <> struct BindTypeTraits<Int64> { typedef BinderMem<Int64> BindType; static const enum_field_types FieldType = MYSQL_TYPE_LONGLONG; };
template <> struct BindTypeTraits<UInt8> { typedef BinderMem<UInt8> BindType; static const enum_field_types FieldType = MYSQL_TYPE_TINY; };
template <> struct BindTypeTraits<UInt16> { typedef BinderMem<UInt16> BindType; static const enum_field_types FieldType = MYSQL_TYPE_SHORT; };
template <> struct BindTypeTraits<UInt32> { typedef BinderMem<UInt32> BindType; static const enum_field_types FieldType = MYSQL_TYPE_LONG; };
template <> struct BindTypeTraits<UInt64> { typedef BinderMem<UInt64> BindType; static const enum_field_types FieldType = MYSQL_TYPE_LONGLONG; };
template <> struct BindTypeTraits<float> { typedef BinderMem<float> BindType; static const enum_field_types FieldType = MYSQL_TYPE_FLOAT; };
template <> struct BindTypeTraits<double> { typedef BinderMem<double> BindType; static const enum_field_types FieldType = MYSQL_TYPE_DOUBLE; };
template <> struct BindTypeTraits<std::string> { typedef BinderString BindType; static const enum_field_types FieldType = MYSQL_TYPE_STRING; };

#define ERROR_TEST(n) \
switch(n) \
{ \
case CR_SOCKET_CREATE_ERROR: \
case CR_CONNECTION_ERROR: \
case CR_CONN_HOST_ERROR: \
case CR_IPSOCK_ERROR: \
case CR_SERVER_GONE_ERROR: \
case CR_SERVER_LOST: \
case CR_TCP_CONNECTION: \
	_inTransaction = 0; \
	return DB_ConnLost; \
default: \
	return DB_Fail; \
}

enum DBResult
{
	DB_OK = 0,
	DB_ConnLost = 1,
	DB_Fail = 2,
	DB_NoMore = 255
};

class DBConn
{
public:
	DBConn(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, UInt16 port):
	  _sql(NULL), _stmt(NULL), _inTransaction(0), _host(host), _user(user), _passwd(passwd), _db(db), _port(port) {}
	~DBConn() { close(); }
	inline bool isConnected() { return _sql != NULL; }
	bool connect();
	void close();
	void closestmt();
	DBResult execute(const char * querystr);

	inline DBResult begin()
	{
		DBResult res = execute("START TRANSACTION");
		if(res == DB_OK)
			++ _inTransaction;
		return res;
	}

	inline DBResult commit()
	{
		if(_inTransaction)
		{
			DBResult res = execute("COMMIT");
			if(res == DB_OK)
				-- _inTransaction;
			return res;
		}
		return DB_OK;
	}

	inline DBResult rollback()
	{
		if(_inTransaction)
		{
			DBResult res = execute("ROLLBACK");
			if(res == DB_OK)
				-- _inTransaction;
			return res;
		}
		return DB_OK;
	}

	inline bool inTrasaction()
	{
		return _inTransaction > 0;
	}

	DBResult prepare(const char * querystr);

	DBResult next();

	template <typename T>
	inline void bind(std::size_t pos, T& obj)
	{
		if(pos >= _metadata.columnsReturned())
			return;
		_binder[pos] = new BinderStruct(_metadata.row()[pos], new typename BindTypeTraits<T>::BindType(), &obj, BindTypeTraits<T>::FieldType, sizeof(T));
		_hardExtract[pos] = __fieldType(_metadata.row()[pos].buffer_type) != __fieldType(BindTypeTraits<T>::FieldType);
	}

private:
	MYSQL * _sql;
	MYSQL_STMT * _stmt;
	std::vector<BinderStruct *> _binder;
	std::vector<bool> _hardExtract;
	int _inTransaction;
	std::string _host;
	std::string _user;
	std::string _passwd;
	std::string _db;
	UInt16 _port;
	ResultMetadata _metadata;
};

}

#endif // _DBCONN_H_

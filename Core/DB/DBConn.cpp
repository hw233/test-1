#include "Config.h"
#include "DBConn.h"

#ifdef _MSC_VER
#pragma comment(lib, "libmysql.lib")
#endif

namespace DB
{

class ResultMetadataHandle
{
public:

	explicit ResultMetadataHandle(MYSQL_STMT* stmt)
	{
		h = mysql_stmt_result_metadata(stmt);
	}

	~ResultMetadataHandle()
	{
		if (h)
		{
			mysql_free_result(h);
		}
	}

	operator MYSQL_RES* ()
	{
		return h;
	}

private:

	MYSQL_RES* h;
};

inline size_t fieldSize(const MYSQL_FIELD& field)
{
	switch (field.type)
	{
	case MYSQL_TYPE_TINY:     return sizeof(char);
	case MYSQL_TYPE_SHORT:    return sizeof(short);
	case MYSQL_TYPE_INT24:
	case MYSQL_TYPE_LONG:     return sizeof(Int32);
	case MYSQL_TYPE_FLOAT:    return sizeof(float);
	case MYSQL_TYPE_DOUBLE:   return sizeof(double);
	case MYSQL_TYPE_LONGLONG: return sizeof(Int64);

	case MYSQL_TYPE_DECIMAL:
	case MYSQL_TYPE_NEWDECIMAL:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_TINY_BLOB:
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_BLOB:
	case MYSQL_TYPE_NULL:
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_NEWDATE:
	case MYSQL_TYPE_VARCHAR:
	case MYSQL_TYPE_BIT:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_GEOMETRY:
	default:
		return field.length;
	}
	return field.length;
}

UInt8 __fieldType(enum_field_types ft)
{
	switch (ft)
	{
	case MYSQL_TYPE_TINY:
	case MYSQL_TYPE_SHORT:
	case MYSQL_TYPE_INT24:
	case MYSQL_TYPE_LONG:
	case MYSQL_TYPE_LONGLONG:
	case MYSQL_TYPE_FLOAT:
	case MYSQL_TYPE_DOUBLE:
		return 0;
	case MYSQL_TYPE_DECIMAL:
	case MYSQL_TYPE_NEWDECIMAL:
	case MYSQL_TYPE_STRING:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_TINY_BLOB:
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_BLOB:
	case MYSQL_TYPE_NULL:
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_DATETIME:
	case MYSQL_TYPE_YEAR:
	case MYSQL_TYPE_NEWDATE:
	case MYSQL_TYPE_VARCHAR:
	case MYSQL_TYPE_BIT:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_SET:
	case MYSQL_TYPE_GEOMETRY:
	default:
		return 1;
	}
	return 1;
}

void ResultMetadata::reset()
{
	_columnLength.resize(0);
	_row.resize(0);
	_buffer.resize(0);
	_lengths.resize(0);
	_isNull.resize(0);
}


void ResultMetadata::init(MYSQL_STMT* stmt)
{
	ResultMetadataHandle h(stmt);

	if (!h)
	{
		reset();
		return;
	}

	size_t count = mysql_num_fields(h);
	MYSQL_FIELD* fields = mysql_fetch_fields(h);

	size_t commonSize = 0;
	_columnLength.resize(count);

	for (size_t i = 0; i < count; i++)
	{
		_columnLength[i] = fieldSize(fields[i]);
		commonSize += _columnLength[i];
	}

	_buffer.resize(commonSize);
	_row.resize(count);
	_lengths.resize(count);
	_isNull.resize(count);

	size_t offset = 0;

	for (size_t i = 0; i < count; i++)
	{
		std::memset(&_row[i], 0, sizeof(MYSQL_BIND));

		_row[i].buffer_type   = fields[i].type;
		_row[i].buffer_length = static_cast<unsigned int>(fieldSize(fields[i]));
		_row[i].buffer        = &_buffer[0] + offset;
		_row[i].length        = &_lengths[i];
		_row[i].is_null       = &_isNull[i];
		_row[i].is_unsigned   = (my_bool) ((fields[i].flags & UNSIGNED_FLAG) == UNSIGNED_FLAG);		
		offset += _row[i].buffer_length;
	}
}


DBResult DBConn::prepare( const char * querystr )
{
	closestmt();

	_metadata.reset();
	_stmt = mysql_stmt_init(_sql);
	if(_stmt == NULL)
	{
		return DB_Fail;
	}
	if(mysql_stmt_prepare(_stmt, querystr, strlen(querystr)))
	{
		unsigned int errcode = mysql_stmt_errno(_stmt);
		closestmt();
		ERROR_TEST(errcode);
	}

	_metadata.init(_stmt);
	if(_metadata.columnsReturned() > 0)
	{
		mysql_stmt_bind_result(_stmt, _metadata.row());
		_binder.resize(_metadata.columnsReturned());
		_hardExtract.resize(_metadata.columnsReturned());
	}

	if(mysql_stmt_execute(_stmt))
	{
		unsigned int errcode = mysql_stmt_errno(_stmt);
		closestmt();
		ERROR_TEST(errcode);
	}

	return DB_OK;
}

DBResult DBConn::next()
{
	int r = mysql_stmt_fetch(_stmt);
	switch(r)
	{
	case 0:
		{
			size_t sz = _binder.size();
			for(size_t i = 0; i < sz; ++ i)
			{
				if(_hardExtract[i])
				{
					BinderStruct * binder = _binder[i];
					MYSQL_BIND bind = {0};
					my_bool isNull = 0;
					bind.is_null       = &isNull;
					bind.buffer_type   = binder->type;
					bind.buffer        = binder->data;
					bind.buffer_length = binder->length;
					mysql_stmt_fetch_column(_stmt, &bind, i, 0);
				}
				else
					_binder[i]->extract();
			}
		}
		return DB_OK;
	case MYSQL_NO_DATA:
		closestmt();
		return DB_NoMore;
	default:
		{
			unsigned int errcode = mysql_stmt_errno(_stmt);
			closestmt();
			ERROR_TEST(errcode);
		}
		break;
	}
	return DB_OK;
}

bool DBConn::connect()
{
	close();
	_sql = mysql_init(NULL);
	if(_sql == NULL)
    {
        fprintf(stderr, "Initialize db connection Error !\n");
		return false;
    }
	mysql_options(_sql, MYSQL_SET_CHARSET_NAME, "utf8");
	my_bool dt = false;
	mysql_options(_sql, MYSQL_REPORT_DATA_TRUNCATION, (char *)&dt);
	dt = true;
	mysql_options(_sql, MYSQL_OPT_RECONNECT, (char *)&dt);
	if(mysql_real_connect(_sql, _host.c_str(), _user.c_str(), _passwd.c_str(),
                _db.c_str(), _port, NULL, CLIENT_REMEMBER_OPTIONS | CLIENT_MULTI_STATEMENTS) == NULL)
	{
        fprintf(stderr, "Connecting to %s@%s:%d/%s Error !\n",
                _user.c_str(), _host.c_str(), _port, _db.c_str());
		close();
		return false;
	}
#define SET_NAME_S "SET NAMES utf8"
	mysql_real_query(_sql, SET_NAME_S, strlen(SET_NAME_S));
	return true;
}

void DBConn::close()
{
	closestmt();
	_inTransaction = 0;
	if(_sql != NULL)
	{
		mysql_close(_sql);
		_sql = NULL;
	}
}


void DBConn::closestmt()
{
	_metadata.reset();

	std::vector<BinderStruct *>::iterator it = _binder.begin(), itEnd = _binder.end();
	for(; it != _binder.end(); ++ it)
	{
		delete *it;
	}
	_binder.clear();

	if(_stmt != NULL)
	{
		mysql_stmt_close(_stmt);
		_stmt = NULL;
	}
}

DBResult DBConn::execute(const char * querystr)
{
	if(mysql_real_query(_sql, querystr, strlen(querystr)))
	{
		printf("%s\n", mysql_error(_sql));
		ERROR_TEST(mysql_errno(_sql));
	}
	do
	{
		MYSQL_RES * res = mysql_store_result(_sql);
		if(res != NULL)
		{
			mysql_free_result(res);
		}
	}
	while(!mysql_next_result(_sql));
	return DB_OK;
}

}

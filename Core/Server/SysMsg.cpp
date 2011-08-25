#include "Config.h"
#include "SysMsg.h"
#include "GObject/Player.h"
#include "WorldServer.h"
#include "MsgID.h"
#include "Cfg.h"
#include <cstdarg>

#include "Script/Script.h"

class SysMsgScript:
	public Script::Script
{
public:
	SysMsgScript(SysMsg * sysMsg)
	{
		class_add<SysMsg>("SysMsg");
		class_def<SysMsg>("set", &SysMsg::add);
		set("sysMsg", sysMsg);
	}
};

SysMsg globalSysMsg;

SysMsgItem::SysMsgItem( UInt8 type, const std::string& msg ): _type(type), _msgBody(msg), _stream(REP::SYSTEM_INFO)
{
	_stream << static_cast<UInt8>(type) << msg << Stream::eos;
}

void SysMsgItem::send( GObject::Player * player )
{
	if(player != NULL)
	{
#if 0
		if(player->GetLev() < 6)
			return;
#endif
		player->send(_stream);
	}
	else
		NETWORK()->Broadcast(_stream);
}

void SysMsgItem::sendva( GObject::Player * player, ... )
{
#if 0
	if(player != NULL && player->GetLev() < 6)
		return;
#else
    if (!player)
        return;
#endif
	/* Guess we need no more than 256 bytes. */
	int size = 256;

	char *p = new(std::nothrow) char[size];
	if (p == NULL)
		return;

	while (1) {
		va_list ap;
		/* Try to print in the allocated space. */
		va_start(ap, player);
		int n = vsnprintf(p, size, _msgBody.c_str(), ap);
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

	Stream st(REP::SYSTEM_INFO);
	st << _type << p << Stream::eos;
	delete[] p;
	if(player != NULL)
		player->send(st);
	else
		NETWORK()->Broadcast(st);
}

void SysMsgItem::get( char * str )
{
	strncpy(str, _msgBody.c_str(), 1023);
	str[1023] = 0;
}

void SysMsgItem::getva( char * str, ... )
{
	va_list ap;
	va_start(ap, str);
	vsnprintf(str, 1023, _msgBody.c_str(), ap);
	va_end(ap);
	str[1023] = 0;
}

void SysMsgItem::getvap( Stream * st, ... )
{
	int size = 256;

	char *p = new(std::nothrow) char[size];
	if (p == NULL)
		return;

	while (1) {
		va_list ap;
		/* Try to print in the allocated space. */
		va_start(ap, st);
		int n = vsnprintf(p, size, _msgBody.c_str(), ap);
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

	st->init(REP::SYSTEM_INFO);
	(*st) << _type << p << Stream::eos;
	delete[] p;
}

void SysMsg::add( UInt32 idx, UInt8 type, const char * msg )
{
	SysMsgItem * mi = new SysMsgItem(type, msg);
	if(idx >= _msg.size())
		_msg.resize((idx + 31) & ~15);
	_msg[idx] = mi;
}

SysMsg::~SysMsg()
{
	for(std::vector<SysMsgItem *>::iterator it = _msg.begin(); it != _msg.end(); ++ it)
	{
		delete *it;
	}
	_msg.clear();
}

void SysMsg::load()
{
	SysMsgScript script(this);
	std::string path = cfg.scriptPath + "SysMsg.lua";
	script.doFile(path.c_str());
}

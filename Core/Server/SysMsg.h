#ifndef _SYSMSG_H_
#define _SYSMSG_H_

#include "Common/Stream.h"

namespace GObject
{
	class Player;
}

class SysMsgItem
{
public:
	SysMsgItem(UInt8, const std::string&);
	void send(GObject::Player *);
	void sendva(GObject::Player *, ...);

	void get(char *);
	void getva(char *, ...);
    void getva(std::string& str, ...);
	void getvap(Stream *, ...);

private:
	UInt8 _type;
	std::string _msgBody;
	Stream _stream;
};

class SysMsg
{
public:
	~SysMsg();
	void load();
	void add(UInt32, UInt8, const char *);
	inline SysMsgItem * operator[] (UInt32 idx) { if(idx < _msg.size()) return _msg[idx]; return NULL;}
private:
	std::vector<SysMsgItem *> _msg;
};

extern SysMsg globalSysMsg;

#define SYSMSG(msg, idx) char msg[1024]; { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->get(msg); else msg[0] = 0; }
#define SYSMSGV(msg, idx, ...) char msg[1024]; { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->getva(msg, ##__VA_ARGS__); else msg[0] = 0; }
#define SYSMSGVP(st, idx, ...) { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->getvap(&st, ##__VA_ARGS__); }
#define SYSMSG_SEND(idx, player) { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->send(player); }
#define SYSMSG_BROADCAST(idx) { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->send(NULL); }
#define SYSMSG_SENDV(idx, player, ...) { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->sendva(player, ##__VA_ARGS__); }
#define SYSMSG_BROADCASTV(idx, ...) { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->sendva(NULL, ##__VA_ARGS__); }
#define SYSMSG2(msg, idx) { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->get(msg); else msg[0] = 0; }
#define SYSMSGV2(msg, idx, ...) { SysMsgItem * mi = globalSysMsg[idx]; if(mi != NULL) mi->getva(msg, ##__VA_ARGS__); else msg[0] = 0; }

#endif // _SYSMSG_H_

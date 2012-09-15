#ifndef _MSGTYPES_H_
#define _MSGTYPES_H_

#include "Common/Queue.h"

namespace GObject
{
	class Player;
}

struct MsgHdr
{
	MsgHdr(): desWorkerID(0), cmdID(0), bodyLen(0) {}
	MsgHdr(UInt32 id, UInt8 did, UInt32 blen): desWorkerID(did), cmdID(id), bodyLen(blen) {}
	UInt8	desWorkerID;
	UInt32	cmdID;
	UInt32	bodyLen;
};

struct GameMsgHdr
{
	GameMsgHdr():
		player(NULL) { }
	GameMsgHdr(UInt32 id, UInt8 did, GObject::Player * pl, UInt32 blen):
		msgHdr(id, did, blen), player(pl) { }
#ifdef _ARENA_SERVER
    GameMsgHdr(UInt32 id, UInt8 did, int cid, int sid, UInt32 blen):
        msgHdr(id, did, blen), channelId(cid), serverId(sid) {}
#endif
	MsgHdr	msgHdr;
    GObject::Player *player;
#ifdef _ARENA_SERVER
    int channelId;
    int serverId;
#endif
};

struct LoginMsgHdr
{
	LoginMsgHdr():
		playerID(0), sessionID(0) { }
#ifdef _ARENA_SERVER
    LoginMsgHdr(UInt32 id, UInt8 did, int sess, UInt32 blen):
        msgHdr(id, did, blen), sessionID(sess) { }
#endif
	LoginMsgHdr(UInt32 id, UInt8 did, UInt64 pid, int sess, UInt32 blen):
		msgHdr(id, did, blen), playerID(pid), sessionID(sess) { }
	MsgHdr	msgHdr;
	UInt64	playerID;
	int	sessionID;
};

struct ArenaMsgHdr
{
	ArenaMsgHdr():
		sessionID(0) { }
	ArenaMsgHdr(UInt32 id, UInt8 did, int sess, UInt32 blen):
		msgHdr(id, did, blen), sessionID(sess) { }
	MsgHdr	msgHdr;
	int	sessionID;
};

typedef Queue<MsgHdr*>	MsgQueue;

#endif // _MSGTYPES_H_

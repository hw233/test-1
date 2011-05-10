#ifndef _MSGFUNC_H_
#define _MSGFUNC_H_

#define MSG_QUERY_PLAYER(p) \
	GObject::Player * p = hdr.player; \
	if(p == NULL) \
		return;

#define MSG_QUERY_CONN_PLAYER(c, p) \
	MSG_QUERY_PLAYER(p) \
	TcpConnection conn = NETWORK()->GetConn(p->GetSessionID()); \
	if(conn.get() == NULL) \
		return;

#endif // _MSGFUNC_H_

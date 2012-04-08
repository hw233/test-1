#ifndef _ARENACONN_H_
#define _ARENACONN_H_

#include "TcpConduit.h"

namespace Network
{

class ArenaConn:
	public TcpConduit
{
public:
	ArenaConn(int fd, Network::TcpSlaveServer * s, int id);
	static bool enabled();

protected:
	virtual int	parsePacket(struct evbuffer * buf, int &off, int &len);
	virtual void onRecv(int cmd, int len, void * buf);

	virtual void onDisconnected();

public:
	virtual bool active() { return false; }
};

}

#endif // _ARENACONN_H_

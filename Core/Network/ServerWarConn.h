#ifndef _SERVERWARCONN_H_
#define _SERVERWARCONN_H_

#include "TcpConduit.h"

namespace Network
{

class ServerWarConn:
	public TcpConduit
{
public:
	ServerWarConn(int fd, Network::TcpSlaveServer * s, int id);
	static bool enabled();
    virtual void initConnection();

protected:
	virtual int	parsePacket(struct evbuffer * buf, int &off, int &len);
	virtual void onRecv(int cmd, int len, void * buf);

	virtual void onDisconnected();

public:
	virtual bool active() { return false; }
};

}

#endif // _SERVERWARCONN_H_

#include "Config.h"
#include "ArenaConn.h"

#include "TcpServer.h"

#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include <event2/buffer.h>
#include <event2/bufferevent.h>

namespace Network
{

ArenaConn::ArenaConn( int fd, Network::TcpSlaveServer * s, int id ) :
	TcpConduit(fd, s, id)
{
}

bool ArenaConn::enabled()
{
	return cfg.arenaPort > 0;
}

void ArenaConn::initConnection()
{
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ResolveAddress(cfg.arenaHost.c_str()));
	addr.sin_port = htons(cfg.arenaPort);
	if(bufferevent_socket_connect(_bev, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::bad_exception();
	Stream st(0x01, 0xEF);
	st << cfg.slugName << cfg.channelNum << cfg.serverNum;
    if (cfg.merged)
    {
        // TODO:
    }
    st << Stream::eos;
	send(&st[0], st.size());
}

int ArenaConn::parsePacket( struct evbuffer * buf, int &off, int &len )
{
	size_t l = evbuffer_get_length(buf);
	if(l < 4)
	{
		off = 0;
		len = 0;
		return 0;
	}
	UInt8 * buf_ = static_cast<UInt8 *>(evbuffer_pullup(buf, 4));
	UInt32 len2 = *reinterpret_cast<UInt16 *>(buf_);
	if(len2 + 4 > l)
	{
		off = 0;
		len = 0;
		return 0;
	}

	off = 4;
	len = len2 + 4;

	switch(buf_[2])
	{
	case 0xEF:
		return 0x100 + buf_[3];
	default:
		return 0;
	}
	return 0;
}

void ArenaConn::onRecv( int cmd, int len, void * buf )
{
	ArenaMsgHdr hdr( cmd, WORKER_THREAD_WORLD, id(), len );
	GLOBAL().PushMsg( hdr, buf );
}

void ArenaConn::onDisconnected()
{
	ArenaMsgHdr hdr( 0x101, WORKER_THREAD_WORLD, id(), 1 );
	UInt8 r = 1;
	GLOBAL().PushMsg( hdr, &r );
	_host->lostConnection(1);
}

}

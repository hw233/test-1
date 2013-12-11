#include "Config.h"
#include "ServerWarConn.h"

#include "TcpServer.h"

#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include "Common/StringTokenizer.h"
//#include "Common/Itoa.h"

namespace Network
{

ServerWarConn::ServerWarConn( int fd, Network::TcpSlaveServer * s, int id ) :
	TcpConduit(fd, s, id)
{
}

bool ServerWarConn::enabled()
{
	//return cfg.arenaPort > 0 && cfg.serverWarPort > 0;
	return cfg.serverWarPort > 0;
}

void ServerWarConn::initConnection()
{
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ResolveAddress(cfg.serverWarHost.c_str()));
	addr.sin_port = htons(cfg.serverWarPort);
    int n = bufferevent_socket_connect(_bev, (struct sockaddr *)&addr, sizeof(addr));
	//if(bufferevent_socket_connect(_bev, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	if(n < 0)
		throw std::bad_exception();
	Stream st(SERVERWARREQ::REG, 0xEE);
	st << cfg.slugName << cfg.channelNum << cfg.serverNo << cfg.merged;
    if (cfg.merged)
    {
        size_t offset = st.size();
        UInt8 cnt = 0;
        st << cnt;
        StringTokenizer tk(cfg.mergeList, " ");
        for(UInt8 i = 0; i < tk.count(); ++ i)
        {
            st << atoi(tk[i].c_str());
            ++ cnt;
        }
        st.data<UInt8>(offset) = cnt;
    }
    st << Stream::eos;
	send(&st[0], st.size());
}

int ServerWarConn::parsePacket( struct evbuffer * buf, int &off, int &len )
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
	case 0xEE:
		return 0x100 + buf_[3];
	default:
		return 0;
	}
	return 0;
}

void ServerWarConn::onRecv( int cmd, int len, void * buf )
{
	ServerWarMsgHdr hdr( cmd, WORKER_THREAD_WORLD, id(), len );
	GLOBAL().PushMsg( hdr, buf );
}

void ServerWarConn::onDisconnected()
{
	ServerWarMsgHdr hdr( 0x121, WORKER_THREAD_WORLD, id(), 1 );
	UInt8 r = 1;
	GLOBAL().PushMsg( hdr, &r );
	_host->lostConnection(2);
}

}

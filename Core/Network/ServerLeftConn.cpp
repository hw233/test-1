#include "Config.h"
#include "ServerLeftConn.h"

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

ServerLeftConn::ServerLeftConn( int fd, Network::TcpSlaveServer * s, int id ) :
	TcpConduit(fd, s, id)
{
}

bool ServerLeftConn::enabled()
{
	return cfg.serverLeftPort > 0;
}

void ServerLeftConn::initConnection()
{
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(ResolveAddress(cfg.serverLeftHost.c_str()));
	addr.sin_port = htons(cfg.serverLeftPort);
    int n = bufferevent_socket_connect(_bev, (struct sockaddr *)&addr, sizeof(addr));
	//if(bufferevent_socket_connect(_bev, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	if(n < 0)
		throw std::bad_exception();
	Stream st(SERVERLEFTREQ::REG, 0xED);
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

int ServerLeftConn::parsePacket( struct evbuffer * buf, int &off, int &len )
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
	case 0xED:
		return 0x100 + buf_[3];
	default:
		return 0;
	}
	return 0;
}

void ServerLeftConn::onRecv( int cmd, int len, void * buf )
{
	ServerLeftMsgHdr hdr( cmd, WORKER_THREAD_WORLD, id(), len );
	GLOBAL().PushMsg( hdr, buf );
}

void ServerLeftConn::onDisconnected()
{
	ServerLeftMsgHdr hdr( 0x131, WORKER_THREAD_WORLD, id(), 1 );
	UInt8 r = 1;
	GLOBAL().PushMsg( hdr, &r );
	_host->lostConnection(3);
}

}

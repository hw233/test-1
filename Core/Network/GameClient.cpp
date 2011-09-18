#include "Config.h"
#include "GameClient.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "MsgHandler/MsgTypes.h"
#include "Server/GlobalObject.h"
#include "Server/Cfg.h"
#include "MsgID.h"
#include <event2/buffer.h>

namespace Network
{

GameClient::GameClient( int fd, Network::TcpSlaveServer * s, int id ):
	TcpConduit(fd, s, id), m_Player(NULL), m_PlayerId(0), _chk(0xff), m_Chk(0xff), m_OldChk(0xff)
{
	struct sockaddr_in peerAddr;
	getAddr(&peerAddr);
	m_RemoteIP = ntohl(peerAddr.sin_addr.s_addr);
	m_RemotePort = ntohs(peerAddr.sin_port);
    setChk(0xff);
    srand(time(NULL));
}

void GameClient::setChk(UInt8 chk)
{
    m_OldChk = m_Chk;
    m_Chk = chk;
    //m_ChkOver = time(NULL) + 10*60;
    m_ChkOver = time(NULL) + 2*60;
}

int GameClient::parsePacket( struct evbuffer * buf, int &off, int &len )
{
	size_t l = evbuffer_get_length(buf);
	if(l < 4)
	{
		off = 0;
		len = 0;
		return 0;
	}
	UInt8 * buf_ = static_cast<UInt8 *>(evbuffer_pullup(buf, 4));
	/* hack: ����xml��ȫ�����flash�ͻ��� */
	if(buf_[2] == 'o' && buf_[0] == '<' && buf_[1] == 'p' && buf_[3] == 'l')
	{
		off = 0;
		len = l;
		return 0xFFFF;
	}

	UInt32 len2 = *reinterpret_cast<UInt16 *>(buf_);
	if(len2 + 4 > l)
	{
		off = 0;
		len = 0;
		return 0;
	}

	off = 5;
	len = len2 + 5;
    _chk = buf_[2];

	switch(buf_[3])
	{
	case 0xFF:
		return buf_[4];
	case 0xFE:
		if(cfg.supportCompress)
			return buf_[4] | 0x1000000;
		return 0;
	default:
		return ((static_cast<UInt16>(buf_[3]))<<8)+static_cast<UInt16>(buf_[4]);
	}
	return 0;
}

UInt8 GameClient::threadFromCmd(GObject::Player * player, int cmd)
{
	/*
	  0 - to local thread
	  1 - to world thread
	  2 - to login thread
	*/
	static int cmdTypes[] =
	{
	/*  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  */
		0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x00
		2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x10
		0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x20
		1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x30
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x40
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x50
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, // 0x60
		1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, // 0x70
		0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, // 0x80
		0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, // 0x90
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xA0
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, // 0xB0
		0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 0xC0
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, // 0xD0
		2, 2, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, // 0xE0
		0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 3, 3, 3, 3, // 0xF0
	/*  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F  */
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	};
	if((size_t)cmd >= sizeof(cmdTypes) / sizeof(int))
		return MAX_THREAD_NUM;
	switch(cmdTypes[cmd])
	{
	case 0:
		{
			if(player == NULL)
				return MAX_THREAD_NUM;
			UInt8 thid = player->getThreadId();
			if(thid >= WORKER_THREAD_COUNTRY_1 && thid <= WORKER_THREAD_NEUTRAL)
				return thid;
		}
		break;
	case 1:
		return WORKER_THREAD_WORLD;
		break;
	case 2:
		return WORKER_THREAD_LOGIN;
		break;
	case 3:
		if(!cfg.isAdminIPAllowed(m_RemoteIP))
			return MAX_THREAD_NUM;
		return WORKER_THREAD_LOGIN;
		break;
	default:
		break;
	}
	return MAX_THREAD_NUM;
}

//�ڴ˶���Ϣ�ĳ���������飬 ������ִ�cmd��Ӧ����Ϣ�����Ȳ��ԣ���ֱ�ӶϿ�������, �������ڲ���Ϣ���߳��ڲ��� �ͷŴ��û����������
void GameClient::onRecv( int cmd, int len, void * buf )
{
	/* hack: ����xml��ȫ�����flash�ͻ��� */
	if(cmd > 0x1FF)
	{
		static char resp[1024] = {0};
		static volatile int resp_len = 0;
		if(resp_len == 0)
		{
			resp_len = -1;
			sprintf(resp, "<?xml version=\"1.0\"?>"
				"<cross-domain-policy>"
				"<allow-access-from domain=\"*\" to-ports=\"*\" />"
				"</cross-domain-policy>");
			resp_len = strlen(resp) + 1;
		}
		send(resp, resp_len);
		pendClose();
		return;
	}

	GObject::Player * pl = m_Player.value();
	UInt8 thrd = threadFromCmd(pl, cmd);
	if(thrd >= MAX_THREAD_NUM)
		return;

    if (cmd == REP::KEEP_ALIVE)
    {
        //if (!(m_ChkOver % 6))
        {
            UInt8 chk = rand()%257;
            if (!chk) chk = 0xff;
            setChk(chk);
            Stream st(REP::USER_INFO_CHANGE);
            st << static_cast<UInt8>(0x14) << static_cast<UInt32>(chk);
            st << Stream::eos;
            pl->send(st);
        }
    }

    if (cmd != REP::KEEP_ALIVE)
    {
        if (_chk != m_Chk)
        {
            if (_chk != m_OldChk)
                return;
        }
    }

	if(thrd == WORKER_THREAD_LOGIN)
	{
		if(pl == NULL)
		{
			LoginMsgHdr hdr( cmd, WORKER_THREAD_LOGIN, m_PlayerId, id(), len );
			GLOBAL().PushMsg( hdr,  buf );
		}
	}
	else
	{
		if(pl != NULL)
		{
			GameMsgHdr hdr( cmd, thrd, pl, len );
			GLOBAL().PushMsg( hdr,  buf );
		}
	}
}

void GameClient::onDisconnected()
{
	GObject::Player * pl = m_Player;

	if(pl == NULL)
		return;

	GameMsgHdr hdr(0xFF, WORKER_THREAD_LOGIN, pl, 0);

	GLOBAL().PushMsg( hdr,  NULL );
}

bool GameClient::active()
{
	GObject::Player * player = m_Player.value();
	return player != NULL && player->isOnline();
}

void GameClient::SetClientAddr( const struct sockaddr * addr )
{
	const struct sockaddr_in * peerAddr = reinterpret_cast<const struct sockaddr_in *>(addr);
	m_RemoteIP = ntohl(peerAddr->sin_addr.s_addr);
	m_RemotePort = ntohs(peerAddr->sin_port);
}

}

#ifndef _GAMECLIENT_H_
#define _GAMECLIENT_H_

#include "Common/AtomicVal.h"

#include "TcpConduit.h"

namespace GObject
{
	class Player;
}

namespace Network
{

class GameClient:
	public TcpConduit
{
public:
	GameClient(int fd, Network::TcpSlaveServer * s, int id);

protected:
	virtual int	parsePacket(struct evbuffer * buf, int &off, int &len);
	virtual void onRecv(int cmd, int len, void * buf);

	virtual void onDisconnected();

	UInt8 threadFromCmd(GObject::Player *, int);
public:
	inline UInt32	GetClientIP() const { return m_RemoteIP; }
	inline UInt16	GetClientPort() const { return m_RemotePort; }

	void SetClientAddr(const struct sockaddr *);

	inline void SetPlayer( GObject::Player * pl ) { m_Player = pl; m_PlayerId = 0; }
	inline void SetPlayerId( UInt64 id ) { m_PlayerId = id; }
	inline GObject::Player * GetPlayer() const { return m_Player.value(); }
	inline UInt64 GetPlayerId( ) { return m_PlayerId; }
    void setChk(UInt8);

	virtual bool active();

private:
	AtomicVal<GObject::Player *>	m_Player;
	UInt32	m_RemoteIP;
	UInt16	m_RemotePort;
	UInt64	m_PlayerId;
    UInt8 _chk;
    UInt8 m_Chk;
    UInt8 m_OldChk;
    UInt32 m_ChkOver;
};

}

#endif // _GAMECLIENT_H_

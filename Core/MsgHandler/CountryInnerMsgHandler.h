#ifndef _COUNTRYINNERMSGHANDLER_H_
#define  _COUNTRYINNERMSGHANDLER_H_

#include "Server/GlobalObject.h"
#include "Server/Cfg.h"
#include "MsgTypes.h"
#include "MsgFunc.h"
#include "Server/WorkerThread.h"
#include "Server/WorldServer.h"
#include "CountryMsgStruct.h"
#include "Battle/BattleSimulator.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"
#include "Script/BattleFormula.h"
#ifndef _WIN32
#endif
#include "Common/StringTokenizer.h"
#include "GObject/Player.h"

//Login thread -> Country thread
void PlayerLogin( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	//const UInt8 flag = *reinterpret_cast<const UInt8 *>(data);

	player->Login();

	Stream st(REP::LOGIN);
	st << static_cast<UInt32>(0) << player->GetName() << Stream::eos;
	TcpConnection conn = NETWORK()->GetConn(player->GetSessionID());
	if(conn.get() == NULL)
	{
		return;
	}
	conn->send(&st[0], st.size());

	struct in_addr ip;
	ip.s_addr=htonl(player->getClientAddress());
	//DBLOG1().PushUpdateData("insert into login_states (server_id,player_id,login_time,login_ip) values(%u, %" I64_FMT "u, %u, '%s')", cfg.serverLogId, player->getId(), TimeUtil::Now(), inet_ntoa(ip));
}
#endif // _COUNTRYINNERMSGHANDLER_H_



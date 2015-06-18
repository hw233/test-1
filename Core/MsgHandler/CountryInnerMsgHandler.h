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


	Stream st(REP::LOGIN);
	st << static_cast<UInt8>(0) << player->getId() << Stream::eos;
	TcpConnection conn = NETWORK()->GetConn(player->GetSessionID());
	if(conn.get() == NULL)
	{
		return;
	}
	conn->send(&st[0], st.size());

    if(1)
    {
        Stream st(REP::USER_INFO);
        player->makePlayerInfo(st);
        st << Stream::eos;
        player->send(st);
    }
    if(0)
    { 
        Stream st(REP::MAIL);
        player->ListMail(st);
        st << Stream::eos;
        player->send(st);
    }
    if(1)
    {
         player->GetPackage()->SendPackageInfo();
    }
    if(1)
    {
        player->GetGovernManager()->GiveGovernOfflineGain(); //发送离线治理获得的物品
    }
    if(1)
    {
        UInt8 type = 1;
        GameMsgHdr hdr(0x156,WORKER_THREAD_COUNTRY_1,player,sizeof(type));
        GLOBAL().PushMsg(hdr,&type);
    }
    if(1)
    {
        player->GetFriendManager()->SendFriendList();
    }

	player->Login();
	struct in_addr ip;
	ip.s_addr=htonl(player->getClientAddress());
	//DBLOG1().PushUpdateData("insert into login_states (server_id,player_id,login_time,login_ip) values(%u, %" I64_FMT "u, %u, '%s')", cfg.serverLogId, player->getId(), TimeUtil::Now(), inet_ntoa(ip))}
}
#endif // _COUNTRYINNERMSGHANDLER_H_


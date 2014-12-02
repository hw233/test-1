#ifndef _COUNTRYOUTERMSGHANDLER_H_
#define _COUNTRYOUTERMSGHANDLER_H_

#include "MsgTypes.h"
#include "MsgID.h"

#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "MsgFunc.h"

#include "CountryMsgStruct.h"
#include "Server/SysMsg.h"
#include "Battle/BattleSimulator.h"
#include "GObject/Var.h"

#include "Common/Serialize.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "Common/BinaryReader.h"
#include "LoginMsgHandler.h"

#include "Memcached.h"

struct NullReq
{
    UInt32 ticket;
    MESSAGE_DEF1(REQ::KEEP_ALIVE, UInt32, ticket);
};
/*
void OnNullReq( GameMsgHdr& hdr, NullReq& nr )
{
    MSG_QUERY_PLAYER(player);
    Stream st(REP::KEEP_ALIVE);
    st << nr.ticket << Stream::eos;
    player->send(st);
}
*/

/*
void OnQixiReq2(GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    if(!player->hasChecked())
        return;
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    UInt8 type = 0;

    brd >> type;
    switch(type)
    {
        default:
            break;
    }
}
*/


#endif // _COUNTRYOUTERMSGHANDLER_H_


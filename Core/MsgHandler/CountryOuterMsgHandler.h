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
#include "GObject/Player.h"
#include "GObject/Fighter.h"

struct NullReq
{
    UInt32 ticket;
    MESSAGE_DEF1(REQ::KEEP_ALIVE, UInt32, ticket);
};

struct PlayerInfoReq
{
    MESSAGE_DEF(REQ::USER_INFO) ;
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

void OnPlayerInfoReq(GameMsgHdr& hdr, PlayerInfoReq &)
{
    MSG_QUERY_CONN_PLAYER(conn,pl);

    pl->setOnline(true);

    //个人信息
    {
        Stream st(REP::USER_INFO);
        pl->makePlayerInfo(st);
        st << Stream::eos;
        conn->send(&st[0],st.size());
    }
}

void OnEnchantReq(GameMsgHdr& hdr, const void * data)
{ 
   MSG_QUERY_PLAYER(player) ;
   BinaryReader br(data,hdr.msgHdr.bodyLen);
   UInt8 opt = 0;
   br >> opt;
   UInt16 fighterId;
   br >> fighterId;
   UInt8 part = 0;
   br >> part;
   UInt8 res = player->GetPackage()->Enchant(fighterId,part,opt);
   Stream st(REQ::ENCHART);
   st << static_cast<UInt8>(res);
   st << Stream::eos;
   player->send(st);
} 
#endif // _COUNTRYOUTERMSGHANDLER_H_


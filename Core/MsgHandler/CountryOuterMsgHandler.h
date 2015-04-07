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

#include "Battle/BattleReport.h"
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
    UInt32 result = player->GetPackage()->EnchantFromClient(fighterId,part,opt);
    Stream st(REP::ENCHART);
    st << static_cast<UInt8>(opt);
    if(opt)
        st << static_cast<UInt8>(part);
    st << result; 
    st << Stream::eos;
    player->send(st);
} 
void OnPackageInfo(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    Stream st(REP::PACKAGE_INFO);
    player->GetPackage()->GetStream(st);
    st << Stream::eos;
    player->send(st);
} 

struct BattleReportReq 
{
    UInt32 _reportId;
    MESSAGE_DEF1(REQ::BATTLE_REPORT_REQ,UInt32,_reportId);
};
void OnBattleReportReq( GameMsgHdr& hdr, BattleReportReq& brr)
{
    MSG_QUERY_PLAYER(player);
    std::vector<UInt8> *r = Battle::battleReport[brr._reportId];
    if(r == NULL)
        return;
    player->send(&(*r)[0], r->size());
}

void OnChat(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 opt = 0;
    UInt64 playerId = 0;
    std::string context;
    br >> type >> opt;
    if(opt)
    { 
        player->SetRecChat(type,opt == 1);
        return ;
    } 

    br >> context;
    if(type ==1)
        br >>  playerId;

    switch(type)
    { 
        case 0:
            {
                player->ChatForWorld(context);
                break;
            }
        case 1:
            {
                player->ChatForFriend(playerId, context);
                break;
            }
        case 2:
            {
                player->ChatForClan(context);
                break;
            }
    } 
} 

void OnMail(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    Stream st(REP::MAIL);
    player->ListMail(st);
    st << Stream::eos;
    player->send(st);
} 
void OnMailGet(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt32 id = 0;
    br >> id;
    UInt8 res = player->ReciveMail(id);

    Stream st(REP::MAIL_GET);
    st << static_cast<UInt8>(res);
    if(!res)
        st << static_cast<UInt32>(id);
    st << Stream::eos;
    player->send(st);
} 
void OnMailGetAll(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    player->ReciveMail();
} 
void OnMailDelete(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt32 id = 0;
    br >> id;
    UInt8 res = player->DeleteMail(id);
    Stream st(REP::MAIL_DELETE);
    st << static_cast<UInt8>(res);
    if(!res)
        st << static_cast<UInt32>(id);
    st << Stream::eos;
    player->send(st);
} 
void OnMailDeleteAll(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    player->DeleteMail();
} 

#endif // _COUNTRYOUTERMSGHANDLER_H_


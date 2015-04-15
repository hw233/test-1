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
#include "GObject/Friend.h"

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
    //好友信息
    {
        Stream st(REQ::FRIEND_LIST);
        pl->GetFriendManager()->GetAllFriendStream(st);
        st<< Stream::eos;
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

struct FriendFindReq
{
    std::string _name;
    MESSAGE_DEF1(REQ::FRIEND_FIND,std::string,_name);
};

void OnFriendFindReq(GameMsgHdr& hdr,FriendFindReq& ffr)
{
    MSG_QUERY_PLAYER(player);
    if( !player )
        return;
    Stream st(REQ::FRIEND_FIND);
    bool res = player->GetFriendManager()->FindFriendByName(ffr._name);
    st<<static_cast<UInt8>(res);
    if ( res )
    {
       GObject::Player* pl = GObject::globalNamedPlayers[ffr._name];
       pl->GetSelfInfoStream(st);

    }
    st<<Stream::eos;
    player->send(st);
}

struct FriendApplyReq
{
    std::string _name;
    MESSAGE_DEF1(REQ::FRIEND_APPLY,std::string,_name);
};

void OnFriendApplyReq( GameMsgHdr& hdr,FriendApplyReq& far)
{
    MSG_QUERY_CONN_PLAYER(conn,player);
    if( !player )
        return;
    player->GetFriendManager()->ApplyAddFriend(far._name);
}

struct FriendDelReq
{
    std::string _name;
    MESSAGE_DEF1(REQ::FRIEND_DELETE,std::string,_name);
};

void OnFriendDeleteReq( GameMsgHdr& hdr,FriendDelReq& fdr)
{
    MSG_QUERY_CONN_PLAYER(conn,player);
    if( !player )
        return;
    Stream  st(REQ::FRIEND_DELETE);
    bool res = player->GetFriendManager()->DelFriendByName(fdr._name);
    st<<(static_cast<UInt8>(res));
    st<<Stream::eos;
    player->send(st);
}

struct FriendRecommandReq
{
    MESSAGE_DEF( REQ::FRIEND_RECOMMAND);
};

void OnFriendRecommandReq(GameMsgHdr& hdr,FriendRecommandReq& frr)
{
    MSG_QUERY_CONN_PLAYER(conn,player);
    if( !player )
        return;
    player->GetFriendManager()->RecommandFriend();
}

struct FriendAddReq
{
    UInt8 _type;  //0 不同意  1 同意
    std::string _name;
    MESSAGE_DEF2( REQ::FRIEND_ADD,UInt8,_type,std::string ,_name);
};

void OnFriendAddReq( GameMsgHdr& hdr,FriendAddReq& far)
{
    MSG_QUERY_CONN_PLAYER(conn,player);
    if( !player )
        return;
    if( far._type == 0 )
        player->GetFriendManager()->RefuseFriend(far._name);
    else
        player->GetFriendManager()->AgreeAddFriend(far._name);
}


struct FriendListReq
{
    MESSAGE_DEF(REQ::FRIEND_LIST) ;
};

void OnFriendListReq(GameMsgHdr& hdr,FriendListReq&)
{
    MSG_QUERY_CONN_PLAYER(conn,player);
    Stream st(REQ::FRIEND_LIST);
    player->GetFriendManager()->GetAllFriendStream(st);
    st << Stream::eos;
    player->send(st);
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
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt16  index = 0;
    br >> index;
    Stream st(REP::MAIL);
    player->ListMail(st,index);
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

struct ReplaceFighterReq
{
    UInt16 fighterId;
    MESSAGE_DEF1(REQ::GOVERN_REPLACE,UInt16,fighterId);
};

void OnReplaceFighter(GameMsgHdr& hdr,ReplaceFighterReq& rfr)
{
    MSG_QUERY_PLAYER(player) ;
    GObject::Fighter* ft = player->findFighter(rfr.fighterId);
    if( ft == NULL )
        return;
    player->GetGovernManager()->ReplaceFighter(ft);
}

struct GovernSpeedUpReq
{
    MESSAGE_DEF(REQ::GOVERN_SPEEDUP);
};

void OnGovernSpeedUp(GameMsgHdr& hdr, GovernSpeedUpReq& gsr)
{
    MSG_QUERY_PLAYER(player);
    player->GetGovernManager()->SpeedUp();
}

struct GovernInfoReq
{
    MESSAGE_DEF(REQ::GOVERN_INFO);
};

void OnGovernInfo(GameMsgHdr& hdr, GovernInfoReq& gir)
{
    MSG_QUERY_PLAYER(player);
    player->GetGovernManager()->SendGovernInfo();
} 
void OnClanFlash(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    player->SendClanListinfo(REP::CLAN_FLASH);
}

void OnClanCreate(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    std::string name;
    UInt8 index;
    br >> name >> index;
    player->CreateClan(name ,index);
}

void OnClanOption(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt8 option = 0;
    switch(option)
    { 
        case 0x01:
            {
                UInt32 clanId = 0;
                br >> clanId;
                GObject::Clan* clan = GObject::globalClan[clanId];
                if(!clan)
                    break;
                UInt8 res = clan->Apply(player);
                Stream st(REP::CLAN_OPTION);
                st << static_cast<UInt8>(1);
                st << res;
                st << Stream::eos;
                player->send(st);
                break;
            }
        case 0x02:
            { 
                std::string name;
                UInt8 type = 0;
                br >> name >> type;
                if(!player->GetClan() || player->GetClanPos() > 2)
                    break;
                GObject::Player* pl = GObject::globalNamedPlayers[name];
                if(!pl)
                    break;
                player->GetClan()->Allow(pl);
            } 
        case 0x03:
            {
                std::string name ;
                UInt8 num;
                br >> name >> num;
                GObject::Player* pl = GObject::globalNamedPlayers[name];
                if(!pl)
                    break;
                if(!player->GetClan() || player->GetClan() != pl->GetClan() || player->GetClanPos() > 2)
                    break;
                player->GetClan()->ChangePosition(player, pl, num);
            }
    } 
}
#endif // _COUNTRYOUTERMSGHANDLER_H_


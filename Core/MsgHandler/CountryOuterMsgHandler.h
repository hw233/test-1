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
#include "GMHandler.h"

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
    player->GetPackage()->SendPackageInfo();
} 

struct BattleReportReq0
{
    UInt32 _reportId;
    MESSAGE_DEF1(REQ::BATTLE_REPORT_REQ,UInt32,_reportId);
};

struct BattleReportReq1
{
    UInt32 _reportId;
    MESSAGE_DEF1(REQ::BATTLE_REPORT_REQ1,UInt32,_reportId);
};

void OnBattleReportReq0( GameMsgHdr& hdr, BattleReportReq0& brr)
{
    MSG_QUERY_PLAYER(player);
    std::vector<UInt8> *r = Battle::battleReport0[brr._reportId];
    if(r == NULL)
        return;
    player->send(&(*r)[0], r->size());
}

void OnBattleReportReq1( GameMsgHdr& hdr, BattleReportReq1& brr)
{
    MSG_QUERY_PLAYER(player);
    std::vector<UInt8> *r = Battle::battleReport1[brr._reportId];
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
    Stream st(REP::FRIEND_FIND);
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
    Stream  st(REP::FRIEND_DELETE);
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
    UInt8 type;
    UInt8 index;
    MESSAGE_DEF2(REQ::FRIEND_LIST,UInt8,type,UInt8,index);
};

void OnFriendListReq(GameMsgHdr& hdr,FriendListReq& flr)
{
    MSG_QUERY_CONN_PLAYER(conn,player);
    player->GetFriendManager()->SendFriendList(flr.type,flr.index);
}

struct FriendBaseInfoReq
{
    MESSAGE_DEF(REQ::FRIEND_BASEINFO);
};


void OnFriendBaseInfoReq( GameMsgHdr& hdr,FriendBaseInfoReq& fbr)
{
    MSG_QUERY_CONN_PLAYER(conn,player);
    player->GetFriendManager()->SendFriendBaseInfo();
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
    UInt8 res = player->ListMail(st,index);
    st << Stream::eos;
    if(!res)
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
    player->GetGovernManager()->SendBaseInfo();
} 
void OnClanFlash(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st(REP::CLAN_OPTION);
    st << static_cast<UInt8>(0x01);
    player->SendClanListinfo(st);
    st << Stream::eos;
    player->send(st);
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
    br >> option;
    switch(option)
    { 
        case 0x01:
            {
                Stream st(REP::CLAN_OPTION);
                st << static_cast<UInt8>(0x01);
                player->SendClanListinfo(st);
                st << Stream::eos;
                player->send(st);
            }
            break;
        case 0x02:
            {
                std::string clanName;
                br >> clanName;
                GObject::Clan* clan = GObject::globalNamedClans[clanName];
                if(!clan)
                    break;
                Stream st(REP::CLAN_OPTION);
                st << static_cast<UInt8>(0x02);
                clan->GetClanInfo(st);
                st << Stream::eos;
                player->send(st);
            }
            break;
        case 0x03:
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
        case 0x04:
            {
                std::string announcement;
                br >> announcement;
                if(player->GetClan())
                { 
                    player->GetClan()->SetAnnouncement(announcement);
                } 
                Stream st(REP::CLAN_OPTION);
                st << static_cast<UInt8>(0x04);
                st << static_cast<UInt8>(0);
                st << Stream::eos;
                player->send(st);
            }
            break;
        case 0x05:
            {
                std::string announcement;
                br >> announcement;
                if(player->GetClan())
                { 
                    player->GetClan()->SetAnnouncement2(announcement);
                } 
                Stream st(REP::CLAN_OPTION);
                st << static_cast<UInt8>(0x05);
                st << static_cast<UInt8>(0);
                st << Stream::eos;
                player->send(st);
            }
            break;
        case 0x06:
            { 
                std::string name;
                UInt8 type = 0;
                br >> name >> type;
                if(!player->GetClan() || player->GetClanPos() > 2)
                    break;
                GObject::Player* pl = GObject::globalNamedPlayers[name];
                if(!pl)
                    break;
                if(!type)
                    player->GetClan()->Allow(pl);
            } 
        case 0x07:
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
void OnGMHandler(GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    std::string gmString ;
    br >> gmString ;
    gmHandler.Handle(gmString, player);
}

void OnFindFighter(GameMsgHdr& hdr, const void * data)
{ 
    MSG_QUERY_PLAYER(player) ;
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt8 option = 0;
    br >> option;
    if(option == 0)
    { 
        UInt8 searchCount = 0;
        br >> searchCount;
        player->SearchFighter(searchCount);
    } 
    else if(option == 1)
    { 
        UInt16 fighterId = 0;
        UInt8 count = 0;
        br >> fighterId >> count;
        player->VisitFighter(fighterId,count);
    } 
}

void OnGiveOnlineAward(GameMsgHdr& hdr,const void * data)
{
    MSG_QUERY_PLAYER(player);
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt8 time = 0;
    br >> time;
    player->GetGovernManager()->SendOnlineGovernAward(time);

}

void OnSendGovernResult(GameMsgHdr& hdr,const void * data)
{
    MSG_QUERY_PLAYER(player);
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    br >> type;
    player->GetGovernManager()->SendGovernResult(type);
}


#endif // _COUNTRYOUTERMSGHANDLER_H_


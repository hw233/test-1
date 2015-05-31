#ifndef _WORLDOUTERMSGHANDLER_H_
#define _WORLDOUTERMSGHANDLER_H_

#include "MsgTypes.h"
#include "MsgFunc.h"
#include "MsgID.h"

#include "Common/Serialize.h"

#include "Server/WorldServer.h"
#include "Server/OidGenerator.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Common/Stream.h"
#include "Common/BinaryReader.h"
#include "GObject/Player.h"
#include "Battle/BattleReport.h"
#include "Battle/ClanBattleDistribute.h"

#include <mysql.h>
#include "Memcached.h"

struct BattleReportReq0 
{
    UInt32 _reportId;
    MESSAGE_DEF1(REQ::BATTLE_REPORT_REQ,UInt32,_reportId);
};

void OnBattleReportReq2( GameMsgHdr& hdr, BattleReportReq0& brr)
{
    MSG_QUERY_PLAYER(player);
    std::vector<UInt8> *r = Battle::battleReport0[brr._reportId];
    if(r == NULL)
        return;
    player->send(&(*r)[0], r->size());
}


struct BattleReportReq1 
{
    UInt32 _reportId;
    MESSAGE_DEF1(REQ::BATTLE_REPORT_REQ1,UInt32,_reportId);
};

void OnBattleReportReq2( GameMsgHdr& hdr, BattleReportReq1& brr)
{
    MSG_QUERY_PLAYER(player);
    std::vector<UInt8> *r = Battle::battleReport1[brr._reportId];
    if(r == NULL)
        return;
    player->send(&(*r)[0], r->size());
}


/*
void OnBattleReportReq2( GameMsgHdr& hdr, BattleReportReq2& brr)
{
    MSG_QUERY_PLAYER(player);
    Stream st(REP::FIGHT_REPORT2);
    st << brr.type;
    std::vector<UInt8> *r = Battle::battleReport[brr._reportId];
    if(r == NULL)
        return;
    st.append(&(*r)[4], r->size() - 4);
    st << Stream::eos;
    player->send(st);
}
*/

/*
void OnQixiReq(GameMsgHdr& hdr, const void * data)
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
        default:break;
    }
}
*/

/*
struct ClanListReq
{
    UInt8 _type;
    UInt16 _startidx;
    UInt8 _count;
    UInt8 _flag;      // 0-???? 1-%?? 2-ȫ??
    std::string _name;
    MESSAGE_DEF5(REQ::CLAN_LIST, UInt8, _type, UInt16, _startidx, UInt8, _count, UInt8, _flag, std::string, _name);
};

void OnClanListReq( GameMsgHdr& hdr, ClanListReq& clr )
{
    MSG_QUERY_PLAYER(player);
    GObject::Clan * clan = NULL;
    switch(clr._type)
    {
        case 0:
            GObject::clanCache.listAll(player, clr._startidx, clr._count, clr._flag);
            return;
        case 1:
            clan = GObject::globalNamedClans[player->getCountry()][player->fixName(clr._name)];
            break;
        case 2:
            clan = GObject::globalOwnedClans[player->getCountry()][player->fixName(clr._name)];
            break;
        default:
            return;
    }
    if(clan == NULL)
    {
        GObject::clanCache.search(player, clr._name, clr._flag);
        return;
    }
    clan->searchMatch(player);
}
*/

void OnJoinClanBattle(GameMsgHdr& hdr,const void * data)
{
    MSG_QUERY_PLAYER(player);
    if( !player )
        return;
    UInt8 res = player->SignUpClanBattle();

    Stream st(REP::CLAN_BATTLE_JOIN);
    st<<static_cast<UInt8>(res);
    st<<Stream::eos;
    player->send(st);
}

void OnClanBattleInfo(GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    if( !player )
        return;
    Stream  st(REP::CLAN_BATTLE_INFO);
    Battle::battleDistribute.GetBattleInfo(player,st);
    st<<Stream::eos;
    player->send(st);
}

void OnClanBattleAddFighter(GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    if( !player )
        return;
    UInt8 mapId = 0;
    UInt16 fighterId = 0;
    UInt8 posx = 0;
    UInt8 posy = 0;
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    brd >> mapId;
    brd >> fighterId;
    brd >> posx;
    brd >> posy;
    bool res = Battle::battleDistribute.PutFighter(mapId,player,fighterId,posx,posy,1);
    Stream st(REP::CLAN_BATTLE_ADDFIGHTER);
    if( res == false )
    {
       st<<static_cast<UInt8>(mapId);
       st<<static_cast<UInt16>(fighterId);
       st<<static_cast<UInt8>(posx);
       st<<static_cast<UInt8>(posy);
    }
    else
    {
        
    }
    st<<Stream::eos;
    player->send(st);
}


void OnClanBattleMoveFighter(GameMsgHdr& hdr,const void * data)
{
    MSG_QUERY_PLAYER(player);
    if( !player )
        return;
    UInt16 fighterId = 0;  //要移动的战将Id
    UInt8 mapId = 0;   //目标城市Id
    UInt8 posx = 0;    //目标的坐标
    UInt8 posy = 0;
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    brd >> mapId;
    brd >> fighterId;
    brd >> posx;
    brd >> posy;
    GObject::ClanBattleFighter* fighterInfo = player->GetClanBattleFighter(fighterId);
    if( fighterInfo == NULL )
        return;
    UInt8 curMapId = fighterInfo->GetMapId();
    if( mapId == 0   || curMapId == 0 )
    {
        return;
    }
    UInt8 res = false;
    if( curMapId == mapId )
    {
        res = Battle::battleDistribute.MoveFighter(mapId,player,fighterInfo->GetPosX(),fighterInfo->GetPosY(),posx,posy);
    }
    else
    {
        res = Battle::battleDistribute.MoveFighterWithDiffTown(player,curMapId,fighterInfo->GetPosX(),fighterInfo->GetPosY(),mapId,posx,posy);

    }
    Stream st(REP::CLAN_BATTLE_MOVEFIGHTER);
    st<<static_cast<UInt8>(res);
    if( res == false )
    {
        st<< static_cast<UInt8>(curMapId);
        st<< static_cast<UInt8>(fighterInfo->GetPosX());
        st<< static_cast<UInt8>(fighterInfo->GetPosY());
        st<< static_cast<UInt8>(mapId);
        st<< static_cast<UInt8>(posx);
        st<< static_cast<UInt8>(posy);
    }
    st<<Stream::eos;
    player->send(st);

}

void OnClanBattleDelFighter(GameMsgHdr& hdr,const void * data)
{
    MSG_QUERY_PLAYER(player);
    if( !player )
        return;
    UInt8 mapId = 0;
    UInt16 fighterId = 0;
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    brd >> mapId;
    brd >> fighterId;

    GObject::ClanBattleFighter* fighterInfo = player->GetClanBattleFighter(fighterId);

    if( fighterInfo == NULL )
        return;

    if( fighterInfo->GetMapId() != mapId)
        return;
    bool res =  Battle::battleDistribute.CancelPutFighter(mapId,player,fighterId,fighterInfo->GetPosX(),fighterInfo->GetPosY());
    Stream st(REP::CLAN_BATTLE_CANCELFIGHTER);
    st << static_cast<UInt8>(res);
    if( res == false )
    {
    }
    st<< Stream::eos;
    player->send(st);

}


#endif // _WORLDOUTERMSGHANDLER_H_

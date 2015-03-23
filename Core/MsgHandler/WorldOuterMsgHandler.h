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

#include <mysql.h>
#include "Memcached.h"

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


#endif // _WORLDOUTERMSGHANDLER_H_

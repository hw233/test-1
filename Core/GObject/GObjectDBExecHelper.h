#ifndef _GOBJECTDBEXECHELPER_H_
#define _GOBJECTDBEXECHELPER_H_

#include "DB/DBExecHelper.h"
#include "ItemData.h"
#include "DB/DBExecHelper.h"
#include "Server/WorldServer.h"
#include "Player.h"

namespace GData
{

}
namespace GObject
{

    struct DBGVar
    {
        UInt32 id;
        UInt32 data;
        UInt32 overTime;
    };

    struct DBPlayerData
    {
        IDTYPE id;
        std::string name;
    };              

    struct DBPlayerVar
    { 
        IDTYPE playerId;
        UInt16 id;
        UInt32 data;
        UInt32 overTime;
    };
    struct DBMailPackageData
    {
        UInt32      id;  
        UInt32      itemId;
        UInt32      itemCount;
    };

    struct DBMailData 
    {
        UInt32      id; 
        IDTYPE     playerId;
        std::string sender;
        UInt32      recvTime;
        UInt8       flag;
        std::string title;
        std::string content;
        UInt32      additionalId;
    };

    struct DBPlayer2Id
    {
        IDTYPE id;
        std::string phoneId;
        std::string accounts;
    };

    struct DBAccountPwd
    {
        std::string accounts;
        std::string password;
    };

    struct DBFighterVar
    { 
        IDTYPE playerId;
        UInt32 fighterId;
        UInt16 id;
        UInt32 data;
        UInt32 overTime;
    };

    struct DBFighterInfo
    { 
        IDTYPE playerId;
        UInt32 fighterId;
        UInt64 experience;
        UInt32 weapon;
        UInt32 armor1;
        UInt32 armor2;
        UInt32 armor3;
        UInt32 armor4;
        UInt32 armor5;
    };

    struct DBFriend
    {
        UInt8  type;
        UInt64 playerId;
        UInt64 friendId;
    };

    struct DBItem
    {
        UInt64 playerId;
        UInt32 itemId;
        UInt32 count;
    };

    struct DBClan
    {
        UInt32 clanId;
        std::string name;
        UInt8 picIndex;
        std::string announcement;
        std::string announcement2;
        UInt64 creater; 
        UInt64 leader; 
        UInt8 level;
        UInt32 contribute;
        UInt8 personMax;
        UInt32 battleRoomId;
        UInt32 clanFame;
        UInt32 conquests;
        UInt8  forceId;
    };
    struct DBClanPlayer
    {
        UInt32 clanId;
        UInt64 playerId; 
        UInt8 position;
        UInt32 contribute;
        UInt32 enterTime;
        UInt8 isClanBattle;
    };
    struct DBMail
    {
        UInt32 id;
        UInt64 playerId; 
        UInt16 contextId;
        std::string items;
        UInt8 option;
        UInt32 overTime;
    };

    struct DBClanApply
    {
        UInt32 clanId;
        UInt64 playerId; 
        UInt32 time;
    };

    struct DBClanBattlePos
    {
        UInt8 mapId;
        UInt64 playerId;
        UInt16 fighterId;
        UInt8  posx;
        UInt8 posy;
    };

    struct DBClanBattleRoom
    {
        UInt32 roomId;
        UInt8  forceId;
        UInt8  battleId;
        std::string clans;
        UInt8 fighterNum;
        UInt32 buildTime;
    };

    struct DBClanBattleComment
    {
        UInt32 roomId;
        UInt8 forceId;
        UInt8 mapId;
        UInt64 playerId;
        std::string comment;
    };

    struct DBClanBattleOrder
    {
        UInt32 roomId;
        UInt8 forceId;
        UInt8 mapId;
        UInt8 order;
    };

    struct DBReport2Id
    {
        UInt32 roomId;
        UInt8  cityId;
        UInt16  actId;
        UInt32 reportId;
        UInt32 time;
    };
}
namespace DB
{
    //
    SPECIALBEGIN(GObject::DBGVar)
    SPECIALDEF(3)
    (
     UInt32, id,
     UInt32, data,
     UInt32, overTime
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBPlayerData)
    SPECIALDEF(2)
    (
     IDTYPE, id,
     std::string, name
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBPlayerVar)
    SPECIALDEF(4)
    (
     IDTYPE, playerId,   
     UInt16, id,
     UInt32, data,
     UInt32, overTime
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBMailPackageData)  
    SPECIALDEF(3)
    (
     UInt32,     id,
     UInt32,     itemId,
     UInt32,     itemCount
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBMailData)
    SPECIALDEF(8)
    (
     UInt32,     id,
     IDTYPE,     playerId,
     std::string,sender,
     UInt32,     recvTime,
     UInt8,      flag,
     std::string,title,
     std::string,content,
     UInt32,     additionalId
    )
    SPECIALEND()
    SPECIALBEGIN(GObject::DBPlayer2Id)
    SPECIALDEF(3)
    (
     IDTYPE, id,
     std::string, phoneId,
     std::string, accounts
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBAccountPwd)
    SPECIALDEF(2)
    (
     std::string, accounts,
     std::string, password
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBFighterVar)
    SPECIALDEF(5)
    (
        IDTYPE, playerId,
        UInt32, fighterId,
        UInt16, id,
        UInt32, data,
        UInt32, overTime
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBFighterInfo)
    SPECIALDEF(9)
    (
        IDTYPE, playerId,
        UInt32, fighterId,
        UInt64, experience,
        UInt32, weapon,
        UInt32, armor1,
        UInt32, armor2,
        UInt32, armor3,
        UInt32, armor4,
        UInt32, armor5
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBFriend)
    SPECIALDEF(3)
    (
     UInt8  , type,
     UInt64 , playerId,
     UInt64 , friendId
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBItem)
    SPECIALDEF(3)
    (
     UInt64 , playerId,
     UInt32 , itemId,
     UInt32 , count
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBClan)
    SPECIALDEF(14)
    (
        UInt32, clanId,
        std::string, name,
        UInt8, picIndex,
        std::string, announcement,
        std::string, announcement2,
        UInt64, creater,
        UInt64, leader,
        UInt8, level,
        UInt32, contribute,
        UInt8, personMax,
        UInt32, battleRoomId,
        UInt32, clanFame,
        UInt32, conquests,
        UInt8, forceId
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBClanPlayer)
    SPECIALDEF(6)
    (
        UInt32, clanId,
        UInt64, playerId,
        UInt8, position,
        UInt32, contribute,
        UInt32, enterTime,
        UInt8, isClanBattle
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBMail)
    SPECIALDEF(6)
    (
        UInt32, id,
        UInt64, playerId,
        UInt16, contextId,
        std::string, items,
        UInt8, option,
        UInt32, overTime
    )
    SPECIALEND()
    
    SPECIALBEGIN(GObject::DBClanApply)
    SPECIALDEF(3)
    (
        UInt32, clanId,
        UInt64, playerId,
        UInt32, time
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBClanBattlePos)
    SPECIALDEF(5)
    (
        UInt8 , mapId,
        UInt64, playerId,
        UInt16, fighterId,
        UInt8, posx,
        UInt8, posy
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBClanBattleRoom)
    SPECIALDEF(6)
    (
        UInt32, roomId,
        UInt8, forceId,
        UInt8, battleId,
        std::string, clans,
        UInt8,fighterNum,
        UInt32,buildTime
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBClanBattleComment)
    SPECIALDEF(5)
    (
        UInt32, roomId,
        UInt8, forceId,
        UInt8, mapId,
        UInt64,playerId,
        std::string, comment
    )
    SPECIALEND()
    SPECIALBEGIN(GObject::DBClanBattleOrder)
    SPECIALDEF(4)
    (
        UInt32, roomId,
        UInt8, forceId,
        UInt8, mapId,
        UInt8, order
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBReport2Id)
    SPECIALDEF(5)
    (
        UInt32, roomId,
        UInt8, cityId,
        UInt16,actId,
        UInt32, reportId,
        UInt32, time
    )
    SPECIALEND()
}

#endif // _GOBJECTDBEXECHELPER_H_

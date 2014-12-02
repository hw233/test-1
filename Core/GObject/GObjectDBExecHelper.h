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
        std::string id;
        std::string accounts;
        std::string password;
        std::string name;
        UInt32 gold;
        UInt32 coupon;
        UInt32 tael;
    };              

    struct DBPlayerVar
    { 
        std::string playerId;
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
        std::string      playerId;
        std::string sender;
        UInt32      recvTime;
        UInt8       flag;
        std::string title;
        std::string content;
        UInt32      additionalId;
    };
}
namespace DB
{

    SPECIALBEGIN(GObject::DBGVar)
    SPECIALDEF(3)
    (
     UInt32, id,
     UInt32, data,
     UInt32, overTime
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBPlayerData)
    SPECIALDEF(7)
    (
     std::string, id,
     std::string, accounts,
     std::string, password,
     std::string, name,
     UInt32, gold,
     UInt32, coupon,
     UInt32, tael
    )
    SPECIALEND()

    SPECIALBEGIN(GObject::DBPlayerVar)
    SPECIALDEF(4)
    (
     std::string, playerId,   
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
     std::string,     playerId,
     std::string,sender,
     UInt32,     recvTime,
     UInt8,      flag,
     std::string,title,
     std::string,content,
     UInt32,     additionalId
    )
    SPECIALEND()
}

#endif // _GOBJECTDBEXECHELPER_H_

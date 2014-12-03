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
        UInt64 id;
        UInt64 phoneId;
        std::string accounts;
        std::string password;
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
    SPECIALDEF(4)
    (
     UInt64, id,
     UInt64, phoneId,
     std::string, accounts,
     std::string, password
    )
    SPECIALEND()

}

#endif // _GOBJECTDBEXECHELPER_H_

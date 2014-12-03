
#ifndef PLAYER2ID_H_
#define PLAYER2ID_H_
#include <map>
#include "Config.h"
#ifndef IDTYPE
#define IDTYPE UInt32
#endif

class Player2Id
{
    struct MapKey
    {
        const UInt64 phone ;
        const std::string accounts;
        MapKey(const UInt64 p , const std::string a):phone(p),accounts(a) { } 
        bool  operator < (const MapKey key) const 
        { 
            return phone < key.phone && accounts != key.accounts;
        } 
        bool  operator == (const MapKey key) const 
        { 
            return phone == key.phone || accounts == key.accounts;
        } 
    };
    struct  MapValue
    { 
        const std::string password;
        const IDTYPE id;
        MapValue(const std::string p,const IDTYPE i):password(p),id(i){}
    };
public:
    void InsertId(const UInt64& phoneId , const std::string& accounts , const std::string& password, const IDTYPE& playerId);
    IDTYPE getPlayerId(const UInt64& phoneId ,const std::string& accounts , const std::string& password);
private:
    std::map<MapKey , MapValue>   _map ;
};

extern Player2Id player2Id;

#endif  //PLAYER2ID_H_

// vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start 


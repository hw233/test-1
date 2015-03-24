
#ifndef PLAYER2ID_H_
#define PLAYER2ID_H_
#include <map>
#include "Config.h"
#ifndef IDTYPE
#define IDTYPE UInt64
#endif

class Player2Id
{
    struct MapKey
    {
        const std::string phone ;
        const std::string accounts;
        MapKey(const std::string p , const std::string a):phone(p),accounts(a) { } 
        bool  operator < (const MapKey key) const 
        { 
            if(!phone.empty())
                return phone < key.phone;
            if(!accounts.empty())
                return accounts < key.accounts ;
            return false;
            //return phone < key.phone && (accounts != key.accounts || !accounts.empty());
        } 
        bool  operator == (const MapKey key) const 
        { 
            if(!phone.empty())
                return phone == key.phone;
            if(!accounts.empty())
                return accounts == key.accounts;
            return false;
        } 
    };
public:
    void InsertId(const std::string& phoneId , const std::string& accounts , const IDTYPE& playerId);
    void InsertAccount(const std::string& accounts ,const std::string& password);
    IDTYPE getPlayerId(const std::string& phoneId ,const std::string& accounts);
    UInt8 CheckAccount(const std::string& accounts, const std::string& password);
private:
    std::map<MapKey , IDTYPE>   _map ;
    std::map<std::string , std::string>   _acc_pwd ;
};

extern Player2Id player2Id;

#endif  //PLAYER2ID_H_

// vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start 


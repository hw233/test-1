#include "Player2Id.h"
Player2Id player2Id;  
void Player2Id::InsertId(const UInt64& phoneId , const std::string& accounts,const std::string& password,const IDTYPE& playerId)
{ 
    struct MapKey mk = MapKey(phoneId,accounts);
    std::map<MapKey ,MapValue>::iterator it = _map.find(mk);
    if(it != _map.end())
        return ;
    struct MapValue val(password , playerId);
    _map.insert(std::map<MapKey,MapValue>::value_type (mk,val));
} 

IDTYPE Player2Id::getPlayerId(const UInt64& phoneId ,const std::string& accounts , const std::string& password)
{ 
    std::map<MapKey , MapValue>::iterator it = _map.find(MapKey(phoneId,accounts));
    if(it == _map.end())
        return 0;
    if(!phoneId)
    {
        return it->second.id;
    }
    else
    {
        if(it->second.password == password)    
            return it->second.id;
    }
    return 0;
} 




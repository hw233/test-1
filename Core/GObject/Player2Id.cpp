#include "Player2Id.h"
Player2Id player2Id;  
void Player2Id::InsertId(const std::string& phoneId , const std::string& accounts,const IDTYPE& playerId)
{ 
    struct MapKey mk = MapKey(phoneId,accounts);
    std::map<MapKey ,IDTYPE>::iterator it = _map.begin();
    for( ;it != _map.end();++it)
    {
        if( it->first == mk )
        {
            return;
        }
    }
    _map.insert(std::map<MapKey,IDTYPE >::value_type (mk,playerId));
} 

void Player2Id::InsertAccount( const std::string& accounts , const std::string& password)
{
    std::map<std::string ,std::string>::iterator it = _acc_pwd.find(accounts);
    if(it != _acc_pwd.end())
        return ; //已存在的帐号
        
    _acc_pwd.insert(std::map<std::string,std::string >::value_type(accounts, password));
}

IDTYPE Player2Id::getPlayerId(const std::string& phoneId ,const std::string& accounts)
{ 
    //std::map<MapKey , IDTYPE>::iterator it = _map.find(MapKey(phoneId,accounts));
    //std::map<MapKey , IDTYPE>::iterator it = _map.begin();//find(MapKey(phoneId,accounts));
    MapKey mk = MapKey(phoneId,accounts);
    //std::map<MapKey , IDTYPE>::iterator it = _map.find(MapKey(phoneId,accounts));
    //if( it != _map.end())
    //{
    //    return it->second;
    //}
    std::map<MapKey ,IDTYPE>::iterator it = _map.begin();
    while( it != _map.end())
    {
        if( it->first == mk )
        {
            return it->second;
        }
        ++it;
    }
    return 0;
} 

UInt8 Player2Id::CheckAccount(const std::string& accounts, const std::string& password)
{
    std::map<std::string ,std::string>::iterator it = _acc_pwd.find(accounts);
    if(it == _acc_pwd.end())
        return 3; //查无此号
    if(it->second != password)
        return 2;  //密码错误
    return 0;
}




#include "BattleManager.h"

namespace Battle
{
    BattleManager battleManager;
    UInt32 BattleManager::GetBattleGroundIndex()
    {
        for(UInt32 i = 0; i < BATTLEGROUND_MAN ; ++i)    
        { 
            if(_map.find(i) == _map.end())
                return i;
        } 
        return BATTLEGROUND_MAN;
    }

    /*
    void BattleManager::EnterBattleGround(UInt32 battleId, GObject::Player * pl , UInt8 index,UInt8 flag)
    { 
        if(_map.find(battleId) == _map.end())
            return ;
        if(!pl)
            return ;
        _map[battleId]->PushPlayer(pl,index,flag);
    } 
    */

    UInt32 BattleManager::CreateBattleGround(UInt32 backGround , UInt8 mapId,UInt8 limit)
    { 
        FastMutex::ScopedLock lk(_mutex); 
        UInt32 battleId = backGround+mapId; //GetBattleGroundIndex();
        BattleGround * bg = new BattleGround(battleId,mapId);
        _map[battleId] = bg;
        //_map.insert(std::map<UInt32,BattleGround*>::value_type (battleId,BattleGround(battleId,5,5))); //TODO
        return battleId;
    } 
    void BattleManager::StartGround(UInt32 index)
    { 
        if(_map.find(index) == _map.end())
            return ;
        _map[index]->start();
        delete _map[index];
        _map.erase(_map.find(index));
    } 
    void BattleManager::EnterBattleGround(UInt32 battleId,GObject::Player* player,UInt16 fighterId, UInt8 x,UInt8 y)
    {
        if( _map.find(battleId) == _map.end())
            return;
        _map[battleId]->PushFighter(player,fighterId,x,y);
    }

    void BattleManager::StartAllGround()
    {
        for( auto it = _map.begin(); it != _map.end(); ++it )
        {
            UInt32 battleId = it->first;
            StartGround(battleId);
        }
    }
}

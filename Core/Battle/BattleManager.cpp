#include "BattleManager.h"
#include "Battle/ClanBattleRoom.h"

namespace Battle
{

    BattleManager battleManager;
    

    void SingleBattle::StartBattle()
    {
        ground->start();
    }

    void SingleBattle::StartOneRound()
    {
        ground->FightOneRound();
    }

    SingleBattle::SingleBattle(UInt32 battleId,UInt8 mapId,UInt8 limit = 4): nextStartTime(0)
    {
        //FastMutex::ScopedLock lk(_mutex);
        battleId = battleId + mapId;
        ground = new BattleGround(battleId,mapId);
    }

    void SingleBattle::EnterBattleGround(GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y)
    {
        ground->PushFighter(player,fighterId,x,y);
    }

    void RoomBattle::InsertSingleBattle(SingleBattle* singBt)
    {
        singleBattles.push_back(singBt);
    }

    void RoomBattle::StartAllGround()
    {
        if( singleBattles.empty() )
            return;
        for(auto it = singleBattles.begin(); it != singleBattles.end(); ++it )
        {
            (*it)->StartBattle();
            delete (*it);
            (*it) = NULL;
        }
    }


    //一回合
    void RoomBattle::StartAllGroundWithOneRound()
    {
        if( singleBattles.empty() )
            return;
        for(auto it = singleBattles.begin(); it != singleBattles.end(); ++it )
        {
            (*it)->StartOneRound();
        }
    }



    UInt8 RoomBattle::GetStage()
    {
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return 0;
        return (room->GetStage());
    }

    void BattleManager::InsertRoomBattle(RoomBattle* roomBattle)
    {
        roomBattleList.push_back(roomBattle);
    }

    void BattleManager::StartAll()
    {
        if( roomBattleList.empty() )
            return;
        for(auto it = roomBattleList.begin(); it != roomBattleList.end(); ++it )
        {
            (*it)->StartAllGround();
            delete (*it);
            (*it) = NULL;
        }
    }


    /*
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
    */

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


    /*
    UInt32 BattleManager::CreateBattleGround(UInt32 backGround , UInt8 mapId,UInt8 limit)
    { 
        FastMutex::ScopedLock lk(_mutex); 
        UInt32 battleId = backGround+mapId; //GetBattleGroundIndex();
        BattleGround * bg = new BattleGround(battleId,mapId);
        _map[battleId] = bg;
        //_map.insert(std::map<UInt32,BattleGround*>::value_type (battleId,BattleGround(battleId,5,5))); //TODO
        return battleId;
    }
    */

    /*
    void BattleManager::StartGround(UInt32 index)
    { 
        if(_map.find(index) == _map.end())
            return ;
        _map[index]->start();
        delete _map[index];
        _map.erase(_map.find(index));
    } 
    */

    /*
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
            //UInt32 battleId = it->first;
            (it->second)->start();
            //StartGround(battleId);
            delete (it->second);
        }
    }
    */
}

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
}

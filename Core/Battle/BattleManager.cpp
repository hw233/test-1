#include "BattleManager.h"
#include "Battle/ClanBattleRoom.h"
#include "Battle/ClanBattleCityStatus.h"

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
        Battle::RoomAllCityStatus* status = Battle::roomAllCityStatusManager.GetRoomAllCityStatus(roomId);
        if( status == NULL )
            return -1;
        return (status->GetStage());
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

    RoomBattle* BattleManager::GetRoomBattle(UInt32 roomId)
    {
        for( auto it = roomBattleList.begin(); it != roomBattleList.end(); ++it )
        {
            if( (*it)->GetRoomId() == roomId )
            {
                return (*it);
            }
        }
        return NULL;
    }

    void BattleManager::removeRoomBattle(UInt32 roomId)
    {
        if( roomBattleList.empty())
            return;
        for( auto it = roomBattleList.begin(); it != roomBattleList.end();)
        {
            if( (*it)->GetRoomId() == roomId )
            {
                it = roomBattleList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void RoomBattle::CollectCaptureInfo(std::map<UInt8,UInt8>& force2captureCityNum)
    {
        if( GetStage() !=  2 )
            return;
        for(auto it = singleBattles.begin(); it != singleBattles.end(); ++it )
        {
            UInt8 captureId = (*it)->GetCaptureForce();
            if( captureId != 0 )
            {
                if( !force2captureCityNum[captureId] )
                {
                    force2captureCityNum[captureId] = 1;
                }
                else
                {
                    force2captureCityNum[captureId] += 1;
                }
            }
        }
         
    }
}

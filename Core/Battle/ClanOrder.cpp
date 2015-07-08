#include"ClanOrder.h"
#include"ClanBattleRoom.h"
#include"MsgID.h"

namespace Battle
{
    RoomOrderManager roomOrderManager;

    Order* RoomOrder::GetOrder(UInt8 mapId,UInt8 forceId)
    {
        for( auto it = orders.begin(); it != orders.end(); ++it )
        {
            if( (*it)->GetMapId() == mapId && (*it)->GetForceId() == forceId)
            {
               return (*it);
            }
        }
        return NULL;
    }

    void RoomOrder::DeleteOrder(Order* order)
    {
        for(auto it = orders.begin(); it != orders.end(); )
        {
            if( *it == order)
            {
                it = orders.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void RoomOrder::InsertOrder(UInt8 forceId,UInt8 mapId,UInt8 order)
    {
        Order* newOrder = new(std::nothrow) Order(forceId,mapId,order);
        if( newOrder == NULL)
            return;
        orders.push_back(newOrder);
    }

    void RoomOrderManager::loadAllClanOrders(UInt8 roomId,UInt8 forceId,UInt8 mapId,UInt8 order)
    {
        RoomOrder* roomOrder = room2order[roomId];
        if( roomOrder == NULL )
        {
            std::vector<Order*> vecOrder;
            vecOrder.push_back(new(std::nothrow) Order(forceId,mapId,order));
            roomOrder = new(std::nothrow) RoomOrder(roomId);
            if( roomOrder == NULL )
                return;
            roomOrder->SetRoomOrder(vecOrder);
        }
        else
        {
            roomOrder->InsertOrder(forceId,mapId,order);
        }
        room2order[roomId] = roomOrder;
    }

    bool RoomOrderManager::InsertClanOrder(GObject::Player* player,UInt8 mapId,UInt8 order)
    {
        if( !player->GetJoinClanBattle())
            return false;
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
            return false;
        UInt32 roomId = clan->GetClanBattleRoomId();
        if( roomId == 0 )
            return false;
        UInt8  forceId = clan->GetBattleForceId();
        if( forceId == 0 )
            return false;
        UInt8 pos =  player->GetClanPos();
        if( pos > 2 )
            return false;
        RoomOrder* roomOrder = room2order[roomId];
        std::vector<Order*> vecOrder;
        if( roomOrder == NULL )
        {
            vecOrder.push_back(new(std::nothrow) Order(forceId,mapId,order));
            roomOrder = new(std::nothrow) RoomOrder(roomId);
            if( roomOrder == NULL)
                return false;
            roomOrder->SetRoomOrder(vecOrder);
        }
        else
        {
            Order* pOrder = roomOrder->GetOrder(mapId,forceId);
            if( pOrder != NULL )
            {
                roomOrder->DeleteOrder(pOrder);
                delete pOrder;
            }
            roomOrder->InsertOrder(forceId,mapId,order);
        }

        room2order[roomId] = roomOrder;

        //更新数据库
        DB7().PushUpdateData("REPLACE INTO `clan_battle_order`(`roomId`,`forceId`,`mapId`,`order`) value(%u,%u,%u,%u)",roomId,forceId,mapId,order);
        return true;
    }

    void RoomOrderManager::GetClanOrder(GObject::Player* player,Stream&st)
    {
        if( !player->GetJoinClanBattle())
            return;
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL)
        {
            return;
        }
        UInt32 roomId = clan->GetClanBattleRoomId();
        if( roomId == 0 )
        {
            return;
        }
        UInt8 forceId = clan->GetBattleForceId();
        if( forceId == 0 )
        {
            return;
        }
        RoomOrder* roomOrder = room2order[roomId];
        if( roomOrder == NULL )
        {
            st<<static_cast<UInt8>(0);
        }
        else
        {

            std::vector<Order*> vecOrder = roomOrder->GetOrders();
            UInt8 count = 0;
            size_t offset = st.size();
            st<<static_cast<UInt8>(count);
            for( auto it = vecOrder.begin(); it != vecOrder.end(); ++it)
            {
                if( (*it)->GetForceId() == forceId )
                {
                    st<<static_cast<UInt8>((*it)->GetMapId());
                    st<<static_cast<UInt8>((*it)->GetOrder());
                    count++;
                }
            }
            st.data<UInt8>(offset) = count;
        }
    }

    void RoomOrderManager::GetAllies(GObject::Player* player,std::set<GObject::Player*>& playerSet)
    {
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
        {
            return ;
        }
        UInt32 roomId = clan->GetClanBattleRoomId();
        if( roomId == 0 )
        {
            return ;
        }
        ClanBattleRoom* room  = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
        {
            return;
        }

        UInt8 forceId = clan->GetBattleForceId();
        if( forceId == 0 )
        {
            return;
        }
        std::vector<UInt32> vecClan = room->GetAllyClans(forceId);
        if( vecClan.empty())
        {
            return;
        }
        
        for( auto it = vecClan.begin(); it != vecClan.end(); ++it)
        {
            GObject::Clan* Clan = GObject::globalClan[(*it)];
            if( clan == NULL )
                continue;
            std::vector<GObject::Player*> vecPlayer = Clan->GetJoinClanBattlePlayer();
            if( vecPlayer.empty())
                continue;
            for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
            {
                playerSet.insert(*it);
            }

        }
    }


    void RoomOrderManager::NoticeOtherAllies(GObject::Player* player,UInt8 mapId,UInt8 order)
    {
         std::set<GObject::Player*> playerSet;
         GetAllies(player,playerSet);
         if( playerSet.empty())
             return;
         for(auto it = playerSet.begin(); it != playerSet.end(); ++it)
         {
             if( (*it) == player )
                 continue;
             Stream st(REP::CLAN_BATTLE_ORDERS);
             st<<static_cast<UInt8>(1);
             st<<static_cast<UInt8>(mapId);
             st<<static_cast<UInt8>(order);
             st<<Stream::eos;
             (*it)->send(st);
         }
    }

}

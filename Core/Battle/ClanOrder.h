#ifndef __CLANORDER_H__
#define __CLANORDER_H__

#include"GObject/Player.h"
#include"GObject/Clan.h"

namespace GObject
{
    class Player;
    class Clan;

}

namespace Battle
{
    class Order
    {
        public:
            Order(UInt8 f,UInt8 m,UInt8 o) : forceId(f),mapId(m),order(o) {}
            UInt8 GetForceId() const { return forceId;}
            UInt8 GetMapId() const { return mapId;}
            UInt8 GetOrder() const { return order;}

        private:
            UInt8 forceId;
            UInt8 mapId;
            UInt8 order;
    };

    class RoomOrder
    {
        public:
            RoomOrder(UInt32 id,std::vector<Order*> os) : roomId(id), orders(os) {}
            UInt32 GetRoomId() const { return roomId;}
            void SetRoomId(UInt32 id) { roomId = id;}
            Order* GetOrder(UInt8 mapId,UInt8 forceId);
            void InsertOrder(UInt8 forceId,UInt8 mapId,UInt8 order);
            std::vector<Order*> GetOrders() const { return orders;}
        private:
            UInt32 roomId;
            std::vector<Order*> orders;
    };

    class RoomOrderManager
    {
        public:
            void loadAllClanOrders(UInt8 roomId,UInt8 forceId,UInt8 mapId,UInt8 order);
            bool InsertClanOrder(GObject::Player* player,UInt8 mapId,UInt8 order);
            void GetClanOrder(GObject::Player* player,Stream &st);
            void NoticeOtherAllies(GObject::Player* player,UInt8 mapId,UInt8 order);
            void GetAllies(GObject::Player* player,std::set<GObject::Player*>& playerSet);
        private:
            std::map<UInt32,RoomOrder*> room2order;
    };

    extern RoomOrderManager roomOrderManager;
}
#endif

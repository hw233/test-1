
#ifndef GOBJECTMANAGER_H_
#define GOBJECTMANAGER_H_
#include <map>
#include "GLocalObjectManager.h"
#include "GGlobalObjectManager.h"
#include "Item.h"
namespace GObject
{
    class ItemEquip ;
    class GObjectManager
    {
        public:
            static void loadAllData();

            static bool InitIDGen();
            static bool loadGVar(); 
            static bool loadFighters();
            static bool loadEquipments();
            static bool unloadEquipments();
            static bool loadAllPlayers();
            static bool loadPlayerId(); 
        private:
            static std::map<UInt32, ItemEquip *> equips;
    };
}
#endif // GOBJECTMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


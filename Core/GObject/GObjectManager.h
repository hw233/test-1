
#ifndef GOBJECTMANAGER_H_
#define GOBJECTMANAGER_H_
#include <map>
#include "GLocalObjectManager.h"
#include "GGlobalObjectManager.h"
#include "Item.h"

namespace Battle
{
}

namespace GObject
{
    class ItemEquip ;
    class ItemWeapon;
    class ItemArmor;
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
            static bool loadFighter(); 
            static bool loadFighterVar(); 
            static bool loadAccountPwd(); 
            static bool loadFriend(); 
            static bool loadItem(); 
            static bool loadClan(); 
            static bool loadClanPlayer(); 
            static bool loadMail();
            static bool loadClanApply();
            static bool loadClanBattlePos();  //军团战阵形排布信息
            static bool loadClanBattleRooms();
            static ItemEquip * fetchEquipment(UInt32, bool = true);  //获取装备
            static ItemWeapon * fetchWeapon(UInt32);    //获取武器，基于fetchEquipment函数实现
            static ItemArmor * fetchArmor(UInt32);      //获取盔甲,基于fetchEquipment函数实现
            static bool loadClanBattleComment(); //军团战留言
            static bool loadClanBattleOrder(); //军团令
            static bool loadReport2Id();  //某一城市对应的战报Id
        private:
            static std::map<UInt32, ItemEquip *> equips;
    };
}
#endif // GOBJECTMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


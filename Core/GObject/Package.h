
#ifndef PACKAGE_H_
#define PACKAGE_H_
#include "Item.h"
#include "GData/GDataManager.h"
#include "GObject/GGlobalObjectManager.h"
#include "Common/URandom.h"
#include "Common/Stream.h"


namespace GObject
{

#define ARENA_BET_ITEM1    9081  
#define ARENA_BET_ITEM2    9082
    class Player;
    struct ItemKey
    {   
        ItemKey(UInt32 id_ = 0, bool bind_ = false):
            id(id_), bind(bind_) { } 
        ItemKey(UInt32 id_, UInt8 bind_):
            id(id_), bind(bind_ > 0) { } 
        UInt32 id; 
        bool bind;
        bool operator < (const ItemKey& other) const
        {   
            if(id == other.id)
            {   
                if(IsEquipId(id))
                    return false;
                return bind < other.bind;
            }   
            return id < other.id;
        }   
    };  
    class Package
    {
        public:
            Package(Player *pl):m_Owner(pl){}
            ItemBase * FindItem(UInt32 id, bool bind = false)
            {
                item_elem_iter iter;
                iter = m_Items.find(ItemKey(id, bind));
                return iter->second; 
            }

            void enumerate(Visitor<ItemBase>& visitor);

            ItemBase * AddItem(UInt32 typeId, UInt32 num, bool bind = false, bool silence = false, UInt16 fromWhere = 0);
            //ItemBase * AddEquip(UInt32 typeId, bool bind = false, bool silence = false, UInt16 fromWhere = 0);
            ItemBase * AddItemFromDB(UInt32 id, UInt32 num, bool bind = 0);

            UInt32 Enchant(UInt16 fighterId, UInt8 part);
            UInt32 EnchantFromClient(UInt16 fighterId, UInt8 part,UInt8 type);
            UInt32 UpGrade(UInt16 fighterId, UInt8 part);

            UInt32 DelItem(UInt32 id, UInt32 num, bool bind = false);
            UInt32 DelAllItem(UInt32 id, UInt32 num);
            UInt32 GetPackageSize();
            UInt32 GetLeftSize();

            void  SendPackageInfo();

        protected:
            typedef std::map<ItemKey, ItemBase *> ItemCont;
            typedef ItemCont::iterator item_elem_iter;
            Player* m_Owner;
        private:
            ItemCont m_Items;   //背包空间
    };
}
#endif // PACKAGE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


#pragma once
#ifndef __STOREA_H__
#define  __STOREA_H__

#include "Config.h"
#include"GObject/Player.h"
#define PAGE_MAX 2
#include "Script/lua_tinker.h"
#include "Country.h"
#include "Common/Stream.h"


//这个是绑在玩家身上的store  并不是通用的store

namespace GObject
{
    class Player;
    class GameActionLua;
    
    struct StoreItemInfo
    {
        public:
            StoreItemInfo(UInt32 id,UInt16 l,UInt16 p,UInt32 c): itemId(id) , limitCount(l) , price(p) , coinType(c) {}
            UInt32 GetItemId() const { return itemId;}
            UInt16 GetLimitCount() const { return limitCount;}
            UInt16 GetPrice() const { return price;}
            UInt32 GetCoinType() const { return coinType;}
            void SetLimitCount(UInt16 count ) { limitCount = count ; }
        private:
            UInt32 itemId;
            UInt16 limitCount;
            UInt16 price;
            UInt32 coinType;
    };

    class StoreA
    {
        public:
            StoreA(Player* pl): _owner(pl)  {}  
            Player* GetOwner() const { return _owner;}
            UInt8  ItemToType(UInt32 itemId );
            void Add(UInt8 pageId,UInt32 itemId,UInt16 limitCount,UInt16 price , UInt32 coinType);
            void Clear();
            void FreshItems();  //刷新物品
            void BuyFreshItems(UInt8 pageId);  //使用货币或者其他东西进行刷新
            StoreItemInfo* GetStoreItemInfo(UInt8 pageId,UInt8 index);
            void UpdateItemNum(UInt8 pageId,UInt8 index, UInt8 num);
            void GetStoreInfo(Stream& st);
            void GetStoreInfo(UInt8 pageId,Stream& st);
        private:
            Player* _owner;
            std::vector<StoreItemInfo*> _storeItems[PAGE_MAX];
    };

}
#endif

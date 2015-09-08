#include"GObject/StoreA.h"
#include"Var.h"
#include "Script/GameActionLua.h"

namespace GObject
{
    void StoreA::Add(UInt8 pageId, UInt32 itemId, UInt16 limitCount, UInt16 price, UInt32 coinType)
    {
        if( limitCount > 0xFFFFF )
            return;
        _storeItems[pageId-1].push_back(new StoreItemInfo(itemId,limitCount,price,coinType));
    }
    

    //清除物品相关信息
    void StoreA::Clear()
    {
        for(UInt8 i = 0 ; i < PAGE_MAX ; ++i )
        {
            _storeItems[i].clear();
        }
    }

    void StoreA::TestPrint()
    {
        for(UInt8 i = 0 ; i < 2 ; ++i )
        {
            std::cout<<"pageId " << static_cast<UInt32>(i+1)<<std::endl;
            for( auto it = _storeItems[i].begin(); it != _storeItems[i].end(); ++it )
            {
                std::cout<<"   itemId   "<<static_cast<UInt32>((*it)->GetItemId());
                std::cout<<"   limitCount  "<<static_cast<UInt32>((*it)->GetLimitCount());
                std::cout<<"   price   "<<static_cast<UInt32>((*it)->GetPrice());
                std::cout<<"   coinType  "<<static_cast<UInt32>((*it)->GetCoinType())<<std::endl;
            }
        }
    }

    
    //刷新物品   type 0 : 整点刷新( 每天9点 )  1 : 使用元宝进行刷新
    void StoreA::FreshItems()
    {
        Clear();
        if( !GameAction()->loadItems(_owner))
            return;
        TestPrint();
    }

    UInt32 static needCoin[2][2] = {
        { 3 , 50 },
        { 27 , 50 }
    };


    void StoreA::BuyFreshItems(UInt8 pageId)
    {
        UInt8 coinType = needCoin[pageId-1][0];
        UInt16 needNum = needCoin[pageId-1][1];
        if( _owner->GetVar( coinType ) < needNum )
            return;
        _owner->SetVar(coinType,_owner->GetVar( coinType )-needNum);
        _storeItems[pageId-1].clear();
        if( !GameAction()->loadPageItems(_owner,pageId) )
            return;
    }

    StoreItemInfo* StoreA::GetStoreItemInfo(UInt8 pageId,UInt8 index)
    {
        return _storeItems[pageId-1][index-1];
    }


    void StoreA::UpdateItemNum(UInt8 pageId,UInt8 index, UInt8 num)
    {
        StoreItemInfo* info = GetStoreItemInfo(pageId,index);
        if( info == NULL )
            return;
        info->SetLimitCount(num);
        _storeItems[pageId-1][index-1] = info;
    }

    void StoreA::GetStoreInfo(Stream& st)
    {
        st<<static_cast<UInt8>(PAGE_MAX);
        for(UInt8 i = 0 ; i < PAGE_MAX ; ++i )
        {
            st<<static_cast<UInt8>(i+1);
            st<<static_cast<UInt8>(_storeItems[i].size());
            for( auto it = _storeItems[i].begin(); it != _storeItems[i].end(); ++it )
            {
                st<<static_cast<UInt32>((*it)->GetItemId());
                st<<static_cast<UInt16>((*it)->GetLimitCount());
                st<<static_cast<UInt16>((*it)->GetPrice());
                st<<static_cast<UInt16>((*it)->GetCoinType());
            }
        }
    }

    void StoreA::GetStoreInfo(UInt8 pageId,Stream& st)
    {
        st<<static_cast<UInt8>(_storeItems[pageId-1].size());
        for( auto it = _storeItems[pageId-1].begin(); it != _storeItems[pageId-1].end(); ++it )
        {
            st<<static_cast<UInt32>((*it)->GetItemId());
            st<<static_cast<UInt16>((*it)->GetLimitCount());
            st<<static_cast<UInt16>((*it)->GetPrice());
            st<<static_cast<UInt16>((*it)->GetCoinType());
        }

    }
}

#include "Package.h"
#include "Fighter.h"
#include "Player.h"
#include "FVar.h"
namespace GObject
{
    ItemBase* Package::AddItemFromDB(UInt32 id, UInt32 num, bool bind)
    {
       // assert(!IsEquipId(id));
        const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[id];
        if(itemType == NULL) return NULL;
        ItemBase * item = new(std::nothrow) ItemBase(id, itemType);
        if (item == NULL) return NULL;
        //ITEM_BIND_CHECK(itemType->bindType,bind);
        item->SetBindStatus(bind);
        item->IncItem(num);
        {
            m_Items[ItemKey(id, bind)] = item;
        }
        return item;
    }

    UInt8 Package::Enchant(UInt16 fighterId,UInt8 part, UInt8 type)
    { 
        GObject::Fighter* fgt = m_Owner->findFighter(fighterId);
        if(!fgt)
            return 1;
        UInt32 val = fgt->GetVar(part + FVAR_WEAPON_ENCHANT );
        if(val%10 ==9 && type == 0 )
            return 2;
        if(val/10 >= 20)
            return 3;

        //XXX 扣除道具
        ++val;
        fgt->SetVar(part + FVAR_WEAPON_ENCHANT , val);
        return 0;
    } 

    ItemBase * Package::AddItem(UInt32 typeId, UInt32 num, bool bind , bool silence , UInt16 fromWhere )
    { 
       ItemBase* item = m_Items[ItemKey(typeId, bind)];
       if(!item)
       {
           const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[typeId];
           if(itemType == NULL) return NULL;
           item = new(std::nothrow) ItemBase(typeId, itemType);
           m_Items[ItemKey(typeId, bind)] = item;
       }
       item->IncItem(num);
       return item;
    } 

    UInt32 Package::DelItem(UInt32 id, UInt32 num, bool bind)
    { 
       ItemBase* item = m_Items[ItemKey(id, bind)];
       if(!item)
           return 0;
       UInt32 count = item->Count();

       if(count < num)
           return count;
       else
           item->DecItem(num);
       return num ;
    } 

    UInt32 Package::DelAllItem(UInt32 id, UInt32 num)
    { 
        UInt32 count = DelItem(id, num,1);  //绑定
        if(count == num)
            return num;
        else
        {
            UInt32 count1 = DelItem(id,num - count,0);//非绑定
            if(count1 + count == num)
            {
                DelItem(id,count,1);
                return num;
            }
        }
        return 0;
    } 
    UInt32 Package::GetPackageSize()
    { 
        UInt32 size = 0;
        for(item_elem_iter it= m_Items.begin(); it != m_Items.end(); ++it)
        { 
            size = (it->second)->Size();
        } 
        return size;
    } 
}

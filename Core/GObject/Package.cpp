#include "Package.h"
#include "Fighter.h"
#include "Player.h"
#include "FVar.h"
namespace GObject
{
    ItemBase* Package::AddItemFromDB(UInt32 id, UInt32 num, bool bind)
    {
        assert(!IsEquipId(id));
        const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[id];
        if(itemType == NULL) return NULL;
        ItemBase * item = new(std::nothrow) ItemBase(id, itemType);
        if (item == NULL) return NULL;
        ITEM_BIND_CHECK(itemType->bindType,bind);
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

        ++val;
        fgt->SetVar(part + FVAR_WEAPON_ENCHANT , val);
        return 0;
    } 
}

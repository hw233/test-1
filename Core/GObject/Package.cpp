#include "Package.h"
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
}

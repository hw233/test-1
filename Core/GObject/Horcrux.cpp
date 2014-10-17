#include "Horcrux.h"
#include "Fighter.h"
namespace GObject
{
    ItemEquip* Horcrux::SetHorcruxEquip(UInt8 pos ,ItemEquip * equip1,UInt8 flag)
    {
        ItemEquip * old = _horcrux[pos];
        if( pos > 4)
            return NULL;
        _horcrux[pos] = equip1;
        if(!flag)
        {
            if(equip1 != NULL)
            {
                _fighter->CheckEquipEnchantAttainment( equip1->getItemEquipData().enchant);
                equip1->DoEquipBind(true);
            }
            _fighter->sendModification(0x71 + pos, equip1 , false);
            //UpdateEquipToDB();
        }
        _fighter->setDirty(true);
        return old;
    }
}

#include "Fighter.h"
#include "Player.h"
#include "Item.h"
namespace GObject
{
    GlobalFighters globalFighters;

    Fighter::Fighter(UInt32 id , Player* pl):_id(id),_owner(pl),_weapon(NULL)
    { 
        //memset(_armor, 0, 5 * sizeof(ItemEquip *));
        memset(_armor, 0, sizeof(_armor));
    } 

    void Fighter::setLevelAndExp(UInt8 l , UInt64 e)   
    { 
        _level = l;
        _exp = e;
    } 

    ItemWeapon * Fighter::setWeapon( ItemWeapon * w, bool writedb )
    {
       // if (w && w->getClass() != Item_Weapon)
       //     return NULL;
        ItemWeapon * r = _weapon;
        _weapon = w;
        //更改攻击距离  TODO
        {
        
        }
        return r; //返回替换下的武器
    }
    
    ItemArmor * Fighter::setArmor( int idx, ItemArmor * a, bool writedb )
    { 
        if( idx >=5 )
            return NULL;
        ItemArmor * r = _armor[idx];
        _armor[idx] = a;
        return r;
    } 

    Fighter * Fighter::clone(Player * player)
    {
        Fighter * fgt = new Fighter(*this);
        if(player != NULL)
        {    
            fgt->_level = 1; 
            fgt->_exp = 0; 
        }    
        fgt->_owner = player;
        fgt->_weapon = NULL;
        memset(fgt->_armor, 0, 5 * sizeof(ItemEquip *));
        return fgt;
    }
}

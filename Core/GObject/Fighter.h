
#ifndef FIGHTER_H_
#define FIGHTER_H_
#include "Config.h"
#include "GGlobalObjectManager.h"
namespace GObject
{
    class Player;
    class ItemWeapon;
    class ItemArmor;
    class Fighter
    {
        public:
            Fighter(UInt32 id , Player*);
            ~Fighter();
            virtual Fighter * clone(Player * owner);
            //virtual Fighter * cloneWithEquip(Player * owner); 
            //virtual Fighter * cloneWithOutDirty(Player * player);
            //Set
            inline void setOwner(Player * p) { _owner = p; }
            inline void setName(const std::string& s) {_name = s;}
            inline void setClass(UInt8 c) { _class = c; }
            inline void setSex(UInt8 s) {_sex = s;}
            inline void setLevel(UInt8 l, bool boss = false) { _level = l; }// if (boss) worldBoss.setLevel(l); }
            inline void setExp(UInt64 e) {_exp = e;}
            void setLevelAndExp(UInt8 l, UInt64 e);   //TODO
            ItemWeapon * setWeapon(ItemWeapon * w, bool = true);  //TODO
            ItemArmor * setArmor(int idx, ItemArmor * a, bool = true);  //TODO

            //Get
            inline UInt32 getId() { return _id; }
            inline Player * getOwner() { return _owner; }
            inline UInt8 getClass() {return _class;}
            inline UInt8 getSex(){return _sex;}
            inline ItemWeapon * getWeapon() { return _weapon; } 
            inline ItemArmor * getArmor(int idx) { return (idx >= 0 && idx < 5) ? _armor[idx] : NULL; }

    private:
            UInt32 _id;
            Player * _owner;
            std::string _name;
            UInt8 _class ;
            UInt8 _sex;
            UInt64 _exp;
            UInt8 _level;
            ItemWeapon * _weapon;
            ItemArmor * _armor[5];
};
typedef GGlobalObjectManagerT<Fighter, UInt64> GlobalFighters;
extern GlobalFighters globalFighters;
}
#endif // FIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


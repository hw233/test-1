
#ifndef FIGHTER_H_
#define FIGHTER_H_
#include "Config.h"
#include "GGlobalObjectManager.h"
namespace GObject
{
    class ItemWeapon;
    class ItemArmor;
    class Player;
    class FVarSystem;

    struct FighterSkill
    {
       UInt16 skillCondId;
       UInt16 skillScopeId;
       UInt16 skillEffectId;
       FighterSkill(UInt16 sc , UInt16 ss,UInt16 se):skillCondId(sc),skillScopeId(ss),skillEffectId(se){}
       FighterSkill():skillCondId(0),skillScopeId(0),skillEffectId(0){}
    };
    class Fighter
    {
        public:
            Fighter(UInt32 id , Player*);
            ~Fighter();
            virtual Fighter * Clone(Player * owner);
            //virtual Fighter * cloneWithEquip(Player * owner); 
            //virtual Fighter * cloneWithOutDirty(Player * player);
            //Set
            inline void SetOwner(Player * p) { _owner = p; }
            inline void SetName(const std::string& s) {_name = s;}
            inline void SetClass(UInt8 c) { _class = c; }
            inline void SetSex(UInt8 s) {_sex = s;}
            inline void SetLevel(UInt8 l, bool boss = false) { _level = l; }// if (boss) worldBoss.SetLevel(l); }
            inline void SetExp(UInt64 e) {_exp = e;}
            void SetLevelAndExp(UInt8 l, UInt64 e);   //TODO
            ItemWeapon * SetWeapon(ItemWeapon * w, bool = true);  //TODO
            ItemArmor * SetArmor(int idx, ItemArmor * a, bool = true);  //TODO
            void SetColor(UInt8 color) { _color = color;}
            void SetTypeId(UInt8 type) { _typeId = type;}
            UInt8 GetTypeId(){return _typeId;}
            void SetChildTypeId(UInt8 type) { _childTypeId = type;}

            //Get
            inline UInt32 getId() { return _id; }
            inline Player * GetOwner() { return _owner; }
            inline UInt8 GetClass() {return _class;}
            inline UInt8 GetSex(){return _sex;}
            inline ItemWeapon * GetWeapon() { return _weapon; } 
            inline ItemArmor * GetArmor(int idx) { return (idx >= 0 && idx < 5) ? _armor[idx] : NULL; }
            inline UInt8 GetColor(){return _color;}

            UInt8 GetSide();//{ if(_owner) return _owner->GetBattleSide(); return 0;}
            FVarSystem* GetFVar();

            void SetRad(UInt16 rad) { _bodySize = rad;} 
            UInt16 GetRad() { return _bodySize;}

            void SetBaseAttr(UInt32 attack , UInt32 defend, UInt32 magatk, UInt32 magdef, UInt32 critical, UInt32 evade) 
            { 
                _attack = attack;
                _defend = defend;
                _magatk = magatk;
                _magdef = magdef;
                _critical = critical;
                _evade = evade;
            } 
            void SetBaseSkill(UInt8 level , UInt16 skillCondId, UInt16 skillScopeId, UInt16 skillEffectId);

            void SetSkill(std::string skill);
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
            FVarSystem* fVarSystem;

            UInt16 _bodySize;  //模型体积半径

            UInt32 _attack;
            UInt32 _defend;
            UInt32 _magatk;
            UInt32 _magdef;
            UInt32 _critical;
            UInt32 _evade;

            UInt8 _typeId;
            UInt8 _childTypeId;
            UInt8 _color; 
            std::map<UInt8,FighterSkill> m_skills;  //仅global使用
            std::vector<FighterSkill> m_baseSkills;
};
typedef GGlobalObjectManagerT<Fighter, UInt32> GlobalFighters;
extern GlobalFighters globalFighters;
}
#endif // FIGHTER_H_

/* vim: Set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


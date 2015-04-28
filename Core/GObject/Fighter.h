
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

    enum
    {
        e_attr_attack = 0,
        e_attr_magatk = 1,
        e_attr_defend = 2,
        e_attr_magdef = 3,
        e_attr_critical = 4,
        e_attr_criticalDef = 5,
        e_attr_hit = 6,
        e_attr_evade = 7,
        e_attr_max
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

            UInt8 GetChildTypeId(){return _childTypeId;}
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

            UInt32 GetVar(UInt32 num);
            void SetVar(UInt32 num, UInt32 val);

            void SetRad(UInt16 rad) { _bodySize = rad;} 
            UInt16 GetRad() { return _bodySize;}

            void SetSpeed(UInt16 speed){ _speed = speed;}

            void SetBaseAttr(UInt32 hp,UInt32 attack , UInt32 defend, UInt32 magatk, UInt32 magdef, UInt32 critical,UInt32 criticalDef,UInt32 hit, UInt32 evade) 
            { 
                _hp = hp;
                _attack = attack;
                _defend = defend;
                _magatk = magatk;
                _magdef = magdef;
                _critical = critical;
                _criticalDef = critical;
                _hit = hit;
                _evade = evade;
            } 
            void SetBaseSkill(UInt8 level ,UInt16 skillId); // UInt16 skillCondId, UInt16 skillScopeId, UInt16 skillEffectId);

            void SetSkill(std::string skill);

            UInt32 GetFighterAttr(UInt8 index)
            { 
                switch(index)
                { 
                    case e_attr_attack:
                        return _attack;
                    case e_attr_magatk:
                        return _magatk;
                    case e_attr_defend:
                        return _defend;
                    case e_attr_magdef:
                        return _magdef;
                    case e_attr_critical:
                        return _critical;
                    case e_attr_criticalDef:
                        return _criticalDef;
                    case e_attr_hit:
                        return _hit;
                    case e_attr_evade:
                        return _evade;
                    case e_attr_max:
                        return _hp;
                } 
                return 0;
            } 

            void MakeFighterInfo(Stream& st);

            //强化更新
            void updateEuipmentLoad(UInt8 index);
            UInt32 GetEquipmentUpgradeLoad(UInt8 index);
            UInt32 GetTotalPower() const;  //获得总战力
            std::string GetName() const { return _name;}

            std::vector<UInt16> m_baseSkills;
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

            UInt16 _speed;  //行动速度
            UInt16 _bodySize;  //模型体积半径

            UInt32 _hp;
            UInt32 _attack;
            UInt32 _defend;
            UInt32 _magatk;
            UInt32 _magdef;
            UInt32 _critical;
            UInt32 _criticalDef;
            UInt32 _hit;
            UInt32 _evade;

            UInt8 _typeId;
            UInt8 _childTypeId;
            UInt8 _color; 
            std::map<UInt8,UInt16> m_skills;  //仅global使用
};
typedef GGlobalObjectManagerT<Fighter, UInt32> GlobalFighters;
extern GlobalFighters globalFighters;
}
#endif // FIGHTER_H_

/* vim: Set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


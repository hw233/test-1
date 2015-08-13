
#ifndef FIGHTER_H_
#define FIGHTER_H_
#include "Config.h"
#include "GGlobalObjectManager.h"
#include "FVar.h"

namespace GObject
{
    class ItemWeapon;
    class ItemArmor;
    class Player;
    class FVarSystem;

#define ENCHANT_MAX 9
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
        e_attr_hp = 8,
        e_attr_max
    };

    enum
    {
        e_fighter = 0,
        e_walk = 1,
        e_ride = 2,
        e_shoot = 3,
        e_lance = 4,
        e_advice = 5,
        e_fighter_max
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
            void SetChildTypeId(UInt16 type) { _childTypeId = type;}

            UInt16 GetChildTypeId(){return _childTypeId;}
            //Get
            inline UInt32 getId() { return _id; }
            inline Player * GetOwner() { return _owner; }
            inline UInt8 GetClass() {return _typeId;}
            inline UInt8 GetSex(){return _sex;}
            inline ItemWeapon * GetWeapon() { return _weapon; } 
            inline ItemArmor * GetArmor(int idx) { return (idx >= 0 && idx < 5) ? _armor[idx] : NULL; }
            inline UInt8 GetColor(){return _color;}

            UInt8 GetSide();//{ if(_owner) return _owner->GetBattleSide();}
            FVarSystem* GetFVar();

            UInt32 GetVar(UInt32 num);
            void SetVar(UInt32 num, UInt32 val);
            void AddVar(UInt32 num, UInt32 val);

            void SetRad(UInt16 rad) { _bodySize = rad;} 
            UInt16 GetRad() { return _bodySize;}

            UInt8 GetLevel(){return _level;}

            void SetSpeed(UInt16 speed){ _speed = speed;}

            UInt16 GetSpeed(){return _speed;}

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
                static UInt32 baseUp[]= {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
                UInt32 up = GetVar(27); // 星级（品质）编号
                if(up >= 20)
                    return 0;
                if(index >= e_attr_max)
                    return 0;
                
                // 缺少了装备的属性加成
                switch(index)
                { 
                    case e_attr_attack:
                        return _attack * (baseUp[up] + 100)/100;
                    case e_attr_magatk:
                        return _magatk * (baseUp[up] + 100)/100;
                    case e_attr_defend:
                        return _defend * (baseUp[up] + 100)/100;
                    case e_attr_magdef:
                        return _magdef * (baseUp[up] + 100)/100;
                    case e_attr_critical:
                        return _critical * (baseUp[up] + 100)/100;
                    case e_attr_criticalDef:
                        return _criticalDef * (baseUp[up] + 100)/100;
                    case e_attr_hit:
                        return _hit * (baseUp[up] + 100)/100;
                    case e_attr_evade:
                        return _evade * (baseUp[up] + 100)/100;
                    case e_attr_hp:
                        return _hp * (baseUp[up] + 100)/100;
                } 
                return 0;
            } 

            UInt32 GetFighterEquipUpAttr(UInt8 index)
            { 
               static UInt32 EquipentBase[e_fighter_max][e_attr_max] = {
                 //攻击 , 魔法攻击，防御，魔法防御，暴击，抗暴，命中 ，躲避
                   {100}, //步兵
                   {}, //骑兵
                   {}, //弓兵
                   {}, //长枪兵
                   {}  //谋士
               };
               UInt8 cls = GetTypeId();
               switch(index)
               { 
                   case e_attr_attack:
                       {
                            UInt32 value = GetVar(FVAR_WEAPON_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                   case e_attr_magatk:
                       {
                            UInt32 value = GetVar(FVAR_WEAPON_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                   case e_attr_defend:
                       {
                            UInt32 value = GetVar(FVAR_ARMOR1_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                   case e_attr_magdef:
                       {
                            UInt32 value = GetVar(FVAR_ARMOR1_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                   case e_attr_critical:
                       {
                            UInt32 value = GetVar(FVAR_ARMOR2_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                   case e_attr_criticalDef:
                       {
                            UInt32 value = GetVar(FVAR_ARMOR3_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                   case e_attr_hit:
                       {
                            UInt32 value = GetVar(FVAR_ARMOR4_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                   case e_attr_evade:
                       {
                            UInt32 value = GetVar(FVAR_ARMOR5_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
                   case e_attr_hp:
                       {
                            UInt32 value = GetVar(FVAR_ARMOR1_ENCHANT);
                            return EquipentBase[cls][index] * (2 << (value/(ENCHANT_MAX+1))) * (100 + value%(ENCHANT_MAX+1))/20;
                       }
                       break;
               } 
            } 

            void MakeFighterInfo(Stream& st);

            //强化更新
            void updateEuipmentLoad(UInt8 index);
            UInt32 GetEquipmentUpgradeLoad(UInt8 index);
            UInt32 GetTotalPower() ;  //获得总战力
            std::string GetName() const { return _name;}
            UInt32 GetHP() const { return _hp;}

            std::vector<UInt16> GetBaseSkills(){return m_baseSkills;}

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
            UInt16 _childTypeId;
            UInt8 _color; 
            std::map<UInt8,UInt16> m_skills;  //仅global使用
};
typedef GGlobalObjectManagerT<Fighter, UInt32> GlobalFighters;
extern GlobalFighters globalFighters;
}
#endif // FIGHTER_H_

#include "Player.h"
#include "Item.h"
#include "Fighter.h"
#include "FVar.h"
#include "Common/StringTokenizer.h"
namespace GObject
{
    GlobalFighters globalFighters;

    Fighter::Fighter(UInt32 id , Player* pl):_id(id),_owner(pl),_weapon(NULL),fVarSystem(NULL),_bodySize(0),_attack(0),_defend(0),_magatk(0),_magdef(0),_critical(0),_evade(0)
    { 
        //memSet(_armor, 0, 5 * sizeof(ItemEquip *));
        memset(_armor, 0, sizeof(_armor));
    } 

    void Fighter::SetLevelAndExp(UInt8 l , UInt64 e)   
    { 
        _level = l;
        _exp = e;
    } 

    ItemWeapon * Fighter::SetWeapon( ItemWeapon * w, bool writedb )
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
    
    ItemArmor * Fighter::SetArmor( int idx, ItemArmor * a, bool writedb )
    { 
        if( idx >=5 )
            return NULL;
        ItemArmor * r = _armor[idx];
        _armor[idx] = a;
        return r;
    } 

    Fighter * Fighter::Clone(Player * player)
    {
        Fighter * fgt = new Fighter(*this);
        if(player != NULL)
        {    
            fgt->_level = 1; 
            fgt->_exp = 0; 
        }    
        fgt->_owner = player;
        fgt->_weapon = NULL;
        fgt->SetColor(GetColor());
        memset(fgt->_armor, 0, 5 * sizeof(ItemEquip *));
        return fgt;
    }

    UInt8 Fighter::GetSide(){ if(_owner) return _owner->GetBattleSide(); return 0;}

    FVarSystem* Fighter::GetFVar() 
    {
        if(!fVarSystem)
            fVarSystem = new FVarSystem(_owner->getId(),getId());
        return fVarSystem;
    }

    void Fighter::SetBaseSkill(UInt8 level ,UInt16 skillId)
    { 
        if(level > 200)
            return ;
        m_skills[level] = skillId;
    } 

    void Fighter::SetSkill(std::string skill)
    { 
        StringTokenizer tk(skill, ",");
        if(!tk.count())
            return ;
        for(UInt8 i = 0; i < tk.count(); ++i)
        {
            UInt32 skillId = atoi(tk[i].c_str());
            m_baseSkills.push_back(skillId)  ;
        }
    } 
    UInt8 Fighter::Enchant(UInt8 part, UInt8 type)
    { 
        UInt32 val = GetFVar()->GetFVar(part + FVAR_WEAPON_ENCHANT );
        if(val%10 ==9 && type == 0 )
            return 2;
        if(val/10 >= 20)
            return 3;

        ++val;
        return 0;
    } 
}

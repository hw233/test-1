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

    UInt32 Fighter::GetVar(UInt32 num)
    { 
       return GetFVar()->GetFVar(num) ;
    } 
    
    void Fighter::SetVar(UInt32 num, UInt32 val)
    { 
        GetFVar()->SetFVar(num,val);
    } 

    void Fighter::AddVar(UInt32 num, UInt32 val)
    { 
        GetFVar()->AddFVar(num,val);
    } 
    void Fighter::MakeFighterInfo(Stream& st)
    { 
       st << static_cast<UInt16>(getId());
       st << static_cast<UInt8>(0);  //状态
       st << static_cast<UInt8>(0);  //品质
       st << static_cast<UInt8>(GetVar(FVAR_QUALITY));  //星级
       st << static_cast<UInt8>(0);  //星级经验
       for(UInt8 i = 0; i < 6; ++i) 
       { 
           st << static_cast<UInt8>(GetVar(FVAR_WEAPON_ENCHANT+i)%10);
           st << static_cast<UInt8>(GetVar(FVAR_WEAPON_ENCHANT+i)/10);
           st << static_cast<UInt16>(GetEquipmentUpgradeLoad(i));
       } 
       st << static_cast<UInt8>(m_baseSkills.size());
       for(UInt8 i = 0; i < m_baseSkills.size(); ++i)
       { 
           st << static_cast<UInt16>(m_baseSkills[i]);
           st << static_cast<UInt8>(GetVar(FVAR_SKILL0_LEVEL+i));
       } 
    } 
    void Fighter::updateEuipmentLoad(UInt8 index)
    { 
        static UInt32 chance[] = {7000,8000,8600,9200,10000};
        UInt32 load = 1;
        UInt8 level = 1;
        while(level < 10)
        { 
            UInt32 rand = uRand(10000);
            for(UInt8 i = 0; i < 5; ++i)
            { 
                if(rand < chance[i])
                {
                    level += (i+1);
                    if(level >= 10)
                        level = 10;
                    break;
                }
            } 
            load |= (1 << (level - 1));
        } 
        SetVar(FVAR_WEAPON_ROAD + index , load);
    } 
    UInt32 Fighter::GetEquipmentUpgradeLoad(UInt8 index) 
    { 
        UInt32 value = GetVar(FVAR_WEAPON_ROAD + index);
        if(!value)
        {
            updateEuipmentLoad(index);
            value = GetVar(FVAR_WEAPON_ROAD + index);
        }
        return value; 
    } 

    UInt32 Fighter::GetTotalPower() 
    {
        //return (_hp+_attack+_defend+_magatk+_magdef+_critical+_criticalDef+_hit+_evade) * (GetVar(FVAR_QUALITY)+1);
        return GetFighterAttr(e_attr_hp) 
            + GetFighterAttr(e_attr_attack)
            + GetFighterAttr(e_attr_magatk)
            + GetFighterAttr(e_attr_defend)
            + GetFighterAttr(e_attr_magdef)
            + GetFighterAttr(e_attr_critical)
            + GetFighterAttr(e_attr_criticalDef)
            + GetFighterAttr(e_attr_hit)
            + GetFighterAttr(e_attr_evade);
    }
}

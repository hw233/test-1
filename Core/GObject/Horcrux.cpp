#include "Horcrux.h"
#include "Fighter.h"
#include "Player.h"
#include "Common/Itoa.h"
#include "Package.h"
#include "MsgID.h"
#include "GData/HorcruxHoldAttr.h"
namespace GObject
{
    float Horcrux::limit[3]={1.3,1.5,1.8};
    ItemHorcrux* Horcrux::SetHorcruxEquip(UInt8 pos ,ItemHorcrux * equip1,UInt8 flag)
    {
        if(pos > 0 && _fighter->getPotential() < limit[pos -1])
            return NULL;
        ItemHorcrux * old = _horcrux[pos];
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
        UpdateHorcruxEquipmentToDB();
        return old;
    }
    void Horcrux::AddHorcruxHoldExp(UInt8 index ,UInt32 exp)
    { 
        if(index >= HORCRUX_HOLD_MAX)
            return ;
        _exp[index] += exp;
    } 
    void Horcrux::SetHorcruxHoldExp(UInt8 index ,UInt32 exp)
    { 
        if(index >= HORCRUX_HOLD_MAX)
            return ;
        _exp[index] = exp;
    } 
    UInt32 Horcrux::GetHorcruxHoldExp(UInt8 index)
    { 
        if(index >= HORCRUX_HOLD_MAX)
            return 0;
        return _exp[index];
    } 
    void Horcrux::UpdateHorcurxHoldToDB()
    {
        if(!_fighter->getOwner())
            return ;
        DB1().PushUpdateData("replace into `fighter_horcrux`(fighterId, playerId, criticaldefExp,piercedefExp,counterdefExp, attackpierceExp ,lingshi_exp) values(%u, %" I64_FMT "u, %u,%u, %u, %u , %u)", _fighter->getId(), _fighter->getOwner()->getId(), GetHorcruxHoldExp(0),GetHorcruxHoldExp(1), GetHorcruxHoldExp(2), GetHorcruxHoldExp(3),GetHorcruxHoldExp(4));

    }
    void Horcrux::UpdateHorcruxEquipmentToDB()
    {
        std::string equipStr = "";
        for(UInt8 i = 0; i < 4; ++i)
        {
            if(_horcrux[i])
                equipStr += Itoa(_horcrux[i]->getId());
            else
                equipStr+="0";
            if( i < 3)
                equipStr +=",";
        }
        DB1().PushUpdateData("update fighter set horcrux = '%s' where id = %u and playerId = %" I64_FMT "u ", equipStr.c_str(),_fighter->getId(), _fighter->getOwner()->getId());
    }
    void Horcrux::EatHorcrux(ItemHorcrux * horcrux)
    { 
        //if (!IsEquipId(id)) return ;
        //item_elem_iter iter ;
        //iter = m_Items.find(ItemKey(id));
        //if(iter == m_Items.end())
        //    return ;
        //ItemBase * item = iter->second;
        static UInt32 exps[] = {8,15,40,80};
        if(!horcrux)
            return ;
        ItemHorcruxAttr horcruxAttr = horcrux->getHorcruxAttr();
        UInt8 color = horcrux->getQuality();
        if(color < 2 || color > 5)
            return ;

        UInt32 exp = exps[color-2];   //经验值，计算公式待定 LIBO

        for( UInt8 i = 0 ; i < 4; ++i)
        {
            if(horcruxAttr.getAttr(i)) 
            { 
                AddHorcruxHoldExp(i,exp);
            } 
        }
        if(color > 3)
            AddHorcruxHoldExp(4 , exp);  

        Package* m_package = _fighter->getOwner()->GetPackage();
        m_package->DelEquip2(horcrux,ToHorcruxHold);
    }
    void Horcrux::sendHorcruxInfo()
    { 
        Stream st(REP::EXTEND_PROTOCAOL);
        st << static_cast<UInt8>(0x06);
        st << static_cast<UInt8>(0x01);
        st << static_cast<UInt16>(_fighter->getId());
        for(UInt8 i = 0; i < HORCRUX_HOLD_MAX; ++i)
        { 
            st << _exp[i]; 
        } 
        st << Stream::eos;
        _fighter->getOwner()->send(st);
    } 
    UInt32 Horcrux::getHorcruxEquipmentTotalAttr(UInt8 index)
    { 
        UInt32 sum = 0;
        if(index > 4)
            return 0;
        for(UInt8 i = 0; i < HORCRUX_EQUIPMENT_MAX; ++i) 
        { 
            if(_horcrux[i])
                sum += _horcrux[i]->getHorcruxAttr().getAttr(index);
        } 
        return sum + GData::horcruxHoldAttr.getHorcruxHoldAttr1(_exp[index]);
    } 
}

#include "Package.h"
#include "Fighter.h"
#include "Player.h"
#include "FVar.h"
#include "Common/URandom.h"
#include "MsgID.h"
namespace GObject
{
    ItemBase* Package::AddItemFromDB(UInt32 id, UInt32 num, bool bind)
    {
        // assert(!IsEquipId(id));
        const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[id];
        if(itemType == NULL) return NULL;
        ItemBase * item = new(std::nothrow) ItemBase(id, itemType);
        if (item == NULL) return NULL;
        //ITEM_BIND_CHECK(itemType->bindType,bind);
        item->SetBindStatus(bind);
        item->IncItem(num);
        {
            m_Items[ItemKey(id, bind)] = item;
        }
        return item;
    }

    UInt32 Package::Enchant(UInt16 fighterId,UInt8 part)
    { 
        GObject::Fighter* fgt = m_Owner->findFighter(fighterId);
        if(!fgt)
            return 1;
        UInt32 val = fgt->GetVar(part + FVAR_WEAPON_ENCHANT );
        UInt8 enchant = val % 10;
        if(enchant >= 9)
            return 3;

        //XXX 扣除道具

        UInt8 AddLevel = 1;
        {
            UInt32 value = fgt->GetEquipmentUpgradeLoad(part);
            //UInt8 rand = (value >> (enchant - 1)*3) & 7;
            //while(value & (1 << (enchant+(++AddLevel))) && (enchant+AddLevel) < 9);
            for(; AddLevel < 9 - enchant ; ++AddLevel)
            {
               if(value &(1 << (enchant+AddLevel)) ) 
                   break;
            }
        }

        {
           if(enchant + AddLevel > 9) 
               val += (9 - enchant);
           else
               val += AddLevel;
        }

        fgt->SetVar(part + FVAR_WEAPON_ENCHANT , val);
        return m_Owner->GetVar(VAR_ENCHANT_CD);
    } 

    UInt32 Package::UpGrade(UInt16 fighterId, UInt8 part)
    { 
        GObject::Fighter* fgt = m_Owner->findFighter(fighterId);
        if(!fgt)
            return 1;
        UInt32 val = fgt->GetVar(part + FVAR_WEAPON_ENCHANT );
        UInt8 enchant = val % 10;
        UInt8 grade = val / 10;
        if(enchant != 9) 
            return 3;
        if( grade >= 20)
            return 3;

        //XXX 扣除道具

        UInt32 load  = 0;

        {
            fgt->updateEuipmentLoad(part);
            load = fgt->GetEquipmentUpgradeLoad(part);
            ++val;
        }

        fgt->SetVar(part + FVAR_WEAPON_ENCHANT , val);
        return load;

    } 

    UInt32 Package::EnchantFromClient(UInt16 fighterId,UInt8 part, UInt8 type)
    { 
        UInt32 result = 0;
        if(!type)
        {
            for(UInt8 i = 0;i < 8; ++i)
            { 
                if(part & (1<<i))
                {
                    result = Enchant(fighterId, i);
                }
            } 
        }
        else
            result = UpGrade(fighterId, part - 1);
        return result;
    } 


    ItemBase * Package::AddItem(UInt32 typeId, UInt32 num, bool bind , bool silence , UInt16 fromWhere )
    { 
        ItemBase* item = m_Items[ItemKey(typeId, bind)];
        UInt8 flag = 0;
        if(!item)
        {
            const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[typeId];
            if(itemType == NULL) return NULL;
            item = new(std::nothrow) ItemBase(typeId, itemType);
            m_Items[ItemKey(typeId, bind)] = item;
            flag =1;
        }
        item->IncItem(num);
        auto it = m_Items.find(ItemKey(typeId, bind));
        if(!flag)
        {
          DB7().PushUpdateData( "update item set `count`= %u where (`itemId` = %u AND `playerId` = %"I64_FMT"u) ",(it->second)->Count()+num,typeId,m_Owner->getId());
        }
        else
        {
            DB7().PushUpdateData("insert into item(itemId,playerId,count) value(%u, %" I64_FMT "u, %u )", typeId, m_Owner->getId(),num);
        }
        return item;
    } 

    UInt32 Package::DelItem(UInt32 id, UInt32 num, bool bind)
    { 
        ItemBase* item = m_Items[ItemKey(id, bind)];
        if(!item)
            return 0;
        UInt32 count = item->Count();

        if(count < num)
            return count;
        else
            item->DecItem(num);
        return num ;
    } 

    UInt32 Package::DelAllItem(UInt32 id, UInt32 num)
    { 
        UInt32 count = DelItem(id, num,1);  //绑定
        if(count == num)
            return num;
        else
        {
            UInt32 count1 = DelItem(id,num - count,0);//非绑定
            if(count1 + count == num)
            {
                DelItem(id,count,1);
                return num;
            }
        }
        return 0;
    }

    UInt32 Package::GetPackageSize()
    { 
        UInt32 size = 0;
        for(item_elem_iter it= m_Items.begin(); it != m_Items.end(); ++it)
        { 
            size = (it->second)->Size();
        } 
        return size;
    } 

    void Package::SendPackageInfo() 
    {
        if( m_Items.size() == 0 )
            return;
        Stream st(REP::PACKAGE_INFO);
        st << static_cast<UInt8>(m_Items.size());
        for(auto it = m_Items.begin();it != m_Items.end(); ++it)
        {
           st << static_cast<UInt32>(it->second->getId());
           st << static_cast<UInt16>(it->second->Count());
        }
        st << Stream::eos;
        m_Owner->send(st);
    } 
}

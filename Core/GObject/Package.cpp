#include "Package.h"
#include "Fighter.h"
#include "Player.h"
#include "FVar.h"
#include "Common/URandom.h"
#include "MsgID.h"
#include "Country.h"
#include "Script/GameActionLua.h"
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
        UInt8 enchant = val % (ENCHANT_MAX +1);
        if(enchant >= ENCHANT_MAX )
            return 3;

        //XXX 扣除道具

        UInt8 AddLevel = 1;
        {
            UInt32 value = fgt->GetEquipmentUpgradeLoad(part);
            //UInt8 rand = (value >> (enchant - 1)*3) & 7;
            //while(value & (1 << (enchant+(++AddLevel))) && (enchant+AddLevel) < 9);
            for(; AddLevel < ENCHANT_MAX - enchant ; ++AddLevel)
            {
                if(value &(1 << (enchant+AddLevel)) ) 
                    break;
            }
        }

        {
            if(enchant + AddLevel > ENCHANT_MAX) 
                val += (ENCHANT_MAX - enchant);
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
        UInt8 enchant = val % (ENCHANT_MAX+1);
        UInt8 grade = val / (ENCHANT_MAX+1);
        if(enchant != ENCHANT_MAX) 
            return 3;
        if( grade >= 20)
            return 3;

        //XXX 扣除道具
        UInt8 res = GameAction()->UpgradeCost(m_Owner, fgt->GetTypeId(), part+1, grade + 1);
        if(!res)
            return 4;

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
            result = UpGrade(fighterId, part);
        return result;
    } 


    ItemBase * Package::AddItem(UInt32 typeId, UInt32 num, bool bind , bool silence , UInt16 fromWhere )
    { 
        if( num == 0 )
            return NULL;
        if( typeId >= 20001 &&  typeId <= 20003 )
        {
            switch(typeId)
            { 
                case 20001:
                case 20002:
                    m_Owner->AddMoney(typeId-20000,num);
                    break;
                case 20003:
                    m_Owner->AddVar(VAR_ARENA_MONEY,num);
                    break;
            } 
            return NULL;
        }
        ItemBase* item = m_Items[ItemKey(typeId, bind)];
        if(!item)
        {
            const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[typeId];
            if(itemType == NULL) return NULL;
            item = new(std::nothrow) ItemBase(typeId, itemType);
            if(!item)
                return NULL;
            m_Items[ItemKey(typeId, bind)] = item;
        }

        //auto it = m_Items.find(ItemKey(typeId, bind));
        item->IncItem(num);
        DB7().PushUpdateData("  REPLACE INTO `item`(`itemId`,`playerId`,`count`) values(%u, %"I64_FMT"u, %u) ",typeId,m_Owner->GetId(),item->Count());
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
        item->DecItem(num);
        if(count == num)
        {
            DB7().PushUpdateData( "delete item  where (`itemId` = %u AND `playerId` = %"I64_FMT"u) ",id,m_Owner->getId());
        }
        else
        {
            DB7().PushUpdateData( "update item set `count`= %u where (`itemId` = %u AND `playerId` = %"I64_FMT"u) ",count - num,id,m_Owner->getId());
        }
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
    UInt32 Package::GetItemCount(UInt32 id)
    { 
        ItemBase* itemFalse = m_Items[ItemKey(id, false)];
        ItemBase* itemTrue = m_Items[ItemKey(id, true)];
        UInt32 count = 0;
        if(itemFalse)
            count += itemFalse->Count();
        if(itemTrue)
            count += itemTrue->Count();
        return count;
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
    UInt32 Package::GetLeftSize()
    {
        UInt32 size = m_Owner->GetVar(VAR_PACKAGE_SIZE);
        if(size > m_Items.size())
            return size - m_Items.size();
        return 0;
    } 

    UInt32 Package::EnchantSoldier(UInt8 index, UInt8 part)
    { 
        UInt32 time = m_Owner->GetVar(VAR_ENCHANT_CD2);
        UInt32 now = TimeUtil::Now();
        if(time > now)
            return 3;

        UInt8 offect = index + part * 10;
        UInt32 val = m_Owner->GetVar(offect + VAR_0_WALK_ENCHANT );
        UInt8 enchant = val % (ENCHANT_MAX +1);
        if(enchant >= ENCHANT_MAX )
            return 3;

        //XXX 扣除道具

        UInt8 AddLevel = 1;
        {
            UInt32 value = GetSoldierUpgradeLoad(offect);
            //UInt8 rand = (value >> (enchant - 1)*3) & 7;
            //while(value & (1 << (enchant+(++AddLevel))) && (enchant+AddLevel) < 9);
            for(; AddLevel < ENCHANT_MAX - enchant ; ++AddLevel)
            {
                if(value &(1 << (enchant+AddLevel)) ) 
                    break;
            }
        }

        {
            if(enchant + AddLevel > ENCHANT_MAX) 
                val += (ENCHANT_MAX - enchant);
            else
                val += AddLevel;
        }

        m_Owner->SetVar(offect + VAR_0_WALK_ENCHANT , val);
        m_Owner->SetVar(VAR_ENCHANT_CD2,now + 300);
        return now + 300;
    }
    UInt32 Package::UpGradeSoldier(UInt8 index,UInt8 part)
    { 
        UInt8 offect = index + part * 10;
        UInt32 val = m_Owner->GetVar(offect + VAR_0_WALK_ENCHANT );
        UInt8 enchant = val % (ENCHANT_MAX+1);
        UInt8 grade = val / (ENCHANT_MAX+1);
        if(enchant != ENCHANT_MAX) 
            return 3;
        if( grade >= 20)
            return 3;

        //XXX 扣除道具
        UInt8 res = GameAction()->UpgradeSoldierCost(m_Owner, index , part+1, grade + 1);
        //if(!res)
        //    return 4;

        UInt32 load  = 0;

        {
            updateSoldierLoad(offect);
            load = GetSoldierUpgradeLoad(offect);
            ++val;
        }

        m_Owner->SetVar(offect + VAR_0_WALK_ENCHANT , val);
        return load;
    } 

    void Package::updateSoldierLoad(UInt8 offect) //兵种，部位
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
        m_Owner->SetVar(VAR_0_WALK_ROAD + offect , load);
    } 
    UInt32 Package::GetSoldierUpgradeLoad(UInt8 offect) 
    { 
        //UInt8 offect = index + part * 10;
        UInt32 value = m_Owner->GetVar(VAR_0_WALK_ROAD + offect);
        if(!value)
        {
            updateSoldierLoad(offect);
            value = m_Owner->GetVar(VAR_0_WALK_ROAD + offect);
        }
        return value; 
    } 

    UInt32 Package::EnchantSoldierFromClient(UInt8 index, UInt8 part, UInt8 type)
    { 
        UInt32 result = 0;
        if(!type)
        {
            for(UInt8 i = 0;i < 8; ++i)
            { 
                if(part & (1<<i))
                {
                    result = EnchantSoldier(index, i);
                }
            } 
        }
        else
            result = UpGradeSoldier(index, part);
        return result;
    } 

}

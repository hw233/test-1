#include "CollectCard.h"
#include <vector>
#include <map>
#include "Server/OidGenerator.h"
#include "GObject/Player.h"
#include "GObject/Package.h"
#include "GObject/Country.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "GData/AttrExtra.h"
#include "Script/GameActionLua.h"


namespace GObject
{
        
    CollectCard::CollectCard(Player* player)
    {
        m_owner = player;
    }
    
    CollectCard::~CollectCard()
    {
    }

    bool CardInfo::checkInfo()
    {
        if(id == 0 || cid == 0 || type == 0 || level == 0)
            return false;

        return true;
    }

    void CollectCard::ReturnCardInfo(UInt8 flag)
    {
        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(1) << flag;//0x01卡牌信息
        
        size_t off = st.size();
        st << static_cast<UInt8>(0);
        UInt8 size = 0;
        for(VecSlot::iterator i = VecEquipSlot.begin(); i != VecEquipSlot.end(); i++)
        {
            if((*i)->checkInfo() == false)  
                continue;
            st << (*i)->id << (*i)->cid << (*i)->level << (*i)->exp;
            ++size;
        }

        if(RoleCard->checkInfo())//人物卡牌
        {
            st << static_cast<UInt8>(1) << RoleCard->id << RoleCard->cid << RoleCard->level << RoleCard->exp;
            ++size;
        }
        
        if (size)
            st.data<UInt8>(off) = size;

        if(flag == 1)//请求追加空闲卡牌信息
        {
            size_t off1 = st.size();
            st << static_cast<UInt8>(0);
            UInt8 size1 = 0;
            for(MSlot::iterator i = MapFreeCardSlot.begin(); i != MapFreeCardSlot.end(); i++)
            {
                if(i->second->checkInfo() == false)  
                    continue;
                st << i->second->id << i->second->cid << i->second->level << i->second->exp;
                ++size1;
            }       
            
            if (size1)
                st.data<UInt8>(off1) = size1;
        } 

        st << Stream::eos; 
        m_owner->send(st);

        return;
    }

    void CollectCard::EquipCard(UInt32 id,UInt8 pos)//装备卡牌
    {
        if(pos > 5)
            return ;

        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(1) << static_cast<UInt8>(2);//0x01卡牌信息
        
        if(pos == 0)//卸下装备
        {
            if(VecEquipSlot[pos-1]->id != id)
                return ;
            VecEquipSlot[pos-1]->pos = 0;
            VecEquipSlot.erase(VecEquipSlot.begin() + pos - 1);//
            //DB
        }
        else if(pos <= 4)
        {
            if(!VecEquipSlot[pos - 1]->checkInfo())
                return ;
            
            CardInfo* ci = VecEquipSlot[pos - 1];
            if(ci->type != 2 && ci->type != 3)//卡牌类型一定为装备卡牌和特殊卡牌
                return ;

            if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
                return ;
            CardInfo* tmp = (MapFreeCardSlot.find(id))->second;
            VecEquipSlot[pos - 1] = tmp;
            MapFreeCardSlot.erase(MapFreeCardSlot.find(id)); 

        }else if(pos == 5)
        {
            if(!RoleCard->checkInfo())
                return ;
            
            if(RoleCard->type != 2 && RoleCard->type != 3)//卡牌类型一定为装备卡牌和特殊卡牌
                return ;
            
            if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
                return ;
            CardInfo* tmp = (MapFreeCardSlot.find(id))->second;
            RoleCard = tmp; 
            MapFreeCardSlot.erase(MapFreeCardSlot.find(id)); 
        }
        
        st << static_cast<UInt8>(pos) << id ;
        st << Stream::eos; 
        m_owner->send(st);
        RebuildCardAttr(); 

        return ;
    }
    
    void CollectCard::ExchangeCard(UInt8 flag/* 0 - 手动 1- 自动*/,UInt8 level ,UInt8 color ,UInt16 count)//兑换卡牌
    {
        UInt32 iid = 9075;
        UInt16 mCount = m_owner->GetPackage()->GetItemAnyNum(iid) ;
        if(mCount < count) //天界牌不足
            return ;   
        if(level < 40 || level > 130 )
           return ;
        static UInt8 PCardChance[]= {5,4,4,3,3,2,2,2};
        std::vector<UInt16> getCards ;
        UInt16 index = 0;
        UInt8 lev = level/10-4;
        while(index < count)
        {
            UInt8 CNum= GameAction()->GetCardByChance(m_owner,_cnt[lev][0],_cnt[lev][1],_cnt[lev][2]);    
            if(CNum > 8)
                continue ;
            getCards.push_back( (static_cast<UInt16>(level)) * 10 + CNum);
            _cnt[lev][0] ++ ;
            _cnt[lev][1] ++ ;
            _cnt[lev][2] ++ ;
            if(PCardChance[CNum] > 2)
            {
               _cnt[lev][PCardChance[CNum]-3] = static_cast<UInt16>(0);
            }
            if(PCardChance[CNum] == color)
                break;
            ++index;
        }

        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(3);
        st << static_cast<UInt32>(getCards.size());
        for(UInt8 i = 0; i < getCards.size();++i)
        {
           //AddCard(level * 10 + getCards[i] , ); 
           st << static_cast<UInt16>(level*10 + getCards[i]);
        }
        st << Stream::eos;
        m_owner->send(st);

        updateCollectCnt(level);
        return;
    }
    
    void CollectCard::ActiveCardSet(UInt8 set_num)//激活卡组
    {
        
        return;
    } 

    void CollectCard::UpGradeCard(CardInfo* ci)//卡片升级
    {
        return;
    }

    void switchAttrFunc(GData::AttrExtra& ae,EAttrType type ,float num)
    {
        switch(type)
        {
            case ATTR_ATK:
                ae.attack += num;
                break;
            case ATTR_MAGATK:
                ae.magatk += num;
                break;
            case ATTR_DEF:
                ae.defend += num;
                break;
            case ATTR_MAGDEF:
                ae.magdef += num;
                break;
            case ATTR_PIERCE:
                ae.pierce += num;
                break;
            case ATTR_CRITICAL:
                ae.critical += num;
                break;
            case ATTR_EVADE:
                ae.evade += num;
                break;
            case ATTR_HITRATE:
                ae.hitrate += num;
                break;
            case ATTR_COUNTER:
                ae.counter += num;
                break;
            case ATTR_TOUGH:
                ae.tough += num;
                break;
            case ATTR_HP:
                ae.hp += num;
                break;
            default:
                break;
        }
    }

    void CollectCard::AddCardAttr(GData::AttrExtra& ae)//计算增加的属性
    {
        VecSlot::iterator it = VecEquipSlot.begin();
        if(VecEquipSlot.size() > 4)
            return;
        while(it != VecEquipSlot.end()) 
        {
            if((*it)->checkInfo())
            {
                switchAttrFunc(ae,(*it)->type1,(*it)->attr_num1);        
                switchAttrFunc(ae,(*it)->type2,(*it)->attr_num2);        
            }
            it++;
        }
       
        if(RoleCard->checkInfo())
            switchAttrFunc(ae,RoleCard->type1,RoleCard->attr_num1);        

        return;
    }

    void CollectCard::RebuildCardAttr()//重算卡片属性
    {
        std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
        for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
            it->second->setDirty();
          
        return;
    }

    bool CollectCard::AddCard(UInt16 cid ,UInt8 type ,EAttrType type1,float attr_num1,EAttrType type2,float attr_num2,UInt16 skill_id)//增加卡牌
    {
        CardInfo* ci = new CardInfo(IDGenerator::gCardOidGenerator.ID(),cid,type,static_cast<UInt8>(1),static_cast<UInt16>(0),type1,attr_num1,type2,attr_num2,skill_id,0);
        if(!ci->checkInfo())
            return false;
        MapFreeCardSlot.insert(std::make_pair(ci->id,ci)); 
        if(MapCardLog.find(cid/10)->second->id  )
        //DB
        return true;
    }
    void CollectCard::loadCollectCnt(UInt8 level ,UInt16 cnt1 ,UInt16 cnt2 ,UInt16 cnt3)
    {
        if(level < 40 || level > 130)
            return ;
        _cnt[level/10-4][0] = cnt1;
        _cnt[level/10-4][1] = cnt2;
        _cnt[level/10-4][2] = cnt3;
    }
    void CollectCard::updateCollectCnt(UInt8 level)
    {
        if(level < 40 || level  > 130 )
            return ;
        if(!m_owner)
            return ;
        DB1().PushUpdateData("REPLACE INTO `collect_cnt` (`playerId`, `level`, `bluecnt`,`purlecnt`,`orangecnt`) VALUES (%" I64_FMT "u, %u , %u , %u ,%u )", m_owner->getId(), level , _cnt[level/10-4][0],_cnt[level/10-4][1], _cnt[level/10-4][2]);
    }
        
}

#include "CollectCard.h"
#include <vector>
#include <map>
#include "Server/OidGenerator.h"
#include "GObject/Player.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "GData/AttrExtra.h"

namespace GObject
{
        
    CollectCard::CollectCard(Player* player)
    {
        m_owner = player;
        VecEquipSlot.resize(5);

    }
    
    CollectCard::~CollectCard()
    {
    }

    bool CardInfo::checkInfo()
    {
        if(this == NULL || id == 0 || cid == 0 || type == 0 || level == 0)
            return false;

        return true;
    }

    bool SuitCardInfo::checkExistSetBit(UInt8 cid)
    {
        UInt8 card_index = cid % 10;
        if(!GET_BIT(suit_mark,card_index))
            return false;  
        
        suit_mark = SET_BIT(suit_mark,card_index); 
        //TODO DB
        
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
        
        if (size)
            st.data<UInt8>(off) = size;

        if(flag == 1)//请求追加空闲卡牌信息
        {
            size_t off1 = st.size();//空闲卡牌数
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
        
            size_t off2 = st.size();//套牌个数
            st << static_cast<UInt8>(0);
            UInt8 size2 = 0;
            for(MStamp::iterator i = MapCardStamp.begin(); i != MapCardStamp.end(); i++)
            {
                if(!ReturnSuitInfo(st,i->second->id))
                    continue;
                ++size2;
            }       
            
            if (size2)
                st.data<UInt8>(off2) = size2;
    
        } 

        st << Stream::eos; 
        m_owner->send(st);

        return;
    }

    bool CollectCard::ReturnSuitInfo(Stream& st,UInt8 suit_lvl)
    {
        if(MapCardStamp.find(suit_lvl) == MapCardStamp.end())
            return false;
        MStamp::iterator it = MapCardStamp.find(suit_lvl);
    
        st << suit_lvl;
        if(suit_lvl == 20)
            st << it->second->suit_mark;
        else
            st << it->second->spe_mark;
        
        st << it->second->active;
            
        return true;
    }

    void CollectCard::EquipCard(UInt32 id,UInt8 pos)//装备卡牌
    {
        if(pos > 5)
            return ;

        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(1) << static_cast<UInt8>(2);//0x01卡牌信息
        
        if(pos == 0)//卸下装备
        {
            VecSlot::iterator it = std::find_if(VecEquipSlot.begin(),VecEquipSlot.end(),CardInfo_eq(id));
            if(it == VecEquipSlot.end())
                return;
            
            (*it)->pos = 0;
            MapFreeCardSlot.insert(std::make_pair((*it)->id,(*it))); 
            *it = NULL; 
            //VecEquipSlot.erase(VecEquipSlot.begin() + pos - 1);//
            //TODO DB
        }else if(pos <= 4)
        {
            if(VecEquipSlot[pos - 1]->checkInfo())
                return ;
            
            if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
                return ;
            CardInfo* tmp = (MapFreeCardSlot.find(id))->second;
            if(tmp->type != 2 && tmp->type != 3)//卡牌类型一定为装备卡牌和特殊卡牌
                return ;
            tmp->pos = pos; 
            VecEquipSlot[pos - 1] = tmp;
            MapFreeCardSlot.erase(MapFreeCardSlot.find(id)); 
            //TODO DB

        }else if(pos == 5)
        {
            if(VecEquipSlot[pos - 1]->checkInfo())
                return ;
            
            if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
                return ;
            CardInfo* tmp = (MapFreeCardSlot.find(id))->second;
            if(tmp->type != 1 && tmp->type != 3)//卡牌类型一定为人物卡牌和特殊卡牌
                return ;
            tmp->pos = pos; 
            VecEquipSlot[pos - 1] = tmp; 
            MapFreeCardSlot.erase(MapFreeCardSlot.find(id)); 
            //TODO DB
        }
        
        st << static_cast<UInt8>(pos) << id ;
        st << Stream::eos; 
        m_owner->send(st);
        RebuildCardAttr(); 

        return ;
    }
    
    void CollectCard::ExchangeCard(UInt8 flag/* 0 - 手动 1- 自动*/,UInt8 level)//兑换卡牌
    {

        return;
    }
    
    void CollectCard::ActiveCardSet(UInt8 set_num)//激活卡组
    {
        
        return;
    } 

    void CollectCard::UpGradeCard(UInt32 id,std::vector<UInt32>& vecid)//卡片升级
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
        
        if(MapCardStamp.find(cid/10) == MapCardStamp.end())
        {
            SuitCardInfo* si = new SuitCardInfo(cid/10);
            MapCardStamp.insert(std::make_pair(cid/10,si));
            //TODO DB
        }
        SuitCardInfo* tmp = MapCardStamp.find(cid/10)->second;
        if(!tmp->checkExistSetBit(ci->id))
            return false;
        //TODO DB
        
        return true;
    }

    bool CollectCard::DelAddCard(UInt32 id)
    {
        if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
            return false;
        MSlot::iterator it = MapFreeCardSlot.find(id);   
        delete it->second;
        MapFreeCardSlot.erase(it);

        //TODO DB
    }




}

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
#include "GData/CardSystem.h"
#include "Country.h"

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
        if(this == NULL || id == 0 || cid == 0 || type == 0 || level == 0 || color == 0 || attr_id == 0)
            return false;

        return true;
    }

    void SuitCardInfo::checkExistSetBit(UInt16 cid)
    {
        if(id != 20)
        {
            UInt8 card_index = cid % 10;
            if(GET_BIT(suit_mark,card_index-1))
                return ;  
            
            suit_mark = SET_BIT(suit_mark,card_index-1); 

            if(card_index == 1)
                collect_degree += 20;
            else if(card_index == 2 || card_index == 3)
                collect_degree += 15;
            else if(card_index == 4 || card_index == 5)
                collect_degree += 10;
            else
                collect_degree += 5;
        }
        else
        {
            UInt8 card_index = cid % 200;
            if(GET_BIT(spe_mark,card_index))
                return ;  
            
            spe_mark = SET_BIT(spe_mark,card_index); 
        }

        //TODO DB
        
        return ;
    }

    bool SuitCardInfo::checkActive(UInt8 active_set)
    {
        switch(active_set)
        {
            case 1:
                if(collect_degree >= 25)
                    return true;
                break;
            case 2:
                if(collect_degree >= 50)
                    return true;
                break;
            case 3:
                if(collect_degree >= 100)
                    return true;
                break;
            default:
                break;
        }
        return false;
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
            st << (*i)->id << (*i)->cid << (*i)->level << (*i)->exp << (*i)->pos;
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
                st << i->second->id << i->second->cid << i->second->level << i->second->exp << i->second->pos;
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
        if(suit_lvl != 20)
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
            //装备卡牌槽的御剑等级限制
            if(pos == 3)
            {
                if(m_owner->getVipLevel() < 1)
                    return;
            }
            if(pos == 4)
            {
                if(m_owner->getVipLevel() < 2)
                    return;
            }

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
        size_t offset = st.size();
        UInt32 cnt = 0;
        st <<cnt;
        //st << static_cast<UInt32>(getCards.size());
        for(UInt8 i = 0; i < getCards.size();++i)
        {
           CardInfo * ci = AddCard(level * 10 + getCards[i] ); 
           if(ci == NULL)
               continue;
           st << static_cast<UInt16>(level*10 + getCards[i]);
           st << static_cast<UInt32>(ci->id);
           ++cnt;
        }
        st.data<UInt32>(offset)=cnt;
        st << Stream::eos;
        m_owner->send(st);

        updateCollectCnt(level);
        return;
    }
    
    void CollectCard::ActiveCardSet(UInt8 suit_lvl,UInt8 active_set)//激活卡组
    {
        if(MapCardStamp.find(suit_lvl) == MapCardStamp.end())
            return ;

        SuitCardInfo* sci = MapCardStamp.find(suit_lvl)->second;
        if(sci->active >= active_set)
            return;
        else
        {
            if(sci->checkActive(active_set))
                sci->active = active_set;
            //TODO DB

        }

        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(2) ;//0x02套牌信息
        ReturnSuitInfo(st,suit_lvl);
        st << Stream::eos; 
        m_owner->send(st);
        return;
    } 

    void CollectCard::UpGradeCard(UInt32 id,std::vector<UInt32>& vecid)//卡片升级
    {
        UInt16 add_exp = 0;
        std::vector<UInt32>::iterator it = vecid.begin();
        
        if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
            return;
        CardInfo* upcard = MapFreeCardSlot.find(id)->second;

        while(it != vecid.end())
        {
            if(MapFreeCardSlot.find(*it) == MapFreeCardSlot.end())
                break;
            UInt16 cid = MapFreeCardSlot.find(*it)->second->cid;
            //TODO 找到卡牌经验值
            GData::CardInitInfo* cii = GData::csys.getCardInitInfo(cid);
            if(cii == NULL)
                break;
            upcard->exp += cii->initExp + MapFreeCardSlot.find(*it)->second->exp;
            while(GData::csys.checkUpgrade(upcard));

            DelAddCard(*it);

            it++;
        }

        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(4) ;//0x02套牌信息
        st << upcard->id << upcard->cid << upcard->level << upcard->exp << upcard->pos;

        size_t off1 = st.size();//空闲卡牌数
        st << static_cast<UInt8>(0);
        UInt8 size1 = 0;
        for(MSlot::iterator i = MapFreeCardSlot.begin(); i != MapFreeCardSlot.end(); i++)
        {
            if(i->second->checkInfo() == false)  
                continue;
            st << i->second->id << i->second->cid << i->second->level << i->second->exp << i->second->pos;
            ++size1;
        }       
        
        if (size1)
            st.data<UInt8>(off1) = size1;

        st << Stream::eos; 
        m_owner->send(st);

        return;
    }

    
    void CollectCard::AddCardAttr(GData::AttrExtra& ae)//计算增加的属性
    {
        VecSlot::iterator it = VecEquipSlot.begin();
        if(VecEquipSlot.size() > 4)
            return;
        while(it != VecEquipSlot.end()) 
        {
            if((*it)->checkInfo())
                GData::csys.AddCardAttr(ae,(*it)->attr_id,(*it)->level ,(*it)->color,(*it)->type);
            it++;
        }

        for(MStamp::iterator i = MapCardStamp.begin(); i != MapCardStamp.end(); i++)
        {
            if(i->second->active == 0 || i->second->active > 3) 
                continue;

            UInt16 attr_id = GameAction()->getsuitAttr(i->second->id);
            GData::csys.AddSuitCardAttr(ae,attr_id,i->second->active);
        }
        
        if(MapCardStamp.find(20) != MapCardStamp.end())
        {
            MStamp::iterator i = MapCardStamp.find(20); 
            if(i->second->spe_mark != 0)
            {
                UInt8 num = 0;
                while(num < 32) 
                {
                    if(GET_BIT(i->second->spe_mark,num))
                    {
                        UInt16 attr_id = GameAction()->getsuitAttr(200 + num);
                        GData::csys.AddSuitCardAttr(ae,attr_id,i->second->active);
                    }
                    num ++;
                }

            }
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

    CardInfo* CollectCard::AddCard(UInt16 cid )//增加卡牌
    {
        GData::CardInitInfo* citmp = GData::csys.getCardInitInfo(cid); 
        if(citmp == NULL)
            return NULL;

        CardInfo* ci = new CardInfo(IDGenerator::gCardOidGenerator.ID(),cid,citmp->type,static_cast<UInt8>(1),static_cast<UInt16>(0),citmp->skillId,static_cast<UInt8>(0), citmp->color,static_cast<UInt16>(32001));
        if(!ci->checkInfo())
        {
            delete ci;
            return NULL;
        }        
        MapFreeCardSlot.insert(std::make_pair(ci->id,ci)); 
        
        if(MapCardStamp.find(cid/10) == MapCardStamp.end())
        {
            SuitCardInfo* si = new SuitCardInfo(cid/10);
            MapCardStamp.insert(std::make_pair(cid/10,si));
            //TODO DB
        }
        SuitCardInfo* tmp = MapCardStamp.find(cid/10)->second;
        tmp->checkExistSetBit(ci->cid);
        //TODO DB
        
        return ci;
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
        DB1().PushUpdateData("REPLACE INTO `collect_cnt` (`playerId`, `lev`, `bluecnt`,`purlecnt`,`orangecnt`) VALUES (%" I64_FMT "u, %u , %u , %u ,%u )", m_owner->getId(), level , _cnt[level/10-4][0],_cnt[level/10-4][1], _cnt[level/10-4][2]);
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

    void CollectCard::ExchangeSpeCard(UInt16 itemid)
    {
        UInt16 cid = GameAction()->getSpeCard(itemid);        
        if(cid == 0)
            return;
        
        SuitCardInfo* tmp = MapCardStamp.find(cid/10)->second;
        if(GET_BIT(tmp->spe_mark,cid % 200))
            return;
        AddCard(cid); 
        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(2) ;//0x02套牌信息
        ReturnSuitInfo(st,20);
        st << Stream::eos; 
        m_owner->send(st);
        return;
    }
}

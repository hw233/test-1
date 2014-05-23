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
#include "Fighter.h"
#include "Common/Itoa.h"
#include "GObjectDBExecHelper.h"
#include "Server/SysMsg.h"

namespace GObject
{
        
    CollectCard::CollectCard(Player* player)
    {
        m_owner = player;
        VecEquipSlot.resize(5);
        //memset(&_cnt, 0, sizeof(_cnt)); 
        MapCntBlue.clear();
        MapCntPurple.clear();
        MapCntOrg.clear();
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

    bool SuitCardInfo::checkExistSetBit(UInt16 cid,UInt8 color)
    {
        if(id != 20 && id != 30)
        {
            UInt8 card_index = cid % 10;
            if(GET_BIT(suit_mark,(card_index-1)))
                return false;  
            
            suit_mark = SET_BIT(suit_mark,(card_index-1)); 

            if(color == 5)
                collect_degree += 20;
            else if(color == 4 )
                collect_degree += 15;
            else if(color == 3 )
                collect_degree += 10;
            else if(color == 2)
                collect_degree += 5;
            

        }
        else if(id == 30)
        {
            UInt8 card_index = cid % 10;
            if(GET_BIT(suit_mark,(card_index-1)))
                return false;  
            
            suit_mark = SET_BIT(suit_mark,(card_index-1)); 

            if(color == 5)
                collect_degree += 35;
            else if(color == 4 )
                collect_degree += 20;
            else if(color == 3 )
                collect_degree += 10;
            else if(color == 2)
                collect_degree += 5;

        }
        else
        {
            UInt8 card_index = cid % 200;
            if(GET_BIT(spe_mark,card_index))
                return false;  
            
            spe_mark = SET_BIT(spe_mark,card_index); 
            return true;
        }

        
        return false;
    }

    bool SuitCardInfo::checkActive(UInt8 active_set)
    {
        switch(active_set)
        {
            case 1:
                if(collect_degree >= 30)
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
            st << static_cast<UInt16>(0);
            UInt16 size1 = 0;
            for(MSlot::iterator i = MapFreeCardSlot.begin(); i != MapFreeCardSlot.end(); i++)
            {
                if(i->second->checkInfo() == false)  
                    continue;
                st << i->second->id << i->second->cid << i->second->level << i->second->exp << i->second->pos;
                ++size1;
            }       
            
            if (size1)
                st.data<UInt16>(off1) = size1;
        
            size_t off2 = st.size();//套牌个数
            st << static_cast<UInt16>(0);
            UInt16 size2 = 0;
            for(MStamp::iterator i = MapCardStamp.begin(); i != MapCardStamp.end(); i++)
            {
                if(!ReturnSuitInfo(st,i->second->id,false,true))
                    continue;
                ++size2;
            }       
            
            if (size2)
                st.data<UInt16>(off2) = size2;
    
        } 

        st << Stream::eos; 
        m_owner->send(st);

        return;
    }

    bool CollectCard::ReturnSuitInfo(Stream& st,UInt16 suit_lvl ,bool isSpe ,bool isRet)
    {
        if(MapCardStamp.find(suit_lvl) == MapCardStamp.end())
            return false;
        MStamp::iterator it = MapCardStamp.find(suit_lvl);
    
        st << suit_lvl;
        if(suit_lvl != 20)
            st << it->second->suit_mark;
        else
            st << it->second->spe_mark;
        
        if(suit_lvl != 20)
        {
            st << it->second->active;
        }

        //if(suit_lvl >= 40 && suit_lvl <= 130)
        if(suit_lvl != 20)
        {
            st << MapCntBlue[suit_lvl];
            st << MapCntPurple[suit_lvl];
            st << MapCntOrg[suit_lvl];
        }
        if(!isRet)
        {
            if(isSpe)
                st << static_cast<UInt8>(1);
            else
                st << static_cast<UInt8>(0);
        }
            
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
            if((*it)->type == 1)
            {
                const GData::SkillBase* s = NULL;
                std::vector<const GData::SkillBase*> vt_skills;
                s = GData::skillManager[(*it)->skill_id];
                if (s)
                    vt_skills.push_back(s);

                if (vt_skills.size())
                    m_owner->getMainFighter()->delSkillsFromCT(vt_skills, false);
            }
            *it = NULL; 
            
            RebuildCardAttr();
            
            //VecEquipSlot.erase(VecEquipSlot.begin() + pos - 1);//
        }else if(pos <= 4)
        {
            /*//装备卡牌槽的御剑等级限制
            if(pos == 3)
            {
                if(m_owner->getVipLevel() < 1)
                    return;
            }
            if(pos == 4)
            {
                if(m_owner->getVipLevel() < 2)
                    return;
            }*/

            if(VecEquipSlot[pos - 1]->checkInfo())
                return ;
            
            if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
                return ;
            CardInfo* tmp = (MapFreeCardSlot.find(id))->second;
            if(tmp->type != 2 && tmp->type != 3 && tmp->type != 5)//卡牌类型一定为装备卡牌和特殊卡牌
                return ;
            if(tmp->type == 3)
            {
                VecSlot::iterator it = VecEquipSlot.begin();
                while(it != VecEquipSlot.end()) 
                {
                    if((*it)->checkInfo())
                    {
                        if((*it)->type == 3)
                            return;
                    }

                    it++;
                }
            }

            tmp->pos = pos; 
            VecEquipSlot[pos - 1] = tmp;
            MapFreeCardSlot.erase(MapFreeCardSlot.find(id)); 
            RebuildCardAttr();
            if(tmp->type == 1)
            {
                std::string skills = "";
                skills += Itoa(tmp->skill_id);
                m_owner->getMainFighter()->setSkills(skills,false);
            }
        }else if(pos == 5)
        {
            if(VecEquipSlot[pos - 1]->checkInfo())
                return ;
            
            if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
                return ;
            CardInfo* tmp = (MapFreeCardSlot.find(id))->second;
            if(tmp->type != 1 && tmp->type != 3 && tmp->type != 5)//卡牌类型一定为人物卡牌和特殊卡牌
                return ;
            if(tmp->type == 3)//特殊卡牌只能装备一张
            {
                VecSlot::iterator it = VecEquipSlot.begin();
                while(it != VecEquipSlot.end()) 
                {
                    if((*it)->checkInfo())
                    {
                        if((*it)->type == 3)
                            return;
                    }

                    it++;
                }
            }
            if(tmp->type == 5)//活动卡牌只能装备一张
            {
                VecSlot::iterator it = VecEquipSlot.begin();
                while(it != VecEquipSlot.end()) 
                {
                    if((*it)->checkInfo())
                    {
                        if((*it)->type == 5)
                            return;
                    }

                    it++;
                }
            }            
            tmp->pos = pos; 
            VecEquipSlot[pos - 1] = tmp; 
            MapFreeCardSlot.erase(MapFreeCardSlot.find(id)); 
           
            if(tmp->type == 1)
            {
                std::string skills = "";
                skills += Itoa(tmp->skill_id);
                m_owner->getMainFighter()->setSkills(skills,false);
            }
            RebuildCardAttr();

        }
        //TODO DB
        DB4().PushUpdateData("UPDATE `card` SET `pos` = '%u' WHERE `playerId` = %" I64_FMT "u and id = '%u'",pos,m_owner->getId(),id);
        
        st << static_cast<UInt8>(pos) << id ;
        st << Stream::eos; 
        m_owner->send(st);
        RebuildCardAttr(); 

        return ;
    }
    
    void CollectCard::ExchangeCard(UInt8 flag/* 0 - 手动 1- 自动*/,UInt16 level ,UInt8 color ,UInt16 count)//兑换卡牌
    {
        UInt32 iid = 0;
        if(flag == 0 )
            count = 1;
        UInt16 mCount = 0;
        if(level != 30)
        {
            iid = 9457;//天界牌
            mCount = m_owner->GetPackage()->GetItemAnyNum(iid) ;
        }
        else
        {
            if(!World::get61CardActivity())
                return;
            iid = 9895;//童心令
            mCount = m_owner->GetPackage()->GetItemAnyNum(iid) ;
        }
        if(mCount < count) //天界牌,童心令不足
            return ;   

        ItemBase * item = m_owner->GetPackage()->FindItem(iid, true);
        if (!item)
            item =m_owner->GetPackage()->FindItem(iid, false);
        if(item ==NULL)
            return ;

        if((level < 30 || level > 130) && level != 400)
           return ;
        static UInt8 PCardChance[]= {5,5,4,3,3,4,2,2};
        static UInt8 PCardChance61[]= {2,3,4,5};
        std::vector<UInt16> getCards ;
        UInt16 index = 0;
        while(index < count)
        {
            UInt8 CNum = 0; 
            if(level != 30)
                CNum = GameAction()->GetCardByChance(m_owner,MapCntBlue[level],MapCntPurple[level],MapCntOrg[level]);    
            else
                CNum = GameAction()->GetCardByChance61(m_owner,MapCntBlue[level],MapCntPurple[level],MapCntOrg[level]);    

            if(CNum > 8 || CNum == 0)
                break;
            getCards.push_back( (static_cast<UInt16>(level)) * 10 + CNum);
            MapCntBlue[level] ++ ;
            MapCntPurple[level] ++ ;
            MapCntOrg[level] ++ ;
            if(level != 30)
            {
                if(PCardChance[CNum-1] > 2)
                {
                    if(PCardChance[CNum-1] == 3)
                        MapCntBlue[level] = static_cast<UInt16>(0); 
                    else if(PCardChance[CNum-1] == 4)
                        MapCntPurple[level] = static_cast<UInt16>(0); 
                    else if(PCardChance[CNum-1] == 5)
                        MapCntOrg[level] = static_cast<UInt16>(0); 
                    else
                        break;

                    char str[32] = {0};
                    sprintf(str, "F_140506_%d",PCardChance[CNum-1]-2);
                    m_owner->udpLog("kapaixitong", str, "", "", "", "", "act");
                }
                if(PCardChance[CNum - 1] == color)
                    break;
            }
            else
            {
                if(PCardChance61[CNum-1] > 2)
                {
                    if(PCardChance61[CNum-1] == 3)
                        MapCntBlue[level] = static_cast<UInt16>(0); 
                    else if(PCardChance61[CNum-1] == 4)
                        MapCntPurple[level] = static_cast<UInt16>(0); 
                    else if(PCardChance61[CNum-1] == 5)
                        MapCntOrg[level] = static_cast<UInt16>(0); 
                    else
                        break;

                    char str[32] = {0};
                    sprintf(str, "F_140506_%d",PCardChance61[CNum-1]-2);
                    m_owner->udpLog("kapaixitong", str, "", "", "", "", "act");
                    m_owner->udpLog("kapaixitong", "F_140506_17", "", "", "", "", "act");
                }
                if(PCardChance61[CNum - 1] == color)
                    break;
            }
            ++index;
        }

        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(3);
        size_t offset = st.size();
        UInt32 cnt = 0;
        st << cnt;
        //st << static_cast<UInt32>(getCards.size());
        for(UInt8 i = 0; i < getCards.size();++i)
        {
           CardInfo * ci = AddCard(getCards[i] ); 
           if(ci == NULL)
               continue;
           st << static_cast<UInt16>(getCards[i]);
           st << static_cast<UInt32>(ci->id);
           ++cnt;
        }
        st.data<UInt32>(offset)=cnt;
        st << Stream::eos;
        m_owner->send(st);


        m_owner->GetPackage()->DelItemAny(iid, index );
        m_owner->GetPackage()->AddItemHistoriesLog(iid ,index);

        updateCollectCnt(level);

        Stream st1(REP::COLLECTCARD);  
        st1 << static_cast<UInt8>(2) ;//0x02套牌信息
        ReturnSuitInfo(st1,level);
        st1 << Stream::eos; 
        m_owner->send(st1);
        return;
    }
    
    void CollectCard::ActiveCardSet(UInt16 suit_lvl,UInt8 active_set)//激活卡组
    {
        if(MapCardStamp.find(suit_lvl) == MapCardStamp.end())
            return ;

        SuitCardInfo* sci = MapCardStamp.find(suit_lvl)->second;
        if(sci->active >= active_set)
            return;
        else
        {
            UInt8 org_active = sci->active;
            if(sci->checkActive(active_set))
                sci->active = active_set;
            if(suit_lvl == 30)
            {
                if(sci->active <= org_active)
                    return;
                if(sci->active != 1 && sci->active != 2 && sci->active != 3)
                    return;
                UInt8 cnt = sci->active - org_active; 
                while(cnt != 0)
                {
                    if(!GameAction()->RunActCardAward(m_owner,org_active + cnt))
                    {
                        sci->active = org_active;
                        return;
                    }
                    cnt --;
                }
            }
            //TODO DB
            DB4().PushUpdateData("UPDATE `cardsuit` SET `active` = '%u' WHERE `playerId` = %" I64_FMT "u and `id` = '%u'",sci->active,m_owner->getId(),sci->id);

        }

        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(2) ;//0x02套牌信息
        ReturnSuitInfo(st,suit_lvl);
        st << Stream::eos; 
        m_owner->send(st);
        RebuildCardAttr();
        return;
    } 

    void CollectCard::UpGradeCard(UInt32 id,std::vector<UInt32>& vecid)//卡片升级
    {
        //UInt16 add_exp = 0;
        std::vector<UInt32>::iterator it = vecid.begin();
        
        if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
            return;
        CardInfo* upcard = MapFreeCardSlot.find(id)->second;
        if(upcard == NULL)
            return;
        GData::CardInitInfo* cii = GData::csys.getCardInitInfo(upcard->cid);
        if(cii->type != 1)
        {
            if(upcard->level > cii->lvLimit)
               return;
        }
        else
        {
            if(upcard->level > m_owner->getMainFighter()->getLevel())
                return;
        }

        while(it != vecid.end())
        {
            if(*it == id)
                break;
            if(MapFreeCardSlot.find(*it) == MapFreeCardSlot.end())
                break;
            UInt16 cid = MapFreeCardSlot.find(*it)->second->cid;
            GData::CardInitInfo* ciitmp = GData::csys.getCardInitInfo(cid);
            if(ciitmp == NULL)
                break;
            /*if(ciitmp->type == 3)//特殊卡牌不能分解
                break;*/
            upcard->exp += ciitmp->initExp + MapFreeCardSlot.find(*it)->second->exp;
            while(GData::csys.checkUpgrade(upcard))
            {
                if(upcard->level == 80 && upcard->color >= 3)
                    SYSMSG_BROADCASTV(954,m_owner->getCountry(), m_owner->getName().c_str(),3,cii->name.c_str());

                if(cii->type != 1)
                {
                    if(upcard->level >= cii->lvLimit)
                       break;
                }
                else
                {
                    if(upcard->level >= m_owner->getMainFighter()->getLevel())
                        break;
                }
                switch(upcard->level)
                {
                    case 10:
                        m_owner->udpLog("kapaixitong", "F_140506_4", "", "", "", "", "act");
                        break;
                    case 20:
                        m_owner->udpLog("kapaixitong", "F_140506_5", "", "", "", "", "act");
                        break;
                    case 30:
                        m_owner->udpLog("kapaixitong", "F_140506_6", "", "", "", "", "act");
                        break;
                    case 40:
                        m_owner->udpLog("kapaixitong", "F_140506_7", "", "", "", "", "act");
                        break;
                    case 50:
                        m_owner->udpLog("kapaixitong", "F_140506_8", "", "", "", "", "act");
                        break;
                    case 60:
                        m_owner->udpLog("kapaixitong", "F_140506_9", "", "", "", "", "act");
                        break;
                    case 70:
                        m_owner->udpLog("kapaixitong", "F_140506_10", "", "", "", "", "act");
                        break;
                    case 80:
                        m_owner->udpLog("kapaixitong", "F_140506_11", "", "", "", "", "act");
                        break;
                    case 90:
                        m_owner->udpLog("kapaixitong", "F_140506_12", "", "", "", "", "act");
                        break;
                    case 100:
                        m_owner->udpLog("kapaixitong", "F_140506_13", "", "", "", "", "act");
                        break;
                    case 110:
                        m_owner->udpLog("kapaixitong", "F_140506_14", "", "", "", "", "act");
                        break;
                    case 120:
                        m_owner->udpLog("kapaixitong", "F_140506_15", "", "", "", "", "act");
                        break;
                    case 130:
                        m_owner->udpLog("kapaixitong", "F_140506_16", "", "", "", "", "act");
                        break;
                    default:
                        break;
                }
                
            }
            //TODO 找到卡牌经验值
            DB4().PushUpdateData("UPDATE `card` SET `exp` = '%u',`level` = '%u' WHERE `playerId` = %" I64_FMT "u and id = '%u'",upcard->exp,upcard->level,m_owner->getId(),id);
            
            DelAddCard(*it);

            if(cii->type != 1)
            {
                if(upcard->level >= cii->lvLimit)
                   break;
            }
            else
            {
                if(upcard->level >= m_owner->getMainFighter()->getLevel())
                    break;
            }
            it++;
        }

        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(4) ;//0x02套牌信息
        st << upcard->id << upcard->cid << upcard->level << upcard->exp << upcard->pos;

        size_t off1 = st.size();//空闲卡牌数
        st << static_cast<UInt16>(0);
        UInt16 size1 = 0;
        for(MSlot::iterator i = MapFreeCardSlot.begin(); i != MapFreeCardSlot.end(); i++)
        {
            if(i->second->checkInfo() == false)  
                continue;
            st << i->second->id << i->second->cid << i->second->level << i->second->exp << i->second->pos;
            ++size1;
        }       
        
        if (size1)
            st.data<UInt16>(off1) = size1;

        st << Stream::eos; 
        m_owner->send(st);

        return;
    }

    
    void CollectCard::AddCardAttr(GData::AttrExtra& ae)//计算增加的属性
    {
        VecSlot::iterator it = VecEquipSlot.begin();
        if(VecEquipSlot.size() > 5)
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
                        GData::csys.AddSuitCardAttr(ae,attr_id,3);
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
        /*//等级保护
        if(m_owner->getMainFighter()->getLevel() < citmp->lvLimit) 
            return NULL;*/
        UInt16 skillId = 0;//计算技能id
        if(citmp->skillId != 0)
            skillId = citmp->skillId * 100 + 1;

        CardInfo* ci = new CardInfo(IDGenerator::gCardOidGenerator.ID(),cid,citmp->type,static_cast<UInt8>(1),static_cast<UInt16>(0),skillId,static_cast<UInt8>(0), citmp->color,static_cast<UInt16>(32001));
        if(!ci->checkInfo())
        {
            delete ci;
            return NULL;
        }        
        MapFreeCardSlot.insert(std::make_pair(ci->id,ci)); 
        DB4().PushUpdateData("REPLACE INTO `card`(`playerId`, `id`, `cid`,`level`,`exp`,`pos`) VALUES(%" I64_FMT "u, %u,%u,%u,%u,%u)", m_owner->getId(), ci->id,ci->cid,ci->level,ci->exp,ci->pos);
        
        if(MapCardStamp.find(cid/100*10) == MapCardStamp.end())
        {
            SuitCardInfo* si = new SuitCardInfo(cid/100*10);
            MapCardStamp.insert(std::make_pair(cid/100*10,si));
            //TODO DB
		    DB4().PushUpdateData("REPLACE INTO `cardsuit`(`playerId`, `id`, `suit_mark`,`active`,`spe_mark`,`collect_degree`) VALUES(%" I64_FMT "u, %u,0,0,0,0)", m_owner->getId(), si->id);
        }
        SuitCardInfo* tmp = MapCardStamp.find(cid/100*10)->second;
        UInt8 org_degree = 0;//初始收集度
        if(tmp->id == 30)//童趣套组
            org_degree = tmp->collect_degree;
        if(tmp->checkExistSetBit(ci->cid,ci->color))
            RebuildCardAttr();
        //TODO DB
        DB4().PushUpdateData("UPDATE `cardsuit` SET `suit_mark` = '%u', `collect_degree` = '%u',`spe_mark` = '%u' WHERE `playerId` = %" I64_FMT "u and `id` = '%u'",tmp->suit_mark,tmp->collect_degree,tmp->spe_mark,m_owner->getId(),tmp->id);
        
        if(ci->color == 5)
            SYSMSG_BROADCASTV(953, m_owner->getCountry(),m_owner->getName().c_str(),3,citmp->name.c_str());

        if(tmp->id == 30)//童趣套组
        {
            if(tmp->collect_degree >= 30 && org_degree != 100 && tmp->collect_degree > org_degree)
            {
                if(org_degree < 30 && tmp->collect_degree >= 30) 
                    m_owner->udpLog("kapaixitong", "F_140506_18", "", "", "", "", "act");
                if(org_degree < 50 && tmp->collect_degree >= 50) 
                    m_owner->udpLog("kapaixitong", "F_140506_19", "", "", "", "", "act");
                if(org_degree < 100 && tmp->collect_degree >= 100) 
                    m_owner->udpLog("kapaixitong", "F_140506_20", "", "", "", "", "act");
            }
        }

        return ci;
    }
    void CollectCard::loadCollectCnt(UInt16 level ,UInt16 cnt1 ,UInt16 cnt2 ,UInt16 cnt3)
    {
        /*if(level < 40 || level > 130)
            return ;*/
        MapCntBlue[level] = cnt1;
        MapCntPurple[level] = cnt2;
        MapCntOrg[level] = cnt3;
    }
    void CollectCard::updateCollectCnt(UInt16 level)
    {
        /*if(level < 40 || level  > 130 )
            return ;*/
        if(!m_owner)
            return ;
        DB1().PushUpdateData("REPLACE INTO `collect_cnt` (`playerId`, `lev`, `bluecnt`,`purlecnt`,`orangecnt`) VALUES (%" I64_FMT "u, %u , %u , %u ,%u )", m_owner->getId(), level , MapCntBlue[level],MapCntPurple[level], MapCntOrg[level]);
    }
        

    bool CollectCard::DelAddCard(UInt32 id)
    {
        if(MapFreeCardSlot.find(id) == MapFreeCardSlot.end())
            return false;
        MSlot::iterator it = MapFreeCardSlot.find(id);   
        delete it->second;
        MapFreeCardSlot.erase(it);

        //TODO DB
        DB4().PushUpdateData("DELETE FROM `card` WHERE `playerId` = %" I64_FMT "u and `id` = '%u'", m_owner->getId(),id);
        return true;
    }

    void CollectCard::ExchangeSpeCard(UInt16 itemid,UInt32 flowid)
    {
        if(flowid != 0)
        {
            std::map<UInt32, Fighter *>& fighters = m_owner->getFighterMap();
            bool find = false;
            for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
            {
                if(it->second->findfashion(flowid) != NULL)
                {
                    find = true;
                    break;
                }
            }

            if(!find)
            {
                ItemBase * item = m_owner->GetPackage()->GetEquip(flowid);
                if(item == NULL)
                    return;
                UInt32 typeId = item->GetTypeId();
                if(itemid != typeId)
                    return;
                item->SetBindStatus(true);
                m_owner->GetPackage()->SendPackageItemInfor(); 
            }
        }
        else
            return;

        UInt16 cid = GameAction()->getSpeCard(itemid);        
        if(cid == 0)
            return;
        
        std::map<UInt16,SuitCardInfo*>::iterator it = MapCardStamp.find(cid/100*10);
        if(it != MapCardStamp.end())
        {
            SuitCardInfo* tmp = it->second;
            if(!tmp)
                return;
            if(GET_BIT(tmp->spe_mark,(cid % 200)))
                return;
        }
        CardInfo* citmp = AddCard(cid); 
        if(!citmp)
            return;
        Stream st(REP::COLLECTCARD);  
        st << static_cast<UInt8>(2) ;//0x02套牌信息
        ReturnSuitInfo(st,20,true);
        st << citmp->id << citmp->cid << citmp->level << citmp->exp << citmp->pos;
        st << Stream::eos; 
        m_owner->send(st);
        return;
    }

    void CollectCard::InsertCard(DBCard dbc)
    {
        GData::CardInitInfo* cii = GData::csys.getCardInitInfo(dbc.cid);
        if(cii == NULL)
            return;
        GData::CardUpgradeTable* cut = GData::csys.getCardUpgradeTable(dbc.level);
        if(cut == NULL)
            return;

        if(dbc.pos == 0)
        {
            UInt16 skillId = 0;
            if(cii->skillId != 0) 
                skillId = cii->skillId*100 + cut->skillLevel;                      

            CardInfo* ci = new CardInfo(dbc.id,dbc.cid,cii->type,dbc.level,dbc.exp,skillId,dbc.pos, cii->color,cut->attrIndex);
            MapFreeCardSlot.insert(std::make_pair(ci->id,ci)); 
        }else if(dbc.pos < 5)
        {
            CardInfo* ci = new CardInfo(dbc.id,dbc.cid,cii->type,dbc.level,dbc.exp,static_cast<UInt16>(0),dbc.pos, cii->color,cut->attrIndex);
            VecEquipSlot[dbc.pos - 1] = ci; 

        }else if(dbc.pos == 5)
        {
            UInt16 skillId = 0;
            if(cii->skillId != 0) 
                skillId = cii->skillId*100 + cut->skillLevel;                      
            CardInfo* ci = new CardInfo(dbc.id,dbc.cid,cii->type,dbc.level,dbc.exp,skillId,dbc.pos, cii->color,cut->attrIndex);
            VecEquipSlot[dbc.pos - 1] = ci; 
            if(cii->type == 1)
            {
                std::string skills = "";
                skills += Itoa(skillId);
                m_owner->getMainFighter()->setSkills(skills,false);
            }
        }
        
        return;
    }

    void CollectCard::InsertCardSuit(DBCardSuit dbcs)
    {
        SuitCardInfo* si = new SuitCardInfo(dbcs.id,dbcs.suit_mark,dbcs.active,dbcs.spe_mark,dbcs.collect_degree);
        MapCardStamp.insert(std::make_pair(dbcs.id,si));

        return;
    }

    void CollectCard::GMAddExp(UInt32 exp)
    {
        CardInfo* upcard = MapFreeCardSlot.begin()->second;
        if(upcard == NULL)
            return;
        upcard->exp += exp; 

        return;
    }

    void CollectCard::Add61Card(UInt16 cid)
    {
        if(MapCardStamp.find(cid/100*10) == MapCardStamp.end())
        {
            UInt8 card_index1 = cid % 10;
            if(card_index1 < 5)
                return;
            CardInfo* citmp = AddCard(cid);
            if(!citmp)
                return;
            Stream st(REP::COLLECTCARD);  
            st << static_cast<UInt8>(2) ;//0x02套牌信息
            ReturnSuitInfo(st,30,true);
            st << citmp->id << citmp->cid << citmp->level << citmp->exp << citmp->pos;
            st << Stream::eos; 
            m_owner->send(st);
            return;
        }
        
        SuitCardInfo* tmp = MapCardStamp.find(cid/100*10)->second;
        if(tmp == NULL)
            return;
        if(tmp->id != 30)
            return;

        UInt8 card_index = cid % 10;
        if(card_index < 5)
            return;
        if(!GET_BIT(tmp->suit_mark,(card_index-1)))
        {
            CardInfo* citmp = AddCard(cid);
            if(!citmp)
                return;
            Stream st(REP::COLLECTCARD);  
            st << static_cast<UInt8>(2) ;//0x02套牌信息
            ReturnSuitInfo(st,30,true);
            st << citmp->id << citmp->cid << citmp->level << citmp->exp << citmp->pos;
            st << Stream::eos; 
            m_owner->send(st);
        }
        if(GET_BIT(tmp->suit_mark,4) && GET_BIT(tmp->suit_mark,5) && GET_BIT(tmp->suit_mark,6) && GET_BIT(tmp->suit_mark,7))
            m_owner->udpLog("kapaixitong", "F_140506_21", "", "", "", "", "act");
        
        return;
    }


}

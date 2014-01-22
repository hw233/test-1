#include "Married.h"
#include "Marry.h"
#include "Player.h"
#include "Common/TimeUtil.h"
#include "MsgID.h"
#include "Common/URandom.h"
#include "Server/SysMsg.h"
#include "GData/CoupleUpgrade.h"

namespace GObject
{
    MarriedMgr gMarriedMgr;
    MarriedMgr::MarriedMgr()
    {
    }

    MarriedMgr::~MarriedMgr()
    {
    }
   
    UInt8 MarriedMgr::PreCheckingStatus(Player* player)
    {
        if(player->GetVar(VAR_MARRY_STATUS) != 5 && player->GetVar(VAR_MARRY_STATUS) != 6)
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        
        Player* obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
        if(!obj_player)
        {
            player->sendMsgCode(0, 6011);
            return 1;
        }

        if(obj_player->GetVar(VAR_MARRY_STATUS) != 5 && obj_player->GetVar(VAR_MARRY_STATUS) != 6)
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }

        return 0;
    }



    void MarriedMgr::ProcessOnlineAward(Player* player,UInt8 flag /*0-上线 1-下线 */)
    {
        Mutex::ScopedLock lk(_mutex);
        UInt32 now = TimeUtil::Now();
        //夫妻同时在线状态写在男方身上
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        
        if(PreCheckingStatus(player) != 0)
            return;
        obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
        
        if(!player->getMainFighter()->getSex())//男的
        {
            man_player = player;
            woman_player = obj_player;
        }
        else
        {
            man_player = obj_player;
            woman_player = player;
        }
        
        //CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(man_player->GetVar(VAR_COUPLE_ONLINE_TIME) >= 60 * 60 * 8) 
            return;
        
        if(flag == 0)
        {
            if(obj_player->isOnline())
            {
                if(man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME) != 0)
                    return;
                else
                    man_player->SetVar(VAR_COUPLE_ONLINE_START_TIME,now);
            }   
        }
        else
        {
            if(obj_player->isOnline())
            {
                if(man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME) > now)
                    return ;
                UInt32 tmp_time = 0;
                if(man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME) != 0)
                    tmp_time = man_player->GetVar(VAR_COUPLE_ONLINE_TIME) + now - man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME); 
                else
                    tmp_time = man_player->GetVar(VAR_COUPLE_ONLINE_TIME);
                man_player->SetVar(VAR_COUPLE_ONLINE_TIME,tmp_time);
                man_player->SetVar(VAR_COUPLE_ONLINE_START_TIME,0);//结算完成
                if(man_player->GetVar(VAR_COUPLE_ONLINE_TIME) > 60*60*8)
                    man_player->SetVar(VAR_COUPLE_ONLINE_TIME, 60*60*8);
            }
        }
        ReturnFirstStatus(man_player);
        ReturnFirstStatus(woman_player);
    } 
    
    void MarriedMgr::ReturnFirstStatus(Player* player)//返回家园初始状态
    {
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        
        if(PreCheckingStatus(player) != 0)
            return;
        obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
       
        if(!player->getMainFighter()->getSex())//男的
        {
            man_player = player;
            woman_player = obj_player;
        }
        else
        {
            man_player = obj_player;
            woman_player = player;
        }
        
        //CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        
        Stream st(REP::MARRIEDMGR);
        st << static_cast<UInt8>(2) << man_player->getName() << man_player->isOnline() << woman_player->getName() << woman_player->isOnline() ;
        if(man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME) == 0 || !man_player->isOnline() || !woman_player->isOnline()) 
            st << man_player->GetVar(VAR_COUPLE_ONLINE_TIME) ;
        else
        {
            UInt32 ret_time = man_player->GetVar(VAR_COUPLE_ONLINE_TIME) + TimeUtil::Now() - man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME); 
            st << static_cast<UInt32>(ret_time); 
        }

        st << static_cast<UInt8>(man_player->GetVar(VAR_COUPLE_ONLINE_FISH)) << static_cast<UInt8>(man_player->GetVar(VAR_COUPLE_ONLINE_AWARD)) << Stream::eos;
        
        player->send(st);
        return;
    }
    
    void MarriedMgr::ReturnCouplePet(Player* player)
    {
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        
        if(PreCheckingStatus(player) != 0)
            return;
        obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
        
        if(!player->getMainFighter()->getSex())//男的
        {
            man_player = player;
            woman_player = obj_player;
        }
        else
        {
            man_player = obj_player;
            woman_player = player;
        }
        
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;

        Stream st(REP::MARRIEDMGR);
        st << static_cast<UInt8>(4) << it->second->petName << it->second->level << it->second->levelExp << it->second->friendliness << it->second->eLove << static_cast<UInt8>(man_player->GetVar(VAR_COUPLE_ONLINE_RENAME))  << Stream::eos;
        player->send(st);
        return;
    }

    void MarriedMgr::GetOnlineAward(Player* player)
    {
        Mutex::ScopedLock lk(_mutex);
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        UInt32 now = TimeUtil::Now();
        
        if(PreCheckingStatus(player) != 0)
            return;
        obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
        
        if(!player->getMainFighter()->getSex())//男的
        {
            man_player = player;
            woman_player = obj_player;
        }
        else
        {
            man_player = obj_player;
            woman_player = player;
        }
        
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
        if(man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME) != 0)
        {
            UInt32 tmp_time = man_player->GetVar(VAR_COUPLE_ONLINE_TIME) + now - man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME);//计算同时在线时间
            man_player->SetVar(VAR_COUPLE_ONLINE_TIME,tmp_time);
        }
        UInt8 num = 0;
        num = man_player->GetVar(VAR_COUPLE_ONLINE_TIME) / (60*60); 
        if(num < 1)
            return; 
        if(num > 2 && num <= 7)
            num = 3;
        if(num >= 8)
            num = 4;

        for(UInt8 i = 0;i < 4;i++)
        {
            if(num <= i)
                break;
            if(!GET_BIT(man_player->GetVar(VAR_COUPLE_ONLINE_AWARD),i))
            {
                UInt32 tmp = man_player->GetVar(VAR_COUPLE_ONLINE_AWARD);
                tmp = SET_BIT(tmp,i);
                man_player->SetVar(VAR_COUPLE_ONLINE_AWARD,tmp);      
                man_player->SetVar(VAR_COUPLE_ONLINE_START_TIME,now);
                ChangPetAttr(man_player,woman_player,static_cast<ECoupleAward>(i+1));
                if(i == 0)
                    player->udpLog("fuqijiayuan", "F_140116_7", "", "", "", "", "act");
                   else if(i == 1)
                        player->udpLog("fuqijiayuan", "F_140116_8", "", "", "", "", "act");
                            else if(i == 2)
                                player->udpLog("fuqijiayuan", "F_140116_9", "", "", "", "", "act");
                                    else if(i == 3)
                                        player->udpLog("fuqijiayuan", "F_140116_10", "", "", "", "", "act");
            }
        }
            
        ReturnFirstStatus(player);
    }

    void MarriedMgr::ModifyPetName(Player* player,std::string name)
    {
        Mutex::ScopedLock lk(_mutex);
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        if(PreCheckingStatus(player) != 0)
            return;
        obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
        
        if(!player->getMainFighter()->getSex())//男的
        {
            man_player = player;
            woman_player = obj_player;
        }
        else
        {
            man_player = obj_player;
            woman_player = player;
        }
        
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
        if(man_player->GetVar(VAR_COUPLE_ONLINE_RENAME) != 0)
            useMoney(player,2,50,1); 
        UInt8 tmp_count = man_player->GetVar(VAR_COUPLE_ONLINE_RENAME) + 1;
        man_player->SetVar(VAR_COUPLE_ONLINE_RENAME,tmp_count);
        
        it->second->petName = name;
        
        DB4().PushUpdateData("UPDATE `married_couple` SET `pet_name` = '%s' WHERE `jh_time` = %u",name.c_str(),player->GetMarriageInfo()->yuyueTime);

        ReturnCouplePet(man_player);
        ReturnCouplePet(woman_player);
        player->udpLog("fuqijiayuan", "F_140116_4", "", "", "", "", "act");
    }

    void MarriedMgr::ModifyeLove(Player* player,UInt8 eLove)
    {
        Mutex::ScopedLock lk(_mutex);
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        if(PreCheckingStatus(player) != 0)
            return;
        obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
        
        if(!player->getMainFighter()->getSex())//男的
        {
            man_player = player;
            woman_player = obj_player;
        }
        else
        {
            man_player = obj_player;
            woman_player = player;
        }
       
        if(eLove != 2 && eLove != 3)
            return;
        if(static_cast<UInt8>(man_player->GetMarriageInfo()->eLove) >= eLove)
            return;
        //UInt8 consumeType = 3;
        UInt16 consumeNum = 0; 
        UInt16 old_consumeNum = 0; 
        UInt16 new_consumeNum = 0; 
        if(eLove == 2)
        {
            new_consumeNum = YUPEI;
            player->udpLog("fuqijiayuan", "F_140116_5", "", "", "", "", "act");
        }
        else
        {
            new_consumeNum = JINZAN;
            player->udpLog("fuqijiayuan", "F_140116_6", "", "", "", "", "act");
        }
        
        if(man_player->GetMarriageInfo()->eLove == LOVE_TONGXINJIE)
            old_consumeNum = TONGXINJIE;
        else if(man_player->GetMarriageInfo()->eLove == LOVE_YUPEI)
            old_consumeNum = YUPEI;
        else
            old_consumeNum = JINZAN;
            
        consumeNum = new_consumeNum - old_consumeNum; 

        if(useMoney(player,static_cast<UInt8>(2),consumeNum,static_cast<UInt8>(2)) != 0)
            return;

        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
        it->second->eLove = eLove;
        man_player->GetMarriageInfo()->eLove = static_cast<ELoveToken>(eLove);
        woman_player->GetMarriageInfo()->eLove = static_cast<ELoveToken>(eLove);
        DB4().PushUpdateData("UPDATE `married_log` SET `lover_item` = %u WHERE `jh_time` = %u", eLove ,player->GetMarriageInfo()->yuyueTime);
        DB4().PushUpdateData("UPDATE `married_couple` SET `lover_item` = %u WHERE `jh_time` = %u", eLove ,player->GetMarriageInfo()->yuyueTime);
        gMarryMgr.SetDirty(man_player,woman_player);//改变fighter基础属性

        ReturnCouplePet(man_player);
        ReturnCouplePet(woman_player);
    }

    UInt8 MarriedMgr::Fishing(Player* player,UInt8 consumeType/*消费类型*/,UInt8 count/*次数*/)
    {
        Mutex::ScopedLock lk(_mutex);
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        if(PreCheckingStatus(player) != 0)
            return 1;
        obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
        
        if(!player->getMainFighter()->getSex())//男的
        {
            man_player = player;
            woman_player = obj_player;
        }
        else
        {
            man_player = obj_player;
            woman_player = player;
        }
        
        //CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(man_player->GetVar(VAR_COUPLE_ONLINE_FISH) + count > 10)
            return 1;
        
        UInt8 i = 0;
        UInt16 consumeNum = 0;
        switch(consumeType)
        {
            case 0://银币
               consumeNum = 1000; 
               break;
            case 1://礼券
               consumeNum = 10; 
               break;
            case 2://仙石
               consumeNum = 10;
               break;
            default:
               return 1;
        }

        UInt8 white_num = 0;
        UInt8 qixing_num = 0;
        UInt8 jinjin_num = 0;

        while(man_player->GetVar(VAR_COUPLE_ONLINE_FISH) + i <= 10)
        {
            if(i >= count)
                break;
            if(useMoney(player,consumeType,consumeNum,0) != 0)
                break;
            UInt8 rand = uRand(100)+1;
            switch(consumeType)
            {
                case 0:
                    if(rand <= 5) 
                    {
                        ChangPetAttr(man_player,woman_player,AWARD_JINJINFISH);
                        jinjin_num++;
                    }
                    else if(rand <= 20)
                     {
                         ChangPetAttr(man_player,woman_player,AWARD_QIXINGFISH);
                         qixing_num++;
                     }
                        else 
                        {
                            ChangPetAttr(man_player,woman_player,AWARD_WHITEFISH);
                            white_num++;
                        }
                    player->udpLog("fuqijiayuan", "F_140116_1", "", "", "", "", "act");
                    break;
                case 1:
                    if(rand <= 15)
                    {
                        ChangPetAttr(man_player,woman_player,AWARD_JINJINFISH);
                        jinjin_num++;
                    }
                    else if(rand <= 90)
                    {
                        ChangPetAttr(man_player,woman_player,AWARD_QIXINGFISH);
                        qixing_num++;
                    }
                        else
                        {
                            ChangPetAttr(man_player,woman_player,AWARD_WHITEFISH);
                            white_num++;
                        }
                    player->udpLog("fuqijiayuan", "F_140116_2", "", "", "", "", "act");
                    break;
                case 2:
                    if(rand <= 80)
                    {
                        ChangPetAttr(man_player,woman_player,AWARD_JINJINFISH);
                        jinjin_num++;
                    }
                    else 
                    {
                        ChangPetAttr(man_player,woman_player,AWARD_QIXINGFISH);
                        qixing_num++;
                    }
                    player->udpLog("fuqijiayuan", "F_140116_3", "", "", "", "", "act");
                    break;
                default:
                    return 1;
            }
            i++; 
        }
        
        UInt8 fish_num = man_player->GetVar(VAR_COUPLE_ONLINE_FISH) + i;
        man_player->SetVar(VAR_COUPLE_ONLINE_FISH,fish_num);
        
        Stream st(REP::MARRIEDMGR);
        st << static_cast<UInt8>(7) << static_cast<UInt8>(man_player->GetVar(VAR_COUPLE_ONLINE_FISH)) << white_num << qixing_num << jinjin_num << Stream::eos;
        man_player->send(st);
        woman_player->send(st);
        
        return 0;
    }
    
    UInt8 MarriedMgr::useMoney(Player* player,UInt8 price_type,UInt32 price_num,UInt8 useType)
    {
        ConsumeInfo ci; 
        switch (useType)
        {
            case 0:
                ci.purchaseType = COUPLEFISH; 
                break;
            case 1:
                ci.purchaseType = PETRENAME; 
                break;
            case 2:
                ci.purchaseType = MODIFYPET;
                break;
            case 3:
                ci.purchaseType = COUPLECOPY; 
                break;
            default:
                return 1;
        }
        switch (price_type)
        {
            case 0:
                if(player->getTael() < price_num)
                {
                    player->sendMsgCode(0, 1100);
                    return 2;
                }
                player->useTael(price_num,&ci);
                break;
            case 1:
                if(player->getCoupon() < price_num)
                {
                    player->sendMsgCode(0, 1101);
                    return 2;
                }
                player->useCoupon(price_num,&ci);
                break;
            case 2:
                if(player->getGold() < price_num)
                {
                    player->sendMsgCode(0, 1101);
                    return 2;
                }
                player->useGold(price_num,&ci);
                break;
            default:
                return 1;
        }
        return 0;
    }
   
    void MarriedMgr::ChangPetAttr(Player* man_player,Player* woman_player,ECoupleAward eAward)
    {
        CoupleList::iterator it = m_couple.find(man_player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
        
        switch(eAward)
        {
            case AWARD_WHITEFISH:
                it->second->levelExp += 1;
                rebuildCouplePet(man_player);
                gMarryMgr.SetDirty(man_player,woman_player);
                
                SYSMSG_SENDV(918, man_player);
                SYSMSG_SENDV(921, man_player);
                SYSMSG_SENDV(918, woman_player);
                SYSMSG_SENDV(921, woman_player);
                DB4().PushUpdateData("UPDATE `married_couple` SET `pet_level` = %u, `pet_levelExp` = %u WHERE `jh_time` = %u", it->second->level, it->second->levelExp,man_player->GetMarriageInfo()->yuyueTime);
                break;
            case AWARD_QIXINGFISH:
                it->second->levelExp += 2;
                rebuildCouplePet(man_player);
                gMarryMgr.SetDirty(man_player,woman_player);

                SYSMSG_SENDV(919, man_player);
                SYSMSG_SENDV(922, man_player);
                SYSMSG_SENDV(919, woman_player);
                SYSMSG_SENDV(922, woman_player);
                DB4().PushUpdateData("UPDATE `married_couple` SET `pet_level` = %u, `pet_levelExp` = %u WHERE `jh_time` = %u", it->second->level, it->second->levelExp,man_player->GetMarriageInfo()->yuyueTime);
                break;
            case AWARD_JINJINFISH:
                it->second->levelExp += 3;
                rebuildCouplePet(man_player);
                gMarryMgr.SetDirty(man_player,woman_player);
                               
                SYSMSG_SENDV(920, man_player);
                SYSMSG_SENDV(923, man_player);
                SYSMSG_SENDV(920, woman_player);
                SYSMSG_SENDV(923, woman_player);
                DB4().PushUpdateData("UPDATE `married_couple` SET `pet_level` = %u, `pet_levelExp` = %u WHERE `jh_time` = %u", it->second->level, it->second->levelExp,man_player->GetMarriageInfo()->yuyueTime);
                break;
            case AWARD_MEMEORY:
                it->second->levelExp += 4;
                rebuildCouplePet(man_player);
                gMarryMgr.SetDirty(man_player,woman_player);
                
                SYSMSG_SENDV(919, man_player);
                SYSMSG_SENDV(922, man_player);
                SYSMSG_SENDV(919, man_player);
                SYSMSG_SENDV(922, man_player);
                SYSMSG_SENDV(919, woman_player);
                SYSMSG_SENDV(922, woman_player);
                SYSMSG_SENDV(919, woman_player);
                SYSMSG_SENDV(922, woman_player);
                DB4().PushUpdateData("UPDATE `married_couple` SET `pet_level` = %u, `pet_levelExp` = %u WHERE `jh_time` = %u", it->second->level, it->second->levelExp,man_player->GetMarriageInfo()->yuyueTime);
               /* it->second->friendliness += 5;
                SYSMSG_SENDV(925, man_player);
                SYSMSG_SENDV(924, man_player);
                DB4().PushUpdateData("UPDATE `married_couple` SET `pet_friendliness` = %u WHERE `jh_time` = %u", it->second->friendliness, man_player->GetMarriageInfo()->yuyueTime);*/
                break;
            default:
                break;
        }

        return;
    }
    
    void MarriedMgr::rebuildCouplePet(Player* player)
    {
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
        while((GData::cu).getUpgradeData(it->second->level)->levelUpExp <= it->second->levelExp)
            it->second->level += 1; 

        return;
    }

    void MarriedMgr::addCouplePetAttr(Player* player,GData::AttrExtra& ae)
    {
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
                
        GData::CoupleUpgradeData* cud = (GData::cu).getUpgradeData(it->second->level);
                
        UInt32 friendliness = it->second->friendliness;
        if(friendliness < 250)
            return; 
        if(friendliness >= 250 ) 
            ae.hp += cud->hp;
        if(friendliness >= 375 )
            ae.attack += cud->attak;
        if(friendliness >= 625 )
            ae.magatk += cud->magic_attak;
        if(friendliness >= 1000)
            ae.criticaldmgimmune += cud->df_critical; 
        if(friendliness >= 1500)
            ae.action += cud->action; 

        return;
    }


    void MarriedMgr::LoadMarriedCouple(DBMarriedCouple* dbpn)
    {
        CoupleInfo* ci = new CoupleInfo(dbpn->pet_name,dbpn->lover_item,dbpn->pet_level,dbpn->pet_levelExp,dbpn->pet_friendliness);
        InsertCoupleInfo(dbpn->jh_time,ci);
        return;
    }
    
    void MarriedMgr::InsertCoupleInfo(UInt32 time,CoupleInfo* ci)
    {
        m_couple.insert(std::make_pair(time,ci));
        return;
    }

    void MarriedMgr::AddPetAttr(Player* player,UInt8 type,UInt16 num)
    {
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
        if(type == 0)
            it->second->levelExp += num;
        if(type == 1)
            it->second->friendliness += num;

        return;
    }

    void MarriedMgr::AppendPetData(Player* player,Stream& st)
    {
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
        if(it->second->petName == "")
            it->second->petName = "玉兔";
        st << it->second->petName << it->second->eLove << it->second->level << it->second->levelExp << it->second->friendliness; 

        return;
    }

    void MarriedMgr::eraseCoupleList(Player* player)
    {
        Mutex::ScopedLock lk(_mutex);
        
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
        m_couple.erase(it);  
        return;
    }


}


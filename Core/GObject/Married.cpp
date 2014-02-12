#include "Married.h"
#include "Marry.h"
#include "Player.h"
#include "Package.h"
#include "Common/TimeUtil.h"
#include "MsgID.h"
#include "Common/URandom.h"
#include "Server/SysMsg.h"
#include "GData/CoupleUpgrade.h"
#include "GData/CoupleCopy.h"

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
        if(man_player->isOnline() && woman_player->isOnline() && man_player->GetVar(VAR_COUPLE_ONLINE_START_TIME) == 0)
        {
            if(man_player->getLastOnline() < TimeUtil::SharpDay(0, TimeUtil::Now()) && woman_player->getLastOnline() < TimeUtil::SharpDay(0, TimeUtil::Now()))
                man_player->SetVar(VAR_COUPLE_ONLINE_START_TIME,TimeUtil::SharpDay(0, TimeUtil::Now()));
        }

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
        
        Stream st1(REP::MARRIEDMGR);
        st1 << static_cast<UInt8>(0x11) << static_cast<UInt8>(player->GetVar(VAR_COUPLE_NAME)) << Stream::eos;
        player->send(st1);
        
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
        num = man_player->GetVar(VAR_COUPLE_ONLINE_TIME) / (15*60); 
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
        {
            if(useMoney(player,2,50,1) != 0)
                return;
        }
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
                if(it->second->levelExp >= 63770)
                    it->second->levelExp = 63770;
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
                if(it->second->levelExp >= 63770)
                    it->second->levelExp = 63770;
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
                if(it->second->levelExp >= 63770)
                    it->second->levelExp = 63770;
                rebuildCouplePet(man_player);
                gMarryMgr.SetDirty(man_player,woman_player);
                               
                SYSMSG_SENDV(920, man_player);
                SYSMSG_SENDV(923, man_player);
                SYSMSG_SENDV(920, woman_player);
                SYSMSG_SENDV(923, woman_player);
                DB4().PushUpdateData("UPDATE `married_couple` SET `pet_level` = %u, `pet_levelExp` = %u WHERE `jh_time` = %u", it->second->level, it->second->levelExp,man_player->GetMarriageInfo()->yuyueTime);
                break;
            case AWARD_MEMEORY:
                /*it->second->levelExp += 4;
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
                it->second->friendliness += 5;
                DB4().PushUpdateData("UPDATE `married_couple` SET `pet_friendliness` = %u WHERE `jh_time` = %u", it->second->friendliness, man_player->GetMarriageInfo()->yuyueTime);*/
                man_player->AddVar(VAR_COUPLE_COPY_COUNT,1);
                SYSMSG_SENDV(925, man_player);
                SYSMSG_SENDV(924, man_player);
                SYSMSG_SENDV(925, woman_player);
                SYSMSG_SENDV(924, woman_player);
                break;
            case WINNER_COPY:
                it->second->levelExp += 5;
                it->second->friendliness += 5;
                if(it->second->friendliness >= 1500)
                    it->second->friendliness = 1500;

                rebuildCouplePet(man_player);
                gMarryMgr.SetDirty(man_player,woman_player);
                SYSMSG_SENDV(926, man_player);
                SYSMSG_SENDV(927, man_player);
                SYSMSG_SENDV(926, woman_player);
                SYSMSG_SENDV(927, woman_player);
                DB4().PushUpdateData("UPDATE `married_couple` SET `pet_level` = %u, `pet_levelExp` = %u, `pet_friendliness` = %u WHERE `jh_time` = %u", it->second->level, it->second->levelExp, it->second->friendliness, man_player->GetMarriageInfo()->yuyueTime);

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
        {
            if(it->second->level == 46)
                break;
            it->second->level += 1; 
            if((GData::cu).getUpgradeData(it->second->level) == NULL)
                break;
        }

        return;
    }

    void MarriedMgr::addCouplePetAttr(Player* player,GData::AttrExtra& ae)
    {
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it == m_couple.end())
            return;
                
        GData::CoupleUpgradeData* cud = (GData::cu).getUpgradeData(it->second->level);
        if(cud == NULL)
            return;

        UInt32 friendliness = it->second->friendliness;
        if(friendliness < 125)
            return; 
        if(friendliness >= 125 ) 
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

/* VAR_COUPLE_COPY_STATUS  从低到高 副本类型 0-离开副本 1，2，3-三个副本类型，玩家队伍状态 0-空 1-队长 2-组员，跳过战斗flag 0-表示勾选 ，自动开始战斗flag 0-表示勾选  */

    void MarriedMgr::EnterCoupleCopy(Player* player,UInt8 copy_type)
    {
        if(copy_type != 0 && player->getMainFighter()->getLevel() < 75)
        {
            if(copy_type != 1)
                return;
        }
        
        player->SetVar(VAR_COUPLE_COPY_STATUS,0);//清空状态 
        if(copy_type != 0 && copy_type != 1 && copy_type != 2 && copy_type != 3)
            return;
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        if(PreCheckingStatus(player) != 0)
            return ;
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
        
      /*  if(GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT), 1) == 0)//每日免费次数
        {
            man_player->AddVar(VAR_COUPLE_COPY_COUNT,1);
            
            UInt32 tmp_value = SET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_STATUS),1,1);  
            man_player->SetVar(VAR_COUPLE_COPY_DAY_COUNT,tmp_value);
        }*/
        
        if(copy_type == 0)
            player->SetVar(VAR_COUPLE_COPY_STATUS,0);
        else
        {
            UInt32 tmp_status = player->GetVar(VAR_COUPLE_COPY_STATUS);
            tmp_status = SET_BIT_8(tmp_status,0,copy_type);  
            if(GET_BIT_8(obj_player->GetVar(VAR_COUPLE_COPY_STATUS), 0) == copy_type)//同一个副本
                tmp_status = SET_BIT_8(tmp_status,1,2);
            else 
                tmp_status = SET_BIT_8(tmp_status,1,1);
            player->SetVar(VAR_COUPLE_COPY_STATUS,tmp_status);
        }

        ReturnCoupleCopyInfo(player); 
        ReturnCoupleCopyInfo(obj_player); 
        return;
    }
    
    void MarriedMgr::ReturnCoupleCopyInfo(Player* player)
    {
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
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

        Stream st(REP::MARRIEDMGR);//返回夫妻副本信息
        st << static_cast<UInt8>(0x08) << static_cast<UInt8>(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS),0));
        if(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 1) == 1)
        {
            st << player->getId();
            if(GET_BIT_8(obj_player->GetVar(VAR_COUPLE_COPY_STATUS), 0) == (GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 0)))
                st << static_cast<UInt64>(obj_player->getId());
            else
                st << static_cast<UInt64>(0);
        }
        else if(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 1) == 2)
            {
                if(GET_BIT_8(obj_player->GetVar(VAR_COUPLE_COPY_STATUS), 0) != (GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 0)))
                {
                    player->SetVar(VAR_COUPLE_COPY_STATUS,SET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS),1,1)); 
                    st << player->getId() << static_cast<UInt64>(0);
                }
                st << obj_player->getId() << player->getId(); 
            }
        if(GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT), 1) == 0)//每日免费次数
            st << static_cast<UInt8>(man_player->GetVar(VAR_COUPLE_COPY_COUNT) + 1) ;
        else
            st << static_cast<UInt8>(man_player->GetVar(VAR_COUPLE_COPY_COUNT)) ;
        st << static_cast<UInt8>(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 2)) << static_cast<UInt8>(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 3)) << static_cast<UInt8>(GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT),0)) << Stream::eos;     
        player->send(st);
        
        return;
    }

    void MarriedMgr::InvitePlayer(Player* player)
    {
        GObject::Player * obj_player = NULL;
        if(PreCheckingStatus(player) != 0)
            return ;
        obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];

        if(obj_player->getMainFighter()->getLevel() < 75)
        {
            if(static_cast<UInt8>(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS),0)) >= 2)
            {
                player->sendMsgCode(0,1709); 
                return;
            }
        }
        
        Stream st(REP::MARRIEDMGR);
        st << static_cast<UInt8>(0x10) << player->getName() << player->getId() << player->getMainFighter()->getColor() << static_cast<UInt8>(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 0)) << Stream::eos;   
        obj_player->send(st);
        //SYSMSG_SENDV(928, player);
        player->sendMsgCode(0, 1708);
        return;
    }

    void MarriedMgr::OpCoupleCopy(Player* player,UInt8 op_type)
    {
        if(op_type != 0 && op_type != 1 && op_type != 2)
            return;
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        if(PreCheckingStatus(player) != 0)
            return ;
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

        Stream st(REP::MARRIEDMGR);
        if(op_type == 2)
        {
            UInt32 tmp_count = 1 + GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT),0); 
            if(useMoney(player,2,15*tmp_count,3) != 0)
                return;
            //购买次数
            man_player->AddVar(VAR_COUPLE_COPY_COUNT,1);
            man_player->AddVar(VAR_COUPLE_COPY_DAY_COUNT,1);//每日购买计数加一
            
            st << static_cast<UInt8>(0x09) << static_cast<UInt8>(2);
            if(GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT), 1) == 0)//每日免费次数
                st << static_cast<UInt8>(man_player->GetVar(VAR_COUPLE_COPY_COUNT) + 1) ;
            else
                st << static_cast<UInt8>(man_player->GetVar(VAR_COUPLE_COPY_COUNT)) ;
            st<< static_cast<UInt8>(GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT),0)) << Stream::eos;
            man_player->send(st);
            woman_player->send(st);
        }
        else
        {
            if(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 1) != 1 && (op_type+2) == 3)//只有队长才能改变跳过战斗或自动开始战斗 
                return;
            UInt32 tmp_status = player->GetVar(VAR_COUPLE_COPY_STATUS);
            UInt32 tmp_status1 = obj_player->GetVar(VAR_COUPLE_COPY_STATUS);
            UInt8 leader_bit = 0;
            if(GET_BIT_8(tmp_status, (op_type+2)) == 0)
            {
                tmp_status = SET_BIT_8(tmp_status,(op_type+2),1);
                if(GET_BIT_8(tmp_status,0) == GET_BIT_8(tmp_status1,0) && (op_type+2) == 3)//同一个副本
                {
                    tmp_status1 = SET_BIT_8(tmp_status1,(op_type+2),1);
                    leader_bit = 1; 
                }
            }
            else 
            {
                tmp_status = SET_BIT_8(tmp_status,(op_type+2),0);
                if(GET_BIT_8(tmp_status,0) == GET_BIT_8(tmp_status1,0) && (op_type+2) == 3)//同一个副本
                {
                    tmp_status1 = SET_BIT_8(tmp_status1,(op_type+2),0);
                    leader_bit = 1; 
                }
            }
            player->SetVar(VAR_COUPLE_COPY_STATUS,tmp_status);
            obj_player->SetVar(VAR_COUPLE_COPY_STATUS,tmp_status1);
            st << static_cast<UInt8>(0x09) << op_type << static_cast<UInt8>(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS),(op_type+2))) << Stream::eos;
            player->send(st);
            if(leader_bit)
                obj_player->send(st);
        }
        return;
    }

    void MarriedMgr::EnterBattle(Player* player)
    {
        if(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS), 1) != 1)//只有队长才能改变跳过战斗或自动开始战斗 
            return; 
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = NULL;
        if(PreCheckingStatus(player) != 0)
            return ;
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
        
        if(static_cast<UInt8>(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS),0)) == 0 || GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS),0) != GET_BIT_8(obj_player->GetVar(VAR_COUPLE_COPY_STATUS),0))//判断俩玩家是否都在副本
            return;

        if(GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT), 1) == 1 && man_player->GetVar(VAR_COUPLE_COPY_COUNT) <= 0)//判断夫妻双方进入副本次数
            return ;

        if(!player->isOnline() || !obj_player->isOnline())
            return;
        player->checkLastBattled();
        obj_player->checkLastBattled();
    
        if (man_player->GetPackage()->GetRestPackageSize() <= 0)
        {
            man_player->sendMsgCode(0, 1011);
            woman_player->sendMsgCode(0, 6028);
            return ;
        }
        if (woman_player->GetPackage()->GetRestPackageSize() <= 0)
        {
            woman_player->sendMsgCode(0, 1011);
            man_player->sendMsgCode(0, 6028);
            return ;
        }

        //读取怪物npcgroup
        UInt8 mapid = static_cast<UInt8>(GET_BIT_8(player->GetVar(VAR_COUPLE_COPY_STATUS),0));
        GData::CoupleCopyData* ccd = (GData::cc).getCopyData(mapid);
        
        if(ccd->location != PLAYER_DATA(player, location))//判断是否在据点上
            return;
        if(ccd->location != PLAYER_DATA(obj_player, location))//判断是否在据点上
            return;
        
        UInt32 NPCId = ccd->monster; 
        UInt32 rptid = 0;
        GData::NpcGroups::iterator it = GData::npcGroups.find(NPCId);
        if(it == GData::npcGroups.end())
            return;
        GData::NpcGroup * ng = it->second;
        if (!ng)
            return;
        
        std::vector<GData::NpcFData>& _npcList = ng->getList();
        if (_npcList.size() == 0)
            return;

        GObject::Fighter* monster = _npcList[0].fighter;

        Battle::BattleSimulator bsim(Battle::BS_COPY1, player, monster->getName(), monster->getLevel(), false);
        bsim.newFighter(0,11,player->getMainFighter());
        bsim.newFighter(0,13,obj_player->getMainFighter());

        ng->putFighters(bsim);
        bsim.start();
        sendBattleReport(man_player,woman_player,ng,bsim,rptid);
        
        man_player->AddVar(VAR_COUPLE_COPY_BATTLE,1);
        UInt8 tmp_count = man_player->GetVar(VAR_COUPLE_COPY_BATTLE);
        if(tmp_count >= 1) 
        {
            if(tmp_count >= 6)
                tmp_count = 6;
            switch(tmp_count)
            {
                case 1:
                    man_player->udpLog("fuqijiayuan", "F_140116_15", "", "", "", "", "act");
                    break;
                case 2:
                    man_player->udpLog("fuqijiayuan", "F_140116_16", "", "", "", "", "act");
                    break;
                case 3:
                    man_player->udpLog("fuqijiayuan", "F_140116_17", "", "", "", "", "act");
                    break;
                case 4:
                    man_player->udpLog("fuqijiayuan", "F_140116_18", "", "", "", "", "act");
                    break;
                case 5:
                    man_player->udpLog("fuqijiayuan", "F_140116_19", "", "", "", "", "act");
                    break;
                case 6:
                    man_player->udpLog("fuqijiayuan", "F_140116_20", "", "", "", "", "act");
                    break;
                default:
                   break;
            }
        }
        man_player->udpLog("fuqijiayuan", "F_140116_15", "", "", "", "", "act");
        man_player->udpLog("fuqijiayuan", "F_140116_7", "", "", "", "", "act");

//        bsim.applyFighterHP(0, pl);
        EnterCoupleCopy(player,0);      
        EnterCoupleCopy(obj_player,0);      
        return;
    }
    
    void MarriedMgr::sendBattleReport(Player* man_player,Player* woman_player,GData::NpcGroup* ng, Battle::BattleSimulator& bsim, UInt32& rptid)
    {
        Stream& packet = bsim.getPacket();
        if(packet.size() <= 8)
            return;

        bool isLast = (ng != NULL);
        UInt8 side = 0;
        UInt8 res = bsim.getWinner();
        UInt16 r = 0;
        if(res == 1)
        {
            side = 1;
            r = static_cast<UInt16>(0x0201);
        }
        else if(res == 2)
        {
            r = static_cast<UInt16>(0x0200);
        }

        UInt32 id = bsim.clearLastBattle(side, isLast);

        if(ng && res == 1)
        {
            ng->getLoots(man_player, man_player->_lastLoot, 0, NULL);
            ng->getLoots(woman_player, woman_player->_lastLoot, 0, NULL);
            
            ChangPetAttr(man_player,woman_player,WINNER_COPY);
            if(GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT), 1) == 0)//每日免费次数
            {
                UInt32 tmp_value = SET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_DAY_COUNT),1,1); 
                man_player->SetVar(VAR_COUPLE_COPY_DAY_COUNT,tmp_value);
            }
            else
            {
                UInt32 tmp_count = man_player->GetVar(VAR_COUPLE_COPY_COUNT); 
                tmp_count --;
                man_player->SetVar(VAR_COUPLE_COPY_COUNT,tmp_count);
            }
        }
        
        Stream st(REP::ATTACK_NPC);
        st << r << id << Stream::eos;
   
        Stream st1(REP::MARRIEDMGR);
        st1 << static_cast<UInt8>(0x12) << static_cast<UInt8>(res-1);
        if(man_player->_lastLoot.size() != 0)
            st1 << static_cast<UInt32>((man_player->_lastLoot.at(0)).id) << Stream::eos;
        else
            st1 << static_cast<UInt32>(0) << Stream::eos; 
        
        Stream st2(REP::MARRIEDMGR);
        st2 << static_cast<UInt8>(0x12) << static_cast<UInt8>(res-1) ;
        if(woman_player->_lastLoot.size() != 0)
            st2 << static_cast<UInt32>((woman_player->_lastLoot.at(0)).id) << Stream::eos;
        else
            st2 << static_cast<UInt32>(0) << Stream::eos; 
        
        if(GET_BIT_8(man_player->GetVar(VAR_COUPLE_COPY_STATUS),2) == 0)
            man_player->send(st);
        else
        {
            man_player->send(st1);
            man_player->checkLastBattled();
        }
        if(GET_BIT_8(woman_player->GetVar(VAR_COUPLE_COPY_STATUS),2) == 0)
            woman_player->send(st);
        else
        {
            woman_player->send(st2);
            woman_player->checkLastBattled();
        }

        if(rptid == 0)
            rptid = id;

        return;
    }
    
    void MarriedMgr::SetCoupleFix(Player* player,UInt8 flag)
    {
        if(flag != 0 && flag != 1)
            return;
        if(PreCheckingStatus(player) != 0)
            return ;

        player->SetVar(VAR_COUPLE_NAME,flag);
        
        Stream st(REP::MARRIEDMGR);
        st << static_cast<UInt8>(0x11) << static_cast<UInt8>(player->GetVar(VAR_COUPLE_NAME)) << Stream::eos;
        player->send(st);

        return;
    }


}


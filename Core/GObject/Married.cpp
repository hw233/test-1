#include "Married.h"
#include "Marry.h"
#include "Player.h"
#include "Common/TimeUtil.h"
#include "MsgID.h"

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
        
        GObject::Player * obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
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
        if(PreCheckingStatus(player) != 0)
            return;
        
        CoupleList::iterator it = m_couple.find(player->GetMarriageInfo()->yuyueTime);
        if(it->second->onlineTime >= 60 * 60 * 8) 
            return;
        
        //夫妻同时在线状态写在男方身上
        GObject::Player * man_player = NULL;
        GObject::Player * obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
        if(!player->getMainFighter()->getSex())//男的
            man_player = player;
        else
            man_player = obj_player;
        
        if(flag == 0)
        {
            if(obj_player->isOnline())
            {
                if(man_player->GetVar(VAR_COUPLE_ONLINE_TIME) != 0)
                    return;
                else
                    man_player->SetVar(VAR_COUPLE_ONLINE_TIME,TimeUtil::Now());
            }   
        }
        else
        {
            if(obj_player->isOnline())
            {
                if(man_player->GetVar(VAR_COUPLE_ONLINE_TIME) > TimeUtil::Now())
                    return ;
                it->second->onlineTime += TimeUtil::Now() - man_player->GetVar(VAR_COUPLE_ONLINE_TIME);//计算同时在线时间
                if(it->second->onlineTime > 60*60*8)
                    it->second->onlineTime = 60*60*8;
            }
        }
    } 
    
    void MarriedMgr::ReturnFirstStatus(Player* player)//返回家园初始状态
    {
        if(PreCheckingStatus(player) != 0)
            return;
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
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
        
        Stream st(REP::MARRIEDMGR);
        st << man_player->getName() << man_player->isOnline() << woman_player->getName() << woman_player->isOnline() << it->second->onlineTime << static_cast<UInt8>(0) << static_cast<UInt8>(0) << Stream::eos;
        
        player->send(st);
        return;
    }
    
    void MarriedMgr::GetOnlineAward(Player* player)
    {
        Mutex::ScopedLock lk(_mutex);
        if(PreCheckingStatus(player) != 0)
            return;
        GObject::Player * man_player = NULL;
        GObject::Player * woman_player = NULL;
        GObject::Player * obj_player = GObject::globalPlayers[player->GetMarriageInfo()->lovers];
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
        
        UInt8 num = 0;
        num = man_player->GetVar(VAR_COUPLE_ONLINE_TIME) / 60*60; 
        if(num < 1)
            return; 
        
        for(UInt8 i = 1;i < 5;i++)
        {
            if(num < i)
                break;
            if(!GET_BIT(player->GetVar(VAR_COUPLE_ONLINE_AWARD),i))
            {
                UInt32 tmp = player->GetVar(VAR_COUPLE_ONLINE_AWARD);
                tmp = SET_BIT(tmp,i);
                player->SetVar(VAR_COUPLE_ONLINE_AWARD,tmp);      
            }
        }
            


    }



    void MarriedMgr::ModifyPetName(Player* player,std::string name)
    {
        Mutex::ScopedLock lk(_mutex);
        if(PreCheckingStatus(player) != 0)
            return;

    }

    void MarriedMgr::LoadMarriedCouple(DBMarriedCouple* dbpn)
    {
        CoupleInfo* ci = new CoupleInfo(dbpn->both_onlinetime,dbpn->pet_name,dbpn->lover_item,dbpn->pet_level,dbpn->pet_levelExp,dbpn->pet_friendliness);
        InsertCoupleInfo(dbpn->jh_time,ci);
        return;
    }
    
    void MarriedMgr::InsertCoupleInfo(UInt32 time,CoupleInfo* ci)
    {
        m_couple.insert(std::make_pair(time,ci));
        return;
    }


}


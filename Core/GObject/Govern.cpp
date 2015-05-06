#include"Govern.h"
#include"MsgID.h"
#include"Common/TimeUtil.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include<time.h>

namespace GObject
{
    GovernManager::GovernManager(Player *pl):m_owner(pl),curMonster(NULL)
    {
         if( pl != NULL )
         {
             m_fighter = pl->findFighter(pl->GetVar(VAR_GOVERN_FIGHTERID));
         }
         else
         {
             m_fighter = NULL;
         }
    }


    void GovernManager::ReplaceFighter(Fighter * ft)
    {
        if( ft == NULL || ft == m_fighter)
           return ;
        m_fighter = ft;
        m_owner->SetVar(VAR_GOVERN_FIGHTERID,ft->getId());
        Stream st(REP::GOVERN_REPLACE);
        st<<static_cast<UInt16>(ft->getId());
        st<<Stream::eos;
        m_owner->send(st);

        //换将的话 怪物信息得重发
        SendGovernResult(1);
    }


    void GovernManager::SpeedUp()
    {
        UInt8 ret = 1;
        if(m_fighter == NULL )
            ret = 2 ;
        if(m_owner->GetVar(VAR_GOVERN_SPEEDUP_CNT) >= SPEEDUP_FREE_CNT )
        {
            ret = 4;
        }
        if( m_owner->GetVar(VAR_GOLD) < 10 )
        {
            ret = 0;
        }
        UInt32 times = SPEEDUP_MAXTIME/TIME_TAB;
        mapId2Num speedupId2Num;        
        if( ret == 1)
        {
            m_owner->UseGold(10);
            GetTotalAward(times,speedupId2Num);
        }
        Stream st(REP::GOVERN_SPEEDUP);
        st<<static_cast<UInt8>(ret);
        st<<static_cast<UInt8>(speedupId2Num.size());
        for(auto it = speedupId2Num.begin(); it != speedupId2Num.end(); ++it)
        {
            st<<static_cast<UInt32>(it->first);
            st<<static_cast<UInt32>(it->second);
        }
        st<<Stream::eos;
        m_owner->send(st);

        if( ret != 1)
        {
            return;
        }

        m_owner->AddVar(VAR_GOVERN_SPEEDUP_CNT,1);
        //发背包 更新背包数据库
        //
        for( auto it = speedupId2Num.begin() ; it != speedupId2Num.end(); ++it)
        {
            if( it->first == 30001 )
            {
                m_owner->AddMoney(1,it->second);
            }
            m_owner->GetPackage()->AddItem((it)->first,(it)->second,true,true,0);
        }
    }

    void GovernManager::SendBaseInfo()
    {
        Stream st(REP::GOVERN_INFO);
        m_fighter = m_owner->findFighter(m_owner->GetVar(VAR_GOVERN_FIGHTERID));
        UInt16 fighterId = 0;
        if( m_fighter != NULL )
            fighterId = m_fighter->getId();
        st<<static_cast<UInt16>(fighterId);
        UInt32 monsterId = 0;
        if( curMonster != NULL )
            monsterId = curMonster->GetMonsterId();
        st<<static_cast<UInt32>(monsterId);
        st<<static_cast<UInt8>(m_owner->GetVar(VAR_GOVERN_SPEEDUP_CNT));
        st<<Stream::eos;
        m_owner->send(st);
    }

    Monster* GovernManager::RandomOneMonster(UInt8 groupId)
    {
        UInt32 monsterId = GameAction()->RandMonster(groupId);
        if( monsterId == 0 )
            monsterId = 1;
        std::cout<<"the monster id is"<<monsterId<<std::endl;
        Monster* mon = monsterTable.GetMonster(groupId,monsterId);
        return mon;
    }



    UInt8 GovernManager::FightWithMonster(Monster* mon)
    {
        UInt8 res = 0;
        if( mon == NULL )
           return res;
        UInt32 power = m_fighter->GetTotalPower();
        //小胜  大胜  平局  失败
        cout<<"  "<<m_fighter->GetName()<<"战力   "<<power<<"   ";
        if( power >= mon->GetPower()*1.4 ) 
        {
            cout<<"  大胜 ";
            res = 1;   //大胜
        }
        else if( power >= mon->GetPower()*1.2)
        {
            cout<<"  小胜 ";
            res = 2;  //小胜
        }
        else if( power >= mon->GetPower())
        {
            cout<<"  平手 ";
            res = 3;
        }
        else if( power >= mon->GetPower()*0.8)
        {
            cout<<"  小败 ";
            res = 4;
        }
        else
        {
            cout<<" 大败 ";
            res = 5;
        }
        cout<<"    野怪名字   "<<mon->GetMonsterName()<<"   野怪战力   " <<mon->GetPower()<<endl;
        return res;

    }
    



    void GovernManager::SendGovernResult(UInt8 type)  //type=1代表登陆时发  0代表正点时发
    {
        _vecGovernInfo.clear();
        if( !m_fighter )
            return;
        UInt8 lv = m_owner->GetGovernLevel();
        UInt32 times = (TIME_ONCE*60)/TIME_TAB;
        for(UInt8 i = 0; i < times ; ++i )
        {
             Monster* mon = RandomOneMonster(lv);
             UInt8 res = FightWithMonster(mon);
             UInt16 base = GameAction()->GetGovernDropItem(res);
             UInt16 random = uRand(10000);

             bool isGet = false;
             if( random < base )
             {
                 isGet = true;
             }
             GovernInfo info(mon->GetGroupId(),mon->GetMonsterId(),res,isGet);
             _vecGovernInfo.push_back(info);
        }
        Stream st(REP::GOVERN_ONLINE_GAIN);
        UInt8 cnt = 0;
        UInt8 begin = 0;
        if(type == 0 )
        {
            cnt = _vecGovernInfo.size();
        }
        else
        {
            time_t now = time(NULL);
            tm* tt=localtime(&now);
            UInt8 min = tt->tm_min;
            UInt8 sec = tt->tm_sec;
            UInt16 second = (min%TIME_ONCE == 0 && sec == 0 ) ? 0 : min%TIME_ONCE+sec;
            if( second == 0 )
            { 
                 begin = 0;
            }
            else
            {
                 begin = (( second%TIME_TAB == 0) ? (second/TIME_TAB):(second/TIME_TAB+1));
            }
            cnt = times - begin; 
        }
        st<<static_cast<UInt8>(cnt);
        GetGovernInfo(st,begin);
        st<<Stream::eos;
        m_owner->send(st);
    }
    


    void GovernManager::GetGovernInfo(Stream& st,UInt8 begin)
    {
        if( _vecGovernInfo.size() == 0 )
            return ;
        for(UInt8 i=begin ;i < _vecGovernInfo.size(); ++i )
        {
            st<<static_cast<UInt32>(_vecGovernInfo[i].monsterId);
            bool isGet = _vecGovernInfo[i].isGet;
            UInt8 res = _vecGovernInfo[i].res;
            res = res | (isGet << 4);
            st<<static_cast<UInt8>(res);
        }
    }




    void GovernManager::SendOnlineGovernAward(UInt8 number)
    {
        if( !m_fighter)
            return;
        if( _vecGovernInfo.size() == 0 )
            return ;
        if( number < 0  || number >= 40 )
            return ;
        GovernInfo& info = _vecGovernInfo[number];
        if( info.groupId < 1 || info.monsterId <= 0 )
            return;
        curMonster = monsterTable.GetMonster(info.groupId , info.monsterId);
        if( curMonster == NULL )
            return;
        std::vector<ItemInfo> vecItem;
        GetItemsByResult(info.res,curMonster->GetGroupId(),curMonster->GetMonsterId(),info.isGet, vecItem);
        for( auto it = vecItem.begin(); it != vecItem.end() ; ++it )
        {
            std::cout<<m_owner->GetName()<<"的战将 >>>>>>>"<<m_fighter->GetName()<<"     在线获得物品    "<<(*it).id<<"  " <<(*it).num<<endl;
            if( (*it).id  == 30001 )
            {
                m_owner->AddMoney(1,(*it).num);
            }
            else
            {
                m_owner->GetPackage()->AddItem((*it).id,(*it).num,true,true,0);
            }
        }
    }




    void GovernManager::GetItemsByResult(UInt8 res,UInt8 groupId,UInt8 monsterId,bool isGet, std::vector<ItemInfo>& vecItem)
    {
        if(res <= 0  || res > 5 )
        {
            return ;
        }
        Monster* mon = monsterTable.GetMonster(groupId,monsterId);
        UInt16 moneyNum = mon->GetMoney();
        UInt16 base =  GameAction()->GetGovernDropMoney(res);
        if( isGet )
        {
            UInt32  itemId = mon->GetItemId();
            UInt8 itemNum = mon->GetItemNum();
            if( itemNum != 0 )
            {
               vecItem.push_back(ItemInfo(itemId,itemNum));
            }
        }
        if( base != 0 )
        {
            vecItem.push_back(ItemInfo(30001,moneyNum*(base/10000.0f)));
        }
    }


    void GovernManager::GetAccumulativeAward(UInt8 res ,Monster* mon,UInt16 prob,UInt8 times,std::vector<ItemInfo>&vecItem)
    {
        if( res <= 0  || res >= 5 || mon == NULL )   //大败的话什么东西都没有的
            return;
        UInt16 moneyNum = mon->GetMoney();
        UInt16 mbase =  GameAction()->GetGovernDropMoney(res);
        UInt16 ibase =  GameAction()->GetGovernDropItem(res);
        vecItem.push_back(ItemInfo(30001,floor(moneyNum*(mbase/10000.0f)*(prob/10000.0f)*times)));
        UInt32 itemId = mon->GetItemId();
        UInt32 itemNum = mon->GetItemNum();
        vecItem.push_back(ItemInfo(itemId,floor(itemNum*(ibase/10000.0f)*(prob/10000.0f)*times)));
    }



    void GovernManager::GetTotalAward(UInt32 times,mapId2Num& mapId2Num)
    {
        UInt8 lv = m_owner->GetGovernLevel();
        std::set<Monster*> monSet = monsterTable.GetMonsterSet(lv);
        if( monSet.empty() )
            return;
        for( auto it = monSet.begin() ; it != monSet.end() ; ++it)
        {
            UInt8 res = FightWithMonster(*it);
            UInt16 prob = (*it)->GetProb();
            std::vector<ItemInfo> vecItem;
            GetAccumulativeAward(res,(*it),prob,times,vecItem);
            for( auto i = vecItem.begin() ; i != vecItem.end() ; ++i )
            {
                if( (*i).id == 0 || (*i).num == 0 )
                    continue;
                if( mapId2Num.find((*i).id) != mapId2Num.end() )
                {
                    mapId2Num[(*i).id] += (*i).num;
                }
                else
                {
                    mapId2Num[(*i).id] = (*i).num;
                }
            }
        }
    }

    void GovernManager::GiveGovernOfflineGain()
    {
        m_fighter = m_owner->findFighter(m_owner->GetVar(VAR_GOVERN_FIGHTERID));
        if( !m_fighter )
            return;
        //往背包中放置物品
        UInt32 now = TimeUtil::Now();
        UInt32 lastoffline = m_owner->GetVar(VAR_OFF_LINE);
        if( lastoffline == 0 )
            return;
        if( now <= lastoffline )
            return;
        UInt32 times = (now-lastoffline)/TIME_TAB;
        mapId2Num offId2Num;
        GetTotalAward(times,offId2Num);
        SendOfflineGainsInfo(offId2Num);
        for( auto it = offId2Num.begin() ; it != offId2Num.end(); ++it)
        {
            
            if( it->first == 30001 )
            {
                m_owner->AddMoney(1,it->second);
            }
            else
            {
                m_owner->GetPackage()->AddItem((it)->first,(it)->second,true,true,0);
            }
        }
    }


    void GovernManager::SendOfflineGainsInfo(mapId2Num& offId2Num)
    {
        if(offId2Num.size() == 0 )
            return;
        Stream st(REP::GOVERN_OFFLINE_GAIN);
        st<<static_cast<UInt32>(offId2Num.size());
        for(auto it = offId2Num.begin() ; it != offId2Num.end() ; ++it)
        {
            st<<static_cast<UInt32>(it->first);
            st<<static_cast<UInt32>(it->second);
        }
        st<<Stream::eos;
        m_owner->send(st);
    }



}

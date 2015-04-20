#include"Govern.h"
#include"MsgID.h"
#include"Common/TimeUtil.h"
#include<time.h>

namespace GObject
{
    GovernManager::GovernManager(Player *pl):m_owner(pl),m_fighter(NULL),curMonster(NULL)
    {
         //memset(_speedUpId2Num,0,sizeof(_speedUpId2Num));
         //memset(_offLineId2Num,0,sizeof(_offLineId2Num));
         //memset(_vecGovernInfo,0,sizeof(_vecGovernInfo));
    }
    void GovernManager::ReplaceFighter(Fighter * ft)
    {
        if( ft == NULL || ft == m_fighter)
           return ;
        m_fighter = ft;
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
            ret = 0;
        if(m_owner->GetVar(VAR_GOVERN_SPEEDUP_CNT) >= SPEEDUP_FREE_CNT )
        {
            return;
            //扣元宝
        }
        UInt32 times = SPEEDUP_MAXTIME/TIME_TAB;
        GetSpeedUpGains(times);
        //判断背包是否已满
        if( false)
        {
            ret = 1;

        }
        Stream st(REP::GOVERN_SPEEDUP);
        st<<static_cast<UInt8>(ret);
        st<<static_cast<UInt8>(_speedUpId2Num.size());
        for(auto it = _speedUpId2Num.begin(); it != _speedUpId2Num.end(); ++it)
        {
            st<<static_cast<UInt32>(it->first);
            st<<static_cast<UInt32>(it->second);
        }
        st<<Stream::eos;
        m_owner->send(st);
        _speedUpId2Num.clear();
        m_owner->AddVar(VAR_GOVERN_SPEEDUP_CNT,1);
        //发背包 更新背包数据库
    }

    void GovernManager::SendGovernInfo()
    {
        Stream st(REP::GOVERN_INFO);
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

    void GovernManager::GetSpeedUpGains(UInt32 times)
    {
        for(UInt16 i=0; i< times ;++i)
        {
            GetOneSpeedUpGain();
        }
    }


    UInt32 GovernManager::RandomOneMonster()
    {
        //UInt32 id = uRandom();
        UInt32 id = 1;
        return id;
    }

    bool GovernManager::FightWithMonster(UInt32 monsterId)
    {
        Monster* mon = monsterTable.GetMonster(monsterId);
        if( mon->GetLev() < m_owner->GetLevel())
        {
            return false;  // 野怪打胜
        }
        return true;

    }

    void GovernManager::SendGovernResult(UInt8 type)  //type=1代表登陆时发  0代表正点时发
    {
        _vecGovernInfo.clear();
        if( !m_fighter )
            return;
        UInt32 times = SPEEDUP_MAXTIME/TIME_TAB;
        for(UInt8 i = 0; i < times ; ++i )
        {
             UInt32 id = RandomOneMonster();
             bool win = FightWithMonster(id);
             GovernInfo info(id,win);
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
            if( min == 60-TIME_TAB || min == 60-TIME_TAB+1 )
            {
                cnt = 0;
                begin = 30;
            }
            else
            {
                begin = (( min%TIME_TAB == 0) ? (min/TIME_TAB):(min/TIME_TAB+1));
                cnt = times - begin; 
            }
        }
        st<<static_cast<UInt8>(cnt);
        GetGovernInfo(st,begin);
        st<<Stream::eos;
        m_owner->send(st);
    }
    

    void GovernManager::GetOneSpeedUpGain() //一次加速获得收益
    {
        UInt32 id = RandomOneMonster();
        bool win = FightWithMonster(id);
        if( win )
        {
            UInt32 itemId = 1;
            UInt32 itemNum = 1;
            if( _speedUpId2Num.find(itemId) != _speedUpId2Num.end() )
            {
                _speedUpId2Num[itemId] += itemNum;
            }
            else
            {
                _speedUpId2Num[itemId]  = itemNum;
            }

        }
    }

    void GovernManager::OfflineGainsInfo(Stream& st)
    {
        if(_offlineId2Num.size() == 0 )
            return ;
        st<<static_cast<UInt32>(_offlineId2Num.size());
        for(auto it = _offlineId2Num.begin() ; it != _offlineId2Num.end() ; ++it)
        {
            st<<static_cast<UInt32>(it->first);
            st<<static_cast<UInt32>(it->second);
        }
    }

    void GovernManager::GetGovernInfo(Stream& st,UInt8 begin)
    {
        if( _vecGovernInfo.size() == 0 )
            return ;
        for(UInt8 i=begin ;i < _vecGovernInfo.size(); ++i )
        {
            st<<static_cast<UInt32>(_vecGovernInfo[i].monsterId);
            st<<static_cast<UInt8>(_vecGovernInfo[i].isWin);

        }
    }

    void GovernManager::SendOnlineGovernAward(UInt8 number)
    {
        if( !m_fighter)
            return;
        if( _vecGovernInfo.size() == 0 )
            return ;
        if( number < 0  || number >= 30 )
            return ;
        GovernInfo& info = _vecGovernInfo[number];
             //curMonster = monsterTable.GetMonster(info.monsterId);
        bool isWin = info.isWin;
        if( !isWin )
        {
            return;
        }
    }




    void GovernManager::SendOfflineGovernAward()
    {
        if( !m_fighter )
            return;
        UInt32 monsterId = RandomOneMonster();
        bool win = FightWithMonster(monsterId);
        if( !win )
             return;
        UInt32 itemId = 1;
        UInt32 itemNum = 1;

        if( _offlineId2Num.find(itemId) != _offlineId2Num.end() )
        {
            _offlineId2Num[itemId] += itemNum;
	        DB7().PushUpdateData("update govern_offlinegain set `itemNum` = %u where `itemId` = %u AND `playerId` = %"I64_FMT"u ",itemId,m_owner->getId());
        }
        else
        {
            _offlineId2Num[itemId] = itemNum;
	        DB7().PushUpdateData("insert into  govern_offlinegain(playerId,itemId,itemNum) value(%"I64_FMT"u,%u,%u)",m_owner->getId(),itemId,itemNum);
        }
    }




    void GovernManager::loadGovernOfflineGain(UInt32 itemId, UInt32 itemNum)
    {
        if( itemId <= 0 && itemNum <= 0 )
            return;
        _offlineId2Num[itemId] = itemNum;
    }


    void GovernManager::GiveGovernOfflineGain()
    {
        if( !m_fighter )
            return;
        //发送离线治理的奖励(登录时发送)
        {
            //TODO 判断背包是否已满
        }
        //往背包中放置物品
        //发完后清理离线治理物品数据库
	    DB7().PushUpdateData("delete from govern_offlinegain where `playerId` = %"I64_FMT"u",m_owner->getId());

    }
}

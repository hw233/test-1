#include "MonsterKettle.h"
#include "Common/Itoa.h" 
#include "Common/StringTokenizer.h"
#include "GData/NpcGroup.h"
#include "MsgID.h"
#include "GData/KettleNpc.h"
#include "Server/SysMsg.h"

namespace GObject
{
    void MonsterKettle::SetMonster(UInt8 idx , UInt32 npcId , UInt8 king)
    {
        if(idx >= INSIDE_KETTLE_MAX)
            return ;
        _npcIds[idx].npcId = npcId;
        _npcIds[idx].king = king;
        SetWin(idx , 0);
    }

    MonsterKing MonsterKettle::GetMonster(UInt8 idx )
    {
        if(idx >= INSIDE_KETTLE_MAX)
            return MonsterKing();
     //   if(GetWin(idx))
     //       return MonsterKing();
        return _npcIds[idx];
    }

    UInt32 MonsterKettle::RandomMonster(Player * pl ,UInt8 num , UInt8 type ,UInt32 count)
    { 
        static UInt32 kingRand[] = {3000, 6000, 8450, 9350, 10000};
        count  = 1;

        //bool flag = false;
        UInt32 cnt = 0;
        for(; cnt < count * 3 ; )
        {
            UInt8 monsterId = uRand(7)+1;
            if(monsterId > 7)
                monsterId = 0 ;
            UInt32 rand = uRand(10000);
            UInt8 number = 0;
            for(UInt8 i = 0; i < 5 ; ++i)
            { 
                if(rand < kingRand[i])
                    break;
                number = i ;
            } 
            if(pl)
            {
                if(pl->GetVar(VAR_KETTLE_4CNT) >= 15)
                {
                    number = 3;
                }
                else if(pl->GetVar(VAR_KETTLE_5CNT) >= 30)
                {
                    number = 4;
                }
                else if(number == 4 && !IsCompelete())
                    continue ;


                if(number == 3)
                {
                    pl->SetVar(VAR_KETTLE_4CNT,0);
                }
                else if(number == 4)
                {
                    pl->SetVar(VAR_KETTLE_5CNT,0);
                }

                // if(MonsterProcess[monsterId-1][number] != 5)
                //     flag = true;
                if(cnt % 3 == 2)
                {
                    pl->AddVar(VAR_KETTLE_4CNT,1) ;
                    pl->AddVar(VAR_KETTLE_5CNT,1) ;
                }
            }
            SetMonster(cnt % 3 ,static_cast<UInt32>(16000 + num * 10 + monsterId), number );
    //      std::cout << "坑位: " << static_cast<UInt32>(cnt%3) << "怪物ID" << static_cast<UInt32>(16000 + num * 10 + monsterId) << std::endl;
            //if(cnt >=2 && flag)
            //  break;
            ++cnt;
        }
        return (cnt?cnt-1:0)/3 + 1;
    } 

    void MonsterKettle::SetWin(UInt8 idx , UInt8 flag)
    {
        if(idx >= INSIDE_KETTLE_MAX)
            return ;
        _hasWin[idx] = flag;
    }

    UInt8 MonsterKettle::GetWin(UInt8 idx)
    {
        if(idx >= INSIDE_KETTLE_MAX)
            return 0;
        return _hasWin[idx];
    }

    void MonsterKettle::SetMonsterProcess(UInt8 idx , UInt8 king , UInt8 count)
    {
        if(idx >= MONSTER_TYPE) 
            return ;
        if(king >= MONSTER_KING)
            return ;
        MonsterProcess[idx][king] = count ;
    }

    UInt8 MonsterKettle::GetMonsterProcess(UInt8 idx , UInt8 king )
    {
        if(idx >= MONSTER_TYPE) 
            return 0;
        if(king >= MONSTER_KING)
            return 0;
        return MonsterProcess[idx][king];
    }

    void MonsterKettle::GetMonsterInfo(Stream& st)
    {
        st << static_cast<UInt8>(INSIDE_KETTLE_MAX);
        for(UInt8 i = 0; i < INSIDE_KETTLE_MAX; ++i)
        {
            st << static_cast<UInt32>(_npcIds[i].npcId % 1000 /10 * 7 + _npcIds[i].npcId%10);
            st << _npcIds[i].king;
            st << _hasWin[i];
        }
    }

    void MonsterKettle::GetMonsterProcessInfo(Stream& st)
    {
        st << static_cast<UInt8>(MONSTER_TYPE);
        for(UInt8 i = 0; i < MONSTER_TYPE ; ++i) 
        {
            for(UInt8 j = 0; j < MONSTER_KING; ++ j)
            {
                st << MonsterProcess[i][j] ;
                //std::cout <<"process : "<< "monsterNum :" << static_cast<UInt32>(i) << " king:" <<static_cast<UInt32>(j)<< " " << static_cast<UInt32>(MonsterProcess[i][j]) << std::endl;
            }
        }
    }

    UInt32 MonsterKettle::challenge(Player * pl,Stream& st , UInt64 & exp2 , UInt8 idx , UInt8 flag)
    {
        static UInt8 kingCount[5] = { 10, 15, 20, 25, 30 };
        static UInt8 kingNum[] = {1,2,3,80,240};
        if(!pl)
            return 0;
        if(_npcIds[idx].npcId == 0 || _npcIds[idx].king >= 5)
            return 0;
        if(GetWin(idx))
            return 0;
        if(idx >= INSIDE_KETTLE_MAX)
            return 0;
        if(_npcIds[idx].king > 4)
            return 0;
        GData::NpcGroups::iterator it = GData::npcGroups.find(_npcIds[idx].npcId);
        if(it == GData::npcGroups.end())
            return 0;
        UInt32 rptid = 0;
        bool res = false;
        GData::NpcGroup * ng = it->second;
        if(!ng)
            return 0;

        if(!flag)
        {
            std::vector<GData::NpcFData> vec = ng->getList();
            for(UInt8 i = 0; i < vec.size(); ++i)
            {
                if(vec[i].fighter)
                    vec[i].fighter->SetPowerUpByP((kingCount[_npcIds[idx].king] - 10)*1.0/10);
            }
            Battle::BattleSimulator bsim(0x7000, pl, ng->getName(), ng->getLevel(), false);
            pl->PutFighters( bsim, 0 );
            ng->putFighters( bsim );
            bsim.start();
            //sendBattleReport( ng, bsim, rptid);
            rptid = bsim.clearLastBattle(1, true);
            res = bsim.getWinner() == 1;

            for(UInt8 i = 0; i < vec.size(); ++i)
            {
                if(vec[i].fighter)
                    vec[i].fighter->ClearPowerUp();
            }
        }
        //std::cout << "monsterId : " << static_cast<UInt32>(_npcIds[idx].npcId) << std::endl;
        //std::cout << "king : " << static_cast<UInt32>(_npcIds[idx].king) << std::endl;
        //std::cout << "playerId : " << static_cast<UInt32>(pl->getId()) << " monsterId:"<<_npcIds[idx].npcId % 1000 /10 * 7 + _npcIds[idx].npcId%10<<" monsterName:" << ng->getName() << " king:" <<static_cast<UInt32>(_npcIds[idx].king)<< " process:" << static_cast<UInt32>(GetMonsterProcess(_npcIds[idx].npcId %10 -1 , _npcIds[idx].king)) << std::endl;
        UInt64 exp = 0;
        if(res || flag)
        {
            exp += ng->getExp()*kingNum[_npcIds[idx].king] ;
         //   std::cout << "基础:" << static_cast<UInt32>(exp) ;
            if(AddMonsterProcess(_npcIds[idx].npcId %10 ,_npcIds[idx].king))
            {
                pl->AddVar(VAR_KETTLE_COUNT,1);
                exp += GData::kettleAttrData.GetMonsterExp(_npcIds[idx].npcId%1000/10, _npcIds[idx].npcId%10 ,_npcIds[idx].king ) * kingCount[_npcIds[idx].king]/10;
            }
            SetWin(idx , 1);
        //    std::cout << "最终:" << static_cast<UInt32>(exp) << std::endl;
            //pl->AddExp(exp);
            pl->setFightersDirty(true);
            if(_npcIds[idx].king == 4)
                SYSMSG_BROADCASTV(5190, pl->getCountry(), pl->getName().c_str(), ng->getName().c_str()); 
        }
        st << static_cast<UInt32>(_npcIds[idx].npcId % 1000 /10 * 7 + _npcIds[idx].npcId%10);
        st << static_cast<UInt8>(_npcIds[idx].king);
        st << static_cast<UInt8>(GetMonsterProcess(_npcIds[idx].npcId %10-1 , _npcIds[idx].king));
        st << rptid;
        st << static_cast<UInt8>(res||flag);
        st << exp ;
        exp2 += exp;
        return 0; 
    }
    bool MonsterKettle::AddMonsterProcess(UInt8 type , UInt8 king)
    { 
        if(type ==0 )
            return false;
        if(type > MONSTER_TYPE || king >= MONSTER_KING)
            return false;
        if( MonsterProcess[type-1][king] >= 5)
            return false;
        ++MonsterProcess[type-1][king];
        return true;

    } 


    void MonsterKettle::UpdateKettleToDB(UInt64 playerId , UInt8 num)
    { 
        std::string history;
        for(UInt8 i = 0; i < MONSTER_TYPE ; ++i)
        { 
            if(i != 0)
                history += "|";
            for(UInt8 j = 0; j < MONSTER_KING ; ++j)
            { 
                if(j != 0)
                    history += ",";
                history += Itoa(MonsterProcess[i][j]);
            } 
        } 
        std::string occupy;
        for(UInt8 i = 0; i < INSIDE_KETTLE_MAX; ++ i)
        { 
            if( i != 0)
                occupy += "|";
            occupy += Itoa(_npcIds[i].npcId);
            occupy += ",";
            occupy += Itoa(_npcIds[i].king);
            occupy += ",";
            occupy += Itoa(_hasWin[i]);
        } 

        DB1().PushUpdateData("REPLACE INTO `kettle`(`playerId`, `num`, `history`, `occupy`) VALUES(%" I64_FMT "u, %u, '%s', '%s')", playerId , num, history.c_str(), occupy.c_str());
    } 
    UInt8 MonsterKettle::IsCompelete()
    {
        for(UInt8 j = 0; j < MONSTER_TYPE ; ++j)
        { 
            for(UInt8 k = 0; k < MONSTER_KING; ++k)
            {
                if(GetMonsterProcess(j,k) < 5)
                    return 0;
            }
        }
        return 1;
    }

    //    void MonsterKettleManager::GetBattleStream(Stream &st)
    //    {
    //        if(BattleInfoDeque.size() > 20)
    //            return ;
    //        st << static_cast<UInt8>(BattleInfoDeque.size());
    //        for(UInt8 i = 0; i < BattleInfoDeque.size();++i)
    //        {
    //            st << BattleInfoDeque[i]._battleId;
    //            st << BattleInfoDeque[i]._npcId;
    //            st << BattleInfoDeque[i]._res;
    //        }
    //        return ;
    //    }
    ////    void MonsterKettleManager::AddBattleInfo(BattleInfo bi)
    //    {
    //        if(BattleInfoDeque.size() >= 20) 
    //            BattleInfoDeque.pop_front();
    //        BattleInfoDeque.push_back(bi);
    //        return ;
    //    }
    void MonsterKettleManager::GetMonsterKettleInfo(Stream &st )
    {
        st << static_cast<UInt32>(m_player->GetVar(VAR_KETTLE_COUNT));
        st << static_cast<UInt32>(m_player->GetVar(VAR_KETTLE_TIME));
        st << static_cast<UInt8>(m_player->GetVar(VAR_KETTLE_LOCK));
        st << static_cast<UInt32>(m_player->GetVar(VAR_KETTLE_4CNT));
        st << static_cast<UInt32>(m_player->GetVar(VAR_KETTLE_5CNT));
        st << static_cast<UInt8>(KETTLE_COUNT);
        std::cout<<"begin:" << static_cast<UInt32>(st.size()) << std::endl;
        if(monsterKettle[0].GetMonster(0).npcId == 0)
        {
            for(UInt8 i = 0 ; i < KETTLE_COUNT ; ++i)
            {
                monsterKettle[i].RandomMonster(NULL,i,1,1) ;
                UpdateToDB(i);
            }
        }
        for(UInt8 i = 0; i < KETTLE_COUNT ; ++i)
        {
            //    std::cout<<"壶中界编号:" << static_cast<UInt32>(i) << std::endl;
            monsterKettle[i].GetMonsterInfo(st);
            monsterKettle[i].GetMonsterProcessInfo(st);
            //   std::cout<<"number:"<<static_cast<UInt32>(i)<<"  "<< static_cast<UInt32>(st.size()) << std::endl;
        }
        //std::cout<<"end:" << static_cast<UInt32>(st.size()) << std::endl;
    }
    void MonsterKettleManager::SetKettleHistory(UInt8 idx, std::string history)
    {
        StringTokenizer tokenizer(history, "|");
        if(tokenizer.count() > KETTLE_COUNT )
            return ;
        for(size_t j = 0; j < tokenizer.count(); ++ j)  //每一个怪
        {
            StringTokenizer tokenizer2(tokenizer[j], ",");
            if(tokenizer2.count() > 5)
                continue;
            for(size_t k = 0; k < tokenizer2.count(); ++ k)  //每一个星级
            {
                monsterKettle[idx].SetMonsterProcess(j,k,static_cast<UInt8>(atoi(tokenizer2[k].c_str())));
            }
        }
    }
    void MonsterKettleManager::SetKettleOccupy(UInt8 idx ,std::string occupy)
    {
        StringTokenizer tokenizer(occupy, "|");
        if(tokenizer.count() > INSIDE_KETTLE_MAX )
            return ;
        for(size_t j = 0; j < tokenizer.count(); ++ j)  //每一个怪
        {
            StringTokenizer tokenizer2(tokenizer[j], ",");
            if(tokenizer2.count() != 3)
                continue;
            monsterKettle[idx].SetMonster(j,static_cast<UInt32>(atoi(tokenizer2[0].c_str())) , static_cast<UInt8>(atoi(tokenizer2[1].c_str())));
            monsterKettle[idx].SetWin(j,static_cast<UInt8>(atoi(tokenizer2[2].c_str())));
        }
        return ;
    }
    void MonsterKettleManager::UpdateToDB(UInt8 idx)
    { 
        if(idx >= KETTLE_COUNT)
            return ;
        monsterKettle[idx].UpdateKettleToDB(m_player->getId() , idx );
    } 
    UInt32 MonsterKettleManager::RandomMonster(UInt8 idx , UInt8 type , UInt32 count )
    { 
        if( idx >= KETTLE_COUNT)
            return 0;
        UInt32 cnt =  monsterKettle[idx].RandomMonster(m_player ,idx , type , count);
        UpdateToDB(idx);
        return cnt ;
    } 
    void MonsterKettleManager::BattleMonster(UInt8 idx , UInt8 num , UInt8 count)
    { 
        if( idx >= KETTLE_COUNT)
            return ;
        Stream st(REP::ACT);
        st << static_cast<UInt8>(0x35);
        st << static_cast<UInt8>(0x03);
        UInt64 exp = 0;
        switch(count)
        {
            case 0:
                {
                    st << static_cast<UInt8>(1);
                    monsterKettle[idx].challenge(m_player,st,exp,num);
                    break;
                }
                break;
            case 1:
            case 2:
                {
                    st << static_cast<UInt8>(count * 10 * 3);
                    for(UInt8 i = 0 ; i < count *10 ; ++i)
                    { 
                        RandomMonster(idx,1,1);
                        monsterKettle[idx].challenge(m_player,st,exp,0,1 );
                        monsterKettle[idx].challenge(m_player,st,exp,1,1 );
                        monsterKettle[idx].challenge(m_player,st,exp,2,1 );
                    } 
                    break;
                }
        }
        st << Stream::eos;
        m_player->send(st);
        m_player->AddExp(exp);
        monsterKettle[idx].UpdateKettleToDB(m_player->getId(),idx);
    } 
    void MonsterKettleManager::GetKettleAttr(GData::AttrExtra& ae)
    { 
        static GData::KettleAttr kettleFloor [] = {
            GData::KettleAttr( 58.65, 51, 136, 5.95),
            GData::KettleAttr( 51, 58.65, 136, 5.95),
            GData::KettleAttr( 35.7, 35.7, 306, 5.95),
            GData::KettleAttr( 58.65, 51, 136, 5.95),
            GData::KettleAttr( 58.65, 51, 136, 5.95),
            GData::KettleAttr( 51, 58.65, 136, 5.95),
            GData::KettleAttr( 35.7, 35.7, 306, 5.95),
            GData::KettleAttr( 58.65, 51, 136, 5.95),
            GData::KettleAttr( 51, 58.65, 136, 5.95),
            GData::KettleAttr( 51, 58.65, 136, 5.95),
            // GData::KettleAttr(72.9,72.9,594.0,0.0 ),
            // GData::KettleAttr(63.0,63.0,150.0,33.3 )
        };
        //std::cout << "改变前:";
        //std::cout << ae.attack <<" "  << ae.magatk << "  ";
        //std::cout << ae.hp <<" "  << ae.action <<std::endl;
        for(UInt8 i = 0; i < KETTLE_COUNT ; ++i)
        { 
            bool flag = true;
            for(UInt8 j = 0; j < MONSTER_TYPE ; ++j)
            { 
                for(UInt8 k = 0; k < MONSTER_KING; ++k)
                {
                    UInt8 process = monsterKettle[i].GetMonsterProcess(j,k);
                    if(flag && process < 5 )
                        flag = false;
                    if(process == 0)
                        continue;
                    GData::kettleAttrData.GetMonsterAttr(ae,i,j,k,process);
                }
            } 
            if(flag)
            { 
                ae.attack += kettleFloor[i]._attack;
                ae.magatk += kettleFloor[i]._magatk;
                ae.hp += kettleFloor[i]._hp; 
                ae.action += kettleFloor[i]._action;
            } 
        } 
        //std::cout << "改变后:";
        //std::cout << ae.attack <<" "  << ae.magatk << "  ";
        //std::cout << ae.hp <<" "  << ae.action <<std::endl;
    } 
    bool MonsterKettleManager::CheckKettleRight(UInt8 idx)
    { 
        static UInt32 limit[] = {0,0,0,0,700,875,1050,1225,1400,1575,1750,1925};
        UInt32 kettleCnt = m_player->GetVar(VAR_KETTLE_COUNT);
        if(kettleCnt < limit[idx])
            return false;
        return true;
    } 
}


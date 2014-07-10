#include "MonsterKettle.h"
#include "Common/Itoa.h" 
#include "Common/StringTokenizer.h"
#include "GData/NpcGroup.h"
#include "MsgID.h"
#include "GData/KettleNpc.h"

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
        if(GetWin(idx))
            return MonsterKing();
        return _npcIds[idx];
    }

    UInt8 MonsterKettle::RandomMonster(UInt8 num , UInt8 type ,UInt8 count)
    { 
        static UInt32 kingRand[] = {3000, 6000, 8450, 9350, 10000};
        if(type == 0 && count > 1)
            return 0;

        bool flag = false;
        UInt8 cnt = 0;
        for(; cnt < count * 3 ; ++cnt)
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
                number = i + 1;
            } 
            if(MonsterProcess[monsterId-1][number] != 5)
                flag = true;
            SetMonster(cnt % 3 ,static_cast<UInt32>(16000 + number * 10 + monsterId), number );
            if(cnt >=3 || flag)
                break;
        }
        return cnt/3 + 1;
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
            st << _npcIds[i].npcId;
            st << _npcIds[i].king;
        }
        GetMonsterProcessInfo(st);
    }

    void MonsterKettle::GetMonsterProcessInfo(Stream& st)
    {
        for(UInt8 i = 0; i < MONSTER_TYPE ; ++i) 
        {
            for(UInt8 j = 0; j < MONSTER_KING; ++ j)
            {
                st << MonsterProcess[i][j] ;
            }
        }
    }

    UInt32 MonsterKettle::challenge(Player * pl , UInt8 idx)
    {
        static UInt8 kingCount[5] = { 10, 15, 20, 25, 30 };
        if(_npcIds[idx].npcId == 0 || _npcIds[idx].king >= 5)
            return 0;
        if(idx >= INSIDE_KETTLE_MAX)
            return 0;
        UInt32 rptid = 0;
        GData::NpcGroups::iterator it = GData::npcGroups.find(_npcIds[idx].npcId);
        if(it == GData::npcGroups.end())
            return 0;
        GData::NpcGroup * ng = it->second;
        std::vector<GData::NpcFData> vec = ng->getList();
        for(UInt8 i = 0; i < vec.size(); ++i)
        {
            vec[i].fighter->SetPowerUpByP((kingCount[_npcIds[idx].king] - 10)*1.0/10);
        }
        Battle::BattleSimulator bsim(0x7000, pl, ng->getName(), ng->getLevel(), false);
        pl->PutFighters( bsim, 0 );
        ng->putFighters( bsim );
        bsim.start();
        //sendBattleReport( ng, bsim, rptid);
        rptid = bsim.clearLastBattle(1, true);
        bool res = bsim.getWinner() == 1;

        for(UInt8 i = 0; i < vec.size(); ++i)
        {
            vec[i].fighter->ClearPowerUp();
        }

        UInt64 exp = 0;
        if(res)
        {
            exp =  ng->getExp() * (kingCount[_npcIds[idx].king]*1.0/10);
            pl->AddExp(exp);
            SetWin(idx , 1);
            AddMonsterProcess(_npcIds[idx].npcId %10 ,_npcIds[idx].king);
        }
        Stream st(REP::ACT);
        st << static_cast<UInt8>(0x35);
        st << static_cast<UInt8>(0x03);
        st << rptid;
        st << static_cast<UInt8>(res);
        st << exp ;
        return 0; 
    }
    void MonsterKettle::AddMonsterProcess(UInt8 type , UInt8 king)
    { 
        if(type ==0 )
            return ;
        if(type > KETTLE_COUNT || king >= MONSTER_KING)
            return ;
        if( MonsterProcess[type-1][king] >= 5)
            return ;
        ++MonsterProcess[type-1][king];
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

        DB1().PushUpdateData("REPLACE INTO `kettle`(`playerId`, `num`, `history`, `occupy`) VALUES(%" I64_FMT "u, %u, %s, %s)", playerId , num, history.c_str(), occupy.c_str());
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
        st << static_cast<UInt8>(KETTLE_COUNT);
        for(UInt8 i = 0; i < KETTLE_COUNT ; ++i)
        {
            monsterKettle[i].GetMonsterInfo(st);
        }
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
    UInt8 MonsterKettleManager::RandomMonster(UInt8 idx , UInt8 type , UInt8 count )
    { 
        if( idx >= KETTLE_COUNT)
            return 0;
        return monsterKettle[idx].RandomMonster(idx , type , count);
    } 
    void MonsterKettleManager::BattleMonster(UInt8 idx , UInt8 num)
    { 
        if( idx >= KETTLE_COUNT)
            return ;
        monsterKettle[idx].challenge(m_player,num);
        monsterKettle[idx].UpdateKettleToDB(m_player->getId(),num);
    } 
    void MonsterKettleManager::GetKettleAttr(GData::AttrExtra& ae)
    { 
        for(UInt8 i = 0; i < KETTLE_COUNT ; ++i)
        { 
            for(UInt8 j = 0; j < MONSTER_TYPE ; ++j)
            { 
                 for(UInt8 k = 0; k < MONSTER_KING; ++k)
                     GData::kettleAttrData.GetMonsterAttr(ae,i,j,k,monsterKettle[i].GetMonsterProcess(j,k));
            } 
        } 
    } 
}


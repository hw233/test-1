#include "Evolution.h"
#include "Player.h"
#include "Fighter.h"
#include "Package.h"
#include "Common/URandom.h"
#include "MsgID.h"
#include "Script/GameActionLua.h"
#include "Common/StringTokenizer.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"
#include "Battle/BattleReport.h"
namespace GObject
{
    UInt32 Evolution::npcIds[EVOLUTION_TASKMAX] = {19901,0,0,19901,19912,19902,0,19911,19912};        
    UInt8 Evolution::GetProcess(UInt8 type) 
    { 
        if(type < EVOLUTION_TASKMAX -1)
            return !!(_process & (1<< (type)));
        else 
            return _process >>8;
    } 
    UInt8 Evolution::CompleteTask(UInt8 number , UInt8 task9)
    { 
        if(!getOwner())
            return 1;
        if(number >= EVOLUTION_TASKMAX)
            return 1;
        UInt8 cnt = GetProcess(number);
        if(number < EVOLUTION_TASKMAX-1 && cnt)
            return 1;
        else if(cnt >=5)
            return 1;
        UInt8 res = 1;
        switch(number)   //与客户端ID差1
        {
            case 0:     //战斗
            case 3: 
            case 4:
            case 5: 
            case 7: 
                res = TryTask0(number);
                break;
            case 1:    //修为
                res = TryTask1();
                break;
            case 2:   //收集道具   飞升丹
                res = TryTask2();
                break;
            case 6:    //收集道具   虚无散
                res = TryTask6();
                break;
            case 8:     //5次
                res = TryTask8(task9);
                break;
        }
        if(res == 0 && number!=8)
            _process |= (1<<number);
        UpdateEvolutionToDB();
        SendTaskInfo(number+1);
        return res;    
    } 

    /* ****************** */
    /* *****任务1打怪**** */
    /* ****************** */
    UInt8 Evolution::TryTask0(UInt8 type , UInt32 npcIdEx)
    { 
        if(type >= EVOLUTION_TASKMAX)
            return 1;
        if(!getOwner())
            return 1;

        UInt32 npcId = 0;
        if(!npcIds[type])
            return 1;
        npcId = npcIds[type];
        if(npcIdEx)
            npcId = npcIdEx;
        if(type == 3)
        {
            //std::cout<<"class :" << static_cast<UInt32>(_fighter->getClass()); 
            //std::cout<< "sex :" << static_cast<UInt32>(_fighter->getSex()) << std::endl;
            npcId += (_fighter->getClass()*2 + _fighter->getSex()) ;   //待定
        }
        GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
        if(it == GData::npcGroups.end())
            return 1;
        UInt32 rptid = 0;
        bool res = false;
        GData::NpcGroup * ng = it->second;
        if(!ng)
            return 1;
        Battle::BattleSimulator bsim(0x7000, _fighter->getOwner(), ng->getName(), ng->getLevel(), false);

        if(type == 5)
            _fighter->getOwner()->PutFighters( bsim, 0);
        else
            _fighter->getOwner()->PutFighters( bsim, 0 ,false, _fighter->getId());

        ng->putFighters( bsim );
        bsim.start();
        rptid = bsim.clearLastBattle(1, true);
        res = bsim.getWinner() == 1;

        std::vector<UInt8> *r = Battle::battleReport[rptid];
        if(r == NULL)
            return !res;
        getOwner()->send(&(*r)[0], r->size());

        return !res;         
    } 

    /* ****************** */
    /* *****消耗修为***** */
    /* ****************** */
    UInt8 Evolution::TryTask1()
    { 
        if(!getOwner())
            return 1;
        UInt32 expNeed = 50000;
        if(_fighter->getPExp() < expNeed)
            return 1;
        _fighter->addPExp(-static_cast<Int32>(expNeed), true);
        _process |= (1<<1);
        return 0;
    } 

    /* ****************** */
    /* *****道具收集***** */
    /* ****************** */
    UInt8 Evolution::TryTask2()
    { 
        if(!getOwner())
            return 1;
        UInt32 itemId = 17100;
        UInt16 count = _fighter->getOwner()->GetPackage()->GetItemAnyNum(itemId) ;
        ItemBase * item = _fighter->getOwner()->GetPackage()->FindItem(itemId, true);
        if (!item)
            item =_fighter->getOwner()->GetPackage()->FindItem(itemId, false);
        if(item ==NULL)
            return 1;
        if(!count)
            return 1;
        _fighter->getOwner()->GetPackage()->DelItemAny(itemId, 1 );
        _fighter->getOwner()->GetPackage()->AddItemHistoriesLog(itemId,1 );
        _process |= (1<<2);
        return 0;
    } 

    /* ****************** */
    /* ***击败职业镜像*** */
    /* ****************** */
    UInt8 Evolution::TryTask3()
    { 
        return 0;
    } 

    /* ****************** */
    /* *****全队战斗***** */
    /* ****************** */
    UInt8 Evolution::TryTask9()
    { 
        return 0;

    } 

    /* ****************** */
    /* *****道具收集***** */
    /* ****************** */
    UInt8 Evolution::TryTask6()
    { 
        if(!getOwner())
            return 1;
        UInt32 itemId = 17101;
        UInt16 count = _fighter->getOwner()->GetPackage()->GetItemAnyNum(itemId) ;
        ItemBase * item = _fighter->getOwner()->GetPackage()->FindItem(itemId, true);
        if (!item)
            item =_fighter->getOwner()->GetPackage()->FindItem(itemId, false);
        if(item ==NULL)
            return 1;
        if(!count)
            return 1;
        _fighter->getOwner()->GetPackage()->DelItemAny(itemId, 1 );
        _fighter->getOwner()->GetPackage()->AddItemHistoriesLog(itemId,1 );
        _process |= (1<<6);
        return 0;
    } 

    /* ****************** */
    /* ******火元素****** */
    /* ****************** */
    UInt8 Evolution::TryTask5()
    { 
        return 0;

    } 

    /* ****************** */
    /* *****雷击三次***** */
    /* ****************** */
    UInt8 Evolution::TryTask7()
    { 
        return 0;

    } 

    /* ****************** */
    /* *****轻量pvp****** */
    /* ****************** */
    UInt8 Evolution::TryTask8(UInt8 task9)
    { 
        if(!task9)
            return 0;

        --task9;

        if(task9 >= TASK9_COUNT)
            return 1;
        if(!getOwner())
            return 1;
        if(_task9[task9].win)
            return 1;

        Player * pl = _task9[task9].pl;
        GData::NpcGroup * ng = NULL;
        bool res = false;
        if(!pl)
        {
            res = !TryTask0(8,_task9[task9].npcId);
        }
        //  UInt8 pos = _fighter->getOwner()->getLineupNum(_fighter->getId());
        //  if(pos >= 5)
        //      return 1;
        //  Fighter * fgt = pl->getLineup(pos).fighter;
        //  if(!fgt)
        else
        {
            Fighter* fgt = pl->getMainFighter();
            if(!fgt)
                return 1;
            UInt32 rptid = 0;
            Battle::BattleSimulator bsim(0x7000, _fighter->getOwner(), pl, true);

            _fighter->getOwner()->PutFighters( bsim, 0 ,false, _fighter->getId());
            if(fgt)
                pl->PutFighters(bsim , 1 ,false, fgt->getId());
            else if(ng)
                ng->putFighters( bsim );

            bsim.start();
            rptid = bsim.clearLastBattle(1, true);
            res = bsim.getWinner() == 1;
            std::vector<UInt8> *r = Battle::battleReport[rptid];
            if(r == NULL)
                return !res;
            getOwner()->send(&(*r)[0], r->size());
        }
        if(res)
        {
            _process += (static_cast<UInt32>(1)<< 8);
            _task9[task9].win = 1;
        }

        return !res;
    } 
    void Evolution::UpdateEvolutionToDB()
    { 
        if(!getOwner())
            return ;
        std::string task9 = "";
        for(UInt8 i = 0; i < 3; ++i)
        {
            if(_task9[i].pl)
                task9 += Itoa(_task9[i].pl->getId());
            else
                task9+="0";
            task9 +=",";
            task9 += Itoa(_task9[i].win);
            task9 +=",";
            task9 += Itoa(_task9[i].npcId);
            task9 +="|";
        }
        DB1().PushUpdateData("replace into `fighter_evolution`(fighterId, playerId, process,award, task9, rTime ,success) values(%u, %" I64_FMT "u, %u,%u, '%s', %u , %u)", _fighter->getId(), getOwner()->getId(), _process,_award, task9.c_str(), randomTime,success);
    } 
    void Evolution::getTask9Info(Stream & st)
    { 
        UInt32 now = TimeUtil::Now();
        st << static_cast<UInt32>((randomTime + 60*30) > now ?(randomTime + 60*30 -now): 0);
        //std::cout<<"leftTime:" << ((randomTime + 60*30) > now ?(randomTime + 60*30 -now): 0) << std::endl;;
        st << static_cast<UInt8>(TASK9_COUNT);
        for(UInt8 i = 0 ;i < TASK9_COUNT; ++i)
        { 
            if(!_task9[i].pl || !_task9[i].pl->getMainFighter())
            {
                st << _task9[i].npcId;
                st << static_cast<UInt8>(0);
                st << static_cast<UInt8>(0);
                st << "";
                st << static_cast<UInt8>(0);
                st << static_cast<UInt8>(0);
                st << static_cast<UInt32>(0);
            }
            else
            {
                st << _task9[i].npcId;
                //st << _task9[i].pl->getId();
                st << _task9[i].pl->getMainFighter()->getClass();
                st << _task9[i].pl->getMainFighter()->getSex();
                st << _task9[i].pl->getName();
                st << _task9[i].pl->getMainFighter()->getLevel();
                st << _task9[i].win;
                st << static_cast<UInt32>(_task9[i].pl->getMainFighter()->getBattlePoint());
            }
        } 
    } 
    void Evolution::RandomTask9Player(UInt8 flag)
    { 
        if(!getOwner())
            return ;
        UInt8 level = _fighter->getOwner()->GetLev(); 
        //if( level < 120)
        //    return ;

        if(!flag )
        { 
            UInt32 now = TimeUtil::Now();
            if(randomTime + 1800 > now)
                return ;
            else
                randomTime = now;
        } 
        else
        {
            UInt32 gold = 10; 
            if (getOwner()->getGold() < gold)
                return ;
            ConsumeInfo ci(ExtendPackage,0,0);
            getOwner()->useGold(gold,&ci);
        }

        std::vector<Player * > vec;
        vec.clear();
        for(UInt8 i = level - 3 ; i <= level + 3 ; ++i)
        {
            GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(i);
            if(it != GObject::globalLevelsPlayers.end())
            {
                GObject::LevelPlayers* lvPlayer = it->second;
                UInt8 size = lvPlayer->size();
                for(UInt8 j = 0; j < size; j++)
                {
                    if((*lvPlayer)[j])
                    {
                        Player *p = globalPlayers[(*lvPlayer)[j]];
                        if(p && p != getOwner())
                            vec.push_back(p);
                    }
                }
            } 
        }
        UInt32 size = vec.size();
        UInt32 rand = uRand(size/3);
        for(UInt32 i = 0; i < TASK9_COUNT; ++i)
        {
            //std::cout <<" index: "<< static_cast<UInt32>(i) ;
            UInt32 pos = rand + i*(size/3);
            if(pos >= size)
            {
                _task9[i].npcId = 14003;
                _task9[i].pl =NULL;
                _task9[i].win = 0;
                //std::cout <<" npcId: "<< static_cast<UInt32>(_task9[i].npcId) ;
            }
            else
            {
                _task9[i].pl = vec.at(pos);
                _task9[i].win = 0;
                //std::cout <<" playerId : "<< static_cast<UInt32>(_task9[i].pl->getId()) ;
            }
            //std::cout << std::endl;

        }
        UpdateEvolutionToDB();
        SendTaskInfo(9);
    } 
    bool Evolution::IsComplete()
    { 
        return success;
        return _process == 0x5FF;
    } 
    void Evolution::SendProcess()
    {
        if(!_fighter || !_fighter->getOwner())
            return ;
        Stream st(REP::EXTEND_PROTOCAOL);
        st << static_cast<UInt8>(0x03);
        st << static_cast<UInt8>(0x01);
        st << static_cast<UInt16>(_fighter->getId());
        st << GetProcess();
        st << GetAward();
        st << Stream::eos;
        _fighter->getOwner()->send(st);

    }
    void Evolution::SendTaskInfo(UInt8 taskId)
    { 
        if(!getOwner())
            return ;
        Stream st(REP::EXTEND_PROTOCAOL);
        st << static_cast<UInt8>(0x03);
        st << static_cast<UInt8>(0x02);
        //st << _fighter->getId();
        st << static_cast<UInt16>(_fighter->getId());
        st << taskId;
        switch(taskId)
        { 
            case 1:
            case 5:
            case 6:
            case 8:
                st << npcIds[taskId-1];
                break;
            case 4: 
                st << static_cast<UInt32>(npcIds[taskId-1]+(_fighter->getClass()*2 + _fighter->getSex()));
                break;
            case 2:
                break;
            case 9:
                if(!randomTime)
                    RandomTask9Player(0);
                getTask9Info(st);
        } 
        st << Stream::eos;
        getOwner()->send(st);
    } 
    Player * Evolution::getOwner()
    { 
        if(_fighter)
            return _fighter->getOwner();
        return NULL;
    }
    ItemEquip * Evolution::SetEvolutionEquip(UInt8 pos ,ItemEquip * equip1,UInt8 flag)
    { 
        ItemEquip * old = _evolution[pos];
        if(equip1 && pos != (equip1->getClass() - Item_Evolution1))
            return NULL;
        if(pos >= e_xq_max)
            return NULL;

        if(pos == e_xq_qi)
        {
            if(equip1)
            {
                const GData::AttrExtra * ae = equip1->getAttrExtra(); 
                if(ae && ae->skills.size())
                    fairyskill = ae->skills[0]->getId();
            }
            else
                fairyskill = 0;
        }
        _evolution[pos] = equip1;

        if(!flag)
        {
            if(equip1 != NULL)
            {
                _fighter->CheckEquipEnchantAttainment( equip1->getItemEquipData().enchant);
                equip1->DoEquipBind(true);
            }
            _fighter->sendModification(0x66 + pos, equip1 , false);
            UpdateEquipToDB();
        }
        return old;
    } 
    void Evolution::LoadTask9Info(std::string str)
    { 
        StringTokenizer tokenizer(str, "|");
        if(tokenizer.count() > TASK9_COUNT )
            return ;
        for(size_t j = 0; j < tokenizer.count(); ++ j)  //每一个怪
        {
            StringTokenizer tokenizer2(tokenizer[j], ",");
            if(tokenizer2.count() != 3)
                continue;
            Player *pl = globalPlayers[static_cast<UInt64>(atoi(tokenizer2[0].c_str()))];
            _task9[j].pl = pl;
            _task9[j].win = static_cast<UInt8>(atoi(tokenizer2[1].c_str()));
            _task9[j].npcId = static_cast<UInt8>(atoi(tokenizer2[2].c_str())); 
        }
    } 
    UInt8 Evolution::GetTaskAward(UInt8 index)
    { 
        //static UInt32 AwardExp[EVOLUTION_TASKMAX] = {100,200,300,400,500,600,700,800,900} ;
        static UInt32 Exp = 72320; 
        static UInt32 tael = 1244;
        UInt8  cnt = GetProcess(index);
        if(index < EVOLUTION_TASKMAX-1 )
        {
            if(!cnt)
                return 1;
        }
        else if(cnt < 5)
            return 1;
        if(_award& (1 << index))
            return 1;
        _fighter->addExp(Exp);
        getOwner()->getTael(tael);
        _award|= (1 << index);
        UpdateEvolutionToDB();
        SendProcess();
        return 0;
    } 
    UInt8 Evolution::FeiSheng()
    {
        //if(_fighter->getLevel()<120)
        //    return 0;
        if(!success && _process == 0x5FF)
        {
            setSuccessValue(1);
            UpdateEvolutionToDB();
            return 1;
        }
        return 0;
    }
    void Evolution::UpdateEquipToDB()
    { 
        std::string equipStr = "";
        for(UInt8 i = 0; i < 3; ++i)
        {
            if(_evolution[i])
                equipStr += Itoa(_evolution[i]->getId());
            else
                equipStr+="0";
            if( i < 2)
                equipStr +=",";
        }
        DB1().PushUpdateData("update fighter set evolution = '%s' where id = %u and playerId = %" I64_FMT "u ", equipStr.c_str(),_fighter->getId(), getOwner()->getId());
    } 
   
}

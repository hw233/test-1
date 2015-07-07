#include "BattleSimulator.h"
#include "Script/BattleFormula.h"
#include "BattleFighter.h"
#include "Server/OidGenerator.h"
#include "Battle/BattleWalkFighter.h"
#include "Battle/BattleRideFighter.h"
#include "Battle/BattleShootFighter.h"
#include "Battle/BattleAdviceFighter.h"
#include "Battle/BattleReport.h"
#include "GData/SkillTable.h"
#include "Battle/BattleAnalyze.h"

#define ADVANCE 120
#define ADVANCE_M 140
namespace Battle
{
    BattleSimulator::BattleSimulator(BattleFighter * bf , BattleFighter* bo ,UInt8 distance,UInt8 mapId1, UInt8 mapId2, bool rpt,UInt32 limitTime):BattleField(mapId1, mapId2),_id(IDGenerator::gBattleOidGenerator1.ID()),_formula(NULL/*Script::BattleFormula::getCurrent()*/),_limitTime(limitTime),_distance(distance)
    { 
        _fgt[0] = bf;
        bf->SetSideInBS(0); 
        bf->SetField(this);

        _fgt[1] = bo;
        bo->SetSideInBS(1); 
        bo->SetField(this);
    } 
    void BattleSimulator::InitFighters(UInt8 index ,UInt8 flag)
    { 
        if(index > 1)
            return ;
        if(!_fgt[index])
            return ;
        //UInt8 myFlag = false;

        UInt16 x[] = { ADVANCE + ADVANCE_M ,ADVANCE , 0};
        /*
           if(_fgt[index]->getClass() == shooter || _fgt[index]->getClass() == adviser )
           x[0] = STEP/2;
           if((!flag && index) || (flag && !index))
           { 
           myFlag = true;
           } 
           */

        for(UInt8 i = 0; i <= FIELD_HIGH/(2*STEP); ++i)
        { 
            if( i ==  0)
            {
                //setObjectXY(myFlag?(FIELD_WIDTH -x[0]):x[0] , static_cast<UInt16>(FIELD_HIGH/2),_fgt[index]);
                setObjectXY((!index)?x[0]:FIELD_WIDTH-x[0], static_cast<UInt16>(FIELD_HIGH/2),_fgt[index]);
                //_fgt[index]->SetMinX(x[0]);
                _fighters[index].push_back(_fgt[index]);
                _fgt[index]->resetBattleStatue();
                std::cout << "主将编号：" << static_cast<UInt32>(_fgt[index]->GetBSNumber()) << "位置:" << static_cast<UInt32>(_fgt[index]->getPosX()) << " , " << static_cast<UInt32>(_fgt[index]->getPosY()) << std::endl;
                continue ;
            }

            //fgt1
            BattleFighter* fgt1 =  _fgt[index]->getMyFighters(i*2-1);
            if(!fgt1 || fgt1->getHP() == 0)
                return ;
            //setObjectXY( myFlag?(FIELD_WIDTH -x[2-i%2]):x[2-i%2] , static_cast<UInt16>((FIELD_HIGH/(2 * STEP)+i)*STEP-STEP/2) , fgt1);
            setObjectXY((!index)?x[2-i%2]:(FIELD_WIDTH -x[2-i%2]), static_cast<UInt16>((FIELD_HIGH/(2 * STEP)+i)*STEP/*-STEP/2*/) , fgt1);
            //fgt1->SetMinX(x[2-i%2]);
            fgt1->resetBattleStatue();
            _fighters[index].push_back(fgt1);

            std::cout << "战将编号：" << static_cast<UInt32>(fgt1->GetBSNumber()) << "位置:" << static_cast<UInt32>(fgt1->getPosX()) << " , " << static_cast<UInt32>(fgt1->getPosY()) << std::endl;

            //fgt2
            BattleFighter* fgt2 =  _fgt[index]->getMyFighters(i*2);
            if(!fgt2 || fgt2->getHP() == 0)
                return ;
            setObjectXY( (!index)?x[2-i%2]:FIELD_WIDTH -x[2-i%2] ,static_cast<UInt16>((FIELD_HIGH/(2*STEP) - i)*STEP/*-STEP/2*/) , fgt2);
            std::cout << "战将编号：" << static_cast<UInt32>(fgt2->GetBSNumber()) << "位置:" << static_cast<UInt32>(fgt2->getPosX()) << " , " << static_cast<UInt32>(fgt2->getPosY()) << std::endl;
            //fgt2->SetMinX(x[2-i%2]);
            fgt2->resetBattleStatue();
            _fighters[index].push_back(fgt2);
        } 
    } 

    void BattleSimulator::InitFightersSide(UInt8 index)
    { 
        //XXX
        static UInt16 XPos[][11] = {
            {420,280,160,280,160,280,160,280,160,280,160},
            {1100,1240,1360,1240,1360,1240,1360,1240,1360,1240,1360},
        };

        static UInt16 YPos[][11] = {
            {180,180,180,108,108,252,252,36,36,324,324},
            {180,180,180,108,108,252,252,36,36,324,324},
        };


        if(index > 1)
            return ;
        if(!_fgt[index])
            return ;

        for(UInt8 i = 0; i < 11;++i)
        { 
            BattleFighter * fgt = _fgt[index]->getMyFighters(i);
            if(!fgt || fgt->getHP() == 0)
                return ;
            setObjectXY(XPos[index][i], YPos[index][i], fgt);
            fgt->resetBattleStatue();
            _fighters[index].push_back(fgt);

            std::cout << "战将编号：" << static_cast<UInt32>(fgt->GetBSNumber()) << "位置:" << static_cast<UInt32>(fgt->getPosX()) << " , " << static_cast<UInt32>(fgt->getPosY()) << std::endl;
        } 
    } 

    void BattleSimulator::start(UInt8 prevWin , bool checkEnh)
    { 
        _packet.init(0x81);
        InitFightersSide(0);
        RandPosBegin(0);
        InitFightersSide(1);
        RandPosBegin(1);
        //UInt32 time  = 0;

        GetBSEnterInfo(_packet);

        BattleFighter * bf[2] = {NULL,NULL};
        UInt8 index = 1;
        UInt16 actCount = 0;
        size_t offset = _packet.size();
        _packet << actCount; 
        //for(UInt8 i = 0; i < _limitTime ; ++i)
        {
            //std::cout << "回合数:: " << static_cast<UInt32>(i) << std::endl <<std::endl;
            for(UInt8 j = 0; j < 22; ++j)
            {
                index = !index;
                bf[index] = getMyFighters(index,j/2);

                if(!bf[0] && !bf[1])          
                    break;

                if(!bf[index] || !bf[index]->GetField() || !bf[index]->getHP())
                    continue;
                //if(_distance > bf[index]->GetDistance())
                //    continue;

                //bf[index]->CheckBuff();
                //bf[index]->SetNowTime(i);
                //bf[index]->Action();

                // actCount +=
                //FighterAction(bf[index],0);//->AppendFighterStream(_packet);
                UInt32 rand = uRand(10) ;
                InsertBattlePre(rand * 0.1, bf[index]);
                bf[index]->SetBeginTime(rand*0.1);
                //std::cout << "战报id: " << static_cast<UInt32>(_id) << "包大小：" << static_cast<UInt32>(_packet.size()) << std::endl;
            }

            //UInt8 count = 0;
            float lastTime = 0.1;
            float time = 0;
            while(1)
            {
                //count ++;
                time = GetMinTime();
                std::cout << "Now time:"  << time <<std::endl;
                std::cout << "HP:" <<static_cast<UInt32>(_fgt[0]->getHP()) << std::endl;
                if(time > 20 || GetWin() < 4)
                {
                    break; 
                }
                actCount += FighterMove(0,lastTime,time);
                actCount += FighterMove(1,lastTime,time);
                actCount += doAction(time);
                actCount += doObjectMove(time, (time - lastTime) / 0.1);  //每回合
                actCount += doImage(time);   
                actCount += doAttack(time);

                std::cout << "行动数量" << static_cast<UInt32>(actCount) <<std::endl;
                lastTime = time;
            }
            _time = time;
        }

        actCount += ClearObjectPackage();

        _packet.data<UInt16>(offset) = actCount;
        _packet << Stream::eos;

        battleReport1.addReport(_id,_packet);
        BattleAnalyze::Analyze(_packet);
    } 

    UInt8 BattleSimulator::GetWin()
    { 
        if( _fgt[0] && _fgt[0]->getHP() == 0 && _fgt[1] && _fgt[1]->getHP()==0 )
            return 3;
        if( !_fgt[0] || _fgt[0]->getHP() <= 0 )
            return 1;
        if( !_fgt[1] || _fgt[1]->getHP() <= 0 )
            return 0;
        if(_fgt[0]->IsStoped() || _fgt[1]->IsStoped())
            return 2;
        return -1;
    } 

    BattleFighter* BattleSimulator::CreateFighter(UInt8 Class ,Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY)
    { 
        if(!bf)
            bf = Script::BattleFormula::getCurrent();
        BattleFighter * fgt = NULL;
        switch(Class)
        { 
            case 2:
                fgt = new BattleRideFighter(bf,f,pointX,pointY);
                break;
            case 1:
                fgt = new BattleWalkFighter(bf,f,pointX,pointY);
                break;
            case 3:
                fgt =  new BattleShootFighter(bf,f,pointX,pointY);
                break;
            case 4:
                fgt =  new BattleAdviceFighter(bf,f,pointX,pointY);
                break;
            default:
                fgt =  new BattleRideFighter(bf,f,pointX,pointY);
                break;
        } 
        //fgt = new BattleShootFighter(bf,f,pointX,pointY);
        if(!fgt)
            return NULL;
        fgt->AddSkill();
        return fgt;
    } 

    //物理攻击
    UInt8 BattleSimulator::doAttack(float time)
    { 
        float time1 = 0;
        std::vector<Battle::ActionPackage> vec = GetTimeBattleAction(time1);
        if(time1 > time)
        { 
            return 0;
        } 

        if(!vec.size())
            return 0;
        UInt8 count = 0;
        std::vector<UInt8> vec_flag ;
        vec_flag.clear();
        vec_flag.resize(vec.size());

        for(UInt8 i = 0; i < vec.size(); ++i)
        { 
            BattleFighter * fgt = vec[i].GetBattleFighter();
            if(!fgt || !fgt->getHP())
                vec_flag[i] = 0;
            else
                vec_flag[i] = 1;
        } 
        for(UInt8 i = 0; i < vec.size(); ++i)
        { 
            ActionPackage bAction = vec[i];
            BattleFighter * fgt = bAction.GetBattleFighter();
            if(!vec_flag[i])
                continue; 
            for(UInt8 j = 0; j < bAction.GetObjectSize(); ++j)
            {
                BattleObject * bo = bAction.GetObject(j);
                if(bo->getHP() == 0)
                    continue;
                UInt16 param = bo->BeActed(&bAction);
                _packet << static_cast<UInt16>(bAction.GetHappenTime2() * 100);
                _packet << static_cast<UInt8>(fgt->GetBSNumber());
                _packet << static_cast<UInt8>(1);
                _packet << static_cast<UInt8>(static_cast<BattleFighter*>(bAction.GetObject(j))->GetBSNumber());
                _packet << static_cast<UInt16>(param);
                std::cout << "时间：" << bAction.GetHappenTime2();
                std::cout << "战将编号：" << static_cast<UInt32>(fgt->GetBSNumber());
                std::cout << " 攻击 战将编号:" << static_cast<UInt32>(static_cast<BattleFighter*>(bAction.GetObject(j))->GetBSNumber());
                std::cout << "伤害：" << static_cast<UInt32>(param) << std::endl; 
                if(bo->getHP() == 0)
                    fgt->AddKillCount();
                ++count;
            }
#if 0
            FighterAction(fgt,time1);
            float cd = 0.1;
            const GData::Skill* s = GData::skillManager[fgt->GetBaseActionNum()];
            if(s)
                cd = s->GetActionBackCd();

            InsertBattlePre(time1 + cd, fgt);
#endif
        } 

        DelBattleAction();
        return count;
    } 

    //技能进攻(有动作时间)
    UInt8 BattleSimulator::doImage(float time)
    { 
        float time1 = 0;
        std::vector<Battle::ImagePackage> vec = GetTimeBattleImage(time1);
        if(time1 > time)
        {
            return 0;
        }

        if(!vec.size())
            return 0;
        UInt8 count = 0;
        for(UInt8 i = 0; i < vec.size(); ++i)
        { 
            ImagePackage bAction = vec[i];
            UInt16 skillId = bAction.GetSkillId();
            const GData::Skill* s = GData::skillManager[skillId];
            if(!s)
                continue;
            const GData::SkillEffect* se = s->GetSkillEffect();
            if(!se)
                continue ;

            BattleFighter * fgt = bAction.GetBattleFighter();
            if(!fgt)
                continue;
            _packet << static_cast<UInt16>(bAction.GetHappenTime2() * 100);
            _packet << fgt->GetBSNumber();
            _packet << static_cast<UInt8>(2);
            _packet << static_cast<UInt16>(skillId);

            std::cout << " 回合数" << static_cast<float>(bAction.GetHappenTime2());
            std::cout << " 战将编号:" << static_cast<UInt32>(fgt->GetBSNumber());
            std::cout << " 法术编号:" << static_cast<UInt32>(bAction.GetSkillId()) << std::endl;

            UInt16 buffId = se->buffId;
            const GData::SkillBuff * sb = GData::skillBuffManager[buffId];
            UInt8 side  = 0;
            UInt8 count = 0;
            if(sb)
            {
                side = sb->side;
                count = sb->count;
                //UInt8 type = sb->type;
            }
            if(buffId && count && !side)
                fgt->AddBuff(buffId);


            for(UInt8 j = 0; j < bAction.GetObjectSize(); ++j)
            {
                BattleObject* bo = bAction.GetObject(j);
                if(!bo || !bo->getHP())
                    continue;
                if(buffId && count && side)
                    bo->AddBuff(buffId);
                UInt16 param = bo->BeActed(&bAction);
                //XXX 差法术协议
                _packet << bo->GetBSNumber();
                _packet << static_cast<UInt16>(param);
                //std::cout << " 战将编号:" << static_cast<UInt32>(bo->GetBSNumber());
                //std::cout << " 受伤数值:" << static_cast<UInt32>(param) << std::endl;
                if(!bo->getHP())
                    fgt->AddKillCount();
                ++count;
            }
            float backCd = s->GetActionBackCd();
            if(backCd < 0.1)
                backCd = 0.1;

            //把行动完成的将领放入准备队列
            InsertBattlePre(backCd + time, fgt);
        } 

        DelBattleImage();
        return count;
    } 

    //物体型技能
    UInt8 BattleSimulator::doObjectMove(float time, UInt8 cnt)
    { 
        std::list<ObjectPackage>& lst = GetObjectpackage();
        if(!lst.size())
            return 0;
        std::list<ObjectPackage>::iterator it = lst.begin(); 
        UInt8 count = 0;
        for(;it != lst.end();)
        { 
            bool flag = false;
            for(UInt8 index = 0; index < cnt; ++index)
            {
                if(it->CanExit())
                { 
                    it->BuildStream(_packet);
                    it = lst.erase(it);
                    ++count;
                    flag = true;
                    break;
                } 

                BattleFighter * fgt = it->GetBattleFighter();
                for(UInt8 i = 0; i < _fighters[!fgt->GetSideInBS()].size(); ++i)
                { 
                    BattleFighter * ft = _fighters[!fgt->GetSideInBS()][i];
                    if(it->CheckFighterAttacked(ft))
                        continue;
                    if(it->CheckFighterInSCope(ft)) 
                    { 
                        if(ft->getHP() == 0)
                            continue;
                        UInt16 param = _fighters[!fgt->GetSideInBS()][i]->BeActed(&(*it));
                        it->InsertIntoPackage(time+index*0.1,_fighters[!fgt->GetSideInBS()][i], param);
                        if(ft->getHP() == 0)
                            fgt->AddKillCount();
                    } 
                } 
                it->GoNext();
            }
            if(!flag)
                ++it;
        } 

        return count;
    } 

    UInt8 BattleSimulator::ClearObjectPackage()
    { 
        std::list<ObjectPackage>& lst = GetObjectpackage();
        if(!lst.size())
            return 0;
        std::list<ObjectPackage>::iterator it = lst.begin(); 
        UInt8 count = 0;
        for(;it != lst.end();)
        {
            count += it->BuildStream(_packet);
            it = lst.erase(it);
        }
        return count;
    } 

    //武将行动
    UInt16 BattleSimulator::FighterAction(BattleFighter* bf,float curTime)
    { 
        if(!bf)
            return 0;

        if(!bf || !bf->GetField() || !bf->getHP())
            return 0;
        if(_distance > bf->GetDistance())
            return 0;

        bf->CheckBuff();
        bf->SetNowTime(curTime);
        bf->Action();

        return bf->AppendFighterStream(_packet);
    } 

    UInt8 BattleSimulator::doAction(float time)
    { 
        float time1 = 0;
        std::vector<BattleFighter* > vec = GetBattlePre(time1);
        if(time1 > time)
        {
            return 0;
        }
        UInt8 count = 0;

        if(!vec.size())
            return 0;
        for(UInt8 i = 0; i < vec.size(); ++i)
        { 
            count +=FighterAction(vec[i],time1);
        } 
        DelBattlePre();
        return count;
    } 

    void BattleSimulator::RandPosBegin(UInt8 index)
    { 
        if(index > 1)
            return ;
        if(!_fighters[index].size())
            return ;
        if(_fgt[index]->GetClass() != 2 && _fgt[index]->GetClass() != 1)
            return ;
        for(UInt8 i = 0; i < (_fighters[index].size()-1)/2 ; ++i)
        { 
            UInt32 rand = uRand(99);
            switch(i)
            { 
                case 0:
                    {
                        BattleFighter * fgt = _fighters[index][1+rand/50];
                        setObjectXY(fgt->getPosX(),fgt->getPosY()-STEP,fgt);
                        fgt->SetHighSpeed(false);

                        BattleFighter * fgt1 = _fighters[index][2-rand/50];
                        setObjectXY(fgt1->getPosX(),fgt1->getPosY()+STEP,fgt1);
                        fgt1->SetHighSpeed(true);
                    }
                    break;
                case 1:
                case 3:
                    {
                        BattleFighter * fgt = _fighters[index][i*2 + 1+rand/50];
                        setObjectXY(fgt->getPosX(),fgt->getPosY()-STEP, fgt);
                        _fighters[index][i*2 + 2 - rand/50]->SetHighSpeed(true);
                    }
                    break;
                case 2:
                case 4:
                    {
                        BattleFighter * fgt = _fighters[index][i*2 + 1+rand/50];
                        setObjectXY(fgt->getPosX(),fgt->getPosY()+STEP, fgt);
                        fgt->SetHighSpeed(true);
                    }
                    break;
                default:
                    break;
            } 
        }
    } 
    void BattleSimulator::PosPrintf(UInt8 index)
    { 
        for(UInt8 i = 0; i < 10; ++i)
        { 
            BattleFighter * fgt = _fgt[index]->getMyFighters(i);
            if(!fgt || fgt->getHP() == 0)
                return ;

            std::cout << "战将编号：" << static_cast<UInt32>(fgt->GetBSNumber()) << "位置:" << static_cast<UInt32>(fgt->getPosX()) << " , " << static_cast<UInt32>(fgt->getPosY()) << std::endl;
        } 
    } 

    UInt8 BattleSimulator::FighterMove(UInt8 index,float lastTime, float time)
    { 
        if(index > 1)
            return 0;
        UInt8 cls = _fgt[index]->GetClass();
        if(cls != 2 && cls != 1)
            return 0;
        UInt8 num = 0;
        for(UInt8 i = 0; i < _fighters[index].size(); ++i)
        { 
            if(_fighters[index][i])
            {
                if(_fighters[index][i]->GetBeginTime() + 0.001 > time)
                    continue;
                _fighters[index][i]->GoForward(2 - cls, static_cast<UInt16>((time - lastTime)*_fighters[index][i]->GetSpeed()*10));
                num += _fighters[index][i]->AppendFighterStream(_packet);
            }
        } 
        if(num)
            std::cout << "武将移动数量" << static_cast<UInt32>(num) << std::endl;
        return num;
    } 
}

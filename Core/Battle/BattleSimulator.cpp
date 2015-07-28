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
            setObjectXY((!index)?x[2-i%2]:(FIELD_WIDTH -x[2-i%2]), static_cast<UInt16>((FIELD_HIGH/(2 * STEP)+i)*STEP/*-STEP/2*/) , fgt1);
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
        //输入会+100
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
            setObjectXY(XPos[index][i] + 100, YPos[index][i], fgt);
            fgt->resetBattleStatue();
            _fighters[index].push_back(fgt);

            std::cout << "战将编号：" << static_cast<UInt32>(fgt->GetBSNumber()) << "位置:" << static_cast<UInt32>(fgt->getPosX()) << " , " << static_cast<UInt32>(fgt->getPosY()) << std::endl;
        } 
    } 

    void BattleSimulator::start(UInt8 prevWin , bool checkEnh)
    { 
        _packet.init(0x81);
        InitFightersSide(0);
        InitFightersSide(1);
        //if(_fgt[0]->GetClass() == e_walk && _fgt[1]->GetClass() == e_walk)
        {
            RandPosBegin(0);
            RandPosBegin(1);
        }
        //UInt32 time  = 0;

        GetBSEnterInfo(_packet);

        BattleFighter * bf = NULL;//[2] = {NULL,NULL};
        UInt8 index = 1;
        UInt16 actCount = 0;
        size_t offset = _packet.size();
        _packet << actCount; 
        for(UInt8 i = 0; i < 2 ; ++i)
        {
            for(UInt8 j = 0; j < _fighters[i].size(); ++j)
            {
                //index = !index;
                bf = _fighters[i][j];// getMyFighters(index,j/2);

                if(!bf || !bf->GetField() || !bf->getHP())
                    continue;

                UInt16 rand = (uRand(25)+1)*4;
                InsertBattlePre(rand , bf);
                bf->SetBeginTime(rand);
            }
        }
        {
            //UInt8 count = 0;
            UInt16 lastTime = 0;
            UInt16 time = 0;
            while(1)
            {
                //count ++;
                time = GetMinTime();
                std::cout << "Now time:"  << time <<std::endl;
                std::cout << "HP:" <<static_cast<UInt32>(_fgt[0]->getHP()) << std::endl;
                if(time > 2000 || GetWin() < 4)
                {
                    break; 
                }
                actCount += doAction(time);
                actCount += doObjectMove(time, (time - lastTime)/4);  //每回合
                actCount += doImage(time);   
                actCount += doAttack(time);

                actCount += FighterMove(0,lastTime,time);
                actCount += FighterMove(1,lastTime,time);
                std::cout << "行动数量" << static_cast<UInt32>(actCount) <<std::endl;
                lastTime = time;
            }
            _time = time;
        }

        actCount += ClearObjectPackage();
        std::cout << "行动数量" << static_cast<UInt32>(actCount) <<std::endl;
        FieldPrint();
        std::cout << "战斗结果" << static_cast<UInt32>(GetWin()) <<std::endl;

        _packet.data<UInt16>(offset) = actCount;
        _packet << Stream::eos;

        battleReport1.addReport(_id,_packet);
        BattleAnalyze::Analyze(_packet);
    } 

    UInt8 BattleSimulator::GetWin()
    { 
        if( _fgt[0] && _fgt[0]->getHP() == 0 && _fgt[1] && _fgt[1]->getHP()==0 )
            return 3;
        if( !_fgt[0] || !_fgt[0]->getHP())
            return 1;
        if( !_fgt[1] || !_fgt[1]->getHP())
            return 0;
        if(GetStop())
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
        return fgt;
    } 

    //物理攻击
    UInt8 BattleSimulator::doAttack(UInt16 time)
    { 
        UInt16 time1 = 0;
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
                BattleFighter * bo = static_cast<BattleFighter*>(bAction.GetObject(j));
                if(bo->getHP() == 0 || bo->GetAvoidHurt())
                    continue;
                UInt16 param = bo->BeActed(&bAction);
                _packet << static_cast<UInt16>(bAction.GetHappenTime());
                _packet << static_cast<UInt8>(fgt->GetBSNumber());
                _packet << static_cast<UInt8>(1);
                _packet << static_cast<UInt8>(static_cast<BattleFighter*>(bAction.GetObject(j))->GetBSNumber());
                _packet << static_cast<UInt16>(param);
                std::cout << "时间：" << bAction.GetHappenTime();
                std::cout << "战将编号：" << static_cast<UInt32>(fgt->GetBSNumber());
                std::cout << " 攻击 战将编号:" << static_cast<UInt32>(static_cast<BattleFighter*>(bAction.GetObject(j))->GetBSNumber());
                std::cout << "伤害：" << static_cast<UInt32>(param) << std::endl; 
                std::cout << "敌将血量剩余：" << static_cast<UInt32>(bo->getHP()) << std::endl; 
                if(bo->getHP() == 0)
                {
                    if(bo->IsMainFighter())
                        fgt->AddKillCount1();
                    else
                        fgt->AddKillCount2();

                }
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
    UInt8 BattleSimulator::doImage(UInt16 time)
    { 
        UInt16 time1 = 0;
        std::vector<Battle::ImagePackage> vec = GetTimeBattleImage(time1);
        if(time1 > time)
        {
            return 0;
        }

        if(!vec.size())
            return 0;
        UInt8 cnt = 0;
        for(UInt8 i = 0; i < vec.size(); ++i)
        { 
            ImagePackage bAction = vec[i];
            UInt16 skillId = bAction.GetSkillId();
            const GData::Skill* s = GData::skillManager[skillId];
            if(!s)
                continue;
            if(GetSuperSkill() && s->GetSuperSkill())
                SetSuperSkill(false);
            const GData::SkillEffect* se = s->GetSkillEffect();
            if(!se)
                continue ;


            UInt8 skillType = se->skillType;

            BattleFighter * fgt = bAction.GetBattleFighter();
            if(!fgt)
                continue;
            //技能无敌持续到下一个动作
            //if(se->avoidhurt)
            //    fgt->SetAvoidHurt(false);
            _packet << static_cast<UInt16>(bAction.GetHappenTime());
            _packet << fgt->GetBSNumber();
            _packet << static_cast<UInt8>(2);
            _packet << static_cast<UInt16>(skillId);
            cnt++;

            std::cout << " 回合数" << static_cast<UInt32>(bAction.GetHappenTime());
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

            size_t offset = _packet.size();

            UInt8 infectCnt = 0;

            _packet << static_cast<UInt8>(infectCnt);

            for(UInt8 j = 0; j < bAction.GetObjectSize(); ++j)
            {
                BattleFighter* bo = static_cast<BattleFighter* >(bAction.GetObject(j));
                if(!bo || !bo->getHP() || bo->GetAvoidHurt())
                    continue;
                if(buffId && count && side)
                    bo->AddBuff(buffId);
                UInt16 param = bo->BeActed(&bAction);
                //XXX 差法术协议
                if(skillType == 6)
                    _packet << static_cast<UInt8>(1);
                else
                    _packet << static_cast<UInt8>(0);
                _packet << bo->GetBSNumber();
                _packet << static_cast<UInt16>(param);
                if(!bo->getHP())
                {
                    if(bo->IsMainFighter())
                        fgt->AddKillCount1();
                    else
                        fgt->AddKillCount2();
                }
                //++count;

                std::cout << "#####战将编号：" << static_cast<UInt32>(bo->GetBSNumber()) << "被法术攻击" << std::endl;
                ++infectCnt; 
            }

            _packet.data<UInt8>(offset) = infectCnt;

            UInt16 backCd = s->GetActionBackCd();

            //把行动完成的将领放入准备队列
            InsertBattlePre(backCd + time, fgt);
        } 

        DelBattleImage();
        return cnt;
    } 

    //物体型技能
    UInt8 BattleSimulator::doObjectMove(UInt16 time, UInt8 cnt)
    { 
        std::list<ObjectPackage>& lst = FieldObject;//GetObjectpackage();
        if(!lst.size())
            return 0;
        std::list<ObjectPackage>::iterator it = lst.begin(); 
        UInt8 count = 0;
        for(;it != lst.end();)
        { 
            if(it->GetBattleFighter() && !it->GetBattleFighter()->getHP() && !it->GetBeing())
            {
                it = lst.erase(it);
                continue;
            }
            else
                it->SetBeing(true);

            bool flag = false;
            for(UInt8 index = 0; index < cnt; ++index)
            {
                BattleFighter * fgt = it->GetBattleFighter();
                BattleFighter * target = it->GetTargetFighter();

                //std::cout << "####### 粒子型技能 移动 : 技能释放者=="  << static_cast<UInt32>(fgt->GetBSNumber()) << " 位置 ：" << static_cast<UInt32>(it->GetPosX()) << " , " << static_cast<UInt32>(it->GetPosY()) << std::endl;

                for(UInt8 i = 0; i < _fighters[!fgt->GetSideInBS()].size(); ++i)
                { 
                    BattleFighter * ft = _fighters[!fgt->GetSideInBS()][i];
                    if(target)
                        ft = target;
                    if(it->CheckFighterAttacked(ft))
                        continue;
                    if(it->CheckFighterInSCope(ft)) 
                    { 
                        if(ft->getHP() == 0)
                            continue;
                        UInt16 param = ft->BeActed(&(*it));
                        it->InsertIntoPackage(time+index*4,ft, param);
                        if(ft->getHP() == 0)
                        {
                            if(ft->IsMainFighter())
                                fgt->AddKillCount1();
                            else
                                fgt->AddKillCount2();

                        }
                        std::cout << "时间点：" << static_cast<UInt32>(time) ;
                        std::cout << " 粒子型技能造成伤害 " ;
                        std::cout << " 技能释放者：" << static_cast<UInt32>(fgt->GetBSNumber());
                        std::cout << " 技能受击者：" << static_cast<UInt32>(ft->GetBSNumber());
                        std::cout << std::endl;
                    } 
                    if(target)
                        break;
                } 

                if(it->CanExit())
                { 
                    count += it->BuildStream(_packet);
                    UInt16 skillId = it->GetSkillId();
                    it = lst.erase(it);
                    flag = true;
                    break;
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
        std::list<ObjectPackage> lst = GetObjectpackage();
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
    UInt16 BattleSimulator::FighterAction(BattleFighter* bf,UInt16 curTime)
    { 
        if(!bf)
            return 0;

        if(!bf || !bf->GetField() || !bf->getHP())
            return 0;
        if(_distance > bf->GetAttackRange())
            return 0;

        bf->SetNowTime(curTime);
        bf->CheckBuff();
        bf->Action();
        return bf->AppendFighterStream(_packet);
    } 

    UInt8 BattleSimulator::doAction(UInt16 time)
    { 
        UInt16 time1 = 0;
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
        } 
    } 

    UInt8 BattleSimulator::FighterMove(UInt8 index,UInt16 lastTime, UInt16 time)
    { 
        if(index > 1)
            return 0;
        UInt8 cls = _fgt[index]->GetClass();
        if(cls != 2 && cls != 1)
            return 0;
        if(time <= lastTime)
            return 0;
        UInt8 num = 0;
        for(UInt8 i = 0; i < _fighters[index].size(); ++i)
        { 
            BattleFighter * fgt = _fighters[index][i];
            if(_distance > fgt->GetAttackRange())
                continue ;
            if(!fgt->getHP())
                continue;
            if(fgt && fgt->CanMove())
            {
                if(fgt->GetBeginTime() > time)
                    continue;
                fgt->SetNowTime(time);
                UInt16 advance = ((time-lastTime)*fgt->GetSpeed()) + fgt->GetCachePx() ;
                fgt->GoForward(2 - cls, advance/100);
                fgt->SetCachePx(advance % 100);
                num += fgt->AppendFighterStream(_packet);
            }
        } 
        return num;
    } 
    bool BattleSimulator::GetStop()
    { 
        for(UInt8 i = 0; i < 2 ; ++i)
        { 
            bool flag = true;
            for(UInt8 j = 0; j < _fighters[i].size(); ++j)
            { 
                if(!_fighters[i][j]->IsStoped())
                {
                    flag = false;
                    break;
                }
            } 
            if(flag)
                return true;
        } 
        return false;
    } 
}

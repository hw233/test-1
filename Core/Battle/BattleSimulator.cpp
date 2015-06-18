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

#define ADVANCE 120
#define ADVANCE_M 140
namespace Battle
{
    BattleSimulator::BattleSimulator(BattleFighter * bf , BattleFighter* bo ,UInt8 distance, bool rpt,UInt32 limitTime):BattleField(),_id(IDGenerator::gBattleOidGenerator1.ID()),_formula(NULL/*Script::BattleFormula::getCurrent()*/),_limitTime(limitTime),_distance(distance)
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
            {260,120,0,120,0,120,0,120,0,120,0},
            {940,1080,1200,1080,1200,1080,1200,1080,1200,1080,1200},
        };

        static UInt16 YPos[][11] = {
            {180,180,180,252,252,108,108,364,364,36,36},
            {180,180,180,252,252,108,108,364,364,36,36},
        };


        if(index > 1)
            return ;
        if(!_fgt[index])
            return ;

        for(UInt8 i = 0; i < 10;++i)
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
            for(UInt8 j = 0; j < 20; ++j)
            {
                index = !index;
                bf[index] = getMyFighters(index,j/2);

                if(!bf[0] && !bf[1])          
                    break;

                //if(!bf[index] || !bf[index]->GetField() || !bf[index]->getHP())
                //    continue;
                //if(_distance > bf[index]->GetDistance())
                //    continue;

                //bf[index]->CheckBuff();
                //bf[index]->SetNowTime(i);
                //bf[index]->Action();

                // actCount +=
                //FighterAction(bf[index],0);//->AppendFighterStream(_packet);
                UInt32 rand = uRand(3);
                InsertBattlePre(rand * 0.1, bf[index]);
                //std::cout << "战报id: " << static_cast<UInt32>(_id) << "包大小：" << static_cast<UInt32>(_packet.size()) << std::endl;
            }

            UInt8 count = 0;
            float time = 0;
            while(count< 40)
            {
                count ++;
                time = GetMinTime();
                std::cout << "Now time:"  << time <<std::endl;
                if(time > 20 || GetWin() < 3)
                {
                    break; 
                }
                actCount += doAction(time);
                actCount += doObjectMove(time);  //每回合
                actCount += doImage(time);   
                actCount += doAttack(time);
            }
        }

        actCount += ClearObjectPackage();

        _packet.data<UInt16>(offset) = actCount;
        _packet << Stream::eos;

        battleReport1.addReport(_id,_packet);
    } 

    UInt8 BattleSimulator::GetWin()
    { 
        if(_fgt[0]&& _fgt[0]->getHP() == 0 && _fgt[1] && _fgt[1]->getHP() == 0)
            return 2;
        if(!_fgt[0] || !_fgt[0]->getHP())
            return 0;
        if(!_fgt[1] || !_fgt[1]->getHP())
            return 1;
        return 3;
    } 

    BattleFighter* BattleSimulator::CreateFighter(UInt8 Class ,Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY)
    { 
        if(!bf)
            bf = Script::BattleFormula::getCurrent();
        BattleFighter * fgt = NULL;
        switch(Class)
        { 
            case 1:
                fgt = new BattleRideFighter(bf,f,pointX,pointY);
            case 2:
                fgt = new BattleWalkFighter(bf,f,pointX,pointY);
            case 3:
                fgt =  new BattleShootFighter(bf,f,pointX,pointY);
            case 4:
                fgt =  new BattleAdviceFighter(bf,f,pointX,pointY);
            default:
                fgt =  new BattleRideFighter(bf,f,pointX,pointY);
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
            return 0;

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
                UInt16 param = bAction.GetObject(j)->BeActed(&bAction);
                _packet << static_cast<UInt8>(bAction.GetHappenTime());
                _packet << fgt->GetBSNumber();
                _packet << static_cast<UInt8>(1);
                _packet << static_cast<BattleFighter*>(bAction.GetObject(j))->GetBSNumber();
                _packet << static_cast<UInt16>(param);
                ++count;
            }
            FighterAction(fgt,time1);
            float cd = 0.1;
            const GData::Skill* s = GData::skillManager[fgt->GetBaseActionNum()];
            if(s)
                cd = s->GetActionBackCd();

            InsertBattlePre(time1 + cd, fgt);
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
            return 0;

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
            _packet << static_cast<UInt8>(bAction.GetHappenTime());
            _packet << fgt->GetBSNumber();
            _packet << static_cast<UInt8>(2);
            _packet << static_cast<UInt16>(skillId);

            //std::cout << " 回合数" << static_cast<UInt32>(bAction.GetHappenTime());
            //std::cout << " 战将编号:" << static_cast<UInt32>(fgt->GetBSNumber());
            //std::cout << " 法术编号:" << static_cast<UInt32>(bAction.GetSkillId()) << std::endl;

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
                if(!bo)
                    continue;
                if(buffId && count && side)
                    bo->AddBuff(buffId);
                UInt16 param = bo->BeActed(&bAction);
                //XXX 差法术协议
                _packet << bo->GetBSNumber();
                _packet << static_cast<UInt16>(param);
                //std::cout << " 战将编号:" << static_cast<UInt32>(bo->GetBSNumber());
                //std::cout << " 受伤数值:" << static_cast<UInt32>(param) << std::endl;
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
    UInt8 BattleSimulator::doObjectMove(float time)
    { 
        const float oneTime = 0.1;
        UInt8 cnt = time / oneTime;
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
                    if(it->CheckFighterInSCope(_fighters[!fgt->GetSideInBS()][i])) 
                    { 
                        UInt16 param = _fighters[!fgt->GetSideInBS()][i]->BeActed(&(*it));
                        it->InsertIntoPackage(time+index*oneTime,_fighters[!fgt->GetSideInBS()][i], param);

                        // _packet << static_cast<UInt8>(it->GetHappenTime());
                        // _packet << fgt->GetBSNumber();
                        // _packet << static_cast<UInt8>(2);
                        // _packet << static_cast<UInt16>(it->GetSkillId()) << static_cast<UInt8>(1);
                        // _packet << _fighters[!fgt->GetSideInBS()][i]->GetBSNumber();
                        // _packet << static_cast<UInt16>(param);
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
            return 1;

        if(!vec.size())
            return 1;
        for(UInt8 i = 0; i < vec.size(); ++i)
        { 
            FighterAction(vec[i],time1);
        } 
        DelBattlePre();
        return 0;
    } 

    void BattleSimulator::RandPosBegin(UInt8 index)
    { 
        if(index > 1)
            return ;
        if(_fgt[index]->GetClass() != 2 && _fgt[index]->GetClass() != 1)
            return ;
        for(UInt8 i = 0; i < 5 ; ++i)
        { 
            UInt32 rand = uRand(99);
            switch(i)
            { 
                case 0:
                    {
                        BattleFighter * fgt = _fighters[index][1+rand/50];
                        setObjectXY(fgt->getPosX(),fgt->getPosY()-STEP,fgt);

                        BattleFighter * fgt1 = _fighters[index][2-rand/50];
                        setObjectXY(fgt1->getPosX(),fgt1->getPosY()+STEP,fgt1);
                    }
                    break;
                case 1:
                case 3:
                    {
                        BattleFighter * fgt = _fighters[index][3+rand/50];
                        setObjectXY(fgt->getPosX(),fgt->getPosY()+STEP, fgt);
                    }
                    break;
                case 2:
                case 4:
                    {
                        BattleFighter * fgt = _fighters[index][3+rand/50];
                        setObjectXY(fgt->getPosX(),fgt->getPosY()-STEP, fgt);
                    }
                    break;
                default:
                    break;
            } 
        }
    } 
}

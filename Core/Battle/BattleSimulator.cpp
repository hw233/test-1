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

        _attackUp[0] = 0;
        _attackUp[1] = 0;
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
                //COUT << "主将编号：" << static_cast<UInt32>(_fgt[index]->GetBSNumber()) << "位置:" << static_cast<UInt32>(_fgt[index]->getPosX()) << " , " << static_cast<UInt32>(_fgt[index]->getPosY()) << std::endl;
                continue ;
            }

            //fgt1
            BattleFighter* fgt1 =  _fgt[index]->getMyFighters(i*2-1);
            if(!fgt1 || fgt1->getHP() == 0)
                return ;
            setObjectXY((!index)?x[2-i%2]:(FIELD_WIDTH -x[2-i%2]), static_cast<UInt16>((FIELD_HIGH/(2 * STEP)+i)*STEP/*-STEP/2*/) , fgt1);
            fgt1->resetBattleStatue();
            _fighters[index].push_back(fgt1);

            //COUT << "战将编号：" << static_cast<UInt32>(fgt1->GetBSNumber()) << "位置:" << static_cast<UInt32>(fgt1->getPosX()) << " , " << static_cast<UInt32>(fgt1->getPosY()) << std::endl;

            //fgt2
            BattleFighter* fgt2 =  _fgt[index]->getMyFighters(i*2);
            if(!fgt2 || fgt2->getHP() == 0)
                return ;
            setObjectXY( (!index)?x[2-i%2]:FIELD_WIDTH -x[2-i%2] ,static_cast<UInt16>((FIELD_HIGH/(2*STEP) - i)*STEP/*-STEP/2*/) , fgt2);
            //COUT << "战将编号：" << static_cast<UInt32>(fgt2->GetBSNumber()) << "位置:" << static_cast<UInt32>(fgt2->getPosX()) << " , " << static_cast<UInt32>(fgt2->getPosY()) << std::endl;
            //fgt2->SetMinX(x[2-i%2]);
            fgt2->resetBattleStatue();
            _fighters[index].push_back(fgt2);
        } 
    } 

    void BattleSimulator::InitFightersSide(UInt8 index)
    { 
        //XXX
        //输入会+100 站位点
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
            fgt->ResetSkill();
            setObjectXY(XPos[index][i] + 100, YPos[index][i], fgt);
            fgt->resetBattleStatue();
            _fighters[index].push_back(fgt);

            //COUT << "战将编号：" << static_cast<UInt32>(fgt->GetBSNumber()) << "位置:" << static_cast<UInt32>(fgt->getPosX()) << " , " << static_cast<UInt32>(fgt->getPosY()) << std::endl;
        } 
    } 

    void BattleSimulator::start(UInt8 prevWin , bool checkEnh)
    { 
        _packet.init(0x81);
        InitFightersSide(0);  //攻方入场
        InitFightersSide(1);  //守方入场
        //if(_fgt[0]->GetClass() == e_walk && _fgt[1]->GetClass() == e_walk)
        {
            RandPosBegin(0); //角色偏移（步兵骑兵防止己方前后同Y轴）
            RandPosBegin(1);
        }
        //UInt32 time  = 0;
        //战力提升
        SetAttackUp();  

        _packet << static_cast<UInt8>(_distance);
        GetBSEnterInfo(_packet);  //入场信息

        BattleFighter * bf = NULL;//[2] = {NULL,NULL};
        UInt8 index = 1;
        UInt16 actCount = 0;
        size_t offset = _packet.size();
        _packet << actCount; 

        //设置各角色的起始时间轴
        for(UInt8 i = 0; i < 2 ; ++i)
        {
            for(UInt8 j = 0; j < _fighters[i].size(); ++j)
            {
                //index = !index;
                bf = _fighters[i][j];// getMyFighters(index,j/2);

                if(!bf || !bf->GetField() || !bf->getHP())
                    continue;

                //角色攻击距离不足战斗的攻击距离
                if(_distance > bf->GetAttackRange())
                    continue ;
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

                //获取时间轴的最小时间点
                time = GetMinTime();  
                //COUT << "Now time:"  << time <<std::endl;
                //COUT << "HP:" <<static_cast<UInt32>(_fgt[0]->getHP()) << std::endl;
                if(time > 2000 || GetWin() < 4)
                {
                    break; 
                }
                //随着时间积累能量
                EnergyUp(time-lastTime);

                actCount += doAction(time);
                actCount += doObjectMove(time, (time - lastTime)/4);  //每回合
                actCount += doImage(time);   
                actCount += doAttack(time);

                actCount += FighterMove(0,lastTime,time);
                actCount += FighterMove(1,lastTime,time);
                //COUT << "行动数量" << static_cast<UInt32>(actCount) <<std::endl;
                lastTime = time;
            }
            _time = time;
        }

        actCount += ClearObjectPackage();
        //COUT << "行动数量" << static_cast<UInt32>(actCount) <<std::endl;
        FieldPrint();
        //COUT << "战斗结果" << static_cast<UInt32>(GetWin()) <<std::endl;

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
        if(!f)
            return NULL;
        if(!bf)
            bf = Script::BattleFormula::getCurrent();
        BattleFighter * fgt = NULL;
        switch(Class)
        { 
            case 2:
                fgt = new BattleRideFighter(bf,f,pointX,pointY);
                break;
            case 1:
            case 5:
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

        //遍历之前先判断是否又零值 (防止因为遍历顺序导致无法同时死亡)
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

            //可能会攻击多个人
            for(UInt8 j = 0; j < bAction.GetObjectSize(); ++j)
            {
                BattleFighter * bo = static_cast<BattleFighter*>(bAction.GetObject(j));
                if(bo->getHP() == 0 || bo->GetAvoidHurt())
                    continue;
                UInt32 param = bo->BeActed(&bAction);

                //普通攻击的协议格式
                _packet << static_cast<UInt16>(bAction.GetHappenTime());
                _packet << static_cast<UInt8>(fgt->GetBSNumber());
                _packet << static_cast<UInt8>(1);
                _packet << static_cast<UInt16>(bAction.GetSkillId());
                _packet << static_cast<UInt8>(static_cast<BattleFighter*>(bAction.GetObject(j))->GetBSNumber());
                //param = 伤害状态{躲避，格挡、、、} << 16 | 伤害值
                _packet << static_cast<UInt8>(param >> 16);
                _packet << static_cast<UInt16>(param);

                //COUT << "时间：" << bAction.GetHappenTime();
                //COUT << "战将编号：" << static_cast<UInt32>(fgt->GetBSNumber());
                //COUT << " 攻击 战将编号:" << static_cast<UInt32>(static_cast<BattleFighter*>(bAction.GetObject(j))->GetBSNumber());
                //COUT << "伤害：" << static_cast<UInt32>(param) << std::endl; 
                //COUT << "敌将血量剩余：" << static_cast<UInt32>(bo->getHP()) << std::endl; 
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

            //场地大招效果产生后，标识重置为false #BattleField::GetSuperSkill 与 Skill::GetSuperSkill 同名
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

            //COUT << " 回合数" << static_cast<UInt32>(bAction.GetHappenTime());
            //COUT << " 战将编号:" << static_cast<UInt32>(fgt->GetBSNumber());
            //COUT << " 法术编号:" << static_cast<UInt32>(bAction.GetSkillId()) << std::endl;

            //技能buf
            UInt16 buffId = se->buffId;
            const GData::SkillBuff * sb = GData::skillBuffManager[buffId];
            UInt8 side  = 0;
            UInt8 count = 0;
            if(sb)
            {
                side = sb->side; //buf作用方
                count = sb->count; //buf作用时间
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

                //param = 伤害状态{躲避，格挡、、、} << 16 | 伤害值
                UInt32 param = bo->BeActed(&bAction);

                if(skillType == e_image_therapy)
                    _packet << static_cast<UInt8>(1);
                else
                    _packet << static_cast<UInt8>(0);
                _packet << bo->GetBSNumber();
                _packet << static_cast<UInt8>(param >> 16);
                _packet << static_cast<UInt16>(param);
                if(!bo->getHP())
                {
                    //小兵，武将斩杀数
                    if(bo->IsMainFighter())
                        fgt->AddKillCount1();
                    else
                        fgt->AddKillCount2();
                }
                //++count;

                //COUT << "#####战将编号：" << static_cast<UInt32>(bo->GetBSNumber()) << "被法术攻击" << std::endl;
                ++infectCnt; 
            }

            _packet.data<UInt8>(offset) = infectCnt;

            //僵直时间
            UInt16 backCd = s->GetActionBackCd();

            //把行动完成的将领放入准备队列
            InsertBattlePre(backCd + time, fgt);
        } 

        DelBattleImage();
        return cnt;
    } 

    //当前时间点物体型技能(释放飞行物)的行动以及效果
    UInt8 BattleSimulator::doObjectMove(UInt16 time, UInt8 cnt)
    { 
        std::list<ObjectPackage>& lst = FieldObject;//GetObjectpackage();
        if(!lst.size())
            return 0;
        std::list<ObjectPackage>::iterator it = lst.begin(); 
        UInt8 count = 0;
        for(;it != lst.end();)
        { 
            BattleFighter * fgt = it->GetBattleFighter();
            if(!fgt)
            { 
                it = lst.erase(it);
                continue;
            } 

            if(it->GetBeing() > time)
            {
                //释放者死亡 （飞行体消失）
                if(!fgt->getHP())
                    it = lst.erase(it);
                else
                    ++it;
                continue;
            }

            bool flag = false;
            for(UInt8 index = 0; index < cnt; ++index)
            {
                BattleFighter * target = it->GetTargetFighter();

                ////COUT << "####### 粒子型技能 移动 : 技能释放者=="  << static_cast<UInt32>(fgt->GetBSNumber()) << " 位置 ：" << static_cast<UInt32>(it->GetPosX()) << " , " << static_cast<UInt32>(it->GetPosY()) << std::endl;

                for(UInt8 i = 0; i < _fighters[!fgt->GetSideInBS()].size(); ++i)
                { 
                    BattleFighter * ft = _fighters[!fgt->GetSideInBS()][i];
                    if(target)
                        ft = target;
                    //检查是否能攻击到对方（对方是否无敌）
                    if(it->CheckFighterAttacked(ft))
                        continue;
                    //检查对方是否在攻击的范围内
                    if(it->CheckFighterInSCope(ft)) 
                    { 
                        if(ft->getHP() == 0)
                            continue;
                        UInt32 param = ft->BeActed(&(*it));

                        //伤害产生放入本技能的伤害列表 （统一写入协议）
                        it->InsertIntoPackage(time+index*4,ft, param);
                        if(ft->getHP() == 0)
                        {
                            if(ft->IsMainFighter())
                                fgt->AddKillCount1();
                            else
                                fgt->AddKillCount2();

                        }
                        //COUT << "时间点：" << static_cast<UInt32>(time) ;
                        //COUT << " 粒子型技能造成伤害 " ;
                        //COUT << " 技能释放者：" << static_cast<UInt32>(fgt->GetBSNumber());
                        //COUT << " 技能受击者：" << static_cast<UInt32>(ft->GetBSNumber());
                        //COUT << std::endl;
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
                //粒子型技能前进
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

    //完成当前时间点攻击队列的攻击行为
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


        //为防止所有人在中间相遇，有根据y值设置速度
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
    void BattleSimulator::SetAttackUp()
    { 
        static UInt8 groundUp[][5] = {
        {0,25,0,0,0},
        {25,0,0,10,25},
        {10,0,25,25,25},
        {10,0,25,10,0},
        };

        static UInt8 armyUp[e_fighter_max][e_fighter_max] = {0};

        armyUp[e_walk][e_lance] = 50;
        armyUp[e_ride][e_walk] = 50;
        armyUp[e_lance][e_ride] = 50;

        if(!_fgt[0] || !_fgt[1])
            return ;

        UInt8 type1 = _fgt[0]->GetTypeId();
        UInt8 type2 = _fgt[1]->GetTypeId();

        if((_mapId1 < 4 && _mapId1 != 0) && (type1 < e_fighter_max && type1 != e_fighter))
        {
            _attackUp[0] = groundUp[_mapId1-1][type1-1];
        }

        if((_mapId2 < 4 && _mapId2 != 0) && (type2 < e_fighter_max && _fgt[1]->GetTypeId() != e_fighter))
        {
            _attackUp[1] = groundUp[_mapId1-1][_fgt[0]->GetTypeId()-1];
        }

        _attackUp[0] += armyUp[type1][type2];
        _attackUp[1] += armyUp[type2][type1];

    } 
    void BattleSimulator::EnergyUp(UInt32 time)
    { 
       _fgt[0]->AddEnergy(time/4);
       _fgt[1]->AddEnergy(time/4);
    } 
}

#include "BattleSimulator.h"
#include "Script/BattleFormula.h"
#include "BattleFighter.h"
#include "Server/OidGenerator.h"
#include "Battle/BattleWalkFighter.h"
#include "Battle/BattleRideFighter.h"
#include "Battle/BattleReport.h"

#define ADVANCE 180
#define ADVANCE_M 220
namespace Battle
{
    BattleSimulator::BattleSimulator(BattleFighter * bf , BattleFighter* bo , bool rpt,UInt32 limitTime):BattleField(),_id(IDGenerator::gBattleOidGenerator.ID()),_formula(NULL/*Script::BattleFormula::getCurrent()*/),_limitTime(limitTime)
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
        UInt8 myFlag = false;

        UInt16 x[] = { ADVANCE + ADVANCE_M ,ADVANCE , 0};
        if(_fgt[index]->getClass() == shooter || _fgt[index]->getClass() == adviser )
            x[0] = STEP/2;
        if((!flag && index) || (flag && !index))
        { 
            myFlag = true;
        } 

        for(UInt8 i = 0; i < FIELD_HIGH/(2*STEP); ++i)
        { 
            if( i ==  0)
            {
                setObjectXY(myFlag?(FIELD_WIDTH -x[0]):x[0] , static_cast<UInt16>(FIELD_HIGH/2),_fgt[index]);
                //_fgt[index]->SetMinX(x[0]);
                _fighters[index].push_back(_fgt[index]);
                _fgt[index]->resetBattleStatue();
                continue ;
            }

            //fgt1
            BattleFighter* fgt1 =  _fgt[index]->getMyFighters(i*2-1);
            if(!fgt1 || fgt1->getHP() == 0)
                return ;
            setObjectXY( myFlag?(FIELD_WIDTH -x[2-i%2]):x[2-i%2] , static_cast<UInt16>((FIELD_HIGH/(2 * STEP)+i)*STEP-STEP/2) , fgt1);
            //fgt1->SetMinX(x[2-i%2]);
            fgt1->resetBattleStatue();
            _fighters[index].push_back(fgt1);

            //fgt2
            BattleFighter* fgt2 =  _fgt[index]->getMyFighters(i*2);
            if(!fgt2 || fgt2->getHP() == 0)
                return ;
            setObjectXY( myFlag?FIELD_WIDTH -x[2-i%2]:x[2-i%2] ,static_cast<UInt16>((FIELD_HIGH/(2*STEP) - i)*STEP-STEP/2) , fgt2);
            //fgt2->SetMinX(x[2-i%2]);
            fgt2->resetBattleStatue();
            _fighters[index].push_back(fgt2);
        } 
    } 
    void BattleSimulator::start(UInt8 prevWin , bool checkEnh)
    { 
        _packet.init(0x81);
        InitFighters(0);
        InitFighters(1);
        //UInt32 time  = 0;

        GetBSEnterInfo(_packet);

        BattleFighter * bf[2] = {NULL,NULL};
        //UInt8 act = 0;
        UInt8 index = 1;
        //UInt32 curTime = 0;
        UInt16 actCount = 0;
        size_t offset = _packet.size();
        _packet << actCount; 
        for(UInt8 i = 0; i < _limitTime ; ++i)
        //while(curTime <= _limitTime && GetWin() == 2  )
        {
            std::cout << "回合数:: " << static_cast<UInt32>(i) << std::endl <<std::endl;
            actCount += doImage(i);
            actCount += doObjectMove();
            for(UInt8 j = 0; j < 20; ++j)
            {
                /*
                if(act == 0)
                { 
                    actCount += doImage(curTime);
                    actCount += doObjectMove();
                } 
                if(act > 20)
                {
                    actCount += doAttack(curTime);
                    ++curTime;
                    act = 0;
                    continue;
                }
                */
                index = !index;
                bf[index] = getMyFighters(index,j/2);


                if(!bf[0] && !bf[1])          
                {
                    //actCount += doAttack(curTime);
                    //++curTime;
                    //act = 0;
                    //continue;
                    break;
                }

                if(!bf[index] || !bf[index]->GetField() || !bf[index]->getHP())
                {
                    //++act;
                    continue;
                }

                bf[index]->SetNowTime(i);
                bf[index]->Action();

                actCount += bf[index]->AppendFighterStream(_packet);
                //std::cout << "战报id: " << static_cast<UInt32>(_id) << "包大小：" << static_cast<UInt32>(_packet.size()) << std::endl;
                //++act;
            }
            actCount += doAttack(i);
        }
        _packet.data<UInt16>(offset) = actCount;
        _packet << Stream::eos;
        battleReport.addReport(_id,_packet);
    } 

    UInt8 BattleSimulator::GetWin()
    { 
        if(!_fgt[0] || !_fgt[0]->getHP())
            return 0;
        if(!_fgt[1] || !_fgt[1]->getHP())
            return 1;
        return 2;
    } 

    BattleFighter* BattleSimulator::CreateFighter(UInt8 Class ,Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY)
    { 
        if(!bf)
            bf = Script::BattleFormula::getCurrent();
        switch(Class)
        { 
            case 1:
                return new BattleRideFighter(bf,f,pointX,pointY);
            case 2:
                return new BattleRideFighter(bf,f,pointX,pointY);
            case 3:
                return new BattleRideFighter(bf,f,pointX,pointY);
            case 4:
                return new BattleRideFighter(bf,f,pointX,pointY);
            default:
                return new BattleRideFighter(bf,f,pointX,pointY);
        } 
    } 
    UInt8 BattleSimulator::doAttack(UInt16 time)
    { 
        std::vector<Battle::ActionPackage> vec = GetTimeBattleAction(time);

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
        } 

        return count;
    } 

    UInt8 BattleSimulator::doImage(UInt16 time)
    { 
        std::vector<Battle::ImagePackage> vec = GetTimeBattleImage(time);

        if(!vec.size())
            return 0;
        UInt8 count = 0;
        for(UInt8 i = 0; i < vec.size(); ++i)
        { 
            ImagePackage bAction = vec[i];
            BattleFighter * fgt = bAction.GetBattleFighter();
            _packet << static_cast<UInt8>(bAction.GetHappenTime());
            _packet << fgt->GetBSNumber();
            _packet << static_cast<UInt8>(2);
            _packet << static_cast<UInt16>(bAction.GetSkillId());

            std::cout << " 回合数" << static_cast<UInt32>(bAction.GetHappenTime());
            std::cout << " 战将编号:" << static_cast<UInt32>(fgt->GetBSNumber());
            std::cout << " 法术编号:" << static_cast<UInt32>(bAction.GetSkillId()) << std::endl;
            for(UInt8 j = 0; j < bAction.GetObjectSize(); ++j)
            {
                UInt16 param = bAction.GetObject(j)->BeActed(&bAction);
                //XXX 差法术协议
                _packet << static_cast<BattleFighter*>(bAction.GetObject(j))->GetBSNumber();
                _packet << static_cast<UInt16>(param);
                std::cout << " 战将编号:" << static_cast<UInt32>(bAction.GetObject(j)->GetBSNumber());
                std::cout << " 受伤数值:" << static_cast<UInt32>(param) << std::endl;
                ++count;
            }
        } 

        return count;
    } 

    UInt8 BattleSimulator::doObjectMove()
    { 
        std::list<ObjectPackage>& lst = GetObjectpackage();
        if(!lst.size())
            return 0;
        std::list<ObjectPackage>::iterator it = lst.begin(); 
        UInt8 count = 0;
        for(;it != lst.end();)
        { 
            if(it->CanExit())
            { 
                it = lst.erase(it);
                continue;
            } 

            BattleFighter * fgt = it->GetBattleFighter();
            for(UInt8 i = 0; i < _fighters[!fgt->GetSideInBS()].size(); ++i)
            { 
                if(it->CheckFighterInSCope(_fighters[!fgt->GetSideInBS()][i])) 
                { 
                    UInt16 param = _fighters[!fgt->GetSideInBS()][i]->BeActed(&(*it));

                    _packet << static_cast<UInt8>(it->GetHappenTime());
                    _packet << fgt->GetBSNumber();
                    _packet << static_cast<UInt8>(2);
                    _packet << static_cast<UInt16>(it->GetSkillId()) << static_cast<UInt8>(1);
                    _packet << _fighters[!fgt->GetSideInBS()][i]->GetBSNumber();
                    _packet << static_cast<UInt16>(param);

                    std::cout << " 回合数：" << static_cast<UInt32>(it->GetHappenTime());
                    std::cout << std::endl;
                    std::cout << " 技能释放者编号: " << static_cast<UInt32>(fgt->GetBSNumber());
                    std::cout << std::endl;
                    std::cout << " 技能编号："  << static_cast<UInt32>(it->GetSkillId()) << " 前进 " << static_cast<UInt32>(it->GetPosX()) << " , " << static_cast<UInt32>(it->GetPosY());
                    std::cout << std::endl;
                    std::cout << " 被击者：" << static_cast<UInt32>(_fighters[!fgt->GetSideInBS()][i]->GetBSNumber()) << " 位置: " << static_cast<UInt32>(_fighters[!fgt->GetSideInBS()][i]->getPosX()) <<" , "<< static_cast<UInt32>(_fighters[!fgt->GetSideInBS()][i]->getPosY());
                    std::cout << std::endl;
                    std::cout << " 伤害: " << static_cast<UInt32>(param);
                    std::cout << std::endl;
                    std::cout << std::endl;
                    ++count;
                } 
            } 
            it->GoNext();
            ++it;
        } 

        return count;
    } 
}

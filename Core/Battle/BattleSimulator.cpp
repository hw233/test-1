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

#define ADVANCE 180
#define ADVANCE_M 220
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
        UInt8 index = 1;
        UInt16 actCount = 0;
        size_t offset = _packet.size();
        _packet << actCount; 
        for(UInt8 i = 0; i < _limitTime ; ++i)
        {
            std::cout << "回合数:: " << static_cast<UInt32>(i) << std::endl <<std::endl;
            actCount += doImage(i);
            actCount += doObjectMove(i);  //每回合
            for(UInt8 j = 0; j < 20; ++j)
            {
                index = !index;
                bf[index] = getMyFighters(index,j/2);

                if(!bf[0] && !bf[1])          
                    break;

                if(!bf[index] || !bf[index]->GetField() || !bf[index]->getHP())
                    continue;
                if(_distance > bf[index]->GetDistance())
                    continue;

                bf[index]->CheckBuff();
                bf[index]->SetNowTime(i);
                bf[index]->Action();

                actCount += bf[index]->AppendFighterStream(_packet);
                std::cout << "战报id: " << static_cast<UInt32>(_id) << "包大小：" << static_cast<UInt32>(_packet.size()) << std::endl;
            }
            actCount += doAttack(i);
        }

        actCount += ClearObjectPackage();

        _packet.data<UInt16>(offset) = actCount;
        _packet << Stream::eos;

        battleReport1.addReport(_id,_packet);
    } 

    UInt8 BattleSimulator::GetWin()
    { 
        if( _fgt[0] && _fgt[0]->getHP() == 0 && _fgt[1] && _fgt[1]->getHP()==0 )
            return 3;
        if( !_fgt[0] || _fgt[0]->getHP() <= 0 )
            return 1;
        if( !_fgt[1] || _fgt[1]->getHP() <= 0 )
            return 0;
        return 2;
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
        fgt = new BattleShootFighter(bf,f,pointX,pointY);
        if(!fgt)
            return NULL;
        fgt->AddSkill();
        return fgt;
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

    //普通进攻(有动作时间)
    UInt8 BattleSimulator::doImage(UInt16 time)
    { 
        std::vector<Battle::ImagePackage> vec = GetTimeBattleImage(time);

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
                std::cout << " 战将编号:" << static_cast<UInt32>(bo->GetBSNumber());
                std::cout << " 受伤数值:" << static_cast<UInt32>(param) << std::endl;
                ++count;
            }
        } 

        return count;
    } 

    UInt8 BattleSimulator::doObjectMove(UInt16 time)
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
                it->BuildStream(_packet);
                it = lst.erase(it);
                ++count;
                continue;
            } 

            BattleFighter * fgt = it->GetBattleFighter();
            for(UInt8 i = 0; i < _fighters[!fgt->GetSideInBS()].size(); ++i)
            { 
                if(it->CheckFighterInSCope(_fighters[!fgt->GetSideInBS()][i])) 
                { 
                    UInt16 param = _fighters[!fgt->GetSideInBS()][i]->BeActed(&(*it));
                    it->InsertIntoPackage(time,_fighters[!fgt->GetSideInBS()][i], param);

                   // _packet << static_cast<UInt8>(it->GetHappenTime());
                   // _packet << fgt->GetBSNumber();
                   // _packet << static_cast<UInt8>(2);
                   // _packet << static_cast<UInt16>(it->GetSkillId()) << static_cast<UInt8>(1);
                   // _packet << _fighters[!fgt->GetSideInBS()][i]->GetBSNumber();
                   // _packet << static_cast<UInt16>(param);
                } 
            } 
            it->GoNext();
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


}

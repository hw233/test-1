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
        UInt8 act = 0;
        UInt8 index = 1;
        UInt32 curTime = 0;
        UInt16 actCount = 0;
        size_t offset = _packet.size();
        _packet << actCount; 
        while(curTime <= _limitTime && GetWin() == 2  )
        {
            if(act == 0)
            { 
            } 

            if(act > 20)
            {
                ++curTime;
                act = 0;
                continue;
            }

            index = !index;
            bf[index] = getMyFighters(index,act/2);

            if(!bf[0] && !bf[1])          
            {
                ++curTime;
                act = 0;
                continue;
            }

            if(!bf[index] || !bf[index]->GetField() || !bf[index]->getHP())
            {
                ++act;
                continue;
            }

            bf[index]->SetNowTime(curTime);
            bf[index]->Action();

            actCount += bf[index]->AppendFighterStream(_packet);
            //std::cout << "战报id: " << static_cast<UInt32>(_id) << "包大小：" << static_cast<UInt32>(_packet.size()) << std::endl;
            ++act;
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
}

#include "BattleSimulator.h"
#include "Script/BattleFormula.h"
#include "BattleFighter.h"

namespace Battle
{
    BattleSimulator::BattleSimulator(BattleFighter * bf , BattleFighter* bo):BattleField()
    { 
        _fgt[0] = bf;
        _fgt[1] = bo;
    } 
    void BattleSimulator::InitFighters(UInt8 index ,UInt8 flag)
    { 
        if(index > 1)
            return ;
        if(!_fgt[index])
            return ;

        UInt16 x[] = {3*STEP+STEP/2, 2*STEP+STEP/2, 1*STEP+STEP/2};
        if(_fgt[index]->getClass() == shooter || _fgt[index]->getClass() == adviser )
            x[0] = STEP/2;
        if((!flag && index) || (flag && !index))
        { 
            for(UInt8 i = 0; i < 3; ++i)
            { 
                x[i] = FIELD_WIDTH - x[i];
            } 
        } 

        //if(index)

        for(UInt8 i = 0; i < FIELD_HIGH/2; ++i)
        { 
            if( i ==  0)
            {
                setObjectXY(x[0] , static_cast<UInt16>(i*STEP+STEP/2),_fgt[index]);
                continue ;
            }

            BattleFighter* fgt1 =  _fgt[index]->getMyFighters((i-1)*2);
            if(!fgt1)
                return ;
            setObjectXY( x[2-i%2] , static_cast<UInt16>((FIELD_HIGH/2+i)*STEP-STEP/2) , fgt1);
            _fighters[index].push_back(fgt1);

            BattleFighter* fgt2 =  _fgt[index]->getMyFighters((i-1)*2 + 1);
            if(!fgt2)
                return ;
            setObjectXY( x[2-i%2] ,static_cast<UInt16>((FIELD_HIGH/2 - i)*STEP-STEP/2) , fgt2);
            _fighters[index].push_back(fgt2);
        } 
    } 
    void BattleSimulator::start(UInt8 prevWin , bool checkEnh)
    { 
        InitFighters(0);
        InitFighters(1);
    } 
   
    UInt8 BattleSimulator::GetWin()
    { 
        if(!_fgt[0] || !_fgt[0]->getHP())
            return 0;
        if(!_fgt[1] || !_fgt[1]->getHP())
            return 1;
        return 2;
    } 
}

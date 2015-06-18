#include "BattleShootFighter.h"
#include "GData/SkillTable.h"
#include <math.h>

#define TRIANGLE_Z(x,y) static_cast<UInt16>(sqrt(x*x + y*y))
#define SUB(x,y) (x>y?x-y:y-x)
namespace Battle
{ 
    bool BattleShootFighter::PreGetObject()
    { 
        if(!_target || !_target->getHP())
        {
            _target = GetField()->GetTarget(!GetSideInBS(),getPosX(),getPosY());
        }
        if(_target)
        {
            SetBattleTargetPos(_target->getPosX(),_target->getPosY());
            std::cout << "战将编号"  << static_cast<UInt32>(GetBSNumber()) << "锁定目标" << static_cast<UInt32>(_target->GetBSNumber()) << std::endl;
        }
        return 0;
    } 

    void BattleShootFighter::resetBattleStatue()
    { 
        _target = NULL;

        _battleTargetY = 0;
        _battleTargetX = 0;
        _nowTime = 0;
        EnterX = 0;
        EnterY = 0;
        _crickSum = 0;
        //_sideInBS = 0;
        return ;
    } 

    UInt8 BattleShootFighter::NormolAttack()
    { 
        if(!_target)
            return 0;
        {
            UInt16 targetX = _target->getPosX();
            UInt16 targetY = _target->getPosY();
            UInt16 targetZ = TRIANGLE_Z(SUB(targetX , getPosX()), SUB(targetY, getPosY())) ;
            if(targetZ == 0)
                targetZ = 1;
            ObjectPackage op(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
            op.setObjectDirection(getPosX(),getPosY(),targetX>getPosX(),targetY>getPosY(),100*SUB(targetX,getPosX())/targetZ, 100*SUB(targetY,getPosY())/targetZ, 50 , _target);
            op.setObjectTime(_actionLast);
            GetField()->InsertObjectPackage(op);

            GetField()->InsertBattlePre(GetNowTime2() + 0.1, this);
        }
        return 1;
    } 
} 


#include "BattleShootFighter.h"
#include "GData/SkillTable.h"
#include <math.h>

#define TRIANGLE_Z(x,y) static_cast<UInt16>(sqrt(x*x + y*y))
#define SUB(x,y) (x>y?x-y:y-x)
namespace Battle
{ 
    bool BattleShootFighter::PreGetObject()
    { 
        if(!_target)// || !_target->getHP())
        {
            _target = GetField()->GetTargetForRide(!GetSideInBS(),getPosX(),getPosY(),2);
        }
        if(!_target->getHP())
        {
            _target = GetField()->GetTarget(!GetSideInBS(),getPosX(),getPosY());
            std::cout << "战将编号"  << static_cast<UInt32>(GetBSNumber()) << "锁定目标" << static_cast<UInt32>(_target->GetBSNumber()) << std::endl;
        }
        if(_target)
        {
            SetBattleTargetPos(_target->getPosX(),_target->getPosY());
        }
        return 0;
    } 

    UInt8 BattleShootFighter::NormolObject()
    { 
        //BattleFighter::NormolAttack();
        if(!_target)
        {
            PreGetObject();
            if(!_target)
                return 0;
        }

        {
            UInt16 targetX = _target->getPosX();
            UInt16 targetY = _target->getPosY();
            UInt16 targetZ = TRIANGLE_Z(SUB(targetX , getPosX()), SUB(targetY, getPosY())) ;
            if(targetZ == 0)
                targetZ = 1;
            ObjectPackage op(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
            op.setObjectDirection(/*getPosX(),getPosY(),*/targetX>getPosX(),targetY>getPosY(),80*SUB(targetX,getPosX())/targetZ, 80*SUB(targetY,getPosY())/targetZ, 50 , _target);
            op.setObjectCount(1); //设置穿透数量
            op.pushObjectPoint(getPosX(),getPosY());
            op.SetEffectType(_actionType);
            op.SetBeing(GetNowTime() + _actionLast);
            GetField()->InsertObjectPackage(op);

            GetField()->InsertBattlePre(GetNowTime() + _actionLast + _actionBackLast, this);
        }
        return 1;
    } 
} 


#include "BattleShootFighter.h"
#include "GData/SkillTable.h"
#include <math.h>

#define TRIANGLE_Z(x,y) static_cast<UInt16>(sqrt(x*x + y*y))
namespace Battle
{ 
    bool BattleShootFighter::PreGetObject()
    { 
        return true;
        if(!_target || !_target->getHP())
        {
            _target = GetField()->GetTarget(!GetSideInBS(),getPosX(),getPosY());
        }
        SetBattleTargetPos(_target->getPosX(),_target->getPosY());
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
        _sideInBS = 0;
        return ;
    } 

    void BattleShootFighter::NormolAttack()
    { 
        if(_target)
        {
            UInt16 targetX = _target->getPosX();
            UInt16 targetY = _target->getPosY();
            UInt16 targetZ = TRIANGLE_Z(targetX , targetY) ;
            ObjectPackage op(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
            op.setObjectDirection(getPosX(),getPosY(),targetX>getPosX(),targetY>getPosY(),100*targetX/targetZ, 100*targetY/targetZ, 50);
            op.setObjectTime(_actionLast);
        }
    } 

    void BattleShootFighter::NormolImage()
    { 
        const GData::Skill * s = GData::skillManager[_ab._skillId];
        if(!s)
            return ;
        //BATTLE2
        ImagePackage ip(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,GetNowTime2());
        GetField()->GetTargetList(!GetSideInBS(), this , ip.vec_bo, _ab._skillId , GetBattleDirection()+1);
        GetField()->InsertTimeBattleAction(GetNowTime2()+s->GetActionCd(),ip);
    } 

    void BattleShootFighter::NormolObject()
    { 
        const GData::SkillScope* ss =GData::skillManager[_ab._skillId]->GetSkillScope();
        if(!ss)
            return ;

        UInt16 myY = getPosY();

        UInt8  minNumber = ss->radx / 2 ;  //radx 表示数量(一般情况为奇数) rady表示间隔 x,y作为上下闭合区间
        UInt16 width = ss->x + ss->y;
        UInt16 minY  =  0;
        if(myY > ((width * ss->rady + width)*minNumber + ss->y))
            minY = myY - ((width * ss->rady + width)*minNumber);

        for(UInt8 i = 0 ; i < ss->radx ; ++i)
        {
            ObjectPackage op(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
            op.setObjectDirection(getPosX(),minY + (ss->rady+1)*i*width,GetBattleDirection(),0,100, 0, 50);
            GetField()->InsertObjectPackage(op);
        }
    } 


} 


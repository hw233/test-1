#include "BattleAdviceFighter.h"
#include "GData/SkillTable.h"
#include <math.h>

namespace Battle
{ 
    bool BattleAdviceFighter::PreGetObject()
    { 
        return true;
        if(!_target || !_target->getHP())
        {
            _target = GetField()->GetTarget(GetSideInBS(),getPosX(),getPosY());
            //BuildLocalStream(e_run);
        }
        SetBattleTargetPos(_target->getPosX(),_target->getPosY());
        return 0;
    } 

    void BattleAdviceFighter::resetBattleStatue()
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

    UInt8 BattleAdviceFighter::NormolImage()
    { 
        const GData::Skill * s = GData::skillManager[_ab._skillId];
        if(!s)
            return 0;
        ImagePackage ip(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,GetNowTime2());
        GetField()->GetTargetList( !GetSideInBS(), this , ip.vec_bo, _ab._skillId);

        float cd = s->GetActionCd(); // s->GetActionCd1()*ip.vec_bo.size() + s->GetActionCd2();
        GetField()->InsertTimeBattleAction(GetNowTime2()+cd,ip);
        //GetField()->InsertTimeBattleAction(_nowTime2 + _actionLast, ip);
        return 1;
    } 
} 


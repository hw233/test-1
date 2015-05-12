#include "BattleShootFighter.h"
#include <math.h>

#define TRIANGLE_Z(x,y) static_cast<UInt16>(sqrt(x*x + y*y))
namespace Battle
{ 
    void BattleShootFighter::Action()
    { 
        _st.clear();
        UpdateActionList();
        //硬直
        if(_crick)
        {
            --_crick;
            return ;
        }
        //动作行为
        if(_actionLast)
        { 
            --_actionLast;
            return ;
        } 

        if(_actionBackLast)
        { 
            --_actionBackLast;
            return ;
        } 

        GetActionFromField();

        switch(_actionType)
        { 
            case e_none:
                break;
            case e_run:
                GoForward(1);
                break;
            case e_attack_near:
            case e_attack_middle:
            case e_attack_distant:
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
                break;
                //
            //魔法
            case e_image_attack:
            case e_image_therapy:
                { 
                    if(_ab._skillId)
                    {
                        ImagePackage ip(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
                        GetField()->GetTargetList(!GetSideInBS(), this , ip.vec_bo, _ab._skillId);
                        GetField()->InsertTimeBattleAction(_nowTime+(_actionLast?_actionLast:1),ip);
                    }
                } 
                break;
            case e_attack_counter:
                break;
            default:
                break;
        } 
    } 
    bool BattleShootFighter::PreGetObject()
    { 
        return true;
        if(!_target || !_target->getHP())
        {
            _target = GetField()->GetTarget(!GetSideInBS(),getPosX(),getPosY());
            //BuildLocalStream(e_run);
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
} 


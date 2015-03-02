#include "BattleAdviceFighter.h"
#include <math.h>

namespace Battle
{ 
    void BattleAdviceFighter::Action()
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
                break;
            case e_attack_near:
            case e_attack_middle:
            case e_attack_distant:
                break;
            case e_image_attack:
            case e_image_therapy:
                { 
                    ImagePackage ip(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
                    GetField()->GetTargetList( !GetSideInBS(), this , ip.vec_bo, _ab._skillId);
                    GetField()->InsertTimeBattleAction(_nowTime + _actionLast, ip);
                } 
                break;
            case e_attack_counter:
                break;
            default:
                break;
        } 
    } 
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
} 


#include "BattleRideFighter.h"
namespace Battle
{ 
    void BattleRideFighter::Action()
    { 
        _st.reset();
        UpdateActionList();
        //硬直
        if(_crick)
        {
            --_crick;
            return ;
        }

        GoForward();

        switch(_actionType)
        { 
            case e_none:
                GetActionFromField();
                break;
            case e_attack_near:
            case e_attack_middle:
            case e_attack_distant:
            case e_image_attack:
            case e_image_therapy:
                {
                    if(_target)
                    { 
                        (_target)->BeActed(MakeActionEffect());//ActionPackage(_actionType, _hit, _wreck, _critical, this));
                        (_target)->AppendFighterStream(_st);
                    } 
                }
                break;
            case e_attack_counter:
                break;
            default:
                break;
        } 
    } 

    void BattleRideFighter::PreGetObject()
    { 
        _target = GetField()->GetTarget(GetSide(),getPosX(),getPosY(),1);
        if(getPosX() < GetMinX() || getPosX() > FIELD_WIDTH - GetMinX())
        {
            if(_battleTargetX)
                _battleTargetX = FIELD_WIDTH;
            else
                _battleTargetX = 0;
            count++;
        }
        if(count > 1)
            SetBattleTargetPos(getPosX(),getPosY());

        if(!_battleTargetY)
            _battleTargetY = _target->getPosY();
    } 

    void BattleRideFighter::BuildLocalStream(UInt8 wait, UInt8 param)
    {
        _st.reset();
        //_st << static_cast<UInt8>(ACTION_HAPPEN); //即使起作用
        //_st << static_cast<UInt8>(getPosX());
        //_st << static_cast<UInt8>(getPosY());
        InsertFighterInfo(_st);
        //TODO 被击
        _st << _actionType;
        switch(_actionType)
        {
            case e_run:
            case e_attack_near:
            case e_attack_middle:
            case e_attack_distant:
            case e_image_attack:
            case e_image_therapy:
                {
                    //_st << static_cast<UInt8>(wait);  //是否延迟
                    //_st << static_cast<UInt8>(_actionLast); //动作持续
                    //for(;it!=targetList.end();++it)
                    { 
                        if(_target)
                            _target->InsertFighterInfo(_st);
                        //_st << (_target)->getPosX();
                        //_st << (_target)->getPosY();
                    } 
                }
            case e_be_attacked:
                _st << static_cast<UInt32>(param);
                break;
            default :
                break;
        }
    }

    UInt16 BattleRideFighter::GetTargetDistance()
    { 
        if(!_target || !GetField())
            return 0;
        return GetField()->getDistance(this,_target);
    } 
     
} 


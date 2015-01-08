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
                    std::list<BattleObject *>::iterator it = targetList.begin();
                    for(;it!=targetList.end();++it)
                    { 
                        (*it)->BeActed(MakeActionEffect());//ActionPackage(_actionType, _hit, _wreck, _critical, this));
                        (*it)->AppendFighterStream(_st);
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

} 


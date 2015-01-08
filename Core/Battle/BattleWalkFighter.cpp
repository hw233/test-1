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
        //动作行为
        if(_actionLast)
        { 
            --_actionLast;
            return ;
        } 

        switch(_actionType)
        { 
            case e_none:
                GetActionFromField();
                break;
            case e_run:
                GoForward();
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
        if(!_target)
        _target = GetField()->GetTarget(GetSide(),getPosX(),getPosY(),1);
        SetBattleTargetPos(_target->getPosX(),_targetPosY());
    } 

} 


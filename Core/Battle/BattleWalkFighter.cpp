#include "BattleWalkFighter.h"
namespace Battle
{ 
    void BattleWalkFighter::Action()
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
                        ActionPackage ap(this,_nowTime);
                        ap.PushObject(_target);

                        GetField()->InsertTimeBattleAction( _nowTime + 3 ,ap );
                        // (_target)->BeActed(MakeActionEffect());//ActionPackage(_actionType, _hit, _wreck, _critical, this));
                        // (_target)->AppendFighterStream(_st);
                    }
                } 
                break;
            case e_image_attack:
            case e_image_therapy:
                { 
                    ImagePackage ip(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
                    GetField()->GetTargetList(!GetSideInBS(), this , ip.vec_bo, _ab._skillId);
                    GetField()->InsertTimeBattleAction(_nowTime+3,ip);
                } 
                break;
            case e_attack_counter:
                break;
            default:
                break;
        } 
    } 
    bool BattleWalkFighter::PreGetObject()
    { 
        if(!_target || !_target->getHP())
        {
            _target = GetField()->GetTarget(!GetSideInBS(),getPosX(),getPosY());
            BuildLocalStream(e_run);
        }
        SetBattleTargetPos(_target->getPosX(),_target->getPosY());
        return 0;
    } 

    UInt16 BattleWalkFighter::GetTargetDistance()
    { 
        if(!_target || !GetField())
            return 0;
        return  GetField()->getDistance(this,_target);
    } 

    void BattleWalkFighter::BuildLocalStream(UInt8 wait, UInt8 param)
    {
        _st.reset();
        //_st << static_cast<UInt8>(ACTION_HAPPEN); //即使起作用
        //_st << static_cast<UInt8>(getPosX());
        //_st << static_cast<UInt8>(getPosY());
        //InsertFighterInfo(_st);
        //TODO 被击
        //_st << _actionType;
        switch(wait)
        {
            case e_run:
                if(_target)
                {
                    _st << static_cast<UInt8>(_nowTime);
                    _st << GetBSNumber();
                    _st << static_cast<UInt8>(wait);
                    _st << _target->GetBSNumber();
                }
                break;
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

    void BattleWalkFighter::resetBattleStatue()
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


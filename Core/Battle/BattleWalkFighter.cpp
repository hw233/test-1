#include "BattleWalkFighter.h"
#include "GData/SkillTable.h"
namespace Battle
{ 
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


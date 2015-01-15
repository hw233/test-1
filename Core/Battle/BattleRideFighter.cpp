#include "BattleRideFighter.h"
namespace Battle
{ 
    void BattleRideFighter::Action()
    { 
        _st.clear();
        UpdateActionList();
        //硬直
        if(_crick)
        {
            --_crick;
            return ;
        }

        GoForward(50);

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
                        BuildLocalStream();
                        (_target)->BeActed(MakeActionEffect());//ActionPackage(_actionType, _hit, _wreck, _critical, this));
                        (_target)->AppendFighterStream(_st);
                        _actionType = 0;
                    } 
                }
                break;
            case e_attack_counter:
                break;
            default:
                {
                }
                break;
        } 
    } 

    void BattleRideFighter::PreGetObject()
    { 
        _target = static_cast<BattleFighter* >(GetField()->GetTarget(!GetSideInBS(),getPosX(),getPosY(),1));
        if(!_target)
            return ;
        if(getPosX() <= GetMinX() || getPosX() >= FIELD_WIDTH - GetMinX())
        {
            if(!_battleTargetX)
                _battleTargetX = FIELD_WIDTH;
            else
                _battleTargetX = 0;
            count++;
        }
        if(count > 2)
            SetBattleTargetPos(getPosX(),getPosY());

        if(!_battleTargetY)
            _battleTargetY = _target->getPosY();
    } 

    void BattleRideFighter::BuildLocalStream(UInt8 wait, UInt8 param)
    {
        _st.clear();
        //_st << static_cast<UInt8>(ACTION_HAPPEN); //即使起作用
        //_st << static_cast<UInt8>(getPosX());
        UInt8 type = _actionType ;
        if(wait)
            type = wait;
        switch(type)
        {
            case e_run:
                if(!isRunSend)
                {
                    _st << static_cast<UInt8>(_nowTime);
                    (_st << GetBSNumber());
                    _st << static_cast<UInt8>(0);
                    _st << static_cast<UInt16>(_battleTargetY);
                    isRunSend = true ;
                    std::cout << " 回合数：" << static_cast<UInt32>(_nowTime);
                    std::cout << " 战将ID: " << static_cast<UInt32>(GetBSNumber());
                    std::cout << " 前进 y 坐标：" << static_cast<UInt32>(_battleTargetY);
                    std::cout << std::endl;
                } 
                break;
            case e_attack_near:
            case e_attack_middle:
            case e_attack_distant:
            case e_image_attack:
            case e_image_therapy:
                {
                    //for(;it!=targetList.end();++it)
                    { 
                        if(_target)
                        {
                            _st << static_cast<UInt8>(_nowTime);
                            _st << GetBSNumber();
                            _st << static_cast<UInt8>(1);
                            _st << _target->GetBSNumber();

                            std::cout << " 回合数：" << static_cast<UInt32>(_nowTime);
                            std::cout << " 战将ID: " << static_cast<UInt32>(GetBSNumber());
                            std::cout << " 进攻战将编号：" << static_cast<UInt32>(_target->GetBattleIndex());
                            std::cout << std::endl;
                        }
                        //_st << (_target)->getPosX();
                        //_st << (_target)->getPosY();
                    } 
                }
                break;
            case e_be_attacked:
                {
                    _st << static_cast<UInt8>(_nowTime);
                    _st << GetBSNumber();
                    _st << static_cast<UInt8>(2);
                    _st << static_cast<UInt16>(param);

                    std::cout << " 回合数：" << static_cast<UInt32>(_nowTime);
                    std::cout << " 战将ID: " << static_cast<UInt32>(GetBattleIndex());
                    std::cout << " 受伤：" << static_cast<UInt32>(param);
                    std::cout << std::endl;
                }
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

    void BattleRideFighter::resetBattleStatue()
    { 
        isRunSend = false;
        _target = NULL;
        _battleTargetY = 0;
        _battleTargetX = 0;
        count = 0;
        _nowTime = 0;
        EnterX = 0;
        EnterY = 0;
        _crickSum = 0;
        _sideInBS = 0;
    } 
} 


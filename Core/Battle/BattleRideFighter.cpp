#include "BattleRideFighter.h"
namespace Battle
{ 
    void BattleRideFighter::Action()
    { 
        if(!getHP())
            return ;
        _st.clear();
        UpdateActionList();
        //硬直
        if(_crick)
        {
            --_crick;
            return ;
        }

        if(!GetGone())
            GoForward();

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
            case e_attack_near:
            case e_attack_middle:
            case e_attack_distant:
            case e_image_attack:
            case e_image_therapy:
                {
                    if(_target)
                    { 
                        UInt16 parm = (_target)->BeActed(MakeActionEffect());//ActionPackage(_actionType, _hit, _wreck, _critical, this));
                        BuildLocalStream(0,parm);
                        //(_target)->AppendFighterStream(_st);
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

    bool BattleRideFighter::PreGetObject()
    { 
        UInt8 res = CheckTarget();
        if(res)
        {
            UInt8 direction = GetBattleDirection();
            if(res == 1)
                direction = 2;
            BattleFighter * target = static_cast<BattleFighter* >(GetField()->GetTarget(!GetSideInBS(),getPosX(),getPosY(),direction));
            if(target)
                _target = target;
        }

        if(!_target)
            return false;
        if(getPosX() <= GetMinX() - GetSideInBS()*1000 || getPosX() >= (GetMinX()+(!GetSideInBS())*1000))
        {
            switch(count)
            {
                case 0:
                    _battleTargetX = (getPosX() > FIELD_WIDTH/2) ? 0:FIELD_WIDTH;
                    break;
                case 1:
                    _battleTargetX = FIELD_WIDTH - _battleTargetX;
                    break;
                case 2:
                    SetBattleTargetPos(getPosX(),getPosY());
                    break;
                default:
                    return false ;
            }
            count++;
        }

        if(!_battleTargetY)
            _battleTargetY = _target->getPosY();
        return true;
    } 

    void BattleRideFighter::BuildLocalStream(UInt8 wait, UInt8 param)
    {
        //_st.clear();
        //_st << static_cast<UInt8>(ACTION_HAPPEN); //即使起作用
        //_st << static_cast<UInt8>(getPosX());
        UInt8 type = _actionType ;
        if(wait)
            type = wait;
        switch(type)
        {
            case e_run:
                if((GetBSNumber() == 0 || GetBSNumber() == 7)&& count == 2)
                {
                    //COUNT << " 战将ID: " << static_cast<UInt32>(GetBSNumber());
                    //COUNT << " 目标 x 坐标：" << static_cast<UInt32>(_battleTargetX);
                    //COUNT << " 自身 x 坐标：" << static_cast<UInt32>(getPosX()) << std::endl;
                }
                if(!isRunSend)
                {
                    _st << static_cast<UInt8>(_nowTime);
                    (_st << GetBSNumber());
                    _st << static_cast<UInt8>(0);
                    _st << static_cast<UInt16>(_battleTargetY);
                    isRunSend = true ;
                    //COUNT << " 回合数：" << static_cast<UInt32>(_nowTime);
                    //COUNT << " 战将ID: " << static_cast<UInt32>(GetBSNumber());
                    //COUNT << " 前进 y 坐标：" << static_cast<UInt32>(_battleTargetY);
                    //COUNT << std::endl;
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
                            _st << static_cast<UInt16>(param);

                            std::cout << " 回合数：" << static_cast<UInt32>(_nowTime);
                            std::cout << " 战将ID: " << static_cast<UInt32>(GetBSNumber());
                            std::cout << " 进攻战将编号：" << static_cast<UInt32>(_target->GetBSNumber());
                            std::cout << " 己方坐标: " << static_cast<UInt32>(getPosX()) << "," << static_cast<UInt32>(getPosY());
                            std::cout << " 敌方坐标: " << static_cast<UInt32>(_target->getPosX()) << "," << static_cast<UInt32>(_target->getPosY()) <<std::endl;
                            //COUNT << std::endl;
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

                    //COUNT << " 回合数：" << static_cast<UInt32>(_nowTime);
                    //COUNT << " 战将ID: " << static_cast<UInt32>(GetBattleIndex());
                    //COUNT << " 受伤：" << static_cast<UInt32>(param);
                    //COUNT << std::endl;
                }
                break;
            default :
                break;
        }
    }

    UInt16 BattleRideFighter::GetTargetDistance()
    { 
        if(!_target || !GetField())
            return -1;
        if(!_target->GetGone())
            _target->GoForward();
        if(CheckTarget())
            return -1;
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
    } 

    UInt8 BattleRideFighter::CheckTarget()
    { 
        if(!_target)
            return 1;
        if(!_target->getHP())
            return 1;
        if(!GetField())
            return 1;
        if(GetBattleDirection() && getPosX() > _target->getPosX())
            return 2;
        if(!GetBattleDirection() && getPosX() < _target->getPosX())
            return 2; 
        return 0;
    } 

    UInt8 BattleRideFighter::GetBattleDirection()
    { 
        if(count == 1)
            return !GetSideInBS();
        if(count == 2)
            return GetSideInBS();
        return 2;
    } 
} 


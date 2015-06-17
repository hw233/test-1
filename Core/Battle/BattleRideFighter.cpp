#include "BattleRideFighter.h"
#include "GData/SkillTable.h"
namespace Battle
{ 
    bool BattleRideFighter::PreGetObject()
    { 
        PreGetObject1() ;
        if(!_target)
            return false;
        if(getPosX() <= GetMinX() - GetSideInBS()*1000 || getPosX() >= (GetMinX()+(!GetSideInBS())*1000))
        {
            switch(count)
            {
                case 0:
                    {
                        _battleTargetX = (getPosX() > FIELD_WIDTH/2) ? 0:FIELD_WIDTH;
                        break;
                    }
                case 1:
                    {
                        _battleTargetX = GetMinX();
                        _battleTargetY = GetMinY();
                        break;
                    }
                case 2:
                    {
                        SetBattleTargetPos(getPosX(),getPosY());
                        break;
                    }
                default:
                    return false ;
            }
            count++;
        }

        if(!_battleTargetY)
            _battleTargetY = _target->getPosY();
        return true;
    } 

    void BattleRideFighter::PreGetObject1()
    { 
        UInt8 res = CheckTarget();
        if(res)
        {
            UInt8 direction = GetBattleDirection();
            if(res == 1)
                direction = 2;
            BattleFighter * target = static_cast<BattleFighter* >(GetField()->GetTargetForRide(!GetSideInBS(),getPosX(),getPosY(),direction));
            if(target)
                _target = target;
        }
    } 

    void BattleRideFighter::BuildLocalStream(UInt8 wait, UInt8 param)
    {
        UInt8 type = _actionType ;
        if(wait)
            type = wait;
        switch(type)
        {
            case e_run:
                if(1)
                {
                    //COUNT<< " 战将ID: " << static_cast<UInt32>(GetBSNumber());
                    //COUNT<< " 目标 x 坐标：" << static_cast<UInt32>(_battleTargetX);
                    //COUNT<< " 自身 x 坐标：" << static_cast<UInt32>(getPosX()) << std::endl;
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

                            //COUNT<< " 回合数：" << static_cast<UInt32>(_nowTime);
                            //COUNT<< " 战将ID: " << static_cast<UInt32>(GetBSNumber());
                            //COUNT<< " 进攻战将编号：" << static_cast<UInt32>(_target->GetBSNumber());
                            //COUNT<< " 己方坐标: " << static_cast<UInt32>(getPosX()) << "," << static_cast<UInt32>(getPosY());
                            //COUNT<< " 敌方坐标: " << static_cast<UInt32>(_target->getPosX()) << "," << static_cast<UInt32>(_target->getPosY()) ;
                            //COUNT << " 对方血量: " << static_cast<UInt32>(_target->getHP()) << std::endl;
                            _actionType = e_none;

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
        _target->SetNowTime(_nowTime);
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

     UInt8 BattleRideFighter::BeForAction()
     { 
        if(!isRunSend)
        {
            if(uRand(100) < 40)  //XXX
                return 1;
        } 

        if(!GetGone())
            GoForward();

        BuildLocalStream(e_run);
        return 0;
     } 
} 


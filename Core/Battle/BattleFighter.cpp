#include "BattleFighter.h"
#include "BattleAction.h"
#include "BattleField.h"
#include "Common/URandom.h"
#include "GData/SkillTable.h"
#include "Script/BattleFormula.h"

namespace Battle
{
    BattleFighter::BattleFighter( Script::BattleFormula * bf ,GObject::Fighter * f ,BattleField * field, UInt8 pointX , UInt8 pointY):BattleObject(1/*XXX*/, pointX, pointY, field),_formula(bf)
    { 
        setFighter(f);
        //_field = field;
    } 
    BattleFighter::~BattleFighter()
    {
    }   

    void BattleFighter::setFighter(GObject::Fighter * f)
    { 
        _fighter = f ;
        //运动行为
        preActionList.push_back(ActionBase(0,0,0));
        //普通攻击
        preActionList.push_back(ActionBase(1,0,0));
    } 

    void BattleFighter::GoForward(UInt8 targetX ,UInt8 targetY,UInt8 advance)
    { 
        UInt8 x = getPosX();
        UInt8 y = getPosY();
        UInt8 distanceX = x > targetX ? x - targetX:targetX -x;
        UInt8 distanceY = y > targetY ? y - targetY:targetY -y;
        while(advance--)
        { 
            if(distanceX > distanceY)
            {
                if(x > targetX && x > 0) 
                    --x;
                else
                    ++x;
                --distanceX;
            }
            else
            {
                if(y > targetY && x > 0) 
                    --y;
                else
                    ++y;
                --distanceY;
            }
        } 
        setPos(x,y);
    } 

    void BattleFighter::BeActed(BattleAction  bAction)
    { 
        UInt32 attack = bAction.GetAttack();
        UInt32 defend = GetDefendNear();
        UInt32 hpSub = attack - defend;
        makeDamage(hpSub);
        BuildLocalStream(e_be_attacked , hpSub);
    } 
    void BattleFighter::Action()
    { 
        _st.reset();
        updateActionList();
        if(_crick)
        {
            --_crick;
            return ;
        }

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
                {
                    BattleObject * bo = targetList.front();
                    if(bo == NULL)
                        GoForward(FIELD_HIGH/2, FIELD_WIDTH/2 , 1);
                    else
                    {
                        GoForward(bo->getPosX(), bo->getPosY(), 1);
                        targetList.pop_front();
                    }
                    _actionType = e_none;
                    _actionLast = 0;
                }
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
                        (*it)->BeActed(ActionPackage(_actionType, _hit, _wreck, _critical, this));
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

    void BattleFighter::GetActionFromField()
    { 
        //填充 actionType actionLast targetList
        //获得视野范围进攻对象 (如无对象则返回中心点虚拟对象)
        BattleObject * bo = GetField()->GetTarget(1,getPosX(),getPosY());  
        if(bo == NULL)   
            return ;

        //40%不行动
        if(uRand(100) < 40)
            return ;

        UInt8 advance = GetField()->getDistance(getPosX(), getPosY(), bo->getPosX(), bo->getPosY());
        //TODO 判断与目标的攻击距离
        //攻击距离不够，前进
        ActionBase ab = GetActionCurrent(advance);

        if(1) 
        { 
            _actionType = GData::skillEffectManager[ab._effect]->skillType;  //XXX
            _actionLast =  GData::skillConditionManager[ab._condition]->actionCd;; //行进时间一秒
            targetList.clear();
            targetList.push_back(bo);
        }
        _st << static_cast<UInt8>(_actionType);  //动作类型
        _st << static_cast<UInt8>(_actionLast);  //动作持续帧数(*8)
        _st << static_cast<UInt8>(ACTION_WAIT);   //延迟起作用
        _st << static_cast<UInt8>(getPosX());    //产生动作的对象坐标
        _st << static_cast<UInt8>(getPosY());
     
    } 

    void BattleFighter::updateActionList()
    { 
        for(ActionSort::iterator it = preActionCD.begin(); it != preActionCD.end();)
        { 
            --(it->_cd); 
            if(it->_cd == 0)
            { 
                ActionSort::iterator it_next = ++it;
                preActionList.push_back((*it));
                preActionCD.erase(it);
                it = it_next;
            } 
            ++it;
        } 
    } 
    ActionBase BattleFighter::GetActionCurrent(UInt8 advance)
    { 
        UInt8 priority = 0;
        ActionSort::iterator result ;
        ActionBase res(0,0,0);
        for(ActionSort::iterator it = preActionList.begin(); it != preActionList.end(); ++it)
        {   
            if(GData::skillConditionManager[it->_condition]->MeetCondition(advance,priority))
                result = it;
        }   
        if(priority != 0)
        { 
            res = *result;
            preActionList.erase(result);
            preActionCD.push_back(res);
        } 
        return res;
    } 
    void BattleFighter::BuildLocalStream(UInt8 wait , UInt8 param)
    { 
        _st.reset();
        _st << static_cast<UInt8>(ACTION_HAPPEN); //即使起作用
        _st << static_cast<UInt8>(getPosX());
        _st << static_cast<UInt8>(getPosY());
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
                    _st << static_cast<UInt8>(wait);  //是否延迟
                    _st << static_cast<UInt8>(_actionLast); //动作持续
                    _st << static_cast<UInt8>(targetList.size());  //动作作用对象个数
                    std::list<BattleObject *>::iterator it = targetList.begin();
                    for(;it!=targetList.end();++it)
                    { 
                        _st << (*it)->getPosX();
                        _st << (*it)->getPosY();
                    } 
                }
            case e_be_attacked:
                _st << static_cast<UInt32>(param);
                break;
            default :
                break;
        }
    } 
}

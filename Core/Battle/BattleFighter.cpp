#include "BattleFighter.h"
#include "BattleField.h"
#include "Common/URandom.h"
#include "GData/SkillTable.h"
#include "Script/BattleFormula.h"
#include "BattleAction.h"

namespace Battle
{
    BattleFighter::BattleFighter( Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):BattleObject(1/*XXX*/, pointX, pointY/*, field*/),_formula(bf)
    { 
        setFighter(f);
        m_fighters = NULL;
        setNumber(0);
        m_mainFighter = NULL;
        if( f != NULL)
        {
            m_fighters = new BattleFighter*[MYFIGHTERMAX];
            for(UInt8 i = 0; i < MYFIGHTERMAX ; ++i)
            {
                m_fighters[i] = new BattleFighter(_formula,NULL,0,0);
                if( m_fighters[i])
                     m_fighters[i]->setNumber(i+1);
            }
        }
        SetGroundX(pointX);
        SetGroundY(pointY);
        _st.reset();
        _hp = 1000;
        _attack_near = 100;
    } 
    BattleFighter::~BattleFighter()
    {
        if(m_fighters)
            delete [] m_fighters;
    }   

    void BattleFighter::setFighter(GObject::Fighter * f)
    { 
        _fighter = f ;
        //运动行为
        preActionList.push_back(ActionBase(0,0,0));
        //普通攻击
        preActionList.push_back(ActionBase(1,0,0));
    } 

    void BattleFighter::GoForward(UInt16 targetX ,UInt16 targetY,UInt16 advance)
    { 
        UInt16 x = getPosX();
        UInt16 y = getPosY();
        UInt16 distanceX = x > targetX ? x - targetX:targetX -x;
        UInt16 distanceY = y > targetY ? y - targetY:targetY -y;
        while(advance--)
        { 
            if(distanceX < distanceY)
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

    void BattleFighter::GetActionFromField()
    { 
        //填充 actionType actionLast targetList
        //获得视野范围进攻对象 (如无对象则返回中心点虚拟对象)
        BattleObject * bo = NULL;
        if(getClass() == eMain + Walker && targetList.size())
        {
           bo = targetList.front(); 
           if(bo->getHP() == 0)
               bo = NULL;
        }
        if(!bo)
            bo = GetField()->GetTarget(GetSide(),getPosX(),getPosY(),1);   //XXX
        if(bo == NULL)  
            return ;

        //40%不行动
        if(uRand(100) < 40)
            return ;

        UInt16 advance = GetField()->getDistance(getPosX(), getPosY(), bo->getPosX(), bo->getPosY());
        //TODO 判断与目标的攻击距离
        //攻击距离不够，前进

        _ab = GetActionCurrent(advance);

        if(1) 
        { 
            _actionType = GData::skillEffectManager[_ab._effect]->skillType;  //XXX
            _actionLast =  GData::skillConditionManager[_ab._condition]->actionCd;; //行进时间一秒
            targetList.clear();
            targetList.push_back(bo);
        }

        _st << static_cast<UInt8>(_actionType);  //动作类型
        _st << static_cast<UInt8>(_actionLast);  //动作持续帧数(*8)
        _st << static_cast<UInt8>(ACTION_WAIT);   //延迟起作用
        _st << static_cast<UInt8>(getPosX());    //产生动作的对象坐标
        _st << static_cast<UInt8>(getPosY());
     
    } 

    void BattleFighter::UpdateActionList()
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
                continue ;
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
    ActionPackage BattleFighter::MakeActionEffect()   //实现动作效果  伤害 法术等
    { 
        UInt8 aEffect = _ab._effect;
        const GData::SkillEffect * se = GData::skillEffectManager[aEffect];
        if(!se)
            return ActionPackage(_actionType, _hit, _wreck, _critical, this);
        UInt32 attack ;
        if(_actionType == e_attack_near)  //近战
            attack =  _attack_near ;
        else if( _actionType == e_attack_distant)  //远攻
            attack =  _attack_distance ;
        else if( _actionType == e_image_attack || _actionType == e_image_therapy )  //魔法
            attack = _attack_near;
        return  ActionPackage(attack, _hit, _wreck, _critical, this);  //未加入目标对象
    } 
    BattleFighter * BattleFighter::getMyFighters(UInt8 index)
    { 
        if(_fighter == NULL )
            return NULL;
        if(index >= MYFIGHTERMAX)
            return NULL;
        UInt8 count = 0;  
        for(UInt8 i = 0; i < MYFIGHTERMAX; ++i)
        {
            if(m_fighters[i]->getHP() == 0)
                continue ;
            if(count++ < index) 
                continue;
            return m_fighters[i];
        }
        return NULL;
    } 
}

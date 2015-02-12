#include "BattleFighter.h"
#include "BattleField.h"
#include "Common/URandom.h"
#include "GData/SkillTable.h"
#include "Script/BattleFormula.h"
#include "BattleAction.h"
#include "GObject/Player.h"

namespace Battle
{
    BattleFighter::BattleFighter( UInt8 Class ,Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):BattleObject(Class/*XXX*/, pointX, pointY/*, field*/),_formula(bf)
    { 
        setFighter(f);
        memset(m_fighters,0,sizeof( BattleFighter *)*MYFIGHTERMAX);
        setNumber(0);

        _nowTime = -1;

        m_mainFighter = NULL;
        if( f != NULL)
        {
            for(UInt8 i = 0; i < MYFIGHTERMAX ; ++i)
            {
                m_fighters[i] =  BattleSimulator::CreateFighter(f->GetChildTypeId(),_formula,NULL,0,0);
                m_fighters[i]->setMainFighter(this);
                if( m_fighters[i])
                    m_fighters[i]->setNumber(i+1);
            }
        }

        if(f)
        { 
            setHp(f->GetFighterAttr(e_attr_max));
            for(UInt8 i = e_attr_attack ; i < e_attr_max; ++i)
            { 
                attrBase[i] = f->GetFighterAttr(i);
            } 
        } 
        else
        {
            setHp(m_mainFighter->getHP());
            for(UInt8 i = e_attr_attack ; i < e_attr_max; ++i)
            { 
                attrBase[i] = f->GetBattleAttr(i);
            } 
            
        }

        SetGroundX(pointX);
        SetGroundY(pointY);
        _st.clear();
        _attack_near = 100;

        setHP(1000);
        _battleIndex = 0;
    } 
    BattleFighter::~BattleFighter()
    {
        for(UInt8 i = 0; i < MYFIGHTERMAX; ++i)
        {
            if(m_fighters[i])
                delete m_fighters[i];
        }

    }   

    void BattleFighter::setFighter(GObject::Fighter * f)
    { 
        _fighter = f ;
        //运动行为
        //preActionList.push_back(ActionBase(0,0,0));
        //普通攻击
        if(f!=NULL)
            preActionList.push_back(ActionBase(2));
    } 

    void BattleFighter::GoForward(UInt8 flag ,UInt16 advance) // flag ===0  表示Y优先  flag ==1 表示斜线
    { 
        SetGone(true);
        if(!advance)
            advance = GetSpeed();
        if(!PreGetObject())
            return ;
        flag = GetRideCount(); 

        UInt16 targetX = _battleTargetX ;
        UInt16 targetY = _battleTargetY ;
        UInt16 x = getPosX();
        UInt16 y = getPosY();
        UInt16 distanceX = x > targetX ? x - targetX:targetX -x;
        UInt16 distanceY = y > targetY ? y - targetY:targetY -y;

        if(!distanceX && !distanceY)
            return ;
        while(advance--)
        { 
            if(!distanceX && !distanceY)
                break;

            if((flag && distanceX > distanceY) || (!flag && !distanceY))
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
       // if(1 ||GetBSNumber() == 0 || GetBSNumber() == 7)
       // { 
       //     std::cout << "时间:" << static_cast<UInt32>(_nowTime) << " 战将" << static_cast<UInt32>(GetBSNumber()) << "编号  x坐标:" << static_cast<UInt32>(getPosX()) << std::endl;
       // } 
            
        //BuildLocalStream(e_run);
    } 

    UInt16 BattleFighter::BeActed(BattleAction *  bAction)
    { 
        //TODO
        UInt32 attack = bAction->GetAttack();
        UInt32 defend = GetDefendNear();
        UInt32 hpSub = attack - defend;
        hpSub = 700;
        makeDamage(hpSub);

        return hpSub;
        //BuildLocalStream(e_be_attacked , hpSub);
    } 
    void BattleFighter::Action()
    { 
        return ;

        //_st.reset();
        //UpdateActionList();
        ////硬直
        //if(_crick)
        //{
        //    --_crick;
        //    return ;
        //}
        ////动作行为
        //if(_actionLast)
        //{ 
        //    --_actionLast;
        //    return ;
        //} 

        ////if(_actionType == e_run_attack)
        //BattleObject * bo = targetList.front();
        //if(bo && getPosX() > STEP && getPosX() < (FIELD_WIDTH - STEP) )
        //{
        //    GoForward(bo->getPosX(), bo->getPosY(), 1);
        //}
        //else
        //{
        //    GetActionFromField();
        //}


        //switch(_actionType)
        //{ 
        //    case e_none:
        //        GetActionFromField();
        //        break;
        //    case e_run:
        //        {
        //        }
        //        break;
        //    case e_attack_near:
        //    case e_attack_middle:
        //    case e_attack_distant:
        //    case e_image_attack:
        //    case e_image_therapy:
        //        {
        //            std::list<BattleObject *>::iterator it = targetList.begin();
        //            for(;it!=targetList.end();++it)
        //            { 
        //                (*it)->BeActed(MakeActionEffect());//ActionPackage(_actionType, _hit, _wreck, _critical, this));
        //                (*it)->AppendFighterStream(_st);
        //            } 
        //        }
        //        break;
        //    case e_attack_counter:
        //        break;
        //    default:
        //        break;
        //} 
    } 

    void BattleFighter::GetActionFromField()
    { 
        //填充 actionType actionLast targetList
        //获得视野范围进攻对象 (如无对象则返回中心点虚拟对象)

         //if(uRand(100) < 40)  XXX
         //   return ;

        //UInt16 advance = GetField()->getDistance(this,_target);
        //TODO 判断与目标的攻击距离
        //攻击距离不够，前进

        _ab = GetActionCurrent(GetTargetDistance());

        if(1)  //XXX
        { 
            _actionType = GData::skillManager[_ab._skillId]->GetSkillEffect()->skillType;  //XXX
            _actionLast =  GData::skillManager[_ab._skillId]->GetActionCd(); //行进时间一秒
            _actionBackLast =  GData::skillManager[_ab._skillId]->GetActionBackCd(); //行进时间一秒
        }

        //BuildLocalStream();
        //_st << static_cast<UInt8>(_actionType);  //动作类型
        //_st << static_cast<UInt8>(_actionLast);  //动作持续帧数(*8)
        //_st << static_cast<UInt8>(ACTION_WAIT);   //延迟起作用
        //_st << static_cast<UInt8>(getPosX());    //产生动作的对象坐标
        //_st << static_cast<UInt8>(getPosY());

    } 

    void BattleFighter::UpdateActionList()
    { 
        for(ActionSort::iterator it = preActionCD.begin(); it != preActionCD.end();)
        { 
            --(it->_cd); 
            if(it->_cd == 0)
            { 
                preActionList.push_back((*it));
                it = preActionCD.erase(it);
                continue ;
            } 
            ++it;
        } 
    } 

    ActionBase BattleFighter::GetActionCurrent(UInt16 advance)
    { 
        UInt8 priority = 0;
        ActionSort::iterator result ;
        ActionBase res(0);//,0,0);
        if(advance == static_cast<UInt16>(-1))
            return res;
        for(ActionSort::iterator it = preActionList.begin(); it != preActionList.end(); ++it)
        {   
            if(advance < GetSpeed()*2 * GData::skillManager[it->_skillId]->GetActionCd())
                continue;
            if(GData::skillManager[it->_skillId]->GetSkillCondition()->MeetCondition(advance,priority)) //XXX
                result = it;
        }   
        if(priority != 0 && GData::skillManager[res._skillId])
        { 
            res = *result;
            res._cd = GData::skillManager[res._skillId]->GetCd();
            preActionList.erase(result);
            preActionCD.push_back(res);
        } 
        return res;
    } 
    void BattleFighter::BuildLocalStream(UInt8 wait , UInt8 param)
    { 
        //_st.reset();
        //_st << static_cast<UInt8>(ACTION_HAPPEN); //即使起作用
        //_st << static_cast<UInt8>(getPosX());
        //_st << static_cast<UInt8>(getPosY());
        ////TODO 被击
        //_st << _actionType;
        //switch(_actionType)
        //{
        //    case e_run:
        //    case e_attack_near:
        //    case e_attack_middle:
        //    case e_attack_distant:
        //    case e_image_attack:
        //    case e_image_therapy:
        //        {
        //            _st << static_cast<UInt8>(wait);  //是否延迟
        //            _st << static_cast<UInt8>(_actionLast); //动作持续
        //            _st << static_cast<UInt8>(targetList.size());  //动作作用对象个数
        //            std::list<BattleObject *>::iterator it = targetList.begin();
        //            for(;it!=targetList.end();++it)
        //            { 
        //                _st << (*it)->getPosX();
        //                _st << (*it)->getPosY();
        //            } 
        //        }
        //    case e_be_attacked:
        //        _st << static_cast<UInt32>(param);
        //        break;
        //    default :
        //        break;
        //}
    } 
    ActionPackage BattleFighter::MakeActionEffect()   //实现动作效果  伤害 法术等
    { 
        return  ActionPackage( this,_nowTime);  //未加入目标对象

        //UInt8 aEffect = _ab._effect;
        //const GData::SkillEffect * se = GData::skillEffectManager[aEffect];
        //if(!se)
        //    return ActionPackage(_actionType, _hit, _wreck, _critical, this,_nowTime);
        //UInt32 attack ;
        //if(_actionType == e_attack_near)  //近战
        //    attack =  _attack_near ;
        //else if( _actionType == e_attack_distant)  //远攻
        //    attack =  _attack_distance ;
        //else if( _actionType == e_image_attack || _actionType == e_image_therapy )  //魔法
        //    attack = _attack_near;

        //return  ActionPackage(attack, _hit, _wreck, _critical, this,_nowTime);  //未加入目标对象
    } 

    ImagePackage BattleFighter::MakeImageEffect()
    { 
        return ImagePackage(2,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
    } 
    BattleFighter * BattleFighter::getMyFighters(UInt8 index)   //找第几个活着的 (0开始)
    { 
        if(_fighter == NULL || !GetField())
            return NULL;
        if(index == 0)
            return this;
        --index;
        if(index >= MYFIGHTERMAX)
            return NULL;
        UInt8 count = 0;  
        for(UInt8 i = 0; i < MYFIGHTERMAX; ++i)
        {
            if(!m_fighters[i])
                continue;
            if(m_fighters[i] && m_fighters[i]->getHP() == 0)
            {
                delete m_fighters[i];
                m_fighters[i] = NULL;
                continue ;
            }
            if(count++ < index) 
                continue;
            return m_fighters[i];
        }
        return NULL;
    } 
    void BattleFighter::InsertFighterInfo(Stream& st,UInt8 flag)  //0表示入场信息，1 表示散仙战斗编号
    {
        if(m_mainFighter)
            return ;
        if(!_fighter)
            return ;

        st << static_cast<UInt8>(GetBattleIndex());
        if(!flag)
            return ;
        st << static_cast<UInt16>(GetId());
        st << static_cast<UInt8>(GetGroundX());
        st << static_cast<UInt8>(GetGroundY());
    }
    BattleField* BattleFighter::GetField()
    { 
        if(m_mainFighter && m_mainFighter != this) 
            return m_mainFighter->GetField();
        return _field;
    }

}

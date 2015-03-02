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

        if(f) //战将属性  小兵属性延后
        { 
            setHP(f->GetFighterAttr(e_attr_max));
            for(UInt8 i = e_attr_attack ; i < e_attr_max; ++i)
            { 
                attrBase[i] = f->GetFighterAttr(i);
            } 
        } 

        //属性
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

        SetGroundX(pointX);
        SetGroundY(pointY);
        _st.clear();

        //setHP(1000);
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
        {
            for(UInt8 i = 0; i < f->m_baseSkills.size(); ++i)
                preActionList.push_back(ActionBase(f->m_baseSkills[i]));
        }
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
        UInt32 defend = GetDefend();
        UInt32 hpSub = attack - defend;
        makeDamage(hpSub);

        return hpSub;
        //BuildLocalStream(e_be_attacked , hpSub);
    } 
    void BattleFighter::Action()
    { 
        return ;
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
    } 
    ActionPackage BattleFighter::MakeActionEffect()   //实现动作效果  伤害 法术等
    { 
        return  ActionPackage( this,_nowTime);  //未加入目标对象
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

    void BattleFighter::AddBuff(UInt16 buffId)
    { 
        const GData::SkillBuff * sb = GData::skillBuffManager[buffId];
        UInt8 count = sb->count;
        UInt8 type = sb->type;
        BattleBuff bb = BattleBuff(buffId, count);
        if(sb->attrIds.size() > sb->valueP.size() || sb->attrIds.size() > sb->value.size())
            return ;
        for(UInt8 i = 0; i < sb->attrIds.size(); ++i )
        { 
            UInt8 attrId = sb->attrIds[i];
            UInt8 valueP = sb->valueP[i];
            UInt16 value = sb->value[i];
            UInt32 val = GetBattleAttr(attrId);
            UInt32 valAdd = val * valueP /100 + value; 

            SetBattleAttr(attrId, valAdd , type+1);
            bb.value.push_back(valAdd);
            GetAttack();
        } 
        bufflst.push_back(bb);
    } 
    void BattleFighter::CheckBuff()
    { 
        if(!bufflst.size())
            return ;
        std::list<BattleBuff>::iterator it = bufflst.begin();
        for(;it != bufflst.end();)
        { 
            UInt8 count = (*it).count;
            if(!count)
            {
                const GData::SkillBuff * sb = GData::skillBuffManager[it->buffId];
                if(sb)
                {
                    UInt8 type = sb->type;
                    for(UInt8 i = 0; i < sb->attrIds.size(); ++i)
                    { 
                        UInt8 attrId = sb->attrIds[i];
                        UInt32 val = GetBattleAttr(attrId, type + 1) ;
                        if(it->value.size() > i)
                        { 
                            if(val > it->value[i])
                                val -= it->value[i];
                        } 
                        else
                            val = 0;
                        SetBattleAttr(attrId, val, type + 1);
                    } 
                }
                it = bufflst.erase(it);
                continue;
            }
            else
            { 
                --it->count;
            } 
            ++it;
        } 
    } 

}

#include "BattleFighter.h"
#include "BattleField.h"
#include "Common/URandom.h"
#include "GData/SkillTable.h"
#include "Script/BattleFormula.h"
#include "BattleAction.h"
#include "GObject/Player.h"
#include "Script/lua_tinker.h"

namespace Battle
{
    BattleFighter::BattleFighter( UInt8 Class ,Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):BattleObject(Class/*XXX*/, pointX, pointY/*, field*/),_formula(bf),_target(NULL)
    { 
        setFighter(f);
        memset(m_fighters,0,sizeof( BattleFighter *)*MYFIGHTERMAX);
        setNumber(0);

        _nowTime = -1;

        _beginTime = 0;
        
        _killCount = 0;

        m_mainFighter = NULL;

        if(f) //战将属性  小兵属性延后
        { 
            setHP(1000);//f->GetFighterAttr(e_attr_max));
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
        if(!f)
            return ;
        _fighter = f ;
        //运动行为
        //preActionList.push_back(ActionBase(0,0,0));
        //普通攻击
    } 

    void BattleFighter::GoForward(UInt8 flag ,UInt16 advance) // flag ===0  表示Y优先  flag ==1 表示斜线
    { 
        //SetGone(true);
        //UInt16 advance = GetSpeed() * count;
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
        if(targetX > x)
            SetBattleDirection(1);
        else 
            SetBattleDirection(0);
         if(GetBSNumber() == 0 || GetBSNumber() == 11)
         { 
           std::cout << "XXXXX时间:" << static_cast<float>(GetNowTime2()) << " 战将" << static_cast<UInt32>(GetBSNumber()) << "编号  x坐标:" << static_cast<UInt32>(getPosX()) << std::endl;
         } 

    } 

    UInt16 BattleFighter::BeActed(BattleAction *  bAction)
    { 
        //TODO
        UInt32 attack = bAction->GetAttack();
        UInt32 defend = GetDefend();
        UInt32 hpSub = attack - defend;
        //TEST
            hpSub = 200;
        makeDamage(hpSub);

        return hpSub;
        //BuildLocalStream(e_be_attacked , hpSub);
    } 
    void BattleFighter::Action()
    { 
        if(!getHP())
            return ;
        //_st.clear();
        UpdateActionList();
        //硬直
        if(_crick)
        {
            --_crick;
            return ;
        }

        if(BeForAction())
            return ;

        /* if(_actionLast)
        { 
            --_actionLast;
            return ;
        } 

        if(_actionBackLast)
        { 
            --_actionBackLast;
            return ;
        } 
        */

        GetActionFromField();

        //Print
        BattlePrintf();

        bool flag = false;
        switch(_actionType)
        { 
            case e_none:
                break;
            case e_run:
                //GoForward(1);
                break;
            case e_attack_near:
            case e_attack_middle:
            case e_attack_distant:
                {
                   flag = NormolAttack();
                }
                break;
            case e_image_attack:
            case e_image_therapy:
                { 
                   flag = NormolImage();
                } 
                break;
            case e_attack_counter:
                break;
            case e_object_image:
                { 
                    flag = NormolObject();
                } 
                break;
            default:
                {
                }
                break;
        } 
        if(!flag)
        {
            GetField()->InsertBattlePre(GetNowTime2() + 0.1, this);
        }
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
            //--(it->_cd); 
            //if(it->_cd == 0)
            if(it->_cd <= GetNowTime2())   //BATTLE2
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
        bool flag = false;
        if(advance == static_cast<UInt16>(-1))
            return res;
        for(ActionSort::iterator it = preActionList.begin(); it != preActionList.end(); ++it)
        {   
            //if(advance < GetSpeed()*2 * GData::skillManager[it->_skillId]->GetActionCd())
            //   continue;
            if(GData::skillManager[it->_skillId]->GetSkillCondition()->MeetCondition(advance,priority)) //XXX
            {
                flag = true;
                result = it;
            }
        }   
        if(/*priority != 0*/ flag && GData::skillManager[result->_skillId])
        { 
            res = *result;
            res._cd = GData::skillManager[res._skillId]->GetCd() + GetNowTime2(); //BATTLE2
            preActionList.erase(result);
            preActionCD.push_back(res);
        } 
        if(GetBSNumber() == 0 || GetBSNumber() == 11)
        { 
            std::cout << "!!!!时间:" << static_cast<float>(GetNowTime2()) << "战将编号:" << static_cast<UInt32>(GetBSNumber()) << " 动作选择：" << static_cast<UInt32>(res._skillId) << std::endl;
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
        st << static_cast<UInt8>(10);
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
        float count = sb->count + GetNowTime2();
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
        //BUFF 时间
        if(!bufflst.size())
            return ;
        std::list<BattleBuff>::iterator it = bufflst.begin();
        for(;it != bufflst.end();)
        { 
            float count = (*it).count;
            if(count >= GetNowTime2())
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
            ++it;
        } 
    } 
   
    void BattleFighter::AddSkill() 
    { 
        if( _fighter == NULL )
        {
            preActionList.push_back(GetBaseActionNum());
            return;
        }
        for(UInt8 i = 0; i < _fighter->m_baseSkills.size(); ++i)
        {
            preActionList.push_back(ActionBase(_fighter->m_baseSkills[i]));
            break;
        }
    }


    UInt8 baseData[3][2] = {
        {1,2},
        {1,2},
        {2,2}
    };

    UInt8 BattleFighter::GetMovePower()
    {
        //lua_State * L = lua_open();
        //luaOpen_base(L);
        //luaOpen_string(L);
        //luaOpen_table(L);
        //luaL_openlibs(L);
        //std::string path = cfg.scriptPath+"items/map.lua";
        //lua_tinker::dofile(L,path.c_str());
        UInt8 stype = GetTypeId();
        UInt8 power = baseData[stype-1][1]; //lua_tinker::call<UInt8>(L,"GetMovePower",stype); 
        return power;
    }


    UInt8 BattleFighter::GetAttackRange()
    {
        //lua_State * L = lua_open();
        //luaOpen_base(L);
        //luaOpen_string(L);
        //luaOpen_table(L);
        //luaL_openlibs(L);
        //std::string path = cfg.scriptPath+"items/map.lua";
        //lua_tinker::dofile(L,path.c_str());
        UInt8 stype = GetTypeId();
        UInt8 range = baseData[stype-1][0]; //lua_tinker::call<UInt8>(L,"GetAttackRange",stype); 
        return range;
    }

    UInt8 BattleFighter::NormolAttack()
    { 
        if(!_target)
        {
            PreGetObject(); 
            if(!_target)
                return 0;
        }
        { 
            ActionPackage ap(this,_nowTime2/*,_target*/);
            ap.PushObject(_target);

            GetField()->InsertTimeBattleAction( GetNowTime2() + _actionLast ,ap );
            _actionType = e_none;
            std::cout << "战将编号: " << static_cast<UInt32>(GetBSNumber()) << "位置: (" << static_cast<UInt32>(getPosX()) << "," << static_cast<UInt32>(getPosY())<< ") 普通攻击对象编号:" << static_cast<UInt32>(_target->GetBSNumber()) << " 位置：(" << static_cast<UInt32>(_target->getPosX()) << " , "<< static_cast<UInt32>(_target->getPosY()) << " )"<< std::endl;
            GetField()->InsertBattlePre(GetNowTime2() + _actionLast + _actionBackLast,this);
            std::cout <<"战将编号：" << static_cast<UInt32>(GetBSNumber())<< "普通攻击发起，行动列表添加到时间" <<  static_cast<float>(GetNowTime2() + _actionLast + _actionBackLast)<< std::endl;
        } 
        return 1;
    } 

    UInt8 BattleFighter::NormolImage()
    { 
        const GData::Skill * s = GData::skillManager[_ab._skillId];
        if(!s)
            return 0;
        //BATTLE2
        ImagePackage ip(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,GetNowTime2());
        GetField()->GetTargetList(!GetSideInBS(), this , ip.vec_bo, _ab._skillId , GetBattleDirection()+1);

        float cd = s->GetActionCd(); // s->GetActionCd1()*ip.vec_bo.size() + s->GetActionCd2();
        GetField()->InsertTimeBattleAction(GetNowTime2()+cd,ip);
        std::cout << "战将编号: " << static_cast<UInt32>(GetBSNumber()) << " 释放技能:"<<static_cast<UInt32>(_ab._skillId) << std::endl;
        return 1;
    } 

    UInt8 BattleFighter::NormolObject()
    { 
        const GData::Skill * s = GData::skillManager[_ab._skillId];
        if(!s)
            return 0;
        const GData::SkillScope* ss = s->GetSkillScope();
        if(!ss)
            return 0;

        UInt16 myY = getPosY();

        UInt8  minNumber = ss->radx / 2 ;  //radx 表示数量(一般情况为奇数) rady表示间隔 x,y作为上下闭合区间
        UInt16 width = ss->x + ss->y;
        UInt16 minY  =  0;
        if(myY > ((width * ss->rady + width)*minNumber + ss->y))
            minY = myY - ((width * ss->rady + width)*minNumber);

        for(UInt8 i = 0 ; i < ss->radx ; ++i)
        {
            ObjectPackage op(_ab._skillId,GetAttack(),GetCritical(),GetWreck(),GetHit(),this,GetNowTime2());
            op.setObjectDirection(getPosX(),minY + (ss->rady+1)*i*width,GetBattleDirection(),0,100, 0, 50);
            GetField()->InsertObjectPackage(op);
        }

        GetField()->InsertBattlePre(GetNowTime2() + s->GetActionBackCd(), this);
        std::cout <<"战将编号：" << static_cast<UInt32>(GetBSNumber()) << "粒子性技能:" << static_cast<UInt32>(_ab._skillId) <<std::endl;
        return 1;
    } 

    void BattleFighter::BattlePrintf()
    { 
       TRACE_LOG("战将编号%d ,技能编号：%d",GetBSNumber(),_ab._skillId);
    } 


    void BattleFighter::SetSoldierHp(UInt8 index,UInt32 hp)
    {
        if( index < 0 || index >= MYFIGHTERMAX)
        {
            return;
        }
        m_fighters[index]->setHP(hp);
    }

    UInt32 BattleFighter::GetSoldierHp(UInt8 index)
    {
        if( index < 0 || index >= MYFIGHTERMAX )
        {
            return 0;
        }
        return m_fighters[index]->getHP();
    }
    
    /*
    void BattleFighter::SetMainFighterHP(UInt32 hp)
    {
        m_mainFighter->setHP(hp);
    }
    */
}

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
    BattleFighter::BattleFighter( UInt8 Class ,Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):BattleObject(Class/*XXX*/, pointX, pointY/*, field*/),_formula(bf),_target(NULL),_multiKill(0)
    { 
        setFighter(f);
        memset(m_fighters,0,sizeof( BattleFighter *)*MYFIGHTERMAX);
        setNumber(0);

        _nowTime = -1;

        _beginTime = 0;

        _killCount1 = 0;
        _killCount2 = 0;

        _cachePx = 0;

        _acted = false;

        _avoidhurt = false;

        _isChild = false;

        m_mainFighter = NULL;

        _energy = 0; //主将集气

        _multiKill = 0;

        //if(f) //战将属性
        if(f)
        { 
            setHP(f->GetFighterAttr(e_attr_hp));
            for(UInt8 i = e_attr_attack ; i < e_attr_max; ++i)
            { 
                attrBase[i] = f->GetFighterAttr(i);
            } 
            AddSkill();
        } 

        //是否又小兵
        if( f->GetChildTypeId())
        {
            UInt16 fighterId = f->GetChildTypeId();
            for(UInt8 i = 0; i < MYFIGHTERMAX ; ++i)
            {
                GObject::Fighter * fgt = GObject::globalFighters[fighterId];
                if(!fgt)
                    break;
                m_fighters[i] =  BattleSimulator::CreateFighter(fgt->GetTypeId(),_formula,fgt,0,0);
                if( m_fighters[i])
                {
                    m_fighters[i]->setMainFighter(this);
                    m_fighters[i]->setNumber(i+1);
                }
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

    } 

    //被攻击
    UInt32 BattleFighter::BeActed(BattleAction *  bAction)
    { 
        //TODO
        UInt32 attack = bAction->GetAttack();
        UInt32 defend = GetDefend();
        UInt32 attackImage = bAction->GetAttackImage();
        UInt32 defendImage = GetImageDefend();
        UInt32 hpSub = 0;
        UInt8 flag = 0;

        //物理攻击
        if(bAction->GetBattleFighter()->GetTypeId() != e_advice)
        {
            if(attack <= defend)
            { 
                hpSub += attack * 25 / 100;
            } 
            else
            {
                hpSub += attack - (defend  * 75 /100);
            }
        }
        else //法术攻击
        {
            if(attackImage <= defendImage)
            { 
                hpSub += attackImage * 25 / 100;
            } 
            else
            {
                hpSub += attackImage - (defendImage * 75 /100);
            }
        }

        //TEST

        UInt32 hit = bAction->GetHit();
        UInt32 evade = GetEvade(); 
        UInt32 rand = uRand(10000);

        //闪避
        if(!evade || rand > (hit*10000/evade))
        {
            flag = 1;
        }

        UInt32 critical = bAction->GetCritical();
        UInt32 criticalDef = GetCriticalDef(); 
        rand = uRand(10000);

        //暴击
        if(!criticalDef || rand > (critical * 10000/criticalDef))
        {
            flag = 2;
        }

        GetAttackedSkill(flag);  //被击的发动被动技能

        //伤害变化
        switch(flag)
        { 
            case 1: //闪避
                {
                    hpSub = 0;
                }
                break;
            case 2: //暴击
                {
                    hpSub *= 2;
                }
                break;
            case 3:  //格挡
                {
                    hpSub = 0;
                }
                break;
        } 

        if(hpSub)
        { 
            UInt16 skillId = bAction->GetSkillId();
            const GData::Skill* s = GData::skillManager[skillId];
            const GData::SkillEffect* se = s->GetSkillEffect();
            UInt16 stiffFixed = se->stiffFixed;  //僵直，眩晕
            if(stiffFixed) 
                _crick += stiffFixed;

            //击退
            if(se->beatBack)
            { 
                BattleFighter *fgt = bAction->GetBattleFighter();
                if(fgt)
                { 
                    if(fgt->getPosX() > getPosX())
                    {
                        if(getPosX() > se->beatBack)
                            setPos(getPosX() - se->beatBack, getPosY());
                    }
                    else
                    {
                        if(getPosX() + se->beatBack < FIELD_WIDTH ) 
                            setPos(getPosX() + se->beatBack, getPosY());
                    }
                } 
            } 
        } 

        makeDamage(hpSub);

        //弓箭手和谋士，谁打我我打谁
        if(GetClass() == e_shoot || GetClass() == e_advice)
        { 
            BattleFighter *fgt = bAction->GetBattleFighter();
            if(fgt)
            {
                _target = fgt;
                //COUT << "战将("<<static_cast<UInt32>(GetBSNumber()) <<  ") 被攻击  转换攻击目标" << static_cast<UInt32>(fgt->GetBSNumber()) << std::endl;
            }
        } 

        AddEnergy(5);

        //param = 伤害状态{躲避，格挡、、、} << 16 | 伤害值
        return hpSub | (flag << 16);
        //BuildLocalStream(e_be_attacked , hpSub);
    } 
    void BattleFighter::Action()
    { 
        if(!getHP())
            return ;
        //更新行动列表(检查技能列表是否冷却结束)
        UpdateActionList();

        //硬直
        if(_crick)
        {
            GetField()->InsertBattlePre(GetNowTime()+_crick, this);
            _crick = 0;
            return ;
        }

        if(_target && _target->GetAvoidHurt())
        {
            //COUT << " 无动作 位置: " << static_cast<UInt32>(getPosX()) << " , " << static_cast<UInt32>(getPosY()) ;
            GetField()->InsertBattlePre(GetNowTime(), this);
            SetMove(true);
            return ;
        }

        if(BeForAction())
            return ;

        //获取当前的行为
        GetActionFromField();

        //Print
        BattlePrintf();

        bool flag = false;

        if(_ab._skillId)
        {
            //COUT << " 技能选择 " << static_cast<UInt32>(_ab._skillId);

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
                case e_attack_back:
                case e_attack_quick:
                    {
                        //COUT << " 发起普通攻击 " ;
                        flag = NormolAttack();
                    }
                    break;
                case e_image_attack:
                case e_image_therapy:
                case e_image_attack_time_special:
                    { 

                        //COUT << " 发起魔法攻击 " ;
                        const GData::Skill * s = GData::skillManager[_ab._skillId];
                        if(s)
                        {
                            const GData::SkillEffect * se = s->GetSkillEffect(); 
                            if(se)
                            { 
                                if(se->avoidhurt)
                                    SetAvoidHurt(true);
                            } 
                        }
                        UInt8 super = s->GetSuperSkill();
                        if(super)
                            GetField()->SetSuperSkill(this);
                        GetField()->AddTimeExtra(s->GetFrozrTime());
                        flag = NormolImage();
                    } 
                    break;
                case e_object_image:
                case e_object_attack:
                    { 
                        //COUT << " 发起粒子攻击 " ;
                        const GData::Skill * s = GData::skillManager[_ab._skillId];
                        if(s)
                        {
                            const GData::SkillEffect * se = s->GetSkillEffect(); 
                            if(se)
                            { 
                                if(se->avoidhurt)
                                    SetAvoidHurt(true);
                            } 
                        }
                        UInt8 super = s->GetSuperSkill();
                        if(super)
                            GetField()->SetSuperSkill(this);
                        GetField()->AddTimeExtra(s->GetFrozrTime());
                        flag = NormolObject();
                    } 
                    break;
                default:
                    {
                    }
                    break;
            } 
        }

        //flag 指明此次是否有动作
        if(!flag)
        {
            //COUT << " 无动作 位置: " << static_cast<UInt32>(getPosX()) << " , " << static_cast<UInt32>(getPosY()) ;
            if(GetField()->GetSuperSkill() && this == GetField()->GetSuperSkill())
            {
                GetField()->SetSuperSkill(NULL);
            }
            GetField()->InsertBattlePre(GetNowTime(), this);
            SetMove(true);
        }
        else
        {
            SetMove(false);
        }
        if(GetAvoidHurt())
            SetAvoidHurt(false);
        //COUT << std::endl;
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

        if(!_ab._skillId)  //XXX
        {
            _actionType = 0;
            _actionLast = 0;
            _actionBackLast = 0;
        }
        else
        { 
            _actionType = GData::skillManager[_ab._skillId]->GetSkillEffect()->skillType;  //XXX
            _actionLast =  GData::skillManager[_ab._skillId]->GetActionCd1(); //行进时间一秒
            _actionBackLast =  GData::skillManager[_ab._skillId]->GetActionBackCd(); //行进时间一秒
        }

    } 

    void BattleFighter::UpdateActionList()
    { 
        for(ActionSort::iterator it = preActionCD.begin(); it != preActionCD.end();)
        { 
            //--(it->_cd); 
            //if(it->_cd == 0)
            if(it->_cd <= GetNowTime())   //BATTLE2
            { 
                preActionList.push_back((*it));
                it = preActionCD.erase(it);
                continue ;
            } 
            ++it;
        } 
    } 

    //从技能准备列表中选取当前可以释放的技能   参数：距离，主/被动
    ActionBase BattleFighter::GetActionCurrent(UInt16 advance, UInt8 type) //type == 0 主动  1 被动
    { 
        UInt8 priority = 0;
        ActionSort::iterator result ;
        ActionBase res(0);//,0,0);
        UInt8 flag = 0;
        if(advance == static_cast<UInt16>(-1))
            return res;
        for(ActionSort::iterator it = preActionList.begin(); it != preActionList.end(); ++it)
        {   
            const GData::Skill * s = GData::skillManager[it->_skillId];
            if(!s)
                continue;
            if(s->GetSkillCondition()->cond != type)  
                continue;

            //超级技能（大招） 的释放，要确保此时没有其他角色释放超级技能，并且能量吵过100
            if(s->GetSuperSkill() && (GetField()->GetSuperSkill() || _energy < 100 ))
                continue;

            UInt16 advance1 = 0;
            UInt16 advance2 = 0;
            UInt16 adv = 0; 
            if(!type && _target)
            {
                if(GetTypeId() == e_ride)
                    advance1 = s->GetActionCd1()*GetSpeed()/100;

                if(_target && _target->GetTypeId() == e_ride)
                    advance2 = s->GetActionCd1()*_target->GetSpeed()/100;

                //预判

                if((getPosX() < _target->getPosX() && _target->GetBattleDirection()) || (getPosX() > _target->getPosX() && !_target->GetBattleDirection()))
                    adv = advance1 - advance2;
                else
                    adv = advance1 + advance2;

            }

            //当前技能是否匹配
            if(s->GetSkillCondition()->MeetCondition(advance - adv,priority)) //XXX
            {
                flag = 1;
                result = it;
                if(s->GetSuperSkill())
                    flag = 2;
            }
        }   
        if(/*priority != 0*/ flag && GData::skillManager[result->_skillId])
        { 
            res = *result;
            res._cd = GData::skillManager[res._skillId]->GetCd() + GetNowTime(); //BATTLE2
            preActionList.erase(result);
            preActionCD.push_back(res);
            if(flag == 2)
                _energy = _energy > 100 ? _energy - 100:0; 
            AddEnergy(5);
        } 
        return res;
    } 
    void BattleFighter::BuildLocalStream(UInt8 wait , UInt8 param)
    { 
    } 

    ActionPackage BattleFighter::MakeActionEffect()   //实现动作效果  伤害 法术等
    { 
        return  ActionPackage( this,_ab._skillId,_nowTime);  //未加入目标对象
    } 

    ImagePackage BattleFighter::MakeImageEffect()
    { 
        return ImagePackage(2,GetAttack(),GetAttackImage(),GetCritical(),GetWreck(),GetHit(),this,_nowTime);
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
        st << static_cast<UInt8>(GetFighterNum());
        st << static_cast<UInt16>(GetTotalPower());
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
        UInt32 count = sb->count + GetNowTime();
        UInt8 type = sb->type;
        BattleBuff bb = BattleBuff(buffId, count);
        if(sb->attrIds.size() > sb->valueP.size() || sb->attrIds.size() > sb->value.size())
            return ;

        //暂时支持属性加成buf
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
        //检查 BUFF 时间
        if(!bufflst.size())
            return ;
        std::list<BattleBuff>::iterator it = bufflst.begin();
        for(;it != bufflst.end();)
        { 
            UInt16 count = (*it).count;
            if(count >= GetNowTime())
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

    //从fighter导入技能
    void BattleFighter::AddSkill() 
    { 
        std::vector<UInt16> vec = GetBaseSkills();
        for(UInt8 i = 0; i < vec.size(); ++i)
        {
            UInt16 skillId= vec[i];
            const GData::Skill * s = GData::skillManager[skillId];
            if(!s)
                continue;
            if(!_fighter/* && skillId > 1000*/)
                continue;
            preActionList.push_back(ActionBase(skillId));
            //break;
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
            //战斗包
            ActionPackage ap(this, _ab._skillId, GetNowTime()/*,_target*/);
            ap.PushObject(_target);
            
            //战斗包队列
            GetField()->InsertTimeBattleAction( GetNowTime() + _actionLast ,ap );
            _actionType = e_none;

            //加入准备队列
            GetField()->InsertBattlePre(GetNowTime() + _actionLast + _actionBackLast,this);
        } 
        return 1;
    } 

    UInt8 BattleFighter::NormolImage()
    { 
        const GData::Skill * s = GData::skillManager[_ab._skillId];
        if(!s)
            return 0;
        //BATTLE2
        ImagePackage ip(_ab._skillId,GetAttack(),GetAttackImage(),GetCritical(),GetWreck(),GetHit(),this,GetNowTime());
        GetField()->GetTargetList(!GetSideInBS(), this , ip.vec_bo, _ab._skillId , GetBattleDirection()+1);

        UInt16 cd = _actionLast; // s->GetActionCd1()*ip.vec_bo.size() + s->GetActionCd2();

        //特殊技能 冷却时间由对方受击角色数量决定
        if(s->GetSkillEffect()->skillType == e_image_attack_time_special)
            cd = s->GetActionCd1()*ip.vec_bo.size() + s->GetActionCd2();
        GetField()->InsertTimeBattleAction(static_cast<UInt16>(GetNowTime()+cd),ip);
        return 1;
    } 

    //粒子型技能
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

        ObjectPackage op(_ab._skillId,GetAttack(),GetAttackImage(),GetCritical(),GetWreck(),GetHit(),this,GetNowTime());
        op.setObjectDirection(/*getPosX(),minY + (ss->rady+1)*i*width,*/GetBattleDirection(),0,40, 0, 50);
        
        //设置最多可攻击数量
        op.setObjectCount(s->GetAttackCount());

        //设置动作行为(粒子分类，追击型，行动型)
        op.SetEffectType(_actionType);
        for(UInt8 i = 0 ; i < ss->radx ; ++i)
        {
            op.pushObjectPoint(getPosX(), minY + (ss->rady+1)*i*width);
        }

        //什么时间开始起作用
        op.SetBeing(GetNowTime() + _actionLast);
        GetField()->InsertObjectPackage(op);

        //粒子型技能释放之后，武将进入行动队列
        GetField()->InsertBattlePre(GetNowTime() + _actionLast + _actionBackLast, this);
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
        if( m_fighters[index] == NULL )
        {
            return;
        }
        m_fighters[index]->setHP(hp);
    }

    UInt32 BattleFighter::GetSoldierHp(UInt8 index)
    {
        if( index < 0 || index >= MYFIGHTERMAX || m_fighters[index] == NULL )
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
    void BattleFighter::ResetSkill()
    { 
        ActionSort::iterator it = preActionCD.begin();  
        if(it != preActionCD.end())
        { 
            for(;it != preActionCD.end(); ++it)
                preActionList.push_back(*it);
            preActionCD.clear();
        } 
        //it = preActionList.begin();
        //for(;it!=preActionList.end();++it)
        //{ 
        //    it->_cd = 0;
        //} 
    } 
    void BattleFighter::resetBattleStatue()
    { 
        _target = NULL;

        _battleTargetY = 0;
        _battleTargetX = 0;
        _nowTime = 0;
        EnterX = 0;
        EnterY = 0;
        _crickSum = 0;
        //_sideInBS = 0;
        _avoidhurt = false;
        return ;
    } 
    bool BattleFighter::GetAttackedSkill(UInt8& flag)
    { 
        ActionBase ab = GetActionCurrent(0,1);
        if(ab._skillId)
        {
            const GData::Skill * s = GData::skillManager[ab._skillId];
            UInt8 type = s->GetSkillEffect()->skillType;
            switch(type)
            { 
                //格挡(产生僵直)
                case e_parry:
                    { 
                        if(_ab._skillId)
                            _crick += s->GetActionBackCd();
                        flag = 3;
                    } 
                    break;
            } 
        }
        return true;
    }
}

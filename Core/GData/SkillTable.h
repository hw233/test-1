
#ifndef SKILLTABLE_H_
#define SKILLTABLE_H_

#include "Config.h"
#include "GObject/GObjectManager.h"
#include "Common/URandom.h"

#undef SKillT 


namespace GData
{
    enum    // 技能触发方式
    {
        /*0*/ SKILL_ACTIVE = 0,  //主动技能
        /*1*/ SKILL_PEERLESS = 1,   //无双技能
        SKILL_PASSSTART,
    };

    enum 
    {
        SCOPE_SINGAL = 0,  //单体
        SCOPE_ROUND = 1 ,  //四周
        SCOPE_CORESS,      //十字形
    };

#ifdef SKillT
    class SkillCondition 
    { 
        public:
            SkillCondition(){}
#else
    class SkillCondition 
        : public ObjectBaseT<UInt16>
    { 
        public:
            SkillCondition(UInt16 id, const std::string& name)
                : ObjectBaseT<UInt16>(id,name) ,cond(0),prob(0),distance(0),priority(0)
#endif
            { } 
            ~SkillCondition(){}
            bool MeetCondition(UInt16 advance ,UInt8& pri) const 
            {
                if(uRand(100) > prob)                
                    return false;
                if(distance >= advance && pri <= priority)  //XXX 等号添加
                { 
                    pri = priority;
                    return true;
                } 
                return false;
            }
        public:
            UInt16 cond;     //释放条件
            UInt8 prob;     //释放几率 (x/100)
            UInt16 distance;  //攻击距离  
            UInt8 priority;   //优先级
    };

#ifdef SkillT
    class SkillScope 
    { 
        public:
            SkillScope(){}
#else
    class SkillScope : public ObjectBaseT<UInt16>
    { 
        public:
            SkillScope(UInt16 id , const std::string& name)
                : ObjectBaseT<UInt16>(id,name) { } 
#endif
            bool InTheScope(UInt16 loaclX ,UInt16 localY ,UInt16 targetX , UInt16 targetY);
        public:
            UInt8 area;  //范围类型  0-单体 1-全体  2-目标扩散型 3-地形半径扩散型  4-地形直线扩散型 4-无限范围
            UInt16 x ;
            UInt16 y ;
            UInt16 radx;  //范围半径
            UInt16 rady;
    };
    class SkillEffect : public ObjectBaseT<UInt16>
    { 
        public:
#ifdef SkillT
            SkillEffect(){}
#else
            SkillEffect(UInt16 id , const std::string& name)
                : ObjectBaseT<UInt16>(id,name) { } 
#endif
        public:
            UInt8 skillType;

            UInt16 buffId;

            UInt16 damage;   //基础伤害
            float damageP;   //伤害百分比

            UInt16 trerapy;  //基础治疗
            float trerapyP;  //治疗百分比

            UInt8 stiffFixed;

            UInt8 avoidhurt ;
            UInt8 beatBack;
            // UInt16 defend;   //附加防御增加
            // float defendP;   //附加防御百分比增加
    };

    class SkillBuff : public ObjectBaseT<UInt16>
    { 
        public:
            SkillBuff(UInt16 id , const std::string& name)
                : ObjectBaseT<UInt16>(id,name) { } 
            void SetCount(UInt16 count1){ count = count1;}
            void SetSide(UInt8 side1){ side = side1;}
            void SetType(UInt8 type1){ type = type1;}
        public:
            std::vector<UInt8> attrIds;
            std::vector<UInt8> valueP;
            std::vector<UInt16> value;
            UInt16 count;   //BATTLE
            UInt8 side;
            UInt8 type;
    } ;


    typedef ObjectMapT<SkillCondition, UInt16> SkillConditionManager;
#define skillConditionManager SkillConditionManager::Instance()

    typedef ObjectMapT<SkillScope, UInt16> SkillScopeManager;   
#define skillScopeManager SkillScopeManager::Instance()

    typedef ObjectMapT<SkillEffect, UInt16> SkillEffectManager;   
#define skillEffectManager SkillEffectManager::Instance()

    typedef ObjectMapT<SkillBuff, UInt16> SkillBuffManager;   
#define skillBuffManager SkillBuffManager::Instance()

    class Skill : public ObjectBaseT<UInt16>
    {
        public:
#ifdef SkillT
            Skill(SkillCondition * sc, SkillScope* ss, SkillEffect* se):_sc(sc),_ss(ss),_se(se){}
#else
            Skill(UInt16 id , const std::string& name)
                : ObjectBaseT<UInt16>(id,name) { } 
            const SkillCondition * GetSkillCondition() const {return skillConditionManager[_conditionId];} 
            const SkillScope * GetSkillScope() const {return skillScopeManager[_scopeId];}
            const SkillEffect * GetSkillEffect() const {return skillEffectManager[_effectId];}
            void LoadSkill(UInt16 conditionId, UInt16 scopeId,UInt16 effectId, UInt16 cd,UInt16 actionBeforeCd, UInt16 actionCd, UInt16 actionBackCd, UInt16 frozeTime, UInt16 mpCost, UInt8 superSkill, UInt8 attackCount, UInt16 lstTime)
            { 
                _conditionId = conditionId;
                _scopeId = scopeId;
                _effectId = effectId;
                _cd = cd ;
                _actionBeforedCd = actionBeforeCd;
                _actionCd = actionCd;
                _actionBackCd = actionBackCd;
                _frozeTime = frozeTime;
                _mpCost = mpCost;
                _superSkill = superSkill;
                _attackCount = attackCount;
                _lstTime = lstTime;
            } 
#endif
            UInt16 GetCd() const {return _cd;}  //技能冷却时间
            UInt16 GetActionCd() const { return _actionBeforedCd+_actionCd;}
            UInt16 GetActionCd1() const { return _actionBeforedCd;}
            UInt16 GetActionCd2() const { return _actionCd;}
            UInt16 GetActionBackCd() const { return _actionBackCd;}
            UInt16 GetFrozrTime() const { return _frozeTime;}
            UInt16 GetMpCost() const { return _mpCost;}
            UInt8 GetSuperSkill()const { return _superSkill;}
            UInt8 GetAttackCount()const { return _attackCount;}
            UInt16 GetLstTime()const { return _lstTime;}
        private:
#ifdef SkillT
            SkillCondition * _sc;
            SkillScope * _ss;
            SkillEffect * _se;
#else
            UInt16 _conditionId;
            UInt16 _scopeId;
            UInt16 _effectId;
#endif

            UInt16 _cd;
            UInt16 _actionBeforedCd;  //技能前置
            UInt16 _actionCd;         //技能动作开始时间
            UInt16 _actionBackCd;     //技能后置
            UInt16 _frozeTime;     //技能冻结
            UInt16 _mpCost;
            UInt8 _superSkill;
            UInt8 _attackCount;
            UInt16 _lstTime;   //持续时间
    };

    typedef ObjectMapT<Skill, UInt16> SkillManager;   
#define skillManager SkillManager::Instance()
}
#endif // SKILLTABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


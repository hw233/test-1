
#ifndef SKILLTABLE_H_
#define SKILLTABLE_H_

#include "Config.h"
#include "GObject/GObjectManager.h"
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

    class SkillCondition : public ObjectBaseT<UInt16>
    { 
        public:
            SkillCondition(UInt16 id, const std::string& name)
                : ObjectBaseT<UInt16>(id,name) ,cond(0),prob(0),distance(0),priority(0)
            { } 
            ~SkillCondition(){}
            bool MeetCondition(UInt16 advance ,UInt8& pri) const 
            {
                if(distance >= advance && pri < priority)
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
    class SkillScope : public ObjectBaseT<UInt16>
    { 
        public:
            SkillScope(UInt16 id , const std::string& name)
                : ObjectBaseT<UInt16>(id,name) { } 
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
            SkillEffect(UInt16 id , const std::string& name)
                : ObjectBaseT<UInt16>(id,name) { } 
        public:
            UInt8 skillType;

            UInt16 damage;   //基础伤害
            float damageP;   //伤害百分比

            UInt16 trerapy;  //基础治疗
            float trerapyP;  //治疗百分比

            // UInt16 defend;   //附加防御增加
            // float defendP;   //附加防御百分比增加
    };

    class SkillBuff : public ObjectBaseT<UInt16>
    { 
        public:
            SkillBuff(UInt16 id , const std::string& name)
                : ObjectBaseT<UInt16>(id,name) { } 
        public:
            std::vector<UInt8> attrIds;
            std::vector<UInt8> valueP;
            std::vector<UInt16> value;
            UInt8 count;
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
            Skill(UInt16 id , const std::string& name)
                : ObjectBaseT<UInt16>(id,name) { } 
            //Skill(SkillCondition * sc, SkillScope* ss, SkillEffect* se):_sc(sc),_ss(ss),_se(se){}
            const SkillCondition * GetSkillCondition() const {return skillConditionManager[_conditionId];} 
            const SkillScope * GetSkillScope() const {return skillScopeManager[_scopeId];}
            const SkillEffect * GetSkillEffect() const {return skillEffectManager[_effectId];}
            void LoadSkill(UInt16 conditionId, UInt16 scopeId,UInt16 effectId,UInt16 cd, UInt16 actionCd, UInt16 actionBackCd)
            { 
                _conditionId = conditionId;
                _scopeId = scopeId;
                _effectId = effectId;
                _cd = cd ;
                _actionCd = actionCd;
                _actionBackCd = actionBackCd;
            } 
            UInt16 GetCd() const {return _cd;}
            UInt16 GetActionCd() const { return _actionCd;}
            UInt16 GetActionBackCd() const { return _actionBackCd;}
        private:
            UInt16 _conditionId;
            UInt16 _scopeId;
            UInt16 _effectId;
            UInt16 _cd;
            UInt16 _actionCd;
            UInt16 _actionBackCd;
    };

    typedef ObjectMapT<Skill, UInt16> SkillManager;   
#define skillManager SkillManager::Instance()
}
#endif // SKILLTABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


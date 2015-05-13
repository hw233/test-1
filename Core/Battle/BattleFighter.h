
#ifndef BATTLEFIGHTER_H_
#define BATTLEFIGHTER_H_
//#include "GObject/Fighter.h"
#include "GData/AttrExtra.h"   
#include <set>
#include "BattleObject.h"
#include "GObject/Fighter.h"
#include "BattleAction.h"
#include "Battle/BattleSimulator.h"
#include "Script/GameActionLua.h"
#include "GObject/Country.h"

namespace Script
{
    class BattleFormula;
}
namespace GObject
{
    class Fighter;
}
namespace Battle
{
#define  ACTION_WAIT 0  //动作延迟起作用
#define  ACTION_HAPPEN 1  //动作立即起作用
#define  MYFIGHTERMAX 10
#define  A_SUB_B(x,y) (x)>(y)?(x)-(y):0
    enum 
    {
        e_none = 0, //待机
        e_run = 1, //行进
        e_attack_near = 2, //近距离攻击
        e_attack_middle = 3, //中距离攻击
        e_attack_distant = 4, //远距离攻击
        e_image_attack = 5,  //魔法攻击
        e_image_therapy = 6, //魔法治疗
        e_attack_counter = 7, //反击
        e_be_attacked = 8 , //被攻击
        e_run_attack = 9 ,   //跑攻 (骑兵专用)
        e_object_image = 10
    };

    enum
    {
        e_attr_attack = 0,
        e_attr_magatk = 1,
        e_attr_defend = 2,
        e_attr_magdef = 3,
        e_attr_critical = 4,
        e_attr_criticalDef = 5,
        e_attr_hit = 6,
        e_attr_evade = 7,
        e_attr_max
    };

    struct BattleBuff
    {
        UInt16 buffId;
        UInt8 count;
        std::vector<UInt16> value;
        BattleBuff(UInt16 buffId1, UInt8 count1):buffId(buffId1),count(count1){}
    };

    struct ActionBase
    {
        ActionBase(UInt16 skillId /*UInt16 condition ,UInt16 scope, UInt16 effect*/):/*_condition(condition),_scpoe(scope),_effect(effect),*/_skillId(skillId),_cd(0),_priority(0) { } 
        ActionBase():/*_condition(0),_scpoe(0),_effect(0),*/_skillId(0),_cd(8),_priority(0){}
        UInt16 _skillId;
        //UInt16 _condition ;  // 触发条件编号
        //UInt16 _scpoe ;      // 触发范围编号
        //UInt16 _effect ;     // 触发效果编号
        UInt8 _cd;           //行动cd  
        UInt8 _priority ;  //触发优先级
    };
    struct lt_absort
    {  
        bool operator()(ActionBase& a, ActionBase& b) const { return a._priority > b._priority;}
    };

    typedef std::list<ActionBase /*, lt_absort*/> ActionSort;

    class BattleField;

    class BattleFighter:
        public BattleObject
    {
        friend class BattleSimulator;
        public:
            BattleFighter(UInt8 Class ,Script::BattleFormula * bf,GObject::Fighter * = NULL,UInt8 pointX = 0, UInt8 pointY = 0);
            virtual ~BattleFighter();

            void setFighter( GObject::Fighter * f );

            void StartAction(UInt8 actionType);
            inline UInt8 GetActionLast(){ return _actionLast;}  //获得当前状态
            inline UInt8 GetActionBackLast(){ return _actionBackLast;}  //获得当前状态

            UInt8 GetSide() {return _fighter->GetSide();}
            void SetGroundX(UInt8 x){_groundX = x;}
            void SetGroundY(UInt8 y){_groundY = y;}
            UInt8 GetGroundX(){ return _groundX;}
            UInt8 GetGroundY(){ return _groundY;}
            UInt8 GetTypeId() { return _fighter->GetTypeId();}

            virtual void Action();  //行动
            //移动
             void GoForward(UInt8 flag = 0 ,UInt16 advance = 0);
             ActionPackage MakeActionEffect();   //实现动作效果  伤害 法术等

            //被击
            UInt16 BeActed(BattleAction *  bAction ); //是否延迟

            //添加本身数据包

            //战斗时期
            //从战斗中或许自己该有的行为
            void GetActionFromField();
            //updateAction
            void UpdateActionList();

            ActionBase GetActionCurrent(UInt16 advance);
            void SetField(BattleField* bfield){ _field = bfield;}

            BattleField * GetField();

            UInt8 GetMovePower();
            UInt8 GetClass(){ return _fighter->GetClass();}
            UInt8 GetAttackRange();
            UInt8 GetDistance(){ return 1;}   //
            UInt16 GetId(){ if(!_fighter) return 2; return _fighter->getId();}

            void setNumber(UInt8 num){ _number = num;}
            UInt8 getNumber(){ return _number; }

            void setMainFighter(BattleFighter * bf){
                m_mainFighter = bf;
                setHP(m_mainFighter->getHP());
                for(UInt8 i = e_attr_attack ; i < e_attr_max; ++i)
                { 
                    attrBase[i] = m_mainFighter->GetBattleAttr(i);
                } 
            }

            void PutBattleFighters(BattleSimulator& bsim);
            BattleFighter* getMyFighters(UInt8 index);

            virtual UInt16 GetRad(){ if(m_mainFighter && m_mainFighter != this) return m_mainFighter->GetRad(); if(_fighter) return _fighter->GetRad(); return 0;}
            void SetEnterPos(UInt8 x1 , UInt8  y1){ if(!_fighter)return ; EnterX = x1; EnterY = y1;}
            inline UInt8 GetEnterPosX(){ return EnterX;}
            inline UInt8 GetEnterPosY(){ return EnterY;}

            virtual void InsertFighterInfo(Stream& st ,UInt8 flag = 0);

            void SetBattleIndex(UInt8 x) { _battleIndex = x;}
            UInt16 GetBattleIndex() { return _battleIndex;}

            virtual void SetMinXY(UInt16 x,UInt16 y) { _minX = x; _minY = y; resetBattleStatue();} //入场
            UInt16 GetMinX(){return _minX;}
            UInt16 GetMinY(){return _minY;}
            
            void SetSideInBS(UInt8 side) { _sideInBS = side;}
            UInt8 GetSideInBS() { if(m_mainFighter&&m_mainFighter!= this) return m_mainFighter->GetSideInBS(); return _sideInBS;}

            void SetBattleTargetPos(UInt16 x,UInt16 y){ _battleTargetX = x; _battleTargetY = y;}

            virtual UInt8 GetBSNumber() { return _number + GetSideInBS()*GetField()->GetFirstSize();}

            void SetNowTime(UInt32 time ){ if(_nowTime != time ) SetGone(false); _nowTime = time;}
            UInt8  GetNowTime() { return _nowTime;}

            //Virtual 
            virtual bool PreGetObject(){ return false;}  //设定攻击对象，以及战斗
            virtual void BuildLocalStream(UInt8 wait = 0 , UInt8 param = 0);
            virtual UInt16 GetTargetDistance(){ return -1;}

            UInt16 GetSpeed() {return 37;} 

            virtual void resetBattleStatue() = 0;

            virtual void SetGone(bool v){ }
            virtual bool GetGone(){return true;}

            virtual UInt8 GetRideCount() {return 1;}
            
            UInt32 GetAttack() { return GetTotalAttr(e_attr_attack);}

            UInt32 GetDefend(){ return GetTotalAttr(e_attr_defend);}
            UInt32 GetImageDefend(){ return GetTotalAttr(e_attr_magatk);}
            UInt32 GetHit() { return GetTotalAttr(e_attr_hit);}
            UInt32 GetEvade() {return GetTotalAttr(e_attr_evade);}
            UInt32 GetCritical() { return GetTotalAttr(e_attr_critical);}
            UInt32 GetCriticalDef() { return GetTotalAttr(e_attr_criticalDef);}

            UInt32 GetWreck() { return _wreck;}
            UInt32 GetParry(){ return _parry;}

            ImagePackage MakeImageEffect();

            UInt32 GetBattleAttr(UInt8 index, UInt8 type =0)
            { 
                if(index >= e_attr_max)
                    return 0;
                switch(type)
                { 
                    case 0:
                        return attrBase[index];
                    case 1:
                        return attrAdd[index];
                    case 2:
                        return attrSub[index];
                } 
                return 0;
            } 

            void SetBattleAttr(UInt8 index , UInt32 val, UInt8 type)
            { 
                if(index >= e_attr_max)
                    return; 
                switch(type)
                { 
                    case 0:
                        attrBase[index] = val;
                        break;
                    case 1:
                        attrAdd[index] = val;
                        break;
                    case 2:
                        attrSub[index] = val;
                        break;
                } 
            } 

            UInt32 GetTotalAttr(UInt8 index)
            { 
                if(index >= e_attr_max)
                    return 0;
                return A_SUB_B(attrBase[index]+attrAdd[index] , attrSub[index]);
            } 

            void AddBuff(UInt16 buffId);
            void CheckBuff();

        protected:
            UInt8 _crick;  //硬直
            UInt8 _actionLast ;   //动作持续
            UInt8 _actionBackLast ;   //动作收招持续
            //std::list<BattleObject*>  targetList; //对象列表 (待解)
            UInt8 _actionType;  // 动作类型

            Script::BattleFormula * _formula;
            BattleFighter * m_fighters[MYFIGHTERMAX];
            BattleFighter * m_mainFighter;   //主将指针
            UInt8 _number;   //所在阵营

            ActionBase _ab;

            ActionSort  preActionList;   //动作行为列表O
            ActionSort  preActionCD;     //待CD动作行为列表

            GObject::Fighter * _fighter;
            BattleField * _field;

            UInt8 EnterX;
            UInt8 EnterY;

            //属性
            UInt8 _crickSum;  //硬直累计
            UInt8 _crickLev; //硬直等级

            //霸体 
            UInt8 _stoic;
            UInt8 _stoicLev;

            //UInt32 _attack_near;
            //UInt32 _attack_distance;
            //UInt32 _attack_image;
            //UInt32 _defend;
            //UInt32 _defend_image;
            //UInt32 _critical; //暴击 
            //UInt32 _defend_critical; //抗暴
            //UInt32 _hit;  //命中
            //UInt32 _evade;  //闪避

            UInt32 _wreck; //破击
            UInt32 _parry;  //格挡

            UInt8 _groundX;
            UInt8 _groundY;

            UInt16 _battleIndex;

            UInt32 _nowTime; //行动时间

            UInt16 _minX;  //入场X坐标  //TODO
            UInt16 _minY;  //入场Y坐标  //TODO

            UInt16 _battleTargetX;
            UInt16 _battleTargetY;

            UInt8 _sideInBS;

            UInt32 attrBase[e_attr_max];
            UInt32 attrAdd[e_attr_max];
            UInt32 attrSub[e_attr_max];

            std::list<BattleBuff> bufflst;
    };

}
#endif // BATTLEFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */



#ifndef BATTLEFIGHTER_H_
#define BATTLEFIGHTER_H_
//#include "GObject/Fighter.h"
#include "GData/AttrExtra.h"   
#include <set>
#include "BattleObject.h"
#include "GObject/Fighter.h"
#include "BattleAction.h"
#include "Battle/BattleSimulator.h"
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
        e_be_attacked = 8
    };

    struct ActionBase
    {
        ActionBase(UInt16 condition ,UInt16 scope, UInt16 effect):_condition(condition),_scpoe(scope),_effect(effect),_cd(0),_priority(0) { } 
        ActionBase():_condition(0),_scpoe(0),_effect(0),_cd(8),_priority(0){}
        UInt16 _condition ;  // 触发条件编号
        UInt16 _scpoe ;      // 触发范围编号
        UInt16 _effect ;     // 触发效果编号
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
            BattleFighter(Script::BattleFormula * bf,GObject::Fighter * = NULL,UInt8 pointX = 0, UInt8 pointY = 0);
            virtual ~BattleFighter();

            void setFighter( GObject::Fighter * f );

            void StartAction(UInt8 actionType);
            inline UInt8 GetActionLast(){ return _actionLast;}  //获得当前状态

            UInt8 GetSide() {return _fighter->GetSide();}
            void SetGroundX(UInt8 x){_groundX = x;}
            void SetGroundY(UInt8 y){_groundY = y;}
            UInt8 GetGroundX(){ return _groundX;}
            UInt8 GetGroundY(){ return _groundY;}

            void Action();  //行动
            //移动
            virtual void GoForward(UInt16 targetX,UInt16 targetY,UInt16 advance);
             ActionPackage MakeActionEffect();   //实现动作效果  伤害 法术等

            //被击
            void BeActed(BattleAction  bAction);

            //添加本身数据包
            void BuildLocalStream(UInt8 wait = 0 , UInt8 param = 0);

            //战斗时期
            //从战斗中或许自己该有的行为
            void GetActionFromField();
            //updateAction
            void UpdateActionList();

            ActionBase GetActionCurrent(UInt8 advance);
            void SetField(BattleField* bfield){ _field = bfield;}
            BattleField * GetField(){ return _field;}

            UInt8 GetRide(){ return 3 ;} //TODO
            UInt8 GetClass(){ return _fighter->GetClass();}
            UInt8 GetDistance(){ return 1;}  
            UInt16 GetId(){ if(!_fighter) return 0; return _fighter->getId();}

            void setNumber(UInt8 num){ _number = num;}
            UInt8 getNumber(){ return _number; }

            void setMainFighter(BattleFighter * bf){m_mainFighter = bf;}

            void PutBattleFighters(BattleSimulator& bsim);
            BattleFighter* getMyFighters(UInt8 index);

            UInt16 GetRad(){ if(_fighter) return _fighter->GetRad(); return 0;}
            void SetEnterPos(UInt8 x1 , UInt8  y1){ if(!_fighter)return ; EnterX = x1; EnterY = y1;}
            inline UInt8 GetEnterPosX(){ return EnterX;}
            inline UInt8 GetEnterPosY(){ return EnterY;}


            void InsertFighterInfo(Stream& st ,UInt8 flag = 0);

        private:
            Script::BattleFormula * _formula;
            BattleFighter ** m_fighters;
            BattleFighter * m_mainFighter;   //主将指针
            UInt8 _number;   //所在阵营

            ActionBase _ab;
            UInt8 _actionType;  // 动作类型
            UInt8 _actionLast ;   //动作持续
            std::list<BattleObject*>  targetList; //对象列表 (待解)

            ActionSort  preActionList;   //动作行为列表O
            ActionSort  preActionCD;     //待CD动作行为列表


            GObject::Fighter * _fighter;
            BattleField * _field;
            UInt8 EnterX;
            UInt8 EnterY;

            //属性
            UInt8 _crick;  //硬直
            UInt8 _crickSum;  //硬直累计
            UInt8 _crickLev; //硬直等级

            //霸体 
            UInt8 _stoic;
            UInt8 _stoicLev;
            
            UInt32 _attack_near;
            UInt32 _attack_distance;

            UInt32 _hit;
            UInt32 _evade; 
            UInt32 _critical; //暴击 
            UInt32 _wreck; //破击
            UInt32 _parry;  //格挡

            UInt8 _groundX;
            UInt8 _groundY;

    };

    class BattleRideFighter :
        public BattleFighter
    {
        public:
            virtual void GoForward(UInt16 targetX,UInt16 targetY,UInt16 advance);
    };
}
#endif // BATTLEFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */



#ifndef BATTLEFIGHTER_H_
#define BATTLEFIGHTER_H_
//#include "GObject/Fighter.h"
#include "GData/AttrExtra.h"                                                                                                      
#include <set>
#include "BattleObject.h"
#include "GObject/Fighter.h"
#include "BattleAction.h"
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
        ActionBase(UInt16 condition ,UInt16 scope, UInt16 effect):_condition(condition),_scpoe(scope),_effect(effect),_cd(0) { } 
        UInt16 _condition ;
        UInt16 _scpoe ;
        UInt16 _effect ;
        UInt8 _cd;
        UInt8 priority ;  //触发优先级
    };
    struct lt_absort
    {  
        bool operator()(ActionBase& a, ActionBase& b) const { return a.priority > b.priority;}
    };

    typedef std::list<ActionBase /*, lt_absort*/> ActionSort;

    class BattleField;

    class BattleFighter:
        public BattleObject
    {
        friend class BattleSimulator;
        public:
            BattleFighter(Script::BattleFormula * bf,GObject::Fighter * = NULL,BattleField * field = NULL ,UInt8 pointX = 0, UInt8 pointY = 0);
            virtual ~BattleFighter();

            void setFighter( GObject::Fighter * f );

            void StartAction(UInt8 actionType);
            inline UInt8 GetActionLast(){ return _actionLast;}  //获得当前状态

            void Action();  //行动

            //移动
            void GoForward(UInt8 targetX,UInt8 targetY,UInt8 advance);

            void MakeActionEffect();   //实现动作效果  伤害 法术等

            //被击
            void BeActed(BattleAction  bAction);

            //添加本身数据包
            void BuildLocalStream(UInt8 wait = 0 , UInt8 param = 0);

            //从战场中或许自己该有的行为
            void GetActionFromField();

            //updateAction
            void updateActionList();

            ActionBase GetActionCurrent(UInt8 advance);
        private:
            Script::BattleFormula * _formula;

            UInt8 _actionType;  // 动作类型
            UInt8 _actionLast ;   //动作持续
            std::list<BattleObject*>  targetList; //对象列表 (待解)

            ActionSort  preActionList;
            ActionSort  preActionCD;


            GObject::Fighter * _fighter;
            BattleField * _field;

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
             
    };
}
#endif // BATTLEFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


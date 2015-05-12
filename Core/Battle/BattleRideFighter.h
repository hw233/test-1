#ifndef BATTLERIDEFIGHTER_H_
#define BATTLERIDEFIGHTER_H_
#include "BattleFighter.h"

namespace Battle
{
    class BattleRideFighter:
        public BattleFighter
    {
        public:
            BattleRideFighter(Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):
                BattleFighter(2,bf,f,pointX,pointY),_target(NULL),count(0),isRunSend(false)
        { 

        } 
            virtual void Action();
            virtual bool PreGetObject(); 
            void PreGetObject1();
            virtual void BuildLocalStream(UInt8 wait = 0 , UInt8 param = 0);
            virtual UInt16 GetTargetDistance();
            virtual void resetBattleStatue();
            UInt8 GetBattleDirection();
            UInt8 CheckTarget();
            virtual void SetGone(bool v){ isGone = v;}
            virtual bool GetGone(){return isGone;}
            virtual UInt8 GetRideCount() {return count-1;}
            virtual UInt16 GetBaseActionNum(){return 5;}
        private:
            BattleFighter * _target ; 
            UInt8 count; //控制来回次数
            bool isRunSend;
            bool isGone;
    };
}
#endif // BATTLERIDEFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


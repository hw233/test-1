
#ifndef BATTLEADVICEFIGHTER_H_
#define BATTLEADVICEFIGHTER_H_

#include "BattleFighter.h"
namespace Battle
{
    class BattleAdviceFighter:
        public BattleFighter
    {
        public:
            BattleAdviceFighter(Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):
                BattleFighter(3,bf,f,pointX,pointY),_target(NULL)
        { 

        } 
            virtual void Action();
            virtual bool PreGetObject(); 
            //virtual void BuildLocalStream(UInt8 wait = 0 , UInt8 param = 0);
            virtual UInt16 GetTargetDistance() {return 0;};
            virtual void resetBattleStatue();
        private:
            BattleObject * _target;
    };
}

#endif // BATTLEADVICEFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


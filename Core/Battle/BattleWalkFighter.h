#ifndef BATTLEWALKFIGHTER_H_
#define BATTLEWALKFIGHTER_H_
#include "BattleFighter.h"

namespace Battle
{
    class BattleWalkFighter:
        public BattleFighter
    {
        public:
            BattleWalkFighter(Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):
                BattleFighter(1,bf,f,pointX,pointY)
        { 
            _count = 0;
            _isHighSpeed = false;
        } 
            //virtual void Action();
            virtual bool PreGetObject(); 
            virtual void BuildLocalStream(UInt8 wait = 0 , UInt8 param = 0);
            virtual UInt16 GetTargetDistance();
            virtual void resetBattleStatue();
            virtual UInt16 GetBaseActionNum(){return 4;}
            virtual UInt16 GetSpeed();// {return 20;} 
            //virtual UInt8 NormolAttack();
            //virtual UInt8 NormolImage();
            //virtual UInt8 NormolObject();
             void SetHighSpeed(bool v){ _isHighSpeed = v; }
            bool GetHighSpeed(){return _isHighSpeed;}
        private:
            //BattleObject* _target; 
            UInt8 _count;
            bool _isHighSpeed;  //是否为左快的行 (步兵专用)
    };
}
#endif // BATTLEWALKFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


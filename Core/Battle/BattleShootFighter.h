
#ifndef BATTLESHOOTFIGHTER_H_
#define BATTLESHOOTFIGHTER_H_

#include "BattleFighter.h"
namespace Battle
{
    class BattleShootFighter:
        public BattleFighter
    {
        public:
            BattleShootFighter(Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY):
                BattleFighter(3,bf,f,pointX,pointY),_target(NULL)
            { 

            }
            
            /*
            virtual ~BattleShootFighter() 
            {
                std::cout<<" shootFighter destroy "<<std::endl;
            }
            */


            virtual void Action();
            virtual bool PreGetObject(); 
            //virtual void BuildLocalStream(UInt8 wait = 0 , UInt8 param = 0);
            virtual UInt16 GetTargetDistance() {return 0;};
            virtual void resetBattleStatue();
            virtual UInt16 GetBaseActionNum(){return 6;}
        private:
            BattleObject * _target;
    };
}

#endif // BATTLESHOOTFIGHTER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


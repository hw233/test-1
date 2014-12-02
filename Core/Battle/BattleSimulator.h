
#ifndef BATTLESIMULATOR_H_
#define BATTLESIMULATOR_H_
#include "BattleField.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
namespace Script
{
    class BattleFormula;
}          
namespace Battle
{
    struct Point
    {
        UInt8 x;
        UInt8 y;
        Point(UInt8 px,UInt8 py):x(px),y(py){}
    };
    class BattleSimulator:
        public BattleField
    {
        public:
            BattleSimulator();
            virtual ~BattleSimulator() {};
            inline int getId() {return _id;} 

            BattleFighter * newFighter( UInt8 x , UInt8 y, GObject::Fighter *);
            void start(UInt8 prevWin = 0xFF, bool checkEnh = true);   

            //Script
            inline Script::BattleFormula * getFormula() { return _formula; }
            inline void setFormula(Script::BattleFormula * formula) { _formula = formula; }

        private:
            int _id;
            Script::BattleFormula * _formula; 
    };
}
#endif // BATTLESIMULATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


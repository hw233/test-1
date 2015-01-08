
#ifndef BATTLESIMULATOR_H_
#define BATTLESIMULATOR_H_
#include "BattleField.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
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
    struct Point
    {
        UInt8 x;
        UInt8 y;
        Point(UInt8 px,UInt8 py):x(px),y(py){}
    };

    class BattleSimulator : public BattleField 
    {
        public:
            BattleSimulator(BattleFighter * bf, BattleFighter * bo,bool rpt = false,UInt32 limitTime = 80);
            virtual ~BattleSimulator() {};
            inline int getId() {return _id;} 

            void start(UInt8 prevWin = 0xFF, bool checkEnh = true);   
            void InitFighters(UInt8 index, UInt8 flag = 0);

            //Script
            inline Script::BattleFormula * getFormula() { return _formula; }
            inline void setFormula(Script::BattleFormula * formula) { _formula = formula; }

            UInt8 GetWin();
            static BattleFighter* CreateFighter(UInt8 Class ,Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY);
        private:
            UInt32 _id;
            BattleFighter* _fgt[2];
            Script::BattleFormula * _formula; 
            Stream _packet;
            UInt32 _limitTime;
    };
}
#endif // BATTLESIMULATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


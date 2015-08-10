
#ifndef BATTLESIMULATOR_H_
#define BATTLESIMULATOR_H_
#include "BattleField.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "Battle/BattleAction.h"
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
            BattleSimulator(BattleFighter * bf, BattleFighter * bo,UInt8 distance = 1, UInt8 mapId1 = 0, UInt8 mapId2 = 0, bool rpt = false,UInt32 limitTime = 80);
            virtual ~BattleSimulator()  {}
            inline int getId() {return _id;} 

            void start(UInt8 prevWin = 0xFF, bool checkEnh = true);   
            void InitFighters(UInt8 index, UInt8 flag = 0);

            void InitFightersSide(UInt8 index);

            //Script
            inline Script::BattleFormula * getFormula() { return _formula; }
            inline void setFormula(Script::BattleFormula * formula) { _formula = formula; }

            UInt8 GetWin();
            static BattleFighter* CreateFighter(UInt8 Class ,Script::BattleFormula * bf ,GObject::Fighter * f , UInt8 pointX , UInt8 pointY);

            //AfterAction
            UInt8 doAction(UInt16 time = 0);
            UInt8 doAttack(UInt16 time = 0);
            UInt8 doImage(UInt16 time = 0);
            UInt8 doObjectMove(UInt16 time = 0, UInt8 cnt = 1);
            UInt8 ClearObjectPackage();
            UInt16 FighterAction(BattleFighter* bf,UInt16 curTime);
            
            void RandPosBegin(UInt8);

            void PosPrintf(UInt8 index);

            UInt8 FighterMove(UInt8 index, UInt16 lastTime, UInt16 cutTime);

            UInt16 GetTime(){return _time + GetTimeExtra();}

            bool GetStop();

            void SetAttackUp();

            UInt8 GetAttackUp(UInt8 side){if(side >1) return 0; return _attackUp[side];} 

            void EnergyUp(UInt32 time);
        private:
            UInt32 _id;
            BattleFighter* _fgt[2];
            Script::BattleFormula * _formula; 
            Stream _packet;
            UInt32 _limitTime;
            UInt16 _time;
            UInt8 _distance;
            //UInt16 _complate[2];
            UInt8 _attackUp[2];
    };
}
#endif // BATTLESIMULATOR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


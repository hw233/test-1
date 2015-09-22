
#ifndef ROBOT_H_
#define ROBOT_H_
#include "Config.h"
#include "GObject/Fighter.h"
namespace GData
{
    struct RobotInfo 
    {
        UInt8 ground;    
        std::vector<UInt16> fighters;
        UInt32 power;
        RobotInfo(){ ground = 0; fighters.clear(); power = 0;}
        RobotInfo(UInt16 g){ ground = g; power = 0;}
        void calcBattlePoint() 
        { 
            UInt32 p = 0;
            for(UInt8 i = 0; i < fighters.size(); ++i)
            { 
                GObject::Fighter* f = GObject::globalFighters[fighters[i]];
                p+= f->GetTotalPower();
            } 
            power = p;
        } 
        UInt32 GetPower(){ if(!power) calcBattlePoint(); return power;}
    };
    class Robot
    {
        public:
            void LoadRobot(UInt16 index,UInt8 ground, UInt16 fgt1, UInt16 fgt2, UInt16 fgt3, UInt16 fgt4, UInt16 fgt5)
            { 
                _robots[index] = RobotInfo(ground);
                _robots[index].fighters.push_back(fgt1);
                _robots[index].fighters.push_back(fgt2);
                _robots[index].fighters.push_back(fgt3);
                _robots[index].fighters.push_back(fgt4);
                _robots[index].fighters.push_back(fgt5);
            } 
            struct RobotInfo GetRobot(UInt16 index) 
            { 
                return _robots[index];
            } 
            UInt32 GetRobotSize(){return _robots.size();}
        private:
            std::map<UInt16, RobotInfo> _robots;
    };

    extern Robot robotInfo;
}

#endif // ROBOT_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


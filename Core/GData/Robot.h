
#ifndef ROBOT_H_
#define ROBOT_H_
#include "Config.h"
namespace GData
{
    struct RobotInfo 
    {
        UInt8 ground;    
        std::vector<UInt16> fighters;
        RobotInfo(){ ground = 0; fighters.clear();}
        RobotInfo(UInt16 g){ ground = g;}
    };
    class Robot
    {
        public:
            void LoadRobot(UInt16 index,UInt8 ground, UInt16 fgt1, UInt16 fgt2, UInt16 fgt3, UInt16 fgt4, UInt16 fgt5)
            { 
                _robots[index] = RobotInfo(index);
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


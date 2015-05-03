
#ifndef MAP_H_
#define MAP_H_

#include "Config.h"
namespace GData
{
    enum eLandForm
    {
        eUnCross = 0 , //不可通过的障碍
        eGrass   = 1 , //草地
        eTown    = 2 , //城镇
        eForest  = 3 , //森林
        eHill    = 4 , //山地
        eMax
    };

    typedef std::vector< std::vector<UInt8> > vecInfo;
    class Map
    {
        public:
            void loadMapInfo(UInt8 mapId);
            void loadCampInfo(UInt8 mapId);         
            UInt8 getlandForm(UInt8 mapId,UInt8 x,UInt8 y);  //由坐标获得地图上的地形
            vecInfo GetMapInfo() { return _mapInfo;}
            vecInfo GetCampInfo() { return _mapCamp;}

        private:
            vecInfo _mapInfo;    //地图信息
            vecInfo _mapCamp;//地图阵营信息
    };
    extern Map map;
}

#endif // MAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


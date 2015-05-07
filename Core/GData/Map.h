
#ifndef MAP_H_
#define MAP_H_

#include "Config.h"
#define MAP_MAX 100

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

    typedef std::vector<UInt8> vecInfo;
    class Map
    {
        public:
            void loadMapInfo(UInt8 index,vecInfo info);
            void loadCampInfo(UInt8 index,vecInfo info);        
            vecInfo GetMap(UInt8 mapId) { return _mapInfo[mapId-1];}
            vecInfo GetCamp(UInt8 mapId) { return _mapCamp[mapId-1];}
        private:
            std::vector<UInt8> _mapInfo[MAP_MAX];    //地图信息
            std::vector<UInt8> _mapCamp[MAP_MAX];//地图阵营信息
    };
    extern Map map;
}

#endif // MAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


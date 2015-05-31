
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

    class MapInfo
    {
        public:
            MapInfo(UInt8 w,UInt8 h,vecInfo t,vecInfo c) : width(w),height(h),tileInfo(t),campInfo(c) {}
            UInt8 GetWidth() const { return width;}
            UInt8 GetHeight() const { return height;}
            vecInfo GetTileInfo() const { return tileInfo;}
            vecInfo GetCampInfo() const { return campInfo;}
        private:
            UInt8 width;
            UInt8 height;
            vecInfo tileInfo;  //地形信息
            vecInfo campInfo;  //地图信息
    };
    class MapTable
    {
        public:
            void loadMapInfo(UInt8 mapId,MapInfo* info);
            MapInfo* GetMapInfo(UInt8 mapId);
        private:
            MapInfo* _mapInfo[MAP_MAX];
    };
    extern MapTable mapTable;
}

#endif // MAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


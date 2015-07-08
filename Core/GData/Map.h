
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
            MapInfo(UInt8 w,UInt8 h,UInt8 cn) : width(w),height(h),campNum(cn)
            {
                camp2direction.clear();
            }
            UInt8 GetWidth() const { return width;}
            UInt8 GetHeight() const { return height;}
            vecInfo GetTileInfo() const { return tileInfo;}
            vecInfo GetCampInfo() const { return campInfo;}
            UInt8 GetCampNum() const { return campNum;}
            void SetCampInfo(vecInfo info) { campInfo = info;}
            void SetTileInfo(vecInfo info) { tileInfo = info;}
            void InsertCampDir(UInt8 campId,UInt8 dir) { camp2direction[campId] = dir ;}
            UInt8 GetAttackDirect(UInt8 side) { return camp2direction[side];}
        private:
            UInt8 width;
            UInt8 height;
            UInt8 campNum;
            vecInfo tileInfo;  //地形信息
            vecInfo campInfo;  //地图信息
            std::map<UInt8,UInt8> camp2direction;
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


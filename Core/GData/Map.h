
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
        eForest  = 2 , //森林
        eTown    = 3 , //城镇
        eHill    = 4 , //山地
        eMax
    };

    struct NpcInfo
    {
        NpcInfo(UInt16 id,UInt8 px,UInt8 py) : fighterId(id),x(px),y(py) {}
        UInt16 fighterId;
        UInt8 x;
        UInt8 y;
    };

    typedef std::vector<UInt8> vecInfo;


    //进攻方向 0 auto 1 left 2 right 3 up 4 down 
    class MapInfo
    {
        public:
            MapInfo(UInt8 w,UInt8 h) : width(w),height(h)
            {
                camp2direction.clear();
            }
            UInt8 GetWidth() const { return width;}
            UInt8 GetHeight() const { return height;}
            vecInfo GetTileInfo() const { return tileInfo;}
            vecInfo GetCampInfo() const { return campInfo;}
            void SetCampInfo(vecInfo info) { campInfo = info;}
            void SetTileInfo(vecInfo info) { tileInfo = info;}
            void InsertCampDir(UInt8 campId,UInt8 dir) { camp2direction[campId] = dir ;}
            UInt8 GetAttackDirect(UInt8 side) { return camp2direction[side];}
            void SetNpcInfo(std::vector<NpcInfo> vecInfo ) { vecNpcInfo = vecInfo;}
            std::vector<NpcInfo> GetNpcInfo() const { return vecNpcInfo;}
            std::vector<UInt8> GetActCamp() const { return vecCamp;}
            void SetActCamp(std::vector<UInt8> vecForce) { vecCamp = vecForce;}
        private:
            UInt8 width;
            UInt8 height;
            vecInfo tileInfo;  //地形信息
            vecInfo campInfo;  //阵营信息
            std::map<UInt8,UInt8> camp2direction;
            std::vector<NpcInfo> vecNpcInfo;
            std::vector<UInt8> vecCamp;  //实际有哪些阵营
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


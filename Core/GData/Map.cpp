#include "Map.h"
#include "GObject/Country.h"
#include "Script/GameActionLua.h"

namespace GData
{
    Map map;
    void Map::loadMapInfo(UInt8 index, vecInfo info)
    { 
        _mapInfo[index] = info;
    } 


    void Map::loadCampInfo(UInt8 index , vecInfo info)
    {
        _mapCamp[index] = info;

    }
}

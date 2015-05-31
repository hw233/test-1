#include "Map.h"
#include "GObject/Country.h"
#include "Script/GameActionLua.h"

namespace GData
{
    MapTable mapTable;
    void MapTable::loadMapInfo(UInt8 mapId, MapInfo* info)
    { 
        _mapInfo[mapId] = info;
    } 

   MapInfo* MapTable::GetMapInfo(UInt8 mapId)
   {
       return _mapInfo[mapId];
   }
}

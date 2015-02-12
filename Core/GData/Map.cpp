#include "Map.h"
namespace GData
{
    Map map;
    void Map::loadMapInfo(UInt8 index, std::string info)
    { 
        map_info[index].push_back(info);
    } 

    const std::vector<std::string>& Map::GetMapInfo(UInt8 mapId)
    { 
        //std::map<UInt8,std::vector<std::string> >::iterator it = map_info.find(mapId);
        //if(it != map_info.end())
        //    return map_info[mapId];
        return map_info[mapId];
    } 
}

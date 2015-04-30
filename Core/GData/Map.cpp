#include "Map.h"
#include "country.h"
#include "GameActionLua.h"

namespace GData
{
    Map map;
    void Map::loadMapInfo(UInt8 mapId)
    { 
        Table t = GameAction()->GetMapInfo();
        for(UInt8 i = 0 ; i < t.size() ; ++i)
        {
            Table table = t.get(i+1);
            for(UInt8 j =0 ; j <  table.size() ;++j)
            {
                _mapInfo[i].insert(table.get(j));
            }
        }
    } 


    void Map::loadMapCampInfo(UInt8 mapId)
    { 
        Table t = GameAction()->GetGetCampInfo();
        for(UInt8 i = 0 ; i < t.size() ; ++i)
        {
            Table table = t.get(i+1);
            for(UInt8 j =0 ; j <  table.size() ;++j)
            {
                _mapCampInfo[i].insert(static<UInt8>(table.get(j)));
            }
        }
    }


    UInt8 getLandForm(UInt8 mapId, UInt8 x, UInt8 y)
    {
        UInt8 form = GameAction()->GetInfo(mapId,x+1,y+1);
        return form;
    }
}

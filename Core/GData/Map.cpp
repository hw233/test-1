#include "Map.h"
#include "GObject/Country.h"
#include "Script/GameActionLua.h"

namespace GData
{
    Map map;
    void Map::loadMapInfo(UInt8 mapId)
    { 
        Table t = GameAction()->GetMapInfo(mapId);
        for(UInt8 i = 0 ; i < t.size() ; ++i)
        {
            Table table = t.get<Table>(i+1);
            for(UInt8 j =0 ; j <  table.size() ; ++j )
            {
                UInt8 info = table.get<UInt8>(j+1);
                 _mapInfo[i].push_back(info);
            }
        }
    } 


    void Map::loadCampInfo(UInt8 mapId)
    { 
        Table t = GameAction()->GetCampInfo(mapId);
        for(UInt8 i = 0 ; i < t.size() ; ++i)
        {
            Table table = t.get<Table>(i+1);
            for(UInt8 j =0 ; j <  table.size() ;++j)
            {
                //UInt8 info = static_cast<UInt8>(table.get<Table>(j+1));
                //_mapCamp[i].push_back(info);
            }
        }
    }


    UInt8 getLandForm(UInt8 mapId, UInt8 x, UInt8 y)
    {
        UInt8 form = GameAction()->GetInfo(mapId,x+1,y+1);
        return form;
    }
}

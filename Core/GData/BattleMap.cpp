#include"BattleMap.h"

namespace GData
{
    BattleMapTable battleMapTable;
    bool SingleMapInfo::IsLink(UInt8 mapId)
    {
        if( links.empty() )
        {
            return false;
        }
        for(auto it = links.begin(); it != links.end(); ++it)
        {
            if( (*it) == mapId )
            {
                return true;
            }
        }
        return false;
    }

    SingleMapInfo* BattleMapInfo::GetSingleMapInfo(UInt8 mapId)
    {
        for(auto it = battleMapInfo.begin(); it != battleMapInfo.end(); ++it )
        {
            if( (*it)->GetMapId() == mapId )
            {
                return (*it);
            }
        }
        return NULL;
    }


    //获得出生点信息
    UInt8 BattleMapInfo::GetBornCity(UInt8 forceId)
    {
        //一种势力只能有一个势力
        for( auto it = battleMapInfo.begin(); it != battleMapInfo.end() ; ++it )
        {
            if( (*it)->GetForce() == forceId )
            {
                return (*it)->GetMapId();
            }
        }
        return 0;
    }



    void BattleMapTable::loadBattleMap(BattleMapInfo * info)
    {
        if( info == NULL )
        {
            return;
        }
        battleMapTable[info->GetBattleId()] = info;
    }

    BattleMapInfo* BattleMapTable::GetBattleMapInfo(UInt8 battleId)
    {
        if( battleId <= 0 )
            return NULL;
        return battleMapTable[battleId];
    }

    std::vector<UInt8> BattleMapInfo::GetForces()
    {
        std::vector<UInt8> vecForce;
        for( auto it = battleMapInfo.begin(); it != battleMapInfo.end();++it )
        {
            if( (*it)->GetForce() != 0 )
            {
                vecForce.push_back((*it)->GetForce());
            }
        }
        return vecForce;
    }
}

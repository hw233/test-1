#ifndef __BATTLEMAP_H__
#define __BATTLEMAP_H__
#include"Config.h"
#define BATTLE_MAP_MAX 100
namespace GData
{
    class SingleMapInfo 
    {
        public:
            SingleMapInfo(UInt8 id,UInt8 f) : mapId(id),force(f)
            {
                links.clear();
            }
            void SetLinks(std::vector<UInt8> vecLinks) { links = vecLinks;}
            ~SingleMapInfo() { mapId = 0 ; force = 0 ; links.clear();}
            UInt8 GetMapId() const { return mapId;}
            UInt8 GetForce() const { return force;}
            bool IsLink(UInt8 mapId);
            std::vector<UInt8> GetLinks() const { return links;}
        private:
            UInt8 mapId;  
            UInt8 force;  //which capacity the town
            std::vector<UInt8> links;  //link to other towns
    };

    class BattleMapInfo
    {
        public:
            BattleMapInfo(UInt8 id )  : battleId(id)
            {
                battleMapInfo.clear();
            }
            void SetSingleMapInfo(std::vector<SingleMapInfo*> vecSingleInfo) { battleMapInfo = vecSingleInfo;}
            std::vector<SingleMapInfo*> GetSingleMapInfos() { return battleMapInfo;}
            ~BattleMapInfo() { battleId = 0 ; battleMapInfo.clear();}
            UInt8 GetBattleId() const { return battleId;}
            SingleMapInfo* GetSingleMapInfo(UInt8 mapId);
            UInt8 GetMapNum() const { return battleMapInfo.size();}
            UInt8 GetBornCity(UInt8 forceId);
            std::vector<UInt8> GetForces();
        private:
            UInt8 battleId;
            std::vector<SingleMapInfo*> battleMapInfo;
    };

    class BattleMapTable
    {
        public:
            void loadBattleMap(BattleMapInfo * info);
            BattleMapInfo* GetBattleMapInfo(UInt8 battleId);
        private:
            BattleMapInfo* battleMapTable[BATTLE_MAP_MAX];
    };

    extern BattleMapTable battleMapTable;
}
#endif

#ifndef __BATTLEMAP_H__
#define __BATTLEMAP_H__
#include"Config.h"
#define BATTLE_MAP_MAX 100
namespace GData
{
    class SingleMapInfo 
    {
        public:
            SingleMapInfo(UInt8 id,UInt8 f,std::vector<UInt8> l) : mapId(id),force(f),links(l) {}
            ~SingleMapInfo() { mapId = 0 ; force = 0 ; links.clear();}
            UInt8 GetMapId() const { return mapId;}
            UInt8 GetForce() const { return force;}
            bool IsLink(UInt8 mapId);
        private:
            UInt8 mapId;  
            UInt8 force;  //which capacity the town
            std::vector<UInt8> links;  //link to other towns
    };

    class BattleMapInfo
    {
        public:
            BattleMapInfo(UInt8 id, std::vector<SingleMapInfo*> info )  : battleId(id), battleMapInfo(info){}
            ~BattleMapInfo() { battleId = 0 ; battleMapInfo.clear();}
            UInt8 GetBattleId() const { return battleId;}
            SingleMapInfo* GetSingleMapInfo(UInt8 mapId);
            UInt8 GetMapNum() const { return battleMapInfo.size();}
        private:
            UInt8 battleId;
            std::vector<SingleMapInfo*> battleMapInfo;
    };

    class BattleMapTable
    {
        public:
            ~BattleMapTable() { delete []battleMapTable;}
            void loadBattleMap(BattleMapInfo * info);
            BattleMapInfo* GetBattleMapInfo(UInt8 battleId);
        private:
            BattleMapInfo* battleMapTable[BATTLE_MAP_MAX];
    };

    extern BattleMapTable battleMapTable;
}
#endif

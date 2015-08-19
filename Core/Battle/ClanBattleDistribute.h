#ifndef __CLANBATTLEDISTRIBUTE_H__
#define __CLANBATTLEDISTRIBUTE_H__

#include"Config.h"
#include"GObject/Player.h"
#include"GObject/Clan.h"
#include"GData/Map.h"
#include"GData/BattleMap.h"

namespace GObject
{
    class Player;
    class Clan;
}

namespace GData
{
    class Map;
    class SingleMapInfo;
    class BattleMapInfo;
}


#define INIT_SOLDIER_NUM 10

namespace Battle
{

    struct InMyZoneEnemy
    {
        InMyZoneEnemy(UInt16 id,UInt8 rhs,UInt8 lhs) : fighterId(id) , x(rhs) , y( lhs) {}
        UInt16 fighterId;
        UInt8 x;
        UInt8 y;
    };

    class DistributeInfo   //军团战某个位置上的信息
    {
        public:
            DistributeInfo(UInt64 p,UInt16 f,UInt8 px,UInt8 py) : playerId(p) , fighterId(f) , posx(px) , posy(py),mainFighterHP(0)  {}
            ~DistributeInfo() {}
            UInt64 GetPlayerId() const { return playerId;}
            UInt16 GetFighterId() const { return fighterId;}
            void   SetFighterId(UInt16 id) { fighterId = id;}
            UInt8  GetPosX() const { return posx;}
            UInt8  GetPosY() const { return posy;}
            void   SetPosX(UInt8 px)  { posx = px ; }
            void   SetPosY(UInt8 py)  { posy = py ; }
            void   SetSoldiersHP(UInt32 childTypeId);
            void   SetMainFighterHP( UInt32 hp ) { mainFighterHP = hp;}
            UInt32   GetMainFighterHP() { return mainFighterHP;}
            void   SetSoldiersHP(std::vector<UInt32> vecHP ) { SoldiersHP = vecHP;}
            std::vector<UInt32> GetSoldiersHP() { return SoldiersHP;}
        private:
            UInt64 playerId;
            UInt16 fighterId;  //战将Id
            UInt8  posx;
            UInt8  posy;
            UInt32 mainFighterHP; //主将血量
            std::vector<UInt32> SoldiersHP;//小兵们的血量
    };



    class MapDistributeInfo   //一张地图的排布
    {
        public:
            MapDistributeInfo(UInt8 id) : mapId(id)
            {
                vecDistributeInfo.clear();
            }
            UInt8 GetMapId() const { return mapId;}
            std::vector<DistributeInfo*> GetDistributeInfo() const { return vecDistributeInfo;}
            void SetDistributeInfo(std::vector<DistributeInfo*> vecInfo ) { vecDistributeInfo = vecInfo;}
            bool HasBody() { return !vecDistributeInfo.empty();}
        private:
            UInt8 mapId; //地图id
            std::vector<DistributeInfo*> vecDistributeInfo;

    };

    class BattleDistribute  //一个房间的排布
    {
        public:
            ~BattleDistribute() { _room2Distribute.clear();}
            bool PutFighter(UInt8 mapId,GObject::Player* player,UInt16 fighterId, UInt8 x, UInt8 y,bool flag,UInt8 tag = 0 );
            bool RemoveFighter(UInt8 mapId, GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y); 
            bool MoveFighter(UInt8 mapId,GObject::Player* player,UInt8 curx,UInt8 cury,UInt8 destx,UInt8 desty,UInt8 tag);
            DistributeInfo* GetDistributeInfo(UInt32 roomId, UInt8 mapId, UInt8 x, UInt8 y);
            void RemoveDistributeInfo(UInt32 roomId,UInt8 mapId,DistributeInfo* info);
            UInt32 GetBattleRoomId(GObject::Player* player);
            DistributeInfo* GetDistributeInfo(std::vector<DistributeInfo*> &vecDistributeInfo,UInt8 x,UInt8 y);
            bool Check(GObject::Player* player);
            MapDistributeInfo* GetMapDistributionInfo(std::vector<MapDistributeInfo*>& vecMapDistributeInfo, UInt8 mapId);
            bool MoveFighterWithDiffTown(GObject::Player* player,UInt8 curMapId,UInt8 curx,UInt8 cury,UInt8 destMapId, UInt8 destx,UInt8 desty);
            UInt8 GetForceId(UInt64 playerId);
            void  GetBattleInfo(GObject::Player* player,Stream &st);
            void  NoticeAlliesAddFighter(GObject::Player* player, UInt16 fighterId);
            void  NoticeAlliesDelFighter(GObject::Player* player,UInt8 mapId,UInt8 posx,UInt8 posy);
            void  NoticeAlliesMoveFighter(GObject::Player* player,UInt8 curMapId,UInt8 curx,UInt8 cury,UInt8 destMapId,UInt8 destx,UInt8 desty);
            map<UInt32,std::vector<MapDistributeInfo*>> GetData() const { return _room2Distribute;}
            void UpdateMainFighterHP(UInt32 roomId,UInt8 mapId,UInt8 x,UInt8 y, UInt32 hp);
            void UpdateSoldiersHP(UInt32 roomId,UInt8 mapId,UInt8 x,UInt8 y,std::vector<UInt32> vecHP);
            void SetMainFighterAndSoldiersHP(UInt32 roomId,UInt8 mapId,UInt8 x,UInt8 y,std::vector<UInt32> vecSoldiersHP,UInt32 mainFighterHP);
            MapDistributeInfo* GetMapDistributionInfo(UInt32 roomId,UInt8 mapId);
            void MoveNpc(UInt32 roomId,UInt8 mapId,UInt16 fighterId,UInt8 curx,UInt8 cury,UInt8 destx,UInt8 desty);
            void PutNpc(UInt32 roomId,UInt8 mapId,UInt16 fighterId,UInt8 x,UInt8 y,bool flag = true);
            void RemoveNpc(UInt32 roomId,UInt8 mapId,UInt8 curx,UInt8 cury);
        private:
            map<UInt32,std::vector<MapDistributeInfo*>> _room2Distribute;
    };

    extern BattleDistribute battleDistribute;


}
#endif

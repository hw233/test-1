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

namespace Battle
{
    class DistributeInfo   //军团战某个位置上的信息
    {
        public:
            DistributeInfo(UInt64 p,UInt16 f,UInt8 px,UInt8 py) : playerId(p) , fighterId(f) , posx(px) , posy(py)  {
                std::cout << static_cast<UInt32> (count ++ )<< std::endl;
            }
            ~DistributeInfo() {}
            UInt64 GetPlayerId() const { return playerId;}
            UInt16 GetFighterId() const { return fighterId;}
            void   SetFighterId(UInt16 id) { fighterId = id;}
            UInt8  GetPosX() const { return posx;}
            UInt8  GetPosY() const { return posy;}
            void   SetPosX(UInt8 px)  { posx = px ; }
            void   SetPosY(UInt8 py)  { posy = py ; }
        private:
            UInt64 playerId;
            UInt16 fighterId;
            UInt8  posx;
            UInt8  posy;
            static UInt32 count ;
    };



    class MapDistributeInfo   //一张地图的排布
    {
        public:
            MapDistributeInfo(UInt8 id,std::vector<DistributeInfo*> vecDis) : mapId(id),vecDistributeInfo(vecDis) {}
            ~MapDistributeInfo() { vecDistributeInfo.clear();}
            UInt8 GetMapId() const { return mapId;}
            std::vector<DistributeInfo*> GetDistributeInfo() const { return vecDistributeInfo;}
            void SetDistributeInfo(std::vector<DistributeInfo*> vecInfo ) { vecDistributeInfo = vecInfo;}
        private:
            UInt8 mapId; //地图id
            std::vector<DistributeInfo*> vecDistributeInfo;

    };

    class BattleDistribute  //一个房间的排布
    {
        public:
            ~BattleDistribute() { _room2Distribute.clear();}
            bool PutFighter(UInt8 mapId,GObject::Player* player,UInt16 fighterId, UInt8 x, UInt8 y,bool flag);
            bool CancelPutFighter(UInt8 mapId, GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y); 
            bool MoveFighter(UInt8 mapId,GObject::Player* player,UInt8 curx,UInt8 cury,UInt8 destx,UInt8 desty);
            DistributeInfo* GetDistributeInfo(UInt32 roomId, UInt8 mapId, UInt8 x, UInt8 y);
            void RemoveDistributeInfo(UInt32 roomId,UInt8 mapId,DistributeInfo* info);
            UInt32 GetBattleRoomId(GObject::Player* player);
            DistributeInfo* GetDistributeInfo(std::vector<DistributeInfo*> &vecDistributeInfo,UInt8 x,UInt8 y);
            bool Check(GObject::Player* player);
            MapDistributeInfo* GetMapDistributionInfo(std::vector<MapDistributeInfo*>& vecMapDistributeInfo, UInt8 mapId);
            bool MoveFighterWithDiffTown(GObject::Player* player,UInt8 curMapId,UInt8 curx,UInt8 cury,UInt8 destMapId, UInt8 destx,UInt8 desty);
            UInt8 GetForceId(UInt64 playerId);
            void  GetBattleInfo(GObject::Player* player,Stream &st);
            void  GetAllies(GObject::Player* player,std::set<GObject::Player*>& playerSet);
            void  NoticeAlliesAddFighter(GObject::Player* player, UInt16 fighterId);
            void  NoticeAlliesDelFighter(GObject::Player* player,UInt8 mapId,UInt8 posx,UInt8 posy);
            void  NoticeAlliesMoveFighter(GObject::Player* player,UInt8 curMapId,UInt8 curx,UInt8 cury,UInt8 destMapId,UInt8 destx,UInt8 desty);
            map<UInt32,std::vector<MapDistributeInfo*>> GetData() const { return _room2Distribute;}

        private:
            map<UInt32,std::vector<MapDistributeInfo*>> _room2Distribute;
    };

    extern BattleDistribute battleDistribute;


}
#endif

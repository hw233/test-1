//#pragma once
#ifndef __CLANBATTLECITYSTATUS_H__
#define __CLANBATTLECITYSTATUS_H__

#include"Config.h"
#include"GData/BattleMap.h"
#include"Server/WorldServer.h"

namespace GData
{
    class SingleMapInfo;
    class BattleMapInfo;
}

namespace Battle
{
    class CityStatus
    {
        public:
            CityStatus(UInt8 cId,UInt8 of) : cityId(cId), ownForce(of) {}
            ~CityStatus() {}
            UInt8 GetCityId() const { return cityId;}
            UInt8 GetOwnForce() const { return ownForce;}
            void  SetOwnForce(UInt8 force) { ownForce = force;}
        private:
            UInt8 cityId; //(即mapId)
            UInt8 ownForce;
    };

    class RoomAllCityStatus
    {
        public:
            RoomAllCityStatus(UInt32 rId,UInt8 bId) : roomId(rId) ,battleId(bId),stage(0) {}
            void   Init();
            UInt32 GetRoomId() const { return roomId;}
            UInt8  GetBattleId() const { return battleId;}
            UInt8  GetCityOwnForce(UInt8 cityId);
            void   SetCityOwnForce(UInt8 cityId,UInt8 force);
            void   InsertCityStatus(UInt8 cityId,UInt8 ownforce) { vecCityStatus.push_back(new CityStatus(cityId,ownforce));}
            UInt8 GetStage() const { return stage;}
            void  SetStage(UInt8 s) { stage = s;}
            void  SetStage(UInt32 t);
            std::vector<CityStatus*> GetAllCityStatus() { return vecCityStatus;}
            bool  IsStop();
            std::set<UInt8> GetCanConfigure(UInt8 forceId);
            std::vector<UInt8> GetCaptureCitys(UInt8 forceId);
            CityStatus* GetCityStatus(UInt8 cityId);
            UInt8 GetCaptureCityNum();
        private:
            UInt32 roomId;
            UInt8 battleId;   //战役Id
            UInt8  stage;     //军团战阶段  0战术配置  1战术推演  2 结束 
            std::vector<CityStatus*> vecCityStatus;

    };


    class RoomAllCityStatusManager
    {
        public:
            void InsertRoomAllCityStatus(UInt32 roomId,UInt8 battleId);
            RoomAllCityStatus* GetRoomAllCityStatus(UInt32 roomId);
            void loadCityStatus(UInt32 roomId,UInt8 battleId,UInt8 cityId,UInt8 ownforce);
            std::vector<RoomAllCityStatus*> GetData() { return _vecRoomAllCityStatus;}
            void SetOwnForce(UInt32 roomId,UInt8 cityId,UInt8 ownForce);
            void RemoveRoomAllCityStatus(UInt32 roomId);
        private:
            std::vector<RoomAllCityStatus*> _vecRoomAllCityStatus;
    };

    extern RoomAllCityStatusManager roomAllCityStatusManager; 
}
#endif

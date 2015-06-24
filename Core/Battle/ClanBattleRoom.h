#pragma once
#ifndef __CLANBATTLE_H__
#define __CLANBATTLE_H__
#include"Config.h"
#include"GObject/Player.h"

namespace GObject
{
    class Player;
    class Clan;
}

namespace GData
{
    class ClanBattleBase;
}

namespace Battle
{
    class ClanBattleRoom
    {
        public:
            ClanBattleRoom(UInt32 id,UInt8 bId) : roomId(id) , battleId(bId) , stage(0) {}
            ~ClanBattleRoom() { roomId = 0 ; battleId = 0; force2num.clear(); force2clans.clear();}
            void InsertClan(UInt8 forceId,UInt32 clanId,UInt32 num );
            UInt32 GetRoomId() const { return roomId; }
            UInt8  GetBattleId() const { return battleId; }
            void InsertFighterNum(UInt8 forceId,UInt8 num)
            { 
                force2num.insert(pair<UInt8,UInt32>(forceId,num));
            }
            UInt32 GetNum(UInt8 forceId) { return force2num[forceId];}
            void InsertClans(UInt8 forceId,std::vector<UInt32>clans) 
            { 
                force2clans.insert(pair<UInt8,std::vector<UInt32>>(forceId,clans));
            }
            std::vector<UInt32> GetAllyClans(UInt8 forceId) { return force2clans[forceId];}
            void SetBuildTime(UInt32 time) { buildTime = time;}
            UInt32 GetBuildTime() const { return buildTime;}
            UInt8 GetStage() const { return stage;}
            void  SetStage(UInt8 s) { stage = s;}
            void  SetStage(UInt32 t);
        private:
            UInt32 roomId;    //以创建者的军团id作为roomId
            UInt8  battleId;  //战役Id
            UInt32 buildTime; //房间创建的时间
            UInt8  stage;     //军团战阶段  0战术配置  1战术推演  2 结束 
            std::map<UInt8,UInt32> force2num;   //势力对应的人数
            std::map<UInt8,std::vector<UInt32>> force2clans; //势力对应的公会
    };


    class ClanBattleRoomManager
    {
        public:
            ~ClanBattleRoomManager() { _roomList.clear();}
            void loadBattleRoom(UInt32 roomId,UInt8 battleId,UInt8 forceId,std::vector<UInt32> vecClan,UInt8 fighterNum,UInt32 buildTime);
            bool CreateRoom(GObject::Player* player);  //创建军团战房间 (军团长)
            bool EnterRoom(GObject::Player* player);   //进入军团战房间 (军团长)
            ClanBattleRoom* GetBattleRoom(UInt32 roomId);
            std::vector<ClanBattleRoom*> GetRoomList() const { return _roomList;}
        private:
            std::vector<ClanBattleRoom*> _roomList;  
    };

    extern ClanBattleRoomManager clanBattleRoomManager;
}
#endif

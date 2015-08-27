#pragma once
#ifndef __CLANBATTLE_H__
#define __CLANBATTLE_H__
#include"Config.h"
#include"GObject/Player.h"
#include"GObject/Clan.h"
#include "GObject/GVar.h"

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
            ClanBattleRoom(UInt32 id,UInt8 bId) : roomId(id) , battleId(bId){}
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
                force2clans[forceId] = clans;
                //force2clans.insert(pair<UInt8,std::vector<UInt32>>(forceId,clans));
            }
            std::vector<UInt32> GetAllyClans(UInt8 forceId) { return force2clans[forceId];}
            void SetBuildTime(UInt32 time) { buildTime = time;}
            UInt32 GetBuildTime() const
            { 
                return buildTime;
            }
            std::vector<GObject::Player*> GetAllJoinPlayer();
            std::vector<GObject::Player*> GetSameForceAllies(UInt8 forceId);
            bool IsFirstDay();
            std::vector<UInt32> GetJoinClan();
            std::vector<UInt8> GetJoinForce();
            std::vector<GObject::Player*> GetJoinAllies(UInt8 forceId);
        private:
            UInt32 roomId;    //以创建者的军团id作为roomId
            UInt8  battleId;  //战役Id
            UInt32 buildTime; //房间创建的时间
            std::map<UInt8,UInt32> force2num;   //势力对应的人数
            std::map<UInt8,std::vector<UInt32>> force2clans; //势力对应的公会
    };


    class ClanBattleRoomManager
    {
        public:
            ~ClanBattleRoomManager() { _roomList.clear();}
            void loadBattleRoom(UInt32 roomId,UInt8 forceId,UInt8 battleId,std::vector<UInt32> vecClan,UInt8 fighterNum,UInt32 buildTime);
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

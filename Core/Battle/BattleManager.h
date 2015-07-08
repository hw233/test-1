
#ifndef BATTLEMANAGER_H_
#define BATTLEMANAGER_H_

#include "Common/Mutex.h"
#include "BattleGround.h"
#include <map>
namespace GObject
{
    class Player;
}


#define BATTLEGROUND_MAN 32

#define DAY_MAX 7

namespace Battle
{

    class SingleBattle   //一个城市的战斗
    {
        public:
            ~SingleBattle() { delete ground;}
            void SetNextStartTime(UInt32 t) { nextStartTime = t;}
            UInt32 GetNextStartTime() const { return nextStartTime;}
            void StartBattle();
            void StartOneRound();
            SingleBattle(UInt32 battleId,UInt8 mapId,UInt8 limit);
            bool IsStop() const { return ground->CheckIsStop();}
            UInt16 GetOneRoundTimeCost() const { return ground->GetOneRoundTimeCost();}
            void EnterBattleGround(GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y);
            UInt8 GetCaptureForce() { return ground->GetCaptureId();}
        private:
            UInt32 nextStartTime;  //下一次开始战术的时间
            BattleGround* ground;
    };


    class RoomBattle  //一个房间 管理一个战役的许多个城市
    {
        public:
            RoomBattle(UInt32 id) : roomId(id) { singleBattles.clear(); }
            void InsertSingleBattle(SingleBattle* singBt); 
            void StartAllGround();
            std::vector<SingleBattle*> GetSingleBattles() { return singleBattles;}
            void StartAllGroundWithOneRound();
            UInt8 GetStage();
            UInt32 GetRoomId() const { return roomId;}
            void CollectCaptureInfo(std::map<UInt8,UInt8>& force2captureCityNum);  //战役结束时  统计各个势力的占领城市的情况
        private:
            UInt32 roomId;
            std::vector<SingleBattle*> singleBattles;
    };


    class BattleManager
    {
        public:
            BattleManager() { roomBattleList.clear();}
            void InsertRoomBattle(RoomBattle* roomBattle);
            void StartAll();
            void StartAllWithOneRound();
            std::vector<RoomBattle*> GetRoomBattleList() { return roomBattleList;}
            RoomBattle* GetRoomBattle(UInt32 roomId);
            void removeRoomBattle(UInt32 roomId);
        private:
            std::vector<RoomBattle*> roomBattleList;
    };

    extern BattleManager battleManager;
}
#endif // BATTLEMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


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
        private:
            UInt32 nextStartTime;  //下一次开始战术的时间
            BattleGround* ground;
    };


    class RoomBattle  //一个房间 管理一个战役的许多个城市
    {
        public:
            RoomBattle(UInt32 id) : roomId(id), isPutFighter(false) { singleBattles.clear(); }
            void InsertSingleBattle(SingleBattle* singBt); 
            void StartAllGround();
            std::vector<SingleBattle*> GetSingleBattles() { return singleBattles;}
            void StartAllGroundWithOneRound();
            UInt8 GetStage();
            UInt32 GetRoomId() const { return roomId;}
            UInt8 GetBattleResult();  //战役结果
            void BattleSettlement();  //战役结算
            bool GetIsPutFighter() const { return isPutFighter;}
            void SetIsPutFighter(bool isPut) { isPutFighter = isPut;}
        private:
            UInt32 roomId;
            bool isPutFighter;
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
        private:
            std::vector<RoomBattle*> roomBattleList;
    };

    extern BattleManager battleManager;

    /*
    class BattleManager
    {
        public:
            //获得战场空闲编号
            UInt32 GetBattleGroundIndex(); 
            //进入战场
            //void EnterBattleGround(UInt32,GObject::Player*,UInt8,UInt8 ); 

            //创建战场
            UInt32 CreateBattleGround(UInt32 backGround = 1,UInt8 mapId=1,UInt8 limit = 4);

            //战斗开始
            void StartGround(UInt32 index);

            //所有的城市都开始战斗
            void StartAllGround();
              
            void EnterBattleGround(UInt32 battleId,GObject::Player* player,UInt16 fighterId, UInt8 x,UInt8 y);
        private:
            FastMutex _mutex;
            FastMutex _mutex2;
            std::map<UInt32,BattleGround*>  _map;
    };
    */
}
#endif // BATTLEMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

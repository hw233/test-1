
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
namespace Battle
{
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
}
#endif // BATTLEMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

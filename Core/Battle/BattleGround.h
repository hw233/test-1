
#ifndef BATTLEGROUND_H_
#define BATTLEGROUND_H_
#include "Config.h"
#include <map>
#include <vector>

#define GROUND_LENGTH 20

namespace Battle
{
    class BattleObject;
    class Player;
    class BattleGround
    {
        struct BattleInfo
        {
            UInt8 _x;
            UInt8 _y;
            UInt8 _attackSide;
            UInt16 _attackFighter;
            UInt8 _defendSide;
            UInt16 _defendFighter;
            UInt64 _brtId;
            UInt8 _res;
            BattleInfo(UInt8 x,UInt8 y, UInt8 attackSide ,UInt16 attackFighter ,UInt8 defendSide ,UInt16 defendFighter,UInt64 brtId, UInt8 res):_x(x),_y(y),_attackSide(attackSide),_attackFighter(attackFighter),_defendSide(defendSide),_defendFighter(defendFighter),_brtId(brtId),_res(res){}
            BattleInfo(){}
        };
        public:
            BattleGround()
            {
                map_player.clear();
            }
            void InitMapFight(UInt8 mapId);
            UInt8 PushPlayer(Player *);
            void PushBattleInfo(const BattleInfo& bi);
            void Process();
        private:
            std::map<UInt8 ,Player *>  map_player;
            BattleObject * _mapFight[GROUND_LENGTH][GROUND_LENGTH];
            std::vector<BattleInfo> battleIds;

    };
}
#endif // BATTLEGROUND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


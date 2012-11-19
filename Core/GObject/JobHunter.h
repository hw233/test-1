#ifndef _JOBHUNTER_H_
#define _JOBHUNTER_H_

//************************************************
// 用于第四职业招募的——寻墨
//************************************************

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/URandom.h"
#include "GData/NpcGroup.h"

namespace GObject
{

class Player;

class JobHunter
{
    enum GridType
    {
        GRID_UNKNOWN = 1,   // 未知格子
        GRID_MONSTER,       // 怪物格子
        GRID_BOSS,          // boss格子
        GRID_NORMAL,        // 普通已探索格子
        GRID_TREASURE,      // 宝箱格子
        GRID_TRAP,          // 陷阱格子
    };

    enum Progress
    {
        PROGRESS_NONE = 0,  // 没有开始寻墨
        PROGRESS_START,
        PROGRESS_END,
    };

    public:
        JobHunter(Player* player);
        ~JobHunter();

        void LoadFighterList(std::string& str);
        void AddToFighterList(UInt16 id);
        void SendFighterList();

        void SendGameInfo();

    private:
        bool list2string(std::string& str);

    private:
        Player *_owner;
        std::set<UInt16> _fighterList;
        UInt8 _gameProgress;             // 寻墨游戏状态
        UInt8 _equipProb;                // 装备寻得率
        UInt8 _cittaProb;                // 心法寻得率
        UInt8 _trumpProb;                // 法宝寻得率
        UInt8 _posX;                     // 玩家在寻墨游戏中的X坐标
        UInt8 _posY;                     // 玩家在寻墨游戏中的Y坐标
};
}

#endif


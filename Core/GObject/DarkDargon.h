#ifndef DARKDARGON_H_
#define DARKDARGON_H_

#include <vector>
#include <map>
#include <set>
#include "Config.h"
#include "GData/NpcGroup.h"
#include "GObject/WBossMgr.h"
#include "GObject/Fighter.h"
#include "Common/Stream.h"
#include "Common/Mutex.h"
#include "Battle/BattleSimulator.h"

namespace GObject
{
    
    /*
     玩家状态--dargonStatus
        0:未在活动中
        1:驻防四轮之塔
        2:活动自由状态
     */

    class Player;

    struct RoundTower
    {
        UInt8 idx;//四轮之塔编号 1~4
        vector<Player*> topPlayer;
        vector<Player*> midPlayer1;
        vector<Player*> midPlayer2;
        vector<Player*> bottomPlayer1;
        vector<Player*> bottomPlayer2;
        vector<Player*> bottomPlayer3;
        UInt8 durability;//耐久度
        bool status;//状态
        vector<UInt8> monsterNum;//怪物数 6组
        RoundTower(UInt8 i) : durability(0),status(false) 
        {
            idx = i; 
            monsterNum(6,10);
        }
        void resetMonster() 
        {
            vector<UInt8> tmpvector(6,10);
            monsterNum.swap(tmpvector);
        }
    };

    struct StarMap
    {
        UInt8 idx;//三星阵编号 1~3
        UInt8 durability;//耐久度
        UInt8 arriveCD;//来临CD
        StarMap(UInt8 i) : durability(100),arriveCD(100) { idx = i; }     
        void resetCD() { arriveCD = 100; }
    };

    class DarkDargon : public Singleton <DarkDargon>
    {
    public:
        DarkDargon();
        ~DarkDargon();
        


    private:
        RoundTower[4] roundTowers;
        StarMap[3] starMaps;
        UInt8 globalBuffer;//7个全服BUFFER 0~3bit 四轮之塔 4~6bit 三星阵 初始值 - 0x0f 

    }






}
























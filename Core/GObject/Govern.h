#pragma once
#ifndef _GOVERN_H_
#define _GOVERN_H_
#include"Player.h"
#include"Fighter.h"
#include"Monster.h"
#include"Common/URandom.h"
//#include "LoadWorker.h"

#define SPEEDUP_MAXTIME 3600  //一次加速的加速时间
#define TIME_TAB 120 //刷一次怪的间隔时间
#define SPEEDUP_FREE_CNT 10  //每日免费加速的次数(vip的话 在此数值上加)

namespace GObject  //治理
{
    /*
    struct ItemInfo
    {
        ItemInfo(UInt32 i,UInt32 n):id(i),num(n){}
        UInt32 id;
        UInt32 num;
    };
    */
    class Player;
    class Fighter;
    class Monster;

    struct GovernInfo
    {
        GovernInfo(UInt32 id,bool win) : monsterId(id) ,isWin(win) {}
        UInt32 monsterId;
        bool isWin;
    };

    class GovernManager
    {
        public:
            GovernManager(Player *pl);
            void ReplaceFighter(Fighter * ft);
            void SpeedUp();
            void SendGovernInfo();
            void GetSpeedUpGains(UInt32 times);    //多次的收益
            void GetOneSpeedUpGain();  //一次的收益
            bool FightWithMonster(UInt32 monsterId);
            UInt32 RandomOneMonster();
            void SendGovernAward(UInt8 number);
            void GetGovernInfo(Stream &st, UInt8 begin);
            void OfflineGainsInfo(Stream &st);
            void SendGovernResult(UInt8 type);
            inline void Clear() { _vecGovernInfo.clear();}
            void loadGovernOfflineGain(UInt32 itemId, UInt32 itemNum);
            void GiveGovernOfflineGain();
            void SendOfflineGovernAward();
            void SendOnlineGovernAward(UInt8 number);
        private:
            Player* m_owner;
            Fighter * m_fighter;
            Monster *curMonster;
            std::map<UInt32,UInt32> _speedUpId2Num; //加速获得物品
            std::map<UInt32,UInt32> _offlineId2Num; //离线获得物品
            std::vector<GovernInfo> _vecGovernInfo;
    };
}
#endif

#pragma once
#ifndef _GOVERN_H_
#define _GOVERN_H_
#include"Player.h"
#include"Fighter.h"
#include"Monster.h"
#include"Common/URandom.h"
#include "LoadWorker.h"
#include "Script/lua_tinker.h"

#define SPEEDUP_MAXTIME 3600  //一次加速的加速时间
#define TIME_TAB 120 //刷一次怪的间隔时间
#define SPEEDUP_FREE_CNT 10  //每日免费加速的次数(vip的话 在此数值上加)

namespace GObject  //治理
{

    struct ItemInfo
    {
        ItemInfo():id(0),num(0){}
        ItemInfo(UInt32 i,UInt32 n):id(i),num(n){}
        UInt32 id;
        UInt32 num;
    };

    class Player;
    class Fighter;
    class Monster;
    class GameActionLua;

    struct GovernInfo
    {
        GovernInfo(UInt8 g,UInt32 i,UInt8 r) : groupId(g) , monsterId(i) ,res(r) {}
        UInt8  groupId;
        UInt32 monsterId;
        UInt8  res;
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
            UInt8 FightWithMonster(Monster* mon);
            Monster* RandomOneMonster(UInt8 groupId);
            void SendGovernAward(UInt8 number);
            void GetGovernInfo(Stream &st, UInt8 begin);
            void OfflineGainsInfo(Stream &st);
            void SendGovernResult(UInt8 type);
            inline void Clear() { _vecGovernInfo.clear();}
            void loadGovernOfflineGain(UInt32 itemId, UInt32 itemNum);
            void GiveGovernOfflineGain();
            void SendOfflineGovernAward();
            void SendOnlineGovernAward(UInt8 number);
            void GetItemsByResult(UInt8 res,UInt8 groupId,UInt8 monsterId,std::vector<ItemInfo>&itemInfo);
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

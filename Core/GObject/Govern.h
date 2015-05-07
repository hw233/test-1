#pragma once
#ifndef _GOVERN_H_
#define _GOVERN_H_
#include"Player.h"
#include"Fighter.h"
#include"Monster.h"
#include"Common/URandom.h"
#include "LoadWorker.h"
#include "Script/lua_tinker.h"

#define SPEEDUP_MAXTIME 10*60 //一次加速的加速时间
#define TIME_TAB 15           //刷一次怪的间隔时间
#define TIME_ONCE 10          //隔多少分钟发送给客户端
#define SPEEDUP_FREE_CNT 10   //每日免费加速的次数(vip的话 在此数值上加)

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
        GovernInfo(UInt8 g,UInt32 i,UInt8 r, bool l) : groupId(g) , monsterId(i) ,res(r),isGet(l) {}
        UInt8  groupId;
        UInt32 monsterId;
        UInt8  res;
        bool   isGet;  //是不是可以领取物品
    };

    typedef std::map<UInt32,UInt32> mapId2Num;

    class GovernManager
    {
        public:
            GovernManager(Player *pl);
            void ReplaceFighter(Fighter * ft);
            void SpeedUp();
            void SendBaseInfo();
            UInt8 FightWithMonster(Monster* mon);
            Monster* RandomOneMonster(UInt8 groupId);
            void SendGovernAward(UInt8 number);
            void GetGovernInfo(Stream &st, UInt8 begin);
            void SendGovernResult(UInt8 type);
            void loadGovernOfflineGain(UInt32 itemId, UInt32 itemNum);
            void GiveGovernOfflineGain();
            void SendOfflineGainsInfo(mapId2Num& offId2Num);
            void SendOnlineGovernAward(UInt8 number);
            void GetItemsByResult(UInt8 res,UInt8 groupId,UInt8 monsterId,bool isGet,std::vector<ItemInfo>&itemInfo);
            void GetAccumulativeAward(UInt8 res ,Monster* mon,UInt16 prob,UInt8 times,std::vector<ItemInfo>&vecItem);
            void GetTotalAward(UInt32 times,mapId2Num& mapId2Num);
        private:
            Player* m_owner;
            Fighter * m_fighter;
            Monster *curMonster;
            std::vector<GovernInfo> _vecGovernInfo;
    };
}
#endif


#ifndef WORLD_H_
#define WORLD_H_

#include "MsgHandler/WorldMsgHandler.h"
#include "Server/WorkerThread.h"
#include "Common/Stream.h"
#include "Server/Cfg.h"
#include "Common/TimeUtil.h"
#include "Common/MCached.h"
#include "GObject/GVar.h"
#ifndef _WIN32
#include "kingnet_analyzer.h"
#endif
#include "World.h"
#include "ChatHold.h"
namespace Script
{
    class WorldScript;
    class BattleFormula;
}
namespace GObject
{
    struct RCSort
    {
        GObject::Player* player;
        UInt32 total;
        UInt32 time;
        RCSort():player(NULL),total(0),time(0){}
    };
    struct lt_rcsort
    {
        bool operator()(const RCSort& a, const RCSort& b) const { return a.total > b.total || (a.total==b.total && a.time < b.time); }
    };
    typedef std::multiset<RCSort, lt_rcsort> RCSortType;

    typedef std::map<UInt16,GObject::Player*> MapIndexPlayer;

    //class ChatHold;
    class World
        :public WorkerRunner<WorldMsgHandler>
    {
        public:
            inline UInt8 TID() const { return WORKER_THREAD_WORLD; }
            bool Init();
            void UnInit();
            std::string GetLogName();
            inline Script::BattleFormula* getBattleFormula() {return _battleFormula;}
            ChatHold* GetChatHold(){return chatHold;}

        private:
            static void InitRank();
            static void World_Midnight_Check( World * );
            static void World_Test( World * );
            static void World_Govern_SendInfo(World *); //每一小时发一组怪物id以及胜负 给客户端 
            static void World_Govern_SendAward(World *);//每2min客户端自己给玩家发送一次奖励
            static void world_clanBattle_putFighters(World* );//军团战放入战将
            static void World_clanBattle_OneRound(World*);  //军团战战术一回合
            static void World_clanBattle_stageCheck(World*); //军团战阶段检测
            static void World_Send_Arena_Award(World*); //发送竞技场奖励
        public:
            //static RCSortType arenaSort;
            static MapIndexPlayer arenaSort;

        private:
            Script::BattleFormula * _battleFormula;
            Script::WorldScript * _worldScript;
            ChatHold* chatHold;
    };
}
#endif // WORLD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


#pragma once
#ifndef __CAMPAIGNMANAGER_H__
#define __CAMPAIGNMANAGER_H__
#include "BattleManager.h"
namespace GObject
{
    class Player;
}

namespace Battle
{
    //比battleManager高一个层级 battleManager是单个地图的 campaignManager是整个战役所有地图
    class CampaignManager
    {
        public:
            void InsertBattleManager(UInt32 roomId,UInt8 mapId,UInt8 limit);
            BattleManager* GetBattleManager(UInt32 roomId);
            void EnterBattleGround(UInt32 roomId,UInt8 mapId,GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y );
            void BattleStart();
        private:
            std::map<UInt32,BattleManager*>  _campaign; 
    };

    extern CampaignManager campaignManager;
}
#endif

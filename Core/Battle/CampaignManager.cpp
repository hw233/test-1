#include"CampaignManager.h"

namespace Battle
{
    CampaignManager campaignManager;

    void CampaignManager::InsertBattleManager(UInt32 roomId,UInt8 mapId,UInt8 limit)
    {
        BattleManager* battleManager = _campaign[roomId];
        if( battleManager == NULL )
        {
           battleManager = new BattleManager();
        }
        battleManager->CreateBattleGround(roomId,mapId);
        _campaign[roomId] = battleManager;
    }

    //获得某一房间
    BattleManager* CampaignManager::GetBattleManager(UInt32 roomId)
    {
        return _campaign[roomId];
    }

    //所有战场开打
    void CampaignManager::BattleStart()
    {
        for(auto it = _campaign.begin(); it != _campaign.end(); ++it )
        {
            (it->second)->StartAllGround();
        }
    }

    void CampaignManager::EnterBattleGround(UInt32 roomId,UInt8 mapId,GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y )
    {
        //进入战场
        BattleManager* battleManager = _campaign[roomId];
        if( battleManager == NULL )
        {
            return;
        }
        else
        {
            UInt32 battleId = roomId+mapId;
            battleManager->EnterBattleGround(battleId,player,fighterId,x,y);
        }
    }

}

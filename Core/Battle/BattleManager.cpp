#include "BattleManager.h"
#include "Battle/ClanBattleRoom.h"
#include "Battle/ClanBattleCityStatus.h"

namespace Battle
{

    BattleManager battleManager;
    

    void SingleBattle::StartBattle()
    {
        ground->start();
    }

    void SingleBattle::StartOneRound()
    {
        ground->FightOneRound();
    }

    SingleBattle::SingleBattle(UInt32 battleId,UInt8 mapId,UInt8 limit = 4): nextStartTime(0)
    {
        //FastMutex::ScopedLock lk(_mutex);
        battleId = battleId + mapId;
        ground = new BattleGround(battleId,mapId);
    }

    void SingleBattle::EnterBattleGround(GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y)
    {
        ground->PushFighter(player,fighterId,x,y);
    }

    void SingleBattle::NpcEnterBattleGround(UInt8 forceId,UInt16 fighterId,UInt8 x,UInt8 y)
    {
        ground->PushNpc(forceId,fighterId,x,y);
    }
    
    void RoomBattle::InsertSingleBattle(SingleBattle* singBt)
    {
        singleBattles.push_back(singBt);
    }

    void RoomBattle::StartAllGround()
    {
        if( singleBattles.empty() )
            return;
        for(auto it = singleBattles.begin(); it != singleBattles.end(); ++it )
        {
            (*it)->StartBattle();
            delete (*it);
            (*it) = NULL;
        }
    }


    //一回合
    void RoomBattle::StartAllGroundWithOneRound()
    {
        if( singleBattles.empty() )
            return;
        for(auto it = singleBattles.begin(); it != singleBattles.end(); ++it )
        {
            (*it)->StartOneRound();
        }
    }



    UInt8 RoomBattle::GetStage()
    {
        Battle::RoomAllCityStatus* status = Battle::roomAllCityStatusManager.GetRoomAllCityStatus(roomId);
        if( status == NULL )
            return -1;
        return (status->GetStage());
    }

    void BattleManager::InsertRoomBattle(RoomBattle* roomBattle)
    {
        roomBattleList.push_back(roomBattle);
    }

    void BattleManager::StartAll()
    {
        if( roomBattleList.empty() )
            return;
        for(auto it = roomBattleList.begin(); it != roomBattleList.end(); ++it )
        {
            (*it)->StartAllGround();
            delete (*it);
            (*it) = NULL;
        }
    }

    RoomBattle* BattleManager::GetRoomBattle(UInt32 roomId)
    {
        for( auto it = roomBattleList.begin(); it != roomBattleList.end(); ++it )
        {
            if( (*it)->GetRoomId() == roomId )
            {
                return (*it);
            }
        }
        return NULL;
    }

    void BattleManager::removeRoomBattle(UInt32 roomId)
    {
        if( roomBattleList.empty())
            return;
        for( auto it = roomBattleList.begin(); it != roomBattleList.end();)
        {
            if( (*it)->GetRoomId() == roomId )
            {
                delete (*it);
                it = roomBattleList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void RoomBattle::CollectCaptureInfo(std::map<UInt8,UInt8>& force2captureCityNum)
    {
        if( GetStage() !=  2 )
            return;
        for(auto it = singleBattles.begin(); it != singleBattles.end(); ++it )
        {
            UInt8 captureId = (*it)->GetCaptureForce();
            if( captureId != 0 )
            {
                if( !force2captureCityNum[captureId] )
                {
                    force2captureCityNum[captureId] = 1;
                }
                else
                {
                    force2captureCityNum[captureId] += 1;
                }
            }
        }
         
    }


    void RoomBattle::GivePlayerKillRankAward()
    {

        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        //按照杀敌人数进行排序
        MoreKillSoldiers moreKillSoldiers;
        std::sort(vecPlayer.begin(), vecPlayer.end(),moreKillSoldiers);
        /*
        static UInt32 Award[][2][2] ={
            {{ 20001,0},{20002,20}},
            {{ 20001,0},{20002,15}},
            {{ 20001,0},{20002,10}},
            {{ 20001,0},{20002,5}},
            {{ 20001,1000},{ 20002,0}},
        };
        */
        std::string items[5] = {
            "20002,20",
            "20002,15",
            "20002,10",
            "20002,5",
            "20001,1000"
        };

        UInt8 pos = 1;
        UInt8 index = 1;
        for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
        {
            if( (*it)->GetKillSoldiersNum() == 0 )
                break;
            if( pos < 9999 )
            {
                if( pos >= 1 && pos <= 3 )
                    index = pos-1;
                else if( pos >= 4 && pos <= 10 )
                    index = 3;
                else 
                    index = 4;
                //发排行奖励
                GObject::Mail* mail = new GObject::Mail(IDGenerator::gMailOidGenerator.ID(),(*it),1,items[index],0,static_cast<UInt32>(-1));
                if(mail)
                { 
                    GObject::globalMails.add(mail->GetId(), mail);
                    (*it)->AddMail(mail->GetId());
                }
            }
            ++pos;
        }
    }


    void RoomBattle::GivePlayerKillFighterRankAward()
    {
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        MoreKillFighters moreKillFighter;
        std::sort(vecPlayer.begin(),vecPlayer.end(),moreKillFighter);
        UInt32 pos=1;
        UInt32 index=0;
        /*
        static UInt32 Award[][2][2] ={
            {{ 20001,0},{20002,20}},
            {{ 20001,0},{20002,15}},
            {{ 20001,0},{20002,10}},
            {{ 20001,0},{20002,5}},
            {{ 20001,1000},{ 20002,0}},
        };
        */

        std::string items[5] = {
            "20002,20",
            "20002,15",
            "20002,10",
            "20002,5",
            "20001,1000"
        };

        for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
        {
            if( (*it)->GetKillFighterNum() == 0 )
                break;
            if( pos < 9999 )
            {
                if( pos >= 1 && pos <= 3 )
                    index = pos-1;
                else if( pos >= 4 && pos <= 10 )
                    index = 3;
                else 
                    index = 4;
                //发排行奖励
                //走邮件
                GObject::Mail* mail = new GObject::Mail(IDGenerator::gMailOidGenerator.ID(),(*it),1,items[index],0,static_cast<UInt32>(-1));
                if(mail)
                { 
                    GObject::globalMails.add(mail->GetId(), mail);
                    (*it)->AddMail(mail->GetId());
                }
            }
            ++pos;
        }
    }

    void RoomBattle::GivePlayerEndConstantlyKillAward()
    {
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        for( auto it =  vecPlayer.begin(); it != vecPlayer.end(); ++it )
        {
            (*it)->GiveEndConstantlyKillAward();
        }

    }

    void RoomBattle::GivePlayerConstantlyKillAward()
    {
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL)
            return;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
        {
            (*it)->GiveConstantlyKillAward();
        }
    }


    void RoomBattle::GiveCaptureCityAward()
    {
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL)
            return;
        std::map<UInt8,UInt8> force2captureCityNum;
        CollectCaptureInfo(force2captureCityNum);
        for( auto it = force2captureCityNum.begin(); it != force2captureCityNum.end(); ++it )
        {
            UInt8 forceId = it->first;
            UInt8 captureCityNum = it->second;
            std::vector<GObject::Player*> vecPlayer = room->GetSameForceAllies(forceId);
            std::cout<<" 势力  "<<static_cast<UInt32>(forceId)<<"占领了   "<<static_cast<UInt32>(captureCityNum)<<"座城市 "<<std::endl;
            for( auto iter = vecPlayer.begin(); iter != vecPlayer.end(); ++iter )
            {
                std::cout<<"给 "<<(*iter)->GetName()<<"发奖励"<<std::endl;
                GObject::Mail* mail = new GObject::Mail(IDGenerator::gMailOidGenerator.ID(),(*iter),1,"10002,100",0,static_cast<UInt32>(-1));
                if(mail)
                { 
                    GObject::globalMails.add(mail->GetId(), mail);
                    (*iter)->AddMail(mail->GetId());
                }
                std::cout<<"奖励是  : 10002 2 个呵呵呵呵" <<std::endl;
            }

        }
    }

    void RoomBattle::GetWinForce(std::vector<UInt8> &vecWinForce, std::vector<UInt8> &vecFailForce)
    {
        std::map<UInt8,UInt8> force2captureCityNum;
        CollectCaptureInfo(force2captureCityNum);
        UInt8 max = 0;

        //先找到最大的占城个数
        for( auto it = force2captureCityNum.begin(); it != force2captureCityNum.end(); ++it )
        {
            std::cout<<static_cast<UInt32>(it->first)<<endl;
            if( it->second > max )
            {
                max = it->second;
            }
        }

        for( auto it = force2captureCityNum.begin(); it != force2captureCityNum.end(); ++it )
        {
            if( it->second == max )
            {
                vecWinForce.push_back(it->first);
            }
            else
            {
                vecFailForce.push_back(it->first);
            }
        }


    }


    void RoomBattle::GiveClanBattleAward()
    {
        //发公会战结果奖励
        std::vector<UInt8> vecWinForce;
        std::vector<UInt8> vecFailForce;

        GetWinForce(vecWinForce,vecFailForce);
        //胜者发奖励
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL)
            return;
        std::map<UInt8,UInt8> force2captureCityNum;
        for( auto it = vecWinForce.begin(); it != vecWinForce.end(); ++it )
        {
            UInt32 forceId = (*it);
            std::vector<UInt32> vecClans = room->GetAllyClans(forceId);
            if( vecClans.empty() )
                continue;
            for( auto iter = vecClans.begin(); iter != vecClans.end(); ++iter)
            {
                UInt32 clanId = (*iter);
                GObject::Clan* clan = GObject::globalClan[clanId];
                if( clan == NULL )
                {
                    continue;
                }
                clan->SetClanFame(clan->GetClanFame()+100);
                DB1().PushUpdateData(" update `clan` set 'clanFame' = %u where `clanId` = %u",clan->GetClanFame(),clanId);
            }

        }


    }

    void RoomBattle::Settlement()
    {
        GivePlayerEndConstantlyKillAward();
        GivePlayerConstantlyKillAward();
        GivePlayerKillRankAward();
        GivePlayerKillFighterRankAward();
        GiveCaptureCityAward();
        //GiveClanBattleAward();
    }


}

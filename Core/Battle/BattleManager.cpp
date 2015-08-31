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
        //battleId = battleId + mapId;
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
    
    /*
    UInt8 RoomBattle::GetCaptureCitys()
    {
        std::map<UInt8,UInt8> force2city;
        CollectCaptureInfo(force2city);
        UInt8 totalNum = 0;
        for( auto it = force2city.begin(); it != force2city.end(); ++it )
        {
            totalNum += (it->second);
        }
        return totalNum;
    }
    */


    void RoomBattle::GivePlayerKillRankAward()
    {

        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        //按照杀敌人数进行排序
        MoreKillSoldiers moreKillSoldiers;
        std::sort(vecPlayer.begin(), vecPlayer.end(),moreKillSoldiers);
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

    
    void RoomBattle::GetClanBattleRankInfo(UInt8 type,Stream& st)
    {
        if( GetStage() != 2 )
            return;
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        switch(type)
        {
            case 0:
                {
                    MoreKillFighters moreKillFighter;
                    std::sort(vecPlayer.begin(),vecPlayer.end(),moreKillFighter);
                }
                break;
            case 1:
                {
                    MoreKillSoldiers moreKillSoldiers;
                    std::sort(vecPlayer.begin(), vecPlayer.end(),moreKillSoldiers);
                }
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                {
                    MoreConstantlyKill moreConstantlyKill;
                    std::sort(vecPlayer.begin(), vecPlayer.end(),moreConstantlyKill);
                }
                break;
            case 5:
                {
                    MoreEndConstantlyKill moreEndConstantlyKill;
                    std::sort(vecPlayer.begin(), vecPlayer.end(),moreEndConstantlyKill);
                }
                break;
        }
        UInt8 pos = 1; 
        UInt8 num = 0;
        size_t offset = st.size();
        st<<static_cast<UInt8>(num);
        for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
        {
            if( pos <= 50 )
            {
                st<<(*it)->GetName();
                ++num;
            }
            else
            {
                break;
            }
            ++pos;
        }
        st.data<UInt8>(offset)=num;
    }


    GObject::Player* RoomBattle::GetMaxTotalKillPlayer()
    {
        if( GetStage() != 2 )
            return NULL;
        GObject::Player* player = NULL;
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return NULL;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        MoreKill moreKill;
        std::sort(vecPlayer.begin(),vecPlayer.end(),moreKill);
        player = vecPlayer.front();
        return player;
    }

    GObject::Player* RoomBattle::GetMaxEndConstantlyKillPlayer()
    {
        if( GetStage() != 2 )
            return NULL;
        GObject::Player* player = NULL;
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return NULL;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        MoreEndConstantlyKill moreEndConstantlyKill;
        std::sort(vecPlayer.begin(),vecPlayer.end(),moreEndConstantlyKill);
        player = vecPlayer.front();
        return player;

    }

    GObject::Player* RoomBattle::GetMaxConstantlyKillPlayer()
    {
        if( GetStage() != 2 )
            return NULL;
        GObject::Player* player = NULL;
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return NULL;
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        MoreConstantlyKill moreConstantlyKill;
        std::sort(vecPlayer.begin(),vecPlayer.end(),moreConstantlyKill);
        player = vecPlayer.front();
        return player;
    }

    void RoomBattle::GetBattleResultInfo(UInt8 forceId,Stream& st)
    {
        if( GetStage() != 2 )
            return ;
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return ;
        Battle::RoomAllCityStatus* status = Battle::roomAllCityStatusManager.GetRoomAllCityStatus(roomId);
        st<<static_cast<UInt8>(status->GetCaptureCityNum());
        //st<<static_cast<UInt8>(GetCaptureCitys());
        std::vector<GObject::Player*> vecPlayer = room->GetAllJoinPlayer();
        st<<static_cast<UInt16>(vecPlayer.size());
        st<<static_cast<UInt16>(GetTotalKillFighterNum(forceId));
        st<<static_cast<UInt16>(GetTotalLostFighterNum(forceId));
        if( GetMaxConstantlyKillPlayer() != NULL )
        {
            st<<GetMaxConstantlyKillPlayer()->GetName();
        }
        else
        {
            st<<" 阳顶天";
        }
        GObject::Player* player = GetMaxTotalKillPlayer();
        if( player == NULL )
        {
            st<<"god is a girl";
        }
        else
        {
            st<<player->GetName();
        }
        st<<" 司马太郎";
        if( GetMaxEndConstantlyKillPlayer() != NULL )
        {
            st<<GetMaxEndConstantlyKillPlayer()->GetName();
        }
        else
        {
            st<<" my godness ";
        }
    }

    UInt32 RoomBattle::GetTotalLostFighterNum(UInt8 forceId)
    {
        if( GetStage() != 2 )
            return 0;
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return 0;
        std::vector<GObject::Player*> vecPlayer = room->GetJoinAllies(forceId);
        UInt32 totalLost = 0;
        for(auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
        {
            totalLost += (*it)->GetBeKilledFighterNum();
        }
        return totalLost;
    }

    UInt32 RoomBattle::GetTotalKillFighterNum(UInt8 forceId)
    {
        if( GetStage() != 2 )
            return 0;
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
            return 0;
        std::vector<GObject::Player*> vecPlayer = room->GetJoinAllies(forceId);
        UInt32 totalKill = 0;
        for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
        {
            totalKill += (*it)->GetKillFighterNum();
        }
        return totalKill;
    }


}

#include"ClanBattleRoom.h"
#include"GData/ClanBattleBase.h"
#include"Battle/ClanBattleCityStatus.h"
#include"GObject/Var.h"

namespace Battle
{

    ClanBattleRoomManager clanBattleRoomManager;

    std::vector<UInt32> ClanBattleRoom::GetJoinClan()
    {
       std::vector<UInt32> vecClans;
       for( auto it = force2clans.begin(); it != force2clans.end(); ++it )
       {
           std::vector<UInt32> clans = it->second;
           for( auto iter = clans.begin(); iter != clans.end(); ++iter )
           {
               vecClans.push_back(*iter);
           }
       }
       return vecClans;
    }

    std::vector<UInt8> ClanBattleRoom::GetJoinForce()
    {
       std::vector<UInt8> vecForce;
       for( auto it = force2clans.begin(); it != force2clans.end(); ++it )
       {
           vecForce.push_back(it->first);
       }
       return vecForce;
    }


    std::vector<UInt32> ClanBattleRoom::GetEnemyClans(UInt8 forceId)
    {
        std::vector<UInt32> vecClan;
        for( auto it = force2clans.begin(); it != force2clans.end(); ++it )
        {
            if( it->first == forceId )
                continue;
            for( auto iter = (it->second).begin(); iter != (it->second).end(); ++iter )
            {
                vecClan.push_back(*iter);
            }
        }
        return vecClan;
    }


    void ClanBattleRoom::InsertClan(UInt8 forceId,UInt32 clanId,UInt32 num )
    {
        //更新数据库
        std::vector<UInt32> vecClan = force2clans[forceId];
        UInt16 totalNum = 0;
        if( !vecClan.empty() )
        {
            for( auto it = vecClan.begin(); it != vecClan.end(); ++it )
            {
                if( *it == clanId )
                {
                    return;
                }
                GObject::Clan* clan = GObject::globalClan[clanId];
                if( clan == NULL )
                    return;
                totalNum += clan->GetMemberNum();
            }
        }
        char buff[1024]={0};
        vecClan.push_back(clanId);
        force2clans[forceId] = vecClan;
        totalNum += num;
        force2num[forceId] = totalNum;
        UInt8 offset = 0 ;
        for(auto it = vecClan.begin(); it != vecClan.end() ; ++it )
        {
            offset += sprintf(buff+offset,"%d,",(*it));
        }
        buff[offset-1] = '\0';
        std::string clans(buff);
        if( vecClan.size() == 1)
        {
            DB7().PushUpdateData("INSERT INTO  `clan_battle_room`(`roomId`,`forceId`,`battleId`,`clans`,`fighterNum`,`buildTime`) VALUES(%u, %u ,%u, '%s',%u, %u)",roomId,forceId,battleId,clans.c_str(),totalNum,buildTime);
        }
        if( vecClan.size() > 1 )
        {
            DB7().PushUpdateData("delete from `clan_battle_room`  where roomId=%u AND forceId=%u",roomId,forceId);

            DB7().PushUpdateData("INSERT INTO  `clan_battle_room`(`roomId`, `forceId`,`battleId`,`clans`,`fighterNum`,`buildTime`) VALUES(%u, %u ,%u,'%s',%u,%u)",roomId,forceId,battleId,clans.c_str(),totalNum,buildTime);
        }
        GObject::Clan* clan = GObject::globalClan[clanId];
        if( clan == NULL )
        {
            return ;
        }
        clan->SetClanBattleRoomId(roomId);
        clan->SetBattleForceId(forceId);
        
        //更新公会表中的roomId 以及势力id
        DB7().PushUpdateData("update clan set battleRoomId = %u , forceId = %u where clanId=%u",roomId,forceId,clanId);

    }


    bool ClanBattleRoom::IsFirstDay()
    {
        time_t time = buildTime;
        tm* tt=localtime(&time);
        UInt8 hour = tt->tm_hour;
        UInt8 min = tt->tm_min;
        UInt32 sday =buildTime+((24-hour)*60-min)*60+36000;   //第二天十点的时间的戳

        //判断该房间是不是不处于报名阶段
        UInt32 now = TimeUtil::Now();
        if( now > sday )
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    
    bool ClanBattleRoomManager::CreateRoom(GObject::Player* player)
    {
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
        {
            return false;
        }
        UInt32 exp = clan->GetClanFame();  //获得公会声望
        GData::ClanBattleBase* base = GData::clanBattleBaseTable.GetClanBattleBase(exp);
        UInt32 clanId = clan->GetId();
        UInt8 memberNum = clan->GetMemberNum();
        UInt8 battleId = base->GetBattleId();

        //检查是不是军团长
        UInt8 pos = player->GetClanPos();  //只有会长和副会长才能开启军团战
        if( pos > 2 )
        {
            return false;
        }
        UInt32 now = TimeUtil::Now();
        ClanBattleRoom* room = new ClanBattleRoom(clanId,battleId);
        room->SetBuildTime(now);
        room->InsertClan(1,clanId,memberNum);
        _roomList.push_back(room);

        Battle::roomAllCityStatusManager.InsertRoomAllCityStatus(clanId,battleId);
        return true;
    }



    bool ClanBattleRoomManager::EnterRoom(GObject::Player* player)
    {
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
        {
            return false;
        }
        if( clan->GetClanBattleRoomId() != 0 )
        {
            return false;
        }
        UInt32 exp = clan->GetClanFame();  //获得公会声望
        GData::ClanBattleBase* base = GData::clanBattleBaseTable.GetClanBattleBase(exp);
        //UInt8 battleId = base->GetBattleId();
        UInt32 clanId = clan->GetId();
        UInt8 memberNum = clan->GetMemberNum();
        //UInt8 forceNum = base->GetForceNum();
        UInt8 playermax = base->GetPlayerMax();
        UInt8 battleId = base->GetBattleId();

        GData::BattleMapInfo* info = GData::battleMapTable.GetBattleMapInfo(battleId);
        std::vector<UInt8> vecForce = info->GetForces();
        
        
        //检查是不是军团长

        UInt8 pos = player->GetClanPos();  //只有会长和副会长才能开启军团战
        if( pos > 2 )
        {
            return false;
        }
        if( _roomList.empty() )
        {
            CreateRoom(player);
        }
        else
        {
            ClanBattleRoom *room = _roomList.back();
            if( room == NULL )
            {
                return false;
            }
            if( !room->IsFirstDay() )
            {
                CreateRoom(player);
            }
            else
            {
                //获得房间中人数最少的势力
                UInt8 i = 1;
                UInt8 theMinNum = 0xFF;  //人数最少的势力
                UInt8 minForce = 1 ;    
                for( auto it = vecForce.begin(); it != vecForce.end(); ++it)
                {
                    UInt8 forceId = (*it);
                    if( room->GetNum(forceId) < theMinNum )
                    {
                        theMinNum = room->GetNum(forceId);
                        minForce = i;
                    }
                }
                if( theMinNum >= playermax )
                {
                    CreateRoom(player);
                }
                else
                {

                    room->InsertClan(minForce,clanId,memberNum);
                }
            }
        }
        return true;
    }

    void ClanBattleRoomManager::loadBattleRoom(UInt32 roomId,UInt8 forceId,UInt8 battleId,std::vector<UInt32> vecClan,UInt8 fighterNum,UInt32 buildTime)
    {
        ClanBattleRoom* room = GetBattleRoom(roomId);
        if( room == NULL )
        {
            room = new ClanBattleRoom(roomId,battleId);
        }
        room->InsertClans(forceId,vecClan);
        room->InsertFighterNum(forceId,fighterNum);
        room->SetBuildTime(buildTime);
        _roomList.push_back(room);

        //每个城的状态
        Battle::roomAllCityStatusManager.InsertRoomAllCityStatus(roomId,battleId);
    }



    ClanBattleRoom* ClanBattleRoomManager::GetBattleRoom(UInt32 roomId)
    {
        for(auto it = _roomList.begin(); it != _roomList.end(); ++it )
        {
            if( (*it)->GetRoomId() == roomId )
            {
                return *it;
            }
        }
        return NULL;
    }

    std::vector<GObject::Player*> ClanBattleRoom::GetAllJoinPlayer()
    {
        std::vector<GObject::Player*> vecPlayer;
        for( auto it = force2clans.begin() ; it != force2clans.end(); ++it )
        {
            std::vector<UInt32> vecClan = it->second;
            for( auto iter = vecClan.begin(); iter != vecClan.end(); ++iter )
            {
                UInt32 clanId = (*iter);
                GObject::Clan* clan = GObject::globalClan[clanId];
                if( clan == NULL )
                    continue;
                std::vector<GObject::Player*> temp = clan->GetJoinClanBattlePlayer();
                for( auto iterator = temp.begin(); iterator != temp.end(); ++iterator)
                {
                    if( (*iterator)->GetVar(GObject::VAR_CLANBATTLE_FIGHTERNUM) > 0 )
                    {
                        vecPlayer.push_back(*iterator);
                    }
                }
            }
        }
        return vecPlayer;
    }

    std::vector<GObject::Player*> ClanBattleRoom::GetSameForceAllies(UInt8 forceId)
    {
        std::vector<GObject::Player*> vecPlayer;
        std::vector<UInt32> vecClan = GetAllyClans(forceId);
        for( auto it = vecClan.begin(); it != vecClan.end(); ++it)
        {
            GObject::Clan* clan = GObject::globalClan[(*it)];
            if( clan == NULL )
                continue;
            std::vector<GObject::Player*> temp = clan->GetJoinClanBattlePlayer();
            if( temp.empty())
                continue;
            for( auto iter = temp.begin(); iter != temp.end(); ++iter )
            {
                vecPlayer.push_back(*iter);
            }

        }
        return vecPlayer;
    }

    std::vector<GObject::Player*> ClanBattleRoom::GetJoinAllies(UInt8 forceId)
    {
        std::vector<GObject::Player*> vecPlayer;
        std::vector<GObject::Player*> vecAllies = GetSameForceAllies(forceId);
        for( auto it = vecAllies.begin(); it != vecAllies.end(); ++it )
        {
            if( (*it)->GetVar(GObject::VAR_CLANBATTLE_FIGHTERNUM) > 0 )
            {
                vecPlayer.push_back(*it);
            }
        }
        return vecPlayer;
    }
}

#include"ClanBattleRoom.h"
#include"GData/ClanBattleBase.h"
#include"Battle/ClanBattleCityStatus.h"

namespace Battle
{

    ClanBattleRoomManager clanBattleRoomManager;


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
        UInt8 forceNum = base->GetForceNum();
        UInt8 playermax = base->GetPlayerMax();
        
        
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
            UInt32 buildTime = room->GetBuildTime();
            time_t time = buildTime;
            tm* tt=localtime(&time);
            UInt8 hour = tt->tm_hour;
            UInt8 min = tt->tm_min;
            UInt32 sday =buildTime+((24-hour)*60-min)*60+36000;   //第二天十点的时间的戳

            //判断该房间是不是不处于报名阶段
            UInt32 now = TimeUtil::Now();
            if( now > sday )
            {
                CreateRoom(player);
            }
            else
            {
                //获得房间中人数最少的势力
                UInt8 i = 1;
                UInt8 theMinNum = 0xFF;  //人数最少的势力
                UInt8 minForce = 1 ;    
                for( i = 1; i <= forceNum; ++i)
                {
                    if( room->GetNum(i) < theMinNum )
                    {
                        theMinNum = room->GetNum(i);
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

                /*
                for(i = 1; i <= forceNum ; ++i)
                {
                    UInt8 num = 0;
                    num = room->GetNum(i);
                    //num = force2num[i];
                    if( num < playermax )
                    {
                        room->InsertClan(i,clanId,memberNum);
                        break;
                    }
                }
                //所有的房间都满了
                if( i > forceNum )
                {
                    CreateRoom(player);
                }
                */
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
                    vecPlayer.push_back(*iterator);
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
}

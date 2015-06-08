#include"ClanBattleRoom.h"
#include"GData/ClanBattleBase.h"
namespace Battle
{

    ClanBattleRoomManager clanBattleRoomManager;

    void ClanBattleRoom::InsertClan(UInt8 forceId,UInt32 clanId,UInt32 num )
    {
        //更新数据库
        std::vector<UInt32> vecClan = force2clans[forceId];

        if( !vecClan.empty() )
        {
            for( auto it = vecClan.begin(); it != vecClan.end(); ++it )
            {
                if( *it == clanId )
                {
                    return;
                }
            }

        }
        char buff[1024]={0};
        vecClan.push_back(clanId);
        force2clans[forceId] = vecClan;
        UInt32 totalNum = force2num[forceId]+num;
        force2num[forceId] += num;
        UInt8 offset = 0 ;
        for(auto it = vecClan.begin(); it != vecClan.end() ; ++it )
        {
            offset += sprintf(buff+offset,"%d,",(*it));
        }
        buff[offset-1] = NULL;
        std::string clans(buff);
        if( vecClan.size() == 1)
        {
            DB7().PushUpdateData("INSERT INTO  `clan_battle_room`(`roomId`, `forceId`,`battleId`,`clans`,`fighterNum`) VALUES(%u, %u ,%u , '%s',%u)",roomId,forceId,battleId,clans.c_str(),totalNum);
        }
        if( vecClan.size() > 1 )
        {
            DB7().PushUpdateData("delete from `clan_battle_room`  where roomId=%u",roomId);

            DB7().PushUpdateData("INSERT INTO  `clan_battle_room`(`roomId`, `forceId`,`battleId`,`clans`,`fighterNum`) VALUES(%u, %u ,%u , '%s',%u)",roomId,forceId,battleId,clans.c_str(),totalNum);
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
        ClanBattleRoom* room = new ClanBattleRoom(clanId,battleId);
        room->InsertClan(1,clanId,memberNum);
        _roomList.push_back(room);
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
            UInt8 i = 1;
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
        }
        return true;
    }

    void ClanBattleRoomManager::loadBattleRoom(UInt32 roomId,UInt8 battleId,UInt8 forceId,std::vector<UInt32> vecClan,UInt8 fighterNum)
    {
        ClanBattleRoom* room = GetBattleRoom(roomId);
        if( room == NULL )
        {
            room = new ClanBattleRoom(roomId,battleId);
        }
        room->InsertClans(forceId,vecClan);
        room->InsertFighterNum(forceId,fighterNum);
        _roomList.push_back(room);
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
}

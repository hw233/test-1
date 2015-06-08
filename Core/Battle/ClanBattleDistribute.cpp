#include"ClanBattleDistribute.h"
#include"ClanBattleRoom.h"
#include"ClanOrder.h"
#include"ClanBattleComment.h"

#include"MsgID.h"

namespace Battle
{
    BattleDistribute battleDistribute;
  
    UInt32 DistributeInfo::count = 0;
    bool BattleDistribute::Check(GObject::Player* player)
    {
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )  //没有参加军团
        {
            return false;
        }
        UInt32 roomId = clan->GetClanBattleRoomId();
        if( roomId == 0 )  //该军团没有报名参加军团战
        {
            return false;
        }
        UInt8 status = player->GetJoinClanBattle();
        if( status == 0 ) //该玩家没有报名参加军团战
        {
            return false;
        }
        return true;
    }



    //有坐标来获得该坐标的排布信息
    DistributeInfo* BattleDistribute::GetDistributeInfo(std::vector<DistributeInfo*> &vecDistributeInfo,UInt8 x,UInt8 y)
    {
        for( auto it = vecDistributeInfo.begin(); it != vecDistributeInfo.end() ; ++it )
        {
            if( (*it)->GetPosX() == x && (*it)->GetPosY() == y )
            {
                return (*it);
            }
        }
        return NULL;
    }

    UInt32 BattleDistribute::GetBattleRoomId(GObject::Player* player)
    {
        if( player == NULL )
        {
            return 0;
        }
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
        {
            return 0; 
        }
        return clan->GetClanBattleRoomId();
    }
    

    //放将
    bool BattleDistribute::PutFighter(UInt8 mapId,GObject::Player* player,UInt16 fighterId, UInt8 x, UInt8 y,bool flag)
    {
        bool status = Check(player);
        if( !status )
        {
            return false;
        }
        UInt32 roomId = GetBattleRoomId(player);
        if( roomId == 0 )
        {
            return false;
        }
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
        {
            return false;
        }
        GData::MapInfo* mapInfo = GData::mapTable.GetMapInfo(mapId);
        UInt8 width = mapInfo->GetWidth();
        UInt8 height = mapInfo->GetHeight();
        vecInfo tileInfo = mapInfo->GetTileInfo();
        vecInfo campInfo = mapInfo->GetCampInfo();

        if( x >= width || y >= height )    //坐标超出了地图的边界
        {
            return false;
        }

        if( tileInfo[x+y*width] == 0 )     //改点是不可放置的点
        {
            return false;
        }

        GObject::Clan* clan = player->GetClan();
        UInt8 side = clan->GetBattleForceId();

        if( campInfo[x+y*width] != side )  //放到敌人的阵营了
        {
            return false;
        }

        //下面判断该点上是不是有人
        std::vector<MapDistributeInfo*> vecMapDistributeInfo = _room2Distribute[roomId];
        if( vecMapDistributeInfo.empty() )
        {
            DistributeInfo* info = new DistributeInfo(player->GetId(),fighterId,x,y);
            std::vector<DistributeInfo*> vecDistributeInfo;
            vecDistributeInfo.push_back(info);
            MapDistributeInfo* mapdistribute  = new MapDistributeInfo(mapId,vecDistributeInfo);
            vecMapDistributeInfo.push_back(mapdistribute);
            _room2Distribute[roomId] = vecMapDistributeInfo;
        }
        else
        {
              MapDistributeInfo* mapDistribute = GetMapDistributionInfo(vecMapDistributeInfo,mapId);
              if( mapDistribute == NULL )
              {
                    //更新数据
                    DistributeInfo* info = new DistributeInfo(player->GetId(),fighterId,x,y);
                    std::vector<DistributeInfo*> vecDistributeInfo;
                    vecDistributeInfo.push_back(info);
                    MapDistributeInfo* mapdistribute  = new MapDistributeInfo(mapId,vecDistributeInfo);
                    vecMapDistributeInfo.push_back(mapdistribute);
                    _room2Distribute[roomId] = vecMapDistributeInfo;
              }
              else
              {
                    std::vector<DistributeInfo*> vecInfo = mapDistribute->GetDistributeInfo();
                    DistributeInfo* info =  GetDistributeInfo(vecInfo,x,y);
                    if( info != NULL )   //此点有人耶
                    {
                        return false;
                    }
                    else
                    {
                        DistributeInfo* info = new DistributeInfo(player->GetId(),fighterId,x,y);
                        vecInfo.push_back(info);
                        for( auto it = vecMapDistributeInfo.begin() ; it != vecMapDistributeInfo.end() ; ++it )
                        {
                            if( (*it)->GetMapId() == mapId )
                            {
                               (*it)->SetDistributeInfo(vecInfo);
                            }
                        }
                        _room2Distribute[roomId] = vecMapDistributeInfo;
                    }
                }
        }

        //更新数据库哈
        if( flag )
        {
            player->InsertClanBattleFighter(mapId,fighterId,x,y);
            DB7().PushUpdateData("REPLACE INTO `clan_battle_pos` value(%u,%"I64_FMT"u,%u,%u,%u)",mapId,player->GetId(),fighterId,x,y);
        }
        return true;
    }

    MapDistributeInfo* BattleDistribute::GetMapDistributionInfo(std::vector<MapDistributeInfo*>& vecMapDistributeInfo, UInt8 mapId)
    {
        for( auto it = vecMapDistributeInfo.begin() ; it != vecMapDistributeInfo.end() ; ++it )
        {
            if( (*it)->GetMapId() == mapId )
            {
                return (*it);
            }
        }
        return NULL;

    }

    //取消放将
    bool BattleDistribute::CancelPutFighter(UInt8 mapId, GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y)
    {
        bool status = Check(player);
        if( !status )
        {
            return false;
        }
        UInt32 roomId = GetBattleRoomId(player);
        if( roomId == 0 )
        {
            return false;
        }
        DistributeInfo* info = GetDistributeInfo(roomId,mapId, x, y );
        if( info == NULL )
        {
            return false;
        }
        else
        {
            //删掉这个数据
            RemoveDistributeInfo(roomId,mapId,info);
            player->DelClanBattleFighter(mapId,fighterId,x,y);
        }
        return true;
    }



    void BattleDistribute::RemoveDistributeInfo(UInt32 roomId,UInt8 mapId,DistributeInfo* info)
    {
        std::vector<MapDistributeInfo*>  vecMapDistribute = _room2Distribute[roomId];
        if( vecMapDistribute.empty() )
        {
            return;
        }
        else
        {
            MapDistributeInfo* mapDistribute = GetMapDistributionInfo(vecMapDistribute,mapId);
            if( mapDistribute == NULL )
            {
                return ;
            }
            else
            {
                std::vector<DistributeInfo*> vecDistributeInfo = mapDistribute->GetDistributeInfo();
                for(auto it = vecDistributeInfo.begin() ; it != vecDistributeInfo.end() ; ++it )
                {
                    if( (*it) == info )
                    {
                        it = vecDistributeInfo.erase(it);
                        break;
                    }
                }

                for( auto it = vecMapDistribute.begin() ; it != vecMapDistribute.end() ; ++it )
                {
                    if((*it)->GetMapId() == mapId )
                    {
                        (*it)->SetDistributeInfo(vecDistributeInfo);
                    }
                }
                _room2Distribute[roomId] = vecMapDistribute;
                DB7().PushUpdateData("delete from `clan_battle_pos` where `mapId`= %u AND `playerId` = %" I64_FMT "u  AND `fighterId` = %u",mapId,info->GetPlayerId(),info->GetFighterId());
                GObject::Player* player = GObject::globalPlayers[info->GetPlayerId()];
                player->DelClanBattleFighter(mapId,info->GetFighterId(),info->GetPosX(),info->GetPosY());
            }
        }
    }


    DistributeInfo* BattleDistribute::GetDistributeInfo(UInt32 roomId, UInt8 mapId, UInt8 x, UInt8 y)
    {
        std::vector<MapDistributeInfo*>  vecMapDistribute = _room2Distribute[roomId];
        if( vecMapDistribute.empty() )
        {
            return NULL;
        }
        else
        {
            MapDistributeInfo* mapDistribute = GetMapDistributionInfo(vecMapDistribute,mapId);
            if( mapDistribute == NULL )
            {
                return NULL;
            }
            else
            {
                std::vector<DistributeInfo*> vecDistributeInfo = mapDistribute->GetDistributeInfo();
                if( vecDistributeInfo.empty() )
                {
                    return NULL;
                }

                return GetDistributeInfo(vecDistributeInfo,x,y);
            }
        }
    }


    //交换战将
    bool BattleDistribute::MoveFighter(UInt8 mapId,GObject::Player* player,UInt8 curx,UInt8 cury,UInt8 destx,UInt8 desty)
    {
        UInt32 roomId = GetBattleRoomId(player);
        if( roomId == 0 )
            return false;
        DistributeInfo* destInfo  = GetDistributeInfo(roomId,mapId,destx,desty);
        DistributeInfo* currentInfo  = GetDistributeInfo(roomId,mapId,curx,cury);
        if( currentInfo == NULL )
        {
            return false;
        }

        if( destInfo == NULL )
        {
            RemoveDistributeInfo(roomId,mapId,currentInfo);
            PutFighter(mapId,player,currentInfo->GetFighterId(),destx,desty,true);
        }
        else
        {
            if( destInfo->GetPlayerId() != currentInfo->GetPlayerId() || destInfo->GetFighterId() == currentInfo->GetFighterId() ) //这两个将不属于同一个玩家
            {
                return false;
            }
            //还要判断这两个坐标是不是同一阵营
            GData::MapInfo* mapInfo = GData::mapTable.GetMapInfo(mapId);
            vecInfo campInfo = mapInfo->GetTileInfo();
            UInt8 width = mapInfo->GetWidth();
            if( campInfo[curx+cury*width] != 0 && campInfo[destx+desty*width] != 0 && campInfo[curx+cury*width] == campInfo[destx+desty*width])
            {
                RemoveDistributeInfo(roomId,mapId,currentInfo);
                PutFighter(mapId,player,destInfo->GetFighterId(),curx,cury,true);
            
                RemoveDistributeInfo(roomId,mapId,destInfo);
                PutFighter(mapId,player,currentInfo->GetFighterId(),destx,desty,true);
            }

        }
        return true;
    }

    bool BattleDistribute::MoveFighterWithDiffTown(GObject::Player* player,UInt8 curMapId,UInt8 curx,UInt8 cury,UInt8 destMapId, UInt8 destx,UInt8 desty)
    {
        UInt32 roomId = GetBattleRoomId(player);
        if( roomId == 0 )
        {
            return false;
        }
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
        {
            return false;
        }
        UInt8 battleId = room->GetBattleId();
        GData::BattleMapInfo* info = GData::battleMapTable.GetBattleMapInfo(battleId);
        if( info == NULL )
        {
            return false;
        }
        GData::SingleMapInfo* singleInfo = info->GetSingleMapInfo(curMapId);
        if( singleInfo == NULL)
        {
            return false;
        }
        bool isLink = singleInfo->IsLink(destMapId);
        //得判断两个城市之间的联通关系
        if( !isLink )
        {
            return false;
        }
        DistributeInfo* curInfo = GetDistributeInfo(roomId,curMapId,curx,cury);
        DistributeInfo* destInfo = GetDistributeInfo(roomId,destMapId,destx,desty);
        if( curInfo == NULL )
        {
            return false;
        }
        if( destInfo == NULL )
        {
            //移动
            RemoveDistributeInfo(roomId,curMapId,curInfo);
            PutFighter(destMapId,player,curInfo->GetFighterId(),destx,desty,true);
        }
        else
        {
            //交换
            RemoveDistributeInfo(roomId,curMapId,curInfo);
            PutFighter(destMapId,player,destInfo->GetFighterId(),curx,cury,true);
            
            RemoveDistributeInfo(roomId,destMapId,destInfo);
            PutFighter(curMapId,player,curInfo->GetFighterId(),destx,desty,true);
        }
        return true;
    }
    
     
    //获取战场信息 
    void BattleDistribute::GetBattleInfo(GObject::Player* player,Stream &st)
    {
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
        {
            return ;
        }
        UInt32 roomId = GetBattleRoomId(player);
        if( roomId == 0 )
        {
            return;
        }
        UInt8 forceId = clan->GetBattleForceId();
        if( forceId == 0 )
        {
            return;
        }
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
        {
            return;
        }
        //UInt8 battleId = room->GetBattleId();
        //GData::BattleMapInfo* info = GData::battleMapTable.GetBattleMapInfo(battleId);

        //UInt8 citys = info->GetMapNum();
        st<<static_cast<UInt8>(forceId);
        std::vector<MapDistributeInfo*> vecMapDistributeInfo = _room2Distribute[roomId];
        st<<static_cast<UInt8>(vecMapDistributeInfo.size());  //有人的发 没人的不发
       


        //自己的排布情况
        std::set<UInt8> enemyForce;
        for(auto it = vecMapDistributeInfo.begin(); it != vecMapDistributeInfo.end(); ++it)
        {
            st<<static_cast<UInt8>((*it)->GetMapId());
            UInt8 count = 0;
            size_t offset = st.size();
            st<<static_cast<UInt8>(count);
            std::vector<DistributeInfo*> vecDistributeInfo = (*it)->GetDistributeInfo();
            for( auto iter = vecDistributeInfo.begin(); iter != vecDistributeInfo.end(); ++iter )
            {
                if( (*iter)->GetPlayerId() == player->GetId())
                {
                    st<<static_cast<UInt16>((*iter)->GetFighterId());
                    st<<static_cast<UInt8>((*iter)->GetPosX());
                    st<<static_cast<UInt8>((*iter)->GetPosY());
                    ++count;
                }
            }
            st.data<UInt8>(offset) = count;
            

            //同盟方
            size_t off = st.size();
            UInt8 allies = 0; 
            st<<static_cast<UInt8>(allies);
            for( auto iter = vecDistributeInfo.begin(); iter != vecDistributeInfo.end(); ++iter )
            {
                UInt8 forceid = GetForceId((*iter)->GetPlayerId());
                if( forceid == forceId  && (*iter)->GetPlayerId() != player->GetId() )
                {
                    st<<static_cast<UInt16>((*iter)->GetFighterId());
                    st<<static_cast<UInt8>((*iter)->GetPosX());
                    st<<static_cast<UInt8>((*iter)->GetPosY());
                    ++allies;
                }
                if( forceid != forceId )
                {
                    enemyForce.insert(forceid);
                }
            }
            st.data<UInt8>(off) = allies;

            //敌方  只发势力Id
            st<<static_cast<UInt8>(enemyForce.size());

            if( !enemyForce.empty() )
            {
                for( auto it = enemyForce.begin(); it != enemyForce.end(); ++it )
                {
                    st<<static_cast<UInt8>(*it);
                }
            }

        }

        //留言
        Battle::roomCommentManager.GetAllisComment(player,st);
        //军团令
        Battle::roomOrderManager.GetClanOrder(player,st);

    }

    UInt8 BattleDistribute::GetForceId(UInt64 playerId)
    {
        GObject::Player* pl = GObject::globalPlayers[playerId];
        if( pl == NULL )
        {
            return 0;
        }
        GObject::Clan * clan = pl->GetClan();
        if( clan == NULL )
        {
            return 0;
        }
        return clan->GetBattleForceId();
    }


    void BattleDistribute::GetAllies(GObject::Player* player,std::set<GObject::Player*>& playerSet)
    {
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
        {
            return ;
        }
        UInt32 roomId = clan->GetClanBattleRoomId();
        if( roomId == 0 )
        {
            return ;
        }
        ClanBattleRoom* room  = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
        {
            return;
        }

        UInt8 forceId = clan->GetBattleForceId();
        if( forceId == 0 )
        {
            return;
        }
        std::vector<UInt32> vecClan = room->GetAllyClans(forceId);
        if( vecClan.empty())
        {
            return;
        }
        
        for( auto it = vecClan.begin(); it != vecClan.end(); ++it)
        {
            GObject::Clan* Clan = GObject::globalClan[(*it)];
            if( clan == NULL )
                continue;
            std::vector<GObject::Player*> vecPlayer = Clan->GetJoinClanBattlePlayer();
            if( vecPlayer.empty())
                continue;
            for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
            {
                playerSet.insert(*it);
            }

        }
    }


    void BattleDistribute::NoticeAlliesAddFighter(GObject::Player* player, UInt16 fighterId)
    {
         std::set<GObject::Player*> playerSet;
         GetAllies(player,playerSet);
         if( playerSet.empty())
             return;
         GObject::ClanBattleFighter* fighterInfo = player->GetClanBattleFighter(fighterId);
         if( fighterInfo == NULL )
         {
             return;
         }

         UInt8 mapId = fighterInfo->GetMapId();
         UInt8 posx  = fighterInfo->GetPosX();
         UInt8 posy  = fighterInfo->GetPosY();
         GObject::Clan* clan = player->GetClan();
         UInt8 forceId = clan->GetBattleForceId();

         for( auto it = playerSet.begin(); it != playerSet.end(); ++it)
         {
             if( (*it) == player )
             {
                 continue;
             }
             Stream st(REP::CLAN_BATTLE_ADDFIGHTER);
             st<<static_cast<UInt8>(1);
             st<<static_cast<UInt8>(mapId);
             st<<static_cast<UInt16>(fighterId);
             st<<static_cast<UInt8>(posx);
             st<<static_cast<UInt8>(posy);
             st<<static_cast<UInt8>(forceId);
             st<<Stream::eos;
             (*it)->send(st);
         }
    }

    void BattleDistribute::NoticeAlliesDelFighter(GObject::Player* player,UInt8 mapId,UInt8 posx,UInt8 posy)
    {
         std::set<GObject::Player*> playerSet;
         GetAllies(player,playerSet);
         if( playerSet.empty())
             return;

         for( auto it = playerSet.begin(); it != playerSet.end(); ++it )
         {
             if((*it) == player )
                 continue;
             Stream st(REP::CLAN_BATTLE_CANCELFIGHTER);
             st<<static_cast<UInt8>(mapId);
             st<<static_cast<UInt8>(posx);
             st<<static_cast<UInt8>(posy);
             st<<Stream::eos;
             (*it)->send(st);
         }
    }

    void BattleDistribute::NoticeAlliesMoveFighter(GObject::Player* player,UInt8 curMapId,UInt8 curx,UInt8 cury,UInt8 destMapId,UInt8 destx,UInt8 desty)
    {
         std::set<GObject::Player*> playerSet;
         GetAllies(player,playerSet);
         if( playerSet.empty())
             return;

         for( auto it = playerSet.begin(); it != playerSet.end(); ++it )
         {
             if( (*it) == player )
                 continue;
             Stream st(REP::CLAN_BATTLE_MOVEFIGHTER);
             st<<static_cast<UInt8>(1);
             st<<static_cast<UInt8>(curMapId);
             st<<static_cast<UInt8>(curx);
             st<<static_cast<UInt8>(cury);
             st<<static_cast<UInt8>(destMapId);
             st<<static_cast<UInt8>(destx);
             st<<static_cast<UInt8>(desty);
             st<<Stream::eos;
             (*it)->send(st);
         }

    }

}

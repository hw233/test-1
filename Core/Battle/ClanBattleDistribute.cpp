#include"ClanBattleDistribute.h"
#include"ClanBattleRoom.h"
#include"ClanOrder.h"
#include"ClanBattleComment.h"
#include"Battle/Report2Id.h"
#include"Battle/ClanBattleCityStatus.h"
#include"GObject/Var.h"

#include"MsgID.h"

namespace Battle
{
    BattleDistribute battleDistribute;

    void DistributeInfo::SetSoldiersHP(UInt32 childTypeId)
    {
        GObject::Fighter* fgt = GObject::globalFighters[childTypeId];
        if( fgt == NULL )
            return;
        UInt32 hp = fgt->GetHP();
        for( UInt8 i = 0 ; i < INIT_SOLDIER_NUM ; ++i )
        {
             SoldiersHP.push_back(hp);//小兵的血量暂时定为100
        }
    }


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
    //特殊说明一下这个tag  在放将初期需要检查阵营和位置是否一致(tag == 0 )    但在打的阶段开打以后就不需要这个检测了( tag == 1 )
    bool BattleDistribute::PutFighter(UInt8 mapId,GObject::Player* player,UInt16 fighterId, UInt8 x, UInt8 y,bool flag,UInt8 tag)
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
        

        if( tileInfo[x+y*width] == 0 )     //该点是不可放置的点
        {
            return false;
        }

        GObject::Clan* clan = player->GetClan();
        UInt8 side = clan->GetBattleForceId();
        
        if( tag == 0 )
        {
            if( campInfo[x+y*width] != side )  //放到敌人的阵营了
            {
                return false;
            }
        }
        //下面判断该点上是不是有人
        //
        DistributeInfo* pInfo = NULL;
        std::vector<MapDistributeInfo*> vecMapDistributeInfo = _room2Distribute[roomId];
        if( vecMapDistributeInfo.empty() )
        {
            DistributeInfo* info = new(std::nothrow) DistributeInfo(player->GetId(),fighterId,x,y);
            if( info == NULL )
                return false;
            //info->SetSoldierNum(INIT_SOLDIER_NUM);
            std::vector<DistributeInfo*> vecDistributeInfo;
            vecDistributeInfo.push_back(info);
            MapDistributeInfo* mapdistribute  = new(std::nothrow) MapDistributeInfo(mapId);
            if( mapdistribute == NULL )
                return false;
            mapdistribute->SetDistributeInfo(vecDistributeInfo);
            vecMapDistributeInfo.push_back(mapdistribute);
            _room2Distribute[roomId] = vecMapDistributeInfo;
            pInfo = info;
        }
        else
        {
              MapDistributeInfo* mapDistribute = GetMapDistributionInfo(vecMapDistributeInfo,mapId);
              if( mapDistribute == NULL )
              {
                    //更新数据
                    DistributeInfo* info = new(std::nothrow) DistributeInfo(player->GetId(),fighterId,x,y);
                    if( info == NULL )
                        return false;
                    std::vector<DistributeInfo*> vecDistributeInfo;
                    vecDistributeInfo.push_back(info);
                    MapDistributeInfo* mapdistribute  = new(std::nothrow) MapDistributeInfo(mapId);
                    if( mapdistribute == NULL )
                        return false;
                    mapdistribute->SetDistributeInfo(vecDistributeInfo);
                    vecMapDistributeInfo.push_back(mapdistribute);
                    _room2Distribute[roomId] = vecMapDistributeInfo;
                    pInfo = info;
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
                        //info->SetSoldierNum(INIT_SOLDIER_NUM);
                        vecInfo.push_back(info);
                        for( auto it = vecMapDistributeInfo.begin() ; it != vecMapDistributeInfo.end() ; ++it )
                        {
                            if( (*it)->GetMapId() == mapId )
                            {
                               (*it)->SetDistributeInfo(vecInfo);
                            }
                        }
                        _room2Distribute[roomId] = vecMapDistributeInfo;
                        pInfo = info;
                    }
                }
        }

        //更新数据库哈
        if( flag )
        {
            DB7().PushUpdateData("REPLACE INTO `clan_battle_pos`(`roomId`,`mapId`,`playerId`,`fighterId`,`posx`,`posy`) value(%u,%u,%"I64_FMT"u,%u,%u,%u)",roomId,mapId,player->GetId(),fighterId,x,y);
            if( flag == true && tag == 0 )
            {
                GObject::Fighter* fgt = player->findFighter(fighterId);
                UInt32 childTypeId = fgt->GetChildTypeId();
                pInfo->SetSoldiersHP(childTypeId);
                UpdateSoldiersHP(roomId,mapId,x,y,pInfo->GetSoldiersHP());
                if( fgt == NULL )
                    return false;
                UpdateMainFighterHP(roomId,mapId,x,y,fgt->GetHP());
                player->SetVar(GObject::VAR_CLANBATTLE_FIGHTERNUM,player->GetVar(GObject::VAR_CLANBATTLE_FIGHTERNUM)+1);
            }
        }
        player->InsertClanBattleFighter(mapId,fighterId,x,y);
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
    bool BattleDistribute::RemoveFighter(UInt8 mapId, GObject::Player* player,UInt16 fighterId,UInt8 x,UInt8 y)
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
        Battle::RoomAllCityStatus* roomStatus = Battle::roomAllCityStatusManager.GetRoomAllCityStatus(roomId);
        if( roomStatus != NULL )
        {
            if( roomStatus->GetStage() == 0 )
            {
                if( player->GetVar(GObject::VAR_CLANBATTLE_FIGHTERNUM) > 1 )
                {
                    player->SetVar(GObject::VAR_CLANBATTLE_FIGHTERNUM,player->GetVar(GObject::VAR_CLANBATTLE_FIGHTERNUM)-1);
                }
            }
        }
        return true;
    }


    MapDistributeInfo* BattleDistribute::GetMapDistributionInfo(UInt32 roomId,UInt8 mapId)
    {
        std::vector<MapDistributeInfo*>  vecMapDistribute = _room2Distribute[roomId];
        if( vecMapDistribute.empty() )
        {
            return NULL;
        }
        MapDistributeInfo* mapDistribute = GetMapDistributionInfo(vecMapDistribute,mapId);
        return mapDistribute;
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
                for(auto it = vecDistributeInfo.begin() ; it != vecDistributeInfo.end() ;)
                {
                    if( (*it) == info )
                    {
                        it = vecDistributeInfo.erase(it);
                        break;
                    }
                    else
                    {
                        ++it;
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
                DB7().PushUpdateData("delete from `clan_battle_pos` where `roomId` = %u AND `mapId`= %u AND `playerId` = %" I64_FMT "u  AND `fighterId` = %u",roomId,mapId,info->GetPlayerId(),info->GetFighterId());
                if( info->GetPlayerId() != 0 )   //非npc
                {
                    GObject::Player* player = GObject::globalPlayers[info->GetPlayerId()];
                    player->DelClanBattleFighter(mapId,info->GetFighterId(),info->GetPosX(),info->GetPosY());
                }
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
    bool BattleDistribute::MoveFighter(UInt8 mapId,GObject::Player* player,UInt8 curx,UInt8 cury,UInt8 destx,UInt8 desty,UInt8 tag)
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
            PutFighter(mapId,player,currentInfo->GetFighterId(),destx,desty,true,tag);
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
                RemoveDistributeInfo(roomId,mapId,destInfo);
                PutFighter(mapId,player,destInfo->GetFighterId(),curx,cury,true,tag);
                PutFighter(mapId,player,currentInfo->GetFighterId(),destx,desty,true,tag);
                delete destInfo;
                destInfo = NULL;
                delete currentInfo;
                currentInfo = NULL;
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
            RemoveDistributeInfo(roomId,destMapId,destInfo);
            PutFighter(destMapId,player,destInfo->GetFighterId(),curx,cury,true);
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
        Battle::RoomAllCityStatus* status = Battle::roomAllCityStatusManager.GetRoomAllCityStatus(roomId);
        st<<static_cast<UInt8>(status->GetStage());
        st<<static_cast<UInt8>(forceId);
        std::vector<MapDistributeInfo*> vecMapDistributeInfo = _room2Distribute[roomId];
        st<<static_cast<UInt8>(vecMapDistributeInfo.size());  //有人的发 没人的不发

        //自己的排布情况
        std::set<UInt8> enemyForce;
        for(auto it = vecMapDistributeInfo.begin(); it != vecMapDistributeInfo.end(); ++it)
        {
            UInt8 mapId = (*it)->GetMapId();
            st<<static_cast<UInt8>(mapId);
            UInt8 ownforce = status->GetCityOwnForce(mapId);
            st<<static_cast<UInt8>(ownforce);
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
                    GObject::Player* pl = GObject::globalPlayers[(*iter)->GetPlayerId()];
                    if( pl == NULL )
                        continue;
                    GObject::Fighter* fgt = pl->findFighter((*iter)->GetFighterId());
                    if( fgt == NULL )
                        continue;
                    st<<static_cast<UInt16>(fgt->GetTotalPower());
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


            //在己方势力范围上的敌人(上一次战斗移动至此)
            GData::MapInfo* mapInfo = GData::mapTable.GetMapInfo(mapId);
            UInt8 width = mapInfo->GetWidth();
            vecInfo campInfo = mapInfo->GetCampInfo();
           

            std::map<UInt8,std::vector<InMyZoneEnemy>> force2InMyZoneEnemy;
            for( auto iter = vecDistributeInfo.begin(); iter != vecDistributeInfo.end(); ++iter)
            {
                UInt8 forceid = GetForceId((*iter)->GetPlayerId());
                if( forceid != forceId )  //人不属于自己
                {
                    UInt8 x = (*iter)->GetPosX();
                    UInt8 y = (*iter)->GetPosY();
                    if( campInfo[x+y*width] == forceId )  //但地方是自己的
                    {
                        if( force2InMyZoneEnemy[forceid].empty())
                        {
                            std::vector<InMyZoneEnemy> vecInMyZoneEnemy;
                            vecInMyZoneEnemy.push_back(InMyZoneEnemy((*iter)->GetFighterId(),x,y));
                            force2InMyZoneEnemy[forceid] = vecInMyZoneEnemy;
                        }
                        else
                        {
                            force2InMyZoneEnemy[forceid].push_back(InMyZoneEnemy((*iter)->GetFighterId(),x,y));
                        }
                    }
                }
            }

            for( auto it = enemyForce.begin(); it != enemyForce.end(); ++it )
            {
                st<<static_cast<UInt8>(*it);
                std::vector<InMyZoneEnemy> vecInMyZoneEnemy = force2InMyZoneEnemy[*it];
                st<<static_cast<UInt8>( vecInMyZoneEnemy.size());
                if( !vecInMyZoneEnemy.empty())
                 {
                     for(auto iter = vecInMyZoneEnemy.begin(); iter != vecInMyZoneEnemy.end();  ++iter )
                     {
                         st<<static_cast<UInt16>((*iter).fighterId);
                         st<<static_cast<UInt8>((*iter).x);
                         st<<static_cast<UInt8>((*iter).y);
                     }
                 }
            }

            UInt32 reportId = Battle::report2IdTable.GetRecentReportId(roomId,mapId);
            if( reportId != 0 )
            {
                std::cout<<"最近的一个战报是    "<<static_cast<UInt32>(reportId)<<std::endl;
            }
            if( status->GetStage() == 1 )
            {
                st<<static_cast<UInt32>(reportId);
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



    void BattleDistribute::NoticeAlliesAddFighter(GObject::Player* player, UInt16 fighterId)
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
        std::vector<GObject::Player*> vecPlayer = room->GetSameForceAllies(forceId);
        if( vecPlayer.empty())
            return;
        GObject::ClanBattleFighter* fighterInfo = player->GetClanBattleFighter(fighterId);
        if( fighterInfo == NULL )
        {
            return;
        }

        UInt8 mapId = fighterInfo->GetMapId();
        UInt8 posx  = fighterInfo->GetPosX();
        UInt8 posy  = fighterInfo->GetPosY();

        for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it)
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
            st<<Stream::eos;
            (*it)->send(st);
        }
    }

    void BattleDistribute::NoticeAlliesDelFighter(GObject::Player* player,UInt8 mapId,UInt8 posx,UInt8 posy)
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
        std::vector<GObject::Player*> vecPlayer = room->GetSameForceAllies(forceId);
        if( vecPlayer.empty())
             return;

        for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
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
        std::vector<GObject::Player*> vecPlayer = room->GetSameForceAllies(forceId);
        if( vecPlayer.empty())
            return;

        for( auto it = vecPlayer.begin(); it != vecPlayer.end(); ++it )
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

    void BattleDistribute::UpdateMainFighterHP(UInt32 roomId,UInt8 mapId,UInt8 x,UInt8 y, UInt32 hp)
    {
        DistributeInfo* info = GetDistributeInfo(roomId,mapId,x,y);
        if( info == NULL)
            return;
        info->SetMainFighterHP(hp);
        //更新数据库
        DB7().PushUpdateData("update `clan_battle_pos`  set `mainFighterHP` = %u where `roomId`=%u AND`mapId`= %u AND `playerId` = %" I64_FMT "u  AND `fighterId` = %u",hp, roomId,mapId,info->GetPlayerId(),info->GetFighterId());

    }

    void BattleDistribute::UpdateSoldiersHP(UInt32 roomId , UInt8 mapId,UInt8 x,UInt8 y,std::vector<UInt32>vecHP)
    {
        DistributeInfo* info = GetDistributeInfo(roomId,mapId,x,y);
        if( info == NULL)
            return;
        info->SetSoldiersHP(vecHP);
        char buff[1024];
        UInt8 offset = 0 ;
        for(auto it = vecHP.begin(); it != vecHP.end() ; ++it )
        {
            offset += sprintf(buff+offset,"%d,",(*it));
        }
        buff[offset-1] = '\0';
        std::string hps(buff);
        DB7().PushUpdateData("update `clan_battle_pos`  set `soldiersHP` = '%s' where `roomId`=%u AND `mapId`= %u AND `playerId` = %" I64_FMT "u  AND `fighterId` = %u",hps.c_str(), roomId,mapId,info->GetPlayerId(),info->GetFighterId());
    }


   
    void BattleDistribute::SetMainFighterAndSoldiersHP(UInt32 roomId,UInt8 mapId,UInt8 x,UInt8 y,std::vector<UInt32> vecSoldiersHP,UInt32 mainFighterHP)
    {
        DistributeInfo* info = GetDistributeInfo(roomId,mapId,x,y);
        if( info == NULL )
            return;
        info->SetSoldiersHP(vecSoldiersHP);
        info->SetMainFighterHP(mainFighterHP);
    }


    void BattleDistribute::PutNpc(UInt32 roomId,UInt8 mapId,UInt16 fighterId,UInt8 x,UInt8 y,bool flag)
    {
        DistributeInfo* info = GetDistributeInfo(roomId,mapId,x,y);
        if( info != NULL )
            return ;
        GData::MapInfo* mapInfo = GData::mapTable.GetMapInfo(mapId);
        UInt8 width = mapInfo->GetWidth();
        UInt8 height = mapInfo->GetHeight();
        vecInfo tileInfo = mapInfo->GetTileInfo();
        vecInfo campInfo = mapInfo->GetCampInfo();

        if( x >= width || y >= height )    //坐标超出了地图的边界
        {
            return ;
        }
        
        if( tileInfo[x+y*width] == 0 )     //该点是不可放置的点
        {
            return ;
        }
        UInt64 playerId=0;
        DistributeInfo* newInfo = new DistributeInfo(0,fighterId,x,y);
        if( _room2Distribute[roomId].empty())
        {
            std::vector<MapDistributeInfo*> vecMapDistributeInfo;
            std::vector<DistributeInfo*> vecDistributeInfo;
            vecDistributeInfo.push_back(newInfo);
            MapDistributeInfo* mapdistribute  = new(std::nothrow) MapDistributeInfo(mapId);
            if( mapdistribute == NULL )
                return ;
            mapdistribute->SetDistributeInfo(vecDistributeInfo);
            vecMapDistributeInfo.push_back(mapdistribute);
            _room2Distribute[roomId] = vecMapDistributeInfo;
        }
        else
        {
            std::vector<MapDistributeInfo*> vecMapDistributeInfo = _room2Distribute[roomId];
            MapDistributeInfo* mapDistributeInfo = GetMapDistributionInfo(roomId,mapId);
            if( mapDistributeInfo != NULL )
            {
                std::vector<DistributeInfo*> vecInfo = mapDistributeInfo->GetDistributeInfo();
                vecInfo.push_back(newInfo);
                for( auto it = vecMapDistributeInfo.begin() ; it != vecMapDistributeInfo.end() ; ++it )
                {
                    if( (*it)->GetMapId() == mapId )
                    {
                        (*it)->SetDistributeInfo(vecInfo);
                    }
                }
            }
            else
            {
                MapDistributeInfo* mapdistribute  = new(std::nothrow) MapDistributeInfo(mapId);
                std::vector<DistributeInfo*> vecDistributeInfo;
                vecDistributeInfo.push_back(newInfo);
                mapdistribute->SetDistributeInfo(vecDistributeInfo);
                vecMapDistributeInfo.push_back(mapdistribute);
            }
            _room2Distribute[roomId] = vecMapDistributeInfo;
        }
        if( flag == true )
        {
            DB7().PushUpdateData("REPLACE INTO `clan_battle_pos`(`roomId`,`mapId`,`playerId`,`fighterId`,`posx`,`posy`) value(%u, %u, %"I64_FMT"u, %u, %u, %u)",roomId,mapId,playerId,fighterId,x,y);
        }


    }

    void BattleDistribute::RemoveNpc(UInt32 roomId,UInt8 mapId,UInt8 curx,UInt8 cury)
    {
        DistributeInfo* info = GetDistributeInfo(roomId,mapId,curx,cury);
        if( info == NULL)
             return ;
        RemoveDistributeInfo(roomId,mapId,info);
    }

    
    //野怪的逻辑比较特殊
    void BattleDistribute::MoveNpc(UInt32 roomId,UInt8 mapId,UInt16 fighterId,UInt8 curx ,UInt8 cury,UInt8 destx,UInt8 desty)
    {
        //先按照位置获得排布信息
        DistributeInfo* curInfo = GetDistributeInfo(roomId,mapId,curx,cury);
        DistributeInfo* destInfo = GetDistributeInfo(roomId,mapId,destx,desty);
        if( destInfo != NULL )
        {
            return;
        }
        if( curInfo == NULL )
        {
            PutNpc(roomId,mapId,fighterId,destx,desty);
        }
        else
        {
            RemoveDistributeInfo(roomId,mapId,curInfo);
            PutNpc(roomId,mapId,fighterId,destx,desty);
        }
    }

    void BattleDistribute::RemoveRoomDistribute(UInt32 roomId) //删除某一房间的所有排布情况
    {
        for( auto it = _room2Distribute.begin(); it != _room2Distribute.end(); ++it )
        {
            if( it->first == roomId )
            {
                (it->second).clear();
                _room2Distribute.erase(it->first);
                break;
            }
        }
    }
}

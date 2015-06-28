#include"ClanBattleCityStatus.h"
#include"Battle/ClanBattleRoom.h"


namespace Battle
{
   RoomAllCityStatusManager roomAllCityStatusManager;

    void RoomAllCityStatus::SetStage(UInt32 t)
    {
        //报名之后的第二天10点为战术推演阶段 
        //由报名时间得到
        ClanBattleRoom* room = Battle::clanBattleRoomManager.GetBattleRoom(roomId);
        if( room == NULL )
        {
            return;
        }
        UInt32 buildTime = room->GetBuildTime();
        time_t time = buildTime;
        tm* tt=localtime(&time);
        UInt8 hour = tt->tm_hour;
        UInt8 min = tt->tm_min;


        UInt32 sday =buildTime+((24-hour)*60-min)*60+36000;   //第二天十点的时间的戳
        UInt8 newStage = 0;
        if( t < sday )
        {
            newStage = 0;
        }
        else if ( t > sday && t < sday + 1*86400)
        {
            newStage = 1;
        }
        else if(  t > sday+1*86400 && t < sday+2*86400)
        {
            newStage = 0;
        }
        else if(  t > sday+2*86400 && t < sday+3*86400 )
        {
            newStage = 1;
        }
        else if(  t > sday+4*86400 && t < sday+5*86400 )
        {
            newStage = 0;
        }
        else if(  t > sday+5*86400 && t < sday+6*86400 )
        {
            newStage = 1;
        }
        else if(  t > sday+6*86400 && t < sday+7*86400 )
        {
            newStage = 2;
        }
        else
        {
            newStage = 3;
        }

        if( newStage != stage )
        {
            stage = newStage;
        }

    }

   void RoomAllCityStatus::SetCityOwnForce(UInt8 cityId,UInt8 force)
   {
       for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it )
       {
           if( (*it)->GetCityId() == cityId )
           {
               (*it)->SetOwnForce(force);
               break;
           }
       }
   }

   void RoomAllCityStatus::Init()
   {
       //根据战役Id 设置每一座城的拥有者
       GData::BattleMapInfo* info = GData::battleMapTable.GetBattleMapInfo(battleId);
       std::vector<GData::SingleMapInfo*> vecSingleMapInfo = info->GetSingleMapInfos();
       for( auto it = vecSingleMapInfo.begin(); it != vecSingleMapInfo.end(); ++it )
       {
           vecCityStatus.push_back(new CityStatus((*it)->GetMapId(),(*it)->GetForce()));
           //如果拥有者的势力不是0 则写入数据库
           if( (*it)->GetForce() != 0 )
           {
               DB7().PushUpdateData("INSERT INTO  `clan_battle_citystatus`(`roomId`,`battleId`,`cityId`,`ownforce`) VALUES(%u,%u,%u,%u)",roomId,battleId,(*it)->GetMapId(),(*it)->GetForce());
           }
       }
   }


   UInt8 RoomAllCityStatus::GetCityOwnForce(UInt8 cityId)
   {
       for( auto it = vecCityStatus.begin(); it != vecCityStatus.end(); ++it )
       {
           if( (*it)->GetCityId() == cityId )
           {
               return (*it)->GetOwnForce();
           }
       }
       return 0;
   }

   void RoomAllCityStatusManager::InsertRoomAllCityStatus(UInt32 roomId,UInt8 battleId)
   {
       RoomAllCityStatus* roomAllStatus = new(std::nothrow) RoomAllCityStatus(roomId,battleId);
       if( roomAllStatus == NULL )
           return;
       roomAllStatus->Init();
       _vecRoomAllCityStatus.push_back(roomAllStatus);
       //
   }


   RoomAllCityStatus* RoomAllCityStatusManager::GetRoomAllCityStatus(UInt32 roomId)
   {
       for( auto it = _vecRoomAllCityStatus.begin(); it != _vecRoomAllCityStatus.end(); ++it )
       {
           if( (*it)->GetRoomId() == roomId )
           {
               return (*it);
           }
       }
       return NULL;
   }


   void RoomAllCityStatusManager::loadCityStatus(UInt32 roomId,UInt8 battleId,UInt8 cityId,UInt8 ownforce)
   {
       RoomAllCityStatus* status = GetRoomAllCityStatus(roomId);
       if( status == NULL )
       {
            status = new RoomAllCityStatus(roomId,battleId);
            status->InsertCityStatus(cityId,ownforce);
            _vecRoomAllCityStatus.push_back(status);
       }
       else
       {
           status->InsertCityStatus(cityId,ownforce);
       }


   }

}

#include"ClanBattleComment.h"
#include"ClanBattleRoom.h"
#include"MsgID.h"
#include"GObject/Var.h"
#include "Common/TimeUtil.h"

namespace Battle
{
    RoomCommentManager roomCommentManager;

    void RoomComment::InsertComments(UInt8 forceId,UInt8 mapId,UInt64 playerId,std::string comment)
    {
        SingleComment* Comment = new(std::nothrow) SingleComment(forceId,mapId,playerId,comment);
        if( Comment == NULL )
            return;
        comments.push_back(Comment);
    }

    void RoomComment::DeleteSingleComment(SingleComment* sc)
    {
        for( auto it = comments.begin(); it != comments.end(); )
        {
            if( (*it) == sc )
            {
                it = comments.erase(it);
            }
            else
            {
                ++it;
            }
        }

    }

    SingleComment* RoomComment::GetSingleComment(UInt8 forceId,UInt8 mapId)
    {
        for(auto it = comments.begin(); it != comments.end(); ++it)
        {
            if( (*it)->GetForceId() == forceId && (*it)->GetMapId() == mapId )
            {
                return (*it);
            }
        }
        return NULL;
    }


    void RoomCommentManager::LoadAllComments(UInt32 roomId,UInt8 forceId,UInt8 mapId,UInt64 playerId,std::string comment)
    {
        RoomComment* roomComment = room2comments[roomId];
        if( roomComment == NULL )
        {
            std::vector<SingleComment*> vecSingleComment;
            vecSingleComment.push_back(new(std::nothrow) SingleComment(forceId,mapId,playerId,comment));
            roomComment = new(std::nothrow) RoomComment(roomId);
            roomComment->SetRoomComment(vecSingleComment);
        }
        else
        {
            roomComment->InsertComments(forceId,mapId,playerId,comment);
        }
        room2comments[roomId] = roomComment;
    }

    bool RoomCommentManager::InsertComment(GObject::Player* player,UInt8 mapId,std::string comment)
    {
        if( !player->GetJoinClanBattle())
            return false;
        if( player == NULL )
            return false;
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
            return false;
        UInt32 roomId = clan->GetClanBattleRoomId();
        if( roomId == 0 )
            return false;
        UInt8  forceId = clan->GetBattleForceId();
        if( forceId == 0 )
            return false;
        UInt64 playerId = player->GetId();
        if( comment.size() > 100 )
            return false;
        UInt32 commentTime = player->GetVar(GObject::VAR_CLANBATTLE_COMMENT_TIME);
        UInt32 now = TimeUtil::Now();
        if( commentTime > 0 )
        {
            if( now - commentTime < 3*60 )
            {
                return false;
            }
        }
        RoomComment* roomComment = room2comments[roomId];
        if( roomComment == NULL )
        {
            std::vector<SingleComment*> vecSingleComment;
            vecSingleComment.push_back(new(std::nothrow) SingleComment(forceId,mapId,playerId,comment));
            roomComment = new(std::nothrow) RoomComment(roomId);
            roomComment->SetRoomComment(vecSingleComment);
        }
        else
        {
            SingleComment* sc = roomComment->GetSingleComment(forceId,mapId);
            if( sc != NULL )
            {
                delete sc;
                roomComment->DeleteSingleComment(sc);
            }
            roomComment->InsertComments(forceId,mapId,playerId,comment);
        }
        player->SetVar(GObject::VAR_CLANBATTLE_COMMENT_TIME,now+10);
        room2comments[roomId] = roomComment;
        //更新数据库
        DB7().PushUpdateData("REPLACE INTO `clan_battle_comment`(`roomId`,`forceId`,`mapId`,`playerId`,`message`) value(%u,%u,%u,%"I64_FMT"u,'%s')",roomId,forceId,mapId,playerId,comment.c_str());
        return true;
    }

    void RoomCommentManager:: GetAllisComment(GObject::Player* player,Stream& st) 
    {
        if( !player->GetJoinClanBattle())
            return;
        GObject::Clan* clan = player->GetClan();
        UInt32 roomId = clan->GetClanBattleRoomId();
        RoomComment* roomComment = room2comments[roomId];
        UInt8 forceId = clan->GetBattleForceId();
        if( roomComment == NULL )
        {
            st<<static_cast<UInt8>(0);
        }
        else
        {
            std::vector<SingleComment*> vecSingleComment = roomComment->GetComments();
            size_t offset = st.size();
            UInt8 count = 0;
            st<<static_cast<UInt8>(count);
            for( auto it = vecSingleComment.begin(); it != vecSingleComment.end(); ++it )
            {
               if( (*it)->GetForceId() == forceId )
               {
                   st<<static_cast<UInt8>((*it)->GetMapId());
                   st<<(*it)->GetComment();
                   ++count;
               }
            }
            st.data<UInt8>(offset) = count;
        }

    }

    void RoomCommentManager::GetAllies(GObject::Player* player,std::set<GObject::Player*>& playerSet)
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


    void RoomCommentManager::NoticeOtherAllies(GObject::Player* player,UInt8 mapId,std::string message)
    {
         std::set<GObject::Player*> playerSet;
         GetAllies(player,playerSet);
         if( playerSet.empty())
             return;
         for(auto it = playerSet.begin(); it != playerSet.end(); ++it)
         {
             if( (*it) == player )
                 continue;
             Stream st(REP::CLAN_BATTLE_COMMENTS);
             st<<static_cast<UInt8>(1);
             st<<static_cast<UInt8>(mapId);
             st<<message;
             st<<Stream::eos;
             (*it)->send(st);
         }
    }

}

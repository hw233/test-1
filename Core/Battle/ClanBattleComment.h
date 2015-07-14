#ifndef __CLANBATTLECOMMENT_H__
#define __CLANBATTLECOMMENT_H__


//公会战留言

#include"Config.h"
#include"GObject/Player.h"
#include"GObject/Clan.h"

namespace GObject
{
    class Player;
    class Clan;
    class Var;
}
namespace Battle
{
    //单个城市的留言
    class SingleComment
    {
        public:
            SingleComment(UInt8 f,UInt8 id,UInt64 p,std::string c) : forceId(f) , mapId(id), playerId(p),comment(c) {}
            UInt8 GetMapId() const { return mapId;}
            UInt8 GetForceId() const { return forceId;}
            std::string GetComment() const  { return comment;}
            void SetComment(std::string newComment) { comment = newComment;}
        private:
            UInt8 forceId;
            UInt8 mapId;  //城市Id
            UInt64 playerId;
            std::string comment; //留言
    };

    //一个房间所有城市的留言
    class RoomComment
    {
        public:
            RoomComment(UInt32 id) : roomId(id)
            {
                comments.clear();
            } 
            void SetRoomComment(std::vector<SingleComment*> vecComment)
            {
                comments = vecComment;
            }
            UInt32 GetRoomId() const { return roomId;}
            std::vector<SingleComment*> GetComments() const { return comments;}
            void InsertComments(UInt8 forceId,UInt8 mapId,UInt64 playerId,std::string comment);
            SingleComment* GetSingleComment(UInt8 forceId,UInt8 mapId);
            void DeleteSingleComment(SingleComment* sc);
        private:
            UInt32 roomId;
            std::vector<SingleComment*> comments;
    };

    class RoomCommentManager
    {
        public:
            void LoadAllComments(UInt32 roomId,UInt8 forceId,UInt8 mapId,UInt64 playerId,std::string comment);
            bool InsertComment(GObject::Player* player,UInt8 mapId,std::string comment);
            RoomComment* GetRoomComment(UInt32 roomId) { return room2comments[roomId];}
            void GetAllisComment(GObject::Player* player,Stream& st);
            void NoticeOtherAllies(GObject::Player* player,UInt8 mapId,std::string message);
        private:
            std::map<UInt32,RoomComment*>  room2comments;
    };

    extern RoomCommentManager roomCommentManager;
};
#endif

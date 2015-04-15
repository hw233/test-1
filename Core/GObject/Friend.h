#ifndef FRIEND_H_
#define FRIEND_H_
#include "Player.h"
#include "LoadWorker.h"
#include "Common/URandom.h"

//#define FRIEND_MAX  20            //好友的上限
#define FRIEND_RECOMMAND_MAX 5    //一次推荐好友的上限

namespace GObject
{
    enum eFriendType
    {
        friend_normal = 0 ,
        friend_recommand,   //好友推荐列表
        friend_apply,       //好友申请列表
        friend_max
    };
    class Player;
    class FriendManager
    {
        public:
            FriendManager(Player *pl):m_owner(pl){}
            void AddFriend(UInt8 index , Player* friendOne);
            void GetFriendStream(eFriendType index , Stream &st);
            void GetAllFriendStream(Stream &st);

            bool FindFriendByName(const std::string& name); 
            bool FindFriendById(UInt64 playerId);
            bool DelFriendByName(std::string& name);//删除好友
            void AgreeAddFriend(std::string& name); //同意加好友

            void DelApplyAddFriend(std::string& name);

            void PushInSet( eFriendType type , Player* pl);
            void PopOutSet( eFriendType type , Player* pl);

            void ApplyAddFriend(std::string& name);
            void RecommandFriend();

            void RefuseFriend(std::string& name);
            UInt8 GetFriendNum(eFriendType type);

            bool IsInList(eFriendType type,Player *pl);
            bool HasFriend(Player* pl);
        private:
            std::set<Player *> _friends[friend_max];
            Player* m_owner;
    };
}
#endif // FRIEND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


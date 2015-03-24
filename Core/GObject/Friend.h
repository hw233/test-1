#ifndef FRIEND_H_
#define FRIEND_H_
#include "Player.h"
#include "LoadWorker.h"

#define FRIEND_MAX  20            //每个列表中好友的上限
#define FRIEND_RECOMMAND_MAX 5    //一次推荐好友的上限

namespace GObject
{
    enum eFriendType
    {
        friend_normal = 0 ,
        friend_close,
        friend_clan,
        friend_black, 
        friend_apply,       //好友申请列表
        friend_recommand,   //好友推荐列表
        friend_max
    };
    class Player;
    class FriendManager
    {
        public:
            FriendManager(Player *pl):m_owner(pl){}
            void AddFriend(eFriendType index , Player* friendOne);
            void GetFriendStream(eFriendType index , Stream &st);
            void GetAllFriendStream(Stream &st);

            void FindFriendByName(std::string name); 
            void FindFriendById(UInt64 playerId);
            bool DelFriendByName(eFriendType type,std::string name);//删除好友
            bool DelFriendById(eFriendType type,UInt64 playerId);
            void AgreeAddFriend(eFriendType type,Player * pl); //同意加好友
            void AgreeAddFriendOneKey();

            void DelApplyAddFriend(Player *pl);
            void DelApplyAddFriendOneKey();

            void PushInSet( eFriendType type , Player* pl);
            void PopOutSet( eFriendType type , Player* pl);

            void ApplyAddFriend(Player* pl);
            void RecommandFriend();

            void RefreshRecommandFriend();
            void ApplyFriendOneKey();

            void RefuseAddFriend(Player* pl);   //拒绝加好友
            UInt8 GetFriendNum(eFriendType type);
        private:
            std::set<Player *> _friends[friend_max];
            Player* m_owner;
    };
}
#endif // FRIEND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


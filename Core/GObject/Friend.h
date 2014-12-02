#ifndef FRIEND_H_
#define FRIEND_H_
#include "Player.h"
namespace GObject
{
    enum eFriendType
    {
        friend_normal = 0 ,
        friend_close,
        friend_clan,
        friend_black, 
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
        private:
            std::set<Player *> _friends[friend_max];
            Player* m_owner;
    };
}
#endif // FRIEND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


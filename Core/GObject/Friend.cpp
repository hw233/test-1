#include "Friend.h"
#include "Player.h"
namespace GObject
{
    void FriendManager::AddFriend(eFriendType index ,Player* friendOne)
    {
        if(m_owner == friendOne)
            return ;
        if(index >= friend_max ) 
            return ;

        _friends[index].insert(friendOne);
    }
    void FriendManager::GetFriendStream(eFriendType index ,Stream & st)
    { 
        if(index >= friend_max )
            return ;
        for(std::set<Player *>::iterator it = _friends[index].begin() ; it != _friends[index].end();++it)
        { 
            if(*it)
                (*it)->GetSelfInfoStream(st);
        } 
    } 
    void FriendManager::GetAllFriendStream(Stream & st)
    { 
        for(UInt8 i = 0; i < friend_max; ++i)
            GetFriendStream((eFriendType)i,st);
    } 
}

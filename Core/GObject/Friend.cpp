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
        {
            GetFriendStream((eFriendType)i,st);
        }
    }

    void FriendManager::FindFriendByName(std::string name)
    {
       UInt8 res = 0;
       Player* pl = globalNamedPlayers[name];
       if( pl == NULL )
       {
           res = 1;
           return;
       }
       UInt64 playerId = pl->GetId();
       FindFriendById(playerId);
    }

    void FriendManager::FindFriendById(UInt64 playerId)
    {
        UInt8 res = 0;
        Player* pl = globalPlayers[playerId];
        if( pl == NULL )
        {
            res = 1;   //没有找到该玩家
        }
        Player* p  = globalOnlinePlayers[playerId];
        if( p == NULL )
        {
            res = 2; // 该玩家不在线
        }
        res = 3;
    }
   

    bool FriendManager::DelFriendByName(eFriendType type ,std::string name)
    {
        if( type < 0 || type > 3 )
            return false;

        for(auto it = _friends[type].begin(); it != _friends[type].end();++it)
        {
            if( (*it)->GetName() == name )
            {
                _friends[type].erase(*it);
                return true;
            }
        }
        return false;
    }

    bool FriendManager::DelFriendById(eFriendType type ,UInt64 playerId)
    {
        if( type < 0 || type > 3 || playerId <= 0 )
            return false;
        for(std::set<Player *>::iterator it = _friends[type].begin(); it != _friends[type].end();++it)
        {
            if( (*it)->GetId() == playerId )
            {
                _friends[type].erase(*it);
                return true;
            }
        }
        return false;
    }

    void FriendManager::AgreeAddFriend(eFriendType type,Player * pl)
    {
        if( pl == NULL || type <0 || type > friend_max)
            return;
        //从申请列表中删掉
        PopOutSet(friend_apply,pl);
        //放到好友列列表中
        PushInSet(friend_normal,pl);
   }

   void FriendManager::AgreeAddFriendOneKey()   //一键同意
   {
       for(auto it = _friends[friend_apply].begin(); it != _friends[friend_apply].end(); ++it)
       {
          AgreeAddFriend(friend_apply,*it);
       }
   }

   void FriendManager::DelApplyAddFriend(Player *pl)
   {
       if( pl == NULL)
            return;
       PopOutSet(friend_apply,pl);
   }

   void FriendManager::DelApplyAddFriendOneKey()
   {
       for(auto it = _friends[friend_apply].begin(); it != _friends[friend_apply].end(); ++it)
       {
           DelApplyAddFriend(*it);
       }
   }


   void FriendManager::PushInSet( eFriendType type , Player* pl)
   {
       if( pl == NULL || type < 0 || type > friend_max )
           return;
       if( _friends[type].size() > FRIEND_MAX )
       {
           std::cout<<"已达上限不能再添加了"<<std::endl;
           return;
       }
        _friends[type].insert(pl);
   }

   void FriendManager::PopOutSet( eFriendType type , Player* pl)
   {
       if( pl == NULL || type < 0 || type > friend_max )
           return;
       auto it = _friends[type].find(pl);
       if( it == _friends[type].end() )
       {
           std::cout<<"未找到不能删除"<<std::endl;
       }
       _friends[type].erase(pl);
   }

   void FriendManager::ApplyAddFriend(Player* pl)   //申请加好友
   {
       if( pl == NULL )
           return;
       pl->GetFriendManager()->PushInSet(friend_apply,m_owner);

   }

}

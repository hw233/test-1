#include "Friend.h"
#include "Config.h"
#include <algorithm>
#include "MsgID.h"

namespace GObject
{
    void FriendManager::AddFriend(UInt8 index ,Player* friendOne)
    {
        if(m_owner == friendOne)
            return ;
        if(index >= friend_max ) 
            return ;
        eFriendType type = (eFriendType)index;
        _friends[type].insert(friendOne);
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
        UInt8 totalNum = 0;
        for(UInt8 i = 0; i < friend_max ; ++i)
        {
            st << static_cast<UInt8>( _friends[i].size());
            totalNum += _friends[i].size();

        }
        st<<static_cast<UInt8>(totalNum);
        for(UInt8 i = 0; i < friend_max; ++i)
        {
            GetFriendStream((eFriendType)i,st);
        }
    }
    bool FriendManager::FindFriendByName(const std::string& name)
    {
       Player* pl = globalNamedPlayers[name];
       if( pl == NULL )
       {
           return false;
       }
       return true;

    }

    bool FriendManager::DelFriendByName(std::string& name)
    {
        for(auto it = _friends[friend_normal].begin(); it != _friends[friend_normal].end();++it)
        {
            if( (*it)->GetName() == name )
            {
                PopOutSet(friend_normal,*it);
                return true;
            }
        }
        return false;
    }
    
    void FriendManager::AgreeAddFriend(std::string& name)
    {
        Player* pl = globalNamedPlayers[name];
        if( pl == NULL )
            return;
        if( GetFriendNum(friend_normal) >= FRIEND_MAX || pl->GetFriendManager()->GetFriendNum(friend_normal) >= FRIEND_MAX )
        {
            std::cout<<"其中之一列表已满 不可添加"<<endl;
            return;
        }
        //从申请列表中删掉
        PopOutSet(friend_apply,pl);
        //放到好友列表中
        PushInSet(friend_normal,pl);
        //gei pl fa xiao xin shuo m_owner yi tong yi
        pl->GetFriendManager()->PushInSet(friend_normal,m_owner);
   }

   void FriendManager::DelApplyAddFriend(std::string& name)
   {
       Player* pl = globalNamedPlayers[name];
       if( pl == NULL)
            return;
       PopOutSet(friend_apply,pl);
   }

   void FriendManager::PushInSet( eFriendType type , Player* pl)
   {
       if( pl == NULL || type < 0 || type > friend_max )
           return;
       if( pl == m_owner)
       {
           std::cout<<"不能添加自己"<<std::endl;
           return;
       }
       if( _friends[type].size() > FRIEND_MAX )
       {
           std::cout<<"已达上限不能再添加了"<<std::endl;
           return;
       }
       auto it = _friends[type].find(pl);
       if( it != _friends[type].end() )
       {
           std::cout<<"已在列表中拉"<<std::endl;
           return ;
       }
        _friends[type].insert(pl);
        //更新数据库
	    DB7().PushUpdateData("insert into friends (type,playerId,friendId) values(%u, %" I64_FMT "u, %"I64_FMT"u )", type, m_owner->getId(),pl->getId());
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
       //更新数据库
	   DB7().PushUpdateData("delete from friends where type = %u AND (playerId = %"I64_FMT"u AND friendId = %"I64_FMT"u)", type, m_owner->getId(),pl->getId());
   }

   void FriendManager::ApplyAddFriend(std::string& name)   //申请加好友
   {
       Player* pl = globalNamedPlayers[name];
       if( pl == NULL )
           return;
       pl->GetFriendManager()->PushInSet(friend_apply,m_owner);
       //发申请者信息给被申请者
       Stream st(REQ::FRIEND_APPLY);
       m_owner->GetSelfInfoStream(st);
       st<<Stream::eos;
       pl->send(st);

   }

   bool MyGreater(Player* p1, Player* p2)
   {
       if((p1->isOnline()+p1->getVipLevel()) > (p2->isOnline()+p2->getVipLevel()))
       {
           return true;
       }
       return false;
   }
   
   //好友推荐
   void FriendManager::RecommandFriend()
   {
       _friends[friend_recommand].clear();
       UInt8 num = GetFriendNum(friend_normal);
       if(num >= FRIEND_MAX)
       {
           cout<<"玩家的好友列表已满 不能推荐了"<<endl;
           return;
       }
       //先对这个set进行排序 vip等级 在线与否 为权值
       std::sort(globalPlayerVec.begin(),globalPlayerVec.end(),MyGreater);

       for( auto it = globalPlayerVec.begin(); it != globalPlayerVec.end();++it)
       {
           if( GetFriendNum(friend_recommand) >= FRIEND_RECOMMAND_MAX )
               break;
           if( (*it) == m_owner )
               continue;
           PushInSet(friend_recommand,*it);
       }
       
       Stream st(REQ::FRIEND_RECOMMAND);
       st<<static_cast<UInt8>(_friends[friend_recommand].size());
       for( auto it = _friends[friend_recommand].begin(); it!=_friends[friend_recommand].end();++it)
       {
           (*it)->GetSelfInfoStream(st);

       }
       st<<Stream::eos;
       m_owner->send(st);
   }
   

   void FriendManager::RefuseFriend(std::string& name)
   {
       DelApplyAddFriend(name);
   }

   UInt8 FriendManager::GetFriendNum( eFriendType type)
   {
       return _friends[type].size();
   }

}

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
    
    /*
    void FriendManager::SendFriendList(UInt8 type,UInt8 index)
    { 
        eFriendType ftype = (eFriendType)type;
        if(ftype >= friend_max || index < 0 || index > _friends[ftype].size())
        {
            return;
        }
        if( _friends[ftype].size() == 0 )
        {
            return;
        }
        Stream st(REP::FRIEND_LIST);
        UInt8 totalNum = _friends[ftype].size();
        UInt8 actSendNum = FRIEND_SENDONCE_MAX;  //实际每次发送好友信息的个数
        if( totalNum -index  < FRIEND_SENDONCE_MAX )
        {
            actSendNum = totalNum-index ;
        }
        st<<static_cast<UInt8>(actSendNum);

        //擦 就为了用下标操作
        std::vector<Player*> vecPlayer;
        for(auto it = _friends[ftype].begin() ; it != _friends[ftype].end();++it)
        {
            vecPlayer.push_back(*it);
        }
        for(UInt8 i = index; i < index+actSendNum ; ++i)
        {
            (vecPlayer[i])->GetSelfInfoStream(st);
        }
        st<<Stream::eos;
        m_owner->send(st);
    }
    */

    void FriendManager::SendFriendList()
    {
         UInt16 totalNum = 0;
         for( UInt8 i = friend_normal; i < friend_max ; ++i)
         {
             totalNum += _friends[i].size();
         }
         if( totalNum == 0 )
         {
             return ;
         }
         Stream st(REP::FRIEND_LIST);
         for(UInt8 index = friend_normal; index < friend_max; ++index)
         {
             GetFriendListStream((eFriendType)index,st);
         }
         st<<Stream::eos;
         m_owner->send(st);
    }


    void FriendManager::GetFriendListStream(eFriendType type, Stream &st)
    {
        if( type < 0 || type >= friend_max )
        {
            return;
        }
        if( type == friend_normal )
        {
            st<<static_cast<UInt8>(m_owner->GetFriendMax());
        }
        st<<static_cast<UInt8>(_friends[type].size());
        for( auto it = _friends[type].begin(); it != _friends[type].end(); ++it )
        {
            (*it)->GetSelfInfoStream(st);
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
        bool res = false;
        for(auto it = _friends[friend_normal].begin(); it != _friends[friend_normal].end();++it)
        {
            if( (*it)->GetName() == name )
            {
                PopOutSet(friend_normal,*it);
                res = true;
            }
        }
        return res;
    }

    void FriendManager::AgreeAddFriend(std::string& name)
    {
        Player* pl = globalNamedPlayers[name];
        if( pl == NULL )
            return;
        if( GetFriendNum(friend_normal) >= m_owner->GetFriendMax() || pl->GetFriendManager()->GetFriendNum(friend_normal) >= pl->GetFriendMax() )
        {
            std::cout<<"其中之一列表已满 不可添加"<<endl;
            return;
        }
        //从申请列表中删掉
        PopOutSet(friend_apply,pl);
        //放到好友列表中
        bool isIn = IsInList(friend_normal,pl);
        if( !isIn )
        {
            PushInSet(friend_normal,pl);
        }

        pl->GetFriendManager()->PushInSet(friend_normal,m_owner);

        Stream st(REP::FRIEND_ADD);
        m_owner->GetSelfInfoStream(st);
        st<<Stream::eos;
        pl->send(st);
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
        if( type == friend_normal && _friends[type].size() > m_owner->GetFriendMax() )
        {
            std::cout<<"好友列表已满"<<std::endl;
            return;
        }
        bool isIn = IsInList(type,pl);
        if(isIn)
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
        bool isIn = IsInList(type,pl);
        if( !isIn )
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
        if( pl->GetFriendManager()->IsInList(friend_apply,m_owner))
        {
            std::cout<<"被申请者表里已经有这个申请者了"<<std::endl;
            return;
        }

        bool isIn = IsInList(friend_normal,pl);
        bool isInlist  = pl->GetFriendManager()->IsInList(friend_normal,m_owner);
        if( isIn & isInlist )
        {
            std::cout<<"已经是你的好友了"<<std::endl;
            return;
        }
        pl->GetFriendManager()->PushInSet(friend_apply,m_owner);
        //发申请者信息给被申请者
        Stream st(REP::FRIEND_APPLY);
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
        DB7().PushUpdateData("delete from friends where type = %u",friend_recommand);
        _friends[friend_recommand].clear();
        UInt8 num = GetFriendNum(friend_normal);
        if(num >= m_owner->GetFriendMax())
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
            if(IsInList(friend_normal,*it))
                continue;
            if((*it)->GetFriendManager()->IsInList(friend_normal,m_owner) || (*it)->GetFriendManager()->IsInList(friend_apply,m_owner))
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

    bool FriendManager::IsInList(eFriendType type,Player *pl)
    {
        if( pl == NULL || type < friend_normal || type >= friend_max)
        {
            return false;
        }
        auto it = _friends[type].find(pl);
        if( it != _friends[type].end() )
        {
            return true;
        }
        return false;
    }
    bool FriendManager::HasFriend(Player* pl)
    {
        std::set<Player *>::iterator it = _friends[friend_normal].find(pl);
        if(it != _friends[friend_normal].end())
            return true;
        return false;
    }
    
    /*
    void FriendManager::SendFriendBaseInfo()
    {
        Stream st(REP::FRIEND_BASEINFO);
        st<<static_cast<UInt8>(_friends[friend_normal].size());
        st<<static_cast<UInt8>(m_owner->GetFriendMax());
        st<<static_cast<UInt8>(_friends[friend_apply].size());
        st<<Stream::eos;
        m_owner->send(st);

    }
    */
}

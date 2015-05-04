#include "Player.h"

#include "Fighter.h"
#include "ChatHold.h"
#include "Mail.h"
#include "Common/StringTokenizer.h"
#include "MsgID.h"
#include "Server/OidGenerator.h"
#include "Script/GameActionLua.h"
#include "Country.h"

#define P_CHAT_MAX 10
namespace GObject
{
    //GlobalNamedPlayers globalPlayers;
    GlobalPlayers globalPlayers;
    GlobalPlayers globalOnlinePlayers;
    GlobalNamedPlayers globalNamedPlayers;
    GlobalPlayerVec globalPlayerVec;
    GlobalClans globalClan;
    GlobalOnlinePlayerSet globalOnlinePlayerSet;
    GlobalNamedClans globalNamedClans;

    //GlobalNamedPlayers globalAccountsPlayers;
    Player::Player( IDTYPE id ): GObjectBaseT<Player, IDTYPE>(id),_isOnline(false),_session(-1),_friendMax(10)
    {
        m_pVars = new VarSystem(id);
        m_Package = new Package(this); 
        m_friendMgr = new FriendManager(this);
        chatHold = NULL;
        clan = NULL;
        m_governManager = new GovernManager(this);
    }

    bool enum_send_chat(Player *pl , void * ptr)
    { 
        Stream * st = reinterpret_cast<Stream * >(ptr);
        if(pl->GetRecChat(e_chat_world))
            return true;
        pl->send(*st);
        return true;
    } 

    Player::~Player()
    { 

    } 
    
    UInt32 Player::GetVar(UInt32 id)
    {
        return m_pVars->GetVar(id);
    }

    Int32 Player::GetVarS(Int32 id)
    {
        return (Int32)m_pVars->GetVar(id);
    }

    void Player::LoadVar(UInt32 id, UInt32 val, UInt32 overTime)
    {
        m_pVars->LoadVar(id, val, overTime);
    }

    void Player::SetVar(UInt32 id, UInt32 val,UInt8 flag)
    {
        m_pVars->SetVar(id,val,flag);
    }

    void Player::DelVar(UInt32 id )
    {
        m_pVars->DelVar(id);
    }

    void Player::AddVar(UInt32 id, UInt32 val)
    {
        m_pVars->AddVar(id,val);
    }

    void Player::AddVarS(UInt32 id, Int32 val)
    {
        Int32 v = GetVarS(id);
        m_pVars->SetVar(id,v+val);
    }

    UInt32 Player::GetVarNow(UInt32 id,  UInt32 now)
    {
        return m_pVars->GetVar(id , now);
    }

    void Player::SetVarNow(UInt32 id, UInt32 val, UInt32 now)
    {
        m_pVars->SetVar(id,val, now);
    }

    void Player::AddVarNow(UInt32 id, UInt32 val, UInt32 now)
    {
        m_pVars->AddVar(id,val, now);
    }

    void Player::SetVarOffset(UInt32 offset)
    {
        m_pVars->SetOffset(offset);
    }
    
    void Player::GetSelfInfoStream(Stream &st)
    {
        
        st<<static_cast<UInt8>(GetSex());
        st<<GetName();
        st<<static_cast<UInt8>(GetLevel());
        if( isOnline() )  //如果在线 
        {
            st<<static_cast<UInt32>(0);
        }
        else     //不在线
        {
            st<<static_cast<UInt32>(GetVar(VAR_OFF_LINE));
        }
        //st << GetVar(VAR_GOLD);
    } 
    
    void Player::send(const void * buf ,int size)
    { 
        if(!_isOnline) return;
        TcpConnection conn = NETWORK()->GetConn(_session);
        if(conn.get() == NULL)
        {
            return;
        }
        conn->send(buf, size);
    } 

    UInt32 Player::getClientAddress()
    {
        TcpConnection conn = NETWORK()->GetConn(this->GetSessionID());
        if(conn.get() == NULL)
            return 0;
        Network::GameClient * client = static_cast<Network::GameClient *>(conn.get());
        return client->GetClientIP();
    }

    void Player::setOpenId(const std::string& openid, bool load /* = false */)
    {
        if (!strncmp(m_openid, openid.c_str(), 256))
            return;
        strncpy(m_openid, openid.c_str(), 256); 
        m_openid[255] = '\0'; 
        if (!load)
        {     
            //DB1().PushUpdateData("UPDATE `player` SET `openid` = '%s' WHERE `id` = '%s' and `accounts` = '%s'"  , m_openid, getId().c_str(),getAccounts().c_str());
            DB1().PushUpdateData("UPDATE `player` SET `openid` = '%s' WHERE `id` = %" I64_FMT "u "  , m_openid, getId());   //LIBOUInt64
            //DB1().PushUpdateData("UPDATE `player` SET `openid` = '%s' WHERE `id` = %u "  , m_openid, getId());   //LIBOUInt32
        }     
    }

    void Player::addFighter( Fighter * fgt, bool writedb, bool load )
    { 
        //TODO
        UInt32 id = fgt->getId();
        UInt32 now = TimeUtil::Now();
        if(id < 10) 
            _fighters.insert(_fighters.begin(), std::make_pair(fgt->getId(), fgt));
        else  
            _fighters[fgt->getId()] = fgt;
        if(writedb)
            DB2().PushUpdateData("INSERT INTO `fighter` (`playerId`,`fighterId`,`experience`,`addTime`) VALUES( %u,%u,0,%u)",getId(),id,now);
    } 

    void Player::addFighter(UInt16 fgtId, bool writedb, bool load )
    { 
        Fighter * fgt = globalFighters[fgtId];
        if(fgt == NULL)
            return ;
        Fighter * fgt2 = fgt->Clone(this);
        if(fgt2 == NULL)
            return ;
        fgt2->SetExp(0);
        addFighter(fgt2, writedb, load);
    } 


    void Player::Login()
    { 
        //TODO
        Mail* mail = new Mail(IDGenerator::gMailOidGenerator.ID(),this,1,"1,1",0,static_cast<UInt32>(-1));
        if(mail)
        { 
            globalMails.add(mail->GetId(), mail);
            AddMail(mail->GetId());
        }
        if(1)
        {
           Stream st(REP::CLAN_LIST);
           SendClanListinfo(st);
           st << Stream::eos;
           send(st);
        }
        if(1 && GetClan())
        {
            Stream st(REP::CLAN_INFO);
            GetClan()->GetClanInfo2(st);
            st << Stream::eos;
            send(st);
        }
    } 
    void Player::PutFighters( Battle::BattleGround& bsim, int side, bool fullhp ,UInt16 fighterId)
    { 
        //TODO
    } 

    Fighter * Player::findFighter(UInt32 fighterId)
    { 
        std::map<UInt32, Fighter *>::iterator it = _fighters.find(fighterId);
        if(it == _fighters.end())
            return NULL;
        return it->second;
    } 

    Fighter * Player::getMainFighter()
    {
        if(_fighters.empty())
            return NULL;
        Fighter * fgt = _fighters.begin()->second;
        if(fgt->getId() > 9)
            return NULL;
        return fgt;
    }

    void Player::makePlayerInfo(Stream& st)
    {
        st << GetName() ; 
        st << GetVar(VAR_EXP);
        st << GetVar(VAR_TEAL);
        st << GetVar(VAR_GOLD);
        st << GetVar(VAR_TOTAL_GOLD);
        UInt8 buttonStatu = 0;

        if(_mailList.size())
            buttonStatu |= 1;

        st << buttonStatu ;
        std::map<UInt32, Fighter *>::iterator it = _fighters.begin();
        st << static_cast<UInt8>(_fighters.size());
        for(;it != _fighters.end();++it)
        { 
            (it->second)->MakeFighterInfo(st);
        } 
        
        std::cout << "获得玩家信息：" << GetName() << std::endl;

    }

    ChatHold* Player::GetChatHold()
    { 
        if(!chatHold)
        { 
            chatHold = new ChatHold(P_CHAT_MAX);
        } 
        return chatHold;
    } 

    void Player::ChatForWorld(std::string text)
    { 
        // 聊天协议添加
        //NETWORK()->Broadcast(st);
        Stream st(REP::CHAT);
        st << static_cast<UInt8>(0);
        st << GetName();
        st << text;
        globalPlayers.enumerate(enum_send_chat,&st);
        //WORLD()->GetChatHold()->InsertChat(this, text);
    } 

    void Player::ChatForClan(std::string text)
    { 
        if(!GetClan())
            return ;

        Stream st(REP::CHAT);
        st << static_cast<UInt8>(e_chat_world);
        st << GetName();
        st << text;
        st << Stream::eos;
        GetClan()->Boradcast(st,0);
        GetClan()->GetChatHold()->InsertChat(this,text);
    } 
    void Player::ChatForFriend(IDTYPE playerId, std::string text)
    { 
        Player* pl = globalPlayers[playerId];
        if(!pl)
            return ;
        if(!GetFriendManager()->HasFriend(pl))
            return ;
        if(pl->GetRecChat(e_chat_friend))
            return ;

        pl->GetChatHold()->InsertChat(this, text);
        Stream st(REP::CHAT);
        st << static_cast<UInt8>(e_chat_friend);
        st << GetName();
        st << text;
        st << Stream::eos;
        pl->send(st);
    } 

    void Player::AddMail(UInt32 id, UInt8 update)
    { 
        Mail* mail = globalMails[id];
        if(!mail || mail->GetOwner()!= this)
            return ;
        _mailList.push_front(id);
        if(update)
        { 
            //DB2().PushUpdateData("delete from var where `playerId` = %" I64_FMT "u  and `id` = %u ",m_PlayerID, id);
            DB2().PushUpdateData("REPLACE INTO `mail`(`id`,`playerId`,`contextId`,`items`,`option`,`overTime`) VALUES( %u, %" I64_FMT "u, %u, '%s',%u,%u)", mail->GetId(), getId(), mail->GetContextId(), mail->GetItems().c_str(),mail->GetOption(),mail->GetOverTime());

            Stream st(REP::MAIL_NOTICE);
            st << static_cast<UInt32>(mail->GetId());
            st << static_cast<UInt16>(mail->GetContextId());
            std::string items = mail->GetItems();
            StringTokenizer token(items,",");
            for(UInt8 i = 0; i < token.count()/2;++i)
            { 
                st << static_cast<UInt32>(::atoi(token[2*i].c_str()));
                st << static_cast<UInt16>(::atoi(token[2*i+1].c_str()));
            } 
            st << static_cast<UInt32>(mail->GetOverTime());
            st << Stream::eos;
            send(st);
        } 
    } 
    
    UInt8 Player::ReciveMail(UInt32 id, UInt8 flag)
    { 
        Mail* mail = globalMails[id];
        if(!mail || mail->GetOwner()!= this)
            return 1;

        std::string items = mail->GetItems();
        StringTokenizer st(items,",");
        if(200 + GetVar(VAR_PACKAGE_SIZE) <  st.count()/2 + GetPackage()->GetPackageSize())
            return 2;
        for(UInt8 i = 0; i < st.count()/2; ++i)
        { 
            GetPackage()->AddItem(::atoi(st[i*2].c_str()), ::atoi(st[i*2+1].c_str()));
        } 

        globalMails.remove(mail->GetId());
        if(!flag)
            _mailList.remove(mail->GetId());
        delete mail;

        DB3().PushUpdateData("DELETE from `mail` WHERE `id` = '%u' ", id);

        //
        //mail->SetOption(1);

        //DB3().PushUpdateData("update `mail` set `option` = 1 WHERE `id` = '%u' ", id);

        //globalMails.remove(mail->GetId());
        //_mailList.remove(mail->GetId());
        //
        //delete mail;
        return 0;
    } 

    UInt8 Player::DeleteMail(UInt32 id,UInt8 flag)
    { 
        Mail* mail = globalMails[id];
        if(!mail || mail->GetOwner()!= this)
            return 1;
        
        globalMails.remove(mail->GetId());
        if(!flag)
            _mailList.remove(mail->GetId());
        delete mail;
        DB3().PushUpdateData("DELETE from `mail` WHERE `id` = '%u' ", id);
        return 0;
    } 

    UInt8 Player::ReciveMail()
    { 
        std::list<UInt32>::iterator it = _mailList.begin();
        UInt8 res = 1;
        for(;it!=_mailList.end();)
        {
            res = ReciveMail(*it,1);
            if(!res)
            {
                it = _mailList.erase(it);
            }
            else
                break;
        }
        Stream st(REP::MAIL_GET_ALL);
        st << static_cast<UInt8>(res);
        st << Stream::eos;
        send(st);
        return 0;
    } 

    UInt8 Player::DeleteMail()
    { 
        std::list<UInt32>::iterator it = _mailList.begin();
        for(;it!=_mailList.end();++it)
        {
            DeleteMail(*it,1);
        }
        _mailList.clear();
        Stream st(REP::MAIL_DELETE_ALL);
        st << static_cast<UInt8>(0);
        st << Stream::eos;
        send(st);
        return 0;
    } 

    UInt8 Player::ListMail(Stream& st, UInt16 index)
    { 
        std::list<UInt32>::iterator it = _mailList.begin();
        if(index >= _mailList.size())
            return 1;
        size_t offset = st.size();
        UInt16 count = 0;
        st << static_cast<UInt16>(count);//(_mailList.size());
        std::advance(it,index);
        for(;it!=_mailList.end();++it)
        { 
            Mail* mail = globalMails[*it];
            if(!mail || mail->GetOwner() != this)
                continue ;
            st << static_cast<UInt32>(mail->GetId());
            st << static_cast<UInt16>(mail->GetContextId());
            std::string items = mail->GetItems();
            StringTokenizer token(items,",");
            if(token.count() < 2)
            { 
                st << static_cast<UInt32>(0);
                st << static_cast<UInt16>(0);
            } 
            else
            {
                for(UInt8 i = 0; i < token.count()/2;++i)
                { 
                    st << static_cast<UInt32>(::atoi(token[2*i].c_str()));
                    st << static_cast<UInt16>(::atoi(token[2*i+1].c_str()));
                } 
            }
            st << static_cast<UInt32>(mail->GetOverTime());
            ++count;
            if(count >= MAIL_LIST_MAX)
                break;
        } 
        st.data<UInt16>(offset) = count;
        return 0;
    } 

    bool GetClanListInfo(Clan* cl, Stream* st)
    { 
        if(!cl)
            return true;
        cl->GetClanInfo(*st);
        return true;
    } 

    void Player::SendClanListinfo(Stream &st)
    { 
        //Stream st(StreamHand);
        //UInt8 count = 0;
        //size_t offect = st.size();
        st << static_cast<UInt16>(globalClan.size());
        globalClan.enumerate(GetClanListInfo, &st);
        //GlobalClans::iterator it = globalClan.begin();
        //for(;it != globalClan.end(); ++it)
        //{
        //    Clan* clan = *(it->second);
        //    if(!clan)
        //        continue;
        //    clan->GetClanInfo(st);
        //    ++count;
        //}
        //st.data<UInt16>(offect) = count;
    } 

    UInt8 Player::CreateClan(std::string name, UInt8 picIndex/*, std::string announcement*/)
    { 
        static UInt32 ClanCreateMoney = 0;
        if(GetClan())
            return 1;
        UInt32 money = GetVar(VAR_TEAL);
        if(money < ClanCreateMoney)
            return 2;
        Clan* clan = new Clan(IDGenerator::gPlayerOidGenerator.ID(), name, this);
        clan->LoadClanInfo(this,"","",50);
        clan->SetPicIndex(picIndex);
        clan->SetLevel(1);
        globalClan.add(clan->GetId(),clan);
        if(!clan)
            return 3;
        SetClanPos(1);
        clan->LoadPlayer(this,1);
        DB2().PushUpdateData("INSERT INTO `clan` VALUES( %u,'%s',%u,'%s','%s',%" I64_FMT "u,%" I64_FMT "u,%u,0,%u)",clan->GetId(),clan->GetName().c_str(),picIndex,clan->GetAnnouncement().c_str(), clan->GetAnnouncement2().c_str(), getId(),getId(),1,0,clan->GetPersonMax());
        return 0;
    } 

    void Player::SearchFighter(UInt8 count)
    { 
        Stream st(REP::FIND_FIGHTER);
        UInt8 num = 0;
        st << static_cast<UInt8>(0);
        size_t offect = st.size();
        st << num;
        for(UInt8 i = 0; i < count ; ++i)
        { 
            UInt16 fighterId = GameAction()->GetRandFighter();
            st << fighterId;
            
            Fighter * fgt = findFighter(fighterId);
            if(!fgt)
            { 
                GetPackage()->AddItem(fighterId,10);
            }
            else
            {
                addFighter(fighterId, true, true);
            }
            ++num;
        } 
        st.data<UInt8>(offect) = num;
        st << Stream::eos;
        send(st);
    } 

    void Player::VisitFighter(UInt16 fighterId,UInt8 count)
    { 
        UInt32 resultCount = 0;
        for(UInt8 i = 0; i < count; ++i)
        { 
            resultCount += uRand(10);
        } 
        GetPackage()->AddItem(fighterId,resultCount);
        Stream st(REP::FIND_FIGHTER);
        st << static_cast<UInt8>(1);
        st << static_cast<UInt8>(resultCount);
        st << Stream::eos;
        send(st);
    } 
}

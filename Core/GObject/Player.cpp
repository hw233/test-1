#include "Player.h"

#include "Fighter.h"
#include "ChatHold.h"
#include "Mail.h"
#include "Common/StringTokenizer.h"
#include "MsgID.h"
#include "Server/OidGenerator.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "GData/BattleAward.h"
#include "Battle/ClanBattleRoom.h"
#include "FVar.h"

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
    Player::Player( IDTYPE id ): GObjectBaseT<Player, IDTYPE>(id),_isOnline(false),_session(-1),_friendMax(10),clanBattleKillCount(0)
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
        if(pl->isOnline())
            pl->send(*st);
        return true;
    } 

    Player::~Player()
    { 

    } 
    
    UInt32 Player::GetVar(UInt32 id)
    {
        if(this == NULL)
            return 0;
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
            DB2().PushUpdateData("INSERT INTO `fighter` (`playerId`,`fighterId`,`experience`,`addTime`) VALUES( %" I64_FMT "u,%u,0,%u)",getId(),id,now);
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
        if(!GetClan())
        {
           Stream st(REP::CLAN_LIST);
           SendClanListinfo(st);
           st << Stream::eos;
           send(st);
        }
        if(1 && GetClan())
        {
            Stream st(REP::CLAN_INFO);
            st << GetClan()->IsTheFounder(this);
            GetClan()->GetClanInfo2(st);
            st << Stream::eos;
            send(st);
        }
        if(0 && GetRecChat(e_chat_world))
        { 
            Stream st(REP::CHAT);
            WORLD().GetChatHold()->GetStream(st,10);
            st << Stream::eos;
            send(st);
        } 

        if(0 && GetRecChat(e_chat_friend))
        { 
            Stream st(REP::CHAT);
            GetChatHold()->GetStream(st,10);
            st << Stream::eos;
            send(st);
        } 
        if(GetRecChat(e_chat_clan))
        { 
            Stream st(REP::CHAT);
            GetClan()->GetChatHold()->GetStream(st,10);
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
        st << static_cast<UInt8>(GetJoinClanBattle());
        st << static_cast<UInt32>(TimeUtil::Now());
        std::cout << "获得玩家信息：" << GetName() << std::endl;
    }

    ChatHold* Player::GetChatHold()
    { 
        if(!chatHold)
        { 
            chatHold = new ChatHold(1,P_CHAT_MAX);
        } 
        return chatHold;
    } 

    void Player::ChatForWorld(std::string text)
    { 
        // 聊天协议添加
        //NETWORK()->Broadcast(st);
        Stream st(REP::CHAT);
        //st << static_cast<UInt8>(0);
        //st << GetName();
        //st << text;
        WORLD().GetChatHold()->InsertChat(this,text);
        WORLD().GetChatHold()->GetStream(st,1);
        st << Stream::eos;
        globalPlayers.enumerate(enum_send_chat,&st);
        //WORLD()->GetChatHold()->InsertChat(this, text);
    } 

    void Player::ChatForClan(std::string text)
    { 
        if(!GetClan())
            return ;

        GetClan()->GetChatHold()->InsertChat(this,text);
        Stream st(REP::CHAT);
        GetClan()->GetChatHold()->GetStream(st,1);
        //st << static_cast<UInt8>(e_chat_world);
        //st << GetName();
        //st << text;
        st << Stream::eos;
        GetClan()->Boradcast(st,0);
    } 
    void Player::ChatForFriend(std::string name, std::string text)
    { 
        Player* pl = globalNamedPlayers[name];
        if(!pl)
            return ;
        if(!GetFriendManager()->HasFriend(pl))
            return ;
        if(pl->GetRecChat(e_chat_friend))
            return ;

        pl->GetChatHold()->InsertChat(this, text);
        Stream st(REP::CHAT);
        //st << static_cast<UInt8>(e_chat_friend);
        //st << GetName();
        //st << text;
        pl->GetChatHold()->GetStream(st,1);
        st << Stream::eos;
        pl->send(st);
        send(st);
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
        Clan::GetBossClan();

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
        Clan* clan = new Clan(IDGenerator::gClanOidGenerator.ID(), name, this);
        if(!clan)
            return 3;
        clan->LoadClanInfo(this,"","",50);
        clan->SetPicIndex(picIndex);
        clan->SetLevel(1);
        clan->SetClanBattleRoomId(0);
        clan->SetBattleForceId(0);
        clan->SetClanFame(0);
        clan->SetConquests(0);
        globalClan.add(clan->GetId(),clan);
        globalNamedClans.add(clan->GetName(), clan);
        SetClanPos(1);
        clan->LoadPlayer(this,1);
        DB2().PushUpdateData("INSERT INTO `clan`(`clanId`,`name`,`picIndex`,`announcement`,`announcement2`,`creater`,`leader`,`level`,`contribute`,`personMax`,`battleRoomId`,`clanFame`,`conquests`,`forceId`) VALUES( %u,'%s',%u,'%s','%s',%" I64_FMT "u,%" I64_FMT "u,%u,0,%u,%u,%u,%u,%u)",clan->GetId(),clan->GetName().c_str(),picIndex,clan->GetAnnouncement().c_str(), clan->GetAnnouncement2().c_str(), getId(),getId(),1,0,clan->GetPersonMax(),0,0,0);

        //Stream st(REP::CLAN_OPTION);
        //st << static_cast<UInt8>(0x02);
        //clan->GetClanInfo(st);
        //st << Stream::eos;
        //send(st);

        return 0;
    } 

    void Player::SearchFighter(UInt8 count)
    { 
        Stream st(REP::FIND_FIGHTER);
        UInt8 num = 0;

        UInt32 free = GetVar(VAR_SEARCH_FIGHTER);

        if(free < 1)
        {
            AddVar(VAR_SEARCH_FIGHTER, 1);
        }
        else
        {
            if(UseGold(count * 10)) 
                return ;
        }

        st << static_cast<UInt8>(0);
        size_t offect = st.size();
        st << num;
        for(UInt8 i = 0; i < count ; ++i)
        { 
            UInt16 fighterId = GameAction()->GetRandFighter();
            
            Fighter * fgt = findFighter(fighterId);
            if(fgt)
            { 
                GetPackage()->AddItem(fighterId + 40000,10);
            }
            else
            {
                addFighter(fighterId, true, true);
            }
            st << fighterId;
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


    void Player::AddGold(UInt32 num)
    {
        AddVar(VAR_GOLD,num);
    }


    UInt8 Player::UseGold(UInt32 num)
    {
        if( GetVar(VAR_GOLD) < num )
            return 1;
        SetVar(VAR_GOLD,GetVar(VAR_GOLD)-num);
        return 0;
    }

    void Player::AddMoney(UInt8 type,UInt32 num)
    {
        if( type <= 0 || type > 3)
            return;
        AddVar(type ,num);
    }
    
    UInt8 Player::GetFreeSearch()
    { 
        const UInt32 freeMax = 1;
        UInt32 Count = GetVar(VAR_SEARCH_FIGHTER);
        if(freeMax > Count )
            return freeMax - Count;
        return 0;
    }

    //推图奖励
    void Player::GiveBattleAward(UInt8 mapId)
    {
        GData::BattleAward* award = GData::battleAwardTable.GetBattleAward(mapId);
        if( award == NULL )
        {
            return;
        }

        //判断背包情况
        UInt8 status = 0;
        //发奖励
        UInt32 moneyNum = award->GetMoney();
        AddMoney(1,moneyNum);
        std::vector<GData::ItemInfo> vecItem = award->GetItems();
        for( auto it = vecItem.begin() ; it != vecItem.end() ; ++it )
        {
            GetPackage()->AddItem((*it).itemId,(*it).itemNum);
        }
        UInt32 exp = award->GetExp();
        SetVar(VAR_EXP,GetVar(VAR_EXP)+exp);

        //
        Stream st(REP::BATTLE_AWARD);
        st<<static_cast<UInt8>(status);
        st<<Stream::eos;
        send(st);
    }
    /*
    UInt8 Player::OpenClanBattle()
    {
        Clan* clan = GetClan();
        if( clan == NULL )
        {
            std::cout<<"sorry you have no clan"<<std::endl;
            return 0;
        }
        UInt8 pos = GetClanPos();  //只有会长和副会长才能开启军团战
        if( pos < 2 )
        {
            std::cout<<"sorry have no root"<<std::endl;
            return 1;
        }
        UInt8 status = clan->GetClanBattleStatus();
        if( status != 0 )
        {
            std::cout<<"sorry have opened"<<std::endl;
            return 2;
        }
        clan->SetClanBattleStatus(1);
        DB7().PushUpdateData( "update clan set `clanBattleStatus`= %u where clanId = %u ",static_cast<UInt8>(1),clan->GetId());
        return 3;
    }
    */

    UInt8 Player::SignUpClanBattle()   //报名军团战
    {
        Clan* clan = GetClan();
        if( clan == NULL )
        {
            return 1;
        }

        UInt32 roomId  = clan->GetClanBattleRoomId();
        UInt8 pos =  GetClanPos();
        if(roomId == 0 )
        {
            //开启军团战
            if( pos < 2 )
            {
                SetJoinClanBattle(1);
                DB7().PushUpdateData( "update clan_player set `isClanBattle`= %u where (clanId = %u and playerId = %"I64_FMT"u)",static_cast<UInt8>(1),clan->GetId(),GetId()); 
                Battle::clanBattleRoomManager.EnterRoom(this);
                return 0;
            }
            else
            {
                return 2;
            }
        }
        else
        {
            UInt8 isJoin = GetJoinClanBattle();
            if( isJoin != 0 )
            {
                return 3;
            }
            SetJoinClanBattle(1);
            DB7().PushUpdateData( "update clan_player set `isClanBattle`= %u where (clanId = %u and playerId = %"I64_FMT"u)",static_cast<UInt8>(1),clan->GetId(),GetId());
            return 0;
        }
    }

    void Player::InsertClanBattleFighter(UInt8 mapId,UInt16 fighterId,UInt8 posx,UInt8 posy)
    {
        ClanBattleFighter* battleInfo = new ClanBattleFighter(mapId,fighterId,posx,posy);
        if( battleInfo == NULL )
            return;
        _vecClanBattleFighter.push_back(battleInfo);
    }

    UInt8 Player::UpFighter(UInt16 fighterId)
    { 
        static UInt32 cost[] = {5 ,15 ,30 ,50 ,75 ,105 ,140 ,180 ,225 ,275 ,330 ,390 ,455 ,525 ,600 ,680 ,765 ,855 ,950 ,1050};
        Fighter *fgt = findFighter(fighterId);
        if(!fgt)
            return 1;
        UInt8 quality = fgt->GetVar(FVAR_QUALITY);
        if(quality >= 20)
            return 2;
        if(cost[quality] != GetPackage()->DelItem(fighterId + 40000 , cost[quality]))
            return 3;
        fgt->AddVar(FVAR_QUALITY,1);
        return 0;
    } 

    UInt8 Player::GetLevel() { return getMainFighter()->GetLevel();}

    void Player::DelClanBattleFighter(UInt8 mapId,UInt16 fighterId,UInt8 posx,UInt8 posy)
    {
        for( auto it = _vecClanBattleFighter.begin(); it != _vecClanBattleFighter.end(); ++it )
        {
            if( (*it)->GetMapId() == mapId && (*it)->GetFighterId() == fighterId )
            {
                delete (*it);
                it = _vecClanBattleFighter.erase(it);
                break;
            }
        }
    }

    ClanBattleFighter* Player::GetClanBattleFighter(UInt16 fighterId)
    {
        if( fighterId <= 0 )
        {
            return NULL;
        }
        for(auto it = _vecClanBattleFighter.begin(); it != _vecClanBattleFighter.end(); ++it)
        {
             if( (*it)->GetFighterId() == fighterId)
             {
                 return (*it);
             }
        }
        return NULL;
    }

    UInt8 Player::Sign(UInt8 opt)
    { 
        UInt32 val = GetVar(VAR_SIGN_MONTH);
        UInt8 day = TimeUtil::MonthDay();
        if(!day)
            return 0;
        if(val & (1 << (day-1)))
            return 0;
        val |= (1 << (day-1));
        SetVar(VAR_SIGN_MONTH,val);
           
        UInt8 res = 0;
        while(val)
        { 
            if(val%2)
            {
                ++res;
            }
            val /= 2;
        } 
        return res;
    } 
}

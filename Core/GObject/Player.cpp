#include "Player.h"
#include "Fighter.h"
#include "ChatHold.h"

#define P_CHAT_MAX 10
namespace GObject
{
    //GlobalNamedPlayers globalPlayers;
    GlobalPlayers globalPlayers;
    GlobalPlayers globalOnlinePlayers;
    GlobalNamedPlayers globalNamedPlayers;
    GlobalClans globalClan;
    //GlobalNamedPlayers globalAccountsPlayers;
    Player::Player( IDTYPE id ): GObjectBaseT<Player, IDTYPE>(id),_isOnline(false),_session(-1)
    {
        m_pVars = new VarSystem(id);
        m_Package = new Package(this); 
        m_friendMgr = new FriendManager(this);
        chatHold = NULL;
        clan = NULL;
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
        DB2().PushUpdateData("INSERT INTO `fighter` (`playerId`,`fighterId`,`experience`,`addTime`) VALUES( %u,%u,0,%u)",getId(),id,now);
    } 

    void Player::Login()
    { 
        //TODO
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
        st << name ; 
        st << GetVar(VAR_EXP);
        st << GetVar(VAR_TEAL);
        st << GetVar(VAR_GOLD);
        st << GetVar(VAR_TOTAL_GOLD);
        std::map<UInt32, Fighter *>::iterator it = _fighters.begin();
        st << static_cast<UInt8>(_fighters.size());
        for(;it != _fighters.end();++it)
        { 
            (it->second)->MakeFighterInfo(st);
        } 
    }

    ChatHold* Player::GetChatHold()
    { 
        if(!chatHold)
        { 
            chatHold = new ChatHold(P_CHAT_MAX);
        } 
        return chatHold;
    } 
}

#include "Player.h"

namespace GObject
{
    GlobalNamedPlayers globalPlayers;
    GlobalNamedPlayers globalOnlinePlayers;
    GlobalNamedPlayers globalNamedPlayers;
    GlobalNamedPlayers globalAccountsPlayers;
    Player::Player(): _isOnline(false),_session(-1)
    {
         _id = "";
         _accounts = "";
         _password = "";
         m_Package = new Package(this); 
         m_friendMgr = new FriendManager(this);
    }
    Player::~Player()
    { 

    } 
    
    UInt32 Player::GetVar(UInt32 id)
    {
        return getVarSystem()->GetVar(id);
    }

    Int32 Player::GetVarS(Int32 id)
    {
        return (Int32)getVarSystem()->GetVar(id);
    }

    void Player::LoadVar(UInt32 id, UInt32 val, UInt32 overTime)
    {
        getVarSystem()->LoadVar(id, val, overTime);
    }

    void Player::SetVar(UInt32 id, UInt32 val)
    {
        getVarSystem()->SetVar(id,val);
    }

    void Player::DelVar(UInt32 id )
    {
        getVarSystem()->DelVar(id);
    }

    void Player::AddVar(UInt32 id, UInt32 val)
    {
        getVarSystem()->AddVar(id,val);
    }

    void Player::AddVarS(UInt32 id, Int32 val)
    {
        Int32 v = GetVarS(id);
        getVarSystem()->SetVar(id,v+val);
    }

    UInt32 Player::GetVarNow(UInt32 id,  UInt32 now)
    {
        return getVarSystem()->GetVar(id , now);
    }

    void Player::SetVarNow(UInt32 id, UInt32 val, UInt32 now)
    {
        getVarSystem()->SetVar(id,val, now);
    }

    void Player::AddVarNow(UInt32 id, UInt32 val, UInt32 now)
    {
        getVarSystem()->AddVar(id,val, now);
    }

    void Player::SetVarOffset(UInt32 offset)
    {
        getVarSystem()->SetOffset(offset);
    }
    
    void Player::GetSelfInfoStream(Stream &st)
    { 
        st << GetVar(VAR_GOLD);
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
            DB1().PushUpdateData("UPDATE `player` SET `openid` = '%s' WHERE `id` = '%s' and `accounts` = '%s'"  , m_openid, getId().c_str(),getAccounts().c_str());
        }     
    }

    void Player::addFighter( Fighter * fgt, bool writedb, bool load )
    { 
        //TODO
    } 

    void Player::Login()
    { 
        //TODO
    } 
    void Player::PutFighters( Battle::BattleSimulator& bsim, int side, bool fullhp ,UInt16 fighterId)
    { 
        //TODO
    } 
    VarSystem * Player::getVarSystem()
    { 
        if(!m_pVars)
            m_pVars = new VarSystem(this);
        return m_pVars;

    } 
}

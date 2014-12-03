#ifndef _LOGINOUTERMSGHANDLER_H_
#define _LOGINOUTERMSGHANDLER_H_

#include <string>

#include "MsgTypes.h"
#include "MsgFunc.h"
#include "MsgID.h"

#include "Common/Serialize.h"
#include "Common/Stream.h"
#include "Common/BinaryReader.h"
#include "CountryMsgStruct.h"
#include "Common/TimeUtil.h"
#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "Server/OidGenerator.h"
#include "GObject/GObjectManager.h"
#include "GObject/Player.h"
#include "GObject/Package.h"
#include "Battle/BattleReport.h"
#include "MsgHandler/GMHandler.h"
#include "Common/BinaryReader.h"
#include "Common/SHA1Engine.h"
#include "Common/StringTokenizer.h"
#include "GObject/Var.h"
//#include "MsgHandler/JsonParser.h"
#include "GObject/GObjectDBExecHelper.h"

#include "Memcached.h"
#include "Version.h"
#include "GObject/Fighter.h"
#include "GObject/Player2Id.h"

#ifndef _WIN32
//#include <libmemcached/memcached.h>
//#include "GObject/DCLogger.h"
#endif

bool getId(char buf[64], UInt8 type = 0);
bool checkKey(UInt8 type, const UInt8* _hashval, std::string  _userid);

inline UInt8 doLogin(Network::GameClient * cl, IDTYPE phoneid,std::string accounts, std::string password, UInt32 hsid, GObject::Player *& player, bool kickOld = true, bool reconnect = false)
{ 
    //return ：0--成功 1--失败  2--重复登录   6--封登录
    IDTYPE pid = player2Id.getPlayerId(phoneid , accounts ,password);

    player = GObject::globalPlayers[pid];

    UInt8 res = 0;
    if(!player)
    { 
        cl->SetPlayer(NULL);
        cl->SetPlayerId(pid);  
        return 1;
    } 
    else
    { 
        //TODO
    } 
    UInt32 sid = player->GetSessionID();
    if(hsid != sid)  //重复登录 （挤下线）
    { 
        //TODO
        return 4;
    } 
    player->SetSessionID(hsid);
    cl->SetPlayer(player);
    return res;
} 

#define CHKKEY() \
{\
    UInt8 hash[64] = {0};\
    if (!br.read(hash, 36))\
    return;\
    char id[64] = {0};\
    if (!getId(id, 0))\
    return;\
    if (!checkKey(1, hash, atoll(id)))\
    return;\
}

struct UserDisconnectStruct
{
    MESSAGE_DEF(0xFF);
};


struct UserReconnectStruct
{
    std::string _userid;
    MESSAGE_DEF1(REQ::RECONNECT, std::string, _userid);
};

struct UserLoginStruct
{
    IDTYPE _userid;
    std::string accounts;
    std::string password;
    typedef Array<UInt8, 36> HashValType;
    UInt8 _hashval[36];
    //std::string _server;
    //std::string _platform;
    //std::string _openid;
    //std::string _openkey;
    //std::string _via;
    std::string _para;
    MESSAGE_DEF5(REQ::LOGIN, IDTYPE, _userid, std::string, accounts, std::string, password, HashValType, _hashval/*, std::string, _server, std::string, _platform, std::string, _openid, std::string, _openkey, std::string, _via */, std::string, _para);
};

struct UserLogonRepStruct 
{
    UInt32 _result;
    std::string _name;
    MESSAGE_DEF2(REP::LOGIN, UInt32, _result, std::string, _name);
};
void UserLoginReq(LoginMsgHdr& hdr, UserLoginStruct& ul)
{ 
    TcpConnection conn = NETWORK()->GetConn(hdr.sessionID);
    if(conn.get() == NULL)
        return;
    UInt8 res = 0;
    if(!ul._userid)  //LIBO  UInt64
    {    
        conn->pendClose();
        return;
    }                 

    if (cfg.onlineLimit && SERVER().GetTcpService()->getOnlineNum() > cfg.onlineLimit)
    {
        conn->pendClose();
        return;
    }

    //checkKey(0, ul._hashval, ul._userid);  //LIBO

    {
        Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());  
        GObject::Player * player = NULL;
        IDTYPE pid = ul._userid;
        std::string accounts = ul.accounts;
        if(cfg.merged)
        { 
            //处理合服
            //pid |= (getServerNo(ul._server) << 48);
        } 

        res = doLogin(cl, pid, accounts,ul.password, hdr.sessionID, player);
        //XXX
        std::string clientIp;StringTokenizer st(ul._para, ":");
        switch (st.count())
        {
            case 1:
                clientIp = st[0];
                break;
            default:
                break;
        }
        char domain[256] = "";
        if (player)
        {
            strncpy (domain, player->getDomain(), 256);
            player->setClientIp(clientIp);
            GObject::globalOnlinePlayers.add(player);
        }
        if(res == 0)
        { 
            cl->SetStatus(Network::GameClient::NORMAL); 
        } 
        if(res > 0)
        {
            UserLogonRepStruct rep;
            rep._result = res;
            NETWORK()->SendMsgToClient(conn.get(), rep);
        }
    }
} 


struct NewUserStruct
{
    IDTYPE playerId;
    std::string accounts;
    std::string password;
    std::string _name;
    UInt8 _class;
    //UInt8 _rp; //1:回流免费用户 2:回流vip1-vip4用户 3:回流vip5+用户
    std::string _para;
    MESSAGE_DEF6(REQ::CREATE_ROLE, IDTYPE , playerId,std::string ,accounts,std::string ,password ,std::string, _name, UInt8, _class, std::string, _para);
};
bool checkKey(UInt8 type, const UInt8* _hashval, IDTYPE _userid)
{
    if (cfg.GMCheck)
    {
        SHA1Engine sha1;
        std::string key1, key2;

        if (type == 0)
        {
            key1 = cfg.cryptKey1;
            key2 = cfg.cryptKey2;
        }
        else if (type == 1)
        {
            key1 = cfg.gmCryptKey1;
            key2 = cfg.gmCryptKey2;
        }
        else if (type == 2)
        {
            key1 = cfg.fbrCryptKey1;
            key2 = cfg.fbrCryptKey2;
        }
        else
        {
            return false;
        }

        sha1.update(_hashval + 8, 4);
        sha1.update(key1.c_str(), key1.length());
        sha1.update(_hashval, 4);
        sha1.update(&_userid, sizeof(std::string));
        sha1.update(_hashval + 12, 4);
        sha1.update(key2.c_str(), key2.length());
        sha1.update(_hashval + 4, 4);

        std::vector<UInt8> buf = sha1.digest();

        if (memcmp(&buf.front(), _hashval + 16, 20) == 0)
            return true;
        return false;
    }
    return true;
}

struct NewUserRepStruct 
{
    UInt32 _result;
    std::string _name;
    MESSAGE_DEF2(REP::NEW_CHARACTER, UInt32, _result, std::string, _name);
};
//创号， 根据手机号（playerId） 根绝帐号密码(accounts)
void NewUserReq( LoginMsgHdr& hdr, NewUserStruct& nu )
{
    TcpConnection conn = NETWORK()->GetConn(hdr.sessionID);
    if(conn.get() == NULL)
        return;

    if (!hdr.playerID && nu.accounts.empty())
    {
        conn->pendClose();
        return;
    }

    std::string clientIp;

    StringTokenizer st(nu._para, ":");
    switch (st.count())
    {
        case 1:
            clientIp = st[0];
            break;
        default:
            break;
    }

    if (cfg.enableLoginLimit && SERVER().GetTcpService()->getOnlineNum() > cfg.loginLimit)
    {
        UserLogonRepStruct rep;
        rep._result = 5;
        NETWORK()->SendMsgToClient(conn.get(), rep);
        conn->pendClose();
        return;
    }

    nu._class &= ~0x80;

    if(nu._name.empty() || nu._name.length() > 15 || (strncmp(nu._name.c_str(), "ERROR_", 6) == 0))
    {
        NewUserRepStruct rep;
        rep._result = 2;
        NETWORK()->SendMsgToClient(conn.get(), rep);
        return;
    }

    UInt32 res;

    std::string newname = nu._name;

    //GObject::Player::patchMergedName(hdr.playerID, newname);

    GObject::Player * pl = GObject::globalNamedPlayers[newname];
    if(pl == NULL)
    {
        pl = GObject::globalPlayers[hdr.playerID];
    }

    if(pl != NULL)
    {
        res = 1;
    }
    else
    {
        pl = new(std::nothrow) GObject::Player(hdr.playerID);  //XXX
        //pl->setId(nu.playerId);
        //pl->setAccounts(nu.accounts);

        //设置玩家数据
        //TODO

        UInt32 fgtId = nu._class + 1;

        GObject::Fighter * fgt2 = GObject::globalFighters[fgtId];
        GObject::Fighter * fgt = NULL;
        if(fgt2 != NULL)
        {
            fgt = fgt2->clone(pl);
            if(fgt != NULL)
                pl->addFighter(fgt, true);
        }
        if(fgt == NULL)
        {
            delete pl;
            res = 1;
        }
        else
        {

            //插入player表
            //TODO

            //GObject::newPlayers.add(pl);
            if(pl->getId())
                GObject::globalPlayers.add(pl);
            //if(pl->getAccounts().empty())
            //    GObject::globalAccountsPlayers.add(nu.accounts,pl);
            GObject::globalNamedPlayers.add(newname, pl);
            pl->setClientIp(clientIp);
            res = 0;

            pl->SetSessionID(hdr.sessionID);
            Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
            cl->SetPlayer(pl);

        }
    }

    if(res == 0)
    {
        return;
    }

    NewUserRepStruct rep;
    rep._result = res;
    NETWORK()->SendMsgToClient(conn.get(), rep);
}
#endif // _LOGINOUTERMSGHANDLER_H_



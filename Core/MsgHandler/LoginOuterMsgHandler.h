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
#include "GObject/MapCollection.h"
#include "GObject/Mail.h"
#include "GObject/Prepaid.h"
#include "GObject/Player.h"
#include "GObject/Fighter.h"
#include "GObject/Package.h"
#include "GObject/Clan.h"
#include "Battle/BattleReport.h"
#include "MsgHandler/GMHandler.h"
#include "Common/BinaryReader.h"
#include "Common/SHA1Engine.h"
#include "Common/StringTokenizer.h"
#include "GData/Formation.h"
#include "GData/ExpTable.h"
#include "GData/Store.h"
#include "GObject/Player.h"
#include "GObject/SaleMgr.h"
#include "GObject/WBossMgr.h"
#include "GObject/FrontMap.h"
#include "GObject/Copy.h"
#include "GObject/Dungeon.h"
#include "GObject/World.h"
#include "GObject/Var.h"
#include "GObject/RealItemAward.h"
#include "GObject/Tianjie.h"
//#include "MsgHandler/JsonParser.h"
#include "GObject/SingleHeroStage.h"
#include "GObject/GObjectDBExecHelper.h"

#include "Memcached.h"
#include "GObject/RechargeTmpl.h"
#include "Version.h"

#ifndef _WIN32
//#include <libmemcached/memcached.h>
#include "GObject/DCLogger.h"
#endif

bool getId(char buf[64], UInt8 type = 0);
bool checkKey(UInt8 type, const UInt8* _hashval, UInt64 _userid);

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

static void serverNameToGlobalName(std::string& name, UInt16 sid)
{
    if(cfg.merged && sid > 0)
    {
        sid = sid - 1;
        do
        {
            name.push_back(static_cast<char>((sid % 31) + 1));
            sid /= 31;
        }
        while(sid > 0);
    }
}

struct UserDisconnectStruct
{
	MESSAGE_DEF(0xFF);
};

struct UserReconnectStruct
{
	UInt64 _userid;
	MESSAGE_DEF1(REQ::RECONNECT, UInt64, _userid);
};

struct UserLoginStruct
{
	UInt64 _userid;
    UInt8 _level;
    UInt8 _level1;
    UInt8 _isYear;
	UInt32 _lang;
	typedef Array<UInt8, 36> HashValType;
	UInt8 _hashval[36];
	std::string _server;
    std::string _platform;
    std::string _openid;
    std::string _openkey;
    std::string _via;
    std::string _para;
	MESSAGE_DEF12(REQ::LOGIN, UInt64, _userid, UInt8, _level, UInt8, _level1, UInt8, _isYear, UInt32, _lang,
            HashValType, _hashval, std::string, _server, std::string, _platform, std::string, _openid, std::string, _openkey, std::string, _via, std::string, _para);
};

struct NewUserStruct
{
	std::string _name;
	UInt8 _class;
    UInt8 _level;
    UInt8 _level1;
    UInt8 _isYear;
    std::string _platform;
    std::string _openid;
    std::string _openkey;
    std::string _via;
    std::string _invited;
    UInt8 _rp; //1:回流免费用户 2:回流vip1-vip4用户 3:回流vip5+用户
    std::string _para;
    MESSAGE_DEF12(REQ::CREATE_ROLE, std::string, _name, UInt8, _class, UInt8, _level, UInt8, _level1, UInt8, _isYear,
            std::string, _platform, std::string, _openid, std::string, _openkey, std::string, _via, std::string, _invited, UInt8, _rp, std::string, _para);
    /*MESSAGE_DEF11(REQ::CREATE_ROLE, std::string, _name, UInt8, _class, UInt8, _level, UInt8, _level1, UInt8, _isYear,
            std::string, _platform, std::string, _openid, std::string, _openkey, std::string, _via, std::string, _invited, UInt8, _rp);
            */
/*	MESSAGE_DEF10(REQ::CREATE_ROLE, std::string, _name, UInt8, _class, UInt8, _level, UInt8, _level1, UInt8, _isYear,
            std::string, _platform, std::string, _openid, std::string, _openkey, std::string, _via, std::string, _invited);
*/
};


void UserDisconnect( GameMsgHdr& hdr, UserDisconnectStruct& )
{
	MSG_QUERY_PLAYER(player);
	player->SetSessionID(-1);
	GameMsgHdr imh(0x200, player->getThreadId(), player, 0);
	GLOBAL().PushMsg(imh, NULL);
#ifndef _WIN32
#ifdef _FB
#else
    GObject::dclogger.decDomainOnlineNum(atoi(player->getDomain()));
#endif
#endif // _WIN32
}

struct UserLogonRepStruct
{
	UInt32 _result;
	std::string _name;

	MESSAGE_DEF2(REP::LOGIN, UInt32, _result, std::string, _name);
};

bool IsBigLock(UInt64 pid)
{
    pid = pid & 0xFFFFFFFFFF;
/*    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
    if (execu.get() != NULL && execu->isConnected())
    {
        GObject::DBBigLock lockData;
        char sql[256] = {0};
        sprintf(sql,"SELECT `player_id`, `lockExpireTime` FROM `locked_player` where `player_id`=%"I64_FMT"u",pid);
        if(execu->Prepare(sql, lockData) == DB::DB_OK && 
           execu->Next() == DB::DB_OK &&
           lockData.lockExpireTime > TimeUtil::Now())
        {
            return true;
        }
    }
    */
    if (getLockUserValue(pid) >= TimeUtil::Now())
        return true;
    return false;
}
inline UInt8 doLogin(Network::GameClient * cl, UInt64 pid, UInt32 hsid, GObject::Player *& player, bool kickOld = true, bool reconnect = false)
{
	player = GObject::globalPlayers[pid];
	if(player == NULL)
	{
		cl->SetPlayer(NULL);
		cl->SetPlayerId(pid);
		return 1;
	}
	else
	{
		if(player->getLockExpireTime() > 0)
		{
			if(player->getLockExpireTime() <= TimeUtil::Now())
			{
				player->setLockExpireTime(0);
				DB1().PushUpdateData("DELETE FROM `locked_player` WHERE `player_id` = %"I64_FMT"u", pid);
			}
			else
				return 6;
		}
	}
    //查看是否被全服禁号
    if (IsBigLock(pid))
    {
        return 6;
    }
	UInt32 sid = player->GetSessionID();
	UInt8 res = 0;
	if(sid != static_cast<UInt32>(-1) && sid != hsid)
	{
		TcpConnection c = NETWORK()->GetConn(sid);
		if(c.get() != NULL)
		{
			if(kickOld)
			{
                TRACE_LOG("被踢, %"I64_FMT"u, %u, %u", player->getId(), sid, hsid);
				Network::GameClient * cl2 = static_cast<Network::GameClient *>(c.get());
                if (cl2 && cl2->GetPlayer() == player)
                {
                    player->SetSessionID(-1);
                    player->testBattlePunish();
                    static UInt8 kick_pkt[4] = {0x00, 0x00, 0xFF, REP::BE_DISCONNECT};
                    cl2->send(kick_pkt, 4);
                    cl2->SetPlayer(NULL);
                    cl2->pendClose();
                    player->SetVar(GObject::VAR_OFFLINE, TimeUtil::Now());
                    res = 4;
                }
			}
			else
			{
				return 2;
			}
		}
	}

	player->SetSessionID(hsid);
	cl->SetPlayer(player);

    std::string fsaleTime;
    player->setForbidSale(checkForbidSale(player->getId(), fsaleTime));
//    std::string foverTime;
//    player->setForbidSale(checkForbidSale(player->getId(), fsaleTime,foverTime));
	return res;
}

void SockAddrReq(LoginMsgHdr&hdr, const void * sa)
{
	TcpConnection conn = NETWORK()->GetConn(hdr.sessionID);
	if(conn.get() == NULL)
		return;
	Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
	cl->SetClientAddr(reinterpret_cast<const sockaddr *>(sa));
}

void UserReconnectReq(LoginMsgHdr& hdr, UserReconnectStruct& ur)
{
	TcpConnection conn = NETWORK()->GetConn(hdr.sessionID);
	if(conn.get() == NULL)
		return;

	if(ur._userid == 0)
		conn->pendClose();

	Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
	GObject::Player * player = NULL;
    UInt8 res = doLogin(cl, ur._userid, hdr.sessionID, player, false, true);
    if (res == 6)
    {
		UserLogonRepStruct rep;
		rep._result = 6;
		NETWORK()->SendMsgToClient(conn.get(), rep);
		conn->pendClose();
        return;
    }

	if(res != 0)
	{
		Stream st(REP::RECONNECT);
		st << static_cast<UInt8>(1) << Stream::eos;
		cl->send(&st[0], st.size());
		return;
	}

	GameMsgHdr imh(0x202, player->getThreadId(), player, 0);
	GLOBAL().PushMsg(imh, NULL);
}

static inline UInt64 getServerNo(std::string& sv)
{
	size_t n = sv.find_last_of('_');
	if(n == std::string::npos)
		return 0;
	return atoi(sv.c_str() + n + 1);
}

void UserLoginReq(LoginMsgHdr& hdr, UserLoginStruct& ul)
{
	TcpConnection conn = NETWORK()->GetConn(hdr.sessionID);
	if(conn.get() == NULL)
		return;

	if(ul._userid == 0)
    {
		conn->pendClose();
        return;
    }

    if (cfg.onlineLimit && SERVER().GetTcpService()->getOnlineNum() > cfg.onlineLimit)
    {
		conn->pendClose();
        return;
    }

	UInt8 res;
	if(!cfg.GMCheck || checkKey(0, ul._hashval, ul._userid))
	{
		Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
		GObject::Player * player = NULL;
		UInt64 pid = ul._userid;
		if(cfg.merged)
		{
            if (!cfg.mergeList.empty())
            {
                StringTokenizer st(cfg.mergeList, " ");
                if(st.count())
                {
                    UInt32 i;
                    for (i = 0; i < st.count(); ++i)
                    {
                        //printf("atoi(st[i].c_str()):%d, getServerNo(ul._server):%lu\n", atoi(st[i].c_str()), getServerNo(ul._server));
                        if(static_cast<UInt64>(atoi(st[i].c_str())) == getServerNo(ul._server))
                            break;
                    }
                    if(i == st.count())
                        return;
                }
            }
			pid |= (getServerNo(ul._server) << 48);
		}

		res = doLogin(cl, pid, hdr.sessionID, player);

        std::string clientIp;
        std::string pf;
        std::string pfkey;
        std::string xinyue;
        StringTokenizer st(ul._para, ":");
        switch (st.count())
        {
            case 4:
                xinyue = st[3];
            case 3:
                pfkey = st[2].c_str();
            case 2:
                pf = st[1].c_str();
            case 1:
                clientIp = st[0].c_str();
                break;
            default:
                break;
        }

        if (4 == res)
        {
            //UInt8 platform = atoi(player->getDomain());
            if (cfg.GMCheck )
            {
                size_t len = 0;
                char key[MEMCACHED_MAX_KEY] = {0};
                char value[32] = {0};
                len = snprintf(key, sizeof(key), "%d_%s_%d", memc_version, clientIp.c_str(), cfg.serverNum);
                if (MemcachedGet(key, len, value, sizeof(value)))
                {
                    int v = atoi(value) + 1;
                    setCrackValue(clientIp.c_str(), v);
                }
            }
        }
        
        TRACE_LOG("id: %"I64_FMT"u from %s of asss_%d", ul._userid, clientIp.c_str(), cfg.serverNum);
        if (res == 0 && player && cfg.GMCheck && checkCrack(ul._platform, clientIp, ul._userid))
        {
            player->SetSessionID(-1);
            cl->SetPlayer(NULL);
            conn->pendClose();
            return;
        }

        char domain[256] = "";
        if (player)
        {
            strncpy (domain, player->getDomain(), 256);
            player->setDomain(ul._platform);
            player->setOpenId(ul._openid);
            player->setOpenKey(ul._openkey);
            player->setVia(ul._via);
            player->setClientIp(clientIp);
            player->setSource(pf);
            player->setPfKey(pfkey);
            player->setXinYue(atoi(xinyue.c_str()));
#ifdef _FB
            PLAYER_DATA(player, wallow) = 0;
#endif
        }

		UInt8 flag = 0;
		if(res == 0)
		{
            TRACE_LOG("登陆成功, %s, %"I64_FMT"u, %"I64_FMT"u, %u, %d", ul._openid.c_str(), ul._userid, pid, hdr.sessionID, player->getThreadId());
#ifndef _WIN32
#ifdef _FB
#else
            GObject::dclogger.incDomainOnlineNum(atoi(ul._platform.c_str()));
#endif
#endif //_WIN32
            player->setQQVipl(ul._level);
            player->setQQVipl1(ul._level1);
            player->setQQVipYear(ul._isYear);
			GameMsgHdr imh(0x201, player->getThreadId(), player, 1);
			GLOBAL().PushMsg(imh, &flag);
		}
		else if(res == 4)
		{
            TRACE_LOG("重复登陆, %s, %"I64_FMT"u, %"I64_FMT"u, %u, %d", ul._openid.c_str(), ul._userid, pid, hdr.sessionID, player->getThreadId());
#ifndef _WIN32
#ifdef _FB
#else
            GObject::dclogger.decDomainOnlineNum(atoi(domain));
            GObject::dclogger.incDomainOnlineNum(atoi(ul._platform.c_str()));
#endif
#endif //_WIN32
            player->setQQVipl(ul._level);
            player->setQQVipl1(ul._level1);
            player->setQQVipYear(ul._isYear);
			flag = 1;
			GameMsgHdr imh(0x201, player->getThreadId(), player, 1);
			GLOBAL().PushMsg(imh, &flag);
			res = 0;
		}

        cl->SetStatus(Network::GameClient::NORMAL);
	}
	else
		res = 2;

	if(res > 0)
	{
		UserLogonRepStruct rep;
		rep._result = res;
		NETWORK()->SendMsgToClient(conn.get(), rep);
	}
	if(res == 2 || res == 3 || res == 6)
		conn->pendClose();
}

struct NewUserRepStruct
{
	UInt32 _result;
	std::string _name;

	MESSAGE_DEF2(REP::NEW_CHARACTER, UInt32, _result, std::string, _name);
};

static inline int char_type(UInt8 p)
{
	if(p < 0x20) return -1;
	if(p >= 0xFE) return -1;
	if(p >= 0xFC) return 6;
	if(p >= 0xF8) return 5;
	if(p >= 0xF0) return 4;
	if(p >= 0xE0) return 3;
	if(p >= 0xC0) return 2;
	if(p >= 0x80) return -1;
	return 1;
}

void trimName(std::string& str)
{
	size_t len = str.length();
	size_t i = 0;
	while(i < len)
	{
		if(str[i] >= 0 && str[i] <= ' ')
			++ i;
		else if(i + 2 < len && static_cast<UInt8>(str[i]) == 0xE3 && static_cast<UInt8>(str[i + 1]) == 0x80 && static_cast<UInt8>(str[i + 2]) == 0x80)
			i += 3;
		else
			break;
	}
	size_t j = len;
	while(j > 0)
	{
		if(str[j - 1] >= 0 && str[j - 1] <= ' ')
			-- j;
		else if(j > 2 && static_cast<UInt8>(str[j - 3]) == 0xE3 && static_cast<UInt8>(str[j - 2]) == 0x80 && static_cast<UInt8>(str[j - 1]) == 0x80)
			j -= 3;
		else
			break;
	}
	if(i < j)
		str = std::string(str.begin() + i, str.begin() + j);
	else
		str.clear();
	len = str.length();
	i = 0;
	while(i < len)
	{
		int cur = char_type(str[i]);
		if(cur < 0)
		{
			str.erase(str.begin() + i);
			-- len;
		}
		else
			i += static_cast<size_t>(cur);
	}
	if(i > len)
		str.clear();
}

void NewUserReq( LoginMsgHdr& hdr, NewUserStruct& nu )
{
    UserStruct us;
	us._name	 = nu._name;
	us._class	 = nu._class;
    us._level	 = nu._level;
    us._level1	 = nu._level1;
    us._isYear	 = nu._isYear;
    us._platform = nu._platform;
    us._openid	 = nu._openid;
    us._openkey	 = nu._openkey;
    us._via	     = nu._via;
    us._invited	 = nu._invited;
    us._rp	     = nu._rp;
    us._clientIp = "";
	TcpConnection conn = NETWORK()->GetConn(hdr.sessionID);
	if(conn.get() == NULL)
		return;

    if (!hdr.playerID)
    {

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
        GObject::dclogger.create_sec(us);
#endif
#endif
#endif
		conn->pendClose();
        return;
    }

    std::string clientIp;
    std::string pf;
    std::string pfkey;
    std::string xinyue;
    StringTokenizer st(nu._para, ":");
    switch (st.count())
    {
        case 4:
            xinyue = st[3];
        case 3:
            pfkey = st[2].c_str();
        case 2:
            pf = st[1].c_str();
        case 1:
            clientIp = st[0].c_str();
            break;
        default:
            break;
    }

    if (cfg.GMCheck && checkCrack(us._platform, clientIp, hdr.playerID))
    {
        conn->pendClose();
        return;
    }

    if (IsBigLock(hdr.playerID))
    {
        UserLogonRepStruct rep;
        rep._result = 6;
        GObject::dclogger.create_sec(us);
        NETWORK()->SendMsgToClient(conn.get(), rep);
        conn->pendClose();
        return;
    }
    if (cfg.rpServer && !GObject::dclogger.checkRPOpenid((char*)us._openid.c_str()))
    {
        UserLogonRepStruct rep;
        rep._result = 7;
        GObject::dclogger.create_sec(us);
        NETWORK()->SendMsgToClient(conn.get(), rep);
        conn->pendClose();
        return;
    }

    if (cfg.enableLoginLimit && SERVER().GetTcpService()->getOnlineNum() > cfg.loginLimit)
    {
        UserLogonRepStruct rep;
		rep._result = 5;
#ifndef _FB
#ifndef _VT
#ifndef _WIN32
        GObject::dclogger.create_sec(us);
#endif
#endif
#endif
		NETWORK()->SendMsgToClient(conn.get(), rep);
		conn->pendClose();
        return;
    }

	trimName(nu._name);

#ifdef _FB
	bool noWallow = true;
#else
	bool noWallow = (nu._class & 0x80) > 0;
#endif
	nu._class &= ~0x80;

	if(nu._name.empty() || nu._name.length() > 15 || (strncmp(nu._name.c_str(), "ERROR_", 6) == 0))
	{
		NewUserRepStruct rep;
		rep._result = 2;
#ifndef _FB
#ifndef _VT
#ifndef _WIN32
        GObject::dclogger.create_sec(us);
#endif
#endif
#endif
		NETWORK()->SendMsgToClient(conn.get(), rep);
		return;
	}

	// TODO: name filter
#if 0
	if(nu._country < 1 || nu._country > 2/* || !StringFilter::Checkup(nu._name)*/)
	{
		conn->pendClose();
		return;
	}
#endif

	UInt32 res;

	std::string newname = nu._name;
	GObject::Player::patchMergedName(hdr.playerID, newname);
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
		pl = new(std::nothrow) GObject::Player(hdr.playerID);
		if(pl == NULL)
        {
#ifndef _FB
#ifndef _VT
#ifndef _WIN32
            GObject::dclogger.create_sec(us);
#endif
#endif
#endif
			return;
        }

		UInt16 loc = 0x0002;
        UInt8 country = COUNTRY_NEUTRAL; // XXX: 低级玩家暂时规为中立

		PLAYER_DATA(pl, name) = newname;
		PLAYER_DATA(pl, country) = country;
		PLAYER_DATA(pl, wallow) = noWallow ? 0 : 1;
		PLAYER_DATA(pl, formation) = FORMATION_1;
		PLAYER_DATA(pl, created) = TimeUtil::Now();
        pl->addNewFormation(FORMATION_1);
        pl->addNewFormation(FORMATION_2);

		UInt32 fgtId = nu._class + 1;
		// PLAYER_DATA(pl, newGuild) = (0x3FFFFFFFull << NEWGUILDSTEP_MAX);
		PLAYER_DATA(pl, newGuild) = 0;

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
			GObject::Lineup& lup = PLAYER_DATA(pl, lineup)[0];
			lup.pos = 12;
			lup.fighter = fgt;
			lup.updateId();

			DB1().PushUpdateData("INSERT INTO `player` (`id`, `name`, `country`, `location`, `lineup`, `wallow`, `formation`, `formations`, `openid`) VALUES (%" I64_FMT "u, '%s', %u, %u, '%u,12', %u, %u, '%u,%u', '%s')", pl->getId(), nu._name.c_str(), country, loc, fgtId, PLAYER_DATA(pl, wallow), FORMATION_1, FORMATION_1, FORMATION_2, nu._openid.c_str());

			GObject::globalPlayers.add(pl);
			GObject::newPlayers.add(pl);
			GObject::globalNamedPlayers.add(newname, pl);
            pl->setClientIp(clientIp);
			res = 0;

			pl->SetSessionID(hdr.sessionID);
			Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
			cl->SetPlayer(pl);

            pl->setDomain(nu._platform);
            pl->setSource(pf);
            pl->setPfKey(pfkey);
            pl->setOpenId(nu._openid);
            pl->setOpenKey(nu._openkey);
            pl->setVia(nu._via);
            pl->setXinYue(atoi(xinyue.c_str()));
            if(cfg.merged)
            {
                UInt64 inviterId = (pl->getId() & 0xffff000000000000) + atoll(nu._invited.c_str());
                char szTmp[128];
                sprintf(szTmp, "%ld", inviterId);
                std::string strTmp(szTmp);
                nu._invited  = strTmp;
            }
            pl->setInvited(nu._invited);
            pl->SetVar(GObject::VAR_VIPFIRST, 1); // XXX: fix old servers

			DBLOG1().PushUpdateData("insert into register_states(server_id,player_id,player_name,platform,reg_time) values(%u,%"I64_FMT"u, '%s', %u, %u)", cfg.serverLogId, pl->getId(), pl->getName().c_str(), atoi(nu._platform.c_str()), TimeUtil::Now());

#ifndef _WIN32
#ifdef _FB
#else
            GObject::dclogger.incDomainOnlineNum(atoi(pl->getDomain()));
#endif
#endif //_WIN32
			CountryEnterStruct ces(false, 1, loc);
			GameMsgHdr imh(0x1F0, country, pl, sizeof(CountryEnterStruct));
			GLOBAL().PushMsg(imh, &ces);
			UInt8 flag = 0;
			GameMsgHdr imh2(0x201, country, pl, 1);
			GLOBAL().PushMsg(imh2, &flag);
			UInt32 gold = GObject::prepaid.pop(pl->getId());
			if(gold > 0)
			{
                struct Recharge
                {
                    UInt8 type;
                    UInt32 gold;
                } recharge;

                recharge.type = 1; // XXX: 创建角色之前充值
                recharge.gold = gold;

				GameMsgHdr hdr(0x2F0, country, pl, sizeof(recharge));
				GLOBAL().PushMsg(hdr, &recharge);
			}
            pl->setQQVipl(nu._level);
            pl->setQQVipl1(nu._level1);
            pl->setQQVipYear(nu._isYear);
            pl->setSysDailog(true);
            GameMsgHdr hdr(0x297, country, pl, 0);
            GLOBAL().PushMsg(hdr, NULL);

            pl->SetVar(GObject::VAR_RP_VALUE, nu._rp);
            pl->setTitle(0, 0);
            if (nu._via == "sscq_byhd") //捕鱼达人用户
            {
                pl->SetVar(GObject::VAR_RP_VALUE, 4);
            }
            if (GObject::World::getPetEggAct())
            {
                GObject::MailPackage::MailItem item = {9366,1};
                pl->sendMailItem(4140, 4141, &item, 1, true);
            }

#ifndef _FB
#ifndef _VT
#ifndef _WIN32
            GObject::dclogger.create_success_sec(pl);
#endif
#endif
#endif
        }
	}

	if(res == 0)
    {
		return;
    }

    
#ifndef _FB
#ifndef _VT
#ifndef _WIN32
    GObject::dclogger.create_sec(us);
#endif
#endif
#endif
	NewUserRepStruct rep;
	rep._result = res;
	NETWORK()->SendMsgToClient(conn.get(), rep);
}

bool callOpenApi(const std::string& param)
{
    if (!cfg.chargeUrl.length())
        return true;
    char curl[4096] = {0};
    snprintf(curl, sizeof(curl), "%s%s", cfg.chargeUrl.c_str(), param.c_str());
    TRACE_LOG("CHARGE URL: %s\n", curl);
    return SERVER().do_http_request(curl, 20);
}

void onUserRecharge( LoginMsgHdr& hdr, const void * data )
{
    BinaryReader br(data, hdr.msgHdr.bodyLen);

    std::string no;
    UInt64 player_Id;
    UInt16 id;
    UInt32 num;
    std::string uint;
    std::string money;

    UInt8 ret = 1;
    std::string err = "";

#ifdef _FB
    UInt8 hash[64] = {0};
    if (!br.read(hash, 36))
        return;

    br>>no;
    br>>player_Id;

    char pid[64] = {0};
    if (!getId(pid, 1))
        return;

    if (!checkKey(2, hash, atoll(pid)))
    {
        err += "Error Key";
        err = 2;
    }
#else
    std::string token;
    br>>token;
    br>>no;
    br>>player_Id;
#endif

    br>>id;
    br>>num;
    br>>uint;
    br>>money;
    UInt64 player_Id_tmp = player_Id;

    UInt16 serverNo = 0;
    br>>serverNo;
    if(cfg.merged)
    {
        player_Id += (static_cast<UInt64>(serverNo) << 48);
    }

#ifndef _WIN32
#ifdef _FB
#else
    // XXX: 只要简体需要这个参数
    std::string param;
    br>>param;

    initMemcache();
    if (memcinited)
    {
        size_t len = 0;
        size_t tlen = 0;
        unsigned int flags = 0;
        char key[MEMCACHED_MAX_KEY] = {0};

        int retry = 3;
        memcached_return rc;
        len = snprintf(key, sizeof(key), "token_27036_%"I64_FMT"u_%s", /*player_Id*/player_Id_tmp, token.c_str());
        while (retry)
        {
            --retry;
            char* rtoken = memcached_get(&memc, key, len, &tlen, &flags, &rc);
            if (rc == MEMCACHED_SUCCESS && rtoken && tlen)
            {
                ret = 0;
                if (strncmp(token.c_str(), rtoken, token.length()) != 0)
                    ret = 2;
                free(rtoken);

                if (ret == 0 && !callOpenApi(param))
                {
                    err = "confirm error.";
                    ret = 5;
                }

                break;
            }
            else
            {
                ret = 3;
                usleep(500);
            }
        }

        if (ret == 2)
            err += "token is not matched.";
        else if (ret == 3)
            err += "fetch token value error.";

        if (rc == MEMCACHED_SUCCESS && !ret)
        {
            rc = memcached_delete(&memc, key, len, (time_t)0);
            if (rc == MEMCACHED_SUCCESS)
            {
                //err += "delete key error.";
            }
        }

        if (err.length() && ret != 1)
        {
            TRACE_LOG("key: %s, token: %s, ret: %u, rc: %u, err: %s", key, token.c_str(), ret, rc, err.c_str());
            uninitMemcache();
            initMemcache();
        }
    }
    else
        err += "token server error.";
#endif
#endif //_WIN32

    if (no.length())
    {
        DB8().PushUpdateData("REPLACE INTO `recharge` (`no`,`playerId`,`id`,`num`,`status`) VALUES ('%s', %"I64_FMT"u, %u, %u, %u)",
                no.c_str(), player_Id, id, num, 0); // 0-准备/不成功 1-成功,2-补单成功
    }
    else
    {
        err += "serial number error.";
        ret = 4;
    }

    GObject::Player * pl = NULL;
    if (!err.length())
    {
        if (id == 29999 && num)
        {
            GObject::Player * player=GObject::globalPlayers[player_Id];
            if(player != NULL)
            {
                pl = player;
                struct Recharge
                {
                    UInt8 type;
                    UInt32 gold;
                    char no[256];
                    char uint[32];
                    char money[32];
                } recharge;

                memset(&recharge, 0x00, sizeof(recharge));
                recharge.type = 0; // 有角色时充值
                recharge.gold = 10 * num;
                memcpy(recharge.no, no.c_str(), no.length()>255?255:no.length());
                memcpy(recharge.uint, uint.c_str(), uint.length()>31?31:uint.length());
                memcpy(recharge.money, money.c_str(), money.length()>31?31:money.length());

                GameMsgHdr hdr(0x2F0, player->getThreadId(), player, sizeof(recharge));
                GLOBAL().PushMsg(hdr, &recharge);
                ret=0;

                player->moneyLog(1, recharge.gold);
            }
            else
            {
                GObject::prepaid.push(player_Id, num, no.c_str());
                ret=0;
            }
        }
        else
        {
            err += "wrong item id or number.";
            ret = 4;
        }
    }

    Stream st;
    st.init(SPEP::USERRECHARGE, 0x01);
    st<< ret << err;
    if (pl)
    {
        st << static_cast<UInt16>(pl->GetLev()); 
        st << static_cast<UInt16>(pl->isYD());
        if (pl->isYD())
            st << static_cast<UInt16>(pl->getQQVipl());
        else
            st << static_cast<UInt16>(0);
    }
    st << Stream::eos;

    NETWORK()->SendMsgToClient(hdr.sessionID,st);

    return;
}

void onUserReRecharge( LoginMsgHdr& hdr, const void * data )
{
    BinaryReader br(data, hdr.msgHdr.bodyLen);

    std::string no;
    UInt64 player_Id;
    UInt16 id;
    UInt32 num;

    br>>no;
    br>>player_Id;
    br>>id;
    br>>num;

    std::string err = "";
    UInt8 ret = GObject::GObjectManager::reRecharge(no, id, num, err);
    GObject::Player * pl = NULL;
    if (!ret)
    {
        GObject::Player * player=GObject::globalPlayers[player_Id];
        if(player != NULL)
        {
            pl = player;
            struct Recharge
            {
                UInt8 type;
                UInt32 gold;
                char no[256];
            } recharge;

            memset(&recharge, 0x00, sizeof(recharge));
            recharge.type = 0; // 有角色时充值
            recharge.gold = 10 * num;
            memcpy(recharge.no, no.c_str(), no.length()>255?255:no.length());

            GameMsgHdr hdr(0x2F0, player->getThreadId(), player, sizeof(recharge));
            GLOBAL().PushMsg(hdr, &recharge);
            ret=0;

            player->moneyLog(1, recharge.gold);
        }
        else
        {
            GObject::prepaid.push(player_Id, num, no.c_str());
            ret=0;
        }
    }

    Stream st;
    st.init(SPEP::RERECHARGE, 0x01);
    st<< ret << err;
    if (pl)
    {
        st << static_cast<UInt16>(pl->GetLev()); 
        st << static_cast<UInt16>(pl->isYD());
        if (pl->isYD())
            st << static_cast<UInt16>(pl->getQQVipl());
        else
            st << static_cast<UInt16>(0);
    }
    st << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
    return;
}

bool getId(char buf[64], UInt8 type)
{
#ifndef _WIN32
    if (cfg.GMCheck)
    {
        UInt8 ret = 1;
        initMemcache();
        if (!memcinited)
            return false;

        size_t len = 0;
        size_t tlen = 0;
        unsigned int flags = 0;
        char key[MEMCACHED_MAX_KEY] = {0};

        int retry = 3;
        memcached_return rc;
        if (type == 0)
            len = snprintf(key, sizeof(key), "sscq_gmtool_key_id");
        else if (type == 1)
            len = snprintf(key, sizeof(key), "sscq_recharge_key_id");
        while (retry)
        {
            --retry;
            char* rid = memcached_get(&memc, key, len, &tlen, &flags, &rc);
            if (rc == MEMCACHED_SUCCESS && rid && tlen)
            {
                ret = 0;
                memcpy(buf, rid, tlen>63?63:tlen);
                free(rid);
                break;
            }
            else
            {
                ret = 1;
                usleep(500);
            }
        }

        if (ret)
            return false;
    }
    else
#endif //_WIN32
    {
        const char* id = "20110503ll";
        memcpy(buf, id, strlen(id));
    }
    return true;
}

bool checkKey(UInt8 type, const UInt8* _hashval, UInt64 _userid = 20110503ll)
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
        sha1.update(&_userid, sizeof(UInt64));
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

inline bool player_enum_1(GObject::Player* p, void* msg)
{
    struct Msg
    {
        UInt8 pf;
        Stream* msg;
    };

    Msg* _msg = (Msg*)msg;
    if (p->isOnline() && atoi(p->getDomain()) == _msg->pf)
        p->send(*_msg->msg);

    return true;
}

void WorldAnnounce( LoginMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 pf = 0;
	UInt8 type;
	std::string msg;
    CHKKEY();
	br >> pf >> type >> msg;

	Stream st(REP::SYSTEM_INFO);
	st << type << msg << Stream::eos;

    if (!pf)
    {
        NETWORK()->Broadcast(st);
    }
    else
    {
        struct Msg
        {
            UInt8 pf;
            Stream* msg;
        } _msg;
        _msg.pf = pf;
        _msg.msg = &st;
        GObject::globalPlayers.enumerate(player_enum_1, (void*)&_msg);
    }
}

void OnKickUser(LoginMsgHdr& hdr,const void * data)
{
    Stream st;
    st.init(REP::RECONNECT, 0x01);
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt64 playerId;
    CHKKEY();
    br>>playerId;
    st<<playerId;
    INFO_LOG("GM[%s]: %"I64_FMT"u", __PRETTY_FUNCTION__, playerId);
	GObject::Player * pl= GObject::globalPlayers[playerId];
    if(pl==NULL)
    {
        st<<static_cast<UInt32>(1);
    }
    else
    {
        if(pl->isOnline())
        {
            st<<static_cast<UInt32>(0);
            TcpConnection conn = NETWORK()->GetConn(pl->GetSessionID());
            if(conn.get() == NULL)
                return;
            Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
            cl->closeConn();
        }
        else
            st<<static_cast<UInt32>(2);
    }
    st<<Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void LockUser(LoginMsgHdr& hdr,const void * data)
{
    Stream st;
    st.init(SPEP::LOCKUSER,0x01);
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt64 playerId;
    //UInt64 expireTime;
    UInt32 expireTime;
    CHKKEY();
    br>>playerId;
    br>>expireTime;
    INFO_LOG("GM[%s]: %"I64_FMT"u, %u", __PRETTY_FUNCTION__, playerId, expireTime);
    st<<playerId;
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        playerId += (static_cast<UInt64>(serverNo) << 48);
    }
    GObject::Player * pl= GObject::globalPlayers[playerId];

    if(pl==NULL)
    {
        st<<static_cast<UInt32>(1);
    }
    else
    {
        if(pl->getLockExpireTime() == static_cast<UInt32>(0))
        {
            pl->setLockExpireTime(static_cast<UInt32>(expireTime));
            DB1().PushUpdateData("REPLACE INTO `locked_player`(`player_id`, `lockExpireTime`) VALUES(%"I64_FMT"u, %u)", playerId, expireTime);
            st<<static_cast<Int32>(0);
			if(pl->isOnline())
			{
				TcpConnection conn = NETWORK()->GetConn(pl->GetSessionID());
				if(conn.get() == NULL)
					return;
				Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
				cl->closeConn();
			}
        }
        else
            st<<static_cast<UInt32>(2);
    }
    st<<Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}


void UnlockUser(LoginMsgHdr& hdr,const void * data)
{
    Stream st;
    st.init(SPEP::UNLOCKUSER,0x01);
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt64 playerId;
    CHKKEY();
    br>>playerId;
    INFO_LOG("GM[%s]: %"I64_FMT"u", __PRETTY_FUNCTION__, playerId);
    st<<playerId;
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        playerId += (static_cast<UInt64>(serverNo) << 48);
    }
    GObject::Player * pl= GObject::globalPlayers[playerId];
    if(pl==NULL)
        st<<static_cast<UInt32>(1);
    else
    {
        if(pl->getLockExpireTime() == static_cast<UInt32>(0))
        {
            st<<static_cast<UInt32>(2);
        }
        else
        {
            pl->setLockExpireTime(0);
            DB1().PushUpdateData("DELETE FROM `locked_player` WHERE `player_id` = %"I64_FMT"u", pl->getId());
            st<<static_cast<UInt32>(0);
        }
    }
    st<<Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}
std::string GetNextSection(std::string& strString , char cSeperator)
{
    std::string strRet;
    int nIndex=(int)strString.find(cSeperator);
    if(nIndex>=0)
    {
        strRet=strString.substr(0,nIndex);//Section
        strString=strString.substr(nIndex+1);
    }
    else
    {
        strRet=strString;
        strString="";
    }
    return strRet;
}

void BigLockUser(LoginMsgHdr& hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    std::string playerIds;
    UInt32 expireTime;
    CHKKEY();
    br>>playerIds;
    br>>expireTime;

    UInt8 ret = 1;
    INFO_LOG("GMBIGLOCK: %s, %u", playerIds.c_str(), expireTime);
//    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
//    if (execu.get() != NULL && execu->isConnected())
    {
        std::string playerId = GetNextSection(playerIds, ',');
        while (!playerId.empty())
        {
            UInt64 pid = atoll(playerId.c_str());
            pid = pid & 0xFFFFFFFFFF;
            memLockUser(pid, expireTime);
            setForbidSaleValue(pid, true);
//            execu->Execute2("REPLACE INTO `locked_player`(`player_id`, `lockExpireTime`) VALUES(%"I64_FMT"u, %u)", pid,expireTime);
            playerId = GetNextSection(playerIds, ',');
        }
        ret = 0;
    }
    Stream st(SPEP::BIGLOCKUSER);
    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}
void BigUnlockUser(LoginMsgHdr& hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);

    std::string playerIds;
    CHKKEY();
    br>>playerIds;

    INFO_LOG("GMBIGUNLOCK: %s", playerIds.c_str());
    UInt8 ret = 1;
//    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
//    if (execu.get() != NULL && execu->isConnected())
    {
        std::string playerId = GetNextSection(playerIds, ',');
        while (!playerId.empty())
        {
            UInt64 pid = atoll(playerId.c_str());
            pid = pid & 0xFFFFFFFFFF;
            memUnLockUser(pid);
            //execu->Execute2("DELETE FROM `locked_player` WHERE `player_id` = %"I64_FMT"u", pid);
            playerId = GetNextSection(playerIds, ',');
        }
        ret = 0;
    }
    Stream st(SPEP::BIGUNLOCKUSER);
    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void ForbidSale(LoginMsgHdr& hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    //UInt32 tm = 0;
    std::string playerIds;
    CHKKEY();
//    br >> tm;
    br>>playerIds;

    UInt16 serverNo = 0;
    if(cfg.merged)
        br >> serverNo;

    UInt8 ret = 1;
    //INFO_LOG("GMBIGLOCK: %s, %u", playerIds.c_str(), expireTime);
    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
    std::string playerId = GetNextSection(playerIds, ',');
    while (!playerId.empty())
    {
        UInt64 pid = atoll(playerId.c_str());
        pid = pid & 0xFFFFFFFFFF;
        setForbidSaleValue(pid, true);
//        setForbidSaleValue(pid, true,tm);

        if(cfg.merged)
            pid += (static_cast<UInt64>(serverNo) << 48);
	    GObject::Player * pl = GObject::globalPlayers[pid];
        if (NULL != pl)
        {
            pl->setForbidSale(true);

            GameMsgHdr hdr(0x352, pl->getThreadId(), pl, NULL);
            GLOBAL().PushMsg(hdr, NULL);
        }
        playerId = GetNextSection(playerIds, ',');

        if (execu.get() != NULL && execu->isConnected())
        {
            execu->Execute2("REPLACE into `fsale_player` values(%"I64_FMT"u,%d,1)", pid, TimeUtil::Now());
        }
 
    }
    ret = 0;
    Stream st(SPEP::FORBIDSALE);
    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void UnForbidSale(LoginMsgHdr& hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    std::string playerIds;
    CHKKEY();
    br>>playerIds;
    UInt16 serverNo = 0;
    if(cfg.merged)
        br >> serverNo;
 
    UInt8 ret = 1;
    //INFO_LOG("GMBIGLOCK: %s, %u", playerIds.c_str(), expireTime);
    std::unique_ptr<DB::DBExecutor> execu(DB::gLockDBConnectionMgr->GetExecutor());
    std::string playerId = GetNextSection(playerIds, ',');
    while (!playerId.empty())
    {
        UInt64 pid = atoll(playerId.c_str());
        pid = pid & 0xFFFFFFFFFF;
        setForbidSaleValue(pid, false);

        if(cfg.merged)
            pid += (static_cast<UInt64>(serverNo) << 48);
        GObject::Player * pl = GObject::globalPlayers[pid];
        if (NULL != pl)
            pl->setForbidSale(false);
        
        playerId = GetNextSection(playerIds, ',');

        if (execu.get() != NULL && execu->isConnected())
        {
            execu->Execute2("REPLACE into `fsale_player` values(%"I64_FMT"u,%d,0)", pid, TimeUtil::Now());
        }
 

    }
    ret = 0;
    Stream st(SPEP::UNFORBIDSALE);
    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void QueryLockUser(LoginMsgHdr& hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt64 pid = 0;
    std::string fsaleTime;
    std::string foverTime;
    CHKKEY();
    br>>pid;

    pid = pid & 0xFFFFFFFFFF;
    UInt8 isLockLogin = IsBigLock(pid);
    UInt8 isForbidSale = checkForbidSale(pid, fsaleTime);
//    UInt8 isForbidSale = checkForbidSale(pid, fsaleTime,foverTime);
        
    Stream st(SPEP::QUERYLOCKUSER);
    st << isLockLogin << isForbidSale << fsaleTime << Stream::eos;
//    st << isLockLogin << isForbidSale << fsaleTime<< foverTime << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void SetPlatformLoginLimit(LoginMsgHdr& hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt8 pf = 0;
    UInt32 value = 0;
    CHKKEY();
    br>>pf;
    br>>value;

    UInt8 ret = 1;
    INFO_LOG("GMSETLOGINLIMIT: %u, %u", pf, value);
    if (pf < 256)
    {
        setPlatformLogin(pf, value);
        ret = 0;
    }
    Stream st(SPEP::SETLOGINLIMIT);
    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void DeleteGold(LoginMsgHdr& hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    std::string playerIds;
    UInt32 gold;
    CHKKEY();
    br>>playerIds;
    br>>gold;

    UInt16 serverNo = 0;
    if(cfg.merged)
        br >> serverNo;
    UInt8 ret = 1;
    INFO_LOG("GMDELETEGOLD: %s, %u", playerIds.c_str(), gold);
    std::string playerId = GetNextSection(playerIds, ',');
    while (!playerId.empty())
    {
        UInt64 pid = atoll(playerId.c_str());
        if (cfg.merged)
            pid += (static_cast<UInt64>(serverNo) << 48);
        GObject::Player * pl = GObject::globalPlayers[pid];
        if (NULL != pl)
            pl->deleteGold(gold);
        playerId = GetNextSection(playerIds, ',');
    }
    ret = 0;
    Stream st(SPEP::DELETEGOLD);
    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}
void addRechargeScore(LoginMsgHdr& hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    std::string playerIds;
    UInt32 score;
    CHKKEY();
    br>>playerIds;
    br>>score;
    UInt16 serverNo = 0;
    if(cfg.merged)
        br >> serverNo;
 
    UInt8 ret = 1;
    INFO_LOG("GMADDCHARGESCORE: %s, %u", playerIds.c_str(), score);
    std::string playerId = GetNextSection(playerIds, ',');
    while (!playerId.empty())
    {
        UInt64 pid = atoll(playerId.c_str());
        if (cfg.merged)
            pid += (static_cast<UInt64>(serverNo) << 48);
        GObject::Player * pl = GObject::globalPlayers[pid];
        if (NULL != pl)
        {
            GObject::RechargeTmpl::instance().addScore(pl, score);
            GObject::RechargeTmpl::instance().sendScoreInfo(pl);
        }
        playerId = GetNextSection(playerIds, ',');
    }
    ret = 0;
    Stream st(SPEP::ADDRECHARGESCORE);
    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void GmHandlerFromBs(LoginMsgHdr &hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
    st.init(SPEP::GMHANDLERFROMBS,0x01);
    std::string playerNameList;
    std::string cmd;
    CHKKEY();
    br>>playerNameList;
    br>>cmd;
	if(cmd.empty())
		return;
    StringTokenizer stk(playerNameList,"%");
    st<<playerNameList;
    std::string result="";
	if(stk.count() == 0)
	{
		result = "0";
		GameMsgHdr hdr(0x306, WORKER_THREAD_NEUTRAL, NULL, strlen(cmd.c_str())+1);
		GLOBAL().PushMsg(hdr, (void *)cmd.c_str());
	}
	else
	{
		for(StringTokenizer::Iterator it=stk.begin();it!=stk.end();it++)
		{
			std::string playerName =*it;
			GObject::Player *player=GObject::globalNamedPlayers[playerName];
			if(player==NULL)
			{
				result+="1 ";
			}
			else
			{
				result+="0 ";
				GameMsgHdr hdr(0x306, player->getThreadId(), player, strlen(cmd.c_str())+1);
				GLOBAL().PushMsg(hdr, (void *)cmd.c_str());
			}
		}
		result=result.substr(0,result.length()-1);
	}
    st<<result;
    st<<Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void PlayerIDAuth( LoginMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 type = 0;
	UInt64 pid;
    CHKKEY();
	br >> type >> pid;
	GObject::Player * player = GObject::globalPlayers[pid];
	Stream st(SPEP::PLAYERIDAUTH);
	st << type << pid;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);

	if(player == NULL)
		return;

	PLAYER_DATA(player, wallow) = type;
	DB1().PushUpdateData("UPDATE `player` SET `wallow`=%u WHERE `id`=%"I64_FMT"u", type, pid);
	player->sendWallow();
}

void MailFromBs(LoginMsgHdr &hdr,const void * data)
{
    BinaryReader br(data, hdr.msgHdr.bodyLen);
    std::string playerName;
    std::string title;
    std::string content;
    CHKKEY();
    br>>playerName;
    br>>title;
    br>>content;
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        serverNameToGlobalName(playerName, serverNo);
    }
    GObject::Player *pl= GObject::globalNamedPlayers[playerName];
    Stream st;
    st.init(SPEP::MAILFROMBS, 0x01);
    st<<playerName;
    if(pl==NULL)
    {
        st<<"1"<<Stream::eos;
        //st<<static_cast<UInt32>(1)<<Stream::eos;
        NETWORK()->SendMsgToClient(hdr.sessionID,st);
        return;
    }
    st<<"0"<<Stream::eos;
    //st<<static_cast<UInt32>(0)<<Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
	pl->GetMailBox()->newMail(NULL,1, title,content);
}

void BanChatFromBs(LoginMsgHdr &hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
    st.init(SPEP::BANCHATFROMBS,0x01);
    std::string playerNameList;
    UInt32 time;
    CHKKEY();
    br>>playerNameList;
    br>>time;
    UInt16 serverNo = 0;
    if(cfg.merged)
    {
        br>>serverNo;
    }
    StringTokenizer stk(playerNameList,"%");
    st<<playerNameList;
	std::string result="";
    for(StringTokenizer::Iterator it=stk.begin();it!=stk.end();it++)
    {
        std::string playerName =*it;
        serverNameToGlobalName(playerName, serverNo);
        GObject::Player *player=GObject::globalNamedPlayers[playerName];
        if(player==NULL)
        {
            result+="1 ";
        }
        else
        {
            if(time!=0)
            {
                if(player->getBuffData(PLAYER_BUFF_BANCHAT,TimeUtil::Now())>0)
                {
                    result+="2 ";
                }
                else
                {
                    player->setBuffData(PLAYER_BUFF_BANCHAT,time);
                    result+="0 ";
                }
            }
            else
            {
                player->setBuffData(PLAYER_BUFF_BANCHAT,time);
                result+="0 ";
            }
        }
    }
    result=result.substr(0,result.length()-1);
    st<<result;
    st<<Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void AddItemFromBs(LoginMsgHdr &hdr,const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
	Stream st;
	st.init(SPEP::ADDITEMFROMBS,0x01);
	std::string playerNameList;
	std::string content;
	std::string title;
	UInt32 money[4] = {0};
	UInt32 moneyType[4] = {GObject::MailPackage::Tael, GObject::MailPackage::Coupon, GObject::MailPackage::Gold, GObject::MailPackage::Achievement};
	UInt16 nums = 0;
	UInt8 bindType = 1;
    CHKKEY();
	br>>playerNameList>>title>>content>>money[0]>>money[1]>>money[2]>>money[3]>>nums>>bindType;
	StringTokenizer stk(playerNameList,"%");
	st << playerNameList;
	std::string result="";
	GObject::MailPackage::MailItem *item = new(std::nothrow) GObject::MailPackage::MailItem[nums + 5];
	if(item == NULL)
		return;
	UInt8 count = {0};
	memset(item, 0, sizeof(GObject::MailPackage::MailItem) * (nums + 5));
    INFO_LOG("GM[%s]: %u, %u, %u, %u", __PRETTY_FUNCTION__, money[0], money[1], money[2], money[3]);
	for(UInt32 i = 0; i < nums; i ++)
	{
		br>>item[i].id>>count;
		item[i].count = count;
        INFO_LOG("GM[%s]: %u, %u", __PRETTY_FUNCTION__, item[i].id, count);
	}
    UInt16 serverNo = 0;
    if(cfg.merged)
    {
        br>>serverNo;
    }
	for(UInt32 i = 0; i < 4; i ++)
	{
		if(money[i] == 0)
			continue;
		item[nums].id = moneyType[i];
		item[nums++].count = money[i];
	}
	for(StringTokenizer::Iterator it=stk.begin();it!=stk.end();it++)
	{
		std::string playerName =*it;
        serverNameToGlobalName(playerName, serverNo);
		GObject::Player *player=GObject::globalNamedPlayers[playerName];
		if(player==NULL)
		{
			result+="1 ";
		}
		else
		{
			GObject::MailItemsInfo itemsInfo(item, BackStage, nums);
			GObject::Mail *pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFD0000, true, &itemsInfo);
			if(pmail != NULL)
			{
				GObject::mailPackageManager.push(pmail->id, item, nums, bindType == 1);
				result +="0 ";

                player->moneyLog(2, money[2], money[1], money[0], money[3]);
			}
			else
			{
				result +="2 ";
			}
		}
	}
	result=result.substr(0,result.length()-1);
	st<<result;
	st<<Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);

	SAFE_DELETE(item);

}

void AddItemFromBsById(LoginMsgHdr &hdr,const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
	Stream st;
	st.init(SPEP::ADDITEMFROMBSBYID,0x01);
	std::string playerIDList;
	std::string content;
	std::string title;
	UInt32 money[4] = {0};
	UInt32 moneyType[4] = {GObject::MailPackage::Tael, GObject::MailPackage::Coupon, GObject::MailPackage::Gold, GObject::MailPackage::Achievement};
	UInt16 nums = 0;
	UInt8 bindType = 1;
    CHKKEY();
	br>>playerIDList>>title>>content>>money[0]>>money[1]>>money[2]>>money[3]>>nums>>bindType;
	StringTokenizer stk(playerIDList,"%");
	st << playerIDList;
	std::string result="";
	GObject::MailPackage::MailItem *item = new(std::nothrow) GObject::MailPackage::MailItem[nums + 5];
	if(item == NULL)
		return;
	UInt8 count = {0};
	memset(item, 0, sizeof(GObject::MailPackage::MailItem) * (nums + 5));
    INFO_LOG("GM[%s]: %u, %u, %u, %u", __PRETTY_FUNCTION__, money[0], money[1], money[2], money[3]);
	for(UInt32 i = 0; i < nums; i ++)
	{
		br>>item[i].id>>count;
		item[i].count = count;
        INFO_LOG("GM[%s]: %u, %u", __PRETTY_FUNCTION__, item[i].id, count);
	}
    UInt16 serverNo = 0;
    if(cfg.merged)
    {
        br>>serverNo;
    }
    for(UInt32 i = 0; i < 4; i ++)
	{
		if(money[i] == 0)
			continue;
		item[nums].id = moneyType[i];
		item[nums++].count = money[i];
	}
	for(StringTokenizer::Iterator it=stk.begin();it!=stk.end();it++)
	{
		UInt64 playerID = strtoll((*it).c_str(),NULL, 10);
        if(cfg.merged)
        {
            playerID += (static_cast<UInt64>(serverNo) << 48);
        }
		GObject::Player *player=GObject::globalPlayers[playerID];
		if(player==NULL)
		{
			result+="1 ";
		}
		else
		{
			GObject::MailItemsInfo itemsInfo(item, BackStage, nums);
			GObject::Mail *pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFD0000, true, &itemsInfo);
			if(pmail != NULL)
			{
				GObject::mailPackageManager.push(pmail->id, item, nums, bindType == 1);
				result +="0 ";
                player->moneyLog(2, money[2], money[1], money[0], money[3]);
			}
			else
			{
				result +="2 ";
			}
		}
	}
	result=result.substr(0,result.length()-1);
	st<<result;
	st<<Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);

	SAFE_DELETE(item);

}
void BattleReportReq(LoginMsgHdr& hdr, const void * data)
{
	if(hdr.msgHdr.bodyLen < sizeof(UInt32))
		return;
	UInt32 battleId = *reinterpret_cast<const UInt32 *>(data);
	std::vector<UInt8> *r = Battle::battleReport[battleId];
	if(r == NULL)
		return;
	NETWORK()->SendMsgToClient(hdr.sessionID, &(*r)[0], r->size());
}

void ServerOnlineNum(LoginMsgHdr& hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
	Stream st;
	st.init(SPEP::ONLINE,0x01);
    CHKKEY();
	st<<SERVER().GetTcpService()->getOnlineNum();
	st<<Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void ServerOnlinePFNum(LoginMsgHdr& hdr, const void * data)
{
#ifdef _FB
#else
	BinaryReader br(data,hdr.msgHdr.bodyLen);
	Stream st;
	st.init(SPEP::ONLINEPF,0x01);
    size_t off = st.size();
    st << static_cast<UInt8>(0);
#ifndef _WIN32
	CHKKEY();
    UInt32 nums[MAX_DOMAIN] = {0,};
    GObject::dclogger.getOnline(nums);

    UInt8 count = 0;
    for (UInt8 i = 0; i < MAX_DOMAIN; ++i)
    {
        if (nums[i])
        {
            st << (i+1);
            st << nums[i];
            ++count;
        }
    }
    st.data<UInt8>(off) = count;
#endif //_WIN32
	st<<Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
#endif
}

void SetLevelFromBs(LoginMsgHdr& hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::SETLEVEL,0x01);
    UInt64 id;
    UInt8 level;
    CHKKEY();
    br>>id;
    br>>level;
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        id += (static_cast<UInt64>(serverNo) <<  48);
    }

    INFO_LOG("GM[%s]: %"I64_FMT"u, %u", __PRETTY_FUNCTION__, id, level);

    UInt8 ret = 1;
    GObject::Player * pl = GObject::globalPlayers[id];
    if (pl)
    {
        if (level > LEVEL_MAX)
            level = LEVEL_MAX;
        GameMsgHdr msg(0x322, pl->getThreadId(), pl, sizeof(level));
        GLOBAL().PushMsg(msg, &level);
        ret = 0;
    }

    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void AddItemToAllFromBs(LoginMsgHdr &hdr,const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
	Stream st;
	st.init(SPEP::ADDITEMTOALL,0x01);
	std::string content;
	std::string title;
    UInt8 pf = 0;
	UInt32 money[4] = {0};
	UInt32 moneyType[4] = {GObject::MailPackage::Tael, GObject::MailPackage::Coupon, GObject::MailPackage::Gold, GObject::MailPackage::Achievement};
	UInt16 nums = 0;
	UInt8 bindType = 1;
    CHKKEY();
	br>>pf>>title>>content>>money[0]>>money[1]>>money[2]>>money[3]>>nums>>bindType;
	std::string result="";
	GObject::MailPackage::MailItem *item = new(std::nothrow) GObject::MailPackage::MailItem[nums + 5];
	if(item == NULL)
		return;
	UInt8 count = {0};
	memset(item, 0, sizeof(GObject::MailPackage::MailItem) * (nums + 5));

    INFO_LOG("GM[%s]: %u, %u, %u, %u", __PRETTY_FUNCTION__, money[0], money[1], money[2], money[3]);
	for(UInt32 i = 0; i < nums; i ++)
	{
		br>>item[i].id>>count;
		item[i].count = count;
        INFO_LOG("GM[%s]: %u, %u", __PRETTY_FUNCTION__, item[i].id, count);
	}
	for(UInt32 i = 0; i < 4; i ++)
	{
		if(money[i] == 0)
			continue;
		item[nums].id = moneyType[i];
		item[nums++].count = money[i];
	}

    UInt16 serverNo = 0;
    if(cfg.merged)
    {
        br>>serverNo;
    }

    for (GObject::GlobalPlayers::iterator it = GObject::globalPlayers.begin(), end = GObject::globalPlayers.end(); it != end; ++it)
	{
		GObject::Player *player=it->second;
		if(player==NULL || (serverNo != 0 && serverNo != (static_cast<UInt16>(player->getId()>>48))))
		{
			result+="1 ";
		}
		else
		{
            if (!pf || (pf && player->isOnline() && atoi(player->getDomain())==pf))
            {
                GObject::MailItemsInfo itemsInfo(item, BackStage, nums);
                GObject::Mail *pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFD0000, true, &itemsInfo);
                if(pmail != NULL)
                {
                    GObject::mailPackageManager.push(pmail->id, item, nums, bindType == 1);
                    result +="0 ";
                    player->moneyLog(2, money[2], money[1], money[0], money[3]);
                }
                else
                {
                    result +="2 ";
                }
            }
		}
	}
	result=result.substr(0,result.length()-1);
	st<<result;
	st<<Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);

	SAFE_DELETE(item);
}

void MailVIPFromBs(LoginMsgHdr &hdr,const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
	Stream st;
	st.init(SPEP::MAILVIP,0x01);
	std::string content;
	std::string title;
    UInt8 viplmin = 0;
    UInt8 viplmax = 0;
	UInt32 money[4] = {0};
	UInt32 moneyType[4] = {GObject::MailPackage::Tael, GObject::MailPackage::Coupon, GObject::MailPackage::Gold, GObject::MailPackage::Achievement};
	UInt16 nums = 0;
	UInt8 bindType = 1;
    CHKKEY();
	br>>title>>content>>viplmin>>viplmax>>money[0]>>money[1]>>money[2]>>money[3]>>nums>>bindType;
	std::string result="";
	GObject::MailPackage::MailItem *item = new(std::nothrow) GObject::MailPackage::MailItem[nums + 5];
	if(item == NULL)
		return;
	UInt8 count = {0};
	memset(item, 0, sizeof(GObject::MailPackage::MailItem) * (nums + 5));

    INFO_LOG("GM[%s]: %u, %u, %u, %u", __PRETTY_FUNCTION__, money[0], money[1], money[2], money[3]);
	for(UInt32 i = 0; i < nums; i ++)
	{
		br>>item[i].id>>count;
		item[i].count = count;
        INFO_LOG("GM[%s]: %u, %u", __PRETTY_FUNCTION__, item[i].id, count);
	}
	for(UInt32 i = 0; i < 4; i ++)
	{
		if(money[i] == 0)
			continue;
		item[nums].id = moneyType[i];
		item[nums++].count = money[i];
	}

    for (GObject::GlobalPlayers::iterator it = GObject::globalPlayers.begin(), end = GObject::globalPlayers.end(); it != end; ++it)
	{
		GObject::Player* player=it->second;
		if(player==NULL)
		{
			result+="1 ";
		}
		else
		{
            if (player->getVipLevel() < viplmin || player->getVipLevel() > viplmax)
                continue;

			GObject::MailItemsInfo itemsInfo(item, BackStage, nums);
            GObject::Mail *pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFD0000, true, &itemsInfo);
			if(pmail != NULL)
			{
				GObject::mailPackageManager.push(pmail->id, item, nums, bindType == 1);
				result +="0 ";
                player->moneyLog(2, money[2], money[1], money[0], money[3]);
			}
			else
			{
				result +="2 ";
			}
		}
	}
	result=result.substr(0,result.length()-1);
	st<<result;
	st<<Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
	SAFE_DELETE(item);
}

void SetPropsFromBs(LoginMsgHdr &hdr,const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::SETPROPS,0x01);
    UInt64 id;
    UInt32 pexp;
    UInt32 prestige;
    UInt32 honor;
    CHKKEY();
    br>>id;
    br>>pexp;
    br>>prestige; // 声望
    br>>honor; // 荣誉
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        id += (static_cast<UInt64>(serverNo) << 48);
    }

    INFO_LOG("GM[%s]: %"I64_FMT"u, %u, %u, %u", __PRETTY_FUNCTION__, id, pexp, prestige, honor);

    UInt8 ret = 1;
    GObject::Player * pl = GObject::globalPlayers[id];
    if (pl)
    {
        struct Props
        {
            UInt32 pexp;
            UInt32 prestige;
            UInt32 honor;
        } props;

        props.pexp = pexp;
        props.prestige = prestige;
        props.honor = honor;

        GameMsgHdr msg(0x321, pl->getThreadId(), pl, sizeof(props));
        GLOBAL().PushMsg(msg, &props);
        ret = 0;
    }
    st << ret << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void SetMoneyFromBs(LoginMsgHdr &hdr,const void * data)
{
    return; // XXX: 取消这个功能 yangyoufa@ 19/12/11 21:04:59
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::SETMONEY,0x01);
    UInt64 id;
    std::string token;
    UInt8 type;
    UInt32 gold;
    UInt32 tael;
    UInt32 coupon;
    UInt32 achievement;

    CHKKEY();
    br>>id;
    br>>token;
    br>>type;
    br>>gold;
    br>>tael;
    br>>coupon;
    br>>achievement;
    INFO_LOG("GM[%s]: %"I64_FMT"u, %s, %u, %u, %u, %u, %u",
            __PRETTY_FUNCTION__, id, token.c_str(), type, gold, tael, coupon, achievement);

    st<<id;

    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        id += (static_cast<UInt64>(serverNo) << 48);
    }

    st<<type;

    UInt8 ret = 0;
    UInt8 retcode = 1;
    GObject::Player * pl = GObject::globalPlayers[id];
    if (pl)
    {
        struct Money
        {
            UInt8 type;
            UInt32 gold;
            UInt32 tael;
            UInt32 coupon;
            UInt32 achievement;
        } money;

        money.type = type;
        money.gold = gold;
        money.tael = tael;
        money.coupon = coupon;
        money.achievement = achievement;

#ifndef _WIN32
        if (cfg.GMCheck)
        {
            initMemcache();
            if (memcinited)
            {
                size_t len = 0;
                size_t tlen = 0;
                unsigned int flags = 0;
                char key[MEMCACHED_MAX_KEY] = {0};

                int retry = 3;
                memcached_return rc;
                len = snprintf(key, sizeof(key), "%s", token.c_str());
                while (retry)
                {
                    --retry;
                    char* rtoken = memcached_get(&memc, key, len, &tlen, &flags, &rc);
                    if (rc == MEMCACHED_SUCCESS && rtoken)
                    {
                        ret = 0;
                        free(rtoken);

                        GameMsgHdr msg(0x323, pl->getThreadId(), pl, sizeof(money));
                        GLOBAL().PushMsg(msg, &money);
                        retcode = 0;

                        break;
                    }
                    else
                    {
                        ret = 3;
                        usleep(500);
                    }
                }

                rc = memcached_delete(&memc, key, len, (time_t)0);
                if (rc == MEMCACHED_SUCCESS)
                {
                    //err += "delete key error.";
                }

                if (ret)
                {
                    TRACE_LOG("token: %s, ret: %u, rc: %u", token.c_str(), ret, rc);
                    uninitMemcache();
                    initMemcache();
                }
            }
        }
        else
#endif //_WIN32
        {
            GameMsgHdr msg(0x323, pl->getThreadId(), pl, sizeof(money));
            GLOBAL().PushMsg(msg, &money);
            retcode = 0;
        }
    }

    st << retcode << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void LoadLuaFromBs(LoginMsgHdr &hdr, const void * data)
{
    UInt16 reloadFlag = 0xFFFF;
    GameMsgHdr hdr1(0x1EE, WORKER_THREAD_WORLD, NULL, sizeof(reloadFlag));
    GLOBAL().PushMsg(hdr1, &reloadFlag);
}

void SetVIPLFromBs(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::SETVIPL,0x01);
    UInt64 id;
    UInt8 lv;
    CHKKEY();
    br>>id;
    br>>lv;
    st<<id;
    st<<lv;
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        id += (static_cast<UInt64>(serverNo) << 48);
    }

    INFO_LOG("GM[%s]: %"I64_FMT"u, %u", __PRETTY_FUNCTION__, id, lv);

    UInt8 ret = 1;
    GObject::Player * pl = GObject::globalPlayers[id];
    if (pl)
    {
        GameMsgHdr msg(0x324, pl->getThreadId(), pl, sizeof(lv));
        GLOBAL().PushMsg(msg, &lv);
        ret = 0;
    }
    st << ret << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void ClearTaskFromBs(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::CLSTASK,0x01);
    UInt64 id;
    UInt8 type;
    CHKKEY();
    br>>id;
    br>>type;
    st<<id;

    INFO_LOG("GM[%s]: %"I64_FMT"u, %u", __PRETTY_FUNCTION__, id, type);

    UInt8 ret = 1;
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        id += (static_cast<UInt64>(serverNo) << 48);
    }
    GObject::Player * pl = GObject::globalPlayers[id];
    if (pl)
    {
        if (type == 3)
            pl->buildClanTask(true);
        else
        {
            GameMsgHdr msg(0x325, pl->getThreadId(), pl, sizeof(type));
            GLOBAL().PushMsg(msg, &type);
        }
        ret = 0;
    }
    st << ret << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}
void ClearTaskAllFromBs(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st(SPEP::CLSTASKALL);
    UInt8 type;
    CHKKEY();
    br>>type;

    INFO_LOG("GM[%s]: %u", __PRETTY_FUNCTION__, type);

    UInt8 ret = 1;
    std::unordered_map<UInt64, GObject::Player*>& pm = GObject::globalPlayers.getMap(); 
    std::unordered_map<UInt64, GObject::Player*>::iterator iter;
    for (iter = pm.begin(); iter != pm.end(); ++iter)
    {
        GObject::Player* pl = iter->second;
        if (pl)
        {
            if (type == 3)
                pl->buildClanTask(true); 
            else
            {
                GameMsgHdr msg(0x325, pl->getThreadId(), pl, sizeof(type));
                GLOBAL().PushMsg(msg, &type);
            }
            ret = 0;
        }
    }
    st << ret << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}


void reqSaleOnOffFromBs(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::SALE_ONOFF,0x1);
    UInt8 flag;
    CHKKEY();
    br >> flag;

    UInt8 ret = flag;
    if(flag < 2)
        GObject::gSaleMgr.setOnOff(flag);
    else
        ret = GObject::gSaleMgr.getOnOff();

    st << ret << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void PlayerInfoFromBs(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::PLAYERINFO,0x1);
    UInt8 type;
    CHKKEY();
    br >> type;
    UInt16 serverNo = 0;

    GObject::Player* player = NULL;
    if (type == 1)
    {
        UInt64 pid;
        br >> pid;
        if(cfg.merged)
        {
            br>>serverNo;
            pid += (static_cast<UInt64>(serverNo) << 48);
        }
        player = GObject::globalPlayers[pid];
    }
    else if (type == 2)
    {
        std::string playerName;
        br >> playerName;
        if(cfg.merged)
        {
            br>>serverNo;
            serverNameToGlobalName(playerName, serverNo);
        }
        player = GObject::globalNamedPlayers[playerName];
    }
    else
        return;

    if (player)
    {
        if(cfg.merged)
        {
            UInt64 playerId = player->getId();
            playerId = (playerId & 0x0000ffffffffffff);
            st << playerId;
        }
        else
        {
            st << player->getId();
        }
        st << player->getNameNoSuffix(player->getName());
        st << player->GetLev();
        st << player->getCountry();
        st << player->GetClass();
        st << static_cast<UInt8>(player->IsMale()?0:1);
        st << static_cast<UInt8>(player->getVipLevel());
        st << player->getTael();
        st << player->getGold();
        st << player->getCoupon();
        if (player->getClan())
        {
            st << player->getClan()->getId();
            st << player->getClan()->getName();
        }
        else
        {
            st << static_cast<UInt32>(0);
            st << "";
        }

        if(player->getLockExpireTime() > 0)
        {
            if(player->getLockExpireTime() <= TimeUtil::Now())
                st << static_cast<UInt8>(0);
            else
                st << static_cast<UInt8>(1);
        }
        else
            st << static_cast<UInt8>(0);
        if(player->getBuffData(PLAYER_BUFF_BANCHAT)!=0)
            st << static_cast<UInt8>(1);
        else
            st << static_cast<UInt8>(0);
        UInt8 freeCnt, goldCnt;
        GObject::playerCopy.getCount(player, &freeCnt, &goldCnt, true);
        st << freeCnt << goldCnt;
        UInt8 fcnt = GObject::frontMap.getCount(player);
        st << static_cast<UInt8>(GObject::FrontMap::getFreeCount()-(fcnt&0xf));
        st << static_cast<UInt8>(GObject::FrontMap::getGoldCount(player->getVipLevel())-((fcnt&0xf0)>>4));
        st << static_cast<UInt8>(PLAYER_DATA(player, dungeonCnt));
        st << static_cast<UInt8>(GObject::Dungeon::getExtraCount(player->getVipLevel()));
    }

    st << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void WBossMgrFromBs(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::WBOSS,0x1);
    UInt8 lvl;
    CHKKEY();
    br >> lvl;
    st << Stream::eos;
    GObject::worldBoss.bossAppear(lvl, true);
}

void AddFighterFromBs(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::ADDFIGHTER,0x1);
    UInt64 playerId = 0;
    UInt16 fgtid = 0;

    CHKKEY();
    br >> playerId;
    br >> fgtid;

    INFO_LOG("GM[%s]: %"I64_FMT"u, %u", __PRETTY_FUNCTION__, playerId, fgtid);

    UInt8 ret = 0;
    GObject::Fighter * fgt = GObject::globalFighters[fgtid];
    if(fgt == NULL)
        ret = 1;

    GObject::Player* player = GObject::globalPlayers[playerId];
    if (player == NULL)
        ret = 1;

    player->takeFighter(fgtid, true);

    st << playerId << fgtid << ret;
    st << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void GetMoneyFromBs(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
	st.init(SPEP::GETMONEY,0x1);
    UInt32 date = 0;

    CHKKEY();
    br >> date;

    UInt32 now = TimeUtil::Now();

    int today[7] = {
        TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-6, now)),
        TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-5, now)),
        TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-4, now)),
        TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-3, now)),
        TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-2, now)),
        TimeUtil::GetYYMMDD(TimeUtil::SharpDay(-1, now)),
        TimeUtil::GetYYMMDD(now),
    };

    bool found = false;
    for (int i = 0; i < 7; ++i)
    {
        if (today[i] == (int)date)
        {
            st << GObject::World::_moneyIn[i][0].gold;
            st << GObject::World::_moneyIn[i][1].gold;
            found = true;
            break;
        }
    }

    if (!found)
        st << static_cast<UInt32>(0) << static_cast<UInt32>(0);

    st << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

inline bool player_enum(GObject::Player* p, int)
{
    if (!p->isOnline())
    {
        p->setSysDailog(true);
    }
    else
    {
        if(GObject::World::getSysDailogPlatform() == SYS_DIALOG_ALL_PLATFORM || GObject::World::getSysDailogPlatform() == atoi(p->getDomain()))
        {
            Stream st(REP::SYSDAILOG);
            st << Stream::eos;
            p->send(st);
        }
        else
        {
            p->setSysDailog(true);
        }
    }
    return true;
}

void SysDailog(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    UInt8 platform = 0;
    br >> platform;
#if 0
    Stream st(REP::SYSDAILOG);
    st << Stream::eos;
	NETWORK()->Broadcast(st);
#endif
    GObject::World::setSysDailogPlatform(platform);
	GObject::globalPlayers.enumerate(player_enum, 0);
}
void SysUpdate(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    UInt8 show = 0;
    br >> show;

    Stream st(REP::SYSDAILOG);
    st << static_cast<UInt8>(1);
    st << static_cast<UInt8>(0);
    st << show;
    st << (char*)VERSION;
    st << Stream::eos;
	NETWORK()->Broadcast(st);

    Stream st1(SPEP::SYSUPDATE);
    st1 << static_cast<UInt8>(0) << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st1);

}


void PwdInfo(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    UInt64 playerId = 0;
    br >> playerId;

    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        playerId += (static_cast<UInt64>(serverNo) << 48);
    }
	GObject::Player * pl= GObject::globalPlayers[playerId];
    if (!pl)
        return;

    const GObject::SecondPWDInfo& info = pl->getPWDInfo();
    Stream st(SPEP::PWDINFO);
    UInt8 onoff = info.secondPWD.empty() ? 0 : 1;
    st << onoff;
    if (onoff)
    {
        st << info.secondPWD << info.isLocked << info.errCount << info.questionForPWD << info.answerForPWD;
    }
    st << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void PwdReset(LoginMsgHdr &hdr, const void * data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    UInt64 playerId = 0;
    br >> playerId;
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        playerId += (static_cast<UInt64>(serverNo) << 48);
    }
	GObject::Player * pl= GObject::globalPlayers[playerId];
    if (!pl)
        return;

    Stream st(SPEP::PWDRESET);
    st << pl->deactiveSecondPWD("", true) << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

#if 0
void JasonParse(LoginMsgHdr& hdr, const void* data)
{
#ifdef _FB
#else
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();

    std::string json;
    br >> json;
    jsonParser(json, hdr.sessionID);
#endif
}
#endif

void SetCFriend(LoginMsgHdr& hdr, const void* data)
{
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    std::string name1;
    std::string name2;
    br >> name1;
    br >> name2;
    if(cfg.merged)
    {
        UInt16 serverNo = 0;
        br>>serverNo;
        serverNameToGlobalName(name1, serverNo);
        serverNameToGlobalName(name2, serverNo);
    }
	GObject::Player * pl1 = GObject::globalNamedPlayers[name1];
	GObject::Player * pl2 = GObject::globalNamedPlayers[name2];
    UInt8 ret = 1;
    if (pl1 && pl2)
    {
        pl1->addCFriend(pl2);
        pl1->tellCFriendLvlUp(pl2->GetLev());
        pl2->tellCFriendLvlUp(pl1->GetLev());
        ret = 0;
    }

    Stream st(SPEP::CFRIEND);
    st << ret << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

UInt8 SwitchSecDC(UInt32 val)
{
    // 设置是否开启安全DCLogger，返回设置是否成功(0 成功，非0 失败)
    cfg.setSecDCLog(val? true:false);
    if (cfg.isTestPlatform)
        cfg.setSecDCLogTest(val? true:false);
    return 0;
}

UInt8 SwitchAutoForbid(UInt32 val)
{
    cfg.setAutoForbid(val? true:false);
    return 0;
}

inline bool player_enum_2(GObject::Player* pl, int type)
{
    switch(type)
    {
        case 1:
            pl->SetVar(GObject::VAR_DRAGONKING_STEP, 0);
            pl->SetVar(GObject::VAR_DRAGONKING_STEP4_COUNT, 0);
            break;
        case 2:
            {
                UInt32 btime = GObject::GVAR.GetVar(GObject::GVAR_LUCKYSTAR_BEGIN);
                UInt32 etime = GObject::GVAR.GetVar(GObject::GVAR_LUCKYSTAR_END);
                UInt32 ltime = pl->GetVar(GObject::VAR_LUCKYSTAR_LOGIN_TIME);
                if(ltime && (ltime < btime || ltime > etime))
                    pl->SetVar(GObject::VAR_LUCKYSTAR_LOGIN_TIME, 0);
            }
            break;
        default:
            return false;
    }
    return true;
}

void GMCmd(LoginMsgHdr& hdr, const void* data)
{
    // 接受后台传来的GM指令，返回0表示操作成功，非0为操作失败
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();

    UInt16 id = 0;
    UInt32 val = 0;

    br >> id;
    br >> val;
    UInt8 result = 1;
    switch (id)
    {
        case 0x01:
            result = SwitchSecDC(val);
            break;
        case 0x02:
            result = SwitchAutoForbid(val);
            break;
        case 0x03:
            {
                UInt32 endTime = 0;
                UInt32 flag = 0;
                br >> endTime >> flag;
                //大闹龙宫的flag暂时只为1,2,3,4,5,6
                //10:聚宝盆
                if(endTime < val || flag >= GObject::DRAGONKING_MAX)
                    result = 1;
                else
                {
                    if (flag != 10)
                    {
                        if(flag != GObject::GVAR.GetVar(GObject::GVAR_DRAGONKING_ACTION))
                            GObject::globalPlayers.enumerate(player_enum_2, 1);
                        GObject::GVAR.SetVar(GObject::GVAR_DRAGONKING_BEGIN, val);
                        GObject::GVAR.SetVar(GObject::GVAR_DRAGONKING_END, endTime);
                        GObject::GVAR.SetVar(GObject::GVAR_DRAGONKING_ACTION, flag);
                    }
                    else
                    {
                        GObject::GVAR.SetVar(GObject::GVAR_TREASURE_BEGIN, val);
                        GObject::GVAR.SetVar(GObject::GVAR_TREASURE_END, endTime); 
                        GObject::GVAR.SetVar(GObject::GVAR_TREASURE_ACTION, flag);   
                    }
                    result = 0;
                }
            }
        case 0x04:
            cfg.setAutoKick(val == 1? true:false);
            result = 0;
            break;
        default:
            break;
    }


    Stream st(SPEP::GMCMD);
    st << result << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);

}

void AddDiscountFromBs(LoginMsgHdr& hdr, const void* data)
{
    // GM增加限时限购活动
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    GData::Discount discount;
    br >> discount.discountType;
    br >> discount.exType;
    br >> discount.exValue;
    br >> discount.beginTime;
    br >> discount.endTime;
    GData::store.clearSpecialDiscountFromBS(discount.discountType);
    UInt16 count;
    br >> count;
    UInt8 result = 0;
    for (UInt8 i = 0; i < count; ++i)
    {
        br >> discount.itemID;            // UInt16
        br >> discount.limitCount;        // UInt32
        br >> discount.priceOriginal;     // UInt16
        br >> discount.priceDiscount;     // UInt16
        result = GData::store.addSpecialDiscountFromBS(discount);
        if (result)
        {
            break;
        }
    }
    GData::store.storeDiscount();
    GData::store.makePacket();

    Stream st(SPEP::ADDDISCOUNT);
    st << result << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void QueryDiscountFromBs(LoginMsgHdr& hdr, const void* data)
{
    // GM查询限时限购活动
	BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    Stream st (SPEP::QUERYDISCOUNT);
    for (UInt8 type = 4; type < 7; ++type)
    {
        UInt8 i = 0;
        const GData::Discount* discount = GData::store.getDiscount(type, i++);
        if (discount)
        {
            st << type;
            st << discount->discountType;
            st << discount->exType;
            st << discount->exValue;
            st << discount->beginTime;
            st << discount->endTime;


        }
        else
            continue;
        st << GData::store.getDiscountItemsCount(type);
        while(discount)
        {
            st << discount->itemID;            // UInt16
            st << discount->limitCount;        // UInt32
            st << discount->priceOriginal;     // UInt16
            st << discount->priceDiscount;     // UInt16
            discount = GData::store.getDiscount(type, i++);
        }
    }
    st << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void ClearDiscountFromBs(LoginMsgHdr& hdr, const void* data)
{
    // GM清空限时限购活动
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    GData::store.clearSpecialDiscountFromBS();
    GData::store.storeDiscount();
    GData::store.makePacket();

    Stream st (SPEP::CLEARDISCOUNT);
    st << static_cast<UInt8>(0);
    st << Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void QueryRealAwardInfo(LoginMsgHdr& hdr, const void* data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    Stream st(SPEP::REALAWARDINFO);
    GObject::realItemAwardMgr.getInfo(st);
    st << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID, st);
}

void AddRealAward(LoginMsgHdr& hdr, const void* data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();

    UInt8 type = 0;
    UInt8 ret = 0;
    br >> type;

    Stream st(SPEP::ADDREALAWARD);
    st << type;
    switch(type)
    {
        case 1:
            {
                UInt32 id = 0;
                UInt32 cd = 0;
                std::string card_no;
                std::string card_psw;
                br >> id >> cd >> card_no >> card_psw;
                if(id == 0 || cd == 0 || card_no.length() == 0 || card_psw.length() == 0)
                    ret = 0;
                else if(GObject::realItemAwardMgr.addAward(id, cd, card_no, card_psw))
                    ret = 1;
                else
                    ret = 2;
            }
            break;
        case 2:
            {
                UInt32 id = 0;
                br >> id;
                if(id == 0)
                    ret = 0;
                else if(GObject::realItemAwardMgr.delAward(id))
                    ret = 1;
                else
                    ret = 2;
            }
            break;
    }
    st << ret << Stream::eos;

    NETWORK()->SendMsgToClient(hdr.sessionID, st);
}

void AddClanAward(LoginMsgHdr& hdr, const void* data)
{
    // 发送物品给指定帮派仓库
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    UInt8 ret = 0;

    UInt32 clanId = 0;
    br >> clanId;
    GObject::Clan *clan = GObject::globalClans[clanId];

    Stream st(SPEP::ADDCLANAWARD);
    if(clan == NULL) 
    {
        st << ret << Stream::eos;
    }
    else
    {
        UInt16 num = 0;
        br >> num;
        st << num;
        UInt16 * itemId = new UInt16[num];
        UInt32 * itemCount = new UInt32[num];
        for (UInt16 i = 0; i < num; ++i)
        {
            br >> itemId[i];
            br >> itemCount[i];
            st << itemId[i] << clan->AddItem(itemId[i], itemCount[i]);
        }

        if(cfg.merged)
        {
            UInt16 serverNo = 0;
            br>>serverNo;
        }
        delete [] itemId;
        delete [] itemCount;

        st << ret << Stream::eos;
    }
    NETWORK()->SendMsgToClient(hdr.sessionID, st);
}

void ManualOpenTj(LoginMsgHdr& hdr, const void* data)
{
    //手动开启天劫
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();

    UInt16 level = 0;
    UInt8 force = 0;
    br >> level;
    br >> force;
    UInt8 ret = GObject::Tianjie::instance().manualOpenTj(level, force);

    Stream st(SPEP::MANUALOPENTJ);

    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID, st);
}

void SHStageOnOff(LoginMsgHdr& hdr, const void* data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();

    struct OnOffData
    {
        UInt32 begin;
        UInt32 end;
        int	sessionID;
    } onOff = {0};
    br >> onOff.begin >> onOff.end;
    onOff.sessionID = hdr.sessionID;

    GameMsgHdr imh(0x1AC, WORKER_THREAD_WORLD, NULL, sizeof(onOff));
    GLOBAL().PushMsg(imh, &onOff);
}

void QuerySHStageOnOff(LoginMsgHdr& hdr, const void* data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();
    GObject::SHOnOffTime onOff = GObject::shStageMgr.getSHOnOffTime();

    Stream st(SPEP::QUERYSHSTAGEONOFF);
    st << onOff._timeBegin << onOff._timeEnd << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID, st);
}

void ControlActivityOnOff(LoginMsgHdr& hdr, const void* data)
{
    BinaryReader br(data, hdr.msgHdr.bodyLen);
    CHKKEY();

    UInt32 begin = 0, end = 0;
    UInt8 type = 0;
    br >> type >> begin >> end;
    UInt8 ret = 0;
    if(type == 1)
    {   //充值幸运星活动
        GObject::GVAR.SetVar(GObject::GVAR_LUCKYSTAR_END, begin);
        GObject::GVAR.SetVar(GObject::GVAR_LUCKYSTAR_BEGIN, end);
        GObject::globalPlayers.enumerate(player_enum_2, 2);
        ret = 1;
    }
    else if (type == 2)
    {
    }

    Stream st(SPEP::ACTIVITYONOFF);
    st << ret << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID, st);
}

void QueryOneActivityOnOff(LoginMsgHdr& hdr, const void* data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    CHKKEY();

    UInt8 type = 0;
    br >> type;

    UInt32 begin = 0, end = 0;
    if(type == 1)
    {
        begin = GObject::GVAR.GetVar(GObject::GVAR_LUCKYSTAR_END);
        end = GObject::GVAR.GetVar(GObject::GVAR_LUCKYSTAR_BEGIN);
    }
    else if (type == 2)
    {
    }

    Stream st(SPEP::QUERYACTIVITYONOFF);
    st << type << begin << end << Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID, st);
}

#endif // _LOGINOUTERMSGHANDLER_H_


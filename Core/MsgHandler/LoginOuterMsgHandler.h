#ifndef _LOGINOUTERMSGHANDLER_H_
#define _LOGINOUTERMSGHANDLER_H_

#include "MsgTypes.h"
#include "MsgFunc.h"

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
#include "Battle/BattleReport.h"
#include "MsgHandler/GMHandler.h"
#include "Common/BinaryReader.h"
#include "Common/SHA1Engine.h"
#include "Common/StringTokenizer.h"

struct UserDisconnectStruct
{
	MESSAGE_DEF(0xFF);
};

struct UserReconnectStruct
{
	UInt64 _userid;
	MESSAGE_DEF1(0x01, UInt64, _userid);
};

struct UserLoginStruct
{
	UInt64 _userid;
	UInt32 _lang;
	typedef Array<UInt8, 36> HashValType;
	UInt8 _hashval[36];
	std::string _server;

	MESSAGE_DEF4(0x10, UInt64, _userid, UInt32, _lang, HashValType, _hashval, std::string, _server);
};

struct NewUserStruct
{
	std::string _name;
	UInt8 _class;

	MESSAGE_DEF2(0x11, std::string, _name, UInt8, _class);
};


void UserDisconnect( GameMsgHdr& hdr, UserDisconnectStruct& )
{
	MSG_QUERY_PLAYER(player);
	player->SetSessionID(-1);
	GameMsgHdr imh(0x200, player->getThreadId(), player, 0);
	GLOBAL().PushMsg(imh, NULL);
}

struct UserLogonRepStruct
{
	UInt32 _result;
	std::string _name;

	MESSAGE_DEF2(0x10, UInt32, _result, std::string, _name);
};

inline UInt8 doLogin(Network::GameClient * cl, UInt64 pid, UInt32 hsid, GObject::Player *& player, bool kickOld = true)
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
				DB().PushUpdateData("DELETE FROM `locked_player` WHERE `player_id` = %"I64_FMT"u", pid);
			}
			else
				return 3;
		}
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
				Network::GameClient * cl2 = static_cast<Network::GameClient *>(c.get());
				player->SetSessionID(-1);
				player->testBattlePunish();
				static UInt8 kick_pkt[4] = {0x00, 0x00, 0xFF, 0x0E};
				cl2->send(kick_pkt, 4);
				cl2->SetPlayer(NULL);
				cl2->pendClose();
				res = 4;
			}
			else
			{
				return 2;
			}
		}
	}
	player->SetSessionID(hsid);
	cl->SetPlayer(player);
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
		conn->closeConn();

	Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
	GObject::Player * player = NULL;
	if(doLogin(cl, ur._userid, hdr.sessionID, player, false) != 0)
	{
		Stream st(0x01);
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
		conn->closeConn();

    // TODO: 可能是这个地方导致登陆后不久断线
	UInt32 now = TimeUtil::Now();
#if 0
	UInt32 loginTime = *reinterpret_cast<UInt32*>(ul._hashval + 12);
	if(cfg.GMCheck && (now + 300 < loginTime || now > loginTime + 600))
	{
		UserLogonRepStruct rep;
		rep._result = 2;
		NETWORK()->SendMsgToClient(conn.get(), rep);
		conn->pendClose();
		return;
	}
#endif

	SHA1Engine sha1;
	sha1.update(ul._hashval + 8, 4);
	sha1.update(cfg.cryptKey1.c_str(), cfg.cryptKey1.length());
	sha1.update(ul._hashval, 4);
	sha1.update(&ul._userid, sizeof(UInt64));
	sha1.update(ul._hashval + 12, 4);
	sha1.update(cfg.cryptKey2.c_str(), cfg.cryptKey2.length());
	sha1.update(ul._hashval + 4, 4);

	std::vector<UInt8> buf = sha1.digest();
	UInt8 res;

	if(!cfg.GMCheck || memcmp(&buf.front(), ul._hashval + 16, 20) == 0)
	{
		Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
		GObject::Player * player = NULL;
		UInt64 pid = ul._userid;
		if(cfg.merged)
		{
			pid |= (getServerNo(ul._server) << 48);
		}
		res = doLogin(cl, pid, hdr.sessionID, player);
		UInt8 flag = 0;
		if(res == 0)
		{
			GameMsgHdr imh(0x201, player->getThreadId(), player, 1);
			GLOBAL().PushMsg(imh, &flag);
		}
		else if(res == 4)
		{
			flag = 1;
			GameMsgHdr imh(0x201, player->getThreadId(), player, 1);
			GLOBAL().PushMsg(imh, &flag);
			res = 0;
		}
	}
	else
		res = 2;
	if(res > 0)
	{
		UserLogonRepStruct rep;
		rep._result = res;
		NETWORK()->SendMsgToClient(conn.get(), rep);
	}
	if(res == 2 || res == 3)
		conn->pendClose();
}

struct NewUserRepStruct
{
	UInt32 _result;
	std::string _name;

	MESSAGE_DEF2(0x11, UInt32, _result, std::string, _name);
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
	TcpConnection conn = NETWORK()->GetConn(hdr.sessionID);
	if(conn.get() == NULL)
		return;

	trimName(nu._name);

#if 0
	bool noWallow = (nu._country & 0x80) > 0;
	nu._country &= 0x03;
#else
	bool noWallow = false;
#endif

	if(nu._name.empty() || nu._name.length() > 15 || (strncmp(nu._name.c_str(), "ERROR_", 6) == 0))
	{
		NewUserRepStruct rep;
		rep._result = 2;
		NETWORK()->SendMsgToClient(conn.get(), rep);
		return;
	}

	// TODO: name filter
#if 0
	if(nu._country < 1 || nu._country > 2/* || !StringFilter::Checkup(nu._name)*/)
	{
		conn->closeConn();
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
			return;

#if 0
		// UInt16 loc = (nu._country == 1) ? 0x0005 : 0x1002;
        // 先将_country移动置放置大地图id的高字节的高四位,然后+1初始化玩家的据点id
#else
		UInt16 loc = 0x0002;
#endif

        UInt8 country = COUNTRY_NEUTRAL; // XXX: 低级玩家暂时规为中立

		PLAYER_DATA(pl, name) = newname;
		PLAYER_DATA(pl, country) = country;
		PLAYER_DATA(pl, wallow) = noWallow ? 0 : 1;
		PLAYER_DATA(pl, created) = TimeUtil::Now();

		UInt32 fgtId = nu._class + 1;
		PLAYER_DATA(pl, newGuild) = (0x3FFFFFFFull << NEWGUILDSTEP_MAX);

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

			DB().PushUpdateData("INSERT INTO `player` (`id`, `name`, `country`, `location`, `lineup`, `wallow`) VALUES (%" I64_FMT "u, '%s', %u, %u, '%u,12', %u)", pl->getId(), nu._name.c_str(), country, loc, fgtId, PLAYER_DATA(pl, wallow));

			DBLOG().PushUpdateData("insert into register_states(server_id,player_id,player_name, reg_time) values(%u,%"I64_FMT"u, '%s', %u)", cfg.serverLogId, pl->getId(), pl->getName().c_str(), TimeUtil::Now());

			GObject::globalPlayers.add(pl);
			GObject::newPlayers.add(pl);
			GObject::globalNamedPlayers.add(newname, pl);
			res = 0;

			pl->SetSessionID(hdr.sessionID);
			Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
			cl->SetPlayer(pl);

            pl->GetPackage()->AddItem(18, 1);

			CountryEnterStruct ces(false, 1, loc);
			GameMsgHdr imh(0x1F0, country, pl, sizeof(CountryEnterStruct));
			GLOBAL().PushMsg(imh, &ces);
			UInt8 flag = 0;
			GameMsgHdr imh2(0x201, country, pl, 1);
			GLOBAL().PushMsg(imh2, &flag);
			UInt32 gold = GObject::prepaid.pop(pl->getId());
			if(gold > 0)
			{
				GameMsgHdr hdr(0x2F0, country, pl, sizeof(UInt32));
				GLOBAL().PushMsg(hdr, &gold);
			}
		}
	}

	if(res == 0)
		return;

	NewUserRepStruct rep;
	rep._result = res;
	NETWORK()->SendMsgToClient(conn.get(), rep);
}

void onUserRecharge( LoginMsgHdr& hdr, const void * data )
{
    BinaryReader brd(data, hdr.msgHdr.bodyLen);

    UInt64 player_Id;
    UInt32 gold;
    //std::string md5Key="veryCD";
    //std::string md5value="";
    brd>>player_Id;
    brd>>gold;
    /**
    brd>>md5value;

    std::stringstream ss;
    ss<<order_Id<<player_Id<<gold;
    std::string key=ss.str()+md5Key;

    HMACEngine<MD5Engine> md5(md5Key);
    MD5Engine  md5Engine;

    md5Engine.update(key);

    std::string s2=md5Engine.digestToHex(md5Engine.digest());
    if(s2==md5value)
    **/

    UInt32 ret;
    GObject::Player * player=GObject::globalPlayers[player_Id];
    if(player!=NULL)
    {
		GameMsgHdr hdr(0x2F0, player->getThreadId(), player, sizeof(UInt32));
		GLOBAL().PushMsg(hdr, &gold);
        ret=0;
    }
    else
	{
		GObject::prepaid.push(player_Id, gold);
        ret=2;
	}


    Stream st;
    st.init(0x00, 0x01);
    st<<ret<<Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);

    return;
    //return failed message.
}

void WorldAnnounce( LoginMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 type;
	std::string msg;
	brd >> type >> msg;
	Stream st(0xF7);
	st << type << msg << Stream::eos;
	NETWORK()->Broadcast(st);
}


void OnKickUser(LoginMsgHdr& hdr,const void * data)
{
    Stream st;
    st.init(0x01,0x01);
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt64 playerId;
    br>>playerId;
    st<<playerId;
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
    st.init(0x02,0x01);
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt64 playerId;
    UInt64 expireTime;
    br>>playerId;
    br>>expireTime;
    st<<playerId;
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
            DB().PushUpdateData("REPLACE INTO `locked_player`(`player_id`, `lockExpireTime`) VALUES(%"I64_FMT"u, %u)", playerId, expireTime);
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
    st.init(0x03,0x01);
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    UInt64 playerId;
    br>>playerId;
    st<<playerId;
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
            DB().PushUpdateData("DELETE FROM `locked_player` WHERE `player_id` = %"I64_FMT"u", pl->getId());
            st<<static_cast<UInt32>(0);
        }
    }
    st<<Stream::eos;
    NETWORK()->SendMsgToClient(hdr.sessionID,st);
}

void GmHandlerFromBs(LoginMsgHdr &hdr,const void * data)
{
    BinaryReader br(data,hdr.msgHdr.bodyLen);
    Stream st;
    st.init(0x04,0x01);
    std::string playerNameList;
    std::string cmd;
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
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 type = 0;
	UInt64 pid;
	brd >> type >> pid;
	GObject::Player * player = GObject::globalPlayers[pid];
	Stream st(0xFD);
	st << type << pid;
	NETWORK()->SendMsgToClient(hdr.sessionID,st);

	if(player == NULL)
		return;

	PLAYER_DATA(player, wallow) = type;
	DB().PushUpdateData("UPDATE `player` SET `wallow`=%u WHERE `id`=%"I64_FMT"u", type, pid);
	player->sendWallow();
}

void MailFromBs(LoginMsgHdr &hdr,const void * data)
{
    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    std::string playerName;
    std::string title;
    std::string content;
    brd>>playerName;
    brd>>title;
    brd>>content;
    GObject::Player *pl= GObject::globalNamedPlayers[playerName];
    Stream st;
    st.init(0x05, 0x01);
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
    st.init(0x06,0x01);
    std::string playerNameList;
    UInt32 time;
    br>>playerNameList;
    br>>time;
    StringTokenizer stk(playerNameList,"%");
    st<<playerNameList;
	std::string result="";
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
	st.init(0x07,0x01);
	std::string playerNameList;
	std::string content;
	std::string title;
	UInt32 money[5] = {0};
	UInt32 moneyType[5] = {GObject::MailPackage::Coin, GObject::MailPackage::Tael, GObject::MailPackage::Coupon, GObject::MailPackage::Gold, GObject::MailPackage::Achievement};
	UInt16 nums = 0;
	UInt8 bindType = 1;
	br>>playerNameList>>title>>content>>money[0]>>money[1]>>money[2]>>money[3]>>money[4]>>nums>>bindType;
	StringTokenizer stk(playerNameList,"%");
	st << playerNameList;
	std::string result="";
	GObject::MailPackage::MailItem *item = new(std::nothrow) GObject::MailPackage::MailItem[nums + 5];
	if(item == NULL)
		return;
	UInt8 count = {0};
	memset(item, 0, sizeof(GObject::MailPackage::MailItem) * (nums + 5));
	for(UInt32 i = 0; i < nums; i ++)
	{
		br>>item[i].id>>count;
		item[i].count = count;
	}
	for(UInt32 i = 0; i < 5; i ++)
	{
		if(money[i] == 0)
			continue;
		item[nums].id = moneyType[i];
		item[nums++].count = money[i];
	}
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
			GObject::MailItemsInfo itemsInfo(item, BackStage, nums);
			GObject::Mail *pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
			if(pmail != NULL)
			{
				GObject::mailPackageManager.push(pmail->id, item, nums, bindType == 1);
				result +="0 ";
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

#endif // _LOGINOUTERMSGHANDLER_H_

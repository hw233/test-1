#ifndef _WORLDOUTERMSGHANDLER_H_
#define _WORLDOUTERMSGHANDLER_H_

#include "MsgTypes.h"
#include "MsgFunc.h"
#include "MsgID.h"

#include "Common/Serialize.h"

#include "Server/WorldServer.h"
#include "Server/OidGenerator.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "GObject/Leaderboard.h"
#include "GObject/CountryBattle.h"
#include "GObject/Clan.h"
#include "GObject/ClanBattle.h"
#include "GObject/ClanManager.h"
#include "GObject/Player.h"
#include "GObject/Fighter.h"
#include "GObject/Package.h"
#include "GObject/SaleMgr.h"
#include "GObject/AthleticsRank.h"
#include "Battle/BattleReport.h"
#include "GData/Store.h"
#include "GObject/Mail.h"
#include "GObject/ClanManager.h"
#include "GObject/PracticePlace.h"
#include "Common/Stream.h"
#include "Common/BinaryReader.h"
#include "GData/Money.h"
#include "GObject/TownDeamon.h"
#include "GObject/ClanRankBattle.h"

#ifdef _ARENA_SERVER
#include "GObject/GameServer.h"
#include "GObject/Stage.h"
#include "Network/ArenaClient.h"
#include "Battle/BattleReport.h"
#else
#include "GObject/Arena.h"
#endif

#include <mysql.h>

struct ClanListReq
{
	UInt8 _type;
	UInt16 _startidx;
	UInt8 _count;
    UInt8 _flag;      // 0-???? 1-%?? 2-ȫ??
	std::string _name;
	MESSAGE_DEF5(REQ::CLAN_LIST, UInt8, _type, UInt16, _startidx, UInt8, _count, UInt8, _flag, std::string, _name);
};

struct ClanInfoReq
{
	MESSAGE_DEF(REQ::CLAN_SELF);
};

struct ClanCreateReq
{
	std::string _name;
	std::string _contact;
	std::string _purpose;
	MESSAGE_DEF3(REQ::CLAN_CREATE, std::string, _name, std::string, _contact, std::string, _purpose);
};

struct ClanApplyReq
{
	UInt32 _id;
	MESSAGE_DEF1(REQ::CLAN_APPLY, UInt32, _id);
};

struct ClanModifyReq
{
	UInt8 _op;
	std::string _text;
	MESSAGE_DEF2(REQ::CLAN_INFO_CHANGE, UInt8, _op, std::string, _text);
};

struct ClanBattleReportReq
{
	UInt32 _clanId;
	UInt32 _rptId;
	MESSAGE_DEF2(REQ::CLAN_BATTLE_INFO, UInt32, _clanId, UInt32, _rptId);
};

struct ClanGetDynamicMsgReq
{
	UInt8 _type;
	UInt16 _start;
	UInt16 _count;
	MESSAGE_DEF3(REQ::CLAN_INFO, UInt8, _type, UInt16, _start, UInt16, _count);
};

struct ClanQueryReq
{
	UInt32 _id;
	MESSAGE_DEF1(REQ::CLAN_REQ_OTHER, UInt32, _id);
};

struct ClanQuery2Req
{
	UInt8 _type;
	MESSAGE_DEF1(REQ::CLAN_PLAYER_LIST, UInt8, _type);
};

struct ClanItemHistoryReq
{
    UInt16 _startId;
    UInt8 _count;
    MESSAGE_DEF2(REQ::CLAN_PACKAGE_RECORD, UInt16, _startId, UInt8, _count);
};

#if 0
struct ClanPersonalRewardReq
{
	MESSAGE_DEF(REQ::CLAN_BUILD);
};
#endif

struct ClanRewardReq
{
	MESSAGE_DEF(REQ::CLANREWARD);
};

struct ClanPersonalRewardOpReq
{
	UInt8 _type;
	UInt32 _allocTime;
	UInt16 _itemId;
	MESSAGE_DEF3(REQ::GET_CLANREWARD, UInt8, _type, UInt32, _allocTime, UInt16, _itemId);
};

struct ClanAllocRecordReq
{
	MESSAGE_DEF(REQ::CLANALLOCRECORD);
};

struct StoreListReq
{
	UInt8 _type;
	MESSAGE_DEF1(REQ::STORE_LIST, UInt8, _type);
};

struct LeaderboardReq
{
	UInt8 _type;
	UInt32 _id;
	MESSAGE_DEF2(REQ::SORT_LIST, UInt8, _type, UInt32, _id);
};

struct OwnLeaderboardReq
{
	UInt32 _id;
	MESSAGE_DEF1(REQ::SORT_PERSON, UInt32, _id);
};

struct PlayerIdReq
{
	std::string _name;
	MESSAGE_DEF1(REQ::CHECK_USER, std::string, _name);
};

struct BattleReportReq
{
	UInt32 _reportId;
	MESSAGE_DEF1(REQ::FIGHT_REPORT, UInt32, _reportId);
};

struct BattleReportReq2
{
	UInt32 _reportId;
    UInt8 type;
	MESSAGE_DEF2(REQ::FIGHT_REPORT2, UInt32, _reportId, UInt8, type);
};

struct SaleListReq
{
	UInt16 _start;
	UInt16 _count;
//	UInt8  _search;
	std::string _name;
	UInt8  _req;
//	UInt8  _sort;
	UInt8  _color;
    UInt8  _career;
	UInt8  _eqType;
	MESSAGE_DEF7(REQ::SALE_LIST, UInt16, _start, UInt16, _count, std::string, _name, UInt8, _req, UInt8, _color, UInt8, _career, UInt8, _eqType);
};

struct SaleBuyAndCancelReq
{
	UInt8  _token;
	UInt32 _id;
	MESSAGE_DEF2(REQ::SALE_OP, UInt8, _token, UInt32, _id);
};

struct PracticePlaceReq
{
	MESSAGE_DEF(REQ::PLACE_INFO);
};

struct PracticeListReq
{
	UInt8 _type;
    UInt8 _pageno;
    UInt8 _pagenum;
	MESSAGE_DEF3(REQ::PRACTICE_PAGE_CHANGE, UInt8, _type, UInt8, _pageno, UInt8, _pagenum);
};

struct PracticeChallengeReq
{
	UInt8 _type;
    UInt8 _idx;
    std::string _name;
	MESSAGE_DEF3(REQ::PLACE_ROB, UInt8, _type, UInt8, _idx, std::string, _name);
};

struct PracticePayReq
{
    UInt8 _place;
    UInt8 _slot;
    UInt8 _type;
    UInt8 _ptype;
	MESSAGE_DEF4(REQ::PLACE_OCCUPY, UInt8, _place, UInt8, _slot, UInt8, _type, UInt8, _ptype);
};

struct PracticeSitDownReq
{
    UInt8 _type;
    UInt32 _fgtid;
	MESSAGE_DEF2(REQ::PRACTICE_START, UInt8, _type, UInt32, _fgtid);
};

struct PracticeStopReq
{
	MESSAGE_DEF(REQ::PRACTICE_STOP);
};

struct AthleticsListReq
{
    UInt16 _type;
	MESSAGE_DEF1(REQ::ARENA_INFO, UInt16, _type);
};

struct AthleticsGetAwardReq
{
    UInt8 _type;
    MESSAGE_DEF1(REQ::ATHLETICS_GET_AWARD, UInt8, _type);
};

struct LuckyDrawRankListReq
{
    MESSAGE_DEF(REQ::LUCKY_RANK);
};

#if 0
struct AthleticsChallengeReq
{
	std::string _name;
	MESSAGE_DEF1(REQ::ATHLETICS_CHALLENGE, std::string, _name);
};
#endif

struct ArenaInfoReq
{
	UInt8 type;
	UInt8 flag;
    UInt16 start;
    UInt8 len;
	MESSAGE_DEF4(REQ::SERVER_ARENA_INFO, UInt8, type, UInt8, flag, UInt16, start, UInt8, len);
};

struct ArenaLeaderBoardReq
{
	MESSAGE_DEF(REQ::SERVER_ARENA_LB);
};

void OnClanListReq( GameMsgHdr& hdr, ClanListReq& clr )
{
	MSG_QUERY_PLAYER(player);
	GObject::Clan * clan = NULL;
    switch(clr._type)
	{
	case 0:
		GObject::clanCache.listAll(player, clr._startidx, clr._count, clr._flag);
		return;
	case 1:
		clan = GObject::globalNamedClans[player->getCountry()][player->fixName(clr._name)];
		break;
	case 2:
		clan = GObject::globalOwnedClans[player->getCountry()][player->fixName(clr._name)];
		break;
	default:
		return;
	}
	if(clan == NULL)
	{
		GObject::clanCache.search(player, clr._name, clr._flag);
		return;
	}
	clan->searchMatch(player);
}

void OnClanInfoReq( GameMsgHdr& hdr, ClanInfoReq& )
{
	MSG_QUERY_PLAYER(player);
	GObject::Clan * clan = player->getClan();
	if(clan == NULL)
	{
		Stream st(REP::CLAN_REQ_USER);
		st << static_cast<UInt8>(1);
		st << Stream::eos;
		player->send(st);
		return;
	}

	clan->sendInfo(player);
}

void OnClanCreateReq( GameMsgHdr& hdr, ClanCreateReq& ccr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	extern void trimName(std::string& str);
	trimName(ccr._name);
	std::string strNametmp(ccr._name);
    if(player->GetLev() < 30)
    {
		Stream st(REP::CLAN_CREATE);
		st << static_cast<UInt8>(4) << Stream::eos;
		player->send(st);
		return;
    }
    if(player->getCountry() > 1)
    {
		Stream st(REP::CLAN_CREATE);
		st << static_cast<UInt8>(5) << Stream::eos;
		player->send(st);
		return;
    }
	if(player->getClan() != NULL || ccr._name.length() > 15 || GObject::globalGlobalNamedClans[player->fixName(ccr._name)] != NULL)
	{
		Stream st(REP::CLAN_CREATE);
		st << static_cast<UInt8>(1) << Stream::eos;
		player->send(st);
		return;
	}
	if(player->getTael() < GData::moneyNeed[GData::CLAN_CREATE].tael) // XXX:
	{
		Stream st(REP::CLAN_CREATE);
		st << static_cast<UInt8>(3) << Stream::eos;
		player->send(st);
		return;
	}
	UInt32 id = IDGenerator::gClanOidGenerator.ID();
    while(GObject::globalClans[id] != NULL)
        id = IDGenerator::gClanOidGenerator.ID();

	if(cfg.merged && player->getId() >= 0x1000000000000ull)
	{
		UInt32 svno = static_cast<UInt32>(player->getId() >> 48);
		id |= (svno << 24);
	}

	GObject::Clan * clan = new(std::nothrow) GObject::Clan(id, ccr._name);
	if(clan == NULL)
	{
		Stream st(REP::CLAN_CREATE);
		st << static_cast<UInt8>(4) << Stream::eos;
		player->send(st);
		return;
	}
	clan->setContact(ccr._contact, false);
	clan->setPurpose(ccr._purpose, false);
	clan->setFounder(player->getId());
	Stream st(REP::CLAN_CREATE);
	st << static_cast<UInt8>(0) << Stream::eos;
	player->send(st);
	clan->join(player, 4);
	UInt8 cny = player->getCountry();
	GObject::globalClans.add(clan->getId(), clan);
	GObject::globalClansByCountry[cny].add(clan->getId(), clan);
	//player->fixName(ccr._name);
	GObject::globalNamedClans[cny].add(ccr._name, clan);
	GObject::globalGlobalNamedClans.add(ccr._name, clan);
	GObject::clanCache.push(clan);

	char contact2[1024], purpose2[1024], name2[1024];
	mysql_escape_string(contact2, ccr._contact.c_str(), ccr._contact.length()>1022?1022:ccr._contact.length());
	mysql_escape_string(purpose2, ccr._purpose.c_str(), ccr._purpose.length()>1022?1022:ccr._purpose.length());
	mysql_escape_string(name2, strNametmp.c_str(), strNametmp.length()>1022?1022:strNametmp.length());
	DB5().PushUpdateData("INSERT INTO `clan` (`id`, `name`, `foundTime`, `founder`, `leader`, `construction`, `contact`, `purpose`, `level`) VALUES (%u, '%s', %u, %"I64_FMT"u, %"I64_FMT"u, 0, '%s', '%s', %u)", clan->getId(), name2, TimeUtil::Now(), player->getId(), player->getId(), contact2, purpose2, clan->getLev());
	ConsumeInfo ci(ClanCreate,0,0);
	player->useTael(GData::moneyNeed[GData::CLAN_CREATE].tael,&ci);
	clan->initBuildClan();
	SYSMSG_SEND(120, player);
	SYSMSG_SENDV(1020, player, clan->getName().c_str());

    int type = 0;
	GameMsgHdr hdr1(0x310, player->getThreadId(), player, sizeof(type));
	GLOBAL().PushMsg(hdr1, &type);
}

void OnClanApplyReq( GameMsgHdr& hdr, ClanApplyReq& car )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	GObject::Clan * clan = NULL;
	if(player->getClan() != NULL ||
		(clan = GObject::globalClansByCountry[player->getCountry()][car._id]) == NULL
		|| clan->isFull())
	{
		Stream st(REP::CLAN_JOIN_IN);
		st << static_cast<UInt8>(1) << Stream::eos;
		player->send(st);
		return;
	}

	UInt8 r = clan->apply(player);
	if(r == 0)
	{
		SYSMSG_SENDV(119, player, clan->getName().c_str());
		SYSMSG_SENDV(1019, player, clan->getName().c_str());
	}

	Stream st(REP::CLAN_JOIN_IN);
	st << r << Stream::eos;
	player->send(st);
}

void OnClanOpReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 op;
	brd >> op;
	UInt64 inviteeId = 0;
    UInt8  cls = 0;
	std::string inviteeName;
	bool r = false;
	GObject::Clan * clan = player->getClan();
	if(clan != NULL)
	{
		switch(op)
		{
		case 0:
			brd >> inviteeName;
			r = clan->invite(player, inviteeName);
			break;
		case 1:
			brd >> inviteeId;
			r = clan->kick(player, inviteeId);
			break;
		case 2:
			brd >> inviteeId;
			r = clan->accept(player, inviteeId);
			break;
		case 3:
			brd >> inviteeId;
			r = clan->decline(player, inviteeId);
			break;
		case 4:
			r = clan->leave(player);
			break;
		case 5:
			brd >> inviteeId;
			r = clan->handoverLeader(player, inviteeId);
			break;
        case 6:
            brd >> inviteeId >> cls;
            r = clan->setClanRank(player, inviteeId, cls);
            break;
        case 7:
            brd >> inviteeId;
            r = clan->setWatchmanId(inviteeId);
            break;
		}
	}
	Stream st(REP::CLAN_MEMBER_OPERATE);
	if (op == 0)
		st << op << static_cast<UInt8>(r ? 1 : 0) << inviteeName << Stream::eos;
	else
		st << op << static_cast<UInt8>(r ? 1 : 0) << inviteeId << Stream::eos;
	player->send(st);
}


void OnClanModifyReq( GameMsgHdr& hdr, ClanModifyReq& cmr )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	GObject::Clan * clan = player->getClan();
	Stream st(REP::CLAN_INFO_CHANGE);
	st << cmr._op;
	UInt8 r = 0;
	if(clan == NULL)
		r = 1;
	else
	{
		switch(cmr._op)
		{
		case 0:
			clan->setContact(cmr._text);
			break;
		case 1:
			clan->setAnnounce(cmr._text);
			break;
		default:
			r = 1;
			break;
		}
	}
	st << r << Stream::eos;
	player->send(st);
}

void OnClanQueryReq( GameMsgHdr& hdr, ClanQueryReq& cqr )
{
	MSG_QUERY_PLAYER(player);
	GObject::Clan * clan = GObject::globalClans[cqr._id];
	if(clan == NULL /*TODO: ¼ì²éÈËÊýÊÇ·ñ´ïµ½ÉÏÏÞ*/)
	{
		Stream st(REP::CLAN_REQ_ITEM);
		st << static_cast<UInt32>(0) << "" << "" << Stream::eos;
		player->send(st);
		return;
	}

	Stream st(REP::CLAN_REQ_ITEM);
	st << clan->getId() << clan->getContact() << clan->getPurpose() << Stream::eos;
	player->send(st);
}

void OnClanQuery2Req( GameMsgHdr& hdr, ClanQuery2Req& cqr )
{
	MSG_QUERY_PLAYER(player);
	GObject::Clan * clan = player->getClan();
	if(clan == NULL)
		return;

	if(cqr._type == 0)
		clan->listMembers(player);
	else
		clan->listPending(player);
}

void OnClanBattleReportReq( GameMsgHdr& hdr, ClanBattleReportReq& req )
{
	MSG_QUERY_PLAYER(player);
	GObject::Clan * clan = player->getClan();
	if (clan == NULL)
		return;
	GObject::ClanBattle * clanBattle = NULL;
	if (req._clanId == 0)
		clanBattle = GObject::clanManager.getRobBattleClan();
	else if (clan->getId() != req._clanId)
	{
		clan = GObject::globalClans[req._clanId];
		if (clan != NULL)
			clanBattle = clan->getClanBattle();
	}
	if (clanBattle == NULL)
		return;
	clanBattle->sendClanBattleReport(player, req._rptId);
}

#if 0
void OnClanPersonalRewardReq( GameMsgHdr& hdr, ClanPersonalRewardReq& )
{
	MSG_QUERY_PLAYER(player);
	if(player->getClan() == NULL)
		return;
	player->getClan()->sendPersonalRewardList(player);
}
#endif

void OnClanPracticePlaceOpReq(GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;

    GObject::Clan* clan = player->getClan();
    if(NULL == clan)
        return;

    if(clan->getOwner() != player)
        return;

	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 op;
    UInt16 money = 0;
	brd >> op;

    switch(op)
    {
    case 0:
        clan->sendPracticePlaceInfo(player);
        break;
    case 1:
        GObject::practicePlace.addSlot(player);
        break;
    case 2:
        break;
    case 3:
        brd >> money;
        GObject::practicePlace.setCharges(player, money);
        break;
    case 4:
        brd >> money;
        GObject::practicePlace.setProtCharges(player, money);
        break;
    }
}

void OnClanRewardReq(GameMsgHdr& hdr, ClanRewardReq&)
{
	MSG_QUERY_PLAYER(player);
	if(player->getClan() == NULL)
		return;
	player->getClan()->sendRepoInfo(player);
}

void OnClanPersonalRewardOpReq(GameMsgHdr& hdr, ClanPersonalRewardOpReq& cpror )
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	if(player->getClan() == NULL)
		return;
	player->getClan()->takeReward(player, cpror._type, cpror._allocTime, cpror._itemId);
}

void OnAllocateClanBattleRewardReq(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	if(player->getClan() == NULL || player->getClan()->getOwner() != player)
		return;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt64 playerId;
	brd >> playerId;
    GObject::Player * player2 = GObject::globalPlayers[playerId];
    if(player2 == NULL || player2->getClan() != player->getClan())
        return;
	UInt8 c;
	brd >> c;
	if(c == 0)
		return;
	std::map<UInt32, UInt8> items;
	for(UInt8 i = 0; i < c; ++ i)
	{
		UInt16 itemId;
		UInt8 itemNum;
		brd >> itemId >> itemNum;
		items[itemId] = itemNum;
	}
	player->getClan()->allocateRepoAsReward(player2, items);
	Stream st(REP::ALLOCATECLANREWARD);
	st << player2->getId() << static_cast<UInt8>(0) << Stream::eos;
	player->send(st);
}

void OnClanAllocRecordReq(GameMsgHdr& hdr, ClanAllocRecordReq&)
{
	MSG_QUERY_PLAYER(player);
	if(player->getClan() == NULL)
		return;
	player->getClan()->sendAllocRecords(player);
}

void OnClanGetDynamicMsgReq(GameMsgHdr& hdr, ClanGetDynamicMsgReq& req)
{
	MSG_QUERY_PLAYER(player);
	if (player->getClan() == NULL)
		return;
	player->getClan()->sendClanDynamicMsg(player, req._type, req._start, req._count);
}

void OnClanTechOpReq(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 op = 0;
	brd >> op;
	GObject::Clan * clan = player->getClan();
	if (clan != NULL)
	{
		switch (op)
		{
		case 0:
			clan->listTechs(player);
			break;
		case 1:
			{
				UInt8 techId;
				brd >> techId;
				clan->listTechDonators(player, techId);
			}
			break;
		case 2:
			{
				if(!player->hasChecked())
					return;
				if (clan->getClanBattle()->isInBattling())
				{
					player->sendMsgCode(0, 1317);
					return;
				}
				struct DonateItems
				{
					UInt8  skillId;
					UInt8 flag;
					UInt32 count;
					UInt8 ret;
				};
				DonateItems items;
				brd >> items.skillId >> items.flag >> items.count;
				if(!clan->checkDonate(player, items.skillId, items.flag, items.count))
					return;
				GameMsgHdr hdr(0x308, player->getThreadId(), player, sizeof(DonateItems));
				GLOBAL().PushMsg(hdr, &items);
			}
			break;
        case 5:
            {
                UInt8 skillId;
                brd >> skillId;
                clan->skillLevelUp(player, skillId);
            }
            break;
		}
	}

    switch (op)
    {
    case 3:
        {
            UInt8 skillId = 0;
            GameMsgHdr hdr1(0x313, player->getThreadId(), player, sizeof(skillId));
            GLOBAL().PushMsg(hdr1, &skillId);
        }
        break;
    case 4:
        {
            UInt8 skillId;
            brd >> skillId;

            GameMsgHdr hdr1(0x313, player->getThreadId(), player, sizeof(skillId));
            GLOBAL().PushMsg(hdr1, &skillId);
        }
        break;
    }

}

void OnClanPackageReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::Clan * clan = player->getClan();
	if (clan == NULL) return;

	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    brd >> op;

    switch(op)
    {
    case 0: //帮派仓库基础信息请求
        {
            clan->SendPackageInfo(player);
        }
        break;
    case 1: //帮派仓库列表请求
        {
            clan->SendItemList(player);
            clan->SendSelfItemList(player);
        }
        break;
    case 2: //分配仓库物品
        {
            UInt64 memId = 0;
            UInt16 itemId = 0;
            UInt16 itemNum = 0;
            brd >> memId >> itemId >> itemNum;
            clan->DistributeItem(player, memId, itemId, itemNum);
        }
        break;
    case 3: //领取个人福利
        {
            clan->GetWeal(player);
        }
        break;
    case 4: //领取个人奖励
        {
            clan->GetItems(player);
        }
        break;
    }
    //TODO
}

void OnItemHistoryReq( GameMsgHdr& hdr, ClanItemHistoryReq& req)
{
    MSG_QUERY_PLAYER(player);
    GObject::Clan* clan = player->getClan();
    if(clan == NULL) return;

    clan->SendItemHistory(player, req._startId, req._count);
}


void OnClanCityBattleReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	GObject::Clan * clan = player->getClan();
	if (clan == NULL) return;
	GObject::ClanBattle * clanBattle = clan->getClanBattle();
	UInt8 op = 0;
	brd >> op;
	switch (op)
	{
	case 0:
		{
			GObject::ClanBattle * blClanBattle = player->getClanBattle();
			UInt8 enterCount = clanBattle->getEnterBattleClanCount(player);
			UInt8 battleTime = clanBattle->getBattleTime();
			UInt8 battleNextTime = clanBattle->getNextBattleTime();
			UInt8 battlingStatus = clanBattle->getBattleStatus();
			UInt8 autobattleeffect = clan->getAutoBattleEffect();
			if (blClanBattle == NULL)
			{
				Stream st(REP::CLAN_BATTLE);
				st << static_cast<UInt8>(0) << static_cast<UInt8>((enterCount << 4) | (GObject::ClanCityBattle::getMaxEnterCount())) << autobattleeffect << battlingStatus << (battlingStatus > 1 ? battleNextTime : battleTime)  <<  static_cast<UInt8>(2) << Stream::eos;
				player->send(st);
			}
			else
			{
				GObject::ClanBattlePlayer * cbp = blClanBattle->getBattlePlayer(player);
				if (cbp == NULL)
				{
					ERROR_LOG("Cannot find the battler [%llu] in battle clan [%u]", player->getId(), blClanBattle->getOwnerClanId());
					return;
				}
				UInt16 loc = cbp->hold;
				{
					Stream st(REP::CLAN_BATTLE);
					st << static_cast<UInt8>(0) << static_cast<UInt8>((enterCount << 4) | (GObject::ClanCityBattle::getMaxEnterCount())) <<  autobattleeffect << battlingStatus << (battlingStatus > 1 ? battleNextTime : battleTime) << static_cast<UInt8>(1) << blClanBattle->getClanBattleType() << blClanBattle->getOwnerClanName() << clanBattle->getClanRelation(blClanBattle->getOwnerClan()) << loc << static_cast<UInt8>(blClanBattle->isClanHoldDestroy(loc) ? 1 : 0) << Stream::eos;
					player->send(st);
				}
				blClanBattle->listClanHoldPlayerInfo(player, loc);
				if (blClanBattle->isInBattling())
				{
					if (cbp->status != 1)
					{
						UInt32 now = TimeUtil::Now();
						UInt32 recoveTime = player->getBuffData(PLAYER_BUFF_CLANRECOVE, now);
						blClanBattle->notifyClanBattleRecoveData(player, (cbp->reliveNum >> 8) + 1, recoveTime > now ? recoveTime - now : 0);
					}
					blClanBattle->notifyClanBattleOverTime(player);
					blClanBattle->notifyClanBattleWinData(player);
					blClanBattle->notifyClanHoldEndurance(player);
					blClanBattle->notifyClanBattlePlayerCount(player);
					blClanBattle->notifyClanBattleRecoveData(player);
					blClanBattle->notifyBattleScore(player);
				}
				blClanBattle->notifyClanHoldPlayerInfo(player, 1);
				blClanBattle->cancelClanBattlerOffCheck(player);
			}
		}
		break;
	case 1:
		{
			if(!player->hasChecked())
				return;
			UInt8 flag;
			brd >> flag;
			if (flag == 1)
				clanBattle->enterTotem(player);
			else
			{
				std::string clanName;
				brd >> clanName;
				GObject::ClanBattle * clanBattle = GObject::clanManager.getBattleClan(player, clanName);
				if (clanBattle == NULL)
				{
					Stream st(REP::CLAN_BATTLE);
					st << static_cast<UInt8>(1) << static_cast<UInt8>(2) << static_cast<UInt8>(2) << static_cast<UInt8>(1) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << clanName << Stream::eos;
					player->send(st);
				}
				else
					clanBattle->enterClanCity(player);
			}
		}
		break;
	case 2:
		{
			if(!player->hasChecked())
				return;
			UInt8 r = true;
			UInt8 flag;
			std::string clanName;
			brd >> flag >> clanName;
			GObject::Clan * allyClan = GObject::globalGlobalNamedClans[player->fixName(clanName)];
			if (allyClan == NULL)
			{
				//player->sendMsgCode(0, 2220);
				return;
			}
			if (clan == allyClan)
				r = false;
			if(!player->hasChecked())
				return;
			if (allyClan->getClanBattle()->isInBattling() || clan->getClanBattle()->isInBattling())
			{
				player->sendMsgCode(0, 1317);
				r = false;
			}
			if (!clan->hasClanAuthority(player, 5))
				r = false;
			if (r)
			{
				switch (flag)
				{
				case 1:
					{
						//????
						if (clan->hasAllyClan())
						{
							r = false;
							//player->sendMsgCode(0, 2211);
						}
						if (clan->hasEnemyClan(allyClan))
						{
							r = false;
							//player->sendMsgCode(0, 2230);
						}
						if (r && allyClan->hasAllyClan())
						{
							r = false;
							//player->sendMsgCode(0, 2212);
						}
						if (r)
						{
							GObject::Player * allyPlayer = allyClan->getOwner();
							if (allyPlayer != NULL)
							{
								SYSMSG(title, 350);
								SYSMSGV(content, 351, clan->getName().c_str(), player->getName().c_str());
								allyPlayer->GetMailBox()->newMail(player, 0x24, title, content, clan->getId());
							}
						}
					}
					break;
				case 2:
					{
						r = false;
						if (clan->delAllyClan(player, allyClan))
						{
							GObject::Player * allyPlayer = allyClan->getOwner();
							if (allyPlayer != NULL)
							{
								SYSMSG(title, 355);
								SYSMSGV(content, 354, clan->getName().c_str(), player->getName().c_str());
								allyPlayer->GetMailBox()->newMail(player, 0x26, title, content, clan->getId());
							}
							r = true;
						}
					}
					break;
				case 3:
					{
						//??b????
						if (clan->hasEnemyClan(allyClan))
						{
							r = false;
							//player->sendMsgCode(0, 2232);
						}
						if (clan->hasAllyClan(allyClan))
						{
							r = false;
							//player->sendMsgCode(0, 2231);
						}
						if (r)
							clan->addEnemyClan(player, allyClan);
					}
					break;
				case 4:
					{
						//????????
						if (clan->hasEnemyClan(allyClan))
							clan->delEnemyClan(player, allyClan);
					}
					break;
				default:
					r = false;
					break;
				}
			}
			if (!r)
			{
				Stream st(REP::CLAN_BATTLE);
				st << static_cast<UInt8>(2) << flag << static_cast<UInt8>(1) << clanName << Stream::eos;
				player->send(st);
			}
		}
		break;
	case 3:
		{
			if(!player->hasChecked())
				return;
			GObject::ClanBattle * blClanBattle = player->getClanBattle();
			if (blClanBattle == NULL)
				return;
			UInt16 buildID;
			brd >> buildID;
			Stream st(REP::CLAN_BATTLE);
			st << static_cast<UInt8>(3) << (blClanBattle->moveToHold(player, buildID) ? 0 : 1) << buildID << Stream::eos;
			player->send(st);
		}
		break;
	case 4:
		{
			if(!player->hasChecked())
				return;
			GObject::ClanBattle * blClanBattle = player->getClanBattle();
			if (blClanBattle == NULL)
				return;
			std::string deferName;
			UInt8 attackType;
			brd >> deferName >> attackType;
			if (attackType == 0)
				blClanBattle->attackNpc(player, deferName);
			else
				blClanBattle->attackPlayer(player, player->fixName(deferName));
			Stream st(REP::CLAN_BATTLE);
			st << static_cast<UInt8>(4) << Stream::eos;
			player->send(st);
		}
		break;
	case 5:
		{
			if (!clan->hasClanAuthority(player, 4))
				return;
			Stream st(REP::CLAN_BATTLE);
			st << static_cast<UInt8>(10) << clan->getClanBattle()->getNextBattleTime() << Stream::eos;
			player->send(st);
		}
		break;
	case 6:
		{
			if(!player->hasChecked())
				return;
			if (!clan->hasClanAuthority(player, 4))
				return;
			UInt8 openT;
			brd >> openT;
			if (clan->getClanBattle()->setNextBattleTime(player, openT))
			{
				Stream st(REP::CLAN_BATTLE);
				st << static_cast<UInt8>(10) << openT << Stream::eos;
				clan->broadcast(st);
			}
		}
		break;
	case 7:
		{
			if(!player->hasChecked())
				return;
			UInt8 r = 0;
			GObject::ClanBattle * blClanBattle = player->getClanBattle();
			if (blClanBattle != NULL && blClanBattle->leaveClanCity(player))
				r = 1;
			Stream st(REP::CLAN_BATTLE);
			st << static_cast<UInt8>(16) << r << Stream::eos;
			player->send(st);
		}
		break;
	case 8:
		{
			clan->notifyAllyClanInfo(player);
		}
		break;
	case 9:
		{
			if(!player->hasChecked())
				return;
			GObject::ClanBattle * blClanBattle = player->getClanBattle();
			if (blClanBattle == NULL)
				return;
			UInt8 recoveT;
			brd >> recoveT;
			blClanBattle->recoveBattlePlayer(player, recoveT);
		}
		break;
	}
}


void OnClanCityBattleListReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if (player->getClan() != NULL)
	{
		BinaryReader brd(data, hdr.msgHdr.bodyLen);
		UInt8 type = 0;
		brd >> type;
		if (type == 0)
		{
			UInt16 startId;
			UInt8 count;
			brd >> startId >> count;
			GObject::clanManager.listClans(player, startId, count);
		}
		else
		{
			std::string name;
			brd >> name;
			GObject::clanCache.search2(player, name);
		}
	}
}

void OnClanCopyReq (GameMsgHdr& hdr, const void * data )
{
    // TODO: 帮派副本系统的请求协议
    MSG_QUERY_PLAYER(player);
    if (!player->hasChecked())
        return;

	GObject::Clan * clan = player->getClan();
	if(clan == NULL)
	{
		Stream st(REP::CLAN_COPY);
		st << static_cast<UInt8>(0);
		st << Stream::eos;
		player->send(st);
		return;
	}

    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;
    UInt8 command = 0;
    brd >> command;
    UInt8 val = 0;
    switch(type)
    {
        case CLAN_COPY_TAB_INFO:
            // 请求帮派副本信息
            if (command == 0)
                clan->sendClanCopyInfo(player);
            else
            {
                brd >> val;
                clan->clanCopyOperate(player, type, command, val);
            }
            break;
        case CLAN_COPY_MEMBER_LIST_OP:
            // 帮派副本成员操作
            brd >> val;
            clan->clanCopyOperate(player, type, command, val);
            break;
        default:
                break;
    }
}

struct PlayerIdRep
{
	UInt64 id;
	std::string name;
	UInt8 cny;
	UInt8 sex;
	UInt8 office;
	UInt8 guard;
	MESSAGE_DEF6(REP::FIND_USER, UInt64, id, std::string, name, UInt8, cny, UInt8, sex, UInt8, office, UInt8, guard);
};

void OnStoreListReq( GameMsgHdr& hdr, StoreListReq& lr )
{
	MSG_QUERY_PLAYER(player);
	GData::store.sendList(lr._type, player);
}

void OnLeaderboardReq( GameMsgHdr& hdr, LeaderboardReq& lr )
{
	MSG_QUERY_PLAYER(player);
	Stream * st;
	if(GObject::leaderboard.hasUpdate(lr._id) && GObject::leaderboard.getPacket(lr._type, st))
	{
        if (!GObject::leaderboard.isSorting())
            player->send(*st);
	}
	else
	{
		UInt8 failed_packet[9] = {0x05, 0x00, 0xFF, REP::SORT_LIST, lr._type, 0x00, 0x00, 0x00, 0x00};
		player->send(failed_packet, 9);
	}
}

void OnOwnLeaderboardReq( GameMsgHdr& hdr, OwnLeaderboardReq& olr )
{
	MSG_QUERY_PLAYER(player);
	GObject::leaderboard.sendOwnRank(player, olr._id);
}

void OnPlayerIdReq( GameMsgHdr& hdr, PlayerIdReq& pir )
{
	MSG_QUERY_PLAYER(player);
	PlayerIdRep rep;
	GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(pir._name)];
	if(pl == NULL)
	{
		rep.id = 0;
		rep.cny = 0;
		rep.sex = 0;
		rep.office = 0;
		rep.guard = 0;
	}
	else
	{
		rep.id = pl->getId();
		rep.name = pl->getName();
		rep.cny = pl->getCountry();
		rep.sex = pl->IsMale() ? 0 : 1;
		rep.office = pl->getTitle();
		rep.guard = 0;
	}
	player->send(rep);
}

void OnBattleReportReq( GameMsgHdr& hdr, BattleReportReq& brr)
{
	MSG_QUERY_PLAYER(player);
	std::vector<UInt8> *r = Battle::battleReport[brr._reportId];
	if(r == NULL)
		return;
	player->send(&(*r)[0], r->size());
}

void OnBattleReportReq2( GameMsgHdr& hdr, BattleReportReq2& brr)
{
	MSG_QUERY_PLAYER(player);
    Stream st(REP::FIGHT_REPORT2);
    st << brr.type;
	std::vector<UInt8> *r = Battle::battleReport[brr._reportId];
	if(r == NULL)
		return;
    st.append(&(*r)[4], r->size() - 4);
    st << Stream::eos;
	player->send(st);
}

void OnSaleListReq( GameMsgHdr& hdr, SaleListReq& req )
{
	MSG_QUERY_PLAYER(player);
	GObject::gSaleMgr.requestSaleList(player, req._start, req._count, req._name, req._req, req._color, req._career, req._eqType);
}

void OnSaleBuyAndCancelReq( GameMsgHdr& hdr, SaleBuyAndCancelReq& req )
{
	MSG_QUERY_PLAYER(player);

    if(GObject::gSaleMgr.getOnOff() == 0)
    {
        return;
    }

	if (req._token == 0)
	{
		if(!player->hasChecked())
			return;
		GObject::gSaleMgr.buySale(player, req._id);
	}
	else if (req._token == 1)
		GObject::gSaleMgr.cancelSale(player, req._id);
}

void OnPracticePlaceReq( GameMsgHdr& hdr, PracticePlaceReq&)
{
	MSG_QUERY_PLAYER(player);
	GObject::practicePlace.getAllPlaceInfo(player);
}

void OnPracticeListReq( GameMsgHdr& hdr, PracticeListReq& req)
{
	MSG_QUERY_PLAYER(player);
	GObject::practicePlace.getList(player, req._type+1, req._pageno, req._pagenum);
}

void OnPracticeChallengeReq( GameMsgHdr& hdr, PracticeChallengeReq& req)
{
	MSG_QUERY_PLAYER(player);
	GObject::practicePlace.doChallenge(player, req._type+1, req._idx, req._name);
}

void OnPracticePayReq( GameMsgHdr& hdr, PracticePayReq& req)
{
	MSG_QUERY_PLAYER(player);
	GObject::practicePlace.pay(player, req._place+1, req._slot, req._type, 1, req._type==0?1:3,req._ptype);
}

void OnPracticeSitDownReq( GameMsgHdr& hdr, PracticeSitDownReq& req)
{
	MSG_QUERY_PLAYER(player);
    UInt32 fgts[1] = {req._fgtid};

    switch (req._type) {
        case 0:
            GObject::practicePlace.sitdown(player, fgts, 1);
        break;
        case 1:
            GObject::practicePlace.standup(player, fgts, 1);
        break;
    }
}

void OnPracticeStopReq( GameMsgHdr& hdr, PracticeStopReq& req)
{
	MSG_QUERY_PLAYER(player);
	GObject::practicePlace.stop(player);
    PopTimerEvent(player, EVENT_PLAYERPRACTICING, player->getId());
}

void OnAthleticsListReq( GameMsgHdr& hdr, AthleticsListReq& req)
{
	MSG_QUERY_PLAYER(player);
    GObject::gAthleticsRank.requestAthleticsList(player, req._type);
}

void OnAthleticsChallengeReq( GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;

    switch(type)
    {
#if 0
    case 0:
    case 1:
        GObject::gAthleticsRank.challenge(player, type);
        break;
#endif
    case 1:
        {
            std::string name;
            brd >> name;
            GObject::gAthleticsRank.challenge(player, name);
        }
        break;
    case 2:
        {
            std::string name;
            UInt8 rivalDifficulty = 0;
            brd >> name;
            brd >> rivalDifficulty;
            GObject::gAthleticsRank.challenge2(player, name, 3, rivalDifficulty);
        }
        break;
    }
}
void OnAthleticsPaging( GameMsgHdr& hdr, const void * data)
{
     MSG_QUERY_PLAYER(player);
     BinaryReader brd(data, hdr.msgHdr.bodyLen);
     UInt8 opt = 0;
     brd >> opt;
     if(opt == 1)
     {
         UInt8 athlDiffculty = 0;
         UInt8 athlCategory = 0;
         brd >> athlDiffculty;
         brd >> athlCategory;
         GObject::gAthleticsRank.RequestSubDir(player, athlDiffculty, athlCategory);
     }
     else
         GObject::gAthleticsRank.RequestPageNum(player);
}

void OnAthleticsKillCD( GameMsgHdr& hdr, const void * data)
{
     MSG_QUERY_PLAYER(player);
     if (!player->hasChecked())
         return;
     GObject::gAthleticsRank.RequestKillCD(player);
}
void OnAthleticsGetAwardReq( GameMsgHdr& hdr, AthleticsGetAwardReq& req )
{
    MSG_QUERY_PLAYER(player);
    GObject::gAthleticsRank.giveAward(player, req._type);
}

void OnLuckDrawRankListReq( GameMsgHdr& hdr, LuckyDrawRankListReq& req )
{
    MSG_QUERY_PLAYER(player);
    WORLD().SendLuckyDrawList(player);
}


struct GetBoxReq
{
	MESSAGE_DEF(0xD4);
};

void OnGetOwnBoxReq( GameMsgHdr& hdr, GetBoxReq& )
{
	MSG_QUERY_PLAYER(player);
	GObject::gAthleticsRank.GetBoxSourceReq(player);
}

void OnArenaConnected( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 r = 0;
	brd >> r;
	if(r == 1)
	{
		INFO_LOG("Failed to connect to arena.");
		NETWORK()->CloseArena();
		return;
	}
	GObject::arena.readFrom(brd);
}

void OnPlayerEntered( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt8 r;
	UInt64 playerId;
	UInt8 entered;
	std::string rname;
	br >> r >> playerId >> entered >> rname;
	GObject::Player * player = GObject::globalPlayers[playerId];
	if(player == NULL)
		return;
	GObject::arena.push(player, entered, rname);
	Stream st(REP::SERVER_ARENA_OP);
	st << static_cast<UInt8>(1) << r << static_cast<UInt8>(entered + 1) << rname << Stream::eos;
	player->send(st);
    if (!r)
    {
        player->arenaUdpLog(1001);
    }
}

void OnLineupCommited( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 r;
	UInt64 playerId;
    br >> r >> playerId;
	GObject::Player * player = GObject::globalPlayers[playerId];
	if(player == NULL)
		return;
    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(4) << r << Stream::eos;
    player->send(st);
}

void OnArenaPriliminary( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::arena.pushPriliminary(br);
}

void OnPriliminaryInfo( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt32 r[3] = {0};
	br >> r[0] >> r[1] >> r[2];
	GObject::arena.pushPriliminaryCount(r);
}

void OnArenaBattleReport( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt64 pid = 0;
    br >> pid;
    Stream st;
    std::vector<UInt8> buf;
    buf.resize(br.size()-8);
    br >> buf;
    st << buf;
    st << Stream::eos;

	GObject::Player * player = GObject::globalPlayers[pid];
    if(player == NULL)
        return;
	player->send(&(st[0]), st.size());
}

void OnArenaSupport( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 flag = 0;
    UInt16 pos = 0;
    br >> type >> flag >> pos;
    GObject::arena.updateSuport(type, flag, pos);
}

void OnArenaBattlePoint( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::arena.updateBattlePoint(br);
}

void OnArenaLeaderBoard( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::arena.updateLeaderBoard(br);
}

void OnArenaInfoReq( GameMsgHdr& hdr, ArenaInfoReq& air )
{
	MSG_QUERY_PLAYER(player);
    switch(air.type)
    {
    case 0:
        GObject::arena.sendStatus(player);
        break;
    case 1:
        GObject::arena.sendEnter(player);
        break;
    case 2:
    case 3:
        GObject::arena.sendPreliminary(player, air.type-2, air.flag, air.start, air.len);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        GObject::arena.sendElimination(player, air.type-4, air.flag);
        break;
    }
	//GObject::arena.sendInfo(player);
}

void OnArenaLeaderBoardReq( GameMsgHdr&hdr, ArenaLeaderBoardReq& aer )
{
	MSG_QUERY_PLAYER(player);
	GObject::arena.sendLeaderBoard(player);
}

void OnArenaOpReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt8 type = 0;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	brd >> type;
	switch(type)
	{
	case 0:
		{
            GObject::arena.sendActive(player);
		}
		break;
	case 1:
		GObject::arena.enterArena(player);
		break;
	case 2:
		{
            UInt8 state = 0, group = 0, tael = 0;
            UInt16 pos = 0;
            UInt64 pid = 0;
			brd >> state >> tael;
            if(state > 6)
                break;
            if(state < 2)
                brd >> pid;
            else
                brd >> pos;
            brd >> group;
            if(tael > 1)
                break;
			//if(tael > 0)
			{
                UInt8 r = 0;
                if(player->GetPackage()->GetItemAnyNum(ARENA_BET_ITEM1) < 1 && tael == 1)
                    r = 3;
                else if(player->GetPackage()->GetItemAnyNum(ARENA_BET_ITEM2) < 1 && tael == 0)
                    r = 4;
                else if(state < 2)
                    r = GObject::arena.bet1(player, state, group, pid, tael);
                else
                    r = GObject::arena.bet2(player, state, group, pos, tael);
                if(r == 0xFF)
                    break;
                if (r <= 1)
                    player->arenaUdpLog(1002, r);
				Stream st(REP::SERVER_ARENA_OP);
				st << type << r << state;
                if(state < 2)
                    st << pid;
                else
                    st << pos;
                st << group << Stream::eos;
				player->send(st);
			}
		}
		break;
	case 3:
		{
            UInt32 battleId = 0;
            brd >> battleId;
            Stream st(ARENAREQ::BATTLE_REPORT, 0xEF);
            st << player->getId() << battleId << Stream::eos;
            NETWORK()->SendToArena(st);
		}
		break;
    case 4:
        {
            if(player->inArenaCommitCD())
            {
                Stream st(REP::SERVER_ARENA_OP);
                st << static_cast<UInt8>(4) << static_cast<UInt8>(2) << Stream::eos;
                player->send(st);
            }
            else
                GObject::arena.commitLineup(player);
        }
        break;
	}
}

#ifdef _ARENA_SERVER
void onServerReg(LoginMsgHdr& hdr, const void * data)
{
	MSG_QUERY_BR(br);
	std::string channelName;
	int channelId = 0;
	int sno = 0;
	int mno = 0;
	br >> channelName >> channelId >> sno >> mno;
	if(channelName.empty())
		return;
	if(sno == 0)
		return;
	std::set<int> mlist;
	for(int i = 0; i < mno; ++ i)
	{
		int mid;
		br >> mid;
		mlist.insert(mid);
	}
	TcpConnection conn = NETWORK()->GetConn(hdr.sessionId);
	Network::ArenaClient * aconn = static_cast<Network::ArenaClient *>(conn.get());
	aconn->SetChannelId(channelId);
	aconn->SetServerId(sno);
	GObject::GameServer * gs = GObject::gameServers.add(channelName, channelId, sno, mlist, hdr.sessionId);
	if(gs == NULL)
	{
		Stream st(0x01);
		st << static_cast<UInt8>(1) << Stream::eos;
		aconn->send(&st[0], st.size());
	}
	else
	{
		int type = GObject::gameServers.channelType(channelId);
		switch(type)
		{
		case 1:
			if(GObject::stageManager.isGlobal() > 1)
				return;
			break;
		case 2:
			if(GObject::stageManager.isGlobal() <= 1)
				return;
			break;
		case 3:
			return;
		}
		Stream st;
		GObject::stageManager.makeInfo(st, channelId, sno, true);
		aconn->send(&st[0], st.size());
	}
}

template<class T, class DT>
static T* readEquip(BinaryReader& br)
{
	UInt16 typeId;
	UInt8 extraCount;
	br >> typeId;
	if(typeId == 0)
		return NULL;
	const GData::ItemBaseType * itype = GData::itemBaseTypeManager[typeId];
	if(itype == NULL)
		return NULL;
	GObject::ItemEquipData edata;
	br >> edata.enchant >> extraCount;
	if(edata.enchant > ENCHANT_LEVEL_MAX || extraCount > 3)
		return NULL;
	switch(extraCount)
	{
	case 1:
		br >> edata.extraAttr2.type1 >> edata.extraAttr2.value1;
		break;
	case 2:
		br >> edata.extraAttr2.type1 >> edata.extraAttr2.value1;
		br >> edata.extraAttr2.type2 >> edata.extraAttr2.value2;
		break;
	case 3:
		br >> edata.extraAttr2.type1 >> edata.extraAttr2.value1;
		br >> edata.extraAttr2.type2 >> edata.extraAttr2.value2;
		br >> edata.extraAttr2.type3 >> edata.extraAttr2.value3;
		break;
	default:
		break;
	}
	br >> edata.sockets;
	if(edata.sockets > 6)
		return NULL;
	for(int i = 0; i < edata.sockets; ++ i)
		br >> edata.gems[i];
	return new(std::nothrow) T(*static_cast<const DT *>(itype), edata);
}

static UInt8 readEquips(GObject::Player * player, BinaryReader& br)
{
	UInt8 formation, fgtCount;
	br >> formation >> fgtCount;
	if(fgtCount == 0 || fgtCount > 5)
	{
		return 1;
	}
	else
	{
		DB().PushUpdateData("DELETE FROM `fighter` WHERE `playerId` = %"I64_FMT"u", player->getId());
		for(UInt8 i = 0; i < fgtCount; ++ i)
		{
			UInt8 pos;
			UInt16 fgtId;
			UInt8 level;
			UInt8 potential;
			UInt16 skillId;
			UInt8 flag;
			br >> pos >> fgtId >> level >> potential >> skillId >> flag;
			GObject::Fighter * fgt = player->findFighter(fgtId);
			if(fgt == NULL)
			{
				GObject::Fighter * specfgt = GObject::globalFighters[fgtId];
				if(specfgt == NULL)
					continue;
				fgt = specfgt->clone(player);
				if(fgt == NULL)
					continue;
				player->addFighter(fgt, false);
			}
			GObject::Lineup& lu = player->getLineup(i);
			if(lu.fid != fgtId || lu.pos != pos)
			{
				lu.fighter = fgt;
				lu.pos = pos;
				lu.updateId();
			}
			fgt->setLevel(level);
			fgt->setPotential(static_cast<float>(potential) / 100.0f);
			fgt->setSkillAndLevel(skillId);
			UInt32 p = static_cast<UInt32>(fgt->getPotential() * 100 + 0.5);
			DB().PushUpdateData("REPLACE INTO `fighter`(`id`, `playerId`, `potential`, `level`, `skill`) VALUES(%u, %"I64_FMT"u, %u.%02u, %u, %u)", fgtId, player->getId(), p / 100, p % 100, fgt->getLevel(), fgt->getSkillAndLevel());
			GObject::ItemWeapon * weapon;
			GObject::ItemArmor * armor;
			GObject::ItemEquip * equip;
			if(flag & 0x01)
			{
				weapon = readEquip<GObject::ItemWeapon, GData::ItemWeaponType>(br);
				weapon = fgt->setWeapon(weapon);
				if(weapon != NULL)
					delete weapon;
			}
			if(flag & 0x02)
			{
				armor = readEquip<GObject::ItemArmor, GData::ItemEquipType>(br);
				armor = fgt->setArmor(0, armor);
				if(armor != NULL)
					delete armor;
			}
			if(flag & 0x04)
			{
				armor = readEquip<GObject::ItemArmor, GData::ItemEquipType>(br);
				armor = fgt->setArmor(1, armor);
				if(armor != NULL)
					delete armor;
			}
			if(flag & 0x08)
			{
				armor = readEquip<GObject::ItemArmor, GData::ItemEquipType>(br);
				armor = fgt->setArmor(2, armor);
				if(armor != NULL)
					delete armor;
			}
			if(flag & 0x10)
			{
				armor = readEquip<GObject::ItemArmor, GData::ItemEquipType>(br);
				armor = fgt->setArmor(3, armor);
				if(armor != NULL)
					delete armor;
			}
			if(flag & 0x20)
			{
				armor = readEquip<GObject::ItemArmor, GData::ItemEquipType>(br);
				armor = fgt->setArmor(4, armor);
				if(armor != NULL)
					delete armor;
			}
			if(flag & 0x40)
			{
				equip = readEquip<GObject::ItemEquip, GData::ItemEquipType>(br);
				equip = fgt->setAmulet(equip);
				if(equip != NULL)
					delete equip;
			}
			if(flag & 0x80)
			{
				equip = readEquip<GObject::ItemEquip, GData::ItemEquipType>(br);
				equip = fgt->setRing(equip);
				if(equip != NULL)
					delete equip;
			}
		}
		for(UInt8 i = fgtCount; i < 5; ++ i)
		{
			GObject::Lineup& lu = player->getLineup(i);
			if(lu.fighter != NULL)
			{
				lu.fighter = NULL;
				lu.pos = 0;
				lu.updateId();
			}
		}
		player->setFormation(formation, false);
		player->storeFighters();
	}
	return 0;
}

void onPlayerEnter(GameMsgHdr& hdr, const void * data)
{
	if(hdr.serverId >= 4096 || hdr.channelId >= 256)
		return;
	int type = GObject::gameServers.channelType(hdr.channelId);
	switch(type)
	{
	case 1:
		if(GObject::stageManager.isGlobal() > 1)
			return;
		break;
	case 2:
		if(GObject::stageManager.isGlobal() <= 1)
			return;
		break;
	case 3:
		return;
	}
	UInt8 pg = GObject::stageManager.getProgress();
	if(pg == 1 || pg == 4 || pg == 5 || pg == 6)
	{
		MSG_QUERY_BR(br);
		UInt64 id;
		std::string name;
		UInt8 flag;
		br >> id >> name >> flag;
		if(name.empty())
			return;
		int pos = static_cast<int>(name.size()) - 1;
		if(static_cast<UInt8>(name[pos]) < 32)
		{
			do
			{
				-- pos;
			}
			while(pos >= 0 && static_cast<UInt8>(name[pos]) < 32);
			if(pos < 0)
				return;
			name.erase(name.begin() + pos + 1, name.end());
		}
		UInt8 r = 0;
		UInt8 entered = 0xFF;
		UInt8 title = flag & 0x7F;
		bool newplayer = false;
		UInt64 pid = id | (static_cast<UInt64>(hdr.channelId) << 40);
		if((pid >> 48) == 0)
			pid |= (static_cast<UInt64>(hdr.serverId) << 48);
		GObject::Player * player = GObject::globalPlayers[pid];
		if(player == NULL)
		{
			player = new(std::nothrow) GObject::Player(pid);
			if(player == NULL)
				r = 1;
			else
				newplayer = true;
		}
		if(r == 0)
		{
			PLAYER_DATA(player, name) = name;
			PLAYER_DATA(player, title) = title;
			DB().PushUpdateData("REPLACE INTO `player`(`id`, `name`, `title`, `entered`) VALUES (%"I64_FMT"u, '%s', %u, %u)", player->getId(), player->getName().c_str(), player->getTitle(), player->getEntered());
			r = readEquips(player, br);
			if(r == 0)
			{
				GObject::globalPlayers.add(player);
				entered = GObject::stageManager.enter(player);
				player->setEntered(entered);
			}
		}
		if(r != 0 && newplayer)
		{
			DB().PushUpdateData("DELETE FROM `player` WHERE `id` = %"I64_FMT"u", player->getId());
			delete player;
		}
		Stream st(0x02);
		if(entered >= STAGE_GROUPS)
			r = 1;
		st << r << id << entered;
		if(player == NULL || r != 0)
			st << "";
		else
		{
			st << player->getDisplayName();
			st << player->getFormation();
			for(int i = 0; i < 5; ++ i)
			{
				GObject::Lineup& lu = player->getLineup(i);
				if(lu.available())
					st << static_cast<UInt16>(lu.fighter->getId()) << lu.fighter->getLevel() << lu.fighter->getColor();
				else
					st << static_cast<UInt32>(0);
			}
		}
		st << Stream::eos;
		player->send(st);
	}
	else
	{
		Stream st(0x02);
		st << static_cast<UInt8>(1) << static_cast<UInt64>(0) << static_cast<UInt8>(0) << "" << Stream::eos;
		GObject::gameServers.send(hdr.channelId, hdr.serverId, &st[0], st.size());
	}
}

void onLineupCommit(GameMsgHdr& hdr, const void * data)
{
	int type = GObject::gameServers.channelType(hdr.channelId);
	switch(type)
	{
	case 1:
		if(GObject::stageManager.isGlobal() > 1)
			return;
		break;
	case 2:
		if(GObject::stageManager.isGlobal() <= 1)
			return;
		break;
	case 3:
		return;
	}
	MSG_QUERY_BR(br);
	UInt64 id;
	br >> id;
	UInt8 r = 0;
	GObject::Player * player = GObject::globalPlayers.find(id, hdr.channelId, hdr.serverId);
	if(player == NULL)
	{
		r = 1;
		Stream st(0x03);
		st << r << id << Stream::eos;
		GObject::gameServers.send(hdr.channelId, hdr.serverId, &st[0], st.size());
	}
	else
	{
		r = readEquips(player, br);
		Stream st(0x03);
		st << r << id;
		st << player->getFormation();
		for(int i = 0; i < 5; ++ i)
		{
			GObject::Lineup& lu = player->getLineup(i);
			if(lu.available())
				st << static_cast<UInt16>(lu.fighter->getId()) << lu.fighter->getLevel() << lu.fighter->getColor();
			else
				st << static_cast<UInt32>(0);
		}
		st << Stream::eos;
		player->send(st);
	}
}

void onBetInfo(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_BR(br);
	GObject::BetsInfo bi[STAGE_GROUPS];
	for(int i = 0; i < STAGE_GROUPS; ++ i)
	{
		for(int j = 0; j < 32; ++ j)
		{
			br >> bi[i].count[j];
		}
	}
	GObject::stageManager.pushBetsInfo(hdr.channelId, hdr.serverId, bi);
}

void onSingleBet(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_BR(br);
	UInt8 group = 0, pos = 0;
	UInt32 count = 0;
	br >> group >> pos >> count;
	GObject::stageManager.pushSingleBet(hdr.channelId, hdr.serverId, group, pos, count);
}

void BattleReportReq(LoginMsgHdr& hdr, const void * data)
{
	if(hdr.msgHdr.bodyLen < sizeof(UInt32))
		return;
	UInt32 battleId = *reinterpret_cast<const UInt32 *>(data);
	std::vector<UInt8> *r = Battle::battleReport[battleId];
	if(r == NULL)
		return;
	NETWORK()->SendMsgToClient(hdr.sessionId, &(*r)[0], r->size());
}

void onDisconnected(GameMsgHdr& hdr, const void * data)
{
	GObject::gameServers.del(hdr.channelId, hdr.serverId);
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
	if(gmHandler.Handle(cmd, NULL, true))
		st<<"0";
	else
		st<<"1";
	st<<Stream::eos;
	NETWORK()->SendMsgToClient(hdr.sessionId,st);
}
#endif

void OnQixiReq(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	if(!player->hasChecked())
		return;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 op;
    UInt8 type;

    brd >> type;
    switch(type)
    {
        case 0x01:
        {
            if(!WORLD().getQixi())
                break;
            brd >> op;
            switch(op)
            {
            case 0x01:
                {
                    UInt8 type = 0;
                    brd >> type;
                    if(type == 1)
                        player->sendQixiInfo();
                    else if(type == 2)
                        WORLD().sendQixiPlayers(player);
                }
                break;
            case 0x02:
                {
                    UInt8 type = 0;
                    brd >> type;
                    if(type == 0)
                    {
                        UInt64 pid = 0;
                        brd >> pid;

                        GObject::Player * pl = GObject::globalPlayers[pid];
                        if(!pl)
                            break;
                        player->postQixiEyes(pl);
                    }
                    else if(type == 1)
                    {
                        player->divorceQixi();
                    }
                }
                break;
            case 0x03:
                {
                    UInt8 pos = player->getQueqiaoPos();
                    GameMsgHdr hdr1(0x254, player->getThreadId(), player, sizeof(pos));
                    GLOBAL().PushMsg(hdr1, &pos);
                }
                break;
            }
            break;
        }
    }
}

void OnTownDeamonReq( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;

    if(op !=0x08 && player->GetLev() < 40)
        return;

    if(op != 0x08)
    {
        GObject::townDeamonManager->checkStartTime(player); 
    }

    switch(op)
    {
    case 0x00:
        {
            GObject::townDeamonManager->showTown(player);
        }
        break;
    case 0x01:
        {
            UInt16 level = 0;
            br >> level;
            GObject::townDeamonManager->showLevelTown(player, level);
        }
        break;
    case 0x02:
        {
            UInt16 start = 0;
            UInt16 count = 0;
            br >> start >> count;
            GObject::townDeamonManager->listDeamons(player, start, count);
        }
        break;
    case 0x03:
        {
            if(!player->hasChecked())
                return;

            UInt8 count = 0;
            br >> count;
            GObject::townDeamonManager->useAccItem(player, count);
        }
        break;
    case 0x04:
        {
            if(!player->hasChecked())
                return;

            UInt8 count = 0;
            br >> count;
            GObject::townDeamonManager->useVitalityItem(player, count);
        }
        break;
    case 0x05:
        {
            if(!player->hasChecked())
                return;
            if(player->hasFlag(GObject::Player::InHeroIsland))
                return;

            UInt16 level = 0;
            UInt8 type = 0;
            br >> level >> type;
            GObject::townDeamonManager->challenge(player, level, type);
        }
        break;
    case 0x06:
        {
            if(!player->hasChecked())
                return;

            GObject::townDeamonManager->cancelDeamon(player);
        }
        break;
    case 0x07:
        {
            if(!player->hasChecked())
                return;

            UInt16 levels = 0;
            br >> levels;
            GObject::townDeamonManager->autoCompleteQuiteCheck(player, levels);
        }
        break;
    case 0x08:
        {
            player->getDeamonAwards();
        }
    default:
        return;
    }
}


#endif // _WORLDOUTERMSGHANDLER_H_

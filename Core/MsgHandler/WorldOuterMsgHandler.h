﻿#ifndef _WORLDOUTERMSGHANDLER_H_
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
#include "GObject/SingleHeroStage.h"
#include "GObject/MarryBoard.h"
#include "GObject/ClanBuilding.h"

#ifdef _ARENA_SERVER
#include "GObject/GameServer.h"
#include "GObject/Stage.h"
#include "Network/ArenaClient.h"
#include "Battle/BattleReport.h"
#else
#include "GObject/Arena.h"
#include "GObject/ArenaServerWar.h"
#endif

#include <mysql.h>
#include "Memcached.h"

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
    UInt8 _type;
    UInt16 _startId;
    UInt8 _count;
    MESSAGE_DEF3(REQ::CLAN_PACKAGE_RECORD, UInt8, _type, UInt16, _startId, UInt8, _count);
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
    UInt8  _attrId;
	UInt8  _eqType;
	MESSAGE_DEF8(REQ::SALE_LIST, UInt16, _start, UInt16, _count, std::string, _name, UInt8, _req, UInt8, _color, UInt8, _career, UInt8, _attrId, UInt8, _eqType);
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
	UInt8 type;
	MESSAGE_DEF1(REQ::SERVER_ARENA_LB, UInt8, type);
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
    /*
	if(cfg.merged && player->getId() >= 0x1000000000000ull)
	{
		UInt32 svno = static_cast<UInt32>(player->getId() >> 48);
		id |= (svno << 24);
	}
    */
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
	DB5().PushUpdateData("INSERT INTO `clan` (`id`, `name`, `foundTime`, `founder`, `leader`, `construction`, `contact`, `purpose`, `level`) VALUES (%u, '%s', %u, %" I64_FMT "u, %" I64_FMT "u, 0, '%s', '%s', %u)", clan->getId(), name2, TimeUtil::Now(), player->getId(), player->getId(), contact2, purpose2, clan->getLev());
	ConsumeInfo ci(ClanCreate,0,0);
	player->useTael(GData::moneyNeed[GData::CLAN_CREATE].tael,&ci);
	clan->initBuildClan();
	SYSMSG_SEND(120, player);
	SYSMSG_SENDV(1020, player, clan->getName().c_str());

    clan->notifyUpdateStatueAttr();

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

    switch(cqr._type)
    {
    case 0:
		clan->listMembers(player);
        break;
    case 1:
		clan->listPending(player);
        break;
    case 2:
        clan->listMembersActivePoint(player);
        break;
    }
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

void OnClanDuoBaoReq(GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
	GObject::Clan * clan = player->getClan();
	if(clan == NULL) return;

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    br >> type;
    switch(type)
    {
    case 0x00:
        clan->DuoBaoInfo(player);
        break;
    case 0x01:
        clan->DuoBaoStart(player);
        break;
    }
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
				clan->listDonators(player);
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
void OnClanQQ( GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
	GObject::Clan * clan = player->getClan();
	if (clan == NULL) return;

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    br >> type;
    switch(type)
    {
        //请求帮派QQ群Openid
    case 0x01:
        clan->sendQQOpenid(player);
        break;
        //绑定qq群
    case 0x02:
        {
            std::string qqOpenid;
            br >> qqOpenid;
            clan->setQQOpenid(player,qqOpenid);
            break;
        }
        //请求帮派qq群列表
    case 0x03:
        break;
        //加入qq群
    case 0x04:
        player->toQQGroup(true);
        break;
    case 0x05:
        player->toQQGroup(false);
        break;
    case 0x06:
        clan->offQQOpenid(player);

    }
}

void OnItemHistoryReq( GameMsgHdr& hdr, ClanItemHistoryReq& req)
{
    MSG_QUERY_PLAYER(player);
    GObject::Clan* clan = player->getClan();
    if(clan == NULL) return;

    switch(req._type)
    {
    case 0:
        clan->SendItemHistory(player, req._startId, req._count);
        break;
    case 1:
        clan->SendDonateHistory(player, req._startId, req._count);
        break;
    }
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
	Stream  st;
    //if(GObject::leaderboard.hasUpdate(lr._id) && GObject::leaderboard.getPacket(lr._type, st, player))
    if (lr._type == 7)
    {
        GameMsgHdr hdr(0x1C6, WORKER_THREAD_WORLD, player, 0);
        GLOBAL().PushMsg(hdr, NULL);
    }
    else
    {
        if (GObject::leaderboard.getPacket(lr._type, st, player))
        {
            if (!GObject::leaderboard.isSorting())
                player->send(st);
        }
        else
        {
            if (lr._type != 6)
            {
                UInt8 failed_packet[9] = {0x05, 0x00, 0xFF, REP::SORT_LIST, lr._type, 0x00, 0x00, 0x00, 0x00};
                player->send(failed_packet, 9);
            }
            else
            {
                UInt8 failed_packet[14] = {0x05, 0x00, 0xFF, REP::SORT_LIST, lr._type, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
                player->send(failed_packet, 14);
            }
        }
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
/*    Stream st(SERVERLEFTREQ::BATTLE_REPORT, 0xEE);
    st << player->getId() << static_cast<UInt32>(brr._reportId); 
    st << Stream::eos;
    NETWORK()->SendToServerLeft(st);
*/
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
	GObject::gSaleMgr.requestSaleList(player, req._start, req._count, req._name, req._req, req._color, req._career, req._attrId, req._eqType);
}

void OnSaleBuyAndCancelReq( GameMsgHdr& hdr, SaleBuyAndCancelReq& req )
{
	MSG_QUERY_PLAYER(player);

    if(GObject::gSaleMgr.getOnOff() == 0)
    {
        return;
    }
    if (req._token == 0 && player->getForbidSale())
    {
        player->sendMsgCode(0, 1040);  
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
    case 1:  //斗剑天梯
        {
            std::string name;
            brd >> name;
            GObject::gAthleticsRank.challenge(player, name);
        }
        break;
    case 2:  //斗剑历练
        {
            std::string name;
            UInt8 rivalDifficulty = 0;
            brd >> name;
            //brd >> rivalDifficulty;
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
         //GObject::gAthleticsRank.RequestSubDir(player, athlDiffculty, athlCategory);
         UInt16 tmp = (athlDiffculty << 8) | athlCategory;
         GameMsgHdr hdr(0x226, player->getThreadId(), player, sizeof(tmp));
         GLOBAL().PushMsg(hdr, &tmp);
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
    UInt8 fhaslater = 0;
    brd >> fhaslater;
    GObject::arena._readbuf.append((UInt8*)(data) + brd.pos(), brd.size() - brd.pos());
    if(!fhaslater)
    {
        BinaryReader brd2((UInt8*)(GObject::arena._readbuf), GObject::arena._readbuf.size());
        GObject::arena.readFrom(brd2);
        GObject::arena._readbuf.clear();
    }
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

void OnArenaPreliminary( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::arena.pushPreliminary(br);
}

void OnPreliminaryInfo( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt32 r[3] = {0};
	br >> r[0] >> r[1] >> r[2];
	GObject::arena.pushPreliminaryCount(r);
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
    switch(GObject::World::getArenaState())
    {
        case GObject::ARENA_XIANJIE_DIYI:
            GObject::arena.updateSuport(type, flag, pos);
            break;
        case GObject::ARENA_XIANJIE_ZHIZUN:
            GObject::teamArenaMgr.updateSuport(type, flag, pos);
            break;
    }
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
    case 10:
        GObject::teamArenaMgr.sendStatus(player);
        break;
    case 11:
        GObject::teamArenaMgr.sendEnter(player);
        break;
    case 12:
        GObject::teamArenaMgr.sendPreliminary(player, air.flag, air.start, air.len);
        break;
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
        GObject::teamArenaMgr.sendElimination(player, air.type-13, air.flag);
        break;
    }
	//GObject::arena.sendInfo(player);
}

void OnArenaLeaderBoardReq( GameMsgHdr&hdr, ArenaLeaderBoardReq& aer )
{
	MSG_QUERY_PLAYER(player);
    switch(aer.type)
    {
        case 1:
	        GObject::arena.sendLeaderBoard(player);
            break;
        case 2:
	        GObject::teamArenaMgr.sendLeaderBoard(player);
            break;
        case 3:
	        GObject::arena.sendLastLeaderBoard(player);
            break;
        case 4:
	        GObject::serverWarMgr.sendLeaderBoard(player);
            break;
    }
}

void OnArenaWarOpReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    if(!cfg.enabledServerWar())
        return;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 type = 0, opt = 0;
	brd >> type >> opt;
	switch(type)
	{
	case 0x01:  //服战信息
        {
            switch(opt)
            {
            case 0x00:  //请求服战当前状态
                GObject::serverWarMgr.sendStatus(player);
                break;
            case 0x01:  // 请求服战报名信息
                GObject::serverWarMgr.sendSignupInfo(player);
                break;
            case 0x02:  //请求服战祭剑赛信息
                GObject::serverWarMgr.sendWarSortInfo(player);
                break;
            case 0x03:  //请求服战入围赛信息
                {
                    UInt8 group = 0, flag = 0, len = 0;
                    UInt16 start = 0;
                    brd >> flag >> group >> start >> len;
                    GObject::serverWarMgr.sendPreliminary(player, group, flag, start, len);
                }
                break;
            case 0x04:
            case 0x05:
            case 0x06:
            case 0x07:
                {
                    UInt8 group = 0;
                    brd >> group;
                    GObject::serverWarMgr.sendElimination(player, opt, group);
                }
                break;
	        }
        }
        break;
	case 0x02:  //服战操作
		{
            switch(opt)
            {
            case 0x01:
                GObject::serverWarMgr.signup(player);
                break;
            case 0x02:
                GObject::serverWarMgr.bet(brd, player);
                break;
            case 0x03:
                {
                    UInt32 battleId = 0;
                    brd >> battleId;
                    Stream st(SERVERWARREQ::BATTLE_REPORT, 0xEE);
                    st << player->getId() << battleId << Stream::eos;
                    NETWORK()->SendToServerWar(st);
                }
                break;
            case 0x04:
                if(player->inServerWarCommitCD())
                {
                    Stream st(REP::SERVERWAR_ARENA_OP);
                    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x04);
                    st << static_cast<UInt8>(2) << Stream::eos;
                    player->send(st);
                }
                else
                    GObject::serverWarMgr.commitLineup(player);
                break;
            case 0x05:
                {
                    std::string name;
                    brd >> name;
                    GObject::serverWarMgr.challenge(player, name);
                }
                break;
            }
        }
		break;
	case 0x04:  //祭剑台
		{
            switch(opt)
            {
            case 0x00:
                GObject::serverWarMgr.sendjiJianTaiInfo(player);
                break;
            case 0x01:
                GObject::serverWarMgr.jiJianTai_operate(player);
                break;
            case 0x02:
                GObject::serverWarMgr.jiJianTai_complete(player, 1);
                break;
            case 0x03:
                GObject::serverWarMgr.jiJianTai_complete(player, 0);
                break;
            case 0x04:
                GObject::serverWarMgr.jiJianTai_convert(player);
                break;
            case 0x05:
                {
                    UInt8 idx = 0;
                    brd >> idx ;
                    GObject::serverWarMgr.jiJianTai_openBox(player, idx);
                }
                break;
            case 0x06:
                GObject::serverWarMgr.jiJianTai_buyTimes(player);
                break;
            }
        }
		break;
    }
}

void OnArenaExtraActReq( GameMsgHdr& hdr, const void * data )
{
    if(!GObject::arena.active())
        return;
	MSG_QUERY_PLAYER(player);
    UInt8 week;
	UInt8 type;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    brd >> week;
	brd >> type;

    if(week < ARENA_WEEK_START || week > ARENA_WEEK_END)
    {
        return;
    }

    UInt32 now = TimeUtil::Now();
    UInt8 serverWeek = TimeUtil::GetWeekDay(now);
    UInt32 t1 = TimeUtil::SharpDayT(0, now) + ARENA_SINGUP_START;

    if(serverWeek == week)
    {
        UInt32 t3 = TimeUtil::SharpDayT(0, now) + ARENA_SUFFER_END;
        UInt8 type2 = week - ARENA_WEEK_START;
        if(now > t3 + 60 && GObject::World::stArenaOld[type2].week == week)
        {
            Stream st(REP::SERVER_ARENA_EXTRA_ACT);
            st << week;
            st << static_cast<UInt8>(0);
            for(UInt8 i = 0; i < 5; i++)
            {
                st << GObject::World::stArenaOld[type2].name[i];
                st << GObject::World::stArenaOld[type2].heroId[i];
            }
            st << GObject::World::stArenaOld[type2].sufferTotal * 24 / 5;
            st << Stream::eos;
            player->send(st);

            Stream st2(REP::SERVER_ARENA_EXTRA_ACT);
            st2 << week;
            st2 << static_cast<UInt8>(3);
            if(week == ARENA_WEEK_START)
                st2 << static_cast<UInt8>(player->GetVar(GObject::VAR_ARENA_SUPPORT_TUE));
            else
                st2 << static_cast<UInt8>(player->GetVar(GObject::VAR_ARENA_SUPPORT_WED));
            for(UInt8 i = 0; i < 5; i++)
            {
                st2 << GObject::World::stArenaOld[type2].sufferCnt[i];
                st2 << GObject::World::stArenaOld[type2].rank[i];
            }
            st2 << Stream::eos;
            player->send(st2);
            return;
        }
        else if(now < t1)
        {
            Stream st(REP::SERVER_ARENA_EXTRA_ACT);
            st << week;
            st << static_cast<UInt8>(5);
            st << Stream::eos;
            player->send(st);
            return;
        }
        switch(type)
        {
            case 0:
                player->ArenaExtraAct(type, 0);
                break;
            case 1:
                if(player->GetLev() < 45)
                    return;
                UInt8 supportId;
                brd >> supportId;
                player->ArenaExtraAct(type, supportId);
                break;
            case 2:
                if(player->GetLev() < 45)
                    return;
                UInt8 sufferId;
                brd >> sufferId;
                player->ArenaExtraAct(type, sufferId);
                break;
            default:
                break;
        }
    }
    else
    {
        UInt8 type2 = week - ARENA_WEEK_START;
        if(serverWeek > week && GObject::World::stArenaOld[type2].week == week)
        {
            Stream st(REP::SERVER_ARENA_EXTRA_ACT);
            st << week;
            st << static_cast<UInt8>(0);
            for(UInt8 i = 0; i < 5; i++)
            {
                st << GObject::World::stArenaOld[type2].name[i];
                st << GObject::World::stArenaOld[type2].heroId[i];
            }
            st << GObject::World::stArenaOld[type2].sufferTotal * 24 / 5;
            st << Stream::eos;
            player->send(st);

            Stream st2(REP::SERVER_ARENA_EXTRA_ACT);
            st2 << week;
            st2 << static_cast<UInt8>(3);
            if(week == ARENA_WEEK_START)
                st2 << static_cast<UInt8>(player->GetVar(GObject::VAR_ARENA_SUPPORT_TUE));
            else
                st2 << static_cast<UInt8>(player->GetVar(GObject::VAR_ARENA_SUPPORT_WED));
            for(UInt8 i = 0; i < 5; i++)
            {
                st2 << GObject::World::stArenaOld[type2].sufferCnt[i];
                st2 << GObject::World::stArenaOld[type2].rank[i];
            }
            st2 << Stream::eos;
            player->send(st2);
        }
        else
        {
            Stream st(REP::SERVER_ARENA_EXTRA_ACT);
            st << week;
            st << static_cast<UInt8>(5);
            st << Stream::eos;
            player->send(st);
            return;
        }
    }
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
                //if (r <= 1)
                //    player->arenaUdpLog(1002, r);
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
            {
                if(arena.isOpen())
                    GObject::arena.commitLineup(player);
                else
                    GObject::teamArenaMgr.commitLineup1(player);
            }
        }
        break;
    case 5:
        {
            std::string name;
            brd >> name;
            extern void trimName(std::string& str);
            trimName(name);
            GObject::teamArenaMgr.createTeam(player, name);
        }
        break;
    case 6:
        GObject::teamArenaMgr.dismissTeam(player);
        break;
    case 7:
        {
            std::string name;
            brd >> name;
	        GObject::Player * pl = GObject::globalNamedPlayers[player->fixName(name)];
			if(!pl || !player->testCanAddTeamMember(pl))
				return;
			SYSMSGV(title, 4214);
			SYSMSGV(content, 4215, player->getCountry(), player->getName().c_str(), player->getTeamArena()->getName().c_str());
            pl->GetMailBox()->newMail(player, 0x14, title, content);
        }
        break;
    case 8:
        {
            UInt64 playerId = 0;
            brd >> playerId;
            GObject::teamArenaMgr.fireTeamMember(player, playerId);
        }
        break;
    case 9:
        {
            UInt64 playerId = 0;
            brd >> playerId;
            GObject::teamArenaMgr.handoverLeader(player, playerId);
        }
        break;
    case 10:
        {
            UInt8 skillId = 0;
            UInt32 tael = 0;
            brd >> skillId >> tael;
            GObject::teamArenaMgr.upgradeTeamSkill(player, skillId, tael);
        }
        break;
    case 11:
        {
            UInt64 playerId1, playerId2, playerId3;
            std::string stampStr;
            brd >> playerId1 >> playerId2 >> playerId3 >> stampStr;
            GObject::teamArenaMgr.setMemberPosition(player, playerId1, playerId2, playerId3, stampStr);
        }
        break;
    case 12:
        GObject::teamArenaMgr.leaveTeamArena(player);
        break;
    case 13:
        GObject::teamArenaMgr.enterArena(player);
        break;
    case 14:
        GObject::teamArenaMgr.commitLineup(player);
        break;
    case 15:
        GObject::teamArenaMgr.getTeamInfo(player);
        break;
    case 16:
        {
            UInt8 state = 0, group = 0, tael = 0;
            UInt16 pos = 0;
            UInt64 pid = 0;
			brd >> state >> tael;
            if(state > 5)
                break;
            if(state < 1)
                brd >> pid;
            else
                brd >> pos;
            brd >> group;
            if(tael > 1)
                break;
            UInt8 r = 0;
            if(player->GetPackage()->GetItemAnyNum(ARENA_BET_ITEM1) < 1 && tael == 1)
                r = 3;
            else if(player->GetPackage()->GetItemAnyNum(ARENA_BET_ITEM2) < 1 && tael == 0)
                r = 4;
            else if(state < 1)
                r = GObject::teamArenaMgr.bet1(player, state, group, pid, tael);
            else
                r = GObject::teamArenaMgr.bet2(player, state, group, pos, tael);
            if(r == 0xFF)
                break;
            Stream st(REP::SERVER_ARENA_OP);
            st << static_cast<UInt8>(14) << r << state;
            if(state < 1)
                st << pid;
            else
                st << pos;
            st << group << Stream::eos;
            player->send(st);
	    }
        break;
    case 17:
        {   //每日崇拜冠军
            UInt8 opt = 0;
            brd >> opt;
            GObject::teamArenaMgr.championWorship(player, opt);
	    }
        break;
    case 18:    //鼓舞队伍
        GObject::teamArenaMgr.inspireTeam(player);
        break;
    case 19:
        {
            std::string name = "";
            brd >> name;
            GObject::teamArenaMgr.applyTeam(player, name);
        }
        break;
    case 20:
        {
            UInt16 idx = 0;
            UInt8 cnt = 0;
            brd >> idx >> cnt;
            GObject::teamArenaMgr.listAllTeam(player, idx, cnt);
        }
        break;
    case 21:
        {
            std::string name = "";
            brd >> name;
            GObject::teamArenaMgr.searchTeam(player, name, 0);
        }
        break;
    case 22:
        {
            std::string name = "";
            brd >> name;
            GObject::teamArenaMgr.searchTeam(player, name, 1);
        }
        break;
    case 23:
        GObject::teamArenaMgr.listTeamPending(player);
        break;
    case 24:
        {
            UInt8 opt = 0;
            UInt64 inviteeId = 0;
            brd >> opt >> inviteeId;
            if(opt)
                GObject::teamArenaMgr.declineApply(player, inviteeId);
            else
                GObject::teamArenaMgr.acceptApply(player, inviteeId);
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
		DB().PushUpdateData("DELETE FROM `fighter` WHERE `playerId` = %" I64_FMT "u", player->getId());
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
			DB().PushUpdateData("REPLACE INTO `fighter`(`id`, `playerId`, `potential`, `level`, `skill`) VALUES(%u, %" I64_FMT "u, %u.%02u, %u, %u)", fgtId, player->getId(), p / 100, p % 100, fgt->getLevel(), fgt->getSkillAndLevel());
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
			DB().PushUpdateData("REPLACE INTO `player`(`id`, `name`, `title`, `entered`) VALUES (%" I64_FMT "u, '%s', %u, %u)", player->getId(), player->getName().c_str(), player->getTitle(), player->getEntered());
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
			DB().PushUpdateData("DELETE FROM `player` WHERE `id` = %" I64_FMT "u", player->getId());
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
    /*
	if(!player->hasChecked())
		return;
    */
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 op = 0;
    UInt8 type = 0;

    brd >> type;
    switch(type)
    {
        case 0x02:  //排行活动
        {
            UInt8 flag = 0;
            brd >> op >> flag;
            if(op != 6)     //跨服充值排行活动
                return;
            if(0 == flag)
            {
                UInt8 idx = 0, cnt = 0;
                brd >> idx >> cnt;
                leaderboard.sendRechargeRank100(player, idx, cnt);
            }
            else if(1 == flag)
                leaderboard.sendMyRechargeRank(player);
            break;
        }
        case 0x01:  // 七夕
        case 0x03:  // 万圣节
        case 0x09:  // 情人节浪漫之旅
        {
            if(!WORLD().getQixi() && !WORLD().getWansheng() && !WORLD().getQingren())
                break;
            brd >> op;
            switch(op)
            {
            case 0x01:
                {
                    UInt8 form = 0;
                    brd >> form;
                    if(form == 1)
                        player->sendQixiInfo();
                    else if(form == 2)
                        WORLD().sendQixiPlayers(player);
               }
                break;
            case 0x02:
                {
                    UInt8 form = 0;
                    brd >> form;
                    if(form == 0)
                    {
                        UInt64 pid = 0;
                        brd >> pid;

                        GObject::Player * pl = GObject::globalPlayers[pid];
                        if(!pl)
                            break;
                        player->postQixiEyes(pl);
                    }
                    else if(form == 1)
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
            default:
                break;
            }
            break;
        }
        //堆雪人
        case 0x05:
        {
            if(!WORLD().getSnowAct())
                break;
            brd >> op;
            switch(op)
            {
            case 0x01:
                {
                    UInt8 t = 0;
                    brd >> t;
                    if(t == 1)
                        player->sendSnowInfo();
                    else if(t == 2)
                        WORLD().sendSnowPlayers(player);
                }
                break;
            case 0x02:
                {
                    UInt8 t = 0;
                    brd >> t;
                    if(t == 0)
                    {
                        UInt64 pid = 0;
                        brd >> pid;

                        GObject::Player * pl = GObject::globalPlayers[pid];
                        if(!pl)
                            break;
                        player->postSnowLover(pl);
                    }
                    else if(t == 1)
                    {
                        player->divorceSnowLover();
                    }
                }
                break;
            case 0x03: //堆雪人
                {
                    UInt32 num = 0;
                    brd >> num;
                    UInt8 res = player->useSnowItem(num);

                    Stream st(REP::ACTIVE);
                    st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x03);
                    st << static_cast<UInt8>(res);
                    st << Stream::eos;
                    player->send(st);
   
                }
                break;
            case 0x04: //领取奖励
                {
                    UInt16 t = 0;
                    brd >> t;
                    UInt8 res = player->getSnowAward(t);
                    Stream st(REP::ACTIVE);
                    st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x04);
                    st << static_cast<UInt8>(res);
                    st << Stream::eos;
                    player->send(st);
                }
                break;
            default:
                break;
            }
            break;
        }
        //大闹龙宫,大闹龙宫之金蛇起舞,大闹龙宫之天芒神梭,之混元剑诀
        case 0x06:
        case 0x0A:
        case 0x0B:
        case 0x0D:
        case 0x0F:
        case 0x11:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x21:
        case 0x24:
        case 0x25:
        case 0x27:
        case 0x29:
        case 0x3A:
        case 0x3B:
        {
            brd >> op;
            switch(op)
            {
            case 0x01:  //获取龙宫信息
                player->getDragonKingInfo();
                break;
            case 0x02:  //龙宫寻宝
                {
                    UInt8 count = 0;
                    brd >> count;
                    player->postDragonKing(count);
                }
                break;
            default:
                break;
            }
            break;
        }
        //金蛇献瑞 聚福兆祥
        case 0x07:
        {
            brd >> op;
            UInt32 param = 0;
            brd >> param;
            struct goldData
            {
                UInt8 opt;
                UInt32 param;
            }data;
            data.opt = op;
            data.param = param;
            GameMsgHdr h(0x343,  player->getThreadId(), player, sizeof(data));
            GLOBAL().PushMsg(h, &data);
            break;
        }
        //拜灵蛇,领金蛋
        case 0x08:
        {
            brd >> op;
            switch (op)
            {
                case 0x01:
                    player->sendSnakeEggInfo();
                    break;
                case 0x02:
                    player->callSnakeEgg();
                    break;
                case 0x03:
                    UInt8 day = 0;
                    brd >> day;
                    player->getSnakeEggAward(day);
                    break;
            }
            break;
        }
        case 0x0C:
        {
            hdr.msgHdr.desWorkerID = player->getThreadId();
            GLOBAL().PushMsg(hdr, (void*)data);
            /*
            brd >> op;
            switch (op)
            {
                case 0x01:
                    player->sendTownTjItemInfo();
                    break;
                case 0x02:
                    UInt32 itemId = 0;
                    brd >> itemId;
                    player->buyTownTjItem(itemId);
                    break;
            }
            */
            break;
        }
        case 0x0E:
        {
            brd >> op;
            switch (op)
            {
                case 0x01:
                    player->sendLongyuanActInfo();
                    break;
                case 0x02:
                    UInt8 idx = 0;
                    UInt8 flag = 0;
                    brd >> idx;
                    brd >> flag;
                    player->getLongyuanAct(idx, flag);
                    break;
            }
            break;
        }
        case 0x10:
        {
            if (!World::getFoolsDay())
                return;
            brd >> op;
            struct foolsData
            {
                UInt8 type;
                UInt8 id;
                char answer;
            }fdata;
            fdata.type = op;
            brd >> fdata.id;
            brd >> fdata.answer;
            GameMsgHdr h(0x344,  player->getThreadId(), player, sizeof(fdata));
            GLOBAL().PushMsg(h, &fdata);
            break;
        }
        case 0x12:
        {
            if (!player->getLuckyStarAct())
                return;
            brd >> op;
            struct starData
            {
                UInt8 type;
                UInt8 idx;
            }sdata;
            sdata.type = op;
            brd >> sdata.idx;
            GameMsgHdr h(0x345,  player->getThreadId(), player, sizeof(sdata));
            GLOBAL().PushMsg(h, &sdata);
        }
        break;
        case 0x13:
        {
            if (!World::getSurnameLegend())
                return;

            struct Data
            {
                UInt8 type;
                char name[32];
            }data;

            brd >> data.type;
            if(0x04 == data.type)
            {
                std::string name;
                brd >> name;
                strncpy(data.name, name.c_str(), 31);
            }

            GameMsgHdr h(0x346,  player->getThreadId(), player, sizeof(data));
            GLOBAL().PushMsg(h, &data);
            break;
        }
        case 0x41:
        {
            if (!World::getSpreadAct())
                return;
            brd >> op;
            if(op == 0)
            {
                std::string name;
                brd >> name;
                player->spreadToOther(0, name);
            }
            else if(op == 1)
                player->spreadGetAward();
            else if(op == 3)
                player->spreadToSelf();
            else if(op == 4)
                player->sendSpreadBasicInfo();
            else if(op == 5)
            {
                GameMsgHdr h(0x348,  player->getThreadId(), player, 0);
                GLOBAL().PushMsg(h, NULL);
            }
        }
        break;
        case 0x42:
        {
            UInt8 flag = 0;
            brd >> flag;
            player->SetVar(VAR_DIRECTPUROPEN, flag?1:0);
        }
        break;
        case 0x20:
        {
            UInt8 index =0;
            brd>>index;
            switch(index)
           {
               case 0x05:
                    {
                        UInt8 type = 0;
                        brd >>type;
                        GameMsgHdr h(0x365,  player->getThreadId(), player, sizeof(UInt8));
                        GLOBAL().PushMsg(h, &type);
               //         break; 
                    }
               case 0x01:
                    {
                       player->On11PlayerGradeRank();
                      // break;
                    }
               case 0x03:
                    {
                        player->On11ClanGradeRank();
                        player->On11CountryGradeRank();
                        break;
                    }
               case 0x04:   //帮派成员积分
                    { 
                        player->SendClanMemberGrade();
                        break;
                    }
               case 0x02:
                    {
                        UInt8 opt = 0;
                        brd>>opt;
                        switch(opt)
                        {
                            case 0:
                                player->sendAirBookInfo();
                                break;
                            case 1:
                                player->sendAirBookOnlineInfo();
                                break;
                        }
                    }
                    break;
               case 0x06:
                    {
                        UInt8 type =0;
                        UInt64 playerid = 0;
                        brd >> type;
                        brd >> playerid;
                        player->AirBookPriase(type,playerid);
                        break;
                    } 
                default:
                    break;
           }
        }
        break;
        case 0x22:  // 光棍节活动
        {
            if( ! WORLD().getGGTime() )
                break;

            hdr.msgHdr.desWorkerID = player->getThreadId();
            GLOBAL().PushMsg(hdr, (void*)data);
            break;
        }
        break;
        case 0x23:  // 七石斗法活动
        {
            if(!WORLD().getQiShiBanTime())
                break;

            brd >> op;
            if(0x09 == op) //包裹相关国家线程处理
            {
                hdr.msgHdr.desWorkerID = player->getThreadId();
                GLOBAL().PushMsg(hdr, (void*)data);
                break;
            }
            else
            {
                switch (op)
                {
                    case 0x01:
                        {
                            UInt8 index = 0;
                            brd >> index;
                            switch(index)
                            {
                                case 0:
                                    {
                                        UInt32 page = 0;
                                        brd >> page;
                                        player->OnQiShiBanRank(page);
                                    }
                                    break;
                                    case 1:
                                    {
                                        if(!player->CheckReqDataTime())
                                            return;

                                        UInt16 count = 0;
                                        brd >> count;
                                        
                                        Stream st(REP::ACT);
                                        st << static_cast<UInt8>(0x23) << static_cast<UInt8>(1) << static_cast<UInt8>(1);
                                        st << count;

                                        for(UInt32 i=0; i<count; i++)
                                        {
                                            std::string openId;
                                            openId.clear();
                                            brd >> openId;
                                            UInt32 score = player->GetQQFriendScore(openId.c_str());
                                            std::string info;
                                            bool ret = player->GetQQFriendInfo(openId.c_str(), info);
                                            if (ret)
                                            {
                                                StringTokenizer tokenizer(info, "_");
                                                if (tokenizer.count() >= 2)
                                                {
                                                    st << tokenizer[1];
                                                    UInt8 fighterId = atoi(tokenizer[0].c_str());
                                                    st << fighterId;
                                                }
                                                else 
                                                    ret = false;
                                            }

                                            if (!ret)
                                            {
                                                st << std::string("未找到");
                                                st << static_cast<UInt8>(0);
                                            }
                                            st << score;
                                        }
                                        st << Stream::eos;
                                        player->send(st);

                                        player->SetReqDataTime();
                                    }
                                    break;
                            }
                            break;
                        }
                    case 0x02:
                        player->ReqStartQSB();
                        break;
                    case 0x03:
                        {
                            int randMark = 0;
                            UInt32 time = 0;
                            brd >> randMark;
                            brd >> time;
                            player->FinishCurStep(randMark, time);
                        }
                        break;
                    case 0x04:
                        player->AddTime();
                        break;
                    case 0x06:
                        {
                            player->Fail(); //时间到，失败

                            Stream st(REP::ACT);
                            st << static_cast<UInt8>(0x23) << static_cast<UInt8>(0x06) << Stream::eos;
                            player->send(st);
                        }
                        break;
                    case 0x07:
                        player->MyQSBInfo();
                        break;
                    case 0x08:
                        player->ContinueCurStep();
                        break;
                    case 0x10:
                        {
                            player->Fail(); //主动放弃，失败

                            Stream st(REP::ACT);
                            st << static_cast<UInt8>(0x23) << static_cast<UInt8>(0x10) << Stream::eos;
                            player->send(st);
                            break;
                        }
                    case 0x11:
                        player->QiShiBanState();
                        break;
                    case 0x12:
                        player->RestCurStep();
                        break;
                        /*
                    case 0x13:
                        {
                            UInt8 count = 0;
                            brd >> count;
                            std::vector<std::string> nameList;
                            for (UInt16 i = 0; i < count; ++i)
                            {
                                std::string name;
                                brd >> name;
                                nameList.push_back(name);
                            }
                            player->ReqQiShiBanPlayCount(nameList);
                        }
                        */
                    default:
                        break;
                }
            }
        }
        break;
        case 0x26:
        {
            if(!World::getOldManTime())
                return ;
            brd >> op;
            if(op ==2)
            {
                UInt8 index =0 ;
                brd >> index ;
                if(index == 0)
                {
                    UInt32 type = GObject::World::FindTheOldMan(player);
                    if( type ==0 )
                    {
                        player->sendMsgCode(0, 1910);
                        break;
                    }
                    GameMsgHdr h(0x355,  player->getThreadId(), player, sizeof(UInt32));
                    GLOBAL().PushMsg(h, &type);
                    break;
                }
            }
            hdr.msgHdr.desWorkerID = player->getThreadId();
            GLOBAL().PushMsg(hdr, (void*)data);
            break;
        }
        break;
        case 0x28:
        {
            hdr.msgHdr.desWorkerID = player->getThreadId();
            GLOBAL().PushMsg(hdr, (void*)data);
            break;
        }
        case 0x30:
        {
            if(!World::getGuankaAct() || player->GetLev() < 65)
                return;
            brd >> op;
            switch(op)
            {
                case 0:
                    player->sendguankaActMyRank();
                    break;
                case 1:
                    {
                        UInt16 count = 0;
                        brd >> count;

                        Stream st(REP::ACT);
                        st << static_cast<UInt8>(0x30) << static_cast<UInt8>(0x01);
                        st << count;

                        for(UInt32 i = 0; i < count; i ++)
                        {
                            std::string openId;
                            openId.clear();
                            brd >> openId;
                            std::string info;
                            bool ret = WORLD().GetMemCach_guankaActInfo(openId.c_str(), info);
                            if (ret)
                            {
                                StringTokenizer tokenizer(info, "_");
                                if (tokenizer.count() >= 3)
                                {
                                    UInt8 csex = atoi(tokenizer[1].c_str());
                                    UInt16 score = atoi(tokenizer[2].c_str());
                                    st << tokenizer[0] << csex << score;
                                }
                                else
                                    ret = false;
                            }

                            if (!ret)
                            {
                                st << std::string("未找到");
                                st << static_cast<UInt8>(0) << static_cast<UInt16>(0);
                            }
                        }
                        st << Stream::eos;
                        player->send(st);
                    }
                    break;
                case 2:
                    {
                        GameMsgHdr hdr(0x1B8, WORKER_THREAD_WORLD, player, 0);
                        GLOBAL().PushMsg(hdr, NULL);
                    }
                    break;
                case 3:
                    {
                        UInt8 type = 0xFF;
                        brd >> type;
                        player->doGuankaAct(type);
                    }
                    break;
                case 4:
                    {
                        UInt8 type = 0xFF;
                        brd >> type;
                        player->getguankaScoreAward(type);
                    }
                    break;
            }
        }
        break;
        default:
            break;
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
            GObject::townDeamonManager->sendTjItemInfo(player);
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
        break;
    case 0x09:
        {
            UInt8 townId = 0;
            br >> townId;
            GObject::townDeamonManager->getTjItem(player, townId);
        }
        break;
    default:
        return;
    }
}

void OnSingleHeroReq( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;

    switch(op)
    {
    case 0x00:
        {
            UInt8 type = 0;
            br >> type;
            switch(type)
            {
            case 0x00:
                GObject::shStageMgr.sendActive(player);
                break;
            case 0x01:
                if(GObject::shStageMgr.isOpen())
                {
                    UInt32 fgtId = 0;
                    br >> fgtId;
                    GObject::Fighter* fgt = player->findFighter( fgtId );
                    if(fgtId > 6 || !fgt)
                        break;
                    GObject::shStageMgr.enter(player, fgt);
                }
                break;
            case 0x02:
                {
                    UInt32 rptId = 0;
                    br >> rptId;
                    std::vector<UInt8> *r = Battle::battleReport[rptId];
                    if(r == NULL)
                        return;
                    player->send(&(*r)[0], r->size());
                }
                break;
            case 0x03:
                break;
            case 0x04:
                if(GObject::shStageMgr.isOpen())
                // 人气投票
                {
                    UInt8 cls = 0;
                    UInt64 playerId = 0;
                    br >> cls;
                    br >> playerId;
                    GObject::Player * votePlayer = GObject::globalPlayers[playerId];
                    if(votePlayer == NULL)
                        return;
                    GObject::shStageMgr.voteForHero(player, cls, votePlayer);
                }
                break;
            }
        }
        break;
    case 0x01:
        {
            UInt8 type = 0;
            UInt8 cls = 0;
            UInt8 state = 0;
            br >> type >> cls >> state;

            switch(type)
            {
            case 0x00:
                GObject::shStageMgr.sendProgress(player);
                break;
            case 0x01:
                GObject::shStageMgr.sendStatus(player);
                break;
            case 0x02:
                GObject::shStageMgr.sendSHStageInfo(player, cls, state);
                break;
            case 0x03:
                GObject::shStageMgr.sendSYStageInfo(player, cls, state);
                break;
            case 0x04:
                GObject::shStageMgr.sendSRStageInfo(player, cls, state);
                break;
            case 0x05:
                GObject::shStageMgr.sendSupportInfo(player);
                break;
            case 0x06:
                GObject::shStageMgr.sendTowerInfo(player, cls);
                break;
            }
        }
        break;
    case 0x02:
        {
            UInt8 type = 0;
            br >> type;
            switch(type)
            {
            case 0x01:
                GObject::shStageMgr.sendLeaderBoard(player);
                break;
            case 0x02:
                GObject::shStageMgr.sendSHLeaderBoard(player);
                break;
            case 0x03:
                GObject::shStageMgr.sendSYLeaderBoard(player);
                break;
            case 0x04:
                GObject::shStageMgr.sendSRLeaderBoard(player);
                break;
            case 0x05:
                GObject::shStageMgr.sendSupportLeaderBoard(player);
                break;
            case 0x06:
                GObject::shStageMgr.sendTowerLeaderBoard(player);
                break;
            }
        }
        break;
    case 0x03:
        {
            UInt8 trump_cnt = 0;
            UInt8 citta_cnt = 0;
            std::vector<UInt16> trumps;
            std::vector<UInt16> cittas;

            br >> trump_cnt;
            if(trump_cnt)
            {
                trumps.resize(trump_cnt);
                br.read((UInt8*)(&trumps[0]), sizeof(UInt16)*trump_cnt);
            }
            br >> citta_cnt;
            if(citta_cnt)
            {
                cittas.resize(citta_cnt);
                br.read((UInt8*)(&cittas[0]), sizeof(UInt16)*citta_cnt);
            }
            GObject::shStageMgr.saveStrategyFighter(player, trumps, cittas);
        }
        break;
    case 0x04:
        {
            GObject::shStageMgr.showStrategyFighter(player);
        }
        break;
    }

}

//组队跨服战
void OnTeamArenaEntered( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt64 teamId;
	UInt8 entered;
	std::string rname;
    br >> teamId >> entered >> rname;
    TeamArenaData * tad = GObject::globalTeamArena[teamId];
    if(!tad)
        return;
    GObject::teamArenaMgr.teamArenaEntered(tad, entered, rname);
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        if(tad->members[i])
            tad->members[i]->arenaUdpLog(1001);
    }
}

void OnTeamArenaConnected( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
	UInt8 r = 0;
	brd >> r;
	if(r == 1)
	{
		INFO_LOG("Failed to connect to team arena.");
		NETWORK()->CloseArena();
		return;
	}
    UInt8 fhaslater = 0;
    brd >> fhaslater;
    GObject::teamArenaMgr._readbuf.append((UInt8*)(data) + brd.pos(), brd.size() - brd.pos());
    if(!fhaslater)
    {
        BinaryReader brd2((UInt8*)(GObject::teamArenaMgr._readbuf), GObject::teamArenaMgr._readbuf.size());
        GObject::teamArenaMgr.readFrom(brd2);
        GObject::teamArenaMgr._readbuf.clear();
    }
}

void OnTeamArenaPreliminary( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::teamArenaMgr.pushPreliminary(br);
}

void OnTeamArenaBattlePoint( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::teamArenaMgr.updateBattlePoint(br);
}

void OnTeamArenaInspireLevel( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::teamArenaMgr.updateInspireLevel(br);
}

void OnTeamArenaLeaderBoard( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::teamArenaMgr.updateLeaderBoard(br);
}

void OnTeamArenaLastRank( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    GObject::teamArenaMgr.updateLastRank(br);
}

void OnUpdateArenaSession( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt16 session = 0, session1 = 0;
    brd >> session >> session1;
    if(!session || !session1)
        return;
    GObject::arena.setSession(session);
    GObject::teamArenaMgr.setSession(session1);
}

//跨服服战
void OnServerWarConnected( ServerWarMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    if(!cfg.enabledServerWar())
        return;
	UInt8 r = 0;
	brd >> r;
	if(r == 1)
	{
		INFO_LOG("Failed to connect to ServerWar arena.");

		NETWORK()->CloseServerWar();
		return;
	}
    UInt8 fhaslater = 0;
    brd >> fhaslater;
    GObject::serverWarMgr._readbuf.append((UInt8*)(data) + brd.pos(), brd.size() - brd.pos());
    if(!fhaslater)
    {
        BinaryReader brd2((UInt8*)(GObject::serverWarMgr._readbuf), GObject::serverWarMgr._readbuf.size());
	    GObject::serverWarMgr.readFrom(brd2);
        GObject::serverWarMgr._readbuf.clear();
    }
}

void OnServerWarPlayerEntered( ServerWarMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    int cid = 0, sid = 0;
    brd >> cid >> sid;
	if(sid != cfg.serverNo || cid != cfg.channelNum)
		return;
    GVAR.SetVar(GVAR_SERVERWAR_ISENTER, 1);

    SYSMSGV(title, 825);
    SYSMSGV(content, 826, GObject::serverWarMgr.getSession());
    GObject::serverWarMgr.sendTeamMail(title, content);
}

void OnServerWarLineupCommited( ServerWarMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 r;
	UInt64 playerId;
    brd >> r >> playerId;
	GObject::Player * player = GObject::globalPlayers[playerId];
	if(player == NULL)
		return;
    Stream st(REP::SERVERWAR_ARENA_OP);
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x04);
    st << r << Stream::eos;
    player->send(st);
}

void OnServerWarPreliminary( ServerWarMsgHdr& hdr, const void * data )
{
    if(!cfg.enabledServerWar())
        return;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    GObject::serverWarMgr.pushPreliminary(brd);
}

void OnServerWarBattleReport( ServerWarMsgHdr& hdr, const void * data )
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

void OnServerWarSupport( ServerWarMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    UInt8 flag = 0;
    UInt16 pos = 0;
    brd >> type >> flag >> pos;
    switch(GObject::World::getArenaState())
    {
        case GObject::ARENA_XIANJIE_CHUANQI:
            GObject::serverWarMgr.updateSuport(type, flag, pos);
            break;
    }
}

void OnServerWarBattlePoint( ServerWarMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    GObject::serverWarMgr.updateBattlePoint(brd);
}

void OnServerWarLeaderBoard( ServerWarMsgHdr& hdr, const void * data )
{
    if(!cfg.enabledServerWar())
        return;
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    GObject::serverWarMgr.updateLeaderBoard(brd);
}
void OnMarryBard( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);

	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 op = 0;
    br >> op;
    UInt8 mType = GObject::MarryBoard::instance()._type;
    switch(op)
    {
        case 0x03:
        case 0x04:
            {
                hdr.msgHdr.desWorkerID = player->getThreadId();
                GLOBAL().PushMsg(hdr, (void*)data);
            }
            break;
        case 0x23:
        case 0x24:
            {
                if(mType != 2)
                    return;
                UInt8 ans = 0 ;
                br >>ans;
                GObject::MarryBoard::instance().answerTheQuestionOn2(player,ans);  
                Stream st(REP::MARRYBOARD);
                st <<static_cast<UInt8>(op);
                st <<static_cast<UInt8>(ans);
                st<<Stream::eos;
                player->send(st);
            }
            break;
        case 0x42:
            {
                if(mType != 3)
                    return;
                player->SetVar(VAR_MARRYBOARD3,1);
                UInt32 rand = uRand(10000);
                player->SetVar(VAR_MARRYBOARD3_KEY,rand);
                Stream st(REP::MARRYBOARD);
                st <<static_cast<UInt8>(0x42);
                st << static_cast<UInt32>(GObject::MarryBoard::instance().wrapTheKey(player->GetVar(VAR_MARRYBOARD3_KEY)));
                st<<Stream::eos;
                player->send(st);
            }
            break;
        case 0x43:
            {
                if(mType != 3)
                    return;
                UInt32 outKey = 0;
                UInt8 flag = 0;
                UInt32 now = TimeUtil::Now();
                UInt32 var = player->GetVar(VAR_MARRYBOARD4_TIME);
                br >> outKey ;
                if(GObject::MarryBoard::instance().unWrapTheOutKey(outKey) == player->GetVar(VAR_MARRYBOARD3_KEY))
                {
                    if(now - var > 16)
                    {
                        flag = 1 ; 
                        player->AddVar(VAR_MARRYBOARD3,1);
                        player->SetVar(VAR_MARRYBOARD4_TIME , now);
                        UInt32 rand = uRand(9900)+ 100 ;
                        player->SetVar(VAR_MARRYBOARD3_KEY,rand);
                        player->AddVar(VAR_MARRYBOARD_LIVELY,15);
                        GObject::MarryBoard::instance()._lively += 1;
                        char str[16] = {0};
                        sprintf(str, "F_140102_15");
                        player->udpLog("jiehunjinxing", str, "", "", "", "", "act");
                    }
                }
                else
                    break ;
                Stream st(REP::MARRYBOARD);
                st <<static_cast<UInt8>(0x43);
                st <<static_cast<UInt8>(flag);
                if(flag)
                    st << static_cast<UInt32>(GObject::MarryBoard::instance().wrapTheKey(player->GetVar(VAR_MARRYBOARD3_KEY)));
                else 
                    st <<static_cast<UInt32>(var + 16 -now );
                st<<Stream::eos;
                player->send(st);
            }
            break;
        case 0x44:
            {
                if(mType != 3)
                    return;
                player->getMarryBoard3Award(GObject::MarryBoard::instance()._norms);
                Stream st(REP::MARRYBOARD);
                st <<static_cast<UInt8>(0x44);
                st <<Stream::eos;
                player->send(st);
            }
            break;
        case 0x63:
        case 0x64:
            {
                if(mType != 4)
                    return;
                UInt8 door=0 ;
                br >> door;
                if( GObject::MarryBoard::instance().selectDoor(player,door) )
                {
                    Stream st(REP::MARRYBOARD);
                    st <<static_cast<UInt8>(op);
                    st << static_cast<UInt8>(door);
                    st<<Stream::eos;
                    if(player == GObject::MarryBoard::instance()._man || player == GObject::MarryBoard::instance()._woman)
                    {
                        GObject::MarryBoard::instance()._man ->send(st);
                        GObject::MarryBoard::instance()._woman->send(st);
                    }
                    else
                        player->send(st);
                }
            }
            break;
        default:
            return;
    }
}

void OnServerRechargeRank( ServerWarMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;
    if(type == 0)
        GObject::leaderboard.giveRechargeRankAward();
    else if(type == 1)
        GObject::leaderboard.readRechargeRank100(brd);
    else if(type == 2)
        GObject::leaderboard.readRechargeSelf(brd);
    else if(type == 3)
        GObject::leaderboard.sendGoldLvlAward(brd);
}
void OnServerLeftConnected( ServerLeftMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    if(!cfg.enabledServerWar())
        return;
	UInt8 r = 0;
	brd >> r;
	if(r == 1)
	{
		INFO_LOG("Failed to connect to ServerWar arena.");
        WORLD().setLeftAddrConnection(0);
		NETWORK()->CloseServerLeft();
		return;
	}
    UInt8 fhaslater = 0;
    brd >> fhaslater;
    GObject::serverWarMgr._readbuf.append((UInt8*)(data) + brd.pos(), brd.size() - brd.pos());
    if(!fhaslater)
    {
        BinaryReader brd2((UInt8*)(GObject::serverWarMgr._readbuf), GObject::serverWarMgr._readbuf.size());
	    GObject::serverWarMgr.readFrom(brd2);
        GObject::serverWarMgr._readbuf.clear();
    }
}

void OnServerLeftPlayerEntered( ServerLeftMsgHdr& hdr, const void * data )
{
    WORLD().setLeftAddrConnection(1);
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    int cid = 0, sid = 0;
    brd >> cid >> sid;
	if(sid != cfg.serverNo || cid != cfg.channelNum)
		return;
//    GVAR.SetVar(GVAR_SERVERWAR_ISENTER, 1);

    UInt32 clanId = 0;
    UInt8 type = 0;
    UInt8 leftId = 0; 
    std::string name ;
    UInt8 res = 0;
    UInt32 rpid = 0; 
    UInt32 battleTime;
    UInt32 energy;
    brd >> clanId ;
    brd >> type ;
    brd >> leftId;
    brd >> name;
    brd >> res;
    brd >> rpid;
    brd >> battleTime;
    brd >> energy;
    Clan * clan = globalClans[clanId];
    if(!clan)
        return ;
    ClanBuildBattleInfo cbbi(leftId , name ,type,res , rpid ,battleTime,energy);
    if(clan->getBuildingOwner() == NULL)
        return ;
    clan->getBuildingOwner()->AddBattlesInfo(cbbi);
   // SYSMSGV(title, 825);
   // SYSMSGV(content, 826, GObject::serverWarMgr.getSession());
   // GObject::serverWarMgr.sendTeamMail(title, content);
}

void OnServerLeftLineupCommited( ServerLeftMsgHdr& hdr, const void * data )
{
	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 r;
	UInt64 playerId;
    brd >> r >> playerId;
	GObject::Player * player = GObject::globalPlayers[playerId];
	if(player == NULL)
		return;
    player->sendMsgCode(0,4033);
}
void OnServerLeftBattleReport( ServerLeftMsgHdr& hdr, const void * data )
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
void OnServerLeftRevInfo(ServerLeftMsgHdr& hdr, const void * data)
{
#define LEFTADDRMAX 30
#define POWERTOWERMAX 9
#define SERVERLEFT_MAX_MEMBER 5
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt64 playerId = 0;
    br >> playerId ;
	GObject::Player * player = GObject::globalPlayers[playerId];
    if(!player)
        return ;
    Stream st(REQ::CLAN_FAIRYLAND);
    st <<static_cast<UInt8>(2);
    std::vector<UInt8> buf;
    buf.resize(br.size()-8);
    br >> buf;
    st << buf;
    st << Stream::eos;
    player->send(st);
}
void OnServerLeftGetAward(ServerLeftMsgHdr& hdr, const void * data)
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 leftId = 0;
    UInt32 clanId = 0 ;
    br >> leftId ;
    SYSMSG_BROADCASTV(4304 , leftId);
    std::cout << "msgBorad" << std::endl;
    br >> clanId ;
    Clan * clan = globalClans[clanId];
    if(!clan)
        return ;
    UInt32 energy = 0;
    br >> energy;
    UInt8 num = 0;
    br >> num ;
    //std::vector<UInt32> item_vec ;
    //std::vector<UInt32> itemNum_vec;
    UInt32 item = 0;
    UInt32 itemNum = 0;
    for(UInt8 i = 0 ; i < num ; ++i )
    {
        br >> item ;
        br >>itemNum;
    //    item_vec.push_back(item);
    //    itemNum_vec.push(itemNum);
        clan->AddItem(item,itemNum);
        Stream st;
    }
}
void OnServerLeftGetSpirit(ServerLeftMsgHdr& hdr, const void * data)
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt32 clanId = 0 ;
    br >> clanId ;
    Clan * clan = globalClans[clanId];
    if(!clan)
        return ;
    UInt32 num = 0;
    br >> num ; 
    GObject::ClanBuildingOwner* buildingOwner = clan->getNewBuildOwner();
    
    if (buildingOwner)
        buildingOwner->addEnergy(num);
}
void OnServerLeftErrInfo(ServerLeftMsgHdr& hdr, const void * data)
{
   //0-操作成功 1-对共有遗迹进行守卫操作错误  2-该遗迹无该镇守成员 3-对象以及非该帮派占领 
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt64 playerId = 0 ;
    UInt32 clanId = 0;
    UInt8 err = 0;
    br >> playerId ;
    br >> clanId;
    br >> err ;
    Clan * clan = globalClans[clanId];
    GObject::Player * player = GObject::globalPlayers[playerId];
    if(!player)
        return ;
    if(player->getClan() != clan )
        return ;
    if(err == 0)
        return ;
    player->sendMsgCode(0,4028+err);
}
// 仙境遗迹协议请求处理
void OnClanFairyLandReq(GameMsgHdr& hdr,const void * data)
{
	MSG_QUERY_PLAYER(player);

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

    GObject::ClanBuildingOwner* buildingOwner = clan->getNewBuildOwner();
    
    if (buildingOwner)
        buildingOwner->processFromBrd(player, brd);
}
void OnServerLeftMemberLeave(ServerLeftMsgHdr& hdr, const void * data)
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt8 val = 0;
    br >> val;
    UInt32 var_val = 0;
    br >> var_val ;
    UInt8 num = 0;
    br >> num ;
    UInt64 playerId=  0;

    for(UInt8 i = 0 ; i < num ;++i)
    {
        br >> playerId;
        if( playerId ==0 )
            continue;
        GObject::Player * player = GObject::globalPlayers[playerId];
        if(!player)
            continue;
        if(val)
            player->setLeftAddrEnter(true);
        else
            player->setLeftAddrEnter(false);
        if( var_val)
        {
            UInt32 val = player->GetVar(GObject::VAR_LEFTADDR_POWER);
            if(val > var_val)
                player->SetVar(GObject::VAR_LEFTADDR_POWER , val - var_val);
        }
    }
}
void OnServerLeftMemberGet(ServerLeftMsgHdr& hdr, const void * data)
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt32 val = 0;
    br >> val;
    UInt8 num = 0;
    br >> num ;
    UInt64 playerId=  0;
    Clan * clan = NULL;
    for(UInt8 i = 0 ; i < num ;++i)
    {
        br >> playerId;
        if( playerId ==0 )
            continue;
        GObject::Player * player = GObject::globalPlayers[playerId];
        if(!player)
            continue;
        if(!clan)
            clan = player->getClan();
        if(!clan)
            continue;
        clan->addMemberProffer(player, val);
    }
}
void OnServerLeftNotice(ServerLeftMsgHdr& hdr, const void * data)
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
    UInt32 clanId = 0 ;
    br >> clanId ;
    Clan * clan = globalClans[clanId];
    if(!clan)
        return ;
    UInt8 leftId = 0;
    br >> leftId ;
    UInt32 num = 0;
    br >> num ; 
    clan->SendLeftAddrMail(num ,leftId);
}
#endif // _WORLDOUTERMSGHANDLER_H_

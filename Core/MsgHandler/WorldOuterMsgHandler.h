#ifndef _WORLDOUTERMSGHANDLER_H_
#define _WORLDOUTERMSGHANDLER_H_

#include "MsgTypes.h"
#include "MsgFunc.h"

#include "Common/Serialize.h"

#include "Server/WorldServer.h"
#include "Server/OidGenerator.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "GObject/Arena.h"
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

#include <mysql.h>

struct ClanListReq
{
	UInt8 _type;
	UInt16 _startidx;
	UInt8 _count;
    UInt8 _flag;      // 0-∂Î·“ 1-¿•¬ÿ 2-»´≤ø
	std::string _name;
	MESSAGE_DEF5(0x90, UInt8, _type, UInt16, _startidx, UInt8, _count, UInt8, _flag, std::string, _name);
};

struct ClanInfoReq
{
	MESSAGE_DEF(0x91);
};

struct ClanCreateReq
{
	std::string _name;
	std::string _contact;
	std::string _purpose;
	MESSAGE_DEF3(0x92, std::string, _name, std::string, _contact, std::string, _purpose);
};

struct ClanApplyReq
{
	UInt32 _id;
	MESSAGE_DEF1(0x93, UInt32, _id);
};

struct ClanModifyReq
{
	UInt8 _op;
	std::string _text;
	MESSAGE_DEF2(0x95, UInt8, _op, std::string, _text);
};

struct ClanBattleReportReq
{
	UInt32 _clanId;
	UInt32 _rptId;
	MESSAGE_DEF2(0x7D, UInt32, _clanId, UInt32, _rptId);
};

struct ClanGetDynamicMsgReq
{
	UInt8 _type;
	UInt16 _start;
	UInt16 _count;
	MESSAGE_DEF3(0x7C, UInt8, _type, UInt16, _start, UInt16, _count);
};

struct ClanQueryReq
{
	UInt32 _id;
	MESSAGE_DEF1(0x96, UInt32, _id);
};

struct ClanQuery2Req
{
	UInt8 _type;
	MESSAGE_DEF1(0x97, UInt8, _type);
};

#if 0
struct ClanPersonalRewardReq
{
	MESSAGE_DEF(0x9B);
};
#endif

struct ClanRewardReq
{
	MESSAGE_DEF(0x9C);
};

struct ClanPersonalRewardOpReq
{
	UInt8 _type;
	UInt32 _allocTime;
	UInt16 _itemId;
	MESSAGE_DEF3(0x9D, UInt8, _type, UInt32, _allocTime, UInt16, _itemId);
};

struct ClanAllocRecordReq
{
	MESSAGE_DEF(0x9F);
};

struct StoreListReq
{
	UInt8 _type;
	MESSAGE_DEF1(0xB0, UInt8, _type);
};

struct LeaderboardReq
{
	UInt8 _type;
	UInt32 _id;
	MESSAGE_DEF2(0xE8, UInt8, _type, UInt32, _id);
};

struct OwnLeaderboardReq
{
	UInt32 _id;
	MESSAGE_DEF1(0xE9, UInt32, _id);
};

struct PlayerIdReq
{
	std::string _name;
	MESSAGE_DEF1(0xF2, std::string, _name);
};

struct BattleReportReq
{
	UInt32 _reportId;
	MESSAGE_DEF1(0x6C, UInt32, _reportId);
};

struct SaleListReq
{
	UInt16 _start;
	UInt16 _count;
	UInt8  _search;
	std::string _name;
	UInt8  _req;
	UInt8  _sort;
	UInt8  _color;
	UInt8  _eqType;
	MESSAGE_DEF8(0xC5, UInt16, _start, UInt16, _count, UInt8, _search, std::string, _name, UInt8, _req, UInt8, _sort, UInt8, _color, UInt8, _eqType);
};

struct SaleBuyAndCancelReq
{
	UInt8  _token;
	UInt32 _id;
	MESSAGE_DEF2(0xC7, UInt8, _token, UInt32, _id);
};

struct PracticePlaceReq
{
	MESSAGE_DEF(0xE0);
};

struct PracticeListReq
{
	UInt8 _type;
    UInt8 _pageno;
    UInt8 _pagenum;
	MESSAGE_DEF3(0xE1, UInt8, _type, UInt8, _pageno, UInt8, _pagenum);
};

struct PracticeChallengeReq
{
	UInt8 _type;
    UInt8 _idx;
    std::string _name;
	MESSAGE_DEF3(0xE2, UInt8, _type, UInt8, _idx, std::string, _name);
};

struct PracticePayReq
{
    UInt8 _place;
    UInt8 _slot;
    UInt8 _type;
    UInt8 _ptype;
	MESSAGE_DEF4(0xE3, UInt8, _place, UInt8, _slot, UInt8, _type, UInt8, _ptype);
};

struct PracticeSitDownReq
{
    UInt8 _type;
    UInt32 _fgtid;
	MESSAGE_DEF2(0xE4, UInt8, _type, UInt32, _fgtid);
};

struct PracticeStopReq
{
	MESSAGE_DEF(0xE5);
};

struct AthleticsListReq
{
	MESSAGE_DEF(0xD0);
};


struct AthleticsChallengeReq
{
	std::string _name;
	MESSAGE_DEF1(0xD2, std::string, _name);
};

struct ArenaInfoReq
{
	MESSAGE_DEF(0xEA);
};

struct ArenaEliminationReq
{
	UInt8 group;
	MESSAGE_DEF1(0xEB, UInt8, group);
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
		Stream st(0x91);
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
		Stream st(0x92);
		st << static_cast<UInt8>(4) << Stream::eos;
		player->send(st);
		return;
    }
    if(player->getCountry() > 1)
    {
		Stream st(0x92);
		st << static_cast<UInt8>(5) << Stream::eos;
		player->send(st);
		return;
    }
	if(player->getClan() != NULL || ccr._name.length() > 15 || GObject::globalGlobalNamedClans[player->fixName(ccr._name)] != NULL)
	{
		Stream st(0x92);
		st << static_cast<UInt8>(1) << Stream::eos;
		player->send(st);
		return;
	}
	if(player->getTael() < 100) // XXX:
	{
		Stream st(0x92);
		st << static_cast<UInt8>(3) << Stream::eos;
		player->send(st);
		return;
	}
	UInt32 id = IDGenerator::gClanOidGenerator.ID();
	if(cfg.merged && player->getId() >= 0x1000000000000ull)
	{
		UInt32 svno = static_cast<UInt32>(player->getId() >> 48);
		id |= (svno << 24);
	}
		
	GObject::Clan * clan = new(std::nothrow) GObject::Clan(id, ccr._name);
	if(clan == NULL)
	{
		Stream st(0x92);
		st << static_cast<UInt8>(4) << Stream::eos;
		player->send(st);
		return;
	}
	clan->setContact(ccr._contact, false);
	clan->setPurpose(ccr._purpose, false);
	clan->setFounder(player->getId());
	Stream st(0x92);
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
	mysql_escape_string(contact2, ccr._contact.c_str(), ccr._contact.length());
	mysql_escape_string(purpose2, ccr._purpose.c_str(), ccr._purpose.length());
	mysql_escape_string(name2, strNametmp.c_str(), strNametmp.length());
	DB().PushUpdateData("INSERT INTO `clan` (`id`, `name`, `foundTime`, `founder`, `leader`, `construction`, `contact`, `purpose`) VALUES (%u, '%s', %u, %"I64_FMT"u, %"I64_FMT"u, %"I64_FMT"u, '%s', '%s')", clan->getId(), name2, TimeUtil::Now(), player->getId(), player->getId(), 0, contact2, purpose2);
	ConsumeInfo ci(ClanCreate,0,0);
	player->useTael(100,&ci);
	clan->initBuildClan();
	SYSMSG_SEND(120, player);
	SYSMSG_SENDV(1020, player, clan->getName().c_str());
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
		Stream st(0x93);
		st << static_cast<UInt8>(1) << Stream::eos;
		player->send(st);
		return;
	}

	bool r = clan->apply(player);
	if(r)
	{
		SYSMSG_SENDV(119, player, clan->getName().c_str());
		SYSMSG_SENDV(1019, player, clan->getName().c_str());
	}

	Stream st(0x93);
	st << static_cast<UInt8>(r ? 0: 1) << Stream::eos;
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
		}
	}
	Stream st(0x94);
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
	Stream st(0x95);
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
	if(clan == NULL /*TODO: ¬º√¨¬≤√©√à√ã√ä√Ω√ä√á¬∑√±¬¥√Ø¬µ¬Ω√â√è√è√û*/)
	{
		Stream st(0x96);
		st << static_cast<UInt32>(0) << "" << "" << Stream::eos;
		player->send(st);
		return;
	}

	Stream st(0x96);
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
	Stream st(0x9E);
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
					player->sendMsgCode(0, 2210);
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
		case 3:
#if 0
			{
				UInt8 skillId;
				brd	>> skillId;
				if(skillId < 7 || skillId > 10)
					return;
				UInt16 favorId = clan->getFavorItemId(skillId);
				Stream st(0x99);
				st << static_cast<UInt8>(5) << skillId << favorId << Stream::eos;
				player->send(st);
			}
#endif
			clan->listSkills(player);
			break;
        case 4:
			{
				UInt8 skillId;
				brd >> skillId;
				clan->showSkill(player, skillId);
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
				Stream st(0x79);
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
					Stream st(0x79);
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
					Stream st(0x79);
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
				player->sendMsgCode(0, 2220);
				return;
			}
			if (clan == allyClan)
				r = false;
			if(!player->hasChecked())
				return;
			if (allyClan->getClanBattle()->isInBattling() || clan->getClanBattle()->isInBattling())
			{
				player->sendMsgCode(0, 2210);
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
						//Ω·√À
						if (clan->hasAllyClan())
						{
							r = false;
							player->sendMsgCode(0, 2211);
						}
						if (clan->hasEnemyClan(allyClan))
						{
							r = false;
							player->sendMsgCode(0, 2230);
						}
						if (r && allyClan->hasAllyClan())
						{
							r = false;
							player->sendMsgCode(0, 2212);
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
						// ˜¡¢µ–√À
						if (clan->hasEnemyClan(allyClan))
						{
							r = false;
							player->sendMsgCode(0, 2232);
						}
						if (clan->hasAllyClan(allyClan))
						{
							r = false;
							player->sendMsgCode(0, 2231);
						}
						if (r)
							clan->addEnemyClan(player, allyClan);
					}
					break;
				case 4:
					{
						//Ω‚≥˝µ–√À
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
				Stream st(0x79);
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
			Stream st(0x79);
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
			Stream st(0x79);
			st << static_cast<UInt8>(4) << Stream::eos;
			player->send(st);
		}
		break;
	case 5:
		{
			if (!clan->hasClanAuthority(player, 4))
				return;
			Stream st(0x79);
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
				Stream st(0x79);
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
			Stream st(0x79);
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
	MESSAGE_DEF6(0xF2, UInt64, id, std::string, name, UInt8, cny, UInt8, sex, UInt8, office, UInt8, guard);
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
		player->send(*st);
	}
	else
	{
		UInt8 failed_packet[9] = {0x05, 0x00, 0xFF, 0xE8, lr._type, 0x00, 0x00, 0x00, 0x00};
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

void OnSaleListReq( GameMsgHdr& hdr, SaleListReq& req )
{
	MSG_QUERY_PLAYER(player);
	GObject::gSaleMgr.requestSaleList(player, req._start, req._count, req._search, req._name, req._req, req._sort, req._color, req._eqType);
}

void OnSaleBuyAndCancelReq( GameMsgHdr& hdr, SaleBuyAndCancelReq& req )
{
	MSG_QUERY_PLAYER(player);
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
}

void OnAthleticsListReq( GameMsgHdr& hdr, AthleticsListReq&)
{
	MSG_QUERY_PLAYER(player);
	GObject::gAthleticsRank.requestAthleticsList(player);
}

void OnAthleticsChallengeReq( GameMsgHdr& hdr, AthleticsChallengeReq& req )
{
	MSG_QUERY_PLAYER(player);
	GObject::gAthleticsRank.challenge(player, req._name);
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
	Stream st(0xEC);
	st << static_cast<UInt8>(1) << r << static_cast<UInt8>(entered + 1) << rname << Stream::eos;
	player->send(st);
}

void OnLineupCommited( ArenaMsgHdr& hdr, const void * data )
{

}

void OnArenaPriliminary( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt64 playerId = 0;
	UInt8 won = 0;
	UInt8 color;
	std::string name;
	UInt32 btime;
	UInt32 bid;
	br >> playerId >> won >> color >> name >> btime >> bid;
	float rate = 0.0f;
	if(won == 2)
		br >> rate;
	GObject::Player * player = GObject::globalPlayers[playerId];
	if(player == NULL)
		return;
	GObject::arena.pushPriliminary(player, won, color, name, btime, bid, rate);
	Stream st(0xED);
	st << static_cast<UInt8>(1) << won << color << name << btime << bid << Stream::eos;
	player->send(st);
}

void OnPriliminaryInfo( ArenaMsgHdr& hdr, const void * data )
{
	BinaryReader br(data, hdr.msgHdr.bodyLen);
	UInt32 r[3] = {0};
	br >> r[0] >> r[1] >> r[2];
	GObject::arena.pushPriliminaryCount(r);
}

void OnArenaInfoReq( GameMsgHdr& hdr, ArenaInfoReq& )
{
	MSG_QUERY_PLAYER(player);
	GObject::arena.sendInfo(player);
}

void OnArenaEliminationReq( GameMsgHdr&hdr, ArenaEliminationReq& aer )
{
	MSG_QUERY_PLAYER(player);
	GObject::arena.sendElimination(player, aer.group);
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
			Stream st(0xEC);
			st << type << static_cast<UInt8>(GObject::arena.active() ? 1 : 0) << Stream::eos;
			player->send(st);
		}
		break;
	case 1:
		GObject::Arena::enterArena(player);
		break;
	case 2:
		{
			UInt8 pos = 0, tael = 0;
			brd >> pos >> tael;
			if(tael > 0)
			{
				UInt8 r = GObject::arena.bet(player, (pos >> 5) - 1, pos & 0x1F, tael);
				Stream st(0xEC);
				st << type << r << pos << tael << Stream::eos;
				player->send(st);
			}
		}
		break;
	case 3:
		{
			Stream st(0xEC);
			UInt32 pc[3];
			GObject::arena.getPlayerCount(pc);
			st << type << pc[0] << pc[1] << pc[2] << Stream::eos;
			player->send(st);
		}
		break;
	case 4:
		{
			UInt8 group = 0;
			UInt8 pos1 = 0, pos2 = 0;
			brd >> group >> pos1 >> pos2;
			Stream st(0xEC);
			st << type << group << pos1 << pos2 << GObject::arena.getBetCount(group - 1, pos1) << GObject::arena.getBetCount(group - 1, pos2) << Stream::eos;
			player->send(st);
		}
		break;
	}
}

#endif // _WORLDOUTERMSGHANDLER_H_

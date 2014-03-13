#ifndef _WORLDINNERMSGHANDLER_H_
#define _WORLDINNERMSGHANDLER_H_

#include "Common/Serialize.h"
#include "MsgTypes.h"
#include "MsgID.h"
#include "MsgFunc.h"
#include "CountryMsgStruct.h"
#include "Server/WorldServer.h"
#include "GObject/Fighter.h"
#include "GObject/Player.h"
#include "GObject/TaskMgr.h"
#include "GObject/EventBase.h"
#include "GObject/ClanBattle.h"
#include "GObject/Mail.h"
#include "GObject/TradeCheck.h"
#include "GObject/SaleMgr.h"
#include "GObject/Athletics.h"
#include "GObject/AthleticsRank.h"
#include "GObject/Dungeon.h"
#include "GObject/BlockBossMgr.h"
#include "Server/SysMsg.h"
#include "Script/WorldScript.h"
#include "Script/BattleFormula.h"
#include "GObject/SpecialAward.h"
#include "GObject/PracticePlace.h"
#include "GObject/ArenaBattle.h"
#include "GObject/World.h"
#include "Common/Itoa.h"
#include <set>
#include "GObject/SingleHeroStage.h"
#include "GObject/SHSYTmpl.h"
#include "GObject/DaysRank.h"
#include "GObject/ClanBoss.h"
#include "GObject/ClanCityBattle.h"
#include "GObject/ArenaTeam.h"
#include "GObject/ArenaServerWar.h"

void OnPushTimerEvent( GameMsgHdr& hdr, const void * data )
{
	GObject::EventBase* event = *reinterpret_cast<GObject::EventBase * const*>(data);
	GObject::eventWrapper.AddTimerEvent(event);
}


void OnPopTimerEvent( GameMsgHdr& hdr, const void * data )
{
	struct EventMsgData
	{
		UInt32 id;
		size_t data;
	};
	const EventMsgData* emd = reinterpret_cast<const EventMsgData *>(data);
	GObject::eventWrapper.DelTimerEvent(hdr.player, emd->id, emd->data);
}

void OnSearchEvents( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct EventQuery
	{
		bool autoBattle;
		bool autoDungeon;
	};
	const EventQuery * eq = reinterpret_cast<const EventQuery *>(data);
	if(eq->autoBattle)
	{
		GObject::EventWrapper::iterator it = GObject::eventWrapper.FindTimerEvent(player, EVENT_AUTOBATTLE, 0);
		if(it != GObject::eventWrapper.end())
		{
			GObject::EventBase * event = it->second;
			event->duplicate();
			if(event == NULL) return;

			GameMsgHdr hdr1(0x28C, player->getThreadId(), player, sizeof(GObject::EventBase *));
			GLOBAL().PushMsg(hdr1, &event);
		}
	}
	if(eq->autoDungeon)
	{
		GObject::EventWrapper::iterator it = GObject::eventWrapper.FindTimerEvent(player, EVENT_DUNGEONAUTO, 0);
		if(it != GObject::eventWrapper.end())
		{
			GObject::EventDungeonAuto * event = static_cast<GObject::EventDungeonAuto *>(it->second);
			GameMsgHdr hdr1(0x28D, player->getThreadId(), player, sizeof(GObject::Dungeon));
			GObject::Dungeon * dg = event->GetDungeon();
			GLOBAL().PushMsg(hdr1, &dg);
		}
	}
    //GObject::arena.sendActive(player);
}

void OnAthleticsOver( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct AthleticsResult
	{
		UInt32 id;
		UInt8 side;
		GObject::Player * defer;
		bool result;
	};
	AthleticsResult * ar = reinterpret_cast<AthleticsResult *>(const_cast<void *>(data));
	GObject::gAthleticsRank.notifyAthletcisOver(ar->side == 0 ? player : ar->defer, ar->side == 0 ? ar->defer : player, ar->id, ar->result);
}

void OnAthMartialOver( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct AthleticsResult
	{
		UInt32 id;
		UInt8 side;
		GObject::Player * defer;
		UInt8 result;
	};
	AthleticsResult * ar = reinterpret_cast<AthleticsResult *>(const_cast<void *>(data));
	GObject::gAthleticsRank.notifyAthMartialOver(ar->side == 0 ? player : ar->defer, ar->side == 0 ? ar->defer : player, ar->id, ar->result);
}

void OnAthleticsEnter( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt8 lev = *reinterpret_cast<UInt8 *>(const_cast<void *>(data));
	GObject::gAthleticsRank.enterAthleticsReq(player, lev);
}

void OnAthleticsAndClanNotify( GameMsgHdr& hdr, const void * )
{
	MSG_QUERY_PLAYER(player);
	GObject::gAthleticsRank.notifyAthletcisBoxFlushTime(player);
	Stream st(REP::DAILY_DATA);
	st << static_cast<UInt8>(3) << GObject::gAthleticsRank.getChallengeNum(player);
	GObject::Clan * clan = player->getClan();
	if(clan == NULL)
		st << static_cast<UInt8>(0);
	else
		st << clan->getClanBattle()->getEnterBattleClanCount(player);
	st << Stream::eos;
	player->send(st);
}

void OnClanChatReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(player->getClan() != NULL)
		player->getClan()->broadcast(data, hdr.msgHdr.bodyLen);
}

void OnSpreadModifyVar(GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);
    player->SetVar(VAR_SPREAD_FLAG, player->GetVar(VAR_SPREAD_FLAG) | SPREAD_ALREADY_GET);
}

void OnClanTakeRewardResultReq(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	if(player->getClan() == NULL)
		return;
	struct ClanTakeRewardResultStruct
	{
		bool result;
		UInt32 allocTime;
		GObject::AllocItem item;
	};
	const ClanTakeRewardResultStruct * ctrss = reinterpret_cast<const ClanTakeRewardResultStruct *>(data);
	player->getClan()->takeRewardResult(player, ctrss->result, ctrss->allocTime, ctrss->item);
}

void OnClanMailInviteClick(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	struct ClanMailClickInviteReq
	{
		UInt32 id;
		GObject::Player * inviter;
		UInt8 action;
	};
	const ClanMailClickInviteReq * cmcir = reinterpret_cast<const ClanMailClickInviteReq *>(data);
	if (player->getClan() != NULL)
		return;
	GObject::Player * inviter = cmcir->inviter;
	if (inviter == NULL || inviter->getClan() == NULL || inviter->getCountry() != player->getCountry())
		return;
	if (cmcir->action == 0)
	{
		inviter->getClan()->apply(player);
	}
	else
	{
		inviter->getClan()->declineInvite(player);
	}
	Stream st(REQ::MAIL_DELETE);
	st << static_cast<UInt8>(1) << cmcir->id << Stream::eos;
	player->send(st);
	player->GetMailBox()->delMail(cmcir->id, false);
}

void OnClanMailClick(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	struct ClanMailClickReq
	{
		UInt32 id;
		GObject::Player * applier;
		UInt8 action;
	};
	const ClanMailClickReq * cmcr = reinterpret_cast<const ClanMailClickReq *>(data);
	do
	{
		GObject::Clan * clan = player->getClan();
		if(clan == NULL)
		{
			player->sendMsgCode(2, 1313);
			break;
		}
		if (clan->getClanBattle()->isInBattling())
		{
			player->sendMsgCode(2, 1317);
			return;
		}
		GObject::Player * p = cmcr->applier;
		if(cmcr->action == 0)
		{
			if (p == NULL)
				break;
			GObject::Clan * applierClan = p->getClan();
			if (applierClan != NULL)
			{
				if (applierClan != clan)
					player->sendMsgCode(2, 1312);
				break;
			}
			if(!player->getClan()->accept(player, p->getId()))
				return;
			Stream st(REP::CLAN_MEMBER_OPERATE);
			st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << p->getId() << Stream::eos;
			player->send(st);
		}
		else
		{
			player->getClan()->decline(player, p->getId());
			Stream st(REP::CLAN_MEMBER_OPERATE);
			st << static_cast<UInt8>(3) << static_cast<UInt8>(1) << p->getId() << Stream::eos;
			player->send(st);
		}
	}
	while(0);
	Stream st(REP::MAIL_DELETE);
	st << static_cast<UInt8>(1) << cmcr->id << Stream::eos;
	player->send(st);
	player->GetMailBox()->delMail(cmcr->id, false);
}


void OnClanTechDonateCheckResp( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct DonateItems
	{
		UInt8  techId;
		UInt8 flag;
		UInt32 count;
		UInt8 ret;
	};
	UInt8 r = 2;
	const DonateItems * items = reinterpret_cast<const DonateItems *>(data);
	if (items->ret == 0)
	{
		GObject::Clan * clan = player->getClan();
		if (clan != NULL && clan->donate(player, items->techId, items->flag, items->count))
			r = 1;
	}
	Stream st(REP::CLAN_SKILL);
	st << static_cast<UInt8>(2) << r << Stream::eos;
	player->send(st);
}
void OnDonateClanbyGM( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct DonateItem
	{
		UInt8  skillId;
		UInt16 type;
		UInt32 count;
	};
	GObject::Clan * clan = player->getClan();
	const DonateItem * items = reinterpret_cast<const DonateItem *>(data);
		if(clan == NULL)
			return;
	clan->GMDonate(player, items->skillId, items->type, items->count);
}
void OnAddClanBoxGM( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct AddClanBox
	{
		GObject::Clan *cl;
		UInt8 count;
	};
	const AddClanBox * clb = reinterpret_cast<const AddClanBox *>(data);
	if(clb->cl == NULL || clb->count == 0)
		return;
	clb->cl->addRepoNum(9041, clb->count);
}
void OnClanAllyMailResp( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct ClanAllyClickReq
	{
		UInt32 clanId;
		bool   agree;
		GObject::Player * reciever;
	};
	const ClanAllyClickReq * cacr = reinterpret_cast<const ClanAllyClickReq *>(data);
	GObject::Clan * clan = player->getClan();
	GObject::Clan * allyClan = cacr->reciever->getClan();
	if (clan == NULL || allyClan == NULL)
		return;
	if (cacr->agree)
	{
		if (clan->getClanBattle()->isInBattling() || allyClan->getClanBattle()->isInBattling())
		{
			cacr->reciever->sendMsgCode(0, 1317);
			return;
		}
		if (clan->hasEnemyClan(allyClan))
		{
			//cacr->reciever->sendMsgCode(0, 2234);
			return;
		}
		if (allyClan->hasEnemyClan(clan))
		{
			//cacr->reciever->sendMsgCode(0, 2230);
			return;
		}
		if (clan->addAllyClan(player, cacr->reciever, allyClan))
		{
			SYSMSG(title, 350);
			SYSMSGV(content, 352, allyClan->getName().c_str(), cacr->reciever->getName().c_str());
			player->GetMailBox()->newMail(player, 0x25, title, content);
		}
	}
	else
	{
		SYSMSG(title, 350);
		SYSMSGV(content, 353, allyClan->getName().c_str(), cacr->reciever->getName().c_str());
		player->GetMailBox()->newMail(player, 0x25, title, content);
	}
}

void OnClanBattlerOffTimeCheck( GameMsgHdr& hdr, const void * )
{
	MSG_QUERY_PLAYER(player);
	GObject::ClanBattle * battleClan = player->getClanBattle();
	if (battleClan == NULL)
		return;
	battleClan->leaveClanCity(player);
}

void OnAddTradeCheckNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct TradeCheckData
	{
		UInt32 id;
		UInt32 time;
	};
	const TradeCheckData * trade = reinterpret_cast<const TradeCheckData *>(data);
	GObject::gTradeCheck.addTradeCheck(trade->id, trade->time, player);
}

void OnDelTradeCheckNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct TradeCheckData
	{
		UInt32 id;
		UInt32 time;
	};
	const TradeCheckData * trade = reinterpret_cast<const TradeCheckData *>(data);
	GObject::gTradeCheck.delTradeCheck(trade->id, trade->time);
}

void OnSearchSaleNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::SaleSearchResp * saleSearchResp = reinterpret_cast<GObject::SaleSearchResp *>(const_cast<void *>(data));
	GObject::gSaleMgr.searchPlayerSaleResp(saleSearchResp->founder, player, saleSearchResp->start, saleSearchResp->count, saleSearchResp->ids, 24);
}

void OnPutSaleNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::SalePut * sale = reinterpret_cast<GObject::SalePut *>(const_cast<void *>(data));
	GObject::gSaleMgr.sellSale(player, sale, 9);
}

void OnBuySaleCheckOKNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	const UInt32 id = *reinterpret_cast<const UInt32 *>(data);
	const UInt32 pos = *reinterpret_cast<const UInt32 *>((const UInt8 *)data + 4);
	GObject::gSaleMgr.addSaleItem(player, id, pos);
}

void OnDoInstantAutoBattleReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::EventWrapper::iterator it = GObject::eventWrapper.FindTimerEvent(player, EVENT_AUTOBATTLE, 0);
	if(it == GObject::eventWrapper.end())
		return;
	GObject::EventAutoBattle * event = static_cast<GObject::EventAutoBattle*>(it->second);
	if(event->instantComplete())
	{
		GObject::eventWrapper.DelTimerEvent(it);
	}
}

void OnDoCancelAutoBattleReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(player, EVENT_AUTOBATTLE, 0);
	if(ev == NULL)
		return;
	ev->release();
	//GameMsgHdr hdr2(0x279, player->getThreadId(), player, 0);
	//GLOBAL().PushMsg(hdr2, NULL);
}

void OnTrainAccelerateReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct TrainAccData
	{
		UInt32 fgtId;
		UInt32 accCount;
	};
	const TrainAccData * tad = reinterpret_cast<const TrainAccData *>(data);
	GObject::EventWrapper::iterator it = GObject::eventWrapper.FindTimerEvent(player, EVENT_FIGHTERAUTOTRAINING, tad->fgtId);
	if(it == GObject::eventWrapper.end())
		return;
	GObject::EventFighterTrain * event = static_cast<GObject::EventFighterTrain*>(it->second);
	if(event->Accelerate(tad->accCount))
	{
		GObject::eventWrapper.DelTimerEvent(it);
	}
}

void OnCancelDungeonAutoReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(player, EVENT_DUNGEONAUTO, 0);
	if(ev == NULL)
		return;
	GameMsgHdr hdr2(0x280, player->getThreadId(), player, sizeof(GObject::EventBase *));
	GLOBAL().PushMsg(hdr2, &ev);
}

void OnCompleteDungeonAutoReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    if (player->GetPackage()->GetRestPackageSize() == 0)
    {
        player->sendMsgCode(0, 1011);
        return;
    }

	if(player->getThreadId() != WORKER_THREAD_NEUTRAL)
		return;

	GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(player, EVENT_DUNGEONAUTO, 0);
	if(ev == NULL)
		return;
	GameMsgHdr hdr2(0x281, player->getThreadId(), player, sizeof(GObject::EventBase *));
	GLOBAL().PushMsg(hdr2, &ev);
}

void OnCancelJobHunterAutoReq( GameMsgHdr & hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
    GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(player, EVENT_JOBHUNTER, player->getId());
    if (ev)
        ev->release();
}

void OnReloadLuaReq( GameMsgHdr& hdr, const void * data )
{
	UInt16 flag = *static_cast<const UInt16 *>(data);
	if(flag & 0x10)
		WORLD().getWorldScript()->reload();
	if(flag & 0x01)
		WORLD().getBattleFormula()->reload();
	if(flag & 0xF0F)
	{
		GameMsgHdr hdr1(0x1EE, WORKER_THREAD_COUNTRY_1, NULL, sizeof(UInt16));
		GLOBAL().PushMsg(hdr1, &flag);
		GameMsgHdr hdr2(0x1EE, WORKER_THREAD_COUNTRY_2, NULL, sizeof(UInt16));
		GLOBAL().PushMsg(hdr2, &flag);
		GameMsgHdr hdr3(0x1EE, WORKER_THREAD_NEUTRAL, NULL, sizeof(UInt16));
		GLOBAL().PushMsg(hdr3, &flag);
	}
    WORLD().getWorldScript()->forceCommitArena();
    GObject::shsyTmpl.load();
}

void OnSpecialAward( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt8 type = *reinterpret_cast<UInt8 *>(const_cast<void *>(data));
	GObject::gSpecialAward.AddSpecialAwardList(type, player);
}

void OnUpdateBlockBossRank(  GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	const UInt8 *buff = reinterpret_cast<const UInt8 *>(data);
	UInt8 playerLevel = *buff;
	UInt16 bossLevel = *reinterpret_cast<const UInt8 *>(buff + 1);
	UInt16 couponCount = *reinterpret_cast<const UInt8 *>(buff + 2);
	UInt16 itemId = *reinterpret_cast<const UInt16 *>(buff + 4);
	UInt8 itemCount = *reinterpret_cast<const UInt8 *>(buff + 6);
	GObject::gBlockbossmgr.resetPlayerRank(player, bossLevel, playerLevel, couponCount, itemId, itemCount);
}

void OnBloackBossDataReq(  GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt16 bossLevel = *reinterpret_cast<const UInt8 *>(data);
	GObject::gBlockbossmgr.reqBlockBossData(player, bossLevel);
}

void OnFighterStandup( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    UInt32 fgtid = *(UInt32*)(data);
    UInt32 fgts[1] = {fgtid};
    GObject::practicePlace.standup(player, fgts, 1);
}

void OnArenaAddLevelMan( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    GObject::ArenaBattle::Instance().addLevelMan();
}

void OnRunscriptReq( GameMsgHdr&, const void * data )
{
	const char * script = reinterpret_cast<const char *>(data);
	WORLD().getWorldScript()->runScript(script);
}

void OnRunscriptBattleReq( GameMsgHdr&, const void * data )
{
	const char * script = reinterpret_cast<const char *>(data);
	Script::BattleFormula::getCurrent()->runScript(script);
}

void OnDoInstantPracticeAccReq( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);
	GObject::EventWrapper::iterator it = GObject::eventWrapper.FindTimerEvent(player, EVENT_PLAYERPRACTICING, player->getId());
	if(it == GObject::eventWrapper.end())
		return;
	GObject::EventPlayerPractice* event = static_cast<GObject::EventPlayerPractice*>(it->second);
	event->instantComplete();
    //player->practiceUdpLog();
}

bool enum_send_sh_active(void * ptr, void * data )
{
    GObject::Player* player = static_cast<GObject::Player*>(ptr);
    if(player == NULL)
        return true;

    GObject::shStageMgr.sendActive(player);
    return true;
}

void OnLevelChange( GameMsgHdr& hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);
    if(hdr.msgHdr.bodyLen != sizeof(GObject::LevelChange))
        return;
	const GObject::LevelChange* lvc = reinterpret_cast<const GObject::LevelChange*>(data);
    if(lvc->oldLv == lvc->newLv)
        return;
    if(lvc->oldLv > 28)
    {
        GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(lvc->oldLv);
        if(it != GObject::globalLevelsPlayers.end())
        {
            GObject::LevelPlayers* lvPlayer = it->second;
            UInt32 nSize = lvPlayer->size();
            UInt64* ppid = &(*lvPlayer)[0];
            for(UInt32 i = 0; i < nSize; ++i)
            {
                if(player->getId() == *ppid)
                {
                    lvPlayer->erase(lvPlayer->begin() + i);
                    break;
                }
                ++ppid;
            }
        }
    }
    if(lvc->newLv > 28)
    {
        GObject::LevelPlayers* lvPlayer = NULL;
        GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(lvc->newLv);
        if(it != GObject::globalLevelsPlayers.end())
             lvPlayer = it->second;

        if(lvPlayer == NULL)
        {
            lvPlayer = new GObject::LevelPlayers();
            GObject::globalLevelsPlayers[lvc->newLv] = lvPlayer;
        }

        lvPlayer->push_back(player->getId());
    }
    if(!GObject::shStageMgr.getActive())
    {
        if(lvc->oldLv < SINGLE_HERO_OPEN_LEVEL && lvc->newLv >= SINGLE_HERO_OPEN_LEVEL)
            GObject::shStageMgr.incActive(1);
        else if(lvc->oldLv >= SINGLE_HERO_OPEN_LEVEL && lvc->newLv < SINGLE_HERO_OPEN_LEVEL)
            GObject::shStageMgr.incActive(-1);
        if(GObject::shStageMgr.getActive())
        {
            GObject::shStageMgr.init(WORLD().Now(), WORLD().getWeekDay());
            GObject::globalPlayers.enumerate(enum_send_sh_active, static_cast<void *>(NULL));
        }
    }
    if(gClanCity && !gClanCity->hasValidateOpenTime() && lvc->newLv >= CCB_OPEN_LEVEL)
    {
        gClanCity->setOpenTime();
    }
}
#if 0
void OnAthleticsMartialFlush( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
    //GObject::gAthleticsRank.updateAthleticsMartial(player);
    player->GetAthletics()->updateAthleticsMartial(player);
}
#endif
#if 0
void OnAthleticsFlush( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
    UInt8 type = *(UInt8 *)data;
    //GObject::gAthleticsRank.updateAthleticsP(player, type);
    player->GetAthletics()->updateAthleticsP(player, type);
}
#endif
void OnAthleticsPayRet( GameMsgHdr& hdr,  const void* data)
{
    MSG_QUERY_PLAYER(player);

    const GObject::AthleticsPay * msg = reinterpret_cast<const GObject::AthleticsPay*>(data);
    if(msg == NULL)
        return;

    if(msg->type == GObject::AthleticsRank::AthleticsPayForPaging)
         GObject::gAthleticsRank.AddPageNum(player, msg->moneyEnough == 1);
    else if(msg->type == GObject::AthleticsRank::AthleticsPayForKillCD)
        GObject::gAthleticsRank.KillCD(player, msg->moneyEnough == 1);

}

void OnReleaseClanSkillLevelOp( GameMsgHdr& hdr,  const void* data)
{
    MSG_QUERY_PLAYER(player);
    player->setClanSkillFlag(0);
}

void OnMoveAllToMax( GameMsgHdr& hdr,  const void* data )
{
    GObject::practicePlace.moveAllToMax();
}

void OnReplaceOwner( GameMsgHdr& hdr,  const void* data )
{
    MSG_QUERY_PLAYER(newpl);
    UInt32 ranking = *(UInt32*)data;
    GObject::practicePlace.replaceOwner(newpl, ranking);
}

void OnPracticeAddExp( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    PracticeData* _data = practicePlace.getPracticeData(player->getId());
    if (!_data) {
        PopTimerEvent(player, EVENT_PLAYERPRACTICING, player->getId());
        return;
    }

    PracticeFighterExp pfexp;
    memset(&pfexp, 0, sizeof(pfexp));

    Fighter* fgt = 0;
    UInt8 n = 0;
    for (std::list<UInt32>::iterator i = _data->fighters.begin(); i != _data->fighters.end(); ++i)
    {
        fgt = player->findFighter(*i);
        if (fgt)
        {
            if(n < sizeof(pfexp.fids)/sizeof(UInt32))
            {
                pfexp.fids[n] = *i;
                pfexp.counts[n] = 10;
                ++ n;
            }
        }
    }

    if (n)
    {
        GameMsgHdr hdr1(0x320, player->getThreadId(), player, sizeof(PracticeFighterExp));
        GLOBAL().PushMsg(hdr1, &pfexp);
    }
}

void OnPracticeStop( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    practicePlace.stop(player);
}

void OnLuckyDraw( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    UInt8 times = *(UInt32*)data;
    UInt32 oldCnt = player->GetVar(VAR_LUCKYDRAW_CNT);
    player->AddVar(VAR_LUCKYDRAW_CNT, times);
    SYSMSG(title, 2364);
    SYSMSG(content, 2365);
    if((oldCnt < 20) && ((oldCnt + times) >=20))
    {
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem mitem[2] = {{9013,5}, {9018,1}};
            mailPackageManager.push(mail->id, mitem, 2, true);

            std::string strItems;
            for(int i = 0; i < 2; ++ i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        }
    }
    if((oldCnt < 40) && ((oldCnt + times) >=40))
    {
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem mitem[2] = {{9013,5}, {9020,1}};
            mailPackageManager.push(mail->id, mitem, 2, true);

            std::string strItems;
            for(int i = 0; i < 2; ++ i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        }
    }
    if((oldCnt < 60) && ((oldCnt + times) >=60))
    {
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem mitem[2] = {{9013,5}, {9018,2}};
            mailPackageManager.push(mail->id, mitem, 2, true);

            std::string strItems;
            for(int i = 0; i < 2; ++ i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        }
    }
    if((oldCnt < 80) && ((oldCnt + times) >=80))
    {
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem mitem[2] = {{9013,5}, {9020,2}};
            mailPackageManager.push(mail->id, mitem, 2, true);

            std::string strItems;
            for(int i = 0; i < 2; ++ i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        }
    }

    WORLD().RankLuckyDraw(player);
}

#define CNT 7
void SyncToLogin4IDIP()
{
    struct RechargeInfo
    {
        char name[64];
        UInt32 total;
    } info[7] = {{{0,},0},};

    UInt32 c = 0;
    for (RCSortType::iterator i = World::rechargeSort.begin(), e = World::rechargeSort.end(); i != e; ++i)
    {
        snprintf(info[c].name, 63, i->player->getName().c_str());
        info[c].total = i->total;

        ++c;
        if (c >= CNT)
            break;
    }

    LoginMsgHdr hdr1(0x331, WORKER_THREAD_LOGIN, NULL, -1, sizeof(info));
    GLOBAL().PushMsg(hdr1, &info);
}

void SendRechargeRank(Stream& st)
{
    using namespace GObject;
    st.init(REP::ACT);
    UInt32 cnt = World::rechargeSort.size();
    if (cnt > CNT)
        cnt = CNT;
    st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << static_cast<UInt8>(0) << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    for (RCSortType::iterator i = World::rechargeSort.begin(), e = World::rechargeSort.end(); i != e; ++i)
    {
        st << i->player->getName();
        st << i->total;
        st << static_cast<UInt8>(i->player->getCountry()<<4|(i->player->IsMale()?0:1));
        ++c;
        if (c >= CNT)
            break;
    }
    st << Stream::eos;

    SyncToLogin4IDIP();
}

void SetQiShiBanRank( GameMsgHdr& hdr,  const void* data )
{
    World::initRCRank();
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *(UInt32*)data;
    if (!total)
        return;

    for (RCSortType::iterator i = World::qishibanScoreSort.begin(), e = World::qishibanScoreSort.end(); i != e; ++i)
    {
        if (i->player == player)
        {
            World::qishibanScoreSort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    World::qishibanScoreSort.insert(s);
}

void SendQiShiBanRank( GameMsgHdr& hdr,  const void* data )
{
    World::initRCRank();
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 curPage = *(UInt32*)data;
    if (!curPage)
        return;

    UInt32 rank = 0;
    UInt32 myRank = 0;
    UInt32 myScore = 0;
    for (RCSortType::iterator i = World::qishibanScoreSort.begin(), e = World::qishibanScoreSort.end(); i != e; ++i)
    {
        rank++;
        if (i->player == player)
        {
            myScore = i->total;
            myRank = rank;
            break;
        }
    }

    Stream st(REP::ACT);
    UInt32 cnt = World::qishibanScoreSort.size();
    UInt32 totalPage = 0;

    if(0 == cnt)
        totalPage = 1;
    else if(0 == cnt % 12)
        totalPage = cnt / 12;
    else
        totalPage = cnt / 12 + 1;

    if(curPage < totalPage)
        cnt = 12;
    else if(curPage == totalPage)
        cnt = cnt - (curPage - 1) * 12;

    st << static_cast<UInt8>(0x23) << static_cast<UInt8>(1) << static_cast<UInt8>(0) << myRank << myScore << totalPage << curPage << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    UInt32 c1 = 0;
    for (RCSortType::iterator i = World::qishibanScoreSort.begin(), e = World::qishibanScoreSort.end(); i != e; ++i)
    {
        /*st << static_cast<UInt8>(c + 1);
        st << i->player->getName();
        st << i->total;
        ++c;
        if (c >= 10)
            break;*/

        if((c>=(curPage-1)*12) && (c<=(curPage*12)))
        {
            st << static_cast<UInt32>(c + 1);
            st << i->player->getName();
            st << i->total;
            ++c1;
        }
        ++c;
        if(c1 >= 12)
            break;
    }
    st << Stream::eos;
    player->send(st);
}

void OnAddQiShiBanCount( GameMsgHdr & hdr, const void * data)
{
    // 增加玩家七石板游戏次数
	MSG_QUERY_PLAYER(player);
	struct QiShiBanMailClickReq
	{
		UInt32 id;
		GObject::Player * applier;
		UInt8 action;
	};
	const QiShiBanMailClickReq * qsbmcr = reinterpret_cast<const QiShiBanMailClickReq *>(data);
    if (!qsbmcr->applier)
        return;

    if (!WORLD().getQiShiBanTime())
    {
        player->sendMsgCode(0, 1512);
        return;
    }

    if (qsbmcr->action)
    {
        if (qsbmcr->applier->GetVar(VAR_QISHI_FRIEND_SEND_COUNT) >= 2)
        {
            player->sendMsgCode(0, 1511);
            return;
        }
        else
        {
            qsbmcr->applier->AddVar(VAR_QISHI_FRIEND_SEND_COUNT, 1);
            SYSMSGV(title, 4979);
            SYSMSGV(content, 4980, player->getCountry(), player->getName().c_str());
            qsbmcr->applier->GetMailBox()->newMail(player, 0x01, title, content);
        }
    }
}

void OnSetGuankaActRank( GameMsgHdr& hdr, const void* data )
{
    World::initRCRank();
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *(UInt32*)data;
    if (!total)
        return;

    UInt32 fgtId = 0;
    if(player->getMainFighter())
        fgtId = (player->getCountry()<<4) | player->getMainFighter()->getSex();
    std::string info = player->getName() + "_" + Itoa(fgtId) + "_" + Itoa(total);
    WORLD().SetMemCach_guankaActInfo(player->getOpenId(), info);

    for (RCSortType::iterator i = World::guankaScoreSort.begin(), e = World::guankaScoreSort.end(); i != e; ++i)
    {
        if (i->player == player)
        {
            World::guankaScoreSort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    s.time = player->GetVar(VAR_GUANKA_ACTION_TIME);
    World::guankaScoreSort.insert(s);
}

void OnClearGuankaActRank( GameMsgHdr& hdr, const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    World::guankaScoreSort.clear();
    WORLD().DelMemCach_guankaAct(player->getOpenId());
}

void OnSendGuankaActRank10( GameMsgHdr& hdr,  const void* data )
{
    World::initRCRank();
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    Stream st(REP::ACT);
    st << static_cast<UInt8>(0x30) << static_cast<UInt8>(0x02);

    size_t offset = st.size();
    UInt8 idx = 0;
    st << idx;
    RCSortType::iterator it = World::guankaScoreSort.begin();
    for(; it != World::guankaScoreSort.end() && idx < 10; ++ it, ++ idx)
    {
        UInt8 csex = (it->player->getCountry() << 4) | it->player->getMainFighter()->getSex();
        st << it->player->getName() << csex << static_cast<UInt16>(it->total);
    }
    st.data<UInt8>(offset) = idx;
    st << Stream::eos;
    player->send(st);
}

void OnSendGuankaActMyRank( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt16 rank = 0;
    UInt16 myRank = 0;
    UInt16 myScore = 0;
    for (RCSortType::iterator i = World::guankaScoreSort.begin(), e = World::guankaScoreSort.end(); i != e; ++i)
    {
        rank++;
        if (i->player == player)
        {
            myScore = i->total;
            myRank = rank;
            break;
        }
    }
    UInt32 value = player->GetVar(VAR_GUANKA_ACTION_NPC);
    Stream st(REP::ACT);
    st << static_cast<UInt8>(0x30) << static_cast<UInt8>(0x00);
    st << myScore << myRank << value;
    st << Stream::eos;
    player->send(st);
}

void SendLuckyBagRank(Stream& st)
{
    using namespace GObject;
    st.init(REP::ACT);
    UInt32 cnt = World::LuckyBagSort.size();
    if (cnt > CNT)
        cnt = CNT;
    st << static_cast<UInt8>(2) << static_cast<UInt8>(4) << static_cast<UInt8>(0) << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    for (RCSortType::iterator i = World::LuckyBagSort.begin(), e = World::LuckyBagSort.end(); i != e; ++i)
    {
        st << i->player->getName();
        st << i->total;
        st << static_cast<UInt8>(i->player->getCountry()<<4|(i->player->IsMale()?0:1));
        ++c;
        if (c >= CNT)
            break;
    }
    st << Stream::eos;
}
void Send11PlayerGradeRank(Stream& st)
{
    World::initRCRank();
    using namespace GObject;
    st.init(REP::ACTIVE);    //lib待定
    UInt32 cnt = World::PlayerGradeSort.size();
    if (cnt > CNT)
        cnt = CNT;
    st << static_cast<UInt8>(0x20) << static_cast<UInt8>(1) << static_cast<UInt8>(0) << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    for (RCSortType::iterator i = World::PlayerGradeSort.begin(), e = World::PlayerGradeSort.end(); i != e; ++i)
    {
        st << i->player->getName();
        st << i->total;
     //   st << static_cast<UInt8>(i->player->getCountry()<<4|(i->player->IsMale()?0:1));
        ++c;
        if (c >= CNT)
            break;
    }
    st << Stream::eos;

}
void Send11ClanGradeRank(Stream& st)
{
    using namespace GObject;
    st.init(REP::ACT);    //lib待定
    UInt32 cnt = World::clanGradeSort.size();
    if (cnt > CNT)
        cnt = CNT;
    st << static_cast<UInt8>(0x20) << static_cast<UInt8>(3)<< static_cast<UInt8>(0) << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    for (ClanGradeSort::iterator i = World::clanGradeSort.begin(), e = World::clanGradeSort.end(); i != e; ++i)
    {
        if(i->clan == NULL)
            continue ;
        st << i->clan->getName();
        st << i->total;
     //   st <<i->clan->getCountry();
      //  st << static_cast<UInt8>(i->player->getCountry()<<4|(i->player->IsMale()?0:1));
        ++c;
        if (c >= CNT)
            break;
    }
    st << Stream::eos;
}
void OnSend11ClanGradeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Stream st;
    Send11ClanGradeRank(st);
    player->send(st);
    Clan * clan = player->getClan();
    if(clan == NULL)
        return ;

    UInt32 rank = 0;
    for (ClanGradeSort::iterator i = World::clanGradeSort.begin(), e = World::clanGradeSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->clan == clan)
        {
            Stream st(REP::ACT);
            st << static_cast<UInt8>(0x20) << static_cast<UInt8>(3) << static_cast<UInt8>(2);
            st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
            player->send(st);
            break;
        }
    }
}
void OnSend11PlayerGradeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Stream st;
    Send11PlayerGradeRank(st);
    player->send(st);

    UInt32 rank = 0;
    for (RCSortType::iterator i = World::PlayerGradeSort.begin(), e = World::PlayerGradeSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->player == player)
        {
            Stream st(REP::ACT);
            st << static_cast<UInt8>(0x20) << static_cast<UInt8>(1) << static_cast<UInt8>(2);
            st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
            player->send(st);
            break;
        }
    }
}

void OnRechargeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::rechargeSort.begin(), e = World::rechargeSort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= CNT)
                inrank = true;
            World::rechargeSort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    s.time = TimeUtil::Now();
    World::rechargeSort.insert(s);

    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (RCSortType::iterator i = World::rechargeSort.begin(), e = World::rechargeSort.end(); i != e; ++i)
    {
        if (!stop)
            ++myrank;

        if (i->player == player)
            stop = true;

        ++rank;

        Stream st(REP::ACT);
        st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << static_cast<UInt8>(2) << i->total << static_cast<UInt8>(rank) << Stream::eos;
        i->player->send(st);
    }

    if (oldrank <= CNT || (!inrank && myrank <= CNT))
    {
        Stream st;
        SendRechargeRank(st);
        NETWORK()->Broadcast(st);
    }
}

/*void OnQiShiBanRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::qishibanScoreSort.begin(), e = World::qishibanScoreSort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= 10)
                inrank = true;
            World::qishibanScoreSort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    World::qishibanScoreSort.insert(s);

    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (RCSortType::iterator i = World::qishibanScoreSort.begin(), e = World::qishibanScoreSort.end(); i != e; ++i)
    {
        if (!stop)
            ++myrank;

        if (i->player == player)
            stop = true;

        ++rank;

        Stream st(REP::ACT);
        st << static_cast<UInt8>(0x23) << static_cast<UInt8>(1) << static_cast<UInt8>(2) << i->total << static_cast<UInt8>(rank) << Stream::eos;
        i->player->send(st);
    }

    if (oldrank <= 10 || (!inrank && myrank <= 10))
    {
        Stream st;
        SendQiShiBanRank(st);
        NETWORK()->Broadcast(st);
    }
}*/

void OnLuckyBagRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::LuckyBagSort.begin(), e = World::LuckyBagSort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= CNT)
                inrank = true;
            World::LuckyBagSort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    World::LuckyBagSort.insert(s);

    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (RCSortType::iterator i = World::LuckyBagSort.begin(), e = World::LuckyBagSort.end(); i != e; ++i)
    {
       if (!stop)
            ++myrank;

        if (i->player == player)
            stop = true;

        ++rank;

        Stream st(REP::ACT);
        st << static_cast<UInt8>(2) << static_cast<UInt8>(4) << static_cast<UInt8>(2);
        st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
        i->player->send(st);
    }

    if (oldrank <= CNT || (!inrank && myrank <= CNT))
    {
        Stream st;
        SendLuckyBagRank(st);
        NETWORK()->Broadcast(st);
    }
}
void On11PlayerGradeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::PlayerGradeSort.begin(), e = World::PlayerGradeSort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= CNT)
                inrank = true;
            World::PlayerGradeSort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    World::PlayerGradeSort.insert(s);

    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (RCSortType::iterator i = World::PlayerGradeSort.begin(), e = World::PlayerGradeSort.end(); i != e; ++i)
    {
       if (!stop)
            ++myrank;

        if (i->player == player)
            stop = true;

        ++rank;

        Stream st(REP::ACT);  //lib待定
        st << static_cast<UInt8>(0x20) << static_cast<UInt8>(1)<< static_cast<UInt8>(2);
        st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
        i->player->send(st);
    }

    if (oldrank <= CNT || (!inrank && myrank <= CNT))
    {
        Stream st;
        Send11PlayerGradeRank(st);
        NETWORK()->Broadcast(st);
    }
}
void On11ClanGradeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    Clan * clan = player->getClan();
    if(clan == NULL)
    {
        return ;
    }
    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (ClanGradeSort::iterator i = World::clanGradeSort.begin(), e = World::clanGradeSort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->clan == clan)
        {
            if (oldrank <= CNT)
                inrank = true;
            World::clanGradeSort.erase(i);
            break;
        }
    }

    ClanSort s;
    s.clan = clan;
    s.total = total;
    World::clanGradeSort.insert(s);

    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (ClanGradeSort::iterator i = World::clanGradeSort.begin(), e = World::clanGradeSort.end(); i != e; ++i)
    {
       if (!stop)
            ++myrank;
        ++rank;
        if (i->clan == clan)
        {
            stop = true;
            Stream st(REP::ACT);  //lib待定  帮派排名
            st << static_cast<UInt8>(0x20) << static_cast<UInt8>(3) << static_cast<UInt8>(2);
            st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
            player->send(st);
            break;
        }
    }

    if (oldrank <= CNT || (!inrank && myrank <= CNT))
    {
        Stream st;
        Send11ClanGradeRank(st);
        NETWORK()->Broadcast(st);
    }
}
void Update11ClanRank(GameMsgHdr& hdr,const void* data)
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 clanId = *((UInt32*)data);

    for (ClanGradeSort::iterator i = World::clanGradeSort.begin(), e = World::clanGradeSort.end(); i != e; ++i)
    {
        if (i->clan->getId() == clanId)
        {
            World::clanGradeSort.erase(i);
            break;
        }
    }

    Clan * clan = globalClans[clanId];
    if(clan == NULL)
    {
        Stream st;
        Send11ClanGradeRank(st);
        NETWORK()->Broadcast(st);
        return ;
    }
    UInt32 total = clan->getGradeInAirBook();
    if (!total)
        return;

    ClanSort s;
    s.clan = clan;
    s.total = total;
    World::clanGradeSort.insert(s);
    Stream st;
    Send11ClanGradeRank(st);
    NETWORK()->Broadcast(st);
}
void On11CountryGradeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 Emei=0;
    UInt32 KunLun = 0;
    UInt8 em=2;
    UInt8 kl=2;
    Stream st(REP::ACT);  //lib待定  帮派排名
    st << static_cast<UInt8>(0x20) << static_cast<UInt8>(3) << static_cast<UInt8>(3);
    size_t pos = st.size();
    st << em ;
    for ( ClanGradeSort::iterator i = World::clanGradeSort.begin(), e = World::clanGradeSort.end(); i != e &&(em != 0 ); ++i)
    {
        Clan * clan = i->clan;
        if(clan == NULL)
            continue ;
        if(clan->getCountry() ==0  && em !=0 )
        {
            Emei += clan->getGradeInAirBook();
            st<<clan->getName()<< clan->getGradeInAirBook();
            em--;
        } 
    }
    st.data<UInt8>(pos) = (2- em );
    pos  =  st.size() ;
    st<<kl;
    for ( ClanGradeSort::iterator i = World::clanGradeSort.begin(), e = World::clanGradeSort.end(); i != e &&(kl != 0 ); ++i)
    {
        Clan * clan = i->clan;
        if(clan == NULL)
            continue ;
        if(clan->getCountry() ==1  && kl !=0)
        {
            KunLun += clan->getGradeInAirBook();
            st<<clan->getName()<< clan->getGradeInAirBook();
            kl--;
        } 
    }
    st.data<UInt8>(pos) = ( 2 - kl );
    st<<static_cast<UInt32>(Emei)<<static_cast<UInt32>(KunLun);
    st<<Stream::eos;
    player->send(st);
}
void SendConsumeRank(Stream& st)
{
    using namespace GObject;
    st.init(REP::ACT);
    UInt32 cnt = World::consumeSort.size();
    if (cnt > CNT)
        cnt = CNT;
    st << static_cast<UInt8>(2) << static_cast<UInt8>(2) << static_cast<UInt8>(0) << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    for (RCSortType::iterator i = World::consumeSort.begin(), e = World::consumeSort.end(); i != e; ++i)
    {
        st << i->player->getName();
        st << i->total;
        st << static_cast<UInt8>(i->player->getCountry()<<4|(i->player->IsMale()?0:1));
        ++c;
        if (c >= CNT)
            break;
    }
    st << Stream::eos;
}

void OnConsumeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::consumeSort.begin(), e = World::consumeSort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= CNT)
                inrank = true;
            World::consumeSort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    World::consumeSort.insert(s);

    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (RCSortType::iterator i = World::consumeSort.begin(), e = World::consumeSort.end(); i != e; ++i)
    {
        if (!stop)
            ++myrank;

        if (i->player == player)
            stop = true;

        ++rank;

        Stream st(REP::ACT);
        st << static_cast<UInt8>(2) << static_cast<UInt8>(2) << static_cast<UInt8>(2) << i->total << static_cast<UInt8>(rank) << Stream::eos;
        i->player->send(st);
    }

    if (oldrank <= CNT || (!inrank && myrank <= CNT))
    {
        Stream st;
        SendConsumeRank(st);
        NETWORK()->Broadcast(st);
    }
}

void OnSendRechargeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Stream st;
    SendRechargeRank(st);
    player->send(st);

    UInt32 rank = 0;
    for (RCSortType::iterator i = World::rechargeSort.begin(), e = World::rechargeSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->player == player)
        {
            Stream st(REP::ACT);
            st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << static_cast<UInt8>(2) << i->total << static_cast<UInt8>(rank) << Stream::eos;
            player->send(st);
            break;
        }
    }
}

/*void OnSendQiShiBanRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Stream st;
    //SendQiShiBanRank(st);
    player->send(st);

    UInt32 rank = 0;
    for (RCSortType::iterator i = World::qishibanScoreSort.begin(), e = World::qishibanScoreSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->player == player)
        {
            Stream st(REP::ACT);
            st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << static_cast<UInt8>(2) << i->total << static_cast<UInt8>(rank) << Stream::eos;
            player->send(st);
            break;
        }
    }
}*/

void OnSendLuckyBagRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Stream st;
    SendLuckyBagRank(st);
    player->send(st);

    UInt32 rank = 0;
    for (RCSortType::iterator i = World::LuckyBagSort.begin(), e = World::LuckyBagSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->player == player)
        {
            Stream st(REP::ACT);
            st << static_cast<UInt8>(2) << static_cast<UInt8>(4) << static_cast<UInt8>(2);
            st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
            player->send(st);
            break;
        }
    }
}

//For 后台操控设置活动时间及清理数据
void OnClearLuckyBagRank ( GameMsgHdr& hdr,  const void* data )
{
    World::LuckyBagSort.clear();
}
void OnClearPlayerGradeSortRank ( GameMsgHdr& hdr,  const void* data )
{
    World::PlayerGradeSort.clear();
}

void OnSendConsumeRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Stream st;
    SendConsumeRank(st);
    player->send(st);

    UInt32 rank = 0;
    for (RCSortType::iterator i = World::consumeSort.begin(), e = World::consumeSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->player == player)
        {
            Stream st(REP::ACT);
            st << static_cast<UInt8>(2) << static_cast<UInt8>(2) << static_cast<UInt8>(2) << i->total << static_cast<UInt8>(rank) << Stream::eos;
            player->send(st);
            break;
        }
    }
}

#define PT_CNT 8
void SendPopularityRank(Stream &st, Player* player)
{
    using namespace GObject;
    st.init(REP::SORT_LIST);
    size_t cnt = World::popularitySort.size();
    if (cnt > PT_CNT)
        cnt = PT_CNT;
	st << static_cast<UInt8>(7) << static_cast<UInt32>(0) << static_cast<UInt32>(player->GetVar(VAR_POPULARITY)) << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    for (RCSortType::iterator i = World::popularitySort.begin(), e = World::popularitySort.end(); i != e; ++i)
    {
        Player* pl = i->player;
        if (pl)
        {
            st << pl->getName() << pl->getPF() << pl->GetLev() << pl->getCountry() << i->total << pl->getClanName();
        }

        ++c;
        if (c >= PT_CNT)
            break;
    }
    st << Stream::eos;
    player->send(st);
}


inline bool popularityChanged(GObject::Player* p, void * data)
{
    if (p == NULL)
        return true;
    Stream st;
    SendPopularityRank(st, p);
    return true;
}

void OnPopularityRank ( GameMsgHdr& hdr, const void * data)
{
    // 更新人气排名
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::popularitySort.begin(), e = World::popularitySort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= PT_CNT)
                inrank = true;
            World::popularitySort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    World::popularitySort.insert(s);

    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (RCSortType::iterator i = World::popularitySort.begin(), e = World::popularitySort.end(); i != e; ++i)
    {
        if (!stop)
            ++myrank;

        if (i->player == player)
            stop = true;

        ++rank;

        //Stream st(REP::ACT);
        //st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << static_cast<UInt8>(2) << i->total << static_cast<UInt8>(rank) << Stream::eos;
    }

    if (oldrank <= PT_CNT || (!inrank && myrank <= PT_CNT))
    {
        Stream st;
        SendPopularityRank(st, player);
    }
    //globalPlayers.enumerate(popularityChanged, static_cast<void *>(NULL));
}

void OnSendPopularityRank(GameMsgHdr& hdr, const void*data)
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Stream st(REP::SORT_LIST);
    SendPopularityRank(st, player);
}

void OnGetQgameGiftAward( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    player->getQgameGiftAward();
}

void OnRoamResult( GameMsgHdr& hdr,  const void* data )
{
    MSG_QUERY_PLAYER(player);

    struct Roam
    {
        UInt8 pos;
        UInt8 event;
        UInt8 score;
    };

    Roam* roam = (Roam*)data;
    player->qixiStepAdvance(roam->pos, roam->event, roam->score);
}

void OnKillMonsterRoamResult( GameMsgHdr& hdr,  const void* data )
{
    MSG_QUERY_PLAYER(player);

    struct _Roam
    {
        UInt32 _pos;
        UInt8 _curType;
        UInt8 _curCount;
        UInt8 _tips;
    };

    const _Roam* roam = reinterpret_cast<const _Roam *>(data);
    player->killMonsterStepAdvance(roam->_pos, roam->_curType, roam->_curCount, roam->_tips);
}

void OnKillMonsterReqInfo( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    WORLD().killMonsterInit();

    Stream st(REP::COUNTRY_ACT);
    UInt8 subType = 0x02;
    st << subType;
    UInt8 subType2 = 0;
    st << subType2;
    st << player->GetVar(VAR_ZYCM_POS);
    st << static_cast<UInt8>(player->GetVar(VAR_ZYCM_TIPS));
    st << player->GetVar(VAR_XIAGU_CNT);
    st << player->GetVar(VAR_ROUQING_CNT);
    st << player->GetVar(VAR_CAIFU_CNT);
    st << player->GetVar(VAR_CHUANQI_CNT);
    WORLD().killMonsterAppend(st, 0);
    WORLD().killMonsterAppend(st, 1);
    WORLD().killMonsterAppend(st, 2);
    WORLD().killMonsterAppend(st, 3);
    st << Stream::eos;
    player->send(st);
}

void OnReCalcWeekDayAddTimer( GameMsgHdr& hdr,  const void* data )
{
    WORLD()._recalcwd = WORLD().AddTimer(10000, WORLD().ReCalcWeekDay, &(WORLD()), 10000);
}

void OnReCalcWeekDayRemoveTimer( GameMsgHdr& hdr,  const void* data )
{
    WORLD().RemoveTimer(WORLD()._recalcwd);
    WORLD()._recalcwd = NULL;
}

void OnServerWarBossTimer( GameMsgHdr& hdr,  const void* data )
{
    UInt8 type = *reinterpret_cast<UInt8*>(const_cast<void *>(data));
    if(type)    //AddTimer
        WORLD()._swBosstimer = WORLD().AddTimer(5000, WORLD().ServerWarBoss_Refresh, &(WORLD()), 10000);
    else        //RemoveTimer
    {
        WORLD().RemoveTimer(WORLD()._swBosstimer);
        WORLD()._swBosstimer = NULL;
    }
}

void OnServerWarResNotify( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    struct SWResNotify
    {
        GObject::Player * peer;
        bool win;
    };
    SWResNotify* notify = reinterpret_cast<SWResNotify*>(const_cast<void *>(data));
    if(!notify) return;

    GObject::serverWarMgr.notifyChallengeResult(player, notify->peer, notify->win);
}

void OnTownDeamonResNotify( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    struct TDResNotify
    {
        GObject::Player * peer;
        bool win;
    };
    TDResNotify* notify = reinterpret_cast<TDResNotify*>(const_cast<void *>(data));

    GObject::townDeamonManager->notifyChallengeResult(player, notify->peer, notify->win);
}

void OnTownDeamonAttackNpcNotify( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    bool win = *reinterpret_cast<bool*>(const_cast<void *>(data));

    GObject::townDeamonManager->notifyAttackNpcResult(player, win);
}

void OnSHFighterCloneRes( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    GObject::Fighter* fgt = *reinterpret_cast<GObject::Fighter**>(const_cast<void*>(data));

    GObject::shStageMgr.onFighterClone(player, fgt);
}

void OnSHReset( GameMsgHdr& hdr, const void* data )
{
    GObject::shStageMgr.reset();
}

inline bool enterSingleHeroStage(GObject::Player* p, UInt32 cnt)
{
    GObject::shStageMgr.enter(p, p->getMainFighter());
    if(cnt != 0 && GObject::shStageMgr.getPlayerCount() > cnt)
        return false;
    return true;
}

void OnSHEnter( GameMsgHdr& hdr, const void* data )
{
    UInt32 cnt = *reinterpret_cast<UInt32*>(const_cast<void*>(data));
    GObject::globalPlayers.enumerate(enterSingleHeroStage, cnt);
}


inline bool enterArena(GObject::Player* p, UInt32* cnt)
{
    if(!p)
        return true;
    if(*cnt > 100)
        return false;

    ++(*cnt);
    GObject::arena.enterArena(p);
    return true;
}

inline bool enterTeamArena(GObject::Player* p, UInt32* cnt)
{
    if(!p)
        return true;
    /*
    if(*cnt > 100)
        return false;

    ++(*cnt);
    */
    GObject::teamArenaMgr.enterArena(p);
    return true;
}

void OnEnterArena( GameMsgHdr& hdr, const void* data )
{
	UInt8 type = *reinterpret_cast<UInt8 *>(const_cast<void *>(data));
    UInt32 cnt = 0;
    if(type)
        GObject::globalPlayers.enumerate(enterTeamArena, &cnt);
    else
        GObject::globalPlayers.enumerate(enterArena, &cnt);
}

struct teamInfo
{
    teamInfo() : teamNum(0), cnt(0), tad(NULL) {}

    UInt16 teamNum;
    UInt16 cnt;
    GObject::TeamArenaData * tad;
};

inline bool createTeamArena(GObject::Player* pl, void* data)
{
    if(!pl || !data)
        return true;

	teamInfo* ti = reinterpret_cast<teamInfo*>(const_cast<void *>(data));
    if(ti->cnt >= ti->teamNum)
        return false;

    if(pl->getTeamData() && pl->getTeamArena()->leader == pl && !pl->getTeamArena()->isFull() && !ti->tad)
        ti->tad = pl->getTeamArena();
    if(!pl->getTeamData() && !ti->tad)
    {
        std::string name = Itoa(ti->cnt+uRand(100000));
        bool res = GObject::teamArenaMgr.createTeam(pl, name);
        if(res)
        {
            ++ ti->cnt;
            ti->tad = pl->getTeamArena();
        }
        return true;
    }
    else if(!pl->getTeamData() && ti->tad)
    {
        if(ti->tad && !ti->tad->isFull())
            GObject::teamArenaMgr.addTeamMember(ti->tad->leader, pl);
        if(ti->tad && ti->tad->isFull())
            ti->tad = NULL;
        return true;
    }
    return false;
}

void OnCreateTeamArena( GameMsgHdr& hdr, const void* data )
{
	UInt16 num = *reinterpret_cast<UInt16 *>(const_cast<void *>(data));
    if(num > globalPlayers.size() / 3)
        num = globalPlayers.size() / 3;

    teamInfo ti;
    ti.teamNum = num;
    GObject::globalPlayers.enumerate(createTeamArena, &ti);
}

void OnAddeamArenaScore( GameMsgHdr& hdr, const void* data )
{
	Player * player = *reinterpret_cast<Player **>(const_cast<void *>(data));
    if(!player) return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad) return;
    GObject::teamArenaMgr.addTeamScore(tad, 5, 1);
    GObject::teamArenaMgr.sendTeamInfo(tad);
}

void OnSHStageOnOff( GameMsgHdr& hdr, const void* data )
{
    struct OnOffData
    {
        UInt32 begin;
        UInt32 end;
        int	sessionID;
    } onOff = {0};

    onOff = *reinterpret_cast<OnOffData*>(const_cast<void*>(data));
    bool ret = GObject::shStageMgr.setOnOffTime(onOff.begin, onOff.end);

    Stream st(SPEP::SHSTAGEONOFF);
    st << static_cast<UInt8>(ret) << Stream::eos;
    NETWORK()->SendMsgToClient(onOff.sessionID, st);
}

void OnDaysRankMsg( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    daysValueRankMsg* msg = reinterpret_cast<daysValueRankMsg*>(const_cast<void*>(data));
    GObject::DaysRank::instance().updateDaysValue(msg); 
}

void SendQSBState( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);

    player->QiShiBanState();
}

void OnSetCFriendInvited( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);

    UInt64 userId = *reinterpret_cast<UInt64 *>(const_cast<void *>(data));
    WORLD().SetMemCach_CFriend_Invited(userId);
}

void OnDelCFriendInvited( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 opt = *reinterpret_cast<UInt8 *>(const_cast<void *>(data));

    UInt64 userId = player->getId();
    if(cfg.merged)
        userId &= 0x0000ffffffffffull;

    if(opt)
        WORLD().DelMemCach_CFriend_Invited(userId);
    else
        WORLD().DelMemCach_CFriend_InvitedAct(userId);
}

void OnSendCFriendInvited( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 opt = *reinterpret_cast<UInt8 *>(const_cast<void *>(data));

    UInt64 userId = player->getId();
    if(cfg.merged)
        userId &= 0x0000ffffffffffull;

    struct CFInvited
    {
        UInt8 type;
        UInt16 invited;
    } cfData = {0};

    Stream st(REP::CFRIEND);
    if(opt)
    {
        UInt16 invited = WORLD().GetMemCach_CFriend_Invited(userId);
        st << static_cast<UInt8>(6) << invited;

        cfData.type = opt;
        cfData.invited = invited;
    }
    else
    {
        UInt16 invited = WORLD().GetMemCach_CFriend_InvitedAct(userId);
        st << static_cast<UInt8>(5) << invited;

        cfData.type = opt;
        cfData.invited = invited;
    }
    st << Stream::eos;
    player->send(st);
    //只能在发送的时候触发
    GameMsgHdr hdr1(0x347, player->getThreadId(), player, sizeof(CFInvited));
    GLOBAL().PushMsg(hdr1, &cfData);
}

void OnSendClanMemberList( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);

    struct ClanMemberListReq
    {
        UInt8 _type;
        UInt8 _start;
        UInt8 _count;
    };

    ClanMemberListReq* cmlr = reinterpret_cast<ClanMemberListReq*>(const_cast<void*>(data));
    GObject::Clan *clan = player->getClan();
    if(clan != NULL)
        clan->sendClanList(player, cmlr->_type, cmlr->_start, cmlr->_count);
}
void OnSendClanMemberGrade( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);

    GObject::Clan *clan = player->getClan();
    if(clan != NULL)
        clan->SendClanMemberGrade(player);
}
#define CNT10 10

void OnRechargeRP7Rank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    if (TimeUtil::Now() > World::getOpenTime()+7*86400)
        return;
 
    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::rechargeRP7Sort.begin(), e = World::rechargeRP7Sort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= CNT10)
                inrank = true;
            World::rechargeRP7Sort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    World::rechargeRP7Sort.insert(s);

    GameMsgHdr hdr1(0x1CB, WORKER_THREAD_WORLD, player, 0);
    GLOBAL().PushMsg(hdr1, NULL);
}
void SendRechargeRP7Rank(GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    if (TimeUtil::Now() > World::getOpenTime()+30*86400)
        return;
 
    World::initRP7RCRank();

    MSG_QUERY_PLAYER(player);    

    UInt32 cnt = World::rechargeRP7Sort.size();
    if (cnt > CNT10)
        cnt = CNT10;
    UInt32 myRecharge = player->getTotalRecharge();
    if (TimeUtil::Now() > World::getOpenTime()+7*86400)
        myRecharge = player->GetVar(VAR_RP7_RECHARGE);
    UInt32 myrank = 0;
    if (myRecharge > 0)
    {
        for (RCSortType::iterator i = World::rechargeRP7Sort.begin(), e = World::rechargeRP7Sort.end(); i != e; ++i)
        {
            ++myrank;
            if (i->player == player)
                break;
        }
    }
    Stream st(REP::RP_SERVER);
    st << static_cast<UInt8>(1) << static_cast<UInt8>(1);
    st << myRecharge << myrank << static_cast<UInt8>(cnt);

    UInt32 c = 0;
    for (RCSortType::iterator i = World::rechargeRP7Sort.begin(), e = World::rechargeRP7Sort.end(); i != e; ++i)
    {
        st << i->player->getName();
        st << i->total;
        ++c;
        if (c >= CNT10)
            break;
    }
    st << Stream::eos;
    player->send(st);
}

void OnSendDuoBaoBegin(GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;

    MSG_QUERY_PLAYER(player);   

    UInt32 nowTime = TimeUtil::Now();
    UInt32 time = TimeUtil::SharpDayT(0,nowTime);
    UInt32 start = time + 10*60*60;     // 每天10点开始
    UInt32 end = time + 22*60*60;       // 每天22点结束(加5秒, 用于最后一次结算)
    if(nowTime >= start && nowTime <= end)
    {
        if(player->getClan() != NULL)
        {
            Stream st(REP::DUOBAO_REP);
            st << static_cast<UInt8>(0x07);
            st << static_cast<UInt8>(1);
            st << Stream::eos;
            player->send(st);
        }
    }
}

void OnSaleItemCancle( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::SaleCancelNotify * saleCancelNotify = reinterpret_cast<GObject::SaleCancelNotify*>(const_cast<void *>(data));

    for(int i = 0; i < saleCancelNotify->count; ++ i)
        GObject::gSaleMgr.cancelSale(player, saleCancelNotify->ids[i]);
}

void OnTeamArenaAddMember( GameMsgHdr& hdr, const void * data )
{   //战队长邀请 接受
	MSG_QUERY_PLAYER(player);

	Player * pl = *reinterpret_cast<Player **>(const_cast<void *>(data));
    GObject::teamArenaMgr.addTeamMember(pl, player);
}
#define CNT_3 3
void SendGuangGunRank(Stream& st)
{
    World::initRCRank();
    using namespace GObject;
    UInt32 cnt = World::guangGunSort.size();
    if (cnt > CNT_3)
        cnt = CNT_3;
    st << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    for (RCSortType::iterator i = World::guangGunSort.begin(), e = World::guangGunSort.end(); i != e; ++i)
    {
        if(i->player == NULL)
            continue;
        st << i->player->getName();
        if(i->player->getGGPlayer1())
            st <<i->player->getGGPlayer1()->getName();
        else
            st<<"";
        if(i->player->getGGPlayer2())
            st <<i->player->getGGPlayer2()->getName();
        else 
            st<<"";
        st << i->total;
        ++c;
        if (c >= CNT_3)
            break;
    }

}
void OnSendGuangGunRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Player* pl = player->getGGTimeCaptain();
    UInt32 rank = 0;
    for (RCSortType::iterator i = World::guangGunSort.begin(), e = World::guangGunSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->player == pl)
        {
            break;
        }
    }
    if(!pl->getGGTimeScore())
       rank = 0; 
    Stream st(REP::ACT);//GG
    st << static_cast<UInt8>(0x22) << static_cast<UInt8>(1) << static_cast<UInt8>(2);
    st << static_cast<UInt8>(rank > 255 ? 255 : rank);
    SendGuangGunRank(st);
    st <<Stream::eos;
    pl->send(st);
    if(pl->getGGPlayer1())
        pl->getGGPlayer1()->send(st);
    if(pl->getGGPlayer2())
        pl->getGGPlayer2()->send(st);
}

void OnSendOffQQClan ( GameMsgHdr& hdr,  const void* data )
{
    struct OnOffQQData 
	{
		UInt64 pid;
		UInt32 clanid;
	};
	OnOffQQData * onOffQQ = reinterpret_cast<OnOffQQData*>(const_cast<void *>(data));
    
    GObject::Clan *clan = GObject::globalClans[onOffQQ->clanid];
	GObject::Player * player = GObject::globalPlayers[onOffQQ->pid];
   
    if (!clan || !player)
        return;
    clan->offQQOpenid(player);

}

void OnGuangGunRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);

    UInt32 total = *((UInt32*)data);

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::guangGunSort.begin(), e = World::guangGunSort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= CNT_3)
                inrank = true;
            World::guangGunSort.erase(i);
            break;
        }
    }
    if (!total)
        return;
    RCSort s;
    s.player = player;
    s.total = total;
    World::guangGunSort.insert(s);
    GObject::Player *cap = player->getGGTimeCaptain();
    cap ->sendGuangGunInfo();
    if(cap->getGGPlayer1())
        cap->getGGPlayer1()->sendGuangGunInfo();
    if(cap->getGGPlayer2())
        cap->getGGPlayer2()->sendGuangGunInfo();
        
/*    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (RCSortType::iterator i = World::PlayerGradeSort.begin(), e = World::PlayerGradeSort.end(); i != e; ++i)
    {
       if (!stop)
            ++myrank;

        if (i->player == pl)
            stop = true;

        ++rank;
        {
            Stream st(REP::ACT);//GG
            st << static_cast<UInt8>(0x22) << static_cast<UInt8>(1) << static_cast<UInt8>(2);
            st << static_cast<UInt8>(rank > 255 ? 255 : rank);
            st << i->player->getName();
            if(i->player->getGGPlayer1())
                st <<i->player->getGGPlayer1()->getName();
            else
                st<<"";
            if(i->player->getGGPlayer2())
                st <<i->player->getGGPlayer2()->getName();
            else 
                st<<"";
            st << i->total;
            st <<Stream::eos;
            i->player->send(st);
            if(i->player->getGGPlayer1())
                i->player->getGGPlayer1()->send(st);
            if(i->player->getGGPlayer2())
                i->player->getGGPlayer2()->send(st);
        }
    }
*/
}
void SendHappyFireRank(Stream& st)
{
    World::initRCRank();
    using namespace GObject;
    st.init(REP::ACTIVE);    //lib待定
    UInt32 cnt = World::happyFireSort.size();
    if (cnt > CNT)
        cnt = CNT;
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(5) << static_cast<UInt8>(0) << static_cast<UInt8>(cnt);
    UInt32 c = 0;
    for (RCSortType::iterator i = World::happyFireSort.begin(), e = World::happyFireSort.end(); i != e; ++i)
    {
        if(i->player == NULL)
            continue;
        st << i->player->getName();
        st << i->total;
        st << static_cast<UInt8>(i->player->getCountry()<<4|(i->player->IsMale()?0:1));
        ++c;
        if (c >= CNT)
            break;
    }
    st << Stream::eos;
}
void OnHappyFireRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    if(!World::getHappyFireTime())
        return;
 
    UInt32 total = *((UInt32*)data);
    if (!total)
        return;

    bool inrank = false;
    UInt32 oldrank = 0;
    for (RCSortType::iterator i = World::happyFireSort.begin(), e = World::happyFireSort.end(); i != e; ++i)
    {
        ++oldrank;
        if (i->player == player)
        {
            if (oldrank <= CNT10)
                inrank = true;
            World::happyFireSort.erase(i);
            break;
        }
    }

    RCSort s;
    s.player = player;
    s.total = total;
    World::happyFireSort.insert(s);

    UInt32 rank = 0;
    UInt32 myrank = 0;
    bool stop = false;
    for (RCSortType::iterator i = World::happyFireSort.begin(), e = World::happyFireSort.end(); i != e; ++i)
    {
       if (!stop)
            ++myrank;

        if (i->player == player)
            stop = true;

        ++rank;

        Stream st(REP::ACT);  //lib待定
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(5)<< static_cast<UInt8>(2);
        st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
        i->player->send(st);
    }

    if (oldrank <= CNT || (!inrank && myrank <= CNT))
    {
        Stream st;
        SendHappyFireRank(st);
        NETWORK()->Broadcast(st);
    }
}
void OnSendHappyFireRank ( GameMsgHdr& hdr,  const void* data )
{
    using namespace GObject;
    MSG_QUERY_PLAYER(player);
    World::initRCRank();
    Stream st;
    SendHappyFireRank(st);
    player->send(st);

    UInt32 rank = 0;
    for (RCSortType::iterator i = World::happyFireSort.begin(), e = World::happyFireSort.end(); i != e; ++i)
    {
        ++rank;
        if (i->player == player)
        {
            Stream st(REP::ACT);
            st << static_cast<UInt8>(0x02) << static_cast<UInt8>(5) << static_cast<UInt8>(2);
            st << i->total << static_cast<UInt8>(rank > 255 ? 255 : rank) << Stream::eos;
            player->send(st);
            break;
        }
    }
}

#endif // _WORLDINNERMSGHANDLER_H_

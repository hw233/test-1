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
	Stream st(REP::SERVER_ARENA_OP);
	st << static_cast<UInt8>(0) << static_cast<UInt8>(GObject::arena.active() ? 1 : 0) << Stream::eos;
	player->send(st);
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
	GameMsgHdr hdr2(0x279, player->getThreadId(), player, 0);
	GLOBAL().PushMsg(hdr2, NULL);
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
	GObject::EventBase * ev = GObject::eventWrapper.RemoveTimerEvent(player, EVENT_DUNGEONAUTO, 0);
	if(ev == NULL)
		return;
	GameMsgHdr hdr2(0x281, player->getThreadId(), player, sizeof(GObject::EventBase *));
	GLOBAL().PushMsg(hdr2, &ev);
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
    player->practiceUdpLog();
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
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
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
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, player->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        }
    }

    WORLD().RankLuckyDraw(player);
}

#define CNT 7
void SendRechargeRank(Stream& st)
{
    using namespace GObject;
    st.init(REP::ACT);
    UInt8 cnt = World::rechargeSort.size();
    if (cnt > CNT)
        cnt = CNT;
    st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << static_cast<UInt8>(0) << cnt;
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

void SendConsumeRank(Stream& st)
{
    using namespace GObject;
    st.init(REP::ACT);
    UInt8 cnt = World::consumeSort.size();
    if (cnt > CNT)
        cnt = CNT;
    st << static_cast<UInt8>(2) << static_cast<UInt8>(2) << static_cast<UInt8>(0) << cnt;
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




#endif // _WORLDINNERMSGHANDLER_H_

﻿#ifndef _COUNTRYINNERMSGHANDLER_H_
#define _COUNTRYINNERMSGHANDLER_H_

#include "Server/GlobalObject.h"
#include "Server/Cfg.h"
#include "MsgTypes.h"
#include "MsgFunc.h"
#include "Server/WorkerThread.h"
#include "Server/WorldServer.h"
#include "GObject/Country.h"
#include "CountryMsgStruct.h"
#include "GObject/Player.h"
#include "GObject/TaskMgr.h"
#include "GObject/EventBase.h"
#include "GObject/Sale.h"
#include "GObject/Mail.h"
#include "GObject/PracticePlace.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "GObject/Athletics.h"
#include "GObject/AthleticsRank.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"
#include "Script/BattleFormula.h"
#include "GObject/Clan.h"
#include "GObject/ClanCopy.h"
#include "GObject/JobHunter.h"
#ifndef _WIN32
#include "GObject/DCLogger.h"
#endif
#include "GObject/ShuoShuo.h"
#include "Common/StringTokenizer.h"

//Login thread -> Country thread
void PlayerEnter( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	const CountryEnterStruct * ces = reinterpret_cast<const CountryEnterStruct *>(data);
	if(!ces->notify)
	{
		if(!player->Init())
		{
			delete player;
			return;
		}
		//player->GetPackage()->AddItemFromDB(8942, 1, true);
		//DB3().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(8942, 1, %"I64_FMT"u, 1)", player->getId());
	}

	UInt16 newLoc = ces->newLoc;
	if(ces->inCity && (newLoc & 0xFF) == 0)
		newLoc += 1;

	PLAYER_DATA(player, location) = newLoc;
	PLAYER_DATA(player, inCity) = ces->inCity;

	GObject::Country& cny = CURRENT_COUNTRY();
	cny.PlayerEnter(player, ces->notify);

	if(ces->notify)
	{
		DB1().PushUpdateData("UPDATE `player` SET `inCity` = %u, `location` = %u WHERE id = %" I64_FMT "u", PLAYER_DATA(player, inCity), PLAYER_DATA(player, location), player->getId());
		GObject::Map * map = GObject::Map::FromSpot(newLoc);
		if(map == NULL)
			return;
		if(ces->inCity)
		{
			map->SendCityNPCs(player);
		}
		map->SendAtCity(player, ces->inCity == 1);
	}

    player->setJumpingMap(false);
}

void PlayerLeave( GameMsgHdr& hdr, const void *)
{
	MSG_QUERY_PLAYER(player);
	CURRENT_COUNTRY().PlayerLeave(player);
}

void PlayerLogin( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	const UInt8 flag = *reinterpret_cast<const UInt8 *>(data);
	if(flag == 1)
	{
		player->selfKick();
	}

	player->Login();

	Stream st(REP::LOGIN);
	st << static_cast<UInt32>(0) << player->getName() << Stream::eos;
	TcpConnection conn = NETWORK()->GetConn(player->GetSessionID());
	if(conn.get() == NULL)
	{
		return;
	}
	conn->send(&st[0], st.size());

	struct in_addr ip;
	ip.s_addr=htonl(player->getClientAddress());
	DBLOG1().PushUpdateData("insert into login_states (server_id,player_id,login_time,login_ip) values(%u, %"I64_FMT"u, %u, '%s')", cfg.serverLogId, player->getId(), TimeUtil::Now(), inet_ntoa(ip));
    player->sendTripodInfo();
}

void OnBroadcast( GameMsgHdr& hdr, const void * data )
{
#define MSG_MAX 4096
    struct BroadcastMsg
    {
        Map* map;
        Player* pl;
        int size;
        char msg[MSG_MAX];
    };

    BroadcastMsg* msg = (BroadcastMsg*)(data);

    if (msg->map && msg->msg)
        msg->map->Broadcast2(msg->msg, msg->size, CURRENT_COUNTRY().GetThreadID(), msg->pl);
}

void PlayerReconnect( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	player->Reconnect();
	Stream st(REP::RECONNECT);
	st << static_cast<UInt8>(0) << Stream::eos;
	player->send(&st[0], st.size());
}

void PlayerLogout( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	player->Logout();
	player->SetSessionID(-1);
}

void PlayerExpAdd( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt64 exp = *reinterpret_cast<const UInt64 *>(data);
	player->AddExp(exp);
}

void OnCancelAutoBattleNotifyReq(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	player->cancelAutoBattleNotify();
}

void OnCancelDungeonAutoNotifyReq(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	GObject::EventBase * ev = *reinterpret_cast<GObject::EventBase * const *>(data);
	if(ev == NULL)
		return;
	GObject::EventDungeonAuto * event = static_cast<GObject::EventDungeonAuto *>(ev);
	event->NotifyCancel();
	ev->release();
}

void OnCompleteDungeonAutoNotifyReq(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	GObject::EventBase * ev = *reinterpret_cast<GObject::EventBase * const *>(data);
	if(ev == NULL)
		return;
	GObject::EventDungeonAuto * event = static_cast<GObject::EventDungeonAuto *>(ev);
	event->Complete();
	ev->release();
}

void OnResetRecharge(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
    if (player->GetVar(VAR_RECHARGE_TOTAL))
        player->SetVar(VAR_RECHARGE_TOTAL, 0);
    if (player->GetVar(VAR_RECHARGE_SCORE))
        player->SetVar(VAR_RECHARGE_SCORE, 0);
    player->sendRechargeInfo();
}

void OnResetAccRechargeCnt(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
    if (player->GetVar(VAR_RECHARGE_CONDCNT))
        player->SetVar(VAR_RECHARGE_CONDCNT, 0);
}

void OnResetConsume(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
    if (player->GetVar(VAR_CONSUME))
        player->SetVar(VAR_CONSUME, 0);
    player->sendConsumeInfo();
}

void OnTimerEventCompletedNotify( GameMsgHdr& hdr, const void * data )
{
	struct EventMsg
	{
		GObject::EventBase * ev;
		UInt32 leftCount;
	};
	const EventMsg* evm = reinterpret_cast<const EventMsg *>(data);
	if(evm == NULL || evm->ev == NULL)
		return;
	evm->ev->Process(evm->leftCount);
	evm->ev->release();
}

void OnSearchAutoBattleResult( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::EventBase* evb = *reinterpret_cast<GObject::EventBase * const *>(data);
	if(evb == NULL)
		return;
	player->sendAutoBattleEvent(evb);
	evb->release();
}

void OnSearchAutoDungeonResult( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::Dungeon * dg = *reinterpret_cast<GObject::Dungeon * const *>(data);
	dg->sendAutoChallengeStart(player);
}

void OnReLoadLuaReq( GameMsgHdr&, const void * data )
{
	GObject::Country& cny = CURRENT_COUNTRY();
	UInt16 flag = *static_cast<const UInt16 *>(data);
	if(flag & 0x100)
		cny.GetGameActionLua()->reload();
	if(flag & 0x1)
		cny.GetBattleFormula()->reload();
}

void OnAthleticsReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::Player * defer = *reinterpret_cast<GObject::Player **>(const_cast<void *>(data));
	player->GetAthletics()->attack(defer);
	GameAction()->RunOperationTaskAction0(player, 1);
	GameAction()->RunOperationTaskAction0(player, 2);
    GameAction()->doStrong(player, SthAthletics1, 0,0);
}

void OnAthleticsMartialReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    player->GetAthletics()->listAthleticsMartial();
}

void OnAthleticsBeReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct AthleticsBeData
	{
		Player * attacker;
		UInt16 formation;
		UInt16 portrait;
		Lineup lineup[5];
	};
	AthleticsBeData * abd = reinterpret_cast<AthleticsBeData *>(const_cast<void *>(data));
	player->GetAthletics()->beAttack(abd->attacker, abd->formation, abd->portrait, abd->lineup);
}

void OnNewRelationAttack(GameMsgHdr& hdr, const void *data)
{
	MSG_QUERY_PLAYER(player);
	struct NewRelationBeData
	{
		Player * attacker;
		UInt16 formation;
		UInt16 portrait;
		Lineup lineup[5];
	};
	NewRelationBeData *abd = reinterpret_cast<NewRelationBeData *>(const_cast<void *>(data));
    player->GetNewRelation()->beAttack(abd->attacker, abd->formation, abd->portrait, abd->lineup, player);
}

void OnNewRelationCountryReq(GameMsgHdr& hdr, const void *data)
{
    MSG_QUERY_PLAYER(player);
    struct stNewRelationReq
    {
        UInt8 type;
        std::string atkName;
    };
    stNewRelationReq *nrw = reinterpret_cast<stNewRelationReq *>(const_cast<void *>(data));
    player->GetNewRelation()->countrySend(player, nrw->type, nrw->atkName);
}

void OnAthleticsEnterResp( GameMsgHdr& hdr, const void * )
{
	MSG_QUERY_PLAYER(player);
	player->GetAthletics()->setEnterAthletics();
}

void OnAthleticsSetPInfo( GameMsgHdr& hdr, const void *data )
{
	MSG_QUERY_PLAYER(player);
    AthleticsPInfo *newPInfo = reinterpret_cast<AthleticsPInfo *>(const_cast<void *>(data));
	AthleticsPInfo *orgPInfo = player->GetAthletics()->getPlayerPInfo();
    memcpy(orgPInfo, newPInfo, sizeof(AthleticsPInfo));
}

void OnAthleticsSetChallengeTime( GameMsgHdr& hdr, const void *data )
{
	MSG_QUERY_PLAYER(player);
    UInt32 challengeTime = *reinterpret_cast<UInt32 *>(const_cast<void *>(data));
	AthleticsPInfo *orgPInfo = player->GetAthletics()->getPlayerPInfo();
    orgPInfo->eChallengeTime = challengeTime;
}

void OnAthleticsSetIndex( GameMsgHdr& hdr, const void *data )
{
	MSG_QUERY_PLAYER(player);
    struct AthleticsUpdate {
        UInt8 eSelectIndex;
        UInt8 eCombineIndex;
        UInt32 eCombine;
    };
    AthleticsUpdate *curUpdate = reinterpret_cast<AthleticsUpdate *>(const_cast<void *>(data));
	AthleticsPInfo *orgPInfo = player->GetAthletics()->getPlayerPInfo();
    orgPInfo->eSelectIndex = curUpdate->eSelectIndex;
    if(curUpdate->eCombineIndex < 5)
        orgPInfo->eCombine[curUpdate->eCombineIndex] = curUpdate->eCombine;
}

void OnAthleticsDeferNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct AthleticsDeferNotify
	{
		UInt32 id;
		UInt8 win;
		UInt8 side;
		Player * target;
		UInt32 rptid;
		UInt32 time;
	};
	AthleticsDeferNotify * adn = reinterpret_cast<AthleticsDeferNotify *>(const_cast<void *>(data));
	player->GetAthletics()->addAthleticsDataFromTarget(adn->side, adn->target, adn->id, adn->win, adn->rptid, adn->time);
}

void OnAthleticsDropNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt32 rptid = *reinterpret_cast<UInt32 *>(const_cast<void *>(data));
	player->GetAthletics()->notifyDropAthleticsData(rptid);
}

void OnAthleticsAwardReq(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);

	struct GObject::AthleticsAward *awd = reinterpret_cast<struct GObject::AthleticsAward *>(const_cast<void *>(data));
    if(awd->itemId && awd->itemCount)
    {
        if(awd->itemId == COUPON_ID)
            player->getCoupon(awd->itemCount);
        else
            player->GetPackage()->AddItem(awd->itemId, awd->itemCount, 1, false, FromAthletAward);
    }
    if(awd->prestige)
    {
        player->getPrestige(awd->prestige);
    }
    if(awd->tael)
    {
        player->getTael(awd->tael);
    }

	if(awd->side == 0)
		player->GetAthletics()->defendergainsource(awd->other, awd->athleticsid, awd->type, awd->count);
	else
	{
		if(awd->side > 1)
			player->notifyFriendAct(8, awd->side - 1);
		player->GetAthletics()->attackergainsource(awd->athleticsid, awd->type, awd->count);
		if(World::_activityStage > 0 && awd->win)
			GameAction()->onAthleticWin(player);
	}
}

void OnAthleticsAwardReq2(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
    struct AthleticsAward2{
        UInt8 diffculty;
        UInt8 opt;
    };
	struct AthleticsAward2 *awd = reinterpret_cast<struct AthleticsAward2 *>(const_cast<void *>(data));
    UInt32 awardId = GameAction()->onGetAthlRandomAward(awd->diffculty, awd->opt);
    UInt16 itemId = awardId >> 16;
    UInt16 count = awardId & 0xFFFF;
    if(itemId && count)
    {
        if(itemId == COUPON_ID)
            player->getCoupon(count);
        else
            player->GetPackage()->AddItem(itemId, count, 1, false, FromAthletAward);
    }
}

void OnAthleticsSubDir(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
    UInt8 athlDiffculty;
    UInt8 athlCategory;
    UInt16 tmp = *reinterpret_cast<const UInt16 *>(data);
    athlDiffculty = tmp >> 8;
    athlCategory = tmp & 0xFF;
    player->GetAthletics()->RequestSubDir(player, athlDiffculty, athlCategory);
}

void OnGetBoxAddSource(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	const UInt8 * buffer = reinterpret_cast<const UInt8*>(data);
	UInt8 type = *reinterpret_cast<const UInt8 *>(buffer);
	UInt32 count = *reinterpret_cast<const UInt32 *>(buffer + 1);
	player->GetAthletics()->GetBoxAddSourceNotify(type, count);
}

void OnOnAthleticsEnterNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt8 row = *reinterpret_cast<UInt8 *>(const_cast<void *>(data));
	player->GetAthletics()->enterAthleticsNotify(row);
}

void OnAthleticsResNotify( GameMsgHdr& hdr, const void * data )
{
    MSG_QUERY_PLAYER(player);
    struct AthleticsResNotify
    {
        GObject::Player * peer;
        bool win;
    };
    AthleticsResNotify * notify = reinterpret_cast<AthleticsResNotify *>(const_cast<void *>(data));
    player->GetAthletics()->notifyAttackRes(notify->peer, notify->win);
}

void OnAthleticsReportList( GameMsgHdr& hdr, const void * data )
{
    MSG_QUERY_PLAYER(player);
    player->GetAthletics()->listAthleticsReport();
}

void OnGetClanBattleRewardReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt8 r;
	struct ClanBattleRewardStruct
	{
		UInt8 expNum;
		UInt8 rNum;
	};
	const ClanBattleRewardStruct * cbrs = reinterpret_cast<const ClanBattleRewardStruct *>(data);
	UInt16 spareCount;
	if(cbrs->rNum > 0)
		spareCount = 2;
	else
		spareCount = 1;
	if(player->GetPackage()->GetRestPackageSize() < spareCount)
	{
		r = 3;
	}
	else
	{
		r = 0;
		if(cbrs->expNum > 0)
		{
			UInt16 cnt = cbrs->expNum & 7;
			switch(cbrs->expNum >> 3)
			{
			case 0:
				player->GetPackage()->AddItem(8937, cnt, true, false, FromClanBattleAward);
				break;
			case 1:
				player->GetPackage()->AddItem(8934, cnt, true, false, FromClanBattleAward);
				break;
			case 2:
				player->GetPackage()->AddItem(8912, cnt, true, false, FromClanBattleAward);
				break;
			}
		}
		if(cbrs->rNum > 0)
			player->GetPackage()->AddItem(8994, cbrs->rNum, true, false, FromClanBattleAward);
	}
	GameMsgHdr hdr2(0x170, WORKER_THREAD_WORLD, player, sizeof(UInt8));
	GLOBAL().PushMsg(hdr2, &r);
}

void OnClanTakeRewardReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct ClanTakeRewardStruct
	{
		UInt32 allocTime;
		AllocItem item;
	};
	struct ClanTakeRewardResultStruct
	{
		bool result;
		UInt32 allocTime;
		GObject::AllocItem item;
	};
	const ClanTakeRewardStruct * ctrs = reinterpret_cast<const ClanTakeRewardStruct *>(data);
	ClanTakeRewardResultStruct ctrss;
	ctrss.result = player->GetPackage()->Add(ctrs->item.itemId, ctrs->item.itemNum, true, false, FromClanBattleAward) != NULL;
	ctrss.allocTime = ctrs->allocTime;
	ctrss.item = ctrs->item;
	GameMsgHdr hdr2(0x171, WORKER_THREAD_WORLD, player, sizeof(ClanTakeRewardResultStruct));
	GLOBAL().PushMsg(hdr2, &ctrss);
}


void OnLaunchTradeNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct LaunchTradeNotify
	{
		UInt32 id;
		Player* launcher;
		char title[49]; //'\0'
		UInt32 time;

		LaunchTradeNotify(UInt32 id)
		{
			memset(this, 0x00, sizeof(LaunchTradeNotify));
		}
	};
	const LaunchTradeNotify* notify = reinterpret_cast<const LaunchTradeNotify *>(data);
	player->GetTrade()->recvLaunchTrade(notify->id, notify->title, notify->time, notify->launcher);
}

void OnOperateTradeNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct OperateTradeNotify
	{
		UInt32 id;
		UInt8  status;

		OperateTradeNotify() : id(0), status(0) {}
	};
	const OperateTradeNotify * notify = reinterpret_cast<const OperateTradeNotify *>(data);
	GObject::Trade * trade = player->GetTrade();
	switch (notify->status)
	{
	case 4:
		trade->recvReplyTrade(notify->id);
		break;
	case 8:
	case 9:
		trade->recvCancelTrade(notify->id);
		break;
	}
}

void OnConfirmTradeNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct ConfirmTradeNotify
	{
		UInt32 id;
		Player* launcher;
		UInt32 coin;
		UInt32 gold;
		ItemBase* items[12];
	};
	UInt32 now = TimeUtil::Now();
	ConfirmTradeNotify * notify = reinterpret_cast<ConfirmTradeNotify *>(const_cast<void *>(data));
	if(notify->launcher->getId()!=player->getId())
	{
	    std::string itemsStr1="";
	    std::string itemsStr2="";
        for(int i=0;i<12;i++)
        {
            if(notify->items[i]!=NULL)
            {
                if(notify->items[i]->getId()>30000)
                {
                    itemsStr1+=Itoa(notify->items[i]->getId());
                    itemsStr1+=",";
                    itemsStr1+=Itoa(notify->items[i]->GetItemType().getId());
                    itemsStr1+="|";
					if(notify->items[i]->getQuality() >= 4)
					{
						DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, notify->launcher->getId(), notify->items[i]->GetItemType().getId(), notify->items[i]->getId(), FromTrade, now);
						DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, player->getId(), notify->items[i]->GetItemType().getId(), notify->items[i]->getId(), ToTrade, now);
					}
             }
                else
                {
                    itemsStr1+=Itoa(notify->items[i]->getId());
                    itemsStr1+=",";
                    itemsStr1+=Itoa(notify->items[i]->Count());
                    itemsStr1+="|";
					if(notify->items[i]->getQuality() >= 3)
					{
                        std::string tbn("item_courses");
                        DBLOG().GetMultiDBName(tbn);
						DBLOG().PushUpdateData("insert into `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)",tbn.c_str(), cfg.serverLogId, notify->launcher->getId(), notify->items[i]->getId(), notify->items[i]->Count(), FromTrade, now);
					}
              }
            }
        }
        itemsStr1=itemsStr1.substr(0,itemsStr1.length()-1);
        TradeData* trade=player->GetTrade()->findTradeDataById(notify->id);
        for(std::vector<ItemBase*>::iterator it=trade->_items.begin();it!=trade->_items.end();it++)
        {
            if((*it)->getId()>30000)
            {
                itemsStr2+=Itoa((*it)->getId());
                itemsStr2+=",";
                itemsStr2+=Itoa((*it)->GetItemType().getId());
                itemsStr2+="|";
				if((*it)->getQuality() >= 4)
				{
					DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, player->getId(), (*it)->GetItemType().getId(), (*it)->getId(), FromTrade, now);
					DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, notify->launcher->getId(), (*it)->GetItemType().getId(), (*it)->getId(), ToTrade, now);
				}
		    }
            else
            {
                itemsStr2+=Itoa((*it)->getId());
                itemsStr2+=",";
                itemsStr2+=Itoa((*it)->Count());
                itemsStr2+="|";
				if((*it)->getQuality() >= 3)
				{
                    std::string tbn("item_courses");
                    DBLOG().GetMultiDBName(tbn);
					DBLOG().PushUpdateData("insert into `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)",tbn.c_str(), cfg.serverLogId, player->getId(), (*it)->getId(), (*it)->Count(), FromTrade, now);
				}
            }
        }
        if (trade==NULL)
            return;

		DBLOG().PushUpdateData("insert into trade (server_id,initiator_id,initiator_coin,initiator_gold,initiator_items,receiver_id,receiver_coin,receiver_gold,receiver_items,trade_time) values(%u,%"I64_FMT"u,%u,%u,'%s',%"I64_FMT"u,%u,%u,'%s',%u)",
			cfg.serverLogId, notify->launcher->getId(), notify->coin, notify->gold, itemsStr1.c_str(), player->getId(), trade->_coin, trade->_gold, itemsStr2.c_str(), TimeUtil::Now());
    }
	player->GetTrade()->recvConfirmTrade(notify->id, notify->coin, notify->gold, notify->items, 12);
}

void OnTradeDataNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct TradeDataNotify
	{
		UInt32 id;
		UInt8  status;
		Player * receiver;
	};
	const TradeDataNotify * notify = reinterpret_cast<const TradeDataNotify *>(data);
	player->GetTrade()->sendTradeData(notify->id, notify->receiver, notify->status);
}

void OnDelTradeNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	const UInt32 id = *reinterpret_cast<const UInt32*>(data);
	player->GetTrade()->cancel2Trade(id);
}

void OnSalePriceCheckNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::SalePriceCheck * check = reinterpret_cast<GObject::SalePriceCheck *>(const_cast<void *>(data));
	player->GetSale()->checkSaleBuyPrice(*check);
}

void OnSaleAddItemNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::SaleItemBuy * saleItemBuy = reinterpret_cast<GObject::SaleItemBuy *>(const_cast<void *>(data));
	player->GetSale()->buySellResp(*saleItemBuy);
}

void OnSaleSellReqNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::SaleSellRespData * sale = reinterpret_cast<GObject::SaleSellRespData *>(const_cast<void *>(data));
	player->GetSale()->sellSaleReqNotify(sale, 9);
}

void OnSaleSellRespNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct BuyInfo
	{
		UInt32 saleId;
		Player *buyer;
        UInt32 id;
        UInt16 num;
	};
	BuyInfo binfo = *reinterpret_cast<BuyInfo *>(const_cast<void *>(data));
	player->GetSale()->sellSaleResp(binfo.saleId, binfo.buyer, binfo.id, binfo.num);
}

void OnSaleItemCancel( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	GObject::SaleItemCancel * saleItemCancel = reinterpret_cast<GObject::SaleItemCancel *>(const_cast<void *>(data));
	player->GetSale()->cancelSellResp(*saleItemCancel);
}

void OnSaleItemSearchReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	SaleSearchReq * saleSearchReq = reinterpret_cast<SaleSearchReq *>(const_cast<void *>(data));
	player->GetSale()->searchMySale(*saleSearchReq);
}
void OnSaleItemCancleAll( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	player->GetSale()->cancleAllItem();
}


void OnDailyCheck( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);

	player->GetTaskMgr()->CheckDayTask(TimeUtil::SharpDay(0));
	player->sendDailyInfo();

    player->buildClanTask(true);
    player->clearFinishCount();
    /*
    if (World::_thanksgiving)
        player->resetThanksgiving();
    */
    if (World::_blueactiveday)
        player->onBlueactiveday();
    player->GetShuoShuo()->reset();
    player->GetCFriend()->reset();
    player->sendSSDTInfo();
    player->send11DailyInfo();
    player->SetVar(VAR_JUNE_HAPPY, 0);
    player->SetVar(VAR_JUNE_ITEM, 0);
    player->sendHappyInfo();

    UInt32 time = *(UInt32*)data;
    player->SendNextdayTime(time);
    player->GetStrengthenMgr()->CheckTimeOver(time);
}

void OnExpGainByInstantCompleteReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct ExpGainInstantCompleteStruct
	{
		float exp;
		UInt32 duration;
		UInt32 count;
		GData::NpcGroup * ng;
	};
	const ExpGainInstantCompleteStruct * ecs = reinterpret_cast<const ExpGainInstantCompleteStruct *>(data);
	float exp = ecs->exp;
    float extraFactor = 0;
	UInt32 duration = ecs->duration;
    UInt32 curHookIndex = player->GetVar(VAR_EXP_HOOK_INDEX);
    UInt32 p;
    UInt32 extraExp = 0;

    if(player->inVipPrivilegeTime())
    {
        extraFactor = 1.0f;
        extraExp = static_cast<UInt32>(exp * 1.0f);
    }
    if(curHookIndex == ENUM_TRAINP1)
    {
        p = player->GetVar(VAR_TRAINP1);
        if(p > 0)
        {
            UInt32 left = p;
            if(left >= duration)
            {
                left -= duration;
                exp *= (extraFactor + 1.3f);
            }
            else
            {
                exp = exp* (1 + 0.3f * left / duration + extraFactor);
                left = 0;
            }
            player->SetVar(VAR_TRAINP1, left);
            player->sendExpHook(PLAYER_BUFF_START + PLAYER_BUFF_TRAINP1, left);
        }
    }
    else if(curHookIndex == ENUM_TRAINP2)
    {
        p = player->GetVar(VAR_TRAINP2);
        if(p > 0)
        {
            UInt32 left = p;
            if(left >= duration)
            {
                exp *= (1.6f + extraFactor);
                left -= duration;
            }
            else
            {
                exp = exp * (1 + 0.6f * left / duration + extraFactor);
                left = 0;
            }
            player->SetVar(VAR_TRAINP2, left);
            player->sendExpHook(PLAYER_BUFF_START + PLAYER_BUFF_TRAINP2, left);
        }
    }
    else if(curHookIndex == ENUM_TRAINP3)
    {
        p = player->GetVar(VAR_TRAINP3);
        if(p > 0)
        {
            UInt32 left = p;
            if(left >= duration)
            {
                exp *= (1.8f + extraFactor);
                left -= duration;
            }
            else
            {
                exp = exp * (1 + 0.8f * left / duration + extraFactor);
                left = 0;
            }
            player->SetVar(VAR_TRAINP3, left);
            player->sendExpHook(PLAYER_BUFF_START + PLAYER_BUFF_TRAINP3, left);
        }
    }
    else
    {
        exp *= 1 + extraFactor;
    }

	player->AddExp(static_cast<UInt64>(exp), 0, extraExp);
#if 0
	ecs->ng->monsterKilled(player, ecs->count);
#else
    UInt32 itemCount = 0;
    for(UInt32 i = 0; i < ecs->count; i++)
    {
        if(uRand(10000) < 74)
            ++itemCount;
    }
    player->GetPackage()->Add(9359, itemCount, true, false);
#endif
}

void OnAutoCopyAttack( GameMsgHdr& hdr, const void * data )
{
    if (!data)
        return;

	MSG_QUERY_PLAYER(player);

    if (player->isAutoCopyFailed())
        return;

    UInt8 id = *(UInt8*)data;
    UInt8 ret = playerCopy.fight(player, id, true);
    if (ret == 0)
        player->autoCopyFailed(id);
}

void OnAutoFrontMapAttack( GameMsgHdr& hdr, const void * data )
{
    if (!data)
        return;

	MSG_QUERY_PLAYER(player);

    UInt16 idspot = *(UInt16*)data;
    UInt8 ret = frontMap.fight(player, (idspot>>8)&0xFF, idspot&0xFF, true);
    if (ret == 0)
        player->autoFrontMapFailed();
}

void OnPlayerTimeTick( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    UInt8 type = *(UInt8*)(data);
    switch (type)
    {
        case 0:
            {
                player->sendNationalDayOnlineAward();
            }
            break;

        case 1:
            {
                GameAction()->onThanksgivingDay(player);
            }
            break;

        case 2:
            {
                GameAction()->onBlueactiveday(player);
            }
            break;

        default:
            break;
    }
}

void OnGoldRecharge( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    UInt8 type = *(UInt8*)(data);
    if (type == 0)
    {
        struct Recharge
        {
            UInt8 type;
            UInt32 gold;
            char no[256];
            char uint[32];
            char money[32];
        };

        Recharge* recharge = (Recharge*)(data);
        if(recharge->gold == 0)
            return;
        player->getGold(recharge->gold);
        player->addTotalRecharge(recharge->gold);
        DB8().PushUpdateData("UPDATE `recharge` SET `status` = 1 WHERE no = '%s' AND playerId = %"I64_FMT"u",
                recharge->no, player->getId());
        char gold[32] = {0};
        char nno[256] = {0};
        const char* id = "29999";
        snprintf(gold, 32, "%u", recharge->gold);
        snprintf(nno, 256, "%s#%s", recharge->uint, recharge->no);
        player->udpLog(nno, recharge->money, gold, id, "", "pay", "pay");

#ifndef _WIN32
#ifdef _FB
#else
        UInt32 mny = atoi(recharge->money);
        dclogger.fee(player, 0, mny);
#endif
#endif //_WIN32
    }
    else
    {
        struct Recharge
        {
            UInt8 type;
            UInt32 gold;
        };

        Recharge* recharge = (Recharge*)(data);
        if(recharge->gold == 0)
            return;
        player->getGold(recharge->gold);
        player->addTotalRecharge(recharge->gold);

        // XXX: 把创建银角色前的所有订单号置成成功
        DB8().PushUpdateData("UPDATE `recharge` SET `status` = 1 WHERE playerId = %"I64_FMT"u", player->getId());
    }
}

void OnDirectPurchase( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    struct DirectPurchase
    {
        UInt16 id;
        UInt16 num;
        UInt32 code; // 0-正常 1-未开启 2-次数上限
    };

    DirectPurchase* pur = (DirectPurchase*)data;
    if (!pur->id || !pur->num)
        return;

    if (pur->code == 1)
    {
        SYSMSG_SENDV(4916, player);
        player->sendMsgCode(0, 1090);
        return;
    }
    if (pur->code == 2)
    {
        SYSMSG_SENDV(4917, player);
        player->sendMsgCode(0, 1507);
        return;
    }

    Package* pkg = player->GetPackage();
    if (pkg->GetRestPackageSize() < 1)
    {
        player->sendMsgCode(0, 1011);
        return;
    }

    pkg->AddItem(pur->id, pur->num, true, false, FromDirectPurchase);
    player->AddVar(VAR_DIRECTPURCNT, 1);
    player->sendDirectPurInfo();
}

void OnYDPacks( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    UInt8 type = *(UInt8*)(data);
    if (type == 0xFF) // key 错误
    {
        player->sendMsgCode(1, 1005);
    }
    else if (type == 0xFE) // 已领取
    {
        player->sendMsgCode(1, 1006);
    }
    else if (type == 0xFD) // 验证服务器错
    {
        player->sendMsgCode(1, 1007);
    }
    else
    {
        GameAction()->onGetVipPack(player, type);
        UInt8 cnt = player->GetVar(type+VAR_KEYPACK1-1);
        player->SetVar(type+VAR_KEYPACK1-1, cnt+1);
    }
}

void OnGmHandler(GameMsgHdr& hdr, const void * data)
{
    std::string cmd(static_cast<const char *>(data));
    gmHandler.Handle(cmd,hdr.player,true);
}

void OnDelFgtTrainNotify( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	const UInt32 fgtId = *reinterpret_cast<const UInt32 *>(data);
	player->delTrainFighter(fgtId);
}

void OnClanSkillDonateBackReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct BackItems
	{
		UInt16 flag;
		UInt32 count;
	};
	const BackItems * items = reinterpret_cast<const BackItems *>(data);
	if (items->flag == 1)
	{
		//player->getAchievement(items->count);
	}
	else if (items->flag == 2)
	{
		player->getTael(items->count);
	}
	else
	{
		Package *package = player->GetPackage();
		if(package == NULL)
			return;
		package->AddItem(items->flag, items->count);
	}

}

void OnOperationTaskAction( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
    int type = *(int*)(data);
    if (player)
    {
        player->OperationTaskAction(type);
    }
}

void OnClanOption( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
	const ClanOpt* co = reinterpret_cast<const ClanOpt*>(data);

    switch(co->type)
    {
    case 0:     // 加入帮派
        {
            player->setClan(co->clan);

            if(player->getClanTaskMax() > PLAYER_DATA(player, ctFinishCount))
                player->buildClanTask();
        }
        break;
    case 1:    // 开除帮派
    case 2:    // 离开帮派
        {
            GObject::ClanCopyMgr::Instance().playerLeave(player);
            player->delClanTask();
            player->setClan(co->clan);
        }
        break;
    }
}

void OnClanSkillDonateCheckReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct DonateItems
	{
		UInt8  skillId;
		UInt8 flag;
		UInt32 count;
		UInt8 ret;
	};
	const DonateItems * items = reinterpret_cast<const DonateItems *>(data);
	UInt8 r = 0;
	ConsumeInfo ci(ClanDonate, 0, 0);
	if (items->flag == 1)
	{
        // 个人资金
		if (items->count > player->getTael())
			r = 1;
		else
			player->useTael(items->count, &ci);
	}
	else if (items->flag == 2)
	{
        // 帮派资金
        Clan* clan = player->getClan();
        if(NULL == clan)
            r = 1;
        else if(clan->getOwner() != player)
            r = 1;
        else if (items->count > clan->getClanFunds())
			r = 1;
		else
			clan->useClanFunds(items->count);
	}
	else
	{
		Package *package = player->GetPackage();
		if(package == NULL)
			return;
		ItemBase *FavorItem = package->GetItem(items->flag);
		if(FavorItem == NULL || FavorItem->Count() < items->count)
			r = 1;
		else
			package->DelItemAny(items->flag, items->count);
	}
	DonateItems itemsRet = { items->skillId, items->flag, items->count, r };
	GameMsgHdr hdr2(0x175, WORKER_THREAD_WORLD, player, sizeof(DonateItems));
	GLOBAL().PushMsg(hdr2, &itemsRet);
}

void OnDailyReq2(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	player->sendDailyInfo();
}

void OnAddAwardGold(GameMsgHdr& hdr, const void * data)
{
	MSG_QUERY_PLAYER(player);
	UInt16 Award = *reinterpret_cast<UInt16 *>(const_cast<void *>(data));
	player->getCoupon(Award);
}

void OnCreateAward(GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    player->GetPackage()->AddItem(18, 1, true);
    //player->GetPackage()->AddItem(449, 1, true); // XXX: 首充礼包
    player->getCoupon(888);
#if defined(_FB) && defined(_FB_TEST)
    player->getGold(20000);
    player->setVipL(6);
#endif
    player->udpLog("create", "", "", "", "", "", "guide");
    StringTokenizer via(player->getVia(), "_");
    if (via.count() == 2)
        player->udpLog(via[0].c_str(), via[1].c_str(), "", "", "", "", "refer");
    else if (via.count() == 1)
        player->udpLog(via[0].c_str(), "", "", "", "", "", "refer");
    else
        player->udpLog("", "", "", "", "", "", "refer");
    player->sendCreateMail();
#ifndef _WIN32
#ifdef _FB
#else
    dclogger.reg(player);
    dclogger.reg_union(player);
#endif
#endif //_WIN32
}

void OnRunScriptReq( GameMsgHdr&, const void * data )
{
	const char * script = reinterpret_cast<const char *>(data);
	GameAction()->runScript(script);
}

void OnRunScriptBattleReq( GameMsgHdr&, const void * data )
{
	const char * script = reinterpret_cast<const char *>(data);
	Script::BattleFormula::getCurrent()->runScript(script);
}


void OnClanBattleAttendReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	if(World::_activityStage > 0)
		GameAction()->onClanBattleAttend(player);
}
void OnArenaBet( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt16 tael = *reinterpret_cast<const UInt16 *>(data);
    if(tael == 0)
        player->GetPackage()->DelItemAny(ARENA_BET_ITEM2, 1, NULL, ToArenaBet);
    else
        player->GetPackage()->DelItemAny(ARENA_BET_ITEM1, 1, NULL, ToArenaBet);
}

void OnArenaBetResult( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt16 idata = *reinterpret_cast<const UInt16 *>(data);
	if(idata & 0x100)
		player->getTael(idata & 0xFF);
}

void OnPracticeExpAddReq( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    const PracticeFighterExp* pfexp = reinterpret_cast<const PracticeFighterExp*>(data);
    player->AddPracticeExp(pfexp);
}

void OnSetPropsReq( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    struct Props
    {
        UInt32 pexp;
        UInt32 prestige;
        UInt32 honor;
    };

    Props* props = (Props*)(data);
    if (props->pexp)
        player->AddPExp(props->pexp);
    if (props->honor)
        player->getAchievement(props->honor);
    if (props->prestige)
    {
        player->getPrestige(props->prestige);
    }
}

void OnSetLevelReq( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 level = *(UInt8*)(data);
    UInt64 exp = GData::expTable.getLevelMin(level);
    player->setLevelAndExp(level, exp);
}

void OnSetMoneyReq( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    struct Money
    {
        UInt8 type;
        UInt32 gold;
        UInt32 tael;
        UInt32 coupon;
        UInt32 achievement;
    };
    Money* money = (Money*)(data);
    if (money->type == 0)
    {
        player->getGold(money->gold);
        player->getTael(money->tael);
        player->getCoupon(money->coupon);
        player->getAchievement(money->achievement);
    }
    else if (money->type == 1)
    {
        if (player->getGold() > money->gold)
            player->useGold(money->gold);
        else
            player->useGold(player->getGold());

        if (player->getTael() > money->tael)
            player->useTael(money->tael);
        else
            player->useTael(player->getTael());

        if (player->getCoupon() > money->coupon)
            player->useCoupon(money->coupon);
        else
            player->useCoupon(player->getCoupon());

        if (player->getAchievement() > money->achievement)
            player->useAchievement(money->achievement);
        else
            player->useAchievement(player->getAchievement());
    }
}

void OnSetVipLReq( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 lvl = *(UInt8*)(data);
    player->setVipL(lvl);
}

void OnClanSkillLevel( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 skillId = *(UInt8*)(data);
    player->clanSkillLevelUp(skillId);
    player->setFightersDirty(true);
}

void OnClanSkillList( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 skillId = *(UInt8*)(data);

    if(0 == skillId)
        player->listClanSkills();
    else
        player->showClanSkill(skillId);
}

void OnClearTaskReq( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 type = *(UInt8*)(data);
    if (type == 1)
    {
        player->resetShiMen();
    }
    else if (type == 2)
    {
        player->resetYaMen();
    }
}
#if 0
void OnMartialUpdateHdr( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    if(hdr.msgHdr.bodyLen != sizeof(GObject::MartialHeader) || !data)
        return;
	const GObject::MartialHeader* mh = reinterpret_cast<const GObject::MartialHeader*>(data);
    player->GetAthletics()->updateMartialHdr(mh);
}
#endif
#if 0
void OnMartialUpdate( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    if(hdr.msgHdr.bodyLen != sizeof(GObject::MartialData) || !data)
        return;
	const GObject::MartialData* md = reinterpret_cast<const GObject::MartialData*>(data);
    player->GetAthletics()->updateMartial(md);
}
#endif
#if 0
void OnMartialUpdate2( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 type = *(UInt8*)(data);
    player->GetAthletics()->listAthleticsMartial2(type);
}
#endif
void OnAthleticsMartialAttack( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
	GObject::Player * defer = *reinterpret_cast<GObject::Player **>(const_cast<void *>(data));
	player->GetAthletics()->attackMartial(defer);
}

void OnAthleticsMartialBeAttack( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
	struct AthleticsBeData
	{
		Player * attacker;
		UInt16 formation;
		UInt16 portrait;
		Lineup lineup[5];
	};
	AthleticsBeData * abd = reinterpret_cast<AthleticsBeData *>(const_cast<void *>(data));
	player->GetAthletics()->beAttackMartial(abd->attacker, abd->formation, abd->portrait, abd->lineup);
}

void OnAwardAthleticsMartial( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    struct AthleticsResNotify
    {
        GObject::Player * peer;
        bool win;
    };
    AthleticsResNotify * notify = reinterpret_cast<AthleticsResNotify *>(const_cast<void *>(data));

    player->GetAthletics()->awardMartial(notify->peer, notify->win);
}
void OnAthlectisPayPaging( GameMsgHdr & hdr,  const void* data)
{
    MSG_QUERY_PLAYER(player);

    const GObject::AthleticsPay * co = reinterpret_cast<const GObject::AthleticsPay*>(data);

    if(co->type == AthleticsRank::AthleticsPayForPaging)
        player->GetAthletics()->PayForPaging(co->type);
    else if(co->type == AthleticsRank::AthleticsPayForKillCD)
        player->GetAthletics()->PayForKillCD(co->type);

}

void  OnDoAttainment(  GameMsgHdr& hdr, const void* data)
{
     MSG_QUERY_PLAYER(player);
     const stAttainMsg* co = reinterpret_cast<const stAttainMsg*>(data);

     if(co->attainID == Script::ONE_FRIEND_LEV_UP)
     {
        player->OnFriendLevUp(co->param);
     }
     else if(co->attainID ==  Script:: ADD_FRIEND)
     {
        player->OnAddOneFriend() ;
     }
     else
     {
        player->OnDoAttainment(co->attainID, co->param);
     }
}
void  OnDoActivity( GameMsgHdr& hdr, const void* data)
{
    MSG_QUERY_PLAYER(player);
    const stActivityMsg* co = reinterpret_cast<const stActivityMsg*>(data);
    if(co->id == SthTownDeamon)
    {
        if(player->GetVar(VAR_TOWNDEAMON))
            return;
        else
            player->SetVar(VAR_TOWNDEAMON, 1);
    }

    GameAction()->doStrong(player, co->id, co->param1, co->param2);
}
void OnAwardHIPrestige( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt16 prestige = *(UInt16*)(data);
    if (prestige)
        player->getPrestige(prestige);
}
void OnSendShusanLoveTitleCard( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    int pos = *(int*)(data);
    player->sendShusanLoveTitleCard(pos);
}
void OnSendMayDayTitleCard( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    int pos = *(int*)(data);
    player->sendMayDayTitleCard(pos);
}

void OnSendJuneHappyTitleCard( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    int pos = *(int*)(data);
    player->sendJuneHappyTitleCard(pos);
}

void OnAddPExpBy( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt32 pexp = *(UInt32*)(data);
    player->AddPExp(pexp);
}
void OnAddItemBy( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    struct ItemAdd
    {
        UInt16 item;
        UInt16 num;
        bool bind;
    };

    ItemAdd* ia = (ItemAdd*)(data);
    player->GetPackage()->AddItem(ia->item, ia->num, ia->bind);
}
void OnPracticeAttack( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    GObject::practicePlace.doAttack(player, data);
}
void OnHeroMemo( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8* msg = (UInt8*)(data);
    player->OnHeroMemo(msg[0], msg[1], msg[2], msg[3]);
}
void OnShuoShuo( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 idx = *(UInt8*)(data);
    player->OnShuoShuo(idx);
}

void OnTownDeamonlBeAttack( GameMsgHdr& hdr, const void* data )
{
	MSG_QUERY_PLAYER(player);
	struct TDBeAttackData
	{
		Player * attacker;
        UInt32 spirit;
		UInt16 formation;
		UInt16 portrait;
		Lineup lineup[5];
	};

	TDBeAttackData* tdabd = reinterpret_cast<TDBeAttackData*>(const_cast<void *>(data));

    townDeamonManager->beAttackByPlayer(player, tdabd->attacker, tdabd->spirit, tdabd->formation, tdabd->portrait, tdabd->lineup);
}

void OnCFriendLvlUp( GameMsgHdr& hdr, const void* data )
{
    struct PlayerLvlUp
    {
        Player* player;
        UInt8 lvl;
    };

    MSG_QUERY_PLAYER(player);
    PlayerLvlUp* msg = (PlayerLvlUp*)data;
    player->OnCFriendLvlUp(msg->player, msg->lvl);
}

void OnRemoveClanRank( GameMsgHdr& hdr, const void* data )
{
    struct ClanData
    {
        Clan* clan;
    };

    ClanData* clan_data = reinterpret_cast<ClanData*>(const_cast<void*>(data));

    ClanRankBattleMgr::Instance().removeClanRank(clan_data->clan);
}

void OnAddItemByIDIP( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    struct AddItem
    {
        UInt16 itemid;
        UInt16 itemnum;
    };

    AddItem* ai = (AddItem*)data;
#if 0
    player->GetPackage()->Add(ai->itemid, ai->itemnum, true, false, FromIDIP);
#else
    player->IDIPAddItem(ai->itemid, ai->itemnum, true);
#endif

}

void OnSendRNR( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    struct SendRNR
    {
        Player* player;
        UInt32 off;
        UInt32 date;
        UInt32 total;
    };
    SendRNR* rnr = (SendRNR*)data;
    GameAction()->sendRNR(rnr->player, rnr->off, rnr->date, rnr->total);
}

void OnGetArenaMoney( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
	const UInt32 arenaMoney = *reinterpret_cast<const UInt32 *>(data);
    IncommingInfo ii(ArenaFromMail,0,0);
    player->getMoneyArena(arenaMoney, &ii);
}

void OnArenaEnterCommit( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
	const UInt8 type = *reinterpret_cast<const UInt8 *>(data);

#ifdef _FB
#define LIMIT_LEVEL  60
#else
#define LIMIT_LEVEL  70
#endif


    if(player->GetLev() < LIMIT_LEVEL)
        return;
    if(type == 0)
    {
        Stream st(ARENAREQ::ENTER, 0xEF);
        st << player->getId() << player->getName() << static_cast<UInt8>(player->getTitle());
        player->appendLineup2(st);
        player->appendPetOnBattle(st);
        st << Stream::eos;
        NETWORK()->SendToArena(st);
    }
    else if(type == 1)
    {
        Stream st(ARENAREQ::COMMIT_LINEUP, 0xEF);
        st << player->getId();
        player->appendLineup2(st);
        player->appendPetOnBattle(st);
        st << Stream::eos;
        NETWORK()->SendToArena(st);
    }
}
void OnSendPExpCard( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    int pos = *(int*)(data);
    player->sendPExpCard(pos);
}

void OnRoamintQueqiao( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    UInt8 pos = *(UInt8*)(data);
    player->roamingQueqiao(pos);
}

void OnRoamintQueqiaoLastLoot( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    player->checkLastQueqiaoAward();
}

void OnNewRC7DayLastLoot( GameMsgHdr& hdr, const void* data)
{
    MSG_QUERY_PLAYER(player);
    player->checkLastNew7DayTargetAward();
}

void OnDelItemAny( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    struct DelItemInfo
    {
        UInt32 id;
        UInt16 num;
        UInt8 toWhere;
    };

	const DelItemInfo* item = reinterpret_cast<const DelItemInfo*>(data);
    if(item)
        player->GetPackage()->DelItemAny(item->id, item->num, NULL, item->toWhere);
}

void OnAddItem( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    struct AddItemInfo
    {
        UInt32 id;
        UInt16 num;
        UInt8 bind;
        UInt8 fromWhere;
    };

	const AddItemInfo* item = reinterpret_cast<const AddItemInfo*>(data);
    if(item)
        player->GetPackage()->AddItem2(item->id, item->num, true, item->bind, item->fromWhere);
}

void OnTownDeamonChallenge( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    struct TownDeamonChallenge
    {
        Player * defer;
        UInt32   spirit;
    };

	const TownDeamonChallenge* tdc = reinterpret_cast<const TownDeamonChallenge*>(data);
    if(tdc)
        townDeamonManager->attackPlayer(player, tdc->defer, tdc->spirit);
}

void OnTownDeamonAutoCompleteQuite( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    struct TownLevels
    {
        UInt16 curLevel;
        UInt16 levels;
    };

	const TownLevels* tls = reinterpret_cast<const TownLevels*>(data);
    if(tls)
        townDeamonManager->autoCompleteQuite(player, tls->curLevel, tls->levels);
}

void OnTownDeamonAttackNpc( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	const UInt16 level = *reinterpret_cast<const UInt16*>(data);
    townDeamonManager->attackNpc(player, level);
}


#if 0
void OnAdvancedHookExp( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    player->advancedHookExp();
}
#endif
void OnSendRechargeRankAward( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    player->sendRechargeRankAward(*(int*)data);
}
void OnSendConsumeRankAward( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    player->sendConsumeRankAward(*(int*)data);
}

void OnSHFighterCloneReq( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
	Fighter* fgt = *reinterpret_cast<Fighter**>(const_cast<void *>(data));
    Fighter* fgtClone = fgt->cloneWithOutDirty(NULL);
    if(fgt->getId() < 10)
        fgtClone->setName(fgt->getName());

    GameMsgHdr hdr2(0x1A8, WORKER_THREAD_WORLD, player, sizeof(Fighter**));
    GLOBAL().PushMsg(hdr2, &fgtClone);
}

void OnTitleCheck( GameMsgHdr& hdr, const void* data )
{
    MSG_QUERY_PLAYER(player);
    player->getTitle();
}

void OnSetTitle( GameMsgHdr& hdr, const void* data )
{
    struct TitleData
    {
        UInt8 title;
        UInt32 timeLen;
    } titleData = {0};

    MSG_QUERY_PLAYER(player);
    titleData = *reinterpret_cast<TitleData*>(const_cast<void *>(data));
    player->setTitle(titleData.title, titleData.timeLen);
}

void OnFighterCheckDiry( GameMsgHdr& hdr, const void * data )
{
    MSG_QUERY_PLAYER(player);
    player->getBattlePoint();
}

struct IDIPBuyItemInfo
{
    UInt32 itemId;
    UInt32 num;
    UInt32 bind;
    UInt32 price;
};

void OnIDIPBuy( GameMsgHdr& hdr, const void * data )
{
    MSG_QUERY_PLAYER(player);
    IDIPBuyItemInfo* ibi = (IDIPBuyItemInfo*)data;
    ConsumeInfo ci(IDIPBuyItem, ibi->itemId, ibi->num);
    player->useGold(ibi->price, &ci);
    player->GetPackage()->Add(ibi->itemId, ibi->num, ibi->bind);
}

void OnAutoJobHunterStep( GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    JobHunter * job_hunter = player->getJobHunter();
    if (!job_hunter)
    {
        return;
    }
    job_hunter->OnAutoCommand(0x10);
}

void OnAddTianjieNpc( GameMsgHdr& hdr, const void * data)
{
    struct TianjieSpotNpc
    {
        UInt32 npcId;
        UInt16 spot;
    };
    TianjieSpotNpc* tjNpc = reinterpret_cast<TianjieSpotNpc*>(const_cast<void *>(data));
    if(tjNpc)
        GObject::Tianjie::instance().addTianjieNpc(tjNpc->npcId, tjNpc->spot);
}

void OnDelTianjieNpc( GameMsgHdr& hdr, const void * data)
{
    struct TianjieSpotNpc
    {
        UInt32 npcId;
        UInt16 spot;
    };
    TianjieSpotNpc* tjNpc = reinterpret_cast<TianjieSpotNpc*>(const_cast<void *>(data));
    if(tjNpc)
        GObject::Tianjie::instance().delTianjieNpc(tjNpc->npcId, tjNpc->spot);
}


void OnDelMapObj( GameMsgHdr& hdr, const void * data)
{
    struct MapNpc
    {
        UInt16 loc;
        UInt32 npcId;
    };
    MapNpc* mapNpc = reinterpret_cast<MapNpc*>(const_cast<void *>(data));
    if(!mapNpc)
        return;
    Map * map = Map::FromSpot(mapNpc->loc);
    if (!map)
        return;
    map->Hide(mapNpc->npcId);
    map->DelObject(mapNpc->npcId);
}

void OnAddMapObj( GameMsgHdr& hdr, const void * data)
{
    MOData* mo = reinterpret_cast<MOData*>(const_cast<void *>(data));
    if(!mo)
        return;
    Map * map = Map::FromSpot(mo->m_Spot);
    if (!map)
        return;

    map->AddObject(*mo);
    map->Show(mo->m_ID, true, mo->m_Type);
}

void OnCFriendAthleticsRank( GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    player->OnCFriendAthleticsRank();
}

void OnForbidSaleQueryFail( GameMsgHdr &hdr, const void *data)
{
    MSG_QUERY_PLAYER(player);
    const Int32 ret = *(reinterpret_cast<Int32*>(const_cast<void *>(data)));
    char buf[16];
    snprintf(buf, 16, "%d", ret);
    player->udpLog("svr_forbid_sale", buf, "", "", "", "", "act_tmp");
}

void OnOpenIdInvalid( GameMsgHdr &hdr, const void *data)
{
	MSG_QUERY_PLAYER(player);
    char* openId = reinterpret_cast<char*>(const_cast<void *>(data));
    std::string invalidOpenId = player->getOpenId();
    std::string validOpenId = openId;
    player->setOpenId(validOpenId);
    player->udpLog("invalid_openid", invalidOpenId.c_str(), openId, "", "", "", "act_tmp");
    if (cfg.autoKick)
    {
        //player->selfKick(); // 这行代码完全没效果
        GameMsgHdr imh(0x200, player->getThreadId(), player, 0);
        GLOBAL().PushMsg(imh, NULL);
#ifndef _WIN32
#ifdef _FB
#else
        GObject::dclogger.decDomainOnlineNum(atoi(player->getDomain()));
#endif
#endif // _WIN32
    }
}

void OnOpenAPIFailed( GameMsgHdr &hdr, const void *data)
{
	MSG_QUERY_PLAYER(player);
#define MAX_MSG_LEN 1024
    struct OpenAPIFailedInfo
    {
        UInt32 type;
        Int32  ret;
        char   msg[MAX_MSG_LEN];
    };
	const OpenAPIFailedInfo * faildInfo = reinterpret_cast<const OpenAPIFailedInfo*>(const_cast<void *>(data));
    char buf[32] = "";
    switch (faildInfo->type)
    {
        case 0:
            snprintf (buf, 32, "%s", "is_login");
            break;
        case 1002:
            snprintf (buf, 32, "%s", "punish");
            break;
        case 3333:
            snprintf (buf, 32, "%s", "union_reg");
            break;
        default:
            snprintf (buf, 32, "unknow_%d", faildInfo->type);
            break;
    }
    char buf2[32] = "";
    snprintf( buf2, 32, "%d", faildInfo->ret);
    player->udpLog(buf, buf2, faildInfo->msg, "", "", "", "act_tmp");
    if (faildInfo->ret == 1002 
            || faildInfo->ret == 1005
            || faildInfo->ret == 1006
            || faildInfo->ret == 1300
            || faildInfo->ret == -2
            || faildInfo->ret == -64
            )
    {
        player->selfKick();
    }
}

void OnPostDragonKing( GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
	const UInt8 count = *reinterpret_cast<const UInt8*>(data);
    player->postDragonKing(count);
}

void OnSaveGoldAct( GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    struct goldData
    {
        UInt8 opt;
        UInt32 param;
    };
    goldData * gData = reinterpret_cast<goldData*>(const_cast<void *>(data));
    if(gData)
        player->saveGoldAct(gData->opt, gData->param);
}

void OnFoolsDayAct( GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    struct foolsData
    {
        UInt8 type;
        UInt8 id;
        char answer;
    };
    foolsData * fdata = reinterpret_cast<foolsData*>(const_cast<void *>(data));
    if(fdata)
    {
        switch(fdata->type)
        {
            case 0x00:
                player->checkAnswerActInFoolsDay();
                break;
            case 0x01:
                player->sendFoolsDayInfo();
                break;
            case 0x02:
                player->submitAnswerInFoolsDay(fdata->id, fdata->answer);
                break;
            case 0x03:
                player->getAwardInFoolsDay();
                break;
            case 0x04:
                player->buyResurrectionCard();
                break;
        }
    }
}

void OnLuckyStarAct( GameMsgHdr& hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    struct starData
    {
        UInt8 type;
        UInt8 idx;
    };
    starData * sdata = reinterpret_cast<starData*>(const_cast<void *>(data));
    if(sdata)
    {
        switch(sdata->type)
        {
            case 0x01:
                player->sendLuckyStarInfo(2);
                break;
            case 0x02:
                player->getLuckyStarItem(sdata->idx);
                break;
        }
    }
}

void OnCalcLBBattlePoint( GameMsgHdr &hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    player->calcLingbaoBattlePoint();
}

void OnSpreadWhisper(GameMsgHdr &hdr, const void* data)
{
	MSG_QUERY_PLAYER(player);
	if(player->getBuffData(PLAYER_BUFF_BANCHAT)!=0)
        return;

	ChatRep rep;
	rep.type = 0xFF;
    UInt64 playerId = *(reinterpret_cast<UInt64 *>(const_cast<void *>(data)));
	GObject::Player * pl = GObject::globalPlayers[playerId];
	if(pl == NULL || pl->GetSessionID() == -1)
	{
		rep.cny = 0;
		rep.sex = 0;
		rep.office = player->getTitle();
		rep.guard = player->getPF();
		rep.level = player->GetLev();
		player->send(rep);
	}
	else
	{
		rep.name = player->getName();
        SYSMSGV(content, 741);
		rep.text = content;
		rep.cny = player->getCountry();
		rep.sex = player->IsMale() ? 0 : 1;
		rep.office = player->getTitle();
		rep.guard = player->getPF();
		rep.level = player->GetLev();
		pl->send(rep);
	}
}

void OnSurnameLegendAct( GameMsgHdr &hdr, const void * data  )
{
    MSG_QUERY_PLAYER(player);
    if(player==NULL)
        return ;
    UInt8 type = *(reinterpret_cast<UInt8 *>(const_cast<void *>(data)));
   switch(type)
   {
    case 0x00:
        player->sendLuckyBagInfo();
        break;
    case 0x03:
          //player->GetLuckyBagAward();
          GameAction()->GetLuckyBagAward(player);
          break;
   }
    
}

void OnSendSpreadAwardInfo(GameMsgHdr &hdr, const void* data)
{
    MSG_QUERY_PLAYER(player);
    player->sendSpreadAwardInfo();
}

void OnSpreadGetAward(GameMsgHdr &hdr, const void* data)
{
    MSG_QUERY_PLAYER(player);
    UInt32 spreadCount = *reinterpret_cast<const UInt32 *>(data);
    player->spreadGetAwardInCountry(spreadCount);
}

void OnBeVoted( GameMsgHdr &hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    player->beVoted();
}

void OnSendPopularityAward(GameMsgHdr &hdr, const void * data)
{
    MSG_QUERY_PLAYER(player);
    player->sendPopularityRandAward(*(int*)data);
}

void OnClanRankBattleReqInitInner(GameMsgHdr& hdr,const void* data)
{
    MSG_QUERY_PLAYER(player);

	BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt8 type = 0;
    brd >> type;

    switch(type)
    {
        case 0: //请求帮会战状态信息
            {
                ClanRankBattleMgr::Instance().SendInitStatus(player);
            }
            break;
        case 1: //报名进入战斗
            {
                UInt8 field = 0;
                brd >> field;
                ClanRankBattleMgr::Instance().Signup(player, field);
            }
            break;
        case 2: //取消报名
            {
                ClanRankBattleMgr::Instance().Signout(player);
            }
            break;
        case 8:
            {
                GObject::Clan *clan = player->getClan();
                if(clan)
                    clan->broadcastClanBattle(player);
            }
            break;
        default:
            break;
    }
}

void OnClanRankBattleSortListInner(GameMsgHdr& hdr, const void* data)
{
    MSG_QUERY_PLAYER(player);

    BinaryReader brd(data, hdr.msgHdr.bodyLen);
    UInt16 startId = 0;
    UInt8 count = 0;
    brd >> startId >> count;

    ClanRankBattleMgr::Instance().SendSortList(player, startId, count);
}

#endif // _COUNTRYINNERMSGHANDLER_H_



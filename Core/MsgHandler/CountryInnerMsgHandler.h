#ifndef _COUNTRYINNERMSGHANDLER_H_
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
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "GObject/Athletics.h"
#include "GObject/AthleticsRank.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"
#include "Script/BattleFormula.h"
#include "GObject/Tripod.h"

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
		//DB().PushUpdateData("INSERT INTO `item`(`id`, `itemNum`, `ownerId`, `bindType`) VALUES(8942, 1, %"I64_FMT"u, 1)", player->getId());
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
		DB().PushUpdateData("UPDATE `player` SET `inCity` = %u, `location` = %u WHERE id = %" I64_FMT "u", PLAYER_DATA(player, inCity), PLAYER_DATA(player, location), player->getId());
		GObject::Map * map = GObject::Map::FromSpot(newLoc);
		if(map == NULL)
			return;
		if(ces->inCity)
		{
			map->SendCityNPCs(player);
		}
		map->SendAtCity(player, ces->inCity == 1);
	}
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
	DBLOG().PushUpdateData("insert into login_states (server_id,player_id,login_time,login_ip) values(%u, %"I64_FMT"u, %u, '%s')", cfg.serverLogId, player->getId(), TimeUtil::Now(), inet_ntoa(ip));

    tripod.getTripodData(player); // XXX: 完家登陆之后如果没有九疑鼎数据则新建

    {
        UInt32 count = LOGIN().Count();
        LOGIN().GetLog()->OutInfo("来自[%s]用户[%"I64_FMT"u]登陆[%05u]成功, 登陆流水号: %d, 当前在线人数: %d\n",
                inet_ntoa(ip), player->getId(), cfg.serverNum, count, LOGIN().Current());
    }
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
    {
        LOGIN().Logout();
        LOGIN().GetLog()->OutInfo("用户[%s][%"I64_FMT"u]正常退出游戏, 当前在线人数: %d\n",
                player->getName().c_str(), player->getId(), LOGIN().Current());
    }
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
}

void OnAthleticsBeReq( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	struct AthleticsBeData
	{
		Player * attacker;
		UInt8 formation;
		UInt16 portrait;
		Lineup lineup[5];
	};
	AthleticsBeData * abd = reinterpret_cast<AthleticsBeData *>(const_cast<void *>(data));
	player->GetAthletics()->beAttack(abd->attacker, abd->formation, abd->portrait, abd->lineup);
}

void OnAthleticsEnterResp( GameMsgHdr& hdr, const void * )
{
	MSG_QUERY_PLAYER(player);
	player->GetAthletics()->setEnterAthletics();
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
        player->GetPackage()->AddItem(awd->itemId, awd->itemCount, 1);

	if(awd->side == 0)
		player->GetAthletics()->defendergainsource(awd->other, awd->athleticsid, awd->type, awd->count);
	else
	{
		if(awd->side > 1)
			player->notifyFriendAct(8, awd->side - 1);
		//if(World::_newYearStage == 9)
		//	awd->count = GameAction()->onAttakerAddexp(player, awd->other, awd->count);
		player->GetAthletics()->attackergainsource(awd->athleticsid, awd->type, awd->count);
		if(World::_activityStage > 0 && awd->win)
			GameAction()->onAthleticWin(player);
	}
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
						DBLOG().PushUpdateData("insert into `item_courses`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, notify->launcher->getId(), notify->items[i]->getId(), notify->items[i]->Count(), FromTrade, now);
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
					DBLOG().PushUpdateData("insert into `item_courses`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, player->getId(), (*it)->getId(), (*it)->Count(), FromTrade, now);
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
	};
	BuyInfo binfo = *reinterpret_cast<BuyInfo *>(const_cast<void *>(data));
	player->GetSale()->sellSaleResp(binfo.saleId, binfo.buyer);
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

void  OnDailyCheck( GameMsgHdr& hdr, const void * )
{
	MSG_QUERY_PLAYER(player);

	player->GetTaskMgr()->CheckDayTask(TimeUtil::SharpDay(0));
	player->sendDailyInfo();
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
	UInt32 duration = ecs->duration;
	UInt32 now = TimeUtil::Now();
	bool hasP2;
	UInt32 p = player->getBuffData(PLAYER_BUFF_TRAINP3, now);
	if(p > 0)
	{
		hasP2 = true;
		UInt32 left = p - now;
		if(left >= duration)
		{
			exp *= 1.7f;
			player->setBuffData(PLAYER_BUFF_TRAINP3, p - duration);
		}
		else
		{
			exp = exp + exp * left * 7 / duration / 10;
			player->setBuffData(PLAYER_BUFF_TRAINP3, 0);
		}
	}
	else
		hasP2 = false;
	p = player->getBuffData(PLAYER_BUFF_TRAINP4, now);
	if(p > 0)
	{
		UInt32 left = p - now;
		if(left >= duration)
		{
			if(!hasP2)
				exp *= 1.5f;
			player->setBuffData(PLAYER_BUFF_TRAINP4, p - duration);
		}
		else
		{
			if(!hasP2)
				exp = exp + exp * left  / duration / 2;
			player->setBuffData(PLAYER_BUFF_TRAINP4, 0);
		}
		hasP2 = true;
	}
	else
		hasP2 = false;
	p = player->getBuffData(PLAYER_BUFF_TRAINP2, now);
	if(p > 0)
	{
		UInt32 left = p - now;
		if(left >= duration)
		{
			if(!hasP2)
				exp *= 1.5f;
			player->setBuffData(PLAYER_BUFF_TRAINP2, p - duration);
		}
		else
		{
			if(!hasP2)
				exp = exp + exp * left / duration / 2;
			player->setBuffData(PLAYER_BUFF_TRAINP2, 0);
		}
		hasP2 = true;
	}
	else
		hasP2 = false;
	p = player->getBuffData(PLAYER_BUFF_TRAINP1, now);
	if(p > 0)
	{
		UInt32 left = p - now;
		if(left >= duration)
		{
			if(!hasP2)
				exp *= 1.3f;
			player->setBuffData(PLAYER_BUFF_TRAINP1, p - duration);
		}
		else
		{
			if(!hasP2)
				exp = exp + exp * left * 3 / duration / 10;
			player->setBuffData(PLAYER_BUFF_TRAINP1, 0);
		}
	}
	player->AddExp(static_cast<UInt32>(exp));
	ecs->ng->monsterKilled(player, ecs->count);
}

void OnCompleteAutoCopy( GameMsgHdr& hdr, const void * data )
{
    if (!data)
        return;

	MSG_QUERY_PLAYER(player);

    if (player->isAutoCopyFailed())
        return;

    UInt8 id = *(UInt8*)data;
    playerCopy.fight(player, id, true, true);
}

void OnGoldRecharge( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
	UInt32 gold = *static_cast<const UInt32 *>(data);
	if(gold == 0)
		return;
	player->getGold(gold);
	player->addTotalRecharge(gold);
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
	player->useTael(tael);
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

#endif // _COUNTRYINNERMSGHANDLER_H_

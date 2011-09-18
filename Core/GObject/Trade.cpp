#include "Config.h"
#include "Common/Itoa.h"
#include "Player.h"
#include "Package.h"
#include "MsgID.h"
#include "Trade.h"
#include "Mail.h"
#include "Server/SysMsg.h"
#include "Server/OidGenerator.h"
#include "Server/Cfg.h"
#include <mysql.h>

namespace GObject
{

//UInt8 SSTATUS2CSTATUS[] = { 0xFF, 1, 0, 4, 3, 0xFF, 5, 6, 2, 2, 7, 9, 8, 10, 2 };
struct SSTATUS2CSTATUST {
	UInt8 operator[](UInt8 s)
	{
		static const UInt8 _status[] = { 0xFF, 1, 0, 4, 3, 0xFF, 5, 6, 2, 2, 7, 9, 5, 2, 2 };
		if (s > static_cast<UInt8>(TRADE_TIMEOUT))
		{
			//ERROR_LOG("Error Trade Status : tradeId = %u, status = %d", );
			return 0xFF;
		}
		return _status[s];
	}
}SSTATUS2CSTATUS;

//·¢Æð½»Ò×Í¨Öª
struct LaunchTradeNotify
{
	UInt32 id;
	Player* launcher;
	char title[49];
	UInt32 time;

	LaunchTradeNotify()
	{
		memset(this, 0x00, sizeof(LaunchTradeNotify));
	}
};

struct OperateTradeNotify
{
	UInt32 id;
	UInt8  status;

	OperateTradeNotify() : id(0), status(0) {}
};

struct ConfirmTradeNotify
{
	UInt32 id;
	Player* launcher;
	UInt32 coin;
	UInt32 gold;
	ItemBase* items[12];

	ConfirmTradeNotify()
	{
		memset(this, 0x00, sizeof(ConfirmTradeNotify));
	}
};

struct TradeDataNotify
{
	UInt32 id;
	UInt8  status;
	Player * receiver;

	TradeDataNotify() : id(0), status(0xFF), receiver(NULL) {}
};

struct TradeDataCheck
{
	UInt32 id;
	UInt32 time;

	TradeDataCheck(UInt32 id_, UInt32 time_) : id(id_), time(time_) {}
};

Trade::Trade(Player * player) : _owner(player)
{
}

Trade::~Trade()
{
	TradeDataList::iterator it = _tradeDatas.begin();
	for (; it != _tradeDatas.end(); ++it)
	{
		for (UInt16 i = 0; i < it->second->_items.size(); ++i)
		{
			SAFE_DELETE(it->second->_items[i]);
		}
		SAFE_DELETE(it->second);
	}
	_tradeDatas.clear();
}

//BUG TODO--
bool Trade::addTradeFromDB(TradeData* trade)
{
	assert(trade != NULL);
        _tradeDatas[trade->_id] = trade;
	//TradeData* & found = _tradeDatas[trade->_id];
	//assert(found == NULL);
	//found = trade;
	return true;
}

void Trade::sendTradeList(UInt16 index, UInt16 num)
{
	Stream st(REP::TRADE_LIST);
	UInt16 sz = static_cast<UInt16>(_tradeDatas.size());
	UInt16 end = index + num;
	if (end > sz)
		end = sz;
	if (end < index)
		num = 0;
	else
		num = end - index;
	st << index << num << sz;
	if (num != 0)
	{
		TradeDataList::iterator offset, sentinel;
		offset = sentinel = _tradeDatas.begin();
		std::advance(offset, index);
		std::advance(sentinel, index+num);
		for (; offset!=_tradeDatas.end() && offset!=sentinel; ++offset)
		{
			st << offset->second->_id << offset->second->_tradePlayer->getName() << offset->second->_tradeTitle << SSTATUS2CSTATUS[offset->second->_tradeStatus] << offset->second->_tradeTime;
		}
	}
	st << Stream::eos;
	_owner->send(st);
}

bool Trade::sendTradeData(UInt32 id)
{
	if (sendTradeData(id, _owner))
	{
		TradeDataList::iterator found = _tradeDatas.find(id);
                if (found == _tradeDatas.end())
                    return false;
		//assert(found != _tradeDatas.end());
		TradeDataNotify notify;
		notify.id = id;
		notify.status = found->second->_tradeStatus;
		notify.receiver = _owner;
		GameMsgHdr hdr(0x294, found->second->_tradePlayer->getThreadId(), found->second->_tradePlayer, sizeof(TradeDataNotify));
		GLOBAL().PushMsg(hdr, &notify);
		return true;
	}
	return false;
}

//TODO : add trade status here
bool Trade::sendTradeData(UInt32 id, Player * receiver, UInt8 status)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return false;
	TradeData * trade = found->second;
	if (status == 0xFF)
		status = trade->_tradeStatus;
	if(status == TRADE_NONE || status >= TRADE_STATUS(TRADE_CONFIRMED))
		return false;
	Stream st(REP::TRADE_DATA);
	st << id << trade->_tradeSide << SSTATUS2CSTATUS[status] << trade->_coin << trade->_gold << static_cast<UInt16>(trade->_items.size());
	Package * package = _owner->GetPackage();
	for (UInt16 i = 0; i < trade->_items.size(); ++i)
	{
		ItemBase * item = trade->_items[i];
		if (IsEquip(item->getClass()))
		{
			ItemEquip * equip = static_cast<ItemEquip *>(item);
			package->AppendEquipData(st, equip);
		}
		else
		{
			package->AppendItemData(st, item);
		}
	}
	st << Stream::eos;
	receiver->send(st);
	return true;
}

bool Trade::launchTrade(std::string& name, const std::string& title, UInt32 coin, UInt32 gold, std::vector<TradeItemChange>& items)
{
	if (title.empty() || name == _owner->getName() || title.length() > 30)
		return false;
	if (coin == 0 && gold == 0 && items.empty())
		return false;
	Player * tradePlayer = globalNamedPlayers[_owner->fixName(name)];
	if (tradePlayer == NULL)
	{
		_owner->sendMsgCode(1, 1150);
		return false;
	}
	if(_owner->hasBlock(tradePlayer))
	{
		_owner->sendMsgCode(1, 1500);
		return false;
	}
	if(tradePlayer->hasBlock(_owner))
	{
		_owner->sendMsgCode(1, 1502);
		return false;
	}
	if (_owner->getCoin() < coin || _owner->getGold() < gold)
		return false;
	std::map<UInt32, UInt16> itemNums;
	std::vector<ItemBase*> packItems;
	packItems.resize(items.size());
	Package* package = _owner->GetPackage();
	for (UInt16 i = 0; i < items.size(); ++i)
	{
		UInt32 id = items[i]._itemId;
		if (IsEquipId(id))
		{
			packItems[i] = package->GetEquip(id);
			itemNums[id] ++;
		}
		else
		{
			packItems[i] = package->GetItem(id);
			itemNums[id] += items[i]._itemNum;
		}
		if (packItems[i] == NULL || packItems[i]->GetBindStatus() || packItems[i]->Count() < itemNums[id])
			return false;
	}
	TradeData * trade = new(std::nothrow) TradeData(IDGenerator::gTradeOidGenerator.ID());
	if (trade == NULL)
		return false;
	trade->_tradeSide = 0;
	trade->_tradeStatus = static_cast<UInt8>(TRADE_LAUNCH);
	trade->_tradeTitle = title;
	trade->_tradeTime = TimeUtil::Now();
	trade->_tradePlayer = tradePlayer;
	trade->_coin = coin;
	trade->_gold = gold;
	_owner->useCoin(coin);
	_owner->useGold(gold);
	for (UInt16 i = 0; i < packItems.size() ; ++i)
	{
		if (IsEquipId(packItems[i]->getId()))
		{
			if(!package->MoveEquipFromPack(static_cast<ItemEquip*>(packItems[i])))
				return false;
			trade->_items.push_back(packItems[i]);
		}
		else
		{
			if(!package->DelItem2(packItems[i], items[i]._itemNum))
				return false;
			trade->_items.push_back(Package::BuildItem(items[i]._itemId, items[i]._itemNum));
		}
	}
	buildTradeItems(items, trade->_strItems);

	_tradeDatas[trade->_id] = trade;

	char escstrTitle[256]; 
	mysql_escape_string(escstrTitle, trade->_tradeTitle.c_str(), trade->_tradeTitle.length());

	DB().PushUpdateData("INSERT INTO `trade` VALUES (%d, %"I64_FMT"u, %d, %d, %"I64_FMT"u, '%s', %d, %d, %d, '%s')", trade->_id, _owner->getId(), trade->_tradeSide, \
		trade->_tradeStatus, tradePlayer->getId(), escstrTitle, trade->_tradeTime, coin, gold, trade->_strItems.c_str());
/*
	TRACE_LOG("LaunchTrade : id[%d] owner[%"I64_FMT"u : %s] tradeSide[%d] trader[%"I64_FMT"u : %s] tradeTitle[%s] tradeTime[%u] coin[%d] gold[%d] items[%s]", \
		trade->_id, _owner->getId(), _owner->getName().c_str(), trade->_tradeSide, tradePlayer->getId(), tradePlayer->getName().c_str(),  trade->_tradeTitle.c_str(), trade->_tradeTime, coin, gold, trade->_strItems.c_str());
*/

	launchTradeNotify(trade->_id, tradePlayer->getName(), trade->_tradeTitle, trade->_tradeStatus, trade->_tradeTime);
	addTradeTimeoutNotify(trade->_id, trade->_tradeTime);

	LaunchTradeNotify notify;
	notify.id = trade->_id;
	notify.launcher = _owner;
	UInt16 len = title.length() > 48 ? 48 : title.length();
	memcpy(notify.title, title.c_str(), len);
	notify.time = trade->_tradeTime;
	GameMsgHdr hdr2(0x291, tradePlayer->getThreadId(), tradePlayer, sizeof(LaunchTradeNotify));
	GLOBAL().PushMsg(hdr2, &notify);

	return true;
}

bool Trade::replyTrade(UInt32 id, UInt32 coin, UInt32 gold, std::vector<TradeItemChange>& items)
{
	if (coin == 0 && gold == 0 && items.empty())
		return false;
	if (_owner->getCoin() < coin || _owner->getGold() < gold)
		return false;
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return false;
	TradeData* trade = found->second;
	if (trade->_tradeStatus != static_cast<UInt8>(TRADE_RECEIVE))
		return false;
	std::map<UInt32, UInt16> itemNums;
	std::vector<ItemBase*> packItems;
	packItems.resize(items.size());
	Package* package = _owner->GetPackage();
	for (UInt16 i = 0; i < items.size(); ++i)
	{
		UInt32 id = items[i]._itemId;
		if (IsEquipId(id))
		{
			packItems[i] = package->GetEquip(id);
			itemNums[id] ++;
		}
		else
		{
			packItems[i] = package->GetItem(id);
			itemNums[id] += items[i]._itemNum;
		}
		if (packItems[i] == NULL || packItems[i]->GetBindStatus() || packItems[i]->Count() < itemNums[id])
			return false;
	}
	trade->_coin = coin;
	trade->_gold = gold;
	_owner->useCoin(coin);
	_owner->useGold(gold);
	buildTradeItems(items, trade->_strItems);
	trade->_tradeStatus = static_cast<UInt8>(TRADE_REPLY);
	for (UInt16 i = 0; i < packItems.size() ; ++i)
	{
		if (IsEquipId(packItems[i]->getId()))
		{
			trade->_items.push_back(packItems[i]);
			package->MoveEquipFromPack(static_cast<ItemEquip*>(packItems[i]));
		}
		else
		{
			package->DelItem2(packItems[i], items[i]._itemNum);
			trade->_items.push_back(Package::BuildItem(items[i]._itemId, items[i]._itemNum));
		}
	}
	DB().PushUpdateData("UPDATE `trade` SET `tradeStatus` = %d, `coin` = %u, `gold` = %u, `items` = '%s' WHERE `tradeId` = %u AND `ownerId` = %"I64_FMT"u", trade->_tradeStatus, coin, gold, trade->_strItems.c_str(), trade->_id, _owner->getId());
/*
	TRACE_LOG("LaunchTrade : id[%d] owner[%"I64_FMT"u : %s] tradeSide[%d] trader[%"I64_FMT"u : %s] tradeTitle[%s] tradeTime[%u] coin[%d] gold[%d] items[%s]", \
		trade->_id, _owner->getId(), _owner->getName().c_str(), trade->_tradeSide, trade->_tradePlayer->getId(), trade->_tradePlayer->getName().c_str(),  trade->_tradeTitle.c_str(), trade->_tradeTime, coin, gold, trade->_strItems.c_str());
*/
	operateTradeNotify(trade->_id, trade->_tradeStatus);

	//»Ø¸´Í¨Öª
	OperateTradeNotify notify;
	notify.id = trade->_id;
	notify.status = static_cast<UInt8>(TRADE_CONFIRM);
	GameMsgHdr hdr(0x292, trade->_tradePlayer->getThreadId(), trade->_tradePlayer, sizeof(OperateTradeNotify));
	GLOBAL().PushMsg(hdr, &notify);

	return true;
}


void Trade::confirmTrade(UInt32 id)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return;
	TradeData* trade = found->second;
	if (trade->_tradeStatus != static_cast<UInt8>(TRADE_CONFIRM))
		return;
	trade->_tradeStatus = static_cast<UInt8>(TRADE_CONFIRMED);

	//½«ÎïÆ·ÍÆËÍ³öÈ¥£¬µ«²»½«ÎïÆ·´ÓÖÐÉ¾³ý£®Ö±µ½ÊÕµ½¶Ô·½µÄÏìÓ¦²ÅÉ¾³ý£®
	ConfirmTradeNotify notify;
	notify.id = id;
	notify.launcher = _owner;
	notify.coin = trade->_coin;
	notify.gold = trade->_gold;


	for (UInt16 i = 0; i < trade->_items.size(); ++i)
	{
	    notify.items[i] =  trade->_items[i];
	}
    GameMsgHdr hdr(0x293, trade->_tradePlayer->getThreadId(), trade->_tradePlayer, sizeof(ConfirmTradeNotify));
	GLOBAL().PushMsg(hdr, &notify);
}

void Trade::cancelTrade(UInt32 id)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return;
	TradeData* trade = found->second;
	if (trade->_tradeStatus == static_cast<UInt8>(TRADE_LAUNCH) || trade->_tradeStatus == static_cast<UInt8>(TRADE_RECEIVE) || trade->_tradeStatus == static_cast<UInt8>(TRADE_REPLY) || trade->_tradeStatus == static_cast<UInt8>(TRADE_CONFIRM))
	{
		//ÍË»¹ÎïÆ·¼°Í­Ç®,Ôª±¦
		_owner->getCoin(trade->_coin);
		_owner->getGold(trade->_gold);
		trade->_coin = trade->_gold = 0;
		if (!appendTradeItems(trade->_items))
		{
			trade->_tradeStatus = static_cast<UInt8>(TRADE_FAILPACKFULL);
			sendTradeMail(id, trade->_tradeStatus);
		}
		else
		{
			trade->_items.clear();
			trade->_strItems.clear();
			trade->_tradeStatus = static_cast<UInt8>(trade->_tradeSide == 0 ? TRADE_SELFCANCEL : TRADE_PEERCANCEL);
		}
		DB().PushUpdateData("UPDATE `trade` SET `tradeStatus` = %d, `items` = '%s' WHERE `tradeId` = %d AND `ownerId` = %"I64_FMT"u", trade->_tradeStatus, trade->_strItems.c_str(), trade->_id, _owner->getId());

		operateTradeNotify(trade->_id, trade->_tradeStatus);
		delTradeTimeoutNotify(trade->_id, trade->_tradeTime);

		//È¡ÏûÍ¨Öª
		OperateTradeNotify notify;
		notify.id = trade->_id;
		notify.status = static_cast<UInt8>(trade->_tradeSide == 0 ? TRADE_PEERCANCEL : TRADE_SELFCANCEL);
		GameMsgHdr hdr(0x292, trade->_tradePlayer->getThreadId(), trade->_tradePlayer, sizeof(OperateTradeNotify));
		GLOBAL().PushMsg(hdr, &notify);
	}
}

void Trade::cancel2Trade(UInt32 id)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return;
	TradeData* trade = found->second;
	if (!needCheckTrade(trade->_tradeStatus))
		return;

	//ÍË»¹ÎïÆ·¼°Í­Ç®,Ôª±¦
	_owner->getCoin(trade->_coin);
	_owner->getGold(trade->_gold);
	trade->_coin = trade->_gold = 0;
	if (!appendTradeItems(trade->_items))
	{
		trade->_tradeStatus = static_cast<UInt8>(TRADE_FAILPACKFULL);
		sendTradeMail(id, trade->_tradeStatus);
	}
	else
	{
		trade->_items.clear();
		trade->_strItems.clear();
		trade->_tradeStatus = static_cast<UInt8>(TRADE_TIMEOUT);
	}
	operateTradeNotify(trade->_id, trade->_tradeStatus);
	DB().PushUpdateData("UPDATE `trade` SET `tradeStatus` = %d, `items` = '%s' WHERE `tradeId` = %d AND `ownerId` = %"I64_FMT"u", trade->_tradeStatus, trade->_strItems.c_str(), trade->_id, _owner->getId());
}

void Trade::deleteTrade(UInt32 id)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return;
	TradeData* trade = found->second;

	if(trade->_tradeStatus >= TRADE_SELFOK && trade->_tradeStatus <= TRADE_TIMEOUT)
	{		
		if(trade->_items.size()!= 0)
		{
			UInt16 itemNum = trade->_items.size();
			for (UInt16 i = 0; i < itemNum; ++i)
			{
				ItemBase *item = trade->_items[i];
				if (item == NULL)
					continue;
				if (IsEquipId(item->getId()))
				{
					DB3().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", item->getId());
					if(item->getQuality() >= 4)
						DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, _owner->getId(), item->GetItemType().getId(), item->getId(), ToDelete, TimeUtil::Now());
				}
				SAFE_DELETE(item);
			}
		}
		SAFE_DELETE(trade);
		_tradeDatas.erase(found);
		operateTradeNotify(id, TRADE_NONE);
		DB().PushUpdateData("DELETE FROM `trade` WHERE `tradeId` = %u AND `ownerId` = %"I64_FMT"u", id, _owner->getId());
	}
}

bool Trade::recvLaunchTrade(UInt32 id, const std::string& title, UInt32 time, Player* launcher)
{
	TradeData * trade = new(std::nothrow) TradeData(id);
	if (trade == NULL)
		return false;
	trade->_tradeSide = 1;
	trade->_tradeTime = time;
	trade->_tradeTitle = title;
	trade->_tradeStatus = static_cast<UInt8>(TRADE_RECEIVE);
	trade->_tradePlayer = launcher;
	_tradeDatas[trade->_id] = trade;
	
	char escstrTitle[1024]; 
	mysql_escape_string(escstrTitle, trade->_tradeTitle.c_str(), trade->_tradeTitle.length());


	DB().PushUpdateData("INSERT INTO `trade` VALUES (%d, %"I64_FMT"u, %d, %d,  %"I64_FMT"u, '%s', %d, %d, %d, '%s')", trade->_id, _owner->getId(), trade->_tradeSide, \
		trade->_tradeStatus, launcher->getId(), escstrTitle, trade->_tradeTime, 0, 0, "");
	launchTradeNotify(id, launcher->getName(), title, trade->_tradeStatus, time);
	addTradeTimeoutNotify(trade->_id, trade->_tradeTime);

	return true;
}

bool Trade::recvReplyTrade(UInt32 id)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return false;
	TradeData* trade = found->second;
	if (trade->_tradeStatus != static_cast<UInt8>(TRADE_LAUNCH))
		return false;
	trade->_tradeStatus = static_cast<UInt8>(TRADE_CONFIRM);

	DB().PushUpdateData("UPDATE `trade` SET `tradeStatus` = %d WHERE `tradeId` = %d AND `ownerId` = %"I64_FMT"u", trade->_tradeStatus, id, _owner->getId());
	operateTradeNotify(id, trade->_tradeStatus);

	return true;
}

TradeData* Trade::findTradeDataById(UInt32 id)
{
    TradeDataList::iterator found =_tradeDatas.find(id);
    if (found == _tradeDatas.end())
		return NULL;
    TradeData* trade = found->second;
    return trade;
}

bool Trade::recvConfirmTrade(UInt32 id, UInt32 coin, UInt32 gold, ItemBase** items, UInt16 itemNum)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return false;
	TradeData* trade = found->second;
	if (trade->_tradeStatus == static_cast<UInt8>(TRADE_REPLY))
	{
		//ÍÆËÍÎïÆ·µ½½»Ò×·½
		ConfirmTradeNotify notify;
		notify.id = id;
		notify.launcher = trade->_tradePlayer;
		notify.coin = trade->_coin;
		notify.gold = trade->_gold;
		for (UInt16 i = 0; i < trade->_items.size(); ++i)
			notify.items[i] =  trade->_items[i];
		GameMsgHdr hdr(0x293, trade->_tradePlayer->getThreadId(), trade->_tradePlayer, sizeof(ConfirmTradeNotify));
		GLOBAL().PushMsg(hdr, &notify);

		_owner->getCoin(coin);
		_owner->getGold(gold);
		buildTradeItems(items, itemNum, trade->_strItems);	//Èç¹û±³°üÒÑÂú,ÔòÏÂ´Î¼ÓÈë±³°üÖÐ
		//printf("Build items : %s\n", trade->_strItems.c_str());
		if (!appendTradeItems(items, itemNum))
		{
			trade->_tradeStatus = static_cast<UInt8>(TRADE_SUCCPACKFULL);
			replaceTradeItems(trade->_items, items, itemNum);
			sendTradeMail(id, trade->_tradeStatus);
		}
		else
		{
			trade->_items.clear();
			trade->_strItems.clear();
			trade->_tradeStatus = static_cast<UInt8>(TRADE_PEEROK);
			delTradeTimeoutNotify(trade->_id, trade->_tradeTime);
		}
		trade->_coin = trade->_gold = 0;
		operateTradeNotify(id, trade->_tradeStatus);
		DB().PushUpdateData("UPDATE `trade` SET `tradeStatus` = %d, `coin` = %u, `gold` = %u, `items` = '%s' WHERE `tradeId` = %d AND `ownerId` = %"I64_FMT"u", trade->_tradeStatus, coin, gold, trade->_strItems.c_str(), trade->_id, _owner->getId());
		return true;
	}
	else if (trade->_tradeStatus == static_cast<UInt8>(TRADE_CONFIRMED))
	{
		_owner->getCoin(coin);
		_owner->getGold(gold);
		buildTradeItems(items, itemNum, trade->_strItems);
		//printf("Build items : %s\n", trade->_strItems.c_str());
		if (!appendTradeItems(items, itemNum))
		{
			trade->_tradeStatus = static_cast<UInt8>(TRADE_SUCCPACKFULL);
			replaceTradeItems(trade->_items, items, itemNum);
			sendTradeMail(id, trade->_tradeStatus);
		}
		else
		{
			trade->_items.clear();
			trade->_strItems.clear();
			trade->_tradeStatus = static_cast<UInt8>(TRADE_SELFOK);
			delTradeTimeoutNotify(trade->_id, trade->_tradeTime);
		}
		trade->_coin = trade->_gold = 0;
		operateTradeNotify(id, trade->_tradeStatus);
		DB().PushUpdateData("UPDATE `trade` SET `tradeStatus` = %d, `coin` = %u, `gold` = %u, `items` = '%s'  WHERE `tradeId` = %d AND `ownerId` = %"I64_FMT"u", trade->_tradeStatus, coin, gold, trade->_strItems.c_str(), trade->_id, _owner->getId());
		return true;
	}
	return false;
}

bool Trade::recvCancelTrade(UInt32 id)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return false;
	TradeData* trade = found->second;

	//ÍË»¹ÎïÆ·¼°Í­Ç®,Ôª±¦
	_owner->getCoin(trade->_coin);
	_owner->getGold(trade->_gold);
	trade->_coin = trade->_gold = 0;
	if (!appendTradeItems(trade->_items))
	{
		trade->_tradeStatus = static_cast<UInt8>(TRADE_FAILPACKFULL);
		sendTradeMail(id, trade->_tradeStatus);
	}
	else
	{
		trade->_items.clear();
		trade->_strItems.clear();
		trade->_tradeStatus = static_cast<UInt8>(trade->_tradeSide == 0 ? TRADE_SELFCANCEL : TRADE_PEERCANCEL);
	}
	operateTradeNotify(id, trade->_tradeStatus);
	DB().PushUpdateData("UPDATE `trade` SET `tradeStatus` = %d, `items` = '%s' WHERE `tradeId` = %d AND `ownerId` = %"I64_FMT"u", trade->_tradeStatus, trade->_strItems.c_str(), trade->_id, _owner->getId());

	return true;
}

void Trade::launchTradeNotify(UInt32 id, const std::string& tradeName, const std::string& title, UInt8 status, UInt32 time)
{
	Stream st(REP::TRADE_LAUNCH);
	st << id << tradeName << title << SSTATUS2CSTATUS[status] << time << Stream::eos;
	_owner->send(st);
}

void Trade::operateTradeNotify(UInt32 id, UInt8 status)
{
	Stream st(REP::TRADE_OPERATE);
	st << id << SSTATUS2CSTATUS[status] << Stream::eos;
	_owner->send(st);
}

void Trade::addTradeTimeoutNotify(UInt32 id, UInt32 time)
{
	TradeDataCheck check(id, time);
	GameMsgHdr hdr(0x1A1, WORKER_THREAD_WORLD, _owner, sizeof(TradeDataCheck));
	GLOBAL().PushMsg(hdr, &check);
}

void Trade::delTradeTimeoutNotify(UInt32 id, UInt32 time)
{
	TradeDataCheck check(id, time);
	GameMsgHdr hdr(0x1A2, WORKER_THREAD_WORLD, _owner, sizeof(TradeDataCheck));
	GLOBAL().PushMsg(hdr, &check);
}

void Trade::sendTradeMail(UInt32 id, UInt8 status)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return ;
	UInt16 msgId = (status == static_cast<UInt8>(TRADE_SUCCPACKFULL) ? 304 : 305);
	SYSMSG(title, msgId);
	SYSMSGV(content, 306, getTradeItemsNum(found->second->_items));
	MailPackage::MailItem mitem[12];
	UInt32 i = 0;
	for(; i < 12 && i < found->second->_items.size(); i ++)
	{
		mitem[i].id = found->second->_items[i]->GetItemType().getId();
		mitem[i].count = found->second->_items[i]->Count();
	}
	MailItemsInfo itemsInfo(mitem, TradeMail, i);
	_owner->GetMailBox()->newMail(_owner, 0x05, title, content, id, true, &itemsInfo);
}

bool Trade::addTradeMailItems(UInt32 id)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return false;
	TradeData* trade = found->second;
	if (trade->_items.empty()) return false;
	if (trade->_tradeStatus == static_cast<UInt8>(TRADE_SUCCPACKFULL) || trade->_tradeStatus == static_cast<UInt8>(TRADE_FAILPACKFULL))
	{
		UInt16 itemNum = getTradeItemsNum(trade->_items);
		if (appendTradeItems(trade->_items))
		{
			_owner->sendMsgCode(2, 1053, itemNum);
			trade->_items.clear();
			trade->_strItems.clear();
			if (trade->_tradeStatus == static_cast<UInt8>(TRADE_SUCCPACKFULL))
				trade->_tradeStatus = static_cast<UInt8>(TRADE_PACKFULLSUCC);
			else
				trade->_tradeStatus = static_cast<UInt8>(TRADE_PACKFULLFAIL);
			operateTradeNotify(id, trade->_tradeStatus);
			DB().PushUpdateData("UPDATE `trade` SET `tradeStatus` = %d, `items` = '%s'  WHERE `tradeId` = %d AND `ownerId` = %"I64_FMT"u", trade->_tradeStatus, trade->_strItems.c_str(), trade->_id, _owner->getId());
			return true;
		}
	}
	return false;
}

void Trade::makeMailInfo(UInt32 id, Stream& st, UInt16& num)
{
	TradeDataList::iterator found = _tradeDatas.find(id);
	if (found == _tradeDatas.end())
		return ;
	TradeData* trade = found->second;
	if (trade->_items.empty()) return ;
	if (trade->_tradeStatus == static_cast<UInt8>(TRADE_SUCCPACKFULL) || trade->_tradeStatus == static_cast<UInt8>(TRADE_FAILPACKFULL))
	{
		for(UInt16 i = 0; i < trade->_items.size(); i ++)
		{
			ItemBase *item = trade->_items[i];
			if(item == NULL)
				continue;
			num ++;
			st << static_cast<UInt16>(item->GetItemType().getId()) << item->Count();
		}
	}
	
}
bool Trade::needBuildTradeItems(UInt8 status)
{
    TRADE_STATUS s = static_cast<TRADE_STATUS>(status);
	return !(s == TRADE_NONE || s == TRADE_SELFOK ||	\
		 s == TRADE_PEEROK || s == TRADE_SELFCANCEL ||	\
		 s == TRADE_PEERCANCEL || s == TRADE_PACKFULLSUCC ||	\
		 s == TRADE_PACKFULLFAIL || s == TRADE_TIMEOUT);
}

bool Trade::needCheckTrade(UInt8 status)
{
	TRADE_STATUS s = static_cast<TRADE_STATUS>(status);
	return (s == TRADE_LAUNCH || s == TRADE_RECEIVE	||	\
		s == TRADE_REPLY || s == TRADE_CONFIRM	||	\
		s == TRADE_CONFIRMED);
}

void Trade::buildTradeItems(std::vector<TradeItemChange>& items, std::string& buildItems)
{
	buildItems.clear();
	for (std::size_t i = 0; i < items.size(); ++i)
	{
		if (IsEquipId(items[i]._itemId))
			buildItems.append(Itoa<UInt32>(items[i]._itemId));
		else
		{
			buildItems.append(Itoa<UInt32>(items[i]._itemId));
			buildItems.append(",");
			buildItems.append(Itoa<UInt16>(items[i]._itemNum));
		}
		if (i+1 != items.size())
			buildItems.append("|");
	}
}

void Trade::buildTradeItems(ItemBase** items, UInt16 itemNum, std::string& buildItems)
{
	buildItems.clear();
	bool firstPos = false;
	for (UInt16 i = 0; i < itemNum; ++i)
	{
		if (items[i] == NULL)
			continue;
		if (!firstPos)
			firstPos = true;
		else
			buildItems.append("|");
		UInt32 id = items[i]->getId();
		UInt32 num = items[i]->Count();
		if (IsEquipId(items[i]->getId()))
			buildItems.append(Itoa<UInt32>(id));
		else
		{
			buildItems.append(Itoa<UInt32>(id));
			buildItems.append(",");
			buildItems.append(Itoa<UInt16>(num));
		}
	}
}

bool Trade::appendTradeItems(std::vector<ItemBase*>& items)
{
	if (items.empty()) return true;
	return appendTradeItems(&items[0], items.size());
}

bool Trade::appendTradeItems(ItemBase** items, UInt16 itemNum)
{
	if (itemNum == 0)
		return true;
	UInt16 grids = 0;
	Package* package = _owner->GetPackage();
	for (UInt16 i = 0; i < itemNum; ++i)
	{
		if (items[i] == NULL)
			continue;
		if (IsEquipId(items[i]->getId()))
			++grids;
		else
			grids += package->GetItemUsedGrids(items[i]->getId(), items[i]->Count());
	}
	if (grids > package->GetRestPackageSize())
	{
		_owner->sendMsgCode(2, 1015);
		return false;
	}
	for (UInt16 i = 0; i < itemNum; ++i)
	{
		if (items[i] == NULL)
			continue;
		if (IsEquipId(items[i]->getId()))
			package->AddEquip2(static_cast<ItemEquip*>(items[i]));
		else
		{
			ItemBase* item = package->AddItem2(items[i]);
			if (item != NULL)
				SAFE_DELETE(item);
		}
		items[i] = NULL;
	}
	return true;
}

void Trade::replaceTradeItems(std::vector<ItemBase *>& itemVec, ItemBase** items, UInt16 itemNum)
{
	itemVec.clear();
	for (UInt16 i = 0; i < itemNum; ++i)
	{
		if (items[i] != NULL)
			itemVec.push_back(items[i]);
	}
}

UInt16 Trade::getTradeItemsNum(std::vector<ItemBase *>& items)
{
	UInt16 num = 0;
	for (UInt16 i = 0; i < items.size(); ++i)
	{
		if (items[i] == NULL)
			continue;
		num += items[i]->Count();
	}
	return num;
}

}

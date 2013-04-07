#include "Config.h"
#include "Sale.h"
#include "Player.h"
#include "Package.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Mail.h"
#include "MsgID.h"
#include "DCLogger.h"
#include "GObject/OpenAPIWorker.h"
#include "LBNameTmpl.h"
#include "SaleMgr.h" 


namespace GObject
{

Sale::Sale(Player * player) : _owner(player)
{

}

Sale::~Sale()
{
	std::map<UInt32, SaleSellRespData *>::iterator it;
	for (it = _sellItems.begin(); it != _sellItems.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	_sellItems.clear();
	std::map<UInt32, SaleMailItem *>::iterator it1;
	for (it1 = _mailItems.begin(); it1 != _mailItems.end(); ++it1)
	{
		SAFE_DELETE(it1->second->item);
		SAFE_DELETE(it1->second);
	}
	_mailItems.clear();
}

void Sale::Init()
{
	SaleMailItem * mailItem = NULL;
	std::map<UInt32, SaleMailItem *>::iterator offset;
	for (offset = _mailItems.begin(); offset != _mailItems.end(); ++offset)
	{
		mailItem = offset->second;
		if (!mailItem->mailSend)
		{
			DB4().PushUpdateData("UPDATE `sale` SET `status` = %d WHERE `saleId` = %u", static_cast<UInt8>(SALE_TIMEOUT), offset->first);
			mailItem->mailSend = true;
			SYSMSG(title, 309);
			SYSMSG(content, 311);
			MailPackage::MailItem mitem[1] = {{static_cast<UInt16>(mailItem->item->getId()), static_cast<UInt32>(mailItem->item->Count())}};
			MailItemsInfo itemsInfo(mitem, SaleTimeOut, 1);
			_owner->GetMailBox()->newMail(_owner, 0x06, title, content, offset->first, true, &itemsInfo);
		}
	}
	_accessItems.clear();
}

void Sale::UnInit()
{
	_accessItems.clear();
}

bool Sale::addSaleFromDB(SaleSellRespData * sale)
{
	_sellItems[sale->id] = sale;
	return true;
}

bool Sale::addSaleMailFromDB(UInt32 id, ItemBase * item, bool mailSend)
{
	SaleMailItem * mailItem = new SaleMailItem(item, mailSend);
	_mailItems[id] = mailItem;
	return true;
}

void Sale::sellSaleReq(std::vector<SaleSellData>& sales)
{
#ifndef _WIN32
#ifndef _FB
#ifndef _VT
#ifdef  OPEN_API_ON
    OPENAPI().Push(_owner->getId(), 1002, _owner->getOpenId(), _owner->getOpenKey(), _owner->getSource().c_str(), _owner->getClientIp());
#endif
#endif
#endif
#endif // _WIN32
	std::size_t sz = sales.size();
	UInt16 ItemCount[9] = {0};
	if(sz == 0)
		return;
    UInt8 vipLvl = _owner->getVipLevel();
    UInt32 revTael =  (vipLvl < 2) ? 200 : 100;
    UInt8 maxItems = 12 + 6*(vipLvl > 2 ? 2 : vipLvl);
	if (sz <= 9 && sz + _sellItems.size() <= maxItems)
	{
		UInt32 revenue = 0;
		std::vector<ItemBase *> saleItems;
		saleItems.resize(sz);
		Package * package = _owner->GetPackage();
		for (std::size_t i = 0; i < sz; ++i)
		{
			if (IsEquipId(sales[i].id))
			{
				saleItems[i] = package->GetEquip(sales[i].id);
				if(saleItems[i] == NULL || saleItems[i]->GetBindStatus())
				{
					saleItems[i] = NULL;
					continue;
				}
                if(saleItems[i]->getClass() == Item_LBling || saleItems[i]->getClass() == Item_LBwu || saleItems[i]->getClass() == Item_LBxin)
                {
                    if(!static_cast<ItemLingbao*>(saleItems[i])->isTongling())
                    {
                        saleItems[i] = NULL;
                        continue;
                    }
                }
			}
			else
			{
				saleItems[i] = package->GetItem(sales[i].id);
				if (saleItems[i] == NULL || saleItems[i]->Count() < sales[i].count)
				{
					saleItems[i] = NULL;
					continue;
				}
			}
			for(size_t j = 0; j < i; ++ j)
			{
				if(saleItems[j] == saleItems[i])
				{
					ItemCount[j] += sales[i].count;
					if(ItemCount[j] > saleItems[i]->Count())
						return ;
					break;
				}

			}
			ItemCount[i] = sales[i].count;

            if(sales[i].count == 0)
                return;
            if(sales[i].price == 0)
                return;

			if (sales[i].priceType == 0)
                return;
				//revenue += 1;
			else
				revenue += revTael;
		}
		if (revenue > _owner->getTael())
        {
            _owner->sendMsgCode(0, 1602);
			return;
        }
		_owner->useTael(revenue);
		SalePut salePuts[9];
		for (std::size_t j = 0; j < sz; ++j)
		{
			if(saleItems[j] == NULL)
				continue;
			if (IsEquipId(saleItems[j]->getId()))
				package->MoveEquipFromPack(static_cast<ItemEquip *>(saleItems[j]));
			else
			{
				package->DelItem2(saleItems[j], sales[j].count);
				saleItems[j] = Package::BuildItem(sales[j].id, sales[j].count);
			}
			salePuts[j].item = saleItems[j];
			salePuts[j].priceType = sales[j].priceType;
			salePuts[j].price = sales[j].price > 99999 ? 99999 : sales[j].price;
		}
		GameMsgHdr hdr(0x1B2, WORKER_THREAD_WORLD, _owner, sizeof(salePuts));
		GLOBAL().PushMsg(hdr, &salePuts);
	}
	else
	{
		_owner->sendMsgCode(0, 1601);
		Stream st(REP::SALE_SELL);
		st << static_cast<UInt8>(0) << static_cast<UInt8>(1) << Stream::eos;
		_owner->send(st);
	}
}

void Sale::sellSaleReqNotify(SaleSellRespData * data, UInt8 count)
{
	UInt8 i = 0;
	for (; i < count; ++i)
	{
		if (data[i].id == 0)
			break;
		SaleSellRespData * saleSellRespData = new SaleSellRespData();
		saleSellRespData->id = data[i].id;
		saleSellRespData->priceType = data[i].priceType;
		saleSellRespData->price = data[i].price;
        UInt32 size = sizeof(saleSellRespData->itemName) - 1;
		memcpy(saleSellRespData->itemName, data[i].itemName, size);
		_sellItems[data[i].id] = saleSellRespData;
	}

	if (i != 0)
	{
		_owner->sendMsgCode(0, 1600);
		Stream st(REP::SALE_SELL);
		st << static_cast<UInt8>(0) << static_cast<UInt8>(0) << Stream::eos;
		_owner->send(st);
	}
}

void Sale::searchMySale(SaleSearchReq& req)
{
	SaleSearchResp saleSearchResp;
	saleSearchResp.founder = req.founder;
	saleSearchResp.start = req.start;
	saleSearchResp.count = req.count;
	UInt16 i = 0;
	std::map<UInt32, SaleSellRespData *>::iterator it = _sellItems.begin();
	for (; i < 24 && it != _sellItems.end(); ++i, ++it)
	{
		saleSearchResp.ids[i] = it->first;
	}
	GameMsgHdr hdr(0x1B1, WORKER_THREAD_WORLD, _owner, sizeof(SaleSearchResp));
	GLOBAL().PushMsg(hdr, &saleSearchResp);
}


void Sale::checkSaleBuyPrice(SalePriceCheck& check)
{
	if (/*(check.priceType == 0 && _owner->holdCoin(check.price, 0)) || */(check.priceType == 1 && _owner->holdGold(check.price, 0)))
	{
		char buffer[8];
		*reinterpret_cast<UInt32 *>(buffer) = check.id;
		*reinterpret_cast<UInt32 *>(buffer+4) = check.pos;
		GameMsgHdr hdr(0x1B3, WORKER_THREAD_WORLD, _owner, sizeof(buffer));
		GLOBAL().PushMsg(hdr, buffer);
	}
}

void Sale::buySellResp(SaleItemBuy& saleItemBuy)
{
	if (saleItemBuy.item != NULL)
	{
        ConsumeInfo ci(PurchaseSale,saleItemBuy.item->getId(),saleItemBuy.item->Count());
#if 0
		if (saleItemBuy.priceType == 0)
			_owner->holdCoin(saleItemBuy.price, 1);
		else
#endif
			_owner->holdGold(saleItemBuy.price, 1, &ci);
		SaleMailItem* & mailItem = _mailItems[saleItemBuy.id];
		if (mailItem != NULL)
		{
			//ERROR_LOG
			SAFE_DELETE(mailItem->item);
			SAFE_DELETE(mailItem);
		}
		mailItem = new SaleMailItem(saleItemBuy.item, true);
		SYSMSG(title, 307);

        std::string name;
        ItemBase* item = saleItemBuy.item;
        if(item->getClass() == Item_LBling || item->getClass() == Item_LBwu || item->getClass() == Item_LBxin)
        {
            ItemLingbao* lb = static_cast<ItemLingbao*>(item);
            lbnameTmpl.getLBName(name, lb);
        }
        else
        {
            name = saleItemBuy.item->getName();
        }
		SYSMSGV(content, 308, name.c_str());
		MailPackage::MailItem mitem[1] = {{static_cast<UInt16>(saleItemBuy.item->getId()), static_cast<UInt32>(saleItemBuy.item->Count())}};
		MailItemsInfo itemsInfo(mitem, SaleBuy, 1);
		_owner->GetMailBox()->newMail(NULL, 0x06, title, content, saleItemBuy.id, true, &itemsInfo);
		Stream st(REP::SALE_SELL);
		st << static_cast<UInt8>(1) << static_cast<UInt8>(0) << Stream::eos;
		_owner->send(st);
	}
	else
	{
#if 0
		if (saleItemBuy.priceType == 0)
			_owner->holdCoin(saleItemBuy.price, 2);
		else
#endif
            _owner->holdGold(saleItemBuy.price, 2);
        _owner->sendMsgCode(0, 1055);
	}
}

void Sale::cancelSellResp(SaleItemCancel& saleItemCancel)
{
	std::map<UInt32, SaleSellRespData *>::iterator found = _sellItems.find(saleItemCancel.id);
	if (found == _sellItems.end())
	{
		//ERROR_LOG
		return;
	}
	SAFE_DELETE(found->second);
	_sellItems.erase(found);
	SaleMailItem* & mailItem = _mailItems[saleItemCancel.id];
	if (mailItem != NULL)
	{
		//ERROR_LOG
		SAFE_DELETE(mailItem->item);
		SAFE_DELETE(mailItem);
	}
	mailItem = new SaleMailItem(saleItemCancel.item, true);
	if (saleItemCancel.status == 2)
	{
		//È¡Ïû
		SYSMSG(title, 309);
        std::string name;
        ItemBase* item = saleItemCancel.item;
        if(item->getClass() == Item_LBling || item->getClass() == Item_LBwu || item->getClass() == Item_LBxin)
        {
            ItemLingbao* lb = static_cast<ItemLingbao*>(item);
            lbnameTmpl.getLBName(name, lb);
        }
        else
        {
            name = saleItemCancel.item->getName();
        }
		SYSMSGV(content, 310, name.c_str());
		MailPackage::MailItem mitem[1] = {{static_cast<UInt16>(mailItem->item->GetItemType().getId()), static_cast<UInt32>(mailItem->item->Count())}};
		MailItemsInfo itemsInfo(mitem, SaleCancel, 1);
		_owner->GetMailBox()->newMail(_owner, 0x06, title, content, saleItemCancel.id, true, &itemsInfo);
		Stream st(REP::SALE_SELL);
		st << static_cast<UInt8>(2) << static_cast<UInt8>(0) << Stream::eos;
		_owner->send(st);
	}
	else if (saleItemCancel.status == 3)
	{
		//³¬Ê±
		SYSMSG(title, 309);
		SYSMSG(content, 311);
		MailPackage::MailItem mitem[1] = {{static_cast<UInt16>(mailItem->item->getId()), static_cast<UInt32>(mailItem->item->Count())}};
		MailItemsInfo itemsInfo(mitem, SaleTimeOut, 1);
		_owner->GetMailBox()->newMail(_owner, 0x06, title, content, saleItemCancel.id, true, &itemsInfo);
	}
}

void Sale::sellSaleResp(UInt32 id, Player *buyer, UInt32 itemId, UInt16 itemNum)
{
	std::map<UInt32, SaleSellRespData *>::iterator found = _sellItems.find(id);
	if (found != _sellItems.end())
	{
		SaleSellRespData * saleSellRespData = found->second;
		SYSMSG(title, 312);
		if (saleSellRespData->priceType == 0)
		{
			//_owner->getCoin(saleSellRespData->price);
			//SYSMSGV(content, 313, saleSellRespData->itemName, buyer->getName().c_str(), saleSellRespData->price);
			//_owner->GetMailBox()->newMail(_owner, 0x07, title, content);
		}
		else
		{
            dclogger.trade_sec(_owner, buyer, itemId, itemNum, saleSellRespData->price);
            IncommingInfo ii(InFromSale, itemId, itemNum);
			_owner->getGold(saleSellRespData->price, &ii);
            _owner->tradeUdpLog(1081, 0, saleSellRespData->price);
            _owner->tradeUdpLog(1081, itemId, itemNum);
            _owner->tradeUdpLog(1081, itemId, saleSellRespData->price, true); 
			SYSMSGV(content, 314, saleSellRespData->itemName, buyer->getNameNoSuffix(buyer->getName()), saleSellRespData->price);
            MailItemsInfo itemsInfo(NULL, SaleSell, 0);
			_owner->GetMailBox()->newMail(_owner, 0x07, title, content, 0, true, &itemsInfo);
#ifndef _WIN32
#ifndef _FB
#ifndef _VT
#ifdef  OPEN_API_ON
            OPENAPI().Push(buyer->getId(), 1002, buyer->getOpenId(), buyer->getOpenKey(), buyer->getSource().c_str(), buyer->getClientIp());
#endif
#endif
#endif
#endif // _WIN32
		}
		SAFE_DELETE(saleSellRespData);
		_sellItems.erase(found);
	}
}

bool Sale::addSaleMailItems(UInt32 id)
{
	std::map<UInt32, SaleMailItem *>::iterator found = _mailItems.find(id);
	if (found == _mailItems.end() || found->second == NULL)
		return false;
	if (found->second == NULL)
	{
		_mailItems.erase(found);
		return false;
	}
	ItemBase * item = found->second->item;
	Package * package = _owner->GetPackage();
	UInt16 usedGrids = IsEquipId(item->getId()) ? 1 :  package->GetItemUsedGrids(item->getId(), item->Count());
	if (usedGrids > package->GetRestPackageSize())
	{
		_owner->sendMsgCode(2, 1015);
		return false;
	}
	if (IsEquipId(item->getId()))
		package->AddEquip2(static_cast<ItemEquip *>(item));
	else
	{
		item = package->AddItem2(item);
		if (item != NULL)
			SAFE_DELETE(item);
	}
	_mailItems.erase(found);
	DB4().PushUpdateData("UPDATE `sale` SET `status` = %d WHERE `saleId` = %u", static_cast<UInt8>(SALE_DELETED), id);
	return true;
}

void Sale::delSaleMailItems(UInt32 id)
{
	std::map<UInt32, SaleMailItem *>::iterator found = _mailItems.find(id);
	if (found == _mailItems.end() || found->second == NULL)
		return;
	DB4().PushUpdateData("UPDATE `sale` SET `status` = %d WHERE `saleId` = %u", static_cast<UInt8>(SALE_DELETED), id);
	if (found->second == NULL)
	{
		_mailItems.erase(found);
		return;
	}
	ItemBase * item = found->second->item;
	if (item == NULL)
	{
		_mailItems.erase(found);
		return;
	}
	if (IsEquipId(item->getId()))
	{
		DB4().PushUpdateData("DELETE FROM `equipment` WHERE `id` = %u", item->getId());
		if(item->getQuality() >= 4)
			DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, _owner->getId(), item->GetItemType().getId(), item->getId(), ToDeleteMail, TimeUtil::Now());
	}
	SAFE_DELETE(item);
	SAFE_DELETE(found->second);
	_mailItems.erase(found);
}

void Sale::makeMailInfo(UInt32 id, Stream& st, UInt16& num)
{
	std::map<UInt32, SaleMailItem *>::iterator found = _mailItems.find(id);
	if (found == _mailItems.end() || found->second == NULL)
		return ;
	if (found->second == NULL)
	{
		_mailItems.erase(found);
		return ;
	}
	ItemBase * item = found->second->item;
    st << static_cast<UInt16>(item->GetItemType().getId()) << item->Count();

    if(item->getClass() == Item_LBling || item->getClass() == Item_LBwu || item->getClass() == Item_LBxin)
    {
        ItemLingbaoAttr& lba = (static_cast<ItemLingbao*>(item))->getLingbaoAttr();
        lba.appendAttrToStream(st);
    }

	num = 1;
}
void Sale::cancleAllItem()
{
	SaleCancelNotify saleCancelNotify;
    SaleItemCancel item;
	UInt16 i = 0;
	std::map<UInt32, SaleSellRespData *>::iterator it = _sellItems.begin();
	for (; i < 24 && it != _sellItems.end(); ++i, ++it)
    {
        saleCancelNotify.ids[i] = it->first;
    }

    saleCancelNotify.count = i;
	GameMsgHdr hdr(0x1B4, WORKER_THREAD_WORLD, _owner, sizeof(SaleCancelNotify));
	GLOBAL().PushMsg(hdr, &saleCancelNotify);
}

}

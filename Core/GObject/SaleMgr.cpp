#include "Config.h"
#include "SaleMgr.h"
#include "Item.h"
#include "Player.h"
#include "Package.h"
#include "Server/OidGenerator.h"
#include "Server/Cfg.h"
#include "GData/GDataManager.h"
#include "MsgHandler/MsgTypes.h"
#include "Common/Itoa.h"
#include "MsgID.h"

namespace GObject
{

SaleMgr gSaleMgr;
static const UInt8 ReqCvt[] = { 0xFF, 1, 2, 3, 12, 16, 20, 33, 50, 59};

SaleMgr::SaleMgr()
{
    _on_off = 1;
}

SaleMgr::~SaleMgr()
{
	std::size_t size = _sales.size();
	for (std::size_t pos = 0; pos < size; ++pos)
	{
		SAFE_DELETE(_sales[pos]);
	}
}

struct SaleRowSort
{
	bool operator()(SaleData * sale1, SaleData * sale2)
	{
		if (sale1->_priceType != sale2->_priceType)
			return sale1->_priceType < sale2->_priceType;
		return (static_cast<float>(sale1->_price)/sale1->_item->Count()) < (static_cast<float>(sale2->_price)/sale2->_item->Count());
	}
}SaleRowSorter;

void SaleMgr::addRowSale(SaleData * sale)
{
	UInt8 subClass = sale->_item->getClass();
	UInt8 quality = sale->_item->getQuality();
	UInt32 typeId = sale->_item->GetItemType().getId();
    UInt8 career = sale->_item->GetCareer();

    UInt8 pIdx = 0;
    UInt8 stIdx = StatIndex(subClass, typeId, pIdx);
    if(stIdx > 61)
    {
        stIdx = 1;
        pIdx = 1;
    }
    if(career > 3)
        career = 0;
    if(quality > 6)
        quality = 1;

	std::vector<SaleData *>& sales = _saleRow[stIdx][typeId];
	sales.push_back(sale);
	std::sort(sales.begin(), sales.end(), SaleRowSorter);
	++ _itemStat[career][stIdx][quality];
	++ _itemStat[career][stIdx][0];
	++ _itemStat[career][0][quality];
	++ _itemStat[career][0][0];
    if(career != 0)
    {
        ++ _itemStat[0][stIdx][quality];
        ++ _itemStat[0][stIdx][0];
        ++ _itemStat[0][0][quality];
        ++ _itemStat[0][0][0];
    }

	if (pIdx != stIdx && pIdx != 0)
	{
		std::vector<SaleData *>& sales = _saleRow[pIdx][typeId];
		sales.push_back(sale);
		std::sort(sales.begin(), sales.end(), SaleRowSorter);
		++ _itemStat[career][pIdx][quality];
		++ _itemStat[career][pIdx][0];
        if(career != 0)
        {
            ++ _itemStat[0][pIdx][quality];
            ++ _itemStat[0][pIdx][0];
        }
	}
}

void SaleMgr::delRowSale(SaleData * sale)
{
	UInt8 subClass = sale->_item->getClass();
	UInt8 quality = sale->_item->getQuality();
	UInt32 typeId = sale->_item->GetItemType().getId();
    UInt8 career = sale->_item->GetCareer();

    UInt8 pIdx = 0;
    UInt8 stIdx = StatIndex(subClass, typeId, pIdx);
    if(stIdx > 61)
    {
        stIdx = 1;
        pIdx = 1;
    }
    if(career > 3)
        career = 0;
    if(quality > 6)
        quality = 1;

	std::vector<SaleData *>& sales = _saleRow[stIdx][typeId];
	std::vector<SaleData *>::iterator found = std::find(sales.begin(), sales.end(), sale);
	if (found != sales.end())
		sales.erase(found);

	UInt32& cnt1 = _itemStat[career][stIdx][quality];
	if (cnt1 > 0) -- cnt1;
	UInt32& cnt2 = _itemStat[career][stIdx][0];
	if (cnt2 > 0) -- cnt2;
	UInt32& cnt3 = _itemStat[career][0][quality];
	if (cnt3 > 0) -- cnt3;
	UInt32& cnt4 = _itemStat[career][0][0];
	if (cnt4 > 0) -- cnt4;
    if(career != 0)
    {
        UInt32& cnt1 = _itemStat[0][stIdx][quality];
        if (cnt1 > 0) -- cnt1;
        UInt32& cnt2 = _itemStat[0][stIdx][0];
        if (cnt2 > 0) -- cnt2;
        UInt32& cnt3 = _itemStat[0][0][quality];
        if (cnt3 > 0) -- cnt3;
        UInt32& cnt4 = _itemStat[0][0][0];
        if (cnt4 > 0) -- cnt4;
    }

	if (pIdx != stIdx && pIdx != 0)
	{
		std::vector<SaleData *>& sales = _saleRow[pIdx][typeId];
		std::vector<SaleData *>::iterator found = std::find(sales.begin(), sales.end(), sale);
		if (found != sales.end())
			sales.erase(found);
		UInt32& cnt5 = _itemStat[career][pIdx][quality];
		if (cnt5 > 0) -- cnt5;
		UInt32& cnt6 = _itemStat[career][pIdx][0];
		if (cnt6 > 0) -- cnt6;
        if(career != 0)
        {
            UInt32& cnt5 = _itemStat[0][pIdx][quality];
            if (cnt5 > 0) -- cnt5;
            UInt32& cnt6 = _itemStat[0][pIdx][0];
            if (cnt6 > 0) -- cnt6;
        }
	}
}

bool SaleMgr::addSaleFromDB(SaleData * sale)
{
	UInt32 pos = getNextIndex();
	if (pos == static_cast<UInt32>(-1))
	{
		_sales.push_back(sale);
		sale->_pos = _sales.size() - 1;
	}
	else
	{
		assert(_sales[pos] == NULL);
		_sales[pos] = sale;
		sale->_pos = pos;
	}

	addRowSale(sale);
    UInt8 idx = sale->_owner->getVipLevel();
    if(idx > 2)
        idx = 2;
	_saleCheck[idx].insert(std::make_pair(sale->_time, sale->_pos));
	_salePos[sale->_id] = sale->_pos;

	return true;
}

void SaleMgr::sellSale(Player * player, SalePut * salePuts, UInt8 count)
{
	UInt8 i = 0;
	SaleSellRespData saleSellRespDatas[9];
	UInt32 now = TimeUtil::Now();
	for (; i < count; ++i)
	{
		if (salePuts[i].item == NULL)
			continue;
		SaleData * sale = new SaleData();
		sale->_id = IDGenerator::gSaleOidGenerator.ID();
		sale->_time = now;
		sale->_priceType = salePuts[i].priceType;
		sale->_price = salePuts[i].price;
		sale->_item = salePuts[i].item;
		sale->_owner = player;
		sale->_status = static_cast<UInt8>(SALE_SELL);
		UInt32 pos = getNextIndex();
		if (pos == static_cast<UInt32>(-1))
		{
			_sales.push_back(sale);
			sale->_pos = _sales.size() - 1;
		}
		else
		{
			assert(_sales[pos] == NULL);
			_sales[pos] = sale;
			sale->_pos = pos;
		}
		addRowSale(sale);
		_salePos[sale->_id] = sale->_pos;
		saleSellRespDatas[i].id = sale->_id;
		saleSellRespDatas[i].priceType = sale->_priceType;
		saleSellRespDatas[i].price = sale->_price;
		const std::string& itemName = sale->_item->getName();
		memcpy(saleSellRespDatas[i].itemName, itemName.c_str(), std::min(sizeof(saleSellRespDatas[i].itemName)-1, itemName.length()));
		DB4().PushUpdateData("INSERT INTO `sale` VALUES (%u,  %"I64_FMT"u, %d, %u, %d, %u, %u, %u)", sale->_id, sale->_owner->getId(), sale->_status, sale->_time, sale->_priceType, sale->_price, sale->_item->getId(), sale->_item->Count());
        UInt8 idx = sale->_owner->getVipLevel();
        if(idx > 2)
            idx = 2;
		_saleCheck[idx].insert(std::make_pair(sale->_time, sale->_pos));
		std::string comboItemId;
		if(sale->_item->getId()>30000)
            comboItemId=Itoa(sale->_item->getId())+"|"+Itoa(sale->_item->GetItemType().getId());
        else
            comboItemId=Itoa(sale->_item->getId());
		DBLOG().PushUpdateData("insert into sales (server_id,sale_id,player_id,price_type,price,item,item_num,sale_time) values(%u,%u,%"I64_FMT"u,%u,%u,'%s',%u,%u)",
			cfg.serverLogId, sale->_id, sale->_owner->getId(), sale->_priceType, sale->_price, comboItemId.c_str(), sale->_item->Count(), sale->_time);
	}

	if (i != 0)
	{
		GameMsgHdr hdr(0x302, player->getThreadId(), player, sizeof(saleSellRespDatas));
		GLOBAL().PushMsg(hdr, &saleSellRespDatas);
	}
}

void SaleMgr::buySale(Player * player, UInt32 id)
{
	SalePosType::iterator found = _salePos.find(id);
	if (found == _salePos.end())
	{
		Stream st(REP::SALE_SELL);
		st << static_cast<UInt8>(1) << static_cast<UInt8>(1) << Stream::eos;
		player->send(st);
		player->sendMsgCode(0, 1055);
		return;
	}
	if (found->second >= _sales.size())
		return;
	SaleData * sale = _sales[found->second];
	if (sale == NULL)
		return;
	if (player == sale->_owner)
		return;

	SalePriceCheck salePriceCheck;
	salePriceCheck.priceType = sale->_priceType;
	salePriceCheck.price = sale->_price;
	salePriceCheck.id = id;
	salePriceCheck.pos = found->second;
	GameMsgHdr hdr(0x300, player->getThreadId(), player, sizeof(SalePriceCheck));
	GLOBAL().PushMsg(hdr, &salePriceCheck);
}

void SaleMgr::addSaleItem(Player * player, UInt32 id, UInt32 pos)
{
	SaleItemBuy saleItemBuy;
	if (pos < _sales.size())
	{
		SaleData * sale = NULL;
		sale = _sales[pos];
		if (sale != NULL && sale->_id == id)
		{
			_sales[pos] = NULL;
			setNextIndex(pos);
			
			delRowSale(sale);
			delSaleCheck(sale);
			_salePos.erase(sale->_id);

			DB4().PushUpdateData("UPDATE `sale` SET `ownerId` = %"I64_FMT"u, `status` = %d WHERE `saleId` = %u", player->getId(), static_cast<UInt8>(SALE_BUY), sale->_id);
			DBLOG().PushUpdateData("update sales set purchaser=%"I64_FMT"u,purchase_time=%u where sale_id=%u and server_id=%u", player->getId(), TimeUtil::Now(), sale->_id, cfg.serverLogId);
			if(IsEquipId(sale->_item->getId()))
			{
				if(sale->_item->getQuality() >= 4)
				{
					DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, player->getId(), sale->_item->GetItemType().getId(), sale->_item->getId(), FromSale, TimeUtil::Now());
					DBLOG().PushUpdateData("insert into `equip_courses`(`server_id`, `player_id`, `template_id`, `equip_id`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)", cfg.serverLogId, sale->_owner->getId(), sale->_item->GetItemType().getId(), sale->_item->getId(), ToSale, TimeUtil::Now());
				}
			}
			else
			{
				if(sale->_item->getQuality() >= 3)
				{
                    std::string tbn("item_courses");
                    DBLOG().GetMultiDBName(tbn);
					DBLOG().PushUpdateData("insert into `%s`(`server_id`, `player_id`, `item_id`, `item_num`, `from_to`, `happened_time`) values(%u, %"I64_FMT"u, %u, %u, %u, %u)",tbn.c_str(), cfg.serverLogId, player->getId(), sale->_item->getId(), sale->_item->Count(), FromSale, TimeUtil::Now());
				}
			}
			saleItemBuy.id = id;
			saleItemBuy.item = sale->_item;
			saleItemBuy.priceType = sale->_priceType;
			saleItemBuy.price = sale->_price;
		
			struct BuyInfo
			{
				UInt32 saleId;
				Player *buyer;
                UInt32 id;
                UInt16 num;
			};
			//给卖家添加金钱
			BuyInfo buyinfo = {id, player, sale->_item->getId(), sale->_item->Count()};
			GameMsgHdr hdr(0x303, sale->_owner->getThreadId(), sale->_owner, sizeof(buyinfo));
			GLOBAL().PushMsg(hdr, &buyinfo);

			SAFE_DELETE(sale);
		}
	}
	GameMsgHdr hdr(0x301, player->getThreadId(), player, sizeof(SaleItemBuy));
	GLOBAL().PushMsg(hdr, &saleItemBuy);
}

void SaleMgr::cancelSale(Player * player, UInt32 id)
{
	SalePosType::iterator found = _salePos.find(id);
	if (found == _salePos.end())
	{
		Stream st(REP::SALE_SELL);
		st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << Stream::eos;
		player->send(st);
		return;
	}
	if (found->second >= _sales.size())
		return;
	SaleData * sale = _sales[found->second];
	if (sale == NULL)
		return;
	if (sale->_owner != player)
		return;
	_sales[found->second] = NULL;
	setNextIndex(found->second);
	DB4().PushUpdateData("UPDATE `sale` SET `status` = %d WHERE `saleId` = %u", static_cast<UInt8>(SALE_CANCEL), id);
	DBLOG().PushUpdateData("update sales set is_cancel=1 where sale_id=%u and server_id=%u", id, cfg.serverLogId);
	SaleItemCancel saleItemCancel;
	saleItemCancel.status = static_cast<UInt8>(SALE_CANCEL);
	saleItemCancel.id = id;
	saleItemCancel.item = sale->_item;
	delSaleCheck(sale);
	delRowSale(sale);
	_salePos.erase(id);
	SAFE_DELETE(sale);
	GameMsgHdr hdr(0x304, player->getThreadId(), player, sizeof(SaleItemCancel));
	GLOBAL().PushMsg(hdr, &saleItemCancel);
}

void SaleMgr::requestSaleList(Player * player, UInt16 start, UInt16 count, std::string& name, UInt8 req, UInt8 color, UInt8 career, UInt8 eqType)
{
	//搜索
	if (!name.empty())
	{
		//if (sch == 0)
			searchSaleByItemName(player, name, start, count);
#if 0
		else
		{
			Player * beFounder = globalNamedPlayers[player->fixName(name)];
			searchPlayerSale(player, beFounder, start, count);
		}
#endif
		return;
	}
	if (req > 10 || career > 3 || color > 6 || eqType > 16)
		return;
	if (req == 10)
	{
		searchPlayerSale(player, player, start, count);
	}
	else
	{
		if (ReqCvt[req] != 0xFF)
		{
			//请求单个列表
			if (req > 2 && req < 10)
                req = ReqCvt[req] + eqType;
            else if(req < 3)
                req = ReqCvt[req];
            else
                req = ReqCvt[0];

			UInt16 sz = static_cast<UInt16>(saleRowStat(req, color, career));
			UInt16 end = start + count;
			if (end > sz)
				end = sz;
			if (end < start)
				count = 0;
			else
				count = end - start;
			UInt16 offset1, offset2;
			UInt16 readCount = 0;
			Stream st(REP::SALE_LIST);
			st << start << static_cast<UInt16>(0) << sz;
			if (shiftSingleSaleList(req, color, career, start, offset1, offset2))
				readCount = appendSingleSaleList(player, st, req, career, color, count, offset1, offset2);
			st.data<UInt16>(6) = readCount;
			st << Stream::eos;
			player->send(st);
		}
		else
		{
			//请求全部列表
			UInt16 sz = static_cast<UInt16>(saleRowStat(0, color, career));
			UInt16 end = start + count;
			if (end > sz)
				end = sz;
			if (end < start)
				count = 0;
			else
				count = end - start;
			UInt8 type;
			UInt16 filter1Offset, filter0Offset;
			UInt16 readCount = 0;
			Stream st(REP::SALE_LIST);
			st << start << static_cast<UInt16>(0) << sz;
			if (shiftTotalSaleList(color, career, start, type, filter1Offset, filter0Offset))
				readCount = appendTotalSaleList(player, st, type, career, color, count, filter1Offset, filter0Offset);
			st.data<UInt16>(6) = readCount;
			st << Stream::eos;
			player->send(st);
		}
	}
}

UInt16 SaleMgr::appendSingleSaleList(Player * player, Stream& st, UInt8 type, UInt8 career, UInt8 quality, UInt16 count, UInt16 offset1, UInt16 offset2)
{
	UInt16 readCount = 0;
	SaleRowType::iterator offsetIter1 = _saleRow[type].begin();
	std::advance(offsetIter1, offset1);
	for (; offsetIter1 != _saleRow[type].end(); ++ offsetIter1)
	{
		//if (sort == 0)
		{
			//升序
			std::vector<SaleData *>& sales = offsetIter1->second;
			for (UInt16 offsetIter2 = offset2; offsetIter2 < sales.size() && readCount < count; ++ offsetIter2)
			{
				SaleData * sale = sales[offsetIter2];
				if (sale == NULL)
					continue;
				if (quality != 0 && sale->_item->getQuality() != quality)
					continue;
                if(career != 0 && sale->_item->GetCareer() != career)
                    continue;

				st << sale->_id;
				if (player->GetSale()->hasAccessSaleItem(sale->_id))
					st << static_cast<UInt8>(1);
				else
				{
					st << static_cast<UInt8>(0) << sale->_owner->getName() << sale->_price << sale->_priceType;
					if (IsEquipId(sale->_item->getId()))
						Package::AppendEquipData(st, static_cast<ItemEquip *>(sale->_item));
					else
						Package::AppendItemData(st, sale->_item);
					player->GetSale()->addAccessSaleItem(sale->_id);
				}
				++readCount;
			}
			if (readCount >= count)
				break;
			offset2 = 0;
		}
	}

	return readCount;
}


UInt16 SaleMgr::appendTotalSaleList(Player * player, Stream& st, UInt8 type, UInt8 career, UInt8 quality, UInt16 count, UInt16 filter1Offset, UInt16 filter0Offset)
{
	UInt16 readCount = 0;
	for (; type <= 9; ++type)
	{
		readCount += appendSingleSaleList(player, st, ReqCvt[type], career, quality, count-readCount, filter1Offset, filter0Offset);
		if (readCount >= count)
			break;
		filter1Offset = filter0Offset = 0;
	}
	return readCount;
}

bool SaleMgr::shiftSingleSaleList(UInt8 type, UInt8 quality, UInt8 career, UInt16 start, UInt16& offset1, UInt16& offset2)
{
	if (start >= _itemStat[career][type][quality])
		return false;
	UInt16 first = 0, second = 0;
	UInt16 count = 0;
	offset1 = offset2 = 0;
	SaleRowType::iterator it = _saleRow[type].begin();
	for (; it != _saleRow[type].end(); ++ it, ++ offset1)
	{
		std::vector<SaleData *>& sales = it->second;
		if (sales.empty())
			continue; 
        if (quality != 0 && career != 0)
        {
			// 指定颜色,职业
			for (offset2 = 0; offset2 < sales.size(); ++ offset2)
			{
                if (sales[offset2]->_item->getQuality() == quality && sales[offset2]->_item->GetCareer() == career)
				{
					++ count;
					if (count > start)
						return true;
				}
			}
        }
        else if (quality != 0)
		{
			// 指定颜色
			for (offset2 = 0; offset2 < sales.size(); ++ offset2)
			{
				if (sales[offset2]->_item->getQuality() == quality)
				{
					++ count;
					if (count > start)
						return true;
				}
			}
		}
        else if(career != 0)
        {
			// 指定职业
			for (offset2 = 0; offset2 < sales.size(); ++ offset2)
			{
                if (sales[offset2]->_item->GetCareer() == career)
				{
					++ count;
					if (count > start)
						return true;
				}
			}
        }
		else
		{
			//任何颜色,职业
			second += sales.size();
			if (start >= first && start < second)
			{
				offset2 = start - first;
				return true;
			}
			else
				first += sales.size();
		}
	}
	return false;
}


bool SaleMgr::shiftSingleSaleList2(UInt8 type, UInt8 quality, UInt8 career, UInt16& offset, UInt16& offset1, UInt16& offset2)
{
	UInt16 first = 0, second = 0;
	UInt16 count = 0;
	offset1 = offset2 = 0;
	SaleRowType::iterator it = _saleRow[type].begin();
	for (; it != _saleRow[type].end(); ++ it, ++ offset1)
	{
		std::vector<SaleData *>& sales = it->second;
		if (sales.empty())
			continue; 
        if(quality != 0 && career != 0)
        {
			//指定颜色,职业
			for (offset2 = 0; offset2 < sales.size(); ++ offset2)
			{
                if (sales[offset2]->_item->getQuality() == quality && sales[offset2]->_item->GetCareer() == career)
                {
                    ++ count;
                    if (count > offset)
                        return true;
                }
			}
		}
        else if(quality != 0)
        {
			//指定颜色
			for (offset2 = 0; offset2 < sales.size(); ++ offset2)
			{
                if (sales[offset2]->_item->getQuality() == quality)
                {
                    ++ count;
                    if (count > offset)
                        return true;
                }
			}
        }
        else if(career != 0)
        {
			//指定职业
			for (offset2 = 0; offset2 < sales.size(); ++ offset2)
			{
                if (sales[offset2]->_item->GetCareer() == career)
                {
                    ++ count;
                    if (count > offset)
                        return true;
                }
			}
        }
		else
		{
			//任何颜色,职业
			second += sales.size();
			if (offset >= first && offset < second)
			{
				offset2 = offset - first;
				return true;
			}
			else
				first += sales.size();
		}
	}
	if (quality != 0)
		offset = offset - count;
	else
		offset = offset - first;
	return false;
}

bool SaleMgr::shiftTotalSaleList(UInt8 quality, UInt8 career, UInt16 offset, UInt8& type, UInt16& filter1Offset, UInt16& filter0Offset)
{
	if (offset > _itemStat[career][0][quality])
		return false;
	for (type = 1; type <= 9; ++type)
	{
		if (shiftSingleSaleList2(ReqCvt[type], quality, career, offset, filter1Offset, filter0Offset))
			return true;
	}
	return false;
}


void SaleMgr::searchSaleByItemName(Player * player, std::string& itemName, UInt16 start, UInt16 count)
{
	const GData::ItemBaseType * itemBaseType = Package::GetItemBaseType(itemName);
	if (itemBaseType == NULL)
	{
		Stream st(REP::SALE_LIST);
		st << static_cast<UInt16>(start) << static_cast<UInt16>(0) << static_cast<UInt16>(0) << Stream::eos;
		player->send(st);
		player->sendMsgCode(1, 1050);
		return;
	}
	UInt8 pos = Index(itemBaseType->subClass, itemBaseType->getId());
	std::vector<SaleData *>& sales = _saleRow[pos][itemBaseType->getId()];
	UInt16 sz = sales.size();
	UInt16 end = start + count;
	if (end > sz)
		end = sz;
	if (end < start)
		count = 0;
	else
		count = end - start;
	UInt16 realRead = 0;
	Stream st(REP::SALE_LIST);
	st << static_cast<UInt16>(start) << static_cast<UInt16>(0) << static_cast<UInt16>(sales.size());
	if (count != 0)
	{
		SaleData * sale = NULL;
		//if (sort == 0)
		{
			for (; start < sales.size() && realRead < count; ++ start)
			{
				sale = sales[start];
				if (sale == NULL)
					continue;
				st << sale->_id;
				if (player->GetSale()->hasAccessSaleItem(sale->_id))
					st << static_cast<UInt8>(1);
				else
				{
					st << static_cast<UInt8>(0) << sale->_owner->getName() << sale->_price << sale->_priceType;
					if (IsEquipId(sale->_item->getId()))
						Package::AppendEquipData(st, static_cast<ItemEquip *>(sale->_item));
					else
						Package::AppendItemData(st, sale->_item);
					player->GetSale()->addAccessSaleItem(sale->_id);
				}
				++realRead;
			}
		}
	}
	st.data<UInt16>(6) = realRead;
	st << Stream::eos;
	player->send(st);
	if (realRead == 0)
	{
		player->sendMsgCode(1, 1050);
	}
}

void SaleMgr::searchPlayerSale(Player * founder, Player * beFounder, UInt16 start, UInt16 count)
{
	if (beFounder == NULL)
	{
		Stream st(REP::SALE_LIST);
		st << static_cast<UInt16>(start) << static_cast<UInt16>(0) << static_cast<UInt16>(0) << Stream::eos;
		founder->send(st);
		founder->sendMsgCode(1, 1050);
		return;
	}
	SaleSearchReq saleSearchReq(start, count, founder);
	GameMsgHdr hdr(0x305, beFounder->getThreadId(), beFounder, sizeof(SaleSearchReq));
	GLOBAL().PushMsg(hdr, &saleSearchReq);
}

void SaleMgr::searchPlayerSaleResp(Player * founder, Player * beFounder, UInt16 start, UInt16 count, UInt32 * ids, UInt16 idsCount)
{
	if (founder == NULL || beFounder == NULL)
		return;
	std::vector<UInt32> vec;
	for (UInt16 i = 0; i < idsCount; ++i)
	{
		if (ids[i] == 0)
			break;
		vec.push_back(ids[i]);
	}
	UInt16 sz = vec.size();
	UInt16 end = start + count;
	if (end > sz)
		end = sz;
	if (end < start)
		count = 0;
	else
		count = end - start;
	UInt16 realRead = 0;
	Stream st(REP::SALE_LIST);
	st << static_cast<UInt16>(start) << static_cast<UInt16>(0) << sz;
	if (count != 0)
	{
		SaleData * sale = NULL;
		UInt16 maxSaleSz = _sales.size();
		SalePosType::iterator found;
		std::vector<UInt32>::iterator offset, sentinel;
		offset = sentinel = vec.begin();
		std::advance(offset, start);
		std::advance(sentinel, start+count);
		for (; offset != vec.end() && offset !=	sentinel; ++offset)
		{
			found = _salePos.find(*offset);
			if (found == _salePos.end())
				continue;
			if (found->second >= maxSaleSz)
				continue;
			sale = _sales[found->second];
			if (sale == NULL || sale->_id != *offset || beFounder != sale->_owner)
				continue;
			st << sale->_id;
			if (founder->GetSale()->hasAccessSaleItem(sale->_id))
				st << static_cast<UInt8>(1);
			else
			{
				st << static_cast<UInt8>(0) << sale->_owner->getName() << sale->_price << sale->_priceType;
				if (IsEquipId(sale->_item->getId()))
					Package::AppendEquipData(st, static_cast<ItemEquip *>(sale->_item));
				else
					Package::AppendItemData(st, sale->_item);
				founder->GetSale()->addAccessSaleItem(sale->_id);
			}
			++realRead;
		}
	}
	st.data<UInt16>(6) = realRead;
	st << Stream::eos;
	founder->send(st);
}

void SaleMgr::delSaleCheck(SaleData * sale)
{
	typedef std::pair<SaleCheckType::iterator, SaleCheckType::iterator> FoundType;
    UInt8 idx = sale->_owner->getVipLevel();
    if(idx > 2)
        idx = 2;
	FoundType found = _saleCheck[idx].equal_range(sale->_time);
	for (SaleCheckType::iterator it = found.first; it != found.second; ++it)
	{
		if (sale->_pos == it->second)
		{
			_saleCheck[idx].erase(it);
			break;
		}
	}
}


void SaleMgr::update(UInt32 curr)
{
	SaleData * sale = NULL;
	UInt32 sz = _sales.size();
    for(int i = 0; i < 3; ++i)
    {
        SaleCheckType::iterator itUp = _saleCheck[i].upper_bound(curr-(SALE_TIME_OUT*(i+1)));
        for (SaleCheckType::iterator it = _saleCheck[i].begin(); it != itUp;)
        {
            if (it->second < sz && _sales[it->second] != NULL)
            {
                sale = _sales[it->second];
                _sales[it->second] = NULL;
                setNextIndex(it->second);
                DB4().PushUpdateData("UPDATE `sale` SET `status` = %d WHERE `saleId` = %u", static_cast<UInt8>(SALE_TIMEOUT), sale->_id);
                DBLOG().PushUpdateData("update sales set is_cancel=1 where sale_id=%u and server_id=%u", sale->_id, cfg.serverLogId);
                SaleItemCancel saleItemTimeout;
                saleItemTimeout.status = static_cast<UInt8>(SALE_TIMEOUT);
                saleItemTimeout.id = sale->_id;
                saleItemTimeout.item = sale->_item;
                delRowSale(sale);
                _salePos.erase(sale->_id);
                GameMsgHdr hdr(0x304, sale->_owner->getThreadId(), sale->_owner, sizeof(SaleItemCancel));
                GLOBAL().PushMsg(hdr, &saleItemTimeout);
                SAFE_DELETE(sale);
            }
            _saleCheck[i].erase(it++);
        }
    }
}

UInt8 SaleMgr::StatIndex(UInt8 type, UInt32 typeId, UInt8& parent)
{
    static UInt8 cvt[] = { 1, 4, 5, 6, 7, 8, 9, 10, 11, 1, 16, 16, 1, 1, 1, 1, 1, 1, 1, 1,         // 装备，法宝   [0-19]
                           1, 1, 1, 1, 1, 1, 1, 1, 1, 16, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,  // 普通物品， 阵法 [20-39]
                           12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  // 心法， 强化 [40-59]
                           34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 33, 33, 33, 33,  // 宝石 [60-79]
                           0,0,0,0,0,0,0,0,0,0,
                           51, 52, 53, 54, 55, 56, 57, 58, 50, 50, // 魂 [90-99]
                           61, 60, 60 // 元神 [100-102]
                         };

    UInt8 res = cvt[type];
    if(type > sizeof(cvt) - 1)
        res = 1;

    parent = res;

    if(res > 3 && res < 12)
    {
        parent = 3;
    }
    else if(res > 33 && res < 50)
    {
        parent = 33;
    }
    else if (res > 50 && res < 59)
    {
        parent = 50;
    }
    else if(res > 59 && res < 62)
    {
        parent = 59;
    }

    switch(res)
    {
    case 16:                                      //法宝
        parent = 16;
        if(type == 29)
            res += 3;
        else if(typeId > 1499 && typeId < 1600)        //被动技能法宝
            res += 1;
        else if(typeId > 1599 && typeId < 1700)   //主动技能法宝
            res += 2;
        break;
    case 20:                                                         //阵法
        parent = 20;
        if(typeId == 1000 || (typeId > 1011 && typeId < 1020))       //两仪微尘阵
            res += 12;
        else if(typeId == 1001 || (typeId > 1019 && typeId < 1031))  //须弥九宫阵
            res += 11;
        else if(typeId == 1002 || (typeId > 1030 && typeId < 1040))  //金刚伏魔阵
            res += 10;
        else if(typeId == 1003 || (typeId > 1039 && typeId < 1052))  //紫薇太极阵
            res += 9;
        else if(typeId == 1010 || (typeId > 1051 && typeId < 1059))  //五行灭绝阵
            res += 8;
        else if(typeId == 1004 || (typeId > 1058 && typeId < 1065))  //四象元灵阵
            res += 2;
        else if(typeId == 1005 || (typeId > 1064 && typeId < 1074))  //北斗七星阵
            res += 7;
        else if(typeId == 1006 || (typeId > 1073 && typeId < 1084))  //颠倒八卦阵
            res += 6;
        else if(typeId == 1007 || (typeId > 1083 && typeId < 1096))  //都天烈火阵
            res += 5;
        else if(typeId == 1008 || (typeId > 1095 && typeId < 1107))  //天罡地煞阵
            res += 4;
        else if(typeId == 1009 || (typeId > 1106 && typeId < 1116))  //奇门遁甲阵
            res += 3;
        else if(typeId == 1111 || (typeId > 1115 && typeId < 1125))  //七色锁云阵
            res += 1;
        break;
    case 12:
        {
            parent = 12;
            const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[typeId];
            if(type == 45)
                res += 3;
            else if(itemType->career == 0)
            {
                res += 1;
            }
            else
            {
                res += 2;
            }
        }
        break;
    }

    return res;
}

UInt8 SaleMgr::Index(UInt8 type, UInt32 typeId)
{
    UInt8 parent = 0;
    UInt8 stIdx = StatIndex(type, typeId, parent);
    if(stIdx > 61)
    {
        stIdx = 1;
        parent = 1;
    }

    return stIdx;
}

}

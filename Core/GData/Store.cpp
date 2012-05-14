#include "Config.h"
#include "Store.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"
#include "MsgID.h"

namespace GData
{

Store store;

void Store::add( UInt8 type, UInt32 itemId, UInt32 price )
{
	if(type < 1 || type > 7)
		return;
	_items[type - 1].push_back(itemId + (price << 16));
	_itemPrices[type - 1][itemId] = price;
}

void Store::addExchange(UInt8 type, UInt32 itemId, UInt32 priceID, UInt32 priceNum)
{
	if(type < EXCHANGE || type > EXCHANGEEND)
		return;
    Exchange exchange;
    exchange.itemID = static_cast<UInt16>(itemId);
    exchange.priceID = static_cast<UInt16>(priceID);
    exchange.priceNum = static_cast<UInt16>(priceNum);
	_itemsExchange[type - EXCHANGE].push_back(exchange);
	_itemPricesExchange[type - EXCHANGE][itemId] = priceID + (priceNum << 16);
}

UInt32 Store::getPrice( UInt8 type, UInt16 itemId )
{
	if(type < 1 || type > EXCHANGEEND)
		return 0xFFFFFFFF;

    if (type >= EXCHANGE)
    {
        if (type <= EXCHANGEEND)
        {
            std::map<UInt32, UInt32>::iterator it = _itemPricesExchange[type - EXCHANGE].find(itemId);
            if(it == _itemPrices[type - EXCHANGE].end())
                return 0xFFFFFFFF;
            return it->second;
        }
    }
    else
    {
        std::map<UInt32, UInt32>::iterator it = _itemPrices[type - 1].find(itemId);
        if(it == _itemPrices[type - 1].end())
            return 0xFFFFFFFF;
        return it->second;
    }
    return 0;
}

UInt32 Store::getPrice( UInt16 itemId )
{
    for (UInt8 type = 1; type <= 3; ++type)
    {
        std::map<UInt32, UInt32>::iterator it = _itemPrices[type - 1].find(itemId);
        if(it == _itemPrices[type - 1].end())
            continue;
        return it->second;
    }
    return 0;
}

void Store::sendList( UInt8 type, GObject::Player * player )
{
	if(type < 1 || type > EXCHANGEEND)
		return;
    if (type >= EXCHANGE)
        player->send(_storePacketExchange[type - EXCHANGE]);
    else
        player->send(_storePacket[type - 1]);
    if (type == 1)
        player->sendDiscountLimit();
}

void Store::makePacket()
{
	for(int i = 0; i < 7; ++ i)
	{
		std::vector<UInt32>& items = _items[i];
		_storePacket[i].init(REP::STORE_LIST);
		_storePacket[i] << static_cast<UInt8>(i + 1) << static_cast<UInt8>(items.size());
		for(std::vector<UInt32>::iterator it = items.begin(); it != items.end(); ++ it)
		{
			_storePacket[i] << *it;
		}
		_storePacket[i] << Stream::eos;
	}

    for (int i = 0; i < 1; ++i)
    {
		std::vector<Exchange>& items = _itemsExchange[i];
		_storePacketExchange[i].init(REP::STORE_LIST_EXCHANGE);
		_storePacketExchange[i] << static_cast<UInt8>(i + EXCHANGE) << static_cast<UInt8>(items.size());
		for(std::vector<Exchange>::iterator it = items.begin(); it != items.end(); ++ it)
        {
            _storePacketExchange[i] << (*it).itemID << (*it).priceID << (*it).priceNum;
        }
		_storePacketExchange[i] << Stream::eos;
    }
}

void Store::clear()
{
	for(int i = 1; i < 7; ++ i) // XXX: do not clear _items[0]
	{
		_storePacket[i].clear();
		_items[i].clear();
		_itemPrices[i].clear();
	}
    for (int i = EXCHANGE-8; i < EXCHANGEEND-7; ++i)
    {
		_storePacketExchange[i].clear();
		_itemsExchange[i].clear();
		_itemPricesExchange[i].clear();
    }
    _discountLimit.clear();
}

void Store::storeDiscount()
{
    DB3().PushUpdateData("DELETE FROM `discount`;");
    for (UInt8 i = 0; i < _items[0].size(); ++i)
    {
        DB3().PushUpdateData("INSERT INTO `discount` (`itemid`, `discount`) VALUES (%u, %u)",
                _items[0][i]&0xFFFF, (_items[0][i]>>16)&0xFFFF);
    }
}

void Store::discountLimit()
{
    static UInt8 baseLimit[] = {3,5,8};
    for (UInt8 n = 0; n < 3; ++n)
    {
        UInt8 c = 0;
        for (UInt8 i = 0; i < _items[0].size() && c < 4; ++i)
        {
            if (((_items[0][i]>>16)&0xFFFF) == baseLimit[n])
                ++c;
        }
        _discountLimit.push_back(baseLimit[n] + 4 / c);
    }
}

UInt16 Store::getDiscountLimit(UInt8 type)
{
    if (type == 3 && _discountLimit.size() >= 1)
        return _discountLimit[0];
    if (type == 5 && _discountLimit.size() >= 2)
        return _discountLimit[1];
    if (type == 8 && _discountLimit.size() >= 3)
        return _discountLimit[2];
    return 0;
}

float Store::getDiscount(UInt8 type)
{
    if (type == 3)
        return .3f;
    if (type == 5)
        return .5f;
    if (type == 8)
        return .8f;
    return 0.f;
}

UInt8 Store::getDisVarOffset(UInt8 type)
{
    if (type == 3)
        return 0;
    if (type == 5)
        return 1;
    if (type == 8)
        return 2;
    return 0xff;
}

UInt8 Store::getItemsByDiscount(UInt8 type, UInt16 items[4])
{
    UInt8 c = 0;
    for (UInt8 i = 0; i < _items[0].size() && c < 4; ++i)
    {
        if (((_items[0][i]>>16)&0xFFFF) == type)
            items[c++] = (_items[0][i]&0xFFFF);
    }
    return c;
}

}


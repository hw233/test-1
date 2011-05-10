#include "Config.h"
#include "Store.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"

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

UInt32 Store::getPrice( UInt8 type, UInt16 itemId )
{
	if(type < 1 || type > 7)
		return 0xFFFFFFFF;
	std::map<UInt32, UInt32>::iterator it = _itemPrices[type - 1].find(itemId);
	if(it == _itemPrices[type - 1].end())
		return 0xFFFFFFFF;
	return it->second;
}

void Store::sendList( UInt8 type, GObject::Player * player )
{
	if(type < 1 || type > 7)
		return;
	player->send(_storePacket[type - 1]);
}

void Store::makePacket()
{
	for(int i = 0; i < 7; ++ i)
	{
		std::vector<UInt32>& items = _items[i];
		_storePacket[i].init(0xB0);
		_storePacket[i] << static_cast<UInt8>(i + 1) << static_cast<UInt8>(items.size());
		for(std::vector<UInt32>::iterator it = items.begin(); it != items.end(); ++ it)
		{
			_storePacket[i] << *it;
		}
		_storePacket[i] << Stream::eos;
	}
}

void Store::clear()
{
	for(int i = 0; i < 7; ++ i)
	{
		_storePacket[i].clear();
		_items[i].clear();
		_itemPrices[i].clear();
	}
}

}

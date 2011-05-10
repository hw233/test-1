#ifndef _STORE_H_
#define _STORE_H_

#include "Common/Stream.h"

namespace GObject
{
	class Player;
}

namespace GData
{

class Store
{
public:
	void add(UInt8 type, UInt32 itemId, UInt32 price);
	void sendList(UInt8 type, GObject::Player *);
	UInt32 getPrice(UInt8 type, UInt16 itemId);
	void makePacket();
	void clear();
private:
	std::vector<UInt32> _items[7];
	std::map<UInt32, UInt32> _itemPrices[7];
	Stream _storePacket[7];
};

extern Store store;

}

#endif // _STORE_H_

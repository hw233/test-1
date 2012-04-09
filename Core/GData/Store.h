#ifndef _STORE_H_
#define _STORE_H_

#include "Common/Stream.h"

namespace GObject
{
	class Player;
}

namespace GData
{

#define EXCHANGE 8
#define EXCHANGEEND 8

struct Exchange
{
    UInt16 itemID;
    UInt16 priceID;
    UInt16 priceNum;
};

class Store
{
public:
	void add(UInt8 type, UInt32 itemId, UInt32 price);
	void addExchange(UInt8 type, UInt32 itemId, UInt32 priceID, UInt32 priceNum);
    bool needResetDiscount() { return !_items[0].size(); }
    void storeDiscount();
	void sendList(UInt8 type, GObject::Player *);
	UInt32 getPrice(UInt8 type, UInt16 itemId);
	UInt32 getPrice(UInt16 itemId);
	void makePacket();
	void clear();
    void resetDistcount() { _items[0].clear(); _discountLimit.clear(); }
    void discountLimit();
    UInt16 getDiscountLimit(UInt8 type);
    UInt8 getItemsByDiscount(UInt8 type, UInt16 items[4]);
    float getDiscount(UInt8 type);
    UInt8 getDisVarOffset(UInt8 type);
private:
	std::vector<UInt32> _items[7];
	std::map<UInt32, UInt32> _itemPrices[7];
	Stream _storePacket[7];

	std::vector<Exchange> _itemsExchange[1];
	std::map<UInt32, UInt32> _itemPricesExchange[1];
	Stream _storePacketExchange[1];

    std::vector<UInt16> _discountLimit;
};

extern Store store;

}

#endif // _STORE_H_

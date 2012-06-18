#ifndef _STORE_H_
#define _STORE_H_

#include "Common/Stream.h"

namespace GObject
{
	class Player;
}

namespace GData
{
#if 0
#define EXCHANGE 8
#else
/** 购买索引: 1-7, 11-14 **/
#define PURCHASE1 1
#define PURCHASE2 7
#define PURCHASE3 11
#define PURCHASE4 14
/** 兑换索引: 8 **/
#define EXCHANGE 8
#define EXCHANGEEND 8
#endif
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
	std::vector<UInt32> _items[PURCHASE2-PURCHASE1+1];
	std::map<UInt32, UInt32> _itemPrices[PURCHASE2-PURCHASE1+1];
	Stream _storePacket[PURCHASE2-PURCHASE1+1];

	std::vector<UInt32> _items2[PURCHASE4-PURCHASE3+1];
	std::map<UInt32, UInt32> _itemPrices2[PURCHASE4-PURCHASE3+1];
	Stream _storePacket2[PURCHASE4-PURCHASE3+1];

	std::vector<Exchange> _itemsExchange[EXCHANGEEND-EXCHANGE+1];
	std::map<UInt32, UInt32> _itemPricesExchange[EXCHANGEEND-EXCHANGE+1];
	Stream _storePacketExchange[EXCHANGEEND-EXCHANGE+1];

    std::vector<UInt16> _discountLimit;
};

extern Store store;

}

#endif // _STORE_H_

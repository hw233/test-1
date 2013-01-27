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
#define PURCHASE4 15 /** 15代表帮派贡献，但放在商场 **/
/** 兑换索引: 8 **/
#define EXCHANGE 8
#define EXCHANGEEND 8
/** 限购索引：1 **/
#define DISCOUNT 1
#define DISCOUNTEND 1
#endif

struct Exchange
{
    UInt16 itemID;
    UInt16 priceID;
    UInt16 priceNum;
};

struct Discount
{
    UInt16 itemID;
    UInt8  discountType;
    UInt8  exType;
    UInt32 exValue;
    UInt32 limitCount;
    UInt32 beginTime;
    UInt32 endTime;
    UInt16 priceOriginal;
    UInt16 priceDiscount;
};

class Store
{
public:
    void process(UInt32 now);
	void add(UInt8 type, UInt32 itemId, UInt32 price);
    void addDiscountFromDB( UInt16 itemID, UInt8 discountType, UInt8 exType, UInt32 exValue,
            UInt32 limitCount, UInt32 beginTime, UInt32 endTime, UInt16 priceOriginal, UInt16 priceDiscount);
	void addExchange(UInt8 type, UInt32 itemId, UInt32 priceID, UInt32 priceNum);
    void addNormalDiscount(UInt32 itemId, UInt32 discount, UInt32 num);
    void addSpecialDiscount();
    UInt8 addSpecialDiscountFromBS(Discount discount);
    void querySpecialDiscountFromBS();
    UInt8 clearSpecialDiscountFromBS(UInt8 type = 0);
    bool needResetDiscount();
    void storeDiscount();
	void sendList(UInt8 type, GObject::Player *);
    UInt32 getPrice( UInt8 type, UInt16 itemId, UInt16 flag, UInt8 index);
	UInt32 getPrice(UInt8 type, UInt16 itemId);
	UInt32 getPrice(UInt16 itemId);
	void makePacket();
	void clear();
    void clearNormalDiscount();
    void clearSpecialDiscount();
    void resetDistcount() { _items[0].clear(); _discountLimit.clear(); }
    UInt8 getDiscountType(UInt8 columnIndex);
    UInt16 getDiscountLimit(UInt8 type);
    UInt8 getItemsByDiscount(UInt8 type, UInt16 items[4]);
    UInt8 getDisTypeVarOffset(UInt8 type);
    UInt8 getNewDisVarOffset(UInt8 type);
    UInt32 getEndTimeByDiscountType(UInt8 type);
    UInt32 getBeginTimeByDiscountType(UInt8 type);
    UInt8 getExDiscount(UInt8 type, UInt32& exValue);

    const Discount* getDiscount(UInt8 type, UInt8 index);
    UInt8 getDiscountItemsCount(UInt8 type);
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

    // 限购栏的商品信息
	std::vector<Discount> _itemsDiscount[DISCOUNTEND-DISCOUNT+1];
	std::map<UInt8, UInt8> _itemTypeCountDiscount[DISCOUNTEND-DISCOUNT+1];
	Stream _storePacketDiscount[DISCOUNTEND-DISCOUNT+1];

    std::vector<UInt16> _discountLimit; };

extern Store store;

}

#endif // _STORE_H_

#ifndef SALE_H_
#define SALE_H_
#include "Common/Stream.h"
namespace GObject
{

class ItemBase;
class Player;

enum SALE_STATUS
{
	SALE_SELL,
	SALE_BUY,
	SALE_CANCEL,
	SALE_TIMEOUT,
	SALE_DELETED
};


struct SaleSellData
{
	UInt32 id;
	UInt16 count;
	UInt32 price;
	UInt8 priceType;
};

struct SaleSellRespData
{
	UInt32 id;
	UInt8 priceType;
	UInt32 price;
	char itemName[32];

	SaleSellRespData()
	{
		memset(this, 0x00, sizeof(SaleSellRespData));
	}
};
//支持不定长 TODO:
struct SalePut
{
	ItemBase * item;
	UInt8 priceType;
	UInt32 price;

	SalePut() : item(NULL), priceType(0), price(0) {}
};

struct SalePriceCheck
{
	UInt8 priceType;
	UInt32 price;
	UInt32 id;
	UInt32 pos;
};

struct SaleItemBuy
{
	ItemBase * item;
	UInt32 id;
	UInt8 priceType;
	UInt32 price;

	SaleItemBuy() : item(NULL), id(0), priceType(0), price(0) {}
};

struct SaleItemCancel
{
	UInt8 status;	//2:取消 3:超时
	UInt32 id;
	ItemBase * item;

	SaleItemCancel() : status(0xFF), id(0), item(NULL) {}
};

struct SalePos
{
	UInt32 id;
	UInt32 pos;

	SalePos() : id(0), pos(0) {}
	SalePos(UInt32 id_, UInt32 pos_) : id(id_), pos(pos_) {}
};

struct SaleSearchReq
{
	UInt16 start;
	UInt16 count;
	Player * founder;

	SaleSearchReq() : start(0), count(0), founder(NULL) {}
	SaleSearchReq(UInt16 start_, UInt16 count_, Player * founder_) : start(start_), count(count_), founder(founder_) {}
};

struct SaleSearchResp
{
	UInt32 ids[24];
	UInt16 start;
	UInt16 count;
	Player * founder;

	SaleSearchResp()
	{
		memset(this, 0x00, sizeof(SaleSearchResp));
	}
};

struct SaleMailItem
{
	ItemBase * item;
	bool mailSend;

	SaleMailItem() : item(NULL), mailSend(false) {}
	SaleMailItem(ItemBase * item_, bool mailSend_) : item(item_), mailSend(mailSend_) {}
};


//每次只能处理一个交易状态 TODO
class Sale
{
public:
	Sale(Player *);
	~Sale();

public:
	void Init();
	void UnInit();

public:
	bool addSaleFromDB(SaleSellRespData *);
	bool addSaleMailFromDB(UInt32, ItemBase *, bool = false);

public:
	void sellSaleReq(std::vector<SaleSellData>&);
	void sellSaleReqNotify(SaleSellRespData *, UInt8);
	void searchMySale(SaleSearchReq&);
	void checkSaleBuyPrice(SalePriceCheck&);
	void buySellResp(SaleItemBuy&);
	void cancelSellResp(SaleItemCancel&);
	void sellSaleResp(UInt32, Player *, UInt32, UInt16);

	bool addSaleMailItems(UInt32);
	void delSaleMailItems(UInt32);

	void makeMailInfo(UInt32 id, Stream& st, UInt16& num);

    void cancleAllItem();
public:
	inline bool hasAccessSaleItem(UInt32 id)
	{
		return _accessItems.find(id) != _accessItems.end();
	}
	inline void addAccessSaleItem(UInt32 id)
	{
		_accessItems.insert(id);
	}

private:
	Player * _owner;
	std::map<UInt32, SaleSellRespData *> _sellItems;
	std::set<UInt32> _accessItems;	//Only accessed by World thread !
	std::map<UInt32, SaleMailItem *> _mailItems;
};

}


#endif

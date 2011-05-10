#ifndef _TRADE_H_
#define _TRADE_H_

namespace GObject
{

class Player;

enum TRADE_STATUS
{
	TRADE_NONE = 0,
	TRADE_LAUNCH,
	TRADE_RECEIVE,
	TRADE_REPLY,
	TRADE_CONFIRM,	 //待确认
	TRADE_CONFIRMED, //已确认
	TRADE_SELFOK,
	TRADE_PEEROK,
	TRADE_SELFCANCEL,
	TRADE_PEERCANCEL,
	TRADE_SUCCPACKFULL, //成功交易后背包已满
	TRADE_FAILPACKFULL, //失败交易后背包已满
	TRADE_PACKFULLSUCC,
	TRADE_PACKFULLFAIL,
	TRADE_TIMEOUT,
};


struct TradeData
{
	UInt32 _id;
	UInt8  _tradeSide;
	UInt8  _tradeStatus;
	std::string _tradeTitle;
	UInt32 _tradeTime;
	Player * _tradePlayer;
	UInt32 _coin;
	UInt32 _gold;
	std::string _strItems;
	std::vector<ItemBase*> _items;
	TradeData(UInt32 id = 0) : _id(id), _tradeSide(0xFF), _coin(0), _gold(0) {}
};



struct TradeItemChange
{
	UInt32 _itemId;
	UInt16 _itemNum;

	TradeItemChange(UInt32 id = 0, UInt16 num = 0) : _itemId(id), _itemNum(num) {}
};

class Trade
{
public:
	Trade(Player * = NULL);
	~Trade();

public:
	bool addTradeFromDB(TradeData*);

public:
	void sendTradeList(UInt16, UInt16);
	bool sendTradeData(UInt32);
	bool sendTradeData(UInt32, Player *, UInt8 = 0xFF);

public:
	bool launchTrade(std::string&, const std::string&, UInt32, UInt32, std::vector<TradeItemChange>&);
	bool replyTrade(UInt32, UInt32, UInt32, std::vector<TradeItemChange>&);
	void confirmTrade(UInt32);
	void cancelTrade(UInt32);
	void deleteTrade(UInt32);
	void cancel2Trade(UInt32);

public:
	bool recvLaunchTrade(UInt32, const std::string&, UInt32, Player*);
	bool recvReplyTrade(UInt32);
	bool recvConfirmTrade(UInt32, UInt32, UInt32, ItemBase**, UInt16);
	bool recvCancelTrade(UInt32);

	void sendTradeMail(UInt32, UInt8);
	bool addTradeMailItems(UInt32);
	void makeMailInfo(UInt32 id, Stream& st, UInt16& num);

	static bool needBuildTradeItems(UInt8);
	static bool needCheckTrade(UInt8);

    TradeData* findTradeDataById(UInt32);
protected:
	void launchTradeNotify(UInt32, const std::string&, const std::string&, UInt8, UInt32);
	void operateTradeNotify(UInt32, UInt8);
	void addTradeTimeoutNotify(UInt32, UInt32);
	void delTradeTimeoutNotify(UInt32, UInt32);

private:
	void buildTradeItems(std::vector<TradeItemChange>&, std::string&);
	void buildTradeItems(ItemBase**, UInt16, std::string&);
	bool appendTradeItems(std::vector<ItemBase *>&);
	bool appendTradeItems(ItemBase**, UInt16);
	void replaceTradeItems(std::vector<ItemBase *>&, ItemBase**, UInt16);
	UInt16 getTradeItemsNum(std::vector<ItemBase *>&);

private:
	Player * _owner;
	typedef std::map<UInt32, TradeData*, std::greater<UInt32> > TradeDataList;
	TradeDataList _tradeDatas;
};

}


#endif

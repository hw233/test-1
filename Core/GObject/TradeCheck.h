#ifndef _TRADECHECK_H_
#define _TRADECHECK_H_

namespace GObject
{

class Player;

struct TradeCheckData
{
	UInt32 _id;
	UInt32 _time;
	Player * _trader;

	TradeCheckData(UInt32 id = 0) : _id(id), _time(0), _trader(NULL){}
	TradeCheckData(UInt32 id, UInt32 time, Player* trader) : _id(id), _time(time), _trader(trader) {}
};

class TradeCheck
{
	typedef std::multimap<UInt32, TradeCheckData*>::iterator iterator;
	typedef std::multimap<UInt32, TradeCheckData*>::const_iterator const_iterator;

	static const UInt32 TRADE_TIME_OUT = 24 * 60 * 60;
public:
	TradeCheck();
	~TradeCheck();

public:
	void addTradeCheck(UInt32, UInt32, Player*);
	void delTradeCheck(UInt32, UInt32);

public:
	void update(UInt32);

private:
	std::multimap<UInt32, TradeCheckData*> _tradeDatas;	//½»Ò×¼ÇÂ¼
};

extern TradeCheck gTradeCheck;

}

#endif
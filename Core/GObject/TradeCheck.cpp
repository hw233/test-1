#include "Config.h"
#include "Player.h"
#include "TradeCheck.h"

namespace GObject
{

TradeCheck gTradeCheck; //Only used in world thread

TradeCheck::TradeCheck()
{

}

TradeCheck::~TradeCheck()
{
	iterator it = _tradeDatas.begin();
	for (; it != _tradeDatas.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}
	_tradeDatas.clear();
}

void TradeCheck::addTradeCheck(UInt32 id, UInt32 time, Player* trader)
{
	TradeCheckData* trade = new(std::nothrow) TradeCheckData(id, time, trader);
	if (trade != NULL)
	{
		_tradeDatas.insert(std::make_pair(time, trade));
	}
}

void TradeCheck::delTradeCheck(UInt32 id, UInt32 time)
{
	std::pair<iterator, iterator> found = _tradeDatas.equal_range(time);
	for(iterator it = found.first; it != found.second; ++it)
	{
		if (it->second->_id == id)
		{
			SAFE_DELETE(it->second);
			_tradeDatas.erase(it);
			break;
		}
	}
}

void TradeCheck::update(UInt32 curr)
{
	iterator itUp = _tradeDatas.upper_bound(curr-TRADE_TIME_OUT);
	for (iterator it = _tradeDatas.begin(); it != itUp;)
	{
		UInt32 id = it->second->_id; 
		GameMsgHdr hdr(0x295, it->second->_trader->getThreadId(), it->second->_trader, sizeof(UInt32));
		GLOBAL().PushMsg(hdr, &id);	
		SAFE_DELETE(it->second);
		_tradeDatas.erase(it++);
	}
}


}
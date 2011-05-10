#ifndef _LUCKYDRAW_H_
#define _LUCKYDRAW_H_

#include "Common/Mutex.h"

namespace GData
{
struct LootResult;
}

namespace GObject
{

class Player;

class LuckyDraw
{
public:
	LuckyDraw();
	void pushCostFromDB(Player * player, UInt32 cost);
	void setLuckyDrawCost();
	GData::LootResult doLuckyDraw(UInt8);
	void checkCleanup();
	void pushCost( Player *, UInt32 );
	void pushResult(Player *, UInt8, UInt16, UInt8, UInt8);
	void sendInfo(Player *);
private:
	void doCleanup();
private:
	struct LuckyItem
	{
		Player * player;
		UInt16 itemId;
		UInt8 count;
		UInt8 area;
	};

	std::list<LuckyItem> _luckyItems;

	std::map<UInt64, UInt32> _paid;
	Player * _topPayer;
	UInt32 _topPay;

	Player * _topLucker[3];
	UInt16 _luckType[3];
	UInt8 _luckQuality[3];

	FastMutex _mutex;
};

extern LuckyDraw luckyDraw;

}

#endif // _LUCKYDRAW_H_

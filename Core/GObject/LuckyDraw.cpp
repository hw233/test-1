#include "Config.h"
#include "LuckyDraw.h"
#include "Country.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "Item.h"
#include "SpecialAward.h"
#include "Script/GameActionLua.h"
#include "GData/LootTable.h"
#include "Common/URandom.h"
#include "Common/TimeUtil.h"
#include "Common/Stream.h"
#include "MsgID.h"

namespace GObject
{

LuckyDraw luckyDraw;

LuckyDraw::LuckyDraw()
{
	doCleanup();
}

GData::LootResult LuckyDraw::doLuckyDraw( UInt8 l )
{
	static GData::LootResult empty = {0, 0};
	if(l > 7)
		return empty;
	//if(World::_newYearStage > 0)
	//{
	//	UInt32 id = GameAction()->onLuckyDrawItemRoll(l);
	//	if(id > 0)
	//	{
	//		GData::LootResult lr;
	//		lr.id = id;
	//		lr.count = 1;
	//		return lr;
	//	}
	//}
	const GData::LootItem * li = GData::lootTable[2001 + l];
	if(li == NULL)
		return empty;
	return li->roll();
}

void LuckyDraw::checkCleanup()
{
	doCleanup();
	DB().PushUpdateData("DELETE FROM `luckydrawgold`");
}

void LuckyDraw::doCleanup()
{
	FastMutex::ScopedLock lk(_mutex);

	_luckyItems.clear();

	_paid.clear();
	_topPayer = NULL;
	_topPay = 0;

	_topLucker[0] = NULL;
	_luckType[0] = 0;
	_luckQuality[0] = 0;
	_topLucker[1] = NULL;
	_luckType[1] = 0;
	_luckQuality[1] = 0;
	_topLucker[2] = NULL;
	_luckType[2] = 0;
	_luckQuality[2] = 0;
}

void LuckyDraw::pushCost( Player * player, UInt32 cost )
{	
	UInt32& paid = _paid[player->getId()];
	paid += cost;
	if(paid > _topPay)
	{
		_topPayer = player;
		_topPay = paid;
	}
	DB().PushUpdateData("REPLACE INTO `luckydrawgold` values(%"I64_FMT"u, %u)", player->getId(), paid);
}

void LuckyDraw::setLuckyDrawCost()
{
	gSpecialAward.luckydrawAward(_topPayer);
}

void LuckyDraw::pushCostFromDB(Player * player, UInt32 cost)
{
	_paid[player->getId()] = cost;
	if(cost > _topPay)
	{
		_topPayer = player;
		_topPay = cost;
	}
}

void LuckyDraw::pushResult( Player * player, UInt8 quality, UInt16 id, UInt8 num, UInt8 t )
{
	if(t < 2 || t > 4)
		return;

	FastMutex::ScopedLock lk(_mutex);
	
	if(quality == 5)
	{
		LuckyItem li = {player, id, num, t};
		while(_luckyItems.size() >= 20)
			_luckyItems.pop_front();
		_luckyItems.push_back(li);
	}

	t -= 2;
	if(quality >= _luckQuality[t])
	{
		_topLucker[t] = player;
		_luckQuality[t] = quality;
		_luckType[t] = id;
	}
}

void LuckyDraw::sendInfo( Player * player )
{
	FastMutex::ScopedLock lk(_mutex);

	Stream st(REP::LUCKYDRAW_INFO);
	if(_topPayer != NULL)
		st << _topPayer->getName() << _topPayer->getCountry();
	else
		st << "" << static_cast<UInt8>(0);
	st << _topPay;
	for(int i = 0; i < 3; ++ i)
	{
		if(_topLucker[i] != NULL)
			st << _topLucker[i]->getName() << _topLucker[i]->getCountry();
		else
			st << "" << static_cast<UInt8>(0);
		st << _luckType[i];
	}
	st << Stream::eos;
	player->send(st);

	st.init(REP::LUCKYDRAW_OTH);
	st << static_cast<UInt8>(_luckyItems.size());
	for(std::list<LuckyItem>::iterator it = _luckyItems.begin(); it != _luckyItems.end(); ++ it)
	{
		st << it->player->getName() << it->player->getCountry() << it->area << it->itemId;
		if(!IsEquipTypeId(it->itemId))
			st << it->count;
	}
	st << Stream::eos;
	player->send(st);
}

}

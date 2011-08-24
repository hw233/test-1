#include "Config.h"
#include "BlockBossMgr.h"
#include "Player.h"
#include "Common/Stream.h"
#include "Server/SysMsg.h"
#include "GData/ItemType.h"
#include "Package.h"
#include "MsgID.h"

namespace GObject
{
#define  MAX_RANK 999
BlockBossMgr gBlockbossmgr;

void BlockBossMgr::resetPlayerRank(Player *pl, UInt16 bossLevel, UInt8 playerLevel, UInt16 couponCount, UInt16 itemId, UInt8 itemCount)
{
	if(!_playerRank.empty())
	{
		Rank::iterator start = _playerRank.begin();
		if( bossLevel > 60 && bossLevel > (*start)._bossLevel)
		{
			if(itemId != 0)
			{
				//SYSMSG_BROADCASTV(376, pl->getCountry(), pl->getName().c_str(), bossLevel - 21, couponCount, itemCount, itemId);
			}
			else
			{
				//SYSMSG_BROADCASTV(375, pl->getCountry(), pl->getName().c_str(), bossLevel - 21);
			}
		}
		Rank::iterator it = find(pl);
		if(it != _playerRank.end())/////
			_playerRank.erase(it);
		else if(_playerRank.size() >= MAX_RANK)
		{
			it = _playerRank.end();
			-- it;
			if(bossLevel < it->_bossLevel)
				return;
			else
				_playerRank.erase(it);
		}
	}
	_playerRank.insert(RankData(pl, bossLevel, playerLevel));

}

void BlockBossMgr::addPlayerRank(Player *pl, UInt16 bossLevel, UInt8 playerLevel)
{
	if(_playerRank.size() >= MAX_RANK)
	{
		Rank::iterator it = _playerRank.end();
		it --;
		if(bossLevel < (*it)._bossLevel && playerLevel < (*it)._playerLevel)
			return;
		_playerRank.erase(it);
	}
	_playerRank.insert(RankData(pl, bossLevel, playerLevel));
}
static bool find_player(RankData rank_pl, Player *pl)
{
	return rank_pl._player == pl;
}
Rank::iterator BlockBossMgr::find(Player *pl)
{
	using namespace std::placeholders;
	Rank::iterator it = std::find_if(_playerRank.begin(), _playerRank.end(), std::bind(find_player, _1, pl));
	return it;
}
void BlockBossMgr::reqBlockBossData(Player *pl, UInt16 bossLevel)
{
	Rank::iterator it = find(pl);
	UInt32 rank = 999;
	if(it != _playerRank.end())
		rank = std::distance(_playerRank.begin(), it) + 1;
		
	Stream st(REP::BLOCKBOSS);
	UInt8 count = static_cast<UInt8>(_playerRank.size() > 3 ? 3 : _playerRank.size());
	st << bossLevel << rank << count;
	it = _playerRank.begin();
	for(UInt8 i = 0; i < count; i ++, it ++)
	{
		st << (*it)._player->getName() << (*it)._player->getCountry() << (*it)._playerLevel << (*it)._bossLevel;
	}
	st << Stream::eos;
	pl->send(st);
}

}

#include "Config.h"
#include "Leaderboard.h"
#include "Country.h"
#include "Server/WorldServer.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Clan.h"
#include "AthleticsRank.h"
#include "DB/DBExecHelper.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecutor.h"
#include "MsgID.h"

namespace GObject
{

struct LeaderboardItem
{
	UInt64 id;
	std::string name;
	UInt8 lvl;
	UInt8 country;
	UInt32 value;
	std::string clan;
};

}

namespace DB {

SPECIALBEGIN(GObject::LeaderboardItem)
SPECIALDEF(6)
(
	UInt64, id,
	std::string, name,
	UInt8, lvl,
	UInt8, country,
	UInt32, value,
	std::string, clan
)
SPECIALEND()

}

namespace GObject
{

Leaderboard leaderboard;

void buildPacket(Stream& st, UInt8 t, UInt32 id, std::vector<LeaderboardItem>& list, bool merge = true)
{
	UInt8 c = static_cast<UInt8>(list.size());
	st.init(REP::SORT_LIST);
	st << t << id << c;
	for(UInt8 i = 0; i < c; ++ i)
	{
		LeaderboardItem& item = list[i];
		if(merge)
			Player::patchMergedName(item.id, item.name);
		st << item.name << item.lvl << item.country << item.value << item.clan;
	}
	st << Stream::eos;
}

void Leaderboard::update()
{
	doUpdate();
}

void Leaderboard::doUpdate()
{
	std::unique_ptr<DB::DBExecutor> execu;
	execu.reset(DB::gObjectDBConnectionMgr->GetExecutor());
	if (execu.get() == NULL || !execu->isConnected()) return;

	++ _id;

	std::vector<LeaderboardItem> blist;

	blist.clear();
	execu->ExtractData("SELECT `player`.`id`, `player`.`name`, `fighter`.`level`, `player`.`country`, `fighter`.`experience`, `clan`.`name` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" ORDER BY `fighter`.`experience` DESC"
		" LIMIT 0, 100", blist);
	buildPacket(_levelStream, 0, _id, blist);
	if(!blist.empty())
		_maxLevel = blist[0].lvl;

	blist.clear();
	std::list<AthleticsRankData *> *pathleticsrank = gAthleticsRank.getAthleticsRank();
	std::list<AthleticsRankData *> athleticsrank = pathleticsrank[1];
	blist.resize(100);
	UInt32 i = 0;
	for(std::list<AthleticsRankData *>::iterator it = athleticsrank.begin(); i < 100 && it != athleticsrank.end(); ++ i, ++ it )
	{
		blist[i].id = (*it)->ranker->getId();
		blist[i].name = (*it)->ranker->getName();
		blist[i].lvl = (*it)->ranker->GetLev();
		blist[i].country = (*it)->ranker->getCountry();
		blist[i].value = i + 1;
		blist[i].clan = (*it)->ranker->getClanName();
	}
	blist.resize(i);
	buildPacket(_moneyStream, 1, _id, blist);

	blist.clear();
	execu->ExtractData("SELECT `player`.`id`, `player`.`name`, `fighter`.`level`, `player`.`country`, `player`.`archievement`, `clan`.`name` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" ORDER BY `player`.`archievement` DESC"
		" LIMIT 0, 100", blist);
	buildPacket(_achievementStream, 2, _id, blist, false);

	blist.clear();
	execu->ExtractData("SELECT `clan`.`id`, `player`.`name`, `clan_skill`.`level`, `player`.`country`, COUNT(`clan_player`.`id`) AS `pcount`, `clan`.`name` FROM"
		"`clan`, `clan_skill`, `clan_player`, `player`"
		"WHERE `clan_skill`.`clanId` = `clan`.`id` AND `clan_skill`.`skillId` = 1 AND `clan`.`id` = `clan_player`.`id`"
		"AND `clan`.`leader` = `player`.`id` GROUP BY `clan_player`.`id`"
		"ORDER BY `clan_skill`.`level` DESC, `clan`.`proffer` DESC, `pcount` DESC LIMIT 0, 100", blist);
	buildPacket(_clanStream, 3, _id, blist);

	std::vector<UInt64> ilist;
	size_t cnt;

	_levelRankWorld.clear();
	_levelRankCountry[0].clear();
	_levelRankCountry[1].clear();
	_moneyRankWorld.clear();
	_moneyRankCountry[0].clear();
	_moneyRankCountry[1].clear();
	_achievementRankWorld.clear();
	_achievementRankCountry[0].clear();
	_achievementRankCountry[1].clear();
	_clanRankWorld.clear();
	_clanRankCountry[0].clear();
	_clanRankCountry[1].clear();

	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" ORDER BY `fighter`.`experience` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_levelRankWorld[ilist[i]] = static_cast<UInt16>(i + 1);
	}

	ilist.clear();
	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" WHERE `player`.`country` = 0"
		" ORDER BY `fighter`.`experience` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_levelRankCountry[0][ilist[i]] = static_cast<UInt16>(i + 1);
	}

	ilist.clear();
	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" WHERE `player`.`country` = 1"
		" ORDER BY `fighter`.`experience` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_levelRankCountry[1][ilist[i]] = static_cast<UInt16>(i + 1);
	}

	ilist.clear();
	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" ORDER BY `coin` DESC, `tael` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_moneyRankWorld[ilist[i]] = static_cast<UInt16>(i + 1);
	}

	ilist.clear();
	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" WHERE `country` = 0"
		" ORDER BY `coin` DESC, `tael` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_moneyRankCountry[0][ilist[i]] = static_cast<UInt16>(i + 1);
	}

	ilist.clear();
	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" WHERE `country` = 1"
		" ORDER BY `coin` DESC, `tael` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_moneyRankCountry[1][ilist[i]] = static_cast<UInt16>(i + 1);
	}

	ilist.clear();
	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" ORDER BY `archievement` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_achievementRankWorld[ilist[i]] = static_cast<UInt16>(i + 1);
	}

	ilist.clear();
	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" WHERE `country` = 0"
		" ORDER BY `archievement` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_achievementRankCountry[0][ilist[i]] = static_cast<UInt16>(i + 1);
	}

	ilist.clear();
	execu->ExtractData("SELECT `player`.`id` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" WHERE `country` = 1"
		" ORDER BY `archievement` DESC"
		" LIMIT 0, 999", ilist);

	cnt = ilist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_achievementRankCountry[1][ilist[i]] = static_cast<UInt16>(i + 1);
	}

	std::vector<UInt32> clist;
	execu->ExtractData("SELECT `clan`.`id` FROM"
		"`clan`, `clan_skill`, `clan_player`, `player`"
		"WHERE `clan_skill`.`clanId` = `clan`.`id` AND `clan_skill`.`skillId` = 1 AND `clan`.`id` = `clan_player`.`id`"
		"AND `clan`.`leader` = `player`.`id` GROUP BY `clan_player`.`id`"
		"ORDER BY `clan_skill`.`level` DESC, `clan`.`proffer` DESC, COUNT(`clan_player`.`id`) DESC LIMIT 0, 100", clist);

	cnt = clist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_clanRankWorld[clist[i]] = static_cast<UInt16>(i + 1);
	}

	clist.clear();
	execu->ExtractData("SELECT `clan`.`id` FROM"
		"`clan`, `clan_skill`, `clan_player`, `player`"
		"WHERE `clan_skill`.`clanId` = `clan`.`id` AND `clan_skill`.`skillId` = 1 AND `clan`.`id` = `clan_player`.`id`"
		"AND `clan`.`leader` = `player`.`id` AND `player`.`country` = 0 GROUP BY `clan_player`.`id`"
		"ORDER BY `clan_skill`.`level` DESC, `clan`.`proffer` DESC, COUNT(`clan_player`.`id`) DESC LIMIT 0, 100", clist);
	
	cnt = clist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_clanRankCountry[0][clist[i]] = static_cast<UInt16>(i + 1);
	}

	clist.clear();
	execu->ExtractData("SELECT `clan`.`id` FROM"
		"`clan`, `clan_skill`, `clan_player`, `player`"
		"WHERE `clan_skill`.`clanId` = `clan`.`id` AND `clan_skill`.`skillId` = 1 AND `clan`.`id` = `clan_player`.`id`"
		"AND `clan`.`leader` = `player`.`id` AND `player`.`country` = 1 GROUP BY `clan_player`.`id`"
		"ORDER BY `clan_skill`.`level` DESC, `clan`.`proffer` DESC, COUNT(`clan_player`.`id`) DESC LIMIT 0, 100", clist);

	cnt = clist.size();
	for(size_t i = 0; i < cnt; ++ i)
	{
		_clanRankCountry[1][clist[i]] = static_cast<UInt16>(i + 1);
	}
}

bool Leaderboard::hasUpdate( UInt32 id )
{
	return _id != id;
}

bool Leaderboard::getPacket( UInt8 t, Stream*& st )
{
	switch(t)
	{
	case 0:
		st = &_levelStream;
		break;
	case 1:
		st = &_moneyStream;
		break;
	case 2:
		st = &_achievementStream;
		break;
	case 3:
		st = &_clanStream;
		break;
	default:
		return false;
	}
	return true;
}

template<typename T1, typename T2>
void _searchInside(Stream& st, T1 mapv, T2 v)
{
	typename T1::iterator it = mapv.find(v);
	if(it != mapv.end())
		st << it->second;
	else
		st << static_cast<UInt16>(0);
}

void Leaderboard::sendOwnRank( Player * player, UInt32 id )
{
	Stream st(REP::SORT_PERSONAL);
	if(id == _id)
	{
		st << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << Stream::eos;
	}
	else
	{
		st << _id;
		UInt64 pid = player->getId();
		UInt8 country = player->getCountry();
		_searchInside(st, _levelRankWorld, pid);
		_searchInside(st, _levelRankCountry[country], pid);
		_searchInside(st, _moneyRankWorld, pid);
		_searchInside(st, _moneyRankCountry[country], pid);
		_searchInside(st, _achievementRankWorld, pid);
		_searchInside(st, _achievementRankCountry[country], pid);
		if(player->getClan() != NULL)
		{
			UInt32 cid = player->getClan()->getId();
			_searchInside(st, _clanRankWorld, cid);
			_searchInside(st, _clanRankCountry[country], cid);
		}
		else
			st << static_cast<UInt32>(0);
		st << Stream::eos;
	}
	player->send(st);
}

}

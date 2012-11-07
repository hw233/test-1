#include "Config.h"
#include "Leaderboard.h"
#include "Country.h"
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Clan.h"
#include "AthleticsRank.h"
#include "DB/DBExecHelper.h"
#include "DB/DBConnectionMgr.h"
#include "DB/DBExecutor.h"
#include "MsgID.h"
#include "ClanRankBattle.h"
#include "Common/Itoa.h"

namespace GObject
{

struct LeaderboardItem
{
	UInt64 id;
	std::string name;
    UInt8 pf;
	UInt8 lvl;
	UInt8 country;
	UInt32 value;
	std::string clan;
};

struct LeaderboardItem2
{
	UInt64 id;
	std::string name;
    UInt8 pf;
	UInt8 lvl;
	UInt8 country;
	UInt64 value;
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

SPECIALBEGIN(GObject::LeaderboardItem2)
SPECIALDEF(6)
(
	UInt64, id,
	std::string, name,
	UInt8, lvl,
	UInt8, country,
	UInt64, value,
	std::string, clan
)
SPECIALEND()

SPECIALBEGIN(GObject::LeaderboardTowndown)
SPECIALDEF(4)
(
    UInt64, id,
	std::string, name,
    UInt16, level,
    UInt32, time
)
SPECIALEND()

SPECIALBEGIN(GObject::LeaderboardClanCopy)
SPECIALDEF(4)
(
    UInt64, id,
	std::string, name,
    UInt32, level,
    UInt64, time
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
		//if(merge)
		//	Player::patchMergedName(item.id, item.name);
        Player* pl = globalPlayers[item.id];
        if (pl)
            st << item.name << pl->getPF() << item.lvl << item.country << item.value << item.clan;
        else
            st << item.name << item.pf << item.lvl << item.country << item.value << item.clan;
	}
	st << Stream::eos;
}

void buildPacket2(Stream& st, UInt8 t, UInt32 id, std::vector<LeaderboardItem2>& list, bool merge = true)
{
	UInt8 c = static_cast<UInt8>(list.size());
	st.init(REP::SORT_LIST);
	st << t << id << c;
	for(UInt8 i = 0; i < c; ++ i)
	{
		LeaderboardItem2& item = list[i];
		if(merge)
			Player::patchMergedName(item.id, item.name);
        Player* pl = globalPlayers[item.id];
        if (pl)
            st << item.name << pl->getPF() << item.lvl << item.country << item.value << item.clan;
        else
            st << item.name << item.pf << item.lvl << item.country << item.value << item.clan;
	}
	st << Stream::eos;
}

void Leaderboard::buildBattlePacket()
{
    _battleRankWorld.clear();
    std::unordered_map<UInt64, Player*>& pm = GObject::globalPlayers.getMap();
    std::unordered_map<UInt64, Player*>::iterator iter;
    for (iter = pm.begin(); iter != pm.end(); ++iter)
    {
        Player* pl = iter->second;
        if (NULL != pl)
        {
            UInt32 battlePoint = pl->getBattlePoint();
            _battleRankWorld.insert(std::make_pair(battlePoint, pl));
        }
    }
    //每个人的战斗力排行
    _playerBattleRank.clear();
    int rank = 0;
    std::multimap<int, Player*, std::greater<int> >::iterator iter1;
    for (iter1 = _battleRankWorld.begin(); iter1 != _battleRankWorld.end(); ++iter1)
    {
        rank++;
        Player* pl = iter1->second;
        if (pl)
            _playerBattleRank[pl->getId()] =  rank;
	}
    int rankCount = _battleRankWorld.size();
    if (rankCount > 100 )
        rankCount = 100;
	_battleStream.init(REP::SORT_LIST);
    _battleStream << static_cast<UInt8>(4) << static_cast<UInt32>(0) << static_cast<UInt8>(rankCount);

    int i = 0;
    std::multimap<int, Player*, std::greater<int> >::iterator iter2;
    for (iter2 = _battleRankWorld.begin(); iter2 != _battleRankWorld.end() && i < rankCount; ++iter2, ++i)
    {
        Player* pl = iter2->second;
        if (pl)
           _battleStream << pl->getName() << pl->getPF() << pl->GetLev() << pl->getCountry() << iter2->first << pl->getClanName();
	}
	_battleStream << Stream::eos;
}

void Leaderboard::update()
{
	doUpdate();
}

void Leaderboard::doUpdate()
{
    FastMutex::ScopedLock lk(_opMutex);
    UInt8 count;
    UInt8 index;
    std::unique_ptr<DB::DBExecutor> execu;
	execu.reset(DB::gObjectDBConnectionMgr->GetExecutor());
	if (execu.get() == NULL || !execu->isConnected()) return;

	++ _id;

	std::vector<LeaderboardItem> blist;

	std::vector<LeaderboardItem2> blist2;
	blist2.clear();
    _playerLevelRank.clear();
	execu->ExtractData("SELECT `player`.`id`, `player`.`name`, `fighter`.`level`, `player`.`country`, `fighter`.`experience`, `clan`.`name` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" ORDER BY `fighter`.`experience` DESC"
		" LIMIT 0, 100", blist2);
    _level.clear();
    for(UInt8 c = 0; c < blist2.size(); c++)
    {
        Player* curPlayer = globalPlayers[blist2[c].id];
        if(curPlayer && curPlayer->getClan())
        {
            curPlayer->patchMergedName(curPlayer->getClan()->getFounder(), blist2[c].clan);
        }
        if (curPlayer == NULL)
            continue;
        RankingInfoList r;
        r.id = curPlayer->getId();
        r.name = curPlayer->getName();
        r.country = curPlayer->getCountry();
        r.clanName = curPlayer->getClanName();
        r.roleLevel = curPlayer->GetLev();
        r.value = blist2[c].value;
        _level.push_back(r);

        _playerLevelRank[curPlayer->getId()] = c+1;
    }

    buildPacket2(_levelStream, 0, _id, blist2);
	if(!blist2.empty())
		_maxLevel = blist2[0].lvl;

	blist.clear();
    _athletics.clear();
	std::list<AthleticsRankData *> *pathleticsrank = gAthleticsRank.getAthleticsRank();
	std::list<AthleticsRankData *> athleticsrank = pathleticsrank[1];
	blist.resize(100);
    _playerAthleticsRank.clear();
	UInt32 i = 0;
	for(std::list<AthleticsRankData *>::iterator it = athleticsrank.begin(); it != athleticsrank.end(); ++ i, ++ it )
	{
        if (i < 100)
        {
		    blist[i].id = (*it)->ranker->getId();
	    	blist[i].name = (*it)->ranker->getName();
	    	blist[i].pf = (*it)->ranker->getPF();
	    	blist[i].lvl = (*it)->ranker->GetLev();
	    	blist[i].country = (*it)->ranker->getCountry();
	    	blist[i].value = i + 1;
	    	blist[i].clan = (*it)->ranker->getClanName();
           
            RankingInfoList r;
            r.id = (*it)->ranker->getId();
            r.country = (*it)->ranker->getCountry();
            r.name = (*it)->ranker->getName();
            r.clanName = (*it)->ranker->getClanName();
            r.roleLevel = (*it)->ranker->GetLev();
            r.value = i+1;
            _athletics.push_back(r);
        }
        _playerAthleticsRank[(*it)->ranker->getId()] = i+1;
	}
	blist.resize(i > 100 ? 100 : i);
	buildPacket(_moneyStream, 1, _id, blist);

	blist.clear();
    _achievement.clear();
	execu->ExtractData("SELECT `player`.`id`, `player`.`name`, `fighter`.`level`, `player`.`country`, `player`.`archievement`, `clan`.`name` FROM"
		" (`player` CROSS JOIN `fighter`"
		" ON `player`.`id` = `fighter`.`playerId` AND `fighter`.`id` < 10)"
		" LEFT JOIN (`clan_player`, `clan`)"
		" ON `player`.`id` = `clan_player`.`playerId` AND `clan_player`.`id` = `clan`.`id`"
		" ORDER BY `player`.`archievement` DESC"
		" LIMIT 0, 100", blist);
    count = static_cast<UInt8>(blist.size());
    for(index = 0; index < count; ++index)
    {
        if(cfg.merged)
            Player::patchMergedName(blist[index].id, blist[index].name);
    }
    for(UInt8 c = 0; c < blist.size(); c++)
    {
        Player* curPlayer = globalPlayers[blist[c].id];
        if(curPlayer && curPlayer->getClan())
        {
            curPlayer->patchMergedName(curPlayer->getClan()->getFounder(), blist[c].clan);

            RankingInfoList r;
            r.id = curPlayer->getId();
            r.name = curPlayer->getName();
            r.country = curPlayer->getCountry();
            r.clanName = curPlayer->getClanName();
            r.roleLevel = curPlayer->GetLev();
            r.value = blist[c].value;
            _achievement.push_back(r);
        }
    }
    buildPacket(_achievementStream, 2, _id, blist);

	blist.clear();
#if 0
	execu->ExtractData("SELECT `clan`.`id`, `player`.`name`, `clan`.`level`, `player`.`country`, COUNT(`clan_player`.`id`) AS `pcount`, `clan`.`name` FROM "
		"`clan`, `clan_player`, `player` "
		"WHERE `clan`.`id` = `clan_player`.`id` AND `clan`.`leader` = `player`.`id` "
		"GROUP BY `clan_player`.`id` ORDER BY `clan`.`level` DESC, `pcount` DESC LIMIT 0, 100", blist);
	buildPacket(_clanStream, 3, _id, blist);
#else
    i = 0;
    const std::vector<Clan*> clanRanking = ClanRankBattleMgr::Instance().getClanRanking();
    UInt32 size = clanRanking.size();
    if (size > 1000) size = 100;
	blist.resize(size);
    _playerClanRank.clear();
    for (std::vector<Clan*>::const_iterator it = clanRanking.begin(), e = clanRanking.end(); it != e; ++i, ++it)
    {
		//blist[i].id = (*it)->getId();
        Player* owner = (*it)->getOwner();
        if (owner)
        {
            blist[i].id = owner->getId();
            blist[i].name = owner->getName();
            blist[i].pf = owner->getPF();
            blist[i].country = owner->getCountry();
        }
        else
        {
            blist[i].id = 0;
            blist[i].name = "";
            blist[i].pf = 0;
            blist[i].country = 2;
        }
		blist[i].lvl = (*it)->getLev();
		blist[i].value = (*it)->getCount();
		blist[i].clan = (*it)->getName();
        _playerClanRank[(*it)->getId()] = i+1;
    }
	buildPacket(_clanStream, 3, _id, blist);
#endif

	std::vector<LeaderboardTowndown> blist3;
	execu->ExtractData("select a.id, a.name, b.maxLevel, b.time2MaxLvl from player a, towndeamon_player b where a.id=b.playerId order by b.maxLevel desc, b.time2MaxLvl asc limit 100;", blist3);
    {
        FastMutex::ScopedLock lk(_tmutex);
        _towndown.clear();
        _towndown.insert(_towndown.end(), blist3.begin(), blist3.end());

        _town.clear();
        for (int i = 0; i < blist3.size(); ++i)
        {
            Player * pl = GObject::globalPlayers[blist3[i].id];
            if (pl == NULL)
                continue;
            TownRankingInfoList r;
            r.id = blist3[i].id;
            r.name = blist3[i].name;
            r.country = pl->getCountry();
            r.clanName = pl->getClanName();
            r.roleLevel = pl->GetLev();
            r.value = blist3[i].level;
            r.reachTime = blist3[i].time;

            _town.push_back(r);
        }
    }

	std::vector<LeaderboardClanCopy> blist4;
	execu->ExtractData("select a.id, a.name, b.maxCopyLevel, b.maxCopyTime from clan a, clan_copy b where a.id=b.clanId order by b.maxCopyLevel desc, b.maxCopyTime  asc limit 100;", blist4);
    {
        FastMutex::ScopedLock lk(_cmutex);
        _clancopy.clear();
        _clancopy.insert(_clancopy.end(), blist4.begin(), blist4.end());

         _clanCopyInfo.clear(); 
        for (int i = 0; i < blist4.size(); ++i)
        {
            GObject::Clan * clan = GObject::globalClans[blist4[i].id];
            if (clan == NULL)
               continue;
            Player* leader = clan->getLeader();
            if(leader == NULL)
                continue;
            ClanCopyRankingInfoList r;
            r.name = blist4[i].name;
            r.leaderName = leader->getName();
            r.level = clan->getLev();
            r.memberCount = clan->getCount();
            r.country = clan->getCountry();
            r.value = blist4[i].level;
            r.reachTime = blist4[i].time;

            _clanCopyInfo.push_back(r);
        }
    }

	std::vector<UInt64> ilist;
	size_t cnt;

	_levelRankWorld.clear();
	_levelRankWorld10.clear();
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
        if(i < 10)
            _levelRankWorld10.push_back(ilist[i]);
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

    const std::vector<Clan*>& clanRanking0 = ClanRankBattleMgr::Instance().getClanRanking();
    cnt = clanRanking0.size() ;
    if (cnt > 100) cnt = 100;
    _clanBattleInfo.clear();
	for(size_t i = 0; i < cnt; ++ i)
	{
       // _clanRankWorld[clist[i]] = static_cast<UInt16>(i + 1);

        GObject::Clan * clan = clanRanking0[i];
        if (clan == NULL)
            continue;
        Player* leader = clan->getLeader();
        if(leader == NULL)
            continue;
        ClanBattleRankingInfoList r;
        r.name = clan->getName();
        r.leaderName = leader->getName();
        r.level = clan->getLev();
        r.memberCount = clan->getCount();
        r.country = clan->getCountry();
        r.value = clan->GetBattleScore();

        _clanBattleInfo.push_back(r);
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

    buildBattlePacket();
}

bool Leaderboard::hasUpdate( UInt32 id )
{
	return _id != id;
}

bool Leaderboard::getPacket( UInt8 t, Stream*& st, Player* pl)
{
    if (isSorting())
        return false;

    FastMutex::ScopedLock lk(_opMutex);
	switch(t)
	{
	case 0:
		st = &_levelStream;
        makeRankStream(st, t, pl);
		break;
	case 1:
		st = &_moneyStream;
        makeRankStream(st, t, pl);
		break;
	case 2:
		st = &_achievementStream;
		break;
	case 3:
		st = &_clanStream;
        makeRankStream(st, t, pl);
		break;
    case 4:
        st = &_battleStream;
        makeRankStream(st, t, pl);
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
    if (isSorting())
        return;
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

        if(country > 1)
            return;

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

void Leaderboard::newDrawingGame(UInt32 nextday)
{
    if(cfg.openYear <= 2011)
        return;
    UInt32 opTime = TimeUtil::MkTime(cfg.openYear, cfg.openMonth, cfg.openDay);

    if(TimeUtil::SharpDay(0, nextday) == opTime + 7 * 86400 )
    {
        UInt16 newAward[] = { 1000,800,600,400,200,100,100,100,100,100 };
        //新人冲级赛 等级前十 送礼券
        for(UInt16 rank = 0; rank < _levelRankWorld10.size(); ++rank){
            Player * pl = GObject::globalPlayers[_levelRankWorld10[rank]];
            if(NULL == pl)
                continue;
            SYSMSGV(title, 4018);
            SYSMSGV(content, 4019, rank + 1, newAward[rank]);
            Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(NULL == mail)
                continue;
            MailPackage::MailItem mitem[1] = { {GObject::MailPackage::Coupon, newAward[rank]} };
            mailPackageManager.push(mail->id, mitem, 1, true);
            std::string strItems;
            for(int i = 0; i < 1; ++i){
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, _levelRankWorld10[rank], mail->id, NewDrawingGameAward, title, content, strItems.c_str(), mail->recvTime);
        }
    }
}

int Leaderboard::getMyRank(Player* pl, UInt8 type)
{
    int rank = 0;
    if (NULL == pl)
        return 0;
    std::map<UInt64, int>::iterator iter;
    Clan* cl = pl->getClan(); 
    switch (type)
    {
        case 0:
            iter = _playerLevelRank.find(pl->getId());
            if (_playerLevelRank.end() != iter)
                rank = iter->second;
            break;
        case 1:
            iter = _playerAthleticsRank.find(pl->getId());
            if (_playerAthleticsRank.end() != iter)
                rank = iter->second;
            break;
        case 2:
            break;
        case 3:
           if (NULL != cl)
           {
               iter = _playerClanRank.find(cl->getId());
               if (_playerClanRank.end() != iter)
                   rank = iter->second;
           }
           break;
        case 4:
            iter = _playerBattleRank.find(pl->getId());
            if (_playerBattleRank.end() != iter)
            {
                rank = iter->second;
            }
            break;
        default:
            break;
    }
    return rank;   
}
void Leaderboard::makeRankStream(Stream*& st, UInt8 type, Player* pl)
{
    int rank = getMyRank(pl, type);
    st->pop_front(9); //将type,rank总共5个字节删除
    st->prepend((UInt8*)&rank, 4); //头先插入自己的排行
    st->prepend((UInt8*)&type, 1);    //最后插入类型
    UInt16 len = st->size();
    UInt8 buf[4] = {0, 0, 0xFF, REP::SORT_LIST};
    memcpy(buf, &len, 2);
    st->prepend(buf, 4);
}

}

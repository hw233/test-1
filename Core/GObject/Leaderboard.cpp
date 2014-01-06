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
#include "DaysRank.h"
#include "Common/StringTokenizer.h"
#include "FairyPet.h"

namespace GObject
{
#define ONLY_BATTLE_POINT_RANK true

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

SPECIALBEGIN(GObject::LeaderboardLingbao)
SPECIALDEF(10)
(
    UInt64, id,
    std::string, name,
    UInt32, itemId,
    UInt8, tongling,
    UInt8, lbColor,
    std::string, types,
    std::string, values,
    std::string, skills,
    std::string, factors,
    UInt32, battlePoint
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

/*
void buildPacketForLingbao(Stream& st, UInt8 t, UInt32 id, std::vector<LingbaoInfoList>& list, bool merge = true)
{
	UInt8 c = static_cast<UInt8>(list.size());
	st.init(REP::SORT_LIST);
	st << t << id << static_cast<UInt32>(0) << c;
	for(UInt8 i = 0; i < c; ++ i)
	{
		LingbaoInfoList& item = list[i];
		if(merge)
			Player::patchMergedName(item.id, item.name);
        st << item.name << item.pf << item.country << item.battlePoint << static_cast<UInt16>(item.itemId) << item.tongling << item.lbColor;
        for (UInt8 i = 0; i < 4; ++i)
        {
            st << item.type[i] << item.value[i];
        }
        st << item.skill[0] << item.factor[0] << item.skill[1] << item.factor[1];
	}
	st << Stream::eos;
}
*/

void Leaderboard::buildPacketForLingbao(Stream& st, UInt8 t, bool merge /* = true */)
{
    FastMutex::ScopedLock lk(_lbMutex);
    _lingbaoRank.clear();

	UInt32 c = static_cast<UInt32>(_lingbaoInfoSet.size());
    TRACE_LOG("_lingbaoInfoSet.size() : %u.", static_cast<UInt32>(c));
	st.init(REP::SORT_LIST);
    if(c > 100)
        c = 100;
	st << t << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt8>(c);
	//for(UInt8 i = 0; i < c; ++ i)
    UInt32 i = 0;
    for (LingbaoInfoSet::iterator it = _lingbaoInfoSet.begin(); it != _lingbaoInfoSet.end(); ++ it)
	{
		const LingbaoInfoList& item = *it;
        if ((_lingbaoRank[item.id] == 0) || (_lingbaoRank[item.id] > static_cast<int>(i+1)))
            _lingbaoRank[item.id] = i+1;

        if(i < c)
        {
            st << item.name << item.pf << item.country << item.battlePoint << static_cast<UInt16>(item.itemId) << item.tongling << item.lbColor;
            for (UInt8 j = 0; j < 4; ++j)
            {
                st << item.type[j] << item.value[j];
            }
            st << item.skill[0] << item.factor[0] << item.skill[1] << item.factor[1];
        }
        ++ i;
	}
	st << Stream::eos;
}

void Leaderboard::buildBattlePacket()
{
    _battleRankWorld.clear();
    _expRankWorld.clear();
    std::unordered_map<UInt64, Player*>& pm = GObject::globalPlayers.getMap();
    std::unordered_map<UInt64, Player*>::iterator iter;
    for (iter = pm.begin(); iter != pm.end(); ++iter)
    {
        Player* pl = iter->second;
        if (NULL != pl)
        {
            UInt32 battlePoint = pl->getBattlePoint();
#if ONLY_BATTLE_POINT_RANK
            UInt32 battlePointOld = pl->GetVar(VAR_TOTAL_BATTLE_POINT);
            if(battlePoint == 0)
                battlePoint = battlePointOld;
            else
            {
                if(battlePoint != battlePointOld)
                    pl->SetVar(VAR_TOTAL_BATTLE_POINT, battlePoint);
            }
#endif
            _battleRankWorld.insert(std::make_pair(battlePoint, pl));
            _expRankWorld.insert(std::make_pair(pl->GetExp(), pl));
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
    //等级排行榜
    _playerLevelRank.clear();
    i = 0;
    std::multimap<UInt64, Player*, std::greater<UInt64> >::iterator iter3;
    for (iter3 = _expRankWorld.begin(); iter3 != _expRankWorld.end(); ++iter3)
    {
        i++;
        Player* pl = iter3->second;  
        if (pl)
            _playerLevelRank[pl->getId()] =  i; 
	}
}

void Leaderboard::update()
{
	doUpdate();
    GObject::DaysRank::instance().process(false);
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
    //_playerLevelRank.clear();
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

        //_playerLevelRank[curPlayer->getId()] = c+1;
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
        for (UInt32 i = 0; i < blist3.size(); ++i)
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
	execu->ExtractData("select a.id, a.name, b.maxCopyLevel, b.maxCopyTime from clan a, clan_copy b where a.id=b.clanId and b.maxCopyLevel > 0 order by b.maxCopyLevel desc, b.maxCopyTime  asc limit 100;", blist4);
    {
        FastMutex::ScopedLock lk(_cmutex);
        _clancopy.clear();
        _clancopy.insert(_clancopy.end(), blist4.begin(), blist4.end());

         _clanCopyInfo.clear(); 
	    blist.resize(100);
        if (blist4.size() < 100)
            blist.resize(blist4.size());
        _playerClanCopyRank.clear();
        for (UInt32 i = 0; i < blist4.size(); ++i)
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

            //帮派副本排行榜
            blist[i].id = clan->getId();
            blist[i].name = leader->getName();
            blist[i].pf = leader->getPF();
            blist[i].country = clan->getCountry();
		    blist[i].lvl = clan->getLev();
		    blist[i].value = blist4[i].level;
		    blist[i].clan = clan->getName();
            _playerClanCopyRank[clan->getId()] = i+1;
        }
	    buildPacket(_clanCopyStream, 5, 0, blist);
    }
    /*

    std::vector<LeaderboardLingbao> blist5;
	execu->ExtractData("select p.id, p.name, e.itemId, l.tongling, l.lbcolor, l.types, l.values, l.skills, l.factors, l.battlepoint from player p, fighter f, equipment e, lingbaoattr l "
            "where p.id=f.playerId and e.id = l.id and (f.lingbao REGEXP concat(',',l.id, '$') or f.lingbao REGEXP concat('^', l.id, ',') or f.lingbao REGEXP concat(',', l.id, ',')) order by l.battlepoint DESC limit 0, 100;", blist5);
    {
        FastMutex::ScopedLock lk(_cmutex);
         _lingbaoInfoSet.clear(); 
	    blist.resize(100);
        if (blist5.size() < 100)
            blist.resize(blist5.size());
        _lingbaoRank.clear();
        for (UInt32 i = 0; i < blist5.size(); ++i)
        {
            LingbaoInfoList r;
            r.id = blist5[i].id;
            Player * pl = GObject::globalPlayers[r.id];
            if(NULL == pl)
                continue;
            r.pf = pl->getPF();
            r.country = pl->getCountry();
            r.name = blist5[i].name;
            r.itemId = blist5[i].itemId;
            r.tongling = blist5[i].tongling;
            r.lbColor = blist5[i].lbColor;
            StringTokenizer tk(blist5[i].types, ",");
            if (tk.count())
            {
                for (size_t i = 0; i < tk.count(); ++i)
                {
                    if(i > 3)
                        break;
                    r.type[i] = ::atoi(tk[i].c_str());
                }
            }
            StringTokenizer tk2(blist5[i].values, ",");
            if (tk2.count())
            {
                for (size_t i = 0; i < tk2.count(); ++i)
                {
                    if(i > 3)
                        break;
                    r.value[i] = ::atoi(tk2[i].c_str());
                }
            }
            StringTokenizer tk3(blist5[i].skills, ",");
            if (tk3.count())
            {
                for (size_t i = 0; i < tk3.count(); ++i)
                {
                    if(i > 1)
                        break;
                    r.skill[i] = ::atoi(tk3[i].c_str());
                }
            }
            StringTokenizer tk4(blist5[i].factors, ",");
            if (tk4.count())
            {
                for (size_t i = 0; i < tk4.count(); ++i)
                {
                    if(i > 1)
                        break;
                    r.factor[i] = ::atoi(tk4[i].c_str());
                }
            }

            r.battlePoint = blist5[i].battlePoint;

            _lingbaoInfoSet.push_back(r);

            if ((_lingbaoRank[r.id] == 0) || (_lingbaoRank[r.id] > static_cast<int>(i+1)))
                _lingbaoRank[r.id] = i+1;
        }
	    buildPacketForLingbao(_lingbaoStream, 6, _id, _lingbaoInfoSet);
    }
    */


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
    buildPacketForLingbao(_lingbaoStream, 6);
    buildPacketForPet();
}

bool Leaderboard::hasUpdate( UInt32 id )
{
	return _id != id;
}

bool Leaderboard::getPacket( UInt8 t, Stream& st, Player* pl)
{
    // XXX: t == 7 的情况不在此处获得排行榜数据，因为是实时更新的前八名数据
    if (isSorting())
        return false;
    FastMutex::ScopedLock lk(_opMutex);
	switch(t)
	{
	case 0:
		st << std::vector<UInt8>(_levelStream) << Stream::eos;
        makeRankStream(&st, t, pl);
		break;
	case 1:
		st << std::vector<UInt8>(_moneyStream) << Stream::eos;
        makeRankStream(&st, t, pl);
		break;
	case 2:
		st << std::vector<UInt8>(_achievementStream) << Stream::eos;
		break;
	case 3:
		st << std::vector<UInt8>(_clanStream) << Stream::eos;
        makeRankStream(&st, t, pl);
		break;
    case 4:
        st << std::vector<UInt8>(_battleStream) << Stream::eos;
        makeRankStream(&st, t, pl);
        break;
    case 5:
        st << std::vector<UInt8>(_clanCopyStream) << Stream::eos;
        makeRankStream(&st, t, pl);
        break;
    case 6:
        st << std::vector<UInt8>(_lingbaoStream) << Stream::eos;
        makeRankAndValueStream(&st, t, pl, pl->getMaxLingbaoBattlePoint());
        break;
    case 8:
        st << std::vector<UInt8>(_petStream) << Stream::eos;
        makeRankAndValueStream(&st, t, pl, pl->getMaxPetBattlePoint());
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
    bool after_20130531 = (TimeUtil::MkTime(2013, 5, 31) <= opTime);
    UInt32 days = 7;
    if (cfg.rpServer || after_20130531)
        days = 30;
    if(TimeUtil::SharpDay(0, nextday) == opTime + days * 86400 )
    {
        FastMutex::ScopedLock lk(_opMutex);
        UInt16 newAward[] = { 1000,800,600,400,200,100,100,100,100,100 };
        UInt16 newAwardRP[] = { 1000,800,600,300,300,300,300,300,300,300 };
        UInt16* pAward = newAward;
        if (cfg.rpServer || after_20130531)
            pAward = newAwardRP;
        //新人冲级赛 等级前十 送礼券
        for(UInt16 rank = 0; rank < _levelRankWorld10.size(); ++rank){
            Player * pl = GObject::globalPlayers[_levelRankWorld10[rank]];
            if(NULL == pl)
                continue;
            SYSMSGV(title, 4018);
            SYSMSGV(content, 4019, rank + 1, pAward[rank]);
            Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(NULL == mail)
                continue;
            MailPackage::MailItem mitem[1] = { {GObject::MailPackage::Coupon, pAward[rank]} };
            mailPackageManager.push(mail->id, mitem, 1, true);
            std::string strItems;
            for(int i = 0; i < 1; ++i){
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, _levelRankWorld10[rank], mail->id, NewDrawingGameAward, title, content, strItems.c_str(), mail->recvTime);
        }
    }
    //回流服务器战斗力排行
    //if(cfg.rpServer && TimeUtil::SharpDay(0, nextday) == opTime + 30 * 86400)
    if(TimeUtil::SharpDay(0, nextday) == opTime + 30 * 86400)
    {
        FastMutex::ScopedLock lk(_opMutex);
        buildBattlePacket();
        static  MailPackage::MailItem s_item[10][6] = {
            {{0xA000,1000},{9367,20},{9369,20},{134,20},{50,20}},
            {{0xA000,800},{9367,10},{9369,10},{134,10},{50,10}},
            {{0xA000,600},{9367,8},{9369,8},{134,8},{50,8}},
            {{0xA000,300},{9367,3},{9369,3},{134,3},{50,3}},
            {{0xA000,300},{9367,3},{9369,3},{134,3},{50,3}},
            {{0xA000,300},{9367,3},{9369,3},{134,3},{50,3}},
            {{0xA000,300},{9367,3},{9369,3},{134,3},{50,3}},
            {{0xA000,300},{9367,3},{9369,3},{134,3},{50,3}},
            {{0xA000,300},{9367,3},{9369,3},{134,3},{50,3}},
            {{0xA000,300},{9367,3},{9369,3},{134,3},{50,3}}
        };
        int rank = 0;
        std::multimap<int, Player*, std::greater<int> >::iterator iter1;
        for (iter1 = _battleRankWorld.begin(); iter1 != _battleRankWorld.end(); ++iter1)
        {
            rank++;
            Player* pl = iter1->second;
            if (pl)
            {
                SYSMSG(title, 4900);
                SYSMSGV(content, 4901, rank);
                Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                if(mail)
                {
                    mailPackageManager.push(mail->id, s_item[rank-1], 5, true);
                }
            }
            if (rank >= 10)
                break;
	    }
    }
    //回流服务器充值排行
    //if(cfg.rpServer && TimeUtil::SharpDay(0, nextday) == opTime + 7 * 86400 )
    if(TimeUtil::SharpDay(0, nextday) == opTime + 7 * 86400 )
        World::SendRechargeRP7RankAward();

}

int Leaderboard::getMyRank(Player* pl, UInt8 type , bool setLock)
{
    int rank = 0;
    if (NULL == pl)
        return 0;
    if( setLock == true )
        FastMutex::ScopedLock lk(_opMutex);

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
        case 5:
           if (NULL != cl)
           {
               iter = _playerClanCopyRank.find(cl->getId());
               if (_playerClanCopyRank.end() != iter)
                   rank = iter->second;
           }
           break;
        case 6:
            iter = _lingbaoRank.find(pl->getId());
            if (_lingbaoRank.end() != iter)
                rank = iter->second;
            break;
        case 8:
            iter = _petRank.find(pl->getId());
            if (_petRank.end() != iter)
                rank = iter->second;
            break;
      
        default:
            break;
    }
    return rank;   
}
void Leaderboard::makeRankStream(Stream* st, UInt8 type, Player* pl)
{
    int rank = getMyRank(pl, type,false);
    st->pop_front(9); //将type,rank总共5个字节删除
    st->prepend((UInt8*)&rank, 4); //头先插入自己的排行
    st->prepend((UInt8*)&type, 1);    //最后插入类型
    UInt16 len = st->size();
    UInt8 buf[4] = {0, 0, 0xFF, REP::SORT_LIST};
    memcpy(buf, &len, 2);
    st->prepend(buf, 4);
}

void Leaderboard::makeRankAndValueStream(Stream* st, UInt8 type, Player* pl, UInt32 value)
{
    int rank = getMyRank(pl, type,false);
    st->pop_front(13); //将type,rank总共5个字节删除
    st->prepend((UInt8*)&value, 4); // 先插入数据（比如宝具的战斗力）
    st->prepend((UInt8*)&rank, 4); //头先插入自己的排行
    st->prepend((UInt8*)&type, 1);    //最后插入类型
    UInt16 len = st->size();
    UInt8 buf[4] = {0, 0, 0xFF, REP::SORT_LIST};
    memcpy(buf, &len, 2);
    st->prepend(buf, 4);
}

void Leaderboard::pushLingbaoInfo(LingbaoInfoList lingbaoInfo)
{
    FastMutex::ScopedLock lk(_lbMutex);
    _lingbaoInfoSet.insert(lingbaoInfo);
}

void Leaderboard::eraseLingbaoInfo(LingbaoInfoList lingbaoInfo)
{
    FastMutex::ScopedLock lk(_lbMutex);
    /*
    LingbaoInfoSet::iterator it = _lingbaoInfoSet.find(lingbaoInfo);
    if(it != _lingbaoInfoSet.end())
    */
    _lingbaoInfoSet.erase(lingbaoInfo);
}

void Leaderboard::buildPacketForPet()
{
    FastMutex::ScopedLock lk(_petMutex);
    _petRank.clear();
    Stream& st = _petStream;

	UInt32 c = static_cast<UInt32>(_petInfoSet.size());
    TRACE_LOG("_petInfoSet.size() : %u.", static_cast<UInt32>(c));
	st.init(REP::SORT_LIST);
    if(c > 100)
        c = 100;
	st << static_cast<UInt8>(8) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt8>(c);
	//for(UInt8 i = 0; i < c; ++ i)
    UInt32 i = 0;
    for (PetInfoSet::iterator it = _petInfoSet.begin(); it != _petInfoSet.end(); ++ it)
	{
		const PetInfoList& item = *it;
        if ((_petRank[item.id] == 0) || (_petRank[item.id] > static_cast<int>(i+1)))
            _petRank[item.id] = i+1;

        if(i < c)
        {
            st << item.name << item.pf << item.country << item.battlePoint << static_cast<UInt16>(item.petId) << item.petLev << item.gengu;
            for (UInt8 j = 0; j < 3; ++j)
            {
                st << item.neidan[j].itemId << item.neidan[j].lv << item.neidan[j].skill;
                for(UInt8 k = 0; k < 4; ++ k)
                {
                    st << item.neidan[j].gems[k];
                }
            }
            st << static_cast<UInt8>(item.sanhun.size());
            std::map<UInt8, UInt8>::const_iterator iter = item.sanhun.begin();
            for(; iter != item.sanhun.end(); ++iter)
                st << iter->first << iter->second;
        }
        ++ i;
	}
	st << Stream::eos;
}

void Leaderboard::pushPetInfo(FairyPet* pet)
{
    if(!pet)
        return;

    FastMutex::ScopedLock lk(_petMutex);
    Player* pl = pet->getOwner();
    if(!pl)
        return;

    PetInfoSetMap::iterator mapit = _petInfoSetMap.find(pl);
    while(mapit != _petInfoSetMap.end())
    {
        if(mapit->first != pl)
            break;
        PetInfoSetIt setit = mapit->second;
        const PetInfoList& petInfo = *setit;
        if(petInfo.petId == pet->getId())
        {
            _petInfoSet.erase(petInfo);
            _petInfoSetMap.erase(mapit++);
            break;
        }
        ++ mapit;
    }

    PetInfoList petInfo;
    petInfo.id = pl->getId();
    petInfo.name = pl->getName();
    petInfo.pf = pl->getPF();
    petInfo.country = pl->getCountry();
    petInfo.battlePoint = pet->getBattlePoint();
    petInfo.petId = pet->getId();
    petInfo.petLev = pet->getPetLev();
    petInfo.gengu = pet->getPetBone();
    for(UInt8 i = 0; i < PET_EQUIP_UPMAX; ++ i)
    {
        ItemPetEq* neidan = pet->findEquip(i);
        if(!neidan)
            continue;
        petInfo.neidan[i].itemId = neidan->GetItemType().getId();
        petInfo.neidan[i].lv = neidan->getPetEqAttr().lv;
        petInfo.neidan[i].skill = neidan->getPetEqAttr().skill;
        for(int j = 0; j < 4; ++ j)
        {
            petInfo.neidan[i].gems[j] = neidan->getPetEqAttr().gems[j];
        }
    }
    petInfo.sanhun = pet->getSanhun();

    _petInfoSet.insert(petInfo);
    _petInfoSetMap.insert(std::make_pair(pl, _petInfoSet.find(petInfo)));
}

void Leaderboard::erasePetInfo(FairyPet* pet)
{
    if(!pet)
        return;

    FastMutex::ScopedLock lk(_petMutex);
    Player* pl = pet->getOwner();
    if(!pl)
        return;

    PetInfoSetMap::iterator mapit = _petInfoSetMap.find(pl);
    while(mapit != _petInfoSetMap.end())
    {
        if(mapit->first != pl)
            break;
        PetInfoSetIt setit = mapit->second;
        const PetInfoList& petInfo = *setit;
        if(petInfo.petId == pet->getId())
        {
            _petInfoSet.erase(petInfo);
            _petInfoSetMap.erase(mapit++);
            break;
        }
        ++ mapit;
    }
}

void Leaderboard::readRechargeRank100(BinaryReader& brd)
{
    _rechargeRank100.clear();
	UInt8 size = 0;
    brd >> size;
    for(UInt32 i = 0; i < size && i < 100; ++ i)
    {
        AllServersRecharge asrData;
        UInt64 playerId = 0;
        brd >> playerId >> asrData.name >> asrData.total;

        Player * player = globalPlayers[playerId];
        asrData.player = player;
        asrData.rank = i+1;
        _rechargeRank100.push_back(asrData);
    }
}

void Leaderboard::readRechargeSelf(BinaryReader& brd)
{
    _rechargeSelf.clear();
	UInt32 size = 0;
    brd >> size;
    for(UInt32 i = 0; i < size; ++ i)
    {
        AllServersRecharge asrData;
        UInt64 playerId = 0;
        brd >> playerId >> asrData.total >> asrData.rank;

        Player * player = globalPlayers[playerId];
        asrData.player = player;
        asrData.name = player ? player->getName() : "";
        _rechargeSelf.insert(std::make_pair(playerId, asrData));
    }
}

void Leaderboard::sendMyRechargeRank(Player * player)
{
    if(!player) return;

    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(2) << static_cast<UInt8>(6) << static_cast<UInt8>(1);
    std::map<UInt64, AllServersRecharge>::iterator it = _rechargeSelf.find(player->getId());
    if(it != _rechargeSelf.end())
        st << it->second.total << it->second.rank;
    else
        st << static_cast<UInt32>(0) << static_cast<UInt32>(0);
    st << Stream::eos;
    player->send(st);
}

void Leaderboard::sendRechargeRank100(Player * player, UInt8 idx, UInt8 cnt)
{
    if(!player) return;

    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(2) << static_cast<UInt8>(6) << static_cast<UInt8>(0);
    UInt8 size = _rechargeRank100.size();
	if(idx >= size)
	{
		idx = size;
		cnt = 0;
	}
	else
	{
		if(idx + cnt > size)
			cnt = size - idx;
	}
	st << size << idx << cnt;

    std::vector<AllServersRecharge>::iterator it = _rechargeRank100.begin();
    for(UInt8 i = 0; i < cnt; ++ i)
    {
        std::advance(it, idx+i);
        st << (*it).name << (*it).total << (*it).rank;
    }
    st << Stream::eos;
    player->send(st);
}

void Leaderboard::giveRechargeRankAward()
{
    SYSMSGV(title1, 827);
    SYSMSGV(title2, 829);
    std::string name10[10];
    UInt32 total10[10] = {0};
    std::vector<AllServersRecharge>::iterator it;
    for(it = _rechargeRank100.begin(); it != _rechargeRank100.end(); ++ it)
    {
        if((*it).rank && (*it).rank <= 10)
        {
            name10[(*it).rank-1] = (*it).name;
            total10[(*it).rank-1] = (*it).total;
        }

        Player * player = (*it).player;
        if(!player)
            continue;

        UInt16 count1 = 0, count2 = 0, count3 = 0, count4 = 0;
        if((*it).rank == 1)
        {
            count1 = 200; count2 = 200;
            count3 = 100; count4 = 100;
        }
        else if((*it).rank == 2)
        {
            count1 = 180; count2 = 180;
            count3 = 88; count4 = 88;
        }
        else if((*it).rank == 3)
        {
            count1 = 160; count2 = 160;
            count3 = 66;  count4 = 66;
        }
        else if((*it).rank >= 4 && (*it).rank <= 10)
        {
            count1 = 140; count2 = 140;
            count3 = 50;  count4 = 50;
        }
        else if((*it).rank >= 11 && (*it).rank <= 20)
        {
            count1 = 100; count2 = 100;
            count3 = 40;  count4 = 40;
        }
        else if((*it).rank >= 21 && (*it).rank <= 40)
        {
            count1 = 80; count2 = 80;
            count3 = 30; count4 = 30;
        }
        else if((*it).rank >= 41 && (*it).rank <= 60)
        {
            count1 = 60; count2 = 60;
            count3 = 25; count4 = 25;
        }
        else if((*it).rank >= 61 && (*it).rank <= 80)
        {
            count1 = 50; count2 = 50;
            count3 = 20; count4 = 20;
        }
        else if((*it).rank >= 81 && (*it).rank <= 100)
        {
            count1 = 25; count2 = 25;
            count3 = 10; count4 = 10;
        }
        else
            continue;
        SYSMSGV(content2, 830, player->getCountry(), player->getName().c_str(), (*it).rank, (*it).total);
        MailPackage::MailItem item[] = {{9418, count1}, {9438, count2}, {9022, count3}, {9075, count4},};
        MailItemsInfo itemsInfo(item, Activity, 4);
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title2, content2, 0xFFFE0000, true, &itemsInfo);
        if(mail)
             mailPackageManager.push(mail->id, item, 4, true);
        if((*it).rank == 1)
        {
            SYSMSGV(title, 833);
            SYSMSGV(content, 834, player->getCountry(), player->getName().c_str());
            Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title, content);
            if(mail)
                mail->writeMailLog(player, Activity);
        }
    }

    std::map<UInt64, AllServersRecharge>::iterator iter;
    for(iter = _rechargeSelf.begin(); iter != _rechargeSelf.end(); ++ iter)
    {
        Player * player = iter->second.player;
        if(!player)
            continue;
        SYSMSGV(content1, 828, player->getCountry(), player->getName().c_str(), iter->second.total, iter->second.rank, name10[0].c_str(), total10[0],
                name10[1].c_str(), total10[1], name10[2].c_str(), total10[2], name10[3].c_str(), total10[3], name10[4].c_str(), total10[4],
                name10[5].c_str(), total10[5], name10[6].c_str(), total10[6], name10[7].c_str(), total10[7], name10[8].c_str(), total10[8],
                name10[9].c_str(), total10[9]);
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x01, title1, content1);
        if(mail)
            mail->writeMailLog(player, Activity);
    }

    _rechargeSelf.clear();
    _rechargeRank100.clear();
}

void Leaderboard::sendGoldLvlAward(BinaryReader& brd)
{
    return;     //测试时没有
    UInt64 playerId = 0;
    UInt8 cnt = 0;
    brd >> playerId >> cnt;
    Player * player = globalPlayers[playerId];
    if(!player || !cnt)
        return;
    static const UInt32 goldLvls[] = { 8888, 18888, 38888, 88888, 188888, 288888 };
    static MailPackage::MailItem s_items[][4] = {
        { {134, 5},   {9438, 5},  {0, 0},      {0, 0} },
        { {1325, 10}, {134, 10},  {1126, 30},  {0, 0} },
        { {515, 15},  {501, 15},  {9338, 15},  {503, 15} },
        { {9076, 30}, {509, 30},  {1126, 30},  {9424, 30} },
        { {9022, 30}, {9075, 30}, {9068, 30},  {1126, 30} },
        { {9022, 40}, {9076, 30}, {9418, 100}, {8556, 10} },
    };
    SYSMSGV(title, 831);
    for(UInt8 i = 0; i < cnt; ++ i)
    {
        UInt8 idx = 0xFF;
        brd >> idx;
        if(idx >= sizeof(goldLvls)/sizeof(goldLvls[0]))
            continue;
        SYSMSGV(content, 832, player->getCountry(), player->getName().c_str(), goldLvls[idx]);
        MailItemsInfo itemsInfo(s_items[idx], Activity, 4);
        Mail * mail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
        if(mail)
             mailPackageManager.push(mail->id, s_items[idx], 4, true);
    }
}

}

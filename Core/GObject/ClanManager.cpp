#include "Config.h"
#include "ClanManager.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Clan.h"
#include "ClanBattle.h"
#include "Package.h"
#include "Battle/BattleSimulator.h"
#include "Common/TimeUtil.h"

namespace GObject 
{

ClanManager clanManager;

ClanManager::ClanManager() : /*_hasAllocRepo(false),*/_firstBroadcast(false), _hasBroadcast(0xFF), _clanBattle(NULL), _clanRobBattle(NULL)
{
	_clanRobBattle = new ClanRobBattle();
}

ClanManager::~ClanManager()
{
	delete _clanRobBattle;
	_clanRobBattle = NULL;
}

std::vector<UInt8>& ClanManager::getClanBattleTimeTable()
{
	static UInt8 clanBattleTimeTable[] = { 25, 30, 43 };
	static std::vector<UInt8> cbTT(clanBattleTimeTable, clanBattleTimeTable+3);
	return cbTT;
}

bool ClanManager::validClanBattleTime(UInt8 cbt)
{
	std::vector<UInt8>& cbTT = getClanBattleTimeTable();
	return std::find(cbTT.begin(), cbTT.end(), cbt) != cbTT.end();
}

void ClanManager::resumeRobClanBattleData()
{
	ClanRobBattle * crb = static_cast<ClanRobBattle *>(clanManager.getRobBattleClan());
	crb->resumeClanBattleData(TimeUtil::Now());	
}

void ClanManager::resumeRobClanBattleScore()
{
	ClanRobBattle * crb = static_cast<ClanRobBattle *>(clanManager.getRobBattleClan());
	crb->resumeBattleScore();
}

void ClanManager::process(UInt32 now, UInt32 today)
{
	for (OnBattleClanT::iterator onBcIter = _onBattleClans.begin(); onBcIter != _onBattleClans.end();)
	{
		_clanBattle = onBcIter->second;
		switch(_clanBattle->testClanBattleStatus(now, today))
		{
		case 1:
			_clanBattle->update(now);
			++ onBcIter;
			break;
		case 2:
			_clanRobBattle->initClanBattle();
			_clanBattle->startClanBattle();
			_clanBattle->setClanBattlerStartData();	
			++ onBcIter;
			break;
		case 3:
			_clanBattle->overClanBattle(1);
			_clanBattle->clearClanBattle();
		case 4:
			addOffBattleClan(_clanBattle);
			_onBattleClans.erase(onBcIter ++);
			//if (!_hasAllocRepo && _onBattleClans.empty())
			//{
			//	allocRepo();
			//	_hasAllocRepo = true;
			//}
			break;
		default:
			++ onBcIter;
		}
	}
	checkCBBroadcast(now);
}

void ClanManager::makeBattleClanInfo(Player * player, Stream& st, ClanBattle * clanBattle)
{
	UInt8 attack = 0;
	if (clanBattle != NULL)
	{
		if (clanBattle->isInBattling() || clanBattle->getOwnerClan() == player->getClan()) attack = 0x10;
		if (clanBattle->isInAttacking()) attack |= 0x01;
		st << clanBattle->getOwnerClanId() << clanBattle->getOwnerClanName() << clanBattle->getOwnerClanLev() << attack << clanBattle->getBattleTime() << clanBattle->getTotalAtkerNum() << clanBattle->getTotalDeferNum() << clanBattle->getWillGainBattlerAward(player->getClan());	
	}
	else
	{
		st << static_cast<UInt32>(0) << "" << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt16>(0) << static_cast<UInt16>(0);
	}
}

void ClanManager::listClans(Player * player, UInt16 start, UInt8 count)
{
	Clan * self = player->getClan();
	if (self == NULL) return;

	Clan * allyClan = self->getAllyClan();
	UInt16 allyClansSize = static_cast<UInt16>(allyClan != NULL ? 1 : 0);
	std::vector<Clan *> enemyClans = self->getEnemyClans();
	UInt16 enemyClansSize = static_cast<UInt16>(enemyClans.size());
	UInt16 onlineClansSize = static_cast<UInt16>(_onBattleClans.size());
	UInt16 offlineClanSize = static_cast<UInt16>(_offBattleClans.size());
	UInt16 sz = onlineClansSize + offlineClanSize + allyClansSize + enemyClansSize + 1;
	UInt16 end = start + count;
	if (end > sz)
		end = sz;
	if (end < start)
		count = 0;
	else
		count = end - start;
	Stream st(0x7A);
	st << sz << start << count;

	{
		if (count > 0 && start < 1)
		{
			makeBattleClanInfo(player, st, _clanRobBattle);	
		}
	}

	if (start >= 1)
		start -= 1;
	else if (count > 0)
		start = 0;
	{
		if (count >0 && start < allyClansSize && allyClan != NULL)
		{
			makeBattleClanInfo(player, st, allyClan->getClanBattle());
		}
	}

	if (start >= allyClansSize)
		start -= allyClansSize;
	else if (count > 0)
		start = 0;
	{
		if (count > 0 && start < enemyClansSize)
		{
			std::vector<Clan *>::iterator offset;
			offset = enemyClans.begin();
			std::advance(offset, start);
			for (; offset != enemyClans.end() && count > 0; ++ offset, -- count)
				makeBattleClanInfo(player, st, (*offset)->getClanBattle());
		}
	}

	if (start >= enemyClansSize)
		start -= enemyClansSize;
	else if (count > 0)
		start = 0;
	{
		if (count > 0 && start < onlineClansSize)
		{
			OnBattleClanT::iterator offset;
			offset = _onBattleClans.begin();
			std::advance(offset, start);
			for (; offset != _onBattleClans.end() && count > 0; ++ offset, -- count )
				makeBattleClanInfo(player, st, offset->second);
		}
	}
 
	if (start >= onlineClansSize)
		start -= onlineClansSize;
	else if (count > 0)
		start = 0;
	{
		if (count > 0 && start < offlineClanSize)
		{
			OffBattleClanT::iterator offset;
			offset = _offBattleClans.begin();
			std::advance(offset, start);
			for (; offset != _offBattleClans.end() && count > 0; ++ offset, -- count)
				makeBattleClanInfo(player, st, *offset);
		}
	}

	st << Stream::eos;
	player->send(st);
}

bool ClanManager::addOnBattleClan(ClanBattle * clanBattle)
{
	//check here
	UInt8 cbt = clanBattle->getBattleTime();
	OnBattleClanT::iterator res = _onBattleClans.insert(std::make_pair(OnBCTKey(cbt, clanBattle->getLev()), clanBattle));
	return res != _onBattleClans.end();
}

bool ClanManager::addOffBattleClan(ClanBattle * clanBattle)
{
	_offBattleClans.push_back(clanBattle);
	return true;
}

bool ClanManager::delBattleClan(ClanBattle * clanBattle)
{
	OffBattleClanT::iterator found = std::find(_offBattleClans.begin(), _offBattleClans.end(), clanBattle);
	if (found != _offBattleClans.end())
	{
		_offBattleClans.erase(found);
		return true;
	}
	OnBattleClanT::iterator onBcIter;
	for (onBcIter = _onBattleClans.begin(); onBcIter != _onBattleClans.end(); ++ onBcIter)
	{
		if (onBcIter->second == clanBattle)
		{
			_onBattleClans.erase(onBcIter);
			return true;
		}
	}
	return false;
}

ClanBattle * ClanManager::getRobBattleClan()
{
	return _clanRobBattle;
}

ClanBattle * ClanManager::getBattleClan(Player * player, std::string& name)
{
	if (name.empty())
		return _clanRobBattle;
	Clan * clan = globalGlobalNamedClans[player->fixName(name)];
	return clan == NULL ? NULL : clan->getClanBattle();
}

static bool buildClanBattleTime(Clan * clan, UInt32 thisDay)
{
	ClanBattle * clanBattle = clan->getClanBattle();
	clanBattle->configClanBattleCheck();
	clanManager.addOnBattleClan(clanBattle);
	return true;
}

void ClanManager::reConfigClanBattle()
{
	UInt32 thisDay = TimeUtil::SharpDay(0, TimeUtil::Now() + 60);
	_onBattleClans.clear();
	_offBattleClans.clear();
	//if (!_hasAllocRepo)
	//	allocRepo();
	//else
	//	_hasAllocRepo = false;
	_hasBroadcast = 0xFF;
	_firstBroadcast = false;
	globalClans.enumerate(buildClanBattleTime, thisDay);
	_clanRobBattle->configClanBattleCheck();
	clanManager.addOnBattleClan(_clanRobBattle);
	DB().PushUpdateData("UPDATE `clan_player` SET `enterCount` = 0, `achieveCount` = 0, `thisDay` = %u", thisDay);
	DB().PushUpdateData("UPDATE `clan` SET `grabAchieve` = 0");
	DB().PushUpdateData("UPDATE `clan` AS `update_clan` SET `battleTime` = `nextBattleTime`, `battleStatus` = 0 WHERE `update_clan`.`id` = `id`");
}

// static bool buildRowAchieveClan(Clan * clan, ClanBttleStatistics *cbs)/*std::multimap<UInt16, Clan *, std::greater<UInt16> >& mat)*/
// {
// 	std::multimap<UInt32, Clan *, std::greater<UInt32> >& mat = *(cbs->maxAchieveTable);
// 	mat.insert(std::make_pair(clan->getClanBattle()->getGrabAchieve(), clan));
// 	clan->retEnterPlsyersCount(cbs->count);
// 	return true;
// }	
// 
//void ClanManager::allocRepo()
//{
//	UInt16 count[3] = {0};
//	std::multimap<UInt32,  Clan *, std::greater<UInt32> > maxAchieveTable;
//
//	ClanBttleStatistics cbs = {count, &maxAchieveTable};
//	globalClans.enumerate(buildRowAchieveClan, &cbs);
//
//	std::multimap<UInt32,  Clan *, std::greater<UInt32> >::iterator offset = maxAchieveTable.begin();
//	const char * maxAchiveName[3];
//	for (UInt8 i = 0; offset != maxAchieveTable.end(); ++ offset)
//	{
//		if (i < 3)
//		{
//			offset->second->addRepo(9041, 3-i, i+1);
//			maxAchiveName[i] = offset->second->getName().c_str();
//			++ i;
//		}
//		else
//		{
//			if (offset->second->getClanBattle()->getGrabAchieve() <= 0)
//				break;
//			offset->second->getClanDynamicMsg()->addCDMsg(6, maxAchiveName[0], maxAchiveName[1], maxAchiveName[2], static_cast<UInt32>(std::distance(maxAchieveTable.begin(), offset)+1));
//		}
//	}
//	std::size_t sz = maxAchieveTable.size();
//	if (sz == 0) return;
//	if (sz >= 3)
//	{
//		SYSMSG_BROADCASTV(448, maxAchiveName[0], maxAchiveName[1], maxAchiveName[2]);
//		offset = maxAchieveTable.begin();
//		for (UInt8 i = 0; i < 3 && offset != maxAchieveTable.end(); ++ offset, ++ i)
//			offset->second->getClanDynamicMsg()->addCDMsg(7, maxAchiveName[0], maxAchiveName[1], maxAchiveName[2]);
//	}
//	else if (sz >= 2)
//	{
//		SYSMSG_BROADCASTV(449, maxAchiveName[0], maxAchiveName[1]);
//	}
//	else if (sz >= 1)
//	{
//		SYSMSG_BROADCASTV(450, maxAchiveName[0]);
//	}
//	
//	DBLOG().PushUpdateData("insert into `clanbattle_players_count`(`server_id`, `enter_players1`, `enter_players2`, `enter_players3`, `max_achievement`, `created_at`) values(%u, %u, %u, %u, %u, %u)", cfg.serverLogId, cbs.count[0], cbs.count[1], cbs.count[2], maxAchieveTable.empty() ? 0 : maxAchieveTable.begin()->first, TimeUtil::Now());
//}

void ClanManager::checkCBBroadcast(UInt32 now)
{
	static const UInt32 broadcastTime[] = { 12*60*60+25*60, 12*60*60+30*60, 14*60*60+55*60, 15*60*60, 19*60*60+28*60, 19*60*60+29*60, 19*60*60+30*60, 21*60*60+25*60, 21*60*60+30*60 };
	static const UInt32 broadcastTips[] = { 456, 457, 456, 457, 527, 528, 529, 456, 457 };
	UInt32 interval = now - TimeUtil::SharpDay(0, now);
	if (interval <= broadcastTime[0])
		return;
	if (_hasBroadcast == 0xFF)
	{
		_hasBroadcast = std::upper_bound(broadcastTime, broadcastTime+9, interval) - broadcastTime;
		-- _hasBroadcast;
		if (_hasBroadcast == 0)
			_firstBroadcast = true;
	}
	if (_hasBroadcast >= 8) return;
	if (interval > broadcastTime[_hasBroadcast+1])
	{
		SYSMSG_BROADCASTV(broadcastTips[++_hasBroadcast]);
	}
	else if (_firstBroadcast && _hasBroadcast == 0)
	{
		SYSMSG_BROADCASTV(broadcastTips[_hasBroadcast]);
		_firstBroadcast = false;
	}
}

//void ClanManager::updateAllocated()
//{
//	_hasAllocRepo = _onBattleClans.empty();
//}


}
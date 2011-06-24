#include "Config.h"
#include "ClanBattle.h"
#include "Clan.h"
#include "ClanTech.h"
#include "ClanDynamicMsg.h"
#include "Player.h"
#include "TaskMgr.h"
#include "Server/SysMsg.h"
#include "Server/OidGenerator.h"
#include "GData/NpcGroup.h"
#include "GData/ClanAssistantData.h"
#include "GData/ClanTechTable.h"
#include "Mail.h"
#include "Battle/BattleSimulator.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "MapCollection.h"
#include "Server/Cfg.h"
#include "Common/Itoa.h"
#include "ClanManager.h"
#include "World.h"

namespace GObject
{

UInt8 RecoveConsumeGold[14] = { 0, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100, 100 };
UInt8 ClanCityBattle::_maxEnterCount = 3;

static bool find_clan_battle_member(ClanBattlePlayer * member, Player * p)
{
	return member->player == p;
}

static bool find_clan_battle_member_name(ClanBattlePlayer * member, std::string name)
{
	return member->player->getName() == name;
}

UInt16 ClanBattleHold::atkerSize()
{
	UInt16 count = 0;
	std::set<ClanBattlePlayer *>::iterator offset = battlers.begin();
	for (; offset != battlers.end(); ++offset)
	{
		if ((*offset)->side == 2)
			++ count;
	}
	return count;
}

UInt16 ClanBattleHold::deferSize()
{
	UInt16 count = 0;
	std::set<ClanBattlePlayer *>::iterator offset = battlers.begin();
	for (; offset != battlers.end(); ++offset)
	{
		if ((*offset)->side != 2)
			++ count;
	}
	return count;
}

UInt16 ClanBattleHold::aliveDeferSize()
{
	UInt16 count = 0;
	std::set<ClanBattlePlayer *>::iterator offset = battlers.begin();
	for (; offset != battlers.end(); ++offset)
	{
		if ((*offset)->side != 2 && (*offset)->status == 1)
			++ count;
	}
	return count;
}

void ClanBattleHold::addBattler(ClanBattlePlayer * cbp)
{
	std::pair<cbp_iter, bool> res = battlers.insert(cbp);
	if (res.second)
	{
		if (cbp->side == 2)
		{
			if (cbp->status == 1)
				holdCount[0] ++;
			else
				holdCount[1] ++;
		}
		else
		{
			if (cbp->status == 1)
				holdCount[2] ++;
			else
				holdCount[3] ++;
		}
	}
}

bool ClanBattleHold::delBattler(Player * player)
{
	using namespace std::placeholders;
	cbp_iter found = std::find_if(battlers.begin(), battlers.end(), std::bind(find_clan_battle_member, _1, player));
	if (found != battlers.end())
		return delBattler(*found);		
	return false;
}

bool ClanBattleHold::existBattler(Player * player)
{
	using namespace std::placeholders;
	cbp_iter found = std::find_if(battlers.begin(), battlers.end(), std::bind(find_clan_battle_member, _1, player));
	return found != battlers.end();
}

bool ClanBattleHold::existBattler(ClanBattlePlayer * cbp)
{
	return battlers.find(cbp) != battlers.end();
}

ClanBattlePlayer * ClanBattleHold::getBattler(Player * player)
{
	using namespace std::placeholders;
	cbp_iter found = std::find_if(battlers.begin(), battlers.end(), std::bind(find_clan_battle_member, _1, player));
	return found != battlers.end() ? *found : NULL;
}

ClanBattlePlayer * ClanBattleHold::getBattler(const std::string& name)
{
	using namespace std::placeholders;
	cbp_iter found = std::find_if(battlers.begin(), battlers.end(), std::bind(find_clan_battle_member_name, _1, name));
	return found != battlers.end() ? *found : NULL;
}

bool ClanBattleHold::delBattler(ClanBattlePlayer * cbp)
{
	if (battlers.erase(cbp) != 0)
	{
		if (cbp->side == 2)
		{
			if (cbp->status == 1)
			{
				if (holdCount[0] > 0)
					holdCount[0] --;
			}
			else
			{
				if (holdCount[1] > 0)
					holdCount[1] --;
			}
		}
		else
		{
			if (cbp->status == 1)
			{
				if (holdCount[2] > 0)
					holdCount[2] --;
			}
			else
			{
				if (holdCount[3] > 0)
					holdCount[3] --;
			}
		}
		return true;
	}
	return false;
}

void ClanBattleHold::changeBattlerStatus(ClanBattlePlayer * cbp, UInt8 status, bool exist)
{
	if (cbp->hold != hold || cbp->status == status)
		return;
	if (!exist && !existBattler(cbp))
		return;
	if (cbp->side == 2)
	{
		if (cbp->status == 1)
		{
			if (holdCount[0] > 0)
				holdCount[0] --;
			holdCount[1] ++;
		}
		else
		{
			if (status == 1)
			{
				if (holdCount[1] > 0)
					holdCount[1] --;
				holdCount[0] ++;
			}
		}
	}
	else
	{
		if (cbp->status == 1)
		{
			if (holdCount[2] > 0)
				holdCount[2] --;
			holdCount[3] ++;
		}
		else
		{
			if (status == 1)
			{
				if (holdCount[3] > 0)
					holdCount[3] --;
				holdCount[2] ++;
			}
		}
	}
	cbp->status = status;
}

void ClanBattleHold::clear()
{
	battlers.clear();
	memset(holdCount, 0x00, sizeof(holdCount));
}

ClanBattle::ClanBattle() : _grabAchieve(0), _battleThisDay(0), _hasBattle(0), _isInbattling(0), _isOldInBattling(0), _isInAttacking(false)
{
	_attackClanBattlerStream.init(0x79);
	_attackClanBattlerStream << static_cast<UInt8>(4) << Stream::eos;
	_attackClanTips.init(0x5F);
	_attackClanTips << static_cast<UInt8>(4) << Stream::eos;
	_firstAttack[0] = _firstAttack[1] = false;
}

ClanBattle::~ClanBattle()
{

}

void ClanBattle::disbandClanBattle()
{
	std::map<Player *, ClanBattlePlayer *>::iterator offset = _clanBattlePlayerLocs.begin();
	for (; offset != _clanBattlePlayerLocs.end(); ++ offset)
	{
		delete offset->second;
	}

	//据点护卫析构
	for (UInt8 i = 0; i < 4; ++ i)
	{
		if (_holds[i].guarder != NULL)
			SAFE_DELETE(_holds[i].guarder);
		std::map<std::string, ClanHoldMonster *>::iterator offset = _holds[i].assists.begin();
		for (; offset != _holds[i].assists.end(); ++ offset)
			delete offset->second;
		_holds[i].clear();
	}
}

void ClanBattle::startClanBattle(UInt32 battleThisDay, bool writedb)
{
	_isInbattling = 1;
	configClanBattleData(writedb);
	_battleThisDay = battleThisDay;
	if (writedb)
		DB().PushUpdateData("UPDATE `clan` SET `battleThisDay` = %u, `battleStatus` = %u WHERE `id` = %u", _battleThisDay, _isInbattling, getOwnerClanId());
}

void ClanBattle::overClanBattle(UInt8 succ)
{
	_isInbattling = 2;
	UInt32 thisDay = TimeUtil::SharpDay();
	std::swap(_nextBattleTime, _battleTime);
	DB().PushUpdateData("UPDATE `clan` SET `battleStatus` = %u WHERE `id` = %u", _isInbattling, getOwnerClanId());
	DB().PushUpdateData("REPLACE INTO `clan_battle_result`(`id`, `battleTime`, `result`) VALUES(%u, %u, %u)", getOwnerClanId(), thisDay, succ);
	closingBattlerAward(succ);
	Clan * ownerClan = getOwnerClan();
	ClanDynamicMsg * clanDynamicMsg = NULL;
	if (ownerClan != NULL)
		clanDynamicMsg = ownerClan->getClanDynamicMsg();
	if (succ == 0)
	{
		SYSMSG_BROADCASTV(451, getOwnerClanName().c_str());
		if (ownerClan != NULL && clanDynamicMsg != NULL)
		{
			Stream st;			
			clanDynamicMsg->addCDMsg(3, getOwnerClanId(), thisDay, &st);
			ownerClan->broadcast(st);
		}
	}
	else
	{
		if (ownerClan != NULL && clanDynamicMsg != NULL)
		{
			Stream st;
			clanDynamicMsg->addCDMsg(2, getOwnerClanId(), thisDay, &st);
			ownerClan->broadcast(st);
		}
	}
	notifyClanBattleOverReport(succ);
	notifyBattleOverMailNotify(succ);
}

void ClanBattle::configClanBattleData(bool writedb)
{
	Clan * clan = getOwnerClan();
	if (clan == NULL)
		return;

	UInt32 guarderId = 0;
	ClanTech* techs = clan->getClanTech();

	//综合宗族技艺配置相关战斗数据
	_holds[0].endurance = _holds[0].totalendurance = 30;
	_holds[3].endurance = _holds[3].totalendurance = 0;
	UInt8 edurance =  static_cast<UInt8>(techs->getNorthEdurance());
	_holds[1].endurance = _holds[1].totalendurance = (edurance == 0 ? 30 : edurance);
	edurance = static_cast<UInt8>(techs->getSouthEdurance());
	_holds[2].endurance = _holds[2].totalendurance = (edurance == 0 ? 30 : edurance);
	if (writedb)
		DB().PushUpdateData("UPDATE `clan` SET `southEdurance` = %u, `northEdurance` = %u, `hallEdurance` = %u WHERE `id` = %u", _holds[1].endurance, _holds[2].endurance, _holds[0].endurance, getOwnerClanId());

	//配置宗祠守卫者
	{
		guarderId = techs->getHoldTotemGuarder();
		if (guarderId == 0) guarderId = 4562;
		const GData::ClanAssistant * cat = GData::clanAssistants[guarderId];
		if (cat == NULL)
		{
			fprintf(stderr, "Error Clan Battle Configure : clanId = %u, guarder = %u", clan->getId(), guarderId);
			return;
		}
		SYSMSG(name, 411);
		_holds[0].guarder = new(std::nothrow) ClanCityHoldMonster(name, cat->getLevel(), cat->getClass(), _holds[0].hold, 0, cat);
		assert(_holds[0].guarder != NULL);
	}

	//配置南北门守卫者，护卫者
	{
		guarderId = techs->getHoldCityGuarder();
		if (guarderId == 0) guarderId = 4540;
		//配置南门图腾 
		{
			const GData::ClanAssistant * cat = GData::clanAssistants[guarderId];
			if (cat == NULL)
			{
				fprintf(stderr, "Error Clan Battle Sourth Configure : clanId = %u, assist = %u", clan->getId(), guarderId);
				return;
			}
			SYSMSG(name, 412);
			_holds[1].guarder = new(std::nothrow) ClanCityHoldMonster(name, cat->getLevel(), cat->getClass(), _holds[1].hold, 0, cat);
			assert(_holds[1].guarder != NULL);
		}
		//配置北门图腾
		{
			const GData::ClanAssistant * cat = GData::clanAssistants[guarderId];
			if (cat == NULL)
			{
				fprintf(stderr, "Error Clan Battle North Configure : clanId = %u, assist = %u", clan->getId(), guarderId);
				return;
			}
			SYSMSG(name, 413);
			_holds[2].guarder = new(std::nothrow) ClanCityHoldMonster(name, cat->getLevel(), cat->getClass(), _holds[2].hold, 0, cat);
			assert(_holds[2].guarder != NULL);
		}
		_firstAttack[0] = _firstAttack[1] = false;


		//配置护卫者
		UInt16 assistCnt;
		UInt32 assistId;
		if (!techs->getHoldAssist(assistCnt, assistId))
			return;
		const GData::ClanAssistant * cat = GData::clanAssistants[assistId];
		if (cat == NULL)
		{
			fprintf(stderr, "Error Clan Battle Assist Configure : clanId = %u, assist = %u", clan->getId(), assistId);
			return;
		}
		for (UInt16 i = 0; i < assistCnt; ++ i)
		{
			//南门
			{
				SYSMSG(name, 414);
				std::string southName;
				southName.append(name);
				southName.append(Itoa<UInt16>(i+1));
				_holds[1].assists.insert(std::make_pair(southName, new ClanCityHoldMonster(southName, cat->getLevel(), cat->getClass(), _holds[1].hold, 0, cat)));
			}
			//北门
			{
				SYSMSG(name, 415);
				std::string northName;
				northName.append(name);
				northName.append(Itoa<UInt16>(i+1));
				_holds[2].assists.insert(std::make_pair(northName, new ClanCityHoldMonster(northName, cat->getLevel(), cat->getClass(), _holds[2].hold, 0, cat)));
			}
		}
	}
}


ClanBattlePlayer * ClanBattle::getBattlePlayer(Player * player)
{
	std::map<Player *, ClanBattlePlayer *>::iterator foundcbp = _clanBattlePlayerLocs.find(player);
	if (foundcbp == _clanBattlePlayerLocs.end())
		return NULL;
	return foundcbp->second;
}

bool ClanBattle::isClanHoldDestroy(UInt16 pos)
{
	return getClanEndurance(pos) == 0;
}

UInt8 ClanBattle::getClanEndurance(UInt16 pos)
{	
	UInt16 hold = (pos & 0x00FF) - 1;
	return hold >= 4 ? 0 : _holds[hold].endurance;
}

bool ClanBattle::resumePlayerToClanBattle(ClanBattlePlayer * cbp, UInt32 now)
{
	if (!isInBattling() && cbp->side != 0)
	{
		cbp->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
		return false;
	}
	UInt16 hold = (cbp->hold & 0x00FF ) - 1;
	if (hold > 3)
		return false;

	_clanBattlePlayerLocs.insert(std::make_pair(cbp->player, cbp));
	if ((cbp->hasEnter & 0x0F) > 0)
	{
		_holds[hold].addBattler(cbp);
		_offClanBattlers.insert(cbp);
		cbp->player->setClanBattle(this);
	}
	if (cbp->status == 0)
		cbp->status = 2;
	cbp->player->setBuffData(PLAYER_BUFF_CLANRECOVE, 0);
	cbp->player->setBuffData(PLAYER_BUFF_CLANMOVE, 0);
	if (isInBattling())
	{
		UInt32 overTime = getBattleOverTime();
		cbp->player->setBuffData(PLAYER_BUFF_CLANBATTING, overTime);
		cbp->player->addFlag(Player::ClanBattling);
		cbp->player->addGlobalFlag(Player::ClanBattleFlag);
	}
	else
		cbp->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);

	return true;
}

void ClanBattle::listClanHoldPlayerInfo(Player * player, UInt16 pos)
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (hold >= 4) return ;
	Stream st(0x79);
	st << static_cast<UInt8>(5) << static_cast<UInt8>(0);

	//怪物, 非战场时间不显示
	UInt8 usedCnt = 0;
	if (hold != 3 && isInBattling() && !isClanHoldDestroy(pos))
	{
		std::map<std::string, ClanHoldMonster *>::iterator offset = _holds[hold].assists.begin();
		for (; offset != _holds[hold].assists.end(); ++ offset)
		{
			ClanHoldMonster * chm = offset->second;
			st << chm->name << (chm == NULL ? static_cast<UInt8>(0) : chm->klass) << getClanMonsterRelation(player) << static_cast<UInt8>(1) << (chm == NULL ? static_cast<UInt8>(0) : chm->level);
		}
		usedCnt += static_cast<UInt8>(_holds[hold].assists.size());
		{
			ClanHoldMonster * guarder = _holds[hold].guarder;
			st << (guarder == NULL ? "" : guarder->name) << (guarder == NULL ? static_cast<UInt8>(0) : guarder->klass) << getClanMonsterRelation(player) << static_cast<UInt8>(2) << (guarder == NULL ? static_cast<UInt8>(0) : guarder->level);
			++ usedCnt;
		}
	}

	ClanBattlePlayer * cbPlayer;
	std::set<ClanBattlePlayer *>::iterator iter = _holds[hold].battlers.begin();
	for (; iter != _holds[hold].battlers.end() && usedCnt <= 0xFF; ++ iter)
	{
		cbPlayer = *iter;
		st << cbPlayer->player->getName() << cbPlayer->player->GetClass() << getClanRelation(player, cbPlayer->player) << static_cast<UInt8>(cbPlayer->status == 1 ? 0 : 3) << cbPlayer->player->GetLev(); 
		++ usedCnt;
	}
	st.data<UInt8>(4+1) = usedCnt;
	st << Stream::eos;
	player->send(st);
}


void ClanBattle::notifyClanBattleEnterInfo(Player * player, UInt8 enterType, UInt8 res, std::string clanname)
{
	Stream st(0x79);
	st << static_cast<UInt8>(1) << enterType << getClanRelation(player->getClan()) << res;
	if (enterType == 2)
		st << getClanBattleType() << getNextBattleTime() << clanname;
	st << Stream::eos;
	player->send(st);
}

void ClanBattle::notifyClanBattlePlayerMoveInfo(Player * player, UInt8 res, UInt16 pos)
{
	Stream st(0x79);
	st << static_cast<UInt8>(3) << res << pos << Stream::eos;
	player->send(st);
}


void ClanBattle::notifyClanBattlePlayerInfo(Player * player, Player * notifier, UInt8 enter)
{
	Stream st(0x79);
	st << static_cast<UInt8>(6) << static_cast<UInt8>(enter == 0 ? 1 : 0) << notifier->getName() << notifier->GetClass() << getClanRelation(player, notifier) << static_cast<UInt8>(0) << notifier->GetLev() << Stream::eos;
	player->send(st);
}


void ClanBattle::notifyClanBattleRecoveData(Player * player, UInt16 reliveNum, UInt32 recoveTime)
{
	UInt16 needGold = reliveNum >= 12 ? 100 : RecoveConsumeGold[reliveNum];
	Stream st(0x79);
	st << static_cast<UInt8>(18) << static_cast<UInt16>(recoveTime) << needGold << Stream::eos;
	player->send(st);
}

void ClanBattle::notifyClanBattleRecoveData(Player * player)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end() || found->second->status != 0)
		return;
	UInt32 now = TimeUtil::Now();
	UInt32 recoveTime = player->getBuffData(PLAYER_BUFF_CLANRECOVE, now);
	if (recoveTime == 0)
		return;
	notifyClanBattleRecoveData(player, (found->second->reliveNum >> 8) + 1, recoveTime - now);
}

void ClanBattle::notifyClanBattleOverTime(Player * player)
{
	Stream st(0x79);
	st << static_cast<UInt8>(11) << static_cast<UInt16>(getClanBattleOverTime()) << Stream::eos;
	if (player == NULL)
		broadcastHold(st);
	else
		player->send(st);	
}

void ClanBattle::notifyClanBattleWinData(Player * player)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end() || (found->second->hasEnter & 0x0F) == 0)
		return;
	Stream st(0x79);
	st << static_cast<UInt8>(13) << found->second->wins << found->second->serailWins << Stream::eos;
	player->send(st);
}

void ClanBattle::notifyClanHoldEndurance(Player * player)
{
	Stream st(0x79);
	st << static_cast<UInt8>(14) << static_cast<UInt8>(3);
	for (UInt8 i = 0; i <= 2; ++ i)
	{
		st << _holds[i].hold << static_cast<UInt8>(_holds[i].endurance * 100 / _holds[i].totalendurance) << static_cast<UInt8>((i != 0 && _holds[i].buff != 0) ? 1 : 0);
	}
	st << Stream::eos;
	if (player == NULL)
		broadcastHold(st);
	else
		player->send(st);
}


void ClanBattle::notifyClanHoldEndurance(ClanBattleHold& hold)
{
	Stream st(0x79);
	st << static_cast<UInt8>(14) << static_cast<UInt8>(1);
	st << hold.hold << static_cast<UInt8>(hold.endurance * 100 / hold.totalendurance) << static_cast<UInt8>((hold.hold != 0 && hold.buff != 0) ? 1 : 0) << Stream::eos;
	broadcastHold(st);
}


void ClanBattle::notifyClanBattlePlayerCount(Player * player)
{
	UInt16 atkerAliveNum, akterTotalNum, deferAliveNum, deferTotalNum;

	atkerAliveNum = akterTotalNum = deferAliveNum = deferTotalNum = 0;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		std::set<ClanBattlePlayer *>::iterator offset = _holds[i].battlers.begin();
		for (; offset != _holds[i].battlers.end(); ++ offset)
		{
			if ((*offset)->side == 2)
			{
				//攻击方
				++ akterTotalNum;
				if ((*offset)->status == 1)
					++ atkerAliveNum;
			}
			else
			{
				//防守方
				++ deferTotalNum;
				if ((*offset)->status == 1)
					++ deferAliveNum;
			}
		}
	}

	Stream st(0x79);
	st << static_cast<UInt8>(12) << atkerAliveNum << akterTotalNum << deferAliveNum << deferTotalNum << Stream::eos;
	if (player == NULL)
		broadcastHold(st);
	else
		player->send(st);
}

void ClanBattle::notifyClanHoldPlayerInfo(Player * player, UInt8 enter)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end() || (found->second->hasEnter & 0x0F) == 0)
		return;
	notifyClanHoldPlayerInfo(player, found->second->hold, found->second->status, enter);
}

void ClanBattle::notifyClanHoldPlayerInfo(Player * player, UInt16 pos, UInt8 live, UInt8 enter)
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (hold >= 4) return ;
	Stream st(0x79);
	st << static_cast<UInt8>(6) << static_cast<UInt8>(enter == 0 ? 1 : 0) << player->getName() << player->GetClass() << static_cast<UInt8>(0) << static_cast<UInt8>(live == 1 ? 0 : 3) << player->GetLev() << Stream::eos;
	size_t offset = 4 + 1 + 1 + 2 + player->getName().length() + 1;
	std::set<ClanBattlePlayer *>::iterator iter = _holds[hold].battlers.begin();
	for (; iter != _holds[hold].battlers.end(); ++ iter)
	{
		st.data<UInt8>(offset) = getClanRelation(player, (*iter)->player);
		(*iter)->player->send(st);
	}
}


void ClanBattle::notifyClanHoldAssistEnterInfo(std::string name, UInt16 pos, UInt8 enter)  //0 : enter 1 : leave
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (hold >= 3) return;

	if (enter == 0 && (!isInBattling() || isClanHoldDestroy(pos)))
		return;

	std::map<std::string, ClanHoldMonster *>::iterator found = _holds[hold].assists.find(name);
	if (found == _holds[hold].assists.end())
		return ;
	ClanHoldMonster * chm = found->second;
	Stream st(0x79);
	st << static_cast<UInt8>(6) << enter << name << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(1) << chm->level << Stream::eos;
	size_t offset = 4 + 1 + 1 + 2 + name.length() + 1;
	std::set<ClanBattlePlayer *>::iterator iter = _holds[hold].battlers.begin();
	for (; iter != _holds[hold].battlers.end(); ++ iter)
	{
		if ((*iter)->side == 2)
		{
			//攻击方
			st.data<UInt8>(offset) = 0x04;
			(*iter)->player->send(st);
		}
		else
		{
			//防守方
			st.data<UInt8>(offset) = 0x01;
			(*iter)->player->send(st);
		}
	}
}


void ClanBattle::notifyClanHoldGuarderEnterInfo(UInt16 pos, UInt8 enter)
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (hold >= 3) return ;

	const ClanHoldMonster * guarder = _holds[hold].guarder;
	Stream st(0x79);
	st << static_cast<UInt8>(6) << enter << (guarder == NULL ? "" : guarder->name) << (guarder == NULL ? static_cast<UInt8>(0) : guarder->klass) << static_cast<UInt8>(0) << static_cast<UInt8>(2) << (guarder == NULL ? static_cast<UInt8>(0) : guarder->level) << Stream::eos;
	size_t offset = 4 + 1 + 1 + 2 + (guarder == NULL ? 2 : guarder->name.length()) + 1;
	std::set<ClanBattlePlayer *>::iterator iter = _holds[hold].battlers.begin();
	for (; iter != _holds[hold].battlers.end(); ++ iter)
	{
		if ((*iter)->side == 2)
		{
			//攻击方
			st.data<UInt8>(offset) = 0x04;
			(*iter)->player->send(st);
		}
		else
		{
			//防守方
			st.data<UInt8>(offset) = 0x01;
			(*iter)->player->send(st);
		}
	}
}

void ClanBattle::notifyClanBattleReport(Player * atker, Player * defer, UInt8 res, UInt32 rpid)
{
	Clan * atkerClan = atker->getClan();
	Clan * deferClan = defer->getClan();

	Stream st(0x79);
	st << static_cast<UInt8>(15) << static_cast<UInt8>(1) << res << rpid << (atkerClan != NULL ? atkerClan->getName() : "") << atker->getName() << static_cast<UInt8>(1) << static_cast<UInt8>(0) << (deferClan != NULL ? deferClan->getName() : "") << defer->getName() << static_cast<UInt8>(1) << static_cast<UInt8>(0) << Stream::eos;
	std::set<ClanBattlePlayer *>::iterator offset;
	UInt8 atkerOffset = 4+1+1+1+4+2+(atkerClan != NULL ? atkerClan->getName().length() : 0)+2+atker->getName().length()+1;
	UInt8 deferOffset = 4+1+1+1+4+2+(atkerClan != NULL ? atkerClan->getName().length() : 0)+2+atker->getName().length()+1+1+2+(deferClan != NULL ? deferClan->getName().length() : 0)+2+defer->getName().length()+1;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		offset = _holds[i].battlers.begin();
		for (; offset != _holds[i].battlers.end(); ++ offset)
		{
			st.data<UInt8>(atkerOffset) = getClanRelation(atker, (*offset)->player);
			st.data<UInt8>(deferOffset) = getClanRelation(defer, (*offset)->player);
			(*offset)->player->send(st);
		}
	}
}

void ClanBattle::notifyClanBattleReport(Player * atker, const std::string& monster, UInt8 res, UInt32 rpid)
{
	Clan * atkerClan = atker->getClan();

	Stream st(0x79);
	st << static_cast<UInt8>(15) << static_cast<UInt8>(1) << res << rpid << (atkerClan != NULL ? atkerClan->getName() : "") << atker->getName() << static_cast<UInt8>(1) << static_cast<UInt8>(0) << getOwnerClanName() << monster << static_cast<UInt8>(0) << static_cast<UInt8>(0) << Stream::eos;
	std::set<ClanBattlePlayer *>::iterator offset;
	UInt8 atkerOffset = 4+1+1+1+4+2+(atkerClan != NULL ? atkerClan->getName().length() : 0)+2+atker->getName().length()+1;
	UInt8 deferOffset = 4+1+1+1+4+2+(atkerClan != NULL ? atkerClan->getName().length() : 0)+2+atker->getName().length()+1+1+2+getOwnerClanName().length()+2+monster.length()+1;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		offset = _holds[i].battlers.begin();
		for (; offset != _holds[i].battlers.end(); ++ offset)
		{
			st.data<UInt8>(atkerOffset) = getClanRelation(atker, (*offset)->player);
			st.data<UInt8>(deferOffset) = getClanMonsterRelation((*offset)->player);
			(*offset)->player->send(st);
		}
	}
}

void ClanBattle::setClanBattlerStartData(bool notify)
{
	UInt32 overTime = getBattleOverTime();
	Player * player = NULL;
	std::set<ClanBattlePlayer *>::iterator offset;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		offset = _holds[i].battlers.begin();
		for (; offset != _holds[i].battlers.end(); ++ offset)
		{
			player = (*offset)->player;
			player->setBuffData(PLAYER_BUFF_CLANBATTING, overTime);
			player->addGlobalFlag(Player::ClanBattleFlag);
			player->addFlag(Player::ClanBattling);
			if (notify)
				notifyClanBattleWinData(player);
		}
	}
	if (notify)
	{
		notifyClanBattleOverTime();
		notifyClanBattlePlayerCount();
		notifyClanHoldEndurance();
	}
}

void ClanBattle::cancelClanBattlerOffCheck(Player * player)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found != _clanBattlePlayerLocs.end())
	{
		found->second->offTime = 0;
	}
}

void ClanBattle::addOfflineBattler(Player * player, UInt32 now)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end())
		return;
	if (isInBattling())
	{
		found->second->offTime = now;
		_offClanBattlers.insert(found->second);
	}
	else
		kickClanBattler(player);
}


bool ClanBattle::moveToHold(Player * player, UInt16 pos)
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (hold >= 4)
		return false;

	ClanBattlePlayer * cbPlayer = NULL;
	std::map<Player *, ClanBattlePlayer *>::iterator foundcbp = _clanBattlePlayerLocs.find(player);
	if (foundcbp == _clanBattlePlayerLocs.end())
		return false;
	cbPlayer = foundcbp->second;
	if (!isInBattling() && cbPlayer->side != 0)
	{
		kickClanBattler(cbPlayer->player);
		return false;
	}
	if (cbPlayer->hold == pos)
		return false;
	UInt32 now = TimeUtil::Now();
	if (cbPlayer->side != 2)
	{
		//防守方
		if (cbPlayer->status != 1)
		{
			ERROR_LOG("Player cannot move now !!!");
			return false;
		}
		if (player->getBuffData(PLAYER_BUFF_CLANMOVE, now))
			return false;
	}
	else
	{
		//攻击方
		if ((cbPlayer->hold == _holds[1].hold || cbPlayer->hold == _holds[2].hold || cbPlayer->hold == _holds[3].hold) && !isClanHoldDestroy(_holds[1].hold) && !isClanHoldDestroy(_holds[2].hold) && pos == _holds[0].hold)
		{
			SYSMSG_SEND(416, cbPlayer->player);
			return false;
		}
		if (cbPlayer->status != 1)
		{
			ERROR_LOG("Player cannot move now !!!");
			return false;
		}
		if (player->getBuffData(PLAYER_BUFF_CLANMOVE, now))
			return false;
	}
	delClanBattlePlayer(cbPlayer->hold, cbPlayer->side, cbPlayer->status, player);
	if (cbPlayer->hold != pos)
	{
		Stream st(0x79);
		st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << pos << Stream::eos;
		cbPlayer->player->send(st);
		UInt32 buffTime = 0;
		if (isInBattling())
		{
			if (isClanHoldDestroy(pos))
				buffTime = cbPlayer->side != 2 ? 15 : 10;
			else
				buffTime = cbPlayer->side != 2 ? 25 : 30;
		}
		else
			buffTime = 25;
		cbPlayer->player->setBuffData(PLAYER_BUFF_CLANMOVE, now+buffTime);
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleHold` = %u WHERE `id` = %u", pos, cbPlayer->id);
	}
	cbPlayer->hold = pos;
	addClanBattlePlayer(cbPlayer);
	listClanHoldPlayerInfo(player, cbPlayer->hold);

	return true;
}

void ClanBattle::kickClanBattler(Player * player)
{
	//踢出战场玩家
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end())
		return;
	ClanBattlePlayer * cbPlayer = found->second;
	cbPlayer->hasEnter &= 0xF0;
	delClanBattlePlayer(cbPlayer->hold, cbPlayer->side, cbPlayer->status, player);
	_recoverClanBattlers.erase(cbPlayer);
	_offClanBattlers.erase(cbPlayer);
	notifyClanBattlePlayerCount();
	{
		//跨地图
		UInt16 spot = player->getLocation();
		CountryEnterStruct ces(true, player->isInCity() ? 1 : 0, spot);
		GameMsgHdr hdr(0x1F0, mapCollection.getCountryFromSpot(spot), player, sizeof(CountryEnterStruct));
		GLOBAL().PushMsg( hdr, &ces );
	}
	player->setClanBattle(NULL);
	player->setBuffData(PLAYER_BUFF_CLANBATTING, 0);
	//player->setBuffData(PLAYER_BUFF_CLANRECOVE, 0);
	//player->setBuffData(PLAYER_BUFF_CLANMOVE, 0);
	player->delGlobalFlag(Player::ClanBattleFlag);
	player->delFlag(Player::ClanBattling);
	player->autoRegenAll();
	DB().PushUpdateData("UPDATE `clan_battler` SET `hasEnter` = %u WHERE `id` = %u", cbPlayer->hasEnter, cbPlayer->id);
}


UInt16 ClanBattle::getTotalAtkerNum()
{
	if (_isInbattling != 1)
		return 0;
	UInt16 count = 0;
	//for (UInt8 i = 0; i < 4; ++ i)
	//	count += (_holds[i].holdCount[0] + _holds[i].holdCount[1]);
	for (UInt8 i = 0; i < 4; ++i)
	{
		std::set<ClanBattlePlayer *>::iterator offset = _holds[i].battlers.begin();
		for (; offset != _holds[i].battlers.end(); ++ offset)
		{
			if ((*offset)->side == 2)
				++ count;
		}
	}
	return count;
}

UInt16 ClanBattle::getTotalDeferNum()
{
	if (_isInbattling != 1)
		return 0;
	UInt16 count = 0;
	//for (UInt8 i = 0; i < 4; ++ i)
	//	count += (_holds[i].holdCount[2] + _holds[i].holdCount[3]);
	for (UInt8 i = 0; i < 4; ++i)
	{
		std::set<ClanBattlePlayer *>::iterator offset = _holds[i].battlers.begin();
		for (; offset != _holds[i].battlers.end(); ++ offset)
		{
			if ((*offset)->side != 2)
				++ count;
		}
	}
	return count;
}

UInt16 ClanBattle::getAtkerNum(UInt16 pos)
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (hold >= 4)
		return 0;
	if (_isInbattling != 1)
		return 0;
	if (hold >=4)
		return 0;
	UInt16 count = 0;
	std::set<ClanBattlePlayer *>::iterator offset = _holds[hold].battlers.begin();
	for (; offset != _holds[hold].battlers.end(); ++ offset)
	{
		if ((*offset)->side == 2)
			++ count;
	}
	return count;
	//return _holds[hold].holdCount[0] + _holds[hold].holdCount[1];
}


UInt16 ClanBattle::getDeferNum(UInt16 pos)
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (_isInbattling != 1)
		return 0;
	if (hold >= 4)
		return 0;
	UInt16 count = 0;
	std::set<ClanBattlePlayer *>::iterator offset = _holds[hold].battlers.begin();
	for (; offset != _holds[hold].battlers.end(); ++ offset)
	{
		if ((*offset)->side != 2)
			++ count;
	}
	return count;
	//return _holds[hold].holdCount[2] + _holds[hold].holdCount[3];
}

UInt8 ClanBattle::getClanRelation(Player * player1, Player * player2)
{
	Clan * clan1 = player1->getClan();
	Clan * clan2 = player2->getClan();
	UInt8 rt = 0;
	if (clan1 == NULL || clan2 == NULL)
		return 0;
	if (clan1 == clan2)
		rt |= 0x01;
	if (clan1->hasAllyClan(clan2))
		rt |= 0x02;
	UInt8 rt1 = getClanRelation(clan1);
	UInt8 rt2 = getClanRelation(clan2);
	if ((rt1 != 2 && rt2 == 2) || (rt1 == 2 && rt2 != 2))
		rt |= 0x04;
	return rt;
}

UInt8 ClanBattle::getClanMonsterRelation(Player * player)
{
	Clan * clan = player->getClan();
	UInt8 r = getClanRelation(clan);
	if(r > 2)
		return 1;
	static UInt8 rCvt[] = { 0x01, 0x02, 0x04 };
	return rCvt[r];
}


UInt16 ClanBattle::getBattleClanPlayerLocation(Player * player)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end() || ((found->second->hasEnter & 0x0F) == 0))
		return 0;
	return found->second->hold;
}

UInt8 ClanBattle::getBattleClanPlayerSide(Player * player)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end() || ((found->second->hasEnter & 0x0F) == 0))
		return 0;
	return found->second->side;
}

void ClanBattle::broadcastHold(UInt16 pos, Stream& st)
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (hold >= 4) return;
	std::set<ClanBattlePlayer *>::iterator offset = _holds[hold].battlers.begin();
	for (; offset != _holds[hold].battlers.end(); ++ offset)
		(*offset)->player->send(st);
}

void ClanBattle::broadcastHold(Stream& st)
{
	std::set<ClanBattlePlayer *>::iterator offset;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		offset = _holds[i].battlers.begin();
		for (; offset != _holds[i].battlers.end(); ++ offset)
			(*offset)->player->send(st);
	}
}

void ClanBattle::delClanBattlePlayer(UInt16 pos, UInt8 side, UInt8 status, Player * player)
{
	UInt16 hold = (pos & 0x00FF) - 1;
	if (hold >= 4) return;
	if (_holds[hold].delBattler(player))
		notifyClanHoldPlayerInfo(player, pos, status, 0);
	else
	{
		for (UInt16 i = 0; i < 4; ++ i)
		{
			if (i != hold)
			{
				if (_holds[i].delBattler(player))
				{
					notifyClanHoldPlayerInfo(player, ((pos & 0xFF00) | (i + 1)), status, 0);
					break;
				}
			}
		}
	}
}

void ClanBattle::addClanBattlePlayer(ClanBattlePlayer * cbPlayer)
{
	UInt16 hold = (cbPlayer->hold & 0x00FF) - 1;
	_holds[hold].addBattler(cbPlayer);
	notifyClanHoldPlayerInfo(cbPlayer->player, cbPlayer->hold,  cbPlayer->status, 1);
}

bool ClanBattle::attackAssist(Player * atker, std::string& name, UInt32& turns, UInt32& damage, ClanHoldMonster * monster)
{
	if (monster == NULL || atker->getClan() == NULL || atker->getClanBattle() == NULL)
		return false;
	ClanCityHoldMonster * chm = dynamic_cast<ClanCityHoldMonster *>(monster);
	if (chm == NULL)
	{
		ERROR_LOG("Error Battle City Status Check !");
		return false;
	}
	const std::vector<GData::AssistantFData>& afds = chm->assist->_assistList;
	assert(!afds.empty());
	Battle::BattleSimulator bsim(atker->getLocation(), atker, afds[0].fighter->getName(), chm->level, false);
	atker->PutFighters(bsim, 0);
	UInt32 seed = TimeUtil::Now() + static_cast<UInt32>(atker->getId()) + static_cast<UInt32>(atker->getId() >> 32) + static_cast<UInt32>(atker->GetLev() << 16);
	URandom rnd(seed);
	std::vector<UInt8> positions = afds[0].pos;
	bsim.setPortrait(1, afds[0].fighter->favor);
	for (size_t i = 0; i < afds[0].fighterNum; ++ i)
	{
		size_t r = rnd(positions.size());
		bsim.newFighter(1, positions[r], afds[0].fighter);
		positions.erase(positions.begin()+r);
	}
	bsim.start();
	damage = bsim.getMaxObjHp(1) - bsim.getObjHp(1);
	Stream& packet = bsim.getPacket();
	if(packet.size() <= 8)
		return false;

	bool r = bsim.getWinner() == 1;
	if (r)
	{
		//玩家赢
		bsim.applyFighterHP(0, atker, false, 30);
		atker->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + bsim.getTurns() * 2);
	}
	else
	{
		atker->regenAll();
	}


	{
		Stream st(0x61);
		st << static_cast<UInt16>(r ? 0x101 : 0x100) << static_cast<UInt32>(0) << static_cast<UInt16>(0);
		st.append(&packet[8], packet.size() - 8);
		st << Stream::eos;
		atker->send(st);
	}

	notifyClanBattleReport(atker, name, (r ? 1 : 0), bsim.getId());
	turns = bsim.getTurns();

	return r;
}

bool ClanBattle::attackGuarder(Player * atker, std::string& name, UInt32& turns, UInt32& damage, ClanHoldMonster * monster)
{
	if (monster == NULL)
		return false;
	ClanCityHoldMonster * guarder = dynamic_cast<ClanCityHoldMonster *>(monster);
	if (guarder == NULL)
	{
		ERROR_LOG("Error Battle City Status Check !");
		return false;
	}
	const std::vector<GData::AssistantFData>& afds = guarder->assist->_assistList;
	assert(!afds.empty());
	Battle::BattleSimulator bsim(atker->getLocation(), atker, afds[0].fighter->getName(), guarder->level, false);

	atker->PutFighters( bsim, 0 );
	bsim.setPortrait(1, afds[0].fighter->favor);
	UInt32 seed = TimeUtil::Now() + static_cast<UInt32>(atker->getId()) + static_cast<UInt32>(atker->getId() >> 32) + static_cast<UInt32>(atker->GetLev() << 16);
	URandom rnd(seed);
	//设置boss的随机位置
	UInt8 bossPos = afds[0].pos[rnd(afds[0].pos.size())];
	bsim.newFighter(1, bossPos, afds[0].fighter);
	//设置monster的随机位置
	if (afds.size() >= 2)
	{
		std::vector<UInt8> positions = afds[1].pos;
		std::vector<UInt8>::iterator found = std::find(positions.begin(), positions.end(), bossPos);
		positions.erase(found);
		for (size_t i = 0; i < afds[1].fighterNum; ++ i)
		{
			UInt8 r = rnd(positions.size());
			bsim.newFighter(1, positions[r], afds[1].fighter);
			positions.erase(positions.begin()+r);
		}
	}

	bsim.start();
	damage = bsim.getMaxObjHp(1) - bsim.getObjHp(1);
	Stream& packet = bsim.getPacket();
	if(packet.size() <= 8)
		return false;

	bool r = bsim.getWinner() == 1;
	if (r)
	{
		//玩家赢
		bsim.applyFighterHP(0, atker, false, 30);
		atker->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + bsim.getTurns() * 2);
	}
	else
	{
		atker->regenAll();
	}

	{
		Stream st(0x61);
		st << static_cast<UInt16>(r ? 0x101 : 0x100) << static_cast<UInt32>(0) << static_cast<UInt16>(0);
		st.append(&packet[8], packet.size() - 8);
		st << Stream::eos;
		atker->send(st);
	}

	notifyClanBattleReport(atker, name, (r ? 1 : 0), bsim.getId());
	turns = bsim.getTurns();

	return r;
}




void ClanBattle::clearClanBattle()
{
	//清理战场
	Player * player = NULL;
	std::map<Player *, ClanBattlePlayer *>::iterator offset = _clanBattlePlayerLocs.begin();
	for (; offset != _clanBattlePlayerLocs.end(); ++ offset)
	{
		//跨地图
		player = offset->first;
		UInt16 spot = player->getLocation();
		CountryEnterStruct ces(true, player->isInCity() ? 1 : 0, spot);
		GameMsgHdr hdr(0x1F0, mapCollection.getCountryFromSpot(spot), player, sizeof(CountryEnterStruct));
		GLOBAL().PushMsg( hdr, &ces );

		Stream st(0x79);
		st << static_cast<UInt8>(16) << static_cast<UInt8>(1) << Stream::eos;
		player->send(st);

		player->setClanBattle(NULL);
		player->setBuffData(PLAYER_BUFF_CLANRECOVE, 0);
		player->setBuffData(PLAYER_BUFF_CLANMOVE, 0);
		player->setBuffData(PLAYER_BUFF_CLANBATTING, 0);
		player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
		player->delGlobalFlag(Player::ClanBattleFlag);
		player->delFlag(Player::ClanBattling);
		player->autoRegenAll();
		delete offset->second;
	}
	for (UInt8 i = 0; i < 4; ++ i)
	{
		SAFE_DELETE(_holds[i].guarder);
		std::map<std::string, ClanHoldMonster *>::iterator offset = _holds[i].assists.begin();
		for (; offset != _holds[i].assists.end(); ++ offset)
			delete offset->second;
		_holds[i].assists.clear();
		_holds[i].battlers.clear();
	}
	_clanBattlePlayerLocs.clear();
	_recoverClanBattlers.clear();
	_offClanBattlers.clear();
}

bool ClanBattle::isDeadBattlePlayer(Player * player)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found != _clanBattlePlayerLocs.end())
	{
		ClanBattlePlayer * cbp = found->second;
		if ((cbp->hasEnter & 0x0F) != 0 && cbp->status != 1)
			return true;
	}
	return false;
}

void ClanBattle::recoveBattlePlayer(Player * player, UInt8 recoveT)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end())
		return;
	ClanBattlePlayer * cbp = found->second;
	if (recoveT == 0)
	{
		if (cbp->status != 2)
		{
			Stream st(0x79);
			st << static_cast<UInt8>(17) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << Stream::eos;
			player->send(st);
			return;
		}
	}
	else
	{
		if (cbp->status == 1)
			return;
		UInt16 reliveNum = (cbp->reliveNum >> 8) + 1;
		UInt16 needGold = reliveNum >= 12 ? 100 : RecoveConsumeGold[reliveNum];
		if (player->getGold() < needGold)
		{
			player->sendMsgCode(0, 2008);
			return;
		}
		ConsumeInfo ci(ClanBattleRecov,0,0);
		player->useGold(needGold, &ci);
		if (reliveNum < 0x00FF)
			cbp->reliveNum += 0x0100;
		player->setBuffData(PLAYER_BUFF_CLANRECOVE, 0);
		player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, TimeUtil::Now()+60*60);
	}
	Stream st(0x79);
	st << static_cast<UInt8>(17) << static_cast<UInt8>(1) << player->getName() << static_cast<UInt8>(1) << Stream::eos;
	broadcastHold(cbp->hold, st);
	cbp->status = 1;
	if ((cbp->reliveNum & 0x00FF) < 0x00FF)
		cbp->reliveNum ++;
	notifyClanBattlePlayerCount();
	DB().PushUpdateData("UPDATE `clan_battler` SET `battleStatus` = 1, `reliveNum` = %u WHERE `id` = %u", cbp->reliveNum, cbp->id);
}


bool ClanBattle::incAwardClanBattleVictor(Player * player)
{
	Clan * clan = player->getClan();
	if (clan == NULL) 
		return false;
	Mutex::ScopedLock lk(clan->_mutex);
	ClanMember * clanMember = clan->getClanMember(player);
	if (clanMember == NULL)
		return false;
	if ((clanMember->enterCount >> 4) >= 3)
		return false;
	clanMember->enterCount += 0x10;
	DB().PushUpdateData("UPDATE `clan_player` SET `enterCount` = %u WHERE `playerId` = %"I64_FMT"u", clanMember->enterCount, player->getId());
	return true;
}

bool ClanBattle::setAwardClanBattleVictor(ClanBattlePlayer * cbp, bool rand)
{
	if ((cbp->hasEnter & 0xF0) > 0)
		return false;
	Clan * myClan = cbp->player->getClan();
	if (myClan == NULL) return false;
	UInt8 myClanLev = myClan->getLev();
	UInt8 battleClanLev = getOwnerClanLev();
	if (myClanLev > battleClanLev)
	{
		if (myClanLev - battleClanLev > 10)
			return false;
	}
	if (rand)
	{
		URandom rnd(TimeUtil::Now());
		if (rnd(100) <= 30)
		{
			if (!incAwardClanBattleVictor(cbp->player))
				return false;	
			cbp->hasEnter |= 0x10;
			DB().PushUpdateData("UPDATE `clan_battler` SET `hasEnter` = %u WHERE `id` = %u", cbp->hasEnter, cbp->id);
			SYSMSG(title, 444);
			SYSMSG(content, 445);
			Mail * pMail = cbp->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
			if (pMail != NULL)
				mailPackageManager.push(pMail->id, 9040, 1, true);
		}
	}
	else
	{
		if (!incAwardClanBattleVictor(cbp->player))
			return false;
		cbp->hasEnter |= 0x10;		
		DB().PushUpdateData("UPDATE `clan_battler` SET `hasEnter` = %u WHERE `id` = %u", cbp->hasEnter, cbp->id);
		SYSMSG(title, 444);
		SYSMSG(content, 445);
		Mail * pMail = cbp->player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
		if (pMail != NULL)
			mailPackageManager.push(pMail->id, 9040, 1, true);
	}

	return true;
}

void ClanBattle::notifyClanBattleOverReport(UInt8 succ)
{
	UInt16 cnt = 0;
	Player * player = NULL;
	ClanBattlePlayer * clanBattlePlayerEx = NULL;
	ClanDynamicMsg * clanDynamicMsg = getClanDynamicMsg();
	ClanBattleReportElem * cbrElem = &clanDynamicMsg->getCBReport(TimeUtil::SharpDay());
	std::map<Player *, ClanBattlePlayer *>::iterator offset;
	UInt32 enter_players[3] = {0};
	Stream st(0x7B);
	st << getBattleReportType() << succ << static_cast<UInt16>(0);
	for (offset = _clanBattlePlayerLocs.begin(); offset != _clanBattlePlayerLocs.end(); ++ offset)
	{
		clanBattlePlayerEx = offset->second;
		player = offset->first;
		if (player->getClan() != NULL)
		{
			if(World::_activityStage > 0)
			{
				if(offset->second->hasAttack > 0)
				{
					GameMsgHdr hdr(0x272, player->getCountry(), player, 0);
					GLOBAL().PushMsg(hdr, NULL);
				}
			}
			enter_players[clanBattlePlayerEx->side] ++;
			st << player->getName() << player->getClan()->getName() << player->GetLev() << static_cast<UInt16>(clanBattlePlayerEx->reliveNum & 0x00FF) << clanBattlePlayerEx->wins << clanBattlePlayerEx->maxSerailWins << static_cast<UInt8>(clanBattlePlayerEx->side == 2 ? 0 : 1)
				<< clanBattlePlayerEx->northEdurance << clanBattlePlayerEx->southEdurance << static_cast<UInt32>(succ == 0 ? clanBattlePlayerEx->grabAchieve : 0);
			++ cnt;
			cbrElem->second.push_back(ClanBattleReport(player->getName(), player->getClan()->getName(), player->GetLev(), static_cast<UInt16>(clanBattlePlayerEx->reliveNum & 0x00FF), clanBattlePlayerEx->wins, clanBattlePlayerEx->maxSerailWins, static_cast<UInt8>(clanBattlePlayerEx->side == 2 ? 0 : 1), \
				clanBattlePlayerEx->northEdurance, clanBattlePlayerEx->southEdurance, static_cast<UInt32>(succ == 0 ? clanBattlePlayerEx->grabAchieve : 0)));
		}
	}
	if(_clanBattlePlayerLocs.size() != 0)
	{
		DBLOG().PushUpdateData("insert into `clanbattle_states`(`server_id`, `clan_id`, `clan_name`, `enter_attackers`, `enter_defenders`, `enter_friends`, `is_win`, `this_day`) values(%u, %u, '%s', %u, %u, %u, %u, %u)", cfg.serverLogId, getOwnerClanId(), getOwnerClanName().c_str(), enter_players[2], enter_players[1], enter_players[0], succ, TimeUtil::SharpDay());
	}
	cbrElem->first = succ;
	st.data<UInt16>(4+2) = cnt;
	st << Stream::eos;
	broadcastHold(st);
}

void ClanBattle::setHasBattle(UInt8 b, bool writedb)
{
	if (_hasBattle == b)
		return;
	_hasBattle = b;
	if (writedb)
	{
		DB().PushUpdateData("UPDATE `clan` SET `hasBattle` = 1 WHERE `id` = %u", getOwnerClanId());
	}
}

void ClanBattle::sendClanBattleReport(Player * player, UInt32 time)
{
	ClanDynamicMsg * cdm = getClanDynamicMsg();
	if (cdm == NULL)
		return;
	Stream st(0x7B);
	st << getBattleReportType(true);
	cdm->makeCBMsgInfor(st, time);
	st << Stream::eos;
	player->send(st);
}

void ClanBattle::pushBattleCount(UInt8 battleType)
{
	DBLOG().PushUpdateData("insert `clanbattle_battle_count`(`server_id`, `battle_type`, `this_day`) values(%u, %u, %u)", cfg.serverLogId, battleType, TimeUtil::SharpDay());
}


bool ClanBattle::leaveClanCity(Player * player)
{
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found == _clanBattlePlayerLocs.end())
		return false;
	ClanBattlePlayer * cbPlayer = found->second;
	cbPlayer->hasEnter &= 0xF0 ;
	delClanBattlePlayer(cbPlayer->hold, cbPlayer->side, cbPlayer->status, player);
	notifyClanBattlePlayerCount();
	{
		//跨地图
		UInt16 spot = player->getLocation();
		CountryEnterStruct ces(true, player->isInCity() ? 1 : 0, spot);
		GameMsgHdr hdr(0x1F0, mapCollection.getCountryFromSpot(spot), player, sizeof(CountryEnterStruct));
		GLOBAL().PushMsg( hdr, &ces );
	}
	player->setClanBattle(NULL);
	if (isInBattling())
		player->setBuffData(PLAYER_BUFF_REENTERCLAN, TimeUtil::Now()+60);
	player->setBuffData(PLAYER_BUFF_CLANBATTING, 0);
	player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
	player->delGlobalFlag(Player::ClanBattleFlag);
	player->delFlag(Player::ClanBattling);
	player->autoRegenAll();
	DB().PushUpdateData("UPDATE `clan_battler` SET `hasEnter` = %u WHERE `id` = %u", cbPlayer->hasEnter, cbPlayer->id);

	return true;
}


//////////////////////////////////////////////////////////////////////////
ClanCityBattle::ClanCityBattle(Clan * c) : _achieve(0), _clan(c)
{
	for (UInt16 i = 0; i < 4; ++i)
	{
		_holds[i].hold = (0xF001 + i);
		memset(_holds[i].holdCount, 0x00, sizeof(_holds[i].holdCount));
	}
}

ClanCityBattle::~ClanCityBattle()
{

}


void ClanCityBattle::initClanBattle(bool)
{
	std::vector<UInt8>& cbTT = clanManager.getClanBattleTimeTable();
	UInt32 randTimePoint = uRand(cbTT.size());
	_battleTime = _nextBattleTime = cbTT[randTimePoint];
	_isInbattling = 0xFF;
	DB().PushUpdateData("UPDATE `clan` SET `battleTime` = %u, `nextBattleTime` = %u, `battleStatus` = %u WHERE `id` = %u", _battleTime, _nextBattleTime, _isInbattling, _clan->getId());
}


UInt8 ClanCityBattle::getEnterBattleClanCount(Player * player)
{
	Mutex::ScopedLock lk(_clan->_mutex);
	Clan::Members::iterator found = _clan->find(player);
	if (found == _clan->_members.end())
		return 0;
	return (*found)->enterCount & 0x0F;
}

UInt8 ClanCityBattle::getBattleTime()
{
	switch (_isInbattling)
	{
	case 0:
	case 1:
	case 0xFF:
		return _battleTime;
	case 2:
		return _nextBattleTime;
	}
	return 0;
}

UInt8 ClanCityBattle::getNextBattleTime()
{
	switch (_isInbattling)
	{
	case 0:
	case 1:
	case 0xFF:
		return _nextBattleTime;
	case 2:
		return _battleTime;
	}
	return 0; 
}

bool ClanCityBattle::canClanBattleOpen()
{
	if (TimeUtil::SharpDay(_clan->getFoundTime()) != WORLD().ThisDay())
		return true;
	return false;
}

UInt32 ClanCityBattle::getClanBattleOverTime()
{
	UInt32 now = TimeUtil::Now();
	UInt32 start = WORLD().ThisDay() + getBattleTime() * 1800;
	UInt32 end = start + (cfg.GMCheck ? (60 * 60) : (7 * 60 * 60));
	if (now > start && now < end)
		return end - now;
	return 0;
}

UInt8 ClanCityBattle::getClanRelation(Clan * clan)
{
	if (clan == NULL)
		return 2;
	if (getOwnerClanId() == clan->getId())
		return 0;
	return clan == getAllyClan() ? 1 : 2;
}


bool ClanCityBattle::enterTotem(Player * player)
{
	if (!_clan->existClanMember(player))
	{
		return false;
	}
	if (player->getClanBattle() != NULL)
	{
		ERROR_LOG("Error Enter player has entered totem : [%s][%llu]", player->getName().c_str(), player->getId());
		return false;
	}
	if (player->GetLev() < 40)
	{
		player->sendMsgCode(0, 2219);
		return false;
	}
	if (player->getBuffData(PLAYER_BUFF_REENTERCLAN) != 0)
	{
		player->sendMsgCode(0, 2224);
		return false;
	}
	UInt32 now = TimeUtil::Now();
	UInt32 thisDay = TimeUtil::SharpDay(0, now);
	if (TimeUtil::SharpDay(0, _clan->getFoundTime()) == thisDay)
	{
		player->sendMsgCode(0, 2214);
		return false;
	}
	Mutex::ScopedLock lk(_clan->_mutex);
	Clan::Members::iterator found = _clan->find(player);
	if (found == _clan->_members.end())
		return false;
	if (TimeUtil::SharpDay(0, (*found)->joinTime) == thisDay)
	{
		player->sendMsgCode(0, 2226);
		return false;
	}

	bool firstEnter = false;
	ClanBattlePlayer * clanBattlePlayerEx = NULL;
	std::map<Player *, ClanBattlePlayer *>::iterator foundcbp = _clanBattlePlayerLocs.find(player);
	if (foundcbp != _clanBattlePlayerLocs.end())
	{
		clanBattlePlayerEx = foundcbp->second;
		if (clanBattlePlayerEx->status != 1)
		{
			UInt32 recoveTime = clanBattlePlayerEx->player->getBuffData(PLAYER_BUFF_CLANRECOVE, now);
			notifyClanBattleRecoveData(clanBattlePlayerEx->player, (clanBattlePlayerEx->reliveNum >> 8) + 1, recoveTime > now ? recoveTime - now : 0);
		}
	}
	else
	{
		clanBattlePlayerEx = new(std::nothrow) ClanBattlePlayer(player, _holds[0].hold, 1, 0);
		if (clanBattlePlayerEx == NULL)
			return false;
		_clanBattlePlayerLocs.insert(std::make_pair(player, clanBattlePlayerEx));
		firstEnter = true;
	}
	clanBattlePlayerEx->hasEnter |= 0x01;
	clanBattlePlayerEx->offTime = 0;
	UInt16 hold = (clanBattlePlayerEx->hold & 0x00FF) - 1;
	if(hold > 3) return false;
	_holds[hold].addBattler(clanBattlePlayerEx);
	{
		//跨地图
		GameMsgHdr hdr(0x1F1, player->getThreadId(), player, 0);
		GLOBAL().PushMsg( hdr, NULL );
	}
	player->setClanBattle(this);
	if (firstEnter)
	{
		clanBattlePlayerEx->id = IDGenerator::gClanBatterRecordIDGenerator.ID();
		DB().PushUpdateData("REPLACE INTO `clan_battler`(`id`, `battler`, `battlerLev`, `battleClanId`, `battleClanTime`, `battleHold`, `hasEnter`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u)", clanBattlePlayerEx->id, player->getId(), player->GetLev(), _clan->getId(), TimeUtil::SharpDay(0, now), clanBattlePlayerEx->hold, clanBattlePlayerEx->hasEnter);
	}
	notifyClanBattleEnterInfo(player, 1, 0);
	notifyClanBattlePlayerMoveInfo(player, 0, clanBattlePlayerEx->hold);
	listClanHoldPlayerInfo(player, clanBattlePlayerEx->hold);
	notifyClanHoldPlayerInfo(player, clanBattlePlayerEx->hold, clanBattlePlayerEx->status, 1);
	if (isInBattling())
	{
		//处在战斗中
		player->setBuffData(PLAYER_BUFF_CLANBATTING, getBattleOverTime(now));
		player->addGlobalFlag(Player::ClanBattleFlag);
		player->addFlag(Player::ClanBattling);
		notifyClanBattleOverTime(player);
		notifyClanBattlePlayerCount();
		notifyClanBattleWinData(player);
		notifyClanHoldEndurance(player);
		DEBUG_LOG("Player [%s] enter clan [%u], side[%u]", player->getName().c_str(), _clan->getId(), clanBattlePlayerEx->side);
	}

	return true;
}


bool ClanCityBattle::enterClanCity(Player * player)
{
	UInt32 now = TimeUtil::Now();
	UInt32 thisDay = TimeUtil::SharpDay(0, now);
	if (TimeUtil::SharpDay(0, _clan->getFoundTime()) == thisDay)
	{
		player->sendMsgCode(0, 2214);
		return false;
	}
	if (!isInBattling())
	{
		if (_isInbattling == 0)
			player->sendMsgCode(0, 2213);
		else
			player->sendMsgCode(0, 2217);
		return false;
	}
	if (player->GetLev() < 40)
	{
		player->sendMsgCode(0, 2219);
		return false;
	}
	Clan * playerClan = player->getClan();
	if (playerClan == NULL || playerClan->getId() == _clan->getId())
	{
		ERROR_LOG("Error Clan City Player : [%s][%llu]", player->getName().c_str(), player->getId());
		return false;
	}
	Mutex::ScopedLock lk(playerClan->_mutex);
	Clan::Members::iterator playerFound = playerClan->find(player);
	if (playerFound == playerClan->end())
	{
		ERROR_LOG("Error Clan City Player, Cannot find the player : [%s][%llu]", player->getName().c_str(), player->getId());
		return false;
	}
	if (TimeUtil::SharpDay(0, (*playerFound)->joinTime) == thisDay)
	{
		player->sendMsgCode(0, 2226);
		return false;
	}
	ClanMember * clanMember = *playerFound;
	if (player->getClanBattle() != NULL)
		return false;
	if (player->getBuffData(PLAYER_BUFF_REENTERCLAN) != 0)
	{
		player->sendMsgCode(0, 2224);
		return false;
	}
	UInt8 side = getClanRelation(playerClan);
	bool firstEnter = false;
	ClanBattlePlayer * clanBattlePlayerEx = NULL;
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found != _clanBattlePlayerLocs.end())
	{
		clanBattlePlayerEx = found->second;
		if ((clanMember->enterCount & 0x0F) >= getMaxEnterCount() && clanBattlePlayerEx->hasAttack == 0)
		{
			player->sendMsgCode(0, 2209);
			return false;
		}
		if (clanBattlePlayerEx->status != 1)
		{
			UInt32 recoveTime = clanBattlePlayerEx->player->getBuffData(PLAYER_BUFF_CLANRECOVE, now);
			notifyClanBattleRecoveData(clanBattlePlayerEx->player, (clanBattlePlayerEx->reliveNum >> 8) + 1, recoveTime > now ? recoveTime - now : 0);
		}
	}
	else
	{
		if ((clanMember->enterCount & 0x0F) >= getMaxEnterCount())
		{
			player->sendMsgCode(0, 2209);
			return false;
		}
		clanBattlePlayerEx = new(std::nothrow) ClanBattlePlayer(player, side == 2 ? _holds[3].hold : _holds[0].hold, 1, side);
		if (clanBattlePlayerEx == NULL)
			return false;
		_clanBattlePlayerLocs.insert(std::make_pair(player, clanBattlePlayerEx));
		firstEnter = true;
	}
	clanBattlePlayerEx->hasEnter |= 0x01;
	UInt16 hold = (clanBattlePlayerEx->hold & 0x00FF) - 1;
	if(hold > 3)
		return false;
	_holds[hold].addBattler(clanBattlePlayerEx);
	clanBattlePlayerEx->offTime = 0;
	{
		GameMsgHdr hdr(0x1F1, player->getThreadId(), player, 0);
		GLOBAL().PushMsg( hdr, NULL );
	}
	player->setClanBattle(this);
	player->setBuffData(PLAYER_BUFF_CLANBATTING, getBattleOverTime(now));
	player->addFlag(Player::ClanBattling);
	player->addGlobalFlag(Player::ClanBattleFlag);
	if (firstEnter)
	{
		clanBattlePlayerEx->id = IDGenerator::gClanBatterRecordIDGenerator.ID();
		DB().PushUpdateData("REPLACE INTO `clan_battler`(`id`, `battler`, `battlerLev`, `battleClanId`, `battleClanTime`, `battleHold`, `hasEnter`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u)", clanBattlePlayerEx->id, player->getId(), player->GetLev(), _clan->getId(), thisDay, clanBattlePlayerEx->hold, clanBattlePlayerEx->hasEnter);
	}
	DB().PushUpdateData("UPDATE `clan_player` SET `enterCount` = %u  WHERE `playerId` = %"I64_FMT"u ", clanMember->enterCount, player->getId());
	notifyClanBattleEnterInfo(player, 2, 0, _clan->getName());
	notifyClanBattlePlayerMoveInfo(player, 0, clanBattlePlayerEx->hold);
	listClanHoldPlayerInfo(player, clanBattlePlayerEx->hold);
	notifyClanBattleOverTime(player);
	notifyClanBattlePlayerCount();
	notifyClanBattleWinData(player);
	notifyClanHoldEndurance(player);
	notifyClanHoldPlayerInfo(player, clanBattlePlayerEx->hold, clanBattlePlayerEx->status, 1);	
	DEBUG_LOG("Player [%s] enter clan [%u], side[%u]", player->getName().c_str(), _clan->getId(), clanBattlePlayerEx->side);

	return true;
}

void ClanCityBattle::notifyClanBattleTotemAttack()
{
	Mutex::ScopedLock lk(_clan->_mutex);
	ClanMember * mem = NULL;
	Clan::Members::iterator offset;
	for(offset = _clan->_members.begin(); offset != _clan->_members.end(); ++ offset)
	{
		mem = *offset;
		if (mem->player->getClanBattle() != static_cast<ClanBattle *>(this))
		{
			SYSMSG_SEND(428, mem->player);
			mem->player->send(_attackClanTips);
		}
	}
}

struct BattleOverMailSender
{
	BattleOverMailSender(const char * t, const char * c) : title(t), content(c) {}
	void operator()(ClanMember * cm)
	{
		cm->player->GetMailBox()->newMail(NULL, 0x01, title, content);
	}

	const char * title;
	const char * content;
};

void ClanCityBattle::notifyBattleOverMailNotify(UInt8 succ)
{
	SYSMSG(title, 440);
	if (succ == 0)
	{
		SYSMSGV(content, 442, _achieve);
		std::for_each(_clan->_members.begin(), _clan->_members.end(), BattleOverMailSender(title, content));
	}
	else
	{
		if (_holds[0].endurance == _holds[0].totalendurance)
		{
			SYSMSG(content, 441);
			std::for_each(_clan->_members.begin(), _clan->_members.end(), BattleOverMailSender(title, content));
		}
		else if (_holds[0].endurance < _holds[0].totalendurance)
		{
			SYSMSGV(content, 443);
			std::for_each(_clan->_members.begin(), _clan->_members.end(), BattleOverMailSender(title, content));
		}
	}
}

bool ClanCityBattle::setNextBattleTime(Player * player, UInt8 r)
{
	if (!_clan->existClanMember(player))
		return false;	
	if (getNextBattleTime() == r)
		return false;
	if (_clan->hasClanAuthority(player, 4) == 0)
		return false;
	if (!clanManager.validClanBattleTime(r))
		return false;
	switch (_isInbattling)
	{
	case 0:
	case 1:
	case 0xFF:
		_nextBattleTime = r;
		break;
	case 2:
		_battleTime = r;
		break;
	}
	DB().PushUpdateData("UPDATE `clan` SET `nextBattleTime` = %u WHERE `id` = %u", r, _clan->getId());

	return true;
}


void ClanCityBattle::incGrabAcheive(UInt32 ga)
{
	_grabAchieve += ga;
	DB().PushUpdateData("UPDATE `clan` SET `grabAchieve` = %u WHERE `id` = %u", _grabAchieve, _clan->getId());
}

UInt8 ClanCityBattle::getClanBattleStatus(UInt8 cbt, UInt32 now)
{
	UInt32 thisDay = TimeUtil::SharpDay(0, now);
	if (now < thisDay + cbt * 30 * 60)
		return 0;
	else if (now < thisDay + cbt * 30 * 60 + (cfg.GMCheck ? 60 * 60 : 7 * 60 * 60))
		return 1;
	else
		return 2;
}

UInt32 ClanCityBattle::getOwnerClanId()
{ 
	return _clan->getId(); 
}

UInt8 ClanCityBattle::getOwnerClanLev()
{
	return _clan->getLev();
}
std::string ClanCityBattle::getOwnerClanName() 
{
	return _clan->getName(); 
}

ClanDynamicMsg * ClanCityBattle::getClanDynamicMsg()
{
	return _clan->getClanDynamicMsg();
}

Clan * ClanCityBattle::getAllyClan() 
{
	return _clan->getAllyClan(); 
}

UInt16 ClanCityBattle::getBattleAchieve() 
{ 
	return _clan->getClanTech()->getBattleAchieve(); 
}

UInt8 ClanCityBattle::getLev() 
{
	return _clan->getLev(); 
}

/* returns:
0: ignore
1: process
2: start
3: over
4: delete overed */

int ClanCityBattle::testClanBattleStatus(UInt32 now, UInt32 today)
{
	switch(_isInbattling)
	{
	case 0:
		if(now >= today + _battleTime * 30 * 60)
			return 2;
		break;
	case 1:
		if(now >= today + _battleTime * 30 * 60 + (cfg.GMCheck ? (60 * 60) : (7 * 60 * 60)))
			return 3;
		return 1;
	default:
		return 4;
	}
	return 0;
}

void ClanCityBattle::configClanBattleCheck()
{	
	_isInbattling = 0;
	_nextBattleTime = _battleTime;
	_grabAchieve = 0;
	Clan::Members::iterator offset = _clan->_members.begin();
	//for backstage enter clancity count
	for (; offset != _clan->_members.end(); ++ offset)
	{
		(*offset)->achieveCount = 0;
		(*offset)->enterCount = 0;
	}
}

bool ClanCityBattle::attackPlayer2(ClanBattlePlayer * cbAtker, ClanBattlePlayer * cbDefer)
{
	Battle::BattleSimulator bsim(0x7FFF, cbAtker->player, cbDefer->player);
	cbAtker->player->PutFighters( bsim, 0 );
	cbDefer->player->PutFighters( bsim, 1 );
	bsim.start();
	bool res = bsim.getWinner() == 1;

	ClanBattlePlayer * cbDeath = NULL;
	if (res)
	{
		bsim.applyFighterHP(0, cbAtker->player, false, 30);
		cbAtker->player->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + bsim.getTurns() * 2);
		cbDefer->player->regenAll();
		cbDeath = cbDefer;
	}
	else
	{
		bsim.applyFighterHP(1, cbDefer->player, false, 30);
		cbAtker->player->regenAll();
		cbDeath = cbAtker;
	}

	if (cbDeath->hold != _holds[3].hold)
	{
		Stream st(0x79);
		st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt16>(_holds[3].hold) << Stream::eos;
		cbDeath->player->send(st);
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleHold` = 61444 WHERE `id` = %u", cbDeath->id);
	}
	delClanBattlePlayer(cbDeath->hold, cbDeath->side, cbDeath->status, cbDeath->player);
	cbDeath->hold = _holds[3].hold;
	cbDeath->status = 0;
	addClanBattlePlayer(cbDeath);
	listClanHoldPlayerInfo(cbDeath->player, cbDeath->hold);	//自己可以看到
	notifyClanBattlePlayerInfo(cbDeath->player, cbDeath->player, 1);
	UInt32 recoveTime = static_cast<UInt32>((isClanHoldDestroy(_holds[1].hold) || isClanHoldDestroy(_holds[2].hold)) ? 60 + bsim.getTurns() * 1.8f : 75 + bsim.getTurns() * 1.8f);
	cbDeath->player->setBuffData(PLAYER_BUFF_CLANRECOVE, TimeUtil::Now() + recoveTime);
	_recoverClanBattlers.insert(cbDeath);
	cbDeath->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
	notifyClanBattleRecoveData(cbDeath->player, (cbDeath->reliveNum >> 8) + 1, recoveTime);

	{
		Stream st(0x61);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		cbAtker->player->send(st);
	}
	{
		Stream st(0x61);
		st << static_cast<UInt8>(res ? 0 : 1) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		cbDefer->player->send(st);
	}	

	return true;
}

bool ClanCityBattle::attackPlayer(Player * atker, std::string deferName)
{
	if(atker->getBuffData(PLAYER_BUFF_ATTACKING))
	{
		atker->sendMsgCode(0, 2035);
		return false;
	}
	if (atker->getClan() == NULL || atker->getClanBattle() == NULL)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	ClanBattlePlayer * cbAtker = NULL;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		cbAtker = _holds[i].getBattler(atker);
		if (cbAtker != NULL)
			break;
	}
	if (cbAtker == NULL)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	UInt16 hold = (cbAtker->hold & 0x00FF) - 1;
	if (hold >= 4)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	atker->fixName(deferName);
	ClanBattlePlayer * cbDefer = _holds[hold].getBattler(deferName);
	if (cbDefer == NULL)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (cbDefer->player->getClan() == NULL || cbDefer->player->getClanBattle() != this)
	{
		ERROR_LOG("Cannot find the player battle Clan ! player[%llu][%s]", cbDefer->player->getId(), cbDefer->player->getName().c_str());
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (cbAtker->side != 2 && cbDefer->side != 2)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	UInt32 now = TimeUtil::Now();
	if (cbAtker->status != 1)
	{
		atker->sendMsgCode(0, 2204);
		atker->send(_attackClanBattlerStream);
		return false;
	}
	//可能是复活状态
	if (cbDefer->status != 1)
	{
		//在复活，不能攻击
		atker->sendMsgCode(0, 2204);
		atker->send(_attackClanBattlerStream);
		return false;
	}
	pushBattleCount(static_cast<UInt8>(1));
	if (atker->getClan() != _clan && cbAtker->hasAttack == 0)
	{
		incEnterClanCount(atker);
		cbAtker->hasAttack = 1;
		DB().PushUpdateData("UPDATE `clan_battler` SET `hasAttack` = 1 WHERE `id` = %u", cbAtker->id);
	}
	if ((cbAtker->side == 2 && cbDefer->side == 2) && (cbAtker->hold == cbDefer->hold))
	{
		if (cbAtker->player->getClan() != cbDefer->player->getClan())
			attackPlayer2(cbAtker, cbDefer);
		atker->send(_attackClanBattlerStream);
		return false;
	}
	Battle::BattleSimulator bsim(0x7FFF, atker, cbDefer->player);
	atker->PutFighters( bsim, 0 );
	cbDefer->player->PutFighters( bsim, 1 );
	bsim.start();
	bool res = bsim.getWinner() == 1;
	UInt32 reptid = bsim.getId();

	{
		Stream st(0x61);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		atker->send(st);
	}
	{
		Stream st(0x61);
		st << static_cast<UInt8>(res ? 0 : 1) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		cbDefer->player->send(st);
	}

	if (res)
	{
		//攻方赢
		//将守方送回复活点
		bsim.applyFighterHP(0, atker, false, 30);
		atker->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + bsim.getTurns() * 2);
		cbDefer->player->regenAll();
		cbAtker->wins ++;
		cbAtker->serailWins ++;	
		if (cbAtker->serailWins >= 3)
		{
			UInt8 tipsPos = (cbAtker->serailWins < 10 ? cbAtker->serailWins - 3 : 7);
			static UInt32 Tips[] = { 417, 418, 419, 420, 421, 422, 423, 424 };
			SYSMSGV(tipsContent, Tips[tipsPos], cbAtker->player->getName().c_str(), getOwnerClanName().c_str());
			Stream st(0x79);
			st << static_cast<UInt8>(15) << static_cast<UInt8>(2) << tipsContent << Stream::eos;
			broadcastHold(st);	
		}
		if (cbDefer->serailWins >= 3)
		{
			SYSMSGV(tipsContent, 425, cbAtker->player->getName().c_str(), getOwnerClanName().c_str(), cbDefer->player->getName().c_str());
			Stream st(0x79);
			st << static_cast<UInt8>(15) << static_cast<UInt8>(3) << tipsContent << Stream::eos;
			broadcastHold(st);
		}
		if (cbDefer->maxSerailWins < cbDefer->serailWins)
			cbDefer->maxSerailWins = cbDefer->serailWins;
		if (cbAtker->maxSerailWins < cbAtker->serailWins)
			cbAtker->maxSerailWins = cbAtker->serailWins;
		cbDefer->serailWins = 0;
		UInt16 recover = cbDefer->side == 2 ? _holds[3].hold : _holds[0].hold;
		if (cbDefer->hold != recover)
		{
			//在不同据点
			delClanBattlePlayer(cbDefer->hold, cbDefer->side, cbDefer->status, cbDefer->player);
			Stream st(0x79);
			st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt16>(recover) << Stream::eos;
			cbDefer->player->send(st);
			cbDefer->status = 0;
			cbDefer->hold = recover;
			addClanBattlePlayer(cbDefer);
			listClanHoldPlayerInfo(cbDefer->player, cbDefer->hold);	//自己可以看到
			notifyClanBattlePlayerInfo(cbDefer->player, cbDefer->player, 1);
		}
		else
		{
			//在相同据点
			Stream st(0x79);
			st << static_cast<UInt8>(17) << static_cast<UInt8>(1) << cbDefer->player->getName() << static_cast<UInt8>(0) << Stream::eos;
			cbDefer->status = 0;
			broadcastHold(cbDefer->hold, st);
		}
		DB().PushUpdateData("UPDATE `clan_battler` SET `wins` = %u, `serialWins` = %u, `maxSerialWins` = %u WHERE `id` = %u", cbAtker->wins, cbAtker->serailWins, cbAtker->maxSerailWins, cbAtker->id);
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleStatus` = 2, `serialWins` = 0, `maxSerialWins` = %u, `battleHold` =%u WHERE `id` = %u", cbDefer->maxSerailWins, recover, cbDefer->id);
		UInt32 recoveTime = static_cast<UInt32>(60 + bsim.getTurns() * 1.8f);
		cbDefer->player->setBuffData(PLAYER_BUFF_CLANRECOVE, now + recoveTime);
		cbDefer->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
		_recoverClanBattlers.insert(cbDefer);
		notifyClanBattleRecoveData(cbDefer->player, (cbDefer->reliveNum >> 8) + 1, recoveTime) ;
		setAwardClanBattleVictor(cbAtker);
	}
	else
	{
		//守方赢
		//将攻方送回复活点
		bsim.applyFighterHP(1, cbDefer->player, false, 30);
		atker->regenAll();
		cbDefer->wins ++;
		cbDefer->serailWins ++;
		if (cbDefer->serailWins >= 3)
		{
			UInt8 tipsPos = (cbDefer->serailWins < 10 ? cbDefer->serailWins - 3 : 7);
			static UInt32 Tips[] = { 417, 418, 419, 420, 421, 422, 423, 424 };
			SYSMSGV(tipsContent, Tips[tipsPos], cbDefer->player->getName().c_str(), getOwnerClanName().c_str());
			Stream st(0x79);
			st << static_cast<UInt8>(15) << static_cast<UInt8>(2) << tipsContent << Stream::eos;
			broadcastHold(st);	
		}
		if (cbAtker->serailWins >= 3)
		{
			SYSMSGV(tipsContent, 425, cbDefer->player->getName().c_str(), getOwnerClanName().c_str(), cbAtker->player->getName().c_str());
			Stream st(0x79);
			st << static_cast<UInt8>(15) << static_cast<UInt8>(3) << tipsContent << Stream::eos;
			broadcastHold(st);
		}
		if (cbDefer->maxSerailWins < cbDefer->serailWins)
			cbDefer->maxSerailWins = cbDefer->serailWins;
		if (cbAtker->maxSerailWins < cbAtker->serailWins)
			cbAtker->maxSerailWins = cbAtker->serailWins;
		cbAtker->serailWins = 0;
		UInt16 recover = cbAtker->side == 2 ? _holds[3].hold : _holds[0].hold;
		if (cbAtker->hold != recover)
		{
			//在不同据点
			delClanBattlePlayer(cbAtker->hold, cbAtker->side, cbAtker->status, atker);
			Stream st(0x79);
			st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt16>(recover) << Stream::eos;
			cbAtker->player->send(st);
			cbAtker->hold = recover;
			cbAtker->status = 0;
			addClanBattlePlayer(cbAtker);
			listClanHoldPlayerInfo(cbAtker->player, cbAtker->hold);	//自己可以看到
			notifyClanBattlePlayerInfo(cbAtker->player, cbAtker->player, 1);
		}
		else
		{
			//在相同据点
			Stream st(0x79);
			st << static_cast<UInt8>(17) << static_cast<UInt8>(1) << cbAtker->player->getName() << static_cast<UInt8>(0) << Stream::eos;
			cbAtker->status = 0;
			broadcastHold(cbAtker->hold, st);
		}
		DB().PushUpdateData("UPDATE `clan_battler` SET `wins` = %u, `serialWins` = %u, `maxSerialWins` = %u WHERE `id` = %u", cbDefer->wins, cbDefer->serailWins, cbDefer->maxSerailWins, cbDefer->id);
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleStatus` = 2, `serialWins` = 0, `maxSerialWins` = %u, `battleHold` =%u WHERE `id` = %u", cbAtker->serailWins, recover, cbAtker->id);
		UInt32 recoveTime = static_cast<UInt32>((isClanHoldDestroy(_holds[1].hold) || isClanHoldDestroy(_holds[2].hold)) ? 60 + bsim.getTurns() * 1.8f : 75 + bsim.getTurns() * 1.8f);
		cbAtker->player->setBuffData(PLAYER_BUFF_CLANRECOVE, now + recoveTime);
		cbAtker->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
		_recoverClanBattlers.insert(cbAtker);
		notifyClanBattleRecoveData(cbAtker->player, (cbAtker->reliveNum >> 8) + 1, recoveTime);
		setAwardClanBattleVictor(cbDefer);
	}
	notifyClanBattleWinData(cbAtker->player);
	notifyClanBattleWinData(cbDefer->player);
	notifyClanBattlePlayerCount();
	notifyClanBattleReport(atker, cbDefer->player, (res ? 1 : 0), reptid);
	atker->send(_attackClanBattlerStream);

	return true;
}



bool ClanCityBattle::attackNpc(Player * atker, std::string npcName)
{
	if(atker->getBuffData(PLAYER_BUFF_ATTACKING))
	{
		atker->sendMsgCode(0, 2035);
		return false;
	}
	ClanBattlePlayer * cbAtker = NULL;
	std::set<ClanBattlePlayer *>::iterator found;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		cbAtker = _holds[i].getBattler(atker);
		if (cbAtker != NULL)
			break;
	}
	if (cbAtker == NULL)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (cbAtker->side != 2)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	UInt16 hold = (cbAtker->hold & 0x00FF) - 1;
	if (hold >= 3)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (cbAtker->status != 1)
	{
		atker->sendMsgCode(0, 2204);
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (_holds[hold].aliveDeferSize() > 0)
	{
		atker->send(_attackClanBattlerStream);
		if (hold != 0)
		{
			SYSMSG_SEND(427, atker);
		}
		else
		{
			SYSMSG_SEND(431, atker);
		}
		return false;
	}
	bool r = false;
	UInt32 damge = 0;
	UInt32 turns = 0;
	ClanHoldMonster * cat = NULL;
	if (_holds[hold].guarder != NULL && npcName == _holds[hold].guarder->name)
	{
		if (hold == 1 || hold == 2)
		{
			if (!_holds[hold].assists.empty())	//存在存活的怪物
			{
				SYSMSG_SEND(427, atker);
				atker->send(_attackClanBattlerStream);
				return false;
			}
		}
		r = attackGuarder(atker, _holds[hold].guarder->name, turns, damge, _holds[hold].guarder);
	}
	else
	{
		//攻击护卫者
		std::map<std::string, ClanHoldMonster *>::iterator found = _holds[hold].assists.find(npcName);
		if (found == _holds[hold].assists.end())
			return false;
		cat = found->second;
		r = attackAssist(atker, cat->name, turns, damge, cat);
	}
	if (atker->getClan() != _clan && cbAtker->hasAttack == 0)
	{
		incEnterClanCount(atker);
		cbAtker->hasAttack = 1;
		DB().PushUpdateData("UPDATE `clan_battler` SET `hasAttack` = 1 WHERE `id` = %u", cbAtker->id);
	}
	_isInAttacking = true;
	if (r)
	{
		setAwardClanBattleVictor(cbAtker);
		if (_holds[hold].guarder != NULL && npcName == _holds[hold].guarder->name)
		{
			//打败守卫者
			UInt16 endurance = _holds[hold].endurance <= 1 ? _holds[hold].endurance : 1;
			_holds[hold].endurance -= endurance;
			switch (hold)
			{
			case 0:				
				cbAtker->grabAchieve ++;	//记录杀死宗族守卫者次数，最后结算
				DB().PushUpdateData("UPDATE `clan` SET `hallEdurance` = %u WHERE `id` = %u", _holds[hold].endurance, getOwnerClanId());
				DB().PushUpdateData("UPDATE `clan_battler` SET `grabAchieve` = %u WHERE `id` = %u",cbAtker->grabAchieve, cbAtker->id);
				break;
			case 1:
				cbAtker->southEdurance += endurance;
				DB().PushUpdateData("UPDATE `clan` SET `southEdurance` = %u WHERE `id` = %u", _holds[hold].endurance, getOwnerClanId());
				DB().PushUpdateData("UPDATE `clan_battler` SET `southEdurance` = %u WHERE `id` = %u",cbAtker->southEdurance, cbAtker->id);
				break;
			case 2:
				cbAtker->northEdurance += endurance;
				DB().PushUpdateData("UPDATE `clan` SET `northEdurance` = %u WHERE `id` = %u", _holds[hold].endurance, getOwnerClanId());
				DB().PushUpdateData("UPDATE `clan_battler` SET `northEdurance` = %u WHERE `id` = %u",cbAtker->northEdurance, cbAtker->id);
				break;
			}
			notifyClanBattleTotemAttack();
			notifyClanHoldEndurance();
			if (_holds[hold].endurance == 0)
			{
				notifyClanHoldGuarderEnterInfo(cbAtker->hold, 1);
				if (hold == 0)
				{
					overClanBattle(0);
					clearClanBattle();
				}
				else 
				{
					if (hold == 1)
					{
						if (!_firstAttack[0])
						{
							_firstAttack[0] = true;
							SYSMSG_BROADCASTV(458, getOwnerClanName().c_str());
						}
					}
					else if (hold == 2)
					{
						if (!_firstAttack[1])
						{
							_firstAttack[1] = true;
							SYSMSG_BROADCASTV(458, getOwnerClanName().c_str());
						}
					}
				}
			}
		}
		else
		{
			notifyClanHoldAssistEnterInfo(npcName, cbAtker->hold, 1);
			cat->buff = TimeUtil::Now();
			_recoverClanHoldAssistant[npcName] = cat;
			_holds[hold].assists.erase(npcName);
		}
	}
	else
	{
		//玩家被打败
		//打输
		delClanBattlePlayer(cbAtker->hold, cbAtker->side, cbAtker->status, atker);
		Stream st(0x79);
		st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt16>(_holds[3].hold) << Stream::eos;
		cbAtker->player->send(st);
		cbAtker->hold = _holds[3].hold;
		cbAtker->status = 0;
		addClanBattlePlayer(cbAtker);
		listClanHoldPlayerInfo(cbAtker->player, cbAtker->hold);	//自己可以看到
		notifyClanBattlePlayerInfo(cbAtker->player, cbAtker->player, 1);
		UInt32 recoveTime = static_cast<UInt32>((isClanHoldDestroy(_holds[1].hold) || isClanHoldDestroy(_holds[2].hold)) ? 60 + 1.8f * turns : 75 + 1.8f * turns);
		cbAtker->player->setBuffData(PLAYER_BUFF_CLANRECOVE, TimeUtil::Now() + recoveTime);
		cbAtker->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
		_recoverClanBattlers.insert(cbAtker);
		notifyClanBattleRecoveData(cbAtker->player, (cbAtker->reliveNum >> 8) + 1, recoveTime);
		notifyClanBattlePlayerCount();
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleStatus` = 2, `battleHold` = %u WHERE `id` = %u", _holds[3].hold, cbAtker->id);
	}
	notifyClanBattleWinData(cbAtker->player);

	atker->send(_attackClanBattlerStream);
	pushBattleCount(2);
	return true;
}

bool ClanCityBattle::resumeClanBattleData(UInt32 now)
{	
	UInt32 thisDay = TimeUtil::SharpDay(0, now);
	bool resume = false, sameDay = false;
	bool start = false;
	if (_isInbattling == 0xFF)
	{
		if (thisDay != TimeUtil::SharpDay(0, _clan->getFoundTime()))
			resume = true;
	}
	else
	{
		if (thisDay != _battleThisDay)
		{
			_isInbattling = 0;
			_battleTime = _nextBattleTime;
		}
		else
		{
			sameDay = true;
		}
		resume = true;
	}

	if (resume)
	{
		if (_isInbattling == 0 || _isInbattling == 1)
			_isInbattling = getClanBattleStatus(_battleTime, now);
		switch (_isInbattling)
		{
		case 0:
			clanManager.addOnBattleClan(this);
			break;
		case 1:
			clanManager.addOnBattleClan(this);
			if (sameDay)
			{
				startClanBattle(thisDay, false);
				_holds[0].endurance = _hallEdurance;
				_holds[1].endurance = _southEdurance;
				_holds[2].endurance = _northEdurance;
				_isInAttacking = (_hallEdurance != _holds[0].totalendurance || _southEdurance != _holds[1].totalendurance || _northEdurance != _holds[2].totalendurance);
				setClanBattlerStartData(false);
			}
			else
			{
				startClanBattle(thisDay);
				setClanBattlerStartData(false);	
			}
			start = true;
			break;
		case 2:
			clanManager.addOffBattleClan(this);
			std::swap(_nextBattleTime, _battleTime);
		}
	}
	return start;
}

void ClanCityBattle::update(UInt32 now)
{
	//check = false;
	ClanBattlePlayer * cbPlayer;
	std::set<ClanBattlePlayer *>::iterator rcIter = _recoverClanBattlers.begin();
	for (; rcIter != _recoverClanBattlers.end();)
	{
		cbPlayer = *rcIter;
		if (cbPlayer->status == 0 && cbPlayer->player->getBuffData(PLAYER_BUFF_CLANRECOVE, now) == 0)
		{
			cbPlayer->status = 2;
			_recoverClanBattlers.erase(rcIter++);
		}
		else
			++ rcIter;
	}
	//城门怪物复活检查
	std::map<std::string, ClanHoldMonster *>::iterator rcIter2 = _recoverClanHoldAssistant.begin();
	for (; rcIter2 != _recoverClanHoldAssistant.end();)
	{
		ClanHoldMonster * cat = rcIter2->second;
		if (now > cat->buff && now - cat->buff > 45)
		{
			UInt16 hold = (cat->hold & 0x00FF) - 1;
			if(hold > 3)
				continue;
			cat->buff = 0;
			_holds[hold].assists[rcIter2->first] = cat;
			notifyClanHoldAssistEnterInfo(cat->name, cat->hold, 0);
			_recoverClanHoldAssistant.erase(rcIter2++);
		}
		else
			++ rcIter2;
	}

	//下线待踢者
	std::set<ClanBattlePlayer *>::iterator rcIter3 = _offClanBattlers.begin();
	for (; rcIter3 != _offClanBattlers.end();)
	{
		cbPlayer = *rcIter3;
		if (cbPlayer->offTime == 0)
			_offClanBattlers.erase(rcIter3++);
		else if (now > cbPlayer->offTime && now - cbPlayer->offTime > 2 * 60)
		{
			++ rcIter3;
			kickClanBattler(cbPlayer->player);
			cbPlayer->offTime = 0;
		}
		else
			++ rcIter3;
	}

	//耐久度恢复检查
	for (UInt8 i = 1; i <= 2; ++ i)
	{
		if (_holds[i].atkerSize() > 0 || _holds[i].deferSize() <= 0 || _holds[i].endurance >= _holds[i].totalendurance)
		{
			if (_holds[i].buff != 0)
			{
				_holds[i].buff = 0;
				notifyClanHoldEndurance(_holds[i]);
			}
		}
		else if (_holds[i].buff == 0)
		{
			_holds[i].buff = now;
			notifyClanHoldEndurance(_holds[i]);
		}
		else if (now > _holds[i].buff && now - _holds[i].buff >= 25)
		{
			_holds[i].endurance ++;
			_holds[i].buff = now;
			notifyClanHoldEndurance(_holds[i]);
		}
	}
}


UInt32 ClanCityBattle::getBattleOverTime(UInt32 now)
{
	return (TimeUtil::SharpDay(0, now) + static_cast<UInt32>(getBattleTime()) * 30 * 60 + (cfg.GMCheck ? (60 * 60) : (7 * 60 * 60)));
}

void ClanCityBattle::closingBattlerAward(UInt8 succ)
{
	if (succ != 0) 
		return;
	UInt32 clanId;
	float gainScore = 0.0f;
	Clan * clan = NULL;
	ClanTech * techs = _clan->getClanTech();
	ClanBattlePlayer * cbp = NULL;
	ClanMember * cm = NULL;
	UInt32 thisDay = TimeUtil::SharpDay();
	typedef std::multimap<float, ClanBattlePlayer *, std::greater<float> > CBPTable;
	std::map<UInt32, CBPTable> awardExpTable;
	std::map<UInt32, float> awardExpSum;
	UInt16 battleAchieve = techs->getBattleAchieve() / 3;
	_achieve = 0;
	std::map<Player *, ClanBattlePlayer *>::iterator offset = _clanBattlePlayerLocs.begin();
	for (; offset != _clanBattlePlayerLocs.end(); ++ offset)
	{
		cbp = offset->second;
		if (cbp->side != 2)
			continue;
		if (cbp->player->getClan() == NULL)
			continue;
		if (cbp->grabAchieve != 0)
		{
			cbp->grabAchieve = static_cast<UInt32>(cbp->grabAchieve * 1.0f / _holds[0].totalendurance * battleAchieve + 0.5f);
			clan = cbp->player->getClan();
			if (clan->getLev() > _clan->getLev() + 10)
				cbp->grabAchieve = static_cast<UInt32>(cbp->grabAchieve * (4.0f / (clan->getLev() - _clan->getLev() - 6)));
			if (cbp->grabAchieve <= 0) cbp->grabAchieve = 1;
			_achieve += cbp->grabAchieve;
			clan->getClanTech()->addAchieve(cbp->grabAchieve);
			Mutex::ScopedLock lk(clan->_mutex);
			Clan::Members::iterator found = clan->find(cbp->player);
			if (found != clan->end())
			{
				cm = (*found);
				std::string oldLeaderName = (_clan->_members.empty() ? "" : (*_clan->_members.begin())->player->getName());
				clan->_members.erase(found);
				cm->proffer += cbp->grabAchieve;
				clan->_members.insert(cm);
				clan->getClanBattle()->incGrabAcheive(cbp->grabAchieve);
				clan->updateRank(cm, oldLeaderName);
				clan->setProffer(clan->getProffer() + cbp->grabAchieve);
				DB().PushUpdateData("UPDATE `clan_player` SET `proffer` = %u WHERE `playerId` = %"I64_FMT"u", cm->proffer, cm->player->getId());
				DB().PushUpdateData("UPDATE `clan_battler` SET `grabAchieve` = %u WHERE `id` = %u", cbp->grabAchieve, cbp->id);
			}
			gainScore = cbp->wins * 1.0f + (cbp->northEdurance + cbp->southEdurance) * 1.5f + cbp->grabAchieve * 2.0f;
			clanId = cbp->player->getClan()->getId();
			awardExpTable[clanId].insert(std::make_pair(gainScore, cbp));
			awardExpSum[clanId] += cbp->grabAchieve;
			{
				Stream st;
				clan->getClanDynamicMsg()->addCDMsg(4, _clan->getName(), cbp->grabAchieve, _clan->getId(), thisDay, &st);
				clan->broadcast(st);
			}
		}
		if ((cbp->hasEnter & 0xF0) == 0 && cbp->hasAttack != 0)
			setAwardClanBattleVictor(cbp, false);
		cbp->player->GetTaskMgr()->DoAcceptedTask(80113);
	}
	techs->delAchieve(_achieve);
	DEBUG_LOG("Clan [%u] lost achieve [%u]", _clan->getId(), _achieve);
	for (std::map<UInt32, CBPTable>::iterator offset = awardExpTable.begin(); offset != awardExpTable.end(); ++ offset)
	{
		Player * player = NULL;
		UInt16 awardSize = static_cast<UInt16>(offset->second.size());
		CBPTable::iterator awardIter = offset->second.begin();
		for (UInt16 row = 1; awardIter != offset->second.end(); ++ awardIter, ++ row)
		{
			player = awardIter->second->player;
			UInt64 exp = static_cast<UInt64>(2 * 200 * awardExpSum[player->getClan()->getId()] * (awardSize - row + 1) / (awardSize * (awardSize + 1)));
			GameMsgHdr hdr(0x203, player->getThreadId(), player, sizeof(UInt64));
			GLOBAL().PushMsg(hdr, &exp);
			SYSMSG_SENDV(446, player, exp);
			SYSMSG(title, 440);
			SYSMSGV(content, 447, awardIter->second->grabAchieve, exp);
			player->GetMailBox()->newMail(NULL, 0x01, title, content);
		}
	}
}

bool ClanCityBattle::incEnterClanCount(Player * player)
{
	Clan * clan = player->getClan();
	if (clan == NULL) 
		return false;
	Mutex::ScopedLock lk(clan->_mutex);
	Clan::Members::iterator found = clan->find(player);
	if (found == clan->end())
		return false;
	if (((*found)->enterCount & 0x0F) >= getMaxEnterCount())
		return false;
	(*found)->enterCount ++;
	return true;
}

UInt16 ClanCityBattle::getWillGainBattlerAward(Clan * c)
{
	if (c == NULL) return 0;
	UInt16 battleAchieve = _clan->getClanTech()->getBattleAchieve() / 3;
	if (c->getLev() > _clan->getLev() + 10)
		return static_cast<UInt16>(4.0f / (c->getLev() - _clan->getLev() - 6) * battleAchieve);
	return battleAchieve;
}

//////////////////////////////////////////////////////////////////////////
ClanRobBattle::ClanRobBattle(UInt8 s, Clan * c) : _status(s), _heroClanId(0), _heroClanLev(0), _heroClanAtkCount(0), _heroClanRank(0), _clan(c)
{
	for (UInt16 i = 0; i < 4; ++i)
	{
		_holds[i].hold = (0xF101 + i);
		memset(_holds[i].holdCount, 0x00, sizeof(_holds[i].holdCount));
	}
	for (UInt8 i = 0; i < 2; ++ i)
		for (UInt8 j = 0; j < 2; ++ j)
			_display[i][j] = false;
	_clanDynamicMsg = new ClanDynamicMsg();
}

ClanRobBattle::~ClanRobBattle()
{
	delete _clanDynamicMsg;
}

void ClanRobBattle::initClanBattle(bool next)
{
	SYSMSG(heroClanName, 465);
	_heroClanName = heroClanName;
	if (_heroClanId != 0)
		_clan = globalClans[_heroClanId];
	if (_clan != NULL)
	{
		_status = 1;
	}
	else
	{
		_status = 0;
		_heroClanId = 0;
	}
	UInt32 now = TimeUtil::Now();
	if (TimeUtil::Day(now) == 0 && (next || (now < TimeUtil::SharpDay(0, now) + _battleTime * 30 * 30 || getBattleThisDay() != TimeUtil::SharpDay(0, now))))
	{
		_status = 0;
		_clan = NULL;
		DB().PushUpdateData("UPDATE `clan` SET `allyClan` = 0 WHERE `id` = 0");
	}
}

UInt8 ClanRobBattle::getOwnerClanLev()
{
	if (_clan != NULL)
		return _clan->getLev();
	return _heroClanRank;
}

void ClanRobBattle::setOwnerClanLev(UInt8 lev, bool writedb)
{
	_heroClanLev = lev;
	if (writedb)
	{
		DB().PushUpdateData("UPDATE `clan` SET `rank` = %u WHERE `id` = 0", lev);
	}
}

void ClanRobBattle::setOwnerClanAtkCount(UInt16 cnt, bool writedb)
{
	_heroClanAtkCount = cnt;
	if (writedb)
	{
		DB().PushUpdateData("UPDATE `clan` SET `foundTime` = %u WHERE `id` = 0", cnt);
	}
}

void ClanRobBattle::setOwnerClanRank(UInt8 rank, bool writedb)
{
	_heroClanRank = rank;
	if (writedb)
	{
		DB().PushUpdateData("UPDATE `clan` SET `rank` = %u WHERE `id` = 0", rank);
	}
}

std::string ClanRobBattle::getOwnerClanName()
{
	if (_clan == NULL)
		return _heroClanName;
	SYSMSGV(name, 464, _clan->getName().c_str());
	return name;
}

/* returns:
0: ignore
1: process
2: start
3: over
4: delete overed */
int ClanRobBattle::testClanBattleStatus(UInt32 now, UInt32 today)
{
	switch(_isInbattling)
	{
	case 0:
		if(now >= today + _battleTime * 30 * 60)
			return 2;
		break;
	case 1:
		if(now >= today + _battleTime * 30 * 60 + (cfg.GMCheck ? (30 * 60) : (7 * 60 * 60)))
			return 3;
		return 1;
	default:
		return 4;
	}
	return 0;
}

//TODO
void ClanRobBattle::configClanBattleCheck()
{	
	_isInbattling = 0;
	_battleTime = _nextBattleTime = 39;
	_grabAchieve = 0;
}

void ClanRobBattle::configClanBattleData(bool writedb)
{
	if (writedb)
	{
		_sst.clear();
		_rowSst.clear();
		_cCbp.clear();
	}
	if (_status == 1)
		return ClanBattle::configClanBattleData(writedb);

	//综合宗族技艺配置相关战斗数据
	_holds[0].endurance = _holds[0].totalendurance = 30;
	_holds[1].endurance = _holds[1].totalendurance = 30;
	_holds[2].endurance = _holds[2].totalendurance = 30;
	_holds[3].endurance = _holds[3].totalendurance = 0;
	if (writedb)
		DB().PushUpdateData("UPDATE `clan` SET `southEdurance` = %u, `northEdurance` = %u, `hallEdurance` = %u WHERE `id` = %u", _holds[1].endurance, _holds[2].endurance, _holds[0].endurance, getOwnerClanId());

	//////////////////////////////////////////////////////////////////////////
	UInt8 bossLev = getBattlerLev();
	//配置宗祠护卫着，守卫着
	{
		GData::NpcGroup * guarder = GData::clanRobMonster._clanTotemGuarderBoss;
		if (guarder == NULL)
		{
			fprintf(stderr, "Error Clan Rob Monster Configure : clanId = %u", getOwnerClanId());
			return;
		}
		SYSMSG(name, 411);
		_holds[0].guarder = new(std::nothrow) ClanRobHoldMonster(name, bossLev, guarder->getClass(), _holds[0].hold, 0, guarder);
		assert(_holds[0].guarder != NULL);
		for (size_t i = 0; i < GData::clanRobMonster._clanTotemAssistBoss.size(); ++ i)
		{
			GData::NpcGroup * assist = GData::clanRobMonster._clanTotemAssistBoss[i];
			if (assist == NULL)
			{
				fprintf(stderr, "Error Clan Rob Monster Configure : clanId = %u", getOwnerClanId());
				return;
			}
			ClanHoldMonster * monster = new(std::nothrow) ClanRobHoldMonster(assist->getName(), bossLev, assist->getClass(), _holds[0].hold, 0, assist);
			_holds[0].assists.insert(std::make_pair(assist->getName(), monster));
		}
	}

	UInt32 assistId;
	//配置南门守卫者， 护卫者
	{
		GData::NpcGroup * guarder = GData::clanRobMonster._clanSouthGuarderBoss;
		if (guarder == NULL)
		{
			fprintf(stderr, "Error Clan Rob Monster Configure : clanId = %u", getOwnerClanId());
			return;
		}
		SYSMSG(name, 412);
		_holds[1].guarder = new(std::nothrow) ClanRobHoldMonster(name, bossLev, guarder->getClass(), _holds[1].hold, 0, guarder);
		assert(_holds[1].guarder != NULL);
		for (size_t i = 0; i < GData::clanRobMonster._clanSouthBoss.size(); ++ i)
		{
			GData::NpcGroup * assist = GData::clanRobMonster._clanSouthBoss[i];
			if (assist == NULL)
			{
				fprintf(stderr, "Error Clan Rob Monster Configure : clanId = %u", getOwnerClanId());
				return;
			}
			assistId = assist->getId();
			if (assistId == 4930 || assistId == 4931)
				continue;
			ClanHoldMonster * monster = new(std::nothrow) ClanRobHoldMonster(assist->getName(), bossLev, assist->getClass(), _holds[1].hold, 0, assist);
			_holds[1].assists.insert(std::make_pair(assist->getName(), monster));
		}

	}

	//配置北门守卫者， 护卫者
	{
		GData::NpcGroup * guarder = GData::clanRobMonster._clanNorthGuarderBoss;
		if (guarder == NULL)
		{
			fprintf(stderr, "Error Clan Rob Monster Configure : clanId = %u", getOwnerClanId());
			return;
		}
		SYSMSG(name, 413);
		_holds[2].guarder = new(std::nothrow) ClanRobHoldMonster(name, bossLev, guarder->getClass(), _holds[2].hold, 0, guarder);
		assert(_holds[2].guarder != NULL);
		for (size_t i = 0; i < GData::clanRobMonster._clanNorthBoss.size(); ++ i)
		{
			GData::NpcGroup * assist = GData::clanRobMonster._clanNorthBoss[i];
			if (assist == NULL)
			{
				fprintf(stderr, "Error Clan Rob Monster Configure : clanId = %u", getOwnerClanId());
				return;
			}
			assistId = assist->getId();
			if (assistId == 4930 || assistId == 4931)
				continue;
			ClanHoldMonster * monster = new(std::nothrow) ClanRobHoldMonster(assist->getName(), bossLev, assist->getClass(), _holds[2].hold, 0, assist);
			_holds[2].assists.insert(std::make_pair(assist->getName(), monster));
		}
	}
	initDisplayMonsterStatus();
}

bool ClanRobBattle::resumeClanBattleData(UInt32 now)
{	
	UInt32 thisDay = TimeUtil::SharpDay(0, now);
	bool sameDay = false;
	bool start = false;
	if (thisDay != _battleThisDay)
	{
		_isInbattling = 0;
		_battleTime = _nextBattleTime;
	}
	else
	{
		sameDay = true;
	}

	if (_isInbattling == 0 || _isInbattling == 1)
		_isInbattling = ClanRobBattle::getClanBattleStatus(_battleTime, now);
	initClanBattle(false);
	switch (_isInbattling)
	{
	case 0:
		clanManager.addOnBattleClan(this);
		break;
	case 1:
		clanManager.addOnBattleClan(this);
		if (sameDay)
		{
			startClanBattle(thisDay, false);
			_holds[0].endurance = _hallEdurance;
			_holds[1].endurance = _southEdurance;
			_holds[2].endurance = _northEdurance;
			_isInAttacking = (_hallEdurance != _holds[0].totalendurance || _southEdurance != _holds[1].totalendurance || _northEdurance != _holds[2].totalendurance);
			if (_status == 0)
			{
				for (UInt8 hold = 1; hold <= 2; ++ hold)
					displayHiddenMonster(hold, false);
			}
			setClanBattlerStartData(false);
		}
		else
		{
			startClanBattle(thisDay);
			setClanBattlerStartData(false);	
		}
		start = true;
		break;
	case 2:
		clanManager.addOffBattleClan(this);
		std::swap(_nextBattleTime, _battleTime);
	}
	return start;
}

UInt32 ClanRobBattle::getClanBattleOverTime()
{
	UInt32 now = TimeUtil::Now();
	UInt32 start = WORLD().ThisDay() + getBattleTime() * 1800;
	UInt32 end = start + 60 * 30;
	if (now > start && now < end)
		return end - now;
	return 0;
}

UInt8 ClanRobBattle::getClanRelation(Clan * clan)
{
	return clan == _clan ? 0 : 2;
}

bool ClanRobBattle::enterTotem(Player * player)
{
	Clan * playerClan = player->getClan();
	if (playerClan == NULL || playerClan != _clan)
		return false;
	if (!_clan->existClanMember(player))	
		return false;
	if (player->getClanBattle() != NULL)
	{
		ERROR_LOG("Error Enter player has entered totem : [%s][%llu]", player->getName().c_str(), player->getId());
		return false;
	}
	if (player->GetLev() < 40)
	{
		player->sendMsgCode(0, 2219);
		return false;
	}
	if (player->getBuffData(PLAYER_BUFF_REENTERCLAN) != 0)
	{
		player->sendMsgCode(0, 2224);
		return false;
	}
	Mutex::ScopedLock lk(_clan->_mutex);
	ClanMember * clanMember = _clan->getClanMember(player);
	if (clanMember == NULL)
		return false;
	if (TimeUtil::SharpDay(0, clanMember->joinTime) == TimeUtil::SharpDay())
	{
		player->sendMsgCode(0, 2226);
		return false;
	}

	bool firstEnter = false;
	UInt32 now = TimeUtil::Now();
	ClanBattlePlayer * clanBattlePlayerEx = NULL;
	std::map<Player *, ClanBattlePlayer *>::iterator foundcbp = _clanBattlePlayerLocs.find(player);
	if (foundcbp != _clanBattlePlayerLocs.end())
	{
		clanBattlePlayerEx = foundcbp->second;
		if (clanBattlePlayerEx->status != 1)
		{
			UInt32 recoveTime = clanBattlePlayerEx->player->getBuffData(PLAYER_BUFF_CLANRECOVE, now);
			notifyClanBattleRecoveData(clanBattlePlayerEx->player, (clanBattlePlayerEx->reliveNum >> 8) + 1, recoveTime > now ? recoveTime - now : 0);
		}
	}
	else
	{
		clanBattlePlayerEx = new(std::nothrow) ClanBattlePlayer(player, _holds[0].hold, 1, 0);
		if (clanBattlePlayerEx == NULL)
			return false;
		_clanBattlePlayerLocs.insert(std::make_pair(player, clanBattlePlayerEx));
		firstEnter = true;
	}
	clanBattlePlayerEx->hasEnter |= 0x01;
	clanBattlePlayerEx->offTime = 0;
	UInt16 hold = (clanBattlePlayerEx->hold & 0x00FF) - 1;
	if(hold > 3) return false;
	_holds[hold].addBattler(clanBattlePlayerEx);
	{
		GameMsgHdr hdr(0x1F1, player->getThreadId(), player, 0);
		GLOBAL().PushMsg( hdr, NULL );
	}
	player->setClanBattle(this);
	if (firstEnter)
	{
		clanBattlePlayerEx->id = IDGenerator::gClanBatterRecordIDGenerator.ID();
		DB().PushUpdateData("REPLACE INTO `clan_battler`(`id`, `battler`, `battlerLev`, `battleClanId`, `battleClanTime`, `battleHold`, `hasEnter`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u)", clanBattlePlayerEx->id, player->getId(),	\
			player->GetLev(), 0, TimeUtil::SharpDay(0, now), clanBattlePlayerEx->hold, clanBattlePlayerEx->hasEnter);
	}
	notifyBattleScore(player);
	notifySelfBattleScore(player);
	notifyClanBattleEnterInfo(player, 2, 0, _heroClanName);
	notifyClanBattlePlayerMoveInfo(player, 0, clanBattlePlayerEx->hold);
	listClanHoldPlayerInfo(player, clanBattlePlayerEx->hold);
	notifyClanHoldPlayerInfo(player, clanBattlePlayerEx->hold, clanBattlePlayerEx->status, 1);
	if (isInBattling())
	{
		player->setBuffData(PLAYER_BUFF_CLANBATTING, getBattleOverTime(now));
		player->addGlobalFlag(Player::ClanBattleFlag);
		player->addFlag(Player::ClanBattling);
		notifyClanBattleOverTime(player);
		notifyClanBattlePlayerCount();
		notifyClanBattleWinData(player);
		notifyClanHoldEndurance(player);
		DEBUG_LOG("Player [%s] enter clan [%u], side[%u]", player->getName().c_str(), _heroClanId, clanBattlePlayerEx->side);
	}

	return true;
}

bool ClanRobBattle::enterClanCity(Player * player)
{
	Clan * playerClan = player->getClan();
	if (playerClan == NULL)
		return false;
	if (playerClan == _clan)
		return enterTotem(player);
	if (_status == 0xFF)
		return false;
	if (!isInBattling())
	{
		if (_isInbattling == 0)
			player->sendMsgCode(0, 2213);
		else
			player->sendMsgCode(0, 2217);
		return false;
	}
	if (player->getClanBattle() != NULL)
		return false;
	if (player->GetLev() < 40)
	{
		player->sendMsgCode(0, 2219);
		return false;
	}
	Mutex::ScopedLock lk(playerClan->_mutex);
	ClanMember * clanMember = playerClan->getClanMember(player);
	if (clanMember == NULL)
	{
		ERROR_LOG("Error Clan City Player, Cannot find the player : [%s][%llu]", player->getName().c_str(), player->getId());
		return false;
	}
	UInt32 now = TimeUtil::Now();
	UInt32 thisDay = TimeUtil::SharpDay(0, now);
	if (TimeUtil::SharpDay(0, clanMember->joinTime) == thisDay)
	{
		player->sendMsgCode(0, 2226);
		return false;
	}
	if (player->getBuffData(PLAYER_BUFF_REENTERCLAN) != 0)
	{
		player->sendMsgCode(0, 2224);
		return false;
	}
	UInt8 side = (_status == 0 ? 2 : getClanRelation(playerClan));
	bool firstEnter = false;
	ClanBattlePlayer * clanBattlePlayerEx = NULL;
	std::map<Player *, ClanBattlePlayer *>::iterator found = _clanBattlePlayerLocs.find(player);
	if (found != _clanBattlePlayerLocs.end())
	{
		clanBattlePlayerEx = found->second;
		if (clanBattlePlayerEx->status != 1)
		{
			UInt32 recoveTime = clanBattlePlayerEx->player->getBuffData(PLAYER_BUFF_CLANRECOVE, now);
			notifyClanBattleRecoveData(clanBattlePlayerEx->player, (clanBattlePlayerEx->reliveNum >> 8) + 1, recoveTime > now ? recoveTime - now : 0);
		}
	}
	else
	{
		clanBattlePlayerEx = new(std::nothrow) ClanBattlePlayer(player, side == 2 ? _holds[3].hold : _holds[0].hold, 1, side);
		if (clanBattlePlayerEx == NULL)
			return false;
		_clanBattlePlayerLocs.insert(std::make_pair(player, clanBattlePlayerEx));
		if (side == 2)
			_cCbp[playerClan].insert(clanBattlePlayerEx);
		firstEnter = true;
	}
	clanBattlePlayerEx->hasEnter |= 0x01;
	UInt16 hold = (clanBattlePlayerEx->hold & 0x00FF) - 1;
	if(hold > 3)
		return false;
	_holds[hold].addBattler(clanBattlePlayerEx);
	clanBattlePlayerEx->offTime = 0;
	{
		GameMsgHdr hdr(0x1F1, player->getThreadId(), player, 0);
		GLOBAL().PushMsg( hdr, NULL );
	}
	player->setClanBattle(this);
	player->setBuffData(PLAYER_BUFF_CLANBATTING, getBattleOverTime(now));
	player->addFlag(Player::ClanBattling);
	player->addGlobalFlag(Player::ClanBattleFlag);
	if (firstEnter)
	{
		clanBattlePlayerEx->id = IDGenerator::gClanBatterRecordIDGenerator.ID();
		DB().PushUpdateData("REPLACE INTO `clan_battler`(`id`, `battler`, `battlerLev`, `battleClanId`, `battleClanTime`, `battleHold`, `hasEnter`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u)", clanBattlePlayerEx->id, player->getId(),
			player->GetLev(), 0, thisDay, clanBattlePlayerEx->hold, clanBattlePlayerEx->hasEnter);	
	}
	DB().PushUpdateData("UPDATE `clan_player` SET `enterCount` = %u  WHERE `playerId` = %"I64_FMT"u ", clanMember->enterCount, player->getId());
	notifyBattleScore(player);
	notifySelfBattleScore(player);
	notifyClanBattleEnterInfo(player, 2, 0, _heroClanName);
	notifyClanBattlePlayerMoveInfo(player, 0, clanBattlePlayerEx->hold);
	listClanHoldPlayerInfo(player, clanBattlePlayerEx->hold);
	notifyClanBattleOverTime(player);
	notifyClanBattlePlayerCount();
	notifyClanBattleWinData(player);
	notifyClanHoldEndurance(player);
	notifyClanHoldPlayerInfo(player, clanBattlePlayerEx->hold, clanBattlePlayerEx->status, 1);
	DEBUG_LOG("Player [%s] enter clan [%u], side[%u]", player->getName().c_str(), getOwnerClanId(), clanBattlePlayerEx->side);

	return true;
}


bool ClanRobBattle::attackPlayer(Player * atker,  std::string deferName)
{
	if(atker->getBuffData(PLAYER_BUFF_ATTACKING))
	{
		atker->sendMsgCode(0, 2035);
		return false;
	}
	if (atker->getClan() == NULL || atker->getClanBattle() == NULL)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	ClanBattlePlayer * cbAtker = NULL;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		cbAtker = _holds[i].getBattler(atker);
		if (cbAtker != NULL)
			break;
 	}
	if (cbAtker == NULL)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	UInt16 hold = (cbAtker->hold & 0x00FF) - 1;
	if (hold >= 4)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	atker->fixName(deferName);
	ClanBattlePlayer * cbDefer = _holds[hold].getBattler(deferName);
	if (cbDefer == NULL)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (cbDefer->player->getClan() == NULL || cbDefer->player->getClanBattle() != this)
	{
		ERROR_LOG("Cannot find the player battle Clan ! player[%llu][%s]", cbDefer->player->getId(), cbDefer->player->getName().c_str());
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (cbAtker->side != 2 && cbDefer->side != 2)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	UInt32 now = TimeUtil::Now();
	if (cbAtker->status != 1)
	{
		atker->sendMsgCode(0, 2204);
		atker->send(_attackClanBattlerStream);
		return false;
	}
	//可能是复活状态
	if (cbDefer->status != 1)
	{
		//在复活，不能攻击
		atker->sendMsgCode(0, 2204);
		atker->send(_attackClanBattlerStream);
		return false;
	}
	pushBattleCount(static_cast<UInt8>(1));
	if (atker->getClan() != _clan && cbAtker->hasAttack == 0)
	{
		cbAtker->hasAttack = 1;
		DB().PushUpdateData("UPDATE `clan_battler` SET `hasAttack` = 1 WHERE `id` = %u", cbAtker->id);
	}
	if ((cbAtker->side == 2 && cbDefer->side == 2) && (cbAtker->hold == cbDefer->hold))
	{
		if (cbAtker->player->getClan() != cbDefer->player->getClan())
			attackPlayer2(cbAtker, cbDefer);
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (_status == 0)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	UInt32 oldHp = 0, newHp = 0;
	Battle::BattleSimulator bsim(0x7FFF, atker, cbDefer->player);
	atker->PutFighters( bsim, 0 );
	cbDefer->player->PutFighters( bsim, 1 );
	bsim.start();
	oldHp = cbAtker->side == 2 ? bsim.getMaxObjHp(1) : bsim.getMaxObjHp(0);
	newHp = cbAtker->side == 2 ? bsim.getObjHp(1) : bsim.getObjHp(0);
	bool res = bsim.getWinner() == 1;
	UInt32 reptid = bsim.getId();

	{
		Stream st(0x61);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		atker->send(st);
	}
	{
		Stream st(0x61);
		st << static_cast<UInt8>(res ? 0 : 1) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		cbDefer->player->send(st);
	}

	if (oldHp > newHp)
	{
		UInt32 score = (oldHp - newHp) / 100;
		if (cbAtker->side == 2)
		{
			cbAtker->grabAchieve += score;
			calcBattleScore(cbAtker->player, score);
		}
		else
		{
			cbDefer->grabAchieve += score;
			calcBattleScore(cbDefer->player, score);
		}
	}
	if (res)
	{
		//攻方赢
		//将守方送回复活点
		bsim.applyFighterHP(0, atker, false, 30);
		atker->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + bsim.getTurns() * 2);
		cbDefer->player->regenAll();
		cbAtker->wins ++;
		cbAtker->serailWins ++;	
		if (cbAtker->serailWins >= 3)
		{
			UInt8 tipsPos = (cbAtker->serailWins < 10 ? cbAtker->serailWins - 3 : 7);
			static UInt32 Tips[] = { 417, 418, 419, 420, 421, 422, 423, 424 };
			SYSMSGV(tipsContent, Tips[tipsPos], cbAtker->player->getName().c_str(), getOwnerClanName().c_str());
			Stream st(0x79);
			st << static_cast<UInt8>(15) << static_cast<UInt8>(2) << tipsContent << Stream::eos;
			broadcastHold(st);	
		}
		if (cbDefer->serailWins >= 3)
		{
			SYSMSGV(tipsContent, 425, cbAtker->player->getName().c_str(), getOwnerClanName().c_str(), cbDefer->player->getName().c_str());
			Stream st(0x79);
			st << static_cast<UInt8>(15) << static_cast<UInt8>(3) << tipsContent << Stream::eos;
			broadcastHold(st);
		}
		if (cbDefer->maxSerailWins < cbDefer->serailWins)
			cbDefer->maxSerailWins = cbDefer->serailWins;
		if (cbAtker->maxSerailWins < cbAtker->serailWins)
			cbAtker->maxSerailWins = cbAtker->serailWins;
		cbDefer->serailWins = 0;
		UInt16 recover = cbDefer->side == 2 ? _holds[3].hold : _holds[0].hold;
		if (cbDefer->hold != recover)
		{
			//在不同据点
			delClanBattlePlayer(cbDefer->hold, cbDefer->side, cbDefer->status, cbDefer->player);
			Stream st(0x79);
			st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt16>(recover) << Stream::eos;
			cbDefer->player->send(st);
			cbDefer->status = 0;
			cbDefer->hold = recover;
			addClanBattlePlayer(cbDefer);
			listClanHoldPlayerInfo(cbDefer->player, cbDefer->hold);	//自己可以看到
			notifyClanBattlePlayerInfo(cbDefer->player, cbDefer->player, 1);
		}
		else
		{
			//在相同据点
			Stream st(0x79);
			st << static_cast<UInt8>(17) << static_cast<UInt8>(1) << cbDefer->player->getName() << static_cast<UInt8>(0) << Stream::eos;
			cbDefer->status = 0;
			broadcastHold(cbDefer->hold, st);
		}
		DB().PushUpdateData("UPDATE `clan_battler` SET `wins` = %u, `serialWins` = %u, `maxSerialWins` = %u, `grabAchieve` = %u WHERE `id` = %u", cbAtker->wins, cbAtker->serailWins, cbAtker->maxSerailWins, cbAtker->grabAchieve, cbAtker->id);
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleStatus` = 2, `serialWins` = 0, `maxSerialWins` = %u, `battleHold` =%u WHERE `id` = %u", cbDefer->maxSerailWins, recover, cbDefer->id);
		UInt32 recoveTime = static_cast<UInt32>(60 + bsim.getTurns() * 1.8f);
		cbDefer->player->setBuffData(PLAYER_BUFF_CLANRECOVE, now + recoveTime);
		cbDefer->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
		_recoverClanBattlers.insert(cbDefer);
		notifyClanBattleRecoveData(cbDefer->player, (cbDefer->reliveNum >> 8) + 1, recoveTime) ;
		setAwardClanBattleVictor(cbAtker);
	}
	else
	{
		//守方赢
		//将攻方送回复活点
		bsim.applyFighterHP(1, cbDefer->player, false, 30);
		atker->regenAll();
		cbDefer->wins ++;
		cbDefer->serailWins ++;
		if (cbDefer->serailWins >= 3)
		{
			UInt8 tipsPos = (cbDefer->serailWins < 10 ? cbDefer->serailWins - 3 : 7);
			static UInt32 Tips[] = { 417, 418, 419, 420, 421, 422, 423, 424 };
			SYSMSGV(tipsContent, Tips[tipsPos], cbDefer->player->getName().c_str(), getOwnerClanName().c_str());
			Stream st(0x79);
			st << static_cast<UInt8>(15) << static_cast<UInt8>(2) << tipsContent << Stream::eos;
			broadcastHold(st);	
		}
		if (cbAtker->serailWins >= 3)
		{
			SYSMSGV(tipsContent, 425, cbDefer->player->getName().c_str(), getOwnerClanName().c_str(), cbAtker->player->getName().c_str());
			Stream st(0x79);
			st << static_cast<UInt8>(15) << static_cast<UInt8>(3) << tipsContent << Stream::eos;
			broadcastHold(st);
		}
		if (cbDefer->maxSerailWins < cbDefer->serailWins)
			cbDefer->maxSerailWins = cbDefer->serailWins;
		if (cbAtker->maxSerailWins < cbAtker->serailWins)
			cbAtker->maxSerailWins = cbAtker->serailWins;
		cbAtker->serailWins = 0;
		UInt16 recover = cbAtker->side == 2 ? _holds[3].hold : _holds[0].hold;
		if (cbAtker->hold != recover)
		{
			//在不同据点
			delClanBattlePlayer(cbAtker->hold, cbAtker->side, cbAtker->status, atker);
			Stream st(0x79);
			st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt16>(recover) << Stream::eos;
			cbAtker->player->send(st);
			cbAtker->hold = recover;
			cbAtker->status = 0;
			addClanBattlePlayer(cbAtker);
			listClanHoldPlayerInfo(cbAtker->player, cbAtker->hold);	//自己可以看到
			notifyClanBattlePlayerInfo(cbAtker->player, cbAtker->player, 1);
		}
		else
		{
			//在相同据点
			Stream st(0x79);
			st << static_cast<UInt8>(17) << static_cast<UInt8>(1) << cbAtker->player->getName() << static_cast<UInt8>(0) << Stream::eos;
			cbAtker->status = 0;
			broadcastHold(cbAtker->hold, st);
		}
		DB().PushUpdateData("UPDATE `clan_battler` SET `wins` = %u, `serialWins` = %u, `maxSerialWins` = %u WHERE `id` = %u", cbDefer->wins, cbDefer->serailWins, cbDefer->maxSerailWins, cbDefer->id);
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleStatus` = 2, `serialWins` = 0, `maxSerialWins` = %u, `grabAchieve` = %u, `battleHold` =%u WHERE `id` = %u", cbAtker->serailWins, cbAtker->grabAchieve, recover, cbAtker->id);
		UInt32 recoveTime = static_cast<UInt32>((isClanHoldDestroy(_holds[1].hold) || isClanHoldDestroy(_holds[2].hold)) ? 60 + bsim.getTurns() * 1.8f : 75 + bsim.getTurns() * 1.8f);
		cbAtker->player->setBuffData(PLAYER_BUFF_CLANRECOVE, now + recoveTime);
		cbAtker->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
		_recoverClanBattlers.insert(cbAtker);
		notifyClanBattleRecoveData(cbAtker->player, (cbAtker->reliveNum >> 8) + 1, recoveTime);
		setAwardClanBattleVictor(cbDefer);
	}
	notifyClanBattleWinData(cbAtker->player);
	notifyClanBattleWinData(cbDefer->player);
	notifyClanBattlePlayerCount();
	notifyClanBattleReport(atker, cbDefer->player, (res ? 1 : 0), reptid);
	atker->send(_attackClanBattlerStream);

	return true;
}

bool ClanRobBattle::attackPlayer2(ClanBattlePlayer * cbAtker, ClanBattlePlayer * cbDefer)
{
	Battle::BattleSimulator bsim(0x7FFF, cbAtker->player, cbDefer->player);
	cbAtker->player->PutFighters( bsim, 0 );
	cbDefer->player->PutFighters( bsim, 1 );
	bsim.start();
	bool res = bsim.getWinner() == 1;

	ClanBattlePlayer * cbDeath = NULL;
	if (res)
	{
		bsim.applyFighterHP(0, cbAtker->player, false, 30);
		cbAtker->player->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + bsim.getTurns() * 2);
		cbDefer->player->regenAll();
		cbDeath = cbDefer;
	}
	else
	{
		bsim.applyFighterHP(1, cbDefer->player, false, 30);
		cbAtker->player->regenAll();
		cbDeath = cbAtker;
	}

	if (cbDeath->hold != _holds[3].hold)
	{
		Stream st(0x79);
		st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt16>(_holds[3].hold) << Stream::eos;
		cbDeath->player->send(st);
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleHold` = 61444 WHERE `id` = %u", cbDeath->id);
	}
	delClanBattlePlayer(cbDeath->hold, cbDeath->side, cbDeath->status, cbDeath->player);
	cbDeath->hold = _holds[3].hold;
	cbDeath->status = 0;
	addClanBattlePlayer(cbDeath);
	listClanHoldPlayerInfo(cbDeath->player, cbDeath->hold);	//自己可以看到
	notifyClanBattlePlayerInfo(cbDeath->player, cbDeath->player, 1);
	UInt32 recoveTime = static_cast<UInt32>((isClanHoldDestroy(_holds[1].hold) || isClanHoldDestroy(_holds[2].hold)) ? 60 + bsim.getTurns() * 1.8f : 75 + bsim.getTurns() * 1.8f);
	cbDeath->player->setBuffData(PLAYER_BUFF_CLANRECOVE, TimeUtil::Now() + recoveTime);
	_recoverClanBattlers.insert(cbDeath);
	cbDeath->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
	notifyClanBattleRecoveData(cbDeath->player, (cbDeath->reliveNum >> 8) + 1, recoveTime);

	{
		Stream st(0x61);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		cbAtker->player->send(st);
	}
	{
		Stream st(0x61);
		st << static_cast<UInt8>(res ? 0 : 1) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		cbDefer->player->send(st);
	}	

	return true;
}

bool ClanRobBattle::attackNpc(Player * atker, std::string npcName)
{
	if(atker->getBuffData(PLAYER_BUFF_ATTACKING))
	{
		atker->sendMsgCode(0, 2035);
		return false;
	}
	ClanBattlePlayer * cbAtker = NULL;
	std::set<ClanBattlePlayer *>::iterator found;
	for (UInt8 i = 0; i < 4; ++ i)
	{
		cbAtker = _holds[i].getBattler(atker);
		if (cbAtker != NULL)
			break;
	}
	if (cbAtker == NULL)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	UInt16 hold = (cbAtker->hold & 0x00FF) - 1;
	if (hold >= 3)
	{
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (cbAtker->status != 1)
	{
		atker->sendMsgCode(0, 2204);
		atker->send(_attackClanBattlerStream);
		return false;
	}
	if (_holds[hold].aliveDeferSize() > 0)
	{
		atker->send(_attackClanBattlerStream);
		if (hold != 0)
		{
			SYSMSG_SEND(518, atker);
		}
		else
		{
			SYSMSG_SEND(519, atker);
		}
		return false;
	}
	bool r = false;
	UInt32 damge = 0;
	UInt32 turns = 0;
	ClanHoldMonster * cat = NULL;
	if (_holds[hold].guarder != NULL && npcName == _holds[hold].guarder->name)
	{
		if (!_holds[hold].assists.empty())	//存在存活的怪物
		{
			SYSMSG_SEND(518, atker);
			atker->send(_attackClanBattlerStream);
			return false;
		}
		r = attackGuarder(atker, _holds[hold].guarder->name, turns, damge, _holds[hold].guarder);
	}
	else
	{
		//攻击护卫者
		std::map<std::string, ClanHoldMonster *>::iterator found = _holds[hold].assists.find(npcName);
		if (found == _holds[hold].assists.end())
			return false;
		cat = found->second;
		r = attackAssist(atker, cat->name, turns, damge, cat);
	}
	if (damge > 0)
	{
		cbAtker->grabAchieve += (damge / 100);
		calcBattleScore(cbAtker->player, damge / 100);
		DB().PushUpdateData("UPDATE `clan_battler` SET `grabAchieve` = %u WHERE `id` = %u",cbAtker->grabAchieve, cbAtker->id);
	}
	_isInAttacking = true;
	if (r)
	{
		//setAwardClanBattleVictor(cbAtker);
		if (_holds[hold].guarder != NULL && npcName == _holds[hold].guarder->name)
		{
			//打败守卫者
			UInt16 endurance = _holds[hold].endurance <= 1 ? _holds[hold].endurance : 1;
			_holds[hold].endurance -= endurance;
			switch (hold)
			{
			case 0:				
				//cbAtker->grabAchieve ++;	//记录杀死宗族守卫者次数，最后结算
				DB().PushUpdateData("UPDATE `clan` SET `hallEdurance` = %u WHERE `id` = %u", _holds[hold].endurance, getOwnerClanId());
				//DB().PushUpdateData("UPDATE `clan_battler` SET `grabAchieve` = %u WHERE `id` = %u",cbAtker->grabAchieve, cbAtker->id);
				break;
			case 1:
				cbAtker->southEdurance += endurance;
				DB().PushUpdateData("UPDATE `clan` SET `southEdurance` = %u WHERE `id` = %u", _holds[hold].endurance, getOwnerClanId());
				DB().PushUpdateData("UPDATE `clan_battler` SET `southEdurance` = %u WHERE `id` = %u",cbAtker->southEdurance, cbAtker->id);
				break;
			case 2:
				cbAtker->northEdurance += endurance;
				DB().PushUpdateData("UPDATE `clan` SET `northEdurance` = %u WHERE `id` = %u", _holds[hold].endurance, getOwnerClanId());
				DB().PushUpdateData("UPDATE `clan_battler` SET `northEdurance` = %u WHERE `id` = %u",cbAtker->northEdurance, cbAtker->id);
				break;
			}
			notifyClanBattleTotemAttack();
			notifyClanHoldEndurance();
			if (_holds[hold].endurance == 0)
			{
				notifyClanHoldGuarderEnterInfo(cbAtker->hold, 1);
				if (hold == 0)
				{
					overClanBattle(0);
					switchBattleClanOwner();
					clearClanBattle();
				}
				else 
				{
					if (hold == 1)
					{
						if (!_firstAttack[0])
						{
							_firstAttack[0] = true;
							SYSMSG_BROADCASTV(458, getOwnerClanName().c_str());
						}
					}
					else if (hold == 2)
					{
						if (!_firstAttack[1])
						{
							_firstAttack[1] = true;
							SYSMSG_BROADCASTV(458, getOwnerClanName().c_str());
						}
					}
				}
			}
			if (_status == 0 && (hold == 1 || hold == 2))
				displayHiddenMonster(hold);
		}
		else
		{
			notifyClanHoldAssistEnterInfo(npcName, cbAtker->hold, 1);
			cat->buff = TimeUtil::Now();
			_recoverClanHoldAssistant[npcName] = cat;
			_holds[hold].assists.erase(npcName);
		}
	}
	else
	{
		//玩家被打败
		//打输
		delClanBattlePlayer(cbAtker->hold, cbAtker->side, cbAtker->status, atker);
		Stream st(0x79);
		st << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt16>(_holds[3].hold) << Stream::eos;
		cbAtker->player->send(st);
		cbAtker->hold = _holds[3].hold;
		cbAtker->status = 0;
		addClanBattlePlayer(cbAtker);
		listClanHoldPlayerInfo(cbAtker->player, cbAtker->hold);	//自己可以看到
		notifyClanBattlePlayerInfo(cbAtker->player, cbAtker->player, 1);
		UInt32 recoveTime = static_cast<UInt32>((isClanHoldDestroy(_holds[1].hold) || isClanHoldDestroy(_holds[2].hold)) ? 60 + 1.8f * turns : 75 + 1.8f * turns);
		cbAtker->player->setBuffData(PLAYER_BUFF_CLANRECOVE, TimeUtil::Now() + recoveTime);
		cbAtker->player->setBuffData(PLAYER_BUFF_CLANRCENHANCE, 0);
		_recoverClanBattlers.insert(cbAtker);
		notifyClanBattleRecoveData(cbAtker->player, (cbAtker->reliveNum >> 8) + 1, recoveTime);
		notifyClanBattlePlayerCount();
		DB().PushUpdateData("UPDATE `clan_battler` SET `battleStatus` = 2, `battleHold` = %u WHERE `id` = %u", _holds[3].hold, cbAtker->id);
	}
	notifyClanBattleWinData(cbAtker->player);

	atker->send(_attackClanBattlerStream);
	pushBattleCount(2);
	return true;
}

bool ClanRobBattle::attackAssist(Player * atker, std::string& name, UInt32& turns, UInt32& damge, ClanHoldMonster * monster)
{
	if (_status == 1)
		return ClanBattle::attackAssist(atker, name, turns, damge, monster);
	return attackMonster(atker, name, turns, damge, monster, 0);
}

bool ClanRobBattle::attackGuarder(Player * atker, std::string& name, UInt32& turns,  UInt32& damge, ClanHoldMonster * monster)
{
	if (_status == 1)
		return ClanBattle::attackGuarder(atker, name, turns, damge, monster);
	return attackMonster(atker, name, turns, damge, monster, 1);
}

bool ClanRobBattle::attackMonster(Player * atker, std::string& name, UInt32& turns, UInt32& damge, ClanHoldMonster * monster, UInt8 type)
{
	if (monster == NULL || atker->getClan() == NULL || atker->getClanBattle() == NULL)
		return false;
	UInt32 oldHp = 0, newHp = 0;
	ClanRobHoldMonster * chm = static_cast<ClanRobHoldMonster *>(monster);
	Battle::BattleSimulator bsim(0x7FFF, atker, chm->name, chm->level, false);
	atker->PutFighters(bsim, 0);
	std::vector<GData::NpcFData>& nflist = chm->assist->getList();
	size_t size = nflist.size();
	bsim.setFormation(1, chm->assist->getFormation());
	bsim.setPortrait(1, nflist[0].fighter->favor);
	Fighter *fgt_clone[25] = { NULL };
	for(size_t i = 0; i < size; ++ i)
	{
		if (!chm->dirty && chm->hp[i] == 0)
		{
			fgt_clone[i] = NULL;
			continue;
		}
		GData::NpcFData& nfdata = nflist[i];
		fgt_clone[i] = nfdata.fighter->cloneWithEquip(NULL);
		fgt_clone[i]->setLevel(chm->level);
		Battle::BattleFighter *bf = bsim.newFighter(1, nfdata.pos, fgt_clone[i]);
		bf->setHP(chm->hp[i]);
		if (type == 0)
			bf->setAttrExtra(2, bf->getClass(), chm->level);
		else
			bf->setAttrExtra(3, bf->getClass(), chm->level);
		if (!chm->dirty)
			oldHp += chm->hp[i];
	}
	bsim.start();
	if (chm->dirty)
	{
		for (size_t i = 0; i < size; ++ i)
		{
			GData::NpcFData& nfdata = nflist[i];
			Battle::BattleFighter * bf = static_cast<Battle::BattleFighter *>(bsim(1, nfdata.pos));
			if (bf != NULL)
			{
				chm->setMaxHp(i, bf->getMaxHP());
				oldHp += bf->getMaxHP();
			}
		}
		chm->dirty = false;
	}
	newHp = bsim.getObjHp( 1 );
	damge = (oldHp > newHp ? oldHp - newHp : 0);
	Stream& packet = bsim.getPacket();
	if(packet.size() <= 8)
		return false;

	bool r = bsim.getWinner() == 1;
	if (r)
	{
		//玩家赢
		bsim.applyFighterHP(0, atker, false, 30);
		atker->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + bsim.getTurns() * 2);
		if (type == 1)
			chm->resetHp();
	}
	else
	{
		atker->regenAll();
		for (size_t i = 0; i < size; ++ i)
		{
			GData::NpcFData& nfdata = nflist[i];
			Battle::BattleObject * obj = bsim(1, nfdata.pos);
			if (obj != NULL)
			{
				Battle::BattleFighter * bf = static_cast<Battle::BattleFighter *>(obj);
				chm->setHp(i, bf->getHP());
			}
		}
	}

	for(size_t i = 0; i < size; ++ i)
	{
		if (fgt_clone[i] != NULL)
			SAFE_DELETE(fgt_clone[i]);
	}

	{
		Stream st(0x61);
		st << static_cast<UInt16>(r ? 0x101 : 0x100) << static_cast<UInt32>(0) << static_cast<UInt16>(0);
		st.append(&packet[8], packet.size() - 8);
		st << Stream::eos;
		atker->send(st);
	}

	notifyClanBattleReport(atker, name, (r ? 1 : 0), bsim.getId());
	turns = bsim.getTurns();

	return r;
}

UInt8 ClanRobBattle::getBattlerLev()
{
	static const UInt8 Levels[] = {60, 60 , 60, 70, 75, 80};

	if (_heroClanRank < 6)
		return Levels[_heroClanRank];
	return 80 + (_heroClanRank - 5) * 2;
}

void ClanRobBattle::calcBattleScore(Player * player, UInt32 score)
{	
	Clan * clan = player->getClan();
	if (clan == NULL) return;
	_sst[clan] += score;
	bool change = false;
	std::multimap<UInt32, Clan *, std::greater<UInt32> >::iterator it = _rowSst.begin();
	for (; it != _rowSst.end(); ++ it)
	{
		if (it->second == clan)
		{
			_rowSst.erase(it);
			_rowSst.insert(std::make_pair(_sst[clan], clan));	
			change = true;
			break;
		}
	}
	if (!change)
	{
		std::multimap<UInt32, Clan *, std::greater<UInt32> >::reverse_iterator rIt = _rowSst.rbegin();
		if (_rowSst.size() < 3 || rIt->first < _sst[clan])
		{
			_rowSst.insert(std::make_pair(_sst[clan], clan));
			if (_rowSst.size() > 3)
			{
				it = _rowSst.begin();
				std::advance(it, 3);
				_rowSst.erase(it, _rowSst.end());
			}
			change = true;
		}
	}
	if (change)
		notifyBattleScore();	
	notifySelfBattleScore(clan, _sst[clan]);
}

void ClanRobBattle::resumeBattleScore()
{
	_sst.clear();
	_rowSst.clear();
	_cCbp.clear();
	Clan * clan = NULL;
	std::map<Player *, ClanBattlePlayer *>::iterator offset = _clanBattlePlayerLocs.begin();
	for (; offset != _clanBattlePlayerLocs.end(); ++ offset)
	{
		clan = offset->first->getClan();
		if (clan != NULL && offset->second->side == 2)
		{
			_sst[clan] += offset->second->grabAchieve;
			_cCbp[clan].insert(offset->second);
		}
	}
	std::map<Clan *, UInt32>::iterator it = _sst.begin();
	for (; it != _sst.end(); ++ it)
	{
		if (it->second != 0)
			_rowSst.insert(std::make_pair(it->second, it->first));
	}
	if (_rowSst.size() > 3)
	{
		std::multimap<UInt32, Clan *, std::greater<UInt32> >::iterator it2 = _rowSst.begin();
		std::advance(it2, 3);
		_rowSst.erase(it2, _rowSst.end());
	}
}

void ClanRobBattle::notifyBattleScore(Player * player)
{
	if (_rowSst.empty())
		return;
	Stream st(0x79);
	st << static_cast<UInt8>(19) << static_cast<UInt8>(_rowSst.size());
	std::multimap<UInt32, Clan *, std::greater<UInt32> >::iterator offset = _rowSst.begin();
	for (; offset != _rowSst.end(); ++ offset)
	{
		st << offset->second->getName() << offset->first;
	}	
	st << Stream::eos;
	player == NULL ? broadcastHold(st) : player->send(st);
}

void ClanRobBattle::notifySelfBattleScore(Player * player, UInt32 score, bool know)
{
	Clan * clan = player->getClan();
	if (clan == NULL)
		return;
	if (!know)
	{
		score = 0;
		std::map<Clan *, UInt32>::iterator found = _sst.find(clan);
		if (found != _sst.end())
			score = found->second;
	}
	Stream st(0x79);
	st << static_cast<UInt8>(21) << score << Stream::eos;
	player->send(st);
}

void ClanRobBattle::notifySelfBattleScore(Clan * clan, UInt32 score)
{
	std::map<Clan *, std::set<ClanBattlePlayer *> >::iterator found  = _cCbp.find(clan);
	if (found == _cCbp.end())
		return;
	Stream st(0x79);
	st << static_cast<UInt8>(21) << score << Stream::eos;
	ClanBattlePlayer * cbp = NULL;
	std::set<ClanBattlePlayer *>::iterator offset = found->second.begin();
	for (; offset != found->second.end(); ++ offset)
	{
		cbp = *offset;
		if ((cbp->hasEnter & 0x0F) != 0)
		{
			cbp->player->send(st);
		}
	}
}

void ClanRobBattle::initDisplayMonsterStatus()
{
	for (UInt8 i = 0; i < 2; ++ i)
		for (UInt8 j = 0; j < 2; ++ j)
			_display[i][j] = false;
}

static bool find_assist(GData::NpcGroup * npc, UInt32 id)
{
	return npc->getId() == id;
}

void ClanRobBattle::displayHiddenMonster(UInt16 hold, bool notify)
{
	//hold == 1: 南门 hold == 2 ： 北门
	//klass == 1: 诸葛亮 klass == 2: 于谦	
	static const UInt32 assistId[2] = { 4930, 4931 };
	static const float bound[2] = { 50.0f, 30.0f };
	using namespace std::placeholders;
	GData::NpcGroup * assist = NULL;
	std::vector<GData::NpcGroup *>::iterator found;
	for (UInt8 klass = 1; klass <= 2; ++ klass)
	{
		if (!_display[hold-1][klass-1] && getEdurance(hold) < static_cast<UInt8>(getTotalEdurance(hold) * bound[klass-1] / 100.0f))
		{
			std::vector<GData::NpcGroup *>& assists = (hold == 1 ? GData::clanRobMonster._clanSouthBoss : GData::clanRobMonster._clanNorthBoss);
			found = std::find_if(assists.begin(), assists.end(), std::bind(find_assist, _1, assistId[klass-1]));
			if (found != assists.end())
			{
				assist = *found;
				ClanHoldMonster * monster = new(std::nothrow) ClanRobHoldMonster(assist->getName(), getBattlerLev(), assist->getClass(), _holds[hold].hold, 0, assist);
				_holds[hold].assists.insert(std::make_pair(assist->getName(), monster));
			}
			_display[hold-1][klass-1] = true;
			if (notify && assist != NULL)
				notifyClanHoldAssistEnterInfo(assist->getName(), _holds[hold].hold, 0);
		}
	}
}

struct AddDynamicMsg1Param
{
	std::vector<Clan *>& clans;
	std::map<Clan *, UInt32>& clanRows;

	AddDynamicMsg1Param(std::vector<Clan *>& cs, std::map<Clan *, UInt32>& cr) : clans(cs), clanRows(cr) {}
};

struct AddDynamicMsg3Param
{
	std::vector<Clan *>& clans;
	std::string name;

	AddDynamicMsg3Param(std::vector<Clan *>& cs, const std::string& n) : clans(cs), name(n) {}
};

static bool add_dynamic_msg1(Clan * c, AddDynamicMsg1Param& admp)
{
	UInt32 thisDay = TimeUtil::SharpDay();
	ClanDynamicMsg * cdm = c->getClanDynamicMsg();
	std::map<Clan *, UInt32>::iterator found = admp.clanRows.find(c);
	UInt32 row = (found != admp.clanRows.end() ? found->second : admp.clanRows.size());
	switch (admp.clans.size())
	{
	case 1:
		cdm->addCDMsg(16, admp.clans[0]->getName(), row, thisDay);
		break;
	case 2:
		cdm->addCDMsg(15, admp.clans[0]->getName(), admp.clans[1]->getName(), row, thisDay);
		break;
	case 3:
		cdm->addCDMsg(14, admp.clans[0]->getName(), admp.clans[1]->getName(), admp.clans[2]->getName(), row, thisDay);
		break;
	default:
		break;
	}
	return true;
}

static bool add_dynamic_msg2(Clan * c, void *)
{
	c->getClanDynamicMsg()->addCDMsg(17, TimeUtil::SharpDay());
	return true;
}

static bool add_dynamic_msg3(Clan * c, AddDynamicMsg3Param& admp)
{
	UInt32 thisDay = TimeUtil::SharpDay();
	ClanDynamicMsg * cdm = c->getClanDynamicMsg();
	switch (admp.clans.size())
	{
	case 1:
		cdm->addCDMsg(21, admp.name, admp.clans[0]->getName(), thisDay);
		break;
	case 2:
		cdm->addCDMsg(20, admp.name, admp.clans[0]->getName(), admp.clans[1]->getName(), thisDay);
		break;
	case 3:
		cdm->addCDMsg(19, admp.name, admp.clans[0]->getName(), admp.clans[1]->getName(), admp.clans[2]->getName(), thisDay);
		break;
	default:
		break;
	}
	return true;
}

static bool add_dynamic_msg4(Clan * c, const std::string& name)
{
	c->getClanDynamicMsg()->addCDMsg(18, name, TimeUtil::SharpDay());
	return true;
}

void ClanRobBattle::closingBattlerAward(UInt8 succ)
{
	size_t i = 0, j = 1;
	std::vector<Clan *> clans;
	std::map<Clan *, UInt32> clanRows;
	std::multimap<UInt32, Clan *, std::greater<UInt32> >::iterator sstItr = _rowSst.begin();
	for (i = 0; sstItr != _rowSst.end(); ++ sstItr, ++ j)
	{
		if (sstItr->second != NULL && i < 3)
		{
			clans.push_back(sstItr->second);
			++ i;
		}
		clanRows[sstItr->second] = j;
	}
	size_t sz = clans.size();
	if (succ == 0)
	{
		for (i = 0; i < sz; ++ i)
			clans[i]->addRepo(9041, 3-i, i+1);
		if (_status == 0)
		{
			if (sz >= 3)
			{
				SYSMSG_BROADCASTV(510, clans[0]->getName().c_str(), clans[1]->getName().c_str(), clans[2]->getName().c_str());
			}
			else if (sz >= 2)
			{
				SYSMSG_BROADCASTV(511, clans[0]->getName().c_str(), clans[1]->getName().c_str());
			}
			else if (sz >= 1)
			{
				SYSMSG_BROADCASTV(512, clans[0]->getName().c_str());
			}
			if (sz >= 1)
				globalClans.enumerate(add_dynamic_msg1, AddDynamicMsg1Param(clans, clanRows));
		}
		else
		{
			if (sz >= 3)
			{
				SYSMSG_BROADCASTV(515, getOwnerClanName().c_str(), clans[0]->getName().c_str(), clans[1]->getName().c_str(), clans[2]->getName().c_str());
			}
			else if (sz >= 2)
			{
				SYSMSG_BROADCASTV(516, getOwnerClanName().c_str(), clans[0]->getName().c_str(), clans[1]->getName().c_str());
			}
			else if (sz >= 1)
			{
				SYSMSG_BROADCASTV(517, getOwnerClanName().c_str(), clans[0]->getName().c_str());
			}
			if (sz >= 1)
				globalClans.enumerate(add_dynamic_msg3, AddDynamicMsg3Param(clans, getOwnerClanName()));
		}
	}
	else
	{
		if (_status == 1)
		{
			if (_clan != NULL)
			{
				_clan->addRepo(9041, 5, 1);
				SYSMSG_BROADCASTV(514, _clan->getName().c_str());
				globalClans.enumerate(add_dynamic_msg4, _clan->getName());
			}
		}
		else
		{
			SYSMSG_BROADCAST(513);
			globalClans.enumerate(add_dynamic_msg2, (void *)NULL);
		}
	}
	if (succ == 0)
		calcBattleExp();
}

void ClanRobBattle::calcBattleExp()
{
	Player * player = NULL;
	ClanBattlePlayer * cbp = NULL;
	std::map<Player *, ClanBattlePlayer *>::iterator offset = _clanBattlePlayerLocs.begin();
	for (; offset != _clanBattlePlayerLocs.end(); ++ offset)
	{
		cbp = offset->second;
		player = cbp->player;
		if (cbp->side != 2)
			continue;
		if (cbp->player->getClan() == NULL)
			continue;
		if (cbp->grabAchieve != 0)
		{
			UInt64 exp = static_cast<UInt64>(cbp->grabAchieve * player->GetLev() / 100 * 5);
			GameMsgHdr hdr(0x203, player->getThreadId(), player, sizeof(UInt64));
			GLOBAL().PushMsg(hdr, &exp);
			SYSMSG_SENDV(446, player, exp);
			SYSMSG(title, 440);
			SYSMSGV(content, 530, cbp->grabAchieve, exp);
			player->GetMailBox()->newMail(NULL, 0x01, title, content);
		}
	}
}

void ClanRobBattle::notifyBattleOverMailNotify(UInt8 succ)
{
	size_t sz = _rowSst.size();
	Clan * clan = NULL;
	Clan * clans[3] = {NULL};
	char msg1[1024], msg2[1024];
	UInt8 day = World::getWeekDay();
	std::multimap<UInt32, Clan *, std::greater<UInt32> >::iterator offset;
	std::map<Player *, ClanBattlePlayer *>::iterator cbpIter;
	offset = _rowSst.begin();
	if (sz >= 3)
	{
		clans[0] = (offset ++)->second;
		clans[1] = (offset ++)->second;
		clans[2] = (offset ++)->second;
	}
	else if (sz >= 2)
	{
		clans[0] = (offset ++)->second;
		clans[1] = (offset ++)->second;
	}
	else if (sz >= 1)
	{
		clans[0] = (offset ++)->second;
	}
	switch (_status)
	{
	case 0:
		{
			//夺城战
			if (succ == 0)
			{
				SYSMSG(title, 520);
				SYSMSG(title1, 521);
				if (sz >= 3)
				{
					SYSMSGV2(msg1, 487, clans[1]->getName().c_str(), clans[2]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 490);
					}
					SYSMSGV(content1, 479, msg1, msg2);
					SYSMSGV2(msg1, 482, clans[0]->getName().c_str(), clans[1]->getName().c_str(), clans[2]->getName().c_str());
					if (day == 6)
					{
						SYSMSGV2(msg2, 486);
					}
					else
					{
						SYSMSGV2(msg2, 485, clans[0]->getName().c_str(), clans[0]->getName().c_str());
					}
					SYSMSGV(content2, 479, msg1, msg2);
					std::for_each(clans[0]->_members.begin(), clans[0]->_members.end(), BattleOverMailSender(title1, content1));
					for (cbpIter = _clanBattlePlayerLocs.begin(); cbpIter != _clanBattlePlayerLocs.end(); ++ cbpIter)
					{
						clan = cbpIter->first->getClan();
						if (clan != clans[0])
							cbpIter->first->GetMailBox()->newMail(NULL, 0x01, title, content2);
					}
				}
				else if (sz >= 2)
				{
					SYSMSGV2(msg1, 488, clans[1]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 490);
					}
					SYSMSGV(content1, 479, msg1, msg2);
					SYSMSGV2(msg1, 483, clans[0]->getName().c_str(), clans[1]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSGV2(msg2, 485, clans[0]->getName().c_str(), clans[0]->getName().c_str());
					}
					SYSMSGV(content2, 479, msg1, msg2);
					std::for_each(clans[0]->_members.begin(), clans[0]->_members.end(), BattleOverMailSender(title1, content1));
					for (cbpIter = _clanBattlePlayerLocs.begin(); cbpIter != _clanBattlePlayerLocs.end(); ++ cbpIter)
					{
						clan = cbpIter->first->getClan();
						if (clan != clans[0])
							cbpIter->first->GetMailBox()->newMail(NULL, 0x01, title, content2);
					}
				}
				else if (sz >= 1)
				{
					SYSMSG(msg1, 489);
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 490);
					}
					SYSMSGV(content1, 479, msg1, msg2);
					std::for_each(clans[0]->_members.begin(), clans[0]->_members.end(), BattleOverMailSender(title1, content1));
				}
			}
			else
			{
				SYSMSG(title, 525);
				SYSMSG(msg1, 506);
				SYSMSG(msg2, 486);
				SYSMSGV(content, 479, msg1, msg2);
				for (cbpIter = _clanBattlePlayerLocs.begin(); cbpIter != _clanBattlePlayerLocs.end(); ++ cbpIter)
				{
					clan = cbpIter->first->getClan();
					if (clan != NULL)
						cbpIter->first->GetMailBox()->newMail(NULL, 0x01, title, content);
				}
			}
		}
		break;
	case 1:
		{
			SYSMSG(title, 481);
			//守城战
			if (succ == 0)
			{
				//被攻破
				SYSMSG(title1, 526);
				SYSMSG(title2, 523);
				SYSMSG(title3, 524);
				if (sz >= 3)
				{
					SYSMSGV(msg1, 493, clans[0]->getName().c_str(), clans[0]->getName().c_str(), clans[1]->getName().c_str(), clans[2]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 496);
					}
					SYSMSGV(content1, 479, msg1, msg2);
					SYSMSGV2(msg1, 497, clans[0]->getName().c_str(), clans[1]->getName().c_str(), clans[2]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 500);
					}
					SYSMSGV(content2, 479, msg1, msg2);
					SYSMSGV2(msg1, 501, clans[0]->getName().c_str(), clans[1]->getName().c_str(), clans[2]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 491);
					}
					SYSMSGV(content3, 479, msg1, msg2);
					if (_clan != NULL)
						std::for_each(_clan->_members.begin(), _clan->_members.end(), BattleOverMailSender(title1, content1));
					std::for_each(clans[0]->_members.begin(), clans[0]->_members.end(), BattleOverMailSender(title2, content2));
					for (cbpIter = _clanBattlePlayerLocs.begin(); cbpIter != _clanBattlePlayerLocs.end(); ++ cbpIter)
					{
						clan = cbpIter->first->getClan();
						if (clan != _clan && clan != clans[0])
							cbpIter->first->GetMailBox()->newMail(NULL, 0x01, title3, content3);
					}

				}
				else if (sz >= 2)
				{
					SYSMSGV(msg1, 494, clans[0]->getName().c_str(), clans[0]->getName().c_str(), clans[1]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 496);
					}
					SYSMSGV(content1, 479, msg1, msg2);
					SYSMSGV2(msg1, 498, clans[0]->getName().c_str(), clans[1]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 500);
					}
					SYSMSGV(content2, 479, msg1, msg2);
					SYSMSGV2(msg1, 502, clans[0]->getName().c_str(), clans[1]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 491);
					}
					SYSMSGV(content3, 479, msg1, msg2);
					if (_clan != NULL)
						std::for_each(_clan->_members.begin(), _clan->_members.end(), BattleOverMailSender(title1, content1));
					std::for_each(clans[0]->_members.begin(), clans[0]->_members.end(), BattleOverMailSender(title2, content2));
					for (cbpIter = _clanBattlePlayerLocs.begin(); cbpIter != _clanBattlePlayerLocs.end(); ++ cbpIter)
					{
						clan = cbpIter->first->getClan();
						if (clan != _clan && clan != clans[0])
							cbpIter->first->GetMailBox()->newMail(NULL, 0x01, title3, content3);
					}
				}
				else if (sz >= 1)
				{
					SYSMSGV(msg1, 495, clans[0]->getName().c_str(), clans[0]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 496);
					}
					SYSMSGV(content1, 479, msg1, msg2);
					SYSMSGV2(msg1, 499, clans[0]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 500);
					}
					SYSMSGV(content2, 479, msg1, msg2);
					SYSMSGV2(msg1, 503, clans[0]->getName().c_str());
					if (day == 6)
					{
						SYSMSG2(msg2, 486);
					}
					else
					{
						SYSMSG2(msg2, 491);
					}
					SYSMSGV(content3, 479, msg1, msg2);
					if (_clan != NULL)
						std::for_each(_clan->_members.begin(), _clan->_members.end(), BattleOverMailSender(title1, content1));
					std::for_each(clans[0]->_members.begin(), clans[0]->_members.end(), BattleOverMailSender(title2, content2));
					for (cbpIter = _clanBattlePlayerLocs.begin(); cbpIter != _clanBattlePlayerLocs.end(); ++ cbpIter)
					{
						clan = cbpIter->first->getClan();
						if (clan != _clan && clan != clans[0])
								cbpIter->first->GetMailBox()->newMail(NULL, 0x01, title3, content3);
					}
				}
			}
			else
			{
				//未被攻破
				SYSMSG(title, 522);
				SYSMSG(title1, 524);
				SYSMSG(msg1, 492);
				if (day == 6)
				{
					SYSMSG2(msg2, 486);
				}
				else
				{
					SYSMSG2(msg2, 491);
				}
				SYSMSGV(content1, 479, msg1, msg2);
				if (day == 6)
				{
					SYSMSGV2(msg1, 505, (_clan == NULL ? "" : _clan->getName().c_str()));
				}
				else
				{
					SYSMSGV2(msg1, 505, (_clan == NULL ? "" : _clan->getName().c_str()));
				}
				if (_clan != NULL)
					std::for_each(_clan->_members.begin(), _clan->_members.end(), BattleOverMailSender(title, content1));
				for (cbpIter = _clanBattlePlayerLocs.begin(); cbpIter != _clanBattlePlayerLocs.end(); ++ cbpIter)
				{
					clan = cbpIter->first->getClan();
					if (clan != _clan)
						cbpIter->first->GetMailBox()->newMail(NULL, 0x01, title1, msg1);
				}
			}
		}
		break;
	default:
		return;
	}
}


UInt8 ClanRobBattle::getClanBattleStatus(UInt8 cbt, UInt32 now)
{
	UInt32 thisDay = TimeUtil::SharpDay(0, now);
	if (now < thisDay + cbt * 30 * 60)
		return 0;
	else if (now < thisDay + cbt * 30 * 60 + (cfg.GMCheck ? (30 * 60) : (7 * 60 * 60)))
		return 1;
	else
		return 2;
}

void ClanRobBattle::switchBattleClanOwner()
{
	if (_sst.empty())
		return;
	Clan * clan = (_rowSst.begin())->second;
	if (clan != NULL)
	{
		_heroClanLev = clan->getLev();
		_heroClanId = clan->getId();
		_clan = clan;
		if (_status == 0)
		{
			++ _heroClanAtkCount;
			++ _heroClanRank;
			DB().PushUpdateData("UPDATE `clan` SET `foundTime` = %u, `rank` = %u, `allyClan` = %u WHERE `id` = 0", _heroClanAtkCount, _heroClanRank, clan->getId());
		}
		else
		{
			DB().PushUpdateData("UPDATE `clan` SET `allyClan` = %u WHERE `id` = 0", clan->getId());
		}
		Stream st(0x79);
		st << static_cast<UInt8>(20) << clan->getId() << clan->getName() << Stream::eos;
		broadcastHold(st);
	}
	_status = 0xFF;	//关闭城池	
}

void ClanRobBattle::update(UInt32 now)
{
	//check = false;
	ClanBattlePlayer * cbPlayer;
	std::set<ClanBattlePlayer *>::iterator rcIter = _recoverClanBattlers.begin();
	for (; rcIter != _recoverClanBattlers.end();)
	{
		cbPlayer = *rcIter;
		if (cbPlayer->status == 0 && cbPlayer->player->getBuffData(PLAYER_BUFF_CLANRECOVE, now) == 0)
		{
			cbPlayer->status = 2;
			_recoverClanBattlers.erase(rcIter++);
		}
		else
			++ rcIter;
	}
	//城门怪物复活检查
	std::map<std::string, ClanHoldMonster *>::iterator rcIter2 = _recoverClanHoldAssistant.begin();
	for (; rcIter2 != _recoverClanHoldAssistant.end();)
	{
		ClanHoldMonster * cat = rcIter2->second;
		if (now > cat->buff && now - cat->buff > 45)
		{
			UInt16 hold = (cat->hold & 0x00FF) - 1;
			if(hold > 3)
				continue;
			cat->buff = 0;
			cat->resetHp();
			_holds[hold].assists[rcIter2->first] = cat;
			notifyClanHoldAssistEnterInfo(cat->name, cat->hold, 0);
			_recoverClanHoldAssistant.erase(rcIter2++);
		}
		else
			++ rcIter2;
	}

	//下线待踢者
	std::set<ClanBattlePlayer *>::iterator rcIter3 = _offClanBattlers.begin();
	for (; rcIter3 != _offClanBattlers.end();)
	{
		cbPlayer = *rcIter3;
		if (cbPlayer->offTime == 0)
			_offClanBattlers.erase(rcIter3++);
		else if (now > cbPlayer->offTime && now - cbPlayer->offTime > 2 * 60)
		{
			++ rcIter3;
			kickClanBattler(cbPlayer->player);
			cbPlayer->offTime = 0;
		}
		else
			++ rcIter3;
	}

	//耐久度恢复检查
	for (UInt8 i = 1; i <= 2; ++ i)
	{
		if (_holds[i].atkerSize() > 0 || _holds[i].deferSize() <= 0 || _holds[i].endurance >= _holds[i].totalendurance)
		{
			if (_holds[i].buff != 0)
			{
				_holds[i].buff = 0;
				notifyClanHoldEndurance(_holds[i]);
			}
		}
		else if (_holds[i].buff == 0)
		{
			_holds[i].buff = now;
			notifyClanHoldEndurance(_holds[i]);
		}
		else if (now > _holds[i].buff && now - _holds[i].buff >= 25)
		{
			_holds[i].endurance ++;
			_holds[i].buff = now;
			notifyClanHoldEndurance(_holds[i]);
		}
	}
}

void ClanRobBattle::clearClanBattle()
{
	ClanBattle::clearClanBattle();
	//_clan = NULL;
	_sst.clear();
	_rowSst.clear();
	_cCbp.clear();
}

}

#include "Config.h"
#include "Clan.h"
#include "GObject/Country.h"
#include "GObject/TaskMgr.h"
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "MapCollection.h"
#include "ClanBattle.h"
#include "ClanManager.h"
#include "GData/ClanTechTable.h"
#include "GData/ClanLvlTable.h"
#include "GData/ClanSkillTable.h"
#include "Player.h"
#include "Package.h"
#include "Mail.h"
#include "ClanTech.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"
#include "GObject/PracticePlace.h"
#include "MsgID.h"

#include <mysql.h>

namespace GObject
{

GlobalClans globalClans, globalClansByCountry[COUNTRY_MAX];
GlobalNamedClans globalGlobalNamedClans, globalNamedClans[COUNTRY_MAX];
GlobalNamedClans globalOwnedClans[COUNTRY_MAX];

UInt8 ClanAuthority[5][7] = 
{
	{ 0, 1, 0, 0, 0, 0, 0 },
	{ 0, 1, 0, 0, 0, 0, 0 },
	{ 1, 1, 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 1, 1, 1 }
};


// 帮派秘术
#define CLAN_SKILL_HP       1
#define CLAN_SKILL_ATTACK   2
#define CLAN_SKILL_DEFEND   3
#define CLAN_SKILL_MAGATK   4
#define CLAN_SKILL_MAGDEF   5

static bool find_pending_member(ClanPendingMember * member, Player * p)
{
	return member->player == p;
}

static bool find_pending_member_id(ClanPendingMember * member, UInt64 id)
{
	return member->player->getId() == id;
}

Clan::Clan( UInt32 id, const std::string& name, UInt32 ft, UInt8 lvl ) :
	GObjectBaseT<Clan>(id), _name(name), _rank(0), _level(lvl), _foundTime(ft == 0 ? TimeUtil::Now() : ft),
    _founder(0), _leader(0), _construction(0), _nextPurgeTime(0), _proffer(0),
    _flushFavorTime(0), _allyClan(NULL), _allyClanId(0), _deleted(false), _funds(0), _watchman(0)
{
	_techs = new ClanTech(this);
    _maxMemberCount = BASE_MEMBER_COUNT + _techs->getMemberCount();
	memset(_favorId, 0, sizeof(_favorId));
	_clanDynamicMsg = new ClanDynamicMsg();
	_clanBattle = new ClanCityBattle(this);
}

Clan::~Clan()
{
	delete _techs;
	delete _clanDynamicMsg;
	delete _clanBattle;
}

bool Clan::accept(Player * player, UInt64 pid )
{
	if (_clanBattle->isInBattling())
	{
		player->sendMsgCode(2, 1317);
		return false;
	}
	if (!hasClanAuthority(player, 0))
	{
		player->sendMsgCode(2, 1313);
		return false;
	}
	using namespace std::placeholders;
	std::vector<ClanPendingMember *>::iterator it = std::find_if(_pending.begin(), _pending.end(), std::bind(find_pending_member_id, _1, pid));
	if(it == _pending.end() || (*it)->cls != 16)
		return false;
	Player * accepter = (*it)->player;
	if (accepter->getClan() != NULL)
	{
		player->sendMsgCode(0, 1312);
		return true;
	}
	if (!isFull() && join(accepter))
	{
		SYSMSG(title, 225);
		SYSMSGV(content, 226, _name.c_str());
		accepter->GetMailBox()->newMail(NULL, 1, title, content);
		if(accepter->isOnline())
		{
			SYSMSG_SENDV(121, accepter, _name.c_str());
			SYSMSG_SENDV(1021, accepter, _name.c_str());
		}
	}
	delete *it;
	_pending.erase(it);
	DB().PushUpdateData("DELETE FROM `clan_pending_player` WHERE `id` = %u AND `playerId` = %"I64_FMT"u", _id, accepter->getId());
	return true;
}

bool Clan::decline( Player* player, UInt64 pid )
{
    if (!hasClanAuthority(player, 0))
	{
		player->sendMsgCode(2, 1313);
		return false;
	}

	using namespace std::placeholders;
	std::vector<ClanPendingMember *>::iterator it = std::find_if(_pending.begin(), _pending.end(), std::bind(find_pending_member_id, _1, pid));
	if(it == _pending.end())
		return false;

	SYSMSG(title, 227);
	SYSMSGV(content, 228, _name.c_str());
	(*it)->player->GetMailBox()->newMail(NULL, 1, title, content);

	delete *it;
	_pending.erase(it);
	{
		Stream st(REP::CLAN_INFO_UPDATE);
		st << static_cast<UInt8>(2) << pid;
		st << Stream::eos;
		broadcast(st);
	}
	DB().PushUpdateData("DELETE FROM `clan_pending_player` WHERE `id` = %u AND `playerId` = %"I64_FMT"u AND `class` = 16", _id, pid);

	return true;
}

bool Clan::join( Player * player, UInt8 jt, UInt16 si, UInt32 ptype, UInt32 p, UInt32 tp, UInt32 ptime )
{
	if(player->getClan() != NULL)
		return false;
	Mutex::ScopedLock lk(_mutex);
	if (existClanMember(player))
		return false;
	UInt32 joinTime = TimeUtil::Now();
	//进行时间自调整
	std::set<UInt32>::iterator found = _membersJoinTime.find(joinTime);
	while (found != _membersJoinTime.end())
		found = _membersJoinTime.find(++joinTime);
	ClanMember * cmem = new(std::nothrow) ClanMember(player, 0, joinTime);
	if(jt == 4)
	{
		cmem->cls = 4;
		cmem->proffer = 2000;
	}
	if (cmem == NULL) return false;
    buildTechSkill(cmem);
	_membersJoinTime.insert(joinTime);
	broadcastMemberInfo(*cmem, 0);
	std::string oldLeaderName = (_members.empty() ? "" : (*_members.begin())->player->getName());
	_members.insert(cmem);
	// updateRank(NULL, oldLeaderName);
	player->setClan(this);
	player->notifyFriendAct(5, _name.c_str());
	DB().PushUpdateData("INSERT INTO `clan_player` (`id`, `playerId`, `joinTime`, `cls`, `proffer`) VALUES (%u, %"I64_FMT"u, %u, %u, %u)", _id, player->getId(), cmem->joinTime, cmem->cls, cmem->proffer);
	if(player->isOnline())
	{
		sendInfo(player);
		listMembers(player);
	}
	SysMsgItem * sysMsgItem = globalSysMsg[460];
	if (sysMsgItem != NULL)
	{
		Stream st;
		sysMsgItem->getvap(&st, player->getName().c_str());
		broadcast(st);
	}
	{
		Stream st;
		_clanDynamicMsg->addCDMsg(0, player->getName(), &st);
		broadcast(st);
	}

    int type = 0;
	GameMsgHdr hdr(0x310, player->getThreadId(), player, sizeof(type));
	GLOBAL().PushMsg(hdr, &type);

    if(CLAN_TASK_MAXCOUNT > PLAYER_DATA(player, ctFinishCount))
        player->buildClanTask();
	return true;
}


bool Clan::join(ClanMember * cm)
{
	Player * player = cm->player;
	if (player->getClan() != NULL)
		return false;
	Mutex::ScopedLock lk(_mutex);
	if (existClanMember(player))
		return false;
	player->setClan(this);
	//std::string oldLeaderName = (_members.empty() ? "" : (*_members.begin())->player->getName());
	std::set<UInt32>::iterator found = _membersJoinTime.find(cm->joinTime);
	while (found != _membersJoinTime.end())
		found = _membersJoinTime.find(++cm->joinTime);
    buildTechSkill(cm);
	_members.insert(cm);
	//updateRank(oldLeaderName);
	_membersJoinTime.insert(cm->joinTime);
    if(CLAN_TASK_MAXCOUNT > PLAYER_DATA(player, ctFinishCount))
        player->buildClanTask();
	return true;
}

bool Clan::kick(Player * player, UInt64 pid)
{
	if (_clanBattle->isInBattling())
	{
		player->sendMsgCode(0, 1317);
		return false;
	}
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(pid);
	if (found == _members.end())
		return false;
	Player * kicker = (*found)->player;
	//if (pid == getFounder())
	//{
	//	player->sendMsgCode(0, 2223);
	//	return false;
	//}
	if (!hasClanAuthority(player, 3))
		return false;
	if (player == kicker)
		return false;
	ClanMember * member = *found;
	getAllocBack(*member);

	{
		Stream st(REP::CLAN_INFO_UPDATE);
		st << static_cast<UInt8>(2) << pid;
		st << Stream::eos;
		broadcast(st);
	}

	{
		Stream st(REP::CLAN_MEMBER_OPERATE);
		st << static_cast<UInt8>(1) << static_cast<UInt8>(1) << pid << Stream::eos;
		player->send(st);
	}
	SysMsgItem * sysMsgItem = globalSysMsg[461];
	if (sysMsgItem != NULL)
	{
		Stream st;
		sysMsgItem->getvap(&st, kicker->getName().c_str());
		broadcast(st);
	}
	{
		Stream st;
		_clanDynamicMsg->addCDMsg(1, kicker->getName(), &st);
		broadcast(st);
	}

	_members.erase(found);
	delete member;
	kicker->setClan(NULL);
	ClanBattle * battleClan = kicker->getClanBattle();
	if (battleClan != NULL)
		battleClan->kickClanBattler(kicker);

	// updateRank();
	DB().PushUpdateData("DELETE FROM `clan_player` WHERE `playerId` = %"I64_FMT"u", pid);

	SYSMSGV(title, 229, _name.c_str());
	SYSMSGV(content, 230, _name.c_str());
	kicker->GetMailBox()->newMail(NULL, 1, title, content);
	if(kicker->isOnline())
	{
		SYSMSG_SEND(125, kicker);
		SYSMSG_SENDV(1025, kicker, player->getCountry(), player->getName().c_str(), _name.c_str());
	}
	else
	{
		SYSMSG_SEND(124, kicker);
		SYSMSG_SENDV(1024, kicker, _name.c_str());
	}
	return true;
}

bool Clan::leave(Player * player)
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return false;
	ClanMember * member = (*found);
	if (_clanBattle->isInBattling())
	{
		player->sendMsgCode(0, 1317);
		return false;
	}
    getAllocBack(*member);
	// std::string oldLeaderName = (*_members.begin())->player->getName();
    if( player == getOwner() && _members.size() != 1)
    {
        // XXX 帮主不能退出帮派, 帮派只剩下帮主一人时，可解散帮派
		player->sendMsgCode(0, 1318);
        return false;
    }

	_members.erase(found);
	delete member;
	player->setClan(NULL);
    player->delClanTask();

	ClanBattle * battleClan = player->getClanBattle();
	if (battleClan != NULL)
		battleClan->kickClanBattler(player);

	{
		Stream st(REP::CLAN_INFO_UPDATE);
		st << static_cast<UInt8>(2) << player->getId();
		st << Stream::eos;
		broadcast(st);
	}

	{
		Stream st(REP::CLAN_MEMBER_OPERATE);
		st << static_cast<UInt8>(4) << static_cast<UInt8>(1) << player->getId() << Stream::eos;
		player->send(st);
	}

	SysMsgItem * sysMsgItem = globalSysMsg[461];
	if (sysMsgItem != NULL)
	{
		Stream st;
		sysMsgItem->getvap(&st, player->getName().c_str());
		broadcast(st);
	}
	{
		Stream st;
		_clanDynamicMsg->addCDMsg(1, player->getName(), &st);
		broadcast(st);
	}

	if (_members.empty())
		disband(player);
	else
	{
		DB().PushUpdateData("DELETE FROM `clan_player` WHERE `playerId` = %"I64_FMT"u", player->getId());
		DB().PushUpdateData("DELETE FROM `clan_skill` WHERE `playerId` = %"I64_FMT"u", player->getId());
		// updateRank(NULL, oldLeaderName);
	}
	
	return true;
}

bool Clan::handoverLeader(Player * leader, UInt64 pid)
{
	Mutex::ScopedLock lk(_mutex);
	if (leader->getId() == pid)
		return false;
	Members::iterator cmLeaderIter = find(leader);
	if (cmLeaderIter == _members.end() || (*cmLeaderIter)->cls != 4)
		return false;
	ClanMember * cmLeader = *cmLeaderIter;
	Members::iterator found = find(pid);
	if (found == _members.end())
		return false;
	ClanMember * cmPlayer = *found;
	_members.erase(cmLeaderIter);
	_members.erase(found);
	cmLeader->cls = 0;
	cmPlayer->cls = 4;
	_members.insert(cmLeader);
	_members.insert(cmPlayer);
    practicePlace.replaceOwner(cmLeader->player, cmPlayer->player);
	{
		Stream st;
		_clanDynamicMsg->addCDMsg(9, cmPlayer->player->getName(), 4, &st);
		broadcast(st);
	}
	{
		Stream st;
		_clanDynamicMsg->addCDMsg(13, cmLeader->player->getName(), cmPlayer->player->getName(), &st);
		broadcast(st);
	}

    broadcastMemberInfo(*cmLeader, 1);
    broadcastMemberInfo(*cmPlayer, 1);

    DB().PushUpdateData("UPDATE `clan_player` SET `cls` = %u WHERE `playerId` = %u", cmLeader->cls, cmLeader->player->getId());
    DB().PushUpdateData("UPDATE `clan_player` SET `cls` = %u WHERE `playerId` = %u", cmPlayer->cls, cmPlayer->player->getId());
	DB().PushUpdateData("UPDATE `clan` SET `leader` = %"I64_FMT"u WHERE `id` = %u", pid, _id);
	// updateRank(cmLeader, cmLeader->player->getName());

	return true;
}

bool Clan::invite(Player * inviter, std::string invitee)
{
	if (inviter == NULL)
		return false;
	Player * inviteePlayer = globalNamedPlayers[inviter->fixName(invitee)];
	if (inviteePlayer == NULL)
		return false;
	if (getCountry() != inviteePlayer->getCountry())
		return false;
	if (inviteePlayer->GetLev() < 20)
	{
		//inviter->sendMsgCode(0, 2225);
		return false;
	}
	if (inviteePlayer->getClan() != NULL)
		return false;
	if (invite(inviteePlayer))
	{
		Clan * inviterClan = inviter->getClan();
		SYSMSG(title, 348);
		SYSMSGV(content, 349, inviter->getName().c_str(), (inviterClan == NULL ? "" : inviterClan->getName().c_str()));
		inviteePlayer->GetMailBox()->newMail(inviter, 0x22, title, content);
		return true;
	}
	return false;
}

ClanMember * Clan::getClanMember(Player * player)
{
	Members::iterator found = find(player);
	return found == _members.end() ? NULL : *found;
}

Clan::Members::iterator Clan::find( Player * player )
{
	Members::iterator found = _members.begin();
	for (; found != _members.end(); ++found)
	{
		if ((*found)->player == player)
			return found;
	}
	return found;
}

Clan::Members::iterator Clan::find( UInt64 pid )
{
	Members::iterator found = _members.begin();
	for (; found != _members.end(); ++found)
	{
		if ((*found)->player->getId() == pid)
			return found;
	}
	return found;
}

Clan::Members::iterator Clan::end()
{
	return _members.end();
}

bool Clan::apply( Player * player, UInt32 optime, bool writedb )
{
	using namespace std::placeholders;

	if(getCountry() != player->getCountry())
		return false;

	if(player->getClan() != NULL)
		return false;

	std::vector<ClanPendingMember *>::iterator it = std::find_if(_pending.begin(), _pending.end(), std::bind(find_pending_member, _1, player));
	if(it != _pending.end() && (*it)->cls == 16)
		return false;

	ClanPendingMember * cpm = NULL;
	if (it != _pending.end())
	{
		cpm = *it;
		cpm->cls = 16;
	}
	else
	{
		cpm = new(std::nothrow) ClanPendingMember(player, 16, (writedb ? TimeUtil::Now() : optime));
		if (cpm == NULL)
			return false;
		_pending.push_back(cpm);
	}
	broadcastPendingMemberInfo(*cpm);
	if(writedb)
	{
		DB().PushUpdateData("INSERT INTO `clan_pending_player` (`id`, `playerId`, `class`, `optime`) VALUES (%u, %"I64_FMT"u, 16, %u)", _id, player->getId(), cpm->opTime);

		Mutex::ScopedLock lk(_mutex);
		if(_members.size() > 0)
		{
			ClanMember * leader = (*_members.begin());
			SYSMSGV(title, 223, player->getName().c_str());
			SYSMSGV(content, 224, player->getName().c_str());
			leader->player->GetMailBox()->newMail(player, 0x23, title, content);
		}
	}

	return true;
}


bool Clan::declineInvite(Player * player)
{
	using namespace std::placeholders;
	std::vector<ClanPendingMember *>::iterator it = std::find_if(_pending.begin(), _pending.end(), std::bind(find_pending_member, _1, player));
	if(it != _pending.end() && (*it)->cls == 15)
	{
		delete *it;
		_pending.erase(it);
		DB().PushUpdateData("DELETE FROM `clan_pending_player` WHERE `id` = %u AND `playerId` = %"I64_FMT"u", _id, player->getId());
		return true;
	}
	return false;
}


bool Clan::invite( Player * player, UInt32 optime, bool writedb )
{
	using namespace std::placeholders;

	if (getCountry() != player->getCountry())
		return false;

	if (player->getClan() != NULL)
		return false;

	std::vector<ClanPendingMember *>::iterator it = std::find_if(_pending.begin(), _pending.end(), std::bind(find_pending_member, _1, player));
	if (it != _pending.end())
		return false;

	UInt32 curtime = writedb ? TimeUtil::Now() : optime;
	ClanPendingMember * cpm = new(std::nothrow) ClanPendingMember(player, 15, curtime);
	if (cpm == NULL)
		return false;
	_pending.push_back(cpm);

	if(writedb)
	{
		DB().PushUpdateData("INSERT INTO `clan_pending_player` (`id`, `playerId`, `class`, `optime`) VALUES (%u, %"I64_FMT"u, 15, %u)", _id, player->getId(), curtime);
	}

	return true;
}

void Clan::updateRank(ClanMember * cm, std::string oldLeaderName)
{
	UInt8 offset = 0;
	Mutex::ScopedLock lk(_mutex);
	if (_members.empty())
		return;
	Members::iterator iter = _members.begin();
	bool needUpdate = cm != NULL;
	if (!oldLeaderName.empty())
	{
		ClanMember * mem = *iter;
		Player * player = mem->player;
		const std::string& newLeaderName = player->getName();
		if (oldLeaderName != newLeaderName)
		{
			if(needUpdate && mem == cm)
				needUpdate = false;
			if (mem->cls < 4)
			{
				Stream st;
				_clanDynamicMsg->addCDMsg(9, player->getName(), 4, &st);
				broadcast(st);
			}
			mem->cls = 4;
			setLeaderId(player->getId());
			UInt8 cny = player->getCountry();
			globalOwnedClans[cny].remove(oldLeaderName);
			globalOwnedClans[cny].add(newLeaderName, this);
			broadcastMemberInfo(*mem, 1);
		}
	}
	++ iter;
	const UInt8 Ranks[] = { 3, 3, 2, 2, 2, 1, 1, 1, 1, 0 };
	for (; iter != _members.end() && offset < 10; ++ iter, ++ offset)
	{
		ClanMember * mem = *iter;
		if (Ranks[offset] != mem->cls)
		{
			if(needUpdate && mem == cm)
				needUpdate = false;
			if (mem->cls < Ranks[offset])
			{
				Stream st;
				_clanDynamicMsg->addCDMsg(9, mem->player->getName(), Ranks[offset], &st);
				broadcast(st);
			}
			mem->cls = Ranks[offset];
			broadcastMemberInfo(*mem, 1);
		}
	}
	if(needUpdate)
		broadcastMemberInfo(*cm, 1);
}

UInt8 Clan::buildRank( UInt16 pos )
{
	const UInt8 Ranks[] = { 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	return pos >= 9 ? 0 : Ranks[pos];
}

UInt16 Clan::getFavorItemId(UInt8 techId)
{
	const UInt16 FavoritemId[] = {5801, 5802, 5803, 5804, 5805, 5806, 5807, 5808, 580, 5809, 5810, 5811, 5812, 5813, 5814, 5815, 5816, 5817, 5818, 5819};
	UInt32 now = TimeUtil::Now();
	UInt32 index = getTechIdIndex(techId);
	if(TimeUtil::SharpDay() != TimeUtil::SharpDay(0, _flushFavorTime))
	{
		UInt32 r_type1 = uRand(19);
		UInt32 r_type2 = uRand(19);
		UInt32 r_type3 = uRand(19);
		UInt32 r_type4 = uRand(19);
		_favorId[0] = FavoritemId[r_type1];
		_favorId[1] = FavoritemId[r_type2];
		_favorId[2] = FavoritemId[r_type3];
		_favorId[3] = FavoritemId[r_type4];
		_flushFavorTime = now;
		DB().PushUpdateData("UPDATE `clan` SET `favorId1` = %u, `favorId2` = %u, `favorId3` = %u, `favorId4` = %u, `favorTime` = %u WHERE `id` = %u", _favorId[0], _favorId[1], _favorId[2], _favorId[3], _flushFavorTime, _id);
	}

	return _favorId[index];
}


bool Clan::existClanMember(Player * player)
{
	Mutex::ScopedLock lk(_mutex);
	return find(player) != _members.end();
}

bool Clan::existClanMember(UInt64 id)
{
	Mutex::ScopedLock lk(_mutex);
	return find(id) != _members.end();
}

bool Clan::checkDonate(Player * player, UInt8 techId, UInt16 type, UInt32 count)
{
	static UInt16 donateMaxFavor[] = { 40, 40, 40, 40, 40 }; 

	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return false;
	if (_techs->isTechFull(techId))
	{
		//player->sendMsgCode(0, 2229);
		return false;
	}
	ClanMember * mem = (*found);
	UInt16 realCount = 0;
	UInt32 now = TimeUtil::Now();
	UInt32 thisDay = 0, lastFavorDay = 0;
	struct AddItems
	{
		UInt16 flag;
		UInt32 count;
	};
	if(type == 1 || type == 2)
	{
		if(techId < 1 || techId > GData::clanTechTable.size())
			return false;
		if (getLev() >= 5 && now > mem->joinTime && now - mem->joinTime < 24 * 60 * 60)
		{
			//player->sendMsgCode(0, 2218);
			return false;
		}

		GData::SingleClanTechTable & techTable = GData::clanTechTable[techId];
		UInt8 maxLev = techTable.size() - 1;
		UInt8 level = _techs->getLev(techId);
		count += static_cast<UInt32>(_techs->getExtra(techId));
		while (level < maxLev && count >= techTable[level+1].needs)
		{
			++ level;
			count -= techTable[level].needs;
		}
		if(level >= maxLev && count > 0)
			return false;
		if(techTable[level].clanLev > getLev())
		{
			//player->sendMsgCode(0, 2222);
			return false;
		}
	}
	else
	{
		GData::SingleClanTechTable & techTable = GData::clanTechTable[7];//pet tech id 7
		UInt8 level = _techs->getLev(7);//pet tech id 7
		if(mem->clanPet.size() >= techTable[level].effect1 && mem->clanPet.find(techId) == mem->clanPet.end())
			return false;
		ClanPlayerPet& playerPetInfo = mem->clanPet[techId];
		thisDay = TimeUtil::SharpDay(0, now);
		lastFavorDay = TimeUtil::SharpDay(0, playerPetInfo.favorTime);
		if(thisDay != lastFavorDay)
			realCount = count;
		else
			realCount = count + playerPetInfo.favorCount;
		if(realCount > donateMaxFavor[mem->cls])
		{
			player->sendMsgCode(0, 1316);
			return false;
		}
	}

	return true;
}


bool Clan::GMDonate(Player * player, UInt8 techId, UInt16 type, UInt32 count)
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return false;
	ClanMember * mem = (*found);
	if (_techs->donate(player, techId, type, count))
	{
		if (type == 1)
		{
			std::string oldLeaderName = (_members.empty() ? "" : (*_members.begin())->player->getName());
			mem->proffer += count;
            {
                Stream st(REP::CLAN_INFO_UPDATE);
                st << static_cast<UInt8>(5) << mem->proffer << Stream::eos;
                player->send(st);
            }
			// updateRank(mem, oldLeaderName);
			DB().PushUpdateData("UPDATE `clan_player` SET `proffer` = %u WHERE `playerId` = %"I64_FMT"u", mem->proffer, player->getId());	
		}
		//else if (type == 2)
		//{
		//	//tael
		//}
		//else
		//{
		//	//favor
		//}
		return true;
	}

	return false;
}


bool Clan::donate(Player * player, UInt8 techId, UInt16 type, UInt32 count)
{
	Mutex::ScopedLock lk(_mutex);
	if (count == 0) 
		return false;
	Members::iterator found = find(player);
	if (found == _members.end())
		return false;
	ClanMember * mem = (*found);
	if (mem == NULL)
		return false;
	UInt32 now = TimeUtil::Now();
	UInt32 thisDay = 0;
	struct AddItems
	{
		UInt16 flag;
		UInt32 count;
	};
	AddItems items = {type, count};
	if(World::_wday == 4 && type == 1)
		count *= 2;
	if (_techs->donate(player, techId, type, count))
	{
		if (type == 1)
		{
            // 个人资金
			thisDay = TimeUtil::SharpDay(0, now);
#if 0
			mem->achieveCount = items.count + mem->achieveCount;
			{
				Stream st(REP::CLAN_INFO_UPDATE);
				st << static_cast<UInt8>(5) << mem->achieveCount << Stream::eos;
				player->send(st);
			}
#endif
			
			std::string oldLeaderName = (_members.empty() ? "" : (*_members.begin())->player->getName());
            mem->proffer += count;
			{
				Stream st(REP::CLAN_INFO_UPDATE);
				st << static_cast<UInt8>(5) << mem->proffer << Stream::eos;
				player->send(st);
			}
			// updateRank(mem, oldLeaderName);
			{
				Stream st;
				SYSMSGVP(st, 430, mem->player->getName().c_str(), count);
				broadcast(st);
			}
			setProffer(getProffer()+count);
			addClanDonateRecord(player->getName(), techId, count, now);
			DB().PushUpdateData("UPDATE `clan_player` SET `proffer` = %u WHERE `playerId` = %"I64_FMT"u", mem->proffer, player->getId());
			player->GetTaskMgr()->DoAcceptedTask(62207);
		}
		else if (type == 2)
		{
            // 帮派资金
			std::string oldLeaderName = (_members.empty() ? "" : (*_members.begin())->player->getName());
			// updateRank(mem, oldLeaderName);
		}
		else
		{
			//ClanPlayerPet& playerPetInfo = mem->clanPet[techId];
			//thisDay = TimeUtil::SharpDay(0, now);
			//lastFavorDay = TimeUtil::SharpDay(0, playerPetInfo.favorTime);
			//if(thisDay != lastFavorDay)
			//	realCount = count;
			//else
			//	realCount = count + playerPetInfo.favorCount;
			//const std::string field1[4] = {"petFriendness1", "petFriendness2", "petFriendness3", "petFriendness4"};
			//const std::string field2[4] = {"favorCount1", "favorCount2", "favorCount3", "favorCount4"};
			//const std::string field3[4] = {"lastFavorTime1", "lastFavorTime2", "lastFavorTime3", "lastFavorTime4"};
			//for(std::map<UInt8, ClanPlayerPet>::iterator it = mem->clanPet.begin(); it != mem->clanPet.end(); it ++)
			//{
			//	if(it->first == techId)
			//	{
			//		it->second.petFriendness += 2;
			//		if(it->second.petFriendness > 100)
			//			it->second.petFriendness = 100;
			//		it->second.favorCount = realCount;
			//		it->second.favorTime = now;
			//		UInt32 index = getTechIdIndex(it->first);
			//		DB().PushUpdateData("UPDATE `clan_player` SET `%s` = %u, `%s` = %u, `%s` = %u WHERE `playerId` = %"I64_FMT"u;", field1[index].c_str(), it->second.petFriendness, field2[index].c_str(), it->second.favorCount, field3[index].c_str(), it->second.favorTime, player->getId());
			//	}					
			//	else
			//	{
			//		if(it->second.petFriendness >= 1)
			//		{
			//			it->second.petFriendness -= 1;
			//			UInt32 index = getTechIdIndex(it->first);
			//			DB().PushUpdateData("UPDATE `clan_player` SET `%s` = %u WHERE `playerId` = %"I64_FMT"u;", field1[index].c_str(), it->second.petFriendness, player->getId());
			//		}
			//	}
			//}
		}

		//Stream st(REP::CLAN_TECH);
		//st << static_cast<UInt8>(4) << techId << player->getName() << count << mem->donateTime << Stream::eos;
		//player->send(st);
		DEBUG_LOG("Player [%s] donate clanId = [%u] techId = [%u] type = [%u] count = [%u]", player->getName().c_str(), _id, techId, type, count);

		return true;
	}
	GameMsgHdr hdr(0x309, player->getThreadId(), player, sizeof(AddItems));
	GLOBAL().PushMsg(hdr, &items);
	return false;
}

void Clan::listMembers( Player * player )
{
	Mutex::ScopedLock lk(_mutex);
	Stream st;
	st.init(REP::CLAN_MEMBER_LIST);
	UInt8 c = _members.size();
	st << static_cast<UInt8>(0) << c;
	Members::iterator offset = _members.begin();
	for (; offset != _members.end(); ++ offset)
	{
		ClanMember * mem = *offset;
        if (!mem || !mem->player)
            continue;
		st << mem->player->getId() << mem->player->getName() << mem->cls << mem->player->GetLev() << static_cast<UInt8>(mem->player->isOnline() ? 1 : 0) << mem->proffer << mem->player->getLastOnline();
	}
	st << Stream::eos;
	player->send(st);
}

void Clan::listPending( Player * player )
{
	purgePending();
	Stream st(REP::CLAN_MEMBER_LIST);
	UInt8 c = 0;
	st << static_cast<UInt8>(1) << c;
	size_t i = 0;
	while(i < _pending.size())
	{
		ClanPendingMember * cmem = _pending[i];
		if(cmem->player->getClan() != NULL)
		{
			DB().PushUpdateData("DELETE FROM `clan_pending_player` WHERE `id` = %u AND `playerId` = %"I64_FMT"u AND (`class` = 16 OR `class` = 15)", _id, cmem->player->getId());
			_pending.erase(_pending.begin() + i);
			continue;
		}
		st << cmem->player->getId() << cmem->player->getName() << cmem->player->GetLev() << cmem->opTime;
		++ c;
		++ i;
	}
	st.data<UInt8>(5) = c;
	st << Stream::eos;
	player->send(st);
}

void Clan::sendInfo( Player * player )
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return;
	ClanMember * member = (*found);
	Stream st(REP::CLAN_REQ_USER);
	Player * owner = getOwner();
    Player* watchman = globalPlayers[_watchman];
    PlayerData& pd = player->getPlayerData();

    st << static_cast<UInt8>(0) << member->cls << static_cast<UInt8>(getCount()) << static_cast<UInt8>(getMaxMemberCount())
        <<  static_cast<UInt8>((pd.ctFinishCount << 4) | CLAN_TASK_MAXCOUNT) << static_cast<UInt32>(getConstruction()) << getClanFunds() << member->proffer
        << _name << (owner == NULL ? "" : owner->getName()) << getFounderName() <<(watchman == NULL ? "" : watchman->getName())
        << _contact << _announce << _purpose;
#if 0
	st << static_cast<UInt8>(0) << _name << (owner == NULL ? "" : owner->getName())
		<< getFounderName() << (watchman == NULL ? "" : watchman->getName()) << getLev() << _contact << _announce << _purpose
		<< static_cast<UInt8>(getCount()) << getDonateAchievement(player) << member->cls;
#endif
	st << Stream::eos;
	player->send(st);
}

void Clan::searchMatch( Player * player )
{
	Stream st(REP::CLAN_REQ_LIST);
	UInt16 c = 1, idx = 0;
	UInt8 cnt = 1;
	st << c << idx << cnt;
	appendListInfo(st);

	st << Stream::eos;
	player->send(st);
}

void Clan::appendListInfo( Stream& st )
{
	Player * owner = getOwner();
	if(owner == NULL)
	{
		st << _id << _name << ""
			<< static_cast<UInt8>(0) << getLev() << static_cast<UInt8>(getCount()) << static_cast<UInt8>(getMaxMemberCount());
	}
	else
	{
		st << _id << _name << owner->getName()
			<< owner->getCountry() << getLev() << static_cast<UInt8>(getCount()) << static_cast<UInt8>(getMaxMemberCount());
	}
}

void Clan::listTechs(Player * player)
{
	Stream st(REP::CLAN_SKILL);
	st << static_cast<UInt8>(0) << _techs->getSize();
	_techs->makeTechInfo(st);
	st << Stream::eos;
	player->send(st);
}

void Clan::listTechDonators(Player * player, UInt8 techId)
{
	Mutex::ScopedLock lk(_mutex);
	MemberDonates& mds = _memberDonates[techId];
	Stream st(REP::CLAN_SKILL);
	st << static_cast<UInt8>(1) << techId << static_cast<UInt8>(mds.size());
	for (MemberDonates::iterator offset = mds.begin(); offset != mds.end(); ++ offset)
	{
		st << (*offset).donateName << static_cast<UInt32>((*offset).donateCount) << (*offset).donateTime;
	}
	st << Stream::eos;
	player->send(st);
}

void Clan::broadcast( Stream& st )
{
	Mutex::ScopedLock lk(_mutex);
	ClanMember * mem = NULL;
	Members::iterator offset;
	for(offset = _members.begin(); offset != _members.end(); ++ offset)
	{
		mem = *offset;
		mem->player->send(st);
	}
}

void Clan::broadcast(SysMsgItem * item)
{
	Mutex::ScopedLock lk(_mutex);
	if (item == NULL)
		return;
	ClanMember * mem = NULL;
	Members::iterator offset;
	for (offset = _members.begin(); offset != _members.end(); ++ offset)
	{
		mem = *offset;
		item->send(mem->player);
	}
}

void Clan::broadcast( const void * data, size_t len)
{
	Mutex::ScopedLock lk(_mutex);
	ClanMember * mem = NULL;
	Members::iterator offset;
	for(offset = _members.begin(); offset != _members.end(); ++ offset)
	{
		mem = *offset;
		mem->player->send(data, len);
	}
}

void Clan::broadcastMemberInfo( ClanMember& cmem, UInt8 t )
{
	Stream st(REP::CLAN_INFO_UPDATE);
	st << t << cmem.player->getId();
	if(t == 0)
		st << cmem.cls << cmem.player->getName() << cmem.player->GetLev() << static_cast<UInt8>(cmem.player->isOnline() ? 1 : 0) << cmem.player->getLastOnline() << Stream::eos;
	else
		st << cmem.cls << cmem.player->GetLev() << static_cast<UInt8>(cmem.player->isOnline() ? 1 : 0) << cmem.proffer << cmem.player->getLastOnline() << Stream::eos;
	broadcast(st);
}

void Clan::broadcastPendingMemberInfo( ClanPendingMember& cpmem )
{
	Stream st(REP::CLAN_INFO_UPDATE);
	st << static_cast<UInt8>(0) << cpmem.player->getId() << static_cast<UInt8>(100) << cpmem.player->getName() << cpmem.player->GetLev() << static_cast<UInt8>(cpmem.player->isOnline() ? 1 : 0) << cpmem.player->getLastOnline() << Stream::eos;
	broadcast(st);
}

void Clan::purgePending()
{
	UInt32 thatTime = TimeUtil::Now() - 10 * 3600 * 24;
	size_t sz = _pending.size();
	size_t i = 0;
	while(i < sz && _pending[i]->opTime < thatTime)
	{
		DB().PushUpdateData("DELETE FROM `clan_pending_player` WHERE `id` = %u AND `playerId` = %"I64_FMT"u AND `class` = 16", _id, _pending[i]->player->getId());
		++ i;
	}
	if(i > 0)
		_pending.erase(_pending.begin(), _pending.begin() + i);
}


void Clan::setContact( const std::string& c, bool announce )
{
	if(_contact == c)
		return;
	_contact = c;
	if(announce)
	{
		Stream st(REP::CLAN_INFO_UPDATE);
		st << static_cast<UInt8>(3) << c << Stream::eos;
		broadcast(st);
		char c2[1024];
		mysql_escape_string(c2, c.c_str(), c.length());
		DB().PushUpdateData("UPDATE `clan` SET `contact` = '%s' WHERE `id` = %u", c2, _id);
	}
}

void Clan::setAnnounce( const std::string& c, bool announce )
{
	if(_announce == c)
		return;
	_announce = c;
	if(announce)
	{
		Stream st(REP::CLAN_INFO_UPDATE);
		st << static_cast<UInt8>(4) << c << Stream::eos;
		broadcast(st);
		char c2[1024];
		mysql_escape_string(c2, c.c_str(), c.length());
		DB().PushUpdateData("UPDATE `clan` SET `announce` = '%s' WHERE `id` = %u", c2, _id);
	}
}

void Clan::setProffer( UInt32 r, bool announce )
{
	if (_proffer == r)
		return;
	_proffer = r;
	if (announce)
	{
		Stream st(REP::CLAN_INFO_UPDATE);
		st << static_cast<UInt8>(5) << r << Stream::eos;
		broadcast(st);
		DB().PushUpdateData("UPDATE `clan` SET `proffer` = %u WHERE `id` = %u", r, _id);
	}
}

bool Clan::hasClanAuthority(Player * player, UInt8 authority)
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return false;
	return ClanAuthority[(*found)->cls][authority] != 0;
}

void Clan::initBuildClan()
{
	Mutex::ScopedLock lk(_mutex);
	_techs->buildTech();
	_clanBattle->initClanBattle();
}

void Clan::disband(Player * player)
{
	//1):清除同盟宗族
	if (_allyClan != NULL)
		_allyClan->delAllyClan(this);

	//2):清除宗族战检测
	clanManager.delBattleClan(_clanBattle);

	//3):从全局中删除
	const std::string& clanName = getName();
	UInt8 cny = player->getCountry();
	globalClans.remove(_id);
	globalClansByCountry[cny].remove(_id);
	globalGlobalNamedClans.remove(clanName);
	globalNamedClans[cny].remove(clanName);
	globalOwnedClans[cny].remove(player->getName());
	clanCache.push(this, false);
	
	DB().PushUpdateData("DELETE FROM `clan` WHERE `id` = %u", _id);
	DB().PushUpdateData("DELETE FROM `clan_pending_player` WHERE `id` = %u", _id);
	DB().PushUpdateData("DELETE FROM `clan_player` WHERE `id` = %u", _id);
	DB().PushUpdateData("DELETE FROM `clan_tech` WHERE `clanId` = %u", _id);

	//4): Maybe bug here
	Members::iterator iter = _members.begin();
	for (; iter != _members.end(); ++ iter)
	{
		delete *iter;
	}
	_members.clear();

	size_t size = _pending.size();
	for (size_t i = 0; i < size; ++ i)
	{
		delete _pending[i];
	}
	_pending.clear();
	//delete _techs;
	//_techs = NULL;
	_clanBattle->clearClanBattle();
	_deleted = true;
}

float Clan::getAutoBattleSpeed()
{ 
	return _techs->getAtuobattleSpeed() / 100.0f + 1.0f;
}

UInt8 Clan::getAutoBattleEffect()
{
	return static_cast<UInt8>(_techs->getAtuobattleSpeed());
}

void Clan::retEnterPlsyersCount(UInt16 *count)
{
	Members::iterator offset = _members.begin();
	for (; offset != _members.end(); ++ offset)
	{
		if(((*offset)->enterCount & 0x0F) != 0 && ((*offset)->enterCount & 0x0F) <= 3)
		{
			count[((*offset)->enterCount & 0x0F) - 1] ++;
		}
	}
}

void Clan::notifyAllyClanInfo(Player * player)
{
	Stream st(REP::CLAN_BATTLE);
	st << static_cast<UInt8>(7) << static_cast<UInt8>((_allyClan != NULL ? 1 : 0) + _enemyClan.size());
	if (_allyClan != NULL)
		st << static_cast<UInt8>(0) << _allyClan->getName() << _allyClan->getLev();
	for (size_t i = 0; i < _enemyClan.size(); ++ i)
	{
		if (_enemyClan[i] != NULL)
			st << static_cast<UInt8>(1) << _enemyClan[i]->getName() << _enemyClan[i]->getLev();
	}
	st << Stream::eos;
	player != NULL ? player->send(st) : broadcast(st);
}

void Clan::notifyAllyClanInfo(Clan * clan, UInt8 type, UInt8 action, Player * player)
{
	Stream st(REP::CLAN_BATTLE);
	st << static_cast<UInt8>(8) << action << type << clan->getName() << clan->getLev() << Stream::eos;
	player != NULL ? player->send(st) : broadcast(st);
}


//player : 是主动方
bool Clan::addAllyClan(Player * player, Player * allyPlayer, Clan * allyClan)
{
	if (allyClan->getId() == _id)
		return false;
	if (hasAllyClan())
	{
		//allyPlayer->sendMsgCode(0, 2228);
		return false;
	}
	if (allyClan->hasAllyClan())
	{
		//allyPlayer->sendMsgCode(0, 2227);
		return false;
	}
	if (!hasClanAuthority(player, 5))
		return false;
	if (!allyClan->hasClanAuthority(allyPlayer, 5))
		return false;
	addAllyClan(allyClan);
	allyClan->addAllyClan(this, false);
	DEBUG_LOG("Player [%s] ally with Player [%s]", player->getName().c_str(), allyPlayer->getName().c_str());
	SysMsgItem * sysMsgItem = globalSysMsg[462];
	if (sysMsgItem != NULL)
	{
		{
			Stream st;
			sysMsgItem->getvap(&st, getName().c_str());
			allyClan->broadcast(st);
			{
				Stream st;
				allyClan->_clanDynamicMsg->addCDMsg(11, getName(), &st);
				allyClan->broadcast(st);
			}
		}
		{
			Stream st;
			sysMsgItem->getvap(&st, allyClan->getName().c_str());
			broadcast(st);
			{
				Stream st;
				_clanDynamicMsg->addCDMsg(11, allyClan->getName(), &st);
				broadcast(st);
			}
		}
	}
	return true;
}

bool Clan::delAllyClan(Player * player, Clan * allyClan)
{
	if (!existClanMember(player))
		return false;
	if (!hasClanAuthority(player, 5))
		return false;
	if (!hasAllyClan(allyClan))
		return false;
	if (!allyClan->hasAllyClan(this))
		return false;	
	delAllyClan(allyClan);
	allyClan->delAllyClan(this);
	SysMsgItem * sysMsgItem = globalSysMsg[463];
	if (sysMsgItem != NULL)
	{
		{
			Stream st;
			sysMsgItem->getvap(&st, getName().c_str());
			allyClan->broadcast(st);
			{
				Stream st;
				allyClan->_clanDynamicMsg->addCDMsg(12, getName(), &st);
				allyClan->broadcast(st);
			}
		}
		{
			Stream st;
			sysMsgItem->getvap(&st, allyClan->getName().c_str());
			broadcast(st);
			{
				Stream st;
				_clanDynamicMsg->addCDMsg(12, allyClan->getName(), &st);
				broadcast(st);
			}
			
		}
	}
	
	return true;
}

void Clan::addAllyClan(Clan * clan, bool writedb)
{
	_allyClanId = clan->getId();
	_allyClan = clan;
	notifyAllyClanInfo(clan, 0, 0);
	if (writedb)
	{
		DB().PushUpdateData("UPDATE `clan` SET `allyClan` = %u WHERE `id` = %u", _allyClanId, _id);
	}
}

void Clan::delAllyClan(Clan * clan)
{
	if (clan != _allyClan)
		return;
	_allyClanId = 0;
	_allyClan = NULL;
	notifyAllyClanInfo(clan, 0, 1);
	DB().PushUpdateData("UPDATE `clan` SET `allyClan` = 0 WHERE `id` = %u", _id);
}

void Clan::addEnemyClanFromDB(UInt32 ci)
{
	if (_enemyClanId.size() >= 2)
		return;
	std::vector<UInt32>::iterator found = std::find(_enemyClanId.begin(), _enemyClanId.end(), ci);
	if (found != _enemyClanId.end())
		return;
	_enemyClanId.push_back(ci);
}

void Clan::addEnemyClanFromDB(Clan * c)
{
	if (_enemyClan.size() >= 2)
		return;
	std::vector<Clan *>::iterator found = std::find(_enemyClan.begin(), _enemyClan.end(), c);
	if (found != _enemyClan.end())
		return;
	_enemyClan.push_back(c);
}

bool Clan::hasEnemyClan(UInt32 ci)
{
	return std::find(_enemyClanId.begin(), _enemyClanId.end(), ci) != _enemyClanId.end();
}

bool Clan::hasEnemyClan(Clan * c)
{
	return std::find(_enemyClan.begin(), _enemyClan.end(), c) !=  _enemyClan.end();
}

bool Clan::addEnemyClan(Player * player, Clan * c)
{
	if (c->getId() == _id)
		return false;
	if (hasEnemyClan(c))
		return false;
	if (!hasClanAuthority(player, 5))
		return false;
	if (hasEnemyClanFull())
		return false;
	_enemyClanId.push_back(c->getId());
	_enemyClan.push_back(c);
	notifyAllyClanInfo(c, 1, 0);
	updateEnemyClandb();
	return true;
}

bool Clan::delEnemyClan(Player * player, Clan * c)
{
	if (c->getId() == _id)
		return false;
	if (!hasEnemyClan(c))
		return false;
	if (!hasClanAuthority(player, 5))
		return false;
	std::vector<UInt32>::iterator found1 =  std::find(_enemyClanId.begin(), _enemyClanId.end(), c->getId());
	if (found1 != _enemyClanId.end())
		_enemyClanId.erase(found1);
	std::vector<Clan *>::iterator found2 =  std::find(_enemyClan.begin(), _enemyClan.end(), c);
	if (found2 != _enemyClan.end())
		_enemyClan.erase(found2);
	notifyAllyClanInfo(c, 1, 1);
	updateEnemyClandb();
	return true;
}

void Clan::updateEnemyClandb()
{
	switch (_enemyClanId.size())
	{
	case 0:
		DB().PushUpdateData("UPDATE `clan` SET `enemyClan1` = 0, `enemyClan2` = 0 WHERE `id` = %u", _id);
		break;
	case 1:
		DB().PushUpdateData("UPDATE `clan` SET `enemyClan1` = %u, `enemyClan2` = 0 WHERE `id` = %u", _enemyClanId[0], _id);	
		break;
	case 2:
		DB().PushUpdateData("UPDATE `clan` SET `enemyClan1` = %u, `enemyClan2` = %u WHERE `id` = %u", _enemyClanId[0], _enemyClanId[1], _id);
		break;
	default:
		break;
	}
}

void Clan::addClanDonateRecordFromDB(const std::string& dn, UInt8 si, UInt16 dc, UInt32 dt)
{
	MemberDonates& mds = _memberDonates[si];
	if (mds.size() >= 30)
		DB().PushUpdateData("DELETE FROM `clan_donate_record` WHERE `clanId` = %u AND `techId` = %u AND `donateTime` = %u", _id, si, dt);
	else
		mds.insert(MemberDonate(dn, dc, dt));
}

void Clan::addSkillFromDB(Player* pl, UInt8 skillId, UInt8 level)
{
	ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
        return;

    ClanSkill& cs = cm->clanSkill[skillId];
    cs.id = skillId;
    cs.level = level;
}

void Clan::buildTechSkill(ClanMember* cm)
{
    UInt8 skillNum = GData::clanSkillTable.size();
    for(UInt8 i = 1; i < skillNum; ++ i)
        addSkill(cm, i);
}

void Clan::addSkill(ClanMember* cm, UInt8 skillId)
{
    if(cm == NULL)
        return;

    ClanSkill& cs = cm->clanSkill[skillId];
    cs.id = skillId;
    cs.level = 0;

	DB().PushUpdateData("REPLACE INTO `clan_skill`(`clanId`, `playerId`, `skillId`, `level`) VALUES(%u, %u, %u, %d)", _id, cm->player->getId(), skillId, 0);
}

UInt8 Clan::getSkillLevel(Player* pl, UInt8 skillId)
{
	ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
        return 0;

    std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.find(skillId);
    if(it == cm->clanSkill.end())
        return 0;

    ClanSkill& cs = it->second;
    return cs.level;
}

UInt8 Clan::skillLevelUp(Player* pl, UInt8 skillId)
{
	Stream st(REP::CLAN_SKILL);
	st << static_cast<UInt8>(8) << skillId;

    UInt8 res = 0;
    do
    {
        ClanMember* cm = getClanMember(pl);
        if(cm == NULL)
        {
            res = 3;
            break;
        }

        std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.find(skillId);
        if(it == cm->clanSkill.end())
        {
            res = 3;
            break;
        }

        ClanSkill& cs = it->second;
        if(_techs->getSkillExtend() < cs.level)
        {
            res = 2;
            break;
        }

        UInt8 level = cs.level + 1;
        GData::SingleClanSkillTable & single = GData::clanSkillTable[cs.id];
        if(level > single.size())
        {
            res = 2;
            break;
        }

        if(cm->proffer < single[level].needs)
        {
            res = 1;
            break;
        }

        cm->proffer -= single[level].needs;
        {
            Stream st(REP::CLAN_INFO_UPDATE);
            st << static_cast<UInt8>(5) << cm->proffer << Stream::eos;
            pl->send(st);
            DB().PushUpdateData("UPDATE `clan_player` SET `proffer` = %u WHERE `playerId` = %u", cm->proffer, cm->player->getId());
        }
        cs.level++;
    } while(false);

    st << res;
	st << Stream::eos;
	pl->send(st);
    return res;
}

void Clan::makeSkillInfo(Stream& st, Player* pl)
{
	ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
    {
        st << static_cast<UInt8>(0);
        return;
    }

    st << cm->clanSkill.size();
	std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.begin();
	for (; it != cm->clanSkill.end(); ++ it)
		st << it->second.id << (GData::clanSkillTable[it->second.id][it->second.level].needs);

    return;
}

void Clan::makeSkillInfo(Stream& st, Player* pl, UInt8 skillId)
{
	ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
        return;

    std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.find(skillId);
    if(it == cm->clanSkill.end())
        return;

    ClanSkill& skill = it->second;
	st << skill.id << (GData::clanSkillTable[skill.id][skill.level].needs);
    return;
}

void Clan::listSkills(Player * player)
{
	Stream st(REP::CLAN_SKILL);
	st << static_cast<UInt8>(6);
	makeSkillInfo(st, player);
	st << Stream::eos;
	player->send(st);
}

void Clan::showSkill(Player* player, UInt8 skillId)
{
    Stream st(REP::CLAN_SKILL);
    st << static_cast<UInt8>(7);
    makeSkillInfo(st, player, skillId);
	st << Stream::eos;
	player->send(st);
}

UInt32 Clan::getSkillHPEffect(Player* pl)
{
    ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
        return 0;

    std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.find(CLAN_SKILL_HP);
    if(it == cm->clanSkill.end())
        return 0;

    ClanSkill& skill = it->second;
	return GData::clanSkillTable[skill.id][skill.level].hp;
}

UInt32 Clan::getSkillAtkEffect(Player* pl)
{
    ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
        return 0;

    std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.find(CLAN_SKILL_ATTACK);
    if(it == cm->clanSkill.end())
        return 0;

    ClanSkill& skill = it->second;
	return GData::clanSkillTable[skill.id][skill.level].attack;
}

UInt32 Clan::getSkillDefendEffect(Player* pl)
{
    ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
        return 0;

    std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.find(CLAN_SKILL_DEFEND);
    if(it == cm->clanSkill.end())
        return 0;

    ClanSkill& skill = it->second;
	return GData::clanSkillTable[skill.id][skill.level].defend;
}

UInt32 Clan::getSkillMagAtkEffect(Player* pl)
{
    ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
        return 0;

    std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.find(CLAN_SKILL_MAGATK);
    if(it == cm->clanSkill.end())
        return 0;

    ClanSkill& skill = it->second;
	return GData::clanSkillTable[skill.id][skill.level].magatk;
}

UInt32 Clan::getSkillMagDefentEffect(Player* pl)
{
    ClanMember* cm = getClanMember(pl);
    if(cm == NULL)
        return 0;

    std::map<UInt8, ClanSkill>::iterator it = cm->clanSkill.find(CLAN_SKILL_MAGDEF);
    if(it == cm->clanSkill.end())
        return 0;

    ClanSkill& skill = it->second;
	return GData::clanSkillTable[skill.id][skill.level].magdef;
}

void Clan::addClanDonateRecord(const std::string& dn, UInt8 si, UInt16 dc, UInt32 dt)
{
	MemberDonates& mds = _memberDonates[si];
	if (mds.size() >= 30)
	{
		const MemberDonate& md = *(mds.begin());
		DB().PushUpdateData("DELETE FROM `clan_donate_record` WHERE `clanId` = %u AND `techId` = %u AND `donateTime` = %u", _id, si, md.donateTime);
		mds.erase(mds.begin());
	}
	mds.insert(MemberDonate(dn, dc, dt));
	Stream st(REP::CLAN_SKILL);
	st << static_cast<UInt8>(4) << si << dn << static_cast<UInt32>(dc) << dt << Stream::eos;
	broadcast(st);
	DB().PushUpdateData("REPLACE INTO `clan_donate_record`(`clanId`, `donateName`, `techId`, `donateCount`, `donateTime`) VALUES(%u, '%s', %u, %u, %u)", _id, dn.c_str(), si, dc, dt);
}

void Clan::sendClanDynamicMsg(Player * player, UInt8 type, UInt16 start, UInt16 count)
{
	Mutex::ScopedLock lk(_mutex);
	UInt16 sz = _clanDynamicMsg->getCDMsgSize(type);
	UInt16 end = start + count;
	if (end > sz)
		end = sz;
	if (end < start)
		count = 0;
	else
		count = end - start;
	Stream st(REP::CLAN_INFO);
	st << type << start << count << sz;
	_clanDynamicMsg->makeCDMsgInfor(st, type, start, count);
	st << Stream::eos;
	player->send(st);
}

#if 0
UInt8 Clan::getLev()
{
	return _techs->getClanLev();
}
#endif

void Clan::setPurpose( const std::string& c, bool writedb )
{
	if(_purpose == c)
		return;
	_purpose = c;
	if(writedb)
	{
		char c2[1024];
		mysql_escape_string(c2, c.c_str(), c.length());
		DB().PushUpdateData("UPDATE `clan` SET `purpose` = '%s' WHERE `id` = %u", c2, _id);
	}
}

UInt8 Clan::setRepoNum( UInt32 itemId, UInt8 itemNum, bool writedb )
{
	if(itemNum == 0)
	{
		std::map<UInt32, UInt8>::iterator it = _repoNum.find(itemId);
		if(it == _repoNum.end())
			return 0;
		_repoNum.erase(it);
		if(writedb)
			DB().PushUpdateData("DELETE FROM `clan_repo` WHERE `id` = %u AND `itemId` = %u", _id, itemId);
		return 0;
	}
	if(itemNum > 90)
		itemNum = 90;
	UInt8& num = _repoNum[itemId];
	if(num == itemNum)
		return itemNum;
	num = itemNum;
	if(writedb)
	{
		DB().PushUpdateData("REPLACE INTO `clan_repo`(`id`, `itemId`, `itemNum`) VALUES(%u, %u, %u)", _id, itemId, itemNum);
		sendRepoInfo();
	}
	return itemNum;
}

UInt8 Clan::addRepo(UInt32 itemId, UInt8 itemNum, UInt8 row)
{
	std::map<UInt32, UInt8>::iterator it = _repoNum.find(itemId);
	if(it == _repoNum.end())
	{
		UInt8 num = setRepoNum(itemId, itemNum);
		if (num != 0)
			sendRepoMail(row, num);
		return num;
	}
	if(itemNum == 0)
		return it->second;
	UInt32 newNum = it->second + itemNum;
	if(newNum > 90)
	{
		newNum = 90;
		itemNum = newNum - it->second;
	}
	if(newNum != it->second)
	{
		it->second = newNum;
		writeRepoNumToDB(it);
	}
	sendRepoMail(row, itemNum);
	return itemNum;
}

void Clan::addRepoNum(UInt32 itemId, UInt8 itemNum)
{
	std::map<UInt32, UInt8>::iterator it = _repoNum.find(itemId);
	if(it == _repoNum.end())
	{
		setRepoNum(itemId, itemNum);
		return;
	}
	UInt16 newNum = static_cast<UInt16>(it->second) + static_cast<UInt16>(itemNum);
	if(newNum > 90)
		newNum = 90;
	it->second = static_cast<UInt8>(newNum);
	writeRepoNumToDB(it);
}


void Clan::sendRepoMail(UInt8 row, UInt8 num)
{
	SYSMSG(title, 440);
	SYSMSG(order, row == 1 ? 452 : (row == 2 ? 453 : 454));
	SYSMSGV(content, 455, order, num);
	for (Members::iterator offset = _members.begin(); offset != _members.end(); ++ offset)
	{
		(*offset)->player->GetMailBox()->newMail(NULL, 0x01, title, content);
	}	
}

void Clan::setFounder(UInt64 id)
{
	_founder = id;
	Player * player = globalPlayers[id];
	_founderName = player == NULL ? "" : player->getName();
}

void Clan::setLeaderId(UInt64 ld, bool writedb)
{
	if (ld == _leader)
		return;
	_leader = ld;
	if (writedb)
	{
		DB().PushUpdateData("UPDATE `clan` SET `leader` = %"I64_FMT"u WHERE `id` = %u", ld, _id);
	}
}

bool Clan::setWatchmanId(UInt64 watchman, bool writedb)
{
	if (watchman == _watchman)
		return false;
    Player* owner = getOwner();
    if(NULL == owner)
        return false;
	Player * plwatchman = globalPlayers[watchman];
    if(NULL == plwatchman)
        return false;

	_watchman = watchman;
    practicePlace.replaceProtecter(owner, watchman);
	if (writedb)
	{
		DB().PushUpdateData("UPDATE `clan` SET `watchman` = %"I64_FMT"u WHERE `id` = %u", watchman, _id);
	}

    Stream st(REP::CLAN_INFO_UPDATE);
    st << static_cast<UInt8>(6) << plwatchman->getName();
    st << Stream::eos;
    broadcast(st);
    return true;
}

void Clan::setConstruction(UInt64 cons, bool writedb)
{
    if (cons == _construction)
        return;
    _construction = cons;

    Stream st(REP::CLAN_INFO_UPDATE);
    st << static_cast<UInt8>(7) << static_cast<UInt32>(_construction);
    st << Stream::eos;
    broadcast(st);

    GData::clanLvlTable.testLevelUp(_level, _construction);
    if (writedb)
    {
		DB().PushUpdateData("UPDATE `clan` SET `level` = %u, `construction` = %"I64_FMT"u WHERE `id` = %u", _level, cons, _id);
    }
}

void Clan::addConstruction(UInt64 cons, bool writedb)
{
    if (cons)
    {
        setConstruction(getConstruction() + cons, writedb);
    }
}

void Clan::fixLeaderId()
{
	if (_members.empty())
		return;
	Members::iterator iter = _members.begin();
	ClanMember * cm = *iter;
	if (cm->cls == 4)
		return;
	_members.erase(iter);
	cm->cls = 4;
	_members.insert(cm);
	setLeaderId(cm->player->getId());
#if 0
	Members::iterator it = _members.begin();
	++ it;
	const UInt8 Ranks[] = { 3, 3, 2, 2, 2, 1, 1, 1, 1, 0 };
	for (UInt8 offset = 0; it != _members.end() && offset < 10; ++ it, ++offset)
	{
		ClanMember * mem = *it;
		if (Ranks[offset] != mem->cls)
			mem->cls = Ranks[offset];
	}
#endif
}

bool Clan::alterLeader()
{
	Mutex::ScopedLock lk(_mutex);
	std::size_t sz = _members.size();
	if (sz <= 1)
		return false;
	ClanMember * leader = NULL, * secondLeader = NULL;
	leader = *_members.begin();
	if (leader == NULL)
		return false;
	_members.erase(_members.begin());
	secondLeader = *_members.begin();
	if (secondLeader == NULL)
	{
		_members.insert(leader);
		return false;
	}
	_members.erase(_members.begin());
	leader->cls = 0;
	secondLeader->cls = 4;
	_members.insert(leader);
	_members.insert(secondLeader);
	{
		Stream st;
		_clanDynamicMsg->addCDMsg(9, secondLeader->player->getName(), 4, &st);
		broadcast(st);
	}
	DB().PushUpdateData("UPDATE `clan` SET `leader` = %"I64_FMT"u WHERE `id` = %u", secondLeader->player->getId(), _id);
	// updateRank(leader, leader->player->getName());
	return true;
}

UInt32 Clan::getDonateAchievement(Player * player)
{
	Members::iterator offset = _members.begin();
	for (; offset != _members.end(); ++ offset)
	{
		if ((*offset)->player == player)
			return (*offset)->proffer;
	}

	return 0;
}

UInt8 Clan::getFromRepo(UInt32 itemId, UInt8 itemNum)
{
	if(itemNum == 0)
		return 0;
	std::map<UInt32, UInt8>::iterator it = _repoNum.find(itemId);
	if(it == _repoNum.end())
	{
		return 0;
	}
	if(it->second < itemNum)
		itemNum = it->second;
	it->second -= itemNum;
	writeRepoNumToDB(it);
	return itemNum;
}

UInt8 Clan::getCountry()
{
	return (_members.size() > 0) ? (*_members.begin())->player->getCountry() : 0; 
}

void Clan::sendRepoInfo( Player * player )
{
	Mutex::ScopedLock lk(_mutex);
	purgeAlloc();
	Stream st(REP::CLANREWARD);
	st << static_cast<UInt8>(_repoNum.size());
	std::map<UInt32, UInt8>::iterator it;
	for(it = _repoNum.begin(); it != _repoNum.end(); ++ it)
	{
		st << static_cast<UInt16>(it->first) << it->second;
	}
	st << Stream::eos;
	if(player != NULL)
		player->send(st);
	else
		broadcast(st);
}

void Clan::allocateRepoAsReward( Player * player, std::map<UInt32, UInt8>& items )
{
	if(items.empty() || player == NULL || player->getClan() != this)
		return;
	
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return;
	ClanMember * member = (*found);
	UInt32 now = TimeUtil::Now();
	std::map<UInt32, UInt8>::iterator it;
	std::map<UInt32, UInt8> r;

	AllocRecord par;
	par.playerName = player->getName();
	par.allocType = 0;
	UInt8 totalNum = 0;

	for(it = items.begin(); it != items.end(); ++ it)
	{
		std::map<UInt32, UInt8>::iterator it2 = _repoNum.find(it->first);
		if(it2 == _repoNum.end() || it2->second < it->second)
			continue;
		AllocItem aitem;
		aitem.itemNum = it->second;
		totalNum += it->second;
		switch(it->first)
		{
		case 9041:
			{
				UInt8 lev = player->GetLev();
				if(lev < 70)
					aitem.itemId = 9201;
				else if(lev < 80)
					aitem.itemId = 9202;
				else if(lev < 90)
					aitem.itemId = 9203;
				else if(lev < 100)
					aitem.itemId = 9204;
				else
					aitem.itemId = 9217;
			}
			break;
		default:
			aitem.itemId = it->first;
			break;
		}
		par.allocItems.push_back(aitem);
		it2->second -= it->second;
		writeRepoNumToDB(it2);
		std::pair<UInt32, AllocItem> air;
		air.first = now;
		air.second.itemId = aitem.itemId;
		air.second.itemNum = aitem.itemNum;
		member->allocItems.insert(air);
		DB().PushUpdateData("INSERT INTO `clan_pending_reward`(`id`, `timeAlloc`, `playerId`, `itemId`, `itemNum`) VALUES(%u, %u, %"I64_FMT"u, %u, %u)", _id, now, player->getId(), aitem.itemId, aitem.itemNum);
	}

	addAllocRecord(now, par);

	sendPersonalRewardList(*member);

	SYSMSG(title, 234);
	SYSMSG(content, 235);
	player->GetMailBox()->newMail(NULL, 1, title, content);
	sendRepoInfo();
	Player * owner = getOwner();
	{
		Stream st;
		_clanDynamicMsg->addCDMsg(5, owner == NULL ? "" : owner->getName(), player->getName(), static_cast<UInt32>(totalNum), &st);		
		broadcast(st);
	}
}

void Clan::purgeAlloc()
{
	UInt32 now = TimeUtil::Now();
	if(now < _nextPurgeTime)
		return;
	_nextPurgeTime = now + 3600;
	Members::iterator offset;
	for(offset = _members.begin(); offset != _members.end(); ++ offset)
	{
		purgeAlloc(now, *(*offset));
	}
}

void Clan::purgeAlloc( UInt32 now, ClanMember& cmem )
{
	std::multimap<UInt32, AllocItem>::iterator iter = cmem.allocItems.begin(), iter_end = cmem.allocItems.upper_bound(now - 3 * 86400);
	if(iter == iter_end)
		return;
	AllocRecord par;
	par.playerName = cmem.player->getName();
	par.allocType = 2;
	for(; iter != iter_end; ++ iter)
	{
		UInt32 iid = iter->second.itemId;
		if((iid >= 9201 && iid <= 9204) || iid == 9217)
			iid = 9041;
		UInt8& num = _repoNum[iid];
		num += iter->second.itemNum;
		par.allocItems.push_back(iter->second);
		DB().PushUpdateData("DELETE FROM `clan_pending_reward` WHERE `id` = %u AND `timeAlloc` = %u AND `playerId` = %"I64_FMT"u", _id, iter->first, cmem.player->getId());
		DB().PushUpdateData("REPLACE INTO `clan_repo`(`id`, `itemId`, `itemNum`) VALUES(%u, %u, %u)", _id, iid, num);
	}
	if(par.allocItems.size() > 0)
	{
		addAllocRecord(now, par);
	}
	cmem.allocItems.erase(cmem.allocItems.begin(), iter_end);
}

void Clan::getAllocBack( ClanMember& cmem )
{
	if(cmem.allocItems.empty())
		return;
	std::multimap<UInt32, AllocItem>::iterator iter;
	AllocRecord par;
	UInt32 now = TimeUtil::Now();
	par.allocType = 2;
	par.playerName = cmem.player->getName();
	for(iter = cmem.allocItems.begin(); iter != cmem.allocItems.end(); ++ iter)
	{
		UInt32 iid = iter->second.itemId;
		if((iid >= 9201 && iid <= 9204) || iid == 9217)
			iid = 9041;
		UInt8& num = _repoNum[iid];
		num += iter->second.itemNum;
		DB().PushUpdateData("REPLACE INTO `clan_repo`(`id`, `itemId`, `itemNum`) VALUES(%u, %u, %u)", _id, iid, num);
		par.allocItems.push_back(iter->second);
	}
	addAllocRecord(now, par);
	cmem.allocItems.clear();
	sendRepoInfo();
}

void Clan::sendPersonalRewardList( Player * player )
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return;
	sendPersonalRewardList(*((*found)));
}

void Clan::sendPersonalRewardList( ClanMember& cmem )
{
	Mutex::ScopedLock lk(_mutex);
	purgeAlloc(TimeUtil::Now(), cmem);
	Stream st(REP::CLAN_BUILD);
	st << static_cast<UInt8>(cmem.allocItems.size());
	std::multimap<UInt32, AllocItem>::iterator iter;
	for(iter = cmem.allocItems.begin(); iter != cmem.allocItems.end(); ++ iter)
	{
		st << iter->first << static_cast<UInt16>(iter->second.itemId) << iter->second.itemNum;
	}
	st << Stream::eos;
	cmem.player->send(st);
}

void Clan::takeReward( Player * player, UInt8 type, UInt32 allocTime, UInt16 itemId )
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return;
	ClanMember * member = (*found);
	if (member == NULL)
		return;
	UInt32 now = TimeUtil::Now();
	purgeAlloc(now, *member);
	std::multimap<UInt32, AllocItem>::iterator iter, iter_begin = member->allocItems.lower_bound(allocTime);
	if(iter_begin == member->allocItems.end())
		return;
	std::multimap<UInt32, AllocItem>::iterator iter_end = member->allocItems.upper_bound(allocTime);
	for(iter = iter_begin; iter != iter_end; ++ iter)
	{
		if(iter->second.itemId == itemId)
		{
			switch(type)
			{
			case 1:
				{
					struct ClanTakeRewardStruct
					{
						UInt32 allocTime;
						AllocItem item;
					};
					ClanTakeRewardStruct ctrs;
					ctrs.allocTime = allocTime;
					ctrs.item = iter->second;
					member->allocItems.erase(iter);
					GameMsgHdr hdr(0x271, player->getThreadId(), player, sizeof(ClanTakeRewardStruct));
					GLOBAL().PushMsg(hdr, &ctrs);
				}
				return;
			case 2:
				{
					UInt32 iid = itemId;
					if(iid >= 9201 && iid <= 9204)
						iid = 9041;
					UInt8& itemNum = _repoNum[iid];
					itemNum += iter->second.itemNum;
					DB().PushUpdateData("REPLACE INTO `clan_repo`(`id`, `itemId`, `itemNum`) VALUES(%u, %u, %u)", _id, iid, itemNum);
					AllocRecord par;
					par.allocType = 2;
					par.playerName = player->getName();
					par.allocItems.push_back(iter->second);
					addAllocRecord(now, par);
					sendRepoInfo();
				}
				break;
			default:
				return;
			}
			Stream st(REP::GET_CLANREWARD);
			st << type << itemId << allocTime << Stream::eos;
			player->send(st);
			DB().PushUpdateData("DELETE FROM `clan_pending_reward` WHERE `id` = %u AND `timeAlloc` = %u AND `playerId` = %"I64_FMT"u AND `itemId` = %u", _id, allocTime, player->getId(), itemId);
			member->allocItems.erase(iter);
			return;
		}
	}
}

void Clan::takeRewardResult( Player * player, bool result, UInt32 timeAlloc, const AllocItem& aitem )
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return;
	ClanMember * member = (*found);
	if (member == NULL)
		return;
	if(result)
	{
		AllocRecord par;
		par.allocType = 1;
		par.playerName = player->getName();
		par.allocItems.push_back(aitem);
		addAllocRecord(TimeUtil::Now(), par);

		Stream st(REP::GET_CLANREWARD);
		st << static_cast<UInt8>(1) << static_cast<UInt16>(aitem.itemId) << timeAlloc << Stream::eos;
		player->send(st);
		DB().PushUpdateData("DELETE FROM `clan_pending_reward` WHERE `id` = %u AND `timeAlloc` = %u AND `playerId` = %"I64_FMT"u AND `itemId` = %u", _id, timeAlloc, player->getId(), aitem.itemId);
	}
	else
	{
		std::pair<UInt32, AllocItem> air;
		air.first = timeAlloc;
		air.second = aitem;
		member->allocItems.insert(air);
	}
}

void Clan::sendAllocRecords( Player * player )
{
	Mutex::ScopedLock lk(_mutex);
	purgeAlloc();

	Stream st(REP::CLANALLOCRECORD);
	st << static_cast<UInt8>(_allocRecords.size());
	for(std::multimap<UInt32, AllocRecord>::iterator it = _allocRecords.begin(); it != _allocRecords.end(); ++ it)
	{
		st << it->second.allocType << it->first << it->second.playerName;
		switch(it->second.allocType)
		{
		case 0:
			{
				st << static_cast<UInt8>(it->second.allocItems.size());
				for(std::vector<AllocItem>::iterator iter = it->second.allocItems.begin(); iter != it->second.allocItems.end(); ++ iter)
				{
					st << static_cast<UInt16>(iter->itemId) << iter->itemNum;
				}
			}
			break;
		default:
			{
				if(it->second.allocItems.empty())
					st << static_cast<UInt16>(0) << static_cast<UInt8>(0);
				st << static_cast<UInt16>(it->second.allocItems[0].itemId) << it->second.allocItems[0].itemNum;
			}
			break;
		}
	}
	st << Stream::eos;
	player->send(st);
}

void Clan::setPendingReward( Player * player, UInt32 allocTime, UInt32 itemId, UInt8 itemNum )
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found = find(player);
	if (found == _members.end())
		return;
	ClanMember * member = (*found);
	if (member == NULL)
		return;
	std::pair<UInt32, AllocItem> air;
	air.first = allocTime;
	air.second.itemId = itemId;
	air.second.itemNum = itemNum;
	member->allocItems.insert(air);
}

void Clan::writeRepoNumToDB( std::map<UInt32, UInt8>::iterator it )
{
	if(it->second == 0)
	{
		DB().PushUpdateData("DELETE FROM `clan_repo` WHERE `id` = %u AND `itemId` = %u", _id, it->first);
		_repoNum.erase(it);
	}
	else
	{
		DB().PushUpdateData("REPLACE INTO `clan_repo`(`id`, `itemId`, `itemNum`) VALUES(%u, %u, %u)", _id, it->first, it->second);
	}
}

void Clan::addAllocRecord( UInt32 tm, AllocRecord& ar, bool writedb )
{
	_allocRecords.insert(std::make_pair(tm, ar));
	if(writedb)
		writeAllocRecordToDB(tm, ar);
}

void Clan::writeAllocRecordToDB( UInt32 tm, AllocRecord& ar )
{
	std::string itemstr;
	size_t sz = ar.allocItems.size();
	for(size_t i = 0; i < sz; ++ i)
	{
		if(i > 0)
			itemstr += "|";
		char pn[64];
		sprintf(pn, "%u,%u", ar.allocItems[i].itemId, ar.allocItems[i].itemNum);
		itemstr += pn;
	}
	DB().PushUpdateData("INSERT INTO `clan_reward_record`(`id`, `type`, `timeAlloc`, `playerName`, `items`) VALUES(%u, %u, %u, '%s', '%s')", _id, ar.allocType, tm, ar.playerName.c_str(), itemstr.c_str());
}

void Clan::broadcastMemberInfo( Player * player )
{
	Mutex::ScopedLock lk(_mutex);
	Members::iterator found= find(player);
	if (found == _members.end())
		return;
	broadcastMemberInfo(*((*found)), 1);
}

void Clan::patchMergedName( UInt32 id, std::string& name )
{
	if(cfg.merged && id >= 0x1000000)
	{
		UInt32 sid = (id >> 24) - 1;
		do
		{
			name.push_back(static_cast<char>((sid % 31) + 1));
			sid /= 31;
		}
		while(sid > 0);
	}
}

float Clan::getClanTechAddon()
{
    // TODO:
    float practiceAddon = 0.0;

    practiceAddon = static_cast<float>(_techs->getPracticeSpeed())/100;

    return practiceAddon;
}

UInt8 Clan::getPracticeSlot()
{
    return _techs->getPracticeSlot();
}

void Clan::sendPracticePlaceInfo(Player* pl)
{
    Player* owner = getOwner();

    UInt8 place = PPLACE_MAX;
    GObject::PlaceData* pd = practicePlace.getPlaceData(owner, place);
    if(pd == NULL)
        return;

    UInt32 price = 0;
    const std::vector<UInt32>& golds = GData::GDataManager::GetGoldOpenSlot();
    if (golds.size() && pd->place.openslot < golds.size())
        price = golds[pd->place.openslot];

    Stream st(REP::CLAN_BUILD);
    st << static_cast<UInt8>(0) << static_cast<UInt8>(pd->place.maxslot) << static_cast<UInt8>(pd->used) << static_cast<UInt16>(price)
        << static_cast<UInt8>(pd->place.slotmoney) << static_cast<UInt8>(pd->place.protmoney) << pd->place.slotincoming << pd->place.protincoming
        << pd->place.enemyCount - pd->place.winCount << pd->place.winCount << place;

    st << Stream::eos;
    pl->send(st);
}

void Clan::broadcastPracticePlaceInfo()
{
    Player* owner = getOwner();

    UInt8 place = PPLACE_MAX;
    GObject::PlaceData* pd = practicePlace.getPlaceData(owner, place);
    if(pd == NULL)
        return;

    UInt32 price = 0;
    const std::vector<UInt32>& golds = GData::GDataManager::GetGoldOpenSlot();
    if (golds.size() && pd->place.openslot < golds.size())
        price = golds[pd->place.openslot];

    Stream st(REP::CLAN_BUILD);
    st << static_cast<UInt8>(0) << static_cast<UInt8>(pd->place.maxslot) << static_cast<UInt8>(pd->used) << static_cast<UInt16>(price)
        << static_cast<UInt8>(pd->place.slotmoney) << static_cast<UInt8>(pd->place.protmoney) << pd->place.slotincoming << pd->place.protincoming
        << pd->place.winCount << pd->place.enemyCount - pd->place.winCount << place;

    st << Stream::eos;
    broadcast(st);
}

ClanCache clanCache;

inline int char_type(UInt8 p)
{
	if(p < 0x20) return -1;
	if(p >= 0xFE) return -1;
	if(p >= 0xFC) return 6;
	if(p >= 0xF8) return 5;
	if(p >= 0xF0) return 4;
	if(p >= 0xE0) return 3;
	if(p >= 0xC0) return 2;
	if(p >= 0x80) return -1;
	if((p >= '0' && p <= '9')
		|| (p >= 'A' && p <= 'Z') || (p >= 'a' && p <= 'z')) return 1;
	return 0;
}

void ClanCache::push( Clan * clan, bool add )
{
	if(add)
		makeKeywordList(clan->getName(), clan->_keywords);
	std::vector<std::string>& slist = clan->_keywords;
	std::vector<std::string>::iterator it;
	for(it = slist.begin(); it != slist.end(); ++ it)
	{
		if(add)
			_keyLists[clan->getCountry()][strlwr(&(*it)[0])].insert(clan);
		else
			_keyLists[clan->getCountry()][strlwr(&(*it)[0])].erase(clan);
	}
}

void ClanCache::search( Player * player, std::string name, UInt8 flag )
{
	if(name.empty())
		return;

	Stream st(REP::CLAN_REQ_LIST);
	std::vector<std::string> slist;
	makeKeywordList(name, slist);
	if(slist.empty())
		st << static_cast<UInt32>(0) << static_cast<UInt8>(0);
	else
	{
		std::set<Clan *> clans[2];
        if(flag == 0)
            searchInternal(0, slist, clans[0]);
        else if(flag == 1)
            searchInternal(1, slist, clans[1]);
        else
        {
            searchInternal(0, slist, clans[0]);
            searchInternal(1, slist, clans[1]);
        }

		UInt16 c = clans[0].size() + clans[1].size();
		UInt16 idx = 0;
		if(c > 20)
			c = 20;
		st << c << idx << static_cast<UInt8>(c);
		std::set<Clan *>::iterator iter;
		for(int i = 0; i < 2; ++ i)
		{
			for (iter = clans[i].begin(); iter != clans[i].end() && c > 0; ++ iter, -- c)
			{
                (*iter)->appendListInfo(st);
            }
		}
	}
	st << Stream::eos;
	player->send(st);
}

void ClanCache::search2(Player * player, std::string name)
{
	if (name.empty())
		return;
	
	std::vector<std::string> slist;
	makeKeywordList(name, slist);
	Stream st(REP::CLAN_OPEN);
	if(!slist.empty())
	{
		std::set<Clan *> clans[2];
        searchInternal(0, slist, clans[0]);
        searchInternal(1, slist, clans[1]);

		UInt16 size = clans[0].size() + clans[1].size();
#if 0
		if (size == 0)
			player->sendMsgCode(0, 2220);
		else if (size > 20) 
#else
		if (size > 20) 
#endif
			size = 20;
		st << static_cast<UInt16>(size) << static_cast<UInt16>(0) << static_cast<UInt8>(size);
		std::set<Clan *>::iterator iter;
		for(int i = 0; i < 2; ++ i)
		{
			for (iter = clans[i].begin(); iter != clans[i].end() && size > 0; ++ iter, -- size)
			{
				Clan * clan = *iter;
				ClanBattle * clanBattle = clan->getClanBattle();
				UInt8 attack = 0;
				if (clanBattle->isInBattling()) attack = 0x10;
				if (clanBattle->isInAttacking()) attack |= 0x01;
				st << clan->getId() << clan->getName() << clan->getLev() << attack << clanBattle->getBattleTime() << clanBattle->getTotalAtkerNum() << clanBattle->getTotalDeferNum() << clanBattle->getWillGainBattlerAward(player->getClan());	
			}
		}
	}
	else
	{
		st << static_cast<UInt16>(0) << static_cast<UInt16>(0) << static_cast<UInt8>(0);
	}

	st << Stream::eos;
	player->send(st);
}

struct EnumStruct
{
	Stream& st;
	UInt16 idx;
	UInt16 start;
	UInt16 end;
};

bool clanEnum(Clan * clan, EnumStruct * es)
{
	if(es->idx >= es->end)
	{
		return false;
	}
	if(es->idx >= es->start)
	{
		clan->appendListInfo(es->st);
	}
	++ es->idx;
	return true;
}

void ClanCache::listAll( Player * player, UInt16 idx, UInt8 cnt, UInt8 cny )
{
	//UInt8 cny = player->getCountry();
	Stream st(REP::CLAN_REQ_LIST);
    UInt16 sz = 0;
    if(cny > 1)
    {
        sz = globalClans.size();
    }
    else
    {
        sz = globalClansByCountry[cny].size();
    }

	if(idx >= sz)
	{
		idx = sz;
		cnt = 0;
	}
	else
	{
		if(idx + cnt > sz)
			cnt = sz - idx;
	}

	st << sz << idx << cnt;

	EnumStruct es = {st, 0, idx, static_cast<UInt16>(idx + cnt)};
    if(cny > 1)
        globalClans.enumerate(clanEnum, &es);
    else
        globalClansByCountry[cny].enumerate(clanEnum, &es);
	st << Stream::eos;
	player->send(st);
}

void ClanCache::searchInternal( UInt8 cny, std::vector<std::string>& slist, std::set<Clan *>& clans )
{
	if(cny > 1)
		return;
	std::vector<std::string>::iterator it;
	bool first = true;
	for(it = slist.begin(); it != slist.end(); ++ it)
	{
		std::map<std::string, std::set<Clan *> > ::iterator it2 = _keyLists[cny].find(strlwr(&(*it)[0]));
		if (it2 == _keyLists[cny].end())
		{
			clans.clear();
			return;
		}
		if(first)
		{
			std::set<Clan *>::iterator iter = it2->second.begin();
			while(iter != it2->second.end())
			{
				if((*iter)->alive())
					++ iter;
				else
					it2->second.erase(iter ++);
			}
			if(it2->second.empty())
			{
				_keyLists[cny].erase(it2);
				clans.clear();
				return;
			}
			clans = it2->second;
			first = false;
		}
		else
		{
			std::set<Clan *>& sr = it2->second;
			std::set<Clan *>::iterator iter = clans.begin();
			while(iter != clans.end())
			{
				if(sr.find(*iter) == sr.end())
					clans.erase(iter ++);
				else
					++ iter;
			}
		}
	}
}

void ClanCache::makeKeywordList( const std::string& n, std::vector<std::string>& slist )
{
	if(n.empty())
		return;
	size_t len = n.length();
	size_t i = 0;
	while(i < len)
	{
		int cur = char_type(n[i]);
		while(cur == 0)
		{
			++ i;
			if(i == len)
				return;
			cur = char_type(n[i]);
		}
		if(cur < 0)
			return;
		if(cur == 1)
		{
			size_t nidx = i + 1;
			while(nidx < len && char_type(n[nidx]) == 1)
			{
				++ nidx;
			}
			if(nidx - i > 1)
			{
				slist.push_back(std::string(n.begin() + i, n.begin() + nidx));
			}
			i = nidx;
		}
		else
		{
			if(i + cur > len)
				return;
			slist.push_back(std::string(n.begin() + i, n.begin() + i + cur));
			i += cur;
		}
	}
}

void Clan::addClanFunds(UInt32 funds)
{
    if(funds != 0)
    {
        _funds += funds;

        Stream st(REP::CLAN_INFO_UPDATE);
        st << static_cast<UInt8>(8) << _funds;
        st << Stream::eos;
        broadcast(st);

		DB().PushUpdateData("UPDATE `clan` SET `funds` = %u WHERE `id` = %u", _funds, _id);
    }
}

void Clan::useClanFunds(UInt32 funds)
{
    if(funds != 0)
    {
        _funds -= funds;

        Stream st(REP::CLAN_INFO_UPDATE);
        st << static_cast<UInt8>(8) << _funds;
        st << Stream::eos;
        broadcast(st);

		DB().PushUpdateData("UPDATE `clan` SET `funds` = %u WHERE `id` = %u", _funds, _id);
    }
}

bool Clan::setClanRank(Player* pl, UInt64 inviteeId, UInt8 cls)
{
    Player* clan_pl = globalPlayers[inviteeId];
    if(!clan_pl)
        return false;

    ClanMember * mem1 = getClanMember(pl);
    ClanMember * mem2 = getClanMember(clan_pl);
    if(!mem1 || !mem2)
        return false;

    if(mem1->cls < 1 || mem1->cls - 2 < mem2->cls)
        return false;

    switch(cls)
    {
    case 3:
        if(getClanRankCount(cls) == 2)
            return false;
        break;
    case 2:
        if(getClanRankCount(cls) == 3)
            return false;
        break;
    case 1:
        if(getClanRankCount(cls) == 4)
            return false;
        break;
    }

    if(mem2->cls != cls)
    {
        mem2->cls = cls;
        _members.erase(mem2);
        _members.insert(mem2);

		broadcastMemberInfo(*mem2, 1);
		DB().PushUpdateData("UPDATE `clan_player` SET `cls` = %u WHERE `playerId` = %u", cls, inviteeId);
    }


    return true;
}

UInt8 Clan::getClanRank(Player* pl)
{
    ClanMember * mem = getClanMember(pl);
    if(mem != NULL)
    {
        return mem->cls;
    }

    return 0;
}

UInt8 Clan::getClanRankCount(UInt8 cls)
{
    UInt8 clsCount = 0;
	Members::iterator found = _members.begin();
	for (; found != _members.end(); ++found)
    {
		if ((*found)->cls == cls)
            break;
    }

	for (; found != _members.end(); ++found)
	{
		if ((*found)->cls == cls)
            clsCount ++;
        else
            break;
	}

    return clsCount;
}

void Clan::addMemberProffer(Player*pl, UInt32 proffer)
{
    ClanMember * mem = getClanMember(pl);
    if(mem)
    {
        mem->proffer += proffer;
        {
            Stream st(REP::CLAN_INFO_UPDATE);
            st << static_cast<UInt8>(5) << mem->proffer << Stream::eos;
            pl->send(st);
        }
        DB().PushUpdateData("UPDATE `clan_player` SET `proffer` = %u WHERE `playerId` = %u", mem->proffer, mem->player->getId());
    }
}

}

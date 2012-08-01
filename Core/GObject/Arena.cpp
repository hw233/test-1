#include "Config.h"
#include "Arena.h"
#include "Player.h"
#include "Fighter.h"
#include "Item.h"
#include "Mail.h"
#include "Server/Cfg.h"
#include "Server/SysMsg.h"
#include "MsgID.h"
#include "Common/Itoa.h"

namespace GObject
{

#define GET_ARENA_NAME(n) char n[1024]; if(_session & 0x8000) { SysMsgItem * mi = globalSysMsg[780]; if(mi != NULL) mi->get(n); else n[0] = 0; } else { strcpy(n, cfg.slugName.c_str()); }
#define GET_PROGRESS_NAME(n, p) char n[1024]; { SysMsgItem * mi = globalSysMsg[781 + p]; if(mi != NULL) mi->get(n); else n[0] = 0; }

#ifdef _FB
#define LIMIT_LEVEL  60
#else
#define LIMIT_LEVEL  70
#endif

const static UInt8 progress_accept[7][13] = {
  // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12 
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},   // 0
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},   // 1
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // 2
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},   // 3
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},   // 4
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},   // 5
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}    // 6
};


bool enum_send_status(void * ptr, void * data )
{
    Player* player = static_cast<Player*>(ptr);
    if(player == NULL)
        return true;

    GObject::arena.sendStatus(player);
    return true;
}

bool enum_send_active(void * ptr, void * data )
{
    Player* player = static_cast<Player*>(ptr);
    if(player == NULL)
        return true;

    GObject::arena.sendActive(player);
    return true;
}


void EliminationPlayer::calcBet(bool won, const char * t)
{
	for(std::map<Player *, UInt8>::iterator it = betMap.begin(); it != betMap.end(); ++ it)
	{
		if(won)
		{
            SYSMSGV(title, 730, t);
            UInt8 rew = it->second * 2;
            SYSMSGV(content, 731, t, name.c_str(), it->second, rew);
            Mail * pMail = it->first->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if (pMail != NULL)
                mailPackageManager.push(pMail->id, MailPackage::Tael, rew);
		}
		else
		{
			SYSMSGV(title, 734, t);
			SYSMSGV(content, 735, t, name.c_str(), it->second);
			it->first->GetMailBox()->newMail(NULL, 0x01, title, content);
		}
	}
}

void EliminationPlayer::resetBet()
{
	betMap.clear();
}

UInt8 EliminationBattle::winner(UInt8 idx)
{
	UInt8 w = 0;
    const UInt8 round[5] = { 3, 3, 5, 5, 7 };
	const UInt8 oflag[7] = {1, 2, 4, 8, 16, 32, 64};
	for(int i = 0; i < round[idx]; ++ i)
		if(wonFlag & oflag[i])
			++ w;
	if((w << 1) > round[idx])
		return 1;
	return 0;
}

UInt8 EliminationBattle::winCount(UInt8 idx)
{
	UInt8 w = 0;
    const UInt8 round[5] = { 3, 3, 5, 5, 7 };
	const UInt8 oflag[7] = {1, 2, 4, 8, 16, 32, 64};
	for(int i = 0; i < round[idx]; ++ i)
		if((wonFlag & oflag[i]) == 0)
			++ w;
	return w;
}

Arena arena;

Arena::Arena():
	_loaded(false), _notified(0), _session(0), _progress(0), _status(0), _round(0), _nextTime(0)
{
	memset(_playerCount, 0, sizeof(_playerCount));
	memset(_winnerColor, 0, sizeof(_winnerColor));
	memset(_finalIdx, 0, sizeof(_finalIdx));
}

void Arena::enterArena( Player * player )
{
    if(_progress != 0 || player->GetLev() < LIMIT_LEVEL)
        return;
    std::map<Player *, ArenaPlayer>::iterator iter = _players.find(player);
    if( iter != _players.end() )
        return;
	GameMsgHdr hdr(0x252, player->getThreadId(), player, 1);
	UInt8 data = 0;
	GLOBAL().PushMsg(hdr, &data);
}

void Arena::commitLineup( Player * player )
{
    bool final = false;
    bool preliminary = false;
    int endi = 0;
    int round = 0;
    int idx = 0;
    switch(_progress)
    {
    case 0:
        {
            std::map<Player*, ArenaPlayer>::iterator it = _players.find(player);
            if(it == _players.end())
                return;
        }
        break;
    case 1:
    case 8:
        preliminary = true; idx = 0;
        break;
    case 2:
    case 9:
        preliminary = true; idx = 1;
        break;
    case 10:
    case 3:
        final = true; endi = 32; round = 0;
        break;
    case 4:
        final = true; endi = 16; round = 1;
        break;
    case 5:
        final = true; endi = 8; round = 2;
        break;
    case 6:
        final = true; endi = 4; round = 3;
        break;
    case 7:
        final = true; endi = 2; round = 4;
        break;
    default:
        return;
    }
    if(final)
    {
        bool find = false;
        for(int i = 0; i < 2; ++ i)
        {
            for(int j = 0; j < endi; ++ j)
            {
                UInt8 nidx = _finalIdx[i][round][j];
                if(_finals[i][nidx].id == player->getId())
                {
                    find = true;
                    break;
                }
            }
            if(find)
                break;
        }
        if(!find)
            return;
    }
    else if(preliminary)
    {
        PreliminaryPlayerListMap::iterator it = _preliminaryPlayers[idx].find(player->getId());
        if(it == _preliminaryPlayers[idx].end())
            return;
    }

	GameMsgHdr hdr(0x252, player->getThreadId(), player, 1);
	UInt8 data = 1;
	GLOBAL().PushMsg(hdr, &data);
}

void Arena::commitArenaForceOnce()
{
    PreliminaryPlayerListIterator it = _preliminaryPlayers_list[0].begin();
    Mutex::ScopedLock lk(globalPlayers.getMutex());
    std::unordered_map<UInt64, Player *>& pm = globalPlayers.getMap();
    for(; it != _preliminaryPlayers_list[0].end(); ++ it)
    {
        PreliminaryPlayer& pp = *it;
        std::unordered_map<UInt64, Player *>::const_iterator it = pm.find(pp.id);
        if(it == pm.end())
            continue;
        Player * pl = it->second;
        if(pl == NULL)
            continue;
        commitLineup(pl);
    }
}

UInt8 Arena::bet1( Player * player, UInt8 state, UInt8 group, UInt64 pid, UInt8 type )
{
	if(group != 0 || state > 1)
		return 0xFF;
    if(progress_accept[state][_progress] == 0)
        return 0xFF;

    PreliminaryPlayerListMap::iterator pit = _preliminaryPlayers[state].find(pid); 
    if(pit == _preliminaryPlayers[state].end())
        return 0xFF;

    PreliminaryPlayerListIterator it = pit->second;
    PreliminaryPlayer& pp = *it;

    UInt16 pos = std::distance(_preliminaryPlayers_list[state].begin(), it);
    std::map<Player*, ArenaPlayer>::iterator ait = _players.find(player);
    if((ait != _players.end() && ait->second.betList[state][0].size() >= 1) || pp.name.empty())
        return 0xFF;
    if(pp.betMap.find(player) != pp.betMap.end())
        return 2;
    pp.betMap[player] = type;

	Stream st(ARENAREQ::BET, 0xEF);
	st << group << pos << Stream::eos;
	NETWORK()->SendToArena(st);
	BetInfo binfo;
	binfo.state = state;
	binfo.round = 1;
	binfo.group = group;
	binfo.recieved = 0;
	binfo.pos = pos;
	binfo.type = type;
    int i = 0;
    if(group > 0)
        i = group - 1;
	_players[player].betList[state][i].push_back(binfo);
	DB1().PushUpdateData("REPLACE INTO `arena_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u, %u)", player->getId(), binfo.round, binfo.state, binfo.group, binfo.recieved, binfo.pos, binfo.type);
	GameMsgHdr hdr(0x230, player->getThreadId(), player, 2);
	UInt16 data = type;
	GLOBAL().PushMsg(hdr, &data);

	return type;
}

UInt8 Arena::bet2( Player * player, UInt8 state, UInt8 group, UInt16 pos, UInt8 type )
{
	if(group < 1 || group > 2 || state > 6 || state < 2 || _status > 0)
		return 0xFF;
    if(progress_accept[state][_progress] == 0)
        return 0xFF;

    UInt16 pos2 = pos;
    int gIdx = group - 1;
    UInt8 fidx = 0;
    switch(state)
    {
    case 2:
    case 3:
        fidx = pos >> _round;
        break;
    case 4:
    case 5:
    case 6:
        if(pos > 7)
            return 0xFF;
        pos2 = _finalIdx[gIdx][2][pos];
        fidx = pos2 >> _round;
        break;
    }
    if(_finalIdx[gIdx][_round][fidx] != pos2 || pos > 31 || _finals[gIdx][pos2].name.empty())
        return 0xFF;

    if( (fidx % 2) == 1)
        fidx -= 1;
    else
        fidx += 1;

    UInt8 pos3 = _finalIdx[gIdx][_round][fidx];
    if(_finals[gIdx][pos2].betMap.find(player) != _finals[gIdx][pos2].betMap.end() || _finals[gIdx][pos3].betMap.find(player) != _finals[gIdx][pos3].betMap.end())
        return 2;
    _finals[gIdx][pos2].betMap[player] = type;

	Stream st(ARENAREQ::BET, 0xEF);
	st << group << pos2 << Stream::eos;
	NETWORK()->SendToArena(st);
	BetInfo binfo;
	binfo.state = state;
	binfo.round = _round + 1;
	binfo.group = group;
	binfo.recieved = 0;
	binfo.pos = pos;
	binfo.type = type;
    int i = 0;
    if(group > 0)
        i = group - 1;
	_players[player].betList[state][i].push_back(binfo);
	DB1().PushUpdateData("REPLACE INTO `arena_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u, %u)", player->getId(), binfo.round, binfo.state, binfo.group, binfo.recieved, binfo.pos, binfo.type);
	GameMsgHdr hdr(0x230, player->getThreadId(), player, 2);
	UInt16 data = type;
	GLOBAL().PushMsg(hdr, &data);
	return type;
}

void Arena::appendLineup2( Stream& st, Player * player)
{
	st << player->getFormation();
	size_t offset = st.size();
	UInt8 c = 0;
	st << c;
	for(UInt8 i = 0; i < 5; ++ i)
	{
		Lineup& pdata = player->getLineup(i);
		if(pdata.available())
		{
			++c;
			st << pdata.pos << static_cast<UInt16>(pdata.fid);
			Fighter * fgt = pdata.fighter;

            st << fgt->getLevel() << fgt->getPotential() << fgt->getCapacity();
            st << fgt->getMaxSoul() << fgt->getPeerlessAndLevel();
            fgt->getAllUpSkillAndLevel(st);
            fgt->getAllPSkillAndLevel4Arena(st);

            fgt->getAttrExtraEquip(st);
		}
	}
	st.data<UInt8>(offset) = c;
}

void Arena::appendLineup( Stream& st, Player * player )
{
	st << player->getFormation();
	size_t offset = st.size();
	UInt8 c = 0;
	st << c;
	for(UInt8 i = 0; i < 5; ++ i)
	{
		Lineup& pdata = player->getLineup(i);
		if(pdata.available())
		{
			++c;
			st << pdata.pos << static_cast<UInt16>(pdata.fid);
			Fighter * fgt = pdata.fighter;

            st << fgt->getLevel() << fgt->getPotential() << fgt->getCapacity();
            st << fgt->getMaxSoul() << fgt->getPeerlessAndLevel();
            st << fgt->getAttrType1();
            st << fgt->getAttrValue1();
            st << fgt->getAttrType2();
            st << fgt->getAttrValue2();
            st << fgt->getAttrType3();
            st << fgt->getAttrValue3();

            fgt->getAllUpSkillAndLevel(st);
            fgt->getAllPSkillAndLevel4Arena(st);
            fgt->getAllUpCittaAndLevel(st);

            for (UInt8 i = 0; i < fgt->getMaxTrumps(); ++i)
                appendEquipInfo(st, fgt->getTrump(i));

			appendEquipInfo(st, fgt->getWeapon());
			appendEquipInfo(st, fgt->getArmor(0));
			appendEquipInfo(st, fgt->getArmor(1));
			appendEquipInfo(st, fgt->getArmor(2));
			appendEquipInfo(st, fgt->getArmor(3));
			appendEquipInfo(st, fgt->getArmor(4));
			appendEquipInfo(st, fgt->getAmulet());
			appendEquipInfo(st, fgt->getRing());
		}
	}
	st.data<UInt8>(offset) = c;
}

void Arena::appendEquipInfo( Stream& st, ItemEquip * equip )
{
	if(equip == NULL)
	{
		st << static_cast<UInt16>(0);
		return;
	}
	ItemEquipData& ied = equip->getItemEquipData();
	st << static_cast<UInt16>(equip->GetItemType().getId()) << ied.enchant << ied.tRank << ied.maxTRank;
	ied.extraAttr2.appendAttrToStream(st);
    ied.spiritAttr.appendAttrToStream(st);
	st << ied.sockets;
	for(UInt8 i = 0; i < ied.sockets; ++ i)
		st << ied.gems[i];
}

#if 0
void Arena::readFrom( BinaryReader& brd )
{
	brd >> _session;
	for(int i = 0; i < 3; ++ i)
		brd >> _winnerColor[i] >> _winner[i];
	UInt8 progress;
	brd >> progress;
	if(progress != _progress)
	{
		_progress = progress;
		_notified = 0;
	}
	switch(_progress)
	{
	case 0:
		if(!_players.empty())
			_players.clear();
		brd >> _nextTime;
		memset(_playerCount, 0, sizeof(_playerCount));
		break;
	case 1:
		DB().PushUpdateData("DELETE FROM `arena_bet`");
	case 2:
		memset(_finalIdx, 0, sizeof(_finalIdx));
		for(int i = 0; i < 3; ++ i) for(int j = 0; j < 31; ++ j)
			_finalBattles[i][j].reset();
		brd >> _nextTime >> _playerCount[0] >> _playerCount[1] >> _playerCount[2];
		for(UInt8 i = 0; i < 3; ++ i)
		{
			UInt32 count = 0;
			brd >> count;
			Mutex::ScopedLock lk(globalPlayers.getMutex());
			std::unordered_map<UInt64, Player *>& pm = globalPlayers.getMap();
			for(UInt32 z = 0; z < count; ++ z)
			{
				UInt64 pid = 0;
				brd >> pid;
				Player * pl = pm[pid];
				if(pl == NULL)
					continue;
				ArenaPlayer& ap = _players[pl];
				ap.group = i;
				brd >> ap.realName;
			}
		}
		break;
	case 3:
		if(!_notified)
		{
			_notified = 1;
			GET_ARENA_NAME(n);
			SYSMSG_BROADCASTV(701, _session & 0x7FFF, n);
		}
		brd >> _nextTime;
		for(UInt8 i = 0; i < 3; ++ i)
		{
			UInt32 count = 0;
			brd >> count;
			Mutex::ScopedLock lk(globalPlayers.getMutex());
			std::unordered_map<UInt64, Player *>& pm = globalPlayers.getMap();
			for(UInt32 z = 0; z < count; ++ z)
			{
				UInt64 pid = 0;
				brd >> pid;
				Player * pl = pm[pid];
				if(pl == NULL)
					continue;
				ArenaPlayer& ap = _players[pl];
				ap.group = i;
				brd >> ap.realName;
				UInt16 bCount = 0;
				brd >> bCount;
				for(UInt16 p = 0; p < bCount; ++ p)
				{
					PriliminaryBattle pb;
					brd >> pb.won >> pb.otherColor >> pb.otherName >> pb.battleTime >> pb.battleId;
					ap.battles.push_back(pb);
				}
			}
		}
		break;
	case 4:
		{
			UInt8 status;
			brd >> status;
			if(_notified && status != _status)
			{
				_status = status;
				_notified = 0;
			}
		}
		brd >> _nextTime;
		for(int i = 0; i < 3; ++ i)
		{
			UInt32 count = 0;
			brd >> count;
			Mutex::ScopedLock lk(globalPlayers.getMutex());
			std::unordered_map<UInt64, Player *>& pm = globalPlayers.getMap();
			for(UInt32 z = 0; z < count; ++ z)
			{
				UInt64 pid = 0;
				brd >> pid;

                std::unordered_map<UInt64, Player *>::const_iterator it = pm.find(pid);
                if(it == pm.end())
                    continue;
				Player * pl = it->second;
				if(pl == NULL)
				{
					std::string tname;
					brd >> tname;
					continue;
				}
				ArenaPlayer& ap = _players[pl];
				ap.group = i;
				brd >> ap.realName;
			}
			for(int j = 0; j < 32; ++ j)
			{
				int cid, sid;
				brd >> cid >> sid >> _finals[i][j].id >> _finals[i][j].color >> _finals[i][j].level >> _finals[i][j].lastRank >> _finals[i][j].name;
				if(cfg.channelNum != cid || cfg.serverNo != sid)
					_finals[i][j].id = 0;
				_finalIdx[i][0][j] = j;
			}
			_round = 0;
			brd >> _round;
			const int aIndex[6] = {0, 16, 24, 28, 30, 31};
			for(int j = 0; j < aIndex[_round]; ++ j)
			{
				brd >> _finalBattles[i][j].wonFlag >> _finalBattles[i][j].battleId[0] >> _finalBattles[i][j].battleId[1] >> _finalBattles[i][j].battleId[2];
			}
			UInt32 r = _round;
			if(_status == 0)
				++ r;
			if(r > 1)
			{
				GET_PROGRESS_NAME(p, _round - 1);
				for(UInt32 j = 1; j < r; ++ j)
				{
					int starti = aIndex[j - 1], endi = aIndex[j];
					for(int k = starti; k < endi; ++ k)
					{
						if(_finalBattles[i][k].winner() == 0)
						{
							UInt8 nidx = _finalIdx[i][j-1][(k - starti) * 2];
							_finalIdx[i][j][k - starti] = nidx;
							if(_status == 0 && j == r - 1)
							{
								_finals[i][nidx].calcBet(true, p);
								_finals[i][_finalIdx[i][j-1][(k - starti) * 2 + 1]].calcBet(false, p);
							}
						}
						else
						{
							UInt8 nidx = _finalIdx[i][j-1][(k - starti) * 2 + 1];
							_finalIdx[i][j][k - starti] = nidx;
							if(_status == 0 && j == r - 1)
							{
								_finals[i][nidx].calcBet(true, p);
								_finals[i][_finalIdx[i][j-1][(k - starti) * 2]].calcBet(false, p);
							}
						}
					}
				}
			}
		}
		if(_loaded)
		{
			Stream st(REP::ARENAPRILIMINARY);
			st << static_cast<UInt8>(2) << Stream::eos;
			NETWORK()->Broadcast(st);
			if(!_notified)
			{
				const int start_hour[3] = {12, 16, 19};
				const int start_min[3] = {15, 0, 0};
				_notified = 1;
				switch(_status)
				{
				case 0:
					if(_round > 0)
					{
						GET_ARENA_NAME(n);
						GET_PROGRESS_NAME(p, _round - 1);
						SYSMSG_BROADCASTV(704, n, p, 3);
						GET_PROGRESS_NAME(p2, _round);
						SYSMSGV(title, 720, _session & 0x7FFF, n, p2);
						SYSMSGV(title2, 722, _session & 0x7FFF, n, p2);
						if(_round < 5)
						{
							char pn[2048] = {0};
							int m = 32 >> _round;
							for(int i = 0; i < 3; ++ i)
							{
								for(int j = 0; j < m; ++ j)
								{
									EliminationPlayer& ep = _finals[i][_finalIdx[i][_round][j]];
									if(ep.id != 0)
									{
										Player * pl = globalPlayers[ep.id];
										if(pl == NULL)
											continue;
										SYSMSGV(sn, 10000, pl->getCountry(), pl->getName().c_str());
										strcat(pn, sn);
									}
								}

								const int aIndex[6] = {0, 16, 24, 28, 30, 31};
								int starti = aIndex[_round - 1];
								int endi = aIndex[_round];
								for(int idx = starti; idx < endi; ++ idx)
								{
									EliminationPlayer& ep1 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2]];
									EliminationPlayer& ep2 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2 + 1]];
									if(ep1.name.empty() || ep2.name.empty())
										continue;
									Player * pl;
									UInt8 winCount = _finalBattles[i][idx].winCount();
									if(winCount > 1)
									{
										if(ep1.id != 0 && (pl = globalPlayers[ep1.id]) != NULL)
										{
											SYSMSGV(content, 721, _session & 0x7FFF, n, p, 3, ep2.name.c_str(), winCount, 3 - winCount, static_cast<UInt32>(ep1.betMap.size()), start_hour[_status], start_min[_status]);
											pl->GetMailBox()->newMail(NULL, 0x01, title, content);
										}
										if(ep2.id != 0 && (pl = globalPlayers[ep2.id]) != NULL)
										{
											SYSMSGV(content, 723, _session & 0x7FFF, n, p, ep1.name.c_str(), 3 - winCount, winCount, static_cast<UInt32>(ep2.betMap.size()), start_hour[_status], start_min[_status]);
											pl->GetMailBox()->newMail(NULL, 0x01, title2, content);
										}
									}
									else
									{
										if(ep2.id != 0 && (pl = globalPlayers[ep2.id]) != NULL)
										{
											SYSMSGV(content, 721, _session & 0x7FFF, n, p, 3, ep1.name.c_str(), 3 - winCount, winCount, static_cast<UInt32>(ep2.betMap.size()), start_hour[_status], start_min[_status]);
											pl->GetMailBox()->newMail(NULL, 0x01, title, content);
										}
										if(ep1.id != 0 && (pl = globalPlayers[ep1.id]) != NULL)
										{
											SYSMSGV(content, 723, _session & 0x7FFF, n, p, ep2.name.c_str(), winCount, 3 - winCount, static_cast<UInt32>(ep1.betMap.size()), start_hour[_status], start_min[_status]);
											pl->GetMailBox()->newMail(NULL, 0x01, title2, content);
										}
									}
								}
							}
							GET_ARENA_NAME(n);
							GET_PROGRESS_NAME(p, _round);
							SYSMSG_BROADCASTV(702, pn, _session & 0x7FFF, n, p);
						}
					}
					break;
				case 1:
				case 2:
					if(_round > 0 && _round < 6)
					{
						GET_ARENA_NAME(n);
						GET_PROGRESS_NAME(p, _round - 1);
						SYSMSG_BROADCASTV(704, n, p, _status);

						SYSMSGV(title, 716, p, _status);
						SYSMSGV(title2, 718, p, _status);
						for(int i = 0; i < 3; ++ i)
						{
							const int aIndex[6] = {0, 16, 24, 28, 30, 31};
							int starti = aIndex[_round - 1];
							int endi = aIndex[_round];
							for(int idx = starti; idx < endi; ++ idx)
							{
								EliminationPlayer& ep1 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2]];
								EliminationPlayer& ep2 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2 + 1]];
								if(ep1.name.empty() || ep2.name.empty())
									continue;
								Player * pl;
								if((_finalBattles[i][idx].wonFlag & _status) == 0)
								{
									if(ep1.id != 0 && (pl = globalPlayers[ep1.id]) != NULL)
									{
										SYSMSGV(content, 717, _session & 0x7FFF, n, p, _status, ep2.name.c_str(), start_hour[_status], start_min[_status]);
										pl->GetMailBox()->newMail(NULL, 0x01, title, content);
									}
									if(ep2.id != 0 && (pl = globalPlayers[ep2.id]) != NULL)
									{
										SYSMSGV(content, 719, _session & 0x7FFF, n, p, _status, ep1.name.c_str(), start_hour[_status], start_min[_status]);
										pl->GetMailBox()->newMail(NULL, 0x01, title2, content);
									}
								}
								else
								{
									if(ep2.id != 0 && (pl = globalPlayers[ep2.id]) != NULL)
									{
										SYSMSGV(content, 717, _session & 0x7FFF, n, p, _status, ep1.name.c_str(), start_hour[_status], start_min[_status]);
										pl->GetMailBox()->newMail(NULL, 0x01, title, content);
									}
									if(ep1.id != 0 && (pl = globalPlayers[ep1.id]) != NULL)
									{
										SYSMSGV(content, 719, _session & 0x7FFF, n, p, _status, ep2.name.c_str(), start_hour[_status], start_min[_status]);
										pl->GetMailBox()->newMail(NULL, 0x01, title2, content);
									}
								}
							}
						}
					}
					break;
				}
			}
			if(_status == 0)
			{
				for(int i = 0; i < 3; ++ i)
				{
					for(int j = 0; j < 32; ++ j)
					{
						_finals[i][j].resetBet();
					}
				}
			}
		}
		else
		{
			UInt8 r = _round;
			if(_status > 0 && r > 0)
			{
				-- r;
			}
			for(std::map<Player *, ArenaPlayer>::iterator it = _players.begin(); it != _players.end(); ++ it)
			{
				std::vector<BetInfo>& blist = it->second.betList;
				if(blist.empty())
					continue;
				BetInfo& bi = blist.back();
				if(bi.round == r)
				{
					_finals[bi.group][bi.pos].betMap[it->first] = bi.tael;
				}
			}
		}
		break;
	}
	UInt32 recCount = 0;
	brd >> recCount;
	for(UInt32 i = 0; i < recCount; ++ i)
	{
		UInt64 playerId;
		brd >> playerId;
		RecordInfo& ri = _records[playerId];
		brd >> ri.lastRank >> ri.session >> ri.rank;
	}
	if(!_loaded)
		_loaded = true;
}
#endif
void Arena::readFrom( BinaryReader& brd )
{
    UInt8 sIdx = 0;
	brd >> sIdx >> _session;
	UInt8 progress;
	UInt8 reg;
	brd >> reg >> progress >> _nextTime;
    bool fStatus = false;
	if(progress != _progress)
	{
		_progress = progress;
		_notified = 0;
        fStatus = true;
	}

	switch(_progress)
	{
	case 0:
		if(!_players.empty() && sIdx == 0)
        {
            DB1().PushUpdateData("DELETE FROM `arena_bet`");
            _playerCount[0] = 0;
			_players.clear();
        }
        if(!_preliminaryPlayers[0].empty() && sIdx == 0)
        {
            _preliminaryPlayers[0].clear();
            _preliminaryPlayers_list[0].clear();
            _preliminaryPlayers_list_set[0].clear();
        }
        if(!_preliminaryPlayers[1].empty() && sIdx == 0)
        {
            _preliminaryPlayers[1].clear();
            _preliminaryPlayers_list[1].clear();
            _preliminaryPlayers_list_set[1].clear();
        }
        readPlayers(brd, sIdx);
        break;
    case 8:
	case 9:
        readPrePlayers(brd, sIdx);
        if(reg == 1)
            readHistories(brd);
		break;
	case 2:
	case 1:
        if(reg == 1)
            readPrePlayers(brd, sIdx);
        readHistories(brd);
		break;
    case 10:
	case 3:
	case 4:
    case 5:
    case 6:
    case 7:
    case 11:
        if(reg == 1)
        {
            readPrePlayers(brd, sIdx);
            readHistories(brd);
        }

        if(sIdx == 0)
            readElimination(brd);
        break;
	}
    if(fStatus)
    {
        globalPlayers.enumerate(enum_send_status, static_cast<void *>(NULL));
    }

	if(!_loaded)
    {
		_loaded = true;
        globalPlayers.enumerate(enum_send_active, static_cast<void *>(NULL));
    }
}

#if 0
void Arena::sendInfo( Player * player )
{
	Stream st(REP::SERVER_ARENA_INFO);
	std::map<Player *, ArenaPlayer>::iterator it = _players.find(player);
	if(it == _players.end())
	{
		st << cfg.slugName << static_cast<UInt8>(0);
	}
	else
	{
		st << it->second.realName << static_cast<UInt8>(it->second.group + 1);
	}
	std::map<UInt64, RecordInfo>::iterator iter = _records.find(player->getId());
	if(iter == _records.end())
		st << static_cast<UInt8>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0);
	else
		st << static_cast<UInt8>(iter->second.lastRank) << static_cast<UInt8>(iter->second.rank) << static_cast<UInt16>(iter->second.session);
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;
	switch(_progress)
	{
	case 0:
        {
            st << static_cast<UInt8>(1);
            UInt8 find = 0;
            std::map<Player *, ArenaPlayer>::iterator it = _players.find(player);
            if(it != _players.end())
                find = 1;
            st << find << leftTime << _playerCount[0];
        }
		break;
	case 1:
	case 2:
        {
            st << static_cast<UInt8>(_progress + 1);
            UInt8 flag = 0;
            UInt8 type = _progress - 1;
            std::map<UInt64, PreliminaryPlayer>::iterator it = _preliminaryPlayers[type].find(player->getId());
            if(it == _preliminaryPlayers[type].end())
                flag = 1;
            st << flag;
            if(flag == 0)
            {
                st << leftTime;
                ArenaPlayer& ap = _players[player];
                UInt8 cnt = static_cast<UInt8>(ap.battles[type].size());
                st << cnt;
                int ttimes = 0;
                int dtimes = 0;
                for(int i = 0; i < cnt; ++ i)
                {
                    UInt8 flag = ap.group << 4 | ap.battles[type][i].won;
                    st << flag;
                    if(ap.group == 0)
                    {
                        ++ ttimes;
                        st << ttimes;
                    }
                    else
                    {
                        ++ dtimes;
                        st << ttimes;
                    }
                    st << ap.battles[type][i].battleId << ap.battles[type][i].otherName;
                }
            }
            std::map<Player*, ArenaPlayer>::iterator ait = _players.find(player);
            if(ait == _players.end())
            {
                st << static_cast<UInt8>(1);
            }
            else
            {
                ArenaPlayer& ap = ait->second;
                std::vector<BetInfo>& blist = ap.betList;
                UInt8 cnt = blist.size();
                UInt8 bets = 0;
                for(int i = 0; i < cnt; ++ i)
                    if(blist[i].state == type)
                        ++ bets;
                st << static_cast<UInt8>(1 - bets);
            }
            UInt16 premNum = static_cast<UInt16>(_preliminaryPlayers[type].size());
            st << premNum;
            for(std::map<UInt64, PreliminaryPlayer>::iterator pit = _preliminaryPlayers[type].begin(); pit != _preliminaryPlayers[type].end(); ++ pit)
            {
                PreliminaryPlayer& pp = pit->second;
                UInt8 fSupport = 0;
                std::map<Player *, UInt8>::iterator it = pp.betMap.find(player);
                if(it != pp.betMap.end())
                    fSupport = 1;
                st << pp.battlePoint << static_cast<UInt16>(pp.support) << fSupport << pp.level << pp.name;
            }
        }
		break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
		break;
	}
	st << Stream::eos;
	player->send(st);
}
#endif

#if 0
void Arena::sendElimination( Player * player, UInt8 group )
{
	bool resultOnly = (group & 0x80) > 0;
	UInt8 g = (group & 0x7F) - 1;
	if(g >= 3)
		return;
	if(_progress < 4)
		return;
	Stream st(REP::SERVER_ARENA_ELIM);
	st << group;
	std::map<Player *, ArenaPlayer>::iterator iter = _players.find(player);
	const int aIndex[6] = {0, 16, 24, 28, 30, 31};
	if(iter == _players.end())
		st << static_cast<UInt8>(0);
	else
	{
		std::vector<BetInfo>& blist = iter->second.betList;
		st << static_cast<UInt8>(blist.size());
		for(std::vector<BetInfo>::iterator it = blist.begin(); it != blist.end(); ++ it)
		{
			EliminationPlayer& pl = _finals[it->group][it->pos];
			st << static_cast<UInt8>(((it->group + 1) << 3) + it->round);
			st << pl.level << pl.name << it->type;
			UInt8 rpos = it->pos >> it->round;
			EliminationBattle& eb = _finalBattles[it->group][aIndex[it->round] + (rpos >> 1)];
			if((rpos & 1) == 0)
				st << eb.wonFlag;
			else
				st << static_cast<UInt8>((~eb.wonFlag) & 0x07);
			st << eb.battleId[0] << eb.battleId[1] << eb.battleId[2];
		}
	}
	if(!resultOnly)
	{
		for(int i = 0; i < 32; ++ i)
		{
			st << _finals[g][i].level << _finals[g][i].name;
		}
	}
	st << _round;
	for(int j = 0; j < aIndex[_round]; ++ j)
	{
		st << _finalBattles[g][j].wonFlag << _finalBattles[g][j].battleId[0] << _finalBattles[g][j].battleId[1] << _finalBattles[g][j].battleId[2];
	}
	st << Stream::eos;
	player->send(st);
}
#endif

void Arena::push( Player * player, UInt8 group, const std::string& rname )
{
	ArenaPlayer& ap = _players[player];
	if(ap.group == 0xFF && group <= 3)
		++ _playerCount[0];
	ap.group = group;
	ap.realName = rname;
}

#if 0
void Arena::pushPriliminary( Player * player, UInt8 won, UInt8 type, const std::string& name, UInt32 btime, UInt32 bid, float rate )
{
	ArenaPlayer& ap = _players[player];
	if(won == 3)
	{
		if(!ap.battles.empty())
		{
			if(ap.battles.back().won == 0)
				ap.battles.back().won = 4;
			else
				ap.battles.back().won = 3;
		}
		else
		{
			PriliminaryBattle pb;
			pb.won = won;
			pb.type = 0;
			pb.battleTime = btime;
			pb.battleId = 0;
			ap.battles.push_back(pb);
		}
		SYSMSGV(sn, 10000, player->getCountry(), player->getName().c_str());
		GET_ARENA_NAME(n);
		GET_PROGRESS_NAME(p, 0);
		SYSMSG_BROADCASTV(702, sn, _session & 0x7FFF, n, p);

		UInt32 twon = 0, tloss = 0;
		for(std::vector<PriliminaryBattle>::iterator it = ap.battles.begin(); it != ap.battles.end(); ++ it)
		{
			if(it->otherName.empty())
				continue;
			if(it->won == 1 || it->won == 3)
			{
				++ twon;
			}
			else
			{
				++ tloss;
			}
		}
		SYSMSG(title, 710);
		if(twon > 2)
		{
			SYSMSGV(content, 711, _session & 0x7FFF, twon, tloss);
			Mail * pMail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
			if (pMail != NULL)
				mailPackageManager.push(pMail->id, 9040, 2, true);
		}
		else
		{
			SYSMSGV(content, 712, _session & 0x7FFF, twon, tloss);
			player->GetMailBox()->newMail(NULL, 0x01, title, content);
		}
	}
	else
	{
		PriliminaryBattle pb;
		pb.won = won;
		pb.type = type;
		pb.otherName = name;
		pb.battleTime = btime;
		pb.battleId = bid;
		ap.battles.push_back(pb);

		if(won == 2)
		{
			UInt32 twon = 0, tloss = 0;
			for(std::vector<PriliminaryBattle>::iterator it = ap.battles.begin(); it != ap.battles.end(); ++ it)
			{
				if(it->won == 1 || it->won == 3)
				{
					++ twon;
				}
				else
				{
					++ tloss;
				}
			}
			SYSMSG(title, 713);
			if(twon > 2)
			{
				SYSMSGV(content, 714, _session & 0x7FFF, twon, tloss, rate);
				Mail * pMail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
				if (pMail != NULL)
					mailPackageManager.push(pMail->id, 9040, 2, true);
			}
			else
			{
				SYSMSGV(content, 715, _session & 0x7FFF, twon, tloss, rate);
				player->GetMailBox()->newMail(NULL, 0x01, title, content);
			}
		}
	}
}
#endif
void Arena::pushPriliminary(BinaryReader& br)
{
    UInt8 won = 0;
    UInt8 type = 0;
    UInt8 old_type = 0;
    UInt8 heroId = 0;
    std::string name;
    UInt32 btime = 0;
    UInt32 bid = 0;
    UInt64 pid = 0;
    UInt32 cid = 0;
    UInt32 sid = 0;

	br >> cid >> sid >> pid >> won >> type >> heroId >> name >> btime >> bid;
    if(type < 1)
        return;
    old_type = type;
    if(type > 2)
    {
        type = 2;
    }

    UInt64 ppid = pid;
    if((cid != cfg.channelNum || sid != cfg.serverNo) && (ppid >> 48) == 0)
        ppid = pid | (static_cast<UInt64>(sid) << 48) | (static_cast<UInt64>(cid) << 40);

	GObject::Player * player = GObject::globalPlayers[ppid];

    GET_PROGRESS_NAME(p, type-1);

    if(won == 3 || won == 4 || won == 2)
    {
        bool flag[5] = {false, false, false, true, true};
        PreliminaryPlayerListMap::iterator pit = _preliminaryPlayers[type-1].find(ppid);
        if(pit != _preliminaryPlayers[type-1].end())
        {
            PreliminaryPlayerListIterator ppit = pit->second;
            PreliminaryPlayer& pp = *ppit;
            UInt16 pos = std::distance(_preliminaryPlayers_list[type-1].begin(), ppit);
            calcBet(pp, pos, type-1, 0, flag[won], p);
        }
    }

	if(player != NULL)
    {
        if(bid != 0)
        {
            Stream st(REP::ARENAPRILIMINARY);

            const UInt8 won_mod[] = {1, 0, 1, 0, 1};
            UInt8 won2 = won_mod[won];
            st << static_cast<UInt8>(type) << won2 << heroId << bid << name << Stream::eos;
            player->send(st);
        }

        ArenaPlayer& ap = _players[player];
        if(won == 3 || won == 4)
        {
            UInt32 twon = 0, tloss = 0;
            for(std::vector<PriliminaryBattle>::iterator it = ap.battles[type-1].begin(); it != ap.battles[type-1].end(); ++ it)
            {
                if(it->otherName.empty())
                    continue;
                if(it->won == 1 || it->won == 3)
                {
                    ++ twon;
                }
                else
                {
                    ++ tloss;
                }
            }

            const UInt32 score[2][2] = {
                {40, 80},
                {50, 100}
            };
            UInt32 award = 0;
            award = score[type-1][0]*tloss + score[type-1][1]*twon;
            int g_type = 709;
            if(old_type == 2)
                g_type = 710;
            else if(old_type == 3)
                g_type = 711;
            SYSMSGV(g, g_type);
            SYSMSGV(title, 712, p, g);
            SYSMSGV(content, 713, _session, p, twon, tloss, award, g);
            player->GetMailBox()->newMail(NULL, 0x01, title, content);
        }
        else
        {
            PriliminaryBattle pb;
            pb.won = won;
            pb.type = type;
            pb.otherHeroId = heroId;
            pb.otherName = name;
            pb.battleTime = btime;
            pb.battleId = bid;
            ap.battles[type-1].push_back(pb);

            const UInt32 score[2][3] = {
                {40, 80, 40},
                {50, 100, 50}
            };
            //player->AddVar(VAR_MONEY_ARENA, score[type-1][won]);

            GameMsgHdr hdr(0x251, player->getThreadId(), player, 4);
            UInt32 data = score[type-1][won];
            GLOBAL().PushMsg(hdr, &data);

            player->AddVar(VAR_MONEY_ARENA2 + type - 1, score[type-1][won]);

            if(won == 2)
            {
                UInt32 twon = 0, tloss = 0;
                for(std::vector<PriliminaryBattle>::iterator it = ap.battles[type-1].begin(); it != ap.battles[type-1].end(); ++ it)
                {
                    if(it->won == 1 || it->won == 3)
                    {
                        ++ twon;
                    }
                    else
                    {
                        ++ tloss;
                    }
                }
                SYSMSGV(title, 714, p);
                UInt32 award = score[type-1][0]*tloss + score[type-1][1]*twon;
                SYSMSGV(content, 715, _session, p, twon, tloss, award);
                player->GetMailBox()->newMail(NULL, 0x01, title, content);
            }
        }
    }
}

void Arena::pushBetFromDB( Player * player, UInt8 round, UInt8 state, UInt8 group, UInt8 recieved, UInt16 pos, UInt8 type )
{
	BetInfo binfo;
	binfo.state = state;
	binfo.round = round;
	binfo.group = group;
	binfo.recieved = recieved;
	binfo.pos = pos;
	binfo.type = type;
    int i = 0;
    if(group > 0)
        i = group - 1;
	_players[player].betList[state][i].push_back(binfo);
}

void Arena::pushPriliminaryCount( UInt32 * c )
{
	_playerCount[0] = c[0];
	_playerCount[1] = c[1];
	_playerCount[2] = c[2];
}

void Arena::check()
{
	switch(_progress)
	{
	case 1:
		if(!_notified && TimeUtil::Now() + 600 >= _nextTime)
		{
			_notified = 1;
			GET_ARENA_NAME(n);
			SYSMSG_BROADCASTV(700, _session & 0x7FFF, n, 10);
		}
		break;
	case 4:
		if(_status == 0 && _notified == 1 && TimeUtil::Now() + 600 >= _nextTime)
		{
			_notified = 2;
			GET_ARENA_NAME(n);
			GET_PROGRESS_NAME(p, _round);
			SYSMSG_BROADCASTV(703, n, p, 10);
		}
		break;
	}
}

void Arena::readPrePlayers(BinaryReader& brd, UInt8 sIdx)
{
    Mutex::ScopedLock lk(globalPlayers.getMutex());
    std::unordered_map<UInt64, Player *>& pm = globalPlayers.getMap();
    for(int i = 0; i < 2; ++ i)
    {
        if(!_preliminaryPlayers[i].empty() && sIdx == 0)
        {
            _preliminaryPlayers[i].clear();
            _preliminaryPlayers_list[i].clear();
            _preliminaryPlayers_list_set[i].clear();
        }

        UInt32 count;
        brd >> count;
        for(UInt32 z = 0; z < count; ++ z)
        {
            UInt64 pid = 0;
            int cid, sid;
            UInt8 level = 0;
            UInt8 heroId = 0;
            UInt32 battlePoint = 0;
            UInt32 support = 0;
            std::string name;
            brd >> cid >> sid >> pid >> heroId >> level >> battlePoint >> support >> name;
            UInt64 ppid = pid;
            if((cid != cfg.channelNum || sid != cfg.serverNo) && (ppid >> 48) == 0)
                ppid = pid | (static_cast<UInt64>(sid) << 48) | (static_cast<UInt64>(cid) << 40);
            PreliminaryPlayer pp;
            pp.id = ppid;
            pp.support = support;
            pp.level = level;
            pp.heroId = heroId;
            pp.battlePoint = battlePoint;
            pp.name = name;
            PreliminaryPlayerListIterator it = _preliminaryPlayers_list[i].insert(_preliminaryPlayers_list[i].end(), pp);
            _preliminaryPlayers[i][ppid] = it;
            _preliminaryPlayers_list_set[i].insert(it);
            if(cid == cfg.channelNum && sid == cfg.serverNo)
            {
                std::unordered_map<UInt64, Player *>::const_iterator it = pm.find(pid);
                if(it == pm.end())
                    continue;

                Player * pl = it->second;
                ArenaPlayer& ap = _players[pl];
                ap.group = 0;
                ap.realName = name;
            }
        }
    }
    for(std::map<Player *, ArenaPlayer>::iterator it = _players.begin(); it != _players.end(); ++ it)
    {
        for(int i = 0; i < 2; ++ i)
        {
            std::vector<BetInfo>& blist = it->second.betList[i][0];
            if(blist.empty())
                continue;

            for(std::vector<BetInfo>::iterator bit = blist.begin(); bit != blist.end(); ++ bit)
            {
                BetInfo& bi = *bit;
                if(bi.round == 1 && bi.group == 0 && bi.state == i)
                {
                    PreliminaryPlayerListIterator pit = _preliminaryPlayers_list[i].begin();
                    if(bi.pos > _preliminaryPlayers_list[i].size())
                        continue;
                    std::advance(pit, bi.pos);
                    if(pit != _preliminaryPlayers_list[i].end())
                    {
                        PreliminaryPlayer& pp = *pit;
                        pp.betMap[it->first] = bi.type;
                    }
                }
            }
        }
    }
}

void Arena::readPlayers(BinaryReader& brd, UInt8 sIdx)
{
    if(!_players.empty() && sIdx == 0)
    {
        _playerCount[0] = 0;
        _players.clear();
    }

    Mutex::ScopedLock lk(globalPlayers.getMutex());
    std::unordered_map<UInt64, Player *>& pm = globalPlayers.getMap();
    UInt32 count;
    brd >> count;
    _playerCount[0] += count;
    for(UInt32 z = 0; z < count; ++ z)
    {
        UInt64 pid = 0;
        int cid, sid;
        UInt8 level = 0;
        UInt8 heroId = 0;
        UInt32 battlePoint = 0;
        UInt32 support = 0;
        std::string name;
        brd >> cid >> sid >> pid >> heroId >> level >> battlePoint >> support >> name;
        UInt64 ppid = pid;
        if((cid != cfg.channelNum || sid != cfg.serverNo) && (ppid >> 48) == 0)
            ppid = pid | (static_cast<UInt64>(sid) << 48) | (static_cast<UInt64>(cid) << 40);
        PreliminaryPlayer pp;
        pp.id = ppid;
        pp.support = support;
        pp.level = level;
        pp.heroId = heroId;
        pp.battlePoint = battlePoint;
        pp.name = name;
        PreliminaryPlayerListIterator it = _preliminaryPlayers_list[0].insert(_preliminaryPlayers_list[0].end(), pp);
        _preliminaryPlayers[0][ppid] = it;
        _preliminaryPlayers_list_set[0].insert(it);
        if(cid == cfg.channelNum && sid == cfg.serverNo)
        {
            std::unordered_map<UInt64, Player *>::const_iterator it = pm.find(pid);
            if(it == pm.end())
                continue;
            Player * pl = it->second;
            if(pl == NULL)
                continue;
            ArenaPlayer& ap = _players[pl];
            ap.group = 0;
            ap.realName = name;
        }
    }
}

void Arena::readHistories(BinaryReader& brd)
{
    Mutex::ScopedLock lk(globalPlayers.getMutex());
    std::unordered_map<UInt64, Player *>& pm = globalPlayers.getMap();
    for(int i = 0; i < 2; ++ i)
    {
        UInt32 cnt = 0;
        brd >> cnt;

        for(int j = 0; j < cnt; ++ j)
        {
            UInt64 pid = 0;
            brd >> pid;

            std::unordered_map<UInt64, Player *>::const_iterator it = pm.find(pid);
            if(it == pm.end())
                continue;
            Player * pl = it->second;
            if(pl == NULL)
                continue;
            ArenaPlayer& ap = _players[pl];
            UInt16 bCount = 0;
            brd >> bCount;
            ap.battles[i].clear();
            ap.battles[i].resize(bCount);
            for(UInt16 p = 0; p < bCount; ++ p)
            {
                PriliminaryBattle pb;
                brd >> pb.won >> pb.type >> pb.otherHeroId >> pb.otherName >> pb.battleTime >> pb.battleId;
                ap.battles[i][p] = pb;
            }
        }
    }
}

void Arena::readElimination(BinaryReader& brd)
{
    UInt8 cnt = 0;
    brd >> cnt;
    for(int i = 0; i < cnt; ++ i)
    {
        for(int j = 0; j < 32; ++ j)
        {
            int cid, sid;
            UInt32 battlePoint = 0;
            UInt32 support = 0;
            brd >> cid >> sid >> _finals[i][j].id >> _finals[i][j].level>> _finals[i][j].heroId >> battlePoint >> support >> _finals[i][j].name;
            UInt64 ppid = _finals[i][j].id;
            if((cfg.channelNum != cid || cfg.serverNo != sid) && (ppid >> 48) == 0)
                _finals[i][j].id = ppid | (static_cast<UInt64>(sid) << 48) | (static_cast<UInt64>(cid) << 40);
            _finalIdx[i][0][j] = j;
            _finals[i][j].battlePoint = battlePoint;
            _finals[i][j].support = support;
        }
        _round = 0;
        UInt8 status;
        brd >> status >> _round;
        if(status != _status)
        {
            _status = status;
            if(_notified)
                _notified = 0;
        }

        int idx = _round;
        const int aIndex[6] = {0, 16, 24, 28, 30, 31};
        for(int j = 0; j < aIndex[idx]; ++ j)
        {
            UInt8 cnt = 0;
            brd >> cnt;
            brd >> _finalBattles[i][j].wonFlag;
            _finalBattles[i][j].battleId.clear();
            _finalBattles[i][j].battleId.resize(cnt);
            for(int k = 0; k < cnt; ++ k)
            {
                UInt32 battleId;
                brd >> battleId;
                _finalBattles[i][j].battleId[k] = battleId;
            }
        }

        calcFinalBet(i);
    }
    if(!_notified)
        _notified = 1;
}

void Arena::calcFinalBet(int i)
{
    const int aIndex[6] = {0, 16, 24, 28, 30, 31};
    UInt32 r = _round;
    if(_status == 0)
        ++ r;
    if(r > 1)
    {
        GET_PROGRESS_NAME(p, _round+1);
        for(UInt32 j = 1; j < r; ++ j)
        {
            int starti = aIndex[j - 1], endi = aIndex[j];
            for(int k = starti; k < endi; ++ k)
            {
                if(_finalBattles[i][k].winner(j-1) == 0)
                {
                    UInt8 nidx = _finalIdx[i][j-1][(k - starti) * 2];
                    _finalIdx[i][j][k - starti] = nidx;
                    if(_status == 0 && j == r - 1)
                    {
                        int pos1 = nidx;
                        int pos2 = _finalIdx[i][j-1][(k - starti) * 2 + 1];
                        if(j > 2)
                        {
                            pos1 = pos1 >> 2;
                            pos2 = pos2 >> 2;
                        }
                        calcBet(_finals[i][nidx], pos1, j+1, i, true, p);
                        calcBet( _finals[i][_finalIdx[i][j-1][(k - starti) * 2 + 1]], pos2, j+1, i, false, p);
                        //_finals[i][nidx].calcBet(true, p);
                        //_finals[i][_finalIdx[i][j-1][(k - starti) * 2 + 1]].calcBet(false, p);
                    }
                }
                else
                {
                    UInt8 nidx = _finalIdx[i][j-1][(k - starti) * 2 + 1];
                    _finalIdx[i][j][k - starti] = nidx;
                    if(_status == 0 && j == r - 1)
                    {
                        int pos1 = nidx;
                        int pos2 = _finalIdx[i][j-1][(k - starti) * 2];
                        if(j > 2)
                        {
                            pos1 = pos1 >> 2;
                            pos2 = pos2 >> 2;
                        }
                        calcBet(_finals[i][nidx], pos1, j+1, i, true, p);
                        calcBet( _finals[i][_finalIdx[i][j-1][(k - starti) * 2]], pos2, j+1, i, false, p);
                        //_finals[i][nidx].calcBet(true, p);
                        //_finals[i][_finalIdx[i][j-1][(k - starti) * 2]].calcBet(false, p);
                    }
                }
            }
        }
    }
    if(_loaded)
    {
        Stream st(REP::ARENAPRILIMINARY);
        st << static_cast<UInt8>(3) << Stream::eos;
        NETWORK()->Broadcast(st);
        if(!_notified)
        {
            if(_status == 0 && _round > 0)
            {
                GET_ARENA_NAME(n);
                GET_PROGRESS_NAME(p, _round + 1);
                SYSMSGV(g, 710 + i);
                SYSMSGV(title, 720, p, g);
                SYSMSGV(title2, 722, p, g);
                if(_round < 5)
                {
                    //for(int i = 0; i < 2; ++ i)
                    {
                        const int aIndex[6] = {0, 16, 24, 28, 30, 31};
                        int starti = aIndex[_round - 1];
                        int endi = aIndex[_round];
                        for(int idx = starti; idx < endi; ++ idx)
                        {
                            EliminationPlayer& ep1 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2]];
                            EliminationPlayer& ep2 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2 + 1]];
                            if(ep1.name.empty() || ep2.name.empty())
                                continue;
                            Player * pl;
                            UInt8 winCount = _finalBattles[i][idx].winCount(_round-1);
                            const UInt8 totalCount[5] = { 3, 3, 5, 5, 7 };
                            const UInt8 winCount_mod[5] = {1, 1, 2, 2, 3};
                            if(winCount > winCount_mod[_round-1])
                            {
                                if(ep1.id != 0 && (pl = globalPlayers[ep1.id]) != NULL)
                                {
                                    SYSMSGV(content, 721, _session, p, winCount, totalCount[_round-1] - winCount);
                                    pl->GetMailBox()->newMail(NULL, 0x01, title, content);
                                }
                                if(ep2.id != 0 && (pl = globalPlayers[ep2.id]) != NULL)
                                {
                                    SYSMSGV(content, 723, _session, p, totalCount[_round-1] - winCount, winCount);
                                    pl->GetMailBox()->newMail(NULL, 0x01, title2, content);
                                }
                            }
                            else
                            {
                                if(ep2.id != 0 && (pl = globalPlayers[ep2.id]) != NULL)
                                {
                                    SYSMSGV(content, 721, _session, p, totalCount[_round-1] - winCount, winCount);
                                    pl->GetMailBox()->newMail(NULL, 0x01, title, content);
                                }
                                if(ep1.id != 0 && (pl = globalPlayers[ep1.id]) != NULL)
                                {
                                    SYSMSGV(content, 723, _session, p, winCount, totalCount[_round-1] - winCount);
                                    pl->GetMailBox()->newMail(NULL, 0x01, title2, content);
                                }
                            }
                        }
                    }
                }
                else
                {
                    const int award[2][6][2] = {
                    {
                        {9063, 6000},
                        {9062, 7000},
                        {9061, 8000},
                        {9060, 10000},
                        {9059, 12000},
                        {9058, 15000}
                    },
                    {
                        {9066, 1000},
                        {9066, 1500},
                        {9065, 2000},
                        {9065, 3000},
                        {9064, 4000},
                        {9063, 5000}
                    }
                    };
                    //for(int i = 0; i < 2; ++ i)
                    {
                        int idx = 0;
                        const int aIndex[6] = {0, 16, 24, 28, 30, 31};
                        UInt8 nidx = 0;
                        UInt8 nidx2 = 0xFF;
                        for(int j = 1; j < 6; ++ j)
                        {
                            SYSMSGV(g, 710 + i);
                            SYSMSGV(title, 724, g);
                            int starti = aIndex[j - 1];
                            int endi = aIndex[j];
                            for(idx = starti; idx < endi; ++ idx)
                            {
                                if(_finalBattles[i][idx].winner(j-1) == 0)
                                {
                                    nidx = _finalIdx[i][j-1][(idx - starti) * 2 + 1];
                                    if(j == 5)
                                        nidx2 = _finalIdx[i][j-1][(idx - starti) * 2];
                                }
                                else
                                {
                                    nidx = _finalIdx[i][j-1][(idx - starti) * 2];
                                    if(j == 5)
                                        nidx2 = _finalIdx[i][j-1][(idx - starti) * 2 + 1];
                                }
                                EliminationPlayer& ep = _finals[i][nidx];
                                Player * pl = globalPlayers[ep.id];
                                if(pl != NULL)
                                {
                                    //pl->AddVar(VAR_MONEY_ARENA, award[i][j-1][1]);

                                    GameMsgHdr hdr(0x251, pl->getThreadId(), pl, 4);
                                    UInt32 data = award[i][j-1][1];
                                    GLOBAL().PushMsg(hdr, &data);

                                    SYSMSGV(content, 725, _session, award[i][j-1][1]);
                                    Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                                    if(mail != NULL)
                                    {
                                        std::string strItems;
                                        strItems += Itoa(award[i][j-1][0]);
                                        strItems += ",";
                                        strItems += Itoa(1);
                                        strItems += "|";
                                        mailPackageManager.push(mail->id, award[i][j-1][0], 1, true);
                                        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
                                    }
                                }
                                if(nidx2 != (UInt8)(0xFF))
                                {
                                    EliminationPlayer& ep = _finals[i][nidx2];
                                    Player * pl = globalPlayers[ep.id];
                                    if(pl == NULL)
                                        continue;

                                    //pl->AddVar(VAR_MONEY_ARENA, award[i][j][1]);

                                    GameMsgHdr hdr(0x251, pl->getThreadId(), pl, 4);
                                    UInt32 data = award[i][j][1];
                                    GLOBAL().PushMsg(hdr, &data);

                                    SYSMSGV(content, 725, _session, award[i][j][1]);
                                    Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                                    if(mail != NULL)
                                    {
                                        std::string strItems;
                                        strItems += Itoa(award[i][j][0]);
                                        strItems += ",";
                                        strItems += Itoa(1);
                                        strItems += "|";
                                        mailPackageManager.push(mail->id, award[i][j][0], 1, true);
                                        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if(_status == 0 && _progress != 3)
        {
            {
                for(int j = 0; j < 32; ++ j)
                {
                    _finals[i][j].resetBet();
                }
            }
            for(std::map<Player *, ArenaPlayer>::iterator it = _players.begin(); it != _players.end(); ++ it)
            {
                for(int sidx = 2; sidx < 7; ++ sidx)
                {
                    std::vector<BetInfo>& blist = it->second.betList[sidx][i];
                    if(blist.empty())
                        continue;
                    int cnt = blist.size();
                    for(int j = 0; j < cnt; ++ j)
                    {
                        BetInfo& bi = blist[j];
                        if(bi.group == i + 1 && bi.recieved == 0)
                        {
                            UInt8 pos2 = bi.pos;
                            switch(bi.state)
                            {
                            case 4:
                            case 5:
                            case 6:
                                pos2 = _finalIdx[i][2][bi.pos];
                                break;
                            }

                            _finals[i][pos2].betMap[it->first] = bi.type;
                        }
                    }
                }
            }
        }
    }
    else
    {
        UInt8 r = _round;
        if(_status > 0 && r > 0)
        {
            -- r;
        }
        for(std::map<Player *, ArenaPlayer>::iterator it = _players.begin(); it != _players.end(); ++ it)
        {
            for(int sidx = 2; sidx < 7; ++ sidx)
            {
                std::vector<BetInfo>& blist = it->second.betList[sidx][i];
                if(blist.empty())
                    continue;
                int cnt = blist.size();
                for(int j = 0; j < cnt; ++ j)
                {
                    BetInfo& bi = blist[j];
                    if(bi.group == i + 1 && bi.recieved == 0)
                    {
                        UInt8 pos2 = bi.pos;
                        switch(bi.state)
                        {
                        case 4:
                        case 5:
                        case 6:
                            pos2 = _finalIdx[i][2][bi.pos];
                            break;
                        }

                        _finals[i][pos2].betMap[it->first] = bi.type;
                    }
                }
            }
        }
    }
}

void Arena::sendActive(Player* pl)
{
    Stream st(REP::SERVER_ARENA_OP);
    UInt8 flag = static_cast<UInt8>(active() ? 1 : 0);
    if(_progress == 12)
        flag |= 0x10;
    st << static_cast<UInt8>(0) << flag << Stream::eos;
    pl->send(st);
}

void Arena::sendStatus(Player* pl)
{
	Stream st(REP::SERVER_ARENA_INFO);
    st << static_cast<UInt8>(0);
    UInt8 find = 0;
    UInt8 progress = 0;
    // 0 - 可报名阶段
    // 1 - 入围赛投注阶段
    // 2 - 入围赛战斗阶段
    // 3 - 32强投注阶段
    // 4 - 32强战斗阶段
    // 5 - 8强投注阶段1
    // 6 - 8强战斗阶段1
    // 7 - 8强投注阶段2
    // 8 - 8强战斗阶段2
    // 9 - 4强投注阶段
    // 10 - 4强战斗阶段
    // 11 - 半决赛投注阶段
    // 12 - 半决赛战斗阶段
    // 13 - 决赛阶段投注阶段
    // 14 - 决赛阶段战斗阶段
    // 15 - 决赛阶段结束（不可报名
    const UInt8 state[] = {0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 15, 16};
    progress = state[_progress];
    if(_progress > 3 && _progress < 8 && _status == 0)
        progress -= 1;
    PreliminaryPlayerListMap::iterator pit0 = _preliminaryPlayers[0].find(pl->getId());
    if(pit0 != _preliminaryPlayers[0].end())
        find = 1;
    UInt8 entered = 0;
    PreliminaryPlayerListMap::iterator pit1 = _preliminaryPlayers[1].find(pl->getId());
    if(pit1 != _preliminaryPlayers[1].end())
        entered = 1;
    st << _session << progress << find << entered << Stream::eos;
    pl->send(st);
}

void Arena::sendEnter(Player* pl)
{
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;

	Stream st(REP::SERVER_ARENA_INFO);
    st << static_cast<UInt8>(1);
    UInt8 find = 0;
    std::map<Player *, ArenaPlayer>::iterator it = _players.find(pl);
    if(it != _players.end())
        find = 1;
    st << find << leftTime << _playerCount[0] << Stream::eos;

    pl->send(st);
}

void Arena::sendPreliminary(Player* player, UInt8 type, UInt8 flag, UInt16 start, UInt8 len)
{
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;
    if(type > 1)
        return;

	Stream st(REP::SERVER_ARENA_INFO);
    st << static_cast<UInt8>(type+2);
    if(flag == 0)
    {
        PreliminaryPlayerListMap::iterator it = _preliminaryPlayers[type].find(player->getId());
        if(it == _preliminaryPlayers[type].end())
            return;
    }

    st << flag;
    if(flag == 0)
    {
        st << leftTime;
        ArenaPlayer& ap = _players[player];
        UInt8 cnt = static_cast<UInt8>(ap.battles[type].size());
        size_t offset = st.size();
        st << cnt;
        UInt8 cnt2 = 0;
        for(int i = 0; i < cnt; ++ i)
        {
            if(ap.battles[type][i].battleId == 0)
                continue;
            ++ cnt2;
            const UInt8 won_mod[] = {1, 0, 1, 0, 1};
            UInt8 won = won_mod[ap.battles[type][i].won];
            UInt8 flag =  won;
            st << flag;
            st << ap.battles[type][i].otherHeroId << ap.battles[type][i].battleId << ap.battles[type][i].otherName;
        }
        st.data<UInt8>(offset) = static_cast<UInt8>(cnt2);
    }
    else
    {
        std::map<Player*, ArenaPlayer>::iterator ait = _players.find(player);
        if(ait == _players.end())
        {
            st << static_cast<UInt8>(1);
        }
        else
        {
            ArenaPlayer& ap = ait->second;
            std::vector<BetInfo>& blist = ap.betList[type][0];
            UInt8 cnt = blist.size();
            if(cnt > 1)
                cnt = 1;
            st << static_cast<UInt8>(1 - cnt);
        }
        UInt16 winNum = 0;
        if(type == 0)
            winNum = player->GetVar(VAR_MONEY_ARENA2);
        else
            winNum = player->GetVar(VAR_MONEY_ARENA3);
        UInt8 premNum = 0;
        UInt16 totalPreNum = _preliminaryPlayers_list_set[type].size();
        st << winNum << totalPreNum << start;
        size_t offset = st.size();
        st << premNum;
        PreliminaryPlayersSet::iterator setIt = _preliminaryPlayers_list_set[type].begin();
        int pos = start;
        if(pos > _preliminaryPlayers_list_set[type].size())
            pos = _preliminaryPlayers_list_set[type].size();
        std::advance(setIt, pos);
        for(int i = 0; setIt != _preliminaryPlayers_list_set[type].end() && i < len; ++ setIt)
        {
            ++ premNum;
            ++ i;
            PreliminaryPlayerListIterator pit = *setIt;
            PreliminaryPlayer& pp = *pit;
            UInt8 fSupport = 0;
            std::map<Player *, UInt8>::iterator it = pp.betMap.find(player);
            if(it != pp.betMap.end())
                fSupport = it->second + 1;
            st << pp.id << pp.battlePoint << static_cast<UInt16>(pp.support) << fSupport << pp.heroId << pp.level << pp.name;
        }
        st.data<UInt8>(offset) = static_cast<UInt8>(premNum);
    }

    st << Stream::eos;
    player->send(st);
}

void Arena::sendElimination( Player * player, UInt8 type, UInt8 group )
{
	bool resultOnly = (group & 0x80) > 0;
    UInt8 r = _round;
    switch(type)
    {
    case 1:
        if(r < 2)
            return;
        break;
    case 2:
        if(r < 3)
            return;
        break;
    case 3:
        if(r < 4)
            return;
        break;
    }
    if(group > 2 || group < 1)
        return;
	if(_progress < 3 || _progress == 8 || _progress == 9)
		return;
	Stream st(REP::SERVER_ARENA_INFO);
    st << static_cast<UInt8>(type + 4);
	//Stream st(REP::SERVER_ARENA_ELIM);
	st << group;
    int gIdx = group - 1;
	std::map<Player *, ArenaPlayer>::iterator iter = _players.find(player);
	const int aIndex[6] = {0, 16, 24, 28, 30, 31};
	if(iter == _players.end())
		st << static_cast<UInt8>(0);
	else
	{
        size_t offset = st.size();
        UInt8 cnt = 0;
        int states = 1;
        int state = 2;
        switch(type)
        {
        case 0:
            ++ states;
            break;
        case 3:
            ++ states;
        case 2:
            ++ states;
        case 1:
            state = 4;
            break;
        }
        st << cnt;
        for(int i = 0; i < states; ++ i)
        {
            std::vector<BetInfo>& blist = iter->second.betList[state+i][gIdx];
            for(std::vector<BetInfo>::iterator it = blist.begin(); it != blist.end(); ++ it)
            {
                if(it->group != group)
                    continue;
                ++ cnt;
                UInt8 pos = it->pos;
                UInt8 g_pos = (pos << 3) | it->round;
                st << g_pos << it->type;
            }
        }
        st.data<UInt8>(offset) = cnt;
	}
	if(!resultOnly)
	{
        if(type < 1)
        {
            for(int i = 0; i < 32; ++ i)
            {
                st << _finals[gIdx][i].heroId << _finals[gIdx][i].level << _finals[gIdx][i].battlePoint << static_cast<UInt16>(_finals[gIdx][i].support) << _finals[gIdx][i].name;
            }
        }
        else
        {
            for(int i = 0; i < 8; ++ i)
            {
                UInt8 pos = _finalIdx[gIdx][2][i];
                st << _finals[gIdx][pos].heroId << _finals[gIdx][pos].level << _finals[gIdx][pos].battlePoint << static_cast<UInt16>(_finals[gIdx][pos].support) << _finals[gIdx][pos].name;
            }
        }
	}

    int istart = 0;
    switch(type)
    {
    case 0:
        if(r > 2)
            r = 2;
        break;
    case 1:
    case 2:
    case 3:
        istart = aIndex[2];
        r = type + 2;
        break;
    }

    st << static_cast<UInt8>(r);
	for(int j = istart; j < aIndex[r]; ++ j)
	{
        UInt8 cnt = static_cast<UInt8>(_finalBattles[gIdx][j].battleId.size());
        st << cnt << _finalBattles[gIdx][j].wonFlag;
        for(int k = 0; k < cnt; ++ k)
            st << _finalBattles[gIdx][j].battleId[k];
	}
	st << Stream::eos;
	player->send(st);
}

void Arena::updateSuport(UInt8 type, UInt8 flag, UInt16 pos)
{
    if(flag == 1)
    {
        if(type == 1)
        {
            PreliminaryPlayerListIterator it = _preliminaryPlayers_list[0].begin();
            int pos2 = pos;
            if(pos2 > _preliminaryPlayers_list[0].size())
                pos2 = _preliminaryPlayers_list[0].size();
            std::advance(it, pos2);
            if(it != _preliminaryPlayers_list[0].end())
            {
                PreliminaryPlayer& pp = *it;
                ++ pp.support;
            }
        }
        else
        {
            PreliminaryPlayerListIterator it = _preliminaryPlayers_list[1].begin();
            int pos2 = pos;
            if(pos2 > _preliminaryPlayers_list[1].size())
                pos2 = _preliminaryPlayers_list[1].size();
            std::advance(it, pos2);
            if(it != _preliminaryPlayers_list[1].end())
            {
                PreliminaryPlayer& pp = *it;
                ++ pp.support;
            }
        }
    }
    else if(flag == 2)
    {
        UInt8 fidx = pos >> _round;
        if(type == 2)
            ++ _finals[0][_finalIdx[0][_round][fidx]].support;
        else
            ++ _finals[1][_finalIdx[1][_round][fidx]].support;
    }
}

void Arena::calcBet(PreliminaryPlayer& pp, UInt16 pos, UInt8 state, UInt8 group, bool won, const char * t)
{
	for(std::map<Player *, UInt8>::iterator it = pp.betMap.begin(); it != pp.betMap.end(); ++ it)
	{
        Player* player = it->first;
        std::map<Player *, ArenaPlayer>::iterator iter = _players.find(player);
		std::vector<BetInfo>& blist = iter->second.betList[state][group];

        bool recieved = true;
		for(std::vector<BetInfo>::iterator bit = blist.begin(); bit != blist.end(); ++ bit)
		{
            if(bit->pos == pos)
            {
                if(bit->recieved == 0)
                {
                    recieved = false;
                    bit->recieved = 1;
                    DB1().PushUpdateData("REPLACE INTO `arena_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u, %u)", player->getId(), bit->round, bit->state, bit->group, bit->recieved, bit->pos, bit->type);
                }
                break;
            }
        }
        if(recieved)
            continue;

		if(won)
		{
            SYSMSGV(title, 730, t, pp.name.c_str());

            UInt8 rew = it->second == 0 ? 100 : 50;
            //player->AddVar(VAR_MONEY_ARENA, rew);

            GameMsgHdr hdr(0x251, player->getThreadId(), player, 4);
            UInt32 data = rew;
            GLOBAL().PushMsg(hdr, &data);

            if(state == 0)
                player->AddVar(VAR_MONEY_ARENA2, rew);
            if(state == 1)
                it->first->AddVar(VAR_MONEY_ARENA3, rew);
            SYSMSGV(content, 731, _session, t, pp.name.c_str(), rew);
            it->first->GetMailBox()->newMail(NULL, 0x01, title, content);
		}
		else
		{
            SYSMSGV(title, 732, t, pp.name.c_str());
			SYSMSGV(content, 733, _session, t, pp.name.c_str());
			it->first->GetMailBox()->newMail(NULL, 0x01, title, content);
		}
	}
}

void Arena::updateBattlePoint(BinaryReader& brd)
{
    UInt32 cid = 0;
    UInt32 sid = 0;
    UInt64 pid = 0;
    UInt32 battlePoint = 0;
    brd >> cid >> sid >> pid >> battlePoint;
    UInt64 ppid = pid;
    if((cid != cfg.channelNum || sid != cfg.serverNo) && (ppid >> 48) == 0)
        ppid = pid | (static_cast<UInt64>(sid) << 48) | (static_cast<UInt64>(cid) << 40);

    PreliminaryPlayerListMap::iterator pit0 = _preliminaryPlayers[0].find(ppid);
    if(pit0 != _preliminaryPlayers[0].end())
    {
        PreliminaryPlayerListIterator ppit = pit0->second;
        PreliminaryPlayer& pp = *ppit;
        _preliminaryPlayers_list_set[0].erase(ppit);
        pp.battlePoint = battlePoint;
        _preliminaryPlayers_list_set[0].insert(ppit);
    }
    PreliminaryPlayerListMap::iterator pit1 = _preliminaryPlayers[1].find(ppid);
    if(pit1 != _preliminaryPlayers[1].end())
    {
        PreliminaryPlayerListIterator ppit = pit1->second;
        PreliminaryPlayer& pp = *ppit;
        _preliminaryPlayers_list_set[1].erase(ppit);
        pp.battlePoint = battlePoint;
        _preliminaryPlayers_list_set[1].insert(ppit);
    }
    for(int i = 0; i < 2; ++ i)
    {
        for(int j = 0; j < 32; ++ j)
        {
            if(_finals[i][j].id == ppid)
            {
                _finals[i][j].battlePoint = battlePoint;
            }
        }
    }
}

}

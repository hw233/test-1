#include "Config.h"
#include "Arena.h"
#include "Player.h"
#include "Fighter.h"
#include "Item.h"
#include "Mail.h"
#include "Server/Cfg.h"
#include "Server/SysMsg.h"
#include "MsgID.h"

namespace GObject
{

#define GET_ARENA_NAME(n) char n[1024]; if(_session & 0x8000) { SysMsgItem * mi = globalSysMsg[780]; if(mi != NULL) mi->get(n); else n[0] = 0; } else { strcpy(n, cfg.slugName.c_str()); }
#define GET_PROGRESS_NAME(n, p) char n[1024]; { SysMsgItem * mi = globalSysMsg[781 + p]; if(mi != NULL) mi->get(n); else n[0] = 0; }

void EliminationPlayer::calcBet(bool won, const char * t)
{
	bool isWinner = lastRank == 6;
	for(std::map<Player *, UInt8>::iterator it = betMap.begin(); it != betMap.end(); ++ it)
	{
		if(won)
		{
			if(isWinner)
			{
				SYSMSGV(title, 732, t);
				UInt8 rew = it->second * 3 / 2;
				SYSMSGV(content, 733, t, name.c_str(), it->second, rew);
				Mail * pMail = it->first->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
				if (pMail != NULL)
					mailPackageManager.push(pMail->id, MailPackage::Tael, rew);
			}
			else
			{
				SYSMSGV(title, 730, t);
				UInt8 rew = it->second * 2;
				SYSMSGV(content, 731, t, name.c_str(), it->second, rew);
				Mail * pMail = it->first->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
				if (pMail != NULL)
					mailPackageManager.push(pMail->id, MailPackage::Tael, rew);
			}
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

UInt8 EliminationBattle::winner()
{
	UInt8 w = 0;
	const UInt8 oflag[3] = {1, 2, 4};
	for(int i = 0; i < 3; ++ i)
		if(wonFlag & oflag[i])
			++ w;
	if(w > 1)
		return 1;
	return 0;
}

UInt8 EliminationBattle::winCount()
{
	UInt8 w = 0;
	const UInt8 oflag[3] = {1, 2, 4};
	for(int i = 0; i < 3; ++ i)
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
	Stream st(REP::ENTER_ARENA, 0xEF);
	st << player->getId() << player->getName() << static_cast<UInt8>(player->getTitle());
	appendLineup(st, player);
	st << Stream::eos;
	NETWORK()->SendToArena(st);
}

void Arena::commitLineup( Player * player, int sid )
{
	Stream st(REP::LINEUP_CHANGE, 0xEF);
	st << player->getId();
	appendLineup(st, player);
	st << Stream::eos;
	NETWORK()->SendMsgToClient(sid, st);
}

UInt8 Arena::bet( Player * player, UInt8 group, UInt8 pos, UInt8 tael )
{
	if(group > 2 || _progress != 4 || _status > 0 || _round > 4 || pos > 31)
		return 1;
	UInt8 fidx = pos >> _round;
	if(_finalIdx[group][_round][fidx] != pos || _finals[group][pos].name.empty() || _finals[group][pos].betMap.find(player) != _finals[group][pos].betMap.end())
		return 1;
	_finals[group][pos].betMap[player] = tael;
	BetInfo binfo;
	binfo.round = _round;
	binfo.group = group;
	binfo.pos = pos;
	binfo.tael = tael;
	_players[player].betList.push_back(binfo);
	DB().PushUpdateData("REPLACE INTO `arena_bet`(`id`, `round`, `group`, `pos`, `tael`) VALUES(%"I64_FMT"u, %u, %u, %u, %u)", player->getId(), binfo.round, binfo.group, binfo.pos, binfo.tael);
	GameMsgHdr hdr(0x230, player->getThreadId(), player, 2);
	UInt16 data = tael;
	GLOBAL().PushMsg(hdr, &data);
	return 0;
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
			++ c;
			st << pdata.pos << static_cast<UInt16>(pdata.fid);
			Fighter * fgt = pdata.fighter;
            // XXX: 
			st << fgt->getLevel() << static_cast<UInt8>(fgt->getPotential() * 100 + 0.5f);
            fgt->getAllUpSkillAndLevel(st);
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
	st << static_cast<UInt16>(equip->GetItemType().getId()) << ied.enchant;
	ied.extraAttr2.appendAttrToStream(st);
	st << ied.sockets;
	for(UInt8 i = 0; i < ied.sockets; ++ i)
		st << ied.gems[i];
}

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
				Player * pl = pm[pid];
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
				if(cfg.channelNum != cid || cfg.serverNum != sid)
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
			Stream st(0xED);
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
	st << _session << _winnerColor[0] << _winner[0] << _winnerColor[1] << _winner[1] << _winnerColor[2] << _winner[2];
	st << _progress;
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;
	switch(_progress)
	{
	case 0:
		st << leftTime;
		break;
	case 1:
	case 2:
		st << leftTime << _playerCount[0] << _playerCount[1] << _playerCount[2];
		break;
	case 3:
		{
			st << leftTime;
			if(it != _players.end())
			{
				st << static_cast<UInt16>(it->second.battles.size());
				for(std::vector<PriliminaryBattle>::iterator iter = it->second.battles.begin(); iter != it->second.battles.end(); ++ iter)
				{
					st << iter->won << iter->otherColor << iter->otherName << iter->battleTime << iter->battleId;
				}
			}
			else
				st << static_cast<UInt16>(0);
		}
		break;
	case 4:
		st << _status << leftTime;
		break;
	}
	st << Stream::eos;
	player->send(st);
}

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
			if(pl.lastRank == 6)
				st << static_cast<UInt8>(0x40 | pl.color);
			else
				st << pl.color;
			st << pl.level << pl.name << it->tael;
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
			st << _finals[g][i].color << _finals[g][i].level << _finals[g][i].lastRank << _finals[g][i].name;
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

void Arena::push( Player * player, UInt8 group, const std::string& rname )
{
	ArenaPlayer& ap = _players[player];
	if(ap.group == 0xFF && group < 3)
		++ _playerCount[group];
	ap.group = group;
	ap.realName = rname;
}

void Arena::pushPriliminary( Player * player, UInt8 won, UInt8 color, const std::string& name, UInt32 btime, UInt32 bid, float rate )
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
			pb.otherColor = 0;
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
		pb.otherColor = color;
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

void Arena::pushBetFromDB( Player * player, UInt8 round, UInt8 group, UInt8 pos, UInt8 tael )
{
	BetInfo binfo;
	binfo.round = round;
	binfo.group = group;
	binfo.pos = pos;
	binfo.tael = tael;
	_players[player].betList.push_back(binfo);
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

}

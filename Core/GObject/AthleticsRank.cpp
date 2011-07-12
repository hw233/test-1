#include "Config.h"
#include "Player.h"
#include "AthleticsRank.h"
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "Common/Itoa.h"
#include "Server/OidGenerator.h"
#include "GData/GDataManager.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Mail.h"
#include "GData/ItemType.h"
#include "Package.h"
#include "GObject/Country.h"
#include "Script/GameActionLua.h"
#include "Script/WorldScript.h"
#include "SpecialAward.h"
#include "Clan.h"
#include "SpecialAward.h"

namespace GObject
{

AthleticsRank gAthleticsRank;


AthleticsRank::AthleticsRank()
{
	memset(_maxRank, 0x00, sizeof(_maxRank));
}

AthleticsRank::~AthleticsRank()
{
	for (UInt8 i = 0; i < 2; ++i)
	{
		Rank offset = _athleticses[i].begin();
		for (; offset != _athleticses[i].end(); ++offset)
		{
			SAFE_DELETE(*offset);
		}
		_athleticses[i].clear();
	}
}

UInt8 AthleticsRank::updateChallengeNum(UInt8 oNum, UInt32 oTime)
{
	if (TimeUtil::SharpDay(0, oTime) != WORLD().ThisDay())
		return 0;
	return oNum;
}

void AthleticsRank::getRankUpNeighbour(UInt8 row, Rank me, Rank& up)
{
	if (frontRankPos(row, me))
		up = getRankEnd(row);
	else
	{
		up = me;
		std::advance(up, -1);
	}
}

void AthleticsRank::getRankDownNeighbour(UInt8 row, Rank me, Rank& down)
{
	if (backRankPos(row, me))
		down == getRankEnd(row);
	else
	{
		down = me;
		std::advance(down, 1);
	}
}

void AthleticsRank::getRankNeighbour(UInt8 row, Rank me, Rank& up, Rank& down)
{
	if (frontRankPos(row, me))
		up = getRankEnd(row);
	else
	{
		up = me;
		std::advance(up, -1);
	}
	if (backRankPos(row, me))
		down = getRankEnd(row);
	else
	{
		down = me;
		std::advance(down, 1);
	}
}

void AthleticsRank::updateBatchRanker(UInt8 row, Rank rank1, Rank rank2)
{
	Rank begin, end;
	begin = rank1;
	end = ++rank2;
	for (; begin != end; ++begin)
	{
		++ (*begin)->rank;
		DB().PushUpdateData("UPDATE `athletics_rank` SET `rank` = %u WHERE `ranker` = %"I64_FMT"u", (*begin)->rank, (*begin)->ranker->getId());
	}
}

void AthleticsRank::addAthleticsFromDB(UInt8 row, AthleticsRankData * data)
{
	_maxRank[row] = std::max(_maxRank[row], data->rank);
	_ranks[row][data->ranker] = _athleticses[row].insert(_athleticses[row].end(), data);
}

bool AthleticsRank::enterAthleticsReq(Player * player ,UInt8 lev)
{
	UInt8 row = 0xFF;
	if(lev < 30)
		return false;
	else if(lev >= 30 && lev <= 50)
		row = 0;
	else
		row =1;
	if(row == 0xFF)
		return false;

	RankList::iterator found = _ranks[row].find(player);
	if (found != _ranks[row].end())
		return false;
	AthleticsRankData * data = NULL;
	if (row == 1)
	{
		RankList::iterator found = _ranks[0].find(player);
		if (found != _ranks[0].end())
		{
			data = *(found->second);
			data->row = row;
			_athleticses[0].erase(found->second);
			_ranks[0].erase(player);
			DB().PushUpdateData("DELETE FROM `athletics_rank` WHERE `ranker` = %"I64_FMT"u", data->ranker->getId());
			_ranks[1][player] = _athleticses[1].insert(_athleticses[1].end(), data);;
			data->rank = ++_maxRank[1];
			data->maxrank = _athleticses[1].size();
			DB().PushUpdateData("INSERT INTO `athletics_rank` VALUES(%u, %u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u)", row, data->rank, data->ranker->getId(), data->maxrank, data->challengenum, data->challengetime, data->boxcolor, data->awardType, data->awardCount, data->boxflushtime, data->winstreak);
		}
	}
	if (data == NULL)
	{
		data = new(std::nothrow) AthleticsRankData;
		if (data == NULL)
			return false;
		data->row = row;
		data->rank = ++_maxRank[row];
		data->ranker = player;
		data->challengenum = 0;
		data->challengetime = 0;
		data->winstreak = 0;
		Rank rank = _athleticses[row].insert(_athleticses[row].end(), data);
		_ranks[row][player] = rank;
		data->maxrank = _athleticses[row].size();
		BuildNewBox(rank);
		DB().PushUpdateData("INSERT INTO `athletics_rank` VALUES(%u, %u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u)", row, data->rank, data->ranker->getId(), data->maxrank, data->challengenum, data->challengetime, data->boxcolor, data->awardType, data->awardCount, data->boxflushtime, data->winstreak);
		GameMsgHdr hdr(0x216, player->getThreadId(), player, 0);
		GLOBAL().PushMsg(hdr, NULL);
	}

	GameMsgHdr hdr(0x219, player->getThreadId(), player, sizeof(Player *));
	GLOBAL().PushMsg(hdr, &row);
	return true;
}

void AthleticsRank::BuildNewBox(Rank &it_rank)// UInt8 row, UInt16 rank, UInt8 lvl)
{
	//row type0:银币 1;礼券 color0:绿色1:蓝色2:紫色 rank_sep
	const UInt16 boxcount[2][2][4][6] =
	{
		{
			{{35}, {70}, {140}, {210}}, 
			{{15}, {30}, {60},  {90}}
		},

		{
			{{40, 45, 50, 60, 80, 100}, {80, 90, 100, 120, 160, 200}, {160, 180, 200, 240, 320, 400}, {240, 270, 300, 360, 480, 600}}, 
			{{17, 20, 22, 25, 27, 30},  {35, 40, 45, 50, 55, 60},     {70, 80, 90,100, 110, 120},     {110, 120, 135, 150, 165, 180}}
		}
	};
	const UInt16 count_base[3][16] = {{60, 80, 100, 120, 150, 200, 250, 300, 350, 400, 450, 500, 600, 700, 800, 900}, {50, 75, 100, 120, 140, 160, 180, 200, 250, 300, 350, 400, 400, 400, 400, 400}, {8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 30, 30, 30, 30}};//type 0:exp 1:coin exp_sep 0:30-40
	const UInt8  color_rate[] = {5, 10, 18, 25};
	const UInt8  rank_rate[] = {50, 55, 60, 65, 75, 100};

	UInt8 type,color, rank_sep, lvl_sep = 0;

	Player * ranker = (*it_rank)->ranker;
	UInt8 lvl = ranker->GetLev();
	if(lvl < 30 || lvl > LEVEL_MAX)
		return;
	UInt8 row = (*it_rank)->row;
	if(row != 0 && row != 1)
		return;//error
	UInt16 rank = getRankPos(row, it_rank);

	UInt32 preBoxTime = (*it_rank)->boxflushtime;
	UInt32 now = TimeUtil::Now();
	UInt32 time_stay = now - preBoxTime;
	
	UInt32 coupon_rate = 10;	//10:礼券宝箱 50:经验宝箱 15: 铜币宝箱 15:银币宝箱 10:战功
	UInt32 exp_rate = 60;
	UInt32 coin_rate = 75;
	UInt32 tael_rate = 90;
	
	if(time_stay < 3600)
		coupon_rate -= coupon_rate * (3600 - time_stay) / 3600 ;
	UInt32 r_type = uRand(100);
	if(r_type < coupon_rate)
		type = 3;
	else if(r_type < exp_rate)
		type = 0;
	else if(r_type < coin_rate)
		type = 1;
	else if(r_type < tael_rate)
		type = 2;
	else
		type = 4;

	UInt32 gre_rate = 15;
	UInt32 blu_rate = 85;
	UInt32 pur_rate = 95;// 15:绿色 70:蓝色 10:紫色 5:橙色
	if(row == 1)
	{
		if(rank == 1)// 0:绿色 0:蓝色 75:紫色 25:橙色
		{
			gre_rate = 0;
			blu_rate = 0;			
			pur_rate = 75;
		}
		else if(rank < 11)// 0:绿色 65:蓝色 25:紫色 10:橙色
		{
			gre_rate = 0;
			blu_rate = 65;	
			pur_rate = 90;
		}
	}


	UInt32 r_color = uRand(100);
	if(r_color < gre_rate)
		color = 0;
	else if(r_color < blu_rate)
		color = 1;
	else if(r_color < pur_rate)
		color = 2;
	else
		color = 3;
	switch(type)
	{
		case 0:
		case 1:
		case 4:
			{
				if(rank > 100)
					rank_sep = 0;
				else if(rank > 50)
					rank_sep = 1;
				else if(rank > 20)
					rank_sep = 2;
				else if(rank > 6)
					rank_sep = 3;
				else if(rank > 2)
					rank_sep = 4;
				else
					rank_sep = 5;
				const UInt8 lvlArray[]= {40, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120};
				for(UInt8 i = 0; i < 16; i ++)
				{
					if(lvl <= lvlArray[i])
					{
						lvl_sep = i;
						break;
					}
				}
			}
			break;		
		case 2:
		case 3:
			{
				if(rank > 500)
					rank_sep = 0;
				else if(rank > 100)
					rank_sep = 1;
				else if(rank > 50)
					rank_sep = 2;
				else if(rank > 10)
					rank_sep = 3;
				else if(rank > 2)
					rank_sep = 4;
				else
					rank_sep = 5;
			}
			break;
	}
	
	(*it_rank)->awardType = type;
	(*it_rank)->boxcolor = color;
	(*it_rank)->boxflushtime = now;	
	if(type == 2 || type == 3)
	{
		if(row == 0)
			(*it_rank)->awardCount = boxcount[0][type - 2][color][0];
		else
			(*it_rank)->awardCount = boxcount[1][type - 2][color][rank_sep];
	}
	else if(type < 2)
	{
		(*it_rank)->awardCount = static_cast<UInt32>(count_base[type][lvl_sep]) * static_cast<UInt32>(color_rate[color]) * static_cast<UInt32>(rank_rate[rank_sep]);
	}
	else
	{
		(*it_rank)->awardCount = static_cast<UInt32>(count_base[2][lvl_sep]) * static_cast<UInt32>(color_rate[color]) * static_cast<UInt32>(rank_rate[rank_sep]) / 100;
	}	
}


void AthleticsRank::requestAthleticsList(Player * player)
{
	UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return ;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
	{
		ERROR_LOG("Cannot find the athletics player[%"I64_FMT"u][%s]", player->getId(), player->getName().c_str());
		enterAthleticsReq(player, player->GetLev());
		return ;
	}
	Rank rank = found->second;
	UInt16 rankpos = getRankPos(row, rank);
	Rank start, end;
	if (rankpos > 15)
	{
		start = end = rank;
		std::advance(start, -14);
	}
	else
	{
		start = end = getRankBegin(row);
		UInt16 ranksize = getRankSize(row);
		if (ranksize > 14)
			std::advance(end, 14);
		else
			std::advance(end, ranksize-1);
	}
	Stream st(0xD0);
	UInt32 now = TimeUtil::Now();
	//UInt32 endTime = (*rank)->challengetime + ATHLETICS_BUFF_TIME;
	//
	//if(player->getVipLevel() >= 2)
	//	endTime=(*rank)->challengetime+5 * 60;
	//else if(player->getVipLevel() == 1)
	//	endTime=(*rank)->challengetime+7 * 60;
	
	(*rank)->challengenum = updateChallengeNum((*rank)->challengenum, (*rank)->challengetime);
	st << static_cast<UInt16>(std::distance(start, end)+1) << (*rank)->challengenum << static_cast<UInt16>(player->getBuffLeft(PLAYER_BUFF_ATHLETICS)) << (*rank)->maxrank << static_cast<UInt32>(getRankPos(row, start));
	++end;

	for (Rank offset = start; offset != end; ++offset)
	{
		updateBoxTimeoutAward(offset, row, now);
		UInt16 tmout = GetOutTimebyColor((*offset)->boxcolor);
		UInt32 endTime = (*offset)->boxflushtime + static_cast<UInt32>(tmout);
		Clan * clan = (*offset)->ranker->getClan();
		st << (*offset)->ranker->getName() << (clan == NULL ? "" : clan->getName()) << (*offset)->ranker->getCountry() << (*offset)->ranker->GetLev() << (*offset)->boxcolor << (*offset)->awardType << (*offset)->awardCount << static_cast<UInt16>(now >= endTime ? 0 : endTime - now);
	}
	st << Stream::eos;
	player->send(st);
}


void AthleticsRank::challenge(Player * atker, std::string& name)
{
	UInt32 Viplvl = atker->getVipLevel();
	const static UInt8 Maxchallengenum[] = {15, 15, 15, 15, 18, 20, 20, 20, 20, 20};

	Player * defer = globalNamedPlayers[atker->fixName(name)];
	if (defer == NULL || atker == defer)
		return ;
	UInt8 row = getRankRow(defer->GetLev());
	if (row == 0xFF)
		return ;
	if (row != getRankRow(atker->GetLev()))
	{
		atker->sendMsgCode(0, 2054);
		Stream st(0xD2);
		st << Stream::eos;
		atker->send(st);
		ERROR_LOG("Cannot find the athletics player[%"I64_FMT"u][%s][%d]", atker->getId(), atker->getName().c_str(), row);
		enterAthleticsReq(atker, atker->GetLev());
		return ;
	}
	RankList::iterator deferRank = _ranks[row].find(defer);
	if (deferRank == _ranks[row].end())
		return ;
	RankList::iterator atkerRank = _ranks[row].find(atker);
	if (atkerRank == _ranks[row].end())
		return ;
	UInt16 atkerRankPos = getRankPos(row, atkerRank->second);
	UInt16 deferRankPos = getRankPos(row, deferRank->second);
	if(atkerRankPos != 1)
	{
		if (atkerRankPos <= deferRankPos)
			return;
		if (atkerRankPos - deferRankPos > 14)
		{
			atker->sendMsgCode(0, 2054);
			Stream st(0xD2);
			st << Stream::eos;
			atker->send(st);
			return ;
		}
	}
	else if(deferRankPos - atkerRankPos > 4)
		return;
	if (defer->hasGlobalFlag(Player::Challenging) || defer->hasGlobalFlag(Player::BeChallenging))
	{
		atker->sendMsgCode(0, 2053);
		return ;
	}
	if (atker->hasGlobalFlag(Player::Challenging) || atker->hasGlobalFlag(Player::BeChallenging) || atker->getBuffLeft(PLAYER_BUFF_ATHLETICS) != 0)
		return ;
	AthleticsRankData * data = *(atkerRank->second);
	data->challengenum = updateChallengeNum(data->challengenum, data->challengetime);
	if (data->challengenum >= Maxchallengenum[Viplvl])
		return ;
	atker->addGlobalFlag(Player::Challenging);
	defer->addGlobalFlag(Player::BeChallenging);
	data->challengenum ++;
	gSpecialAward.newServerActivity(atker, data->challengenum);
	data->challengetime = TimeUtil::Now();

	UInt32 challengeBuff=data->challengetime+ATHLETICS_BUFF_TIME;
	if(Viplvl >= 2)
		challengeBuff=data->challengetime+ 5 * 60;
	else if(Viplvl == 1)
		challengeBuff=data->challengetime+ 7 * 60;

	atker->setBuffData(PLAYER_BUFF_ATHLETICS, challengeBuff);
	DB().PushUpdateData("UPDATE `athletics_rank` SET `challengeNum` = %u, `challengeTime` = %u WHERE `ranker` = %"I64_FMT"u", data->challengenum, data->challengetime, data->ranker->getId());
	GameMsgHdr hdr(0x212, atker->getThreadId(), atker, sizeof(Player *));
	GLOBAL().PushMsg(hdr, &defer);
	DBLOG().PushUpdateData("insert into `athletics_challenge`(`server_id`, `row`, `attacker_rank`, `defender_rank`, `created_at`) values(%u, %u, %u, %u, %u)", cfg.serverLogId, row, atkerRankPos, deferRankPos, TimeUtil::Now());
}


void AthleticsRank::notifyAthletcisOver(Player * atker, Player * defer, UInt32 id, bool win)
{
	atker->delGlobalFlag(Player::Challenging);
	defer->delGlobalFlag(Player::BeChallenging);
	UInt8 row = getRankRow(atker->GetLev());
	if (row == 0xFF)
		return ;
	if (row != getRankRow(defer->GetLev()))
		return ;
	RankList::iterator deferRank = _ranks[row].find(defer);
	if (deferRank == _ranks[row].end())
		return ;
	RankList::iterator atkerRank = _ranks[row].find(atker);
	if (atkerRank == _ranks[row].end())
		return ;
	UInt16 atkerRankPos = getRankPos(row, atkerRank->second);
	UInt16 deferRankPos = getRankPos(row, deferRank->second);
	if(atkerRankPos != 1)
	{
		if (atkerRankPos <= deferRankPos || atkerRankPos - deferRankPos > 14)
			return ;
	}
	else if(deferRankPos - atkerRankPos > 4)
		return;
	AthleticsRankData * deferdata = *(deferRank->second);
	UInt8 type = 0xFF;
	UInt8 color = deferdata->boxcolor;
	//std::string boxName = deferdata->awardName;
	UInt32 atkerAward = 0;
	UInt8 newRank = 0;
	if (!win)
	{
		(*(atkerRank->second))->winstreak = 0;
		DB().PushUpdateData("UPDATE `athletics_rank` SET `winStreak` = 0 WHERE `ranker` = %"I64_FMT"u", deferdata->ranker->getId());
		if (getRankPos(row, deferRank->second) == 1)
		{
			SYSMSG_BROADCASTV(324, deferdata->ranker->getName().c_str());
		}
	}
	else
	{
		deferdata->winstreak = 0;
		DB().PushUpdateData("UPDATE `athletics_rank` SET `winStreak` = 0 WHERE `ranker` = %"I64_FMT"u", deferdata->ranker->getId());
		
		AthleticsRankData * data = *(atkerRank->second);
		data->winstreak ++;
		if (data->winstreak >= 3)
		{
			Stream st(0xD3);
			st << atker->getName() << atker->getCountry() << defer->getName() << defer->getCountry() << data->winstreak << Stream::eos;
			NETWORK()->Broadcast(st);

			if (data->winstreak == 3 || data->winstreak == 5 || data->winstreak == 10 || data->winstreak == 15)
			{
				SYSMSG_BROADCASTV(322, data->ranker->getName().c_str(), data->winstreak);
			}
		}

		if(atkerRankPos != 1)
		{
			data->rank = deferdata->rank;
			UInt32 deferPos = static_cast<UInt32>(getRankPos(row, deferRank->second));
			data->maxrank = std::min(data->maxrank, deferPos);
			if (deferPos == 1)
			{
				SYSMSG_BROADCASTV(323, data->ranker->getName().c_str(), (*(deferRank->second))->ranker->getName().c_str());
			}
			if (deferPos <= 10)
			{
				newRank = deferPos;
			}
			DB().PushUpdateData("UPDATE `athletics_rank` SET `rank` = %u, `maxRank` = %u, `winStreak` = %u WHERE `ranker` = %"I64_FMT"u", data->rank, data->maxrank, data->winstreak, data->ranker->getId());
			
			Rank rankUp;
			getRankUpNeighbour(row, atkerRank->second, rankUp);
			updateBatchRanker(row, deferRank->second, rankUp);
			_athleticses[row].erase(atkerRank->second);
			_ranks[row][atker] = _athleticses[row].insert(deferRank->second, data);
		}
		else
		{
			DB().PushUpdateData("UPDATE `athletics_rank` SET `winStreak` = %u WHERE `ranker` = %"I64_FMT"u", data->winstreak, data->ranker->getId());
		}
		if (DistributeBox(atker, deferRank->second, atkerAward))
		{
			type = deferdata->awardType;
			BuildNewBox(deferRank->second);
			DB().PushUpdateData("UPDATE `athletics_rank` SET `boxColor` = %u, `boxType` = %u, `boxCount` = %u, boxFlushTime = %u WHERE `ranker` = %"I64_FMT"u", deferdata->boxcolor, deferdata->awardType, deferdata->awardCount, deferdata->boxflushtime, deferdata->ranker->getId());
			DB().PushUpdateData("UPDATE `athletics_record` SET `awardType` = %u, `awardAtkerCount` = %u WHERE `id` = %u", type, atkerAward, id);
		}
	}
	AthleticsAward atkerAthleticsAward = { id, color, type, static_cast<UInt8>(1 + newRank), win, atkerAward, deferdata->ranker };
	AthleticsAward deferAthleticsAward = { id, color, type, 0, !win, atkerAward, atker };

	GameMsgHdr hdr1(0x217, atker->getThreadId(), atker, sizeof(AthleticsAward));
	GLOBAL().PushMsg(hdr1, &atkerAthleticsAward);

	GameMsgHdr hdr2(0x217, deferdata->ranker->getThreadId(), deferdata->ranker, sizeof(AthleticsAward));
	GLOBAL().PushMsg(hdr2, &deferAthleticsAward);
}

void AthleticsRank::notifyAthletcisBoxFlushTime(Player * player)
{
	UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return ;
	RankList::iterator playerRank = _ranks[row].find(player);
	if (playerRank == _ranks[row].end())
		return ;
	UInt32 now = TimeUtil::Now();
	updateBoxTimeoutAward(playerRank->second, row, now);
	UInt16 tmout = GetOutTimebyColor((*playerRank->second)->boxcolor);
	UInt32 endTime = (*playerRank->second)->boxflushtime + static_cast<UInt32>(tmout);
	Stream st(0xD4);

	st << static_cast<UInt8>(255) << (*playerRank->second)->awardType << (*playerRank->second)->awardCount <<static_cast<UInt16>(now >= endTime ? 0 : endTime - now) << Stream::eos;
	player->send(st); 
}


bool AthleticsRank::DistributeBox(Player *atker, Rank deferRank, UInt32& atkerAward)
{
	if(isTimeOut((*deferRank)->boxflushtime, (*deferRank)->boxcolor))
		return false;

	UInt32 count = (*deferRank)->awardCount;

	UInt32 now = TimeUtil::Now();
	if (now < (*deferRank)->boxflushtime)
		now = (*deferRank)->boxflushtime;
	atkerAward =  static_cast<UInt32>(static_cast<UInt64>(count) * static_cast<UInt64>(now - (*deferRank)->boxflushtime) / static_cast<UInt64>(GetOutTimebyColor((*deferRank)->boxcolor)));

	//deferAward = count * PLUND_RATE / 10;
	//atkerAward = count * (10 - PLUND_RATE) / 10;
	if(World::_wday == 5)
		atkerAward = atkerAward * 3 / 2;

	return true;
}


bool AthleticsRank::isTimeOut(UInt32 tm, UInt8 boxcolor)
{
	UInt32 tmout = static_cast<UInt32>(GetOutTimebyColor(boxcolor));
	if( TimeUtil::Now() >= tmout + tm)
		return true;
	return false;
}


void AthleticsRank::GetBoxSourceReq(Player *owner)
{
	UInt8 row = getRankRow(owner->GetLev());
	if (row == 0xFF)
		return ;

	RankList::iterator ownerRank = _ranks[row].find(owner);

	if (ownerRank == _ranks[row].end())
		return ;
	AthleticsRankData *data = *(ownerRank->second);

	if(!isTimeOut(data->boxflushtime, data->boxcolor))
		return ;
	UInt32 awardCount = data->awardCount;
	if(World::_wday == 5)
		awardCount = awardCount * 3 / 2;

	char buffer[5];
	*reinterpret_cast<UInt8 *>(buffer) = data->awardType;
	*reinterpret_cast<UInt32 *>(buffer+1) = awardCount;

	// 发消息到玩家线程，获取资源。
	GameMsgHdr hdr(0x218, owner->getThreadId(), owner, sizeof(buffer));
	GLOBAL().PushMsg(hdr, buffer);

	BuildNewBox(ownerRank->second);

	Stream st(0xD4);
	st << data->boxcolor << data->awardType << data->awardCount << GetOutTimebyColor(data->boxcolor) << Stream::eos;
	owner->send(st);
	DB().PushUpdateData("UPDATE `athletics_rank` SET `boxColor` = %u, `boxType` = %u, `boxCount` = %u, boxFlushTime = %u WHERE `ranker` = %"I64_FMT"u", data->boxcolor, data->awardType, data->awardCount, data->boxflushtime, data->ranker->getId());
}


bool AthleticsRank::updateBoxTimeoutAward(Rank rank, UInt8 row, UInt32 now)
{
	AthleticsRankData * data = *rank;
	if (data->boxflushtime >= now)
		return false;

	if (now - data->boxflushtime >= 12 * 60 * 60)
	{
		if(data->ranker->isOnline() || now - data->ranker->getLastOnline() < 86400)
		{
			char buffer[5];
			if(World::_wday == 5)
				data->awardCount = data->awardCount * 3 / 2;
			*reinterpret_cast<UInt8 *>(buffer) = data->awardType;
			*reinterpret_cast<UInt32 *>(buffer+1) = data->awardCount;

			GameMsgHdr hdr(0x218, data->ranker->getThreadId(), data->ranker, sizeof(buffer));
			GLOBAL().PushMsg(hdr, buffer);
			SYSMSG(title, 318);
			const UInt32 Index[]  = {315, 339, 316, 317, 356};
			SYSMSG(strType, Index[data->awardType]);
			SYSMSGV(content, 333, data->awardCount, strType);
			data->ranker->GetMailBox()->newMail(NULL, 0x01, title, content);
		}

		BuildNewBox(rank);
		DB().PushUpdateData("UPDATE `athletics_rank` SET `boxColor` = %u, `boxType` = %u, `boxCount` = %u, `boxFlushTime` = %u WHERE `ranker` = %"I64_FMT"u", data->boxcolor, data->awardType, data->awardCount, data->boxflushtime, data->ranker->getId());

		return true;
	}

	return false;
}


void AthleticsRank::TmExtraAward()
{
	gSpecialAward.AthleticsAward();
	static UInt32 mailIndex[] = { 326, 327, 328, 329, 329, 329, 329, 329, 329, 329 };

	AthleticsList::iterator start = _athleticses[1].begin();
	AthleticsList::iterator end = start;

	if (_athleticses[1].size() >= 10)
		std::advance(end, 10);
	else
		end = _athleticses[1].end();
	Mail *pmail = NULL;
	for (UInt16 rank = 1; start != end; ++start, ++rank)
	{
		Player *ranker = (*start)->ranker;
		UInt8 level = ranker->GetLev();
		UInt16 equipid = 0;
		UInt16 cnt = 0;
		getRandomEquip(level, rank, equipid, cnt);

		if(equipid == 0)
			continue;//error; 
		const GData::ItemBaseType *item1 = Package::GetItemBaseType(equipid);
        if (!item1)
            return;

		MailPackage::MailItem mitem[5] = {{8913, 1}, {equipid, cnt}};
		UInt32 count = 2;
		SYSMSG(title, 318);
		if(rank <= 3)
		{
			UInt16 FavorId = 0;
			if(level >= 100)
			{
				UInt16 rand = uRand(4);
				FavorId = DietyFavor[rand];
			}
			else
			{
				UInt16 rand = uRand(5);
				FavorId = OrangeFavor[rand];
			}
			const GData::ItemBaseType *item2 = Package::GetItemBaseType(FavorId);
            if (!item2)
                return;
			mitem[2].id = 8999; mitem[2].count = 4 - rank;
			mitem[3].id = FavorId; mitem[3].count = 4 - rank;
			count = 4;
			if(rank == 1)
			{
				mitem[4].id = 9000; mitem[4].count = 1;
				count = 5;
			}
			MailItemsInfo itemsInfo(mitem, AthliticisTimeAward, count);
			SYSMSGV(awardStr, mailIndex[rank-1], item2->getName().c_str(), item1->getName().c_str(), cnt);
			SYSMSGV(content, 325, rank, awardStr);
			pmail = ranker->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
		}
		else
		{
			MailItemsInfo itemsInfo(mitem, AthliticisTimeAward, count);
			SYSMSGV(awardStr, mailIndex[rank-1], item1->getName().c_str());
			SYSMSGV(content, 325, rank, awardStr);
			pmail = ranker->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
		}

		if(pmail != NULL)
		{
			mailPackageManager.push(pmail->id, mitem, count, false);
		}
	}

	if(_athleticses[1].size() >= 3)
	{
		AthleticsList::iterator first  = _athleticses[1].begin();
		AthleticsList::iterator second = first;
		AthleticsList::iterator third  = first;

		std::advance(second, 1);
		std::advance(third, 2);

		SYSMSG_BROADCASTV(330, (*first)->ranker->getCountry(), (*first)->ranker->getName().c_str(), (*second)->ranker->getCountry(), (*second)->ranker->getName().c_str(), (*third)->ranker->getCountry(), (*third)->ranker->getName().c_str());
	}
	else if(_athleticses[1].size() == 2)
	{
		AthleticsList::iterator first  = _athleticses[1].begin();
		AthleticsList::iterator second = first;

		std::advance(second, 1);

		SYSMSG_BROADCASTV(331, (*first)->ranker->getCountry(), (*first)->ranker->getName().c_str(), (*second)->ranker->getCountry(), (*second)->ranker->getName().c_str());
	}
	else if(_athleticses[1].size() == 1)
	{
		AthleticsList::iterator first  = _athleticses[1].begin();

		SYSMSG_BROADCASTV(332, (*first)->ranker->getCountry(), (*first)->ranker->getName().c_str());
	}

}


void AthleticsRank::getRandomEquip(UInt8 level, UInt16 rank, UInt16& EquipId, UInt16& cnt)
{
	cnt = 1;
	if ( rank >= 1 && rank <=3)
	{
		UInt32 rand = uRand(28);
		if(level >= 100)
		{
			UInt16 count[] = {2, 1, 1};
			EquipId = 9214;
			cnt = count[rank - 1];
		}
		else if(level >= 90)
		{
			UInt16 count[] = {3, 1, 1};
			EquipId = 9031;
			cnt = count[rank - 1];
		}
		else if(level >= 80)
			EquipId = OrangeEquip80[rand];
		else if (level >= 70)
			EquipId = OrangeEquip70[rand];
		else
			EquipId = OrangeEquip50[rand];
	}
	else if( rank >= 4 && rank <= 10)
	{
		if(level >= 100)
		{
			UInt32 rand = uRand(14);
			EquipId = PurpleEquip100[rand];
		}
		else if(level >= 90)
		{
			UInt32 rand = uRand(14);
			EquipId = PurpleEquip90[rand];
		}
		else if(level >= 80)
		{
			UInt32 rand = uRand(56);
			EquipId = PurpleEquip80[rand];
		}
		else if (level >= 70)
		{
			UInt32 rand = uRand(56);
			EquipId = PurpleEquip70[rand];
		}
		else if (level >= 60)
		{
			UInt32 rand = uRand(56);
			EquipId = PurpleEquip60[rand];
		}
		else 
		{		
			UInt32 rand = uRand(56);
			EquipId = PurpleEquip50[rand];
		}
	}
}

UInt8 AthleticsRank::getChallengeNum( Player * player )
{
	UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;
	return (*found->second)->challengenum;
}

}

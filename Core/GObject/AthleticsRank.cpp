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
#include "Athletics.h"
#include "MsgID.h"
#include "Common/URandom.h"

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

        for (std::size_t j = 0; j < _athleticsesEvent[i].size(); ++j)
        {
            SAFE_DELETE(_athleticsesEvent[i][j]);
        }
        _athleticsesEvent[i].clear();
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
		DB6().PushUpdateData("UPDATE `athletics_rank` SET `rank` = %u WHERE `ranker` = %"I64_FMT"u", (*begin)->rank, (*begin)->ranker->getId());
	}
}
void AthleticsRank::updatePageNum(Rank r)
{
    DB6().PushUpdateData("UPDATE `athletics_rank` SET `pageNum` = %u WHERE `ranker` = %"I64_FMT"u", (*r)->pageNum,(*r)->ranker->getId());
}
void AthleticsRank::addAthleticsFromDB(UInt8 row, AthleticsRankData * data)
{
	_maxRank[row] = std::max(_maxRank[row], data->rank);
    if(data->oldrank == 0)
    {
        data->oldrank = data->rank;
    }

	_ranks[row][data->ranker] = _athleticses[row].insert(_athleticses[row].end(), data);
}

bool AthleticsRank::enterAthleticsReq(Player * player ,UInt8 lev)
{
	UInt8 row = getRankRow(lev);
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
			//DB6().PushUpdateData("DELETE FROM `athletics_rank` WHERE `ranker` = %"I64_FMT"u", data->ranker->getId());
			_ranks[1][player] = _athleticses[1].insert(_athleticses[1].end(), data);;
			data->rank = ++_maxRank[1];
            data->oldrank = data->rank;
			data->maxrank = _athleticses[1].size();
            DB6().PushUpdateData("UPDATE `athletics_rank` SET `row`=1, `rank`=%u, `maxRank`=%u, `oldrank`=%u where ranker=%"I64_FMT"u", data->rank, data->maxrank, data->oldrank, data->ranker->getId());
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
        data->prestige = 0;
        data->tael = 0;
		data->winstreak = 0;
        data->bewinstreak = 0;
        data->failstreak = 0;
        data->befailstreak = 0;
        data->oldrank = data->rank;
        data->first4rank = 0;
        data->extrachallenge = 0;
        data->pageNum = 0;
		Rank rank = _athleticses[row].insert(_athleticses[row].end(), data);
		_ranks[row][player] = rank;
		data->maxrank = _athleticses[row].size();
		BuildNewBox(rank);
        DB6().PushUpdateData("INSERT INTO `athletics_rank` VALUES(%u, %u, %"I64_FMT"u, %u, %u, %u, 0, 0, %u, %u, %u, %u, %u, %u, %u, %u)", row, data->rank, data->ranker->getId(), data->maxrank, data->challengenum, data->challengetime, data->winstreak, data->bewinstreak, data->failstreak, data->befailstreak, data->oldrank, data->first4rank, data->extrachallenge, data->pageNum);
		GameMsgHdr hdr(0x216, player->getThreadId(), player, 0);
		GLOBAL().PushMsg(hdr, NULL);
	}

	GameMsgHdr hdr(0x219, player->getThreadId(), player, sizeof(Player *));
	GLOBAL().PushMsg(hdr, &row);
	return true;
}

void AthleticsRank::BuildNewBox(Rank &it_rank)// UInt8 row, UInt16 rank, UInt8 lvl)
{
    return;
#if 0
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
#endif
}


void AthleticsRank::requestAthleticsList(Player * player, UInt16 type)
{
    if(type == 0)
        type = 0xFFFF;

    if(type & 0x10)
    {
        GameMsgHdr hdr(0x232, player->getThreadId(), player, 0);
        GLOBAL().PushMsg(hdr, NULL);
        //requestAthleticsEvent(st, player);
    }

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
    UInt8 rank3num = 0;
    UInt8 ranknum = 0;
	if (rankpos > 10)
	{
        UInt16 top = 10 * ((*rank)->pageNum + 1);

        if(top  < rankpos)
        {
            start = rank;
            std::advance(start ,  -1 * top);
            end = start;
            std::advance(end , 10);
        }
        else
        {
            start = end = getRankBegin(row);
            std::advance(end , 10);
        }
//		start = end = rank;
//		std::advance(start, -10);
        rank3num = 3;
        ranknum = 10;
	}
	else
	{
		start = end = getRankBegin(row);
		UInt16 ranksize = getRankSize(row);
		if (ranksize > 9)
        {
			std::advance(end, 10);
            ranknum = 10;
        }
		else
        {
			std::advance(end, ranksize);
            ranknum = ranksize;
        }

        if(ranksize > 2)
            rank3num = 3;
        else
            rank3num = ranksize;
	}
	Stream st(REP::ARENA_IFNO);
    st << static_cast<UInt16>(type & 0xE7);
	//UInt32 now = TimeUtil::Now();
	//UInt32 endTime = (*rank)->challengetime + ATHLETICS_BUFF_TIME;
	//
	//if(player->getVipLevel() >= 2)
	//	endTime=(*rank)->challengetime+5 * 60;
	//else if(player->getVipLevel() == 1)
	//	endTime=(*rank)->challengetime+7 * 60;
	
	(*rank)->challengenum = updateChallengeNum((*rank)->challengenum, (*rank)->challengetime);
    if(type & 0x01)
    {
        st << static_cast<UInt32>(getRankPos(row, rank)) << (*rank)->maxrank << (*rank)->challengenum << (*rank)->winstreak << static_cast<UInt16>(player->getBuffLeft(PLAYER_BUFF_ATHLETICS)) << (*rank)->prestige << (*rank)->tael;
    }

    if(type & 0x02)
    {
        st << rank3num;
        UInt8 i = 0;
        for(Rank it = getRankBegin(row); i < rank3num; ++i, ++it)
        {
            st << i << (*it)->ranker->getName();
        }
    }

    if(type & 0x04)
    {
        st << ranknum;
        UInt32 i = static_cast<UInt32>(getRankPos(row, start));
        for (Rank offset = start; offset != end; ++offset, ++i)
        {
            st << i << (*offset)->ranker->getName() << (*offset)->ranker->getCountry() << (*offset)->ranker->GetLev() << (*offset)->winstreak;
#if 0
            updateBoxTimeoutAward(offset, row, now);
            UInt16 tmout = GetOutTimebyColor((*offset)->boxcolor);
            UInt32 endTime = (*offset)->boxflushtime + static_cast<UInt32>(tmout);
            Clan * clan = (*offset)->ranker->getClan();
            st << (*offset)->ranker->getName() << (clan == NULL ? "" : clan->getName()) << (*offset)->ranker->getCountry() << (*offset)->ranker->GetLev() << (*offset)->boxcolor << (*offset)->awardType << (*offset)->awardCount << static_cast<UInt16>(now >= endTime ? 0 : endTime - now);
#endif
        }
    }

    if(type & 0x08)
    {
        GameMsgHdr hdr(0x221, player->getThreadId(), player, 0);
        GLOBAL().PushMsg(hdr, NULL);
    }

	st << Stream::eos;
	player->send(st);

    if((*rank)->extrachallenge && type == static_cast<UInt16>(0xFFFF) && !(static_cast<UInt32>(0x80000000) & (*rank)->extrachallenge))
    {
        UInt8 color = 0;
        if((*rank)->first4rank & 0x100)
            color = 5;
        else if((*rank)->first4rank & 0x200)
            color = 4;
        else if((*rank)->first4rank & 0x400)
            color = 3;
        else if((*rank)->first4rank & 0x800)
            color = 2;

        Stream st(REP::FIGHT_INFO_CHANGE);
        st << static_cast<UInt16>(0) << color << Stream::eos;
        player->send(st);
    }
}
void AthleticsRank::RequestPageNum(Player* player)
{
    if(player->hasGlobalFlag(Player::AthPayForPage))
        return;

    UInt8 row = getRankRow(player->GetLev());
    if (row == 0xFF)
       return ;
    RankList::iterator found = _ranks[row].find(player);
    if (found == _ranks[row].end())
    {
        return;
    }
    Rank rank = found->second;
    UInt16 rankpos = getRankPos(row, rank);
    if(rankpos <=10)
        return;

    if(( (*rank)->pageNum *10 + 11) >= rankpos) 
        return;

    if((*rank)->pageNum >= 10)
    {
        player->sendMsgCode(0, 1499); //最多翻10页
        return;
    }
    player->addGlobalFlag(Player::AthPayForPage);
   // (*rank)->pageNum  ++ ;
   // send msg to Country
    AthleticsPayPaging  msg;
    GameMsgHdr hdr1(0x222, player->getThreadId(), player, sizeof(msg));
    GLOBAL().PushMsg(hdr1, &msg);

}
void AthleticsRank::AddPageNum(Player* player, bool bMoneyEnough)
{

    if(player->hasGlobalFlag(Player::AthPayForPage) == false)
        return;
    
    player->delGlobalFlag(Player::AthPayForPage);

     UInt8 row = getRankRow(player->GetLev());
     if(row == 0xFF)
         return;

     if(!bMoneyEnough)

         return;
   RankList::iterator found = _ranks[row].find(player);
    if (found == _ranks[row].end())
    {
        return;
    }
    Rank rank = found->second;
    UInt16 rankpos = getRankPos(row, rank);
    if(rankpos <=10)
    {
        return;
    }

    (*rank)->pageNum  ++ ;
    updatePageNum(rank);
   requestAthleticsList(player, 0  );//sendList
}
void AthleticsRank::challenge(Player* atker, UInt8 type)
{
	UInt8 row = getRankRow(atker->GetLev());

	RankList::iterator atkerRank = _ranks[row].find(atker);
	if (atkerRank == _ranks[row].end())
		return;

    UInt32 extrachallenge = (*atkerRank->second)->extrachallenge;
    if((0 == extrachallenge) || (extrachallenge & static_cast<UInt32>(0x80000000)))
        return;

    Rank tmp = _athleticses[row].begin();
    std::advance(tmp, extrachallenge - 1);
    Player* defer = (*tmp)->ranker;
    switch(type)
    {
    case 0:
        //RunAthleticsEvent(row, atkerRank->second, _athleticses[row].end(), 2);
        break;
    case 1:
        {
            if (TimeUtil::SharpDay(0, (*atkerRank->second)->challengetime) != WORLD().ThisDay())
            {
                setAthleticsExtraChallenge(atker, 0);
                (*atkerRank->second)->oldrank = (*atkerRank->second)->rank;
                (*atkerRank->second)->first4rank &= 0xFFFFB0FF;
                DB6().PushUpdateData("UPDATE `athletics_rank` SET `oldrank` = %u, `first4rank` = %u WHERE `ranker` = %"I64_FMT"u", (*atkerRank->second)->oldrank, (*atkerRank->second)->first4rank, (*atkerRank->second)->ranker->getId());
            }
            else
            {
                setAthleticsExtraChallenge(atker, static_cast<UInt32>(0x80000000) | extrachallenge);
                setAthleticsFirst4Rank(atker, 0x4000);
                std::string name = defer->getName();
                challenge(atker, name, type);
            }
        }
        break;
    }
}

void AthleticsRank::challenge(Player * atker, std::string& name, UInt8 type)
{
	UInt32 Viplvl = atker->getVipLevel();
	//const static UInt8 Maxchallengenum[] = {15, 15, 15, 15, 15, 15, 20, 20, 20, 20, 20};
    if( atker->hasFlag(GObject::Player::InHeroIsland) )
        return;

	Player * defer = globalNamedPlayers[atker->fixName(name)];
	if (defer == NULL || atker == defer)
		return ;
	UInt8 row = getRankRow(defer->GetLev());
	if (row == 0xFF)
		return ;
	if (row != getRankRow(atker->GetLev()))
	{
		atker->sendMsgCode(0, 1405);
		Stream st(REP::ATHLETICS_CHALLENGE);
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
    UInt16 pageNum = (*atkerRank->second)->pageNum; //得到攻击者的页面
	if(atkerRankPos > 10 )
	{
		if (atkerRankPos <= deferRankPos)
        {
            if(type)
                setAthleticsExtraChallenge(atker, 0);
			return;
        }
		if ((atkerRankPos - deferRankPos > 10 * (pageNum + 1))  && !type)
		{
			//Stream st(REP::ATHLETICS_CHALLENGE);
			//st << Stream::eos;
			//atker->send(st);
			atker->sendMsgCode(0, 1405);
			return ;
		}
	}
	else 
    {
        if(deferRankPos > 10)
		    return;
    }

    if (defer->hasGlobalFlag(Player::Challenging) || defer->hasGlobalFlag(Player::BeChallenging))
    {
        atker->sendMsgCode(0, 1413);
        return ;
    }

    if(!type)
    {
        AthleticsRankData * data = *(atkerRank->second);
        if (atker->hasGlobalFlag(Player::Challenging) || atker->hasGlobalFlag(Player::BeChallenging) || atker->getBuffLeft(PLAYER_BUFF_ATHLETICS) != 0)
            return ;

#if 0
        data->challengenum = updateChallengeNum(data->challengenum, data->challengetime);
        if (data->challengenum >= Maxchallengenum[Viplvl] && cfg.GMCheck)
            return ;

        data->challengenum ++;

        gSpecialAward.newServerActivity(atker, data->challengenum);
#endif

        //updateAthleticsRank(data);
        data->challengenum = updateChallengeNum(data->challengenum, data->challengetime);
        data->challengetime = TimeUtil::Now();

        UInt32 challengeBuff=data->challengetime+ (cfg.GMCheck ? ATHLETICS_BUFF_TIME : 10);
        if(Viplvl >= 1 && Viplvl <= 3)
            challengeBuff=data->challengetime + 10 * 60;
        else if (Viplvl > 3)
            challengeBuff=data->challengetime + 5 * 60;

        atker->setBuffData(PLAYER_BUFF_ATHLETICS, challengeBuff);
        DB6().PushUpdateData("UPDATE `athletics_rank` SET `challengeNum` = %u, `challengeTime` = %u WHERE `ranker` = %"I64_FMT"u",
                data->challengenum, data->challengetime, data->ranker->getId());
    }

	atker->addGlobalFlag(Player::Challenging);
	defer->addGlobalFlag(Player::BeChallenging);
	GameMsgHdr hdr(0x212, atker->getThreadId(), atker, sizeof(Player *));
	GLOBAL().PushMsg(hdr, &defer);
//	DBLOG().PushUpdateData("insert into `athletics_challenge`(`server_id`, `row`, `attacker_rank`, `defender_rank`, `created_at`) values(%u, %u, %u, %u, %u)", cfg.serverLogId, row, atkerRankPos, deferRankPos, TimeUtil::Now());
}

void AthleticsRank::challenge2(Player * atker, std::string& name, UInt8 type)
{
	UInt32 Viplvl = atker->getVipLevel();
	const static UInt8 Maxchallengenum[] = {15, 15, 15, 15, 15, 15, 20, 20, 20, 20, 20};
    if( atker->hasFlag(GObject::Player::InHeroIsland) )
        return;
	Player * defer = globalNamedPlayers[atker->fixName(name)];
	if (defer == NULL || atker == defer || type != 3)
		return ;
	UInt8 row = getRankRow(atker->GetLev());
	if (row == 0xFF)
		return ;
	RankList::iterator atkerRank = _ranks[row].find(atker);
	if (atkerRank == _ranks[row].end())
		return ;
    AthleticsRankData * data = *(atkerRank->second);
    if (atker->hasGlobalFlag(Player::Challenging) || atker->hasGlobalFlag(Player::BeChallenging) || atker->getBuffLeft(PLAYER_BUFF_ATHLETICS) != 0)
        return ; 
    data->challengenum = updateChallengeNum(data->challengenum, data->challengetime);
    if (data->challengenum >= Maxchallengenum[Viplvl] && cfg.GMCheck)
        return ;
    data->challengenum ++;
    data->challengetime = TimeUtil::Now();

    UInt32 challengeBuff=data->challengetime+ (cfg.GMCheck ? ATHLETICS_BUFF_TIME : 10);
    if(Viplvl >= 1 && Viplvl <= 3)
        challengeBuff=data->challengetime + 10 * 60;
    else if (Viplvl > 3)
        challengeBuff=data->challengetime + 5 * 60;

    atker->setBuffData(PLAYER_BUFF_ATHLETICS, challengeBuff);
    DB6().PushUpdateData("UPDATE `athletics_rank` SET `challengeNum` = %u, `challengeTime` = %u WHERE `ranker` = %"I64_FMT"u", data->challengenum, data->challengetime, data->ranker->getId());

	atker->addGlobalFlag(Player::Challenging);
	defer->addGlobalFlag(Player::BeChallenging);
	GameMsgHdr hdr(0x233, atker->getThreadId(), atker, sizeof(Player *));
	GLOBAL().PushMsg(hdr, &defer);
//	DBLOG().PushUpdateData("insert into `athletics_challenge`(`server_id`, `row`, `attacker_rank`, `defender_rank`, `created_at`) values(%u, %u, %u, %u, %u)", cfg.serverLogId, row, atkerRankPos, deferRankPos, TimeUtil::Now());
}

void AthleticsRank::notifyAthMartialOver(Player * atker, Player * defer, UInt32 id, UInt8 res)
{
    atker->delGlobalFlag(Player::Challenging);
	defer->delGlobalFlag(Player::BeChallenging);

	UInt8 row = getRankRow(atker->GetLev());
	RankList::iterator atkerRank = _ranks[row].find(atker);
	if (atkerRank == _ranks[row].end())
		return ;

    AthleticsRankData * data = *(atkerRank->second);
    if(!data)
        return;

    if(res == 2)
    {
        -- data->challengenum;
        DB6().PushUpdateData("UPDATE `athletics_rank` SET `challengeNum` = %u WHERE `ranker` = %"I64_FMT"u", data->challengenum, atker->getId());
    }
    else
    {
        Stream st(REP::FIGHT_INFO_CHANGE);
        st << static_cast<UInt16>(0x09);
        st << Stream::eos;
        atker->send(st);
    }
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

    AthleticsRankData * data = *(atkerRank->second);
	UInt16 atkerRankPos = getRankPos(row, atkerRank->second);
	UInt16 deferRankPos = getRankPos(row, deferRank->second);
	if(atkerRankPos >10)
	{
		if (atkerRankPos <= deferRankPos || (atkerRankPos - deferRankPos > 10*(data->pageNum + 1)))// && !(getAthleticsExtraChallenge(atker) & static_cast<UInt32>(0x80000000))))
			return;
	}
	else 
    {
        if(deferRankPos >10)
		     return;
    }
	AthleticsRankData * deferdata = *(deferRank->second);
	UInt8 type = 0xFF;
	//UInt8 color = deferdata->boxcolor;
	//std::string boxName = deferdata->awardName;
	UInt32 atkerAward = 0;
	UInt8 newRank = 0;
    UInt16 flag = 0x09;    // 战报和基本信息
    //UInt32 prestige = 0;

	if (!win)
	{
//		AthleticsRankData * data = *(atkerRank->second);
        ++ data->failstreak;
        ++ deferdata->bewinstreak;

        //prestige = RunAthleticsEvent(row, atkerRank->second, deferRank->second, win);

		data->winstreak = 0;
        deferdata->befailstreak = 0;
		//DB6().PushUpdateData("UPDATE `athletics_rank` SET `winStreak` = 0, `failstreak` = %u WHERE `ranker` = %"I64_FMT"u", data->failstreak, data->ranker->getId());
		//DB6().PushUpdateData("UPDATE `athletics_rank` SET `bewinstreak` = %u WHERE `ranker` = %"I64_FMT"u", data->bewinstreak, deferdata->ranker->getId());
		if (getRankPos(row, deferRank->second) == 1)
		{
			SYSMSG_BROADCASTV(324, deferdata->ranker->getName().c_str());
		}
        if(data->pageNum > 0)
            flag |= 0x04;
	}
	else
	{
#if 0
		if (data->winstreak >= 3)
		{
			Stream st(REP::WINSTREAK);
			st << atker->getName() << atker->getCountry() << defer->getName() << defer->getCountry() << data->winstreak << Stream::eos;
			NETWORK()->Broadcast(st);

			if (data->winstreak == 3 || data->winstreak == 5 || data->winstreak == 10 || data->winstreak == 15)
			{
				SYSMSG_BROADCASTV(322, data->ranker->getName().c_str(), data->winstreak);
			}
		}
#endif

//		AthleticsRankData * data = *(atkerRank->second);
        bool bNeedChangePos =  true;
        if(  atkerRankPos <=  deferRankPos)
            bNeedChangePos = false;
//		if(atkerRankPos != 1)
        if(bNeedChangePos)
		{
            flag |= 0x04;
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
            if(deferPos < 4)
            {
                flag |= 0x02;
            }
			
			Rank rankUp;
			getRankUpNeighbour(row, atkerRank->second, rankUp);
			updateBatchRanker(row, deferRank->second, rankUp);
			_athleticses[row].erase(atkerRank->second);
			_ranks[row][atker] = _athleticses[row].insert(deferRank->second, data);
		}

		++ data->winstreak;
        ++ deferdata->befailstreak;

        //prestige = RunAthleticsEvent(row, atkerRank->second, deferRank->second, win);

		deferdata->winstreak = 0;
		deferdata->bewinstreak = 0;
        data->failstreak = 0;

		//DB6().PushUpdateData("UPDATE `athletics_rank` SET `winStreak` = 0, `befailstreak` = %u WHERE `ranker` = %"I64_FMT"u", deferdata->befailstreak, deferdata->ranker->getId());
		
//		if(atkerRankPos != 1)
        if(bNeedChangePos)
        {
			DB6().PushUpdateData("UPDATE `athletics_rank` SET `rank` = %u, `maxRank` = %u, `winStreak` = %u WHERE `ranker` = %"I64_FMT"u", data->rank, data->maxrank, data->winstreak, data->ranker->getId());
        }
#if 0
		else
		{
			DB6().PushUpdateData("UPDATE `athletics_rank` SET `winStreak` = %u WHERE `ranker` = %"I64_FMT"u", data->winstreak, data->ranker->getId());
		}

		if (DistributeBox(atker, deferRank->second, atkerAward))
		{
			type = deferdata->awardType;
			BuildNewBox(deferRank->second);
			DB6().PushUpdateData("UPDATE `athletics_rank` SET `boxColor` = %u, `boxType` = %u, `boxCount` = %u, boxFlushTime = %u WHERE `ranker` = %"I64_FMT"u", deferdata->boxcolor, deferdata->awardType, deferdata->awardCount, deferdata->boxflushtime, deferdata->ranker->getId());
			DB6().PushUpdateData("UPDATE `athletics_record` SET `awardType` = %u, `awardAtkerCount` = %u WHERE `id` = %u", type, atkerAward, id);
		}
#endif
	}

    if(data->pageNum > 0)
    {
        data->pageNum = 0;
        updatePageNum(atkerRank->second);
    }

    {
        Stream st(REP::FIGHT_INFO_CHANGE);
        st << flag;
        st << Stream::eos;
        atker->send(st);
        deferdata->ranker->send(st);
    }

	AthleticsAward atkerAthleticsAward = { id, type, static_cast<UInt8>(1 + newRank), win, 0, 0, atkerAward, deferdata->ranker, 0, 0 };
	AthleticsAward deferAthleticsAward = { id, type, 0, !win, 0, 0, atkerAward, atker, 0, 0 };

	GameMsgHdr hdr1(0x217, atker->getThreadId(), atker, sizeof(AthleticsAward));
	GLOBAL().PushMsg(hdr1, &atkerAthleticsAward);

	GameMsgHdr hdr2(0x217, deferdata->ranker->getThreadId(), deferdata->ranker, sizeof(AthleticsAward));
	GLOBAL().PushMsg(hdr2, &deferAthleticsAward);
}

void AthleticsRank::notifyAthletcisBoxFlushTime(Player * player)
{
    return;
#if 0
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
	Stream st(REP::GETBOX);

	st << static_cast<UInt8>(255) << (*playerRank->second)->awardType << (*playerRank->second)->awardCount <<static_cast<UInt16>(now >= endTime ? 0 : endTime - now) << Stream::eos;
	player->send(st); 
#endif
}


bool AthleticsRank::DistributeBox(Player *atker, Rank deferRank, UInt32& atkerAward)
{
    return true;
#if 0
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
#endif
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
    return;
#if 0
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

	Stream st(REP::GETBOX);
	st << data->boxcolor << data->awardType << data->awardCount << GetOutTimebyColor(data->boxcolor) << Stream::eos;
	owner->send(st);
	DB6().PushUpdateData("UPDATE `athletics_rank` SET `boxColor` = %u, `boxType` = %u, `boxCount` = %u, boxFlushTime = %u WHERE `ranker` = %"I64_FMT"u", data->boxcolor, data->awardType, data->awardCount, data->boxflushtime, data->ranker->getId());
#endif
}


bool AthleticsRank::updateBoxTimeoutAward(Rank rank, UInt8 row, UInt32 now)
{
    return true;
#if 0
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
			//const UInt32 Index[]  = {315, 339, 316, 317, 356};
			//SYSMSG(strType, Index[data->awardType]);
			//SYSMSGV(content, 333, data->awardCount, strType);
			//data->ranker->GetMailBox()->newMail(NULL, 0x01, title, content);
		}

		BuildNewBox(rank);
		DB6().PushUpdateData("UPDATE `athletics_rank` SET `boxColor` = %u, `boxType` = %u, `boxCount` = %u, `boxFlushTime` = %u WHERE `ranker` = %"I64_FMT"u", data->boxcolor, data->awardType, data->awardCount, data->boxflushtime, data->ranker->getId());

		return true;
	}

	return false;
#endif
}


void AthleticsRank::TmExtraAward()
{
	AthleticsList::iterator start = _athleticses[1].begin();
	AthleticsList::iterator end = start;

	if (_athleticses[1].size() >= 10)
		std::advance(end, 10);
	else
		end = _athleticses[1].end();

	for (UInt16 rank = 1; start != end; ++start, ++rank)
    {
        UInt16 itemId = 0;
        UInt8  itemCount = 1;
		Player *ranker = (*start)->ranker;
        if(rank == 1)
        {
            itemId = 2;
            SYSMSG_BROADCASTV(330, ranker->getCountry(), ranker->getName().c_str(), itemId);
        }
        else if(rank == 2)
        {
            itemId = 3;
        }
        else if(rank == 3)
        {
            itemId = 4;
        }
        else
        {
            itemId = 5;
        }

        const GData::ItemBaseType *item1 = Package::GetItemBaseType(itemId);
        if(item1)
        {
            Mail *pmail = NULL;
            MailPackage::MailItem mitem[5] = {{itemId, itemCount}};
            UInt32 count = 1;
			MailItemsInfo itemsInfo(mitem, AthliticisTimeAward, count);

            SYSMSG(title, 318);
			SYSMSGV(awardStr, 326, item1->getName().c_str());
			SYSMSGV(content, 325, rank, awardStr);
			pmail = ranker->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);

            if(pmail != NULL)
            {
                mailPackageManager.push(pmail->id, mitem, count, false);
            }

        }
    }

	AthleticsList::iterator start1 = _athleticses[1].begin();
	AthleticsList::iterator end1 = _athleticses[1].end();
	for (UInt32 i = 1; start1 != end1; ++start1, ++i)
    {
        UInt32 prestige = 0;
        UInt32 tael = 0;
        if( i < 25 )
            prestige = 850 - ( 25 * ( i - 1 ) - ( i - 1 ) * ( i - 2 ) / 2 );
        else if( i < 501 )
            prestige = 551 - i;
        else
            prestige = 50;

        if( i < 39 )
            tael = 18000 - ( 400 * ( i - 1 ) - 10 * ( i - 1 ) * ( i - 2 ) / 2 );
        else if( i < 501 )
            tael = 9830 - ( i - 39 ) * 20;
        else
            tael = 600;

        AthleticsRankData *rank = *start1;
        rank->prestige = prestige;
        rank->tael = tael;
		DB6().PushUpdateData("UPDATE `athletics_rank` SET `prestige` = %u, `tael` = %u WHERE `ranker` = %"I64_FMT"u", rank->prestige, rank->tael, rank->ranker->getId());
    }

    return;
#if 0
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
			MailItemsInfo itemsInfo(mitem, AathliticisTimeAward, count);
			//SYSMSGV(awardStr, mailIndex[rank-1], item2->getName().c_str(), item1->getName().c_str(), cnt);
			//SYSMSGV(content, 325, rank, awardStr);
			//pmail = ranker->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
		}
		else
		{
			MailItemsInfo itemsInfo(mitem, AthliticisTimeAward, count);
			//SYSMSGV(awardStr, mailIndex[rank-1], item1->getName().c_str());
			//SYSMSGV(content, 325, rank, awardStr);
			//pmail = ranker->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
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
#endif
}


void AthleticsRank::getRandomEquip(UInt8 level, UInt16 rank, UInt16& EquipId, UInt16& cnt)
{
    return;
#if 0
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
#endif
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

UInt32 AthleticsRank::getAthleticsRank(Player* player)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;
	return (*found->second)->rank;
}

UInt32 AthleticsRank::getAthleticsRankUpADay(Player* player)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;

	if (TimeUtil::SharpDay(0, (*found->second)->challengetime) != WORLD().ThisDay())
    {
        (*found->second)->oldrank = (*found->second)->rank;
        (*found->second)->first4rank &= 0xFFFFB0FF;
		DB6().PushUpdateData("UPDATE `athletics_rank` SET `oldrank` = %u, `first4rank` = %u WHERE `ranker` = %"I64_FMT"u", (*found->second)->oldrank, (*found->second)->first4rank, (*found->second)->ranker->getId());
    }

    if((*found->second)->oldrank > (*found->second)->rank)
        return (*found->second)->oldrank - (*found->second)->rank;
    else
        return 0;
}

UInt8 AthleticsRank::getAthleticsWinStreak(Player* player)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;
	return (*found->second)->winstreak;
}

UInt8 AthleticsRank::getAthleticsBeWinStreak(Player* player)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;
	return (*found->second)->bewinstreak;
}

UInt8 AthleticsRank::getAthleticsFailStreak(Player* player)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;
	return (*found->second)->failstreak;
}

UInt8 AthleticsRank::getAthleticsBeFailStreak(Player* player)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;
	return (*found->second)->befailstreak;
}

void AthleticsRank::broadcastAthleticsEvent(UInt8 count)
{
    for(int idx = 0; idx < 2; ++idx)
    {
        UInt8 count2 = count;
        if (count2 > static_cast<UInt16>(_athleticsesEvent[idx].size()))
            count2 = static_cast<UInt16>(_athleticsesEvent[idx].size());

        for(Rank it = _athleticses[idx].begin(); it != _athleticses[idx].end(); ++ it)
        {
            std::deque<AthleticsEventData*>::reverse_iterator rit = _athleticsesEvent[idx].rbegin();
            Stream st(REP::FIGHT_INFO_CHANGE);
            st << static_cast<UInt16>(0x10);
            st << Stream::eos;
            (*it)->ranker->send(st);
        }
    }
}

void AthleticsRank::requestAthleticsEvent(Stream& st, Player * player)
{
	UInt8 row = getRankRow(player->GetLev());
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return;

    Rank ranker = found->second;
    if(ranker == _athleticses[row].end())
        return;

    UInt8 count = 15;
    if (count > static_cast<UInt16>(_athleticsesEvent[row].size()))
        count = static_cast<UInt16>(_athleticsesEvent[row].size());

    std::deque<AthleticsEventData*>::reverse_iterator rit = _athleticsesEvent[row].rbegin();
    st << count;
    for (UInt16 i = 0; rit != _athleticsesEvent[row].rend() && i < count; ++rit, ++i)
    {
        if((*rit)->player1)
            st << (*rit)->player1->getName() << (*rit)->player1->getCountry();
        else
            st << "" << static_cast<UInt8>(0);
        if((*rit)->player2)
            st << (*rit)->player2->getName() << (*rit)->player2->getCountry();
        else
            st << "" << static_cast<UInt8>(0);

        st <<  (*rit)->cond << (*rit)->value << (*rit)->itemCount << (*rit)->itemId << (*rit)->color;
    }
}

bool AthleticsRank::addAthleticsEventDataFromDB(UInt8 row, UInt32 id, Player* player1, Player* player2, UInt8 cond, UInt8 color, UInt16 value, UInt8 itemCount, UInt32 itemId, UInt32 time)
{
    if(row > 1)
        return false;

	if (_athleticsesEvent[row].size() > 14)
		return false;

	AthleticsEventData* data = new AthleticsEventData(id, player1, player2, cond, color, value, itemCount, itemId, time);
	_athleticsesEvent[row].push_front(data);
	return true;
}

UInt32 AthleticsRank::addAthleticsEventData(UInt8 row, Player* player1, Player* player2, UInt8 cond, UInt8 color, UInt16 value, UInt8 itemCount, UInt32 itemId)
{
	AthleticsEventData * data = new(std::nothrow) AthleticsEventData();
	if (data == NULL)
		return -1;
	data->id = IDGenerator::gAthleticsRecordOidGenerator.ID();
	data->player1 = player1;
	data->player2 = player2;
	data->cond = cond;
	data->color = color;
    data->value = value;
	data->itemCount = itemCount;
    data->itemId = itemId;
	data->time = TimeUtil::Now();
	if (_athleticsesEvent[row].size() >= 15)
	{
		AthleticsEventData * del = _athleticsesEvent[row].front();
        DB6().PushUpdateData("DELETE FROM `athletics_event` WHERE `id` = %u", del->id);
		SAFE_DELETE(del);
		_athleticsesEvent[row].pop_front();
	}
	_athleticsesEvent[row].push_back(data);
	DB6().PushUpdateData("INSERT INTO `athletics_event` (`id`, `row`, `player1`, `player2`, `cond`, `color`, `value`, `itemcount`, `itemid`, `time`) VALUES(%u, %"I64_FMT"u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u)", data->id, row, player1 ? player1->getId() : 0, player2 ? player2->getId() : 0, data->cond, data->color, data->value, data->itemCount, data->itemId, data->time);

    broadcastAthleticsEvent(15);

	return data->id;
}

UInt32 AthleticsRank::getAthleticsFirst4Rank(Player* player, UInt32 first4rank)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;

    if( first4rank & 0x2000 )
        (*found->second)->first4rank &= 0xFFFFEFFF;
    else if( first4rank & 0x1000 )
        (*found->second)->first4rank &= 0xFFFFDFFF;

	if (TimeUtil::SharpDay(0, (*found->second)->challengetime) != WORLD().ThisDay())
    {
        (*found->second)->first4rank &= 0xFFFFB0FF;
		DB6().PushUpdateData("UPDATE `athletics_rank` SET `first4rank` = %u WHERE `ranker` = %"I64_FMT"u", (*found->second)->first4rank, (*found->second)->ranker->getId());
    }

	return (*found->second)->first4rank & first4rank;
}

UInt32 AthleticsRank::setAthleticsFirst4Rank(Player* player, UInt32 first4rank)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;

	(*found->second)->first4rank |= first4rank;
    DB6().PushUpdateData("UPDATE `athletics_rank` SET `first4rank` = %u WHERE `ranker` = %"I64_FMT"u", (*found->second)->first4rank, (*found->second)->ranker->getId());

	return (*found->second)->first4rank;
}

UInt32 AthleticsRank::getAthleticsExtraChallenge(Player* player)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;

    return (*found->second)->extrachallenge;
}

UInt32 AthleticsRank::setAthleticsExtraChallenge(Player* player, UInt32 extrachallenge)
{
    UInt8 row = getRankRow(player->GetLev());
	if (row == 0xFF)
		return 0;
	RankList::iterator found = _ranks[row].find(player);
	if (found == _ranks[row].end())
		return 0;

    if(getRankPos(row, found->second) < (static_cast<UInt32>(0x7FFFFFFF) & extrachallenge) + 1)
        return 0;

    if(extrachallenge)
    {
        if( ((*found->second)->first4rank & 0x4000) && !(static_cast<UInt32>(0x80000000) & extrachallenge) )
            return 0;
    }


    (*found->second)->extrachallenge = extrachallenge;
    DB6().PushUpdateData("UPDATE `athletics_rank` SET `extrachallenge` = %u WHERE `ranker` = %"I64_FMT"u", (*found->second)->extrachallenge, (*found->second)->ranker->getId());

    if(extrachallenge && !(static_cast<UInt32>(0x80000000) & extrachallenge))
    {
        UInt8 color = 0;
        if((*found->second)->first4rank & 0x100)
            color = 5;
        else if((*found->second)->first4rank & 0x200)
            color = 4;
        else if((*found->second)->first4rank & 0x400)
            color = 3;
        else if((*found->second)->first4rank & 0x800)
            color = 2;

        Stream st(REP::FIGHT_INFO_CHANGE);
        st << static_cast<UInt16>(0) << color << Stream::eos;
        player->send(st);
    }

    return (*found->second)->extrachallenge;
}

UInt32 AthleticsRank::RunAthleticsEvent(UInt8 row, Rank atkRank, Rank defRank, UInt8 win)
{
    Player* player1 = NULL;
    Player* player2 = NULL;
    Player* atker = NULL;
    Player* defer = NULL;
    if(atkRank != _athleticses[row].end())
        atker = (*(atkRank))->ranker;
    if(defRank != _athleticses[row].end())
        defer = (*(defRank))->ranker;

    UInt8 cond = 0;
    UInt8 color = 0;
    UInt16 value = 0;
    UInt32 itemId = 0;
    UInt8 itemCount = 0;
    UInt32 prestige = 0;

    if(win == 2 && row == 1 && getAthleticsExtraChallenge(atker) != 0) //放弃特殊挑战
    {
         setAthleticsExtraChallenge(atker, 0);
    }
    else if( win == 1 )
    {
        if( 0 != getAthleticsFirst4Rank( defer, 0x2000 ) && getRankPos(row, defRank) > 10 )
        {
            setAthleticsFirst4Rank( defer, 0x1000 );
        }

        prestige = (10 * (World::_wday == 1 ? 2 : 1));
        UInt8 winStreakPrestige = 0;
        UInt8 endWinStreakPrestige = 0;

        if(getAthleticsWinStreak(atker) > 1)
        {
            winStreakPrestige = (getAthleticsWinStreak(atker)-1) * 2;
            if(winStreakPrestige > 20)
                winStreakPrestige = 20;
            prestige += winStreakPrestige;
        }
        if(getAthleticsWinStreak(defer) > 1)
        {
            endWinStreakPrestige = (getAthleticsWinStreak(defer)-1) * 2;
            if(endWinStreakPrestige > 20)
                endWinStreakPrestige = 20;
            prestige += endWinStreakPrestige;
        }

        player1 = atker;
        player2 = defer;

        if( row == 1 && 0 != getAthleticsFirst4Rank(atker, 0x100) && (getAthleticsExtraChallenge(atker) & static_cast<UInt32>(0x80000000)) )
        {
            //特殊挑战胜利 一天连升200个排名
            cond = 16;
            color = 5;
            value = getAthleticsExtraChallenge(atker);
            itemId = 25;
            itemCount = 3;
            setAthleticsExtraChallenge(atker, 0);
        }
        else if( 1 == getRankPos(row, atkRank) && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x1) )   //第一次成为竞技场第一
        {
            cond = 1;
            itemId = 25;
            itemCount = 1;
            setAthleticsFirst4Rank(atker, 0x1);
        }
        else if( getAthleticsRankUpADay(atker) > 199 && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x100) )   //一天内提升200个排名
        {
            cond = 15;
            UInt32 first4rank = 0x100;
            setAthleticsFirst4Rank(atker, first4rank);
            if(0 == getAthleticsFirst4Rank(atker, 0x4000))
            {
                color = 5;
                setAthleticsExtraChallenge(atker, getRankPos(row, atkRank)*0.5);
            }
            value = getAthleticsRankUpADay(atker);
            itemId = 25;
            itemCount = 1;
        }
        else if( row == 1 && 0 != getAthleticsFirst4Rank(atker, 0x200) && (getAthleticsExtraChallenge(atker) & static_cast<UInt32>(0x80000000)) )
        {
            //特殊挑战胜利 一天连升100个排名
            cond = 16;
            color = 4;
            value = getAthleticsExtraChallenge(atker);
            itemId = 25;
            itemCount = 1;
            setAthleticsExtraChallenge(atker, 0);
        }
        else if( getAthleticsWinStreak(atker) == 30 )   //30连胜
        {
            cond = 12;
            value = getAthleticsWinStreak(atker);
            itemId = 25;
            itemCount = 1;
        }
        else if( getAthleticsRankUpADay(atker) > 99 && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x200) )  //一天内提升100个排名
        {
            cond = 15;
            UInt32 first4rank = 0x200;
            setAthleticsFirst4Rank(atker, first4rank);
            if(0 == getAthleticsFirst4Rank(atker, 0x4000))
            {
                color = 4;
                setAthleticsExtraChallenge(atker, getRankPos(row, atkRank)*0.7);
            }
            value = getAthleticsRankUpADay(atker);
            itemId = 24;
            itemCount = 1;
        }
        else if( 2 == getRankPos(row, atkRank) && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x2) )     //第一次杀入竞技场二强
        {
            cond = 3;
            itemId = 24;
            itemCount = 1;
            setAthleticsFirst4Rank(atker, 0x2);
        }
        else if( 3 == getRankPos(row, atkRank) && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x4) )    //第一次杀入竞技场三强
        {
            cond = 5;
            itemId = 24;
            itemCount = 1;
            setAthleticsFirst4Rank(atker, 0x4);
        }
        else if( 11 > getRankPos(row, atkRank) && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x8) )
        {
            //第一次杀入竞技场10强
            cond = 7;
            itemId = 24;
            itemCount = 1;
            setAthleticsFirst4Rank(atker, 0x2008);
        }
        else if( (101 > getRankPos(row, atkRank)) && 10 < getRankPos(row, atkRank) && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x10) )
        {
            //第一次杀入竞技场100强
            cond = 9;
            itemId = 24;
            value = 100;
            itemCount = 1;
            setAthleticsFirst4Rank(atker, 0x10);
        }
        else if( (201 > getRankPos(row, atkRank)) && (100 < getRankPos(row, atkRank)) && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x20) )
        {
            //第一次杀入竞技场200强
            cond = 9;
            value = 200;
            itemId = 24;
            itemCount = 1;
            setAthleticsFirst4Rank(atker, 0x20|0x40);
        }
        else if( getAthleticsWinStreak(atker) == 20 )   //15连胜
        {
            cond = 12;
            value = getAthleticsWinStreak(atker);
            itemId = 24;
            itemCount = 1;
        }
        else if( getAthleticsWinStreak(defer) > 19 )   //终结了XXX的(20+)连胜
        {
            cond = 11;
            value = getAthleticsWinStreak(defer);
            itemId = 24;
            itemCount = 2;
        }
        else if( getAthleticsWinStreak(defer) > 9 )    //终结了XXX的(10~19)连胜
        {
            cond = 11;
            value = getAthleticsWinStreak(defer);
            itemId = 24;
            itemCount = 1;
        }
        else if( row == 1 && 0 != getAthleticsFirst4Rank(atker, 0x400) && (getAthleticsExtraChallenge(atker) & static_cast<UInt32>(0x80000000)) )
        {
            //特殊挑战胜利 一天连升50个排名
            cond = 16;
            color = 3;
            value = getAthleticsExtraChallenge(atker);
            itemId = 24;
            itemCount = 1;
            setAthleticsExtraChallenge(atker, 0);
        }
        else if( getAthleticsRankUpADay(atker) > 49 && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x400) )     //一天内提升50个排名
        {
            cond = 15;
            UInt32 first4rank = 0x400;
            setAthleticsFirst4Rank(atker, first4rank);
            if(0 == getAthleticsFirst4Rank(atker, 0x4000))
            {
                color = 3;
                setAthleticsExtraChallenge(atker, getRankPos(row, atkRank)*0.9);
            }
            value = getAthleticsRankUpADay(atker);
            itemId = 23;
            itemCount = 1;
        }
        else if( row == 1 && 0 != getAthleticsFirst4Rank(atker, 0x800) && (getAthleticsExtraChallenge(atker) & static_cast<UInt32>(0x80000000)) )
        {
            //特殊挑战胜利 一天连升20个排名
            cond = 16;
            color = 2;
            value = getAthleticsExtraChallenge(atker);
            itemId = 23;
            itemCount = 1;
            setAthleticsExtraChallenge(atker, 0);
        }
        else if( (301 > getRankPos(row, atkRank)) && (200 < getRankPos(row, atkRank)) && row == 1 && (0 == getAthleticsFirst4Rank(atker, 0x40)) )
        {
            //第一次杀入竞技场300强
            cond = 9;
            value = 300;
            itemId = 23;
            itemCount = 1;
            setAthleticsFirst4Rank(atker, 0x40);
        }
        else if( getAthleticsWinStreak(atker) == 10 )    //10连胜
        {
            cond = 12;
            value = getAthleticsWinStreak(atker);
            itemId = 23;
            itemCount = 1;
        }
        else if( getAthleticsWinStreak(defer) > 4 )    //终结了XXX的(5~9)连胜
        {
            cond = 11;
            value = getAthleticsWinStreak(defer);
            itemId = 23;
            itemCount = 1;
        }
        else if( getAthleticsRankUpADay(atker) > 19 && row == 1 && 0 == getAthleticsFirst4Rank(atker, 0x800) )    //一天内提升20个排名
        {
            cond = 15;
            UInt32 first4rank = 0x800;
            setAthleticsFirst4Rank(atker, first4rank);
            if(0 == getAthleticsFirst4Rank(atker, 0x4000))
            {
                color = 2;
                setAthleticsExtraChallenge(atker, getRankPos(row, atkRank)*0.95);
            }
            value = getAthleticsRankUpADay(atker);
            itemId = 22;
            itemCount = 1;
        }
        else if( getAthleticsWinStreak(atker) == 5 )    //5连胜
        {
            cond = 12;
            value = getAthleticsWinStreak(atker);
            itemId = 22;
            itemCount = 1;
        }
        else if( getAthleticsWinStreak(defer) > 2 )    //终结了XXX的(3~4)连胜
        {
            cond = 11;
            value = getAthleticsWinStreak(defer);
            itemId = 22;
            itemCount = 1;
        }

        if( cond == 0 && (uRand(100) < 7 || World::_wday == 3) )    //意外之喜
        {
            cond = 17;
            static UInt8 extra_roll[4] = {0, 5, 30, 100};
            static UInt8 extra_award[4] = {25, 24, 23, 22};
            for( int i = 0; i < 4; ++i )
            {
                if(uRand(100) < extra_roll[i])
                    itemId = extra_award[i];
            }
            itemCount = 1;
        }
    }
    else
    {
        prestige = (5 * (World::_wday == 1 ? 2 : 1));

        player1 = defer;
        player2 = atker;

        if( row == 1 && getAthleticsExtraChallenge(atker) != 0 )    //特殊挑战失败
            setAthleticsExtraChallenge(atker, 0);
        else if( getAthleticsBeWinStreak(defer) == 10 )    // 连续被10人挑战而不败
        {
            cond = 13;
            value = 10;
            itemId = 22;
            itemCount = 1;
        }
        else if( getAthleticsBeWinStreak(defer) == 20 )    // 连续被20人挑战而不败
        {
            cond = 13;
            value = 20;
            itemId = 23;
            itemCount = 1;
        }
        else if( getAthleticsBeWinStreak(defer) == 100 )    // 连续被100人挑战而不败
        {
            cond = 13;
            value = 100;
            value = 20;
            itemId = 24;
            itemCount = 1;
        }
        else if( getAthleticsFailStreak(atker) == 25 )    //百折不挠
        {
            player1 = atker;
            player2 = defer;
            cond = 14;
            itemId = 23;
            itemCount = 1;
        }
    }

    if((getAthleticsExtraChallenge(atker) & static_cast<UInt32>(0x80000000)))
    {
        // 情况特殊挑战标识
        setAthleticsExtraChallenge(atker, 0);
    }

    if(cond == 0 && 0 == getAthleticsFirst4Rank(atker, 0xFFFFFFFF))  //第一次竞技场挑战
    {
        cond = 10;
        itemId = 22;
        itemCount = 1;
        setAthleticsFirst4Rank(atker, 0x80);
    }

    if( cond != 0 )
    {
        AthleticsAward atkerAthleticsAward = { 0, 0, 0, 0, 0, 0, 0, 0, itemId, itemCount };

        GameMsgHdr hdr2(0x217, player1->getThreadId(), player1, sizeof(AthleticsAward));
        GLOBAL().PushMsg(hdr2, &atkerAthleticsAward);

        addAthleticsEventData(row, player1, player2, cond, color, value, itemCount, itemId);
    }

    return prestige;
}

void AthleticsRank::updateAthleticsRank(AthleticsRankData* data)
{
    if (TimeUtil::SharpDay(0, data->challengetime) != WORLD().ThisDay())
    {
        data->extrachallenge = 0;
        data->oldrank = data->rank;
        data->first4rank &= 0xFFFFB0FF;
		DB6().PushUpdateData("UPDATE `athletics_rank` SET `oldrank` = %u, `first4rank` = %u, `extrachallenge` = %u WHERE `ranker` = %"I64_FMT"u", data->oldrank, data->first4rank, data->extrachallenge, data->ranker->getId());
    }

    data->challengetime = TimeUtil::Now();
}

void AthleticsRank::updateAthleticsMartial(Player* pl)
{
    if(pl == NULL)
        return;

    UInt64 idIdx[3] = {0};
    UInt8 level = pl->GetLev();

    GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(level);
    if(it == GObject::globalLevelsPlayers.end())
        return;

    GObject::LevelPlayers* lvPlayer = it->second;

    UInt32 size = lvPlayer->size();
    UInt32 size2 = size;
    UInt8 cnt = 1;
    while(size < 100 && (level - cnt) > 29)
    {
        GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(level - cnt);
        ++ cnt;
        if(it == GObject::globalLevelsPlayers.end())
            continue;
        GObject::LevelPlayers* lvPlayer2 = it->second;

        size += lvPlayer2->size();
    }

    if(size < 5)
    {
        int k = 0;
        for(int i = 0; i < cnt; ++i)
        {
            GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(level - i);
            if(it == GObject::globalLevelsPlayers.end())
                continue;
            GObject::LevelPlayers* lvPlayer2 = it->second;

            UInt32 size1 = lvPlayer2->size();
            for(UInt32 j = 0; j < size1; ++j)
            {
                if(i == 0 && pl->getId() == (*lvPlayer2)[j])
                    continue;
                idIdx[k] = (*lvPlayer2)[j];
                ++k;
                if(k > 2)
                    break;
            }
            if(k > 2)
                break;
        }
    }
    else
    {
        URandom rnd(time(NULL));

        UInt32 roll0 = rnd(size);
        while(size2 > roll0 && pl->getId() == (*lvPlayer)[roll0])
        {
            roll0 = (roll0 + 1) % size;
        }
        UInt32 roll1 = rnd(size);
        while((size2 > roll1 && pl->getId() == (*lvPlayer)[roll1]) || roll1 == roll0)
        {
            roll1 = (roll1 + 1) % size;
        }
        UInt32 roll2 = rnd(size);
        while((size2 > roll2 && pl->getId() == (*lvPlayer)[roll2]) || roll2 == roll0 || roll2 == roll1)
        {
            roll2 = (roll2 + 1) % size;
        }

        for(int i = 0; i < cnt; ++i)
        {
            GObject::GlobalLevelsPlayersIterator it = GObject::globalLevelsPlayers.find(level - i);
            if(it == GObject::globalLevelsPlayers.end())
                continue;
            GObject::LevelPlayers* lvPlayer2 = it->second;

            UInt32 size1 = lvPlayer2->size();
            if(size1 == 0)
                continue;

            if(idIdx[0] == 0)
            {
                if(roll0 < size1)
                {
                    idIdx[0] = (*lvPlayer2)[roll0];
                }
                else
                    roll0 -= size1;
            }

            if(idIdx[1] == 0)
            {
                if(roll1 < size1)
                {
                    idIdx[1] = (*lvPlayer2)[roll1];
                }
                else
                    roll1 -= size1;
            }

            if(idIdx[2] == 0)
            {
                if(roll2 < size1)
                {
                    idIdx[2] = (*lvPlayer2)[roll2];
                }
                else
                    roll2 -= size1;
            }
        }
    }

    for(int i = 0; i < 3; ++i)
    {
        if(idIdx[i] != 0)
        {
            Player* defer = globalPlayers[idIdx[i]];
            if(defer == NULL)
                continue;
            MartialHeader mh = {0};
            mh.owner = pl;
            mh.md.idx = i;
            mh.md.defer = defer;
            mh.md.canAttack = 0;
            GameMsgHdr hdr2(0x330, pl->getThreadId(), pl, sizeof(MartialHeader));
            GLOBAL().PushMsg(hdr2, &mh);
        }
    }
}

void AthleticsRank::giveAward( Player* pl )
{
	RankList::iterator found = _ranks[1].find(pl);
	if (found == _ranks[1].end())
		return;

    AthleticsRankData* rankData = *found->second;
    if(rankData->prestige == 0 && rankData->tael == 0)
        return;

    AthleticsAward atkerAthleticsAward = { 0, 0, 0, 0, rankData->prestige, rankData->tael, 0, 0, 0, 0};

    GameMsgHdr hdr2(0x217, pl->getThreadId(), pl, sizeof(AthleticsAward));
    GLOBAL().PushMsg(hdr2, &atkerAthleticsAward);

    rankData->prestige = 0;
    rankData->tael = 0;

    DB6().PushUpdateData("UPDATE `athletics_rank` SET `prestige` = %u, `tael` = %u WHERE `ranker` = %"I64_FMT"u", rankData->prestige, rankData->tael, rankData->ranker->getId());

    {
        Stream st(REP::FIGHT_INFO_CHANGE);
        st << static_cast<UInt16>(0x01);
        st << Stream::eos;
        pl->send(st);
    }

}

}


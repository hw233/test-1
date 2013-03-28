#include "Config.h"
#include "CountryBattle.h"
#include "NewCountryBattle.h"
#include "GObject/Country.h"
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Player.h"
#include "Map.h"
#include "Mail.h"
#include "World.h"
#include "Script/GameActionLua.h"
#include "Common/Serialize.h"
#include "Common/TimeUtil.h"
#include "SpecialAward.h"
#include "MsgID.h"
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include "Package.h"

namespace GObject
{

GlobalCountryBattle globalCountryBattle;

UInt32 CountryBattleData::getReward(UInt8 lvl, UInt32 curtime, UInt32 nextReward)
{
	if(nextReward == 0)
		return 0;
	UInt32 duration;
	if(joinTime + 60 > curtime)
	{
		if(joinTime >= curtime)
			return 0;
		duration = curtime - joinTime;
	}
	else
		duration = curtime + 60 - nextReward;
	UInt8 plvl = player->GetLev();
    /*
    UInt32 exp = 16 * ((plvl - 10) * ((plvl > 99 ? 99 : plvl) / 10) * 5 + 25);
    player->AddExp(exp);
    */
    if(plvl <= 90)
		player->AddExp(2 * (duration * ((plvl - 40) * 6 + 20)));
	else if(plvl <= 100)
		player->AddExp(2 * (duration * ((plvl - 90) * 22 + 320)));
	else if(plvl <= 114)
		player->AddExp(2 * (duration * ((plvl - 100) * 140 + 540)));
	else
		player->AddExp(2 * (duration * 2500));
	return duration;
}

void CountryBattleData::sendAchieveUpdate( UInt16 achieve )
{
	Stream st(REP::COUNTRY_WAR_PROCESS);
	st << static_cast<UInt8>(1) << achieve << Stream::eos;
	player->send(st);
}

int CountryBattle::top(UInt32 curtime, UInt8 lvl, UInt8 side)
{
	std::vector<CountryBattleData *>& blist = _players[lvl][side];
	size_t size = blist.size();
	for(size_t i = 0; i < size; ++ i)
	{
		CountryBattleData * cbd = blist[i];
		if(cbd->resetEndTime <= curtime)
		{
			cbd->resetEndTime = 0;
			return i;
		}
	}
	return -1;
}

inline void addAchievement(UInt32& achieve, UInt8 lvl, UInt8& killStreak1, UInt8& killStreak2)
{
    ++ killStreak1;
    switch(killStreak1)
    {
        case 1:
            achieve += 8 * (lvl + 1);
            break;
        case 2:
            achieve += 12 * (lvl + 1);
            break;
        default:
            achieve += 16 * (lvl + 1);
            break;
    }
    if(killStreak2 >= 5)
        achieve += 10 * (lvl + 1);
	if(World::_wday == 1)
		achieve *= 2;

    // XXX: 阵营战时间减半后处理
    achieve *= 2;
}

void CountryBattle::LeaveGetAttainment(Player* p, CBPlayerData& data)
{
    if(data.totalWin)
    {
        UInt32 w = p->GetVar( VAR_COUNTRY_BATTLE_WIN);

        GameAction()->doAttainment(  p  , Script:: COUNTRY_BATTLE_WIN, data.totalWin + w);
    }
    if(data.maxKillStreak)
     GameAction()->doAttainment(p, Script:: COUNTRY_BATTLE_KILL_STREAK, data.maxKillStreak);

}

void CountryBattle::process(UInt32 curtime)
{
	if(curtime >= _rewardTime)
	{
		if(_rewardTime != 0)
		{
			getReward(curtime);
		}
		_rewardTime = curtime + 60;
	}
	for(UInt8 lvl = 0; lvl < 3; ++ lvl)
	{
		while(1)
		{
			int pos1 = top(curtime, lvl, 0);
			if(pos1 < 0)
			{
				restCount(curtime, lvl, 1);
				break;
			}
			int pos2 = top(curtime, lvl, 1);
			if(pos2 < 0)
			{
				restCount(curtime, lvl, 0);
				break;
			}

			CountryBattleData * cbd1 = _players[lvl][0][pos1], * cbd2 = _players[lvl][1][pos2];

			int turns = 0;
			bool res = cbd1->player->challenge(cbd2->player, NULL, &turns, false, 50);

			GameAction()->RunOperationTaskAction1(cbd1->player, 2, res);
			GameAction()->RunOperationTaskAction1(cbd2->player, 2, !res);
			cbd1->player->setBuffData(PLAYER_BUFF_ATTACKING, curtime + 2 * turns);
			cbd2->player->setBuffData(PLAYER_BUFF_ATTACKING, curtime + 2 * turns);

			UInt8 gotScore = 8 * (lvl + 1);
			UInt32 achieve = 0, loserAchieve = 2 * (lvl + 1);
			if(World::_wday == 1)
				loserAchieve *= 2;

            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA(cbd1->player, created));
            if(thisDay <= endDay)
            {
                UInt32 targetVal = cbd1->player->GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_COUNTRY_BATTLE))
                {
                    targetVal |=TARGET_COUNTRY_BATTLE;
                    cbd1->player->AddVar(VAR_CTS_TARGET_COUNT, 1);
                    cbd1->player->SetVar(VAR_CLAWARD2, targetVal);
                    cbd1->player->sendNewRC7DayTarget();
                    cbd1->player->newRC7DayUdpLog(1152, 4);
                }
            }

            endDay = TimeUtil::SharpDay(6, PLAYER_DATA(cbd2->player, created));
            if(thisDay <= endDay)
            {
                UInt32 targetVal = cbd2->player->GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_COUNTRY_BATTLE))
                {
                    targetVal |=TARGET_COUNTRY_BATTLE;
                    cbd2->player->AddVar(VAR_CTS_TARGET_COUNT, 1);
                    cbd2->player->SetVar(VAR_CLAWARD2, targetVal);
                    cbd2->player->sendNewRC7DayTarget();
                    cbd2->player->newRC7DayUdpLog(1152, 4);
                }
            }

			if(res)
			{
				CBStatusData data;
				if(!_firstBlooded)
				{
					_firstBlooded = true;
					data.type = 5;
					achieve += 10 * (lvl + 1);
				}
				else
					data.type = 4;

				addAchievement(achieve, lvl, cbd1->killStreak, cbd2->killStreak);
				if(cbd1->killStreak > _topStreak)
				{
					_topKiller = cbd1->player;
					_topStreak = cbd1->killStreak;
				}
				_score[0] += gotScore;

				data.setSideLevel(0, lvl);
				data.player = cbd1->player;
				data.streak = cbd1->killStreak;
				data.score = achieve;
				data.playerOther = cbd2->player;
				data.streakOther = cbd2->killStreak;
				_cbsdlist.push_back(data);

				if(cbd1->player->isOnline())
					cbd1->player->pendAchievement(achieve);
				else
					cbd1->player->getAchievement(achieve);
				if(cbd2->player->isOnline())
					cbd2->player->pendAchievement(loserAchieve);
				else
					cbd2->player->getAchievement(loserAchieve);

				rotate(curtime + turns * 2, lvl, 0, pos1, achieve, 2);

				UInt32 viplvl = cbd2->player->getVipLevel();
				if(viplvl >= 4 || cbd2->player->inVipPrivilegeTime())
				{
					rotate(curtime + turns * 2, lvl, 1, pos2, loserAchieve, 1);
					cbd2->killStreak = 0;

					CBStatusData data;
					data.type = 0;
					data.setSideLevel(1, lvl);
					data.player = cbd2->player;
					_cbsdlist.push_back(data);
				}
				else
				{
					doLeave(curtime, lvl, 1, pos2, loserAchieve, 1);
				}

				if(cbd1->killStreak >= 10 && World::_actAvailable && gSpecialAward.getAwardSize(0) < 3)
					gSpecialAward.AddSpecialAwardList(0, cbd1->player);

                cbd1->player->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 1);
                cbd1->player->OnShuoShuo(SS_CTRYBATTLE);
                if (cbd1->killStreak == 2)
                    cbd1->player->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 2);
			}
			else
			{
				CBStatusData data;
				if(!_firstBlooded)
				{
					_firstBlooded = true;
					data.type = 5;
					achieve += 10 * (lvl + 1);
				}
				else
					data.type = 4;

				addAchievement(achieve, lvl, cbd2->killStreak, cbd1->killStreak);
				if(cbd2->killStreak > _topStreak)
				{
					_topKiller = cbd2->player;
					_topStreak = cbd2->killStreak;
				}
				_score[1] += gotScore;

				data.setSideLevel(1, lvl);
				data.player = cbd2->player;
				data.streak = cbd2->killStreak;
				data.score = achieve;
				data.playerOther = cbd1->player;
				data.streakOther = cbd1->killStreak;
				_cbsdlist.push_back(data);

				if(cbd2->player->isOnline())
					cbd2->player->pendAchievement(achieve);
				else
					cbd2->player->getAchievement(achieve);
				if(cbd1->player->isOnline())
					cbd1->player->pendAchievement(loserAchieve);
				else
					cbd1->player->getAchievement(loserAchieve);

				rotate(curtime + turns * 2, lvl, 1, pos2, achieve, 2);

				UInt32 viplvl = cbd1->player->getVipLevel();
				if(viplvl >= 4 || cbd1->player->inVipPrivilegeTime())
				{
					rotate(curtime + turns * 2, lvl, 0, pos1, loserAchieve, 1);
					cbd1->killStreak = 0;

					CBStatusData data;
					data.type = 0;
					data.setSideLevel(0, lvl);
					data.player = cbd1->player;
					_cbsdlist.push_back(data);
				}
				else
				{
					doLeave(curtime, lvl, 0, pos1, loserAchieve, 1);
				}

				if(cbd2->killStreak >= 10 && World::_actAvailable && gSpecialAward.getAwardSize(0) < 3)
					gSpecialAward.AddSpecialAwardList(0, cbd2->player);

                cbd2->player->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 1);
                cbd2->player->OnShuoShuo(SS_CTRYBATTLE);
                if (cbd2->killStreak == 2)
                    cbd2->player->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 2);
			}
		}
	}
	if(!_cbsdlist.empty())
	{
		CBStatusDataList::iterator it;
		Stream st(REP::COUNTRY_WAR_STRING);
		st << _score[0] << _score[1] << static_cast<UInt16>(0);
		UInt16 proc_count = 0;
		for(it = _cbsdlist.begin(); it != _cbsdlist.end(); ++ it)
		{
			padPlayerData(st, *it);
			++ proc_count;
		}
		st.data<UInt16>(12) = proc_count;
		st << Stream::eos;
		broadcast(st);
		_cbsdlist.clear();
	}
}

void CountryBattle::prepare(UInt32 rt)
{
	_topKiller = NULL;
	_topStreak = 0;
	_firstBlooded = false;
	_score[0] = 0;
	_score[1] = 0;
	_rewardTime = 0;
	_battleDuration[0].clear();
	_battleDuration[1].clear();
    _cbsdlist.clear();
	for(UInt8 lvl = 0; lvl < 3; ++ lvl)
	{
		for(UInt8 side = 0; side < 2; ++ side)
		{
			_players[lvl][side].clear();
		}
	}
	Stream st(REP::COUNTRY_WAR_PROCESS);
	st << static_cast<UInt8>(2) << static_cast<UInt8>(0) << rt << Stream::eos;
	broadcast(st);
}

void CountryBattle::start(UInt32 rt)
{
	Stream st(REP::COUNTRY_WAR_PROCESS);
	st << static_cast<UInt8>(2) << static_cast<UInt8>(1) << rt << Stream::eos;
	NETWORK()->Broadcast(st);
}

struct _rankCompare
{
	bool operator() (const CBPlayerData * pd1, const CBPlayerData * pd2) const
	{
		if(pd1->totalWin == pd2->totalWin)
		{
			if(pd1->maxKillStreak == pd2->maxKillStreak)
			{
				return pd1->totalAchievement > pd2->totalAchievement;
			}
			return pd1->maxKillStreak > pd2->maxKillStreak;
		}
		return pd1->totalWin > pd2->totalWin;
	}
};

void CountryBattle::end(UInt32 curtime)
{
	UInt32 rewardid[2];
	UInt32 mailid[2];
	if(_score[0] == _score[1])
	{
		_owner = 0;
		rewardid[0] = PLAYER_BUFF_TRAINP1;
		rewardid[1] = PLAYER_BUFF_TRAINP1;
		mailid[0] = 264;
		mailid[1] = 264;
		SYSMSG_BROADCASTV(259, _spot);
	}
	else if(_score[0] > _score[1])
	{
		_owner = 1;
		rewardid[0] = PLAYER_BUFF_TRAINP2;
		rewardid[1] = PLAYER_BUFF_TRAINP1;
		mailid[0] = 262;
		mailid[1] = 263;
		SYSMSG_BROADCASTV(260, _spot, 0);
	}
	else
	{
		_owner = 2;
		rewardid[0] = PLAYER_BUFF_TRAINP1;
		rewardid[1] = PLAYER_BUFF_TRAINP2;
		mailid[0] = 263;
		mailid[1] = 262;
		SYSMSG_BROADCASTV(260, _spot, 1);
	}

	SYSMSG(title, 261);

	for(UInt8 lvl = 0; lvl < 3; ++ lvl)
	{
		for(UInt8 side = 0; side < 2; ++ side)
		{
			std::vector<CountryBattleData *>& blist = _players[lvl][side];
			size_t size = blist.size();
			for(size_t i = 0; i < size; ++ i)
			{
				UInt32 dur = blist[i]->getReward(lvl, curtime, _rewardTime);
				Player * player = blist[i]->player;
				if(dur > 0)
				{
					CBPlayerData& cbpdata = _battleDuration[player->getCountry()][player];
					cbpdata.awardTime += (dur << 1);
				}
				player->delFlag(Player::CountryBattle);
				player->autoRegenAll();
				delete blist[i];
			}
			blist.clear();
		}
	}
	UInt32 totalAchievement[3] = {0};
	UInt32 maxAchievement[3] = {0};
	UInt64 maxPlayer[3] = {0};
	UInt16 enterSize[3] = {0};

	std::map<CBPlayerData *, Player *, _rankCompare> _rank;
	for(UInt8 side = 0; side < 2; ++ side)
	{
		for(std::map<Player *, CBPlayerData>::iterator it = _battleDuration[side].begin(); it != _battleDuration[side].end(); ++ it)
		{
			if(World::_activityStage > 0)
				GameAction()->onCountryBattleAttend(it->first);
			UInt32 awardTime = it->second.awardTime;

            UInt16 count = awardTime / 1800 + ((awardTime % 1800) ? 1: 0);
            if (rewardid[side] == PLAYER_BUFF_TRAINP1 && count)
            {
                MailPackage::MailItem mitem[1] = {{55, count}};
                MailItemsInfo itemsInfo(mitem, CountryBattleAward, 1);

                SYSMSGV(content, mailid[side], it->second.totalAchievement, count, it->second.totalWin, it->second.totallose, it->second.maxKillStreak);
                Mail * pmail = it->first->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(pmail != NULL)
                    mailPackageManager.push(pmail->id, mitem, 1, true);
                it->first->countryBattleUdpLog(1091, it->first->getCountry());
            }
            else if (rewardid[side] == PLAYER_BUFF_TRAINP2 && count)
            {
                MailPackage::MailItem mitem[1] = {{56, count}};
                MailItemsInfo itemsInfo(mitem, CountryBattleAward, 1);

                SYSMSGV(content, mailid[side], it->second.totalAchievement, count, it->second.totalWin, it->second.totallose, it->second.maxKillStreak);
                Mail * pmail = it->first->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(pmail != NULL)
                    mailPackageManager.push(pmail->id, mitem, 1, true);
                it->first->countryBattleUdpLog(1091, it->first->getCountry());
            }

			/*for back stage*/
			UInt8 lvl = getJoinLevel(it->first->GetLev());
			enterSize[lvl] ++;
			totalAchievement[lvl] += it->second.totalAchievement;
			if(maxAchievement[lvl] < it->second.totalAchievement)
			{
				maxAchievement[lvl] = it->second.totalAchievement;
				maxPlayer[lvl] = it->first->getId();
			}
			_rank[&it->second] = it->first;
            LeaveGetAttainment(it->first, it->second);
		}
	}
	DBLOG1().PushUpdateData("insert into `country_battle`(`server_id`, `total_achievement1`, `total_players1`, `total_achievement2`, `total_players2`, `total_achievement3`, `total_players3`, `max_player1`, `max_achievement1`, `max_player2`, `max_achievement2`, `max_player3`, `max_achievement3`, `created_at`) values(%u, %u, %u, %u, %u, %u, %u, %"I64_FMT"u, %u, %"I64_FMT"u, %u, %"I64_FMT"u, %u, %u)", cfg.serverLogId, totalAchievement[0], enterSize[0], totalAchievement[1], enterSize[1], totalAchievement[2], enterSize[2], maxPlayer[0], maxAchievement[0], maxPlayer[1], maxAchievement[1], maxPlayer[2], maxAchievement[2], TimeUtil::Now());

	_lastReport.init(REP::COUNTRY_WAR_RESULT);
	_lastReport << _spot << _score[0] << _score[1] << _owner << static_cast<UInt16>(0);
	int i = 0;
	for(std::map<CBPlayerData *, Player *, _rankCompare>::iterator it = _rank.begin(); it != _rank.end() && i < 5; ++ it, ++ i)
	{
		_lastReport << it->second->getCountry() << it->second->getName() << it->second->GetLev() << it->first->totalWin << it->first->maxKillStreak << it->second->getClanName() << it->first->totalAchievement;
	}
	_lastReport << Stream::eos;

	bool cansend = _lastReport.size() >= 16;
	UInt8 rnk = 0;
	for(std::map<CBPlayerData *, Player *, _rankCompare>::iterator it = _rank.begin(); it != _rank.end(); ++ it)
	{
		++ rnk;
		Player * pl = it->second;
		it->first->rank = rnk;
		if(cansend && pl->getLocation() == _spot)
		{
			_lastReport.data<UInt16>(15) = rnk;
			pl->send(_lastReport);
		}
	}
}

void CountryBattle::rotate( UInt32 curtime, UInt8 lvl, UInt8 side, int pos, UInt16 achievement, UInt8 flag)
{
	std::vector<CountryBattleData *>& blist = _players[lvl][side];
	if(static_cast<size_t>(pos) >= blist.size())
		return;

	CountryBattleData * cbd = blist[pos];
	cbd->restCountTime = 0;

	CBPlayerData& cbpdata = _battleDuration[cbd->player->getCountry()][cbd->player];
	cbpdata.totalAchievement += achievement;
	cbd->sendAchieveUpdate(cbpdata.totalAchievement);
	if(flag == 2)
	{
        cbd->resetEndTime = curtime + 25;
		cbpdata.totalWin ++;
		cbpdata.currKillStreak = cbd->killStreak;
		if(cbpdata.currKillStreak > cbpdata.maxKillStreak)
			cbpdata.maxKillStreak = cbpdata.currKillStreak;
	}
	else if(flag == 1)
	{
        cbd->resetEndTime = curtime + 30;
		cbpdata.totallose ++;
	}

	blist.erase(blist.begin() + pos);
	blist.push_back(cbd);
}

bool CountryBattle::playerEnter( Player * player )
{
    if (player && player->getCountry() >= COUNTRY_NEUTRAL)
        return false;
	if(!globalCountryBattle.isRunning())
		return false;
	UInt8 plvl = player->GetLev();
	if(plvl < 40)
		return false;
	UInt32 curtime = TimeUtil::Now();
	if(player->getBuffData(PLAYER_BUFF_WEAK, curtime))
	{
		player->sendMsgCode(0, 1401);
		return false;
	}
    // XXX: yangyoufa@ 08/01/12 21:15:02
#if 0
	if(player->getBuffData(PLAYER_BUFF_ATTACKING, curtime))
	{
		player->sendMsgCode(0, 1412);
		return false;
	}
#endif
	UInt8 lvl, side;
    int pos = findPlayer(player, lvl, side);
    if(pos >= 0)
        return true;
    lvl = getJoinLevel(plvl);
    CountryBattleData * data = new(std::nothrow) CountryBattleData;
	if(data == NULL)
		return false;
	data->player = player;
	data->joinTime = curtime;
	data->resetEndTime = curtime + 30;
	_players[lvl][side].push_back(data);
	player->addFlag(Player::CountryBattle);

	CBStatusData cbsd;
	cbsd.type = 0;
	cbsd.setSideLevel(side, lvl);
	cbsd.player = player;
	Stream st(REP::COUNTRY_WAR_STRING);
	st << _score[0] << _score[1] << static_cast<UInt16>(1);
	padPlayerData(st, cbsd);
	st << Stream::eos;
	broadcast(st);

	SYSMSG_SEND(141, player);
	SYSMSG_SENDV(1041, player, player->getCountry());

    GameAction()->doStrong(player, SthCountryWar, 0, 0);
    player->OnHeroMemo(MC_ATHLETICS, MD_ADVANCED, 0, 0);
    if (World::getWhiteLoveDay() && World::_wday == 7 && !player->GetVar(VAR_CBWHITEPACK))
    {
        player->GetPackage()->AddItem2(476, 5, 1, 1);
        player->SetVar(VAR_CBWHITEPACK, 1);
    }
    player->countryBattleUdpLog(1090, 2);
	return true;
}

void CountryBattle::playerLeave( Player * player )
{
    if (player && player->getCountry() >= COUNTRY_NEUTRAL)
        return;
	if(!globalCountryBattle.isRunning())
		return;
	UInt8 lvl, side;
	int pos = findPlayer(player, lvl, side);
	if(pos < 0)
		return;

	std::vector<CountryBattleData *>& blist = _players[lvl][side];
	if(static_cast<size_t>(pos) >= blist.size())
		return;

	UInt32 curtime = TimeUtil::Now();
	if(curtime >= globalCountryBattle.getStartTime())
		player->setBuffData(PLAYER_BUFF_WEAK, curtime + 5 * 60);

	CBStatusData cbsd;
	cbsd.type = 1;
	cbsd.setSideLevel(side, lvl);
	cbsd.player = player;
	Stream st(REP::COUNTRY_WAR_STRING);
	st << _score[0] << _score[1] << static_cast<UInt16>(1);
	padPlayerData(st, cbsd);
	st << Stream::eos;
	broadcast(st);

	doLeave(curtime, lvl, side, pos);
	player->autoRegenAll();
	SYSMSG_SEND(142, player);
	SYSMSG_SEND(1042, player);
}

void CountryBattle::doLeave( UInt32 curtime, UInt8 lvl, UInt8 side, int pos, UInt16 achievement, UInt8 loseFlag )
{
	std::vector<CountryBattleData *>& blist = _players[lvl][side];
	if(static_cast<size_t>(pos) >= blist.size())
		return;
	CountryBattleData * cbd = blist[pos];
	UInt32 dur = cbd->getReward(lvl, curtime, _rewardTime);
	Player * player = cbd->player;

	CBPlayerData& cbpdata = _battleDuration[player->getCountry()][player];
	if(loseFlag != 0)
	{
		cbpdata.totallose ++;
		cbpdata.totalAchievement += achievement;
		cbd->sendAchieveUpdate(cbpdata.totalAchievement);
	}
	if(dur > 0)
		cbpdata.awardTime += (dur << 1);

	player->delFlag(Player::CountryBattle);

   // printf("离开阵营战 ： killall %u , streak : %u\n",  cbpdata.totalWin, cbpdata.maxKillStreak);
    LeaveGetAttainment(player,  cbpdata);
	blist.erase(blist.begin() + pos);
	delete cbd;
}

int CountryBattle::findPlayer( Player * player, UInt8& lvl, UInt8& side )
{
	side = PLAYER_DATA(player, country);
    if (side >= COUNTRY_NEUTRAL) // XXX: no useful?
        return -1;
	int lvlend = getJoinLevel(player->GetLev());
	for(lvl = 0; lvl <= lvlend; ++ lvl)
	{
		std::vector<CountryBattleData *>& blist = _players[lvl][side];
		size_t size = blist.size();
		for(size_t i = 0; i < size; ++ i)
		{
			if(blist[i]->player == player)
			{
				return i;
			}
		}
	}
	lvl = 0;
	return -1;
}

void CountryBattle::restCount( UInt32 curtime, UInt8 lvl, UInt8 side )
{
	std::vector<CountryBattleData *>& blist = _players[lvl][side];
	size_t size = blist.size();
	for(size_t i = 0; i < size; ++ i)
	{
		CountryBattleData * cbd = blist[i];
		if(cbd->resetEndTime > 0 && cbd->resetEndTime <= curtime)
		{
			cbd->restCountTime = cbd->resetEndTime + 60;
			cbd->resetEndTime = 0;

			CBStatusData data;
			data.type = 2;
			data.setSideLevel(side, lvl);
			data.player = cbd->player;
			_cbsdlist.push_back(data);
		}
		if(cbd->restCountTime <= curtime && cbd->resetEndTime == 0)
		{
			Player * player = cbd->player;
			UInt8 gotScore = lvl + 1;
			UInt8 achievement = 2 * (lvl + 1);
			if(World::_wday == 1)
				achievement *= 2;
			cbd->restCountTime = curtime + 60;
			_score[side] += gotScore;
			cbd->player->getAchievement(achievement);

			CBPlayerData& cbpdata = _battleDuration[player->getCountry()][player];
			cbpdata.totalAchievement += achievement;
			cbd->sendAchieveUpdate(cbpdata.totalAchievement);

			CBStatusData data;
			data.type = 3;
			data.setSideLevel(side, lvl);
			data.player = cbd->player;
			data.score = achievement;
			_cbsdlist.push_back(data);
		}
	}
}

void CountryBattle::padPlayerData( Stream& st, UInt8 lvl )
{
	UInt32 curtime = TimeUtil::Now();
#if 0
	bool ishigh = lvl > 0;
	padPlayerData(st, curtime, 0, 0, !ishigh);
	padPlayerData(st, curtime, 1, 0, ishigh);
	padPlayerData(st, curtime, 0, 1, !ishigh);
	padPlayerData(st, curtime, 1, 1, ishigh);
#endif
    st << static_cast<UInt8>(3);
	padPlayerData(st, curtime, 0, 0, true);
	padPlayerData(st, curtime, 1, 0, true);
	padPlayerData(st, curtime, 2, 0, true);
    st << static_cast<UInt8>(3);
	padPlayerData(st, curtime, 0, 1, true);
	padPlayerData(st, curtime, 1, 1, true);
	padPlayerData(st, curtime, 2, 1, true);
}

void CountryBattle::padPlayerData( Stream& st, UInt32 curtime, UInt8 lvl, UInt8 side, bool fulllist )
{
	std::vector<CountryBattleData *>& blist = _players[lvl][side];
	std::vector<CountryBattleData *>::iterator it;
    st << lvl;
	UInt16 size = static_cast<UInt16>(blist.size());
	if(fulllist)
	{
		st << size;
		for(UInt16 i = 0; i < size; ++ i)
		{
			CountryBattleData * cbd = blist[i];
			Player * player = cbd->player;
			st << player->getName() << static_cast<UInt8>(cbd->resetEndTime < curtime ? 1 : 0);
		}
	}
	else
	{
		st << static_cast<UInt16>(0x8000 | size);
	}
}

void CountryBattle::padPlayerData( Stream& st, CBStatusData& data )
{
	st << data.type << data.lvlside;
	switch(data.type)
	{
	case 0:
	case 1:
	case 2:
		st << data.player->getName();
		break;
	case 3:
		st << data.player->getName() << data.score;
		break;
	case 4:
	case 5:
		st << data.player->getName() << data.streak << data.score << data.playerOther->getName() << data.streakOther;
		break;
	default:
		break;
	}
}

void CountryBattle::broadcast( Stream& st )
{
	if(_map != NULL)
		_map->Broadcast(_spot, st);
}

void CountryBattle::getReward(UInt32 curtime)
{
	for(UInt8 lvl = 0; lvl < 3; ++ lvl)
	{
		for(UInt8 side = 0; side < 2; ++ side)
		{
			std::vector<CountryBattleData *>& blist = _players[lvl][side];
			size_t size = blist.size();
			size_t i = 0;
			while(i < size)
			{
				UInt32 dur = blist[i]->getReward(lvl, curtime, _rewardTime);
				if(dur > 0)
				{
					Player * player = blist[i]->player;
					CBPlayerData& cbpdata = _battleDuration[player->getCountry()][player];
					cbpdata.awardTime += (dur << 1);
				}
				UInt8 nlev = getJoinLevel(blist[i]->player->GetLev());
				if(nlev > lvl)
				{
					CountryBattleData * data = new(std::nothrow) CountryBattleData;
					if(data != NULL)
					{
						Player * player = blist[i]->player;

						CBStatusData cbsd[2];
						cbsd[0].type = 1;
						cbsd[0].setSideLevel(side, lvl);
						cbsd[0].player = player;

						player->delFlag(Player::CountryBattle);
						blist.erase(blist.begin() + i);
						size = blist.size();
						player->autoRegenAll();

						sendInfo(player);

						data->player = player;
						data->joinTime = curtime;
						data->resetEndTime = curtime + 30;
						_players[nlev][side].push_back(data);
						player->addFlag(Player::CountryBattle);

						cbsd[1].type = 0;
						cbsd[1].setSideLevel(side, nlev);
						cbsd[1].player = player;
						Stream st(REP::COUNTRY_WAR_STRING);
						st << _score[0] << _score[1] << static_cast<UInt16>(2);
						padPlayerData(st, cbsd[0]);
						padPlayerData(st, cbsd[1]);
						st << Stream::eos;
						broadcast(st);
						continue;
					}
				}
				++ i;
			}
		}
	}
}

void CountryBattle::sendInfo( Player * pl)
{
    if (pl && pl->getCountry() >= COUNTRY_NEUTRAL)
        return;
	Stream st(REP::COUNTRY_WAR_PROCESS);
	st << static_cast<UInt8>(0);
	UInt32 curtime = TimeUtil::Now();
	if(curtime < globalCountryBattle.getStartTime())
	{
		if(curtime + 5 * 60 >= globalCountryBattle.getStartTime())
		{
			CBPlayerData& cbpdata = _battleDuration[pl->getCountry()][pl];
			if(!cbpdata.firstEnter)
			{
				cbpdata.firstEnter = true;
				playerEnter(pl);
			}
		}
		st << static_cast<UInt8>(0) << (globalCountryBattle.getStartTime() - curtime);
	}
	else
	{
		if(curtime + 60 <= globalCountryBattle.getEndTime())
		{
			CBPlayerData& cbpdata = _battleDuration[pl->getCountry()][pl];
			if(!cbpdata.firstEnter)
			{
				cbpdata.firstEnter = true;
				playerEnter(pl);
			}
		}
		st << static_cast<UInt8>(1) << (globalCountryBattle.getEndTime() - curtime);
	}
	st << getOwner() << getScore(0) << getScore(1);

	UInt8 cny = pl->getCountry();
	std::map<Player *, CBPlayerData>::iterator it = _battleDuration[cny].find(pl);
	if(it == _battleDuration[cny].end())
	{
		st.append_zero(6);
	}
	else
	{
		CBPlayerData& cbpdata = it->second;
		st << cbpdata.totalAchievement << cbpdata.totalWin << cbpdata.totallose << cbpdata.currKillStreak << cbpdata.maxKillStreak;
	}

	if(_topKiller == NULL)
		st << static_cast<UInt8>(0) << "" << static_cast<UInt8>(0);
	else
		st << _topKiller->getCountry() << _topKiller->getName() << _topStreak;

	padPlayerData(st, getJoinLevel(pl->GetLev()));
	st << Stream::eos;
	pl->send(st);
}

void GlobalCountryBattle::prepare( UInt32 t )
{
	_running = false;
	if(cfg.GMCheck)
		_prepareTime = TimeUtil::SharpDay(0) + 19 * 60 * 60 + 45 * 60;
	else
		_prepareTime = t + 30;
	if(_prepareTime + 45 * 60 < t)
		_prepareTime += 24 * 60 * 60;
	_startTime = _prepareTime + 15 * 60;
	if(cfg.GMCheck)
	{
		_startTime = _prepareTime + 15 * 60;
		_endTime = _startTime + 30 * 60;
	}
	else
	{
		_startTime = _prepareTime + 30;
		_endTime = _startTime + 8 * 60;
	}
}

void GlobalCountryBattle::prepare2( UInt32 t )
{
    if (WORLD().isNewCountryBattle())
    {
        if (!_NewcountryBattle) return;
	    _NewcountryBattle->prepare(_startTime - t);
    }
    else
    {
        if (!_countryBattle) return;
	    _countryBattle->prepare(_startTime - t);
    }
	_running = true;
}

void GlobalCountryBattle::start( UInt32 t )
{
    if (WORLD().isNewCountryBattle())
    {
        if (!_NewcountryBattle) return;
	    _NewcountryBattle->start(_endTime - t);
    }
    else
    {
        if (!_countryBattle) return;
	    _countryBattle->start(_endTime - t);
    }
	_running = true;
}

void GlobalCountryBattle::end( )
{
    if (!WORLD().isNewCountryBattle())
    {
        if (!_countryBattle) return;
        UInt32 curtime = TimeUtil::Now();
        _countryBattle->end(curtime);
    }
    setStatus(2);

	_running = false;
	_prepareTime = 0;
	_startTime = 0;
	_endTime = 0;
}

bool GlobalCountryBattle::process(UInt32 curtime)
{
    if (WORLD().isNewCountryBattle() && !_NewcountryBattle)
    {
        ERROR_LOG("NewCountryBattle Init Error !");
        Thread::sleep(1000);
        return false;
    }
    if (!WORLD().isNewCountryBattle() && !_countryBattle)
    {
        ERROR_LOG("CountryBattle Init Error !");
        Thread::sleep(1000);
        return false;
    }

	if(_prepareTime == 0)
		prepare(curtime);

	if(curtime < _prepareTime)
	{
		return false;
	}

	switch(_prepareTime)
	{
	case 1:
		break;
	case 2:
		if(curtime < _startTime)
		{
			return false;
		}
		start(curtime);
		_prepareTime = 1;
		{
			for(std::set<Player *>::iterator it = _autoList.begin(); it != _autoList.end(); ++ it)
			{
				Player * player = *it;
				if(player->getThreadId() != WORKER_THREAD_NEUTRAL)
					continue;
                if (WORLD().isNewCountryBattle())
                {
                    player->moveTo(_NewcountryBattle->getLocation(), true);
                    _NewcountryBattle->playerEnter(player);
                }
                else
                {
                    player->moveTo(_countryBattle->getLocation(), true);
                    _countryBattle->playerEnter(player);
                }
			}
		}
        setStatus(1);
		break;
	case 3:
		{
			if(curtime < _startTime - 5 * 60)
				return false;
            if(WORLD().isNewCountryBattle())
            {
			    SYSMSG_BROADCASTV(239, 5);
            }
            else
            {
			    SYSMSG_BROADCASTV(241, 5);
            }
			_prepareTime = 2;
		}
		return false;
	case 4:
		{
			if(curtime < _startTime - 10 * 60)
				return false;
            if(WORLD().isNewCountryBattle())
            {
			    SYSMSG_BROADCASTV(239, 10);
            }
            else
            {
			    SYSMSG_BROADCASTV(241, 10);
            }
			_prepareTime = 3;
		}
		return false;
	default:
		{
            if(WORLD().isNewCountryBattle())
            {
			    SYSMSG_BROADCAST(238);
            }
            else
            {
			    SYSMSG_BROADCAST(240);
            }
			prepare2(curtime);
			_prepareTime = 4;
		}
		return false;
	}

    if (WORLD().isNewCountryBattle())
	    _NewcountryBattle->process(curtime); //不能用定时器去直接调用_NewcountryBattle->end()
	if(curtime >= _endTime)
	{
		end();
		return true;
	}
    if (!WORLD().isNewCountryBattle())
	    _countryBattle->process(curtime);

	return false;
}

void GlobalCountryBattle::addAutoCB( Player * player )
{
	_autoList.insert( player );
}

void GlobalCountryBattle::delAutoCB( Player * player )
{
	_autoList.erase( player );
}

void GlobalCountryBattle::sendDaily(Player* player)
{
    if(WORLD().isNewCountryBattle())
        return;
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(9);
    st << static_cast<UInt8>(_status);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

void GlobalCountryBattle::sendForNewCB(Player * player)
{   //玩家登录时调用
    if(!WORLD().isNewCountryBattle())
        return;
    UInt32 curtime = TimeUtil::Now();
    if(curtime >= _prepareTime && curtime < _startTime)
    {
        Stream st(REP::NEW_CAMPS_WAR_JOIN);
        st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0) << static_cast<UInt16>(_startTime - curtime) << Stream::eos;
        player->send(st);
        //_NewcountryBattle->sendSelfInfo(player);
    }
    else if(curtime >= _startTime && curtime < _endTime)
    {
        Stream st(REP::NEW_CAMPS_WAR_JOIN);
        st << static_cast<UInt8>(0x05) << static_cast<UInt8>(1) << static_cast<UInt16>(_endTime - curtime) << Stream::eos;
        player->send(st);
        //_NewcountryBattle->sendSelfInfo(player);
    }
    else if(curtime < TimeUtil::SharpDay(0, _prepareTime))
    {
        Stream st(REP::NEW_CAMPS_WAR_JOIN);
        st << static_cast<UInt8>(0x05) << static_cast<UInt8>(2) << static_cast<UInt16>(0) << Stream::eos;
        player->send(st);
    }
}

}


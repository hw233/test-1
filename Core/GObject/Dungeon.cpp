﻿#include "Config.h"
#include "Dungeon.h"
#include "Player.h"
#include "Package.h"
#include "Fighter.h"
#include "Country.h"
#include "Script/GameActionLua.h"
#include "Battle/BattleSimulator.h"
#include "Server/Cfg.h"
#include "MsgID.h"
#include "GData/Money.h"
#include "HeroMemo.h"
#include "GObject/Tianjie.h"
namespace GObject
{

EventDungeonAuto::EventDungeonAuto( Player * player, UInt32 interval, Dungeon * dg, bool won, UInt32 exp )
	: EventBase(player, interval), m_Dungeon(dg), m_Won(won), m_TotalExp(exp)
{
}

void EventDungeonAuto::Process( UInt32 )
{
	m_Dungeon->processAutoChallenge(m_Player, m_Won ? 1 : 2, &m_TotalExp);
}

void EventDungeonAuto::NotifyCancel() const
{
	m_Dungeon->cancelAutoChallengeNotify(m_Player, m_TotalExp);
}

void EventDungeonAuto::Complete() const
{
	m_Dungeon->completeAutoChallenge(m_Player, m_TotalExp, m_Won);
}

UInt8 Dungeon::_maxCount[2] = {1,1};
UInt16 Dungeon::_price[2][5] ={ {0, 20, 20, 20, 20},{0, 50, 50, 50, 50} };
size_t Dungeon::_priceCount = 0;

GGlobalObjectManagerT<Dungeon, UInt8> dungeonManager;
UInt8 Dungeon::_extraCount[2][16] = { {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},{0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };

Dungeon::Dungeon( UInt8 id, const GData::DungeonData * dd ): GObjectBaseT<Dungeon, UInt8>(id), _dungeon(dd)
{
	size_t levels = _dungeon->monsters[0].size();
    for(int i = 0; i < Max_Difficulty; i++ )
    {
        _levels[i].resize(levels);
        _recentReports[i].resize(levels);
    }
}

Dungeon::~Dungeon()
{
}

void Dungeon::playerJump( Player * player, UInt8 difficulty , UInt8 level )
{
    if(difficulty >= Max_Difficulty)
    {
        return;
    }
	if(level >= _levels[difficulty].size())
		return;
	DungeonPlayerInfo& dpi = _players[player];
    dpi.difficulty = difficulty;
    dpi.level[difficulty] = level;
	updateToDB(player, dpi);
	sendDungeonLevelData(player, dpi);
}

UInt8 Dungeon::playerEnter( Player * player, UInt8 difficulty )
{
    
    if(difficulty >= Max_Difficulty)
    {
        return 3;
    } 
    if(player->GetLev() < _dungeon->levelReq)
		return 1;

	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	DungeonPlayerInfo* dpi;
	if(it == _players.end())
	{
		dpi = &_players[player];
		dpi->counterEnd = TimeUtil::SharpDay(1);
		++ dpi->count[difficulty];
        dpi->level[difficulty] = 1;
		PLAYER_DATA(player, dungeonEnd) = TimeUtil::SharpDay(1);
        ++ PLAYER_DATA1(player, difficulty);
        dpi->difficulty = difficulty;
		sendDungeonInfo(player, *dpi);
        player->dungeonUdpLog(_dungeon->levelReq, 1);
	}
	else
	{

		UInt32 viplvl = player->getVipLevel();
		UInt8 extraCount = _extraCount[difficulty][viplvl];
		dpi = &it->second;

		checkForTimeout(player, *dpi, false);

		if(difficulty == dpi->difficulty &&  dpi->level[dpi->difficulty] != 0xFF)
            return 2;

		leaveLevel(player, it->second.level[difficulty],it->second.difficulty);
		//if(it->second.level > 0)
		{
            if(/*cfg.GMCheck && */PLAYER_DATA1(player, difficulty) >= getMaxCount(difficulty) + extraCount)// fix gold less?
            {
                return 2;
            }

			if(PLAYER_DATA1(player, difficulty) >= getMaxCount(difficulty))
			{
				UInt32 price = _price[difficulty][PLAYER_DATA1(player, difficulty)];
				if(price > 0)
				{
					if(!player->hasChecked())
						return 4;
					if(player->getGold() < price)
						return 3;
					ConsumeInfo ci(VipEnterDungeon,0,0);
					player->useGold(price, &ci);
                    player->dungeonUdpLog(_dungeon->levelReq, 3);
				}
			}
            else
            {
                    player->dungeonUdpLog(_dungeon->levelReq, 1);
            }

		}

        ++ dpi->count[difficulty];
        ++ PLAYER_DATA1(player, difficulty); 
        dpi->level[difficulty] = 1;
        dpi->difficulty = difficulty;
        sendDungeonInfo(player, *dpi);
	}

	sendDungeonLevelData(player, *dpi);
	enterLevel(player, dpi->level[difficulty],dpi->difficulty);
	updateToDB(player, *dpi);

    GameAction()->doStrong( player, SthDungeon, 0 , 0);
	return 0;
}

UInt8 Dungeon::playerLeave( Player * player,UInt8 difficulty )
{
    std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return 1;

	if(it->second.level == 0)
		return 0;
    
    it->second.difficulty = difficulty;	
	cancelAutoChallengeNotify(player, 0);

	leaveLevel(player, it->second.level[it->second.difficulty],it->second.difficulty);
	it->second.level[it->second.difficulty] = 0;
	updateToDB(player, it->second);
	//it->second.difficulty = 0xFF;

    UInt32 mark = player->GetVar(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK);
    UInt8 pos = _id - 1; 
    mark = CLR_BIT(mark, pos);
    player->SetVar(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK, mark);

	return 0;
}

UInt8 Dungeon::playerContinue( Player * player ,UInt8 difficulty)
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
    it->second.difficulty = difficulty;	
    if(it == _players.end())
		return 1;

    if(it->second.level == 0)
        return 1;

    enterLevel(player, it->second.level[it->second.difficulty],it->second.difficulty);
    sendDungeonLevelData(player, it->second);
	return 0;
}

UInt8 Dungeon::playerBreak( Player * player ,UInt8 difficulty)
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return 1;

    it->second.difficulty = difficulty;	
	leaveLevel(player, it->second.level[it->second.difficulty], it->second.difficulty);
	return 0;
}

void Dungeon::takeLoot( Player * player, DungeonPlayerInfo& dpi, UInt32& exp )
{
	const GData::DungeonLevel * dgl = _dungeon->monsters[dpi.difficulty][dpi.level[dpi.difficulty]];
    if(!dgl)
        return;

	const GData::DungeonMonster * dm = dgl->monsterSet;
    if(!dm)
        return;

    exp = dm->exp;
    if(player->getBuffData(PLAYER_BUFF_QI_TIAN_CHU_MO))
        exp *= (18.f/10.f);

	player->pendExp(exp);
	UInt32 itemId = 0;
	if(World::_activityStage > 0)
	{
        // TODO: lua function
		GameAction()->onDungeonLootItemRoll(player, _dungeon->getId(), dpi.level[dpi.difficulty], dm->formated);
		dgl->getLoot(player, itemId, static_cast<UInt8>(PLAYER_DATA1(player, dpi.difficulty) < 2 ? 40 : 0));
	}
	else
		dgl->getLoot(player, itemId, static_cast<UInt8>(PLAYER_DATA1(player, dpi.difficulty) < 2 ? 40 : 0));
	std::map<UInt16, UInt8> cloots;
	for(std::vector<GData::LootResult>::iterator iter = player->_lastLoot.begin(); iter < player->_lastLoot.end(); ++ iter)
	{
		if(IsEquipTypeId(iter->id) || IsGemId(iter->id))
		{
			const GData::ItemBaseType * itype = GData::itemBaseTypeManager[iter->id];
			if(itype != NULL && itype->quality >= 4)
				cloots[iter->id] += iter->count;
		}
	}
	if(!cloots.empty())
	{
		UInt16 sz = cloots.size();
	/*	Stream stp(REP::COPY_DATA_UPDATE);
		Stream stb(REP::COPY_DATA_UPDATE);
		stp << static_cast<UInt8>(4) << _id << sz;
		stb << static_cast<UInt8>(3) << _id << sz;
		for(std::map<UInt16, UInt8>::iterator iter = cloots.begin(); iter != cloots.end(); ++ iter)
		{
			dpi.lootToday.push_back(DungeonItemInfo(iter->first, iter->second));
			_recentLoots.push_back(DungeonLootInfo(player, iter->first, iter->second));
			stp << iter->first << iter->second;
			stb << player->getCountry() << player->getName() << iter->first << iter->second;
		}
		stp << Stream::eos;
		stb << Stream::eos;
		player->send(stp);
		broadcast(stb, dpi.difficulty, player);*/
		if(dpi.lootToday[dpi.difficulty].size() > 50)
		{
			std::list<DungeonItemInfo>::iterator iter = dpi.lootToday[dpi.difficulty].begin();
			std::advance(iter, dpi.lootToday[dpi.difficulty].size() - 50);
			dpi.lootToday[dpi.difficulty].erase(dpi.lootToday[dpi.difficulty].begin(), iter);
		}
		if(_recentLoots.size() > 50)
		{
			std::list<DungeonLootInfo>::iterator iter = _recentLoots.begin();
			std::advance(iter, _recentLoots.size() - 50);
			_recentLoots.erase(_recentLoots.begin(), iter);
		}
	}
}

bool Dungeon::doAttack( Player * player, UInt8 difficulty, UInt8 level)
{

    if(difficulty >= Max_Difficulty)
    {
        return false;
    }
    if (player->GetPackage()->GetRestPackageSize() <= 0)
    {
        player->sendMsgCode(0, 1011);
        return false;
    }
    if(level >= _levels[difficulty].size())
        return false;
    const GData::DungeonLevel * dgl = _dungeon->monsters[difficulty][level];
    if(dgl == NULL)
		return false;
	const GData::DungeonMonster * dm = dgl->monsterSet;
	if(dm == NULL)
		return false;

    if(!dm->fighter)
        return false;

    player->OnHeroMemo(MC_SLAYER, MD_STARTED, 0, 0);

	Battle::BattleSimulator bsim(player->getLocation(), player, dm->fighter->getName(), dm->fighter->getLevel(), false);
	player->PutFighters(bsim, 0);
	bsim.setPortrait(1, dm->fighter->favor);
	if(dm->formated)
	{
		std::map<GObject::Fighter *, std::vector<UInt8> >::const_iterator iter;
		for(iter = dm->monsterPos.begin(); iter != dm->monsterPos.end(); ++ iter)
		{
			GObject::Fighter * fgt = iter->first;
            size_t c = iter->second.size();
			for(size_t i = 0; i < c; ++ i)
			{
				bsim.newFighter(1, iter->second[i], fgt);
			}
		}
	}
	else
	{
		UInt32 seed = TimeUtil::SharpDay(0) + static_cast<UInt32>(player->getId()) + static_cast<UInt32>(player->getId() >> 32) + (static_cast<UInt32>(_id) << 24) + (static_cast<UInt32>(level) << 8);
		URandom rnd(seed);
		UInt8 count;
		if(dm->minNum < dm->maxNum)
			count = static_cast<UInt8>(dm->minNum + rnd(dm->maxNum - dm->minNum + 1));
		else
			count = dm->minNum;
		std::vector<UInt8> vcount;
		UInt8 sz = dm->monsterPos.size();
		vcount.resize(sz, 1);
		count -= sz;
		for(UInt8 i = 0; i < count; ++ i)
		{
			UInt8 r = rnd(sz);
			++ vcount[r];
		}
		std::map<GObject::Fighter *, std::vector<UInt8> >::const_iterator iter;
		UInt8 idx = 0;
		for(iter = dm->monsterPos.begin(); iter != dm->monsterPos.end(); ++ iter, ++ idx)
		{
			GObject::Fighter * fgt = iter->first;
			for(UInt8 i = 0; i < vcount[idx]; ++ i)
			{
				UInt8 pos = rnd(25 - i);
				while(bsim(1, pos) != NULL)
				{
					++ pos;
					if(pos == 25)
						pos = 0;
				}
				bsim.newFighter(1, pos, fgt);
			}
		}
	}
	bsim.start();

	return bsim.getWinner() == 1;
}

bool Dungeon::doChallenge( Player * player, DungeonPlayerInfo& dpi, bool report, UInt32 * turns )
{
    UInt8 difficulty = dpi.difficulty;
    if(difficulty >= Max_Difficulty)
    {
        return false;
    } 
    
    UInt8 level = dpi.level[dpi.difficulty];
	if(level >= _levels[dpi.difficulty].size())
		return false;
	const GData::DungeonLevel * dgl = _dungeon->monsters[difficulty][level];
	if(dgl == NULL)
		return false;
	const GData::DungeonMonster * dm = dgl->monsterSet;
	if(dm == NULL)
		return false;

    player->OnHeroMemo(MC_SLAYER, MD_STARTED, 0, 0);

    player->setJusticeRoar(dpi.justice_roar[dpi.difficulty]);
	Battle::BattleSimulator bsim(player->getLocation(), player, dm->fighter->getName(), dm->fighter->getLevel(), false);
	player->PutFighters(bsim, 0);
	bsim.setPortrait(1, dm->fighter->favor);
	if(dm->formated)
	{
		std::map<GObject::Fighter *, std::vector<UInt8> >::const_iterator iter;
		for(iter = dm->monsterPos.begin(); iter != dm->monsterPos.end(); ++ iter)
		{
			GObject::Fighter * fgt = iter->first;
		    size_t c = iter->second.size();
			for(size_t i = 0; i < c; ++ i)
			{
				bsim.newFighter(1, iter->second[i], fgt);
			}
		}
	}
	else
	{
		UInt32 seed = TimeUtil::SharpDay(0) + static_cast<UInt32>(player->getId()) + static_cast<UInt32>(player->getId() >> 32) + (static_cast<UInt32>(_id) << 24) + (static_cast<UInt32>(level) << 8) + (static_cast<UInt32>(PLAYER_DATA1(player, dpi.difficulty)) << 24);
		URandom rnd(seed);
		UInt8 count;
		if(dm->minNum < dm->maxNum)
			count = static_cast<UInt8>(dm->minNum + rnd(dm->maxNum - dm->minNum + 1));
		else
			count = dm->minNum;
		std::vector<UInt8> vcount;
		UInt8 sz = dm->monsterPos.size();
		vcount.resize(sz, 1);
		count -= sz;
		for(UInt8 i = 0; i < count; ++ i)
		{
			UInt8 r = rnd(sz);
			++ vcount[r];
		}
		std::map<GObject::Fighter *, std::vector<UInt8> >::const_iterator iter;
		UInt8 idx = 0;
		for(iter = dm->monsterPos.begin(); iter != dm->monsterPos.end(); ++ iter, ++ idx)
		{
			GObject::Fighter * fgt = iter->first;
			for(UInt8 i = 0; i < vcount[idx]; ++ i)
			{
				UInt8 pos = rnd(25 - i);
				while(bsim(1, pos) != NULL)
				{
					++ pos;
					if(pos == 25)
						pos = 0;
				}
				bsim.newFighter(1, pos, fgt);
			}
		}
	}
	bsim.start();
    player->setJusticeRoar(0);
    dpi.justice_roar[dpi.difficulty] = 0;
    DB3().PushUpdateData("UPDATE `dungeon_player` SET `justice_roar` = %u WHERE `id` = %u AND `playerId` = %" I64_FMT "u", dpi.justice_roar[dpi.difficulty], _id, player->getId());

	Stream& packet = bsim.getPacket();
	if(packet.size() <= 8)
		return false;

    // TODO:
	if (_id == 1)
            GameAction()->RunOperationTaskAction2(player, 1, 1, level);

	Stream st(REP::ATTACK_NPC);
	bool res = bsim.getWinner() == 1;
	UInt32 turns_ = bsim.getTurns();
	if(dm->formated && res)
	{
		std::list<DungeonReportInfo>& dril = _recentReports[difficulty][level];
		bool found = false;
		DungeonReportInfo dri;
		dri.player = player;
		dri.id = bsim.getId();
		for(std::list<DungeonReportInfo>::iterator it = dril.begin(); it != dril.end(); ++ it)
		{
			if(it->player == player)
			{
				found = true;
				dril.erase(it);
				dril.push_front(dri);
				break;
			}
		}
		if(!found)
		{
			dril.push_front(dri);
			if(dril.size() > 3)
				dril.pop_back();
		}
	}
	if(report)
	{
		if(res)
		{
			st << static_cast<UInt16>(0x0101);
			UInt32 exp;
			takeLoot(player, dpi, exp);
		}
		else
		{
			st << static_cast<UInt16>(0x0100);
		}
		st << player->getPendExp() << static_cast<UInt8>(0);
		UInt8 sz = player->_lastLoot.size();
		st << sz;
		for(std::vector<GData::LootResult>::iterator iter = player->_lastLoot.begin(); iter != player->_lastLoot.end(); ++ iter)
		{
			st << iter->id << iter->count;
		}
		st.append(&packet[8], packet.size() - 8);
		st << Stream::eos;
		player->send(st);
		bsim.applyFighterHP(0, player);
		player->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + turns_);
	}

	if(turns != NULL)
		*turns = turns_;

	st.init(REP::COPY_FIGHT_RESULT);
	st << level << static_cast<UInt8>(res ? 0 : 1) << Stream::eos;
	player->send(st);

	return res;
}

bool Dungeon::advanceLevel( Player * player, DungeonPlayerInfo& dpi, bool noreport, UInt32 * totalExp, UInt32 gold )
{

    if(dpi.difficulty >= Max_Difficulty)
    {
        return true;
    }
    UInt8 level = dpi.level[dpi.difficulty];
	if(level >= _levels[dpi.difficulty].size())
    {
        dpi.justice_roar[dpi.difficulty] = 0;
        dpi.justice[dpi.difficulty] = 0;
        DB3().PushUpdateData("UPDATE `dungeon_player` SET `justice` = %u, `justice_roar` = %u WHERE `id` = %u AND `playerId` = %" I64_FMT "u", dpi.justice[dpi.difficulty], dpi.justice_roar[dpi.difficulty], _id, player->getId());
		return true;
    }
	leaveLevel(player, level, dpi.difficulty);

    if(dpi.justice[dpi.difficulty] < 100)
    {
        dpi.justice[dpi.difficulty] += 5;
        DB3().PushUpdateData("UPDATE `dungeon_player` SET `justice` = %u WHERE `id` = %u AND `playerId` = %" I64_FMT "u", dpi.justice[dpi.difficulty], _id, player->getId());
    }

    // TODO:
	GameAction()->RunOperationTaskAction2(player, 2, _id, level);

	++ level;
	UInt16 dpd = (static_cast<UInt16>(level) << 8) + _id;
	bool online = player->isOnline();
	UInt32 exp = 0;
	if(noreport)
	{
		takeLoot(player, dpi, exp);
		if(totalExp != NULL)
			*totalExp += exp;
	}
	bool r;
	if(level < _levels[dpi.difficulty].size())
	{
		dpi.level[dpi.difficulty] = level;
		enterLevel(player, level,dpi.difficulty);
		DB3().PushUpdateData("UPDATE `dungeon_player` SET `level` = %u WHERE `id` = %u AND `playerId` = %" I64_FMT "u AND `difficulty` = %u", dpi.level[dpi.difficulty], _id, player->getId(),dpi.difficulty);
		r = false;
	}
	else
	{
		dpi.level[dpi.difficulty] = 0xFF;
		++ dpi.totalCount[dpi.difficulty];
		if(dpi.firstPass[dpi.difficulty] == 0)
		{
			dpi.firstPass[dpi.difficulty] = TimeUtil::Now();
			DB3().PushUpdateData("UPDATE `dungeon_player` SET `level` = %u, `totalCount` = %u, `firstPass` = %u WHERE `id` = %u AND `playerId` = %" I64_FMT "u", dpi.level[dpi.difficulty], dpi.totalCount[dpi.difficulty], dpi.firstPass[dpi.difficulty], _id, player->getId());

            //第一次通关给荣誉
            GameAction()->doAttainment(player, 10354, _id);
		}
		else
			DB3().PushUpdateData("UPDATE `dungeon_player` SET `level` = %u, `totalCount` = %u WHERE `id` = %u AND `playerId` = %" I64_FMT "u", dpi.level[dpi.difficulty], dpi.totalCount[dpi.difficulty], _id, player->getId());
		r = true;
		DBLOG1().PushUpdateData("insert into `dungeon_statistics` (`server_id`, `player_id`, `dungeon_id`, `this_day`, `pass_time`) values(%u, %" I64_FMT "u, %u, %u, %u)", cfg.serverLogId, player->getId(), _id, TimeUtil::SharpDay(0), TimeUtil::Now());

        player->OnHeroMemo(MC_SLAYER, MD_STARTED, 0, 2);
        if (World::getWhiteLoveDay() && World::_wday == 6)
            player->GetPackage()->AddItem2(476, 5, 1, 1);

        if(World::getFourCopAct())
        {
            UInt32 randNum;
            if(PLAYER_DATA1(player, dpi.difficulty) > getMaxCount(dpi.difficulty))
            {
                randNum = 11 + uRand(2);
            }
            else
                randNum = 10;
            player->GetPackage()->AddItem2(9209, randNum, true, true);
        }
        if(World::getGoldSnakeAct())
        {
            if(PLAYER_DATA1(player, dpi.difficulty) > getMaxCount(dpi.difficulty))
                player->GetPackage()->Add(9314, 2, true, false);
            else
                player->GetPackage()->Add(9314, 1, true, false);
        }
        if (GObject::Tianjie::instance().isTjOpened())
        { 
            player->GetPackage()->AddItem(9138, 1, false, false);
        }
        if (World::getItem9344Act())
        {
            if(PLAYER_DATA1(player, dpi.difficulty) > getMaxCount(dpi.difficulty))
                player->GetPackage()->Add(9344, 2, true, false);
            else
                player->GetPackage()->Add(9344, 1, true, false);
        }
        if (World::getItem9343Act())
        {
            if(PLAYER_DATA1(player, dpi.difficulty) > getMaxCount(dpi.difficulty))
player->GetPackage()->Add(9343, 2, true, false);
            else
                player->GetPackage()->Add(9343, 1, true, false);
        }
        bool free = (PLAYER_DATA1(player, dpi.difficulty) <= getMaxCount(dpi.difficulty));

        UInt32 mark = player->GetVar(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK);
        UInt8 pos = _id - 1; 
        mark = CLR_BIT(mark, pos);
        player->SetVar(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK, mark);

		GameAction()->onDungeonWin(player, _id, dpi.totalCount[dpi.difficulty], free);
        if(dpi.difficulty > 0)
            player->copyFrontWinAward(3, free == false);
        if(PLAYER_DATA1(player, dpi.difficulty) > getMaxCount(dpi.difficulty))
        {
            player->dungeonUdpLog(_dungeon->levelReq, 4);
        }
        else
        {
            player->dungeonUdpLog(_dungeon->levelReq, 2);
        }
	}

	if(noreport)
	{
		if(online)
		{
			Stream st(REP::COPY_AUTO_FIGHT);
			st << _id << static_cast<UInt8>(level - 1) << dpi.difficulty;
			if(r)
				st << static_cast<UInt8>(4) << *totalExp;
			else
				st << static_cast<UInt8>(3);
			UInt8 size = player->_lastLoot.size();
			st << size;
			for(UInt8 i = 0; i < size; ++ i)
			{
				st << player->_lastLoot[i].id << static_cast<UInt8>(player->_lastLoot[i].count);
			}
			st << Stream::eos;
			player->send(st);
		}
	}

	if(noreport || !online)
		player->checkLastBattled();
	else
	{
		player->pendDungeon(dpd);
		sendDungeonLevelData(player, dpi);
	}
	return r;
}

void Dungeon::startChallenge( Player * player ,UInt8 difficulty)
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
    it->second.difficulty = difficulty;
    if(it == _players.end())
		return;

	if(player->hasFlag(Player::AutoDungeon))
	{
		player->sendMsgCode(0, 1407);
		return;
	}

	UInt32 now = TimeUtil::Now();
	UInt32 buffLeft = player->getBuffData(PLAYER_BUFF_ATTACKING, now);
	if(buffLeft > 0)
	{
		player->sendMsgCode(0, 1407, buffLeft - now);
		return;
	}

	player->checkLastBattled();

	if(doChallenge(player, it->second))
		advanceLevel(player, it->second);
}

void Dungeon::sendAutoChallengeStart( Player * player )
{
	if(!player->hasFlag(Player::AutoDungeon))
		return;
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
	Stream st(REP::COPY_AUTO_FIGHT);
	st << _id << static_cast<UInt8>(it->second.level[it->second.difficulty]) << it->second.difficulty << static_cast<UInt8>(0) << Stream::eos;
	player->send(st);
}

void Dungeon::processAutoChallenge( Player * player, UInt8 type, UInt32 * totalExp, UInt8 mtype )
{
	if(type != 0 && !player->hasFlag(Player::AutoDungeon))
		return;

	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
    
    UInt32 mark = player->GetVar(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK);
    UInt8 pos = _id - 1; 
	switch(type)
	{
	case 0:
		{
            bool girl = (World::getGirlDay() && !player->IsMale());
            if (!World::getNewYear() &&
                !girl &&
                !World::getNetValentineDay() &&
                0 == GET_BIT_MARK(mark, pos))
            {
                UInt32 viplevel = player->getVipLevel();
                if(viplevel < 6)
                {
                    if (mtype == 1)
                    {
                        ConsumeInfo ci(DungeonAutoConsume, 0, 0);
                        if(World::_wday == 5)
                        {
                            if(player->getGoldOrCoupon() < GData::moneyNeed[GData::DUNGEON_AUTO].gold / 2)
                                return;
                            player->useGoldOrCoupon(GData::moneyNeed[GData::DUNGEON_AUTO].gold / 2, &ci);
                        }
                        else
                        {
                            if(player->getGoldOrCoupon() < GData::moneyNeed[GData::DUNGEON_AUTO].gold)
                                return;
                            player->useGoldOrCoupon(GData::moneyNeed[GData::DUNGEON_AUTO].gold, &ci);
                        }
                    }
                    else
                    {
                        const UInt32 taelReq[] = {
                            0,
                            GData::moneyNeed[GData::DUNGEON_AUTO1].tael,
                            GData::moneyNeed[GData::DUNGEON_AUTO2].tael,
                            GData::moneyNeed[GData::DUNGEON_AUTO3].tael,
                            GData::moneyNeed[GData::DUNGEON_AUTO4].tael,
                            GData::moneyNeed[GData::DUNGEON_AUTO5].tael,
                        };

                        ConsumeInfo ci(DungeonAutoConsume, 0, 0);
                        if(World::_wday == 5)
                        {
                            if(player->getTael() < taelReq[_id] / 2)
                                return;
                            player->useTael(taelReq[_id] / 2, &ci);
                        }
                        else
                        {
                            if(player->getTael() < taelReq[_id])
                                return;
                            player->useTael(taelReq[_id], &ci);
                        }
                    }

                    mark = SET_BIT(mark, pos);
                    player->SetVar(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK, mark);
                }
            }
			DBLOG1().PushUpdateData("insert into `dungeon_statistics` (`server_id`, `player_id`, `dungeon_id`, `this_day`, `pass_time`) values(%u, %" I64_FMT "u, %u, %u, %u)", cfg.serverLogId, player->getId(), _id + 100, TimeUtil::SharpDay(0), TimeUtil::Now());
			Stream st(REP::COPY_AUTO_FIGHT);
			st << _id << static_cast<UInt8>(it->second.level[it->second.difficulty]) << it->second.difficulty <<static_cast<UInt8>(0) << Stream::eos;
			player->send(st);
            player->addFlag(Player::AutoDungeon);
		}
		break;
    case 1:
        if(advanceLevel(player, it->second, true, totalExp))
        {
            player->delFlag(Player::AutoDungeon);
            DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %" I64_FMT "u", player->getId());
            return;
        }
        if(player->GetPackage()->GetRestPackageSize() < 4)
        {
            player->delFlag(Player::AutoDungeon);
            Stream st(REP::COPY_AUTO_FIGHT);
            st << _id << static_cast<UInt8>(it->second.level[it->second.difficulty]) << it->second.difficulty << static_cast<UInt8>(5) << *totalExp << Stream::eos;
			player->send(st);
			DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %" I64_FMT "u", player->getId());
			return;
		}
        break;
	case 2:
		{
			player->delFlag(Player::AutoDungeon);
			Stream st(REP::COPY_AUTO_FIGHT);
			st << _id << static_cast<UInt8>(it->second.level[it->second.difficulty]) << it->second.difficulty << static_cast<UInt8>(2) << *totalExp << Stream::eos;
			player->send(st);
			DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %" I64_FMT "u", player->getId());
			return;
		}
		break;
	default:
		break;
	}
	if(!player->hasFlag(Player::AutoDungeon))
		return;
	UInt32 turns = 0;
	bool r = doChallenge(player, it->second, false, &turns);
	DB3().PushUpdateData("REPLACE INTO `dungeon_auto`(`playerId`, `dungeonId`, `totalExp`, `won`) VALUES(%" I64_FMT "u, %u, %u, %u)", player->getId(), _id, *totalExp, r ? 1 : 0);
	EventDungeonAuto * event = new(std::nothrow) EventDungeonAuto(player, 2 * turns, this, r, *totalExp);
	PushTimerEvent(event);
}

void Dungeon::cancelAutoChallengeNotify( Player * player, UInt32 exp )
{
	player->delFlag(Player::AutoDungeon);
	DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %" I64_FMT "u", player->getId());
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
	Stream st(REP::COPY_AUTO_FIGHT);
	st << _id << it->second.level[it->second.difficulty] << it->second.difficulty << static_cast<UInt8>(1) << exp << Stream::eos;
	player->send(st);
}

void Dungeon::completeAutoChallenge( Player * player, UInt32 exp, bool won )
{
	if(!player->hasFlag(Player::AutoDungeon))
		return;

	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
    if (player->getGoldOrCoupon() < GData::moneyNeed[GData::DUNGEON_IM].gold)
        return;
	UInt32 count = 0;

    ConsumeInfo ci(DungeonAutoConsume, 0, 0);
    UInt16 gold = player->useGoldOrCoupon(GData::moneyNeed[GData::DUNGEON_IM].gold, &ci);
    Stream st_(REP::COPY_END_FIGHT);
    st_ << static_cast<UInt8>(0) << gold << static_cast<UInt16>(GData::moneyNeed[GData::DUNGEON_IM].gold - gold) << Stream::eos;
    player->send(st_);

	while(1)
	{
		if(won)
		{
			if(advanceLevel(player, it->second, true, &exp, count))
			{
				player->delFlag(Player::AutoDungeon);
				DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %" I64_FMT "u", player->getId());
                player->setBuffData(PLAYER_BUFF_ATTACKING, 0);
                return;
			}
            won = doChallenge(player, it->second, false, NULL);
		}
		else
		{
			player->delFlag(Player::AutoDungeon);

			Stream st(REP::COPY_AUTO_FIGHT);
			st << _id << static_cast<UInt8>(it->second.level[it->second.difficulty]) << it->second.difficulty << static_cast<UInt8>(2) << exp << Stream::eos;
			player->send(st);
			DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %" I64_FMT "u", player->getId());
            player->setBuffData(PLAYER_BUFF_ATTACKING, 0);
            return;
		}
	}

	processAutoChallenge(player, 3, &exp);
}

void Dungeon::autoChallenge( Player * player, UInt8 mtype, UInt8 difficulty)
{
    std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
    it->second.difficulty = difficulty;
    if(player->hasFlag(Player::AutoDungeon))
		return;
	UInt32 now = TimeUtil::Now();
	UInt32 buffLeft = player->getBuffData(PLAYER_BUFF_ATTACKING, now);
	if(buffLeft > 0)
	{
		player->sendMsgCode(0, 1407, buffLeft - now);
		return;
	}

	player->checkLastBattled();
	UInt32 exp = 0;
	processAutoChallenge(player, 0, &exp, mtype);
}

void Dungeon::pushChallenge( Player * player, UInt32 exp, bool won )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
    if (it == _players.end())
        return;
	player->addFlag(Player::AutoDungeon);
	EventDungeonAuto * event = new(std::nothrow) EventDungeonAuto(player, 1 + uRand(20), this, won, exp);
	PushTimerEvent(event);
}

void Dungeon::pushPlayer( Player * player, UInt8 difficulty, UInt8 level, UInt8 count, UInt16 totalCount, UInt32 firstPass, UInt32 counterEnd, UInt8 justice, UInt8 justice_roar )
{
    if(difficulty >= Max_Difficulty)
        return;
	DungeonPlayerInfo& dpi = _players[player];
    dpi.difficulty = difficulty;
	dpi.level[difficulty] = level;
	dpi.count[difficulty] = count;
	dpi.totalCount[difficulty] = totalCount;
    dpi.firstPass[difficulty] = firstPass;
	dpi.counterEnd = counterEnd;
	dpi.justice[difficulty] = justice;
    dpi.justice_roar[difficulty] = justice_roar;
	if(level >= _levels[difficulty].size())
		return;
	DungeonLevel& dl = _levels[difficulty][level];
	dl.singles.insert(player);
}

void Dungeon::sendDungeonInfo(Player * player,UInt8 difficulty)
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
	{
		Stream st(REP::COPY_DATA_UPDATE);
		UInt8 enterCount = (_extraCount[difficulty][player->getVipLevel()] << 4) | getEnterCount();
        UInt32 mark = player->GetVar(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK);
        UInt8 pos = _id - 1;
        pos = static_cast<UInt8>(GET_BIT_MARK(mark, pos));
		st << static_cast<UInt8>(0) << _id << difficulty<< static_cast<UInt8>(0) << PLAYER_DATA1(player, difficulty) << enterCount << static_cast<UInt16>(0) << static_cast<UInt32>(0) << static_cast<UInt8>(0) << pos << Stream::eos;
		player->send(st);
		return;
	}
	checkForTimeout(player, it->second, true);
	sendDungeonInfo(player, it->second, difficulty);
}

void Dungeon::sendDungeonInfo(Player * player, DungeonPlayerInfo& dpi, UInt8 difficulty)
{
	Stream st(REP::COPY_DATA_UPDATE);
	UInt8 enterCount = (_extraCount[difficulty][player->getVipLevel()] << 4) | getEnterCount();

    UInt32 mark = player->GetVar(VAR_DUNGEON_AUTO_FIGHT_USE_MONEY_MARK);
    UInt8 pos = _id - 1;
    pos = static_cast<UInt8>(GET_BIT_MARK(mark, pos));
    if(difficulty == 0xFF)
        return;//difficulty = dpi.difficulty;

    st << static_cast<UInt8>(0) << _id << difficulty << static_cast<UInt8>(dpi.level[difficulty]) << PLAYER_DATA1(player, difficulty) << enterCount << dpi.totalCount[difficulty] << dpi.firstPass[difficulty] << dpi.justice[difficulty]<< pos << Stream::eos;

	player->send(st);
}

void Dungeon::buildInfo( Player * player, Stream& st )
{
	st << _id;
#if 0
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	UInt8 enterCount = (_extraCount[player->getVipLevel()] << 4) | getEnterCount();
	if(it == _players.end())
		st << static_cast<UInt8>(0) << enterCount;
	else
	{
		checkForTimeout(player, it->second, true);
		st << it->second.count << enterCount;
	}
#endif
}

void Dungeon::sendDungeonLevelData( Player * player, DungeonPlayerInfo& dpi )
{

    if(dpi.difficulty >= Max_Difficulty)
        return;   
    
    if(dpi.level[dpi.difficulty] >= _levels[dpi.difficulty].size())
	{
		Stream st(REP::COPY_DATA_UPDATE);
		st << static_cast<UInt8>(5) << _id << dpi.difficulty << static_cast<UInt8>(0xFF) << dpi.justice[dpi.difficulty] << static_cast<UInt8>(0) << static_cast<UInt8>(0) << Stream::eos;
		player->send(st);
		return;
	}
	const GData::DungeonLevel * dgl = _dungeon->monsters[dpi.difficulty][dpi.level[dpi.difficulty]];
	if(dgl == NULL)
		return;
	Stream st(REP::COPY_DATA_UPDATE);
	st << static_cast<UInt8>(5) << _id << dpi.difficulty << static_cast<UInt8>(dpi.level[dpi.difficulty]) << dpi.justice[dpi.difficulty] << static_cast<UInt8>(1);
	const GData::DungeonMonster * dm = dgl->monsterSet;
	if(dm->formated)
	{
		UInt8 sz = static_cast<UInt8>(dm->monsterPos.size());
		std::map<GObject::Fighter *, std::vector<UInt8> >::const_iterator it;
		st << sz;
		for(it = dm->monsterPos.begin(); it != dm->monsterPos.end(); ++ it)
		{
			st << static_cast<UInt16>(it->first->getId()) << static_cast<UInt8>(it->second.size());
		}
	}
	else
	{
		UInt8 sz = static_cast<UInt8>(dm->monsterPos.size());
		std::map<GObject::Fighter *, std::vector<UInt8> >::const_iterator it;
		st << sz;
		UInt8 cnt = dm->minNum | (dm->maxNum << 4);
		for(it = dm->monsterPos.begin(); it != dm->monsterPos.end(); ++ it)
		{
			st << static_cast<UInt16>(it->first->getId()) << cnt;
		}
	}
	std::list<DungeonReportInfo>& dril = _recentReports[dpi.difficulty][dpi.level[dpi.difficulty]];
	st << static_cast<UInt8>(dril.size());
	for(std::list<DungeonReportInfo>::iterator it = dril.begin(); it != dril.end(); ++ it)
	{
		st << it->player->getCountry() << it->player->getName() << it->id;
	}
	st << Stream::eos;
	player->send(st);
}

void Dungeon::enterLevel( Player * player, UInt8 level,UInt8 difficulty )
{
    
    if(difficulty >= Max_Difficulty)
        return;
	if(level >= _levels[difficulty].size())
		return;
	DungeonLevel& dl = _levels[difficulty][level];
	if(dl.singles.find(player) != dl.singles.end())
		return;
	dl.singles.insert(player);
}

void Dungeon::leaveLevel( Player * player, UInt8 level,UInt8 difficulty )
{

    if(difficulty >= Max_Difficulty)
        return;
	if(level >= _levels[difficulty].size())
		return;
	DungeonLevel& dl = _levels[difficulty][level];
	std::set<Player *>::iterator it = dl.singles.find(player);
	if(it == dl.singles.end())
		return;
	dl.singles.erase(it);
}

void Dungeon::updateToDB( Player * player, DungeonPlayerInfo& dpi )
{
	DB3().PushUpdateData("REPLACE INTO `dungeon_player`(`id`, `playerId`, `difficulty`, `level`, `count`, `totalCount`, `firstPass`, `counterEnd`) VALUES(%u, %" I64_FMT "u, %u, %u, %u, %u, %u, %u)", _id, player->getId(), dpi.difficulty, dpi.level[dpi.difficulty], dpi.count[dpi.difficulty], dpi.totalCount[dpi.difficulty], dpi.firstPass[dpi.difficulty], dpi.counterEnd);

	DB1().PushUpdateData("UPDATE `player` SET `dungeonCnt` = %d, `dungeonCnt1` = %d, `dungeonEnd` = %u where `id` = %" I64_FMT "u", PLAYER_DATA(player, dungeonCnt),  PLAYER_DATA(player, dungeonCnt1), PLAYER_DATA(player, dungeonEnd), player->getId());
}

void Dungeon::checkForTimeout( Player * player, DungeonPlayerInfo& dpi, bool writeDB )
{
	UInt32 now = TimeUtil::Now();
	if(now >= PLAYER_DATA(player, dungeonEnd))
	{
        dpi.counterEnd = TimeUtil::SharpDay(1);
        dpi.count[dpi.difficulty] = 0;
		PLAYER_DATA(player, dungeonEnd) = dpi.counterEnd;
		PLAYER_DATA(player, dungeonCnt) = 0;
		PLAYER_DATA(player, dungeonCnt1) = 0;
		dpi.lootToday[dpi.difficulty].clear();
		if(writeDB)
		{
			DB3().PushUpdateData("UPDATE `dungeon_player` SET `count` = 0, `counterEnd` = %u WHERE `id` = %u AND `playerId` = %" I64_FMT "u", dpi.counterEnd, _id, player->getId());

            DB1().PushUpdateData("UPDATE `player` SET `dungeonCnt` = %u, `dungeonCnt1` = %u , `dungeonEnd` = %u where `id` = %" I64_FMT "u", PLAYER_DATA(player, dungeonCnt), PLAYER_DATA(player, dungeonCnt1), PLAYER_DATA(player, dungeonEnd), player->getId());
		}
	}
}

void Dungeon::sendMyLootInfo( Player * player )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
/*	{
		UInt16 sz = _recentLoots.size();
		Stream st(REP::COPY_DATA_UPDATE);
		st << static_cast<UInt8>(1) << _id << sz;
		for(std::list<DungeonLootInfo>::iterator iter = _recentLoots.begin(); iter != _recentLoots.end(); ++ iter)
		{
			st << iter->player->getCountry() << iter->player->getName() << iter->id << iter->count;
		}
		st << Stream::eos;
		player->send(st);
	}

	{
		std::list<DungeonItemInfo>& info = it->second.lootToday;
		UInt16 sz = info.size();
		Stream st(REP::COPY_DATA_UPDATE);
		st << static_cast<UInt8>(2) << _id << sz;
		for(std::list<DungeonItemInfo>::iterator iter = info.begin(); iter != info.end(); ++ iter)
		{
			st << iter->id << iter->count;
		}
		st << Stream::eos;
		player->send(st);
	}*/
}

void Dungeon::broadcast( Stream& st, UInt8 difficulty, Player * player )
{
	std::vector<DungeonLevel>& dll = _levels[difficulty];
	if(player == NULL)
	{
		for(std::vector<DungeonLevel>::iterator it = dll.begin(); it != dll.end(); ++ it)
		{
			std::set<Player *>& sl = it->singles;
			for(std::set<Player *>::iterator iter = sl.begin(); iter != sl.end(); ++ iter)
			{
				(*iter)->send(st);
			}
		}
	}
	else
	{
		for(std::vector<DungeonLevel>::iterator it = dll.begin(); it != dll.end(); ++ it)
		{
			std::set<Player *>& sl = it->singles;
			for(std::set<Player *>::iterator iter = sl.begin(); iter != sl.end(); ++ iter)
			{
				if(*iter != player)
					(*iter)->send(st);
			}
		}
	}
}

UInt32 Dungeon::getFirstPass( Player * player ,UInt8 difficulty)
{
    if(difficulty >= Max_Difficulty)
        return 0;
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return 0;
	return it->second.firstPass[difficulty];
}

void Dungeon::setMaxCount(UInt32 cnt, UInt8 difficulty)
{
	_maxCount[difficulty] = cnt;
}

void Dungeon::setPrice( UInt32 idx, UInt16 price ,UInt8 difficulty)
{
	_price[difficulty][idx] = price;
	if(idx >= _priceCount)
		_priceCount = idx + 1;
}

UInt16 * Dungeon::getPrice( size_t& size ,UInt8 difficulty)
{
	size = _priceCount;
	return _price[difficulty];
}

void Dungeon::setExtraCount( UInt32 idx, UInt8 count ,UInt8 difficulty)
{
	_extraCount[difficulty][idx] = count;
}

void Dungeon::pushEnterCountBS(UInt32 now)
{
    return;  // TODO
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.begin();
	UInt32 count[3] = {0};

	for(; it != _players.end(); it ++)
	{
		if(it->second.counterEnd > now && it->second.count[it->second.difficulty] != 0 && it->second.count[it->second.difficulty] <= 3)
			count[it->second.count[it->second.difficulty] - 1] ++;
	}
	DBLOG1().PushUpdateData("insert into `dungeon_count`(`server_id`, `dungeon_id`, `enter_players1`, `enter_players2`, `enter_players3`, `created_at`) values(%u, %u, %u, %u, %u, %u)", cfg.serverLogId, _id, count[0], count[1], count[2], now);
}

void Dungeon::doJusticeRoar(Player* pl)
{
	DungeonPlayerInfo& dpi = _players[pl];
    UInt8 justiceflag = dpi.justice[dpi.difficulty] / 25;
    switch(justiceflag)
    {
    case 1:
        dpi.justice_roar[dpi.difficulty] = 25;
        dpi.justice[dpi.difficulty] -= 25;
        break;
    case 2:
        dpi.justice_roar[dpi.difficulty] = 50;
        dpi.justice[dpi.difficulty] -= 50;
        break;
    case 3:
        dpi.justice_roar[dpi.difficulty] = 75;
        dpi.justice[dpi.difficulty] -= 75;
        break;
    case 4:
        dpi.justice_roar[dpi.difficulty] = 100;
        dpi.justice[dpi.difficulty] -= 100;
        break;
    default:
        return;
    }

    DB3().PushUpdateData("UPDATE `dungeon_player` SET `justice` = %u, `justice_roar` = %u WHERE `id` = %u AND `playerId` = %" I64_FMT "u", dpi.justice[dpi.difficulty], dpi.justice_roar[dpi.difficulty], _id, pl->getId());
    Stream st(REP::COPY_DATA_UPDATE);
    st << static_cast<UInt8>(7) << _id << dpi.difficulty << dpi.justice[dpi.difficulty] << Stream::eos;
    pl->send(st);
}

}

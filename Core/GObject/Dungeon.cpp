#include "Config.h"
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

UInt8 Dungeon::_maxCount = 1;
UInt16 Dungeon::_price[5] = {0, 50, 50};
size_t Dungeon::_priceCount = 0;

GGlobalObjectManagerT<Dungeon, UInt8> dungeonManager;
UInt8 Dungeon::_extraCount[11] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1};

Dungeon::Dungeon( UInt8 id, const GData::DungeonData * dd ): GObjectBaseT<Dungeon, UInt8>(id), _dungeon(dd)
{
	size_t levels = _dungeon->monsters.size();
    _levels.resize(levels);
    _recentReports.resize(levels);
}

Dungeon::~Dungeon()
{
}

void Dungeon::playerJump( Player * player, UInt8 level )
{
	if(level >= _levels.size())
		return;
	DungeonPlayerInfo& dpi = _players[player];
	dpi.level = level;
	updateToDB(player, dpi);
	sendDungeonLevelData(player, dpi);
}

UInt8 Dungeon::playerEnter( Player * player )
{
	if(player->GetLev() < _dungeon->levelReq)
		return 1;

	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	DungeonPlayerInfo* dpi;
	if(it == _players.end())
	{
		dpi = &_players[player];
		dpi->counterEnd = TimeUtil::SharpDay(1);
		++ dpi->count;
        dpi->level = 1;
		PLAYER_DATA(player, dungeonEnd) = TimeUtil::SharpDay(1);
        ++ PLAYER_DATA(player, dungeonCnt);
		sendDungeonInfo(player, *dpi);
	}
	else
	{
		UInt32 viplvl = player->getVipLevel();
		UInt8 extraCount = _extraCount[viplvl];
		dpi = &it->second;
		checkForTimeout(player, *dpi, false);
		leaveLevel(player, it->second.level);
		//if(it->second.level > 0)
		{
            if(cfg.GMCheck && PLAYER_DATA(player, dungeonCnt) >= getMaxCount() + extraCount)// fix gold less?
            {
                return 2;
            }

			if(PLAYER_DATA(player, dungeonCnt) >= getMaxCount())
			{
				UInt32 price = _price[PLAYER_DATA(player, dungeonCnt)];
				if(price > 0)
				{
					if(!player->hasChecked())
						return 3;
					if(player->getGold() < price)
						return 3;
					ConsumeInfo ci(VipEnterDungeon,0,0);
					player->useGold(price, &ci);
				}
			}

		}

        ++ dpi->count;
        ++ PLAYER_DATA(player, dungeonCnt);
        sendDungeonInfo(player, *dpi);
	}

    dpi->level = 1;

	sendDungeonLevelData(player, *dpi);
	enterLevel(player, dpi->level);
	updateToDB(player, *dpi);
	return 0;
}

UInt8 Dungeon::playerLeave( Player * player )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return 1;

	if(it->second.level == 0)
		return 0;

	leaveLevel(player, it->second.level);
	it->second.level = 0;
	updateToDB(player, it->second);
	return 0;
}

UInt8 Dungeon::playerContinue( Player * player )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return 1;

    if(it->second.level == 0)
        return 1;

    enterLevel(player, it->second.level);
    sendDungeonLevelData(player, it->second);
	return 0;
}

UInt8 Dungeon::playerBreak( Player * player )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return 1;

	leaveLevel(player, it->second.level);
	return 0;
}

void Dungeon::takeLoot( Player * player, DungeonPlayerInfo& dpi, UInt32& exp )
{
	const GData::DungeonLevel * dgl = _dungeon->monsters[dpi.level];
    if(!dgl)
        return;

	const GData::DungeonMonster * dm = dgl->monsterSet;
    if(!dm)
        return;

	exp = dm->exp;
	player->pendExp(exp);
	UInt32 itemId = 0;
	UInt16 gem3Id[] = {5003, 5013, 5023, 5033, 5043, 5053, 5063, 5073, 5083, 5093, 5103, 5113, 5123};
 	if(World::_wday == 5)
	{
		UInt8 bossIndex = getLevelBossIndex(_dungeon->getId(), dpi.level);
		if(uRand(100) < getGemLeve3Chance(_dungeon->getId(), bossIndex))
		{
			itemId = gem3Id[uRand(13)];
		}
	}
	if(World::_activityStage > 0)
	{
        // TODO: lua function
		GameAction()->onDungeonLootItemRoll(player, _dungeon->getId(), dpi.level, dm->formated);
		dgl->getLoot(player, itemId, static_cast<UInt8>(PLAYER_DATA(player, dungeonCnt) < 2 ? 40 : 0));
	}
	else
		dgl->getLoot(player, itemId, static_cast<UInt8>(PLAYER_DATA(player, dungeonCnt) < 2 ? 40 : 0));
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
		Stream stp(REP::COPY_DATA_UPDATE);
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
		broadcast(stb, player);
		if(dpi.lootToday.size() > 50)
		{
			std::list<DungeonItemInfo>::iterator iter = dpi.lootToday.begin();
			std::advance(iter, dpi.lootToday.size() - 50);
			dpi.lootToday.erase(dpi.lootToday.begin(), iter);
		}
		if(_recentLoots.size() > 50)
		{
			std::list<DungeonLootInfo>::iterator iter = _recentLoots.begin();
			std::advance(iter, _recentLoots.size() - 50);
			_recentLoots.erase(_recentLoots.begin(), iter);
		}
	}
}

bool Dungeon::doAttack( Player * player, UInt8 level)
{
	if(level >= _levels.size())
		return false;
	const GData::DungeonLevel * dgl = _dungeon->monsters[level];
	if(dgl == NULL)
		return false;
	const GData::DungeonMonster * dm = dgl->monsterSet;
	if(dm == NULL)
		return false;

    if(!dm->fighter)
        return false;

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
	UInt8 level = dpi.level;
	if(level >= _levels.size())
		return false;
	const GData::DungeonLevel * dgl = _dungeon->monsters[level];
	if(dgl == NULL)
		return false;
	const GData::DungeonMonster * dm = dgl->monsterSet;
	if(dm == NULL)
		return false;

    player->setJusticeRoar(dpi.justice_roar);
	Battle::BattleSimulator bsim(player->getLocation(), player, dm->fighter->getName(), dm->fighter->getLevel(), dm->formated);
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
		UInt32 seed = TimeUtil::SharpDay(0) + static_cast<UInt32>(player->getId()) + static_cast<UInt32>(player->getId() >> 32) + (static_cast<UInt32>(_id) << 24) + (static_cast<UInt32>(level) << 8) + (static_cast<UInt32>(PLAYER_DATA(player, dungeonCnt)) << 24);
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
    dpi.justice_roar = 0;
    DB3().PushUpdateData("UPDATE `dungeon_player` SET `justice_roar` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", dpi.justice_roar, _id, player->getId());

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
		std::list<DungeonReportInfo>& dril = _recentReports[level];
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
		player->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + turns_ * 2);
	}

	if(turns != NULL)
		*turns = turns_;

	st.init(REP::COPY_FIGHT_RESULT);
	st << _id << static_cast<UInt8>(res ? 0 : 1) << Stream::eos;
	player->send(st);

	return res;
}

bool Dungeon::advanceLevel( Player * player, DungeonPlayerInfo& dpi, bool noreport, UInt32 * totalExp, UInt32 gold )
{
    // TODO:
#if 0
	if(World::_activityStage > 0)
		GameAction()->onDungeonWin(player, _id, dpi.level + 1);
#endif
	UInt8 level = dpi.level;
	if(level >= _levels.size())
    {
        dpi.justice_roar = 0;
        dpi.justice = 0;
        DB3().PushUpdateData("UPDATE `dungeon_player` SET `justice` = %u, `justice_roar` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", dpi.justice, dpi.justice_roar, _id, player->getId());
		return true;
    }
	leaveLevel(player, level);

    if(dpi.justice < 100)
    {
        dpi.justice += 5;
        DB3().PushUpdateData("UPDATE `dungeon_player` SET `justice` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", dpi.justice, _id, player->getId());
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
	if(level < _levels.size())
	{
		dpi.level = level;
		enterLevel(player, level);
		DB3().PushUpdateData("UPDATE `dungeon_player` SET `level` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", dpi.level, _id, player->getId());
		r = false;
	}
	else
	{
		dpi.level = 0xFF;
		++ dpi.totalCount;
		if(dpi.firstPass == 0)
		{
			dpi.firstPass = TimeUtil::Now();
			DB3().PushUpdateData("UPDATE `dungeon_player` SET `level` = %u, `totalCount` = %u, `firstPass` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", dpi.level, dpi.totalCount, dpi.firstPass, _id, player->getId());
		}
		else
			DB3().PushUpdateData("UPDATE `dungeon_player` SET `level` = %u, `totalCount` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", dpi.level, dpi.totalCount, _id, player->getId());
		r = true;
		DBLOG1().PushUpdateData("insert into `dungeon_statistics` (`server_id`, `player_id`, `dungeon_id`, `this_day`, `pass_time`) values(%u, %"I64_FMT"u, %u, %u, %u)", cfg.serverLogId, player->getId(), _id, TimeUtil::SharpDay(0), TimeUtil::Now());
	}

	if(noreport)
	{
		if(r && gold > 0)
		{
			ConsumeInfo ci(DungeonAutoConsume, 0, 0);
			UInt16 gold_ = player->useGoldOrCoupon(gold, &ci);
			if(online)
			{
				Stream st_(REP::COPY_END_FIGHT);
				st_ << static_cast<UInt8>(0) << gold_ << static_cast<UInt16>(gold - gold_) << Stream::eos;
				player->send(st_);
			}
		}

		if(online)
		{
			Stream st(REP::COPY_AUTO_FIGHT);
			st << _id << static_cast<UInt8>(level - 1);
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

void Dungeon::startChallenge( Player * player )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
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
	st << _id << static_cast<UInt8>(it->second.level) << static_cast<UInt8>(0) << Stream::eos;
	player->send(st);
}

void Dungeon::processAutoChallenge( Player * player, UInt8 type, UInt32 * totalExp )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
	switch(type)
	{
	case 0:
		{
			UInt32 viplevel = player->getVipLevel();
			if(viplevel < 6)
			{
				const UInt32 taelReq[] = {
                    0,
                    GData::moneyNeed[GData::DUNGEON_AUTO1].tael,
                    GData::moneyNeed[GData::DUNGEON_AUTO2].tael,
                    GData::moneyNeed[GData::DUNGEON_AUTO3].tael,
                    GData::moneyNeed[GData::DUNGEON_AUTO4].tael,
                    GData::moneyNeed[GData::DUNGEON_AUTO5].tael,
                };

				if(player->getTael() < taelReq[_id])
					break;
				ConsumeInfo ci(DungeonAutoConsume, 0, 0);
				if(World::_wday == 5)
					player->useTael(taelReq[_id] / 2, &ci);
				else
					player->useTael(taelReq[_id], &ci);
			}
			DBLOG1().PushUpdateData("insert into `dungeon_statistics` (`server_id`, `player_id`, `dungeon_id`, `this_day`, `pass_time`) values(%u, %"I64_FMT"u, %u, %u, %u)", cfg.serverLogId, player->getId(), _id + 100, TimeUtil::SharpDay(0), TimeUtil::Now());
			Stream st(REP::COPY_AUTO_FIGHT);
			st << _id << static_cast<UInt8>(it->second.level) << static_cast<UInt8>(0) << Stream::eos;
			player->send(st);
		}
		break;
    case 1:
        if(advanceLevel(player, it->second, true, totalExp))
        {
            player->delFlag(Player::AutoDungeon);
            DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %"I64_FMT"u", player->getId());
            return;
        }
        if(player->GetPackage()->GetRestPackageSize() < 4)
        {
            player->delFlag(Player::AutoDungeon);
            Stream st(REP::COPY_AUTO_FIGHT);
            st << _id << static_cast<UInt8>(it->second.level) << static_cast<UInt8>(5) << *totalExp << Stream::eos;
			player->send(st);
			DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %"I64_FMT"u", player->getId());
			return;
		}
        break;
	case 2:
		{
			player->delFlag(Player::AutoDungeon);
			Stream st(REP::COPY_AUTO_FIGHT);
			st << _id << static_cast<UInt8>(it->second.level) << static_cast<UInt8>(2) << *totalExp << Stream::eos;
			player->send(st);
			DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %"I64_FMT"u", player->getId());
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
	DB3().PushUpdateData("REPLACE INTO `dungeon_auto`(`playerId`, `dungeonId`, `totalExp`, `won`) VALUES(%"I64_FMT"u, %u, %u, %u)", player->getId(), _id, *totalExp, r ? 1 : 0);
	EventDungeonAuto * event = new(std::nothrow) EventDungeonAuto(player, 2 * turns, this, r, *totalExp);
	PushTimerEvent(event);
}

void Dungeon::cancelAutoChallengeNotify( Player * player, UInt32 exp )
{
	player->delFlag(Player::AutoDungeon);
	DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %"I64_FMT"u", player->getId());
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
	Stream st(REP::COPY_AUTO_FIGHT);
	st << _id << it->second.level << static_cast<UInt8>(1) << exp << Stream::eos;
	player->send(st);
}

void Dungeon::completeAutoChallenge( Player * player, UInt32 exp, bool won )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
	UInt32 maxCount = player->getCoupon() + player->getGold();
	UInt32 count = 0;
	while(1)
	{
		count += GData::moneyNeed[GData::DUNGEON_IM].gold;
		if(won)
		{
			if(advanceLevel(player, it->second, true, &exp, count))
			{
				player->delFlag(Player::AutoDungeon);
				DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %"I64_FMT"u", player->getId());
                return;
			}
			if(count < maxCount)
				won = doChallenge(player, it->second, false, NULL);
			else
				break;
		}
		else
		{
			player->delFlag(Player::AutoDungeon);
			ConsumeInfo ci(DungeonAutoConsume, 0, 0);
			UInt16 gold = player->useGoldOrCoupon(count, &ci);
			Stream st_(REP::COPY_END_FIGHT);
			st_ << static_cast<UInt8>(0) << gold << static_cast<UInt16>(count - gold) << Stream::eos;
			player->send(st_);
			Stream st(REP::COPY_AUTO_FIGHT);
			st << _id << static_cast<UInt8>(it->second.level) << static_cast<UInt8>(2) << exp << Stream::eos;
			player->send(st);
			DB3().PushUpdateData("DELETE FROM `dungeon_auto` WHERE `playerId` = %"I64_FMT"u", player->getId());
            return;
		}
	}
	ConsumeInfo ci(DungeonAutoConsume, 0, 0);
	UInt16 gold = player->useGoldOrCoupon(count, &ci);
	Stream st_(REP::COPY_END_FIGHT);
	st_ << static_cast<UInt8>(1) << gold << static_cast<UInt16>(count - gold) << Stream::eos;
	player->send(st_);
	processAutoChallenge(player, 3, &exp);
}

void Dungeon::autoChallenge( Player * player )
{
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
	player->addFlag(Player::AutoDungeon);
	UInt32 exp = 0;
	processAutoChallenge(player, 0, &exp);
}

void Dungeon::pushChallenge( Player * player, UInt32 exp, bool won )
{
	player->addFlag(Player::AutoDungeon);
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
    if (it == _players.end())
        return;
	EventDungeonAuto * event = new(std::nothrow) EventDungeonAuto(player, 1 + uRand(20), this, won, exp);
	PushTimerEvent(event);
}

void Dungeon::pushPlayer( Player * player, UInt8 level, UInt8 count, UInt16 totalCount, UInt32 firstPass, UInt32 counterEnd, UInt8 justice, UInt8 justice_roar )
{
	DungeonPlayerInfo& dpi = _players[player];
	dpi.level = level;
	dpi.count = count;
	dpi.totalCount = totalCount;
    dpi.firstPass = firstPass;
	dpi.counterEnd = counterEnd;
	dpi.justice = justice;
    dpi.justice_roar = justice_roar;
	if(level >= _levels.size())
		return;
	DungeonLevel& dl = _levels[level];
	dl.singles.insert(player);
}

void Dungeon::sendDungeonInfo(Player * player)
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
	{
		Stream st(REP::COPY_DATA_UPDATE);
		UInt8 enterCount = (_extraCount[player->getVipLevel()] << 4) | getEnterCount();
		st << static_cast<UInt8>(0) << _id << static_cast<UInt8>(0) << PLAYER_DATA(player, dungeonCnt) << enterCount << static_cast<UInt16>(0) << static_cast<UInt32>(0) << static_cast<UInt8>(0) << Stream::eos;
		player->send(st);
		return;
	}
	checkForTimeout(player, it->second, true);
	sendDungeonInfo(player, it->second);
}

void Dungeon::sendDungeonInfo(Player * player, DungeonPlayerInfo& dpi)
{
	Stream st(REP::COPY_DATA_UPDATE);
	UInt8 enterCount = (_extraCount[player->getVipLevel()] << 4) | getEnterCount();
	st << static_cast<UInt8>(0) << _id << static_cast<UInt8>(dpi.level) << PLAYER_DATA(player, dungeonCnt) << enterCount << dpi.totalCount << dpi.firstPass << dpi.justice << Stream::eos;
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
	if(dpi.level >= _levels.size())
	{
		Stream st(REP::COPY_DATA_UPDATE);
		st << static_cast<UInt8>(5) << _id << static_cast<UInt8>(0xFF) << dpi.justice << static_cast<UInt8>(0) << static_cast<UInt8>(0) << Stream::eos;
		player->send(st);
		return;
	}
	const GData::DungeonLevel * dgl = _dungeon->monsters[dpi.level];
	if(dgl == NULL)
		return;
	Stream st(REP::COPY_DATA_UPDATE);
	st << static_cast<UInt8>(5) << _id << static_cast<UInt8>(dpi.level) << dpi.justice << static_cast<UInt8>(1);
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
	std::list<DungeonReportInfo>& dril = _recentReports[dpi.level];
	st << static_cast<UInt8>(dril.size());
	for(std::list<DungeonReportInfo>::iterator it = dril.begin(); it != dril.end(); ++ it)
	{
		st << it->player->getCountry() << it->player->getName() << it->id;
	}
	st << Stream::eos;
	player->send(st);
}

void Dungeon::enterLevel( Player * player, UInt8 level )
{
	if(level >= _levels.size())
		return;
	DungeonLevel& dl = _levels[level];
	if(dl.singles.find(player) != dl.singles.end())
		return;
	dl.singles.insert(player);
}

void Dungeon::leaveLevel( Player * player, UInt8 level )
{
	if(level >= _levels.size())
		return;
	DungeonLevel& dl = _levels[level];
	std::set<Player *>::iterator it = dl.singles.find(player);
	if(it == dl.singles.end())
		return;
	dl.singles.erase(it);
}

void Dungeon::updateToDB( Player * player, DungeonPlayerInfo& dpi )
{
	DB3().PushUpdateData("REPLACE INTO `dungeon_player`(`id`, `playerId`, `level`, `count`, `totalCount`, `firstPass`, `counterEnd`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u, %u)", _id, player->getId(), dpi.level, dpi.count, dpi.totalCount, dpi.firstPass, dpi.counterEnd);

	DB1().PushUpdateData("UPDATE `player` SET `dungeonCnt` = %d, `dungeonEnd` = %u where `id` = %"I64_FMT"u", PLAYER_DATA(player, dungeonCnt), PLAYER_DATA(player, dungeonEnd), player->getId());
}

void Dungeon::checkForTimeout( Player * player, DungeonPlayerInfo& dpi, bool writeDB )
{
	UInt32 now = TimeUtil::Now();
	if(now >= PLAYER_DATA(player, dungeonEnd))
	{
        dpi.counterEnd = TimeUtil::SharpDay(1);
        dpi.count = 0;
		PLAYER_DATA(player, dungeonEnd) = dpi.counterEnd;
		PLAYER_DATA(player, dungeonCnt) = 0;
		dpi.lootToday.clear();
		if(writeDB)
		{
			DB3().PushUpdateData("UPDATE `dungeon_player` SET `count` = 0, `counterEnd` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", dpi.counterEnd, _id, player->getId());

            DB1().PushUpdateData("UPDATE `player` SET `dungeonCnt` = %u, `dungeonEnd` = %u where `id` = %"I64_FMT"u", PLAYER_DATA(player, dungeonCnt), PLAYER_DATA(player, dungeonEnd), player->getId());
		}
	}
}

void Dungeon::sendMyLootInfo( Player * player )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return;
	{
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
	}
}

void Dungeon::broadcast( Stream& st, Player * player )
{
	std::vector<DungeonLevel>& dll = _levels;
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

UInt32 Dungeon::getFirstPass( Player * player )
{
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.find(player);
	if(it == _players.end())
		return 0;
	return it->second.firstPass;
}

void Dungeon::setMaxCount(UInt32 cnt)
{
	_maxCount = cnt;
}

void Dungeon::setPrice( UInt32 idx, UInt16 price )
{
	_price[idx] = price;
	if(idx >= _priceCount)
		_priceCount = idx + 1;
}

UInt16 * Dungeon::getPrice( size_t& size )
{
	size = _priceCount;
	return _price;
}

void Dungeon::setExtraCount( UInt32 idx, UInt8 count )
{
	_extraCount[idx] = count;
}

void Dungeon::pushEnterCountBS(UInt32 now)
{
    return;  // TODO
	std::map<Player *, DungeonPlayerInfo>::iterator it = _players.begin();
	UInt32 count[3] = {0};

	for(; it != _players.end(); it ++)
	{
		if(it->second.counterEnd > now && it->second.count != 0 && it->second.count <= 3)
			count[it->second.count - 1] ++;
	}
	DBLOG1().PushUpdateData("insert into `dungeon_count`(`server_id`, `dungeon_id`, `enter_players1`, `enter_players2`, `enter_players3`, `created_at`) values(%u, %u, %u, %u, %u, %u)", cfg.serverLogId, _id, count[0], count[1], count[2], now);
}

void Dungeon::doJusticeRoar(Player* pl)
{
	DungeonPlayerInfo& dpi = _players[pl];
    UInt8 justiceflag = dpi.justice / 25;
    switch(justiceflag)
    {
    case 1:
        dpi.justice_roar = 25;
        dpi.justice -= 25;
        break;
    case 2:
        dpi.justice_roar = 50;
        dpi.justice -= 50;
        break;
    case 3:
        dpi.justice_roar = 75;
        dpi.justice -= 75;
        break;
    case 4:
        dpi.justice_roar = 100;
        dpi.justice -= 100;
        break;
    default:
        return;
    }

    DB3().PushUpdateData("UPDATE `dungeon_player` SET `justice` = %u, `justice_roar` = %u WHERE `id` = %u AND `playerId` = %"I64_FMT"u", dpi.justice, dpi.justice_roar, _id, pl->getId());
    Stream st(REP::COPY_DATA_UPDATE);
    st << static_cast<UInt8>(6) << _id << dpi.justice << Stream::eos;
    pl->send(st);
}

}

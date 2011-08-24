#include "Config.h"
#include "Boss.h"
#include "GData/NpcGroup.h"
#include "Player.h"
#include "Fighter.h"
#include "Country.h"
#include "Map.h"
#include "Mail.h"
#include "Package.h"
#include "Leaderboard.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Battle/BattleSimulator.h"
#include "Script/GameActionLua.h"
#include "Common/TimeUtil.h"
#include "AthleticsRank.h"
#include "MsgID.h"

namespace GObject
{

Boss::Boss( UInt32 id ) : _hppercent(0), _level(0), _ng(GData::npcGroups[id])
{
    if (_ng) {
        std::vector<GData::NpcFData>& nflist = _ng->getList();
        _hp.resize(nflist.size());
    }
	_checkFBReward = false;
}

bool Boss::attackBy( Player * player )
{
    if (!_ng) return false;
	Battle::BattleSimulator bsim(player->getLocation(), player, _ng->getName(), _ng->getLevel(), false);
	player->PutFighters(bsim, 0);
	std::vector<GData::NpcFData>& nflist = _ng->getList();
	size_t sz = nflist.size();
	bsim.setFormation(1, _ng->getFormation());
	bsim.setPortrait(1, nflist[0].fighter->favor);
	for(size_t i = 0; i < sz; ++ i)
	{
		if(_hp[i] == 0xFFFFFFFF)
			continue;
		GData::NpcFData& nfdata = nflist[i];
		Battle::BattleFighter * bf = bsim.newFighter(1, nfdata.pos, nfdata.fighter);
		bf->setHP(_hp[i]);
	}
	bsim.start();
	if(World::_activityStage > 0)
		GameAction()->onAttackBoss(player);
	if(_checkFBReward)
	{
		firstBloodReward(player);
		_checkFBReward = false;
	}
	Stream& packet = bsim.getPacket();
	if(packet.size() <= 8)
		return false;
	bool res = bsim.getWinner() == 1;
	Stream st(REP::ATTACK_NPC);
	if(res)
		st << static_cast<UInt16>(0x0101);
	else
		st << static_cast<UInt16>(0x0100);
	st << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
	st.append(&packet[8], packet.size() - 8);
	st << Stream::eos;
	player->send(st);
	bsim.applyFighterHP(0, player);
	UInt32 oldHP = _hp[0];
	for(size_t i = 0; i < sz; ++ i)
	{
		GData::NpcFData& nfdata = nflist[i];
		Battle::BattleObject * obj = bsim(1, nfdata.pos);
		if(obj == NULL || !obj->isChar())
			continue;
		Battle::BattleFighter * bfgt = static_cast<Battle::BattleFighter *>(obj);
		UInt32 nHP = bfgt->getHP();
		if(nHP == 0)
			nHP = 0xFFFFFFFF;
		if(_hp[i] != 0xFFFFFFFF && _hp[i] != nHP)
		{
			if(i == 0 && nHP != 0xFFFFFFFF)
			{
				UInt32 nPercent = (nHP - 1) * 10 / bfgt->getMaxHP();
				if(nPercent < 9 && nPercent != _hppercent)
				{
					_hppercent = nPercent;
					//SYSMSG_BROADCASTV(405, player->getCountry(), player->getName().c_str(), _ng->getId(), (nPercent + 1) * 10);
					Stream st(REP::DAILY_DATA);
					st << static_cast<UInt8>(2);
					bossManager.buildCurrBossInfo(st, (nPercent + 1) * 10);
					st << Stream::eos;
					NETWORK()->Broadcast(st);
				}
			}
			_hp[i] = nHP;
			DB().PushUpdateData("REPLACE INTO `boss`(`id`, `level`, `pos`, `hp`) VALUES(%u, %u, %u, %u)", _ng->getId(), _level, i, _hp[i]);
		}
	}
	UInt32 now = TimeUtil::Now();
	player->setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns() * 2);
	DamageInfo& dmg = _damage[player];
	const UInt32 dur[] = {20, 30, 40, 50, 60};
	if(dmg.count > 3)
		player->setBuffData(PLAYER_BUFF_BOSSWEAK, now + dur[4]);
	else
		player->setBuffData(PLAYER_BUFF_BOSSWEAK, now + dur[dmg.count]);
	++ dmg.count;
	if(oldHP == 0xFFFFFFFF)
		return res;
	if(oldHP == 0)
		oldHP = nflist[0].fighter->getMaxHP();
	UInt32 newHP = (_hp[0] == 0xFFFFFFFF) ? 0 : _hp[0];
	if(oldHP > newHP)
	{
		UInt32 damage = oldHP - newHP;
		dmg.damage += damage;
		UInt32 exp;
		UInt8 lev = player->GetLev();
		if(World::_wday == 7)
		{
			if(lev >= 120)
				exp = damage * 2;
			else if(lev >= 110)
				exp = damage * 9 / 5;
			else if(lev >= 100)
				exp = damage * 8 / 5;
			else if(lev >= 90)
				exp = damage * 7 / 5;
			else if(lev >= 80)
				exp = damage * 6 / 5;
			else
				exp = damage;
		}
		else
		{
			if(lev >= 120)
				exp = damage;
			else if(lev >= 110)
				exp = damage * 9 / 10;
			else if(lev >= 100)
				exp = damage * 4 / 5;
			else if(lev >= 90)
				exp = damage * 7 / 10;
			else if(lev >= 80)
				exp = damage * 3 / 5;
			else
				exp = damage / 2;
		}
		dmg.exp += exp;
		DB().PushUpdateData("REPLACE INTO `boss_damage`(`id`, `playerId`, `damage`, `exp`) VALUES(%u, %"I64_FMT"u, %u, %u)", _ng->getId(), player->getId(), dmg.damage, dmg.exp);
		player->pendExp(exp, !res);
	}
	else
		player->pendExp(0, !res);
	UInt32 newPercent = newHP * 10 / nflist[0].fighter->getMaxHP();
	if(newPercent <= 4 && oldHP * 10 / nflist[0].fighter->getMaxHP() > 4 && newHP != 0)
		BroadCastTV(nflist[0].fighter->getMaxHP());
	return res;
}

void Boss::BroadCastTV(UInt32 damage)
{
	if(_damage.size() == 0)
		return;
	std::multimap<UInt32, Player *, std::greater<UInt32> > expList;
	std::map<Player *, DamageInfo>::iterator it;
	for(it = _damage.begin(); it != _damage.end(); ++ it)
	{
		if(expList.size() >= 3)
		{
			std::multimap<UInt32, Player *, std::greater<UInt32> >::iterator explstEnd = expList.end();
			explstEnd --;
			if(explstEnd->first >= it->second.damage)
				continue;
			else
				expList.erase(explstEnd);
		}
		expList.insert(std::make_pair(it->second.damage, it->first));
	}

	std::multimap<UInt32, Player *, std::greater<UInt32> >::iterator iter;
	Player *playerArr[3] = {NULL, NULL, NULL};
	UInt32 damageArr[3] = {0, 0, 0};
	UInt32 j = 0;
	for(iter = expList.begin(); iter != expList.end(); iter ++, j ++)
	{
		playerArr[j] = iter->second;
		damageArr[j] = iter->first * 100 / damage;
	}
	if(j == 3)
		SYSMSG_BROADCASTV(265, playerArr[0]->getCountry(), playerArr[0]->getName().c_str(), damageArr[0], playerArr[1]->getCountry(), playerArr[1]->getName().c_str(), damageArr[1], playerArr[2]->getCountry(), playerArr[2]->getName().c_str(), damageArr[2])
	else if(j == 2)
		SYSMSG_BROADCASTV(266, playerArr[0]->getCountry(), playerArr[0]->getName().c_str(), damageArr[0], playerArr[1]->getCountry(), playerArr[1]->getName().c_str(), damageArr[1])
	else
		SYSMSG_BROADCASTV(267, playerArr[0]->getCountry(), playerArr[0]->getName().c_str(), damageArr[0]);

}

void Boss::firstBloodReward(Player *player)
{
	MailPackage::MailItem Items[] = {{MailPackage::Tael, 50}, {8933, 1}, {8912, 1}, {8915, 1}, {8926, 1}, {8918, 1}, {8910, 1}};
	UInt32 rand = uRand(7);
	SYSMSG(title, 231);
	Mail *pmail = NULL;
	MailItemsInfo itemsInfo(&Items[rand], BossFBAward, 1);
	if(rand != 0)
	{
		const GData::ItemBaseType *item = GData::itemBaseTypeManager[Items[rand].id];
		SYSMSGV(content, 281, _ng->getName().c_str(), _ng->getName().c_str(), item->getName().c_str());
		pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
	}
	else
	{
		SYSMSGV(content, 280, _ng->getName().c_str(), _ng->getName().c_str());
		pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);	
	}
	if(pmail != NULL)
		mailPackageManager.push(pmail->id, Items[rand].id, Items[rand].count, true);
	SYSMSG_BROADCASTV(282, player->getCountry(), player->getName().c_str(), _ng->getName().c_str());
}

struct _PEData
{
	Player * player;
	UInt32 rank;
	UInt32 exp;
};

void Boss::onReward()
{
	if(_damage.size() == 0)
		return;
	std::multimap<UInt32, _PEData, std::greater<UInt32> > expList;
	std::map<Player *, DamageInfo>::iterator it;
	for(it = _damage.begin(); it != _damage.end(); ++ it)
	{
		_PEData pd = {it->first, 0, it->second.exp};
		expList.insert(std::make_pair(it->second.damage, pd));
	}
	UInt32 idx = 0;
	std::multimap<UInt32, _PEData, std::greater<UInt32> >::iterator iter;
	for(iter = expList.begin(); iter != expList.end(); ++ iter)
	{
		iter->second.rank = ++ idx;
	}

	static UInt32 rewardId[3] = {8927, 8913};
	UInt8 extraAwardNum = 0;
	std::vector<GData::NpcFData>& nflist = _ng->getList();
	UInt32 maxHp = nflist[0].fighter->getMaxHP();

	const GData::ItemBaseType *item1 = NULL;
	const GData::ItemBaseType *item2 = NULL;

	SYSMSG(title, 231);
	iter = expList.begin();

	Player *player_arr[3] = {NULL, NULL, NULL};
	UInt32 exp[3] = {0, 0, 0};
	double damage_percert[3] = {0, 0, 0};	
	std::string playerName[] = {"", "", ""};
	for(UInt16 i = 0; i < 3 && iter != expList.end(); i ++, iter ++)
	{
		player_arr[i] = iter->second.player;
		exp[i] = iter->first;
		damage_percert[i] = static_cast<double>(exp[i]) / static_cast<double>(maxHp) * 100;
		playerName[i] = player_arr[i]->getName();
	}
	
	Mail *pmail = NULL;	
	do
	{		
		iter = expList.begin();
		Player * player = iter->second.player;
		UInt32 rank = iter->second.rank;
		UInt16 itemId1 = 0, count1 = 0, itemId2 = 0, count2 = 0;
		getRandomItemId(player_arr[0], itemId1, count1, itemId2, count2, extraAwardNum);
		item1 = GData::itemBaseTypeManager[itemId1];
		item2 = GData::itemBaseTypeManager[itemId2];
		if(item1 ==NULL || item2 == NULL)
			break;
		SYSMSGV(content, 233, item1->getName().c_str(), item2->getName().c_str(), iter->second.exp, _ng->getName().c_str(), playerName[0].c_str(), damage_percert[0], playerName[1].c_str(), damage_percert[1],  playerName[2].c_str(), damage_percert[2], rank);
		MailPackage::MailItem mitem[3] = {{itemId1, count1}, {itemId2, count2}, {8934, 1}};
		MailItemsInfo itemsInfo(mitem, BossAward, 3);
		pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
		if(pmail != NULL)
		{
			mailPackageManager.push(pmail->id, mitem, 3, false);
		}
		expList.erase(iter);
	} while(0);

	idx = 0;
	while(!expList.empty() && idx < 2)
	{
		iter = expList.begin();
		Player * player = iter->second.player;
		UInt32 rank = iter->second.rank;
		const GData::ItemBaseType *item = GData::itemBaseTypeManager[rewardId[idx]];
		SYSMSGV(content, 276, item->getName().c_str(), iter->second.exp, _ng->getName().c_str(), playerName[0].c_str(), damage_percert[0], playerName[1].c_str(), damage_percert[1],  playerName[2].c_str(), damage_percert[2], rank);
		MailPackage::MailItem mitem[2] = {{8934, 1}, {static_cast<UInt16>(rewardId[idx]), 1}};
		MailItemsInfo itemsInfo(mitem, BossAward, 2);
		pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);

		if(pmail != NULL)
		{
			mailPackageManager.push(pmail->id, mitem, 2, false);
		}
		expList.erase(iter);
		idx ++;
	}
	if(!expList.empty())
	{
		iter = expList.begin();
		Player * player = iter->second.player;
		UInt32 rank = iter->second.rank;
		const GData::ItemBaseType *item = GData::itemBaseTypeManager[8934];
		SYSMSGV(content, 278, item->getName().c_str(), iter->second.exp, _ng->getName().c_str(), playerName[0].c_str(), damage_percert[0], playerName[1].c_str(), damage_percert[1], playerName[2].c_str(), damage_percert[2], rank);
		MailPackage::MailItem mitem[1] = {{8934, 1}};
		MailItemsInfo itemsInfo(mitem, BossAward, 1);
		player->GetMailBox()->newMail(NULL, 0x21, title, content, (1 << 16) | 8934, true, &itemsInfo);
		expList.erase(iter);
	}
	idx = 0;
	while (!expList.empty() && idx < extraAwardNum)//随机初级升级加速符
	{
		UInt32 rand = uRand(expList.size());
		iter = expList.begin();
		std::advance(iter, rand);
		Player * player = iter->second.player;
		UInt32 rank = iter->second.rank;
		const GData::ItemBaseType *item = GData::itemBaseTypeManager[8933];
		SYSMSGV(content, 278, item->getName().c_str(), iter->second.exp, _ng->getName().c_str(), playerName[0].c_str(), damage_percert[0], playerName[1].c_str(), damage_percert[1], playerName[2].c_str(), damage_percert[2], rank + rand);//rank error fix..
		MailPackage::MailItem mitem[1] = {{8933, 1}};
		MailItemsInfo itemsInfo(mitem, BossAward, 1);
		player->GetMailBox()->newMail(NULL, 0x21, title, content, (1 << 16) | 8933, true, &itemsInfo);
		expList.erase(iter);
		idx ++;
	}
	for(iter = expList.begin(); iter != expList.end(); ++ iter)
	{
		Player * player = iter->second.player;
		UInt32 rank = iter->second.rank;

		SYSMSGV(content, 232, iter->second.exp, _ng->getName().c_str(), playerName[0].c_str(), damage_percert[0], playerName[1].c_str(), damage_percert[1], playerName[2].c_str(), damage_percert[2], rank);
		player->GetMailBox()->newMail(NULL, 1, title, content);
	}

	if(player_arr[2] != NULL && item1 != NULL && item2 != NULL)
	{
		SYSMSG_BROADCASTV(279, _ng->getName().c_str(), player_arr[0]->getCountry(), player_arr[0]->getName().c_str(), item1->getName().c_str(), item2->getName().c_str(), player_arr[1]->getCountry(), player_arr[1]->getName().c_str(), player_arr[2]->getCountry(), player_arr[2]->getName().c_str())
	}

}

void Boss::getRandomItemId(Player *player, UInt16& itemId1,UInt16& count1, UInt16& itemId2, UInt16& count2, UInt8& extraAwardNum)
{
	UInt32 bossId = _ng->getId();
	UInt16 lev = player->GetLev();
	if(bossId == 4369)//boss id 秦仙人 fix 4369
	{
		extraAwardNum = 8;
		UInt32 rand1 = uRand(100);
		if(rand1 < 50)
		{
			UInt32 rand_equip = uRand(28);
			if(lev >= 100)
				itemId1 = 9214;
			if(lev >= 90)
				itemId1 = 9031;
			if(lev >= 80)
				itemId1 = GObject::OrangeEquip80[rand_equip];
			else if(lev >= 70)
				itemId1 = GObject::OrangeEquip70[rand_equip];
			else
				itemId1 = GObject::OrangeEquip50[rand_equip];
		}
		else
		{
			itemId1 = 8927;//enchant protect
		}
		count1 = 1;

		UInt32 rand2 = uRand(100);
		if(rand2 < 75)
		{
			if(lev >= 100)
				itemId2 = GObject::DietyFavor[uRand(4)];
			else
				itemId2 = GObject::OrangeFavor[uRand(5)];
		}
		else
		{
			itemId2 = 9000;//potential protect
		}
		count2 = 2;
	}
	else if(bossId == 4367 || bossId == 4368)//bossId 雨迦神272(4368)、岱魔王271(4367) fix
	{
		if(bossId == 4367)
			extraAwardNum = 3;
		else
			extraAwardNum = 5;
		itemId1 = 8927;//enchant protect
		count1 = 1;
		UInt32 rand = uRand(100);
		if(rand < 75)
		{
			if(lev >= 100)
				itemId2 = GObject::DietyFavor[uRand(4)];
			else
				itemId2 = GObject::OrangeFavor[uRand(5)];
		}
		else
		{
			itemId2 = 9000;
		}
		count2 = 1;
	}
}

void Boss::reset()
{
	size_t sz = _hp.size();
	if(sz > 0)
	{
		if(_hp[0] == 0xFFFFFFFF)
		{
			_hppercent = 254;
		}
		else
		{
			_hp[0] = 0xFFFFFFFE;
			_hppercent = 255;
			DB().PushUpdateData("REPLACE INTO `boss`(`id`, `level`, `pos`, `hp`) VALUES(%u, %u, 0, %u)", _ng->getId(), _level, _hp[0]);
		}
	}
	else
		_hppercent = 0;
	for(size_t i = 0; i < sz; ++ i)
	{
		_hp[i] = 0;
	}
	_damage.clear();
	DB().PushUpdateData("DELETE FROM `boss` WHERE `id` = %u AND `pos` > 0", _ng->getId());
	DB().PushUpdateData("DELETE FROM `boss_damage` WHERE `id` = %u", _ng->getId());
}

void Boss::setHP( UInt8 pos, UInt32 hp )
{
	if(pos >= _hp.size())
		return;
	if(pos == 0)
	{
		if(hp == 0xFFFFFFFF)
		{
			_hppercent = 254;
			hp = 0;
		}
		else if(hp == 0xFFFFFFFE)
		{
			_hppercent = 255;
			hp = 0;
		}
		else
			_hppercent = 200;
	}
	_hp[pos] = hp;
}

void Boss::setDamage( Player * player, UInt32 damage, UInt32 exp )
{
	DamageInfo& di = _damage[player];
	di.damage = damage;
	di.exp = exp;
}

UInt16 Boss::getId()
{
    if (_ng)
        return _ng->getId();
    return 0;
}

const std::string& Boss::getName()
{
    static std::string nullstr;
    if (_ng)
        return _ng->getName();
    return nullstr;
}

UInt8 Boss::getHPPercent()
{
	if(_hppercent == 254)
		return 2;
	if(_hppercent == 255)
		return 1;
	if(_hppercent == 200 && _hp.size() > 0)
		_hppercent = (_hp[0] - 1) * 10 / _ng->getList()[0].fighter->getMaxHP();
	return (_hppercent + 1) * 10;
}

void Boss::appear()
{
	size_t cnt = _hp.size();
	std::vector<GData::NpcFData>& nflist = _ng->getList();
	for(size_t i = 0; i < cnt; ++ i)
	{
		if(_hp[i] == 0 || _hp[i] >= 0xFFFFFFF0)
		{
			_hp[i] = nflist[i].fighter->getMaxHP();
			DB().PushUpdateData("REPLACE INTO `boss`(`id`, `level`, `pos`, `hp`) VALUES(%u, %u, %u, %u)", _ng->getId(), _level, i, _hp[i]);
		}
	}
	_hppercent = (_hp[0] - 1) * 10 / nflist[0].fighter->getMaxHP();
	bossManager.getNextBoss();
	Stream st(REP::DAILY_DATA);
	st << static_cast<UInt8>(2);
	bossManager.buildInfo(st);
	st << Stream::eos;
	NETWORK()->Broadcast(st);
	_checkFBReward = true;
}

void Boss::setLevel(UInt8 lvl, bool writedb)
{
	if(lvl > 200 || _level == lvl || _hp.size() == 0)
		return;
	std::vector<GData::NpcFData>& nflist = _ng->getList();
	Fighter * fgt = nflist[0].fighter;
	fgt->maxhp = fgt->maxhp - 100000 * _level + 100000 * lvl;
	fgt->attack = fgt->attack - 85 * _level + 85 * lvl;
	fgt->defend = fgt->defend - 120 * _level + 120 * lvl;
	fgt->action = fgt->action + 5 * _level - 5 * lvl;
	fgt->setDirty(true);
	_level = lvl;
	if(writedb)
		DB().PushUpdateData("UPDATE `boss` SET `level` = %u WHERE `id` = %u AND `pos` = 0", _level, _ng->getId());
}

void Boss::pushAttackerCountBS(bool isDead, UInt32 spawnTime)
{
	UInt32 now = TimeUtil::Now();
	int bossStay = 0;//
	UInt32 size = _damage.size();
	if(isDead)
		bossStay = now - spawnTime;
	else
		bossStay = 0 - _hp[0];
	DBLOG().PushUpdateData("insert into `boss_states`(`server_id`, `boss_id`, `boss_level`, `attacker_count`, `boss_status`, `created_at`) values(%u, %u, %u, %u, %d, %u)", cfg.serverLogId, getId(), _level, size, bossStay, now);
}

BossManager bossManager;

BossManager::BossManager()
{
	UInt32 now = TimeUtil::Now();
	_nextBroadcast = now / 1800 * 1800 + 1800;
	_prevBoss = 0;
	_currBoss = 0;
	_nextBoss = 0;
	_prevBossLevel = 0;
	_currBossLevel = 0;
	_nextBossLevel = 0;
	_prevBossPHP = 0;
	_currBossPHP = 0;
}

BossManager::~BossManager()
{
	std::map<UInt32, BossStruct>::iterator it;
	for(it = _bossOff.begin(); it != _bossOff.end(); ++ it)
	{
		delete it->second.boss;
	}
	for(it = _bossOn.begin(); it != _bossOn.end(); ++ it)
	{
		delete it->second.boss;
	}
	_bossOn.clear();
	_bossOff.clear();
}

void BossManager::addBoss( UInt32 id, UInt16 loc, UInt32 spawn, UInt32 flee, UInt8 lvl )
{
	BossStruct bs;
	bs.boss = new Boss(id);
	bs.location = loc;
	UInt32 tm = TimeUtil::SharpDay(-1);
	bs.spawnTime = tm + spawn;
	bs.fleeTime = bs.spawnTime + flee;
	bs.appearLevel = lvl;
	while(bs.fleeTime < TimeUtil::Now())
	{
		bs.spawnTime += 86400;
		bs.fleeTime += 86400;
	}
	bs.prepareTime = bs.spawnTime - 300;
	_bossOff[id] = bs;
}

void BossManager::process( UInt32 curtime )
{
	bool broadcast = curtime >= _nextBroadcast;
	if(broadcast)
		_nextBroadcast += 1800;
	UInt8 maxLevel = leaderboard.getMaxLevel();
	std::map<UInt32, BossStruct>::iterator it = _bossOff.begin();
	while(it != _bossOff.end())
	{
		BossStruct& bs = it->second;
		if(bs.appearLevel > maxLevel)
		{
			++ it;
			continue;
		}
		if(bs.spawnTime <= curtime)
		{
			if(bs.fleeTime <= curtime || (bs.spawnTime + 600 <= curtime && !it->second.boss->isAlive()))
			{
				bs.spawnTime += 86400;
				bs.fleeTime += 86400;
				bs.prepareTime = bs.spawnTime - 300;
				++ it;
				continue;
			}
			it->second.boss->appear();
			Map * map = Map::FromSpot(it->second.location);
			if(map != NULL)
				map->Show(it->second.boss->getId());
			//SYSMSG_BROADCASTV(400, it->second.boss->getId(), it->second.location);
			_bossOn[it->first] = it->second;
			_bossOff.erase(it ++);
		}
		else if(bs.prepareTime <= curtime)
		{
			//SYSMSG_BROADCASTV(404, it->second.location, it->second.boss->getId());
			bs.prepareTime += 60;
		}
		else
			++ it;
	}
	it = _bossOn.begin();
	while(it != _bossOn.end())
	{
		BossStruct& bs = it->second;
		if(bs.fleeTime <= curtime)
		{
			getNextBoss();
			Stream st(REP::DAILY_DATA);
			st << static_cast<UInt8>(2);
			buildInfo(st);
			st << Stream::eos;
			NETWORK()->Broadcast(st);

			//SYSMSG_BROADCASTV(402, it->second.boss->getId());
			it->second.boss->pushAttackerCountBS(false, it->second.spawnTime);
			it->second.boss->onReward();
			it->second.boss->reset();
			it->second.prepareTime += 86400;
			it->second.spawnTime += 86400;
			it->second.fleeTime += 86400;
			it->second.prepareTime = it->second.spawnTime - 300;
			Map * map = Map::FromSpot(it->second.location);
			if(map != NULL)
				map->Hide(it->second.boss->getId());
			_bossOff[it->first] = it->second;
			_bossOn.erase(it ++);
		}
		else
		{
			if(broadcast)
			{
				//SYSMSG_BROADCASTV(403, it->second.boss->getId(), it->second.location);
			}
			++ it;
		}
	}
}

Boss * BossManager::findBoss( UInt32 id )
{
	std::map<UInt32, BossStruct>::iterator it = _bossOff.find(id);
	if(it == _bossOff.end())
		return NULL;
	return it->second.boss;
}

bool BossManager::attack( Player * player, UInt32 id )
{
	std::map<UInt32, BossStruct>::iterator it = _bossOn.find(id);
	if(it == _bossOn.end())
		return false;
	UInt32 now = TimeUtil::Now();
	if(player->getBuffData(PLAYER_BUFF_BOSSWEAK, now) > 0)
	{
		player->sendMsgCode(2, 2033);
		return true;
	}
	if(it->second.boss->attackBy(player))
	{
		it->second.boss->pushAttackerCountBS(true, it->second.spawnTime);

		UInt32 now = TimeUtil::Now();
		it->second.boss->onReward();
		if(now - it->second.spawnTime < 15 * 60)
			it->second.boss->setLevel(it->second.boss->getLevel() + 1);
		//SYSMSG_BROADCASTV(401, it->second.boss->getId(), player->getCountry(), player->getName().c_str(), it->second.boss->getId());
		it->second.boss->reset();
		it->second.spawnTime += 86400;
		it->second.fleeTime += 86400;
		it->second.prepareTime = it->second.spawnTime - 300;
		Map * map = Map::FromSpot(it->second.location);
		if(map != NULL)
			map->Hide(it->second.boss->getId());
		_bossOff[it->first] = it->second;
		_bossOn.erase(it);
		getNextBoss();
		Stream st(REP::DAILY_DATA);
		st << static_cast<UInt8>(2);
		buildInfo(st);
		st << Stream::eos;
		NETWORK()->Broadcast(st);
	}
	return true;
}

void BossManager::buildInfo( Stream& st )
{
	UInt8 c = 0;
	if(_prevBoss != 0)
		++ c;
	if(_currBoss != 0)
		++ c;
	if(_nextBoss != 0)
		++ c;
	st << c;
	if(_prevBoss != 0)
		st << _prevBoss << _prevBossLevel << _prevBossPHP;
	if(_currBoss != 0)
		st << _currBoss << _currBossLevel << _currBossPHP;
	if(_nextBoss != 0)
		st << _nextBoss << _nextBossLevel << static_cast<UInt8>(0);
}

void BossManager::buildCurrBossInfo( Stream& st, UInt8 chpp )
{
	_currBossPHP = chpp;
	if(_currBoss == 0)
		st << static_cast<UInt8>(0);
	else
		st << static_cast<UInt8>(1) << _currBoss << _currBossLevel << _currBossPHP;
}

void BossManager::getNextBoss()
{
	UInt32 minTime = 0xFFFFFFFF;
	std::map<UInt32, BossStruct>::iterator cur = _bossOff.end();
	_currBoss = 0;
	for(std::map<UInt32, BossStruct>::iterator it = _bossOff.begin(); it != _bossOff.end(); ++ it)
	{
		if(it->second.spawnTime < minTime && it->second.appearLevel <= leaderboard.getMaxLevel())
		{
			minTime = it->second.spawnTime;
			cur = it;
		}
	}
	if(cur == _bossOff.end())
	{
		_currBoss = 0;
		_currBossLevel = 0;
		_currBossPHP = 0;
		_prevBoss = 0;
		_prevBossLevel = 0;
		_prevBossPHP = 0;
		_nextBoss = 0;
		_nextBossLevel = 0;
		return;
	}
	
	if(cur->second.spawnTime <= TimeUtil::Now())
	{
		++ cur;
		if(cur == _bossOff.end())
			cur = _bossOff.begin();
	}
	_nextBoss = cur->second.boss->getId();
	if(cur == _bossOff.begin())
	{
		cur = _bossOff.end();
		-- cur;
	}
	else
		-- cur;
	if(cur->second.appearLevel <= leaderboard.getMaxLevel())
	{
		_currBoss = cur->second.boss->getId();
		_currBossLevel = cur->second.boss->getLevel();
		_currBossPHP = cur->second.boss->getHPPercent();
	}
	else
	{
		_currBoss = 0;
		_currBossLevel = 0;
		_currBossPHP = 0;
	}
	if(cur == _bossOff.begin())
	{
		cur = _bossOff.end();
		-- cur;
	}
	else
		-- cur;
	if(cur->second.appearLevel <= leaderboard.getMaxLevel())
	{
		_prevBoss = cur->second.boss->getId();
		_prevBossLevel = cur->second.boss->getLevel();
		_prevBossPHP = cur->second.boss->getHPPercent();
		if(_prevBossPHP > 2)
			_prevBossPHP = 1;
	}
	else
	{
		_prevBoss = 0;
		_prevBossLevel = 0;
		_prevBossPHP = 0;
	}
}

void BossManager::sendInfo( Player * player )
{
	Stream st(REP::DAILY_DATA);
	st << static_cast<UInt8>(2);
	buildInfo(st);
	st << Stream::eos;
	player->send(st);
}

}

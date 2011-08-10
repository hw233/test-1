#include "Config.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "Package.h"
#include "TaskMgr.h"
#include "AttainMgr.h"
#include "Trade.h"
#include "Sale.h"
#include "Country.h"
#include "GObjectManager.h"
#include "MapCollection.h"
#include "Country.h"
#include "Dungeon.h"
#include "Fighter.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Map.h"
#include "GData/NpcGroup.h"
#include "GData/Title.h"
#include "Clan.h"
#include "Mail.h"
#include "Boss.h"
#include "Athletics.h"
#include "CountryBattle.h"
#include "Battle/BattleSimulator.h"
#include "Battle/BattleFighter.h"
#include "Script/GameActionLua.h"
#include "Server/SysMsg.h"
#include "Server/OidGenerator.h"
#include "Server/Cfg.h"
#include "Common/Itoa.h"
#include "ClanDynamicMsg.h"
#include "PracticePlace.h"
#include "Tripod.h"
#include <mysql.h>
#include "GData/Formation.h"

#include <cmath>

namespace GObject
{
    UInt32 Player::_recruit_cost = 20;
    UInt32 Player::_tavernBlueCount = 24;
    UInt32 Player::_tavernPurpleCount = 89;
	UInt32 Player::_tavernInterval = 2 * 3600, Player::_tavernRate = 100;
	UInt32 Player::_bookStoreInterval = 2 * 3600, Player::_bookStoreRate = 100;
	const UInt8 MaxICCount[] = {8, 16, 24, 24, 24, 24, 24, 24, 24, 24, 24};
	const UInt16 MAX_EXTEND_TIME	= 10;
	const UInt16 EACH_EXTEND_NUM	= 50;
	GlobalPlayers globalPlayers;
	GlobalPlayers newPlayers;
	GlobalNamedPlayers globalNamedPlayers;
	ChallengeCheck challengeCheck;

	inline UInt8 getMaxIcCount(UInt8 vipLevel)
	{
		UInt8 maxCount = MaxICCount[vipLevel];
		if(World::_wday == 6)
			maxCount += 8; 
		return maxCount;
	}

	float EventAutoBattle::calcExpEach(UInt32 now)
	{
		float theirbp = _npcGroup->getBattlePoints();
		float mybp = 0.0f;
		for(int i = 0; i < 5; ++ i)
		{
			Lineup& lup = m_Player->getLineup(i);
			if(!lup.available())
				continue;
			int ext = 0;
			if(m_Player->getBuffData(PLAYER_BUFF_ATTR3, now))
				ext = 3;
			else if(m_Player->getBuffData(PLAYER_BUFF_ATTR2, now))
				ext = 2;
			else if(m_Player->getBuffData(PLAYER_BUFF_ATTR1, now))
				ext = 1;
			if(lup.fighter->getBuffData(FIGHTER_BUFF_ATTR3, now))
				ext += 3;
			else if(lup.fighter->getBuffData(FIGHTER_BUFF_ATTR2, now))
				ext += 2;
			else if(lup.fighter->getBuffData(FIGHTER_BUFF_ATTR1, now))
				ext += 1;
			if(m_Player->getBuffData(PLAYER_BUFF_HOLY))
				ext += lup.fighter->getLevel() / 4;
			mybp += lup.fighter->getBattlePoint() * (1.0f + 0.015f * ext);
		}
		const float autobattle_tweak = 0.5;
		const float autobattle_A = 2.5;
		float exp = 3.0f * _npcGroup->getExp();
		float clanEffect = 1.0f;
		if(m_Player->getClan() != NULL)
			clanEffect = m_Player->getClan()->getAutoBattleSpeed();
		if(mybp >= theirbp)
			exp *= autobattle_A / (theirbp / mybp + (autobattle_A - 1.0f));
		else
			exp /= 1.0f + autobattle_tweak - autobattle_tweak * mybp / theirbp;
		return exp * clanEffect;
	}

	void EventAutoBattle::Process(UInt32)
	{
		UInt32 now = TimeUtil::Now();
		float exp = calcExpEach(now);
		if(m_Player->getBuffData(PLAYER_BUFF_TRAINP3, now))
			exp *= 1.7f;
		else if(m_Player->getBuffData(PLAYER_BUFF_TRAINP4, now))
			exp *= 1.5f;
		else if(m_Player->getBuffData(PLAYER_BUFF_TRAINP2, now))
			exp *= 1.5f;
		else if(m_Player->getBuffData(PLAYER_BUFF_TRAINP1, now))
			exp *= 1.3f;
		_npcGroup->monsterKilled(m_Player);
		if(m_Player->isOnline())
			m_Player->AddExp(static_cast<UInt32>(exp));
		else
			m_Player->pendExp(static_cast<UInt32>(exp));
		_npcGroup->getLoots(m_Player);
		notify();
		updateDB(false);
	}

	void EventAutoBattle::notify()
	{
		if(m_Player == NULL || !m_Player->isOnline())
			return;
		Stream st(0x6D);
		UInt16 cnt = static_cast<UInt16>(m_Timer.GetLeftTimes());

		UInt32 vipLevel = m_Player->getVipLevel();
		if(cnt > 0)
		{
			if(_npcGroup == NULL)
				return;
			UInt32 t = TimeUtil::Now();
			if(t > _finalEnd) t = 0; else t = _finalEnd - t;
			st << _npcGroup->getId() << static_cast<UInt8>(1) << cnt << t << (getMaxIcCount(vipLevel) - m_Player->getIcCount()) << Stream::eos;
		}
		else
		{
			st << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt32>(0) << (getMaxIcCount(vipLevel) - m_Player->getIcCount()) << Stream::eos;
			m_Player->delFlag(Player::Training);
		}
		m_Player->send(st);
	}

	bool EventAutoBattle::instantComplete()
	{
		UInt32 oldCnt = m_Timer.GetLeftTimes();
		UInt32 cnt = 60;
		if(cnt > oldCnt)
			cnt = oldCnt;
		UInt32 newCnt = oldCnt - cnt;
		struct ExpGainInstantCompleteStruct
		{
			float exp;
			UInt32 duration;
			UInt32 count;
			GData::NpcGroup * ng;
		};
		ExpGainInstantCompleteStruct ecs;
		ecs.duration = cnt * 60;
		m_Timer.SetLeftTimes(newCnt);
		ecs.exp = calcExpEach(TimeUtil::Now()) * cnt;
		ecs.count = cnt;
		ecs.ng = _npcGroup;
		GameMsgHdr hdr(0x274, m_Player->getThreadId(), m_Player, sizeof(ExpGainInstantCompleteStruct));
		GLOBAL().PushMsg(hdr, &ecs);
		_finalEnd -= ecs.duration;
		notify();
		updateDB(false);
		return newCnt == 0;
	}

	void EventAutoBattle::updateDB(bool isNew)
	{
		UInt32 count = m_Timer.GetLeftTimes();
		if(count > 0)
		{
			if(isNew)
				DB().PushUpdateData("REPLACE INTO `auto_battle`(`playerId`, `npcId`, `count`, `interval`) VALUES(%"I64_FMT"u, %u, %u, %u)", m_Player->getId(), _npcGroup->getId(), count, m_Timer.GetInterval());
			else
				DB().PushUpdateData("UPDATE `auto_battle` SET `count` = %u WHERE `playerId` = %"I64_FMT"u", count, m_Player->getId());
		}
		else
			DB().PushUpdateData("DELETE FROM `auto_battle` WHERE `playerId` = %"I64_FMT"u", m_Player->getId());
	}

	bool EventFighterTrain::Equal(UInt32 id, size_t fgtId) const
	{
		return 	id == GetID() && _fighter->getId() == fgtId;
	}

	bool EventFighterTrain::Accelerate(UInt32 times)
	{
		UInt32 count = m_Timer.GetLeftTimes();
		if(times > count)
		{
			times = count;
		}
		count -= times;
		m_Timer.SetLeftTimes(count);
		_end -= times * 60;
		return count == 0;
	}

	void EventFighterTrain::Process(UInt32 leftCount)
	{
		TrainFighterData * data =  m_Player->getTrainFighterData(_fighter->getId());
		UInt32 fgtId = _fighter->getId();
		if (data == NULL)
		{
			PopTimerEvent(m_Player, EVENT_FIGHTERAUTOTRAINING, fgtId);
			return;
		}
		const std::vector<UInt32>& levExp = GData::GDataManager::GetLevelTrainExp();
		UInt32 exp = static_cast<UInt32>(levExp[_fighter->getLevel()] * data->factor);
		_fighter->addExp(exp);
		data->accExp += exp;
		data->checktime = leftCount;
		DB().PushUpdateData("UPDATE `fighter_train` SET `checkTime` = %u, `accExp` = %u WHERE `fgtId` = %u AND `ownerId` = %"I64_FMT"u", data->checktime, data->accExp, fgtId, m_Player->getId());
		if(leftCount == 0)
		{
			m_Player->delTrainFighter(fgtId, true);		
			return;
		}
		if (_fighter->getLevel() >= m_Player->GetLev())
		{
			m_Player->delTrainFighter(fgtId, true);
			PopTimerEvent(m_Player, EVENT_FIGHTERAUTOTRAINING, fgtId);
			return;
		}
	}

	bool EventPlayerPractice::Equal(UInt32 id, size_t playerid) const
	{
		return 	id == GetID() && playerid == m_Player->getId();
	}

	bool EventPlayerPractice::Accelerate(UInt32 times)
    {
		UInt32 count = m_Timer.GetLeftTimes();
		if(times > count)
		{
			times = count;
		}
		count -= times;
		m_Timer.SetLeftTimes(count);
		_end -= times * 60;
		return count == 0;
    }

	void EventPlayerPractice::Process(UInt32 leftCount)
    {
        PracticeData* data = practicePlace.getPracticeData(m_Player->getId());
        if (!data) {
			PopTimerEvent(m_Player, EVENT_PLAYERPRACTICING, m_Player->getId());
			return;
        }

        //data->lock.lock();
        Fighter* fgt = 0;
        for (auto i = data->fighters.begin(), e = data->fighters.end(); i != e; ++i)
        {
            fgt = m_Player->findFighter(*i);
            if (fgt)
            {
                fgt->addPExp(fgt->getPracticeInc() * 10); 
            }
        }
        //data->lock.unlock();

		data->checktime -= 10;
        if ((int)data->checktime < 0)
            data->checktime = 0;
		if(leftCount == 0 || data->checktime == 0)
		{
            DB().PushUpdateData("UPDATE `practice_data` SET `checktime` = %u, `place` = %u, `slot` = %u, winnerid = %u, fighters = '' WHERE `id` = %"I64_FMT"u", data->checktime, PPLACE_MAX, 0, 0, m_Player->getId());
            practicePlace.stop(m_Player);
			PopTimerEvent(m_Player, EVENT_PLAYERPRACTICING, m_Player->getId());
			return;
		}
        else
        {
            DB().PushUpdateData("UPDATE `practice_data` SET `checktime` = %u WHERE `id` = %"I64_FMT"u",
                    data->checktime, m_Player->getId());
        }
        return;
    }

	bool EventPlayerTripod::Equal(UInt32 id, size_t playerid) const
	{
		return 	id == GetID() && playerid == m_Player->getId();
	}

	bool EventPlayerTripod::Accelerate(UInt32 times)
    {
		UInt32 count = m_Timer.GetLeftTimes();
		if(times > count)
		{
			times = count;
		}
		count -= times;
		m_Timer.SetLeftTimes(count);
		return count == 0;
    }

	void EventPlayerTripod::Process(UInt32 leftCount)
    {
        TripodData& data = tripod.getTripodData(m_Player->getId());
        data.soul += POINT_PERMIN;
        if (data.soul > MAX_TRIPOD_SOUL)
            data.soul = MAX_TRIPOD_SOUL;

        if (!leftCount || data.soul >= MAX_TRIPOD_SOUL - POINT_PERMIN/2) {
            PopTimerEvent(m_Player, EVENT_PLAYERPRTRIPOD, m_Player->getId());
            data.awdst = 1;
            DB().PushUpdateData("UPDATE `tripod` SET `awdst` = %u WHERE `id` = %"I64_FMT"u", data.awdst, m_Player->getId());
            return;
        }

        if (!(leftCount % 3))
            DB().PushUpdateData("UPDATE `tripod` SET `soul` = %u WHERE `id` = %"I64_FMT"u", data.soul, m_Player->getId());
    }

	void Lineup::updateId()
	{
		if(fighter != NULL) fid = fighter->getId(); else fid = 0;
	}

	Player::Player( UInt64 id ): GObjectBaseT<Player, UInt64>(id),
		_isOnline(false), _threadId(0xFF), _session(-1),
		_availInit(false), _vipLevel(0), _clan(NULL), _clanBattle(NULL), _flag(0), _gflag(0), _onlineDuration(0),
		_nextTavernUpdate(0), _nextBookStoreUpdate(0), _bossLevel(21), _ng(NULL), _lastNg(NULL),
		_lastDungeon(0), _exchangeTicketCount(0), _praplace(0), _justice_roar(0), m_tripodAwdId(0), m_tripodAwdNum(0)
	{
		memset(_buffData, 0, sizeof(UInt32) * PLAYER_BUFF_COUNT);
		m_Package = new Package(this);
		m_TaskMgr = new TaskMgr(this);
		m_Trade = new Trade(this);
		m_Sale = new Sale(this);
		m_MailBox = new MailBox(this);
		m_Athletics = new Athletics(this);
		m_AttainMgr = new AttainMgr(this);
	}


	Player::~Player()
	{
		UnInit();
	}

	bool Player::Load()
	{
		updateBattleFighters(false);

		SERVER().GetCountry(GetCountryThread()).PlayerEnter(this, false);

		recalcVipLevel();

		if(!hasStatus(TopupRewarded))
		{
			addStatus(TopupRewarded);
			if(_vipLevel > 8)				
				sendVIPMails(9, _vipLevel);
			sendBlockBossMail(24, _bossLevel);
		}

		if(hasStatus(AutoCB))
		{
			globalCountryBattle.addAutoCB(this);
		}

		setBlockBossByLevel();
		return true;
	}

	UInt32 Player::calcVipLevel()
	{
#define VIP_OPEN_MAX 10
		UInt32 totalRecharge = _playerData.totalRecharge;
		UInt32 vipl;
		if(totalRecharge < 199)
			vipl = 0;
		else if(totalRecharge < 999)
			vipl = 1;
		else if(totalRecharge < 1999)
			vipl = 2;
		else if(totalRecharge < 4999)
			vipl = 3;
		else if(totalRecharge < 9999)
			vipl = 4;
		else if (totalRecharge < 19999)
			vipl = 5;
		else if(totalRecharge < 49999)
			vipl = 6;
		else if(totalRecharge < 99999)
			vipl = 7;
		else if(totalRecharge < 199999)
			vipl = 8;
		else if(totalRecharge < 299999)
			vipl = 9;
		else
			vipl = 10;
		if(vipl > VIP_OPEN_MAX)
			return VIP_OPEN_MAX;
		return vipl;
	}

	bool Player::Init()
	{
		if(_availInit)
			return true;

		m_Package->Init();

		_availInit = true;

        return true;
	}

	void Player::UnInit()
	{
		m_Package->UnInit();
		SAFE_DELETE(m_Package);

		m_TaskMgr->UnInit();
		SAFE_DELETE(m_TaskMgr);

		SAFE_DELETE(m_Trade);
		SAFE_DELETE(m_Sale);
		SAFE_DELETE(m_Athletics);

		SAFE_DELETE(m_MailBox);
	}

	UInt8 Player::GetCountryThread()
	{
		return mapCollection.getCountryFromSpot(_playerData.location);
	}

	const std::string& Player::getClanName() const
	{
		static std::string empty;
		if(_clan == NULL)
			return empty;
		return _clan->getName();
	}

	void Player::Login()
	{
		UInt32 curtime = TimeUtil::Now();

		if(cfg.enableWallow && _playerData.wallow)
		{
			UInt32 dur = curtime - _playerData.lastOnline;
			if(dur >= 5 * 60 * 60)
				_onlineDuration = 0;
		}

		_playerData.lastOnline = curtime;
		DB().PushUpdateData("UPDATE `player` SET `lastOnline` = %u WHERE `id` = %"I64_FMT"u", curtime, getId());

		if(_isOnline)
			_isOnline = false;

		Map * map = GetMap();
		if(map != NULL)
			map->PlayerEnter( this );

		m_TaskMgr->Init();
		m_Sale->Init();

		lockSecondPWD();

		checkLastBattled();
		GameAction()->onLogin(this);
	}

	void Player::Reconnect()
	{
		Map * map = GetMap();
		if(map != NULL)
			map->PlayerEnter( this );

		m_TaskMgr->Init();
		m_Sale->Init();

		checkLastBattled();
		_isOnline = true;
		if(_clan != NULL)
			_clan->broadcastMemberInfo(this);
	}

	void Player::selfKick()
	{
		if (m_TaskMgr->IsInConvey())
		{
			m_TaskMgr->SetConvey(false);
		}

		UInt32 curtime = TimeUtil::Now();
		DBLOG().PushUpdateData("update login_states set logout_time=%u where server_id=%u and player_id=%"I64_FMT"u and login_time=%u", curtime, cfg.serverLogId, _id, _playerData.lastOnline);
		UInt32 deadline = _playerData.nextRewardTime + _playerData.lastOnline;
		if(deadline <= curtime)
			deadline = 0;
		else
			deadline -= curtime;

		_playerData.nextRewardTime = deadline;
		_playerData.lastOnline = curtime;
		writeOnlineRewardToDB();

		removeStatus(SGPunish);
	}

	void Player::Logout(bool nobroadcast)
	{
		if (m_TaskMgr->IsInConvey())
		{
			m_TaskMgr->SetConvey(false);
		}

		UInt32 curtime = TimeUtil::Now();

		if(cfg.enableWallow && _playerData.wallow)
		{
			_onlineDuration = _onlineDuration + curtime - _playerData.lastOnline;
		}
		
		DBLOG().PushUpdateData("update login_states set logout_time=%u where server_id=%u and player_id=%"I64_FMT"u and login_time=%u", curtime, cfg.serverLogId, _id, _playerData.lastOnline);
		UInt32 deadline = _playerData.nextRewardTime + _playerData.lastOnline;
		if(deadline <= curtime)
			deadline = 0;
		else
			deadline -= curtime;

		_playerData.nextRewardTime = deadline;
		DB().PushUpdateData("UPDATE `player` SET `lastOnline` = %u, `nextReward` = '%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", curtime, _playerData.rewardStep, _playerData.nextRewardItem, _playerData.nextRewardCount, _playerData.nextRewardTime, _id);
		_isOnline = false;

		if(!nobroadcast)
		{
			Map * map = GetMap();
			if(map != NULL)
				map->PlayerLeave( this, true );

			if(_clan != NULL)
				_clan->broadcastMemberInfo(this);
		}
		testBattlePunish();

		checkLastBattled();

		if (_clanBattle != NULL)
		{
			GameMsgHdr hdr(0x177, WORKER_THREAD_WORLD, this, 0);
			GLOBAL().PushMsg(hdr, NULL);
		}

		removeStatus(SGPunish);
	}

	void Player::checkLastBattled()
	{
		bool update = false;
		if(_playerData.lastResource != 0)
		{
			UInt32 coupon = static_cast<UInt32>((_playerData.lastResource >> 16) & 0xFFFF);
			UInt32 tael = static_cast<UInt32>(_playerData.lastResource & 0xFFFF);
			UInt32 coin = static_cast<UInt32>((_playerData.lastResource >> 32) & 0xFFFFF);
			UInt32 achievement = static_cast<UInt32>((_playerData.lastResource >> 52) & 0xFFF);
			if(coupon > 0)
				getCoupon(coupon);
			if(tael > 0)
				getTael(tael);
			if(coin > 0)
				getCoin(coin);

			if(achievement > 0)
				getAchievement(achievement);
			_playerData.lastResource = 0;
			update = true;
		}
		if(_lastNg != NULL)
		{
			_lastNg->monsterKilled(this);
			if(_playerData.lastExp > 0)
			{
				AddExp(_playerData.lastExp, _lastNg->getLevel());
				_playerData.lastExp = 0;
			}
			_lastNg = NULL;
		}
		else if(_playerData.lastExp != 0)
		{
			UInt32 exp = _playerData.lastExp;
			if((exp & 0x80000000) > 0)
			{
				exp &= 0x7FFFFFFF;
				moveTo(_playerData.location & 0xFF00, false);
			}
			AddExp(exp);
			_playerData.lastExp = 0;
			update = true;
		}
		if(!_lastLoot.empty())
		{
			if(_lastDungeon > 0)
			{
				std::string itemnames;
				std::vector<GData::LootResult>::iterator it;
				for(it = _lastLoot.begin(); it != _lastLoot.end(); ++ it)
				{
					if(IsEquipTypeId(it->id))
					{
						const GData::ItemBaseType * ibt = GData::itemBaseTypeManager[it->id];
						if(ibt != NULL && ibt->quality > 4)
						{
							SYSMSGV(iname, 10002, it->id);
							itemnames += iname;
						}
					}
				}
				if(!itemnames.empty())
				{
					UInt32 did = _lastDungeon & 0x7F;
					UInt32 level = _lastDungeon >> 8;
					const GData::DungeonData * dd = GData::dungeons[did];
					if(dd != NULL)
						SYSMSG_BROADCASTV(270, _playerData.country, _playerData.name.c_str(), dd->getName().c_str(), level, itemnames.c_str());
				}
			}
			std::vector<GData::LootResult>::iterator it;
			for(it = _lastLoot.begin(); it != _lastLoot.end(); ++ it)
			{
				m_Package->ItemNotify(it->id, it->count);
			}
			_lastLoot.clear();
		}
		if(_lastDungeon > 0)
		{
			if((_lastDungeon & 0x80) > 0)
			{
				const GData::DungeonData * dd = GData::dungeons[_lastDungeon & 0x7F];
				if(dd != NULL)
					SYSMSG_BROADCASTV(272, _playerData.country, _playerData.name.c_str(), dd->getName().c_str());
			}
			_lastDungeon = 0;
		}
		if(update)
		{
			DB().PushUpdateData("UPDATE `player` SET `lastExp` = 0, `lastResource` = 0 WHERE `id` = %"I64_FMT"u", _id);
		}
	}

	void Player::setBuffData(UInt8 id, UInt32 data, bool writedb)
	{
		UInt32 now = TimeUtil::Now();
		if(id == PLAYER_BUFF_ATTACKING && data >= now)
		{
			if(_playerData.status & SGPunish)
				_playerData.battlecdtm = data;
			else
				_playerData.battlecdtm = now + (data - now) / 6;
		}
		if(id >= PLAYER_BUFF_COUNT || _buffData[id] == data)
			return;
		_buffData[id] = data;
		if(writedb)
			sendModification(0x40 + id, data);
	}

	void Player::addBuffData(UInt8 id, UInt32 data)
	{
		if(id >= PLAYER_BUFF_COUNT || data == 0)
			return;
		UInt32 now = TimeUtil::Now();
		UInt32 olddata = getBuffData(id, now);
		if(olddata == 0 && id != PLAYER_BUFF_AUTOHEAL)
			_buffData[id] = now + data;
		else
			_buffData[id] = olddata + data;
		sendModification(0x40 + id, _buffData[id]);
	}

	UInt32 Player::getBuffData( UInt8 idx, UInt32 tm )
	{
		if(idx > PLAYER_BUFF_COUNT)
			return 0;
		if(idx != PLAYER_BUFF_AUTOHEAL && idx != PLAYER_BUFF_HOLY && _buffData[idx] > 0 && _buffData[idx] <= tm)
		{
			_buffData[idx] = 0;
			updateDB(0x40 + idx, 0);
			return 0;
		}
		return _buffData[idx];
	}

	UInt32 Player::getBuffLeft( UInt8 idx, UInt32 tm )
	{
		UInt32 buff = getBuffData( idx, tm );
		if(buff == 0)
			return 0;
		if(buff <= tm)
		{
			setBuffData( idx, 0 );
			return 0;
		}
		return buff - tm;
	}

	void Player::send( const void * buf, int size )
	{
		if(!_isOnline) return;
		TcpConnection conn = NETWORK()->GetConn(_session);
		if(conn.get() == NULL)
		{
			return;
		}
		conn->send(buf, size);
	}

	bool Player::isFighterFull() const
	{
		size_t size = _fighters.size();
		return (size > 9 || (GetLev() < 40 && size >= static_cast<size_t>(1 + GetLev() / 10)));
	}

	void Player::storeFighters()
	{
		DB().PushUpdateData("UPDATE `player` SET `lineup` = '%u,%u|%u,%u|%u,%u|%u,%u|%u,%u' WHERE id = %" I64_FMT "u",
			_playerData.lineup[0].fid, _playerData.lineup[0].pos, _playerData.lineup[1].fid, _playerData.lineup[1].pos,
			_playerData.lineup[2].fid, _playerData.lineup[2].pos, _playerData.lineup[3].fid, _playerData.lineup[3].pos,
			_playerData.lineup[4].fid, _playerData.lineup[4].pos, getId());
	}

	void Player::updateBattleFighters(bool updatedb)
	{
		int c = 0;
		for(int i = 0; i < 5; ++ i)
		{
			Lineup& lup = _playerData.lineup[i];
			for(int j = 0; j < i; ++ j)
			{
				if(_playerData.lineup[j].fid == lup.fid)
				{
					lup.fid = 0;
					lup.pos = 0;
				}
			}
			UInt32 id = lup.fid;
			if(id == 0)
			{
				lup.fighter = NULL;
				continue;
			}
			std::map<UInt32, Fighter *>::iterator it = _fighters.find(id);
			if(it == _fighters.end())
			{
				lup.fid = 0;
				lup.fighter = NULL;
				lup.pos = 0;
			}
			else
			{
				lup.fighter = it->second;
				lup.updateId();
				++ c;
				if(i > 0 && lup.fid < 10)
				{
					std::swap(_playerData.lineup[0], _playerData.lineup[i]);
				}
			}
		}
		if(c == 0 && !_fighters.empty())
		{
			if(_fighters.empty())
				return;
			Lineup& lup = _playerData.lineup[0];
			lup.fighter = _fighters.begin()->second;
			lup.pos = 12;
			lup.updateId();
		}
		if(updatedb)
		{
			storeFighters();
		}
	}


	Map* Player::GetMap()
	{
		return Map::FromSpot(_playerData.location);
	}

	SpotData * Player::GetMapSpot()
	{
		return Map::Spot(_playerData.location);
	}

	UInt8 Player::GetClass() const
	{
		return (!_fighters.empty()) ? _fighters.begin()->second->getClass() : 1;
	}

	UInt8 Player::GetClassAndSex() const
	{
		return (!_fighters.empty()) ? _fighters.begin()->second->getClassAndSex() : 0;
	}

	bool Player::IsMale() const
	{
		return (!_fighters.empty()) ? _fighters.begin()->second->isMale() : 0;
	}

	UInt8 Player::GetLev() const
	{
		return (!_fighters.empty()) ? _fighters.begin()->second->getLevel() : LEVEL_MAX;
	}

	UInt64 Player::GetExp() const
	{
		return (!_fighters.empty()) ? _fighters.begin()->second->getExp() : 0;
	}

    void Player::upInitCitta(Fighter* fgt)
    {
        static UInt16 cittas[] = {301, 401, 701};
        UInt16 citta = cittas[fgt->getClass()-1];
        if (fgt->addNewCitta(citta)) {
            if (fgt->upCitta(citta, 0, true)) {
            }
        }
    }

	void Player::addFighter( Fighter * fgt, bool writedb )
	{
		UInt32 id = fgt->getId();
		if(id < 10)
			_fighters.insert(_fighters.begin(), std::make_pair(fgt->getId(), fgt));
		else
			_fighters[fgt->getId()] = fgt;

        upInitCitta(fgt);

		if(writedb)
		{
			UInt32 p = static_cast<UInt32>(fgt->getPotential() * 100);
			UInt32 c = static_cast<UInt32>(fgt->getCapacity() * 100);
			DB().PushUpdateData("REPLACE INTO `fighter` (`id`, `playerId`, `potential`, `capacity`, `level`, `experience`)\
                    VALUES(%u, %"I64_FMT"u, %u.%02u, %u.%02u, %u, %u)",
                    id, getId(), p / 100, p % 100, c / 100, c % 100, fgt->getLevel(), fgt->getExp());
		}
	}

	void Player::notifyAddFighter( Fighter * fgt )
	{
		if(!_isOnline)
			return;
		Stream st(0x28);
		st << static_cast<UInt16>(fgt->getId());
        st << fgt->getPotential() << fgt->getCapacity() << fgt->getLevel() << fgt->getExp() << fgt->getDefaultSkillAndLevel() << fgt->getDefaultCittaAndLevel();
		st << Stream::eos;
		send(st);
		SYSMSG_SENDV(110, this, fgt->getColor(), fgt->getName().c_str());
		SYSMSG_SENDV(1010, this, fgt->getColor(), fgt->getName().c_str());
	}

	Fighter * Player::findFighter( UInt32 id )
	{
		std::map<UInt32, Fighter *>::iterator it = _fighters.find(id);
		if(it == _fighters.end())
			return NULL;
		return it->second;
	}

	Fighter * Player::getMainFighter()
	{
		if(_fighters.empty())
			return NULL;
		Fighter * fgt = _fighters.begin()->second;
		if(fgt->getId() > 9)
			return NULL;
		return fgt;
	}

	bool Player::hasFighter( UInt32 id )
	{
		std::map<UInt32, Fighter *>::iterator it = _fighters.find(id);
		return it != _fighters.end();
	}

	Fighter * Player::removeFighterFromLineup(UInt32 id)
	{
		Fighter * fighter = NULL;
		bool r = false;
		for(int j = 0; j < 5; ++ j)
		{
			if(_playerData.lineup[j].fid == id)
			{
				r = true;
				fighter = _playerData.lineup[j].fighter;
				_playerData.lineup[j].fid = 0;
				_playerData.lineup[j].fighter = NULL;
				_playerData.lineup[j].pos = 0;
			}
		}
		if(r)
		{
			setFormation(0);
			storeFighters();
			Stream st;
			makeFormationInfo(st);
			send(st);
		}
		return fighter;
	}
	
	Fighter * Player::removeFighter( UInt32 id )
	{
		if(_fighters.empty())
			return NULL;
		std::map<UInt32, Fighter *>::iterator it = _fighters.find(id);
		if(it != _fighters.end() && it->second->getId() > 6)
		{
			bool r = false;
			for(int j = 0; j < 5; ++ j)
			{
				if(_playerData.lineup[j].fid == id)
				{
					r = true;
					_playerData.lineup[j].fid = 0;
					_playerData.lineup[j].fighter = NULL;
					_playerData.lineup[j].pos = 0;
				}
			}
			if(r)
			{
				storeFighters();
				Stream st;
				makeFormationInfo(st);
				send(st);
			}
			if(delTrainFighter(id))
				PopTimerEvent(this, EVENT_FIGHTERAUTOTRAINING, id);
			Fighter * fgt = it->second;
			ItemEquip * equip;
			for(UInt8 z = 1; z < 9; ++ z)
				m_Package->EquipTo(0, fgt, z, equip, true);
			_fighters.erase(it);
			DB().PushUpdateData("DELETE FROM `fighter` WHERE `id` = %u AND `playerId` = %"I64_FMT"u", id, getId());
			if(r)
				sendMsgCode(0, 2034);
			SYSMSG_SENDV(111, this, fgt->getColor(), fgt->getName().c_str());
			SYSMSG_SENDV(1011, this, fgt->getColor(), fgt->getName().c_str());
			return fgt;
		}
		return NULL;
	}

	UInt16 Player::addFightCurrentHp(UInt32 id, UInt16 hp, bool battle)
	{
		UInt16 currHp = 0;
		if (battle)
		{
			for(int i = 0; i < 5; ++ i)
			{
				GObject::Lineup& pd = _playerData.lineup[i];
				if(pd.fighter != NULL && pd.fighter->getId() == id)
				{
					currHp = pd.fighter->getCurrentHP() + hp;
					pd.fighter->setCurrentHP(currHp);
					return currHp;
				}
			}
		}
		else
		{
			for (std::map<UInt32, Fighter*>::iterator it = _fighters.begin(); it != _fighters.end(); ++it)
			{
				Fighter * fgt = it->second;
				if (fgt->getId() == id)
				{
					currHp = fgt->getCurrentHP() + hp;
					fgt->setCurrentHP(currHp);
					return currHp;
				}
			}
		}
		return 0;
	}

	void Player::addFightCurrentHpAll(UInt16 hp)
	{
		UInt16 currHp = 0;
		for (std::map<UInt32, Fighter*>::iterator it = _fighters.begin(); it != _fighters.end(); ++it)
		{
			Fighter * fgt = it->second;
			currHp = fgt->getCurrentHP() + hp;
			fgt->setCurrentHP(currHp);
		}
	}

    bool Player::checkFormation(UInt16 f)
    {
        bool find = false;
        int cnt = _playerData.formations.size();
        for( int idx = 0; idx < cnt; ++ idx )
        {
            if(_playerData.formations[idx] == f)
            {
                find = true;
                break;
            }
        }

        if(!find)
            return false;

        return true;
    }

	bool Player::setFormation( UInt16 f )
	{
        if(!checkFormation(f))
            return false;

        int cnt = _playerData.formations.size();
		if(_playerData.formation == f)
			return true;
		_playerData.formation = f;
		DB().PushUpdateData("UPDATE `player` SET `formation` = %u WHERE id = %" I64_FMT "u", f, _id);

        return true;
	}

	void Player::makePlayerInfo( Stream& st )
	{
		UInt32 curtime = TimeUtil::Now();
		UInt8 c = 0;
		UInt8 buffid[PLAYER_BUFF_COUNT];
		UInt32 buffleft[PLAYER_BUFF_COUNT];
		for(UInt8 i = 0; i < PLAYER_BUFF_COUNT; ++ i)
		{
			if(i == 0)
			{
				if(_buffData[i] > 0)
				{
					buffid[c] = i + 0x40;
					buffleft[c] = _buffData[i];
					++ c;
				}
			}
			else
			{
				if(_buffData[i] > curtime)
				{
					buffid[c] = i + 0x40;
					buffleft[c] = _buffData[i] - curtime;
					++ c;
				}
			}
		}
		st.init(0x14);
		UInt8 status = static_cast<UInt8>(_playerData.status);
		if(cfg.limitLuckyDraw == 2 || (cfg.limitLuckyDraw == 1 && _vipLevel < 2))
			status |= 0x80;
		st << _playerData.country << _playerData.gold << _playerData.coupon << _playerData.tael << _playerData.coin << getClanName()
			<< status << _playerData.title << static_cast<UInt8>(0) << _playerData.totalRecharge << _playerData.achievement << _playerData.packSize << _playerData.newGuild <<  _playerData.mounts << c;
		for(UInt8 i = 0; i < c; ++ i)
		{
			st << buffid[i] << buffleft[i];
		}
		st << Stream::eos;
	}

	void Player::makeFormationInfo( Stream& st )
	{
		st.init(0x1E);
		st << _playerData.formation << static_cast<UInt8>(0);
		UInt8 c = 0;
		for(UInt8 i = 0; i < 5; ++ i)
		{
			Lineup& pdata = _playerData.lineup[i];
			if(pdata.available())
			{
				++ c;
				st << pdata.fid << pdata.pos;
			}
		}
		st.data<UInt8>(6) = c;
		st << Stream::eos;
	}

	void Player::makeFighterIdList( Stream& st )
	{
		size_t c = _fighters.size();
		st.init(0x18);
		st << static_cast<UInt8>(c);
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
			st << it->second->getId();
		st << Stream::eos;
	}

	void Player::makeFighterList( Stream& st )
	{
		size_t c = _fighters.size();
		st.init(0x23);
		st << static_cast<UInt8>(c);
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
			makeFighterInfo(st, it->second);
		st << Stream::eos;
	}

	void Player::makeFighterInfo( Stream& st, Fighter * fgt, bool withequip )
	{
		st << static_cast<UInt16>(fgt->getId()) << fgt->getPotential()
            << fgt->getCapacity() << fgt->getLevel() << fgt->getExp()
            << fgt->getPExp() << fgt->getPExpMax() << fgt->getSoul() << fgt->getMaxSoul();
		st << fgt->getPeerlessAndLevel() << fgt->getCurrentHP() << fgt->getUpCittasMax();
		if(withequip)
		{
			st << fgt->getWeaponId() << fgt->getArmorId(0) << fgt->getArmorId(1)
                << fgt->getArmorId(2) << fgt->getArmorId(3) << fgt->getArmorId(4)
				<< fgt->getAmuletId() << fgt->getRingId();
            fgt->getAllTrumps(st);
            fgt->getAllAcupointsBits(st);
            fgt->getAllSkillAndLevel(st);
            fgt->getAllPeerlessAndLevel(st);
            fgt->getAllCittaAndLevel(st);

			UInt8 c = 0;
			UInt8 buffid[PLAYER_BUFF_COUNT];
			UInt32 buffleft[PLAYER_BUFF_COUNT];
			UInt32 curtime = TimeUtil::Now();
			for(UInt8 i = 0; i < FIGHTER_BUFF_COUNT; ++ i)
			{
				UInt32 data = fgt->getBuffData(i);
				if(data > 0)
				{
					buffid[c] = i + 0x40;
					buffleft[c] = data - curtime;
					++ c;
				}
			}
			st << c;
			for(UInt8 i = 0; i < c; ++ i)
			{
				st << buffid[i] << buffleft[i];
			}
		}
	}

	bool Player::makeFighterInfo( Stream&st, UInt32 id )
	{
		std::map<UInt32, Fighter *>::iterator it = _fighters.find(id);
		if(it == _fighters.end())
			return false;
		makeFighterInfo(st, it->second);
		return true;
	}

	void Player::checkGreatFighterFriendliness(UInt32 id)
	{
		std::map<UInt32, GreatFighterTaskVal>::iterator it = _greatFighterTaskVal.find(id);
		if(it == _greatFighterTaskVal.end())
			setGreatFighterFriendliness(id, 10);
	}

	bool Player::setGreatFighterTaskVal(UInt32 id, UInt16 friendliness, UInt16 submitFavorCount, UInt32 submitFavorDay)
	{
		Fighter * fgt = globalFighters[id];
		if (fgt == NULL)
			return false;
		GreatFighterTaskVal& v = _greatFighterTaskVal[id];
		v.friendliness = friendliness;
		if (v.friendliness >= fgt->reqFriendliness)
		{
			v.friendliness = fgt->reqFriendliness;
			_greatFighterFull.insert(id);
		}
		v.submitFavorCount = submitFavorCount;
		v.submitFavorDay = submitFavorDay;
		return true;
	}

	void Player::setGreatFighterFriendliness(UInt32 id, UInt16 friendliness, bool writedb)
	{
		if(id > GREAT_FIGHTER_MAX)
			return;
		Fighter * fgt = globalFighters[id];
		if (fgt == NULL)
			return;
		GreatFighterTaskVal& v = _greatFighterTaskVal[id];
		v.friendliness = friendliness;
		v.submitFavorDay = TimeUtil::SharpDay(0);
		if (v.friendliness >= fgt->reqFriendliness)
		{
			v.friendliness = fgt->reqFriendliness;
			_greatFighterFull.insert(id);
		}
		if(writedb)
		{
			DB().PushUpdateData("REPLACE INTO `friendliness` (`playerId`, `fighterId`, `friendliness`, `favorSubmitCount`, `favorSubmitDay`) VALUES (%"I64_FMT"u, %u, %u, %u, %u)", getId(), id, v.friendliness, v.submitFavorCount, v.submitFavorDay);
			sendGreatFighterTaskVal(id, v.friendliness, v.submitFavorCount);
			//SYSMSG_SENDV(148, this, friendliness);
			SYSMSG_SENDV(1048, this, id, friendliness);
		}
	}

	UInt16 Player::getGreatFighterFriendliness( UInt32 id )
	{
		std::map<UInt32, GreatFighterTaskVal>::const_iterator cit = _greatFighterTaskVal.find(id);
		if (cit == _greatFighterTaskVal.end())
			return 0;
		return cit->second.friendliness;
	}

	void Player::sendGreatFighterTaskVal(UInt32 id)
	{
		std::map<UInt32, GreatFighterTaskVal>::const_iterator cit = _greatFighterTaskVal.find(id);
		if (cit == _greatFighterTaskVal.end())
			return;
		sendGreatFighterTaskVal(id, cit->second.friendliness, cit->second.submitFavorCount);
	}

	void Player::sendGreatFighterTaskVal( UInt32 id, UInt16 friendliness, UInt16 favorSubmitCount )
	{
		Stream st(0x25);
		st << id << friendliness << favorSubmitCount << Stream::eos;
		send(st);
	}

	void Player::greatFighterFavorSubmitCheck(GreatFighterTaskVal * task, UInt32 fightId, UInt32 thisDay)
	{
		if (task->submitFavorDay != thisDay)
		{
			task->submitFavorDay = thisDay;
			if(task->submitFavorCount != 0)
			{
				task->submitFavorCount = 0;
				DB().PushUpdateData("UPDATE `friendliness` SET `favorSubmitCount`= %u,	`favorSubmitDay` = %u WHERE `playerId` = %"I64_FMT"u AND `fighterId` = %u", 0, thisDay, getId(), fightId);
			}
		}
	}

	bool Player::submitGreatFighterFavor(UInt32 id)
	{
		std::map<UInt32, GreatFighterTaskVal>::iterator it = _greatFighterTaskVal.find(id);
		if (it == _greatFighterTaskVal.end())
			return false;

		UInt32 thisDay = TimeUtil::SharpDay(0);
		GreatFighterTaskVal * v = &it->second;
		if (v->submitFavorDay != thisDay)
		{
			v->submitFavorCount = 0;
			v->submitFavorDay = thisDay;
		}

		v->submitFavorCount ++;
		v->friendliness += 30;
		Fighter * fgt = globalFighters[id];
		if (fgt != NULL && v->friendliness >= fgt->reqFriendliness)
		{
			v->friendliness = fgt->reqFriendliness;
			_greatFighterFull.insert(id);
		}
		//SYSMSG_SENDV(148, this, 30);
		SYSMSG_SENDV(1048, this, id, 30);
		DB().PushUpdateData("REPLACE INTO `friendliness` (`playerId`, `fighterId`, `friendliness`, `favorSubmitCount`, `favorSubmitDay`) VALUES (%"I64_FMT"u, %u, %u, %u, %u)", getId(), id, v->friendliness, v->submitFavorCount, v->submitFavorDay);
		DBLOG().PushUpdateData("insert into `fighter_friendness`(`server_id`, `player_id`, `fighter_id`, `friendness`, `time`) values(%u, %"I64_FMT"u, %u, %u, %u)", cfg.serverLogId, getId(), id, v->friendliness, TimeUtil::Now());		
		GameAction()->RunGreatTaskAction(this, id);

		sendGreatFighterTaskVal(id, v->friendliness, v->submitFavorCount);
		return true;
	}

	UInt16 Player::getGreatFighterFavorSubmitCount(UInt32 id)
	{
		std::map<UInt32, GreatFighterTaskVal>::iterator it = _greatFighterTaskVal.find(id);
		if (it == _greatFighterTaskVal.end())
			return 0;

		greatFighterFavorSubmitCheck(&it->second, it->first);
		return it->second.submitFavorCount;
	}

	UInt32 Player::getGreatFighterFavor(UInt32 fightId)
	{
		GObject::Fighter& fighter = GObject::getGreatFighter(fightId);
		return fighter.getId() == 0 ? 0 : fighter.favor;
	}

	void Player::makeGreatFighterTaskValList(Stream& st, UInt16 start, UInt16 count)
	{
		UInt16 sz = static_cast<UInt16>(_greatFighterTaskVal.size());
		UInt16 end = start + count;
		if (end > sz)
			end = sz;
		if (start < end)
			count = end - start;
		else
			count = 0;
		st.init(0x24);
		st << start << count << sz;
		std::map<UInt32, GreatFighterTaskVal>::iterator pos = _greatFighterTaskVal.begin();
		std::advance(pos, start);
		UInt32 thisDay = TimeUtil::SharpDay(0);
		for (; pos != _greatFighterTaskVal.end() && start < end; ++pos, ++start)
		{
			greatFighterFavorSubmitCheck(&pos->second, pos->first, thisDay);
			st << pos->first << pos->second.friendliness << pos->second.submitFavorCount;
		}
		st << Stream::eos;
	}

	bool Player::setNewGuildTaskStep(UInt32 step)
	{
		if (step <= 0 || step > NEWGUILDSTEP_MAX)
			return false;
		UInt64 stepVal = 1ull << (step - 1);
 		if ((stepVal & _playerData.newGuild) == 0)
		{
			_playerData.newGuild |= stepVal;
			DB().PushUpdateData("UPDATE `player` SET `newGuild` = %"I64_FMT"u WHERE `id` = %"I64_FMT"u", _playerData.newGuild, _id);
			switch (step)
			{
			case 3:
				AddExp(300);
				getCoupon(50);
				getCoin(1000);
				break;
			case 5:
				AddExp(500);
				getCoupon(50);
				getCoin(1000);
				break;
			case 7:
				AddExp(700);
				getCoupon(50);
				getCoin(1000);
				break;
			case 8:
				AddExp(1000);
				getCoupon(50);
				getCoin(1000);
				break;
			default:
				break;
			}
		} 
		Stream st(0x15);
		st << static_cast<UInt8>(0x10) << step << Stream::eos;
		send(st);
		return true;
	}

    bool Player::setMounts(UInt8 mounts)
    {
        if (mounts != getMounts())
        {
            sendModification(9, mounts, false);
            return true;
        }
        return false;
    }

	bool Player::challenge( Player * other, UInt32 * rid, int * turns, bool applyhp, UInt32 sysRegen )
	{
		checkLastBattled();
		other->checkLastBattled();
		Battle::BattleSimulator bsim(applyhp ? _playerData.location : 0x8FFF, this, other);
		PutFighters( bsim, 0 );
		other->PutFighters( bsim, 1 );
		bsim.start();
		bool res = bsim.getWinner() == 1;

		Stream st(0x61);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		send(st);
		st.data<UInt8>(4) = static_cast<UInt8>(res ? 0 : 1);
		other->send(st);

		if(turns != NULL)
			*turns = bsim.getTurns();

		if(rid != NULL)
			*rid = bsim.getId();

		if(applyhp)
		{
			if(bsim.applyFighterHP(0, this, !hasFlag(CountryBattle | ClanBattling), 0))
				checkHPLoss();
			if(bsim.applyFighterHP(1, other, !other->hasFlag(CountryBattle | ClanBattling), 0))
				other->checkHPLoss();
		}
		else if(sysRegen > 0)
		{
			if(res)
			{
				bsim.applyFighterHP(0, this, false, sysRegen);
				other->regenAll();
			}
			else
			{
				bsim.applyFighterHP(1, other, false, sysRegen);
				regenAll();
			}
		}
		if(res)
			other->checkDeath();
		else
			checkDeath();

		return res;
	}

	bool Player::attackNpc( UInt32 npcId, UInt32 turns, bool regen )
	{
		UInt32 now = TimeUtil::Now();
        // TODO:
		UInt32 buffLeft = getBuffData(PLAYER_BUFF_ATTACKING, now);
        buffLeft = 0;
		if(buffLeft > now)
		{
			sendMsgCode(0, 2035, buffLeft - now);
			return false;
		}
		checkLastBattled();
		if(getThreadId() == WORKER_THREAD_NEUTRAL && turns == 0xFFFFFFFF)
		{
			if(bossManager.attack(this, npcId))
				return false;
		}
		GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
		if(it == GData::npcGroups.end())
			return false;

		GData::NpcGroup * ng = it->second;

		if(GameAction()->RunExploreTask(this, npcId))
			turns = 0;
		Battle::BattleSimulator bsim(_playerData.location, this, ng->getName(), ng->getLevel(), false, turns);

		PutFighters( bsim, 0 );
		if(npcId == 6145)
		{
			Fighter * fgt;
			if(_playerData.country == 0)
				fgt = globalFighters[4104];
			else
				fgt = globalFighters[4147];
			int pos = 22;
			while(bsim(0, pos) != 0 && pos >= 0) -- pos;
			if(pos >= 0)
				bsim.newFighter(0, pos, fgt);
		}
		ng->putFighters( bsim );
		bsim.start();
		Stream& packet = bsim.getPacket();
		if(packet.size() <= 8)
			return false;

		Stream st(0x61);

		bool res = bsim.getWinner() == 1;
		if(res)
		{
			st << static_cast<UInt16>(0x0101);
			_lastNg = ng;
			if(getBuffData(PLAYER_BUFF_TRAINP3, now))
				pendExp(ng->getExp() * 17 / 10);
			else if(getBuffData(PLAYER_BUFF_TRAINP4, now))
				pendExp(ng->getExp() * 3 / 2);
			else if(getBuffData(PLAYER_BUFF_TRAINP2, now))
				pendExp(ng->getExp() * 3 / 2);
			else if(getBuffData(PLAYER_BUFF_TRAINP1, now))
				pendExp(ng->getExp() * 13 / 10);
			else
				pendExp(ng->getExp());
			ng->getLoots(this, _lastLoot);
		}
		else
			st << static_cast<UInt16>(0x0100);
		st << _playerData.lastExp << static_cast<UInt8>(0);
		UInt8 sz = _lastLoot.size();
		st << sz;
		for(UInt8 i = 0; i < sz; ++ i)
		{
			st << _lastLoot[i].id << _lastLoot[i].count;
		}
		st.append(&packet[8], packet.size() - 8);
		st << Stream::eos;
		send(st);
		if(!regen)
		{
			if(bsim.applyFighterHP(0, this))
			{
				checkHPLoss();
			}
		}

		if(!res)
			checkDeath();

		setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns() * 2);

		return res;
	}

	bool Player::attackCopyNpc( UInt32 npcId )
	{
		UInt32 now = TimeUtil::Now();
        // TODO:
		UInt32 buffLeft = getBuffData(PLAYER_BUFF_ATTACKING, now);
        buffLeft = 0;
		if(buffLeft > now)
		{
			sendMsgCode(0, 2035, buffLeft - now);
			return false;
		}
		checkLastBattled();
		GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
		if(it == GData::npcGroups.end())
			return false;

		GData::NpcGroup * ng = it->second;

		Battle::BattleSimulator bsim(_playerData.location, this, ng->getName(), ng->getLevel(), false);
		PutFighters( bsim, 0 );
		ng->putFighters( bsim );
		bsim.start();
		Stream& packet = bsim.getPacket();
		if(packet.size() <= 8)
			return false;

		Stream st(0x61);
		bool res = bsim.getWinner() == 1;
		if(res)
		{
			st << static_cast<UInt16>(0x0101);
			_lastNg = ng;
			if(getBuffData(PLAYER_BUFF_TRAINP3, now))
				pendExp(ng->getExp() * 17 / 10);
			else if(getBuffData(PLAYER_BUFF_TRAINP4, now))
				pendExp(ng->getExp() * 3 / 2);
			else if(getBuffData(PLAYER_BUFF_TRAINP2, now))
				pendExp(ng->getExp() * 3 / 2);
			else if(getBuffData(PLAYER_BUFF_TRAINP1, now))
				pendExp(ng->getExp() * 13 / 10);
			else
				pendExp(ng->getExp());
			ng->getLoots(this, _lastLoot);
		}
		else
			st << static_cast<UInt16>(0x0100);
		st << _playerData.lastExp << static_cast<UInt8>(0);
		UInt8 sz = _lastLoot.size();
		st << sz;
		for(UInt8 i = 0; i < sz; ++ i)
		{
			st << _lastLoot[i].id << _lastLoot[i].count;
		}
		st.append(&packet[8], packet.size() - 8);
		st << Stream::eos;
		send(st);

		if(!res)
			checkDeath();

		setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns() * 2);
		return res;
	}

	bool Player::autoBattle( UInt32 npcId )
	{
		GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
		if(it == GData::npcGroups.end())
			return false;
		GData::NpcGroup * ng = it->second;
		if(GetLev() < ng->getLevel())
		{
			sendMsgCode(0, 2005);
			return false;
		}
		const UInt32 eachBattle = 60;
		UInt8 level = GetLev();
		UInt32 count = 60 * ((level / 10) + 1);
		if(level >= LEVEL_MAX)
			count = 60 * (LEVEL_MAX / 10);

		UInt32 viplvl = getVipLevel();
		if(viplvl >= 3)
			count += 60 * 2;
		else if(viplvl == 2)
            count += 60;

		UInt32 timeDur = count * eachBattle;

		UInt32 final = TimeUtil::Now() + timeDur;
		EventAutoBattle* event = new(std::nothrow) EventAutoBattle(this, eachBattle, count, ng, final);
		if (event == NULL) return false;
		cancelAutoBattle();
		PushTimerEvent(event);
		addFlag(Training);
		event->notify();
		event->updateDB(true);
		return true;
	}

	void Player::pushAutoBattle(UInt32 npcId, UInt16 count, UInt16 interval)
	{
		if(npcId == 0 || count == 0 || interval == 0)
			return;
		GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
		if(it == GData::npcGroups.end())
			return;
		GData::NpcGroup * ng = it->second;
		UInt32 final = TimeUtil::Now() + interval * count;
		EventAutoBattle* event = new(std::nothrow) EventAutoBattle(this, interval, count, ng, final);
		if (event == NULL) return;
		PushTimerEvent(event);
		addFlag(Training);
	}

	void Player::pushAutoDungeon( UInt32 id, UInt32 exp, UInt8 won )
	{
		Dungeon * dg = dungeonManager[id];
		if(dg == NULL)
			return;
		dg->pushChallenge(this, exp, won > 0);
	}

	void Player::cancelAutoBattle()
	{
		GameMsgHdr hdr2(0x179, WORKER_THREAD_WORLD, this, 0);
		GLOBAL().PushMsg(hdr2, 0);
	}

	void Player::cancelAutoBattleNotify()
	{
		Stream st(0x6D);
		st << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt32>(0) << (getMaxIcCount(_vipLevel) - getIcCount()) << Stream::eos;
		send(st);
		DB().PushUpdateData("DELETE FROM `auto_battle` WHERE `playerId` = %"I64_FMT"u", _id);
		delFlag(Training);
	}

	void Player::cancelAutoDungeon()
	{
		if(!hasFlag(AutoDungeon)) return;
		GameMsgHdr hdr(0x180, WORKER_THREAD_WORLD, this, 0);
		GLOBAL().PushMsg(hdr, 0);
	}

	void Player::instantAutoBattle()
	{
		if(_playerData.icCount >= getMaxIcCount(_vipLevel) || !hasFlag(Training) || getGoldOrCoupon() < 10)
			return;

        ConsumeInfo ci(InstantAutoBattle,0,0);
		useGoldOrCoupon(10,&ci);
        incIcCount();
		GameMsgHdr hdr(0x178, WORKER_THREAD_WORLD, this, 0);
		GLOBAL().PushMsg(hdr, NULL);
	}

	void Player::sendEvents()
	{
		struct EventQuery
		{
			bool autoBattle;
			bool autoDungeon;
		};
		EventQuery eq = { hasFlag(Training), hasFlag(AutoDungeon) };
		GameMsgHdr hdr(0x19C, WORKER_THREAD_WORLD, this, sizeof(EventQuery));
		GLOBAL().PushMsg(hdr, &eq);
	}

	void Player::sendAutoBattleEvent(EventBase * event)
	{
		EventAutoBattle * evab = dynamic_cast<EventAutoBattle *>(event);
		if(evab == NULL)
			return;
		evab->notify();
	}

	void Player::PutFighters( Battle::BattleSimulator& bsim, int side, bool fullhp )
	{
		bsim.setFormation(side, getFormation());
		for(int i = 0; i < 5; ++ i)
		{
			Lineup& lup = getLineup(i);
			if(lup.fid != 0 && lup.fighter == NULL)
			{
				std::map<UInt32, Fighter *>::iterator it = _fighters.find(lup.fid);
				if(it == _fighters.end())
				{
					lup.fid = 0;
					lup.fighter = NULL;
					lup.pos = 0;
				}
				else
				{
					lup.fighter = it->second;
					lup.updateId();
				}
			}
			if(lup.fighter != NULL)
			{
				Battle::BattleFighter * bf = bsim.newFighter(side, lup.pos, lup.fighter);
				bf->setHP(fullhp ? 0 : lup.fighter->getCurrentHP());
			}
			else if(i == 0 && !_fighters.empty())
			{
				lup.fighter = _fighters.begin()->second;
				lup.updateId();
				lup.pos = 12;
				Battle::BattleFighter * bf = bsim.newFighter(side, lup.pos, lup.fighter);
				bf->setHP(fullhp ? 0 : lup.fighter->getCurrentHP());
				storeFighters();
			}
		}
		bsim.setPortrait(side, _fighters.empty() ? 0 : _fighters.begin()->second->getId());
	}

	Fighter * Player::takeFighter( UInt32 id, bool writedb )
	{
		if(id > GREAT_FIGHTER_MAX || (writedb && id < 10))
			return NULL;
		if(hasFighter(id))
			return NULL;
		Fighter * fgt = globalFighters[id];
		if(fgt == NULL)
			return NULL;
		if(id > 6 && id <= 300)
			setGreatFighterFriendliness(id, fgt->reqFriendliness, writedb);
		Fighter * fgt2 = fgt->clone(this);
		addFighter(fgt2, writedb);
		if (_clan != NULL)
		{
			UInt8 color = fgt2->getColor();
			if (color == 4 || color == 5)
			{
				Stream st;
				_clan->getClanDynamicMsg()->addCDMsg(color == 4 ? 10 : 22, getName(), fgt2->getName(), &st);
				_clan->broadcast(st);
			}
		}
		if(writedb)
		{
			bool rewrite = false;
			for(int i = 0; i < 6; ++ i)
			{
				if(_playerData.tavernId[i] == id)
				{
					_playerData.tavernId[i] = 0;
					rewrite = true;
					break;
				}
			}
			if(rewrite)
				writeTavernIds();
			notifyFriendAct(6, id);
			Map * map = Map::FromSpot(_playerData.location);
			if(map != NULL)
				map->SendHide(id, this);
			if(fgt->getColor() > 1 && fgt->getId() > 6 && fgt->getId() <= GREAT_FIGHTER_MAX)
				SYSMSG_BROADCASTV(10, getCountry(), getName().c_str(), fgt->getColor(), fgt->getName().c_str());
			notifyAddFighter(fgt2);
			autoLineup(fgt2);
		}
		return fgt2;
	}


	bool Player::ExtendPackageSize()
	{
		static const UInt16 MAX_PACK_SIZE = PlayerData::INIT_PACK_SIZE+MAX_EXTEND_TIME*EACH_EXTEND_NUM;
		if (_playerData.packSize >= MAX_PACK_SIZE || (_playerData.packSize+EACH_EXTEND_NUM) > MAX_PACK_SIZE)
			return false;
		UInt32 lvl = 1 + (_playerData.packSize - PlayerData::INIT_PACK_SIZE) / EACH_EXTEND_NUM;
		UInt32 gold = lvl * (lvl + 1) * 5;
		if (getGold() < gold)
			return false;
		ConsumeInfo ci(ExtendPackage,0,0);
		useGold(gold,&ci);

		_playerData.packSize += EACH_EXTEND_NUM;
		updateDB(32, _playerData.packSize);
		Stream st(0x34);
		st << getPacksize() << Stream::eos;
		send(st);
		return true;
	}


	UInt16 Player::GetFreePackageSize()
	{
		return m_Package->GetRestPackageSize();
	}

	bool Player::addFriend( Player * pl )
	{
		if(pl == NULL || this == pl)
			return false;
		Mutex::ScopedLock lk(_mutex);
		Mutex::ScopedLock lk2(pl->getMutex());
		if(!testCanAddFriend(pl))
			return false;
		if(_id < pl->getId())
		{
			addFriendInternal(pl, true, true);
			pl->addFriendInternal(this, true, false);
		}
		else
		{
			pl->addFriendInternal(this, true, true);
			addFriendInternal(pl, true, false);
		}
		return true;
	}

	void Player::addFriendFromDB( Player * pl )
	{
		if(pl == NULL || this == pl)
			return;
		if(_id < pl->getId())
		{
			addFriendInternal(pl, false, false);
			pl->addFriendInternal(this, false, false);
		}
		else
		{
			pl->addFriendInternal(this, false, false);
			addFriendInternal(pl, false, false);
		}
	}

	void Player::addFriendInternal( Player * pl, bool notify, bool writedb )
	{
		if(notify)
		{
			notifyFriendAct(1, pl);
			Stream st(0xA9);
			st << static_cast<UInt8>(0x01) << pl->getId() << pl->getName() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << Stream::eos;
			send(st);
			SYSMSG_SEND(132, this);
			SYSMSG_SENDV(1032, this, pl->getCountry(), pl->getName().c_str());
			if(writedb)
				DB().PushUpdateData("REPLACE INTO `friend` (`id`, `type`, `friendId`) VALUES (%"I64_FMT"u, 0, %"I64_FMT"u)", getId(), pl->getId());
		}
		_friends[0].insert(pl);
	}

	void Player::delFriend( Player * pl )
	{
		Mutex::ScopedLock lk(_mutex);
		Mutex::ScopedLock lk2(pl->getMutex());
		delFriendInternal(pl);
		pl->delFriendInternal(this);
	}

	void Player::delFriendInternal( Player * pl, bool writedb )
	{
		std::set<Player *>::iterator it = _friends[0].find(pl);
		if(it == _friends[0].end())
			return;
		_friends[0].erase(it);
		Stream st(0xA9);
		st << static_cast<UInt8>(0x02) << pl->getName() << Stream::eos;
		send(st);
		SYSMSG_SEND(134, this);
		SYSMSG_SENDV(1034, this, pl->getCountry(), pl->getName().c_str());
		if(writedb)
			DB().PushUpdateData("DELETE FROM `friend` WHERE `id` = %"I64_FMT"u AND `type` = 0 AND `friendId` = %"I64_FMT"u", getId(), pl->getId());
	}

	Player * Player::_findFriend( UInt8 type, std::string& name )
	{
		Player * pl = globalNamedPlayers[fixName(name)];
		if(pl == NULL)
			return NULL;
		std::set<Player *>::iterator it = _friends[type].find(pl);
		if(it == _friends[type].end())
			return NULL;
		return *it;
	}

	bool Player::_hasFriend( UInt8 type, Player * pl ) const
	{
		std::set<Player *>::const_iterator it = _friends[type].find(pl);
		if(it == _friends[type].end())
			return false;
		return true;
	}

	bool Player::addBlock( Player * pl )
	{
		if(pl == this)
			return false;
		Mutex::ScopedLock lk(_mutex);
		if(_friends[1].size() >= 20)
		{
			sendMsgCode(2, 2085);
			return false;
		}
		if(pl == NULL || _hasBlock(pl))
		{
			SYSMSG_SEND(136, this);
			SYSMSG_SENDV(1036, this, pl->getCountry(), pl->getName().c_str());
			return false;
		}

		notifyFriendAct(4, pl);
		Stream st(0xA9);
		st << static_cast<UInt8>(0x03) << pl->getId() << pl->getName() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << Stream::eos;
		send(st);
		DB().PushUpdateData("REPLACE INTO `friend` (`id`, `type`, `friendId`) VALUES (%"I64_FMT"u, 1, %"I64_FMT"u)", getId(), pl->getId());
		SYSMSG_SEND(135, this);
		SYSMSG_SENDV(1035, this, pl->getCountry(), pl->getName().c_str());

		_friends[1].insert(pl);
		delFriend(pl);
		return true;
	}

	void Player::addBlockFromDB( Player * pl )
	{
		if(pl == NULL || this == pl)
			return;
		_friends[1].insert(pl);
	}

	bool Player::delBlock( Player * pl )
	{
		Mutex::ScopedLock lk(_mutex);
		std::set<Player *>::iterator it = _friends[1].find(pl);
		if(it == _friends[1].end())
			return false;
		_friends[1].erase(it);
		Stream st(0xA9);
		st << static_cast<UInt8>(0x04) << pl->getName() << Stream::eos;
		send(st);
		DB().PushUpdateData("DELETE FROM `friend` WHERE `id` = %"I64_FMT"u AND `type` = 1 AND `friendId` = %"I64_FMT"u", getId(), pl->getId());
		return true;
	}

	bool Player::addFoe( Player * pl )
	{
		if(pl == this)
			return false;
		Mutex::ScopedLock lk(_mutex);
		if(pl == NULL || _hasFriend(pl) || _hasFoe(pl))
		{
			return false;
		}

		notifyFriendAct(3, pl);
		Stream st(0xA9);
		st << static_cast<UInt8>(0x05) << pl->getId() << pl->getName() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << Stream::eos;
		send(st);
		DB().PushUpdateData("REPLACE INTO `friend` (`id`, `type`, `friendId`) VALUES (%"I64_FMT"u, 2, %"I64_FMT"u)", getId(), pl->getId());
		SYSMSG_SEND(157, this);
		SYSMSG_SENDV(1057, this, pl->getCountry(), pl->getName().c_str());

		if(_friends[2].size() >= 20)
		{
			std::set<Player *>::iterator it = _friends[2].begin();
			Stream st(0xA9);
			st << static_cast<UInt8>(0x06) << (*it)->getName() << Stream::eos;
			send(st);
			_friends[2].erase(it);
		}
		_friends[2].insert(pl);
		return true;
	}

	void Player::addFoeFromDB( Player * pl )
	{
		if(pl == NULL || this == pl)
			return;
		_friends[2].insert(pl);
	}

	bool Player::delFoe( Player * pl )
	{
		Mutex::ScopedLock lk(_mutex);
		std::set<Player *>::iterator it = _friends[2].find(pl);
		if(it == _friends[2].end())
			return false;
		_friends[2].erase(it);
		Stream st(0xA9);
		st << static_cast<UInt8>(0x06) << pl->getName() << Stream::eos;
		send(st);
		DB().PushUpdateData("DELETE FROM `friend` WHERE `id` = %"I64_FMT"u AND `type` = 2 AND `friendId` = %"I64_FMT"u", getId(), pl->getId());
		return true;
	}

	void Player::sendFriendList( UInt8 type, UInt8 start, UInt8 cnt )
	{
		UInt8 sz = static_cast<UInt8>(_friends[type].size());
		UInt8 end = start + cnt;
		if(end > sz)
			end = sz;
		if(end < start)
			cnt = 0;
		else
			cnt = end - start;
		Stream st(0xA8);
		st << static_cast<UInt8>(type) << start << cnt << sz;
		std::set<Player *>::iterator it = _friends[type].begin();
		std::advance(it, start);
		for(UInt8 i = 0; i < cnt; ++ i)
		{
			Player * pl = *it;
			st << pl->getId() << pl->getName() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry()
				<< pl->GetLev() << pl->GetClass() << pl->getClanName();
			++it;
		}
		st << Stream::eos;
		send(st);
	}

	void Player::sendModification( UInt8 t, UInt32 v, bool updateToDB )
	{
		if(_isOnline)
		{
			Stream st(0x15);
			if(t > 0x40)
			{
				UInt32 tm = TimeUtil::Now();
				if(v > tm)
					st << t << static_cast<UInt32>(v - tm) << Stream::eos;
				else
					st << t << static_cast<UInt32>(0) << Stream::eos;
			}
			else
				st << t << v << Stream::eos;
			send(st);
		}
		if(updateToDB)
			updateDB(t, v);
	}

	void Player::updateDB( UInt8 t, UInt32 v )
	{
		if(t >= 0x40 && t < 0x40 + PLAYER_BUFF_COUNT)
		{
			if(t >= 0x40 + PLAYER_BUFF_DISPLAY_MAX)
				return;
			if(v > 0)
				DB().PushUpdateData("REPLACE INTO `player_buff`(`id`, `buffId`, `data`) VALUES(%"I64_FMT"u, %u, %u)", _id, t - 0x40, v);
			else
				DB().PushUpdateData("DELETE FROM `player_buff` WHERE `id` = %"I64_FMT"u AND `buffId` = %u", _id, t - 0x40);
			return;
		}
		const char * field = NULL;
		switch(t)
		{
		case 1: field = "gold"; break;
		case 2: field = "coupon"; break;
		case 3: field = "tael"; break;
		case 4: field = "coin"; break;
		case 5: field = "status"; v &= ~0x80; break;
		case 6: field = "title"; break;
		case 7: field = "totalRecharge"; break;
		case 8: field = "archievement"; break;
		case 9: field = "mounts"; break;
		case 0x20: field = "packSize"; break;
		}
		if(field != NULL)
			DB().PushUpdateData("UPDATE `player` SET `%s` = %u WHERE `id` = %"I64_FMT"u", field, v, _id);
	}

	UInt32 Player::getGold( UInt32 c )
	{
		if(c == 0)
			return _playerData.gold;
		_playerData.gold += c;
		SYSMSG_SENDV(149, this, c);
		SYSMSG_SENDV(1049, this, c);
		sendModification(1, _playerData.gold);
		return _playerData.gold;
	}

	UInt32 Player::useGold( UInt32 c ,ConsumeInfo * ci)
	{
		if(c == 0 || _playerData.gold == 0)
			return _playerData.gold;
		if(_playerData.gold < c)
			_playerData.gold = 0;
		else
		{
		    _playerData.gold -= c;
		    if(ci!=NULL)
		    {
				DBLOG().PushUpdateData("insert into consume_gold (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
            }
        }
		SYSMSG_SENDV(150, this, c);
		SYSMSG_SENDV(1050, this, c);
		sendModification(1, _playerData.gold);
		return _playerData.gold;
	}

	bool Player::holdGold(UInt32 c, UInt8 action)
	{
		switch(action)
		{
		case 0:
			{
				if (c > _playerData.gold)
					return false;
				_playerData.gold -= c;
			}
			break;
		case 1:
			{
				sendModification(1, _playerData.gold);
				SYSMSG_SENDV(150, this, c);
				SYSMSG_SENDV(1050, this, c);
			}
			break;
		case 2:
			{
				_playerData.gold += c;
				updateDB(1, _playerData.gold);
			}
			break;
		}
		WARN_LOG("Hold money : action[%d]owerId[%"I64_FMT"u]gold[%u]time[%u]",  action, getId(), c, TimeUtil::Now());
		return true;
	}

	UInt32 Player::getGoldOrCoupon()
	{
		return _playerData.gold + _playerData.coupon;
	}

	UInt32 Player::useGoldOrCoupon(UInt32 num,ConsumeInfo * ci)
	{
		if(_playerData.coupon > 0)
		{
			if(_playerData.coupon < num)
			{
				num -= _playerData.coupon;
				useCoupon(_playerData.coupon, ci);
			}
			else
			{
				useCoupon(num, ci);
				return 0;
			}
		}
		useGold(num, ci);
		return num;
	}

	UInt32 Player::getCoupon( UInt32 c )
	{
		if(c == 0)
			return _playerData.coupon;
		_playerData.coupon += c;
		SYSMSG_SENDV(155, this, c);
		SYSMSG_SENDV(1055, this, c);
		sendModification(2, _playerData.coupon);
		return _playerData.coupon;
	}

	UInt32 Player::useCoupon( UInt32 c ,ConsumeInfo * ci)
	{
		if(c == 0 || _playerData.coupon == 0)
			return _playerData.coupon;
		if(_playerData.coupon < c)
			_playerData.coupon = 0;
		else
		{
		    _playerData.coupon -= c;
            if(ci!=NULL)
		    {
				DBLOG().PushUpdateData("insert into consume_coupon (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
            }
        }
		SYSMSG_SENDV(156, this, c);
		SYSMSG_SENDV(1056, this, c);
		sendModification(2, _playerData.coupon);
		return _playerData.coupon;
	}

	UInt32 Player::getTael( UInt32 c )
	{
		if(c == 0)
			return _playerData.tael;

		if(cfg.enableWallow && _playerData.wallow)
		{
			UInt32 onlineDuration = (_onlineDuration + TimeUtil::Now() - _playerData.lastOnline);
			if(onlineDuration >= 5 * 60 * 60)
			{
				c = 0;
				SYSMSG_SENDV(184, this);
				SYSMSG_SENDV(1084, this);
			}
			else if(onlineDuration >= 3 * 60 * 60)
			{
				c /= 2;
				SYSMSG_SENDV(182, this, c);
				SYSMSG_SENDV(1082, this, c);
			}
			else
			{
				SYSMSG_SENDV(151, this, c);
				SYSMSG_SENDV(1051, this, c);
			}
		}
		else
		{
			SYSMSG_SENDV(151, this, c);
			SYSMSG_SENDV(1051, this, c);
		}
		if(c > 0)
		{
			_playerData.tael += c;
			sendModification(3, _playerData.tael);
		}
		return _playerData.tael;
	}

	UInt32 Player::useTael( UInt32 c,ConsumeInfo * ci )
	{
		if(c == 0 || _playerData.tael == 0)
			return _playerData.tael;
		if(_playerData.tael < c)
			_playerData.tael = 0;
		else
		{
			if(ci!=NULL)
			{
				DBLOG().PushUpdateData("insert into consume_tael (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)", cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
			}
			_playerData.tael -= c;
		}
		SYSMSG_SENDV(152, this, c);
		SYSMSG_SENDV(1052, this, c);
		sendModification(3, _playerData.tael);
		return _playerData.tael;
	}
	void Player::useTael2(UInt32 c, Player *attacker, ConsumeInfo * ci)//nature challengge use
	{
		if(c == 0 || _playerData.tael == 0)
			return ;
		if(_playerData.tael < c)
			_playerData.tael = 0;
		else
		{
			if(ci!=NULL)
			{
				DBLOG().PushUpdateData("insert into consume_tael (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
			}
			_playerData.tael -= c;
		}
		SYSMSG_SENDV(152, this, c);
		SYSMSG_SENDV(1060, this, attacker->getCountry(), attacker->getName().c_str(), c);
		sendModification(3, _playerData.tael);
	}

	UInt32 Player::getCoin( UInt32 c )
	{
        return 0; // XXX: no useful
		if(c == 0)
			return _playerData.coin;
		if(_playerData.coin >= 99999999)
		{
			SYSMSG_SENDV(159, this);
			return _playerData.coin;
		}

	
		if(cfg.enableWallow && _playerData.wallow)
		{
			UInt32 onlineDuration = (_onlineDuration + TimeUtil::Now() - _playerData.lastOnline);
			if(onlineDuration >= 5 * 60 * 60)
			{
				c = 0;
				SYSMSG_SENDV(184, this);
				SYSMSG_SENDV(1084, this);
			}
			else if(onlineDuration >= 3 * 60 * 60)
			{
				c /= 2;
				SYSMSG_SENDV(183, this, c);
				SYSMSG_SENDV(1083, this, c);
			}
			else
			{
				SYSMSG_SENDV(153, this, c);
				SYSMSG_SENDV(1053, this, c);
			}
		}
		else
		{
			SYSMSG_SENDV(153, this, c);
			SYSMSG_SENDV(1053, this, c);
		}

		if(_playerData.coin >= 99999999)
		{
			SYSMSG_SENDV(159, this);
		}

		if(c > 0)
		{
			if(_playerData.coin + c > 99999999)
				c = 99999999 - _playerData.coin;
			_playerData.coin += c;
			sendModification(4, _playerData.coin);
		}
		return _playerData.coin;
	}

	UInt32 Player::useCoin( UInt32 c, ConsumeInfo * ci, bool notify )
	{
        return 0; // XXX: no useful
		if(c == 0 || _playerData.coin == 0)
			return _playerData.coin;
		if(_playerData.coin < c)
			_playerData.coin = 0;
		else
		{
		    if(ci!=NULL)
		    {
				DBLOG().PushUpdateData("insert into consume_coin (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
            }
		    _playerData.coin -= c;
		}
		if(notify)
		{
			SYSMSG_SENDV(154, this, c);
			SYSMSG_SENDV(1054, this, c);
		}
		sendModification(4, _playerData.coin);
		return _playerData.coin;
	}

	bool Player::holdCoin(UInt32 c, UInt8 action)
	{
		switch(action)
		{
		case 0:
			{
				if (c > _playerData.coin)
					return false;
				_playerData.coin -= c;
			}
			break;
		case 1:
			{
				sendModification(4, _playerData.coin);
				SYSMSG_SENDV(154, this, c);
				SYSMSG_SENDV(1054, this, c);
			}
			break;
		case 2:
			{
				_playerData.coin += c;
				updateDB(4, _playerData.coin);
			}
			break;
		}
		WARN_LOG("Hold money : action[%d]owerId[%"I64_FMT"u]coin[%u]time[%u]",  action, getId(), c, TimeUtil::Now());
		return true;
	}

	TrainFighterData* Player::getTrainFighterData(UInt32 id)
	{
		std::map<UInt32, TrainFighterData *>::iterator found = _trainFighters.find(id);
		return found == _trainFighters.end() ? NULL : found->second;
	}

	void Player::addTrainFighterFromDB(UInt32 id,  UInt8 priceType, UInt32 price, UInt32 tm, UInt32 ct, UInt32 exp)
	{
		if(ct == 0)
			return;
		Fighter * fgt = findFighter(id);
		if(fgt == NULL)
			return;
		TrainFighterData *& data = _trainFighters[id];
		if (data != NULL) delete data;
		data = new(std::nothrow) TrainFighterData;
		if(data == NULL)
			return;
		data->traintime = tm;
		data->checktime = ct;
		data->factor = (priceType == 0 ? 4.0f : 2.5f);
		data->price = price;
		data->priceType = priceType;
		data->accExp = exp;
		data->trainend = TimeUtil::Now() + 60 * data->checktime;
		EventFighterTrain* event = new(std::nothrow) EventFighterTrain(this, 60, data->checktime, fgt, data->trainend);
		if (event == NULL)
		{
			_trainFighters.erase(id);
			delete data;
			return;
		}
		PushTimerEvent(event);
	}

	bool Player::delTrainFighter(UInt32 id, bool notify)
	{
		std::map<UInt32, TrainFighterData *>::iterator found = _trainFighters.find(id);
		if (found == _trainFighters.end()) return false;
		Fighter * fgt = findFighter(id);
		if (fgt == NULL) return false;
		TrainFighterData * data = found->second;
		UInt32 count = data->checktime;
		if (count > 0)
		{
			UInt32 money = data->checktime * data->price / data->traintime;
			if (data->priceType == 0)
			{
				if (money != 0)
				{
					SYSMSGV(title, 406);
					SYSMSGV(content, 409, fgt->getName().c_str(), data->price - money, data->accExp, money);
					GetMailBox()->newMail(NULL, 0x12, title, content);	
					getGold(money);
				}
				else
				{
					SYSMSGV(title, 406);
					SYSMSGV(content, 407, fgt->getName().c_str(), data->price - money, data->accExp);
					GetMailBox()->newMail(NULL, 0x12, title, content);	
				}
			}
			else
			{
				if (money != 0)
				{
					SYSMSGV(title, 406);
					SYSMSGV(content, 410, fgt->getName().c_str(), data->price - money, data->accExp, money);
					GetMailBox()->newMail(NULL, 0x12, title, content);
					getTael(money);
				}
				else
				{
					SYSMSGV(title, 406);
					SYSMSGV(content, 408, fgt->getName().c_str(), data->price - money, data->accExp);
					GetMailBox()->newMail(NULL, 0x12, title, content);
				}
			}
		}
		delete data;
		_trainFighters.erase(id);
		DB().PushUpdateData("DELETE FROM `fighter_train` WHERE `fgtId` = %u AND `ownerId` = %"I64_FMT"u", id, _id);
		Stream st(0x2E);
		st << id << static_cast<UInt8>(2) << static_cast<UInt32>(0) << Stream::eos;
		send(st);
		if (notify)
			sendMsgCode(0, 2088, id);

		return true;
	}

	bool Player::hasTrainFighter(UInt32 id)
	{
		return _trainFighters.find(id) != _trainFighters.end();
	}

	bool Player::addTrainFighter(UInt32 id, UInt8 priceType, UInt32 time)
	{
		Fighter * fgt = findFighter(id);
		if (fgt == NULL)
			return false;
		if (hasTrainFighter(id))
			return false;
		if (fgt->getLevel() >= GetLev())
			return false;
		if (time > 24)
			return false;
		UInt32 price = 0;
		ConsumeInfo ci(TrainFighter,0,0);
		if (priceType == 0)
		{
			const std::vector<UInt32>& golds = GData::GDataManager::GetGoldTrainList();
			price = time * golds[fgt->getLevel()]; 
			if (getGold() < price)
			{
				sendMsgCode(0, 2008);
				return false;
			}
			useGold(price, &ci);
		}
		else
		{
			const std::vector<UInt32>& taels = GData::GDataManager::GetTaelTrainList();
			price = time * taels[fgt->getLevel()];
			if (getTael() < price)
			{
				sendMsgCode(0, 2007);
				return false;
			}
			useTael(price, &ci);
		}
		TrainFighterData *& data = _trainFighters[id];
		if (data != NULL) delete data;
		data = new TrainFighterData();
		data->checktime = time * 60;
		data->traintime = data->checktime;
		data->factor = (priceType == 0 ? 4.0f : 2.5f);
		data->price = price;
		data->priceType = priceType;
		data->trainend = TimeUtil::Now() + 60 * data->checktime;
		DB().PushUpdateData("REPLACE INTO `fighter_train`(`fgtId`, `ownerId`, `priceType`, `price`, `trainTime`, `checkTime`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u)", id, getId(), priceType, price, data->traintime, data->checktime);
		removeFighterFromLineup(id);
		EventFighterTrain* event = new(std::nothrow) EventFighterTrain(this, 60, data->checktime, fgt, data->trainend);
		if (event == NULL) return false;
		PushTimerEvent(event);
		Stream st(0x2E);
		UInt32 remain = event->GetEnd() - TimeUtil::Now();
		st << id << data->priceType << remain << Stream::eos;
		send(st);

		return true;
	}

	bool Player::accTrainFighter(UInt32 id, UInt32 hrs)
	{
		std::map<UInt32, TrainFighterData *>::iterator found = _trainFighters.find(id);
		if (found == _trainFighters.end())
			return false;
		Fighter * fighter = findFighter(id);
		if (fighter == NULL)
			return false;
		TrainFighterData * data = found->second;
		if (data->checktime > 0)
		{
			UInt32 count = 60 * hrs;
			if(count > data->checktime)
			{
				count = data->checktime;
			}
			UInt32 goldUse = 10 * ((count + 59) / 60);
			if (getGold() < goldUse)
				return false;
			ConsumeInfo ci(AccTrainFighter, 0, 0);
			useGold(goldUse, &ci);
			const std::vector<UInt32>& levExp = GData::GDataManager::GetLevelTrainExp();
			UInt32 exp = static_cast<UInt32>(levExp[fighter->getLevel()] * data->factor * count);
			fighter->addExp(exp);
			data->accExp += exp;
			data->checktime -= count;
			data->trainend -= count * 60;
			if (data->checktime == 0 || fighter->getExp() >= GetExp())
			{
				if(delTrainFighter(id, true))
					PopTimerEvent(this, EVENT_FIGHTERAUTOTRAINING, id);
			}
			else
			{
				DB().PushUpdateData("UPDATE `fighter_train` SET `checkTime` = %u, `accExp` = %u WHERE `fgtId` = %u AND `ownerId` = %"I64_FMT"u", data->checktime, data->accExp, id, _id);
				Stream st(0x2E);
				UInt32 now = TimeUtil::Now();
				st << id << data->priceType;
				if(data->trainend > now)
					st << static_cast<UInt32>(data->trainend - now);
				else
					st << static_cast<UInt32>(0);
				st << Stream::eos;
				send(st);
				struct TrainAccData
				{
					UInt32 fgtId;
					UInt32 accCount;
				};
				TrainAccData tad = { id, count };
				GameMsgHdr hdr(0x17C, WORKER_THREAD_WORLD, this, sizeof(TrainAccData));
				GLOBAL().PushMsg(hdr, &tad);
			}
			return true;
		}
		
		return false;
	}

	bool Player::cancelTrainFighter(UInt32 id)
	{
		if(delTrainFighter(id))
			PopTimerEvent(this, EVENT_FIGHTERAUTOTRAINING, id);
		return true;
	}

	void Player::makeTrainFighterInfo(Stream& st)
	{
		st.init(0x2D);
		UInt8 cnt = 0;
		st << cnt;
		if(!_trainFighters.empty())
		{
			UInt32 now = TimeUtil::Now();
			for (std::map<UInt32, TrainFighterData *>::iterator it = _trainFighters.begin(); it != _trainFighters.end(); ++ it)
			{
				if (it->second->trainend > now)
				{
					st << it->first << it->second->priceType << static_cast<UInt32>(it->second->trainend - now);
					++ cnt;
				}
			}
			st.data<UInt8>(4) = cnt;
		}
		st << Stream::eos;
	}

	UInt32 Player::addStatus( UInt32 s )
	{
		if(s == 0 || (_playerData.status & s) == s)
			return _playerData.status;
		_playerData.status |= s;
		UInt32 status = _playerData.status;
		if(cfg.limitLuckyDraw == 2 || (cfg.limitLuckyDraw == 1 && _vipLevel < 2))
			status |= 0x80;
		sendModification(5, status);
		return _playerData.status;
	}

	UInt32 Player::setStatusBit( UInt32 bitStart, UInt32 bitCount, UInt32 s )
	{
		_playerData.status &= ~(((1 << bitCount) - 1) << bitStart);
		_playerData.status |= (s << bitStart);
		UInt32 status = _playerData.status;
		if(cfg.limitLuckyDraw == 2 || (cfg.limitLuckyDraw == 1 && _vipLevel < 2))
			status |= 0x80;
		sendModification(5, status);
		return _playerData.status;
	}

	UInt32 Player::removeStatus( UInt32 s )
	{
		if(s == 0 || (_playerData.status & s) == 0)
			return _playerData.status;
		_playerData.status &= ~s;
		UInt32 status = _playerData.status;
		if(cfg.limitLuckyDraw == 2 || (cfg.limitLuckyDraw == 1 && _vipLevel < 2))
			status |= 0x80;
		sendModification(5, status);
		return _playerData.status;
	}

	void Player::setTitle( UInt8 t )
	{
		if(t == _playerData.title)
			return;
		_playerData.title = t;
		sendModification(6, _playerData.title);
		rebuildBattleName();
	}

	UInt32 Player::getAchievement( UInt32 a )
	{
		if(a == 0)
			return _playerData.achievement;
		_playerData.achievement += a;
		SYSMSG_SENDV(105, this, a);
		SYSMSG_SENDV(1005, this, a);
		sendModification(8, _playerData.achievement);
		return _playerData.achievement;
	}

	UInt32 Player::useAchievement( UInt32 a ,ConsumeInfo * ci)
	{
		if(a == 0 || _playerData.achievement == 0)
			return _playerData.achievement;
		if(_playerData.achievement < a)
			_playerData.achievement = 0;
		else
		{
		    _playerData.achievement -= a;
		    if(ci!=NULL)
		    {
				DBLOG().PushUpdateData("insert into consume_achievement (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
            }
		}
		SYSMSG_SENDV(106, this, a);
		SYSMSG_SENDV(1006, this, a);
		sendModification(8, _playerData.achievement);
		return _playerData.achievement;
	}
	void Player::useAchievement2( UInt32 a, Player *attacker, ConsumeInfo * ci)
	{
		if(a == 0 || _playerData.achievement == 0)
			return ;
		if(_playerData.achievement < a)
			_playerData.achievement = 0;
		else
		{
			_playerData.achievement -= a;
			if(ci!=NULL)
			{
				DBLOG().PushUpdateData("insert into consume_achievement (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
			}
		}
		SYSMSG_SENDV(106, this, a);
		SYSMSG_SENDV(1061, this, attacker->getCountry(), attacker->getName().c_str(), a);
		sendModification(8, _playerData.achievement);
		return ;
	}

    void Player::AddPExp(UInt32 pexp)
    {
		for(int i = 0; i < 5; ++ i)
		{
			GObject::Fighter * fgt = getLineup(i).fighter;
			if(fgt != NULL)
				fgt->addPExp(pexp);
		}
    }

	void Player::AddExp(UInt64 exp, UInt8 mlvl)
	{
		if(exp == 0)
			return;
		if(mlvl == 0)
			mlvl = 255;
		else
			mlvl += 10;
		if(cfg.enableWallow && _playerData.wallow)
		{
			UInt32 onlineDuration = (_onlineDuration + TimeUtil::Now() - _playerData.lastOnline);
			if(onlineDuration >= 5 * 60 * 60)
			{
				exp = 0;
				SYSMSG_SENDV(184, this);
				SYSMSG_SENDV(1084, this);
				return;
			}
			else if(onlineDuration >= 3 * 60 * 60)
			{
				exp /= 2;
				SYSMSG_SENDV(181, this);
				SYSMSG_SENDV(1081, this);
			}
		}
		for(int i = 0; i < 5; ++ i)
		{
			GObject::Fighter * fgt = getLineup(i).fighter;
			if(fgt != NULL && fgt->getLevel() < mlvl)
				fgt->addExp(exp);
		}
	}

	void Player::setLevelAndExp( UInt8 l, UInt64 e )
	{
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
		{
			it->second->setLevelAndExp(l, e);
		}
	}

	void Player::moveTo( UInt16 spot, bool inCity )
	{
		if (spot == _playerData.location && inCity == (_playerData.inCity > 0))
			return;

		cancelAutoBattle();
		cancelAutoDungeon();
		GObject::Country& cny = CURRENT_COUNTRY();

		UInt8 new_cny = GObject::mapCollection.getCountryFromSpot(spot);
		if(new_cny != cny.GetThreadID())
		{
			CountryEnterStruct ces(true, inCity ? 1 : 0, spot);
			cny.PlayerLeave(this);
			_threadId = new_cny;
			GameMsgHdr hdr(0x1F0, new_cny, this, sizeof(CountryEnterStruct));
			GLOBAL().PushMsg( hdr, &ces );
			return;
		}

		GObject::Map * map = GObject::Map::FromSpot(spot);

		if(map == NULL)
			return;

		UInt16 oldLoc = _playerData.location;
		bool sameMap = ((oldLoc >> 8) == (spot >> 8));
		bool oldInCity = _playerData.inCity > 0;
		if(oldInCity || inCity)
		{
			GObject::Map * oldMap = GObject::Map::FromSpot(oldLoc);
			if(oldMap != NULL)
			{
				oldMap->PlayerLeave(this, false, !sameMap);
			}
		}

		_playerData.inCity = inCity ? 1 : 0;
		_playerData.location = spot;
		DB().PushUpdateData("UPDATE `player` SET `inCity` = %u, `location` = %u WHERE id = %" I64_FMT "u", _playerData.inCity, _playerData.location, getId());

		if(inCity)
		{
			bool notify = !sameMap || !oldInCity;

			if(notify)
				map->SendCityNPCs(this);

			map->PlayerEnter(this, !sameMap);
		}
		map->SendAtCity(this, inCity, !sameMap);
	}

	bool Player::regenHP( UInt32 hp )
	{
		std::map<UInt32, Fighter*>::iterator it;
		bool r = false;
		for(it = _fighters.begin(); it != _fighters.end(); ++ it)
		{
			Fighter * fgt = it->second;
			if(fgt == NULL)
				continue;
			r |= fgt->getCurrentHP() != fgt->regenHP(hp);
		}
		return r;
	}

	void Player::writeTavernIds()
	{
		DB().PushUpdateData("UPDATE `player` SET `tavernId` = '%u|%u|%u|%u|%u|%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.tavernId[0], _playerData.tavernId[1], _playerData.tavernId[2], _playerData.tavernId[3], _playerData.tavernId[4], _playerData.tavernId[5], _playerData.tavernBlueCount, _playerData.tavernPurpleCount, _nextTavernUpdate, _id);
	}

	void Player::writeShiMen()
	{
		DB().PushUpdateData("UPDATE `player` SET `shimen` = '%u,%u|%u,%u|%u,%u|%u,%u|%u,%u|%u,%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.shimen[0], _playerData.smcolor[0], _playerData.shimen[1], _playerData.smcolor[1], _playerData.shimen[2], _playerData.smcolor[2], _playerData.shimen[3], _playerData.smcolor[3], _playerData.shimen[4], _playerData.smcolor[4], _playerData.shimen[5], _playerData.smcolor[5], _playerData.smFreeCount, _playerData.smFinishCount, _playerData.smAcceptCount, _id);
	}

	void Player::writeYaMen()
	{
		DB().PushUpdateData("UPDATE `player` SET `yamen` = '%u,%u|%u,%u|%u,%u|%u,%u|%u,%u|%u,%u|%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.yamen[0], _playerData.ymcolor[0], _playerData.yamen[1], _playerData.ymcolor[1], _playerData.yamen[2], _playerData.ymcolor[2], _playerData.yamen[3], _playerData.ymcolor[3], _playerData.yamen[4], _playerData.ymcolor[4], _playerData.yamen[5], _playerData.ymcolor[5], _playerData.ymFreeCount, _playerData.ymFinishCount, _id);
	}

    void Player::delColorTask(UInt32 taskid)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (_playerData.shimen[i] == taskid)
            {
                //_playerData.shimen[i] = 0;
                _playerData.smcolor[i] |= 0xF0;
                sendColorTask(0, 0);
                return;
            }
        }
        for (int i = 0; i < 6; ++i)
        {
            if (_playerData.yamen[i] == taskid)
            {
                //_playerData.yamen[i] = 0;
                _playerData.ymcolor[i] |= 0xF0;
                sendColorTask(1, 0);
                return;
            }
        }

        return;
    }

    bool Player::addAwardByTaskColor(UInt32 taskid)
    {
        for (int i = 0; i < 6; ++i) {
            if (_playerData.shimen[i] == taskid) {
                if (_playerData.smFinishCount >= 5)
                    return false;
                if (_playerData.smFinishCount + _playerData.smAcceptCount >= 5)
                    return false;

                //_playerData.shimen[i] = 0;
                //_playerData.smcolor[i] = 0;

                UInt32 award = GData::GDataManager::GetTaskAwardFactor(1, _playerData.smcolor[i]&0x0F);
                AddExp(award); // TODO:
                ++_playerData.smFinishCount;
                sendColorTask(0, 0);
                return true;
            }
        }
        for (int i = 0; i < 6; ++i) {
            if (_playerData.yamen[i] == taskid) {
                if (_playerData.ymFinishCount >= 5)
                    return false;
                if (_playerData.ymFinishCount + _playerData.ymAcceptCount >= 5)
                    return false;

                //_playerData.yamen[i] = 0;
                //_playerData.ymcolor[i] = 0;

                UInt32 award = GData::GDataManager::GetTaskAwardFactor(2, _playerData.ymcolor[i]&0x0F);
                getTael(award); // TODO:
                ++_playerData.ymFinishCount;
                sendColorTask(1, 0);
                return true;
            }
        }
        return false;
    }

    bool Player::ColorTaskOutOf(UInt8 type)
    {
        if (type == 0)
        {
            return _playerData.smFinishCount >= 5;
        }
        else if (type == 1)
        {
            return _playerData.ymFinishCount >= 5;
        }
        return false;
    }

    bool Player::ColorTaskOutOfAccept(UInt8 type)
    {
        if (type == 4)
        {
            if (_playerData.smFinishCount >= 5)
                return true;
            if (_playerData.smAcceptCount >= 5)
                return true;
            if (_playerData.smFinishCount + _playerData.smAcceptCount >= 5)
                return true;
        }
        else if (type == 5)
        {
            if (_playerData.ymFinishCount >= 5)
                return true;
            if (_playerData.ymAcceptCount >= 5)
                return true;
            if (_playerData.ymFinishCount + _playerData.ymAcceptCount >= 5)
                return true;
        }
        return false;
    }

    void Player::ColorTaskAccept(UInt8 type, UInt32 taskid)
    {
        if (type == 4)
        {
            for (int i = 0; i < 6; ++i)
            {
                if (_playerData.shimen[i] == taskid)
                {
                    //_playerData.shimen[i] = 0;
                    _playerData.smcolor[i] |= 0xF0;
                    ++_playerData.smAcceptCount;
                    sendColorTask(0, 0);
                    return;
                }
            }
        }
        if (type == 5)
        {
            for (int i = 0; i < 6; ++i)
            {
                if (_playerData.yamen[i] == taskid)
                {
                    //_playerData.yamen[i] = 0;
                    _playerData.ymcolor[i] |= 0xF0;
                    ++_playerData.ymAcceptCount;
                    sendColorTask(1, 0);
                    return;
                }
            }
        }
    }

    void Player::clearFinishCount()
    {
        _playerData.smFinishCount = 0;
        _playerData.ymFinishCount = 0;
    }

    bool Player::finishClanTask(UInt32 taskId)
    {
		const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(taskId);
        if(taskType.m_Class != 6)
        {
            return false;
        }

        if(getClan() == NULL)
        {
            delClanTask();
            return false;
        }

        if(taskId != _playerData.clanTaskId || _playerData.ctFinishCount > CLAN_TASK_MAXCOUNT - 1)
            return false;

        ++ _playerData.ctFinishCount;
        if(CLAN_TASK_MAXCOUNT > _playerData.ctFinishCount)
        {
            URandom rnd(time(NULL));
            const std::vector<UInt32>& task = GData::GDataManager::GetClanTask();
            _playerData.clanTaskId = task[rnd(task.size())];
            GetTaskMgr()->AddCanAcceptTask(_playerData.clanTaskId);
        }
        else
        {
            _playerData.clanTaskId = 0;
        }

        writeClanTask();
        return true;
    }

    void Player::delClanTask()
    {
        GetTaskMgr()->DelTask(_playerData.clanTaskId);
        _playerData.clanTaskId = 0;
        _playerData.ctFinishCount = 0;

        writeClanTask();
    }

    void Player::buildClanTask()
    {
        if(getClan() == NULL)
        {
            return;
        }

        const std::vector<UInt32>& task = GData::GDataManager::GetClanTask();
        if(task.size() == 0)
            return;

        if(_playerData.clanTaskId == 0)
        {
            URandom rnd(time(NULL));
            _playerData.clanTaskId = task[rnd(task.size())];
        }
        else
        {
            const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(_playerData.clanTaskId);
            if(taskType.m_Class != 6)
            {
                URandom rnd(time(NULL));
                _playerData.clanTaskId = task[rnd(task.size())];
            }
        }

        GetTaskMgr()->AddCanAcceptTask(_playerData.clanTaskId);
        _playerData.ctFinishCount = 0;
        writeClanTask();

    }


	void Player::writeClanTask()
	{
        Stream st(0x98);
        st << static_cast<UInt8>(8) << ((_playerData.ctFinishCount << 4) | CLAN_TASK_MAXCOUNT);
        st << Stream::eos;
        send(st);

		DB().PushUpdateData("UPDATE `player` SET `clantask` = '%u,%u' WHERE `id` = %"I64_FMT"u",  _playerData.clanTaskId, _playerData.ctFinishCount, _id);
	}

    UInt32 Player::getClanTaskId()
    {
        return _playerData.clanTaskId;
    }

    bool Player::isClanTask(UInt32 taskId)
    {
        return _playerData.clanTaskId == taskId;
    }

    bool Player::isClanTaskFull()
    {
        return CLAN_TASK_MAXCOUNT  - 1 < _playerData.ctFinishCount;
    }

    void Player::AddClanBuilding(UInt32 building)
    {
        Clan* clan = getClan();
        if(clan == NULL)
        {
            return;
        }

        clan->addConstruction(building);
   }

    void Player::AddClanContrib(UInt32 contrib)
    {
        Clan* clan = getClan();
        if(clan == NULL)
        {
            return;
        }

        clan->addMemberProffer(this, contrib);
    }

	inline UInt32 getTavernPriceByColor(UInt8 color)
	{
        return 0;
		switch(color)
		{
		case 0:
		case 1:
			return 100000;
			break;
		case 2:
			return 500000;
			break;
		case 3:
			return 5000000;
			break;
		default:
			return 99999999;
			break;
		}
		return 99999999;
	}

	inline UInt32 getTavernPrice(UInt32 id)
	{
		Fighter * fgt = globalFighters[id];
		if(fgt == NULL)
			return 0;
		return getTavernPriceByColor(fgt->getColor());
	}

	void Player::flushTaskColor(UInt8 tasktype, UInt8 type, UInt8 color, UInt16 count, bool force)
    {
        int ttype = 0;
        if (tasktype >= 1)
            ttype = 1;
        else
            ttype = 0;

        int ftype = 0;
        if (type == 1)
            ftype = 0;
        if (type == 2 || type == 3)
            ftype = 1;
        else
            ftype = 0;

        bool first = false;
        if (!force) {
            if (ttype == 0) {
                if (!_playerData.smcolor[0])
                    first = true;
            }
            if (ttype == 1) {
                if (!_playerData.ymcolor[0])
                    first = true;
            }
        }

        UInt16 ncount = 0;
        if (type || first) {
            if (ftype) {
                if (getGold() < 2) {
                    sendMsgCode(1, 1007);
                    return;
                }
            }

            const std::vector<UInt8>& factor = GData::GDataManager::GetFlushTaskFactor(ttype, ftype);
            UInt8 rfac[5] = {0};
            rfac[0] = factor[0];
            for (int i = 1; i < 5; ++i) {
                rfac[i] = rfac[i-1] + factor[i];
            }

            bool percolor = false;
            do {
                ++ncount;
                if ((!ftype && ((ttype == 0 && _playerData.smFreeCount < 5) || (ttype == 1 && _playerData.ymFreeCount < 5))) || ftype) {
                    URandom rnd(time(NULL));
                    const std::vector<UInt32>& task = GData::GDataManager::GetShiYaMenTask(_playerData.country, ttype);
                    if (!task.size())
                        return;
                    std::set<UInt32> idxs;
                    if (task.size() <= 6) {
                        for (size_t i = 0; i < task.size(); ++i)
                            idxs.insert(i);
                    } else {
                        for (int i = 0; i < 6; ++i) {
                            UInt32 j = rnd(task.size());
                            while (idxs.find(j) != idxs.end())
                                j = rnd(task.size());
                            idxs.insert(j);
                        }
                    }

                    int n = 0;
                    for (std::set<UInt32>::iterator i = idxs.begin(), e = idxs.end(); i != e; ++i) {
                        UInt32 rd = rnd(100);
                        for (int j = 0; j < 5; ++j) {
                            if (rd <= rfac[j]) {
                                if (ttype == 0) {
                                    _playerData.shimen[n] = task[*i];
                                    _playerData.smcolor[n] = j+1;
                                } else {
                                    _playerData.yamen[n] = task[*i];
                                    _playerData.ymcolor[n] = j+1;
                                }
                                if (j+1 == color)
                                    percolor = true;
                                ++n;
                                break;
                            }
                        }
                    }

                    if (!ftype && !first) {
                        if (ttype == 0)
                            ++_playerData.smFreeCount;
                        else
                            ++_playerData.ymFinishCount;
                    }
                }

                if (percolor)
                    break;
                if (type == 2)
                    break;
                --count;
            } while (count > 0);

            if (!ttype)
                writeShiMen();
            else
                writeYaMen();
        }

        sendColorTask(ttype, ncount);
    }

    void Player::sendColorTask(UInt8 ttype, UInt16 ncount)
    {
        Stream st(0x8B);
        st << ttype;
        if (ttype == 0) 
        {
            st <<  ncount << _playerData.smFinishCount;
            st << _playerData.smFreeCount;
        }
        else
        {
            st <<  ncount << _playerData.ymFinishCount;
            st << _playerData.ymFreeCount;
        }

        if (ttype == 0) {
            for (int i = 0; i < 6; ++i) {
                if (_playerData.smcolor[i] & 0xF0)
                {
                    st << static_cast<UInt32>(0);
                    st << static_cast<UInt8>(0);
                }
                else
                {
                    st << _playerData.shimen[i];
                    st << static_cast<UInt8>(_playerData.smcolor[i]&0x0F);
                }
            }
        } else {
            for (int i = 0; i < 6; ++i) {
                if (_playerData.ymcolor[i] & 0xF0)
                {
                    st << static_cast<UInt32>(0);
                    st << static_cast<UInt8>(0);
                }
                else
                {
                    st << _playerData.yamen[i];
                    st << static_cast<UInt8>(_playerData.ymcolor[i]&0x0F);
                }
            }
        }
        st << Stream::eos;
        send(st);
    }

    void Player::listRecruit(UInt8 type, UInt8 color, UInt16 count)
	{
		UInt32 curtime = TimeUtil::Now();
		bool extraRefresh = false;
		UInt16 usedGold = 0, maxGold = _recruit_cost * count;

		if(_nextTavernUpdate == 0)
		{
			maxGold = 0;
			extraRefresh = true;
			count = 1;
			updateNextTavernUpdate(curtime);
		}
		else if(type > 0 && curtime >= _nextTavernUpdate)
		{
			if(type == 1)
			{
				maxGold -= _recruit_cost;
				extraRefresh = true;
				updateNextTavernUpdate(curtime);
			}
		}

		if(_playerData.gold < maxGold)
		{
			sendMsgCode(1, 2029);
			return;
		}

        while(color > 0)
        {
            if(globalFighters.getColorFighterNum(color) > 0)
                break;
            -- color;
        }

		UInt16 tcount = 0;
		Stream st(0x26);
		if(count > 0)
		{
			do
			{
				bool hasBlue = false, hasPurple = false;
				std::set<UInt32> excepts, excepts2;
				int i;
				for(i = 0; i < 6; ++ i)
					excepts2.insert(_playerData.tavernId[i]);
				i = 0;
				if(!extraRefresh)
				{
					usedGold += _recruit_cost;
					if(_playerData.tavernBlueCount >= _tavernBlueCount)
					{
						Fighter * fgt = globalFighters.getRandomOut(this, excepts, excepts2, 2, _tavernRate);
						if(fgt != NULL)
						{
							_playerData.tavernId[i] = fgt->getId();
							excepts.insert(fgt->getId());

							hasBlue = true;
							++ i;
						}
					}
					if(_playerData.tavernPurpleCount >= _tavernPurpleCount)
					{
						Fighter * fgt = globalFighters.getRandomOut(this, excepts, excepts2, 3, _tavernRate);
						if(fgt != NULL)
						{
							_playerData.tavernId[i] = fgt->getId();
							excepts.insert(fgt->getId());

							hasPurple = true;
							++ i;
						}
					}
				}
				for(; i < 6; ++ i)
				{
					Fighter * fgt = globalFighters.getRandomOut(this, excepts, excepts2, extraRefresh ? 0 : 1, _tavernRate);
					if(fgt == NULL)
					{
						_playerData.tavernId[i] = 0;
					}
					else
					{
						_playerData.tavernId[i] = fgt->getId();
						excepts.insert(fgt->getId());
						UInt8 color = fgt->getColor();
						switch(color)
						{
						case 2:
							hasBlue = true;
							break;
						case 3:
							hasPurple = true;
							break;
						default:
							break;
						}
					}
				}
				if(extraRefresh)
				{
					extraRefresh = false;
				}
				else
				{
					++ tcount;
					if(hasBlue)
						_playerData.tavernBlueCount = 0;
					else
						++ _playerData.tavernBlueCount;
					if(hasPurple)
						_playerData.tavernPurpleCount = 0;
					else
						++ _playerData.tavernPurpleCount;
				}
				-- count;
				if(hasBlue)
				{
					if(color <= 2)
						break;
				}
				if(hasPurple)
				{
					if(color <= 3)
						break;
				}
			}
			while(count > 0);
			st << calcNextTavernUpdate(curtime);
			if(type == 2)
				st << tcount;
			else
				st << static_cast<UInt16>(0);
			writeTavernIds();
			ConsumeInfo ci(FlushFighter, 0, 0);
			useGold(usedGold, &ci);
		}
		else
		{
			st << calcNextTavernUpdate(curtime) << static_cast<UInt16>(0);
		}
		for(int i = 0; i < 6; ++ i)
		{
			st << _playerData.tavernId[i] << getTavernPrice(_playerData.tavernId[i]);
		}
		st << Stream::eos;
		send(st);
	}

	UInt16 Player::calcNextTavernUpdate(UInt32 curtime)
	{
		if(_nextTavernUpdate > curtime)
			return static_cast<UInt16>(_nextTavernUpdate - curtime);
		return 0;
	}

	UInt32 Player::hireRecruit( UInt8 idx )
	{
		if(idx > 5 || _playerData.tavernId[idx] == 0)
			return 0;
		if(isFighterFull())
		{
			sendMsgCode(0, 2006);
			return 0;
		}
		UInt32 price = getTavernPrice(_playerData.tavernId[idx]);
		if(_playerData.tael < price)
		{
			sendMsgCode(0, 2009);
			return 0;
		}
		Fighter * fgt = takeFighter(_playerData.tavernId[idx], true);
		if(fgt == NULL)
			return 0;
		_playerData.tavernId[idx] = 0;
		writeTavernIds();
		ConsumeInfo ci(RecruitFighter,0,0);
		useTael(price,&ci);

		return fgt->getId();
	}

	void Player::updateNextTavernUpdate(UInt32 curtime)
	{
		_nextTavernUpdate = (curtime + _tavernInterval) / _tavernInterval * _tavernInterval;
	}

	void Player::exceptAvailableFighters( std::map<UInt32, UInt32>& data )
	{
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
		{
			if(it->first > 6)
				data.erase(it->first);
		}
	}

	void Player::sendMsgCode( UInt8 type, UInt32 code, UInt32 data )
	{
		if(!_isOnline)
			return;
		Stream st(0x0F);
		st << type << code << data << Stream::eos;
		send(st);
	}

	bool Player::canClosePK()
	{
		return getBuffData(PLAYER_BUFF_PKLOCK) == 0;
	}

	void Player::moveToHome()
	{
		if(_playerData.country == 0)
			moveTo(0x0005, true);
		else
			moveTo(0x1002, true);
	}

	void Player::moveToNeutralHome()
	{
		moveTo(0x3004, true);
	}

	void Player::sendGreatFighterMet()
	{
		Stream st(0xB8);
		UInt8 data[(GREAT_FIGHTER_MAX + 8) / 8] = {0};
		st << static_cast<UInt8>(20);
		std::map<UInt32, GreatFighterTaskVal>::iterator it;
		for(it = _greatFighterTaskVal.begin(); it != _greatFighterTaskVal.end(); ++ it)
		{
			if (it->second.friendliness > 0)
				data[it->first >> 3] |= 1 << (it->first & 0x07);
		}
		st.append(data, 20);
		st << Stream::eos;
		send(st);
	}

	void Player::autoRegenAll()
	{
		if(hasFlag(CountryBattle | ClanBattling))
			return;
		UInt32 autohp = getBuffData(0);
		if(autohp == 0)
			return;
		for(int i = 0; i < 5; ++ i)
		{
			Fighter * fighter = getLineup(i).fighter;
			if(fighter == NULL)
				continue;
			UInt16 hp = fighter->getCurrentHP();
			if(hp == 0)
				continue;
			UInt32 maxhp = fighter->getMaxHP();
			if(hp < maxhp)
			{
				if(hp + autohp >= maxhp)
				{
					autohp -= maxhp - hp;
					hp = 0;
				}
				else
				{
					hp += autohp;
					fighter->setCurrentHP(hp);
					autohp = 0;
					break;
				}
			}
			else
				hp = 0;
			fighter->setCurrentHP(hp);
		}
		setBuffData(0, autohp);
	}

	UInt32 Player::getClientAddress()
	{
        TcpConnection conn = NETWORK()->GetConn(this->GetSessionID());
		if(conn.get() == NULL)
			return 0;
        Network::GameClient * client = static_cast<Network::GameClient *>(conn.get());
        return client->GetClientIP();
	}

	void Player::makeWallow( Stream& st )
	{
		UInt8 wallow, stage;
		UInt32 duration;
		wallow = _playerData.wallow;
		if(wallow)
		{
			UInt32 onlineDuration = (_onlineDuration + TimeUtil::Now() - _playerData.lastOnline);
			if(onlineDuration < 3 * 60 * 60)
			{
				stage = 0;
				duration = 3 * 60 * 60 - onlineDuration;
			}
			else if(onlineDuration < 5 * 60 * 60)
			{
				stage = 1;
				duration = 5 * 60 * 60 - onlineDuration;
			}
			else
			{
				stage = 2;
				duration = 5 * 60 * 60;
			}
		}
		else
		{
			stage = 0;
			duration = 0;
		}
		st.init(0x12);
		st << wallow << stage << duration << Stream::eos;
	}

	void Player::sendWallow( )
	{
		if(!cfg.enableWallow)
			return;

		Stream st;
		makeWallow(st);
		send(st);
	}

	void Player::setGMLevel( UInt8 l )
	{
		if(l > 3)
			l = 3;
		if(_playerData.gmLevel == l)
			return;
		_playerData.gmLevel = l;
		DB().PushUpdateData("UPDATE `player` SET `gmLevel` = %u WHERE `id` = %"I64_FMT"u", l, _id);
	}

	void Player::autoLineup( Fighter * fgt )
	{
		if(_fighters.size() <= 5)
		{
			UInt8 c = 0, fe = 0;
			for(UInt8 i = 0; i < 5; ++ i)
			{
				Lineup& lu = _playerData.lineup[i];
				if(lu.available())
					++ c;
				else
				{
					if(fe == 0)
						fe = i;
				}
			}
			if(c >= 5)
				return;
			else
			{
				_playerData.lineup[fe].fighter = fgt;
				_playerData.lineup[fe].updateId();
				++ c;
			}
            UInt8 newPos[5] = {0};
            if(0 == _playerData.formation)
            {
                setFormation(FORMATION_1); // 七绝阵
            }

            for( int i = 0; i < 5; ++ i)
            {
                newPos[i] = GData::formationManager[_playerData.formation]->operator[](i).pos;
            }

			int starti = 0;
			int endi = c - 1;
			Lineup& lu1 = _playerData.lineup[0];
			bool mfSolid = lu1.fighter->getClass() == 3;
			if(mfSolid)
				lu1.pos = newPos[starti ++];
			for(int i = 1; i < 5; ++ i)
			{
				Lineup& lu = _playerData.lineup[i];
				if(!lu.available())
					continue;
				if(lu.fighter->getClass() == 3)
					lu.pos = newPos[starti ++];
				else
					lu.pos = newPos[endi --];
			}
			if(!mfSolid)
				lu1.pos = newPos[starti];
			updateBattleFighters();
			Stream st;
			makeFormationInfo(st);
			send(st);
		}
	}

	void Player::incIcCount( )
	{
		checkIcExpire(false);
		++ _playerData.icCount;
		DB().PushUpdateData("UPDATE `player` SET `icCount` = '%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.icCount, _playerData.nextIcReset, _id);
	}

	void Player::resetIcCount( )
	{
		checkIcExpire(false);
		if(_playerData.icCount > 0)
			_playerData.icCount = 0;
		DB().PushUpdateData("UPDATE `player` SET `icCount` = '%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.icCount, _playerData.nextIcReset, _id);
	}

	UInt8 Player::getIcCount()
	{
		checkIcExpire();
		return _playerData.icCount;
	}

	void Player::checkIcExpire( bool writedb )
	{
		UInt32 now = TimeUtil::Now();
		if(now >= _playerData.nextIcReset)
		{
			_playerData.nextIcReset = TimeUtil::SharpDay(1, now);
			_playerData.icCount = 0;
			DB().PushUpdateData("UPDATE `player` SET `icCount` = '%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.icCount, _playerData.nextIcReset, _id);
		}
	}

	void Player::checkHPLoss()
	{
		if(!hasStatus(FirstHPLoss))
		{
			Stream st(0x13);
			st << static_cast<UInt16>(1) << Stream::eos;
			send(st);
			addStatus(FirstHPLoss);
		}
	}

	void Player::checkDeath()
	{
		if(!hasStatus(FirstDeath))
		{
			Stream st(0x13);
			st << static_cast<UInt16>(2) << Stream::eos;
			send(st);
			addStatus(FirstDeath);
		}
	}

	void Player::checkLevUp(UInt8 oLev, UInt8 nLev)
	{
		if(_clan != NULL)
		{
			_clan->broadcastMemberInfo(this);
		}
		m_TaskMgr->CheckCanAcceptTaskByLev(nLev);
        sendOnlineReward();
		if ((nLev >= 30 && !m_Athletics->hasEnterAthletics()) || (oLev < 51 && nLev >= 51))
		{
			GameMsgHdr hdr(0x19E, WORKER_THREAD_WORLD, this, sizeof(nLev));
			GLOBAL().PushMsg(hdr, &nLev);
			setBlockBossByLevel();
		}
		Map * map = GetMap();
		if(map != NULL)
			map->OnPlayerLevUp(this);

		GameAction()->onLevelup(this, oLev, nLev);
		if(nLev >= 30)
		{
			UInt8 buffer[7];
			*reinterpret_cast<UInt8 *>(buffer) = nLev;
			*reinterpret_cast<UInt8 *>(buffer + 1) = _bossLevel;
			*reinterpret_cast<UInt16 *>(buffer + 2) = 0;
			*reinterpret_cast<UInt16 *>(buffer + 4) = 0;
			*reinterpret_cast<UInt8 *>(buffer + 6) = static_cast<UInt8>(0);
			GameMsgHdr hdr1(0x1A4, WORKER_THREAD_WORLD, this, 7);
			GLOBAL().PushMsg(hdr1, buffer);
		}
	}

    void Player::sendFormationList()
    {
        Stream st(0x1D);
        UInt8 cnt = _playerData.formations.size();
        st << static_cast<UInt8>(0) << cnt;
        for( int idx = 0; idx < cnt; ++ idx )
        {
            st << _playerData.formations[idx];
        }

        st << Stream::eos;
        send(st);
    }

    bool Player::formationLevUp(UInt16 formationId)
    {
        const GData::Formation* formation = GData::formationManager[formationId];
        if(formation == NULL)
            return false;

        Stream st(0x1D);
        st << static_cast<UInt8>(1);
        int cnt = formation->getLevUpItemCount();
        if(0 == cnt)
        {
            st << static_cast<UInt8>(0) << Stream::eos;
            send(st);

            SYSMSG_SENDV(2100, this, formation->getName().c_str());
            return false;
        }

        for( int idx = 0; idx < cnt; ++ idx )
        {
            UInt32 itemId = formation->LevUpItem(idx);
            if(0 == m_Package->GetItemNum(itemId))
            {
                st << static_cast<UInt8>(0) << Stream::eos;
                send(st);
                const GData::ItemBaseType* item = GData::itemBaseTypeManager[itemId];
                if(item)
                    SYSMSG_SENDV(2101, this, item->getName().c_str());
                return false;
            }
        }

        UInt16 newFormationId = formationId + 1;
        const GData::Formation* newformation = GData::formationManager[newFormationId];
        if(NULL == newformation)
        {
            st << static_cast<UInt8>(0) << Stream::eos;
            send(st);

            SYSMSG_SENDV(2100, this, formation->getName().c_str());
            return false;
        }

        std::vector<UInt16>& act_form = _playerData.formations;
        cnt = act_form.size();
        for( int idx = 0; idx < cnt; ++ idx )
        {
            if( act_form[idx] == formationId )
                act_form.erase(act_form.begin() + idx);
        }

        addNewFormation(newFormationId, true);

        for( int idx = 0; idx < cnt; ++ idx )
        {
            m_Package->DelItemAny(formation->LevUpItem(idx), 1);
        }

        if(_playerData.formation == formationId)
			setFormation(newFormationId);

        st << static_cast<UInt8>(1) << Stream::eos;
        send(st);
        return true;
    }

    bool Player::addNewFormation(UInt16 newformationId, bool writedb)
    {
        const GData::Formation* newformation = GData::formationManager[newformationId];
        if(NULL == newformation)
            return false;

        _playerData.formations.push_back(newformationId);

        if(!writedb)
            return true;

        std::string formations = "";
        int cnt = _playerData.formations.size();

        formations += Itoa(_playerData.formations[0]);
        for(int idx = 1; idx < cnt; ++ idx)
        {
            formations += ",";
            formations += Itoa(_playerData.formations[idx]);
        }

		DB().PushUpdateData("UPDATE `player` SET `formations` = '%s' WHERE id = %" I64_FMT "u", formations.c_str(), _id);

        Stream st(0x1D);
        st << static_cast<UInt8>(2) << newformationId << Stream::eos;
        send(st);

        return true;
    }

	void Player::addTotalRecharge( UInt32 r )
	{
		if(r == 0)
			return;
		UInt32 oldRecharge = _playerData.totalRecharge;
		UInt32 oldVipLevel = _vipLevel;
		_playerData.totalRecharge += r;
		recalcVipLevel();
		if((cfg.limitLuckyDraw == 1 && oldVipLevel < 2) && _vipLevel >= 2)
		{
			sendModification(5, _playerData.status, false);
		}
		sendModification(7, _playerData.totalRecharge);
		if(!hasStatus(TopupRewarded))
		{
			oldVipLevel = 0;
			addStatus(TopupRewarded);
		}
		if(oldRecharge == 0)
		{
			SYSMSG(title, 254);
			SYSMSG(content, 255);
			MailPackage::MailItem mitem[1] = {{8998, 1}};
			MailItemsInfo itemsInfo(mitem, FirstReChargeAward, 1);
			m_MailBox->newMail(NULL, 0x21, title, content, 8998 + 0x10000, true, &itemsInfo);
		}
		sendVIPMails(oldVipLevel + 1, _vipLevel);
		//if(World::_newYearStage > 0)
		//	GameAction()->onTopup(this, oldRecharge, _playerData.totalRecharge);
	}
	void Player::sendTopupMail(const char* title, const char* content, UInt32 gold, UInt8 num)
	{
		m_MailBox->newMail(NULL, 0x01, title, content);
		DBLOG().PushUpdateData("insert into `topup_num`(`server_id`, `player_id`, `topup_gold`, `rand_num`, `topup_time`) values(%u, %"I64_FMT"u, %u, %u, %u)", cfg.serverLogId, getId(), gold, num, TimeUtil::Now());
	}

	void Player::setTotalRecharge( UInt32 r )
	{
		if(r == _playerData.totalRecharge)
			return;
		_playerData.totalRecharge = r;
		if(cfg.limitLuckyDraw == 1 && _vipLevel < 2)
		{
			recalcVipLevel();
			if(_vipLevel >= 2)
				sendModification(5, _playerData.status, false);
		}
		else
			recalcVipLevel();
		sendModification(7, _playerData.totalRecharge);
	}

	void Player::genOnlineRewardItems()
	{
        UInt32 now = TimeUtil::Now();
        _playerData.nextRewardTime = GData::GDataManager::GetOnlineAwardTime(_playerData.rewardStep) + now - _playerData.lastOnline;
		writeOnlineRewardToDB();
	}

	void Player::writeOnlineRewardToDB()
	{
		DB().PushUpdateData("UPDATE `player` SET `nextReward` = '%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.rewardStep, _playerData.nextRewardItem, _playerData.nextRewardCount, _playerData.nextRewardTime, _id);
	}

	bool Player::takeOnlineReward()
	{
		UInt32 now = TimeUtil::Now();
		if(_playerData.lastOnline + _playerData.nextRewardTime > now + 60)
			return false;

        const std::vector<UInt16>& ids = GData::GDataManager::GetOnlineAward(GetClass(),  _playerData.rewardStep);
        UInt8 size = ids.size();
        if (!size)
            return false;

        for (UInt8 i = 0; i < size; i += 2)
        {
            if(!m_Package->Add(ids[i], ((UInt8)(i+1)>=ids.size())?1:ids[i+1], true, false, FromOnlineAward))
                return false;
        }

        UInt8 count = GData::GDataManager::GetOnlineAwardCount();
		if(_playerData.rewardStep >= count)
			_playerData.rewardStep = count;
		else
			++_playerData.rewardStep;

		genOnlineRewardItems();
		return true;
	}

	UInt32 Player::getOnlineReward()
	{
		UInt32 now = TimeUtil::Now();
		if(_playerData.nextRewardTime == 0)
		{
            _playerData.rewardStep = 0;
			genOnlineRewardItems();
		}
        else 
        {
            if (_playerData.rewardStep >= GData::GDataManager::GetOnlineAwardCount())
            {
                _playerData.nextRewardTime = -1;
                return -1;
            }
        }
		UInt32 deadline = _playerData.nextRewardTime + _playerData.lastOnline;
		if(deadline <= now)
			return 0;
		return deadline - now;
	}

	void Player::sendOnlineReward()
	{
		Stream st(0x38);
        UInt32 left = getOnlineReward();
        if (left == static_cast<UInt32>(-1))
        {
            st << static_cast<UInt16>(-1);
            st << Stream::eos;
            send(st);
            return ;
        }
        else
            st << static_cast<UInt16>(left);

        size_t pos = st.size();
        st << static_cast<UInt8>(0);

        UInt8 n = 0;
        const std::vector<UInt16>& ids = GData::GDataManager::GetOnlineAward(GetClass(),  _playerData.rewardStep);
        UInt8 size = ids.size();
        if (size)
        {
            for (; n < size; n+=2)
            {
                st << ids[n]; 
                if (n+1 >= size)
                {
                    st << static_cast<UInt8>(1);
                    break;
                }
                else
                    st << static_cast<UInt8>(ids[n+1]);
            }
        }

        st.data<UInt8>(pos) = (n+1)/2;
        st << Stream::eos;
		send(st);
	}

	struct Dungeon_Enum
	{
		Player * player;
		Stream& st;
	};

	bool enum_dm(Dungeon * dg, Dungeon_Enum * de)
	{
		dg->buildInfo(de->player, de->st);
		return true;
	}

	void Player::sendDailyInfo()
	{
		Stream st(0x5F);
		st << static_cast<UInt8>(1);
		bossManager.buildInfo(st);
		st << static_cast<UInt8>(dungeonManager.size());
		Dungeon_Enum de = {this, st};
		dungeonManager.enumerate(enum_dm, &de);
		size_t sz;
		UInt16 * prices = Dungeon::getPrice(sz);
		st << static_cast<UInt8>(sz);
		for(size_t i = 0; i < sz; ++ i)
			st << prices[i];
		st << Stream::eos;
		send((st));
	}

	void Player::regenAll()
	{
		for(int i = 0; i < 5; ++ i)
		{
			Lineup& pd = _playerData.lineup[i];
			if(pd.fighter != NULL && pd.fighter->getCurrentHP() != 0)
			{
				pd.fighter->setCurrentHP(0);
			}
		}
	}

	UInt8 Player::trainFighter( UInt32 id, UInt8 type )
	{
#define ITEM_TRAIN_TYPE1 506         // 补髓丹(不带保护) 资质
#define ITEM_TRAIN_TYPE2 507         // 补髓益元丹(带保护) 资质
#define ITEM_TRAIN_TYPE3 508         // 凝神丹(不带保护) 潜力
#define ITEM_TRAIN_TYPE4 509         // 凝神易经丹(带保护) 潜力
		Fighter * fgt = findFighter(id);
		if(fgt == NULL)
			return 1;

        bool isPotential = false;
        float p = 0; 
		UInt32 rate = 0;
        UInt32 itemId = ITEM_TRAIN_TYPE1 + type - 1;

        if(type == 1 || type == 2)
        {
		    p = fgt->getCapacity();
            std::vector<UInt32>& chance = GObjectManager::getCapacityChance();
            size_t cnt = chance.size();
            for(UInt32 idx = 0; idx < cnt; idx ++)
            {
                if(p < static_cast<float>(CHANCECOND(chance[idx]))/100)
                {
                    rate = CHANCEVALUE(chance[idx]);
                    break;
                }
            }
        }
        else if(type == 3 || type == 4)
        {
            isPotential = true;
		    p = fgt->getPotential();
            std::vector<UInt32>& chance = GObjectManager::getPotentialChance();
            size_t cnt = chance.size();
            for(UInt32 idx = 0; idx < cnt; idx ++)
            {
                if(p < static_cast<float>(CHANCECOND(chance[idx]))/100)
                {
                    rate = CHANCEVALUE(chance[idx]);
                    break;
                }
            }
        }

		Fighter * fgt_orig = globalFighters[id];
		if(fgt_orig == NULL)
			return 1;

		if(rate == 0)
			return 1;

		if(!m_Package->DelItemAny(itemId, 1))
			return 1;
		
		if(uRand(1000) < rate)
		{
			p += 0.01f;
			p = floorf(p * 100.0f + 0.5f) / 100.0f;

            if(isPotential)
            {
                if(p > GObjectManager::getMaxPotential()/100)
                    p = GObjectManager::getMaxPotential()/100;
                fgt->setPotential(p);
            }
            else
            {
                if(p > GObjectManager::getMaxCapacity()/100)
                    p = GObjectManager::getMaxCapacity()/100;
                fgt->setCapacity(p);
            }
		}
		else
		{
			if(type == 1 || type == 3)
			{
                if(isPotential)
                {
                    float decp = fgt->getPotential() - 0.01f;
                    float newp = std::max(std::max(decp, static_cast<float>(GObjectManager::getMinPotential())/100), fgt_orig->getPotential());
                    fgt->setPotential(newp);
                }
                else
                {
                    float decp = fgt->getCapacity() - 0.01f;
                    float newp = std::max(std::max(decp, static_cast<float>(GObjectManager::getMinCapacity())/100), fgt_orig->getCapacity());
                    fgt->setCapacity(newp);
                }
			}
			return 1;
		}
		return 0;
	}

	void Player::sendAllFightersInfoToGM()
	{
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
		{
			SYSMSG_SENDV(604, this, it->second->getId(), it->second->getColor(), it->second->getName().c_str(), it->second->getLevel(), it->second->getPotential());
		}
	}

	void Player::pendExp( UInt32 exp, bool leaveCity )
	{
		_playerData.lastExp += exp;
		if(leaveCity)
			_playerData.lastExp |= 0x80000000;
		DB().PushUpdateData("UPDATE `player` SET `lastExp` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastExp, _id);
	}

	void Player::pendTael( UInt32 t )
	{
		_playerData.lastResource = (_playerData.lastResource & 0xFFFFFFFFFFFF0000ull) | ((_playerData.lastResource & 0xFFFFull) + t);
		DB().PushUpdateData("UPDATE `player` SET `lastResource` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastResource, _id);
	}

	void Player::pendCoupon( UInt32 c )
	{
		_playerData.lastResource = (_playerData.lastResource & 0xFFFFFFFF0000FFFFull) | ((((_playerData.lastResource >> 16) + static_cast<UInt64>(c)) & 0xFFFFull) << 16);
		DB().PushUpdateData("UPDATE `player` SET `lastResource` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastResource, _id);
	}

	void Player::pendCoin( UInt32 c )
	{
		_playerData.lastResource = (_playerData.lastResource & 0xFFF00000FFFFFFFFull) | ((((_playerData.lastResource >> 32) + static_cast<UInt64>(c)) & 0xFFFFFull) << 32);
		DB().PushUpdateData("UPDATE `player` SET `lastResource` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastResource, _id);
	}

	void Player::pendAchievement( UInt32 a )
	{
		_playerData.lastResource = (_playerData.lastResource & 0x000FFFFFFFFFFFFFull) | (((_playerData.lastResource >> 52) + static_cast<UInt64>(a)) << 52);
		DB().PushUpdateData("UPDATE `player` SET `lastResource` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastResource, _id);
	}

	void Player::setTavernInterval( UInt32 inter )
	{
		_tavernInterval = inter;
	}

	void Player::setTavernRate( UInt32 rate )
	{
		_tavernRate = rate;
	}

    void Player::setRecruitCost(UInt32 recruit_cost)
    {
        _recruit_cost = recruit_cost;
    }

    void Player::setTavernBlueCount(UInt32 tavernBlueCount)
    {
        _tavernBlueCount = tavernBlueCount;
    }

    void Player::setTavernPurpleCount(UInt32 tavernPurpleCount)
    {
        _tavernPurpleCount = tavernPurpleCount;
    }

	void Player::setNextExtraReward( UInt32 ner )
	{
		_playerData.nextExtraReward = ner;
		DB().PushUpdateData("UPDATE `player` SET `nextExtraReward` = %u WHERE `id` = %"I64_FMT"u", _playerData.nextExtraReward, _id);
	}

	bool Player::isDungeonPassed( UInt8 id )
	{
		Dungeon * dg = dungeonManager[id];
		if(dg == NULL)
			return false;
		return dg->getFirstPass(this) > 0;
	}

	void Player::sendFriendActList()
	{
		size_t sz = _friendActs.size();
		Stream st(0xAB);
		st << static_cast<UInt8>(sz);
		for(size_t i = 0; i < sz; ++ i)
		{
			appendFriendAct(st, _friendActs[i]);
		}
		st << Stream::eos;
		send(st);
	}

	void Player::appendFriendAct( Stream& st, FriendActStruct* fas )
	{
		st << fas->type << fas->player->getCountry() << fas->player->getName();
		switch(fas->type)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			st << fas->target->getCountry() << fas->target->getName();
			break;
		case 5:
			st << fas->str;
			break;
		case 6:
			st << static_cast<UInt16>(fas->num);
			break;
		case 7:
		case 8:
			st << static_cast<UInt8>(fas->num);
			break;
		}
	}

	void Player::pushFriendAct( FriendActStruct * fas )
	{
		if(_friendActs.size() > 14)
		{
			FriendActStruct * fas_ = _friendActs[0];
			delete fas_;
			_friendActs.erase(_friendActs.begin());
		}
		_friendActs.push_back(fas);
		if(_isOnline)
		{
			Stream st(0xAB);
			st << static_cast<UInt8>(0xFF);
			appendFriendAct(st, fas);
			st << Stream::eos;
			send(st);
		}
	}

	bool Player::testCanAddFriend( Player * pl )
	{
		Mutex::ScopedLock lk(_mutex);
		Mutex::ScopedLock lk2(pl->getMutex());
		if(isFriendFull())
		{
			sendMsgCode(2, 2083);
			return false;
		}
		if(_hasFriend(pl))
		{
			return false;
		}
		if(_hasBlock(pl))
		{
			sendMsgCode(2, 2080);
			return false;
		}
		if(_hasFoe(pl))
		{
			sendMsgCode(2, 2081);
			return false;
		}
		if(pl->isFriendFull())
		{
			sendMsgCode(2, 2084);
			return false;
		}
		if(pl->_hasBlock(this))
		{
			sendMsgCode(2, 2082);
			return false;
		}
		if(pl->_hasFoe(this))
		{
			sendMsgCode(2, 2082);
			return false;
		}
		return true;
	}

	std::string& Player::fixName( std::string& name )
	{
		if(cfg.merged && !name.empty())if(static_cast<UInt8>(*(name.end() - 1)) >= 32 && !_playerData.name.empty())
		{
			const std::string& pn = _playerData.name;
			size_t idx = pn.size() - 1;
			for(; idx > 0; -- idx)
			{
				if(static_cast<UInt8>(pn[idx]) >= 32)
					break;
			}
			name.insert(name.end(), pn.begin() + idx + 1, pn.end());
		}
		return name;
	}

	void Player::patchMergedName( UInt64 id, std::string& name )
	{
		if(cfg.merged && id >= 0x1000000000000ull)
		{
			UInt32 sid = static_cast<UInt32>(id >> 48) - 1;
			do
			{
				name.push_back(static_cast<char>((sid % 31) + 1));
				sid /= 31;
			}
			while(sid > 0);
		}
	}

	void Player::sendVIPMails( UInt8 l, UInt8 h )
	{
		if(l < 1)
			l = 1;
		if(h > 10)
			h = 10;
		for(UInt32 i = l; i <= h; ++ i)
		{
			SYSMSG(title, 256);
			SYSMSGV(content, 257, i);
			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
			if(mail == NULL)
				continue;
			const UInt32 vipRollTable[10][3] = {{3001, 3002, 0}, {3003, 3004, 3005}, {3006, 3007, 3008},
			{3009, 3010, 0}, {3011, 3012, 0}, {3013, 3014, 0}, {3015, 3016, 0}, {3017, 3018, 0},
			{3019, 3020, 0}, {3021, 3022, 0}};
			const UInt32 vipGoldTable[10] = { 0, 0, 0, 100, 500, 1000, 2000, 3000, 6000, 6000 };
			MailPackage::MailItem mitem[4];
			UInt32 mcount = 0;
			UInt32 g = vipGoldTable[i - 1];
			std::string strItems;
			if(g > 0)
			{
				mitem[mcount].id = MailPackage::Gold;
				mitem[mcount ++].count = g;
				strItems += Itoa(static_cast<UInt16>(MailPackage::Gold));
				strItems += ",";
				strItems += Itoa(g);
				strItems += "|";
			}
			const UInt32 * t = vipRollTable[i - 1];
			URandom ur(mail->id);
			for(UInt32 i = 0; i < 3 && t[i] > 0; ++ i)
			{
				const GData::LootItem * li = GData::lootTable[t[i]];
				GData::LootResult r = li->roll(&ur);
				mitem[mcount].id = r.id;
				mitem[mcount ++].count = r.count;
				strItems += Itoa(r.id);
				strItems += ",";
				strItems += Itoa(r.count);
				strItems += "|";
			}
			mailPackageManager.push(mail->id, mitem, mcount, true);
			DBLOG().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
		}
	}
	void Player::sendBlockBossMail(UInt8 l, UInt8 h)
	{
		UInt16 coupon = 0;
		if(h > 24)
		{
			for(UInt8 i = l; i < h; i ++)
			{
				if(i % 4 == 0)
					coupon += 5 * (i / 4 - 5);
			}

			if(coupon > 0)
			{
				MailPackage::MailItem mitem[1] = {{MailPackage::Coupon, coupon}};
				MailItemsInfo itemsInfo(mitem, Activity, 1);
				SYSMSG(title, 545);
				SYSMSGV(content, 546, coupon);
				Mail * pmail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
				if(pmail != NULL)
				{
					mailPackageManager.push(pmail->id, mitem, 1, false);
				}
			}
		}
	}

	void Player::setTicketCount(UInt32 cnt, bool writedb)
	{
		_exchangeTicketCount = cnt;
		if(writedb)
		{
			DB().PushUpdateData("REPLACE INTO `exchange_ticket` VALUES(%"I64_FMT"u, %u)", getId(), _exchangeTicketCount);
			DBLOG().PushUpdateData("replace into `exchange_ticket` VALUES(%u, %"I64_FMT"u, %u)", cfg.serverLogId, getId(), _exchangeTicketCount);
		}
	}

	void Player::setBlockBossByLevel()
	{
		UInt16 index = (_bossLevel - 21) % 4;
		UInt16 npcId = 0;
		if(_bossLevel < 81)
			npcId = 4800;
		else
			npcId = 4900;
		GData::NpcGroups::iterator it = GData::npcGroups.find(npcId + index);
		if(it == GData::npcGroups.end())
		{
			_ng = NULL;
			return;
		}
		_ng = it->second;
	}


	void Player::setBossLevel(UInt8 lvl, bool writedb)
	{
		if(_bossLevel == lvl)
			return;
		_bossLevel = lvl;
		if(writedb)
			DB().PushUpdateData("UPDATE `player` SET `bossLevel` = %u WHERE id = %"I64_FMT"u", _bossLevel, getId());
		setBlockBossByLevel();
	}


	bool Player::attackBlockBoss()
	{
		if(GetLev() < 30)
			return false;
		if(_ng == NULL)//error
			return false;
		if(_bossLevel % 4 == 0 && GetFreePackageSize() < 1)
		{
			sendMsgCode(0, 2016);
			return false;
		}
		UInt32 now = TimeUtil::Now();
		UInt32 buffLeft = getBuffData(PLAYER_BUFF_ATTACKING, now);
		if(buffLeft > now)
		{
			sendMsgCode(0, 2035, buffLeft - now);
			return false;
		}

		Battle::BattleSimulator bsim(0x7FFF, this, _ng->getName(), static_cast<UInt8>(_bossLevel));

		PutFighters(bsim, 0);
		std::vector<GData::NpcFData>& nflist = _ng->getList();
		size_t size = nflist.size();
		bsim.setFormation(1, _ng->getFormation());
		bsim.setPortrait(1, nflist[0].fighter->favor);
		Fighter *fgt_clone[25] = { NULL };
		for(size_t i = 0; i < size; ++ i)
		{
			GData::NpcFData& nfdata = nflist[i];
			fgt_clone[i] = nfdata.fighter->cloneWithEquip(NULL);
			fgt_clone[i]->setLevel(static_cast<UInt8>(_bossLevel));
			Battle::BattleFighter *bf = bsim.newFighter(1, nfdata.pos, fgt_clone[i]);
			bf->setAttrExtra(1, bf->getClass(), _bossLevel);
		}
		bsim.start();
		bsim.applyFighterHP(0, this, !hasFlag(CountryBattle | ClanBattling));
		for(size_t i = 0; i < size; ++ i)
		{
			SAFE_DELETE(fgt_clone[i]);
		}
		Stream& packet = bsim.getPacket();
		if(packet.size() <= 8)
			return false;
		Stream st(0x61);
		bool isWin = bsim.getWinner() == 1;
		UInt8 sz = 0;
		GData::LootResult lt = {0, 0};
		UInt16 couponCount = 0;
		if(isWin)
		{
			st << static_cast<UInt16>(0x0101);
			if(_bossLevel % 4 == 0)
			{
				const static UInt16 dietyFavor[] = {5825, 5826, 5827, 5828};
				const static UInt16 orangeFavor[] = {5820, 5821, 5822, 5823, 5824};
				const static UInt16 purpleFavor[] = {5813, 5814, 5815, 5816, 5817, 5818, 5819};
				if(_bossLevel < 84)
				{
					const UInt8 favorCount[] = {1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3};
					UInt16 index = _bossLevel / 4 - 6;
					lt.id = purpleFavor[uRand(7)];
					lt.count = favorCount[index];
				}
				else if(_bossLevel < 128)
				{
					const UInt8 favorCount[] = {2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4};
					UInt16 index = _bossLevel / 4 - 21;
					lt.id = orangeFavor[uRand(5)];
					lt.count = favorCount[index];
				}
				else
				{
					lt.id = dietyFavor[uRand(4)];		
					if(_bossLevel < 144)
						lt.count = 2;
					else if(_bossLevel < 160)
						lt.count = 3;
					else
						lt.count = 4;
				}
				couponCount = 5 * (_bossLevel / 4 - 5);//fix /////////
				pendCoupon(couponCount);
				m_Package->AddItem(lt.id, lt.count, true, true);
				_lastLoot.push_back(lt);
				sz = 1;
				addStatus(TopupRewarded);
			}

			setBossLevel(_bossLevel + 1);
			UInt8 buffer[7];
			*reinterpret_cast<UInt8 *>(buffer) = GetLev();
			*reinterpret_cast<UInt8 *>(buffer + 1) = _bossLevel;
			*reinterpret_cast<UInt16 *>(buffer + 2) = couponCount;
			*reinterpret_cast<UInt16 *>(buffer + 4) = lt.id;
			*reinterpret_cast<UInt8 *>(buffer + 6) = static_cast<UInt8>(lt.count);
			GameMsgHdr hdr(0x1A4, WORKER_THREAD_WORLD, this, 7);
			GLOBAL().PushMsg(hdr, buffer);

		}
		else
		{
			st << static_cast<UInt16>(0x0100);
		}
		st << 0 << static_cast<UInt8>(0);
		st << sz;
		if(sz != 0)
			st << lt.id << lt.count;
		st.append(&packet[8], packet.size() - 8);
		st << Stream::eos;
		send(st);

		setBuffData(PLAYER_BUFF_ATTACKING, 2 * bsim.getTurns() + now);

		return isWin;
	}


	void Player::setClan(Clan * c)
	{
		if(c == _clan)
			return;
		_clan = c;
		rebuildBattleName();
	}

	void Player::testBattlePunish()
	{
		UInt32 atktime = _buffData[PLAYER_BUFF_ATTACKING];
		if(atktime == 0)
			return;
		if(atktime > TimeUtil::Now())
			_playerData.battlecdtm = atktime;
	}

	void Player::rebuildBattleName()
	{
		char numstr[16];
		sprintf(numstr, "%u", _playerData.title);
		_battleName.clear();
		_battleName = getClanName();
		if(cfg.merged && !_battleName.empty() && static_cast<UInt8>(*(_battleName.end() - 1)) < 32)
		{
			sprintf(numstr, ".S%u", static_cast<UInt8>(*(_battleName.end() - 1)));
			_battleName.erase(_battleName.end() - 1);
			_battleName += numstr;
		}
		_battleName = _battleName + "\n" + numstr + "\n" + _playerData.name;
	}

	void Player::writeBookStoreIds()
	{
		DB().PushUpdateData("UPDATE `player` SET `bookStore` = '%u|%u|%u|%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.bookStore[0], _playerData.bookStore[1], _playerData.bookStore[2], _playerData.bookStore[3], _playerData.bookStore[4], _playerData.bookStore[5], _nextBookStoreUpdate, _id);
	}

	inline UInt32 getBookPriceById(UInt32 id)
	{
        UInt32 deftael = 5000;
        const std::vector<UInt32>& bookprice = GData::GDataManager::GetFlushBookPrice();
        if (!bookprice.size())
            return deftael;
        for (UInt32 i = 0; i < bookprice.size(); i+=2)
        {
            if (bookprice[i] == id)
                return bookprice[i+1];
        }
        return deftael;
	}

	void Player::listBookStore(UInt8 type)
	{
		UInt32 curtime = TimeUtil::Now();
		UInt16 money = 0;
        int count = 0;

		if(_nextBookStoreUpdate == 0 || curtime >= _nextBookStoreUpdate)
		{
            count = 1;
			updateNextBookStoreUpdate(curtime);
		}
		else if(type == 1)
		{
            count = 1;
            money = 50;
            // updateNextBookStoreUpdate(curtime);
        }

		if(type > 0 && _playerData.tael < money)
		{
			sendMsgCode(1, 1006);
			return;
		}

		Stream st(0x1A);
		if(count > 0)
		{
            const std::vector<UInt32>& factor = GData::GDataManager::GetFlushBookFactor(type);
            if (!factor.size())
                return;
            UInt32 totalfactor = factor[0];

			do
			{
				int i = 0;
				for(; i < 6; ++ i)
				{
                    UInt32 rnd = uRand(totalfactor);
                    UInt32 j = 1;
                    for (; j < factor.size(); j += 2)
                    {
                        if (rnd <= factor[j])
                            break;
                    }
                    ++j;

					UInt32 iid = factor[j];
					if(iid == 0)
					{
						_playerData.bookStore[i] = 0;
					}
					else
					{
						_playerData.bookStore[i] = iid;
					}
				}
				--count;
			}
			while(count > 0);

			st << calcNextBookStoreUpdate(curtime);
			writeBookStoreIds();

            if (money)
            {
                ConsumeInfo ci(FlushBookStore, 0, 0);
                useTael(money, &ci);
            }
		}
		else
		{
			st << calcNextBookStoreUpdate(curtime);
		}

		for(int i = 0; i < 6; ++ i)
		{
			st << _playerData.bookStore[i] << getBookPriceById(_playerData.bookStore[i]);
		}
		st << Stream::eos;
		send(st);
	}

	UInt16 Player::calcNextBookStoreUpdate(UInt32 curtime)
	{
		if(_nextBookStoreUpdate > curtime)
			return static_cast<UInt16>(_nextBookStoreUpdate - curtime);
		return 0;
	}

	UInt32 Player::purchaseBook( UInt8 idx )
	{
		if(idx > 5 || _playerData.bookStore[idx] == 0)
			return 0;
		UInt16 iid = _playerData.bookStore[idx];
		UInt32 price = getBookPriceById(iid);
		if(_playerData.tael < price)
		{
			sendMsgCode(0, 1006);
			return 0;
		}
		if(!m_Package->AddItem(iid, 1))
		{
			sendMsgCode(2, 1010);
			return 0;
		}
		_playerData.bookStore[idx] = 0;
		writeBookStoreIds();
		ConsumeInfo ci(PurchaseBook,0,0);
		useTael(price,&ci);
		return iid;
	}

	void Player::updateNextBookStoreUpdate(UInt32 curtime)
	{
		_nextBookStoreUpdate = (curtime + _bookStoreInterval) / _bookStoreInterval * _bookStoreInterval;
	}

	UInt8 Player::unLockSecondPWD(std::string pwd)
	{
		UInt32 now = TimeUtil::Now();
		if(getBuffLeft(PLAYER_BUFF_PWDLOCK, now) != 0)
			return 0xFF;
		else if(_pwdInfo.errCount == 0xFF)
			_pwdInfo.errCount = 0;
		if(_pwdInfo.isLocked == 0)
			return 0;
		if(_pwdInfo.secondPWD == pwd)
		{
			_pwdInfo.errCount = 0;
			_pwdInfo.isLocked = 0;
			return 0;
		}
		_pwdInfo.errCount ++;
		if(_pwdInfo.errCount >= 5)
		{
			setBuffData(PLAYER_BUFF_PWDLOCK, now + 600);
			_pwdInfo.errCount = 0xFF;
		}
		return _pwdInfo.errCount;
	}

	void Player::lockSecondPWD()
	{
		if(_pwdInfo.secondPWD.empty())
			return;
		_pwdInfo.isLocked = 1;
	}

	void Player::setSecondPWDDB(std::string pwd, std::string question, std::string answer)
	{
		_pwdInfo.secondPWD = pwd;
		_pwdInfo.questionForPWD = question;
		_pwdInfo.answerForPWD = answer;
	}

	UInt8 Player::activeSecondPWD(std::string pwd, std::string question, std::string answer)
	{
		if(pwd.length() != 6)
			return 1;
		if(question.length() > 30 || answer.length() > 30)
			return 1;
		if(!_pwdInfo.secondPWD.empty())
			return 1;
		_pwdInfo.secondPWD = pwd;
		_pwdInfo.questionForPWD = question;
		_pwdInfo.answerForPWD = answer;
		_pwdInfo.isLocked = 1;
		char questionTmp[256];
		char answerTmp[256];
		mysql_escape_string(questionTmp, _pwdInfo.questionForPWD.c_str(), _pwdInfo.questionForPWD.length());
		mysql_escape_string(answerTmp, _pwdInfo.answerForPWD.c_str(), _pwdInfo.answerForPWD.length());
		DB().PushUpdateData("INSERT INTO `pass_word` VALUES(%"I64_FMT"u, '%s', '%s', '%s')", _id, _pwdInfo.secondPWD.c_str(), questionTmp, answerTmp);
		Stream st;
		makeSenconPWDInfo(st);
		send(st);
		return 0;
	}

	UInt8 Player::deactiveSecondPWD(std::string answer)
	{
		if(answer != _pwdInfo.answerForPWD)
			return 1;
		_pwdInfo.errCount = 0;
		_pwdInfo.secondPWD.clear();
		_pwdInfo.answerForPWD.clear();
		_pwdInfo.questionForPWD.clear();
		DB().PushUpdateData("DELETE FROM `pass_word` WHERE `playerId` = %"I64_FMT"u", _id);
		Stream st;
		makeSenconPWDInfo(st);
		send(st);
		return 0;
	}

	UInt8 Player::forgetSecondPWD(std::string pwd, std::string answer)
	{
		if(answer != _pwdInfo.answerForPWD)
			return 1;
		_pwdInfo.errCount = 0;
		_pwdInfo.secondPWD = pwd;
		DB().PushUpdateData("UPDATE `pass_word` SET `password` = '%s' WHERE `playerId` =  %"I64_FMT"u", _pwdInfo.secondPWD.c_str(), _id);
		return 0;
	}

	UInt8 Player::changeSecondPWD(std::string oldPWD, std::string newPWD)
	{
		if(oldPWD != _pwdInfo.secondPWD)
			return 1;
		if(_pwdInfo.secondPWD == newPWD)
			return 0;
		_pwdInfo.secondPWD = newPWD;
		DB().PushUpdateData("UPDATE `pass_word` SET `password` = '%s' WHERE `playerId` =  %"I64_FMT"u", _pwdInfo.secondPWD.c_str(), _id);
		return 0;
	}

	void Player::makeSenconPWDInfo(Stream& st)
	{
		st.init(0xCB);
		if(!_pwdInfo.secondPWD.empty())
			st << static_cast<UInt8>(0) << _pwdInfo.questionForPWD;
		else
			st << static_cast<UInt8>(1);
		st << Stream::eos;
	}

	bool Player::hasChecked()
	{
		if(!_pwdInfo.secondPWD.empty() && _pwdInfo.isLocked != 0)
		{
			if(getBuffLeft(PLAYER_BUFF_PWDLOCK, TimeUtil::Now()) != 0)
			{
				sendMsgCode(0, 2003);
				return false;
			}
			Stream st(0xCC);
			st << Stream::eos;
			send(st);
			return false;
		}
		return true;
	}


	void Player::autoCB( bool add /*= true*/ )
	{
		if(add)
		{
			globalCountryBattle.addAutoCB(this);
			addStatus(AutoCB);
		}
		else
		{
			removeStatus(AutoCB);
			globalCountryBattle.delAutoCB(this);
		}
	}

    void Player::payPractice(UInt8 place, UInt16 slot, UInt8 type, UInt8 priceType, UInt8 time, UInt8 prot)
    {
        practicePlace.pay(this, place, slot, type, priceType, time, prot);
    }

    void Player::addPracticeFighter(UInt32* fighters, size_t size)
    {
        practicePlace.sitdown(this, fighters, size);
    }

    float Player::getPracticeBufFactor()
    {
        if(getBuffData(PLAYER_BUFF_PRACTICE1, TimeUtil::Now()))
        {
            return 0.2;
        }
        if(getBuffData(PLAYER_BUFF_PRACTICE2, TimeUtil::Now()))
        {
            return 0.5;
        }
        return 0.0;
    }

}


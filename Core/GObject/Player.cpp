#include "Config.h"
#include "Server/WorldServer.h"
#include "MsgID.h"
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
#include "GData/Money.h"
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
#include <mysql.h>
#include "GData/Formation.h"
#include "GData/Money.h"
#include "Script/BattleFormula.h"
#include "Copy.h"
#include "FrontMap.h"
#include "HeroIsland.h"
#include "GObject/AthleticsRank.h"
#include "DCLogger.h"

#include <cmath>

#define NTD_ONLINE_TIME (4*60*60)

namespace GObject
{
    UInt32 Player::_recruit_cost = 20;
    UInt32 Player::_tavernBlueCount = 24;
    UInt32 Player::_tavernPurpleCount = 89;
    UInt32 Player::_tavernOrangeCount = 200;
	UInt32 Player::_tavernInterval = 1 * 3600, Player::_tavernRate = 100;
	UInt32 Player::_bookStoreInterval = 2 * 3600, Player::_bookStoreRate = 100;
    UInt8 Player::_yaMenActiveCount = 0;
    UInt8 Player::_shiMenActiveCount = 0;
	const UInt8 MaxICCount[] = {8, 16, 16, 16, 24, 24, 24, 24, 24, 24, 24};
	const UInt16 MAX_EXTEND_TIME	= 10;
	const UInt16 EACH_EXTEND_NUM	= 50;
	GlobalPlayers globalPlayers;
	GlobalPlayers newPlayers;
	GlobalNamedPlayers globalNamedPlayers;
	ChallengeCheck challengeCheck;
    GlobalLevelsPlayers globalLevelsPlayers;
    static TripodData nulltd;

	inline UInt8 getMaxIcCount(UInt8 vipLevel)
	{
		UInt8 maxCount = MaxICCount[vipLevel];
		//if(World::_wday == 6)
	    //    maxCount += 8; 
		return maxCount;
	}

    inline UInt8 getShiMenMax()
    {
        return SHIMEN_TASK_MAXCOUNT + Player::_shiMenActiveCount;
    }

    inline UInt8 getYaMenMax()
    {
        return YAMEN_TASK_MAXCOUNT + Player::_yaMenActiveCount;
    }

	float EventAutoBattle::calcExpEach(UInt32 now)
	{
#if 0
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
#else
        return 4.0f * _npcGroup->getExp();
#endif
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
			exp *= 1.2f;
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
		Stream st(REP::TASK_RESPONSE_HOOK);
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
				DB3().PushUpdateData("REPLACE INTO `auto_battle`(`playerId`, `npcId`, `count`, `interval`) VALUES(%"I64_FMT"u, %u, %u, %u)", m_Player->getId(), _npcGroup->getId(), count, m_Timer.GetInterval());
			else
				DB3().PushUpdateData("UPDATE `auto_battle` SET `count` = %u WHERE `playerId` = %"I64_FMT"u", count, m_Player->getId());
		}
		else
			DB3().PushUpdateData("DELETE FROM `auto_battle` WHERE `playerId` = %"I64_FMT"u", m_Player->getId());
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
		_end -= times * 3600;
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
		UInt32 exp = static_cast<UInt32>(levExp[_fighter->getLevel()] * data->factor * 60);
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

        PracticeFighterExp pfexp;
        memset(&pfexp, 0, sizeof(pfexp));

        //data->lock.lock();
        Fighter* fgt = 0;
        UInt8 n = 0;
        for (auto i = data->fighters.begin(), e = data->fighters.end(); i != e; ++i)
        {
            fgt = m_Player->findFighter(*i);
            if (fgt)
            {
                //fgt->addPExp(fgt->getPracticeInc() * 10); 
                if(n < sizeof(pfexp.fids)/sizeof(UInt32))
                {
                    pfexp.fids[n] = *i;
                    pfexp.counts[n] = 10;
                    ++ n;
                }
            }
        }
        //data->lock.unlock();
        GameMsgHdr hdr1(0x320, m_Player->getThreadId(), m_Player, sizeof(PracticeFighterExp));
        GLOBAL().PushMsg(hdr1, &pfexp);

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

	bool EventPlayerPractice::instantComplete()
	{
        PracticeData* data = practicePlace.getPracticeData(m_Player->getId());
        if (!data) {
			PopTimerEvent(m_Player, EVENT_PLAYERPRACTICING, m_Player->getId());
			return false;
        }

        //data->lock.lock();
        //data->lock.unlock();
        if(m_Player->getPIcCount() <= 0)
            return false;

		if (data->checktime > 0)
		{
			UInt32 count = 60;
			if(count > data->checktime)
			{
				count = data->checktime;
			}

            UInt32 goldUse = GData::moneyNeed[GData::INSTANTPRACTICE].gold;
            PracticeFighterExp pfexp;
            memset(&pfexp, 0, sizeof(pfexp));
            pfexp.goldUse = goldUse;

            //data->lock.lock();
            Fighter* fgt = 0;
            UInt8 n = 0;
            for (auto i = data->fighters.begin(), e = data->fighters.end(); i != e; ++i)
            {
                fgt = m_Player->findFighter(*i);
                if (fgt)
                {
                    //fgt->addPExp(fgt->getPracticeInc() * 10); 
                    if(n < sizeof(pfexp.fids)/sizeof(UInt32))
                    {
                        pfexp.fids[n] = *i;
                        pfexp.counts[n] = count;
                        ++ n;
                    }
                }
            }
            GameMsgHdr hdr1(0x320, m_Player->getThreadId(), m_Player, sizeof(PracticeFighterExp));
            GLOBAL().PushMsg(hdr1, &pfexp);

            m_Player->incPIcCount();
            data->checktime -= count;
            if ((int)data->checktime < 0)
                data->checktime = 0;
            if(data->checktime == 0)
            {
                DB().PushUpdateData("UPDATE `practice_data` SET `checktime` = %u, `place` = %u, `slot` = %u, winnerid = %u, fighters = '' WHERE `id` = %"I64_FMT"u", data->checktime, PPLACE_MAX, 0, 0, m_Player->getId());
                practicePlace.stop(m_Player);
                PopTimerEvent(m_Player, EVENT_PLAYERPRACTICING, m_Player->getId());
            }
            else
            {
                DB().PushUpdateData("UPDATE `practice_data` SET `checktime` = %u WHERE `id` = %"I64_FMT"u",
                        data->checktime, m_Player->getId());
            }

            {
                Stream st1(REP::PRACTICE_OCCUPY);
                st1 << static_cast<UInt8>(0);
                st1 << data->checktime * 60;
                st1 << data->prot;
                st1 << m_Player->getPIcCount();
                st1 << static_cast<UInt8>(m_Player->getPracticePlace()-1);

                UInt8 size = data->fighters.size();
                st1 << size;

                for (std::list<UInt32>::iterator i = data->fighters.begin(), e = data->fighters.end(); i != e; ++i)
                    st1 << *i;

                st1 << Stream::eos;
                m_Player->send(st1);
            }
        }

        return true;
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
        TripodData& data = m_Player->getTripodData();
        if(m_Player->getVipLevel() > 4)
            data.soul += POINT_PERMIN * 2;
        else
            data.soul += POINT_PERMIN;

        if (data.soul > MAX_TRIPOD_SOUL)
            data.soul = MAX_TRIPOD_SOUL;

        if (!leftCount || data.soul >= MAX_TRIPOD_SOUL - POINT_PERMIN/2) {
            PopTimerEvent(m_Player, EVENT_PLAYERPRTRIPOD, m_Player->getId());
            data.awdst = 1;
            data.soul = MAX_TRIPOD_SOUL;
            DB().PushUpdateData("UPDATE `tripod` SET `awdst` = %u WHERE `id` = %"I64_FMT"u", data.awdst, m_Player->getId());
            return;
        }

        if (!(leftCount % 3))
            DB().PushUpdateData("UPDATE `tripod` SET `soul` = %u WHERE `id` = %"I64_FMT"u", data.soul, m_Player->getId());
    }

    bool EventAutoCopy::Equal(UInt32 id, size_t playerid) const
    {
		return 	id == GetID() && playerid == m_Player->getId();
    }

    void EventAutoCopy::Process(UInt32 leftCount)
    {
		GameMsgHdr hdr(0x276, m_Player->getThreadId(), m_Player, sizeof(id));
		GLOBAL().PushMsg(hdr, &id);
        if (!leftCount)
			PopTimerEvent(m_Player, EVENT_AUTOCOPY, m_Player->getId());
    }

    bool EventAutoCopy::Accelerate(UInt32 times)
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

    bool EventPlayerTimeTick::Equal(UInt32 id, size_t playerid) const
    {
		return 	id == GetID() && playerid == m_Player->getId();
    }

    void EventPlayerTimeTick::Process(UInt32 leftCount)
    {
        GameMsgHdr hdr(0x277, m_Player->getThreadId(), m_Player, sizeof(type));
        GLOBAL().PushMsg(hdr, &type);

        if (!leftCount)
			PopTimerEvent(m_Player, EVENT_TIMETICK, m_Player->getId());
    }

    bool EventPlayerTimeTick::Accelerate(UInt32 times)
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

	void Lineup::updateId()
	{
		if(fighter != NULL) fid = fighter->getId(); else fid = 0;
	}

	Player::Player( UInt64 id ): GObjectBaseT<Player, UInt64>(id),
		_isOnline(false), _isHoding(false), _holdGold(0), _threadId(0xFF), _session(-1),
		_availInit(false), _vipLevel(0), _clan(NULL), _clanBattle(NULL), _flag(0), _gflag(0), _onlineDuration(0), _offlineTime(0),
		_nextTavernUpdate(0), _nextBookStoreUpdate(0), _bossLevel(21), _ng(NULL), _lastNg(NULL),
		_lastDungeon(0), _exchangeTicketCount(0), _praplace(0), m_autoCopyFailed(false),
        _justice_roar(0), _worldBossHp(0), m_autoCopyComplete(0), hispot(0xFF), hitype(0), m_ulog(NULL), m_hasTripod(false)
	{
		memset(_buffData, 0, sizeof(UInt32) * PLAYER_BUFF_COUNT);
		m_Package = new Package(this);
		m_TaskMgr = new TaskMgr(this);
		m_Trade = new Trade(this);
		m_Sale = new Sale(this);
		m_MailBox = new MailBox(this);
		m_Athletics = new Athletics(this);
		m_AttainMgr = new AttainMgr(this);
        m_pVars = new VarSystem(id);
        _recruit_cost = GData::moneyNeed[GData::RECRUIT].gold;
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

	void Player::setVipL(UInt8 lvl)
	{
#define VIP_OPEN_MAX 10
        if (!lvl || lvl > VIP_OPEN_MAX)
            return;
        if (lvl == 0)
            _playerData.totalRecharge = 87;
        else if (lvl == 1)
            _playerData.totalRecharge = 88;
        else if (lvl == 2)
            _playerData.totalRecharge = 888;
        else if (lvl == 3)
            _playerData.totalRecharge = 1888;
        else if (lvl == 4)
            _playerData.totalRecharge = 5888;
        else if (lvl == 5)
            _playerData.totalRecharge = 18888;
        else if (lvl == 6)
            _playerData.totalRecharge = 58888;
        else if (lvl == 7)
            _playerData.totalRecharge = 88888;
        else if (lvl == 8)
            _playerData.totalRecharge = 188888;
        else if (lvl == 9)
            _playerData.totalRecharge = 288888;
        else if (lvl == 10)
            _playerData.totalRecharge = 588888;
		DB1().PushUpdateData("UPDATE `player` SET `totalRecharge` = %u WHERE `id` = %"I64_FMT"u", _playerData.totalRecharge, getId());
        recalcVipLevel();
		sendModification(7, _playerData.totalRecharge);
    }

	UInt32 Player::calcVipLevel()
	{
#define VIP_OPEN_MAX 10
		UInt32 totalRecharge = _playerData.totalRecharge;
		UInt32 vipl;
		if(totalRecharge < 88)
			vipl = 0;
		else if(totalRecharge < 888)
			vipl = 1;
		else if(totalRecharge < 1888)
			vipl = 2;
		else if(totalRecharge < 5888)
			vipl = 3;
		else if(totalRecharge < 18888)
			vipl = 4;
		else if (totalRecharge < 58888)
			vipl = 5;
		else if(totalRecharge < 88888)
			vipl = 6;
		else if(totalRecharge < 188888)
			vipl = 7;
		else if(totalRecharge < 288888)
			vipl = 8;
		else if(totalRecharge < 588888)
			vipl = 9;
		else
			vipl = 10;
		if(vipl > VIP_OPEN_MAX)
			return VIP_OPEN_MAX;
		return vipl;
	}

	UInt32 Player::calcYDVipLevel(UInt32 total)
	{
#define YDVIP_OPEN_MAX 6 
		UInt32 totalRecharge = total;
		UInt32 vipl;
		if(totalRecharge < 100)
			vipl = 0;
		else if(totalRecharge < 300)
			vipl = 1;
		else if(totalRecharge < 1000)
			vipl = 2;
		else if(totalRecharge < 3000)
			vipl = 3;
		else if(totalRecharge < 10000)
			vipl = 4;
		else if (totalRecharge < 30000)
			vipl = 5;
		else
			vipl = 6;
		if(vipl > YDVIP_OPEN_MAX)
			return YDVIP_OPEN_MAX;
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

        SAFE_DELETE(m_pVars);
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
            _offlineTime = dur;
			if(dur >= 5 * 60 * 60) {
				_onlineDuration = 0;
                _offlineTime = 0;
            }
		}

        if (World::_halloween)
            sendHalloweenOnlineAward(curtime);
        else
            setBuffData(PLAYER_BUFF_ONLINE, 0);

		_playerData.lastOnline = curtime;
		DB1().PushUpdateData("UPDATE `player` SET `lastOnline` = %u WHERE `id` = %"I64_FMT"u", curtime, getId());

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

        if (World::_nationalDay) // XXX: 国庆节活动
        {
            UInt32 online = getBuffData(PLAYER_BUFF_ONLINE);
            if (online != static_cast<UInt32>(-1))
            {
                if (online < NTD_ONLINE_TIME)
                {
                    EventPlayerTimeTick* event = new(std::nothrow) EventPlayerTimeTick(this, NTD_ONLINE_TIME-online, 1, 0);
                    if (event) PushTimerEvent(event);
                }
                else
                    sendNationalDayOnlineAward();
            }
        }
#if 0 // XXX: 所有人都已经没有这个BUFF了
        else
            setBuffData(PLAYER_BUFF_ONLINE, 0, true);
#endif

        sendLevelPack(GetLev());

        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "%"I64_FMT"u", _id);
        m_ulog = _analyzer.GetInstance(buf);
        if (m_ulog)
        {
            TcpConnection conn = NETWORK()->GetConn(_session);
            if (conn)
            {
                Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
                struct in_addr inaddr = inet_makeaddr(cl->GetClientIP(), 0);
                m_ulog->SetUserIP(inet_ntoa(inaddr));
            }
        }

        udpLog("", "", "", "", "", "", "login");
        dclogger.login(this);
	}

    void Player::udpLog(const char* str1, const char* str2, const char* str3, const char* str4,
                const char* str5, const char* str6, const char* type, UInt32 count)
    {
        if (m_ulog)
        {
            UInt8 platform = atoi(getDomain().c_str());
            char buf[1024] = {0};
            char* pbuf = &buf[0];
            pbuf += snprintf(pbuf, sizeof(buf), "%u_%u_%"I64_FMT"u|%s|||||%u||||||||||%u|",
                    cfg.serverNum, cfg.tcpPort, getId(), getOpenId().c_str(), GetLev(), cfg.serverNum);

            m_ulog->SetUserMsg(buf);
            m_ulog->LogMsg(str1, str2, str3, str4, str5, str6, type, count, 0);

            if (platform)
            {
                snprintf(pbuf, pbuf - buf, "|%u|", platform);
                m_ulog->SetUserMsg(buf);
                m_ulog->LogMsg(str1, str2, str3, str4, str5, str6, type, count, platform);
            }
            TRACE_LOG("%s", buf);
        }
    }

    void Player::sendHalloweenOnlineAward(UInt32 now, bool _online)
    {
        _online = false; // XXX: fuck
        UInt32 online = getBuffData(PLAYER_BUFF_ONLINE);
        if (online == static_cast<UInt32>(-1))
            return;

        UInt8 oday = TimeUtil::Day(_playerData.lastOnline);
        UInt8 nday = TimeUtil::Day(now);
        if (oday != nday || _online)
        {
            if (nday - oday > 1) // XXX: 隔天
            {
                setBuffData(PLAYER_BUFF_ONLINE, 1);
            }
            else
            {
                ++online;
                setBuffData(PLAYER_BUFF_ONLINE, online);
            }
            if (_online)
            {
                _playerData.lastOnline = now;
                DB1().PushUpdateData("UPDATE `player` SET `lastOnline` = %u WHERE `id` = %"I64_FMT"u", now, getId());
            }
        }
        else
        {
            if (!online)
            {
                ++online;
                setBuffData(PLAYER_BUFF_ONLINE, online);
            }
            else
                return;
        }

        SYSMSG(title, 2122);
        SYSMSG(content, 2123);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem mitem[1] = {{GObject::MailPackage::Coupon,50}};
            mailPackageManager.push(mail->id, mitem, 1, true);
            std::string strItems;
            for (int i = 0; i < 1; ++i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
        }

        if (online == 3)
        {
            SYSMSG(title, 2124);
            SYSMSG(content, 2125);
            Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[2] = {{GObject::MailPackage::Coupon,100}, {1750,1}};
                mailPackageManager.push(mail->id, mitem, 2, true);
                std::string strItems;
                for (int i = 0; i < 2; ++i)
                {
                    strItems += Itoa(mitem[i].id);
                    strItems += ",";
                    strItems += Itoa(mitem[i].count);
                    strItems += "|";
                }
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
            }
        }

        if (online >= 4)
            setBuffData(PLAYER_BUFF_ONLINE, static_cast<UInt32>(-1));
    }

    void Player::sendNationalDayOnlineAward()
    {
        if (getBuffData(PLAYER_BUFF_ONLINE) == static_cast<UInt32>(-1))
            return;

        SYSMSG(title, 2112);
        SYSMSG(content, 2113);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem mitem[2] = {{GObject::MailPackage::Coupon,100},{56,2}};
            mailPackageManager.push(mail->id, mitem, 2, true);
            std::string strItems;
            for (int i = 0; i < 2; ++i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
            setBuffData(PLAYER_BUFF_ONLINE, static_cast<UInt32>(-1), true);
        }
    }

    void Player::openLevelBox(UInt8 lvl, UInt8 cls)
    {
        static const UInt32 itemId[3][8] = {
            {2392,2393,2394,2395,2396,2397,2398,2399},
            {2384,2385,2386,2387,2388,2389,2390,2391},
            {2376,2377,2378,2379,2380,2381,2382,2383},
        };

        static const UInt32 gemId[3][8][6] = {
            {
                {5061,5081,5021,5031,5051,5041},
                {5071,5081,5021,5031,5051,5041},
                {5071,5091,5021,5031,5051,5041},
                {5071,5091,5021,5031,5051,5041},
                {5071,5131,5021,5031,5051,5041},
                {5071,5101,5021,5031,5051,5041},
                {5101,5131,5111,5021,5031,5051},
                {5061,5101,5131,5111,5021,5031},
            },
            {
                {5061,5081,5021,5031,5051,5041},
                {5071,5081,5021,5031,5051,5041},
                {5071,5091,5021,5031,5051,5041},
                {5071,5091,5021,5031,5051,5041},
                {5071,5131,5021,5031,5051,5041},
                {5071,5101,5021,5031,5051,5041},
                {5101,5131,5111,5021,5031,5051},
                {5061,5101,5131,5111,5021,5031},
            },
            {
                {5061,5081,5001,5031,5051,5011},
                {5071,5081,5001,5031,5051,5011},
                {5071,5091,5001,5031,5051,5011},
                {5071,5091,5001,5031,5051,5011},
                {5071,5131,5001,5031,5051,5011},
                {5071,5101,5001,5031,5051,5011},
                {5101,5131,5111,5001,5031,5051},
                {5061,5101,5131,5111,5001,5031},
            },
        };

        if (!cls || cls > 3)
            return;

        if (lvl == 30)
        {
            for (UInt8 i = 0; i < 8; ++i)
            {
                ItemEquip* ie = static_cast<ItemEquip*>(GetPackage()->AddEquip(itemId[cls-1][i], true, false));
                if (ie)
                {
                    ItemEquipData& ied = ie->getItemEquipData();
                    ied.sockets = 6;
                    for (UInt8 j = 0; j < 6; ++j)
                    {
                        ied.gems[j] = gemId[cls-1][i][j];
                        DB4().PushUpdateData("UPDATE `equipment` SET `enchant` = %u, `sockets` = %u, `socket1` = %u, `socket2` = %u, `socket3` = %u, `socket4` = %u, `socket5` = %u, `socket6` = %u, `attrType1` = %u, `attrValue1` = %d, `attrType2` = %u, `attrValue2` = %d, `attrType3` = %u, `attrValue3` = %d WHERE `id` = %u", ied.enchant, ied.sockets, ied.gems[0], ied.gems[1], ied.gems[2], ied.gems[3], ied.gems[4], ied.gems[5], ied.extraAttr2.type1, ied.extraAttr2.value1, ied.extraAttr2.type2, ied.extraAttr2.value2, ied.extraAttr2.type3, ied.extraAttr2.value3, ie->getId());
                    }
                    GetPackage()->SendSingleEquipData(ie);
                }
            }
        }
    }

    void Player::setVipAwardFlag(UInt8 type, UInt32 value)
    {
        switch (type)
        {
            case 2:
            case 1:
                _playerData.qqawardgot |= value;
                DB1().PushUpdateData("UPDATE `player` SET `qqawardgot` = %u WHERE `id` = %"I64_FMT"u",
                        _playerData.qqawardgot, getId());
            break;

            default:
            break;
        }
    }

    void Player::sendMailPack(UInt16 title, UInt16 content, lua_tinker::table items)
    {
        UInt32 size = items.size();
        if (!size || size > 100)
            return;

        SYSMSG(_title, title);
        SYSMSG(_content, content);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, _title, _content, 0xFFFE0000);
        if(mail)
        {
            std::string strItems;

            MailPackage::MailItem* mitem = new MailPackage::MailItem[size];
            for (UInt32 i = 0; i < size; ++i)
            {
                lua_tinker::table tmp = items.get<lua_tinker::table>(i);;
                mitem[i].id = tmp.get<UInt32>(0);
                mitem[i].count = tmp.get<UInt32>(1);

                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            mailPackageManager.push(mail->id, mitem, size, true);
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, _title, _content, strItems.c_str(), mail->recvTime);
            delete mitem;
        }
    }

    void Player::sendMailItem(UInt16 title, UInt16 content, MailPackage::MailItem* mitem, UInt16 size, bool bind)
    {
        if (!mitem || !size)
            return;

        SYSMSG(_title, title);
        SYSMSG(_content, content);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, _title, _content, 0xFFFE0000);
        if(mail)
        {
            std::string strItems;
            for (UInt32 i = 0; i < size; ++i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            mailPackageManager.push(mail->id, mitem, size, bind);
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, _title, _content, strItems.c_str(), mail->recvTime);
        }
    }

    void Player::sendLevelPack(UInt8 lvl)
    {
        if (lvl >= 30 && !(_playerData.qqawardgot & 0x10))
        {
            SYSMSG(title, 2120);
            SYSMSG(content, 2121);
            Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[1] = {{37,1}};
                mailPackageManager.push(mail->id, mitem, 1, true);

                std::string strItems;
                strItems += Itoa(mitem[0].id);
                strItems += ",";
                strItems += Itoa(mitem[0].count);
                strItems += "|";
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);

                _playerData.qqawardgot |= 0x10;
                DB1().PushUpdateData("UPDATE `player` SET `qqawardgot` = %u WHERE `id` = %"I64_FMT"u", _playerData.qqawardgot, getId());
            }
        }

        if (lvl >= 40 && !(_playerData.qqawardgot & 0x08))
        {
            SYSMSG(title, 2114);
            SYSMSG(content, 2115);
            Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[1] = {{36,1}};
                mailPackageManager.push(mail->id, mitem, 1, true);

                std::string strItems;
                strItems += Itoa(mitem[0].id);
                strItems += ",";
                strItems += Itoa(mitem[0].count);
                strItems += "|";
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);

                _playerData.qqawardgot |= 0x08;
                DB1().PushUpdateData("UPDATE `player` SET `qqawardgot` = %u WHERE `id` = %"I64_FMT"u", _playerData.qqawardgot, getId());
            }
        }
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
		DBLOG1().PushUpdateData("update login_states set logout_time=%u where server_id=%u and player_id=%"I64_FMT"u and login_time=%u", curtime, cfg.serverLogId, _id, _playerData.lastOnline);
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
		
		DBLOG1().PushUpdateData("update login_states set logout_time=%u where server_id=%u and player_id=%"I64_FMT"u and login_time=%u", curtime, cfg.serverLogId, _id, _playerData.lastOnline);
		DB1().PushUpdateData("UPDATE `player` SET `lastOnline` = %u, `nextReward` = '%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", curtime, _playerData.rewardStep, _playerData.nextRewardItem, _playerData.nextRewardCount, _playerData.nextRewardTime, _id);
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

        if (World::_nationalDay)
        {
            UInt32 online = getBuffData(PLAYER_BUFF_ONLINE);
            if (online != static_cast<UInt32>(-1))
            {
                if (online + curtime - _playerData.lastOnline >= NTD_ONLINE_TIME)
                    sendNationalDayOnlineAward();
                else
                    setBuffData(PLAYER_BUFF_ONLINE, online + curtime - _playerData.lastOnline);
            }
        }

        dclogger.logout(this);
        heroIsland.playerOffline(this);
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
        if(!_lastAthAward.empty())
        {
            std::vector<LastAthAward>::iterator it;
            for(it = _lastAthAward.begin(); it != _lastAthAward.end(); ++ it)
            {
                if(it->itemId && it->itemCount)
                {
                    SYSMSG_SENDV(102, this, it->itemId, it->itemCount);
                    SYSMSG_SENDV(1002, this, it->itemId, it->itemCount);
                }
                if(it->prestige)
                {
                    SYSMSG_SENDV(185, this, it->prestige);
                    SYSMSG_SENDV(1090, this, it->prestige);
                }
            }
            _lastAthAward.clear();
        }
		if(update)
		{
			DB1().PushUpdateDataL("UPDATE `player` SET `lastExp` = 0, `lastResource` = 0 WHERE `id` = %"I64_FMT"u", _id);
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
            {
                if (data - now >= 30)
                    _playerData.battlecdtm = now + (data - now) / 6;
                else
                    _playerData.battlecdtm = now + (data - now);
                if (data - now < 2)
                    _playerData.battlecdtm = now + 2;
            }
		}
		if(id >= PLAYER_BUFF_COUNT || _buffData[id] == data)
			return;
		_buffData[id] = data;
		if(writedb || (id >= PLAYER_BUFF_HIFIGHT && id <= PLAYER_BUFF_HIESCAPE))
			sendModification(0x40 + id, data, writedb);
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
		if(idx != PLAYER_BUFF_AUTOHEAL &&
                idx != PLAYER_BUFF_HOLY && 
                idx != PLAYER_BUFF_AUTOCOPY && 
                idx != PLAYER_BUFF_WBOSS &&
                idx != PLAYER_BUFF_YDOTR &&
                idx != PLAYER_BUFF_ONLINE &&
                idx != PLAYER_BUFF_AMARTIAL_WIN &&
                _buffData[idx] > 0 && _buffData[idx] <= tm)
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

    UInt32 Player::GetVar(UInt32 id)
    {
        return m_pVars->GetVar(id);
    }

    void Player::LoadVar(UInt32 id, UInt32 val, UInt32 overTime)
    {
        m_pVars->LoadVar(id, val, overTime);
    }

    void Player::SetVar(UInt32 id, UInt32 val)
    {
        m_pVars->SetVar(id,val);
    }

    void Player::AddVar(UInt32 id, UInt32 val)
    {
        m_pVars->AddVar(id,val);
    }

    void Player::SetVarOffset(UInt32 offset)
    {
        m_pVars->SetOffset(offset);
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
		DB1().PushUpdateData("UPDATE `player` SET `lineup` = '%u,%u|%u,%u|%u,%u|%u,%u|%u,%u' WHERE id = %" I64_FMT "u",
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

    void Player::upInitCitta(Fighter* fgt, bool writedb)
    {
        static UInt16 cittas[] = {301, 401, 701};
        UInt16 citta = cittas[fgt->getClass()-1];
        if (fgt->hasCitta(citta) < 0) {
            if (fgt->addNewCitta(citta, writedb, true)) {
                if (fgt->upCitta(citta, 0, writedb)) {
                }
            }
        }
    }

	void Player::addFighter( Fighter * fgt, bool writedb, bool load )
	{
		UInt32 id = fgt->getId();
		if(id < 10)
			_fighters.insert(_fighters.begin(), std::make_pair(fgt->getId(), fgt));
		else
			_fighters[fgt->getId()] = fgt;

		if(writedb)
		{
			UInt32 p = static_cast<UInt32>((fgt->getPotential()+0.005) * 100);
			UInt32 c = static_cast<UInt32>((fgt->getCapacity()+0.05) * 100);
			DB2().PushUpdateData("REPLACE INTO `fighter` (`id`, `playerId`, `potential`, `capacity`, `level`, `experience`)\
                    VALUES(%u, %"I64_FMT"u, %u.%02u, %u.%02u, %u, %u)",
                    id, getId(), p / 100, p % 100, c / 100, c % 100, fgt->getLevel(), fgt->getExp());
		}

        if (!load || !fgt->getCittasNum())
            upInitCitta(fgt, true);
	}

    bool Player::addFighterFromItem(UInt32 itemid, UInt32 price)
    {
        if(isFighterFull())
        {
            sendMsgCode(0, 1200);
            return 0;
        }

        UInt32 id = 0;
        if (itemid == 73)
            id = 113;
        else if (itemid == 74)
            id = 18;
        else if (itemid == 75)
            id = 114;
        else if (itemid == 76)
            id = 28;
        else if (itemid == 77)
            id = 115;

        if (id)
        {
            if (hasFighter(id))
            {
                sendMsgCode(1, 1017);
                return false;
            }

            Fighter * fgt = globalFighters[id];
            if(fgt == NULL)
                return false;
            Fighter * fgt2 = fgt->clone(this);
            addFighter(fgt2, true);
            notifyAddFighter(fgt2);
            autoLineup(fgt2);
            return true;
        }

        return false;
    }

	void Player::notifyAddFighter( Fighter * fgt )
	{
		if(!_isOnline)
			return;
		Stream st(REP::FIGHTER_ACCEPT);
		st << static_cast<UInt16>(fgt->getId());
        st << fgt->getPotential() << fgt->getCapacity() << fgt->getLevel() << fgt->getExp() << fgt->getDefaultSkillAndLevel() << fgt->getDefaultCittaAndLevel()
            << fgt->getSoul() << fgt->getMaxSoul();
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
				m_Package->EquipTo(0, fgt, z+0x20, equip, true);
            for(UInt8 t = 0; t < 3; ++ t)
				m_Package->EquipTo(0, fgt, t+0x50, equip, true);

			_fighters.erase(it);
			DB2().PushUpdateData("DELETE FROM `fighter` WHERE `id` = %u AND `playerId` = %"I64_FMT"u", id, getId());
			if(r)
				sendMsgCode(0, 1201);
			SYSMSG_SENDV(111, this, fgt->getColor(), fgt->getName().c_str());
			SYSMSG_SENDV(1011, this, fgt->getColor(), fgt->getName().c_str());

            UInt32 fgtid = fgt->getId();
            GameMsgHdr hdr2(0x1A6, WORKER_THREAD_WORLD, this, sizeof(fgtid));
            GLOBAL().PushMsg(hdr2, &fgtid);

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

    UInt8 Player::allHpP()
    {
        UInt32 total = 0;
        UInt32 totalmax = 0;
        for(int i = 0; i < 5; ++ i)
        {
            GObject::Lineup& pd = _playerData.lineup[i];
            if(pd.fighter != NULL)
            {
                if (pd.fighter->getCurrentHP())
                    total += pd.fighter->getCurrentHP();
                else
                    total += pd.fighter->getMaxHP();
                totalmax += pd.fighter->getMaxHP();
            }
        }
        if (!totalmax)
            return 0;
        if (!total)
            return 100;
        UInt8 p = (((float)total)/totalmax) * 100;
        if (p > 100)
            p = 100;
        return p;
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

    bool Player::checkFormation_ID(UInt16 f)
    {
        bool find = false;
        int cnt = _playerData.formations.size();
        for( int idx = 0; idx < cnt; ++ idx )
        {
            if(FORMATION_ID(_playerData.formations[idx]) == FORMATION_ID(f))
            {
                find = true;
                break;
            }
        }

        if(!find)
            return false;

        return true;
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

		if(_playerData.formation == f)
			return true;
		_playerData.formation = f;
		DB1().PushUpdateData("UPDATE `player` SET `formation` = %u WHERE id = %" I64_FMT "u", f, _id);

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
		st.init(REP::USER_INFO);
		UInt8 status = static_cast<UInt8>(_playerData.status);
		if(cfg.limitLuckyDraw == 2 || (cfg.limitLuckyDraw == 1 && _vipLevel < 2))
			status |= 0x80;
		st << _playerData.country << _playerData.gold << _playerData.coupon << _playerData.tael << _playerData.coin << getClanName()
			<< status << _playerData.title << static_cast<UInt8>(0) << _playerData.totalRecharge << static_cast<UInt8>(_playerData.qqvipl%10) << _playerData.qqvipyear << _playerData.achievement << _playerData.prestige << _playerData.packSize << _playerData.newGuild <<  _playerData.mounts << c;
		for(UInt8 i = 0; i < c; ++ i)
		{
			st << buffid[i] << buffleft[i];
		}
		st << Stream::eos;
	}

	void Player::makeFormationInfo( Stream& st )
	{
		st.init(REP::RANK_SETTING);
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

	void Player::makeFighterList( Stream& st )
	{
		size_t c = _fighters.size();
		st.init(REP::FIGHTER_INFO);
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

	bool Player::setNewGuildTaskStep(UInt32 step)
	{
#if 0
		if (step <= 0 || step > NEWGUILDSTEP_MAX)
			return false;
		UInt64 stepVal = 1ull << (step - 1);
 		if ((stepVal & _playerData.newGuild) == 0)
		{
			_playerData.newGuild |= stepVal;
			DB1().PushUpdateData("UPDATE `player` SET `newGuild` = %"I64_FMT"u WHERE `id` = %"I64_FMT"u", _playerData.newGuild, _id);
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
		Stream st(REP::USER_INFO_CHANGE);
		st << static_cast<UInt8>(0x10) << step << Stream::eos;
		send(st);
#else
        PLAYER_DATA(this, newGuild) = step;
        DB1().PushUpdateData("UPDATE `player` SET `newGuild` = %"I64_FMT"u WHERE `id` = %"I64_FMT"u", _playerData.newGuild, getId());
#endif
		return true;
	}

    void Player::sendNewGuild()
    {
        Stream st(REP::GUIDE_RESPONSE_STEP);
        st << static_cast<UInt16>(_playerData.newGuild);
        st << Stream::eos;
        send(st);
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

	bool Player::challenge( Player * other, UInt32 * rid, int * turns, bool applyhp, UInt32 sysRegen, bool noreghp, UInt32 scene, bool report )
	{
		checkLastBattled();
		other->checkLastBattled();
		Battle::BattleSimulator bsim(applyhp ? _playerData.location : scene, this, other);
		PutFighters( bsim, 0 );
		other->PutFighters( bsim, 1 );
		bsim.start();
		bool res = bsim.getWinner() == 1;

		Stream st(REP::ATTACK_NPC);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		send(st);
		st.data<UInt8>(4) = static_cast<UInt8>(res ? 0 : 1);

        if (report)
            other->send(st);
        else
        {
            if (res)
            {
                SYSMSG_SENDV(2141, other, getCountry(), getName().c_str());
            }
            else
            {
                SYSMSG_SENDV(2140, other, getCountry(), getName().c_str());
            }
        }

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
		else if(sysRegen > 0 && !noreghp)
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
        else if (noreghp)
        {
            bsim.applyFighterHP(0, this, false, sysRegen);
            bsim.applyFighterHP(1, other, false, sysRegen);
        }

		if(res)
			other->checkDeath();
		else
			checkDeath();

		return res;
	}

	bool Player::attackNpc( UInt32 npcId, UInt32 turns, bool regen, bool needtype )
	{
		UInt32 now = TimeUtil::Now();
		UInt32 buffLeft = getBuffData(PLAYER_BUFF_ATTACKING, now);
		if(buffLeft > now)
		{
			sendMsgCode(0, 1407, buffLeft - now);
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

        if (!ng)
            return false;

        if (cfg.GMCheck && needtype && ng->getType()) // XXX: 必须是野外怪
            return false;

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

		Stream st(REP::ATTACK_NPC);

		bool res = bsim.getWinner() == 1;
		if(res)
		{
			st << static_cast<UInt16>(0x0101);
			_lastNg = ng;

            if (ng->getLevel() <= GetLev() || (ng->getLevel() > GetLev() && (ng->getLevel() - GetLev()) < 10))
            {
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

		setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns());

		return res;
	}

    void Player::autoCopyFailed(UInt8 id)
    {
        GObject::playerCopy.failed(this, id);
        setCopyFailed();
    }

    bool Player::attackRareAnimal(UInt32 id)
    {
        return attackCopyNpc(id, 1/*XXX:使用这个背景*/, 5, 1, 1, false, NULL, false);
    }

	bool Player::attackCopyNpc( UInt32 npcId, UInt8 type, UInt8 copyId,
            UInt8 expfactor, UInt8 lootlvl, bool ato, std::vector<UInt16>* loot, bool applayhp )
	{
		UInt32 now = TimeUtil::Now();
		UInt32 buffLeft = getBuffData(PLAYER_BUFF_ATTACKING, now);
		if(cfg.GMCheck && buffLeft > now && !ato)
		{
			sendMsgCode(0, 1407, buffLeft - now);
			return false;
		}
		checkLastBattled();
		GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
		if(it == GData::npcGroups.end())
			return false;

		GData::NpcGroup * ng = it->second;
        UInt16 bs = 0;
        if(type == 0)
            bs = Battle::BS_FRONTMAP1;
        else if(type == 1)
            bs = copyId - 1 + Battle::BS_COPY1;

		Battle::BattleSimulator bsim(bs, this, ng->getName(), ng->getLevel(), false);
		PutFighters( bsim, 0 );
		ng->putFighters( bsim );
		bsim.start();
		Stream& packet = bsim.getPacket();
		if(packet.size() <= 8)
			return false;

        UInt8 atoCnt = 0;
        UInt16 ret = 0x0100;
		bool res = bsim.getWinner() == 1;
		if(res)
		{
			ret = 0x0101;
			_lastNg = ng;
            pendExp(ng->getExp()*expfactor);
			ng->getLoots(this, _lastLoot, lootlvl, &atoCnt);
		}

        if (ato)
        {
            if (loot)
            {
                UInt8 sz = _lastLoot.size();
                (*loot).push_back(atoCnt);
                for(UInt8 i = 0; i < sz; ++ i)
                {
                    (*loot).push_back(_lastLoot[i].id);
                    (*loot).push_back(_lastLoot[i].count);
                }
            }
        }
        else
        {
            Stream st(REP::ATTACK_NPC);
            st << ret << _playerData.lastExp << static_cast<UInt8>(0);
            UInt8 sz = _lastLoot.size();
            st << sz;
            for(UInt8 i = 0; i < sz; ++ i)
            {
                st << _lastLoot[i].id << _lastLoot[i].count;
            }
            st.append(&packet[8], packet.size() - 8);
            st << Stream::eos;
            send(st);
        }

        if (applayhp)
            bsim.applyFighterHP(0, this);

		if(!res)
			checkDeath();

		setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns());
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
			sendMsgCode(0, 1151);
			return false;
		}
        if (ng->getType())
            return false;
		const UInt32 eachBattle = 60;
		//UInt8 level = GetLev();
		UInt32 count = 60 * 8;
#if 0
		if(level >= LEVEL_MAX)
			count = 60 * (LEVEL_MAX / 10);
#endif

		UInt32 viplvl = getVipLevel();
		if(viplvl >= 4 && viplvl <= 7)
			count += 60 * 8;
        else if (viplvl > 7 && viplvl <= 10)
			count += 60 * 16;

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
		Stream st(REP::TASK_RESPONSE_HOOK);
		st << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt32>(0) << (getMaxIcCount(_vipLevel) - getIcCount()) << Stream::eos;
		send(st);
		DB3().PushUpdateData("DELETE FROM `auto_battle` WHERE `playerId` = %"I64_FMT"u", _id);
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
		useGoldOrCoupon(GData::moneyNeed[GData::INSTANTAUTOBATTLE].gold,&ci);
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
		Stream st(REP::PACK_EXTEND);
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
			Stream st(REP::FRIEND_ACTION);
			st << static_cast<UInt8>(0x01) << pl->getId() << pl->getName() << pl->getPF() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << Stream::eos;
			send(st);
			SYSMSG_SEND(132, this);
			SYSMSG_SENDV(1032, this, pl->getCountry(), pl->getName().c_str());
			if(writedb)
				DB1().PushUpdateData("REPLACE INTO `friend` (`id`, `type`, `friendId`) VALUES (%"I64_FMT"u, 0, %"I64_FMT"u)", getId(), pl->getId());
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
		Stream st(REP::FRIEND_ACTION);
		st << static_cast<UInt8>(0x02) << pl->getName() << Stream::eos;
		send(st);
		SYSMSG_SEND(134, this);
		SYSMSG_SENDV(1034, this, pl->getCountry(), pl->getName().c_str());
		if(writedb)
			DB1().PushUpdateData("DELETE FROM `friend` WHERE `id` = %"I64_FMT"u AND `type` = 0 AND `friendId` = %"I64_FMT"u", getId(), pl->getId());
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
			sendMsgCode(2, 1505);
			return false;
		}
		if(pl == NULL || _hasBlock(pl))
		{
			SYSMSG_SEND(136, this);
			SYSMSG_SENDV(1036, this, pl->getCountry(), pl->getName().c_str());
			return false;
		}

		notifyFriendAct(4, pl);
		Stream st(REP::FRIEND_ACTION);
		st << static_cast<UInt8>(0x03) << pl->getId() << pl->getName() << pl->getPF() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << Stream::eos;
		send(st);
		DB1().PushUpdateData("REPLACE INTO `friend` (`id`, `type`, `friendId`) VALUES (%"I64_FMT"u, 1, %"I64_FMT"u)", getId(), pl->getId());
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
		Stream st(REP::FRIEND_ACTION);
		st << static_cast<UInt8>(0x04) << pl->getName() << Stream::eos;
		send(st);
		DB1().PushUpdateData("DELETE FROM `friend` WHERE `id` = %"I64_FMT"u AND `type` = 1 AND `friendId` = %"I64_FMT"u", getId(), pl->getId());
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
		Stream st(REP::FRIEND_ACTION);
		st << static_cast<UInt8>(0x05) << pl->getId() << pl->getName() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << Stream::eos;
		send(st);
		DB1().PushUpdateData("REPLACE INTO `friend` (`id`, `type`, `friendId`) VALUES (%"I64_FMT"u, 2, %"I64_FMT"u)", getId(), pl->getId());
		//SYSMSG_SEND(157, this);
		//SYSMSG_SENDV(1057, this, pl->getCountry(), pl->getName().c_str());

		if(_friends[2].size() >= 20)
		{
			std::set<Player *>::iterator it = _friends[2].begin();
			Stream st(REP::FRIEND_ACTION);
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
		Stream st(REP::FRIEND_ACTION);
		st << static_cast<UInt8>(0x06) << pl->getName() << Stream::eos;
		send(st);
		DB1().PushUpdateData("DELETE FROM `friend` WHERE `id` = %"I64_FMT"u AND `type` = 2 AND `friendId` = %"I64_FMT"u", getId(), pl->getId());
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
		Stream st(REP::FRIEND_LIST);
		st << static_cast<UInt8>(type) << start << cnt << sz;
        if (sz && cnt)
        {
            std::set<Player *>::iterator it = _friends[type].begin();
            std::advance(it, start);
            for(UInt8 i = 0; i < cnt; ++ i)
            {
                Player * pl = *it;
                st << pl->getId() << pl->getName() << pl->getPF() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry()
                    << pl->GetLev() << pl->GetClass() << pl->getClanName();
                ++it;
            }
        }
		st << Stream::eos;
		send(st);
	}

	void Player::sendModification( UInt8 t, UInt32 v, bool updateToDB )
	{
		if(_isOnline)
		{
			Stream st(REP::USER_INFO_CHANGE);
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
				DB7().PushUpdateData("REPLACE INTO `player_buff`(`id`, `buffId`, `data`) VALUES(%"I64_FMT"u, %u, %u)", _id, t - 0x40, v);
			else
				DB7().PushUpdateData("DELETE FROM `player_buff` WHERE `id` = %"I64_FMT"u AND `buffId` = %u", _id, t - 0x40);
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
			DB1().PushUpdateData("UPDATE `player` SET `%s` = %u WHERE `id` = %"I64_FMT"u", field, v, _id);
	}

	UInt32 Player::getGold( UInt32 c, UInt8 incomingType )
	{
		if(c == 0)
			return _playerData.gold;
		_playerData.gold += c;
		SYSMSG_SENDV(149, this, c);
		SYSMSG_SENDV(1049, this, c);
		sendModification(1, _playerData.gold);

        if(incomingType != 0)
        {
            DBLOG1().PushUpdateData("insert into consume_gold (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,0,0,%u,%u)",
                cfg.serverLogId, getId(), incomingType, c, TimeUtil::Now());
        }

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
				DBLOG1().PushUpdateData("insert into consume_gold (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
            }
        }
		SYSMSG_SENDV(150, this, c);
		SYSMSG_SENDV(1050, this, c);
		sendModification(1, _playerData.gold);
		return _playerData.gold;
	}

	bool Player::holdGold(UInt32 c, UInt8 action, ConsumeInfo * ci)
	{
		switch(action)
		{
		case 0:
			{
				if (c > _playerData.gold)
					return false;
                if(_isHoding)
                    return false;
                _isHoding = true;
                _holdGold = c;
			}
			break;
		case 1:
			{
                _playerData.gold -= _holdGold;
				sendModification(1, _playerData.gold);

                if(ci!=NULL)
                {
                    DBLOG1().PushUpdateData("insert into consume_gold (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                        cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
                }

				SYSMSG_SENDV(150, this, c);
				SYSMSG_SENDV(1050, this, c);
                _isHoding = false;
			}
			break;
		case 2:
			{
                _holdGold = 0;
				updateDB(1, _playerData.gold);
                _isHoding = false;
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
				DBLOG1().PushUpdateData("insert into consume_coupon (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
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
                std::string tbn("consume_tael");
                DBLOG1().GetMultiDBName(tbn);
				DBLOG1().PushUpdateData("insert into %s (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",tbn.c_str(), cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
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
                std::string tbn("consume_tael");
                DBLOG1().GetMultiDBName(tbn);
				DBLOG1().PushUpdateData("insert into %s (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					tbn.c_str(),cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
			}
			_playerData.tael -= c;
		}
		SYSMSG_SENDV(152, this, c);
		//SYSMSG_SENDV(1060, this, attacker->getCountry(), attacker->getName().c_str(), c);
		sendModification(3, _playerData.tael);
	}

	UInt32 Player::getCoin( UInt32 c )
	{
        return 0; // XXX: no useful
		if(c == 0)
			return _playerData.coin;
		if(_playerData.coin >= 99999999)
		{
			//SYSMSG_SENDV(159, this);
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
				//SYSMSG_SENDV(153, this, c);
				//SYSMSG_SENDV(1053, this, c);
			}
		}
		else
		{
			//SYSMSG_SENDV(153, this, c);
			//SYSMSG_SENDV(1053, this, c);
		}

		if(_playerData.coin >= 99999999)
		{
			//SYSMSG_SENDV(159, this);
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
				DBLOG1().PushUpdateData("insert into consume_coin (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
            }
		    _playerData.coin -= c;
		}
		if(notify)
		{
			//SYSMSG_SENDV(154, this, c);
			//SYSMSG_SENDV(1054, this, c);
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
				//SYSMSG_SENDV(154, this, c);
				//SYSMSG_SENDV(1054, this, c);
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
		data->factor = (priceType == 0 ? 8.0f : 3.0f);
		data->price = price;
		data->priceType = priceType;
		data->accExp = exp;
		data->trainend = TimeUtil::Now() + 3600 * data->checktime;
		EventFighterTrain* event = new(std::nothrow) EventFighterTrain(this, 3600, data->checktime, fgt, data->trainend);
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
		//UInt32 count = data->checktime;
		//if (count > 0)
		{
            UInt32 count = 0;
            if((TimeUtil::Now() + (data->checktime * 3600)) > data->trainend)
                count = (TimeUtil::Now() + (data->checktime * 3600) - data->trainend)/60;
            UInt32 money = 0;
            if(data->checktime * 60 > count)
                money = data->price * static_cast<float>(data->checktime * 60 - count)/(data->traintime * 60);
			//UInt32 money = data->checktime * data->price / data->traintime;
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
		DB1().PushUpdateData("DELETE FROM `fighter_train` WHERE `fgtId` = %u AND `ownerId` = %"I64_FMT"u", id, _id);
		Stream st(REP::TRAIN_FIGHTER_OP);
		st << static_cast<UInt8>(2) << id << static_cast<UInt8>(1) << static_cast<UInt32>(0) << Stream::eos;
		send(st);
		//if (notify)
		//	sendMsgCode(0, 2088, id);

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
				sendMsgCode(0, 1101);
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
				sendMsgCode(0, 1100);
				return false;
			}
			useTael(price, &ci);
		}
		TrainFighterData *& data = _trainFighters[id];
		if (data != NULL) delete data;
		data = new TrainFighterData();
		data->checktime = time;
		data->traintime = data->checktime;
		data->factor = (priceType == 0 ? 8.0f : 3.0f);
		data->price = price;
		data->priceType = priceType;
		data->trainend = TimeUtil::Now() + 3600 * data->checktime;
		DB1().PushUpdateData("REPLACE INTO `fighter_train`(`fgtId`, `ownerId`, `priceType`, `price`, `trainTime`, `checkTime`) VALUES(%u, %"I64_FMT"u, %u, %u, %u, %u)", id, getId(), priceType, price, data->traintime, data->checktime);
		removeFighterFromLineup(id);
		EventFighterTrain* event = new(std::nothrow) EventFighterTrain(this, 3600, data->checktime, fgt, data->trainend);
		if (event == NULL) return false;
		PushTimerEvent(event);
		Stream st(REP::TRAIN_FIGHTER_OP);
		UInt32 remain = event->GetEnd() - TimeUtil::Now();
		st << static_cast<UInt8>(1) << id << data->priceType << remain << Stream::eos;
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
			UInt32 count = hrs;
			if(count > data->checktime)
			{
				count = data->checktime;
			}
			UInt32 goldUse = 10 * count;
			if (getGold() < goldUse)
				return false;
			ConsumeInfo ci(AccTrainFighter, 0, 0);
			const std::vector<UInt32>& levExp = GData::GDataManager::GetLevelTrainExp();
            UInt32 i = 0;
            for(; i < count; ++ i)
            {
                if (fighter->getLevel() >= GetLev())
                    break;

                UInt32 exp = static_cast<UInt32>(levExp[fighter->getLevel()] * data->factor * 60);
                fighter->addExp(exp);
                data->accExp += exp;
            }

            goldUse = 10 * i;
			useGold(goldUse, &ci);
			data->checktime -= i;
			data->trainend -= i * 3600;
			if (data->checktime == 0 || fighter->getLevel() >= GetLev())
			{
				if(delTrainFighter(id, true))
					PopTimerEvent(this, EVENT_FIGHTERAUTOTRAINING, id);
			}
			else
			{
				DB().PushUpdateData("UPDATE `fighter_train` SET `checkTime` = %u, `accExp` = %u WHERE `fgtId` = %u AND `ownerId` = %"I64_FMT"u", data->checktime, data->accExp, id, _id);
				Stream st(REP::TRAIN_FIGHTER_OP);
				UInt32 now = TimeUtil::Now();
				st << static_cast<UInt8>(2) << id << static_cast<UInt8>(0);
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
		std::map<UInt32, TrainFighterData *>::iterator found = _trainFighters.find(id);
		if (found == _trainFighters.end())
			return false;
		Fighter * fighter = findFighter(id);
		if (fighter == NULL)
			return false;
		TrainFighterData * data = found->second;
        UInt32 exp = 0;
		if (data->checktime > 0)
		{
            UInt32 count = (TimeUtil::Now() + (data->checktime * 3600) - data->trainend)/60;
            const std::vector<UInt32>& levExp = GData::GDataManager::GetLevelTrainExp();
            exp = static_cast<UInt32>(levExp[fighter->getLevel()] * data->factor * count);
            data->accExp += exp;
        }

		if(delTrainFighter(id))
        {
            fighter->addExp(exp);
			PopTimerEvent(this, EVENT_FIGHTERAUTOTRAINING, id);
        }
		return true;
	}

    void Player::makeTrainFighterInfo(Stream& st)
    {
        st.init(REP::TRAIN_FIGHTER_OP);
        UInt8 cnt = 0;
        st << static_cast<UInt8>(0) << cnt;
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
            st.data<UInt8>(5) = cnt;
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
		// SYSMSG_SENDV(105, this, a);
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
				DBLOG1().PushUpdateData("insert into consume_achievement (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
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
				DBLOG1().PushUpdateData("insert into consume_achievement (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
			}
		}
		//SYSMSG_SENDV(106, this, a);
		//SYSMSG_SENDV(1061, this, attacker->getCountry(), attacker->getName().c_str(), a);
		sendModification(8, _playerData.achievement);
		return ;
	}

    UInt32 Player::getPrestige(UInt32 a, bool notify)
    {
		if(a == 0)
			return _playerData.prestige;
		_playerData.prestige += a;

        if(notify)
        {
            SYSMSG_SENDV(185, this, a);
            SYSMSG_SENDV(1090, this, a);
        }

        DB6().PushUpdateData("UPDATE `player` SET `prestige` = %u WHERE `id` = %"I64_FMT"u", _playerData.prestige, getId());

        Stream st(REP::USER_INFO_CHANGE);
        st << static_cast<UInt8>(0x55) << _playerData.prestige << Stream::eos;
        send(st);

		return _playerData.prestige;
    }

    UInt32 Player::usePrestige(UInt32 a,ConsumeInfo * ci)
    {
		if(a == 0 || _playerData.prestige == 0)
			return _playerData.prestige;
		if(_playerData.prestige < a)
			_playerData.prestige = 0;
		else
		{
		    _playerData.prestige -= a;
		    if(ci!=NULL)
		    {
				DBLOG1().PushUpdateData("insert into consume_prestige (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
					cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
            }
		}
		SYSMSG_SENDV(186, this, a);
		SYSMSG_SENDV(1091, this, a);

        DB6().PushUpdateData("UPDATE `player` SET `prestige` = %u WHERE `id` = %"I64_FMT"u", _playerData.prestige, getId());

        Stream st(REP::USER_INFO_CHANGE);
        st << static_cast<UInt8>(0x55) << _playerData.prestige << Stream::eos;
        send(st);

		return _playerData.prestige;
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
			if(fgt != NULL)
				fgt->addExp(exp);
		}
        sendLevelPack(GetLev()); // XXX: 
	}

    void Player::addHIAttr(const GData::AttrExtra& attr)
    {
        _hiattr += attr;
		for(int i = 0; i < 5; ++ i)
		{
			GObject::Fighter * fgt = getLineup(i).fighter;
			if(fgt != NULL)
				fgt->setDirty();
        }
    }

    void Player::clearHIAttr()
    {
        _hiattr.reset();
		for(int i = 0; i < 5; ++ i)
		{
			GObject::Fighter * fgt = getLineup(i).fighter;
			if(fgt != NULL)
				fgt->setDirty();
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

        if (_playerData.location == 8977)
        {
            heroIsland.playerLeave(this);
            delFlag(Player::InHeroIsland);
        }

#if 1
		UInt8 new_cny = GObject::mapCollection.getCountryFromSpot(spot);
        if (new_cny > WORKER_THREAD_LOGIN)
        {
            SYSMSG_SENDV(621, this, new_cny);
            return;
        }

		if(new_cny != cny.GetThreadID())
		{
			CountryEnterStruct ces(true, inCity ? 1 : 0, spot);
			cny.PlayerLeave(this);
			_threadId = new_cny;
			GameMsgHdr hdr(0x1F0, new_cny, this, sizeof(CountryEnterStruct));
			GLOBAL().PushMsg( hdr, &ces );
			return;
		}
#else
        if (getCountry() != cny.GetThreadID())
        {
			CountryEnterStruct ces(true, inCity ? 1 : 0, spot);
			cny.PlayerLeave(this);
			_threadId = getCountry();
			GameMsgHdr hdr(0x1F0, getCountry(), this, sizeof(CountryEnterStruct));
			GLOBAL().PushMsg( hdr, &ces );
			return;
        }
#endif

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
		DB1().PushUpdateData("UPDATE `player` SET `inCity` = %u, `location` = %u WHERE id = %" I64_FMT "u", _playerData.inCity, _playerData.location, getId());

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
		DB1().PushUpdateData("UPDATE `player` SET `tavernId` = '%u|%u|%u|%u|%u|%u|%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u",
                _playerData.tavernId[0], _playerData.tavernId[1], _playerData.tavernId[2], _playerData.tavernId[3],
                _playerData.tavernId[4], _playerData.tavernId[5], _playerData.tavernBlueCount,
                _playerData.tavernPurpleCount, _playerData.tavernOrangeCount, _nextTavernUpdate, _id);
	}

    void Player::resetShiMen()
    {
        _playerData.smFinishCount = 0;
        _playerData.smFreeCount = 0;
        _playerData.smAcceptCount = 0;
        _playerData.shimen.clear();

        writeShiMen();

        if (isOnline())
            sendColorTask(0, 0);
    }

    void Player::resetYaMen()
    {
        _playerData.ymFinishCount = 0;
        _playerData.ymFreeCount = 0;
        _playerData.ymAcceptCount = 0;
        _playerData.yamen.clear();

        writeYaMen();

        if (isOnline())
            sendColorTask(1, 0);
    }

	void Player::writeShiMen()
	{
        std::string shimen = "";
        int cnt = _playerData.shimen.size();
        if(cnt == 0)
            shimen = "0,0|";
        for(int i = 0; i < cnt; ++ i)
        {
            shimen += Itoa(_playerData.shimen[i]);
            shimen += ',';
            shimen += Itoa(_playerData.smcolor[i]);
            shimen += '|';
        }

		DB1().PushUpdateData("UPDATE `player` SET `shimen` = '%s%u|%u|%u', `fshimen` = '%u,%u|%u,%u|%u,%u|%u,%u|%u,%u|%u,%u' WHERE `id` = %"I64_FMT"u",
                shimen.c_str(), _playerData.smFreeCount, _playerData.smFinishCount, _playerData.smAcceptCount,
                _playerData.fshimen[0], _playerData.fsmcolor[0], _playerData.fshimen[1], _playerData.fsmcolor[1],
                _playerData.fshimen[2], _playerData.fsmcolor[2], _playerData.fshimen[3], _playerData.fsmcolor[3],
                _playerData.fshimen[4], _playerData.fsmcolor[4], _playerData.fshimen[5], _playerData.fsmcolor[5], _id);
	}

	void Player::writeYaMen()
	{
        std::string yamen = "";
        int cnt = _playerData.yamen.size();
        if(cnt == 0)
            yamen = "0,0|";
        for(int i = 0; i < cnt; ++ i)
        {
            yamen += Itoa(_playerData.yamen[i]);
            yamen += ',';
            yamen += Itoa(_playerData.ymcolor[i]);
            yamen += '|';
        }

		DB1().PushUpdateData("UPDATE `player` SET `yamen` = '%s%u|%u|%u',`fyamen` = '%u,%u|%u,%u|%u,%u|%u,%u|%u,%u|%u,%u' WHERE `id` = %"I64_FMT"u",
                yamen.c_str(), _playerData.ymFreeCount, _playerData.ymFinishCount, _playerData.ymAcceptCount,
                _playerData.fyamen[0], _playerData.fymcolor[0], _playerData.fyamen[1], _playerData.fymcolor[1],
                _playerData.fyamen[2], _playerData.fymcolor[2], _playerData.fyamen[3], _playerData.fymcolor[3],
                _playerData.fyamen[4], _playerData.fymcolor[4], _playerData.fyamen[5], _playerData.fymcolor[5], _id);
	}

    void Player::writeShiYaMen()
    {
        std::string shimen = "";
        int cnt = _playerData.shimen.size();
        if(cnt == 0)
            shimen = "0,0|";
        for(int i = 0; i < cnt; ++ i)
        {
            shimen += Itoa(_playerData.shimen[i]);
            shimen += ',';
            shimen += Itoa(_playerData.smcolor[i]);
            shimen += '|';
        }

        std::string yamen = "";
        cnt = _playerData.yamen.size();
        if(cnt == 0)
            yamen = "0,0|";
        for(int j = 0; j < cnt; ++ j)
        {
            yamen += Itoa(_playerData.yamen[j]);
            yamen += ',';
            yamen += Itoa(_playerData.ymcolor[j]);
            yamen += '|';
        }

		DB1().PushUpdateData("UPDATE `player` SET `shimen` = '%s|%u|%u|%u', `fshimen` = '%u,%u|%u,%u|%u,%u|%u,%u|%u,%u|%u,%u', `yamen` = '%s|%u|%u|%u', `fyamen` = '%u,%u|%u,%u|%u,%u|%u,%u|%u,%u|%u,%u' WHERE `id` = %"I64_FMT"u",
                shimen.c_str(), _playerData.smFreeCount, _playerData.smFinishCount, _playerData.smAcceptCount,
                _playerData.fshimen[0], _playerData.fsmcolor[0], _playerData.fshimen[1], _playerData.fsmcolor[1],
                _playerData.fshimen[2], _playerData.fsmcolor[2], _playerData.fshimen[3], _playerData.fsmcolor[3],
                _playerData.fshimen[4], _playerData.fsmcolor[4], _playerData.fshimen[5], _playerData.fsmcolor[5],
                yamen.c_str(), _playerData.ymFreeCount, _playerData.ymFinishCount, _playerData.ymAcceptCount,
                _playerData.fyamen[0], _playerData.fymcolor[0], _playerData.fyamen[1], _playerData.fymcolor[1],
                _playerData.fyamen[2], _playerData.fymcolor[2], _playerData.fyamen[3], _playerData.fymcolor[3],
                _playerData.fyamen[4], _playerData.fymcolor[4], _playerData.fyamen[5], _playerData.fymcolor[5], _id);
    }

    bool Player::addAwardByTaskColor(UInt32 taskid, bool im)
    {
        if (!im) {
            std::vector<UInt32>& shimen = _playerData.shimen;
            std::vector<UInt8>& smcolor = _playerData.smcolor;
            int cnt = shimen.size();
            for (int i = 0; i < cnt; ++i) {
                if (shimen[i] == taskid) {
                    UInt32 award = Script::BattleFormula::getCurrent()->calcTaskAward(0, smcolor[i], GetLev());
                    AddExp(award*(World::_wday==2?2:1));
                    ++_playerData.smFinishCount;
                    shimen.erase(shimen.begin() + i);
                    smcolor.erase(smcolor.begin() + i);
                    --cnt;

                    sendColorTask(0, 0);
                    writeShiMen();
                    return true;
                }
            }

            std::vector<UInt32>& yamen = _playerData.yamen;
            std::vector<UInt8>& ymcolor = _playerData.ymcolor;
            cnt = yamen.size();
            for (int i = 0; i < cnt; ++i) {
                if (yamen[i] == taskid) {
                    UInt32 award = Script::BattleFormula::getCurrent()->calcTaskAward(2, ymcolor[i], GetLev());
                    getTael(award*(World::_wday==2?2:1));
                    ++_playerData.ymFinishCount;
                    yamen.erase(yamen.begin() + i);
                    ymcolor.erase(ymcolor.begin() + i);
                    --cnt;

                    sendColorTask(1, 0);
                    writeYaMen();
                    return true;
                }
            }
        } else {
            for (int i = 0; i < 6; ++i) {
                if (_playerData.fshimen[i] == taskid) {
                    if (getVipLevel() < 3) {
                        sendMsgCode(0, 1003);
                        return false;
                    }

                    if (GetLev() < static_cast<UInt8>(30)) {
                        sendMsgCode(1, 1016);
                        return false;
                    }

                    if (ColorTaskOutOfAccept(4, im))
                        return false;

                    if (getGold() < GData::moneyNeed[GData::SHIMEN_IM].gold) {
                        sendMsgCode(0, 1101);
                        return false;
                    }

                    ConsumeInfo ci(ShimenTask, 0, 0);
                    useGold(GData::moneyNeed[GData::SHIMEN_IM].gold, &ci);

                    UInt32 award = Script::BattleFormula::getCurrent()->calcTaskAward(0, _playerData.fsmcolor[i], GetLev());
                    AddExp(award*(World::_wday==2?2:1));
                    ++_playerData.smFinishCount;
                    ++_playerData.smAcceptCount;
                    _playerData.fshimen[i] = 0;
                    _playerData.fsmcolor[i] = 0;

                    sendColorTask(0, 0);
                    writeShiMen();
                    return true;
                }
            }

            for (int i = 0; i < 6; ++i) {
                if (_playerData.fyamen[i] == taskid) {
                    if (GetLev() < static_cast<UInt8>(30)) {
                        sendMsgCode(1, 1016);
                        return false;
                    }

                    if (ColorTaskOutOfAccept(5, im))
                        return false;

                    if (getGold() < GData::moneyNeed[GData::YAMEN_IM].gold) {
                        sendMsgCode(0, 1101);
                        return false;
                    }
                    
                    ConsumeInfo ci(YamenTask, 0, 0);
                    useGold(GData::moneyNeed[GData::YAMEN_IM].gold, &ci);

                    UInt32 award = Script::BattleFormula::getCurrent()->calcTaskAward(2, _playerData.fymcolor[i], GetLev());
                    getTael(award*(World::_wday==2?2:1));
                    AddExp(3000);

                    ++_playerData.ymFinishCount;
                    ++_playerData.ymAcceptCount;
                    _playerData.fyamen[i] = 0;
                    _playerData.fymcolor[i] = 0;

                    sendColorTask(1, 0);
                    writeYaMen();
                    return true;
                }
            }
        }
        return false;
    }

    bool Player::ColorTaskOutOfAccept(UInt8 type, bool im)
    {
        if (type == 4) {
            if (_playerData.smAcceptCount >= getShiMenMax()) {
                SYSMSG_SENDV(2107, this);
                return true;
            }
        } else if (type == 5) {
            if (_playerData.ymAcceptCount >= getYaMenMax()) {
                SYSMSG_SENDV(2108, this);
                return true;
            }
        }
        return false;
    }

    void Player::ColorTaskAccept(UInt8 type, UInt32 taskid)
    {
        if (type == 4) {
            for (int i = 0; i < 6; ++i) {
                if (_playerData.fshimen[i] == taskid) {
                    _playerData.shimen.push_back(taskid);
                    _playerData.smcolor.push_back(_playerData.fsmcolor[i]);

                    _playerData.fshimen[i] = 0;
                    _playerData.fsmcolor[i] = 0;

                    ++_playerData.smAcceptCount;
                    sendColorTask(0, 0);
                    writeShiMen();
                    return;
                }
            }
        }

        if (type == 5) {
            for (int i = 0; i < 6; ++i) {
                if (_playerData.fyamen[i] == taskid) {
                    _playerData.yamen.push_back(taskid);
                    _playerData.ymcolor.push_back(_playerData.fymcolor[i]);

                    _playerData.fyamen[i] = 0;
                    _playerData.fymcolor[i] = 0;

                    ++_playerData.ymAcceptCount;
                    sendColorTask(1, 0);
                    writeYaMen();
                    return;
                }
            }
        }
    }

    void Player::ColorTaskAbandon(UInt8 type, UInt32 taskid)
    {
        if (type == 4) {
            std::vector<UInt32>& shimen = _playerData.shimen;
            std::vector<UInt8>& smcolor = _playerData.smcolor;
            int cnt = shimen.size();
            for (int i = 0; i < cnt; ++i) {
                if (shimen[i] == taskid) {
                    shimen.erase(shimen.begin() + i);
                    smcolor.erase(smcolor.begin() + i);
                    if (_playerData.smAcceptCount) {
                        --_playerData.smAcceptCount;
                        sendColorTask(0, 0);
                        writeShiMen();
                    }
                    return;
                }
            }
        }

        if (type == 5) {
            std::vector<UInt32>& yamen = _playerData.yamen;
            std::vector<UInt8>& ymcolor = _playerData.ymcolor;
            int cnt = yamen.size();
            for (int i = 0; i < cnt; ++i) {
                if (yamen[i] == taskid) {
                    yamen.erase(yamen.begin() + i);
                    ymcolor.erase(ymcolor.begin() + i);
                    if (_playerData.ymAcceptCount) {
                        --_playerData.ymAcceptCount;
                        sendColorTask(1, 0);
                        writeYaMen();
                    }
                    return;
                }
            }
        }
    }

    void Player::clearFinishCount()
    {
        _playerData.smFinishCount = 0;
        _playerData.ymFinishCount = 0;
        _playerData.smFreeCount = 0;
        _playerData.ymFreeCount = 0;

        _playerData.smAcceptCount = _playerData.shimen.size();
        _playerData.ymAcceptCount = _playerData.yamen.size();

        writeShiYaMen();

        if (isOnline()) {
            sendColorTask(0, 0);
            sendColorTask(1, 0);
        }
    }

    bool Player::finishClanTask(UInt32 taskId)
    {
		const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(taskId);
        if(taskType.m_Class != 6)
            return false;

        if(getClan() == NULL) {
            delClanTask();
            return false;
        }

        if(taskId != _playerData.clanTaskId || _playerData.ctFinishCount > CLAN_TASK_MAXCOUNT - 1)
            return false;

        ++ _playerData.ctFinishCount;
        if(CLAN_TASK_MAXCOUNT > _playerData.ctFinishCount) {
            URandom rnd(time(NULL));
            const std::vector<UInt32>& task = GData::GDataManager::GetClanTask();
            UInt8 idx = rnd(task.size());
            UInt32 taskid = task[idx];
            if(_playerData.clanTaskId == taskid)
                _playerData.clanTaskId = task[++idx % task.size()];
            else
                _playerData.clanTaskId = taskid;
            GetTaskMgr()->AddCanAcceptTask(_playerData.clanTaskId);
        } else {
            _playerData.clanTaskId = 0;
        }

        writeClanTask();
        return true;
    }

    void Player::delClanTask()
    {
        if(_playerData.clanTaskId)
        {
            if(!GetTaskMgr()->DelTask(_playerData.clanTaskId))
                GetTaskMgr()->DelCanAcceptTask(_playerData.clanTaskId);

            _playerData.clanTaskId = 0;

            writeClanTask();
        }
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
        Stream st(REP::CLAN_INFO_UPDATE);
        st << static_cast<UInt8>(9) << ((_playerData.ctFinishCount << 4) | CLAN_TASK_MAXCOUNT);
        st << Stream::eos;
        send(st);

		DB1().PushUpdateData("UPDATE `player` SET `clantask` = '%u,%u' WHERE `id` = %"I64_FMT"u",  _playerData.clanTaskId, _playerData.ctFinishCount, _id);
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

    bool isFirst(UInt8* colors)
    {
        for (UInt8 i = 0; i < 6; ++i)
            if (colors[i])
                return false;
        return true;
    }

    inline bool hasCTAccept(std::vector<UInt32>& tasks, UInt32 task, int rcnt)
    {
        if (rcnt > 100)
            return false;

        int cnt = tasks.size();
        UInt32* t = &tasks[0];
        for (int i = 0; i < cnt; ++i)
        {
            if (*t++ == task)
                return true;
        }
        return false;
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
                if (isFirst(_playerData.fsmcolor))
                    first = true;
            }
            if (ttype == 1) {
                if (isFirst(_playerData.fymcolor))
                    first = true;
            }
        }

        UInt16 ncount = 0;
        if (type || first) {
            UInt32 money = 0;
            UInt16 usedGold = 0, maxGold = 0;
            if (ftype) {
                if (ttype == 0)
                    money = GData::moneyNeed[GData::SHIMEN_FRESH].gold;
                else
                    money = GData::moneyNeed[GData::YAMEN_FRESH].gold;

                maxGold = money * count;

                if(_playerData.gold < maxGold) {
                    sendMsgCode(1, 1101);
                    return;
                }
            }

            const std::vector<UInt8>& factor = GData::GDataManager::GetFlushTaskFactor(ttype, ftype);
            UInt8 rfac[5] = {0};
            rfac[0] = factor[0];
            for (int i = 1; i < 5; ++i)
                rfac[i] = rfac[i-1] + factor[i];

            bool percolor = false;
            do {
                ++ncount;
                if ((!ftype && ((ttype == 0 && _playerData.smFreeCount < getShiMenMax()) ||
                                (ttype == 1 && _playerData.ymFreeCount < getYaMenMax()))) || ftype) {
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
                            if (ttype == 0) {
                                int rcnt = 0;
                                while (idxs.find(j) != idxs.end() || hasCTAccept(_playerData.shimen, task[j], ++rcnt))
                                    j = rnd(task.size());
                            } else {
                                int rcnt = 0;
                                while (idxs.find(j) != idxs.end() || hasCTAccept(_playerData.yamen, task[j], ++rcnt))
                                    j = rnd(task.size());
                            }
                            idxs.insert(j);
                        }
                    }

                    int n = 0;
                    for (std::set<UInt32>::iterator i = idxs.begin(), e = idxs.end(); i != e; ++i) {
                        UInt32 rd = rnd(100);
                        for (int j = 0; j < 5; ++j) {
                            if (rd <= rfac[j]) {
                                if (ttype == 0) {
                                    _playerData.fshimen[n] = task[*i];
                                    _playerData.fsmcolor[n] = j+1;
                                    if (getVipLevel() >= 3) {
                                        static UInt8 viptaskcolor[11] = {0,0,0,3,3,3,4,4,4,4,4};
                                        if (_playerData.fsmcolor[n] < viptaskcolor[getVipLevel()])
                                            _playerData.fsmcolor[n] = viptaskcolor[getVipLevel()];
                                    }
                                } else {
                                    _playerData.fyamen[n] = task[*i];
                                    _playerData.fymcolor[n] = j+1;
                                    if (getVipLevel() >= 2) {
                                        static UInt8 viptaskcolor[11] = {0,0,3,3,3,4,4,4,4,4,4};
                                        if (_playerData.fymcolor[n] < viptaskcolor[getVipLevel()])
                                            _playerData.fymcolor[n] = viptaskcolor[getVipLevel()];
                                    }
                                }

                                if (color && j+1 == color)
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
                            ++_playerData.ymFreeCount;
                    }
                }

                if (ftype)
                    usedGold += money;

                if (percolor)
                    break;
                if (type == 2)
                    break;

                --count;
            } while (count > 0);

            if (ftype) {
                if (ttype == 0) {
                    ConsumeInfo ci(ShimenTaskFresh, 0, 0);
                    useGold(usedGold, &ci);
                } else {
                    ConsumeInfo ci(YamenTaskFresh, 0, 0);
                    useGold(usedGold, &ci);
                }
            }

            if (!ttype)
                writeShiMen();
            else
                writeYaMen();
        }

        sendColorTask(ttype, ncount);
    }

    void Player::sendColorTask(UInt8 ttype, UInt16 ncount)
    {
        Stream st(REP::TASK_CYC_FRESH);
        st << ttype;
        if (ttype == 0) {
            st <<  ncount << _playerData.smFinishCount;
            st << _playerData.smFreeCount;
        } else {
            st <<  ncount << _playerData.ymFinishCount;
            st << _playerData.ymFreeCount;
        }

        if (ttype == 0) {
            for (int i = 0; i < 6; ++i) {
                if (!_playerData.fsmcolor[i]) {
                    st << static_cast<UInt32>(0);
                    st << static_cast<UInt8>(0);
                } else {
                    st << _playerData.fshimen[i];
                    st << static_cast<UInt8>(_playerData.fsmcolor[i]);
                }
            }
        } else {
            for (int i = 0; i < 6; ++i) {
                if (!_playerData.fymcolor[i]) {
                    st << static_cast<UInt32>(0);
                    st << static_cast<UInt8>(0);
                } else {
                    st << _playerData.fyamen[i];
                    st << static_cast<UInt8>(_playerData.fymcolor[i]);
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

		if(_playerData.gold + _playerData.coupon < maxGold)
		{
			sendMsgCode(1, 1101);
			return;
		}

        if(count == 1)
            color = 1;
        else if(color > 0)
            -- color;

        if(count > 0 && globalFighters.getColorFighterNum(extraRefresh ? 0 : 1, color) == getColorFighterNum(color))
        {
			sendMsgCode(0, 1200);
            return;
        }

        while(color > 0)
        {
            if(globalFighters.getColorFighterNum(extraRefresh ? 0 : 1, color) > 0)
                break;
            -- color;
        }

		UInt16 tcount = 0;
		Stream st(REP::HOTEL_PUB_LIST);
		if(count > 0)
		{
            UInt8 hasGet = 0;
			do
			{
				bool hasBlue = false, hasPurple = false, hasOrange = false;
				std::set<UInt32> excepts, excepts2;
				int i;
				//for(i = 0; i < 6; ++ i)
                    //excepts2.insert(_playerData.tavernId[i]);
				i = 0;
                int rateidx0 = 0;
                int rateidx1 = 0;
                int rateidx2 = 0;
                if(!extraRefresh)
                {
                    rateidx0 = _playerData.tavernBlueCount / 10;
                    rateidx1 = _playerData.tavernPurpleCount / 10;
                    rateidx2 = _playerData.tavernOrangeCount / 10;
                }
                else
                {
                    rateidx0 = _playerData.tavernBlueCount / 10;
                    rateidx1 = _playerData.tavernPurpleCount / 10;
                }

                if(rateidx0 < 0)
                    rateidx0 = 0;
                if(rateidx1 < 0)
                    rateidx1 = 0;
                if(rateidx2 < 0)
                    rateidx2 = 0;

				if(!extraRefresh)
				{
					usedGold += _recruit_cost;
#if 0
					if(_playerData.tavernBlueCount >= _tavernBlueCount)
					{
						Fighter * fgt = globalFighters.getRandomOut(this, excepts, excepts2, 2, rateidx);
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
						Fighter * fgt = globalFighters.getRandomOut(this, excepts, excepts2, 3, rateidx);
						if(fgt != NULL)
						{
							_playerData.tavernId[i] = fgt->getId();
							excepts.insert(fgt->getId());

							hasPurple = true;
							++ i;
						}
					}
					if(_playerData.tavernOrangeCount >= _tavernOrangeCount)
					{
						Fighter * fgt = globalFighters.getRandomOut(this, excepts, excepts2, 4, rateidx);
						if(fgt != NULL)
						{
							_playerData.tavernId[i] = fgt->getId();
							excepts.insert(fgt->getId());

							hasOrange = true;
							++ i;
						}
					}
#endif
				}

				for(; i < 6; ++ i)
				{
					Fighter * fgt = globalFighters.getRandomOut(this, excepts, excepts2, extraRefresh ? 0 : 1, rateidx0, rateidx1, rateidx2);
					if(fgt == NULL)
					{
						_playerData.tavernId[i] = 0;
					}
					else
					{
						UInt8 tmpcolor = fgt->getColor();
                        if(tmpcolor)
                        {
                            if( (1 << tmpcolor) & hasGet ) 
                            {
                                -- i;
                                continue;
                            }
                            else
                            {
                                hasGet |= (1 << tmpcolor);
                            }
                        }

						_playerData.tavernId[i] = fgt->getId();
						excepts.insert(fgt->getId());
						switch(tmpcolor)
						{
						case 1:
							hasBlue = true;
							break;
						case 2:
							hasPurple = true;
							break;
                        case 3:
                            hasOrange = true;
						default:
							break;
						}
					}
				}
				if(extraRefresh)
				{
					extraRefresh = false;
					if(hasBlue)
						_playerData.tavernBlueCount = 0;
					else
						++ _playerData.tavernBlueCount;
                    if(hasPurple)
						_playerData.tavernPurpleCount = 0;
					else
						++ _playerData.tavernPurpleCount;
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
                    if(hasOrange)
                        _playerData.tavernOrangeCount = 0;
                    else
                        ++ _playerData.tavernOrangeCount;
				}
				-- count;
				if(hasBlue)
				{
					if(color <= 1)
						break;
				}
				if(hasPurple)
				{
					if(color <= 2)
						break;
				}
                if(hasOrange)
                {
                    if(color <=3)
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
            useGoldOrCoupon(usedGold, &ci);
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
			sendMsgCode(0, 1200);
			return 0;
		}
		UInt32 price = getTavernPrice(_playerData.tavernId[idx]);
		if(_playerData.tael < price)
		{
			sendMsgCode(0, 1100);
			return 0;
		}
		Fighter * fgt = takeFighter(_playerData.tavernId[idx], true);
		if(fgt == NULL)
			return 0;
		_playerData.tavernId[idx] = 0;
		writeTavernIds();
		ConsumeInfo ci(RecruitFighter,0,0);
		useTael(price,&ci);

        if(fgt->getColor() == 2)
        {
            _playerData.tavernPurpleCount = 0;
        }
        else if(fgt->getColor() == 3)
        {
            _playerData.tavernPurpleCount = 0;
            _playerData.tavernOrangeCount = 0;
        }

        if(fgt->getColor() ==3)
        {
            GameAction()->onRecruitAward(this);
        }

		return fgt->getId();
	}

	void Player::updateNextTavernUpdate(UInt32 curtime)
	{
		_nextTavernUpdate = (curtime + _tavernInterval) / _tavernInterval * _tavernInterval;
	}

    UInt32 Player::getColorFighterNum(UInt8 color)
    {
        UInt32 num = 0;
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
        {
            Fighter* fighter = it->second;
            if(fighter->getColor() == color && it->second->getId() > 6)
                ++ num;
        }

        return num;
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
		Stream st(REP::ALERT_MSG);
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
					hp = maxhp;
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
		st.init(REP::WALLOW_VERIFY);
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
		DB1().PushUpdateData("UPDATE `player` SET `gmLevel` = %u WHERE `id` = %"I64_FMT"u", l, _id);
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
            if (!lu1.fighter)
                return;
			bool mfSolid = lu1.fighter->getClass() == 3;
			if(mfSolid)
				lu1.pos = newPos[starti ++];
			for(int i = 1; i < 5; ++ i)
			{
				Lineup& lu = _playerData.lineup[i];
				if(!lu.available())
					continue;
                if (!lu.fighter)
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
		DB1().PushUpdateData("UPDATE `player` SET `icCount` = '%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.icCount, _playerData.nextIcReset, _id);
	}

	void Player::resetIcCount( )
	{
		checkIcExpire(false);
		if(_playerData.icCount > 0)
			_playerData.icCount = 0;
		DB1().PushUpdateData("UPDATE `player` SET `icCount` = '%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.icCount, _playerData.nextIcReset, _id);
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
			DB1().PushUpdateData("UPDATE `player` SET `icCount` = '%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.icCount, _playerData.nextIcReset, _id);
		}
	}

	void Player::checkHPLoss()
	{
#if 0
		if(!hasStatus(FirstHPLoss))
		{
			Stream st(REP::GUIDE_RESPONSE_STEP);
			st << static_cast<UInt16>(1) << Stream::eos;
			send(st);
			addStatus(FirstHPLoss);
		}
#endif
	}

	void Player::checkDeath()
	{
#if 0
		if(!hasStatus(FirstDeath))
		{
			Stream st(REP::GUIDE_RESPONSE_STEP);
			st << static_cast<UInt16>(2) << Stream::eos;
			send(st);
			addStatus(FirstDeath);
		}
#endif
	}

	void Player::checkLevUp(UInt8 oLev, UInt8 nLev)
	{
		if(_clan != NULL)
		{
			_clan->broadcastMemberInfo(this);
		}
		m_TaskMgr->CheckCanAcceptTaskByLev(nLev);
		if ((nLev > 29 && !m_Athletics->hasEnterAthletics()) || (oLev < 40 && nLev > 39))
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
        Stream st(REP::RANK_DATA);
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

        Stream st(REP::RANK_DATA);
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
            if(0 == m_Package->GetItemAnyNum(itemId))
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
            {
                act_form.erase(act_form.begin() + idx);
                cnt = act_form.size();
            }
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

        if(writedb && checkFormation_ID(newformationId))
        {
            SYSMSG_SENDV(2103, this);
            return false;
        }

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

        SYSMSG_SENDV(2104, this, newformation->getName().c_str());
		DB1().PushUpdateData("UPDATE `player` SET `formations` = '%s' WHERE id = %" I64_FMT "u", formations.c_str(), _id);

        Stream st(REP::RANK_DATA);
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

			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[5] = {{56, 2}, {503,10}, {514,10}, {507,2}, {509,2}};
                MailItemsInfo itemsInfo(mitem, FirstReChargeAward, 5);
                mailPackageManager.push(mail->id, mitem, 5, true);

                std::string strItems;
                for (int i = 0; i < 5; ++i)
                {
                    strItems += Itoa(mitem[i].id);
                    strItems += ",";
                    strItems += Itoa(mitem[i].count);
                    strItems += "|";
                }

                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
            }
		}

		sendVIPMails(oldVipLevel + 1, _vipLevel);

        UInt32 total = getBuffData(PLAYER_BUFF_YDOTR);
		if(World::_nationalDay)
        {
            if (total == (UInt32)(-1))
                return;

            UInt32 oldVipLevel = calcYDVipLevel(total);
            total += r;
            UInt32 vipLevel = calcYDVipLevel(total);
            sendYDVIPMails(oldVipLevel + 1, vipLevel);
            if (vipLevel == 6)
            {
                setBuffData(PLAYER_BUFF_YDOTR, static_cast<UInt32>(-1), true);
            }
            else
                setBuffData(PLAYER_BUFF_YDOTR, total, true);
        }
        else
        {
            if (total)
                setBuffData(PLAYER_BUFF_YDOTR, 0, true);
        }

        sendTripodInfo();
	}

	void Player::sendTopupMail(const char* title, const char* content, UInt32 gold, UInt8 num)
	{
		m_MailBox->newMail(NULL, 0x01, title, content);
		DBLOG1().PushUpdateData("insert into `topup_num`(`server_id`, `player_id`, `topup_gold`, `rand_num`, `topup_time`) values(%u, %"I64_FMT"u, %u, %u, %u)", cfg.serverLogId, getId(), gold, num, TimeUtil::Now());
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
        _playerData.nextRewardTime = GData::GDataManager::GetOnlineAwardTime(_playerData.rewardStep) + now;
		writeOnlineRewardToDB();
	}

	void Player::writeOnlineRewardToDB()
	{
		DB1().PushUpdateData("UPDATE `player` SET `nextReward` = '%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.rewardStep, _playerData.nextRewardItem, _playerData.nextRewardCount, _playerData.nextRewardTime, _id);
	}

	bool Player::takeOnlineReward()
	{
		UInt32 now = TimeUtil::Now();
		//if(_playerData.lastOnline + _playerData.nextRewardTime > now + 60)
		if(now < _playerData.nextRewardTime)
			return false;

        const std::vector<UInt16>& ids = GData::GDataManager::GetOnlineAward(GetClass(),  _playerData.rewardStep);
        UInt8 size = ids.size();
        if (!size)
            return false;

        for (UInt8 i = 0; i < size; i += 2)
        {
            if(!m_Package->Add(ids[i], ((UInt8)(i+1)>=ids.size())?1:ids[i+1], true, false, FromOnlineAward))
                // return false;
                ; // XXX: ugly
        }

        UInt8 count = GData::GDataManager::GetOnlineAwardCount();
        ++_playerData.rewardStep;
		if(_playerData.rewardStep >= count) {
			_playerData.rewardStep = count;

            Stream st(REP::REWARD_DRAW);
            st << static_cast<UInt16>(-1) << static_cast<UInt8>(0);
            st << Stream::eos;
            send(st);
            return true;
        }

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
        if (_playerData.nextRewardTime <= now)
            return 0;
        return _playerData.nextRewardTime - now;
	}

	void Player::sendOnlineReward()
	{
		Stream st(REP::REWARD_DRAW);
        UInt32 left = getOnlineReward();
        if (left == static_cast<UInt32>(-1))
        {
            st << static_cast<UInt16>(-1) << static_cast<UInt8>(0);
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
		Stream st(REP::DAILY_DATA);
		st << static_cast<UInt8>(1);
        UInt32 curtime = TimeUtil::Now();
		UInt32 vipLevel = getVipLevel();
        if (getShiMenMax() < _playerData.smFinishCount)
        {
            _playerData.smFinishCount = 0;
            writeShiMen();
        }
        if (getYaMenMax() < _playerData.ymFinishCount)
        {
            _playerData.ymFinishCount = 0;
            writeYaMen();
        }
        st << static_cast<UInt8>(getMaxIcCount(vipLevel) - getIcCount()) << static_cast<UInt8>(getShiMenMax() - _playerData.smFinishCount) << getShiMenMax() << static_cast<UInt8>(getYaMenMax() - _playerData.ymFinishCount) << getYaMenMax() << static_cast<UInt8>(CLAN_TASK_MAXCOUNT - _playerData.ctFinishCount);
        st << calcNextBookStoreUpdate(curtime) << calcNextTavernUpdate(curtime);
		//bossManager.buildInfo(st);
        UInt8 cnt = playerCopy.getCopySize(this);
        UInt8 freeCnt, goldCnt;
        playerCopy.getCount(this, &freeCnt, &goldCnt, true);
        st << cnt << static_cast<UInt8>(freeCnt + goldCnt) << static_cast<UInt8>(GObject::PlayerCopy::getFreeCount()) << static_cast<UInt8>(GObject::PlayerCopy::getGoldCount(vipLevel));
        if(cnt)
        {
            playerCopy.buildInfo(this, st);
        }

        UInt32 now = TimeUtil::Now();
        if(now >= _playerData.dungeonEnd)
        {
            _playerData.dungeonCnt = 0;
        }

        cnt = dungeonManager.size();
        st << cnt << _playerData.dungeonCnt << GObject::Dungeon::getMaxCount() << GObject::Dungeon::getExtraCount(vipLevel);
        if(cnt)
        {
            Dungeon_Enum de = {this, st};
            dungeonManager.enumerate(enum_dm, &de);
        }

        cnt = frontMap.getFrontMapSize(this);
        UInt8 fcnt = frontMap.getCount(this); // XXX: lock???
        st << cnt << static_cast<UInt8>(GObject::FrontMap::getFreeCount()+GObject::FrontMap::getGoldCount(vipLevel)-(((fcnt&0xf0)>>4)+(fcnt&0xf))) << static_cast<UInt8>(GObject::FrontMap::getFreeCount()) << static_cast<UInt8>(GObject::FrontMap::getGoldCount(vipLevel));
        if(cnt)
        {
            frontMap.buildInfo(this, st);
        }
#if 0
		size_t sz;
		UInt16 * prices = Dungeon::getPrice(sz);
		st << static_cast<UInt8>(sz);
		for(size_t i = 0; i < sz; ++ i)
			st << prices[i];
#endif
		st << Stream::eos;
		send((st));

        worldBoss.sendDaily(this);
	}

	void Player::regenAll(bool full)
	{
		for(int i = 0; i < 5; ++ i)
		{
			Lineup& pd = _playerData.lineup[i];
			if(pd.fighter != NULL && (pd.fighter->getCurrentHP() != 0 || full))
			{
                if (full)
                    pd.fighter->setCurrentHP(pd.fighter->getMaxHP());
                else
                    pd.fighter->setCurrentHP(0);
			}
		}
	}

    void Player::setHPPercent(UInt8 p)
    {
		for(int i = 0; i < 5; ++ i)
		{
			Lineup& pd = _playerData.lineup[i];
			if(pd.fighter != NULL)
			{
                pd.fighter->addHPPercent(p);
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
            if(p > GObjectManager::getMaxCapacity()/100 - 0.001)
                return 1;
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
            if(p > GObjectManager::getMaxPotential()/100 - 0.001)
                return 1;
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

		if(!m_Package->DelItemAny(itemId, 1, NULL, ToTrainFighter))
			return 2;
		
		if(uRand(1000) < rate)
		{
            if(isPotential)
            {
                p += 0.01f;
                p = floorf(p * 100.0f + 0.5f) / 100.0f;

                if(p > GObjectManager::getMaxPotential()/100)
                    p = GObjectManager::getMaxPotential()/100;
                fgt->setPotential(p);

                if (p >= 1.5f && p < 1.505f)
                {
                    if (isMainFighter(fgt->getId()))
                    {
                        SYSMSG_BROADCASTV(2200, getCountry(), getName().c_str(), fgt->getColor(), getName().c_str());
                    }
                    else
                    {
                        SYSMSG_BROADCASTV(2200, getCountry(), getName().c_str(), fgt->getColor(), fgt->getName().c_str());
                    }
                }
            }
            else
            {
                p += 0.1f;
                p = floorf(p * 10.0f + 0.5f) / 10.0f;

                if(p > GObjectManager::getMaxCapacity()/100)
                    p = GObjectManager::getMaxCapacity()/100;
                fgt->setCapacity(p);
                if (p >= 7.0f && p <= 7.05f)
                {
                    if (isMainFighter(fgt->getId()))
                    {
                        SYSMSG_BROADCASTV(2201, getCountry(), getName().c_str(), fgt->getColor(), getName().c_str());
                    }
                    else
                    {
                        SYSMSG_BROADCASTV(2201, getCountry(), getName().c_str(), fgt->getColor(), fgt->getName().c_str());
                    }
                }
            }
		}
		else
		{
			if(type == 1 || type == 3)
			{
                if(isPotential)
                {
                    float decp = fgt->getPotential() - 0.01f;
                    if (decp < static_cast<float>(GObjectManager::getMinPotential())/100)
                        decp = static_cast<float>(GObjectManager::getMinPotential())/100;
                    if (decp < fgt_orig->getPotential())
                        decp = fgt_orig->getPotential();
                    fgt->setPotential(decp);
                }
                else
                {
                    float decp = fgt->getCapacity() - 0.1f;
                    if (decp < static_cast<float>(GObjectManager::getMinCapacity())/100)
                        decp = static_cast<float>(GObjectManager::getMinCapacity())/100;
                    if (decp < fgt_orig->getCapacity())
                        decp = fgt_orig->getCapacity();
                    fgt->setCapacity(decp);
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
		DB1().PushUpdateDataL("UPDATE `player` SET `lastExp` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastExp, _id);
	}

	void Player::pendTael( UInt32 t )
	{
		_playerData.lastResource = (_playerData.lastResource & 0xFFFFFFFFFFFF0000ull) | ((_playerData.lastResource & 0xFFFFull) + t);
		DB1().PushUpdateData("UPDATE `player` SET `lastResource` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastResource, _id);
	}

	void Player::pendCoupon( UInt32 c )
	{
		_playerData.lastResource = (_playerData.lastResource & 0xFFFFFFFF0000FFFFull) | ((((_playerData.lastResource >> 16) + static_cast<UInt64>(c)) & 0xFFFFull) << 16);
		DB1().PushUpdateData("UPDATE `player` SET `lastResource` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastResource, _id);
	}

	void Player::pendCoin( UInt32 c )
	{
		_playerData.lastResource = (_playerData.lastResource & 0xFFF00000FFFFFFFFull) | ((((_playerData.lastResource >> 32) + static_cast<UInt64>(c)) & 0xFFFFFull) << 32);
		DB1().PushUpdateData("UPDATE `player` SET `lastResource` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastResource, _id);
	}

	void Player::pendAchievement( UInt32 a )
	{
		_playerData.lastResource = (_playerData.lastResource & 0x000FFFFFFFFFFFFFull) | (((_playerData.lastResource >> 52) + static_cast<UInt64>(a)) << 52);
		DB1().PushUpdateData("UPDATE `player` SET `lastResource` = %u WHERE `id` = %"I64_FMT"u", _playerData.lastResource, _id);
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

    void Player::setShiMenActiveCount(UInt8 c)
    {
        _shiMenActiveCount = c;
    }

    void Player::setYaMenActiveCount(UInt8 c)
    {
        _yaMenActiveCount = c;
    }

    void Player::setTavernBlueCount(UInt32 tavernBlueCount)
    {
        _tavernBlueCount = tavernBlueCount;
    }

    void Player::setTavernPurpleCount(UInt32 tavernPurpleCount)
    {
        _tavernPurpleCount = tavernPurpleCount;
    }

    void Player::setTavernOrangeCount(UInt32 tavernOrangeCount)
    {
        _tavernOrangeCount = tavernOrangeCount;
    }

	void Player::setNextExtraReward( UInt32 ner )
	{
		_playerData.nextExtraReward = ner;
		DB1().PushUpdateData("UPDATE `player` SET `nextExtraReward` = %u WHERE `id` = %"I64_FMT"u", _playerData.nextExtraReward, _id);
	}

	bool Player::isDungeonPassed( UInt8 id )
	{
		Dungeon * dg = dungeonManager[id];
		if(dg == NULL)
			return false;
		return dg->getFirstPass(this) > 0;
	}

#if 0
	void Player::sendFriendActList()
	{
		size_t sz = _friendActs.size();
		Stream st(REP::FRIEND_STATUS);
		st << static_cast<UInt8>(sz);
		for(size_t i = 0; i < sz; ++ i)
		{
			appendFriendAct(st, _friendActs[i]);
		}
		st << Stream::eos;
		send(st);
	}
#endif

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

#if 0
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
			Stream st(REP::FRIEND_STATUS);
			st << static_cast<UInt8>(0xFF);
			appendFriendAct(st, fas);
			st << Stream::eos;
			send(st);
		}
	}
#endif

	bool Player::testCanAddFriend( Player * pl )
	{
		Mutex::ScopedLock lk(_mutex);
		Mutex::ScopedLock lk2(pl->getMutex());
		if(isFriendFull())
		{
			sendMsgCode(2, 1503);
			return false;
		}
		if(_hasFriend(pl))
		{
			return false;
		}
		if(_hasBlock(pl))
		{
			sendMsgCode(2, 1500);
			return false;
		}
#if 0
		if(_hasFoe(pl))
		{
			sendMsgCode(2, 1500);
			return false;
		}
#endif
		if(pl->isFriendFull())
		{
			sendMsgCode(2, 1504);
			return false;
		}
		if(pl->_hasBlock(this))
		{
			sendMsgCode(2, 1502);
			return false;
		}
#if 0
		if(pl->_hasFoe(this))
		{
			sendMsgCode(2, 1500);
			return false;
		}
#endif
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

	void Player::sendYDVIPMails( UInt8 l, UInt8 h )
	{
		if(l < 1)
			l = 1;
		if(h > 6)
			h = 6;

		for(UInt32 j = l; j <= h; ++j)
		{
			SYSMSG(title, 2110);
			SYSMSGV(content, 2111);
			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
			if(mail == NULL)
                return;

			MailPackage::MailItem mitem[2] = {{0,0},};
			UInt32 mcount = 0;
            std::string strItems;

			const UInt32 vipTable[7][4] =
            {
                {56,2,0,0},
                {57,3,0,0},
                {GObject::MailPackage::Coupon,1000,0,0}, // --
                {502,10,503,20},
                {515,20,0,0},
                {509,20,507,20},
                {0,0,0,0},
            };

			const UInt32 * t = vipTable[j-1];
			for(UInt32 i = 0; i < 4 && t[i] > 0; i += 2)
			{
				mitem[mcount].id = t[i];
				mitem[mcount++].count = t[i+1];
				strItems += Itoa(t[i]);
				strItems += ",";
				strItems += Itoa(t[i+1]);
				strItems += "|";
			}

			mailPackageManager.push(mail->id, mitem, mcount, true);
			DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
        }
    }

	void Player::sendVIPMails( UInt8 l, UInt8 h )
	{
		if(l < 1)
			l = 1;
		if(h > 10)
			h = 10;
		for(UInt32 j = l; j <= h; ++j)
		{
			SYSMSG(title, 256);
			SYSMSGV(content, 257, j);
			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
			if(mail == NULL)
				continue;

			const UInt32 vipTable[11][12] =
            {
                {56,2,0,0,0,0,0,0,0,0,0,0},
                {56,10,0,0,0,0,0,0,0,0,0,0},
                {56,10,503,5,0,0,0,0,0,0,0,0},
                {56,10,503,10,514,10,0,0,0,0,0,0},
                {503,20,515,5,507,2,509,2,0,0,0,0},
                {503,30,515,15,507,10,509,10,0,0,0,0},
                {503,30,515,20,507,10,509,10,0,0,0,0},
                {503,50,515,30,507,30,509,30,0,0,0,0},
                {503,100,515,30,507,30,509,30,0,0,0,0},
                {503,200,515,50,507,50,509,50,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0},
            };

			MailPackage::MailItem mitem[6];
			UInt32 mcount = 0;
            std::string strItems;

			const UInt32 * t = vipTable[j-1];
			for(UInt32 i = 0; i < 12 && t[i] > 0; i += 2)
			{
				mitem[mcount].id = t[i];
				mitem[mcount++].count = t[i+1];
				strItems += Itoa(t[i]);
				strItems += ",";
				strItems += Itoa(t[i+1]);
				strItems += "|";
			}

            UInt16 equips[][24] = {
            {2544, 2545, 2546, 2547, 2548, 2549, 2550, 2551, 2552, 2553, 2554, 2555, 2556, 2557, 2558, 2559, 2560, 2561, 2562, 2563, 2564, 2565, 2566, 2567},
            {2568, 2569, 2570, 2571, 2572, 2573, 2574, 2575, 2576, 2577, 2578, 2579, 2580, 2581, 2582, 2583, 2584, 2585, 2586, 2587, 2588, 2589, 2590, 2591},
            {2592, 2593, 2594, 2595, 2596, 2597, 2598, 2599, 2600, 2601, 2602, 2603, 2604, 2605, 2606, 2607, 2608, 2609, 2610, 2611, 2612, 2613, 2614, 2615},
            {2592, 2593, 2594, 2595, 2596, 2597, 2598, 2599, 2600, 2601, 2602, 2603, 2604, 2605, 2606, 2607, 2608, 2609, 2610, 2611, 2612, 2613, 2614, 2615},
            {2592, 2593, 2594, 2595, 2596, 2597, 2598, 2599, 2600, 2601, 2602, 2603, 2604, 2605, 2606, 2607, 2608, 2609, 2610, 2611, 2612, 2613, 2614, 2615},
            {2592, 2593, 2594, 2595, 2596, 2597, 2598, 2599, 2600, 2601, 2602, 2603, 2604, 2605, 2606, 2607, 2608, 2609, 2610, 2611, 2612, 2613, 2614, 2615},
            };

            UInt8 lvl = GetLev();
            if (lvl < 50)
                lvl = 50;
            lvl -= 50;
            lvl /= 10;

            if (j >= 5) // XXX: 玩家等级橙色装备x1
            {
                URandom tmpRand;
                UInt16 id = equips[lvl][tmpRand(24)];
                mitem[mcount].id = id;
                mitem[mcount++].count = 1;
				strItems += Itoa(id);
				strItems += ",";
				strItems += Itoa(1);
            }

			mailPackageManager.push(mail->id, mitem, mcount, true);
			DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
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
			DB1().PushUpdateData("REPLACE INTO `exchange_ticket` VALUES(%"I64_FMT"u, %u)", getId(), _exchangeTicketCount);
			DBLOG1().PushUpdateData("replace into `exchange_ticket` VALUES(%u, %"I64_FMT"u, %u)", cfg.serverLogId, getId(), _exchangeTicketCount);
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
			DB1().PushUpdateData("UPDATE `player` SET `bossLevel` = %u WHERE id = %"I64_FMT"u", _bossLevel, getId());
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
			sendMsgCode(0, 1011);
			return false;
		}
		UInt32 now = TimeUtil::Now();
		UInt32 buffLeft = getBuffData(PLAYER_BUFF_ATTACKING, now);
		if(buffLeft > now)
		{
			sendMsgCode(0, 1407, buffLeft - now);
			return false;
		}

		Battle::BattleSimulator bsim(_playerData.location, this, _ng->getName(), static_cast<UInt8>(_bossLevel));

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
		Stream st(REP::ATTACK_NPC);
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
		DB1().PushUpdateData("UPDATE `player` SET `bookStore` = '%u|%u|%u|%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", _playerData.bookStore[0], _playerData.bookStore[1], _playerData.bookStore[2], _playerData.bookStore[3], _playerData.bookStore[4], _playerData.bookStore[5], _nextBookStoreUpdate, _id);
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
        Stream st(REP::BOOK_SHOP_LIST);
        if (GetLev() < 10 && !type && GetTaskMgr()->HasCompletedTask(5) && !GetTaskMgr()->HasSubmitedTask(5)) {
            st << static_cast<UInt16>(0);
            _playerData.bookStore[0] = 1200;
            _playerData.bookStore[1] = 1201;
            _playerData.bookStore[2] = 1204;
            _playerData.bookStore[3] = 1205;
            _playerData.bookStore[4] = 1207;
            _playerData.bookStore[5] = 1208;
        } else {
            UInt32 curtime = TimeUtil::Now();
            UInt16 money = 0;
            int count = 0;

            if(type == 1)
            {
                if(_nextBookStoreUpdate == 0 || curtime >= _nextBookStoreUpdate)
                {
                    updateNextBookStoreUpdate(curtime);
                }
                else
                {
                    money = GData::moneyNeed[GData::BOOK_LIST].tael;
                }

                count = 1;
                // updateNextBookStoreUpdate(curtime);
            }

            if(type > 0 && _playerData.tael < money)
            {
                sendMsgCode(1, 1100);
                return;
            }

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
			sendMsgCode(0, 1100);
			return 0;
		}
		if(!m_Package->AddItem(iid, 1, true))
		{
			sendMsgCode(2, 1011);
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
		mysql_escape_string(questionTmp, _pwdInfo.questionForPWD.c_str(), _pwdInfo.questionForPWD.length()>255?255:_pwdInfo.questionForPWD.length());
		mysql_escape_string(answerTmp, _pwdInfo.answerForPWD.c_str(), _pwdInfo.answerForPWD.length()>255?255:_pwdInfo.answerForPWD.length());
		DB1().PushUpdateData("INSERT INTO `pass_word` VALUES(%"I64_FMT"u, '%s', '%s', '%s')", _id, _pwdInfo.secondPWD.c_str(), questionTmp, answerTmp);
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
		DB1().PushUpdateData("DELETE FROM `pass_word` WHERE `playerId` = %"I64_FMT"u", _id);
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
		DB1().PushUpdateData("UPDATE `pass_word` SET `password` = '%s' WHERE `playerId` =  %"I64_FMT"u", _pwdInfo.secondPWD.c_str(), _id);
		return 0;
	}

	UInt8 Player::changeSecondPWD(std::string oldPWD, std::string newPWD)
	{
		if(oldPWD != _pwdInfo.secondPWD)
			return 1;
		if(_pwdInfo.secondPWD == newPWD)
			return 0;
		_pwdInfo.secondPWD = newPWD;
		DB1().PushUpdateData("UPDATE `pass_word` SET `password` = '%s' WHERE `playerId` =  %"I64_FMT"u", _pwdInfo.secondPWD.c_str(), _id);
		return 0;
	}

	void Player::makeSenconPWDInfo(Stream& st)
	{
		st.init(REP::SECOND_PWD);
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
			Stream st(REP::PWD_DAILOG);
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
#if 0
        if(getBuffData(PLAYER_BUFF_PRACTICE1, TimeUtil::Now()))
        {
            return 0.2;
        }
        if(getBuffData(PLAYER_BUFF_PRACTICE2, TimeUtil::Now()))
        {
            return 0.5;
        }
#else
        if(getBuffData(PLAYER_BUFF_PROTECT, TimeUtil::Now()))
        {
            return 0.2;
        }
#endif
        return 0.0;
    }

    bool Player::accPractice()
    {
        UInt32 goldUse = GData::moneyNeed[GData::INSTANTPRACTICE].gold;
        if(getGold() < goldUse)
        {
            Stream st(REP::PRACTICE_HOOK_ADD);
            st << static_cast<UInt8>(1) << Stream::eos;
            send(st);
            return false;
        }

        GameMsgHdr hdr1(0x17D, WORKER_THREAD_WORLD, this, 0);
        GLOBAL().PushMsg(hdr1, NULL);

        return true;
    }

    bool Player::OperationTaskAction(int type)
    {
        switch (type)
        {
            case 0:
                return GameAction()->RunOperationTaskAction0(this, 6);
                break;
            default:
                break;
        }
        return true;
    }

    void Player::setCountry(UInt8 cny)
    { 
        _playerData.country = cny;
		DB1().PushUpdateData("UPDATE `player` SET `country` = %u WHERE `id` = %"I64_FMT"u", cny, getId());

		Stream st(REP::USER_INFO_CHANGE);
		st << static_cast<UInt8>(0x11) << static_cast<UInt32>(cny) << Stream::eos;
		send(st);
    }

    void Player::startAutoCopy(UInt8 id)
    {
        playerCopy.autoBattle(this, id, 0);
    }

    void Player::cancelAutoCopy(UInt8 id)
    {
        playerCopy.autoBattle(this, id, 1);
    }

    void Player::instantAutoCopy(UInt8 id)
    {
        playerCopy.autoBattle(this, id, 2);
    }

    void Player::sendAutoCopy()
    {
        playerCopy.sendAutoCopy(this);
    }

    void Player::AddPracticeExp(const PracticeFighterExp* pfexp)
    {
        if(!pfexp)
            return;

        if(pfexp->goldUse)
        {
            if(getGold() < pfexp->goldUse)
            {
                Stream st(REP::PRACTICE_HOOK_ADD);
                st << static_cast<UInt8>(1) << Stream::eos;
                send(st);
                return;
            }
            else
            {
                Stream st(REP::PRACTICE_HOOK_ADD);
                st << static_cast<UInt8>(0) << Stream::eos;
                send(st);
            }
            ConsumeInfo ci(InstantPracticeAcc,0,0);
            useGold(pfexp->goldUse,&ci);
        }

        for(int i = 0; i < MAX_PRACTICE_FIGHTRES; ++ i)
        {
            Fighter* fgt = findFighter(pfexp->fids[i]);
            if(fgt && pfexp->counts[i])
            {
                fgt->addPExp(fgt->getPracticeInc() * pfexp->counts[i]); 
            }
        }

    }

    void Player::RollYDGem()
    {
        std::vector<UInt32> ydGem = GObjectManager::getYDGem();
        _playerData.ydGemId = ydGem[uRand(ydGem.size())];
		DB1().PushUpdateData("UPDATE `player` SET `ydgemid` = %u WHERE `id` = %"I64_FMT"u", _playerData.ydGemId, getId());
    }

    void Player::checkQQAward()
    {
		UInt32 now = TimeUtil::Now();

        UInt8 qqvipl = _playerData.qqvipl;
        UInt8 flag = 8*(_playerData.qqvipl / 10);
        if(flag)
        {
            if(_playerData.qqvipl % 10 == 0)
                qqvipl = 0;
            else
                qqvipl = _playerData.qqvipl%10 + 1;
        }

		if(now >= _playerData.qqawardEnd)
		{
			_playerData.qqawardEnd = TimeUtil::SharpDay(1, now);
            _playerData.qqawardgot &= 0xFCFC;
            DB1().PushUpdateData("UPDATE `player` SET `qqawardEnd` = %u, `qqawardgot` = %u WHERE `id` = %"I64_FMT"u", _playerData.qqawardEnd, _playerData.qqawardgot, getId());
            RollYDGem();
        }

        if( !(_playerData.qqawardgot & (0x80<<flag)) && qqvipl )
        {
            if(GetFreePackageSize() < 1)
            {
                sendMsgCode(2, 1011);
            }
            else
            {
                _playerData.qqawardgot |= (0x80<<flag);
                if(flag)
                    GetPackage()->AddItem2(7, 1, true, true);
                else
                    GetPackage()->AddItem2(67, 1, true, true);
                DB1().PushUpdateData("UPDATE `player` SET `qqawardgot` = %u WHERE `id` = %"I64_FMT"u", _playerData.qqawardgot, getId());
            }
        }

        if(_playerData.ydGemId == 0)
        {
            RollYDGem();
        }
    }

    void Player::sendYellowDiamondInfo()
    {
        checkQQAward();

        UInt8 qqvipl = _playerData.qqvipl % 10;
        UInt8 flag = 8*(_playerData.qqvipl / 10);

        Stream st(REP::YD_INFO);
        st << qqvipl << _playerData.qqvipyear << static_cast<UInt8>((_playerData.qqawardgot>>flag) & 0x03);
        UInt8 maxCnt = GObjectManager::getYDMaxCount();
        if(flag)
            st << static_cast<UInt8>(maxCnt - 2);
        else
            st << maxCnt;
        for(UInt8 i = 0; i < maxCnt; ++ i)
        {
            if(flag && (i == 0 || i > 6))
                continue;
            std::vector<YDItem>& ydItem = GObjectManager::getYDItem(i);
            UInt8 itemCnt = ydItem.size();
            st << itemCnt;
            for(int j = 0; j < itemCnt; ++ j)
            {
                UInt32 itemId = ydItem[j].itemId;
                if(GetItemSubClass(itemId) == Item_Gem)
                    itemId = _playerData.ydGemId;

                st << itemId << ydItem[j].itemNum;
            }
        }

        std::vector<YDItem>& ydItem = GObjectManager::getYearYDItem();
        UInt8 itemCnt = ydItem.size();
        st << itemCnt;
        for(UInt8 j = 0; j < itemCnt; ++ j)
            st << ydItem[j].itemId << ydItem[j].itemNum;

        st << Stream::eos;
        send(st);
    }

    UInt8 Player::rcvYellowDiamondAward(UInt8 type)
    {
        UInt8 nRes = 0;

        Stream st(REP::YD_AWARD_RCV);
        checkQQAward();

        UInt8 qqvipl = _playerData.qqvipl;
        UInt8 flag = 8*(_playerData.qqvipl / 10);
        if(flag)
        {
            if(_playerData.qqvipl % 10 == 0)
                qqvipl = 0;
            else
                qqvipl = _playerData.qqvipl%10 + 1;
        }

        if(type == 1 && !(_playerData.qqawardgot & (0x1<<flag)) && qqvipl != 0)
        {
            std::vector<YDItem>& ydItem = GObjectManager::getYDItem(qqvipl - 1);
            UInt8 itemCnt = ydItem.size();
            if(GetPackage()->GetRestPackageSize() > ydItem.size() - 1)
            {
                 nRes = 1;
                _playerData.qqawardgot |= (0x1<<flag);
                for(int j = 0; j < itemCnt; ++ j)
                {
                    UInt32 itemId = ydItem[j].itemId;
                    if(GetItemSubClass(itemId) == Item_Gem)
                        itemId = _playerData.ydGemId;

                    GetPackage()->AddItem2(itemId, ydItem[j].itemNum, true, true);
                }
            }
            else
            {
                sendMsgCode(2, 1011);
            }
        }
        else if(type == 2 && !(_playerData.qqawardgot & (0x2<<flag)) && _playerData.qqvipyear != 0)
        {
            std::vector<YDItem>& ydItem = GObjectManager::getYearYDItem();
            UInt8 itemCnt = ydItem.size();
            if(GetPackage()->GetRestPackageSize() > ydItem.size() - 1)
            {
                nRes = 2;
                _playerData.qqawardgot |= (0x2<<flag);

                for(int j = 0; j < itemCnt; ++ j)
                    GetPackage()->AddItem2(ydItem[j].itemId, ydItem[j].itemNum, true, true);
            }
            else
            {
                sendMsgCode(2, 1011);
            }
        }

        if(nRes)
        {
            DB1().PushUpdateData("UPDATE `player` SET `qqawardgot` = %u WHERE `id` = %"I64_FMT"u", _playerData.qqawardgot, getId());
        }

        st << nRes << Stream::eos;
        send(st);

        return nRes;
    }

	void Player::incPIcCount( )
	{
        checkPIcCount();
		++ _playerData.picCount;
        DB1().PushUpdateData("UPDATE `player` SET piccount = %u, nextpicreset = %u where `id`= %"I64_FMT"u", _playerData.picCount, _playerData.nextPIcReset, _id);
	}


    UInt8 Player::getPIcCount()
    {
        checkPIcCount();
        int nVipLevel = getVipLevel();
        return PracticePlace::_picCnt[nVipLevel] - _playerData.picCount;
    }

    void Player::checkPIcCount()
    {
		UInt32 now = TimeUtil::Now();
		if(now >= _playerData.nextPIcReset)
		{
			_playerData.nextPIcReset = TimeUtil::SharpDay(1, now);
            _playerData.picCount = 0;
            DB1().PushUpdateData("UPDATE `player` SET piccount = %u, nextpicreset = %u where `id`= %"I64_FMT"u", _playerData.picCount, _playerData.nextPIcReset, _id);
		}
    }

    void Player::setLineupDirty(bool dirty)
    {
		for(int i = 0; i < 5; ++ i)
		{
			GObject::Fighter * fgt = getLineup(i).fighter;
			if(fgt != NULL)
				fgt->setDirty(dirty);
		}
    }

	void Player::setFightersDirty( bool bDirty )
	{
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
        {
            it->second->setDirty(bDirty);
        }

		return;
	}

    bool Player::enchanted8( UInt32 id )
    {
        if (!id) return false;
        size_t sz = _enchantEqus.size();
        for (size_t i = 0; i < sz; ++i)
        {
            if (id == _enchantEqus[i])
                return true;
        }
        _enchantEqus.push_back(id);
        return false;
    }

    void Player::sendEnchanted8Box()
    {
        SYSMSG(title, 2126);
        SYSMSG(content, 2127);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem mitem[2] = {{507,5}, {509,5}};
            mailPackageManager.push(mail->id, mitem, 2, true);

            std::string strItems;
            strItems += Itoa(mitem[0].id);
            strItems += ",";
            strItems += Itoa(mitem[0].count);
            strItems += "|";
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
        }
    }

    void Player::sendTripodInfo()
    {
        if (!m_hasTripod)
            newTripodData();

        if (getVipLevel() > 2 && m_td.quality < 3)
        {
            m_td.quality = 3;
            m_td.needgen = 1;
        }

        Stream st(REP::TRIPOD_INFO);
        st << static_cast<UInt8>(0);
        st << m_td.fire;
        st << m_td.quality;

        genAward(st);
        DB6().PushUpdateData("UPDATE `tripod` SET `regen` = %u, `quality` = %u, `itemId` = %u, `num` = %u WHERE `id` = %"I64_FMT"u",
                m_td.needgen, m_td.quality, m_td.itemId, m_td.num, getId());

        st << static_cast<UInt32>(MAX_TRIPOD_SOUL) << m_td.soul << Stream::eos;
        send(st);
    }

    bool Player::genAward()
    {
        if (m_td.needgen) {
            UInt32 loot = GData::GDataManager::GetTripodAward(m_td.fire, 5-m_td.quality); // 0-橙,1-紫,2-蓝,3-绿
            const GData::LootItem* li = GData::lootTable[loot];
            if (li)
            {
                GData::LootResult lr = li->roll();
                if (lr.id)
                {
                    m_td.itemId = lr.id;
                    m_td.num = lr.count;
                    m_td.needgen = 0;
                    return true;
                }
            }
            return false;
        }
        return true;
    }

    void Player::genAward(Stream& st)
    {
        if (genAward()) {
            st << m_td.num;
            st << m_td.itemId;
        } else {
            st << static_cast<UInt8>(0);
            st << static_cast<UInt32>(0);
        }
        return;
    }

    static UInt8 tripod_factor[4][4] =
    {
        {30,    0,      0,      0},
        {50,    70,     0,      0},
        {0,     50,     100,    0},
        {0,     0,      30,     100}
    };

    void Player::addItem(UInt32 itemid, UInt16 num, UInt8 bind)
    {
        if (!itemid || !num)
            return;

        Package* pk = GetPackage();
        if (!pk) return;

        ItemBase* ib = NULL;
        ib = pk->FindItem(itemid, bind);
        if (ib)
        {
            if (ib->Count() < num)
                return;

            m_td.soul += (ib->getEnergy() * num);
            UInt8 quality = ib->getQuality() > 1 ? ib->getQuality() - 2 : 0;
            for (UInt16 j = 0; j < num; ++j)
            {
                int rnd = uRand(100);
                for (int i = 0; i < 4; ++i)
                {
                    if (tripod_factor[quality][i] && rnd <= tripod_factor[quality][i])
                    {
                        if (m_td.quality < i+2)
                        {
                            m_td.quality = i+2; // 2-绿,3-蓝,4-紫,5-橙
                            m_td.needgen = 1;
                        }
                        break;
                    }
                }
            }

            if (getVipLevel() >= 3)
            {
                if (m_td.quality < 3)
                {
                    m_td.quality = 3;
                    m_td.needgen = 1;
                }
            }

            if (m_td.quality > 5)
            {
                m_td.quality = 5;
                m_td.needgen = 1;
            }

            if (IsEquipId(itemid))
            {
                pk->DelEquip(itemid, ToDesdroy);
            }
            else
            {
                pk->DelItem(itemid, num, bind);
            }
        }

        {
            Stream st(REP::TRIPOD_INFO);
            st << static_cast<UInt8>(2);
            st << static_cast<UInt8>(m_td.needgen);
            st << Stream::eos;
            send(st);
        }

        if (m_td.needgen)
            genAward();

        if (m_td.soul >= MAX_TRIPOD_SOUL)
        {
            PopTimerEvent(this, EVENT_PLAYERPRTRIPOD, getId());
            m_td.soul = MAX_TRIPOD_SOUL;
            m_td.awdst = 1;
            m_td.needgen = 0;
        }

        DB6().PushUpdateData("UPDATE `tripod` SET `soul` = %u, `quality` = %u, `awdst` = %u, `regen` = %u, `itemId` = %u, `num` = %u WHERE `id` = %"I64_FMT"u",
                m_td.soul, m_td.quality, m_td.awdst, m_td.needgen, m_td.itemId, m_td.num, getId());
    }

    static UInt16 fire_begin = 47;
    static UInt16 fire_end = 51;
    static UInt8 fire_id2bit[] = {16/*47*/, 8/*48*/, 4/*49*/, 2/*50*/, 1/*51*/};
    static UInt8 fire_com[] = {24,20,18,17,12,10,9,6,5,3};
    static UInt8 fire_factor[][6] = 
    {
        {0,     0,      100,    0,      0,      0},
        {0,     0,      0,      0,      100,    0},
        {0,     0,      0,      0,      0,      100},
        {0,     0,      0,      100,    0,      0},
        {0,     80,     100,    0,      0,      0},
        {0,     50,     100,    0,      0,      0},
        {40,    90,     0,      100,    0,      0},
        {0,     50,     0,      0,      100,    0},
        {80,    0,      0,      100,    0,      0},
        {50,    0,      0,      100,    0,      0},
    };

    void Player::makeFire(UInt32 id1, UInt32 id2)
    {
        Stream st(REP::TRIPOD_INFO);
        if (id1 < fire_begin)
            id1 = fire_begin;
        if (id2 < fire_begin)
            id2 = fire_begin;
        if (id1 > fire_end)
            id1 = fire_end;
        if (id2 > fire_end)
            id2 = fire_end;

        ItemBase* ib1 = GetPackage()->GetItem(id1, true);
        if (!ib1)
            ib1 = GetPackage()->GetItem(id1);
        if (!ib1) {
            SYSMSG_SEND(2003, this);
            return;
        }
        ItemBase* ib2 = GetPackage()->GetItem(id2, true);
        if (!ib2)
            ib2 = GetPackage()->GetItem(id2);
        if (!ib2) {
            SYSMSG_SEND(2003, this);
            return;
        }

        UInt32 id = fire_id2bit[id1-fire_begin] | fire_id2bit[id2-fire_begin];
        int i = 0;
        while (i < 10)
        {
            if (id == fire_com[i])
                break;
            ++i;
        }
        UInt8 rnd = uRand(100);
        int j = 0;
        while (j < 6)
        {
            if (fire_factor[i][j] && rnd <= fire_factor[i][j])
                break;
            ++j;
        }
        if (j < 6)
            m_td.fire = j+1;

        m_td.needgen = 1;

        st << static_cast<UInt8>(1);
        st << m_td.fire;
        st << m_td.quality;
        genAward(st);
        st << Stream::eos;
        send(st);
        DB6().PushUpdateData("UPDATE `tripod` SET `fire` = %u, `regen` = %u, `itemId` = %u, `num` = %u WHERE `id` = %"I64_FMT"u",
                m_td.fire, m_td.needgen, m_td.itemId, m_td.num, getId());

        GetPackage()->DelItem2(ib1, 1);
        GetPackage()->DelItem2(ib2, 1);
        SYSMSG_SEND(2002, this);
    }

    void Player::getAward()
    {
        if (GetPackage()->IsFull())
        {
            sendMsgCode(0, 1011);
            return;
        }

        if (m_td.awdst != 1)
            return;

        if (!genAward())
            return;

        if (IsEquipTypeId(m_td.itemId))
            GetPackage()->AddEquip(m_td.itemId, true, false, FromTripod);
        else
            GetPackage()->AddItem(m_td.itemId, m_td.num, true, false, FromTripod);

        m_td.fire = 0;
        m_td.quality = 2;
        m_td.needgen = 1;
        m_td.awdst = 0;
        m_td.soul = 0;
        m_td.itemId = 0;
        m_td.num = 0;
        DB6().PushUpdateData("UPDATE `tripod` SET `soul`=0, `fire`=0, `quality`=2, `awdst`=0, `itemId`=0, `num`=0, `regen`=1 WHERE `id` = %"I64_FMT"u", getId());
        runTripodData(m_td);
        sendTripodInfo();
    }

    TripodData& Player::runTripodData(TripodData& data, bool init)
    {
        if(getVipLevel() > 2)
            m_td.quality = 3;

        if (&data != &m_td)
            m_td = data;

        EventPlayerTripod* event = new (std::nothrow) EventPlayerTripod(this, 60, MAX_TRIPOD_SOUL/POINT_PERMIN);
        if (!event) return nulltd;
        PushTimerEvent(event);

        if (init && m_td.itemId && m_td.num)
            m_td.needgen = 0;
        DB6().PushUpdateData("UPDATE `tripod` SET `regen` = 0, `quality` = %u WHERE `id` = %"I64_FMT"u", m_td.quality, getId());
        m_hasTripod = true;
        return m_td;
    }

    TripodData& Player::newTripodData()
    {
        DB6().PushUpdateData("REPLACE INTO `tripod`(`id`, `soul`, `fire`, `quality`, `awdst`, `regen`, `itemId`, `num`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u, %u,%u)" , getId(), m_td.soul, m_td.fire, m_td.quality, m_td.awdst, m_td.needgen, m_td.itemId, m_td.num);
        return runTripodData(m_td);
    }

    // XXX: 光棍节强化光棍补偿
    void Player::sendSingleEnchant(UInt8 enchant)
    {
        if (enchant && enchant <= 10)
        {
            MailPackage::MailItem item1[3] = {{514, 1}, {507, 5},{509, 5}};
            MailPackage::MailItem item2[3] = {{514, 2}, {507, 5},{509, 5}};
            MailPackage::MailItem item3[3] = {{514, 3}, {507, 5},{509, 5}};
            MailPackage::MailItem item4[3] = {{514, 5}, {507, 5},{509, 5}};
            MailPackage::MailItem item5[3] = {{514, 10},{507, 5},{509, 5}};
            MailPackage::MailItem item6[3] = {{515, 10},{507, 5},{509, 5}};
            MailPackage::MailItem item7[3] = {{515, 20},{507, 5},{509, 5}};
            MailPackage::MailItem item8[3] = {{515, 30},{507, 5},{509, 5}};

            MailPackage::MailItem* item[8] = {item1,item2,item3,item4,item5,item6,item7,item8};

            sendMailItem(2205, 2206, item[enchant-1], 3);
        }
    }

} // namespace GObject


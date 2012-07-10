#include "Config.h"
#include "Server/WorldServer.h"
#include "MsgID.h"
#include "Player.h"
#include "Package.h"
#include "TaskMgr.h"
#include "AttainMgr.h"
#include "ActivityMgr.h"
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include "CFriend.h"
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
#include "ClanRankBattle.h"
#include "TeamCopy.h"
#include "HoneyFall.h"
#include "GData/ClanTechTable.h"
#include "GData/ClanLvlTable.h"
#include "GData/ClanSkillTable.h"
#include "Common/StringTokenizer.h"
#include "TownDeamon.h"
#include "ArenaBattle.h"
#ifdef _ARENA_SERVER
#include "GameServer.h"
#endif
#include "GData/Store.h"
#include "RealItemAward.h"

#include <cmath>

#define NTD_ONLINE_TIME (4*60*60)
#ifndef _DEBUG
#define TGD_ONLINE_TIME (3*60*60)
#else
#define TGD_ONLINE_TIME (10*60)
#endif

#define CLAN_SKILL_ATTACK   1
#define CLAN_SKILL_DEFEND   2
#define CLAN_SKILL_MAGATK   3
#define CLAN_SKILL_MAGDEF   4
#define CLAN_SKILL_HP       5
#define CLAN_SKILL_ACTION   6
#define CLAN_SKILL_HITRATE  7
#define CLAN_SKILL_MAXSOUL  8

#define DAY_SECS (24*60*60)
#define CREATE_OFFSET(c, n) (((n) - (c)) / (DAY_SECS))

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
	const UInt8 MaxICCount[] = {8, 16, 16, 16, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24};
	const UInt16 MAX_EXTEND_TIME	= 17;
	const UInt16 EACH_EXTEND_NUM	= 50;
	GlobalPlayers globalPlayers;
	GlobalPlayers newPlayers;
	GlobalNamedPlayers globalNamedPlayers;
	ChallengeCheck challengeCheck;
    GlobalLevelsPlayers globalLevelsPlayers;
    static TripodData nulltd;

	UInt8 Player::getMaxIcCount(UInt8 vipLevel)
	{
        if (World::getICAct())
            return MaxICCount[5];
		UInt8 maxCount = MaxICCount[vipLevel];
		return maxCount;
	}

	float EventAutoBattle::calcExpEach(UInt32 now)
	{
        return 8.0f * _npcGroup->getExp();
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
        UInt8 iccnt = Player::getMaxIcCount(vipLevel) - m_Player->getIcCount();
        if (Player::getMaxIcCount(vipLevel) < m_Player->getIcCount())
            iccnt = Player::getMaxIcCount(vipLevel);
		if(cnt > 0)
		{
			if(_npcGroup == NULL)
				return;
			UInt32 t = TimeUtil::Now();
			if(t > _finalEnd) t = 0; else t = _finalEnd - t;
			st << _npcGroup->getId() << static_cast<UInt8>(1) << cnt << t << iccnt << Stream::eos;
		}
		else
		{
			st << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt32>(0) << iccnt << Stream::eos;
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

        GameMsgHdr hdr1(0x1F6, WORKER_THREAD_WORLD, m_Player, 0);
        GLOBAL().PushMsg(hdr1, NULL);

		data->checktime -= 10;
        if ((int)data->checktime < 0)
            data->checktime = 0;
		if(leftCount == 0 || data->checktime == 0)
		{
            DB().PushUpdateData("UPDATE `practice_data` SET `checktime` = %u, `place` = %u, `slot` = %u, winnerid = %"I64_FMT"u, fighters = '' WHERE `id` = %"I64_FMT"u", data->checktime, PPLACE_MAX, 0, 0, m_Player->getId());
            GameMsgHdr hdr1(0x1F7, WORKER_THREAD_WORLD, m_Player, 0);
            GLOBAL().PushMsg(hdr1, NULL);
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

            Fighter* fgt = 0;
            UInt8 n = 0;
            for (auto i = data->fighters.begin(), e = data->fighters.end(); i != e; ++i)
            {
                fgt = m_Player->findFighter(*i);
                if (fgt)
                {
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

            stActivityMsg msg;
            msg.id = AtyPSpeed;
            GameMsgHdr hdr2(0x245, m_Player->getThreadId(), m_Player, sizeof(stActivityMsg));
            GLOBAL().PushMsg(hdr2, &msg);

            m_Player->incPIcCount();
            data->checktime -= count;
            if ((int)data->checktime < 0)
                data->checktime = 0;
            if(data->checktime == 0)
            {
                DB().PushUpdateData("UPDATE `practice_data` SET `checktime` = %u, `place` = %u, `slot` = %u, winnerid = %"I64_FMT"u, fighters = '' WHERE `id` = %"I64_FMT"u", data->checktime, PPLACE_MAX, 0, 0, m_Player->getId());
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

    bool EventAutoFrontMap::Equal(UInt32 id, size_t playerid) const
    {
		return 	id == GetID() && playerid == m_Player->getId();
    }

    void EventAutoFrontMap::Process(UInt32 leftCount)
    {
        UInt16 idspot = (id << 8) + spot;
		GameMsgHdr hdr(0x278, m_Player->getThreadId(), m_Player, sizeof(idspot));
		GLOBAL().PushMsg(hdr, &idspot);
        if (!leftCount)
			PopTimerEvent(m_Player, EVENT_AUTOFRONTMAP, m_Player->getId());
        ++spot;
    }

    bool EventAutoFrontMap::Accelerate(UInt32 times)
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
		_isJumpingMap(false),
#ifdef _ARENA_SERVER
        _channelId(static_cast<int>(id >> 40) & 0xFF), _serverId(static_cast<int>(id >> 48)),
#endif
        _isOnline(false), _isHoding(false), _holdGold(0), _threadId(0xFF), _session(-1),
		_availInit(false), _vipLevel(0), _clan(NULL), _clanBattle(NULL), _flag(0), _gflag(0), _onlineDuration(0), _offlineTime(0),
		_nextTavernUpdate(0), _nextBookStoreUpdate(0), _bossLevel(21), _ng(NULL), _lastNg(NULL),
		_lastDungeon(0), _exchangeTicketCount(0), _praplace(0), m_autoCopyFailed(false),
        _justice_roar(0), _spirit_factor(1.0f), _diamond_privilege(false), _worldBossHp(0), m_autoCopyComplete(0), hispot(0xFF), hitype(0), m_ulog(NULL),
        m_isOffical(false), m_sysDailog(false), m_hasTripod(false)
	{
        m_ClanBattleStatus = 1;
        m_ClanBattleScore = 0;
        m_ClanBattleWinTimes = 0;
        m_ClanBattleSkillFlag = 0;
        _invitedBy = 0;
        m_arenaCommitCD = 0;

		memset(_buffData, 0, sizeof(UInt32) * PLAYER_BUFF_COUNT);
		m_Package = new Package(this);
		m_TaskMgr = new TaskMgr(this);
		m_Trade = new Trade(this);
		m_Sale = new Sale(this);
		m_MailBox = new MailBox(this);
		m_Athletics = new Athletics(this);
		m_AttainMgr = new AttainMgr(this);
        m_ActivityMgr = new ActivityMgr(this);
        m_HeroMemo = new HeroMemo(this);
        m_ShuoShuo = new ShuoShuo(this);
        m_CFriend = new CFriend(this);
        m_pVars = new VarSystem(id);
        memset(&m_ctp, 0, sizeof(m_ctp));
        m_teamData = NULL;
        m_tcpInfo = new TeamCopyPlayerInfo(this);
        m_hf = new HoneyFall(this);
        m_dpData = new DeamonPlayerData();
        m_csFlag = 0;
        _mditem = 0;
	}


	Player::~Player()
	{
		UnInit();
        delete m_tcpInfo;
        m_tcpInfo = NULL;
        delete m_hf;
        m_hf = NULL;
        delete m_dpData;
        m_dpData = NULL;
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
        
        ClanRankBattleMgr::Instance().PlayerEnter(this);

		setBlockBossByLevel();
		return true;
	}

	void Player::setVipL(UInt8 lvl)
	{
#define _VIP_OPEN_MAX 15
        if (!lvl || lvl > _VIP_OPEN_MAX)
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
        else if (lvl == 11)
            _playerData.totalRecharge = 988888;
        else if (lvl == 12)
            _playerData.totalRecharge = 1888888;
        else if (lvl == 13)
            _playerData.totalRecharge = 2888888;
        else if (lvl == 14)
            _playerData.totalRecharge = 5888888;
        else if (lvl == 15)
            _playerData.totalRecharge = 8888888;
		DB1().PushUpdateData("UPDATE `player` SET `totalRecharge` = %u WHERE `id` = %"I64_FMT"u", _playerData.totalRecharge, getId());
        recalcVipLevel();
		sendModification(7, _playerData.totalRecharge);
    }

	UInt32 Player::calcVipLevel()
	{
#define VIP_OPEN_MAX 15
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
		else if(totalRecharge < 988888)
			vipl = 10;
		else if(totalRecharge < 1888888)
			vipl = 11;
		else if(totalRecharge < 2888888)
			vipl = 12;
		else if(totalRecharge < 5888888)
			vipl = 13;
		else if(totalRecharge < 8888888)
			vipl = 14;
        else
			vipl = 15;
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

    //春节充值活动额度
    //static UInt32 recharge[] = {188,288,688,888,1888,2012,2888,3888,4888,5888,6888,7888,8888,9888,9999};
    //2012年3月份充值活动额度
    //static UInt32 recharge[] = {199,399,599,999,1299,1599,1999,2999,3999,4999,5999,6999,7999,8999,9999,};
    //2012年5月16充值活动额度
    //static UInt32 recharge[] = {199,399,599,799,999,1499,1999,3999,5999,7999,9999,19999,29999,39999,49999};
    static UInt32 recharge[] = {99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,26999,42999,64999,99999,};

    UInt8 Player::calcRechargeLevel(UInt32 total, UInt8& maxlevel)
    {
        UInt32 totalRecharge = total;
        maxlevel = sizeof(recharge)/sizeof(UInt32);
        for (UInt8 i = 0; i < sizeof(recharge)/sizeof(UInt32); ++i)
        {
            if (totalRecharge < recharge[i])
                return i;
        }
        return maxlevel;
    }

    inline UInt32 levelToRecharge(UInt8 lvl)
    {
        if (lvl < 1)
            return 0;
        if (lvl > sizeof(recharge)/sizeof(UInt32))
            lvl = sizeof(recharge)/sizeof(UInt32);
        return recharge[lvl-1];
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
        SAFE_DELETE(m_AttainMgr);
        SAFE_DELETE(m_ActivityMgr);
        SAFE_DELETE(m_HeroMemo);
        SAFE_DELETE(m_ShuoShuo);
        SAFE_DELETE(m_CFriend);
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

    void Player::enchantGt11()
    {
        struct EnchantGt11 : public Visitor<ItemBase>
        {
            EnchantGt11(Player* player) : player(player) {}

            bool operator()(ItemBase* ptr)
            {
                if (IsWeapon(ptr->getClass()) || IsArmor(ptr->getClass()))
                {
                    ItemEquip* ie = (ItemEquip*)(ptr);
                    if (ie->getItemEquipData().enchant == 11 || ie->getItemEquipData().enchant == 12)
                    {
                        UInt8 type = IsWeapon(ptr->getClass())?1:2;
                        for (UInt8 l = ie->getItemEquipData().enchant; l >= 11; --l)
                            GameAction()->onEnchantGt11(player, ie->GetItemType().getId(), l, type);
                    }
                }
                return true;
            }

            Player* player;
        } eg(this);
        GetPackage()->enumerate(eg);

        std::map<UInt32, Fighter *>::iterator it = _fighters.begin();
        for (; it != _fighters.end(); ++it)
        {
            Fighter* fgt = it->second; // XXX: Fashion can not be enchanted
            ItemEquip* e[11] = {fgt->getWeapon(), fgt->getArmor(0), fgt->getArmor(1),
                fgt->getArmor(2), fgt->getArmor(3), fgt->getArmor(4), fgt->getAmulet(),
                fgt->getRing(), fgt->getTrump(0), fgt->getTrump(1), fgt->getTrump(2)};

            for (int i = 0; i < 11; ++i)
            {
                if (e[i] && (e[i]->getItemEquipData().enchant == 11 || e[i]->getItemEquipData().enchant == 12))
                {
                    UInt8 type = IsWeapon(e[i]->getClass())?1:2;
                    for (UInt8 l = e[i]->getItemEquipData().enchant; l >= 11; --l)
                        GameAction()->onEnchantGt11(this, e[i]->GetItemType().getId(), l, type);
                }
            }
        }

        SetVar(VAR_ENCHANTGT11, 1);
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

        continuousLogin(curtime);
        continuousLoginRF(curtime);

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
        if (World::getChristmas())
            GameAction()->onChristmas(this);
        if (World::getNewYear())
            GameAction()->onNewYear(this);
        if (World::getValentineDay())
            GameAction()->onValentineDay(this);
        if (World::getFoolsDay())
            GameAction()->onFoolsDay(this);
        if (World::getMayDay())
            GameAction()->onMayDay(this);
        if (World::getMayDay1())
            GameAction()->onMayDay1(this);
        if (World::getEnchantGt11() && !GetVar(VAR_ENCHANTGT11))
            enchantGt11();
        if (World::getEnchantAct() && !GetVar(VAR_2NDSOUL))
        {
            for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
            {
                Fighter* fgt = it->second;
                GameAction()->onSoulEnchantMaxSoul(this, 0, fgt->get2ndSounSoulMax());
            }
            SetVar(VAR_2NDSOUL, 1);
        }

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

        if (World::_thanksgiving)
        {
            UInt32 online = GetVar(VAR_TGDT);
            if (online != static_cast<UInt32>(-1))
            {
                if (online < TGD_ONLINE_TIME)
                {
                    EventPlayerTimeTick* event = new(std::nothrow) EventPlayerTimeTick(this, TGD_ONLINE_TIME-online, 1, 1);
                    if (event) PushTimerEvent(event);
                }
                else
                    GameAction()->onThanksgivingDay(this);
            }
        }

        if (World::_blueactiveday)
            onBlueactiveday();

        sendLevelPack(GetLev());
        offlineExp(curtime);

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

        if (!m_via.empty())
        {
            StringTokenizer via(m_via, "_");
            if (via.count() > 1)
                udpLog(via[0].c_str(), via[1].c_str(), "", "", "", "1", "login");
            else
                udpLog(m_via.c_str(), "", "", "", "", "1", "login");
        }
        else
            udpLog("", "", "", "", "", "1", "login");

        if (!m_invited.empty())
        {
            if (!getInvitedBy())
            {
                UInt64 playerid = atoll(m_invited.c_str());
                if (playerid != getId())
                {
                    Player* cfriend = globalPlayers[playerid];
                    if (cfriend)
                    {
                        if (addCFriend(cfriend))
                        {
                            setInvitedBy(playerid);
                        }
                    }
                }
            }
        }

		if(!_playerData.totalRecharge && !GetVar(VAR_VIPFIRST))
		{
            SetVar(VAR_VIPFIRST, 1);

			SYSMSG(title, 254);
			SYSMSG(content, 255);

			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[1] = {{449, 1}};
                MailItemsInfo itemsInfo(mitem, FirstReChargeAward, 1);
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
		}

#ifdef _FB
#else
        dclogger.login(this);
#endif
	}

#define WEBDOWNLOAD 255
#define OFFICAL 12

    void Player::udpLog(UInt8 platform, const char* str1, const char* str2, const char* str3, const char* str4,
                const char* str5, const char* str6, const char* type, UInt32 count)
    {
        if (m_ulog)
        {
            char buf[1024] = {0};
            char* pbuf = &buf[0];
            pbuf += snprintf(pbuf, sizeof(buf), "%u_%u_%"I64_FMT"u|%s|||||%u||%u||||||%u||%u||%u|",
                    cfg.serverNum, cfg.tcpPort, getId(), getOpenId().c_str(), GetLev(), _playerData.gold, _playerData.qqvipl, cfg.serverNum, platform);

            m_ulog->SetUserMsg(buf);
            if (platform != WEBDOWNLOAD)
                m_ulog->LogMsg(str1, str2, str3, str4, str5, str6, type, count, 0);
            if (platform)
                m_ulog->LogMsg(str1, str2, str3, str4, str5, str6, type, count, platform);

            TRACE_LOG("%s - (%s,%s,%s,%s,%s,%s,%s,%u)", buf, str1, str2, str3, str4, str5, str6, type, count);
        }
    }

    void Player::udpLog(const char* str1, const char* str2, const char* str3, const char* str4,
                const char* str5, const char* str6, const char* type, UInt32 count)
    {
        if (m_ulog)
        {
            UInt8 platform = atoi(getDomain().c_str());
            if (platform == OFFICAL && strstr(m_via.c_str(), "webdownload"))
                platform = WEBDOWNLOAD;

            udpLog(platform, str1, str2, str3, str4, str5, str6, type, count);
        }
    }

    void Player::udpLog(UInt32 type, UInt32 id, UInt32 num, UInt32 price, const char* op)
    {    
        if (!op || !price)
            return;
        char _price[32] = {0};
        char _type[32] = {0};
        char _id[32] = {0};
        if (!id || !num)
        {
            snprintf(_type, 32, "1");
            snprintf(_id, 32, "GN_%u", type);
            snprintf(_price, 32, "%u", price);
            udpLog(op, _type, _id, _price, "", "", "props");
        }
        else
        {
            UInt8 type = GetItemLogType(id);
            snprintf(_type, 32, "%u", type);
            snprintf(_id, 32, "%u", id);
            snprintf(_price, 32, "%u", price/num);
            udpLog(op, _type, _id, _price, "", "", "props", num);
        }
    }

    void Player::guideUdp(UInt8 type, std::string& p1, std::string& p2)
    {
        if (type == 0)
            udpLog(p1.c_str(), p2.c_str(), "", "", "", "", "guide");
        else if (type == 1)
            udpLog(p1.c_str(), p2.c_str(), "", "", "", "", "act");
    }

    void Player::moneyLog(int type, int gold, int coupon, int tael, int achievement, int prestige)
    {
        if (!type || type > 2) return;
        UInt32 now = TimeUtil::Now();
        int today = TimeUtil::GetYYMMDD(now);
        bool sameDay = TimeUtil::SameDay(World::_moneyLogged, now);
        if (!sameDay)
        {
            for (int i = 0; i < 6; ++i)
                memcpy(&World::_moneyIn[i], &World::_moneyIn[i+1], sizeof(World::_moneyIn[i]));
            World::_moneyIn[6] = {{0,},};

            DB8().PushUpdateData("INSERT INTO `money` (`time`, `type`, `gold`, `coupon`, `tael`, `achievement`, `prestige`) VALUES (%d,1,0,0,0,0,0)", today);
            DB8().PushUpdateData("INSERT INTO `money` (`time`, `type`, `gold`, `coupon`, `tael`, `achievement`, `prestige`) VALUES (%d,2,0,0,0,0,0)", today);
        }
        DB8().PushUpdateData("UPDATE `money` SET `gold` = `gold` + %d, `coupon` = `coupon` + %d, `tael` = `tael` + %d, `achievement` = `achievement` + %d, `prestige` = `prestige` + %d WHERE `time` = %d AND `type` = %d", gold, coupon, tael, achievement, prestige, today, type);

        World::_moneyLogged = now;
        // TODO:
        World::_moneyIn[6][type-1].gold += gold;
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

    UInt32 Player::GetOnlineTimeToday()
    {
        UInt32 now = TimeUtil::Now();
         UInt32 onlineToday = GetVarNow(VAR_TODAY_ONLINE, now);

         UInt32 t = GetOnlineTimeTodaySinceLastLogin(now);
         return onlineToday + t;

    }
    UInt32  Player::GetOnlineTimeTodaySinceLastLogin(UInt32 now)
    {
//        UInt32 now  = TimeUtil::Now();
        UInt32 today = TimeUtil::SharpDayT( 0 , now);
        UInt32 lastOnline = _playerData.lastOnline;
        if( today >= lastOnline)
        {
           return now  - today ;
        }
        else
        {
            if(now  > lastOnline)
                return  now - lastOnline ;
        }
        return 0;
    }
    void Player::LogoutSaveOnlineTimeToday()
    {
        UInt32 now = TimeUtil::Now();

       // UInt32 onlineToday = GetVar(VAR_TODAY_ONLINE, now);
    
        UInt32 t = GetOnlineTimeTodaySinceLastLogin(now);
        AddVarNow(VAR_TODAY_ONLINE,  t , now);

    }
	void Player::selfKick()
	{
		if (m_TaskMgr->IsInConvey())
		{
			m_TaskMgr->SetConvey(false);
		}

		UInt32 curtime = TimeUtil::Now();

        if (World::_thanksgiving)
        {
            PopTimerEvent(this, EVENT_TIMETICK, getId());
            UInt32 online = GetVar(VAR_TGDT);
            if (online != static_cast<UInt32>(-1))
            {
                if (online + curtime - _playerData.lastOnline >= TGD_ONLINE_TIME)
                    GameAction()->onThanksgivingDay(this);
                else
                    SetVar(VAR_TGDT, online + curtime - _playerData.lastOnline);
            }
        }

		DBLOG1().PushUpdateData("update login_states set logout_time=%u where server_id=%u and player_id=%"I64_FMT"u and login_time=%u", curtime, cfg.serverLogId, _id, _playerData.lastOnline);
		//_playerData.lastOnline = curtime; // XXX: 在线时间统计问题
		writeOnlineRewardToDB();

		removeStatus(SGPunish);
        LogoutSaveOnlineTimeToday();
        udpLog("", "", "", "", "", "2", "login");
	}

	void Player::Logout(bool nobroadcast)
	{
		if (m_TaskMgr->IsInConvey())
		{
			m_TaskMgr->SetConvey(false);
		}

		UInt32 curtime = TimeUtil::Now();
        SetVar(VAR_OFFLINE, curtime);

        if(hasFlag(InCopyTeam))
            teamCopyManager->leaveTeamCopy(this);

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

        if (World::_thanksgiving)
        {
            PopTimerEvent(this, EVENT_TIMETICK, getId());
            UInt32 online = GetVar(VAR_TGDT);
            if (online != static_cast<UInt32>(-1))
            {
                if (online + curtime - _playerData.lastOnline >= TGD_ONLINE_TIME)
                    GameAction()->onThanksgivingDay(this);
                else
                    SetVar(VAR_TGDT, online + curtime - _playerData.lastOnline);
            }
        }

        if (World::_blueactiveday)
            PopTimerEvent(this, EVENT_TIMETICK, getId());

        LogoutSaveOnlineTimeToday();
#ifdef _FB
#else
        dclogger.logout(this);
#endif
        heroIsland.playerOffline(this);
		removeStatus(SGPunish);
        udpLog("", "", "", "", "", "2", "login");
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

        TeamCopyPlayerInfo* tcpInfo = getTeamCopyPlayerInfo();
        if(tcpInfo)
        {
            tcpInfo->sendAwardInfo();
        }

        sendDeamonAwardsInfo();

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

    Int32 Player::GetVarS(Int32 id)
    {
        return (Int32)m_pVars->GetVar(id);
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

    void Player::AddVarS(UInt32 id, Int32 val)
    {
        Int32 v = GetVarS(id);
        m_pVars->SetVar(id,v+val);
    }

    UInt32 Player::GetVarNow(UInt32 id,  UInt32 now)
    {
        return m_pVars->GetVar(id , now);
    }
    void Player::SetVarNow(UInt32 id, UInt32 val, UInt32 now)
    {
        m_pVars->SetVar(id,val, now);
    }

    void Player::AddVarNow(UInt32 id, UInt32 val, UInt32 now)
    {
        m_pVars->AddVar(id,val, now);
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

    UInt8 Player::GetColor() const
    {
        return (!_fighters.empty()) ? _fighters.begin()->second->getColor() : 0;
    }

    UInt8 Player::getPortraitAndColor() const
    {
        if(_fighters.empty())
            return 0;
        Fighter * fgt = _fighters.begin()->second;
        return (fgt->getColor() << 3) + fgt->getId();
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

            //招募散仙荣誉
           if(!load && CURRENT_THREAD_ID() <= WORKER_THREAD_NEUTRAL)
           {
                 UInt8 col  = fgt->getColor();
           
                 if(_fighters.size())
                     GameAction()->doAttainment(this, 10101, static_cast<UInt32>(col));

                 UInt8 minCol = col;

                 std::map<UInt32, Fighter *>::iterator it = _fighters.begin();

                 while(it != _fighters.end())
                 {
                      if(it->second->getId() >=  10 )
                         minCol = min(minCol, it->second->getColor());
                       it ++ ;
                 }

           
                 if(_fighters.size() == 5)
                     GameAction()->doAttainment(this, 10102, minCol);
                 if(_fighters.size() == 10)
                     GameAction()->doAttainment(this, 10108, minCol);
           }
		}

        if (!load && !fgt->getCittasNum())
            upInitCitta(fgt, true);

        fgt->getAttrType1(true);
        fgt->getAttrType2(true);
        fgt->getAttrType3(true);
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
        else if (itemid == 9011)
            id = 116;

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

        { // XXX: 更全的信息
            Stream st(REP::FIGHTER_INFO);
            st << static_cast<UInt8>(1);
            makeFighterInfo(st, fgt);
            st << Stream::eos;
            send(st);
        }

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

            for(Int8 k = MAX_SKILL_NUM - 1; k >= 0; --k)
                fgt->equipSoulSkill(k, 0);

			ItemEquip * equip;
			for(UInt8 z = 0; z < 9; ++ z)
				m_Package->EquipTo(0, fgt, z+0x20, equip, true);
            for(UInt8 t = 0; t < 3; ++ t)
				m_Package->EquipTo(0, fgt, t+0x50, equip, true);

			_fighters.erase(it);
			DB2().PushUpdateData("DELETE FROM `fighter` WHERE `id` = %u AND `playerId` = %"I64_FMT"u", id, getId());

            if(fgt->getColor() >= 2) //删除散仙荣誉
                GameAction()->doAttainment(this, 10107, fgt->getColor());
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

	UInt32 Player::addFightCurrentHp(UInt32 id, UInt32 hp, bool battle)
	{
		UInt32 currHp = 0;
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

	void Player::addFightCurrentHpAll(UInt32 hp)
	{
		UInt32 currHp = 0;
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
			<< status << _playerData.title << static_cast<UInt8>(0) << _playerData.totalRecharge << static_cast<UInt8>(_playerData.qqvipl%10) << _playerData.qqvipyear << _playerData.achievement << _playerData.prestige<< _playerData.attainment << _playerData.packSize << _playerData.newGuild <<  _playerData.mounts << c;
		for(UInt8 i = 0; i < c; ++ i)
		{
			st << buffid[i] << buffleft[i];
		}
        st << GetVar(VAR_MONEY_ARENA);
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

        if(hasFlag(GObject::Player::InCopyTeam))
        {
            teamCopyManager->updateTeamInfo(this);
        }
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
		st << fgt->getPeerlessAndLevel() << fgt->getCurrentHP();
        st << fgt->getAttrType1();
        st << fgt->getAttrValue1();
        st << fgt->getAttrType2();
        st << fgt->getAttrValue2();
        st << fgt->getAttrType3();
        st << fgt->getAttrValue3();
        fgt->appendElixirAttr(st);
        st << fgt->getUpCittasMax();
		if(withequip)
		{
			st << fgt->getFashionId() << fgt->getWeaponId() << fgt->getArmorId(0) << fgt->getArmorId(1)
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

	bool Player::challenge( Player * other, UInt32 * rid, int * turns, bool applyhp, UInt32 sysRegen, bool noreghp, UInt32 scene, UInt8 report )
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
        if (report & 0x01)
        {
            send(st);
        }
        else
        {
            if (res)
            {
                SYSMSG_SENDV(2144, this, other->getCountry(), other->getName().c_str());
            }
            else
            {
                SYSMSG_SENDV(2143, this, other->getCountry(), other->getName().c_str());
            }
        }

        if (report & 0x02)
        {
            st.data<UInt8>(4) = static_cast<UInt8>(res ? 0 : 1);
            other->send(st);
        }
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
			if(bsim.applyFighterHP(0, this, !hasFlag(CountryBattle | ClanBattling | ClanRankBattle), 0))
				checkHPLoss();
			if(bsim.applyFighterHP(1, other, !other->hasFlag(CountryBattle | ClanBattling | ClanRankBattle), 0))
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
                UInt32 exp = 0;
                if(getBuffData(PLAYER_BUFF_TRAINP3, now))
                    exp = ng->getExp() * 17 / 10;
                else if(getBuffData(PLAYER_BUFF_TRAINP4, now))
                    exp = ng->getExp() * 3 / 2;
                else if(getBuffData(PLAYER_BUFF_TRAINP2, now))
                    exp = ng->getExp() * 3 / 2;
                else if(getBuffData(PLAYER_BUFF_TRAINP1, now))
                    exp = ng->getExp() * 13 / 10;
                else
                    exp  = ng->getExp();

                if (isOffical())
                    exp -= (exp/10);
                if(this->getPlatform() == 10 && World::getQQGameAct())
                {
                    UInt32 extraExp = exp / 2;//蓝黄钻野外手动打怪经验+50%
                    SYSMSG_SENDV(1092, this, extraExp);
                    SYSMSG_SENDV(1093, this, extraExp);
                    exp += extraExp;
                }
                pendExp(exp);
                ng->getLoots(this, _lastLoot);
            }
            //战胜特定NPC之后 荣誉
            GameAction()->doAttainment(this, 10351, npcId);
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

    void Player::autoFrontMapFailed()
    {
        //PopTimerEvent(this, EVENT_AUTOFRONTMAP, getId());
        //delFlag(Player::AutoFrontMap);
    }

    void Player::autoCopyFailed(UInt8 id)
    {
        //GObject::playerCopy.failed(this, id);
        //setCopyFailed();
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
            //战胜NPC 成就
            GameAction()->doAttainment(this, 10351, npcId);

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
		UInt32 count = 60 * 8;

        if (!World::getNewYear() || GetLev() < 45)
        {
            UInt32 viplvl = getVipLevel();
            if(viplvl >= 4 && viplvl <= 7)
                count += 60 * 8;
            else if (viplvl > 7 && viplvl <= 15)
                count += 60 * 16;
        }
        else
        {
            count = 60 * 240;
        }

		UInt32 timeDur = count * eachBattle;

		UInt32 final = TimeUtil::Now() + timeDur;
		EventAutoBattle* event = new(std::nothrow) EventAutoBattle(this, eachBattle, count, ng, final);
		if (event == NULL) return false;
		cancelAutoBattle();
		PushTimerEvent(event);
		addFlag(Training);
		event->notify();
		event->updateDB(true);

        OnHeroMemo(MC_FIGHTER, MD_STARTED, 0, 0);
		return true;
	}

	void Player::pushAutoBattle(UInt32 npcId, UInt16 count, UInt16 interval)
	{
		if(npcId == 0 || count == 0 || interval == 0)
			return;
        if (count > 1440 && GetLev() < 45) // XXX: 45级以下不允许挂机240小时
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
        UInt8 cnt = Player::getMaxIcCount(getVipLevel()) - getIcCount();
        if (Player::getMaxIcCount(getVipLevel()) < getIcCount())
            cnt = Player::getMaxIcCount(getVipLevel());
		st << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt32>(0) << cnt << Stream::eos;
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
		if(_playerData.icCount > getMaxIcCount(_vipLevel))
            _playerData.icCount = 0;
		if(_playerData.icCount >= getMaxIcCount(_vipLevel) || !hasFlag(Training) || getGoldOrCoupon() < 10)
			return;

        ConsumeInfo ci(InstantAutoBattle,0,0);
		useGoldOrCoupon(GData::moneyNeed[GData::INSTANTAUTOBATTLE].gold,&ci);
        incIcCount();
		GameMsgHdr hdr(0x178, WORKER_THREAD_WORLD, this, 0);
		GLOBAL().PushMsg(hdr, NULL);
        GameAction()->doAty(this, AtyTaskHook, 0,0);
        OnHeroMemo(MC_FIGHTER, MD_STARTED, 0, 1);
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
			pl->addFriendInternal(this, true, true);
		}
		else
		{
			pl->addFriendInternal(this, true, true);
			addFriendInternal(pl, true, true);
		}

        AddFriendAttainment(pl);
		return true;
	}

	bool Player::addCFriend( Player * pl )
	{
		if(pl == NULL || this == pl)
			return false;
		Mutex::ScopedLock lk(_mutex);
		Mutex::ScopedLock lk2(pl->getMutex());
		if(!testCanAddCFriend(pl))
			return false;
		if(_id < pl->getId())
		{
			addCFriendInternal(pl, true, true);
			pl->addCFriendInternal(this, true, true);
		}
		else
		{
			pl->addCFriendInternal(this, true, true);
			addCFriendInternal(pl, true, true);
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

	void Player::addCFriendFromDB( Player * pl )
	{
		if(pl == NULL || this == pl)
			return;
		if(_id < pl->getId())
		{
			addCFriendInternal(pl, false, false);
			pl->addCFriendInternal(this, false, false);
		}
		else
		{
			pl->addCFriendInternal(this, false, false);
			addCFriendInternal(pl, false, false);
		}
	}

    void Player::AddFriendAttainment( Player* pl)
    {
        //增加好友成就
            //处理别人的
            stAttainMsg  msg;
            msg.attainID =  Script::ADD_FRIEND;
            msg.param =  0;
            GameMsgHdr h(0x244,  pl->getThreadId(), pl, sizeof(msg));
            GLOBAL().PushMsg(h, & msg);

            msg.attainID = Script::ONE_FRIEND_LEV_UP;
            msg.param = this->GetLev();
            GLOBAL().PushMsg(h, & msg);

            //处理自己的
            GameAction()->doAttainment(this, Script::ADD_FRIEND,       _friends[0].size());
            //GameAction()->doAttainment(this, Script::ONE_FRIEND_LEV_UP, pl->GetLev());
            OnFriendLevUp(pl->GetLev());
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

        if (notify || writedb)
        {
            if (getFrendsNum() >= 1)
                OnHeroMemo(MC_CONTACTS, MD_STARTED, 0, 0);
            if (getFrendsNum() >= 5)
                OnHeroMemo(MC_CONTACTS, MD_STARTED, 0, 1);
            if (getFrendsNum() >= 10)
                OnHeroMemo(MC_CONTACTS, MD_STARTED, 0, 2);
        }
	}

	void Player::addCFriendInternal( Player * pl, bool notify, bool writedb )
	{
		if(notify)
		{
			//notifyFriendAct(1, pl);
			Stream st(REP::FRIEND_ACTION);
			st << static_cast<UInt8>(0x07) << pl->getId() << pl->getName() << pl->getPF() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << Stream::eos;
			send(st);
			SYSMSG_SEND(2341, this);
			SYSMSG_SENDV(2342, this, pl->getCountry(), pl->getName().c_str());
			if(writedb)
				DB1().PushUpdateData("REPLACE INTO `friend` (`id`, `type`, `friendId`) VALUES (%"I64_FMT"u, 3, %"I64_FMT"u)", getId(), pl->getId());
		}
		_friends[3].insert(pl);
	}
	void Player::delFriend( Player * pl )
	{
		Mutex::ScopedLock lk(_mutex);
		Mutex::ScopedLock lk2(pl->getMutex());
		delFriendInternal(pl);
		pl->delFriendInternal(this);
	}
    void Player::delCFriend(Player* pl)
	{
		Mutex::ScopedLock lk(_mutex);
		Mutex::ScopedLock lk2(pl->getMutex());
		delCFriendInternal(pl);
		pl->delCFriendInternal(this);
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

	void Player::delCFriendInternal( Player * pl, bool writedb )
	{
		std::set<Player *>::iterator it = _friends[3].find(pl);
		if(it == _friends[3].end())
			return;
		_friends[3].erase(it);
		Stream st(REP::FRIEND_ACTION);
		st << static_cast<UInt8>(0x08) << pl->getName() << Stream::eos;
		send(st);
		SYSMSG_SEND(2339, this);
		SYSMSG_SENDV(2340, this, pl->getCountry(), pl->getName().c_str());
		if(writedb)
			DB1().PushUpdateData("DELETE FROM `friend` WHERE `id` = %"I64_FMT"u AND `type` = 3 AND `friendId` = %"I64_FMT"u", getId(), pl->getId());
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
		if(_friends[1].size() >= MAX_FRIENDS)
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

		if(_friends[2].size() >= MAX_FRIENDS)
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
        case 0x0B:field = "attainment"; break;
		case 0x20: field = "packSize"; break;
		}
		if(field != NULL)
			DB1().PushUpdateData("UPDATE `player` SET `%s` = %u WHERE `id` = %"I64_FMT"u", field, v, _id);
	}

	UInt32 Player::getGold( UInt32 c, IncommingInfo* ii)
	{
		if(c == 0)
			return _playerData.gold;
		_playerData.gold += c;
		SYSMSG_SENDV(149, this, c);
		SYSMSG_SENDV(1049, this, c);
		sendModification(1, _playerData.gold);

        if(ii && ii->incommingType != 0)
        {
            DBLOG1().PushUpdateData("insert into consume_gold (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ii->incommingType, ii->itemId, ii->itemNum, c, TimeUtil::Now());
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

        if (ci)
            udpLog(ci->purchaseType, ci->itemId, ci->itemNum, c, "add");
#ifdef _FB
#else
        dclogger.consume(this, _playerData.gold, c);
#endif
		return _playerData.gold;
	}

    UInt32 Player::useGold4LuckDraw(UInt32 c)
    {
		ConsumeInfo ci(LuckyDraw,0,0);
		return useGold(c, &ci);
    }

    UInt32 Player::getGold4LuckDraw()
    {
        return getGold();
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

	UInt32 Player::getMoneyArena( UInt32 c )
	{
        UInt32 moneyArena = GetVar(VAR_MONEY_ARENA);
		if(c == 0)
			return moneyArena;
		moneyArena += c;
		SYSMSG_SENDV(164, this, c);
		SYSMSG_SENDV(1064, this, c);
        SetVar(VAR_MONEY_ARENA, moneyArena);

        Stream st(REP::USER_INFO_CHANGE);
        st << static_cast<UInt8>(0x56) << moneyArena << Stream::eos;
        send(st);

        return moneyArena;
	}

	UInt32 Player::useMoneyArena( UInt32 a,ConsumeInfo * ci )
	{
        UInt32 moneyArena = GetVar(VAR_MONEY_ARENA);
        if(a == 0 || moneyArena == 0)
            return moneyArena;
        if(moneyArena < a)
            moneyArena = 0;
        else
        {
            moneyArena -= a;
            if(ci!=NULL)
            {
                DBLOG1().PushUpdateData("insert into consume_achievement (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
            }
        }
        SYSMSG_SENDV(163, this, a);
        SYSMSG_SENDV(1063, this, a);
        SetVar(VAR_MONEY_ARENA, moneyArena);

        Stream st(REP::USER_INFO_CHANGE);
        st << static_cast<UInt8>(0x56) << moneyArena << Stream::eos;
        send(st);

        return moneyArena;
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

    bool Player::useDemonLog(UInt32 id, UInt32 num, ConsumeInfo* ci)
    {
        if (!ci)
            return false;
        DBLOG1().PushUpdateData("insert into consume_demon (server_id,player_id,consume_type,item_id,item_num,id,num,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u,%u)", cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, id, num, TimeUtil::Now());
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
            if (fgt->getLevel() >= golds.size())
                return false;
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
            if (fgt->getLevel() >= taels.size())
                return false;
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

    UInt32  Player::getAttainment( UInt32 a)
    {
        if(a == 0)
            return _playerData.attainment;

        _playerData.attainment += a;
        
        sendModification(0x0B, _playerData.attainment);
        return _playerData.attainment;
    }
    
    UInt32 Player::useAttainment(UInt32 a, ConsumeInfo* ci)
    {
        if(a == 0 || _playerData.attainment == 0)
            return _playerData.attainment;
        if(_playerData.attainment < a)
            _playerData.attainment = 0;
        else
            _playerData.attainment -= a;

        if(ci)
        {

        }
        sendModification(0x0B,  _playerData.attainment);
        return _playerData.attainment;
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

    void Player::AddPExpBy(Player* player, UInt32 pexp)
    {
        if (!player || !pexp)
            return;

        if (isOnline())
        {
            GameMsgHdr hdr2(0x238, this->getThreadId(), this, sizeof(pexp));
            GLOBAL().PushMsg(hdr2, &pexp);
        }
        else
            AddPExp(pexp);
    }

    void Player::AddItemBy(Player* player, UInt16 item, UInt16 num, bool bind)
    {
        if (!player || !item || !num)
            return;

        if (isOnline())
        {
            struct ItemAdd
            {
                UInt16 item;
                UInt16 num;
                bool bind;
            };

            ItemAdd ia;
            ia.item = item;
            ia.num = num;
            ia.bind = bind;

            GameMsgHdr hdr2(0x241, this->getThreadId(), this, sizeof(ia));
            GLOBAL().PushMsg(hdr2, &ia);
        }
        else
            m_Package->AddItem(item, num, bind, true);
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

		GObject::Country& cny = CURRENT_COUNTRY();
		UInt8 old_cny = GObject::mapCollection.getCountryFromSpot(_playerData.location);
        if (old_cny != cny.GetThreadID())
            return;

        if (isJumpingMap())
            return;
        setJumpingMap(true);

		cancelAutoBattle();
		cancelAutoDungeon();

        if (getBuffData(PLAYER_BUFF_AUTOCOPY))
            cancelAutoCopy(getBuffData(PLAYER_BUFF_AUTOCOPY));
        if (GetVar(VAR_ATOFM))
            cancelAutoFrontMap(GetVar(VAR_ATOFM));

        if (_playerData.location == 8977)
        {
            heroIsland.playerLeave(this);
            delFlag(Player::InHeroIsland);
        }
        ClanRankBattleMgr::Instance().PlayerLeave(this);

        if(hasFlag(InCopyTeam))
            teamCopyManager->leaveTeamCopy(this);

		UInt8 new_cny = GObject::mapCollection.getCountryFromSpot(spot);
        if (new_cny > WORKER_THREAD_LOGIN)
        {
            SYSMSG_SENDV(621, this, new_cny);
            setJumpingMap(false);
            return;
        }

		if(new_cny != cny.GetThreadID())
		{
			CountryEnterStruct ces(true, inCity ? 1 : 0, spot);
			cny.PlayerLeave(this);
			// _threadId = new_cny; // XXX: why here???
            // setJumpingMap(true);
			GameMsgHdr hdr(0x1F0, new_cny, this, sizeof(CountryEnterStruct));
			GLOBAL().PushMsg( hdr, &ces );
			return;
		}

		GObject::Map * map = GObject::Map::FromSpot(spot);
		if(map == NULL)
        {
            setJumpingMap(false);
			return;
        }

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

        ClanRankBattleMgr::Instance().PlayerEnter(this);

		if(inCity)
		{
			bool notify = !sameMap || !oldInCity;

			if(notify)
				map->SendCityNPCs(this);

			map->PlayerEnter(this, !sameMap);
		}
		map->SendAtCity(this, inCity, !sameMap);
        setJumpingMap(false);
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

       // printf("%u\n",  _playerData.smFinishCount );

        if(   _playerData.smFinishCount  == 5)
            GameAction()->doAttainment(this, Script::SHIMEN_5_TODAY, 0);
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

        if(_playerData.ymFinishCount == 5)
            GameAction()->doAttainment(this, Script::YAMEN_5_TODAY, 0);
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

                    if (World::getWhiteLoveDay() && World::_wday == 1)
                        GetPackage()->AddItem2(476, 1, 1, 1);

                    sendColorTask(0, 0);
                    writeShiMen();
                    OnShuoShuo(SS_SHIMEN);
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

                    if (World::getWhiteLoveDay() && World::_wday == 2)
                        GetPackage()->AddItem2(476, 1, 1, 1);

                    sendColorTask(1, 0);
                    writeYaMen();
                    OnShuoShuo(SS_YAMEN);
                    return true;
                }
            }
        } else {
            for (int i = 0; i < 6; ++i) {
                if (_playerData.fshimen[i] == taskid) {
                    if (!World::getNewYear())
                    {
                        if (getVipLevel() < 3) {
                            sendMsgCode(0, 1003);
                            return false;
                        }
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

                    if (World::getWhiteLoveDay() && World::_wday == 1)
                        GetPackage()->AddItem2(476, 1, 1, 1);

                    sendColorTask(0, 0);
                    writeShiMen();
                    OnShuoShuo(SS_SHIMEN);
                    return true;
                }
            }

            for (int i = 0; i < 6; ++i) {
                if (_playerData.fyamen[i] == taskid) {
                    if (!World::getNewYear())
                    {
                        if (getVipLevel() < 3) {
                            sendMsgCode(0, 1003);
                            return false;
                        }
                    }

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

                    if (World::getWhiteLoveDay() && World::_wday == 2)
                        GetPackage()->AddItem2(476, 1, 1, 1);

                    sendColorTask(1, 0);
                    writeYaMen();
                    OnShuoShuo(SS_YAMEN);
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

    UInt32 Player::getClanTask()
    {
        if (!getClan())
            return 0;
        URandom rnd(time(NULL));
        const std::vector<UInt32>& task = GData::GDataManager::GetClanTask();
        UInt8 idx = rnd(task.size());
        UInt32 taskid = task[idx];
        if(_playerData.clanTaskId == taskid)
            _playerData.clanTaskId = task[++idx % task.size()];
        else
            _playerData.clanTaskId = taskid;
        GetTaskMgr()->AddCanAcceptTask(_playerData.clanTaskId);
        return _playerData.clanTaskId;
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

        if(taskId != _playerData.clanTaskId || _playerData.ctFinishCount > getClanTaskMax() - 1)
            return false;

        ++ _playerData.ctFinishCount;
        if(getClanTaskMax() > _playerData.ctFinishCount) {
            getClanTask();
        } else {
            _playerData.clanTaskId = 0;
        }

        if (World::getWhiteLoveDay() && World::_wday == 3)
            GetPackage()->AddItem2(476, 1, 1, 1);

        OnHeroMemo(MC_CONTACTS, MD_ADVANCED, 0, 1);
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

    void Player::buildClanTask(bool fReset)
    {
        bool writeDB = false;
        do
        {
            if(getClan() == NULL)
            {
                break;
            }

            const std::vector<UInt32>& task = GData::GDataManager::GetClanTask();
            if(task.size() == 0)
                break;

            if(_playerData.clanTaskId == 0)
            {
                URandom rnd(time(NULL));
                _playerData.clanTaskId = task[rnd(task.size())];
                writeDB = true;
            }
            else
            {
                const GData::TaskType& taskType = GData::GDataManager::GetTaskTypeData(_playerData.clanTaskId);
                if(taskType.m_Class != 6)
                {
                    URandom rnd(time(NULL));
                    _playerData.clanTaskId = task[rnd(task.size())];
                    writeDB = true;
                }
            }

            GetTaskMgr()->AddCanAcceptTask(_playerData.clanTaskId);
        } while(false);

        if(fReset && _playerData.ctFinishCount != 0)
        {
            _playerData.ctFinishCount = 0;
            writeDB = true;
        }

        if(writeDB)
            writeClanTask();
    }

	void Player::writeClanTask()
	{
        if (!isOffical() && getClanTaskMax() < _playerData.ctFinishCount)
            _playerData.ctFinishCount = 0;

        Stream st(REP::CLAN_INFO_UPDATE);
        st << static_cast<UInt8>(9) << ((_playerData.ctFinishCount << 4) | getClanTaskMax());
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
        return getClanTaskMax() - 1 < _playerData.ctFinishCount;
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
                                        static UInt8 viptaskcolor[16] = {0,0,0,3,3,3,4,4,4,4,4,4,4,4,4,4};
                                        if (_playerData.fsmcolor[n] < viptaskcolor[getVipLevel()])
                                            _playerData.fsmcolor[n] = viptaskcolor[getVipLevel()];
                                    }
                                } else {
                                    _playerData.fyamen[n] = task[*i];
                                    _playerData.fymcolor[n] = j+1;
                                    if (getVipLevel() >= 2) {
                                        static UInt8 viptaskcolor[16] = {0,0,3,3,3,4,4,4,4,4,4,4,4,4,4,4};
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
            OnHeroMemo(MC_FIGHTER, MD_ADVANCED, 0, 0);
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
        if(type > 0)
            GameAction()->doAty(this, AtyBarRef, 0, 0);
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

        OnHeroMemo(MC_FIGHTER, MD_ADVANCED, 0, 1);
        if (fgt->getColor() == 1)
            OnHeroMemo(MC_FIGHTER, MD_ADVANCED, 0, 2);

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
		if(hasFlag(CountryBattle | ClanBattling | ClanRankBattle))
			return;
		UInt32 autohp = 0; // getBuffData(0);
		//if(autohp == 0)
		//	return;
		for(int i = 0; i < 5; ++ i)
		{
			Fighter * fighter = getLineup(i).fighter;
			if(fighter == NULL)
				continue;
			UInt32 hp = fighter->getCurrentHP();
			if(hp == 0)
				continue;
            if (!World::getAutoHeal())
            {
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
            }
            else
            {
                hp = 0;
            }
			fighter->setCurrentHP(hp);
		}
		//setBuffData(0, autohp);
	}

#ifdef _ARENA_SERVER
	UInt32 Player::getClientAddress()
	{
        return 0;
    }
#else
	UInt32 Player::getClientAddress()
	{
        TcpConnection conn = NETWORK()->GetConn(this->GetSessionID());
		if(conn.get() == NULL)
			return 0;
        Network::GameClient * client = static_cast<Network::GameClient *>(conn.get());
        return client->GetClientIP();
	}
#endif

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
            if (c == 4)
                OnHeroMemo(MC_FIGHTER, MD_ADVANCED, 1, 0);
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

    void Player::OnAddOneFriend()
    {
        GameAction()->doAttainment(this, Script::ADD_FRIEND,       _friends[0].size()); 
    }
    void Player::OnFriendLevUp(UInt8 nLev)
    {
    
        //一个好友的等级上升
        GameAction()->doAttainment(this, Script::ONE_FRIEND_LEV_UP, nLev);

        if(nLev < 50)
            return;
        UInt32 f80 = 0;
        UInt32 f50 = 0;
        std::set<Player *> :: iterator it = _friends[0].begin();

        while(it != _friends[0].end())
        {
            UInt8 curLev = (*it)->GetLev();
            if(curLev >= 50)
                f50 ++ ;
            if(curLev >= 80)
                f80 ++;
            it ++;
        }

        if(f80 >= 5)
        {
            GameAction()->doAttainment(this,  Script::FIVE_FRIEND_LEV_80, 0);
        }
        if(f50>= 5)
        {
            GameAction()->doAttainment(this,   Script:: FIVE_FRIEND_LEV_50, 0);
        }

    }

    static UInt8 cf_lvls[5] = {45, 50, 60, 70, LEVEL_MAX};
    void Player::tellCFriendLvlUp(UInt8 lvl)
    {
        if (!World::getCFriend())
            return;
        bool found = false;
        for (UInt8 i = 0; i < sizeof(cf_lvls)/sizeof(UInt8); ++i)
        {
            if (lvl < cf_lvls[i])
            {
                if (i)
                {
                    found = true;
                    lvl = cf_lvls[i-1];
                }
                break;
            }
        }

        if (!found)
            return;

        if (!_invitedBy)
            return;

        struct PlayerLvlUp
        {
            Player* player;
            UInt8 lvl;
        } msg;

        msg.player = this;
        msg.lvl = lvl;

        Player* player = globalPlayers[_invitedBy];
        if (!player)
            return;

        GameMsgHdr h(0x242,  player->getThreadId(), player, sizeof(msg));
        GLOBAL().PushMsg(h, &msg);
    }

    void Player::setCFriends()
    {
        UInt32 cfs[4] = {0};
        static UInt8 cf_nums[6] = {1, 3, 5, 10, 20, 30};
        for (std::set<Player *>::iterator it = _friends[3].begin();
                it != _friends[3].end(); ++it)
        {
            UInt8 lvl = (*it)->GetLev();
            for (UInt8 i = 0; i < sizeof(cf_lvls)/sizeof(UInt8); ++i)
            {
                if (lvl < cf_lvls[i] || lvl >= LEVEL_MAX)
                    break;
                ++cfs[i];
            }
        }

        for (UInt8 j = 0; j < sizeof(cfs)/sizeof(UInt8); ++j)
        {
            for (UInt8 i = 0; i < sizeof(cf_nums)/sizeof(UInt8); ++i)
            {
                if (cfs[j] < cf_nums[i])
                    break;
                GetCFriend()->setCFriendSafe(CF_45_1+6*j+i);
            }
        }
    }

    void Player::OnCFriendLvlUp(Player* player, UInt8 lvl)
    {
        if (!player || !lvl)
            return;
        if (!_hasCFriend(player))
            return;
        setCFriends();
    }

	void Player::checkLevUp(UInt8 oLev, UInt8 nLev)
	{
        if(nLev >= 40)
        {
            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 firstDay = TimeUtil::SharpDay(0, PLAYER_DATA(this, created));
            if(thisDay == firstDay && !this->GetVar(VAR_CLAWARD2))
            {
                 this->SetVar(VAR_CLAWARD2, 1);
                 this->sendRC7DayInfo(TimeUtil::Now());
            }
        }

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
        if (nLev >= 45)
            OnHeroMemo(MC_FIGHTER, MD_STARTED, 0, 2);
       tellCFriendLvlUp(nLev);

        //TELL my frined that my Level up  so get the Attainment!
        if(nLev == 50 || nLev == 80 || nLev == 100)
        {
            stAttainMsg  msg;
            msg .attainID = Script::ONE_FRIEND_LEV_UP;
            msg.param = nLev;
            std::set<Player *> :: iterator it = _friends[0].begin();
            while(it != _friends[0].end())
            {
                
                GameMsgHdr hdr(0x244, (*it)->getThreadId(), *it, sizeof(msg));
                GLOBAL().PushMsg(hdr, &msg);
                it ++ ;
            }
        }

        if (nLev == 70)
        {
            GameMsgHdr hdr(0x1A7, WORKER_THREAD_WORLD, this, 0);
            GLOBAL().PushMsg(hdr, 0);
        }

        if (nLev == 40 || nLev == 50 || nLev == 60 || nLev == 70 || nLev == 80 || nLev == 90 || nLev == 100)
            OnShuoShuo(nLev/10-4 + SS_40);
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
        int c = act_form.size();
        for( int idx = 0; idx < c; )
        {
            if( act_form[idx] == formationId )
            {
                act_form.erase(act_form.begin() + idx);
                c = act_form.size();
            }
            else
                ++idx;
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

        OnShuoShuo(SS_FMTLVLUP);
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
        formations += Itoa(_playerData.formations[0]);
        UInt8 cnt = _playerData.formations.size();
        UInt8 scnt = 0;

        if(_playerData.formations[0] %10 > 1)
            scnt ++ ;
        for(int idx = 1; idx < cnt; ++ idx)
        {
            formations += ",";
            formations += Itoa(_playerData.formations[idx]);

            if(_playerData.formations[idx] %10 > 1)
                scnt ++ ;
        }

        SYSMSG_SENDV(2104, this, newformation->getName().c_str());
        DB1().PushUpdateData("UPDATE `player` SET `formations` = '%s' WHERE id = %" I64_FMT "u", formations.c_str(), _id);

        //学习阵法的成就
        GameAction()->doAttainment(this,Script:: LEARNED_FORMATION, cnt); 
        GameAction()->doAttainment(this,Script:: LEARNED_SFORMATION , scnt);
        Stream st(REP::RANK_DATA);
        st << static_cast<UInt8>(2) << newformationId << Stream::eos;
        send(st);

        return true;
    }

	void Player::addTotalRecharge( UInt32 r )
	{
		if(r == 0)
			return;
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

		sendVIPMails(oldVipLevel + 1, _vipLevel);
        addRC7DayRecharge(r);
        addRF7DayRecharge(r);
        addRechargeNextRet(r);

        if (World::getRechargeActive())
        {
#if 0
            UInt32 total = GetVar(VAR_RECHARGE_TOTAL);
            UInt8 maxlevel = 0;
            UInt8 oldVipLevel = calcRechargeLevel(total, maxlevel);
            total += r;
            UInt8 vipLevel = calcRechargeLevel(total, maxlevel);
            sendRechargeMails(oldVipLevel + 1, vipLevel, maxlevel);
            SetVar(VAR_RECHARGE_TOTAL, total);
            sendRechargeInfo();
#else
            UInt32 total = GetVar(VAR_RECHARGE_TOTAL);
            GameAction()->sendRechargeMails(this, total, total+r);
            SetVar(VAR_RECHARGE_TOTAL, total+r);
            sendRechargeInfo();
#endif
        }

        if(World::getJune())
        {
            UInt32 total = GetVar(VAR_JUNE_RECHARGE_TOTAL);
            UInt32 value = total % 20;
            value += r;
            total += r;
            SetVar(VAR_JUNE_RECHARGE_TOTAL, total);
            sendJuneRechargeMails(value);
        }

        UInt32 total = getBuffData(PLAYER_BUFF_YDOTR);
		if(World::_nationalDay)
        {
            if (total != (UInt32)(-1))
            {
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
        }
        else
        {
            if (total)
                setBuffData(PLAYER_BUFF_YDOTR, 0, true);
        }

#ifdef _FB // XXX: 单笔反利
        if (World::IsNewServer())
            GameAction()->onRechargeAct(this, r);
#endif

        sendTripodInfo();
	}

    void Player::addRechargeNextRet(UInt32 r)
    {
        if (!World::getRechargeNextRet())
            return;
        if (World::_rechargenextretstart > World::_rechargenextretend)
            return;
        UInt32 now = TimeUtil::Now();
        if (now < World::_rechargenextretstart || now >= World::_rechargenextretend)
            return;
        UInt32 off = (now - World::_rechargenextretstart) / DAY_SECS;
        if (off > 60)
            return;
        UInt32 size = rechargs.size();
        if (off >= size)
            rechargs.resize(off+1, RNR());

        rechargs[off].date = TimeUtil::SharpDay(0, now);
        rechargs[off].recharge += r;

        Stream st(REP::DAILY_DATA);
        st << static_cast<UInt8>(14) << static_cast<UInt8>(off+1) << rechargs[off].recharge << Stream::eos;
        send(st);

        updateRNR2DB();
    }

    void Player::updateRNR2DB()
    {
        std::string str;
        UInt32 size = rechargs.size();
        for (UInt32 i = 0; i < size; ++i)
        {
            str += Itoa(rechargs[i].date);
            str += ",";
            str += Itoa(rechargs[i].recharge);
            if(i != size - 1)
                str += "|";
        }
		DB1().PushUpdateData("REPLACE INTO `rechargenextret` VALUES (%"I64_FMT"u, '%s')", getId(), str.c_str());
    }

    void Player::loadRNRFromDB(const std::string& str)
    {
        // XXX: 如果超出活动时间处理
        if (str.empty())
            return;
        StringTokenizer rs(str, "|");
        UInt32 count = rs.count();
        if (count)
        {
            rechargs.resize(count, RNR());
            for (UInt32 i = 0; i < count; ++i)
            {
                StringTokenizer t(rs[i], ",");
                if (t.count() == 2)
                {
                    rechargs[i].date = atoi(t[0].c_str());
                    rechargs[i].recharge = atoi(t[1].c_str());
                }
            }
        }
    }

    void Player::sendRNR(UInt32 now)
    {
        UInt32 size = rechargs.size();
        for (UInt32 i = 0; i < size; ++i)
        {
            UInt32 date = rechargs[i].date;
            UInt32 recharge = rechargs[i].recharge;

            if (date && recharge && (date >= TimeUtil::SharpDay(0, World::_rechargenextretstart)) &&
                    (date < TimeUtil::SharpDay(0, World::_rechargenextretend)) &&
                    (now >= (TimeUtil::SharpDay(0, date) + 13 * DAY_SECS)) &&
                    (now < (TimeUtil::SharpDay(0, date) + 13 * DAY_SECS + 3 * DAY_SECS)))
            {
                struct SendRNR
                {
                    Player* player;
                    UInt32 off;
                    UInt32 date;
                    UInt32 total;
                } rnr;

                rnr.player = this;
                rnr.off = (now - date) / (24 * 60 * 60) - 13;
                rnr.date = date;
                rnr.total = recharge;
                GameMsgHdr hdr(0x249, getThreadId(), this, sizeof(rnr));
                GLOBAL().PushMsg(hdr, &rnr);
            }
        }
    }

    void Player::sendRechargeNextRetInfo(UInt32 now)
    {
        if (now >= World::_rechargenextretstart &&
                now < (TimeUtil::SharpDay(0, World::_rechargenextretend) + 13 * DAY_SECS + 2 * DAY_SECS))
        {
            Stream st(REP::DAILY_DATA);
            UInt8 sz = rechargs.size();
            st << static_cast<UInt8>(13) << static_cast<UInt8>(sz);
            for (UInt8 i = 0; i < sz; ++i)
                st << rechargs[i].recharge;
            st << Stream::eos;
            send((st));
        }
    }

    void Player::sendRechargeInfo()
    {
        if (!World::getRechargeActive())
            return;

        UInt32 total = GetVar(VAR_RECHARGE_TOTAL);
		Stream st(REP::DAILY_DATA);
		st << static_cast<UInt8>(12) << total << Stream::eos;
		send((st));
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

    void Player::GetDailyTask(UInt8& shimenF, UInt8& shimenMax, UInt8& yamenF, UInt8& yamenMax, UInt8& clanF, UInt8& clanMax)
    {
        //UInt32 vipLevel = getVipLevel();
        if (!isOffical() && getShiMenMax() < _playerData.smFinishCount)
        {
            _playerData.smFinishCount = 0;
            writeShiMen();
        }
        if (!isOffical() && getYaMenMax() < _playerData.ymFinishCount)
        {
            _playerData.ymFinishCount = 0;
            writeYaMen();
        }
        if (!isOffical() && getClanTaskMax() < _playerData.ctFinishCount)
        {
            _playerData.ctFinishCount = 0;
            writeClanTask();
        }

        shimenF = _playerData.smFinishCount > getShiMenMax() ? getShiMenMax() : _playerData.smFinishCount;
        shimenMax = getShiMenMax();

        yamenF = _playerData.ymFinishCount > getYaMenMax() ? getYaMenMax() : _playerData.ymFinishCount;
        yamenMax = getYaMenMax();

        clanF =  _playerData.ctFinishCount > getClanTaskMax() ? getClanTaskMax() : _playerData.ctFinishCount;
        clanMax = getClanTaskMax();
    }

    void Player::GetFuben(UInt8& copy, UInt8& copyMax, UInt8& dung, UInt8& dungMax, UInt8& format, UInt8& formatMax )
    {
        UInt32 vipLevel = getVipLevel();
        UInt8 freeCnt, goldCnt;
        playerCopy.getCount(this, &freeCnt, &goldCnt, true);

        UInt8 currentCnt, totalCnt;
        if(this->isBD() && World::getBlueDiamondAct()) {
            currentCnt = this->GetVar(VAR_DIAMOND_BLUE);
            totalCnt = 1;
            if(currentCnt > totalCnt)
                currentCnt = 0;
        } else if (this->isYD() && World::getYellowDiamondAct()) {
            currentCnt = this->GetVar(VAR_DIAMOND_YELLOW);
            totalCnt = 1;
            if(currentCnt > totalCnt)
                currentCnt = 0;
        } else {
            currentCnt = 0;
            totalCnt = 0;
        }

        copy = freeCnt + goldCnt + currentCnt;
        copyMax = GObject::PlayerCopy::getFreeCount() + GObject::PlayerCopy::getGoldCount(vipLevel) + totalCnt;

        UInt32 now = TimeUtil::Now();
        if(now >= _playerData.dungeonEnd)
        {
            _playerData.dungeonCnt = 0;
        }
        dung = _playerData.dungeonCnt;
        dungMax = GObject::Dungeon::getMaxCount() + GObject::Dungeon::getExtraCount(vipLevel);

        UInt8 fcnt = frontMap.getCount(this); // XXX: lock???
        format = GObject::FrontMap::getFreeCount()+GObject::FrontMap::getGoldCount(vipLevel)-(((fcnt&0xf0)>>4)+(fcnt&0xf));
        formatMax = GObject::FrontMap::getFreeCount()  +    GObject::FrontMap::getGoldCount(vipLevel);

    }
	void Player::sendDailyInfo()
	{
		Stream st(REP::DAILY_DATA);
		st << static_cast<UInt8>(1);
        UInt32 curtime = TimeUtil::Now();
		UInt32 vipLevel = getVipLevel();
        if (!isOffical() && getShiMenMax() < _playerData.smFinishCount)
        {
            _playerData.smFinishCount = 0;
            writeShiMen();
        }
        if (!isOffical() && getYaMenMax() < _playerData.ymFinishCount)
        {
            _playerData.ymFinishCount = 0;
            writeYaMen();
        }
        if (!isOffical() && getClanTaskMax() < _playerData.ctFinishCount)
        {
            _playerData.ctFinishCount = 0;
            writeClanTask();
        }
        if (!_playerData.clanTaskId && !isOffical() && getClanTaskMax() > _playerData.ctFinishCount)
        {
            _playerData.clanTaskId = getClanTask();
        }

        UInt8 iccnt = Player::getMaxIcCount(vipLevel) - getIcCount();
        if (Player::getMaxIcCount(vipLevel) < getIcCount())
            iccnt = Player::getMaxIcCount(vipLevel);
        st << iccnt << static_cast<UInt8>(getShiMenMax() >= _playerData.smFinishCount ? getShiMenMax() - _playerData.smFinishCount : 0) << getShiMenMax() << static_cast<UInt8>(getYaMenMax() >= _playerData.ymFinishCount ? getYaMenMax() - _playerData.ymFinishCount : 0) << getYaMenMax() << static_cast<UInt8>(getClanTaskMax() > _playerData.ctFinishCount ? getClanTaskMax() - _playerData.ctFinishCount : 0);
        st << calcNextBookStoreUpdate(curtime) << calcNextTavernUpdate(curtime);
		//bossManager.buildInfo(st);
        UInt8 cnt = playerCopy.getCopySize(this);
        UInt8 freeCnt, goldCnt;
        playerCopy.getCount(this, &freeCnt, &goldCnt, true);

        UInt8 currentDiamondCnt;
        UInt8 totalDiamondCnt;
        if(this->isBD() && World::getBlueDiamondAct()) {
            currentDiamondCnt = this->GetVar(VAR_DIAMOND_BLUE);
            totalDiamondCnt = 1;
            if(currentDiamondCnt > totalDiamondCnt)
                currentDiamondCnt = 0;
        } else if (this->isYD() && World::getYellowDiamondAct()) {
            currentDiamondCnt = this->GetVar(VAR_DIAMOND_YELLOW);
            totalDiamondCnt = 1;
            if(currentDiamondCnt > totalDiamondCnt)
                currentDiamondCnt = 0;
        } else {
            currentDiamondCnt = 0;
            totalDiamondCnt = 0;
        }

        st << cnt << static_cast<UInt8>(freeCnt + goldCnt + currentDiamondCnt) << static_cast<UInt8>(GObject::PlayerCopy::getFreeCount()) << static_cast<UInt8>(GObject::PlayerCopy::getGoldCount(vipLevel)) << static_cast<UInt8>(totalDiamondCnt);
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
        heroIsland.sendDaily(this);
        globalCountryBattle.sendDaily(this);
        teamCopyManager->sendDaily(this, 7);
        teamCopyManager->sendDaily(this, 11);
	}

	void Player::regenAll(bool full)
	{
		for(int i = 0; i < 5; ++ i)
		{
			Lineup& pd = _playerData.lineup[i];
			if(pd.fighter != NULL && (pd.fighter->getCurrentHP() != 0 || full))
                    pd.fighter->setCurrentHP(0);
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
    UInt8 Player::GetFullPotFighterNum()
    {
        UInt8 num = 0 ; 
        std::map<UInt32, Fighter *> ::iterator it = _fighters.begin();
        while(it != _fighters.end())
        {
            if(it->second->getPotential() >= GObjectManager::getMaxPotential()/100 )
                num ++ ;
            it ++ ;
        }
        return num;
    }

    UInt8 Player::GetFullCapFighterNum()
    {
        UInt8 num = 0 ; 
        std::map<UInt32, Fighter *> ::iterator it = _fighters.begin();
        while(it != _fighters.end())
        {
            if(it->second->getCapacity() >= GObjectManager::getMaxCapacity()/100 )
                num ++; 
            it ++ ;
        }
        return num;
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

        if (type > 4 && (type&0xF) == 5)
        {
            UInt8 lock = type >> 4;
            UInt8 ret = fgt->forge(0, lock);
            if (!ret)
                fgt->updateForgeAttr();
            return ret;
        }

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

        if(isPotential)
            OnHeroMemo(MC_FIGHTER, MD_MASTER, 0, 0);
        else
            OnHeroMemo(MC_FIGHTER, MD_MASTER, 0, 1);

        bool bMainFighter = isMainFighter( fgt->getId()) ; 
		if(uRand(1000) < rate)
		{
            if(isPotential)
            {
                p += 0.01f;
                p = floorf(p * 100.0f + 0.5f) / 100.0f;
                
                bool bFull = false;
                if(p >=  GObjectManager::getMaxPotential()/100)
                {
                    p = GObjectManager::getMaxPotential()/100;
                    bFull = true;

                }
                fgt->setPotential(p);

                if(bFull)
                {
                    //触发潜力满 成就
                    UInt8 num = GetFullPotFighterNum();
                   
                    GameAction()->doAttainment(this, Script::FIGHTER_POT_FULL , num);
                    if(num == 10 && GetFullCapFighterNum() == 10)
                    {
                        //十个人全满
                        GameAction()->doAttainment(this, Script::TEN_FIGHTER_PC_FULL, 10);
                    }
                }

                if(bMainFighter)
                {
                    //主将潜力增加成就
                    GameAction()->doAttainment(this, Script:: MainFighterColChange ,fgt->getColor());
                    GameAction()->doAttainment(this, Script:: MainFighterFullPot ,  static_cast<UInt32>(p));

                }

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

#ifdef _FB
                    if (World::getTrainFighter() && _trainFightersAct.find(fgt->getId()) == _trainFightersAct.end())
                    {
                        GameAction()->onTrainFighterAct(this, fgt);
                        _trainFightersAct.insert(fgt->getId());
                    }
#endif
                }
            }
            else
            {
                p += 0.1f;
                p = floorf(p * 10.0f + 0.5f) / 10.0f;
                
                bool bFull = false;
                if(p >= GObjectManager::getMaxCapacity()/100)
                {
                    p = GObjectManager::getMaxCapacity()/100;
                    bFull = true;
                }

                fgt->setCapacity(p);
                if(bFull)
                {
                    UInt8 num = GetFullCapFighterNum();
                    //资质练满
                    GameAction()->doAttainment(this, Script::FIGHTER_CAP_FULL  , num);

                    if(num == 10 && GetFullPotFighterNum() == 10)
                    {
                        //十个人全满
                        
                        GameAction()->doAttainment(this,Script::TEN_FIGHTER_PC_FULL  , 10);
                    }

                    if(bMainFighter)
                        //主将潜质练满
                        GameAction()->doAttainment(this, Script::MainFighterCapFull , 0);

                }

                if(bMainFighter)
                {
                    GameAction()->doAttainment(this, Script:: AddMainFighterCapacity , static_cast<UInt32>(p));
                }

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

        if (fgt->getPotential() + 0.005f >= 1.2f)
            fgt->getAttrType2(true);
        if (fgt->getPotential() + 0.005f >= 1.5f && fgt->getCapacity() >= 7.0f)
            fgt->getAttrType3(true);

#ifdef _FB
        if (bMainFighter && World::getFighter1368())
        {
            if (fgt->getPotential() + 0.005f >= 1.3f && fgt->getCapacity() >= 6.8f && !GetVar(VAR_FIGHTER1_3_6_8))
            {
                SetVar(VAR_FIGHTER1_3_6_8, 1);

                SYSMSG(title, 3000);
                SYSMSG(content, 3001);
                Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFD0000);
                if(mail)
                {
                    MailPackage::MailItem mitem[1] = {{30,6}};
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
            }
        }
#endif

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

	bool Player::testCanAddCFriend( Player * pl )
	{
		Mutex::ScopedLock lk(_mutex);
		Mutex::ScopedLock lk2(pl->getMutex());
		if(isCFriendFull())
			return false;

		if(_hasCFriend(pl))
			return false;

#if 0
		if(_hasBlock(pl))
		{
			sendMsgCode(2, 1500);
			return false;
		}
#endif

		if(pl->isCFriendFull())
			return false;

#if 0
		if(pl->_hasBlock(this))
		{
			sendMsgCode(2, 1502);
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

    static char nameStr[2048];
    const char* Player::patchShowName(const char* name, const UInt64 playerId)
    {
        if(cfg.merged)
        {
            unsigned short len = 0;
            const char *pname = name;
            while(pname[len] && len < 2048)
                ++len;
            strncpy(nameStr, name, len);
#if 0
            char a = nameStr[len - 1];
            char b = nameStr[len - 2];
            char serverNo = a * 31 + b;
            nameStr[len - 2] = '\0';
#else
            unsigned char a;
            unsigned char serverNo = 0;
            for(unsigned short i = len; i >= 1 && i >= len - 1; i--)
            {
                a = static_cast<unsigned char>(nameStr[i - 1]) - static_cast<unsigned char>(1);
                if(a > static_cast<unsigned char>(31))
                    break;
                serverNo = serverNo * static_cast<unsigned char>(31) + a;
                nameStr[i -1] = '\0';
            }
            serverNo += 1; //起点从0开始
#endif
            if(playerId != 0 && serverNo != static_cast<unsigned char>(playerId >> 48))
            {
                char tmp[32];
                sprintf(tmp, ".S%d", serverNo);
                strcat(nameStr, tmp);
            }
            return reinterpret_cast<const char*>(nameStr);
        }
        return name;
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
		if(h > 15)
			h = 15;
		for(UInt32 j = l; j <= h; ++j)
		{
			SYSMSG(title, 256);
			SYSMSGV(content, 257, j);
			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
			if(mail == NULL)
				continue;

			const UInt32 vipTable[16][12] =
            {
                {450,1,0,0,0,0,0,0,0,0,0,0},
                {451,1,0,0,0,0,0,0,0,0,0,0},
                {452,1,0,0,0,0,0,0,0,0,0,0},
                {453,1,0,0,0,0,0,0,0,0,0,0},
                {454,1,0,0,0,0,0,0,0,0,0,0},
                {455,1,0,0,0,0,0,0,0,0,0,0},
                {456,1,0,0,0,0,0,0,0,0,0,0},
                {457,1,0,0,0,0,0,0,0,0,0,0},
                {458,1,0,0,0,0,0,0,0,0,0,0},
                {459,1,0,0,0,0,0,0,0,0,0,0},
                {460,1,0,0,0,0,0,0,0,0,0,0},
                {461,1,0,0,0,0,0,0,0,0,0,0},
                {462,1,0,0,0,0,0,0,0,0,0,0},
                {463,1,0,0,0,0,0,0,0,0,0,0},
                {464,1,0,0,0,0,0,0,0,0,0,0},
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

            // XXX: 改成发礼包，在礼包使用里给装备 yangyoufa@ 23/02/12 10:21:47 
#if 0
            if (j >= 5) // XXX: 玩家等级橙色装备x1
            {
                UInt16 id = getRandOEquip(GetLev());
                mitem[mcount].id = id;
                mitem[mcount++].count = 1;
				strItems += Itoa(id);
				strItems += ",";
				strItems += Itoa(1);
            }
#endif

			mailPackageManager.push(mail->id, mitem, mcount, true);
			DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, title, content, strItems.c_str(), mail->recvTime);
		}
	}

	void Player::sendRechargeMails( UInt8 l, UInt8 h, UInt8 m )
	{
		if(l < 1)
			l = 1;
		if(h > m)
			h = m;

		for(UInt32 j = l; j <= h; ++j)
		{
			SYSMSGV(title, 2320, levelToRecharge(j));
			SYSMSGV(content, 2321, levelToRecharge(j));
			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
			if(mail == NULL)
				continue;

#if 0
            //春节充值活动额度
			const UInt32 vipTable[16][14] =
            {
                {430,2,0,0,0,0,0,0,0,0,0,0,0,0},
                {430,2,6006,2,0,0,0,0,0,0,0,0,0,0},
                {431,2,6006,3,6008,1,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,1000,MailPackage::Coupon,20,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,2000,MailPackage::Coupon,40,0,0,0,0,0,0,0,0,0,0},
                {432,2,6006,5,507,2,509,2,6008,3,30,2,MailPackage::Coupon,500},
                {MailPackage::Tael,3000,MailPackage::Coupon,60,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,4000,MailPackage::Coupon,80,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,5000,MailPackage::Coupon,100,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,6000,MailPackage::Coupon,120,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,7000,MailPackage::Coupon,140,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,8000,MailPackage::Coupon,160,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,9000,MailPackage::Coupon,180,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Tael,10000,MailPackage::Coupon,200,0,0,0,0,0,0,0,0,0,0},
                {432,10,6008,6,507,5,509,5,30,10,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            };
            // XXX: 2012年03月充值反利活动
			const UInt32 vipTable[16][14] =
            {
                {514,3,0,0,0,0,0,0,0,0,0,0,0,0},
                {503,3,0,0,0,0,0,0,0,0,0,0,0,0},
                {516,3,0,0,0,0,0,0,0,0,0,0,0,0},
                {514,5,503,5,516,5,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,80,0,0,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,100,0,0,0,0,0,0,0,0,0,0,0,0},
                {507,2,509,2,30,10,MailPackage::Coupon,100,0,0,0,0,0,0},
                {MailPackage::Coupon,120,0,0,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,140,0,0,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,160,0,0,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,180,0,0,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,200,0,0,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,220,0,0,0,0,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,240,0,0,0,0,0,0,0,0,0,0,0,0},
                {515,10,507,10,509,10,30,10,MailPackage::Coupon,300,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            };
            //2012年5月16充值反利活动
			const UInt32 vipTable[16][14] =
            {
                {503,3,514,4,9012,5,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,50,9012,5,0,0,0,0,0,0,0,0,0,0},
                {516,3,514,6,9012,5,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,75,9013,5,0,0,0,0,0,0,0,0,0,0},
                {501,6,513,2,56,3,57,3,9013,5,0,0,0,0},
                {MailPackage::Coupon,100,9013,5,0,0,0,0,0,0,0,0,0,0},
                {515,3,505,4,9014,5,0,0,0,0,0,0,0,0},
                {MailPackage::Coupon,200,9014,5,0,0,0,0,0,0,0,0,0,0},
                {507,3,509,3,56,2,57,2,9014,5,0,0,0,0},
                {MailPackage::Coupon,300,9015,5,0,0,0,0,0,0,0,0,0,0},
                {515,10,30,10,56,5,57,5,9015,5,0,0,0,0},
                {507,3,509,3,515,3,547,5,9015,5,0,0,0,0},
                {507,5,509,5,515,5,547,5,9016,5,0,0,0,0},
                {507,7,509,7,515,7,547,5,9016,5,0,0,0,0},
                {507,10,509,10,515,10,547,5,9016,5,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            };
#else
            //2012年6月6日充值反利活动
			const UInt32 vipTable[18][14] =
            {
                {503,2,514,1,509,1,0,0,0,0,0,0,0,0},
                {500,3,56,6,57,2,509,1,0,0,0,0,0,0},
                {508,2,56,6,57,5,509,1,0,0,0,0,0,0},
                {511,6,466,6,509,1,0,0,0,0,0,0,0,0},
                {516,3,512,2,509,1,0,0,0,0,0,0,0,0},
                {5065,1,56,5,509,1,0,0,0,0,0,0,0,0},
                {503,5,56,6,57,2,509,1,0,0,0,0,0,0},
                {515,2,56,6,57,2,509,2,0,0,0,0,0,0},
                {515,2,56,6,57,5,509,2,0,0,0,0,0,0},
                {515,2,56,6,57,6,509,2,0,0,0,0,0,0},
                {549,2,56,6,57,6,509,2,0,0,0,0,0,0},
                {515,5,30,10,56,5,57,5,509,3,0,0,0,0},
                {507,5,509,5,515,5,547,5,0,0,0,0,0,0},
                {507,10,509,10,515,5,547,5,0,0,0,0,0,0},
                {507,10,509,10,515,10,547,5,0,0,0,0,0,0},
                {507,15,509,15,515,15,547,5,0,0,0,0,0,0},
                {507,20,509,20,515,20,547,10,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            };
#endif

			MailPackage::MailItem mitem[7];
			UInt32 mcount = 0;
            std::string strItems;

			const UInt32 * t = vipTable[j-1];
			for(UInt32 i = 0; i < 14 && t[i] > 0; i += 2)
			{
                if (!t[i])
                    break;

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

    void Player::sendJuneRechargeMails(UInt32 value)
    {
        UInt32 count = value / 20;
        if(count > 0)
        {
			SYSMSGV(title, 2324);
			SYSMSG(content, 2325);
            int cnt = count / 255;
            int left = count % 255;
            std::string strItems;
            for(int i = 0; i < cnt; ++ i)
            {
                Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                if(mail == NULL)
                    return;

                MailPackage::MailItem mitem;
                mitem.id = 9028;
                mitem.count = 255;
                strItems += Itoa(mitem.id);
                strItems += ",";
                strItems += Itoa(255);
                strItems += "|";

                mailPackageManager.push(mail->id, &mitem, 1, true);

                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
            }
            if(left > 0)
            {
                Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
                if(mail == NULL)
                    return;
                MailPackage::MailItem mitem;
                mitem.id = 9028;
                mitem.count = left;
                strItems += Itoa(mitem.id);
                strItems += ",";
                strItems += Itoa(left);
                strItems += "|";

                mailPackageManager.push(mail->id, &mitem, 1, true);

                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
            }
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
		bsim.applyFighterHP(0, this, !hasFlag(CountryBattle | ClanBattling | ClanRankBattle));
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

    void Player::OnDoAttainment(UInt32 attId,   UInt32 param)
    {
        GameAction()->doAttainment(this, attId, param);
    }

    void Player::OnHeroMemo(UInt8 chapter, UInt8 diff, UInt8 group, UInt8 item)
    {
        if (CURRENT_THREAD_ID() != getThreadId())
        {
            UInt8 msg[4] = {chapter, diff, group, item};
            GameMsgHdr h(0x240,  getThreadId(), this, sizeof(msg));
            GLOBAL().PushMsg(h, &msg);
        }
        else
            GetHeroMemo()->setMemo(chapter, diff, group, item, 1);
    }

    void Player::OnShuoShuo(UInt8 idx)
    {
        if (!World::getShuoShuo())
            return;
        if (CURRENT_THREAD_ID() != getThreadId())
        {
            GameMsgHdr h(0x243,  getThreadId(), this, sizeof(idx));
            GLOBAL().PushMsg(h, &idx);
        }
        else
            GetShuoShuo()->setShuoSafe(idx);
    }

	void Player::setClan(Clan * c)
	{
		if(c == _clan)
			return;
		_clan = c;
		rebuildBattleName();

        //加入帮会成就
        if(IsMainThread() == false  &&  CURRENT_THREAD_ID() <= WORKER_THREAD_NEUTRAL)
            GameAction()->doAttainment(this,Script::JOIN_CLAN,0 );
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
#ifdef _ARENA_SERVER
        char numstr[16];
        sprintf(numstr, "%u\n", _playerData.title);
        GameServer * gs = gameServers(_channelId, _serverId);
        if(gs != NULL)
        {
            char chstr[16];
            sprintf(chstr, ".S%u", _serverId);
            _battleName = gs->getChannel() + chstr + "\n" + numstr + _playerData.name;
            _displayName = _playerData.name + "@" + gs->getChannel() + chstr;
        }
        else
        {
            _battleName = std::string("\n") + numstr + _playerData.name;
            _displayName = _playerData.name;
        }
#else
		char numstr[16];
		sprintf(numstr, "%u", _playerData.title);
		_battleName.clear();
		_battleName = getClanName();
        /*
        if(cfg.merged && !_battleName.empty() && static_cast<UInt8>(*(_battleName.end() - 1)) < 32)
		{
			sprintf(numstr, ".S%u", static_cast<UInt8>(*(_battleName.end() - 1)));
			_battleName.erase(_battleName.end() - 1);
			_battleName += numstr;
		}
        */
		_battleName = _battleName + "\n" + numstr + "\n" + _playerData.name;
#endif
	}

#ifdef _ARENA_SERVER
    void Player::setEntered( UInt8 e )
    {
        if(_playerData.entered == e)
            return;
        _playerData.entered = e;
        DB().PushUpdateData("UPDATE `player` SET `entered` = %u WHERE `id` = %"I64_FMT"u", e, _id);
    }

    UInt64 Player::getOriginId(UInt64 id)
    {
        int channelId = static_cast<int>(id >> 40) & 0xFF, serverId = static_cast<int>(id >> 48);
        if(gameServers.isMerged(channelId, serverId))
            return id & 0xFFFF00FFFFFFFFFFull;
        return id & 0xFFFFFFFFFFull;
    }

    UInt64 Player::getOriginId()
    {
        if(gameServers.isMerged(_channelId, _serverId))
            return _id & 0xFFFF00FFFFFFFFFFull;
        return _id & 0xFFFFFFFFFFull;
    }

    int Player::getRealServerId()
    {
        GameServer * gs = gameServers(_channelId, _serverId);
        if(gs == NULL)
            return 0;
        if(gs->getMainId() < 0)
            return _serverId;
        return gs->getMainId();
    }

    int Player::getRealCSId()
    {
        GameServer * gs = gameServers(_channelId, _serverId);
        if(gs == NULL)
            return 0;
        if(gs->getMainId() < 0)
            return (_serverId << 8) + _channelId;
        return (gs->getMainId() << 8) + _channelId;
    }
#endif

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
                    count = 1;
                    updateNextBookStoreUpdate(curtime);
                }
                else
                {
                    count = 1;
                    money = GData::moneyNeed[GData::BOOK_LIST].tael;
                }

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
        if(type > 0)
            GameAction()->doAty(this, AtyBookStore, 0 , 0);
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
        UInt32 tmp = _bookStoreInterval;
        if(this->getPlatform() == 10 && World::getQQGameAct())
            tmp /= 2;
        if(tmp == 0)
            tmp = 1;
        _nextBookStoreUpdate = (curtime + tmp) / tmp * tmp;
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
        if (GetLev() < 30)
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

	UInt8 Player::deactiveSecondPWD(std::string answer, bool force)
	{
		if(!force && answer != _pwdInfo.answerForPWD)
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
		if(newPWD.length() != 6)
			return 1;
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
            UInt32 left = getBuffLeft(PLAYER_BUFF_PWDLOCK, TimeUtil::Now());
			if(left != 0)
			{
				sendMsgCode(0, 1009, left);
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

    float Player::getPracticeIncByDiamond()
    {
        if(this->isBD() && World::getBlueDiamondAct())
        {
            return 0.1;
        }
        else if(this->isYD() && World::getYellowDiamondAct())
        {
            return 0.1;
        }
        else
        {
            return 0.0;
        }
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

    void Player::OnSelectCountry()
    {

        GameAction()->doAttainment(this,  Script::SELECT_COUNTRY , 0);
    }
    void Player::setCountry(UInt8 cny)
    { 
        _playerData.country = cny;
		DB1().PushUpdateData("UPDATE `player` SET `country` = %u WHERE `id` = %"I64_FMT"u", cny, getId());

		Stream st(REP::USER_INFO_CHANGE);
		st << static_cast<UInt8>(0x11) << static_cast<UInt32>(cny) << Stream::eos;
		send(st);
    }

    void Player::startAutoCopy(UInt8 id, UInt8 mtype = 0)
    {
        playerCopy.autoBattle(this, id, 0, mtype);
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

    void Player::startAutoFrontMap(UInt8 id, UInt8 mtype = 0)
    {
        frontMap.autoBattle(this, id, 0, mtype);
    }

    void Player::cancelAutoFrontMap(UInt8 id)
    {
        frontMap.autoBattle(this, id, 1);
    }

    void Player::instantAutoFrontMap(UInt8 id)
    {
        frontMap.autoBattle(this, id, 2);
    }

    void Player::sendAutoFrontMap()
    {
        frontMap.sendAutoFrontMap(this);
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

            for(int i = 0; i < MAX_PRACTICE_FIGHTRES; ++ i)
            {
                Fighter* fgt = findFighter(pfexp->fids[i]);
                if(fgt && pfexp->counts[i])
                {
                    fgt->addPExp(fgt->getPracticeInc() * pfexp->counts[i]); 
                }
            }

            UInt32 now = TimeUtil::Now();
            UInt32 duration = 60*60;
            UInt32 p = getBuffData(PLAYER_BUFF_PROTECT, now);
            UInt32 left = 0;
            if (p > 0)
                left = p - now;

            if (left >= duration)
            {
                left -= duration;
                setBuffData(PLAYER_BUFF_PROTECT, left+now);
            }
            else if (left)
            {
                setBuffData(PLAYER_BUFF_PROTECT, 0);
            }
        }
        else
        {
            for(int i = 0; i < MAX_PRACTICE_FIGHTRES; ++ i)
            {
                Fighter* fgt = findFighter(pfexp->fids[i]);
                if(fgt && pfexp->counts[i])
                {
                    fgt->addPExp(fgt->getPracticeInc() * pfexp->counts[i]); 
                }
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
        UInt8 qqvipl = 0;
        UInt8 flag = 0;

        if (_playerData.qqvipl >= 20 && _playerData.qqvipl < 40) // 20-29 - 3366, 30-39 - Q+ 40-49 - qqvip
        {
            qqvipl = _playerData.qqvipl1;
            flag = 8*(_playerData.qqvipl1 / 10);
        }
        else
        {
            if (_playerData.qqvipl >= 40)
            {
                qqvipl = _playerData.qqvipl;
                flag = 8*((_playerData.qqvipl-20) / 10);
            }
            else
            {
                qqvipl = _playerData.qqvipl;
                flag = 8*(_playerData.qqvipl / 10);
            }
        }

        if(flag)
        {
            if(qqvipl % 10 == 0)
                qqvipl = 0;
            else
                qqvipl = qqvipl%10 + 1;
        }

        if(now >= _playerData.qqawardEnd)
        {
            _playerData.qqawardEnd = TimeUtil::SharpDay(1, now);
            _playerData.qqawardgot &= 0xFCFC;
            DB1().PushUpdateData("UPDATE `player` SET `qqawardEnd` = %u, `qqawardgot` = %u WHERE `id` = %"I64_FMT"u", _playerData.qqawardEnd, _playerData.qqawardgot, getId());
            RollYDGem();
        }

        if(isQQVIP())
        {
            if(GetFreePackageSize() >= 1 && GetVar(VAR_QQVIP) == 0)
            {
                SetVar(VAR_QQVIP, 1);
                GetPackage()->AddItem2(9084, 1, true, true);
            }
        }
        else if( !(_playerData.qqawardgot & (0x80<<flag)) && qqvipl )
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

        bool blue = false;
        bool qplus = false;
        UInt8 domain = atoi(m_domain.c_str());
        if (domain == 11 && _playerData.qqvipl >= 20 && _playerData.qqvipl < 40)
        {
            Stream st(REP::YD_INFO);

            UInt8 qqvipl = _playerData.qqvipl % 10;
            st << qqvipl << _playerData.qqvipyear << static_cast<UInt8>(GetVar(VAR_AWARD_3366));
            UInt8 maxCnt = GObjectManager::getD3D6MaxCount();
            st << maxCnt;
            st << static_cast<UInt8>(1);
            for(UInt8 i = 0; i < maxCnt; ++ i)
            {
                std::vector<YDItem>& ydItem = GObjectManager::getD3D6Item(i);
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
            st << static_cast<UInt8>(0);
            st << Stream::eos;
            send(st);

            blue = true;
        }
        else if (domain == 4 && _playerData.qqvipl >= 30 && _playerData.qqvipl < 40)
        {
            // TODO:
            Stream st(REP::YD_INFO);

            UInt8 qqvipl = _playerData.qqvipl % 10;
            st << qqvipl << _playerData.qqvipyear << static_cast<UInt8>(GetVar(VAR_AWARD_QPLUS));
            UInt8 maxCnt = GObjectManager::getQPlusMaxCount();
            st << maxCnt;
            st << static_cast<UInt8>(1);
            for(UInt8 i = 0; i < maxCnt; ++ i)
            {
                std::vector<YDItem>& ydItem = GObjectManager::getQPlusItem(i);
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
            st << static_cast<UInt8>(0);
            st << Stream::eos;
            send(st);

            qplus = true;
        }

        if (_playerData.qqvipl < 20 || _playerData.qqvipl >= 40 || blue || qplus)
        {
            Stream st(REP::YD_INFO);

            UInt8 qqvipl = 0;
            UInt8 flag = 0;

            if (blue || qplus)
            {
                flag = 8*(_playerData.qqvipl1 / 10);
                qqvipl = _playerData.qqvipl1 % 10;
            }
            else
            {
                if (_playerData.qqvipl >= 40)
                {
                    flag = 8*((_playerData.qqvipl-20) / 10);
                    qqvipl = _playerData.qqvipl % 10;
                }
                else
                {
                    flag = 8*(_playerData.qqvipl / 10);
                    qqvipl = _playerData.qqvipl % 10;
                }
            }

            st << qqvipl << _playerData.qqvipyear << static_cast<UInt8>((_playerData.qqawardgot>>flag) & 0x03);
            UInt8 maxCnt = GObjectManager::getYDMaxCount();
            if(flag == 8)
                st << static_cast<UInt8>(maxCnt - 2);
            else if (flag == 16)
                st << static_cast<UInt8>(maxCnt - 1);
            else
                st << maxCnt;
            st << static_cast<UInt8>(0);

            UInt8 factor = 1;
            if (_playerData.qqvipl >= 1 && _playerData.qqvipl <= 10 && (domain == 1 || domain == 2 || domain == 4) && World::getYDMDAct())
                factor = 2;

            for(UInt8 i = 0; i < maxCnt; ++ i)
            {
                if(flag == 8 && (i == 0 || i > 6))
                    continue;
                if (flag == 16 && i > 6)
                    continue;
                std::vector<YDItem>& ydItem = GObjectManager::getYDItem(i);
                UInt8 itemCnt = ydItem.size();
                st << itemCnt;
                for(int j = 0; j < itemCnt; ++ j)
                {
                    UInt32 itemId = ydItem[j].itemId;
                    if(GetItemSubClass(itemId) == Item_Gem)
                        itemId = _playerData.ydGemId;

                    st << itemId << static_cast<UInt8>(ydItem[j].itemNum*factor);
                }
            }

            std::vector<YDItem>& ydItem = GObjectManager::getYearYDItem();
            UInt8 itemCnt = ydItem.size();
            st << itemCnt;
            for(UInt8 j = 0; j < itemCnt; ++ j)
                st << ydItem[j].itemId << static_cast<UInt8>(ydItem[j].itemNum*factor);
            st << Stream::eos;
            send(st);
        }
    }

    UInt8 Player::rcvYellowDiamondAward(UInt8 type, UInt8 d3d6)
    {
        checkQQAward();

        UInt8 nRes = 0;
        Stream st(REP::YD_AWARD_RCV);

        UInt8 domain = atoi(m_domain.c_str());
        if (domain == 11 && _playerData.qqvipl >= 20 && _playerData.qqvipl < 40 && d3d6 == 1)
        {
            UInt8 qqvipl = _playerData.qqvipl % 10;
            if (!qqvipl)
                return 0;

            UInt32 award = GetVar(VAR_AWARD_3366);
            if (!award)
            {
                std::vector<YDItem>& ydItem = GObjectManager::getD3D6Item(qqvipl - 1);
                UInt8 itemCnt = ydItem.size();
                if(GetPackage()->GetRestPackageSize() > ydItem.size() - 1)
                {
                    nRes = 3;
                    SetVar(VAR_AWARD_3366, 1);
                    for(int j = 0; j < itemCnt; ++ j)
                    {
                        UInt32 itemId = ydItem[j].itemId;
                        if(GetItemSubClass(itemId) == Item_Gem)
                            itemId = _playerData.ydGemId;

                        GetPackage()->AddItem2(itemId, ydItem[j].itemNum, true, true);
                    }

#ifdef _FB
#else
                    dclogger.d3d6(this);
#endif
                }
                else
                {
                    sendMsgCode(2, 1011);
                }
            }
        }
        else if (domain == 4 && _playerData.qqvipl >= 30 && _playerData.qqvipl < 40 && d3d6/*qplus*/ == 1)
        {
            UInt8 qqvipl = _playerData.qqvipl % 10;

            UInt32 award = GetVar(VAR_AWARD_QPLUS);
            if (!award)
            {
                std::vector<YDItem>& ydItem = GObjectManager::getQPlusItem(qqvipl);
                UInt8 itemCnt = ydItem.size();
                if(GetPackage()->GetRestPackageSize() > ydItem.size() - 1)
                {
                    nRes = 4;
                    SetVar(VAR_AWARD_QPLUS, 1);
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
        }
        else if (_playerData.qqvipl < 20 || _playerData.qqvipl >= 40 || ((domain == 11 || domain == 4) && d3d6 == 0 && _playerData.qqvipl1 > 0))
        {
            UInt8 qqvipl = 0;
            UInt8 flag = 0;
            if ((domain == 11 || domain == 4) && d3d6 == 0 && _playerData.qqvipl1 > 0)
            {
                qqvipl = _playerData.qqvipl1;
                flag = 8*(_playerData.qqvipl1 / 10);
            }
            else
            {
                if (_playerData.qqvipl >= 40)
                {
                    qqvipl = _playerData.qqvipl;
                    flag = 8*((_playerData.qqvipl-20) / 10);
                }
                else
                {
                    qqvipl = _playerData.qqvipl;
                    flag = 8*(_playerData.qqvipl / 10);
                }
            }

            if(flag == 8)
            {
                if(qqvipl % 10 == 0)
                    qqvipl = 0;
                else
                    qqvipl = qqvipl%10 + 1;
            }
            else if (flag == 16)
            {
                qqvipl %= 10;
            }

            UInt8 factor = 1;
            if (_playerData.qqvipl >= 1 && _playerData.qqvipl <= 10 && (domain == 1 || domain == 2 || domain == 4) && World::getYDMDAct())
                factor = 2;

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

                        GetPackage()->AddItem2(itemId, factor*ydItem[j].itemNum, true, true);
                    }

#ifdef _FB
#else
                    dclogger.blue(this);
#endif
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
                        GetPackage()->AddItem2(ydItem[j].itemId, factor*ydItem[j].itemNum, true, true);
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

    bool Player::IsFighterEquipEnchantLev(UInt8 en, UInt8 num)
    {
        UInt8 cur = 0;
        for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
        {
            if(it->second->IsEquipEnchantLev(en))
            {
                cur ++;
                if(cur >= num)
                    return true;
            }
        }
        
       return false;

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
                pk->DelItem(itemid, num, bind, ToDesdroy);
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
        {0,     85,     100,    0,      0,      0},
        {0,     60,     100,    0,      0,      0},
        {30,    95,     0,      100,    0,      0},
        {0,     60,     0,      0,      100,    0},
        {85,    0,      0,      100,    0,      0},
        {60,    0,      0,      100,    0,      0},
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

        GameAction()->doAty(this, AtyTripodFire , 0, 0);
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
        if (getVipLevel() > 2)
            m_td.quality = 3;
        else
            m_td.quality = 2;
        m_td.needgen = 1;
        m_td.awdst = 0;
        m_td.soul = 0;
        m_td.itemId = 0;
        m_td.num = 0;
        DB6().PushUpdateData("UPDATE `tripod` SET `soul`=0, `fire`=0, `quality`=%u, `awdst`=0, `itemId`=0, `num`=0, `regen`=1 WHERE `id` = %"I64_FMT"u",
                m_td.quality, getId());
        runTripodData(m_td);
        sendTripodInfo();
    }

    void Player::getAward(UInt8 type, UInt8 opt)
    {
        switch(type)
        {
        case 1:
            // 搜搜地图
            getSSDTAward(opt);
            break;
        case 2:
            // 欢乐大转盘
            getHappyAward(opt);
            break;
        case 3:
            // 今日目标
            getTargetAward(opt);
            break;
            // 回流今日目标
        case 4:
            getTargetAwardRF(opt);
            break;
        }
    }

    void Player::getSSDTAward(UInt8 opt)
    {
        if(!World::getSSDTAct() || opt > 3)
            return;

        UInt8 status = GetVar(VAR_AWARD_SSDT_2);
        // 点亮每日旗帜
        if(opt == 0)
        {
            if(GetVar(VAR_AWARD_SSDT_1))
                return;
            if(!GameAction()->RunSSDTAward(this, opt))
                return;

            ++ status;
            SetVar(VAR_AWARD_SSDT_1, 1);
            SetVar(VAR_AWARD_SSDT_2, status);
        }
        else
        {
            static UInt8 flags[] = {1, 2, 4, 7};
            if( (1 << opt) & (status >> 4) )
                return;
            if(flags[opt] > (status & 0x0F))
                return;
            if(!GameAction()->RunSSDTAward(this, opt))
                return;

            status |= (1 << (opt + 4));
            SetVar(VAR_AWARD_SSDT_2, status);
        }

        sendSSDTInfo();
    }

    void Player::sendSSDTInfo()
    {
        if(!World::getSSDTAct())
            return;

        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(1);
        UInt8 status = GetVar(VAR_AWARD_SSDT_2);
        if(GetVar(VAR_AWARD_SSDT_1))
            status |= (1 << 4);
        st << status << Stream::eos;
        send(st);
    }

    void Player::getHappyAward(UInt8 opt)
    {
        if(opt == 6)
        {
			std::vector<GData::LootResult>::iterator it;
			for(it = _lastLoot.begin(); it != _lastLoot.end(); ++ it)
			{
				m_Package->ItemNotify(it->id, it->count);
			}
			_lastLoot.clear();
        }

        if(!World::getJune() || opt > 5)
            return;

        UInt16 itemId = 0;
        UInt8 status = GetVar(VAR_JUNE_ITEM);
        // 转到转盘
        if(opt == 0)
        {
            UInt32 happy = GetVar(VAR_JUNE_HAPPY);
            if(happy < 20)
                return;
            if(0 == (itemId = GameAction()->RunHappyAward(this, opt)))
                return;

            GData::LootResult lt = {itemId, 1};
            _lastLoot.push_back(lt);
            SetVar(VAR_JUNE_HAPPY, happy - 20);
        }
        else
        {
            if( (1 << (opt - 1)) & status )
                return;
            if(0 == GameAction()->RunHappyAward(this, opt))
                return;

            status |= (1 << (opt - 1));
            SetVar(VAR_JUNE_ITEM, status);
        }

        sendHappyInfo(itemId);
    }

    void Player::getTargetAward(UInt8 opt)
    {
        UInt8 idx = 1;
        // 转到转盘
        if(opt == 0 && 1 == GetVar(VAR_CLAWARD2))
        {
            idx = GameAction()->RunTargetAward(this);
            SetVar(VAR_CLAWARD2, 2);
            Stream st(REP::GETAWARD);
            st << static_cast<UInt8>(3);
            st << idx;
            st << Stream::eos;
            send(st);
        }
        else if(opt == 1)
        {
			std::vector<GData::LootResult>::iterator it;
			for(it = _lastLoot.begin(); it != _lastLoot.end(); ++ it)
			{
				m_Package->ItemNotify(it->id, it->count);
			}
			_lastLoot.clear();
        }
    }

    void Player::getTargetAwardRF(UInt8 opt)
    {
        UInt8 idx = 1;
        // 转到转盘
        if(opt == 0 && 1 == GetVar(VAR_CTSAWARDRF))
        {
            idx = GameAction()->RunTargetAwardRF(this);
            SetVar(VAR_CTSAWARDRF, 2);
            Stream st(REP::GETAWARD);
            st << static_cast<UInt8>(4);
            st << idx;
            st << Stream::eos;
            send(st);
        }
        else if(opt == 1)
        {
			std::vector<GData::LootResult>::iterator it;
			for(it = _lastLoot.begin(); it != _lastLoot.end(); ++ it)
			{
				m_Package->ItemNotify(it->id, it->count);
			}
			_lastLoot.clear();
        }
    }

    void Player::lastLootPush(UInt16 itemId, UInt16 num)
    {
        GData::LootResult lt = {itemId, num};
        _lastLoot.push_back(lt);
    }

    void Player::sendHappyInfo(UInt16 itemId)
    {

        if(!World::getJune())
            return;

        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(2);
        UInt32 happy = GetVar(VAR_JUNE_HAPPY);
        UInt8 status = GetVar(VAR_JUNE_ITEM);
        st << happy << static_cast<UInt8>(happy >= 20 ? 1 : 0) << itemId << status << Stream::eos;
        send(st);
    }

    TripodData& Player::runTripodData(TripodData& data, bool init)
    {
        if (&data != &m_td)
            m_td = data;

        bool update = false;
        if(getVipLevel() > 2 && m_td.quality < 3)
        {
            m_td.quality = 3;
            update = true;
        }

        EventPlayerTripod* event = new (std::nothrow) EventPlayerTripod(this, 60, MAX_TRIPOD_SOUL/POINT_PERMIN);
        if (!event) return nulltd;
        PushTimerEvent(event);

        if (init && m_td.itemId && m_td.num && m_td.needgen)
        {
            m_td.needgen = 0;
            update = true;
        }

        if (update)
        {
            DB6().PushUpdateData("UPDATE `tripod` SET `regen` = %u, `quality` = %u WHERE `id` = %"I64_FMT"u",
                    m_td.needgen, m_td.quality, getId());
        }
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

    void Player::resetThanksgiving()
    {
        SetVar(VAR_TGDT, 0);
        _playerData.lastOnline = TimeUtil::Now(); // XXX: hack
        if (isOnline())
        {
            PopTimerEvent(this, EVENT_TIMETICK, getId());
            EventPlayerTimeTick* event = new(std::nothrow) EventPlayerTimeTick(this, TGD_ONLINE_TIME, 1, 1);
            if (event) PushTimerEvent(event);
        }
    }

    TeamData* Player::getTeamData()
    {
        return m_teamData;
    }

    void Player::setTeamData(TeamData* td)
    {
        m_teamData = td;
    }

    CopyTeamPage& Player::getCopyTeamPage()
    {
        return m_ctp;
    }

    void Player::clearCopyTeamPage()
    {
        memset(&m_ctp, 0, sizeof(m_ctp));
    }

    TeamCopyPlayerInfo* Player::getTeamCopyPlayerInfo()
    {
        return m_tcpInfo;
    }

    HoneyFall* Player::getHoneyFall()
    {
        return m_hf;
    }

    void Player::addClanSkillFromDB(UInt8 skillId, UInt8 level)
    {
        ClanSkill& cs = m_clanSkill[skillId];
        cs.id = skillId;
        cs.level = level;
    }

    void Player::buildClanTechSkill()
    {
        UInt8 skillNum = GData::clanSkillTable.size();
        for(UInt8 i = 1; i < skillNum; ++ i)
            addClanSkill(i);
    }

    void Player::addClanSkill(UInt8 skillId)
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(skillId);
        if(it != m_clanSkill.end())
            return;

        ClanSkill& cs = m_clanSkill[skillId];
        cs.id = skillId;
        cs.level = 0;

        DB5().PushUpdateData("REPLACE INTO `clan_skill`(`playerId`, `skillId`, `level`) VALUES(%"I64_FMT"u, %u, 0)", getId(), skillId);
    }

    UInt8 Player::getClanSkillLevel(UInt8 skillId)
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(skillId);
        if(it == m_clanSkill.end())
            return 0xFF;

        ClanSkill& cs = it->second;
        return cs.level;
    }

    UInt8 Player::clanSkillLevelUp(UInt8 skillId)
    {
        if(m_csFlag != 1)
            return 3;

        Stream st(REP::CLAN_SKILL);
        st << static_cast<UInt8>(8) << skillId;

        UInt8 res = 0;
        do
        {
            std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(skillId);
            if(it == m_clanSkill.end())
            {
                res = 3;
                break;
            }

            ClanSkill& cs = it->second;
            ++ cs.level;
            DB5().PushUpdateData("UPDATE `clan_skill` SET `level` = %u WHERE `playerId` = %"I64_FMT"u and `skillId`=%u", cs.level, getId(), skillId);

            if(skillId == CLAN_SKILL_MAXSOUL)
            {
                for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
                {
                    Fighter * fgt = it->second;
                    fgt->sendMaxSoul();
                }
            }

            showClanSkill(skillId);

            if (skillId == CLAN_SKILL_ACTION && cs.level == 3)
                OnHeroMemo(MC_CONTACTS, MD_ADVANCED, 0, 2);
        } while(false);

        st << res;
        st << Stream::eos;
        send(st);

        GameMsgHdr hdr1(0x1F3, WORKER_THREAD_WORLD, this, 0);
        GLOBAL().PushMsg(hdr1, NULL);

        return res;
    }

    void Player::makeClanSkillInfo(Stream& st)
    {
        UInt8 cnt = static_cast<UInt8>(m_clanSkill.size());
        UInt8 skillNum = GData::clanSkillTable.size();
        if(cnt < skillNum)
        {
            buildClanTechSkill();
            cnt = static_cast<UInt8>(m_clanSkill.size());
        }

        st << cnt;
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.begin();
        for (; it != m_clanSkill.end(); ++ it)
            st << it->second.id << it->second.level;

        return;
    }

    void Player::makeClanSkillInfo(Stream& st, UInt8 skillId)
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(skillId);
        if(it == m_clanSkill.end())
            return;

        ClanSkill& skill = it->second;
        st << skill.id << skill.level;
        return;
    }

    void Player::listClanSkills()
    {
        Stream st(REP::CLAN_SKILL);
        st << static_cast<UInt8>(6);
        makeClanSkillInfo(st);
        st << Stream::eos;
        send(st);
    }

    void Player::showClanSkill(UInt8 skillId)
    {
        Stream st(REP::CLAN_SKILL);
        st << static_cast<UInt8>(7);
        makeClanSkillInfo(st, skillId);
        st << Stream::eos;
        send(st);
    }

    float Player::getClanSkillHPEffect()
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_HP);
        if(it == m_clanSkill.end())
            return 0;

        ClanSkill& skill = it->second;
        return GData::clanSkillTable[skill.id][skill.level].value;
    }

    float Player::getClanSkillAtkEffect()
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_ATTACK);
        if(it == m_clanSkill.end())
            return 0;

        ClanSkill& skill = it->second;
        return GData::clanSkillTable[skill.id][skill.level].value;
    }

    float Player::getClanSkillDefendEffect()
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_DEFEND);
        if(it == m_clanSkill.end())
            return 0;

        ClanSkill& skill = it->second;
        return GData::clanSkillTable[skill.id][skill.level].value;
    }

    float Player::getClanSkillMagAtkEffect()
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_MAGATK);
        if(it == m_clanSkill.end())
            return 0;

        ClanSkill& skill = it->second;
        return GData::clanSkillTable[skill.id][skill.level].value;
    }

    float Player::getClanSkillMagDefentEffect()
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_MAGDEF);
        if(it == m_clanSkill.end())
            return 0;

        ClanSkill& skill = it->second;
        return GData::clanSkillTable[skill.id][skill.level].value;
    }

    float Player::getClanSkillActionEffect()
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_ACTION);
        if(it == m_clanSkill.end())
            return 0;

        ClanSkill& skill = it->second;
        return GData::clanSkillTable[skill.id][skill.level].value;
    }

    float Player::getClanSkillHitrLvlEffect()
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_HITRATE);
        if(it == m_clanSkill.end())
            return 0;

        ClanSkill& skill = it->second;
        return GData::clanSkillTable[skill.id][skill.level].value;
    }

    float Player::getClanSkillMaxSoulEffect()
    {
        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_MAXSOUL);
        if(it == m_clanSkill.end())
            return 0;

        ClanSkill& skill = it->second;
        return GData::clanSkillTable[skill.id][skill.level].value;
    }

    void Player::onBlueactiveday()
    {
        // XXX: 原来是为蓝钻准备的，现在全平台也要了
        //if (!(atoi(m_domain.c_str()) == 11 || atoi(m_domain.c_str()) == 10))
        //    return;

        UInt32 online = GetOnlineTimeToday();
        UInt32 maxOnline = 60 * 60;
        if (!cfg.GMCheck)
            maxOnline = 60;
        if (online < maxOnline)
        {
            EventPlayerTimeTick* event = new(std::nothrow) EventPlayerTimeTick(this, maxOnline - online, 1, 2);
            if (event) PushTimerEvent(event);
        }
        else if (!GetVar(VAR_BLUE_ACTIVE_GET))
        {
            GameAction()->onBlueactiveday(this);
        }
    }

    void Player::sendShusanLoveTitleCard(int pos)
    {
        if (!pos || pos > 3)
            return;
        MailPackage::MailItem item[3][2] =
        {
            {{442, 1}, {443, 1},},
            {{444, 1}, {445, 1},},
            {{446, 1}, {447, 1},},
        };
        sendMailItem(2331, 2332, &item[pos-1][0], 2, false);
    }

    void Player::sendMayDayTitleCard(int pos)
    {
        if (!pos || pos > 3)
            return;
        MailPackage::MailItem item[3] =
        {
            {9009, 1},
            {9007, 1},
            {9008, 1},
        };
        sendMailItem(2354, 2355, &item[pos-1], 1, false);
    }

    void Player::sendJuneHappyTitleCard(int pos)
    {
        if (!pos || pos > 2)
            return;
        MailPackage::MailItem item[2] =
        {
            {9029, 1},
            {9030, 1},
        };
        sendMailItem(2356, 2357, &item[pos-1], 1, false);
    }

    void Player::sendCreateMail()
    {
#ifdef _FB
#else
        SYSMSG(title, 2335);
        SYSMSG(content, 2336);
        GetMailBox()->newMail(NULL, 0x12, title, content);
#endif
    }

    void Player::sendOpenAct(UInt32 day)
    {
#ifdef _FB
        SYSMSGV(title, 4006, day);
        SYSMSGV(content, 4007, day);
        GetMailBox()->newMail(NULL, 0x12, title, content);
#endif
    }

    void Player::initHeroMemo()
    {
        if(GetLev() >= 45)
            GetHeroMemo()->setMemo(MC_FIGHTER, MD_STARTED, 0, 2, 1);
        if (getClan())
            GetHeroMemo()->setMemo(MC_CONTACTS, MD_ADVANCED, 0, 0, 1);

        for (std::map<UInt32, Fighter*>::iterator it = _fighters.begin(); it != _fighters.end(); ++it)
        {
            Fighter * fgt = it->second;
            if (fgt->getAcupointsBit(1) >= 1 && fgt->getAcupointsBit(1) <= 3)
                GetHeroMemo()->setMemo(MC_CITTA, MD_STARTED, 1, 0, 1);
            if (fgt->getAcupointsBit(2) == 3)
                GetHeroMemo()->setMemo(MC_CITTA, MD_STARTED, 1, 1, 1);
            if (fgt->getAcupointsBit(3) == 3)
                GetHeroMemo()->setMemo(MC_CITTA, MD_STARTED, 1, 2, 1);
        }

        if (getFrendsNum() >= 10)
            GetHeroMemo()->setMemo(MC_CONTACTS, MD_STARTED, 0, 2, 1);
        if (getFrendsNum() >= 5)
            GetHeroMemo()->setMemo(MC_CONTACTS, MD_STARTED, 0, 1, 1);
        if (getFrendsNum() >= 1)
            GetHeroMemo()->setMemo(MC_CONTACTS, MD_STARTED, 0, 0, 1);

        if (_fighters.size() > 9)
        {
            GetHeroMemo()->setMemo(MC_FIGHTER, MD_ADVANCED, 0, 1, 1);
            GetHeroMemo()->setMemo(MC_FIGHTER, MD_ADVANCED, 0, 2, 1);
        }

        std::map<UInt8, ClanSkill>::iterator it = m_clanSkill.find(CLAN_SKILL_ACTION);
        if(it != m_clanSkill.end())
        {
            ClanSkill& cs = it->second;
            if (cs.level >= 3)
                GetHeroMemo()->setMemo(MC_CONTACTS, MD_ADVANCED, 0, 2, 1);
        }
    }

    void Player::initShuoShuo()
    {
        if (GetLev() >= 40)
            GetShuoShuo()->setShuoSafe(SS_40);
        if (GetLev() >= 50)
            GetShuoShuo()->setShuoSafe(SS_50);
        if (GetLev() >= 60)
            GetShuoShuo()->setShuoSafe(SS_60);
        if (GetLev() >= 70)
            GetShuoShuo()->setShuoSafe(SS_70);
        if (GetLev() >= 80)
            GetShuoShuo()->setShuoSafe(SS_80);
        if (GetLev() >= 90)
            GetShuoShuo()->setShuoSafe(SS_90);
        if (GetLev() >= 100)
            GetShuoShuo()->setShuoSafe(SS_100);
    }

    void Player::sendDeamonAwardsInfo()
    {
        if(m_dpData->itemNum != 0 || m_dpData->attacker != NULL)
        {
            Stream st(REP::TOWN_DEAMON);
            st << static_cast<UInt8>(0x08);
            string name = "";
            if(m_dpData->attacker)
                name = m_dpData->attacker->getName();

            st << m_dpData->quitLevel << static_cast<UInt16>(m_dpData->itemId) << static_cast<UInt8>(m_dpData->itemNum) << name;
            st << Stream::eos;
            send(st);
        }
    }

    void Player::getDeamonAwards()
    {
        if(m_dpData->itemNum == 0 && m_dpData->attacker == NULL)
        {
            return;
        }
        if(m_dpData->itemNum != 0)
        {
            if(NULL != GetPackage()->AddItem2(m_dpData->itemId, m_dpData->itemNum, true, true, FromTownDeamon))
            {
                m_dpData->itemId = 0;
                m_dpData->itemNum = 0;
                m_dpData->quitLevel = 0;
                m_dpData->attacker = NULL;
                DB3().PushUpdateData("UPDATE `towndeamon_player` SET `itemId`=0, `itemNum`=0, `quitLevel`=0, `attacker`=0 WHERE `playerId` = %"I64_FMT"u", getId());
            }
        }
        else
        {
            m_dpData->quitLevel = 0;
            m_dpData->attacker = NULL;
            DB3().PushUpdateData("UPDATE `towndeamon_player` SET `quitLevel`=0, `attacker`=0 WHERE `playerId` = %"I64_FMT"u", getId());
        }
    }

    void Player::setInvitedBy(UInt64 id, bool writedb)
    {
        if (!id)
            return;
        if (_invitedBy)
            return;
        _invitedBy = id;
        if (writedb)
        {
            DB3().PushUpdateData("REPLACE INTO `cfriend_awards` (`playerId`, `invitedId`, `awards`) VALUES (%"I64_FMT"u, %"I64_FMT"u, '')", _id, id);
            GetCFriend()->setCFriendSafe(CF_INVITED);
        }
    }

    void Player::offlineExp(UInt32 now)
    {
        if (GetLev() <= 30)
            return;
        UInt32 lastOffline = GetVar(VAR_OFFLINE);
        if (!lastOffline)
            lastOffline = _playerData.lastOnline;
        if (now < lastOffline)
            return;
        UInt32 offline = now - lastOffline;
        if (offline < 24 * 60 * 60)
            return;
        offline -= 24 * 60 * 60;
        if (!offline)
            return;

        UInt8 lvl = GetLev();
        UInt64 exp = (offline/60)*((lvl-10)*(lvl/10)*5+25)*0.8f;
        AddVar(VAR_OFFLINE_EXP, exp);
        AddVar(VAR_OFFLINE_PEXP, offline/60);
        AddVar(VAR_OFFLINE_EQUIP, offline);
    }

    void Player::getOfflineExp()
    {
        UInt32 equip = GetVar(VAR_OFFLINE_EQUIP);
        if(equip)
        {
            if(GetPackage()->GetRestPackageSize() < this->_equipAward.size())
            {
                sendMsgCode(0, 1011);
                return;
            }
            for(UInt16 i = 0; i < this->_equipAward.size(); i++)
            {
                if(IsEquipTypeId(this->_equipAward[i].id))
                    m_Package->AddEquip(this->_equipAward[i].id, true, false, FromPExp);
                else
                    m_Package->AddItem(this->_equipAward[i].id, this->_equipAward[i].count, true, false, FromPExp);
            }
            SetVar(VAR_OFFLINE_EQUIP, 0);
        }
        UInt32 exp = GetVar(VAR_OFFLINE_EXP);
        if (exp)
        {
            AddExp(exp);
            SetVar(VAR_OFFLINE_EXP, 0);
        }

        UInt32 pexp = GetVar(VAR_OFFLINE_PEXP);
        if (pexp)
        {
            for(int i = 0; i < 5; ++i)
            {
                GObject::Fighter * fgt = getLineup(i).fighter;
                if(fgt != NULL)
                    fgt->addPExp(pexp*fgt->getPracticeInc()*0.8f, true);
            }
            SetVar(VAR_OFFLINE_PEXP, 0);
        }
    }

    void Player::sendSecondInfo()
    {
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
        {
            Fighter* fgt = it->second;
            fgt->send2ndSoulInfo();
        }
    }

    void Player::useToken(UInt8 type)
    {
        if (type > 2)
            return;

        if (GetPackage()->GetRestPackageSize() < 3)
        {
            sendMsgCode(0, 1011);
            return;
        }

        if (!GetVar(VAR_GOLD_TOKEN+type))
            return;

        static UInt16 items[3][4] = {
            {515,509,507,47},
            {503,514,501,547},
            {56,57,15,500},
        };

        bool same = true;
        UInt16 ids[3] = {0,};
        URandom rnd(time(NULL));

        for (UInt8 i = 0; i < 3; ++i)
        {
            UInt8 idx = rnd(sizeof(items[type])/sizeof(UInt16));
            ids[i] = items[type][idx];
            if (i && ids[i] != ids[i-1])
                same = false;
        }

        if (same)
        {
            GetPackage()->AddItem(ids[0], 6, true, false);
        }
        else
        {
            for (UInt8 i = 0; i < 3; ++i)
            {
                GetPackage()->AddItem(ids[i], 1, true, false);
            }
        }

        Stream st(REP::TOKEN);
        st << static_cast<UInt8>(1) << ids[0] << ids[1] << ids[2] << Stream::eos;
        send(st);

        SetVar(VAR_GOLD_TOKEN+type, GetVar(VAR_GOLD_TOKEN+type)-1);
        sendTokenInfo();

        if (type == 0 && !same)
            SYSMSG_BROADCASTV(2351, _playerData.country, _playerData.name.c_str(), ids[0], ids[1], ids[2]);
        if (type == 0 && same)
            SYSMSG_BROADCASTV(2352, _playerData.country, _playerData.name.c_str(), ids[0]);
    }

    void Player::sendTokenInfo()
    {
        Stream st(REP::TOKEN);
        st << static_cast<UInt8>(0) << GetVar(VAR_GOLD_TOKEN) << GetVar(VAR_TAEL_TOKEN) << GetVar(VAR_COIN_TOKEN) << Stream::eos;
        send(st);
    }

    void Player::sendDiscountLimit()
    {
        static UInt8 discount[] = {3,5,8,};
        Stream st(REP::STORE_DISLIMIT);
        for (UInt8 i = 0; i < 3; ++i)
        {
            UInt8 max = GData::store.getDiscountLimit(discount[i]);
            UInt8 used = GetVar(GObject::VAR_DISCOUNT_1+i);
            if (used > max)
            {
                used = max;
                SetVar(GObject::VAR_DISCOUNT_1+i, used);
            }
            st << static_cast<UInt8>(max - used);
        }
        st << Stream::eos;
        send(st);
    }

    void Player::continuousLogin(UInt32 now)
    {
        if (!World::getRC7Day())
            return;

        if (now < getCreated())
            return;

        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 created_sharp = TimeUtil::SharpDay(0, getCreated());
        if (created_sharp > now_sharp)
            return;

        if (now_sharp - created_sharp > 7 * DAY_SECS)
            return;

        UInt32 ctslanding = GetVar(VAR_CTSLANDING);
        UInt32 off = CREATE_OFFSET(created_sharp, now_sharp);
        if (off >= 7)
            return;
        ctslanding |= (1<<off);
        SetVar(VAR_CTSLANDING, ctslanding);
    }

    void Player::continuousLoginRF(UInt32 now)
    {
        UInt32 rf = GetVar(VAR_INRF7DAY);
        if (now < rf)
            return;

        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 rf_sharp = 0;
        if (rf)
            rf_sharp = TimeUtil::SharpDay(0, rf);

        UInt32 lastOffline = GetVar(VAR_OFFLINE);
        if (!lastOffline)
            return;
        UInt32 last_sharp = TimeUtil::SharpDay(0, lastOffline);

        bool inact = false;
        if (now_sharp - last_sharp > 14 * DAY_SECS)
        {
            rf = now;
            rf_sharp = TimeUtil::SharpDay(0, rf);
            SetVar(VAR_INRF7DAY, now);

            if (GetVar(VAR_CTSLANDINGRF))
                SetVar(VAR_CTSLANDINGRF, 0);
            if (GetVar(VAR_RF7DAYRECHARGE))
                SetVar(VAR_RF7DAYRECHARGE, 0);
            if (GetVar(VAR_RF7DAYWILL))
                SetVar(VAR_RF7DAYWILL, 0);
            if (GetVar(VAR_RF7DAYTURNON))
                SetVar(VAR_RF7DAYTURNON, 0);
            if (GetVar(VAR_CTSAWARDRF))
                SetVar(VAR_CTSAWARDRF, 0);
            if (GetVar(VAR_CLAWARDRF))
                SetVar(VAR_CLAWARDRF, 0);
            if (GetVar(VAR_CL3DAYRF))
                SetVar(VAR_CL3DAYRF, 0);
            inact = true;
        }
        else if (now_sharp - rf_sharp <= 7 * DAY_SECS)
            inact = true;

        if (!inact)
            return;

        UInt32 ctslanding = GetVar(VAR_CTSLANDINGRF);
        UInt32 off = CREATE_OFFSET(rf_sharp, now_sharp);
        if (off >= 7)
            return;
        ctslanding |= (1<<off);
        SetVar(VAR_CTSLANDINGRF, ctslanding);
    }

    void Player::sendRC7DayInfo(UInt32 now)
    {
        if (!World::getRC7Day())
            return;

        if (now < getCreated())
            return;

        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 created_sharp = TimeUtil::SharpDay(0, getCreated());
        if (created_sharp > now_sharp)
            return;

        if (now_sharp - created_sharp > 7 * DAY_SECS)
            return;

        UInt32 ctslanding = GetVar(VAR_CTSLANDING);
        UInt32 off = CREATE_OFFSET(created_sharp, now_sharp);
        if (off >= 7)
            return;

        UInt32 cts = 0;
        for (int i = off; i >= 0; --i)
        {
            if (ctslanding & (1<<i))
                ++cts;
            else
                break;
        }

        UInt32 t = 0;
        UInt32 cts3 = 0;
        for (int i = off; i >= 0; --i)
        {
            if (ctslanding & (1<<i))
                ++t;
            else
            {
                if (cts3 < t)
                    cts3 = t;
                t = 0;
            }
        }
        if (cts3 < t)
            cts3 = t;

        Stream st(REP::RC7DAY);
        st << static_cast<UInt8>(0);
        st << GetVar(VAR_RC7DAYRECHARGE);
        st << static_cast<UInt8>(GetVar(VAR_RC7DAYTURNON));
        st << static_cast<UInt8>(GetVar(VAR_CTSAWARD));
        st << static_cast<UInt8>(cts);
        st << static_cast<UInt8>(GetVar(VAR_CLAWARD));
        st << static_cast<UInt8>(cts3);
        st << static_cast<UInt8>(GetVar(VAR_CL3DAY));
        st << static_cast<UInt8>(GetVar(VAR_RC7DAYWILL));
        st << static_cast<UInt8>(off + 1);
        st << static_cast<UInt8>(GetVar(VAR_CLAWARD2));
        st << Stream::eos;
        send(st);
    }

    void Player::sendRF7DayInfo(UInt32 now)
    {
        UInt32 rf = GetVar(VAR_INRF7DAY);
        if (!rf || now < rf)
            return;

        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 rf_sharp = TimeUtil::SharpDay(0, rf);
        if (now_sharp - rf_sharp > 7 * DAY_SECS)
            return;

        UInt32 ctslanding = GetVar(VAR_CTSLANDINGRF);
        UInt32 off = CREATE_OFFSET(rf_sharp, now_sharp);
        if (off >= 7)
            return;

        UInt32 cts = 0;
        for (int i = off; i >= 0; --i)
        {
            if (ctslanding & (1<<i))
                ++cts;
            else
                break;
        }

        UInt32 t = 0;
        UInt32 cts3 = 0;
        for (int i = off; i >= 0; --i)
        {
            if (ctslanding & (1<<i))
                ++t;
            else
            {
                if (cts3 < t)
                    cts3 = t;
                t = 0;
            }
        }
        if (cts3 < t)
            cts3 = t;

        Stream st(REP::RF7DAY);
        st << static_cast<UInt8>(0);
        st << GetVar(VAR_RF7DAYRECHARGE);
        st << static_cast<UInt8>(GetVar(VAR_RF7DAYTURNON));
        st << static_cast<UInt8>(cts);
        st << static_cast<UInt8>(GetVar(VAR_CLAWARDRF));
        st << static_cast<UInt8>(cts3);
        st << static_cast<UInt8>(GetVar(VAR_CL3DAYRF));
        st << static_cast<UInt8>(GetVar(VAR_RF7DAYWILL));
        st << static_cast<UInt8>(off + 1);
        st << static_cast<UInt8>(GetVar(VAR_CTSAWARDRF)); // 抽奖
        st << Stream::eos;
        send(st);
    }

    void Player::getContinuousReward(UInt8 type, UInt8 idx)
    {
        if (!World::getRC7Day())
            return;

        UInt32 now = TimeUtil::Now();
        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 created_sharp = TimeUtil::SharpDay(0, getCreated());
        if (created_sharp > now_sharp)
            return;

        if (now_sharp - created_sharp > 7 * DAY_SECS)
            return;
#if 0
        // 简体今日目标
        if (type == 9 && 1 == GetVar(VAR_CLAWARD2))
        {
            if(this->GetFreePackageSize() < 1)
            {
                this->sendMsgCode(0, 1011);
                return;
            }
            SetVar(VAR_CLAWARD2, 2);
            Stream st(REP::RC7DAY);
            UInt8 idx;
            idx = GameAction()->RunTargetAward(this);
            st << static_cast<UInt8>(2) << idx;
            st << Stream::eos;
            send(st);
            return;
        }
#endif
        UInt32 ctslanding = GetVar(VAR_CTSLANDING);
        UInt32 off = CREATE_OFFSET(created_sharp, now_sharp);
        if (off >= 7)
            return;

        UInt32 cts = 0;
        for (int i = off; i >= 0; --i)
        {
            if (ctslanding & (1<<i))
                ++cts;
            else
                break;
        }

        if (type == 1 && !GetVar(VAR_CTSAWARD))
        {
            GameAction()->onCLLoginReward(this, cts);
            SetVar(VAR_CTSAWARD, 1);
            Stream st(REP::RC7DAY);
            st << static_cast<UInt8>(1);
            st << Stream::eos;
            send(st);
            return;
        }
        if (type == 2 && !GetVar(VAR_CLAWARD) && cts)
        {
            GameAction()->onCLLoginReward(this, cts);
            SetVar(VAR_CLAWARD, 1);

            Stream st(REP::RC7DAY);
            st << static_cast<UInt8>(2);
            st << Stream::eos;
            send(st);
            return;
        }

        if (type == 3 && !GetVar(VAR_CL3DAY))
        {
            UInt32 t = 0;
            UInt32 cts3 = 0;
            for (int i = off; i >= 0; --i)
            {
                if (ctslanding & (1<<i))
                    ++t;
                else
                {
                    if (cts3 < t)
                        cts3 = t;
                    t = 0;
                }
            }
            if (cts3 < t)
                cts3 = t;

            if (cts3 >= 3)
            {
                if (addFighterFromItem(9011, 0))
                {
                    // GameAction()->onCL3DayReward(this);
                    SetVar(VAR_CL3DAY, 1);

                    Stream st(REP::RC7DAY);
                    st << static_cast<UInt8>(3);
                    st << Stream::eos;
                    send(st);
                    return;
                }
            }
        }

        if (idx && type == 4 && !GetVar(VAR_RC7DAYWILL))
        {
            if (GameAction()->onRC7DayWill(this, idx))
            {
                SetVar(VAR_RC7DAYWILL, idx);

                Stream st(REP::RC7DAY);
                st << static_cast<UInt8>(4);
                st << static_cast<UInt8>(idx);
                st << Stream::eos;
                send(st);
                return;
            }
        }
    }

    void Player::setContinuousRFAward(UInt32 no)
    {
        if (GetVar(VAR_CTSAWARDRF))
            return;

        UInt32 now = TimeUtil::Now();
        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 rf = GetVar(VAR_INRF7DAY);
        UInt32 rf_sharp = TimeUtil::SharpDay(0, rf);
        if (!rf || now < rf)
            return;
        if (now_sharp - rf_sharp > 7 * DAY_SECS)
            return;
        UInt32 off = CREATE_OFFSET(rf_sharp, now_sharp) + 1;

        if (off == no)
        {
            SetVar(VAR_CTSAWARDRF, 1);
            sendRF7DayInfo(now);
        }
    }

    void Player::getContinuousRewardRF(UInt8 type, UInt8 idx)
    {
        UInt32 now = TimeUtil::Now();
        UInt32 rf = GetVar(VAR_INRF7DAY);
        if (!rf || now < rf)
            return;

        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 rf_sharp = TimeUtil::SharpDay(0, rf);

        if (now_sharp - rf_sharp > 7 * DAY_SECS)
            return;

        UInt32 ctslanding = GetVar(VAR_CTSLANDINGRF);
        UInt32 off = CREATE_OFFSET(rf_sharp, now_sharp);
        if (off >= 7)
            return;

        UInt32 cts = 0;
        for (int i = off; i >= 0; --i)
        {
            if (ctslanding & (1<<i))
                ++cts;
            else
                break;
        }
#if 0 // XXX: 走 0x13
#ifdef _FB
        if (type == 1 && !GetVar(VAR_CTSAWARDRF))
        {
            GameAction()->onCLLoginRewardRF(this, 0, type);
            SetVar(VAR_CTSAWARDRF, 1);
            Stream st(REP::RF7DAY);
            st << static_cast<UInt8>(1);
            st << Stream::eos;
            send(st);
            return;
        }
#else
        if (type == 1 && GetVar(VAR_CTSAWARDRF) == 1)
        {
            UInt16 id = GameAction()->onCLLoginRewardRF(this, cts, type);
            if (!id)
                return;
            SetVar(VAR_CTSAWARDRF, 2);
            Stream st(REP::RF7DAY);
            st << static_cast<UInt8>(1);
            st << id;
            st << Stream::eos;
            send(st);
            return;
        }
#endif
#endif
        if (type == 2 && !GetVar(VAR_CLAWARDRF) && cts)
        {
            GameAction()->onCLLoginRewardRF(this, cts, type);
            SetVar(VAR_CLAWARDRF, 1);

            Stream st(REP::RF7DAY);
            st << static_cast<UInt8>(2);
            st << Stream::eos;
            send(st);
            return;
        }

        if (type == 3 && !GetVar(VAR_CL3DAYRF))
        {
            UInt32 t = 0;
            UInt32 cts3 = 0;
            for (int i = off; i >= 0; --i)
            {
                if (ctslanding & (1<<i))
                    ++t;
                else
                {
                    if (cts3 < t)
                        cts3 = t;
                    t = 0;
                }
            }
            if (cts3 < t)
                cts3 = t;

            if (cts3 >= 3)
            {
                if (GameAction()->onCLLoginRewardRF(this, cts3, type))
                {
                    SetVar(VAR_CL3DAYRF, 1);

                    Stream st(REP::RF7DAY);
                    st << static_cast<UInt8>(3);
                    st << Stream::eos;
                    send(st);
                    return;
                }
            }
        }

        if (idx && type == 4 && !GetVar(VAR_RF7DAYWILL))
        {
            if (GameAction()->onRC7DayWill(this, idx))
            {
                SetVar(VAR_RF7DAYWILL, idx);

                Stream st(REP::RF7DAY);
                st << static_cast<UInt8>(4);
                st << static_cast<UInt8>(idx);
                st << Stream::eos;
                send(st);
                return;
            }
        }
    }

    void Player::sendMDSoul(UInt8 type, UInt32 id)
    {
        if (!World::getMayDay())
            return;
        Stream st(REP::USESOUL);
        if (type == 0)
        {
            st << static_cast<UInt8>(0);
            st << GetVar(VAR_MDSOUL) << Stream::eos;
        }
        else if (type == 1)
        {
            st << static_cast<UInt8>(1);
            st << id << Stream::eos;
        }
        send(st);
    }

    void Player::getMDItem()
    {
        if (!World::getMayDay())
            return;

        UInt32 soul = GetVar(VAR_MDSOUL);
        if (!soul)
            return;

        UInt8 type = 0;
        UInt32 subsoul = 0;
        if (soul >= 100)
        {
            type = 1;
            subsoul = 100;
        }
        else if (soul >= 20)
        {
            type = 2;
            subsoul = 20;
        }
        else if (soul >= 10)
        {
            type = 3;
            subsoul = 10;
        }

        if (!type)
            return;

        _mditem = GameAction()->onUseMDSoul(this, type);
        if (!_mditem)
            return;

        sendMDSoul(1, _mditem);
    }

    void Player::useMDSoul()
    {
        if (!World::getMayDay())
            return;

        UInt32 soul = GetVar(VAR_MDSOUL);
        if (!soul)
            return;

        UInt8 type = 0;
        UInt32 subsoul = 0;
        if (soul >= 100)
        {
            type = 1;
            subsoul = 100;
        }
        else if (soul >= 20)
        {
            type = 2;
            subsoul = 20;
        }
        else if (soul >= 10)
        {
            type = 3;
            subsoul = 10;
        }

        if (!type)
            return;

        if (!_mditem)
            return;

        if (GetPackage()->IsFull())
        {
            sendMsgCode(0, 1011);
            return;
        }

        GetPackage()->AddItem(_mditem, 1, true);

        soul -= subsoul;
        SetVar(VAR_MDSOUL, soul);
        sendMDSoul(0);
        _mditem = 0;
    }

    void Player::svrSt(UInt8 type)
    {
        Stream st(REP::SVRST);
        st << type;
        if (type == 1)
        {
            st << TimeUtil::Now() << Stream::eos;
        }
        else if (type == 2)
        {
            st << getCreated() << Stream::eos;
        }
        else if (type == 3)
        {
            st << GetVar(VAR_INRF7DAY) << Stream::eos;
        }
        send(st);
    }

    void Player::turnOnRC7Day()
    {
        if (!World::getRC7Day())
            return;

        UInt32 offset = GetVar(VAR_RC7DAYTURNON);
        if (offset >= 7)
            return;
        UInt32 total = GetVar(VAR_RC7DAYRECHARGE);
        if (GameAction()->onTurnOnRC7Day(this, total, offset))
        {
            SetVar(VAR_RC7DAYTURNON, offset+1);

            Stream st(REP::RC7DAY);
            st << static_cast<UInt8>(6) << static_cast<UInt8>(GetVar(VAR_RC7DAYTURNON));
            st << Stream::eos;
            send(st);
        }
    }

    void Player::turnOnRF7Day()
    {
        UInt32 now = TimeUtil::Now();
        UInt32 rf = GetVar(VAR_INRF7DAY);
        if (!rf || now < rf)
            return;

        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 rf_sharp = TimeUtil::SharpDay(0, rf);

        if (now_sharp - rf_sharp > 7 * DAY_SECS)
            return;

        UInt32 offset = GetVar(VAR_RF7DAYTURNON);
        if (offset >= 7)
            return;
        UInt32 total = GetVar(VAR_RF7DAYRECHARGE);
        if (GameAction()->onTurnOnRF7Day(this, total, offset))
        {
            SetVar(VAR_RF7DAYTURNON, offset+1);

            Stream st(REP::RF7DAY);
            st << static_cast<UInt8>(6) << static_cast<UInt8>(GetVar(VAR_RF7DAYTURNON));
            st << Stream::eos;
            send(st);
        }
    }

    void Player::addRC7DayRecharge(UInt32 r)
    {
        if (!World::getRC7Day())
            return;
        AddVar(VAR_RC7DAYRECHARGE, r);

        Stream st(REP::RC7DAY);
        st << static_cast<UInt8>(5) << GetVar(VAR_RC7DAYRECHARGE);
        st << Stream::eos;
        send(st);
    }

    void Player::addRF7DayRecharge(UInt32 r)
    {
        UInt32 now = TimeUtil::Now();
        UInt32 rf = GetVar(VAR_INRF7DAY);
        if (!rf || now < rf)
            return;

        UInt32 rf_sharp = TimeUtil::SharpDay(0, rf);
        UInt32 now_sharp = TimeUtil::SharpDay(0, now);

        if (now_sharp - rf_sharp > 7 * DAY_SECS)
            return;

        AddVar(VAR_RF7DAYRECHARGE, r);

        Stream st(REP::RF7DAY);
        st << static_cast<UInt8>(5) << GetVar(VAR_RF7DAYRECHARGE);
        st << Stream::eos;
        send(st);
    }

    void Player::recvYBBuf(UInt8 type)
    {
        if((this->isBD() && World::getBlueDiamondAct()) || (this->isYD() && World::getYellowDiamondAct()))
        {
            UInt32 ybbuf = GetVar(VAR_YBBUF);
            UInt32 ybuf = (ybbuf >> 16) & 0xFFFF;
            UInt32 bbuf = ybbuf & 0xFFFF;

            // type = 0 黄钻 1 蓝钻

            bool r = false;
            UInt32 now = TimeUtil::Now();
            if (!ybuf && type == 0)
            {
                setBuffData(PLAYER_BUFF_YBUF, now + 60 * 60);
                ybuf = 1;
                r = true;
            }

            if (!bbuf && type == 1)
            {
                setBuffData(PLAYER_BUFF_BBUF, now + 60 * 60);
                bbuf = 1;
                r = true;
            }

            if (r)
            {
                ybbuf = (ybuf << 16) | bbuf;
                SetVar(VAR_YBBUF, ybbuf);
            }

            sendYBBufInfo(ybbuf);
        }
    }

    void Player::sendYBBufInfo(UInt32 ybbuf)
    {
        Stream st(REP::YBBUF);
        st << static_cast<UInt8>((ybbuf >> 16) & 0xFFFF) << static_cast<UInt8>(ybbuf & 0xFFFF) << Stream::eos;
        send(st);
    }

    bool Player::isCopyPassed(UInt8 copyid)
    {
        TeamCopyPlayerInfo* tcpInfo = getTeamCopyPlayerInfo();
        if (!tcpInfo)
            return false;
        return tcpInfo->getPass(copyid);
    }

    bool Player::hasRealItemAward(UInt32 id)
    {
        return realItemAwardMgr.hasAward(id);
    }

    void Player::getRealItemAward(UInt32 id)
    {
        realItemAwardMgr.getAward(this, id);
    }

    void Player::IDIPAddItem(UInt16 itemId, UInt16 num, bool bind)
    {
        SYSMSG(title, 4004);
        SYSMSG(content, 4005);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFD0000/*free*/);
        if(mail)
        {
            MailPackage::MailItem mitem[1] = {{itemId,num}};
            mailPackageManager.push(mail->id, mitem, 1, bind);
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
    }

    void Player::sendFourCopAct()
    {
        UInt16 lengxueCnt;
        UInt16 wuqingCnt;
        UInt16 tieshouCnt;
        UInt16 zhuimingCnt;

        lengxueCnt = static_cast<UInt16>(GetVar(VAR_LX_CNT));
        wuqingCnt = static_cast<UInt16>(GetVar(VAR_WQ_CNT));
        tieshouCnt = static_cast<UInt16>(GetVar(VAR_TS_CNT));
        zhuimingCnt = static_cast<UInt16>(GetVar(VAR_ZM_CNT));

        Stream st(REP::FOURCOP);
        st << static_cast<UInt8>(0) << lengxueCnt << wuqingCnt << tieshouCnt << zhuimingCnt << Stream::eos;
        send(st);
    }

    bool Player::inArenaCommitCD()
    {
        UInt32 now = TimeUtil::Now();
        if(now < m_arenaCommitCD)
            return true;

        m_arenaCommitCD = now + 60;
        return false;
    }

    void Player::appendLineup2( Stream& st)
    {
        st << getFormation();
        size_t offset = st.size();
        UInt8 c = 0;
        st << c;
        for(UInt8 i = 0; i < 5; ++ i)
        {
            Lineup& pdata = getLineup(i);
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

                st << fgt->getSoulExtraAura();
                st << fgt->getBattlePortrait();
                fgt->appendElixirAttr2(st);
            }
        }
        st.data<UInt8>(offset) = c;
    }

#ifdef _FB
    void Player::sendLevelAward()
    {
        if (GetLev() < 60)
            return;

        UInt32 t = 0;
        if (GetLev() < 65)
            t = 500;
        else if (GetLev() >= 65 && GetLev() < 70)
            t = 1000;
        else
            t = 2000;
        SYSMSG(title, 4008);
        SYSMSGV(content, 4009, GetLev(), t);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFD0000/*free*/);
        if(mail)
        {
            MailPackage::MailItem mitem[1] = {{0xA000,t},};
            mailPackageManager.push(mail->id, mitem, 1);
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
    }
#endif

#ifdef _FB
    void Player::equipForge(UInt32 fighterId, UInt32 itemId, UInt32 num)
    {
        UInt32 fi = fighterId << 16 | itemId;
        UInt32& on = _forges[fi];
        if (on < num)
        {
            on = num;
            GameAction()->onEquipForge(this, itemId, num);
        }
    }
#endif

} // namespace GObject


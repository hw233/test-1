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
#include "ClanCopy.h"
#include "Mail.h"
#include "Boss.h"
#include "Athletics.h"
#include "CountryBattle.h"
#include "NewCountryBattle.h"
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
#ifndef _FB
#ifndef _VT
#ifndef _WIN32
#include "DCLogger.h"
#include "OpenAPIWorker.h"
#endif
#endif
#endif
#include "ClanRankBattle.h"
#include "TeamCopy.h"
#include "HoneyFall.h"
#include "GData/ClanTechTable.h"
#include "GData/ClanLvlTable.h"
#include "GData/ClanSkillTable.h"
#include "GData/ClanStatueTable.h"
#include "GData/ExpTable.h"
#include "GData/FairyPetTable.h"
#include "Common/StringTokenizer.h"
#include "TownDeamon.h"
#include "ArenaBattle.h"
#ifdef _ARENA_SERVER
#include "GameServer.h"
#endif
#include "GData/Store.h"
#include "RealItemAward.h"
#include "GObject/Tianjie.h"
#include <cmath>
#include "QixiTmpl.h"
#include "MsgHandler/Memcached.h"
#include "RechargeTmpl.h"
#include "GData/ExpTable.h"
#include "Version.h"


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

#define ATHL_BUFF_SECS        (10*60)
#define ATHL_BUFF_SECS_MAX    (30*60)

#define QIXI_XIQUE 9122

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
        // 限时vip特权
        if(maxCount < 16 && inVipPrivilegeTime())
            maxCount = 16;
		return maxCount;
	}

	float EventAutoBattle::calcExpEach(UInt32 now)
	{
#if 0
        return 8.0f * _npcGroup->getExp();
#else
        UInt8 plvl = m_Player->GetLev();
        UInt64 exp = (plvl - 10) * ((plvl > 99 ? 99 : plvl) / 10) * 5 + 25;
        return static_cast<float>(exp);
#endif
	}

	void EventAutoBattle::Process(UInt32)
	{
		UInt32 now = TimeUtil::Now();
		float exp = calcExpEach(now);
        float factor = 1.0f;
        UInt32 tmp;
        UInt32 curHookIndex = m_Player->GetVar(VAR_EXP_HOOK_INDEX);
        if(curHookIndex == ENUM_TRAINP1)
        {
            tmp = m_Player->GetVar(VAR_TRAINP1);
            if(tmp > 0)
            {
                factor = 1.2f;
                if(tmp > 60)
                    tmp -= 60;
                else
                    tmp = 0;
                m_Player->SetVar(VAR_TRAINP1, tmp);
            }
        }
        else if(curHookIndex == ENUM_TRAINP2)
        {
            tmp = m_Player->GetVar(VAR_TRAINP2);
            if(tmp > 0)
            {
                factor = 1.5f;
                if(tmp > 60)
                    tmp -= 60;
                else
                    tmp = 0;
                m_Player->SetVar(VAR_TRAINP2, tmp);
            }
        }
        else if(curHookIndex == ENUM_TRAINP3)
        {
            tmp = m_Player->GetVar(VAR_TRAINP3);
            if(tmp > 0)
            {
                factor = 1.8f;
                if(tmp > 60)
                    tmp -= 60;
                else
                    tmp = 0;
                m_Player->SetVar(VAR_TRAINP3, tmp);
            }
        }

        UInt32 extraExp = 0;
        // 限时vip特权
        if(m_Player->inVipPrivilegeTime())
        {
            factor += 0.2f;
            extraExp = static_cast<UInt32>(exp * 0.2f);
        }
#if 0
		_npcGroup->monsterKilled(m_Player);
#endif
        if (cfg.rpServer && m_Player->GetLev() < 70)
            exp *= 2;
		if(m_Player->isOnline())
			m_Player->AddExp(static_cast<UInt32>(exp * factor), 0, extraExp);
		else
			m_Player->pendExp(static_cast<UInt32>(exp * factor));
#if 0
		_npcGroup->getLoots(m_Player);
#else
        if(uRand(10000) < 74)
            m_Player->GetPackage()->Add(9359, 1, true, false);
#endif
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
        UInt8 iccnt = m_Player->getMaxIcCount(vipLevel) - m_Player->getIcCount();
        if (m_Player->getMaxIcCount(vipLevel) < m_Player->getIcCount())
            iccnt = m_Player->getMaxIcCount(vipLevel);
        UInt8 curType = static_cast<UInt8>(m_Player->GetVar(VAR_EXP_HOOK_INDEX));
		if(cnt > 0)
		{
#if 0
			if(_npcGroup == NULL)
				return;
#endif
			UInt32 t = TimeUtil::Now();
			if(t > _finalEnd) t = 0; else t = _finalEnd - t;
			st << /*_npcGroup->getId()*/static_cast<UInt32>(0) << static_cast<UInt8>(1) << cnt << t << iccnt << curType << Stream::eos;
		}
		else
		{
			st << static_cast<UInt32>(0) << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt32>(0) << iccnt << curType << Stream::eos;
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
		ecs.ng = NULL;//_npcGroup;
        if (cfg.rpServer && m_Player->GetLev() < 70)
            ecs.exp *= 2;
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
				DB3().PushUpdateData("REPLACE INTO `auto_battle`(`playerId`, `npcId`, `count`, `interval`) VALUES(%"I64_FMT"u, %u, %u, %u)", m_Player->getId(), /*_npcGroup->getId()*/0, count, m_Timer.GetInterval());
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
            msg.id = SthPSpeed;
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

    bool EventAutoJobHunter::Equal(UInt32 id, size_t playerid) const
    {
		return 	id == GetID() && playerid == m_Player->getId();
    }

    void EventAutoJobHunter::Process(UInt32 leftCount)
    {
		GameMsgHdr hdr(0x2A1, m_Player->getThreadId(), m_Player, sizeof(id));
		GLOBAL().PushMsg(hdr, &id);
        if (!leftCount)
			PopTimerEvent(m_Player, EVENT_JOBHUNTER, m_Player->getId());
    }

    bool EventAutoJobHunter::Accelerate(UInt32 times)
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

    bool EventAutoRefreshOpenKey::Equal(UInt32 id, size_t playerid) const
    {
        return playerid == m_Player->getId();
    }

    void EventAutoRefreshOpenKey::Process(UInt32 leftCount)
    {
#ifndef _WIN32
#ifndef _FB
#ifndef _VT
#ifdef  OPEN_API_ON
        OPENAPI().Push(m_Player->getId(), 0, m_Player->getOpenId(), m_Player->getOpenKey(), m_Player->getSource().c_str(), m_Player->getClientIp());
#endif
#endif
#endif
#endif // _WIN32
    }

    bool EventAutoRefreshOpenKey::Accelerate(UInt32 times)
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
        _justice_roar(0), _spirit_factor(1.0f), _diamond_privilege(false), _qqvip_privilege(false), _athlRivalBuf(0), _new_rank(false), _worldBossHp(0), m_autoCopyComplete(0), hispot(0xFF), hitype(0),
#ifndef _WIN32
		m_ulog(NULL),
#endif
		m_isOffical(false), m_isXY(false), m_XinYue(0), m_sysDailog(false), m_hasTripod(false), _maxLingbaoBattlePoint(0), _jobHunter(NULL), _dreamer(NULL), _onBattlePet(NULL)
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
        m_StrengthenMgr = new StrengthenMgr(this);
        m_HeroMemo = new HeroMemo(this);
        m_ShuoShuo = new ShuoShuo(this);
        m_CFriend = new CFriend(this);
        m_relation = new NewRelation(this);
        m_pVars = new VarSystem(id);
        memset(&m_ctp, 0, sizeof(m_ctp));
        m_teamData = NULL;
        m_tcpInfo = new TeamCopyPlayerInfo(this);
        m_hf = new HoneyFall(this);
        m_dpData = new DeamonPlayerData();
        m_csFlag = 0;
        _mditem = 0;
        _qixiBinding = false;

        memset (m_domain, 0, sizeof(m_domain));
        memset (m_openid, 0, sizeof(m_openid));
        memset (m_openkey, 0, sizeof(m_openkey));
        //memset (m_clientIp, 0, sizeof(m_clientIp));
        strncpy (m_clientIp, "0.0.0.0", 10);
        _isForbidSale = false;

        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "%"I64_FMT"u", _id);
#ifndef _WIN32
        m_ulog = _analyzer.GetInstance(buf);
        m_ulog->SetUserIP("0.0.0.0");
#endif
        /*if(cfg.merged)
            _new_rank = true;
        else
            _new_rank = false;*/
        memset(cf_posPut, 0, sizeof(cf_posPut));
        memset(cf_itemId, 0, sizeof(cf_itemId));
        memset(cf_ratio, 0 ,sizeof(cf_ratio));
        _hiattrFlag = false;

        _inQQGroup = false;
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
        SAFE_DELETE(m_relation);
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

        //calcNewYearQzoneContinueDay(curtime);
        continuousLogin(curtime);
        continuousLoginRF(curtime);
        sendYearRPInfo();

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
        /*
        if (World::getFoolsDay())
            GameAction()->onFoolsDay(this);
        */
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
        /*
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
        */
        if (World::_blueactiveday)
            onBlueactiveday();

        if (World::getQgameGiftAct())
        {
            getQgameGiftAward();
        }

        /**
         * 1：蓝钻
         * 2：黄钻
         * 4：会员
         **/
        UInt32 curStatus = GetVar(VAR_BDIAMOND_CMD);
        UInt32 newStatus;
        UInt16 titleId;
        if(isBD() && (curStatus&1) == 0)
        {
            newStatus = 1;
            titleId = 283;
        }
        else if(isYD() && (curStatus&2) == 0)
        {
            newStatus = 2;
            titleId = 285;
        }
        else if(isQQVIP() && (curStatus&4) == 0)
        {
            newStatus = 4;
            titleId = 287;
        }
        else
        {
            newStatus = 0;
            titleId = 0;
        }
        if(newStatus > 0)
        {
            SetVar(VAR_BDIAMOND_CMD, curStatus | newStatus);

			SYSMSG(title, titleId);
			SYSMSG(content, titleId + 1);

			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                UInt8 level = GetLev();
                UInt16 itemId;
                if(level < 60)
                    itemId = 9012;
                else if(level < 70)
                    itemId = 9013;
                else if(level < 80)
                    itemId = 9014;
                else if(level < 90)
                    itemId = 9015;
                else if(level < 100)
                    itemId = 9016;
                else
                    itemId = 9035;
                MailPackage::MailItem mitem[1] = {{itemId, 1}};
                MailItemsInfo itemsInfo(mitem, BlueDiamondCmd, 1);
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

        sendLevelPack(GetLev());
        offlineExp(curtime);

        //QQGame登录奖励
        sendQQGameGift1218();
        sendFeastLoginAct();
        //蛇年春节套装
        sendSnakeSpringEquipMail();

        char buf[64] = {0};
        snprintf(buf, sizeof(buf), "%"I64_FMT"u", _id);
#ifndef _WIN32
        if (!m_ulog)
            m_ulog = _analyzer.GetInstance(buf);
        if (m_ulog)
        {
            TcpConnection conn = NETWORK()->GetConn(_session);
            if (conn)
            {
                Network::GameClient * cl = static_cast<Network::GameClient *>(conn.get());
                struct in_addr inaddr = inet_makeaddr(cl->GetClientIP(), 0);
                //m_ulog->SetUserIP(inet_ntoa(inaddr));
                m_ulog->SetUserIP(m_clientIp);
            }
        }
#endif
        if (!m_via.empty())
        {
            StringTokenizer via(m_via, "_");
            if (via.count() > 1)
                udpLog(via[0].c_str(), via[1].c_str(), "", "", "", "0", "login");
            else
                udpLog(m_via.c_str(), "", "", "", "", "0", "login");
        }
        else
            udpLog("", "", "", "", "", "0", "login");

        sendTowerLoginAct();

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
        /*
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
        */
#ifndef _WIN32
#ifndef _FB
#ifndef _VT
        dclogger.login(this);
        dclogger.login_sec(this);

        EventAutoRefreshOpenKey* event = new(std::nothrow) EventAutoRefreshOpenKey(this, 60 * 110, 24);
        if (event)
            PushTimerEvent(event);
            

#ifdef  OPEN_API_ON
        OPENAPI().Push(getId(), 0, getOpenId(), getOpenKey(), getSource().c_str(), getClientIp());
#endif
        
#endif
#endif
#endif // _WIN32
#ifdef DREAMER_DEBUG
        getDreamer();
#endif
	}

#define WEBDOWNLOAD 255

    void Player::udpLog(UInt8 platform, const char* str1, const char* str2, const char* str3, const char* str4,
                const char* str5, const char* str6, const char* type, UInt32 count)
    {
#ifndef _WIN32
        if (m_ulog && cfg.udplog)
        {
            char buf[1024] = {0};
            char* pbuf = &buf[0];
            if (cfg.isTestPlatform)
                pbuf += snprintf(pbuf, sizeof(buf), "%u_%u_%"I64_FMT"u|%s|||||%u||%u|%u|%u|%u|%u|%u|%u||%u||%u|1|",
                    cfg.serverNum, cfg.tcpPort, getId(), getOpenId(), GetLev(), _playerData.gold, _playerData.coupon, _playerData.tael, getVipLevel(), _clan? _clan->getId() : 0, getXinYue(), _playerData.qqvipl, cfg.serverNum, platform);
            else
                pbuf += snprintf(pbuf, sizeof(buf), "%u_%u_%"I64_FMT"u|%s|||||%u||%u|%u|%u|%u|%u|%u|%u||%u||%u|",
                    cfg.serverNum, cfg.tcpPort, getId(), getOpenId(), GetLev(), _playerData.gold, _playerData.coupon, _playerData.tael, getVipLevel(), _clan? _clan->getId() : 0, getXinYue(), _playerData.qqvipl, cfg.serverNum, platform);

            m_ulog->SetUserMsg(buf);
            if (platform != WEBDOWNLOAD)
                m_ulog->LogMsg(str1, str2, str3, str4, str5, str6, type, count, 0);
            // XXX: 不再分平台发送
            //if (platform)
            //    m_ulog->LogMsg(str1, str2, str3, str4, str5, str6, type, count, platform);

            TRACE_LOG("%s - (%s,%s,%s,%s,%s,%s,%s,%u) - %u", buf, str1, str2, str3, str4, str5, str6, type, count, platform);
        }
#endif
	}

    void Player::udpLog(const char* str1, const char* str2, const char* str3, const char* str4,
                const char* str5, const char* str6, const char* type, UInt32 count)
    {
#ifndef _WIN32
        if (m_ulog)
        {
            UInt8 platform = atoi(getDomain());
            if (platform == OFFICAL && strstr(m_via.c_str(), "webdownload"))
                platform = WEBDOWNLOAD;

            if (isXY())
                platform = PF_XY;

            udpLog(platform, str1, str2, str3, str4, str5, str6, type, count);
        }
#endif // _WIN32
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

    void Player::luaUdpLog(const char* str1, const char* str2, const char* type)
    {
        udpLog(str1, str2, "", "", "", "", type);
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
            //World::_moneyIn[6] = {{0,},};
            memset(&World::_moneyIn, 0, sizeof(World::_moneyIn));

            DB8().PushUpdateData("INSERT INTO `money` (`time`, `type`, `gold`, `coupon`, `tael`, `achievement`, `prestige`) VALUES (%d,1,0,0,0,0,0)", today);
            DB8().PushUpdateData("INSERT INTO `money` (`time`, `type`, `gold`, `coupon`, `tael`, `achievement`, `prestige`) VALUES (%d,2,0,0,0,0,0)", today);
        }
        DB8().PushUpdateData("UPDATE `money` SET `gold` = `gold` + %d, `coupon` = `coupon` + %d, `tael` = `tael` + %d, `achievement` = `achievement` + %d, `prestige` = `prestige` + %d WHERE `time` = %d AND `type` = %d", gold, coupon, tael, achievement, prestige, today, type);

        World::_moneyLogged = now;
        // TODO:
        World::_moneyIn[6][type-1].gold += gold;
    }

    void Player::discountLog(UInt8 discountType)
    {
        char action[16] = "";
        snprintf (action, 16, "F1703_%d", discountType);
        udpLog("discount", action, "", "", "", "", "act");
    }

    void Player::tradeUdpLog(UInt32 id, UInt32 val /* = 0 */, UInt32 num /* = 1 */, bool priceLog /* = false */)
    {
        // 交易相关日志
        char action[32] = "";
        if (val)
        {
            if (priceLog)
                snprintf (action, 32, "F_%d_%d_2", id, val);
            else
                snprintf (action, 32, "F_%d_%d", id, val);
        }
        else
        {
            snprintf (action, 32, "F_%d", id);
        }
        udpLog("trade", action, "", "", "", "", "act", num);
    }

    void Player::skillStrengthenLog(UInt8 type, UInt32 val)
    {
        char action[16] = "";
        UInt32 num = 1;
        switch (type)
        {
            case 1:
                snprintf (action, 16, "%s", "F_1075");
                if (val)
                    strcat (action, "_1");
                else
                    strcat (action, "_2");
                break;
            case 2:
                snprintf (action, 8, "%s", "F_1076");
                num = val;
                break;
            default:
                snprintf (action, 8, "%s", "Error");
                break;
        }
        udpLog("skillStrengthen", action, "", "", "", "", "act", num);
    }

    void Player::townDeamonUdpLog(UInt16 level)
    {
        char action[16] = "";
        snprintf (action, 16, "F_1074_%d", level);
        udpLog("townDeamon", action, "", "", "", "", "act");
    }

    void Player::dungeonUdpLog(UInt8 levelReq, UInt8 type)
    {
        // 决战之地日志(又叫通天塔又叫地牢)
        char action[16] = "";
        switch(levelReq)
        {
            case 30:
                snprintf (action, 16, "F1068_%d", type);
                break;
            case 45:
                snprintf (action, 16, "F1069_%d", type);
                break;
            case 60:
                snprintf (action, 16, "F1070_%d", type);
                break;
            case 75:
                snprintf (action, 16, "F1071_%d", type);
                break;
            case 90:
                snprintf (action, 16, "F1072_%d", type);
                break;
            default:
                snprintf (action, 16, "ERROR");
                break;
        }
        udpLog("dungeon", action, "", "", "", "", "act");
    }

    void Player::frontMapUdpLog(UInt8 id, UInt8 type)
    {
        // 阵图日志
        char action[16] = "";

        snprintf (action, 16, "F%d_%d", id + 1055, type);

        udpLog("frontMap", action, "", "", "", "", "act");
    }

    void Player::copyUdpLog(UInt8 id, UInt8 type)
    {
        // 副本日志
        char action[16] = "";

        snprintf (action, 16, "F%d_%d", id + 1049, type);

        udpLog("copy", action, "", "", "", "", "act");
    }

    void Player::athleticsUdpLog(UInt32 id, UInt8 type /* = 0 */)
    {
        // 斗剑功能相关日志
        char action[16] = "";
        if (type)
        {
            snprintf (action, 16, "F%d_%d", id, type);
        }
        else
        {
            snprintf (action, 16, "F%d", id);
        }
        udpLog("athletics", action, "", "", "", "", "act");
    }

    void Player::activityUdpLog(UInt32 id, UInt8 type /* = 0 */)
    {
        // 活跃度功能相关日志
        // FIXME: 只能记录兑换积分小于255
        char action[16] = "";
        if (type)
        {
            snprintf (action, 16, "F%d_%d", id, type);
        }
        else
        {
            snprintf (action, 16, "F%d", id);
        }
        udpLog("activity", action, "", "", "", "", "act");
    }

    void Player::practiceUdpLog()
    {
        // 修为相关日志（暂时只有加速）
        char action[16] = "";
        snprintf (action, 16, "F%d", _vipLevel + 1003);
        udpLog("practice", action, "", "", "", "", "act");
    }

    void Player::arenaUdpLog(UInt32 id, UInt8 type /* = 0 */)
    {
        // 跨服战操作相关日志
        char action[16] = "";
        if (type)
        {
            snprintf (action, 16, "F%d_%d", id, type);
        }
        else
        {
            snprintf (action, 16, "F%d", id);
        }
        udpLog("arena", action, "", "", "", "", "act");
    }

    void Player::luckyDrawUdpLog(UInt32 id, UInt8 type, UInt32 num /* = 1 */)
    {
        // 秘境寻宝相关日志
        char action[16] = "";
        snprintf (action, 16, "F%d_%d", id + 1018, type);
        udpLog("luckyDraw", action, "", "", "", "", "act", num);
    }

    void Player::qixiUdpLog(UInt32 id)
    {
        // 七夕活动相关日志
        char action[16] = "";
        snprintf (action, 16, "F_%d", id);
        udpLog("qixi", action, "", "", "", "", "act");
    }

    void Player::clanUdpLog(UInt32 id)
    {
        // 帮派相关日志
        char action[16] = "";
        snprintf (action, 16, "F_%d", id);
        udpLog("clan", action, "", "", "", "", "act");
    }

    void Player::countryBattleUdpLog(UInt32 id, UInt8 country, std::string str)
    {
        // 国战相关日志
        char action[32] = "";
        if(str.empty())
            snprintf (action, 32, "F_%d_%d", id, country);
        else
            snprintf (action, 32, "F_%d_%d_%s", id, country, str.c_str());
        udpLog("countryBattle", action, "", "", "", "", "act");
    }

    void Player::heroIslandUdpLog(UInt32 id, UInt8 type)
    {
        // 英雄岛相关日志
        char action[16] = "";
        snprintf (action, 16, "F_%d_%d", id, type);
        udpLog("heroIsland", action, "", "", "", "", "act");
    }

    void Player::secondSoulUdpLog(UInt32 id, UInt32 val /* = 0 */, UInt32 num /* = 1 */)
    {
        // 元神相关日志
        char action[16] = "";
        if (val)
        {
            snprintf(action, 16, "F_%d_%d", id, val);
        }
        else
        {
            snprintf (action, 16, "F_%d", id);
        }
        if (!num)
            return;
        udpLog("secondSoul", action, "", "", "", "", "act", num);
    }

    void Player::wBossUdpLog(UInt32 id)
    {
        // 世界boss相关日志
        char action[16] = "";
        snprintf (action, 16, "F_%d", id);
        udpLog("worldBoss", action, "", "", "", "", "act");
    }

    void Player::clanCopyUdpLog(UInt32 id, UInt32 val /* = 0 */, UInt32 num /* = 1 */)
    {
        // 帮派副本相关日志
        char action[16] = "";
        if (val)
        {
            snprintf (action, 16, "F_%d_%d", id, val);
        }
        else
        {
            snprintf (action, 16, "F_%d", id);
        }
        udpLog("clanCopy", action, "", "", "", "", "act", num);
    }

    void Player::tripodUdpLog(UInt32 id, UInt32 val /* = 0 */, UInt32 num /* = 1 */)
    {
        // 九疑鼎相关日志
        char action[16] = "";
        if (val)
        {
            snprintf (action, 16, "F_%d_%d", id, val);
        }
        else
        {
            snprintf (action, 16, "F_%d", id);
        }
        udpLog("tripod", action, "", "", "", "", "act", num);
    }

    void Player::storeUdpLog(UInt32 id, UInt32 type, UInt32 itemId, UInt32 num /* = 1 */)
    {
        // 商城购买相关日志（现在只有荣誉和声望）
        char action[32] = "";
        snprintf (action, 32, "F_%d_%d_%d", id, type, itemId);
        udpLog("store", action, "", "", "", "", "act", num);
    }

    void Player::newRC7DayUdpLog(UInt32 id, UInt32 type /* = 0 */, UInt32 num /* = 1 */)
    {
        // 新版注册七日活动日志
        char action[32] = "";
        if (type)
            snprintf (action, 32, "F_%d_%d", id, type);
        else
            snprintf (action, 32, "F_%d", id);
        udpLog("register", action, "", "", "", "", "act", num);
    }

    void Player::transformUdpLog(UInt32 id, UInt32 type, UInt32 money1, UInt32 money2, UInt32 money3, UInt32 money4, UInt8 val1)
    {
        // 属性转移udp日志
        char action[64] = "";
        if (type & 0x01)
        {
            snprintf (action, 64, "F_%d_%d", id, 1);
            udpLog("transform", action, "", "", "", "", "act", money1);
        }
        if (type & 0x02)
        {
            snprintf (action, 64, "F_%d_%d_%d", id, 2, val1);
            udpLog("transform", action, "", "", "", "", "act", money2);
        }
        if (type & 0x08)
        {
            snprintf (action, 64, "F_%d_%d", id, 8);
            udpLog("transform", action, "", "", "", "", "act", money3);
        }
        if (type & 0x10)
        {
            snprintf (action, 64, "F_%d_%d", id, 10);
            udpLog("transform", action, "", "", "", "", "act", money4);
        }
    }

    void Player::dreamerUdpLog(UInt32 id, UInt32 type, UInt32 num /* = 1 */)
    {
        // 水晶梦境udp日志
        char action[32] = "";
        snprintf (action, 32, "F_%d_%d", id, type);
        udpLog("dream", action, "", "", "", "", "act", num);
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
        /** deleted by qiwy 2012-12-07 **/
#if 0
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
#endif
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
        //UInt32 now  = TimeUtil::Now();
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
        /*
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
        */

        int addr = inet_addr(m_clientIp);
		DBLOG1().PushUpdateData("update login_states set logout_time=%u where server_id=%u and player_id=%"I64_FMT"u and login_time=%u", curtime, addr?addr:cfg.serverLogId, _id, _playerData.lastOnline);
		writeOnlineRewardToDB();

		removeStatus(SGPunish);
        LogoutSaveOnlineTimeToday();

        char online[32] = {0,};
        snprintf(online, sizeof(online), "%u", curtime - _playerData.lastOnline);
        udpLog("", "", "", "", "", online, "login");

        //UInt8 platform = atoi(getDomain());
        if (cfg.GMCheck )
        {
            struct CrackValue
            {
                char ip[1024];
                int v;
            } crackValue;
            snprintf(crackValue.ip, sizeof(crackValue.ip), "%s", getClientIp());
            crackValue.v = 0;

            LoginMsgHdr hdr1(0x301, WORKER_THREAD_LOGIN, 0, this->GetSessionID(), sizeof(crackValue));
            GLOBAL().PushMsg(hdr1, &crackValue);
        }
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
        setQQGameOnlineTotalTime();
        int addr = inet_addr(m_clientIp);
		DBLOG1().PushUpdateData("update login_states set logout_time=%u where server_id=%u and player_id=%"I64_FMT"u and login_time=%u", curtime, addr?addr:cfg.serverLogId, _id, _playerData.lastOnline);
		DB1().PushUpdateData("UPDATE `player` SET `lastOnline` = %u, `nextReward` = '%u|%u|%u|%u' WHERE `id` = %"I64_FMT"u", curtime, _playerData.rewardStep, _playerData.nextRewardItem, _playerData.nextRewardCount, _playerData.nextRewardTime, _id);
        if(_isOnline && !hasFlag(Training))
        {
            //if(cfg.GMCheck)
             {
                 if(worldBoss.needAutoBattle(_playerData.location))
                    autoBattle(0, 0);
             }
        }
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
        /*
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
        */
        if (World::_blueactiveday)
            PopTimerEvent(this, EVENT_TIMETICK, getId());

        LogoutSaveOnlineTimeToday();
#ifndef _WIN32
#ifdef _FB
#else
        dclogger.logout(this);
        PopTimerEvent(this, EVENT_REFRESHOPENKEY, getId());
#endif
#endif // _WIN32
        heroIsland.playerOffline(this);
		removeStatus(SGPunish);
        char online[32] = {0,};
        snprintf(online, sizeof(online), "%u", TimeUtil::Now() - _playerData.lastOnline);
        udpLog("", "", "", "", "", online, "login");

        //UInt8 platform = atoi(getDomain());
        if (cfg.GMCheck )
        {
            struct CrackValue
            {
                char ip[1024];
                int v;
            } crackValue;
            snprintf(crackValue.ip, sizeof(crackValue.ip), "%s", getClientIp());
            crackValue.v = 0;

            LoginMsgHdr hdr1(0x301, WORKER_THREAD_LOGIN, 0, this->GetSessionID(), sizeof(crackValue));
            GLOBAL().PushMsg(hdr1, &crackValue);
        }
        //愚公移山活动
        setLogoutInFoolsDay();
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
        sendDeamonAwardsInfo();
        checkLastExJobAward();

		if(update)
		{
			DB1().PushUpdateDataL("UPDATE `player` SET `lastExp` = 0, `lastResource` = 0 WHERE `id` = %"I64_FMT"u", _id);
		}
	}

    void Player::addLastTjScore()
    {
        if (!GObject::Tianjie::instance().isTjOpened())
        {
            _playerData.lastTjEventScore = 0;
            _playerData.lastTjTotalScore = 0;
            return;
        }

        if (_playerData.lastTjEventScore > 0)
        {
            SYSMSG_SENDV(167, this, _playerData.lastTjEventScore);
            SYSMSG_SENDV(169, this, _playerData.lastTjEventScore);
        }
        if (_playerData.lastTjTotalScore > 0)
        {
            SYSMSG_SENDV(168, this, _playerData.lastTjTotalScore);
            SYSMSG_SENDV(170, this, _playerData.lastTjTotalScore);
        }
        _playerData.lastTjEventScore = 0;
        _playerData.lastTjTotalScore = 0;
    }

    UInt32 bufferId2VarId(UInt8 id)
    {
        if(id == PLAYER_BUFF_TRAINP1)
            return VAR_TRAINP1;
        else if(id == PLAYER_BUFF_TRAINP2/* || id == PLAYER_BUFF_TRAINP4 || id == PLAYER_BUFF_ADVANCED_HOOK*/)
            return VAR_TRAINP2;
        else if(id == PLAYER_BUFF_TRAINP3)
            return VAR_TRAINP3;
        else
            return 0;
    }

	void Player::sendExpHook(UInt8 id, UInt32 data)
	{
        if(_isOnline)
        {
            Stream st(REP::USER_INFO_CHANGE);
            st << id << data << Stream::eos;
            send(st);
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
        bool hasMo = false;
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
                if(hasMo && lup.fighter->getClass() == e_cls_mo)
                {
                    lup.fighter = NULL;
                    lup.fid = 0;
                    lup.pos = 0;
                    continue;
                }
                else if(lup.fighter && lup.fighter->getClass() == e_cls_mo)
                    hasMo = true;
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
        SetVar(VAR_HAS_MO_BATTLER, hasMo? 1: 0);
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
		return (!_fighters.empty()) ? _fighters.begin()->second->getLevel() : 1;
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
        static UInt16 cittas[4][9] =
        {
            {301, 0, 0, 0, 0, 0, 0, 0, 0},
            {401, 0, 0, 0, 0, 0, 0, 0, 0},
            {701, 0, 0, 0, 0, 0, 0, 0, 0},
            {13409, 13709, 13509, 14109, 14209, 14309, 0, 0, 0}
        };
        if(fgt->getClass() >= e_cls_max)
            return;
        UInt8 clsIdx = fgt->getClass() - 1;
        for(int i = 0; i < 9; ++ i)
        {
            UInt16 citta = cittas[clsIdx][i];
            if(citta == 0)
                continue;
            if (fgt->hasCitta(citta) < 0) {
                if (fgt->addNewCitta(citta, writedb, true)) {
                    /*
                    if (fgt->upCitta(citta, 0, writedb)) {
                    }
                    */
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
			DB2().PushUpdateData("INSERT INTO `fighter` (`id`, `playerId`, `potential`, `capacity`, `level`, `experience`)\
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
                         minCol = std::min(minCol, it->second->getColor());
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

        //if (fgt->getClass() == e_cls_mo)    // XXX: 更新前招募未完全打通穴道
        if (fgt->getClass() == e_cls_mo && !load)
        {
            // 70级，关元穴穴道，60级白虎
            fgt->addExp(GData::expTable.getLevelMin(70));
            fgt->openSecondSoul(13);
            fgt->setSoulLevel(60);
            fgt->setToAcupoints(11, true);
        }
    }

    bool Player::addFighterFromItem(UInt32 itemid, UInt32 price)
    {
        if(isFighterFull())
        {
            sendMsgCode(0, 1200);
            return false;
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

    bool Player::fighterFromItem(UInt32 fgtid)
    {
        if (!fgtid)
            return false;

        if(isFighterFull())
        {
            sendMsgCode(0, 1200);
            return false;
        }

        if (hasFighter(fgtid))
        {
            sendMsgCode(1, 1017);
            return false;
        }

        Fighter * fgt = globalFighters[fgtid];
        if(fgt == NULL)
            return false;
        Fighter* fgt2 = fgt->clone(this);
        addFighter(fgt2, true);
        notifyAddFighter(fgt2);
        autoLineup(fgt2);
        return true;
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
				m_Package->EquipTo(0, fgt, t+0x0a, equip, true);
            m_Package->EquipTo(0, fgt, 0x1f, equip, true);
            for(UInt8 t = 0; t < 3; ++ t)
				m_Package->EquipTo(0, fgt, t+0x60, equip, true);

			_fighters.erase(it);
			DB2().PushUpdateData("DELETE FROM `fighter` WHERE `id` = %u AND `playerId` = %"I64_FMT"u", id, getId());
			DB2().PushUpdateData("DELETE FROM `second_soul` WHERE `fighterId` = %u AND `playerId` = %"I64_FMT"u", id, getId());

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
    
    UInt32 Player::getBattleMaxHp()
    {
        UInt32 Hp = 0;
        for(int j = 0; j < 5; ++ j)
        {
            Fighter* fighter = _playerData.lineup[j].fighter;
            if(fighter)
                Hp += fighter->getMaxHP();
        }
        return Hp;
    }

    UInt32 Player::getBattleCurrentHp()
    {
        UInt32 Hp = 0;
        for(int j = 0; j < 5; ++ j)
        {
            Fighter* fighter = _playerData.lineup[j].fighter;
            if(fighter)
                Hp += fighter->getCurrentHP() > 0 ? fighter->getCurrentHP() : fighter->getMaxHP();
        }
        return Hp;
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
		DB1().PushUpdateData("UPDATE `player` SET `formation` = %u WHERE id = %"I64_FMT"u", f, _id);

        return true;
	}

	void Player::makePlayerInfo( Stream& st )
	{
		UInt32 curtime = TimeUtil::Now();
		UInt8 c = 0;
		UInt8 buffid[PLAYER_BUFF_COUNT];
		UInt32 buffleft[PLAYER_BUFF_COUNT];
        if (cfg.rpServer && GetLev() < 70)
            setBuffData(PLAYER_BUFF_EXPDOUBLE, curtime+99*3600, false);
 
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
                if(relateExpHook(i))
                {
                    UInt32 varId = bufferId2VarId(i);
                    if(varId == 0)
                        continue;
                    UInt32 varData = GetVar(varId);
                    if(varData == 0)
                        continue;
					buffid[c] = i + 0x40;
					buffleft[c] = varData;
                    ++c;
                }
                else if(_buffData[i] > curtime)
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
        st << getClanProffer();
        bool fchange = makeTitleAllInfo(st);
        st << Stream::eos;

        if(fchange)
            writeTitleAll();
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

	void Player::makeFighterSSList( Stream& st )
	{
		size_t c = _fighters.size();
		st.init(REP::SKILLSTRENGTHEN);
        st << static_cast<UInt8>(0);
		st << static_cast<UInt8>(c);
		for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
        {
            if (it->second)
                it->second->makeFighterSSInfo(st);
        }
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
            st << static_cast<UInt8>(fgt->getHideFashion());
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

	bool Player::attackNpc( UInt32 npcId, UInt32 turns, bool regen, bool needtype)
	{
        if (GetPackage()->GetRestPackageSize() <= 0)
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

        if (cfg.GMCheck && needtype && ng->getType()) // XXX: 必须是野外怪,天劫怪除外
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
#if 0
                if(getBuffData(PLAYER_BUFF_TRAINP3, now))
                    exp = ng->getExp() * 18 / 10;
                else if(getBuffData(PLAYER_BUFF_TRAINP4, now))
                    exp = ng->getExp() * 3 / 2;
                else if(getBuffData(PLAYER_BUFF_TRAINP2, now))
                    exp = ng->getExp() * 3 / 2;
                else if(getBuffData(PLAYER_BUFF_TRAINP1, now))
                    exp = ng->getExp() * 13 / 10;
                else
                    exp  = ng->getExp();
#else
                    exp  = ng->getExp();
#endif
                if (isOffical())
                    exp -= (exp/10);
                if((this->getPlatform() == 10 && World::getQQGameAct()) || (this->getPlatform() == 11 && World::get3366PrivilegeAct()) || ((getPlatform() == 1 || getPlatform() == 2) && World::getQzonePYPrivilegeAct()))
                {
                    UInt32 extraExp = exp / 2;//蓝黄钻野外手动打怪经验+50%
                    if(getPlatform() == 1 && World::getQzonePYPrivilegeAct())
                    {
                        SYSMSG_SENDV(1096, this, extraExp);
                        SYSMSG_SENDV(1097, this, extraExp);
                    }
                    else if(getPlatform() == 2 && World::getQzonePYPrivilegeAct())
                    {
                        SYSMSG_SENDV(1098, this, extraExp);
                        SYSMSG_SENDV(1099, this, extraExp);
                    }
                    else if(this->getPlatform() == 10 && World::getQQGameAct())
                    {
                        SYSMSG_SENDV(1092, this, extraExp);
                        SYSMSG_SENDV(1093, this, extraExp);
                    }
                    else
                    {
                        SYSMSG_SENDV(1094, this, extraExp);
                        SYSMSG_SENDV(1095, this, extraExp);
                    }
                    exp += extraExp;
                }
                if (cfg.rpServer && GetLev() < 70)
                    exp *= 2;
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

        if (getVipLevel() >= 7 && bsim.getTurns() > 30)
            setBuffData(PLAYER_BUFF_ATTACKING, now + 30);
        else
		    setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns());

		return res;
	}

    bool Player::attackTianjieNpc(UInt32 npcId, UInt32 expMulti, bool isEvent,bool isBoss)
	{
        int turns = 100;
		UInt32 now = TimeUtil::Now();
		UInt32 buffLeft = getBuffData(PLAYER_BUFF_ATTACKING, now);
		if(buffLeft > now)
		{
			sendMsgCode(0, 1407, buffLeft - now);
			return false;
		}
		checkLastBattled();

        GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
        if(it == GData::npcGroups.end())
			return false;

		GData::NpcGroup * ng = it->second;

        if (!ng)
            return false;

        if (cfg.GMCheck && (ng->getType() != 8 || GObject::Tianjie::instance().isTjOpened() == false)) //只能在天劫期间
            return false;

		if(GameAction()->RunExploreTask(this, npcId))
			turns = 0;
        UInt16 location = _playerData.location;
        if (isBoss)
            location = Battle::BS_WBOSS;
		Battle::BattleSimulator bsim(location, this, ng->getName(), ng->getLevel(), false, turns);
		PutFighters( bsim, 0 );
        ng->putFighters( bsim );
		bsim.start();

        Stream& packet = bsim.getPacket();
		if(packet.size() <= 8)
			return false;

		bool res = bsim.getWinner() == 1;
		if(res)
		{
			_lastNg = ng;
            UInt32 exp = TIANJIE_EXP(GetLev()) * ng->getExp() * expMulti;
            addExpOrTjScore(exp, 0, isEvent, true);
		}

        UInt16 ret = 0x0100;
        if (res) ret = 0x0101;

        Stream st(REP::ATTACK_NPC);
        st << ret << PLAYER_DATA(this, lastExp) << static_cast<UInt8>(0);
        UInt8 size = _lastLoot.size();
        st << size;
        for (UInt8 i = 0; i < size; ++i)
        {
            st << _lastLoot[i].id << _lastLoot[i].count;
        }
        st.append(&packet[8], packet.size() - 8);
        st << Stream::eos;
        send(st);

        turns = bsim.getTurns() > 15 ? 15 : bsim.getTurns();
		setBuffData(PLAYER_BUFF_ATTACKING, now + turns);
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
        bool isFull = false;
        return attackCopyNpc(id, 1/*XXX:使用这个背景*/, 5, 1, isFull, 1, false, NULL, false);
    }

	bool Player::attackCopyNpc( UInt32 npcId, UInt8 type, UInt8 copyId,
            UInt8 expfactor, bool& full, UInt8 lootlvl, bool ato, std::vector<UInt16>* loot, bool applayhp )
	{
        if (GetPackage()->GetRestPackageSize() == 0)
        {
            sendMsgCode(0, 1011);
            full = true;
            return false;
        }
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

            UInt32 exp = expfactor * ng->getExp();
            if(getBuffData(PLAYER_BUFF_QI_TIAN_CHU_MO, now))
                exp *= (18.f/10.f);
            pendExp(exp);

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

	bool Player::autoBattle( UInt32 npcId, UInt8 type)
	{
        if(type > 3)
            return false;
        if(GetLev() < 30)
            return false;
#if 0
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
#else
        SpotData * spotData = GetMapSpot();
        if (!spotData || spotData->m_Type != 9)
            return false;
#endif
        if (GetPackage()->GetRestPackageSize() == 0)
        {
            sendMsgCode(0, 1011);
            return false;
        }
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
        if (World::getAutoBattleAct())
            count = 60*216;

		UInt32 timeDur = count * eachBattle;
		UInt32 final = TimeUtil::Now() + timeDur;
#if 0
        GObject::EventWrapper::iterator it = GObject::eventWrapper.FindTimerEvent(this, EVENT_AUTOBATTLE, 0);
        if(it != GObject::eventWrapper.end())
        {
            GObject::EventBase *event = it->second;
            if(event)
            {
                count = event->GetTimer().GetLeftTimes();
                GObject::EventAutoBattle *eventAuto = static_cast<GObject::EventAutoBattle *>(event);
                final = eventAuto->getFinalEnd();
            }
        }
#endif
		EventAutoBattle* event = new(std::nothrow) EventAutoBattle(this, eachBattle, count, /*ng*/NULL, final);
		if (event == NULL) return false;
        SetVar(VAR_EXP_HOOK_INDEX, type);
		cancelAutoBattle(false);
		addFlag(Training);
		event->notify();
		event->updateDB(true);
		PushTimerEvent(event);
        OnHeroMemo(MC_FIGHTER, MD_STARTED, 0, 0);
        GameAction()->doStrong(this, SthTaskHook, 0,0);
		return true;
	}

	void Player::pushAutoBattle(UInt32 npcId, UInt16 count, UInt16 interval)
	{
		if(/*npcId == 0 || */count == 0 || interval == 0)
			return;
        if (count > 1440 && GetLev() < 45) // XXX: 45级以下不允许挂机240小时
            return;
#if 0
		GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
		if(it == GData::npcGroups.end())
			return;
		GData::NpcGroup * ng = it->second;
#endif
		UInt32 final = TimeUtil::Now() + interval * count;
		EventAutoBattle* event = new(std::nothrow) EventAutoBattle(this, interval, count, /*ng*/NULL, final);
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

	void Player::cancelAutoBattle(bool needNotify)
	{
		GameMsgHdr hdr2(0x179, WORKER_THREAD_WORLD, this, 0);
		GLOBAL().PushMsg(hdr2, 0);
        if(needNotify)
            cancelAutoBattleNotify();
	}

	void Player::cancelAutoBattleNotify()
	{
		Stream st(REP::TASK_RESPONSE_HOOK);
        UInt8 cnt = getMaxIcCount(getVipLevel()) - getIcCount();
        if (getMaxIcCount(getVipLevel()) < getIcCount())
            cnt = getMaxIcCount(getVipLevel());
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
        if(GetFreePackageSize() < 1)
        {
            sendMsgCode(2, 1011);
            return;
        }
		if(_playerData.icCount > getMaxIcCount(_vipLevel))
            _playerData.icCount = 0;
		if(_playerData.icCount >= getMaxIcCount(_vipLevel) || !hasFlag(Training) || getGoldOrCoupon() < 10)
			return;

        ConsumeInfo ci(InstantAutoBattle,0,0);
		useGoldOrCoupon(GData::moneyNeed[GData::INSTANTAUTOBATTLE].gold,&ci);
        incIcCount();
		GameMsgHdr hdr(0x178, WORKER_THREAD_WORLD, this, 0);
		GLOBAL().PushMsg(hdr, NULL);
        GameAction()->doStrong(this, SthHookSpeed, 0,0);
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
                if (lup.fighter->getClass() == 4)
                    OnShuoShuo(SS_MO_BATTLE);
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
        PutPets(bsim, side);
	}

	void Player::PutPets( Battle::BattleSimulator& bsim, int side, bool init /* = true */)
    {
        // 战斗模拟器中加载宠物
        if (_onBattlePet)
        {
            Battle::BattleFighter * bf = bsim.newPet(side, 0, _onBattlePet);
            bf->setHP(_onBattlePet->getMaxHP());
        }
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
			st << static_cast<UInt8>(0x01) << pl->getId() << pl->getName() << pl->getPF() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << pl->GetNewRelation()->getMood() << pl->GetNewRelation()->getSign() << GObject::gAthleticsRank.getAthleticsRank(pl) << static_cast<UInt8>(pl->isOnline()) << Stream::eos;
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
			st << static_cast<UInt8>(0x07) << pl->getId() << pl->getName() << pl->getPF() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry() << pl->GetLev() << pl->GetClass() << pl->getClanName() << pl->GetNewRelation()->getMood() << pl->GetNewRelation()->getSign() << GObject::gAthleticsRank.getAthleticsRank(pl) << static_cast<UInt8>(pl->isOnline()) << Stream::eos;
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
		st << static_cast<UInt8>(type) << static_cast<UInt8>(GetVar(VAR_HAS_VOTE)?1:0) << start << cnt << sz;
        if (sz && cnt)
        {
            std::set<Player *>::iterator it = _friends[type].begin();
            std::advance(it, start);
            for(UInt8 i = 0; i < cnt; ++ i)
            {
                Player * pl = *it;
                st << pl->getId() << pl->getName() << pl->getPF() << static_cast<UInt8>(pl->IsMale() ? 0 : 1) << pl->getCountry()
                    << pl->GetLev() << pl->GetClass() << pl->getClanName() << pl->GetNewRelation()->getMood() << pl->GetNewRelation()->getSign() << GObject::gAthleticsRank.getAthleticsRank(pl);
                st << static_cast<UInt8>(pl->isOnline());
                ++it;
            }
        }
		st << Stream::eos;
		send(st);
	}

    void Player::vote(Player* other)
    {
        if (GetVar(VAR_HAS_VOTE))
        {
            return;
        }
        SetVar(VAR_HAS_VOTE, 1);
        GameMsgHdr hdr(0x360, other->getThreadId(), other, 0);
        GLOBAL().PushMsg(hdr, NULL);
        sendMsgCode(0, 1509);
        //GameMsgHdr hdr2(0x1C6, WORKER_THREAD_WORLD, this, 0);
        //GLOBAL().PushMsg(hdr2, NULL);
    }

    void Player::beVoted()
    {
        AddVar(VAR_POPULARITY, 1);
        UInt32 total = GetVar(VAR_POPULARITY);
        GameMsgHdr hdr(0x1C7, WORKER_THREAD_WORLD, this, sizeof(total));
        GLOBAL().PushMsg(hdr, &total);
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
        if (cfg.rpServer && t == 7 && TimeUtil::Now() < World::getOpenTime() + 7 * 86400)
        {
            SetVar(VAR_RP7_RECHARGE, v);
        }
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

        if (ii && ii->incommingType == InFromSale)
        {
            char gold[32] = {0};
            snprintf(gold, sizeof(gold), "%u", c);
            udpLog("sale", gold, "", "", "", "", "currency");
        }

#ifndef _WIN32
#ifdef _FB
#else
        if (ii)
            dclogger.gold_sec(this, c, ii->incommingType, true);
        else
            dclogger.gold_sec(this, c, 0, true);
#endif
#endif
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
        {
            udpLog(ci->purchaseType, ci->itemId, ci->itemNum, c, "add");
        }

        // 统计鹊桥道具购买的日志
        if (ci && ci->itemId == 9122)
        {
            udpLog("qixi", "I_9122_1", "", "", "", "", "act", ci->itemNum);
        }
#ifndef _WIN32
#ifdef _FB
#else
        dclogger.consume(this, _playerData.gold, c);
        if (ci)
            dclogger.gold_sec(this, c, ci->purchaseType, false);
        else
            dclogger.gold_sec(this, c, 0, false);

#endif
#endif // _WIN32
        if(ci && ci->purchaseType != TrainFighter)
            AddVar(VAR_USEGOLD_CNT, c);
        return _playerData.gold;
	}

    void Player::deleteGold(UInt32 c)
    {
        UInt32 n = _playerData.gold;
        if( c > _playerData.gold || c == 0)
        {
            _playerData.gold = 0;
        }
		else
		{
		    _playerData.gold -= c;
            n = c;
        }
        sendModification(1, _playerData.gold);
        udpLog("clear", "F_1158", "", "", "", "", "act", n);
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

    UInt32 Player::useGoldInLua(UInt32 c, UInt32 pt)
    {
		ConsumeInfo ci(pt,0,0);
		return useGold(c, &ci);
    }

    UInt32 Player::getGoldInLua()
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

#ifndef _WIN32
#ifdef _FB
#else
                if (ci)
                    dclogger.gold_sec(this, c, ci->purchaseType, false);
                else
                    dclogger.gold_sec(this, c, 0, false);
#endif
#endif

                if(ci!=NULL)
                {
                    DBLOG1().PushUpdateData("insert into consume_gold (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                            cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, c, TimeUtil::Now());
                }

                if (ci && ci->purchaseType == PurchaseSale)
                {
                    char gold[32] = {0};
                    snprintf(gold, sizeof(gold), "-%u", _holdGold);
                    udpLog("sale", gold, "", "", "", "", "currency");
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
#ifndef _WIN32
#ifdef _FB
#else
        dclogger.coupon_sec(this, c, 0, true);
#endif
#endif
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
#ifndef _WIN32
#ifdef _FB
#else
            if (ci)
                dclogger.coupon_sec(this, c, ci->purchaseType, false);
            else
                dclogger.coupon_sec(this, c, 0, false);
#endif
#endif
        }
		SYSMSG_SENDV(156, this, c);
		SYSMSG_SENDV(1056, this, c);
		sendModification(2, _playerData.coupon);
        {
            AddVar(VAR_USECOUPON_CNT, c);
            daysValueRankMsg msg;
            msg.playerId = getId();
            msg.type = 2;
            msg.value = GetVar(VAR_USETAEL_CNT) + GetVar(VAR_USECOUPON_CNT) * 100;
            GameMsgHdr hdr(0x1EC, WORKER_THREAD_WORLD, this, sizeof(msg));
            GLOBAL().PushMsg(hdr, &msg);
        }
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
#ifndef _WIN32
#ifdef _FB
#else
        dclogger.tael_sec(this, c, 0, true);
#endif
#endif
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
#ifndef _WIN32
#ifdef _FB
#else
            if (ci)
                dclogger.tael_sec(this, c, ci->purchaseType, false);
            else
                dclogger.tael_sec(this, c, 0, false);
#endif
#endif
        }
        SYSMSG_SENDV(152, this, c);
        SYSMSG_SENDV(1052, this, c);
		sendModification(3, _playerData.tael);
        if(ci && ci->purchaseType != TrainFighter)
        {
            AddVar(VAR_USETAEL_CNT, c);
            daysValueRankMsg msg;
            msg.playerId = getId();
            msg.type = 2;
            msg.value = GetVar(VAR_USETAEL_CNT) + GetVar(VAR_USECOUPON_CNT) * 100;
            GameMsgHdr hdr(0x1EC, WORKER_THREAD_WORLD, this, sizeof(msg));
            GLOBAL().PushMsg(hdr, &msg);
        }
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
#ifndef _WIN32
#ifdef _FB
#else
            if (ci)
                dclogger.tael_sec(this, c, ci->purchaseType, false);
            else
                dclogger.tael_sec(this, c, 0, false);
#endif
#endif
        }
        SYSMSG_SENDV(152, this, c);
        sendModification(3, _playerData.tael);
    }

    UInt32 Player::getMoneyArenaLua(UInt32 c)
    {
        IncommingInfo ii(ArenaFromCard,0,0);
        return getMoneyArena(c, &ii);
    }

    UInt32 Player::getMoneyArena( UInt32 c, IncommingInfo* ii)
    {
        UInt32 moneyArena = GetVar(VAR_MONEY_ARENA);
		if(c == 0)
			return moneyArena;
		moneyArena += c;
		SYSMSG_SENDV(164, this, c);
		SYSMSG_SENDV(1064, this, c);
        SetVar(VAR_MONEY_ARENA, moneyArena);

        Stream st(REP::USER_INFO_CHANGE);
        st << static_cast<UInt8>(0x16) << moneyArena << Stream::eos;
        send(st);

        if(ii && ii->incommingType != 0)
        {
            DBLOG1().PushUpdateData("insert into consume_arena (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ii->incommingType, ii->itemId, ii->itemNum, c, TimeUtil::Now());
        }

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
                DBLOG1().PushUpdateData("insert into consume_arena (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
            }
        }
        SYSMSG_SENDV(163, this, a);
        SYSMSG_SENDV(1063, this, a);
        SetVar(VAR_MONEY_ARENA, moneyArena);

        Stream st(REP::USER_INFO_CHANGE);
        st << static_cast<UInt8>(0x16) << moneyArena << Stream::eos;
        send(st);

        return moneyArena;
    }

	UInt32 Player::getClanProffer( UInt32 c, IncommingInfo* ii)
	{
        Clan* clan = getClan();
        if(!clan)
            return 0;
        ClanMember* member = clan->getClanMember(this);
        if(!member)
            return 0;
        if(c == 0)
			return member->proffer;
		member->proffer += c;
		SYSMSG_SENDV(166, this, c);
		SYSMSG_SENDV(1066, this, c);
        {
            Stream st(REP::CLAN_INFO_UPDATE);
            st << static_cast<UInt8>(5) << member->proffer << Stream::eos;
            send(st);
        }
        DB5().PushUpdateData("UPDATE `clan_player` SET `proffer` = %u WHERE `playerId` = %"I64_FMT"u", member->proffer, this->getId());

        if(ii && ii->incommingType != 0)
        {
            DBLOG1().PushUpdateData("insert into consume_clan_proffer (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ii->incommingType, ii->itemId, ii->itemNum, c, TimeUtil::Now());
        }

		return member->proffer;
	}

	UInt32 Player::useClanProffer( UInt32 a,ConsumeInfo * ci )
	{
        Clan* clan = getClan();
        if(!clan)
            return 0;
        ClanMember* member = clan->getClanMember(this);
        if(!member)
            return 0;
        if(member->proffer < a)
            member->proffer = 0;
        else
        {
            member->proffer -= a;
            if(ci!=NULL)
            {
                DBLOG1().PushUpdateData("insert into consume_clan_proffer (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
            }
        }
        SYSMSG_SENDV(165, this, a);
        SYSMSG_SENDV(1065, this, a);
        {
            Stream st(REP::CLAN_INFO_UPDATE);
            st << static_cast<UInt8>(5) << member->proffer << Stream::eos;
            send(st);
        }
        DB5().PushUpdateData("UPDATE `clan_player` SET `proffer` = %u WHERE `playerId` = %"I64_FMT"u", member->proffer, this->getId());

        return member->proffer;
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
            if(fgt->getLevel() >= golds.size())
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
            if(fgt->getLevel() >= taels.size())
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

    UInt8 Player::getTitle_noCheck()
    {
        return _playerData.title;
    }

    UInt8 Player::getTitle()
    {
        UInt32 timeLeft = 0;
        if(CURRENT_THREAD_ID() == getThreadId())
        {
            if(!checkTitleTimeEnd(_playerData.title, timeLeft))
            {
                notifyTitleAll();
                writeTitleAll();
                changeTitle(0);
            }
        }
        else
        {
            GameMsgHdr h(0x264,  getThreadId(), this, 0);
            GLOBAL().PushMsg(h, NULL);
        }

        return _playerData.title;
    }

    void Player::loadTitleAll(UInt8 t, UInt32 timeEnd)
    {
        std::map<UInt8, UInt32>& titleAll = _playerData.titleAll;
        titleAll[t] = timeEnd;
    }

	void Player::fixOldVertionTitle( UInt8 t)
	{
        _playerData.titleAll[t] = 0;
        writeTitleAll();
	}

	void Player::setTitle( UInt8 t, UInt32 timeLen )
	{
        if(CURRENT_THREAD_ID() != getThreadId())
        {
            UInt8 thr = getThreadId();
            if(0xFF == thr)
                thr = getCountry();

            struct TitleData
            {
                UInt8 title;
                UInt32 timeLen;
            } titleData = {t, timeLen};
            GameMsgHdr h(0x265,  thr, this, sizeof(titleData));
            GLOBAL().PushMsg(h, &titleData);
            return;
        }

        std::map<UInt8, UInt32>& titleAll = _playerData.titleAll;
        bool flag = false;
        std::map<UInt8, UInt32>::iterator it = _playerData.titleAll.find(t);
        if(it == titleAll.end())
        {
            if(timeLen == 0)
                titleAll[t] = 0;
            else
                titleAll[t] = TimeUtil::Now() + timeLen;
            flag = true;
        }
        else if(it->second != 0)
        {
            if(timeLen == 0)
                titleAll[t] = 0;
            else
                titleAll[t] = TimeUtil::Now() + timeLen;
            flag = true;
        }
        bool fchange = notifyTitleAll();
        if(flag || fchange)
        {
            writeTitleAll();
        }

		if(t == _playerData.title)
			return;
		_playerData.title = t;
		sendModification(6, _playerData.title);
		rebuildBattleName();
	}

    bool Player::notifyTitleAll()
    {
        Stream st(REP::USER_INFO_CHANGE);
        st << static_cast<UInt8>(0x17);
        bool flag = makeTitleAllInfo(st);
        st << Stream::eos;
        send(st);

        return flag;
    }

    void Player::changeTitle(UInt8 t)
    {
        if(!hasTitle(t))
            return;

        UInt32 timeLeft = 0;
        if(!checkTitleTimeEnd(t, timeLeft))
        {
            notifyTitleAll();
            writeTitleAll();
            return;
        }

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
        {
            AddVar(VAR_GETACHIEVEMENT_CNT, a);
            daysValueRankMsg msg;
            msg.playerId = getId();
            msg.type = 3;
            msg.value = GetVar(VAR_GETACHIEVEMENT_CNT) + GetVar(VAR_GETPRESTIGE_CNT);
            GameMsgHdr hdr(0x1EC, WORKER_THREAD_WORLD, this, sizeof(msg));
            GLOBAL().PushMsg(hdr, &msg);
        }
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
        st << static_cast<UInt8>(0x15) << _playerData.prestige << Stream::eos;
        send(st);
        {
            AddVar(VAR_GETPRESTIGE_CNT, a);
            daysValueRankMsg msg;
            msg.playerId = getId();
            msg.type = 3;
            msg.value = GetVar(VAR_GETACHIEVEMENT_CNT) + GetVar(VAR_GETPRESTIGE_CNT);
            GameMsgHdr hdr(0x1EC, WORKER_THREAD_WORLD, this, sizeof(msg));
            GLOBAL().PushMsg(hdr, &msg);
        }
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
        st << static_cast<UInt8>(0x15) << _playerData.prestige << Stream::eos;
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

	void Player::AddExp(UInt64 exp, UInt8 mlvl, UInt32 extraExp)
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
                extraExp = 0;
				SYSMSG_SENDV(184, this);
				SYSMSG_SENDV(1084, this);
				return;
			}
			else if(onlineDuration >= 3 * 60 * 60)
			{
				exp /= 2;
                extraExp /= 2;
				SYSMSG_SENDV(181, this);
				SYSMSG_SENDV(1081, this);
            }
		}
		for(int i = 0; i < 5; ++ i)
		{
			GObject::Fighter * fgt = getLineup(i).fighter;
			if(fgt != NULL)
				fgt->addExp(exp, extraExp);
		}
        //是否开启天劫
        GObject::Tianjie::instance().isOpenTj(this);
        sendLevelPack(GetLev()); // XXX:
        if (cfg.rpServer && GetLev()>=70)
            setBuffData(PLAYER_BUFF_EXPDOUBLE,0);
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
        SpotData * spotData = GetMapSpot();
        if(spotData && spotData->m_CountryBattle)
        {
            GObject::CountryBattle *cb = spotData->GetCountryBattle();
	        NewCountryBattle * ncb = spotData->GetNewCountryBattle();
            if(WORLD().isNewCountryBattle() && ncb)
                ncb->playerLeave(this);
            if(!WORLD().isNewCountryBattle() && cb)
                cb->playerLeave(this);
        }
        ClanRankBattleMgr::Instance().PlayerLeave(this);

        if (_playerData.location == CLAN_COPY_LOCATION)
        {
            ClanCopyMgr::Instance().playerLeave(this);
        }

        if (_jobHunter)
            _jobHunter->OnLeaveGame(_playerData.location);

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

        ClanCopyMgr::Instance().playerEnter(this);

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


    ////////////////////////////////////////////
    // 帮派副本

    void Player::setClanCopyLevel(UInt16 level)
    {
        // GM命令设置帮派副本等级
        if (_clan == NULL)
            return;
        _clan->setCopyLevel(level);
    }

    void Player::setClanCopyTime(UInt32 time)
    {
        // GM命令设置帮派副本每轮时间
        ClanCopyMgr::Instance().setInterval(time);
    }


    // 帮派副本
    ////////////////////////////////////////////


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
        return find(tasks.begin(), tasks.end(), task) != tasks.end();
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
        /*
        if(type > 0)
            GameAction()->doAty(this, AtyBarRef, 0, 0);
        */
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
		//if(hasFlag(CountryBattle | ClanBattling | ClanRankBattle))
		if(hasFlag(ClanBattling | ClanRankBattle))
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
            // 道默认站前排
			bool mfSolid = lu1.fighter->getClass() == e_cls_dao;
			if(mfSolid)
				lu1.pos = newPos[starti ++];
			for(int i = 1; i < 5; ++ i)
			{
				Lineup& lu = _playerData.lineup[i];
				if(!lu.available())
					continue;
                if (!lu.fighter)
                    continue;
				if(lu.fighter->getClass() == e_cls_dao)
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
        if(nLev >= 47)
        {
            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA(this, created));
            if(thisDay <= endDay)
            {
                UInt32 targetVal = GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_LEVEL))
                {
                    targetVal |= TARGET_LEVEL;
                    AddVar(VAR_CTS_TARGET_COUNT, 1);
                    SetVar(VAR_CLAWARD2, targetVal);
                    sendNewRC7DayTarget();
                    newRC7DayUdpLog(1152, 1);
                }
            }
        }
        
        if(World::getTowerLoginAct() && !GetVar(VAR_TOWER_LEVEL) && strcasestr(m_via.c_str(), "sscq_dlhd") && nLev >= 40)
        {
            SYSMSGV(title, 4106);
            SYSMSGV(content, 4107);
            Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[6] = {{516,1},{509,1},{507,1},{56,3},{57,3},{503,1}};
                mailPackageManager.push(mail->id, mitem, 6, true);
            }
            SetVar(VAR_TOWER_LEVEL, 1);
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

        Stream stream(REQ::STATE);
        stream<<static_cast<UInt8>(0x19);
        stream<<r;
    //    stream<<_playerData.totalRecharge;
        stream<<Stream::eos;
        send(stream);

		sendVIPMails(oldVipLevel + 1, _vipLevel);
        addRC7DayRecharge(r);
        addRF7DayRecharge(r);
        addRechargeNextRet(r);
        if (cfg.rpServer)
        {
            GameMsgHdr hdr(0x1CA, WORKER_THREAD_WORLD, this, sizeof(_playerData.totalRecharge));
            GLOBAL().PushMsg(hdr, &_playerData.totalRecharge);
        }
        
        if (World::getRechargeActive())
        {
            UInt32 total = GetVar(VAR_RECHARGE_TOTAL);
            GameAction()->sendRechargeMails(this, total, total+r);
            SetVar(VAR_RECHARGE_TOTAL, total+r);
            sendRechargeInfo(true);
        }

        if (World::getRechargeActive3366() && atoi(m_domain) == 11)
        {
            UInt32 total = GetVar(VAR_RECHARGE_TOTAL3366);
            GameAction()->sendRechargeMails(this, total, total+r);
            SetVar(VAR_RECHARGE_TOTAL3366, total+r);
            sendRechargeInfo(true);
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

        if(World::getRechargeActive())
        {
            GObject::RechargeTmpl::instance().addScore(this, GetVar(VAR_RECHARGE_TOTAL)-r, GetVar(VAR_RECHARGE_TOTAL));
            GObject::RechargeTmpl::instance().sendScoreInfo(this);
        }

        AddVar(VAR_FIRST_RECHARGE_VALUE, r);
        sendFirstRecharge();

        bool flag = false;

        UInt32 now = TimeUtil::Now();
        if ((GVAR.GetVar(GVAR_DISCOUNT_TYPE1) == 2)
                && GVAR.GetVar(GVAR_DISCOUNT_BEGIN1) < now
                && GVAR.GetVar(GVAR_DISCOUNT_END1) > now)
        {
            SetVar(VAR_DISCOUNT_EX1_TIME, GVAR.GetVar(GVAR_DISCOUNT_END1));
            AddVar(VAR_DISCOUNT_RECHARGE1, r);
            flag = true;
        }
        else
            SetVar(VAR_DISCOUNT_RECHARGE1, 0);
        if ((GVAR.GetVar(GVAR_DISCOUNT_TYPE2) == 2)
                && GVAR.GetVar(GVAR_DISCOUNT_BEGIN2) < now
                && GVAR.GetVar(GVAR_DISCOUNT_END2) > now)
        {
            SetVar(VAR_DISCOUNT_EX2_TIME, GVAR.GetVar(GVAR_DISCOUNT_END2));
            AddVar(VAR_DISCOUNT_RECHARGE2, r);
            flag = true;
        }
        else
            SetVar(VAR_DISCOUNT_RECHARGE2, 0);
        if ((GVAR.GetVar(GVAR_DISCOUNT_TYPE3) == 2)
                && GVAR.GetVar(GVAR_DISCOUNT_BEGIN3) < now
                && GVAR.GetVar(GVAR_DISCOUNT_END3) > now)
        {
            SetVar(VAR_DISCOUNT_EX3_TIME, GVAR.GetVar(GVAR_DISCOUNT_END3));
            AddVar(VAR_DISCOUNT_RECHARGE3, r);
            flag = true;
        }
        else
            SetVar(VAR_DISCOUNT_RECHARGE3, 0);
        if (flag)
            sendDiscountLimit();
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

    void Player::sendRechargeInfo(bool rank)
    {
        if (!World::getRechargeActive() && !World::getRechargeActive3366())
            return;

        UInt32 total;
        if(World::getRechargeActive())
            total = GetVar(VAR_RECHARGE_TOTAL);
        else
            total = GetVar(VAR_RECHARGE_TOTAL3366);

        Stream st(REP::DAILY_DATA);
        st << static_cast<UInt8>(12) << total << Stream::eos;
        send((st));
           
        if (rank && World::getNeedRechargeRank())
        {
            GameMsgHdr hdr(0x1C1, WORKER_THREAD_WORLD, this, sizeof(total));
            GLOBAL().PushMsg(hdr, &total);
        }
    }

    void Player::sendConsumeInfo(bool rank)
    {
        if (!World::getConsumeActive())
            return;

        UInt32 total = GetVar(VAR_CONSUME);
		Stream st(REP::DAILY_DATA);
		st << static_cast<UInt8>(15) << total << Stream::eos;
		send((st));

        if (rank && World::getNeedConsumeRank())
        {
            GameMsgHdr hdr(0x1C2, WORKER_THREAD_WORLD, this, sizeof(total));
            GLOBAL().PushMsg(hdr, &total);
        }
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
            //ItemBase* item = 
            m_Package->Add(ids[i], ((UInt8)(i+1)>=ids.size())?1:ids[i+1], true, false, FromOnlineAward);
            //if(!item)
                // return false;
                //; // XXX: ugly
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

        UInt8 currentCnt2 = 0;
        UInt8 totalCnt2 = 0;
        if(this->isQQVIP() && World::getQQVipAct()){
            currentCnt2 = this->GetVar(VAR_QQVIP_CNT);
            totalCnt2 = 1;
            if(currentCnt2 > totalCnt2)
                currentCnt2 = 0;
        }
        copy = freeCnt + goldCnt + currentCnt + currentCnt2;
        copyMax = GObject::PlayerCopy::getFreeCount() + GObject::PlayerCopy::getGoldCount(vipLevel) + totalCnt + totalCnt2;

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

        UInt8 iccnt = getMaxIcCount(vipLevel) - getIcCount();
        if (getMaxIcCount(vipLevel) < getIcCount())
            iccnt = getMaxIcCount(vipLevel);
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

        UInt8 currentCnt2 = 0;
        UInt8 totalCnt2 = 0;
        if(this->isQQVIP() && World::getQQVipAct()){
            currentCnt2 = this->GetVar(VAR_QQVIP_CNT);
            totalCnt2 = 1;
            if(currentCnt2 > totalCnt2)
                currentCnt2 = 0;
        }

        st << cnt << static_cast<UInt8>(freeCnt + goldCnt + currentDiamondCnt + currentCnt2) << static_cast<UInt8>(GObject::PlayerCopy::getFreeCount()) << static_cast<UInt8>(GObject::PlayerCopy::getGoldCount(vipLevel)) << static_cast<UInt8>(totalDiamondCnt) << static_cast<UInt8>(totalCnt2);
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
                    /*
                    if (decp < fgt_orig->getPotential())
                        decp = fgt_orig->getPotential();
                    */
                    fgt->setPotential(decp);
                }
                else
                {
                    float decp = fgt->getCapacity() - 0.1f;
                    if (decp < static_cast<float>(GObjectManager::getMinCapacity())/100)
                        decp = static_cast<float>(GObjectManager::getMinCapacity())/100;
                    /*
                    if (decp < fgt_orig->getCapacity())
                        decp = fgt_orig->getCapacity();
                    */
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
        isDoubleExp(exp);
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

    void Player::patchDeleteDotS(std::string& name)
    {
        if(cfg.merged && !name.empty())
        {
            UInt16 serverNo;
            std::string nameTmp = name;
            size_t pos = nameTmp.find(".S");
            if(pos == std::string::npos)
                pos = nameTmp.find(".s");
            if(pos == std::string::npos)
                return;
            if(pos + 2 >= nameTmp.size())
                return;
            std::string tmp(nameTmp.begin()+pos+2, nameTmp.end());
            serverNo = atoi(tmp.c_str());
            if(serverNo == (this->getId() >> 48))
            {
                nameTmp.erase(nameTmp.begin()+pos, nameTmp.end());
                name = nameTmp;
            }
        }
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
#if 0
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
#endif
    const char* Player::getNameNoSuffix(std::string name)
    {
        if(!cfg.merged || name.size() == 0)
        {
            _playerData.nameNoSuffix = name;
            return _playerData.nameNoSuffix.c_str();
        }
        Int32 len = name.size() - 1;
        for (; len > 0; --len)
        {
            if (static_cast<UInt8>(name[len]) >= 32)
                break;
        }
        name.resize(len+1);
        _playerData.nameNoSuffix = name;
        return _playerData.nameNoSuffix.c_str();
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

			SYSMSG(title, 256);
			SYSMSGV(content, 257, vipTable[j-1][0]);
			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
			if(mail == NULL)
				continue;

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
		GData::LootResult lt;
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
        {
            GetShuoShuo()->setShuoSafe(idx);
            if (idx == SS_PUBTST_PKG)
            {
                char action[16] = "";
                snprintf (action, 16, "F_%d", 1101);
                udpLog("916", action, "", "", "", "", "act");
            }
        }
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
        //char separator[2] = {32, 0};
        char separator[] = {"\n "}; //分隔符是回车加空格

        std::string sepStr(separator);
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
		//_battleName = _battleName + "\n" + numstr + "\n" + _playerData.name;
		_battleName = _battleName + sepStr + numstr + sepStr + _playerData.name;
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
        /*
        if(type > 0)
            GameAction()->doAty(this, AtyBookStore, 0 , 0);
        */
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
        if((this->getPlatform() == 10 && World::getQQGameAct()) || (this->getPlatform() == 11 && World::get3366PrivilegeAct()) || ((getPlatform() == 1 || getPlatform() == 2) && World::getQzonePYPrivilegeAct()))
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
        float factor = 0.0f;
        if(getBuffData(PLAYER_BUFF_ADVANCED_P_HOOK, TimeUtil::Now()))
        {
            factor = 0.2f;
        }
        else if(getBuffData(PLAYER_BUFF_PRACTICE1, TimeUtil::Now()))
        {
            factor = 0.5f;
        }
#if 0
        if(getBuffData(PLAYER_BUFF_PRACTICE2, TimeUtil::Now()))
        {
            return 0.5f;
        }
#else
        else if(getBuffData(PLAYER_BUFF_PROTECT, TimeUtil::Now()))
        {
            factor = 0.2f;
        }
#endif
        // 限时vip特权
        if(inVipPrivilegeTime())
            factor += 0.2f;

        return factor;
    }

    float Player::getPracticeIncByDiamond()
    {
        if(this->isBD() && World::getBlueDiamondAct())
        {
            return 0.1f;
        }
        else if(this->isYD() && World::getYellowDiamondAct())
        {
            return 0.1f;
        }
        else
        {
            return 0.0f;
        }
    }

    float Player::getPracticeIncByQQVip()
    {
        if(isQQVIP() && World::getQQVipAct())
        {
            return 0.1f;
        }
        else
        {
            return 0.0f;
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
                    UInt32 extraPExp = 0;
                    UInt32 pExp = fgt->getPracticeInc() * pfexp->counts[i];
                    if(inVipPrivilegeTime())
                        extraPExp = fgt->getBasePExpEach() * pfexp->counts[i] * 0.2f;
                    isDoubleExp(pExp);
                    fgt->addPExp(pExp, true, false, extraPExp);
                }
            }

            UInt32 now = TimeUtil::Now();
            UInt32 duration = 60*60;
            UInt8 type = 0;
            UInt32 p = getBuffData(PLAYER_BUFF_ADVANCED_P_HOOK, now);
            if(!p)
                p = getBuffData(PLAYER_BUFF_PROTECT, now);
            if (!p)
            {
                p = getBuffData(PLAYER_BUFF_PRACTICE1, now);
                type = 1;
            }
            UInt32 left = 0;
            if (p > 0)
                left = p - now;

            if (left >= duration)
            {
                left -= duration;
                if (type == 0)
                {
                    if(getBuffData(PLAYER_BUFF_ADVANCED_P_HOOK, now))
                        setBuffData(PLAYER_BUFF_ADVANCED_P_HOOK, left+now);
                    else
                        setBuffData(PLAYER_BUFF_PROTECT, left+now);
                }
                else
                    setBuffData(PLAYER_BUFF_PRACTICE1, left+now);
            }
            else if (left)
            {
                if (type == 0)
                {
                    if(getBuffData(PLAYER_BUFF_ADVANCED_P_HOOK, now))
                        setBuffData(PLAYER_BUFF_ADVANCED_P_HOOK, 0);
                    else
                        setBuffData(PLAYER_BUFF_PROTECT, 0);
                }
                else
                    setBuffData(PLAYER_BUFF_PRACTICE1, 0);
            }
        }
        else
        {
            for(int i = 0; i < MAX_PRACTICE_FIGHTRES; ++ i)
            {
                Fighter* fgt = findFighter(pfexp->fids[i]);
                if(fgt && pfexp->counts[i])
                {
                    UInt32 extraPExp = 0;
                    UInt32 pExp = fgt->getPracticeInc() * pfexp->counts[i];
                    if(inVipPrivilegeTime())
                        extraPExp = fgt->getBasePExpEach() * pfexp->counts[i] * 0.2f;
                    isDoubleExp(pExp);
                    fgt->addPExp(pExp, true, false, extraPExp);
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
        UInt8 domain = atoi(m_domain);
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

            if (blue)
            {
                flag = 8*(_playerData.qqvipl1 / 10);
                qqvipl = _playerData.qqvipl1 % 10;
            }
            else
            {
                if(qplus /*&& World::getQQVipAct()*/ && _playerData.qqvipl1 >= 40 && _playerData.qqvipl1 <= 49)
                {
                    flag = 8*((_playerData.qqvipl1-20) / 10);
                    qqvipl = _playerData.qqvipl1 % 10;
                }
                else if (_playerData.qqvipl >= 40)
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
            st << qqvipl << _playerData.qqvipyear << static_cast<UInt8>(((_playerData.qqawardgot>>flag)|(GetVar(VAR_HHBAWARD_GOT) << 3))& 0x0B);
            UInt8 maxCnt = GObjectManager::getYDMaxCount();
            if(flag == 8)
                st << static_cast<UInt8>(maxCnt - 1);
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
                if(flag == 8 && (i == 0 || i > 7))
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

        UInt8 domain = atoi(m_domain);
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

#ifndef _WIN32
#ifdef _FB
#else
                    dclogger.d3d6(this);
#endif
#endif // _WIN32
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
        else if (_playerData.qqvipl < 20 || _playerData.qqvipl >= 40 || (domain == 4 && _playerData.qqvipl1 >= 40) ||
                ((domain == 11 || domain == 4) && d3d6 == 0 && _playerData.qqvipl1 > 0))
        {
            UInt8 qqvipl = 0;
            UInt8 flag = 0;
            if ((domain == 11 || domain == 4) && d3d6 == 0 && _playerData.qqvipl1 > 0)
            {
                if (World::getQQVipAct() && _playerData.qqvipl1 >= 40 && _playerData.qqvipl1 <= 49)
                {
                    qqvipl = _playerData.qqvipl1;
                    flag = 8*((_playerData.qqvipl1-20) / 10);
                }
                else
                {
                    qqvipl = _playerData.qqvipl1;
                    flag = 8*(_playerData.qqvipl1 / 10);
                }
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

#ifndef _WIN32
#ifdef _FB
#else
                    dclogger.blue(this);
#endif
#endif // _WIN32
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
            
            else if(type == 3 &&_playerData.isHHBlue &&!GetVar(VAR_HHBAWARD_GOT))
            {
                if(GetPackage()->GetRestPackageSize() > 0)
                {
                    nRes = 5;
                    SetVar(VAR_HHBAWARD_GOT, 1);

                    GetPackage()->AddItem2(503,1, true, true);
                }
                else
                {
                    sendMsgCode(2, 1011);
                }
            }

        }
        if(nRes && nRes != 5)
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
        if (PracticePlace::_picCnt[nVipLevel] < _playerData.picCount)
            _playerData.picCount = 0;
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
                std::vector<GData::LootResult> lr;
                li->roll(lr);
                if (lr.size())
                {
                    m_td.itemId = lr[0].id;
                    m_td.num = lr[0].count;
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

        if (!World::canDestory(itemid))
            return;

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

        GameAction()->doStrong(this, SthTripodFire , 0, 0);
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
        {
            GetPackage()->AddEquip(m_td.itemId, true, false, FromTripod);
            tripodUdpLog(1140, m_td.itemId);
        }
        else
        {
            GetPackage()->AddItem(m_td.itemId, m_td.num, true, false, FromTripod);
            tripodUdpLog(1140, m_td.itemId, m_td.num);
        }

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
        case 4:
            // 回流今日目标
            getTargetAwardRF(opt);
            break;

        case 5:
            getSoSoMapAward();
            break;
        case 6:
            getNewRegisterAward(opt);
            break;
        case 7:
            //推广用注册玩家登录奖励领取
            getAwardFromAD();
            break;
        case 8:
            //回流用户新区道具奖
            getAwardFromRF();
            break;
        case 9:
            //领取礼物卡
            getAwardGiftCard();
            break;
        case 10:
            getAwardBirthday(opt);
            break;
        case 11:
            getAwardLogin(opt);
            break;
        case 12:
            getAwardBlueDiamond(opt);
            break;
        case 13:
            get11DailyAward(opt);
            break;
        case 14:
            getSSToolbarAward();
        case 15:
            getThanksGivingDay(opt);
            break;
        case 16:
            getConsumeAward();
            break;
        }
    }

    void Player::getSSToolbarAward()
    {
        if (!World::getSSToolbarAct())
            return;
        if (GetPackage()->GetRestPackageSize() < 6)
        {
			sendMsgCode(0, 1011);
            return;
        }
        if (GetVar(VAR_AWARD_SSTOOLBAR) == 0 )
        {
            GetPackage()->Add(509, 1, true);
            GetPackage()->Add(50, 1, true);
            GetPackage()->Add(49, 1, true);
            GetPackage()->Add(133, 1, true);
            GetPackage()->Add(500, 1, true);
            GetPackage()->Add(56, 1, true);
            SetVar(VAR_AWARD_SSTOOLBAR, 1);
            udpLog("huodong", "F_10000_16", "", "", "", "", "act");
        }
        sendSSToolbarInfo();
    }
    void Player::sendSSToolbarInfo()
    {
        if (!World::getSSToolbarAct())
            return;
        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(14);
        UInt8 res = GetVar(VAR_AWARD_SSTOOLBAR);
        st << res << Stream::eos;
        send(st);
    }
    void Player::get11DailyAward(UInt8 opt)
    {
        if(!World::get11Act())
            return;

        if(opt == 1) 
        {
            if(GetVar(VAR_ACT_LOGIN_AWARD) != 0)
                return;
            if(!GameAction()->Run11ActAward(this, opt))
                return;
            SetVar(VAR_ACT_LOGIN_AWARD, 1);
        }
        else if(opt == 2) 
        {
            if(GetVar(VAR_ACT_LOGIN_AWARD_VIP) != 0 || getVipLevel() == 0)
                return;
            if(!GameAction()->Run11ActAward(this, opt))
                return;
            SetVar(VAR_ACT_LOGIN_AWARD_VIP, 1);
        }     

        send11DailyInfo();
    }

    void Player::send11DailyInfo()
    {
        if(!World::get11Act())
            return;

        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(13);
        UInt8 normal_award = GetVar(VAR_ACT_LOGIN_AWARD);
        UInt8 vip_award = GetVar(VAR_ACT_LOGIN_AWARD_VIP);
        st << normal_award << vip_award << Stream::eos;
        send(st);
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

    void Player::getNewRegisterAward(UInt8 opt)
    {
        if(opt != 0 && opt != 1)
            return;
        if(GetLev() > 1)
            return;
        if(opt == 1)
        {
            if(1 != GetVar(VAR_AWARD_NEWREGISTER))
                return;
			std::vector<GData::LootResult>::iterator it;
			for(it = _RegisterAward.begin(); it != _RegisterAward.end(); ++ it)
			{
				m_Package->ItemNotify(it->id, it->count);
			}
			_RegisterAward.clear();
            SetVar(VAR_AWARD_NEWREGISTER, 2);
        }
        else{
            if(GetVar(VAR_AWARD_NEWREGISTER))
                return;
            UInt8 idx = 0;
            if( 0 == (idx = GameAction()->RunNewRegisterAward(this)) )
                return;
            sendNewRegisterAward(idx);
            SetVar(VAR_AWARD_NEWREGISTER, 1);
        }
    }

    void Player::sendNewRegisterAward(UInt8 idx)
    {
        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(6);
        st << idx;
        st << Stream::eos;
        send(st);
    }

    void Player::getAwardFromAD()
    {
        if(GetVar(VAR_AWARD_NEWREGISTER))
            return;
        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(7);
        UInt8 succ = GameAction()->RunNewRegisterAwardAD_RF(this, 1);
        if(succ)
            SetVar(VAR_AWARD_NEWREGISTER, 3);
        st << succ << Stream::eos;
        send(st);
    }

    void Player::getAwardFromRF()
    {
        if(GetVar(VAR_AWARD_NEWREGISTER))
            return;
        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(8);
        UInt8 succ = GameAction()->RunNewRegisterAwardAD_RF(this, 2);
        if(succ)
            SetVar(VAR_AWARD_NEWREGISTER, 4);
        st << succ << Stream::eos;
        send(st);
    }

    void Player::getAwardGiftCard()
    {
        if(GetVar(VAR_AWARD_NEWREGISTER))
            return;
        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(9);
        UInt8 succ = GameAction()->RunNewRegisterAwardAD_RF(this, 3);
        if(succ)
            SetVar(VAR_AWARD_NEWREGISTER, 5);
        st << succ << Stream::eos;
        send(st);
    }

    void Player::getAwardBirthday(UInt8 opt)
    {
    #define NUM 3
        if(opt != 0 && opt != 1 && opt != 2)
            return;
        UInt32 day = 1;
        UInt32 mon = 1;
        UInt32 year = 2012;
        TimeUtil::GetDMY(&day, &mon, &year);
        if(year != 2012 || mon != 9 || day < 16 || day > 30)
            return;
        UInt8 num = GetVar(VAR_AWARD_BIRTHDAY);
        if(opt == 2){    //邀请好友成功
            if(num >= NUM) return;
            Stream st(REP::GETAWARD);
            st << static_cast<UInt8>(10) << static_cast<UInt8>(0);
            st << static_cast<UInt8>(2 - num) << static_cast<UInt8>(1)<< Stream::eos;
            send(st);
            SetVar(VAR_AWARD_BIRTHDAY, num + NUM);
        }
        else if(opt == 1){  //领奖
			std::vector<GData::LootResult>::iterator it;
			for(it = _BirthdayAward.begin(); it != _BirthdayAward.end(); ++ it)
			{
				m_Package->ItemNotify(it->id, it->count);
			}
			_BirthdayAward.clear();
            OnShuoShuo(SS_SLLP);
        }
        else if(opt == 0){  //抽奖
            UInt8 flag = 0;
            if(num >= NUM){ //已邀请过好友
                num -= NUM;
                if(num < 0 || num >= NUM)
                    num = 2;
                flag = 1;
                if(num >= 2) return;
            }
            else{
                if(num >= 1) //未邀请过好友
                    return;
            }
            UInt8 id = GameAction()->RunBirthdayAward(this);
            if(!id) return;
            {
                Stream st(REP::GETAWARD);
                st << static_cast<UInt8>(10) << id << Stream::eos;
                send(st);
            }
            SetVar(VAR_AWARD_BIRTHDAY, flag ? (num + 1 + NUM) : (num + 1));
            {
                Stream st(REP::GETAWARD);
                st << static_cast<UInt8>(10) << static_cast<UInt8>(0);
                st << static_cast<UInt8>(flag ? (2 - num - 1) : 0) << flag << Stream::eos;
                send(st);
            }
        }
    #undef NUM
    }
    void Player::CheckCanAwardBirthday()
    {
        UInt32 day = 1;
        UInt32 mon = 1;
        UInt32 year = 2012;
        TimeUtil::GetDMY(&day, &mon, &year);
        if(year == 2012 && mon == 9 && day >= 16 && day <= 30){
            //生日罗盘许愿星(周年庆活动)
            UInt8 num = GetVar(VAR_AWARD_BIRTHDAY);
            UInt8 flag = 0;
            if(num >= 3){
                num -= 3;
                if(num < 0 || num >= 3)
                    num = 2;
                flag = 1;
            }
            Stream st(REP::GETAWARD);
            st << static_cast<UInt8>(10) << static_cast<UInt8>(0);
            st << static_cast<UInt8>(flag ? (2 - num) : (1 - num)) << flag << Stream::eos;
            send(st);
        }
    }
    
    void Player::getAwardLogin(UInt8 opt)
    {
        if(opt == 1) //领奖
        {
            if(1 != GetVar(VAR_AWARD_LOGIN))
                return;
            //10.14登录抽奖合作与生日罗盘许愿星(周年庆活动)相同的抽奖
			std::vector<GData::LootResult>::iterator it;
			for(it = _BirthdayAward.begin(); it != _BirthdayAward.end(); ++ it)
			{
				m_Package->ItemNotify(it->id, it->count);
			}
			_BirthdayAward.clear();
            SetVar(VAR_AWARD_LOGIN, 2);
        }
        else if(opt == 0) //抽奖
        {
            if(GetVar(VAR_AWARD_LOGIN))
                return;
            UInt8 idx = 0;
            if( 0 == (idx = GameAction()->RunBirthdayAward(this)) )
                return;
            Stream st(REP::GETAWARD);
            st << static_cast<UInt8>(11) << idx << Stream::eos;
            send(st);
            SetVar(VAR_AWARD_LOGIN, 1);
        }
        else if(opt == 2) //告诉客户端可以抽奖
        {
            if(GetVar(VAR_AWARD_LOGIN))
                return;
            if(World::getLoginAward())
            {
                Stream st(REP::GETAWARD);
                st << static_cast<UInt8>(11) << static_cast<UInt8>(0) << Stream::eos;
                send(st);
            }
        }
    }

    void Player::getAwardBlueDiamond(UInt8 opt)
    {
        if(opt >= 1) //抽奖
        {
            UInt8 idx = 0;
            if( 0 == (idx = GameAction()->RunBlueDiamondAward(this, opt)) )
                return;
            Stream st(REP::GETAWARD);
            st << static_cast<UInt8>(12) << idx << Stream::eos;
            send(st);
        }
    }
    void Player::getConsumeAward()
    {
        static int s_items[2][8] ={
            {515,507,509,503,1325,47,134,5026},
            {515,507,509,503,1325,47,1528,5026}
            };
        if (!World::getConsumeAwardAct())
            return;
        if (!is3366AndBD())
            return;
        if (int(GetVar(VAR_CONSUME)/500) - GetVar(VAR_CONSUME_AWARD_COUNT) > 0)
        {
            UInt8 opt = 2; //1:普通用户 2:3366蓝钻
           // if (atoi(getDomain()) == 11 && isBD())
            UInt8 idx = GameAction()->RunConsumeAward(this, opt);
            if (idx > 0)
            {
                AddVar(VAR_CONSUME_AWARD_COUNT, 1);
                sendConsumeAwardInfo(idx);

                char str[64] = {0};
                sprintf(str, "F_10001_0118_%d", s_items[opt-1][idx-1]);
                udpLog("huodong", str, "", "", "", "", "act");
            }
        }
    }
    void Player::getWeiboAward(UInt8 opt, std::string key)
    {
        if (GetPackage()->GetRestPackageSize() < 2)
        {
            sendMsgCode(0, 1011);
            return;
        }
        UInt8 v = GetVar(VAR_WEIBO_AWARD_GOT);
        if (1 == opt)//微信
        {
            if (key != "27036")
            {
                sendMsgCode(0, 1043);
                return;
            }
            if ((v&0x01) == 0)
            {
                getCoupon(20);
                m_Package->Add(503,1,true);
                m_Package->Add(514,1,true);
                v |= 0x01;
                SetVar(VAR_WEIBO_AWARD_GOT, v);
            }
        }
        if (2 == opt) //微博
        {
            if((v&0x02) == 0)
            {
                getCoupon(10);
                m_Package->Add(134,1,true);
                v |= 0x02;
                SetVar(VAR_WEIBO_AWARD_GOT, v);
            }
        }
        sendWeiboAwardInfo();
    }
    void Player::sendWeiboAwardInfo()
    {
        Stream st(REP::ACTIVITY_REWARD);
        st << static_cast<UInt8>(13) << static_cast<UInt8>(GetVar(VAR_WEIBO_AWARD_GOT)) << Stream::eos;
        send(st);
    }
    void Player::sendConsumeAwardInfo(UInt8 idx)
    {
        if (!World::getConsumeAwardAct())
            return;
        if (!is3366AndBD())
            return;
        UInt8 opt = 2; //1:普通用户 2:3366蓝钻
        //if (atoi(getDomain()) == 11 && isBD())
        int v = int(GetVar(VAR_CONSUME)/500) - GetVar(VAR_CONSUME_AWARD_COUNT);
        if (v < 0)
            v = 0;
        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(16) << opt << idx << v << Stream::eos;
        send(st);
    }

    void Player::getThanksGivingDay(UInt8 opt)
    {
        if(!World::getThanksgiving())
            return;
        if(opt == 0) //免费领取
        {
            if(GetVar(VAR_TGDT) & 0x01)
                return;
            if(GameAction()->RunThanksGivingDayAward(this, 1))
            {
                UInt32 var = GetVar(VAR_TGDT) | 0x01;
                SetVar(VAR_TGDT, var);
                Stream st(REP::GETAWARD);
                st << static_cast<UInt8>(15) << static_cast<UInt8>(4) << Stream::eos;
                send(st);
                udpLog("huodong", "F_10000_15", "", "", "", "", "act");
            }
        }
        if(opt == 1) //付费领取(30仙石)
        {
            if(!hasChecked())
                return;
            if(GetVar(VAR_TGDT) & 0x02)
                return;
			if (getGold() < 30)
			{
				sendMsgCode(0, 1104);
				return;
			}
            if(GameAction()->RunThanksGivingDayAward(this, 2))
            {
                UInt32 var = GetVar(VAR_TGDT) | 0x02;
                SetVar(VAR_TGDT, var);
                ConsumeInfo ci(ThanksGivingDay, 0, 0);
                useGold(30, &ci);
                Stream st(REP::GETAWARD);
                st << static_cast<UInt8>(15) << static_cast<UInt8>(5) << Stream::eos;
                send(st);
            }
        }
        if(opt == 2) //告诉客户端领取情况
        {
            Stream st(REP::GETAWARD);
            st << static_cast<UInt8>(15) << static_cast<UInt8>(GetVar(VAR_TGDT)) << Stream::eos;
            send(st);
        }
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

            GData::LootResult lt;
            lt.id = itemId;
            lt.count = 1;
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
        GData::LootResult lt;
        lt.id = itemId;
        lt.count = num;
        _lastLoot.push_back(lt);
    }

    void Player::RegisterAward(UInt16 itemId, UInt16 num)
    {
        GData::LootResult lt;
        lt.id = itemId;
        lt.count = num;
        _RegisterAward.push_back(lt);
    }

    void Player::BirthdayAward(UInt16 itemId, UInt16 num)
    {
        GData::LootResult lt;
        lt.id = itemId;
        lt.count = num;
        _BirthdayAward.push_back(lt);
    }

    void Player::lastQueqiaoAwardPush(UInt16 itemId, UInt16 num)
    {
        GData::LootResult lt;
        lt.id = itemId;
        lt.count = num;
        _lastQueqiaoAward.push_back(lt);
    }

    void Player::checkLastQueqiaoAward()
    {
        std::vector<GData::LootResult>::iterator it;
        for(it = _lastQueqiaoAward.begin(); it != _lastQueqiaoAward.end(); ++ it)
        {
            m_Package->ItemNotify(it->id, it->count);
        }
        _lastQueqiaoAward.clear();
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

    void Player::getYearActAward(UInt8 type)
    {
        bool ret = false;
        if(GetPackage()->GetRestPackageSize() < 9)
        {
            sendMsgCode(0, 1011);
            return;
        }
        Stream st(REP::COUNTRY_ACT);
        UInt8 subType = 1;
        st << subType;
        st << type;
        if(type == 1)
        {
            if(GetVar(VAR_YEAR_SWORDSMAN))
                return;
            ret = GameAction()->onGetYearActAward(this, type);
            if(ret)
            {
                SetVar(VAR_YEAR_SWORDSMAN, 1);
                st << static_cast<UInt8>(1);
                st << Stream::eos;
                send(st);
                OnShuoShuo(SS_PUBTST_PKG);
                char action[16] = "";
                snprintf (action, 16, "F_%d_%d", 1098, 1);
                udpLog("916", action, "", "", "", "", "act");
            }
        }
        else if(type == 2)
        {
            UInt8 status = GetVar(VAR_YEAR_NOBLE);
            UInt8 newStatus;
            if(atoi(m_domain) == 11)
            {
                if(is3366AndLevel4() && (status & 0x08) == 0)
                {
                    char action[16] = "";
                    snprintf (action, 16, "F_%d_%d", 1098, 5);
                    udpLog("916", action, "", "", "", "", "act");
                    newStatus = status | 0x08;
                }
                else
                    return;
            }
            else if(isBD())
            {
                if((status & 0x01) == 0)
                {
                    newStatus = status | 0x01;
                    char action[16] = "";
                    snprintf (action, 16, "F_%d_%d", 1098, 2);
                    udpLog("916", action, "", "", "", "", "act");
                }
                else
                    return;
            }
            else if(isYD())
            {
                if((status & 0x02) == 0)
                {
                    char action[16] = "";
                    snprintf (action, 16, "F_%d_%d", 1098, 3);
                    udpLog("916", action, "", "", "", "", "act");
                    newStatus = status | 0x02;
                }
                else
                    return;
            }
            else if(isQQVIP())
            {
                if((status & 0x04) == 0)
                {
                    char action[16] = "";
                    snprintf (action, 16, "F_%d_%d", 1098, 4);
                    udpLog("916", action, "", "", "", "", "act");
                    newStatus = status | 0x04;
                }
                else
                    return;
            }
            else
                return;
            ret = GameAction()->onGetYearActAward(this, type);
            if(ret)
            {
                SetVar(VAR_YEAR_NOBLE, newStatus);
                st << static_cast<UInt8>(1);
                st << Stream::eos;
                send(st);
            }
        }
    }

    void Player::getQgameGiftAward()
    {
        if(atoi(m_domain) != 10)
            return;
        if(GetLev() < 40)
            return;
        if(GetVar(VAR_QGAME_GIFT) == 0)
        {

            MailPackage::MailItem item[5] = {{512, 1}, {49, 1}, {50, 1}, {548, 1}, {551, 1}};
            sendMailItem(2382, 2383, item, 5);
            SetVar(VAR_QGAME_GIFT, 1);

        }
    }

    void Player::sendYearActInfo()
    {
        Stream st(REP::COUNTRY_ACT);
        Stream st2(REP::COUNTRY_ACT);
        UInt8 type;
        UInt8 result;

        UInt8 opt = 1;
        st << opt;
        st2 << opt;

        type = 1;
        st << type;
        if(GetVar(VAR_YEAR_SWORDSMAN) > 0)
            result = 3;
        else
            result = 2;
        st << result;
        st << Stream::eos;
        send(st);

        type = 2;
        st2 << type;
#if 0
        if(GetVar(VAR_YEAR_NOBLE) > 0)
            result = 3;
        else
        {
            if(isBD() || isYD() || isQQVIP() || is3366AndLevel4())
                result = 2;
            else
                result = 4;
        }
#else
        /*
         * 0x01:蓝钻(不考虑3366)
         * 0x02:黄钻
         * 0x04:QQ会员
         * 0x08:3366且大于等于4级
         */
        UInt8 status = GetVar(VAR_YEAR_NOBLE);
        if(atoi(m_domain) == 11)
        {
            if(is3366AndLevel4())
            {
                if(status & 0x08)
                    result = 3;
                else
                    result = 2;
            }
            else
                result = 4;
        }
        else if(isBD())
        {
            if(status & 0x01)
                result = 3;
            else
                result = 2;
        }
        else if(isYD())
        {
            if(status & 0x02)
                result = 3;
            else
                result = 2;
        }
        else if(isQQVIP())
        {
            if(status & 0x04)
                result = 3;
            else
                result = 2;
        }
        else
            result = 4;
#endif
        st2 << result;
        st2 << Stream::eos;
        send(st2);
    }

    void Player::lastKillMonsterAwardPush(UInt16 itemId, UInt16 num)
    {
        GData::LootResult lt;
        lt.id = itemId;
        lt.count = num;
        _lastKillMonsterAward.push_back(lt);
    }

    void Player::lastExJobAwardPush(UInt16 itemId, UInt16 num)
    {
        GData::LootResult lt;
        lt.id = itemId;
        lt.count = num;
        _lastExJobAward.push_back(lt);
    }

    void Player::checkLastExJobAward()
    {
        std::vector<GData::LootResult>::iterator it;
        for(it = _lastExJobAward.begin(); it != _lastExJobAward.end(); ++ it)
        {
            m_Package->ItemNotify(it->id, it->count);
        }
        _lastExJobAward.clear();
    }

    void Player::lastExJobStepAwardPush(UInt16 itemId, UInt16 num)
    {
        GData::LootResult lt;
        lt.id = itemId;
        lt.count = num;
        _lastExJobStepAward.push_back(lt);
    }

    void Player::checkLastExJobStepAward()
    {
        std::vector<GData::LootResult>::iterator it;
        for(it = _lastExJobStepAward.begin(); it != _lastExJobStepAward.end(); ++ it)
        {
            m_Package->ItemNotify(it->id, it->count);
        }
        _lastExJobStepAward.clear();
    }

    void Player::lastNew7DayTargetAwardPush(UInt16 itemId, UInt16 num)
    {
        GData::LootResult lt;
        lt.id = itemId;
        lt.count = num;
        _lastNew7DayTargetAward.push_back(lt);
    }

    void Player::checkLastKillMonsterAward()
    {
        std::vector<GData::LootResult>::iterator it;
        for(it = _lastKillMonsterAward.begin(); it != _lastKillMonsterAward.end(); ++ it)
        {
            m_Package->ItemNotify(it->id, it->count);
        }
        _lastKillMonsterAward.clear();
    }

    void Player::checkLastNew7DayTargetAward()
    {
        std::vector<GData::LootResult>::iterator it;
        for(it = _lastNew7DayTargetAward.begin(); it != _lastNew7DayTargetAward.end(); ++ it)
        {
            m_Package->ItemNotify(it->id, it->count);
        }
        _lastNew7DayTargetAward.clear();
    }

    void Player::sendNewRC7DayInfo(UInt8 type /* = 0 */)
    {
        // 发送新注册七日活动页面信息
        switch (type)
        {
            case 0:
                sendNewRC7DayLogin();
                sendNewRC7DayRecharge();
                sendNewRC7DayTarget();
                break;
            case 1:
                sendNewRC7DayLogin();
                break;
            case 2:
                sendNewRC7DayRecharge();
                break;
            case 3:
                sendNewRC7DayTarget();
                break;
            default:
                break;
        }
    }

    void Player::sendNewRC7DayLogin()
    {
        // 发送新注册七日活动登录奖励页面信息
        Stream st(REP::NEWRC7DAY);
        UInt32 loginAward = GetVar(VAR_CTSLANDING_AWARD);
        UInt32 totalLoginAward = GetVar(VAR_CTSLANDING_AWARD2);
        st << static_cast<UInt8> (1);
        st << static_cast<UInt8> (loginAward);
        st << static_cast<UInt8> (totalLoginAward);
        st << Stream::eos;
        send(st);

        GameMsgHdr hdr1(0x266, getThreadId(), this, 0);
        GLOBAL().PushMsg(hdr1, NULL);
    }

    void Player::sendNewRC7DayRecharge()
    {
        // 发送新注册七日活动充值奖励页面信息
        Stream st(REP::NEWRC7DAY);
        st << static_cast<UInt8> (2);
        UInt32 totalRecharge = GetVar(VAR_RC7DAYRECHARGE);
        UInt32 rechargeAward = GetVar(VAR_RC7DAYWILL);
        UInt8 wishIndex = rechargeAward & 0xff;
        UInt8 wishType = (rechargeAward >> 8) & 0xff;
        st << static_cast<UInt8>(wishIndex);
        st << static_cast<UInt8>(wishType);
        st << static_cast<UInt32>(totalRecharge);
        st << Stream::eos;
        send(st);
    }

    void Player::sendNewRC7DayTarget(UInt8 idx /* = 0 */)
    {
        // 发送新注册七日活动每日目标页面信息
        Stream st(REP::NEWRC7DAY);
        UInt32 awardIndex = GetVar(VAR_CLAWARD2);
        UInt32 count = GetVar(VAR_CTS_TARGET_COUNT);
        st << static_cast<UInt8> (3);
        st << static_cast<UInt16>(awardIndex); // 抽奖情况bit表
        st << static_cast<UInt8>(count);  // 剩余抽奖次数
        if (idx)
            st << static_cast<UInt8>(idx);
        st << Stream::eos;
        send(st);
    }

    void Player::getNewRC7DayLoginAward(UInt8 val, UInt8 off)
    {
        // 申请领取新注册七天登录奖励 (包括补签和累计登录）
        UInt32 ctslanding = GetVar(VAR_CTSLANDING);
        UInt32 ctslandingAward = GetVar(VAR_CTSLANDING_AWARD);
        UInt32 ctslandingAward2 = GetVar(VAR_CTSLANDING_AWARD2);
        UInt32 cts = 0;  // 累计登录
        UInt32 ctsAwd = 0;
        for (int i = off; i >= 0; --i)
        {
            if (ctslanding & (1<<i))
                ++cts;
            if (ctslandingAward & (1 << i))
                ++ctsAwd;
        }
        if (val == 0)
            return;
        if (val <= 7)
        {
            // 领取签到登录奖励
            if (val < off + 1)
            {
                // 补签，需要消耗10仙石
                if (ctslandingAward & (1<<(val - 1)))
                    return;
                if (!GameAction()->RunNewRC7DayLoginAward(this, val))
                    return;
                ConsumeInfo ci(RC7DayLogin, 0, 0);
                useGold(10, &ci);

                ctslanding |= (1<<(val - 1));
                SetVar(VAR_CTSLANDING, ctslanding);

                ctslandingAward |= (1<<(val - 1));
                SetVar(VAR_CTSLANDING_AWARD, ctslandingAward);
                sendNewRC7DayLogin();
                newRC7DayUdpLog(1143, val);
            }
            else if (val == off + 1)
            {
                // 正常签到登录奖励
                if (ctslandingAward & (1<<(val - 1)))
                    return;

                if (!GameAction()->RunNewRC7DayLoginAward(this, val))
                    return;

                ctslandingAward |= (1<<(val - 1));
                SetVar(VAR_CTSLANDING_AWARD, ctslandingAward);
                sendNewRC7DayLogin();
                newRC7DayUdpLog(1142, val);
            }
            else
            {
                // 穿越签到，不予理会
            }
        }
        else if (val <= 11)
        {
            // 领取累计签到
            val -= 7;
            if (val <= ctsAwd)
            {
                if (1<<(val - 1) & ctslandingAward2)
                {
                    // 已经领取过了
                    sendMsgCode(0, 1340);
                }
                else
                {
                    // 正常领取
                    if (!GameAction()->RunNewRC7DayLoginAward2(this, val))
                        return;

                    ctslandingAward2 |= 1 << (val - 1);
                    SetVar(VAR_CTSLANDING_AWARD2, ctslandingAward2);
                    sendNewRC7DayLogin();
                    newRC7DayUdpLog(1144 + val - 1);
                }
            }
            else
            {
                // 穿越领取累计登录奖励，不予理会
            }
        }
    }

    void Player::getNewRC7DayRechargeAward(UInt8 val)
    {
        // 申请领取新注册七天充值奖励 (神龙许愿)
        // val 1-7 七龙珠奖励bit图（1为点亮，0为未点亮）， 
        //     8-11 声望，荣誉，修为，经验
        UInt32 totalRecharge = GetVar(VAR_RC7DAYRECHARGE);
        UInt32 rechargeAward = GetVar(VAR_RC7DAYWILL);
        UInt8 wishIndex = rechargeAward & 0xff;
        UInt8 wishType = (rechargeAward >> 8) & 0xff;

        if (val == 0 || val > 11)
            return;

        if (val <= 7)
        {
            // 点亮龙珠
            if (!(wishIndex & (0x01 << (val - 1))))
            {
                if (GameAction()->RunNewRC7DayRechargeAward(this, val, totalRecharge))
                {
                    wishIndex |= 0x01 << (val - 1);
                    SetVar(VAR_RC7DAYWILL, static_cast<UInt32>(wishType) << 8 | wishIndex);
                    newRC7DayUdpLog(1154, val);
                }
            }
        }
        else if (val <= 11)
        {
            // 神龙许愿
            if (wishIndex == 127 && !wishType)
            {
                if (GameAction()->onRC7DayWill(this, val - 7))
                {
                    wishType = val - 7;
                    SetVar(VAR_RC7DAYWILL, static_cast<UInt32>(wishType) << 8 | wishIndex);
                    newRC7DayUdpLog(1148 + val - 8);
                }
            }
        }

        sendNewRC7DayRecharge();
    }
    
    void Player::getNewRC7DayTargetAward(UInt8 val)
    {
        // 申请领取新注册七天每日目标奖励 
        // val 0:转盘开始 1:转盘结束
        UInt8 idx = 0;
        switch (val)
        {
            case 0:
                {
                    if (!_lastNew7DayTargetAward.empty()) // 转盘正在转
                        return;
                    UInt8 count = GetVar(VAR_CTS_TARGET_COUNT);
                    if (count)
                    {
                        idx = GameAction()->RunNewRC7DayTargetAward(this);
                        if (idx)
                        {
                            --count;
                            SetVar(VAR_CTS_TARGET_COUNT, count);
                            newRC7DayUdpLog(1153);
                        }
                    }
                }
                break;
            case 1:
                {
                    std::vector<GData::LootResult>::iterator it;
                    for(it = _lastNew7DayTargetAward.begin(); it != _lastNew7DayTargetAward.end(); ++ it)
                    {
                        m_Package->ItemNotify(it->id, it->count);
                    }
                    _lastNew7DayTargetAward.clear();
                }
                break;
            default:
                break;
        }
        sendNewRC7DayTarget(idx);
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

    void Player::sendOldRC7DayAward()
    {
        // 老版本转换成新版本的注册七日签到补偿
        MailPackage::MailItem item1[3] = {{MailPackage::Coupon,20},{56, 2}, {57, 2}};
        MailPackage::MailItem item2[4] = {{MailPackage::Coupon,20},{502, 5}, {503, 2},{15,5}};
        MailPackage::MailItem item3[4] = {{MailPackage::Coupon,30},{500, 5}, {501, 2},{15,5}};
        MailPackage::MailItem item4[4] = {{MailPackage::Coupon,30},{509, 1}, {507, 1},{511,3}};
        MailPackage::MailItem item5[5] = {{MailPackage::Coupon,40},{509, 1}, {507, 1},{5025,1},{512,4}};
        MailPackage::MailItem item6[5] = {{MailPackage::Coupon,40},{509, 1}, {507, 1},{514,5},{515,2}};
        MailPackage::MailItem item7[6] = {{MailPackage::Coupon,50},{509, 1}, {507, 1},{517,5},{134,2},{15,5}};
        UInt16 size[7] = {3,4,4,4,5,5,6};

        MailPackage::MailItem* item[7] = {item1,item2,item3,item4,item5,item6,item7};


        UInt32 now = TimeUtil::Now();
        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 created_sharp = TimeUtil::SharpDay(0, getCreated());
        if (created_sharp > now_sharp)
            return; // 创建时间错误（穿越了）

        if (now_sharp - created_sharp > 7 * 24*60*60)
            return; // 玩家注册时间超过7日，无法参与活动

        UInt32 off = CREATE_OFFSET(created_sharp, now_sharp);
        if (off >= 7)
            return; // 玩家注册时间超过7日，无法参与活动

        UInt32 ctslanding = GetVar(VAR_CTSLANDING);
        UInt32 ctslandingAward = GetVar(VAR_CTSLANDING_AWARD);
        for (UInt8 i = 0; i < off; ++i)
        {
            if (ctslanding & (0x01<<i))
            {
                SYSMSG(title, 2207);
                SYSMSGV(content, 2208, i + 1);
                Mail * mail = GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFD0000/*free*/);
                if (mail)
                {
                    mailPackageManager.push(mail->id, item[i], size[i], true);

                    std::string strItems;
                    for (int j = 0; j < size[i]; ++j)
                    {
                        strItems += Itoa(item[i][j].id);
                        strItems += ",";
                        strItems += Itoa(item[i][j].count);
                        strItems += "|";
                    }

                    DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, BuChangNewRC7Day, title, content, strItems.c_str(), mail->recvTime);

                    ctslandingAward |= (1<<i);
                    SetVar(VAR_CTSLANDING_AWARD, ctslandingAward);
                }
            }
            if (GetLev() >= 47)
            {
                UInt32 targetVal = GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_LEVEL))
                {
                    targetVal |= TARGET_LEVEL;
                    AddVar(VAR_CTS_TARGET_COUNT, 1);
                    SetVar(VAR_CLAWARD2, targetVal);
                    //newRC7DayUdpLog(1152, 1);
                }
            }
        }

    }
    /*
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
    */
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

    float Player::getClanStatueHPEffect()
    {
        if (_clan == NULL)
            return 0;
        UInt16 level = _clan->getStatueLevel();
        return static_cast<float>(GData::clanStatueTable[level].exHp);
    }

    float Player::getClanStatueAtkEffect()
    {
        if (_clan == NULL)
            return 0;
        UInt16 level = _clan->getStatueLevel();
        return static_cast<float>(GData::clanStatueTable[level].exAttack);
    }

    float Player::getClanStatueDefendEffect()
    {
        if (_clan == NULL)
            return 0;
        UInt16 level = _clan->getStatueLevel();
        return static_cast<float>(GData::clanStatueTable[level].exDefend);
    }

    float Player::getClanStatueMagAtkEffect()
    {
        if (_clan == NULL)
            return 0;
        UInt16 level = _clan->getStatueLevel();
        return static_cast<float>(GData::clanStatueTable[level].exMagAtk);
    }

    float Player::getClanStatueMagDefentEffect()
    {
        if (_clan == NULL)
            return 0;
        UInt16 level = _clan->getStatueLevel();
        return static_cast<float>(GData::clanStatueTable[level].exMagDef);
    }

    float Player::getClanStatueActionEffect()
    {
        if (_clan == NULL)
            return 0;
        UInt16 level = _clan->getStatueLevel();
        return static_cast<float>(GData::clanStatueTable[level].exAction);
    }

    float Player::getClanStatueHitrLvlEffect()
    {
        if (_clan == NULL)
            return 0;
        UInt16 level = _clan->getStatueLevel();
        return static_cast<float>(GData::clanStatueTable[level].exHitRate);
    }

    void  Player::AddStatueExp(UInt32 exp)
    {
        if (_clan == NULL)
            return;
        _clan->addStatueExp(exp);
    }

    void Player::SubStatueExp(UInt32 exp)
    {
        if (_clan == NULL)
            return;
        _clan->subStatueExp(exp);
    }


    void Player::onBlueactiveday()
    {
        // XXX: 原来是为蓝钻准备的，现在全平台也要了
        //if (!(atoi(m_domain) == 11 || atoi(m_domain) == 10))
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

    void Player::sendPExpCard(int pos)
    {
        if (!pos || pos > 1)
            return;
        MailPackage::MailItem item[1] =
        {
            {9089, 1},
        };
        sendMailItem(3002, 3002, &item[pos-1], 1, false);
    }

    void Player::sendRechargeRankAward(int pos)
    {
        if (!pos || pos > 7)
            return;
#if 0
        MailPackage::MailItem item[7][3] =
        {
            {{9076,30},{509,30},{9177,10},},
            {{9076,20},{509,10},{9177,5},},
            {{9076,10},{509,10},{9177,2},},
            {{9076,5},{509,5},{9177,1},},
            {{9076,5},{509,5},{9177,1},},
            {{9076,5},{509,5},{9177,1},},
            {{9076,5},{509,5},{9177,1},},
        };

        SYSMSGV(_title, 4026, pos);
        SYSMSGV(_content, 4027, pos);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, _title, _content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem* mitem = &item[pos-1][0];
            UInt32 size = 3;
            std::string strItems;
            for (UInt32 i = 0; i < size; ++i)
            {
                strItems += Itoa(mitem[i].id);
                strItems += ",";
                strItems += Itoa(mitem[i].count);
                strItems += "|";
            }
            mailPackageManager.push(mail->id, mitem, size, true);
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, getId(), mail->id, VipAward, _title, _content, strItems.c_str(), mail->recvTime);
        }
#else
        GameAction()->sendRechargeRankAward(this, pos);
#endif
    }

    void Player::sendConsumeRankAward(int pos)
    {
        if (!pos || pos > 7)
            return;
#if 0
        MailPackage::MailItem item[1][1] =
        {
            {{444, 1},},
        };
        sendMailItem(2331, 2332, &item[pos-1][0], 1, false);
#else
        GameAction()->sendConsumeRankAward(this, pos);
#endif
    }

    void Player::sendPopularityRandAward(int popularity)
    {
        // TODO: 发送称号卡
    }

    void Player::sendKillMonsterRankAward(UInt8 index, Int32 pos)
    {
        if (index > 3 || !pos || pos > 1)
            return;
        MailPackage::MailItem item[5][1] =
        {
            {{1329, 1},},
            {{1330, 1},},
            {{1331, 1},},
            {{1332, 1},},
        };
        sendMailItem(2372, 2373, &item[index][0], 1, false);
    }

    void Player::sendCreateMail()
    {
#ifdef _FB
#else
        /** deleted by qiwy 2012-12-07 **/
#if 0
        SYSMSG(title, 2335);
        SYSMSG(content, 2336);
        GetMailBox()->newMail(NULL, 0x12, title, content);

        MailPackage::MailItem item[5] = {{9161, 1}, {9162, 1}, {9164, 1}, {9165, 1}, {9166, 1}};
        sendMailItem(4028, 4028, item, 5);
#endif
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
            std::string name = "";
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
            if(GetFreePackageSize() > m_dpData->itemNum/99)
            {
                struct AddItemInfo
                {
                    UInt32 id;
                    UInt16 num;
                    UInt8 bind;
                    UInt8 fromWhere;
                };

                AddItemInfo item;
                item.id = m_dpData->itemId;
                item.num = m_dpData->itemNum;
                item.bind = true;
                item.fromWhere = FromTownDeamon;
                GameMsgHdr hdr1(0x259, getThreadId(), this, sizeof(AddItemInfo));
                GLOBAL().PushMsg(hdr1, &item);

                m_dpData->itemId = 0;
                m_dpData->itemNum = 0;
                m_dpData->quitLevel = 0;
                m_dpData->attacker = NULL;
                DB3().PushUpdateData("UPDATE `towndeamon_player` SET `itemId`=0, `itemNum`=0, `quitLevel`=0, `attacker`=0 WHERE `playerId` = %"I64_FMT"u", getId());
            }
            else
                sendMsgCode(2, 1011);
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
        UInt8 yalvl = lvl;
        if (lvl > 99)
            yalvl = 99;
        UInt64 exp = (offline/60)*((lvl-10)*(yalvl/10)*5+25)*0.8f;
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
                {
                    UInt32 pExp = pexp*fgt->getPExpNoBuf()*0.8f;
                    fgt->addPExp(pExp);
                }
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
            {503,1325,134,516},
            {8000,551,517,500},
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
            GetPackage()->Add(ids[0], 6, true, false);
        }
        else
        {
            for (UInt8 i = 0; i < 3; ++i)
            {
                GetPackage()->Add(ids[i], 1, true, false);
            }
        }

        Stream st(REP::TOKEN);
        st << static_cast<UInt8>(1) << ids[0] << ids[1] << ids[2] << Stream::eos;
        send(st);

        SetVar(VAR_GOLD_TOKEN+type, GetVar(VAR_GOLD_TOKEN+type)-1);
        sendTokenInfo();

        if (type == 0 && !same)
            SYSMSG_BROADCASTV(4060, _playerData.country, _playerData.name.c_str(), ids[0], ids[1], ids[2]);
        if (type == 0 && same)
            SYSMSG_BROADCASTV(4061, _playerData.country, _playerData.name.c_str(), ids[0]);
    }

    void Player::sendTokenInfo()
    {
        Stream st(REP::TOKEN);
        st << static_cast<UInt8>(0) << GetVar(VAR_GOLD_TOKEN) << GetVar(VAR_TAEL_TOKEN) << GetVar(VAR_COIN_TOKEN) << Stream::eos;
        send(st);
    }

    void Player::sendDiscountLimit()
    {
        // 发送给客户端的有关限购的相关数据
        Stream st(REP::STORE_DISLIMIT);
        for (UInt8 type = 4; type < 7; ++type)
        {
            // 发送活动限购三栏的种类，时间和剩余数量
            UInt32 max = GData::store.getDiscountLimit(type);
            UInt8 offset = GData::store.getDisTypeVarOffset(type);
            UInt32 used = GetVar(GObject::VAR_DISCOUNT_1+offset);

            if (offset == 0xff)
            {
                st << static_cast<UInt8>(type);
                st << static_cast<UInt8>(0);
                st << static_cast<UInt32>(0);
                st << static_cast<UInt32>(0);
                st << static_cast<UInt32>(0);
                continue;
            }


            UInt32 time = GetVar(GObject::VAR_DISCOUNT_SP_1_TIME + type - 4);
            UInt32 now  = TimeUtil::Now();
            if (time < now)
            {
                // 还没购买过该限购
                SetVar(GObject::VAR_DISCOUNT_1+offset, 0);
                time = GData::store.getEndTimeByDiscountType(type);
                SetVar(GObject::VAR_DISCOUNT_SP_1_TIME + type - 4, time);
                used = 0;
            }

            if (used >= max)
            {
                // 购买已经超过限购次数
                used = max;
                SetVar(GObject::VAR_DISCOUNT_1+offset, used);
            }

            UInt32 count = 0; // 限购剩余的购买次数

            // 检查开始限购开始时间
            time = GData::store.getBeginTimeByDiscountType(type);
            if (time > now)
            {
                // 活动限购还未开始
                SetVar(GObject::VAR_DISCOUNT_SP_1_TIME + type - 4, 0);
                st << static_cast<UInt8>(type);
                st << static_cast<UInt8>(0);
                st << static_cast<UInt32>(0);
                st << static_cast<UInt32>(0);
                st << static_cast<UInt32>(0);
                continue;
            }

            // 检查结束时间
            time = GData::store.getEndTimeByDiscountType(type);
            if (time < now)
            {
                // 活动限购已经结束
                SetVar(GObject::VAR_DISCOUNT_SP_1_TIME + type - 4, 0);
                st << static_cast<UInt8>(type);
                st << static_cast<UInt8>(0);
                st << static_cast<UInt32>(0);
                st << static_cast<UInt32>(0);
                st << static_cast<UInt32>(0);
                continue;
            }

            UInt8 exType = 0;
            UInt32 exValue = 0;

            exType = GData::store.getExDiscount(type, exValue);
            UInt32 goldVal = 0;
            switch(exType)
            {
                case 1:
                    if (GetVar(VAR_DISCOUNT_EX1_TIME + type - 4) < now)
                    {
                        SetVar(VAR_DISCOUNT_EX1_TIME + type - 4, time);
                        SetVar(VAR_DISCOUNT_CONSUME1 + type - 4, 0);
                    }
                    goldVal = GetVar(VAR_DISCOUNT_CONSUME1 + type - 4);
                    break;
                case 2:
                    if (GetVar(VAR_DISCOUNT_EX1_TIME + type - 4) < now)
                    {
                        SetVar(VAR_DISCOUNT_EX1_TIME + type - 4, time);
                        SetVar(VAR_DISCOUNT_RECHARGE1 + type - 4, 0);
                    }
                    goldVal = GetVar(VAR_DISCOUNT_RECHARGE1 + type - 4);
                    break;
                default:
                    break;
            }

            if (exValue && exValue >= goldVal)
                goldVal = exValue - goldVal;
            else
                goldVal = 0;

            time -= now;    // 正在限购活动中, 计算剩余时间
            count = max - used;

            st << static_cast<UInt8>(type);
            st << static_cast<UInt8>(exType);
            st << static_cast<UInt32>(goldVal);
            st << static_cast<UInt32>(time); // 限购剩余时间 （0表示没有限购或者还未开始）
            st << static_cast<UInt32>(count);
        }
        st << Stream::eos;
        send(st);

        // 再发送一个完整的三五八折协议
        Stream st2(REP::STORE_DISLIMIT);
        for (UInt8 type = 7; type < 10; ++type)
        {
            UInt8 offset = GData::store.getDisTypeVarOffset(type);
            UInt32 max = GData::store.getDiscountLimit(type);
            if (offset == 0xff)
            {
                st2 << static_cast<UInt8>(type);
                st2 << static_cast<UInt8>(0);
                st2 << static_cast<UInt32>(0);
                st2 << static_cast<UInt32>(0);
                st2 << static_cast<UInt32>(0);
                continue;
            }
            UInt32 used = GetVar(GObject::VAR_DISCOUNT_1+offset);
            if (used >= max)
            {
                used = max;
                SetVar(GObject::VAR_DISCOUNT_1+offset, used);
            }
            UInt32 count = max - used;

            UInt32 time = GData::store.getEndTimeByDiscountType(type);
            UInt32 now = TimeUtil::Now();
            if (time < now)
                time = 0;
            else
                time -= now;

            st2 << static_cast<UInt8>(type);
            st2 << static_cast<UInt8>(0);
            st2 << static_cast<UInt32>(0);
            st2 << static_cast<UInt32>(time);
            st2 << static_cast<UInt32>(count);
        }
        st2 << Stream::eos;
        send(st2);
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
    UInt8 Player::getRPLoginDay()
    {
        UInt32 now = TimeUtil::Now();
        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 created_sharp = TimeUtil::SharpDay(0, getCreated());
        if (created_sharp > now_sharp)
            return 0;

        if (now_sharp - created_sharp > 7 * DAY_SECS)
            return 0;

        UInt32 ctslanding = GetVar(VAR_CTSLANDING);
        UInt32 off = CREATE_OFFSET(created_sharp, now_sharp);
        if (off >= 7)
            return 0;
        UInt32 cts = 0;
        for (int i = off; i >= 0; --i)
        {
            if (ctslanding & (1<<i))
                ++cts;
            else
                break;
        }

        return cts;
    }
    void Player::sendYearRPInfo()
    {
        UInt8 cts = getRPLoginDay();
        if (cts == 0)
            return;

        UInt32 rpValue = GetVar(VAR_RP_VALUE);
        UInt8 packageType = rpValue;
        if (packageType > 0)
        {
            UInt8 packageGot = rpValue >> 8;
            UInt8 rewardGot = GetVar(VAR_RP_REWARD_GOT);

            Stream st(REP::RC7DAY);
            st << static_cast<UInt8>(8);
            st << static_cast<UInt8>(0);
            st << packageType << packageGot << cts << rewardGot;
            st << Stream::eos;
            send(st);
        }
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
    const int g_rpCoupon[3] = {365,365,365};
    const int g_rpServerCoupon[3] = {88,188,365};
    const int g_rp1Items[][2] = {
    {15, 20}, {510, 10}, {56, 10}, {57, 10}, {502, 100},
    {514, 10}, {508, 10}, {506, 10},{1700, 1},{0,0}};
    const int g_rp2Items[][2] = {
    {15, 30}, {56, 20}, {57, 20}, {502, 100}, {503,20},
    {515, 10}, {508, 10}, {506, 10},{1700, 1},{0,0}};
    const int g_rp3Items[][2] = {
    {15, 50}, {515, 20}, {509, 20}, {507, 20},
    {1700, 1}, {1701, 1}, {1704, 1}, {1705,1},{0,0}};
    const int g_rpRewardItems[][2] = {{15,1},{502,2},{509,1}};

    const int g_rpServer1Items[][2] = {
    {15, 20}, {510, 10}, {56, 10}, {57, 10}, {502, 100},
    {514, 10}, {508, 10}, {506, 10},{0,0}};
    const int g_rpServer2Items[][2] = {
    {15, 30}, {56, 20}, {57, 20}, {502, 100}, {503,20},
    {508, 10}, {506, 10},{1700, 1},{0,0}};
    const int g_rpServer3Items[][2] = {
    {15, 50}, {515, 20}, {509, 20}, {507, 20},{1325,20},
    {134,20}, {1700, 1}, {1701, 1}, {0,0}};

    void Player::getYearRPPackage()
    {
        UInt32 rpValue = GetVar(VAR_RP_VALUE);
        UInt8 packageType = rpValue;
        if (packageType > 0)
        {
            UInt8 packageGot = rpValue >> 8;
            if (packageGot == 1)
            {
                Stream st(REP::RF7DAY);
                st << static_cast<UInt8>(8);
                st << static_cast<UInt8>(1);
                st << Stream::eos;
                send(st);
                return;
            }
            const int (*pItems)[2];
            if (1 == packageType)
            {
                if (cfg.rpServer)
                {
                    getCoupon(g_rpServerCoupon[0]);
                    pItems = g_rpServer1Items;
                }
                else
                {
                    getCoupon(g_rpCoupon[0]);
                    pItems = g_rp1Items;
                }
                for (UInt8 i = 0; pItems[i][0] != 0; ++i)
                    GetPackage()->Add(pItems[i][0], pItems[i][1], true);

                udpLog("rpPacket", "F_1100_1", "", "", "", "", "act");
            }
            else if (2 == packageType)
            {
                if (cfg.rpServer)
                {
                    getCoupon(g_rpServerCoupon[1]);
                    pItems = g_rpServer2Items;
                }
                else
                {
                    getCoupon(g_rpCoupon[1]);
                    pItems = g_rp2Items;

                }
                for (UInt8 i = 0; pItems[i][0] != 0; ++i)
                    GetPackage()->Add(pItems[i][0], pItems[i][1], true);

                udpLog("rpPacket", "F_1100_2", "", "", "", "", "act");
            }
            else if (3 == packageType)
            {
                if (cfg.rpServer)
                {
                    getCoupon(g_rpServerCoupon[2]);
                    pItems = g_rpServer3Items;
                }
                else
                {
                    getCoupon(g_rpCoupon[2]);
                    pItems = g_rp3Items;

                }
                for (UInt8 i = 0; pItems[i][0] != 0; ++i)
                    GetPackage()->Add(pItems[i][0], pItems[i][1], true);

                udpLog("rpPacket", "F_1100_3", "", "", "", "", "act");
            }
            rpValue += (0xFF+1);
            SetVar(VAR_RP_VALUE, rpValue);

            sendYearRPInfo();
        }
    }
    void Player::getYearRPReward()
    {
        UInt8 cts = getRPLoginDay();
        if (cts == 0)
            return;

        UInt8 isGot = GetVar(VAR_RP_REWARD_GOT);
        if (isGot == 1)
        {
            Stream st(REP::RF7DAY);
            st << static_cast<UInt8>(8);
            st << static_cast<UInt8>(2);
            st << Stream::eos;
            send(st);
            return;
        }
        cts = cts > 3 ? 3 : cts;
        getCoupon(10*cts);
        for (int i = 0; i < 3; ++i)
        {
           GetPackage()->Add(g_rpRewardItems[i][0], g_rpRewardItems[i][1]*cts, true);
        }
        SetVar(VAR_RP_REWARD_GOT, 1);

        sendYearRPInfo();
    }
    void Player::sendFishUserInfo()
    {
        UInt8 today = (TimeUtil::SharpDay(0, TimeUtil::Now()) - TimeUtil::SharpDay(0, getCreated())) / DAY_SECS + 1;
        if (today > 7)
            return;
        UInt32 rpValue = GetVar(VAR_RP_VALUE);
        if (rpValue != 4)
            return;
        UInt32 v = GetVar(VAR_FISHUSER_AWARD);
        Stream st(REP::RC7DAY);
        st << static_cast<UInt8>(9);
        st << static_cast<UInt8>(today);
        st << static_cast<UInt8>(v);
        st << Stream::eos;
        send(st);
    }
    void Player::getFishUserPackage()
    {
        if (GetLev() < 45)
            return;
        UInt32 rpValue = GetVar(VAR_RP_VALUE);
        if (rpValue != 4)
            return;

        if (TimeUtil::SharpDay(0, TimeUtil::Now()) - TimeUtil::SharpDay(0, getCreated()) > 6 * DAY_SECS)
            return;

        UInt32 v = GetVar(VAR_FISHUSER_AWARD);
        if ((v&0x80) == 0)
        {
            if (GetPackage()->GetRestPackageSize() < 3)
            {
                sendMsgCode(0, 1011);
            }
            else
            {
                GetPackage()->Add(1277,1,true);
                GetPackage()->Add(30,1,true);
                GetPackage()->Add(1325,5,true);
                v |= 0x80;
                SetVar(VAR_FISHUSER_AWARD, v);
            }
        }
        sendFishUserInfo();
    } 
    void Player::getFishUserAward()
    {
        static const MailPackage::MailItem s_items[][3] = {
            {{15,5},{1604,1},{56,2}},
            {{15,5},{503,10},{56,5}},
            {{15,5},{514,10},{56,5}},
            {{15,5},{515,3},{56,5}},
            {{15,5},{1610,1},{56,5}},
            {{15,5},{509,5},{56,10}},
            {{15,5},{226,1},{56,10}}
        };
        UInt8 today = (TimeUtil::SharpDay(0, TimeUtil::Now()) - TimeUtil::SharpDay(0, getCreated())) / DAY_SECS + 1;
        if (today > 7 || today == 0)
            return;
        UInt8 v = GetVar(VAR_FISHUSER_AWARD);
        if ((v&(0x01<<(today-1))) == 0)
        {
            if (GetPackage()->GetRestPackageSize() < 3)
            {
                sendMsgCode(0, 1011);
                return;
            }
            getCoupon(50);
            for (UInt8 i = 0; i < 3; ++i)
            {
                GetPackage()->Add(s_items[today-1][i].id, s_items[today-1][i].count, true);
            }
            v |= (0x01<<(today-1));
            SetVar(VAR_FISHUSER_AWARD, v); 
        }
        sendFishUserInfo();
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

#if 0
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
#endif
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
 //       if (!World::getMayDay() && !World::getCompassAct())
 //           return;
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

    void Player::getMDItem(UInt8 v)
    {
//        if (!World::getMayDay() && !World::getCompassAct())
//            return;

        UInt32 soul = GetVar(VAR_MDSOUL);
        if (!soul)
            return;

        UInt8 type = 0;
        UInt32 subsoul = 0;
        /*if (soul >= 100)
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
        */
        {
            type = 3;
            subsoul = 10;
        }
        if (soul < subsoul)
            return;

        if (!type)
            return;
        if (GetPackage()->IsFull())
        {
            sendMsgCode(0, 1011);
            return;
        }

        UInt32 itemId  = GameAction()->onUseMDSoul(this, type, v);
        if (!itemId)
            return;

        sendMDSoul(1, itemId);

        char str[64] = {0};
        sprintf(str, "F_10000_0118_%d", itemId);
        udpLog("huodong", str, "", "", "", "", "act");

        soul -= subsoul;
        SetVar(VAR_MDSOUL, soul);
    }

    void Player::useMDSoul(UInt8 v)
    {
//        if (!World::getMayDay() && !World::getCompassAct())
//            return;

/*        UInt32 soul = GetVar(VAR_MDSOUL);
        if (!soul)
            return;

        UInt8 type = 0;
        UInt32 subsoul = 0;
        if (soul >= 100)
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
        if (soul < subsoul)
            return;

        if (!type)
            return;

        if (!_mditem)
            return;
*/
        std::vector<MDItem>* itemVec = &_mditemVec1;
        if (v == 2)
            itemVec = &_mditemVec2;
        for(std::vector<MDItem>::iterator it=itemVec->begin(); it != itemVec->end(); ++it)
        {
	        m_Package->ItemNotify(it->id, it->count);
        }
        sendMDSoul(0);
        itemVec->clear();
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
        else if (type == 4)
        {
            st << World::getOpenTime() << Stream::eos;
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
        
        sendNewRC7DayRecharge();

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
        if(this->isQQVIP() && World::getQQVipAct() && type == 2)
        {
            UInt32 qqbuf = GetVar(VAR_QQVIP_BUF);
            if(!qqbuf)
            {
                UInt32 now = TimeUtil::Now();
                setBuffData(PLAYER_BUFF_QQVIPBUF, now + 60 * 60);
                SetVar(VAR_QQVIP_BUF, 1);
                //sendYBBufInfo(0, 1);
                sendYBBufInfo(GetVar(VAR_YBBUF), GetVar(VAR_QQVIP_BUF));
            }
            return;
        }
        if (type == 3)
        {
            if (GetVar(VAR_JOYVIP_BUFF_GOT) == 0)
            {
                addBuffData(PLAYER_BUFF_JOYBUFF, 3600);
                SetVar(VAR_JOYVIP_BUFF_GOT, 1);
                sendYBBufInfo(GetVar(VAR_YBBUF), GetVar(VAR_QQVIP_BUF), 1);
            }
        }
        if ( type == 4)
        {
            if (getBuffData(PLAYER_BUFF_JOYBUFF, TimeUtil::Now()) >= TimeUtil::Now() + 99*3600)
                return;

            if (getGold() >= 5)
            {
                ConsumeInfo ci(BUYJOYBUFF,0,0);
                useGold(5, &ci);
                addBuffData(PLAYER_BUFF_JOYBUFF, 3600);
                sendYBBufInfo(GetVar(VAR_YBBUF), GetVar(VAR_QQVIP_BUF), GetVar(VAR_JOYVIP_BUFF_GOT)+0x02);
              //pl->udpLog("tianjie", "F_1108_1", "", "", "", "", "act");
            }
            else
                sendYBBufInfo(GetVar(VAR_YBBUF), GetVar(VAR_QQVIP_BUF), GetVar(VAR_JOYVIP_BUFF_GOT)+0x00); 
        }

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

            //sendYBBufInfo(ybbuf, 0);
            sendYBBufInfo(GetVar(VAR_YBBUF), GetVar(VAR_QQVIP_BUF));
        }
    }

    void Player::sendYBBufInfo(UInt32 ybbuf, UInt32 qqvipbuf, UInt8 joy)
    {
        Stream st(REP::YBBUF);
        UInt8 qqbuf = qqvipbuf ? true : false;
        //st << static_cast<UInt8>((ybbuf >> 16) & 0xFFFF) << static_cast<UInt8>(ybbuf & 0xFFFF) << qqbuf << Stream::eos;
        if (joy == 0)
            joy = GetVar(VAR_JOYVIP_BUFF_GOT);
        st << static_cast<UInt8>((ybbuf >> 16) & 0xFFFF) << static_cast<UInt8>(ybbuf & 0xFFFF) << qqbuf << joy << Stream::eos;
        send(st);
    }

    void Player::adjustAthlBuffData(UInt32 type)
    {
        UInt32 now = TimeUtil::Now();
        UInt32 leftTime;
        UInt32 flag = 0;
        UInt8 count = 0;
        UInt8 index;
        if(type & Battle::BattleFighter::AthlEnh1)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL1))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL1) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL1, now + leftTime);
            flag = PLAYER_BUFF_ATHL1;
        }
        else if(type & Battle::BattleFighter::AthlEnh2)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL2))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL2) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL2, now + leftTime);
            flag = PLAYER_BUFF_ATHL2;
        }
        else if(type & Battle::BattleFighter::AthlEnh3)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL3))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL3) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL3, now + leftTime);
            flag = PLAYER_BUFF_ATHL3;
        }
        else if(type & Battle::BattleFighter::AthlEnh4)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL4))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL4) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL4, now + leftTime);
            flag = PLAYER_BUFF_ATHL4;
        }
        else if(type & Battle::BattleFighter::AthlEnh5)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL5))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL5) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL5, now + leftTime);
            flag = PLAYER_BUFF_ATHL5;
        }
        else if(type & Battle::BattleFighter::AthlEnh6)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL6))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL6) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL6, now + leftTime);
            flag = PLAYER_BUFF_ATHL6;
        }
        else if(type & Battle::BattleFighter::AthlEnh7)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL7))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL7) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL7, now + leftTime);
            flag = PLAYER_BUFF_ATHL7;
        }
        else if(type & Battle::BattleFighter::AthlEnh8)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL8))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL8) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL8, now + leftTime);
            flag = PLAYER_BUFF_ATHL8;
        }
        else if(type & Battle::BattleFighter::AthlEnh9)
        {
            leftTime = ATHL_BUFF_SECS;
            if(getBuffData(PLAYER_BUFF_ATHL9))
            {
                leftTime = getBuffData(PLAYER_BUFF_ATHL9) - now + ATHL_BUFF_SECS;
                if(leftTime > ATHL_BUFF_SECS_MAX)
                    leftTime = ATHL_BUFF_SECS_MAX;
            }
            setBuffData(PLAYER_BUFF_ATHL9, now + leftTime);
            flag = PLAYER_BUFF_ATHL9;
        }

        //sendAthlBufInfo();
        if(flag == 0)
            return;
        count = 1;
        Stream st(REP::ATHLETICS_REFRESH_MARTIAL);
        st << static_cast<UInt8>(5);
        st << count;
        index = flag - PLAYER_BUFF_ATHL1;
        st << index;
        leftTime = getBuffData(flag) - now;
        st << leftTime;
        st << Stream::eos;
        send(st);
    }

    void Player::sendAthlBufInfo()
    {
        UInt32 leftTime;
        Stream st(REP::ATHLETICS_REFRESH_MARTIAL);
        st << static_cast<UInt8>(5);
        UInt8 count = 0;
        UInt8 index;
        for(index = PLAYER_BUFF_ATHL1; index <= PLAYER_BUFF_ATHL9; index++)
        {
            if(getBuffData(index) > 0)
                count++;
        }
        st << count;
        for(index = PLAYER_BUFF_ATHL1; index <= PLAYER_BUFF_ATHL9; index++)
        {
            if(getBuffData(index) > 0)
            {
                st << static_cast<UInt8>(index - PLAYER_BUFF_ATHL1);
                leftTime = getBuffData(index) - TimeUtil::Now();
                st << leftTime;
            }
        }
        st << Stream::eos;
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
        SYSMSGV(title, 4004, itemId);
        SYSMSGV(content, 4005, itemId, num);
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

    int Player::IDIPBuy(UInt32 itemId, UInt32 num, UInt32 price, std::string& err, bool bind)
    {
        const GData::ItemBaseType* ibt = GData::itemBaseTypeManager[itemId];
        if (!ibt)
        {
            err = "物品不存在";
            return -101;
        }

        if (!num)
        {
            err = "购买的物品个数不对";
            return -102;
        }

        if (getGold() < price)
        {
            err = "仙石不足";
            return -103;
        }

        if (GetPackage()->GetRestPackageSize() < (num+ibt->maxQuantity)/ibt->maxQuantity)
        {
            err = "背包空间不足";
            return -104;
        }

        err = "购买成功";

        struct IDIPBuyItemInfo
        {
            UInt32 itemId;
            UInt32 num;
            UInt32 bind;
            UInt32 price;
        };
        IDIPBuyItemInfo ibi;
        ibi.itemId = itemId;
        ibi.num = num;
        ibi.bind = bind;
        ibi.price = price;
        GameMsgHdr hdr1(0x268, getThreadId(), this, sizeof(ibi));
        GLOBAL().PushMsg(hdr1, &ibi);

        return 0;
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
                fgt->getAllSSAndLevel(st);
                fgt->getAllLbSkills(st);

                fgt->getAttrExtraEquip(st);

                st << fgt->getSoulExtraAura();
                st << fgt->getSoulAuraLeft();
                st << fgt->getPortrait();
                fgt->appendElixirAttr2(st);
            }
        }
        st.data<UInt8>(offset) = c;
    }

    void Player::appendPetOnBattle( Stream& st)
    {
        if(_onBattlePet)
        {
            st << static_cast<UInt16>(_onBattlePet->getId());
            st << _onBattlePet->getLevel() << _onBattlePet->getPotential() << _onBattlePet->getCapacity();
            st << _onBattlePet->getMaxSoul() << _onBattlePet->getPeerlessAndLevel();
            _onBattlePet->getAllUpSkillAndLevel(st);
            _onBattlePet->getAllPSkillAndLevel4Arena(st);
            _onBattlePet->getAllSSAndLevel(st);
            _onBattlePet->getAllLbSkills(st);

            _onBattlePet->getAttrExtraEquip(st);

            st << _onBattlePet->getSoulExtraAura();
            st << _onBattlePet->getSoulAuraLeft();
            st << _onBattlePet->getPortrait();
            _onBattlePet->appendElixirAttr2(st);
        }
        else
        {
            st << static_cast<UInt16>(0);
        }
    }

    void Player::SendNextdayTime(UInt32 nextDay)
    {
        Stream st(REP::SVRST);
        st << static_cast<UInt8>(1);
        st << nextDay << Stream::eos;
        send(st);
        ActivityMgr* mgr = this->GetActivityMgr();
        mgr->CheckTimeOver();
        UInt8 day = TimeUtil::MonthDay(nextDay);
        Stream st1(REP::ACTIVITY_SIGNIN);
        st1 << static_cast<UInt8>(0x00);
        st1 << static_cast<UInt32>(mgr->GetScores());
        st1 << static_cast<UInt16>(mgr->GetOneDayRecord(day));
        st1 << mgr->GetContinueSignInCnt(day);
        st1 << Stream::eos;
        send(st1);
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

    void Player::getSoSoMapAward()
    {
        UInt32 now = TimeUtil::Now();
        if (!World::_sosomapbegin || now < World::_sosomapbegin)
            return;

        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 soso_sharp = TimeUtil::SharpDay(0, World::_sosomapbegin);

        if (now_sharp - soso_sharp > 7 * DAY_SECS)
            return;

        UInt32 off = CREATE_OFFSET(soso_sharp, now_sharp);
        if (off >= 7)
            return;

        UInt32 soso = GetVar(VAR_SOSOMAPAWARD);
        bool got = (soso>>off)&0x1;
        if (got)
            return;

        GameAction()->onSoSoMapAward(this, off);

        soso |= (1<<off);
        SetVar(VAR_SOSOMAPAWARD, soso);

        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(5) << static_cast<UInt8>(0) << Stream::eos;
        send(st);
    }

    void Player::sendSoSoMapInfo()
    {
        UInt32 now = TimeUtil::Now();

        if (!World::_sosomapbegin || now < World::_sosomapbegin)
            return;

        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        UInt32 soso_sharp = TimeUtil::SharpDay(0, World::_sosomapbegin);

        if (now_sharp - soso_sharp > 7 * DAY_SECS)
            return;

        UInt32 off = CREATE_OFFSET(soso_sharp, now_sharp);
        if (off >= 7)
            return;

        UInt32 soso = GetVar(VAR_SOSOMAPAWARD);
        bool got = (soso>>off)&0x1;

        Stream st(REP::GETAWARD);
        st << static_cast<UInt8>(5) << static_cast<UInt8>(got?0:1) << Stream::eos;
        send(st);
    }

    void Player::getKillMonsterAward()
    {
        if (GetPackage()->GetRestPackageSize() < 1)
        {
            sendMsgCode(0, 1011);
            return;
        }
        if(GetPackage()->GetItemAnyNum(9163) < 1)
        {
            return;
        }
        GetPackage()->DelItemAny(9163, 1);
        GameAction()->onGetKillMonsterReward(this);
        udpLog("916", "F_1099", "", "", "", "", "act");
    }

    void Player::consumeGold(UInt32 c)
    {
       if (World::getConsumeActive())
        {
            UInt32 total = GetVar(VAR_CONSUME);
            GameAction()->sendConsumeMails(this, total, total+c);
            SetVar(VAR_CONSUME, total+c);
            sendConsumeInfo(true);
        }
        if (World::getConsume918())
        {
            UInt32 total = GetVar(VAR_CONSUME_918);
            if (total < 100)
            {
                if (total+c >= 100)
                {
                    MailPackage::MailItem item = {515,2};
                    sendMailItem(5100, 5101, &item, 1, true);
                }
            }
            int gold = 0;
            int coupon = 0;
            if (total < 1000 && total+c>=1000)
            {
                gold = 1000;
                coupon = 300;
                SYSMSGV(title, 5103);
                SYSMSGV(content, 5104, gold, coupon, coupon);
                GetMailBox()->newMail(NULL, 0x21, title, content);
            }
            if (total < 5000 && total+c>=5000)
            {
                gold = 5000;
                coupon = 500;
                SYSMSGV(title, 5103);
                SYSMSGV(content, 5104, gold, coupon, coupon);
                GetMailBox()->newMail(NULL, 0x21, title, content);

            }
            if (total < 10000 && total+c>=10000)
            {
                gold = 10000;
                coupon = 700;
                SYSMSGV(title, 5103);
                SYSMSGV(content, 5104, gold, coupon, coupon);
                GetMailBox()->newMail(NULL, 0x21, title, content);

            }
            if (total < 50000 && total+c>=50000)
            {
                gold = 50000;
                coupon = 3500;
            }
            if (gold > 0)
            {
                SYSMSGV(title, 5103);
                SYSMSGV(content, 5104, gold, coupon, coupon);
                GetMailBox()->newMail(NULL, 0x21, title, content);
            }
            AddVar(VAR_CONSUME_918, c);

            Stream st(REP::DAILY_DATA);
		    st << static_cast<UInt8>(17) << total+c << Stream::eos;
		    send((st));

            udpLog("consumeGold", "F_1103", "", "", "", "", "act", c);
        }
        if (World::getConsumeAwardAct())
        {
            sendConsumeAwardInfo(0);
        }
    }

    static const UInt32 s_task1ColorScore[] = {100, 200, 300, 400}; //日常任务的积分
    static const UInt32 s_tjTask1MaxCount = 5;       //天劫日常任务1的最大可完成数
    static const UInt32 s_tjTask2MaxScore = 16000;    //天劫日常任务2的最大捐献积分
    static const UInt8  s_tjTask2Score[4] = {200, 200, 200, 200};
    static const UInt8  s_tjTask2ExpMulti[4] = {30, 30, 30, 30};
    static const UInt8  s_tjTask3CopyCount = 50;
    static const UInt8  s_tjTask3AutoTime  = 30;
    static const UInt8  s_task3ExpMulti = 8;
    static const UInt8  s_task3Score    = 50;
    void Player::OnDoTianjieTask(UInt8 eventId, UInt8 cmd, UInt8 id)
    {
        Stream st(REQ::TIANJIE);
        UInt8 type = 0;
        UInt8 rcmd = 0;
        if (1 == eventId)
        {
            type = 1;
            switch (cmd)
            {
                //查询列表
            case 0:
                {
                    st << type << rcmd;
                    //获取数据列表
                    getTjTask1Data(st);
                    st << Stream::eos;

                    send(st);
                }
                break;
                //攻击据点 默认攻击
            case 1:
            case 2:
                {
                    UInt8 res = attackTjEvent1(id, cmd);
                    if (res > 0)
                    {
                        UInt8 rcmd = res;
                        st << type << rcmd << Stream::eos;
                        send(st);
                    }
                }
                break;
                //刷新任务
            case 3:
                {
                    if (GetVar(VAR_TJ_TASK1_NUMBER) >= 5)
                    {
                        rcmd = 1;
                    }
                    else if (getTael() < 500)
                    {
                        rcmd = 3;
                    }
                    if (rcmd > 0)
                    {
                        st << type << rcmd << Stream::eos;
                        send(st);
                        return;
                    }
                    ConsumeInfo ci(TianjieTask, 0, 0);
                    useTael(500, &ci);

                    memset(_playerData.tjEvent1, 0, sizeof(_playerData.tjEvent1));
                    memset(_playerData.tjColor1, 0, sizeof(_playerData.tjColor1));
                    memset(_playerData.tjExp1,   0, sizeof(_playerData.tjExp1));
                    for (int i = 0; i < 3; ++i)
                        GObject::Tianjie::instance().randomTask1Data(GetLev(),_playerData.tjEvent1[i], _playerData.tjColor1[i], _playerData.tjExp1[i]);

                    st << type << rcmd;
                    getTjTask1Data(st, true);
                    st << Stream::eos;
                    send(st);
                }
                break;
           default:
                break;
            }
        }
        else if (2 == eventId)
        {
            type = 2;
            UInt32 score = GetVar(VAR_TJ_TASK2_SCORE);
            switch (cmd)
            {
                    //查询列表
                case 0:
                    {
                        st << type << rcmd;
                        getTjTask2Data(st);
                        st << Stream::eos;
                        send(st);
                    }
                    break;
                    //捐款
                case 1:
                    {
                        if (score >= s_tjTask2MaxScore) //达到上限
                        {
                            rcmd = 5;
                            st << type << rcmd << Stream::eos;
                            send(st);
                            return;
                        }
                        int tjScore = 0;
                        if (0 == id) //白银
                        {
                            if (getTael() < 1000)
                            {
                                rcmd = 1; //银币不足
                                st << type << rcmd << Stream::eos;
                                send(st);
                                return;
                            }
                            ConsumeInfo ci(TianjieTask, 0, 0);
                            useTael(1000, &ci);
                            AddVar(VAR_TJ_TASK2_TAEL, 1);
                            tjScore = s_tjTask2Score[0];

                            udpLog("tianjie", "F_1109_3", "", "", "", "", "act");
                        }
                        else if (1 == id) //仙石
                        {
                            if (getGold() < 10)
                            {
                                rcmd = 2; //仙石不足
                                st << type << rcmd << Stream::eos;
                                send(st);
                                return;
                            }
                            ConsumeInfo ci(TianjieTask, 0, 0);
                            useGold(10, &ci);
                            AddVar(VAR_TJ_TASK2_GOLD, 1);
                            tjScore = s_tjTask2Score[1];
                            udpLog("tianjie", "F_1109_1", "", "", "", "", "act");
                        }
                        else if (2 == id) //礼券
                        {
                            if (getCoupon() < 10)
                            {
                                rcmd = 3; //不足
                                st << type << rcmd << Stream::eos;
                                send(st);
                                return;
                            }
                            ConsumeInfo ci(TianjieTask, 0, 0);
                            useCoupon(10, &ci);
                            AddVar(VAR_TJ_TASK2_COUPON, 1);
                            tjScore = s_tjTask2Score[2];
                            udpLog("tianjie", "F_1109_2", "", "", "", "", "act");
                        }
                        else if (3 == id) //天劫印记
                        {
                            if (!GetPackage()->DelItemAny(9138, 1))
                            {
                                rcmd = 4; //印记不足
                                st << type << rcmd << Stream::eos;
                                send(st);
                                return;
                            }
                            AddVar(VAR_TJ_TASK2_TJYJ, 1);
                            tjScore = s_tjTask2Score[3];
                            udpLog("tianjie", "F_1109_4", "", "", "", "", "act");
                        }
                        if (tjScore > 0)
                        {
                            int exp = TIANJIE_EXP(GetLev()) * s_tjTask2ExpMulti[id];
                            addExpOrTjScore(exp, tjScore, false, false);

                            AddVar(VAR_TJ_TASK2_SCORE, tjScore);
                        }
                        st << type << rcmd;
                        getTjTask2Data(st);
                        st << Stream::eos;
                        send(st);
                    }
                    break;
                default:
                    break;
            }
        }
        else if (3 == eventId)
        {
            type = 3;
            switch (cmd)
            {
            case 0: //列表
                break;
            case 1: //前往破阵
                {
                    if (GetVar(VAR_TJ_TASK3_COPYID) >= 51)
                    {
                        rcmd = 1;
                        st << type << rcmd << Stream::eos;
                        send(st);
                        return;
                    }
                    attackTjEvent3(id);
                }
                break;
            case 2: //自动破阵
                {
                    processAutoTlz();
                }
                break;
            case 3: //自动完成
                {
                    completeAutoTlz();
                }
                break;
            default:
                break;
            }
        }
    }
    UInt8 Player::attackTjEvent1(UInt8 id, UInt8 cmd)
    {
        if (id > 3)
            return 4;

        if (GetVar(VAR_TJ_TASK1_NUMBER) >= 5)
            return 1;

        if (cmd == 2 && getGold() < 5) //双倍奖励
            return 2; //仙石不足

        bool res = attackTianjieNpc(_playerData.tjEvent1[id], cmd, false);
        if (res)
        {
            if (2 == cmd)
            {
                ConsumeInfo ci(TianjieTask, 0, 0);
                useGold(5, &ci);
            }
            int taskScore = s_task1ColorScore[_playerData.tjColor1[id]] * cmd;

            int exp = _playerData.tjExp1[id] * cmd;
            addExpOrTjScore(exp, taskScore, false, true);

            AddVar(VAR_TJ_TASK1_NUMBER, 1);

            _playerData.tjEvent1[id] = 0;
            _playerData.tjColor1[id] = 0;
            _playerData.tjExp1[id] = 0;

            if (cmd == 1)
                udpLog("tianjie", "F_1111", "", "", "", "", "act");
            else if (cmd == 2)
                udpLog("tianjie", "F_1112", "", "", "", "", "act");
        }
        Stream st(REQ::TIANJIE);
        st <<  static_cast<UInt8>(1) << static_cast<UInt8>(0);
        //获取数据列表
        getTjTask1Data(st);
        st << Stream::eos;
        send(st);
        return 0;
    }


   UInt8 Player::attackTjEvent3(UInt8 id)
   {
        UInt8 copyid = GetVar(VAR_TJ_TASK3_COPYID);
        if (copyid > s_tjTask3CopyCount) return 1;

        if (0 == copyid)
        {
            copyid = 1;
            AddVar(VAR_TJ_TASK3_COPYID, 1);
        }
        bool res = GObject::Tianjie::instance().attackTlz(this, copyid);
        if (res)
        {
            copyid++;
            AddVar(VAR_TJ_TASK3_COPYID, 1);
            int exp = TIANJIE_EXP(GetLev()) * s_task3ExpMulti;
            addExpOrTjScore(exp, s_task3Score, false, true);

            Stream st(REQ::TIANJIE);
            st << static_cast<UInt8>(3) << static_cast<UInt8>(0);
            getTjTask3Data(st);
            st << Stream::eos;
            send(st);

            if (copyid == 51)
            {
                udpLog("tianjie", "F_1114", "", "", "", "", "act");
            }
        }
        return 0;
   }
   void Player::getTjTask1Data(Stream& st, bool isRefresh)
   {
        if (isRefresh || GetVar(VAR_TJ_TASK1_NUMBER) == 0) //今日还没做任务
        {
            for (int i = 0; i < 3; ++i)
            {
                if (_playerData.tjEvent1[i] == 0)
                {
                    GObject::Tianjie::instance().randomTask1Data(GetLev(),_playerData.tjEvent1[i], _playerData.tjColor1[i], _playerData.tjExp1[i]);
                }
            }
        }
        UInt8 type = 0;
        int value[3] = {0};
        value[0] = _playerData.tjExp1[0];
        value[1] = _playerData.tjExp1[1];
        value[2] = _playerData.tjExp1[2];
        for (int i = 0; i < 3; ++i)
        {
            GData::NpcGroups::iterator it = GData::npcGroups.find(_playerData.tjEvent1[i]);
            if(it != GData::npcGroups.end())
            {
		        GData::NpcGroup * ng = it->second;
                value[i] +=  TIANJIE_EXP(GetLev()) * ng->getExp();
            }
        }
        if (GObject::Tianjie::instance().isPlayerInTj(GetLev()))
        {
            type = 1;
            value[0] = value[0] * 2 / TIANJIE_EXP(GetLev()) + s_task1ColorScore[_playerData.tjColor1[0]];
            value[1] = value[1] * 2 / TIANJIE_EXP(GetLev()) + s_task1ColorScore[_playerData.tjColor1[1]];
            value[2] = value[2] * 2 / TIANJIE_EXP(GetLev()) + s_task1ColorScore[_playerData.tjColor1[2]];
        }
        UInt8 num1 = 5-GetVar(VAR_TJ_TASK1_NUMBER);
        st << num1 << type;
        st << _playerData.tjEvent1[0] << _playerData.tjColor1[0] << value[0];
        st << _playerData.tjEvent1[1] << _playerData.tjColor1[1] << value[1];
        st << _playerData.tjEvent1[2] << _playerData.tjColor1[2] << value[2];

   }
   void Player::getTjTask2Data(Stream& st)
   {
       short n1 = GetVar(VAR_TJ_TASK2_TAEL);
       short n2 = GetVar(VAR_TJ_TASK2_GOLD);
       short n3 = GetVar(VAR_TJ_TASK2_COUPON);
       short n4 = GetVar(VAR_TJ_TASK2_TJYJ);
       UInt8 percent = GetVar(VAR_TJ_TASK2_SCORE)*100/s_tjTask2MaxScore;      //捐献百分比
       int exp2 = TIANJIE_EXP(GetLev()) * s_tjTask2ExpMulti[0] ; //经验
       int score = s_tjTask2Score[0];
       if (GObject::Tianjie::instance().isPlayerInTj(GetLev()))
       {
           score += exp2*2/TIANJIE_EXP(GetLev());
           exp2 = 0;
       }
       st << n1 << n2 << n3 << n4 << percent << exp2 << score;
   }
   void Player::getTjTask3Data(Stream& st)
   {
       UInt8 finish = 0;
       UInt8 copyid = GetVar(VAR_TJ_TASK3_COPYID);
       if (copyid >= (s_tjTask3CopyCount+1)) //已完成
       {
           finish = 1;
       }
       if (copyid == 0) copyid = 1;

       UInt8 percent = (copyid-1) * 100/ s_tjTask3CopyCount;
       int exp3 = TIANJIE_EXP(GetLev()) * s_task3ExpMulti;
       int score = s_task3Score;
       int time3 = 0;
       if (hasFlag(Player::AutoTlz))
           time3 = (s_tjTask3CopyCount-copyid+1) * s_tjTask3AutoTime;
       if (GObject::Tianjie::instance().isPlayerInTj(GetLev()))
       {
           score += exp3*2/TIANJIE_EXP(GetLev());
           exp3 = 0;
       }

       st << finish << static_cast<UInt8>(copyid-1) << percent << time3 << exp3 << score;
   }
   void Player::addExpOrTjScore(int exp, int score, bool isEventScore, bool isEndScore)
   {
       int eventScore = 0;
       //天劫事件的经验转换为天劫积分
       if (isEventScore) eventScore = score;

       if (GObject::Tianjie::instance().isPlayerInTj(GetLev()))
       {
           if (isEventScore)
               eventScore += exp*2/TIANJIE_EXP(GetLev());
           score += exp*2/TIANJIE_EXP(GetLev());;
       }
       else if (isEndScore) //战斗结束后再加经验
       {
           pendExp(exp);
       }
       else                 //立即加经验
       {
           AddExp(exp);
       }
       if (isEndScore)
       {
           if (isEventScore)
               _playerData.lastTjEventScore += eventScore;
           _playerData.lastTjTotalScore += score;
       }
       if (eventScore > 0)
       {
           int oldScore = GetVar(VAR_TJ_EVENT_PRESTIGE);
           AddVar(VAR_TJ_EVENT_PRESTIGE, eventScore);
           //捐款不会超过40000积分
           GObject::Tianjie::instance().setEvent2MaxScore(this);
           GObject::Tianjie::instance().insertToEventSortMap(this, GetVar(VAR_TJ_EVENT_PRESTIGE), oldScore);
           GObject::Tianjie::instance().updateEventData(this);
           GObject::Tianjie::instance().broadEventTop1(this);

           GObject::Tianjie::instance().udplogScore(this, eventScore, 1);

           if (!isEndScore)
           {
               SYSMSG_SENDV(167, this, eventScore);
               SYSMSG_SENDV(169, this, eventScore);
           }
       }
       if (score > 0)
       {
           AddVar(VAR_TJ_TASK_PRESTIGE, score);
           GObject::Tianjie::instance().insertToScoreSortMap(this, GetVar(VAR_TJ_TASK_PRESTIGE),GetVar(VAR_TJ_TASK_PRESTIGE)-score);
           GObject::Tianjie::instance().updateRankData(this);

           GObject::Tianjie::instance().udplogScore(this, score, 0);

           if (!isEndScore)
           {
               SYSMSG_SENDV(168, this, score);
               SYSMSG_SENDV(170, this, score);
           }
       }
   }
   void Player::clearTjTaskData()
   {
       memset(_playerData.tjEvent1, 0, sizeof(_playerData.tjEvent1));
       memset(_playerData.tjColor1, 0, sizeof(_playerData.tjColor1));
       memset(_playerData.tjExp1, 0, sizeof(_playerData.tjExp1));

       cancleAutoTlz();
   }
   void Player::processAutoTlz()
   {
       Stream st(REQ::TIANJIE);
       UInt8 type = 3;
       UInt8 rcmd = 0;
       if (hasFlag(Player::AutoTlz))
           return;

       if (GetVar(VAR_TJ_TASK3_COPYID) >= (s_tjTask3CopyCount+1))
       {
           rcmd = 1;
           st << type << rcmd << Stream::eos;
           send(st);
           return;
       }
       else if (GetVar(VAR_TJ_TASK3_COPYID) == 0)
       {
           SetVar(VAR_TJ_TASK3_COPYID, 1);
       }
       if (getTael() < 1000)
       {
           rcmd = 3; //银币不足
           st << type << rcmd << Stream::eos;
           send(st);
           return;
       }
       ConsumeInfo ci(TianjieTask, 0, 0);
       useTael(1000, &ci);

       addFlag(Player::AutoTlz);

       int count = s_tjTask3CopyCount+1 - GetVar(VAR_TJ_TASK3_COPYID);
       EventTlzAuto* event = new(std::nothrow) EventTlzAuto(this, s_tjTask3AutoTime, count);
       if (event == NULL) return;
       PushTimerEvent(event);

       event->notify(true);

       udpLog("tianjie", "F_1116", "", "", "", "", "act");
   }
   void Player::cancleAutoTlz()
   {
       if (hasFlag(Player::AutoTlz))
       {
           //删除定时事件
           PopTimerEvent(this, EVENT_TLZAUTO, getId());
           delFlag(Player::AutoTlz);
       }
   }
   void Player::completeAutoTlz()
   {
       Stream st(REQ::TIANJIE);
       UInt8 type = 3;
       UInt8 rcmd = 0;
       if (!hasFlag(Player::AutoTlz))
           return;

       if (GetVar(VAR_TJ_TASK3_COPYID) >= (s_tjTask3CopyCount+1))
       {
           rcmd = 1;
       }
       if (getGold() < 10)
       {
           rcmd = 2; //xs不足
       }
       if (rcmd > 0 )
       {
           st << type << rcmd << Stream::eos;
           send(st);
           return;
       }
       ConsumeInfo ci(TianjieTask, 0, 0);
       useGold(10, &ci);

       int currCopyId = GetVar(VAR_TJ_TASK3_COPYID);
       if (currCopyId == 0)
           currCopyId = 1;
       int copyCount = s_tjTask3CopyCount - currCopyId + 1;

       SetVar(VAR_TJ_TASK3_COPYID, (s_tjTask3CopyCount+1));
       //加积分和经验
       int exp = TIANJIE_EXP(GetLev()) * s_task3ExpMulti * copyCount;
       addExpOrTjScore(exp, s_task3Score*copyCount, false);

       st << type << rcmd;
       getTjTask3Data(st);
       st << Stream::eos;
       send(st);
       //删除定时事件
       PopTimerEvent(this, EVENT_TLZAUTO, getId());
       delFlag(Player::AutoTlz);

       udpLog("tianjie", "F_1115", "", "", "", "", "act");
   }
   
   void Player::setOpenId(const std::string& openid, bool load /* = false */)
   {
       if (!strncmp(m_openid, openid.c_str(), 256))
           return;
       strncpy(m_openid, openid.c_str(), 256);
       m_openid[255] = '\0';
       if (!load)
       {
           DB1().PushUpdateData("UPDATE `player` SET `openid` = '%s' WHERE `id` = %"I64_FMT"u", m_openid, getId());
       }
   }

   JobHunter* Player::getJobHunter()
   {
       if (GetVar(VAR_EX_JOB_ENABLE) < 2)
           return NULL;
       if (!_jobHunter)
       {
           _jobHunter = new JobHunter(this);
       }
       return _jobHunter;
   }

   void Player::setJobHunter(std::string& fighterList, std::string& mapInfo, UInt8 progress,
           UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY, UInt32 stepCount, UInt8 slotVal1, UInt8 slotVal2, UInt8 slotVal3)
   {
       if (_jobHunter)
           return;
       _jobHunter = new JobHunter(this, fighterList, mapInfo, progress, posX, posY, earlyPosX, earlyPosY, stepCount, slotVal1, slotVal2, slotVal3);
   }

   void Player::sendAutoJobHunter()
   {
       if (!getJobHunter())
           return;
       _jobHunter->SendAutoInfo();
   }

   void Player::setDreamer(UInt8 progress, UInt8 level, UInt8 maxX, UInt8 maxY, UInt8 maxGrid,
           const std::string& mapInfo, UInt8 posX, UInt8 posY, UInt8 earlyPosX, UInt8 earlyPosY,
           UInt8 timeConsume, UInt8 remainTime, UInt8 keysCount, 
           UInt8 eyesCount, UInt8 eyeTime, UInt8 eyeX, UInt8 eyeY)
   {
       if (_dreamer)
           return;
       _dreamer = new Dreamer(this, progress, level, maxX, maxY, maxGrid, mapInfo,
               posX, posY, earlyPosX, earlyPosY, timeConsume, remainTime, keysCount, 
               eyesCount, eyeTime, eyeX, eyeY);
   }


EventTlzAuto::EventTlzAuto( Player * player, UInt32 interval, UInt32 count)
	: EventBase(player, interval, count)
{
}

void EventTlzAuto::Process(UInt32 leftCount)
{
    m_Player->AddVar(VAR_TJ_TASK3_COPYID, 1);

    notify();
    if (leftCount == 0)
    {
        PopTimerEvent(m_Player, EVENT_TLZAUTO,  m_Player->getId());
    }
}

void EventTlzAuto::complete() const
{
}
void EventTlzAuto::notify(bool isBeginAuto)
{
    int copyid = m_Player->GetVar(VAR_TJ_TASK3_COPYID);
    if (!isBeginAuto)
    {
        int exp = TIANJIE_EXP(m_Player->GetLev()) * s_task3ExpMulti;
        m_Player->addExpOrTjScore(exp, s_task3Score, false);
    }
    if (copyid  >= (s_tjTask3CopyCount+1))
    {
        m_Player->delFlag(Player::AutoTlz);
        m_Player->udpLog("tianjie", "F_1114", "", "", "", "", "act");
    }

    if(!m_Player->isOnline())
        return;

    Stream st(REQ::TIANJIE);
    UInt8 type = 3;
    UInt8 cmd = 0;

    st << type << cmd;
    m_Player->getTjTask3Data(st);
    st << Stream::eos;

    m_Player->send(st);
}

    void Player::sendQixiInfo()
    {
        Stream st(REP::ACTIVE);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01);
        if(m_qixi.lover)
            st << m_qixi.lover->getName();
        else
            st << "";

        st << m_qixi.bind << m_qixi.pos << m_qixi.event;
        st << Stream::eos;
        send(st);

        GameMsgHdr hdr1(0x255, getThreadId(), this, 0);
        GLOBAL().PushMsg(hdr1, NULL);
    }

    void Player::divorceQixi()
    {
        /*
        if(!m_qixi.bind)
            return;
        */
        Player* pl = m_qixi.lover;
        if(!pl) return;
        m_qixi.lover = NULL;
        m_qixi.bind = 0;

        pl->beDivorceQixi(this);
        qixiUdpLog(1085);

		DB1().PushUpdateData("UPDATE `qixi` SET `lover`=0, `bind`=0 WHERE `playerId` = %"I64_FMT"u", getId());
        WORLD().DivorceQixiPair(this);
        sendQixiInfo();
    }

    void Player::postQixiEyes(Player* pl)
    {
        if(m_qixi.bind)
            return;

        if(!_hasFriend(0, pl))
            return;
        m_qixi.lover = pl;
        UInt8 bind = pl->beQixiEyes(this);
        onQixiEyesResp(bind);
        if(m_qixi.bind)
        {
            WORLD().UpdateQixiScore(this, m_qixi.lover);
        }
        qixiUdpLog(1084);

		DB1().PushUpdateData("REPLACE INTO `qixi` (`pos`, `event`, `score`, `bind`, `lover`, `playerId`) VALUES(%u, %u, %u, %u, %"I64_FMT"u, %"I64_FMT"u)", m_qixi.pos, m_qixi.event, m_qixi.score, m_qixi.bind, pl->getId(), getId());
    }

    void Player::roamingQueqiao(UInt8 pos)
    {
        if (GetPackage()->GetRestPackageSize() < 1)
        {
            sendMsgCode(0, 1011);
            return;
        }
        if(false == GetPackage()->DelItemAny(qixiTmpl._qixiItem, 1, NULL, ToQixi))
        {
            return;
        }
        udpLog("qixi", "I_9122_2", "", "", "", "", "act");

        UInt8 pos2 = GameAction()->onRoamingQueqiao(this, pos);

        qixiUdpLog(1083);
        Stream st(REP::ACTIVE);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x03) << pos2;
        st << Stream::eos;
        send(st);
    }

    void Player::beDivorceQixi(Player* pl)
    {
        if(m_qixi.lover != pl)
            return;

        m_qixi.bind = 0;

        sendMsgCode(0, qixiTmpl._divorceMsgCode);
		DB1().PushUpdateData("UPDATE `qixi` SET `bind`=0 WHERE `playerId` = %"I64_FMT"u", getId());
        sendQixiInfo();
    }

    UInt8 Player::beQixiEyes(Player* pl)
    {
        UInt8 bind = 0;
        if(m_qixi.bind || m_qixi.lover != pl)
        {
            bind = 0;
        }
        else
        {
            m_qixi.bind = 1;
            bind = 1;

            DB1().PushUpdateData("UPDATE `qixi` SET `bind`=%u WHERE `playerId` = %"I64_FMT"u", bind, getId());
            sendQixiInfo();
        }

        return bind;
    }

    void Player::onQixiEyesResp(UInt8 bind)
    {
        m_qixi.bind = bind;

        Stream st(REP::ACTIVE);
        st << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x02) << bind;
        st << Stream::eos;
        send(st);

        //sendQixiInfo();
    }

    void Player::postRoamResult(UInt8 pos, UInt8 event, UInt8 score)
    {
        struct Roam
        {
            UInt8 pos;
            UInt8 event;
            UInt8 score;
        };

        Roam roam;
        roam.pos = pos;
        roam.event = event;
        roam.score = score;

		GameMsgHdr hdr(0x1F9, WORKER_THREAD_WORLD, this, sizeof(Roam));
		GLOBAL().PushMsg(hdr, &roam);
    }

    void Player::qixiStepAdvance(UInt8 pos, UInt8 event, UInt8 score)
    {
        m_qixi.pos = pos;
        m_qixi.event = event;
        m_qixi.score += score;

        if(m_qixi.lover == NULL && m_qixi.score == score)
            DB1().PushUpdateData("REPLACE INTO `qixi` (`pos`, `event`, `score`, `bind`, `lover`, `playerId`) VALUES(%u, %u, %u, %u, 0, %"I64_FMT"u)", m_qixi.pos, m_qixi.event, m_qixi.score, m_qixi.bind, getId());
        else
            DB1().PushUpdateData("UPDATE `qixi` SET `pos`=%u, `event`=%u, `score`=%u WHERE `playerId` = %"I64_FMT"u", pos, event, m_qixi.score, getId());
        if(m_qixi.bind)
            WORLD().UpdateQixiScore(this, m_qixi.lover);
    }

    void Player::resetQixi()
    {
        m_qixi.bind = 0;
        m_qixi.lover = NULL;
        DB1().PushUpdateData("UPDATE `qixi` SET `bind`=0, `lover`=0 WHERE `playerId` = %"I64_FMT"u", getId());
    }

    void Player::resetSnow()
    {
        m_snow.bind = 0;
        m_snow.lover = NULL;
        DB1().PushUpdateData("UPDATE `snow` SET `bind`=0, `lover`=0 WHERE `playerId` = %"I64_FMT"u", getId());
    }

    void Player::sendSnowInfo()
    {
        Stream st(REP::ACTIVE);
        st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x01) << static_cast<UInt8>(0x01);
        UInt32 score = m_snow.score;
        if(m_snow.lover && m_snow.bind)
        {
            st << m_snow.lover->getName();
    //        score += m_snow.lover->getSnowScore();
        }
        else
            st << "";

        st << score << static_cast<UInt8>(m_snow.bind) << static_cast<UInt16>(GetVar(VAR_SNOW_AWARD));
        st << Stream::eos;
        send(st);
    }

    void Player::divorceSnowLover()
    {
        if(!m_snow.bind)
            return;
        Player* pl = m_snow.lover;
        m_snow.lover = NULL;
        m_snow.bind = 0;

        pl->beDivorceSnowLover(this);
       // qixiUdpLog(1085);

		DB1().PushUpdateData("UPDATE `snow` SET `lover`=0, `bind`=0 WHERE `playerId` = %"I64_FMT"u", getId());
        WORLD().DivorceSnowPair(this);
        sendSnowInfo();
    }
    void Player::beDivorceSnowLover(Player* pl)
    {
        if(m_snow.lover != pl)
            return;

        m_snow.bind = 0;
        m_snow.lover = NULL;

        sendMsgCode(0, 1034);
		DB1().PushUpdateData("UPDATE `snow` SET `bind`=0,`lover`=0 WHERE `playerId` = %"I64_FMT"u", getId());
        sendSnowInfo();
    }

    void Player::postSnowLover(Player* pl)
    {
        if(m_snow.bind)
            return;

        if(!_hasFriend(0, pl))
            return;
        m_snow.lover = pl;
        UInt8 bind = pl->beSnowLoverBind(this);
        onSnowLoverResp(bind);
        if(m_snow.bind)
        {
            WORLD().UpdateSnowScore(this, m_snow.lover);
        }
       // qixiUdpLog(1084);

		DB1().PushUpdateData("REPLACE INTO `snow` (`score`, `bind`, `lover`, `playerId`) VALUES(%u, %u, %"I64_FMT"u, %"I64_FMT"u)", m_snow.score, m_snow.bind, pl->getId(), getId());
    }
    UInt8 Player::beSnowLoverBind(Player* pl)
    {
        UInt8 bind = 0;
        if(m_snow.bind || m_snow.lover != pl)
        {
            bind = 0;
        }
        else
        {
            m_snow.bind = 1;
            bind = 1;

            DB1().PushUpdateData("UPDATE `snow` SET `bind`=%u WHERE `playerId` = %"I64_FMT"u", bind, getId());
            sendSnowInfo();
        }

        return bind;
    }
    void Player::onSnowLoverResp(bool bind)
    {
        m_snow.bind = bind;

        Stream st(REP::ACTIVE);
        st << static_cast<UInt8>(0x05) << static_cast<UInt8>(0x02);
        st << static_cast<UInt8>(bind);
        st << Stream::eos;
        send(st);
    }
    UInt8 Player::useSnowItem(UInt32 num)
    {
        //9275
        if (GetPackage()->DelItemAny(9275, num))
        {
            UInt32 oldScore = m_snow.score;

            UInt32 minScore = num * 3;
            UInt32 maxScore = num * 5;
            UInt32 score = minScore + uRand(maxScore-minScore);
            m_snow.score += score;
            AddPExp(99*num);

            //sendSnowScoreAward();
            //if (m_snow.bind && m_snow.lover != NULL)
            //    m_snow.lover->sendSnowScoreAward();
            if (oldScore < 300 && m_snow.score >= 300)
            {
                SYSMSG(title, 4112);
                SYSMSGV(ctx, 4113, 300);
                Mail * mail = GetMailBox()->newMail(NULL, 0x21, title, ctx, 0xFFFE0000);
                if (mail)
                {
                    MailPackage::MailItem item = {1637,1};
                    mailPackageManager.push(mail->id, &item, 1, true);
                }
            }
            WORLD().UpdateSnowScore(this, m_snow.lover);
            if (NULL !=  m_snow.lover)
		        DB1().PushUpdateData("REPLACE INTO `snow` (`score`, `bind`, `lover`, `playerId`) VALUES(%u, %u, %"I64_FMT"u, %"I64_FMT"u)", m_snow.score, m_snow.bind, m_snow.lover->getId(), getId());
            else
		        DB1().PushUpdateData("REPLACE INTO `snow` (`score`, `bind`, `lover`, `playerId`) VALUES(%u, %u, 0, %"I64_FMT"u)", m_snow.score, m_snow.bind, getId());

            return 0;
        }
        return 1;
    }

    void Player::sendSnowScoreAward()
    {
        static  MailPackage::MailItem s_item[][3] = {
            {{1637,1}},
            {{401,5},{547,5}},
            {{509,2},{507,2}},
            {{503,10},{516,10}},
            {{134,5},{1325,5},{9076,5}},
            {{1325,10},{134,10},{9076,10}}
        };
        static int s_itemCount[]= {1,2,2,2,3,3};
        static UInt32 s_score[] = {300, 900, 1800, 3000, 5100, 9900};
        UInt32 score = m_snow.score;
        //if (NULL != m_snow.lover && m_snow.bind)
        //score += m_snow.lover->getSnowScore();
        UInt32 v =  GetVar(VAR_SNOW_AWARD) >> 8; //邮件是否已发的标志
        for (UInt8 i = 0; i < sizeof(s_score)/sizeof(s_score[0]); ++i)
        {
            if (score < s_score[i])
                break;
            if (((v>>i) & 0x01) == 0)
            {
                SYSMSG(title, 4102);
                SYSMSGV(ctx, 4103, s_score[i]);
                Mail * mail = GetMailBox()->newMail(NULL, 0x21, title, ctx, 0xFFFE0000);
                if (mail)
                {
                    mailPackageManager.push(mail->id, s_item[i], s_itemCount[i], true);
                    v |= (0x01 << i);
                }
            }
        }
        if (v > (GetVar(VAR_SNOW_AWARD) >> 8))
        {
            v  = v << 8;
            v |= GetVar(VAR_SNOW_AWARD);
            SetVar(VAR_SNOW_AWARD, v);
        }
    }
    UInt8 Player::getSnowAward(UInt16 type)
    {
        static  MailPackage::MailItem s_item1[4] = {{56,1},{502,1},{510,1},{548,1}};
        static  MailPackage::MailItem s_item2[4] = {{514,1},{57,1},{500,1},{15,1}};
        static  MailPackage::MailItem s_item3[4] = {{503,1},{512,1},{516,1},{513,1}};
        static  MailPackage::MailItem s_item4[4] = {{1325,1},{134,1},{547,1},{551,1}};
        static  MailPackage::MailItem s_item5[4] = {{401,3},{547,3},{511,3},{514,3}};
        static  MailPackage::MailItem s_item6[4] = {{509,2},{507,2},{501,2},{513,2}};
        static  MailPackage::MailItem s_item7[4] = {{503,8},{516,8},{500,8},{505,8}};
        static  MailPackage::MailItem s_item8[4] = {{134,5},{1325,5},{9076,5},{507,5}};
        static  MailPackage::MailItem s_item9[4] = {{1325,10},{134,10},{9076,10},{509,10}};
  
        if(GetPackage()->GetRestPackageSize() < 4)
        {
            sendMsgCode(0, 1011);
            return 2;
        }
        UInt16 v = GetVar(VAR_SNOW_AWARD);
        if (v&type)
            return 3;

        UInt32 score = m_snow.score;
        // if (m_snow.lover != NULL && m_snow.bind)
        //     score += m_snow.lover->getSnowScore();
        
        UInt32 needScore = 0;
        MailPackage::MailItem* pItems = NULL;
        switch (type)
        {
            case 0x01:
                needScore = 60;
                pItems = s_item1;
                break;
            case 0x02:
                needScore = 140;
                pItems = s_item2;
                break;
            case 0x04:
                needScore = 220;
                pItems = s_item3;
                break;
            case 0x08:
                needScore = 300;
                pItems = s_item4;
                break;
            case 0x10:
                needScore = 450;
                pItems = s_item5;
                break;
            case 0x20:
                needScore = 900;
                pItems = s_item6;
                break;
            case 0x40:
                needScore = 1500;
                pItems = s_item7;
                break;
            case 0x80:
                needScore = 2550;
                pItems = s_item8;
                break;
            case 0x100:
                needScore = 4950;
                pItems = s_item9;
                break;
        }
        if (needScore > score)
            return 1;
        if (pItems != NULL)
        {
            for (int i = 0; i < 4; ++i)
            {
                if (pItems[i].id == 0 )
                    break;
                m_Package->Add(pItems[i].id, pItems[i].count, true);
            }
            v |= type;
            SetVar(VAR_SNOW_AWARD, v);
        }
        return 0;
    }

    void  Player::setForbidSale(bool b, bool isAuto /* = false */)
    {
        _isForbidSale = b;
        if (isAuto)
        {
            // 服务器自助查询OpenAPI封杀交易，udp上报一下
            udpLog("svr_forbid_sale", "success", "", "", "", "", "act_tmp");
        }
    }

    void Player::postKillMonsterRoamResult(UInt32 pos, UInt8 curType, UInt8 curCount, UInt8 tips)
    {
        struct _Roam
        {
            UInt32 _pos;
            UInt8 _curType;
            UInt8 _curCount;
            UInt8 _tips;
        }curRoam = {pos, curType, curCount, tips};

		GameMsgHdr hdr(0x1FE, WORKER_THREAD_WORLD, this, sizeof(curRoam));
		GLOBAL().PushMsg(hdr, &curRoam);
    }

    void Player::killMonsterStepAdvance(UInt32 pos, UInt8 curType, UInt8 curCount, UInt8 tips)
    {
        SetVar(VAR_ZYCM_POS, pos);
        if(curType == 1)
            AddVar(VAR_XIAGU_CNT, curCount);
        else if(curType == 2)
            AddVar(VAR_ROUQING_CNT, curCount);
        else if(curType == 3)
            AddVar(VAR_CAIFU_CNT, curCount);
        else if(curType == 4)
            AddVar(VAR_CHUANQI_CNT, curCount);

        if(GetVar(VAR_ZYCM_TIPS) != tips)
            SetVar(VAR_ZYCM_TIPS, tips);

        WORLD().UpdateKillMonsterRank(this, curType, curCount);
    }

    ///////////////////////////////////////////////
    // 帮派副本相关

    // 帮派副本相关
    ///////////////////////////////////////////////
    bool Player::checkTrumpMutually(UInt32 trumpid)
    {
        static UInt32 muttrumps[][2] = {
            {1529, 1532},
            {1530, 1533},
            {1531, 1534},
            {1650, 1653},
            {1651, 1654},
            {1652, 1655}
        };
        if ((trumpid >= 1529 && trumpid <= 1534)
                || (trumpid >= 1650 && trumpid <= 1655))
        {
            size_t i = 0;
            for (; i < sizeof(muttrumps)/(sizeof(UInt32)*2); ++i)
            {
                if (trumpid == muttrumps[i][0] || trumpid == muttrumps[i][1])
                    break;
            }

            for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++it)
            {
                Fighter* fgt = it->second;
                UInt32 trumpids[32];
                size_t c = fgt->getAllTrumpTypeId(trumpids, sizeof(trumpids)/sizeof(UInt32));
                if (!c)
                    continue;
                for (size_t j = 0; j < c; ++j)
                {
                    if (!trumpids[j])
                        continue;
                    if (trumpids[j] == muttrumps[i][0] || trumpids[j] == muttrumps[i][1])
                    {
                        sendMsgCode(0, 1032);
                        return true;
                    }
                }
            }
        }
        return false;
    }

    UInt32 Player::getEventState(UInt32 type)
    {
        if (!World::getTgcEvent())
            return 0;

        switch (type)
        {
            case 1:
                {
                    ItemBase* ib = GetPackage()->GetItem(9188, true);
                    if (!ib)
                        return 0;
                    return 1;
                }
                break;
            case 2:
                {
                    ItemBase* ib = GetPackage()->GetItem(9273, true);
                    if (!ib)
                        return 0;
                    return 1;
                }
                break;
            default:
                break;
        }
        return 0;
    }

    UInt32 Player::getBattlePoint()
    {
        UInt32 bp = 0;
        if(CURRENT_THREAD_ID() == getThreadId())
        {
            for(int j = 0; j < 5; ++ j)
            {
                Fighter* fighter = _playerData.lineup[j].fighter;
                if(fighter)
                    bp += fighter->getBattlePoint();
            }
            calcLingbaoBattlePoint();
        }
        else
        {
            GameMsgHdr hdr(0x267, getThreadId(), this, 0);
            GLOBAL().PushMsg(hdr, NULL);

            for(int j = 0; j < 5; ++ j)
            {
                Fighter* fighter = _playerData.lineup[j].fighter;
                if(fighter)
                    bp += fighter->getBattlePoint_Dirty();
            }
        }

        return bp;
    }

    void Player::calcLingbaoBattlePoint()
    {
        if(CURRENT_THREAD_ID() == getThreadId())
        {
            UInt32 value = 0;
            _maxLingbaoBattlePoint = 0;
            for(int j = 0; j < 5; ++ j)
            {
                Fighter* fighter = _playerData.lineup[j].fighter;
                if(fighter)
                {
                    value = fighter->calcLingbaoBattlePoint();
                    _maxLingbaoBattlePoint = value > _maxLingbaoBattlePoint? value:_maxLingbaoBattlePoint;
                }
            }
        }
        else
        {
            GameMsgHdr hdr(0x353, getThreadId(), this, 0);
            GLOBAL().PushMsg(hdr, NULL);
        }

    }

    void Player::setMaxLingbaoBattlePoint(UInt32 value)
    {
        _maxLingbaoBattlePoint = value;
    }


    UInt32 Player::getMaxLingbaoBattlePoint()
    {
        return _maxLingbaoBattlePoint;
    }

    void Player::verifyFighter()
    {
        Fighter* mfgt = NULL;
        bool find = false;
        UInt8 maxLevel = 0;
        for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++it)
        {
            Fighter* fgt = it->second;
            if(fgt->getLevel() > maxLevel)
                maxLevel = fgt->getLevel();
            if(fgt->isMainFighter())
            {
                if(!mfgt)
                    mfgt = fgt;
                else if( fgt->getLevel() <= mfgt->getLevel())
                {
                    DB1().PushUpdateData("DELETE FROM `fighter` where `id`=%d and `playerId`=%"I64_FMT"u", fgt->getId(), _id);
                    std::map<UInt32, Fighter *>::iterator tmp = it;
                    -- tmp;
                    _fighters.erase(it);
                    delete fgt;
                    find = true;
                    it = tmp;
                }
                else
                {
                    DB1().PushUpdateData("DELETE FROM `fighter` where `id`=%d and `playerId`=%"I64_FMT"u", mfgt->getId(), _id);
                    _fighters.erase(mfgt->getId());
                    delete mfgt;
                    mfgt = fgt;
                    find = true;
                }
            }
        }

        if(find)
        {
            bool first = false;
            bool lfind = false;
            for(int i = 0; i < 5; ++ i)
            {
                Lineup& lup = _playerData.lineup[i];
                if(lup.fid < 10)
                {
                    if(!first)
                    {
                        if(lup.fid != mfgt->getId())
                            lfind = true;
                        lup.fid = mfgt->getId();
                        lup.fighter = mfgt;
                        first = true;
                    }
                    else
                    {
                        lfind = true;
                        lup.fid = 0;
                        lup.fighter = NULL;
                    }
                }
            }
            if(lfind)
                storeFighters();
        }

        if(mfgt->getLevel() < maxLevel)
        {
            mfgt->resetLevelAndExp(maxLevel);
            mfgt->reload2ndSoul();
        }
    }

    bool Player::hasTitle(UInt8 title)
    {
        std::map<UInt8, UInt32>::iterator it = _playerData.titleAll.find(title);
        if(it == _playerData.titleAll.end())
            return false;
        return true;
    }

    bool Player::checkTitleTimeEnd(UInt8 title, UInt32& timeLeft)
    {
        UInt32 now = TimeUtil::Now();
        std::map<UInt8, UInt32>::iterator it = _playerData.titleAll.find(title);
        if(it == _playerData.titleAll.end())
            return false;

        timeLeft = 0;
        if(it->second == 0)
            return true;

        if(it->second > now)
        {
            timeLeft = it->second - now;
            return true;
        }

        _playerData.titleAll.erase(title);
        return false;
    }

    bool Player::makeTitleAllInfo(Stream& st)
    {
        bool flag = false;
        std::map<UInt8, UInt32>& titleAll = _playerData.titleAll;
        UInt8 cnt = titleAll.size();
        size_t offset = st.size();
        st << cnt;
        for(std::map<UInt8, UInt32>::iterator it = titleAll.begin(); it != titleAll.end();)
        {
            UInt32 timeLeft = 0;
            std::map<UInt8, UInt32>::iterator tmp = it;
            ++ tmp;
            if(checkTitleTimeEnd(it->first, timeLeft))
            {
                st << it->first << static_cast<UInt32>(timeLeft);
            }
            else
            {
                flag = true;
                -- cnt;
            }
            it = tmp;
        }
        st.data<UInt8>(offset) = cnt;

        return flag;
    }

    void Player::writeTitleAll()
    {
        std::map<UInt8, UInt32>& titleAll = _playerData.titleAll;
        UInt8 cnt = titleAll.size();
        std::string title = "";

        if(!cnt)
        {
            _playerData.titleAll[0] = 0;
            title += "0,0|";
        }

        for(std::map<UInt8, UInt32>::iterator it = titleAll.begin(); it != titleAll.end(); ++ it)
        {
            title += Itoa(it->first);
            title += ',';
            title += Itoa(it->second);
            title += '|';
        }

        DB1().PushUpdateData("UPDATE `player` SET `titleAll` = '%s' WHERE `id` = %"I64_FMT"u", title.c_str(), getId());
    }
    UInt8 Player::fightTransform(UInt16 fFighterId, UInt16 tFighterId, UInt8 type)
    {
        UInt8 res = 0;
        Fighter * fFgt = findFighter(fFighterId);
        Fighter * tFgt = findFighter(tFighterId);
        if (NULL == fFgt || NULL == tFgt)
            return 1;
        if (isMainFighter(fFighterId) || isMainFighter(tFighterId))
            return 2;
        if (fFgt->getLevel() < 70 || tFgt->getLevel() < 70)
            return 3;
        res = canTransform(fFgt, tFgt, type);
        if (res > 0 )
            return res;
        if ((res = transformUseMoney(fFgt, tFgt, type)) > 0)
            return res;

        if (type & 0x01)
            res = transformExp(fFgt, tFgt);
        if ((type & 0x02) && res==0)
            transformPotential(fFgt, tFgt);
        //if ((type & 0x04) && res==0)
        //    transformCapacity(fFgt, tFgt);
        if ((type & 0x08) && res==0)
            res = transformSoul(fFgt, tFgt);
        if ((type & 0x10) && res==0)
            transformElixir(fFgt, tFgt);

        return res;
    }
    UInt8 Player::canTransform(Fighter * fFgt, Fighter * tFgt, UInt8 type)
    {
    //    if ((type & 0x01) && (tFgt->getLevel() >= GetLev()))
    //            return 4;
        if ((type & 0x02 || type & 0x04) && GetPackage()->GetRestPackageSize() < 1)
            return 5;
        if ((type & 0x08) && (fFgt->getSecondSoul() == NULL || tFgt->getSecondSoul() == NULL))
            return 6;
        return 0;
    }

    UInt8 Player::transformUseMoney(Fighter * fFgt, Fighter * tFgt, UInt8 type)
    {
        UInt32 money = 0;
        UInt32 money1 = 0;
        UInt32 money2 = 0;
        UInt32 money3 = 0;
        UInt32 money4 = 0;
        UInt8 val1 = 0;
        if (type & 0x01)
        {
             money += 10;
             money1 += 10;
        }
        if (type & 0x02)
        {
            float p = std::max(fFgt->getPotential(), tFgt->getPotential());
            if (p >= 1.80f)
            {
                money += 100;
                money2 += 100;
                val1 = 4;
            }
            else if (p >= 1.50f)
            {
                money += 60;
                money2 += 60;
                val1 = 3;
            }
            else if (p >= 1.20f)
            {
                money += 30;
                money2 += 30;
                val1 = 2;
            }
            else
            {
                money += 10;
                money2 += 10;
                val1 = 1;
            }

            float c = std::max(fFgt->getCapacity(),tFgt->getCapacity());
            if (c >= 9.0f)
            {
                money += 100;
                money2 += 100;
            }
            else if (c >= 8.0f)
            {
                money += 60;
                money2 += 60;
            }
            else if (c >= 7.0f)
            {
                money += 30;
                money2 += 30;
            }
            else
            {
                money += 10;
                money2 += 10;
            }
        }
        if (type & 0x08)
        {

            SecondSoul* fSoul = fFgt->getSecondSoul();
            SecondSoul* tSoul = tFgt->getSecondSoul();
            //元神境界
            UInt8 f = fSoul->getStateLevel();
            UInt8 t = tSoul->getStateLevel();
            //元神等级
            UInt8 fPracLev = fSoul->getPracticeLevel();
            UInt8 tPracLev = tSoul->getPracticeLevel();
            //星宿
            UInt8 fXinxiu = fSoul->getXinxiu();
            UInt8 tXinxiu = tSoul->getXinxiu();
            money += (std::max(f,t) * 10);
            money += abs(int(fPracLev-tPracLev))*1;
            money3 += (std::max(f,t) * 10);
            money3 += abs(int(fPracLev-tPracLev))*1;
            if (fXinxiu != tXinxiu)
            {
                money += 50;
                money3 += 50;
            }
        }
        if (type & 0x10)
        {
            for (UInt8 i = 0; i < 14; ++i)
            {
                Int32 f = fFgt->getElixirAttrByOffset(i);
                Int32 t = tFgt->getElixirAttrByOffset(i);
                money += abs(int(f-t))*1;
                money4 += abs(int(f-t))*1;
            }
        }
        //34是测试区
        if(getGold() < money && cfg.serverNum != 34)
	    {
            sendMsgCode(0, 1101);
            return 10;
        }
        if (money > 0 && cfg.serverNum != 34)
        {
            ConsumeInfo ci(FightTransform,0,0);
            useGold(money, &ci);
        }

        transformUdpLog(1164, type, money1, money2, money3, money4, val1);
        
        return 0;
    }

    UInt8 Player::transformExp(Fighter * fFgt, Fighter * tFgt)
    {
     //   UInt64 exp_70 = GData::expTable.getLevelMin(70);
     //   UInt64 exp_add = fFgt->getExp() - exp_70;
     //   tFgt->addExp(exp_add);
        UInt64 fExp = fFgt->getExp();
        UInt64 tExp = tFgt->getExp();
        UInt8 fLev = fFgt->getLevel();
        UInt8 tLev = tFgt->getLevel();
        fFgt->setLevelAndExp(tLev, tExp);
        tFgt->setLevelAndExp(fLev, fExp);
        return 0;
    }
    //潜力和资质,天赋
    UInt8 Player::transformPotential(Fighter * fFgt, Fighter * tFgt)
    {
        //天赋对换
        UInt8 fAttrType1 = fFgt->getAttrType1();
        UInt8 fAttrType2 = fFgt->getAttrType2();
        UInt8 fAttrType3 = fFgt->getAttrType3();
        UInt8 tAttrType1 = tFgt->getAttrType1();
        UInt8 tAttrType2 = tFgt->getAttrType2();
        UInt8 tAttrType3 = tFgt->getAttrType3();

        UInt16 fAttrValue1 = fFgt->getAttrValue1();
        UInt16 fAttrValue2 = fFgt->getAttrValue2();
        UInt16 fAttrValue3 = fFgt->getAttrValue3();
        UInt16 tAttrValue1 = tFgt->getAttrValue1();
        UInt16 tAttrValue2 = tFgt->getAttrValue2();
        UInt16 tAttrValue3 = tFgt->getAttrValue3();

        fFgt->setAttrType1(tAttrType1);
        fFgt->setAttrType2(tAttrType2, true);
        fFgt->setAttrType3(tAttrType3, true);
        fFgt->setAttrValue1(tAttrValue1);
        fFgt->setAttrValue2(tAttrValue2, true);
        fFgt->setAttrValue3(tAttrValue3, true);

        tFgt->setAttrType1(fAttrType1);
        tFgt->setAttrType2(fAttrType2, true);
        tFgt->setAttrType3(fAttrType3, true);
        tFgt->setAttrValue1(fAttrValue1);
        tFgt->setAttrValue2(fAttrValue2, true);
        tFgt->setAttrValue3(fAttrValue3, true);

        float fp = fFgt->getPotential();
        float tp = tFgt->getPotential();
        float fc = fFgt->getCapacity();
        float tc = tFgt->getCapacity(); 
        fFgt->setPotential(tp);
        tFgt->setPotential(fp);
        fFgt->setCapacity(tc);
        tFgt->setCapacity(fc);

        //卸载第一个被动法宝
        GObject::ItemEquip* p =NULL;
        if ( tp < 1.50f || tc < 7.0f)
        {
           p =  fFgt->setTrump(p, 1, true);
           if (p != NULL)
               GetPackage()->AddExistEquip(p);
        }
        GObject::ItemEquip* p1 =NULL;
        if ( fp < 1.50f || fc < 7.0f)
        {
           p1 =  tFgt->setTrump(p1, 1, true);
           if (p1 != NULL)
               GetPackage()->AddExistEquip(p1);
        }
 
        fFgt->updateForgeAttr(true);
        tFgt->updateForgeAttr(true);
        return 0;
    }
    //资质
    UInt8 Player::transformCapacity(Fighter * fFgt, Fighter * tFgt)
    {
        float f = fFgt->getCapacity();
        float t = tFgt->getCapacity();
        fFgt->setCapacity(t);
        tFgt->setCapacity(f);

        //卸载第一个被动法宝
        GObject::ItemEquip* p =NULL;
        if ( t < 7.0)
        {
            p =  fFgt->setTrump(p, 1, true);
            if (p != NULL)
                GetPackage()->AddExistEquip(p);
        }
        if (t < 7.0f && f >= 7.0f)
        {
            tFgt->getAttrType2(true);
            tFgt->getAttrType3(true);
        }
        return 0;
    }
    UInt8 Player::transformSoul(Fighter * fFgt, Fighter * tFgt)
    {
        SecondSoul* fSoul = fFgt->getSecondSoul();
        SecondSoul* tSoul = tFgt->getSecondSoul();
        if (NULL == fSoul || NULL ==tSoul)
            return 6;
        UInt32 f = fSoul->getStateExp();
        UInt32 t = tSoul->getStateExp();
        UInt32 fLev = fSoul->getStateLevel();
        UInt32 tLev = tSoul->getStateLevel();
        fSoul->setStateExp(tLev, t);
        tSoul->setStateExp(fLev, f);

        //元神等级
        UInt8 fPracLev = fSoul->getPracticeLevel();
        UInt8 tPracLev = tSoul->getPracticeLevel();
        if (fPracLev != tPracLev)
        {
            fSoul->setPracticeLevel(tPracLev);
            tSoul->setPracticeLevel(fPracLev);
        }
        //星宿
        UInt8 fXinxiu = fSoul->getXinxiu();
        UInt8 tXinxiu = tSoul->getXinxiu();
        if (fXinxiu != tXinxiu)
        {
            fSoul->setXinxiu(tXinxiu);
            tSoul->setXinxiu(fXinxiu);
        }
        //原始散仙卸载所有心法
        fFgt->offAllCitta();
        tFgt->offAllCitta();

        fFgt->send2ndSoulInfo();
        tFgt->send2ndSoulInfo();

        fFgt->sendMaxSoul();
        tFgt->sendMaxSoul();
        return 0;
    }
    void Player::transformElixir(Fighter * fFgt, Fighter * tFgt)
    {
        for (UInt8 i = 0; i < 14; ++i)
        {
            Int32 f = fFgt->getElixirAttrByOffset(i);
            Int32 t = tFgt->getElixirAttrByOffset(i);
            if (f != t)
            {
                fFgt->addElixirAttrByOffset(i, t-f);
                tFgt->addElixirAttrByOffset(i, f-t);
                if (i == 5)
                {
                    fFgt->offAllCitta();
                    tFgt->offAllCitta();
                }
            }
        }
    }
    
    void Player::ArenaExtraAct(UInt8 type, UInt8 opt)
    {
        UInt32 now = TimeUtil::Now();
        UInt8 week = TimeUtil::GetWeekDay(now);
        UInt32 t1 = TimeUtil::SharpDayT(0, now) + ARENA_SINGUP_START;
        UInt32 t2 = TimeUtil::SharpDayT(0, now) + ARENA_SINGUP_END;
        UInt32 t3 = TimeUtil::SharpDayT(0, now) + ARENA_SUFFER_END;
        static UInt32 broadfreq;
        UInt8 curtype;

        if(week < ARENA_WEEK_START || week > ARENA_WEEK_END)
            return;
        if(now < t1)
            return;

        curtype = week - ARENA_WEEK_START;
        if(WORLD().getArenaHeroId(0) == 0)
            WORLD().setArenaInfo(curtype);
        if(WORLD().getArenaHeroId(0) == 0)
            return;
        if(now >= t2 && World::getArenaTotalCnt() == 0)
        {
            WORLD().setArenaTotalCntEnum();
        }

        UInt32 totalCnt = WORLD().getArenaTotalCnt();
        UInt32 totalSufferCnt = totalCnt * 24 / 5;

        switch(type)
        {
            case 0:
            {
                Stream st(REP::SERVER_ARENA_EXTRA_ACT);
                st << week;
                st << type;
                for(UInt8 i = 0; i < 5; i++)
                {
                    st << WORLD().getArenaName(i);
                    st << WORLD().getArenaHeroId(i);
                }
                st << totalSufferCnt << Stream::eos;
                send(st);
                if(now >= t1 && now < t2)
                    ArenaExtraAct(1+ARENA_ACT_SYSTEM, 0);
                else if(now >= t2 && now < t3)
                    ArenaExtraAct(2+ARENA_ACT_SYSTEM, 0);
                else
                    ArenaExtraAct(3, 0);
            }
            break;
            case 1:
            case 1+ARENA_ACT_SYSTEM:
            {
                if(type < ARENA_ACT_SYSTEM)
                {
                    if(now >= t2)
                        return;
                    UInt8 supportId = opt;
                    if(supportId == 0 || supportId > 5)
                        return;
                    SetVar(VAR_ARENA_SUPPORT, supportId);
                    if(week == ARENA_WEEK_START)
                        SetVar(VAR_ARENA_SUPPORT_TUE, supportId);
                    else
                        SetVar(VAR_ARENA_SUPPORT_WED, supportId);
                }
                Stream st(REP::SERVER_ARENA_EXTRA_ACT);
                st << week;
                st << static_cast<UInt8>(1);
                UInt32 seconds = 0;
                if(now >= t1 && now < t2)
                    seconds = t2 - now;
                st << static_cast<UInt8>(GetVar(VAR_ARENA_SUPPORT)) << seconds;
                st << Stream::eos;
                send(st);
            }
            break;
            case 2:
            case 2+ARENA_ACT_SYSTEM:
            {
                if(type < ARENA_ACT_SYSTEM)
                {
                    if(now < t2 || now >= t3)
                        return;
                    if(getBuffLeft(PLAYER_BUFF_SUFFER) >= 5)//5秒误差
                        return;
                    UInt8 sufferId = opt;
                    if(sufferId == 0 || sufferId > 5)
                        return;
                    if(World::stArena.sufferCnt[sufferId-1] >= totalSufferCnt)
                        return;

                    ++broadfreq;
                    UInt32 pexp = 1000;
                    GameMsgHdr hdr2(0x238, getThreadId(), this, sizeof(pexp));
                    GLOBAL().PushMsg(hdr2, &pexp);

                    setBuffData(PLAYER_BUFF_SUFFER, TimeUtil::Now() + 30);
                    World::stArena.sufferCnt[sufferId-1] += 1;
                    World::stArena.lasttime[sufferId-1] = now;

                    if(World::stArena.sufferCnt[sufferId-1] == totalSufferCnt)
                    {
                        UInt32 moneyArena = 500;
                        SYSMSGV(title, 736);
                        SYSMSGV(content, 738, moneyArena);
                        GetMailBox()->newMail(NULL, 0x01, title, content);
                        GameMsgHdr hdr(0x251, getThreadId(), this, sizeof(moneyArena));
                        GLOBAL().PushMsg(hdr, &moneyArena);
                        broadfreq = 5;
                    }
                }
                Stream st(REP::SERVER_ARENA_EXTRA_ACT);
                st << week;
                st << static_cast<UInt8>(2);

                UInt32 seconds = 0;
                if(now >= t2 && now < t3)
                    seconds = t3 - now;
                st << static_cast<UInt8>(GetVar(VAR_ARENA_SUPPORT)) << seconds << static_cast<UInt16>(getBuffLeft(PLAYER_BUFF_SUFFER));
                for(UInt8 i = 0; i < 5; i++)
                {
                    st << World::stArena.sufferCnt[i];
                }
                st << Stream::eos;
                send(st);
                if(broadfreq >= 5)
                {
                    broadfreq = 0;

                    Stream st(REP::SERVER_ARENA_EXTRA_ACT);
                    st << week;
                    st << static_cast<UInt8>(4);
                    for(UInt8 i = 0; i < 5; i++)
                    {
                        st << World::stArena.sufferCnt[i];
                    }
                    st << Stream::eos;
                    NETWORK()->Broadcast(st);
                }
                }
                break;
                case 3:
                {
                    if(GObject::World::stArena.rank[0] == 0 && GObject::World::stArena.rank[1] == 0 && GObject::World::stArena.rank[2] == 0 && GObject::World::stArena.rank[3] == 0 && GObject::World::stArena.rank[4] == 0)
                    {
                        ValueSort cur;
                        ValueSortType resultRank;
                        for(UInt8 i = 0; i < 5; i++)
                        {
                            cur.sufferCnt = World::stArena.sufferCnt[i];
                            cur.lastTime = World::stArena.lasttime[i];
                            cur.name = World::stArena.name[i];
                            resultRank.insert(cur);
                        }
                        for(UInt8 i = 0; i < 5; i++)
                        {
                            UInt8 j = 0;
                            for(ValueSortType::iterator iter = resultRank.begin(), e = resultRank.end(); iter != e && j < 5; ++iter, ++j)
                            {
                                if(iter->name == World::stArena.name[i])
                                    break;
                            }
                            World::stArena.rank[i] = j + 1;
                        }
                    }
                    Stream st(REP::SERVER_ARENA_EXTRA_ACT);
                    st << week;
                    st << type;
                    st << static_cast<UInt8>(GetVar(VAR_ARENA_SUPPORT));
                    for(UInt8 i = 0; i < 5; i++)
                    {
                        st << World::stArena.sufferCnt[i];
                        st << World::stArena.rank[i];
                    }
                    st << Stream::eos;
                    send(st);
                }
                break;
                 default:
                break;
            }
        }
    bool Player::hasFighterWithClass(UInt8 cls)
    {
        for ( std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++ it)
        {
            if (it->second->getClass() == cls)
                return true;
        }
        return false;
        
    }


    static UInt32 newRecharge[] = {10, 88, 188, 588};
    void Player::FirstRechargeAct(UInt8 step, UInt8 type, UInt8 career)
    {
        if(step == 0 || step > 4)
            return;
        if(type > 1)
            return;
        if(career == 0 || career > 3)
            return;
        UInt32 curStep = GetVar(VAR_FIRST_RECHARGE_STEP);
        /*
         * bit 1: 10XS
         * bit 2: 88XS
         * bit 3: 188XS
         * bit 4: 588XS
         */
        if(curStep & (1 << (step-1)))
            return;
        UInt8 index;
        if(step == 1)
            index = 1 + type;
        else
            index = 1 + career * 2 + type;
        UInt32 goldNum = 0;
        if(type == 1)
        {
            if(step == 1)
                goldNum = 10;
            else
                goldNum = 50;
            if(getGold() < goldNum)
            {
                sendMsgCode(2, 1104, 0);
                return;
            }
        }

        bool bRet = GameAction()->onFirstRecharge(this, index);
        if(bRet)
        {
            if(goldNum > 0)
            {
                ConsumeInfo ci(EnumFirstRecharge1 + step - 1, 0, 0);
                useGold(goldNum, &ci);
            }
            else
            {
                char tag[32];
                sprintf(tag, "F_10000_1212_%u", step);
                udpLog("firstRecharge", tag, "", "", "", "", "act");
            }

            curStep |= (1 << (step-1));
            SetVar(VAR_FIRST_RECHARGE_STEP, curStep);
            sendFirstRecharge();

            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA(this, created));
            if(thisDay <= endDay)
            {
                // 新注册七日内开启首充礼包，完成每日目标
                UInt32 targetVal = GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_RECHARGE_PACKGE))
                {
                    targetVal |=TARGET_RECHARGE_PACKGE;
                    AddVar(VAR_CTS_TARGET_COUNT, 1);
                    SetVar(VAR_CLAWARD2, targetVal);
                    sendNewRC7DayTarget();
                    newRC7DayUdpLog(1152, 11);
                }
            }
        }
    }

    void Player::sendFirstRecharge(bool isLogin)
    {
        UInt32 lostValue = 0;
        UInt8 lostStep = 4;
        UInt8 canStep = 4;
        UInt8 index = 0;

        if(_playerData.totalRecharge < 10)
        {
            Stream st(REP::COUNTRY_ACT);
            st << static_cast<UInt8>(0x03) << index << Stream::eos;
            send(st);
            return;
        }

        //之前充过
        if(_playerData.totalRecharge > GetVar(VAR_FIRST_RECHARGE_VALUE))
        {
            lostValue = _playerData.totalRecharge - GetVar(VAR_FIRST_RECHARGE_VALUE);
        }
        while(lostStep > 0)
        {
            if(newRecharge[lostStep - 1] <= lostValue)
                break;
            --lostStep;
        }

        while(canStep > 0)
        {
            if(newRecharge[canStep - 1] <= _playerData.totalRecharge)
                break;
            --canStep;
        }

        for(index = lostStep + 1; index <= canStep; ++index)
        {
            if((GetVar(VAR_FIRST_RECHARGE_STEP)&(1 << (index-1))) == 0)
                break;
        }

        if(isLogin && index > 4)
            return;
        Stream st(REP::COUNTRY_ACT);
        st << static_cast<UInt8>(0x03) << index << Stream::eos;
        send(st);
    }

void Player::copyFrontWinAward(UInt8 index)
{
    if(!World::getCopyFrontWinSwitch())
        return;
    UInt32 tmp = (GetVar(VAR_CF_BIND)&0x0F);
    UInt32 cf_tmp = ((tmp << 4) | tmp);
    SetVar(VAR_CF_BIND, cf_tmp);
    SetVar(VAR_CF_FLAG, index);
    resetCopyFrontWinAward();
    sendCopyFrontAllAward();
}

void Player::loadCopyFrontWinFromDB(UInt8 posOrig, UInt8 posPut, UInt32 itemId, UInt16 ratio)
{
    if(posOrig >= 5)
        return;
    cf_itemId[posOrig] = itemId;
    cf_ratio[posOrig] = ratio;
    if(posPut <= 5)
        cf_posPut[posOrig] = posPut;
}

void Player::getCopyFrontCurrentAward(UInt8 index)
{
    UInt32 leftIndex[5];
    UInt32 leftCnt = 0;
    UInt8 i;
    for(i = 0; i < 5; i++)
    {
        if(cf_posPut[i] == 0)
            leftIndex[leftCnt++] = i;
    }
    if(leftCnt == 0)
        return;
    UInt16 totalRatio = 0;
    for(i = 0; i < leftCnt; i++)
        totalRatio += cf_ratio[leftIndex[i]];
    UInt16 totalRatioTmp = 0;
    UInt16 curRatio = uRand(totalRatio);
    UInt8 curId = 5;
    for(i = 0; i < leftCnt; i++)
    {
        totalRatioTmp += cf_ratio[leftIndex[i]];
        if(curRatio < totalRatioTmp)
        {
            curId = leftIndex[i];
            break;
        }
    }
    if(curId >= 5)
        return;
    UInt8 order = 5 - leftCnt + 1;
    if(order == 2)
    {
        if(getGoldOrCoupon() < 10)
         {
             sendMsgCode(0, 1101);
             return;
         }
         ConsumeInfo ci(EnumCopyFrontWin, 0, 0);
         useGoldOrCoupon(10, &ci);
    }
    else if(order >= 3)
    {
        UInt32 needGold;
        if(order ==  4)
            needGold = 20;
        else if(order >= 5)
            needGold = 40;
        else
            needGold = 10;
        if(getGold() < needGold)
        {
             sendMsgCode(0, 1104);
             return;
        }
        ConsumeInfo ci(EnumCopyFrontWin, 0, 0);
        useGold(needGold, &ci);
    }
    cf_posPut[curId] = index;
    DB1().PushUpdateData("UPDATE `copy_front_win` SET `posPut` = %u where `playerId` = %"I64_FMT"u and `posOrig` = %u", cf_posPut[curId], getId(), curId);

    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(0x04);
    st << static_cast<UInt8>(0x00);
    st << cf_posPut[curId];
    st << static_cast<UInt16>(cf_itemId[curId]);
    UInt8 curCnt = 1;
    st << curCnt;
    st << Stream::eos;
    send(st);

    UInt32 cf_bind_flag = GetVar(VAR_CF_BIND);
    bool bind;
    if(GetVar(VAR_CF_FLAG) == 1 && (cf_bind_flag&0x10))
        bind = 0;
    else if(GetVar(VAR_CF_FLAG) == 2 && (cf_bind_flag&0x20))
        bind = 0;
    else
        bind = 1;
    UInt32 itemTmp = cf_itemId[curId];
    if(bind)
    {
        //特殊处理4个相同的物品，绑定的全部是后者(前者可以是非全绑定，后者全部是绑定)
        if(itemTmp == 135)
            itemTmp = 133;
        else if(itemTmp == 1412)
            itemTmp = 1327;
        else if(itemTmp == 1411)
            itemTmp = 1326;
        else if(itemTmp == 9283)
            itemTmp = 400;
    }
    m_Package->Add(itemTmp, 1, bind);

    {
        char tag[32];
        if(!bind)
            order += 5;
        if(GetVar(VAR_CF_FLAG) == 2)
            order += 11;
        sprintf(tag, "F_10000_1212_%u", order);
        udpLog("CopyFrontWin", tag, "", "", "", "", "act");
    }

    if(leftCnt == 1)
        closeCopyFrontAwardByIndex(GetVar(VAR_CF_FLAG) - 1, 0);
}

void Player::getCopyFrontAwardByIndex(UInt8 copy_or_front, UInt8 index, UInt8 indexPut)
{
    if(copy_or_front > 1)
        return;
    if(static_cast<UInt32>(copy_or_front + 1) != GetVar(VAR_CF_FLAG))
        return;
#if 0
    if(index !=  PLAYER_DATA(this, location))
        return;
#endif
    if(indexPut == 0 || indexPut > 5)
        return;
    if(GetFreePackageSize() < 1)
    {
        sendMsgCode(0, 1011);
        return;
    }

    UInt8 i;
    bool isPut = false;
    for(i = 0; i < 5; i++)
    {
        if(cf_posPut[i] == indexPut)
        {
            isPut = true;
            break;
        }
    }
    if(isPut)
        return;
    getCopyFrontCurrentAward(indexPut);
}

void Player::resetCopyFrontWinAward(bool fresh)
{
    UInt8 index = GetVar(VAR_CF_FLAG);
    UInt8 step;
    UInt8 tmp1;
    UInt8 tmp2;

    if(index == 0 || index > 2)
        return;

    tmp1 = uRand(5);
    tmp2 = uRand(5);
    UInt32 count = 0;
    while(tmp2 == tmp1 && count < 10000)
    {
        tmp2 = uRand(5);
        ++count;
    }

    for(UInt8 i = 0; i < 5; i++)
    {
        if(i == tmp1)
            step = 1;
        else if(i == tmp2)
            step = 2;
        else
            step = 0;
        if(GetVar(VAR_CF_LOCATION) == 0)
            SetVar(VAR_CF_LOCATION, PLAYER_DATA(this, location));
        Table award = GameAction()->getCopyFrontmapAward(step, GetVar(VAR_CF_LOCATION));
        if (award.size() < 2)
        {
            printf("award.size() < 2\n");
            continue;
        }
        cf_itemId[i] = award.get<UInt32>(1);
        cf_ratio[i] = award.get<UInt32>(2);
        cf_posPut[i] = 0;
        if(fresh)
            DB1().PushUpdateData("UPDATE `copy_front_win` SET `posPut` = %u, `itemId` = %u, `ratio` = %u WHERE `playerId` = %"I64_FMT"u AND `posOrig` = %u", cf_posPut[i], cf_itemId[i], cf_ratio[i], getId(), i);
        else
            DB1().PushUpdateData("REPLACE INTO `copy_front_win` (`playerId`, `posOrig`, `posPut`, `itemId`, `ratio`) VALUES(%"I64_FMT"u, %u, %u, %u, %u)", getId(), i, cf_posPut[i], cf_itemId[i], cf_ratio[i]);
    }
}

void Player::freshCopyFrontAwardByIndex(UInt8 copy_or_front, UInt8 index)
{
    if(copy_or_front > 1)
        return;
    if(static_cast<UInt32>(copy_or_front + 1) != GetVar(VAR_CF_FLAG))
        return;
#if 0
    if(index !=  PLAYER_DATA(this, location))
        return;
#endif
    bool isPut = false;
    for(UInt8 i = 0; i < 5; i++)
    {
        if(cf_posPut[i] != 0)
        {
            isPut = true;
            break;
        }
    }
    if(isPut)
        return;
    if(getTael() < 50)
    {
        sendMsgCode(0, 1100);
        return;
    }
    ConsumeInfo ci(EnumCopyFrontWin, 0, 0);
    useTael(50, &ci);

    resetCopyFrontWinAward(true);
    sendCopyFrontAllAward();
}

void Player::closeCopyFrontAwardByIndex(UInt8 copy_or_front, UInt8 index)
{
    if(copy_or_front > 1)
        return;
    if(static_cast<UInt32>(copy_or_front + 1) != GetVar(VAR_CF_FLAG))
        return;
#if 0
    if(index !=  PLAYER_DATA(this, location))
        return;
#endif
    SetVar(VAR_CF_FLAG, 0);
    SetVar(VAR_CF_INDEX, 0);
    SetVar(VAR_CF_LOCATION, 0);
    for(UInt8 i = 0; i < 5; i++)
    {
        cf_posPut[i] = 0;
        cf_itemId[i] = 0;
        cf_ratio[i] = 0;
        //DB1().PushUpdateData("UPDATE `copy_front_win` SET `posPut` = %u, `itemId` = %u, `ratio` = %u WHERE `playerId` = %"I64_FMT"u AND `posOrig` = %u", cf_posPut[i], cf_itemId[i], cf_ratio[i], getId(), i);
    }
    DB1().PushUpdateData("DELETE FROM `copy_front_win` WHERE `playerId` = %"I64_FMT"u", getId());
}

void Player::sendCopyFrontAllAward()
{
    if(GetVar(VAR_CF_FLAG) == 0)
        return;

    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(0x04);
    st << static_cast<UInt8>(0x01);
    st << static_cast<UInt8>(GetVar(VAR_CF_FLAG) - 1);

    if(GetVar(VAR_CF_FLAG) == 1)
    {
        if(GetVar(VAR_CF_INDEX) == 0)
            SetVar(VAR_CF_INDEX, getCopyId());
        st << static_cast<UInt8>(GetVar(VAR_CF_INDEX));
    }
    else
    {
        if(GetVar(VAR_CF_INDEX) == 0)
            SetVar(VAR_CF_INDEX, getFrontmapId());
        st << static_cast<UInt8>(GetVar(VAR_CF_INDEX));
    }
    st << static_cast<UInt8>(5);
    bool isPut = false;
    UInt8 index;
    UInt8 i;
    UInt16 itemId;
    UInt8 itemCnt = 1;
    for(index = 0; index < 5; index++)
    {
        if(cf_posPut[index] != 0)
        {
            isPut = true;
            break;
        }
    }
    if(!isPut)
    {
        for(index = 0; index < 5; index++)
        {
            itemId = cf_itemId[index];
            st << itemId;
            st << itemCnt;
        }
    }
    else
    {
        for(index = 0; index < 5; index++)
        {
            for(i = 0; i < 5; i++)
            {
                if(cf_posPut[i] == index + 1)
                    break;
            }
            if(i < 5)
                itemId = cf_itemId[i];
            else
                itemId = 0;
            st << itemId;
            st << itemCnt;
        }
    }

    st << Stream::eos;
    send(st);
}

UInt8 Player::getCopyId()
{
    static UInt16 spots[] = {776, 2067, 5906, 8198, 12818, 10512, 0x1411};

    UInt16 currentSpot = PLAYER_DATA(this, location);
    for(UInt8 i = 0; i < sizeof(spots)/sizeof(spots[0]); i++)
    {
        if(spots[i] == currentSpot)
            return (i+1);
    }
    return 0;
}

UInt8 Player::getFrontmapId()
{
    static UInt16 spots[] = {1284, 2053, 4360, 4611, 5893, 5637, 8195, 6153, 9222, 9481, 10244, 5129};
    UInt16 currentSpot = PLAYER_DATA(this, location);
    for(UInt8 i = 0; i < sizeof(spots)/sizeof(spots[0]); i++)
    {
        if(spots[i] == currentSpot)
            return (i+1);
    }
    return 0;
}

void Player::getGoodVoiceAward(UInt8 type)
{
    if(type == 1 && GetVar(VAR_GOOD_VOICE) == 0)
    {
        if(GetFreePackageSize() < 1)
        {
            sendMsgCode(0, 1011);
            return;
        }
        SetVar(VAR_GOOD_VOICE, 1);
        m_Package->Add(9273, 1, true);
        sendGoodVoiceInfo();
    }
}

void Player::sendGoodVoiceInfo()
{
    if(!World::getGoodVoiceAct())
        return;
    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(5);
    st << static_cast<UInt8>(GetVar(VAR_GOOD_VOICE));
    st << Stream::eos;
    send(st);
}

void Player::get3366GiftAward(UInt8 type)
{
    if(type == 1 && GetVar(VAR_3366GIFT) < 99)
    {
        if(GetFreePackageSize() < 6)
        {
            sendMsgCode(0, 1011);
            return;
        }
        if(getGold() < 368)
        {
            sendMsgCode(0, 1104);
            return;
        }
		ConsumeInfo ci(Enum3366Gift,0,0);
		useGold(368,&ci);
        AddVar(VAR_3366GIFT, 1);
        m_Package->Add(500, 2, true);
        m_Package->Add(501, 2, true);
        m_Package->Add(1325, 2, true);
        m_Package->Add(516, 2, true);
        m_Package->Add(134, 2, true);
        m_Package->Add(515, 2, true);
        send3366GiftInfo();
    }
}

void Player::send3366GiftInfo()
{
    if(getPlatform() != 11)
        return;
    if(!isBD())
        return;
    if(!World::get3366GiftAct())
        return;
    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(6);
    UInt8 opt = GetVar(VAR_3366GIFT);
    /*
    if(GetVar(VAR_3366GIFT) < 9)
        opt = 0;
    else
        opt = 1;
    */
    st << opt;
    st << Stream::eos;
    send(st);
}

void Player::sendQQGameGift1218()
{
    UInt8 platform = atoi(getDomain());
    if (GetVar(VAR_QQGAME_GIFT_1218) > 0 || platform != 10)
        return;
    UInt32 now = TimeUtil::Now();
    if (now < TimeUtil::MkTime(2012, 12, 18) || now > TimeUtil::MkTime(2012, 12, 21))
        return;
    UInt32 h = (now - TimeUtil::SharpDay())/3600;//现在的小时
    if (h == 20)
    {
        SYSMSGV(title, 4100, TimeUtil::MonthDay());
        SYSMSGV(content, 4101, TimeUtil::MonthDay());
        Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            MailPackage::MailItem mitem = {15,5};
            mailPackageManager.push(mail->id, &mitem, 1, true);
        }
        SetVar(VAR_QQGAME_GIFT_1218, 1);
    }
}

void Player::sendFeastLoginAct()
{
    if(GetLev() < 40 || GetVar(VAR_FEAST_LOGIN) > 0 || !World::getFeastLoginAct())
        return;
    SYSMSGV(title, 4102);
    SYSMSGV(content, 4103);
    Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
    if(mail)
    {
        MailPackage::MailItem mitem = {1759,1};
        mailPackageManager.push(mail->id, &mitem, 1, true);
    }
    SetVar(VAR_FEAST_LOGIN, 1);
}

void Player::sendTowerLoginAct()
{
    UInt8 day = World::getTowerLoginAct();
    if (day > 7) return;

    if (day && strcasestr(m_via.c_str(), "sscq_dlhd") && !GetVar(VAR_TOWER_LOGIN))
    {
        SYSMSGV(title, 4104);
        SYSMSGV(content, 4105);
        Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {

         MailPackage::MailItem item1[2] ={{56,3},   {57,3}};
         MailPackage::MailItem item2[2] ={{508,1},  {506,1}};
         MailPackage::MailItem item3[2] ={{56,3},   {57,3}};
         MailPackage::MailItem item4[2] ={{508,1},  {506,1}};
         MailPackage::MailItem item5[2] ={{56,3},   {57,3}};
         MailPackage::MailItem item6[2] ={{508,1},  {506,1}};
         MailPackage::MailItem item7[2] ={{509,1},  {507,1}};

         MailPackage::MailItem* item[7] = {item1,item2,item3,item4,item5,item6,item7};

         mailPackageManager.push(mail->id, item[day - 1], 2, true);
        }
        SetVar(VAR_TOWER_LOGIN, 1);

        if(!GetVar(VAR_TOWER_LEVEL) && GetLev() >= 40)
        {
            SYSMSGV(title, 4106);
            SYSMSGV(content, 4107);
            Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if(mail)
            {
                MailPackage::MailItem mitem[6] = {{516,1},{509,1},{507,1},{56,3},{57,3},{503,1}};
                mailPackageManager.push(mail->id, mitem, 6, true);
            }
            SetVar(VAR_TOWER_LEVEL, 1);
        }
    }

}

void Player::getFeastGiftAward(UInt8 type)
{
    if(type == 0 || type > 2)
        return;
    bool bRet;
    UInt32 status = GetVar(VAR_FEAST_GIFT);

    if(type == 1)
    {
        if(status & 0x01)
            return;
        bRet = GameAction()->onGetFeastGiftAward(this, type);
        if(bRet)
        {
            udpLog("huodong", "F_10000_15", "", "", "", "", "act");
            status |= 0x01;
            SetVar(VAR_FEAST_GIFT, status);
            sendFeastGiftAct();
        }
    }
    else
    {
        if(status & 0x02)
            return;
        if(getGold() < 30)
        {
            sendMsgCode(0, 1104);
            return;
        }
        bRet = GameAction()->onGetFeastGiftAward(this, type);
        if(bRet)
        {
            ConsumeInfo ci(EnumFEASTGIFT,0,0);
            useGold(30,&ci);
            status |= 0x02;
            SetVar(VAR_FEAST_GIFT, status);
            sendFeastGiftAct();
        }
    }
}

void Player::sendFeastGiftAct()
{
    if(!World::getFeastLoginAct())
        return;
    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(7);
    UInt8 opt = GetVar(VAR_FEAST_GIFT);
    st << opt;
    st << Stream::eos;
    send(st);
}

Dreamer* Player::getDreamer()
{
       if (GetVar(VAR_EX_JOB_ENABLE) < 2)
           return NULL;
       if (!_dreamer)
       {
           _dreamer = new Dreamer(this);
       }
       return _dreamer;
}

void Player::setDreamerTime(UInt8 count)
{
    if (!_dreamer)
        return;
    _dreamer->SetTime(count);
}

void Player::setDreamerEye(UInt8 count)
{
    if (!_dreamer)
        return;
    _dreamer->SetEye(count);
}

void Player::setDreamerKey(UInt8 count)
{
    if (!_dreamer)
        return;
    _dreamer->SetKey(count);
}

void Player::sendSysUpdate()
{
   //版本更新公告
   Stream st(REP::SYSDAILOG);
   st << static_cast<UInt8>(1);
   st << static_cast<UInt8>(1); //0:老版本 1:最新版
   st << static_cast<UInt8>(1); //0:按钮不显示 1:按钮显示
   st << (char*)VERSION;
   st << Stream::eos;
   send(st);
}

void Player::sendSnakeEggInfo()
{
    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(0x08) << static_cast<UInt8>(0x01);
    UInt8 t = World::getCallSnakeEggAct(); //0:非活动期间 1:拜年期 2:拜年-领奖等待期
    if (World::getSnakeEggAwardAct() >= 1 && World::getSnakeEggAwardAct() <= 7)
        t = 3;
    else if (World::getSnakeEggAwardAct() == 0xFF)
        t = 4;
    st << t;
    st << static_cast<UInt8>(GetVar(VAR_CALLSNAKEEGG)) << static_cast<UInt8>(World::getSnakeEggAwardAct());
    st << static_cast<UInt8>(GetVar(VAR_SNAKEEGG_AWARD));
    st << Stream::eos;
    send(st);
 
}

void Player::callSnakeEgg()
{
  if (!World::getCallSnakeEggAct() || GetVar(VAR_CALLSNAKEEGG) != 0 )
      return;
  SetVar(VAR_CALLSNAKEEGG, 1);
  sendSnakeEggInfo();
}

void Player::getSnakeEggAward(UInt8 v)
{
    UInt8 day = World::getSnakeEggAwardAct();
    if (!day || v > 7 || v > day)
        return;
    UInt8 var = GetVar(VAR_SNAKEEGG_AWARD); 
    if (var & (0x01<<(v-1))) //已领取
        return;
    if (v < day || GetVar(VAR_CALLSNAKEEGG) == 0)
    {
        if(!hasChecked())
            return;
        if (getGold() < 30)
        {
		    sendMsgCode(0, 1104);
            return;
        }
        ConsumeInfo ci(SnakeSprintAct, 0, 0);
        useGold(30, &ci);
    }
    getCoupon(100);
    var |= (0x01<<(v-1));
    SetVar(VAR_SNAKEEGG_AWARD, var);
    sendSnakeEggInfo();
}

void Player::getNewYearGiveGiftAward(UInt8 dayOrder, UInt8 result)
{
    if(dayOrder > 10)
        return;

    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(8);
    //st << dayOrder;

    UInt16 opt = GetVar(VAR_NEW_YEAR_GIVE_GIFT);
    UInt16 offset = 1;
    switch(dayOrder)
    {
        case 0:
        break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        {
            offset <<= (dayOrder - 1);
            if((opt & offset) > 0)
            {
                sendMsgCode(0, 1018);
                return;
            }

            UInt8 validMaxDay = 0;
            UInt8 serverDay = 0;
            UInt32 now = TimeUtil::Now();
            if(TimeUtil::SharpDay(0, now) < TimeUtil::MkTime(2013, 2, 3))
            {
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 3))
            {
                validMaxDay = 1;
                serverDay = 1;
            }
            else if(TimeUtil::SharpDay(0, now) < TimeUtil::MkTime(2013, 2, 9))
            {
                validMaxDay = 1;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 9))
            {
                validMaxDay = 2;
                serverDay = 2;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 10))
            {
                validMaxDay = 3;
                serverDay = 3;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 11))
            {
                validMaxDay = 4;
                serverDay = 4;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 12))
            {
                validMaxDay = 5;
                serverDay = 5;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 13))
            {
                validMaxDay = 6;
                serverDay = 6;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 14))
            {
                validMaxDay = 7;
                serverDay = 7;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 15))
            {
                validMaxDay = 8;
                serverDay = 8;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 16))
            {
                validMaxDay = 9;
                serverDay = 9;
            }
            else if(TimeUtil::SharpDay(0, now) < TimeUtil::MkTime(2013, 2, 24))
            {
                validMaxDay = 9;
            }
            else if(TimeUtil::SharpDay(0, now) == TimeUtil::MkTime(2013, 2, 24))
            {
                validMaxDay = 10;
                serverDay = 10;
            }
            else
            {
                validMaxDay = 10;
            }

            if(dayOrder > validMaxDay)
            {
                sendMsgCode(0, 1041);
                return;
            }

            UInt8 times;
            UInt8 recharge = false;
            if(serverDay == dayOrder)
                times = 2;
            else
            {
                if(result == 0)
                {
                    sendMsgCode(0, 1042);
                    return;
                }
                else if(result == 1)
                {
                    if(!hasChecked())
                        return;
                    if (getGold() < 30)
                    {
                        sendMsgCode(0, 1104);
                        return;
                    }
                    times = 2;
                    recharge = true;
                }
                else
                    times = 1;
            }

            bool bRet = GameAction()->onGetNewYearGiveGiftAward(this, dayOrder, times);
            if(!bRet)
                return;
            if(recharge)
            {
                ConsumeInfo ci(NewYearGetDouble,0,0);
                useGold(30, &ci);
            }
            opt |= offset;
            SetVar(VAR_NEW_YEAR_GIVE_GIFT, opt);
        }
        break;

        default:
        break;
    }
    st << opt;
    st << Stream::eos;
    send(st);
}
void Player::sendSnakeSpringEquipMail()
{
    if(GetLev() < 40)
        return;
    static MailPackage::MailItem s_item[2] = {{1762, 1}, {1764, 1}};
    UInt8 act = World::getSnakeSpringEquipAct();
    UInt32 v = GetVar(VAR_SNAKE_SPRING_EQUIP_GOT);
    if (1 == act && ((v&0x01)==0)) //春节套装 item=1762
    {
        sendMailItem(4126, 4127, &s_item[0], 1);
        SetVar(VAR_SNAKE_SPRING_EQUIP_GOT, v|=0x01);
    }
    if (2 == act && ((v&0x02)==0))
    {
        sendMailItem(4128, 4129, &s_item[1], 1);
        SetVar(VAR_SNAKE_SPRING_EQUIP_GOT, v|=0x02);
    }
}

void Player::getNewYearQQGameAward(UInt8 type)
{
    if(type == 0 || type > 2)
        return;
    if(atoi(m_domain) != 10)
        return;
    bool bRet;
    UInt32 status = GetVar(VAR_NEWYEAR_QQGAME_ACT);

    if(type == 1)
    {
        if(status & 0x01)
            return;
        bRet = GameAction()->onGetNewYearQQGameAward(this, 1);
        if(bRet)
        {
            status |= 0x01;
            SetVar(VAR_NEWYEAR_QQGAME_ACT, status);
            sendNewYearQQGameAct();
        }
    }
    else
    {
        if(status & 0x02)
            return;
        if(!isBD())
            return;
        bRet = GameAction()->onGetNewYearQQGameAward(this, 2);
        if(bRet)
        {
            status |= 0x02;
            SetVar(VAR_NEWYEAR_QQGAME_ACT, status);
            sendNewYearQQGameAct();
        }
    }
}

void Player::getQZoneQQGameAward(UInt8 domainType, UInt8 type)
{
    if(domainType == 1)
    {
        if(atoi(m_domain) != 1 && atoi(m_domain) != 2)
            return;
        if(type == 0 || type > 2)
            return;
        bool bRet;
        UInt32 status = GetVar(VAR_QZONE_QQGAME_ACT);
        if(type == 1)
        {
            if(status & 0x01)
                return;
            bRet = GameAction()->onGetQZoneQQGameAward(this, 1);
            if(bRet)
            {
                status |= 0x01;
                SetVar(VAR_QZONE_QQGAME_ACT, status);
                sendQZoneQQGameAct(domainType);
            }
        }
        else
        {
            if(status & 0x02)
                return;
            if(!isYD())
                return;
            bRet = GameAction()->onGetQZoneQQGameAward(this, 2);
            if(bRet)
            {
                status |= 0x02;
                SetVar(VAR_QZONE_QQGAME_ACT, status);
                sendQZoneQQGameAct(domainType);
            }
        }
    }
    else if(domainType == 2)
    {
        if(atoi(m_domain) != 10)
            return;
        if(type == 0 || type > 2)
            return;
        bool bRet;
        UInt32 status = GetVar(VAR_QZONE_QQGAME_ACT);
        if(type == 1)
        {
            if(status & 0x04)
                return;
            bRet = GameAction()->onGetQZoneQQGameAward(this, 1);
            if(bRet)
            {
                status |= 0x04;
                SetVar(VAR_QZONE_QQGAME_ACT, status);
                sendQZoneQQGameAct(domainType);
            }
        }
        else
        {
            if(status & 0x08)
                return;
            if(!isBD())
                return;
            bRet = GameAction()->onGetQZoneQQGameAward(this, 2);
            if(bRet)
            {
                status |= 0x08;
                SetVar(VAR_QZONE_QQGAME_ACT, status);
                sendQZoneQQGameAct(domainType);
            }
        }
    }
}

void Player::sendQZoneQQGameAct(UInt8 domainType)
{
    if(!World::getQZoneQQGameAct())
        return;
    if(domainType == 1)
    {
        if(atoi(m_domain) != 1 && atoi(m_domain) != 2)
            return;
    }
    else if(domainType == 2)
    {
        if(atoi(m_domain) != 10)
            return;
    }
    else
        return;
    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(0x0C);
    st << domainType;
    UInt8 opt = GetVar(VAR_QZONE_QQGAME_ACT);
    if(domainType == 1)
        opt = opt & 0x3;
    else
        opt = (opt >> 2) & 0x03;
    st << opt;
    st << Stream::eos;
    send(st);
}

void Player::sendNewYearQQGameAct()
{
    if(!World::getNewYearQQGameAct())
        return;
    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(9);
    UInt8 opt = GetVar(VAR_NEWYEAR_QQGAME_ACT);
    st << opt;
    st << Stream::eos;
    send(st);
}

void Player::getNewYearQzoneContinueAward(UInt8 type)
{
    if(type == 0 || type > 7)
        return;
    if(atoi(m_domain) != 1 && atoi(m_domain) != 2)
        return;

    UInt32 tmp = GetVar(VAR_NEWYEAR_QZONECONTINUE_ACT);
    UInt16 isGet = static_cast<UInt16>(tmp & 0xFFFF);
    if(isGet & (0x01 << (type - 1)))
        return;
    UInt8 continueDays = static_cast<UInt8>(tmp >> 16);
    const static UInt8 needMinDay[] = {3, 5, 7, 10, 15, 21, 28};
    if(continueDays < needMinDay[type - 1])
        return;
    bool bRet = GameAction()->onGetNewYearQzoneContinueAward(this, type);
    if(bRet)
    {
        tmp |= (0x01 << (type - 1));
        SetVar(VAR_NEWYEAR_QZONECONTINUE_ACT, tmp);
        sendNewYearQzoneContinueAct();
    }
}

void Player::sendNewYearQzoneContinueAct()
{
    if(!World::getNewYearQzoneContinueAct())
        return;
    if(atoi(m_domain) != 1 && atoi(m_domain) != 2)
        return;

    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(10);
    UInt32 tmp = GetVar(VAR_NEWYEAR_QZONECONTINUE_ACT);
    UInt8 continueDays = static_cast<UInt8>(tmp >> 16);
    UInt16 isGet = static_cast<UInt16>(tmp & 0xFFFF);
    st << continueDays;
    st << isGet;
    st << Stream::eos;
    send(st);
}

void Player::calcNewYearQzoneContinueDay(UInt32 now)
{
    if(!World::getNewYearQzoneContinueAct())
        return;
    if(atoi(m_domain) != 1 && atoi(m_domain) != 2)
        return;

    UInt32 lasttime = GetVar(VAR_NEWYEAR_QZONECONTINUE_LASTTIME);
    if(lasttime == 0)
    {
    }
    else
    {
        UInt32 lasttime_sharp = TimeUtil::SharpDay(0, lasttime);
        UInt32 now_sharp = TimeUtil::SharpDay(0, now);
        if(lasttime_sharp >= now_sharp)
            return;
    }

    SetVar(VAR_NEWYEAR_QZONECONTINUE_LASTTIME, now);
    UInt32 tmp = GetVar(VAR_NEWYEAR_QZONECONTINUE_ACT);
    UInt32 continueDays = (tmp >> 16);
    if(continueDays >= 0xFF)
        continueDays = 0xFE;
    continueDays += 1;
    tmp = (continueDays << 16) + (tmp & 0xFFFF);
    SetVar(VAR_NEWYEAR_QZONECONTINUE_ACT, tmp);
}


/*
 *flag == 1,2,3
 *1:大闹龙宫
 *2:大闹龙宫之金蛇起舞
 *3:大闹龙宫之天芒神梭
*/
static UInt8 Dragon_type[]  = { 0xFF, 0x06, 0x0A, 0x0B, 0x0D, 0x0F };
static UInt32 Dragon_Ling[] = { 0xFFFFFFFF, 9337, 9354, 9358, 9364, 9372 };
//6134:龙神秘典残页 6135:金蛇宝鉴残页 136:天芒神梭碎片 6136:混元剑诀残页
static UInt32 Dragon_Broadcast[] = { 0xFFFFFFFF, 6134, 6135, 136, 6136, 1357 };
void Player::getDragonKingInfo()
{
    if(TimeUtil::Now() > GVAR.GetVar(GVAR_DRAGONKING_END)
            && GVAR.GetVar(GVAR_DRAGONKING_ACTION) != DRAGONKING_CLOSE)
    {
        GVAR.SetVar(GVAR_DRAGONKING_ACTION, 0);
        GVAR.SetVar(GVAR_DRAGONKING_BEGIN, 0);
        GVAR.SetVar(GVAR_DRAGONKING_END, 0);
        return;
    }
    UInt8 flag = GVAR.GetVar(GVAR_DRAGONKING_ACTION);
    if (flag <= DRAGONKING_CLOSE || flag >= DRAGONKING_MAX
            || flag+1 > sizeof(Dragon_type)/sizeof(UInt8))
    {
        sendMsgCode(0, 1090);
        return;
    }
    UInt8 step = GetVar(VAR_DRAGONKING_STEP);
    if( step == 0 || step > 5)
        step = 1;
    Stream st(REP::ACTIVE);
    st << Dragon_type[flag] << static_cast<UInt8>(0x01);
    st << step << Stream::eos;
    send(st);
}

void Player::postDragonKing(UInt8 count)
{
    if (CURRENT_THREAD_ID() != getThreadId())
    {
        GameMsgHdr h(0x342,  getThreadId(), this, sizeof(count));
        GLOBAL().PushMsg(h, &count);
        return;
    }
    if(TimeUtil::Now() > GVAR.GetVar(GVAR_DRAGONKING_END)
            && GVAR.GetVar(GVAR_DRAGONKING_ACTION) != DRAGONKING_CLOSE)
    {
        GVAR.SetVar(GVAR_DRAGONKING_ACTION, 0);
        GVAR.SetVar(GVAR_DRAGONKING_BEGIN, 0);
        GVAR.SetVar(GVAR_DRAGONKING_END, 0);
        return;
    }
    if (count == 0) return;
    UInt8 flag = GVAR.GetVar(GVAR_DRAGONKING_ACTION);
    if (flag <= DRAGONKING_CLOSE || flag >= DRAGONKING_MAX
           || flag+1 > sizeof(Dragon_type)/sizeof(UInt8))
    {
        sendMsgCode(0, 1090);
        return;
    }
    UInt32 XBLing = Dragon_Ling[flag];
    if (GetPackage()->GetItemAnyNum(XBLing) < count)
        return;
    if (GetPackage()->GetRestPackageSize() < count)
    {
        sendMsgCode(0, 1011);
        return;
    }
    GetPackage()->DelItemSendMsg(XBLing, this);
    Stream st(REP::ACTIVE);
    st << Dragon_type[flag] << static_cast<UInt8>(0x02) << count;
    UInt8 step = GetVar(VAR_DRAGONKING_STEP);
    if(step == 0 || step > 5)
        step = 1;
    bool isBind = true;
    for(UInt8 i = 0; i < count; ++i)
    {
        GetPackage()->DelItemAny(XBLing, 1, &isBind);
        Table award = GameAction()->getDragonKingAward(step, flag);
        if (GameAction()->checkDragonKingCanSucceed(this, step, flag))
            step = (step + 1) > 5 ? 1 : step + 1;
        else
            step = 1;
        st << step;
        UInt8 size = award.size();
        st << static_cast<UInt8>(size / 2);
        for(UInt8 j = 1; j <= size; j += 2)
        {
            UInt16 itemId = award.get<UInt16>(j);
            st << itemId << award.get<UInt8>(j+1);
            GetPackage()->Add(itemId, award.get<UInt32>(j+1), isBind, true, FromQixi);
            if (itemId == Dragon_Broadcast[flag])
                SYSMSG_BROADCASTV(295, getCountry(), getName().c_str(), itemId);
        }
    }
    st << Stream::eos;
    send(st);
    SetVar(VAR_DRAGONKING_STEP, step);
}

//金蛇献瑞 聚福兆祥
void Player::saveGoldAct(UInt8 opt, UInt32 param)
{
#define GATHER_TIME 7 * 86400
#define ALL_TIMES 10
    switch(opt)
    {
    case 0x01:  //查看信息
        sendSaveGoldAct();
        break;
    case 0x02:  //是否结算
        {
            if(!World::getSaveGoldAct())
                return;
            if(param)
            {
                if(GetVar(VAR_SAVEGOLD_SET_TIME))
                    return;
                SetVar(VAR_SAVEGOLD_SET_TIME, TimeUtil::Now());
            }
            else
            {
                if(!GetVar(VAR_SAVEGOLD_SET_TIME))
                    return;
                SetVar(VAR_SAVEGOLD_SET_TIME, 0);
            }
            sendSaveGoldAct();
        }
        break;
    case 0x03:  //存仙石
        {
            if(!hasChecked())
                return;
            if(!World::getSaveGoldAct())
                return;
            if(GetVar(VAR_SAVEGOLD_SET_TIME))
                return;
            if(getGold() < param)
                return;
            if(!param || param % ALL_TIMES)
                return;
            AddVar(VAR_SAVEGOLD_COUNT, param);
		    useGold(param);
            sendSaveGoldAct();
            TRACE_LOG("此次存仙石数量playerId_num:SaveGoldAction_%"I64_FMT"u_%u", getId(), param);
        }
        break;
    case 0x04:  //领取福囊
        {
            UInt32 setTime = GetVar(VAR_SAVEGOLD_SET_TIME);
            if(!setTime) return;
            if(setTime + GATHER_TIME > TimeUtil::Now())
                return;
            if(GetVar(VAR_SAVEGOLD_ISGET))
                return;
            UInt32 status = GetVar(VAR_SAVEGOLD_GET_STATUS);
            UInt8 cnt = 0;
            for(UInt8 i = 0; i < ALL_TIMES; ++i)
            {
                if(status & (1 << i))
                    cnt ++;
            }
            if(cnt >= ALL_TIMES)
                return;
            UInt32 gold = GetVar(VAR_SAVEGOLD_COUNT);
            Table awards = GameAction()->getSaveGoldActAward(gold);
            UInt8 size = awards.size() + 1;
            MailPackage::MailItem * mitem = new MailPackage::MailItem[size];
            for(UInt8 i = 0; i < size; ++i)
            {
                if(i == size - 1)
                {
                    mitem[i].id = MailPackage::Gold;
                    mitem[i].count = gold / ALL_TIMES;
                }
                else
                {
                    Table item = awards.get<Table>(i+1);
                    if(item.size() < 2) continue;
                    UInt16 itemId = item.get<UInt16>(1);
                    mitem[i].id = itemId == COUPON_ID ? MailPackage::Coupon : itemId;
                    mitem[i].count = item.get<UInt32>(2);
                }
            }
            SYSMSGV(title, 296);
            SYSMSGV(content, 297, cnt+1, ALL_TIMES);
            MailItemsInfo itemsInfo(mitem, Activity, size);
            Mail * pmail = GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
            if(pmail != NULL)
                mailPackageManager.push(pmail->id, mitem, size, true);
            delete [] mitem;
            mitem = NULL;
            if(cnt + 1 == ALL_TIMES)
            {
                Table extraAwards = GameAction()->getSaveGoldActExtraAward(gold);
                UInt8 size = extraAwards.size();
                MailPackage::MailItem * mitem = new MailPackage::MailItem[size];
                for(UInt8 i = 0; i < size; ++i)
                {
                    Table item = extraAwards.get<Table>(i+1);
                    if(item.size() < 2) continue;
                    UInt16 itemId = item.get<UInt16>(1);
                    mitem[i].id = itemId == COUPON_ID ? MailPackage::Coupon : itemId;
                    mitem[i].count = item.get<UInt32>(2);
                }
                SYSMSGV(title, 298);
                SYSMSGV(content, 299, ALL_TIMES);
                MailItemsInfo itemsInfo(mitem, Activity, size);
                Mail * pmail = GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
                if(pmail != NULL)
                    mailPackageManager.push(pmail->id, mitem, size, true);
                delete [] mitem;
                mitem = NULL;
            }
            SetVar(VAR_SAVEGOLD_GET_STATUS, status | (1 << cnt));
            SetVar(VAR_SAVEGOLD_ISGET, 1);
            sendSaveGoldAct();
        }
        break;
    default:    //零点自动结算
        {
            if(getCreated() >= TimeUtil::MkTime(2013, 2, 9))
                return;
            if(!GetVar(VAR_SAVEGOLD_SET_TIME))
                SetVar(VAR_SAVEGOLD_SET_TIME, TimeUtil::Now());
            if(isOnline())
                sendSaveGoldAct();
        }
        break;
    }
}

void Player::sendSaveGoldAct()
{
    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(0x07);
    st << static_cast<UInt8>(0x01);
    st << GetVar(VAR_SAVEGOLD_COUNT);
    UInt32 curTime = TimeUtil::Now();
    UInt32 setTime = GetVar(VAR_SAVEGOLD_SET_TIME);
    if(setTime > curTime)
        setTime = curTime;
    int time = -1;
    if(setTime)
    {
        time = GATHER_TIME + setTime - curTime;
        if(time < 0)
            time = 0;
    }
    st << time;
    if(time == 0)
    {
        UInt32 status = GetVar(VAR_SAVEGOLD_GET_STATUS);
        UInt8 cnt = 0;
        for(UInt8 i = 0; i < ALL_TIMES; ++i)
        {
            if(status & (1 << i))
                cnt ++;
        }
        if(cnt >= ALL_TIMES)
            st << static_cast<UInt8>(1);
        else
            st << static_cast<UInt8>(GetVarNow(VAR_SAVEGOLD_ISGET, curTime + 30));
        st << status;
    }
    st << Stream::eos;
    send(st);
}
void Player::buyTownTjItem(const UInt32 itemId)
{
    static const UInt32 s_items[] = {1653,1654,1655,1532,1533,1534};
    int opt = -1;
    for (UInt8 i = 0; i < sizeof(s_items)/sizeof(s_items[0]); ++i)
    {
        if (itemId == s_items[i])
        {
            opt = i;
            break;
        }
    }
    if (opt < 0)
        return;
    int flag = GetVar(VAR_TJ_TOWN_ITEM_GOT);
    if (flag & (1 << opt))//已领取
    {
        std::vector<ItemEquip*> items;
        std::vector<ItemEquip*> fgtItems;
        GetPackage()->FindEquipByTypeId(items, itemId, true);
        if (!items.empty()) 
        {
            sendMsgCode(0, 1044);
            return;
        }
        for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++it)
        {
            fgtItems.clear();
            Fighter* fgt = it->second;
            fgt->findTrumpByTypeId(fgtItems, itemId);
            if (!fgtItems.empty())
            {
                sendMsgCode(0, 1044);
                return;
            }
        }
        if (getGold() < 100)
        {
            sendMsgCode(0, 1104); 
            return;
        }
        if (GetPackage()->GetRestPackageSize() == 0)
        {
            sendMsgCode(0, 1011); 
            return;
        }
        ConsumeInfo ci(BuyTownTjTrump,0,0);
		useGold(100,&ci);
        GetPackage()->Add(itemId,1,true);
        sendTownTjItemInfo();
    }
}
void Player::sendTownTjItemInfo()
{
    static const UInt32 s_items[] = {1653,1654,1655,1532,1533,1534};
    UInt8 flag = GetVar(VAR_TJ_TOWN_ITEM_GOT);
    std::vector<ItemEquip*> items;
    std::vector<ItemEquip*> fgtItems;
    for (UInt8 i = 0; i < sizeof(s_items)/sizeof(s_items[0]); ++i) 
    {
        if (flag&(1<<i))
        {
            items.clear();
            GetPackage()->FindEquipByTypeId(items,s_items[i], true);
            if (!items.empty())
            {
                flag &= ~(1<<i);
                continue;
            }
            for(std::map<UInt32, Fighter *>::iterator it = _fighters.begin(); it != _fighters.end(); ++it)
            {
                fgtItems.clear();
                Fighter* fgt = it->second;
                fgt->findTrumpByTypeId(fgtItems, s_items[i]);
                if (!fgtItems.empty())
                {
                    flag &= ~(1<<i);
                    continue;
                }
            }
        }
    }
    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(0x0C) << static_cast<UInt8>(0x01) << static_cast<UInt8>(flag);
    st << Stream::eos;
    send(st);
}

void Player::getLongyuanAct(UInt8 idx, UInt8 flag/*0:领取 1:结缘*/)
{
    static UInt32 s_lev[] = {60,70,80,90,100,110};
    static UInt32 s_longyuan[] = {50000,150000,300000,450000,500000,600000};
    static UInt32 s_fengsui[] = {10000,50000,100000,200000,500000,1000000};
    static UInt32 s_gold[] = {50,200,500,1000,3000,5000};
    if (!World::getLongyuanAct())
        return;
    if (idx > 5)
        return;
    UInt32 v = GetVar(VAR_LONGYUAN_GOT);
    UInt8 res = 0;
    if (GetLev() >= s_lev[idx])
    {
        UInt8 i = idx*2 + flag; //每2位存一个等级的标志 第1位:领取 第2位:结缘
        if (v&(0x01<<i))
        {
            res = 1; //已领取
        }
        else
        {
            if (flag == 1)
            {
                if (getGold() < s_gold[idx])
                {
                    sendMsgCode(0, 1104); 
                    return;
                }
                ConsumeInfo ci(LongYuanAct,0,0);
                useGold(s_gold[idx],&ci);
           }
           IncommingInfo ii1(LongYuanAct, 0, 0);
           getLongyuan(s_longyuan[idx], &ii1);
           IncommingInfo ii2(LongYuanAct, 0, 0);
           getFengsui(s_fengsui[idx], &ii2);
           sendFairyPetResource();

           v |= (0x01<<i);
           SetVar(VAR_LONGYUAN_GOT, v);
           sendLongyuanActInfo();
        }
    }
    else
    {
        res = 3;
    }
}
void Player::sendLongyuanActInfo()
{
    static UInt32 s_lev[] = {60,70,80,90,100,110};
    UInt8 total = 0;
    UInt8 i = 0;
    for (i = 0; i < sizeof(s_lev)/sizeof(s_lev[0]); ++i)
    {
        if (GetLev() < s_lev[i])
            break;
        total += 1;
    }
    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(0x0E) << static_cast<UInt8>(0x01);
    st << static_cast<UInt8>(total);
    UInt32 v = GetVar(VAR_LONGYUAN_GOT);
    for (i = 0; i < total; ++i)
    {
        UInt8 flag = 0;
        flag = (v>>(i*2))&0x03;
        st << flag;
    }
    st << Stream::eos;
    send(st);
}
void Player::transferExpBuffer2Var()
{
    UInt32 tm = TimeUtil::Now();
    UInt32 left;
    UInt32 total;
    /** 初级**/
    total = 0;
    if((left = getBuffLeft(PLAYER_BUFF_TRAINP1, tm))> 0)
    {
        total += left;
        setBuffData(PLAYER_BUFF_TRAINP1, 0);
    };
    if(total > 0)
    {
        SetVar(VAR_TRAINP1, total);
        if(hasFlag(Training))
            SetVar(VAR_EXP_HOOK_INDEX, ENUM_TRAINP1);
    }

    /** 高级 **/
    total = 0;
    if((left = getBuffLeft(PLAYER_BUFF_TRAINP2, tm)) > 0)
    {
        total += left;
        setBuffData(PLAYER_BUFF_TRAINP2, 0);
    }
    if((left = getBuffLeft(PLAYER_BUFF_TRAINP4, tm)) > 0)
    {
        total += left;
        setBuffData(PLAYER_BUFF_TRAINP4, 0);
    }
    if((left = getBuffLeft(PLAYER_BUFF_ADVANCED_HOOK, tm)) > 0)
    {
        total += left;
        setBuffData(PLAYER_BUFF_ADVANCED_HOOK, 0);
    }
    if(total > 0)
    {
        SetVar(VAR_TRAINP2, total);
        if(hasFlag(Training))
            SetVar(VAR_EXP_HOOK_INDEX, ENUM_TRAINP2);
    }

    /** 齐天 **/
    total = 0;
    if((left = getBuffLeft(PLAYER_BUFF_TRAINP3, tm)) > 0)
    {
        total += left;
        setBuffData(PLAYER_BUFF_QI_TIAN_CHU_MO, tm + left);
    }
    if(total > 0)
    {
        SetVar(VAR_TRAINP3, total);
        if(hasFlag(Training))
            SetVar(VAR_EXP_HOOK_INDEX, ENUM_TRAINP3);
    }
}

//////////////////////////////////////////////////////
UInt8 Player::toQQGroup(bool isJoin)
{
    if (getClan() == NULL)
        return 1;
    if (_inQQGroup != isJoin)
    {
        _inQQGroup = isJoin;
	    DB1().PushUpdateData("UPDATE `clan_player` SET `inQQGroup` = %u WHERE `playerId` = %"I64_FMT"u", _inQQGroup, getId());

        getClan()->sendQQOpenid(this);
    }
    return 0;
}

    //仙宠
    void Player::fairyPetUdpLog(UInt32 id, UInt8 type)
    {
        // 仙宠相关日志
        char action[16] = "";
        snprintf (action, 16, "F_%d_%d", id, type);
        udpLog("FairyPet", action, "", "", "", "", "act");
    }

	FairyPet * Player::findFairyPet( UInt32 id )
	{
		std::map<UInt32, FairyPet *>::iterator it = _fairyPets.find(id);
		if(it == _fairyPets.end())
			return NULL;
		return it->second;
	}

    bool Player::hasCanHirePet( UInt32 id )
    {
        std::vector<UInt32>::iterator it = std::find(_playerData.canHirePet.begin(), _playerData.canHirePet.end(), id);
        return it != _playerData.canHirePet.end();
    }

    bool Player::delCanHirePet( UInt32 id )
    {
        bool isDel = false;
        std::vector<UInt32>::iterator it = _playerData.canHirePet.begin();
        for(; it != _playerData.canHirePet.end(); ++ it)
        {
            if(*it == id)
            {
                _playerData.canHirePet.erase(it);
                isDel = true;
                break;
            }
        }
        if(isDel)
            writeCanHiretPet();
        return isDel;
    }

    void Player::delFairyPet(UInt32 id, UInt8 delete_type)
    {   //delete_type=>>0:放生 1:传承
        std::map<UInt32, FairyPet *>::iterator it = _fairyPets.find(id);
        DBLOG1().PushUpdateData("insert into pet_histories (server_id,player_id,pet_id,pet_name,delete_type,pet_pinjie,pet_gengu,delete_time) values(%u,%"I64_FMT"u,%u,'%s',%u,%u,%u,%u)",
            cfg.serverLogId, getId(), id, it->second->getName().c_str(), delete_type, it->second->getPetLev(), it->second->getPetBone(), TimeUtil::Now());

        _fairyPets.erase(it);
        DB2().PushUpdateData("DELETE FROM `fairyPet` WHERE id = %u AND `playerId` = %"I64_FMT"u", id, getId());
	    DB2().PushUpdateData("DELETE FROM `fighter` WHERE `id` = %u AND `playerId` = %"I64_FMT"u", id, getId());
    }

	void Player::writeCanHiretPet()
	{
        std::string petStr = "";
        UInt8 size = getCanHirePetNum();
        for(UInt8 i = 0; i < size; ++ i)
        {
            petStr += Itoa(_playerData.canHirePet[i]);
            if(i < size - 1)
                petStr += ",";
        }
        DB1().PushUpdateData("UPDATE `player` SET `canHirePet` = '%s' WHERE id = %"I64_FMT"u", petStr.c_str(), getId());
    }

	bool Player::isFairyPetFull() const
	{
		size_t size = _fairyPets.size();
        return size >= 6;
	}

    void Player::setCanHirePet(UInt32 id)
    {
        PLAYER_DATA(this, canHirePet).push_back(id);
    }

    UInt32 Player::setFairypetBattle( UInt32 id )
    {
        FairyPet * pet = findFairyPet(id);
        if(!pet) return 0;
        if(_onBattlePet == pet) //表明取消出战
        {
            pet = NULL;
            id = 0;
        }
        setFairypetBattle(pet);
        return id;
	}

    void Player::setFairypetBattle(FairyPet * pet, bool writedb)
    {
        if(_onBattlePet == pet)
            return;
        if(_onBattlePet && writedb)
        {
            _onBattlePet->setOnBattle(false);
            _onBattlePet->UpdateToDB();
        }
        if(pet && writedb)
        {
            pet->setOnBattle(true);
            pet->UpdateToDB();
        }
        _onBattlePet = pet;
    }

    void Player::addFairyPet(FairyPet * pet, bool writedb)
    {
        if(!pet) return;
        _fairyPets.insert(std::make_pair(pet->getId(), pet));

        if(writedb)
        {
            pet->UpdateToDB();
			UInt32 p = static_cast<UInt32>((pet->getPotential()+0.005) * 100);
			UInt32 c = static_cast<UInt32>((pet->getCapacity()+0.05) * 100);
			DB2().PushUpdateData("INSERT INTO `fighter` (`id`, `playerId`, `potential`, `capacity`, `level`, `experience`)\
                    VALUES(%u, %"I64_FMT"u, %u.%02u, %u.%02u, %u, %u)",
                    pet->getId(), getId(), p / 100, p % 100, c / 100, c % 100, pet->getLevel(), pet->getExp());
            pet->updateToDBPetSkill();
        }
    }

    //招募仙宠
	UInt8 Player::hireFairyPet( UInt32 id )
    {
		if(id > GREAT_FIGHTER_MAX)
			return 1;
        if(!hasCanHirePet(id))
            return 1;
        if(isFairyPetFull())
            return 3;
		if(findFairyPet(id) || hasFighter(id))
			return 2;
		FairyPet * pet = static_cast<FairyPet *>(globalFighters[id]);
		if(pet == NULL)
			return 1;
		FairyPet * pet2 = pet->clone(this);
		addFairyPet(pet2, true);
        delCanHirePet(id);
        if(pet2->getColor() > 1)
            SYSMSG_BROADCASTV(4133, getCountry(), getName().c_str(), pet2->getColor(), pet2->getName().c_str());
		SYSMSG_SENDV(4134, this, pet2->getColor(), pet2->getName().c_str());
        if(_fairyPets.size() == 1)  //第一个仙宠自动出战
        {
            setFairypetBattle(pet2, true);
            Stream st(REP::FAIRY_PET);
            st << static_cast<UInt8>(0x03) << static_cast<UInt8>(0x03);
            st << id << Stream::eos;
            send(st);
        }
        return 0;
    }

    //放生转化仙宠
	UInt8 Player::convertFairyPet( UInt32 id, UInt8 isHas)
    {
        UInt8 color = 0;
        if(isHas)
        {
            FairyPet * pet = findFairyPet(id);
            if(pet == NULL)
                return 1;
            if(pet->isOnBattle() || pet == _onBattlePet)
                return 2;
            color = pet->getColor();
            delFairyPet(id);
            delete pet;
        }
        else
        {
            if(!hasCanHirePet(id))
                return 1;
            delCanHirePet(id);
		    FairyPet * pet = static_cast<FairyPet *>(globalFighters[id]);
            if(pet == NULL)
                return 1;
            color = pet->getColor();
        }
        Table values = GameAction()->getConvertPetValue(color);
        UInt32 longYuan = values.get<UInt32>("longyuan");
        UInt32 fengSui = values.get<UInt32>("fengsui");
        UInt32 like = values.get<UInt32>("like");

        IncommingInfo ii1(LongyuanFromYouli, 0, 0);
        getLongyuan(longYuan, &ii1);
        IncommingInfo ii2(FengsuiFromYouli, 0, 0);
        getFengsui(fengSui, &ii2);
        AddVar(VAR_FAIRYPET_LIKEABILITY, like);
        if(like)
        {
            SYSMSG_SENDV(146, this, like);
            SYSMSG_SENDV(1046, this, like);
        }
        return 0;
    }

	void Player::sendFairyPetList()
    {
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x03) << static_cast<UInt8>(0x01);
        st << static_cast<UInt32>(_onBattlePet ? _onBattlePet->getId() : 0);
        st << static_cast<UInt8>(_fairyPets.size());
		std::map<UInt32, FairyPet *>::iterator it = _fairyPets.begin();
		while(it != _fairyPets.end())
        {
            st << it->first;
            st << it->second->getPetLev();
            st << it->second->getPetBone();
            st << it->second->getChongNum();
            ++ it;
        }
        st << Stream::eos;
        send(st);
    }

    void Player::getFariyPetSpaceInfo()
    {
        UInt8 step = GetVar(VAR_FAIRYPET_STEP);
        if(step < 1 || step > 5)
            step = 1;
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x01);
        UInt8 size = getCanHirePetNum();
        st << step << size;
        for(UInt8 i = 0; i < size; ++ i)
            st << _playerData.canHirePet[i];
        st << Stream::eos;
        send(st);
    }

    void Player::sendFairyPetResource()
    {
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x03) << static_cast<UInt8>(0x02);
        st << static_cast<UInt8>(GetVar(VAR_FAIRYPET_ISGET_PET));
        st << GetVar(VAR_FAIRYPET_LONGYUAN);
        st << GetVar(VAR_FAIRYPET_FENGSUI);
        st << GetVar(VAR_FAIRYPET_XIANYUAN);
        st << static_cast<UInt16>(GetVar(VAR_FAIRYPET_LIKEABILITY));
        st << Stream::eos;
        send(st);
    }

    //寻宠
    void Player::seekFairyPet(UInt8 count, UInt8 isConvert)
    {
        if(count == 0) return;
        if(getCanHirePetNum())
            return;
        static UInt32 cost[] = {0xFFFFFFFF, 16, 24, 48, 120, 360};  //游历消耗仙缘
        UInt32 xianYuan = GetVar(VAR_FAIRYPET_XIANYUAN);
        UInt8 step = GetVar(VAR_FAIRYPET_STEP);
        if(step < 1 || step > 5)
            step = 1;
        UInt32 longYuan = 0, fengSui = 0;
        UInt32 greenId = 0, blueId = 0;
        UInt8 like = 0;
        UInt32 convert1 = 0, convert2 = 0;
        UInt32 used = 0;
        std::string petStr = "";
        Stream st(REP::FAIRY_PET);
        st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x02);
	    size_t pos = st.size();
        UInt8 num = 0;
        st << num;
        for(UInt8 i = 0; i < count; ++ i)
        {
            if(xianYuan < cost[step] + used)
                break;
            fairyPetUdpLog(10000, 21);
            ++ num;
            used += cost[step];
            Table values = GameAction()->onSeekFairypetAwardAndSucceed(step, isConvert);
            longYuan += values.get<UInt32>("longyuan");
            fengSui += values.get<UInt32>("fengsui");
            like += values.get<UInt8>("like");
            greenId = values.get<UInt32>("greenId");
            if(greenId)
            {
                petStr += Itoa(greenId);
                if(i < count - 1)
                    petStr += ",";
                if(!isConvert)
                    setCanHirePet(greenId);
            }
            blueId = values.get<UInt32>("blueId");
            if(blueId)
            {
                petStr += Itoa(blueId);
                if(i < count - 1)
                    petStr += ",";
                if(!isConvert)
                    setCanHirePet(blueId);
            }
            if(isConvert)   //是否放生仙宠 0:否,1:是
            {
                convert1 += values.get<UInt32>("convert1");
                convert2 += values.get<UInt32>("convert2");
            }
            if(values.get<bool>("succeed"))
                step = (step + 1) > 5 ? 1 : step + 1;
            else
                step = 1;
            st << step;
        }
        if(num == 0)
            return;
        st << longYuan << fengSui << like;
        st << static_cast<UInt32>(xianYuan - used) << isConvert;
        st << petStr.c_str();
		st.data<UInt8>(pos) = num;
        st << Stream::eos;
        send(st);

        IncommingInfo ii1(LongyuanFromYouli, 0, 0);
        getLongyuan(longYuan, &ii1);
        IncommingInfo ii2(FengsuiFromYouli, 0, 0);
        getFengsui(fengSui, &ii2);
        if(isConvert)   //放生仙宠
        {
            IncommingInfo ii1(LongyuanFromConvert, 0, 0);
            getLongyuan(convert1, &ii1);
            IncommingInfo ii2(FengsuiFromConvert, 0, 0);
            getFengsui(convert2, &ii2);
        }
        ConsumeInfo ci(YouliForPet, 0, 0);
        useXianyuan(used, &ci);
        if(!isConvert && !petStr.empty())   //不放生仙宠
            writeCanHiretPet();
        AddVar(VAR_FAIRYPET_LIKEABILITY, like);
        SetVar(VAR_FAIRYPET_STEP, step);
        if(like)
        {
            SYSMSG_SENDV(146, this, like);
            SYSMSG_SENDV(1046, this, like);
        }
    }

    //仙宠免费领取(>=50级)
    void Player::getPetByLevelUp(UInt8 idx)
    {
        if(idx > 3 || GetLev() < 50)
            return;
        UInt32 isGet = GetVar(VAR_FAIRYPET_ISGET_PET);
        if(idx < 3)
        {
            if(isGet & (1 << 0))
                return;
            static UInt32 petId[] = { 501, 510, 507 };
            setCanHirePet(petId[idx]);
            UInt8 res = hireFairyPet(petId[idx]);
            Stream st(REP::FAIRY_PET);
            st << static_cast<UInt8>(0x04);
            st << static_cast<UInt32>(res == 0 ? petId[idx] : 0);
            st << Stream::eos;
            send(st);
            if(res)
                delCanHirePet(petId[idx]);
            SetVar(VAR_FAIRYPET_ISGET_PET, isGet | (1 << 0));
        }
        else
        {
            if(isGet & (1 << 1))
                return;
            IncommingInfo ii1(LongyuanFromUseItem, 0, 0);
            getLongyuan(16000, &ii1);
            IncommingInfo ii2(FengsuiFromUseItem, 0, 0);
            getFengsui(16000, &ii2);
            SetVar(VAR_FAIRYPET_ISGET_PET, isGet | (1 << 1));
            sendFairyPetResource();
        }
    }

    //使用仙宠蛋获得蓝色仙宠
    UInt8 Player::getPetByPetEgg(UInt32 id)
    {
        setCanHirePet(id);
        UInt8 res = hireFairyPet(id);
        if(res == 2) //已有相同的仙宠
            sendMsgCode(0, 4004);
        else
        {
            Stream st(REP::FAIRY_PET);
            st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x03);
            st << res << id;
            st << Stream::eos;
            send(st);
            if(res != 0)
                delCanHirePet(id);
        }
        return res;
    }

    UInt8 Player::transferPet(UInt32 petId1, UInt32 petId2)
    {
        FairyPet * pet1 = findFairyPet(petId1);
        FairyPet * pet2 = findFairyPet(petId2);
        if(!pet1 || !pet2 || pet1 == pet2)
            return 1;
        if(pet1->isOnBattle() || pet2->isOnBattle())
            return 1;
        if(pet1->getColor() > pet2->getColor())
            return 1;
        UInt16 lev = std::max(pet1->getPetLev(), pet2->getPetLev());
        UInt16 bone = std::max(pet1->getPetBone(), pet2->getPetBone());
        pet2->setPetLev(lev);
        pet2->setPetBone(bone);
        pet2->UpdateToDB();
        pet2->setPotential(GData::pet.getPetPotential(bone));
        pet2->setLevel(lev);
        pet2->updateToDB(2, lev);
        pet2->initSkillUp();

        delFairyPet(petId1, 1);
        delete pet1;
        return 0;
    }

    void Player::getLongyuanLua(UInt32 c)
    {
        IncommingInfo ii(LongyuanFromUseItem, 0, 0);
        getLongyuan(c, &ii);
        sendFairyPetResource();
    }

    void Player::getFengsuiLua(UInt32 c)
    {
        IncommingInfo ii(FengsuiFromUseItem, 0, 0);
        getFengsui(c, &ii);
        sendFairyPetResource();
    }

    void Player::getXianyuanLua(UInt32 c)
    {
        IncommingInfo ii(XianyuanFromUseItem, 0, 0);
        getXianyuan(c, &ii);
        sendFairyPetResource();
    }

    UInt32 Player::getXianyuan( UInt32 c, IncommingInfo* ii)
    {
        UInt32 xianyuan = GetVar(VAR_FAIRYPET_XIANYUAN);
		if(c == 0)
			return xianyuan;
		xianyuan += c;
		SYSMSG_SENDV(161, this, c);
		SYSMSG_SENDV(1061, this, c);
        SetVar(VAR_FAIRYPET_XIANYUAN, xianyuan);
        /*
        Stream st(REP::USER_INFO_CHANGE);
        st << static_cast<UInt8>(0x18) << xianyuan << Stream::eos;
        send(st);
        */

        if(ii && ii->incommingType != 0)
        {
            DBLOG1().PushUpdateData("insert into consume_pet (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ii->incommingType, ii->itemId, ii->itemNum, c, TimeUtil::Now());
        }

        return xianyuan;
	}

	UInt32 Player::useXianyuan( UInt32 a, ConsumeInfo * ci )
	{
        UInt32 xianyuan = GetVar(VAR_FAIRYPET_XIANYUAN);
        if(a == 0 || xianyuan == 0)
            return xianyuan;
        if(xianyuan < a)
            xianyuan = 0;
        else
        {
            xianyuan -= a;
            if(ci != NULL)
            {
                DBLOG1().PushUpdateData("insert into consume_pet (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
            }
        }
        SYSMSG_SENDV(148, this, a);
        SYSMSG_SENDV(1048, this, a);
        SetVar(VAR_FAIRYPET_XIANYUAN, xianyuan);

        return xianyuan;
    }

    UInt32 Player::getLongyuan( UInt32 c, IncommingInfo* ii)
    {
        UInt32 longyuan = GetVar(VAR_FAIRYPET_LONGYUAN);
		if(c == 0)
			return longyuan;
		longyuan += c;
		SYSMSG_SENDV(159, this, c);
		SYSMSG_SENDV(1059, this, c);
        SetVar(VAR_FAIRYPET_LONGYUAN, longyuan);

        if(ii && ii->incommingType != 0)
        {
            DBLOG1().PushUpdateData("insert into consume_pet (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ii->incommingType, ii->itemId, ii->itemNum, c, TimeUtil::Now());
        }

        return longyuan;
	}

	UInt32 Player::useLongyuan( UInt32 a, ConsumeInfo * ci )
	{
        UInt32 longyuan = GetVar(VAR_FAIRYPET_LONGYUAN);
        if(a == 0 || longyuan == 0)
            return longyuan;
        if(longyuan < a)
            longyuan = 0;
        else
        {
            longyuan -= a;
            if(ci != NULL)
            {
                DBLOG1().PushUpdateData("insert into consume_pet (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
            }
        }
        SYSMSG_SENDV(158, this, a);
        SYSMSG_SENDV(1058, this, a);
        SetVar(VAR_FAIRYPET_LONGYUAN, longyuan);

        return longyuan;
    }

    UInt32 Player::getFengsui( UInt32 c, IncommingInfo* ii)
    {
        UInt32 fengsui = GetVar(VAR_FAIRYPET_FENGSUI);
		if(c == 0)
			return fengsui;
		fengsui += c;
		SYSMSG_SENDV(160, this, c);
		SYSMSG_SENDV(1060, this, c);
        SetVar(VAR_FAIRYPET_FENGSUI, fengsui);

        if(ii && ii->incommingType != 0)
        {
            DBLOG1().PushUpdateData("insert into consume_pet (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ii->incommingType, ii->itemId, ii->itemNum, c, TimeUtil::Now());
        }

        return fengsui;
	}

	UInt32 Player::useFengsui( UInt32 a, ConsumeInfo * ci )
	{
        UInt32 fengsui = GetVar(VAR_FAIRYPET_FENGSUI);
        if(a == 0 || fengsui == 0)
            return fengsui;
        if(fengsui < a)
            fengsui = 0;
        else
        {
            fengsui -= a;
            if(ci != NULL)
            {
                DBLOG1().PushUpdateData("insert into consume_pet (server_id,player_id,consume_type,item_id,item_num,expenditure,consume_time) values(%u,%"I64_FMT"u,%u,%u,%u,%u,%u)",
                cfg.serverLogId, getId(), ci->purchaseType, ci->itemId, ci->itemNum, a, TimeUtil::Now());
            }
        }
        SYSMSG_SENDV(157, this, a);
        SYSMSG_SENDV(1057, this, a);
        SetVar(VAR_FAIRYPET_FENGSUI, fengsui);

        return fengsui;
    }

void Player::getQQGameOnlineAward()
{
    if(!World::getQQGameOnlineAwardAct())
        return;
    if(atoi(getDomain()) != 10)
        return;
    if(GetVar(VAR_ONLINE_AWARD) > 0)
        return;
    if(getQQGameOnlineTotalTime() < 3600)
        return;
    if (GetPackage()->GetRestPackageSize() < 4)
    {
        sendMsgCode(0, 1011);
        return;
    }
    SetVar(VAR_ONLINE_AWARD, 1);
    GetPackage()->Add(134, 1, true, false);
    GetPackage()->Add(1325, 1, true, false);
    GetPackage()->Add(15, 1, true, false);
    GetPackage()->Add(500, 1, true, false);
    sendQQGameOnlineAward();
}

void Player::sendQQGameOnlineAward()
{
    if(!World::getQQGameOnlineAwardAct())
        return;
    if(atoi(getDomain()) != 10)
        return;
    Stream st(REP::COUNTRY_ACT);
    st << static_cast<UInt8>(0x0B);
    st << static_cast<UInt8>(GetVar(VAR_ONLINE_AWARD));
    UInt16 totalTime = getQQGameOnlineTotalTime();
    UInt16 leftTime;
    if(totalTime >= 3600)
        leftTime = 0;
    else
        leftTime = 3600 - totalTime;
    st << leftTime;
    st << Stream::eos;
    send(st);
}

void Player::setQQGameOnlineTotalTime()
{
    if(!World::getQQGameOnlineAwardAct())
        return;
    if(atoi(getDomain()) != 10)
        return;
    SetVar(VAR_ONLINE_TOTAL_TIME, getQQGameOnlineTotalTime());
}

UInt32 Player::getQQGameOnlineTotalTime()
{
    UInt32 now = TimeUtil::Now();
    UInt32 today = TimeUtil::SharpDayT( 0 , now);
    UInt32 lastOnline = _playerData.lastOnline; //考虑了是前几天登录的情况
    UInt32 curTime;
    if(now <= today + 19*3600)
        curTime = 0;
    else if(now <= today + 21*3600)
    {
        if(lastOnline <= (today + 19*3600))
            curTime = now - (today + 19*3600);
        else
            curTime = now - lastOnline;
    }
    else
    {
        if(lastOnline <= (today + 19*3600))
            curTime = 3600;
        else
            curTime = (today + 21*3600) > lastOnline ? ((today + 21*3600) - lastOnline) : 0;
    }
    return GetVar(VAR_ONLINE_TOTAL_TIME) + curTime;
}
void Player::sendRP7TreasureInfo(bool isLogin)
{
    bool isFinish = false;
    UInt8 totalGot = 0;
    UInt32 b = GVAR.GetVar(GVAR_TREASURE_BEGIN);
    UInt32 n = GVAR.GetVar(GVAR_TREASURE_END);
    if (cfg.rpServer && b ==0)
    {
        b = World::getOpenTime();
        n = b+7*86400;
    }
    Stream st(REP::RP_SERVER);
    st << static_cast<UInt8>(0x02) <<static_cast<UInt8>(0x01);
    st << b << n;
    UInt8 v = GetVar(VAR_RP7_TREASURE);
    UInt8 c = 0;
    st << v;
    for (UInt8 i = 0; i < 6; i++)
    {
        if (v&(1<<i))
        {
            UInt32 tm = GetVar(VAR_RP7_TREASURE1_GETTIME+c);
            UInt8 count =10-(GetVar(VAR_RP7_TREASURE)>>(c*8+8));
            UInt8 got = (GetVar(VAR_RP7_TREASURE_TODAY_GOT)&(1<<c))?1:0;
            c++;
            st << i << tm << count <<got;
            totalGot += 10-count;
        }
    }
    st << Stream::eos;
    if (isLogin)
    {
        if (c >0 && totalGot==c*10)
            isFinish = true;
        if (cfg.rpServer)
        {
            UInt32 opTime = TimeUtil::MkTime(cfg.openYear, cfg.openMonth, cfg.openDay);
            if(TimeUtil::SharpDay(0) >= opTime + 7 * 86400  && c == 0)
                isFinish = true;
        }
        else
        {
            UInt32 now = TimeUtil::Now();
            if (c == 0 && (now < b || now > n))
                isFinish = true;
        }
    }
    if (!isFinish)
       send(st);
}

void Player::buyRP7Treasure(UInt8 idx)
{
    static UInt32 s_gold[] = {1000,10000,30000,50000,80000,120000};
    if (idx > 5)
        return;
 
    UInt8 res = 0;
    UInt32 opTime = TimeUtil::MkTime(cfg.openYear, cfg.openMonth, cfg.openDay);
    if((TimeUtil::SharpDay(0) >= opTime + 7 * 86400 && cfg.rpServer) ||
       (TimeUtil::Now()<GVAR.GetVar(GVAR_TREASURE_BEGIN) || TimeUtil::Now()>GVAR.GetVar(GVAR_TREASURE_END))
      )
        res = 4;
    UInt8 v = GetVar(VAR_RP7_TREASURE); 
    if (v&(1<<idx))
    {
        res = 1;
    }
    else if (res == 0)
    {
        UInt8 c = 0;
        UInt32 var = VAR_RP7_TREASURE1_GETTIME;
        for (UInt8 i = 0; i < 6; i++)
        {
            if (v&(1<<i)) 
            {
                c++;
                if (idx > i)
                    var += 1;
            }
        }
        if ( c >= 3)
        {
            res = 2;
        }
        else
        {
     		if (getGold() < s_gold[idx])
            {
                res = 3;
                sendMsgCode(0,1104);
                return;
            }
            else
            {
            	ConsumeInfo ci(RP7Treasure,0,0);
        	    useGold(s_gold[idx],&ci);
                v |= (1<<idx);
                SetVar(VAR_RP7_TREASURE, v);

                UInt32 tm = TimeUtil::SharpDay(0)+10*86400;
                //需要重新排列3个var的数值
                if (var == VAR_RP7_TREASURE1_GETTIME)
                {
                    SetVar(VAR_RP7_TREASURE1_GETTIME+2, GetVar(VAR_RP7_TREASURE1_GETTIME+1)); 
                    SetVar(VAR_RP7_TREASURE1_GETTIME+1, GetVar(VAR_RP7_TREASURE1_GETTIME));
                }
                else if ( var == (VAR_RP7_TREASURE1_GETTIME+1))
                {
                    SetVar(VAR_RP7_TREASURE1_GETTIME+2, GetVar(VAR_RP7_TREASURE1_GETTIME+1));
                }
                SetVar(var,tm);  
            }
        }
    }
    Stream st(REP::RP_SERVER);
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x02) << res;
    st << Stream::eos;
    send(st);
    if (res == 0)
        sendRP7TreasureInfo();
}

void Player::getRP7TreasureAward(UInt8 idx)
{
    static UInt32 s_gold[] = {1000,10000,30000,50000,80000,120000};
    static MailPackage::MailItem s_item[][8] = {
        {{0xB000,100},{57,1},{56,1},{29,10},{15,1},{48,1}},
        {{0xB000,1000},{0xA000,20},{509,1},{507,1},{516,1},{547,1}},
        {{0xB000,3000},{0xA000,50},{503,1},{47,1},{500,1},{501,1}},
        {{0xB000,5000},{0xA000,80},{509,2},{503,2},{515,2},{507,2}},
        {{0xB000,8000},{0xA000,100},{1325,3},{134,3},{509,3},{507,3}},
        {{0xB000,12000},{0xA000,150},{1325,5},{134,5},{509,5},{507,5}},
    };
    if (idx > 5)
        return;
 
    UInt8 res = 0;
    UInt8 v = GetVar(VAR_RP7_TREASURE); 
    if (v&(1<<idx))
    {
        UInt8 c = 0;
        for (UInt8 i = 0; i < 6; i++)
        {
            if (v&(1<<i)) 
                c++;
            if (idx == i)
                break;
        }
        UInt8 count = GetVar(VAR_RP7_TREASURE)>>(c*8);
        if (count >= 10)
            res = 2;
        else if (GetVar(VAR_RP7_TREASURE_TODAY_GOT)&(1<<(c-1)))
            res = 1;
        else if (TimeUtil::Now() < GetVar(VAR_RP7_TREASURE1_GETTIME+c-1))
            res = 3;
        else
        {
            SYSMSG(title,4912);
            SYSMSGV(content,4913,s_gold[idx],count+1);
			Mail * mail = m_MailBox->newMail(NULL, 0x21, title, content, 0xFFFE0000);
            if (mail)
            {
                UInt8 t = GetVar(VAR_RP7_TREASURE_TODAY_GOT);
                SetVar(VAR_RP7_TREASURE_TODAY_GOT, t|=(1<<(c-1)));
                count++;
                UInt32 v1 = GetVar(VAR_RP7_TREASURE);
                v1 &= ~(0xFF<<(c*8));
                v1 += (count<<(c*8));
                SetVar(VAR_RP7_TREASURE, v1);
                mailPackageManager.push(mail->id, s_item[idx], 8, true);
            }
        }
    }
    Stream st(REP::RP_SERVER);
    st << static_cast<UInt8>(0x02) << static_cast<UInt8>(0x03) << res;
    st << Stream::eos;
    send(st);
    sendRP7TreasureInfo();
}

void Player::sendRP7SignInfo()
{
    if (!cfg.rpServer)
        return;
 
    UInt32 now_sharp = TimeUtil::SharpDay(0);
    UInt32 created_sharp = TimeUtil::SharpDay(0, getCreated());
    if (now_sharp < created_sharp)
        return;
    UInt8 day = (now_sharp-created_sharp)/86400 + 1;
    if (day > 30)
        return;
    UInt32 v = GetVar(VAR_RP7_SIGN);
    UInt8 p = GetVar(VAR_RP7_SIGN_PACKAGE); 
    Stream st(REP::RP_SERVER);
    st << static_cast<UInt8>(0x03) << static_cast<UInt8>(0x01);
    st << day << v << p;
    st << Stream::eos;
    send(st);
}
void Player::RP7Sign(UInt8 idx)
{
    static UInt32 s_item[][3] = {
        {15,56,57},
        {15,51,48},
        {500,512},
        {48,50,466}
    };
    if (!cfg.rpServer)
        return;
 
    UInt32 now_sharp = TimeUtil::SharpDay(0);
    UInt32 created_sharp = TimeUtil::SharpDay(0, getCreated());
    if (now_sharp < created_sharp)
        return;
    UInt8 day = (now_sharp-created_sharp)/86400 + 1;
    if (day > 30 || idx > day)
        return;
    UInt32 v = GetVar(VAR_RP7_SIGN); 
    if (v&(1<<(idx-1)))
        return;
    if (GetPackage()->GetRestPackageSize() < 3)
    {
        sendMsgCode(0, 1011);
        return;
    }
    if (idx < day)
    {
        if (getGold() < 10)
        {
            sendMsgCode(0, 1104);
            return;
        }
       	ConsumeInfo ci(RP7Treasure,0,0);
        useGold(10,&ci);
    }
    UInt8 i = 0;
    if (idx >= 4 && idx <=7)
        i = 1;
    else if (idx >= 8 && idx <= 15)
        i = 2;
    else if (idx > 15) 
    {
        getCoupon(10);
        i = 3;
    }
    for (UInt8 j = 0; j < sizeof(s_item[i])/sizeof(s_item[i][0]); ++j)
        GetPackage()->Add(s_item[i][j], 1, true);
    v |= (1<<(idx-1));
    SetVar(VAR_RP7_SIGN, v);
    sendRP7SignInfo();
    
}
void Player::getRP7SignPackage(UInt8 idx)
{
    static UInt8 s_needDays[] = {3,7,15,28};
    if(idx > 3)
        return;
    if (!cfg.rpServer)
        return;
 
    UInt8 v = GetVar(VAR_RP7_SIGN_PACKAGE); 
    if (v&(1<<(idx)))
        return;
    if (GetPackage()->GetRestPackageSize() < 3)
    {
        sendMsgCode(0, 1011);
        return;
    }
    UInt32 s = GetVar(VAR_RP7_SIGN);
    UInt8 count = 0;
    for (UInt8 i = 0; i < 32; ++i)
    {
        if (s&(1<<i))
            count++;
    }
    if (count < s_needDays[idx])
        return;

    if (0 == idx)
        GetPackage()->Add(15, 5, true);   
    else if (1 == idx)
        GetPackage()->Add(516, 5, true); 
    else if (2 == idx)
    {
        GetPackage()->Add(1646, 1, true); 
    }
    else if (3 == idx)
        GetPackage()->Add(1700, 1, true);
    v |= 1<<(idx);
    SetVar(VAR_RP7_SIGN_PACKAGE, v);
    sendRP7SignInfo();
}

bool Player::inVipPrivilegeTime()
{
    UInt32 validate = GetVar(VAR_VIP_PRIVILEGE_TIME);
    UInt32 now = TimeUtil::Now();
    bool ret = true;
    if(validate <= now)
    {
        ret = false;
    }

    return ret;
}

bool Player::SetVipPrivilege()
{
    UInt32 validate = GetVar(VAR_VIP_PRIVILEGE_TIME);
    bool ret = false;
    if(validate == 0)
    {
        UInt32 now = TimeUtil::Now();
        UInt32 validate = now + 168*3600;
        // 保持最低位为0
        if(validate & 0x1)
            validate = validate + 1;
        SetVar(VAR_VIP_PRIVILEGE_TIME, validate);
        ret = true;
        ConsumeInfo ci(VipPrivilege, 0, 0);
        useGold(100, &ci);
    }

    return ret;
}


#define VIP_PRIVILEGE_DAYLYAWARD(data) (0x01&data)
#define VIP_PRIVILEGE_LIMITBUY1(data)  (0x02&data)
#define VIP_PRIVILEGE_LIMITBUY2(data)  (0x04&data)
#define VIP_PRIVILEGE_LIMITBUY3(data)  (0x08&data)
#define VIP_PRIVILEGE_TIMEOUT(data)  (0x1&data)

#define SET_VIP_PRIVILEGE_DAYLYAWARD(data, v) (data|=(v&0x01))
#define SET_VIP_PRIVILEGE_LIMITBUY1(data, v)  (data|=((v<<1)&0x02))
#define SET_VIP_PRIVILEGE_LIMITBUY2(data, v)  (data|=((v<<2)&0x04))
#define SET_VIP_PRIVILEGE_LIMITBUY3(data, v)  (data|=((v<<3)&0x08))
#define SET_VIP_PRIVILEGE_OPEN(data, v)       (data|=((v<<4)&0x10))
#define SET_VIP_PRIVILEGE_DAYTH(data, v)      (data|=((v<<5)&0xE0))
#define SET_VIP_PRIVILEGE_TIMEOUT(data, v)    (data|=((v)&0x1))

void Player::doVipPrivilege(UInt8 idx)
{
    UInt32 data = GetVar(VAR_VIP_PRIVILEGE_DATA);
    switch(idx)
    {
    case 1:
        if(VIP_PRIVILEGE_DAYLYAWARD(data))
            return;
        SET_VIP_PRIVILEGE_DAYLYAWARD(data, 1);
        break;
    case 2:
        if(VIP_PRIVILEGE_LIMITBUY1(data))
            return;
        SET_VIP_PRIVILEGE_LIMITBUY1(data, 1);
        break;
    case 3:
        if(VIP_PRIVILEGE_LIMITBUY2(data))
            return;
        SET_VIP_PRIVILEGE_LIMITBUY2(data, 1);
        break;
    case 4:
        if(VIP_PRIVILEGE_LIMITBUY3(data))
            return;
        SET_VIP_PRIVILEGE_LIMITBUY3(data, 1);
        break;
    case 5:
        if(!in7DayFromCreated())
            return;

        if (getGold() < 100)
            return;
        SetVipPrivilege();
        break;
    }

    if(idx > 0 && idx < 5)
    {
        if(inVipPrivilegeTime())
        {
            UInt32 validate = GetVar(VAR_VIP_PRIVILEGE_TIME);
            UInt32 now = TimeUtil::Now();
            UInt8 dayth = (TimeUtil::SharpDayT(0, now) + 168*3600 - TimeUtil::SharpDayT(0, validate))/86400 + 1;
            if(!GameAction()->RunVipPrivilegeAward(this, idx, dayth))
                return;
            SetVar(VAR_VIP_PRIVILEGE_DATA, data);
        }
        else
        {
            return;
        }
    }

    sendVipPrivilege();
}

void Player::sendVipPrivilege()
{
    UInt32 validate = GetVar(VAR_VIP_PRIVILEGE_TIME);
    UInt32 data = GetVar(VAR_VIP_PRIVILEGE_DATA);
    UInt32 now = TimeUtil::Now();
    UInt8 dayth = (TimeUtil::SharpDayT(0, now) + 168*3600 - TimeUtil::SharpDayT(0, validate))/86400;
    if(dayth > 7)
        dayth = 7;
    UInt32 timeLeft = 0;
    UInt8 timeOut = 0;
    if(validate > now)
        timeLeft = validate - now;
    if(validate != 0)
    {
        SET_VIP_PRIVILEGE_OPEN(data, 1);
        if(timeLeft == 0 && VIP_PRIVILEGE_TIMEOUT(validate) == 0)
        {
            timeOut = 1;
            SET_VIP_PRIVILEGE_TIMEOUT(validate, 1);
            SetVar(VAR_VIP_PRIVILEGE_TIME, validate);
        }
    }
    else
    {
        SET_VIP_PRIVILEGE_OPEN(data, 0);
    }

    SET_VIP_PRIVILEGE_DAYTH(data, dayth);
    Stream st(REP::RC7DAY);
    st << static_cast<UInt8>(10) << timeLeft << static_cast<UInt8>(data) << timeOut;
    st << Stream::eos;
    send(st);
}

bool Player::in7DayFromCreated()
{
    UInt32 now = TimeUtil::Now();
    UInt32 now_sharp = TimeUtil::SharpDay(0, now);
    UInt32 created_sharp = TimeUtil::SharpDay(0, getCreated());
    if (created_sharp > now_sharp)
        return false; // 创建时间错误（穿越了）

    if (now_sharp - created_sharp > 7 * 24*60*60)
        return false; // 玩家注册时间超过7日，无法参与活动

    UInt32 off = CREATE_OFFSET(created_sharp, now_sharp);
    if (off >= 7)
        return false; // 玩家注册时间超过7日，无法参与活动

    return true;
}

#define QUESTIONID_MAX 30
#define SET_BIT(X,Y) (X | (1<<Y))
#define CLR_BIT(X,Y) (X & ~(1<<Y))
#define CLR_BIT_8(X,Y) (X & ~(0xFF<<(Y*8)))
#define SET_BIT_8(X,Y,V) (CLR_BIT_8(X,Y) | V<<(Y*8))
#define GET_BIT_8(X,Y) ((X >> (Y*8)) & 0xFF)
void Player::sendFoolsDayInfo()
{
    UInt32 info = GetVar(VAR_FOOLS_DAY_INFO);
    UInt32 value = GetVar(VAR_FOOLS_DAY);
    UInt32 qtime = GetVar(VAR_FOOLS_DAY_TIME);
    UInt8 qid = GET_BIT_8(value, 0);
    UInt8 right = 0;
    bool isFail = false;
    if((info & (1<<0)) == 1)
        isFail = true;
    if((info & (1<<0)) == 0 && qid)
    {
        if(!GET_BIT_8(value, 3) && qtime + 15 < TimeUtil::Now())
            isFail = true;
        else    //有离线标志
        {
            qtime = TimeUtil::Now();
            SetVar(VAR_FOOLS_DAY_TIME, qtime);
        }
    }
    for(UInt8 i = 1; i <= QUESTIONID_MAX; ++ i)
    {
        if(info & (1<<i))
            ++ right;
    }
    if(!isFail && qid == 0 && right < QUESTIONID_MAX && GET_BIT_8(value, 1) == 0)
    {   //分配新题目
        UInt8 index = uRand(QUESTIONID_MAX-right);
        UInt8 j = 0;
        for(UInt8 i = 1; i <= QUESTIONID_MAX; ++ i)
        {
            if((info & (1<<i)) == 0)
            {
                if(j == index)
                {
                    qid = i;
                    break;
                }
                ++ j;
            }
        }
        SetVar(VAR_FOOLS_DAY, SET_BIT_8(value, 0, qid));
        qtime = TimeUtil::Now();
        SetVar(VAR_FOOLS_DAY_TIME, qtime);
        if(right == 0)
            foolsDayUdpLog(7);
    }
    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(0x10) << static_cast<UInt8>(0x01);
    st << right << static_cast<UInt8>(GET_BIT_8(value, 1));
    st << static_cast<UInt8>(GET_BIT_8(value, 2));
    st << static_cast<UInt8>(isFail ? 1 : 0) << qid << qtime;
    st << Stream::eos;
    send(st);
}

void Player::submitAnswerInFoolsDay(UInt8 id, char answer)
{
    UInt32 info = GetVar(VAR_FOOLS_DAY_INFO);
    UInt32 value = GetVar(VAR_FOOLS_DAY);
    if(GET_BIT_8(value, 1))
        return;
    UInt32 qtime = GetVar(VAR_FOOLS_DAY_TIME);
    if(info == 0 && value == 0 && qtime == 0)
    {   //零点重置
        sendFoolsDayInfo();
        return;
    }
    UInt8 qid = GET_BIT_8(value, 0);
    bool isRight = true;
    if((info & 0x01) == 1)
        isRight = false;
    if(id != qid || qtime + 15 < TimeUtil::Now())
        isRight = false;
    if(isRight)
        isRight = GameAction()->checkAnswerInFoolsDay(id, answer);
    value = CLR_BIT_8(value, 3);    //清除离线标志
    if(isRight) //答对
    {
        SetVar(VAR_FOOLS_DAY, CLR_BIT_8(value, 0));
        info = SET_BIT(info, id);
        SetVar(VAR_FOOLS_DAY_INFO, CLR_BIT(info, 0));
        SYSMSG_SENDV(4148, this);
    }
    else        //答错
    {
        SetVar(VAR_FOOLS_DAY, SET_BIT_8(value, 0, id));
        SetVar(VAR_FOOLS_DAY_INFO, SET_BIT(info, 0));
        if(qtime + 15 < TimeUtil::Now())
        {
            SYSMSG_SENDV(4150, this);
        }
        else
        {
            SYSMSG_SENDV(4149, this);
        }
    }
    sendFoolsDayInfo();
}

void Player::getAwardInFoolsDay()
{
    UInt32 value = GetVar(VAR_FOOLS_DAY);
    if(GET_BIT_8(value, 1))
        return;
    UInt8 right = 0;
    UInt32 info = GetVar(VAR_FOOLS_DAY_INFO);
    for(UInt8 i = 1; i <= QUESTIONID_MAX; ++ i)
    {
        if(info & (1<<i))
            ++ right;
    }
    if(right < 5)
        return;
    if (GetPackage()->GetRestPackageSize() < right / 5)
    {
        sendMsgCode(2, 1011, 0);
        return;
    }
    GameAction()->getAwardInFoolsDay(this, right / 5);
    SetVar(VAR_FOOLS_DAY, SET_BIT_8(value, 1, right/5 * 5));
    sendFoolsDayInfo();
    foolsDayUdpLog(right / 5);
}

void Player::buyResurrectionCard()
{
    if(!hasChecked())
        return;
    UInt32 value = GetVar(VAR_FOOLS_DAY);
    if(GET_BIT_8(value, 1))
        return;
    if(GET_BIT_8(value, 0) == 0)
        return;
    UInt8 cnt = GET_BIT_8(value, 2) + 1;
    if(cnt * 10 > getGold())
    {
        sendMsgCode(0, 1104);
        return;
    }
    ConsumeInfo ci(FoolsDayAnswerAct, 0, 0);
    useGold(cnt*10, &ci);

    UInt32 info = GetVar(VAR_FOOLS_DAY_INFO);
    info = SET_BIT(info, GET_BIT_8(value, 0));
    SetVar(VAR_FOOLS_DAY_INFO, CLR_BIT(info, 0));
    value = SET_BIT_8(value, 2, cnt);
    value = CLR_BIT_8(value, 3);    //清除离线标志
    SetVar(VAR_FOOLS_DAY, CLR_BIT_8(value, 0));
    sendFoolsDayInfo();
}

void Player::setLogoutInFoolsDay()
{
    if (World::getFoolsDay())
    {
        UInt32 info = GetVar(VAR_FOOLS_DAY_INFO);
        if(info == 0 || (info & (1<<0)) == 1)
            return;
        UInt32 qtime = GetVar(VAR_FOOLS_DAY_TIME);
        if(qtime == 0)
            return;
        UInt32 value = GetVar(VAR_FOOLS_DAY);
        if(qtime + 10 < TimeUtil::Now())
        {
            SetVar(VAR_FOOLS_DAY, CLR_BIT_8(value, 3));
            return;
        }
        SetVar(VAR_FOOLS_DAY, SET_BIT_8(value, 3, 1));
    }
}

void Player::checkAnswerActInFoolsDay()
{
    UInt32 info = GetVar(VAR_FOOLS_DAY_INFO);
    UInt32 value = GetVar(VAR_FOOLS_DAY);
    UInt8 type = 0, right = 0;
    if(GET_BIT_8(value, 1))
        type = 1;
    else
    {
        for(UInt8 i = 1; i <= QUESTIONID_MAX; ++ i)
        {
            if(info & (1<<i))
                ++ right;
        }
        if(right == 0 && (info & (1<<0)) == 0)
            type = 3;
        else
            type = 2;
    }
    Stream st(REP::ACTIVE);
    st << static_cast<UInt8>(0x10) << static_cast<UInt8>(0x00);
    st << type << Stream::eos;
    send(st);
}

void Player::foolsDayUdpLog(UInt8 type)
{
    // 愚公移山相关日志
    char action[16] = "";
    snprintf (action, 16, "F_10000_0327_%d", type);
    udpLog("FoolsDay", action, "", "", "", "", "act");
}

} // namespace GObject



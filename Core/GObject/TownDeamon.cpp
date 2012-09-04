#include "TownDeamon.h"
#include "Player.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "Package.h"
#include "Server/Cfg.h"
#include "GData/Money.h"
#include "Country.h"
#include "Script/GameActionLua.h"
#include "Server/SysMsg.h"
#include "Var.h"
#include "MsgHandler/CountryMsgStruct.h"

namespace GObject
{

#define ACC_ITEM        465
#define VITALITY_ITEM   466
#define TD_AUTO_TAEL    50

TownDeamon* townDeamonManager = new TownDeamon();


UInt32 DeamonPlayerData::calcAccLeft()
{
    UInt32 accLeft = 0;
    UInt32 nowTm = TimeUtil::Now();
    if(accLen > nowTm - accTime)
        accLeft = accLen - nowTm + accTime;
    return accLeft;
}

UInt32 DeamonPlayerData::calcAwards()
{
    UInt32 awards = 0;
    UInt32 accLeft = 0;
    UInt32 nowTm = TimeUtil::Now();
    if(accLen > nowTm - accTime)
        accLeft = accLen - nowTm  + accTime;

    UInt32 tmpStartTime = 0;
    if(deamonLevel != 0)
        tmpStartTime = startTime;

    if(tmpStartTime != 0 && nowTm > tmpStartTime)
    {
        UInt32 normal = (nowTm - tmpStartTime)/TD_AWARD_TIMEUNIT;
        awards = normal + accAwards + (accLen - accLeft)/TD_AWARD_TIMEUNIT;
    }
    return awards;
}

UInt32 DeamonPlayerData::calcVitality()
{
    UInt32 vitalityLeft = 0;
    UInt32 nowTm = TimeUtil::Now();
    if(vitality * TD_VITALITY_TIMEUNIT > nowTm - vitalityTime)
        vitalityLeft = vitality - (nowTm - vitalityTime) / TD_VITALITY_TIMEUNIT;

    return vitalityLeft;
}

UInt32 DeamonPlayerData::calcSpirit()
{
    UInt32 spiritLeft = 0;
    UInt32 nowTm = TimeUtil::Now();
    if(vitality * TD_VITALITY_TIMEUNIT > nowTm - vitalityTime)
        spiritLeft = spirit;
    else if((spirit + vitality) * TD_VITALITY_TIMEUNIT > (nowTm - vitalityTime))
        spiritLeft = spirit + vitality - (nowTm - vitalityTime) / TD_VITALITY_TIMEUNIT;

    return spiritLeft;
}


TownDeamon::TownDeamon()
{
    m_Monsters.clear();
    m_maxDeamonLevel = 0;
    m_maxLevel = 0;
}

TownDeamon::~TownDeamon()
{
    m_Monsters.clear();
}

void TownDeamon::loadDeamonMonstersFromDB(UInt16 level, UInt32 npcId, UInt32 itemId, UInt32 itemNum)
{
    DeamonMonster dm;
    dm.npcId = npcId;
    dm.itemId = itemId;
    dm.itemNum = itemNum;
    m_Monsters.push_back(dm);
    if(level != m_Monsters.size())
        printf("TownDeamon: loadDeamonMonsterFromDB Error\n");
}

void TownDeamon::loadDeamonPlayersFromDB(UInt16 level, UInt16 maxLevel, Player* pl)
{
    if(level > m_Monsters.size() || level == 0)
        printf("TownDeamon: loadDeamonPlayerFromDB Error\n");
    UInt16 idx = level - 1;
    if(level > m_maxDeamonLevel)
        m_maxDeamonLevel = level;
    if (maxLevel > m_maxLevel)
        m_maxLevel = maxLevel;

    DeamonMonster& dm = m_Monsters[idx];
    dm.player = pl;
}

void TownDeamon::listDeamons(Player* pl, UInt16 start, UInt16 count)
{
    if(start > m_Monsters.size())
        return;

    UInt8 cnt = 0;
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x02);
    st << m_maxDeamonLevel << start << cnt;
    UInt16 end = start + count;
    if(end > m_Monsters.size())
        end = m_Monsters.size();

    UInt8 pos = st.size() - 1;
    for(Int16 idx = start; idx < end; ++idx)
    {
        Int16 lvlIdx = m_maxDeamonLevel - idx - 1;
        if(lvlIdx < 0)
            break;
        ++ cnt;
        Player* pl2 = m_Monsters[lvlIdx].player;
        if(pl2)
        {
            DeamonPlayerData* dpd = pl2->getDeamonPlayerData();
            UInt32 spirit = dpd->calcSpirit();
            UInt16 awards = dpd->calcAwards();
            st << static_cast<UInt16>(lvlIdx + 1) << pl2->getName() << pl2->GetLev() << awards << spirit;
        }
        else
            st << static_cast<UInt16>(lvlIdx + 1) << "" << static_cast<UInt8>(0) << static_cast<UInt16>(0) << static_cast<UInt32>(0);
    }

    st.data<UInt8>(pos) = cnt;
    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::showTown(Player* pl)
{
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x00);

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    UInt32 awards = dpd->calcAwards();
    UInt32 accLeft = dpd->calcAccLeft();
    UInt32 spirit = dpd->calcSpirit();
    UInt32 vitality = dpd->calcVitality();
    UInt32 leftTime = 0;
    UInt32 challengeCD = 0;
    if(TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM > TimeUtil::Now())
        leftTime = TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM - TimeUtil::Now();
    else
        leftTime = TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM + 86400 - TimeUtil::Now();

    if(dpd->challengeTime + TD_CHALLENGE_TIMEUNIT > TimeUtil::Now())
        challengeCD = dpd->challengeTime + TD_CHALLENGE_TIMEUNIT - TimeUtil::Now();
    st << dpd->curLevel << dpd->maxLevel << dpd->deamonLevel << leftTime << challengeCD;
    st << awards << accLeft << vitality << spirit;
    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::showLevelTown(Player* pl, UInt16 level)
{
    if(level > m_Monsters.size() || level == 0)
    {
        return;
    }

    Int16 idx = level - 1;
    Player* pl2 = m_Monsters[idx].player;
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x01);
    if(!pl2)
    {
        st << level << "" << static_cast<UInt8>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0);
    }
    else
    {
        DeamonPlayerData* dpd = pl2->getDeamonPlayerData();

        UInt32 leftTime = 0;
        if(TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM > TimeUtil::Now())
            leftTime = TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM - TimeUtil::Now();
        else
            leftTime = TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM + 86400 - TimeUtil::Now();

        st << level << pl2->getName() << pl2->GetLev();
        st << leftTime;
        st << dpd->calcAwards();
        st << dpd->calcSpirit();
    }

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::useAccItem(Player* pl, UInt8 count)
{
    if(!checkTownDeamon(pl))
        return;

    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x03);

    UInt8 res = 0;

    UInt32 leftTime = 0;
    if(TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM > TimeUtil::Now())
        leftTime = TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM - TimeUtil::Now();
    else
        leftTime = TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM + 86400 - TimeUtil::Now();
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    UInt32 accLeft = dpd->calcAccLeft();
    if(dpd->deamonLevel == 0 || accLeft > TD_MAXACCTIME)
    {
        res = 2;
    }
    else
    {
        UInt32 accLeft = dpd->calcAccLeft();
        UInt32 need = (TD_MAXACCTIME - accLeft)/86400 + ((TD_MAXACCTIME - accLeft)%86400 > 0 ? 1 : 0);

        if(need > count)
            need = count;

        if(pl->GetPackage()->GetItemAnyNum(ACC_ITEM) < need)
            need = pl->GetPackage()->GetItemAnyNum(ACC_ITEM);
        if(need == 0)
            res = 1;
        else
        {
            pl->GetPackage()->DelItemAny(ACC_ITEM, need);
            if(dpd->accTime != 0 && accLeft == 0)
            {
                dpd->accAwards += dpd->accLen/TD_AWARD_TIMEUNIT;
                dpd->accLen = need * 86400;
                dpd->accTime = TimeUtil::Now();
            }
            else
            {
                dpd->accLen += /*dpd->calcAccLeft() + */need * 86400;
            }

            //dpd->accTime = TimeUtil::Now();
            if(dpd->calcAccLeft() > TD_MAXACCTIME)
                dpd->accLen = TD_MAXACCTIME + TimeUtil::Now() - dpd->accTime;

            DB3().PushUpdateData("UPDATE `towndeamon_player` SET `accTime`=%u,`accLen`=%u, `accAwards`=%u WHERE `playerId` = %"I64_FMT"u", dpd->accTime, dpd->accLen, dpd->accAwards, pl->getId());
        }
    }

    accLeft = dpd->calcAccLeft();
    st << res << accLeft;

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::useVitalityItem(Player* pl, UInt8 count)
{
    if(!checkTownDeamon(pl))
        return;

    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x04);

    UInt8 res = 0;

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    UInt32 spirit = dpd->calcSpirit();
    UInt32 vitality = dpd->calcVitality();
    if(dpd->deamonLevel == 0 || vitality >= TD_MAXVITALITY)
    {
        res = 2;
    }
    else
    {
        UInt32 tmp = TD_MAXVITALITY - (100 - spirit + vitality);
        UInt32 need = tmp / 20 + (tmp % 20 > 0 ? 1 : 0);
        if(need > count)
            need = count;

        if(pl->GetPackage()->GetItemAnyNum(VITALITY_ITEM) < need)
            need = pl->GetPackage()->GetItemAnyNum(VITALITY_ITEM);
        if(need == 0)
            res = 1;
        else
        {
            UInt32 tmpVitality = need * 20;
            if(tmpVitality >= 100 - spirit)
            {
                tmpVitality -= 100 - spirit;
                dpd->vitality = vitality + tmpVitality;
                dpd->spirit = 100;
            }
            else
            {
                dpd->spirit = spirit + tmpVitality;
                dpd->vitality = 0;
            }

            pl->GetPackage()->DelItemAny(VITALITY_ITEM, need);
            dpd->vitalityTime = TimeUtil::Now();
            if(dpd->vitality > TD_MAXVITALITY)
                dpd->vitality = TD_MAXVITALITY;

            DB3().PushUpdateData("UPDATE `towndeamon_player` SET `spirit`=%u,`vitality`=%u, `vitalityTime`=%u WHERE `playerId` = %"I64_FMT"u", dpd->spirit, dpd->vitality, dpd->vitalityTime, pl->getId());
        }
    }

    st << res << dpd->vitality;
    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::cancelDeamon(Player* pl)
{
    if(!checkTownDeamon(pl))
        return;


    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x06);
    UInt8 res = 0;

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    UInt16 idx = dpd->deamonLevel - 1;

    if(dpd->deamonLevel == 0)
        res = 1;
    else if(dpd->inChallenge)
        res = 1;
    else if(m_Monsters[idx].player != pl)
    {
        res = 1;
        dpd->vitality = dpd->calcVitality();
        dpd->accLen = dpd->calcAccLeft();
        dpd->deamonLevel = 0;
        dpd->accTime = 0;
        dpd->vitalityTime = 0;
        dpd->accAwards = 0;
        dpd->spirit = 100;

        showTown(pl);
        DB3().PushUpdateData("UPDATE `towndeamon_player` SET `deamonLevel`=%u, `startTime`=%u, `accTime`=%u, `vitalityTime`=%u, `accAwards`=%u, `vitality`=%u, `spirit`=%u, `accLen`=%u WHERE `playerId` = %"I64_FMT"u", dpd->deamonLevel, dpd->startTime, dpd->accTime, dpd->vitalityTime, dpd->accAwards, dpd->vitality, dpd->spirit, dpd->accLen, pl->getId());
    }
    else
    {
        quitDeamon(pl, pl);
    }

    st << Stream::eos;
    pl->send(st);
}

bool TownDeamon::attackNpc(Player* pl, UInt32 npcId)
{
    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now)
    {
        pl->sendMsgCode(0, 1407, buffLeft - now);
        return false;
    }
    pl->checkLastBattled();
    GData::NpcGroups::iterator it = GData::npcGroups.find(npcId);
    if(it == GData::npcGroups.end())
        return false;

    GData::NpcGroup * ng = it->second;
    Battle::BattleSimulator bsim(/*pl->getLocation()*/Battle::BS_COPY5, pl, ng->getName(), ng->getLevel(), false);
    pl->PutFighters( bsim, 0 );
    ng->putFighters( bsim );
    bsim.start();
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return false;

    UInt16 ret = 0x0100;
    bool res = bsim.getWinner() == 1;
    if(res)
    {
        ret = 0x0101;
        pl->_lastNg = ng;
        pl->pendExp(ng->getExp());
        ng->getLoots(pl, pl->_lastLoot, 0, NULL);
    }

    Stream st(REP::ATTACK_NPC);
    st << ret << PLAYER_DATA(pl, lastExp) << static_cast<UInt8>(0);
    UInt8 sz = pl->_lastLoot.size();
    st << sz;
    for(UInt8 i = 0; i < sz; ++ i)
    {
        st << pl->_lastLoot[i].id << pl->_lastLoot[i].count;
    }
    st.append(&packet[8], packet.size() - 8);
    st << Stream::eos;
    pl->send(st);

    bsim.applyFighterHP(0, pl);

    pl->setBuffData(PLAYER_BUFF_ATTACKING, now + bsim.getTurns());
    return res;
}

void TownDeamon::attackPlayer(Player* pl, Player* defer)
{
	UInt8 tid = defer->getThreadId();
	if(tid == pl->getThreadId())
	{
		bool res;

		Battle::BattleSimulator bsim(/*pl->getLocation()*/Battle::BS_COPY5, pl, defer);
		pl->PutFighters( bsim, 0, true );
		defer->PutFighters( bsim, 1, true );
        DeamonPlayerData* deferDpd = defer->getDeamonPlayerData();
        UInt32 spirit = deferDpd->calcSpirit();
        float factor = static_cast<float>(spirit)/100.0f;
        defer->setSpiritFactor(factor);
		bsim.start();
		res = bsim.getWinner() == 1;
        defer->setSpiritFactor(1.0f);

		Stream st(REP::ATTACK_NPC);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
		pl->send(st);

        notifyChallengeResult(pl, defer, res);

		return;
	}
	struct TDBeAttackData
	{
		Player * attacker;
		UInt16 formation;
		UInt16 portrait;
		Lineup lineup[5];
	};
	TDBeAttackData tdbad = { pl, pl->getFormation(), static_cast<UInt16>(pl->getMainFighter() != NULL ? pl->getMainFighter()->getId() : 0) };
	for(int i = 0; i < 5; ++ i)
		tdbad.lineup[i] = pl->getLineup(i);
	GameMsgHdr hdr(0x340, tid, defer, sizeof(TDBeAttackData));
	GLOBAL().PushMsg(hdr, &tdbad);
}

void TownDeamon::beAttackByPlayer(Player* defer, Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup)
{
	Battle::BattleSimulator bsim(/*atker->getLocation()*/Battle::BS_COPY5, atker, defer);
	bsim.setFormation( 0, formation );
	bsim.setPortrait( 0, portrait );
	for(int i = 0; i < 5; ++ i)
	{
		if(lineup[i].fighter != NULL)
		{
			Battle::BattleFighter * bf = bsim.newFighter(0, lineup[i].pos, lineup[i].fighter);
			bf->setHP(0);
		}
	}
	defer->PutFighters( bsim, 1, true );
    DeamonPlayerData* deferDpd = defer->getDeamonPlayerData();
    UInt32 spirit = deferDpd->calcSpirit();
    float factor = static_cast<float>(spirit)/100.0f;
    defer->setSpiritFactor(factor);
	bsim.start();
	bool res = bsim.getWinner() == 1;
    defer->setSpiritFactor(1.0f);

	Stream st(REP::ATTACK_NPC);
	st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId() << Stream::eos;
	atker->send(st);

	struct TDResNotify
	{
		Player * peer;
		bool win;
	};

	TDResNotify notify = { defer, res };
	GameMsgHdr hdr2(0x341, atker->getThreadId(), atker, sizeof(TDResNotify));
	GLOBAL().PushMsg(hdr2, &notify);
}

void TownDeamon::challenge(Player* pl, UInt16 level, UInt8 type)
{
    if(!checkTownDeamon(pl))
        return;

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    UInt16 idx = level - 1;
    switch(type)
    {
    case 0:
        {
            if(level != dpd->curLevel + 1)
                break;
            else
            {
                UInt8 res = 0;
                if(attackNpc(pl, m_Monsters[idx].npcId))
                {
                    if (level > m_maxLevel)
                    {
                        SYSMSG_BROADCASTV(2337, pl->getCountry(), pl->getName().c_str(), level);
                        m_maxLevel = level;
                    }

                    pl->townDeamonUdpLog(level); // 上报挑战成功的udp日志

                    res = 0;
                    ++ dpd->curLevel;
                    if(dpd->startTime == 0)
                        dpd->startTime = TimeUtil::Now();

                    if(dpd->maxLevel == 0)
                    {
                        ++ dpd->maxLevel;
                        DB3().PushUpdateData("INSERT INTO `towndeamon_player` ( `deamonLevel`, `curLevel`, `maxLevel`, `time2MaxLvl`, `playerId`, `startTime`, `accTime`, `accLen`, `accAwards`, `vitalityTime`, `vitality`, `spirit`, `challengeTime`, `itemId`, `itemNum`, `quitLevel`, `attacker`) VALUES (%u, %u, %u, %u, %"I64_FMT"u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %"I64_FMT"u) ", dpd->deamonLevel, dpd->curLevel, dpd->maxLevel, TimeUtil::Now(), pl->getId(), dpd->startTime, dpd->accTime, dpd->accLen, dpd->accAwards, dpd->vitalityTime, dpd->vitality, dpd->spirit, dpd->challengeTime, dpd->itemId, dpd->itemNum, dpd->quitLevel, dpd->attacker);
                    }
                    else
                    {
                        if(dpd->curLevel > dpd->maxLevel)
                        {
                            dpd->maxLevel = dpd->curLevel;
                            DB3().PushUpdateData("UPDATE `towndeamon_player` SET `curLevel`=%u, `maxLevel`=%u, `time2MaxLvl`=%u, `startTime`=%u WHERE `playerId` = %"I64_FMT"u", dpd->curLevel, dpd->maxLevel, TimeUtil::Now(), dpd->startTime, pl->getId());
                        }
                        else
                            DB3().PushUpdateData("UPDATE `towndeamon_player` SET `curLevel`=%u, `startTime`=%u  WHERE `playerId` = %"I64_FMT"u", dpd->curLevel, dpd->startTime, pl->getId());
                    }

                    UInt32 thisDay = TimeUtil::SharpDay();
                    UInt32 seventhDay = TimeUtil::SharpDay(6, PLAYER_DATA(pl, created));
                    if(20 == dpd->curLevel && thisDay == seventhDay && !pl->GetVar(VAR_CLAWARD2))
                    {
                        pl->SetVar(VAR_CLAWARD2, 1);
                        pl->sendRC7DayInfo(TimeUtil::Now());
                    }
                    if (20 == dpd->curLevel)
                        pl->setContinuousRFAward(7);
                }
                else
                    res = 1;
                Stream st(REP::TOWN_DEAMON);
                st << static_cast<UInt8>(0x05);
                st << level << type << res << Stream::eos;
                pl->send(st);
            }

        }
        break;
    case 1:
        {
            Player* def = m_Monsters[idx].player;
            if(dpd->inChallenge || level == 0 || 0 != dpd->deamonLevel || level > dpd->curLevel || TimeUtil::Now() - dpd->challengeTime < TD_CHALLENGE_TIMEUNIT)
                break;

            if(def)
            {
                DeamonPlayerData* deferDpd = def->getDeamonPlayerData();
                if(deferDpd->inChallenge)
                    break;

                dpd->inChallenge = true;
                deferDpd->inChallenge = true;
                if (def != pl && def->getDeamonPlayerData() && def->getDeamonPlayerData()->deamonLevel)
                    attackPlayer(pl, def);
            }
            else
            {
                dpd->challengeTime = TimeUtil::Now();
                DB3().PushUpdateData("UPDATE `towndeamon_player` SET `challengeTime`=%u WHERE `playerId` = %"I64_FMT"u", dpd->challengeTime, pl->getId());

                occupyDeamon(pl, level);
                UInt8 res = 0;
                UInt32 challengeCD = 0;
                if(dpd->challengeTime + TD_CHALLENGE_TIMEUNIT > TimeUtil::Now())
                    challengeCD = dpd->challengeTime + TD_CHALLENGE_TIMEUNIT - TimeUtil::Now();
                Stream st(REP::TOWN_DEAMON);
                st << static_cast<UInt8>(0x05);
                st << level << type << res << challengeCD << Stream::eos;
                pl->send(st);
            }
        }
        break;
    }

    addActivity(pl);
}

void TownDeamon::notifyChallengeResult(Player* pl, Player* defer, bool win)
{
    Stream st(REP::TOWN_DEAMON);

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    DeamonPlayerData* deferDpd = defer->getDeamonPlayerData();

    dpd->inChallenge = false;
    deferDpd->inChallenge = false;

    UInt16 level = deferDpd->deamonLevel;
//    if (!level) // XXX:
//        level = deferDpd->quitLevel;
    if (!level)
        return;
    UInt8 res = 0;
    UInt16 idx = level - 1;

    dpd->challengeTime = TimeUtil::Now();
    if(win)
    {
        if(TimeUtil::Now() - deferDpd->startTime >= 3600)
        {
            pl->GetPackage()->AddItem2(m_Monsters[idx].itemId, 1, true, false, FromTownDeamon);
            SYSMSG_BROADCASTV(2338, pl->getCountry(), pl->getName().c_str(), level, defer->getCountry(), defer->getName().c_str(), m_Monsters[idx].itemId);
        }

        quitDeamon(defer, pl);
        occupyDeamon(pl, level);
    }
    else
    {
        res = 1;
    }

    UInt32 challengeCD = 0;
    if(dpd->challengeTime + TD_CHALLENGE_TIMEUNIT > TimeUtil::Now())
        challengeCD = dpd->challengeTime + TD_CHALLENGE_TIMEUNIT - TimeUtil::Now();

    DB3().PushUpdateData("UPDATE `towndeamon_player` SET `challengeTime`=%u WHERE `playerId` = %"I64_FMT"u", dpd->challengeTime, pl->getId());
    st << static_cast<UInt8>(0x05);
    st << level << static_cast<UInt8>(1) << res << challengeCD << Stream::eos;
    pl->send(st);
}

void TownDeamon::autoCompleteQuite(Player* pl, UInt16 levels)
{
    if(!checkTownDeamon(pl))
        return;

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    UInt16 curLevelTmp = dpd->curLevel;

    UInt32 maxCnt = levels;
    if(levels > dpd->maxLevel - dpd->curLevel)
        maxCnt = dpd->maxLevel - dpd->curLevel;

    UInt8 res = 0;
    if(maxCnt == 0)
        res = 2;
    if(pl->getTael() < maxCnt * TD_AUTO_TAEL)
        maxCnt = 0;

    if(maxCnt == 0)
        res = 1;

    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x07) << res;
    if(res == 0)
    {
		ConsumeInfo ci(TownDeamonAuto,0,0);
		pl->useTael(maxCnt * TD_AUTO_TAEL,&ci);
        st << static_cast<UInt16>(dpd->curLevel + maxCnt);
        for(UInt32 idx = 0; idx < maxCnt ; ++idx)
        {
            GData::NpcGroups::iterator it = GData::npcGroups.find(m_Monsters[idx + dpd->curLevel].npcId);
            if(it == GData::npcGroups.end())
                continue;

            GData::NpcGroup * ng = it->second;
            pl->pendExp(ng->getExp());
            ng->getLoots(pl, pl->_lastLoot, 0, NULL);
            pl->townDeamonUdpLog(dpd->curLevel + idx);
        }
    }

    if(maxCnt != 0)
    {
        if(dpd->startTime == 0)
            dpd->startTime = TimeUtil::Now();
        dpd->curLevel += maxCnt;
        DB3().PushUpdateData("UPDATE `towndeamon_player` SET `curLevel`=%u, `startTime`=%u WHERE `playerId` = %"I64_FMT"u", dpd->curLevel, dpd->startTime, pl->getId());

        UInt32 thisDay = TimeUtil::SharpDay();
        UInt32 seventhDay = TimeUtil::SharpDay(6, PLAYER_DATA(pl, created));
        if(curLevelTmp < 20 && dpd->curLevel >= 20 && thisDay == seventhDay && !pl->GetVar(VAR_CLAWARD2))
        {
            pl->SetVar(VAR_CLAWARD2, 1);
            pl->sendRC7DayInfo(TimeUtil::Now());
        }
        if (curLevelTmp < 20 && dpd->curLevel >= 20)
            pl->setContinuousRFAward(7);
    }
    st << pl->getPendExp();
    UInt16 sz = pl->_lastLoot.size();
    st << sz;
    for(UInt8 i = 0; i < sz; ++ i)
    {
        st << static_cast<UInt16>(pl->_lastLoot[i].id) << static_cast<UInt8>(pl->_lastLoot[i].count);
    }

    pl->checkLastBattled();
    st << Stream::eos;
    pl->send(st);
    addActivity(pl);
}

void TownDeamon::process()
{
    int cnt = m_Monsters.size();
    for(int i = 0; i < cnt; ++ i)
    {
        Player* pl = m_Monsters[i].player;
        if(!pl)
            continue;

        //DeamonPlayerData* dpd = pl->getDeamonPlayerData();

        //if(TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM <= TimeUtil::Now() && dpd->startTime <= TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM)
        quitDeamon(pl, NULL);
    }
}

bool TownDeamon::checkTownDeamon(Player* pl)
{
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    if(dpd->itemId != 0)
        return false;

    return true;
}

void TownDeamon::quitDeamon(Player* pl, Player* attacker)
{
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    if(dpd->deamonLevel == 0)
        return;

    UInt32 awards = dpd->calcAwards();
    if(attacker != NULL || pl == attacker)
    {
#if 0
        awards = (awards >> 1) + (awards & 1);
#else
        UInt32 awardsNumerator;
        awardsNumerator = awards * dpd->calcSpirit();
        awards = awardsNumerator / 100;
#endif
    }
    UInt16 idx = dpd->deamonLevel - 1;
    if(awards > 0)
    {
        dpd->itemId = m_Monsters[idx].itemId;
        dpd->itemNum = m_Monsters[idx].itemNum * awards;
    }
    dpd->quitLevel = dpd->deamonLevel;
    dpd->attacker = attacker;
    pl->sendDeamonAwardsInfo();

    if(NULL == attacker)
    {
        dpd->curLevel = 0;
        dpd->startTime = 0;
    }
    dpd->vitality = dpd->calcVitality();
    dpd->accLen = dpd->calcAccLeft();
    m_Monsters[idx].player = NULL;
    dpd->deamonLevel = 0;
    dpd->accTime = 0;
    dpd->vitalityTime = 0;
    dpd->accAwards = 0;
    dpd->spirit = 100;

    UInt64 attackerId = 0;
    if(attacker)
        attackerId = attacker->getId();

    showTown(pl);
    DB3().PushUpdateData("UPDATE `towndeamon_player` SET `deamonLevel`=%u, `curLevel`=%u, `startTime`=%u, `accTime`=%u, `vitalityTime`=%u, `accAwards`=%u, `vitality`=%u, `spirit`=%u, `accLen`=%u, `itemId`=%u, `itemNum`=%u, `quitLevel`=%u, `attacker`=%"I64_FMT"u  WHERE `playerId` = %"I64_FMT"u", dpd->deamonLevel, dpd->curLevel, dpd->startTime, dpd->accTime, dpd->vitalityTime, dpd->accAwards, dpd->vitality, dpd->spirit, dpd->accLen, dpd->itemId, dpd->itemNum, dpd->quitLevel, attackerId, pl->getId());
}

void TownDeamon::occupyDeamon(Player* pl, UInt16 level)
{
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    if(level > m_maxDeamonLevel)
        m_maxDeamonLevel = level;

    UInt16 idx = level - 1;
    m_Monsters[idx].player = pl;
    dpd->deamonLevel = level;
    UInt32 tmNow = TimeUtil::Now();
    dpd->startTime = tmNow;

    dpd->accTime = tmNow;
    dpd->vitalityTime = tmNow;

    showTown(pl);
    DB3().PushUpdateData("UPDATE `towndeamon_player` SET `deamonLevel`=%u,`startTime`=%u, `accTime`=%u, `vitalityTime`=%u WHERE `playerId` = %"I64_FMT"u", dpd->deamonLevel, dpd->startTime, dpd->accTime, dpd->vitalityTime, pl->getId());
}

void TownDeamon::checkStartTime(Player* pl)
{
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    if(dpd->startTime == 0)
        return;

    if((TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM <= TimeUtil::Now() && dpd->startTime <= TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM)
            || (TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM > dpd->startTime && TimeUtil::SharpDayT(0) + TOWNDEAMONENDTM - dpd->startTime > 86400)
                )
    {
        if(dpd->deamonLevel == 0 && dpd->curLevel != 0)
        {
            dpd->startTime = 0;
            dpd->curLevel = 0;
            showTown(pl);
            DB3().PushUpdateData("UPDATE `towndeamon_player` SET `startTime`=0, `curLevel`=0 WHERE `playerId` = %"I64_FMT"u", pl->getId());
        }
    }
}

void TownDeamon::addActivity(Player* pl)
{
    if (!pl->GetVar(VAR_TOWNDEAMON))
    {
        if(pl->getThreadId() != WORKER_THREAD_NEUTRAL)
        {
            stActivityMsg msg;
            msg.id = AtyTownDeamon;
            GameMsgHdr hdr2(0x245, pl->getThreadId(), pl, sizeof(stActivityMsg));
            GLOBAL().PushMsg(hdr2, &msg);
        }
        else
        {
            GameAction()->doAty(pl, AtyTownDeamon, 0, 0);
        }
        pl->SetVar(VAR_TOWNDEAMON, 1);
    }
}

}



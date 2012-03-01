#include "TownDeamon.h"
#include "Player.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "Package.h"
#include "Server/Cfg.h"
#include "GData/Money.h"
#include "Country.h"
#include "Script/GameActionLua.h"


namespace GObject
{

#define ACC_ITEM        465
#define VITALITY_ITEM   466
#define TD_AUTO_ITEM    467

TownDeamon* townDeamonManager = new TownDeamon();


UInt32 DeamonPlayerData::calcAccLeft()
{
    UInt32 accLeft = 0;
    if(accLen > TimeUtil::Now() - accTime)
        accLeft = accLen - TimeUtil::Now() + accTime;
    return accLeft;
}

UInt32 DeamonPlayerData::calcAwards()
{
    UInt32 awards = 0;
    UInt32 accLeft = 0;
    if(accLen > TimeUtil::Now() - accTime)
        accLeft = accLen - TimeUtil::Now() + accTime;

    if(startTime != 0)
        awards = (TimeUtil::Now() - startTime + accLen - accLeft)/TD_AWARD_TIMEUNIT + accAwards;
    return awards;
}

UInt32 DeamonPlayerData::calcVitality()
{
    UInt32 vitalityLeft = 0;
    if(vitality * TD_VITALITY_TIMEUNIT > TimeUtil::Now() - vitalityTime)
        vitalityLeft = vitality - (TimeUtil::Now() + vitalityTime) / TD_VITALITY_TIMEUNIT;

    return vitalityLeft;
}

UInt32 DeamonPlayerData::calcSpirit()
{
    UInt32 spiritLeft = 0;
    if(vitality * TD_VITALITY_TIMEUNIT > TimeUtil::Now() - vitalityTime)
        spiritLeft = spirit;
    else if((spirit + vitality) * TD_VITALITY_TIMEUNIT > (TimeUtil::Now() - vitalityTime))
        spiritLeft = spirit + vitality - (TimeUtil::Now() - vitalityTime) / TD_VITALITY_TIMEUNIT;

    return spiritLeft;
}


TownDeamon::TownDeamon()
{
    m_Monsters.clear();
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

void TownDeamon::loadDeamonPlayersFromDB(UInt16 level, Player* pl)
{
    if(level > m_Monsters.size() || level == 0)
        printf("TownDeamon: loadDeamonPlayerFromDB Error\n");
    DeamonMonster& dm = m_Monsters[level];
    dm.player = pl;
}

void TownDeamon::listDeamons(Player* pl, UInt16 start, UInt16 count)
{
    if(start > m_Monsters.size())
        return;

    UInt16 total = m_Monsters.size();
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x02);
    st << total << start << count;
    UInt16 end = start + count;
    if(end > m_Monsters.size())
        end = m_Monsters.size();

    UInt8 pos = st.size() - 1;
    UInt8 cnt = 0;
    for(Int16 idx = start - 1; idx < end; ++idx)
    {
        ++ cnt;
        Player* pl2 = m_Monsters[idx].player;
        if(pl2)
        {
            DeamonPlayerData* dpd = pl2->getDeamonPlayerData();
            UInt32 spirit = dpd->calcSpirit();
            st << idx << pl2->getName() << pl2->GetLev() << spirit;
        }
        else
            st << idx << "" << static_cast<UInt16>(0) << static_cast<UInt8>(0) << static_cast<UInt32>(0);
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
    if(TimeUtil::SharpDayT(TOWNDEAMONENDTM) > TimeUtil::Now())
        leftTime = TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();
    else
        leftTime = TimeUtil::SharpDayT(TOWNDEAMONENDTM) + TD_MAXACCTIME - TimeUtil::Now();

    st << dpd->curLevel << dpd->maxLevel << dpd->deamonLevel << leftTime;
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
        if(TimeUtil::SharpDayT(TOWNDEAMONENDTM) > TimeUtil::Now())
            leftTime = TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();
        else
            leftTime = TimeUtil::SharpDayT(TOWNDEAMONENDTM) + TD_MAXACCTIME - TimeUtil::Now();

        st << dpd->curLevel << pl2->getName() << pl2->GetLev();
        st << leftTime;
        st << dpd->calcAwards();
        st << dpd->calcSpirit();
    }

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::useAccItem(Player* pl, UInt8 count)
{
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x03);

    UInt8 res = 0;

    UInt32 leftTime = 0;
    if(TimeUtil::SharpDayT(TOWNDEAMONENDTM) > TimeUtil::Now())
        leftTime = TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();
    else
        leftTime = TimeUtil::SharpDayT(TOWNDEAMONENDTM) + TD_MAXACCTIME - TimeUtil::Now();
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    if(dpd->deamonLevel == 0 || dpd->accTime >= leftTime)
    {
        res = 2;
    }
    else
    {
        UInt32 accLeft = dpd->calcAccLeft();
        UInt32 need = (TD_MAXACCTIME - accLeft)/3600 + ((TD_MAXACCTIME - accLeft)%3600 > 0 ? 1 : 0);

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
                dpd->accLen = need * 3600;
            }
            else
            {
                dpd->accLen = dpd->calcAccLeft() + need * 3600;
            }

            dpd->accTime = TimeUtil::Now();
            if(dpd->accLen > TD_MAXACCTIME)
                dpd->accLen = TD_MAXACCTIME;

            DB3().PushUpdateData("UPDATE `towndeamon_player` SET `accTime`=%u,`accLen`=%u, `accAwards`=%u WHERE `playerId` = %"I64_FMT"u", dpd->accTime, dpd->accLen, dpd->accAwards, pl->getId());
        }
    }

    UInt32 accLeft = dpd->calcAccLeft();
    st << res << accLeft;

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::useVitalityItem(Player* pl, UInt8 count)
{
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x04);

    UInt8 res = 0;

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    if(dpd->deamonLevel == 0 || dpd->vitality >= TD_MAXVITALITY)
    {
        res = 2;
    }
    else
    {
        UInt32 spirit = dpd->calcSpirit();
        UInt32 vitality = dpd->calcVitality();

        UInt32 tmp = 100 - spirit + vitality;
        UInt32 need = tmp / 100 + (tmp % 100 > 0 ? 1 : 0);
        if(need > count)
            need = count;

        if(pl->GetPackage()->GetItemAnyNum(VITALITY_ITEM) < need)
            need = pl->GetPackage()->GetItemAnyNum(VITALITY_ITEM);
        if(need == 0)
            res = 1;
        else
        {
            UInt32 tmpVitality = need * 100;
            if(tmpVitality >= 100 - spirit)
            {
                tmpVitality -= 100 - spirit;
                dpd->vitality = vitality + tmpVitality;
            }
            else
            {
                dpd->spirit = spirit + tmpVitality;
                dpd->vitality = 0;
            }

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
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x06);
    UInt8 res = 0;

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    if(dpd->deamonLevel == 0)
        res = 1;
    else if(m_Monsters[dpd->deamonLevel - 1].player != pl)
        res = 1;
    else
    {
        quitDeamon(pl);
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
    Battle::BattleSimulator bsim(pl->getLocation(), pl, ng->getName(), ng->getLevel(), false);
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

		Battle::BattleSimulator bsim(pl->getLocation(), pl, defer);
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
	Battle::BattleSimulator bsim(atker->getLocation(), atker, defer);
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
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    switch(type)
    {
    case 0:
        {
            if(level != dpd->curLevel)
                break;
            else
            {
                UInt8 res = 0;
                if(attackNpc(pl, m_Monsters[level].npcId))
                    res = 0;
                else
                    res = 1;
                Stream st(REP::TOWN_DEAMON);
                st << static_cast<UInt8>(0x06);
                st << level << type << res << Stream::eos;
                pl->send(st);
            }

        }
        break;
    case 1:
        {
            if(level == 0 || 0 != dpd->deamonLevel || level > dpd->curLevel || TimeUtil::Now() - dpd->challengeTime < TD_CHALLENGE_TIMEUNIT)
                break;
            else if(m_Monsters[level].player)
            {
                attackPlayer(pl, m_Monsters[level].player);
            }
            else
            {
                occupyDeamon(pl, level);
                UInt8 res = 0;
                Stream st(REP::TOWN_DEAMON);
                st << static_cast<UInt8>(0x06);
                st << level << type << res << Stream::eos;
                pl->send(st);
            }
        }
        break;
    }
}

void TownDeamon::notifyChallengeResult(Player* pl, Player* defer, bool win)
{
    Stream st(REP::TOWN_DEAMON);

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    DeamonPlayerData* deferDpd = defer->getDeamonPlayerData();
    UInt16 level = deferDpd->deamonLevel;
    UInt8 res = 0;

    dpd->challengeTime = TimeUtil::Now();
    if(win)
    {
        if(TimeUtil::Now() - deferDpd->startTime >= 3600)
            pl->GetPackage()->AddItem2(m_Monsters[level].itemId, 1, true, true, FromTownDeamon);

        quitDeamon(defer);
        occupyDeamon(pl, level);
    }
    else
    {
        res = 1;
    }

    DB3().PushUpdateData("UPDATE `towndeamon_player` SET `challengeTime`=%u WHERE `playerId` = %"I64_FMT"u", dpd->challengeTime, pl->getId());
    st << static_cast<UInt8>(0x06);
    st << level << static_cast<UInt8>(1) << res << Stream::eos;
    pl->send(st);
}

void TownDeamon::autoCompleteQuite(Player* pl, UInt16 levels)
{
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    int maxCnt = levels;
    if(levels > dpd->maxLevel - dpd->curLevel)
        maxCnt = dpd->maxLevel - dpd->curLevel;

    UInt8 res = 0;
    if(maxCnt == 0)
        res = 2;
    if(pl->GetPackage()->GetItemAnyNum(TD_AUTO_ITEM) < maxCnt)
        maxCnt = pl->GetPackage()->GetItemAnyNum(TD_AUTO_ITEM);

    if(maxCnt == 0)
        res = 1;

    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x07) << res;
    if(res == 0)
    {
        st << static_cast<UInt16>(dpd->curLevel + maxCnt);
        for(int idx = 0; idx < maxCnt ; ++idx)
        {
            GData::NpcGroups::iterator it = GData::npcGroups.find(m_Monsters[idx + dpd->curLevel].npcId);
            if(it == GData::npcGroups.end())
                continue;

            GData::NpcGroup * ng = it->second;
            pl->pendExp(ng->getExp());
            ng->getLoots(pl, pl->_lastLoot, 0, NULL);
        }
    }

    st << pl->getPendExp();
    UInt16 sz = pl->_lastLoot.size();
    st << sz;
    for(UInt8 i = 0; i < sz; ++ i)
    {
        st << pl->_lastLoot[i].id << pl->_lastLoot[i].count;
    }

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::process()
{
    int cnt = m_Monsters.size();
    for(int i = 0; i < cnt; ++ i)
    {
        Player* pl = m_Monsters[i].player;
        if(!pl)
            continue;

        DeamonPlayerData* dpd = pl->getDeamonPlayerData();

        if(TimeUtil::SharpDayT(TOWNDEAMONENDTM) < TimeUtil::Now() && dpd->startTime < TimeUtil::SharpDayT(TOWNDEAMONENDTM))
            quitDeamon(pl, true);
    }
}

bool TownDeamon::checkTownDeamon(Player* pl)
{
    if(PLAYER_DATA(pl, location) != 0x1414)
        return false;

    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    if(dpd->itemId != 0)
        return false;

    return true;
}

void TownDeamon::quitDeamon(Player* pl, bool fullAward)
{
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    if(dpd->startTime == 0)
        return;

    UInt32 awards = dpd->calcAwards();
    if(!fullAward)
        awards /= 2;

    if(awards > 0)
    {
        dpd->itemId = m_Monsters[dpd->deamonLevel].itemId;
        dpd->itemNum = m_Monsters[dpd->deamonLevel].itemNum * awards;
    }
    //pl->GetPackage()->AddItem2(, , true, true, FromTownDeamon);

    m_Monsters[dpd->deamonLevel].player = NULL;
    dpd->deamonLevel = 0;
    dpd->startTime = 0;
    dpd->accTime = 0;
    dpd->vitalityTime = 0;
    dpd->accAwards = 0;
    dpd->vitality = dpd->calcVitality();
    dpd->spirit = dpd->calcSpirit();
    dpd->accLen = dpd->calcAccLeft();

    DB3().PushUpdateData("UPDATE `towndeamon_player` SET `deamonLevel`=%u,`startTime`=%u, `accTime`=%u, `vitalityTime`=%u, `accAwards`=%u, `vitality`=%u, `spirit`=%u, `accLen`=%u, `itemId`=%u, `itemNum`=%u WHERE `playerId` = %"I64_FMT"u", dpd->deamonLevel, dpd->startTime, dpd->accTime, dpd->vitalityTime, dpd->accAwards, dpd->vitality, dpd->spirit, dpd->accLen, dpd->itemId, dpd->itemNum, pl->getId());
}

void TownDeamon::occupyDeamon(Player* pl, UInt16 level)
{
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    m_Monsters[level].player = pl;
    dpd->deamonLevel = level;
    UInt32 tmNow = TimeUtil::Now();
    dpd->startTime = tmNow;

    dpd->accTime = tmNow;
    dpd->vitalityTime = tmNow;

    DB3().PushUpdateData("UPDATE `towndeamon_player` SET `deamonLevel`=%u,`startTime`=%u, `accTime`=%u, `vitalityTime`=%u WHERE `playerId` = %"I64_FMT"u", dpd->deamonLevel, dpd->startTime, dpd->accTime, dpd->vitalityTime, pl->getId());
}

}



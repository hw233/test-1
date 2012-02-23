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

TownDeamon* townDeamonManager = new TownDeamon();

TownDeamon::TownDeamon()
{
    m_Monsters.clear();
}

TownDeamon::~TownDeamon()
{
    m_Monsters.clear();
}

void TownDeamon::loadDeamonMonstersFromDB(UInt16 level, UInt32 npcId)
{
    DeamonMonster dm;
    dm.npcId = npcId;
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
            UInt32 spirit = dpd->vitality >= 0 ? 100 : (100 + dpd->vitality);
            if(spirit < 0)
                spirit = 0;
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

    UInt32 awards = 0;
    UInt32 accLeft = 0;
    UInt32 spirit = 0;
    UInt32 vitality = 0;
    UInt32 timeLeft = TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();

    if(dpd->accLen > TimeUtil::Now() - dpd->accTime)
        accLeft = dpd->accLen - TimeUtil::Now() + dpd->accTime;

    if(dpd->startTime != 0)
        awards = (TimeUtil::Now() - dpd->startTime + dpd->accLen - accLeft)/TD_AWARD_TIMEUNIT + dpd->accAwards;

    if(dpd->vitality * TD_VITALITY_TIMEUNIT > TimeUtil::Now() - dpd->vitalityTime)
        vitality = dpd->vitality - (TimeUtil::Now() + dpd->vitalityTime) / TD_VITALITY_TIMEUNIT;

    st << dpd->curLevel << dpd->maxLevel << dpd->deamonLevel << timeLeft;
    st << awards << accLeft << vitality << spirit;
    spirit = dpd->vitality >= 0 ? 100 : (100 + dpd->vitality);
    vitality = dpd->vitality > 0 ? dpd->vitality : 0;
    if(spirit < 0)
        spirit = 0;

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

        st << dpd->curLevel << pl2->getName() << pl2->GetLev();
        st << TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();
        st << dpd->awards;
        UInt32 spirit = dpd->vitality >= 0 ? 100 : (100 + dpd->vitality);
        if(spirit < 0)
            spirit = 0;
        st << spirit;
    }

    st << Stream::eos;
    pl->send(st);
}

void TownDeamon::useAccItem(Player* pl, UInt8 count)
{
    Stream st(REP::TOWN_DEAMON);
    st << static_cast<UInt8>(0x03);

    UInt8 res = 0;

    UInt32 leftTime = TimeUtil::SharpDayT(TOWNDEAMONENDTM) - TimeUtil::Now();
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();

    if(dpd->deamonLevel == 0 || dpd->accTime >= leftTime)
    {
        res = 2;
    }
    else
    {
        UInt32 tmpTime = TD_MAXACCTIME - dpd->accTime;
        UInt32 need = tmpTime / 3600 + ((tmpTime % 100) == 0 ? 0 : 1);
        if(need > count)
            need = count;
        if(pl->GetPackage()->GetItemAnyNum() < need)
            need = pl->GetPackage()->GetItemAnyNum();
        if(need == 0)
            res = 1;
        else
        {
            dpd->accTime += need * 3600;
            if(dpd->accTime > TD_MAXACCTIME)
                dpd->accTime = TD_MAXACCTIME;
        }
    }

    st << res << dpd->accTime;

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
        UInt32 tmpVitality = TD_MAXACCTIME - dpd->vitality;
        UInt32 need = tmpVitality / 100 + ((tmpVitality % 100) == 0 ? 0 : 1);
        if(need > count)
            need = count;
        if(pl->GetPackage()->GetItemAnyNum() < need)
            need = pl->GetPackage()->GetItemAnyNum();
        if(need == 0)
            res = 1;
        else
        {
            dpd->vitality += need * 100;
            if(dpd->vitality > TD_MAXVITALITY)
                dpd->vitality = TD_MAXVITALITY;
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
        m_Monsters[dpd->deamonLevel].player = NULL;
        dpd->deamonLevel = 0;
        dpd->startTime = 0;
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
    UIntew npcId = m_Monsters[level].npcId;
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
		bsim.start();
		res = bsim.getWinner() == 1;

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
		abd.lineup[i] = pl->getLineup(i);
	GameMsgHdr hdr(0x340, tid, defer, sizeof(TDBeAttackData));
	GLOBAL().PushMsg(hdr, &abd);
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
	bsim.start();
	bool res = bsim.getWinner() == 1;

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
            if(level == 0 || 0 != dpd->deamonLevel || level > dpd->curLevel)
                break;
            else if(m_Monsters[level].player)
            {
                attackPlayer(pl, m_Monsters[level].player);
            }
            else
            {
                m_Monsters[level].player = pl;
                dpd->deamonLevel = level;
                dpd->startTime = TimeUtil::Now();

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

    DeamonPlayerData* deferDpd = defer->getDeamonPlayerData();
    DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    UInt16 level = deferDpd->deamonLevel;
    UInt8 res = 0;

    dpd->challengeTime = TimeUtil::Now();
    if(win)
    {
        dpd->deamonLevel = level;
        dpd->startTime = TimeUtil::Now();

        deferDpd->deamonLevel = 0;
        deferDpd->startTime = 0;
    }
    else
    {
        res = 1;
    }

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

    if(pl->GetPackage()->GetItemAnyNum() < maxCnt)
        maxCnt = pl->GetPackage()->GetItemAnyNum();

    for(int idx = 0; idx < maxCnt ; ++idx)
    {
    }
}

void TownDeamon::process()
{
    int cnt = m_Monsters.size();
    for(int i = 0; i < cnt;; ++ i)
    {
        Player* pl = m_Monsters[i].player;
        if(!pl)
            continue;

        DeamonPlayerData* dpd = pl->getDeamonPlayerData();
    }
}

bool TownDeamon::checkTownDeamon(Player* pl)
{
    if(PLAYER_DATA(pl, location) != )
        return false

    return true;
}

}



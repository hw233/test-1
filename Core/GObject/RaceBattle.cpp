#include "Config.h"
#include "Common/Platform.h"
#include "Common/TimeUtil.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "GData/XingchenData.h"
#include "Common/URandom.h"
#include "Package.h"
#include "Server/SysMsg.h"
#include "Mail.h"
#include "Common/Itoa.h"
#include "Common/TimeUtil.h"
#include "Country.h"
#include "Script/GameActionLua.h"
#include "RaceBattle.h"

#define RACEBATTLE_STARTTIME 20*3600
//#define RACEBATTLE_STARTTIME ((TimeUtil::Now() - TimeUtil::SharpDay(0, now))/ 3600 * 3600)
#define RACEBATTLE_ENDTIME RACEBATTLE_STARTTIME+1800
#define PAGE_MAX 5

namespace GObject
{
    static UInt8 gPerLeveCnt[] = {2, 3, 4, 5, 7, 0};
    UInt8 RaceBattle::_status = 0;
    RBSortType RaceBattle::_levelStarSort[6];
    RBSortType RaceBattle::_contineWinSort;

    void raceBattleBroadcast(UInt8 status, UInt32 lefttime)
    {
        Stream st(REP::RACE_BATTLE);
        UInt8 type = 1;
        st << type;
        st << status;
        st << lefttime;
        st << Stream::eos;
        NETWORK()->Broadcast(st);
    }

    void RaceBattle::sendRBStatus(Player* pl)
    {
        if(!pl)
            return;
        if(_status == 0)
            return;
        UInt8 status;
        UInt32 lefttime;
        UInt32 now = TimeUtil::Now();
        UInt32 startTime = TimeUtil::SharpDay(0, now) + RACEBATTLE_STARTTIME;
        if(isStart())
        {
            status = 1;
            lefttime = startTime + 1800 - now;
        }
        else
        {
            status = 0;
            lefttime = startTime - now;
        }

        Stream st(REP::RACE_BATTLE);
        UInt8 type = 1;
        st << type;
        st << status;
        st << lefttime;
        st << Stream::eos;
        pl->send(st);
    }

    UInt32 second2min(UInt32 second)
    {
        UInt32 min = second / 60;
        if(min * 60 < second)
            ++min;
        return min;
    }

    void RaceBattle::totalAward(Player* pl)
    {
        if(!pl)
            return;
        UInt8 level = pl->getRaceBattlePos() / 10;
        if((level == 0 || level == 1) && (pl->getRaceBattlePos() % 10) == 0)
            return;

        UInt32 rank = 0;
        bool found = false;
        for(UInt8 i = 6; i >= 1; i--)
        {
            RBSortType& levelSort = _levelStarSort[i - 1];
            for(RBSortType::iterator it = levelSort.begin(); it != levelSort.end(); ++it)
            {
                ++rank;
                if(it->player == pl)
                {
                    found = true;
                    break;
                }
            }
            if(found)
                break;
        }
        if(found)
        {
            UInt8 type = 0xFF;
            if(rank == 0)
            {
            }
            else if(rank <= 1)
                type = 0;
            else if(rank <= 2)
                type = 1;
            else if(rank <= 3)
                type = 2;
            else if(rank <= 9)
                type = 3;
            else if(rank <= 29)
                type = 4;
            else if(rank <= 50)
                type = 5;
            else
            {
            }
            if(type <= 5)
            {
                static UInt32 achievement[] = {300, 220, 180, 150, 120, 100};
                pl->getAchievement(achievement[type]);
                pl->setTotalAchievement(pl->getTotalAchievement() + achievement[type]);
            }
        }

        UInt8 continueWinMaxCnt = pl->getContinueWinMaxCnt();
        {
            UInt8 num = continueWinMaxCnt;
            UInt8 type = 0xFF;
            if(num >= 90)
                type = 9;
            else if(num >= 80)
                type = 8;
            else if(num >= 70)
                type = 7;
            else if(num >= 60)
                type = 6;
            else if(num >= 50)
                type = 5;
            else if(num >= 40)
                type = 4;
            else if(num >= 30)
                type = 3;
            else if(num >= 20)
                type = 2;
            else if(num >= 10)
                type = 1;
            else if(num >= 5)
                type = 0;
            else
            {
            }
            if(type <= 9)
            {
                static UInt32 achievement[] = {40, 45, 50, 55, 60, 65, 70, 75, 80, 85};
                pl->getAchievement(achievement[type]);
                pl->setTotalAchievement(pl->getTotalAchievement() + achievement[type]);
            }
        }

        UInt8 totalWinCnt = pl->getTotalWinCnt();
        UInt8 totalLoseCnt = pl->getTotalLoseCnt();
        UInt32 totalAchievement = pl->getTotalAchievement();
        UInt8 itemCnt = pl->getTotalItemCnt();
        UInt64 totalExp = pl->getTotalExp();

        SYSMSG(title, 5140);
        if(found)
        {
            SYSMSGV(content, 5141, level, rank, continueWinMaxCnt, totalWinCnt, totalLoseCnt, totalAchievement, itemCnt, totalExp);
            pl->GetMailBox()->newMail(NULL, 0x1, title, content, 0xFFFE0000);
        }
        else
        {
            SYSMSGV(content, 5142, level, continueWinMaxCnt, totalWinCnt, totalLoseCnt, totalAchievement, itemCnt, totalExp);
            pl->GetMailBox()->newMail(NULL, 0x1, title, content, 0xFFFE0000);
        }
    }

    inline bool enum_reset_all(Player* pl, int)
    {
        if(!pl)
            return true;
        raceBattle.totalAward(pl);

        pl->setRaceBattlePos(0);
        for(UInt8 i = 0; i < 7; i++)
            pl->setStarCnt(i, 0);
        pl->setContinueWinCnt(0);
        pl->setAwardLevel(2);
        pl->clearChallengePlayer();
        pl->clearPlayerRecord();
        pl->setContinueWinPage(1);
        pl->setRBBuf(0, 0);
        pl->setExitCd(0);
        pl->setStarTotal(0);
        pl->setCanContinueCnt(0);
        pl->setContinueLoseCnt(0);
        pl->setAttackCd(0);
        pl->setIsLastLevel(false);
        pl->setMatchPlayer(NULL);
        pl->setContinueWinMaxCnt(0);
        pl->setTotalWinCnt(0);
        pl->setTotalLoseCnt(0);
        pl->setTotalAchievement(0);
        pl->setTotalItemCnt(0);
        pl->setTotalExp(0);
        if(pl->getLocation() == 1556)
            pl->cancelAutoRaceBattle();
        return true;
    }

    void RaceBattle::raceBattleCheck()
    {
        UInt32 now = TimeUtil::Now();
        UInt32 startTime = TimeUtil::SharpDay(0, now) + RACEBATTLE_STARTTIME;

        if(now < startTime - 600)
            _status = 0;
        else if(now < startTime - 300)
        {
            if(_status == 1)
                return;
            _status = 1;
            UInt32 min = second2min(startTime - now);
            SYSMSG_BROADCASTV(6005, min);
            raceBattleBroadcast(0, startTime - now);
        }
        else if(now < startTime - 60)
        {
            if(_status == 2)
                return;
            _status = 2;
            UInt32 min = second2min(startTime - now);
            SYSMSG_BROADCASTV(6005, min);
        }
        else if(now < startTime)
        {
            if(_status == 3)
                return;
            _status = 3;
            UInt32 min = second2min(startTime - now);
            SYSMSG_BROADCASTV(6005, min);
        }
        else if(now < startTime + 1800)
        {
            if(_status == 4)
                return;
            _status = 4;
            SYSMSG_BROADCASTV(6006);
            raceBattleBroadcast(1, startTime + 1800 - now);
        }
        else
        {
            if(_status == 4)
            {
                SYSMSG_BROADCASTV(6007);
                raceBattleBroadcast(2, 0);
                //awardLevelRank();
                globalPlayers.enumerate(enum_reset_all, 0);
                for(UInt8 i = 0; i < 6; i++)
                    _levelStarSort[0].clear();
                _contineWinSort.clear();
            }
            _status = 0;
        }
    }

    void RaceBattle::enterPos(Player* pl, UInt8 offset, bool fromServer)
    {
        UInt8 origPos = pl->getRaceBattlePos();
        UInt8 level = origPos / 10;
        if(level == 0)
            level = 1;
        if(level > sizeof(gPerLeveCnt) / sizeof(gPerLeveCnt[0]))
            return;
        if(offset == 0xFF || offset == 0)
            offset = origPos % 10;
        if(offset > gPerLeveCnt[level - 1])
            return;

        UInt8 pos = level * 10 + offset;
        GData::RandBattleData::stRandBattle* rb = GData::randBattleData.getRandBattleData(pos);
        if(!rb)
            return;

        if(level >= 2 && level <= 5)
        {
            UInt32 num = World::getRBTimeRank();
            UInt32 rank = GET_BIT_3(num, level);
            if(rank == 0)
            {
                ++rank;
                SYSMSG_BROADCASTV(6008 + level - 2, pl->getCountry(), pl->getPName());
                num = SET_BIT_3(num, level, rank);
                World::setRBTimeRank(num);
            }
        }

        if(level == sizeof(gPerLeveCnt) / sizeof(gPerLeveCnt[0]) && !pl->getIsLastLevel())
        {
            pl->sendMsgCode(0, 8018);
            pl->setIsLastLevel(true);
            UInt32 num = World::getRBTimeRank();
            UInt32 rank = GET_BIT_3(num, level);
            if(rank < 3)
            {
                ++rank;
                SYSMSG_BROADCASTV(6012 + rank - 1, pl->getCountry(), pl->getPName());
                num = SET_BIT_3(num, level, rank);
                World::setRBTimeRank(num);
            }
        }

        pl->setRaceBattlePos(pos);

        if(offset == 0)
        {
            if(fromServer)
                insertLevelStarSort(pl, level);
        }
        else if(level == 1 && (origPos % 10) == 0)
        {
            GameAction()->doStrong(pl, SthZixiaozhidian, 0, 0);
            insertLevelStarSort(pl, level);
            autoBattle(pl);
        }
        else
        {
        }

        UInt8 page = pl->getContinueWinPage();
        sendContinueWinSort(pl, page);
        sendOwnerInfo(pl);
        sendBattleInfo(pl);
    }

    void RaceBattle::autoBattle(Player* pl)
    {
        UInt32 now = TimeUtil::Now();
        UInt32 endTime = TimeUtil::SharpDay(0, now) + RACEBATTLE_ENDTIME;
        UInt32 count = 0;
        if(endTime > now)
            count = (endTime - now) / 60;
        if(count * 60 < endTime - now)
            ++count;
        if(count == 0)
            return;
        pl->autoRaceBattle(count);
    }

    void RaceBattle::cancelBattle(Player* pl)
    {
        pl->cancelAutoRaceBattle();
    }

    void RaceBattle::freshContinueWinRank(Player* pl)
    {
        sendContinueWinSort(pl, pl->getContinueWinPage());
    }

    void RaceBattle::getAward(Player* pl)
    {
        if(!pl)
            return;
        UInt8 awardlevel = pl->getAwardLevel();
        UInt8 pos = pl->getRaceBattlePos();
        UInt8 level = pos / 10;
        if(awardlevel < 2 || awardlevel > 6 || awardlevel > level)
            return;
        if(awardlevel >= 4)
        {
            if(pl->GetPackage()->GetRestPackageSize() < 1)
            {
                pl->sendMsgCode(0, 1011);
                return;
            }
            else
            {
                pl->GetPackage()->Add(9457, awardlevel - 3, true, false);
                pl->setTotalItemCnt(pl->getTotalItemCnt() + awardlevel - 3);
            }
        }
        UInt32 achievement = 100 * (awardlevel - 1);
        if(awardlevel == 6)
            achievement += 100;
        pl->getAchievement(achievement);
        pl->setTotalAchievement(pl->getTotalAchievement() + achievement);

        ++awardlevel;
        pl->setAwardLevel(awardlevel);

        Stream st(REP::RACE_BATTLE);
        UInt8 type = 8;
        st << type;
        st << awardlevel;
        st << Stream::eos;
        pl->send(st);
    }

    bool RaceBattle::requestMatch(Player* pl)
    {
        if(!pl)
            return false;
        UInt32 now = TimeUtil::Now();
        if(pl->getAttackCd() > now)
        {
            pl->sendMsgCode(0, 4045);
            return false;
        }
        if(pl->getExitCd() > TimeUtil::Now())
        {
            pl->sendMsgCode(0, 4040);
            return false;
        }
        UInt8 pos = pl->getRaceBattlePos();
        UInt8 level = pos / 10;
        if(level == 0)
        {
            return false;
        }
        if(level >= sizeof(gPerLeveCnt) / sizeof(gPerLeveCnt[0]))
        {
            return false;
        }
        UInt8 offset = pos % 10;
        if(offset == 0)
        {
            pl->sendMsgCode(0, 4044);
            return false;
        }
        if(offset > gPerLeveCnt[level - 1])
        {
            return false;
        }
        if(pl->getStarCnt(offset - 1) >= 6)
        {
            pl->sendMsgCode(0, 4043);
            return false;
        }

        std::vector<Player* > vecPlayer;
        UInt32 count = 0;
        for(UInt8 i = level; i <= 6; i++)
        {
            RBSortType& levelSort = _levelStarSort[i - 1];
            for(RBSortType::iterator it = levelSort.begin(); it != levelSort.end(); ++it)
            {
                if(it->player == pl)
                    continue;
                ++count;
                vecPlayer.push_back(it->player);
            }
            if(count >= 5)
                break;
        }
        for(UInt8 i = level - 1; i > 0; i--)
        {
            if(count >= 5)
                break;
            RBSortType& levelSort = _levelStarSort[i - 1];
            for(RBSortType::iterator it = levelSort.begin(); it != levelSort.end(); ++it)
            {
                if(it->player == pl)
                    continue;
                ++count;
                vecPlayer.push_back(it->player);
            }
        }
        if(count == 0)
        {
            pl->sendMsgCode(0, 4042);
            return false;
        }

        UInt32 index = uRand(count);
        Player* matchPlayer = vecPlayer[index];
        sendMatchPlayer(pl, matchPlayer);
        pl->setMatchPlayer(matchPlayer);
        return true;
    }

    bool RaceBattle::isStart()
    {
        if(_status == 4)
            return true;
        return false;
    }

	void RaceBattle::makeStarInfo(Stream& st, Player* pl, UInt8 level)
    {
        if(pl == NULL || level == 0 || level > 6)
        {
            st << static_cast<UInt32>(0);
            st << static_cast<UInt8>(0);
            return;
        }

        Int32 rank = 0;
        Int32 rankAdd = 0;
        RBSortType& starSort = _levelStarSort[level - 1];
        RBSortType::iterator it1;
        for(it1 = starSort.begin(); it1 != starSort.end(); ++it1)
        {
            ++rank;
            if(it1->player == pl)
            {
                break;
            }

            RBSortType::iterator it2 = it1;
            ++it2;
            if(it2 == starSort.end())
                break;
            else if(it2->player == pl)
            {
                rankAdd = 1;
                break;
            }

            ++it2;
            if(it2 == starSort.end())
                break;
            else if(it2->player == pl)
            {
                rankAdd = 2;
                break;
            }

            ++it2;
            if(it2 == starSort.end())
                break;
            else if(it2->player == pl)
            {
                rankAdd = 3;
                break;
            }
        }

        st << (rank + rankAdd);
        UInt32 offset = st.size();
        UInt8 count = 0;
        st << count;

        for(RBSortType::iterator it = it1; it != starSort.end(); ++it)
        {
            if(count >= 3)
                break;
            Player* player = it->player;
            st << player->getName();
            st << static_cast<UInt32>(rank + count);
            st << it->total;
            st << player->GetClass();
            st << static_cast<UInt8>(player->IsMale() ? 0 : 1);
            ++count;
        }
        st.data<UInt8>(offset) = count;
    }

    void RaceBattle::sendContinueWinSort(Player* player, UInt8 page)
    {
        if(player == NULL)
            return;

        UInt32 playerTotal = _contineWinSort.size();
        if(playerTotal > 50)
            playerTotal = 50;
        UInt32 pageTotal = playerTotal / PAGE_MAX;
        if(pageTotal * PAGE_MAX < playerTotal)
            ++pageTotal;
        if(page > pageTotal)
            return;

        Stream st(REP::RACE_BATTLE);
        UInt8 type = 3;
        st << type;
        st << page;
        st << pageTotal;

        UInt8 playerCnt;
        if(page == pageTotal)
            playerCnt = playerTotal - (page - 1) * PAGE_MAX;
        else
            playerCnt = PAGE_MAX;
        st << playerCnt;

        UInt8 count = 0;
        UInt8 aimBegin = (page - 1) * PAGE_MAX;
        RBSortType::iterator it = _contineWinSort.begin();
        for(; it != _contineWinSort.end(); it++)
        {
           if(count == aimBegin)
               break;
           ++count;
        }

        count = 0;
        for(; it != _contineWinSort.end(); it++)
        {
            if(count == playerCnt)
                break;
            Player* pl = it->player;
            st << pl->getName();
            st << pl->getId();
            st << pl->getContinueWinCnt();
            st << player->getChallengeStatus(pl) ;

            ++count;
        }

        st << Stream::eos;
        player->send(st);
    }

    void RaceBattle::sendBattleInfo(Player* pl)
    {
        if(!pl)
            return;
        Stream st(REP::RACE_BATTLE);
        UInt8 type = 4;
        st << type;
        pl->makeRBBattleInfo(st);
        st << Stream::eos;
        pl->send(st);
    }

    void RaceBattle::sendMatchPlayer(Player* pl, Player* matchPlayer)
    {
        if(!pl)
            return;
        Stream st(REP::RACE_BATTLE);
        UInt8 type = 5;
        st << type;

        UInt8  isPink;
        if(matchPlayer)
        {
            isPink = 1;
            st << isPink;
            st << matchPlayer->getName();
            st << matchPlayer->getCountry();
            st << matchPlayer->GetClass();
            st << matchPlayer->GetLev();
            st << static_cast<UInt8>(matchPlayer->getMainFighter()->getId());
            st << matchPlayer->getBattlePoint();
            st << matchPlayer->getRaceBattlePos();
            st << matchPlayer->getId();
        }
        else
        {
            isPink = 0;
            st << isPink;
        }

        st << Stream::eos;
        pl->send(st);
    }

    //return value: 0-invalid,1-win,2-lose
    UInt8 RaceBattle::attackPlayer(Player* pl, Player* matchPlayer)
    {
        if(!pl || !matchPlayer)
            return 0;

        UInt32 now = TimeUtil::Now();
        if(pl->getAttackCd() > now)
        {
            pl->sendMsgCode(0, 4045);
            return 0;
        }
        pl->setAttackCd(now + 20);

        if(pl->getExitCd() > now)
        {
            pl->sendMsgCode(0, 4040);
            return 0;
        }

        Battle::BattleSimulator bsim(Battle::BS_RACEBATTLE, pl, matchPlayer);
        pl->PutFighters( bsim, 0 );
        matchPlayer->PutFighters( bsim, 1 );

        GData::RandBattleData::stRandBattle* rb = GData::randBattleData.getRandBattleData(pl->getRaceBattlePos());
        if(rb)
            pl->setRBBuf(rb->id, rb->value);
        bsim.start();
        if(rb)
            pl->setRBBuf(0, 0);

        bool res = bsim.getWinner() == 1;
        UInt32 reptid = bsim.getId();

		Stream st(REP::ATTACK_NPC);
		st << static_cast<UInt8>(res ? 1 : 0) << static_cast<UInt8>(0) << bsim.getId()<<static_cast<UInt64>(0) << Stream::eos;
		pl->send(st);

        PlayerReport stReport;
        stReport.pl = matchPlayer;
        stReport.win = res ? 0 : 1;
        stReport.reportId = reptid;
        pl->insertPlayerRecord(stReport);
        UInt32 achievement;
        if(res == 1)
            achievement = 30;
        else
            achievement = 15;
        pl->getAchievement(achievement);
        pl->setTotalAchievement(pl->getTotalAchievement() + achievement);

        return res ? 1 : 2;
    }

    void RaceBattle::awardLevelRank()
    {
#if 0
        UInt8 rank = 0;
        for(UInt8 i = 6; i >= 1; i--)
        {
            RBSortType& levelSort = _levelStarSort[i - 1];
            for(RBSortType::iterator it = levelSort.begin(); it != levelSort.end(); ++it)
            {
                ++rank;
                if(rank > 50)
                    return;
                awardLevelRankOne(it->player, rank);
            }
        }
#endif
    }

    void RaceBattle::awardContinueWinRank()
    {
#if 0
        for(RBSortType::iterator it = _contineWinSort.begin(); it != _contineWinSort.end(); ++it)
        {
            if(it->total < 5)
                return;
            awardContinueWinRankOne(it->player, it->total);
        }
#endif
    }

    void RaceBattle::awardLevelRankOne(Player* pl, UInt8 rank)
    {
#if 0
        if(!pl)
            return;

        UInt8 type = 0;
        if(rank == 0)
            return;
        else if(rank <= 1)
            type = 0;
        else if(rank <= 2)
            type = 1;
        else if(rank <= 3)
            type = 2;
        else if(rank <= 9)
            type = 3;
        else if(rank <= 29)
            type = 4;
        else if(rank <= 50)
            type = 5;
        else
            return;

        static UInt32 achievement[] = {300, 200, 150, 120, 100, 50};
        pl->getAchievement(achievement[type]);
        SYSMSG(title, 5135);
        SYSMSGV(content, 5136, rank, achievement[type]);
        pl->GetMailBox()->newMail(NULL, 0x1, title, content, 0xFFFE0000);
#endif
    }

    void RaceBattle::awardContinueWinRankOne(Player* pl, UInt8 num)
    {
        if(!pl)
            return;

        UInt8 type;
        if(num == 90)
            type = 9;
        else if(num == 80)
            type = 8;
        else if(num == 70)
            type = 7;
        else if(num == 60)
            type = 6;
        else if(num == 50)
            type = 5;
        else if(num == 40)
            type = 4;
        else if(num == 30)
            type = 3;
        else if(num == 20)
            type = 2;
        else if(num == 10)
            type = 1;
        else if(num == 5)
            type = 0;
        else
            return;

        UInt8 broad = type;
        if(broad > 3)
            broad = 3;
        if(broad <= 3)
            SYSMSG_BROADCASTV(6015 + broad, pl->getCountry(), pl->getPName());
#if 0
        static UInt32 achievement[] = {5, 15, 30, 50, 75, 105, 140, 180, 225, 275};
        pl->getAchievement(achievement[type]);
        SYSMSG(title, 5137);
        SYSMSGV(content, 5138, num, achievement[type]);
        pl->GetMailBox()->newMail(NULL, 0x1, title, content, 0xFFFE0000);
#endif
    }

    void RaceBattle::eraseLevelStarSort(Player* pl, UInt8 level)
    {
        if(!pl)
            return;
        if(level == 0 || level > 6)
            return;

        RBSortType& levelSort = _levelStarSort[level - 1];
        for(RBSortType::iterator it = levelSort.begin(); it != levelSort.end(); ++it)
        {
            if(it->player == pl)
            {
                levelSort.erase(it);
                break;
            }
        }
    }

    void RaceBattle::insertLevelStarSort(Player* pl, UInt8 level)
    {
        if(!pl)
            return;
        if(level == 0 || level > 6)
            return;

        TSort tsort;
        tsort.player = pl;
        tsort.total = pl->getStarTotal();
        tsort.time = TimeUtil::Now();
        _levelStarSort[level - 1].insert(tsort);
    }

    void RaceBattle::exitRB(Player* pl)
    {
        if(!pl)
            return;
        pl->setExitCd(TimeUtil::Now() + 20);
        eraseContinueWinSort(pl);
        pl->setContinueWinCnt(0);
    }

    void RaceBattle::attackLevelPlayer(Player* pl, UInt64 defenderId)
    {
        if(!pl)
            return;
        Player* defender = globalPlayers[defenderId];
        if(!defender)
            return;
        if(defender!= pl->getMatchPlayer())
            return;
        UInt8 pos = pl->getRaceBattlePos();
        UInt8 level = pos / 10;
        UInt8 offset = pos % 10;
        if(level == 0 || level > 5)
            return;
        if(offset == 0)
            return;
        if(offset > gPerLeveCnt[level - 1])
            return;
        if(pl->getStarCnt(offset - 1) >= 6)
        {
            pl->sendMsgCode(0, 4043);
            return;
        }

        UInt8 res = attackPlayer(pl, defender);
        if(res == 0)
            return;

        UInt8 starAdd;
        UInt8 isDouble;
        if(pl->getContinueLoseCnt() >= 5 * level)
            isDouble = 1;
        else
            isDouble = 0;
        if(res == 1)
        {
            starAdd = 2;
            if(isDouble)
                starAdd *= 2;
            eraseContinueWinSort(pl);
            pl->setContinueWinCnt(pl->getContinueWinCnt() + 1);
            awardContinueWinRankOne(pl, pl->getContinueWinCnt());
            insertContinueWinSort(pl);
            pl->setContinueLoseCnt(0);
            pl->setTotalWinCnt(pl->getTotalWinCnt() + 1);
        }
        else
        {
            starAdd = 1;
            if(isDouble)
                starAdd *= 2;
            eraseContinueWinSort(pl);
            broadCancelContinueWin(pl, defender);
            pl->setContinueWinCnt(0);
            pl->setContinueLoseCnt(pl->getContinueLoseCnt() + 1);
            pl->setTotalLoseCnt(pl->getTotalLoseCnt() + 1);
        }

        if(pl->getContinueLoseCnt() >= 5 * level)
            starAdd *= 2;
        UInt16 starCnt = pl->getStarCnt(offset - 1) + starAdd;
        if(starCnt < 6)
            pl->setStarCnt(offset - 1, starCnt);
        else
        {
            pl->setStarCnt(offset - 1, 6);
            UInt8 canContinueCnt = pl->getCanContinueCnt();
            pl->setCanContinueCnt(++canContinueCnt);
        }
        UInt16 starTotal = pl->getStarTotal() + starAdd;
        pl->setStarTotal(starTotal);
        eraseLevelStarSort(pl, level);
        GData::RandBattleData::stRandBattle* rb = GData::randBattleData.getRandBattleData(pl->getRaceBattlePos());
        if(rb && starTotal >= rb->next * 2)
        {
            level = level + 1;
            for(UInt8 i = 0; i < gPerLeveCnt[level - 1]; i++)
                pl->setStarCnt(i, 0);
            UInt8 pos = level * 10;
            pl->setRaceBattlePos(pos);
            pl->setContinueLoseCnt(0);
            if(level == 6 && starTotal > rb->next * 2)
                pl->setStarTotal(rb->next * 2);
            enterPos(pl, 0, true);
        }
        else
        {
            insertLevelStarSort(pl, level);
        }
        eraseContinueWinSort(pl);
        insertContinueWinSort(pl);
        sendContinueWinSort(pl, pl->getContinueWinPage());
        sendOwnerInfo(pl);
        sendBattleInfo(pl);
        pl->setMatchPlayer(NULL);
    }

    void RaceBattle::attackContinueWinPlayer(Player* pl, UInt64 defenderId)
    {
        if(!pl)
            return;
        UInt8 pos = pl->getRaceBattlePos();
        UInt8 level = pos / 10;
        UInt8 offset = pos % 10;
        if(level == 0 || level > 6)
            return;
        if(offset == 0 && level != 6)
            return;
        if(offset > gPerLeveCnt[level - 1])
            return;
        UInt8 continueCnt = pl->getCanContinueCnt();
        if(continueCnt == 0)
        {
            pl->sendMsgCode(0, 4041);
            return;
        }
        Player* defender = globalPlayers[defenderId];
        if(!defender)
            return;
        if(defender == pl)
            return;

        RBSortType::iterator it;
        UInt32 continueRank = 0;
        for(it = _contineWinSort.begin(); it != _contineWinSort.end(); ++it)
        {
            ++continueRank;
            if(it->player == defender)
                break;
        }
        if(it == _contineWinSort.end())
            return;
        if(pl->getChallengeStatus(defender) > 0)
            return;
        UInt8 page = pl->getContinueWinPage();
        UInt8 pageStart = page;
        if(pageStart > 1)
            pageStart -= 2;
        else if(pageStart > 0)
            pageStart -= 1;
        UInt8 pageEnd = page + 1;
        if(!(continueRank > pageStart * PAGE_MAX && continueRank < pageEnd * PAGE_MAX))
        {
            pl->sendMsgCode(0, 4046);
            sendContinueWinSort(pl, pl->getContinueWinPage());
            return;
        }

        UInt8 res = attackPlayer(pl, defender);
        if(res == 0)
            return;

        if(level == 6)
            return;

        UInt8 starAdd = 1;
        if(res == 1)
        {
            starAdd = 2;
            UInt8 contineWinCnt = defender->getContinueWinCnt();
            if(contineWinCnt < 3)
            {
            }
            else if(contineWinCnt < 10)
                starAdd += 1;
            else if(contineWinCnt < 20)
                starAdd += 2;
            else if(contineWinCnt < 30)
                starAdd += 4;
            else
                starAdd += 6;
        }

        UInt16 starCnt = pl->getStarCnt(offset - 1) + starAdd;
        if(starCnt < 6)
            pl->setStarCnt(offset - 1, starCnt);
        else
            pl->setStarCnt(offset - 1, 6);

        UInt16 starTotal = pl->getStarTotal() + starAdd;
        pl->setStarTotal(starTotal);
        eraseLevelStarSort(pl, level);
        GData::RandBattleData::stRandBattle* rb = GData::randBattleData.getRandBattleData(pl->getRaceBattlePos());
        if(rb && starTotal >= rb->next * 2)
        {
            level = level + 1;
            for(UInt8 i = 0; i < gPerLeveCnt[level - 1]; i++)
                pl->setStarCnt(i, 0);
            UInt8 pos = level * 10;
            pl->setRaceBattlePos(pos);
            pl->setContinueLoseCnt(0);
            if(level == 6 && starTotal > rb->next * 2)
                pl->setStarTotal(rb->next * 2);
            enterPos(pl, 0, true);
        }
        else
        {
            insertLevelStarSort(pl, level);
            eraseContinueWinSort(pl);
            insertContinueWinSort(pl);
            sendContinueWinSort(pl, pl->getContinueWinPage());
        }
        pl->setCanContinueCnt(--continueCnt);
        pl->insertChallengePlayer(defender);
        sendOwnerInfo(pl);
        sendBattleInfo(pl);
    }

    void RaceBattle::insertContinueWinSort(Player* pl)
    {
        if(!pl)
            return;
        if(pl->getContinueWinCnt() < 3)
            return;

        TSort tsort;
        tsort.player = pl;
        tsort.total = pl->getContinueWinCnt();
        tsort.time = TimeUtil::Now();
        _contineWinSort.insert(tsort);
    }

    void RaceBattle::eraseContinueWinSort(Player* pl)
    {
        if(!pl)
            return;
        if(pl->getContinueWinCnt() < 3)
            return;

        for(RBSortType::iterator it = _contineWinSort.begin(); it != _contineWinSort.end(); ++it)
        {
            if(it->player == pl)
            {
                _contineWinSort.erase(it);
                break;
            }
        }
    }

    void RaceBattle::sendOwnerInfo(Player* pl)
    {
        if(!pl)
            return;
        UInt8 pos = pl->getRaceBattlePos();
        UInt8 level = pos / 10;
        if(level == 0 || level > 6)
            return;
        GData::RandBattleData::stRandBattle* rb = GData::randBattleData.getRandBattleData(pos);
        if(!rb)
            return;

        Stream st(REP::RACE_BATTLE);
        UInt8 type = 2;
        st << type;
        st << pl->getRaceBattlePos();
        st << pl->getStarTotal();
        st << gPerLeveCnt[level - 1];
        for(UInt8 i = 0; i < gPerLeveCnt[level - 1]; i++)
            st << pl->getStarCnt(i);
        st << pl->getCanContinueCnt();
        st << pl->getContinueWinCnt();
        st << rb->next;
        makeStarInfo(st, pl, level);
        st << pl->getAwardLevel();
        UInt8 isDouble;
        if(pl->getContinueLoseCnt() >= 5 * level)
            isDouble = 1;
        else
            isDouble = 0;
        st << isDouble;
        st << Stream::eos;
        pl->send(st);
    }

    void RaceBattle::broadCancelContinueWin(Player* pl, Player* player2)
    {
        if(!pl || !player2)
            return;

        UInt8 contineWinCnt = pl->getContinueWinCnt();
        UInt8 type = 0;
        if(contineWinCnt >= 20)
            type = 2;
        else if(contineWinCnt >= 10)
            type = 1;
        else if(contineWinCnt >= 5)
            type = 0;
        else
            return;

        SYSMSG_BROADCASTV(6021 + type, player2->getCountry(), player2->getPName(), pl->getCountry(), pl->getPName(), contineWinCnt);
    }

    void RaceBattle::pageContinueWin(Player* pl, UInt8 flag)
    {
        if(!pl)
            return;
        if(flag > 1)
            return;

        UInt8 page = pl->getContinueWinPage();
        if(flag == 0)
        {
            UInt32 playerTotal = _contineWinSort.size();
            if(playerTotal > 50)
                playerTotal = 50;
            UInt32 pageTotal = playerTotal / PAGE_MAX;
            if(pageTotal * PAGE_MAX < playerTotal)
                ++pageTotal;
            if(page >= pageTotal)
                return;
            ++page;
        }
        else
        {
            if(page <= 1)
                return;
            --page;
        }
        pl->setContinueWinPage(page);
        sendContinueWinSort(pl, page);
    }
}


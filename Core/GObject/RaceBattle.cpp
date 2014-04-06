#include "Config.h"
#include "Common/Platform.h"
#include "Common/TimeUtil.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "GData/XingchenData.h"

#include "RaceBattle.h"

#define RACEBATTLE_STARTTIME 20*3600
#define RACEBATTLE_ENDTIME RACEBATTLE_STARTTIME+1800

namespace GObject
{
    RaceBattle* gRaceBattle = NULL;
    static UInt8 gPerLeveCnt[] = {2, 3, 4, 5, 7};
    RBSortType RaceBattle::_levelStarSort[5];
    RBSortType RaceBattle::_contineWinSort;

    RaceBattle::RaceBattle() : _status(0)
    {
    }

    void RaceBattleBroadcast(UInt8 type, UInt32 lefttime)
    {
        Stream st(REP::RACE_BATTLE);
        st << type;
        st << lefttime;
        st << Stream::eos;
        NETWORK()->Broadcast(st);
    }

    void RaceBattle::RaceBattleCheck(UInt32 time)
    {
        UInt32 curTime = TimeUtil::SharpDay(0, time);

        if(curTime < RACEBATTLE_STARTTIME - 600)
            _status = 0;
        else if(curTime < RACEBATTLE_STARTTIME)
        {
            if(_status == 1)
                return;
            _status = 1;
            RaceBattleBroadcast(0, RACEBATTLE_STARTTIME - curTime);
        }
        else if(curTime <= RACEBATTLE_ENDTIME)
        {
            if(_status == 2)
                return;
            _status = 2;
            RaceBattleBroadcast(1, RACEBATTLE_ENDTIME - curTime);
        }
        else
        {
            if(_status == 2)
                RaceBattleBroadcast(1, 0);
            _status = 0;
        }
    }

    void RaceBattle::enterPos(Player* pl, UInt8 pos)
    {
        if(pl->getRaceBattlePos() == pos)
            return;
        pl->setRaceBattlePos(pos);
        if(pl->GetVar(VAR_RCAE_BATTLE_SIGN) == 0)
            pl->SetVar(VAR_RCAE_BATTLE_SIGN, 1);

        GData::RandBattleData::stRandBattle* rb = GData::randBattleData.getRandBattleData(pos);
        if(!rb)
            return;
        UInt8 level = pos / 10;
        if(level == 0)
            return;
        if(level > sizeof(gPerLeveCnt) / sizeof(gPerLeveCnt[0]))
            return;
        UInt8 offset = pos % 10;
        if(offset > gPerLeveCnt[level - 1])
            return;
        Stream st(REP::RACE_BATTLE);
        UInt8 type = 2;
        st << type;
        st << pl->getRaceBattlePos();
        st << gPerLeveCnt[level - 1];
        for(UInt8 i = 0; i < gPerLeveCnt[level - 1]; i++)
            st << pl->getStarCnt(i);
        st << pl->getCanChallengeCnt();
        st << pl->getContinueWinCnt();
        st << rb->next;
        makeStarInfo(st, pl, level);
        st << pl->getAwardLevel();
        st << Stream::eos;
    }

    void RaceBattle::autoBattle(Player* pl)
    {
    }

    void RaceBattle::cancelBattle(Player* pl)
    {
    }

    void RaceBattle::freshContinueWinRank(Player* pl)
    {
    }

    void RaceBattle::getAward(Player* pl)
    {
    }

    void RaceBattle::readBattleReport(Player* pl, UInt32 reportId)
    {
    }

    void RaceBattle::requestMatch(Player* pl)
    {
    }

    bool RaceBattle::isStart()
    {
        if(_status == 2)
            return true;
        return false;
    }

	void RaceBattle::makeStarInfo(Stream& st, Player* pl, UInt8 level)
    {
        if(pl == NULL || level == 0 || level > 5)
        {
            st << static_cast<UInt32>(0);
            st << static_cast<UInt8>(0);
            return;
        }

        Int32 rank;
        RBSortType::reverse_iterator rstart;
        RBSortType& starSort = _levelStarSort[level - 1];
        UInt8 totalCnt = 0;
        for(UInt8 i = 0; i < gPerLeveCnt[level - 1]; i++)
            totalCnt += pl->getStarCnt(i);
        if(totalCnt == 0)
        {
            rank = starSort.size() + 1;
            rstart = starSort.rbegin();
        }
        else
        {
            RBSortType::reverse_iterator it = starSort.rbegin();
            rank = 0;
            for(; it != starSort.rend(); ++it)
            {
                ++rank;
                if(it->player == pl)
                    break;
            }
            if(it != starSort.rend())
                rstart = it;
            else
            {
                rank = starSort.size() + 1;
                rstart = starSort.rbegin();
            }
        }

        st << rank;
        UInt32 offset = st.size();
        st.data<UInt8>(offset) = 0;
        UInt8 count = 0;
        for(RBSortType::reverse_iterator it = rstart; it != starSort.rend(); ++it)
        {
            ++count;
            if(count > 3)
                break;
            st << it->player->getName();
            st << static_cast<UInt32>(rank - count);
            st << it->total;
        }
        st.data<UInt8>(offset) = count; 
    }

    void RaceBattle::sendContinueWinSort(Player* player, UInt8 page)
    {
        #define PAGE_MAX 5
        if(player == NULL || page == 0)
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

    void RaceBattle::matchPlayer(Player* pl, Player* matchPlayer)
    {
        if(!pl || !matchPlayer)
            return;
        Stream st(REP::RACE_BATTLE);
        UInt8 type = 4;
        st << type;

        st << pl->getName();
        st << pl->getCountry();
        st << pl->GetClass();
        st << pl->GetLev();
        st << pl->getBattlePoint();
        st << pl->getRaceBattlePos();

        st << matchPlayer->getName();
        st << matchPlayer->getCountry();
        st << matchPlayer->GetClass();
        st << matchPlayer->GetLev();
        st << matchPlayer->getBattlePoint();
        st << matchPlayer->getRaceBattlePos();

        st << Stream::eos;
        pl->send(st);
    }
}


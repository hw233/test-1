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
            rank = starSort.size();
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
                rstart = starSort.rbegin();
        }

        st << rank;
        UInt32 offset = st.size();
        st.data<UInt8>(offset) = 0;
        UInt8 count = 0;
        Int32 i = rank;
        for(RBSortType::reverse_iterator it = rstart; it != starSort.rend() && i > 0 && i > rank - 3; ++it, --i)
        {
            ++count;
            st << it->player->getName();
            st << i;
            st << it->total;
        }
        st.data<UInt8>(offset) = count; 
    }
}


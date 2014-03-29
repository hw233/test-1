#include "Config.h"
#include "Common/Platform.h"
#include "Common/TimeUtil.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "Server/WorldServer.h"

#include "RaceBattle.h"

#define RACEBATTLE_STARTTIME 20*3600
#define RACEBATTLE_ENDTIME RACEBATTLE_STARTTIME+1800

namespace GObject
{
    RaceBattle* gRaceBattle = NULL;

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

}


#include "Config.h"
#include "Common/TimeUtil.h"
#include "GObject/Leaderboard.h"
#include "DB/DBExecutor.h"
#include "DB/DBConnectionMgr.h"
#include "GData/GDataDBExecHelper.h"
#include "GObject/Leaderboard.h"
#include "Player.h"
#include "DaysRank.h"

namespace GObject
{

DaysRank::DaysRank()
{
}

void DaysRank::Init()
{
    LoadFromDB();
}
void DaysRank::process(bool toDB)
{
    FastMutex::ScopedLock lk(_opMutex);
    std::unordered_map<UInt64, Player*>& pm = GObject::globalPlayers.getMap();
    std::unordered_map<UInt64, Player*>::iterator iter;
    for (iter = pm.begin(); iter != pm.end(); ++iter)
    {
        Player* pl = iter->second;
        if (NULL != pl)
        {
            std::map<UInt64, PlayerRankInfo>::iterator iter = m_playerDaysRank.find(pl->getId());
            if (iter == m_playerDaysRank.end())
            {
                PlayerRankInfo info;
                m_playerDaysRank.insert(make_pair(pl->getId(), info));
                iter = m_playerDaysRank.find(pl->getId());
            }
            PlayerRankInfo* pInfo = &(iter->second);

            int rank1 = GObject::leaderboard.getMyRank(pl, 1);
            int rank2 = GObject::leaderboard.getMyRank(pl, 0);
            int rank3 = pl->GetVar(VAR_USETAEL_CNT) + pl->GetVar(VAR_USECOUPON_CNT) * 100;
            int rank4 = pl->GetVar(VAR_GETACHIEVEMENT_CNT) + pl->GetVar(VAR_GETPRESTIGE_CNT);
            int rank5 = GObject::leaderboard.getMyRank(pl, 4);
            if (!toDB)
            {
                pInfo->update(rank1, 0);
                pInfo->update(rank2, 1);
                pInfo->update(rank3, 2);
                pInfo->update(rank4, 3);
                pInfo->update(rank5, 4);
                continue;
            }
            pInfo->push_back(rank1, 0);
            pInfo->push_back(rank2, 1);
            pInfo->push_back(rank3, 2);
            pInfo->push_back(rank4, 3);
            pInfo->push_back(rank5, 4);
            std::string s1 = pInfo->toString(0);
            std::string s2 = pInfo->toString(1);
            std::string s3 = pInfo->toString(2);
            std::string s4 = pInfo->toString(3);
            std::string s5 = pInfo->toString(4);
            if(toDB)
                DB1().PushUpdateData("REPLACE INTO `days_rank` VALUES(%"I64_FMT"u,'%s','%s','%s','%s','%s')", 
                    pl->getId(),s1.c_str(),s2.c_str(),s3.c_str(),s4.c_str(),s5.c_str());

        }
    }
}

void DaysRank::LoadFromDB()
{
    FastMutex::ScopedLock lk(_opMutex);

    std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
    if (execu.get() == NULL || !execu->isConnected()) return;
    GData::DBDaysRank dbexp;
    if(execu->Prepare("select player_id,athletics,level,money,pk,battle from days_rank", dbexp) != DB::DB_OK)
        return;
    while (execu->Next() == DB::DB_OK)
    {
        PlayerRankInfo info;
        info.parseFromDB(dbexp.athletics, 0);
        info.parseFromDB(dbexp.level, 1);
        info.parseFromDB(dbexp.money, 2);
        info.parseFromDB(dbexp.pk, 3);
        info.parseFromDB(dbexp.battle, 4);

        m_playerDaysRank.insert(make_pair(dbexp.player_id, info));
    }
}

PlayerRankInfo* DaysRank::getDaysRank(UInt64 playerId)
{
    FastMutex::ScopedLock lk(_opMutex);
    std::map<UInt64, PlayerRankInfo>::iterator iter = m_playerDaysRank.find(playerId);
    if (iter != m_playerDaysRank.end())
    {
        return &(iter->second);
    }
    return NULL;
}

void DaysRank::updateDaysValue(daysValueRankMsg * msg)
{
    if(!msg)
        return;

    UInt8 type = msg->type;
    PlayerRankInfo* pInfo = getDaysRank(msg->playerId);
    if(pInfo)
    {
        if (type < DAYS_RANK_TYPE)
        {
            pInfo->rank[type][DAYS_RANK_COUNT-1] = msg->value;
        }
    }
}

}



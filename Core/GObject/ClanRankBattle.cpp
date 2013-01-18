#include "ClanRankBattle.h"
#include <algorithm>
#include "Clan.h"
#include "Server/SysMsg.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Player.h"
#include "Package.h"
#include "Country.h"
#include "PracticePlace.h"
#include "Script/GameActionLua.h"
#include "HeroMemo.h"
#include "ShuoShuo.h"
#include <sstream>

namespace GObject
{

    //战斗准备时间
    const static UInt32 PREPARE_TIME = 5 * 60;
    //实际战斗时间
    const static UInt32 BATTLE_TIME = 7 * 60;
    //完整战斗时间
    const static UInt32 FULL_BATTLE_TIME = PREPARE_TIME + BATTLE_TIME;
    //每天战斗次数
    const static UInt32 BATTLE_NUM_PER_DAY = 5;


    //每个战场的分数
    const static UInt32 BATTLE_FIELD_SCORE[] = {3, 5, 6, 7, 8, 10};
    //胜利方额外积分
    const static UInt32 WINNER_EXTRA_SCORE = 20;


    //帮会排名战报名开始时间
    const static UInt32 RANK_BATTLE_SIGNUP_BEGINTIME = 20 * 60 * 60;
    //帮会排名战报名持续时间
    const static UInt32 RANK_BATTLE_SIGNUP_TIME = 40 * 60;

    //每次战斗时间
    const static UInt32 RANK_BATTLE_FIGHT_TIME = 40;



    bool ClanRankBattleInfo::HasPlayer(Player* player)
    {
        for(PlayerMap::iterator iter = players.begin();
                iter != players.end(); ++iter)
        {
            PlayerVec& list = iter->second;
            for(PlayerVec::iterator iter2 = list.begin();
                    iter2 != list.end(); ++iter2)
            {
                if(*iter2 == player) return true;
            }
        }
        return false;
    }


#define REMOVE_PLAYER_INLIST(List, Player) \
    do{ \
        for(PlayerVec::iterator iter = List.begin(); \
            iter != List.end(); ++iter){ \
            if(*iter == Player) { \
                List.erase(iter); \
                return; \
            } \
        } \
    }while(false);


    void ClanRankBattleInfo::RemovePlayer(Player* player, UInt32 field)
    {
        if(field >= RANK_BATTLE_FIELD_NUM)
        {
            for(PlayerMap::iterator iter = players.begin();
                iter != players.end(); ++iter)
            {
                PlayerVec& list = iter->second;
                REMOVE_PLAYER_INLIST(list, player)
            }
        }
        else
        {
            PlayerVec& list = players[field];
            REMOVE_PLAYER_INLIST(list, player)
        }
    }

    void ClanRankBattleInfo::AddPlayer(Player* player, UInt32 field)
    {
        if(field < RANK_BATTLE_FIELD_NUM && player != NULL)
            players[field].push_back(player);
    }

    UInt32 ClanRankBattleInfo::GetMinPlayerField()
    {
        UInt32 min = 0;
        UInt32 minNum = 0xFFFFFFFF;
        for(UInt32 i = 1; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            UInt32 size =  players[i].size();
            if(size < minNum)
            {
                minNum = size;
                min = i;
            }
        }
        return min;
    }

    UInt32 ClanRankBattleInfo::GetPlayerNum()
    {
        UInt32 num = 0;
        for(PlayerMap::iterator iter = players.begin();
                iter != players.end(); ++iter)
        {
            num += iter->second.size();
        }
        return num;
    }

    void ClanRankBattleInfo::Broadcast(Stream& stream, bool bAll)
    {
        for(PlayerMap::iterator iter1 = players.begin();
                iter1 != players.end(); ++iter1)
        {
            PlayerVec& list = iter1->second;
            for(PlayerVec::iterator iter2 = list.begin();
                    iter2 != list.end(); ++iter2)
            {
                if(bAll || (*iter2)->getLocation() == RANK_BATTLE_LOCATION)
                    (*iter2)->send(stream);
            }
        }
    }



    ClanRankBattleField::ClanRankBattleField()
    {
        m_Id = 0;
        m_pBattle = NULL;

        m_bEnd = false;
        m_Winner = 0;

        m_LastWinClan = 0;

        m_Clan1 = m_Clan2 = 0;
        m_Now = TimeUtil::Now();
        m_StartTime = m_ReportTime = m_Now;
        m_Round = 0;
    }

    ClanRankBattleField::~ClanRankBattleField()
    {
        m_WaitPlayers.clear();
    }


    void ClanRankBattleField::Init(UInt32 id, ClanRankBattle* battle)
    {
        m_Id = id;
        m_pBattle = battle;
    }

    void ClanRankBattleField::Start(UInt32 clan1, const PlayerVec& team1, UInt32 clan2, const PlayerVec& team2, UInt32 now)
    {
        m_Clan1 = clan1;
        m_Clan2 = clan2;

        //设置状态
        for(PlayerVec::const_iterator iter = team1.begin();
                iter != team1.end(); ++iter){
            (*iter)->SetClanBattleStatus(PLAYER_WAIT);
            (*iter)->addFlag(Player::ClanRankBattle);
            (*iter)->regenAll(true);
            (*iter)->SetVar(VAR_DAILY_CLANBATTLE, 1);
            (*iter)->SetVar(VAR_WEEKLY_CLANBATTLE, 1);

            GameAction()->doStrong((*iter), SthClanWar, 1, 0);
            (*iter)->OnHeroMemo(MC_ATHLETICS, MD_LEGEND, 0, 0);
            (*iter)->OnShuoShuo(SS_CLANRANK);
            (*iter)->setContinuousRFAward(2);

            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA((*iter), created));
            if(thisDay <= endDay)
            {
                UInt32 targetVal = (*iter)->GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_CLAN_BATTLE))
                {
                    targetVal |=TARGET_CLAN_BATTLE;
                    (*iter)->AddVar(VAR_CTS_TARGET_COUNT, 1);
                    (*iter)->SetVar(VAR_CLAWARD2, targetVal);
                    (*iter)->sendNewRC7DayTarget();
                    (*iter)->newRC7DayUdpLog(1152, 5);
                }
            }

        }
        for(PlayerVec::const_iterator iter = team2.begin();
                iter != team2.end(); ++iter){
            (*iter)->SetClanBattleStatus(PLAYER_WAIT);
            (*iter)->addFlag(Player::ClanRankBattle);
            (*iter)->regenAll(true);
            (*iter)->SetVar(VAR_DAILY_CLANBATTLE, 1);
            (*iter)->SetVar(VAR_WEEKLY_CLANBATTLE, 1);

            GameAction()->doStrong((*iter), SthClanWar, 1, 0);
            (*iter)->OnHeroMemo(MC_ATHLETICS, MD_LEGEND, 0, 0);
            (*iter)->OnShuoShuo(SS_CLANRANK);
            (*iter)->setContinuousRFAward(2);

            UInt32 thisDay = TimeUtil::SharpDay();
            UInt32 endDay = TimeUtil::SharpDay(6, PLAYER_DATA((*iter), created));
            if(thisDay <= endDay)
            {
                UInt32 targetVal = (*iter)->GetVar(VAR_CLAWARD2);
                if (!(targetVal & TARGET_CLAN_BATTLE))
                {
                    targetVal |=TARGET_CLAN_BATTLE;
                    (*iter)->AddVar(VAR_CTS_TARGET_COUNT, 1);
                    (*iter)->SetVar(VAR_CLAWARD2, targetVal);
                    (*iter)->sendNewRC7DayTarget();
                }
            }


        }

        m_WaitPlayers.clear();
        m_WaitPlayers.insert(std::make_pair(clan1, team1));
        m_WaitPlayers.insert(std::make_pair(clan2, team2));
        m_DeadPlayers.clear();

        m_StartTime = now;
        m_bEnd = false;
        m_Winner = 0;
        m_Round = 0;

    }


    void ClanRankBattleField::Process(UInt32 now)
    {
        m_Now = now;

        if(IsEnd()) return;

        //开始下一轮
        if(now >= m_StartTime + m_Round * RANK_BATTLE_FIGHT_TIME)
        {
            ProcessRound(now, true);
        }
    }

    void ClanRankBattleField::ProcessRound(UInt32 now, bool bNotify)
    {

        m_StatusChanged.clear();
        ResetPlayerStatus(m_Clan1);
        ResetPlayerStatus(m_Clan2);

        m_ReportTime = now;
        m_Reports.clear();

        PlayerVec& waitPlayers1 = m_WaitPlayers[m_Clan1];
        PlayerVec& waitPlayers2 = m_WaitPlayers[m_Clan2];

        UInt32 fightNum1 = waitPlayers1.size();
        UInt32 fightNum2 = waitPlayers2.size();

        UInt32 fightNum = std::min(fightNum1, fightNum2);
        if(fightNum > 3) fightNum = 3;

        if(fightNum > 0)
        {
            PlayerVec fightPlayers1;
            PlayerVec fightPlayers2;

            //从队伍头取出对阵玩家
            for(UInt32 i = 0; i < fightNum; ++i)
            {
                fightPlayers1.push_back(*waitPlayers1.begin());
                waitPlayers1.erase(waitPlayers1.begin());

                fightPlayers2.push_back(*waitPlayers2.begin());
                waitPlayers2.erase(waitPlayers2.begin());
            }


            for(UInt32 i = 0; i < fightNum; ++i)
            {
                Player* player1 = fightPlayers1[i];
                Player* player2 = fightPlayers2[i];

                player1->SetClanBattleStatus(PLAYER_BATTLE);
                player2->SetClanBattleStatus(PLAYER_BATTLE);

                m_StatusChanged[player1->getId()] = PLAYER_BATTLE;
                m_StatusChanged[player2->getId()] = PLAYER_BATTLE;

                ClanRankBattleReport report;
                int turns = 0;
                player1->setHiAttrFlag(true);
                player2->setHiAttrFlag(true);
                bool fightRes = player1->challenge(player2, &report.reportid, &turns, false, 0, true, Battle::BS_COPY5, 0x03);
                player1->setHiAttrFlag(false);
                player2->setHiAttrFlag(false);
                //player1->setBuffData(PLAYER_BUFF_ATTACKING, now + 2 * turns);
                //player2->setBuffData(PLAYER_BUFF_ATTACKING, now + 2 * turns);

                report.player1 = player1->getName();
                report.player2 = player2->getName();

                if(fightRes) //玩家1胜
                {
                    player1->IncClanBattleWinTimes();   //增加本场连胜次数
                    player1->AddClanBattleScore(player1->GetClanBattleWinTimes());

                    player2->SetClanBattleWinTimes(0);

                    waitPlayers1.push_back(player1); //放回等待队列
                    m_DeadPlayers[m_Clan2].push_back(player2); //死亡
                    player2->regenAll(true);

                    report.result = 1;
                    m_LastWinClan = m_Clan1;

                    player1->OnHeroMemo(MC_ATHLETICS, MD_LEGEND, 0, 1);
                }
                else //玩家2胜
                {
                    player1->SetClanBattleWinTimes(0);

                    player2->IncClanBattleWinTimes();
                    player2->AddClanBattleScore(player2->GetClanBattleWinTimes());

                    m_DeadPlayers[m_Clan1].push_back(player1);
                    waitPlayers2.push_back(player2);
                    player1->regenAll(true);

                    report.result = 2;
                    m_LastWinClan = m_Clan2;

                    player2->OnHeroMemo(MC_ATHLETICS, MD_LEGEND, 0, 1);
                }

                m_Reports.push_back(report);
            }

            if(bNotify)
            {
                Stream battleStream(REP::CLAN_RANKBATTLE_REP);
                battleStream << UInt8(10);
                GetReportsData(battleStream);
                m_pBattle->BroadcastBattle(battleStream);
            }
        }
        else
        {
            m_bEnd = true;
            if(fightNum1 > 0) m_Winner = m_Clan1;
            else if(fightNum2 > 0) m_Winner = m_Clan2;
            else m_Winner = m_LastWinClan;
        }

        //刷新状态变化
        if(bNotify && !m_StatusChanged.empty())
        {
            Stream stream(REP::CLAN_RANKBATTLE_REP);
            stream << UInt8(6);
            stream << UInt8(m_StatusChanged.size());
            for(std::map<UInt64,UInt32>::iterator iter = m_StatusChanged.begin();
                iter != m_StatusChanged.end(); ++iter)
            {
                stream << UInt64(iter->first);
                stream << UInt8(iter->second);
            }
            stream << Stream::eos;
            m_pBattle->BroadcastBattle(stream);
        }

        ++m_Round;

    }

    bool ClanRankBattleField::OnPlayerLeave(Player* player)
    {
        Clan* clan = player->getClan();
        if(clan == NULL) return false;

        UInt32 id = clan->getId();
        if(id != m_Clan1 && id != m_Clan2) return false; //不是两个帮会之一

        PlayerVec& players1 = m_WaitPlayers[id];
        for(PlayerVec::iterator iter = players1.begin(); iter != players1.end(); ++iter)
        {
            if(*iter == player)
            {
                ResetPlayerData(player);
                players1.erase(iter);
                NotifyPlayerLeave(player);
                return true;
            }
        }

        PlayerVec& players2 = m_DeadPlayers[id];
        for(PlayerVec::iterator iter = players2.begin(); iter != players2.end(); ++iter)
        {
            if(*iter == player)
            {
                ResetPlayerData(player);
                players2.erase(iter);
                NotifyPlayerLeave(player);
                return true;
            }
        }

        return false;
    }

    UInt32 ClanRankBattleField::GetPlayerNum(UInt32 clan)
    {
        if(clan != m_Clan1 && clan != m_Clan2) return 0;

        return m_WaitPlayers[clan].size() + m_DeadPlayers[clan].size();
    }

    void ClanRankBattleField::End(UInt32 extScore1, UInt32 extScore2)
    {
        PlayerVec& players1 = m_WaitPlayers[m_Clan1];
        PlayerVec& players2 = m_WaitPlayers[m_Clan2];

        players1.insert(players1.end(), m_DeadPlayers[m_Clan1].begin(), m_DeadPlayers[m_Clan1].end());
        players2.insert(players2.end(), m_DeadPlayers[m_Clan2].begin(), m_DeadPlayers[m_Clan2].end());
        m_DeadPlayers[m_Clan1].clear();
        m_DeadPlayers[m_Clan2].clear();

        for(PlayerVec::iterator iter = players1.begin();
                iter != players1.end(); ++iter){
            Player* player = *iter;
            player->AddClanBattleScore(extScore1);

            Clan* clan = player->getClan();
            if(clan != NULL) //增加帮贡献
            {
                UInt32 proffer = player->GetClanBattleScore() * 5;
                clan->addMemberProffer(player, proffer);
                player->AddVar(VAR_CLANBATTLE_HONOUR, proffer);

                if (GetWinner() == clan->getId())
                    player->OnHeroMemo(MC_ATHLETICS, MD_LEGEND, 0, 2);
            }
        }

        for(PlayerVec::iterator iter = players2.begin();
                iter != players2.end(); ++iter){
            Player* player = *iter;
            player->AddClanBattleScore(extScore2);

            Clan* clan = player->getClan();
            if(clan != NULL) //增加帮贡献
            {
                UInt32 proffer = player->GetClanBattleScore() * 5;
                clan->addMemberProffer(player, proffer);
                player->AddVar(VAR_CLANBATTLE_HONOUR, proffer);

                if (GetWinner() == clan->getId())
                    player->OnHeroMemo(MC_ATHLETICS, MD_LEGEND, 0, 2);
            }
        }
    }

    void ClanRankBattleField::Reset()
    {
        PlayerVec& players1 = m_WaitPlayers[m_Clan1];
        PlayerVec& players2 = m_WaitPlayers[m_Clan2];

        for(PlayerVec::iterator iter = players1.begin();
                iter != players1.end(); ++iter){
            ResetPlayerData(*iter);
        }

        for(PlayerVec::iterator iter = players2.begin();
                iter != players2.end(); ++iter){
            ResetPlayerData(*iter);
        }
    }

    void ClanRankBattleField::FillPlayers(Stream& stream, UInt32 clan)
    {
        if(clan != m_Clan1 && clan != m_Clan2) return;

        PlayerVec& players1 = m_WaitPlayers[clan];
        for(PlayerVec::iterator iter = players1.begin();
                iter != players1.end(); ++iter)
        {
            FillPlayer(stream, *iter);
        }

        PlayerVec& players2 = m_DeadPlayers[clan];
        for(PlayerVec::iterator iter = players2.begin();
                iter != players2.end(); ++iter)
        {
            FillPlayer(stream, *iter);
        }
    }

    void ClanRankBattleField::FillPlayer(Stream& stream, Player* player)
    {
        stream << player->getId();
        stream << player->getName();
        stream << UInt8(player->GetLev());
        stream << UInt8(m_Id);
        stream << UInt8(player->GetClanBattleStatus());
    }

    void ClanRankBattleField::NotifyPlayerLeave(Player* player)
    {
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(8) << player->getId() << Stream::eos;
        m_pBattle->BroadcastBattle(stream);
    }

    void ClanRankBattleField::ResetPlayerStatus(UInt32 clan)
    {
        PlayerVec& players1 = m_WaitPlayers[clan];
        PlayerVec& players2 = m_DeadPlayers[clan];

        for(PlayerVec::iterator iter = players1.begin();
                iter != players1.end(); )
        {
            Player* player = *iter;

            if(player->GetClanBattleStatus() != PLAYER_BATTLE){
                ++iter;
                continue;
            }

            if(player->GetClanBattleWinTimes() == 8){ //连胜8场了
                player->SetClanBattleStatus(PLAYER_WIN);
                m_StatusChanged[player->getId()] = PLAYER_WIN;
                players2.push_back(player);

                iter = players1.erase(iter);
            }
            else{
                player->SetClanBattleStatus(PLAYER_WAIT);
                m_StatusChanged[player->getId()] = PLAYER_WAIT;

                ++iter;
            }
        }

        for(PlayerVec::iterator iter = players2.begin();
                iter != players2.end(); ++iter)
        {
            Player* player = *iter;

            if(player->GetClanBattleStatus() != PLAYER_BATTLE) continue;

            player->SetClanBattleStatus(PLAYER_DEAD);
            m_StatusChanged[player->getId()] = PLAYER_DEAD;
        }
    }

    void ClanRankBattleField::ResetPlayerData(Player* player)
    {
        player->clearHIAttr();
        player->regenAll(true);
        player->SetClanBattleStatus(PLAYER_WAIT);
        player->SetClanBattleScore(0);
        player->SetClanBattleWinTimes(0);
        player->ClearClanBattleSkillFlag();
        player->delFlag(Player::ClanRankBattle);
    }

    void ClanRankBattleField::GetReportsData(Stream& stream)
    {
        UInt16 leftTime = m_ReportTime + 40 > m_Now ? m_ReportTime + 40 - m_Now : 0;

        stream << UInt8(m_Id) << UInt8(leftTime) << UInt8(m_Reports.size());
        for(ReportVec::iterator iter = m_Reports.begin(); iter != m_Reports.end(); ++iter)
        {
            stream << UInt8(iter->result);
            stream << iter->player1 << iter->player2 << iter->reportid;
        }
        stream << Stream::eos;
    }

    void ClanRankBattleField::Broadcast(UInt32 clan, Stream& stream)
    {
        PlayerVec& players1 = m_WaitPlayers[clan];
        PlayerVec& players2 = m_DeadPlayers[clan];

        for(PlayerVec::iterator iter = players1.begin();
                iter != players1.end(); ++iter)
        {
            (*iter)->send(stream);
        }

        for(PlayerVec::iterator iter = players2.begin();
                iter != players2.end(); ++iter)
        {
            (*iter)->send(stream);
        }
    }

    void ClanRankBattleField::Broadcast(Stream& stream)
    {
        Broadcast(m_Clan1, stream);
        Broadcast(m_Clan2, stream);
    }

    void ClanRankBattleField::SendBattleReport(Player* player)
    {
        if(player == NULL || !player->hasFlag(Player::ClanRankBattle)) return;

        Stream battleStream(REP::CLAN_RANKBATTLE_REP);
        battleStream << UInt8(10);
        GetReportsData(battleStream);
        player->send(battleStream);
    }



    ClanRankBattle::ClanRankBattle(ClanRankBattleInfo* clan1, ClanRankBattleInfo* clan2)
    {
        m_Clan1 = clan1;
        m_Clan2 = clan2;

        m_ClanScore1 = m_ClanScore2 = 0;
        if(clan2 == NULL) m_ClanScore1 = 20;
        if(clan1 == NULL) m_ClanScore2 = 20;

        m_State = STATE_PREPARE;
        m_StartTime = 0;
        m_Now = 0;
        m_bEnd = false;

        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            m_Fields[i].Init(i, this);
        }

        m_Winner = 0;
    }

    ClanRankBattle::~ClanRankBattle()
    {
    }

    void ClanRankBattle::Process(UInt32 now)
    {
        m_Now = now;

        if(m_State == STATE_PREPARE)  //准备状态
        {
            if(now >= m_StartTime + PREPARE_TIME) //开始战斗时间
            {
                for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
                {
                    PlayerVec battleList1;
                    UInt32 clan1 = 0;
                    if(m_Clan1 != NULL)
                    {
                        clan1 = m_Clan1->clan->getId();
                        PlayerVec& players1 = m_Clan1->players[i];
                        for(PlayerVec::iterator iter = players1.begin(); iter != players1.end(); ++iter)
                        {
                            if((*iter)->getLocation() == RANK_BATTLE_LOCATION
                                    && (*iter)->getThreadId() == WORKER_THREAD_NEUTRAL)  battleList1.push_back(*iter); //选取当前在国战点上的玩家战斗
                        }
                    }

                    PlayerVec battleList2;
                    UInt32 clan2 = 0;
                    if(m_Clan2 != NULL)
                    {
                        clan2 = m_Clan2->clan->getId();
                        PlayerVec& players2 = m_Clan2->players[i];
                        for(PlayerVec::iterator iter = players2.begin(); iter != players2.end(); ++iter)
                        {
                            if((*iter)->getLocation() == RANK_BATTLE_LOCATION
                                    && (*iter)->getThreadId() == WORKER_THREAD_NEUTRAL)  battleList2.push_back(*iter);
                        }
                    }

                    m_Fields[i].Start(clan1, battleList1, clan2, battleList2, now);
                }
                //切换到战斗状态
                m_State = STATE_BATTLE;
                BroadcastStatus(m_Clan1);
                BroadcastStatus(m_Clan2);
            }
        }
        else
        {
            UInt32 endNum = 0;
            for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
            {
                if(m_Fields[i].IsEnd()) ++endNum;
                else
                {
                    m_Fields[i].Process(now);
                    //更新积分
                    if(m_Clan1 != NULL && m_Clan2 != NULL && m_Fields[i].IsEnd())
                    {
                        UInt32 winner = m_Fields[i].GetWinner();
                        if(winner == 0) continue;

                        if(winner == m_Clan1->clan->getId()) m_ClanScore1 += BATTLE_FIELD_SCORE[i];
                        else if(winner == m_Clan2->clan->getId()) m_ClanScore2 += BATTLE_FIELD_SCORE[i];
                        BroadcastScores(i, winner);
                    }
                }
            }
            if(endNum == RANK_BATTLE_FIELD_NUM)
            {
                End(false);
            }
        }
    }


    void ClanRankBattle::Start(UInt32 now)
    {
        m_Now = now;
        m_StartTime = now;

        if(m_Clan2 == NULL) //只有m_Clan2可能为空
        {
            SysMsgItem *sysMsgItem = globalSysMsg[2233];
            if(sysMsgItem != NULL)
            {
                Stream stream;
                sysMsgItem->getvap(&stream, 40);
                m_Clan1->clan->broadcast(stream);
            }
            m_Clan1->battle = this;
        }
        else
        {
            Stream stream1;
            Stream stream2;
            SysMsgItem* msg = globalSysMsg[2212];
            if(msg != NULL)
            {
                if(cfg.merged)
                {
                    Player* founderMan = globalPlayers[m_Clan2->clan->getFounder()];
                    if(founderMan)
                        msg->getvap(&stream1, founderMan->getNameNoSuffix(m_Clan2->clan->getName()));
                }
                else
                    msg->getvap(&stream1, m_Clan2->clan->getName().c_str());
                m_Clan1->clan->broadcast(stream1);
                if(cfg.merged)
                {
                    Player* founderMan = globalPlayers[m_Clan1->clan->getFounder()];
                    if(founderMan)
                        msg->getvap(&stream2, founderMan->getNameNoSuffix(m_Clan1->clan->getName()));
                }
                else
                    msg->getvap(&stream2, m_Clan1->clan->getName().c_str());
                m_Clan2->clan->broadcast(stream2);
            }
            m_Clan1->battle = this;
            m_Clan2->battle = this;
        }

        BroadcastStatus(m_Clan1);
        BroadcastStatus(m_Clan2);
    }

    void ClanRankBattle::End(bool bTimeout)
    {
        if(m_State == STATE_PREPARE) return;
        if(IsEnd()) return;
        m_bEnd = true;

        //结算积分
        m_ClanScore1 = 0;
        m_ClanScore2 = 0;
        m_Winner = m_Clan1->clan->getId();

        if(m_Clan2 == NULL)
        {
            m_ClanScore1 = 20;
        }
        else
        {
            for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
            {
                int times = 100;
                while(!m_Fields[i].IsEnd() && times-- > 0)
                {
                    m_Fields[i].ProcessRound(m_Now, false);
                }

                m_Winner = m_Fields[i].GetWinner();

                if(m_Clan1 != NULL && m_Winner == m_Clan1->clan->getId())
                {
                    m_ClanScore1 += BATTLE_FIELD_SCORE[i];
                }
                else if(m_Clan2 != NULL && m_Winner == m_Clan2->clan->getId())
                {
                    m_ClanScore2 += BATTLE_FIELD_SCORE[i];
                }
            }
        }

        //个人额外积分
        UInt32 playerScore1 = m_ClanScore1 + 5;
        UInt32 playerScore2 = m_ClanScore2 + 5;

        if(m_ClanScore1 > m_ClanScore2)
        {
            m_ClanScore1 += WINNER_EXTRA_SCORE;
            playerScore1 += 15;
            if(m_Clan1 != NULL)
            {
                m_Winner = m_Clan1->clan->getId();
            }
        }
        else if(m_ClanScore2 > m_ClanScore1)
        {
            m_ClanScore2 += WINNER_EXTRA_SCORE;
            playerScore2 += 15;
            if(m_Clan2 != NULL)
            {
                m_Winner = m_Clan2->clan->getId();
            }
        }
        else
        {
            if(m_Clan1 != NULL && m_Winner == m_Clan1->clan->getId())
            {
                m_ClanScore1 += WINNER_EXTRA_SCORE;
                playerScore1 += 15;
            }
            else
            {
                m_ClanScore2 += WINNER_EXTRA_SCORE;
                playerScore2 += 15;
            }
        }

        if(m_Clan1 != NULL)
        {
            m_Clan1->clan->SetBattleScore(m_Clan1->clan->GetBattleScore() + m_ClanScore1);
            m_Clan1->clan->SetDailyBattleScore(m_Clan1->clan->GetDailyBattleScore() + m_ClanScore1);
        }
        if(m_Clan2 != NULL)
        {
            m_Clan2->clan->SetBattleScore(m_Clan2->clan->GetBattleScore() + m_ClanScore2);
            m_Clan2->clan->SetDailyBattleScore(m_Clan2->clan->GetDailyBattleScore() + m_ClanScore2);
        }


        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            m_Fields[i].End(playerScore1, playerScore2);
        }

        if(m_Clan1 != NULL) m_Clan1->clan->BroadcastBattleData(m_Now);
        if(m_Clan2 != NULL) m_Clan2->clan->BroadcastBattleData(m_Now);

        BroadcastScores(bTimeout ? UInt8(-2) : UInt8(-1), m_Winner, WINNER_EXTRA_SCORE);
    }

    void ClanRankBattle::Reset()
    {
        if(m_Clan1 != NULL) m_Clan1->battle = NULL;
        if(m_Clan2 != NULL) m_Clan2->battle = NULL;

        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i){
            m_Fields[i].Reset();
        }
    }

    void ClanRankBattle::OnPlayerLeave(Player* player)
    {
        player->clearHIAttr();
        player->ClearClanBattleSkillFlag();

        if(m_State == STATE_BATTLE)
        {

            for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
            {
                if(m_Fields[i].OnPlayerLeave(player))
                {
                    break;
                }
            }
        }
    }

    ClanRankBattleInfo* ClanRankBattle::GetClan(UInt32 id)
    {
        if(m_Clan1 != NULL && m_Clan1->clan->getId() == id) return m_Clan1;
        else if(m_Clan2 != NULL && m_Clan2->clan->getId() == id) return m_Clan2;
        return NULL;
    }

    ClanRankBattleInfo* ClanRankBattle::GetOtherClan(UInt32 id)
    {
        if(m_Clan1 != NULL && m_Clan1->clan->getId() == id) return m_Clan2;
        else if(m_Clan2 != NULL && m_Clan2->clan->getId() == id) return m_Clan1;
        return NULL;
    }

    void ClanRankBattle::UseSkill(Player* player, ClanBattleSkill* skill)
    {
        if(m_State != STATE_PREPARE) return; //准备阶段才能用

        if(player->CheckClanBattleSkillFlag(skill->id)) return; //已使用

        if(player->getGold() < skill->price)
        {
            player->sendMsgCode(0, 1104);
            return;
        }

        ConsumeInfo info(ClanRankBattleSkill, 0, 0);
        player->useGold(skill->price, &info);

        player->addHIAttr(skill->attrs);
        player->SetClanBattleSkillFlag(skill->id);

        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(1);
        stream << player->getId();
        stream << UInt16(player->GetClanBattleSkillFlag());
        stream << Stream::eos;
        Broadcast(player->getClan()->getId(), stream);

        //Stream stream(REP::CLAN_RANKBATTLE_REP);
        //stream << UInt8(4);
        //stream << UInt8(skill->id);
        //stream << Stream::eos;
        //player->send(stream);
    }

    void ClanRankBattle::SendBattleStatus(Player* player)
    {
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(0);
        stream << UInt8(m_State);
        UInt32 time = 0;
        if(m_State == STATE_PREPARE)
        {
            if(m_StartTime + PREPARE_TIME > m_Now) time = m_StartTime + PREPARE_TIME - m_Now;
        }
        else
        {
            if(m_StartTime + FULL_BATTLE_TIME > m_Now) time = m_StartTime + FULL_BATTLE_TIME - m_Now;
        }
        stream << time;

        if(m_Clan1 != NULL && player->getClan() == m_Clan1->clan)
        {
            stream << ((m_Clan2 != NULL) ? m_Clan2->clan->getName() : "");
        }
        else if(m_Clan2 != NULL && player->getClan() == m_Clan2->clan)
        {
            stream << ((m_Clan1 != NULL) ? m_Clan1->clan->getName() : "");
        }
        else return;

        stream << Stream::eos;
        player->send(stream);
    }

    void ClanRankBattle::BroadcastStatus(ClanRankBattleInfo* clan)
    {
        if(clan == NULL) return;

        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(0);
        stream << UInt8(m_State);
        UInt32 time = 0;
        if(m_State == STATE_PREPARE)
        {
            if(m_StartTime + PREPARE_TIME > m_Now) time = m_StartTime + PREPARE_TIME - m_Now;
        }
        else
        {
            if(m_StartTime + FULL_BATTLE_TIME > m_Now) time = m_StartTime + FULL_BATTLE_TIME - m_Now;
        }
        stream << time;

        if(clan == m_Clan1)
        {
            stream << ((m_Clan2 != NULL) ? m_Clan2->clan->getName() : "");
        }
        else if(clan == m_Clan2)
        {
            stream << ((m_Clan1 != NULL) ? m_Clan1->clan->getName() : "");
        }
        else return;

        stream << Stream::eos;
        clan->Broadcast(stream);
    }

    void ClanRankBattle::BroadcastScores(UInt8 fightId, UInt32 winner, UInt32 extScore)
    {
        if(m_Clan1 != NULL)
        {
            Stream stream1(REP::CLAN_RANKBATTLE_REP);
            stream1 << UInt8(11) << UInt8(m_Clan1->clan->getId() == winner ? 1 : 2)
                << fightId << m_ClanScore1 << m_ClanScore2 << extScore << Stream::eos;
            BroadcastBattle(m_Clan1->clan->getId(), stream1);
        }

        if(m_Clan2 != NULL)
        {
            Stream stream2(REP::CLAN_RANKBATTLE_REP);
            stream2 << UInt8(11) << UInt8(m_Clan2->clan->getId() == winner ? 1 : 2)
                << fightId << m_ClanScore2 << m_ClanScore1 << extScore << Stream::eos;
            BroadcastBattle(m_Clan2->clan->getId(), stream2);
        }
    }

    void ClanRankBattle::Broadcast(UInt32 id, Stream& stream, bool bAll)
    {
        if(m_Clan1 != NULL && m_Clan1->clan->getId() == id) m_Clan1->Broadcast(stream, bAll);
        if(m_Clan2 != NULL && m_Clan2->clan->getId() == id) m_Clan2->Broadcast(stream, bAll);
    }

    void ClanRankBattle::Broadcast(Stream& stream, bool bAll)
    {
        if(m_Clan1 != NULL) m_Clan1->Broadcast(stream, bAll);
        if(m_Clan2 != NULL) m_Clan2->Broadcast(stream, bAll);
    }

    void ClanRankBattle::BroadcastBattle(UInt32 clan, Stream& stream)
    {
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            m_Fields[i].Broadcast(clan, stream);
        }
    }

    void ClanRankBattle::BroadcastBattle(Stream &stream)
    {

        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            m_Fields[i].Broadcast(stream);
        }
    }

    void ClanRankBattle::SendBattleInfo(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(5);

        ClanRankBattleInfo* myClan = NULL;
        ClanRankBattleInfo* otherClan = NULL;

        if(m_Clan1 != NULL && player->getClan() == m_Clan1->clan)
        {
            myClan = m_Clan1;
            otherClan = m_Clan2;
            if(m_Winner != 0) stream << UInt8(m_Winner == m_Clan1->clan->getId()?1:2);
            else stream << UInt8(0);
            stream << m_ClanScore1 << m_ClanScore2;
            stream << UInt32(WINNER_EXTRA_SCORE);
        }
        else if(m_Clan2 != NULL && player->getClan() == m_Clan2->clan)
        {
            myClan = m_Clan2;
            otherClan = m_Clan1;
            if(m_Winner != 0) stream << UInt8(m_Winner == m_Clan2->clan->getId()?1:2);
            else stream << UInt8(0);
            stream << m_ClanScore2 << m_ClanScore1;
            stream << UInt32(WINNER_EXTRA_SCORE);
        }
        else
        {
            return;
        }

        UInt32 num1 = 0;
        UInt32 num2 = 0;
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            if(m_Fields[i].GetWinner() == 0) stream << UInt8(0);
            else if(m_Fields[i].GetWinner() == myClan->clan->getId()) stream << UInt8(1);
            else stream << UInt8(2);

            if(myClan != NULL) num1 += m_Fields[i].GetPlayerNum(myClan->clan->getId());
            if(otherClan != NULL) num2 += m_Fields[i].GetPlayerNum(otherClan->clan->getId());
        }

        stream << UInt8(num1);
        if(myClan != NULL)
        {
            for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
            {
                m_Fields[i].FillPlayers(stream, myClan->clan->getId());
            }
        }

        stream << UInt8(num2);
        if(otherClan != NULL)
        {
            for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
            {
                m_Fields[i].FillPlayers(stream, otherClan->clan->getId());
            }
        }

        stream << Stream::eos;
        player->send(stream);
    }


    void ClanRankBattle::SendBattleReport(Player* player, UInt8 fightId)
    {
        if(m_State != STATE_BATTLE) return;
        if(fightId >= RANK_BATTLE_FIELD_NUM) return;

        m_Fields[fightId].SendBattleReport(player);
    }





    ClanRankBattleMgr::ClanRankBattleMgr()
    {
        m_State = STATE_INIT;
        m_StartTime = 0;
        m_SignupCountDown = 0;
        m_BattleNo = 0;
        m_Now = 0;
        m_expTime = 0;
        m_singupNotified = false;
        m_battleNotified = false;
        m_Status = 0;

        memset(m_Skills, 0, sizeof(m_Skills));
    }

    ClanRankBattleMgr::~ClanRankBattleMgr()
    {
        UnInit();
    }

    void ClanRankBattleMgr::Init()
    {
        m_Now = TimeUtil::Now();
        m_StartTime = TimeUtil::SharpDayT(0, m_Now) + RANK_BATTLE_SIGNUP_BEGINTIME;
        //m_StartTime = m_Now - m_Now % ( 2 * 60 * 60 );

        if(m_Now < m_StartTime) //还没开始
        {
            m_State = STATE_INIT;
        }
        else if(m_Now < m_StartTime + RANK_BATTLE_SIGNUP_TIME) //报名时间
        {
            m_State = STATE_SIGNUP;
            //倒计时
            m_SignupCountDown = (m_StartTime + RANK_BATTLE_SIGNUP_TIME - m_Now) / 60;
        }
        else if(m_Now < m_StartTime + RANK_BATTLE_SIGNUP_TIME + FULL_BATTLE_TIME * BATTLE_NUM_PER_DAY) //战斗时间
        {
            //获取有资格的帮会
            GetCanBattleClans(false);

            m_State = STATE_BATTLE;
            m_BattleNo = (m_Now - m_StartTime - RANK_BATTLE_SIGNUP_TIME) / FULL_BATTLE_TIME + 1;
        }
        else
        {
            m_State = STATE_INIT;
            m_StartTime = m_StartTime + 24 * 60 * 60; //第二天战斗开始时间
            //m_StartTime = m_StartTime + 2 * 60 * 60;
        }

        //获取帮派排行
        m_ClanRanking.clear();
        class GetClansVisitor : public Visitor<Clan>
        {
        public:
            GetClansVisitor(ClanVec& list)
                :m_ClanList(list){}

            bool operator()(Clan* clan)
            {
                if(clan->GetBattleScore() != 0){
                    m_ClanList.push_back(clan);
                }
                return true;
            }

        private:
            ClanVec& m_ClanList;
        };
        GetClansVisitor visitor(m_ClanRanking);
        globalClans.enumerate(visitor);
        SortClans(false);

        //获取技能buff配置
        Table buffs = GameAction()->GetClanBattleBuffs();
        size_t buffNum = buffs.size();
        for(UInt32 i = 1; i <= buffNum && i <= RANK_BATTLE_SKILL_NUM; ++i)
        {
            Table buff = buffs.get<Table>(i);
            if(buff.size() < 13) continue;

            m_Skills[i].id = i;
            m_Skills[i].price = buff.get<UInt32>(1);
            m_Skills[i].attrs.attackP = m_Skills[i].attrs.magatkP = buff.get<float>(2) / 100;
            m_Skills[i].attrs.defendP = m_Skills[i].attrs.magdefP = buff.get<float>(3) / 100;
            m_Skills[i].attrs.hpP = buff.get<float>(4) / 100;
            m_Skills[i].attrs.actionP = buff.get<float>(5) / 100;
            m_Skills[i].attrs.hitrateP = buff.get<float>(6) / 100;
            m_Skills[i].attrs.evadeP = buff.get<float>(7) / 100;
            m_Skills[i].attrs.criticalP = buff.get<float>(8) / 100;
            m_Skills[i].attrs.criticaldmg = buff.get<float>(9) / 100;
            m_Skills[i].attrs.pierceP = buff.get<float>(10) / 100;
            m_Skills[i].attrs.counterP = buff.get<float>(11) / 100;
            m_Skills[i].attrs.magresP = buff.get<float>(12) / 100;
            m_Skills[i].attrs.aura = buff.get<float>(13);
        }

        //获取每日个人奖励
        Table dailyRewards = GameAction()->GetClanBattleDailyRewards();
        for(int i = 1; i <= dailyRewards.size(); ++i)
        {
            Table dailyReward = dailyRewards.get<Table>(i);
            if(dailyReward.size() < 2) continue;

            //个人积分上限
            UInt32 topLimit = dailyReward.get<UInt32>(1);

            RewardVec& rewards = m_DailyRewards[topLimit];
            for(int i = 2; i <= dailyReward.size(); ++i)
            {
                Table item = dailyReward.get<Table>(i);
                if(item.size() < 2) continue;

                ClanBattleReward reward;
                reward.id = item.get<UInt16>(1);
                reward.count = item.get<UInt32>(2);
                rewards.push_back(reward);
            }
        }

        //帮会周排名奖励
        Table weeklySortRewards = GameAction()->GetClanBattleWeekSortRewards();
        for(int i = 1; i <= weeklySortRewards.size(); ++i)
        {
            Table weeklySortReward = weeklySortRewards.get<Table>(i);
            if(weeklySortReward.size() < 2) continue;

            //周排名上限
            UInt32 topLimit = weeklySortReward.get<UInt32>(1);

            RewardVec& rewards = m_WeeklyClanSortRewards[topLimit];
            for(int i = 2; i <= weeklySortReward.size(); ++i)
            {
                Table item = weeklySortReward.get<Table>(i);
                if(item.size() < 2) continue;

                ClanBattleReward reward;
                reward.id = item.get<UInt16>(1);
                reward.count = item.get<UInt32>(2);
                rewards.push_back(reward);
            }
        }

        //帮会周积分奖励
        Table weeklyScoreRewards = GameAction()->GetClanBattleWeekScoreRewards();
        for(int i = 1; i <= weeklyScoreRewards.size(); ++i)
        {
            Table weeklyScoreReward = weeklyScoreRewards.get<Table>(i);
            if(weeklyScoreReward.size() < 3) continue;

            //周积分上限
            UInt32 topLimit = weeklyScoreReward.get<UInt32>(1);

            Table playerRewards = weeklyScoreReward.get<Table>(2);
            RewardVec& playerRewardsVec = m_WeeklyRewards[topLimit];
            for(int i = 1; i <= playerRewards.size(); ++i)
            {
                Table item = playerRewards.get<Table>(i);
                if(item.size() < 2) continue;

                ClanBattleReward reward;
                reward.id = item.get<UInt16>(1);
                reward.count = item.get<UInt32>(2);
                playerRewardsVec.push_back(reward);
            }

            Table clanRewards = weeklyScoreReward.get<Table>(3);
            RewardVec& clanRewardsVec = m_WeeklyClanRewards[topLimit];
            for(int i = 1; i <= clanRewards.size(); ++i)
            {
                Table item = clanRewards.get<Table>(i);
                if(item.size() < 2) continue;

                ClanBattleReward reward;
                reward.id = item.get<UInt16>(1);
                reward.count = item.get<UInt32>(2);
                clanRewardsVec.push_back(reward);
            }
        }
    }

    void ClanRankBattleMgr::UnInit()
    {
        for(BattleVec::iterator iter = m_Battles.begin();
                iter != m_Battles.end(); ++iter)
        {
            delete (*iter);
        }
        m_Battles.clear();
        m_Clans.clear();
    }

    bool ClanRankBattleMgr::IsInBattle(Clan* clan)
    {
        if(clan == NULL || m_State == STATE_INIT) return false;

        return true;
    }

    void ClanRankBattleMgr::Process(UInt32 now)
    {
        UInt32 oldTime = m_Now;
        m_Now = now;

        switch(m_State)
        {
            case STATE_INIT:
                {
                    ProcessInit(oldTime);
                }
                break;
            case STATE_SIGNUP:
                {
                    if (!m_singupNotified)
                    {
                        setStatus(3);
                        m_singupNotified = true;
                    }
                    ProcessSignup();
                    CheckAddExp();
                }
                break;
            case STATE_BATTLE:
                {
                    if (!m_battleNotified)
                    {
                        setStatus(1);
                        m_battleNotified = true;
                    }
                    ProcessBattle();
                    CheckAddExp();
                }
                break;
        }
    }

    void ClanRankBattleMgr::Signup(Player* player, UInt32 fieldId)
    {
        if(m_State == STATE_INIT) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        if (player->GetVar(VAR_CLAN_LEAVE_TIME) + 24 * 60 * 60 > TimeUtil::Now())
        {
            player->sendMsgCode(1, 1331);
            return;
        }

        if(m_State == STATE_SIGNUP)
        {
            if(fieldId >= RANK_BATTLE_FIELD_NUM) return;

            if(!clan->SignupRankBattle(player, fieldId ,m_Now)) return;
        }
        else if(m_State == STATE_BATTLE)
        {
            //帮派没资格
            ClanRankBattleInfo *info = GetClanInfo(clan->getId());
            if(info == NULL) return;

            UInt32 fieldId = info->GetMinPlayerField();
            if(!clan->SignupRankBattle(player, fieldId, m_Now)) return;
            info->AddPlayer(player, fieldId);

            //更新列表
            Stream stream(REP::CLAN_RANKBATTLE_REP);
            stream << UInt8(7);
            stream << UInt8(0);
            stream << player->getId();
            stream << player->getName();
            stream << UInt8(player->GetLev());
            stream << UInt8(fieldId);
            stream << UInt8(player->getLocation() == RANK_BATTLE_LOCATION ? 1 : 0);
            stream << Stream::eos;
            info->Broadcast(stream);
        }

        clan->BroadcastBattleData(m_Now);

        Stream stream(REP::CLAN_RANKBATTLE_REPINIT);
        stream << UInt8(1) << UInt8(0) << Stream::eos;
        player->clanUdpLog(1088);
        player->send(stream);
    }

    void ClanRankBattleMgr::Signout(Player* player)
    {
        if(m_State == STATE_INIT) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        if(m_State == STATE_SIGNUP)
        {
            if(!clan->SignoutRankBattle(player, m_Now)) return;
        }
        else if(m_State == STATE_BATTLE)
        {
            //帮派没资格
            ClanRankBattleInfo* info = GetClanInfo(clan->getId());
            if(info == NULL) return;

            if(!clan->SignoutRankBattle(player, m_Now)) return;

            //更新列表
            Stream stream(REP::CLAN_RANKBATTLE_REP);
            stream << UInt8(7);
            stream << UInt8(1);
            stream << player->getId();
            stream << Stream::eos;
            info->Broadcast(stream);

            info->RemovePlayer(player);
            if(info->battle != NULL) info->battle->OnPlayerLeave(player);
        }

        clan->BroadcastBattleData(m_Now);

        Stream stream(REP::CLAN_RANKBATTLE_REPINIT);
        stream << UInt8(2) << UInt8(0) << Stream::eos;
        player->send(stream);
    }



    void ClanRankBattleMgr::PlayerEnter(Player* player)
    {
        if(player->getLocation() != RANK_BATTLE_LOCATION) return;
        if(!m_InplacePlayers.insert(player).second) return;

        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return; //帮会没参加

        if(!info->HasPlayer(player)) return;

        if (player->GetVar(VAR_CLAN_LEAVE_TIME) + 24 * 60 * 60 > TimeUtil::Now())
        {
            player->sendMsgCode(1, 1331);
            return;
        }

        UInt32 field = clan->GetRankBattleField(player, m_Now);
        if(field >= RANK_BATTLE_FIELD_NUM) return;

        //通知状态变化
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(9);
        stream << player->getId();
        stream << UInt8(1);
        stream << Stream::eos;
        info->Broadcast(stream);
    }

    void ClanRankBattleMgr::PlayerLeave(Player* player)
    {
        if(player->getLocation() != RANK_BATTLE_LOCATION) return;
        PlayerSet::iterator iter = m_InplacePlayers.find(player);
        if(iter == m_InplacePlayers.end()) return;
        m_InplacePlayers.erase(iter);

        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

        if(!info->HasPlayer(player)) return;

        if(info->battle != NULL) info->battle->OnPlayerLeave(player);

        //通知状态变化
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(9);
        stream << player->getId();
        stream << UInt8(0);
        stream << Stream::eos;
        info->Broadcast(stream);
    }



    class ClanPlayerLevelSorter
    {
    public:
        ClanPlayerLevelSorter(bool bLowerHead)
            :m_bLowerHead(bLowerHead){}

        bool operator()(Player* player1, Player* player2) const
        {
            if(m_bLowerHead) return player1->GetLev() < player2->GetLev();
            else return player1->GetLev() > player2->GetLev();
        }

    private:
        //是否从小到大排序
        bool m_bLowerHead;
    };

    void ClanRankBattleMgr::SortClanPlayers(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* pInfo = GetClanInfo(clan->getId());
        if(pInfo == NULL) return; //该帮没参加

        if(!pInfo->HasPlayer(player)) return; //没报名

        if(clan->getClanRank(player) < 2) return; //没权力

        pInfo->m_bLowerHead = !pInfo->m_bLowerHead;
        ClanPlayerLevelSorter sorter(pInfo->m_bLowerHead);

        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(3);
        stream << UInt8(pInfo->GetPlayerNum());
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            UInt32 pos = 0;
            PlayerVec& players = pInfo->players[i];
            std::sort(players.begin(), players.end(), sorter);

            for(PlayerVec::iterator iter = players.begin();
                    iter != players.end(); ++iter)
            {
                Player* member = *iter;
                stream << member->getId();
                stream << UInt8(i);
                stream << UInt8(++pos);
            }
        }
        stream << Stream::eos;
        pInfo->Broadcast(stream);
    }

    void ClanRankBattleMgr::AdjustPlayerPos(Player* player, Player* member, UInt32 field, UInt32 pos)
    {
        if(m_State != STATE_BATTLE || field >= RANK_BATTLE_FIELD_NUM) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* pInfo = GetClanInfo(clan->getId());
        if(pInfo == NULL) return; //该帮没有参加

        if(!pInfo->HasPlayer(player)) return; //没报名

        if(clan->getClanRank(player) < 2) return; //没权力


        PlayerVec& players = pInfo->players[field];
        if(pos > players.size()) pos = players.size();


        UInt32 oldField = clan->AdjustRankBattleField(member, field, m_Now);
        if(oldField >= RANK_BATTLE_FIELD_NUM)
        {
            if(oldField == UInt32(-3))
            {
                SYSMSG_SENDV(2234, player, Clan::RANK_BATTLE_FIELD_PLAYERNUM);
            }
            return;
        }

        pInfo->RemovePlayer(member, oldField);

        PlayerVec::iterator posIter = players.begin();
        while(pos > 0 && posIter != players.end())
        {
            ++posIter;
            --pos;
        }
        players.insert(posIter, member);

        //刷新出战顺序
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(3);
        UInt32 num = players.size();
        if(field != oldField) num += pInfo->players[oldField].size();

        stream << UInt8(num);

        UInt32 index = 0;
        for(PlayerVec::iterator iter = players.begin();
            iter != players.end(); ++iter)
        {
            Player* member = *iter;
            stream << member->getId();
            stream << UInt8(field);
            stream << UInt8(index++);
        }

        if(field != oldField)
        {
            index = 0;
            PlayerVec& players = pInfo->players[oldField];
            for(PlayerVec::iterator iter = players.begin();
                iter != players.end(); ++iter)
            {
                Player* member = *iter;
                stream << member->getId();
                stream << UInt8(oldField);
                stream << UInt8(index++);
            }
        }

        stream << Stream::eos;
        pInfo->Broadcast(stream);

        const static UInt32 msg[RANK_BATTLE_FIELD_NUM] = {2216, 2217, 2218, 2219, 2220, 2221};
        if(oldField != field && player != member)
        {
            SYSMSG_SEND(msg[field], member);
        }
    }


    void ClanRankBattleMgr::UseSkill(Player* player, UInt32 id)
    {
        if(m_State != STATE_BATTLE) return;

        if(id == 0 || id > RANK_BATTLE_SKILL_NUM) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

        if(!info->HasPlayer(player)) return;

        if(info->battle != NULL)
        {
           info->battle->UseSkill(player, &m_Skills[id]);
        }
    }

    void ClanRankBattleMgr::SendInitStatus(Player* player)
    {
        Clan* clan = player->getClan();
        if(clan == NULL) return;

        Stream stream(REP::CLAN_RANKBATTLE_REPINIT);
        stream << UInt8(0);
        stream << UInt32(clan->GetBattleScore());
        stream << UInt32(player->GetVar(VAR_CLANBATTLE_HONOUR));
        stream << UInt16(clan->GetLastBattleRanking());
        stream << UInt16(clan->GetBattleRanking());
        stream << UInt8(m_State);
        UInt32 time = 0;
        switch(m_State)
        {
            case STATE_INIT:
                if(m_StartTime > m_Now)
                {
                    time = m_StartTime - m_Now;
                }
                break;
            case STATE_SIGNUP:
                if(m_StartTime + RANK_BATTLE_SIGNUP_TIME > m_Now)
                {
                    time = m_StartTime + RANK_BATTLE_SIGNUP_TIME - m_Now;
                }
                break;
            case STATE_BATTLE:
                if(m_StartTime + RANK_BATTLE_SIGNUP_TIME + BATTLE_NUM_PER_DAY * FULL_BATTLE_TIME > m_Now)
                {
                    time = m_StartTime + RANK_BATTLE_SIGNUP_TIME + BATTLE_NUM_PER_DAY * FULL_BATTLE_TIME - m_Now;
                }
                break;
        }
        stream << time;

        UInt32 field = clan->GetRankBattleField(player, m_Now);
        stream << UInt8(field < RANK_BATTLE_FIELD_NUM ? 1:0);
        stream << UInt8(clan->GetSignupRankBattleNum(m_Now));
        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        stream << UInt8(info != NULL ? 1 : 0);
        std::string str = "";
        if(info != NULL && info->battle != NULL)
        {
            ClanRankBattleInfo* otherInfo = info->battle->GetOtherClan(clan->getId());
            if(otherInfo != NULL) str = otherInfo->clan->getName();
        }
        stream << str << Stream::eos;
        player->send(stream);
    }

    void ClanRankBattleMgr::SendBattleStatus(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

        if(info->battle != NULL)
        {
            info->battle->SendBattleStatus(player);
        }
    }

    void ClanRankBattleMgr::SendSkillList(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

        if(!info->HasPlayer(player)) return;

        Stream st(REP::CLAN_RANKBATTLE_REP);
        st << UInt8(1);
        st << player->getId();
        st << UInt16(player->GetClanBattleSkillFlag());
        st << Stream::eos;
        player->send(st);
    }

    void ClanRankBattleMgr::SendSortList(Player* player, UInt16 start, UInt8 count)
    {
        Clan* clan = player->getClan();
        if(clan == NULL) return;

        Stream st(REP::CLAN_RANKBATTLE_SORTLIST);

        UInt16 total = UInt16(m_ClanRanking.size());

        if(start >= total)
        {
            start = total;
            count = 0;
        }
        else if(start + count > total)
        {
            count = total - start;
        }

        st << total << start << count;
        for(UInt32 index = start; index < start + count; ++index)
        {
            Clan* clan = m_ClanRanking[index];
            st << UInt32(clan->getId());
            st << clan->getName();
            Player* leader = clan->getLeader();
            if(leader == NULL) st << "";
            else st << leader->getName();
            st << UInt8(clan->getCountry());
            st << UInt32(clan->GetBattleScore());
        }
        st << Stream::eos;
        player->send(st);
    }


    void ClanRankBattleMgr::SyncState()
    {
        UInt32 time = 0;
        switch(m_State)
        {
            case STATE_INIT:
                if(m_StartTime > m_Now)
                {
                    time = m_StartTime - m_Now;
                }
                break;
            case STATE_SIGNUP:
                if(m_StartTime + RANK_BATTLE_SIGNUP_TIME > m_Now)
                {
                    time = m_StartTime + RANK_BATTLE_SIGNUP_TIME - m_Now;
                }
                break;
            case STATE_BATTLE:
                if(m_StartTime + RANK_BATTLE_SIGNUP_TIME + BATTLE_NUM_PER_DAY * FULL_BATTLE_TIME > m_Now)
                {
                    time = m_StartTime + RANK_BATTLE_SIGNUP_TIME + BATTLE_NUM_PER_DAY * FULL_BATTLE_TIME - m_Now;
                }
                break;
        }

        class NotifyClanVisitor : public Visitor<Clan>
        {
        public:
            NotifyClanVisitor(ClanRankBattleMgr* mgr, UInt8 state, UInt32 time)
                :m_pMgr(mgr),m_State(state),m_Time(time){}

            bool operator()(Clan* clan)
            {
                //将帮会战状态变化同步给所有人
                Stream stream(REP::CLAN_RANKBATTLE_REPINIT);
                stream << UInt8(5);
                stream << UInt8(m_State);
                stream << m_Time;
                UInt8 isClanIn = 0;
                if(m_State == STATE_BATTLE)
                {
                    isClanIn = (m_pMgr->GetClanInfo(clan->getId()) != NULL?1:0);
                }
                stream << isClanIn;
                stream << Stream::eos;
                clan->broadcast(stream);
                return true;
            }
        private:
            ClanRankBattleMgr* m_pMgr;
            UInt8 m_State;
            UInt32 m_Time;
        };

        NotifyClanVisitor visitor(this, m_State, time);
        globalClans.enumerate(visitor);
    }

    void ClanRankBattleMgr::SendPlayerList(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

        if(!info->HasPlayer(player)) return;

        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(2);
        stream << UInt8(info->GetPlayerNum());
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            PlayerVec& players = info->players[i];
            for(PlayerVec::iterator iter = players.begin();
                    iter != players.end(); ++iter)
            {
                Player* member = *iter;
                stream << member->getId();
                stream << member->getName();
                stream << UInt8(member->GetLev());
                stream << UInt8(i);
                stream << UInt8(member->getLocation() == RANK_BATTLE_LOCATION ? 1 : 0);
                stream << UInt16(member->GetClanBattleSkillFlag());
            }
        }
        stream << Stream::eos;
        player->send(stream);
    }

    void ClanRankBattleMgr::SendBattleInfo(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

        if(!info->HasPlayer(player)) return;

        if(info->battle != NULL)
        {
            info->battle->SendBattleInfo(player);
        }
    }

    void ClanRankBattleMgr::SendBattleReport(Player* player, UInt8 fightId)
    {
        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

        if(!info->HasPlayer(player)) return;

        if(info->battle != NULL)
        {
            info->battle->SendBattleReport(player, fightId);
        }
    }

    void ClanRankBattleMgr::GiveDailyRewards()
    {
        class GiveRewardsVisitor : public Visitor<ClanMember>
        {
        public:
            GiveRewardsVisitor(const std::string& title, const std::string& content, UInt32 score, MailItemsInfo* info)
                :m_Title(title), m_Content(content), m_Score(score), m_pInfo(info){}

            bool operator()(ClanMember* member)
            {
                if(member->player->GetVar(VAR_DAILY_CLANBATTLE) != 0)
                {
                    member->player->GetMailBox()->newMail(NULL, 0x41, m_Title, m_Content, m_Score, true, m_pInfo);
                    member->player->SetVar(VAR_DAILY_CLANBATTLE, 0);
                }
                return true;
            }

        private:
            std::string m_Title;
            std::string m_Content;
            UInt32 m_Score;
            MailItemsInfo* m_pInfo;
        };

        SYSMSG(title, 2236);
        SysMsgItem* msg = globalSysMsg[2237];
        for(ClanMap::iterator iter = m_Clans.begin(); iter != m_Clans.end(); ++iter)
        {
            Clan* clan = iter->second.clan;
            UInt32 score = clan->GetDailyBattleScore();


            for(RewardsMap::iterator iter = m_DailyRewards.begin();
                iter != m_DailyRewards.end(); ++iter)
            {
                if(score <= iter->first)
                {

                    RewardVec& rewards = iter->second;
                    MailItemsInfo info(&rewards[0], ClanBattleAward, rewards.size());

                    std::string content;
                    if(msg != NULL) msg->getva(content, score);
                    GiveRewardsVisitor visitor(title, content, score, &info);
                    clan->VisitMembers(visitor);
                    break;
                }
            }
        }
    }

    void ClanRankBattleMgr::GiveWeeklyRewards()
    {
        SYSMSG(title1, 2213);
        SysMsgItem* msg1 = globalSysMsg[2214];
        SYSMSG(title2, 2238);
        SysMsgItem* msg2 = globalSysMsg[2239];

        // 所有人移走
        GameMsgHdr hdr(0x1F4, WORKER_THREAD_WORLD, NULL, 0);
        GLOBAL().PushMsg(hdr, NULL);

        UInt32 ranking = 0;
        for(ClanVec::iterator iter = m_ClanRanking.begin();
            iter != m_ClanRanking.end();++iter)
        {
            Clan* clan = *iter;
            ++ranking;

            if (ranking < PPLACE_MAX)
            {
                if (clan && clan->getOwner())
                {
                    GameMsgHdr hdr(0x1F5, WORKER_THREAD_WORLD, clan->getOwner(), sizeof(ranking));
                    GLOBAL().PushMsg(hdr, &ranking);
                }
            }

            //邮件通知
            class SendMailVisitor : public Visitor<ClanMember>
            {
            public:
                SendMailVisitor(const std::string& title, const std::string& content, UInt32 score, MailItemsInfo* info)
                    :m_Title(title), m_Content(content), m_Score(score), m_pInfo(info){}

                bool operator()(ClanMember* member)
                {
                    if(member->player->GetVar(VAR_WEEKLY_CLANBATTLE) != 0)
                    {
                        member->player->GetMailBox()->newMail(NULL, 0x42, m_Title, m_Content, m_Score, true, m_pInfo);
                        member->player->SetVar(VAR_WEEKLY_CLANBATTLE, 0);
                    }
                    return true;
                }

            private:
                std::string m_Title;
                std::string m_Content;
                UInt32 m_Score;
                MailItemsInfo *m_pInfo;
            };

            UInt32 score = clan->GetBattleScore();
            std::string content1;
            if(msg1 != NULL) msg1->getva(content1, score);

            //每周个人奖励
            for(RewardsMap::reverse_iterator iter = m_WeeklyRewards.rbegin();
                iter != m_WeeklyRewards.rend(); ++iter)
            {
                if(score >= iter->first)
                {
                    RewardVec& rewards = iter->second;
                    MailItemsInfo info(&rewards[0], ClanBattleAward, rewards.size());

                    SendMailVisitor visitor(title1, content1, score, &info);
                    clan->VisitMembers(visitor);
                    break;
                }
            }


            //邮件通知排名奖励
            class SendTextMailVisitor : public Visitor<ClanMember>
            {
            public:
                SendTextMailVisitor(const std::string& title, const std::string& content)
                    :m_Title(title), m_Content(content){}

                bool operator()(ClanMember* member)
                {
                    member->player->GetMailBox()->newMail(NULL, 0x01, m_Title, m_Content);
                    return true;
                }

            private:
                std::string m_Title;
                std::string m_Content;
            };
            //周排名奖励
            for(RewardsMap::iterator iter = m_WeeklyClanSortRewards.begin();
                    iter != m_WeeklyClanSortRewards.end(); ++iter)
            {
                if(ranking <= iter->first)
                {
                    std::string content2;
                    if(msg2 != NULL) msg2->getva(content2, ranking);
                    SendTextMailVisitor visitor(title2, content2);
                    clan->VisitMembers(visitor);

                    GiveClanRewards(clan, iter->second);
                    break;
                }
            }

            //周积分奖励
            for(RewardsMap::reverse_iterator iter = m_WeeklyClanRewards.rbegin();
                    iter != m_WeeklyClanRewards.rend(); ++iter)
            {
                if(score >= iter->first)
                {
                    GiveClanRewards(clan, iter->second);
                    break;
                }
            }
        }
    }

    void ClanRankBattleMgr::GiveClanRewards(Clan* clan, RewardVec& rewards)
    {
        std::ostringstream itemstream;
        for(RewardVec::iterator iter = rewards.begin();
            iter != rewards.end(); ++iter)
        {
            UInt32 num = clan->AddItem(iter->id, iter->count);
            if(num != 0)
            {
                itemstream << iter->id << ',' << num << ';';
            }
        }

        if(!itemstream.str().empty())
        {
            clan->AddItemHistory(ClanItemHistory::CLANBATTLE, m_Now, 0, itemstream.str());
        }
    }

    void ClanRankBattleMgr::MakeDailyMailInfo(UInt32 score, Stream& st)
    {
        for(RewardsMap::iterator iter = m_DailyRewards.begin();
                iter != m_DailyRewards.end(); ++iter)
        {
            if(score <= iter->first)
            {
                MakeMailInfo(st, iter->second);
                return;
            }
        }
        st << UInt16(0);
    }

    void ClanRankBattleMgr::MakeWeeklyMailInfo(UInt32 score, Stream& st)
    {
        for(RewardsMap::reverse_iterator iter = m_WeeklyRewards.rbegin();
                iter != m_WeeklyRewards.rend(); ++iter)
        {
            if(score >= iter->first)
            {
                MakeMailInfo(st, iter->second);
                return;
            }
        }
        st << UInt16(0);
    }

    void ClanRankBattleMgr::MakeMailInfo(Stream& st, RewardVec& rewards)
    {
        st << UInt16(rewards.size());
        for(RewardVec::iterator iter = rewards.begin(); iter != rewards.end(); ++iter)
        {
            st << UInt16(iter->id) << UInt16(iter->count);
        }
    }

    bool ClanRankBattleMgr::AddDailyMailItems(Player* player, UInt32 score)
    {
        for(RewardsMap::iterator iter = m_DailyRewards.begin();
                iter != m_DailyRewards.end(); ++iter)
        {
            if(score <= iter->first)
            {
                return AddMailItems(player, iter->second);
            }
        }
        return false;
    }

    bool ClanRankBattleMgr::AddWeeklyMailItems(Player* player, UInt32 score)
    {
        for(RewardsMap::reverse_iterator iter = m_WeeklyRewards.rbegin();
                iter != m_WeeklyRewards.rend(); ++iter)
        {
            if(score >= iter->first)
            {
                return AddMailItems(player, iter->second);
            }
        }
        return false;
    }

    bool ClanRankBattleMgr::AddMailItems(Player* player, RewardVec& rewards)
    {
        UInt16 grid = 1;
        for(RewardVec::iterator iter = rewards.begin(); iter != rewards.end();
                ++iter)
        {
            grid += player->GetPackage()->GetItemUsedGrids(iter->id, iter->count);
        }

        if(grid > player->GetPackage()->GetRestPackageSize())
        {
            player->sendMsgCode(1, 1011);
            return false;
        }

        for(RewardVec::iterator iter = rewards.begin(); iter != rewards.end();
                ++iter)
        {
            player->GetPackage()->Add(iter->id, iter->count, true, false, FromMail);
        }
        return true;
    }

    void ClanRankBattleMgr::ProcessInit(UInt32 oldtime)
    {
        if(m_Now >= m_StartTime)
        {
            //切换到报名状态，初始10次倒计时
            m_SignupCountDown = RANK_BATTLE_SIGNUP_TIME / 60;
            m_State = STATE_SIGNUP;
            SyncState();
            setStatus(3);
        }
        else
        {
            UInt32 oldweekday = TimeUtil::GetWeekDay(oldtime);
            UInt32 newweekday = TimeUtil::GetWeekDay(m_Now);

            //新的一周重新设置积分和排名
            if(oldweekday == 7 && newweekday != 7)
            {
                //清空原积分排名
                class ClearClanVisitor : public Visitor<Clan>
                {
                public:
                    ClearClanVisitor(){}
                    ~ClearClanVisitor(){}

                public:
                    bool operator()(Clan* clan)
                    {
                        clan->SetBattleScore(0);
                        clan->SetLastBattleRanking(0);
                        clan->SetBattleRanking(0);
                        return true;
                    }
                };
                ClearClanVisitor clearVisitor;
                globalClans.enumerate(clearVisitor);

                //新排名
                UInt32 rank = 0;
                for(ClanVec::iterator iter = m_ClanRanking.begin();
                        iter != m_ClanRanking.end(); ++iter)
                {
                    (*iter)->SetLastBattleRanking(++rank);
                }

                //清空本周积分排行
                m_ClanRanking.clear();
            }


            //新的一天发邮件
            if(oldweekday != newweekday)
            {
                SYSMSG(title, 2240);
                SYSMSG(content, 2241);

                //发放清理仓库通知邮件
                class ClearStorageVisitor : public Visitor<Clan>
                {
                public:
                    ClearStorageVisitor(const std::string& title, const std::string& content)
                        :m_Title(title), m_Content(content){}
                    ~ClearStorageVisitor(){}

                public:
                    bool operator()(Clan* clan)
                    {
                        if(clan->GetGridNum() < 25
                                && clan->getOwner() != NULL)
                        {
                            clan->getOwner()->GetMailBox()->newMail(NULL, 0x01, m_Title, m_Content);
                        }
                        return true;
                    }

                private:
                    std::string m_Title;
                    std::string m_Content;
                };

                ClearStorageVisitor visitor(title, content);
                globalClans.enumerate(visitor);
            }
        }
    }

    void ClanRankBattleMgr::ProcessSignup()
    {
        if(m_SignupCountDown > 0)
        {
            //报名倒计时
            if(m_Now >= m_StartTime + RANK_BATTLE_SIGNUP_TIME - m_SignupCountDown * 60)
            {
                if(m_SignupCountDown == 5 || m_SignupCountDown == 1)
                {
                    SYSMSG_BROADCASTV( 2210, m_SignupCountDown);
                }
                --m_SignupCountDown;
            }
        }
        else if(m_Now >= m_StartTime + RANK_BATTLE_SIGNUP_TIME) //战斗开始
        {
            SYSMSG_BROADCAST(2211);

            GetCanBattleClans();
            //切换到战斗状态
            m_BattleNo = 1;
            m_State = STATE_BATTLE;
            SyncState();
            StartOneBattle();
        }
    }

    void ClanRankBattleMgr::ProcessBattle()
    {
        if(m_Now >= m_StartTime + RANK_BATTLE_SIGNUP_TIME + m_BattleNo * FULL_BATTLE_TIME)
        {
            EndOneBattle();

            if(++m_BattleNo > BATTLE_NUM_PER_DAY) //今天的比赛结束了
            {
                setStatus(2);
                m_singupNotified = false;
                m_battleNotified = false;

                //第二天战斗开始时间
                m_StartTime = m_StartTime + 24 * 60 * 60;
                //m_StartTime = m_StartTime + 2 * 60 * 60;
                m_State = STATE_INIT;
                SyncState();

                //给每天奖励
                GiveDailyRewards();

                //每周最后一场结束给奖励
                if(TimeUtil::GetWeekDay(m_Now) == 7 && TimeUtil::GetWeekDay(m_StartTime) != 7)
                {
                    GiveWeeklyRewards();
                }

                //每天比赛结束发布前六名
                if(m_ClanRanking.size() == 0)
                {
                    SYSMSG_BROADCASTV(2222,"");
                }
                else
                {
                    ClanVec::iterator iter = m_ClanRanking.begin();
                    std::string rankStr;
                    UInt32 first = 2223;
                    SysMsgItem* msg = globalSysMsg[first];
                    if(cfg.merged)
                    {
                        Player* founderMan = globalPlayers[(*iter)->getFounder()];
                        if(founderMan)
                        {
                            if(msg != NULL) msg->getva(rankStr, founderMan->getNameNoSuffix((*iter)->getName()));
                            for(++iter; iter != m_ClanRanking.end() && first < 2228; ++iter)
                            {
                                msg = globalSysMsg[++first];
                                if(msg != NULL) msg->getva(rankStr, founderMan->getNameNoSuffix((*iter)->getName()));
                            }
                            SYSMSG(end,2229);
                            rankStr.append(end);

                            SYSMSG_BROADCASTV(2222,founderMan->getNameNoSuffix(rankStr));
                        }
                    }
                    else
                    {
                        if(msg != NULL) msg->getva(rankStr, (*iter)->getName().c_str());
                        for(++iter; iter != m_ClanRanking.end() && first < 2228; ++iter)
                        {
                            msg = globalSysMsg[++first];
                            if(msg != NULL) msg->getva(rankStr, (*iter)->getName().c_str());
                        }
                        SYSMSG(end,2229);
                        rankStr.append(end);

                        SYSMSG_BROADCASTV(2222,rankStr.c_str());
                    }
                }

                m_Clans.clear();
            }
            else
            {
                StartOneBattle();
            }
        }

        for(BattleVec::iterator iter = m_Battles.begin();
                iter != m_Battles.end(); ++iter)
        {
            (*iter)->Process(m_Now);
        }
    }


    struct RankBattleSorter
    {
        bool operator()(ClanRankBattleInfo* info1, ClanRankBattleInfo* info2) const
        {
            return info1->clan->GetBattleScore() > info2->clan->GetBattleScore();
        }
    };
    void ClanRankBattleMgr::StartOneBattle()
    {
        typedef std::vector<ClanRankBattleInfo*> InfoVec;

        //根据当前积分排序
        InfoVec clans;
        for(ClanMap::iterator iter = m_Clans.begin();
                iter != m_Clans.end(); ++iter)
        {
            clans.push_back(&(iter->second));
        }
        RankBattleSorter sorter;
        std::sort(clans.begin(), clans.end(), sorter);

        InfoVec group;
        for(InfoVec::iterator iter = clans.begin();
                iter != clans.end(); ++iter)
        {
            group.push_back(*iter);
            if(group.size() == 4)
            {
                std::random_shuffle(group.begin(), group.end());

                if(!CreateBattle(group[0], group[1])) return;
                if(!CreateBattle(group[2], group[3])) return;

                group.clear();
            }
        }

        if(group.size() == 0) return;

        std::random_shuffle(group.begin(), group.end());
        if(group.size() == 1)
        {
            //轮空
            if(!CreateBattle(group[0], NULL)) return;
        }
        else
        {
            if(!CreateBattle(group[0],group[1])) return;

            if(group.size() > 2)
            {
                if(!CreateBattle(group[2], NULL)) return;
            }
        }
    }

    void ClanRankBattleMgr::EndOneBattle()
    {
        for(BattleVec::iterator iter = m_Battles.begin();
                iter != m_Battles.end(); ++iter)
        {
            (*iter)->End(true);
            (*iter)->Reset();
            delete *iter;
        }
        m_Battles.clear();

        //进入新的帮派重新排序
        std::set<Clan*> clans;
        clans.insert(m_ClanRanking.begin(), m_ClanRanking.end());
        m_ClanRanking.clear();
        for(ClanMap::iterator iter = m_Clans.begin();
                iter != m_Clans.end(); ++iter){
            Clan* clan = iter->second.clan;
            if(clan->GetBattleScore() != 0)
            {
                clans.insert(clan);
            }
        }
        m_ClanRanking.assign(clans.begin(), clans.end());

        SortClans(true);
    }

    bool ClanRankBattleMgr::CreateBattle(ClanRankBattleInfo* clan1, ClanRankBattleInfo* clan2)
    {
        ClanRankBattle* battle = new(std::nothrow) ClanRankBattle(clan1, clan2);
        if(battle == NULL) return false;
        battle->Start(m_Now);
        m_Battles.push_back(battle);

        return true;
    }


    ClanRankBattleInfo* ClanRankBattleMgr::GetClanInfo(UInt32 id)
    {
        ClanMap::iterator iter = m_Clans.find(id);
        if(iter != m_Clans.end())
        {
            return &(iter->second);
        }
        return NULL;
    }


    void ClanRankBattleMgr::GetCanBattleClans(bool bBegin)
    {
        m_Clans.clear();
        class GetRankBattleClansVisitor : public Visitor<Clan>
        {
        public:
            GetRankBattleClansVisitor(ClanMap& clanMap, UInt32 now, bool bbegin)
                :m_ClanMap(clanMap),m_Now(now),m_bBegin(bbegin){}

            bool operator()(Clan* clan)
            {
                ClanRankBattleInfo info(clan);
                UInt32 res = clan->CheckJoinRankBattle(m_Now, info.players);
                if(res == 2) //满足报名条件
                {
                    if(m_bBegin) clan->SetDailyBattleScore(0);
                    m_ClanMap.insert(std::make_pair(clan->getId(), info));
                }
                else if(res == 1 && m_bBegin) //有报名但未达人数
                {
                    //系统提示
                    SysMsgItem *sysMsgItem = globalSysMsg[2215];
                    if(sysMsgItem != NULL)
                    {
                        Stream stream;
                        sysMsgItem->getvap(&stream, 5);
                        clan->broadcast(stream);
                    }
                }
                return true;
            }

        private:
            ClanMap& m_ClanMap;
            UInt32 m_Now;
            bool m_bBegin;
        };
        GetRankBattleClansVisitor visitor(m_Clans, m_Now, bBegin);
        globalClans.enumerate(visitor);
    }

    struct ClanRankBattleSorter
    {
        bool operator()(Clan* clan1, Clan* clan2) const
        {
            return clan1->GetBattleScore() > clan2->GetBattleScore();
        }
    };
    void ClanRankBattleMgr::SortClans(bool bNotify)
    {

        ClanRankBattleSorter sorter;
        std::sort(m_ClanRanking.begin(), m_ClanRanking.end(), sorter);

        UInt32 ranking = 0;
        for(ClanVec::iterator iter = m_ClanRanking.begin();
                iter != m_ClanRanking.end(); ++iter)
        {
            UInt32 oldRanking = (*iter)->GetBattleRanking();
            UInt32 newRanking = ++ranking;

            (*iter)->SetBattleRanking(newRanking);

            if(bNotify && oldRanking != newRanking)
            {
                Stream stream(REP::CLAN_RANKBATTLE_REPINIT);
                stream << UInt8(7);
                stream << ranking;
                stream << Stream::eos;
                (*iter)->broadcast(stream);
            }
        }
    }


    void ClanRankBattleMgr::CheckAddExp()
    {
        if(m_Now < m_expTime) return;


        for(PlayerSet::iterator iter = m_InplacePlayers.begin();
                iter != m_InplacePlayers.end(); ++iter)
        {
            Player* player = *iter;
            if(player->getLocation() == RANK_BATTLE_LOCATION
                && player->getThreadId() == WORKER_THREAD_NEUTRAL)
            {
                UInt32 exp = ((player->GetLev() - 10) * UInt32(player->GetLev() / 10) * 5 + 25) * 2;
                player->AddExp(exp);
            }
        }

        m_expTime = m_Now + 60;
    }

    void ClanRankBattleMgr::sendDaily(Player* player)
    {
        Stream st(REP::DAILY_DATA);
        st << static_cast<UInt8>(10);
        st << static_cast<UInt8>(m_Status);
        st << Stream::eos;
        if (player)
            player->send(st);
        else
            NETWORK()->Broadcast(st);
    }

    void ClanRankBattleMgr::removeClanRank(Clan* clan)
    {
        if(!clan)
            return;

        for(ClanVec::iterator it = m_ClanRanking.begin(); it != m_ClanRanking.end(); ++ it)
        {
            if(*it == clan)
                m_ClanRanking.erase(it);
        }
    }
}



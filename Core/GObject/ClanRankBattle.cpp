#include "ClanRankBattle.h"
#include <algorithm>
#include "Clan.h"
#include "Server/SysMsg.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Player.h"
#include "Country.h"
#include "Script/GameActionLua.h"

namespace GObject
{        
    
    //战斗准备时间
    const static UInt32 PREPARE_TIME = 5 * 60;
    //实际战斗时间
    const static UInt32 BATTLE_TIME = 15 * 60;
    //完整战斗时间
    const static UInt32 FULL_BATTLE_TIME = PREPARE_TIME + BATTLE_TIME;
    //每天战斗次数
    const static UInt32 BATTLE_NUM_PER_DAY = 3;


    //每个战场的分数
    const static UInt32 BATTLE_FIELD_SCORE[] = {3, 5, 6, 7, 8, 10};


    //帮会排名战报名开始时间
    const static UInt32 RANK_BATTLE_SIGNUP_BEGINTIME = 21 * 60 * 60;
    //帮会排名战报名持续时间
    const static UInt32 RANK_BATTLE_SIGNUP_TIME = 10 * 60;

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

        m_Clan1 = m_Clan2 = 0;
        m_StartTime = 0;
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
        }
        for(PlayerVec::const_iterator iter = team2.begin();
                iter != team2.end(); ++iter){
            (*iter)->SetClanBattleStatus(PLAYER_WAIT);
        }

        m_WaitPlayers.clear();
        m_WaitPlayers.insert(std::make_pair(clan1, team1));
        m_WaitPlayers.insert(std::make_pair(clan2, team2));
        m_DeadPlayers.clear();

        m_StartTime = now;
        m_bEnd = false;
        m_Winner = 0;
        m_Round = 0;

        TRACE_LOG("clanrankbattle field(%u) start(%u:%u)(%u:%u).", m_Id, clan1, UInt32(team1.size()), clan2, UInt32(team2.size()));
    }


    void ClanRankBattleField::Process(UInt32 now)
    {
        if(IsEnd()) return;

        //开始下一轮
        if(now >= m_StartTime + m_Round * RANK_BATTLE_FIGHT_TIME)
        {
            UInt32 fightNum1 = m_WaitPlayers[m_Clan1].size();
            UInt32 fightNum2 = m_WaitPlayers[m_Clan2].size();

            m_StatusChanged.clear();
            ResetPlayerStatus(m_Clan1);
            ResetPlayerStatus(m_Clan2);

            UInt32 fightNum = std::min(fightNum1, fightNum2);
            if(fightNum > 3) fightNum = 3;

            if(fightNum > 0)
            {
                std::map<UInt32, PlayerVec> fightPlayers;
                //从队伍头取出对阵玩家
                for(UInt32 i = 0; i < fightNum; ++i)
                {
                    PlayerVec::iterator iter1 = m_WaitPlayers[m_Clan1].begin();
                    fightPlayers[m_Clan1].push_back(*iter1);
                    m_WaitPlayers[m_Clan1].erase(iter1);

                    PlayerVec::iterator iter2 = m_WaitPlayers[m_Clan2].begin();
                    fightPlayers[m_Clan2].push_back(*iter2);
                    m_WaitPlayers[m_Clan2].erase(iter2);
                }


                Stream battleStream(REP::CLAN_RANKBATTLE_REP);
                battleStream << UInt8(10);
                battleStream << UInt8(fightNum);

                for(UInt32 i = 0; i < fightNum; ++i)
                {
                    Player* player1 = fightPlayers[m_Clan1][i];
                    Player* player2 = fightPlayers[m_Clan2][i];
 
                    player1->SetClanBattleStatus(PLAYER_BATTLE);
                    player2->SetClanBattleStatus(PLAYER_BATTLE);

                    m_StatusChanged[player1->getId()] = PLAYER_BATTLE;
                    m_StatusChanged[player2->getId()] = PLAYER_BATTLE;

                    int turns = 0;
                    UInt32 rid = 0;
                    bool fightRes = player1->challenge(player2, &rid, &turns, false, 0, true, Battle::BS_COPY5, 0x03);
                    player1->setBuffData(PLAYER_BUFF_ATTACKING, now + 2 * turns);
                    player2->setBuffData(PLAYER_BUFF_ATTACKING, now + 2 * turns);

                    if(fightRes) //玩家1胜
                    {
                        player1->IncClanBattleWinTimes();   //增加本场连胜次数
                        
                        player2->SetClanBattleWinTimes(0);
                        player2->regenAll();

                        player1->AddClanBattleScore(player1->GetClanBattleWinTimes());

                        m_WaitPlayers[m_Clan1].push_back(player1); //放回等待队列
                        m_DeadPlayers[m_Clan2].push_back(player2); //死亡

                        battleStream << UInt8(1);
                    }
                    else //玩家2胜
                    {
                        player1->SetClanBattleWinTimes(0);
                        player1->regenAll();

                        player2->IncClanBattleWinTimes();

                        player2->AddClanBattleScore(player2->GetClanBattleWinTimes());

                        m_DeadPlayers[m_Clan1].push_back(player1);
                        m_WaitPlayers[m_Clan2].push_back(player2);
                        battleStream << UInt8(2);
                    }

                    battleStream << rid;
                }

                battleStream << Stream::eos;
                Broadcast(m_Clan1, battleStream);
                Broadcast(m_Clan2, battleStream);
            }
            else
            {
                m_bEnd = true;
                if(fightNum1 > 0) m_Winner = m_Clan1;
                else if(fightNum2 > 0) m_Winner = m_Clan2;
                else m_Winner = 0;
            }

            //刷新状态变化
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
            m_pBattle->Broadcast(stream);

            ++m_Round;
        }
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

        for(PlayerVec::iterator iter = players1.begin();
                iter != players1.end(); ++iter)
        {
            Player* player = *iter;
            player->AddClanBattleScore(extScore1);
            
            Clan* clan = player->getClan();
            if(clan != NULL) //增加帮贡献
            {
                UInt32 proffer = player->GetClanBattleScore() * 10;
                clan->addMemberProffer(player, proffer);
                player->AddVar(VAR_CLANBATTLE_HONOUR, proffer);
            }
            
            ResetPlayerData(player);
        }

        for(PlayerVec::iterator iter = players2.begin();
                iter != players2.end(); ++iter)
        {
            Player* player = *iter;
            player->AddClanBattleScore(extScore2);
             
            Clan* clan = player->getClan();
            if(clan != NULL) //增加帮贡献
            {
                UInt32 proffer = player->GetClanBattleScore() * 10;
                clan->addMemberProffer(player, proffer);
                player->AddVar(VAR_CLANBATTLE_HONOUR, proffer);
            }
            
            ResetPlayerData(player);
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
        m_pBattle->Broadcast(stream);
    }

    void ClanRankBattleField::ResetPlayerStatus(UInt32 clan)
    {
        std::vector<Player*> winPlayers;

        PlayerVec& players1 = m_WaitPlayers[clan];
        PlayerVec& players2 = m_DeadPlayers[clan];
        
        for(PlayerVec::iterator iter = players1.begin();
                iter != players1.end(); )
        {
            Player* player = *iter;
            
            if(player->GetClanBattleStatus() != PLAYER_BATTLE)
            {
                ++iter;
                continue;
            }

            if(player->GetClanBattleWinTimes() == 10) //连胜10场了
            {
                player->SetClanBattleStatus(PLAYER_WIN);
                m_StatusChanged[player->getId()] = PLAYER_WIN;
                players2.push_back(player);
                    
                iter = players1.erase(iter);
            }
            else
            {
                player->SetClanBattleStatus(PLAYER_WAIT);
                m_StatusChanged[player->getId()] = PLAYER_WAIT;
                    
                ++iter;
            }
        }

        for(PlayerVec::iterator iter = players2.begin();
                iter != players2.end(); ++iter)
        {
            if((*iter)->GetClanBattleStatus() != PLAYER_BATTLE){ continue; }
                
            (*iter)->SetClanBattleStatus(PLAYER_DEAD);
            m_StatusChanged[(*iter)->getId()] = PLAYER_DEAD;
        }
    }

    void ClanRankBattleField::ResetPlayerData(Player* player)
    {                
        player->clearHIAttr();
        player->regenAll();
        player->SetClanBattleStatus(PLAYER_WAIT);
        player->SetClanBattleScore(0);
        player->SetClanBattleWinTimes(0);
        player->ClearClanBattleSkillFlag();   
    }

    void ClanRankBattleField::Broadcast(UInt32 clan, Stream& stream)
    {
        PlayerVec& players1 = m_WaitPlayers[clan];
        PlayerVec& players2 = m_DeadPlayers[clan];     

        for(PlayerVec::iterator iter = players1.begin();
                iter != players1.end(); ++iter)
        {
            (*iter)->send(stream);
            TRACE_LOG("send battle report to player %s.", (*iter)->getName().c_str());
        }

        for(PlayerVec::iterator iter = players2.begin();
                iter != players2.end(); ++iter)
        {
            (*iter)->send(stream);
            TRACE_LOG("send battle report to player %s.", (*iter)->getName().c_str());
        }
    }




    ClanRankBattle::ClanRankBattle(ClanRankBattleInfo* clan1, ClanRankBattleInfo* clan2)
    {
        m_Clan1 = clan1;
        m_Clan2 = clan2;

        m_ClanScore1 = m_ClanScore2 = 0;
        if(clan2 == NULL) m_ClanScore1 = 40;
        if(clan1 == NULL) m_ClanScore2 = 40;

        m_State = STATE_PREPARE;
        m_StartTime = 0;
        m_Now = 0;
        m_bEnd = false;

        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            m_Fields[i].Init(i, this);
        }
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
                BroadcastStatus();
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
                   
                        if(m_Clan1 != NULL && winner == m_Clan1->clan->getId()) m_ClanScore1 += BATTLE_FIELD_SCORE[i];
                        else if(m_Clan2 != NULL && winner == m_Clan2->clan->getId()) m_ClanScore2 += BATTLE_FIELD_SCORE[i];
                        BroadcastScores();
                    }
                }
            }
            if(endNum == RANK_BATTLE_FIELD_NUM)
            {
                End();
            }
        }
    }

    void ClanRankBattle::Start(UInt32 now)
    {
        m_Now = now;
        m_StartTime = now;
        
        //clan->clan->SetBattleScore(clan->clan->GetBattleScore() + 40);

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
                msg->getvap(&stream1, m_Clan2->clan->getName().c_str());
                m_Clan1->clan->broadcast(stream1);
                msg->getvap(&stream2, m_Clan1->clan->getName().c_str());
                m_Clan2->clan->broadcast(stream2);
            }
            m_Clan1->battle = this;
            m_Clan2->battle = this;
        }

        BroadcastStatus();
    }

    void ClanRankBattle::End()
    {
        if(m_State == STATE_PREPARE) return;
        if(IsEnd()) return;
        m_bEnd = true;

        //结算积分
        m_ClanScore1 = 0;
        m_ClanScore2 = 0;
        UInt32 winner = m_Clan1->clan->getId();


        if(m_Clan2 == NULL)
        {
            m_ClanScore1 = 20;
        }
        else
        {
            for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
            {
                if(!m_Fields[i].IsEnd()) continue;
      
                winner = m_Fields[i].GetWinner();

                if(m_Clan1 != NULL && winner == m_Clan1->clan->getId()) m_ClanScore1 += BATTLE_FIELD_SCORE[i];
                else if(m_Clan2 != NULL && winner == m_Clan2->clan->getId()) m_ClanScore2 += BATTLE_FIELD_SCORE[i];
            }
        }

        //个人额外积分
        UInt32 extScore1 = 5;
        UInt32 extScore2 = 5;

        if(m_ClanScore1 > m_ClanScore2)
        {
            m_ClanScore1 += 20;
            if(m_Clan1 != NULL) winner = m_Clan1->clan->getId();
            extScore1 = 20;
        }
        else if(m_ClanScore2 > m_ClanScore1)
        {
            m_ClanScore2 += 20;
            if(m_Clan2 != NULL) winner = m_Clan2->clan->getId();
            extScore2 = 20;
        }
        else
        {
            if(m_Clan1 != NULL && winner == m_Clan1->clan->getId()) m_ClanScore1 += 20;
            else m_ClanScore2 += 20;
        }

        if(m_Clan1 != NULL) m_Clan1->clan->SetBattleScore(m_Clan1->clan->GetBattleScore() + m_ClanScore1);
        if(m_Clan2 != NULL) m_Clan2->clan->SetBattleScore(m_Clan2->clan->GetBattleScore() + m_ClanScore2);

        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            UInt32 winner = m_Fields[i].GetWinner();

            if(m_Clan1 != NULL && winner == m_Clan1->clan->getId())
            {
                m_Fields[i].End(extScore1 + BATTLE_FIELD_SCORE[i], extScore2);
            }
            else if(m_Clan2 != NULL && winner == m_Clan2->clan->getId())
            {
                m_Fields[i].End(extScore1, extScore2 + BATTLE_FIELD_SCORE[i]);
            }
        }

        if(m_Clan1 != NULL) m_Clan1->clan->BroadcastBattleData(m_Now);
        if(m_Clan2 != NULL) m_Clan2->clan->BroadcastBattleData(m_Now);

        BroadcastScores(true);
    }

    void ClanRankBattle::Clear()
    {
        if(m_Clan1 != NULL) m_Clan1->battle = NULL;
        if(m_Clan2 != NULL) m_Clan2->battle = NULL;
    }

    void ClanRankBattle::OnPlayerLeave(Player* player)
    {
        if(m_State == STATE_PREPARE) return;

        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            if(m_Fields[i].OnPlayerLeave(player))
            {
                break;
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
        stream << UInt8(4);
        stream << UInt8(skill->id);
        stream << Stream::eos;
        player->send(stream);
    }

    void ClanRankBattle::SendBattleStatus(Player* player)
    {
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(0);
        stream << UInt8(m_State);
        UInt32 time = 0;
        if(m_State == STATE_PREPARE)
        {
            if(m_StartTime + PREPARE_TIME > m_Now)
            {
                time = m_StartTime + PREPARE_TIME - m_Now;
            }
        }
        else
        {
            if(m_StartTime + FULL_BATTLE_TIME > m_Now)
            {
                time = m_StartTime + FULL_BATTLE_TIME - m_Now;
            }
        }
        
        stream << time;
        stream << Stream::eos;
        player->send(stream);
    }

    void ClanRankBattle::BroadcastStatus()
    {
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(0);
        stream << UInt8(m_State);
        UInt32 time = 0;
        if(m_State == STATE_PREPARE)
        {
            if(m_StartTime + PREPARE_TIME > m_Now)
            {
                time = m_StartTime + PREPARE_TIME - m_Now;
            }
        }
        else
        {
            if(m_StartTime + FULL_BATTLE_TIME > m_Now)
            {
                time = m_StartTime + FULL_BATTLE_TIME - m_Now;
            }
        }

        stream << time;
        stream << Stream::eos;
        Broadcast(stream);
    }

    void ClanRankBattle::BroadcastScores(bool bEnd)
    {
        Stream stream1(REP::CLAN_RANKBATTLE_REP);
        stream1 << UInt8(11) << UInt8(bEnd ? 1 : 0) << m_ClanScore1 << m_ClanScore2 << Stream::eos;
        if(m_Clan1 != NULL) m_Clan1->Broadcast(stream1);

        Stream stream2(REP::CLAN_RANKBATTLE_REP);
        stream2 << UInt8(11) << UInt8(bEnd ? 1 : 0) << m_ClanScore2 << m_ClanScore1 << Stream::eos;
        if(m_Clan2 != NULL) m_Clan2->Broadcast(stream2);
    }

    void ClanRankBattle::SendBattleInfo(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(5);
        
        ClanRankBattleInfo* myClan = NULL;
        ClanRankBattleInfo* otherClan = NULL;
        UInt32 myScore = 0;
        UInt32 otherScore = 0;
        if(m_Clan1 != NULL && player->getClan() == m_Clan1->clan)
        {
            myClan = m_Clan1;
            myScore = m_ClanScore1;
            otherClan = m_Clan2;
            otherScore = m_ClanScore2;
        }
        else if(m_Clan2 != NULL && player->getClan() == m_Clan2->clan)
        {
            myClan = m_Clan2;
            myScore = m_ClanScore2;
            otherClan = m_Clan1;
            otherScore = m_ClanScore1;
        }
        else
        {
            return;
        }

        stream << ((otherClan != NULL) ? otherClan->clan->getName() : "");
        stream << myScore;
        stream << otherScore;

        UInt32 num1 = 0;
        UInt32 num2 = 0;
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
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



    ClanRankBattleMgr::ClanRankBattleMgr()
    {
        m_State = STATE_INIT;
        m_StartTime = 0;
        m_SignupCountDown = 0;
        m_BattleNo = 0;
        m_Now = 0;
        m_expTime = 0;

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
        }

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
            m_Skills[i].attrs.attackP = m_Skills[i].attrs.magatkP = buff.get<float>(2);
            m_Skills[i].attrs.defendP = m_Skills[i].attrs.magdefP = buff.get<float>(3);
            m_Skills[i].attrs.hpP = buff.get<float>(4);
            m_Skills[i].attrs.actionP = buff.get<float>(5);
            m_Skills[i].attrs.hitrateP = buff.get<float>(6);
            m_Skills[i].attrs.evadeP = buff.get<float>(7);
            m_Skills[i].attrs.criticalP = buff.get<float>(8);
            m_Skills[i].attrs.criticaldmg = buff.get<float>(9);
            m_Skills[i].attrs.pierceP = buff.get<float>(10);
            m_Skills[i].attrs.counterP = buff.get<float>(11);
            m_Skills[i].attrs.magresP = buff.get<float>(12);
            m_Skills[i].attrs.aura = buff.get<float>(13);
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
        if(clan == NULL || m_State != STATE_BATTLE) return false;
        
        return GetClanInfo(clan->getId()) != NULL; 
    }

    void ClanRankBattleMgr::Process(UInt32 now)
    {
        UInt32 oldTime = m_Now;
        m_Now = now;
        
        switch(m_State)
        {
            case STATE_INIT:
                {
                    bool bWeekChange = TimeUtil::GetWeekDay(oldTime) == 7 && TimeUtil::GetWeekDay(m_Now) == 1;
                    ProcessInit(bWeekChange);
                }
                break;
            case STATE_SIGNUP:
                {
                    ProcessSignup();
                    CheckAddExp();
                }
                break;
            case STATE_BATTLE:
                {
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
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            std::sort(pInfo->players[i].begin(), pInfo->players[i].end(), sorter);
        }

        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(3);
        stream << UInt8(pInfo->GetPlayerNum());
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            UInt32 pos = 0;
            PlayerVec& players = pInfo->players[i];
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

    void ClanRankBattleMgr::GetRewards(Player* player)
    {
        //TODO
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

        if(!info->HasPlayer(player)) return;

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

        std::vector<UInt32> skills;
        for(UInt32 i = 0; i < RANK_BATTLE_SKILL_NUM; ++i)
        {
            if(player->CheckClanBattleSkillFlag(i))
            {
                skills.push_back(i);
            }
        }

        Stream st(REP::CLAN_RANKBATTLE_REP);
        st << UInt8(1);
        st << UInt8(skills.size());
        for(std::vector<UInt32>::iterator iter = skills.begin();
                iter != skills.end(); ++iter)
        {
            st << UInt8(*iter);
        }
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

    void ClanRankBattleMgr::SendRewards(Player* player)
    {
        //TODO
    }

    void ClanRankBattleMgr::ProcessInit(bool bWeekChange)
    {
        if(m_Now >= m_StartTime)
        {
            //切换到报名状态，初始10次倒计时
            m_SignupCountDown = RANK_BATTLE_SIGNUP_TIME / 60;
            m_State = STATE_SIGNUP;
            SyncState();
        }
        else
        {
            //新的一周重新设置积分和排名
            if(bWeekChange)
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
                //第二天战斗开始时间
                m_Clans.clear();
                m_StartTime = m_StartTime + 24 * 60 * 60;
                m_State = STATE_INIT;
                SyncState();

                //每周最后一场结束发邮件
                if(TimeUtil::GetWeekDay(m_Now) == 7)
                {
                    SysMsgItem* msg = globalSysMsg[2214];
                    UInt32 ranking = 0;
                    for(ClanVec::iterator iter = m_ClanRanking.begin();
                        iter != m_ClanRanking.end();++iter)
                    {
                        class SendMailVisitor : public Visitor<ClanMember>
                        {
                        public:
                            SendMailVisitor(const std::string& title, const std::string& content)
                                :m_Title(title), m_Content(content){}

                            bool operator()(ClanMember* member)
                            {
                                member->player->GetMailBox()->newMail(NULL, 1, m_Title, m_Content);
                                return true;
                            }

                        private:
                            std::string m_Title;
                            std::string m_Content;
                        };
                        std::string content;
                        SYSMSG(title, 2213);
                        if(msg != NULL) msg->getva(content, ++ranking);
                        SendMailVisitor visitor(title,content);
                        (*iter)->VisitMembers(visitor);
                    }
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
            (*iter)->End();
            (*iter)->Clear();
            delete *iter;
        }
        m_Battles.clear();
        
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


    void ClanRankBattleMgr::GetCanBattleClans(bool bNotify)
    {
        m_Clans.clear();
        class GetRankBattleClansVisitor : public Visitor<Clan>
        {
        public:
            GetRankBattleClansVisitor(ClanMap& clanMap, UInt32 now, bool bnotify)
                :m_ClanMap(clanMap),m_Now(now),m_bNotify(bnotify){}

            bool operator()(Clan* clan)
            {
                ClanRankBattleInfo info(clan);
                UInt32 res = clan->CheckJoinRankBattle(m_Now, info.players);
                if(res == 2) //满足报名条件
                {
                    m_ClanMap.insert(std::make_pair(clan->getId(), info));
                }
                else if(res == 1 && m_bNotify) //有报名但未达人数
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
            bool m_bNotify;
        };
        GetRankBattleClansVisitor visitor(m_Clans, m_Now, bNotify);
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
        m_ClanRanking.clear();
        class GetClansVisitor : public Visitor<Clan>
        {
        public:
            GetClansVisitor(ClanVec& list)
                :m_ClanList(list){}

            bool operator()(Clan* clan)
            {
                if(clan->GetBattleScore() != 0)
                {
                    m_ClanList.push_back(clan);
                }
                return true;
            }

        private:
            ClanVec& m_ClanList;
        };
        GetClansVisitor visitor(m_ClanRanking);
        globalClans.enumerate(visitor);

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
}



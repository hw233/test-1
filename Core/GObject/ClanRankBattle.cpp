#include "ClanRankBattle.h"
#include "Clan.h"
#include "Server/SysMsg.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Player.h"
#include <algorithm>

namespace GObject
{        
    
    //战斗准备时间
    const static UInt32 PREPARE_TIME = 5 * 60;
    //实际战斗时间
    const static UInt32 BATTLE_TIME = 15 * 60;
    //完整战斗时间
    const static UInt32 FULL_BATTLE_TIME = PREPARE_TIME + BATTLE_TIME;


    //每个战场的分数
    const static UInt32 BATTLE_FIELD_SCORE[] = {3, 5, 6, 7, 8, 10};


    //帮会排名战报名开始时间
    const static UInt32 RANK_BATTLE_SIGNUP_BEGINTIME = 21 * 60 * 60;
    //帮会排名战报名持续时间
    const static UInt32 RANK_BATTLE_SIGNUP_TIME = 10 * 60;

    //每场战斗准备时间
    const static UInt32 RANK_BATTLE_PREPARE_TIME = 5 * 60;
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

    void ClanRankBattleInfo::RemovePlayer(Player* player)
    {
        for(PlayerMap::iterator iter = players.begin();
                iter != players.end(); ++iter)
        {
            PlayerVec& list = iter->second;
            for(PlayerVec::iterator iter2 = list.begin();
                    iter2 != list.end(); ++iter2)
            {
                if(*iter2 == player)
                {
                    list.erase(iter2);
                    return;
                }
            }
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

        //清空相关数据
        for(PlayerVec::const_iterator iter = team1.begin();
                iter != team1.end(); ++iter){
            (*iter)->SetClanBattleStatus(PLAYER_WAIT);
            (*iter)->SetClanBattleScore(0);
            (*iter)->SetClanBattleWinTimes(0);
            (*iter)->regenAll();
        }
        for(PlayerVec::const_iterator iter = team2.begin();
                iter != team2.end(); ++iter){
            (*iter)->SetClanBattleStatus(PLAYER_WAIT);
            (*iter)->SetClanBattleScore(0);
            (*iter)->SetClanBattleWinTimes(0);
            (*iter)->regenAll();
        }

        m_WaitPlayers.clear();
        m_WaitPlayers.insert(std::make_pair(clan1, team1));
        m_WaitPlayers.insert(std::make_pair(clan2, team2));

        m_StartTime = now;
        m_bEnd = false;
        m_Winner = 0;
        m_Round = 0;
    }


    void ClanRankBattleField::Process(UInt32 now)
    {
        if(IsEnd()) return;

        //开始下一轮
        if(now >= m_StartTime + m_Round * RANK_BATTLE_FIGHT_TIME)
        {
            UInt32 fightNum1 = m_WaitPlayers[m_Clan1].size();
            if(fightNum1 == 0) //没玩家了，战役结束
            {
                m_bEnd = true;
                m_Winner = m_Clan2;
                return;
            }
            UInt32 fightNum2 = m_WaitPlayers[m_Clan2].size();
            if(fightNum2 == 0)
            {
                m_bEnd = true;
                m_Winner = m_Clan1;
                return;
            }

            m_StatusChanged.clear();
            ResetPlayerStatus(m_Clan1);
            ResetPlayerStatus(m_Clan2);

            UInt32 fightNum = std::min(fightNum1, fightNum2);
            if(fightNum > 3) fightNum = 3;

            std::map<UInt32, PlayerVec> fightPlayers;
            //取出对阵玩家
            for(UInt32 i = 0; i < fightNum; ++i)
            {
                PlayerVec::iterator iter1 = m_WaitPlayers[m_Clan1].begin();
                fightPlayers[m_Clan1].push_back(*iter1);
                m_WaitPlayers[m_Clan1].erase(iter1);

                PlayerVec::iterator iter2 = m_WaitPlayers[m_Clan2].begin();
                fightPlayers[m_Clan2].push_back(*iter2);
                m_WaitPlayers[m_Clan2].erase(iter2);
            }

            for(UInt32 i = 0; i < fightNum; ++i)
            {
                Player* player1 = fightPlayers[m_Clan1][i];
                Player* player2 = fightPlayers[m_Clan2][i];
 
                player1->SetClanBattleStatus(PLAYER_BATTLE);
                player2->SetClanBattleStatus(PLAYER_BATTLE);

                m_StatusChanged[player1->getId()] = PLAYER_BATTLE;
                m_StatusChanged[player2->getId()] = PLAYER_BATTLE;

                int turns = 0;
                bool fightRes = player1->challenge(player2, NULL, &turns, false, 0, true, Battle::BS_COPY5, true);
                player1->setBuffData(PLAYER_BUFF_ATTACKING, now + 2 * turns);
                player2->setBuffData(PLAYER_BUFF_ATTACKING, now + 2 * turns);

                if(fightRes)
                {
                    player1->IncClanBattleWinTimes();   //增加本场连胜次数
                    player2->SetClanBattleWinTimes(0);

                    player1->AddClanBattleScore(player1->GetClanBattleWinTimes());

                    m_WaitPlayers[m_Clan1].push_back(player1);
                    m_DeadPlayers[m_Clan2].push_back(player2);
                }
                else
                {
                    player1->SetClanBattleWinTimes(0);
                    player2->IncClanBattleWinTimes();

                    player2->AddClanBattleScore(player2->GetClanBattleWinTimes());

                    m_DeadPlayers[m_Clan1].push_back(player1);
                    m_WaitPlayers[m_Clan2].push_back(player2);
                }
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
            m_pBattle->GetClan(m_Clan1)->Broadcast(stream);
            m_pBattle->GetClan(m_Clan2)->Broadcast(stream);

            ++m_Round;
        }
    }

    bool ClanRankBattleField::OnPlayerLeave(Player* player)
    {
        Clan* clan = player->getClan();
        if(clan == NULL) return false;

        UInt32 id = clan->getId();
        if(id != m_Clan1 && id != m_Clan2) return false; //不是两个帮会之一

        PlayerVec& players = m_WaitPlayers[id];
        for(PlayerVec::iterator iter = players.begin(); iter != players.end(); ++iter)
        {
            if(*iter == player)
            {
                players.erase(iter);
                NotifyPlayerLeave(player);
                return true;
            }
        }

        players = m_DeadPlayers[id];
        for(PlayerVec::iterator iter = players.begin(); iter != players.end(); ++iter)
        {
            if(*iter == player)
            {
                players.erase(iter);
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

    void ClanRankBattleField::AddProffer(UInt32 extScore1, UInt32 extScore2)
    {
        m_WaitPlayers[m_Clan1].assign(m_DeadPlayers[m_Clan1].begin(), m_DeadPlayers[m_Clan1].end());
        m_WaitPlayers[m_Clan2].assign(m_DeadPlayers[m_Clan2].begin(), m_DeadPlayers[m_Clan2].end());

        PlayerVec& players = m_WaitPlayers[m_Clan1];
        for(PlayerVec::iterator iter = players.begin();
                iter != players.end(); ++iter)
        {
            Player* player = *iter;
            player->AddClanBattleScore(extScore1);
            Clan* clan = player->getClan();
            if(clan != NULL) clan->addMemberProffer(player, player->GetClanBattleScore() * 10);
        }

        players = m_WaitPlayers[m_Clan2];
        for(PlayerVec::iterator iter = players.begin();
                iter != players.end(); ++iter)
        {
            Player* player = *iter;
            player->AddClanBattleScore(extScore2);
            Clan* clan = player->getClan();
            if(clan != NULL) clan->addMemberProffer(player, player->GetClanBattleScore() * 10);
        }
    }
 
    void ClanRankBattleField::FillPlayers(Stream& stream, UInt32 clan)
    {
        if(clan != m_Clan1 && clan != m_Clan2) return;


        PlayerVec& players = m_WaitPlayers[clan];
        for(PlayerVec::iterator iter = players.begin();
                iter != players.end(); ++iter)
        {
            FillPlayer(stream, *iter);
        }

        players = m_DeadPlayers[clan];
        for(PlayerVec::iterator iter = players.begin();
                iter != players.end(); ++iter)
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
        m_pBattle->GetClan(m_Clan1)->Broadcast(stream);
        m_pBattle->GetClan(m_Clan2)->Broadcast(stream);
    }

    void ClanRankBattleField::ResetPlayerStatus(UInt32 clan)
    {
        PlayerVec& players = m_WaitPlayers[clan];
        for(PlayerVec::iterator iter = players.begin();
                iter != players.end(); ++iter)
        {
            if((*iter)->GetClanBattleStatus() == PLAYER_BATTLE)
            {
                (*iter)->SetClanBattleStatus(PLAYER_WAIT);
                m_StatusChanged[(*iter)->getId()] = PLAYER_WAIT;
            }
        }

        players = m_DeadPlayers[clan];
        for(PlayerVec::iterator iter = players.begin();
                iter != players.end(); ++iter)
        {
            if((*iter)->GetClanBattleStatus() == PLAYER_BATTLE)
            {
                (*iter)->SetClanBattleStatus(PLAYER_DEAD);
                m_StatusChanged[(*iter)->getId()] = PLAYER_DEAD;
            }
        }
    }





    ClanRankBattle::ClanRankBattle(ClanRankBattleInfo* clan1, ClanRankBattleInfo* clan2)
    {
        m_Clan1 = clan1;
        m_Clan2 = clan2;

        m_State = STATE_PREPARE;
        m_StartTime = 0;

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
        if(m_State == STATE_PREPARE)  //准备状态
        {
            if(now >= m_StartTime + RANK_BATTLE_PREPARE_TIME) //开始战斗时间
            {
                for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
                {
                    PlayerVec& players = m_Clan1->players[i];
                    PlayerVec battleList1;
                    for(PlayerVec::iterator iter = players.begin(); iter != players.end(); ++iter)
                    {
                        if((*iter)->getLocation() == RANK_BATTLE_LOCATION) battleList1.push_back(*iter); //选取当前在国战点上的玩家战斗
                    }

                    players = m_Clan2->players[i];
                    PlayerVec battleList2;
                    for(PlayerVec::iterator iter = players.begin(); iter != players.end(); ++iter)
                    {
                        if((*iter)->getLocation() == RANK_BATTLE_LOCATION) battleList2.push_back(*iter);
                    }
                    
                    m_Fields[i].Start(m_Clan1->clan->getId(), battleList1, m_Clan2->clan->getId(), battleList2, now);
                }
                //切换到战斗状态
                m_State = STATE_BATTLE;
            }
        }
        else
        {
            for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
            {
                m_Fields[i].Process(now);
            }
        }
    }

    void ClanRankBattle::Start(UInt32 now)
    {
        m_StartTime = now;

        Stream stream1;
        Stream stream2;
        SysMsgItem* msg = globalSysMsg[2212];
        if(msg != NULL)
        {
            msg->getvap(&stream1, m_Clan2->clan->getName().c_str());
            msg->getvap(&stream2, m_Clan1->clan->getName().c_str());
        
            m_Clan1->Broadcast(stream1, true);
            m_Clan2->Broadcast(stream2, true);
        }
    }

    void ClanRankBattle::End()
    {
        if(m_State == STATE_PREPARE) return;

        //结算积分
        UInt32 clanScore1 = 0;
        UInt32 clanScore2 = 0;
        UInt32 winner = m_Clan1->clan->getId();
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            if(!m_Fields[i].IsEnd()) continue;
                
            winner = m_Fields[i].GetWinner();

            if(winner == m_Clan1->clan->getId()) clanScore1 += BATTLE_FIELD_SCORE[i];
            else clanScore2 += BATTLE_FIELD_SCORE[i];
        }

        //个人额外积分
        UInt32 extScore1 = 5;
        UInt32 extScore2 = 5;

        if(clanScore1 > clanScore2)
        {
            clanScore1 += 20;
            winner = m_Clan1->clan->getId();
            extScore1 = 20;
        }
        else if(clanScore2 > clanScore1)
        {
            clanScore2 += 20;
            winner = m_Clan2->clan->getId();
            extScore2 = 20;
        }
        else
        {
            if(winner == m_Clan1->clan->getId()) clanScore1 += 20;
            else clanScore2 += 20;
        }

        m_Clan1->clan->SetBattleScore(m_Clan1->clan->GetBattleScore() + clanScore1);
        m_Clan2->clan->SetBattleScore(m_Clan2->clan->GetBattleScore() + clanScore2);

        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            if(!m_Fields[i].IsEnd()) continue;

            if(m_Fields[i].GetWinner() == m_Clan1->clan->getId())
            {
                m_Fields[i].AddProffer(extScore1 + BATTLE_FIELD_SCORE[i], extScore2);
            }
            else
            {
                m_Fields[i].AddProffer(extScore1, extScore2 + BATTLE_FIELD_SCORE[i]);
            }
        }
    }

    void ClanRankBattle::OnPlayerLeave(Player* player)
    {
        if(m_State == STATE_PREPARE) return;

        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            if(m_Fields[i].OnPlayerLeave(player)) break;
        }
    }

    ClanRankBattleInfo* ClanRankBattle::GetClan(UInt32 id)
    {
        if(m_Clan1->clan->getId() == id) return m_Clan1;
        else if(m_Clan2->clan->getId() == id) return m_Clan2;
        return NULL;
    }

    ClanRankBattleInfo* ClanRankBattle::GetOtherClan(UInt32 id)
    {
        if(m_Clan1->clan->getId() == id) return m_Clan2;
        else if(m_Clan2->clan->getId() == id) return m_Clan1;
        return NULL;
    }

    void ClanRankBattle::SendBattleStatus(Player* player)
    {
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(0);
        stream << UInt8(m_State);
        if(m_State == STATE_PREPARE)
        {
            stream << m_StartTime + RANK_BATTLE_PREPARE_TIME;
        }
        else
        {
            stream << m_StartTime + FULL_BATTLE_TIME;
        }
        stream << Stream::eos;
        player->send(stream);
    }

    void ClanRankBattle::SendBattleInfo(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(5);
        
        ClanRankBattleInfo* myClan = NULL;
        ClanRankBattleInfo* otherClan = NULL;
        if(player->getClan() == m_Clan1->clan)
        {
            myClan = m_Clan1;
            otherClan = m_Clan2;
        }
        else
        {
            myClan = m_Clan2;
            otherClan = m_Clan1;
        }

        stream << otherClan->clan->getName();
        stream << myClan->clan->GetBattleScore();
        stream << otherClan->clan->GetBattleScore();

        UInt32 num1 = 0;
        UInt32 num2 = 0;
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            num1 += m_Fields[i].GetPlayerNum(myClan->clan->getId());
            num2 += m_Fields[i].GetPlayerNum(otherClan->clan->getId());
        }

        stream << UInt8(num1);    
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            m_Fields[i].FillPlayers(stream, myClan->clan->getId());
        }

        stream << UInt8(num2);
        for(UInt32 i = 0; i < RANK_BATTLE_FIELD_NUM; ++i)
        {
            m_Fields[i].FillPlayers(stream, otherClan->clan->getId());
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
            m_SignupCountDown = (m_StartTime + RANK_BATTLE_SIGNUP_TIME - m_Now) / 60;
        }
        else if(m_Now < m_StartTime + RANK_BATTLE_SIGNUP_TIME + FULL_BATTLE_TIME * 3) //战斗时间
        {
            GetCanBattleClans(false);

            m_State = STATE_BATTLE;
            m_BattleNo = (m_Now - m_StartTime - RANK_BATTLE_SIGNUP_TIME - 1) / FULL_BATTLE_TIME + 1; 
        }
        else
        {
            m_State = STATE_INIT;
            m_StartTime = m_StartTime + 24 * 60 * 60; //第二天战斗开始时间
        }

        SortClans();
    }

    void ClanRankBattleMgr::UnInit()
    {
        for(BattleVec::iterator iter = m_Battles.begin();
                iter != m_Battles.end(); ++iter)
        {
            delete (*iter);
        }
        m_Battles.clear();
        m_ClanBattles.clear();
    }

    void ClanRankBattleMgr::Process(UInt32 now)
    {
        switch(m_State)
        {
            case STATE_INIT:
                {
                    ProcessInit(now);
                }
                break;
            case STATE_SIGNUP:
                {
                    ProcessSignup(now);
                }
                break;
            case STATE_BATTLE:
                {
                    ProcessBattle(now);
                }
                break;
        }
        
        m_Now = now;
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
            
            info->RemovePlayer(player);
            ClanRankBattle* battle = GetClanBattle(clan->getId());
            if(battle != NULL) battle->OnPlayerLeave(player);

            player->autoRegenAll();
            
            //更新列表
            Stream stream(REP::CLAN_RANKBATTLE_REP);
            stream << UInt8(7);
            stream << UInt8(1);
            stream << player->getId();
            stream << Stream::eos;
            info->Broadcast(stream);
        }

        Stream stream(REP::CLAN_RANKBATTLE_REPINIT);
        stream << UInt8(2) << UInt8(0) << Stream::eos;
        player->send(stream);
    }



    void ClanRankBattleMgr::PlayerEnter(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        if(player->getLocation() != RANK_BATTLE_LOCATION) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return; //帮会没参加
       
        UInt32 field = clan->GetRankBattleField(player, m_Now);
        if(field >= RANK_BATTLE_FIELD_NUM) return;

        //通知状态变化
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(9);
        stream << player->getId();
        stream << UInt8(player->getLocation() == RANK_BATTLE_LOCATION ? 1 : 0);
        stream << Stream::eos;
        player->send(stream);
    }

    void ClanRankBattleMgr::PlayerLeave(Player* player)
    {
        if(m_State != STATE_BATTLE) return;
        
        if(player->getLocation() != RANK_BATTLE_LOCATION) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

        if(!info->HasPlayer(player)) return;

        ClanRankBattle* battle = GetClanBattle(clan->getId());
        if(battle != NULL) battle->OnPlayerLeave(player);

        player->autoRegenAll();

        //通知状态变化
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(9);
        stream << player->getId();
        stream << UInt8(player->getLocation() == RANK_BATTLE_LOCATION ? 1 : 0);
        stream << Stream::eos;
        player->send(stream);
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

        if(clan->getClanRank(player) < 2) return; //没权力


        PlayerVec& players = pInfo->players[field];
        if(pos == 0 || pos > players.size()) return;

        UInt32 oldField = clan->AdjustRankBattleField(member, field, m_Now);
        if(oldField >= RANK_BATTLE_FIELD_NUM) return;

        pInfo->RemovePlayer(member);

        PlayerVec::iterator posIter = players.begin();
        while(--pos > 0) ++posIter;
        players.insert(posIter, member);

        //刷新出战顺序 
        Stream stream(REP::CLAN_RANKBATTLE_REP);
        stream << UInt8(3);
        UInt32 num = pInfo->players[oldField].size();
        if(field != oldField) num += pInfo->players[field].size();

        stream << UInt8(num);
            
        UInt32 index = 0;
        players = pInfo->players[oldField];
        for(PlayerVec::iterator iter = players.begin();
            iter != players.end(); ++iter)
        {
            Player* member = *iter;
            stream << member->getId();
            stream << UInt8(oldField);
            stream << UInt8(++index);
        }

        if(field != oldField)
        {
            index = 0;
            players = pInfo->players[field];
            for(PlayerVec::iterator iter = players.begin();
                iter != players.end(); ++iter)
            {
                Player* member = *iter;
                stream << member->getId();
                stream << UInt8(field);
                stream << UInt8(++index);
            }
        }

        stream << Stream::eos;
        pInfo->Broadcast(stream);        
        
        const static UInt32 msg[RANK_BATTLE_FIELD_NUM] = {2216, 2217, 2218, 2219, 2220, 2221};
        if(oldField != field)
        {
            SYSMSG_SEND(msg[field], member);
        }
    }

    void ClanRankBattleMgr::GetRewards(Player* player)
    {
        //TODO
    }

    void ClanRankBattleMgr::BuySkill(Player* player)
    {
        //TODO
    }

    void ClanRankBattleMgr::SendInitStatus(Player* player)
    {
        Clan* clan = player->getClan();
        if(clan == NULL) return;

        Stream stream(REP::CLAN_RANKBATTLE_REPINIT);
        stream << UInt8(0);
        stream << UInt32(clan->GetBattleScore());
        stream << UInt16(clan->GetBattleRanking());
        stream << UInt8(m_State);
        UInt32 time = 0;
        switch(m_State)
        {
            case STATE_INIT:
                time = m_StartTime;
                break;
            case STATE_SIGNUP:
                time = m_StartTime + RANK_BATTLE_SIGNUP_TIME;
                break;
            case STATE_BATTLE:
                time = m_StartTime + RANK_BATTLE_SIGNUP_TIME + 3 * FULL_BATTLE_TIME;
                break;
        }
        stream << time;
        UInt32 field = clan->GetRankBattleField(player, m_Now);
        stream << UInt8(field < RANK_BATTLE_FIELD_NUM ? 1:0);
        stream << UInt8(clan->GetSignupRankBattleNum(m_Now));
        std::string str = "";
        ClanRankBattle* battle = GetClanBattle(clan->getId());
        if(battle != NULL)
        {
            ClanRankBattleInfo* info = battle->GetOtherClan(clan->getId());
            if(info != NULL) str = info->clan->getName();
        }
        stream << str << Stream::eos;
        player->send(stream);
    }

    void ClanRankBattleMgr::SendBattleStatus(Player* player)
    {
        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattle* battle = GetClanBattle(clan->getId());
        if(battle != NULL)
        {
            battle->SendBattleStatus(player);
        }
    }

    void ClanRankBattleMgr::SendSkillList(Player* player)
    {
        //TODO
    }

    void ClanRankBattleMgr::SendSortList(Player* player, UInt16 start, UInt8 count)
    {
        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        Stream st(REP::CLAN_RANKBATTLE_SORTLIST);
        
        UInt16 total = UInt16(m_ClanRanking.size());
        st << total;
        
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
        //将帮会战状态变化同步给所有人
        Stream stream(REP::CLAN_RANKBATTLE_REPINIT);
        stream << UInt8(5);
        stream << UInt8(m_State);
        UInt32 time = 0;
        switch(m_State)
        {
            case STATE_INIT:
                time = m_StartTime;
                break;
            case STATE_SIGNUP:
                time = m_StartTime + RANK_BATTLE_SIGNUP_TIME;
                break;
            case STATE_BATTLE:
                time = m_StartTime + RANK_BATTLE_SIGNUP_TIME + 3 * FULL_BATTLE_TIME;
                break;
        }
        stream << time;
        stream << Stream::eos;
        NETWORK()->Broadcast(stream);
    }

    void ClanRankBattleMgr::SendPlayerList(Player* player)
    {
        if(m_State != STATE_BATTLE) return;

        Clan* clan = player->getClan();
        if(clan == NULL) return;

        ClanRankBattleInfo* info = GetClanInfo(clan->getId());
        if(info == NULL) return;

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

        ClanRankBattle* battle = GetClanBattle(clan->getId());
        if(battle != NULL)
        {
            battle->SendBattleInfo(player);
        }
    }

    void ClanRankBattleMgr::SendRewards(Player* player)
    {
        //TODO
    }

    void ClanRankBattleMgr::ProcessInit(UInt32 now)
    {
        if(now >= m_StartTime)
        {
            //切换到报名状态
            m_SignupCountDown = RANK_BATTLE_SIGNUP_TIME / 60;
            m_State = STATE_SIGNUP;
            SyncState();
        }
        else
        {
            //新的一周重新设置积分和排名
            if(TimeUtil::GetWeekDay(m_Now) == 7 && TimeUtil::GetWeekDay(now) == 1)
            {
                //清空原积分
                class ClearClanVisitor : public Visitor<Clan>
                {
                public:
                    ClearClanVisitor(){}
                    ~ClearClanVisitor(){}

                public:
                    bool operator()(Clan* clan)
                    {
                        clan->SetBattleScore(0);
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
                    (*iter)->SetBattleRanking(++rank);
                }
            }
        }
    }

    void ClanRankBattleMgr::ProcessSignup(UInt32 now)
    {
        if(m_SignupCountDown > 0)
        {
            //报名倒计时
            if(now >= m_StartTime + RANK_BATTLE_SIGNUP_TIME - m_SignupCountDown * 60
                    && (m_SignupCountDown == 5 || m_SignupCountDown == 1))
            {
                SYSMSG_BROADCASTV( 2210, m_SignupCountDown);
                --m_SignupCountDown;
            }
        }
        else if(now >= m_StartTime + RANK_BATTLE_SIGNUP_TIME) //战斗开始
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

    void ClanRankBattleMgr::ProcessBattle(UInt32 now)
    {
        if(now >= m_StartTime + RANK_BATTLE_SIGNUP_TIME + m_BattleNo * FULL_BATTLE_TIME)
        {
            EndOneBattle();

            if(++m_BattleNo > 3) //今天的比赛结束了
            {
                //第二天战斗开始时间
                m_Clans.clear();
                m_StartTime = TimeUtil::SharpDayT(1, now) + RANK_BATTLE_SIGNUP_BEGINTIME;
                m_State = STATE_INIT;
                SyncState();

                //每周最后一场结束发邮件
                if(TimeUtil::GetWeekDay(now) == 7)
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
                                member->player->GetMailBox()->newMail(NULL, 0x21, m_Title, m_Content, 0xFFFE0000);
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
                    for(++iter; iter != m_ClanRanking.end(); ++iter)
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
            (*iter)->Process(now);
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
            //轮空直接获得40分
            SetNoTurns(group[0]);
        }
        else
        {
            if(!CreateBattle(group[0],group[1])) return;
            
            if(group.size() > 2)
            {
                SetNoTurns(group[2]);
            }
        }
    }

    void ClanRankBattleMgr::EndOneBattle()
    {
        for(BattleVec::iterator iter = m_Battles.begin();
                iter != m_Battles.end(); ++iter)
        {
            (*iter)->End();
            delete *iter;
        }
        m_Battles.clear();
        m_ClanBattles.clear();

        SortClans();
    }

    bool ClanRankBattleMgr::CreateBattle(ClanRankBattleInfo* clan1, ClanRankBattleInfo* clan2)
    {
        ClanRankBattle* battle = new(std::nothrow) ClanRankBattle(clan1, clan2);
        if(battle == NULL) return false;
        battle->Start(m_Now);
        m_Battles.push_back(battle);
        m_ClanBattles.insert(std::make_pair(clan1->clan->getId(),battle));
        m_ClanBattles.insert(std::make_pair(clan2->clan->getId(),battle));

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

    ClanRankBattle* ClanRankBattleMgr::GetClanBattle(UInt32 id)
    {
        BattleMap::iterator iter = m_ClanBattles.find(id);
        if(iter != m_ClanBattles.end())
        {
            return iter->second;
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
        GetRankBattleClansVisitor visitor(m_Clans, TimeUtil::Now(), bNotify);
        globalClans.enumerate(visitor);
    }

    struct ClanRankBattleSorter
    {
        bool operator()(Clan* clan1, Clan* clan2) const
        {
            return clan1->GetBattleScore() > clan2->GetBattleScore();
        }
    };   
    void ClanRankBattleMgr::SortClans()
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
    }

    void ClanRankBattleMgr::SetNoTurns(ClanRankBattleInfo* clan)
    {
        clan->clan->SetBattleScore(clan->clan->GetBattleScore() + 40);
        SysMsgItem *sysMsgItem = globalSysMsg[2233];
        if(sysMsgItem != NULL)
        {
            Stream stream;
            sysMsgItem->getvap(&stream, 40);
            clan->clan->broadcast(stream);
        }
    }
}



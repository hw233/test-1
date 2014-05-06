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

#include "RaceBattle.h"

#define RACEBATTLE_STARTTIME 20*3600
#define RACEBATTLE_ENDTIME RACEBATTLE_STARTTIME+1800

namespace GObject
{
    static UInt8 gPerLeveCnt[] = {2, 3, 4, 5, 7, 0};
    UInt8 RaceBattle::_status = 0;
    RBSortType RaceBattle::_levelStarSort[5];
    RBSortType RaceBattle::_contineWinSort;

    void raceBattleBroadcast(UInt8 type, UInt32 lefttime)
    {
        Stream st(REP::RACE_BATTLE);
        st << type;
        st << lefttime;
        st << Stream::eos;
        NETWORK()->Broadcast(st);
    }

    void RaceBattle::raceBattleCheck(UInt32 time)
    {
        UInt32 curTime = TimeUtil::SharpDay(0, time);

        if(curTime < RACEBATTLE_STARTTIME - 600)
            _status = 0;
        else if(curTime < RACEBATTLE_STARTTIME)
        {
            if(_status == 1)
                return;
            _status = 1;
            raceBattleBroadcast(0, RACEBATTLE_STARTTIME - curTime);
        }
        else if(curTime < RACEBATTLE_ENDTIME)
        {
            if(_status == 2)
                return;
            _status = 2;
            raceBattleBroadcast(1, RACEBATTLE_ENDTIME - curTime);
        }
        else
        {
            if(_status == 2)
            {
                raceBattleBroadcast(1, 0);
                awardLevelRank();
                //awardContinueWinRank();
            }
            _status = 0;
        }
    }

    void RaceBattle::enterPos(Player* pl, UInt8 offset)
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

        if(level == sizeof(gPerLeveCnt) / sizeof(gPerLeveCnt[0]) && !pl->getIsLastLevel())
        {
            pl->setIsLastLevel(true);
            UInt32 rank = World::getRBTimeRank();
            ++rank;
            if(rank <= 3)
                SYSMSG_BROADCASTV(6004, rank, pl->getCountry(), pl->getPName());
            World::setRBTimeRank(rank);
        }

        pl->setRaceBattlePos(pos);
        //if(pl->GetVar(VAR_RCAE_BATTLE_SIGN) == 0)
        //    pl->SetVar(VAR_RCAE_BATTLE_SIGN, 1);

        if(offset == 0)
        {
            //if((origPos % 10) != 0)
            //    return;
            //    eraseLevelStarSort(pl, level);
        }
        else if((origPos % 10) == 0)
        {
            insertLevelStarSort(pl, level);
        }
        else
        {
        }


        UInt8 page = pl->getContinueWinPage();
        if(page == 0)
        {
            page = 1;
            pl->setContinueWinPage(page);
        }
        sendContinueWinSort(pl, page);
        sendOwnerInfo(pl);
        sendBattleInfo(pl);
    }

    void RaceBattle::autoBattle(Player* pl)
    {
    }

    void RaceBattle::cancelBattle(Player* pl)
    {
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
        if(level < 3 || level > 6 || awardlevel > level)
            return;
        UInt8 index = level - 3;
        if(index > 3)
            return;

        static UInt32 awardItem[][3][2] = {
            {{499, 20}, {499, 10}, {499, 5}},
            {{499, 20}, {499, 10}, {499, 5}},
            {{499, 20}, {499, 10}, {499, 5}},
            {{499, 20}, {499, 10}, {499, 5}}
        };

        for(UInt8 i = 0; i < 3; i++)
           pl->GetPackage()->Add(awardItem[index][i][0], awardItem[index][i][1], true, false);

        ++awardlevel;
        pl->setAwardLevel(awardlevel);

        Stream st(REP::RACE_BATTLE);
        UInt8 type = 8;
        st << type;
        st << awardlevel;
        st << Stream::eos;
        pl->send(st);
    }
#if 0
    void RaceBattle::readBattleReport(Player* pl, UInt32 reportId)
    {
        if(!pl)
            return;
        pl->readRandBattleReport(reportId);
    }
#endif
    bool RaceBattle::requestMatch(Player* pl)
    {
        if(!pl)
            return false;
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
        if(level > sizeof(gPerLeveCnt) / sizeof(gPerLeveCnt[0]))
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
        if(pl->getStarCnt(offset - 1) >= 6 && !pl->getIsLastLevel())
        {
            pl->sendMsgCode(0, 4043);
            return false;
        }

        std::vector<Player* > vecPlayer;
        UInt32 count = 0;
        for(UInt8 i = level; i <= 5; i++)
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
        return true;
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

        Int32 rank = 0;
        Int32 rankAdd = 0;
        RBSortType& starSort = _levelStarSort[level - 1];
        //UInt32 sortSize = starSort.size();
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
        #define PAGE_MAX 5
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
        if(pl->getExitCd() > TimeUtil::Now())
        {
            pl->sendMsgCode(0, 4040);
            return 0;
        }
        if(pl->getBuffData(PLAYER_BUFF_ATTACKING))
        {
            pl->sendMsgCode(0, 1407);
            return 0;
        }
		pl->setBuffData(PLAYER_BUFF_ATTACKING, TimeUtil::Now() + 3);

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

        return res ? 1 : 2;
    }

    void RaceBattle::awardLevelRank()
    {
        UInt8 rank = 0;
        for(UInt8 i = 5; i >= 1; i--)
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
    }

    void RaceBattle::awardContinueWinRank()
    {
        for(RBSortType::iterator it = _contineWinSort.begin(); it != _contineWinSort.end(); ++it)
        {
            if(it->total < 5)
                return;
            awardContinueWinRankOne(it->player, it->total);
        }
    }

    void RaceBattle::awardLevelRankOne(Player* pl, UInt8 rank)
    {
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

        SYSMSG(title, 5135);
        SYSMSGV(content, 5136, rank);
        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            static MailPackage::MailItem mitem[][3] = {
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}}
            };
            MailItemsInfo itemsInfo(mitem[type], RandBattleAward, 3);
            mailPackageManager.push(mail->id, mitem[type], 3, true);
            std::string strItems;
            for (int i = 0; i < 3; ++i)
            {
                strItems += Itoa(mitem[type][i].id);
                strItems += ",";
                strItems += Itoa(mitem[type][i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        }
    }

    void RaceBattle::awardContinueWinRankOne(Player* pl, UInt8 num)
    {
        if(!pl)
            return;

        UInt8 type = 0;
        if(type == 50)
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

        SYSMSG(title, 5137);
        SYSMSGV(content, 5138, num);
        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail)
        {
            static MailPackage::MailItem mitem[][3] = {
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}},
                {{503, 2}, {515, 2}, {25, 1}}
            };
            MailItemsInfo itemsInfo(mitem[type], RandBattleAward, 3);
            mailPackageManager.push(mail->id, mitem[type], 3, true);
            std::string strItems;
            for (int i = 0; i < 3; ++i)
            {
                strItems += Itoa(mitem[type][i].id);
                strItems += ",";
                strItems += Itoa(mitem[type][i].count);
                strItems += "|";
            }
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, Activity, title, content, strItems.c_str(), mail->recvTime);
        }
    }

    void RaceBattle::eraseLevelStarSort(Player* pl, UInt8 level)
    {
        if(!pl)
            return;
        if(level == 0 || level > 5)
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
        if(level == 0 || level > 5)
            return;
        //if(pl->getStarTotal() < 1)
        //    return;

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
        pl->setContinueWinCnt(0);
        eraseLevelStarSort(pl, pl->getRaceBattlePos() / 10);
    }

    void RaceBattle::attackLevelPlayer(Player* pl, UInt64 defenderId)
    {
        if(!pl)
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
        Player* defender = globalPlayers[defenderId];
        if(!defender)
            return;
        if(pl->getStarCnt(offset - 1) >= 6 && !pl->getIsLastLevel())
        {
            pl->sendMsgCode(0, 4043);
            return;
        }

        UInt8 res = attackPlayer(pl, defender);
        if(res == 0)
            return;

        UInt32 now = TimeUtil::Now();
        if(pl->getAttackCd() > now)
        {
            pl->sendMsgCode(0, 4045);
            return;
        }
        pl->setAttackCd(now + 20);

        if(pl->getIsLastLevel())
            return;

        UInt8 starAdd;
        if(res == 1)
        {
            starAdd = 2;
            eraseContinueWinSort(pl);
            pl->setContinueWinCnt(pl->getContinueWinCnt() + 1);
            awardContinueWinRankOne(pl, pl->getContinueWinCnt());
            insertContinueWinSort(pl);
            pl->setContinueLoseCnt(0);
        }
        else
        {
            starAdd = 1;
            eraseContinueWinSort(pl);
            pl->setContinueWinCnt(0);
            pl->setContinueLoseCnt(pl->getContinueLoseCnt() + 1);
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
            enterPos(pl, 0);
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
    }

    void RaceBattle::attackContinueWinPlayer(Player* pl, UInt64 defenderId)
    {
        if(!pl)
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
        for(it = _contineWinSort.begin(); it != _contineWinSort.end(); ++it)
        {
            if(it->player == defender)
                break;
        }
        if(it == _contineWinSort.end())
            return;

        UInt8 res = attackPlayer(pl, defender);
        if(res == 0)
            return;

        UInt32 now = TimeUtil::Now();
        if(pl->getAttackCd() > now)
        {
            pl->sendMsgCode(0, 4045);
            return;
        }
        pl->setAttackCd(now + 20);

        if(pl->getIsLastLevel())
            return;

        UInt8 starAdd = 1;
        if(res == 1)
        {
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
        if(rb && starCnt >= rb->next * 2)
        {
            level = level + 1;
            for(UInt8 i = 0; i < gPerLeveCnt[level - 1]; i++)
                pl->setStarCnt(i, 0);
            UInt8 pos = level * 10;
            pl->setRaceBattlePos(pos);
            pl->setContinueLoseCnt(0);
            enterPos(pl, 0);
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
        if(level == 0 || level > 5)
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
        //st << pl->getCanChallengeCnt();
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
}


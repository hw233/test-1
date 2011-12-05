#include "TeamCopy.h"
#include "Player.h"
#include "Common/Stream.h"
#include "MsgID.h"


namespace GObject
{

TeamCopy* teamCopyManager = new TeamCopy();

const UInt8 TeamCopy::lvls[6] = {30, 45, 60, 70, 80, 90};

TeamCopy::TeamCopy()
{
    for(int t = 0; t < TEAMCOPY_MAXTYPECNT; ++t)
    {
        for(int i = 0; i < TEAMCOPY_MAXCOPYCNT; ++i)
        {
            for(UInt16 j = 0; j < TEAMCOPY_MAXTEAMCNT; ++j)
            {
                m_tnIdle[t][i].push_back(j + 1);
            }
            m_copysTeam[t][i] = new CopyTeams();
        }
    }
}

TeamCopy::~TeamCopy()
{
    for(UInt8 t = 0; t < TEAMCOPY_MAXTYPECNT; ++t)
    {
        for(UInt8 idx = 0; idx < TEAMCOPY_MAXCOPYCNT; ++idx)
        {
            m_clvTeamCnt[t][idx].clear();
            m_tnIdle[t][idx].clear();
            m_playerIdle[t][idx].clear();
        }

        for(UInt8 idx = 0; idx < TEAMCOPY_MAXCOPYCNT; ++idx)
        {
            if(m_copysTeam[t][idx])
            {
                CopyTeams& ct = *(m_copysTeam[t][idx]);
                UInt32 cnt = ct.size();
                for(UInt32 idx2 = 0; idx2 < cnt; ++idx2)
                {
                    if(NULL == ct[idx2])
                        continue;
                    delete ct[idx2];
                    ct[idx2] = NULL;
                }
                ct.clear();
                delete m_copysTeam[t][idx];
            }
        }

    }

}

bool TeamCopy::checkTeamCopy(Player* pl, UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    if(pl == NULL)
        return false;
    UInt8 level = pl->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return false;

    return true;
}

void TeamCopy::addTeamCopyNpc(UInt8 copyId, UInt8 t, UInt16 location, UInt32 npcId)
{
    UInt8 copyIdx = copyId - 1;
    if( copyId > TEAMCOPY_MAXCOPYCNT || t > TEAMCOPY_MAXTYPECNT)
        return;

    TeamCopyNpc& tcNpc = m_tcNpcId[t][copyIdx];
    tcNpc.location = location;
    tcNpc.npcId.push_back(npcId);
}

void TeamCopy::sendTeamCopyPageUpdate(UInt8 copyId, UInt8 t, UInt32 startIdx, UInt32 endIdx)
{
    UInt8 copyIdx = copyId - 1;
    TeamCopyPlayer& tcp = m_playerIdle[t][copyIdx];
    UInt32 cnt = tcp.size();
    for(UInt32 idx = 0; idx < cnt; ++idx)
    {
        Player* pl = tcp[idx];
        if(NULL == pl)
            continue;

        CopyTeamPage& ctp = pl->getCopyTeamPage();
        if(ctp.start <= startIdx && ctp.end >= endIdx)
        {
            reqTeamList(pl, ctp.start, TEAMCOPY_MAXPAGECNT, ctp.type);
        }
    }
}

void TeamCopy::reqTeamList(Player* pl, UInt32 start, UInt8 count, UInt8 type)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;
    UInt8 level = pl->GetLev();
    if(copyIdx >= TEAMCOPY_MAXCOPYCNT)
        return;

    if(!checkTeamCopy(pl, copyId, t))
        return;
    if(m_copysTeam[t][copyIdx] == NULL)
        return;

    CopyTeams& ct = *(m_copysTeam[t][copyIdx]);

    UInt32 sz = ct.size();
    if(start > sz)
    {
        start = sz > count ? sz - count : 0;
    }
    UInt32 end = start + count;
    if(end > sz)
    {
        end = sz;
        count = end - start;
    }
    if(end < start)
        count = 0;

    UInt32 sz2 = sz;
    if(type == 1)
    {
        LevelTeamCnt& lvt = m_clvTeamCnt[t][copyIdx];
        LevelTeamCntIterator lvIt = lvt.find(level);
        if(lvIt == lvt.end())
        {
            pl->sendMsgCode(1, 2100);
            return;
        }
        sz2 = lvIt->second;
    }

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x01) << sz2 << start << type << count;
    UInt8 pos = st.size() - 1;
    UInt8 cnt = 0;
    UInt32 idx = start;
    for(; idx < sz && cnt < count; ++idx)
    {
        if(type == 1 && ((ct[idx]->upLevel > 0 && ct[idx]->upLevel < level) || (ct[idx]->dnLevel > 0 && ct[idx]->dnLevel > level) || ct[idx]->pwd.length() > 0 || ct[idx]->count >= TEAMCOPY_MAXMEMCNT ))
        {
            continue;
        }
        ++cnt;

        st << ct[idx]->id << ct[idx]->leader->getId() << ct[idx]->dnLevel << ct[idx]->upLevel << static_cast<UInt8>(ct[idx]->pwd.length() > 0 ? 1 : 0);
        st << ct[idx]->count;
        for(int i = 0; i < ct[idx]->count; ++i)
        {
            Player* mem = ct[idx]->members[i];
            Fighter* fgt =  mem->getMainFighter();
            st << mem->getId() << mem->getName() << fgt->getLevel() << fgt->getColor() << static_cast<UInt16>(fgt->getId());
        }
    }

    ctp.start = start;
    ctp.end = idx > (start + TEAMCOPY_MAXPAGECNT) ? idx : (start + TEAMCOPY_MAXPAGECNT);
    ctp.type = type;

    st.data<UInt8>(pos) = cnt;
    st << Stream::eos;
    pl->send(st);
}

void TeamCopy::updateTeamInfo(Player* pl)
{
    TeamData* td = pl->getTeamData();
    if(!td)
        return;

    for(UInt8 j = 0; j < td->count; ++j)
    {
        reqTeamInfo(td->members[j]);
    }
}

void TeamCopy::reqTeamInfo(Player* pl)
{
    TeamData* td = pl->getTeamData();
    if(td == NULL)
        return;
    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x10);
    st << td->id << td->leader->getId() << td->count;

    for(UInt8 idx = 0; idx < td->count; ++idx)
    {
        st << td->members[idx]->getId() << td->members[idx]->getName();
        Fighter* mainfgt = td->members[idx]->getMainFighter();
        st << mainfgt->getLevel() << mainfgt->getColor() << static_cast<UInt16>(mainfgt->getId()) << td->members[idx]->getFormation();
        UInt8 pos = st.size();
        UInt8 cnt = 0;
        st << cnt;
		for(int i = 0; i < 5; ++ i)
		{
			GObject::Fighter * fgt = td->members[idx]->getLineup(i).fighter;
			if(fgt != NULL && fgt != mainfgt)
            {
                st << fgt->getId() << fgt->getLevel() << fgt->getColor();
                ++cnt;
            }
		}
        st.data<UInt8>(pos) = cnt;
    }

    st << Stream::eos;
    pl->send(st);
}

UInt16 TeamCopy::getIdleTeamNumber(UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    if(m_tnIdle[t][copyIdx].size() == 0)
        return 0;

    UInt16 number = m_tnIdle[t][copyIdx][0];
    m_tnIdle[t][copyIdx].erase(m_tnIdle[t][copyIdx].begin());

    return number;
}

void TeamCopy::recycleTeamNumber(UInt8 copyId, UInt8 t, UInt16 number)
{
    UInt8 copyIdx = copyId - 1;
    m_tnIdle[t][copyIdx].push_back(number);
}

UInt32 TeamCopy::createTeam(Player* pl, std::string pwd, UInt8 upLevel, UInt8 dnLevel)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;

    if(!checkTeamCopy(pl, copyId, t))
        return 0;
    if(pl->getTeamData() != NULL)
        return 0;
    if(m_copysTeam[t][copyIdx] == NULL)
        return 0;

    UInt16 number = getIdleTeamNumber(copyId, t);
    if(number == 0)
    {
		pl->sendMsgCode(1, 2101);
        return 0;
    }

    CopyTeams& ct = *(m_copysTeam[t][copyIdx]);
    TeamData* td = new TeamData();

    td->pwd = pwd;
    td->id = copyId + (number << 8);
    td->upLevel = upLevel;
    td->upLevel = dnLevel;
    td->count = 1;
    td->t = t;
    td->leader = pl;
    td->members[0] = pl;
    ct.push_back(td);
    pl->setTeamData(td);

    m_allCopyTeams[td->id] = td;

    dnLevel = dnLevel > lvls[copyIdx] ? dnLevel : lvls[copyIdx];
    upLevel = upLevel < LEVEL_MAX ? upLevel : LEVEL_MAX;
    incLevelTeamCnt(copyId, t, upLevel, dnLevel);

    sendTeamCopyPageUpdate(copyId, t, ctp.start, ctp.end);

    return td->id;
}

void TeamCopy::incLevelTeamCnt(UInt8 copyId, UInt8 t, UInt8 upLevel, UInt8 dnLevel)
{
    UInt8 copyIdx = copyId - 1;
    if(!dnLevel)
        dnLevel = lvls[copyIdx];
    if(!upLevel)
        upLevel = LEVEL_MAX;
    for(UInt8 i = dnLevel; i <= upLevel; ++i)
    {
        LevelTeamCnt& lvt = m_clvTeamCnt[t][copyIdx];
        LevelTeamCntIterator lvIt = lvt.find(i);
        if(lvIt == lvt.end())
            lvt[i] = 1;
        else
            ++lvt[i];
    }
}

void TeamCopy::decLevelTeamCnt(UInt8 copyId, UInt8 t, UInt8 upLevel, UInt8 dnLevel)
{
    UInt8 copyIdx = copyId - 1;
    if(!dnLevel)
        dnLevel = lvls[copyIdx];
    if(!upLevel)
        upLevel = LEVEL_MAX;
    for(UInt8 i = dnLevel; i <= upLevel; ++i)
    {
        LevelTeamCnt& lvt = m_clvTeamCnt[t][copyIdx];
        LevelTeamCntIterator lvIt = lvt.find(i);
        if(lvIt == lvt.end())
            continue;
        --lvt[i];
    }
}

bool TeamCopy::quikJoinTeam(Player* pl)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;

    if(pl->getTeamData() != NULL)
        return false;
    if(!checkTeamCopy(pl, copyId, t))
        return false;

    CopyTeams& ct = *(m_copysTeam[t][copyIdx]);
    UInt32 sz = ct.size();

    bool res = false;
    for(UInt32 idx = 0; idx < sz; ++idx)
    {
        if(ct[idx]->count < TEAMCOPY_MAXMEMCNT && ct[idx]->pwd.length() == 0)
        {
            res = true;
            joinTeam(pl, ct[idx]->id, "");
            break;
        }
    }

    return res;
}

UInt32 TeamCopy::joinTeam(Player* pl, UInt32 teamId, std::string pwd)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 t = ctp.t;

    if(pl->getTeamData() != NULL)
        return 0;

    if(!checkTeamCopy(pl, copyId, t))
        return 0;
    
    AllCopyTeamsIterator it = m_allCopyTeams.find(teamId);
    if(it == m_allCopyTeams.end() || it->second == NULL)
    {
		pl->sendMsgCode(1, 2102);
        return 0;
    }

    TeamData* td = it->second;
    if(td->count >= TEAMCOPY_MAXMEMCNT || t != td->t)
    {
		pl->sendMsgCode(1, 2103);
        return 0;
    }

    if(td->pwd != pwd)
    {
		pl->sendMsgCode(1, 2104);
        return 0;
    }

    pl->setTeamData(td);
    td->members[td->count] = pl;
    ++td->count;

    sendTeamCopyPageUpdate(copyId, t, ctp.start, ctp.end);
    // send team info to members ::TODO
    for(UInt8 j = 0; j < td->count; ++j)
    {
        reqTeamInfo(td->members[j]);
    }

    return teamId;
}

void TeamCopy::leaveTeam(Player* pl)
{
    TeamData* td = pl->getTeamData();
    if(td == NULL)
        return;

    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;
    if(!checkTeamCopy(pl, copyId, t))
        return;

    UInt8 res = 0;
    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x04);
    for(UInt8 i = 0; i < td->count; ++i)
    {
        if(td->members[i] == pl)
        {
            res = 1;
            td->members[i] = NULL;
            UInt8 j = i+1;
            for(; j < td->count; ++j)
            {
                td->members[j-1] = td->members[j];
            }
            td->members[j-1] = NULL;
            --td->count;
            break;
        }
    }
    if(res == 0)
    {
		pl->sendMsgCode(1, 2105);
        return;
    }

    pl->setTeamData(NULL);
    // send team info to members ::TODO
    if(td->count == 0)
    {
        m_allCopyTeams.erase(td->id);

        CopyTeams& ct = *(m_copysTeam[t][copyIdx]);
        for(UInt32 i = 0; i < ct.size(); ++i)
        {
            if (td == ct[i])
            {
                ct.erase(ct.begin() + i);
                break;
            }
        }

        UInt8 dnLevel = td->dnLevel > lvls[copyIdx] ? td->dnLevel : lvls[copyIdx];
        UInt8 upLevel = td->upLevel < LEVEL_MAX ? td->upLevel : LEVEL_MAX;
        decLevelTeamCnt(copyId, t, upLevel, dnLevel);
        recycleTeamNumber(copyId, t, td->id >> 8);
    }
    else if(pl == td->leader)
    {
        td->leader = td->members[0];
    }

    sendTeamCopyPageUpdate(copyId, t, ctp.start, ctp.end);

    st << td->leader->getId() << pl->getId();
    st << Stream::eos;
    // send team info to members ::TODO
    for(UInt8 j = 0; j < td->count; ++j)
    {
        reqTeamInfo(td->members[j]);
    }

    pl->send(st);
}

void TeamCopy::teamKick(Player* pl, UInt64 playerId)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 t = ctp.t;
    if(!checkTeamCopy(pl, copyId, t))
        return;

    TeamData* td = pl->getTeamData();
    if(td == NULL)
        return;

    Player* member = GObject::globalPlayers[playerId];
    if(NULL == member || pl == member)
        return;

    if(td->leader != pl)
        return;

    UInt8 res = 0;
    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x12);
    for(UInt8 i = 0; i < td->count; ++i)
    {
        if(td->members[i] == member)
        {
            res = 1;
            member->setTeamData(NULL);
            td->members[i] = NULL;
            for(UInt8 j = i+1; j < td->count; ++j)
            {
                td->members[j-1] = td->members[j];
            }
            --td->count;
            break;
        }
    }

    sendTeamCopyPageUpdate(copyId, t, ctp.start, ctp.end);

    st << res;
    st << Stream::eos;
    for(UInt8 i = 0; i < td->count; ++i)
    {
        td->members[i]->send(st);
    }

    member->send(st);
}

void TeamCopy::reQueueTeam(Player* pl, UInt8 idx0, UInt8 idx1, UInt8 idx2)
{
    TeamData* td = pl->getTeamData();
    if(td == NULL)
        return;

    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 t = ctp.t;
    if(!checkTeamCopy(pl, copyId, t))
        return;

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x13);
    if(idx0 != 0 && td->members[idx0] != NULL)
    {
        Player* ptmp = NULL;
        ptmp = td->members[0];
        td->members[0] = td->members[idx0];
        td->members[idx0] = ptmp;
    }
    if(idx1 != 1 && idx1 != 0 && td->members[idx1] != NULL)
    {
        Player* ptmp = NULL;
        ptmp = td->members[1];
        td->members[1] = td->members[idx1];
        td->members[idx1] = ptmp;
    }

    st << idx0 << idx1 << idx2;
    st << Stream::eos;
    for(UInt8 i = 0; i < td->count; ++i)
    {
        td->members[i]->send(st);
    }
}

void TeamCopy::handoverLeader(Player* pl, UInt64 playerId)
{
    TeamData* td = pl->getTeamData();
    if(td == NULL)
        return;

    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 t = ctp.t;
    if(!checkTeamCopy(pl, copyId, t))
        return;

    if(td->leader != pl)
        return;

    Player* member = GObject::globalPlayers[playerId];
    if(NULL == member ||  pl == member)
        return;

    td->leader = member;

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x11) << td->leader->getId();

    st << Stream::eos;
    for(UInt8 i = 0; i < td->count; ++i)
    {
        td->members[i]->send(st);
    }
}

bool TeamCopy::enterTeamCopy(Player* pl, UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    if(!checkTeamCopy(pl, copyId, t))
        return false;
    if(pl->hasFlag(GObject::Player::InCopyTeam))
    {
		pl->sendMsgCode(1, 2106);
        return false;
    }

    TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
    if(!tcpInfo->checkTeamCopyPlayer(copyId, t))
    {
		pl->sendMsgCode(1, 2108);
        return false;
    }

    TeamCopyNpc& npcIds = m_tcNpcId[t][copyIdx];
    if(npcIds.location != PLAYER_DATA(pl, location))
        return false;

    pl->addFlag(GObject::Player::InCopyTeam);
    m_playerIdle[t][copyIdx].push_back(pl);

    CopyTeamPage& ctp = pl->getCopyTeamPage();
    ctp.copyId = copyId;
    ctp.t = t;

    return true;
}

bool TeamCopy::leaveTeamCopy(Player* pl)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;
    if(!checkTeamCopy(pl, copyId, t))
        return false;
    if(!pl->hasFlag(GObject::Player::InCopyTeam))
    {
		pl->sendMsgCode(1, 2107);
        return false;
    }

    pl->delFlag(GObject::Player::InCopyTeam);
    for(UInt16 idx = 0; idx < m_playerIdle[t][copyIdx].size(); ++idx)
    {
        if(m_playerIdle[t][copyIdx][idx] == pl)
        {
            m_playerIdle[t][copyIdx].erase(m_playerIdle[t][copyIdx].begin() + idx);
            break;
        }
    }

    TeamData* td = pl->getTeamData();
    if(td != NULL)
    {
        leaveTeam(pl);
    }

    pl->clearCopyTeamPage();

    return true;
}

void TeamCopy::teamBattleStart(Player* pl)
{
    TeamData* td = pl->getTeamData();
    if(td == NULL)
        return;

    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;
    if(!checkTeamCopy(pl, copyId, t))
        return;

    if(td->leader != pl)
        return;

    Player* member = td->members[0];
    if(!member)
        return;

    TeamCopyNpc& npcIds = m_tcNpcId[t][copyIdx];
    TeamCopyNpcId& npcId = npcIds.npcId;
    UInt8 cnt = npcId.size();

    std::vector<GData::NpcGroup*> ngs;
    for(UInt8 i = 0; i < cnt; ++i)
    {
        GData::NpcGroups::iterator it = GData::npcGroups.find(npcId[i]);
        if(it == GData::npcGroups.end())
            return;
        GData::NpcGroup * ng = it->second;
        if (!ng)
            return;

        ngs.push_back(ng);
    }
    GData::NpcGroup * ng = ngs[0];

    Battle::BattleSimulator bsim(npcIds.location, member, ng->getName(), ng->getLevel(), false);

    UInt8 npcIdx = 0;
    UInt8 memIdx = 0;
    UInt8 res = 0;
    UInt32 rptid = 0;
    for(; memIdx < td->count; ++memIdx)
    {
        if(npcIdx >= cnt)
            break;

        member = td->members[memIdx];
        if(!member)
            break;

        if(res == 2)
        {
            sendBattleReport(td, NULL, bsim, rptid);
        }

        member->PutFighters( bsim, 0 );
        for(UInt8 mIdx = 1; mIdx <= td->count; ++mIdx)
        {
            UInt8 idx = mIdx % td->count;
            if(idx == 0)
                break;
            Fighter* fgt = NULL;
            fgt = td->members[idx]->getMainFighter();
            bsim.putTeams(td->members[idx]->getName(), fgt->getLevel(), fgt->getId(), 0);
        }

        while(npcIdx < cnt)
        {
            if( 1 == res )
            {
                sendBattleReport(td, NULL, bsim, rptid);
            }

            if( 2 != res )
            {
                ng = ngs[npcIdx];
                if(!ng)
                    break;
                ng->putFighters( bsim );

                for(UInt8 nIdx = 1; nIdx <= cnt; ++nIdx)
                {
                    UInt8 idx = nIdx % cnt;
                    if(idx == 0)
                        break;

                    bsim.putTeams(ngs[idx]->getName(), ngs[idx]->getLevel(), ngs[idx]->getPortrait(), 1);
                }
            }

            bsim.start();
            res = bsim.getWinner();
            if( 2 == res )
            {
                break;
            }
            ++npcIdx;
        }
    }

    sendBattleReport(td, ng, bsim, rptid);

    if( 1 == res )
    {
        Stream st(REP::TEAM_COPY_REQ);
        st << static_cast<UInt8>(0x00);
        st << static_cast<UInt8>(0) << static_cast<UInt8>(t);
        st << Stream::eos;

        for(UInt8 i = td->count; i > 0; --i)
        {
            Player* pl = td->members[i-1];
            if(pl == NULL)
                continue;

            pl->send(st);
            TeamCopyPlayerInfo* tcpInfo = pl->getTeamCopyPlayerInfo();
            tcpInfo->incPass(copyId, t);
            if(t == 0 && tcpInfo->getPass(copyId, 1) == false)
            {
                tcpInfo->setPass(copyId, 1, true);
            }

            leaveTeamCopy(pl);
        }
    }


    recycleTeamNumber(copyId, t, td->id >> 8);
}

void TeamCopy::sendBattleReport(TeamData* td, GData::NpcGroup* ng, Battle::BattleSimulator& bsim, UInt32& rptid)
{
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return;

    bool isLast = (ng != NULL);
    UInt8 side = 0;
    UInt8 res = bsim.getWinner();
    UInt16 r = 0;
    if(res == 1)
    {
        side = 1;
        r = static_cast<UInt16>(0x0201);
    }
    else if(res == 2)
    {
        r = static_cast<UInt16>(0x0200);
    }

    UInt32 id = bsim.clearLastBattle(side, isLast);

    for(UInt8 i = 0; i < td->count; ++i)
    {
        Player* pl = td->members[i];
        if(!pl)
            continue;
        if(ng && res == 1)
        {
            pl->_lastNg = ng;

            if (ng->getLevel() <= pl->GetLev() || (ng->getLevel() > pl->GetLev() && (ng->getLevel() - pl->GetLev()) < 10))
            {
                pl->pendExp(ng->getExp());
                ng->getLoots(pl, pl->_lastLoot);
            }
        }

        if(rptid == 0)
        {
            Stream st(REP::ATTACK_NPC);
            st << r << id << Stream::eos;
            pl->send(st);
        }
    }

    if(rptid == 0)
        rptid = id;
}

TeamCopyPlayerInfo::TeamCopyPlayerInfo(Player* owner)
{
    memset(m_pass, 0, sizeof(m_pass));
    memset(m_passTimes, 0, sizeof(m_passTimes));
    memset(m_vTime, 0, sizeof(m_vTime));
    m_owner = owner;
    m_maxPass = 100;
}

bool TeamCopyPlayerInfo::getPass(UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return false;

    return m_pass[t][copyIdx];
}

UInt8 TeamCopyPlayerInfo::getPassTimes(UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT || m_pass[t][copyIdx] == false )
        return 0xFF;

    UInt32 now = TimeUtil::Now();
    if(now > m_vTime[t][copyIdx])
    {
        m_vTime[t][copyIdx] = TimeUtil::SharpDayT(1, now);
        m_passTimes[t][copyIdx] = 0;
        DB3().PushUpdateData("UPDATE `teamcopy_player` SET `passTimes`=%u,`vTime`=%u WHERE `playerId` = %"I64_FMT"u AND `copyId` = %u AND `type` = %u", m_passTimes[t][copyIdx], m_vTime[t][copyIdx], m_owner->getId(), copyId, t);

        reqTeamCopyInfo();
    }

    return m_passTimes[t][copyIdx];
}

void TeamCopyPlayerInfo::setPassFromDB(UInt8 copyId, UInt8 t, bool pass)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return;

    m_pass[t][copyIdx] = pass;
}

void TeamCopyPlayerInfo::setPass(UInt8 copyId, UInt8 t, bool pass)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return;

    m_pass[t][copyIdx] = pass;
    DB3().PushUpdateData("REPLACE INTO `teamcopy_player`(`playerId`, `copyId`, `type`, `pass`, `passTimes`, `vTime`) VALUES(%"I64_FMT"u, %u, %u, %u, %u, %u)", m_owner->getId(), copyId, t, m_pass[t][copyIdx] ? 1 : 0, m_passTimes[t][copyIdx], m_vTime[t][copyIdx]);
}

void TeamCopyPlayerInfo::setPassTimesFromDB(UInt8 copyId, UInt8 t, UInt8 passTimes, UInt32 vTime)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return;

    m_passTimes[t][copyIdx] = passTimes;
    m_vTime[t][copyIdx] = vTime;
}

void TeamCopyPlayerInfo::setPassTimes(UInt8 copyId, UInt8 t, UInt8 passTimes, UInt32 vTime)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return;

    m_passTimes[t][copyIdx] = passTimes;
    m_vTime[t][copyIdx] = vTime;

    DB3().PushUpdateData("UPDATE `teamcopy_player` SET `passTimes`=%u,`vTime`=%u WHERE `playerId` = %"I64_FMT"u AND `copyId` = %u AND `type` = %u", m_passTimes[t][copyIdx], m_vTime[t][copyIdx], m_owner->getId(), copyId, t);
}

void TeamCopyPlayerInfo::incPass(UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return;

    ++m_passTimes[t][copyIdx];

    UInt32 now = TimeUtil::Now();
    if(now > m_vTime[t][copyIdx])
    {
        m_vTime[t][copyIdx] = TimeUtil::SharpDayT(1, now);
        m_passTimes[t][copyIdx] = 0;
    }

    sendUpdateTeamCopyInfo(copyId, t);
    DB3().PushUpdateData("UPDATE `teamcopy_player` SET `passTimes`=%u,`vTime`=%u WHERE `playerId` = %"I64_FMT"u AND `copyId` = %u AND `type` = %u", m_passTimes[t][copyIdx], m_vTime[t][copyIdx], m_owner->getId(), copyId, t);
}

bool TeamCopyPlayerInfo::checkTeamCopyPlayer(UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return false;

    UInt8 passTimes = getPassTimes(copyId, t);
    if(passTimes >= m_maxPass)
        return false;

    return m_pass[t][copyIdx];
}

void TeamCopyPlayerInfo::checkCopyPass(UInt32 taskId)
{
    switch(taskId)
    {
    case 121:
        setPass(1, 0, true);
        break;
    case 94:
        setPass(2, 0, true);
        break;
    case 148:
        setPass(3, 0, true);
        break;
    case 157:
        setPass(4, 0, true);
        break;
    }
}

void TeamCopyPlayerInfo::reqTeamCopyInfo()
{
    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0xFF) << static_cast<UInt8>(TEAMCOPY_MAXCOPYCNT * TEAMCOPY_MAXTYPECNT);
    for(UInt8 i = 0; i < TEAMCOPY_MAXCOPYCNT; ++i)
    {
        for(UInt8 t = 0; t < TEAMCOPY_MAXTYPECNT; ++t)
        {
            UInt32 now = TimeUtil::Now();
            if(now > m_vTime[t][i])
            {
                m_vTime[t][i] = TimeUtil::SharpDayT(1, now);
                m_passTimes[t][i] = 0;
            }

            st << static_cast<UInt8>(i+1) << t << m_passTimes[t][i];
        }
    }
    st << Stream::eos;

    m_owner->send(st);
}

void TeamCopyPlayerInfo::sendUpdateTeamCopyInfo(UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return;

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0xFF) << static_cast<UInt8>(1);
    st << copyId << t << m_passTimes[t][copyIdx];
    st << Stream::eos;
    m_owner->send(st);
}

}




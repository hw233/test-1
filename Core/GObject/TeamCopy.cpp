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
    for(int t = 0; t < 2; ++t)
    {
        for(int i = 0; i < maxCopyCnt; ++i)
        {
            for(UInt16 j = 0; j < maxTeamCnt; ++j)
            {
                m_tnIdle[t][i].push_back(j + 1);
            }
            m_copysTeam[t][i] = new CopyTeams();
        }
    }
}

TeamCopy::~TeamCopy()
{
    for(UInt8 t = 0; t < 2; ++t)
    {
        for(UInt8 idx = 0; idx < maxCopyCnt; ++idx)
        {
            m_clvTeamCnt[t][idx].clear();
            m_tnIdle[t][idx].clear();
            m_playerIdle[t][idx].clear();
        }

        for(UInt8 idx = 0; idx < maxCopyCnt; ++idx)
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
    if( copyId > maxCopyCnt || level < lvls[copyIdx] || t > 1)
        return false;

    return true;
}

void TeamCopy::addTeamCopyNpc(UInt8 copyId, UInt8 t, UInt16 location, UInt32 npcId)
{
    UInt8 copyIdx = copyId - 1;
    if( copyId > maxCopyCnt || t > 1)
        return;

    TeamCopyNpc& tcNpc = m_tcNpcId[t][copyIdx];
    tcNpc.location = location;
    tcNpc.npcId.push_back(npcId);
}

void TeamCopy::sendTeamCopyPageUpdate(UInt8 copyId, UInt8 t, UInt32 tdIdx)
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
        if(ctp.start < tdIdx && ctp.end > tdIdx)
        {
            reqTeamList(pl, ctp.start, maxPageLen, ctp.type);
        }
    }
}

void TeamCopy::reqTeamList(Player* pl, UInt32 start, UInt32 count, UInt8 type)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;
    UInt8 level = pl->GetLev();
    if(copyIdx >= maxCopyCnt )
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

    if(type == 1)
    {
        LevelTeamCnt& lvt = m_clvTeamCnt[t][copyIdx];
        LevelTeamCntIterator lvIt = lvt.find(level);
        if(lvIt == lvt.end())
            return;
        sz = lvIt->second;
    }

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x01) << sz << start << type << count;
    UInt8 cnt = 0;
    UInt32 idx = start;
    for(; idx < sz && cnt < count; ++idx)
    {
        if(type == 1 && ((ct[idx]->upLevel > 0 && ct[idx]->upLevel < level) || (ct[idx]->dnLevel > 0 && ct[idx]->dnLevel > level) || ct[idx]->pwd.length() > 0 || ct[idx]->count >= maxMemCnt))
        {
            continue;
        }
        ++cnt;

        st << ct[idx]->id << ct[idx]->leader->getId() << ct[idx]->upLevel << ct[idx]->dnLevel << static_cast<UInt8>(ct[idx]->pwd.length() > 0 ? 1 : 0);
        st << ct[idx]->count;
        for(int i = 0; i < ct[idx]->count; ++i)
        {
            Player* mem = ct[idx]->members[i];
            if(mem == ct[idx]->leader)
                continue;
            Fighter* fgt =  mem->getMainFighter();
            st << mem->getId() << fgt->getLevel() << fgt->getColor() << static_cast<UInt16>(fgt->getId());
        }
    }

    ctp.start = start;
    ctp.end = idx > (start + maxPageLen) ? idx : (start + maxPageLen);
    ctp.type = type;

    st << Stream::eos;
    pl->send(st);
}

void TeamCopy::reqTeamInfo(Player* pl)
{
    TeamData* td = pl->getTeamData();
    if(td == NULL)
        return;
    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x10);
    st << td->leader->getId() << td->count;

    for(UInt8 idx = 0; idx < td->count; ++idx)
    {
        st << td->members[idx]->getId() << td->members[idx]->getName();
        Fighter* fgt = td->members[idx]->getMainFighter();
        st << fgt->getLevel() << fgt->getColor() << static_cast<UInt16>(fgt->getId()) << td->members[idx]->getFormation();
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
        return 0;

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

    sendTeamCopyPageUpdate(copyId, t, ct.size());

    return td->id;
}

void TeamCopy::incLevelTeamCnt(UInt8 copyId, UInt8 t, UInt8 upLevel, UInt8 dnLevel)
{
    UInt8 copyIdx = copyId - 1;
    for(UInt8 i = dnLevel; i <= upLevel; ++i)
    {
        LevelTeamCnt& lvt = m_clvTeamCnt[t][copyIdx];
        LevelTeamCntIterator lvIt = lvt.find(i);
        if(lvIt == lvt.end())
            lvt[i] = 1;
        ++lvt[i];
    }
}

void TeamCopy::decLevelTeamCnt(UInt8 copyId, UInt8 t, UInt8 upLevel, UInt8 dnLevel)
{
    UInt8 copyIdx = copyId - 1;
    for(UInt8 i = dnLevel; i <= upLevel; ++i)
    {
        LevelTeamCnt& lvt = m_clvTeamCnt[t][copyIdx];
        LevelTeamCntIterator lvIt = lvt.find(i);
        if(lvIt == lvt.end())
            lvt[i] = 1;
        --lvt[i];
    }
}

UInt32 TeamCopy::joinTeam(Player* pl, UInt32 teamId, std::string pwd)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 t = ctp.t;
    if(!checkTeamCopy(pl, copyId, t))
        return 0;
    
    AllCopyTeamsIterator it = m_allCopyTeams.find(teamId);
    if(it == m_allCopyTeams.end())
        return 0;
    if(it->second == NULL)
        return 0;

    TeamData* td = it->second;
    if(td->count >= maxMemCnt || t != td->t)
        return 0;

    if(td->pwd != pwd)
        return 0;

    pl->setTeamData(td);
    td->members[td->count] = pl;
    ++td->count;

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
            for(UInt8 j = i+1; j < td->count; ++j)
            {
                td->members[j-1] = td->members[j];
            }
            --td->count;
            break;
        }
    }
    if(res == 0)
    {
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
                sendTeamCopyPageUpdate(copyId, t, i);
            }
        }

        recycleTeamNumber(copyId, t, td->id >> 8);
    }
    else if(pl == td->leader)
    {
        td->leader = td->members[0];
    }

    st << td->leader->getId() << pl->getId();
    st << Stream::eos;
    // send team info to members ::TODO
    for(UInt8 j = 0; j < td->count; ++j)
    {
        td->members[j]->send(st);
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
            td->members[i] = NULL;
            for(UInt8 j = i+1; j < td->count; ++j)
            {
                td->members[j-1] = td->members[j];
            }
            --td->count;
            break;
        }
    }

    member->setTeamData(NULL);
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
        return false;

//    TeamCopyNpc& npcIds = m_tcNpcId[t][copyIdx];
//    pl->moveTo(npcIds.location, false);

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
        return false;

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
    for(; memIdx < td->count; ++memIdx)
    {
        if(npcIdx >= cnt)
            break;

        member = td->members[memIdx];
        if(!member)
            break;

        if(res == 1)
        {
            sendBattleReport(td, NULL, bsim);
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
            if( 2 == res )
            {
                sendBattleReport(td, NULL, bsim);
            }

            ng = ngs[npcIdx];
            if(!ng)
                break;
            ng->putFighters( bsim );
            for(UInt8 nIdx = 1; nIdx <= cnt; ++nIdx)
            {
                UInt8 idx = nIdx % cnt;
                if(idx == 0)
                    break;

                bsim.putTeams(ngs[idx]->getName(), ngs[idx]->getLevel(), ngs[idx]->getPortrait(), 0);
            }

            bsim.start();
            res = bsim.getWinner() == 1;
            ++npcIdx;
            if( 1 == res )
            {
                break;
            }
        }
    }

    sendBattleReport(td, NULL, bsim);

    if( 1 == res )
    {
        for(UInt8 i = 0; i < td->count; ++i)
            leaveTeamCopy(td->members[i]);
    }


    recycleTeamNumber(copyId, t, td->id >> 8);
}

void TeamCopy::sendBattleReport(TeamData* td, GData::NpcGroup* ng, Battle::BattleSimulator& bsim)
{
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return;

    UInt8 res = bsim.getWinner() == 1;
    UInt16 r = 0;
    if(res == 1)
    {
        if(!ng)
            bsim.clearLastBattle(1);

        r = static_cast<UInt16>(0x0101);
    }
    else if(res == 2)
    {
        if(!ng)
            bsim.clearLastBattle(0);

        r = static_cast<UInt16>(0x0100);
    }

    for(UInt8 i = td->count; i < td->count; ++i)
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

        Stream st(REP::ATTACK_NPC);
        st << pl->getPendExp() << static_cast<UInt8>(0) << r;
        UInt8 sz = pl->_lastLoot.size();
        st << sz;
        for(UInt8 j = 0; j < sz; ++ j)
        {
            st << pl->_lastLoot[j].id << pl->_lastLoot[j].count;
        }

        st.append(&packet[8], packet.size() - 8);
        st << Stream::eos;

        pl->send(st);
    }
}

}








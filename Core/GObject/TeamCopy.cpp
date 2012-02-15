#include "TeamCopy.h"
#include "Player.h"
#include "Common/Stream.h"
#include "MsgID.h"
#include "Package.h"
#include "Server/Cfg.h"
#include "GData/Money.h"
#include "Country.h"
#include "Script/GameActionLua.h"
#include "HeroMemo.h"

namespace GObject
{

TeamCopy* teamCopyManager = new TeamCopy();

const UInt8 TeamCopy::lvls[6] = {30, 45, 60, 70, 80, 90};
const UInt8 TeamCopyPlayerInfo::_needRoll = 1;
const UInt8 TeamCopyPlayerInfo::_hasRoll = 2;
std::vector<TeamCopyAwards> TeamCopyPlayerInfo::_awards[TEAMCOPY_MAXCOPYCNT];

TeamCopy::TeamCopy() : _notify1(false), _notify2(false), _isDoubleExp(false)
{
    for(int t = 0; t < TEAMCOPY_MAXTYPECNT; ++t)
    {
        for(int i = 0; i < TEAMCOPY_MAXCOPYCNT; ++i)
        {
            for(UInt16 j = TEAMCOPY_MAXTEAMCNT; j > 0; --j)
            {
                m_tnIdle[t][i].push_back(j);
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
    if( !copyId || copyId > TEAMCOPY_MAXCOPYCNT || level < lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
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
        if(type == 1 && ((ct[idx]->upLevel > 0 && ct[idx]->upLevel < level) || (ct[idx]->dnLevel > 0 && ct[idx]->dnLevel > level) || ct[idx]->count >= TEAMCOPY_MAXMEMCNT ))
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
    st << td->id << td->leader->getId() << td->dnLevel << td->count;

    for(UInt8 idx = 0; idx < td->count; ++idx)
    {
        st << td->members[idx]->getId() << td->members[idx]->getName() << td->status[idx];
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
    UInt32 cnt = m_tnIdle[t][copyIdx].size();
    if(cnt == 0)
        return 0;

    UInt16 number = m_tnIdle[t][copyIdx][cnt-1];
    m_tnIdle[t][copyIdx].pop_back();

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

    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now)
    {
        pl->sendMsgCode(0, 1407, buffLeft - now);
        return 0;
    }

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

    dnLevel = dnLevel > lvls[copyIdx] ? dnLevel : lvls[copyIdx];
    upLevel = upLevel < LEVEL_MAX ? upLevel : LEVEL_MAX;

    CopyTeams& ct = *(m_copysTeam[t][copyIdx]);
    TeamData* td = new TeamData();

    td->pwd = pwd;
    td->id = copyId + (t << 5) + (number << 8);
    td->upLevel = upLevel;
    td->dnLevel = dnLevel;
    td->count = 1;
    td->t = t;
    td->leader = pl;
    td->members[0] = pl;
    ct.push_back(td);
    pl->setTeamData(td);

    m_allCopyTeams[td->id] = td;

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

    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now)
    {
        pl->sendMsgCode(0, 1407, buffLeft - now);
        return 0;
    }

    if(pl->getTeamData() != NULL)
        return false;
    if(!checkTeamCopy(pl, copyId, t))
        return false;

    CopyTeams& ct = *(m_copysTeam[t][copyIdx]);
    UInt32 sz = ct.size();

    UInt8 level = pl->GetLev();
    bool res = false;
    for(UInt32 idx = 0; idx < sz; ++idx)
    {
        if((ct[idx]->upLevel != 0 && level > ct[idx]->upLevel) || (ct[idx]->dnLevel != 0 && level < ct[idx]->dnLevel))
            continue;

        if(ct[idx]->count < TEAMCOPY_MAXMEMCNT && ct[idx]->pwd.length() == 0)
        {
            res = true;
            joinTeam(pl, ct[idx]->id, "");
            break;
        }
    }

    if(!res)
        pl->sendMsgCode(1, 2110);

    return res;
}

UInt32 TeamCopy::joinTeam(Player* pl, UInt32 teamId, std::string pwd)
{
    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 t = ctp.t;

    UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now)
    {
        pl->sendMsgCode(0, 1407, buffLeft - now);
        return 0;
    }

    if(pl->getTeamData() != NULL)
        return 0;

    if(!checkTeamCopy(pl, copyId, t))
        return 0;

    UInt8 teamCopyId = teamId & 0x1F;
    UInt8 teamCopyType = (teamId >> 5 & 0x07);

    if(copyId != teamCopyId || t != teamCopyType)
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

    UInt8 level = pl->GetLev();
    if((td->upLevel != 0 && level > td->upLevel) || (td->dnLevel != 0 && level < td->dnLevel))
    {
		pl->sendMsgCode(1, 2109);
        return 0;
    }

    if(td->pwd != pwd)
    {
        if(pwd.length() == 0)
            return 1;
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
            td->status[i] = 0;
            UInt8 j = i+1;
            for(; j < td->count; ++j)
            {
                td->members[j-1] = td->members[j];
                td->status[j-1] = td->status[j];
            }
            td->members[j-1] = NULL;
            td->status[j-1] = 0;
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
    bool find = false;
    if(td->count == 0)
    {
        m_allCopyTeams.erase(td->id);

        CopyTeams& ct = *(m_copysTeam[t][copyIdx]);
        for(UInt32 i = 0; i < ct.size(); ++i)
        {
            if (td == ct[i])
            {
                find = true;
                ct.erase(ct.begin() + i);
                break;
            }
        }

        if(!find)
        {
            for(UInt8 i = 0; i < TEAMCOPY_MAXTYPECNT; ++i)
            {
                for(UInt8 j = 0; j < TEAMCOPY_MAXCOPYCNT; ++j)
                {
                    CopyTeams& ct = *(m_copysTeam[i][j]);
                    for(UInt32 k = 0; k < ct.size(); ++k)
                    {
                        if (td == ct[k])
                        {
                            find = true;
                            ct.erase(ct.begin() + k);
                            break;
                        }
                    }
                }
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

    st << res;
    st << Stream::eos;
    // send team info to members ::TODO
    for(UInt8 j = 0; j < td->count; ++j)
    {
        reqTeamInfo(td->members[j]);
    }

    pl->send(st);

    if(find)
        delete td;
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

    for(UInt8 i = 0; i < td->count; ++i)
    {
        if(td->members[i] == member)
        {
            leaveTeam(member);
            sendTeamCopyPageUpdate(copyId, t, ctp.start, ctp.end);
            break;
        }
    }

    member->sendMsgCode(1, 2111);
}

void TeamCopy::reQueueTeam(Player* pl, UInt8 idx0, UInt8 idx1)
{
    TeamData* td = pl->getTeamData();
    if(td == NULL)
        return;

    CopyTeamPage& ctp = pl->getCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 t = ctp.t;
    if(!checkTeamCopy(pl, copyId, t))
        return;
    if(idx0 >= td->count || idx1 >= td->count)
        return;

    if(td->leader != pl)
        return;

    Player* ptmp = NULL;
    ptmp = td->members[idx1];
    UInt8 tmpStatus = td->status[idx1];
    td->members[idx1] = td->members[idx0];
    td->members[idx0] = ptmp;
    td->status[idx1] = td->status[idx0];
    td->status[idx0] = tmpStatus;

    updateTeamInfo(pl);
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

    bool find = false;
    for(UInt8 idx = 0; idx < td->count; ++idx)
    {
        if(td->members[idx] == member)
        {
            find = true;
            break;
        }
    }

    if(!find)
        return;

    td->leader = member;

    updateTeamInfo(pl);
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

    if (pl->hasFlag(GObject::Player::AutoCopy)) {
        pl->sendMsgCode(0, 1414);
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
    pl->delFlag(GObject::Player::InCopyTeam);

    return true;
}

void TeamCopy::teamBattleStart(Player* pl, UInt8 type)
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

    if(type == 1 || type == 2)
    {
        for(int i = 0; i < td->count; ++i)
        {
            if(td->members[i] == pl)
            {
                if(type == 1)
                    td->status[i] = 1;
                else
                    td->status[i] = 0;
                break;
            }
        }

        for(int j = 0; j < td->count; ++j)
        {
            reqTeamInfo(td->members[j]);
        }
        return;
    }
    else if(td->leader != pl)
    {
        return;
    }
    else
    {
        UInt32 now = TimeUtil::Now();
        for(int i = 0; i < td->count; ++i)
        {
            if(td->status[i] != 1 && td->members[i] != pl)
                return;

            Player* member = td->members[i];
            UInt32 buffLeft = member->getBuffData(PLAYER_BUFF_ATTACKING, now);
            if(cfg.GMCheck && buffLeft > now)
            {
                member->sendMsgCode(0, 1407, buffLeft - now);
                return;
            }
        }

        for(int j = 0; j < td->count; ++j)
        {
            td->members[j]->checkLastBattled();
        }
    }

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
    UInt16 bs = 0;
    bs = copyId - 1 + Battle::BS_COPY1;

    Battle::BattleSimulator bsim(bs, member, ng->getName(), ng->getLevel(), false);

    UInt8 npcIdx = 0;
    UInt8 memIdx = 0;
    UInt8 res = 0;
    UInt32 rptid = 0;
    UInt32 turns = 0;
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
        if(memIdx != 0)
            bsim.switchPlayer( member, 0 );

        for(UInt8 mIdx = memIdx + 1; mIdx < td->count; ++mIdx)
        {
            Fighter* fgt = NULL;
            fgt = td->members[mIdx]->getMainFighter();
            bsim.putTeams(td->members[mIdx]->getName(), fgt->getLevel(), fgt->getId(), 0);
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
                if(npcIdx != 0)
                    bsim.switchPlayer( ng->getName(), ng->getLevel() );

                for(UInt8 nIdx = npcIdx + 1; nIdx < cnt; ++nIdx)
                {
                    bsim.putTeams(ngs[nIdx]->getName(), ngs[nIdx]->getLevel(), ngs[nIdx]->getPortrait(), 1);
                }
            }

            if(res == 0)
                bsim.start();
            else
                bsim.start(res);

            turns += bsim.getTurns();
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
                tcpInfo->setPass(copyId, 1, true, true);
            }

            if(t == 1)
            {
                tcpInfo->setAwardRoll(copyId);
            }
        }
    }

    UInt32 now = TimeUtil::Now();
    for(UInt8 i = td->count; i > 0; --i)
    {
        Player* pl = td->members[i-1];
        if(pl == NULL)
            continue;

        if( 1 == res )
            leaveTeamCopy(pl);
        else
            leaveTeam(pl);

        bsim.applyFighterHP(0, pl);

        if(res != 1)
            pl->checkDeath();

        pl->setBuffData(PLAYER_BUFF_ATTACKING, now + turns);
    }

    pl->OnHeroMemo(MC_SLAYER, MD_MASTER, 0, 0);
    if (copyId == 1 && type == 0)
        pl->OnHeroMemo(MC_SLAYER, MD_MASTER, 0, 1);
    if (copyId == 1 && type == 1)
        pl->OnHeroMemo(MC_SLAYER, MD_MASTER, 0, 2);
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

            //if (ng->getLevel() <= pl->GetLev() || (ng->getLevel() > pl->GetLev() && (ng->getLevel() - pl->GetLev()) < 10))
            {
                UInt32 now = TimeUtil::Now();
                UInt32 chkPoint1 = TimeUtil::SharpDayT(0, now) + TEAMCOPY_EXTRAREWARDTM1;
                UInt32 chkPoint2 = TimeUtil::SharpDayT(0, now) + TEAMCOPY_EXTRAREWARDTM2;
                bool flag = (now > chkPoint1 && now < chkPoint1 + 3600) || (now > chkPoint2 && now < chkPoint2 + 3600);
                if(flag)
                {
                    pl->pendExp(ng->getExp());
                    ng->getLoots(pl, pl->_lastLoot, td->count - 1, NULL);
                }

                pl->pendExp(ng->getExp());
                ng->getLoots(pl, pl->_lastLoot, td->count - 1, NULL);
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

void TeamCopy::process(UInt32 now)
{
    UInt32 notify1 = TimeUtil::SharpDayT(0,now) + 11 * 60 * 60;
    UInt32 notifyEnd1 = TimeUtil::SharpDayT(0,now) + 12 * 60 * 60;
    UInt32 notify2 = TimeUtil::SharpDayT(0,now) + 19 * 60 * 60;
    UInt32 notifyEnd2 = TimeUtil::SharpDayT(0,now) + 20 * 60 * 60;

    if (now >= notify1 && now < notifyEnd1 && !_notify1)
    {
        _notify1 = true;
        _isDoubleExp = true;
        sendDaily(NULL);
    }

    if (now >= notify2 && now < notifyEnd2 && !_notify2)
    {
        _notify2 = true;
        _isDoubleExp = true;
        sendDaily(NULL);
    }

    if (now >= notifyEnd1 && notify2)
    {
        _isDoubleExp = false;
        sendDaily(NULL);
    }

    if (now >= notifyEnd2 && _isDoubleExp)
    {
        _notify1 = _notify2 = false;
        _isDoubleExp = false;
        sendDaily(NULL);
    }
}

void TeamCopy::sendDaily(Player* player)
{
    if (player && !_isDoubleExp)
        return;
    Stream st(REP::DAILY_DATA);
    st << static_cast<UInt8>(7);
    st << static_cast<UInt8>(_isDoubleExp?0:1);
    st << Stream::eos;
    if (player)
        player->send(st);
    else
        NETWORK()->Broadcast(st);
}

TeamCopyPlayerInfo::TeamCopyPlayerInfo(Player* owner)
{
    memset(m_pass, 0, sizeof(m_pass));
    memset(m_passTimes, 0, sizeof(m_passTimes));
    memset(m_vTime, 0, sizeof(m_vTime));
    m_owner = owner;
    m_maxPass = 1;
    m_roll = 0;
    m_rollId = 0;
    m_awardId = 0;
    m_awardCnt = 0;
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

void TeamCopyPlayerInfo::setPass(UInt8 copyId, UInt8 t, bool pass, bool notify)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] || t >= TEAMCOPY_MAXTYPECNT )
        return;

    m_pass[t][copyIdx] = pass;
    if(notify)
        sendUpdateTeamCopyInfo(copyId);

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

    if(m_passTimes[t][copyIdx] == 1)
    {

               GameAction()->doAty(m_owner, AtyGroupCopy, 1,0);
    }
    UInt32 now = TimeUtil::Now();
    if(now > m_vTime[t][copyIdx])
    {
        m_vTime[t][copyIdx] = TimeUtil::SharpDayT(1, now);
        m_passTimes[t][copyIdx] = 0;
    }

    sendUpdateTeamCopyInfo(copyId);
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
    if(m_roll != 0)
        return false;

    return m_pass[t][copyIdx];
}

void TeamCopyPlayerInfo::checkCopyPass(UInt32 taskId)
{
    switch(taskId)
    {
    case 121:
        if(getPass(1, 0) == false)
            setPass(1, 0, true);
        break;
    case 94:
        if(getPass(2, 0) == false)
            setPass(2, 0, true);
        break;
    case 148:
        if(getPass(3, 0) == false)
            setPass(3, 0, true);
        break;
    case 157:
        if(getPass(4, 0) == false)
            setPass(4, 0, true);
        break;
    }
}

void TeamCopyPlayerInfo::reqTeamCopyInfo()
{
    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x0F) << static_cast<UInt8>(TEAMCOPY_MAXCOPYCNT);
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
        }
        st << static_cast<UInt8>(i+1) << m_pass[0][i] << m_pass[1][i] << m_passTimes[0][i] << m_passTimes[1][i];
    }
    st << Stream::eos;

    m_owner->send(st);
}

void TeamCopyPlayerInfo::sendUpdateTeamCopyInfo(UInt8 copyId)
{
    UInt8 copyIdx = copyId - 1;
    UInt8 level = m_owner->GetLev();
    if( copyId > TEAMCOPY_MAXCOPYCNT || level < TeamCopy::lvls[copyIdx] )
        return;

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x0F) << static_cast<UInt8>(1);
    st << copyId << m_pass[0][copyIdx] << m_pass[1][copyIdx] << m_passTimes[0][copyIdx] << m_passTimes[1][copyIdx];
    st << Stream::eos;
    m_owner->send(st);
}

void TeamCopyPlayerInfo::rollAward(UInt8 type)
{
    if(m_roll != _needRoll)
        return;
    if(m_rollId == 0)
        return;

    if(type > 0)
    {
        UInt32 goldUse = GData::moneyNeed[GData::TEAMCOPYAWARD_ROLL].gold;
        if(m_owner->getGold() < goldUse)
        {
            m_owner->sendMsgCode(0, 1101);
            return;
        }
        else
        {
            ConsumeInfo ci(TeamCopyAwardRoll, 0, 0);
            m_owner->useGold(goldUse, &ci);
        }
    }

    UInt8 rollIdx = m_rollId - 1;

    m_roll = _hasRoll;
    UInt8 cnt = _awards[rollIdx].size();
    UInt8 idx = uRand(cnt);
    m_awardId = idx + 1;
    m_awardCnt = _awards[rollIdx][idx].num;
    if(type > 0)
        m_awardCnt *= 2;

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x0D) << m_awardId << Stream::eos;
    m_owner->send(st);

    DB3().PushUpdateData("UPDATE `teamcopy_player_award` SET `roll`=%u, `awardId`=%u, `awardCnt`=%u WHERE `playerId`=%"I64_FMT"u", m_roll, m_awardId, m_awardCnt, m_owner->getId());
}

bool TeamCopyPlayerInfo::getAward()
{
    if(m_awardId == 0 || m_awardCnt == 0)
        return false;

    if(m_owner->GetFreePackageSize() < 1)
    {
        m_owner->sendMsgCode(0, 1011);
        return false;
    }

    if(m_rollId > 0 && m_rollId <= TEAMCOPY_MAXCOPYCNT)
    {
        UInt8 rollIdx = m_rollId - 1;
        if(m_awardId <= _awards[rollIdx].size())
        {
            UInt32 itemId = _awards[rollIdx][m_awardId - 1].id;
            m_owner->GetPackage()->AddItem(itemId, m_awardCnt, true, false, FromTeamCopy);
        }
    }

    m_awardCnt = 0;
    m_awardId = 0;
    m_roll = 0;
    m_rollId = 0;

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x0C);
    st << m_rollId << m_roll << m_awardId << Stream::eos;
    m_owner->send(st);

    DB3().PushUpdateData("DELETE FROM `teamcopy_player_award` WHERE `playerId` = %"I64_FMT"u", m_owner->getId());

    return true;
}

void TeamCopyPlayerInfo::setAwardRoll(UInt8 rollId)
{
    m_rollId = rollId;
    m_roll = _needRoll;

    DB3().PushUpdateData("REPLACE INTO `teamcopy_player_award`(`playerId`, `rollId`, `roll`, `awardId`, `awardCnt`) VALUES(%"I64_FMT"u, %u, %u, 0, 0)", m_owner->getId(), m_rollId, m_roll);
}

void TeamCopyPlayerInfo::sendAwardInfo()
{
    if(m_roll == 0)
        return;

    Stream st(REP::TEAM_COPY_REQ);
    st << static_cast<UInt8>(0x0C);
    st << m_rollId << m_roll << m_awardId << Stream::eos;
    m_owner->send(st);
}

void TeamCopyPlayerInfo::loadAwardInfoFromDB(UInt8 rollId, UInt8 roll, UInt32 awardId, UInt32 awardCnt)
{
    if(rollId == 0 || roll == 0)
        return;

    m_rollId = rollId;
    m_roll = roll;
    m_awardCnt = awardCnt;
    m_awardId = awardId;
}

void TeamCopyPlayerInfo::addTeamCopyAwardCfg(UInt8 rollId, UInt32 awardId, UInt32 awardCnt)
{
    UInt8 rollIdx = rollId - 1;
    if( rollIdx > TEAMCOPY_MAXCOPYCNT)
        return;

    _awards[rollIdx].push_back(TeamCopyAwards(awardId, awardCnt));
}

void TeamCopyPlayerInfo::clearTeamCopyAwardCfg()
{
    for(UInt8 rollIdx = 0; rollIdx < TEAMCOPY_MAXCOPYCNT; ++ rollIdx)
        _awards[rollIdx].clear();
}

void TeamCopyPlayerInfo::resetTCPlayer()
{
    memset(m_passTimes, 0, sizeof(m_passTimes));
}

UInt8 TeamCopyPlayerInfo::getPassNum()
{
    UInt8 num = 0;
    for (UInt8 i = 0 ; i < TEAMCOPY_MAXCOPYCNT ; i ++)
    {
        for(UInt8 j = 0 ; j < TEAMCOPY_MAXTYPECNT; j ++)
        if( m_passTimes[j][i] )
            num ++;
    }
    return num;
}
}




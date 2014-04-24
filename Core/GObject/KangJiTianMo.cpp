#include "KangJiTianMo.h"
#include "Player.h"
#include "Clan.h"
#include "Common/Stream.h"
#include "Common/Itoa.h"
#include "Common/StringTokenizer.h"
#include "MsgID.h"
#include "Package.h"
#include "Server/Cfg.h"
#include "GData/Money.h"
#include "Country.h"
#include "Server/SysMsg.h"

namespace GObject
{

KangJiTianMo* KJTMManager = new KangJiTianMo();

KangJiTianMo::KangJiTianMo()
{
}

KangJiTianMo::~KangJiTianMo()
{
}

void KangJiTianMo::GetKJTMStatus(Player* pl)
{
    if(NULL == pl)
        return;

    UInt32 begin = GVAR.GetVar(GVAR_KANGJITIANMO_BEGIN);
    UInt32 end = GVAR.GetVar(GVAR_KANGJITIANMO_END);
    UInt32 status = pl->GetVar(VAR_KJTM_STATUS);
    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x00) << static_cast<UInt8>(0x00);
    st << status;
    st << begin;
    st << end;
    st << Stream::eos;
    pl->send(st);
}

void KangJiTianMo::GetKJTMData(Player* pl)
{
    if(NULL == pl)
        return;

    UInt32 status = pl->GetVar(VAR_KJTM_STATUS);
    UInt8 mark = GET_BIT(status, 1);
    if(0 == mark)
    {
        status = SET_BIT(status, 1);
        pl->SetVar(VAR_KJTM_STATUS, status);
    }

    CreateTeamMember(pl);

    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x00) << static_cast<UInt8>(0x01);
    TeamMemberInfo(pl, st);
    st << Stream::eos;
    pl->send(st);
}

inline bool player_enum_0(GObject::Player* pl, int)
{
    if(NULL == pl)
        return false;

    UInt32 dur = TimeUtil::Now() - pl->getLastOnline();
    if(dur >= 7*86400)
    {
        pl->SetVar(VAR_KJTM_STATUS, 1);
        if(pl->getVipLevel() > 0)
        {
            KJTMManager->_InactiveMember.push_back(pl->getId());
            KJTMManager->_Mark.push_back(KJTMManager->_InactiveMember.size());

            DB5().PushUpdateData("INSERT INTO `inactivemember` (`playerId`) VALUES (%" I64_FMT "u)", pl->getId());
        }
    }
    return true;
}

void KangJiTianMo::AddInactiveMember()
{
    globalPlayers.enumerate(player_enum_0, 0);
}

void KangJiTianMo::ClearInactiveMember()
{
    if(_InactiveMember.size() > 0)
        _InactiveMember.clear();

    if(_Mark.size() > 0)
        _Mark.clear();
}

void KangJiTianMo::AddInactiveMemberFromDB(UInt64 playerId)
{
    _InactiveMember.push_back(playerId);
    _Mark.push_back(_InactiveMember.size());
}

void KangJiTianMo::RandInactiveMember(Player* pl, UInt8 type)
{
	Mutex::ScopedLock lk(_mutex);

    if(NULL == pl)
        return;

    ClearInactiveSort();
    
    UInt8 cnt = 0;
    InactiveMemberMark tempMark(_Mark);
    //tempMark.resize(_Mark.size());
    //memcpy(&tempMark[0], &_Mark[0], _Mark.size()*sizeof(UInt32));

    InactiveMember tempMember;
    while(tempMark.size() > 0 &&_InactiveMember.size() >= tempMark.size() && cnt<10)
    {
        UInt32 pos = uRand(tempMark.size());
        UInt64 playerId = _InactiveMember[tempMark[pos]-1];
        Player* member = globalPlayers[playerId];
        if(NULL == member)
        {
            tempMark.erase(tempMark.begin()+pos);
            continue;
        }

        if(NULL != member->getTeamMemberData())
        {
            tempMark.erase(tempMark.begin()+pos);
            continue;
        }

        if(pl->isFriend(member))
        {
            tempMark.erase(tempMark.begin()+pos);
            continue;
        }

        Clan* clan = pl->getClan();
        if(NULL != clan)
        {
            if(clan->IsClanFriends(member))
            {
                tempMark.erase(tempMark.begin()+pos);
                continue;
            }
        }
  
        bool mark = false;
        for(UInt8 i=0; i<tempMember.size(); i++)
        {
            if(playerId == tempMember[i])
            {
                tempMark.erase(tempMark.begin()+pos);
                mark = false;
                break;
            }
            mark = true;
        }
        
        if(mark || 0 == cnt)
        {
            //std::cout <<playerId<<"@!@# "<<member->getName()<<std::endl;
            SetInactiveSort(member);
            tempMember.push_back(playerId);
            cnt++;
        }
    }
    SendInactiveSort(pl, type);
}

void KangJiTianMo::SetInactiveSort(Player* pl)
{
    if(NULL == pl)
        return;

    InactiveSort s;
    s.player = pl;
    s.level = pl->GetLev();
    s.power = pl->GetVar(VAR_TOTAL_BATTLE_POINT);
    s.time = TimeUtil::Now();
    _CommonSort.insert(s);
}
        
void KangJiTianMo::SendInactiveSort(Player* pl, UInt8 type)
{
    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x01);
    st << type;
    st << static_cast<UInt8>(_CommonSort.size());
    for(InactiveSortType::iterator i = _CommonSort.begin(), e = _CommonSort.end(); i != e; ++i)
    {
        if(NULL == i->player)
            continue;

        st << i->player->getId();
        st << i->player->getCountry();
        st << i->player->getName();
        st << i->level;
        st << i->power;
        st << i->player->getOpenId();
        st <<  static_cast<UInt8>(i->player->GetVar(VAR_FRIEND_SECURITY));
        //std::cout <<i->player->getId()<<"@!@# "<<i->player->getName()<<std::endl;
    }
    st << Stream::eos;
    pl->send(st);
}

void KangJiTianMo::ClearInactiveSort()
{
    if(_CommonSort.size() > 0)
        _CommonSort.clear();
}

void KangJiTianMo::AddTeamMember(UInt32 teamId, Player* p1, Player* p2, Player* p3)
{
    if(0==teamId || NULL==p1)
        return;

    TeamMemberData* tmd = new TeamMemberData();
    tmd->teamId = teamId;
    tmd->members[0] = p1;
    tmd->memCnt = 1;
    
    bool markA = false;
    bool markB = false;
    if(NULL != p2)
    {
        tmd->members[1] = p2;
        tmd->memCnt++;
        markA = true;
    }
    if(NULL != p3)
    {
        tmd->members[2] = p3;
        tmd->memCnt++;
        markB = true;
    }
    
    p1->setTeamMemberData(tmd);
    if(markA)
        p2->setTeamMemberData(tmd);
    if(markB)
        p3->setTeamMemberData(tmd);
}

void KangJiTianMo::CreateTeamMember(Player* pl)
{
	Mutex::ScopedLock lk(_mutex);
    UInt32 status = pl->GetVar(VAR_KJTM_STATUS);
    UInt8 mark = GET_BIT(status, 0);

    TeamMemberData* tmdA = pl->getTeamMemberData();
    if(NULL != tmdA)
        return;

    if(0 == mark)
    {
        TeamMemberData* tmdB = new TeamMemberData();
        UInt32 curId = GVAR.GetVar(GVAR_MAX_TEAM_NUM);
        UInt32 nextId = curId + 1;
        tmdB->teamId = nextId;
        tmdB->members[0] = pl;
        tmdB->memCnt = 1;
        GVAR.SetVar(GVAR_MAX_TEAM_NUM, nextId);
        pl->setTeamMemberData(tmdB);
        DB5().PushUpdateData("INSERT INTO `teammember` (`teamId`, `member1`) VALUES ( %u, %" I64_FMT "u)", nextId, pl->getId());
    }
}


void KangJiTianMo::JoinTeamMember(UInt64 playerId, UInt64 applicantId)
{
	Mutex::ScopedLock lk(_mutex);

    Player* pl = globalPlayers[playerId];
    if(NULL == pl)
        return;

    Player* applicant = globalPlayers[applicantId];
    if(NULL == applicant)
        return;

    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    TeamMemberData* tmdA = applicant->getTeamMemberData();
    if(NULL != tmdA)
    {
        pl->sendMsgCode(1, 8016);
        return;
    }

    if(pl != tmd->members[0])
        return;

    if(tmd->memCnt >= TEAM_MAXMEMCNT)
    {
        pl->sendMsgCode(1, 8017);
        return;
    }

    UInt32 status = applicant->GetVar(VAR_KJTM_STATUS);
    UInt8 mark = GET_BIT(status, 0);
    if(0 == mark)
        return;

    tmd->members[tmd->memCnt] = applicant;
    tmd->memCnt++;
    applicant->setTeamMemberData(tmd);

    UpdateTeamMember(pl);

    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x08);
    TeamMemberInfo(pl, st);
    st << Stream::eos;

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL != tmd->members[i])
            tmd->members[i]->send(st);
    }
}

void KangJiTianMo::LeaveTeamMember(Player* pl)
{
    if(NULL == pl)
        return;

    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    Player* leader = tmd->members[0]; 
    if(NULL == leader)
        return;

    TeamMemberKick(leader, pl);
}

void KangJiTianMo::TeamMemberKick(Player* leader, Player* pl)
{
	Mutex::ScopedLock lk(_mutex);

    if(NULL == leader)
        return;

    if(NULL == pl)
        return;

    TeamMemberData* tmd = leader->getTeamMemberData();
    if(NULL == tmd)
        return;

    if(pl == tmd->members[0])
        return;
    
    bool res = false;
    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(pl == tmd->members[i])
        {
            res = true;
            tmd->members[i] = NULL;
            UInt8 j = i + 1;
            for(; j<tmd->memCnt; j++)
            {
                tmd->members[j-1] = tmd->members[j];
            }
            tmd->members[j-1] = NULL;
            tmd->memCnt--;
            break;
        }
    }

    if(!res)
    {
		pl->sendMsgCode(1, 2105);
        return;
    }

    pl->setTeamMemberData(NULL);

    UpdateTeamMember(leader);

    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x08);
    TeamMemberInfo(leader, st);
    st << Stream::eos;
    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL != tmd->members[i])
            tmd->members[i]->send(st);
    }

    Stream st1(REP::KANGJITIANMO_REP);
    st1 << static_cast<UInt8>(0x09);
    st1 << Stream::eos;
    pl->send(st1);
}

void KangJiTianMo::UpdateTeamMember(Player* pl)
{
    if(NULL == pl)
        return;

    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    UInt64 member0 = 0;
    UInt64 member1 = 0;
    UInt64 member2 = 0;
    if(NULL != tmd->members[0])
        member0 = tmd->members[0]->getId();

    if(NULL != tmd->members[1])
        member1 = tmd->members[1]->getId();

    if(NULL != tmd->members[2])
        member2 = tmd->members[2]->getId();

    DB5().PushUpdateData("REPLACE INTO `teammember` VALUES ( %u, %" I64_FMT "u, %" I64_FMT "u, %" I64_FMT "u)", tmd->teamId, member0, member1, member2);
}

void KangJiTianMo::TeamMemberInfo(Player* pl, Stream& st)
{
	Mutex::ScopedLock lk(_mutex);

    if(NULL == pl)
        return;

    size_t offsetA = st.size();
    UInt8 countA = 0;
    st << countA;

    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        Player* member = tmd->members[i];
        if(NULL == member)
            continue;

        st << member->getId();
        st << member->GetClassAndSex();
        st << member->getName();
        st << member->GetLev();
        st << static_cast<UInt8>(member->getVipLevel());
        UInt32 power = member->GetVar(VAR_TOTAL_BATTLE_POINT);
       
        if(0 == i)
            power = power * 0.3f;
        else
        {
            if(member->getVipLevel() >= 1 && member->getVipLevel() <= 4)
                power += power * 0.5f;
            else if(member->getVipLevel() >= 5)
                power += power;
        }
        st << power;
    
        UInt8 isOnline = 0;
        if(member->isOnline())
            isOnline = 1;
        st << isOnline;

        countA++;
    }
    st.data<UInt8>(offsetA) = countA;
}

void KangJiTianMo::GetKillNPCStatus(Player* pl)
{
    if(NULL == pl)
        return;

    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x10);
    size_t offsetA = st.size();
    UInt8 countA = 0;
    st << countA;

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        Player* member = tmd->members[i];
        if(NULL == member)
            continue;

        UInt32 status = member->GetVar(VAR_KJTM_KILL_NPC_STATUS);
        st << status;
        countA++;
    }
    st.data<UInt8>(offsetA) = countA;
    st << Stream::eos;
    pl->send(st);

    UInt32 statusA = pl->GetVar(VAR_KJTM_STATUS);
    UInt8 mark = GET_BIT(statusA, 3);
    if(0 == mark)
    {
        statusA = SET_BIT(statusA, 3);
        pl->SetVar(VAR_KJTM_STATUS, statusA);
    }
}

void KangJiTianMo::CreateBattleRoom(Player* pl, UInt8 index)
{
    if(NULL == pl)
        return;

    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    if(0==index || index>11)
        return;

    if(pl != tmd->members[0])
        return;

    if(pl->GetTMDYRoomStatus() > 0)
        return;

    UInt32 NPCId[11] = {14000, 14003, 14006, 14009, 14012, 14015, 14018, 14021, 14024, 14027, 14030};
    tmd->NPCId = NPCId[index-1];
    tmd->index = index;

    pl->SetTMDYRoomStatus(1);
    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x11);
    st << pl->getId();
    st << pl->getName();
    st << pl->GetClassAndSex();
    st << Stream::eos;
    pl->send(st);
}

void KangJiTianMo::InviteTeamMember(Player* pl)
{
    if(NULL == pl)
        return;

    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    if(pl != tmd->members[0])
        return;

    if(1 != pl->GetTMDYRoomStatus())
        return;

    if(0 == tmd->NPCId)
        return;

    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x12);
    st << pl->getId();
    st << pl->getName();
    st << Stream::eos;

    if(NULL != tmd->members[1] 
            && tmd->members[1]->isOnline() 
            && 0 == tmd->members[1]->GetTMDYRoomStatus())
        tmd->members[1]->send(st);

    if(NULL != tmd->members[2] 
            && tmd->members[2]->isOnline() 
            && 0 == tmd->members[2]->GetTMDYRoomStatus())
        tmd->members[2]->send(st);
}

void KangJiTianMo::JoinBattleRoom(Player* pl, UInt64 leaderId)
{
    if(NULL == pl)
        return;

    Player* leader = globalPlayers[leaderId];
    if(NULL == leader)
        return;

    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    if(leader != tmd->members[0])
        return;

    if(1 != leader->GetTMDYRoomStatus())
    {
		pl->sendMsgCode(1, 2102);
        return;
    }

    if(1 == pl->GetTMDYRoomStatus())
        return;
    
    if(1 == tmd->start)
    {
		pl->sendMsgCode(1, 8001);
        return;
    }

    pl->SetTMDYRoomStatus(1);

    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x13);
    GetBattleRoomInfo(leader, st);
    st << Stream::eos;

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL != tmd->members[i] && 1 == tmd->members[i]->GetTMDYRoomStatus())
            tmd->members[i]->send(st);
    }
}

void KangJiTianMo::LeaveRoom(Player* pl, UInt8 opt)
{    
    if(NULL == pl)
        return;
    
    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    if(pl == tmd->members[0])
        DismissBattleRoom(pl);
    else
        LeaveBattleRoom(pl, opt);
}

void KangJiTianMo::LeaveBattleRoom(Player* pl, UInt8 opt)
{
    if(NULL == pl)
        return;
    
    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    if(pl == tmd->members[0])
        return;

    if(1 != pl->GetTMDYRoomStatus())
        return;

    if(1 == tmd->start && 0 == opt)
        return;

    pl->SetTMDYRoomStatus(0);

    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x13);
    GetBattleRoomInfo(tmd->members[0], st);
    st << Stream::eos;

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL != tmd->members[i] && 1 == tmd->members[i]->GetTMDYRoomStatus())
            tmd->members[i]->send(st);
    }

    DelBattleRoomInfo(pl);
}

void KangJiTianMo::DismissBattleRoom(Player* pl)
{
    if(NULL == pl)
        return;
    
    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    if(pl != tmd->members[0])
        return;
    
    if(0 == pl->GetTMDYRoomStatus())
        return;

    tmd->start = 0;
    tmd->index = 0;
    tmd->NPCId = 0;

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL != tmd->members[i] && 1 == tmd->members[i]->GetTMDYRoomStatus())
        {
            DelBattleRoomInfo(tmd->members[i]);
            tmd->members[i]->SetTMDYRoomStatus(0);
        }
    }
}

void KangJiTianMo::DelBattleRoomInfo(Player* pl)
{
    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x14);
    st << Stream::eos;
    pl->send(st);
}

void KangJiTianMo::GetBattleRoomInfo(Player* pl, Stream& st)
{
    if(NULL == pl)
        return;
    
    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    st << tmd->index;
    size_t offsetA = st.size();
    UInt8 countA = 0;
    st << countA;
    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL != tmd->members[i] && 1 == tmd->members[i]->GetTMDYRoomStatus())
        {
            st << tmd->members[i]->getId();
            st << tmd->members[i]->getName();
            st << tmd->members[i]->GetClassAndSex();
            countA++;
        }
    }
    st.data<UInt8>(offsetA) = countA;
}

void KangJiTianMo::NoticeBattle(Player* pl)
{
    if(NULL == pl)
        return;
    
    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    tmd->start = 1;

    Stream st(REP::KANGJITIANMO_REP);
    st << static_cast<UInt8>(0x16);
    st << Stream::eos;

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL != tmd->members[i] && 1 == tmd->members[i]->GetTMDYRoomStatus())
            tmd->members[i]->send(st);
    }
}

void KangJiTianMo::StartBattle(Player* pl)
{
    if(NULL == pl)
        return;
    
    TeamMemberData* tmd = pl->getTeamMemberData();
    if(NULL == tmd)
        return;

    Player* member = tmd->members[0];
    if(!member)
        return;

    if(pl != member)
        return;

    if(1 != pl->GetTMDYRoomStatus())
        return;

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL != tmd->members[i] && 1 == tmd->members[i]->GetTMDYRoomStatus())
            tmd->members[i]->checkLastBattled();
    }

    //暂时一组怪
    GData::NpcGroups::iterator it = GData::npcGroups.find(tmd->NPCId);
    if(it == GData::npcGroups.end())
        return;
    GData::NpcGroup * ng = it->second;
    if (!ng)
        return;

    UInt8 cnt = 0;
    std::vector<GData::NpcGroup*> ngs;
    ngs.push_back(ng);
    cnt++;
    ng = ngs[0];

    Battle::BattleSimulator bsim(Battle::BS_COPY1, member, ng->getName(), ng->getLevel(), false);

    UInt8 npcIdx = 0;
    UInt8 memIdx = 0;
    UInt8 res = 0;
    UInt32 rptid = 0;
    UInt32 turns = 0;
    for(; memIdx < tmd->memCnt; ++memIdx)
    {
        if(npcIdx >= cnt)
            break;

        member = tmd->members[memIdx];
        if(!member)
            break;

        if(1 != member->GetTMDYRoomStatus())
            continue;

        if(2 == res)    //战败
            SendBattleReport(tmd, NULL, bsim, rptid);

        member->PutFighters(bsim, 0);
        if(memIdx != 0)
            bsim.switchPlayer(member, 0);

        float factor = 1.0f;
        if(memIdx==0)
            factor = static_cast<float>(30)/100.0f;
        else
        {
            UInt32 status = member->GetVar(VAR_KJTM_KILL_NPC_STATUS); 
            UInt8 mark = GET_BIT(status, (tmd->index-1));
            if(0 == mark)
            {
                if(member->getVipLevel() >= 1 && member->getVipLevel() <= 4)
                    factor = static_cast<float>(150)/100.0f;
                else if(member->getVipLevel() >= 5)
                    factor = static_cast<float>(200)/100.0f;
            }
        }
        member->setKJTMFactor(factor);

        for(UInt8 mIdx = memIdx + 1; mIdx < tmd->memCnt; ++mIdx)
        {
            if(NULL == tmd->members[mIdx])
                continue;

            if(1 != tmd->members[mIdx]->GetTMDYRoomStatus())
                continue;

            Fighter* fgt = NULL;
            fgt = tmd->members[mIdx]->getMainFighter();
            if(NULL != fgt)
                bsim.putTeams(tmd->members[mIdx]->getName(), fgt->getLevel(), fgt->getId(), 0);
        }

        while(npcIdx < cnt)
        {
            if(1 == res)   //战胜
                SendBattleReport(tmd, NULL, bsim, rptid);

            if(2 != res)
            {
                ng = ngs[npcIdx];
                if(!ng)
                    break;
                ng->putFighters(bsim);
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
            res = bsim.getWinner();    // 1表示攻防胜利(右下角)，2表示守方胜利(左上角)
            if( 2 == res )
            {
                break;
            }
            ++npcIdx;
        }
    }

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        if(NULL == tmd->members[i] || 1 != tmd->members[i]->GetTMDYRoomStatus())
            continue;

        tmd->members[i]->setKJTMFactor(1.0f);
    }

    SendBattleReport(tmd, ng, bsim, rptid);

    for(UInt8 i=0; i<tmd->memCnt; i++)
    {
        Player* member = tmd->members[i];
        if(NULL == pl)
            continue;

        if(1 != member->GetTMDYRoomStatus())
            continue;

        bsim.applyFighterHP(0, member);

        member->udpLog("kangjitianmo", "F_140406_4", "", "", "", "", "act");
        if(0 != i)
        {
            member->udpLog("kangjitianmo", "F_140406_5", "", "", "", "", "act");
            if(member->getVipLevel() > 0)
                member->udpLog("kangjitianmo", "F_140406_6", "", "", "", "", "act");
            if(member->getVipLevel() > 4)
                member->udpLog("kangjitianmo", "F_140406_7", "", "", "", "", "act");
        }
    }
    DismissBattleRoom(pl);
}

void KangJiTianMo::SendBattleReport(TeamMemberData* tmd, GData::NpcGroup* ng, Battle::BattleSimulator& bsim, UInt32& rptid)
{
    Stream& packet = bsim.getPacket();
    if(packet.size() <= 8)
        return;

    bool isLast = (ng != NULL);
    UInt8 side = 0;
    UInt16 r = 0;
    UInt8 res = bsim.getWinner();
    if(1 == res)
    {
        side = 1;
        r = static_cast<UInt16>(0x0201);
    }
    else if(2 == res)
    {
        r = static_cast<UInt16>(0x0200);
    }

    UInt32 id = bsim.clearLastBattle(side, isLast);

    for(UInt8 i=0; i<tmd->memCnt; ++i)
    {
        Player* pl = tmd->members[i];
        if(NULL == pl)
            continue;

        if(1 != pl->GetTMDYRoomStatus())
            continue;

        if(ng && 1==res && tmd->index>0 && tmd->index<12)
        {
            UInt32 status = pl->GetVar(VAR_KJTM_KILL_NPC_STATUS); 
            UInt8 mark = GET_BIT(status, (tmd->index-1));
            if(0 == mark)
            {
                ng->getLoots(pl, pl->_lastLoot, 0, NULL);
                {
                    status = SET_BIT(status, (tmd->index-1));
                    pl->SetVar(VAR_KJTM_KILL_NPC_STATUS, status);
                }
            }
            else
                pl->sendMsgCode(1, 1450);
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

}

#include "PetTeamCopy.h"
#include "Player.h"
#include "Common/Stream.h"
#include "Common/Itoa.h"
#include "Common/StringTokenizer.h"
#include "MsgID.h"
#include "Package.h"
#include "Server/Cfg.h"
#include "GData/Money.h"
#include "Server/SysMsg.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "TeamCopy.h"

namespace GObject
{

PetTeamCopy* petTeamCopyManager = new PetTeamCopy();

const UInt8 PetTeamCopy::lvls[PETTEAMCOPY_MAXCOPYCNT] = {80, 90, 100, 110, 120};

PetTeamCopy::PetTeamCopy()
{
    for(int t = 0; t < PETTEAMCOPY_MAXTYPECNT; ++t)
    {
        for(int i = 0; i < PETTEAMCOPY_MAXCOPYCNT; ++i)
        {
            for(UInt16 j = PETTEAMCOPY_MAXTEAMCNT; j > 0; --j)
            {
                m_ptnIdle[t][i].push_back(j);
            }
        }
    }
}

PetTeamCopy::~PetTeamCopy()
{
    m_playerIdle.clear();

    for(UInt8 t = 0; t < PETTEAMCOPY_MAXTYPECNT; ++t)
    {
        for(UInt8 idx = 0; idx < PETTEAMCOPY_MAXCOPYCNT; ++idx)
        {
            m_ptnIdle[t][idx].clear();
        }
    }
}

PetTeamCopyPlayerInfo::PetTeamCopyPlayerInfo(Player* owner)
{
    m_owner = owner;
}

void PetTeamCopyPlayerInfo::setNpcGroupIdFromDB(UInt8 copyId, UInt8 type, UInt32 npcGroup1Id, UInt32 npcGroup2Id, UInt32 npcGroup3Id)
{
   if(copyId == 0 || npcGroup1Id == 0 || npcGroup2Id == 0 || npcGroup3Id == 0)
       return;

   m_playerNpcId[type][copyId-1].push_back(npcGroup1Id); 
   m_playerNpcId[type][copyId-1].push_back(npcGroup2Id); 
   m_playerNpcId[type][copyId-1].push_back(npcGroup3Id); 
}

bool PetTeamCopy::checkPetTeamCopy(Player* pl, UInt8 copyId, UInt8 t)
{
    if(pl == NULL)
        return false;

    if(!copyId || copyId > PETTEAMCOPY_MAXCOPYCNT || t >= PETTEAMCOPY_MAXTYPECNT)
        return false;

    return true;
}

UInt16 PetTeamCopy::getIdleTeamNumber(UInt8 copyId, UInt8 t)
{
    UInt8 copyIdx = copyId - 1;
    UInt32 cnt = m_ptnIdle[t][copyIdx].size();
    if(cnt == 0)
        return 0;

    UInt16 number = m_ptnIdle[t][copyIdx][cnt-1];
    m_ptnIdle[t][copyIdx].pop_back();

    return number;
}

void PetTeamCopy::recycleTeamNumber(UInt8 copyId, UInt8 t, UInt16 number)
{
    UInt8 copyIdx = copyId - 1;
    m_ptnIdle[t][copyIdx].push_back(number);
}

void PetTeamCopy::addPetTeamCopyNpc(UInt8 copyId, UInt8 t, UInt8 quality, UInt32 npcId)
{
    if(copyId > PETTEAMCOPY_MAXCOPYCNT || t > PETTEAMCOPY_MAXTYPECNT)
        return;

    if(copyId == 0 || quality == 0 || npcId == 0)
        return;

    UInt8 copyIdx = copyId - 1;
    PetTeamCopyNpcId& ptcNpcId = m_ptcNpcId[t][copyIdx];
    PetTeamCopyNpcIdIterator it = ptcNpcId.find(quality);
    if(it == ptcNpcId.end())
    {
        std::vector<UInt32> _npcId;
        _npcId.push_back(npcId);
        ptcNpcId.insert(std::make_pair(quality, _npcId));
    }
    else
        it->second.push_back(npcId);

    m_petTeamCopyNpc.insert(std::make_pair(npcId, quality));
}

void PetTeamCopy::sendPetTeamCopyPageUpdate(UInt8 opt, UInt32 teamId)
{
    if(opt != 0 && opt != 1)
        return;

    for(UInt32 idx=0; idx<m_playerIdle.size(); ++idx)
    {
        if(m_playerIdle[idx] == NULL)
            continue;

        if(opt == 0)
            updateTeamInfo(m_playerIdle[idx], teamId);
        else
            delTeamInfo(m_playerIdle[idx], teamId);
    }
}

void PetTeamCopy::reqTeamList(Player* pl)
{
    if(pl == NULL)
        return;

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x00);
    UInt16 helpCount = pl->GetVar(VAR_HELP_NUM);
    UInt16 ordinaryKillCount = pl->GetVar(VAR_ORDINARY_KILL_NUM);
    UInt16 difficultyKillCount = pl->GetVar(VAR_DIFFICULTY_KILL_NUM);
    UInt8 mark = pl->GetVar(VAR_PETTEAMCOPY_BATTLE_RES);
    UInt8 freeKillA = 0;
    UInt8 freeKillB = 0;

    if(0 == ordinaryKillCount)
        freeKillA = 1;

    if(0 == difficultyKillCount)
        freeKillB = 1;

    if(1 == GET_BIT(mark, 0))
        freeKillA++;

    if(1 == GET_BIT(mark, 1))
        freeKillB++;

    st << ordinaryKillCount << difficultyKillCount << freeKillA << freeKillB << helpCount;
    UInt16 pos = st.size();
    UInt16 cnt = 0;
    st << cnt;
    AllTeamsIterator it = m_allTeams.begin();
    for(; it!=m_allTeams.end(); it++)
    { 
        PetTeamData* td = it->second;
        if(!td)
            continue;

        if(td->leader == NULL)
            continue;

        FairyPet * pet = td->leader->getBattlePet();
        if(NULL == pet)
            continue;

        PetTeamCopyNpcIterator iter = m_petTeamCopyNpc.find(td->NPCId);
        if(iter == m_petTeamCopyNpc.end())
            continue;

        UInt8 quality = iter->second;

        GData::NpcGroups::iterator it = GData::npcGroups.find(td->NPCId);
        if(it == GData::npcGroups.end())
            continue;

        GData::NpcGroup * ng = it->second;
        if (ng == NULL)
            continue;

        std::vector<GData::NpcFData>& _npcList = ng->getList();
        if (_npcList.size() == 0)
            continue;

        GObject::Fighter* monster = _npcList[0].fighter;
        if(monster == NULL)
            continue;

        cnt++;
        st << static_cast<UInt8>(td->index) << static_cast<UInt8>(td->type) 
            << static_cast<UInt32>(td->id) << quality << static_cast<UInt32>(monster->getId()) 
            << td->leader->getId() << static_cast<UInt32>(pet->getId()) << static_cast<UInt8>(td->count);
        for(UInt8 i=0; i<td->count; i++)
        {
            st << td->members[i]->getId();
        }
    }
    st.data<UInt16>(pos) = cnt;
    st << Stream::eos;
    pl->send(st);
}

void PetTeamCopy::reqTeamInfo(Player* pl)
{
    if(pl == NULL)
        return;

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x03);
    teamInfo(pl, st);
    st << Stream::eos;
    pl->send(st);
}

void PetTeamCopy::updateTeamInfo(Player* pl, UInt32 teamId)
{
    if(pl == NULL)
        return;

    AllTeamsIterator it = m_allTeams.find(teamId);
    if(it == m_allTeams.end())
        return;

    PetTeamData* td = it->second;
    if(!td)
        return;

    if(td->leader == NULL)
        return;

    FairyPet * pet = td->leader->getBattlePet();
    if(NULL == pet)
        return;

    GData::NpcGroups::iterator itA = GData::npcGroups.find(td->NPCId);
    if(itA == GData::npcGroups.end())
        return;

    GData::NpcGroup * ng = itA->second;
    if (!ng)
        return;

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x13);
    st << static_cast<UInt8>(td->index) << static_cast<UInt8>(td->type) 
        << static_cast<UInt32>(td->id) << static_cast<UInt8>(td->quality) << static_cast<UInt32>(ng->getId()) 
        << td->leader->getId() << static_cast<UInt32>(pet->getId()) << static_cast<UInt8>(td->count);

    st << Stream::eos;
    pl->send(st);
}

void PetTeamCopy::delTeamInfo(Player* pl, UInt32 teamId)
{
    if(pl == NULL)
        return;

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x14) << teamId;
    st << Stream::eos;
    pl->send(st);
}

void PetTeamCopy::teamInfo(Player* pl, Stream& st)
{
    if(pl == NULL)
        return;

    PetTeamData* td = pl->getPetTeamData();
    if(td == NULL)
        return;

    if(td->leader == NULL)
        return;

    st << static_cast<UInt8>(td->index) << static_cast<UInt8>(td->type) << static_cast<UInt32>(td->NPCId) 
        << static_cast<UInt32>(td->id) << td->leader->getId() << static_cast<UInt8>(td->count);

    for(UInt8 i=0; i<td->count; ++i)
    {
        st << td->members[i]->getId() << td->members[i]->getCountry() << td->members[i]->getName().c_str() << static_cast<UInt8>(td->formation[i]);
        FairyPet * pet = td->members[i]->getBattlePet();
        if(pet)
        {
            st << static_cast<UInt32>(pet->getId()); 
            st << static_cast<UInt32>(pet->getBattlePoint()); 
            st << pet->getPetLev();
            st << pet->getPetBone();
            st << pet->getChongNum();
            st << pet->getPetEvolve();
            pet->AppendEquipData(st);
            pet->sendHunPoInfo(st);
            pet->sendSevenSoulInfo(st);
        }
    }
}

void PetTeamCopy::reqMonsterInfo(Player* pl)
{
    if(pl == NULL)
        return;

    PetCopyTeamPage& ctp = pl->getPetCopyTeamPage();
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;

    PetTeamCopyPlayerInfo* ptcpInfo = pl->getPetTeamCopyPlayerInfo();
    if(ptcpInfo == NULL)
        return;

    if(ptcpInfo->m_playerNpcId[t][copyIdx].size() < 3)
        return;

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x04);
    
    UInt16 refreshNum = 0;
    if(t == ORDINARYCOPY)
        refreshNum = pl->GetVar(VAR_ORDINARY_REFRESH_NUM);
    else
        refreshNum = pl->GetVar(VAR_DIFFICULTY_REFRESH_NUM);

    UInt16 ordinaryKillCount = pl->GetVar(VAR_ORDINARY_KILL_NUM);
    UInt16 difficultyKillCount = pl->GetVar(VAR_DIFFICULTY_KILL_NUM);
    UInt8 mark = pl->GetVar(VAR_PETTEAMCOPY_BATTLE_RES);
    UInt8 freeKillA = 0;
    UInt8 freeKillB = 0;

    if(0 == ordinaryKillCount)
        freeKillA = 1;

    if(0 == difficultyKillCount)
        freeKillB = 1;

    if(1 == GET_BIT(mark, 0))
        freeKillA++;

    if(1 == GET_BIT(mark, 1))
        freeKillB++;
 
    st << refreshNum << ordinaryKillCount << difficultyKillCount << freeKillA << freeKillB;
    for(UInt8 j=0; j<ptcpInfo->m_playerNpcId[t][copyIdx].size(); j++)
    {
        UInt32 NPCId = ptcpInfo->m_playerNpcId[t][copyIdx][j];
        GData::NpcGroups::iterator it = GData::npcGroups.find(NPCId);
        if(it == GData::npcGroups.end())
            return;

        GData::NpcGroup * ng = it->second;
        if (ng == NULL)
            return;

        std::vector<GData::NpcFData>& _npcList = ng->getList();
        if (_npcList.size() == 0)
            return;

        GObject::Fighter* monster = _npcList[0].fighter;
        if(monster == NULL)
            return;

        st << static_cast<UInt32>(monster->getId()) << NPCId;
    }
    
    UInt16 sz = _logs.size();
    st << sz;
    for (std::list<KillMonsterLog>::iterator i=_logs.begin(), e=_logs.end(); i!=e; ++i)
    {
        Player* pl = globalNamedPlayers[(*i).playerName];
        if (!pl)
            continue;

        st << (*i).playerName;
        st << pl->getCountry();
        st << (*i).monsterName;
        UInt16 s = (*i).items.size();
        st << s;
        for (UInt8 k=0; k<s; k++)
        {
            st << (*i).items[k];
        }
    }

    st << Stream::eos;
    pl->send(st);
}

void PetTeamCopy::pushLog(UInt8 color1, const std::string& playerName, UInt8 color2, const std::string& monsterName, const std::vector<UInt32>& i)
{
    KillMonsterLog l;
    l.playerName = playerName;
    l.monsterName = monsterName;
    l.items = i;

    UInt32 size = _logs.size();
    if (size >= 10)
        _logs.pop_back();
    _logs.push_front(l);

    std::string its;
    std::string itemnames;
    UInt32 sz = i.size();
    for (UInt32 j=0; j<sz; ++j)
    {
        SYSMSGV(iname, 5120, i[j]);
        itemnames += iname;

        its += Itoa(i[j]);
        if (j != sz - 1)
        {
            its += ",";
            itemnames += ",";
        }
    }

    if (playerName.empty() || monsterName.empty() || its.empty() || itemnames.empty())
        return;

    SYSMSG_BROADCASTV(5121, color1, playerName.c_str(), color2, monsterName.c_str(), itemnames.c_str());

    DB6().PushUpdateData("INSERT INTO `petteamcopylog` VALUES (0, '%s','%s', '%s')", playerName.c_str(), monsterName.c_str(), its.c_str());
}

void PetTeamCopy::pushLog(const std::string& playerName, const std::string& monsterName, const std::string& its)
{
    KillMonsterLog l;
    l.playerName = playerName;
    l.monsterName = monsterName;

    StringTokenizer items(its, ",");
    UInt32 sz = items.count();
    if (!sz)
        return;

    for (UInt32 i=0; i<sz; i++)
    {
        UInt32 itemId = atoi(items[i].c_str());
        l.items.push_back(itemId);
    }
    _logs.push_back(l);
}

void PetTeamCopy::enter(Player* pl)
{
    if(pl == NULL)
        return;

    if(!pl->GetEnterPTCStatus())
        m_playerIdle.push_back(pl);

    pl->SetEnterPTCStatus(true);

    reqTeamList(pl);
}

void PetTeamCopy::quit(Player* pl)
{
    if(pl == NULL)
        return;

    for(UInt32 idx = 0; idx <m_playerIdle .size(); ++idx)
    {
        if(m_playerIdle[idx] == pl)
        {
            m_playerIdle.erase(m_playerIdle.begin() + idx);
            break;
        }
    }

    pl->SetEnterPTCStatus(false);
}

bool PetTeamCopy::enterTeamCopy(Player* pl, UInt8 copyId, UInt8 t)
{
    if(pl == NULL)
        return false;

    UInt8 copyIdx = copyId - 1;
    if(!checkPetTeamCopy(pl, copyId, t))
        return false;

    PetTeamCopyPlayerInfo* ptcpInfo = pl->getPetTeamCopyPlayerInfo();
    if(ptcpInfo == NULL)
        return false;

    if(0 == ptcpInfo->m_playerNpcId[t][copyIdx].size())
    {
        UInt32 npcGroupId[3];
        memset(npcGroupId, 0, sizeof(npcGroupId));
        for(UInt8 i=0; i<3; i++)
        {
            UInt8 quality = 0;
            UInt8 randA = uRand(100);
            if(randA >= 92)
                quality = 4;
            else if(randA >= 68)
                quality = 3;
            else
                quality = 2;

            PetTeamCopyNpcId& ptcNpcId = m_ptcNpcId[t][copyIdx];
            PetTeamCopyNpcIdIterator it = ptcNpcId.find(quality);
            if(it != ptcNpcId.end())
            {
                UInt8 cnt = it->second.size();
                if(cnt > 0)
                {
                    UInt8 rand = uRand(cnt);
                    ptcpInfo->m_playerNpcId[t][copyIdx].push_back(it->second[rand]);
                    npcGroupId[i] = it->second[rand];
                }
            }
        }
        DB3().PushUpdateData("REPLACE INTO `player_petteamcopy`(`playerId`, `copyId`, `type`, `npcGroup1Id`, `npcGroup2Id`, `npcGroup3Id`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u)", 
                pl->getId(), copyId, t, npcGroupId[0], npcGroupId[1], npcGroupId[2]);
    }


    PetCopyTeamPage& pctp = pl->getPetCopyTeamPage();
    pctp.copyId = copyId;
    pctp.t = t;

    reqMonsterInfo(pl);
    quit(pl);

    return true;
}

void PetTeamCopy::refreshMonster(Player* pl)
{
    if(pl == NULL)
        return;

    PetCopyTeamPage& ctp = pl->getPetCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = copyId - 1;
    UInt8 t = ctp.t;

    UInt16 refreshNum = 0;
    if(t == ORDINARYCOPY)
        refreshNum = pl->GetVar(VAR_ORDINARY_REFRESH_NUM);
    else
        refreshNum = pl->GetVar(VAR_DIFFICULTY_REFRESH_NUM);
    
    if(refreshNum > 2)
    {
        if(!pl->hasChecked())
            return;

        if(pl->getGold() < 2)
        {
            pl->sendMsgCode(0, 1104);
            return;
        }

        ConsumeInfo ci(RefreshMonster, 0, 0);
        pl->useGold(2, &ci);
        
        if(0 == t)
            pl->udpLog("chongwufuben", "F_55500", "", "", "", "", "act");
        else
            pl->udpLog("chongwufuben", "F_55501", "", "", "", "", "act");
    }

    UInt8 quality = 0;
    bool mark1 = false;
    bool mark2 = false;
    UInt32 npcGroupId[3];
    memset(npcGroupId, 0, sizeof(npcGroupId));
    UInt32 monsterId[3];
    memset(monsterId, 0, sizeof(monsterId));

    PetTeamCopyPlayerInfo* ptcpInfo = pl->getPetTeamCopyPlayerInfo();
    if(ptcpInfo->m_playerNpcId[t][copyIdx].size() > 0)
        ptcpInfo->m_playerNpcId[t][copyIdx].clear();

    for(UInt8 i=0; i<3; i++)
    {
        UInt8 purpleNum = pl->GetVar(VAR_PURPLE_NUM);
        UInt8 orangeNum = pl->GetVar(VAR_ORANGE_NUM);

        UInt8 randA = uRand(100);
        if(randA >= (99 - orangeNum))
        {
            quality = 5;
            pl->SetVar(VAR_ORANGE_NUM, 0);
            mark1 = true;
        }
        else if(randA >= (92 - purpleNum))
        {
            quality = 4;
            pl->SetVar(VAR_PURPLE_NUM, 0);
            mark2 = true;
        }
        else if(randA >= 68)
            quality = 3;
        else
            quality = 2;

        PetTeamCopyNpcId& ptcNpcId = m_ptcNpcId[t][copyIdx];
        PetTeamCopyNpcIdIterator it = ptcNpcId.find(quality);
        if(it != ptcNpcId.end())
        {
            UInt8 cnt = it->second.size();
            if(cnt > 0)
            {
                UInt8 rand = uRand(cnt);
                ptcpInfo->m_playerNpcId[t][copyIdx].push_back(it->second[rand]);
                npcGroupId[i] = it->second[rand];

                GData::NpcGroups::iterator itA = GData::npcGroups.find(npcGroupId[i]);
                if(itA == GData::npcGroups.end())
                    continue;

                GData::NpcGroup * ng = itA->second;
                if(ng == NULL)
                    continue;

                std::vector<GData::NpcFData>& _npcList = ng->getList();
                if (_npcList.size() == 0)
                    continue;

                GObject::Fighter* monster = _npcList[0].fighter;
                if(monster == NULL)
                    continue;

                monsterId[i] = monster->getId();
            }
        }
        
        if(5 == quality)
        {
            if(0 == t)
                pl->udpLog("chongwufuben", "F_556005", "", "", "", "", "act");
            else
                pl->udpLog("chongwufuben", "F_556015", "", "", "", "", "act");
        }
    }

    DB3().PushUpdateData("REPLACE INTO `player_petteamcopy`(`playerId`, `copyId`, `type`, `npcGroup1Id`, `npcGroup2Id`, `npcGroup3Id`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u)", 
            pl->getId(), copyId, t, npcGroupId[0], npcGroupId[1], npcGroupId[2]);

    if(mark1 && quality != 4)
        pl->AddVar(VAR_PURPLE_NUM, 2);

    if(mark2 && quality != 5)
        pl->AddVar(VAR_ORANGE_NUM, 1);

    if(t == ORDINARYCOPY)
        pl->AddVar(VAR_ORDINARY_REFRESH_NUM, 1);
    else
        pl->AddVar(VAR_DIFFICULTY_REFRESH_NUM, 1);

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x05);
    st << static_cast<UInt16>(refreshNum+1) << npcGroupId[0] << monsterId[0] << npcGroupId[1] << monsterId[1] << npcGroupId[2] << monsterId[2];
    st << Stream::eos;
    pl->send(st);
}

void PetTeamCopy::addMonster(Player* pl, UInt8 copyId, UInt8 type, UInt32 oldNPCId)
{
    PetTeamCopyPlayerInfo* ptcpInfo = pl->getPetTeamCopyPlayerInfo();
    if(ptcpInfo == NULL)
        return;
    
    UInt8 copyIdx = copyId - 1;
    UInt8 t = type;

    if(ptcpInfo->m_playerNpcId[t][copyIdx].size() != 3)
        return;

    UInt32 npcGroupId[3];
    memset(npcGroupId, 0, sizeof(npcGroupId));

    for(UInt8 k=0; k<ptcpInfo->m_playerNpcId[t][copyIdx].size(); k++)
    {
        if(ptcpInfo->m_playerNpcId[t][copyIdx][k] == oldNPCId)
        {
            UInt8 quality = 0;
            UInt8 randA = uRand(100);
            if(randA >= 92)
                quality = 4;
            else if(randA >= 68)
                quality = 3;
            else
                quality = 2;

            PetTeamCopyNpcId& ptcNpcId = m_ptcNpcId[t][copyIdx];
            PetTeamCopyNpcIdIterator it = ptcNpcId.find(quality);
            if(it != ptcNpcId.end())
            {
                UInt8 cnt = it->second.size();
                if(cnt > 0)
                {
                    UInt8 rand = uRand(cnt);
                    ptcpInfo->m_playerNpcId[t][copyIdx][k] = it->second[rand];
                    npcGroupId[k] = it->second[rand];
                }
            }
        }
        else
            npcGroupId[k] = ptcpInfo->m_playerNpcId[t][copyIdx][k];
    }
    DB3().PushUpdateData("REPLACE INTO `player_petteamcopy`(`playerId`, `copyId`, `type`, `npcGroup1Id`, `npcGroup2Id`, `npcGroup3Id`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u)", 
                          pl->getId(), copyId, t, npcGroupId[0], npcGroupId[1], npcGroupId[2]);
}

UInt32 PetTeamCopy::createTeam(Player* pl, UInt32 NPCId, UInt32 monsterId)
{
    if(pl == NULL)
        return 0;

    if(pl->GetInPTCStatus())
    {
		pl->sendMsgCode(1, 2106);
        return 0;
    }

    PetCopyTeamPage& ctp = pl->getPetCopyTeamPage();
    UInt8 copyId = ctp.copyId;
    UInt8 copyIdx = ctp.copyId - 1;
    UInt8 t = ctp.t;

    /*UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now)
    {
        pl->sendMsgCode(0, 1407, buffLeft - now);
        return 0;
    }*/

    UInt8 level = pl->GetLev();
    if(level < lvls[copyIdx])
        return 0;

    if(!checkPetTeamCopy(pl, copyId, t))
        return 0;

    if(pl->getPetTeamData() != NULL)
        return 0;

    UInt16 number = getIdleTeamNumber(copyId, t);
    if(number == 0)
    {
		pl->sendMsgCode(1, 2101);
        return 0;
    }

    PetTeamCopyNpcIterator it = m_petTeamCopyNpc.find(NPCId);
    if(it == m_petTeamCopyNpc.end())
        return 0;

    UInt8 quality = it->second;
    if(quality < 2 && quality > 5)
        return 0;

    FairyPet* pet = pl->getBattlePet();
    if(pet == NULL)
        return 0;

    UInt32 killNum = 0;
    UInt32 money = 0;
    if(t == ORDINARYCOPY)
    {
        killNum = pl->GetVar(VAR_ORDINARY_KILL_NUM);
        if(killNum >= 1)
            money = killNum * 10;
    }
    else
    {
        killNum = pl->GetVar(VAR_DIFFICULTY_KILL_NUM);
        if(killNum == 1)
            money = 20;
        else if(killNum > 1)
            money = 20 + (killNum - 1) * 10;
    }
    bool markA = false;
    UInt8 mark = pl->GetVar(VAR_PETTEAMCOPY_BATTLE_RES);
    if(0 == GET_BIT(mark, t))
        markA = true;

    if(killNum >= 1 && 0 == GET_BIT(mark, t))
    {
        if(!pl->hasChecked())
            return 0;

        if(money > pl->getGold())
        {
            pl->sendMsgCode(0, 1104);
            return 0;
        }

        ConsumeInfo ci(CreateRoom, 0, 0);
        pl->useGold(money, &ci);
        mark = SET_BIT(mark, t);
        pl->SetVar(VAR_PETTEAMCOPY_BATTLE_RES, mark);

        if(0 == t)
        {
            switch(copyId)
            {
                case 1:
                    pl->udpLog("chongwufuben", "F_58000", "", "", "", "", "act");
                    break;
                case 2:
                    pl->udpLog("chongwufuben", "F_59000", "", "", "", "", "act");
                    break;
                case 3:
                    pl->udpLog("chongwufuben", "F_510000", "", "", "", "", "act");
                    break;
                case 4:
                    pl->udpLog("chongwufuben", "F_511000", "", "", "", "", "act");
                    break;
                case 5:
                    pl->udpLog("chongwufuben", "F_512000", "", "", "", "", "act");
                    break;
            }
        }
        else
        {
            switch(copyId)
            {
                case 1:
                    pl->udpLog("chongwufuben", "F_58001", "", "", "", "", "act");
                    break;
                case 2:
                    pl->udpLog("chongwufuben", "F_59001", "", "", "", "", "act");
                    break;
                case 3:
                    pl->udpLog("chongwufuben", "F_510001", "", "", "", "", "act");
                    break;
                case 4:
                    pl->udpLog("chongwufuben", "F_511001", "", "", "", "", "act");
                    break;
                case 5:
                    pl->udpLog("chongwufuben", "F_512001", "", "", "", "", "act");
                    break;
            }
        }
    }

    UInt8 pos = 0;
    UInt8 petClass = pet->getClass();
    switch(petClass)
    {
    case e_cls_xuanwu:
        {
           pos = 1;
        }
        break;
    case e_cls_zhuque:
        {
            pos = 4;
        }
        break;
    case e_cls_qinglong:
        {
            pos = 7;
        }
        break;
    case e_cls_baihu:
        {
            pos = 7;
        }
        break;
    }
    
    PetTeamData* td = new PetTeamData();
    td->id = copyId + (t << 5) + (number << 8);
    td->index = copyId;
    td->type = t;
    td->leader = pl;
    td->quality = quality;
    td->count = 1;
    td->members[0] = pl;
    td->formation[0] = pos;
    td->NPCId = NPCId;
    if(markA)
        td->useMoney = true;

    pl->setPetTeamData(td);
    pl->SetInPTCStatus(true);
    m_allTeams.insert(std::make_pair(td->id, td));

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x06);
    teamInfo(pl, st);
    st << Stream::eos;
    pl->send(st);

    sendPetTeamCopyPageUpdate(0, td->id);

    return td->id;
}

UInt32 PetTeamCopy::joinTeam(Player* pl, UInt32 teamId)
{
    if(pl->hasFlag(GObject::Player::InCopyTeam))
        return 0;

    if(pl == NULL)
        return 0;

    UInt8 level = pl->GetLev();
    if(level < 80)
        return 0;

    UInt8 copyId = teamId & 0x1F;
    UInt8 copyType = (teamId >> 5 & 0x07);

    if(!checkPetTeamCopy(pl, copyId, copyType))
        return 0;

    if(pl->GetInPTCStatus())
    {
		pl->sendMsgCode(1, 2106);
        return 0;
    }

    /*UInt32 now = TimeUtil::Now();
    UInt32 buffLeft = pl->getBuffData(PLAYER_BUFF_ATTACKING, now);
    if(cfg.GMCheck && buffLeft > now)
    {
        pl->sendMsgCode(0, 1407, buffLeft - now);
        return 0;
    }*/

    if(pl->getPetTeamData() != NULL)
        return 0;

    AllTeamsIterator it = m_allTeams.find(teamId);
    if(it == m_allTeams.end() || it->second == NULL)
    {
		pl->sendMsgCode(1, 2102);
        return 0;
    }

    PetTeamData* td = it->second;
    if(td->count >= PETTEAMCOPY_MAXMEMCNT)
    {
		pl->sendMsgCode(1, 2103);
        return 0;
    }

    if(td->start)
    {
		pl->sendMsgCode(1, 8001);
        return 0;
    }

    FairyPet * pet = pl->getBattlePet();
    if(pet == NULL)
        return 0;

    UInt8 pos = 0;
    UInt8 petClass = pet->getClass();
    switch(petClass)
    {
    case e_cls_xuanwu:
        {
            pos = 0;
        }
        break;
    case e_cls_zhuque:
        {
            pos = 3;
        }
        break;
    case e_cls_qinglong:
        {
            pos = 6;
        }
        break;
    case e_cls_baihu:
        {
            pos = 6;
        }
        break;
    }

    UInt8 temp[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for(UInt8 k=0; k<3; k++)
    {  
        bool markA = false;
        for(UInt8 i=0; i<td->count; i++)
        {
            if(td->formation[i] == temp[pos+k])
            {
                markA = true;
                break;
            }
        }
        if(!markA)
        {
            td->formation[td->count] = temp[pos+k];
            break;
        }
    }

    td->members[td->count] = pl;
    ++td->count;
    pl->setPetTeamData(td);

    pl->udpLog("chongwufuben", "F_55000", "", "", "", "", "act");
    /*for(UInt8 k=0; k<td->count; k++)
    {
        std::cout << "Formation A: " <<  static_cast<UInt32>(td->formation[k]) << std::endl;
    }*/

    if(td->count == 3 && !td->mark)
    {
        Player* tempPL = NULL;
        if(td->formation[0] > td->formation[1])
        {
            tempPL = td->members[0];
            td->members[0] = td->members[1];
            td->members[1] = tempPL;
        }
        if(td->formation[0] > td->formation[2])
        {
            tempPL = td->members[0];
            td->members[0] = td->members[2];
            td->members[2] = tempPL;
        }
        if(td->formation[1] > td->formation[2])
        {
            tempPL = td->members[1];
            td->members[1] = td->members[2];
            td->members[2] = tempPL;
        }
        setFormation(td->leader);
    }

    pl->SetInPTCStatus(true);

    PetCopyTeamPage& pctp = pl->getPetCopyTeamPage();
    pctp.copyId = copyId;
    pctp.t = copyType;

    // send team info to members ::TODO
    for(UInt8 j = 0; j < td->count; ++j)
    {
        Stream st(REP::PET_TEAM_COPY);
        st << static_cast<UInt8>(0x07);
        teamInfo(td->members[j], st);
        st << Stream::eos;
        td->members[j]->send(st);
    }

    sendPetTeamCopyPageUpdate(0, td->id);

    quit(pl);

    return teamId;
}

void PetTeamCopy::dismissTeam(Player* pl, UInt8 mark)
{
    if(pl == NULL)
        return;

    PetTeamData* td = pl->getPetTeamData();
    if(td == NULL)
        return;

    if(pl != td->leader)
        return;

    if(!checkPetTeamCopy(pl, td->index, td->type))
        return;

    AllTeamsIterator itC = m_allTeams.find(td->id);
    if(itC != m_allTeams.end())
    {
        m_allTeams.erase(itC);
    }

    recycleTeamNumber(td->index, td->type, td->id >> 8);

    sendPetTeamCopyPageUpdate(1, td->id);

    // send team info to members ::TODO
    for(UInt8 j = 0; j < td->count; ++j)
    {
        td->members[j]->setPetTeamData(NULL);
        td->members[j]->clearPetCopyTeamPage();
        td->members[j]->SetInPTCStatus(false);

        if(0 == mark)
        {
            Stream st(REP::PET_TEAM_COPY);
            st << static_cast<UInt8>(0x12);
            st << Stream::eos;
            td->members[j]->send(st);
        }
    }

    delete td;
}

void PetTeamCopy::leaveTeam(Player* pl, UInt8 mark)
{
    if(pl == NULL)
        return;

    PetTeamData* td = pl->getPetTeamData();
    if(td == NULL)
        return;

    Player * leader = td->leader;
    if(leader == NULL)
        return;
    
    if(leader == pl)
        dismissTeam(pl);
    else
        teamKick(leader, pl->getId(), mark);
   
    if(mark == 3)
        enter(pl);
}

void PetTeamCopy::teamKick(Player* pl, UInt64 playerId, UInt8 mark)
{
    if(mark > 2)
        return;

    if(pl == NULL)
        return;

    Player* member = GObject::globalPlayers[playerId];
    if(NULL == member || pl == member)
        return;

    PetTeamData* td = pl->getPetTeamData();
    if(td == NULL)
        return;

    if(!checkPetTeamCopy(pl, td->index, td->type))
        return;

    if(td->start && mark==1)
        return;

    if(td->leader != pl)
        return;

    bool res = false;
    for(UInt8 i=0; i<td->count; ++i)
    {
        if(td->members[i] == member)
        {
            res = true;
            td->members[i] = NULL;
            td->formation[i] = 0;
            UInt8 j = i+1;
            for(; j<td->count; ++j)
            {
                td->members[j-1] = td->members[j];
                td->formation[j-1] = td->formation[j];
            }
            td->members[j-1] = NULL;
            td->formation[j-1] = 0;
            --td->count;
            break;
        }
    }

    if(!res)
    {
		pl->sendMsgCode(1, 2105);
        return;
    }

    member->clearPetCopyTeamPage();
    member->setPetTeamData(NULL);
    member->SetInPTCStatus(false);
    
    Stream st(REP::PET_TEAM_COPY);
    if(mark == 0)
        st << static_cast<UInt8>(0x09);
    else
        st << static_cast<UInt8>(0x08);
    st << static_cast<UInt8>(0);        //自己
    st << Stream::eos;
    member->send(st);

    // send team info to members ::TODO
    for(UInt8 j = 0; j < td->count; ++j)
    {
        Stream st(REP::PET_TEAM_COPY);
        if(mark == 0)
            st << static_cast<UInt8>(0x09);
        else
            st << static_cast<UInt8>(0x08);
        st << static_cast<UInt8>(1);    //其他成员
        teamInfo(td->members[j], st);
        st << Stream::eos;
        td->members[j]->send(st);
    }

    sendPetTeamCopyPageUpdate(0, td->id);

    if(mark == 0)
        member->sendMsgCode(1, 2111);
}

void PetTeamCopy::setFormation(Player* pl, UInt8 pos1, UInt8 pos2, UInt8 pos3)
{
    if(pl == NULL)
        return;

    PetTeamData* td = pl->getPetTeamData();
    if(td == NULL)
        return;

    if(!checkPetTeamCopy(pl, td->index, td->type))
        return;

    if(0 == pos1 && 0 == pos2 && 0 == pos3)
    {
        UInt8 mark1 = 0;
        UInt8 mark2 = 0;
        UInt8 mark3 = 0;
        for(UInt8 i=0; i<td->count; i++)
        {
            UInt8 petClass = 0;
            FairyPet * pet = td->members[i]->getBattlePet();
            if(pet)
                petClass = pet->getClass();

            switch(petClass)
            {
            case e_cls_xuanwu:
                {
                   mark1++;
                }
                break;
            case e_cls_zhuque:
                {
                    mark2++;
                }
                break;
            case e_cls_qinglong:
                {
                    mark3++;
                }
                break;
            case e_cls_baihu:
                {
                    mark3++;
                }
                break;
            }
        }

        if((mark1 == 3) && (mark2 == 0) && (mark3 == 0))
        {
            for(UInt8 j=0; j<mark1; j++)
            {
                td->formation[j] = j+1; 
            }
        }
        else if((mark2 == 3) && (mark1 == 0) && (mark3 == 0))
        {
            for(UInt8 j=0; j<mark2; j++)
            {
                td->formation[j] = j+4; 
            }
        }
        else if((mark3 == 3) && (mark1 == 0) && (mark2 == 0))
        {
            for(UInt8 j=0; j<mark3; j++)
            {
                td->formation[j] = j+7; 
            }
        }
        else if(((mark1 == 2) && (mark2 == 1 || mark3 == 1)) ||
                (mark1 == 1 && mark2 == 2) ||
                (mark1 == 1 && mark2 == 1 && mark3 == 1))
        {
            td->formation[0] = 2; 
            td->formation[1] = 5; 
            td->formation[2] = 8; 
        }
        else if(((mark2 == 2) && ((mark1 == 1) || (mark3 == 1))) ||
                ((mark2 == 1) && (mark3 == 2)))
        { 
            td->formation[0] = 5; 
            td->formation[1] = 8; 
            td->formation[2] = 9; 
        }
        else if((mark1 == 1) && (mark3 == 2))
        {
            td->formation[0] = 2; 
            td->formation[1] = 8; 
            td->formation[2] = 9; 
        }
       
        /*for(UInt8 k=0; k<td->count; k++)
        {
            std::cout << "Formation B: " << static_cast<UInt32>(td->formation[k]) << std::endl;
        }*/
    }
    else
    {
        if(pl != td->leader)
            return;

        if(pos1 > 9 || pos2 > 9 || pos3 > 9)
            return;

        if(td->formation[0] > 0)
            td->formation[0] = pos1;
        if(td->formation[1] > 0)
            td->formation[1] = pos2;
        if(td->formation[2])
            td->formation[2] = pos3;

        if(!td->mark)
            td->mark = true;

        for(UInt8 j = 0; j < td->count; ++j)
        {
            //std::cout << "Formation C: " << static_cast<UInt32>(td->formation[j]) << std::endl;
            Stream st(REP::PET_TEAM_COPY);
            st << static_cast<UInt8>(0x11);
            teamInfo(td->members[j], st);
            st << Stream::eos;
            td->members[j]->send(st);
        }
    }
}

void PetTeamCopy::inviteFriend(Player* pl, UInt64 friendId)
{
    if(pl == NULL)
        return;

    Player* member = GObject::globalPlayers[friendId];
    if(NULL == member)
        return;

    if(!member->isOnline())
    {
        pl->sendMsgCode(0, 2218);
        return;
    }

    if(!pl->isFriend(member))
        return;

    PetTeamData* td = pl->getPetTeamData();
    if(td == NULL)
        return;

    if(!checkPetTeamCopy(pl, td->index, td->type))
        return;

    if(td->start)
        return;

    if(td->leader != pl)
        return;

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x16);
    st << pl->getId() << pl->getCountry() << pl->getName().c_str() << static_cast<UInt8>(td->index) << static_cast<UInt8>(td->type) << static_cast<UInt32>(td->id) << static_cast<UInt32>(td->NPCId); 
    st << Stream::eos;
    member->send(st);

    SYSMSG_SEND(5130, pl);
}

void PetTeamCopy::refuseJoin(Player* pl, UInt64 leaderId)
{
    if(pl == NULL)
        return;

    Player* leader = GObject::globalPlayers[leaderId];
    if(NULL == leader)
        return;

    if(!pl->isFriend(leader))
        return;

    Stream st(REP::PET_TEAM_COPY);
    st << static_cast<UInt8>(0x17);
    st << pl->getCountry() << pl->getName().c_str(); 
    st << Stream::eos;
    leader->send(st);
}

void PetTeamCopy::reqStart(Player* pl, UInt8 opt)
{
    if(opt != 0 && opt != 1)
        return;

    if(pl == NULL)
        return;

    PetTeamData* td = pl->getPetTeamData();
    if(td == NULL)
        return;

    if(!checkPetTeamCopy(pl, td->index, td->type))
        return;

    if(td->leader != pl)
        return;

    /*UInt32 now = TimeUtil::Now();
    for(int i=0; i<td->count; ++i)
    {
        Player* member = td->members[i];
        UInt32 buffLeft = member->getBuffData(PLAYER_BUFF_ATTACKING, now);
        if(cfg.GMCheck && buffLeft > now)
        {
            member->sendMsgCode(0, 1407, buffLeft - now);
            return;
        }
    }*/

    if(opt == 0)
        td->start = true;
    else
        td->start = false;

    for(int j=0; j<td->count; ++j)
    {
        Stream st(REP::PET_TEAM_COPY);
        st << static_cast<UInt8>(0x15);
        st << opt;
        st << Stream::eos;
        td->members[j]->send(st);
    }
}

void PetTeamCopy::teamBattleStart(Player* pl)
{
    if(pl == NULL)
        return;

    PetTeamData* td = pl->getPetTeamData();
    if(td == NULL)
        return;

    if(!checkPetTeamCopy(pl, td->index, td->type))
        return;

    if(td->leader != pl)
        return;
    
    /*UInt32 now = TimeUtil::Now();
    for(int i=0; i<td->count; ++i)
    {
        Player* member = td->members[i];
        UInt32 buffLeft = member->getBuffData(PLAYER_BUFF_ATTACKING, now);
        if(cfg.GMCheck && buffLeft > now)
        {
            member->sendMsgCode(0, 1407, buffLeft - now);
            return;
        }
    }*/

    for(int j=0; j<td->count; ++j)
    {
        td->members[j]->checkLastBattled();
    }

    Player* member = td->members[0];
    if(!member)
        return;

    UInt32 NPCId = td->NPCId;
    GData::NpcGroups::iterator it = GData::npcGroups.find(NPCId);
    if(it == GData::npcGroups.end())
        return;
    GData::NpcGroup * ng = it->second;
    if (ng == NULL)
        return;
    
    std::vector<GData::NpcFData>& _npcList = ng->getList();
    if (_npcList.size() == 0)
        return;

    GObject::Fighter* monster = _npcList[0].fighter;
    if(monster == NULL)
        return;

    UInt16 bs = 0;
    bs = td->index - 1 + Battle::BS_COPY1;
    Battle::BattleSimulator bsim(bs, member, monster->getName(), monster->getLevel(), false);

    UInt32 rptid = 0;
    for(UInt8 memIdx=0; memIdx<td->count; ++memIdx)
    {
        member = td->members[memIdx];
        if(!member)
            break;
        
        UInt8 pos = 0;
        if(td->formation[memIdx] >= 1 && td->formation[memIdx] <=3)
            pos = td->formation[memIdx] + 5;
        else if(td->formation[memIdx] >= 4 && td->formation[memIdx] <=6)
            pos = td->formation[memIdx] + 7;
        else if(td->formation[memIdx] >= 7 && td->formation[memIdx] <=9)
            pos = td->formation[memIdx] + 9;

        member->PutSpecialPets(bsim, 0, pos);

        if(memIdx+1 < td->count)
        {
            Fighter * fgt = td->members[memIdx+1]->getMainFighter();
            if(NULL == fgt)
                continue;

            bsim.putTeams(fgt->getName(), fgt->getLevel(), fgt->getId(), 0);
        }
    }
    if(td->members[0])
        bsim.setPortrait(0, td->members[0]->getMainFighter()->getId());

    ng->putFighters(bsim);
    //bsim.putTeams(monster->getName(), monster->getLevel(), monster->getPortrait(), 1);

    bsim.start();

    sendBattleReport(td, ng, bsim, rptid);

    if( td->count != 0 )
    {
        for(UInt8 i = 0 ; i < (td->count -1); ++i)
        {
            Player* pl = td->members[i];
            if(pl == NULL)
                continue;
            for(UInt8 j = i + 1 ; j < td->count ; ++j)
            {
                Player* p = td->members[j];
                if(p == NULL)
                    continue;
                if(pl->getThreadId() != p->getThreadId())
                    continue ;
                pl->CompleteFriendlyTask(p,3 ,1);
                p->CompleteFriendlyTask(pl,3 ,1);
            }
        }
    }

    for(UInt8 i = td->count; i > 0; --i)
    {
        Player* pl = td->members[i-1];
        if(pl == NULL)
            continue;

        bsim.applyFighterHP(0, pl);
        GameAction()->doStrong(pl, SthPetCopy, 0,0);
    }
    dismissTeam(td->leader, 1);
}

void PetTeamCopy::sendBattleReport(PetTeamData* td, GData::NpcGroup* ng, Battle::BattleSimulator& bsim, UInt32& rptid)
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

    for(UInt8 i=0; i<td->count; ++i)
    {
        Player* pl = td->members[i];
        if(!pl)
            continue;
        if(ng && res == 1)
        {
            if(pl == td->leader)
            {
                ng->getLoots(pl, pl->_lastLoot, 0, NULL);
                if(td->quality == 5)
                {
                    std::vector<GData::NpcFData>& _npcList = ng->getList();
                    if(_npcList.size() == 0)
                        continue;

                    GObject::Fighter* monster = _npcList[0].fighter;
                    if(monster == NULL)
                        continue;

                    std::vector<UInt32> logits;
                    for(UInt8 k=0; k<pl->_lastLoot.size(); k++)
                    {
                        logits.push_back(pl->_lastLoot[k].id);
                    }
                    if(logits.size() > 0)
                        pushLog(pl->getCountry(), pl->getName(), 3, monster->getName(), logits);
                }
                
                if(td->useMoney)
                {
                    if(td->type == ORDINARYCOPY)
                        pl->AddVar(VAR_ORDINARY_KILL_NUM, 1);
                    else
                        pl->AddVar(VAR_DIFFICULTY_KILL_NUM, 1);
                }

                UInt8 mark = pl->GetVar(VAR_PETTEAMCOPY_BATTLE_RES);
                if(1 == GET_BIT(mark, td->type))
                {
                    mark = CLR_BIT(mark, td->type);
                    pl->SetVar(VAR_PETTEAMCOPY_BATTLE_RES, mark);
                }
                addMonster(pl, td->index, td->type, td->NPCId);
            }
            else
            {
                if(pl->GetVar(VAR_HELP_NUM) < 2)
                    ng->getLoots(pl, pl->_lastLoot, 1, NULL);

                pl->AddVar(VAR_HELP_NUM, 1);
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

}



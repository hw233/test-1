#include "Config.h"
#include "ArenaTeam.h"
#include "Player.h"
#include "MsgID.h"
#include "GData/Money.h"
#include "GData/TeamArenaSkill.h"
#include "GObjectDBExecHelper.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "Server/OidGenerator.h"
#include "Server/SysMsg.h"
#include "ArenaServerWar.h"

namespace GObject
{


#define GET_PROGRESS_NAME(n, p) char n[1024]; { SysMsgItem * mi = globalSysMsg[781 + p]; if(mi != NULL) mi->get(n); else n[0] = 0; }


const static UInt8 progress_accept[6][11] = {
  // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},   // 0
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // 1
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},   // 2
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},   // 3
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},   // 4
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},   // 5
};

const static UInt32 ArenaLastAwards[2][6][2] = {
    {
        {9063, 6000},
        {9062, 7000},
        {9061, 8000},
        {9060, 10000},
        {9059, 12000},
        {9058, 15000}
    },
    {
        {9066, 1000},
        {9066, 1500},
        {9065, 2000},
        {9065, 3000},
        {9064, 4000},
        {9063, 5000}
    }
};

bool team_enum_send_status(void * ptr, void * data )
{
    Player* player = static_cast<Player*>(ptr);
    if(player == NULL)
        return true;

    GObject::teamArenaMgr.sendStatus(player);
    return true;
}

bool team_enum_send_active(void * ptr, void * data )
{
    Player* player = static_cast<Player*>(ptr);
    if(player == NULL)
        return true;

    GObject::teamArenaMgr.sendActive(player);
    return true;
}

inline void GET_ORIGINID(UInt64& pid, UInt32 sid, UInt32 cid)
{
    if((cid != (UInt32)cfg.channelNum || sid != (UInt32)cfg.serverNo) && (pid >> 48) == 0)
        pid = pid | (static_cast<UInt64>(sid) << 48) | (static_cast<UInt64>(cid) << 40);
}

void TeamArenaData::loadSkillFromDB(DBTeamArenaSkill& dbtas)
{
    if(dbtas.skillId <= 0)
        return;
	teamskills.insert(std::make_pair(dbtas.skillId, TeamSkillData(dbtas.skillId, dbtas.level, dbtas.extra)));
}

void TeamArenaData::buildTeamSkill()
{
    UInt8 skillNum = GData::teamArenaSkill.size();
	for (UInt8 id = 1; id < skillNum; ++ id)
    {
        TeamSkills::iterator it = teamskills.find(id);
        if(it != teamskills.end())
            return;

        teamskills[id] = TeamSkillData(id, 0, 0);
        updateSkillToDB(&teamskills[id]);
    }
}

bool TeamArenaData::checkTimeOver(UInt32 now)
{
    if(now < overTime)
        return false;
    UInt32 over = TimeUtil::SharpWeek(1 , now);
    overTime = over;
    inspireLvl = 0;
    updateToDB();
    return true;
}

UInt8 TeamArenaData::getLastRank()
{
    if(maxRank)
        teamArenaMgr.resetTeamLastRank(this);
    return lastRank;
}

void TeamArenaData::appendListInfo(Stream& st)
{
    st << getName() << GetLev() << count;
    if(leader)
        st << leader->getName();
    else
        st << "";
    for(UInt8 i = 0; i < count; ++ i)
    {
        if(members[i])
        {
            st << static_cast<UInt8>(members[i]->getMainFighter()->getId());
            st << members[i]->getName() << members[i]->getCountry() << members[i]->getBattlePoint();
        }
        else
            st << static_cast<UInt8>(0) << "" << static_cast<UInt8>(0) << static_cast<UInt32>(0);
    }
}

void TeamArenaData::broadcastTeam(Stream& st)
{
    for(UInt8 i = 0; i < count; ++ i)
    {
        if(members[i])
            members[i]->send(st);
    }
}

void TeamArenaData::purgePending()
{
	UInt32 thatTime = TimeUtil::Now() - 7 * 3600 * 24;
	size_t i = 0;
    std::map<UInt64, TeamPendingMember>::iterator it = pendingMap.begin();
	while(i < pendingMap.size())
	{
        if((it->second).opTime < thatTime)
        {
		    DB1().PushUpdateData("DELETE FROM `team_pending_player` WHERE `teamId` = %" I64_FMT "u AND `playerId` = %" I64_FMT "u", getId(), (it->second).player->getId());
            pendingMap.erase(it ++ );
        }
		++ i;
	}
}

void TeamArenaData::broadcastPendingMemberInfo(TeamPendingMember& tpm)
{
	Stream st(REP::SERVER_ARENA_OP);
	st << static_cast<UInt8>(18) << static_cast<UInt8>(1);
	st << static_cast<UInt8>(1);
    st << tpm.player->getId() << tpm.player->getName();
    st << tpm.player->GetLev() << tpm.opTime << tpm.player->getPF();
	st << Stream::eos;
    broadcastTeam(st);
}

void TeamArenaData::updateSkillToDB(TeamSkillData * tsd)
{
    if(!tsd)
        return;
    DB1().PushUpdateData("REPLACE INTO `arena_team_skill`(`teamId`, `skillId`, `level`, `extra`) VALUES(%" I64_FMT "u, %u, %u, %u)", getId(), tsd->skillId, tsd->level, tsd->extra);
}

void TeamArenaData::updateToDB()
{
    if(!leader)
        return;

    std::string playerIdStr, stampStr, scoreStr;
    for(UInt8 i = 0; i < TEAMARENA_MAXMEMCNT; ++ i)
    {
        playerIdStr += Itoa(members[i] ? members[i]->getId() : 0);
        stampStr += Itoa(stamps[i]);
        scoreStr += Itoa(scores[i]);
        if(i < TEAMARENA_MAXMEMCNT - 1)
        {
            playerIdStr += ",";
            stampStr += ",";
            scoreStr += ",";
        }
    }
	DB1().PushUpdateData("REPLACE INTO `arena_team`(`id`, `name`, `leader`, `score`, `members`, `stamps`, `memScore`, `maxRank`, `inspireLvl`, `overTime`)\
            VALUES(%" I64_FMT "u, '%s', %" I64_FMT "u, %u, '%s', '%s', '%s', %u, %u, %u)", getId(), getName().c_str(),
            leader->getId(), score, playerIdStr.c_str(), stampStr.c_str(), scoreStr.c_str(), maxRank, inspireLvl, overTime);
}


UInt8 TeamEliminationBattle::winner(UInt8 idx)
{
	UInt8 w = 0;
    const UInt8 round[5] = { 3, 3, 5, 5, 7 };
	const UInt8 oflag[7] = {1, 2, 4, 8, 16, 32, 64};
	for(int i = 0; i < round[idx]; ++ i)
		if(wonFlag & oflag[i])
			++ w;
	if((w << 1) > round[idx])
		return 1;
	return 0;
}

UInt8 TeamEliminationBattle::winCount(UInt8 idx)
{
	UInt8 w = 0;
    const UInt8 round[5] = { 3, 3, 5, 5, 7 };
	const UInt8 oflag[7] = {1, 2, 4, 8, 16, 32, 64};
	for(int i = 0; i < round[idx]; ++ i)
		if((wonFlag & oflag[i]) == 0)
			++ w;
	return w;
}

/********** 组队跨服战 **********/
GlobalTeamArenaData globalTeamArena;
GlobalNamedTeamArenaData globalNamedTeamArena;
TeamArenaMgr teamArenaMgr;

void TeamArenaMgr::loadFromDB(DBTeamArenaData& dbtad)
{
    if(dbtad.id <= 0)
        return;
    TeamArenaData * tad = new(std::nothrow) TeamArenaData;
    if(!tad)
        return;
    tad->id = dbtad.id;
    tad->name = dbtad.name;
    tad->score = dbtad.score;
    tad->level = tad->countLevel(dbtad.score);
    tad->maxRank = dbtad.maxRank;
    tad->inspireLvl = dbtad.inspireLvl;
    tad->overTime = dbtad.overTime;
    Player * player = globalPlayers[dbtad.leader];
    tad->leader = player;

    StringTokenizer tk(dbtad.stamps, ",");
    for(UInt8 i = 0; i < tk.count() && i < TEAMARENA_MAXMEMCNT; ++ i)
    {
        tad->stamps[i] = atoi(tk[i].c_str());
    }
    StringTokenizer tk1(dbtad.memScore, ",");
    for(UInt8 i = 0; i < tk1.count() && i < TEAMARENA_MAXMEMCNT; ++ i)
    {
        tad->scores[i] = atoi(tk1[i].c_str());
    }
    StringTokenizer tk2(dbtad.members, ",");
    for(UInt8 i = 0; i < tk2.count() && i < TEAMARENA_MAXMEMCNT; ++ i)
    {
        player = globalPlayers[atoll(tk2[i].c_str())];
        tad->members[i] = player;
        if(player)
        {
            ++ tad->count;
            player->setTeamArena(tad);
        }
    }
    if(!tad->count)
    {
        DB1().PushUpdateData("DELETE FROM `arena_team` WHERE `id` = %" I64_FMT "u", tad->getId());
        DB1().PushUpdateData("DELETE FROM `arena_team_skill` WHERE `teamId` = %" I64_FMT "u", tad->getId());
        delete tad;
        return;
    }
    if(!tad->leader)
    {
        for(UInt8 i = 0; i < tad->count; ++ i)
        {
            if(tad->members[i])
            {
                tad->leader = tad->members[i];
                break;
            }
        }
    }

    globalNamedTeamArena.add(dbtad.name, tad);
    globalTeamArena.add(tad);
}

void TeamArenaMgr::pushBetFromDB(Player * player, DBArenaBet& dbab)
{
    if(!player || dbab.state >= 7 || dbab.group > 2)
        return;
	BetInfo binfo;
	binfo.state = dbab.state;
	binfo.round = dbab.round;
	binfo.group = dbab.group;
	binfo.recieved = dbab.recieved;
	binfo.pos = dbab.pos;
	binfo.type = dbab.tael;
    int i = 0;
    if(dbab.group > 0)
        i = dbab.group - 1;
	_playerBet[player].betList[dbab.state][i].push_back(binfo);
}

bool TeamArenaMgr::createTeam(Player * leader, std::string& name)
{
    if(!leader || leader->GetLev() < LIMIT_LEVEL || leader->getTeamArena())
        return false;
    if(leader->GetVar(VAR_TEAMARENA_CREATE))
    {
        leader->sendMsgCode(0, 1338);
        return false;
    }
    if(name.empty() || name.length() > 15 || (strncmp(name.c_str(), "ERROR_", 6) == 0))
        return false;
    name = leader->fixName(name);
    if(globalNamedTeamArena[name])
    {
        leader->sendMsgCode(0, 1332);
        return false;
    }
    UInt32 money = GData::moneyNeed[GData::ARENATEAM_CREATE].tael;
	if(leader->getTael() < money)
    {
        leader->sendMsgCode(0, 1100);
        return false;
    }
    UInt64 id = IDGenerator::gTeamArenaOidGenerator.ID();
    if(cfg.merged)
        id |= (static_cast<UInt64>(cfg.serverNo) << 48);
    while(globalTeamArena[id] != NULL)
    {
        id = IDGenerator::gTeamArenaOidGenerator.ID();
        if(cfg.merged)
            id |= (static_cast<UInt64>(cfg.serverNo) << 48);
    }
    TeamArenaData * tad = new(std::nothrow) TeamArenaData;
    if(!tad)
        return false;
    tad->id = id;
    tad->leader = leader;
    tad->name = name;
    tad->members[tad->count] = leader;
    tad->stamps[tad->count] = uRand(TEAMARENA_STAMP_MAX-1) + 1;
    ++ tad->count;
    tad->updateToDB();
    tad->buildTeamSkill();
    leader->setTeamArena(tad);

    globalNamedTeamArena.add(name, tad);
    globalTeamArena.add(tad);
    sendTeamInfo(tad);
    leader->AddVar(VAR_TEAMARENA_CREATE, 1);
    sendReqInfo(leader, 10);
    SYSMSG_SENDV(179, leader);
    SYSMSG_SENDV(1079, leader, tad->getName().c_str());

    GameMsgHdr hdr(0x336, leader->getThreadId(), leader, sizeof(money));
    GLOBAL().PushMsg(hdr, &money);
    return true;
}

void TeamArenaMgr::dismissTeam(Player * leader)
{
    if(!leader)
        return;
    TeamArenaData * tad = leader->getTeamArena();
    if(!tad || tad->leader != leader || tad->isInArena() || tad->count > 1)
        return;
    for(UInt8 i = 0; i < TEAMARENA_MAXMEMCNT; ++ i)
    {
        if(tad->members[i])
            tad->members[i]->setTeamArena(NULL);
    }
    globalTeamArena.remove(tad->getId());
    globalNamedTeamArena.remove(tad->getName());
    DB1().PushUpdateData("DELETE FROM `arena_team` WHERE `id` = %" I64_FMT "u", tad->getId());
    DB1().PushUpdateData("DELETE FROM `arena_team_skill` WHERE `teamId` = %" I64_FMT "u", tad->getId());
    DB1().PushUpdateData("DELETE FROM `team_pending_player` WHERE `teamId` = %" I64_FMT "u", tad->getId());
    SYSMSG_SENDV(154, leader);
    SYSMSG_SENDV(1054, leader, tad->getName().c_str());

    delete tad;
    sendReqInfo(leader, 6);
}

bool TeamArenaMgr::addTeamMember(Player * leader, Player * player)
{
    if(!leader || !player || player->getTeamArena())
        return false;
    if(player->GetLev() < LIMIT_LEVEL)
    {
        player->sendMsgCode(0, 2010, LIMIT_LEVEL);
        return false;
    }
    TeamArenaData * tad = leader->getTeamArena();
    if(!tad || tad->leader != leader)
        return false;
    if(tad->isInArena())
    {
        player->sendMsgCode(0, 1339);
        return false;
    }
    if(tad->isFull())
    {
        player->sendMsgCode(0, 1335);
        return false;
    }
    for(UInt8 i = 0; i < TEAMARENA_MAXMEMCNT; ++ i)
    {
        if(tad->members[i] == player)
            return false;
    }
    tad->members[tad->count] = player;
    tad->stamps[tad->count] = uRand(TEAMARENA_STAMP_MAX-1) + 1;
    ++ tad->count;
    player->setTeamArena(tad);
    tad->updateToDB();
    sendTeamInfo(tad);

    if(player->isOnline())
    {
        SYSMSG_SENDV(117, player, tad->getName().c_str());
        SYSMSG_SENDV(1017, player, tad->getName().c_str());
    }
    return true;
}

void TeamArenaMgr::fireTeamMember(Player * leader, UInt64 playerId)
{
    if(!leader)
        return;
    TeamArenaData * tad = leader->getTeamArena();
    if(!tad || tad->leader != leader || tad->count <= 1)
        return;
    if(tad->isInArena())
    {
        leader->sendMsgCode(0, 1339);
        return;
    }
    Player* player = globalPlayers[playerId];
    if(!player || leader == player)
        return;
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        if(tad->members[i] == player)
        {
            UInt8 j = i + 1;
            for(; j < tad->count; ++ j)
            {
                tad->members[j-1] = tad->members[j];
                tad->stamps[j-1] = tad->stamps[j];
                tad->scores[j-1] = tad->scores[j];
            }
            tad->members[j-1] = NULL;
            tad->stamps[j-1] = TEAMARENA_NONE;
            tad->scores[j-1] = 0;
            player->setTeamArena(NULL);
            -- tad->count;
            break;
        }
    }
    tad->updateToDB();
    if(player->isOnline())
    {
        SYSMSG_SENDV(153, player);
        SYSMSG_SENDV(1053, player, leader->getCountry(), leader->getName().c_str(), tad->getName().c_str());
    }

    sendTeamInfo(tad);
    sendReqInfo(player, 6);
}

void TeamArenaMgr::handoverLeader(Player * leader, UInt64 playerId)
{
    if(!leader)
        return;
    TeamArenaData * tad = leader->getTeamArena();
    if(!tad || tad->leader != leader || tad->count <= 1)
        return;
    if(tad->isInArena())
    {
        leader->sendMsgCode(0, 1339);
        return;
    }
    Player* member = globalPlayers[playerId];
    if(NULL == member || leader == member)
        return;

    bool find = false;
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        if(tad->members[i] == member)
        {
            find = true;
            break;
        }
    }
    if(!find)
        return;
    member->setTeamArena(tad);
    tad->leader = member;
    tad->updateToDB();

    SYSMSG_SENDV(1077, leader);
    if(member->isOnline())
        SYSMSG_SENDV(1078, member, tad->getName().c_str());

	Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(7);
    st << playerId;
    st << Stream::eos;
    tad->broadcastTeam(st);
}

void TeamArenaMgr::setMemberPosition(Player * leader, UInt64 playerId1, UInt64 playerId2, UInt64 playerId3, std::string& stampStr)
{
    if(!leader)
        return;
    TeamArenaData * tad = leader->getTeamArena();
    if(!tad || tad->leader != leader || !tad->isFull())
        return;
    StringTokenizer stk(stampStr, ",");
    if(stk.count() != TEAMARENA_MAXMEMCNT)
        return;
    for(UInt8 i = 0; i < stk.count(); ++ i)
    {
        if(atoi(stk[i].c_str()) <= TEAMARENA_NONE || atoi(stk[i].c_str()) >= TEAMARENA_STAMP_MAX)
            return;
    }
    if(playerId1 == playerId2 || playerId1 == playerId3 || playerId2 == playerId3)
        return;
    UInt32 tmpScore[TEAMARENA_MAXMEMCNT] = {0};
    Player * temPlayer[TEAMARENA_MAXMEMCNT] = {NULL};
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        if(tad->members[i] && tad->members[i]->getId() == playerId1)
        {
            tmpScore[0] = tad->scores[i];
            temPlayer[0] = tad->members[i];
        }
        else if(tad->members[i] && tad->members[i]->getId() == playerId2)
        {
            tmpScore[1] = tad->scores[i];
            temPlayer[1] = tad->members[i];
        }
        else if(tad->members[i] && tad->members[i]->getId() == playerId3)
        {
            tmpScore[2] = tad->scores[i];
            temPlayer[2] = tad->members[i];
        }
        else
            return;
    }
    for(UInt8 i = 0; i < TEAMARENA_MAXMEMCNT; ++ i)
    {
        tad->members[i] = temPlayer[i];
        tad->scores[i] = tmpScore[i];
        tad->stamps[i] = atoi(stk[i].c_str());
    }
    tad->updateToDB();

	Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(9);
    st << tad->count;
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        Player * pl = tad->members[i];
        if(pl)
        {
            st << pl->getId() << pl->getName() << static_cast<UInt8>(pl->getMainFighter()->getId());
            st << tad->stamps[i] << tad->scores[i];
            st << pl->GetLev() << pl->getBattlePoint();
        }
        else
        {
            st << static_cast<UInt64>(0) << "" << static_cast<UInt8>(0);
            st << static_cast<UInt8>(0) << static_cast<UInt32>(0);
            st << static_cast<UInt8>(0) << static_cast<UInt32>(0);
        }
    }
    st << Stream::eos;
    tad->broadcastTeam(st);
    //自动同步到跨服战
	GameMsgHdr hdr(0x335, tad->leader->getThreadId(), tad->leader, sizeof(TeamArenaData *));
	GLOBAL().PushMsg(hdr, &tad);
}

void TeamArenaMgr::leaveTeamArena(Player * player)
{
    if(!player)
        return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad || tad->leader == player)
        return;
    if(tad->isInArena())
    {
        player->sendMsgCode(0, 1339);
        return;
    }
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        if(tad->members[i] == player)
        {
            UInt8 j = i + 1;
            for(; j < tad->count; ++ j)
            {
                tad->members[j-1] = tad->members[j];
                tad->stamps[j-1] = tad->stamps[j];
                tad->scores[j-1] = tad->scores[j];
            }
            tad->members[j-1] = NULL;
            tad->stamps[j-1] = TEAMARENA_NONE;
            tad->scores[j-1] = 0;
            player->setTeamArena(NULL);
            -- tad->count;
            break;
        }
    }
    SYSMSG_SENDV(1076, player, tad->getName().c_str());
    tad->updateToDB();
    sendTeamInfo(tad);
    sendReqInfo(player, 6);
}

void TeamArenaMgr::upgradeTeamSkill(Player * player, UInt8 skillId, UInt32 tael)
{
    if(!player || !tael || !skillId || skillId > GData::teamArenaSkill.size())
        return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad)
        return;
    TeamSkillData * tsd = tad->getOneSkill(skillId);
    if(!tsd)
        return;
	GData::SingleTeamArenaSkill& skillTable = GData::teamArenaSkill[skillId];
	UInt8 maxLev = static_cast<UInt8>(skillTable.size()) - 1;
    if(skillTable.size() <= 0 || tsd->level >= maxLev)
    {
        player->sendMsgCode(0, 1333);
        return;
    }
    if(tad->GetLev() < skillTable[tsd->level+1].teamLev)
    {
        player->sendMsgCode(0, 1334);
        return;
    }
	if(player->getTael() < tael)
    {
        player->sendMsgCode(0, 1100);
        return;
    }
    bool isMax = false;
    UInt32 useTael = 0;
    UInt32 tmpTael = tsd->extra + tael;
    while(tsd->level < maxLev && tmpTael >= skillTable[tsd->level+1].needs
            && tad->GetLev() >= skillTable[tsd->level+1].teamLev)
    {
        ++ tsd->level;
        tmpTael -= skillTable[tsd->level].needs;
        if(tsd->level == maxLev)
            isMax = true;
    }
    if(isMax)
    {   //达到技能等级上限
        useTael = tael - tmpTael;
        tsd->extra = 0;
    }
    else
    {
        if(tad->GetLev() < skillTable[tsd->level+1].teamLev)
        {   //战队等级限制升级
            useTael = tael - tmpTael;
            tsd->extra = 0;
        }
        else
        {
            useTael = tael;
            tsd->extra = tmpTael;
        }
    }
    tad->updateSkillToDB(tsd);

    GameMsgHdr hdr(0x336, player->getThreadId(), player, sizeof(useTael));
    GLOBAL().PushMsg(hdr, &useTael);

	Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(8);
    st << tsd->skillId << tsd->level << tsd->extra;
    st << Stream::eos;
    tad->broadcastTeam(st);
}

void TeamArenaMgr::inspireTeam(Player * player)
{
    if(!player)
        return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad || !tad->isInArena() || _progress <= e_team_32)
        return;
    tad->checkTimeOver();
    if(tad->inspireLvl >= GData::teamInspireMap.size())
        return;
    GData::TeamInspireMap::iterator it = GData::teamInspireMap.find(tad->inspireLvl+1);
    if(it == GData::teamInspireMap.end())
        return;
    UInt32 gold = it->second.consume;
    if(player->getGold() < gold)
    {
        player->sendMsgCode(0, 1104);
        return;
    }
    UInt32 rate = 0;
    if(tad->inspireLvl+1 == 9)
    {
        const static UInt32 RATE[5] = { 500, 1000, 1500, 2000, 5000 };
        UInt32 failTimes = player->GetVar(VAR_TEAMARENA_INSPIRE);
        failTimes = failTimes > 4 ? 4 : failTimes;
        rate = RATE[failTimes];
    }
    else if(tad->inspireLvl+1 == 10)
    {
        const static UInt32 RATE[10] = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 5500 };
        UInt32 failTimes = player->GetVar(VAR_TEAMARENA_INSPIRE);
        failTimes = failTimes > 9 ? 9 : failTimes;
        rate = RATE[failTimes];
    }
    else
        rate = it->second.rate;
    if(rate > uRand(10000))
    {
        SYSMSG_SENDV(789, player);
        if(tad->inspireLvl+1 == 9 || tad->inspireLvl+1 == 10)
            player->SetVar(VAR_TEAMARENA_INSPIRE, 0);
        ++ tad->inspireLvl;
        tad->updateToDB();
        GameMsgHdr hdr(0x339, player->getThreadId(), player, sizeof(TeamArenaData *));
        GLOBAL().PushMsg(hdr, &tad);
    }
    else
    {
        SYSMSG_SENDV(788, player);
        if(tad->inspireLvl+1 == 9 || tad->inspireLvl+1 == 10)
            player->AddVar(VAR_TEAMARENA_INSPIRE, 1);
    }

    GameMsgHdr hdr(0x337, player->getThreadId(), player, sizeof(gold));
    GLOBAL().PushMsg(hdr, &gold);

	Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(15);
    st << tad->getId() << tad->inspireLvl;
    st << Stream::eos;
    tad->broadcastTeam(st);
}

void TeamArenaMgr::enterArena(Player * player)
{
    if(_progress != e_team_sign || !isOpen() || player->GetLev() < LIMIT_LEVEL)
        return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad || tad->isInArena() || tad->leader != player || !tad->isFull())
        return;

    struct TeamEnterData {
        Stream st;
        TeamArenaData* tad;
        UInt8 memidx;

        TeamEnterData(Stream& st2, TeamArenaData* tad2) : st(st2), tad(tad2), memidx(0) {}
    };

    Stream st(ARENAREQ::TEAMARENA_ENTER, 0xEF);
    st << tad->getId() << tad->getName() << tad->count;
    st << tad->leader->getId() << tad->level << tad->inspireLvl;

    TeamEnterData * ted = new TeamEnterData(st, tad);
    Player* mem = tad->members[ted->memidx];
    if(!mem)
    {
        delete ted;
        return;
    }
	GameMsgHdr hdr(0x334, mem->getThreadId(), mem, sizeof(TeamEnterData*));
	GLOBAL().PushMsg(hdr, &ted);
}

void TeamArenaMgr::teamArenaEntered( TeamArenaData * tad, UInt8 group, const std::string& rname )
{
    if(!tad) return;
	TeamArenaPlayer& ap = _teams[tad];
	if(ap.group == 0xFF && group <= 3)
		++ _teamsCount[0];
	ap.group = group;
	ap.realName = rname;

    tad->setArenaState(GObject::TEAMARENA_ENTER);
    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(13);
    st << tad->inArena << Stream::eos;
    tad->broadcastTeam(st);
}


void TeamArenaMgr::commitLineup1(Player * player)
{
    if(!player || !isOpen())
        return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad || !tad->isInArena())
        return;
    bool final = false;
    bool preliminary = false;
    int endi = 0;
    int round = 0;
    switch(_progress)
    {
    case e_team_sign:
        {
            TeamArenaPlayerMap::iterator it = _teams.find(tad);
            if(it == _teams.end())
                return;
        }
        break;
    case e_team_32:
    case e_team_sign_end:
        preliminary = true;
        break;
    case e_team_16:
    case e_team_32_end:
        final = true; endi = 32; round = 0;
        break;
    case e_team_8:
        final = true; endi = 16; round = 1;
        break;
    case e_team_4:
        final = true; endi = 8; round = 2;
        break;
    case e_team_2:
        final = true; endi = 4; round = 3;
        break;
    case e_team_1:
        final = true; endi = 2; round = 4;
        break;
    default:
        return;
    }
    if(final)
    {
        bool find = false;
        for(int i = 0; i < 2; ++ i)
        {
            for(int j = 0; j < endi; ++ j)
            {
                UInt8 nidx = _finalIdx[i][round][j];
                if(_finals[i][nidx].id == tad->getId())
                {
                    find = true;
                    break;
                }
            }
            if(find)
                break;
        }
        if(!find)
            return;
    }
    else if(preliminary)
    {
        TeamPreliminaryPlayerListMap::iterator it = _preliminaryPlayers.find(tad->getId());
        if(it == _preliminaryPlayers.end())
            return;
    }

	GameMsgHdr hdr(0x252, player->getThreadId(), player, 1);
	UInt8 data = 1;
	GLOBAL().PushMsg(hdr, &data);
}

void TeamArenaMgr::commitLineup(Player * player)
{
    if(!player || arena.active())
        return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad || !tad->isInArena() || tad->leader != player || tad->count < TEAMARENA_MAXMEMCNT)
        return;
    if(player->inArenaTeamCommitCD())
    {
        Stream st(REP::SERVER_ARENA_OP);
        st << static_cast<UInt8>(4) << static_cast<UInt8>(2) << Stream::eos;
        player->send(st);
        return;
    }
	GameMsgHdr hdr(0x335, player->getThreadId(), player, sizeof(TeamArenaData *));
	GLOBAL().PushMsg(hdr, &tad);
}

//冠军崇拜
void TeamArenaMgr::championWorship(Player* player, UInt8 opt)
{
    if(player == NULL) return;
    if(player->GetVar(VAR_TEAMARENA_WORSHIP))
        return;
    bool has = false;
    if(arena.active())  //仙界第一
        has = serverWarMgr.hasLastLeaderBoard();
    else if(active())  //仙界至尊
        has = arena.hasLastLeaderBoard();
    else    //仙界传奇
    {   //相互交换
        std::map<UInt16, LeaderTeam>::iterator it = _leaderBoard.find(_session-1);
        if(it != _leaderBoard.end())
            has = true;
    }
    if(has == false)
        return;
    UInt32 arenaMoney = 0;
    UInt8 value = 0;
    if(opt == 0)
    {
        arenaMoney = 50;
        value = 1;
    }
    else
    {
        UInt32 gold = 5;
        if(player->getGold() < gold)
        {
            player->sendMsgCode(0, 1104);
            return;
        }
        arenaMoney = 100;
        value = 2;
        GameMsgHdr hdr(0x337, player->getThreadId(), player, sizeof(gold));
        GLOBAL().PushMsg(hdr, &gold);
    }
    player->SetVar(VAR_TEAMARENA_WORSHIP, value);

    GameMsgHdr hdr(0x338, player->getThreadId(), player, sizeof(arenaMoney));
    GLOBAL().PushMsg(hdr, &arenaMoney);

    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(11);
    st << value << Stream::eos;
    player->send(st);
}

void TeamArenaMgr::resetTeamState(TeamArenaData * tad)
{
    if(!tad) return;
    UInt64 tid = tad->getId();
    int progress = 0;
    bool isEntered = false, isInrank = false;
    TeamPreliminaryPlayerListMap::iterator pit = _preliminaryPlayers.find(tid);
    if(pit != _preliminaryPlayers.end())
        isEntered = true;
    switch(_progress)
    {
    case e_team_32_end:
    case e_team_16:
        progress = 0;
        break;
    case e_team_8:
        progress = 1;
        break;
    case e_team_4:
        progress = 2;
        break;
    case e_team_2:
        progress = 3;
        break;
    case e_team_1:
        progress = 4;
        break;
    case e_team_end:
    case e_team_nextbegin:
        tad->setArenaState(TEAMARENA_UNENTER);
        //progress = 5;
        return;
    case e_team_sign:
    case e_team_sign_end:
        if(!isEntered)
        {
            TeamArenaPlayerMap::iterator it = _teams.find(tad);
            if(it != _teams.end())
                isEntered = true;
        }
        if(isEntered)
            tad->setArenaState(TEAMARENA_ENTER);
        else
            tad->setArenaState(TEAMARENA_UNENTER);
        return;
    case e_team_32:
        if(isEntered)
            tad->setArenaState(TEAMARENA_ENTER);
        else
            tad->setArenaState(TEAMARENA_UNENTER);
        return;
    default:
        return;
    }

    for(int i = 0; i < 2; ++ i)
    {
        for(int j = 0; j < 32; ++ j)
        {
            UInt8 idx = _finalIdx[i][progress][j];
            if(idx == 0 && j > 0)
                break;
            if(_finals[i][idx].id == tid)
            {
                tad->inArena = TEAMARENA_ENTER;
                isInrank = true;
                break;
            }
        }
        if(isInrank)
            break;
    }

    if(isInrank)
        tad->setArenaState(TEAMARENA_ENTER);
    else if(isEntered && !isInrank)
        tad->setArenaState(TEAMARENA_OBSOLETE);
    else
        tad->setArenaState(TEAMARENA_UNENTER);
}

void TeamArenaMgr::addTeamScore(TeamArenaData * tad, UInt8 round, bool isWin)
{
    if(!tad || !tad->leader || round > 5)
        return;
    //round:0=进32强 1=进16强 2=进8强 3=进4强 4=进半决赛 5=进决赛
    const static UInt32 winScore[6][2] = { {5,10}, {5,15}, {8,20}, {10,25}, {15,30}, {20,35} };
    UInt32 score = 0;
    UInt8 level = tad->GetLev();
    if(isWin)
        score = winScore[round][1];
    else
        score = winScore[round][0];
    tad->addScore(score);
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        tad->scores[i] += score;
        if(tad->members[i] && tad->members[i]->isOnline())
        {
            SYSMSG_SENDV(131, tad->members[i], score);
            SYSMSG_SENDV(1031, tad->members[i], score);
        }
    }
    tad->updateToDB();
    if(tad->GetLev() != level)
    {
        GameMsgHdr hdr(0x339, tad->leader->getThreadId(), tad->leader, sizeof(TeamArenaData *));
        GLOBAL().PushMsg(hdr, &tad);
    }
}

void TeamArenaMgr::getTeamInfo(Player * player)
{
    if(!player)
        return;
    TeamArenaData * tad = player->getTeamArena();
    if(tad)
    {
        resetTeamState(tad);
        sendTeamInfo(tad, player);
    }
    else
        sendReqInfo(player, 12);
}

void TeamArenaMgr::sendTeamInfo(TeamArenaData * tad, Player * player)
{
    if(!tad || !tad->leader)
        return;
    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(5) << tad->getId() << tad->getName();
    st << tad->score << tad->leader->getId();
    st << tad->inArena << tad->inspireLvl;
    st << tad->getLastRank() << static_cast<UInt8>(tad->maxRank);
    st << static_cast<UInt8>(tad->teamskills.size());
    for(TeamSkills::iterator it = tad->teamskills.begin(); it != tad->teamskills.end(); ++ it)
    {
        st << it->first << it->second.level << it->second.extra;
    }
    st << tad->count;
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        Player * pl = tad->members[i];
        if(pl)
        {
            st << pl->getId() << pl->getName() << static_cast<UInt8>(pl->getMainFighter()->getId());
            st << tad->stamps[i] << tad->scores[i];
            st << pl->GetLev() << pl->getBattlePoint();
        }
        else
        {
            st << static_cast<UInt64>(0) << "" << static_cast<UInt8>(0);
            st << static_cast<UInt8>(0) << static_cast<UInt32>(0);
            st << static_cast<UInt8>(0) << static_cast<UInt32>(0);
        }
    }
    st << Stream::eos;
    if(player)
    {
        player->send(st);
        return;
    }

    tad->broadcastTeam(st);
}

void TeamArenaMgr::sendReqInfo(Player * player, UInt8 type)
{
    if(!player)
        return;
    Stream st(REP::SERVER_ARENA_OP);
    st << type;
    st << Stream::eos;
    player->send(st);
}

void TeamArenaMgr::pushPreliminary(BinaryReader& br)
{
    UInt8 won = 0;
    UInt8 type = 0;
    UInt8 heroId = 0;
    std::string name;
    UInt32 btime = 0;
    UInt64 tid = 0;
    UInt32 cid = 0;
    UInt32 sid = 0;

	br >> cid >> sid >> tid >> won >> type >> heroId >> name >> btime;
    if(type < 2)
        return;
    GET_ORIGINID(tid, sid, cid);

    TeamPlayerBattleReport tpbr;
    tpbr.readReport(br);

    UInt8 old_type = type;
    if(type > 2)
    {
        type = 2;
    }

    GET_PROGRESS_NAME(p, type-1);

    if(won == 3 || won == 4 || won == 2)
    {
        bool flag[5] = {false, false, false, true, true};
        TeamPreliminaryPlayerListMap::iterator pit = _preliminaryPlayers.find(tid);
        if(pit != _preliminaryPlayers.end())
        {
            TeamPreliminaryPlayerListIterator ppit = pit->second;
            TeamPreliminaryPlayer& pp = *ppit;
            UInt16 pos = std::distance(_preliminaryPlayers_list.begin(), ppit);
            calcBet(pp, pos, type-1, 0, flag[won], p);
        }
    }

	TeamArenaData * tad = globalTeamArena[tid];
    if(!tad) return;
    TeamArenaPlayer& ap = _teams[tad];
    if(won == 3 || won == 4)
    {   //进32强
        UInt32 twon = 0, tloss = 0;
        for(std::vector<TeamPriliminaryBattle>::iterator it = ap.battles.begin(); it != ap.battles.end(); ++ it)
        {
            if(it->otherName.empty())
                continue;
            if(it->won == 1 || it->won == 3)
            {
                ++ twon;
            }
            else
            {
                ++ tloss;
            }
        }

        const UInt32 score[2][2] = {
            {40, 80},
            {50, 100}
        };
        UInt32 award = 0;
        award = score[type-1][0]*tloss + score[type-1][1]*twon;
        int g_type = 709;
        if(old_type == 2)
            g_type = 710;
        else if(old_type == 3)
            g_type = 711;
        SYSMSGV(g, g_type);
        SYSMSGV(title, 716, p, g);
        SYSMSGV(content, 717, tad->getName().c_str(), _session, p, twon, tloss, award, g);
        sendTeamMail(tad, title, content);
    }
    else
    {
        TeamPriliminaryBattle pb;
        pb.won = won;
        pb.type = type;
        pb.otherHeroId = heroId;
        pb.otherName = name;
        pb.battleTime = btime;
        pb.tpbr = tpbr;
        ap.battles.push_back(pb);

        const UInt32 score[2][3] = {
            {40, 80, 40},
            {50, 100, 50}
        };
        for(UInt8 i = 0; i < tad->count; ++ i)
        {
            Player * player = tad->members[i];
            if(!player)
                continue;
            GameMsgHdr hdr(0x338, player->getThreadId(), player, 4);
            UInt32 data = score[type-1][won];
            GLOBAL().PushMsg(hdr, &data);

            player->AddVar(VAR_MONEY_ARENA2 + type - 1, data);
        }
        const bool isWin[2][3] = {
            {false, true, false},
            {false, true, false}
        };
        addTeamScore(tad, 0, isWin[type-1][won]);

        if(won == 2)    //32强被淘汰
        {
            UInt32 twon = 0, tloss = 0;
            for(std::vector<TeamPriliminaryBattle>::iterator it = ap.battles.begin(); it != ap.battles.end(); ++ it)
            {
                if(it->won == 1 || it->won == 3)
                {
                    ++ twon;
                }
                else
                {
                    ++ tloss;
                }
            }
            SYSMSGV(title, 718, p);
            UInt32 award = score[type-1][0]*tloss + score[type-1][1]*twon;
            SYSMSGV(content, 719, tad->getName().c_str(), _session, p, twon, tloss, award);
            sendTeamMail(tad, title, content);
        }

        //更新32强战报
        Stream st(REP::ARENAPRILIMINARY);
        st << static_cast<UInt8>(4);
        st << tad->getName() << tad->GetLev();
        ap.battles[ap.battles.size()-1].append(st);
        st << Stream::eos;
        tad->broadcastTeam(st);
    }
}

void TeamArenaMgr::readFrom( BinaryReader& brd )
{
    UInt16 otherSession = 0;
	brd >> _session >> otherSession;
    arena.setSession(otherSession);
	UInt8 progress;
	UInt8 reg;
	brd >> reg >> progress >> _nextTime;
    bool fStatus = false;
	if(progress != _progress)
	{
		_progress = progress;
		_notified = 0;
        fStatus = true;
        if(_progress == e_team_nextbegin)
            World::setArenaState(ARENA_XIANJIE_NONE);
	}
    if(_progress != e_team_nextbegin)
        World::setArenaState(ARENA_XIANJIE_ZHIZUN);

	switch(_progress)
	{
    case e_team_nextbegin:
        if(!_playerBet.empty())
        {
			_playerBet.clear();
            DB1().PushUpdateData("DELETE FROM `arena_team_bet`");
        }
		if(!_teams.empty())
        {
            _teamsCount[0] = 0;
			_teams.clear();
        }
        if(!_preliminaryPlayers.empty())
        {
            _preliminaryPlayers.clear();
            _preliminaryPlayers_list.clear();
            _preliminaryPlayers_list_set.clear();
        }
        break;
	case e_team_sign:
        if(!_playerBet.empty())
        {
			_playerBet.clear();
            DB1().PushUpdateData("DELETE FROM `arena_team_bet`");
        }
		if(!_teams.empty())
        {
            _teamsCount[0] = 0;
			_teams.clear();
        }
        if(!_preliminaryPlayers.empty())
        {
            _preliminaryPlayers.clear();
            _preliminaryPlayers_list.clear();
            _preliminaryPlayers_list_set.clear();
        }
        readTeams(brd);
        break;
    case e_team_sign_end:
        readPreTeams(brd);
        if(reg == 1)
            readHistories(brd);
		break;
	case e_team_32:
        if(reg == 1)
            readPreTeams(brd);
        readHistories(brd);
		break;
	case e_team_16:
	case e_team_8:
    case e_team_4:
    case e_team_2:
    case e_team_1:
    case e_team_32_end:
    case e_team_end:
        if(reg == 1)
        {
            readPreTeams(brd);
            readHistories(brd);
        }

        bool oldstatus = (_status > 0);
        readElimination(brd);
        if(oldstatus != (_status > 0))
            fStatus = true;
        break;
	}
    if(fStatus)
    {
        globalPlayers.enumerate(team_enum_send_status, static_cast<void *>(NULL));
    }

    if(!_loaded || _progress == e_team_sign)
    {
		_loaded = true;
        globalPlayers.enumerate(team_enum_send_active, static_cast<void *>(NULL));
    }
}

void TeamArenaMgr::readTeams(BinaryReader& brd)
{
    if(!_teams.empty())
    {
        _teamsCount[0] = 0;
        _teams.clear();
    }

    Mutex::ScopedLock lk(globalTeamArena.getMutex());
    std::unordered_map<UInt64, TeamArenaData *>& pm = globalTeamArena.getMap();
    UInt32 count;
    brd >> count;
    _teamsCount[0] += count;
    for(UInt32 z = 0; z < count; ++ z)
    {
        UInt64 tid = 0;
        int cid, sid;
        UInt8 level = 0;
        UInt8 leaderFid = 0;
        UInt32 battlePoint = 0;
        UInt32 support = 0;
        std::string name;
        brd >> cid >> sid >> tid >> leaderFid >> level >> battlePoint >> support >> name;
        GET_ORIGINID(tid, sid, cid);
        TeamPreliminaryPlayer pp;
        pp.id = tid;
        pp.support = support;
        pp.level = level;
        pp.leaderFid = leaderFid;
        pp.battlePoint = battlePoint;
        pp.name = name;
        UInt8 memCnt = 0;
        brd >> memCnt;
        for(UInt8 i = 0; i < memCnt && i < TEAMARENA_MAXMEMCNT; ++ i)
        {
            UInt64 pid = 0;
            brd >> pid;
            GET_ORIGINID(pid, sid, cid);
            pp.tprd.playerId[i] = pid;
            brd >> pp.tprd.name[i] >> pp.tprd.heroId[i];
            brd >> pp.tprd.battlePoint[i] >> pp.tprd.stamps[i];
        }
        TeamPreliminaryPlayerListIterator it = _preliminaryPlayers_list.insert(_preliminaryPlayers_list.end(), pp);
        _preliminaryPlayers[tid] = it;
        _preliminaryPlayers_list_set.insert(it);
        if(cid == cfg.channelNum && sid == cfg.serverNo)
        {
            std::unordered_map<UInt64, TeamArenaData *>::const_iterator it = pm.find(tid);
            if(it == pm.end())
                continue;
            TeamArenaData * tad = it->second;
            if(tad == NULL)
                continue;
            TeamArenaPlayer& ap = _teams[tad];
            ap.group = 0;
            ap.realName = name;
        }
    }
}

void TeamArenaMgr::readPreTeams(BinaryReader& brd)
{
    Mutex::ScopedLock lk(globalPlayers.getMutex());
    std::unordered_map<UInt64, TeamArenaData *>& pm = globalTeamArena.getMap();
    if(!_preliminaryPlayers.empty())
    {
        _preliminaryPlayers.clear();
        _preliminaryPlayers_list.clear();
        _preliminaryPlayers_list_set.clear();
    }

    UInt32 count;
    brd >> count;
    for(UInt32 z = 0; z < count; ++ z)
    {
        UInt64 tid = 0;
        int cid, sid;
        UInt8 level = 0;
        UInt8 leaderFid = 0;
        UInt32 battlePoint = 0;
        UInt32 support = 0;
        std::string name;
        brd >> cid >> sid >> tid >> leaderFid >> level >> battlePoint >> support >> name;
        GET_ORIGINID(tid, sid, cid);
        TeamPreliminaryPlayer pp;
        pp.id = tid;
        pp.support = support;
        pp.level = level;
        pp.leaderFid = leaderFid;
        pp.battlePoint = battlePoint;
        pp.name = name;
        UInt8 memCnt = 0;
        brd >> memCnt;
        for(UInt8 i = 0; i < memCnt && i < TEAMARENA_MAXMEMCNT; ++ i)
        {
            UInt64 pid = 0;
            brd >> pid;
            GET_ORIGINID(pid, sid, cid);
            pp.tprd.playerId[i] = pid;
            brd >> pp.tprd.name[i] >> pp.tprd.heroId[i];
            brd >> pp.tprd.battlePoint[i] >> pp.tprd.stamps[i];
        }
        TeamPreliminaryPlayerListIterator it = _preliminaryPlayers_list.insert(_preliminaryPlayers_list.end(), pp);
        _preliminaryPlayers[tid] = it;
        _preliminaryPlayers_list_set.insert(it);
        if(cid == cfg.channelNum && sid == cfg.serverNo)
        {
            std::unordered_map<UInt64, TeamArenaData *>::const_iterator it = pm.find(tid);
            if(it == pm.end())
                continue;
            TeamArenaData * tad = it->second;
            if(tad == NULL)
                continue;
            TeamArenaPlayer& ap = _teams[tad];
            ap.group = 0;
            ap.realName = name;
        }
    }
    for(std::map<Player *, PlayerBetInfo>::iterator it = _playerBet.begin(); it != _playerBet.end(); ++ it)
    {
        std::vector<BetInfo>& blist = it->second.betList[1][0];
        if(blist.empty())
            continue;

        for(std::vector<BetInfo>::iterator bit = blist.begin(); bit != blist.end(); ++ bit)
        {
            BetInfo& bi = *bit;
            if(bi.round == 1 && bi.group == 0 && bi.state == 1)
            {
                TeamPreliminaryPlayerListIterator pit = _preliminaryPlayers_list.begin();
                if(bi.pos > _preliminaryPlayers_list.size())
                    continue;
                std::advance(pit, bi.pos);
                if(pit != _preliminaryPlayers_list.end())
                {
                    TeamPreliminaryPlayer& pp = *pit;
                    pp.betMap[it->first] = bi.type;
                }
            }
        }
    }
}

void TeamArenaMgr::readHistories(BinaryReader& brd)
{
    Mutex::ScopedLock lk(globalTeamArena.getMutex());
    std::unordered_map<UInt64, TeamArenaData *>& pm = globalTeamArena.getMap();
    UInt32 cnt = 0;
    brd >> cnt;

    for(size_t j = 0; j < cnt; ++ j)
    {
        UInt64 tid = 0;
        brd >> tid;
        UInt16 bCount = 0;
        brd >> bCount;
        std::vector<TeamPriliminaryBattle> pbs;
        for(UInt16 p = 0; p < bCount; ++ p)
        {
            TeamPriliminaryBattle pb;
            brd >> pb.won >> pb.type >> pb.otherHeroId >> pb.otherName >> pb.battleTime;
            TeamPlayerBattleReport tpbr;
            tpbr.readReport(brd);

            pb.tpbr = tpbr;
            pbs.push_back(pb);
        }

        std::unordered_map<UInt64, TeamArenaData *>::const_iterator it = pm.find(tid);
        if(it == pm.end())
            continue;
        TeamArenaData * tad = it->second;
        if(tad == NULL)
            continue;
        TeamArenaPlayer& ap = _teams[tad];
        ap.battles.clear();
        ap.battles.resize(bCount);
        ap.battles = pbs;
    }
}

void TeamArenaMgr::readElimination(BinaryReader& brd)
{
    UInt8 cnt = 0;
    brd >> cnt;
    for(int i = 0; i < cnt; ++ i)
    {
        for(int j = 0; j < 32; ++ j)
        {
            int cid, sid;
            UInt32 battlePoint = 0;
            UInt32 support = 0;
            brd >> cid >> sid >> _finals[i][j].id >> _finals[i][j].level >> _finals[i][j].inspireLvl;
            brd >> _finals[i][j].leaderFid >> battlePoint >> support >> _finals[i][j].name;
            GET_ORIGINID(_finals[i][j].id, sid, cid);
            _finalIdx[i][0][j] = j;
            _finals[i][j].battlePoint = battlePoint;
            _finals[i][j].support = support;

            UInt8 memCnt = 0;
            brd >> memCnt;
            TeamPlayerReportData tprd;
            for(UInt8 m = 0; m < memCnt && m < TEAMARENA_MAXMEMCNT; ++ m)
            {
                brd >> tprd.playerId[m] >> tprd.name[m];
                brd >> tprd.heroId[m] >> tprd.battlePoint[m] >> tprd.stamps[m];
                GET_ORIGINID(tprd.playerId[m], sid, cid);
            }
            _finals[i][j].tprd = tprd;
        }
        _round = 0;
        UInt8 status;
        brd >> status >> _round;
        if(status != _status)
        {
            _status = status;
            if(_notified)
                _notified = 0;
        }

        int idx = _round;
        const int aIndex[6] = {0, 16, 24, 28, 30, 31};
        for(int j = 0; j < aIndex[idx]; ++ j)
        {
            UInt8 cnt = 0;
            brd >> cnt;
            brd >> _finalBattles[i][j].wonFlag;
            _finalBattles[i][j].battleId.clear();
            _finalBattles[i][j].battleId.resize(cnt);
            for(int k = 0; k < cnt; ++ k)
            {
                TeamPlayerBattleReport tpbr;
                tpbr.readReport(brd);
                _finalBattles[i][j].battleId[k] = tpbr;
            }
        }

        calcFinalBet(i);
    }
    if(!_notified)
        _notified = 1;
}

void TeamArenaMgr::calcFinalBet(int i)
{
    const int aIndex[6] = {0, 16, 24, 28, 30, 31};
    UInt32 r = _round;
    if(_status == 0)
        ++ r;
    if(r > 1)
    {
        GET_PROGRESS_NAME(p, _round+1);
        for(UInt32 j = 1; j < r; ++ j)
        {
            int starti = aIndex[j - 1], endi = aIndex[j];
            for(int k = starti; k < endi; ++ k)
            {
                if(_finalBattles[i][k].winner(j-1) == 0)
                {
                    UInt8 nidx = _finalIdx[i][j-1][(k - starti) * 2];
                    _finalIdx[i][j][k - starti] = nidx;
                    if(_status == 0 && j == r - 1)
                    {
                        int pos1 = nidx;
                        int pos2 = _finalIdx[i][j-1][(k - starti) * 2 + 1];
                        if(j > 2)
                        {
                            pos1 = pos1 >> 2;
                            pos2 = pos2 >> 2;
                        }
                        else if(j > 1)
                        {
                            pos1 = pos1 >> 1;
                            pos2 = pos2 >> 1;
                        }
                        calcBet(_finals[i][nidx], pos1, j+1, i, true, p);
                        calcBet( _finals[i][_finalIdx[i][j-1][(k - starti) * 2 + 1]], pos2, j+1, i, false, p);
                        //_finals[i][nidx].calcBet(true, p);
                        //_finals[i][_finalIdx[i][j-1][(k - starti) * 2 + 1]].calcBet(false, p);
                    }
                }
                else
                {
                    UInt8 nidx = _finalIdx[i][j-1][(k - starti) * 2 + 1];
                    _finalIdx[i][j][k - starti] = nidx;
                    if(_status == 0 && j == r - 1)
                    {
                        int pos1 = nidx;
                        int pos2 = _finalIdx[i][j-1][(k - starti) * 2];
                        if(j > 2)
                        {
                            pos1 = pos1 >> 2;
                            pos2 = pos2 >> 2;
                        }
                        else if(j > 1)
                        {
                            pos1 = pos1 >> 1;
                            pos2 = pos2 >> 1;
                        }
                        calcBet(_finals[i][nidx], pos1, j+1, i, true, p);
                        calcBet( _finals[i][_finalIdx[i][j-1][(k - starti) * 2]], pos2, j+1, i, false, p);
                        //_finals[i][nidx].calcBet(true, p);
                        //_finals[i][_finalIdx[i][j-1][(k - starti) * 2]].calcBet(false, p);
                    }
                }
            }
        }
    }
    if(_loaded)
    {
        Stream st(REP::ARENAPRILIMINARY);
        st << static_cast<UInt8>(5) << Stream::eos;
        NETWORK()->Broadcast(st);
        if(!_notified)
        {
            if(_status == 0 && _round > 0)
            {
                GET_PROGRESS_NAME(p, _round + 1);
                SYSMSGV(g, 710 + i);
                SYSMSGV(title, 790, p, g);
                SYSMSGV(title2, 792, p, g);
                if(_round < 5)
                {
                    //_round:1=进16强 2=进8强 3=进4强 4=进半决赛 5=进决赛
                    const int aIndex[6] = {0, 16, 24, 28, 30, 31};
                    int starti = aIndex[_round - 1];
                    int endi = aIndex[_round];
                    for(int idx = starti; idx < endi; ++ idx)
                    {
                        TeamEliminationPlayer& ep1 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2]];
                        TeamEliminationPlayer& ep2 = _finals[i][_finalIdx[i][_round - 1][(idx - starti) * 2 + 1]];
                        if(ep1.name.empty() || ep2.name.empty())
                            continue;
                        UInt8 winCount = _finalBattles[i][idx].winCount(_round-1);
                        const UInt8 totalCount[5] = { 3, 3, 5, 5, 7 };
                        const UInt8 winCount_mod[5] = {1, 1, 2, 2, 3};
                        if(winCount > winCount_mod[_round-1])
                        {
                            if(ep1.id != 0)
                            {
                                TeamArenaData * tad = globalTeamArena[ep1.id];
                                if(tad)
                                {
                                    SYSMSGV(content, 791, tad->getName().c_str(), _session, p, winCount, totalCount[_round-1] - winCount);
                                    sendTeamMail(tad, title, content);
                                    addTeamScore(tad, _round, true);
                                }
                            }
                            if(ep2.id != 0)
                            {
                                TeamArenaData * tad = globalTeamArena[ep2.id];
                                if(tad)
                                {
                                    SYSMSGV(content, 793, tad->getName().c_str(), _session, p, totalCount[_round-1] - winCount, winCount);
                                    sendTeamMail(tad, title2, content);
                                    addTeamScore(tad, _round, false);
                                    giveTeamLastAward(tad, i, _round, 0);
                                }
                            }
                        }
                        else
                        {
                            if(ep2.id != 0)
                            {
                                TeamArenaData * tad = globalTeamArena[ep2.id];
                                if(tad)
                                {
                                    SYSMSGV(content, 791, tad->getName().c_str(), _session, p, totalCount[_round-1] - winCount, winCount);
                                    sendTeamMail(tad, title, content);
                                    addTeamScore(tad, _round, true);
                                }
                            }
                            if(ep1.id != 0)
                            {
                                TeamArenaData * tad = globalTeamArena[ep1.id];
                                if(tad)
                                {
                                    SYSMSGV(content, 793, tad->getName().c_str(), _session, p, winCount, totalCount[_round-1] - winCount);
                                    sendTeamMail(tad, title2, content);
                                    addTeamScore(tad, _round, false);
                                    giveTeamLastAward(tad, i, _round, 0);
                                }
                            }
                        }
                    }
                }
                else
                {
                    int idx = 0;
                    const int aIndex[6] = {0, 16, 24, 28, 30, 31};
                    UInt8 nidx = 0;
                    UInt8 nidx2 = 0xFF;
                    for(int j = 1; j < 6; ++ j)
                    {
                        int starti = aIndex[j - 1];
                        int endi = aIndex[j];
                        for(idx = starti; idx < endi; ++ idx)
                        {
                            if(_finalBattles[i][idx].winner(j-1) == 0)  //攻方赢
                            {
                                nidx = _finalIdx[i][j-1][(idx - starti) * 2 + 1];   //淘汰
                                if(j == 5)
                                    nidx2 = _finalIdx[i][j-1][(idx - starti) * 2];  //冠军
                            }
                            else        //守方赢
                            {
                                nidx = _finalIdx[i][j-1][(idx - starti) * 2];
                                if(j == 5)
                                    nidx2 = _finalIdx[i][j-1][(idx - starti) * 2 + 1];
                            }
                            TeamEliminationPlayer& ep = _finals[i][nidx];
                            TeamArenaData * tad = globalTeamArena[ep.id];
                            if(j == 5)
                            {
                                giveTeamLastAward(tad, i, j, 0);
                                addTeamScore(tad, j, false);
                            }

                            setTeamMaxRank(tad, j, i);
                            if(nidx2 != (UInt8)(0xFF))
                            {
                                TeamEliminationPlayer& ep = _finals[i][nidx2];
                                TeamArenaData * tad = globalTeamArena[ep.id];
                                giveTeamLastAward(tad, i, j, 1);
                                setTeamMaxRank(tad, j+1, i);  //进决赛,冠军
                                addTeamScore(tad, _round, true);
                            }
                        }
                    }
                }
            }
        }
        if(_status == 0 && _progress != 2)
        {
            {
                for(int j = 0; j < 32; ++ j)
                {
                    _finals[i][j].resetBet();
                }
            }
            for(std::map<Player *, PlayerBetInfo>::iterator it = _playerBet.begin(); it != _playerBet.end(); ++ it)
            {
                for(int sidx = 2; sidx < 7; ++ sidx)
                {
                    std::vector<BetInfo>& blist = it->second.betList[sidx][i];
                    if(blist.empty())
                        continue;
                    int cnt = blist.size();
                    for(int j = 0; j < cnt; ++ j)
                    {
                        BetInfo& bi = blist[j];
                        if(bi.group == i + 1 && bi.recieved == 0)
                        {
                            UInt8 pos2 = bi.pos;
                            switch(bi.state)
                            {
                            case 3:
                                pos2 = _finalIdx[i][1][bi.pos];
                                break;
                            case 4:
                            case 5:
                            case 6:
                                pos2 = _finalIdx[i][2][bi.pos];
                                break;
                            }

                            _finals[i][pos2].betMap[it->first] = bi.type;
                        }
                    }
                }
            }
        }
    }
    else
    {
        UInt8 r = _round;
        if(_status > 0 && r > 0)
        {
            -- r;
        }
        for(std::map<Player *, PlayerBetInfo>::iterator it = _playerBet.begin(); it != _playerBet.end(); ++ it)
        {
            for(int sidx = 2; sidx < 7; ++ sidx)
            {
                std::vector<BetInfo>& blist = it->second.betList[sidx][i];
                if(blist.empty())
                    continue;
                int cnt = blist.size();
                for(int j = 0; j < cnt; ++ j)
                {
                    BetInfo& bi = blist[j];
                    if(bi.group == i + 1 && bi.recieved == 0)
                    {
                        UInt8 pos2 = bi.pos;
                        switch(bi.state)
                        {
                        case 3:
                            pos2 = _finalIdx[i][1][bi.pos];
                            break;
                        case 4:
                        case 5:
                        case 6:
                            pos2 = _finalIdx[i][2][bi.pos];
                            break;
                        }

                        _finals[i][pos2].betMap[it->first] = bi.type;
                    }
                }
            }
        }
    }
}

void TeamArenaMgr::calcBet(TeamPreliminaryPlayer& pp, UInt16 pos, UInt8 state, UInt8 group, bool won, const char * t)
{
	for(std::map<Player *, UInt8>::iterator it = pp.betMap.begin(); it != pp.betMap.end(); ++ it)
	{
        Player* player = it->first;
        std::map<Player *, PlayerBetInfo>::iterator iter = _playerBet.find(player);
		std::vector<BetInfo>& blist = iter->second.betList[state][group];

        bool recieved = true;
		for(std::vector<BetInfo>::iterator bit = blist.begin(); bit != blist.end(); ++ bit)
		{
            if(bit->pos == pos)
            {
                if(bit->recieved == 0)
                {
                    recieved = false;
                    bit->recieved = 1;
                    DB1().PushUpdateData("REPLACE INTO `arena_team_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u, %u)", player->getId(), bit->round, bit->state, bit->group, bit->recieved, bit->pos, bit->type);
                }
                break;
            }
        }
        if(recieved)
            continue;

        std::string strItems = "";
		if(won)
		{
            SYSMSGV(title, 796, t, pp.name.c_str());

            UInt32 rew = it->second == 0 ? 100 : 50;

            GameMsgHdr hdr(0x338, player->getThreadId(), player, sizeof(rew));
            GLOBAL().PushMsg(hdr, &rew);

            if(state == 1)
                it->first->AddVar(VAR_MONEY_ARENA3, rew);
            SYSMSGV(content, 797, _session, t, pp.name.c_str(), rew);
            Mail * mail = it->first->GetMailBox()->newMail(NULL, 0x01, title, content);
            if(mail)
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, it->first->getId(), mail->id, ArenaAward, title, content, strItems.c_str(), mail->recvTime);
		}
		else
		{
            SYSMSGV(title, 798, t, pp.name.c_str());
			SYSMSGV(content, 799, _session, t, pp.name.c_str());
			Mail * mail = it->first->GetMailBox()->newMail(NULL, 0x01, title, content);
            if(mail)
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, it->first->getId(), mail->id, ArenaAward, title, content, strItems.c_str(), mail->recvTime);
		}
	}
}

void TeamArenaMgr::sendTeamMail(TeamArenaData * tad, const std::string& title, const std::string& content)
{
    if(!tad || title.empty() || content.empty())
        return;
    for(UInt8 i = 0; i < tad->count; ++ i)
    {
        if(tad->members[i])
        {
            Mail * mail = tad->members[i]->GetMailBox()->newMail(NULL, 0x01, title, content);
            if(mail != NULL)
            {
                std::string strItems;
                DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, tad->members[i]->getId(), mail->id, ArenaAward, title.c_str(), content.c_str(), strItems.c_str(), mail->recvTime);
            }
        }
    }
}

void TeamArenaMgr::giveTeamLastAward(TeamArenaData * tad, UInt8 group, UInt8 idx, UInt8 type)
{
    //group 0天榜 1地榜
    if(!tad || group > 1 || idx < 1 || idx > 5 || type > 1)
        return;
    SYSMSGV(g, 710 + group);
    SYSMSGV(title, 794, g);
    for(UInt8 m = 0; m < tad->count; ++ m)
    {
        Player * pl = tad->members[m];
        if(pl == NULL) continue;
        UInt32 data = ArenaLastAwards[group][idx+type-1][1];    //龙魂
        GameMsgHdr hdr(0x338, pl->getThreadId(), pl, sizeof(data));
        GLOBAL().PushMsg(hdr, &data);

        SYSMSGV(content, 795, tad->getName().c_str(), _session, data);
        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
        if(mail != NULL)
        {
            data = ArenaLastAwards[group][idx+type-1][0];   //仙界宝箱
            std::string strItems;
            strItems += Itoa(data);
            strItems += ",";
            strItems += Itoa(1);
            strItems += "|";
            mailPackageManager.push(mail->id, data, 1, true);
            DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %" I64_FMT "u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, pl->getId(), mail->id, ArenaAward, title, content, strItems.c_str(), mail->recvTime);
        }
    }
}

UInt8 TeamArenaMgr::bet1( Player * player, UInt8 state, UInt8 group, UInt64 tid, UInt8 type )
{
	if(group != 0 || state >= 1)
		return 0xFF;
    if(progress_accept[state][_progress] == 0)
        return 0xFF;

    state += 1;
    TeamPreliminaryPlayerListMap::iterator pit = _preliminaryPlayers.find(tid);
    if(pit == _preliminaryPlayers.end())
        return 0xFF;

    TeamPreliminaryPlayerListIterator it = pit->second;
    TeamPreliminaryPlayer& pp = *it;

    UInt16 pos = std::distance(_preliminaryPlayers_list.begin(), it);
    std::map<Player*, PlayerBetInfo>::iterator ait = _playerBet.find(player);
    if((ait != _playerBet.end() && ait->second.betList[state][0].size() >= 1) || pp.name.empty())
        return 0xFF;
    if(pp.betMap.find(player) != pp.betMap.end())
        return 2;
    pp.betMap[player] = type;

	Stream st(ARENAREQ::BET, 0xEF);
	st << group << pos << Stream::eos;
	NETWORK()->SendToArena(st);
	BetInfo binfo;
	binfo.state = state;
	binfo.round = 1;
	binfo.group = group;
	binfo.recieved = 0;
	binfo.pos = pos;
	binfo.type = type;
    int i = 0;
    if(group > 0)
        i = group - 1;
	_playerBet[player].betList[state][i].push_back(binfo);
	DB1().PushUpdateData("REPLACE INTO `arena_team_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u, %u)", player->getId(), binfo.round, binfo.state, binfo.group, binfo.recieved, binfo.pos, binfo.type);
	GameMsgHdr hdr(0x230, player->getThreadId(), player, 2);
	UInt16 data = type;
	GLOBAL().PushMsg(hdr, &data);

	return type;
}

UInt8 TeamArenaMgr::bet2( Player * player, UInt8 state, UInt8 group, UInt16 pos, UInt8 type )
{
    //group 1天榜 2地榜
	if(group < 1 || group > 2 || state > 5 || state < 1 || _status > 0)
		return 0xFF;
    if(progress_accept[state][_progress] == 0)
        return 0xFF;

    state += 1;
    UInt16 pos2 = pos;
    int gIdx = group - 1;
    UInt8 fidx = 0;
    switch(state)
    {
    case 2:
        fidx = pos >> _round;
        break;
    case 3:
        if(pos > 15)
            return 0xFF;
        pos2 = _finalIdx[gIdx][1][pos];
        fidx = pos2 >> _round;
        break;
    case 4:
    case 5:
    case 6:
        if(pos > 7)
            return 0xFF;
        pos2 = _finalIdx[gIdx][2][pos];
        fidx = pos2 >> _round;
        break;
    }
    if(_finalIdx[gIdx][_round][fidx] != pos2 || pos > 31 || _finals[gIdx][pos2].name.empty())
        return 0xFF;

    if( (fidx % 2) == 1)
        fidx -= 1;
    else
        fidx += 1;

    UInt8 pos3 = _finalIdx[gIdx][_round][fidx];
    if(_finals[gIdx][pos2].betMap.find(player) != _finals[gIdx][pos2].betMap.end()
            || _finals[gIdx][pos3].betMap.find(player) != _finals[gIdx][pos3].betMap.end())
        return 2;
    _finals[gIdx][pos2].betMap[player] = type;

	Stream st(ARENAREQ::BET, 0xEF);
	st << group << pos2 << Stream::eos;
	NETWORK()->SendToArena(st);
	BetInfo binfo;
	binfo.state = state;
	binfo.round = _round + 1;
	binfo.group = group;
	binfo.recieved = 0;
	binfo.pos = pos;
	binfo.type = type;
    int i = 0;
    if(group > 0)
        i = group - 1;
	_playerBet[player].betList[state][i].push_back(binfo);
	DB1().PushUpdateData("REPLACE INTO `arena_team_bet`(`id`, `round`, `state`, `group`, `recieved`, `pos`, `tael`) VALUES(%" I64_FMT "u, %u, %u, %u, %u, %u, %u)", player->getId(), binfo.round, binfo.state, binfo.group, binfo.recieved, binfo.pos, binfo.type);
	GameMsgHdr hdr(0x230, player->getThreadId(), player, 2);
	UInt16 data = type;
	GLOBAL().PushMsg(hdr, &data);
	return type;
}

void TeamArenaMgr::updateBattlePoint(BinaryReader& brd)
{
    UInt32 cid = 0;
    UInt32 sid = 0;
    UInt64 tid = 0, pid = 0;
    UInt32 battlePoint = 0, battlePoint1 = 0;
    std::string name;
    brd >> cid >> sid >> tid >> pid >> battlePoint >> battlePoint1 >> name;
    GET_ORIGINID(tid, sid, cid);
    GET_ORIGINID(tid, sid, pid);

    TeamPreliminaryPlayerListMap::iterator pit = _preliminaryPlayers.find(tid);
    if(pit != _preliminaryPlayers.end())
    {
        TeamPreliminaryPlayerListIterator ppit = pit->second;
        TeamPreliminaryPlayer& pp = *ppit;
        _preliminaryPlayers_list_set.erase(ppit);
        pp.battlePoint = battlePoint;
        for(UInt8 i = 0; i < TEAMARENA_MAXMEMCNT; ++ i)
        {
            if(pp.tprd.playerId[i] == pid)
            {
                pp.tprd.battlePoint[i] = battlePoint1;
                pp.tprd.name[i] = name;
                break;
            }
        }
        _preliminaryPlayers_list_set.insert(ppit);
    }
    bool find = false;
    for(int i = 0; i < 2; ++ i)
    {
        for(int j = 0; j < 32; ++ j)
        {
            if(_finals[i][j].id == tid)
            {
                _finals[i][j].battlePoint = battlePoint;
                for(UInt8 m = 0; m < TEAMARENA_MAXMEMCNT; ++ m)
                {
                    if(_finals[i][j].tprd.playerId[m] == pid)
                    {
                        _finals[i][j].tprd.battlePoint[m] = battlePoint1;
                        _finals[i][j].tprd.name[m] = name;
                        break;
                    }
                }
                find = true;
                break;
            }
        }
        if(find) break;
    }
}

void TeamArenaMgr::updateInspireLevel(BinaryReader& brd)
{
    UInt32 cid = 0;
    UInt32 sid = 0;
    UInt64 tid = 0;
    UInt8 level = 0, inspireLvl = 0;
    brd >> cid >> sid >> tid >> level >> inspireLvl;
    GET_ORIGINID(tid, sid, cid);

    TeamPreliminaryPlayerListMap::iterator pit = _preliminaryPlayers.find(tid);
    if(pit != _preliminaryPlayers.end())
    {
        TeamPreliminaryPlayerListIterator ppit = pit->second;
        TeamPreliminaryPlayer& pp = *ppit;
        _preliminaryPlayers_list_set.erase(ppit);
        pp.level = level;
        pp.inspireLvl = inspireLvl;
        _preliminaryPlayers_list_set.insert(ppit);
    }
    bool find = false;
    for(int i = 0; i < 2; ++ i)
    {
        for(int j = 0; j < 32; ++ j)
        {
            if(_finals[i][j].id == tid)
            {
                _finals[i][j].level = level;
                _finals[i][j].inspireLvl = inspireLvl;
                find = true;
                break;
            }
        }
        if(find) break;
    }
}

void TeamArenaMgr::updateLeaderBoard(BinaryReader& brd)
{
    UInt16 cnt;
    brd >> cnt;
    for(int i = 0; i < cnt; ++ i)
    {
        LeaderTeam lteam;
        UInt16 session = 0;
        UInt8 memCnt = 0;
        brd >> session >> lteam.teamName >> lteam.leaderName;
        brd >> memCnt;
        for(UInt8 m = 0; m < memCnt; ++ m)
        {
            brd >> lteam.memberName[m] >> lteam.memberLvl[m];
            brd >> lteam.memberBp[m] >> lteam.heroId[m];
        }
        _leaderBoard[session] = lteam;
    }
}

//上届战队排名(64个)
void TeamArenaMgr::updateLastRank(BinaryReader& brd)
{
    UInt8 cnt = 0; 
    brd >> cnt;
    _lastRankTeam.clear();
    for(int i = 0; i < cnt; ++ i)
    {
        UInt64 teamId = 0;
        brd >> teamId;
        _lastRankTeam.push_back(teamId);
    }
}

void TeamArenaMgr::resetTeamLastRank(TeamArenaData * tad)
{
    //round:1=32强 2=16强 3=8强 4=4强 5=半决赛(亚军) 6=决赛(冠军)
    if(!tad) return;
    UInt8 cnt = _lastRankTeam.size();
    UInt8 lastRank = 0;
    for(UInt8 id = 0; id < cnt; ++ id)
    {
        if(tad->getId() == _lastRankTeam[id])
        {
            if(id == 0)                     // 1
                lastRank = 1 << 6;
            else if(id == 1)                // 1
                lastRank = 1 << 5;
            else if(id >= 2 && id < 4)      // 2
                lastRank = 1 << 4;
            else if(id >= 4 && id < 8)      // 4
                lastRank = 1 << 3;
            else if(id >= 8 && id < 16)     // 8
                lastRank = 1 << 2;
            else if(id >= 16 && id < 32)    // 16
                lastRank = 1 << 2;
            else if(id == 32)               // 1
                lastRank = (1 << 6) + 1;
            else if(id == 33)               // 1
                lastRank = (1 << 5) + 1;
            else if(id >= 34 && id < 36)    // 2
                lastRank = (1 << 4) + 1;
            else if(id >= 36 && id < 40)    // 4
                lastRank = (1 << 3) + 1;
            else if(id >= 40 && id < 48)    // 8
                lastRank = (1 << 2) + 1;
            else if(id >= 48 && id < 64)    // 16
                lastRank = (1 << 1) + 1;
            tad->lastRank = lastRank;
        }
    }
}

void TeamArenaMgr::setTeamMaxRank(TeamArenaData * tad, UInt8 round, UInt8 group)
{
    //group:0天榜 1地榜
    //round:1=32强 2=16强 3=8强 4=4强 5=半决赛(亚军) 6=决赛(冠军)
    if(!tad || round > 6 || round < 0)
        return;
    UInt8 nrank = 1 << round;
    UInt8 lrank = tad->maxRank >> 1;
    bool isWrite = false;
    if(!lrank)
        isWrite = true;
    else if(nrank >= lrank)
    {
        if(group <= (tad->maxRank & 1))
            isWrite = true;
    }
    else
    {
        if(group < (tad->maxRank & 1))
            isWrite = true;
    }
    if(group)
        nrank |= 1;
    if(isWrite)
        tad->maxRank = nrank;
	DB1().PushUpdateData("UPDATE `arena_team` SET `maxRank` = %u WHERE `id` = %" I64_FMT "u", tad->maxRank, tad->getId());
}

void TeamArenaMgr::sendLeaderBoard(Player* pl)
{   //历届冠军战队
    if(pl == NULL) return;
	Stream st(REP::SERVER_ARENA_LB);
    UInt16 cnt = _leaderBoard.size();
    st << static_cast<UInt8>(2) << cnt;
    for(std::map<UInt16, LeaderTeam>::iterator it = _leaderBoard.begin(); it != _leaderBoard.end(); ++ it)
    {
        st << it->first << it->second.teamName;
        st << it->second.leaderName;
        for(UInt8 j = 0; j < TEAMARENA_MAXMEMCNT; ++ j)
        {
            if(it->second.memberName[j] == it->second.leaderName)
                continue;
            st << it->second.memberName[j];
        }
    }
    st << Stream::eos;

    pl->send(st);
}

void TeamArenaMgr::sendLastLeaderBoard(Player* pl)
{   //上一届冠军战队
    if(pl == NULL) return;
	Stream st(REP::SERVER_ARENA_LB);
    st << static_cast<UInt8>(4);
    std::map<UInt16, LeaderTeam>::iterator it = _leaderBoard.find(_session-1);
    if(it != _leaderBoard.end())
    {
        st << it->second.teamName << it->second.leaderName;
        st << static_cast<UInt8>(TEAMARENA_MAXMEMCNT);
        for(UInt8 m = 0; m < TEAMARENA_MAXMEMCNT; ++ m)
        {
            st << it->second.memberName[m];
            st << it->second.memberLvl[m];
            st << it->second.memberBp[m];
            st << it->second.heroId[m];
        }
        st << static_cast<UInt8>(pl->GetVar(VAR_TEAMARENA_WORSHIP));
    }
    st << Stream::eos;

    pl->send(st);
}

void TeamArenaMgr::sendActive(Player* pl)
{
    if(pl == NULL) return;
    if(arena.active())
    {
        arena.sendActive(pl);
        return;
    }
    Stream st(REP::SERVER_ARENA_OP);
    UInt8 flag = static_cast<UInt8>(active() ? ARENA_XIANJIE_ZHIZUN : 0);
    if(_progress == e_team_nextbegin)
        flag |= 0x10;
    st << static_cast<UInt8>(0) << flag << Stream::eos;
    pl->send(st);
}

void TeamArenaMgr::sendStatus(Player* pl)
{
    if(pl == NULL) return;
	Stream st(REP::SERVER_ARENA_INFO);
    st << static_cast<UInt8>(10);
    UInt8 find = 0;
    UInt8 entered = 0;
    // 0 - 可报名阶段
    // 1 - 32强投注阶段
    // 2 - 32强战斗阶段
    // 3 - 16强投注阶段
    // 4 - 16强战斗阶段
    // 5 - 8强投注阶段
    // 6 - 8强战斗阶段
    // 7 - 4强投注阶段
    // 8 - 4强战斗阶段
    // 9 - 半决赛投注阶段
    // 10 - 半决赛战斗阶段
    // 11 - 决赛阶段投注阶段
    // 12 - 决赛阶段战斗阶段
    // 13 - 决赛阶段结束（不可报名
    // 14 - 无法战斗
    //                     0  1  2  3  4  5   6   7  8   9  10
    const UInt8 state[] = {0, 2, 4, 6, 8, 10, 12, 1, 3, 13, 14};
    UInt8 progress = state[_progress];
    if(_progress >= e_team_8 && _progress <= e_team_1 && _status == 0)
        progress -= 1;
    TeamArenaData * tad = pl->getTeamArena();
    if(tad)
    {
        TeamPreliminaryPlayerListMap::iterator pit = _preliminaryPlayers.find(tad->getId());
        if(pit != _preliminaryPlayers.end())
            find = 1;
    }
    st << _session << progress << find << entered << Stream::eos;
    pl->send(st);
}

void TeamArenaMgr::sendEnter(Player* pl)
{
    if(pl == NULL) return;
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;

	Stream st(REP::SERVER_ARENA_INFO);
    st << static_cast<UInt8>(11);
    UInt8 find = 0;
    TeamArenaData * tad = pl->getTeamArena();
    if(tad)
    {
        TeamArenaPlayerMap::iterator it = _teams.find(tad);
        if(it != _teams.end())
            find = 1;
    }
    st << find << leftTime << _teamsCount[0] << Stream::eos;

    pl->send(st);
}

void TeamArenaMgr::sendPreliminary(Player* player, UInt8 flag, UInt16 start, UInt8 len)
{
    if(player == NULL) return;
	UInt32 leftTime;
	UInt32 now = TimeUtil::Now();
	if(_nextTime > now)
		leftTime = _nextTime - now;
	else
		leftTime = 0;

	Stream st(REP::SERVER_ARENA_INFO);
    st << static_cast<UInt8>(12);
    st << flag;
    if(flag == 0)
    {
        TeamArenaData * tad = player->getTeamArena();
        if(tad == NULL) return;
        TeamPreliminaryPlayerListMap::iterator it = _preliminaryPlayers.find(tad->getId());
        if(it == _preliminaryPlayers.end())
            return;
        TeamArenaPlayerMap::iterator ait = _teams.find(tad);
        if(ait == _teams.end())
            return;
        st << leftTime << ait->second.realName;
        st << tad->GetLev();
        TeamArenaPlayer& ap = ait->second;
        UInt8 cnt = static_cast<UInt8>(ap.battles.size());
        st << cnt;
        for(UInt8 i = 0; i < cnt; ++ i)
        {
            ap.battles[i].append(st);
        }
    }
    else
    {
        std::map<Player *, PlayerBetInfo>::iterator bit =  _playerBet.find(player);
        if(bit == _playerBet.end())
        {
            st << static_cast<UInt8>(1);
        }
        else
        {
            PlayerBetInfo& pbi = bit->second;
            std::vector<BetInfo>& blist = pbi.betList[1][0];
            UInt8 cnt = blist.size();
            if(cnt > 1)
                cnt = 1;
            st << static_cast<UInt8>(1 - cnt);
        }
        UInt16 winNum = player->GetVar(VAR_MONEY_ARENA3);
        UInt16 totalPreNum = _preliminaryPlayers_list_set.size();
        st << winNum << totalPreNum << start;
        size_t offset = st.size();
        UInt8 premNum = 0;
        st << premNum;
        TeamPreliminaryPlayersSet::iterator setIt = _preliminaryPlayers_list_set.begin();
        size_t pos = start;
        if(pos > _preliminaryPlayers_list_set.size())
            pos = _preliminaryPlayers_list_set.size();
        std::advance(setIt, pos);
        for(int i = 0; setIt != _preliminaryPlayers_list_set.end() && i < len; ++ setIt)
        {
            ++ premNum;
            ++ i;
            TeamPreliminaryPlayerListIterator pit = *setIt;
            TeamPreliminaryPlayer& pp = *pit;
            UInt8 fSupport = 0;
            std::map<Player *, UInt8>::iterator it = pp.betMap.find(player);
            if(it != pp.betMap.end())
                fSupport = it->second + 1;
            st << pp.id << pp.level << pp.battlePoint << static_cast<UInt16>(pp.support) << fSupport << pp.name;
        }
        st.data<UInt8>(offset) = static_cast<UInt8>(premNum);
    }

    st << Stream::eos;
    player->send(st);
}

void TeamArenaMgr::sendElimination( Player * player, UInt8 type, UInt8 group )
{
	bool resultOnly = (group & 0x80) > 0;
    UInt8 r = _round;
    switch(type)
    {
    case 1:
        if(r < 1)
            return;
        break;
    case 2:
        if(r < 2)
            return;
        break;
    case 3:
        if(r < 3)
            return;
        break;
    case 4:
        if(r < 4)
            return;
        break;
    }
    if(group > 2 || group < 1)
        return;
	if(_progress < e_team_16 || _progress == e_team_sign_end)
		return;
	Stream st(REP::SERVER_ARENA_INFO);
    st << static_cast<UInt8>(type + 13);
	st << group;
    int gIdx = group - 1;
	std::map<Player *, PlayerBetInfo>::iterator iter = _playerBet.find(player);
	const int aIndex[6] = {0, 16, 24, 28, 30, 31};
	if(iter == _playerBet.end())
		st << static_cast<UInt8>(0);
	else
	{
        size_t offset = st.size();
        UInt8 cnt = 0;
        int states = 1;
        int state = 2;
        switch(type)
        {
        case 1:
            state = 3;
            break;
        case 4:
            ++ states;
        case 3:
            ++ states;
        case 2:
            state = 4;
            break;
        }
        st << cnt;
        for(int i = 0; i < states; ++ i)
        {
            std::vector<BetInfo>& blist = iter->second.betList[state+i][gIdx];
            for(std::vector<BetInfo>::iterator it = blist.begin(); it != blist.end(); ++ it)
            {
                if(it->group != group)
                    continue;
                ++ cnt;
                UInt8 pos = it->pos;
                UInt8 g_pos = (pos << 3) | it->round;
                st << g_pos << it->type;
            }
        }
        st.data<UInt8>(offset) = cnt;
	}
	if(!resultOnly)
	{
        int cnt = 0;
        int progress = 0;
        if(type == 0)
        {
            cnt = 32;
        }
        else if(type == 1)
        {
            cnt = 16;
            progress = 1;
        }
        else
        {
            progress = 2;
            cnt = 8;
        }

        for(int idx = 0; idx < cnt; ++ idx)
        {
            int i = _finalIdx[gIdx][progress][idx];
            st << _finals[gIdx][i].id << _finals[gIdx][i].name << _finals[gIdx][i].level << _finals[gIdx][i].leaderFid;
            st << _finals[gIdx][i].battlePoint << static_cast<UInt16>(_finals[gIdx][i].support) << _finals[gIdx][i].inspireLvl;
            TeamPlayerReportData tprd;
            _finals[gIdx][i].tprd.sortByBattlePoint(tprd);
            for(UInt8 j = 0; j < TEAMARENA_MAXMEMCNT; ++ j)
            {
                st << tprd.name[j] << tprd.stamps[j];
                st << tprd.heroId[j] << tprd.battlePoint[j];
            }
        }
	}

    int istart = 0;
    switch(type)
    {
    case 0:
        if(r > 1)
            r = 1;
        break;
    case 1:
        r = 2;
        istart = aIndex[1];
        break;
    case 2:
    case 3:
    case 4:
        istart = aIndex[2];
        r = type + 1;
        break;
    }

    st << static_cast<UInt8>(r);
	for(int k = istart; k < aIndex[r]; ++ k)
	{
        std::vector<TeamPlayerBattleReport>& tpbrVec = _finalBattles[gIdx][k].battleId;
        UInt8 cnt = tpbrVec.size();
        st << cnt << _finalBattles[gIdx][k].wonFlag;
        for(size_t i = 0; i < cnt; ++ i)
        {
            tpbrVec[i].appendReport(st);
        }
	}
	st << Stream::eos;
	player->send(st);
}

void TeamArenaMgr::updateSuport(UInt8 type, UInt8 flag, UInt16 pos)
{
    if(flag == 1)
    {
        TeamPreliminaryPlayerListIterator it = _preliminaryPlayers_list.begin();
        size_t pos2 = pos;
        if(pos2 > _preliminaryPlayers_list.size())
            pos2 = _preliminaryPlayers_list.size();
        std::advance(it, pos2);
        if(it != _preliminaryPlayers_list.end())
        {
            TeamPreliminaryPlayer& pp = *it;
            ++ pp.support;
        }
    }
    else if(flag == 2)
    {
        UInt8 fidx = pos >> _round;
        if(type == 2)
            ++ _finals[0][_finalIdx[0][_round][fidx]].support;
        else
            ++ _finals[1][_finalIdx[1][_round][fidx]].support;
    }
}

void TeamArenaMgr::applyTeam(Player * player, std::string& name)
{
    if(player == NULL) return;
    UInt8 res = 0;
    TeamArenaData * tad = globalNamedTeamArena[player->fixName(name)];
    if(player->GetLev() < LIMIT_LEVEL)
        res = 6;
    else if(player->getTeamArena())
        res = 1;
    else if(!tad || !tad->leader)
        res = 2;
    else if(tad->isFull())
        res = 3;
    if(res == 0)
    {
        UInt64 pid = player->getId();
        std::map<UInt64, TeamPendingMember>::iterator it = tad->pendingMap.find(pid);
        if(it != tad->pendingMap.end())
            res = 4;
        else if(tad->pendingMap.size() > 99)
            res = 5;
        if(res == 0)
        {
            TeamPendingMember tpm = TeamPendingMember(player, TimeUtil::Now());
            tad->broadcastPendingMemberInfo(tpm);
            tad->pendingMap.insert(std::make_pair(pid, tpm));

            DB5().PushUpdateData("INSERT INTO `team_pending_player` (`teamId`, `playerId`, `optime`) VALUES (%" I64_FMT "u, %" I64_FMT "u, %u)", tad->getId(), pid, tpm.opTime);
        }
    }

    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(16) << res;
    st << Stream::eos;
    player->send(st);
}

void TeamArenaMgr::listTeamPending( Player * player )
{
    if(player == NULL) return;
    TeamArenaData * tad = player->getTeamArena();
    if(tad == NULL) return;
	tad->purgePending();
	Stream st(REP::SERVER_ARENA_OP);
	st << static_cast<UInt8>(18) << static_cast<UInt8>(0);
    size_t offset = st.size();
	UInt8 cnt = 0;
    st << cnt;
    std::map<UInt64, TeamPendingMember>::iterator it = tad->pendingMap.begin();
	while(it != tad->pendingMap.end())
	{
		TeamPendingMember tpm = it->second;
		if(tpm.player->getTeamArena() != NULL)
		{
			DB1().PushUpdateData("DELETE FROM `team_pending_player` WHERE `teamId` = %" I64_FMT "u AND `playerId` = %" I64_FMT "u", tad->getId(), tpm.player->getId());
			tad->pendingMap.erase(it ++ );
		}
        else
        {
            st << tpm.player->getId() << tpm.player->getName();
            st << tpm.player->GetLev() << tpm.opTime;
            ++ cnt;
            ++ it;
        }
	}
    st.data<UInt8>(offset) = cnt;
	st << Stream::eos;
	player->send(st);
}

void TeamArenaMgr::acceptApply( Player* player, UInt64 pid )
{
    if(player == NULL) return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad || tad->leader != player || tad->isFull())
        return;
    std::map<UInt64, TeamPendingMember>::iterator it = tad->pendingMap.find(pid);
    if(it == tad->pendingMap.end())
        return;
    Player * applyer = globalPlayers[pid];
    if(!applyer) return;

    bool res = addTeamMember(player, applyer);
	tad->pendingMap.erase(it);
	DB1().PushUpdateData("DELETE FROM `team_pending_player` WHERE `teamId` = %" I64_FMT "u AND `playerId` = %" I64_FMT "u", tad->getId(), pid);

    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(19);
    st << static_cast<UInt8>(0) << pid;
    st << static_cast<UInt8>(res ? 0 : 1);
    st << Stream::eos;
    player->send(st);
}

void TeamArenaMgr::declineApply( Player* player, UInt64 pid )
{
    if(player == NULL) return;
    TeamArenaData * tad = player->getTeamArena();
    if(!tad || tad->leader != player)
        return;
    std::map<UInt64, TeamPendingMember>::iterator it = tad->pendingMap.find(pid);
    if(it == tad->pendingMap.end())
        return;
	tad->pendingMap.erase(it);
	DB1().PushUpdateData("DELETE FROM `team_pending_player` WHERE `teamId` = %" I64_FMT "u AND `playerId` = %" I64_FMT "u", tad->getId(), pid);

    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(19);
    st << static_cast<UInt8>(1) << pid;
    st << Stream::eos;
    player->send(st);
}

struct EnumStruct
{
	Stream& st;
	UInt16 idx;
	UInt16 start;
	UInt16 end;
};

bool teamEnum(TeamArenaData * tad, EnumStruct * es)
{
	if(es->idx >= es->end)
	{
		return false;
	}
	if(es->idx >= es->start)
	{
		tad->appendListInfo(es->st);
	}
	++ es->idx;
	return true;
}

void TeamArenaMgr::listAllTeam(Player * player, UInt16 idx, UInt8 cnt)
{
    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(17);
    UInt16 size = globalTeamArena.size();
	if(idx >= size)
	{
		idx = size;
		cnt = 0;
	}
	else
	{
		if(idx + cnt > size)
			cnt = size - idx;
	}
	st << size << idx << cnt;

	EnumStruct es = {st, 0, idx, static_cast<UInt16>(idx + cnt)};
    globalTeamArena.enumerate(teamEnum, &es);

	st << Stream::eos;
	player->send(st);
}

void TeamArenaMgr::searchTeam(Player * player, std::string& name, UInt8 type)
{
    if(player == NULL) return;
    TeamArenaData * tad = NULL;
    name = player->fixName(name);
    if(type)
    {
        Player * leader = globalNamedPlayers[name];
        if(!leader) return;
        tad = leader->getTeamArena();
        if(!tad || tad->leader != leader)
            return;
    }
    else
    {
        tad = globalNamedTeamArena[name];
        if(!tad) return;
    }
    Stream st(REP::SERVER_ARENA_OP);
    st << static_cast<UInt8>(17);
	UInt16 c = 1, idx = 0;
	UInt8 cnt = 1;
	st << c << idx << cnt;
	tad->appendListInfo(st);

	st << Stream::eos;
	player->send(st);
}

}

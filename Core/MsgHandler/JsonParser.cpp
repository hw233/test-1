
#ifdef _FB
#elif defined(_VT)
#else

#include "Config.h"
#include "JsonParser.h"
#include "GObject/Player.h"
#include "MsgID.h"
#include "Common/BinaryReader.h"
#include "GObject/Leaderboard.h"
#include "GObject/ActivityMgr.h"
#include "GObject/TownDeamon.h"
#include "GObject/Leaderboard.h"

extern "C" {
//#include "bits.h"
//#include "debug.h"
//#include "printbuf.h"
//#include "json_t.h"
//#include "json_tokener.h"
//#include "json_util.h"
#include "../../tools/json-1.4/json.h"
}

#define SERVICE "OP_SSCQ"
#define EMPTY "{head:{},body:{}}"

#define EUNKNOW (-100)
#define EPLAYER_NOT_EXIST (1)

#define GET_STRING(obj, key, dst, maxlen) \
{ \
    json_t* val = json_find_first_label(obj, key); \
    if (val && val->child && val->child->text) \
    { \
        int len = strlen(val->child->text); \
        if (len > maxlen) \
        len = maxlen; \
        memcpy(dst, val->child->text, len); \
    } \
}

struct JsonHead
{
    UInt32 length;
    UInt32 cmd;
    UInt32 seqid;
    char   name[16+1];
    UInt32 sendTime;
    UInt32 version;
    char   auth[32+1];
    UInt32 retcode;
    char   errmsg[100+1];
};

json_t* my_json_new_number(int number)
{
    char buf[1024] = {0};
    snprintf(buf, sizeof(buf), "%d", number);
    return json_new_number(buf);
}

bool parseHead(json_t* obj, JsonHead* head)
{
    if (!obj || !head)
        return false;

    json_t* hdr = json_find_first_label(obj, "head");
    if (!hdr)
        return false;
    if (!hdr->child)
        return false;

    hdr = hdr->child;
    json_t* val = json_find_first_label(hdr, "uiPacketLen");
    if (val && val->child && val->child->text)
        head->length = atoi(val->child->text);

    val = json_find_first_label(hdr, "uiCmdid");
    if (val && val->child && val->child->text)
        head->cmd = atoi(val->child->text);

    val = json_find_first_label(hdr, "uiSeqid");
    if (val && val->child && val->child->text)
        head->seqid = atoi(val->child->text);

    GET_STRING(hdr, "szServiceName", head->name, 16);

    val = json_find_first_label(hdr, "uiSendTime");
    if (val && val->child && val->child->text)
        head->sendTime = atoi(val->child->text);

    val = json_find_first_label(hdr, "uiVersion");
    if (val && val->child && val->child->text)
        head->version = atoi(val->child->text);

    GET_STRING(hdr, "ucAuthenticate", head->auth, 32);

    return true;
}

void fillHead(json_t* head, JsonHead* hdr)
{
    json_insert_pair_into_object(head, "uiPacketLen", my_json_new_number(hdr->length));
    json_insert_pair_into_object(head, "uiCmdid", my_json_new_number(hdr->cmd));
    json_insert_pair_into_object(head, "uiSeqid", my_json_new_number(hdr->seqid));
    json_insert_pair_into_object(head, "szServiceName", json_new_string(hdr->name));
    json_insert_pair_into_object(head, "uiSendTime", my_json_new_number(hdr->sendTime));
    json_insert_pair_into_object(head, "uiVersion", my_json_new_number(hdr->version));
    json_insert_pair_into_object(head, "ucAuthenticate", json_new_string(hdr->auth));
    json_insert_pair_into_object(head, "iResult", my_json_new_number(hdr->retcode));
    json_insert_pair_into_object(head, "szRetErrMsg", json_new_string(hdr->errmsg));
}

std::string fixPlayerName(std::string name)
{
    if (cfg.merged && name.length())
    {
        int len = name.size() - 1;
        for (; len > 0; --len)
        {
            if (static_cast<UInt8>(name[len]) >= 32)
                break;
        }
        name.resize(len+1);
    }
    return name;
}

int query_rolelist_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;

    body = body->child;
    if (!body)
        return EUNKNOW;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    playerid = atoll(playerId);
    //if (cfg.merged)
    //    playerid |= (areaid << 48);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    json_insert_pair_into_object(retbody, "ullRoleId", json_new_string(playerId));
    json_insert_pair_into_object(retbody, "szRoleName", json_new_string(fixPlayerName(player->getName()).c_str()));

    head->cmd = 2;
    return 0;
}

int query_rolebaseinfo_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;

    body = body->child;
    if (!body)
        return EUNKNOW;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    json_insert_pair_into_object(retbody, "szRoleName", json_new_string(fixPlayerName(player->getName()).c_str()));
    json_insert_pair_into_object(retbody, "ucGender", my_json_new_number(player->IsMale()?1:2));
    char title[32] = {0};
    snprintf(title, sizeof(title), "%u", player->getTitle_noCheck());
    json_insert_pair_into_object(retbody, "szCharTitle", json_new_string(title));
    json_insert_pair_into_object(retbody, "ucJob", my_json_new_number(player->GetClass()));
    json_insert_pair_into_object(retbody, "ucNation", my_json_new_number(player->getCountry()));
    json_insert_pair_into_object(retbody, "uiCurHP", my_json_new_number(player->getMainHP()));
    json_insert_pair_into_object(retbody, "uiQuality", my_json_new_number(player->getMainPExp()));
    json_insert_pair_into_object(retbody, "uiReputation", my_json_new_number(0));
    json_insert_pair_into_object(retbody, "uiMoney", my_json_new_number(player->getGold()));
    json_insert_pair_into_object(retbody, "uiGold", my_json_new_number(player->getTael()));
    json_insert_pair_into_object(retbody, "usLevel", my_json_new_number(player->GetLev()));
    json_insert_pair_into_object(retbody, "usVip", my_json_new_number(player->getVipLevel()));
    json_insert_pair_into_object(retbody, "uiRegisterTime", my_json_new_number(player->getCreated()));
    json_insert_pair_into_object(retbody, "uiLastLoginTime", my_json_new_number(player->getLastOnline()));

    head->cmd = 4;
    return 0;
}

int add_playeritem_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;
    UInt32 itemid = 0;
    Int32 inum = 0;

    body = body->child;
    if (!body)
        return EUNKNOW;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    val = json_find_first_label(body, "iNum");
    if (val && val->child && val->child->text)
        inum = atoi(val->child->text);

    val = json_find_first_label(body, "uiItemId");
    if (val && val->child && val->child->text)
        itemid = atoi(val->child->text);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        json_insert_pair_into_object(retbody, "iResult", my_json_new_number(1));
        json_insert_pair_into_object(retbody, "szRetMsg", json_new_string(err.c_str()));
        return EPLAYER_NOT_EXIST;
    }

    struct AddItem
    {
        UInt16 itemid;
        UInt16 itemnum;
    } ai;

    ai.itemid = itemid;
    ai.itemnum = inum;
    GameMsgHdr hdr(0x248, player->getThreadId(), player, sizeof(ai));
    GLOBAL().PushMsg(hdr, &ai);

    head->cmd = 6;
    json_insert_pair_into_object(retbody, "iResult", my_json_new_number(0));
    json_insert_pair_into_object(retbody, "szRetMsg", json_new_string("success"));
    return 0;
}
int query_town_ranking_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    body = body->child;
    if (!body)
        return EUNKNOW;

    UInt32 areaid = 0;
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    std::vector<GObject::TownRankingInfoList>& townVec = GObject::leaderboard.getTownList();
    UInt32 count = townVec.size();

    json_insert_pair_into_object(retbody, "uiCount", my_json_new_number(count));
    json_t* arr = json_new_array();
    if (arr)
    {
        for (UInt32 i = 0; i < count; ++i)
        {
            GObject::TownRankingInfoList& info = townVec[i];
            json_t* obj = json_new_object();
            if (obj)
            {
                char playerId[64] = {0};
                char time[64] = {0};
                sprintf(playerId, "%"I64_FMT"u", info.id);
                snprintf(time, sizeof(time), "%"I64_FMT"u", info.reachTime);
                std::string clanName = fixPlayerName(info.clanName);
                if (clanName.empty())
                    clanName = "-";
                json_insert_pair_into_object(obj, "ullRoleId", json_new_string(playerId));
                json_insert_pair_into_object(obj, "szRoleName", json_new_string(fixPlayerName(info.name).c_str()));
                json_insert_pair_into_object(obj, "ucCamp", my_json_new_number(info.country));
                json_insert_pair_into_object(obj, "szFactionName", json_new_string(clanName.c_str()));
                json_insert_pair_into_object(obj, "usLevel", my_json_new_number(info.roleLevel));
                json_insert_pair_into_object(obj, "uiLayers", my_json_new_number(info.value));
                json_insert_pair_into_object(obj, "uiReachedTime", json_new_string(time));
                json_insert_child(arr, obj);
            }
        }
        json_insert_pair_into_object(retbody, "pSealDevilList", arr);
    }
    head->cmd = 64;
    return 0;
}


int query_ranking_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    body = body->child;
    if (!body)
        return EUNKNOW;

    UInt32 areaid = 0;
    UInt32 type = 0;
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    val = json_find_first_label(body, "ucType");
    if (val && val->child && val->child->text)
        type = atoi(val->child->text);

    UInt32 count = 0;
    std::vector<GObject::RankingInfoList>* levelVec = NULL;
    if (type == 1)
        levelVec = GObject::leaderboard.getLevelList();
    else if (type == 2)
        levelVec = GObject::leaderboard.getAthleticsList();
    else if (type == 3)
        levelVec = GObject::leaderboard.getAchievementList();

    if (NULL == levelVec)
        return EUNKNOW;

    count = levelVec->size();
    json_insert_pair_into_object(retbody, "uiCount", my_json_new_number(count));
    json_t* arr = json_new_array();
    if (arr)
    {
        for (UInt32 i = 0; i < count; ++i)
        {
            GObject::RankingInfoList& info = (*levelVec)[i];
            json_t* obj = json_new_object();
            if (obj)
            {
                char playerId[64] = {0};
                char exp[64] = {0};
                sprintf(playerId, "%"I64_FMT"u", info.id);
                sprintf(exp, "%"I64_FMT"u", info.value);
                std::string clanName = fixPlayerName(info.clanName);
                if (clanName.empty())
                    clanName = "-";
                
                json_insert_pair_into_object(obj, "ullRoleId", json_new_string(playerId));
                json_insert_pair_into_object(obj, "szRoleName", json_new_string(fixPlayerName(info.name).c_str()));
                json_insert_pair_into_object(obj, "ucCamp", my_json_new_number(info.country));
                json_insert_pair_into_object(obj, "szFactionName", json_new_string(clanName.c_str()));
                json_insert_pair_into_object(obj, "usLevel", my_json_new_number(info.roleLevel));
                json_insert_pair_into_object(obj, "uiRankValue", json_new_string(exp));
                json_insert_child(arr, obj);
            }
        }
        json_insert_pair_into_object(retbody, "pRankingInfoList", arr);
    }
    head->cmd = 28;
    return 0;
}

int query_inviter_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;

    body = body->child;
    if (!body)
        return EUNKNOW;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    std::set<GObject::Player *>& inviters = player->getInviters();
    UInt32 count = inviters.size();
    json_insert_pair_into_object(retbody, "pInviteList_count", my_json_new_number(count));
    json_t* arr = json_new_array();
    if (arr)
    {
        std::set<GObject::Player *>::iterator iter;
        for (iter = inviters.begin(); iter != inviters.end(); ++iter)
        {
            GObject::Player* pl = *iter; 
            json_t* obj = json_new_object();
            if (obj && pl)
            {
                char inviterId[64] = {0};
                sprintf(inviterId, "%"I64_FMT"u", pl->getId());
                
                json_insert_pair_into_object(obj, "ullRoleId", json_new_string(inviterId));
                json_insert_pair_into_object(obj, "szRoleName", json_new_string(fixPlayerName(pl->getName()).c_str()));
                json_insert_pair_into_object(obj, "uiCreateTime", my_json_new_number(pl->getCreated()));
                json_insert_child(arr, obj);
            }
        }
        json_insert_pair_into_object(retbody, "pInviteList", arr);
    }
 
    head->cmd = 108;
    return 0;
}

int query_fighters_battle_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;

    body = body->child;
    if (!body)
        return EUNKNOW;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }
    std::map<UInt32, GObject::Fighter *>& fm = player->getFighterMap();    
    UInt32 count = fm.size();
    json_insert_pair_into_object(retbody, "ullRoleId", json_new_string(playerId));
    json_insert_pair_into_object(retbody, "szRoleName", json_new_string(fixPlayerName(player->getName()).c_str()));
    json_insert_pair_into_object(retbody, "pSummonList_count", my_json_new_number(count));
    json_t* arr = json_new_array();
    if (arr)
    {
        std::map<UInt32, GObject::Fighter *>::iterator iter;
        for (iter = fm.begin(); iter != fm.end(); ++iter)
        {
            GObject::Fighter* fgt = iter->second; 
            json_t* obj = json_new_object();
            if (obj && fgt)
            {
                UInt32 fgtId = fgt->getId();
                UInt32 battle = fgt->getBattlePoint_Dirty();
                json_insert_pair_into_object(obj, "uiSummonId", my_json_new_number(fgtId));
                json_insert_pair_into_object(obj, "uiSumBttlEffctv", my_json_new_number(battle));
                json_insert_child(arr, obj);
            }
        }
        json_insert_pair_into_object(retbody, "pSummonList", arr);
    }
 
    head->cmd = 110;
    return 0;
}


int query_pagoda_rsq(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    body = body->child;
    if (!body)
        return EUNKNOW;

    UInt32 areaid = 0;
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    const std::vector<GObject::LeaderboardTowndown>& towndown = GObject::leaderboard.getTowndown();
    UInt32 count = towndown.size();
    json_insert_pair_into_object(retbody, "uiCount", my_json_new_number(count));
    json_t* arr = json_new_array();
    if (arr)
    {
        for (UInt32 i = 0; i < count; ++i)
        {
            char playerId[32] = {0};
            json_t* obj = json_new_object();
            if (obj)
            {
                snprintf(playerId, sizeof(playerId), "%"I64_FMT"u", towndown[i].id);
                json_insert_pair_into_object(obj, "ullRoleId", json_new_string(playerId));
                json_insert_pair_into_object(obj, "szRoleName", json_new_string(fixPlayerName(towndown[i].name).c_str()));
                json_insert_pair_into_object(obj, "uiLayers", my_json_new_number(towndown[i].level));
                json_insert_pair_into_object(obj, "uiReachedTime", my_json_new_number(towndown[i].time));
                json_insert_child(arr, obj);
            }
        }
        json_insert_pair_into_object(retbody, "pPagodaList", arr);
    }

    head->cmd = 56;
    return 0;
}

int query_gangcopy_complete_layer_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    body = body->child;
    if (!body)
        return EUNKNOW;

    UInt32 areaid = 0;
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    const std::vector<GObject::LeaderboardClanCopy>& clancopy = GObject::leaderboard.getClanCopy();
    UInt32 count = clancopy.size();
    json_insert_pair_into_object(retbody, "uiCount", my_json_new_number(count));
    json_t* arr = json_new_array();
    if (arr)
    {
        for (UInt32 i = 0; i < count; ++i)
        {
            char id[32] = {0};
            char time[32] = {0};
            json_t* obj = json_new_object();
            if (obj)
            {
                snprintf(id, sizeof(id), "%"I64_FMT"u", clancopy[i].id);
                json_insert_pair_into_object(obj, "uiFactionID", json_new_string(id));
                json_insert_pair_into_object(obj, "szFactionName", json_new_string(fixPlayerName(clancopy[i].name).c_str()));
                json_insert_pair_into_object(obj, "uiFactionMaxLevel", my_json_new_number(clancopy[i].level));
                snprintf(time, sizeof(time), "%"I64_FMT"u", clancopy[i].time);
                json_insert_pair_into_object(obj, "uiFactiontime2MaxLvl", json_new_string(time));
                json_insert_child(arr, obj);
            }
        }
        json_insert_pair_into_object(retbody, "pGangCopyOverList", arr);
    }

    head->cmd = 58;
    return 0;
}
int query_clancopy_ranking_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    body = body->child;
    if (!body)
        return EUNKNOW;

    UInt32 areaid = 0;
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    const std::vector<GObject::ClanCopyRankingInfoList>& clancopy = GObject::leaderboard.getClanCopyList();
    UInt32 count = clancopy.size();
    json_insert_pair_into_object(retbody, "uiCount", my_json_new_number(count));
    json_t* arr = json_new_array();
    if (arr)
    {
        for (UInt32 i = 0; i < count; ++i)
        {
            char time[32] = {0};
            json_t* obj = json_new_object();
            if (obj)
            {
                const GObject::ClanCopyRankingInfoList& info = clancopy[i];
                json_insert_pair_into_object(obj, "szFactionName", json_new_string(fixPlayerName(info.name).c_str()));
                json_insert_pair_into_object(obj, "szBangzhu", json_new_string(fixPlayerName(info.leaderName).c_str()));
                json_insert_pair_into_object(obj, "usLevel", my_json_new_number(info.level));
                json_insert_pair_into_object(obj, "usNum", my_json_new_number(info.memberCount));
                json_insert_pair_into_object(obj, "ucCamp", my_json_new_number(info.country));
                json_insert_pair_into_object(obj, "uiLayers", my_json_new_number(info.value));
                snprintf(time, sizeof(time), "%"I64_FMT"u", info.reachTime);
                json_insert_pair_into_object(obj, "uiReachedTime", json_new_string(time));
                json_insert_child(arr, obj);
            }
        }
        json_insert_pair_into_object(retbody, "pGangCopyList", arr);
    }

    head->cmd = 66;
    return 0;
}

int query_clanbattle_ranking_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    body = body->child;
    if (!body)
        return EUNKNOW;

    UInt32 areaid = 0;
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    const std::vector<GObject::ClanBattleRankingInfoList>& clanbattle = GObject::leaderboard.getClanBattleList();
    UInt32 count = clanbattle.size();
    json_insert_pair_into_object(retbody, "uiCount", my_json_new_number(count));
    json_t* arr = json_new_array();
    if (arr)
    {
        for (UInt32 i = 0; i < count; ++i)
        {
            json_t* obj = json_new_object();
            if (obj)
            {
                const GObject::ClanBattleRankingInfoList& info = clanbattle[i];
                json_insert_pair_into_object(obj, "szFactionName", json_new_string(fixPlayerName(info.name).c_str()));
                json_insert_pair_into_object(obj, "szBangzhu", json_new_string(fixPlayerName(info.leaderName).c_str()));
                json_insert_pair_into_object(obj, "usLevel", my_json_new_number(info.level));
                json_insert_pair_into_object(obj, "usNum", my_json_new_number(info.memberCount));
                json_insert_pair_into_object(obj, "ucCamp", my_json_new_number(info.country));
                json_insert_pair_into_object(obj, "uiIntegral", my_json_new_number(info.value));
                json_insert_child(arr, obj);
            }
        }
        json_insert_pair_into_object(retbody, "pGangWarList", arr);
    }

    head->cmd = 68;
    return 0;
}


int query_activity_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    body = body->child;
    if (!body)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt64 playerid = 0;
    UInt32 areaid = 0;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    json_insert_pair_into_object(retbody, "szOpenId", json_new_string(openid));
    json_insert_pair_into_object(retbody, "szRoleName", json_new_string(fixPlayerName(player->getName()).c_str()));
    json_insert_pair_into_object(retbody, "ullRoleId", json_new_string(playerId));
    json_insert_pair_into_object(retbody, "uiActval", my_json_new_number(player->GetStrengthenMgr()?player->GetStrengthenMgr()->GetSouls():0));

    head->cmd = 60;
    return 0;
}

int query_role_pagoda_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    body = body->child;
    if (!body)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt64 playerid = 0;
    UInt32 areaid = 0;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    json_insert_pair_into_object(retbody, "ullRoleId", json_new_string(playerId));
    json_insert_pair_into_object(retbody, "szRoleName", json_new_string(fixPlayerName(player->getName()).c_str()));
    json_insert_pair_into_object(retbody, "uiLayers", my_json_new_number(player->getDeamonPlayerData()?player->getDeamonPlayerData()->maxLevel:0));

    head->cmd = 62;
    return 0;
}

int query_event_state_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;
    UInt32 eventid = 0;

    body = body->child;
    if (!body)
        return EUNKNOW;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    val = json_find_first_label(body, "uiEventId");
    if (val && val->child && val->child->text)
        eventid = atoi(val->child->text);

    json_insert_pair_into_object(retbody, "ucState", my_json_new_number(player->getEventState(eventid)));

    head->cmd = 70;
    return 0;
}

int do_item_pay_req(JsonHead* head, json_t* body, json_t* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;
    UInt32 itemId = 0;
    UInt32 itemCount = 0;
    UInt32 price = 0;

    body = body->child;
    if (!body)
        return EUNKNOW;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    json_t* val = json_find_first_label(body, "uiAreaId");
    if (val && val->child && val->child->text)
        areaid = atoi(val->child->text);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    val = json_find_first_label(body, "uiItemId");
    if (val && val->child && val->child->text)
        itemId = atoi(val->child->text);

    val = json_find_first_label(body, "uiItemCount");
    if (val && val->child && val->child->text)
        itemCount = atoi(val->child->text);

    val = json_find_first_label(body, "uiPrice");
    if (val && val->child && val->child->text)
        price = atoi(val->child->text);

    int ret = player->IDIPBuy(itemId, itemCount, price, err);
    json_insert_pair_into_object(retbody, "iResult", my_json_new_number(ret));
    json_insert_pair_into_object(retbody, "szRetMsg", json_new_string(err.c_str()));

    head->cmd = 72;
    return 0;
}

void jsonParser2(void * buf, int len, Stream& st)
{
	BinaryReader br(buf, len);
    //CHKKEY();
    UInt8 hash[64] = {0};
    if (!br.read(hash, 36))
        return;

    std::string json;
    br >> json;

    //TRACE_LOG("JSON RECV: %s\n", json.c_str());
#ifdef _DEBUG
    //fprintf(stderr, "JSON RECV: %s\n", json.c_str());
#endif
    int ret = EUNKNOW;
    std::string err;

    JsonHead head = {0,};
    json_t* obj = NULL;
    json_t* body = NULL;

    json_t* retobj = NULL;
    json_t* rethead = NULL;
    json_t* retbody = NULL;

    // setlocale(LC_ALL, "");

    retobj = json_new_object();
    if (!retobj)
        goto _error1;

    rethead = json_new_object();
    if (!rethead)
        goto _error1;

    retbody = json_new_object();
    if (!retbody)
    {
        json_free_value(&rethead);
        goto _error1;
    }

    enum json_error jerr;
    if ((jerr = json_parse_document(&obj, (char*)json.c_str())) != JSON_OK || !parseHead(obj, &head))
    {
        ++head.cmd; // XXX:
        err += "request error.";
        goto _error;
    }

    body = json_find_first_label(obj, "body");
    if (!body)
        goto _error;

    switch (head.cmd)
    {
        case 1:
            ret = query_rolelist_req(&head, body, retbody, err);
            break;
        case 3:
            ret = query_rolebaseinfo_req(&head, body, retbody, err);
            break;
        case 5:
            ret = add_playeritem_req(&head, body, retbody, err);
            break;
        case 27:
            ret = query_ranking_req(&head, body, retbody, err);
            break;
        case 55:
            ret = query_pagoda_rsq(&head, body, retbody, err);
            break;
        case 57:
            ret = query_gangcopy_complete_layer_req(&head, body, retbody, err);
            break;
        case 59:
            ret = query_activity_req(&head, body, retbody, err);
            break;
        case 61:
            ret = query_role_pagoda_req(&head, body, retbody, err);
            break;
        case 63:
            ret = query_town_ranking_req(&head, body, retbody, err);
            break;
        case 65:
            ret = query_clancopy_ranking_req(&head, body, retbody, err);
            break;
        case 67:
            ret = query_clanbattle_ranking_req(&head, body, retbody, err);
            break;
        case 69:
            ret = query_event_state_req(&head, body, retbody, err);
            break;
        case 71:
            ret = do_item_pay_req(&head, body, retbody, err);
            break;
        case 107:
            ret = query_inviter_req(&head, body, retbody, err);
            break;
        case 109:
            ret = query_fighters_battle_req(&head, body, retbody, err);
            break;
       
        default:
            break;
    }

_error:
    {
        head.retcode = ret;
        head.sendTime = time(NULL);
        snprintf(head.errmsg, sizeof(head.errmsg), "%s", err.c_str());
        snprintf(head.name, sizeof(head.name), "%s", SERVICE);

        fillHead(rethead, &head);

        json_insert_pair_into_object(retobj, "head", rethead);
        json_insert_pair_into_object(retobj, "body", retbody);

        char* retstr = 0;
        json_tree_to_string(retobj, &retstr);
        //TRACE_LOG("JOSN SEND: %s\n", retstr?retstr:"null");
#ifdef _DEBUG
        //fprintf(stderr, "JOSN SEND: %s\n", retstr?retstr:"null");
#endif

        st << std::string(retstr?retstr:EMPTY);
        if (retstr)
            free(retstr);
    }

_error1:
    if (retobj)
        json_free_value(&retobj);

    return;
}

#endif


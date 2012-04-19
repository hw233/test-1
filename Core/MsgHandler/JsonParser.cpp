
#ifdef _FB
#else

#include "Config.h"
#include "JsonParser.h"
#include "GObject/Player.h"
#include "MsgID.h"

extern "C" {
#include "bits.h"
#include "debug.h"
#include "printbuf.h"
#include "json_object.h"
#include "json_tokener.h"
#include "json_util.h"
}

#define SERVICE "OP_SSCQ"
#define EMPTY "{head:{},body:{}}"

#define EUNKNOW (-100)
#define EPLAYER_NOT_EXIST (1)

#define GET_STRING(obj, key, dst, maxlen) \
{ \
    struct json_object* val = json_object_object_get(obj, key); \
    if (val) \
    { \
        const char* str = json_object_get_string(val); \
        if (str) \
        { \
            int len = strlen(str); \
            if (len > maxlen) \
                len = maxlen; \
            memcpy(dst, str, len); \
        } \
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

bool parseHead(struct json_object* obj, JsonHead* head)
{
    if (!obj || !head)
        return false;

    struct json_object* hdr = json_object_object_get(obj, "head");
    if (!hdr)
        return false;

    struct json_object* val = json_object_object_get(hdr, "uiPacketLen");
    if (val)
        head->length = json_object_get_int(val);

    val = json_object_object_get(hdr, "uiCmdid");
    if (val)
        head->cmd = json_object_get_int(val);

    val = json_object_object_get(hdr, "uiSeqid");
    if (val)
        head->seqid = json_object_get_int(val);

    GET_STRING(hdr, "szServiceName", head->name, 16);

    val = json_object_object_get(hdr, "uiSendTime");
    if (val)
        head->sendTime = json_object_get_int(val);

    val = json_object_object_get(hdr, "uiVersion");
    if (val)
        head->version = json_object_get_int(val);

    GET_STRING(hdr, "ucAuthenticate", head->auth, 32);

    return true;
}

void fillHead(struct json_object* head, JsonHead* hdr)
{
    json_object_object_add(head, "uiPacketLen", json_object_new_int(hdr->length));
    json_object_object_add(head, "uiCmdid", json_object_new_int(hdr->cmd));
    json_object_object_add(head, "uiSeqid", json_object_new_int(hdr->seqid));
    json_object_object_add(head, "szServiceName", json_object_new_string(hdr->name));
    json_object_object_add(head, "uiSendTime", json_object_new_int(hdr->sendTime));
    json_object_object_add(head, "uiVersion", json_object_new_int(hdr->version));
    json_object_object_add(head, "ucAuthenticate", json_object_new_string(hdr->auth));
    json_object_object_add(head, "iResult", json_object_new_int(hdr->retcode));
    json_object_object_add(head, "szRetErrMsg", json_object_new_string(hdr->errmsg));
}

int query_rolelist_req(JsonHead* head, struct json_object* body, struct json_object* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    struct json_object* val = json_object_object_get(body, "uiAreaId");
    if (val)
        areaid = json_object_get_int(val);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    json_object_object_add(retbody, "ullRoleId", json_object_new_string(playerId));
    json_object_object_add(retbody, "szRoleName", json_object_new_string(player->getName().c_str()));

    head->cmd = 2;
    return 0;
}

int query_rolebaseinfo_req(JsonHead* head, struct json_object* body, struct json_object* retbody, std::string& err)
{
    if (!head || !body || !retbody)
        return EUNKNOW;

    char openid[36] = {0};
    char playerId[32] = {0};
    UInt32 areaid = 0;
    UInt64 playerid = 0;

    GET_STRING(body, "szOpenId", openid, 36);
    GET_STRING(body, "playerId", playerId, 32);
    struct json_object* val = json_object_object_get(body, "uiAreaId");
    if (val)
        areaid = json_object_get_int(val);

    playerid = atoll(playerId);
    GObject::Player* player = GObject::globalPlayers[playerid];
    if (!player)
    {
        err += "player not exist!";
        return EPLAYER_NOT_EXIST;
    }

    json_object_object_add(retbody, "szRoleName", json_object_new_string(player->getName().c_str()));
    json_object_object_add(retbody, "ucGender", json_object_new_int(player->IsMale()?1:2));
    char title[32] = {0};
    snprintf(title, sizeof(title), "%u", player->getTitle());
    json_object_object_add(retbody, "szCharTitle", json_object_new_string(title));
    json_object_object_add(retbody, "ucJob", json_object_new_int(player->GetClass()));
    json_object_object_add(retbody, "ucNation", json_object_new_int(player->getCountry()));
    json_object_object_add(retbody, "uiCurHP", json_object_new_int(player->getMainHP()));
    json_object_object_add(retbody, "uiQuality", json_object_new_int(player->getMainPExp()));
    json_object_object_add(retbody, "uiReputation", json_object_new_int(0));
    json_object_object_add(retbody, "uiMoney", json_object_new_int(player->getGold()));
    json_object_object_add(retbody, "uiGold", json_object_new_int(player->getTael()));
    json_object_object_add(retbody, "usLevel", json_object_new_int(player->GetLev()));
    json_object_object_add(retbody, "usVip", json_object_new_int(player->getVipLevel()));
    json_object_object_add(retbody, "uiRegisterTime", json_object_new_int(player->getCreated()));
    json_object_object_add(retbody, "uiLastLoginTime", json_object_new_int(player->getLastOnline()));

    head->cmd = 4;
    return 0;
}

void jsonParser(std::string& json, int sessionid)
{
    TRACE_LOG("JSON RECV: %s\n", json.c_str());
#ifdef _DEBUG
    fprintf(stderr, "JSON RECV: %s\n", json.c_str());
#endif
    int ret = EUNKNOW;
    std::string err;

    JsonHead head = {0,};
    struct json_object* obj = NULL;
    struct json_object* body = NULL;

    struct json_object* retobj = NULL;
    struct json_object* rethead = NULL;
    struct json_object* retbody = NULL;

    retobj = json_object_new_object();
    if (!retobj)
        goto _error1;

    rethead = json_object_new_object();
    if (!rethead)
        goto _error1;

    retbody = json_object_new_object();
    if (!retbody)
        goto _error1;

    obj = json_tokener_parse(json.c_str());
    if (!parseHead(obj, &head))
    {
        ++head.cmd; // XXX:
        err += "request error.";
        goto _error;
    }

    body = json_object_object_get(obj, "body");
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

        json_object_object_add(retobj, "head", rethead);
        json_object_object_add(retobj, "body", retbody);

        const char* retstr = json_object_to_json_string(retobj);
        TRACE_LOG("JOSN SEND: %s\n", retstr?retstr:"null");
#ifdef _DEBUG
        fprintf(stderr, "JOSN SEND: %s\n", retstr?retstr:"null");
#endif

        if (sessionid >= 0)
        {
            Stream st(SPEP::JASON);
            st << std::string(retstr?retstr:EMPTY);
            st << Stream::eos;
            NETWORK()->SendMsgToClient(sessionid, st);
        }
    }

_error1:
    if (retobj)
        json_object_put(retobj);
    if (retbody)
        json_object_put(retbody);
    if (rethead)
        json_object_put(rethead);

    return;
}
#endif


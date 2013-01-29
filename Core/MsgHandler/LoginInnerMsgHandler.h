#ifndef _LOGININNERMSGHANDLER_H_
#define _LOGININNERMSGHANDLER_H_

#include "Common/Serialize.h"
#include "MsgTypes.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"

//static unsigned long long failed_cnt = 0;

void OnCheckPackKey( LoginMsgHdr& hdr, const void * data )
{
    struct Key
    {
        GObject::Player* player;
        char key[128];
    };
    Key* key = (Key*)(data);

    UInt8 ret = 0;
    UInt8 type = 0xFF;
    size_t len = strlen(key->key);
#ifndef _WIN32
    if (cfg.GMCheck)
    {
        memcached_return rc;
        initMemcache();
        if (memcinited)
        {
            size_t tlen = 0;
            unsigned int flags = 0;

            int retry = 3;
            while (retry)
            {
                --retry;
                char* rtoken = memcached_get(&memc, key->key, len, &tlen, &flags, &rc);
                if (rc == MEMCACHED_SUCCESS && rtoken && tlen)
                {
                    ret = 0;
                    if (tlen == 1 && rtoken[0] == '0')
                    {
                        if (isdigit(key->key[0]) && key->key[1] == '-')
                            type = key->key[0] - '0';
                        else
                            type = 3;
                    }
                    else
                    {
                        type = 0xFE; // XXX: 已领取
                    }

                    free(rtoken);
                    break;
                }
                else
                {
                    ret = 1;
                    type = 0xFD;
                    usleep(500);
                }
            }
        }

        if (ret == 1)
        {
            TRACE_LOG("key: %s, rc: %u", key->key, rc);
            if (rc == MEMCACHED_HOST_LOOKUP_FAILURE || rc == MEMCACHED_NO_SERVERS || rc == MEMCACHED_SERVER_ERROR || MEMCACHED_SERVER_MARKED_DEAD)
            {
                uninitMemcache();
                initMemcache();
            }
        }

        if (key->player)
        {
            GameMsgHdr hdr(0x2F1, key->player->getThreadId(), key->player, sizeof(type));
            GLOBAL().PushMsg(hdr, &type);

            if (type < 0xF0)
            //if (type != 0xFF)
            {
                char id[32] = {0};
                size_t vlen = snprintf(id, 32, "%"I64_FMT"u", key->player->getId());
                memcached_return_t rc = memcached_set(&memc, key->key, len, id, vlen, (time_t)(60), 0);

                int retry = 2;
                while (rc != MEMCACHED_SUCCESS && retry)
                {
                    rc = memcached_set(&memc, key->key, len, id, vlen, (time_t)(30), 0);
                    --retry;
                }

                if (rc != MEMCACHED_SUCCESS)
                {
                    rc = memcached_delete(&memc, key->key, len, (time_t)0);
                }
            }
        }
    }
    else
#endif // _WIN32
    {
        UInt8 type = 0xFF;
        if (isdigit(key->key[0]) && key->key[1] == '-')
            type = key->key[0] - '0';
        else
            type = 3;

        if (key->player)
        {
            GameMsgHdr hdr(0x2F1, key->player->getThreadId(), key->player, sizeof(type));
            GLOBAL().PushMsg(hdr, &type);
        }
    }
}

void OnSetCrackValue( LoginMsgHdr& hdr, const void * data )
{
    struct CrackValue
    {
        char ip[1024];
        int v;
    };
    CrackValue* p = (CrackValue*)(data);

    initMemcache();
    int v = 0;
    if (memcinited)
    {
        size_t len = 0;
        char key[MEMCACHED_MAX_KEY] = {0};
        char value[32] = {0};

        len = snprintf(key, sizeof(key), "%d_%s_%d", memc_version, p->ip, cfg.serverNum);
        if (MemcachedGet(key, len, value, sizeof(value)))
        {
            v = atoi(value) - 1;
            if (v < 0)
                v = 0;
            setCrackValue(p->ip, v);
        }
    }         
}

#endif // _LOGININNERMSGHANDLER_H_

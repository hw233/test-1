#ifndef _LOGININNERMSGHANDLER_H_
#define _LOGININNERMSGHANDLER_H_

#include "Common/Serialize.h"
#include "MsgTypes.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"

void OnCheckPackKey( LoginMsgHdr& hdr, const void * data )
{
    struct Key
    {
        GObject::Player* player;
        char key[128];
    };
    Key* key = (Key*)(data);

    UInt8 ret = 0;
    UInt8 type = 0;
    size_t len = strlen(key->key);
    if (cfg.GMCheck)
    {
        memcached_return rc;
        initMemcache();
        if (memc)
        {
            size_t tlen = 0;
            unsigned int flags = 0;

            int retry = 3;
            while (retry)
            {
                --retry;
                char* rtoken = memcached_get(memc, key->key, len, &tlen, &flags, &rc);
                if (rc == MEMCACHED_SUCCESS && rtoken && tlen)
                {
                    ret = 0;
                    if (tlen == 1 && rtoken[0] == '0')
                    {
                        if (key->key[0] == '1' && key->key[1] == '-')
                        {
                            type = 1;
                        }
                        else if (key->key[0] == '2' && key->key[1] == '-')
                        {
                            type = 2;
                        }
                        else
                        {
                            type = 3;
                        }
                    }
                    else
                    {
                        type = 4;
                    }

                    free(rtoken);
                    break;
                }
                else
                {
                    ret = 1;
                    usleep(500);
                }
            }
        }

        if (ret)
        {
            TRACE_LOG("key: %s, rc: %u", key->key, rc);
            uninitMemcache();
            initMemcache();
            type = 5;
        }

        if (key->player)
        {
            GameMsgHdr hdr(0x2F1, key->player->getThreadId(), key->player, sizeof(type));
            GLOBAL().PushMsg(hdr, &type);

            if (type == 1 || type == 2)
            {
                char id[32] = {0};
                size_t vlen = snprintf(id, 32, "%"I64_FMT"u", key->player->getId());
                memcached_return_t rc = memcached_set(memc, key->key, len, id, vlen, (time_t)(60), 0);

                int retry = 2;
                while (rc != MEMCACHED_SUCCESS && retry)
                {
                    rc = memcached_set(memc, key->key, len, id, vlen, (time_t)(30), 0);
                    --retry;
                }

                if (rc != MEMCACHED_SUCCESS)
                {
                    rc = memcached_delete(memc, key->key, len, (time_t)0);
                }
            }
        }
    }
    else
    {
        UInt8 type = 1;
        if (key->key[0] == '1' && key->key[1] == '-')
        {
            type = 1;
        }
        else if (key->key[0] == '2' && key->key[1] == '-')
        {
            type = 2;
        }
        else
        {
            type = 3;
        }
        if (key->player)
        {
            GameMsgHdr hdr(0x2F1, key->player->getThreadId(), key->player, sizeof(type));
            GLOBAL().PushMsg(hdr, &type);
        }
    }
}

#endif // _LOGININNERMSGHANDLER_H_

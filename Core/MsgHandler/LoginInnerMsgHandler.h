#ifndef _LOGININNERMSGHANDLER_H_
#define _LOGININNERMSGHANDLER_H_

#include "Common/Serialize.h"
#include "MsgTypes.h"
#include "Server/WorldServer.h"
#include "GObject/Player.h"

void OnCheckPackKey( GameMsgHdr& hdr, const void * data )
{
	MSG_QUERY_PLAYER(player);
    struct Key
    {
        char key[128];
    };
    Key* key = (Key*)(data);

    UInt8 ret = 0;
    memcached_return rc;

    initMemcache();
    if (memc)
    {
        size_t tlen = 0;
        unsigned int flags = 0;
        size_t len = strlen(key->key);

        int retry = 3;
        while (retry)
        {
            --retry;
            char* rtoken = memcached_get(memc, key->key, len, &tlen, &flags, &rc);
            if (rc == MEMCACHED_SUCCESS && rtoken && tlen)
            {
                ret = 0;
                UInt8 type = 0;
                if (tlen == 1 && rtoken[0] == '0')
                {
                    if (key->key[0] == '1' && key->key[1] == '-')
                    {
                        type = 1;
                    }
                    if (key->key[0] == '2' && key->key[1] == '-')
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

                if (type)
                {
                    GameMsgHdr hdr(0x2F1, player->getThreadId(), player, sizeof(type));
                    GLOBAL().PushMsg(hdr, &type);

                    if (type == 1 || type == 2)
                    {
                        char id[32] = {0};
                        size_t vlen = snprintf(id, 32, "%"I64_FMT"u", player->getId());
                        //memcached_set(memc, key->key, len, id, vlen);
                    }
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
    }
}

#endif // _LOGININNERMSGHANDLER_H_

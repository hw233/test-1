
#ifndef _WIN32
#include <libmemcached/memcached.h>
static memcached_st* memc = NULL;
#endif

#ifndef _WIN32
static bool initMemcache()
{
    bool hasServer = false;
    if (!memc)
    {
        memcached_return rc;
        memc = memcached_create(NULL);

        size_t sz = cfg.tokenServer.size();
        for (size_t i = 0; i < sz; ++i)
        {
            memcached_server_st* servers = memcached_server_list_append(NULL, cfg.tokenServer[i].ip.c_str(), cfg.tokenServer[i].port, &rc);
            if (rc == MEMCACHED_SUCCESS)
            {
                rc = memcached_server_push(memc, servers);
                memcached_server_free(servers);
                hasServer = true;
            }
        }

        if (!hasServer)
        {
            memcached_free(memc);
            memc = NULL;
        }
        else
        {
            //err += "can not connect to token server ";
            //err += cfg.tokenServer[i].ip;
        }
    }
    return hasServer;
}

__attribute__((destructor)) static void uninitMemcache()
{
    if (memc)
    {
        memcached_free(memc);
        memc = NULL;
    }
}
#endif // _WIN32


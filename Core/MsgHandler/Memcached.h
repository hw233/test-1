
#ifndef _WIN32

#include <libmemcached/memcached.h>

extern memcached_st* memc;
extern int memc_version;

static const char* MemcachedGet(const char* key, size_t key_size, char* value, size_t size);
static bool MemcachedSet(const char* key, size_t key_size, const char* value, size_t size, int timeout = -1);

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

    if (hasServer)
    {
        memc_version = (rand()*(rand()/132))%0x8FFFFFFF;
        TRACE_LOG("memc_version: %d", memc_version);
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

const char* MemcachedGet(const char* key, size_t key_size, char* value, size_t size)
{
    if (!memc || !key || !key_size || !value || !size)
        return 0;

    size_t tlen = 0;
    unsigned int flags = 0;

    int retry = 3;
    memcached_return rc;
    bool got = false;

    while (retry)
    {
        --retry;
        char* rval = memcached_get(memc, key, key_size, &tlen, &flags, &rc);
        if (rc == MEMCACHED_SUCCESS && rval && tlen)
        {
            tlen = tlen > size ? tlen : size;
            memcpy(value, rval, tlen);
            free(rval);
            got = true;
            break;
        }
        else
        {
            usleep(500);
        }
    }
    TRACE_LOG("memc get: %s => %s", key, value);
    return got?value:NULL;
}

bool MemcachedSet(const char* key, size_t key_size, const char* value, size_t size, int timeout)
{
    if (!memc || !key || !key_size || !value || !size)
        return false;

    memcached_return_t rc = memcached_set(memc, key, key_size, value, size, timeout, 0);

    int retry = 2;
    while (rc != MEMCACHED_SUCCESS && retry)
    {
        rc = memcached_set(memc, key, key_size, value, size, timeout, 0);
        --retry;
    }

    TRACE_LOG("memc set: %s => %s [%d]", key, value, rc);
    return rc == MEMCACHED_SUCCESS;
}

static void setCrackValue(const char* ip, int v)
{
    (void)setCrackValue;
    int timeout = 24*60*60;
    initMemcache();
    if (memc)
    {
        char value[64] = {0};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "%d_%s_%d", memc_version, ip, cfg.serverNum);
        size_t vlen = snprintf(value, sizeof(value), "%d", v);
        MemcachedSet(key, len, value, vlen, timeout);
    }
}

static void setForbidSaleValue(const UInt64 playerId, bool isForbid)
{
    (void)setForbidSaleValue;
    initMemcache();
    if (memc)
    {
        char value[2] = {'0'};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "asss_globallock_%"I64_FMT"u", playerId);
        size_t vlen = 1;
        if (isForbid) value[0] = '1';

        MemcachedSet(key, len, value, vlen, 0);
    }
}

static bool checkForbidSale(const UInt64 playerId)
{
    (void)checkForbidSale;
    initMemcache();
    char value[2] = {0};
    char key[MEMCACHED_MAX_KEY] = {0};
    UInt64 pid = playerId & 0xFFFFFFFFFF;
    size_t len = snprintf(key, sizeof(key), "asss_globallock_%"I64_FMT"u", pid);

    if (memc)
        MemcachedGet(key, len, value, sizeof(value));

    return value[0] == '1';
}
static bool checkCrack(std::string& platform, std::string& ip, UInt64 id)
{
    (void)checkCrack;
    int pf = atoi(platform.c_str());
    if (!pf)
        return false;

    if (ip.empty())
        return false;

    if (pf == 11)
    {
        initMemcache();
        if (memc)
        {
            size_t len = 0;
            char key[MEMCACHED_MAX_KEY] = {0};
            char value[32] = {0};

            len = snprintf(key, sizeof(key), "%d_%s_%d", memc_version, ip.c_str(), cfg.serverNum);
            int v = 0;
            if (MemcachedGet(key, len, value, sizeof(value)))
            {
                v = atoi(value);
                if (v >= 3)
                {
                    TRACE_LOG("id: %"I64_FMT"u from %s of asss_%d is cracking...", id, ip.c_str(), cfg.serverNum);
                    return true;
                }
            }
            v += 1;
            setCrackValue(ip.c_str(), v);
        }
    }

    return false;
}

#endif // _WIN32


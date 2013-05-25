
#ifndef _WIN32

#include <libmemcached/memcached.h>
#include "Common/StringTokenizer.h"
#include "Common/TimeUtil.h"
#include <string>
extern bool memcinited;
extern memcached_st memc;
extern int memc_version;
extern int g_platform_login_number[256];

static const char* MemcachedGet(const char* key, size_t key_size, char* value, size_t size);
static bool MemcachedSet(const char* key, size_t key_size, const char* value, size_t size, int timeout = -1);
static bool initMemcache()
{
    if (!memcinited)
    {
        memcached_return rc;
        memcached_create(&memc);

        size_t sz = cfg.tokenServer.size();
        for (size_t i = 0; i < sz; ++i)
        {
            memcached_server_list_st servers = memcached_server_list_append(NULL, cfg.tokenServer[i].ip.c_str(), cfg.tokenServer[i].port, &rc);
            if (rc == MEMCACHED_SUCCESS)
            {
                rc = memcached_server_push(&memc, servers);
                memcached_server_list_free(servers);
                memcinited = true;
            }
        }
    }

    if (memcinited)
    {
        memc_version = (rand()*(rand()/132))%0x8FFFFFFF;
        TRACE_LOG("memc_version: %d", memc_version);
    }
    return memcinited;
}

__attribute__((destructor)) static void uninitMemcache()
{
    if (memcinited)
    {
        memcinited = false;
        memcached_free(&memc);
        //memcached_servers_reset(&memc);
    }
}

const char* MemcachedGet(const char* key, size_t key_size, char* value, size_t size)
{
    if (!memcinited || !key || !key_size || !value || !size)
        return 0;

    size_t tlen = 0;
    unsigned int flags = 0;

    int retry = 3;
    memcached_return rc;
    bool got = false;

    while (retry)
    {
        --retry;
        char* rval = memcached_get(&memc, key, key_size, &tlen, &flags, &rc);
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
    if (!memcinited || !key || !key_size || !value || !size)
        return false;

    memcached_return_t rc = memcached_set(&memc, key, key_size, value, size, timeout, 0);

    int retry = 2;
    while (rc != MEMCACHED_SUCCESS && retry)
    {
        rc = memcached_set(&memc, key, key_size, value, size, timeout, 0);
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
    if (memcinited)
    {
        char value[64] = {0};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "%d_%s_%d", memc_version, ip, cfg.serverNum);
        size_t vlen = snprintf(value, sizeof(value), "%d", v);
        MemcachedSet(key, len, value, vlen, timeout);
    }
}

static void setForbidSaleValue(const UInt64 playerId, bool isForbid)
//static void setForbidSaleValue(const UInt64 playerId, bool isForbid, UInt32 fTime = 9999999)
{
    (void)setForbidSaleValue;
    initMemcache();
    if (memcinited)
    {
        char value[32] = {'0'};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "asss_globallock_%"I64_FMT"u", playerId);
        if (isForbid) value[0] = '1';
        sprintf(&value[1],"%d", TimeUtil::Now());
/*        {
            if(fTime != 9999999)
            sprintf(&value[1],"%d_%d", TimeUtil::Now(),TimeUtil::Now()+fTime);
            else
            sprintf(&value[1],"%d", TimeUtil::Now());
        }
        */
        size_t vlen = strlen(value);

        MemcachedSet(key, len, value, vlen, 0);
    }
}

static bool checkForbidSale(const UInt64 playerId, std::string& t)
//static bool checkForbidSale(const UInt64 playerId, std::string& fsale, std::string& over)
{
    (void)checkForbidSale;
    initMemcache();
//    std::string t;
    char value[32] = {0};
    char key[MEMCACHED_MAX_KEY] = {0};
    UInt64 pid = playerId & 0xFFFFFFFFFF;
    size_t len = snprintf(key, sizeof(key), "asss_globallock_%"I64_FMT"u", pid);

    if (memcinited)
        MemcachedGet(key, len, value, sizeof(value));
    if (len > 1)
    {
        t = &(value[1]);
    }
    return value[0] == '1';
/*    
    if(value[0]=='0' || value[0] == 0 )
    {
        over = "0";
        return false;
    }
    StringTokenizer tk(t, "_");
    if( tk.count() != 2)
    {
        fsale=tk[0];
       over = "1577808000";    //2020年1月1号
        return true;
    }
    fsale =tk[0];
    over = tk[1];
    if(TimeUtil::Now() > static_cast<UInt32>(atoi( over.c_str() ) ) )
    {
        setForbidSaleValue(playerId,false);
        return false;
    }
    return true;
    */
}

static bool checkCrack(std::string& platform, std::string& ip, UInt64 id)
{
    (void)checkCrack;
    int pf = atoi(platform.c_str());
    if (!pf)
        return false;

    if (ip.empty())
        return false;

//    if (pf == 11 || pf == 17)
    if (pf >= 0 && pf < 255) 
    {
        initMemcache();
        if (memcinited)
        {
            size_t len = 0;
            char key[MEMCACHED_MAX_KEY] = {0};
            char value[32] = {0};

            len = snprintf(key, sizeof(key), "%d_%s_%d", memc_version, ip.c_str(), cfg.serverNum);
            int v = 0;
            if (MemcachedGet(key, len, value, sizeof(value)))
            {
                v = atoi(value);
                /*if (pf == 11 && v >= 3)
                {
                    TRACE_LOG("id: %"I64_FMT"u from %s of asss_%d is cracking...", id, ip.c_str(), cfg.serverNum);
                    return true;
                }
                else if (pf == 17 && v >= 5)
                {
                    TRACE_LOG("id: %"I64_FMT"u from %s of asss_%d is cracking...", id, ip.c_str(), cfg.serverNum);
                    return true;
                }*/
                if (g_platform_login_number[pf] > 0 && v >= g_platform_login_number[pf])
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
static void memLockUser(const UInt64 playerId, UInt32 expireTime)
{
    (void)memLockUser;
    initMemcache();
    if (memcinited)
    {
        char value[32] = {0};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "asss_locklogin_%"I64_FMT"u", playerId);
        size_t vlen = snprintf(value, sizeof(value), "%d", expireTime);

        MemcachedSet(key, len, value, vlen, 0);
    }
}
static void memUnLockUser(const UInt64 playerId)
{
    (void)memUnLockUser;
    initMemcache();
    if (memcinited)
    {
        char value[2] = {'0'};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "asss_locklogin_%"I64_FMT"u", playerId);

        MemcachedSet(key, len, value, 1, 0);
    }
}


static UInt32 getLockUserValue(const UInt64 playerId)
{
    (void)getLockUserValue;
    initMemcache();
    char value[32] = {0};
    char key[MEMCACHED_MAX_KEY] = {0};
    UInt64 pid = playerId & 0xFFFFFFFFFF;
    size_t len = snprintf(key, sizeof(key), "asss_locklogin_%"I64_FMT"u", pid);

    if (memcinited)
        MemcachedGet(key, len, value, sizeof(value));

    UInt32 v = 0;
    v = atoi(value);
    return v;
}

static void setPlatformLogin(UInt8 pf, int v)
{
    (void)setPlatformLogin;
    initMemcache();
    if (memcinited)
    {
        char value[32] = {0};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "asss_loginlimit_%u", pf);
        size_t vlen = snprintf(value, sizeof(value), "%d", v);

        MemcachedSet(key, len, value, vlen, 0);

        g_platform_login_number[pf] = v;
    }
}
static void initPlatformLogin()
{
    (void)initPlatformLogin;
    initMemcache();

    for (int i = 0; i < 256; ++i)
    {
        char value[32] = {0};
        char key[MEMCACHED_MAX_KEY] = {0};
        size_t len = snprintf(key, sizeof(key), "asss_loginlimit_%u", i);
        if (memcinited)
            MemcachedGet(key, len, value, sizeof(value));
        g_platform_login_number[i] = atoi(value);
    }
}
static bool isRPOpenid(const std::string openid)
{
    (void)isRPOpenid;
    initMemcache();
    char value[32] = {0};
    char key[64] = {0};
    size_t len = snprintf(key, sizeof(key), "asss_rp_%s", openid.c_str());;

    if (memcinited)
        MemcachedGet(openid.c_str(), len, value, sizeof(value));

    return value[0] == '1';
}


#endif // _WIN32


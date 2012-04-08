
#include <sys/time.h>
#include <string>
#include <vector>
#include <libmemcached/memcached.h>
#include <stdlib.h>
#include <stdio.h>


static memcached_st* memc = NULL;

struct Sp
{
    std::string ip;
    unsigned short port;
};

static bool initMemcache()
{
    bool hasServer = false;
    if (!memc)
    {
        memcached_return rc;
        memc = memcached_create(NULL);
        std::vector<Sp> tokenServer;
        Sp s;
#if defined(_FB)
        s.ip = "192.168.100.2";
        s.port = 11211;
        tokenServer.push_back(s);
#else
        s.ip = "10.142.16.11";
        s.port = 9146;
        tokenServer.push_back(s);
        s.ip = "10.142.39.27";
        s.port = 9146;
        tokenServer.push_back(s);
#endif
        size_t sz = tokenServer.size();

        for (size_t i = 0; i < sz; ++i) 
        {    
            memcached_server_st* servers = memcached_server_list_append(NULL, tokenServer[i].ip.c_str(), tokenServer[i].port, &rc);
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

        srandom(time(0));
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

void usage(int argc, char* argv[])
{
    fprintf(stderr, "useage: %s 0|1\n\
                        0-sscq_gmtool_key_id\n\
                        1-sscq_recharge_key_id\n", argv[0]);
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        usage(argc, argv);
        return 0;
    }

    int type = atoi(argv[1]);
    if (type != 0 && type != 1)
        return 0;
    while (true)
    {
        initMemcache();
        int ret = 0;
        size_t len = 0;
        size_t tlen = 0;
        unsigned int flags = 0;
        char key[MEMCACHED_MAX_KEY] = {0};

        if (type == 0)
            len = snprintf(key, sizeof(key), "sscq_gmtool_key_id");
        else if (type == 1)
            len = snprintf(key, sizeof(key), "sscq_recharge_key_id");
        char id[64] = {0};
        long long rid = random();
        long long rrid = rid << 32 | (random() | 0x12345);
        size_t vlen = snprintf(id, sizeof(id), "%llu", rrid);
        memcached_return_t rc = memcached_set(memc, key, len, id, vlen, -1, 0);
        int retry = 2;
        while (rc != MEMCACHED_SUCCESS && retry)
        {
            rc = memcached_set(memc, key, len, id, vlen, -1, 0);
            --retry;
        }
        printf("%s\n", id);
        sleep(5*60);
    }
    return 0;
}



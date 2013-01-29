
#include <libmemcached/memcached.h>

bool memcinited = false;
memcached_st memc;
int memc_version = 0;
int g_platform_login_number[256] = {0};



#ifndef BATTLEMANAGER_H_
#define BATTLEMANAGER_H_

#include "Common/Mutex.h"
namespace Battle
{
    class BattleManager
    {
        public:

        private:
            FastMutex _mutex;
            FastMutex _mutex2;
    };
}
#endif // BATTLEMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


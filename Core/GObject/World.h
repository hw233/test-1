
#ifndef WORLD_H_
#define WORLD_H_

#include "MsgHandler/WorldMsgHandler.h"
#include "Server/WorkerThread.h"
#include "Common/Stream.h"
#include "Server/Cfg.h"
#include "Common/TimeUtil.h"
#include "Common/MCached.h"
#include "GObject/GVar.h"
#ifndef _WIN32
#include "kingnet_analyzer.h"
#endif
#include "World.h"
namespace Script
{
    class WorldScript;
    class BattleFormula;
}
namespace GObject
{
    class World
        :public WorkerRunner<WorldMsgHandler>
    {
        public:
            inline UInt8 TID() const { return WORKER_THREAD_WORLD; }
            bool Init();
            void UnInit();
            std::string GetLogName();

        private:
            static void World_Midnight_Check( World * );

        public:

        private:
            Script::WorldScript * _worldScript;
    };
}
#endif // WORLD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


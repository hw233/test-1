#include "Config.h"
#include "World.h"
#include "Common/TimeUtil.h"
#include "Script/WorldScript.h"
#include "Script/BattleFormula.h"
#include "GObject/GVar.h"
namespace GObject
{
    bool World::Init()
    {
        TimeUtil::TimeAdd(GVAR.GetVar(GVAR_TIMEADD));
        {
            std::string path = cfg.scriptPath + "World/main.lua";
            _worldScript = new Script::WorldScript(path.c_str()); 
        }
        UInt32 now = TimeUtil::Now(), sday = TimeUtil::SharpDay(1) - 10; 

        if(sday < now) sday += 86400;
        AddTimer(86400 * 1000, World_Midnight_Check, this, (sday - now) * 1000);

        return true; 
    }
    void World::UnInit() 
    {

    }
    std::string World::GetLogName()
    {
        return "log/World/"; 
    }

    void World::World_Midnight_Check( World * world )
    {
            UInt32 curtime = TimeUtil::Now();

            //记录此刻活动状态
            world->_worldScript->onActivityCheck(curtime+300);  //延迟300秒计算是否活动时间已过
            
    }
}

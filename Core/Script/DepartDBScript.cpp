#include "Config.h"
#include "DepartDBScript.h"

#include "Server/Cfg.h"

namespace Script
{
    DepartDBScript::DepartDBScript( Cfg * cfg )
    {

        class_add<Cfg>("Cfg");
        class_def<Cfg>("SetSQLConsumetael", &Cfg::SetSQLConsumetael);
        class_def<Cfg>("SetSQLItemCourses", &Cfg::SetSQLItemCourses);
        class_def<Cfg>("SetSQLItemHistories", &Cfg::SetSQLItemHistories);
        class_def<Cfg>("SetSQLMailItemHistories", &Cfg:: SetSQLMailItemHistories);
        set("cfg", cfg);
    }
}

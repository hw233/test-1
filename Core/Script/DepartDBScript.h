#ifndef  _DEPART_DB_SCRIPT_H
#define  _DEPART_DB_SCRIPT_H
#include "Script/Script.h"

class Cfg;

namespace Script
{

    class DepartDBScript:
            public Script
    {
        public:
            DepartDBScript(Cfg * cfg);
    };
}

#endif

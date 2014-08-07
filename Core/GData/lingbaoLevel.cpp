
#include "lingbaoLevel.h"
namespace GData
{
    lingbaoLevel lingbaoLevelData;
    lingbaoL lingbaoLevel::getLingbaoLevel(UInt8 type , UInt8 level)  
    { 
        if(type >= LINBAOTYPE)
            return lingbaoL();
        if(level >= LINGBAOMAX)
            return lingbaoL();
        return _lbl[type][level];
    } 
}

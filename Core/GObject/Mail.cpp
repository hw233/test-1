#include "Mail.h"
namespace GObject
{
    GlobalMails globalMails;
    void Mail::LoadMailInfo(UInt32 id, Player* pl, UInt16 contextId, std::string items, UInt8 option, UInt32 overTime)
    { 
        _id = id;
        _owner = pl;
        _contextId = contextId;
        _items = items;
        _option = option;
        _overTime = overTime;
    } 
}

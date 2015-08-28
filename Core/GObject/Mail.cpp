#include "Mail.h"
#include "Common/StringTokenizer.h"
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
    
    /*
    void Mail::Accept(UInt32 id)
    { 
        std::string items = GetItems();
        StringTokenizer item(items,",");
        if(item.count() % 2)
            return ;
        if(!GetOwner())
            return ;
        for(UInt8 i = 0; i < item.count()/2; ++i)
        { 
            _owner->GetPackage()->AddItem(item[2*i],item[2*i+1]);
        } 
        _option = 1;
        _owner->DeleteMail(_id);
    } 
    */
}

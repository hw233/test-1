#include "ChatHold.h"
#include "Player.h"
namespace GObject
{
    void ChatHold::InsertChat(Player * p, std::string text)
    { 
        UInt8 size = _lst.size();
        if(size >= _countMax)
            _lst.pop_back();
        _lst.push_front(ChatStruct(p,text));
    } 
    void ChatHold::GetStream(Stream& st, UInt8 count)
    { 
        if(count > _countMax)
            count = _countMax;
        std::list<ChatStruct>::iterator it = _lst.begin();
        UInt8 index = 0;
        for(;it != _lst.end(); ++it)
        {
            if(index >= count )
                break;

            Player * pl = it->pl;
            st << _type;
            st << static_cast<UInt16>(pl->getMainFighter()->getId());
            st << static_cast<UInt8>(pl->GetLevel());
            st << static_cast<UInt32>(it->time);
            st << pl->GetName();
            st << it->context;
            index ++;
        }
    } 
}

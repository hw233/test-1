#include "ChatHold.h"
#include "Player.h"
namespace GObject
{
    void ChatHold::InsertChat(Player * p, std::string text)
    { 
        UInt8 size = lst.size();
        if(size >= CountMax)
            lst.pop_back();
        lst.push_front(ChatStruct(p,text));
    } 
    void ChatHold::GetStream(Stream& st, UInt8 count)
    { 
        if(count > CountMax)
            count = CountMax;
        std::list<ChatStruct>::iterator it = lst.begin();
        UInt8 index = 0;
        for(;it != lst.end(); ++it)
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

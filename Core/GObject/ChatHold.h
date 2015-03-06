
#ifndef CHATHOLD_H_
#define CHATHOLD_H_
#include "Config.h"
#include "Common/Stream.h"
namespace GObject
{
    class Player;

    struct ChatStruct
    {
        Player* pl;
        std::string context;
        ChatStruct(Player* p , std::string text):pl(p),context(text){}
    };
    class ChatHold
    {
        public:
            ChatHold(UInt8 max = 0):CountMax(max){}
            void InsertChat(Player * pl, std::string text);
            void GetStream(Stream& st,UInt8 count = 1);
        private:
            std::list<ChatStruct>  lst;
            UInt8 CountMax;
    };
}
#endif // CHATHOLD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


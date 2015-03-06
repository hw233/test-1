
#ifndef CLAN_H_
#define CLAN_H_
#include "Common/Config.h"

#define CLAN_MAX 60
namespace GObject
{
    class Player;
    class ChatHold;
    class Clan
    {
        public:
            void LoadPlayer(Player* pl);
            ChatHold * GetChatHold();
        private:
            std::vector<Player* > _players;
            ChatHold * chatHold;
    };
}
#endif // CLAN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


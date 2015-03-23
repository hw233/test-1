#include "Clan.h"
#include "Player.h"

#define C_CHAT_MAX 10
namespace GObject
{
    void Clan::LoadPlayer(Player* pl)
    { 
        if(_players.size() >= CLAN_MAX)
            return ;
        _players.push_back(pl);
        pl->SetClan(this);
    } 

    ChatHold * Clan::GetChatHold()
    { 
        if(!chatHold)
            chatHold = new ChatHold(C_CHAT_MAX);
        return chatHold;
    } 
}

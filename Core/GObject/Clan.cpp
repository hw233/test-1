#include "Clan.h"
#include "Player.h"

#define C_CHAT_MAX 10
namespace GObject
{
    Clan::Clan(Player* creater, std::string name):_creater(creater),_name(name) { } 
    
    void LoadClanInfo(Player* leader, std::string announcement, UInt8 personMax)
    { 
        _leader = leader;
        _announcement = announcement;
        _personMax = personMax;
    } 

    UInt8 Clan::Apply(Player* pl)
    { 
        if(_applicant.size() >= APPLICANTMAX)
            return 1;
        if(pl->GetClan())
            return 2;

        _applicant.push_back(pl);
    } 

    UInt8 Clan::Allow(Player* pl)
    { 
        if(!pl)
            return 1;
        if(pl->GetClan())
            return 2;

    } 



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

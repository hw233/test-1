#include "Clan.h"
#include "Player.h"

#define C_CHAT_MAX 30
namespace GObject
{
    Clan::Clan(UInt32 id, std::string name, Player* creater):_id(id),_name(name),_creater(creater) { } 
    
    void Clan::LoadClanInfo(Player* leader, std::string announcement, UInt8 personMax)
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
        return 0;
    } 

    UInt8 Clan::Allow(Player* pl,UInt8 type)
    { 
        if(!pl)
            return 1;
        if(pl->GetClan())
            return 2;
        if(_players.size() >= _personMax)
            return 3;

        for(std::vector<Player* >::iterator it = _applicant.begin(); it != _applicant.end();++it)
        { 
            if(*it == pl)
            { 
                if(!type)
                {
                    UInt32 now = TimeUtil::Now();
                    _players.push_back(pl);
                    pl->SetClan(this);
                    pl->SetClanPos(1);
                    DB1().PushUpdateData("REPLACE INTO  `clan_player`(`clanId`, `playerId`,`position`,`contribute`,`enterTime`) VALUES(%u, %" I64_FMT "u ,%u , 0, %u",_id, pl->getId(),pl->GetClanPos(),now );   //LIBOUInt64
                }
                _applicant.erase(it);
                break;
            } 
        } 
        return 0;
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

    std::vector<Player*>::iterator Clan::HasMember(Player* pl)
    { 
        std::vector<Player* >::iterator it = _players.begin();
        for(; it != _players.end(); ++it)
        { 
            if(*it == pl)
                return it;
        } 
        return _players.end();
    } 

    UInt8 Clan::DelMember(Player* pl)
    { 
        std::vector<Player*>::iterator it = HasMember(pl);
        if(it != _players.end())
        {
            _players.erase(it);
            DB1().PushUpdateData(" DELETE FROM `clan_player` WHERE `clanId` = %u, `playerId` = %" I64_FMT "u ",_id, pl->getId());   //LIBOUInt64
            return 0;
        }
        return 1;
    } 

    UInt8 Clan::ChangePosition(Player* opter, Player* pl, UInt8 pos)
    { 
        if(opter->GetClanPos() <= pl->GetClanPos())
            return 1;
        return 0;
    } 

    void Clan::Boradcast(Stream st, UInt8 index)
    { 
        for(UInt8 i = 0; i < _players.size(); ++i)
        {
            switch(index)
            { 
                case 0:
                    {
                        if(!_players[i]->GetRecChat(e_chat_clan))
                        return ;
                    }
                    break;
            } 
            _players[index]->send(st);
        }
    } 
    void Clan::GetClanInfo(Stream& st)
    { 
        st << GetName();
        st << GetLevel();
        st << static_cast<UInt8>(_players.size());
        st << static_cast<UInt8>(_personMax);
        st << _announcement;
    } 
}

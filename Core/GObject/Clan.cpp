#include "Clan.h"
#include "Player.h"
#include "MsgID.h"

#define C_CHAT_MAX 30

namespace GObject
{
    Clan::Clan(UInt32 id, std::string name, Player* creater):_id(id),_name(name),_picIndex(0),_creater(creater) { 
        _chatHold = NULL; 
    } 
    
    void Clan::LoadClanInfo(Player* leader, std::string announcement,std::string announcement2, UInt8 personMax)
    { 
        _leader = leader;
        _announcement = announcement;
        _announcement2 = announcement2;
        _personMax = personMax;
        _players.clear();
        _applicant.clear();
    } 

    UInt8 Clan::Apply(Player* pl, UInt8 update)
    { 
        if(_applicant.size() >= APPLICANTMAX)
            return 1;
        if(pl->GetClan())
            return 2;

        if(_id < 10)
        { 
            UInt32 now = TimeUtil::Now();
            _players.push_back(pl);
            pl->SetClan(this);
            pl->SetClanPos(1);  //普通成员
            DB1().PushUpdateData("REPLACE INTO  `clan_player`(`clanId`, `playerId`,`position`,`contribute`,`enterTime`) VALUES(%u, %" I64_FMT "u ,%u , 0, %u",_id, pl->getId(),pl->GetClanPos(),now );   //LIBOUInt64
            //DB1().PushUpdateData("DELETE FROM player_apply_clan where `playerId` = %" I64_FMT "u",pl->getId() );   //LIBOUInt64
            return 0;
        } 
        if(!CheckApplicant(pl))
            return 1;

        _applicant.push_back(pl);

        Stream st(REP::CLAN_APPLY);
        st << pl->GetName();
        st << pl->GetVar(VAR_BATTLE_POINT);
        st << static_cast<UInt8>(pl->GetLevel());
        st << Stream::eos;

        SendStreamOnPos(e_clan_header,st);
        SendStreamOnPos(e_clan_header2,st);

        if(!update)
            DB1().PushUpdateData("REPLACE INTO  `player_apply_clan`(`clanId`, `playerId`,`time`) VALUES(%u, %" I64_FMT "u , %u)",_id, pl->getId(),TimeUtil::Now() );   //LIBOUInt64
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
                _applicant.erase(it);
                if(!type)
                {
                    UInt32 now = TimeUtil::Now();
                    _players.push_back(pl);
                    pl->SetClan(this);
                    pl->SetClanPos(1);
                    DB1().PushUpdateData("REPLACE INTO  `clan_player`(`clanId`, `playerId`,`position`,`contribute`,`enterTime`) VALUES(%u, %" I64_FMT "u ,%u , 0, %u)",_id, pl->getId(),pl->GetClanPos(),now );   //LIBOUInt64
                    DB1().PushUpdateData("DELETE FROM player_apply_clan where `playerId` = %" I64_FMT "u",pl->getId() );   //LIBOUInt64
                    Stream st(REP::CLAN_INFO);
                    st << IsTheFounder(pl);
                    GetClanInfo2(st);
                    st << Stream::eos;
                    pl->send(st);
                }
                break;
            } 
        } 
        return 0;
    } 

    void Clan::LoadPlayer(Player* pl,UInt8 flag)
    { 
        if(_players.size() >= CLAN_MAX)
            return ;
        _players.push_back(pl);
        pl->SetClan(this);
        if(flag)
        {
            UInt32 now = TimeUtil::Now();
            DB1().PushUpdateData("REPLACE INTO  `clan_player`(`clanId`, `playerId`,`position`,`contribute`,`enterTime`) VALUES(%u, %" I64_FMT "u ,%u , 0, %u)",_id, pl->getId(),pl->GetClanPos(),now );   //LIBOUInt64
        }
    } 

    ChatHold * Clan::GetChatHold()
    { 
        if(!_chatHold)
            _chatHold = new ChatHold(2,C_CHAT_MAX);
        return _chatHold;
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
        static UInt8 posLimit[] = {1,5,10,200,200};
        if(pos > 5 || !pos )
            return -1;
        if(HasMember(pl) == _players.end())
            return 2;
        if(opter->GetClanPos() <= pl->GetClanPos() || opter->GetClanPos() <= pos)
            return 1;
        if(GetPosCount(pos) < posLimit[pos - 1])
            return 3;

        pl->SetClanPos(pos);
        return 0;
    } 

    void Clan::Boradcast(Stream st, UInt8 index)
    { 
        for(UInt8 i = 0; i < _players.size(); ++i)
        {
            _players[i]->send(st);
            continue;
            switch(index)
            { 
                case 0:
                    {
                        if(_players[i]->GetRecChat(e_chat_clan))
                            _players[i]->send(st);
                    }
                    break;
            } 
        }
    } 
    void Clan::GetClanInfo(Stream& st)
    { 
        st << static_cast<UInt32>(_id);
        st << GetName();
        st << GetLevel();
        st << static_cast<UInt8>(_players.size());
        st << static_cast<UInt8>(_personMax);
        st << _announcement;
    } 
    void Clan::GetClanInfo2(Stream& st)
    { 
        st << static_cast<UInt32>(_id);
        st << GetName();
        st << static_cast<UInt32>(GetLevel());
        st << _announcement;
        st << static_cast<UInt8>(_players.size());
        for(UInt8 i = 0; i < _players.size(); ++i)
        { 
            Player* pl = _players[i];
            if(!pl)
                continue;
            st << pl->GetName();
            st << pl->GetClanPos();
            st << static_cast<UInt8>(pl->isOnline());
            st << static_cast<UInt8>(pl->GetLevel());
        } 
        st << _announcement2;
        st << static_cast<UInt8>(_applicant.size());
        for(UInt8 i = 0; i < _applicant.size(); ++i)
        { 
            Player* pl = _applicant[i];
            if(!pl)
                continue;
            st << pl->GetName();
            st << static_cast<UInt32>(pl->GetVar(VAR_BATTLE_POINT));
            st << static_cast<UInt8>(pl->GetLevel());
        } 
        st << static_cast<UInt32>(0);
        st << static_cast<UInt32>(0);
    } 

    UInt8 Clan::GetPosCount(UInt8 pos)
    { 
        UInt8 count = 0 ;
        for(UInt8 i = 0; i < _players.size(); ++i)
        { 
            if(_players[i]->GetClanPos() == pos)
                ++ count ;
        } 
        return count;
    } 
    Clan* Clan::GetBossClan()
    { 
        for(UInt32 i = 0; i < 100; ++i)
        {
            Clan * cl = globalClan[i];
            if(!cl)
            {
                char str[3];
                sprintf(str,"朝廷军团%d",i);
                cl = new Clan(i,str,NULL);
                cl->LoadClanInfo(NULL,"","",50);
                cl->SetPicIndex(0);
                cl->SetLevel(1);
                globalClan.add(cl->GetId(),cl);
                globalNamedClans.add(cl->GetName(), cl);
                DB2().PushUpdateData("INSERT INTO `clan` VALUES( %u,'%s',%u,'%s','%s',%" I64_FMT "u,%" I64_FMT "u,%u,0,%u)",cl->GetId(),cl->GetName().c_str(),0,cl->GetAnnouncement().c_str(), cl->GetAnnouncement2().c_str(), 0,0,1,0,cl->GetPersonMax());
                return cl;
            }
            else if(cl->GetClanNumberCount() < cl->GetPersonMax())
            {
                return cl;
            }
        }
        return NULL;
    } 

    void Clan::SendStreamOnPos(UInt8 pos, Stream st)
    { 
        for(UInt8 i = 0; i < _players.size(); ++i)
        { 
            if(_players[i]->GetClanPos() == pos)
                _players[i]->send(st);
        } 
    } 
    bool Clan::CheckApplicant(Player* pl)
    { 
        for(UInt8 i = 0; i < _applicant.size(); ++i)
        { 
            if(_applicant[i] == pl)
                return false;
        } 
        return true;
    } 
}

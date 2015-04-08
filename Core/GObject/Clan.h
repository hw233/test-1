
#ifndef CLAN_H_
#define CLAN_H_
#include "Config.h"
#include "iostream"
using namespace std;
#include "Player.h"
#include "ChatHold.h"
#include "Common/Stream.h"

#define CLAN_MAX 60
#define APPLICANTMAX 60

namespace GObject
{
    class Player;
    class ChatHold;
    class Clan
    {
        public:
            Clan(UInt32 id, std::string name, Player* creater);//:_name(name),_creater(creater),_personMax(30){}
            void LoadPlayer(Player* pl);
            void LoadClanInfo(Player* leader, std::string announcement, UInt8 personMax );
            ChatHold * GetChatHold();

            std::string GetName(){ return _name;}
            std::string GetAnnouncement() { return _announcement;}
            Player* GetCreater(){return _creater;}
//          void SetLeader(Player* leader){ _leader = leader;}
            Player* GetLeader(){ return _leader;}

            UInt8 GetPersonMax(){ return _personMax;}

            UInt8 Apply(Player* pl);
            UInt8 Allow(Player* pl, UInt8 type = 0);
            UInt8 DelMember(Player* pl);
            std::vector<Player*>::iterator HasMember(Player* pl);
            UInt8 ChangePosition(Player* opter, Player* pl, UInt8 pos);

            void Boradcast(Stream st, UInt8 index);
            void SetLevel(UInt8 lev){ _level = lev;}
            UInt8 GetLevel(){return _level;}
            void GetClanInfo(Stream& st);
        private:
            std::vector<Player* > _players;
            std::vector<Player* > _applicant;
            ChatHold * chatHold;

            UInt32 _id;
            std::string _name;
            std::string _announcement;

            UInt8 _level;

            Player* _creater;
            Player* _leader;

            UInt8 _personMax;
    };
}
#endif // CLAN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


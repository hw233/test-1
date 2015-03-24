
#ifndef CLAN_H_
#define CLAN_H_
#include "Config.h"
#include "iostream"
using namespace std;
#include "Player.h"
#include "ChatHold.h"

#define CLAN_MAX 60

namespace GObject
{
    class Player;
    class ChatHold;
    class Clan
    {
        public:
            //Clan(Player* creater, std::string name):_name(name),_creater(creater),_personMax(30){}
            void LoadPlayer(Player* pl);
            ChatHold * GetChatHold();

            std::string GetName(){ return _name;}

        private:
            std::vector<GObject::Player* > _players;
            ChatHold * chatHold;

            std::string _name;
            std::string _announcement;

            Player* _creater;
            Player* _leader;

            UInt8 _personMax;
    };
}
#endif // CLAN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


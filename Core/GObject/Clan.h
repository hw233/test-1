
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
            void LoadPlayer(Player* pl,UInt8 flag = 0);
            void LoadClanInfo(Player* leader, std::string announcement, std::string announcement2, UInt8 personMax );
            ChatHold * GetChatHold();

            std::string GetName(){ return _name;}
            std::string GetAnnouncement() { return _announcement;}
            std::string GetAnnouncement2() { return _announcement2;}
            Player* GetCreater(){return _creater;}
//          void SetLeader(Player* leader){ _leader = leader;}
            Player* GetLeader(){ return _leader;}

            UInt8 GetPersonMax(){ return _personMax;}

            UInt8 Apply(Player* pl,UInt8 update = 0);
            UInt8 Allow(Player* pl, UInt8 type = 0);
            UInt8 DelMember(Player* pl);
            std::vector<Player*>::iterator HasMember(Player* pl);
            UInt8 ChangePosition(Player* opter, Player* pl, UInt8 pos);

            void Boradcast(Stream st, UInt8 index);
            void SetLevel(UInt8 lev){ _level = lev;}
            UInt8 GetLevel(){return _level;}
            void GetClanInfo(Stream& st);
            void GetClanInfo2(Stream& st);
            UInt32 GetId(){ return _id;}
            void SetPicIndex(UInt8 picIndex){_picIndex = picIndex;}
            UInt8 GetPicIndex(){return _picIndex;}
            UInt8 GetPosCount(UInt8 pos);
            UInt8 IsTheFounder(Player * pl){ return !(pl == _creater);}

            void SetAnnouncement(std::string announcement){ _announcement = announcement;}
            void SetAnnouncement2(std::string announcement){ _announcement2 = announcement;}

            UInt8 GetClanBattleRoomId() { return _clanBattleRoomId; }
            void  SetClanBattleRoomId(UInt32 roomId) { _clanBattleRoomId = roomId; }
            UInt8 GetMemberNum() const { return _players.size();}
            UInt32 GetClanFame() const { return _clanFame;}   //公会声望
            void SetClanFame(UInt32 fame) { _clanFame = fame;}
            void SetConquests(UInt32 conquest) { _conquests = conquest;}
            UInt32 GetConquests() const { return _conquests;} //公会战绩
            UInt8  GetBattleForceId() const { return _forceId;}
            void SetBattleForceId(UInt8 forceId) { _forceId = forceId;}

        private:
            std::vector<Player* > _players;
            std::vector<Player* > _applicant;
            ChatHold * chatHold;

            UInt32 _id;
            std::string _name;
            UInt8 _picIndex;
            std::string _announcement;
            std::string _announcement2;

            UInt8 _level;

            Player* _creater;
            Player* _leader;

            UInt8 _personMax;
            UInt8 _clanBattleRoomId;
            UInt32 _clanFame;
            UInt32 _conquests;
            UInt8  _forceId;
    };
}
#endif // CLAN_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


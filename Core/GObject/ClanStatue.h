#ifndef _CLAN_STATUE_H_
#define _CLAN_STATUE_H_

namespace GObject
{

class Clan;
class Player;

class ClanStatue
{
    public:
        ClanStatue(Clan *c);
        ~ClanStatue();

    public:
        void addClanStatueFromDB();

    private:
        UInt8 _level;
        Clan * _clan;
};

}
#endif


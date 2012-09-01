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
        void updateLevel(UInt32 exp);
        UInt16 getLevel();
        UInt32 getExp();

    private:
        UInt16 _level;
        UInt32 _exp;
        Clan * _clan;
};

}
#endif


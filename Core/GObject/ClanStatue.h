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
        void updateLevel(UInt32 exp, UInt32 expUpdateTime);
        void addExp(UInt32 exp);
        void subExp(UInt32 exp);
        UInt16 getLevel();
        UInt32 getExp();
        UInt32 getShownExp();
        UInt32 getShownNextExp();

    private:
        UInt16 _level;
        UInt32 _exp;
        Clan * _clan;
};

}
#endif


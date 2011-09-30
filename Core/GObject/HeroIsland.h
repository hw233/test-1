
#ifndef HEROISLAND_H_
#define HEROISLAND_H_

#include "Config.h"
#include "Player.h"
#include "Script/GameActionLua.h"

namespace GObject
{

#define HERO_ISLAND_SPOTS 5

struct HIPlayerData
{
    Player* player;
    UInt32 straight;
};

struct RareAnimals
{
    UInt32 id;
    GData::AttrExtra attr;
    UInt32 last;
    UInt32 cd;

    UInt32 cdend;
};

class HeroIsland
{
public:
    HeroIsland() : _running(false)
    {
        _types[0] = _types[1] = _types[2] = 0;
    }

    ~HeroIsland() {}

public:
    static void setRareAnimals(UInt8 spot, UInt32 npcid, Table attr, UInt32 last, UInt32 cd);
    static bool isRareAnimal(UInt32 npcid);
private:
    static std::vector<RareAnimals> _animals[HERO_ISLAND_SPOTS];

public:
    void process(UInt32 now);

    void getIdentity(Player* player);
    void enter(Player* player);
    void leave(Player* player);
    void listPlayers(Player* player);
    void moveTo(Player* player, UInt8 spot);
    void attack(Player* player, UInt8 spot, UInt8 type, UInt64 id);

    HIPlayerData* findPlayer(Player* player, UInt8 spot = 0);
    void sendPlayers(Player* player, UInt8 spot = 0);

    inline void setRunning(bool r) { _running = r; }
    inline bool isRunning() { return _running; }

public:
    std::vector<HIPlayerData*> _players[HERO_ISLAND_SPOTS];
    UInt32 _types[3];

private:
    bool _running;
};

extern HeroIsland heroIsland;

}

#endif // HEROISLAND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


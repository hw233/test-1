
#ifndef WORLDBOSS_H_
#define WORLDBOSS_H_

#include "Config.h"

namespace GObject
{

class Player;

struct WBoss
{
    UInt32 npcId;
    UInt8 level;
    UInt8 count;
};

class WorldBoss
{
public:
    static bool isWorldBoss(UInt32 npcid);

public:
    WorldBoss() : m_max(0) {}
    ~WorldBoss() {}

    void refresh(UInt32);
    void prepare(UInt32 now);
    void attack(Player* pl, UInt16 loc, UInt32 npcid);
    void reset();
    const WBoss& get(UInt16 loc) { return m_boss[loc]; }
    void add(UInt16 loc, UInt32 npcId, UInt8 level, UInt8 count);
    void setLevel(UInt8 lvl) { m_max = lvl; }
private:
    std::map<UInt16, WBoss> m_boss;
    UInt8 m_max;
};

extern WorldBoss worldBoss;

}

#endif // WORLDBOSS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


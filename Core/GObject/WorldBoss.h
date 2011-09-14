
#ifndef WORLDBOSS_H_
#define WORLDBOSS_H_

#include "Config.h"
#include "Common/Mutex.h"

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
    WorldBoss() : m_level(0), m_max(0) {}
    ~WorldBoss() {}

    void refresh(UInt32);
    void prepare(UInt32 now);
    void attack(Player* pl, UInt16 loc, UInt32 npcid);
    void reset();
    void add(UInt16 loc, UInt32 npcId, UInt8 level, UInt8 count, bool = false, bool = true);

    inline const WBoss& get(UInt16 loc) { return m_boss[loc]; }
    inline void setLevel(UInt8 lvl)
    {
        if (lvl > m_max)
            m_max = lvl;
    }

private:
    UInt8 getLevel(UInt32 now);

private:
    std::map<UInt16, WBoss> m_boss;
    FastMutex m_lck;
    UInt8 m_level;
    UInt8 m_max;
};

extern WorldBoss worldBoss;

}

#endif // WORLDBOSS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


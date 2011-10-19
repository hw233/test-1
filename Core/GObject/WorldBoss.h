
#ifndef WORLDBOSS_H_
#define WORLDBOSS_H_

#include "Config.h"
#include "Common/Mutex.h"
#include "Player.h"

namespace GObject
{

class Player;

struct WBoss
{
    UInt32 npcId;
    UInt8 level;
    UInt8 count;
};

struct lt
{
    bool operator()(Player* p1, Player* p2) const
    {
        return p1->getWorldBossHp() >= p2->getWorldBossHp(); // XXX: 倒
    }
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
    bool attackWorldBoss(Player* pl, UInt32 npcId, UInt8 expfactor, bool final = false);
    void reset();
    void add(UInt16 loc, UInt32 npcId, UInt8 level, UInt8 count, bool = false, bool = true);

    inline const WBoss& get(UInt16 loc) { return m_boss[loc]; }
    inline void setLevel(UInt8 lvl)
    {
        if (lvl > m_max)
            m_max = lvl;
    }

    void setBossLevel(UInt8 lvl);

private:
    UInt8 getLevel(UInt32 now);

private:
    std::map<UInt16, WBoss> m_boss;
    FastMutex m_lck;
    UInt8 m_level;
    UInt8 m_max;
    typedef std::set<Player*, lt> AttackInfo;
    AttackInfo atkinfo;
};

extern WorldBoss worldBoss;

}

#endif // WORLDBOSS_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


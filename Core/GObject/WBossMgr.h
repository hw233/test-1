
#ifndef WBOSSMGR_H_
#define WBOSSMGR_H_

#include "Config.h"

namespace GObject
{

class Player;

class WBoss
{
public:
    WBoss();
    ~WBoss();

private:
    UInt8 m_count;
    UInt8 m_maxcnt;
};

class WBossMgr
{
public:
    static bool isWorldBoss(UInt32 npcid);

public:
    WBossMgr() : _prepareTime(0), _prepareStep(0), _appearTime(0), _disapperTime(0), m_level(0), m_maxlvl(0) {}
    ~WBossMgr() {}

    void process(UInt32 now);
    void appear(UInt8 level, UInt32 now);
    void disapper(UInt8 level, UInt32 now);
    void attack(Player* pl, UInt16 loc, UInt32 npcid);
    bool attackWorldBoss(Player* pl, UInt32 npcId, UInt8 expfactor, bool final = false);
    void broadcastTV(UInt32 now);
    void calcNext(UInt32 now);

    inline void setLevel(UInt8 lvl)
    {   
        if (lvl > m_maxlvl)
            m_maxlvl = lvl;
    }
    void setBossLevel(UInt8 lvl);

private:
    UInt32 _prepareTime;
    UInt8 _prepareStep;
    UInt32 _appearTime;
    UInt32 _disapperTime;
    UInt8 m_level;
    UInt8 m_maxlvl;
    std::vector<WBoss> m_bosses;
};

extern WBossMgr worldBoss;

} // namespace GObject

#endif // WBOSSMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


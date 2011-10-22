
#ifndef WBOSSMGR_H_
#define WBOSSMGR_H_

#include "Config.h"

namespace GObject
{

class Player;

struct AttackInfo
{
};

class WBoss
{
public:
    WBoss(UInt8 cnt, UInt8 max, UInt16 loc, UInt8 lvl)
        : m_count(cnt), m_maxcnt(max), m_loc(loc), m_lvl(lvl), m_final(false) {}
    ~WBoss() {}

    inline void setFinal(bool f) { m_final = f; }
    inline bool isFinal() const { return m_final; }

    bool attack(Player* pl, UInt32 id);
    void appear(UInt32 npcid, UInt32 oldid);
    void disapper(UInt32 npcid);

private:
    UInt8 m_count;
    UInt8 m_maxcnt;
    UInt16 m_loc;
    UInt8 m_lvl;
    bool m_final;
};

class WBossMgr
{
public:
    static bool isWorldBoss(UInt32 npcid);

public:
    WBossMgr()
        : _prepareTime(0), _prepareStep(0), _appearTime(0),
        _disapperTime(0), m_level(0), m_maxlvl(0), m_boss(NULL) {}
    ~WBossMgr() {}

    void initWBoss();
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
    WBoss* m_boss;
};

extern WBossMgr worldBoss;

} // namespace GObject

#endif // WBOSSMGR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


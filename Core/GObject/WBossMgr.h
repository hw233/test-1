
#ifndef WBOSSMGR_H_
#define WBOSSMGR_H_

#include "Config.h"

namespace GObject
{

class Player;

struct AttackInfo
{
    UInt64 playerId;
    UInt32 score;
};

struct lt
{
    bool operator()(AttackInfo a, AttackInfo b) const
    {
        return a.score < b.score;
    }
};

class WBoss
{
public:
    WBoss(UInt32 id, UInt8 cnt, UInt8 max, UInt16 loc, UInt8 lvl)
        : m_id(id), m_count(cnt), m_maxcnt(max), m_loc(loc),
        m_lvl(lvl), m_disappered(false), m_final(false) {}
    ~WBoss() {}

    inline void setFinal(bool f) { m_final = f; }
    inline bool isFinal() const { return m_final; }
    inline bool isDisappered() const { return m_disappered; }

    inline void setId(UInt32 id) { m_id = id; }
    inline UInt32 getId() const { return m_id; }
    bool attack(Player* pl, UInt16 loc, UInt32 id);
    void appear(UInt32 npcid, UInt32 oldid = 0);
    void disapper();
    bool attackWorldBoss(Player* pl, UInt32 npcId, UInt8 expfactor, bool final = false);

    inline void setLevel(UInt8 lvl) { m_lvl = lvl; }
    inline UInt8 getLevel() const { return m_lvl; }
    inline void setLoc(UInt16 loc) { m_loc = loc; }
    inline UInt16 getLoc() const { return m_loc; }

private:
    UInt32 m_id;
    UInt8 m_count;
    UInt8 m_maxcnt;
    UInt16 m_loc;
    UInt8 m_lvl;
    bool m_disappered;

    UInt8 _hppercent;
    std::vector<UInt32> _hp;
    bool m_final;
    std::multiset<AttackInfo, lt> m_atkinfo;
};

class WBossMgr
{
public:
    static bool isWorldBoss(UInt32 npcid);

public:
    WBossMgr()
        : _prepareTime(0), _prepareStep(0), _appearTime(0),
        _disapperTime(0), m_level(0), m_maxlvl(0), m_boss(NULL) {}
    ~WBossMgr()
    {
        if (m_boss)
        {
            delete m_boss;
            m_boss = NULL;
        }
    }

    void initWBoss();
    void process(UInt32 now);
    void appear(UInt8 level, UInt32 now);
    void disapper(UInt32 now);
    void attack(Player* pl, UInt16 loc, UInt32 npcid);
    void broadcastTV(UInt32 now);
    void calcNext(UInt32 now);
    void nextDay(UInt32 now);

    inline void setLevel(UInt8 lvl) {   if (lvl > m_maxlvl) m_maxlvl = lvl; }
    inline void setBossLevel(UInt8 lvl) { m_level = lvl; }

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

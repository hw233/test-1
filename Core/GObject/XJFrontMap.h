
#ifndef XJFRONTMAP_H_
#define XJFRONTMAP_H_

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/Mutex.h"
#include <map>
#include <vector>

namespace GObject
{

struct XJFrontMapData
{
    XJFrontMapData() : count(0), status(0), lootlvl(0) {}
    void reset() { count = 0; status = 0; lootlvl = 0; }
    UInt8 count;
    UInt8 status;
    UInt8 lootlvl;
};

class Player;

class XJFrontMap : public Singleton<XJFrontMap>
{
public:
    static const UInt8 FREECNT = 2;
    static const UInt8 GOLDCNT = 3;

    static UInt8 _activeCount;
    static void setFrontMapActiveCount(UInt8);

    static UInt8 getFreeCount();
    static UInt8 getGoldCount(UInt8 vipl = 0);
    static UInt32 getEnterGold(Player*);

public:
    XJFrontMap() {}
    ~XJFrontMap() {}

    void sendInfo(Player* pl, UInt8 id, bool = false, bool = false);
    void sendInfo2(Player* pl, UInt8 id, bool = false);
    UInt8 getCount(Player* pl);

    void enter(Player* pl, UInt8 id);
    UInt8 fight(Player* pl, UInt8 id, UInt8 spot, bool ato = false, bool complete = false);
    void reset(Player* pl, UInt8 id);

    UInt8 getFrontMapSize(Player* pl);
    void buildInfo(Player* pl, Stream& st);

    bool checkLevel(Player* pl, UInt8 id);
    void autoBattle(Player* pl, UInt8 id, UInt8 type, UInt8 mtype = 0, bool = false);
    void sendAutoFrontMap(Player* pl);

    void addPlayer(UInt64 playerId, UInt8 id, UInt8 spot, UInt8 count, UInt8 status, UInt8 lootlvl);

private:
    void sendFrontMap(Stream& st, Player* pl, UInt8 id, bool force = false);

private:
    std::map<UInt64, std::map<UInt8, std::vector<XJFrontMapData> > > m_frts;
    FastMutex _mutex;
};

#define xjfrontMap XJFrontMap::Instance()

} // namespace GObject

#endif // XJFRONTMAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

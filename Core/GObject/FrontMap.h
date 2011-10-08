
#ifndef FRONTMAP_H_
#define FRONTMAP_H_

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/Mutex.h"
#include <map>
#include <vector>

namespace GObject
{

struct FrontMapData
{
    FrontMapData() : count(0), status(0), lootlvl(0) {}
    void reset() { count = 0; status = 0; lootlvl = 0; }
    UInt8 count;
    UInt8 status;
    UInt8 lootlvl;
};

class Player;

class FrontMap : public Singleton<FrontMap>
{
public:
    static const UInt8 FREECNT = 2;
    static const UInt8 GOLDCNT = 2;

    static UInt8 _activeCount;
    static void setFrontMapActiveCount(UInt8);

    static UInt8 getFreeCount();
    static UInt8 getGoldCount(UInt8 vipl);

public:
    void sendAllInfo(Player* pl);
    void sendInfo(Player* pl, UInt8 id, bool = false, bool = false);
    void sendInfo2(Player* pl, UInt8 id, bool = false);
    UInt8 getCount(Player* pl);

    void enter(Player* pl, UInt8 id);
    void fight(Player* pl, UInt8 id, UInt8 spot);
    void reset(Player* pl, UInt8 id);

    UInt8 getFrontMapSize(Player* pl);
    void buildInfo(Player* pl, Stream& st);

    void addPlayer(UInt64 playerId, UInt8 id, UInt8 spot, UInt8 count, UInt8 status, UInt8 lootlvl);
private:
    void sendFrontMap(Stream& st, Player* pl, UInt8 id, bool force = false);

private:
    std::map<UInt64, std::map<UInt8, std::vector<FrontMapData> > > m_frts;
    FastMutex _mutex;
};

#define frontMap FrontMap::Instance()

} // namespace GObject

#endif // FRONTMAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


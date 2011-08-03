
#ifndef FRONTMAP_H_
#define FRONTMAP_H_

#include "Config.h"
#include "Common/Singleton.h"
#include <map>
#include <vector>

namespace GObject
{

struct FrontMapData
{
    FrontMapData() : count(0), status(0) {}
    void reset() { memset(this, 0x00, sizeof(*this)); }
    UInt8 count;
    UInt8 status;
};

class Player;

class FrontMap : public Singleton<FrontMap>
{
public:
    void sendAllInfo(Player* pl);
    void sendInfo(Player* pl, UInt8 id);

    void enter(Player* pl, UInt8 id);
    void fight(Player* pl, UInt8 id, UInt8 spot);
    void reset(Player* pl, UInt8 id);

    void addPlayer(UInt64 playerId, UInt8 id, UInt8 spot, UInt8 count, UInt8 status);
    std::vector<FrontMapData>& getFrontMapData(Player* pl, UInt8 id, bool update = false);
    std::vector<FrontMapData>& getFrontMapData(Player* pl, UInt64 playerId, UInt8 id, bool update = false);

private:
    std::map<UInt64, std::map<UInt8, std::vector<FrontMapData> > > m_frts;
};

#define frontMap FrontMap::Instance()

} // namespace GObject

#endif // FRONTMAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


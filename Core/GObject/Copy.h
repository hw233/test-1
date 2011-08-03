
#ifndef COPY_H_
#define COPY_H_

#include "Config.h"
#include "Common/Singleton.h"
#include <map>

namespace GObject
{

struct CopyData
{
    CopyData() : floor(0), spot(0), freeCount(0), goldCount(0) {}
    void reset() { memset(this, 0x00, sizeof(*this)); }

    UInt8 floor;
    UInt8 spot;
    UInt8 freeCount;
    UInt8 goldCount;
    UInt32 updatetime;
};

class Player;

class PlayerCopy : public Singleton<PlayerCopy>
{
public:

    void sendAllInfo(Player* pl);
    void sendInfo(Player* pl, UInt8 id);
    void enter(Player* pl, UInt8 id);
    void fight(Player* pl, UInt8 id);
    void reset(Player* pl, UInt8 id);

    void addPlayer(UInt64 playerId, UInt8 id, UInt8 floor, UInt8 spot);
    CopyData& getCopyData(Player* pl, UInt8 id, bool update = false);
    CopyData& getCopyData(Player* pl, UInt64 playerId, UInt8 id, bool update = false);

private:
    std::map<UInt64, std::map<UInt8, CopyData> > m_copys;
};

#define playerCopy PlayerCopy::Instance()

} // namespace GObject

#endif // COPY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


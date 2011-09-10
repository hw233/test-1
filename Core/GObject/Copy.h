
#ifndef COPY_H_
#define COPY_H_

#include "Config.h"
#include "Common/Singleton.h"
#include "Common/Mutex.h"
#include <map>

class Stream;

namespace GObject
{

struct CopyData
{
    CopyData() : floor(0), spot(0) {}
    void reset() { memset(this, 0x00, sizeof(*this)); }

    UInt8 floor;
    UInt8 spot;
};

class Player;

class PlayerCopy : public Singleton<PlayerCopy>
{
public:
    static const UInt8 GOLDCNT = 3;
    static const UInt8 FREECNT = 2;

    static UInt8 getGoldCount(UInt8 vipl);

public:
    void sendAllInfo(Player* pl);
    void sendInfo(Player* pl, UInt8 id);
    void enter(Player* pl, UInt8 id);
    UInt8 fight(Player* pl, UInt8 id, bool = false);
    void reset(Player* pl, UInt8 id);

    void getCount(Player* pl, UInt8* free, UInt8* gold, bool = false);

    UInt8 getCopySize(Player* pl);
    void buildInfo(Player* pl, Stream& st);

    void autoBattle(Player* pl, UInt8 id, UInt8 type, bool = false);
    void failed(Player* pl, UInt8 id);

    void addPlayer(UInt64 playerId, UInt8 id, UInt8 floor, UInt8 spot);
    CopyData& getCopyData(Player* pl, UInt8 id, bool update = false);
    CopyData& getCopyData(Player* pl, UInt64 playerId, UInt8 id, bool update = false);
    UInt8 getCopyFloors(UInt8 id);
    UInt8 checkCopy(Player* pl, UInt8 id);
    
private:
    std::map<UInt64, std::map<UInt8, CopyData> > m_copys;
	FastMutex _mutex;
};

#define playerCopy PlayerCopy::Instance()

} // namespace GObject

#endif // COPY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


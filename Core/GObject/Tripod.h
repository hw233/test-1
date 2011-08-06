
#ifndef TRIPOD_H_
#define TRIPOD_H_

#include "Common/Singleton.h"
#include "Common/Stream.h"
#include "Common/Mutex.h"

namespace GObject
{

#define MAX_TRIPOD_SOUL 100000
#define POINT_PERMIN (60*10)

struct TripodData
{
    TripodData()
    {
        memset(this, 0x00, sizeof(*this));
        quality = 2;
        needgen = 1;
    }

    UInt32 soul;    // 元气值
    UInt8 fire;     // 火种: 0-普通的火 1-貅目鑫火 2-极地冽火 3-盘木玄火 4-炼狱冥火 5-三昧真火 6-九天离火 
    UInt8 quality;  // 奖励品质 1-白 2-绿 3-蓝 4-紫 5-橙
    UInt8 awdst;    // 奖励状态 0-熔炼中 1-未领取
    UInt8 needgen;  // 需要重新生成奖励
};

class Player;

class Tripod : public Singleton<Tripod>
{
public:
    Tripod() {}
    ~Tripod() { m_tds.clear(); }

    void getTripodInfo(Player* pl);
    void addItem(Player* pl, UInt32 itemid, int num, UInt8 bind);
    void makeFire(Player* pl, UInt32 id1, UInt32 id2);
    void getAward(Player* pl);
    void genAward(Player* pl, TripodData& td, Stream& st);
    bool genAward(Player* pl, TripodData& td, UInt32& id, UInt8& num);

    TripodData& newTripodData(Player* pl);
    TripodData& addTripodData(UInt64 id, const TripodData& data);
    TripodData& getTripodData(Player* pl);
    TripodData& getTripodData(UInt64 id);

private:
    std::map<UInt64, TripodData> m_tds;
	FastMutex _mutex;
};

#define tripod Tripod::Instance()

} // namespace GObject

#endif // TRIPOD_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


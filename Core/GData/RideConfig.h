#ifndef _RIDECONFIG_H_
#define _RIDECONFIG_H_

#include "AttrExtra.h"
#include "GDataDBExecHelper.h"

namespace GData
{

#define MOUNTCHIP_MAX 8

class Ride
{
public:
    struct RideData
    {
        UInt8 id;
        std::string name;
        UInt32 itemId;
        UInt32 chips[MOUNTCHIP_MAX];
        UInt32 propId;

        RideData() : id(0), itemId(0), propId(0) { memset(chips, 0, sizeof(chips)); }
    };
    struct RideUpgradeData
    {
        UInt16 level;
        std::string name;
        UInt8 lvLimit;
        UInt32 singleCost;
        UInt32 lvExp;
        float rate;

        RideUpgradeData() : level(0), lvLimit(0), singleCost(0), lvExp(0), rate(0.0f) {}
    };
public:
    void setRideTable(DBRideConfig&);
    void setRideUpgradeTable(DBRideUpgradeCfg&);
    UInt32 getMountItemId(UInt8);
    UInt8 getModifyMountId(UInt32);
    float getMountRate(UInt16);
    RideUpgradeData * getRideUpgTable(UInt16);
    UInt8 getChipPos(UInt8, UInt32);

private:
    std::map<UInt8, RideData> _rideData;
    std::map<UInt16, RideUpgradeData> _rideUpgData;
    std::map<UInt32, UInt8> _rideChipsMap;
};

extern Ride ride;

}

#endif

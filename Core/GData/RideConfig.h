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
        bool show;  //是否在藏剑崖显示

        RideData() : id(0), itemId(0), propId(0), show(false) { memset(chips, 0, sizeof(chips)); }
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
    struct cjBless
    {
        UInt16 times;
        UInt16 prob;
        cjBless() { times = 0; prob = 0; }
    };
    struct CangjianData
    {
        UInt8 floor;
        std::string name;
        UInt16 prob;
        UInt8 otherNum;
        std::vector<cjBless> bless;  //失败祝福值

        CangjianData() : floor(0), prob(0), otherNum(0) {}
    };
public:
    void setRideTable(DBRideConfig&);
    void setRideUpgradeTable(DBRideUpgradeCfg&);
    void setCangjianTable(DBCangjianCfg&);
    bool canShowCangjian(UInt8);
    UInt32 getMountItemId(UInt8);
    UInt8 getModifyMountId(UInt32);
    float getMountRate(UInt16);
    RideUpgradeData * getRideUpgTable(UInt16);
    UInt8 getChipPos(UInt8, UInt32);
    CangjianData * getCangjianTable(UInt8);
    UInt16 getCangjianBless(UInt8 floor, UInt16 v);

private:
    std::map<UInt8, RideData> _rideData;
    std::map<UInt16, RideUpgradeData> _rideUpgData;
    std::map<UInt32, UInt8> _rideChipsMap;
    std::map<UInt8, CangjianData> _cangjianData;
};

extern Ride ride;

}

#endif

#include "Config.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "RideConfig.h"

namespace GData
{

Ride ride;

void Ride::setRideTable(DBRideConfig& dbrc)
{
    if(dbrc.id <= 0)
        return;
    RideData rd;
    rd.id = dbrc.id;
    rd.name = dbrc.name;
    rd.itemId = dbrc.itemId;
    rd.propId = dbrc.propId;

    StringTokenizer tk(dbrc.chips, ";");
    for(UInt8 idx = 0; idx < MOUNTCHIP_MAX && idx < tk.count(); ++ idx)
    {
        rd.chips[idx] = atoi(tk[idx].c_str());
        _rideChipsMap.insert(std::make_pair(rd.chips[idx], rd.id));
    }
    _rideData.insert(std::make_pair(rd.id, rd));
}

void Ride::setRideUpgradeTable(DBRideUpgradeCfg& dbruc)
{
    RideUpgradeData rud;
    rud.level = dbruc.level;
    rud.name = dbruc.name;
    rud.lvLimit = dbruc.lvLimit;
    rud.lvExp = dbruc.lvExp;
    rud.singleCost = dbruc.singleCost;
    rud.rate = dbruc.rate;
    _rideUpgData.insert(std::make_pair(rud.level, rud));
}

UInt32 Ride::getMountItemId(UInt8 rideId)
{
    std::map<UInt8, RideData>::iterator it = _rideData.find(rideId);
    if(it != _rideData.end())
        return it->second.itemId;
    return 0;
}

UInt8 Ride::getModifyMountId(UInt32 chipId)
{
    std::map<UInt32, UInt8>::iterator it = _rideChipsMap.find(chipId);
    if(it != _rideChipsMap.end())
        return it->second;
    return 0;
}

float Ride::getMountRate(UInt16 level)
{
    std::map<UInt16, RideUpgradeData>::iterator it = _rideUpgData.find(level);
    if(it != _rideUpgData.end())
        return it->second.rate;
    return 0.0f;
}

Ride::RideUpgradeData * Ride::getRideUpgTable(UInt16 level)
{
    std::map<UInt16, RideUpgradeData>::iterator it = _rideUpgData.find(level);
    if(it != _rideUpgData.end())
        return &(it->second);
    return NULL;
}

UInt8 Ride::getChipPos(UInt8 rideId, UInt32 chipId)
{
    std::map<UInt8, RideData>::iterator it = _rideData.find(rideId);
    if(it != _rideData.end())
    {
        RideData& rd = it->second;
        for(UInt8 i = 0; i < MOUNTCHIP_MAX; ++ i)
        {
            if(rd.chips[i] == chipId)
                return i;
        }
    }
    return 0xFF;
}

}

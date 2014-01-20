#ifndef _COUPLEUPGRADE_H_
#define _COUPLEUPGRADE_H_

#include "GDataDBExecHelper.h"

namespace GData
{
    
    struct CoupleUpgradeData
    {
        UInt32 levelUpExp;
        UInt32 hp;
        UInt32 attak;
        UInt32 magic_attak;
        float df_critical;
        UInt32 action;

        CoupleUpgradeData(): levelUpExp(0),hp(0),attak(0),magic_attak(0),df_critical(0.0f),action(0) {}
    };

class CoupleUpgrade 
{
public:
        
public:
    void loadUpgradeTable(DBCoupleInfo&);
    CoupleUpgradeData* getUpgradeData(UInt8);
    
private:
    std::map<UInt8, CoupleUpgradeData> _coupleUpgData;
};

extern CoupleUpgrade cu;

}

#endif

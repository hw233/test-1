#include"VipTable.h"

namespace GData
{
    VipBaseTable vipTable;

    void VipBaseTable::InsertVipBase(VipBase* base)
    {
        if( base == NULL || base->GetLevel() <= 0 )
            return;
        vipBaseTable[base->GetLevel()] = base;
    }

    VipBase* VipBaseTable::GetVipBase(UInt8 level)
    {
        return vipBaseTable[level];
    }

    UInt8 VipBaseTable::GetVipLevel(UInt32 coinCount)
    {
        UInt8 level = 0;
        for( UInt8 i = 1 ; i < VIP_MAX ; ++i )
        {
            if( vipBaseTable[i]->GetNeedCoin() <= coinCount )
            {
                ++level;
            }
        }
        return level;
    }
};

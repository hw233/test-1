#pragma once
#ifndef __VIPTABLE_H__
#define __VIPTABLE_H__

#include "Config.h"
#define VIP_MAX 16   //( 0 位不放数据 )

namespace GData
{

    struct Gift
    {
        Gift(UInt32 id,UInt32 c ) : itemId(id) , count(c) {}
        UInt32 itemId;
        UInt32 count;
    };


    class VipBase
    {
        public:
            VipBase(UInt8 l, UInt32 n) : level(l), needCoin(n) {}
            UInt8 GetLevel() const { return level;}
            UInt32 GetNeedCoin() const { return needCoin;}
            void SetGifts(std::vector<Gift> gifts) { vecGift = gifts;}
            std::vector<Gift> GetGifts() { return vecGift;}
        private:
            UInt8 level;
            UInt32 needCoin;
            std::vector<Gift> vecGift;
    };


    class VipBaseTable
    {
        public:
            void InsertVipBase(VipBase* base);
            VipBase* GetVipBase(UInt8 level);
            UInt8 GetVipLevel(UInt32 coinCount);
        private:
            VipBase* vipBaseTable[VIP_MAX];
    };


    extern VipBaseTable vipTable;
}
#endif

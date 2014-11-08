#include "HorcruxHoldAttr.h"
namespace GData
{
    HorcruxHoldAttr horcruxHoldAttr;
    void HorcruxHoldAttr::setHorcruxHoldAttr1(UInt32 exp,UInt32 val)
    {
         _horcruxHoldAttr1.insert( std::make_pair(exp ,val) );
    }
    void HorcruxHoldAttr::setHorcruxHoldAttr2(UInt32 exp,float val)
    {
         _horcruxHoldAttr2.insert( std::make_pair(exp ,val) );
    }
    UInt32  HorcruxHoldAttr::getHorcruxHoldAttr1(UInt32 val)
    {
        std::map<UInt32 , UInt32>::iterator it =  _horcruxHoldAttr1.begin();
        UInt32 res = 0;
        for(;it != _horcruxHoldAttr1.end();++it)
        {
            if(val < it->first )
                break;
            res = it->second;
        }
        return res;
    }
    float  HorcruxHoldAttr::getHorcruxHoldAttr2(UInt32 val)
    {
        std::map<UInt32 , float>::iterator it =  _horcruxHoldAttr2.begin();
        float  res = 0;
        for(;it != _horcruxHoldAttr2.end();++it)
        {
            if(val < it->first )
                break;
            res = it->second;
        }
        return res;
    }
}

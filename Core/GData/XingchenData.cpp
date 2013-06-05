#include "XingchenData.h"


namespace GData
{

    XingchenData xingchenData;

    void XingchenData::setXingchenTable(stXingchen& xcd)
    {
        _xingchen.insert(std::make_pair(xcd.level, xcd));
    }

    XingchenData::stXingchen * XingchenData::getXingchenTable(UInt8 lvl)
    {
        std::map<UInt8, stXingchen>::iterator iter = _xingchen.find(lvl);
        if(iter != _xingchen.end())
            return &(iter->second);
        return NULL;
    }

}

#include "XingchenData.h"


namespace GData
{

    XingchenData xingchenData;

    void XingchenData::setXingchenTable(stXingchen& xcd)
    {
        _xingchen.insert(std::make_pair(xcd.level, xcd));
    }

    XingchenData::stXingchen * XingchenData::getXingchenTable(int lvl)
    {
        if(lvl <= 0)
            return NULL;
        std::map<UInt8, stXingchen>::iterator iter = _xingchen.find(lvl);
        if(iter != _xingchen.end())
            return &(iter->second);
        return NULL;
    }

    XinMoData xinmoData;

    void XinMoData::setXinMoTable(stXinMo& xcd)
    {
        _xinmo.insert(std::make_pair(xcd.level, xcd));
    }

    XinMoData::stXinMo * XinMoData::getXinMoTable(int lvl)
    {
        std::map<UInt8, stXinMo>::iterator iter = _xinmo.find(lvl);
        if(iter != _xinmo.end())
            return &(iter->second);
        return NULL;
    }
    UInt32 XinMoData::getXinMoLev(UInt32 val )
    {
        std::map<UInt8, stXinMo>::iterator iter = _xinmo.begin();
        for(;iter!=_xinmo.end();++iter) 
        {
            if(iter->second.maxVal > val )
                return iter->first;
        }
        return -1;
    }
}

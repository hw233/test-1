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
        UInt32 lev = 0 ;
        for(;iter!=_xinmo.end();++iter) 
        {
            lev = iter->first;
            if(iter->second.maxVal > val )
                break;
        }
        return  lev;
    }

    SkillEvData skillEvData;

    void SkillEvData::setSkillEvData(UInt8 lev, stSkillEv& skillEv)
    {
        _skillEv.insert(std::make_pair(lev, skillEv));
    }

    SkillEvData::stSkillEv* SkillEvData::getSkillEvData(UInt8 lev)
    {
        std::map<UInt8, stSkillEv>::iterator iter = _skillEv.find(lev);
        if(iter != _skillEv.end())
            return &(iter->second);
        return NULL;
    }

}

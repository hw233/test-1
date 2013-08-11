#include "JiguanData.h"
#include "Common/StringTokenizer.h"

namespace GData
{
    JiguanData jiguanData;

    void JiguanData::setJiguanyuInfo(DBJiguanyuConfig & jgyData)
    {
        jiguanyuInfo info;

        StringTokenizer tk(jgyData.molding, ",");
        UInt8 cnt = tk.count();
        for(UInt8 idx = 0; idx < cnt && idx < 9; ++idx)
        {
            info.molding[idx] = atoi(tk[idx].c_str());
        }

        info.jiguanyuId = jgyData.jiguanyuId;
        info.name = jgyData.name;
        info.mark = jgyData.mark;
        info.quality = jgyData.quality;
        info.attrType = jgyData.attrType;
        info.element = jgyData.element;
        info.attrValue = jgyData.attrValue;
        info.skillId = jgyData.skillId;

        _jiguanyuInfo.insert(std::make_pair(info.jiguanyuId, info));
    }

    void JiguanData::setJiguanshuInfo(DBJiguanshuConfig & jgsData)
    {
        jiguanshuInfo info;
        
        info.jgshuLvl = jgsData.jgshuLvl;
        info.needExp = jgsData.needExp;

        _jiguanshuInfo.insert(std::make_pair(info.jgshuLvl, info));
    }
    
    void JiguanData::settuzhiInfo(DBTuzhiConfig & tuzhiData)
    {
        tuzhiInfo info;

        info.tuzhiId = tuzhiData.tuzhiId;
        info.name = tuzhiData.name;
        info.quality = tuzhiData.quality;
        info.needjgsLvl = tuzhiData.needjgsLvl;
        info.jiguanyuId = tuzhiData.jiguanyuId;

        _tuzhiInfo.insert(std::make_pair(info.tuzhiId, info));
    }

    JiguanData::jiguanyuInfo * JiguanData::getJiguanyuInfo(UInt32 jgyuId)
    {
        std::map<UInt8, jiguanyuInfo>::iterator iter = _jiguanyuInfo.find(jgyuId);
        if(iter != _jiguanyuInfo.end())
            return &(iter->second);

        return NULL;
    }

    JiguanData::jiguanshuInfo * JiguanData::getJiguanshuInfo(UInt8 lvl)
    {
        std::map<UInt8, jiguanshuInfo>::iterator iter = _jiguanshuInfo.find(lvl);
        if(iter != _jiguanshuInfo.end())
            return &(iter->second);

        return NULL;
    }
    
    JiguanData::tuzhiInfo * JiguanData::getTuzhiInfo(UInt32 tuzhiId)
    {
        std::map<UInt8, tuzhiInfo>::iterator iter = _tuzhiInfo.find(tuzhiId);
        if(iter != _tuzhiInfo.end())
            return &(iter->second);

        return NULL;
    }
}

#include "JiguanData.h"
#include "Common/StringTokenizer.h"
#include "Common/URandom.h"

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
        info.mark = jgyData.mark;
        info.name = jgyData.name;
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
        info.needExp = jgsData.totalNeedExp;

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
        
        std::map<UInt8, std::vector<UInt32>>::iterator iter = _lvltuzhiInfo.find(info.needjgsLvl);
        if(iter == _lvltuzhiInfo.end())
        {
            std::vector<UInt32> _tuzhiId;
            _tuzhiId.push_back(info.tuzhiId);
            _lvltuzhiInfo.insert(std::make_pair(info.needjgsLvl, _tuzhiId));
        }
        else
            iter->second.push_back(info.tuzhiId);
    }

    void JiguanData::setkeyinInfo(DBKeyinConfig & keyinData)
    {
        keyinInfo info;

        info.keyinId = keyinData.keyinId;
        info.keyinLvl = keyinData.keyinLvl;
        info.name = keyinData.name;
        info.quality = keyinData.quality;
        info.attrType = keyinData.attrType;
        info.attrValue = keyinData.attrValue;
        info.materialA = keyinData.materialA;
        info.materialB = keyinData.materialB;
        info.maxValue = keyinData.maxValue;

        std::map<UInt8, std::map<UInt8, keyinInfo>>::iterator iter = _keyinInfo.find(info.keyinId);
        if(iter == _keyinInfo.end())
        {
            std::map<UInt8, keyinInfo> _lvlkeyinInfo;
            _lvlkeyinInfo.insert(std::make_pair(info.keyinLvl, info));
            _keyinInfo.insert(std::make_pair(info.keyinId, _lvlkeyinInfo));
        }
        else
            iter->second.insert(std::make_pair(info.keyinLvl, info));
    }

    JiguanData::keyinInfo * JiguanData::getKeyinInfo(UInt8 keyinId, UInt8 keyinLvl)
    {
        std::map<UInt8, std::map<UInt8, keyinInfo>>::iterator iter = _keyinInfo.find(keyinId);
        if(iter != _keyinInfo.end())
        {
            std::map<UInt8, keyinInfo>::iterator iterA = iter->second.find(keyinLvl);
            if(iterA != iter->second.end())
                return &(iterA->second);
        }

        return NULL;
    }

    JiguanData::jiguanyuInfo * JiguanData::getJiguanyuInfo(UInt32 jgyuId)
    {
        std::map<UInt32, jiguanyuInfo>::iterator iter = _jiguanyuInfo.find(jgyuId);
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

    JiguanData::jiguanshuInfo * JiguanData::getUpgradeInfo(UInt32 curExp)
    {
        std::map<UInt8, jiguanshuInfo>::iterator iter = _jiguanshuInfo.begin();
        for(; iter!=_jiguanshuInfo.end(); iter++)
        {
            if((iter->second.jgshuLvl >= 20) || (iter->second.needExp >= curExp))
            {
                return &(iter->second);
            }
        }

        return NULL;
    }
    
    JiguanData::tuzhiInfo * JiguanData::getTuzhiInfo(UInt32 tuzhiId)
    {
        std::map<UInt32, tuzhiInfo>::iterator iter = _tuzhiInfo.find(tuzhiId);
        if(iter != _tuzhiInfo.end())
            return &(iter->second);

        return NULL;
    }

    UInt32 JiguanData::getTuzhiId(UInt8 lvl)
    {
        std::map<UInt8, std::vector<UInt32>>::iterator iter = _lvltuzhiInfo.find(lvl);
        if(iter != _lvltuzhiInfo.end())
        {
            //UInt8 count = iter->second.size();
            //UInt8 index = uRand(count);

            UInt8 color = 0;
            UInt8 randColor = uRand(100);

            if(randColor < 20)
            {
                color = 0;
            }
            else if(randColor < 70)
            {
                color = 1;
            }
            else if(randColor < 95)
            {
                color = 2;
            }
            else
            {
                color = 3;
            }

            UInt8 index = uRand(3);
            index = 4 * index + color;
            UInt8 count = iter->second.size();
            
            if(index < count)
                return iter->second[index];
            else
                return 0;
        }

        return 0;
    }
}

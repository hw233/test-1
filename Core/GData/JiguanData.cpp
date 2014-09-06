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
        info.totalExp = jgsData.totalNeedExp;
        info.needExp = jgsData.needExp;
        info.attrValue1 = jgsData.attrValue1;
        info.attrValue2 = jgsData.attrValue2;
        info.attrValue3 = jgsData.attrValue3;
        info.attrValue4 = jgsData.attrValue4;

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

    void JiguanData::setZhenweiInfo(DBZhenweiConfig & zhenweiData)
    {
        if(zhenweiData.keyId > 0)
        {
            zhenweiInfo info;

            info.keyId = zhenweiData.keyId;
            info.name = zhenweiData.name;
            info.type = zhenweiData.type;
            info.collect1 = zhenweiData.collect1;
            info.collect2 = zhenweiData.collect2;
            info.collect3 = zhenweiData.collect3;
            info.collect4 = zhenweiData.collect4;
            info.collect5 = zhenweiData.collect5;
            info.collect6 = zhenweiData.collect6;
            info.collect7 = zhenweiData.collect7;
            info.collect8 = zhenweiData.collect8;
            info.award = zhenweiData.award;

            _zhenweiInfo.insert(std::make_pair(info.keyId, info));
        }
    }

    void JiguanData::setGearInfo(DBGearConfig & gearData)
    {
        if(gearData.partId > 0)
        {
            gearInfo info;

            info.partId = gearData.partId;
            info.name = gearData.name;
            info.ZYKNum = gearData.needMoneyA;
            info.ZYMNum = gearData.needMoneyB;
            info.ZYSNum = gearData.needMoneyC;
            info.ZYLFNum = gearData.needMoneyD;
            info.taelNum = gearData.needMoneyE;
            info.attrValueA = gearData.attrValueA;
            info.attrValueB = gearData.attrValueB;
            info.attrValueC = gearData.attrValueC;
            info.attrValueD = gearData.attrValueD;
            info.attrValueE = gearData.attrValueE;
            info.attrValueF = gearData.attrValueF;
            info.attrValueG = gearData.attrValueG;

            _gearInfo.insert(std::make_pair(info.partId, info));
        }
    }

    void JiguanData::setGearTreeInfo(DBGearTreeConfig & gearTreeData)
    {
        if(gearTreeData.suiteId > 0)
        {
            needPart np;
            needComponent nc1;
            needComponent nc2;
            needComponent nc3;
            
            np.lastSuiteId = gearTreeData.lastSuiteId;
            np.needPartAId = gearTreeData.partAId;
            np.needPartBId = gearTreeData.partBId;
            np.needPartCId = gearTreeData.partCId;
            nc1.lastSuiteId = gearTreeData.lastSuiteId;
            nc1.needComponentAId = gearTreeData.componentAId;
            nc1.needComponentBId = gearTreeData.componentBId;
            nc2.lastSuiteId = gearTreeData.lastSuiteId;
            nc2.needComponentAId = gearTreeData.componentCId;
            nc2.needComponentBId = gearTreeData.componentDId;
            nc3.lastSuiteId = gearTreeData.lastSuiteId;
            nc3.needComponentAId = gearTreeData.componentEId;
            nc3.needComponentBId = gearTreeData.componentFId;
            
            /**套件由部件组成，部件由元件组成**/
            _makeComponent.insert(std::make_pair(gearTreeData.componentAId, gearTreeData.lastSuiteId));
            _makeComponent.insert(std::make_pair(gearTreeData.componentBId, gearTreeData.lastSuiteId));
            _makeComponent.insert(std::make_pair(gearTreeData.componentCId, gearTreeData.lastSuiteId));
            _makeComponent.insert(std::make_pair(gearTreeData.componentDId, gearTreeData.lastSuiteId));
            _makeComponent.insert(std::make_pair(gearTreeData.componentEId, gearTreeData.lastSuiteId));
            _makeComponent.insert(std::make_pair(gearTreeData.componentFId, gearTreeData.lastSuiteId));
            _makePart.insert(std::make_pair(np.needPartAId, nc1));
            _makePart.insert(std::make_pair(np.needPartBId, nc2));
            _makePart.insert(std::make_pair(np.needPartCId, nc3));
            _makeSuite.insert(std::make_pair(gearTreeData.suiteId, np));
            _tempType.insert(std::make_pair(gearTreeData.suiteId, 1));      //套件
            _tempType.insert(std::make_pair(gearTreeData.partAId, 2));      //部件
            _tempType.insert(std::make_pair(gearTreeData.partBId, 2));
            _tempType.insert(std::make_pair(gearTreeData.partCId, 2));
            _tempType.insert(std::make_pair(gearTreeData.componentAId, 3)); //元件
            _tempType.insert(std::make_pair(gearTreeData.componentBId, 3));
            _tempType.insert(std::make_pair(gearTreeData.componentCId, 3));
            _tempType.insert(std::make_pair(gearTreeData.componentDId, 3));
            _tempType.insert(std::make_pair(gearTreeData.componentEId, 3));
            _tempType.insert(std::make_pair(gearTreeData.componentFId, 3));
        }
    }

    UInt8 JiguanData::getGearType(UInt16 gearId)
    {
        std::map<UInt16, UInt8>::iterator iter = _tempType.find(gearId);
        if(iter != _tempType.end())
            return iter->second;

        return 0;
    }

    UInt16 JiguanData::getMakeComponentInfo(UInt16 gearId)
    {
        std::map<UInt16, UInt16>::iterator iter = _makeComponent.find(gearId);
        if(iter != _makeComponent.end())
            return iter->second;

        return 0;
    }

    JiguanData::needComponent * JiguanData::getMakePartInfo(UInt16 gearId)
    {
        std::map<UInt16, needComponent>::iterator iter = _makePart.find(gearId);
        if(iter != _makePart.end())
            return &(iter->second);

        return NULL;
    }

    JiguanData::needPart * JiguanData::getMakeSuiteInfo(UInt16 gearId)
    {
        std::map<UInt16, needPart>::iterator iter = _makeSuite.find(gearId);
        if(iter != _makeSuite.end())
            return &(iter->second);

        return NULL;
    }

    JiguanData::gearInfo * JiguanData::getGearInfo(UInt16 gearId)
    {
        std::map<UInt16, gearInfo>::iterator iter = _gearInfo.find(gearId);
        if(iter != _gearInfo.end())
            return &(iter->second);

        return NULL;
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
    #define JGS_MAXLVL          70
        std::map<UInt8, jiguanshuInfo>::iterator iter = _jiguanshuInfo.begin();
        for(; iter!=_jiguanshuInfo.end(); iter++)
        {
            if((iter->second.jgshuLvl == JGS_MAXLVL) || (iter->second.totalExp >= curExp))
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

            UInt8 count = iter->second.size();
            UInt8 index = uRand(count/4);
            index = 4 * index + color;
            
            if(index < count)
                return iter->second[index];
            else
                return 0;
        }

        return 0;
    }

    JiguanData::zhenweiInfo * JiguanData::getZhenweiInfo(UInt16 keyId)
    {
        std::map<UInt16, zhenweiInfo>::iterator iter = _zhenweiInfo.find(keyId);
        if(iter != _zhenweiInfo.end())
            return &(iter->second);

        return NULL;
    }
}

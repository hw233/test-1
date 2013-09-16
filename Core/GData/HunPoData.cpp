#include "HunPoData.h"
#include "Common/StringTokenizer.h"
#include "Common/URandom.h"

namespace GData
{
    HunPoData hunpoData;


    void HunPoData::setSanHunInfo(DBSanHunConfig & sanHunData)
    {
        sanhunInfo info;

        info.sanhunLvl = sanHunData.sanhunLvl;
        info.attr1 = sanHunData.attr1;
        info.attr2 = sanHunData.attr2;
        info.attr3 = sanHunData.attr3;
        info.attr4 = sanHunData.attr4;
        info.attr5 = sanHunData.attr5;
        info.attr6 = sanHunData.attr6;
        info.attr7 = sanHunData.attr7;
        info.attr8 = sanHunData.attr8;
        info.attr9 = sanHunData.attr9;
        info.money1 = sanHunData.money1;
        info.money2 = sanHunData.money2;

        _sanhunInfo.insert(std::make_pair(info.sanhunLvl, info));
    }

    HunPoData::sanhunInfo * HunPoData::getSanHunInfo(UInt8 SHLvl)
    {
        std::map<UInt8, sanhunInfo>::iterator iter = _sanhunInfo.find(SHLvl);
        if(iter != _sanhunInfo.end())
            return &(iter->second);

        return NULL;
    }
} 

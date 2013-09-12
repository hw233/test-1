#ifndef _HUNPODATA_H_
#define _HUNPODATA_H_

#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"

namespace GData
{

class HunPoData
{
    public:
        struct sanhunInfo
        {
            UInt8 sanhunLvl;     // 三魂等级
            float attr1;         // 属性1
            float attr2;         // 属性2
            float attr3;         // 属性3
            float attr4;         // 属性4
            float attr5;         // 属性5
            float attr6;         // 属性6
            float attr7;         // 属性7
            float attr8;         // 属性8
            float attr9;         // 属性9
            UInt32 money1;       // 消耗1
            UInt32 money2;       // 消耗2

            sanhunInfo(): sanhunLvl(0), attr1(0.00 ), attr2(0.00), attr3(0.00), attr4(0.00), attr5(0.00), attr6(0.00), attr7(0.00), attr8(0.00), attr9(0.00), money1(0), money2(0) {}
        };

    private:
        std::map<UInt8, sanhunInfo> _sanhunInfo;

    public:
        void setSanHunInfo(DBSanHunConfig &);

        sanhunInfo * getSanHunInfo(UInt8 SHLvl);

};

extern HunPoData hunpoData;

}

#endif

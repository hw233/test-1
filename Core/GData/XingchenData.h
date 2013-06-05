#ifndef _XINGCHENDATA_H_
#define _XINGCHENDATA_H_

#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"

namespace GData
{

class XingchenData
{
    public:
        struct stXingchen
        {
            UInt8 level;
            UInt32 maxVal;
            UInt32 consume;
            float rate;
        };
    private:
        std::map<UInt8, stXingchen> _xingchen;
    public:
        void setXingchenTable(stXingchen&);
        stXingchen * getXingchenTable(UInt8);
};

extern XingchenData xingchenData;

}

#endif // _XINGCHENDATA_H_

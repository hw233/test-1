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
            UInt8 limitLev;
            std::string name;
            UInt32 consume;
            UInt32 maxVal;
            float rate1;    //基础宝石加成
            float rate2;    //攻防宝石加成
            float rate3;    //身法宝石加成
            UInt32 payBack; //解雇补偿
            stXingchen(): level(0), limitLev(0), name(""), consume(0), maxVal(0), rate1(0.0), rate2(0.0), rate3(0.0), payBack(0) {}
        };
    private:
        std::map<UInt8, stXingchen> _xingchen;
    public:
        void setXingchenTable(stXingchen&);
        stXingchen * getXingchenTable(int);
};

extern XingchenData xingchenData;

}

#endif // _XINGCHENDATA_H_

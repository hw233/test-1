#ifndef _LINGSHITABLE_H_
#define _LINGSHITABLE_H_

#include "AttrExtra.h"
#include "GDataDBExecHelper.h"

namespace GData
{

struct LingshiData
{
    UInt8 level;
    UInt32 consume;     //吞噬银币消耗
    UInt32 exp;
    bool isBreak;   //等级临界点时,若可突破就不可升级
    UInt16 useItem;
    UInt16 useGold;
    AttrExtra attrs;

    LingshiData() : level(0), consume(0), exp(0), isBreak(false), useItem(0), useGold(0) {}

    bool canUpgrade(UInt32& exp);
};

class LingshiCls
{
public:
    void setLingshiTable(DBLingShi&);
    LingshiData * getLingshiData(UInt8);
    bool canUpgrade(UInt8, UInt32&);
    bool canBreak(UInt8);
    UInt32 getLingShiExp(UInt8);
    UInt32 getLevUpTael(UInt8);
private:
    std::map<UInt8, LingshiData> _lingshiData;
};

extern LingshiCls lingshiCls;

}

#endif

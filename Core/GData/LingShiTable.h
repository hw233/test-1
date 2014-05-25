#ifndef _LINGSHITABLE_H_
#define _LINGSHITABLE_H_

#include "AttrExtra.h"
#include "GDataDBExecHelper.h"

namespace GData
{

struct LingshiData
{
    UInt16 id;
    UInt8 level;
    bool isBreak;   //等级临界点时,若可突破就不可升级
    UInt16 useItem;
    UInt16 useGold;
    AttrExtra attrs;

    LingshiData() : id(0), level(0), isBreak(false), useItem(0), useGold(0) {}
};
typedef std::vector<LingshiData> LingshiDataVec;
struct LingshiUpgrade
{
    UInt8 level;
    UInt32 consume;     //吞噬银币消耗
    UInt32 exp;

    LingshiUpgrade() : level(0), consume(0), exp(0) {}
};

class LingshiCls
{
public:
    void setLingshiTable(DBLingShi&);
    void setLingshiUpTable(DBLingShiUp&);
    UInt8 getLingshiMaxLev(UInt32, UInt8);
    LingshiData * getLingshiData(UInt32, UInt8);
    UInt32 getLingShiMaxExp(UInt8);
    UInt32 getLevUpTael(UInt8);
    bool canUpgrade(UInt32, UInt8, UInt32&);
    bool canBreak(UInt32, UInt8);
private:
    std::map<UInt32, LingshiDataVec> _lingshiData;
    std::map<UInt8, LingshiUpgrade> _lingshiUp;
};

extern LingshiCls lingshiCls;

}

#endif

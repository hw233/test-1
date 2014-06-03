#include "Config.h"
#include "Common/StringTokenizer.h"
#include "LingShiTable.h"


namespace GData
{

LingshiCls lingshiCls;

bool LingshiData::canUpgrade(UInt32& exp_)
{
    if(isBreak)
    {
        if(exp_ >= exp)
        {
            exp_ = exp;
            return false;
        }
        return true;
    }
    return !isBreak;
}

void LingshiCls::setLingshiTable(DBLingShi& dbls)
{
    LingshiData lsd;
    lsd.level = dbls.level;
    lsd.consume = dbls.consume;
    lsd.exp = dbls.exp;
    lsd.isBreak = dbls.isBreak > 0;
    lsd.useItem = dbls.useItem;
    lsd.useGold = dbls.useGold;

    lsd.attrs.attack = dbls.attack;
    lsd.attrs.magatk = dbls.magatk;
    lsd.attrs.hp = dbls.hp;
    lsd.attrs.action = dbls.action;

    _lingshiData.insert(std::make_pair(lsd.level, lsd));
}

LingshiData * LingshiCls::getLingshiData(UInt8 lvl)
{
    std::map<UInt8, LingshiData>::iterator iter = _lingshiData.find(lvl);
    if(iter == _lingshiData.end())
        return NULL;
    return &(iter->second);
}

bool LingshiCls::canUpgrade(UInt8 lvl, UInt32& exp)
{
    std::map<UInt8, LingshiData>::iterator iter = _lingshiData.find(lvl);
    if(iter == _lingshiData.end())
        return false;
    bool isBreak = iter->second.isBreak;
    if(isBreak)
    {
        if(exp >= iter->second.exp)
        {
            exp = iter->second.exp;
            return false;
        }
        return true;
    }
    return !isBreak;
}

bool LingshiCls::canBreak(UInt8 lvl)
{
    std::map<UInt8, LingshiData>::iterator iter = _lingshiData.find(lvl);
    if(iter == _lingshiData.end())
        return false;
    return iter->second.isBreak;
}

UInt32 LingshiCls::getLingShiExp(UInt8 lvl)
{
    std::map<UInt8, LingshiData>::iterator iter = _lingshiData.find(lvl);
    if(iter == _lingshiData.end())
        return 0;
    return iter->second.exp;
}

UInt32 LingshiCls::getLevUpTael(UInt8 lvl)
{
    std::map<UInt8, LingshiData>::iterator iter = _lingshiData.find(lvl);
    if(iter == _lingshiData.end())
        return 0;
    return iter->second.consume;
}

}

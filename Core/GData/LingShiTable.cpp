#include "Config.h"
#include "Common/StringTokenizer.h"
#include "LingShiTable.h"


namespace GData
{

LingshiCls lingshiCls;

void LingshiCls::setLingshiTable(DBLingShi& dbls)
{
    LingshiData lsd;
    lsd.id = dbls.id;
    lsd.level = dbls.level;
    lsd.isBreak = dbls.isBreak > 0;
    lsd.useItem = dbls.useItem;
    lsd.useGold = dbls.useGold;

    lsd.attrs.attack = dbls.attack;
    lsd.attrs.magatk = dbls.magatk;
    lsd.attrs.defend = dbls.defend;
    lsd.attrs.magdef = dbls.magdef;
    lsd.attrs.hp = dbls.hp;
    lsd.attrs.toughlvl = dbls.toughlvl;
    lsd.attrs.action = dbls.action;
    lsd.attrs.hitrlvl = dbls.hitrlvl;
    lsd.attrs.evdlvl = dbls.evdlvl;
    lsd.attrs.crilvl = dbls.crilvl;
    lsd.attrs.criticaldmg = dbls.criticaldmg;
    lsd.attrs.pirlvl = dbls.pirlvl;
    lsd.attrs.counterlvl = dbls.counterlvl;
    lsd.attrs.mreslvl = dbls.mreslvl;
    std::map<UInt32, LingshiDataVec>::iterator iter = _lingshiData.find(lsd.id);
    if(iter == _lingshiData.end())
    {
        LingshiDataVec lsdVec;
        lsdVec.push_back(lsd);
        _lingshiData.insert(std::make_pair(lsd.id, lsdVec));
    }
    else
        iter->second.push_back(lsd);
}

void LingshiCls::setLingshiUpTable(DBLingShiUp& dblsu)
{
    LingshiUpgrade lsup;
    lsup.level = dblsu.level;
    lsup.consume = dblsu.consume;
    lsup.exp = dblsu.exp;
    _lingshiUp.insert(std::make_pair(lsup.level, lsup));
}

UInt8 LingshiCls::getLingshiMaxLev(UInt32 lid, UInt8 lvl)
{
    std::map<UInt32, LingshiDataVec>::iterator iter = _lingshiData.find(lid);
    if(iter == _lingshiData.end() || lvl > iter->second.size())
        return 0;
    LingshiDataVec::iterator it = iter->second.begin();
    std::advance(it, lvl);
    for(; it != iter->second.end(); ++ it)
    {
        if((*it).isBreak)
            return (*it).level;
    }
    return iter->second.size();
}

UInt32 LingshiCls::getLingShiMaxExp(UInt8 lvl)
{
    std::map<UInt8, LingshiUpgrade>::iterator iter = _lingshiUp.find(lvl);
    if(iter == _lingshiUp.end())
        return 0;
    return iter->second.exp;
}

UInt32 LingshiCls::getLevUpTael(UInt8 lvl)
{
    std::map<UInt8, LingshiUpgrade>::iterator iter = _lingshiUp.find(lvl);
    if(iter == _lingshiUp.end())
        return 0.0f;
    return iter->second.consume;
}

LingshiData * LingshiCls::getLingshiData(UInt32 lid, UInt8 lvl)
{
    std::map<UInt32, LingshiDataVec>::iterator iter = _lingshiData.find(lid);
    if(iter == _lingshiData.end())
        return NULL;
    LingshiDataVec::iterator it = iter->second.begin();
    for(; it != iter->second.end(); ++ it)
    {
        if((*it).level == lvl)
            return &(*it);
    }
    return NULL;
}

bool LingshiCls::canUpgrade(UInt32 lid, UInt8 lvl, UInt32& exp)
{
    std::map<UInt32, LingshiDataVec>::iterator iter = _lingshiData.find(lid);
    if(iter == _lingshiData.end())
        return false;
    if(lvl >= iter->second.size())
        return false;
    UInt8 tmpLvl = lvl >= 1 ? lvl - 1 : lvl;
    bool isBreak = iter->second[tmpLvl].isBreak;
    if(isBreak)
    {
        std::map<UInt8, LingshiUpgrade>::iterator it = _lingshiUp.find(lvl);
        if(it == _lingshiUp.end())
            return false;
        if(exp >= it->second.exp)
        {
            exp = it->second.exp;
            return false;
        }
        return true;
    }
    return !isBreak;
}

bool LingshiCls::canBreak(UInt32 lid, UInt8 lvl)
{
    std::map<UInt32, LingshiDataVec>::iterator iter = _lingshiData.find(lid);
    if(iter == _lingshiData.end())
        return false;
    if(lvl >= iter->second.size())
        return false;
    lvl = lvl >= 1 ? lvl - 1 : lvl;
    return iter->second[lvl].isBreak;
}

}

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


class XinMoData
{
    public:
        struct stXinMo
        {
            UInt8 level;
            UInt8 limitLev;
            std::string name;
            UInt32 consume;
            UInt32 maxVal;
            //float rate1;    //基础宝石加成
            //float rate2;    //攻防宝石加成
            //float rate3;    //身法宝石加成
            UInt32 attack;
            UInt32 hp;
            UInt32 action;
            float cridec;
            std::string skilllev;
            UInt32 payBack; //解雇补偿
            stXinMo(): level(0), limitLev(0), name(""), consume(0), maxVal(0), attack(0), hp(0), action(0), cridec(0.0),skilllev(""), payBack(0) {}
        };
    private:
        std::map<UInt8, stXinMo> _xinmo;
    public:
        void setXinMoTable(stXinMo&);
        stXinMo * getXinMoTable(int);
        UInt32 getXinMoLev(UInt32 val);
};

class SkillEvData
{
    public:
        struct stSkillEv
        {
            UInt32 effect;
            UInt32 consume;
            stSkillEv() : effect(0), consume(0) {}
        };
    private:
        std::map<UInt8, stSkillEv> _skillEv;
    public:
        void setSkillEvData(UInt8 lev, stSkillEv& skillEv);
        stSkillEv* getSkillEvData(UInt8 lev);
        UInt8 getSkillEvSize() { return static_cast<UInt8>(_skillEv.size()); }

};

extern XinMoData xinmoData;
extern XingchenData xingchenData;
extern SkillEvData skillEvData;
}

#endif // _XINGCHENDATA_H_

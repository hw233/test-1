#ifndef _FAIRYPETTABLE_H_
#define _FAIRYPETTABLE_H_

#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"

namespace GData
{

class Pet
{
public:
    struct stBless
    {
        UInt16 times;
        UInt16 prob;
        stBless() { times = 0; prob = 0; }
    };
	struct PinjieData
	{
        UInt16 lev; //id
        std::string name;   //名称
		UInt32 consume;     //品阶升级消耗
        UInt16 prob;        //成功概率
        UInt8 skillLev[4];  //技能升级
        std::vector<stBless> bless;  //失败祝福值
        PinjieData(): lev(0), name(""), consume(0), prob(0)
        { memset(skillLev, 0, sizeof(skillLev)); }
	};
	struct GenguData
	{
        UInt16 id;
        std::string name;   //名称
        UInt8 limit;        //突破时限制的重数
		UInt16 baseProb;    //成功概率
		UInt8 failBack;     //失败回退的重数
		UInt32 consume1;    //小周天消耗的凤髓
		UInt32 consume2;    //大周天消耗的凤髓
        float growRate;     //成长倍率
	};
	struct LingyaData
	{
        UInt32 petId;
        UInt8 color;
        UInt8 lingya;
        UInt16 initBone;
        UInt16 finalBone;
	};
public:
    void setLevTable(DBPinJie&);
    void setBoneTable(GenguData&);
    void setLingyaTable(LingyaData&);
    PinjieData * getLevTable(UInt16);
    GenguData * getBoneTable(UInt16);
    LingyaData * getLingyaTable(UInt32);
    UInt16 getPinjieBless(UInt16, UInt16);
private:
    std::map<UInt16, PinjieData> _levData;
    std::map<UInt16, GenguData> _boneData;
    std::map<UInt32, LingyaData> _lingyaData;
};

extern Pet pet;
}

#endif // _FAIRYPETTABLE_H_

#ifndef _FAIRYPETTABLE_H_
#define _FAIRYPETTABLE_H_

#include <map>
#include "Config.h"

namespace GData
{

class Pet
{
public:
	struct PinjieData
	{
        UInt16 lev; //id
        std::string name;   //名称
		UInt32 consume;     //品阶升级消耗
        UInt16 prob;        //成功概率
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
	};
public:
    void setLevTable(PinjieData&);
    void setBoneTable(GenguData&);
    void setLingyaTable(LingyaData&);
    PinjieData * getLevTable(UInt16);
    GenguData * getBoneTable(UInt16);
    LingyaData * getLingyaTable(UInt32);
private:
    std::map<UInt16, PinjieData> _levData;
    std::map<UInt16, GenguData> _boneData;
    std::map<UInt32, LingyaData> _lingyaData;
};

extern Pet pet;
}

#endif // _FAIRYPETTABLE_H_

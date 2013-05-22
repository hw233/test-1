#ifndef _FAIRYPETTABLE_H_
#define _FAIRYPETTABLE_H_

#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"
#include "ItemType.h"

namespace GData
{

class Pet
{
#define PETEQUIP_MAXLEV 71
#define PETEQUIP_ATTR 11
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
	struct EquipExpData
	{   //仙宠装备升级经验
        UInt8 level;
        UInt8 skillLev;
        UInt32 levExp[4];   //绿蓝紫橙
        EquipExpData(): level(0), skillLev(0)
        { memset(levExp, 0, sizeof(levExp)); }
	};
    struct EquipAttreffect
    {
        UInt8 level;
        float effect[PETEQUIP_ATTR];
        //生命,物理攻击,法术攻击,物理防御,法术防御,暴击,破击,命中,闪避,反击,坚韧
        EquipAttreffect(): level(0)
        { memset(effect, 0, sizeof(effect)); }
    };
public:
    void setLevTable(DBPinJie&);
    void setBoneTable(GenguData&);
    void setLingyaTable(LingyaData&);
    void setEqExpTable(EquipExpData&);
    void setEqAttreffect(EquipAttreffect&);
    PinjieData * getLevTable(UInt16);
    GenguData * getBoneTable(UInt16);
    LingyaData * getLingyaTable(UInt32);
    UInt16 getPinjieBless(UInt16, UInt16);
    float getPetPotential(UInt16);
    UInt32 getEquipExpData(UInt8, int);
    UInt8 getEquipSkillLev(UInt8);
    UInt8 getEquipMaxLev();
    float getEquipAttreffect(UInt8, ItemClass);
private:
    std::map<UInt16, PinjieData> _levData;
    std::map<UInt16, GenguData> _boneData;
    std::map<UInt32, LingyaData> _lingyaData;
    std::map<UInt8, EquipExpData> _equipExpData;
    std::map<UInt8, EquipAttreffect> _equipAttreffect;
};

extern Pet pet;
}

#endif // _FAIRYPETTABLE_H_

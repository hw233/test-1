﻿#ifndef  _FAIRYPET_H
#define  _FAIRYPET_H


#define INIT_SKILL_UPMAX 4
#define PET_EQUIP_UPMAX 3
#define PROB_BASE 10000
#define GET_REMAINDER(val)  (val % 10)
#define MAX_EVOLVE_VALUE 100
#define EVOLVE_BONE_LIMIT 30

class Stream;
namespace GObject
{

class Player;
struct DBFairyPetData;

// 仙宠
class FairyPet:public Fighter
{
private:
    UInt16 _petLev;    // 品阶(第一品阶为50级，之后每一品阶等价10级) 重天 (每十重天升一品阶)
    UInt16 _petBone;     //根骨的阶段 (每阶段分为0:下品、1:中品、2:上品)
    bool _onBattle;   // 出战
    UInt16 _chong;   //根骨升级的重数
    UInt16 _pinjieBless;    //品阶升级失败的次数
    UInt32 _genguBless;     //突破时大小周天增加的祝福值
    UInt8 _lingya;      //仙宠灵压
    UInt32 _overTime;
    UInt16 _xiaozhou;
    UInt16 _dazhou;
    UInt16 _evolve;     //仙宠进化值(紫色)
    UInt16 _initskl[INIT_SKILL_UPMAX];
    ItemPetEq * _equips[PET_EQUIP_UPMAX];
public:
    FairyPet(UInt32, Player *);
    void LoadFromDB(DBFairyPetData&);
    void LoadInitSkills(std::string&);
    void UpdateToDB();
    bool checkTimeOver();
    virtual FairyPet * clone(Player *);
    void upgradeLev();
    void upgradeLevAuto();
    void upgradeBone();
    void useZhoutian(UInt8);
    void sendPinjieInfo();
    void sendGenguInfo();
    void sendPetEvolve();
    UInt16 addPetEvolveInlua(UInt16);
    void initSkillUp();
    void delSkills(std::string&);
    void AppendEquipData(Stream& st);
    void sendModification(ItemPetEq * equip, UInt8 pos);
    ItemPetEq * findEquip(UInt8 pos);
    ItemPetEq * findEquip(UInt32 id, UInt8& pos);
    ItemPetEq * setEquip(ItemPetEq * eq, UInt8& pos, bool writedb = true);
    void rebuildEquipAttr();

    inline bool isOnBattle()            { return _onBattle; }
    inline void setOnBattle(bool flag)  { _onBattle = flag; }
    inline void setPetEvolve(UInt16 v)  { _evolve = v > getPetEvMax() ? getPetEvMax() : v; }
    inline void setPetBone(UInt16 v)    { _petBone = v; }
    inline void setPetLev(UInt16 v)     { _petLev = v; }
    inline void setXiaozhou(UInt16 v)   { _xiaozhou = v; }
    inline void setDazhou(UInt16 v)     { _dazhou = v; }
    inline void setPetLingya(UInt8 ly)  { _lingya = ly; }
    inline void setChongNum(UInt16 v)   { _chong = v;; }
    inline void setGenguBless(UInt16 v) { _genguBless = v; }
    inline void setPinjieBless1(UInt16 v) { _pinjieBless = v; }
    inline UInt8 getPetBone()           { return _petBone; }
    inline UInt8 getPetLev()            { return _petLev; }
    inline UInt16 getXiaozhou()         { return _xiaozhou; }
    inline UInt16 getDazhou()           { return _dazhou; }
    inline UInt8 getPetLingya()         { return _lingya; }
    inline UInt8 getChongNum()          { return _chong; }
    inline UInt32 getGenguBless()       { return _genguBless; }
    inline UInt16 getPinjieBless1()     { return _pinjieBless; }
    inline UInt8 getPetEvolve()         { return _evolve; }
    inline UInt8 getPetEvMax()          { return MAX_EVOLVE_VALUE; }

    inline UInt8 getTargetPos()
    {
        switch (getClass())
        {
            case e_cls_qinglong:
                return 24;
            case e_cls_xuanwu:
                return 4;
            case e_cls_zhuque:
                return 0;
            case e_cls_baihu:
                return 20;
        }
        return 0;
    }

private:
    UInt16 getPinjieBless();
    void addChongNum(int);
    void addGenguBless(int);
    void addPinjieBless(UInt16);
    void boneUp();
    void levUp();
    bool canLevUp();
    bool canBoneUp();
    void reset(UInt8 type);
};

}
#endif

#ifndef  _FAIRYPET_H
#define  _FAIRYPET_H


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
    UInt16 _pinjieBless;    //品阶升级失败时获得祝福值
    UInt32 _genguBless;     //突破时大小周天增加的祝福值
    UInt8 _lingya;      //仙宠灵压
    UInt32 _overTime;
    UInt16 _xiaozhou;
    UInt16 _dazhou;
public:
    FairyPet(UInt32, Player *);
    void LoadFromDB(DBFairyPetData&);
    void UpdateToDB(bool = false);
    bool checkTimeOver();
    FairyPet * clone(Player *);
    inline bool isOnBattle() { return _onBattle; }
    inline void setOnBattle(bool flag) { _onBattle = flag; }
    void upgradeLev();
    void upgradeLevAuto();
    void upgradeBone();
    void useZhoutian(UInt8);
    void sendPinjieInfo();
    void sendGenguInfo();
    inline UInt8 getPetLev() { return _petLev; }
    inline UInt8 getPetBone() { return _petBone; }
    inline UInt8 getPetLingya() { return _lingya; }
    inline void setPetLingya(UInt8 ly) { _lingya = ly; }
private:
    inline UInt8 getChongNum() { return _chong; }
    inline UInt16 getPinjieBless() { return _pinjieBless; }
    inline UInt32 getGenguBless() { return _genguBless; }
    inline UInt16 getXiaozhou() { return _xiaozhou; }
    inline UInt16 getDazhou() { return _dazhou; }
    void addChongNum(int);
    void addGenguBless(int);
    void addPinjieBless(int);
    inline void setXiaozhou(UInt16 v) { _xiaozhou = v; }
    inline void setDazhou(UInt16 v) { _dazhou = v; }
    void boneUp();
    bool canLevUp();
    bool canBoneUp();
    void reset(UInt8 type);
};

}
#endif

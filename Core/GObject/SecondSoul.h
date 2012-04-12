#ifndef _SECOND_SOUL_H_
#define _SECOND_SOUL_H_

#include "Config.h"
#include "GData/AttrExtra.h"


namespace GObject
{

class Fighter;
class Player;

struct SoulSkill
{
    UInt8 id;
    UInt8 level;
};

#define MAX_SKILL_NUM 6

#define SOUL_SKILL_ATTRTRANS     1          // 增幅术, 给角色提供元神属性x%的提升
#define SOUL_SKILL_STRENGHT      2          // 力量, 增加元神的力量
#define SOUL_SKILL_INTELLIGENCE  3          // 智力, 增加元神的智力
#define SOUL_SKILL_PHYSIQUE      4          // 耐力, 增加元神的耐力
#define SOUL_SKILL_AGILITY       5          // 敏捷, 增加元神的敏捷
#define SOUL_SKILL_WILL          6          // 意志, 增加元神的意志
#define SOUL_SKILL_HPFACTOR      7          // 坚韧术, 增加角色的生命上限
#define SOUL_SKILL_ACUADDON      8          // 伐脉术, 修炼速度增加
#define SOUL_SKILL_ACUFACTOR     9          // 吐纳术, 修炼速度增加
#define SOUL_SKILL_AURAEXTRA     10         // 聚灵术, 灵气获得额外增加
#define SOUL_SKILL_AURAMAX       11         // 通灵术, 灵气上限增加
#define SOUL_SKILL_PRACLVLUP     12         // 天赋术, 减少元神升级的修为消耗
//#define SOUL_SKILL_ALLATTR      11          // 通神术, 增加元神所有属性

class SecondSoul
{
    friend class Fighter;
public:
    SecondSoul(Fighter* fgt, UInt8 cls, UInt8 practiceLevel = 1, UInt32 stateExp = 0, UInt8 stateLevel = 1 );
    void insertIntoDB();

    bool setClass(UInt8 cls);
    UInt8 getClass() { return m_cls; }
    UInt8 getPracticeLevel() { return m_practiceLevel; }
    UInt32 getStateExp() { return m_stateExp; }
    UInt8 getStateLevel() { return m_stateLevel; }
    void addStateExp(UInt32 exp);
    void decStateExp(UInt32 exp);

    void addAttr(GData::AttrExtra& ae);

    float getPracticeAddOn();
    float getPracticeFactor();
    float getPracticeLevUpFactor();

    UInt8 getExtraAura();
    bool practiceLevelUp(UInt32& pexp);
    SoulSkill* getSoulSkill(UInt8 idx);
    UInt32 setSoulSkill(UInt8 idx, SoulSkill ss, bool writeDB = false);
    UInt32 setSoulSkill(UInt8 idx, UInt16 skillId, bool writeDB = false);

    float getSoulSkillValue(UInt8 id);
    void skillData2String(std::string& str);

    static UInt16 getSkillIdOfItem(UInt32 itemId);
    UInt8 getSoulColor()
    {
        switch(m_stateLevel)
        {
        case 1:
        case 2:
            return 2;
        case 3:
        case 4:
            return 3;
        case 5:
        case 6:
            return 4;
        case 7:
        case 8:
            return 5;
        case 9:
        case 10:
            return 6;
        default:
            return 2;
        }
    }

    Int16 getSoulMax()
    {
        Int16 soul[] = {0, 10, 20, 30, 40, 60,80, 100, 120, 160, 210};
        int idx = (m_stateLevel > 10 ? 10 : m_stateLevel);
        return soul[idx];
    }

    void sendInfo(Player* pl);
    void sendSoulSkill(Player* pl);

private:
    Fighter* m_fgt;
    UInt8 m_cls;
    UInt8 m_practiceLevel;   // 元神修为等级
    UInt32 m_stateExp;       // 元神强度
    UInt8 m_stateLevel;      // 元神境界
    float m_strength;
    float m_agility;
    float m_physique;
    float m_intelligence;
    float m_will;

    std::vector<SoulSkill> m_skills;
};

}
#endif

#include "SecondSoul.h"
#include "GData/SoulSkillTable.h"
#include "GData/SoulExpTable.h"
#include "Script/BattleFormula.h"
#include "Fighter.h"
#include "Player.h"
#include "Package.h"
#include "MsgID.h"

namespace GObject
{

SecondSoul::SecondSoul(Fighter* fgt, UInt8 cls, UInt8 practiceLevel, UInt32 stateExp, UInt8 stateLevel, UInt8 xinxiu)
    : m_fgt(fgt), m_cls(cls), m_practiceLevel(practiceLevel), m_stateExp(stateExp), m_stateLevel(stateLevel), m_xinxiu(xinxiu),
        m_strength(0), m_agility(0), m_physique(0), m_intelligence(0), m_will(0),
        m_xinxiu_attack(0), m_xinxiu_action(0), m_xinxiu_defend(0), m_xinxiu_hp(0), m_skill_num1(0), m_skill_num2(0),
        m_dirty(true)
{
}

void SecondSoul::addAttr(GData::AttrExtra& ae)
{
    float attrTransFactor = getSoulSkillValue(SOUL_SKILL_ATTRTRANS);
    float strenghtFactor = getSoulSkillValue(SOUL_SKILL_STRENGHT);
    float intelligenceFactor = getSoulSkillValue(SOUL_SKILL_INTELLIGENCE);
    float physiqueFactor = getSoulSkillValue(SOUL_SKILL_PHYSIQUE);
    float agilityFactor = getSoulSkillValue(SOUL_SKILL_AGILITY);
    float willFactor = getSoulSkillValue(SOUL_SKILL_WILL);
    //float allFactor = getSoulSkillValue(SOUL_SKILL_ALLATTR);
    //float hpFactor = getSoulSkillValue(SOUL_SKILL_HPFACTOR);

    if(m_dirty)
    {
        m_strength = Script::BattleFormula::getCurrent()->calcSoulStrenght(this);
        m_agility = Script::BattleFormula::getCurrent()->calcSoulAgility(this);
        m_physique = Script::BattleFormula::getCurrent()->calcSoulPhysique(this);
        m_intelligence = Script::BattleFormula::getCurrent()->calcSoulIntelligence(this);
        m_will = Script::BattleFormula::getCurrent()->calcSoulWill(this);
        m_xinxiu_attack = Script::BattleFormula::getCurrent()->calcSoulXinxiuAttack(this);
        m_xinxiu_action = Script::BattleFormula::getCurrent()->calcSoulXinxiuAction(this);
        m_xinxiu_defend = Script::BattleFormula::getCurrent()->calcSoulXinxiuDefend(this);
        m_xinxiu_hp = Script::BattleFormula::getCurrent()->calcSoulXinxiuHp(this);
        m_dirty = false;
    }

    //ae.hpP += hpFactor;
    ae.strength += m_strength * (1 + strenghtFactor + attrTransFactor);
    ae.agility += m_agility * (1 + agilityFactor + attrTransFactor);
    ae.physique += m_physique * (1 + physiqueFactor + attrTransFactor);
    ae.intelligence += m_intelligence * (1 + intelligenceFactor + attrTransFactor);
    ae.will += m_will * (1 + willFactor + attrTransFactor);
    ae.attack += m_xinxiu_attack;
    ae.magatk += m_xinxiu_attack;
    ae.action += m_xinxiu_action;
    ae.defend += m_xinxiu_defend;
    ae.magdef += m_xinxiu_defend;
    ae.hp += m_xinxiu_hp;
    ae.auraMax += getSoulSkillValue(SOUL_SKILL_AURAMAX);
}

float SecondSoul::getPracticeAddOn()
{
    return getSoulSkillValue(SOUL_SKILL_ACUADDON);
}

float SecondSoul::getPracticeFactor()
{
    return getSoulSkillValue(SOUL_SKILL_ACUFACTOR);
}

float SecondSoul::getPracticeLevUpFactor()
{
    return 0;  // 这个技能暂时废除了
   // return getSoulSkillValue(SOUL_SKILL_PRACLVLUP);
}

UInt8 SecondSoul::getExtraAura()
{
    return static_cast<UInt8>(getSoulSkillValue(SOUL_SKILL_AURAEXTRA));
}

UInt8 SecondSoul::getAuraLeft()
{
    return static_cast<UInt8>(getSoulSkillValue(SOUL_SKILL_TALENT));
}

float SecondSoul::getSoulSkillValue(UInt8 id)
{
    int size = m_skills.size();
    for(int i = 0; i < size; ++ i)
    {
        if(m_skills[i].id == id)
        {
            return GData::soulSkillTable[m_skills[i].id][m_skills[i].level].value;
        }
    }

    return 0;
}

bool SecondSoul::practiceLevelUp(UInt32& pexp)
{
    if(m_practiceLevel >= m_fgt->getLevel())
        return false;

    float factor = 1 - getPracticeLevUpFactor();
    if(factor < 0)
        factor = 1;
    UInt32 formalPexp = pexp;
    if(GData::soulExpTable.testPracticeLevelUp(m_practiceLevel, pexp, factor))
    {
        if (pexp < formalPexp)
        {
            m_fgt->getOwner()->secondSoulUdpLog(1093);
            m_fgt->getOwner()->secondSoulUdpLog(1095, 0, formalPexp - pexp);
            m_fgt->getOwner()->secondSoulUdpLog(1094, m_practiceLevel);
        }
        setDirty(true);
        m_fgt->setDirty(true);
        DB2().PushUpdateData("UPDATE `second_soul` SET `practiceLevel` = %u WHERE `fighterId` = %u AND `playerId` = %"I64_FMT"u", m_practiceLevel, m_fgt->getId(), m_fgt->getOwner()->getId());
        return true;
    }

    return false;
}

void SecondSoul::decStateExp(UInt32 exp)
{
    if(m_stateExp == GData::soulExpTable.getLevelMin(m_stateLevel))
        return;

    m_stateExp -= exp;
    if(m_stateExp < GData::soulExpTable.getLevelMin(m_stateLevel))
        m_stateExp = GData::soulExpTable.getLevelMin(m_stateLevel);

    DB2().PushUpdateData("UPDATE `second_soul` SET `stateExp` = %u WHERE `fighterId` = %u AND `playerId` = %"I64_FMT"u", m_stateExp, m_fgt->getId(), m_fgt->getOwner()->getId());
}

void SecondSoul::addStateExp(UInt32 exp)
{
    m_stateExp += exp;
    if(GData::soulExpTable.testLevelUp(m_stateLevel, m_stateExp))
    {
        setDirty(true);
        m_fgt->setDirty(true);
        m_fgt->sendMaxSoul();
    }

    DB2().PushUpdateData("UPDATE `second_soul` SET `stateLevel` = %u, `stateExp` = %u WHERE `fighterId` = %u AND `playerId` = %"I64_FMT"u", m_stateLevel, m_stateExp, m_fgt->getId(), m_fgt->getOwner()->getId());
}

SoulSkill* SecondSoul::getSoulSkill(UInt8 idx)
{
    int size = m_skills.size();
    if(idx >= size)
        return NULL;
    return &(m_skills[idx]);
}

UInt32 SecondSoul::setSoulSkill(UInt8 idx, SoulSkill ss, bool writeDB)
{
    int size = m_skills.size();
    if(idx >= MAX_SKILL_NUM)
        return 0xFFFFFFFF;

    if(ss.id != 0)
    {
        for(int i = 0; i < size; ++ i)
        {
            if(m_skills[i].id == ss.id && idx != i)
            {
                return 0xFFFFFFFF;
            }
        }
    }

    UInt32 retItemId = 0;
    if(idx >= size)
    {
        if(ss.id !=0)
            m_skills.push_back(ss);
    }
    else
    {
        retItemId = GData::soulSkillTable[m_skills[idx].id][m_skills[idx].level].itemId;

        if(ss.id !=0)
        {
            m_skills[idx].id = ss.id;
            m_skills[idx].level = ss.level;
        }
        else
        {
            m_skills.erase(m_skills.begin() + idx);
        }
    }

    if(writeDB)
    {
        std::string str;
        vector2string(m_skills, m_skills.size(), str);
        DB2().PushUpdateData("UPDATE `second_soul` SET `skills` = '%s' WHERE `fighterId` = %u AND `playerId` = %"I64_FMT"u", str.c_str(), m_fgt->getId(), m_fgt->getOwner()->getId());
    }

    switch(ss.id)
    {
    case 0:
    case SOUL_SKILL_ATTRTRANS:
    case SOUL_SKILL_STRENGHT:
    case SOUL_SKILL_INTELLIGENCE:
    case SOUL_SKILL_PHYSIQUE:
    case SOUL_SKILL_AGILITY:
    case SOUL_SKILL_WILL:
    //case SOUL_SKILL_ALLATTR:
    case SOUL_SKILL_HPFACTOR:
        setDirty(true);
        m_fgt->setDirty(true);
        break;
    default:
        break;
    }

    return retItemId;
}

UInt32 SecondSoul::setSoulSkill(UInt8 idx, UInt16 skillId, bool writeDB)
{
    UInt32 itemId = 0;
    SoulSkill ss;

    ss.id = skillId >> 8;
    ss.level = skillId & 0xFF;

    itemId = setSoulSkill(idx, ss, writeDB);

    if(ss.id != 0 && itemId == 0)
    {
        UInt32 tmpItemId = GData::soulSkillTable[ss.id][ss.level].itemId;
        const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[tmpItemId];
        if(itemType->subClass == Item_SL1)
            ++ m_skill_num1;
        else
            ++ m_skill_num2;
    }
    else if( ss.id == 0 && itemId != 0)
    {
        const GData::ItemBaseType* itemType = GData::itemBaseTypeManager[itemId];
        if(itemType->subClass == Item_SL1)
            -- m_skill_num1;
        else
            -- m_skill_num2;
    }

    if(m_skill_num1 < 0)
        m_skill_num1 = 0;

    if(m_skill_num2 < 0)
        m_skill_num2 = 0;

    return itemId;
}

UInt16 SecondSoul::getSkillIdOfItem(UInt32 itemId)
{
    UInt16 skillId = 0;
    int size1 = GData::soulSkillTable.size();
    for(int i = 1; i < size1; ++ i)
    {
        int size2 = GData::soulSkillTable[i].size();
        for(int j = 1; j < size2; ++ j)
        {
            if(itemId == GData::soulSkillTable[i][j].itemId)
            {
                skillId = (i << 8) + j;
                break;
            }
        }
    }

    return skillId;
}

void SecondSoul::insertIntoDB()
{
    std::string str;
    vector2string(m_skills, m_skills.size(), str);
    DB2().PushUpdateData("INSERT INTO `second_soul` (`fighterId`, `playerId`, `cls`, `xinxiu`, `practiceLevel`, `stateLevel`, `stateExp`, `skills`) VALUES (%u, %"I64_FMT"u, %u, 0, 1, 1, 0, '%s')", m_fgt->getId(), m_fgt->getOwner()->getId(), m_cls, str.c_str());
}

void SecondSoul::sendInfo(Player* pl)
{
    Stream st(REP::SECOND_SOUL);
    st << static_cast<UInt8>(0);
    st << static_cast<UInt16>(m_fgt->getId()) << m_cls << m_xinxiu << m_practiceLevel << m_stateExp;

    UInt8 cnt = m_skills.size();
    st << cnt;
    for(int i = 0; i < cnt; ++ i)
    {
        st << m_skills[i].id << m_skills[i].level;
    }

    st << Stream::eos;
    pl->send(st);
}

void SecondSoul::sendSoulSkill(Player* pl)
{
    Stream st(REP::SECOND_SOUL);
    st << static_cast<UInt8>(5);
    st << static_cast<UInt16>(m_fgt->getId());

    UInt8 cnt = m_skills.size();
    st << cnt;
    for(int i = 0; i < cnt; ++ i)
    {
        st << m_skills[i].id << m_skills[i].level;
    }

    st << Stream::eos;
    pl->send(st);
}

bool SecondSoul::setClass(UInt8 cls)
{
    if(cls == 0 || cls == m_cls || cls > 12)
    {
        m_fgt->getOwner()->sendMsgCode(0, 1073);
        return false;
    }

    m_cls = cls;
    DB2().PushUpdateData("UPDATE `second_soul` SET `cls` = %d WHERE `fighterId` = %u AND `playerId` = %"I64_FMT"u", m_cls, m_fgt->getId(), m_fgt->getOwner()->getId());

    sendInfo(m_fgt->getOwner());
    setDirty(true);
    m_fgt->setDirty(true);
    m_fgt->getOwner()->sendMsgCode(0, 1074);

    return true;
}

UInt8 SecondSoul::getSoulSkillIdx(SoulSkill ss)
{
    int size = m_skills.size();

    if(ss.id != 0)
    {
        for(int i = 0; i < size; ++ i)
        {
            if(m_skills[i].id == ss.id && ss.level == m_skills[i].level)
            {
                return i;
            }
        }
    }

    return 0xFF;
}

UInt8 SecondSoul::getSoulSkillIdx(UInt16 skillId)
{
    SoulSkill ss;

    ss.id = skillId >> 8;
    ss.level = skillId & 0xFF;

    return getSoulSkillIdx(ss);
}

bool SecondSoul::setXinxiu(UInt8 xinxiu)
{
    if(xinxiu == 0 || xinxiu == m_xinxiu || xinxiu > 4)
    {
        m_fgt->getOwner()->sendMsgCode(0, 1079);
        return false;
    }

    m_xinxiu = xinxiu;
    DB2().PushUpdateData("UPDATE `second_soul` SET `xinxiu` = %d WHERE `fighterId` = %u AND `playerId` = %"I64_FMT"u", m_xinxiu, m_fgt->getId(), m_fgt->getOwner()->getId());

    sendInfo(m_fgt->getOwner());
    setDirty(true);
    m_fgt->setDirty(true);
    m_fgt->getOwner()->sendMsgCode(0, 1078);
    return true;
}

}




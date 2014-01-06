#include "SevenSoul.h"
#include "Common/StringTokenizer.h"

namespace GData
{
SevenSoul sevenSoul;

SevenSoul::SevenSoul()
{
    memset(_needSoulNum, 0, sizeof(_needSoulNum));
    memset(_skillLevel, 0, sizeof(_skillLevel));
    memset(_skillId, 0, sizeof(_skillId));
    memset(_condtionValue, 0, sizeof(_condtionValue));
}

SevenSoul::~SevenSoul()
{
}

void SevenSoul::loadSevenSoulLevel(UInt8 soulLevel, UInt32 needSoulNum, UInt8 skillLevel)
{
    if(soulLevel == 0 || soulLevel > SEVEN_SOUL_LEVEL_MAX)
        return;
    if(needSoulNum == 0)
        return;
    if(skillLevel == 0 || skillLevel > 9)
        return;
    _needSoulNum[soulLevel - 1] = needSoulNum;
    _skillLevel[soulLevel - 1] = skillLevel;
}

void SevenSoul::loadSevenSoulUpgrade(UInt8 pettype, UInt8 soulid, std::string skillstr, UInt8 condionvalue)
{
    if(pettype == 0 || pettype > PET_TYPE_MAX)
        return;
    if(soulid == 0 || soulid > SOUL_ID_MAX)
        return;
    StringTokenizer sk(skillstr, ",");
    if(sk.count() < 2)
        return;
    UInt32 index = (pettype - 1) * SOUL_ID_MAX + soulid - 1;
    _skillId[index][0] = atoi(sk[0].c_str());
    _skillId[index][1] = atoi(sk[1].c_str());
    _condtionValue[index] = condionvalue;
}

UInt32 SevenSoul::getNeedSoulNum(UInt8 level)
{
    if(level == 0)
        return 0;
    else if(level < SEVEN_SOUL_LEVEL_MAX)
        return _needSoulNum[level - 1];
    else
        return 0xFFFFFFFF;
}

UInt8 SevenSoul::getSkillLevel(UInt8 level)
{
    if(level == 0 || level > SEVEN_SOUL_LEVEL_MAX)
        return 0;
    else
        return _skillLevel[level - 1];
}

UInt16 SevenSoul::getSkillId(UInt32 petType, UInt8 soulId, UInt8 order)
{
    if(petType == 0 || petType > 4)
        return 0;
    if(soulId == 0 || soulId > 7)
        return 0;
    if(order == 0 || order > 2)
        return 0;
    UInt32 index = (petType - 1) * SOUL_ID_MAX + soulId - 1;
    return _skillId[index][order - 1];
}

UInt32 SevenSoul::getConditonValue(UInt8 soulId)
{
    if(soulId == 0)
        return 0;
    else if(soulId <= SOUL_ID_MAX)
        return _condtionValue[soulId - 1];
    else
        return 0xFFFFFFFF;
}

}


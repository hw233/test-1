#ifndef _SEVENSOUL_H_
#define _SEVENSOUL_H_

#include "Config.h"
#include "GDataDBExecHelper.h"

namespace GData
{
//七魄等级
#define SEVEN_SOUL_LEVEL_MAX 25
//宠物类型
#define PET_TYPE_MAX 25
//七魄个数
#define SOUL_ID_MAX  7
#define TYPE_SOUL_ID_MAX (PET_TYPE_MAX * SOUL_ID_MAX)
//每个七魄2种技能
#define SKILL_MAX_PER_SOUL 2

class SevenSoul
{
    public:
        SevenSoul();
        ~SevenSoul();
        void loadSevenSoulLevel(UInt8 soulLevel, UInt32 needSoulNum, UInt8 skillLevel);
        void loadSevenSoulUpgrade(UInt8 pettype, UInt8 soulid, std::string skillstr, UInt8 condionvalue);
        UInt32 getNeedSoulNum(UInt8 level);
        UInt8 getSkillLevel(UInt8 level);
        UInt16 getSkillId(UInt32 petType, UInt8 soulId, UInt8 order);
        UInt32 getConditonValue(UInt8 soulId);

    private:
        UInt32 _needSoulNum[SEVEN_SOUL_LEVEL_MAX];
        UInt8 _skillLevel[SEVEN_SOUL_LEVEL_MAX];

        UInt16 _skillId[TYPE_SOUL_ID_MAX][SKILL_MAX_PER_SOUL];
        UInt32 _condtionValue[TYPE_SOUL_ID_MAX];
};

extern SevenSoul sevenSoul;
}

#endif //_SEVENSOUL_H_


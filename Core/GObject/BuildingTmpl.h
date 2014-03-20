#ifndef BUILDING_TMPL_H
#define BUILDING_TMPL_H

#include "Config.h"

namespace GObject
{
    enum LevelUpType
    {
        eLevelUpDefault = 0,    // 默认升级方式，经验值离散增加，超过最大值不增加
        eLevelUpByLevel = 1,    // 只能根据等级一下子升级, 超过最大值不增加
        eLevelUpByExp = 2,      // 经验值离散增加，超过最大值不增加
    };

    enum BuildingTmplErrCode
    {
        eErrSuccess = 0,        // 成功，没有错误 
        eErrUnknown = 1,        // 未知错误 
        eErrNoEnoughExp = 2,    // 所需经验值不足
        eErrMaxLevel = 3,       // 已经是最高等级，无法升级
    };

    class BuildingTmpl
    {
        public:
            BuildingTmpl();
            virtual ~BuildingTmpl();
        public:
            virtual void loadFromDB(UInt32 exp, UInt16 level, UInt32 updateTime);

            UInt16 getLevel() const;
            UInt32 getExp() const;
            UInt32 getShownExp() const;
            UInt32 getShownNextExp() const;
            UInt32 getUpdateTime() const;

            virtual UInt32 tryLevelUp(UInt32& uVal, bool writeDB = true);

            virtual UInt32 getTableLevelExp(UInt16 level) const = 0;
            virtual UInt16 getTableLevel(UInt32 exp) const = 0;
            virtual UInt16 getMaxLevel() const = 0;

            virtual bool saveToDB() const = 0;

        public:
            void setExp(UInt32 exp, bool writeDB = true);
            void setLevel(UInt16 level, bool writeDB = true);

        private:
            UInt32 levelUpByLevel(UInt32& uVal);
            UInt32 levelUpByExp(UInt32& uVal);
            UInt32 levelUpDefault(UInt32& uVal);

        protected:
            LevelUpType _levelType;
            
        private:
            UInt16 _level;
            UInt32 _exp;
            UInt32 _updateTime;
    };

}
#endif  //#ifndef BUILDING_TMPL_H


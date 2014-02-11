#include "BuildingTmpl.h"

#include "Log/Log.h"
#include "Common/TimeUtil.h"

namespace GObject
{
    BuildingTmpl::BuildingTmpl()
        :_levelType(eLevelUpDefault), _level(0), _exp(0), _updateTime(0)
    {
    }

    BuildingTmpl::~BuildingTmpl()
    {
    }


    void BuildingTmpl::loadFromDB(UInt32 exp, UInt16 level, UInt32 updateTime)
    {
        // 从数据库读取时更新建筑经验值
        _exp = exp;
        _level = level;
        _updateTime = updateTime;
        if (_level != getTableLevel(_exp))
        {
            WARN_LOG("Building exp error. level = %u, exp = %u.", _level, _exp);
            _level = getTableLevel(_exp);
        }
    }

    UInt32 BuildingTmpl::tryLevelUp(UInt32& uVal, bool writeDB /* = true */)
    {
        // 建筑升级
        UInt32 ret = eErrUnknown;
        switch(_levelType)
        {
            case eLevelUpByLevel:
                ret = levelUpByLevel(uVal);
            case eLevelUpByExp:
                ret = levelUpByExp(uVal);
            case eLevelUpDefault:
            default:
                ret = levelUpDefault(uVal);
        }
        if (ret == eErrSuccess && writeDB)
            saveToDB();
        return ret;
    }

    UInt16 BuildingTmpl::getLevel() const
    {
        return _level;
    }

    UInt32 BuildingTmpl::getExp() const
    {
        return _exp;
    }

    UInt32 BuildingTmpl::getUpdateTime() const
    {
        return _updateTime;
    }

    void BuildingTmpl::setExp(UInt32 exp, bool writeDB /* = true */)
    {
        // 设定经验值
        _exp = exp;
        _level = getTableLevel(_exp);
        _updateTime = TimeUtil::Now();
        if (writeDB)
            saveToDB();
    }

    void BuildingTmpl::setLevel(UInt16 level, bool writeDB /* = true */)
    {
        // 设定等级
        if (level > getMaxLevel())
            level = getMaxLevel();
        _level = level;
        _exp = getTableLevelExp(_level);
        _updateTime = TimeUtil::Now();
        if (writeDB)
            saveToDB();
    }


    UInt32 BuildingTmpl::getShownExp() const
    {
        // 返回客户端实际显示的exp
        return _exp - getTableLevelExp(_level);
    }

    UInt32 BuildingTmpl::getShownNextExp() const
    {
        // 返回客户端实际显示需要的下一级的exp
        return getTableLevelExp(_level + 1) - getTableLevelExp(_level);
    }

    UInt32 BuildingTmpl::levelUpByLevel(UInt32& uVal)
    {
        // 只能根据等级一下子升级, 超过最大值不增加
        UInt32 uMaxExp = getTableLevelExp(getMaxLevel());
        if (uMaxExp <= _exp)
            return eErrMaxLevel;
        UInt32 uNeedVal = getTableLevelExp(_level + 1) - getTableLevelExp(_level);
        if (!uNeedVal) // 需要升级的经验值为0，应该是已经在最大等级，无法升级
            return eErrMaxLevel;
        if (uNeedVal > uVal)
            return eErrNoEnoughExp;
        _exp += uNeedVal;
        uVal -= uNeedVal;
        ++_level;
        _updateTime = TimeUtil::Now();
        return eErrSuccess;
    }

    UInt32 BuildingTmpl::levelUpByExp(UInt32& uVal)
    {
        // 经验值离散增加，超过最大值不增加
        UInt32 uMaxExp = getTableLevelExp(getMaxLevel());
        if (uMaxExp <= _exp)
            return eErrMaxLevel;
        UInt32 uNeedVal = getTableLevelExp(_level + 1) - getTableLevelExp(_level);
        if (!uNeedVal) // 需要升级的经验值为1，应该是已经在最大等级，无法升级
            return eErrMaxLevel;
        UInt16 oldLevel = _level;
        _exp += uVal;
        _level = getTableLevelExp(_exp);
        _updateTime = TimeUtil::Now();
        if (_level == oldLevel)
            return eErrSuccess;
        else
            return eErrSuccess;
    }

    UInt32 BuildingTmpl::levelUpDefault(UInt32& uVal)
    {
        // TODO: 默认升级方式
        return eErrUnknown;
    }
}

#include "Config.h"

#include "Common/Stream.h"
#include "Player.h"
#include "Clan.h"

#include "ClanStatue.h"
#include "GData/ClanStatueTable.h"
#include "GData/ClanCopyTable.h"

namespace GObject
{

ClanStatue::ClanStatue(Clan *c) : _level(1), _clan(c)
{
    _exp = 0;
}

ClanStatue::~ClanStatue()
{
}


void ClanStatue::updateLevel(UInt32 exp, UInt32 expUpdateTime, UInt16 maxLevel)
{
    // 根据现有exp更新等级（FIXME: 重置副本的时间段经验结算）
    UInt16 formalLevel = _level;
    UInt16 copyLevel = _clan->getCopyLevel();
    UInt32 expOutput = GData::clanCopyTable[copyLevel].expOutput;
    UInt32 expConsume = GData::clanStatueTable[_level].consumeExp;
    UInt32 now = TimeUtil::Now();
    Int64  bigExp = 0;

    if (expUpdateTime)
    {
        // 从数据库读取时更新神像经验值
        if (now < expUpdateTime)
        {
            // 被穿越了?
            bigExp = exp - (expOutput - expConsume ) * (expUpdateTime / 3600 - now / 3600);
        }
        else
        {
            bigExp = exp + (static_cast<Int64>(expOutput) - expConsume ) * (now / 3600 - expUpdateTime / 3600);
        }
    }
    else
    {
        bigExp = static_cast<Int64>(_exp) + expOutput - expConsume;
    }
    if (bigExp < 0)
        _exp = 0;
    else
        _exp = static_cast<UInt32>(bigExp);
    _level = 0;

    for (std::vector<GData::ClanStatueTableData>::iterator it = GData::clanStatueTable.begin(); 
            it != GData::clanStatueTable.end(); ++it)
    {
        if (_exp < it->needExp)
        {
            _level = it->level - 1;
            if (!_level)
                ++ _level;
            if (_level > maxLevel)
            {
                _level = maxLevel;
                _exp = GData::clanStatueTable[_level + 1].needExp;
                if (_exp)
                    --_exp;
            }
            DB5().PushUpdateData("REPLACE INTO `clan_statue` (`clanId`, `exp`, `level`, `expUpdateTime`) VALUES (%u,%u,%u,%u)", _clan->getId(), _exp, _level, now);
            if (formalLevel != _level)
            {
                _clan->notifyUpdateStatueAttr();
            }
            return;
        }
    }
    _level = GData::clanStatueTable.size() - 2;
    if (_level > maxLevel)
    {
        _level = maxLevel;
        _exp = GData::clanStatueTable[_level + 1].needExp;
        if (_exp)
            --_exp;
    }
    DB5().PushUpdateData("REPLACE INTO `clan_statue` (`clanId`, `exp`, `level`, `expUpdateTime`) VALUES (%u,%u,%u,%u)", _clan->getId(), _exp, _level, now);
    if (formalLevel != _level)
    {
        _clan->notifyUpdateStatueAttr();
    }
}

void ClanStatue::addExp(UInt32 exp, UInt16 maxLevel)
{
    UInt16 formalLevel = _level;
    _exp += exp;
    _level = 0;
    for (std::vector<GData::ClanStatueTableData>::iterator it = GData::clanStatueTable.begin(); 
            it != GData::clanStatueTable.end(); ++it)
    {
        if (_exp < it->needExp)
        {
            _level = it->level - 1;
            if (!_level)
                ++ _level;
            if (_level > maxLevel)
            {
                _level = maxLevel;
                _exp = GData::clanStatueTable[_level + 1].needExp;
                if (_exp)
                    --_exp;
            }
            DB5().PushUpdateData("REPLACE INTO `clan_statue` (`clanId`, `exp`, `level`, `expUpdateTime`) VALUES (%u,%u,%u,%u)", _clan->getId(), _exp, _level, TimeUtil::Now());
            if (formalLevel != _level)
            {
                _clan->notifyUpdateStatueAttr();
            }
            return;
        }
    }
    _level = GData::clanStatueTable.size() - 2;
    if (_level > maxLevel)
    {
        _level = maxLevel;
        _exp = GData::clanStatueTable[_level + 1].needExp;
        if (_exp)
            --_exp;
    }
    DB5().PushUpdateData("REPLACE INTO `clan_statue` (`clanId`, `exp`, `level`, `expUpdateTime`) VALUES (%u,%u,%u,%u)", _clan->getId(), _exp, _level, TimeUtil::Now());
    if (formalLevel != _level)
    {
        _clan->notifyUpdateStatueAttr();
    }
}

void ClanStatue::subExp(UInt32 exp, UInt16 maxLevel)
{
    UInt16 formalLevel = _level;
    if (_exp < exp)
        _exp = 0;
    else
        _exp -= exp;
    _level = 0;
    for (std::vector<GData::ClanStatueTableData>::iterator it = GData::clanStatueTable.begin(); 
            it != GData::clanStatueTable.end(); ++it)
    {
        if (_exp < it->needExp)
        {
            _level = it->level - 1;
            if (!_level)
                ++ _level;
            if (_level > maxLevel)
            {
                _level = maxLevel;
                _exp = GData::clanStatueTable[_level + 1].needExp;
                if (_exp)
                    --exp;
            }
            DB5().PushUpdateData("REPLACE INTO `clan_statue` (`clanId`, `exp`, `level`, `expUpdateTime`) VALUES (%u,%u,%u,%u)", _clan->getId(), _exp, _level, TimeUtil::Now());
            if (formalLevel != _level)
            {
                _clan->notifyUpdateStatueAttr();
            }
            return;
        }
    }
    _level = GData::clanStatueTable.size() - 1;
    if (_level > maxLevel)
    {
        _level = maxLevel;
        _exp = GData::clanStatueTable[_level + 1].needExp;
        if (_exp)
            --exp;
    }
    DB5().PushUpdateData("REPLACE INTO `clan_statue` (`clanId`, `exp`, `level`, `expUpdateTime`) VALUES (%u,%u,%u,%u)", _clan->getId(), _exp, _level, TimeUtil::Now(), _clan->getId());
    if (formalLevel != _level)
    {
        _clan->notifyUpdateStatueAttr();
    }
}

UInt16 ClanStatue::getLevel()
{
    return _level;
}

UInt32 ClanStatue::getExp()
{
    return _exp;
}

UInt32 ClanStatue::getShownExp()
{
    // 返回客户端实际显示的exp
    if (_level <= 1)
        return _exp;
    else
    {
        return _exp - GData::clanStatueTable[_level].needExp;
    }
}

UInt32 ClanStatue::getShownNextExp()
{
    // 返回客户端实际显示需要的下一级的exp
    
    if (!_level)
        return GData::clanStatueTable[2].needExp;
    else
    {
        UInt32 maxLevel = GData::clanStatueTable.size() - 2;
        if (_level < maxLevel)
            return GData::clanStatueTable[_level + 1].needExp - GData::clanStatueTable[_level].needExp;
        else 
            return 0;
    }
}

}

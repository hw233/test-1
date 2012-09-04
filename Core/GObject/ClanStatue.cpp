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
}

ClanStatue::~ClanStatue()
{
}


void ClanStatue::updateLevel(UInt32 exp, UInt32 expUpdateTime)
{
    // 根据现有exp更新等级（FIXME: 重置副本的时间段经验结算）
    UInt16 copyLevel = _clan->getCopyLevel();
    UInt32 expOutput = GData::clanCopyTable[copyLevel].expOutput;
    UInt32 expConsume = GData::clanStatueTable[_level].consumeExp;
    UInt32 now = TimeUtil::Now();
    if (now < expUpdateTime)
    {
        if (expConsume > expOutput)
        {
            _exp = exp + (expConsume - expOutput) * (expUpdateTime / 3600 - now / 3600);
        }
        else
        {
            _exp = exp - (expOutput - expConsume ) * (expUpdateTime / 3600 - now / 3600);
        }
    }
    else
    {
        if (expConsume > expOutput)
        {
            _exp = exp - (expConsume - expOutput) * (now / 3600 - expUpdateTime / 3600);
        }
        else
        {
            _exp = exp + (expOutput - expConsume ) * (now / 3600 - expUpdateTime / 3600);
        }
    }
    _level = 0;
    for (std::vector<GData::ClanStatueTableData>::iterator it = GData::clanStatueTable.begin(); 
            it != GData::clanStatueTable.end(); ++it)
    {
        if (_exp < it->needExp)
        {
            _level = it->level;
            return;
        }
    }
    if (!_level)
        ++ _level;
}

UInt16 ClanStatue::getLevel()
{
    return _level;
}

UInt32 ClanStatue::getExp()
{
    return _exp;
}

}

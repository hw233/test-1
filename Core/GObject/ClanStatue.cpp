#include "Config.h"

#include "Common/Stream.h"
#include "Player.h"
#include "Clan.h"

#include "ClanStatue.h"
#include "GData/ClanStatueTable.h"

namespace GObject
{

ClanStatue::ClanStatue(Clan *c) : _level(1), _clan(c)
{
}

ClanStatue::~ClanStatue()
{
}


void ClanStatue::updateLevel(UInt32 exp)
{
    // 根据现有exp更新等级
    _exp = exp;
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

#include "Config.h"
#include "ClanLvlTable.h"
#include "Server/ServerTypes.h"

namespace GData
{

ClanLvl clanLvlTable;

ClanLvl::ClanLvl()
{
	memset(_exp, 0, sizeof(_exp));
}

bool ClanLvl::testLevelUp( UInt8& lvl, UInt64& exp )
{
	bool r = false;
    while(_exp[lvl-1] > 0 && exp < _exp[lvl-1] && lvl > 0)
    {
        -- lvl;
    }
    if(lvl < 1)
    {
        lvl = 1;
    }

	while(_exp[lvl-1] > 0 && exp >= _exp[lvl-1] && lvl <= CLAN_LEVEL_MAX)
	{
		r = true;
		++ lvl;
	}
	if(lvl >= CLAN_LEVEL_MAX)
    {
        lvl = CLAN_LEVEL_MAX;
		exp = _exp[CLAN_LEVEL_MAX - 1];
    }
	return r;
}

void ClanLvl::setTable( UInt8 lvl, UInt64 exp, UInt32 pkgsize, UInt32 weal )
{
    if (lvl > CLAN_LEVEL_MAX)
        return;
	_exp[lvl-1] = exp;
    _pkgsize[lvl - 1] = pkgsize;
    _weal[lvl - 1] = weal;
}

UInt64 ClanLvl::getLevelMin( UInt8 lvl )
{
	if(lvl < 2)
		return 0;
	if(lvl >= CLAN_LEVEL_MAX)
		return _exp[CLAN_LEVEL_MAX - 1];
	return _exp[lvl - 1];
}

UInt32 ClanLvl::getWeal(UInt8 lvl)
{
    if(lvl > CLAN_LEVEL_MAX || lvl == 0) return 0;
    return _weal[lvl - 1];
}

UInt32 ClanLvl::getPkgSize(UInt8 lvl)
{
    if(lvl > CLAN_LEVEL_MAX || lvl == 0) return 0;
    return _pkgsize[lvl - 1];
}

}


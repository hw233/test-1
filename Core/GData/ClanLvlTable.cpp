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
	while(_exp[lvl] > 0 && exp >= _exp[lvl])
	{
		r = true;
		++ lvl;
	}
	if(lvl == LEVEL_MAX)
		exp = _exp[LEVEL_MAX - 1];
	return r;
}

void ClanLvl::setTable( UInt8 lvl, UInt64 exp )
{
	_exp[lvl] = exp;
}

UInt64 ClanLvl::getLevelMin( UInt8 lvl )
{
	if(lvl < 2)
		return 0;
	if(lvl > LEVEL_MAX)
		return _exp[LEVEL_MAX - 1];
	return _exp[lvl - 1];
}

}


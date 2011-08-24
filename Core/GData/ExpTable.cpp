#include "Config.h"
#include "ExpTable.h"
#include "Server/ServerTypes.h"

namespace GData
{

ExpTable expTable;

ExpTable::ExpTable()
{
	memset(_exp, 0, sizeof(_exp));
}

bool ExpTable::testLevelUp( UInt8& lvl, UInt64& exp )
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

void ExpTable::setTable( UInt8 lvl, UInt64 exp )
{
    if (lvl >= LEVEL_MAX)
        return;
	_exp[lvl] = exp;
}

UInt64 ExpTable::getLevelMin( UInt8 lvl )
{
	if(lvl < 2)
		return 0;
	if(lvl >= LEVEL_MAX)
		return _exp[LEVEL_MAX - 1];
	return _exp[lvl - 1];
}

}

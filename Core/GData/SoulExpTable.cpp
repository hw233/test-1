#include "Config.h"
#include "SoulExpTable.h"
#include "Server/ServerTypes.h"

namespace GData
{

SoulExpTable soulExpTable;

SoulExpTable::SoulExpTable()
{
	memset(_exp, 0, sizeof(_exp));
}

bool SoulExpTable::testLevelUp( UInt8& lvl, UInt32& exp )
{
	bool r = false;
	while(_exp[lvl] > 0 && exp >= _exp[lvl])
	{
		r = true;
		++ lvl;
	}
	if(lvl == STATE_LEVEL_MAX)
		exp = _exp[STATE_LEVEL_MAX - 1];
	return r;
}

void SoulExpTable::setTable( UInt8 lvl, UInt32 exp )
{
    if (lvl >= STATE_LEVEL_MAX)
        return;
	_exp[lvl] = exp;
}

UInt32 SoulExpTable::getLevelMin( UInt8 lvl )
{
	if(lvl < 2)
		return 0;
	if(lvl >= STATE_LEVEL_MAX)
		return _exp[STATE_LEVEL_MAX - 1];
	return _exp[lvl - 1];
}

bool SoulExpTable::testPracticeLevelUp(UInt8& lvl, UInt32& pexp, float factor)
{
	bool r = false;
	if(_pexp[lvl] > 0 && pexp >= _pexp[lvl] * factor)
	{
		r = true;
        pexp -= _pexp[lvl] * factor;
		++ lvl;
	}

	return r;
}

void SoulExpTable::setPracticeTable( UInt8 lvl, UInt32 pexp )
{
    if (lvl >= LEVEL_MAX)
        return;
	_pexp[lvl] = pexp;
}

}

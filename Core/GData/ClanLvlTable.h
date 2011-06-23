#ifndef CLANLVLTABLE_H_
#define CLANLVLTABLE_H_

#include "Server/ServerTypes.h"

namespace GData
{

class ClanLvl
{
public:
	ClanLvl();
	bool testLevelUp(UInt8& lvl, UInt64& exp);
	void setTable(UInt8 lvl, UInt64 exp);
	UInt64 getLevelMin(UInt8 lvl);
private:
	UInt64 _exp[CLAN_LEVEL_MAX];
};

extern ClanLvl clanLvlTable;

}

#endif // CLANLVLTABLE_H_

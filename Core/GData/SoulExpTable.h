#ifndef _SOULEXPTABLE_H_
#define _SOULEXPTABLE_H_

#include "Config.h"
#include "Server/ServerTypes.h"

namespace GData
{

#define STATE_LEVEL_MAX 10

class SoulExpTable
{
public:
    SoulExpTable();
	bool testPracticeLevelUp(UInt8& lvl, UInt32& pexp, float factor);
	bool testLevelUp(UInt8& lvl, UInt32& exp);
	void setTable(UInt8 lvl, UInt32 exp);
	UInt32 getLevelMin(UInt8 lvl);

	void setPracticeTable(UInt8 lvl, UInt32 exp);
private:
	UInt32 _pexp[LEVEL_MAX];
	UInt32 _exp[STATE_LEVEL_MAX];
};

extern SoulExpTable soulExpTable;

}

#endif // _EXPTABLE_H_

#ifndef _EXPTABLE_H_
#define _EXPTABLE_H_

#include "Server/ServerTypes.h"

namespace GData
{

class ExpTable
{
public:
	ExpTable();
	bool testLevelUp(UInt8& lvl, UInt64& exp);
	void setTable(UInt8 lvl, UInt64 exp);
	UInt64 getLevelMin(UInt8 lvl);
    UInt8 exp2level(UInt64 exp);
private:
	UInt64 _exp[LEVEL_MAX];
};

extern ExpTable expTable;

}

#endif // _EXPTABLE_H_

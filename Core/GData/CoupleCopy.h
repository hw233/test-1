#ifndef _COUPLECOPY_H_
#define _COUPLECOPY_H_

#include "GDataDBExecHelper.h"

namespace GData
{
    
struct CoupleCopyData
{
    std::string name;
    UInt32 location;
    UInt32 monster;

    CoupleCopyData(): location(0),monster(0) {}
};

class CoupleCopy
{

public:
    void loadCopyTable(DBCoupleCopy&);
    CoupleCopyData* getCopyData(UInt8);
    
private:
    std::map<UInt8, CoupleCopyData> _coupleCopyData;
};

extern CoupleCopy cc;

}

#endif



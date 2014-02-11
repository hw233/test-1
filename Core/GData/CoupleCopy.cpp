#include "Config.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "CoupleCopy.h"

namespace GData
{

CoupleCopy cc;

void CoupleCopy::loadCopyTable(DBCoupleCopy& dbcc)
{
    if(dbcc.id<= 0)
        return;
    CoupleCopyData ccd;
    ccd.name = dbcc.name;
    ccd.location = dbcc.location;
    ccd.monster = dbcc.monster;
    
    _coupleCopyData.insert(std::make_pair(dbcc.id,ccd));          

}

CoupleCopyData* CoupleCopy::getCopyData(UInt8 id)
{
    std::map<UInt8,CoupleCopyData>::iterator it = _coupleCopyData.find(id);
    if(it != _coupleCopyData.end())
        return &(it->second);
    return NULL; 
}



}


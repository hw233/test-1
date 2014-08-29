#include "IncenseTable.h"
namespace GData
{
    IncenseTable incenseData;
    void IncenseTable::setIncenseAttrTable(UInt32 val,float value)
    { 
        _incenseAttr.insert(std::make_pair(val,value));
        if(val > incenseMax )
            incenseMax = val;
    } 
    float IncenseTable::getIncenseAttr(UInt32 val)
    { 
        std::map<UInt32 ,float>::iterator it = _incenseAttr.begin();
        float value = 0; 
        for(;it != _incenseAttr.end(); ++it)
        { 
            if( val < it->first)
                break;
            value = it->second;
        } 
        return value;
    } 
}

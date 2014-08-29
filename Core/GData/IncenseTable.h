
#ifndef INCENSETABLE_H_
#define INCENSETABLE_H_
#include <map>
#include "Config.h"

namespace GData
{
    class IncenseTable
    {
        public:
            IncenseTable():incenseMax(0){ }
            void setIncenseAttrTable(UInt32 val , float value);
            float getIncenseAttr(UInt32);
            UInt32 getIncenseMax() {return incenseMax;}
        private:
            UInt32 incenseMax;
            std::map<UInt32,float>  _incenseAttr;
    };
    extern IncenseTable incenseData;
}
#endif // INCENSETABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


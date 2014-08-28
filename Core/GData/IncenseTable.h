
#ifndef INCENSETABLE_H_
#define INCENSETABLE_H_
#include <map>
#include "Config.h"

namespace GData
{
    class IncenseTable
    {
        public:
            void setIncenseAttrTable(UInt32 val , float value);
            float getIncenseAttr(UInt32);
        private:
            std::map<UInt32,float>  _incenseAttr;
    };
    extern IncenseTable incenseData;
}
#endif // INCENSETABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


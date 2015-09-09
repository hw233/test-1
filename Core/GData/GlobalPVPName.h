
#ifndef GLOBALPVPNAME_H_
#define GLOBALPVPNAME_H_

#include "Config.h"
namespace GData
{
    class GlobalPVPName
    {
        public:
            void LoadNames(UInt16 index, std::string name)
            { 
                _names[index] = name;
            } 
            std::string GetName(UInt16 index)
            { 
                return _names[index];
            } 
        private:
            std::map<UInt16, std::string> _names;
    };
    extern GlobalPVPName globalPVPName;
}

#endif // GLOBALPVPNAME_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


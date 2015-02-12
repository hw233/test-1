
#ifndef MAP_H_
#define MAP_H_

#include "Config.h"
namespace GData
{
    class Map
    {
        public:
            void loadMapInfo(UInt8 index, std::string );
            const std::vector<std::string>& GetMapInfo(UInt8);
        private:
            std::map<UInt8,std::vector<std::string> >  map_info;
    };
    extern Map map;
}

#endif // MAP_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


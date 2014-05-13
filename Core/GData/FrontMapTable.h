
#ifndef FRONTMAPTABLE_H_
#define FRONTMAPTABLE_H_

#include "Config.h"

#include <map>
#include <vector>

namespace GData
{

struct FrontMapFighter
{
    FrontMapFighter() : count(0), fighterId(0) {}
    UInt8 count;
    UInt32 fighterId;
};

typedef std::map<UInt16, std::vector<FrontMapFighter> > FrontMapManager;
extern FrontMapManager frontMapManager;
extern FrontMapManager xjfrontMapManager;

typedef std::map<UInt8, UInt8> FrontMapMaxManager;
extern FrontMapMaxManager frontMapMaxManager;
extern FrontMapMaxManager xjfrontMapMaxManager;

} // namespace GData

#endif // FRONTMAPTABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


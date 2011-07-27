
#ifndef COPYTABLE_H_
#define COPYTABLE_H_

#include "Config.h"

#include <map>
#include <vector>

namespace GData
{

typedef std::map<UInt16, std::vector<UInt32> > CopyManager;
extern CopyManager copyManager;

} // namespace GData

#endif // COPYTABLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


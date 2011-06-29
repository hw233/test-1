#ifndef FIGHTERPROB_H_
#define FIGHTERPROB_H_

#include "Config.h"
#include "Server/ServerTypes.h"

namespace GData
{

struct FighterProb
{
    Int32 free;
    Int32 gold;
};

extern std::map<UInt32, FighterProb> fighterProb;

} // namespace GData

#endif // FIGHTERPROB_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */


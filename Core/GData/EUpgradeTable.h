#ifndef _EUPGRADE_TABLE_H_
#define _EUPGRADE_TABLE_H_

namespace GData
{
struct stUseItem
{
    UInt32 id;
    UInt32 num;
};

typedef     std::vector<stUseItem>   EUpgradeStfs;

struct stEUpgradeItem
{
    UInt32 toId;
    EUpgradeStfs stfs;
};
typedef std::map<UInt32 ,  stEUpgradeItem >  EUpgradeTable;

extern  EUpgradeTable   eUpgradeTable;
}
#endif

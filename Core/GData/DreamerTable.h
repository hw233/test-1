#ifndef _DREAMER_TABLE_
#define _DREAMER_TABLE_


namespace GData
{

// 每一层梦境的个数配置（key：类型，value：最大数量）
typedef std::map <UInt16,UInt8> GridTypeCount;

struct DreamerData
{
    std::vector<UInt8> hpConsume;               // 每层消耗的体力
    std::vector<GridTypeCount> gridTypeCount;   // 每层地图种类的最大数量
};

typedef std::vector<DreamerData> DreamerDataTable;

extern DreamerDataTable dreamerDataTable;

}

#endif

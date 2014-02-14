#ifndef CLAN_BUILDING_TABLE_H
#define CLAN_BUILDING_TABLE_H

#include "Config.h"

namespace GData
{

    const UInt8 MAX_BUILDING_TYPE = 4;
    enum BuildingType
    {
        buildingTypeUnknown = 0,
        buildingTypePhyAtk = 1,     // 物攻加成
        buildingTypeMagAtk = 2,     // 法攻加成
        buildingTypeAction = 3,     // 身法加成
        buildingTypeHP = 4,         // 生命加成
        buildingTypeOracle = 5,     // 神谕塔（限制其他建筑等级）
        buildingTypeMax
    };
    struct ClanBuildingTableData
    {
        // 帮派建筑模板参数
        ClanBuildingTableData( UInt8 type = 0, UInt16 level = 0, UInt32 needExp = 0, UInt32 value = 0)
            : type(type), level(level), needExp(needExp), value(value)
        {
        }

        ClanBuildingTableData(const ClanBuildingTableData& cbtd)
            : type(cbtd.type), level(cbtd.level), needExp(cbtd.needExp), value(cbtd.value)
        {
        }

        ClanBuildingTableData& operator= (const ClanBuildingTableData& cbtd)
        {
            type = cbtd.type;
            level = cbtd.level;
            needExp = cbtd.needExp;
            value = cbtd.value;
            return *this;
        }

        UInt8  type;        // 建筑加成类型
        UInt16 level;       // 建筑等级
        UInt32 needExp;     // 升级所需经验值
        UInt32 value;       // 建筑加成值
    };

    typedef std::vector<ClanBuildingTableData>  ClanBuildingTable;
    typedef std::vector<ClanBuildingTable>     ClanBuildingList;

    extern ClanBuildingList clanBuildingList;
}

#endif //#ifndef CLAN_BUILDING_TABLE_H


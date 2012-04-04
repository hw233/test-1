#ifndef SOULSKILLTABLE_INC
#define SOULSKILLTABLE_INC

#include <string>

namespace GData
{

struct SoulSkillTableData
{
	SoulSkillTableData(UInt16 id = 0, const std::string& name = std::string(),
            UInt8 level = 0, UInt32 itemId = 0, float value = 0)
        : id(id), name(name), level(level), itemId(itemId), value(value) {} 

    SoulSkillTableData(const SoulSkillTableData& sstd)
    {
        id = sstd.id;
        name = sstd.name;
        level = sstd.level;
        itemId = sstd.itemId;
        value = sstd.value;
    }

    SoulSkillTableData& operator= (const SoulSkillTableData& sstd)
    {
        id = sstd.id;
        name = sstd.name;
        level = sstd.level;
        itemId = sstd.itemId;
        value = sstd.value;
        return *this;
    }

	UInt16 id;
    std::string name;
	UInt8  level;
    UInt32 itemId;
    float value;
};

typedef std::vector<SoulSkillTableData>		SingleSoulSkillTable;
typedef std::vector<SingleSoulSkillTable>	SoulSkillTable;

extern SoulSkillTable soulSkillTable;

}

#endif

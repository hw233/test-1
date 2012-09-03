#ifndef CLANSKILLTABLE_INC
#define CLANSKILLTABLE_INC

#include <string>

namespace GData
{

struct ClanSkillTableData
{
	ClanSkillTableData(UInt16 id = 0, const std::string& name = std::string(),
            UInt8 level = 0, UInt32 needs = 0, UInt32 value = 0)
        : id(id), name(name), level(level), needs(needs), value(value) {}

    ClanSkillTableData(const ClanSkillTableData& cstd)
    {
        id = cstd.id;
        name = cstd.name;
        level = cstd.level;
        needs = cstd.needs;
        value = cstd.value;
    }

    ClanSkillTableData& operator= (const ClanSkillTableData& cstd)
    {
        id = cstd.id;
        name = cstd.name;
        level = cstd.level;
        needs = cstd.needs;
        value = cstd.value;
        return *this;
    }

	UInt16 id;
    std::string name;
	UInt8  level;
	UInt32 needs;
    float value;
};

typedef std::vector<ClanSkillTableData>		SingleClanSkillTable;
typedef std::vector<SingleClanSkillTable>	ClanSkillTable;

extern ClanSkillTable clanSkillTable;

}

#endif

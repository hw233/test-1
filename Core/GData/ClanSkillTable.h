#ifndef CLANSKILLTABLE_INC
#define CLANSKILLTABLE_INC

#include <string>

namespace GData
{

struct ClanSkillTableData
{
	ClanSkillTableData(UInt16 id = 0, const std::string& name = std::string(),
            UInt8 level = 0, UInt32 needs = 0, UInt32 hp = 0, UInt32 attack = 0,
            UInt32 defend = 0, UInt32 magatk = 0, UInt32 magdef = 0)
        : id(id), name(name), level(level), needs(needs), hp(hp),
        attack(attack), defend(defend), magatk(magatk), magdef(magdef) {} 

    ClanSkillTableData(const ClanSkillTableData& cstd)
    {
        id = cstd.id;
        name = cstd.name;
        level = cstd.level;
        needs = cstd.needs;
        hp = cstd.hp;
        attack = cstd.attack;
        defend = cstd.defend;
        magatk = cstd.magatk;
        magdef = cstd.magdef;
    }

    ClanSkillTableData& operator= (const ClanSkillTableData& cstd)
    {
        id = cstd.id;
        name = cstd.name;
        level = cstd.level;
        needs = cstd.needs;
        hp = cstd.hp;
        attack = cstd.attack;
        defend = cstd.defend;
        magatk = cstd.magatk;
        magdef = cstd.magdef;
        return *this;
    }

	UInt16 id;
    std::string name;
	UInt8  level;
	UInt32 needs;
    UInt32 hp;
    UInt32 attack;
    UInt32 defend;
    UInt32 magatk;
    UInt32 magdef;
};

typedef std::vector<ClanSkillTableData>		SingleClanSkillTable;
typedef std::vector<SingleClanSkillTable>	ClanSkillTable;

extern ClanSkillTable clanSkillTable;

}

#endif

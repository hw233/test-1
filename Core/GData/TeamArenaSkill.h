#ifndef _TEAMARENASKILL_H_
#define _TEAMARENASKILL_H_

#include "AttrExtra.h"
#include "GDataDBExecHelper.h"

namespace GData
{

struct DBTeamArenaSkillConfig;
struct DBTeamArenaInspireConfig;

struct TeamArenaSkillData
{
	TeamArenaSkillData()
		: id(0), level(0), needs(0), teamLev(0) {}

	UInt16 id;
	UInt8  level;
	UInt32 needs;
	UInt8  teamLev;
    AttrExtra attrs;
};

struct TeamArenaInspire
{
    TeamArenaInspire()
        : lvl(0), consume(0), rate(0) {}

    UInt8 lvl;
    UInt32 consume;
    UInt32 rate;
    AttrExtra attrs;
};

class TeamArenaConfigMgr
{
    public:
        TeamArenaConfigMgr() {}
        ~TeamArenaConfigMgr() {}

        static void LoadSkillFromDB(DBTeamArenaSkillConfig&);
        static void LoadInspireFromDB(DBTeamArenaInspireConfig&);
};

typedef std::vector<TeamArenaSkillData>   SingleTeamArenaSkill;
typedef std::vector<SingleTeamArenaSkill> TeamArenaSkill;
typedef std::map<UInt8, TeamArenaInspire> TeamInspireMap;

extern TeamArenaSkill teamArenaSkill;
extern TeamInspireMap teamInspireMap;

}

#endif

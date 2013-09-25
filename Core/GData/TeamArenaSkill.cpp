#include "Config.h"
#include "TeamArenaSkill.h"

namespace GData
{

TeamArenaSkill teamArenaSkill;
TeamInspireMap teamInspireMap;

void TeamArenaConfigMgr::LoadSkillFromDB(DBTeamArenaSkillConfig& dbtasc)
{
    if (dbtasc.id <= 0)
        return;
    if (dbtasc.id >= teamArenaSkill.size())
        teamArenaSkill.resize(dbtasc.id + 1);
    SingleTeamArenaSkill& single = teamArenaSkill[dbtasc.id];
    if (dbtasc.level >= single.size())
        single.resize(dbtasc.level + 1);

    TeamArenaSkillData tas;
    tas.id = dbtasc.id;
    tas.level = dbtasc.level;
    tas.needs = dbtasc.needs;
    tas.teamLev = dbtasc.teamLev;
    tas.attrs.attack = dbtasc.attack;
    tas.attrs.magatk = dbtasc.attack;
    tas.attrs.hp = dbtasc.hp;
    tas.attrs.defend = dbtasc.defend;
    tas.attrs.magdef = dbtasc.magdef;
    tas.attrs.action = dbtasc.action;
    tas.attrs.magresP = dbtasc.magresP;

    single[dbtasc.level] = tas;
}

void TeamArenaConfigMgr::LoadInspireFromDB(DBTeamArenaInspireConfig& dbtaic)
{
    if (dbtaic.level <= 0)
        return;

    TeamArenaInspire tai;
    tai.lvl = dbtaic.level;
    tai.consume = dbtaic.consume;
    tai.rate = dbtaic.rate;
    tai.attrs.attackP = dbtaic.attackP;
    tai.attrs.magatkP = dbtaic.attackP;
    tai.attrs.defendP = dbtaic.defendP;
    tai.attrs.magdefP = dbtaic.defendP;
    tai.attrs.hpP = dbtaic.hpP;
    tai.attrs.actionP = dbtaic.actionP;

    teamInspireMap.insert(std::make_pair(tai.lvl, tai));
}

}

#include "Config.h"
#include "GObject/Fighter.h"
#include "ClanAssistantData.h"

namespace GData
{

ObjectListT<ClanAssistant> clanAssistants;
ClanRobMonster clanRobMonster;

UInt8 ClanAssistant::getLevel() const
{
	return _assistList.empty() ? 0 : _assistList[0].fighter->getLevel();
}

UInt8 ClanAssistant::getClass() const
{
	return _assistList.empty() ? 0 : _assistList[0].fighter->getClass();
}

}


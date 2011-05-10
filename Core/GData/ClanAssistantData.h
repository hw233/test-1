#ifndef _CLANASSISTANTDATA_H_
#define _CLANASSISTANTDATA_H_

#include "ObjectManager.h"
#include "Common/URandom.h"

namespace GObject
{
	class Fighter;
}

namespace GData
{

class NpcGroup;

struct AssistantFData
{
	AssistantFData(GObject::Fighter * f = NULL, UInt8 fn = 0, UInt8 pb = 0, UInt8 pe = 4)
		: fighter(f), fighterNum(fn) { }
	
	GObject::Fighter * fighter;
	UInt8 fighterNum;
	std::vector<UInt8> pos;
};

struct ClanAssistant : public ObjectBaseT<>
{
	ClanAssistant(UInt32 id): ObjectBaseT<>(id), formated(false) {}
	UInt32 id;
	UInt8  formated;
	std::vector<AssistantFData> _assistList;

	UInt8 getLevel() const;
	UInt8 getClass() const;
};

struct ClanRobMonster
{
	ClanRobMonster() : _clanSouthGuarderBoss(NULL), _clanNorthGuarderBoss(NULL), _clanTotemGuarderBoss(NULL) {}

	std::vector<NpcGroup *> _clanTotemAssistBoss;
	std::vector<NpcGroup *> _clanSouthBoss;
	std::vector<NpcGroup *> _clanNorthBoss;
	NpcGroup * _clanSouthGuarderBoss;
	NpcGroup * _clanNorthGuarderBoss;
	NpcGroup * _clanTotemGuarderBoss;
};

extern ObjectListT<ClanAssistant> clanAssistants;
extern ClanRobMonster    clanRobMonster;

}

#endif
#ifndef _DUNGEONDATA_H_
#define _DUNGEONDATA_H_

#include "ObjectManager.h"
#include "Common/URandom.h"

namespace GObject
{
	class Player;
	class Fighter;
}

namespace GData
{

struct LootItem;
struct LootResult;

#define DUNGEON_DIFFICULTY_MAX 2

struct DungeonMonster: public ObjectBaseT<>
{
	DungeonMonster(UInt32 id): ObjectBaseT<>(id), formated(false), exp(0), fighter(NULL), minNum(0), maxNum(0) {}
	bool formated;
	UInt32 exp;
	GObject::Fighter * fighter;
	UInt8 minNum;
	UInt8 maxNum;

	std::map<GObject::Fighter *, std::vector<UInt8> > monsterPos;
};

struct DungeonLevel
{
	const DungeonMonster * monsterSet;
	std::vector<const LootItem *> loots;
	
	void getLoot(GObject::Player *, UInt32, UInt8 = 0) const;
};

struct DungeonData: public ObjectBaseT<>
{
	DungeonData(UInt32 id): ObjectBaseT<>(id) {}
	virtual ~DungeonData();
	UInt16 location;
	UInt8 type;
	UInt8 levelReq;
	std::vector<const DungeonLevel *> monsters[DUNGEON_DIFFICULTY_MAX];
};

extern ObjectListT<DungeonMonster> dungeonMonsters;
extern ObjectListT<DungeonData> dungeons;

}

#endif // _DUNGEONDATA_H_
